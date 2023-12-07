/*
  ==============================================================================

    IntensitySynthesizer.cpp
    Created: 7 Dec 2023 2:10:12pm
    Author:  paulm

  ==============================================================================
*/

#include "MultiIntensitySynthesiser.h"

void MultiIntensitySynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    // Calculating the position in the context of total intensity levels
    float position = velocity * (mIntensitySizes.size() - 1);

    // Determining the lower and higher indices for intensity levels
    int lowerIndex = static_cast<int>(position);
    int higherIndex = lowerIndex + 1;

    if (higherIndex >= mIntensitySizes.size()) {
        higherIndex = lowerIndex; // If at the end, only use the highest intensity
    }

    // Calculating the blend ratio
    float blendRatio = position - lowerIndex;

    // Calculating the specific sample indices within the intensity levels
    int lowerSampleIndex = mCurrentSoundIndices[lowerIndex] % mIntensitySizes[lowerIndex];
    int higherSampleIndex = mCurrentSoundIndices[higherIndex] % mIntensitySizes[higherIndex];

    // Calculate the actual sound index in the 'sounds' array for lower intensity
    int lowerSoundIndex = std::accumulate(mIntensitySizes.begin(), mIntensitySizes.begin() + lowerIndex, 0)
        + lowerSampleIndex;

    // Calculate the actual sound index in the 'sounds' array for higher intensity
    int higherSoundIndex = (lowerIndex != higherIndex) ?
        std::accumulate(mIntensitySizes.begin(), mIntensitySizes.begin() + higherIndex, 0)
        + higherSampleIndex : lowerSoundIndex;

    // Retrieve the corresponding sounds for lower and higher intensities
    juce::SynthesiserSound* lowerIntensitySound = sounds[lowerSoundIndex].get();
    juce::SynthesiserSound* higherIntensitySound = sounds[higherSoundIndex].get();

    for (auto* voice : voices) {
        if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
            stopVoice(voice, 1.0f, true);
        }
    }

    // Starting voices for lower and higher intensity samples
    if (lowerIntensitySound && lowerIntensitySound->appliesToNote(midiNoteNumber) && lowerIntensitySound->appliesToChannel(midiChannel)) {
        startVoice(voices[lowerIndex], lowerIntensitySound, midiChannel, midiNoteNumber, 1.0 - blendRatio);
        
        if (lowerIndex != higherIndex) {
            startVoice(voices[higherIndex], higherIntensitySound, midiChannel, midiNoteNumber, blendRatio);
        }

        // Update round-robin indices
        mCurrentSoundIndices[lowerIndex]++;
        if (lowerIndex != higherIndex) {
            mCurrentSoundIndices[higherIndex]++;
        }
    }
}

void MultiIntensitySynthesiser::addSamplerSoundAtIntensityIndex(const juce::SamplerSound::Ptr& newSound, int index) {
    addSound(newSound);

    if (index >= mCurrentSoundIndices.size()) {
        mCurrentSoundIndices.push_back(0); // Initialize round-robin index for this new intensity
        mIntensitySizes.push_back(1);      // Initialize size (number of samples) for this intensity
    } else {
        // Existing intensity level, just add a new sample to it
        mIntensitySizes[index]++;
    }
}
