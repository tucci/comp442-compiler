#ifndef LEXER_H
#define LEXER_H


class Lexer {
public:
	Lexer();
	// Create the lexer with the language specification for this lexer to use
	Lexer(Specification* spec);
	~Lexer();
	// Spits out the next token that the lexer reads
	Token nextToken();
	// Sets the source file for this lexer to tokenize
	bool setSource(std::string pathToFile);
	// Whether the lexer has more tokens
	bool hasMoreTokens();
	// Writes the tokens and error tokens to the file
	void writeTokensToFile(std::ostream* outputFile);
	// Resets the lexer to the start of the token stream
	void resetToStart();
	void setMainStream(std::ostream* stream);
private:
	// The state transiton table tokenizer that holds all the rules for this lexer
	std::shared_ptr<Dfa> tokenizer;

	// The token that is returned back from nextToken
	Token lookaheadToken;
	// the current line of the source file we are at
	int currentLine;
	// The file path to the current source file
	std::string sourceFilePath;
	// The buffer to hold the source file
	std::vector<char> source;
	// The list of tokens in ourfile to be used when outputing to file
	std::vector<Token> outputTokens;
	// The current token we are at in the output tokens. used when doing phase 2
	int tokenIndex;
	// the size of the source file in bytes
	// use streamoff to avoid data loss
	std::streamoff sourceSizeInBytes;
	// the current index of the source buffer we are indexing into
	int sourceIndex;
	// If the lexer has been reset
	bool streamReset;

	// Error stream
	std::ofstream errorStream;
	std::ostream* mainStream;
	// This does all the actual work of getting the token
	Token getLookaheadToken();
	// Returns the token that is evaluated from this state
	Token createToken(std::string lexeme, State state);
	// Returns the next char in the stream for the tokenizer to read from
	char nextChar();
	// moves the current sourceIndex back by one
	void backupChar();
	// Handle the error for this state
	void handleError(Token* token, std::string lexeme, State* errorType, std::string lookup);
	// Method to handle comments while tokenizing
	void handleComment(Token* token, State* state);
};

#endif

