#ifndef  COMPILER_H
#define COMPILER_H
#include "MoonGenerator.h"

// A simple wrapper class that holds on the neccessary data and data structures to init our compiler
class Compiler {
public:
	// Builds the compiler from the given grammar file
	Compiler(std::string grammarFile="grammar_with_sdt.txt", std::string grammarStartSymbol="prog", bool outputToFile=true);
	
	~Compiler();
	// Method used to turn the source code into the target code
	// This will be implemented in further assignments
	// Currently this is just a stub method
	void compile();
	// Sets the source file for our compiler to compiler
	void setSourceFile(std::string sourceFile);
	// Gets the symbol table for this program
	const SymbolTable& getSymbolTable();
	// Whether the source file has been parsed successfully
	bool parsedSuccessfully;
private:
	// the input source file name
	std::string sourceFile;
	// Whether to write the outputs
	bool writeOutputs;
	// stream that outputs all errors or any neccessary outputs
	std::ofstream outputStream;
	// The specfication for the lexer
	std::shared_ptr<Specification> spec;
	// The lexer that will tokenize our source
	std::shared_ptr<Lexer> lexer;
	// The grammar that holds the data structure for our LL(1) grammar
	std::shared_ptr<Grammar> grammar;
	// The parser that does the syntax/semantic checking and semantic actons
	std::shared_ptr<Parser> parser;
	// The code generator for the moon simulator
	// Translates our high level language to moon instructions
	std::shared_ptr<MoonGenerator> generartor;
	// TODO: figure out if this works
	// Executes the output moon file
	void executeMoonSimulator(std::string moonfile);
};

#endif // ! COMPILER_H