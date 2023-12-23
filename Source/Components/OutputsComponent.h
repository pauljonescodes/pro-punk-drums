#pragma once
#include <JuceHeader.h>
#include "OutputParametersComponent.h"

class OutputsComponent : public juce::Component
{
public:
    OutputsComponent(juce::AudioProcessorValueTreeState& apvts, std::function<void(int, float, std::string)> onDrumMidiButtonClicked);
    ~OutputsComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Viewport viewport;
    juce::Component container;
    juce::OwnedArray<OutputParametersComponent> mComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputsComponent)
};
