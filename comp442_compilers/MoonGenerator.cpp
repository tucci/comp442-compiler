#include "stdafx.h"
#include "MoonGenerator.h"
#include "Instructions.h"


MoonGenerator::MoonGenerator() {
}

MoonGenerator::MoonGenerator(SymbolTable* symbolTable) {
	this->globalTable = symbolTable;
}

MoonGenerator::~MoonGenerator() {
}

void MoonGenerator::addInstruction(std::shared_ptr<Instruction> instruction) {
	instructions.push_back(instruction);
}

void MoonGenerator::generateCode() {
	moonOutputStream.open(outputMoonFile);

	SymbolTable* programTable = globalTable->find("program").first->scope.get();
	createEntriesForTable(*programTable);


	
	
	
	moonOutputStream << AlignInstruction()._toMoonCode() << std::endl;
	moonOutputStream << EntryInstruction()._toMoonCode() << std::endl;;
	// Then generate all the instructions
	/*for (std::shared_ptr<Instruction> instr : instructions) {
		moonOutputStream << instr->_toMoonCode();
	}*/

	moonOutputStream << HaltInstruction()._toMoonCode() << std::endl;
	moonOutputStream.close();
}


std::string MoonGenerator::getMoonFile() {
	return outputMoonFile;
}

void MoonGenerator::setOutputFileName(std::string outputfile) {	
	outputMoonFile = outputfile;
}



void MoonGenerator::createEntriesForTable(SymbolTable& symbolTable) {
	for (std::unordered_map<std::string, SymbolTableRecord>::value_type entry : symbolTable.table) {
		// Create declrations in the moon code for variables or parameters
		if (entry.second.kind == SymbolKind::kind_variable || entry.second.kind == SymbolKind::kind_parameter) {
			DW_Directive dwDirective(globalTable, &entry.second);
			moonOutputStream << dwDirective._toMoonCode() << std::endl;
		}
		// TODO: figure out how to handle function declrations
		
		
	}
}
