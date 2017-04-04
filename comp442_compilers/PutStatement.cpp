#include "stdafx.h"
#include "PutStatement.h"
#include "Instructions.h"


PutStatement::PutStatement() {
}


PutStatement::~PutStatement() {
}

std::string PutStatement::_toMoonCode() {
	std::string instrBlock;
	Register r = generator->getUnusedRegister();

	// TODO: evaluate the expression and put it in a register
	std::string expressionValue = "66";
	
	instrBlock.append(ClearRegisterInstruction(r)._toMoonCode());
	instrBlock.append(AddImmediateInstruction(r, r, expressionValue)._toMoonCode());
	instrBlock.append(PutCInstruction(r)._toMoonCode());
	instrBlock.append(ClearRegisterInstruction(r)._toMoonCode());
	generator->freeRegister(r);
	return instrBlock;
}
