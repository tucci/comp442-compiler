#include "stdafx.h"

Parser::Parser(const Lexer& lexer, const Grammar& grammar) {
	this->lexer = lexer;
	this->grammar = grammar;
	buildFirstSet();
	buildFollowSet();
	buildParseTable();
}


Parser::~Parser() {
}

bool Parser::parse() {
	//bool error;
	//parseStack.push(Symbol::END_OF_FILE);
	//parseStack.push(grammar->getStartSymbol());
	//lookAheadToken = lexer->nextToken();
	//while (true) {
	//	Symbol x = parseStack.top();
	//	if (x.isTerminal) {
	//		if (x == lookAheadToken) {
	//			parseStack.pop();
	//			lookAheadToken = lexer->nextToken();
	//		} else {
	//			skipErrors();
	//			error = true;
	//		}
	//	} else {
	//		// TODO
	//		if (parseTable.at(x).at(a) != "error") {
	//			parseStack.pop();
	//			inverseRHSMultiplePush();
	//		} else {
	//			skipErrors();
	//			error = true;
	//		}
	//	}
	//}
	//// TODO
	//if (lookAheadToken != EOF || error == true) {
	//	return false;
	//} else {
	//	return true;
	//}
	//
	//
	return false;
}

// TODO: we can probably just merge first/inset and follow/inset into 2 methods
// ex. inFirstSet(), inFollowSet();
std::unordered_set<Terminal, SymbolHasher, SymbolEqual> Parser::first(const NonTerminal& symbol) {
	return firstSet.at(symbol);
}

std::unordered_set<Terminal, SymbolHasher, SymbolEqual> Parser::follow(const NonTerminal& symbol) {
	return followSet.at(symbol);
}

bool Parser::inSet(const Terminal& symbol, const std::unordered_set<Terminal, SymbolHasher, SymbolEqual>& symbolSet) {
	std::unordered_set<Terminal, SymbolHasher, SymbolEqual>::const_iterator got = symbolSet.find(symbol);
	if (got == symbolSet.end()) {
		return false;
	}
	return true;
}


void Parser::buildFirstSet() {

}

void Parser::buildFollowSet() {

}

void Parser::buildParseTable() {
	std::vector<std::shared_ptr<Production>> productions = grammar.getProductions();
	for (std::vector<std::shared_ptr<Production>>::iterator it = productions.begin(); it != productions.end(); ++it) {
		
	}
}

void Parser::skipErrors() {
}

void Parser::inverseRHSMultiplePush(const Production& rhs) {
	// TODO inverser
	// This pushes the production in reverse order
}


