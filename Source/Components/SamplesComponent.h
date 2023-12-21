#pragma once
#include <JuceHeader.h>
#include "SamplesParameterComponent.h"

class SamplesComponent : public juce::Component
{
public:
    SamplesComponent(const std::vector<int> midiNotesVector, juce::AudioProcessorValueTreeState& apvts, std::function<void(int, float, std::string)> onDrumMidiButtonClicked);
    ~SamplesComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    juce::Viewport viewport;
    juce::Component container;
    juce::OwnedArray<SamplesParameterComponent> mSamplesParameterComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesComponent)
};
