#ifndef MOON_GENERATOR_H
#define MOON_GENERATOR_H


class MoonGenerator {
public:
	MoonGenerator(SymbolTable* symbolTable);
	MoonGenerator();
	~MoonGenerator();
	void addInstruction(std::shared_ptr<Instruction> instruction);
	void generateCode();
	std::string getMoonFile();
	void setOutputFileName(std::string outputfile);
	Register getUnusedRegister();
	void freeRegister(Register r);
private:
	std::ofstream moonOutputStream;
	SymbolTable* globalTable;
	std::vector<std::shared_ptr<Instruction>> instructions;
	std::string outputMoonFile;
	void initRegisterAllocation();
	// map of registers being used or free
	// Where second value means if the register is free or not
	// if true, that means the register is free to use
	// Does not include r0
	std::unordered_map<std::string, bool> registers;
	void createEntriesForTable(SymbolTable& symbolTable);

};

#endif
