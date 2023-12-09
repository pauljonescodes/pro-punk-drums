/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Configuration/Samples.h"
#include "Synthesisers/MultiSampleSynthesiser.h"
#include "Synthesisers/MultiIntensitySynthesiser.h"
#include "Synthesisers/MultiMicrophoneSynthesiser.h"
#include "Synthesisers/MultiVoicingSynthesizer.h"

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
    
    juce::AudioFormatManager mFormatManager;

    std::vector<std::unique_ptr<juce::Synthesiser>> synthesisers;

    std::unique_ptr<MultiVoicingSynthesiser> mTriangleSynthesiser;

    std::unique_ptr<juce::Synthesiser> mHiHatSynthesiser;
    
    void addMultiVoicedSounds(samples::InstrumentEnum instrument, MultiVoicingSynthesiser& synthesizer);
    void addVariedSounds(samples::InstrumentEnum instrument, MultiSampleSynthesiser& synthesizer);
    void addMultiMicMultiIntensitySounds(samples::InstrumentEnum instrument, MultiMicrophoneSynthesiser& synthesizer, std::vector<std::string> micIds, std::vector<std::string> intensityIds);    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ABKit2AudioProcessor)
};
