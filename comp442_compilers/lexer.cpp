#include "stdafx.h"

lexer::lexer(specification* spec) {
	this->spec = spec->get_spec();
}

lexer::~lexer() {
}

token lexer::next_token() {

	// flag to see if we should add the transition char to the current lexeme we are building
	bool should_add_char;
	// flag to see if we are currently in a multi comment
	bool in_multi_comment = false;
	// flag to see if we created the token and should return it to the lexer
	bool token_created = false;
	// How many multi line comments we have. if cmt nest cout == 0, then we've matched any inner nested comments
	int cmt_nest_count = 0;

	// the token to return
	token token;
	// the lexeme for the token we are creating
	std::string lexeme;
	// create init state at state 1
	state current_state = { 1 };
	// loop until we have created a token
	do {
		// We should add chars to tokens such as id's or numbers
		should_add_char = false;

		// get the next char in the input
		char c = next_char();
		std::string lookup_str;
		// TODO: this may be confusing on first glance. Give a comment to tell the teacher why this is here
		// this is used to fix the problem with single line comments
		// However this might break other things
		if (c == EOF) {
			lookup_str = std::to_string(c);
		} else {
			lookup_str = std::string(1, c);
		}
	
		state* state_lookup;
		// Get the next state from the current state and lookup char
		state_lookup = spec->table(current_state.state_identifier, lookup_str);

		// If we dont have a stae for this state and lookup
		if (state_lookup == NULL) {
			// Check to see if there is a else transition
			state* else_state = spec->table(current_state.state_identifier, dfa::ELSE_TRANSITION);
			// If we do have an else state, we'll go to it and continue
			if (else_state != NULL) {
				current_state = *else_state;
				// Handle nested comments openings
				if (in_multi_comment) {
					// lookahead for comment open
					if (source_index != source.size() && c == '/' && source.at(source_index) == '*') {
						// Increment nested comment
						cmt_nest_count++;
					} 
				} else {
					if (current_state.token_type == token_type::cmt_multi_start && !in_multi_comment) {
						in_multi_comment = true;
					}
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
			if (!isspace(c) && c != EOF) {
				// if this is not a whitespace character we can add it to our lexeme
				should_add_char = true;
			}

			// If we are in a multi comment and have a comment nest
			// we need to go to it's previous state and not the final state
			if (in_multi_comment && cmt_nest_count > 0) {
				current_state = *spec->table(current_state.state_identifier, dfa::ELSE_TRANSITION);
			} else {
				// get the state for the current state and lookup
				current_state = *state_lookup;
			}
			
		}

		// Add the char to the lexeme or add it if we are in a comment
		// We should add chars when we are in a comment because all the characters matter, even the whitespaces
		if (should_add_char) {
			lexeme += c;
		}

		// If we are the final, then we create the token
		if (current_state.is_final_state && cmt_nest_count == 0) {
			// Create the the token from this state
			token = create_token(lexeme, current_state);
			token_created = true;
			if (current_state.needs_to_backtrack) {
				backup_char();
			}
		}
		// lookahead for comment close
		if (in_multi_comment && source.at(source_index - 2) == '*' && c == '/') {
			// decrement comment nest
			cmt_nest_count--;
		}
	} while (!token_created);

	// Since we dont want to return comment tokens, we'll return the next token
	if (token.type == token_type::cmt) {
		return next_token();
	}
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
		// copy the whole contents of file into the vector
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
	return !source.empty() && source_index < source.size() - 1;
}

token lexer::create_token(std::string lexeme, state state) {
	token t;
	t.lexeme = lexeme;
	// Set this token's type from the accept state
	t.type = state.token_type;
	// Check and update token type if needed
	// To avoid adding more complexity to our dfa, we will change tokens depending on their lexeme
	if (t.type == token_type::id) {
		// Check to see if this identifier is in our token map
		std::unordered_map<std::string, token_type>::const_iterator in_token_map = specification::TOKEN_MAP.find(t.lexeme);
		// We will change it from an identifier token to it's respective token
		if (in_token_map != specification::TOKEN_MAP.end()) {
			t.type = in_token_map->second;
		}
	}
	if (t.type == token_type::num) {
		// if it is a float we will change it
		if (t.lexeme.find('.') != std::string::npos) {
			t.type = token_type::float_token;
		} else {
			// if it is an integer we will change it to that
			t.type = token_type::int_token;
		}
	}
	t.token_line = current_line;
	return t;
}

char lexer::next_char() {
	if (source_index < source.size()) {
		char c = source.at(source_index);
		// advance the char index in the source file
		if (source_index != 0 && is_new_line(source.at(source_index - 1))) {
			current_line++;
		}
		source_index++;
		return c;
	}
	// There are no more chars to read
	return EOF;
}

void lexer::backup_char() {
	// go back in the source array
	if (source_index != 0 && source_index < source.size()) {
		 char c = source.at(source_index);
		 // Handle case where we backup to the previous line
		 if (is_new_line(c)) {
			 current_line--;
		 }
		 source_index--;
	}

}

bool lexer::is_new_line(char c) {
	return c == '\n\r' || c == '\n' || c == '\r';
}
