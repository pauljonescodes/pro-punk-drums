/*
 ==============================================================================

 SampledInstrument.h
 Created: 6 Dec 2023 11:50:39am
 Author:  paulm

 ==============================================================================
 */

#pragma once

#include <vector>
#include <map>
#include "Constants.h"
#include "GeneralMidi.h"

namespace samples {
	static const int bitRate = 44100;
	static const int bitDepth = 24;

	static const std::vector<std::string> samplesVector = {
	"acoustic_bass_drum_in",
	"acoustic_bass_drum_out",
	"bass_drum_1",
	"side_stick",
	"acoustic_snare_bottom",
	"acoustic_snare_top",
	"hand_clap",
	"electric_snare",
	"closed_hi_hat",
	"closed_hi_hat_left",
	"closed_hi_hat_right",
	"high_floor_tom",
	"pedal_hi_hat",
	"pedal_hi_hat_left",
	"pedal_hi_hat_right",
	"open_hi_hat",
	"open_hi_hat_left",
	"open_hi_hat_right",
	"crash_cymbal_1",
	"crash_cymbal_1_left",
	"crash_cymbal_1_right",
	"high_tom",
	"ride_cymbal_1",
	"ride_cymbal_1_left",
	"ride_cymbal_1_right",
	"chinese_cymbal",
	"chinese_cymbal_left",
	"chinese_cymbal_right",
	"ride_bell",
	"ride_bell_left",
	"ride_bell_right",
	"tambourine",
	"splash_cymbal",
	"splash_cymbal_left",
	"splash_cymbal_right",
	"cowbell",
	"crash_cymbal_2",
	"crash_cymbal_2_left",
	"crash_cymbal_2_right",
	"maraca",
	"mute_triangle",
	"open_triangle"
	};

	static const std::vector<std::string> micSuffixes = { "_in", "_out", "_top", "_bottom", "_left", "_right" };

};
