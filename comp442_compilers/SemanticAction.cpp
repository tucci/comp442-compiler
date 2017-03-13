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

void SemanticAction::endGlobalTable(SemanticActionContainer& container) {
	std::cout << "end global table" << std::endl;
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

void SemanticAction::addParameter(SemanticActionContainer& container) {
	

	SymbolTableRecord& functionRecord = container.context.functionDataAggregate;
	FunctionData& function = functionRecord.functionData;
	std::pair<TypeStruct, std::string> typeAndName;

	// The type of this param
	TypeStruct& paramType = typeAndName.first;
	// Get the name of this param
	typeAndName.second = container.context.storeId;
	//  Get the return type for this functin
	SymbolType type = _storedTypeToType(container.context.storeType);
	paramType.type = type;
	// Set the name of the class of the return type
	if (type == SymbolType::type_class) {
		paramType.className = container.context.storeType;
	}
	// Even though we shouldn't have functions that return arrays, it is here for completeness sake
	if (!container.context.storeArraySize.empty()) {
		// this is an array
		paramType.structure = SymbolStructure::struct_array;
		paramType.dimensions = container.context.storeArraySize;
	} else {
		// this is not an array and is a simple structure
		paramType.structure = SymbolStructure::struct_simple;
	}

	// Add this parameter to the parameter list
	function.parameters.push_back(typeAndName);

}

void SemanticAction::createFuncEntryAndTable(SemanticActionContainer& container) {

	SymbolTableRecord record = container.context.functionDataAggregate;
	record = *(*container.currentTable)->addRecord(record.name, record, *container.currentTable);
	_clearContext(container);
	_goToScope(container, &record);
	// We added the record to the symbol table, but now we have to create the child table/scope and add all the parameter entries to it
	for (std::pair<TypeStruct, std::string>& param : record.functionData.parameters) {
		SymbolTableRecord paramRecord;
		paramRecord.kind = SymbolKind::kind_parameter;
		paramRecord.name = param.second;
		paramRecord.typeStructure = param.first;
		(*container.currentTable)->addRecord(paramRecord.name, paramRecord);
	}
}

void SemanticAction::endFuncEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}

void SemanticAction::startFuncDef(SemanticActionContainer& container) {
	SymbolTableRecord& functionRecord = container.context.functionDataAggregate;
	FunctionData& function = functionRecord.functionData;

	// Set this to a function kind
	functionRecord.kind = SymbolKind::kind_function;
	// Set the name of this function from the stored id
	functionRecord.name = container.context.storeId;

	//  Get the return type for this functin
	SymbolType type = _storedTypeToType(container.context.storeType);
	function.returnType.type = type;
	// Set the name of the class of the return type
	if (type == SymbolType::type_class) {
		function.returnType.className = container.context.storeType;
	}
	// Even though we shouldn't have functions that return arrays, it is here for completeness sake
	if (!container.context.storeArraySize.empty()) {
		// this is an array
		function.returnType.structure = SymbolStructure::struct_array;
		function.returnType.dimensions = container.context.storeArraySize;
	} else {
		// this is not an array and is a simple structure
		function.returnType.structure = SymbolStructure::struct_simple;
	}
	
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
	// Clear record data;
	container.context.functionDataAggregate = {};
}

std::unordered_map<std::string, void(*)(SemanticActionContainer&)> SemanticAction::ACTION_MAP = {
	{ "#createGlobalTable#",		&SemanticAction::createGlobalTable },
	{ "#endGlobalTable#",			&SemanticAction::endGlobalTable },
	{ "#createClassEntryAndTable#",	&SemanticAction::createClassEntryAndTable },
	{ "#endClassEntryAndTable#",	&SemanticAction::endClassEntryAndTable },
	{ "#createProgramTable#",		&SemanticAction::createProgramTable },
	{ "#endProgramTable#",			&SemanticAction::endProgramTable },
	{ "#createVariableEntry#",		&SemanticAction::createVariableEntry },
	{ "#addParameter#",				&SemanticAction::addParameter },
	{ "#createFuncEntryAndTable#",	&SemanticAction::createFuncEntryAndTable },
	{ "#endFuncEntryAndTable#",		&SemanticAction::endFuncEntryAndTable },
	{ "#startFuncDef#",				&SemanticAction::startFuncDef },
	{ "#storeId#",					&SemanticAction::storeId },
	{ "#storeType#",				&SemanticAction::storeType },
	{ "#storeArraySize#",			&SemanticAction::storeArraySize },
};