#include "stdafx.h"
#include "GetStatement.h"
#include "Instructions.h"


GetStatement::GetStatement() {
	
}
GetStatement::~GetStatement() {
	
}

std::string GetStatement::_toMoonCode() {
	Register r = generator->getUnusedRegister();
	
	std::string instrBlock;
	instrBlock.append(GetCInstruction(r)._toMoonCode()); // Read the char from the keyboard
	instrBlock.append(StoreWordInstruction(r0, r, var.record->label)._toMoonCode());// Store the value of that register into the var
	instrBlock.append(ClearRegisterInstruction(r)._toMoonCode());// clear the register
	generator->freeRegister(r);
	
	return instrBlock;
}


