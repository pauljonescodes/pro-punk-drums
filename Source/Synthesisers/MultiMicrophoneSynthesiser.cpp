/*
  ==============================================================================

    MultiMicrophoneSynthesiser.cpp
    Created: 7 Dec 2023 5:55:11pm
    Author:  paulm

  ==============================================================================
*/

#include "MultiMicrophoneSynthesiser.h"
#include "../Configuration/Samples.h"

void MultiMicrophoneSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    int numVariations = samples::standardVariationsCount;
    int micIndex = 0;

    for (const auto& micEntry : mCLRToCurrentVariationIndices) {
        const std::string& micId = micEntry.first;
       
        int numIntensities = mCLRToIntensitySizes[micId];
        
        // Check if there are intensity levels defined for this mic ID
        if (mCLRToIntensitySizes.find(micId) == mCLRToIntensitySizes.end()) continue;

        // Calculating the position in the context of total intensity levels for this mic ID
        int totalIntensities = mCLRToIntensitySizes[micId];
        float position = velocity * (totalIntensities - 1);

        // Determining the lower and higher indices for intensity levels
        int lowerIndex = static_cast<int>(position);
        int higherIndex = lowerIndex + 1;

        if (higherIndex >= totalIntensities) {
            higherIndex = lowerIndex; // If at the end, only use the highest intensity
        }

        // Calculating the blend ratio
        float blendRatio = position - lowerIndex;
        float perceivedBlendRatio = std::pow(blendRatio, 0.33);

        // Calculating the specific sample indices within the intensity levels
        int totalSamplesPerMic = numIntensities * numVariations;

        // Calculate base index for lower and higher intensity levels
        int baseLowerIndex = (micIndex * totalSamplesPerMic) + (lowerIndex * numVariations);
        int baseHigherIndex = (micIndex * totalSamplesPerMic) + (higherIndex * numVariations);

        int lowerSampleIndex = mCLRToCurrentVariationIndices[micId][lowerIndex] % mCLRToIntensitySizes[micId];
        int higherSampleIndex = mCLRToCurrentVariationIndices[micId][higherIndex] % mCLRToIntensitySizes[micId];

        // Calculate the actual sound index in the 'sounds' array for lower and higher intensity
        int lowerSoundIndex = baseLowerIndex + lowerSampleIndex;
        int higherSoundIndex = (lowerIndex != higherIndex) ? baseHigherIndex + higherSampleIndex : lowerSoundIndex;

        // Retrieve the corresponding sounds for lower and higher intensities
        juce::SynthesiserSound* lowerIntensitySound = sounds[baseLowerIndex].get();
        juce::SynthesiserSound* higherIntensitySound = sounds[baseHigherIndex].get();

        // Starting voices for lower and higher intensity samples
        if (lowerIntensitySound && lowerIntensitySound->appliesToNote(midiNoteNumber) && lowerIntensitySound->appliesToChannel(midiChannel)) {
            for (auto* voice : voices) {
                if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
                    stopVoice(voice, 1.0f, true);
                }
            }
            
            
            startVoice(voices[lowerIndex], lowerIntensitySound, midiChannel, midiNoteNumber, 1.0 - perceivedBlendRatio);

            if (lowerIndex != higherIndex && blendRatio != 0) {
                startVoice(voices[higherIndex], higherIntensitySound, midiChannel, midiNoteNumber, perceivedBlendRatio);
            }

            // Update round-robin indices
            mCLRToCurrentVariationIndices[micId][lowerIndex]++;
            if (lowerIndex != higherIndex) {
                mCLRToCurrentVariationIndices[micId][higherIndex]++;
            }
        }

        micIndex++;
    }
}

void MultiMicrophoneSynthesiser::addCLRSamplerSound(const juce::SamplerSound::Ptr& newSound, int intensityIndex, const std::string& micId) {
    addSound(newSound);

    // Ensure the micId entry exists in mCurrentSoundIndices and mIntensitySizes
    if (mCLRToCurrentVariationIndices.find(micId) == mCLRToCurrentVariationIndices.end()) {
        mCLRToCurrentVariationIndices[micId] = std::vector<int>();
        mCLRToIntensitySizes[micId] = 0;
    }

    // Check if this is a new intensity level for this micId
    if (intensityIndex >= mCLRToCurrentVariationIndices[micId].size()) {
        mCLRToCurrentVariationIndices[micId].push_back(0);  // Initialize round-robin index for this new intensity
        mCLRToIntensitySizes[micId] = intensityIndex + 1;  // Update the size (number of samples) for this intensity
    }
    else {
        // Existing intensity level, increment the round-robin index
        mCLRToCurrentVariationIndices[micId][intensityIndex]++;
    }
    DBG("done");
}
