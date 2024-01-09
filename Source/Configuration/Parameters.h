#pragma once

#include <string>
#include "../PluginUtils.h"
#include "GeneralMidi.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>

namespace AudioParameters 
{
	// https://www.desmos.com/calculator/qkc6naksy5
	static inline juce::NormalisableRange<float> makeLogarithmicRange(float rangeStart, float rangeEnd, float intervalValue, float exponent = 6.0f)
	{
		juce::NormalisableRange<float> normalisableRange = {
			rangeStart, rangeEnd,
			// In all the following, "start" and "end" describe the unnormalized range
			// for example 0 to 15 or 0 to 100.
			[=](float start, float end, float normalised) {
				return start + (std::exp2(normalised * exponent) - 1) * (end - start) / (std::exp2(exponent) - 1);
			},
			[=](float start, float end, float unnormalised) {
				return std::log2(((unnormalised - start) / (end - start) * (std::exp2(exponent) - 1)) + 1) / exponent;
			}
		};
		normalisableRange.interval = intervalValue;
		return normalisableRange;
	}

	static inline juce::NormalisableRange<float> makeDecibelRange(float rangeStart, float rangeEnd, float rangeInterval)
	{
		//jassert(rangeStart < rangeEnd);  // Ensuring the start is less than the end.
		juce::NormalisableRange<float> range = {
			rangeStart,
			rangeEnd,  // Directly using rangeEnd as the max dB value

			// convertFrom0to1
			[=](float min, float max, float normalizedGain) {
				return normalizedGain * (max - min) + min;  // Linear mapping from normalized to dB
			},

			// convertTo0to1
			[=](float min, float max, float dB) {
				return (dB - min) / (max - min);  // Linear mapping from dB to normalized
			}
		};
		range.interval = rangeInterval;  // Setting the interval for the range
		return range;
	}

	static const std::string multiOutComponentId = "multi-out";
	static const std::string phaseComponentId = "phase";

	static const std::string panComponentId = "pan";
	static const juce::NormalisableRange<float> panNormalisableRange = juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f);

	static const std::string onComponentId = "on";
	static const std::string compressionComponentId = "compression";

	static const std::string gainComponentId = "gain";
	static constexpr float gainDecibelsMinimumValue = -64.0f;
	static constexpr float gainDeciblesMaximumValue = 8.0f;
	static constexpr float gainDeciblesIntervalValue = 0.01f;
	static constexpr float gainDeciblesDefaultValue = 0.0f;
	static const juce::NormalisableRange<float> gainNormalisableRange = makeDecibelRange(
		gainDecibelsMinimumValue,
		gainDeciblesMaximumValue,
		gainDeciblesIntervalValue);

	static const std::string thresholdComponentId = "threshold";
	static constexpr float thresholdMinimumValue = -64.0f;
	static constexpr float thresholdMaximumValue = 0.0f;
	static constexpr float thresholdIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> thresholdNormalisableRange =
		makeDecibelRange(
			thresholdMinimumValue,
			thresholdMaximumValue,
			thresholdIntervalValue);
	static constexpr float thresholdDefaultValue = 0.0f;

	static const std::string ratioComponentId = "ratio";
	static constexpr float ratioMinimumValue = 1.0f;
	static constexpr float ratioMaximumValue = 32.0f;
	static constexpr float ratioIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> ratioNormalisableRange =
		juce::NormalisableRange<float>(
			ratioMinimumValue,
			ratioMaximumValue,
			ratioIntervalValue);
	static constexpr float ratioDefaultValue = 1.0f;

	static const std::string attackId = "attack";
	static constexpr float attackMinimumValue = 0.0f;
	static constexpr float attackMaximumValue = 1000.0f;
	static constexpr float attackIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> attackNormalisableRange =
		juce::NormalisableRange<float>(
			attackMinimumValue,
			attackMaximumValue,
			attackIntervalValue);
	static constexpr float attackDefaultValue = 0.0f;

	static const std::string releaseComponentId = "release";
	static constexpr float releaseMinimumValue = 0.0f;
	static constexpr float releaseMaximumValue = 1000.0f;
	static constexpr float releaseIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> releaseNormalisableRange =
		juce::NormalisableRange<float>(
			releaseMinimumValue,
			releaseMaximumValue,
			releaseIntervalValue);
	static constexpr float releaseDefaultValue = 0.0f;

	static const std::string qualityComponentId = "quality";
	static constexpr float qualityDefaultValue = 1.0f;
	static constexpr float qualityMinimumValue = 0.001f;
	static constexpr float qualityMaximumValue = 10.f;
	static constexpr float qualityIntervalValue = 0.001f;
	static const juce::NormalisableRange<float> qualityNormalisableRange = 
		makeLogarithmicRange(qualityMinimumValue, qualityMaximumValue, qualityIntervalValue);

	static const std::string frequencyComponentId = "frequency";
	static constexpr float frequencyMinimumValue = 15.0f;
	static constexpr float frequencyMaximumValue = 20000.0f;
	static constexpr float frequencyIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> frequencyNormalisableRange =
		makeLogarithmicRange(frequencyMinimumValue, frequencyMaximumValue, frequencyIntervalValue);

	static constexpr float gainMinimumValue = 0.0f;
	static constexpr float peakFilterGainMinimumValue = 0.001f;
	static constexpr float gainMaximumValue = 24.0f;
	static constexpr float gainInterval = 0.001;
	static constexpr float gainDefaultValue = 1.0f;
	static const juce::NormalisableRange<float> eqGainNormalisableRange =
		makeLogarithmicRange(
			gainMinimumValue,
			gainMaximumValue,
			gainInterval);

	static const std::string lowShelfEqualizationTypeId = "low shelf";
	static constexpr float lowShelfFrequencyDefaultValue = 20.0f;

	static const std::string peakFilterEqualizationTypeId = "peak filter";
	static constexpr float peakFilterFrequencyDefaultValue = 1000.0f;

	static const std::string highShelfEqualizationTypeId = "high shelf";
	static constexpr float highShelfFrequencyDefaultValue = 15000.0f;

	static const std::vector<std::string> equalizationTypeIdVector = {
		lowShelfEqualizationTypeId,
		peakFilterEqualizationTypeId,
		highShelfEqualizationTypeId,
	};

	static const std::map<std::string, float> equalizationTypeIdToDefaultFrequencyMap = {
		{highShelfEqualizationTypeId,highShelfFrequencyDefaultValue},
		{peakFilterEqualizationTypeId,peakFilterFrequencyDefaultValue},
		{lowShelfEqualizationTypeId,lowShelfFrequencyDefaultValue}
	};

	static const std::map<std::string, juce::NormalisableRange<float>> equalizationTypeIdToGainNormalisableRange = {
	{highShelfEqualizationTypeId,gainNormalisableRange},
	{peakFilterEqualizationTypeId,eqGainNormalisableRange},
	{lowShelfEqualizationTypeId,gainNormalisableRange}
	};

	static const std::string reverbComponentId = "reverb";

	static constexpr float reverbGainDefaultValue = 0.05f;

	static const std::string roomSizeComponentId = "room_size";
	static constexpr float roomSizeMinimumValue = 0.00f;
	static constexpr float roomSizeMaximumValue = 1.00f;
	static constexpr float roomSizeIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> roomSizeNormalisableRange =
		juce::NormalisableRange<float>(
			roomSizeMinimumValue,
			roomSizeMaximumValue,
			roomSizeIntervalValue);
	static constexpr float roomSizeDefaultValue = 0.5f;

	static const std::string dampingComponentId = "damping";
	static constexpr float dampingMinimumValue = 0.00f;
	static constexpr float dampingMaximumValue = 1.00f;
	static constexpr float dampingIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> dampingNormalisableRange =
		juce::NormalisableRange<float>(
		 dampingMinimumValue,
		 dampingMaximumValue,
		 dampingIntervalValue);
	static constexpr float dampingDefaultValue = 0.5f;

	static const std::string widthComponentId = "width";
	static constexpr float widthMinimumValue = 0.00f;
	static constexpr float widthMaximumValue = 1.00f;
	static constexpr float widthIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> widthNormalisableRange =
		juce::NormalisableRange<float>(
			widthMinimumValue,
			widthMaximumValue,
			widthIntervalValue);
	static constexpr float widthDefaultValue = 1.0f;

	static const std::string dryWetComponentId = "blend";

	static constexpr float dryWetMinimumValue = 0.0f;
	static constexpr float dryWetMaximumValue = 1.0f;
	static constexpr float dryWetIntervalValue = 0.001f;
	static constexpr float allWetDefaultValue = 1.0f;
	static constexpr float allDryDefaultValue = 0.0f;
	static const juce::NormalisableRange<float> dryWetNormalisableRange = 
		juce::NormalisableRange<float>(
			dryWetMinimumValue,
			dryWetMaximumValue,
			dryWetIntervalValue);

	inline std::map<int, std::set<std::string>> getUniqueMidiNoteMicCombinations()
	{
		std::map<int, std::set<std::string>> uniqueMidiNoteMicCombinations;

		for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; resourceIndex++)
		{
			std::string namedResource = BinaryData::namedResourceList[resourceIndex];
			std::string micId, midiName, generalMidiSnakeCaseName = "";
			int midiNote = -1;

			for (auto midiNameIndex = 0; midiNameIndex < GeneralMidiPercussion::midiNamesVector.size(); midiNameIndex++)
			{
				midiName = GeneralMidiPercussion::midiNamesVector.at(midiNameIndex);
				generalMidiSnakeCaseName = stringToSnakeCase(midiName);

				std::string cleanedNamedResource = namedResource.substr(0, namedResource.size() - 4);
				size_t pos = cleanedNamedResource.find(generalMidiSnakeCaseName);
				if (pos == 0)
				{
					std::string resourceMetadata = cleanedNamedResource.substr(pos + generalMidiSnakeCaseName.length());

					std::istringstream iss(resourceMetadata);
					std::vector<std::string> parts;
					std::string part;

					while (std::getline(iss, part, '_')) {
						if (!part.empty()) {
							parts.push_back(part);
						}
					}

					switch (parts.size()) {
					case 0:
						midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						break;
					case 1:
						midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						if (!stringIsNumeric(parts[0]))
						{
							micId = parts[0];
						}
						break;
					case 2:
						if (stringIsNumeric(parts[1]))
						{
							midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						}
						else
						{
							midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
							micId = parts[1];
						}
						break;
					case 3:
						if (stringIsNumeric(parts[1]))
						{
							midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
							micId = parts[2];
						}
						else
						{
							midiNameIndex = (int)GeneralMidiPercussion::midiNamesVector.size() + 1;
						}
						break;
					default:
						midiNameIndex = (int)GeneralMidiPercussion::midiNamesVector.size() + 1;
						break;
					}
				}
			}

			if (midiNote != -1)
			{
				uniqueMidiNoteMicCombinations[midiNote].insert(micId);
			}
		}

		return uniqueMidiNoteMicCombinations;
	}
}
