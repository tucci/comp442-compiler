#include "stdafx.h"
#include "ForStatement.h"
#include "Instructions.h"

ForStatement::ForStatement() {
}


ForStatement::~ForStatement() {
}

std::string ForStatement::_toMoonCode() {
	std::string instrBlock;
	std::string unqLabel = uniqueLabel();
	// A for loop internally gets turned into a while loop
	std::string goWhile = "gowhile_" + unqLabel;
	std::string endWhile = "endwhile_" + unqLabel;

	Register r = generator->getUnusedRegister();

	instrBlock.append(NoopInstruction().setLabel(goWhile).setComment("Loop header")._toMoonCode());
	// Creates all the sub expressions and outputs a temp memory
	ExpressionEvalulationInstruction exprInstructions(generator, condition);

	instrBlock.append(exprInstructions._toMoonCode());

	instrBlock.append(LoadWordInstruction(r, r0, exprInstructions.outputTemp.label)._toMoonCode());
	instrBlock.append(BranchIfZeroInstruction(r, endWhile)._toMoonCode());
	// for block statement
	instrBlock.append(statements._toMoonCode());
	// Increment loop
	instrBlock.append(incrementer.setComment("Increment loop")._toMoonCode());
	// Jump to start of while block
	instrBlock.append(JumpInstruction(goWhile)._toMoonCode());// loops here
	instrBlock.append(NoopInstruction().setLabel(endWhile)._toMoonCode());
	// End of for code
	// Clear register values
	instrBlock.append(ClearRegisterInstruction(r)._toMoonCode());
	generator->freeRegister(r);
	// Free the expressions output temp
	generator->freeTempMemory(exprInstructions.outputTemp);


	return instrBlock;
	
}
