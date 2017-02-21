#include "stdafx.h"

Grammar::Grammar() {
}

Grammar::Grammar(std::string filename, std::string startSymbol) {
	// Read grammar from file
	std::ifstream inputFile;
	try {
		// Read the grammar from a file
		inputFile.open(filename);
		// Create the stream for our first pass
		std::stringstream firstPassInputStream;
		firstPassInputStream << inputFile.rdbuf();
		// Create the stream for our final pass
		std::stringstream finalPassInputStream(firstPassInputStream.str());
		inputFile.close();

		std::string line;

		// Store rhs for second pass to store terminals and productions
		std::vector<std::string> rhsList;

		// First pass gets all the non terminals
		while (std::getline(firstPassInputStream, line, '\n')) {
			// Split the rule on ->
			size_t splitIndex = line.find("->");
			// Gets the lhs non terminal
			std::string nonTerminal = trim(line.substr(0, splitIndex));
			if (!nonTerminal.empty()) {
				// Checks to see if the current non terminal is the start symbol
				bool isStartSymbol = nonTerminal == startSymbol;
				// Add the non terminal to the grammar
				addNonTerminal(nonTerminal, isStartSymbol);
				// push the rhs for this production to be used in the second pass
				rhsList.push_back(trim(line.substr(splitIndex + 2)));
			}
		}

		// Second pass: loop over all rhs for each prodution to find non terminals
		for (std::vector<std::string>::iterator it = rhsList.begin(); it != rhsList.end(); ++it) {
			std::vector<std::string> split = simpleSplit(*it);
			// loop over rhs split
			for (std::vector<std::string>::iterator st = split.begin(); st != split.end(); ++st) {
				// Make sure we dont add non terminals and epislon to the terminal string
				if (!isNonTerminal(*st) && !SpecialTerminal::isEpsilon(*st)) {
					addTerminal(*st);
				}
			}
		}

		// Final pass to create all the productions
		while (std::getline(finalPassInputStream, line, '\n')) {
			if (!line.empty()){
			// Split the rule on ->
			size_t splitIndex = line.find("->");
			// Gets the lhs non terminal
			std::string nonTerminal = trim(line.substr(0, splitIndex));
			// Get the rhs as a vector of strings
			std::vector<std::string> rhsProductionStrVec = simpleSplit(trim(line.substr(splitIndex + 2)));

				// Convert the string symbols to Symbol objects
				std::vector<Symbol> production;
				for (std::vector<std::string>::iterator st = rhsProductionStrVec.begin(); st != rhsProductionStrVec.end(); ++st) {
					production.push_back(stringToSymbol(*st));
				}
				// Create the production and add it to our grammar
				addProduction(nonTerminal, production);
			}
		}
		
	} catch (std::ifstream::failure e) {
		std::cout << "Error reading sample file";
	}
}

Grammar::~Grammar() {
}

const NonTerminal& Grammar::addNonTerminal(const std::string& nonTerminalString, bool isStartSymbol) {
	// Creates a new non terminal and adds it to our non terminal set
	std::shared_ptr<NonTerminal> nonTerminal = std::shared_ptr<NonTerminal>(new NonTerminal(nonTerminalString));
	mNonTerminalSymbols.emplace(nonTerminal);
	nonTerminal = *mNonTerminalSymbols.find(nonTerminal);
	// If this is a starting symbol, we'll assign the starting symbol to it
	if (isStartSymbol) {
		mStartSymbol = nonTerminal;
	}
	return *nonTerminal;
}

const Terminal& Grammar::addTerminal(const std::string& terminalString) {
	// Creates a new terminal and adds it to our terminal set
	std::shared_ptr<Terminal> terminal = std::shared_ptr<Terminal>(new Terminal(terminalString));
	mTerminalSymbols.emplace(terminal);
	terminal = *mTerminalSymbols.find(terminal);
	return *terminal;
}

const Production& Grammar::addProduction(const NonTerminal& symbol, std::vector<Symbol> production) {
	// Creates a new production and adds it to our production list
	std::shared_ptr<Production> productionRule = std::shared_ptr<Production>(new Production(symbol, production));
	mProductions.push_back(productionRule);
	productionRule = mProductions.at(mProductions.size() - 1);
	return *productionRule;	
}

const NonTerminal& Grammar::getStartSymbol() {
	return *mStartSymbol;
}

 const std::vector<std::shared_ptr<Production>>& Grammar::getProductions() const{
	return mProductions;
}

 const std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>& Grammar::getTerminals() const {
	 return mTerminalSymbols;
 }

 const std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual>& Grammar::getNonTerminals() const {
	 return mNonTerminalSymbols;
 }

std::ostream& operator <<(std::ostream& os, Grammar& g) {
	// Outputs all productions for this grammar
	for (std::vector<std::shared_ptr<Production>>::iterator it = g.mProductions.begin(); it != g.mProductions.end(); ++it) {
		os << *it->get() << std::endl;
	}
	return os;
}


bool Grammar::isNonTerminal(const std::string& nonTerminalString) {
	std::shared_ptr<NonTerminal> nonTerminal = std::shared_ptr<NonTerminal>(new NonTerminal(nonTerminalString));
	auto got = mNonTerminalSymbols.find(nonTerminal);
	if (got == mNonTerminalSymbols.end()) {
		// not found
		return false;
	} else {
		return true;
	}	
}

Symbol Grammar::stringToSymbol(const std::string& symbolString) {
	if (SpecialTerminal::isEpsilon(symbolString)) {
		return SpecialTerminal::EPSILON;
	}
	// Check if this is a non terminal symbol
	std::shared_ptr<NonTerminal> nonTerminal = std::shared_ptr<NonTerminal>(new NonTerminal(symbolString));
	auto got = mNonTerminalSymbols.find(nonTerminal);
	if (got == mNonTerminalSymbols.end()) {
		// This is not a non terminal symbol and is a terminal symbol
		std::shared_ptr<Terminal> terminal = std::shared_ptr<Terminal>(new Terminal(symbolString));
		auto got = mTerminalSymbols.find(terminal);
		if (got == mTerminalSymbols.end()) {
			throw std::exception("symbolString is not a valid symbol");
		} else {
			return *got->get();
		}
		
	} else {
		return *got->get();
	}
}



NonTerminalMapToTerminalSet GrammarFirstFollowSetGenerator::buildFirstSet(const Grammar& grammar) {
	NonTerminalMapToTerminalSet firstSet = initMap(grammar);
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
			std::pair<TerminalSet, bool> merge = left_merge(firstSet.at(pNonTerminal), first);
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


NonTerminalMapToTerminalSet GrammarFirstFollowSetGenerator::buildFollowSet(const Grammar& grammar) {
	NonTerminalMapToTerminalSet followSet = initMap(grammar);
	return followSet;
}


TerminalSet GrammarFirstFollowSetGenerator::computeFirst(const std::vector<Symbol>& symbols, const NonTerminalMapToTerminalSet& first) {
	TerminalSet computedSet;
	for (auto s = symbols.begin(); s!= symbols.end(); ++s) {
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
			std::pair<TerminalSet, bool> merge = left_merge(computedSet, first.at(nt));
			computedSet = merge.first;
			return computedSet;
		}
		std::pair<TerminalSet, bool> merge = left_merge(computedSet, first.at(nt));
		computedSet = merge.first;
		computedSet.erase(SpecialTerminal::EPSILON);
	}
	computedSet.emplace(SpecialTerminal::EPSILON);
	return computedSet;
}

NonTerminalMapToTerminalSet GrammarFirstFollowSetGenerator::initMap(const Grammar& grammar) {
	NonTerminalMapToTerminalSet map;
	const std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> nonTerminals = grammar.getNonTerminals();

	for (auto itr = nonTerminals.begin(); itr != nonTerminals.end(); ++itr) {
		TerminalSet emptySet;
		map.emplace(*itr->get(), emptySet);
	}

	return map;

}


std::pair<TerminalSet, bool> GrammarFirstFollowSetGenerator::left_merge(const TerminalSet& set1, const TerminalSet& set2) {
	TerminalSet unionedSet(set1);
	bool hasChanges = false;
	for (TerminalSet::const_iterator t = set2.begin(); t != set2.end(); ++t) {
		TerminalSet::const_iterator got = unionedSet.find(*t);
		if (got == unionedSet.end()) {
			// no duplicate found, so there were changes to the set
			hasChanges = true;
			unionedSet.emplace(*t);
		}
	}

	return std::pair<TerminalSet, bool>(unionedSet, hasChanges);
}
