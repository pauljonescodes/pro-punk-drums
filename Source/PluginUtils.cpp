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