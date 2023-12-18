#pragma once
#include <map>
#include "GeneralMidi.h"
#include <string>

namespace channels {
const int outputIndex = 0;
const int kickIndex = 1;
const int snareIndex = 2;
const int tomsIndex = 3;
const int hiHatIndex = 4;
const int cymbalsIndex = 5;
const int percussionIndex = 6;

const int size = 7;

const std::string outputName = "Output";
const std::string kickName = "1 Kick";
const std::string snareName = "2 Snare";
const std::string tomsName = "3 Toms";
const std::string hiHatName = "4 Hi-hat";
const std::string cymbalsName = "5 Cymbals";
const std::string percussionName = "6 Percussion";

std::map<int, std::string> channelIndexToNameMap = {
    { channels::outputIndex, channels::outputName },
    { channels::kickIndex, channels::kickName },
    { channels::snareIndex, channels::snareName },
    { channels::tomsIndex, channels::tomsName },
    { channels::hiHatIndex, channels::hiHatName },
    { channels::cymbalsIndex, channels::cymbalsName },
    { channels::percussionIndex, channels::percussionName },
};

std::map<int, int> generalMidiNoteToChannelIndex = {
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
