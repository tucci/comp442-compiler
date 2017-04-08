#ifndef MOON_GENERATOR_H
#define MOON_GENERATOR_H
#include "Attribute.h"

// A simple struct that holds the label/name for a temoporary memory value in the moon simulator
struct TempMemory {
	// The actual name/label of the temp memory in the moon code
	std::string label;
};

// Moon generator generates all the low level moon simulation code
// Converts our high level lanugage to moon code
class MoonGenerator {
public:
	// Creates a moon generator witht the given global table
	// You can also specify how much temporary memory the simulation could use
	MoonGenerator(SymbolTable* symbolTable, int tempMemorySize = 32);
	MoonGenerator();
	~MoonGenerator();
	// Add an instruction to the instruction list for the generator to create
	void addInstruction(std::shared_ptr<Instruction> instruction);
	// This takes all the added instructions and creates a moon file with the moon code
	void generateCode();
	// Returns the name of the ouput moonfile
	std::string getMoonFile();
	// Set the name of the output moonfile. You must add the .m file extension
	// This method does not append .m to the name of the file
	void setOutputFileName(std::string outputfile);
	// Claim/free register data
	Register getUnusedRegister();
	void freeRegister(Register r);
	// Claim/free temp memory
	TempMemory getTempMemory();
	void freeTempMemory(TempMemory tn);
	// Adds a declaration declration and its statments to the generator
	// Since function declarations can't be defined during code execution or after the align directive
	// It must be generated before the actual code generation
	void addFuncDeclStatmentList(FunctionDeclStatementList funcDec);
private:
	// The output stream of the generated moon code
	std::ofstream moonOutputStream;
	// The global symbol table for the code we are generating
	SymbolTable* globalTable;
	// list of function declarations to be generated before the actual user program/align directive
	std::vector<FunctionDeclStatementList> functionDeclarations;
	// the list of instructions inside the program body
	std::vector<std::shared_ptr<Instruction>> instructions;
	// the output name of the moon file
	std::string outputMoonFile;
	// map of registers being used or free
	// Where second value means if the register is free or not
	// if true, that means the register is free to use
	// Does not include r0,r14,r15 as these values are special registers used for jumping and return values
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
	// This allocated the memory for all the variables/arrays/objects for this record
	// This will create all the function return values memory, variable/parameter memory for this record
	void createEntriesForRecord(SymbolTableRecord& record);
	// Generates the function declaration for the provided function declaration statment list
	void generateCodeForFunction(FunctionDeclStatementList funcStatements);

};

#endif
