#ifndef DFA_H
#define DFA_H

#include <unordered_map>
#include "token.h"


struct state {
	// integer value to hold the identifier for this state (ie. state 1, state 2, ...)
	int state_identifier;
	// Whether this state is a start state
	bool is_start_state;
	// Whether this state is a final state
	bool is_final_state;
	// Whether this state needs to use backtracking
	bool is_backup;
	// the token type for the lexeme it accepts
	token_type final_token;
};

class dfa {
public:
	dfa();
	~dfa();
	// Create the starting state for this dfa
	state create_start_state();
	// Create the a state to be used in this dfa
	state create_state(bool is_final_state=false, bool is_backup=false, token_type type=non_token_t);
	// add a transition from the from state to the to_state using a transition string
	bool add_transition(state from_state, std::string transition, state to_state);
	// Add an else transition to the two states
	bool add_else_transition(state from_state, state to_state);
	// Get the transition state from the from_state identifier using lookup_transition input
	state table(int from_state, std::string lookup_transition);

private:
	// constant identifier to be used in the state transition table to use when no onther lookup is there
	const static std::string ELSE_TRANSITION;
	// the count for how many states we have
	int m_state_count;
	// Check to see if the dfa has the current state
	bool has_state(state state);
	// the data structure that stores the state transition table
	// where the key to this map is the state identifier
	// the value to for the specified key, is the transition map for that state
	std::unordered_map<int, std::unordered_map<std::string, state>*> state_transition_table;
};

#endif

