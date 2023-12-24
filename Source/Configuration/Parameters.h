#pragma once

#include <string>

namespace parameters {
	static const std::string multiOutId = "multi-out";
	static const std::string phaseId = "phase";

	static const std::string panId = "pan";
	static const juce::NormalisableRange<float> panNormalizableRange = juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f);

	static const std::string onId = "on";
	static const std::string compressionId = "compression";

	static const std::string gainId = "gain";
	static const juce::NormalisableRange<float> gainNormalizableRange = juce::NormalisableRange<float>(-64.0, 12.0f, 0.01f);
	static constexpr float gainDefaultValue = 0.0f;

	static const std::string thresholdId = "threshold";
	static constexpr float thresholdMinimumValue = -64.0f;
	static constexpr float thresholdMaximumValue = 0.0f;
	static constexpr float thresholdIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> thresholdNormalizableRange = juce::NormalisableRange<float>(thresholdMinimumValue, thresholdMaximumValue, thresholdIntervalValue);
	static constexpr float thresholdDefaultValue = 0.0f;

	static const std::string ratioId = "ratio";
	static constexpr float ratioMinimumValue = 1.0f;
	static constexpr float ratioMaximumValue = 100.0f;
	static constexpr float ratioIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> ratioNormalizableRange = juce::NormalisableRange<float>(ratioMinimumValue, ratioMaximumValue, ratioIntervalValue);
	static constexpr float ratioDefaultValue = 1.0f;

	static const std::string attackId = "attack";
	static constexpr float attackMinimumValue = 0.0f;
	static constexpr float attackMaximumValue = 1000.0f;
	static constexpr float attackIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> attackNormalizableRange = juce::NormalisableRange<float>(attackMinimumValue, attackMaximumValue, attackIntervalValue);
	static constexpr float attackDefaultValue = 0.0f;

	static const std::string releaseId = "release";
	static constexpr float releaseMinimumValue = 0.0f;
	static constexpr float releaseMaximumValue = 1000.0f;
	static constexpr float releaseIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> releaseNormalizableRange = juce::NormalisableRange<float>(releaseMinimumValue, releaseMaximumValue, releaseIntervalValue);
	static constexpr float releaseDefaultValue = 0.0f;

	static const std::string qualityId = "quality";
	static constexpr float qualityDefaultValue = 1.1f;
	static constexpr float qualityMinimumValue = 0.1f;
	static constexpr float qualityMaximumValue = 10.f;
	static constexpr float qualityIntervalValue = 0.01f;
	static const juce::NormalisableRange<float> qualityNormalizableRange = juce::NormalisableRange<float>(qualityMinimumValue, qualityMaximumValue, qualityIntervalValue);

	static const std::string frequencyId = "frequency";
	static constexpr float frequencyMinimumValue = 15.0f;
	static constexpr float frequencyMaximumValue = 20000.0f;
	static constexpr float frequencyIntervalValue = 1.0f;
	static const juce::NormalisableRange<float> frequencyNormalizableRange = juce::NormalisableRange<float>(frequencyMinimumValue, frequencyMaximumValue, frequencyIntervalValue);

	static const juce::NormalisableRange<float> eqFilterGainNormalizableRange = juce::NormalisableRange<float>(0.001f, 12.0f, 0.001f);
	static constexpr float eqFilterGainDefaultValue = 1.0f;

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
}
