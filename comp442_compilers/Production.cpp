#include "stdafx.h"

// Special production to be used in our parsing table to indicate that this is an error
const NonTerminal Production::ERROR_PRODUCTION_NON_TERMINAL("ERROR_PRODUCTION");
const Production Production::ERROR_PRODUCTION(ERROR_PRODUCTION_NON_TERMINAL, std::vector<Symbol>{});

Production::Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production) :mNonTerminal(nonTerminal), mProduction(production) {
	// If you want an epsilon production, you must explicity add Symbol::EPSILON to the production vector
}

Production::~Production() {
}

NonTerminal Production::getNonTerminal() {
	return mNonTerminal;
}

std::vector<Symbol> Production::getProduction() {
	return mProduction;
}

std::ostream& operator <<(std::ostream& os, const Production& p) {	
	std::string productionStr;
	for (std::vector<Symbol>::const_iterator it = p.mProduction.begin(); it != p.mProduction.end(); ++it) {
		productionStr.append(" " + it->getName());
	}

	// i.e S->tABC
	os << p.mNonTerminal.getName() << "->" << productionStr;
	return os;
}
