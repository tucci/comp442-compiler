// Taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "stdafx.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

static inline std::vector<std::string> simpleSplit(const std::string& splitString) {
	std::vector<std::string> split;
	std::istringstream iss(splitString);
	// Split the rhs into a vector
	for (std::string s; iss >> s;) {
		split.push_back(s);
	}
	return split;
}

#endif // !STRING_UTILS_H