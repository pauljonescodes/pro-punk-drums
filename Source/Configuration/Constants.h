#pragma once

#include <string>

namespace parameters {
	const std::string presetId = "preset";
	const std::string multiOutId = "multi-out";
	
	const std::string phaseId = "phase";

	const std::string leftId = "left";
	const std::string rightId = "right";
	const std::string bottomId = "bottom";

	const std::string thresholdId = "threshold";
	const juce::NormalisableRange<float> thresholdNormalizableRange = juce::NormalisableRange<float>(-64.0, 0.0f, 0.01f);
	const float thresholdDefault = 0.0f;

	const std::string ratioId = "ratio";
	const juce::NormalisableRange<float> ratioNormalizableRange = juce::NormalisableRange<float>(1.0f, 100.0f, 1.0f);
	const float ratioDefault = 1.0f;

	const std::string attackId = "attack";
	const juce::NormalisableRange<float> attackNormalizableRange = juce::NormalisableRange<float>(0.0f, 1000.0f, 1.0f);
	const float attackDefault = 0.0f;

	const std::string releaseId = "release";
	const juce::NormalisableRange<float> releaseNormalizableRange = juce::NormalisableRange<float>(0.0f, 1000.0f, 1.0f);
	const float releaseDefault = 0.0f;

	const std::string gainId = "gain";
	const juce::NormalisableRange<float> gainNormalizableRange = juce::NormalisableRange<float>(-64.0, 12.0f, 0.01f);
	const float gainDefault = 0.0f;

	const std::string qualityId = "quality";
	static const float qualityDefaultValue = 0.7f;
	static const float qualityMinimumValue = 0.1f;
	static const float qualityMaximumValue = 10.f;
	static const float qualityIntervalValue = 0.01f;
	const juce::NormalisableRange<float> qualityNormalizableRange = juce::NormalisableRange<float>(qualityMinimumValue, qualityMaximumValue, qualityIntervalValue);

	const std::string frequencyId = "frequency";
	static const float frequencyMinimumValue = 15.0f;
	static const float frequencyMaximumValue = 20000.0f;
	static const float frequencyIntervalValue = 1.0f;
	const juce::NormalisableRange<float> frequencyNormalizableRange = juce::NormalisableRange<float>(frequencyMinimumValue, frequencyMaximumValue, frequencyIntervalValue);

	const std::string highShelfEqualizationTypeId = "high shelf";
	static const float highShelfFrequencyDefaultValue = 10000.0f;
	
	const std::string peakFilterEqualizationTypeId = "peak filter";
	static const float peakFilterFrequencyDefaultValue = 1000.0f;
	
	const std::string lowShelfEqualizationTypeId = "low shelf";
	static const float lowShelfFrequencyDefaultValue = 800.0f;

	const std::vector<std::string> equalizationTypeIdVector = {
		highShelfEqualizationTypeId,
		peakFilterEqualizationTypeId,
		lowShelfEqualizationTypeId
	};

	const std::map<std::string, float> equalizationTypeIdToDefaultFrequencyMap = {
		{highShelfEqualizationTypeId,highShelfFrequencyDefaultValue},
		{peakFilterEqualizationTypeId,peakFilterFrequencyDefaultValue},
		{lowShelfEqualizationTypeId,lowShelfFrequencyDefaultValue}
	};

	const std::string panId = "pan";
	const juce::NormalisableRange<float> panNormalizableRange = juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f);
}
