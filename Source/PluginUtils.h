#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream> 

static inline std::string stringToSnakeCase(const std::string& str) {
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

static inline std::string stringToTitleCase(const std::string& str)
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

static inline std::string stringsJoin(const std::vector<std::string>& strings)
{
    std::ostringstream result;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (!strings[i].empty()) {
            if (!result.str().empty()) {
                result << "_";
            }
            result << strings[i];
        }
    }
    return result.str();
}

static inline std::string stringsJoinAndSnakeCase(const std::vector<std::string>& strings)
{
    return stringToSnakeCase(stringsJoin(strings));
};

static inline bool stringIsNumeric(const std::string& str) 
{
    return !str.empty() && 
        std::find_if(str.begin(), str.end(), [](unsigned char c) 
            { 
                return !std::isdigit(c); 
            }) == str.end();
};

