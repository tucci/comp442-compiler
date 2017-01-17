#ifndef DFA_H
#define DFA_H

#include <unordered_map>
#include "token.h"


struct state {
	int state_identifier;
	bool is_start_state;
	bool is_final_state;
	bool is_backup;
};

class dfa {
public:
	dfa();
	~dfa();
	state create_start_state();
	state create_state(bool is_final_state=false, bool is_backup=false);
	bool add_transition(state from_state, std::string transition, state to_state);
	bool add_else_transition(state from_state, state to_state);
	state table(int state, std::string lookup_transition);
	//bool test_string(std::string);

private:
	const static std::string ELSE_TRANSITION;
	int m_state_count;
	bool has_state(state state);
	// TODO: implement state transition has this
	// TODO: implmennt some way to track backtrackking and final tokens
	//std::unordered_map<state, std::unordered_map<token, state>> state_transition_table;
	std::unordered_map<int, std::unordered_map<std::string, state>*> state_transition_table;
};

#endif // !FA_AUTOMATON_H

