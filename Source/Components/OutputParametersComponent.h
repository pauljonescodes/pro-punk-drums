#pragma once
#include <JuceHeader.h>
#include "TitledFrequencyQualityGainComponent.h"
#include "CompressionWithGainComponent.h"

class OutputParametersComponent : public juce::Component, public juce::Button::Listener, private juce::Timer
{
public:
	OutputParametersComponent(int channelIndex, juce::AudioProcessorValueTreeState& apvts);

	void paint(juce::Graphics&) override;
	void resized() override;

	std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;

private:
	bool mIsPlaying = false;
	int mMidiNoteValue = -1;
	void timerCallback() override;

	juce::AudioProcessorValueTreeState& mApvts;

	std::unique_ptr<juce::GroupComponent> mGroupComponentPtr;

	std::unique_ptr<CompressionWithGainComponent> mCompressionWithGainComponent;

	std::unique_ptr<TitledFrequencyQualityGainComponent> mLowShelfComponentPtr, mMidPeakComponentPtr, mHighShelfComponentPtr;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mChannelGainAttachmentPtr;
	std::unique_ptr<juce::Slider> mChannelGainSliderPtr;
	std::unique_ptr<juce::Label> mChannelGainLabelPtr;

	std::unique_ptr<juce::TextButton> mNoteOnButtonPtr;

	void buttonClicked(juce::Button* button) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputParametersComponent)
};
