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

class VaryingCLRSynthesiser : public juce::Synthesiser {

public:
	VaryingCLRSynthesiser();

	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
	void addCLRSamplerSound(const PanningSamplerSound::Ptr& newSound, const samples::CenterLeftRightEnum micId);

protected:
	std::unique_ptr<PanningSamplerVoice> mCenterVoice;
	std::unique_ptr<PanningSamplerVoice> mLeftVoice;
	std::unique_ptr<PanningSamplerVoice> mRightVoice;

	std::pair<int, std::vector<PanningSamplerSound::Ptr>> mCenterSamples;
	std::pair<int, std::vector<PanningSamplerSound::Ptr>> mLeftSamples;
	std::pair<int, std::vector<PanningSamplerSound::Ptr>> mRightSamples;
};
