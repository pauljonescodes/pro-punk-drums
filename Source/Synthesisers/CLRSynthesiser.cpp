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

CLRSynthesiser::CLRSynthesiser()
{
	setNoteStealingEnabled(false);
}

void CLRSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
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
			auto samplesSize = sampleInfo.samples.size();
			if (sampleInfo.currentVariationIndex < samplesSize) {
				auto currentVariationIndex = sampleInfo.currentVariationIndex;
				auto& sound = sampleInfo.samples[currentVariationIndex];
				if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
				{
					stopVoice(sampleInfo.voice.get(), 1.0f, true);
					startVoice(sampleInfo.voice.get(), sound.get(), midiChannel, midiNoteNumber, velocity);
				}

				mSampleInfoMap[sampleInfoEntry.first].currentVariationIndex = (currentVariationIndex + 1) % samplesSize;
			}
		}
	}
}

void CLRSynthesiser::addCLRSamplerSound(const PanningSamplerSound::Ptr& newSound, const std::string micId, const float defaultPan) {
	addSound(newSound);

	auto& sampleInfo = mSampleInfoMap[micId];
	if (!sampleInfo.voice) {
		sampleInfo.voice = std::make_unique<PanningSamplerVoice>(defaultPan);
		addVoice(sampleInfo.voice.get());
	}

	sampleInfo.samples.push_back(newSound);
}
