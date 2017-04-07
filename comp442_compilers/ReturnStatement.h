#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
#include "SymbolTable.h"

class ReturnStatement : public Statement {
public:
	ReturnStatement();
	~ReturnStatement();
	SymbolTable* linkedFunction;
	TypeStruct functionReturnType;
	Expression returnExpression;

	std::string _toMoonCode() override;
};

#endif
