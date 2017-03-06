
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
// Gets from start to end, but not including end
template <class T> static inline std::vector<T> sublist(const std::vector<T>& vec, int start, int end) {
	std::vector<T>::const_iterator first = vec.begin() + start;
	std::vector<T>::const_iterator last = vec.begin() + end;
	return std::vector<T> (first, last);
}

// Returns true if the terminal is in the given terminal set
static inline bool inSet(const Terminal& symbol, const TerminalSet& symbolSet) {
	TerminalSet::const_iterator got = symbolSet.find(symbol);
	if (got == symbolSet.end()) {
		return false;
	}
	return true;
}

template <class T, class H, class E> static inline bool inSet(const T& toFind, const std::unordered_set<T, H, E>& set) {
	std::unordered_set<T, H, E>::const_iterator got = set.find(toFind);
	if (got == set.end()) {
		return false;
	}
	return true;
}

template <class K, class V, class H, class E> static inline bool inMap(const K& toFind, const std::unordered_map<K, V, H, E>& map) {
	std::unordered_map<K, V, H, E>::const_iterator got = map.find(toFind);
	if (got == map.end()) {
		return false;
	}
	return true;
}

// second value, returns true if there were items in set2 that were not int set1. Adds items in set2 to set1
static std::pair<TerminalSet, bool> leftMerge(const TerminalSet& set1, const TerminalSet& set2) {
	TerminalSet unionedSet(set1);
	bool hasChanges = false;
	for (TerminalSet::const_iterator t = set2.begin(); t != set2.end(); ++t) {
		TerminalSet::const_iterator got = unionedSet.find(*t);
		if (got == unionedSet.end()) {
			// no duplicate found, so there were changes to the set
			hasChanges = true;
			unionedSet.emplace(*t);
		}
	}
	return std::pair<TerminalSet, bool>(unionedSet, hasChanges);
}

#endif // !UTILS_H