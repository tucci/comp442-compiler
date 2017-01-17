#include "stdafx.h"
#include "dfa.h"



const std::string dfa::ELSE_TRANSITION = "ELSE_TRANSITION";

dfa::dfa() {
	m_state_count = 0;
}

dfa::~dfa() {
}

state dfa::create_start_state() {
	// create as it were a normal state
	state start_state = create_state(false);
	// set this state to a start state
	start_state.is_start_state = true;
	return start_state;
}

state dfa::create_state(bool is_final_state, bool is_backup, token_type type) {
	// init state with the incremented state identifier and specified is_final_state
	state new_state = { ++m_state_count, false, is_final_state, is_backup, type};
	// create the transition map for this state
	std::unordered_map<std::string, state>* transitions = new std::unordered_map<std::string, state>;
	// add the transition map to our state transition table
	state_transition_table.emplace(new_state.state_identifier, transitions);
	return new_state;
}

bool dfa::add_transition(state from_state, std::string transition, state to_state) {
	// We need to check if these states are in the table
	if (has_state(from_state) && has_state(to_state)) {
		// Get this transitions for the from state
		std::unordered_map<std::string, state>* state_transitions = state_transition_table.at(from_state.state_identifier);
		state_transitions->emplace(transition, to_state);
		return true;
	}
	return false;
}
// Add an else transition to the two states
bool dfa::add_else_transition(state from_state, state to_state) {
	return add_transition(from_state, ELSE_TRANSITION, to_state);
}

bool dfa::has_state(state state) {
	return state_transition_table.find(state.state_identifier) != state_transition_table.end();
}

state dfa::table(int from_state, std::string lookup_transition) {
	return state_transition_table.at(from_state)->at(lookup_transition);
}
