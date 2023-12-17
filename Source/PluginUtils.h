/*
  ==============================================================================

    PluginUtils.h
    Created: 14 Dec 2023 6:50:37pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <string>
#include <map>
#include <set>

class PluginUtils {
public:
    static std::string toSnakeCase(const std::string& str);
    static bool isNumeric(const std::string& str);
    static std::string capitalizeFirstLetter(const std::string& str);
    static std::string getParamId(int midiNote, std::string micId, std::string param);
    static std::map<int, std::set<std::string>> getUniqueMidiNoteMicCombinations();
};