/*
 ==============================================================================
 
 CLROHSamplerVoice.h
 Created: 8 Dec 2023 5:42:13pm
 Author:  paulm
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "./Configuration/Samples.h"

class PluginSamplerVoice : public juce::SynthesiserVoice
{
public:
    PluginSamplerVoice(
        juce::RangedAudioParameter& gainParameter,
        juce::RangedAudioParameter& panParameter,
        juce::AudioParameterBool& phaseParameter
    );
    ~PluginSamplerVoice() override;
    
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;

private:
    juce::RangedAudioParameter& mGainParameter;
    juce::RangedAudioParameter& mPanParameter;
    juce::AudioParameterBool& mPhaseParameter;

    float mVelocityGain = 0;
    double mPitchRatio = 0;
    double mSourceSamplePosition = 0;
    juce::ADSR mAdsr;
    float mPan = 0;
    
    JUCE_LEAK_DETECTOR(PluginSamplerVoice)
};
