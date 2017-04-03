#ifndef MOON_GENERATOR_H
#define MOON_GENERATOR_H
#include "Directives.h"


class MoonGenerator {
public:
	MoonGenerator(SymbolTable* symbolTable);
	MoonGenerator();
	~MoonGenerator();
	void addInstruction(std::shared_ptr<Instruction> instruction);
	void generateCode();
	std::string getMoonFile();
	void setOutputFileName(std::string outputfile);
private:
	std::ofstream moonOutputStream;
	SymbolTable* globalTable;
	std::vector<std::shared_ptr<Instruction>> instructions;
	std::string outputMoonFile;

	void createEntriesForTable(SymbolTable& symbolTable);

};

#endif
