#pragma once

#include <JuceHeader.h>
#include "PluginAudioProcessor.h"
#include "PluginPresetManager.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include "Components/OutputsComponent.h"

class PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PluginAudioProcessorEditor(PluginAudioProcessor&);
    ~PluginAudioProcessorEditor() override;
    
    
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    PluginAudioProcessor& mAudioProcessor;
    
    std::unique_ptr<juce::TabbedComponent> mTabbedComponentPtr;
    std::unique_ptr<PresetComponent> mPresetComponentPtr;
    std::unique_ptr<DrumsComponent> mDrumsComponentPtr;
    std::unique_ptr<SamplesComponent> mSamplesComponentPtr;
    std::unique_ptr<OutputsComponent> mOutputsComponentPtr;
    juce::OwnedArray<juce::TextButton> mMidiNoteButtonsOwnedArray;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginAudioProcessorEditor)
};
