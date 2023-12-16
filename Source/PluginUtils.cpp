/*
  ==============================================================================

    PluginUtils.cpp
    Created: 14 Dec 2023 6:50:37pm
    Author:  paulm

  ==============================================================================
*/

#include "PluginUtils.h"
#include <map>
#include <sstream> 
#include <BinaryData.h>
#include "Configuration/GeneralMidi.h"

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

std::map<int, std::set<std::string>> PluginUtils::getUniqueMidiNoteMicCombinations()
{
	std::map<int, std::set<std::string>> uniqueMidiNoteMicCombinations;

	for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; resourceIndex++)
	{
		std::string namedResource = BinaryData::namedResourceList[resourceIndex];
		std::string micId, midiName, generalMidiSnakeCaseName = "";
		int midiNote = -1;

		for (int midiNameIndex = 0; midiNameIndex < generalmidi::midiNamesVector.size(); midiNameIndex++)
		{
			midiName = generalmidi::midiNamesVector.at(midiNameIndex);
			generalMidiSnakeCaseName = PluginUtils::toSnakeCase(midiName);

			std::string cleanedNamedResource = namedResource.substr(0, namedResource.size() - 4);
			size_t pos = cleanedNamedResource.find(generalMidiSnakeCaseName);
			if (pos == 0)
			{
				std::string resourceMetadata = cleanedNamedResource.substr(pos + generalMidiSnakeCaseName.length());

				std::istringstream iss(resourceMetadata);
				std::vector<std::string> parts;
				std::string part;

				while (std::getline(iss, part, '_')) {
					if (!part.empty()) {
						parts.push_back(part);
					}
				}

				switch (parts.size()) {
				case 0:
					midiNote = generalmidi::midiNameToNoteMap.at(midiName);
					break;
				case 1:
					midiNote = generalmidi::midiNameToNoteMap.at(midiName);
					if (!PluginUtils::isNumeric(parts[0]))
					{
						micId = parts[0];
					}
					break;
				case 2:
					if (PluginUtils::isNumeric(parts[0]) && PluginUtils::isNumeric(parts[1]))
					{
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
					}
					else
					{
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
						micId = parts[1];
					}
					break;
				case 3:
					if (PluginUtils::isNumeric(parts[0]) && PluginUtils::isNumeric(parts[1]))
					{
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
						micId = parts[2];
					}
					else
					{
						midiNameIndex = generalmidi::midiNamesVector.size() + 1;
					}
					break;
				default:
					midiNameIndex = generalmidi::midiNamesVector.size() + 1;
					break;
				}
			}
		}

		if (midiNote != -1)
		{
			uniqueMidiNoteMicCombinations[midiNote].insert(micId);
		}
	}

	return uniqueMidiNoteMicCombinations;
}