#ifndef SPECIFICATION_H
#define SPECIFICATION_H


// This class is specifically used to set the language specs
// We set the specification by settings the specs directly into the dfa
class specification {
public:
	specification();
	~specification();
	// Returns the dfa for this specification
	dfa get_spec();
private:
	// The dfa/state transition table for this spec
	dfa spec;
};

#endif // !SPECIFICATION_H