#include "stdafx.h"

Grammar::Grammar() {
}

Grammar::~Grammar() {
}

const NonTerminal& Grammar::addNonTerminal(std::string nonTerminalString, bool isStartSymbol) {
	std::shared_ptr<NonTerminal> nonTerminal = std::shared_ptr<NonTerminal>(new NonTerminal(nonTerminalString));
	mNonTerminalSymbols.emplace(nonTerminal);
	nonTerminal = *mNonTerminalSymbols.find(nonTerminal);
	if (isStartSymbol) {
		mStartSymbol = nonTerminal;
	}
	return *nonTerminal;
}

const Terminal& Grammar::addTerminal(std::string terminalString) {
	std::shared_ptr<Terminal> terminal = std::shared_ptr<Terminal>(new Terminal(terminalString));
	mTerminalSymbols.emplace(terminal);
	terminal = *mTerminalSymbols.find(terminal);
	return *terminal;
}

const Production& Grammar::addProduction(const NonTerminal& symbol, std::vector<Symbol> production) {
	std::shared_ptr<Production> productionRule = std::shared_ptr<Production>(new Production(symbol, production));
	mProductions.push_back(productionRule);
	productionRule = mProductions.at(mProductions.size() - 1);
	return *productionRule;	
}

const NonTerminal& Grammar::getStartSymbol() {
	return *mStartSymbol;
}

 const std::vector<std::shared_ptr<Production>>& Grammar::getProductions() {
	return mProductions;
}

std::ostream& operator <<(std::ostream& os, Grammar& g) {
	for (std::vector<std::shared_ptr<Production>>::iterator it = g.mProductions.begin(); it != g.mProductions.end(); ++it) {
		os << *it->get() << std::endl;
	}
	return os;
}
