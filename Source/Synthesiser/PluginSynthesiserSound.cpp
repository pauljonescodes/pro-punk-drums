#include "PluginSynthesiserSound.h"

PluginSynthesiserSound::PluginSynthesiserSound(const juce::String& soundName,
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
        
        mAdsrParameters.attack = static_cast<float> (attackTimeSecs);
        mAdsrParameters.release = static_cast<float> (releaseTimeSecs);
    }
}

PluginSynthesiserSound::~PluginSynthesiserSound()
{
}

bool PluginSynthesiserSound::appliesToNote(int midiNoteNumber)
{
    return mMidiNotes[midiNoteNumber];
}

bool PluginSynthesiserSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}
