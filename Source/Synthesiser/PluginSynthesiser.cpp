#include "PluginSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PluginSynthesiserVoice.h"

void PluginSynthesiser::noteOff(const int midiChannel, const int midiNoteNumber, const float velocity, const bool allowTailOff)
{
}

void PluginSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity, const std::string micId)
{
    if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end())
    {
        auto &instrument = mMidiNoteToInstruments.at(midiNoteNumber);

        for (auto &voice : voices)
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

        auto &intensities = instrument.velocities;

        int intensityIndex = static_cast<int>(floor(velocity * intensities.size()));
        intensityIndex = std::min(intensityIndex, (int)intensities.size() - 1);

        auto &lowerIntensity = intensities[intensityIndex];
        auto samplesSize = lowerIntensity.variations.size();

        if (lowerIntensity.currentVariationIndex < samplesSize)
        {
            auto &variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];

            auto &sound = variation.microphones.at(micId).sound;
            PluginSynthesiserVoice *voice = variation.microphones.at(micId).voice;

            if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
            {
                stopVoice(voice, 1.0f, true);
                startVoice(voice, sound, midiChannel, midiNoteNumber, velocityToGain(velocity));
            }

            lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
        }
    }
}

void PluginSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    if (mMidiNoteToInstruments.find(midiNoteNumber) != mMidiNoteToInstruments.end())
    {
        auto &instrument = mMidiNoteToInstruments.at(midiNoteNumber);

        for (auto &voice : voices)
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

        auto &intensities = instrument.velocities;

        int intensityIndex = static_cast<int>(floor(velocity * intensities.size()));
        intensityIndex = std::min(intensityIndex, (int)intensities.size() - 1);

        auto &lowerIntensity = intensities[intensityIndex];
        auto samplesSize = lowerIntensity.variations.size();

        if (lowerIntensity.currentVariationIndex < samplesSize)
        {
            auto &variation = lowerIntensity.variations[lowerIntensity.currentVariationIndex];

            for (auto &microphone : variation.microphones)
            {
                auto &sound = microphone.second.sound;
                PluginSynthesiserVoice *voice = microphone.second.voice;

                if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
                {
                    stopVoice(voice, 1.0f, true);
                    startVoice(voice, sound, midiChannel, midiNoteNumber, velocityToGain(velocity));
                }
            }

            lowerIntensity.currentVariationIndex = (lowerIntensity.currentVariationIndex + 1) % samplesSize;
        }
    }
}

float PluginSynthesiser::velocityToGain(float x)
{
    if (x <= 0.05)
    {
        return 0.9 * (x / 0.05);
    }
    else if (x > 0.05 && x < 0.95)
    {
        return 0.95;
    }
    else
    {
        return 0.95 + (x - 0.95);
    }
}

PluginSynthesiser::PluginSynthesiser()
{
    setNoteStealingEnabled(false);
}

PluginSynthesiser::~PluginSynthesiser()
{
    DBG("~PluginSynthesiser()");
}

void PluginSynthesiser::addSample(const std::string resourceName,
                                  const int bitRate,
                                  const int bitDepth,
                                  const int midiNote,
                                  const std::string micId,
                                  const std::vector<int> stopsMidiNotes,
                                  const int velocityIndex,
                                  const int variationIndex,
                                  juce::AudioFormatManager &audioFormatManager,
                                  juce::RangedAudioParameter &gainParameter,
                                  juce::RangedAudioParameter &panParameter,
                                  juce::AudioParameterBool &phaseParameter)
{
    juce::BigInteger range;
    range.setRange(midiNote, 1, true);
    int dataSizeInBytes;

    const char *sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
    auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
    juce::AudioFormatReader *reader = audioFormatManager.createReaderFor(std::move(memoryInputStream));

    double maxSampleLengthSeconds = dataSizeInBytes / (Samples::bitRate * (Samples::bitDepth / 8.0));
    PluginSynthesiserSound *sound = new PluginSynthesiserSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);

    addSound(sound);

    auto &instrument = mMidiNoteToInstruments[midiNote];

    instrument.stopsMidiNotes = stopsMidiNotes;

    while (instrument.velocities.empty() || velocityIndex >= instrument.velocities.size())
    {
        instrument.velocities.emplace_back();
    }

    auto &velocity = instrument.velocities[velocityIndex];
    while (velocity.variations.empty() || variationIndex >= velocity.variations.size())
    {
        velocity.variations.emplace_back();
    }

    PluginSynthesiserVoice *voice = new PluginSynthesiserVoice(gainParameter, panParameter, phaseParameter);
    auto microphone = Microphone(sound, voice);

    addVoice(voice);
    velocity.variations[variationIndex].microphones.emplace(
        micId,
        std::move(microphone));
}

std::vector<int> PluginSynthesiser::getMidiNotesVector()
{
    std::vector<int> keys;
    for (const auto &pair : mMidiNoteToInstruments)
    {
        keys.push_back(pair.first); // Collect the keys
    }
    return keys;
}
