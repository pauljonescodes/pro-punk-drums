#pragma once
#include <map>
#include "GeneralMidi.h"
#include <string>

namespace Channels {
	static constexpr int kickChannelIndex = 0;
	static constexpr int snareChannelIndex = 1;
	static constexpr int tomsChannelIndex = 2;
	static constexpr int hiHatChannelIndex = 3;
	static constexpr int cymbalsChannelIndex = 4;
	static constexpr int percussionChannelIndex = 5;
	static constexpr int outputChannelIndex = 6;

	static constexpr int size = 7;

	static const std::string kickId = "kick";
	static const std::string snareId = "snare";
	static const std::string tomsId = "toms";
	static const std::string hiHatId = "hi_hat";
	static const std::string cymbalsId = "cymbals";
	static const std::string percussionId = "perc";
	static const std::string outputId = "output";

	static const std::map<int, std::string> channelIndexToIdMap = {
		{ kickChannelIndex, kickId },
		{ snareChannelIndex, snareId },
		{ tomsChannelIndex, tomsId },
		{ hiHatChannelIndex, hiHatId },
		{ cymbalsChannelIndex, cymbalsId },
		{ percussionChannelIndex, percussionId },
		{ outputChannelIndex, outputId },
	};

	static const std::map<int, int> channelIndexToMainGeneralMidiNote = {
		{ kickChannelIndex, GeneralMidiPercussion::bassDrum1Note },
		{ snareChannelIndex, GeneralMidiPercussion::acousticSnareNote },
		{ tomsChannelIndex, GeneralMidiPercussion::highTomNote },
		{ hiHatChannelIndex, GeneralMidiPercussion::closedHiHatNote},
		{ cymbalsChannelIndex, GeneralMidiPercussion::crashCymbal2Note },
		{ percussionChannelIndex, GeneralMidiPercussion::tambourineNote },
		{ outputChannelIndex, GeneralMidiPercussion::notApplicable },
	};

	static const std::map<int, int> generalMidiNoteToChannelIndex = {
		{GeneralMidiPercussion::acousticBassDrumNote, kickChannelIndex},
		{GeneralMidiPercussion::bassDrum1Note, kickChannelIndex},
		{GeneralMidiPercussion::sideStickNote, snareChannelIndex},
		{GeneralMidiPercussion::acousticSnareNote, snareChannelIndex},
		{GeneralMidiPercussion::handClapNote, percussionChannelIndex},
		{GeneralMidiPercussion::electricSnareNote, snareChannelIndex},
		{GeneralMidiPercussion::lowFloorTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::closedHiHatNote, hiHatChannelIndex},
		{GeneralMidiPercussion::highFloorTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::pedalHiHatNote, hiHatChannelIndex},
		{GeneralMidiPercussion::lowTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::openHiHatNote, hiHatChannelIndex},
		{GeneralMidiPercussion::lowMidTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::hiMidTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::crashCymbal1Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::highTomNote, tomsChannelIndex},
		{GeneralMidiPercussion::rideCymbal1Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::chineseCymbalNote, cymbalsChannelIndex},
		{GeneralMidiPercussion::rideBellNote, cymbalsChannelIndex},
		{GeneralMidiPercussion::tambourineNote, percussionChannelIndex},
		{GeneralMidiPercussion::splashCymbalNote, cymbalsChannelIndex},
		{GeneralMidiPercussion::cowbellNote, percussionChannelIndex},
		{GeneralMidiPercussion::crashCymbal2Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::vibraslapNote, percussionChannelIndex},
		{GeneralMidiPercussion::rideCymbal2Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::hiBongoNote, percussionChannelIndex},
		{GeneralMidiPercussion::lowBongoNote, percussionChannelIndex},
		{GeneralMidiPercussion::muteHiCongaNote, percussionChannelIndex},
		{GeneralMidiPercussion::openHiCongaNote, percussionChannelIndex},
		{GeneralMidiPercussion::lowCongaNote, percussionChannelIndex},
		{GeneralMidiPercussion::highTimbaleNote, percussionChannelIndex},
		{GeneralMidiPercussion::lowTimbaleNote, percussionChannelIndex},
		{GeneralMidiPercussion::highAgogNote, percussionChannelIndex},
		{GeneralMidiPercussion::lowAgogNote, percussionChannelIndex},
		{GeneralMidiPercussion::cabasNote, percussionChannelIndex},
		{GeneralMidiPercussion::maracaNote, percussionChannelIndex},
		{GeneralMidiPercussion::shortWhistleNote, percussionChannelIndex},
		{GeneralMidiPercussion::longWhistleNote, percussionChannelIndex},
		{GeneralMidiPercussion::shortGuirNote, percussionChannelIndex},
		{GeneralMidiPercussion::longGuirNote, percussionChannelIndex},
		{GeneralMidiPercussion::claveNote, percussionChannelIndex},
		{GeneralMidiPercussion::hiWoodBlockNote, percussionChannelIndex},
		{GeneralMidiPercussion::lowWoodBlockNote, percussionChannelIndex},
		{GeneralMidiPercussion::muteCuicaNote, percussionChannelIndex},
		{GeneralMidiPercussion::openCuicaNote, percussionChannelIndex},
		{GeneralMidiPercussion::muteTriangleNote, percussionChannelIndex},
		{GeneralMidiPercussion::openTriangleNote, percussionChannelIndex}
	};
}
