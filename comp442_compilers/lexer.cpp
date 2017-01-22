#include "stdafx.h"

lexer::lexer(specification* spec) {
	this->spec = spec->get_spec();
}

lexer::~lexer() {
	std::cout << "delete lexer" << std::endl;
}

token lexer::next_token() {
	// Next token works by always keeping a reference to a lookahead token
	// this is done so that when driver code/parser keeps calling next_token()
	// it doesn't get a null token at the end
	// Keeping a lookahead token ensures that has_more_tokens() will always supply it with actual tokens
	// and spit out a nullable token at the end of stream indicating that this is the last token

	// next_token() basically sets the current token to return to the lookahed token we alread had
	// then it finds the lookahead token and caches it for the next call to next_token()
	// then once we found the lookahead token, we return the current token

	// TODO: fix duplicate token creation
	token current_token;
	// flag to see if we should add the transition char to the current lexeme we are building
	bool should_add_char;
	// flag to see if we are currently in a comment
	bool in_comment;
	// if this is not the first call to next_token
	// we should set the current token to the lookahead token we already found last time we called next_token
	if (lookahead_token.type != token_type::non_token) {
		// set the current token to the lookahead token
		// we found previously
		current_token = lookahead_token;
	} else {
		// this is the first call to next_token()
		// We need to set this to a non token type
		// because the first time we call next_token()
		// we need to identidy that we don't have a lookahead token
		current_token = { "", token_type::non_token };
	}


	// Find the lookahead token
	bool token_created = false;
	// the token to return
	token token;
	// create init state at state 1
	std::string lexeme;
	state current_state = { 1 };
	// loop until we have created a token
	do {
		in_comment = false;
		should_add_char = false;
		// get the next char in the input
		char* lookup_ptr = next_char();
		char lookup;
		// There are no more chars to read
		if (lookup_ptr == NULL) {
			// Since we have no more chars to read
			// we also have no more tokens to parse
			out_of_tokens = true;
			token = create_token(lexeme, current_state);
			break;
		} else {
			lookup = *lookup_ptr;
		}
		// advance the char index in the source file
		current_char_index++;

		// Get the next state from the current state and lookup char
		state* state_lookup = spec->table(current_state.state_identifier, std::string(1, lookup));

		if (state_lookup == NULL) {
			// Check to see if there is a else transition
			state* else_state = spec->table(current_state.state_identifier, dfa::ELSE_TRANSITION);
			// If we do have an else state, we'll go to it and continue
			if (else_state != NULL) {
				current_state = *else_state;
				if (current_state.token_type == token_type::cmt_start) {
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
			if (!isspace(lookup)) {
				// if this is not a whitespace character we can add it to our lexeme
				should_add_char = true;
				
			}
			// get the state for the current state and lookup
			current_state = *state_lookup;
		}
		if (in_comment || should_add_char) {
			lexeme += lookup;
		}
		
		// If we are the final, then we create the token
		if (current_state.is_final_state) {
			token = create_token(lexeme, current_state);
			token_created = true;
			if (current_state.needs_to_backtrack) {
				backup_char();
			}
		}
		// Check to see if we are at the end of a line
		if (lookup == '\n\r' || lookup == '\n' || lookup == '\r') {
			current_line_index++;
			current_char_index = 0;
		}
	} while (!token_created);


	// If the lookahead token is a non token
	// then we have no more tokens to read
	if (current_state.token_type == token_type::non_token) {
		out_of_tokens = true;
	} else {
		// If the next token is not a non token, then it is a good token
		// Set the lookahead token to the one we just found
		lookahead_token = token;
	}
	// If there is no current token
	// then this means this is the first time we are calling next_token()
	// which means we have no current token because there doesn't exist a lookahead token
	// so we will just return the lookahead token
	if (current_token.type == token_type::non_token) {
		return lookahead_token;
	}

	// Return the current token
	// This value was actually found last time we called next_token
	return current_token;
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
		current_line_index = 1;
		read_success = true;// everything was read properly
	}
	istream.close();
	next_token();
	return read_success; // file was not read properly
	
}

bool lexer::has_more_tokens() {
	return !out_of_tokens;
}

token lexer::create_token(std::string lexeme, state state) {
	token t;
	t.lexeme = lexeme;
	t.type = state.token_type;
	// Check and update token type if needed
	specification::update_token_for_lexeme(&t);
	t.token_line = current_line_index;
	// We need token length to compute the starting char index for this token
	// instead of having the index point to the end of the token in the source file
	t.token_column = current_char_index - t.lexeme.size();
	return t;
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




