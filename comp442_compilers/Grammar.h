#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
public:
	Grammar();
	~Grammar();
	const NonTerminal& addNonTerminal(std::string, bool isStartSymbol = false);
	const Terminal& addTerminal(std::string);
	const Production& addProduction(const NonTerminal& symbol, std::vector<Symbol> production);
	const NonTerminal& Grammar::getStartSymbol();
	const std::vector<std::shared_ptr<Production>>& getProductions() ;
	friend std::ostream& operator<<(std::ostream& os, Grammar& g);
private:
	std::shared_ptr<NonTerminal> mStartSymbol;
	std::vector<std::shared_ptr<Production>> mProductions;
	std::unordered_set<std::shared_ptr<Terminal>> mTerminalSymbols;
	std::unordered_set<std::shared_ptr<NonTerminal>> mNonTerminalSymbols;
};

#endif // !GRAMMAR_H