#include "stdafx.h"
#include "state.h"

int state::state_count = 0;

state::state()  {
	// TODO check what compiler version the school uses
	// could use constructor delegation here in c++11
	this->is_start_state = false;
	this->is_final_state = false;
	state_count++;
	state_identifier = state_count;
}

state::state(bool is_start_state, bool is_final_state) {
	this->is_start_state = is_start_state;
	this->is_final_state = is_final_state;
	state_count++;
	state_identifier = state_count;
}

// Adds this transition and returns true if successful, false if the transition already exists
bool state::add_transition_to(std::string transition, state to_state) {
	// Check to see if this transition already exists
	if (has_transition(transition)) {
		// this transition already exists and we cannot add two transitions of the same input
		// this will make it an nfa which we don't want
		return false;
	}
	// add this transiton to the out going transitions
	transitions.emplace(transition, to_state);
	return true;
};

// Returns true if this transition already exists
bool state::has_transition(std::string transition) {
	return transitions.find(transition) != transitions.end();
}

// Gets the state from this transition. Throws out_of_range excpection if transition is not found
state state::get_state_from_transition(std::string transition) {
	return transitions.at(transition);
}