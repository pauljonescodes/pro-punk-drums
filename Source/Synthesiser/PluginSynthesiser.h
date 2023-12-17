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
#include "PluginSynthesiserVoice.h"
#include "PluginSynthesiserSound.h"
#include "../Configuration/Samples.h"

class PluginSynthesiser : public juce::Synthesiser {

public:
	PluginSynthesiser();

	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
	virtual void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override;

	void addSample(
		const std::string resourceName,
		const int bitRate,
		const int bitDepth,
		const int midiNote,
		const std::vector<int> stopsMidiNotes,
		const int intensityIndex,
		const int variationIndex,
		juce::AudioFormatManager& audioFormatManager,
		juce::RangedAudioParameter& gainParameter,
		juce::RangedAudioParameter& panParameter,
		juce::AudioParameterBool& phaseParameter
	);

	std::vector<int> getMidiNotesVector();

protected:

	struct Microphone {
		PluginSynthesiserSound::Ptr sound;
		std::unique_ptr<PluginSynthesiserVoice> voice;

		Microphone(PluginSynthesiserSound::Ptr s, std::unique_ptr<PluginSynthesiserVoice> v)
			: sound(std::move(s)), voice(std::move(v))
		{}
	};

	struct Variation {
		std::vector<Microphone> microphones;

		Variation() {}
	};

	struct Intensity {
		int currentVariationIndex;
		std::vector<Variation> variations;

		Intensity() : currentVariationIndex(0) { }
	};

	struct Instrument {
		std::vector<Intensity> velocities;
		std::vector<int> stopsMidiNotes;

		Instrument() {
			velocities.emplace_back();
		}
	};

	std::map<int, Instrument> mMidiNoteToInstruments;
};
