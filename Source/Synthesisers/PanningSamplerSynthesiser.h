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
    
    void addSample(const std::string resourceName,
                   const int bitRate,
                   const int bitDepth,
                   const int midiNote,
                   const std::vector<int> stopsMidiNotes,
                   const int intensityIndex,
                   const int variationIndex,
                   const float defaultStereoPan,
                   juce::AudioFormatManager& audioFormatManager);
    
protected:
    
    struct Microphone {
        PanningSamplerSound::Ptr sound;
        std::unique_ptr<PanningSamplerVoice> voice;
        
        Microphone(PanningSamplerSound::Ptr s, std::unique_ptr<PanningSamplerVoice> v)
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
        std::vector<Intensity> intensities;
        std::vector<int> stopsMidiNotes;
        
        Instrument() {
            intensities.emplace_back();
        }
    };
    
    std::unordered_map<int, Instrument> mMidiNoteToInstruments;
};
