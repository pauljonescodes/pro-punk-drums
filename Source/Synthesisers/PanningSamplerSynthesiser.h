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

class PanningSamplerSynthesiser : public juce::Synthesiser {

public:
	PanningSamplerSynthesiser();

	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
	
	void addSample(
		const std::string name,
		const int bitRate,
		const int bitDepth,
		const int midiNote,
		juce::AudioFormatManager& audioFormatManager
	);

	void addSample(
		const std::string name, 
		const int bitRate, 
		const int bitDepth, 
		const int midiNote, 
		const std::string micId, 
		const float defaultStereoPan,
		juce::AudioFormatManager& audioFormatManager
	);

	void addSample(
		const std::string name,
		const int bitRate,
		const int bitDepth,
		const int midiNote,
		const std::string micId,
		const int intensityIndex,
		const float defaultStereoPan,
		juce::AudioFormatManager& audioFormatManager
	);

protected:
	struct Variations {
		int currentVariationIndex;
		std::unique_ptr<PanningSamplerVoice> voice;
		std::vector<PanningSamplerSound::Ptr> variations;

		Variations() : currentVariationIndex(0) {}
	};

	struct Intensities {
		std::vector<Variations> intensities;
		Intensities() {}
	};

	std::map<std::string, Intensities> mSampleInfoMap;
};
