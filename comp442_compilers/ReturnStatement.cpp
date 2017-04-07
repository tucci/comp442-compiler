#include "stdafx.h"
#include "ReturnStatement.h"
#include "Instructions.h"


ReturnStatement::ReturnStatement() {
}


ReturnStatement::~ReturnStatement() {
}

std::string ReturnStatement::_toMoonCode() {
	std::string instrBlock;
	ExpressionEvalulationInstruction exprInstruction(generator, returnExpression);
	instrBlock.append(exprInstruction._toMoonCode());
	// TODO: find better way to get the name of the _return. this is hardcoded
	// Store the value of the return expression into function's temporary return value memory
	instrBlock.append(StoreWordInstruction(r0, exprInstruction.outputRegister, linkedFunction->resolvedName + "_return").setComment("Store the return value into the return temp memory")._toMoonCode());
	// Since the output of the expression is a register, we need to copy it to our return value register
	instrBlock.append(LoadWordInstruction(r14, r0, linkedFunction->resolvedName + "_return").setComment("Return the value to the r14 return register")._toMoonCode());
	instrBlock.append(JumpRegisterInstruction(r15)._toMoonCode());
	return instrBlock;
}
