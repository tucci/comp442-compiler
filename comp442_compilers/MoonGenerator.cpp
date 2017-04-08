#include "stdafx.h"
#include "MoonGenerator.h"
#include "Instructions.h"


MoonGenerator::MoonGenerator() : MoonGenerator(NULL, 32) {
}

MoonGenerator::MoonGenerator(SymbolTable* symbolTable, int tempMemorySize) {
	// sets the global symbol tables from the given symbol table
	this->globalTable = symbolTable;
	// initializes all the usable registers sets them to free
	initRegisterAllocation();
	// initializes the temporary memory with the given tempMemorySize
	initTempMemoryAllocation(tempMemorySize);
	
}

MoonGenerator::~MoonGenerator() {
	this->globalTable = NULL;
}


void MoonGenerator::addInstruction(std::shared_ptr<Instruction> instruction) {
	// Add the given instruction to the instruction list
	instructions.push_back(instruction);
}


void MoonGenerator::generateCode() {
	// Starts creating the output moon file
	moonOutputStream.open(outputMoonFile);
	// First we will allocate all the temp memory
	moonOutputStream << CommentInstruction("Allocate temp memory")._toMoonCode();
	for (std::unordered_map<std::string, bool>::value_type temp : tempMemory) {
		moonOutputStream << DefineWordDirective(temp.first)._toMoonCode();
	}
	// Then we will allocate all the memory for the program variables/params/objects/arrays
	moonOutputStream << CommentInstruction("Allocate function var/param temp data")._toMoonCode();
	for (std::_Simple_types<std::pair<const std::basic_string<char>, SymbolTableRecord>>::value_type table : globalTable->getTable()) {
		if (table.second.kind == kind_class || table.second.kind == kind_function) {
			createEntriesForRecord(table.second);
		}
	}

	// Then Create function definitions/statements
	moonOutputStream << CommentInstruction("Create function definitions")._toMoonCode();
	for (std::vector<FunctionDeclStatementList>::value_type funcDef : functionDeclarations) {
		// Create the function declrations
		generateCodeForFunction(funcDef);
	}

	
	// Then generate the actual user program
	moonOutputStream << CommentInstruction("Start of user program")._toMoonCode();
	moonOutputStream << AlignDirective()._toMoonCode();
	moonOutputStream << EntryDirective()._toMoonCode();
	
	for (std::shared_ptr<Instruction> instr : instructions) {
		// output each instruction to the file sequentially
		moonOutputStream << instr->_toMoonCode();
	}

	moonOutputStream << HaltInstruction()._toMoonCode();
	moonOutputStream << CommentInstruction("end of program")._toMoonCode();
	// End of program
	moonOutputStream.close();
	// close the file
}


std::string MoonGenerator::getMoonFile() {
	return outputMoonFile;
}

void MoonGenerator::setOutputFileName(std::string outputfile) {	
	outputMoonFile = outputfile;
}

 Register MoonGenerator::getUnusedRegister() {
	// Loop over our availble registers to use
	for (std::unordered_map<std::basic_string<char>, bool>::value_type& regi : registers) {
		if (regi.second) {
			// if the register is free, give it
			// claim the register and set it used
			regi.second = false;
			return stringToRegister(regi.first);	
		}
	}
	// We don't have any more registers to use
	throw std::exception("Moon error: Out of registers to use");

}

void MoonGenerator::freeRegister(Register r) {
	// Find the register
	std::unordered_map<std::basic_string<char>, bool>::iterator& reg = registers.find(registerToString(r));
	// and free it.
	reg->second = true;
}

TempMemory MoonGenerator::getTempMemory() {
	for (std::unordered_map<std::basic_string<char>, bool>::value_type& ti : tempMemory) {
		if (ti.second) {
			// if the temp memoery is free, give it
			// claim the temp and set it used
			ti.second = false;
			TempMemory tn;
			tn.label = ti.first;
			return tn;
		}
	}
	// We don't have any more temp memory to use
	throw std::exception("Moon error: Out of temp memory to use");
}

void MoonGenerator::freeTempMemory(TempMemory tn) {
	// find the temp memory
	std::unordered_map<std::basic_string<char>, bool>::iterator& tni = tempMemory.find(tn.label);
	// and free it
	tni->second = true;
}

void MoonGenerator::addFuncDeclStatmentList(FunctionDeclStatementList funcDec) {
	// add this func dec to the list
	functionDeclarations.push_back(funcDec);
}


void MoonGenerator::initTempMemoryAllocation(int size) {
	// Init a size amount of temp memory
	for (int i = 0; i < size; ++i) {
		// Memory name range from t1,t2,t3.... tsize
		std::string tn = "t" + std::to_string(i);
		tempMemory.emplace(tn, true);
	}
}


void MoonGenerator::initRegisterAllocation() {
	// r0 is not a usable register, it is used for the value 0
	// Set all usable registers to free
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
	// r14 is not a usable register, it is used for function return value
	// r15 is not a usable register, it is used for function linking
}

void MoonGenerator::createEntriesForRecord(SymbolTableRecord& record) {

	// If this record is a class, then we dont want to create the variables for this
	// we onlt have to create the function declrations
	if (record.kind == kind_class) {
		// Loop over the records in this record's table
		for (std::_Simple_types<std::pair<const std::basic_string<char>, SymbolTableRecord>>::value_type entry : record.scope->getTable()) {
			if (entry.second.kind ==  kind_function) {
				// Create the inner function entries
				createEntriesForRecord(entry.second);
			}
		}
	}
	// If this record is a function, then we want to create all the inner variables/params/objects
	else if (record.kind == kind_function) {
		// create the variables
		for (std::_Simple_types<std::pair<const std::basic_string<char>, SymbolTableRecord>>::value_type entry : record.scope->getTable()) {
			// Create dws in the moon code for variables or parameters
			if (entry.second.kind == SymbolKind::kind_variable || entry.second.kind == SymbolKind::kind_parameter) {
				// generate the dw directive
				DefineWordDirective dwDirective(globalTable, &entry.second);
				moonOutputStream << dwDirective._toMoonCode();
			}
		}
		// Create the return value memory for this function
		// however do not create it for the program function
		if (record.name != "program") {
			// Add the return value memory
			DefineWordDirective dwDirective(record.label + "_return");
			moonOutputStream << dwDirective._toMoonCode();
		}
		
	}
}

void MoonGenerator::generateCodeForFunction(FunctionDeclStatementList funcStatements) {
	// Get the function record and table
	SymbolTableRecord* functionRecord = funcStatements.functionRecord;
	SymbolTable* functionTable = functionRecord->scope.get();
	// Add the function header instruction using a noop instruction
	moonOutputStream << NoopInstruction(functionRecord->label, "")._toMoonCode();
	// generate all the inner statements
	for (std::vector<std::shared_ptr<Statement>>::value_type statement : funcStatements.statements) {
		moonOutputStream << statement->_toMoonCode();
	}
}
