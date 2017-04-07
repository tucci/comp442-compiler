#include "stdafx.h"
#include "Instruction.h"


Instruction::Instruction() {
}

Instruction::Instruction(std::string label, std::string comment) {
	this->label = label;
	this->comment = comment;
}

std::string Instruction::_toMoonCode(std::string instr) {
	// This allows the instruction comment and labels to be added
	// All subclasses should call this method to allow for labels and comments to be added to the instr string
	std::string fullInstr = label + "\t" + instr;
	if (!comment.empty()) {
		fullInstr.append("\t% " + comment);
	}
	fullInstr.append("\n");
	return fullInstr;

}


Instruction::~Instruction() {
}

Instruction& Instruction::setLabel(std::string label) {
	this->label = label;
	return *this;

}

Instruction& Instruction::setComment(std::string comment) {
	this->comment = comment;
	return *this;
}

void Instruction::setCodeGenerator(MoonGenerator* generator) {
	this->generator = generator;
}

