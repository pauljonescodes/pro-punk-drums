/*
  ==============================================================================

	SampledInstrument.h
	Created: 6 Dec 2023 11:50:39am
	Author:  paulm

  ==============================================================================
*/

#pragma once
#include <stdexcept>
#include <vector>
#include <map>
#include "Constants.h"
#include "GeneralMidi.h"

namespace samples {

	static const int standardVariationsCount = 5;

	enum class InstrumentEnum {
		acousticBassDrum,
		bassDrum1,
		sideStick,
		acousticSnare,
		handClap,
		electricSnare,
		hiHatEdgeTight,
		hiHatTipClosed,
		hiHatTipTight,
		hiHatEdgeClosed,
		tomFloorHigh,
		hiHatPedal,
		hiHatOpen3,
		hiHatOpen2,
		hiHatOpen1,
		crashCymbal1,
		tomHigh,
		rideCymbal1,
		chineseCymbal,
		rideBell,
		tambourine,
		splashCymbal,
		cowbell,
		crashCymbal2,
		maraca,
		triangleMute,
		triangleOpen
	};

	static const std::vector<InstrumentEnum> instrumentVector = {
		InstrumentEnum::acousticBassDrum,
		InstrumentEnum::bassDrum1,
		InstrumentEnum::sideStick,
		InstrumentEnum::acousticSnare,
		InstrumentEnum::handClap,
		InstrumentEnum::electricSnare,
		InstrumentEnum::hiHatEdgeTight,
		InstrumentEnum::hiHatTipClosed,
		InstrumentEnum::hiHatTipTight,
		InstrumentEnum::hiHatEdgeClosed,
		InstrumentEnum::tomFloorHigh,
		InstrumentEnum::hiHatPedal,
		InstrumentEnum::hiHatOpen3,
		InstrumentEnum::hiHatOpen2,
		InstrumentEnum::hiHatOpen1,
		InstrumentEnum::crashCymbal1,
		InstrumentEnum::tomHigh,
		InstrumentEnum::rideCymbal1,
		InstrumentEnum::chineseCymbal,
		InstrumentEnum::rideBell,
		InstrumentEnum::tambourine,
		InstrumentEnum::splashCymbal,
		InstrumentEnum::cowbell,
		InstrumentEnum::crashCymbal2,
		InstrumentEnum::maraca,
		InstrumentEnum::triangleMute,
		InstrumentEnum::triangleOpen
	};

	static const std::map<InstrumentEnum, std::string> instrumentIdMap = {
		{InstrumentEnum::acousticBassDrum, constants::acousticBassDrumId},
		{InstrumentEnum::bassDrum1, constants::bassDrum1Id},
		{InstrumentEnum::sideStick, constants::sideStickId},
		{InstrumentEnum::acousticSnare, constants::acousticSnareId},
		{InstrumentEnum::handClap, constants::handClapId},
		{InstrumentEnum::electricSnare, constants::electricSnareId},
		{InstrumentEnum::hiHatEdgeTight, constants::hiHatEdgeTightId},
		{InstrumentEnum::hiHatTipClosed, constants::hiHatTipClosedId},
		{InstrumentEnum::hiHatTipTight, constants::hiHatTipTightId},
		{InstrumentEnum::hiHatEdgeClosed, constants::hiHatEdgeClosedId},
		{InstrumentEnum::tomFloorHigh, constants::tomFloorHighId},
		{InstrumentEnum::hiHatPedal, constants::hiHatPedalId},
		{InstrumentEnum::hiHatOpen3, constants::hiHatOpen3Id},
		{InstrumentEnum::hiHatOpen2, constants::hiHatOpen2Id},
		{InstrumentEnum::hiHatOpen1, constants::hiHatOpen1Id},
		{InstrumentEnum::crashCymbal1, constants::crashCymbal1Id},
		{InstrumentEnum::tomHigh, constants::tomHighId},
		{InstrumentEnum::rideCymbal1, constants::rideCymbal1Id},
		{InstrumentEnum::chineseCymbal, constants::chineseCymbalId},
		{InstrumentEnum::rideBell, constants::rideBellId},
		{InstrumentEnum::tambourine, constants::tambourineId},
		{InstrumentEnum::splashCymbal, constants::splashCymbalId},
		{InstrumentEnum::cowbell, constants::cowbellId},
		{InstrumentEnum::crashCymbal2, constants::crashCymbal2Id},
		{InstrumentEnum::maraca, constants::maracaId},
		{InstrumentEnum::triangleMute, constants::triangleMuteId},
		{InstrumentEnum::triangleOpen, constants::triangleOpenId}
	};
	
	//==============================================================================
	// Intensity
	//==============================================================================

	enum class IntensityEnum {
		soft,
		medium,
		hard
	};

	static const std::vector<IntensityEnum> intensityVector = {
	IntensityEnum::soft,
	IntensityEnum::medium,
	IntensityEnum::hard,
	};


	static const std::map<IntensityEnum, std::string> intensityIdMap = {
		{IntensityEnum::soft, constants::softIntensityId},
		{IntensityEnum::medium, constants::mediumIntensityId},
		{IntensityEnum::hard, constants::hardIntensityId}
		// Add more entries for other velocity types as needed...
	};

	//==============================================================================
	// Cymbal mics
	//==============================================================================

	enum class CymbalMicrophoneEnum {
		cymbal,
		overheadLeft,
		overheadRight
	};

	static const std::vector<CymbalMicrophoneEnum> cymbalMicrophoneVector = {
	CymbalMicrophoneEnum::cymbal,
	CymbalMicrophoneEnum::overheadLeft,
	CymbalMicrophoneEnum::overheadRight,
	};

	static const std::map<CymbalMicrophoneEnum, std::string> cymbalMicrophoneIdMap = {
		{CymbalMicrophoneEnum::cymbal, constants::cymbalMicrophoneId},
		{CymbalMicrophoneEnum::overheadLeft, constants::overheadLeftMicrophoneId},
		{CymbalMicrophoneEnum::overheadRight, constants::overheadRightMicrophoneId}	};

	//==============================================================================
	// Acoustic Snare Microphones
	//==============================================================================

	enum class AcousticSnareMicrophoneEnum {
		top,
		bottom
	};

	static const std::vector<AcousticSnareMicrophoneEnum> allAcousticSnareMicrophones = {
		AcousticSnareMicrophoneEnum::top,
		AcousticSnareMicrophoneEnum::bottom,
	};

	static const std::map<AcousticSnareMicrophoneEnum, std::string> acousticSnareMicrophoneIdMap = {
	{AcousticSnareMicrophoneEnum::top, constants::acousticSnareMicrophoneTopId},
	{AcousticSnareMicrophoneEnum::bottom, constants::acousticSnareMicrophoneBottomId}
	};

	//==============================================================================
	// Acoustic Bass Drum Microphones
	//==============================================================================

	enum class AcousticBassDrumMicrophoneEnum {
		in,
		out
	};

	static const std::vector<AcousticBassDrumMicrophoneEnum> acousticBassDrumMicrophoneVector = {
		AcousticBassDrumMicrophoneEnum::in,
		AcousticBassDrumMicrophoneEnum::out,
	};

	static const std::map<AcousticBassDrumMicrophoneEnum, std::string> acousticBassDrumMicrophoneIdMap = {
	{AcousticBassDrumMicrophoneEnum::in, constants::acousticBassDrumMicrophoneInId},
	{AcousticBassDrumMicrophoneEnum::out, constants::acousticBassDrumMicrophoneOutId}
	};
	
	//==============================================================================
	// General Midi
	//==============================================================================

	const std::vector<int> generalMidiNotesVector = {
	generalmidi::midinotes::acousticBassDrum,
	generalmidi::midinotes::bassDrum1,
	generalmidi::midinotes::sideStick,
	generalmidi::midinotes::acousticSnare,
	generalmidi::midinotes::handClap,
	generalmidi::midinotes::electricSnare,
	generalmidi::midinotes::closedHiHat,
	generalmidi::midinotes::highFloorTom,
	generalmidi::midinotes::pedalHiHat,
	generalmidi::midinotes::openHiHat,
	generalmidi::midinotes::crashCymbal1,
	generalmidi::midinotes::highTom,
	generalmidi::midinotes::rideCymbal1,
	generalmidi::midinotes::chineseCymbal,
	generalmidi::midinotes::rideBell,
	generalmidi::midinotes::tambourine,
	generalmidi::midinotes::splashCymbal,
	generalmidi::midinotes::cowbell,
	generalmidi::midinotes::crashCymbal2,
	generalmidi::midinotes::maraca,
	generalmidi::midinotes::muteTriangle,
	generalmidi::midinotes::openTriangle
	};

	const std::map<InstrumentEnum, int> instrumentToGeneralMidiNoteMap = {
	{InstrumentEnum::acousticBassDrum, generalmidi::midinotes::acousticBassDrum},
	{InstrumentEnum::bassDrum1, generalmidi::midinotes::bassDrum1},
	{InstrumentEnum::sideStick, generalmidi::midinotes::sideStick},
	{InstrumentEnum::acousticSnare, generalmidi::midinotes::acousticSnare},
	{InstrumentEnum::handClap, generalmidi::midinotes::handClap},
	{InstrumentEnum::electricSnare, generalmidi::midinotes::electricSnare},
	{InstrumentEnum::hiHatEdgeTight, generalmidi::midinotes::closedHiHat},
	{InstrumentEnum::hiHatTipClosed, generalmidi::midinotes::closedHiHat},
	{InstrumentEnum::hiHatTipTight, generalmidi::midinotes::closedHiHat},
	{InstrumentEnum::hiHatEdgeClosed, generalmidi::midinotes::closedHiHat},
	{InstrumentEnum::tomFloorHigh, generalmidi::midinotes::highFloorTom},
	{InstrumentEnum::hiHatPedal, generalmidi::midinotes::pedalHiHat},
	{InstrumentEnum::hiHatOpen3, generalmidi::midinotes::openHiHat},
	{InstrumentEnum::hiHatOpen2, generalmidi::midinotes::openHiHat},
	{InstrumentEnum::hiHatOpen1, generalmidi::midinotes::openHiHat},
	{InstrumentEnum::crashCymbal1, generalmidi::midinotes::crashCymbal1},
	{InstrumentEnum::tomHigh, generalmidi::midinotes::highTom},
	{InstrumentEnum::rideCymbal1, generalmidi::midinotes::rideCymbal1},
	{InstrumentEnum::chineseCymbal, generalmidi::midinotes::chineseCymbal},
	{InstrumentEnum::rideBell, generalmidi::midinotes::rideBell},
	{InstrumentEnum::tambourine, generalmidi::midinotes::tambourine},
	{InstrumentEnum::splashCymbal, generalmidi::midinotes::splashCymbal},
	{InstrumentEnum::cowbell, generalmidi::midinotes::cowbell},
	{InstrumentEnum::crashCymbal2, generalmidi::midinotes::crashCymbal2},
	{InstrumentEnum::maraca, generalmidi::midinotes::maraca},
	{InstrumentEnum::triangleMute, generalmidi::midinotes::muteTriangle},
	{InstrumentEnum::triangleOpen, generalmidi::midinotes::openTriangle}
	};

	const std::map<int, InstrumentEnum> generalMidiNoteToInstrumentMap = {
	{generalmidi::midinotes::acousticBassDrum, InstrumentEnum::acousticBassDrum},
	{generalmidi::midinotes::bassDrum1, InstrumentEnum::bassDrum1},
	{generalmidi::midinotes::sideStick, InstrumentEnum::sideStick},
	{generalmidi::midinotes::acousticSnare, InstrumentEnum::acousticSnare},
	{generalmidi::midinotes::handClap, InstrumentEnum::handClap},
	{generalmidi::midinotes::electricSnare, InstrumentEnum::electricSnare},
	{generalmidi::midinotes::closedHiHat, InstrumentEnum::hiHatEdgeTight},
	{generalmidi::midinotes::highFloorTom, InstrumentEnum::tomFloorHigh},
	{generalmidi::midinotes::pedalHiHat, InstrumentEnum::hiHatPedal},
	{generalmidi::midinotes::openHiHat, InstrumentEnum::hiHatOpen2},
	{generalmidi::midinotes::crashCymbal1, InstrumentEnum::crashCymbal1},
	{generalmidi::midinotes::highTom, InstrumentEnum::tomHigh},
	{generalmidi::midinotes::rideCymbal1, InstrumentEnum::rideCymbal1},
	{generalmidi::midinotes::chineseCymbal, InstrumentEnum::chineseCymbal},
	{generalmidi::midinotes::rideBell, InstrumentEnum::rideBell},
	{generalmidi::midinotes::tambourine, InstrumentEnum::tambourine},
	{generalmidi::midinotes::splashCymbal, InstrumentEnum::splashCymbal},
	{generalmidi::midinotes::cowbell, InstrumentEnum::cowbell},
	{generalmidi::midinotes::crashCymbal2, InstrumentEnum::crashCymbal2},
	{generalmidi::midinotes::maraca, InstrumentEnum::maraca},
	{generalmidi::midinotes::muteTriangle, InstrumentEnum::triangleMute},
	{generalmidi::midinotes::openTriangle, InstrumentEnum::triangleOpen}
	};
};