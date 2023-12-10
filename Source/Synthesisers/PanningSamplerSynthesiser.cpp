/*
 ==============================================================================
 
 CLROHSynthesiser.cpp
 Created: 8 Dec 2023 5:21:07pm
 Author:  paulm
 
 ==============================================================================
 */

#include "PanningSamplerSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PanningSamplerVoice.h"

PanningSamplerSynthesiser::PanningSamplerSynthesiser()
{
    setNoteStealingEnabled(false);
}

void PanningSamplerSynthesiser::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) {
    
}

void PanningSamplerSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end()) {
        auto& instrument = mMidiNoteToInstruments.at(midiNoteNumber);
        
        for (auto& voice : voices) {
            for (int stopsMidiNote : instrument.stopsMidiNotes) {
                auto currentlyPlayingSounds = voice->getCurrentlyPlayingSound();
                if (currentlyPlayingSounds != nullptr && currentlyPlayingSounds->appliesToNote(stopsMidiNote)) {
                    voice->stopNote(1, true);
                }
            }
        }
        
        auto& intensities = instrument.intensities;
        float position = velocity * (intensities.size() - 1);
        
        int lowerIntensityIndex = static_cast<int>(position);
        int higherIntensityIndex = lowerIntensityIndex + 1;
        
        if (lowerIntensityIndex >= intensities.size()) {
            lowerIntensityIndex = 0;
        }
        
        if (higherIntensityIndex >= intensities.size() || velocity == 0) {
            higherIntensityIndex = lowerIntensityIndex;
        }
        
        float blendRatio = position - lowerIntensityIndex;
        float perceivedBlendRatio = lowerIntensityIndex != higherIntensityIndex ? std::pow(blendRatio, 0.33) : 1.0f;
        
        auto& lowerIntensity = intensities[lowerIntensityIndex];
        auto samplesSize = lowerIntensity.variations.size();
        
        if (lowerIntensity.currentVariationIndex < samplesSize) {
            auto& variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];
            
            for (auto& microphone : variation.microphones) {
                auto& sound = microphone.sound;
                auto& voice = microphone.voice;
                
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
                {
                    stopVoice(voice.get(), 1.0f, true);
                    startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
                }
            }
            
            lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
        }
        
        if (lowerIntensityIndex != higherIntensityIndex) {
            auto& higherIntensity = intensities[higherIntensityIndex];
            auto& variation = higherIntensity.variations[higherIntensity.currentVariationIndex];

            for (auto& microphone : variation.microphones) {
                auto& sound = microphone.sound;
                auto& voice = microphone.voice;
                
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
                {
                    stopVoice(voice.get(), 1.0f, true);
                    startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio);
                }
            }
            
            higherIntensity.currentVariationIndex = (higherIntensity.currentVariationIndex + 1) % samplesSize;
        }
    }
}

void PanningSamplerSynthesiser::addSample(const std::string resourceName,
                                          const int bitRate,
                                          const int bitDepth,
                                          const int midiNote,
                                          const std::vector<int> stopsMidiNotes,
                                          const int intensityIndex,
                                          const int variationIndex,
                                          const float defaultStereoPan,
                                          juce::AudioFormatManager& audioFormatManager
                                          ) {
    juce::BigInteger range;
    range.setRange(midiNote, 1, true);
    int dataSizeInBytes;
    
    const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
    auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
    juce::AudioFormatReader* reader = audioFormatManager.createReaderFor(std::move(memoryInputStream));
    
    double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
    PanningSamplerSound* sound = new PanningSamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);
    
    addSound(sound);
    
    auto& instrument = mMidiNoteToInstruments[midiNote];
    
    instrument.stopsMidiNotes = stopsMidiNotes;
    
    while (instrument.intensities.empty() || intensityIndex >= instrument.intensities.size()) {
        instrument.intensities.emplace_back();
    }
    
    auto& intensity = instrument.intensities[intensityIndex];
    if (intensity.variations.empty() || variationIndex >= intensity.variations.size()) {
        intensity.variations.emplace_back();
    }
    
    auto microphone = Microphone(PanningSamplerSound::Ptr(sound), std::make_unique<PanningSamplerVoice>(defaultStereoPan));
    addVoice(microphone.voice.get());
    intensity.variations[variationIndex].microphones.push_back(std::move(microphone));
}
