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
	const NonTerminal& Grammar::getStartSymbol();
	// Get a list of productions for the grammar
	const std::vector<std::shared_ptr<Production>>& getProductions() ;
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
	// Whether the given string is an epsilon symbol
	bool isEpsilon(const std::string& symbolString);
	// Turns the string into either the terminal/nonterminal/epsilon symbol
	Symbol stringToSymbol(const std::string& symbolString);

};

#endif // !GRAMMAR_H