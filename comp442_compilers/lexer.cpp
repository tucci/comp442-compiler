#include "stdafx.h"
#include "lexer.h"


lexer::lexer() {
	source = "   1 23 55";
	index = 0;
	init_tokenizer();
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
		// TODO: find a better way to do this
		if (lookup != " ") {
			// add this char to our token as we go
			token.lexeme.append(lookup);
			// get the state for the current state and lookup
			current_state = tokenizer.table(current_state.state_identifier, lookup);
			// If we are the final, then we create the token
			if (current_state.is_final_state) {
				token_created = true;
				if (current_state.is_backup) {
					//backup_char();
				}
			}
		}
		
	} while (!token_created);

	return token;
}

std::string lexer::next_char() {
	return source.substr(index++, 1);
	
}

void lexer::init_tokenizer() {
	state start = tokenizer.create_start_state();
	state s2 = tokenizer.create_state(true, non_zero_t);
	tokenizer.add_transition(start, " ", start);
	tokenizer.add_transition(start, "1", s2);
	tokenizer.add_transition(start, "2", s2);
	tokenizer.add_transition(start, "3", s2);
	tokenizer.add_transition(start, "4", s2);
	tokenizer.add_transition(start, "5", s2);
	tokenizer.add_transition(start, "6", s2);
	tokenizer.add_transition(start, "7", s2);
	tokenizer.add_transition(start, "8", s2);
	tokenizer.add_transition(start, "9", s2);

}


