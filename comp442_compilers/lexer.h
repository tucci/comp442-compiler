#ifndef LEXER_H
#define LEXER_H

class Lexer {
public:
	// Create the lexer with the specifiec language specification for this lexer to use
	Lexer(Specification* spec);
	~Lexer();
	// Spits out the next token that the lexer reads
	Token nextToken();
	// Sets the source file for this lexer to tokenize
	bool setSource(std::string pathToFile);
	// Whether the lexer has more tokens
	bool hasMoreTokens();
private:
	// The state transiton table tokenizer that holds all the rules for this lexer
	std::shared_ptr<Dfa> tokenizer;
	
	// the current line of the source file we are at
	int currentLine;
	// The file path to the current source file
	std::string sourceFilePath;
	// The buffer to hold the source file
	std::vector<char> source;
	// the size of the source file in bytes
	int sourceSize;
	// the current index of the source buffer we are indexing into
	int sourceIndex;

	// Returns the token that is evaluated from this state
	Token createToken(std::string lexeme, State state);
	// Returns the next char pointer in the stream for the tokenizer to read from
	char nextChar();
	// moves the pointer of the current sourceIndex back by one
	void backupChar();
	// Check if the character is a newline character
	bool isNewLine(char);
};

#endif

