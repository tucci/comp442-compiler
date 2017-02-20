#include "stdafx.h"


Production::Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production) :mNonTerminal(nonTerminal), mProduction(production) {
	// If you want an epsilon production, you must explicity add Symbol::EPSILON to the production vector
	if (production.empty()) {
		throw std::exception("Production list cannot be empty. EPSILON must be explictly added.");
	}
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
