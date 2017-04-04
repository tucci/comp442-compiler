#include "stdafx.h"
#include "MoonGenerator.h"
#include "Instructions.h"


MoonGenerator::MoonGenerator() {
}

MoonGenerator::MoonGenerator(SymbolTable* symbolTable, int tempMemorySize) {
	this->globalTable = symbolTable;
	initRegisterAllocation();
	initTempMemoryAllocation(tempMemorySize);
	
}

MoonGenerator::~MoonGenerator() {
	this->globalTable = NULL;
	initRegisterAllocation();
}

void MoonGenerator::addInstruction(std::shared_ptr<Instruction> instruction) {
	instructions.push_back(instruction);
}


void MoonGenerator::generateCode() {
	moonOutputStream.open(outputMoonFile);

	moonOutputStream << CommentInstruction("Allocate temp memory")._toMoonCode();
	// Create the temp memory
	for (std::unordered_map<std::string, bool>::value_type temp : tempMemory) {
		moonOutputStream << DefineWordDirective(temp.first)._toMoonCode();
	}
	moonOutputStream << CommentInstruction("Allocate user data")._toMoonCode();
	SymbolTable* programTable = globalTable->find("program").first->scope.get();
	createEntriesForTable(*programTable);
	

	moonOutputStream << CommentInstruction("Start of user program")._toMoonCode();
	moonOutputStream << AlignDirective()._toMoonCode();
	moonOutputStream << EntryDirective()._toMoonCode();
	// Then generate all the instructions
	for (std::shared_ptr<Instruction> instr : instructions) {
		moonOutputStream << instr->_toMoonCode();
	}

	moonOutputStream << HaltInstruction()._toMoonCode();
	moonOutputStream << CommentInstruction("end of program")._toMoonCode();
	moonOutputStream.close();
}


std::string MoonGenerator::getMoonFile() {
	return outputMoonFile;
}

void MoonGenerator::setOutputFileName(std::string outputfile) {	
	outputMoonFile = outputfile;
}

 Register MoonGenerator::getUnusedRegister() {
	for (std::unordered_map<std::basic_string<char>, bool>::value_type& regi : registers) {
		if (regi.second) {// if the register is free, give it
			regi.second = false;// claim the register and set it used
			return stringToRegister(regi.first);	
		}
	}
	// We don't have any more registers to use
	throw std::exception("Moon error: Out of registers to use");

}

void MoonGenerator::freeRegister(Register r) {
	std::unordered_map<std::basic_string<char>, bool>::iterator& reg = registers.find(registerToString(r));
	reg->second = true;
}

TempMemory MoonGenerator::getTempMemory() {
	for (std::unordered_map<std::basic_string<char>, bool>::value_type& ti : tempMemory) {
		if (ti.second) {// if the register is free, give it
			ti.second = false;// claim the register and set it used
			TempMemory tn;
			tn.label = ti.first;
			return tn;
		}
	}
	// We don't have any more temp memory to use
	throw std::exception("Moon error: Out of temp memory to use");
}

void MoonGenerator::freeTempMemory(TempMemory tn) {
	std::unordered_map<std::basic_string<char>, bool>::iterator& tni = tempMemory.find(tn.label);
	tni->second = true;
}


void MoonGenerator::initTempMemoryAllocation(int size) {
	for (int i = 0; i < size; ++i) {
		std::string tn = "t" + std::to_string(i);
		tempMemory.emplace(tn, true);
	}
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
			moonOutputStream << dwDirective._toMoonCode();
		}
		// TODO: figure out how to handle function declrations
		
		
	}
}
