#pragma once
#include <JuceHeader.h>
#include "../Configuration/Samples.h"

class PluginSynthesiserVoice : public juce::SynthesiserVoice
{
public:
    PluginSynthesiserVoice(
        juce::RangedAudioParameter &gainParameter,
        juce::RangedAudioParameter &panParameter,
        juce::AudioParameterBool &phaseParameter);
    ~PluginSynthesiserVoice() override;

    bool canPlaySound(juce::SynthesiserSound *) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;

private:
    juce::RangedAudioParameter &mGainParameter;
    juce::RangedAudioParameter &mPanParameter;
    juce::AudioParameterBool &mInvertPhaseParameter;

    double mPitchRatio = 0;
    float mVelocityGain = 0;
    double mSourceSamplePosition = 0;
    juce::ADSR mAdsr;

    JUCE_LEAK_DETECTOR(PluginSynthesiserVoice)
};
