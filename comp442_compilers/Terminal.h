#ifndef TERMINAL_H
#define TERMINAL_H

// A concrete class to hold terminal symbols
class Terminal : public Symbol {
public:
	Terminal(std::string name);
	~Terminal();
};

class SpecialTerminal : public Terminal {
public:
	// Special symbols to hold epsilon/end of file symbols
	// TODO: maybe move this out of a static context
	static const SpecialTerminal EPSILON;
	static const SpecialTerminal END_OF_FILE;
	SpecialTerminal(std::string name);
	~SpecialTerminal();

	// Whether the given string is an epsilon symbol
	static bool isEpsilon(const std::string& symbolString);

};



#endif