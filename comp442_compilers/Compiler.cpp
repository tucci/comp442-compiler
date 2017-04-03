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
	// Create the generator
	generartor = std::shared_ptr<MoonGenerator>(new MoonGenerator(&parser->globalTable));
	if (writeOutputs) {
		parser->outputParserDataToFile();
	}
}

Compiler::~Compiler() {
}

// Compiles our source code the to the target code
void Compiler::compile() {
	std::cout << "Compiling " << sourceFile << std::endl;
	std::cout << "Analyzing Syntax and Building symbol table...";
	parser->buildSymbolTable();
	parsedSuccessfully = parser->parse(); // Phase 2 parse
	std::cout << "Done" << std::endl;

	

	// TODO: output all errors in one file/stream
	if (writeOutputs) {
		parser->outputDerivationAndErrors();
		lexer->writeTokensToFile();
		parser->outputSymbolTable();
		parser->outputSemanticErrors();
	}

	if (parsedSuccessfully) {
		std::cout << "Parsed Successfully" << std::endl;
		std::cout << "Generating Code...";
		generartor->generateCode();
		std::cout << "Done" << std::endl;
		std::cout << "Executing Code" << std::endl;
		//executeMoonSimulator(generartor->getMoonFile());
	} else {
		std::cout << "There was an error during parsing" << std::endl;
	}
}

// Sets the source file for our compile to compile
void Compiler::setSourceFile(std::string sourceFile) {
	this->sourceFile = sourceFile;
	lexer->setSource(sourceFile);
	generartor->setOutputFileName(sourceFile.substr(0, sourceFile.find_first_of(".")) + ".m");
	
}

const SymbolTable& Compiler::getSymbolTable() {
	return parser->globalTable;
}

void Compiler::executeMoonSimulator(std::string moonfile) {
	// Run the process
	char   psBuffer[128];
	FILE   *pPipe;
	std::string moonExe = "moon.exe -p -s -t +x " + moonfile;
	if ((pPipe = _popen(moonExe.c_str(), "rt")) == NULL)
		exit(1);

	/* Read pipe until end of file, or an error occurs. */

	while (fgets(psBuffer, 128, pPipe)) {
		printf(psBuffer);
	}

	/* Close pipe and print return value of pPipe. */
	if (feof(pPipe)) {
		printf("\nProcess returned %d\n", _pclose(pPipe));
	} else {
		printf("Error: Failed to read the pipe to the end.\n");
	}

}


