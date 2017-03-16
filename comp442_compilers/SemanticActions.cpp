#include "stdafx.h"
#include "SemanticActions.h"
#include "SymbolTableData.h"

// A simple data structure that keeps some context during the semantic actions
static struct SemanticContext {
	bool inParam;
	bool inPhase2;
	bool skipClass;
	bool skipFunction;
} context;


void SemanticActions::performAction(const SemanticSymbol& symbol,
                                    std::vector<SymbolTableRecord>& semanticStack,
                                    SymbolTable** currentTable,
                                    const Token& token,
                                    bool phase2, 
									std::vector<SemanticError>& semanticErrors) {

	context.inPhase2 = phase2;
	if (phase2) {
		//std::cout << "error check on";
	}
	if (!shouldSkip(symbol)) {
		if (semanticStack.empty()) {
			semanticStack.push_back(SymbolTableRecord());
		}
		SymbolTableRecord& top = semanticStack.back();
		SemanticActionContainer container = { symbol, semanticStack, top, currentTable, token, semanticErrors };
		ACTION_MAP.at(symbol.getName())(container);
	}
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
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_class;
	// add this record to our current table
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (!found.second && !context.inPhase2) {
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
		_goToScope(container, &container.top);
	} else {
		if  (isRedefined(*found.first, container.top)) {
			context.skipClass = true;
			if (context.inPhase2) {
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Class " + container.top.name + " redeclared on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			}
			
		} else {
			_goToScope(container, found.first);
		}
	}
	
	container.semanticStack.pop_back();
	
}
void SemanticActions::endClassEntryAndTable(SemanticActionContainer& container) {
	if (!context.skipClass) {
		_goToParentScope(container);
	}
	context.skipClass = false;
}
	  
void SemanticActions::createProgramTable(SemanticActionContainer& container) {
	
	// Add that this is a class type
	container.top.kind = SymbolKind::kind_function;
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);

	if (!context.inPhase2) {
		// add this record to our current table
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
		_goToScope(container, &container.top);
	} else {
		_goToScope(container, found.first);
	}
	
	container.semanticStack.pop_back();
}

void SemanticActions::endProgramTable(SemanticActionContainer& container) {
	_goToParentScope(container);
}
	  
void SemanticActions::createVariableEntry(SemanticActionContainer& container) {
	container.top.kind = SymbolKind::kind_variable;
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	
	if (!found.second && !context.inPhase2) {
		(*container.currentTable)->addRecord(container.top.name, container.top);
	} else {
		if (isRedefined(*found.first, container.top)) {
			if (context.inPhase2) {
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Variable " + container.top.name + " redeclared on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			}
		}
	}
	
	container.semanticStack.pop_back();
}

void SemanticActions::addParameter(SemanticActionContainer& container) {
	// Push a new param to the function data parameters
	container.top.functionData.parameters.push_back(std::pair<TypeStruct, std::string>());
	context.inParam = true;
}

void SemanticActions::createFuncEntryAndTable(SemanticActionContainer& container) {
	context.inParam = false;
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (!found.second && !context.inPhase2) {
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
	} else {
		_goToScope(container, found.first);
	}
	container.semanticStack.pop_back();
}

void SemanticActions::endFuncEntryAndTable(SemanticActionContainer& container) {
	if (!context.skipFunction) {
		_goToParentScope(container);
	}
	context.skipFunction= false;
}

void SemanticActions::startFuncDef(SemanticActionContainer& container) {
	// Set this to a function kind
	container.top.kind = SymbolKind::kind_function;
	// Move the data from type struture to the function return type structure
	container.top.functionData.returnType = container.top.typeStructure;
	// Reset the type structure, since this is a function. We moved that data to the function data
	container.top.typeStructure = {};

	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (found.second && isRedefined(*found.first, container.top)) {
		context.skipFunction = true;
		if (context.inPhase2) {
			SemanticError error;
			error.tokenLine = container.token.tokenLine;
			error.message = "Function " + container.top.name + " redeclared on line " + std::to_string(error.tokenLine);
			container.semanticErrors.push_back(error);
		}
	}

	
}
	  
void SemanticActions::storeId(SemanticActionContainer& container) {
	if (context.inParam) {
		container.top.functionData.parameters.back().second = container.token.lexeme;
	} else {
		container.top.name = container.token.lexeme;
		container.top.definedLocation = container.token.tokenIndex;
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

bool SemanticActions::isRedefined(SymbolTableRecord& found, SymbolTableRecord& record) {
	return found.definedLocation != record.definedLocation;
}

bool SemanticActions::shouldSkip(const SemanticSymbol& symbol) {
	if (context.skipClass && symbol.getName() != "#endClassEntryAndTable#") {
		return true;
	}
	if (context.skipFunction&& symbol.getName() != "#endFuncEntryAndTable#") {
		return true;
	}
	return false;
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