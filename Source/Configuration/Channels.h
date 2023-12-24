#pragma once
#include <map>
#include "GeneralMidi.h"
#include <string>

namespace channels {
	static constexpr int kickIndex = 0;
	static constexpr int snareIndex = 1;
	static constexpr int tomsIndex = 2;
	static constexpr int hiHatIndex = 3;
	static constexpr int cymbalsIndex = 4;
	static constexpr int percussionIndex = 5;
	static constexpr int outputIndex = 6;

	static constexpr int size = 7;

	static const std::string kickName = "Kick";
	static const std::string snareName = "Snare";
	static const std::string tomsName = "Toms";
	static const std::string hiHatName = "Hi-hat";
	static const std::string cymbalsName = "Cymbals";
	static const std::string percussionName = "Percussion";
	static const std::string outputName = "Output";

	static const std::string kickId = "kick";
	static const std::string snareId = "snare";
	static const std::string tomsId = "toms";
	static const std::string hiHatId = "hi_hat";
	static const std::string cymbalsId = "cymbals";
	static const std::string percussionId = "perc";
	static const std::string outputId = "output";

	static const std::map<int, std::string> channelIndexToIdMap = {
		{ channels::kickIndex, channels::kickId },
		{ channels::snareIndex, channels::snareId },
		{ channels::tomsIndex, channels::tomsId },
		{ channels::hiHatIndex, channels::hiHatId },
		{ channels::cymbalsIndex, channels::cymbalsId },
		{ channels::percussionIndex, channels::percussionId },
		{ channels::outputIndex, channels::outputId },
	};

	static const std::map<int, std::string> channelIndexToNameMap = {
		{ channels::kickIndex, channels::kickName },
		{ channels::snareIndex, channels::snareName },
		{ channels::tomsIndex, channels::tomsName },
		{ channels::hiHatIndex, channels::hiHatName },
		{ channels::cymbalsIndex, channels::cymbalsName },
		{ channels::percussionIndex, channels::percussionName },
		{ channels::outputIndex, channels::outputName },
	};

	static const std::map<int, int> channelIndexToMainGeneralMidiNote = {
		{ channels::kickIndex, generalmidi::midinotes::bassDrum1 },
		{ channels::snareIndex, generalmidi::midinotes::acousticSnare },
		{ channels::tomsIndex, generalmidi::midinotes::highTom },
		{ channels::hiHatIndex, generalmidi::midinotes::closedHiHat},
		{ channels::cymbalsIndex, generalmidi::midinotes::crashCymbal2 },
		{ channels::percussionIndex, generalmidi::midinotes::tambourine },
		{ channels::outputIndex, generalmidi::midinotes::notApplicable },
	};

	static const std::map<int, int> generalMidiNoteToChannelIndex = {
		{generalmidi::midinotes::acousticBassDrum, kickIndex},
		{generalmidi::midinotes::bassDrum1, kickIndex},
		{generalmidi::midinotes::sideStick, snareIndex},
		{generalmidi::midinotes::acousticSnare, snareIndex},
		{generalmidi::midinotes::handClap, percussionIndex},
		{generalmidi::midinotes::electricSnare, snareIndex},
		{generalmidi::midinotes::lowFloorTom, tomsIndex},
		{generalmidi::midinotes::closedHiHat, hiHatIndex},
		{generalmidi::midinotes::highFloorTom, tomsIndex},
		{generalmidi::midinotes::pedalHiHat, hiHatIndex},
		{generalmidi::midinotes::lowTom, tomsIndex},
		{generalmidi::midinotes::openHiHat, hiHatIndex},
		{generalmidi::midinotes::lowMidTom, tomsIndex},
		{generalmidi::midinotes::hiMidTom, tomsIndex},
		{generalmidi::midinotes::crashCymbal1, cymbalsIndex},
		{generalmidi::midinotes::highTom, tomsIndex},
		{generalmidi::midinotes::rideCymbal1, cymbalsIndex},
		{generalmidi::midinotes::chineseCymbal, cymbalsIndex},
		{generalmidi::midinotes::rideBell, cymbalsIndex},
		{generalmidi::midinotes::tambourine, percussionIndex},
		{generalmidi::midinotes::splashCymbal, cymbalsIndex},
		{generalmidi::midinotes::cowbell, percussionIndex},
		{generalmidi::midinotes::crashCymbal2, cymbalsIndex},
		{generalmidi::midinotes::vibraslap, percussionIndex},
		{generalmidi::midinotes::rideCymbal2, cymbalsIndex},
		{generalmidi::midinotes::hiBongo, percussionIndex},
		{generalmidi::midinotes::lowBongo, percussionIndex},
		{generalmidi::midinotes::muteHiConga, percussionIndex},
		{generalmidi::midinotes::openHiConga, percussionIndex},
		{generalmidi::midinotes::lowConga, percussionIndex},
		{generalmidi::midinotes::highTimbale, percussionIndex},
		{generalmidi::midinotes::lowTimbale, percussionIndex},
		{generalmidi::midinotes::highAgog, percussionIndex},
		{generalmidi::midinotes::lowAgog, percussionIndex},
		{generalmidi::midinotes::cabas, percussionIndex},
		{generalmidi::midinotes::maraca, percussionIndex},
		{generalmidi::midinotes::shortWhistle, percussionIndex},
		{generalmidi::midinotes::longWhistle, percussionIndex},
		{generalmidi::midinotes::shortGuir, percussionIndex},
		{generalmidi::midinotes::longGuir, percussionIndex},
		{generalmidi::midinotes::clave, percussionIndex},
		{generalmidi::midinotes::hiWoodBlock, percussionIndex},
		{generalmidi::midinotes::lowWoodBlock, percussionIndex},
		{generalmidi::midinotes::muteCuica, percussionIndex},
		{generalmidi::midinotes::openCuica, percussionIndex},
		{generalmidi::midinotes::muteTriangle, percussionIndex},
		{generalmidi::midinotes::openTriangle, percussionIndex}
	};
}
