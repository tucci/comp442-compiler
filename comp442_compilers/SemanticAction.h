#ifndef SEMANTIC_ACTION_H
#define SEMANTIC_ACTION_H

#include "SemanticSymbol.h"
#include "SemanticContext.h"
#include "SymbolTable.h"
#include "Token.h"

// A simple container that gets passed
struct SemanticActionContainer {
	const SemanticSymbol& symbol;
	SemanticContext& context;
	SymbolTable** currentTable;
	const Token& token;
};

class SemanticAction {
public:
	// Map of action name to function pointers that call the action
	static std::unordered_map<std::string, void (*)(SemanticActionContainer&)> ACTION_MAP;
	static void performAction(const SemanticSymbol& symbol, SemanticContext& context, SymbolTable** currentTable, const Token& token);
private:
	// All over our semantic actions in the grammar
	static void createGlobalTable(SemanticActionContainer& container);
	static void createClassEntryAndTable(SemanticActionContainer& container);
	static void endClassEntryAndTable(SemanticActionContainer& container);
	static void createProgramTable(SemanticActionContainer& container);
	static void endProgramTable(SemanticActionContainer& container);
	static void createVariableEntry(SemanticActionContainer& container);
	static void createParameterEntry(SemanticActionContainer& container);
	static void createFuncEntryAndTable(SemanticActionContainer& container);
	static void endFuncEntryAndTable(SemanticActionContainer& container);
	static void startFuncDef(SemanticActionContainer& container);
	static void startParamEntry(SemanticActionContainer& container);
	static void storeId(SemanticActionContainer& container);
	static void storeType(SemanticActionContainer& container);
	static void storeArraySize(SemanticActionContainer& container);


	static void _goToParentScope(SemanticActionContainer& container);
	static void _goToScope(SemanticActionContainer& container, SymbolTableRecord* record);
	
	
};


#endif // !SEMANTIC_ACTION
