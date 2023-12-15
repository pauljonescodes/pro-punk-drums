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
    PluginSamplerVoice();
    ~PluginSamplerVoice() override;
    
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;
    
private:
    double mPitchRatio = 0;
    double mSourceSamplePosition = 0;
    juce::ADSR mAdsr;
    float mPan = 0;
    float mVelocityGain = 1.0f;
    
    JUCE_LEAK_DETECTOR(PluginSamplerVoice)
};
