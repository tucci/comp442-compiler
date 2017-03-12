#include "stdafx.h"
#include "SemanticSymbol.h"


SemanticSymbol::SemanticSymbol(std::string name) : Symbol(name, false, true) {
}


SemanticSymbol::~SemanticSymbol() {
}

void SemanticSymbol::performAction(SymbolTable* currentTable, const Token& token) {
	std::cout << "Performing action: " + mName <<  " on " << token.lexeme << std::endl;
}


bool SemanticSymbol::isSemanticPattern(const std::string& symbolName) {
	// Does string match #name# pattern
	return symbolName.at(0) == '#' && symbolName.at(symbolName.length() - 1) == '#';
}

std::vector<Symbol> SemanticSymbol::filterOutSemanticSymbols(const std::vector<Symbol>& rhs) {
	std::vector<Symbol> filtered;
	for (const Symbol& s : rhs) {
		if (!isSemanticPattern(s.getName())) {
			filtered.push_back(s);
		}
	}

	return filtered;

}
