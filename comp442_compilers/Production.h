#ifndef PRODUCTION_H
#define PRODUCTION_H

class Production {
public:
	static const NonTerminal ERROR_PRODUCTION_NON_TERMINAL;
	static const Production ERROR_PRODUCTION;
	// Creates a production from a given non terminal symbol, and list of symbols for a production
	Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production);
	~Production();
	// Gets the lhs for this production
	NonTerminal getNonTerminal();
	// Gets the rhs for this production
	std::vector<Symbol> getProduction();
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, const Production& p);
private:
	// the lhs non terminal
	const NonTerminal mNonTerminal;
	// the rhs for this production
	const std::vector<Symbol> mProduction;
};

#endif