#include "stdafx.h"
#include "Grammar.h"

Grammar::Grammar(Production& startProduction) : mStartSymbol(startProduction.getNonTerminal()) {
	addProduction(startProduction);
}


Grammar::~Grammar() {
}

void Grammar::addProduction(Production& prod) {
	
	// Add this new production rule to our production list
	mProductions.push_back(prod);

	// Add the non terminal symbol for thr production in the set
	mNonTerminalSymbols.emplace(prod.getNonTerminal());
	// Add rhs symbols to the sets
	std::vector<Symbol> productions = prod.getProduction();
	// Loop over symbols in rhs
	for (std::vector<Symbol>::iterator it = productions.begin(); it != productions.end(); ++it) {
		// If the symbol is a terminal symbol we add it to the terminal symbol set
		if (it->isTerminal()) {
			addTerminal(static_cast<Terminal&>(*it));
		} else {
			// If the symbol is a non terminal symbol we add it to the non terminal symbol set
			addNonTerminal(static_cast<NonTerminal&>(*it));
		}
	}
}

void Grammar::addNonTerminal(NonTerminal& nonTerminal) {
	mNonTerminalSymbols.emplace(nonTerminal);
}

void Grammar::addTerminal(Terminal& terminal) {
	mTerminalSymbols.emplace(terminal);
}

NonTerminal Grammar::getStartSymbol() {
	return mStartSymbol;
}

std::ostream& operator <<(std::ostream& os, Grammar& g) {

	for (std::vector<Production>::iterator it = g.mProductions.begin(); it != g.mProductions.end(); ++it) {
		os << *it << std::endl;
	}
	return os;
}
