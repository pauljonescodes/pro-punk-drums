/*
  ==============================================================================

	SamplerComponent.h
	Created: 8 Dec 2023 10:49:23am
	Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DrumsComponent : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
public:
	DrumsComponent(std::vector<int> midiNotesVector);
	~DrumsComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked; 

private:
	juce::OwnedArray<juce::TextButton> mMidiNoteButtons;
	std::unique_ptr <juce::Slider> mVelocitySlider;
	std::unique_ptr <juce::Label> mVelocityLabel;

	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsComponent)

};
