#ifndef SEMANTIC_ACTION_H
#define SEMANTIC_ACTION_H

#include "SemanticSymbol.h"
#include "SymbolTable.h"
#include "SymbolTableRecord.h"
#include "Token.h"

struct SemanticError {
	int tokenLine;// the line it is defined
	std::string message; // the message for this semantic error
};

// A simple container that gets passed to each semantic action
// Holds all the data need for the semantic actions
struct SemanticActionContainer {
	const SemanticSymbol& symbol;
	std::vector<SymbolTableRecord>& semanticStack;
	SymbolTable& globalTable;
	SymbolTableRecord& top;
	SymbolTable** currentTable;
	const Token& token;
	std::vector<SemanticError>& semanticErrors;
	bool* parserError;
	MoonGenerator* generator;
};


class SemanticActions {
public:
	// Map of action name to function pointers that call the action
	static std::unordered_map<std::string, void (*)(SemanticActionContainer&)> ACTION_MAP;

	// Perform action takes in the semantic action, and redirects the action to the given function handler
	static void performAction(const SemanticSymbol& symbol,
		std::vector<SymbolTableRecord>& semanticStack,
		SymbolTable& globalTable,
		SymbolTable** currentTable,
		const Token& token,
		bool phase2,
		std::vector<SemanticError>& semanticErrors,
		bool* parserError,
		MoonGenerator* generator);
private:
	// All of our semantic actions in the grammar

	// semantic actions to build the symbol table
	static void createGlobalTable(SemanticActionContainer& container);
	static void endGlobalTable(SemanticActionContainer& container);
	static void createClassEntryAndTable(SemanticActionContainer& container);
	static void endClassEntryAndTable(SemanticActionContainer& container);
	static void createProgramTable(SemanticActionContainer& container);
	static void endProgramTable(SemanticActionContainer& container);
	static void createVariableEntry(SemanticActionContainer& container);
	static void addFuncDefParameter(SemanticActionContainer& container);
	static void createFuncEntryAndTable(SemanticActionContainer& container);
	static void endFuncEntryAndTable(SemanticActionContainer& container);
	static void startFuncDef(SemanticActionContainer& container);
	static void checkTypeGlobal(SemanticActionContainer& container);
	static void checkCircular(SemanticActionContainer& container);
	static void storeId(SemanticActionContainer& container);
	static void storeType(SemanticActionContainer& container);
	static void storeArraySize(SemanticActionContainer& container);
	static void checkIfReturns(SemanticActionContainer& container);

	// Expression building actions
	static void addNumericExprFragment(SemanticActionContainer& container);
	static void addSignExprFragment(SemanticActionContainer& container);
	static void operatorExprFragment(SemanticActionContainer& container);
	static void addLeftParen(SemanticActionContainer& container);
	static void addRightParen(SemanticActionContainer& container);
	static void pushExpr(SemanticActionContainer& container);
	static void popExpr(SemanticActionContainer& container);

	// Variable building action
	static void pushVar(SemanticActionContainer& container);
	static void popVar(SemanticActionContainer& container);
	static void addToVar(SemanticActionContainer& container);
	static void startFuncCall(SemanticActionContainer& container);


	// Statemtent building actions
	static void pushStatement(SemanticActionContainer& container);
	static void popStatement(SemanticActionContainer& container);
	static void assignmentStatementStart(SemanticActionContainer& container);
	static void forStatementStart(SemanticActionContainer& container);
	static void forInitStatementStart(SemanticActionContainer& container);
	static void forInitStatementEnd(SemanticActionContainer& container);
	static void forIncrementStatementStart(SemanticActionContainer& container);
	static void forIncrementStatementEnd(SemanticActionContainer& container);
	static void forRelExpr(SemanticActionContainer& container);
	static void forAddVar(SemanticActionContainer& container);
	
	static void ifelseStatementStart(SemanticActionContainer& container);
	static void setInElseBlock(SemanticActionContainer& container);
	static void getStatementStart(SemanticActionContainer& container);
	static void putStatementStart(SemanticActionContainer& container);
	static void returnStatementStart(SemanticActionContainer& container);
	static void assignmentStatementEnd(SemanticActionContainer& container);
	static void forStatementEnd(SemanticActionContainer& container);
	static void ifelseStatementEnd(SemanticActionContainer& container);
	static void getStatementEnd(SemanticActionContainer& container);
	static void putStatementEnd(SemanticActionContainer& container);
	static void returnStatementEnd(SemanticActionContainer& container);


	// Internal methods
	static void _goToParentScope(SemanticActionContainer& container);
	static void _goToScope(SemanticActionContainer& container, SymbolTableRecord* record);
	static bool _isRedefined(SymbolTableRecord& found, SymbolTableRecord& record);
	static bool _shouldSkip(const SemanticSymbol& symbol);
	static bool _isCircularDependent(SymbolTable& global, SymbolTable& firstTable, SymbolTable& dependentTable, const std::string& dependency);
	static bool _isCircularDependent(SymbolTable& global, SymbolTable& dependentTable, const std::string& dependency);
	static void _unmarkAllTables(SymbolTable& global);
	static void _checkVarError(SemanticActionContainer& container);
	static void _reportError(SemanticActionContainer& container, std::string message);
	
};


#endif // !SEMANTIC_ACTION


