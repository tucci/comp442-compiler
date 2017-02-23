#include "stdafx.h"

// Special production to be used in our parsing table to indicate that this is an error
const NonTerminal Production::ERROR_PRODUCTION_NON_TERMINAL("ERROR_PRODUCTION");
const Production Production::ERROR_PRODUCTION(ERROR_PRODUCTION_NON_TERMINAL, std::vector<Symbol>{});

Production::Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production) :mNonTerminal(nonTerminal), mProduction(production) {
	// If you want an epsilon production, you must explicity add Symbol::EPSILON to the production vector
}

Production::~Production() {
}

NonTerminal Production::getNonTerminal() const {
	return mNonTerminal;
}

std::vector<Symbol> Production::getProduction() const {
	return mProduction;
}

std::ostream& operator <<(std::ostream& os, const Production& p) {	
	std::string productionStr;
	for (std::vector<Symbol>::const_iterator it = p.mProduction.begin(); it != p.mProduction.end(); ++it) {
		productionStr.append(" " + it->getName());
	}

	// i.e S->tABC
	os << p.mNonTerminal.getName() << " -> " << productionStr;
	return os;
}

bool operator==(const Production& p1, const Production& p2) {
	bool nameTrue = p1.getNonTerminal().getName() == p2.getNonTerminal().getName();
		// Compare names first
		if (!nameTrue) {
			return false;
		}
		std::vector<Symbol> prod1 = p1.getProduction();
		std::vector<Symbol> prod2 = p2.getProduction();
		// Compare rhs production rules by size first
		if (prod1.size() != prod2.size()) {
			return false;
		}
		// Compare each symbol in both productions
		for (int i = 0; i < prod1.size(); i++) {
			// if names are the same, we still need to compare nonterminal/terminal types
			if (prod1.at(i).getName() == prod2.at(i).getName()) {
				if (prod1.at(i).isTerminal() != prod2.at(i).isTerminal()) {
					// If there was a type mismatch then these are not the same productions
					return false;
				}
			} else {
				// If there was a name mismatch then these are not the same productions
				return false;
			}
		}
		// Everything seemed to match, so the productions are the same
		return true;
		
}

bool operator!=(const Production& p1, const Production& p2) {
	return !(p1 == p2);
}
