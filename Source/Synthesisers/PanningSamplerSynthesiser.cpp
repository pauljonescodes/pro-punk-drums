/*
  ==============================================================================

	CLROHSynthesiser.cpp
	Created: 8 Dec 2023 5:21:07pm
	Author:  paulm

  ==============================================================================
*/

#include "CLRSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PanningSamplerVoice.h"

PanningSamplerSynthesiser::PanningSamplerSynthesiser()
{
	setNoteStealingEnabled(false);
}

void PanningSamplerSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	auto hasSoundThatAppliesToNote = false;

	for (auto* sound : sounds)
	{
		if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
		{
			hasSoundThatAppliesToNote = true;
			break;
		}
	}

	if (hasSoundThatAppliesToNote) {
		for (const auto& sampleInfoEntry : mSampleInfoMap) {
			auto& sampleInfo = sampleInfoEntry.second;
			auto& intensities = sampleInfo.intensities;
			
			// Calculating the position in the context of total intensity levels
			float position = velocity * (intensities.size() - 1);

			// Determining the lower and higher indices for intensity levels
			int lowerIntensityIndex = static_cast<int>(position);
			int higherIntensityIndex = lowerIntensityIndex + 1;

			if (lowerIntensityIndex >= intensities.size()) {
				lowerIntensityIndex = 0;
			}

			if (higherIntensityIndex >= intensities.size()) {
				higherIntensityIndex = lowerIntensityIndex; 
			}

			float blendRatio = position - lowerIntensityIndex;
			float perceivedBlendRatio = lowerIntensityIndex != higherIntensityIndex ? std::pow(blendRatio, 0.33) : 1.0f;

			auto& lowerIntensity = intensities[lowerIntensityIndex];
			auto samplesSize = lowerIntensity.variations.size();

			if (lowerIntensity.currentVariationIndex < samplesSize) {
				auto& sound = lowerIntensity.variations[lowerIntensity.currentVariationIndex];
				if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
				{
					stopVoice(lowerIntensity.voice.get(), 1.0f, true);
					startVoice(lowerIntensity.voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
				}

				// Sorry
				mSampleInfoMap[sampleInfoEntry.first].intensities[lowerIntensityIndex].currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
			}

			if (lowerIntensityIndex != higherIntensityIndex) {
				auto& higherIntensity = intensities[higherIntensityIndex];
				auto& sound = higherIntensity.variations[higherIntensity.currentVariationIndex];
				if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
				{
					stopVoice(higherIntensity.voice.get(), 1.0f, true);
					startVoice(higherIntensity.voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
				}

				// Sorry
				mSampleInfoMap[sampleInfoEntry.first].intensities[higherIntensityIndex].currentVariationIndex = (higherIntensity.currentVariationIndex + 1) % samplesSize;
			}
		}
	}
}

void PanningSamplerSynthesiser::addSample(
	const std::string name,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	const std::string micId,
	const int intensityIndex,
	const float defaultStereoPan,
	juce::AudioFormatManager& audioFormatManager
) {
	juce::BigInteger range;
	range.setRange(midiNote, 1, true);
	int dataSizeInBytes;

	const char* sourceData = BinaryData::getNamedResource(name.c_str(), dataSizeInBytes);
	auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
	juce::AudioFormatReader* reader = audioFormatManager.createReaderFor(std::move(memoryInputStream));

	double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
	PanningSamplerSound* sound = new PanningSamplerSound(juce::String(name), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);

	addSound(sound);

	auto& sampleIntensities = mSampleInfoMap[micId];

	// Ensure there's at least one SampleVariations instance for the base intensity level
	if (sampleIntensities.intensities.empty() || intensityIndex >= sampleIntensities.intensities.size()) {
		sampleIntensities.intensities.emplace_back(); 
	}

	auto& variationInfo = sampleIntensities.intensities[intensityIndex]; 

	if (!variationInfo.voice) {
		variationInfo.voice = std::make_unique<PanningSamplerVoice>(defaultStereoPan);
		addVoice(variationInfo.voice.get());
	}

	// Add the new sound to the variations of the base intensity
	variationInfo.variations.push_back(PanningSamplerSound::Ptr(sound));
}

void PanningSamplerSynthesiser::addSample(
	const std::string name,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	const std::string micId,
	const float defaultStereoPan,
	juce::AudioFormatManager& audioFormatManager
) {
	addSample(name, bitRate, bitDepth, midiNote, micId, 0, defaultStereoPan, audioFormatManager);
}

void PanningSamplerSynthesiser::addSample(
	const std::string name,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	juce::AudioFormatManager& audioFormatManager
) {
	addSample(name, bitRate, bitDepth, midiNote, "defaultMicId-1234", 0, audioFormatManager);
}