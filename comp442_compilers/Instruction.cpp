#include "stdafx.h"
#include "Instruction.h"


Instruction::Instruction() {
}


Instruction::~Instruction() {
}

void Instruction::setLabel(std::string label) {
	this->label = label;
}

void Instruction::setComment(std::string comment) {
	this->comment = comment;
}

