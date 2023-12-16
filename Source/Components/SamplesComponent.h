/*
  ==============================================================================

    SamplesComponent.h
    Created: 16 Dec 2023 12:32:59am
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SamplesParameterComponent.h"

class SamplesComponent : public juce::Component
{
public:
	SamplesComponent(const std::vector<int> midiNotesVector, juce::AudioProcessorValueTreeState& apvts);
	~SamplesComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	juce::Viewport viewport;
	juce::Component container;
	juce::OwnedArray<SamplesParameterComponent> mSamplesParameterComponents;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesComponent)
};
