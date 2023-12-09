/*
  ==============================================================================

    MultiMicrophoneSynthesiser.h
    Created: 7 Dec 2023 5:55:11pm
    Author:  paulm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class MultiMicrophoneSynthesiser : public juce::Synthesiser {
public:
    virtual void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    void addCLRSamplerSound(const juce::SamplerSound::Ptr& newSound, int intensityIndex, const std::string& micId);
protected:
    std::map<std::string, std::vector<int>> mCLRToCurrentVariationIndices;
    std::map<std::string, int> mCLRToIntensitySizes;

};
