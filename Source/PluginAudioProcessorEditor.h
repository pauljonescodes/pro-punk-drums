#pragma once

#include <JuceHeader.h>
#include "PluginAudioProcessor.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"

class PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PluginAudioProcessorEditor(PluginAudioProcessor&);
    ~PluginAudioProcessorEditor() override;
    
    
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginAudioProcessor& mAudioProcessor;
    
    
    std::unique_ptr<juce::TabbedComponent> mTabbedComponent;
    std::unique_ptr<DrumsComponent> mDrumsComponent;
    std::unique_ptr<SamplesComponent> mSamplesComponent;
    juce::OwnedArray<juce::TextButton> mMidiNoteButtons;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginAudioProcessorEditor)
};
