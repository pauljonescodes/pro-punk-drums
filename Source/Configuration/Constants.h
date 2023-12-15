/*
  ==============================================================================

	Constants.h
	Created: 5 Dec 2023 5:45:35pm
	Author:  paulm

  ==============================================================================
*/

#pragma once

#include <string>

namespace constants {
	const int maximumVariations = 127;

	const std::string acousticBassDrumMicrophoneInId = "in";
	const std::string acousticBassDrumMicrophoneOutId = "out";

	const std::string acousticSnareMicrophoneTopId = "top";
	const std::string acousticSnareMicrophoneBottomId = "bottom";

	const std::string centerMicrophoneId = "on";
	const std::string leftMicrophoneId = "ohl";
	const std::string rightMicrophoneId = "ohr";

	const std::string softIntensityId = "soft";
	const std::string mediumIntensityId = "medium";
	const std::string hardIntensityId = "hard";
}