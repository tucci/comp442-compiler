#ifndef SEMANTIC_ACTION_H
#define SEMANTIC_ACTION_H

// TODO: rewrite this in stdafx
#include "SemanticSymbol.h"
#include "SymbolTable.h"
#include "SymbolTableData.h"
#include "Token.h"

struct SemanticError {
	int tokenLine;// the line it is defined
	std::string message; // the message for this semantic error
};

// A simple container that gets passed to each semantic action
struct SemanticActionContainer {
	const SemanticSymbol& symbol;
	std::vector<SymbolTableRecord>& semanticStack;
	SymbolTable& globalTable;
	SymbolTableRecord& top;
	SymbolTable** currentTable;
	const Token& token;
	std::vector<SemanticError>& semanticErrors;
};

class SemanticActions {
public:
	// Map of action name to function pointers that call the action
	static std::unordered_map<std::string, void (*)(SemanticActionContainer&)> ACTION_MAP;
	static void performAction(const SemanticSymbol& symbol,
		std::vector<SymbolTableRecord>& semanticStack,
		SymbolTable& globalTable,
		SymbolTable** currentTable,
		const Token& token,
		bool phase2,
		std::vector<SemanticError>& semanticErrors);
private:
	// All of our semantic actions in the grammar
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

	static void checkTypeGlobal(SemanticActionContainer& container);

	static void storeId(SemanticActionContainer& container);
	static void storeType(SemanticActionContainer& container);
	static void storeArraySize(SemanticActionContainer& container);


	// Internal methods
	static void _goToParentScope(SemanticActionContainer& container);
	static void _goToScope(SemanticActionContainer& container, SymbolTableRecord* record);
	static bool _isRedefined(SymbolTableRecord& found, SymbolTableRecord& record);
	static bool _shouldSkip(const SemanticSymbol& symbol);
	
	
};


#endif // !SEMANTIC_ACTION

