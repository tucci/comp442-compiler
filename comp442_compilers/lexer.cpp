#include "stdafx.h"

Lexer::Lexer(Specification* spec) {
	tokenizer = spec->getSpec();
}

Lexer::~Lexer() {
}

Token Lexer::nextToken() {

	// flag to see if we should add the transition char to the current lexeme we are building
	bool shouldAddChar;
	// flag to see if we are currently in a multi comment
	bool inMultiComment = false;
	// flag to see if we created the token and should return it to the lexer
	bool tokenCreated = false;
	// How many multi line comments we have. if cmt nest cout == 0, then we've matched any inner nested comments
	int cmtNestCount = 0;

	// the token to return
	Token token;
	// the lexeme for the token we are creating
	std::string lexeme;
	// create init state at state 1
	State currentState = { 1 };
	// loop until we have created a token
	do {
		// We should add chars to tokens such as id's or numbers
		shouldAddChar = false;

		// get the next char in the input
		char c = nextChar();
		std::string lookupStr;
		// TODO: this may be confusing on first glance. Give a comment to tell the teacher why this is here
		// this is used to fix the problem with single line comments
		// However this might break other things
		if (c == EOF) {
			lookupStr = std::to_string(c);
		} else {
			lookupStr = std::string(1, c);
		}
	
		State* stateLookup;
		// Get the next state from the current state and lookup char
		stateLookup = tokenizer->table(currentState.stateIdentifier, lookupStr);

		// If we dont have a stae for this state and lookup
		if (stateLookup == NULL) {
			// Check to see if there is a else transition
			State* elseState = tokenizer->table(currentState.stateIdentifier, Dfa::ELSE_TRANSITION);
			// If we do have an else state, we'll go to it and continue
			if (elseState != NULL) {
				currentState = *elseState;
				// Handle nested comments openings
				if (inMultiComment) {
					// lookahead for comment open
					if (sourceIndex != source.size() && c == '/' && source.at(sourceIndex) == '*') {
						// Increment nested comment
						cmtNestCount++;
					} 
				} else {
					if (currentState.tokenType == TokenType::cmt_multi_start && !inMultiComment) {
						inMultiComment = true;
					}
				}
			} else {
				// If we don't have en else state, then there is an error in the source code
				// and we should handle this

				// TODO: write error handling
				// this is used right now to test states that dont have else transitions
				// Question: How should we handle the error in the code?
				// Possible solutions: just move on to the next char?
				// Possible solutions: create an error token and give line number?
				std::cout << "error";
			}

		} else {
			if (!isspace(c) && c != EOF) {
				// if this is not a whitespace character we can add it to our lexeme
				shouldAddChar = true;
			}

			// If we are in a multi comment and have a comment nest
			// we need to go to it's previous state and not the final state
			if (inMultiComment && cmtNestCount > 0) {
				currentState = *tokenizer->table(currentState.stateIdentifier, Dfa::ELSE_TRANSITION);
			} else {
				// get the state for the current state and lookup
				currentState = *stateLookup;
			}
			
		}

		// Add the char to the lexeme or add it if we are in a comment
		// We should add chars when we are in a comment because all the characters matter, even the whitespaces
		if (shouldAddChar) {
			lexeme += c;
		}

		// If we are the final, then we create the token
		if (currentState.isFinalState && cmtNestCount == 0) {
			// Create the the token from this state
			token = createToken(lexeme, currentState);
			tokenCreated = true;
			if (currentState.needsToBacktrack) {
				backupChar();
			}
		}
		// lookahead for comment close
		if (inMultiComment && source.at(sourceIndex - 2) == '*' && c == '/') {
			// decrement comment nest
			cmtNestCount--;
		}
	} while (!tokenCreated);

	// Since we dont want to return comment tokens, we'll return the next token
	if (token.type == TokenType::cmt) {
		return nextToken();
	}
	return token;
}

bool Lexer::setSource(std::string pathToFile) {
	sourceFilePath = pathToFile;
	std::ifstream istream(pathToFile);
	bool readSuccess = false;
	if (istream) {
		// put position of stream to the end of source file
		istream.seekg(0, istream.end);
		// get the size of the source file
		sourceSize = istream.tellg();
		// put position of stream to the start of source file
		istream.seekg(0, istream.beg);
		// create our source buffer array
		source.reserve(sourceSize);
		// copy the whole contents of file into the vector
		source = std::vector<char>((std::istreambuf_iterator<char>(istream)),std::istreambuf_iterator<char>());
		// Close the stream to the source file
		istream.close();
		// lines start at index 1 and not 0 in files
		currentLine = 1;
		readSuccess = true;// everything was read properly
	}
	istream.close(); // close the stream
	return readSuccess;
}

bool Lexer::hasMoreTokens() {
	// If we have no more chars to read, then we are out of tokens to read
	return !source.empty() && sourceIndex < source.size() - 1;
}

Token Lexer::createToken(std::string lexeme, State state) {
	Token token;
	token.lexeme = lexeme;
	// Set this token's type from the accept state
	token.type = state.tokenType;
	// Check and update token type if needed
	// To avoid adding more complexity to our dfa, we will change tokens depending on their lexeme
	if (token.type == TokenType::id) {
		// Check to see if this identifier is in our token map
		std::unordered_map<std::string, TokenType>::const_iterator inTokenMap = Specification::TOKEN_MAP.find(token.lexeme);
		// We will change it from an identifier token to it's respective token
		if (inTokenMap != Specification::TOKEN_MAP.end()) {
			token.type = inTokenMap->second;
		}
	}
	// Convert num types to their respective int/float tokens
	if (token.type == TokenType::num) {
		// if it is a float we will change it
		if (token.lexeme.find('.') != std::string::npos) {
			token.type = TokenType::float_token;
		} else {
			// if it is an integer we will change it to that
			token.type = TokenType::int_token;
		}
	}
	token.tokenLine = currentLine;
	return token;
}

char Lexer::nextChar() {
	if (sourceIndex < source.size()) {
		char c = source.at(sourceIndex);
		// advance the char index in the source file
		if (sourceIndex != 0 && isNewLine(source.at(sourceIndex - 1))) {
			currentLine++;
		}
		sourceIndex++;
		return c;
	}
	// There are no more chars to read
	return EOF;
}

void Lexer::backupChar() {
	// go back in the source array
	if (sourceIndex != 0 && sourceIndex < source.size()) {
		 char c = source.at(sourceIndex);
		 sourceIndex--;
	}

}

bool Lexer::isNewLine(char c) {
	return c == '\n\r' || c == '\n' || c == '\r';
}
