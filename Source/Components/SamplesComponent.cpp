#include "SamplesComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Constants.h"
#include "SamplesParameterComponent.h"
#include "../Configuration/GeneralMidi.h"

SamplesComponent::SamplesComponent(const std::vector<int> midiNotesVector, juce::AudioProcessorValueTreeState& apvts)
{
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&container, false);
    
    for (const auto& pair : PluginUtils::getUniqueMidiNoteMicCombinations()) {
        int midiNote = pair.first;
        std::string midiName = generalmidi::midiNoteToNameMap.at(midiNote);
        const std::set<std::string>& micIds = pair.second;
        
        for (const std::string& micId : micIds) {
            auto* component = new SamplesParameterComponent(midiNote, micId, apvts);
            mSamplesParameterComponents.add(component);
            container.addAndMakeVisible(component);
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
    viewport.setBounds(getLocalBounds());
    int heightPerComponent = 100;
    int totalHeight = heightPerComponent * mSamplesParameterComponents.size();
    
    container.setBounds(0, 0, viewport.getWidth(), totalHeight);
    
    auto area = container.getLocalBounds();
    for (auto* component : mSamplesParameterComponents)
    {
        component->setBounds(area.removeFromTop(heightPerComponent));
    }
}
