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
};
