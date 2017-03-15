#include "stdafx.h"
#include "SemanticAction.h"


// A simple data structure that keeps some context during the semantic actions
static struct SemanticContext {
	bool inParam;
} context;


void SemanticAction::performAction(const SemanticSymbol& symbol, std::vector<SymbolTableRecord>& semanticStack, SymbolTable** currentTable, const Token& token) {
	if (semanticStack.empty()) {
		semanticStack.push_back(SymbolTableRecord());
	}
	SymbolTableRecord& top = semanticStack.back();
	SemanticActionContainer container = { symbol, semanticStack, top, currentTable, token };
	ACTION_MAP.at(symbol.getName())(container);
}


void SemanticAction::createGlobalTable(SemanticActionContainer& container) {
	// The perform action adds something to the stack, and we dont need it for the global table
	// so pop it off right away
	container.semanticStack.pop_back();
}

void SemanticAction::endGlobalTable(SemanticActionContainer& container) {
	// Pop the left over record
	container.semanticStack.pop_back();
}
	  
void SemanticAction::createClassEntryAndTable(SemanticActionContainer& container) {
	// Add the class name
	container.top.name = container.token.lexeme;
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_class;
	// add this record to our current table
	container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
	_goToScope(container, &container.top);
	container.semanticStack.pop_back();
	
}
void SemanticAction::endClassEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticAction::createProgramTable(SemanticActionContainer& container) {
	
	// Add the class name
	container.top.name = container.token.lexeme;
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_function;
	// add this record to our current table
	container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
	_goToScope(container, &container.top);
	container.semanticStack.pop_back();
}

void SemanticAction::endProgramTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticAction::createVariableEntry(SemanticActionContainer& container) {
	container.top.kind = SymbolKind::kind_variable;
	(*container.currentTable)->addRecord(container.top.name, container.top);
	container.semanticStack.pop_back();
}

void SemanticAction::addParameter(SemanticActionContainer& container) {
	// Push a new param to the function data parameters
	container.top.functionData.parameters.push_back(std::pair<TypeStruct, std::string>());
	context.inParam = true;
}

void SemanticAction::createFuncEntryAndTable(SemanticActionContainer& container) {
	context.inParam = false;
	container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
	// Pop this record of the semantic stack and it to our table entry
	_goToScope(container, &container.top);
	// We added the record to the symbol table, but now we have to create the child table/scope and add all the parameter entries to it
	for (std::pair<TypeStruct, std::string>& param : container.top.functionData.parameters) {
		SymbolTableRecord paramRecord;
		paramRecord.kind = SymbolKind::kind_parameter;
		paramRecord.name = param.second;
		paramRecord.typeStructure = param.first;
		(*container.currentTable)->addRecord(paramRecord.name, paramRecord);
	}
	container.semanticStack.pop_back();
}

void SemanticAction::endFuncEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}

void SemanticAction::startFuncDef(SemanticActionContainer& container) {
	// Set this to a function kind
	container.top.kind = SymbolKind::kind_function;
	// Move the data from type struture to the function return type structure
	container.top.functionData.returnType = container.top.typeStructure;
	// Reset the type structure, since this is a function. We moved that data to the function data
	container.top.typeStructure = {};
	
}
	  
void SemanticAction::storeId(SemanticActionContainer& container) {
	if (context.inParam) {
		container.top.functionData.parameters.back().second = container.token.lexeme;
	} else {
		container.top.name = container.token.lexeme;
	}
}

void SemanticAction::storeType(SemanticActionContainer& container) {
	SymbolType type = _stringToType(container.token.lexeme);
	std::string className = type == SymbolType::type_class ? container.token.lexeme : "";

	if (context.inParam) {
		std::pair<TypeStruct, std::string>& param = container.top.functionData.parameters.back();
		param.first.type = type;
		param.first.className = className;
	} else {
		container.top.typeStructure.type = type;
		container.top.typeStructure.className = className;
	}

}

void SemanticAction::storeArraySize(SemanticActionContainer& container) {
	if (context.inParam) {
		std::pair<TypeStruct, std::string>& param = container.top.functionData.parameters.back();
		param.first.dimensions.push_back(std::stoi(container.token.lexeme));
		param.first.structure = SymbolStructure::struct_array;
	} else {
		container.top.typeStructure.dimensions.push_back(std::stoi(container.token.lexeme));
		// Set this type structure to an array since we know that it has some dimension
		container.top.typeStructure.structure = SymbolStructure::struct_array;
	}
	
}

// --------------------------------- INTERNAL HELPER METHODS ----------------------------------
void SemanticAction::_goToParentScope(SemanticActionContainer& container) {
	// Leave the current scope and go to the parent scope
	(*container.currentTable) = (*container.currentTable)->parent;
}

void SemanticAction::_goToScope(SemanticActionContainer& container, SymbolTableRecord* record) {
	*container.currentTable = record->scope.get();
}

SymbolType SemanticAction::_stringToType(const std::string& storeType) {
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