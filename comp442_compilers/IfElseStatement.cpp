#include "stdafx.h"
#include "IfElseStatement.h"
#include "Instructions.h"

IfElseStatement::IfElseStatement() {
};
IfElseStatement::~IfElseStatement() {}

std::string IfElseStatement::_toMoonCode() {

	std::string instrBlock;

	std::string unqLabel = uniqueLabel();

	std::string elseLabel = "else_" + unqLabel;
	std::string endifLabel = "endif_" + unqLabel;

	Register r = generator->getUnusedRegister();
	// Creates all the sub expressions and outputs a temp memory
	ExpressionEvalulationInstruction exprInstructions(generator, condition);
	instrBlock.append(exprInstructions._toMoonCode());
	
	instrBlock.append(LoadWordInstruction(r, r0, exprInstructions.outputTemp.label)._toMoonCode());
	instrBlock.append(BranchIfZeroInstruction(r, elseLabel)._toMoonCode());
	// if code statments
	instrBlock.append(ifStatements._toMoonCode());
	// Jump to end of if/else
	instrBlock.append(JumpInstruction(endifLabel)._toMoonCode());
	instrBlock.append(NoopInstruction().setLabel(elseLabel)._toMoonCode());
	// else code statments
	instrBlock.append(elseStatements._toMoonCode());
	instrBlock.append(NoopInstruction().setLabel(endifLabel)._toMoonCode());
	instrBlock.append(ClearRegisterInstruction(r)._toMoonCode());
	generator->freeRegister(r);
	// Free the expressions output temp
	generator->freeTempMemory(exprInstructions.outputTemp);


	return instrBlock;
};
