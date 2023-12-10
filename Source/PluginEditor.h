/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/SamplerComponent.h"

//==============================================================================
/**
 */
class ABKit2AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ABKit2AudioProcessorEditor(ABKit2AudioProcessor&);
    ~ABKit2AudioProcessorEditor() override;
    
    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ABKit2AudioProcessor& mAudioProcessor;
    
    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> mTabbedComponent;
    std::unique_ptr<SamplerComponent> mSamplerComponent;
    juce::OwnedArray<juce::TextButton> mMidiNoteButtons;
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ABKit2AudioProcessorEditor)
};
