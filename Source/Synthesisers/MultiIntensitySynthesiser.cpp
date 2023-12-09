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
    float position = velocity * (mCLRToIntensitySizes.size() - 1);

    // Determining the lower and higher indices for intensity levels
    int lowerIndex = static_cast<int>(position);
    int higherIndex = lowerIndex + 1;

    if (higherIndex >= mCLRToIntensitySizes.size()) {
        higherIndex = lowerIndex; // If at the end, only use the highest intensity
    }

    // Calculating the blend ratio
    float blendRatio = position - lowerIndex;
    float perceivedBlendRatio = std::pow(blendRatio, 0.33);

    // Calculating the specific sample indices within the intensity levels
    int lowerSampleIndex = mCLRToCurrentVariationIndices[lowerIndex] % mCLRToIntensitySizes[lowerIndex];
    int higherSampleIndex = mCLRToCurrentVariationIndices[higherIndex] % mCLRToIntensitySizes[higherIndex];

    // Calculate the actual sound index in the 'sounds' array for lower intensity
    int lowerSoundIndex = std::accumulate(mCLRToIntensitySizes.begin(), mCLRToIntensitySizes.begin() + lowerIndex, 0)
        + lowerSampleIndex;

    // Calculate the actual sound index in the 'sounds' array for higher intensity
    int higherSoundIndex = (lowerIndex != higherIndex) ?
        std::accumulate(mCLRToIntensitySizes.begin(), mCLRToIntensitySizes.begin() + higherIndex, 0)
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
        startVoice(voices[lowerIndex], lowerIntensitySound, midiChannel, midiNoteNumber, 1.0 - perceivedBlendRatio);
        
        if (lowerIndex != higherIndex) {
            startVoice(voices[higherIndex], higherIntensitySound, midiChannel, midiNoteNumber, perceivedBlendRatio);
        }

        // Update round-robin indices
        mCLRToCurrentVariationIndices[lowerIndex]++;
        if (lowerIndex != higherIndex) {
            mCLRToCurrentVariationIndices[higherIndex]++;
        }
    }
}

void MultiIntensitySynthesiser::addSamplerSoundAtIntensityIndex(const juce::SamplerSound::Ptr& newSound, int index) {
    addSound(newSound);

    if (index >= mCLRToCurrentVariationIndices.size()) {
        mCLRToCurrentVariationIndices.push_back(0); // Initialize round-robin index for this new intensity
        mCLRToIntensitySizes.push_back(1);      // Initialize size (number of samples) for this intensity
    } else {
        // Existing intensity level, just add a new sample to it
        mCLRToIntensitySizes[index]++;
    }
}
