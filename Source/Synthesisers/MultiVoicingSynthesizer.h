/*
  ==============================================================================

    MultiVoicingSynthesizer.h
    Created: 7 Dec 2023 7:57:27pm
    Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MultiVoicingSynthesiser : public juce::Synthesiser {
public:
	virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void addSamplerSound(const juce::SamplerSound::Ptr& newSound, int midiNoteNumber);
protected:
    std::vector<int> mMidiNoteNumbers;
    std::map<int, std::pair<int, int>> mSoundVariations; // midi note number -> count:current
};
