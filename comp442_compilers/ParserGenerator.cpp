#include "stdafx.h"


Parser* ParserGenerator::buildParser(Lexer* lexer, Grammar* grammar) {
	Parser* parser = new Parser;
	parser->lexer = lexer;
	parser->grammar = grammar;
	parser->firstSet = buildFirstSet(*parser->grammar);
	parser->followSet = buildFollowSet(*parser->grammar, parser->firstSet);
	parser->parseTable = buildParseTable(*grammar, parser->firstSet, parser->followSet);
	return parser;
}

ParserGenerator::~ParserGenerator() {
}

std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> ParserGenerator::buildFirstSet(const Grammar& grammar) {
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet = initMap(grammar);
	std::vector<std::shared_ptr<Production>> productions = grammar.getProductions();

	// Whether we should continue proccessing this grammar in order to add all the neccessary terminals
	bool continueProcessing = true;
	while (continueProcessing) {

		// Assume we shouldnt continue
		continueProcessing = false;
		for (auto p = productions.begin(); p != productions.end(); ++p) {
			NonTerminal pNonTerminal = p->get()->getNonTerminal();
			TerminalSet first = computeFirst(p->get()->getProduction(), firstSet);
			// Merge the current set for the non terminal with the computed first set
			std::pair<TerminalSet, bool> merge = leftMerge(firstSet.at(pNonTerminal), first);
			// Check if there were changes
			if (merge.second) {
				// Set the merged set to the set for that non terminal
				firstSet.at(pNonTerminal) = merge.first;
				// If there were changes while merging, then we need to continue adding until there were no more changes
				continueProcessing = true;
			}
		}
	}
	return firstSet;
}


std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> ParserGenerator::buildFollowSet(const Grammar& grammar, const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet) {
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet = initMap(grammar);
	// the end of file is in the follow set for the start symbol
	followSet.at(grammar.getStartSymbol()).emplace(SpecialTerminal::END_OF_FILE);
	bool continueProcessing = true;
	std::vector<std::shared_ptr<Production>> productions = grammar.getProductions();
	while (continueProcessing) {
		// Assume we dont need to contine proccessing
		// Further down, some caller may change this value
		continueProcessing = false;

		for (auto p = productions.begin(); p != productions.end(); ++p) {
			std::vector<Symbol> rhs = (*p)->getProduction();

			for (int i = 0; i < rhs.size(); i++) {
				Symbol currentSymbol = rhs.at(i);
				// Skip if this is a terminal symbol
				if (currentSymbol.isTerminal()) {
					continue;
				}
				// It is a nonterminal at this point
				// This is a nonTerminal
				const NonTerminal nt = static_cast<const NonTerminal&>(currentSymbol);
				TerminalSet offsetSet = computeFirst(sublist(rhs, i + 1, rhs.size()), firstSet);
				for (Terminal t : offsetSet) {
					if (SpecialTerminal::isEpsilon(t.getName())) {
						continue;
					}

					std::pair<TerminalSet::iterator, bool> emplaceResult = followSet.at(nt).emplace(t);
					if (emplaceResult.second) {
						continueProcessing = true;
					}
				}

				if (offsetSet.find(SpecialTerminal::EPSILON) != offsetSet.end()) {
					std::pair<TerminalSet, bool> merge = leftMerge(followSet.at(nt), followSet.at((*p)->getNonTerminal()));
					followSet.at(nt) = merge.first;
					if (merge.second) {
						continueProcessing = true;
					}
				}
			}
		}
	}
	return followSet;
}


TerminalSet ParserGenerator::computeFirst(const std::vector<Symbol>& symbols, const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& first) {
	TerminalSet computedSet;
	for (auto s = symbols.begin(); s != symbols.end(); ++s) {
		if (s->isTerminal()) {
			const Terminal t = static_cast<const Terminal&>(*s);
			computedSet.emplace(t);
			return computedSet;
		}
		// This is a nonTerminal
		const NonTerminal nt = static_cast<const NonTerminal&>(*s);

		TerminalSet ntSet = first.at(nt);

		// Check if this set has epislion
		TerminalSet::iterator got = ntSet.find(SpecialTerminal::EPSILON);
		if (got == ntSet.end()) {
			// epsilon not found
			std::pair<TerminalSet, bool> merge = leftMerge(computedSet, first.at(nt));
			computedSet = merge.first;
			return computedSet;
		}
		std::pair<TerminalSet, bool> merge = leftMerge(computedSet, first.at(nt));
		computedSet = merge.first;
		computedSet.erase(SpecialTerminal::EPSILON);
	}
	computedSet.emplace(SpecialTerminal::EPSILON);
	return computedSet;
}

std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> ParserGenerator::initMap(const Grammar& grammar) {
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> map;
	const std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> nonTerminals = grammar.getNonTerminals();

	for (auto itr = nonTerminals.begin(); itr != nonTerminals.end(); ++itr) {
		TerminalSet emptySet;
		map.emplace(*itr->get(), emptySet);
	}
	return map;
}


std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> ParserGenerator::buildParseTable(const Grammar& g) {
	std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> parseTable;
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet = ParserGenerator::buildFirstSet(g);
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet = ParserGenerator::buildFollowSet(g, firstSet);
	return buildParseTable(g, firstSet, followSet);
}


std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> ParserGenerator::buildParseTable(const Grammar& g,
	const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet,
	const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& followSet) {

	std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> parseTable;

	// Taken from power point
	const std::vector<std::shared_ptr<Production>> productions = g.getProductions();
	typedef std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> TerminalSetPtr;
	const TerminalSetPtr terminalSet = g.getTerminals();
	std::unordered_map <Terminal, Production, SymbolHasher, SymbolEqual> emptyRow;
	for (std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>::const_iterator t = terminalSet.begin(); t != terminalSet.end(); ++t) {
		// Add this terminal to the row
		emptyRow.emplace(*(t->get()), Production::ERROR_PRODUCTION);
	}
	for (std::vector<std::shared_ptr<Production>>::const_iterator p = productions.begin(); p != productions.end(); ++p) {
		NonTerminal lhs = (*p)->getNonTerminal();
		std::vector<Symbol> rhs = (*p)->getProduction();
		parseTable.emplace(lhs, emptyRow);
		TerminalSet firstRhs = ParserGenerator::computeFirst(rhs, firstSet);
		for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
			Terminal t = *t_ptr->get();
			if (t.isTerminal() && !SpecialTerminal::isEpsilon(t.getName())) {
				Terminal terminalCast = static_cast<Terminal&>(t);
				if (inSet(terminalCast, firstRhs)) {
					// Revove the error production there
					parseTable.at(lhs).erase(terminalCast);
					// Add the updated production
					parseTable.at(lhs).emplace(terminalCast, *p->get());
				}
			}
		}
		if (inSet(SpecialTerminal::EPSILON, firstRhs)) {
			for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
				Terminal t = *t_ptr->get();
				if (t.isTerminal()) {
					Terminal terminalCast = static_cast<Terminal&>(t);
					if (ParserGenerator::inFollow(terminalCast, lhs, followSet)) {
						// Revove the error production there
						parseTable.at(lhs).erase(terminalCast);
						// Add the updated production
						parseTable.at(lhs).emplace(terminalCast, *p->get());
					}
				}
			}
		}
	}
	return parseTable;
}

bool ParserGenerator::inFirst(const Terminal& terminal, const NonTerminal& nonTerminal, const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet) {
	return inSet(terminal, firstSet.at(nonTerminal));
}

bool ParserGenerator::inFollow(const Terminal& terminal, const NonTerminal& nonTerminal, const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& followSet) {
	return inSet(terminal, followSet.at(nonTerminal));
}

