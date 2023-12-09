/*
  ==============================================================================

    IntensitySynthesizer.h
    Created: 7 Dec 2023 2:10:12pm
    Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class MultiIntensitySynthesiser : public juce::Synthesiser {
public:
    virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void addSamplerSoundAtIntensityIndex(const juce::SamplerSound::Ptr& newSound, int index);
protected:
	std::vector<int> mCLRToCurrentVariationIndices;
    std::vector<int> mCLRToIntensitySizes;
};
