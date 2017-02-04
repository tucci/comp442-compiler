#include "stdafx.h"


Production::Production(NonTerminal& nonTerminal, std::vector<Symbol>& production)
	: mNonTerminal(nonTerminal)
	, mProduction(production)
{
}

Production::~Production() {
}

NonTerminal Production::getNonTerminal() {
	return mNonTerminal;
}

std::vector<Symbol> Production::getProduction() {
	return mProduction;
}

std::ostream& operator <<(std::ostream& os, Production& p) {
	
	std::string productionStr;
	for (std::vector<Symbol>::iterator it = p.mProduction.begin(); it != p.mProduction.end(); ++it) {
		productionStr.append(it->getName());
	}

	// i.e S->tABC
	os << p.mNonTerminal.getName() << "->" << productionStr << std::endl;
	return os;
}
