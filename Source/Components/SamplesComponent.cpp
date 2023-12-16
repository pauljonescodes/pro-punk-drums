/*
  ==============================================================================

    SamplesComponent.cpp
    Created: 16 Dec 2023 12:32:59am
    Author:  paulm

  ==============================================================================
*/

#include "SamplesComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Constants.h"
#include "SamplesParameterComponent.h"

SamplesComponent::SamplesComponent(const std::vector<int> midiNotesVector, juce::AudioProcessorValueTreeState& apvts)
{
    for (int midiNote : midiNotesVector)
    {
        auto gainParameterId = PluginUtils::getParamId(midiNote, "", constants::gainId);
        auto* gainParameter = apvts.getParameter(gainParameterId);
        if (gainParameter != nullptr) {
            auto* component = new SamplesParameterComponent(midiNote, apvts);
            mSamplesParameterComponents.add(component);
            addAndMakeVisible(component);
        }
       
    }
}

SamplesComponent::~SamplesComponent()
{
    // OwnedArray will automatically clean up the components it owns
}

void SamplesComponent::paint(juce::Graphics& g)
{
    // Custom painting code here
}

void SamplesComponent::resized()
{
    auto area = getLocalBounds(); // Get the entire area of this component
    int heightPerComponent = 100;

    for (auto* component : mSamplesParameterComponents)
    {
        component->setBounds(area.removeFromTop(heightPerComponent));
    }
}