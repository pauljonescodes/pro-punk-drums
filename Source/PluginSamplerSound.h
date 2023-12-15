/*
 ==============================================================================
 
 CLRSamplerSound.h
 Created: 9 Dec 2023 11:30:35am
 Author:  paulm
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "./Configuration/Samples.h"

class PluginSamplerSound : public juce::SynthesiserSound
{
public:
    
    PluginSamplerSound(const juce::String& name,
                        juce::AudioFormatReader& source,
                        const juce::BigInteger& midiNotes,
                        int midiNoteForNormalPitch,
                        double attackTimeSecs,
                        double releaseTimeSecs,
                        double maxSampleLengthSeconds);
    
    /** Destructor. */
    ~PluginSamplerSound() override;
    
    const juce::String& getName() const noexcept { return mName; }
    juce::AudioBuffer<float>* getAudioData() const noexcept { return mData.get(); }
    void setEnvelopeParameters(juce::ADSR::Parameters parametersToUse) { mParams = parametersToUse; }
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
    
    double mSourceSampleRate;
    int mLength = 0;
    std::unique_ptr<juce::AudioBuffer<float>> mData;
    juce::ADSR::Parameters mParams;
    juce::String mName;
    juce::BigInteger mMidiNotes;
    int mMidiRootNote = 0;
    
    JUCE_LEAK_DETECTOR(PluginSamplerSound)
};
