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
	SymbolTableRecord record;
	record.kind = SymbolKind::kind_variable;
	_fillRecordFromContext(container, record);
	(*container.currentTable)->addRecord(record.name, record);
	
}

void SemanticAction::createParameterEntry(SemanticActionContainer& container) {
	SymbolTableRecord record;
	record.kind = SymbolKind::kind_parameter;
	_fillRecordFromContext(container, record);
	(*container.currentTable)->addRecord(record.name, record);
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

// --------------------------------- INTERNAL HELPER METHODS ----------------------------------
void SemanticAction::_goToParentScope(SemanticActionContainer& container) {
	// Leave the current scope and go to the parent scope
	(*container.currentTable) = (*container.currentTable)->parent;
}

void SemanticAction::_goToScope(SemanticActionContainer& container, SymbolTableRecord* record) {
	*container.currentTable = record->scope.get();
}

SymbolType SemanticAction::_storedTypeToType(const std::string& storeType) {
	// TODO: dont hardcode this
	if (storeType == "int") {
		// this is a int type
		return SymbolType::type_int;
	} else if (storeType == "float") {
		// this is a float type
		return SymbolType::type_float;
	} else {
		// This is a class
		return SymbolType::type_class;
	}
}

void SemanticAction::_fillRecordFromContext(SemanticActionContainer& container, SymbolTableRecord& record) {
	record.name = container.context.storeId;
	SymbolType type = _storedTypeToType(container.context.storeType);
	record.typeStructure.type = type;
	// Set the name of the class
	if (type == SymbolType::type_class) {
		record.typeStructure.className = container.context.storeType;
	}

	if (!container.context.storeArraySize.empty()) {
		// this is an array
		record.typeStructure.structure = SymbolStructure::struct_array;
		record.typeStructure.dimensions = container.context.storeArraySize;
	} else {
		// this is not an array and is a simple structure
		record.typeStructure.structure = SymbolStructure::struct_simple;
	}
	_clearContext(container);
}

void SemanticAction::_clearContext(SemanticActionContainer& container) {
	// Clear the array
	container.context.storeArraySize.clear();
	container.context.storeId.clear();
	container.context.storeType.clear();
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