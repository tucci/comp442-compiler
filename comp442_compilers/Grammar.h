#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
public:
	Grammar(Production& startProduction);
	~Grammar();
	void addProduction(Production& prod);
	void addNonTerminal(NonTerminal& nonTerminal);
	void addTerminal(Terminal& terminal);
	NonTerminal Grammar::getStartSymbol();
	friend std::ostream& operator<<(std::ostream& os, Grammar& g);
private:
	NonTerminal& mStartSymbol;
	std::vector<Production> mProductions;
	std::unordered_set<Terminal> mTerminalSymbols;
	std::unordered_set<NonTerminal> mNonTerminalSymbols;
};

#endif // !GRAMMAR_H