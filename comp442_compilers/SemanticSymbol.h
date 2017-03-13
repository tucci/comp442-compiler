#ifndef SEMANTIC_SYMBOL_H
#define SEMANTIC_SYMBOL_H




// A concrete class to hold Semantic Symbols
class SemanticSymbol : public Symbol {
public:
	
	SemanticSymbol(std::string name);
	~SemanticSymbol();
	// Whether the given string is a semantic string
	// Checks the string if it matchs our semantic string pattern of #name#
	static bool isSemanticPattern(const std::string& symbolString);
	static std::vector<Symbol> filterOutSemanticSymbols(const std::vector<Symbol>& rhs);
};

#endif // !SEMANTIC_SYMBOL_H