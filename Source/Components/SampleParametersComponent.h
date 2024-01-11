#pragma once
#include <JuceHeader.h>
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"
#include "../Configuration/Strings.h"
#include "../Configuration/GeneralMidi.h"

class SamplesParametersComponent : public juce::Component, public juce::Button::Listener
{
public:
	SamplesParametersComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState &apvts)
		: mMicId(micId), mApvts(apvts)
	{
		mGroupComponentPtr.reset(new juce::GroupComponent(std::to_string(midiNote) + "_label", GeneralMidiPercussion::midiNoteToNameMap.at(midiNote) + " " + stringToTitleCase(micId)));
		addAndMakeVisible(mGroupComponentPtr.get());

		mNoteOnButtonPtr.reset(new juce::TextButton(std::to_string(midiNote) + " " + micId));
		mNoteOnButtonPtr->setComponentID(juce::String(midiNote));
		mNoteOnButtonPtr->addListener(this);
		addAndMakeVisible(mNoteOnButtonPtr.get());

		// Initialize gain slider and label
		auto gainParameterId = stringsJoinAndSnakeCase({std::to_string(midiNote), micId, AudioParameters::gainComponentId});
		mRatioSliderPtr.reset(new juce::Slider(stringToTitleCase(gainParameterId)));
		mRatioSliderPtr->setScrollWheelEnabled(false);
		mRatioSliderPtr->setTextValueSuffix(Strings::db);
		addAndMakeVisible(mRatioSliderPtr.get());
		mGainAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
			apvts,
			juce::String(gainParameterId),
			*mRatioSliderPtr));

		auto panParameterId = stringsJoinAndSnakeCase({std::to_string(midiNote), micId, AudioParameters::panComponentId});
		mPanSliderPtr.reset(new juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
		mPanSliderPtr->setScrollWheelEnabled(false);
		mPanSliderPtr->setTextValueSuffix(" " + Strings::pan);
		addAndMakeVisible(mPanSliderPtr.get());
		mPanAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
			apvts,
			juce::String(panParameterId),
			*mPanSliderPtr));

		auto phaseParameterId = stringsJoinAndSnakeCase({std::to_string(midiNote), micId, AudioParameters::phaseComponentId});
		mInvertPhaseToggleButtonPtr.reset(new juce::ToggleButton(Strings::invertPhase));
		addAndMakeVisible(mInvertPhaseToggleButtonPtr.get());
		mInvertPhaseAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
			apvts,
			phaseParameterId,
			*mInvertPhaseToggleButtonPtr));
	}

	~SamplesParametersComponent() override
	{
		mNoteOnButtonPtr->removeListener(this);

		mGainAttachmentPtr.reset();
		mPanAttachmentPtr.reset();
		mInvertPhaseAttachmentPtr.reset();

		// Now it's safe to reset the sliders and buttons
		mRatioSliderPtr.reset();
		mPanSliderPtr.reset();
		mInvertPhaseToggleButtonPtr.reset();
		mNoteOnButtonPtr.reset();
		mGroupComponentPtr.reset();
	};

	void resized() override
	{
		int padding = 10;
		int toggleSize = 100;
		int labelHeight = 15;

		auto area = getLocalBounds().reduced(padding);

		mGroupComponentPtr->setBounds(getLocalBounds());

		mInvertPhaseToggleButtonPtr->setBounds(area.removeFromLeft(toggleSize).reduced(padding));
		mPanSliderPtr->setBounds(area.removeFromLeft(toggleSize).reduced(padding));

		auto buttonArea = area.removeFromRight(area.getHeight()).reduced(padding);

		mRatioSliderPtr->setBounds(area);
		mNoteOnButtonPtr->setBounds(buttonArea);
	};

	void paint(juce::Graphics &g) override
	{
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	}

	std::optional<std::function<void(int, float, std::string)>> mOnDrumMidiButtonClicked;

private:
	std::string mMicId;

	std::unique_ptr<juce::GroupComponent> mGroupComponentPtr;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachmentPtr, mPanAttachmentPtr;
	std::unique_ptr<juce::Slider> mRatioSliderPtr, mPanSliderPtr;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mInvertPhaseAttachmentPtr;
	std::unique_ptr<juce::ToggleButton> mInvertPhaseToggleButtonPtr;

	std::unique_ptr<juce::TextButton> mNoteOnButtonPtr;

	juce::AudioProcessorValueTreeState &mApvts;

	void buttonClicked(juce::Button *button) override
	{
		const juce::String componentID = button->getComponentID();
		int midiNoteValue = componentID.getIntValue();

		if (mOnDrumMidiButtonClicked.has_value())
		{
			mOnDrumMidiButtonClicked.value()(midiNoteValue, 0.75f, mMicId);
		}
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesParametersComponent)
};
