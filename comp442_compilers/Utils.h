
#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// Taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
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

// Splits the string by a space
static inline std::vector<std::string> simpleSplit(const std::string& splitString) {
	std::vector<std::string> split;
	std::istringstream iss(splitString);
	// Split the rhs into a vector
	for (std::string s; iss >> s;) {
		split.push_back(s);
	}
	return split;
}

// Taken from http://stackoverflow.com/questions/421573/best-way-to-extract-a-subvector-from-a-vector
template <class T> static inline std::vector<T> sublist(const std::vector<T>& vec, int start, int end) {
	std::vector<T>::const_iterator first = vec.begin() + start;
	std::vector<T>::const_iterator last = vec.begin() + end;
	return std::vector<T> (first, last);
}

static inline std::string vectorToString(const std::vector <std::string>& vec) {
	std::string str;
	for (auto s : vec) {
		str += (s + " ");
	}
	return str;
}

#endif // !UTILS_H