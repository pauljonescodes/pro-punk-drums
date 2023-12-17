/*
 ==============================================================================
 
 CLROHSamplerVoice.h
 Created: 8 Dec 2023 5:42:13pm
 Author:  paulm
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "../Configuration/Samples.h"

class PluginSynthesiserVoice : public juce::SynthesiserVoice
{
public:
    PluginSynthesiserVoice(
        juce::RangedAudioParameter& gainParameter,
        juce::AudioParameterBool& phaseParameter
    );
    ~PluginSynthesiserVoice() override;
    
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;

private:
    juce::RangedAudioParameter& mGainParameter;
    juce::AudioParameterBool& mInvertPhaseParameter;

    float mVelocityGain = 0;
    double mSourceSamplePosition = 0;
    juce::ADSR mAdsr;
    
    JUCE_LEAK_DETECTOR(PluginSynthesiserVoice)
};
