#include "stdafx.h"
#include "SemanticActions.h"
#include "SymbolTableData.h"

// A simple data structure that keeps some context during the semantic actions
static struct SemanticContext {
	bool inParam = false;
	bool inPhase2 = false;
	bool skipClass = false;
	bool skipFunction = false;
} context;


void SemanticActions::performAction(const SemanticSymbol& symbol,
                                    std::vector<SymbolTableRecord>& semanticStack,
                                    SymbolTable& globalTable,
                                    SymbolTable** currentTable,
                                    const Token& token,
                                    bool phase2, std::vector<SemanticError>& semanticErrors) {

	context.inPhase2 = phase2;
	if (!_shouldSkip(symbol)) {
		if (semanticStack.empty()) {
			semanticStack.push_back(SymbolTableRecord());
		}
		SymbolTableRecord& top = semanticStack.back();
		SemanticActionContainer container = {symbol, semanticStack, globalTable, top, currentTable, token, semanticErrors};

		// TODO: remove try/catch
		try {
			ACTION_MAP.at(symbol.getName())(container);
		} catch(std::exception e) {
			std::cout <<  "unknown symbol " + symbol.getName();
			throw e;
		}
		
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
	
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (!found.second && !context.inPhase2) {
		// add this record to our current table
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable);
		_goToScope(container, &container.top);
	}
	else {
		if (_isRedefined(*found.first, container.top)) {
			context.skipClass = true;
			if (context.inPhase2) {
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Class " + container.top.name + " redeclared on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			}

		}
		else {
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
	}
	else {
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
	}
	else {
		if (_isRedefined(*found.first, container.top)) {
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
	}
	else {
		_goToScope(container, found.first);
	}
	container.semanticStack.pop_back();
}

void SemanticActions::endFuncEntryAndTable(SemanticActionContainer& container) {
	if (!context.skipFunction) {
		_goToParentScope(container);
	}
	context.skipFunction = false;
}

void SemanticActions::startFuncDef(SemanticActionContainer& container) {
	// Set this to a function kind
	container.top.kind = SymbolKind::kind_function;
	// Move the data from type struture to the function return type structure
	container.top.functionData.returnType = container.top.typeStructure;
	// Reset the type structure, since this is a function. We moved that data to the function data
	container.top.typeStructure = {};

	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (found.second && _isRedefined(*found.first, container.top)) {
		context.skipFunction = true;
		if (context.inPhase2) {
			SemanticError error;
			error.tokenLine = container.token.tokenLine;
			error.message = "Function " + container.top.name + " redeclared on line " + std::to_string(error.tokenLine);
			container.semanticErrors.push_back(error);
		}
	}


}

void SemanticActions::checkTypeGlobal(SemanticActionContainer& container) {
	if (context.inPhase2) {
		std::string typeName = container.token.lexeme;
		std::pair<SymbolTableRecord*, bool> found = container.globalTable.find(typeName);
		if (!found.second) {
			// We have a type being used that is not found in the global table
			SemanticError error;
			error.tokenLine = container.token.tokenLine;
			error.message = "Type " + typeName + " not defined on line " + std::to_string(error.tokenLine);
			container.semanticErrors.push_back(error);
		}
	}
}

void SemanticActions::checkCircular(SemanticActionContainer& container) {
	if (context.inPhase2) {
		// The class that the current definition is being defined in
		std::string definedInClass = (*container.currentTable)->name; // Ex: ClassA
		// Get the class name we are defining in this class
		std::string className = container.token.lexeme; // ClassB

		if (_isCircularDependent(container.globalTable, **container.currentTable, className)) {
			SemanticError error;
			error.tokenLine = container.token.tokenLine;
			error.message = "Circular dependency of " + className + " on line " + std::to_string(error.tokenLine);
			container.semanticErrors.push_back(error);
		}
		_unmarkAllTables(container.globalTable);

	}
}

void SemanticActions::storeId(SemanticActionContainer& container) {
	if (context.inParam) {
		bool isRedefinition = false;
		// Check to see if we are adding a redefined parameter
		for (auto param : container.top.functionData.parameters) {
			if (param.second == container.token.lexeme) {
				if (context.inPhase2) {
					isRedefinition = true;
					// We have a redefinnition
					SemanticError error;
					error.tokenLine = container.token.tokenLine;
					error.message = "Parameter " + container.token.lexeme + " redeclared on line " + std::to_string(error.tokenLine);
					container.semanticErrors.push_back(error);
				}
			}
		}
		if (!isRedefinition) {
			container.top.functionData.parameters.back().second = container.token.lexeme;
		}
		else {
			// Remove this parameter from the list
			container.top.functionData.parameters.pop_back();
		}
	}
	else {
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
	}
	else {
		container.top.typeStructure.type = type;
		container.top.typeStructure.className = className;
	}

}

void SemanticActions::storeArraySize(SemanticActionContainer& container) {
	if (context.inParam) {
		std::pair<TypeStruct, std::string>& param = container.top.functionData.parameters.back();
		param.first.dimensions.push_back(std::stoi(container.token.lexeme));
		param.first.structure = SymbolStructure::struct_array;
	}
	else {
		container.top.typeStructure.dimensions.push_back(std::stoi(container.token.lexeme));
		// Set this type structure to an array since we know that it has some dimension
		container.top.typeStructure.structure = SymbolStructure::struct_array;
	}
}


void SemanticActions::addNumericExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addNumeric(container.token.lexeme);
	}
}



void SemanticActions::addSignExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		
	}
}

void SemanticActions::operatorExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addOperator(container.token.lexeme);
	}

}

void SemanticActions::checkExpr(SemanticActionContainer& container) {
	if (context.inPhase2) {

	}
}

void SemanticActions::pushExpr(SemanticActionContainer& container) {
	if (context.inPhase2) {
		SymbolTableRecord record;
		record.attr.type = AttributeType::attr_expr;
		container.semanticStack.push_back(record);
	}
}

void SemanticActions::popExpr(SemanticActionContainer& container) {
	if (context.inPhase2) {
		SymbolTableRecord popped = container.top;
		container.semanticStack.pop_back();
		SymbolTableRecord& top = container.semanticStack.back();
		switch (top.attr.type) {

		case attr_expr: {
			top.attr.expr = popped.attr.expr;
			break;
		} 
		case attr_statement: {
			if (top.attr.statmenent.statType == StatmentType::assignStat) {
				top.attr.statmenent.statData.assignStatement.expression = popped.attr.expr;
			}
			break;
		}
		case attr_var: {
			top.attr.var.vars.back().indices.push_back(popped.attr.expr);
			break;
		}
		default: ;
		}

		
	}
}

void SemanticActions::pushVar(SemanticActionContainer& container) {
	if (context.inPhase2) {
		SymbolTableRecord record;
		record.attr.type = AttributeType::attr_var;		
		container.semanticStack.push_back(record);
	}
}

void SemanticActions::popVar(SemanticActionContainer& container) {
	if (context.inPhase2) {
		_checkVarError(container);

		SymbolTableRecord popped = container.top;
		container.semanticStack.pop_back();
		SymbolTableRecord& top = container.semanticStack.back();
		switch (top.attr.type) {
		case attr_expr: {
			top.attr.expr.addVar(popped.attr.var);
			break;
		}
		case attr_statement: {
			switch (top.attr.statmenent.statType) {
			case assignStat: top.attr.statmenent.statData.assignStatement.var = popped.attr.var; break;
			case forStat: break;
			case ifelseStat: break;
			case getStat: top.attr.statmenent.statData.getStatement.var = popped.attr.var; break;
			case putStat: break;
			case returnStat: break;
			default:;
			}
			break;
		};
		case attr_var: {
			break;
		}
		default:;
		}
	}
}

void SemanticActions::addToVar(SemanticActionContainer& container) {
	if (context.inPhase2) {
		VariableFragment fragment;
		fragment.identifier = container.token.lexeme;
		if (container.top.attr.var.vars.size() == 0) {
			container.top.attr.var.location = container.token.tokenIndex;
		}
		container.top.attr.var.vars.push_back(fragment);
	}
}

void SemanticActions::setFunc(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.var.isFunc = true;
	}
}


void SemanticActions::pushStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		SymbolTableRecord record;
		record.attr.type = AttributeType::attr_statement;
		container.semanticStack.push_back(record);
	}
}

void SemanticActions::popStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statData.assignStatement.expression.toFullName();
		container.semanticStack.pop_back();
	}
}

void SemanticActions::assignmentStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::assignStat;
	}
}

void SemanticActions::forStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::forStat;
	}
}

void SemanticActions::ifelseStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::ifelseStat;
	}
}

void SemanticActions::getStatement(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::getStat;
	}
}

void SemanticActions::putStatment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::putStat;
	}
}

void SemanticActions::returnStatment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statmenent.statType = StatmentType::returnStat;
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

bool SemanticActions::_isRedefined(SymbolTableRecord& found, SymbolTableRecord& record) {
	return found.definedLocation != record.definedLocation;
}

bool SemanticActions::_shouldSkip(const SemanticSymbol& symbol) {
	if (context.skipClass && symbol.getName() != "#endClassEntryAndTable#") {
		return true;
	}
	if (context.skipFunction && symbol.getName() != "#endFuncEntryAndTable#") {
		return true;
	}
	return false;
}

bool SemanticActions::_isCircularDependent(SymbolTable& global, SymbolTable& firstTable, SymbolTable& dependentTable, const std::string& dependency) {

	if (dependentTable.marked) {
		return true;
	}
	dependentTable.marked = true;

	std::pair<SymbolTableRecord*, bool> dependencyTable = global.find(dependency);
	if (dependencyTable.second && dependencyTable.first->kind == SymbolKind::kind_class) {
		SymbolTable* possibleDependentTable = dependencyTable.first->scope.get();
		std::vector<SymbolTable*> possibleDependencies;

		for (auto identifier : possibleDependentTable->table) {
			if (identifier.second.typeStructure.type == SymbolType::type_class) {
				std::pair<SymbolTableRecord*, bool> scope = global.find(identifier.second.typeStructure.className);
				possibleDependencies.push_back(scope.first->scope.get());
			}
		}


		for (auto d : possibleDependencies) {
			// if the table is locally dependent on itself then this is table is circular because the other table is dependent on itself
			if (_isCircularDependent(global, firstTable, *possibleDependentTable, d->name)) {
				return true;
			}
		}

		return false;
	}
	// Table not found in global table
	// Possible not defined
	return false;

}

bool SemanticActions::_isCircularDependent(SymbolTable& global, SymbolTable& dependentTable, const std::string& dependency) {
	return _isCircularDependent(global, dependentTable, dependentTable, dependency);
}

void SemanticActions::_unmarkAllTables(SymbolTable& global) {
	for (std::unordered_map<std::basic_string<char>, SymbolTableRecord>::value_type record : global.table) {
		if (record.second.kind == SymbolKind::kind_class) {
			record.second.scope->marked = false;
		}
	}
}

void SemanticActions::_checkVarError(SemanticActionContainer& container) {
	Variable var = container.top.attr.var;
	VariableFragment varFragment;

	SymbolTable* currentScope = (*container.currentTable);
	std::string currentIdentifier;

	for (int i = 0; i < var.vars.size(); ++i) {
		varFragment = var.vars[i];
		currentIdentifier = varFragment.identifier;
		// Finds in current scope, or parent scopes
		std::pair<SymbolTableRecord*, bool> found;// = currentScope->findInParents(currentIdentifier);
		if (i == 0) {
			found = currentScope->findInParents(currentIdentifier);
		} else {
			found = currentScope->find(currentIdentifier);
		}
		SymbolTableRecord* varDefinition = found.first;

		if (!found.second) {
			// Not found, this variable is undefined
			SemanticError error;
			error.tokenLine = container.token.tokenLine;
			error.message = "Identifier " + var.toFullName() + " is undefined on line " + std::to_string(error.tokenLine);
			container.semanticErrors.push_back(error);
		} else {


			if (varDefinition->kind == SymbolKind::kind_function && !var.isFunc) {
				// The definition is a function, but it is not being used as a function
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Identifier " + var.toFullName() + " is not a function on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			} else if (varDefinition->kind != SymbolKind::kind_function && var.isFunc) {
				// The definition is not a function, but it is being used as a function
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Identifier " + var.toFullName() + " is not a function on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			}

			// If the var is being used before it has been declared
			// If it is a function, then we can ignore it's location
			if (varDefinition->kind != SymbolKind::kind_function && i == 0 && varDefinition->definedLocation > var.location) {
				SemanticError error;
				error.tokenLine = container.token.tokenLine;
				error.message = "Identifier " + var.toFullName() + " is used before it is defined on line " + std::to_string(error.tokenLine);
				container.semanticErrors.push_back(error);
			}


			switch (varDefinition->typeStructure.structure) {
			case struct_simple: {
				// trying to index a variable, but the variable is not an array
				if (varFragment.indices.size() != 0) {
					SemanticError error;
					error.tokenLine = container.token.tokenLine;
					error.message = "Identifier " + var.toFullName() + " is not an array on line " + std::to_string(error.tokenLine);
					container.semanticErrors.push_back(error);
				}
				break;
			}
			case struct_array: {
				//  Trying to use a array without indexing into it
				if (varFragment.indices.size() == 0) {
					SemanticError error;
					error.tokenLine = container.token.tokenLine;
					error.message = "Identifier " + var.toFullName() + " is not an array on line " + std::to_string(error.tokenLine);
					container.semanticErrors.push_back(error);
				} else {
					// Compare dimension length 
					// The dimensions are different
					if (varDefinition->typeStructure.dimensions.size() != varFragment.indices.size()) {
						SemanticError error;
						error.tokenLine = container.token.tokenLine;
						error.message = "Array " + var.toFullName() + " has wrong dimensions on line " + std::to_string(error.tokenLine);
						container.semanticErrors.push_back(error);
					} else {
						// Compare each size in the dimension
						// TODO: We cant compare sizes at compile time as we dont know what the values of the expressions are
						//std::vector<int> dimensions = varDefinition->typeStructure.dimensions;
						//for (int i = 0; i < dimensions.size(); ++i) {
						//	//if (dimensions[i] != std::stoi(varFragment.indices[i]))
						//}
					}

				}
				break;
			}
			default:;
			}

			// This is a object
			if (varDefinition->typeStructure.type == SymbolType::type_class) {
				// We need to nest down the object structure
				// Get the symbol table for this class type
				std::string className = varDefinition->typeStructure.className;
				currentScope = container.globalTable.find(className).first->scope.get();
			} else {
				// this is an int/float
				if (var.vars.size() - i > 1) {
					// However it is being used as it was an object
					SemanticError error;
					error.tokenLine = container.token.tokenLine;
					error.message = "Identifier " + var.toFullName() + " is not an object on line " + std::to_string(error.tokenLine);
					container.semanticErrors.push_back(error);
					return; // Early return
				}
			}

		}
	}

}


// Map from semantic action name to function pointer that handles that action
std::unordered_map<std::string, void(*)(SemanticActionContainer&)> SemanticActions::ACTION_MAP = {
			// Symbol Table building
			{"#createGlobalTable#", &SemanticActions::createGlobalTable},
			{"#endGlobalTable#", &SemanticActions::endGlobalTable},
			{"#createClassEntryAndTable#", &SemanticActions::createClassEntryAndTable},
			{"#endClassEntryAndTable#", &SemanticActions::endClassEntryAndTable},
			{"#createProgramTable#", &SemanticActions::createProgramTable},
			{"#endProgramTable#", &SemanticActions::endProgramTable},
			{"#createVariableEntry#", &SemanticActions::createVariableEntry},
			{"#addParameter#", &SemanticActions::addParameter},
			{"#createFuncEntryAndTable#", &SemanticActions::createFuncEntryAndTable},
			{"#endFuncEntryAndTable#", &SemanticActions::endFuncEntryAndTable},
			{"#startFuncDef#", &SemanticActions::startFuncDef},
			{"#checkTypeGlobal#", &SemanticActions::checkTypeGlobal},
			{"#checkCircular#", &SemanticActions::checkCircular},
			{ "#storeId#", &SemanticActions::storeId },
			{ "#storeType#", &SemanticActions::storeType },
			{ "#storeArraySize#", &SemanticActions::storeArraySize },
			// Expression building
			{"#addNumericExprFragment#", &SemanticActions::addNumericExprFragment},
			{"#operatorExprFragment#", &SemanticActions::operatorExprFragment},
			{"#addSignExprFragment#", &SemanticActions::addSignExprFragment},
			{"#checkExpr#", &SemanticActions::checkExpr},
			{"#pushExpr#", &SemanticActions::pushExpr},
			{"#popExpr#", &SemanticActions::popExpr},
			// Var building
			{ "#pushVar#", &SemanticActions::pushVar},
			{ "#popVar#", &SemanticActions::popVar},
			{ "#addToVar#", &SemanticActions::addToVar},
			{ "#setFunc#", &SemanticActions::setFunc },
			// Statement building
			{"#pushStatement#", &SemanticActions::pushStatement },
			{"#popStatement#", &SemanticActions::popStatement},
			{"#assignmentStatement#", &SemanticActions::assignmentStatement},
			{"#forStatement#", &SemanticActions::forStatement},
			{"#ifelseStatement#", &SemanticActions::ifelseStatement},
			{"#getStatement#", &SemanticActions::getStatement},
			{"#putStatment#", &SemanticActions::putStatment},
			{"#returnStatment#", &SemanticActions::returnStatment},

			
			
	};


