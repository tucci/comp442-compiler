#ifndef DFA_H
#define DFA_H

#include <unordered_map>
#include "token.h"
#include "state.h"



class dfa {
public:
	dfa();
	~dfa();

	//bool add_transition(state from_state, std::string transition, state to_state);
	bool test_string(std::string);

private:
	state m_start_state;
	// TODO: implement state transition has this
	// TODO: implmennt some way to track backtrackking and final tokens
	//std::unordered_map<state, std::unordered_map<token, state>> state_transition_table;
	std::unordered_map<int, std::unordered_map<std::string, state>> state_transition_table;
};

#endif // !FA_AUTOMATON_H

