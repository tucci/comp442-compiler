#include "stdafx.h"
#include "dfa.h"


dfa::dfa() {
}


dfa::~dfa() {

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