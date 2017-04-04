#include "stdafx.h"
#include "IfElseStatement.h"
#include "Instructions.h"

IfElseStatement::IfElseStatement() {
};
IfElseStatement::~IfElseStatement() {}

std::string IfElseStatement::_toMoonCode() {

	std::string instrBlock;

	
	std::string elseLabel = "else_" + uniqueLabel();
	std::string endifLabel = "endif_" + uniqueLabel();

	// TODO implement comparision
	Register r = generator->getUnusedRegister();
	TempMemory tn = generator->getTempMemory();

	instrBlock.append(LoadWordInstruction(r1, r0, tn.label)._toMoonCode());
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
	generator->freeTempMemory(tn);

	return instrBlock;
};
