#ifndef NONTERMINAL_H
#define NONTERMINAL_H

// A concrete class to hold non terminal symbols
class NonTerminal : public Symbol {
public:
	NonTerminal(std::string name);
	~NonTerminal();
};




#endif 