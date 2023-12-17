/*
  ==============================================================================

    Channels.h
    Created: 14 Dec 2023 2:28:02pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <map>
#include "GeneralMidi.h"

namespace channels {
    const int stereo = 0;
    const int kick = 1;
    const int snare = 2;
    const int toms = 3;
    const int hiHat = 4;
    const int cymbals = 5;
    const int percussion = 6;
    const int overheadLeft = 7;
    const int overheadRight = 8;

    std::map<int, int> generalMidiNoteToChannel = {
    {generalmidi::midinotes::acousticBassDrum, kick},
    {generalmidi::midinotes::bassDrum1, kick},
    {generalmidi::midinotes::sideStick, snare},
    {generalmidi::midinotes::acousticSnare, snare},
    {generalmidi::midinotes::handClap, percussion},
    {generalmidi::midinotes::electricSnare, snare},
    {generalmidi::midinotes::lowFloorTom, toms},
    {generalmidi::midinotes::closedHiHat, hiHat},
    {generalmidi::midinotes::highFloorTom, toms},
    {generalmidi::midinotes::pedalHiHat, hiHat},
    {generalmidi::midinotes::lowTom, toms},
    {generalmidi::midinotes::openHiHat, hiHat},
    {generalmidi::midinotes::lowMidTom, toms},
    {generalmidi::midinotes::hiMidTom, toms},
    {generalmidi::midinotes::crashCymbal1, cymbals},
    {generalmidi::midinotes::highTom, toms},
    {generalmidi::midinotes::rideCymbal1, cymbals},
    {generalmidi::midinotes::chineseCymbal, cymbals},
    {generalmidi::midinotes::rideBell, cymbals},
    {generalmidi::midinotes::tambourine, percussion},
    {generalmidi::midinotes::splashCymbal, cymbals},
    {generalmidi::midinotes::cowbell, percussion},
    {generalmidi::midinotes::crashCymbal2, cymbals},
    {generalmidi::midinotes::vibraslap, percussion},
    {generalmidi::midinotes::rideCymbal2, cymbals},
    {generalmidi::midinotes::hiBongo, percussion},
    {generalmidi::midinotes::lowBongo, percussion},
    {generalmidi::midinotes::muteHiConga, percussion},
    {generalmidi::midinotes::openHiConga, percussion},
    {generalmidi::midinotes::lowConga, percussion},
    {generalmidi::midinotes::highTimbale, percussion},
    {generalmidi::midinotes::lowTimbale, percussion},
    {generalmidi::midinotes::highAgog, percussion},
    {generalmidi::midinotes::lowAgog, percussion},
    {generalmidi::midinotes::cabas, percussion},
    {generalmidi::midinotes::maraca, percussion},
    {generalmidi::midinotes::shortWhistle, percussion},
    {generalmidi::midinotes::longWhistle, percussion},
    {generalmidi::midinotes::shortGuir, percussion},
    {generalmidi::midinotes::longGuir, percussion},
    {generalmidi::midinotes::clave, percussion},
    {generalmidi::midinotes::hiWoodBlock, percussion},
    {generalmidi::midinotes::lowWoodBlock, percussion},
    {generalmidi::midinotes::muteCuica, percussion},
    {generalmidi::midinotes::openCuica, percussion},
    {generalmidi::midinotes::muteTriangle, percussion},
    {generalmidi::midinotes::openTriangle, percussion}
    };
}