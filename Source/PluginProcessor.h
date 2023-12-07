/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Samples.h"
#include "MultiSampleSynthesiser.h"
#include "MultiIntensitySynthesiser.h"

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
    void noteOnSynthesiser(int midiNoteNumber, float velocity);

private:
    //==============================================================================
    
    juce::AudioFormatManager mFormatManager;

    std::unique_ptr<juce::Synthesiser> mBassDrum1Synthesiser;
    std::unique_ptr<juce::Synthesiser> mElectricSnareSynthesiser;

    std::unique_ptr<MultiSampleSynthesiser> mHandClapSynthesiser;
    std::unique_ptr<MultiSampleSynthesiser> mTambourineSynthesiser;
    std::unique_ptr<MultiSampleSynthesiser> mCowbellSynthesiser;
    std::unique_ptr<MultiSampleSynthesiser> mMaracaSynthesiser;
    std::unique_ptr<MultiSampleSynthesiser> mTriangleSynthesiser;

    std::unique_ptr<MultiIntensitySynthesiser> mHighTomSynthesiser;
    std::unique_ptr<MultiIntensitySynthesiser> mSideStickSynthesiser;
    std::unique_ptr<MultiIntensitySynthesiser> mHighFloorTomSynthesiser;

    std::unique_ptr<juce::Synthesiser> mAcousticBassDrumSynthesiser;
    std::unique_ptr<juce::Synthesiser> mAcousticSnareSynthesiser;
    std::unique_ptr<juce::Synthesiser> mCrashCymbal1Synthesiser;
    std::unique_ptr<juce::Synthesiser> mRideCymbal1Synthesiser;
    std::unique_ptr<juce::Synthesiser> mChineseCymbalSynthesiser;
    std::unique_ptr<juce::Synthesiser> mRideBellSynthesiser;
    std::unique_ptr<juce::Synthesiser> mSplashCymbalSynthesiser;
    std::unique_ptr<juce::Synthesiser> mCrashCymbal2Synthesiser;

    std::unique_ptr<juce::Synthesiser> mHiHatSynthesiser;
    
    void initializeSimpleInstrument(samples::InstrumentEnum instrument, juce::Synthesiser& synthesizer);
    void initializeVariedInstrument(samples::InstrumentEnum instrument, MultiSampleSynthesiser& synthesizer);
    void initializeIntensityInstrument(samples::InstrumentEnum instrument, MultiIntensitySynthesiser& synthesizer);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ABKit2AudioProcessor)
};
