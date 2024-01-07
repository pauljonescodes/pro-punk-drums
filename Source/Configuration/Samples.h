#pragma once

#include <vector>
#include <map>

namespace Samples {
	static constexpr int bitRate = 44100;
	static constexpr int bitDepth = 24;

	static const std::string leftComponentId = "left";
	static const std::string rightComponentId = "right";
	static const std::string bottomComponentId = "bottom";
	static const std::string topComponentId = "top";
	static const std::string inComponentId = "in";
	static const std::string outComponentId = "out";
};
