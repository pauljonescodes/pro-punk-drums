#pragma once
#include <JuceHeader.h>

class OutputParametersComponent : public juce::Component, public juce::Button::Listener, private juce::Timer
{
public:
	OutputParametersComponent(int channelIndex, juce::AudioProcessorValueTreeState& apvts);
	~OutputParametersComponent() override;


	void paint(juce::Graphics&) override;
	void resized() override;

	std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;

private:
	bool mIsPlaying = false;
	int mMidiNoteValue = -1;
	void timerCallback() override;

	juce::AudioProcessorValueTreeState& mApvts;
	std::unique_ptr <juce::Label> mLabel;

	std::unique_ptr <juce::ToggleButton> mCompressionOnToggleButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mCompressionOnAttachment;

	std::unique_ptr <juce::Slider> mThresholdSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mThresholdAttachment;
	std::unique_ptr <juce::Label> mThresholdLabel;

	std::unique_ptr <juce::Slider> mRatioSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mRatioAttachment;
	std::unique_ptr <juce::Label> mRatioLabel;

	std::unique_ptr <juce::Slider> mAttackSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment;
	std::unique_ptr <juce::Label> mAttackLabel;

	std::unique_ptr <juce::Slider> mReleaseSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mReleaseAttachment;
	std::unique_ptr <juce::Label> mReleaseLabel;

	//

	std::unique_ptr <juce::ToggleButton> mHighPassOnToggleButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mHighPassOnAttachment;

	std::unique_ptr <juce::Slider> mHighPassFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHighPassFrequencyAttachment;
	std::unique_ptr <juce::Label> mHighPassFrequencyLabel;

	std::unique_ptr<juce::Slider> mHighPassQualitySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHighPassQualityAttachment;
	std::unique_ptr<juce::Label> mHighPassQualityLabel;

	//

	std::unique_ptr<juce::ToggleButton> mPeakFilterOnToggleButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mPeakFilterOnAttachment;

	std::unique_ptr<juce::Slider> mPeakFilterFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPeakFilterFrequencyAttachment;
	std::unique_ptr<juce::Label> mPeakFilterFrequencyLabel;

	std::unique_ptr<juce::Slider> mPeakFilterQualitySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPeakFilterQualityAttachment;
	std::unique_ptr<juce::Label> mPeakFilterQualityLabel;

	std::unique_ptr<juce::Slider> mPeakFilterGainSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPeakFilterGainAttachment;
	std::unique_ptr<juce::Label> mPeakFilterGainLabel;

	//

	std::unique_ptr <juce::ToggleButton> mLowPassOnToggleButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mLowPassOnAttachment;

	std::unique_ptr <juce::Slider> mLowPassFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mLowPassFrequencyAttachment;
	std::unique_ptr <juce::Label> mLowPassFrequencyLabel;

	std::unique_ptr<juce::Slider> mLowPassQualitySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mLowPassQualityAttachment;
	std::unique_ptr<juce::Label> mLowPassQualityLabel;

	//

	std::unique_ptr <juce::Slider> mChannelGainSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mChannelGainAttachment;
	std::unique_ptr <juce::Label> mChannelGainLabel;

	std::unique_ptr <juce::TextButton> mNoteOnButton;

	void buttonClicked(juce::Button* button) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputParametersComponent)
};
