/*
  ==============================================================================

	CLROHSynthesiser.cpp
	Created: 8 Dec 2023 5:21:07pm
	Author:  paulm

  ==============================================================================
*/

#include "PanningSamplerSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PanningSamplerVoice.h"

PanningSamplerSynthesiser::PanningSamplerSynthesiser()
{
	setNoteStealingEnabled(false);
}

void PanningSamplerSynthesiser::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) {

}

void PanningSamplerSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end()) {
		auto& intensities = mMidiNoteToInstruments.at(midiNoteNumber).intensities;
		float position = velocity * (intensities.size() - 1);

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
			auto& variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];
			auto& sound = variation.sound;
			auto& voice = variation.voice;

			if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
			{
				stopVoice(voice.get(), 1.0f, true);
				startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
			}

			lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
		}

		if (lowerIntensityIndex != higherIntensityIndex) {
			auto& higherIntensity = intensities[higherIntensityIndex];
			auto& variation = higherIntensity.variations[higherIntensity.currentVariationIndex];
			auto& sound = variation.sound;
			auto& voice = variation.voice;

			if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
			{
				stopVoice(voice.get(), 1.0f, true);
				startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
			}

			higherIntensity.currentVariationIndex = (higherIntensity.currentVariationIndex + 1) % samplesSize;
		}
	}
}

void PanningSamplerSynthesiser::addSample(
	const std::string resourceName,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	const int intensityIndex,
	const float defaultStereoPan,
	juce::AudioFormatManager& audioFormatManager
) {
	juce::BigInteger range;
	range.setRange(midiNote, 1, true);
	int dataSizeInBytes;

	const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
	auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
	juce::AudioFormatReader* reader = audioFormatManager.createReaderFor(std::move(memoryInputStream));

	double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
	PanningSamplerSound* sound = new PanningSamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);

	addSound(sound);

	auto& instrument = mMidiNoteToInstruments[midiNote];

	// Ensure there's at least one SampleVariations instance for the base intensity level
	if (instrument.intensities.empty() || intensityIndex >= instrument.intensities.size()) {
		instrument.intensities.emplace_back();
	}

	auto& intensity = instrument.intensities[intensityIndex];

	auto variation = Variation(PanningSamplerSound::Ptr(sound), std::make_unique<PanningSamplerVoice>(defaultStereoPan));
	addVoice(variation.voice.get());
	intensity.variations.push_back(std::move(variation));
}

void PanningSamplerSynthesiser::addSample(
	const std::string resourceName,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	const float defaultStereoPan,
	juce::AudioFormatManager& audioFormatManager
) {
	addSample(resourceName, bitRate, bitDepth, midiNote, 0, defaultStereoPan, audioFormatManager);
}

void PanningSamplerSynthesiser::addSample(
	const std::string resourceName,
	const int bitRate,
	const int bitDepth,
	const int midiNote,
	juce::AudioFormatManager& audioFormatManager
) {
	addSample(resourceName, bitRate, bitDepth, midiNote, 0, audioFormatManager);
}