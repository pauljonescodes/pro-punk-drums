/*
 ==============================================================================
 
 CLRSamplerSound.cpp
 Created: 9 Dec 2023 11:30:35am
 Author:  paulm
 
 ==============================================================================
 */

#include "PluginSamplerSound.h"

PluginSamplerSound::PluginSamplerSound(const juce::String& soundName,
                                         juce::AudioFormatReader& source,
                                         const juce::BigInteger& notes,
                                         int midiNoteForNormalPitch,
                                         double attackTimeSecs,
                                         double releaseTimeSecs,
                                         double maxSampleLengthSeconds) :
mSourceSampleRate(source.sampleRate),
mName(soundName),
mMidiNotes(notes),
mMidiRootNote(midiNoteForNormalPitch)
{
    if (mSourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        mLength = juce::jmin((int)source.lengthInSamples, (int)(maxSampleLengthSeconds * mSourceSampleRate));
        
        mData.reset(new juce::AudioBuffer<float>(juce::jmin(2, (int)source.numChannels), mLength + 4));
        
        source.read(mData.get(), 0, mLength + 4, 0, true, true);
        
        mParams.attack = static_cast<float> (attackTimeSecs);
        mParams.release = static_cast<float> (releaseTimeSecs);
    }
}

PluginSamplerSound::~PluginSamplerSound()
{
}

bool PluginSamplerSound::appliesToNote(int midiNoteNumber)
{
    return mMidiNotes[midiNoteNumber];
}

bool PluginSamplerSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}
