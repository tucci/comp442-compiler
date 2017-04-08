#include "stdafx.h"
#include "AssignStatement.h"
#include "Instructions.h"


AssignStatement::AssignStatement() {
}



AssignStatement::~AssignStatement() {
}

std::string AssignStatement::_toMoonCode() {
	std::string instrs;
	ExpressionElementNode* exprRoot = rhs.root.get();
	// expression that needs to be evaluated	
	ExpressionEvalulationInstruction exprInstr(generator, rhs);
	instrs.append(exprInstr._toMoonCode());
	instrs.append(StoreWordInstruction(r0, exprInstr.outputRegister, var.record->label).setComment(var.record->name + " assignment operation")._toMoonCode());
	generator->freeRegister(exprInstr.outputRegister);

	return instrs;

}
