#include "stdafx.h"

Grammar::Grammar() {
	mStartSymbol = std::shared_ptr<NonTerminal>(new NonTerminal("S"));
	// Add end of file to the list of terminals 
	// We are gonna need this when creating the parse table
	mTerminalSymbols.emplace(std::shared_ptr<Terminal>(new Terminal(SpecialTerminal::END_OF_FILE.getName())));

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
				// Make sure we dont add non terminals and epislon to the terminal string or a semantic symbol
				// skip semantic symbols here
				if (!isNonTerminal(*st) && !SpecialTerminal::isEpsilon(*st) && !SemanticSymbol::isSemanticPattern(*st)) {
					addTerminal(*st);
				}
				
			}
		}

		// Add end of file to the list of terminals 
		// We are gonna need this when creating the parse table
		mTerminalSymbols.emplace(std::shared_ptr<Terminal>(new Terminal(SpecialTerminal::END_OF_FILE.getName())));

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

const NonTerminal& Grammar::getStartSymbol() const {
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
	// If this this a semantic action, we'll return a semantic symbol
	if (SemanticSymbol::isSemanticPattern(symbolString)) {
		SemanticSymbol semanticSymbol(symbolString);
		return semanticSymbol;
	}
	// Check if this is a non terminal symbol
	std::shared_ptr<NonTerminal> nonTerminal = std::shared_ptr<NonTerminal>(new NonTerminal(symbolString));
	auto got = mNonTerminalSymbols.find(nonTerminal);
	if (got == mNonTerminalSymbols.end()) {
		// This is not a non terminal symbol and is a terminal symbol
		std::shared_ptr<Terminal> terminal = std::shared_ptr<Terminal>(new Terminal(symbolString));
		std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>::iterator found = mTerminalSymbols.find(terminal);
		if (found == mTerminalSymbols.end()) {
			throw std::exception("symbolString is not a valid symbol");
		} else {
			return *found->get();
		}
		
	} else {
		return *got->get();
	}
}
