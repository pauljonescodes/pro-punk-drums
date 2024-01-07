#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream> 
#include "JuceHeader.h"

class PluginUtils {
public:
	static inline std::string toSnakeCase(const std::string& str) {
        std::string result;
        for (char ch : str) {
            if (ch == ' ' || ch == '-') {
                result += '_';
            }
            else {
                result += std::tolower(ch);
            }
        }
        return result;
    };

	static inline std::string toTitleCase(const std::string& str)
    {
        std::string result;
        bool nextUpper = true;

        for (char ch : str) {
            if (ch == '_') {
                nextUpper = true;
                result += " ";
            }
            else if (nextUpper) {
                result += std::toupper(ch);
                nextUpper = false; // Reset the flag
            }
            else {
                if (result.empty()) { // first letter in lowercase for camelCase
                    result += std::tolower(ch);
                }
                else {
                    result += ch; // preserve the original case for the rest
                }
            }
        }

        return result;
    };

	static inline std::string joinAndSnakeCase(const std::vector<std::string>& strings) {
        std::ostringstream result;
        for (size_t i = 0; i < strings.size(); ++i) {
            if (!strings[i].empty()) {
                if (!result.str().empty()) {
                    result << "_";
                }
                result << strings[i];
            }
        }
        return PluginUtils::toSnakeCase(result.str());
    };
	
    static inline bool isNumeric(const std::string& str) {
        return !str.empty() && std::find_if(str.begin(),
            str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
    };

    static inline auto stringFromHzValue = [](double value) {
        if (value < 2.0f)
            return juce::String(value, 2) + "Hz";
        if (value < 10.0f)
            return juce::String(value, 1) + "Hz";
        else
            return juce::String(value, 1).dropLastCharacters(2) + "Hz";
    };

    static inline auto hzValueFromString = [](const juce::String& text) {
        if (text.endsWith("Hz") || (text.endsWith("hz")))
        {
            return text.dropLastCharacters(2).getFloatValue();
        }
        else
            return text.getFloatValue();
    };
};

