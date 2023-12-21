#include "PluginSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PluginSynthesiserVoice.h"

PluginSynthesiser::PluginSynthesiser()
{
    setNoteStealingEnabled(false);
}

void PluginSynthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity, const bool allowTailOff) {
    
}

void PluginSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity, const std::string micId) {
    if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end())
    {
        auto& instrument = mMidiNoteToInstruments.at(midiNoteNumber);

        for (auto& voice : voices)
        {
            for (int stopsMidiNote : instrument.stopsMidiNotes)
            {
                auto currentlyPlayingSounds = voice->getCurrentlyPlayingSound();
                if (currentlyPlayingSounds != nullptr && currentlyPlayingSounds->appliesToNote(stopsMidiNote))
                {
                    voice->stopNote(1, true);
                }
            }
        }

        auto& intensities = instrument.velocities;
        float position = velocity * (intensities.size() - 1);

        int lowerIntensityIndex = static_cast<int>(position);
        int higherIntensityIndex = lowerIntensityIndex + 1;

        if (lowerIntensityIndex >= intensities.size())
        {
            lowerIntensityIndex = 0;
        }

        if (higherIntensityIndex >= intensities.size() || velocity == 0)
        {
            higherIntensityIndex = lowerIntensityIndex;
        }

        float blendRatio = position - lowerIntensityIndex;
        float perceivedBlendRatio = lowerIntensityIndex != higherIntensityIndex ? std::pow(blendRatio, 0.25) : 0.0f;
        float velocityFactor = std::sqrt(velocity);

        auto& lowerIntensity = intensities[lowerIntensityIndex];
        int samplesSize = lowerIntensity.variations.size();

        if (lowerIntensity.currentVariationIndex < samplesSize)
        {
            auto& variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];

            auto& sound = variation.microphones[micId].sound;
            auto& voice = variation.microphones[micId].voice;

            if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
            {
                stopVoice(voice.get(), 1.0f, true);
                startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, (1.0f - perceivedBlendRatio) * velocityFactor);
            }

            lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
        }

        if (lowerIntensityIndex != higherIntensityIndex)
        {
            auto& higherIntensity = intensities[higherIntensityIndex];
            auto& variation = higherIntensity.variations[higherIntensity.currentVariationIndex];

            auto& sound = variation.microphones[micId].sound;
            auto& voice = variation.microphones[micId].voice;

            if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
            {
                stopVoice(voice.get(), 1.0f, true);
                startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio * velocityFactor);
            }

            higherIntensity.currentVariationIndex = (higherIntensity.currentVariationIndex + 1) % samplesSize;
        }
    }
}

void PluginSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end())
    {
        auto& instrument = mMidiNoteToInstruments.at(midiNoteNumber);
        
        for (auto& voice : voices)
        {
            for (int stopsMidiNote : instrument.stopsMidiNotes)
            {
                auto currentlyPlayingSounds = voice->getCurrentlyPlayingSound();
                if (currentlyPlayingSounds != nullptr && currentlyPlayingSounds->appliesToNote(stopsMidiNote))
                {
                    voice->stopNote(1, true);
                }
            }
        }
        
        auto& intensities = instrument.velocities;
        float position = velocity * (intensities.size() - 1);
        
        int lowerIntensityIndex = static_cast<int>(position);
        int higherIntensityIndex = lowerIntensityIndex + 1;
        
        if (lowerIntensityIndex >= intensities.size())
        {
            lowerIntensityIndex = 0;
        }
        
        if (higherIntensityIndex >= intensities.size() || velocity == 0)
        {
            higherIntensityIndex = lowerIntensityIndex;
        }
        
        float blendRatio = position - lowerIntensityIndex;
        float perceivedBlendRatio = lowerIntensityIndex != higherIntensityIndex ? std::pow(blendRatio, 0.25) : 0.0f;
        float velocityFactor = std::sqrt(velocity);
        
        auto& lowerIntensity = intensities[lowerIntensityIndex];
        int samplesSize = lowerIntensity.variations.size();
        
        if (lowerIntensity.currentVariationIndex < samplesSize)
        {
            auto& variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];
            
            for (auto& microphone : variation.microphones)
            {
                auto& sound = microphone.second.sound;
                auto& voice = microphone.second.voice;
                
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
                {
                    stopVoice(voice.get(), 1.0f, true);
                    startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, (1.0f - perceivedBlendRatio) * velocityFactor);
                }
            }
            
            lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
        }
        
        if (lowerIntensityIndex != higherIntensityIndex)
        {
            auto& higherIntensity = intensities[higherIntensityIndex];
            auto& variation = higherIntensity.variations[higherIntensity.currentVariationIndex];
            
            for (auto& microphone : variation.microphones) {
                auto& sound = microphone.second.sound;
                auto& voice = microphone.second.voice;
                
                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
                {
                    stopVoice(voice.get(), 1.0f, true);
                    startVoice(voice.get(), sound.get(), midiChannel, midiNoteNumber, perceivedBlendRatio * velocityFactor);
                }
            }
            
            higherIntensity.currentVariationIndex = (higherIntensity.currentVariationIndex + 1) % samplesSize;
        }
    }
}

void PluginSynthesiser::addSample(const std::string resourceName,
                                  const int bitRate,
                                  const int bitDepth,
                                  const int midiNote,
                                  const std::string micId,
                                  const std::vector<int> stopsMidiNotes,
                                  const int velocityIndex,
                                  const int variationIndex,
                                  juce::AudioFormatManager& audioFormatManager,
                                  juce::RangedAudioParameter& gainParameter,
                                  juce::RangedAudioParameter& panParameter,
                                  juce::AudioParameterBool& phaseParameter
                                  ) {
    juce::BigInteger range;
    range.setRange(midiNote, 1, true);
    int dataSizeInBytes;
    
    const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
    auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
    juce::AudioFormatReader* reader = audioFormatManager.createReaderFor(std::move(memoryInputStream));
    
    double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
    PluginSynthesiserSound* sound = new PluginSynthesiserSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);
    
    addSound(sound);
    
    auto& instrument = mMidiNoteToInstruments[midiNote];
    
    instrument.stopsMidiNotes = stopsMidiNotes;
    
    while (instrument.velocities.empty() || velocityIndex >= instrument.velocities.size())
    {
        instrument.velocities.emplace_back();
    }
    
    auto& velocity = instrument.velocities[velocityIndex];
    while (velocity.variations.empty() || variationIndex >= velocity.variations.size())
    {
        velocity.variations.emplace_back();
    }
    
    auto microphone = Microphone(PluginSynthesiserSound::Ptr(sound),
                                 std::make_shared<PluginSynthesiserVoice>(gainParameter, panParameter, phaseParameter)
                                 );
    
    addVoice(microphone.voice.get());
    velocity.variations[variationIndex].microphones.emplace(
        micId,
        std::move(microphone)
    );
}

std::vector<int> PluginSynthesiser::getMidiNotesVector()
{
    std::vector<int> keys;
    for (const auto& pair : mMidiNoteToInstruments) {
        keys.push_back(pair.first); // Collect the keys
    }
    return keys;
}
