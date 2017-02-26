#include "stdafx.h"


Compiler::Compiler(std::string grammarFile, std::string grammarStartSymbol, bool outputTofile) {
	writeOutputs = outputTofile;
	// Create the spec for the tokens
	spec = std::shared_ptr<Specification>(new Specification());
	// Read the grammar from the file that our parser will use
	grammar = std::shared_ptr<Grammar>(new Grammar(grammarFile, grammarStartSymbol));
	// Create the lexer with the given specification
	lexer = std::shared_ptr<Lexer>(new Lexer(spec.get()));
	// Create the parser given from our lexer and grammar
	parser = std::shared_ptr<Parser>(ParserGenerator::buildParser(lexer.get(), grammar.get()));
	if (writeOutputs) {
		parser->outputParserDataToFile();
	}
}

Compiler::~Compiler() {
}

// Compiles our source code the to the target code
void Compiler::compile() {
	// Method stub
}

// Sets the source file for our compile to compile
void Compiler::setSourceFile(std::string sourceFile) {
	lexer->setSource(sourceFile);
}

// Analyses the syntax of the source code
bool Compiler::analyzeSyntax() {
	std::cout << "Analyzing Syntax..." << std::endl;
	bool parsedSuccessfully = parser->parse();

	if (writeOutputs) {
		parser->outputAnalysis();
		lexer->writeTokensToFile();
	}

	if (parsedSuccessfully) {
		std::cout << "Parsed Successfully" << std::endl;
		if (writeOutputs) {
			std::cout << "See derivation.html for derivation" << std::endl;
		}
	} else {
		std::cout << "There was an error during parsing" << std::endl;
		if (writeOutputs) {
			std::cout << "See parserErrors.html for errors" << std::endl;
		}
	}
	return parsedSuccessfully;
}


