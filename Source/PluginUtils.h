/*
  ==============================================================================

    PluginUtils.h
    Created: 14 Dec 2023 6:50:37pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <string>

class PluginUtils {
public:
    static std::string toSnakeCase(const std::string& str);
};