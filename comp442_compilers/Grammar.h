#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
public:

	Grammar();
	// Create our grammar from a file and specify the start symbol for this grammar
	Grammar(std::string filename, std::string startSymbol);
	~Grammar();
	// Creates and adds a nonterminal to the non terminal set
	const NonTerminal& addNonTerminal(const std::string&, bool isStartSymbol = false);
	// Creates and adds a terminal to the terminal set
	const Terminal& addTerminal(const std::string&);
	// Creates a producution and it to the list of productions for this grammar
	const Production& addProduction(const NonTerminal& symbol, std::vector<Symbol> production);
	// Get the starting symbol for this grammar
	const NonTerminal& Grammar::getStartSymbol() const;
	// Get a list of productions for the grammar
	const std::vector<std::shared_ptr<Production>>& getProductions() const;
	// Gets the set of terminals/non terminals
	const std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>& getTerminals() const;
	const std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual>& getNonTerminals() const;
	// Output this grammar
	friend std::ostream& operator<<(std::ostream& os, Grammar& g);

private:
	// The starting symbol for this grammar
	std::shared_ptr<NonTerminal> mStartSymbol;
	// The list of productions for this grammar
	std::vector<std::shared_ptr<Production>> mProductions;
	// The set of terminals for this grammar
	std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> mTerminalSymbols;
	// The set of nonterminals for this grammar
	std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> mNonTerminalSymbols;
	
	// Whether the given string is a non terminal symbol
	bool isNonTerminal(const std::string& nonTerminalString);
	// Turns the string into either the terminal/nonterminal/epsilon symbol
	Symbol stringToSymbol(const std::string& symbolString);

};

class GrammarFirstFollowSetGenerator {
	// TODO: move this back to the parser to avoid copying
	// For now this is here for testing
	// algorithm to build first and follow sets for the given grammar
public:
	// NonTerminalMapToTerminalSet is typedefed in stdafx.h
	static NonTerminalMapToTerminalSet buildFirstSet(const Grammar&);
	static NonTerminalMapToTerminalSet buildFollowSet(const Grammar&, const NonTerminalMapToTerminalSet& firstSet);
	// TerminalSet is typedef in stdafx.h
	static TerminalSet computeFirst(const std::vector<Symbol>& symbols, const NonTerminalMapToTerminalSet& first);

	// Creates an empty map that holds all the non terminals to empty sets
	static NonTerminalMapToTerminalSet initMap(const Grammar& g);
	// Returns a pair, where the first value is the union of the two sets
	// second value, returns true if there were items in set2 that were not int set1. Adds items in set2 to set1
	static std::pair<TerminalSet, bool> left_merge(const TerminalSet& set1, const TerminalSet& set2);
};

#endif // !GRAMMAR_H