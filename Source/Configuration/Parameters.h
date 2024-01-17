#pragma once

#include <string>
#include "../PluginUtils.h"
#include "GeneralMidi.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <cassert>

namespace AudioParameters
{
	// https://www.desmos.com/calculator/qkc6naksy5
	static inline juce::NormalisableRange<float> makeLogarithmicRange(float rangeStart, float rangeEnd, float intervalValue, float exponent = 6.0f)
	{
		juce::NormalisableRange<float> normalisableRange = {
			rangeStart, rangeEnd,
			// In all the following, "start" and "end" describe the unnormalized range
			// for example 0 to 15 or 0 to 100.
			[=](float start, float end, float normalised)
			{
				return start + (std::exp2(normalised * exponent) - 1) * (end - start) / (std::exp2(exponent) - 1);
			},
			[=](float start, float end, float unnormalised)
			{
				return std::log2(((unnormalised - start) / (end - start) * (std::exp2(exponent) - 1)) + 1) / exponent;
			} };
		normalisableRange.interval = intervalValue;
		return normalisableRange;
	}

	static inline juce::NormalisableRange<float> makeDecibelRange(float rangeStart, float rangeEnd, float rangeInterval)
	{
		// jassert(rangeStart < rangeEnd);  // Ensuring the start is less than the end.
		juce::NormalisableRange<float> range = {
			rangeStart,
			rangeEnd, // Directly using rangeEnd as the max dB value

			// convertFrom0to1
			[=](float min, float max, float normalizedGain)
			{
				return normalizedGain * (max - min) + min; // Linear mapping from normalized to dB
			},

			// convertTo0to1
			[=](float min, float max, float dB)
			{
				return (dB - min) / (max - min); // Linear mapping from dB to normalized
			} };
		range.interval = rangeInterval; // Setting the interval for the range
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
	static constexpr float gainDeciblesMaximumValue = 24.0f;
	static constexpr float gainDeciblesIntervalValue = 0.01f;
	static constexpr float gainDeciblesDefaultValue = 0.0f;
	static const juce::NormalisableRange<float> decibelGainNormalisableRange = makeDecibelRange(
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
	static constexpr float attackIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> attackNormalisableRange =
		juce::NormalisableRange<float>(
			attackMinimumValue,
			attackMaximumValue,
			attackIntervalValue);
	static constexpr float attackDefaultValue = 0.0f;

	static const std::string releaseComponentId = "release";
	static constexpr float releaseMinimumValue = 1.0f;
	static constexpr float releaseMaximumValue = 3000.0f;
	static constexpr float releaseIntervalValue = 0.01f;
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
	static const juce::NormalisableRange<float> linearGainNormalisableRange =
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
		{highShelfEqualizationTypeId, highShelfFrequencyDefaultValue},
		{peakFilterEqualizationTypeId, peakFilterFrequencyDefaultValue},
		{lowShelfEqualizationTypeId, lowShelfFrequencyDefaultValue} };

	static const std::map<std::string, juce::NormalisableRange<float>> equalizationTypeIdToGainNormalisableRange = {
		{highShelfEqualizationTypeId, decibelGainNormalisableRange},
		{peakFilterEqualizationTypeId, linearGainNormalisableRange},
		{lowShelfEqualizationTypeId, decibelGainNormalisableRange} };

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

					while (std::getline(iss, part, '_'))
					{
						if (!part.empty())
						{
							parts.push_back(part);
						}
					}

					switch (parts.size())
					{
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

	static const std::map<std::string, float> defaults = {
{"35_in_gain", -5.0},
{"35_in_pan", 0.0},
{"35_in_phase", 0.0},
{"35_out_gain", -10.0},
{"35_out_pan", 0.0},
{"35_out_phase", 0.0},
{"36_in_gain", -5.0},
{"36_in_pan", 0.0},
{"36_in_phase", 0.0},
{"36_out_gain", -10.0},
{"36_out_pan", 0.0},
{"36_out_phase", 0.0},
{"37_gain", -10.0},
{"37_pan", 0.0},
{"37_phase", 0.0},
{"38_bottom_gain", 0.0},
{"38_bottom_pan", 0.0},
{"38_bottom_phase", 1.0},
{"38_top_gain", 0.0},
{"38_top_pan", 0.0},
{"38_top_phase", 0.0},
{"39_gain", 0.0},
{"39_pan", 0.0},
{"39_phase", 0.0},
{"40_bottom_gain", 0.0},
{"40_bottom_pan", 0.0},
{"40_bottom_phase", 1.0},
{"40_top_gain", 0.0},
{"40_top_pan", 0.0},
{"40_top_phase", 0.0},
{"42_gain", -1.0},
{"42_left_gain", -1.0},
{"42_left_pan", -1.0},
{"42_left_phase", 0.0},
{"42_pan", 0.0},
{"42_phase", 0.0},
{"42_right_gain", -1.0},
{"42_right_pan", 1.0},
{"42_right_phase", 0.0},
{"43_gain", -10.0},
{"43_pan", 0.0},
{"43_phase", 0.0},
{"44_gain", -4.0},
{"44_left_gain", -4.0},
{"44_left_pan", -1.0},
{"44_left_phase", 0.0},
{"44_pan", 0.0},
{"44_phase", 0.0},
{"44_right_gain", -4.0},
{"44_right_pan", 1.0},
{"44_right_phase", 0.0},
{"46_gain", -2.0},
{"46_left_gain", -2.0},
{"46_left_pan", -1.0},
{"46_left_phase", 0.0},
{"46_pan", 0.0},
{"46_phase", 0.0},
{"46_right_gain", -2.0},
{"46_right_pan", 1.0},
{"46_right_phase", 0.0},
{"49_gain", -12.0},
{"49_left_gain", -12.0},
{"49_left_pan", -1.0},
{"49_left_phase", 0.0},
{"49_pan", 0.0},
{"49_phase", 0.0},
{"49_right_gain", -12.0},
{"49_right_pan", 1.0},
{"49_right_phase", 0.0},
{"50_gain", -15.0},
{"50_pan", 0.0},
{"50_phase", 0.0},
{"51_gain", -12.0},
{"51_left_gain", -12.0},
{"51_left_pan", -1.0},
{"51_left_phase", 0.0},
{"51_pan", 0.0},
{"51_phase", 0.0},
{"51_right_gain", -12.0},
{"51_right_pan", 1.0},
{"51_right_phase", 0.0},
{"52_gain", -8.0},
{"52_left_gain", -8.0},
{"52_left_pan", -1.0},
{"52_left_phase", 0.0},
{"52_pan", 0.0},
{"52_phase", 0.0},
{"52_right_gain", -8.0},
{"52_right_pan", 1.0},
{"52_right_phase", 0.0},
{"53_gain", -10.0},
{"53_left_gain", -10.0},
{"53_left_pan", -1.0},
{"53_left_phase", 0.0},
{"53_pan", 0.0},
{"53_phase", 0.0},
{"53_right_gain", -10.0},
{"53_right_pan", 1.0},
{"53_right_phase", 0.0},
{"54_gain", -10.0},
{"54_pan", 0.0},
{"54_phase", 0.0},
{"55_gain", -10.0},
{"55_left_gain", -10.0},
{"55_left_pan", -1.0},
{"55_left_phase", 0.0},
{"55_pan", 0.0},
{"55_phase", 0.0},
{"55_right_gain", -10.0},
{"55_right_pan", 1.0},
{"55_right_phase", 0.0},
{"56_gain", 6.0},
{"56_pan", 0.0},
{"56_phase", 0.0},
{"57_gain", -10.0},
{"57_left_gain", -10.0},
{"57_left_pan", -1.0},
{"57_left_phase", 0.0},
{"57_pan", 0.0},
{"57_phase", 0.0},
{"57_right_gain", -10.0},
{"57_right_pan", 1.0},
{"57_right_phase", 0.0},
{"70_gain", -10.0},
{"70_pan", 0.0},
{"70_phase", 0.0},
{"80_gain", -10.0},
{"80_pan", 0.0},
{"80_phase", 0.0},
{"81_gain", -10.0},
{"81_pan", 0.0},
{"81_phase", 0.0},
{"cymbals_attack", 2.0},
{"cymbals_compression_blend", 0.3330000042915344},
{"cymbals_compression_gain", 8.0},
{"cymbals_gain", -4.0},
{"cymbals_high_shelf_frequency", 6000.00048828125},
{"cymbals_high_shelf_gain", 0.1110000088810921},
{"cymbals_high_shelf_quality", 0.4440000057220459},
{"cymbals_low_shelf_frequency", 105.0},
{"cymbals_low_shelf_gain", 3.899000406265259},
{"cymbals_low_shelf_quality", 1.00000011920929},
{"cymbals_peak_filter_frequency", 1000.0},
{"cymbals_peak_filter_gain", 1.0},
{"cymbals_peak_filter_quality", 1.00000011920929},
{"cymbals_ratio", 3.0},
{"cymbals_release", 316.0},
{"cymbals_reverb_gain", -8.0},
{"cymbals_threshold", -32.0},
{"damping", 0.1099999994039536},
{"hi_hat_attack", 15.0},
{"hi_hat_compression_blend", 0.3330000042915344},
{"hi_hat_compression_gain", 0.0},
{"hi_hat_gain", 0.0},
{"hi_hat_high_shelf_frequency", 5000.0},
{"hi_hat_high_shelf_gain", 0.1110000088810921},
{"hi_hat_high_shelf_quality", 0.4440000057220459},
{"hi_hat_low_shelf_frequency", 200.0},
{"hi_hat_low_shelf_gain", 3.000000238418579},
{"hi_hat_low_shelf_quality", 1.00000011920929},
{"hi_hat_peak_filter_frequency", 2000.0},
{"hi_hat_peak_filter_gain", 0.2220000326633453},
{"hi_hat_peak_filter_quality", 0.1110000088810921},
{"hi_hat_ratio", 2.0},
{"hi_hat_release", 316.0},
{"hi_hat_reverb_gain", -25.0},
{"hi_hat_threshold", -40.0},
{"kick_attack", 10.0},
{"kick_compression_blend", 0.3330000042915344},
{"kick_compression_gain", 4.0},
{"kick_gain", -5.0},
{"kick_high_shelf_frequency", 6000.00048828125},
{"kick_high_shelf_gain", 1.0},
{"kick_high_shelf_quality", 1.00000011920929},
{"kick_low_shelf_frequency", 65.0},
{"kick_low_shelf_gain", 3.000000238418579},
{"kick_low_shelf_quality", 1.00000011920929},
{"kick_peak_filter_frequency", 200.0},
{"kick_peak_filter_gain", 0.75},
{"kick_peak_filter_quality", 1.00000011920929},
{"kick_ratio", 8.0},
{"kick_release", 105.0},
{"kick_reverb_gain", -10.0},
{"kick_threshold", -20.0},
{"multi-out", 0.0},
{"other_attack", 30.0},
{"other_compression_blend", 0.5550000071525574},
{"other_compression_gain", 1.0},
{"other_gain", -14.0},
{"other_high_shelf_frequency", 15000.0009765625},
{"other_high_shelf_gain", 1.0},
{"other_high_shelf_quality", 1.00000011920929},
{"other_low_shelf_frequency", 20.0},
{"other_low_shelf_gain", 1.0},
{"other_low_shelf_quality", 1.00000011920929},
{"other_peak_filter_frequency", 1000.0},
{"other_peak_filter_gain", 1.0},
{"other_peak_filter_quality", 1.00000011920929},
{"other_ratio", 6.0},
{"other_release", 316.0},
{"other_reverb_gain", -35.0},
{"other_threshold", -30.0},
{"output_attack", 300.0},
{"output_compression_blend", 0.3330000042915344},
{"output_compression_gain", 3.0},
{"output_gain", 0.0},
{"output_high_shelf_frequency", 4900.00048828125},
{"output_high_shelf_gain", 1.0},
{"output_high_shelf_quality", 1.00000011920929},
{"output_low_shelf_frequency", 20.0},
{"output_low_shelf_gain", 1.0},
{"output_low_shelf_quality", 1.00000011920929},
{"output_peak_filter_frequency", 1000.0},
{"output_peak_filter_gain", 1.0},
{"output_peak_filter_quality", 1.00000011920929},
{"output_ratio", 3.0},
{"output_release", 316.0},
{"output_threshold", -30.0},
{"room_attack", 300.0},
{"room_compression_blend", 0.7770000100135803},
{"room_compression_gain", 3.0},
{"room_gain", -34.0},
{"room_high_shelf_frequency", 6249.00048828125},
{"room_high_shelf_gain", 0.1110000088810921},
{"room_high_shelf_quality", 1.00000011920929},
{"room_low_shelf_frequency", 170.0},
{"room_low_shelf_gain", 0.05000000074505806},
{"room_low_shelf_quality", 1.00000011920929},
{"room_peak_filter_frequency", 1000.0},
{"room_peak_filter_gain", 1.0},
{"room_peak_filter_quality", 1.00000011920929},
{"room_ratio", 3.0},
{"room_release", 316.0},
{"room_size", 0.2199999988079071},
{"room_threshold", -30.0},
{"snare_attack", 10.0},
{"snare_compression_blend", 0.3330000042915344},
{"snare_compression_gain", 0.0},
{"snare_gain", -6.0},
{"snare_high_shelf_frequency", 5000.0},
{"snare_high_shelf_gain", 1.25},
{"snare_high_shelf_quality", 1.00000011920929},
{"snare_low_shelf_frequency", 214.0000152587891},
{"snare_low_shelf_gain", 1.333000063896179},
{"snare_low_shelf_quality", 1.00000011920929},
{"snare_peak_filter_frequency", 215.0},
{"snare_peak_filter_gain", 3.000000238418579},
{"snare_peak_filter_quality", 2.500000238418579},
{"snare_ratio", 6.0},
{"snare_release", 316.0},
{"snare_reverb_gain", -5.0},
{"snare_threshold", -15.0},
{"toms_attack", 10.0},
{"toms_compression_blend", 0.3330000042915344},
{"toms_compression_gain", 5.0},
{"toms_gain", 0.0},
{"toms_high_shelf_frequency", 5320.0},
{"toms_high_shelf_gain", 1.200000047683716},
{"toms_high_shelf_quality", 1.00000011920929},
{"toms_low_shelf_frequency", 177.0000152587891},
{"toms_low_shelf_gain", 2.0},
{"toms_low_shelf_quality", 1.00000011920929},
{"toms_peak_filter_frequency", 799.9999389648438},
{"toms_peak_filter_gain", 0.5},
{"toms_peak_filter_quality", 1.00000011920929},
{"toms_ratio", 4.0},
{"toms_release", 158.0},
{"toms_reverb_gain", -5.0},
{"toms_threshold", -40.0},
{"width", 1.0},
	};

	static float getDefaultValue(const std::string key, const float fallbackValue)
	{
		auto it = defaults.find(key);
		if (it != defaults.end())
		{
			return it->second;
		}
		else
		{
			assert(false);
			return fallbackValue;
		}
	}
}
