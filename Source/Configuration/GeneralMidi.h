#pragma once
#include <vector>
#include <unordered_map>

namespace GeneralMidiPercussion
{
	static constexpr int notApplicable = -1;
	static constexpr int acousticBassDrumNote = 35;
	static constexpr int bassDrum1Note = 36;
	static constexpr int sideStickNote = 37;
	static constexpr int acousticSnareNote = 38;
	static constexpr int handClapNote = 39;
	static constexpr int electricSnareNote = 40;
	static constexpr int lowFloorTomNote = 41;
	static constexpr int closedHiHatNote = 42;
	static constexpr int highFloorTomNote = 43;
	static constexpr int pedalHiHatNote = 44;
	static constexpr int lowTomNote = 45;
	static constexpr int openHiHatNote = 46;
	static constexpr int lowMidTomNote = 47;
	static constexpr int hiMidTomNote = 48;
	static constexpr int crashCymbal1Note = 49;
	static constexpr int highTomNote = 50;
	static constexpr int rideCymbal1Note = 51;
	static constexpr int chineseCymbalNote = 52;
	static constexpr int rideBellNote = 53;
	static constexpr int tambourineNote = 54;
	static constexpr int splashCymbalNote = 55;
	static constexpr int cowbellNote = 56;
	static constexpr int crashCymbal2Note = 57;
	static constexpr int vibraslapNote = 58;
	static constexpr int rideCymbal2Note = 59;
	static constexpr int hiBongoNote = 60;
	static constexpr int lowBongoNote = 61;
	static constexpr int muteHiCongaNote = 62;
	static constexpr int openHiCongaNote = 63;
	static constexpr int lowCongaNote = 64;
	static constexpr int highTimbaleNote = 65;
	static constexpr int lowTimbaleNote = 66;
	static constexpr int highAgogNote = 67;
	static constexpr int lowAgogNote = 68;
	static constexpr int cabasNote = 69;
	static constexpr int maracaNote = 70;
	static constexpr int shortWhistleNote = 71;
	static constexpr int longWhistleNote = 72;
	static constexpr int shortGuirNote = 73;
	static constexpr int longGuirNote = 74;
	static constexpr int claveNote = 75;
	static constexpr int hiWoodBlockNote = 76;
	static constexpr int lowWoodBlockNote = 77;
	static constexpr int muteCuicaNote = 78;
	static constexpr int openCuicaNote = 79;
	static constexpr int muteTriangleNote = 80;
	static constexpr int openTriangleNote = 81;

	static const std::string acousticBassDrumName = "Acoustic Bass Drum";
	static const std::string bassDrum1Name = "Bass Drum 1";
	static const std::string sideStickName = "Side Stick";
	static const std::string acousticSnareName = "Acoustic Snare";
	static const std::string handClapName = "Hand Clap";
	static const std::string electricSnareName = "Electric Snare";
	static const std::string lowFloorTomName = "Low Floor Tom";
	static const std::string closedHiHatName = "Closed Hi-Hat";
	static const std::string highFloorTomName = "High Floor Tom";
	static const std::string pedalHiHatName = "Pedal Hi-Hat";
	static const std::string lowTomName = "Low Tom";
	static const std::string openHiHatName = "Open Hi-Hat";
	static const std::string lowMidTomName = "Low-Mid Tom";
	static const std::string hiMidTomName = "Hi Mid Tom";
	static const std::string crashCymbal1Name = "Crash Cymbal 1";
	static const std::string highTomName = "High Tom";
	static const std::string rideCymbal1Name = "Ride Cymbal 1";
	static const std::string chineseCymbalName = "Chinese Cymbal";
	static const std::string rideBellName = "Ride Bell";
	static const std::string tambourineName = "Tambourine";
	static const std::string splashCymbalName = "Splash Cymbal";
	static const std::string cowbellName = "Cowbell";
	static const std::string crashCymbal2Name = "Crash Cymbal 2";
	static const std::string vibraslapName = "Vibraslap";
	static const std::string rideCymbal2Name = "Ride Cymbal 2";
	static const std::string hiBongoName = "Hi Bongo";
	static const std::string lowBongoName = "Low Bongo";
	static const std::string muteHiCongaName = "Mute Hi Conga";
	static const std::string openHiCongaName = "Open Hi Conga";
	static const std::string lowCongaName = "Low Conga";
	static const std::string highTimbaleName = "High Timbale";
	static const std::string lowTimbaleName = "Low Timbale";
	static const std::string highAgogName = "High Agog";
	static const std::string lowAgogName = "Low Agog";
	static const std::string cabasName = "Cabas";
	static const std::string maracaName = "Maraca";
	static const std::string shortWhistleName = "Short Whistle";
	static const std::string longWhistleName = "Long Whistle";
	static const std::string shortGuirName = "Short Guir";
	static const std::string longGuirName = "Long Guir";
	static const std::string claveName = "Clave";
	static const std::string hiWoodBlockName = "Hi Wood Block";
	static const std::string lowWoodBlockName = "Low Wood Block";
	static const std::string muteCuicaName = "Mute Cuica";
	static const std::string openCuicaName = "Open Cuica";
	static const std::string muteTriangleName = "Mute Triangle";
	static const std::string openTriangleName = "Open Triangle";


	static const std::vector<int> midiNotesVector = {
		acousticBassDrumNote,
		bassDrum1Note,
		sideStickNote,
		acousticSnareNote,
		handClapNote,
		electricSnareNote,
		lowFloorTomNote,
		closedHiHatNote,
		highFloorTomNote,
		pedalHiHatNote,
		lowTomNote,
		openHiHatNote,
		lowMidTomNote,
		hiMidTomNote,
		crashCymbal1Note,
		highTomNote,
		rideCymbal1Note,
		chineseCymbalNote,
		rideBellNote,
		tambourineNote,
		splashCymbalNote,
		cowbellNote,
		crashCymbal2Note,
		vibraslapNote,
		rideCymbal2Note,
		hiBongoNote,
		lowBongoNote,
		muteHiCongaNote,
		openHiCongaNote,
		lowCongaNote,
		highTimbaleNote,
		lowTimbaleNote,
		highAgogNote,
		lowAgogNote,
		cabasNote,
		maracaNote,
		shortWhistleNote,
		longWhistleNote,
		shortGuirNote,
		longGuirNote,
		claveNote,
		hiWoodBlockNote,
		lowWoodBlockNote,
		muteCuicaNote,
		openCuicaNote,
		muteTriangleNote,
		openTriangleNote
	};

	static const std::vector<std::string> midiNamesVector = {
		acousticBassDrumName,
		bassDrum1Name,
		sideStickName,
		acousticSnareName,
		handClapName,
		electricSnareName,
		lowFloorTomName,
		closedHiHatName,
		highFloorTomName,
		pedalHiHatName,
		lowTomName,
		openHiHatName,
		lowMidTomName,
		hiMidTomName,
		crashCymbal1Name,
		highTomName,
		rideCymbal1Name,
		chineseCymbalName,
		rideBellName,
		tambourineName,
		splashCymbalName,
		cowbellName,
		crashCymbal2Name,
		vibraslapName,
		rideCymbal2Name,
		hiBongoName,
		lowBongoName,
		muteHiCongaName,
		openHiCongaName,
		lowCongaName,
		highTimbaleName,
		lowTimbaleName,
		highAgogName,
		lowAgogName,
		cabasName,
		maracaName,
		shortWhistleName,
		longWhistleName,
		shortGuirName,
		longGuirName,
		claveName,
		hiWoodBlockName,
		lowWoodBlockName,
		muteCuicaName,
		openCuicaName,
		muteTriangleName,
		openTriangleName
	};

	static const std::unordered_map<int, std::string> midiNoteToNameMap = {
		{acousticBassDrumNote, acousticBassDrumName},
		{bassDrum1Note, bassDrum1Name},
		{sideStickNote, sideStickName},
		{acousticSnareNote, acousticSnareName},
		{handClapNote, handClapName},
		{electricSnareNote, electricSnareName},
		{lowFloorTomNote, lowFloorTomName},
		{closedHiHatNote, closedHiHatName},
		{highFloorTomNote, highFloorTomName},
		{pedalHiHatNote, pedalHiHatName},
		{lowTomNote, lowTomName},
		{openHiHatNote, openHiHatName},
		{lowMidTomNote, lowMidTomName},
		{hiMidTomNote, hiMidTomName},
		{crashCymbal1Note, crashCymbal1Name},
		{highTomNote, highTomName},
		{rideCymbal1Note, rideCymbal1Name},
		{chineseCymbalNote, chineseCymbalName},
		{rideBellNote, rideBellName},
		{tambourineNote, tambourineName},
		{splashCymbalNote, splashCymbalName},
		{cowbellNote, cowbellName},
		{crashCymbal2Note, crashCymbal2Name},
		{vibraslapNote, vibraslapName},
		{rideCymbal2Note, rideCymbal2Name},
		{hiBongoNote, hiBongoName},
		{lowBongoNote, lowBongoName},
		{muteHiCongaNote, muteHiCongaName},
		{openHiCongaNote, openHiCongaName},
		{lowCongaNote, lowCongaName},
		{highTimbaleNote, highTimbaleName},
		{lowTimbaleNote, lowTimbaleName},
		{highAgogNote, highAgogName},
		{lowAgogNote, lowAgogName},
		{cabasNote, cabasName},
		{maracaNote, maracaName},
		{shortWhistleNote, shortWhistleName},
		{longWhistleNote, longWhistleName},
		{shortGuirNote, shortGuirName},
		{longGuirNote, longGuirName},
		{claveNote, claveName},
		{hiWoodBlockNote, hiWoodBlockName},
		{lowWoodBlockNote, lowWoodBlockName},
		{muteCuicaNote, muteCuicaName},
		{openCuicaNote, openCuicaName},
		{muteTriangleNote, muteTriangleName},
		{openTriangleNote, openTriangleName}
	};

	static const std::unordered_map<std::string, int> midiNameToNoteMap = {
		{acousticBassDrumName, acousticBassDrumNote},
		{bassDrum1Name, bassDrum1Note},
		{sideStickName, sideStickNote},
		{acousticSnareName, acousticSnareNote},
		{handClapName, handClapNote},
		{electricSnareName, electricSnareNote},
		{lowFloorTomName, lowFloorTomNote},
		{closedHiHatName, closedHiHatNote},
		{highFloorTomName, highFloorTomNote},
		{pedalHiHatName, pedalHiHatNote},
		{lowTomName, lowTomNote},
		{openHiHatName, openHiHatNote},
		{lowMidTomName, lowMidTomNote},
		{hiMidTomName, hiMidTomNote},
		{crashCymbal1Name, crashCymbal1Note},
		{highTomName, highTomNote},
		{rideCymbal1Name, rideCymbal1Note},
		{chineseCymbalName, chineseCymbalNote},
		{rideBellName, rideBellNote},
		{tambourineName, tambourineNote},
		{splashCymbalName, splashCymbalNote},
		{cowbellName, cowbellNote},
		{crashCymbal2Name, crashCymbal2Note},
		{vibraslapName, vibraslapNote},
		{rideCymbal2Name, rideCymbal2Note},
		{hiBongoName, hiBongoNote},
		{lowBongoName, lowBongoNote},
		{muteHiCongaName, muteHiCongaNote},
		{openHiCongaName, openHiCongaNote},
		{lowCongaName, lowCongaNote},
		{highTimbaleName, highTimbaleNote},
		{lowTimbaleName, lowTimbaleNote},
		{highAgogName, highAgogNote},
		{lowAgogName, lowAgogNote},
		{cabasName, cabasNote},
		{maracaName, maracaNote},
		{shortWhistleName, shortWhistleNote},
		{longWhistleName, longWhistleNote},
		{shortGuirName, shortGuirNote},
		{longGuirName, longGuirNote},
		{claveName, claveNote},
		{hiWoodBlockName, hiWoodBlockNote},
		{lowWoodBlockName, lowWoodBlockNote},
		{muteCuicaName, muteCuicaNote},
		{openCuicaName, openCuicaNote},
		{muteTriangleName, muteTriangleNote},
		{openTriangleName, openTriangleNote}
	};

	static const std::unordered_map<int, std::vector<int>> midiNoteToStopNotesMap = {
		{acousticBassDrumNote, {}},
		{bassDrum1Note, {}},
		{sideStickNote, {}},
		{acousticSnareNote, {}},
		{handClapNote, {}},
		{electricSnareNote, {}},
		{lowFloorTomNote, {}},
		{closedHiHatNote, {openHiHatNote}},
		{highFloorTomNote, {}},
		{pedalHiHatNote, {openHiHatNote}},
		{lowTomNote, {}},
		{openHiHatNote, {}},
		{lowMidTomNote, {}},
		{hiMidTomNote, {}},
		{crashCymbal1Note, {}},
		{highTomNote, {}},
		{rideCymbal1Note, {}},
		{chineseCymbalNote, {}},
		{rideBellNote, {}},
		{tambourineNote, {}},
		{splashCymbalNote, {}},
		{cowbellNote, {}},
		{crashCymbal2Note, {}},
		{vibraslapNote, {}},
		{rideCymbal2Note, {}},
		{hiBongoNote, {}},
		{lowBongoNote, {}},
		{muteHiCongaNote, {}},
		{openHiCongaNote, {}},
		{lowCongaNote, {}},
		{highTimbaleNote, {}},
		{lowTimbaleNote, {}},
		{highAgogNote, {}},
		{lowAgogNote, {}},
		{cabasNote, {}},
		{maracaNote, {}},
		{shortWhistleNote, {}},
		{longWhistleNote, {}},
		{shortGuirNote, {}},
		{longGuirNote, {}},
		{claveNote, {}},
		{hiWoodBlockNote, {}},
		{lowWoodBlockNote, {}},
		{muteCuicaNote, {}},
		{openCuicaNote, {}},
		{muteTriangleNote, {openTriangleNote}},
		{openTriangleNote, {muteTriangleNote}}
	};
}
