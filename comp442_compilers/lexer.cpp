#include "stdafx.h"

lexer::lexer(specification* spec) {
	this->spec = spec->get_spec();
}

lexer::~lexer() {
	std::cout << "delete lexer" << std::endl;
}

token lexer::next_token() {

	// flag to see if we should add the transition char to the current lexeme we are building
	bool should_add_char;
	// flag to see if we are currently in a comment
	bool in_comment = false;
	// Find the lookahead token
	bool token_created = false;
	int cmt_start_line;
	int cmt_start_col;
	// the token to return
	token token;
	// create init state at state 1
	std::string lexeme;
	state current_state = { 1 };
	// loop until we have created a token
	do {
		should_add_char = false;
		// get the next char in the input
		char* lookup = next_char();
		
		// advance the column in the current line
		current_column++;

		state* state_lookup;
		if (lookup != NULL) {
			// Get the next state from the current state and lookup char
			state_lookup = spec->table(current_state.state_identifier, std::string(1, *lookup));
		} else {
			state_lookup = NULL;
		}

		if (state_lookup == NULL) {
			// Check to see if there is a else transition
			state* else_state = spec->table(current_state.state_identifier, dfa::ELSE_TRANSITION);
			// If we do have an else state, we'll go to it and continue
			if (else_state != NULL) {
				current_state = *else_state;
				if (current_state.token_type == token_type::cmt_start && !in_comment) {
					// Since multiline comments have multiple lines in them, it would be a nightmare to compute the starting position
					cmt_start_line = current_line;
					cmt_start_col = current_column - 2; // Where 2 is the length of the comment of "//" or "/*"
					in_comment = true;
				}
			} else {
				// If we don't have en else state, then there is an error in the source code
				// and we should handle this

				// TODO: Figure out if we need this bit of code
				// this is used right now to test states that dont have else transitions
				// Question: How should we handle the error in the code?
				// Possible solutions: just move on to the next char?
				// Possible solutions: create an error token and give line number?
				std::cout << "error";
			}

		} else {
			// TODO: figure out a way to handle comments
			if (!isspace(*lookup)) {
				// if this is not a whitespace character we can add it to our lexeme
				should_add_char = true;
			}
			// get the state for the current state and lookup
			current_state = *state_lookup;
		}

		// Add the char to the lexeme or add it if we are in a comment
		// We should add chars when we are in a comment because all the characters matter, even the whitespaces
		if (should_add_char || in_comment) {
			lexeme += *lookup;
		}

		// If we are the final, then we create the token
		if (current_state.is_final_state) {
			if (current_state.token_type == token_type::cmt) {
				// Create the token for this comment
				token = create_token(lexeme, current_state, cmt_start_line, cmt_start_col);
			} else {
				// Create the the token from this state
				token = create_token(lexeme, current_state);
			}
			
			token_created = true;
			if (current_state.needs_to_backtrack) {
				backup_char();
			}
		}

		// Check to see if we are at the end of a line
		if (lookup != NULL && is_new_line(*lookup) && !token_created) {
			current_line++;
			current_column = 0;
		}
	} while (!token_created);
	return token;
}

bool lexer::set_source(std::string path_to_file) {
	source_file_path = path_to_file;
	std::ifstream istream(path_to_file);
	bool read_success = false;
	if (istream) {
		// put position of stream to the end of source file
		istream.seekg(0, istream.end);
		// get the size of the source file
		source_size = istream.tellg();
		// put position of stream to the start of source file
		istream.seekg(0, istream.beg);
		// create our source buffer array
		source.reserve(source_size);
		// copy the whole contents of file into the buffer
		source = std::vector<char>((std::istreambuf_iterator<char>(istream)),std::istreambuf_iterator<char>());
		// Close the stream to the source file
		istream.close();
		// lines start at index 1 and not 0 in files
		current_line = 1;
		read_success = true;// everything was read properly
	}
	istream.close(); // close the stream
	return read_success;
}

bool lexer::has_more_tokens() {
	// If we have no more chars to read, then we are out of tokens to read
	return source_index < source.size() - 1;
}

token lexer::create_token(std::string lexeme, state state) {
	token t;
	t.lexeme = lexeme;
	// Set this token's type from the accept state
	t.type = state.token_type;
	// Check and update token type if needed
	specification::update_token_for_lexeme(&t);
	t.token_line = current_line;
	// We need token length to compute the starting char index for this token
	// instead of having the index point to the end of the token in the source file
	// We don't compute the column for comments here as they are a little more complex
	// If you want to create a comment token, call the the other create_token
	// and specify where the comment line and column are
	t.token_column = current_column - t.lexeme.size();
	return t;
}
token lexer::create_token(std::string lexeme, state state, int line, int column) {
	// this method is currently used to fix the problem with computing multi line comment start offsets
	// in order to compute multiline comments starting position
	// we would need to keep track of how many lines it has and how many columns for each of those lines it has
	// that is more diffcult than it needs to be. So we will solve this by setting the starting position when we encouter the comment
	// create token normally
	token t = create_token(lexeme, state);
	// adjust the line and column indexes to the specified valus
	t.token_line = line;
	t.token_column = column;
	return t;
}

char* lexer::next_char() {
	if (source_index < source.size()) {
		// advance the char index in the source file
		return &source.at(source_index++);
	}
	// There are no more chars to read
	return NULL;	
}

void lexer::backup_char() {
	// if we are at the start of a line, we need to go back up to the previous line
	if (current_column == 1) {
		// go the the previous line
		current_line--;
		// go to the end of the previous line
		current_column = previous_line_column_count;
	} else {
		// We are in the middle of the line
		current_column--;
	}
	// go back in the source array
	source_index--;
}

bool lexer::is_new_line(char c) {
	return c == '\n\r' || c == '\n' || c == '\r';
}



