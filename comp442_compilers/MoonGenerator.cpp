#include "stdafx.h"
#include "MoonGenerator.h"
#include "Instructions.h"


MoonGenerator::MoonGenerator() {
}

MoonGenerator::MoonGenerator(SymbolTable* symbolTable) {
	this->globalTable = symbolTable;
	initRegisterAllocation();
	
}

MoonGenerator::~MoonGenerator() {
	this->globalTable = NULL;
	initRegisterAllocation();
}

void MoonGenerator::addInstruction(std::shared_ptr<Instruction> instruction) {
	instruction->setCodeGenerator(this);
	instructions.push_back(instruction);
}


void MoonGenerator::generateCode() {
	moonOutputStream.open(outputMoonFile);

	SymbolTable* programTable = globalTable->find("program").first->scope.get();
	createEntriesForTable(*programTable);
	
	moonOutputStream << AlignDirective()._toMoonCode();
	moonOutputStream << EntryDirective()._toMoonCode();
	// Then generate all the instructions
	for (std::shared_ptr<Instruction> instr : instructions) {
		moonOutputStream << instr->_toMoonCode();
	}

	moonOutputStream << HaltInstruction()._toMoonCode();
	moonOutputStream.close();
}


std::string MoonGenerator::getMoonFile() {
	return outputMoonFile;
}

void MoonGenerator::setOutputFileName(std::string outputfile) {	
	outputMoonFile = outputfile;
	
}

 Register MoonGenerator::getUnusedRegister() {
	for (std::unordered_map<std::basic_string<char>, bool>::value_type regi : registers) {
		if (regi.second) {// if the register is free, give it
			regi.second = false;// claim the register and set it used
			return stringToRegister(regi.first);	
		}
	}
	// We don't have any more registers to use
	throw std::exception("Moon error: Out of registers to use");

}

void MoonGenerator::freeRegister(Register r) {
	std::unordered_map<std::basic_string<char>, bool>::iterator reg = registers.find(registerToString(r));
	reg->second = true;
}


void MoonGenerator::initRegisterAllocation() {
	// Set all registers to free
	// r0 is not included
	registers.emplace(registerToString(r1), true);
	registers.emplace(registerToString(r2), true);
	registers.emplace(registerToString(r3), true);
	registers.emplace(registerToString(r4), true);
	registers.emplace(registerToString(r5), true);
	registers.emplace(registerToString(r6), true);
	registers.emplace(registerToString(r7), true);
	registers.emplace(registerToString(r8), true);
	registers.emplace(registerToString(r9), true);
	registers.emplace(registerToString(r10), true);
	registers.emplace(registerToString(r11), true);
	registers.emplace(registerToString(r12), true);
	registers.emplace(registerToString(r13), true);
	registers.emplace(registerToString(r14), true);
	registers.emplace(registerToString(r15), true);
}

void MoonGenerator::createEntriesForTable(SymbolTable& symbolTable) {
	for (std::unordered_map<std::string, SymbolTableRecord>::value_type entry : symbolTable.table) {
		// Create declrations in the moon code for variables or parameters
		if (entry.second.kind == SymbolKind::kind_variable || entry.second.kind == SymbolKind::kind_parameter) {
			DefineWordDirective dwDirective(globalTable, &entry.second);
			moonOutputStream << dwDirective._toMoonCode() << std::endl;
		}
		// TODO: figure out how to handle function declrations
		
		
	}
}
