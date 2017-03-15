#include "stdafx.h"
#include "SemanticActions.h"
#include "SymbolTableData.h"

// A simple data structure that keeps some context during the semantic actions
static struct SemanticContext {
	bool inParam;
	bool phase2;
} context;


void SemanticActions::performAction(const SemanticSymbol& symbol, std::vector<SymbolTableRecord>& semanticStack, SymbolTable** currentTable, const Token& token) {
	if (semanticStack.empty()) {
		semanticStack.push_back(SymbolTableRecord());
	}
	SymbolTableRecord& top = semanticStack.back();
	SemanticActionContainer container = { symbol, semanticStack, top, currentTable, token };
	ACTION_MAP.at(symbol.getName())(container);
}


void SemanticActions::createGlobalTable(SemanticActionContainer& container) {
	// The perform action adds something to the stack, and we dont need it for the global table
	// so pop it off right away
	container.semanticStack.pop_back();
}

void SemanticActions::endGlobalTable(SemanticActionContainer& container) {
	// Pop the left over record
	container.semanticStack.pop_back();
}
	  
void SemanticActions::createClassEntryAndTable(SemanticActionContainer& container) {
	// Add the class name
	container.top.name = container.token.lexeme;
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_class;
	// add this record to our current table
	container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
	_goToScope(container, &container.top);
	container.semanticStack.pop_back();
	
}
void SemanticActions::endClassEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticActions::createProgramTable(SemanticActionContainer& container) {
	
	// Add the class name
	container.top.name = container.token.lexeme;
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_function;
	// add this record to our current table
	container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
	_goToScope(container, &container.top);
	container.semanticStack.pop_back();
}

void SemanticActions::endProgramTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticActions::createVariableEntry(SemanticActionContainer& container) {
	container.top.kind = SymbolKind::kind_variable;
	(*container.currentTable)->addRecord(container.top.name, container.top);
	container.semanticStack.pop_back();
}

void SemanticActions::addParameter(SemanticActionContainer& container) {
	// Push a new param to the function data parameters
	container.top.functionData.parameters.push_back(std::pair<TypeStruct, std::string>());
	context.inParam = true;
}

void SemanticActions::createFuncEntryAndTable(SemanticActionContainer& container) {
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

void SemanticActions::endFuncEntryAndTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}

void SemanticActions::startFuncDef(SemanticActionContainer& container) {
	// Set this to a function kind
	container.top.kind = SymbolKind::kind_function;
	// Move the data from type struture to the function return type structure
	container.top.functionData.returnType = container.top.typeStructure;
	// Reset the type structure, since this is a function. We moved that data to the function data
	container.top.typeStructure = {};
	
}
	  
void SemanticActions::storeId(SemanticActionContainer& container) {
	if (context.inParam) {
		container.top.functionData.parameters.back().second = container.token.lexeme;
	} else {
		container.top.name = container.token.lexeme;
	}
}

void SemanticActions::storeType(SemanticActionContainer& container) {
	SymbolType type = stringToType(container.token.lexeme);
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

void SemanticActions::storeArraySize(SemanticActionContainer& container) {
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
void SemanticActions::_goToParentScope(SemanticActionContainer& container) {
	// Leave the current scope and go to the parent scope
	(*container.currentTable) = (*container.currentTable)->parent;
}

void SemanticActions::_goToScope(SemanticActionContainer& container, SymbolTableRecord* record) {
	*container.currentTable = record->scope.get();
}

// Map from semantic action name to function pointer that handles that action
std::unordered_map<std::string, void(*)(SemanticActionContainer&)> SemanticActions::ACTION_MAP = {
	{ "#createGlobalTable#",		&SemanticActions::createGlobalTable },
	{ "#endGlobalTable#",			&SemanticActions::endGlobalTable },
	{ "#createClassEntryAndTable#",	&SemanticActions::createClassEntryAndTable },
	{ "#endClassEntryAndTable#",	&SemanticActions::endClassEntryAndTable },
	{ "#createProgramTable#",		&SemanticActions::createProgramTable },
	{ "#endProgramTable#",			&SemanticActions::endProgramTable },
	{ "#createVariableEntry#",		&SemanticActions::createVariableEntry },
	{ "#addParameter#",				&SemanticActions::addParameter },
	{ "#createFuncEntryAndTable#",	&SemanticActions::createFuncEntryAndTable },
	{ "#endFuncEntryAndTable#",		&SemanticActions::endFuncEntryAndTable },
	{ "#startFuncDef#",				&SemanticActions::startFuncDef },
	{ "#storeId#",					&SemanticActions::storeId },
	{ "#storeType#",				&SemanticActions::storeType },
	{ "#storeArraySize#",			&SemanticActions::storeArraySize },
};