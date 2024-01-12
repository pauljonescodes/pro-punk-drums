#pragma once

#include <JuceHeader.h>
#include "PresetComponent.h"
#include "../PluginPresetManager.h"
#include "../Configuration/Samples.h"
#include "../Configuration/GeneralMidi.h"
#include "../Configuration/Strings.h"
#include "../Configuration/Parameters.h"

class DrumsComponent : public juce::Component, public juce::Button::Listener
{
public:
	DrumsComponent(const std::vector<int> midiNotesVector)
	{
		mVelocitySlider = std::make_unique<juce::Slider>();
		mVelocitySlider->setRange(0, 127, 1);
		mVelocitySlider->setValue(100);
		addAndMakeVisible(mVelocitySlider.get());

		mVelocityLabel = std::make_unique<juce::Label>();
		mVelocityLabel->setText(Strings::velocity, juce::dontSendNotification);
		mVelocityLabel->attachToComponent(mVelocitySlider.get(), true);
		addAndMakeVisible(mVelocityLabel.get());

		for (int note : midiNotesVector)
		{
			juce::TextButton *button = new juce::TextButton(GeneralMidiPercussion::midiNoteToNameMap.at(note) + "\n" + juce::String(note).toStdString() + " " + juce::MidiMessage::getMidiNoteName(note, true, true, 4).toStdString());
			button->setComponentID(juce::String(note));
			mMidiNoteButtonsOwnedArray.add(button);
			addAndMakeVisible(button);
			button->addListener(this);
		}

		resized();
	};

	void paint(juce::Graphics &g) override
	{
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	};

	void resized() override
	{
		const auto localBounds = getLocalBounds();
		// auto sampleControls = localBounds;

		auto sampleControlHeight = 0; // 50;

		// sampleControls.setHeight(sampleControlHeight);

		// mVelocitySlider->setBounds(sampleControls.withTrimmedLeft(localBounds.proportionOfWidth(0.1)));

		// Layout for the MIDI note buttons
		int numRows = 4; //std::max(1, (localBounds.getHeight()) / 128);
		int numCols = 5; //std::max(1, localBounds.getWidth() / 128);
		int buttonWidth = localBounds.getWidth() / numCols;
		int buttonHeight = (localBounds.getHeight() - sampleControlHeight) / numRows;

		for (int i = 0; i < mMidiNoteButtonsOwnedArray.size(); ++i)
		{
			int row = i / numCols;
			int col = i % numCols;
			mMidiNoteButtonsOwnedArray[i]->setBounds(col * buttonWidth, row * buttonHeight + sampleControlHeight, buttonWidth, buttonHeight);
		}
	};

	std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;

private:
	juce::OwnedArray<juce::TextButton> mMidiNoteButtonsOwnedArray;

	std::unique_ptr<juce::Slider> mVelocitySlider;
	std::unique_ptr<juce::Label> mVelocityLabel;

	void buttonClicked(juce::Button *button) override
	{
		const juce::String componentID = button->getComponentID();
		int midiNoteValue = componentID.getIntValue();
		float velocity = mVelocitySlider->getValue() / 127.0f;

		if (mOnDrumMidiButtonClicked.has_value())
		{
			mOnDrumMidiButtonClicked.value()(midiNoteValue, velocity);
		}
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsComponent)
};
