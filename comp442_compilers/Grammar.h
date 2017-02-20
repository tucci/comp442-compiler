#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
public:

	Grammar();
	Grammar(std::string filename, std::string startSymbol);
	~Grammar();
	const NonTerminal& addNonTerminal(const std::string&, bool isStartSymbol = false);
	const Terminal& addTerminal(const std::string&);
	const Production& addProduction(const NonTerminal& symbol, std::vector<Symbol> production);
	const NonTerminal& Grammar::getStartSymbol();
	const std::vector<std::shared_ptr<Production>>& getProductions() ;
	friend std::ostream& operator<<(std::ostream& os, Grammar& g);
private:
	std::shared_ptr<NonTerminal> mStartSymbol;
	std::vector<std::shared_ptr<Production>> mProductions;
	std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> mTerminalSymbols;
	std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> mNonTerminalSymbols;

	bool isNonTerminal(const std::string& nonTerminalString);
	Symbol stringToSymbol(const std::string& symbolString);
};

#endif // !GRAMMAR_H