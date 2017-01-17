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

state dfa::create_state(bool is_final_state, bool is_backup) {
	// init state with the incremented state identifier and specified is_final_state
	state new_state = { ++m_state_count, false, is_final_state, is_backup};
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

bool dfa::add_else_transition(state from_state, state to_state) {
	return add_transition(from_state, ELSE_TRANSITION, to_state);
}

bool  dfa::has_state(state state) {
	return state_transition_table.find(state.state_identifier) != state_transition_table.end();
}

state dfa::table(int state, std::string lookup_transition) {
	return state_transition_table.at(state)->at(lookup_transition);
}

//bool dfa::test_string(std::string input) {
//	bool is_accepted = false;
//	int str_index = 0;
//	// reference to the current str
//	std::string current_str;
//	// reference to the current state
//	state current_state = m_start_state;
//
//	while (!is_accepted && str_index < input.length()) {
//		current_str = input.substr(str_index, 1);
//		state transition_state;
//		try { // try to see if the current state has a transition from thr current_str string
//			transition_state = current_state.transitions.at(current_str);
//			// if no exception has been thrown, then there is a transition
//			// set the current state to the transition state
//			current_state = transition_state;
//			// move the string index to read the next char
//			str_index++;
//			// Check to see if we are the final state and have read all the input
//			if (current_state.is_final_state && str_index == input.length()) {
//				return true;
//			}
//		} catch (const std::out_of_range& oor) {
//			// Didn't find the transition. therefore we reject this string
//			return false;
//		}
//	}
//	return false;
//}