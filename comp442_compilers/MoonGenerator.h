#ifndef MOON_GENERATOR_H
#define MOON_GENERATOR_H

struct TempMemory {
	std::string label;
};


class MoonGenerator {
public:
	MoonGenerator(SymbolTable* symbolTable, int tempMemorySize = 32);
	MoonGenerator();
	~MoonGenerator();
	void addInstruction(std::shared_ptr<Instruction> instruction);
	void generateCode();
	std::string getMoonFile();
	void setOutputFileName(std::string outputfile);
	// Claim/free register data
	Register getUnusedRegister();
	void freeRegister(Register r);
	// Claim/free temo memory
	TempMemory getTempMemory();
	void freeTempMemory(TempMemory tn);

private:
	std::ofstream moonOutputStream;
	SymbolTable* globalTable;
	std::vector<std::shared_ptr<Instruction>> instructions;
	std::string outputMoonFile;
	// map of registers being used or free
	// Where second value means if the register is free or not
	// if true, that means the register is free to use
	// Does not include r0
	std::unordered_map<std::string, bool> registers;
	// Map of temp memory being used
	// Where second value means if the temp memory is free or not
	// if true, that means the temp memory is free to use
	std::unordered_map<std::string, bool> tempMemory;
	
	// Inits the register allocations. sets all registers to free
	void initRegisterAllocation();
	// Inits the temp memory allocation. sets all temp to free
	// Used when the program needs temp memory
	void initTempMemoryAllocation(int size);
	void createEntriesForTable(SymbolTable& symbolTable);

};

#endif
