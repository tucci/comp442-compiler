#include "stdafx.h"
#include "SemanticAction.h"




void SemanticAction::performAction(const SemanticSymbol& symbol, SemanticContext& context, SymbolTable** currentTable, const Token& token) {
	SemanticActionContainer container = { symbol, context, currentTable, token };
	std::string action = symbol.getName();
	std::cout << action << std::endl;
	ACTION_MAP.at(action)(container);
}


void SemanticAction::createGlobalTable(SemanticActionContainer& container) {

}
	  
void SemanticAction::createClassEntryAndTable(SemanticActionContainer& container) {
	SymbolTableRecord record;
	// Add the class name
	record.name = container.token.lexeme;
	// Add that this is a class type
	record.kind = SymbolKind::kind_class;
	// add this record to our current table
	
	record = *(*container.currentTable)->addRecord(record.name, record, *container.currentTable);
	_goToScope(container, &record);
	
}
void SemanticAction::endClassEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticAction::createProgramTable(SemanticActionContainer& container) {
	SymbolTableRecord record;
	// Add the class name
	record.name = container.token.lexeme;
	// Add that this is a class type
	record.kind = SymbolKind::kind_function;
	// add this record to our current table
	record = *(*container.currentTable)->addRecord(record.name, record, *container.currentTable);
	_goToScope(container, &record);
	
}

void SemanticAction::endProgramTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticAction::createVariableEntry(SemanticActionContainer& container) {

}

void SemanticAction::createParameterEntry(SemanticActionContainer& container) {

}

void SemanticAction::createFuncEntryAndTable(SemanticActionContainer& container) {

}

void SemanticAction::endFuncEntryAndTable(SemanticActionContainer& container) {

}

void SemanticAction::startFuncDef(SemanticActionContainer& container) {

}

void SemanticAction::startParamEntry(SemanticActionContainer& container) {

}
	  
void SemanticAction::storeId(SemanticActionContainer& container) {
	container.context.storeId = container.token.lexeme;
}

void SemanticAction::storeType(SemanticActionContainer& container) {
	container.context.storeType = container.token.lexeme;
}

void SemanticAction::storeArraySize(SemanticActionContainer& container) {
	container.context.storeArraySize.push_back(std::stoi(container.token.lexeme));
}


void SemanticAction::_goToParentScope(SemanticActionContainer& container) {
	// Leave the current scope and go to the parent scope
	(*container.currentTable) = (*container.currentTable)->parent;
}

void SemanticAction::_goToScope(SemanticActionContainer& container, SymbolTableRecord* record) {
	*container.currentTable = record->scope.get();
}

std::unordered_map<std::string, void(*)(SemanticActionContainer&)> SemanticAction::ACTION_MAP = {
	{ "#createGlobalTable#",		&SemanticAction::createGlobalTable },
	{ "#createClassEntryAndTable#",	&SemanticAction::createClassEntryAndTable },
	{ "#endClassEntryAndTable#",	&SemanticAction::endClassEntryAndTable },
	{ "#createProgramTable#",		&SemanticAction::createProgramTable },
	{ "#endProgramTable#",			&SemanticAction::endProgramTable },
	{ "#createVariableEntry#",		&SemanticAction::createVariableEntry },
	{ "#createParameterEntry#",		&SemanticAction::createParameterEntry },
	{ "#createFuncEntryAndTable#",	&SemanticAction::createFuncEntryAndTable },
	{ "#endFuncEntryAndTable#",		&SemanticAction::endFuncEntryAndTable },
	{ "#startFuncDef#",				&SemanticAction::startFuncDef },
	{ "#startParamEntry#",			&SemanticAction::startParamEntry },
	{ "#storeId#",					&SemanticAction::storeId },
	{ "#storeType#",				&SemanticAction::storeType },
	{ "#storeArraySize#",			&SemanticAction::storeArraySize },
};