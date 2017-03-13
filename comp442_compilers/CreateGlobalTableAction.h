#ifndef CREATE_GLOBAL_TABLE_ACTION_H
#define CREATE_GLOBAL_TABLE_ACTION_H

#include "SemanticAction.h"

class CreateGlobalTableAction : public SemanticAction {
public:
	CreateGlobalTableAction() {

	}

	~CreateGlobalTableAction() {

	}

	void execute(SemanticContext& context, SymbolTable* currentTable, const Token& token) {

	}
	
	
};

#endif // !CREATE_GLOBAL_TABLE_ACTION_H

