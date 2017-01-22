#include "stdafx.h"

const std::string dfa::ELSE_TRANSITION = "ELSE_TRANSITION";

dfa::dfa() {
	m_state_count = 0;
}

dfa::~dfa() {
	std::cout << "delete dfa" << std::endl;

	// Delete states
	for (std::vector<state*>::iterator it = states.begin(); it != states.end(); ++it) {
		delete *it;
		*it = NULL;
	}
		
}

state* dfa::create_start_state() {
	// create as it were a normal state
	state* start_state = create_state(false);
	// set this state to a start state
	start_state->is_start_state = true;
	return start_state;
}

state* dfa::create_state(bool is_final_state, bool is_backup, token_type type) {
	// init state with the incremented state identifier and specified is_final_state
	state* new_state = new state{ ++m_state_count, false, is_final_state, is_backup, type };
	states.push_back(new_state);
	// create the transition map for this state
	std::shared_ptr<std::unordered_map<std::string, state*>> transitions = std::shared_ptr<std::unordered_map<std::string, state*>>(new std::unordered_map<std::string, state*>);
	// add the transition map to our state transition table
	state_transition_table.emplace(new_state->state_identifier, transitions);
	return new_state;
}

bool dfa::add_transition(state* from_state, std::string transition, state* to_state) {
	// We need to check if these states are in the table
	if (has_state(*from_state) && has_state(*to_state)) {
		// Get this transitions for the from state
		std::shared_ptr<std::unordered_map<std::string, state*>> state_transitions = state_transition_table.at(from_state->state_identifier);
		state_transitions->emplace(transition, to_state);
		return true;
	}
	return false;
}

bool dfa::add_else_transition(state* from_state, state* to_state) {
	// We can either set states to be backup states when we create the state
	// Or we can automatically do it when we create the else transitions/
	// However doing it when we create the else state makes it prone to create un wanted bugs
	// So create backup states explicity when creating the states
	// For now this code is commented in case of anything
	//if (to_state->token_type != token_type::error_token) {
	//	to_state->is_backup = true;
	//}
	return add_transition(from_state, ELSE_TRANSITION, to_state);
}

bool dfa::has_state(state state) {
	return state_transition_table.find(state.state_identifier) != state_transition_table.end();
}

state* dfa::table(int from_state, std::string lookup_transition) {
	std::shared_ptr<std::unordered_map<std::string, state*>> state_transitions = state_transition_table.at(from_state);
	std::unordered_map<std::string, state*>::iterator found = state_transitions->find(lookup_transition);
	// If we dont have a transition for this state, go the else state
	if (found == state_transitions->end()) {
		return NULL;
	}
	// if we did find something, then go that transition state
	return found->second;
}

