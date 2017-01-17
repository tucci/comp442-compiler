#ifndef STATE_H
#define STATE_H

#include <unordered_map>
class state {
public:
	state();
	state(bool is_start_state, bool is_final_state);
	// Adds this transition and returns true if successful, false if the transition already exists
	bool add_transition_to(std::string transition, state to_state);
	// Returns true if this transition already exists
	bool has_transition(std::string transition);
	// Gets the state from this transition. Throws out_of_range excpection if transition is not found
	state get_state_from_transition(std::string transition);
private:
	static int state_count;
	int state_identifier;
	bool is_start_state;
	bool is_final_state;
	std::unordered_map<std::string, state> transitions;
};


#endif