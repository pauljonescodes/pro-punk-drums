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
	static constexpr int otherChannelIndex = 5;
	static constexpr int roomChannelIndex = 6;
	static constexpr int outputChannelIndex = 7;

	static constexpr int size = 8;

	static const std::string kickId = "kick";
	static const std::string snareId = "snare";
	static const std::string tomsId = "toms";
	static const std::string hiHatId = "hi_hat";
	static const std::string cymbalsId = "cymbals";
	static const std::string otherId = "other";
	static const std::string roomId = "room";
	static const std::string outputId = "output";

	static const std::map<int, std::string> channelIndexToIdMap = {
		{ kickChannelIndex, kickId },
		{ snareChannelIndex, snareId },
		{ tomsChannelIndex, tomsId },
		{ hiHatChannelIndex, hiHatId },
		{ cymbalsChannelIndex, cymbalsId },
		{ otherChannelIndex, otherId },
		{ roomChannelIndex, roomId },
		{ outputChannelIndex, outputId },
	};

	static const std::map<int, int> channelIndexToGeneralMidiPerccussionNote = {
		{ kickChannelIndex, GeneralMidiPercussion::bassDrum1Note },
		{ snareChannelIndex, GeneralMidiPercussion::acousticSnareNote },
		{ tomsChannelIndex, GeneralMidiPercussion::highTomNote },
		{ hiHatChannelIndex, GeneralMidiPercussion::closedHiHatNote},
		{ cymbalsChannelIndex, GeneralMidiPercussion::crashCymbal2Note },
		{ otherChannelIndex, GeneralMidiPercussion::tambourineNote },
		{ roomChannelIndex, GeneralMidiPercussion::notApplicable },
		{ outputChannelIndex, GeneralMidiPercussion::notApplicable },
	};

	static const std::map<int, int> generalMidiNoteToChannelIndex = {
		{GeneralMidiPercussion::acousticBassDrumNote, kickChannelIndex},
		{GeneralMidiPercussion::bassDrum1Note, kickChannelIndex},
		{GeneralMidiPercussion::sideStickNote, snareChannelIndex},
		{GeneralMidiPercussion::acousticSnareNote, snareChannelIndex},
		{GeneralMidiPercussion::handClapNote, otherChannelIndex},
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
		{GeneralMidiPercussion::tambourineNote, otherChannelIndex},
		{GeneralMidiPercussion::splashCymbalNote, cymbalsChannelIndex},
		{GeneralMidiPercussion::cowbellNote, otherChannelIndex},
		{GeneralMidiPercussion::crashCymbal2Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::vibraslapNote, otherChannelIndex},
		{GeneralMidiPercussion::rideCymbal2Note, cymbalsChannelIndex},
		{GeneralMidiPercussion::hiBongoNote, otherChannelIndex},
		{GeneralMidiPercussion::lowBongoNote, otherChannelIndex},
		{GeneralMidiPercussion::muteHiCongaNote, otherChannelIndex},
		{GeneralMidiPercussion::openHiCongaNote, otherChannelIndex},
		{GeneralMidiPercussion::lowCongaNote, otherChannelIndex},
		{GeneralMidiPercussion::highTimbaleNote, otherChannelIndex},
		{GeneralMidiPercussion::lowTimbaleNote, otherChannelIndex},
		{GeneralMidiPercussion::highAgogNote, otherChannelIndex},
		{GeneralMidiPercussion::lowAgogNote, otherChannelIndex},
		{GeneralMidiPercussion::cabasNote, otherChannelIndex},
		{GeneralMidiPercussion::maracaNote, otherChannelIndex},
		{GeneralMidiPercussion::shortWhistleNote, otherChannelIndex},
		{GeneralMidiPercussion::longWhistleNote, otherChannelIndex},
		{GeneralMidiPercussion::shortGuirNote, otherChannelIndex},
		{GeneralMidiPercussion::longGuirNote, otherChannelIndex},
		{GeneralMidiPercussion::claveNote, otherChannelIndex},
		{GeneralMidiPercussion::hiWoodBlockNote, otherChannelIndex},
		{GeneralMidiPercussion::lowWoodBlockNote, otherChannelIndex},
		{GeneralMidiPercussion::muteCuicaNote, otherChannelIndex},
		{GeneralMidiPercussion::openCuicaNote, otherChannelIndex},
		{GeneralMidiPercussion::muteTriangleNote, otherChannelIndex},
		{GeneralMidiPercussion::openTriangleNote, otherChannelIndex}
	};
}
