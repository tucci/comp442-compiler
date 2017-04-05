#include "stdafx.h"
#include "PutStatement.h"
#include "Instructions.h"


PutStatement::PutStatement() {
}


PutStatement::~PutStatement() {
}

std::string PutStatement::_toMoonCode() {
	std::string instrBlock;
	
	ExpressionEvalulationInstruction exprInstr(generator, expression);
	instrBlock.append(exprInstr._toMoonCode());
	instrBlock.append(PutCInstruction(exprInstr.outputRegister)._toMoonCode());
	generator->freeRegister(exprInstr.outputRegister);
	return instrBlock;
}
