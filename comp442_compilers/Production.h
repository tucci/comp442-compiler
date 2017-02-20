#ifndef PRODUCTION_H
#define PRODUCTION_H

class Production {
public:
	Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production);
	~Production();
	NonTerminal getNonTerminal();
	std::vector<Symbol> getProduction();
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, const Production& p);
private:
	const NonTerminal mNonTerminal;
	const std::vector<Symbol> mProduction;
};

#endif