#ifndef SEMANTIC_SYMBOL_H
#define SEMANTIC_SYMBOL_H


// A concrete class to hold Semantic Symbols
class SemanticSymbol : public Symbol {
public:
	SemanticSymbol(std::string name);
	~SemanticSymbol();
};

#endif // !SEMANTIC_SYMBOL_H