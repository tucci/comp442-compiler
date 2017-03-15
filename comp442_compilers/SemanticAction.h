#ifndef SEMANTIC_ACTION_H
#define SEMANTIC_ACTION_H

#include "SemanticSymbol.h"
#include "SymbolTable.h"
#include "Token.h"

// A simple container that gets passed
struct SemanticActionContainer {
	const SemanticSymbol& symbol;
	std::vector<SymbolTableRecord>& semanticStack;
	SymbolTableRecord& top;
	SymbolTable** currentTable;
	const Token& token;
};

class SemanticAction {
public:
	// Map of action name to function pointers that call the action
	static std::unordered_map<std::string, void (*)(SemanticActionContainer&)> ACTION_MAP;
	static void performAction(const SemanticSymbol& symbol, std::vector<SymbolTableRecord>& semanticStack, SymbolTable** currentTable, const Token& token);
private:
	// All over our semantic actions in the grammar
	static void createGlobalTable(SemanticActionContainer& container);
	static void endGlobalTable(SemanticActionContainer& container);
	static void createClassEntryAndTable(SemanticActionContainer& container);
	static void endClassEntryAndTable(SemanticActionContainer& container);
	static void createProgramTable(SemanticActionContainer& container);
	static void endProgramTable(SemanticActionContainer& container);
	static void createVariableEntry(SemanticActionContainer& container);
	static void addParameter(SemanticActionContainer& container);
	static void createFuncEntryAndTable(SemanticActionContainer& container);
	static void endFuncEntryAndTable(SemanticActionContainer& container);
	static void startFuncDef(SemanticActionContainer& container);
	static void storeId(SemanticActionContainer& container);
	static void storeType(SemanticActionContainer& container);
	static void storeArraySize(SemanticActionContainer& container);


	static void _goToParentScope(SemanticActionContainer& container);
	static void _goToScope(SemanticActionContainer& container, SymbolTableRecord* record);
	static SymbolType _stringToType(const std::string& storeType);	
	
	
};


#endif // !SEMANTIC_ACTION
