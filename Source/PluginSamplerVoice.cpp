
#include <JuceHeader.h>
#include "PluginSamplerVoice.h"
#include "PluginSamplerSound.h"

//==============================================================================
PluginSamplerVoice::PluginSamplerVoice(
    juce::RangedAudioParameter& gainParameter,
    juce::RangedAudioParameter& panParameter,
    juce::AudioParameterBool& phaseParameter
) :
    mGainParameter(gainParameter), 
    mPanParameter(panParameter), 
    mPhaseParameter(phaseParameter)
{ }

PluginSamplerVoice::~PluginSamplerVoice() {}

bool PluginSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<const PluginSamplerSound*> (sound) != nullptr;
}

void PluginSamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    if (auto* sound = dynamic_cast<const PluginSamplerSound*> (s))
    {
        mSourceSamplePosition = 0.0;
        
        mVelocityGain = 0.2f + velocity * 0.8f;
        
        mAdsr.setSampleRate(sound->mSourceSampleRate);
        mAdsr.setParameters(sound->mAdsrParameters);
        mAdsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void PluginSamplerVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        mAdsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        mAdsr.reset();
    }
}

void PluginSamplerVoice::pitchWheelMoved(int newValue) {}
void PluginSamplerVoice::controllerMoved(int controllerNumber, int newValue) {}

//==============================================================================
void PluginSamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = static_cast<PluginSamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        auto& data = *playingSound->mData;
        const float* const inL = data.getReadPointer(0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;
        
        float* outL = outputBuffer.getWritePointer(0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;
        
        while (--numSamples >= 0)
        {
            auto pos = (int)mSourceSamplePosition;
            auto alpha = (float)(mSourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;
            
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;
            
            auto envelopeValue = mAdsr.getNextSample();
            
            float pan = mPanParameter.getValue();
            float panLeft = pan <= 0.0f ? 1.0f : 1.0f - pan;
            float panRight = pan >= 0.0f ? 1.0f : 1.0f + pan;
            
            float phaseMultiplier = mPhaseParameter.get() ? -1 : 1;
            float gain = mGainParameter.getValue();
            
            l *= panLeft * envelopeValue * gain * mVelocityGain * phaseMultiplier;
            r *= panRight * envelopeValue * gain * mVelocityGain * phaseMultiplier;
            
            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }
            
            mSourceSamplePosition += 1.0f;
            
            if (mSourceSamplePosition > playingSound->mLength)
            {
                stopNote(0.0f, false);
                break;
            }
        }
    }
}
