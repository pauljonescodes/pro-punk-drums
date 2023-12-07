/*
  ==============================================================================

	MultiSampleSynthesizer.h
	Created: 6 Dec 2023 8:46:48pm
	Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MultiSampleSynthesiser : public juce::Synthesiser {
public:
	MultiSampleSynthesiser() : mCurrentSoundIndex(0) {}
	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
protected:
	int mCurrentSoundIndex;
};
