
#include <JuceHeader.h>
#include "PanningSamplerVoice.h"
#include "PanningSamplerSound.h"

//==============================================================================
PanningSamplerVoice::PanningSamplerVoice(
    juce::AudioBuffer<float>& targetBuffer, 
    int targetBufferChannel) : 
    mTargetBuffer(targetBuffer), 
    mTargetBufferChannel(targetBufferChannel) {}

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
void PanningSamplerVoice::renderNextBlock(juce::AudioBuffer<float>& /*outputBuffer*/, int startSample, int numSamples)
{
    if (auto* playingSound = dynamic_cast<PanningSamplerSound*>(getCurrentlyPlayingSound().get()))
    {
        auto& data = *playingSound->mData;
        const float* const inMono = data.getReadPointer(0);

        float* outMono = mTargetBuffer.getWritePointer(mTargetBufferChannel, startSample);

        while (--numSamples >= 0)
        {
            auto pos = (int)mSourceSamplePosition;
            auto alpha = (float)(mSourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            float sampleValue = (inMono[pos] * invAlpha + inMono[pos + 1] * alpha) * mVelocityGain * mAdsr.getNextSample();

            *outMono++ += sampleValue;

            mSourceSamplePosition += 1.0f;

            if (mSourceSamplePosition > playingSound->mLength)
            {
                stopNote(0.0f, false);
                break;
            }
        }
    }
}
