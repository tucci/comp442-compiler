#include "stdafx.h"
#include "lexer.h"


lexer::lexer(specification spec) {
	this->spec = spec.get_spec();
}

lexer::~lexer() {
}

token lexer::next_token() {
	bool token_created = false;
	// the token to return
	token token;
	// create init state at state 1

	state current_state = {1};
	// loop until we have created a token
	do {
		// get the next char in the input
		char lookup = *next_char();
		// There are no more chars to read
		if (lookup == NULL) {
			// Since we have no more chars to read
			// we also have no more tokens to parse
			out_of_tokens = true;
			// TODO: figure out a better way to return a nullable value
			// Return a value indicating that no token was create
			token = { "", token_type::null_token ,-1 , -1 };
			break;
		}
		// advance the char index in the source file
		current_char_index++;
	
		// TODO: figure out a way to handle comments
		if (!isspace(lookup)) {
			// if this is not a whitespace character we can add it to our lexeme
			token.lexeme += lookup;
		}
		const state* state_lookup = spec.table(current_state.state_identifier, std::string(1, lookup));
		if (state_lookup == NULL) {
			// Check to see if there is a else transition
			const state* else_state = spec.table(current_state.state_identifier, dfa::ELSE_TRANSITION);
			if (else_state != NULL) {
				current_state = *else_state;
			} else {
				// TODO: Figure out if we need this bit of code
				// this is used right now to test states that dont have else transitions
				// Question: does this mean that there is an error in the code and we should just move on to the next char?
				// Answer:???
				throw 10;
			}
			
		} else {
			// get the state for the current state and lookup
			current_state = *state_lookup;
		}
		

		// If we are the final, then we create the token
		if (current_state.is_final_state) {
			token_created = true;
			token.type = current_state.token_type;
			token.token_line = current_line_index;
			// We need token length to compute the starting char index for this token
			// instead of having the index point to the end of the token in the source file
			token.token_location = current_char_index - token.lexeme.size();;
			if (current_state.is_backup) {
				// TODO: implement backtracking and see why
				backup_char();
			}
		}
		// Check to see if we are at the end of a line
		if (lookup == '\n\r' || lookup == '\n' || lookup == '\r') {
			current_line_index++;
			current_char_index = 0;
		}
	} while (!token_created);

	return token;
}

bool lexer::set_source(std::string path_to_file) {
	source_file_path = path_to_file;
	std::ifstream istream(path_to_file);
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
		current_line_index = 1;
		return true; // everything was read properly
	}
	istream.close();
	return false; // file was not read properly
	
}

bool lexer::has_more_tokens() {
	return !out_of_tokens;
}

char* lexer::next_char() {
	if (source_index < source.size()) {
		return &source.at(source_index++);
	}
	return NULL;	
}

void lexer::backup_char() {
	source_index--;
}




