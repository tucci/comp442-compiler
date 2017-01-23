#ifndef DFA_H
#define DFA_H



struct state {
	// integer value to hold the identifier for this state (ie. state 1, state 2, ...)
	int state_identifier;
	// Whether this state is a start state
	bool is_start_state;
	// Whether this state is a final state
	bool is_final_state;
	// Whether this state needs to tell the lexer to backtracking
	// This will usually be true if there is an else transition to this state
	// Example the else state between < intermediate and < final state use an else transition between them
	// So the < final state needs to be a back tracking state
	// However you may not want this for the error state.
	// You must explicitly set this state when you create a state
	bool needs_to_backtrack;
	// the token type for the lexeme it accepts
	token_type token_type;
};

class dfa {
public:
	dfa();
	~dfa();
	// constant identifier to be used in the state transition table to use when no onther lookup is there
	const static std::string ELSE_TRANSITION;
	// Create the starting state for this dfa
	state* create_start_state();
	// Create the a state to be used in this dfa
	state* create_state(bool is_final_state=false, bool needs_to_backtrack=false, token_type type=non_token);
	// add a transition from the from state to the to_state using a transition string
	bool add_transition(state* from_state, std::string transition, state* to_state);
	// Add an else transition to the two states
	bool add_else_transition(state* from_state, state* to_state);
	// Get the transition state from the from_state identifier using lookup_transition input
	state* table(int from_state, std::string lookup_transition);
private:
	// the count for how many states we have
	int m_state_count;
	// Check to see if the dfa has the current state
	bool has_state(state state);
	// the data structure that stores the state transition table
	// where the key to this map is the state identifier
	// the value to for the specified key, is the transition map for that state
	std::unordered_map<int, std::shared_ptr<std::unordered_map<std::string, state*>>> state_transition_table;
	std::vector<state*> states;
};

#endif

