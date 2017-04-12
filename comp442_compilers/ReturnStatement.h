#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
#include "SymbolTable.h"

// an intermediate representation for a return statement
class ReturnStatement : public Statement {
public:
	ReturnStatement();
	~ReturnStatement();
	// the return statment this function belongs to
	SymbolTable* linkedFunction;
	// the return type for this function
	TypeStruct functionReturnType;
	// the expression for this return
	Expression returnExpression;

	std::string _toMoonCode() override;
};

#endif
