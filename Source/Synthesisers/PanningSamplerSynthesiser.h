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
	virtual void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override;
	
	void addSample(
		const std::string resourceName,
		const int bitRate,
		const int bitDepth,
		const int midiNote,
		juce::AudioFormatManager& audioFormatManager
	);

	void addSample(
		const std::string resourceName,
		const int bitRate, 
		const int bitDepth, 
		const int midiNote, 
		const float defaultStereoPan,
		juce::AudioFormatManager& audioFormatManager
	);

	void addSample(
		const std::string resourceName,
		const int bitRate,
		const int bitDepth,
		const int midiNote,
		const int intensityIndex,
		const float defaultStereoPan,
		juce::AudioFormatManager& audioFormatManager
	);

protected:
	
	// on
	struct Variation {
		PanningSamplerSound::Ptr sound;
		std::unique_ptr<PanningSamplerVoice> voice;
		
		Variation(PanningSamplerSound::Ptr s, std::unique_ptr<PanningSamplerVoice> v)
			: sound(std::move(s)), voice(std::move(v))
		{}
	};

	// medium
	struct Intensity {
		int currentVariationIndex;

		// 1
		std::vector<Variation> variations;

		Intensity() : currentVariationIndex(0) {
			
		}
	};

	// acoustic_bass_drum
	struct Instrument {
		std::vector<Intensity> intensities;

		Instrument() {
			intensities.emplace_back();
		}
	};

	std::unordered_map<int, Instrument> mMidiNoteToInstruments;
};
