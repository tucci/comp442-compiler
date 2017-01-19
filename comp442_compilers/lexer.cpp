#include "stdafx.h"
#include "lexer.h"


lexer::lexer(specification spec) {
	this->spec = spec.get_spec();
}

lexer::~lexer() {
}

token lexer::next_token() {
	//if (lookup == "\n") {
	//    // TODO: make sure we dont start at line 1
	//    // TODO: test this
	//    source_line_index++;
	//    source_char_index++;
	//}
	

	bool token_created = false;
	token token;
	// create init state at state 1
	state current_state = {1};
	// loop until we have created a token
	do {
		// get the next char in the input
		char lookup = *next_char();
		if (lookup == NULL) {
			out_of_tokens = true;
			// TODO: figure out a better way to return a nullable value
			token = { "", token_type::null_token ,-1 , -1 };
			break;
		}
		// get the state for the current state and lookup
		current_state = spec.table(current_state.state_identifier, std::string(1, lookup));

		// If we are the final, then we create the token
		if (current_state.is_final_state) {
			token_created = true;
			token = create_token(current_state);
			if (current_state.is_backup) {
				// TODO: implement backtracking and see why
				backup_char();
			}
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
		return true; // everything was read properly
	}
	istream.close();
	return false; // file was not read properly
	
}

bool lexer::has_more_tokens() {
	return !out_of_tokens;
}

token lexer::create_token(state state) {
	token token;
	// TODO: figure out how to set this token's lexeme
	// TODO: set token line and index
	// TODO: check if this token is a reserved word or operator
	token.type = state.token_type;
	return token;
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



