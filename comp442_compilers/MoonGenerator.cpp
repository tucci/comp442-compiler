#include "stdafx.h"
#include "MoonGenerator.h"


MoonGenerator::MoonGenerator(){
	
}




MoonGenerator::~MoonGenerator() {
}

void MoonGenerator::addInstruction(std::shared_ptr<Instruction> instruction) {
	instructions.push_back(instruction);
}

void MoonGenerator::generateCode() {
	std::ofstream moonOutputStream;
	moonOutputStream.open(outputMoonFile);
	// TODO: First generate all the allocations from the symbol tables;


	// Then generate all the instructions
	for (std::shared_ptr<Instruction> instr : instructions) {
		moonOutputStream << instr->_toMoonCode();
	}
	moonOutputStream.close();
}


std::string MoonGenerator::getMoonFile() {
	return outputMoonFile;
}

void MoonGenerator::setOutputFileName(std::string outputfile) {	
	outputMoonFile = outputfile;
}
