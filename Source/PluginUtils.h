#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>

class PluginUtils {
public:
	static std::string toSnakeCase(const std::string& str);
	static std::string toTitleCase(const std::string& str);
	static std::string joinId(const std::vector<std::string>& strings);
	static bool isNumeric(const std::string& str);
	static std::map<int, std::set<std::string>> getUniqueMidiNoteMicCombinations();

	//static std::string capitalizeFirstLetter(const std::string& str);
	//static std::string getMidiNoteParameterId(int midiNote, std::string micId, std::string param);
	//static std::string getChannelParameterId(std::string channelName, std::string paramId);
	//static std::string getEqualizationParameterId(std::string channelName, std::string equalitizationTypeId, std::string equalizationSubtypeId);
};
