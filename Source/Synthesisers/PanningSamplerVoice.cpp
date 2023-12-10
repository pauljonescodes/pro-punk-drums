
#include <JuceHeader.h>
#include "PanningSamplerVoice.h"
#include "PanningSamplerSound.h"

//==============================================================================
PanningSamplerVoice::PanningSamplerVoice(float pan) : mPan(pan) {}

PanningSamplerVoice::~PanningSamplerVoice() {}

bool PanningSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<const PanningSamplerSound*> (sound) != nullptr;
}

void PanningSamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    if (auto* sound = dynamic_cast<const PanningSamplerSound*> (s))
    {
        mSourceSamplePosition = 0.0;
        
        mVelocityGain = 0.1f + velocity * 0.9f;
        
        mAdsr.setSampleRate(sound->mSourceSampleRate);
        mAdsr.setParameters(sound->mParams);
        mAdsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void PanningSamplerVoice::stopNote(float /*velocity*/, bool allowTailOff)
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

void PanningSamplerVoice::pitchWheelMoved(int newValue) {}
void PanningSamplerVoice::controllerMoved(int controllerNumber, int newValue) {}

//==============================================================================
void PanningSamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = static_cast<PanningSamplerSound*> (getCurrentlyPlayingSound().get()))
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
            
            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;
            
            auto envelopeValue = mAdsr.getNextSample();
            
            float panLeft = mPan <= 0.0f ? 1.0f : 1.0f - mPan;
            float panRight = mPan >= 0.0f ? 1.0f : 1.0f + mPan;
            
            l *= panLeft * envelopeValue * mVelocityGain;
            r *= panRight * envelopeValue * mVelocityGain;
            
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
