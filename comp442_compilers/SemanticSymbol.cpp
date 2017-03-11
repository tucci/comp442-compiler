#include "stdafx.h"
#include "SemanticSymbol.h"


SemanticSymbol::SemanticSymbol(std::string name) : Symbol(name, false, true) {
}


SemanticSymbol::~SemanticSymbol() {
}


bool SemanticSymbol::isSemanticPattern(const std::string& symbolName) {
	// Does string match #name# pattern
	return symbolName.at(0) == '#' && symbolName.at(symbolName.length() - 1) == '#';
}
