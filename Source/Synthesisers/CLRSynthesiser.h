/*
  ==============================================================================

	CLROHSynthesiser.h
	Created: 8 Dec 2023 5:21:07pm
	Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "PanningSamplerVoice.h"
#include "PanningSamplerSound.h"
#include "../Configuration/Samples.h"

struct PannedSampleInfo {
	int currentVariationIndex;
	std::unique_ptr<PanningSamplerVoice> voice;
	std::vector<PanningSamplerSound::Ptr> samples;

	PannedSampleInfo() : currentVariationIndex(0) {}
};

class CLRSynthesiser : public juce::Synthesiser {

public:
	CLRSynthesiser();

	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
	void addCLRSamplerSound(const PanningSamplerSound::Ptr& newSound, const std::string micId, const float defaultPan);

protected:

	std::map<std::string, PannedSampleInfo> mSampleInfoMap;
};
