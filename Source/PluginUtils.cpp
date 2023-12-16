/*
  ==============================================================================

    PluginUtils.cpp
    Created: 14 Dec 2023 6:50:37pm
    Author:  paulm

  ==============================================================================
*/

#include "PluginUtils.h"

std::string PluginUtils::toSnakeCase(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (ch == ' ' || ch == '-') {
            result += '_';
        } else {
            result += std::tolower(ch);
        }
    }
    return result;
}

bool PluginUtils::isNumeric(const std::string& str) {
    return !str.empty() && std::find_if(str.begin(),
        str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
}

std::string PluginUtils::capitalizeFirstLetter(const std::string& str) 
{
    if (str.empty()) {
        return str;
    }

    std::string capitalized = str;
    if (std::islower(capitalized[0])) {
        capitalized[0] = std::toupper(capitalized[0]);
    }
    return capitalized;
}

std::string PluginUtils::getParamId(int midiNote, std::string micId, std::string param)
{
    if (micId.size() > 0) {
        return std::to_string(midiNote) + "_" + micId + "_" + param;
    }
    else {
        return std::to_string(midiNote) + "_" + param;
    }
}