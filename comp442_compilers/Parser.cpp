#include "stdafx.h"

Parser::Parser(const Lexer& lexer, const Grammar& grammar) {
	this->lexer = lexer;
	this->grammar = grammar;
	firstSet = GrammarFirstFollowSetGenerator::buildFirstSet(grammar);
	followSet = GrammarFirstFollowSetGenerator::buildFollowSet(grammar, firstSet);
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




void Parser::buildParseTable() {
	
	const std::vector<std::shared_ptr<Production>> productions = grammar.getProductions();
	typedef std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> TerminalSetPtr;
	const TerminalSetPtr terminalSet = grammar.getTerminals();
	std::unordered_map <Terminal, Production, SymbolHasher, SymbolEqual> emptyRow;
	for (std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>::const_iterator t = terminalSet.begin(); t != terminalSet.end(); ++t) {
		// Add this terminal to the row
		emptyRow.emplace(*(t->get()), Production::ERROR_PRODUCTION);
	}
	for (std::vector<std::shared_ptr<Production>>::const_iterator p = productions.begin(); p != productions.end(); ++p) {
		NonTerminal lhs = (*p)->getNonTerminal();
		std::vector<Symbol> rhs = (*p)->getProduction();
		parseTable.emplace(lhs, emptyRow);
		TerminalSet firstRhs = GrammarFirstFollowSetGenerator::computeFirst(rhs, firstSet);
		for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
			Terminal t = *t_ptr->get();
			if (t.isTerminal() && !SpecialTerminal::isEpsilon(t.getName())) {
				Terminal terminalCast = static_cast<Terminal&>(t);
				if (inSet(terminalCast, firstRhs)) {
					// Revove the error production there
					parseTable.at(lhs).erase(terminalCast);
					// Add the updated production
					parseTable.at(lhs).emplace( terminalCast, *p->get());
				}
			}
		}
		if (inSet(SpecialTerminal::EPSILON, firstRhs)) {
			for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
				Terminal t = *t_ptr->get();
				if (t.isTerminal()) {
					Terminal terminalCast = static_cast<Terminal&>(t);
					if (inFollow(terminalCast, lhs)) {
						// Revove the error production there
						parseTable.at(lhs).erase(terminalCast);
						// Add the updated production
						parseTable.at(lhs).emplace(terminalCast, *p->get());
					}
				}	
			}
		}

	}

}

void Parser::skipErrors() {
}

void Parser::inverseRHSMultiplePush(const Production& rhs) {
	// TODO inverser
	// This pushes the production in reverse order
}

bool Parser::inFirst(const Terminal& terminal, const NonTerminal& nonTerminal)  {
	return inSet(terminal, firstSet.at(nonTerminal));
}

bool Parser::inFollow(const Terminal& terminal, const NonTerminal& nonTerminal) {
	return inSet(terminal, followSet.at(nonTerminal));
}

bool Parser::inSet(const Terminal& symbol, const TerminalSet& symbolSet) {
	TerminalSet::const_iterator got = symbolSet.find(symbol);
	if (got == symbolSet.end()) {
		return false;
	}
	return true;
}