#include "stdafx.h"
#include "lexer.h"


lexer::lexer(specification spec) {
	this->spec = spec.get_spec();
}

lexer::~lexer() {
}

token lexer::next_token() {
	bool token_created = false;
	token token;
	// create init state at state 1
	state current_state = {1};
	// loop until we have created a token
	do {
		// get the next char in the input
		std::string lookup = next_char();
		// TODO: find a better way to do this, dont want to put spaces here
		// TODO: appending the lexeme like wont work out nicely
		if (lookup != " ") {
			// add this char to our token as we go
			token.lexeme.append(lookup);
			// get the state for the current state and lookup
			current_state = spec.table(current_state.state_identifier, lookup);
			// If we are the final, then we create the token
			if (current_state.is_final_state) {
				token_created = true;
				if (current_state.is_backup) {
					// TODO: implement backtracking. might need to double buffer
					//backup_char();
				}
			}
		}
		
	} while (!token_created);

	return token;
}

void lexer::set_source(std::string path_to_file) {
	source_file = path_to_file;
}


std::string lexer::next_char() {
	//return source.substr(index++, 1);
	
}


