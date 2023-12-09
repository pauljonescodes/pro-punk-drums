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
	const int multiOutChannelAcousticBassDrum = 0;
	const int multiOutChannelAcousticSnare = 1;
	const int multiOutChannelTomHigh = 2;
	const int multiOutChannelTomFloorHigh = 3;
	const int multiOutChannelHiHat = 4; // HiHatEdgeTight hiHatTipClosed hiHatTipTight hiHatEdgeClosed hiHatPedal hiHatOpen3 hiHatOpen2 hiHatOpen1
	const int multiOutChannelRide = 5; // RideBell RideCymbal1
	const int multiOutChannelCrashCymbal1 = 6;
	const int multiOutChannelChineseCymbal = 7;
	const int multiOutChannelSplashCymbal = 8;
	const int multiOutChannelCrashCymbal2 = 9;
	const int multiOutChannelOverheadLeft = 10;
	const int multiOutChannelOverheadRight = 11;
	const int multiOutChannelPercussion = 12; // maraca triangleMute triangleOpen cowbell handClap sideStick tambourine
	const int multiOutChannelBassDrum1 = 13;
	const int multiOutChannelElectricSnare = 14;

	const std::string acousticBassDrumId = "acoustic_bass_drum";
	const std::string bassDrum1Id = "bass_drum1";
	const std::string sideStickId = "side_stick";
	const std::string acousticSnareId = "acoustic_snare";
	const std::string handClapId = "hand_clap";
	const std::string electricSnareId = "electric_snare";
	const std::string hiHatEdgeTightId = "hi_hat_edge_tight";
	const std::string hiHatTipClosedId = "hi_hat_tip_closed";
	const std::string hiHatTipTightId = "hi_hat_tip_tight";
	const std::string hiHatEdgeClosedId = "hi_hat_edge_closed";
	const std::string tomFloorHighId = "tom_floor_high";
	const std::string hiHatPedalId = "hi_hat_pedal";
	const std::string hiHatOpen3Id = "hi_hat_open3";
	const std::string hiHatOpen2Id = "hi_hat_open2";
	const std::string hiHatOpen1Id = "hi_hat_open1";
	const std::string crashCymbal1Id = "crash_cymbal1";
	const std::string tomHighId = "tom_high";
	const std::string rideCymbal1Id = "ride_cymbal1";
	const std::string chineseCymbalId = "chinese_cymbal";
	const std::string rideBellId = "ride_bell";
	const std::string tambourineId = "tambourine";
	const std::string splashCymbalId = "splash_cymbal";
	const std::string cowbellId = "cowbell";
	const std::string crashCymbal2Id = "crash_cymbal2";
	const std::string maracaId = "maraca";
	const std::string triangleMuteId = "triangle_mute";
	const std::string triangleOpenId = "triangle_open";

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