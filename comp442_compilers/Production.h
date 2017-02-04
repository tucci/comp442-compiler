#ifndef PRODUCTION_H
#define PRODUCTION_H

class Production {
public:
	Production(NonTerminal& nonTerminal, std::vector<Symbol>& production);
	~Production();
	NonTerminal getNonTerminal();
	std::vector<Symbol> getProduction();
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, Production& p);
private:
	NonTerminal& mNonTerminal;
	std::vector<Symbol>& mProduction;
};

#endif