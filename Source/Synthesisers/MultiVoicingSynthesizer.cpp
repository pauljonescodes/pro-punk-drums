/*
  ==============================================================================

    MultiVoicingSynthesizer.cpp
    Created: 7 Dec 2023 7:57:27pm
    Author:  paulm

  ==============================================================================
*/

#include "MultiVoicingSynthesizer.h"

// Ignore velocity
void MultiVoicingSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    auto& variationInfo = mSoundVariations[midiNoteNumber];
    auto midiNoteNumberIterator = std::find(mMidiNoteNumbers.begin(), mMidiNoteNumbers.end(), midiNoteNumber);
    int midiNoteNumberIndex = std::distance(mMidiNoteNumbers.begin(), midiNoteNumberIterator);
   
    auto soundIndex = (midiNoteNumberIndex * variationInfo.first) + variationInfo.second;
    auto sound = sounds[soundIndex].get(); // assume uniform and contiguous

    if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
    {
        for (auto* voice : voices) {
            if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
                stopVoice(voice, 1.0f, true);
            }
        }

        startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, true), sound, midiChannel, midiNoteNumber, 0.1f + velocity * 0.9f);
        variationInfo.second = (variationInfo.second + 1) % variationInfo.first;
    }
}

void MultiVoicingSynthesiser::addSamplerSound(const juce::SamplerSound::Ptr& newSound, int midiNoteNumber) {
    // Add the MIDI note number to mMidiNoteNumbers if it's not already present
    if (std::find(mMidiNoteNumbers.begin(), mMidiNoteNumbers.end(), midiNoteNumber) == mMidiNoteNumbers.end()) {
        mMidiNoteNumbers.push_back(midiNoteNumber);
    }

    // Update the count for this MIDI note number in mSoundVariations
    auto& variationInfo = mSoundVariations[midiNoteNumber];
    variationInfo.first++;  // Increment the count of variations
    // The second element of the pair can be left as is, or set to some default value if needed

    // Add the new sound to the synthesizer
    addSound(newSound);
}
