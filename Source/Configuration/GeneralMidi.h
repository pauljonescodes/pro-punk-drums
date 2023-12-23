#pragma once
#include <vector>
#include <unordered_map>

namespace generalmidi
{
	namespace midinotes {
		static constexpr int notApplicable = -1;
		static constexpr int acousticBassDrum = 35;
		static constexpr int bassDrum1 = 36;
		static constexpr int sideStick = 37;
		static constexpr int acousticSnare = 38;
		static constexpr int handClap = 39;
		static constexpr int electricSnare = 40;
		static constexpr int lowFloorTom = 41;
		static constexpr int closedHiHat = 42;
		static constexpr int highFloorTom = 43;
		static constexpr int pedalHiHat = 44;
		static constexpr int lowTom = 45;
		static constexpr int openHiHat = 46;
		static constexpr int lowMidTom = 47;
		static constexpr int hiMidTom = 48;
		static constexpr int crashCymbal1 = 49;
		static constexpr int highTom = 50;
		static constexpr int rideCymbal1 = 51;
		static constexpr int chineseCymbal = 52;
		static constexpr int rideBell = 53;
		static constexpr int tambourine = 54;
		static constexpr int splashCymbal = 55;
		static constexpr int cowbell = 56;
		static constexpr int crashCymbal2 = 57;
		static constexpr int vibraslap = 58;
		static constexpr int rideCymbal2 = 59;
		static constexpr int hiBongo = 60;
		static constexpr int lowBongo = 61;
		static constexpr int muteHiConga = 62;
		static constexpr int openHiConga = 63;
		static constexpr int lowConga = 64;
		static constexpr int highTimbale = 65;
		static constexpr int lowTimbale = 66;
		static constexpr int highAgog = 67;
		static constexpr int lowAgog = 68;
		static constexpr int cabas = 69;
		static constexpr int maraca = 70;
		static constexpr int shortWhistle = 71;
		static constexpr int longWhistle = 72;
		static constexpr int shortGuir = 73;
		static constexpr int longGuir = 74;
		static constexpr int clave = 75;
		static constexpr int hiWoodBlock = 76;
		static constexpr int lowWoodBlock = 77;
		static constexpr int muteCuica = 78;
		static constexpr int openCuica = 79;
		static constexpr int muteTriangle = 80;
		static constexpr int openTriangle = 81;
	}

	static const std::vector<int> midiNotesVector = {
		midinotes::acousticBassDrum,
		midinotes::bassDrum1,
		midinotes::sideStick,
		midinotes::acousticSnare,
		midinotes::handClap,
		midinotes::electricSnare,
		midinotes::lowFloorTom,
		midinotes::closedHiHat,
		midinotes::highFloorTom,
		midinotes::pedalHiHat,
		midinotes::lowTom,
		midinotes::openHiHat,
		midinotes::lowMidTom,
		midinotes::hiMidTom,
		midinotes::crashCymbal1,
		midinotes::highTom,
		midinotes::rideCymbal1,
		midinotes::chineseCymbal,
		midinotes::rideBell,
		midinotes::tambourine,
		midinotes::splashCymbal,
		midinotes::cowbell,
		midinotes::crashCymbal2,
		midinotes::vibraslap,
		midinotes::rideCymbal2,
		midinotes::hiBongo,
		midinotes::lowBongo,
		midinotes::muteHiConga,
		midinotes::openHiConga,
		midinotes::lowConga,
		midinotes::highTimbale,
		midinotes::lowTimbale,
		midinotes::highAgog,
		midinotes::lowAgog,
		midinotes::cabas,
		midinotes::maraca,
		midinotes::shortWhistle,
		midinotes::longWhistle,
		midinotes::shortGuir,
		midinotes::longGuir,
		midinotes::clave,
		midinotes::hiWoodBlock,
		midinotes::lowWoodBlock,
		midinotes::muteCuica,
		midinotes::openCuica,
		midinotes::muteTriangle,
		midinotes::openTriangle
	};

	namespace names {
		static const std::string acousticBassDrum = "Acoustic Bass Drum";
		static const std::string bassDrum1 = "Bass Drum 1";
		static const std::string sideStick = "Side Stick";
		static const std::string acousticSnare = "Acoustic Snare";
		static const std::string handClap = "Hand Clap";
		static const std::string electricSnare = "Electric Snare";
		static const std::string lowFloorTom = "Low Floor Tom";
		static const std::string closedHiHat = "Closed Hi-Hat";
		static const std::string highFloorTom = "High Floor Tom";
		static const std::string pedalHiHat = "Pedal Hi-Hat";
		static const std::string lowTom = "Low Tom";
		static const std::string openHiHat = "Open Hi-Hat";
		static const std::string lowMidTom = "Low-Mid Tom";
		static const std::string hiMidTom = "Hi Mid Tom";
		static const std::string crashCymbal1 = "Crash Cymbal 1";
		static const std::string highTom = "High Tom";
		static const std::string rideCymbal1 = "Ride Cymbal 1";
		static const std::string chineseCymbal = "Chinese Cymbal";
		static const std::string rideBell = "Ride Bell";
		static const std::string tambourine = "Tambourine";
		static const std::string splashCymbal = "Splash Cymbal";
		static const std::string cowbell = "Cowbell";
		static const std::string crashCymbal2 = "Crash Cymbal 2";
		static const std::string vibraslap = "Vibraslap";
		static const std::string rideCymbal2 = "Ride Cymbal 2";
		static const std::string hiBongo = "Hi Bongo";
		static const std::string lowBongo = "Low Bongo";
		static const std::string muteHiConga = "Mute Hi Conga";
		static const std::string openHiConga = "Open Hi Conga";
		static const std::string lowConga = "Low Conga";
		static const std::string highTimbale = "High Timbale";
		static const std::string lowTimbale = "Low Timbale";
		static const std::string highAgog = "High Agog";
		static const std::string lowAgog = "Low Agog";
		static const std::string cabas = "Cabas";
		static const std::string maraca = "Maraca";
		static const std::string shortWhistle = "Short Whistle";
		static const std::string longWhistle = "Long Whistle";
		static const std::string shortGuir = "Short Guir";
		static const std::string longGuir = "Long Guir";
		static const std::string clave = "Clave";
		static const std::string hiWoodBlock = "Hi Wood Block";
		static const std::string lowWoodBlock = "Low Wood Block";
		static const std::string muteCuica = "Mute Cuica";
		static const std::string openCuica = "Open Cuica";
		static const std::string muteTriangle = "Mute Triangle";
		static const std::string openTriangle = "Open Triangle";
	}

	static const std::vector<std::string> midiNamesVector = {
		names::acousticBassDrum,
		names::bassDrum1,
		names::sideStick,
		names::acousticSnare,
		names::handClap,
		names::electricSnare,
		names::lowFloorTom,
		names::closedHiHat,
		names::highFloorTom,
		names::pedalHiHat,
		names::lowTom,
		names::openHiHat,
		names::lowMidTom,
		names::hiMidTom,
		names::crashCymbal1,
		names::highTom,
		names::rideCymbal1,
		names::chineseCymbal,
		names::rideBell,
		names::tambourine,
		names::splashCymbal,
		names::cowbell,
		names::crashCymbal2,
		names::vibraslap,
		names::rideCymbal2,
		names::hiBongo,
		names::lowBongo,
		names::muteHiConga,
		names::openHiConga,
		names::lowConga,
		names::highTimbale,
		names::lowTimbale,
		names::highAgog,
		names::lowAgog,
		names::cabas,
		names::maraca,
		names::shortWhistle,
		names::longWhistle,
		names::shortGuir,
		names::longGuir,
		names::clave,
		names::hiWoodBlock,
		names::lowWoodBlock,
		names::muteCuica,
		names::openCuica,
		names::muteTriangle,
		names::openTriangle
	};

	static const std::unordered_map<int, std::string> midiNoteToNameMap = {
		{midinotes::acousticBassDrum, names::acousticBassDrum},
		{midinotes::bassDrum1, names::bassDrum1},
		{midinotes::sideStick, names::sideStick},
		{midinotes::acousticSnare, names::acousticSnare},
		{midinotes::handClap, names::handClap},
		{midinotes::electricSnare, names::electricSnare},
		{midinotes::lowFloorTom, names::lowFloorTom},
		{midinotes::closedHiHat, names::closedHiHat},
		{midinotes::highFloorTom, names::highFloorTom},
		{midinotes::pedalHiHat, names::pedalHiHat},
		{midinotes::lowTom, names::lowTom},
		{midinotes::openHiHat, names::openHiHat},
		{midinotes::lowMidTom, names::lowMidTom},
		{midinotes::hiMidTom, names::hiMidTom},
		{midinotes::crashCymbal1, names::crashCymbal1},
		{midinotes::highTom, names::highTom},
		{midinotes::rideCymbal1, names::rideCymbal1},
		{midinotes::chineseCymbal, names::chineseCymbal},
		{midinotes::rideBell, names::rideBell},
		{midinotes::tambourine, names::tambourine},
		{midinotes::splashCymbal, names::splashCymbal},
		{midinotes::cowbell, names::cowbell},
		{midinotes::crashCymbal2, names::crashCymbal2},
		{midinotes::vibraslap, names::vibraslap},
		{midinotes::rideCymbal2, names::rideCymbal2},
		{midinotes::hiBongo, names::hiBongo},
		{midinotes::lowBongo, names::lowBongo},
		{midinotes::muteHiConga, names::muteHiConga},
		{midinotes::openHiConga, names::openHiConga},
		{midinotes::lowConga, names::lowConga},
		{midinotes::highTimbale, names::highTimbale},
		{midinotes::lowTimbale, names::lowTimbale},
		{midinotes::highAgog, names::highAgog},
		{midinotes::lowAgog, names::lowAgog},
		{midinotes::cabas, names::cabas},
		{midinotes::maraca, names::maraca},
		{midinotes::shortWhistle, names::shortWhistle},
		{midinotes::longWhistle, names::longWhistle},
		{midinotes::shortGuir, names::shortGuir},
		{midinotes::longGuir, names::longGuir},
		{midinotes::clave, names::clave},
		{midinotes::hiWoodBlock, names::hiWoodBlock},
		{midinotes::lowWoodBlock, names::lowWoodBlock},
		{midinotes::muteCuica, names::muteCuica},
		{midinotes::openCuica, names::openCuica},
		{midinotes::muteTriangle, names::muteTriangle},
		{midinotes::openTriangle, names::openTriangle}
	};

	static const std::unordered_map<std::string, int> midiNameToNoteMap = {
		{names::acousticBassDrum, midinotes::acousticBassDrum},
		{names::bassDrum1, midinotes::bassDrum1},
		{names::sideStick, midinotes::sideStick},
		{names::acousticSnare, midinotes::acousticSnare},
		{names::handClap, midinotes::handClap},
		{names::electricSnare, midinotes::electricSnare},
		{names::lowFloorTom, midinotes::lowFloorTom},
		{names::closedHiHat, midinotes::closedHiHat},
		{names::highFloorTom, midinotes::highFloorTom},
		{names::pedalHiHat, midinotes::pedalHiHat},
		{names::lowTom, midinotes::lowTom},
		{names::openHiHat, midinotes::openHiHat},
		{names::lowMidTom, midinotes::lowMidTom},
		{names::hiMidTom, midinotes::hiMidTom},
		{names::crashCymbal1, midinotes::crashCymbal1},
		{names::highTom, midinotes::highTom},
		{names::rideCymbal1, midinotes::rideCymbal1},
		{names::chineseCymbal, midinotes::chineseCymbal},
		{names::rideBell, midinotes::rideBell},
		{names::tambourine, midinotes::tambourine},
		{names::splashCymbal, midinotes::splashCymbal},
		{names::cowbell, midinotes::cowbell},
		{names::crashCymbal2, midinotes::crashCymbal2},
		{names::vibraslap, midinotes::vibraslap},
		{names::rideCymbal2, midinotes::rideCymbal2},
		{names::hiBongo, midinotes::hiBongo},
		{names::lowBongo, midinotes::lowBongo},
		{names::muteHiConga, midinotes::muteHiConga},
		{names::openHiConga, midinotes::openHiConga},
		{names::lowConga, midinotes::lowConga},
		{names::highTimbale, midinotes::highTimbale},
		{names::lowTimbale, midinotes::lowTimbale},
		{names::highAgog, midinotes::highAgog},
		{names::lowAgog, midinotes::lowAgog},
		{names::cabas, midinotes::cabas},
		{names::maraca, midinotes::maraca},
		{names::shortWhistle, midinotes::shortWhistle},
		{names::longWhistle, midinotes::longWhistle},
		{names::shortGuir, midinotes::shortGuir},
		{names::longGuir, midinotes::longGuir},
		{names::clave, midinotes::clave},
		{names::hiWoodBlock, midinotes::hiWoodBlock},
		{names::lowWoodBlock, midinotes::lowWoodBlock},
		{names::muteCuica, midinotes::muteCuica},
		{names::openCuica, midinotes::openCuica},
		{names::muteTriangle, midinotes::muteTriangle},
		{names::openTriangle, midinotes::openTriangle}
	};

	static const std::unordered_map<int, std::vector<int>> midiNoteToStopNotesMap = {
		{midinotes::acousticBassDrum, {}},
		{midinotes::bassDrum1, {}},
		{midinotes::sideStick, {}},
		{midinotes::acousticSnare, {}},
		{midinotes::handClap, {}},
		{midinotes::electricSnare, {}},
		{midinotes::lowFloorTom, {}},
		{midinotes::closedHiHat, {midinotes::openHiHat}},
		{midinotes::highFloorTom, {}},
		{midinotes::pedalHiHat, {midinotes::openHiHat}},
		{midinotes::lowTom, {}},
		{midinotes::openHiHat, {}},
		{midinotes::lowMidTom, {}},
		{midinotes::hiMidTom, {}},
		{midinotes::crashCymbal1, {}},
		{midinotes::highTom, {}},
		{midinotes::rideCymbal1, {}},
		{midinotes::chineseCymbal, {}},
		{midinotes::rideBell, {}},
		{midinotes::tambourine, {}},
		{midinotes::splashCymbal, {}},
		{midinotes::cowbell, {}},
		{midinotes::crashCymbal2, {}},
		{midinotes::vibraslap, {}},
		{midinotes::rideCymbal2, {}},
		{midinotes::hiBongo, {}},
		{midinotes::lowBongo, {}},
		{midinotes::muteHiConga, {}},
		{midinotes::openHiConga, {}},
		{midinotes::lowConga, {}},
		{midinotes::highTimbale, {}},
		{midinotes::lowTimbale, {}},
		{midinotes::highAgog, {}},
		{midinotes::lowAgog, {}},
		{midinotes::cabas, {}},
		{midinotes::maraca, {}},
		{midinotes::shortWhistle, {}},
		{midinotes::longWhistle, {}},
		{midinotes::shortGuir, {}},
		{midinotes::longGuir, {}},
		{midinotes::clave, {}},
		{midinotes::hiWoodBlock, {}},
		{midinotes::lowWoodBlock, {}},
		{midinotes::muteCuica, {}},
		{midinotes::openCuica, {}},
		{midinotes::muteTriangle, {midinotes::openTriangle}},
		{midinotes::openTriangle, {midinotes::muteTriangle}}
	};
}
