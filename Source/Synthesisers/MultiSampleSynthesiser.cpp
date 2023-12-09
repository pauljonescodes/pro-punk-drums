/*
  ==============================================================================

    MultiSampleSynthesizer.cpp
    Created: 6 Dec 2023 8:46:48pm
    Author:  paulm

  ==============================================================================
*/

#include "MultiSampleSynthesiser.h"

// Ignore velocity
void MultiSampleSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity) 
{
    auto sound = sounds[mCurrentSoundIndex].get();

    if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
    {
        // If hitting a note that's still ringing, stop it first (it could be
        // still playing because of the sustain or sostenuto pedal).
        for (auto* voice : voices) {
            if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
                stopVoice(voice, 1.0f, true);
            }
        }

        startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, true), sound, midiChannel, midiNoteNumber, 0.1f + velocity * 0.9f);
    }

    mCurrentSoundIndex = (mCurrentSoundIndex + 1) % sounds.size();
}