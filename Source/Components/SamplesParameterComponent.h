/*
  ==============================================================================

	SamplesParameterComponent.h
	Created: 16 Dec 2023 12:02:19am
	Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class SamplesParameterComponent : public juce::Component
{
public:
	SamplesParameterComponent(int midiNote, juce::AudioProcessorValueTreeState& apvts);
	~SamplesParameterComponent() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;

private:

	std::unique_ptr <juce::Slider> mGainSlider;
	std::unique_ptr <juce::Label> mGainLabel;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;


	std::unique_ptr <juce::Slider> mPanSlider;
	std::unique_ptr <juce::Label> mPanLabel;

	std::unique_ptr <juce::ToggleButton> mInvertPhaseToggleButton;
	std::unique_ptr <juce::Label> mInvertPhaseLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesParameterComponent)
};
