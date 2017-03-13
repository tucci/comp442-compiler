#ifndef SEMANTIC_CONTEXT_H
#define SEMANTIC_CONTEXT_H

#include "stdafx.h"

// A simple container holding current data about the parse
struct SemanticContext {
	std::string storeId;
	std::string storeType;
	std::vector<int> storeArraySize;
	// Keep a local copy of the function data as we create the function def
	SymbolTableRecord functionDataAggregate;

};
#endif // !SEMANTIC_CONTEXT_H

