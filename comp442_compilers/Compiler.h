#ifndef  COMPILER_H
#define COMPILER_H

// A simple wrapper class that holds on the neccessary data and data structures to init our compiler
class Compiler {
public:
	Compiler(std::string grammarFile="grammar.txt", std::string grammarStartSymbol="prog", bool outputToFile=true);
	
	~Compiler();
	// Method used to turn the source code into the target code
	// This will be implemented in further assignments
	// Currently this is just a stub method
	void compile();
	// Sets the source file for our compiler to compiler
	void setSourceFile(std::string sourceFile);
	// Checks the syntax of the source fie
	bool analyzeSyntax();
private:
	bool writeOutputs;
	// The specfication for the token
	std::shared_ptr<Specification> spec;
	// The grammar that holds the data structure for our LL(1) grammar
	std::shared_ptr<Grammar> grammar;
	// The lexer that will tokenize our source
	std::shared_ptr<Lexer> lexer;
	//  The parser that does the syntax checking
	std::shared_ptr<Parser> parser;
};

#endif // ! COMPILER_H