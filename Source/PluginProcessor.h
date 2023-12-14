/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Configuration/Samples.h"
#include "Synthesisers/PanningSamplerSynthesiser.h"

//==============================================================================
/**
 */
class ABKit2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ABKit2AudioProcessor();
    ~ABKit2AudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void noteOnSynthesisers(int midiNoteNumber, float velocity);

private:
    //==============================================================================
    
    juce::AudioFormatManager mAudioFormatManager;
    std::unique_ptr<PanningSamplerSynthesiser> mSynthesiserPtr;
    std::unique_ptr<juce::AudioBuffer<float>> mInternalBufferPtr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ABKit2AudioProcessor)
};
