#pragma once
#include <JuceHeader.h>
#include "SampleParametersComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"
#include "../Configuration/GeneralMidi.h"

class SamplesComponent : public juce::Component
{
public:
    SamplesComponent(
        const std::vector<int> midiNotesVector,
        juce::AudioProcessorValueTreeState &apvts,
        std::function<void(int, float, std::string)> onDrumMidiButtonClicked)
    {
        addAndMakeVisible(mViewport);
        mViewport.setViewedComponent(&mContainer, false);
        mViewport.setScrollBarsShown(true, false, true, false);

        for (const auto &pair : AudioParameters::getUniqueMidiNoteMicCombinations())
        {
            int midiNote = pair.first;
            std::string midiName = GeneralMidiPercussion::midiNoteToNameMap.at(midiNote);
            const std::set<std::string> &micIds = pair.second;

            for (const std::string &micId : micIds)
            {
                auto *component = new SamplesParametersComponent(midiNote, micId, apvts);
                component->mOnDrumMidiButtonClicked = onDrumMidiButtonClicked;
                mComponents.add(component);
                mContainer.addAndMakeVisible(component);
            }
        }
    };

    void resized() override
    {
        mViewport.setBounds(getLocalBounds());

        int heightPerComponent = 144;
        int totalHeight = heightPerComponent * mComponents.size();

        mContainer.setBounds(0, 0, mViewport.getMaximumVisibleWidth() - 8, totalHeight);

        auto area = mContainer.getLocalBounds();
        for (auto *component : mComponents)
        {
            component->setBounds(area.removeFromTop(heightPerComponent).reduced(0, 12));
        }
    };

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    };

private:
    juce::OwnedArray<SamplesParametersComponent> mComponents;
    juce::Viewport mViewport;
    juce::Component mContainer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesComponent)
};
