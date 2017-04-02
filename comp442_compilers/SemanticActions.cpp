#include "stdafx.h"
#include "SemanticActions.h"
#include "SymbolTableRecord.h"

// A simple data structure that keeps some context during the semantic actions
static struct SemanticContext {
	bool inParam = false;
	bool inPhase2 = false;
	bool skipClass = false;
	bool skipFunction = false;
	bool returnedValued = false;
	bool inForInitAssignStat = false;
	bool inForIncrementAssignStat = false;
	bool inForRelExpression = false;
} context;


void SemanticActions::performAction(const SemanticSymbol& symbol,
                                    std::vector<SymbolTableRecord>& semanticStack,
                                    SymbolTable& globalTable,
                                    SymbolTable** currentTable,
                                    const Token& token,
                                    bool phase2,
									std::vector<SemanticError>& semanticErrors,
									bool* parserError) {

	context.inPhase2 = phase2;
	if (!_shouldSkip(symbol)) {
		if (semanticStack.empty()) {
			semanticStack.push_back(SymbolTableRecord());
		}
		SymbolTableRecord& top = semanticStack.back();
		SemanticActionContainer container = {symbol, semanticStack, globalTable, top, currentTable, token, semanticErrors, parserError};

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
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable, true);
		_goToScope(container, &container.top);
	}
	else {
		if (_isRedefined(*found.first, container.top)) {
			context.skipClass = true;
			if (context.inPhase2) {
				_reportError(container, "Class " + container.top.name + " redeclared");
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
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable, true);
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
	if (!context.inPhase2) {
		container.top.kind = SymbolKind::kind_variable;
		std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);

		if (!found.second && !context.inPhase2) {
			(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable, false);
		} else {
			if (_isRedefined(*found.first, container.top)) {
				if (context.inPhase2) {
					_reportError(container, "Variable " + container.top.name + " redeclared");
				}
			}
		}

		container.semanticStack.pop_back();
	}
}

void SemanticActions::addFuncDefParameter(SemanticActionContainer& container) {
	// Push a new param to the function data parameters
	container.top.functionData.parameters.push_back(std::pair<TypeStruct, std::string>());
	context.inParam = true;
}

void SemanticActions::createFuncEntryAndTable(SemanticActionContainer& container) {
	context.inParam = false;
	std::pair<SymbolTableRecord*, bool> found = (*container.currentTable)->find(container.top.name);
	if (!found.second && !context.inPhase2) {
		container.top = *(*container.currentTable)->addRecord(container.top.name, container.top, *container.currentTable, true);
		// Pop this record of the semantic stack and it to our table entry
		_goToScope(container, &container.top);
		// We added the record to the symbol table, but now we have to create the child table/scope and add all the parameter entries to it
		for (std::pair<TypeStruct, std::string>& param : container.top.functionData.parameters) {
			SymbolTableRecord paramRecord;
			paramRecord.kind = SymbolKind::kind_parameter;
			paramRecord.name = param.second;
			paramRecord.typeStructure = param.first;
			(*container.currentTable)->addRecord(paramRecord.name, paramRecord, *container.currentTable, false);
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
			_reportError(container, "Function " + container.top.name + " redeclared");
		}
	}


}

void SemanticActions::checkTypeGlobal(SemanticActionContainer& container) {
	if (context.inPhase2) {
		std::string typeName = container.token.lexeme;
		std::pair<SymbolTableRecord*, bool> found = container.globalTable.find(typeName);
		if (!found.second) {
			// We have a type being used that is not found in the global table
			_reportError(container, "Type " + typeName + " not defined");
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
			_reportError(container, "Circular dependency of " + className);
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
					_reportError(container, "Parameter " + container.token.lexeme + " redeclared");
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

void SemanticActions::checkIfReturns(SemanticActionContainer& container) {
	if (context.inPhase2) {
		std::string functionName = (*container.currentTable)->name;
		// Skip the program function. It doenst return a value
		if (!context.returnedValued && functionName != "program") {
			_reportError(container, "Function " + functionName + " doesn't return a value");
			
		}
		// Reset value
		context.returnedValued = false;
		
	}
}


void SemanticActions::addNumericExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addNumeric(container.token.lexeme);
	}
}



void SemanticActions::addSignExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.setSign(container.token.lexeme);
	}
}

void SemanticActions::operatorExprFragment(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addOperator(container.token.lexeme);
	}

}

void SemanticActions::addLeftParen(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addParen("(");
	}
}

void SemanticActions::addRightParen(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.expr.addParen(")");
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
		container.top.attr.expr.buildExpressionTree();

		std::pair<bool, TypeStruct> valueType = container.top.attr.expr.typeCheckExpression();
		if (valueType.second.type == type_none || !valueType.first) {
			_reportError(container, "Expression " + container.top.attr.expr.toFullName() + " has different types");
			container.top.attr.expr.type = {};
		} else {
			container.top.attr.expr.type = valueType.second;
		}
		

		SymbolTableRecord popped = container.top;
		container.semanticStack.pop_back();
		SymbolTableRecord& top = container.semanticStack.back();
		switch (top.attr.type) {

			// Migrate attribute
		case attr_expr: {
			top.attr.expr = popped.attr.expr;
			break;
		} 
		case attr_statement: {
			if (top.attr.statementData.statType == StatementType::assignStat) {
				top.attr.statementData.assignStatement.rhs = popped.attr.expr;
			}
			if (top.attr.statementData.statType == StatementType::putStat) {
				top.attr.statementData.putStatement.expression = popped.attr.expr;
			}
			if (top.attr.statementData.statType == StatementType::returnStat) {
				top.attr.statementData.returnStatement.returnExpression = popped.attr.expr;
			}
			if (top.attr.statementData.statType == StatementType::forStat) {
				if (context.inForInitAssignStat) {
					top.attr.statementData.forStatement.initializer.rhs = popped.attr.expr;
				}
				if (context.inForRelExpression) {
					top.attr.statementData.forStatement.condition = popped.attr.expr;
					context.inForRelExpression = false;
				}

				
			}
			if (top.attr.statementData.statType == StatementType::ifelseStat) {
				top.attr.statementData.ifelseStatement.condition = popped.attr.expr;
			}
			break;
		}
		case attr_var: {
			if (top.attr.var.isFunc) {
				// Set this expression to the last arugment for this function
				top.attr.var.arguments.push_back(popped.attr.expr.type);
			} else {
				// Add this expression to the last indice for this var
				top.attr.var.vars.back().indices.push_back(popped.attr.expr);
			}

			
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

		Variable& var = container.top.attr.var;

		// Get the var type
		VariableFragment varFragment;
		SymbolTable* currentScope = (*container.currentTable);
		SymbolTableRecord* record = NULL;
		std::string currentIdentifier;

		for (int i = 0; i < var.vars.size(); ++i) {
			varFragment = var.vars[i];
			currentIdentifier = varFragment.identifier;
			// Finds in current scope, or parent scope
			std::pair<SymbolTableRecord*, bool> found;
			if (i == 0) {
				found = currentScope->findInParents(currentIdentifier);
			} else {
				found = currentScope->find(currentIdentifier);
			}

			if (found.second) {
				record = found.first;
				// This is a object
				if (record->typeStructure.type == SymbolType::type_class) {
					// We need to nest down the object structure
					// Get the symbol table for this class type
					std::string className = record->typeStructure.className;
					currentScope = container.globalTable.find(className).first->scope.get();
				}
				if (var.isFunc) {
					var.varType = record->functionData.returnType;
				} else {
					var.varType = record->typeStructure;
				}
				
				
			}
		}
		// link the var to it's record;
		var.record = record;

		SymbolTableRecord popped = container.top;
		
		if (popped.attr.var.isFunc) {
			bool error = false;
			if (record == NULL) {
				error = true;
			} else {
				// Check to see if this is a function call. If it is, we need to check the argument types and values
				if (popped.attr.var.arguments.size() == record->functionData.parameters.size()) {
					// Compare each function type
					for (int i = 0; i < popped.attr.var.arguments.size(); ++i) {
						if (!(popped.attr.var.arguments[i] == record->functionData.parameters[i].first)) {
							// There was a type mismatch for the type at param/arg index i
							error = true;
						}
					}

				} else {
					error = true; // Arugment length is not the same as function definition
				}
			}
			

			if (error) {
				_reportError(container, "Function call " + popped.attr.var.toFullName() + " has mismatich in arugment length or types");
			}
			
		}

		
		container.semanticStack.pop_back();
		SymbolTableRecord& top = container.semanticStack.back();
		switch (top.attr.type) {
		case attr_expr: {
			top.attr.expr.addVar(popped.attr.var);
			break;
		}
		case attr_statement: {
			switch (top.attr.statementData.statType) {
			case assignStat: top.attr.statementData.assignStatement.var = popped.attr.var; break;
			case forStat: break;
			case ifelseStat: break;
			case getStat: top.attr.statementData.getStatement.var = popped.attr.var; break;
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

void SemanticActions::startFuncCall(SemanticActionContainer& container) {
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
		SymbolTableRecord popped = container.top;
		container.semanticStack.pop_back();
		SymbolTableRecord& top = container.semanticStack.back();
		if (top.attr.statementData.statType == StatementType::forStat) {
			if (context.inForIncrementAssignStat) {
				top.attr.statementData.forStatement.incrementer = popped.attr.statementData.assignStatement;
			} else {
				// Add this statement to the statement block for this for loop
				top.attr.statementData.forStatement.statements.statements.push_back(popped.attr.statementData);
			}
			
		}
		if (top.attr.statementData.statType == StatementType::ifelseStat) {
			// Add this statement to the statement block for this if condition
			if (!top.attr.statementData.ifelseStatement.inElseBlock) {
				top.attr.statementData.ifelseStatement.ifStatements.statements.push_back(popped.attr.statementData);
			} else {
				top.attr.statementData.ifelseStatement.elseStatements.statements.push_back(popped.attr.statementData);
			}
			
		}
		
	
	}
}

void SemanticActions::assignmentStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::assignStat;
	}
}

void SemanticActions::forStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::forStat;
	}
}

void SemanticActions::forInitStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		context.inForInitAssignStat = true;
	}
}

void SemanticActions::forAddVar(SemanticActionContainer& container) {
	if (context.inPhase2) {
		std::string varname = container.token.lexeme;
		Variable var;
		VariableFragment fragment;
		fragment.identifier = varname;
		var.vars.push_back(fragment);
		var.location = container.token.tokenIndex;
		// link the record of this var from the symbol table to the var
		var.record = (*container.currentTable)->find(varname).first;
		container.top.attr.statementData.forStatement.initializer.var = var;
		
	}
}

void SemanticActions::forInitStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {
		context.inForInitAssignStat = false;
	}
}

void SemanticActions::forIncrementStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		context.inForIncrementAssignStat= true;
	}
}

void SemanticActions::forIncrementStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {
		context.inForIncrementAssignStat = false;
	}
}

void SemanticActions::forRelExpr(SemanticActionContainer& container) {
	if (context.inPhase2) {
		context.inForRelExpression = true;
	}
}

void SemanticActions::ifelseStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::ifelseStat;
	}
}

void SemanticActions::setInElseBlock(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.ifelseStatement.inElseBlock = true;
	}
}

void SemanticActions::getStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::getStat;
	}
}

void SemanticActions::putStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::putStat;
	}
}

void SemanticActions::returnStatementStart(SemanticActionContainer& container) {
	if (context.inPhase2) {
		container.top.attr.statementData.statType = StatementType::returnStat;
		// Get current function return type
		container.top.attr.statementData.returnStatement.functionReturnType = (*container.currentTable)->parent->find((*container.currentTable)->name).first->functionData.returnType;
		
	}
}

void SemanticActions::assignmentStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {
	
		// Type check assignment statement
		AssignStatement assignStatement = container.top.attr.statementData.assignStatement;
		if (!(assignStatement.var.varType == assignStatement.rhs.type)) {
			_reportError(container, "Assignment Statement " + assignStatement.var.toFullName() + " = " + assignStatement.rhs.toFullName() + " has type mismatch");
		}
		
	}
}

void SemanticActions::forStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {

	}
}

void SemanticActions::ifelseStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {

	}
}

void SemanticActions::getStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {

	}
}

void SemanticActions::putStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {

	}
}

void SemanticActions::returnStatementEnd(SemanticActionContainer& container) {
	if (context.inPhase2) {
		ReturnStatement returnStatement = container.top.attr.statementData.returnStatement;
		if (!(returnStatement.functionReturnType == returnStatement.returnExpression.type)) {
			_reportError(container, "Return type " + returnStatement.returnExpression.type.toString() + " mismatches function return type " + returnStatement.functionReturnType.toString());
		}
		context.returnedValued = true;
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


		for (std::vector<SymbolTable*>::value_type d : possibleDependencies) {
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
		// Finds in current scope, or parent scope
		std::pair<SymbolTableRecord*, bool> found;
		if (i == 0) {
			found = currentScope->findInParents(currentIdentifier);
		} else {
			found = currentScope->find(currentIdentifier);
		}
		SymbolTableRecord* varDefinition = found.first;

		if (!found.second) {
			// Not found, this variable is undefined
			_reportError(container, "Identifier " + var.toFullName() + " is undefined");
		} else {


			if (varDefinition->kind == SymbolKind::kind_function && !var.isFunc) {
				_reportError(container, "Identifier " + var.toFullName() + " is not a function");
			} else if (varDefinition->kind != SymbolKind::kind_function && var.isFunc) {
				// The definition is not a function, but it is being used as a function
				_reportError(container, "Identifier " + var.toFullName() + " is not a function");
			}

			// If the var is being used before it has been declared
			// If it is a function, then we can ignore it's location
			if (varDefinition->kind != SymbolKind::kind_function && i == 0 && varDefinition->definedLocation > var.location) {
				_reportError(container, "Identifier " + var.toFullName() + " is used before it is defined");
			}


			switch (varDefinition->typeStructure.structure) {
			case struct_simple: {
				// trying to index a variable, but the variable is not an array
				if (varFragment.indices.size() != 0) {
					_reportError(container, "Identifier " + var.toFullName() + " is not an array");
				}
				break;
			}
			case struct_array: {
				//  Trying to use a array without indexing into it
				if (varFragment.indices.size() == 0) {
					_reportError(container, "Identifier " + var.toFullName() + " is not an array");
				} else {
					// Compare dimension length 
					// The dimensions are different
					if (varDefinition->typeStructure.dimensions.size() != varFragment.indices.size()) {
						_reportError(container, "Array " + var.toFullName() + " has wrong dimensions");
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
					_reportError(container, "Identifier " + var.toFullName() + " is not an object");
					return; // Early return
				}
			}

		}
	}

}

void SemanticActions::_reportError(SemanticActionContainer& container, std::string message) {
	// If an error was reported than there is a parser error
	// This will get changed in the parser and will tell the compiler to not generate any code due to errors
	*container.parserError = true;
	SemanticError error;
	error.tokenLine = container.token.tokenLine;
	error.message = message + " on line " + std::to_string(error.tokenLine);
	container.semanticErrors.push_back(error);

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
			{"#addFuncDefParameter#", &SemanticActions::addFuncDefParameter},
			{"#createFuncEntryAndTable#", &SemanticActions::createFuncEntryAndTable},
			{"#endFuncEntryAndTable#", &SemanticActions::endFuncEntryAndTable},
			{"#startFuncDef#", &SemanticActions::startFuncDef},
			{"#checkTypeGlobal#", &SemanticActions::checkTypeGlobal},
			{"#checkCircular#", &SemanticActions::checkCircular},
			{ "#storeId#", &SemanticActions::storeId },
			{ "#storeType#", &SemanticActions::storeType },
			{ "#storeArraySize#", &SemanticActions::storeArraySize },
			{ "#checkIfReturns#", &SemanticActions::checkIfReturns },
			// Expression building
			{"#addNumericExprFragment#", &SemanticActions::addNumericExprFragment},
			{"#operatorExprFragment#", &SemanticActions::operatorExprFragment},
			{"#addSignExprFragment#", &SemanticActions::addSignExprFragment},
			{ "#addLeftParen#", &SemanticActions::addLeftParen },
			{ "#addRightParen#", &SemanticActions::addRightParen },

			{"#pushExpr#", &SemanticActions::pushExpr},
			{"#popExpr#", &SemanticActions::popExpr},
			// Var building
			{ "#pushVar#", &SemanticActions::pushVar},
			{ "#popVar#", &SemanticActions::popVar},
			{ "#addToVar#", &SemanticActions::addToVar},
			{ "#startFuncCall#", &SemanticActions::startFuncCall },
			// Statement building
			{ "#pushStatement#", &SemanticActions::pushStatement },
			{ "#popStatement#", &SemanticActions::popStatement},
			{ "#assignmentStatementStart#", &SemanticActions::assignmentStatementStart},
			{ "#forStatementStart#", &SemanticActions::forStatementStart},
			{ "#forAddVar#", &SemanticActions::forAddVar },
			{ "#forInitStatementStart#", &SemanticActions::forInitStatementStart },
			{ "#forInitStatementEnd#", &SemanticActions::forInitStatementEnd },
			{ "#forIncrementStatementStart#", &SemanticActions::forIncrementStatementStart },
			{ "#forIncrementStatementEnd#", &SemanticActions::forIncrementStatementEnd },
			{ "#forRelExpr#", &SemanticActions::forRelExpr },
			

			{ "#ifelseStatementStart#", &SemanticActions::ifelseStatementStart},
			{ "#getStatementStart#", &SemanticActions::getStatementStart},
			{ "#putStatementStart#", &SemanticActions::putStatementStart},
			{ "#returnStatementStart#", &SemanticActions::returnStatementStart},

			{ "#assignmentStatementEnd#", &SemanticActions::assignmentStatementEnd },
			{ "#forStatementEnd#", &SemanticActions::forStatementEnd },
			{ "#ifelseStatementEnd#", &SemanticActions::ifelseStatementEnd },
			{ "#setInElseBlock#", &SemanticActions::setInElseBlock },
			{ "#getStatementEnd#", &SemanticActions::getStatementEnd },
			{ "#putStatementEnd#", &SemanticActions::putStatementEnd },
			{ "#returnStatementEnd#", &SemanticActions::returnStatementEnd },

		

			
			
	};


