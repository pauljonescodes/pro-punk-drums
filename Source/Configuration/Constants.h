#pragma once

#include <string>

namespace constants {
const std::string gainId = "gain";
const std::string panId = "pan";
const std::string phaseId = "phase";

const juce::NormalisableRange<float> gainNormalizableRange = juce::NormalisableRange<float>(-60.0f, 12.0f, 0.01f);
const juce::NormalisableRange<float> panNormalizableRange = juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f);
}
