#include "stdafx.h"

Lexer::Lexer() {
}

Lexer::Lexer(Specification* spec) {
	tokenizer = spec->getSpec();
}

Lexer::~Lexer() {
}

Token Lexer::nextToken() {
	Token token = lookaheadToken;
	lookaheadToken = getLookaheadToken();
	outputTokens.push_back(lookaheadToken);
	return token;
}

Token Lexer::getLookaheadToken() {
	
	// flag to see if we should add the transition char to the current lexeme we are building
	bool shouldAddChar;
	// flag to see if we created the token and should return it to the lexer
	bool tokenCreated = false;

	// the token to return
	Token token;
	// the lexeme for the token we are creating
	std::string lexeme;
	// create init state at state 1
	State currentState = *tokenizer->getStartingState();
	// loop until we have created a token
	do {
		// We should add chars to tokens such as id's or numbers
		shouldAddChar = false;

		// get the next char in the input
		char c = nextChar();
		std::string lookupStr;
		if (c == EOF) {
			// Convrt EOF = -1, to its string
			lookupStr = std::to_string(c);
		} else {
			lookupStr = std::string(1, c);
		}
	
		State* stateLookup;
		// Get the next state from the current state and lookup char
		stateLookup = tokenizer->table(currentState.stateIdentifier, lookupStr);

		// stateLookup == NULL when we dont have a state for this state and lookup
		if (stateLookup == NULL) {
			// Check to see if there is a else transition
			State* elseState = tokenizer->table(currentState.stateIdentifier, Dfa::ELSE_TRANSITION);
			// If we do have an else state, we'll go to it and continue
			if (elseState != NULL) {
				// Move to the else state
				currentState = *elseState;
				if (currentState.tokenType == TokenType::error_token && c == EOF) {
					Token token;
					token.type = TokenType::non_token;
					
					if (sourceIndex >= source.size()) {
						// If we are the end of the file, we'll change this to end of file token
						token.lexeme = SpecialTerminal::END_OF_FILE.getName();
						token.type = TokenType::end_of_file_token;
					}
					return token;
				}
				if (currentState.tokenType == TokenType::error_token) {				
					// there is an error in the source code and we should handle this
					handleError(&token, lexeme, &currentState, lookupStr);
					return token;
				}
			}

		} else {
			if (!isspace(c) && c != EOF) {
				// if this is not a whitespace character we can add it to our lexeme
				shouldAddChar = true;
			}

			// get the state normally for the current state and lookup
			currentState = *stateLookup;
			if (currentState.tokenType == TokenType::cmt_multi_start) {
				token.tokenLine = currentLine;
				handleComment(&token, &currentState);
				if (currentState.errorType == ErrorType::mulitcomment_error) {
					return token;
				}
			}
		}

		// Add the char to the lexeme
		if (shouldAddChar) {
			lexeme += c;
		}

		// If we are the final, then we create the token
		if (currentState.isFinalState) {
			// Create the the token from this state
			token = createToken(lexeme, currentState);
			tokenCreated = true;
			if (currentState.needsToBacktrack && c != EOF) {
				backupChar();
			}
		}
	} while (!tokenCreated);

	// Since we dont want to return comment tokens, we'll return the next token
	if (token.type == TokenType::cmt) {
		return getLookaheadToken();
	}
	return token;
}

bool Lexer::setSource(std::string pathToFile) {
	sourceFilePath = pathToFile;
	std::ifstream istream(pathToFile);
	bool readSuccess = false;
	outputTokens.clear();
	if (istream) {
		// put position of stream to the end of source file
		istream.seekg(0, istream.end);
		// get the size of the source file
		sourceSizeInBytes = istream.tellg();
		// put position of stream to the start of source file
		istream.seekg(0, istream.beg);
		// create our source buffer array
		source.reserve(sourceSizeInBytes);
		// copy the whole contents of file into the vector
		source = std::vector<char>((std::istreambuf_iterator<char>(istream)),std::istreambuf_iterator<char>());
		// Close the stream to the source file
		istream.close();
		// lines start at index 1 and not 0 in files
		currentLine = 1;
		nextToken();
		readSuccess = true;// everything was read properly
	}
	istream.close(); // close the stream
	return readSuccess;
}

bool Lexer::hasMoreTokens() {
	// If we have no more chars to read, then we are out of tokens to read
	return lookaheadToken.type != TokenType::non_token;
}

Token Lexer::createToken(std::string lexeme, State state) {
	Token token;
	token.lexeme = lexeme;
	// Set this token's type from the accept state
	token.type = state.tokenType;
	// Check and update token type if needed
	// To avoid adding more complexity to our dfa, we will change token types depending on their lexeme
	Specification::updateTokenType(&token);
	token.tokenLine = currentLine;
	return token;
}

char Lexer::nextChar() {
	if (sourceIndex < source.size()) {
		char c = source.at(sourceIndex);
		// Move to the next line if we are at a new line char
		if (sourceIndex != 0 && isNewLine(source.at(sourceIndex - 1))) {
			currentLine++;
		}
		// advance the char index in the source file
		sourceIndex++;
		return c;
	}
	// There are no more chars to read
	return EOF;
}

void Lexer::backupChar() {
	// go back in the source array
	if (sourceIndex != 0) {
		 sourceIndex--;
	}

}

bool Lexer::isNewLine(char c) {
	return c == '\n\r' || c == '\n' || c == '\r';
}



void Lexer::handleError(Token* token, std::string lexeme, State* errorState, std::string lookup) {

	switch (errorState->errorType) {
		case unknown_symbol: {
			// Create the error token. This is a not symbol in our alphabet
			token->lexeme = lookup;
			token->tokenLine = currentLine;
			token->type = TokenType::error_token;
			token->error = TokenError{"Unknown symbol", ErrorType::unknown_symbol};
			break;
		}
		case incomplete_float: {
			// Remove the last char in the lexeme. in this case it is a .
			lexeme.pop_back();
			// Backup so that next time we call nextToken, it will start from the .
			backupChar();
			backupChar();
			// Create the current integer token
			token->lexeme = lexeme;
			token->tokenLine = currentLine;
			token->type = TokenType::int_token;
			// Recover from this error
			// We are not going to tokens such as "3.a" as an error because we don't know anything about the language
			// Instead we are going to turn it into 3 tokens such as 3	.	a. Perhaphs the language has number literal functions
			break;
		}
		case invalid_float: {
				// Float that ends with 0 error. Does not follow the specification
				if (lookup != std::to_string(EOF)) {
					// Backup so that next time we call nextToken the next token will be recovered
					backupChar();
				}
				token->lexeme = lexeme;
				token->tokenLine = currentLine;
				token->type = TokenType::error_token;
				token->error = TokenError{ "Invalid float", ErrorType::invalid_float};
			break;
		}
		case mulitcomment_error: {
			if (token->tokenLine == -1) {
				token->tokenLine = currentLine;
			}
			token->type = TokenType::error_token;
			token->error = TokenError{ "Mulit-line comment not closed", ErrorType::mulitcomment_error };
			break;
		}
		default: {
			break;
		}
	}

}


void Lexer::handleComment(Token* token, State* currentState) {
	std::stack<char> cmtStack;
	cmtStack.push('/');
	cmtStack.push('*');
	char c;
	std::string lookup;
	bool inMulitCmt = true;
	State* state;
	while(inMulitCmt) {
		c = nextChar();
		if (c == EOF) {
			// Convrt EOF = -1, to its string
			lookup = std::to_string(c);
		} else {
			lookup = std::string(1, c);
		}

		if (!cmtStack.empty()) {
			char top = cmtStack.top();
			if (c == '/') {
				if (top == '*') {cmtStack.push('/');}
				else if (top == '/') {cmtStack.pop();}
			}
			if (c == '*') {
				if (top == '*') {
					if (source.at(sourceIndex - 1) == '*' && source.at(sourceIndex) == '/') {
						cmtStack.pop();
					}
					
				}
				else if (top == '/') {cmtStack.push('*');}
			}
			
		} else {
			inMulitCmt = false;
		}


		state = tokenizer->table(currentState->stateIdentifier, lookup);
		if (state == NULL) {
			state = tokenizer->table(currentState->stateIdentifier, Dfa::ELSE_TRANSITION);
		} else {
			*currentState = *state;
			if (currentState->tokenType == TokenType::cmt && cmtStack.empty()) {
				token->type = TokenType::cmt;
				inMulitCmt = false;
				break;
			}
			if (currentState->errorType == ErrorType::mulitcomment_error) {
				handleError(token, "", currentState, "");
				inMulitCmt = false;
				break;
			}
		}
	}
	


		
		
}

void Lexer::writeTokensToFile() {
	std::ofstream output;
	std::ofstream error;
	output.open("lexerOutput.txt");
	error.open("lexerErrors.txt");

	// TODO: also lexerOutput needs to be fixed. uses first follow set text
	// TODO: this is broken for some reason
	/*for (std::vector<Token>::iterator it = outputTokens.begin(); it != outputTokens.end(); ++it) {
		if (it->type == TokenType::error_token) {
			error << *it;
		} else {
			output << *it;
		}
	}*/

	output.close();
	error.close();
	std::cout << "Successfully Wrote to output.txt and errors.txt" << std::endl;
}
