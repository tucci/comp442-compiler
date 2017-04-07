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

	
	
	initializer.setCodeGenerator(generator);
	// Set the loop initializer assignment before the loop starts
	instrBlock.append(initializer.setComment("Reset loop assignment")._toMoonCode());
	instrBlock.append(NoopInstruction().setLabel(goWhile).setComment("Loop header")._toMoonCode());
	// Creates all the sub expressions and outputs a temp memory
	ExpressionEvalulationInstruction exprInstructions(generator, condition);

	instrBlock.append(exprInstructions._toMoonCode());
	instrBlock.append(BranchIfZeroInstruction(exprInstructions.outputRegister, endWhile)._toMoonCode());
	// for block statement
	instrBlock.append(statements._toMoonCode());
	// Increment loop
	instrBlock.append(incrementer.setComment("Increment loop")._toMoonCode());
	// Jump to start of while block
	instrBlock.append(JumpInstruction(goWhile)._toMoonCode());// loops here
	instrBlock.append(NoopInstruction().setLabel(endWhile)._toMoonCode());

;


	// End of for code
	// Clear register values
	instrBlock.append(ClearRegisterInstruction(exprInstructions.outputRegister)._toMoonCode());
	generator->freeRegister(exprInstructions.outputRegister);;


	return instrBlock;
	
}
