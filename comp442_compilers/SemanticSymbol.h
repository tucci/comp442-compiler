#ifndef SEMANTIC_SYMBOL_H
#define SEMANTIC_SYMBOL_H

// TODO: How should the grammar handle semantic symbols?
// 1. Store as it were a terminal, and when we build the first/follow sets, we ignore these symbols. Let the parser deal with the symbols

// 2.Create seperate productions. productions with semantic symbols. and without. 
// I think this may be the best. We wouldnt have to deal with the first/follow code changes, and we'll just change it when we are building the parse table
// change in buildParseTable() when we call getProductions(), we can call getProductionsWithSemantics(). For this we wouldnt have to change the output table code
// maybe not. i think we need to figure out to map the normal production array to production array with semantics. we have production hashing, so things might not map 1 to 1
// However we still need to skip it when creating the productions
// Also the parse table needs the production with the semantic action in them, but when parsing you should handle these and treat them as they were nonterminal/terminal.
// i.e they should not reduce code as they part of the normal grammar

// 3.Have the grammar care about sematnic symbols and have it deal with it early on, and store the semantic symbols in it's own array
// IDEA: ddd



// A concrete class to hold Semantic Symbols
class SemanticSymbol : public Symbol {
public:
	
	SemanticSymbol(std::string name);
	~SemanticSymbol();
	void performAction(SymbolTable* currentTable, const Token& token);

	// Whether the given string is a semantic string
	// Checks the string if it matchs our semantic string pattern of #name#
	static bool isSemanticPattern(const std::string& symbolString);
	static std::vector<Symbol> filterOutSemanticSymbols(const std::vector<Symbol>& rhs);
};

#endif // !SEMANTIC_SYMBOL_H