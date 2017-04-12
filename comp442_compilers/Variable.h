#ifndef VARIABLE_H
#define VARIABLE_H

// Forward declare
class Expression;
class SymbolTableRecord;

// a fragment/part of this variable
// example obj.a.b.c[32].d()
// obj, a, b, c[32], d are fragments
// the indices are attacahed to each fragment
struct VariableFragment {
	// the identifier of this fragment
	std::string identifier;
	// the inner exppression indices
	// example a[32][b*c]
	// 32 would be the first expression index
	// b * c would be the second expression index
	std::vector<Expression> indices;
};


// an intermediate representation for a variable
class Variable {
public:
	Variable();
	~Variable();
	// the individual fragments that build this var
	std::vector<VariableFragment> vars;
	// the readable name for this var
	std::string toFullName();
	// The type of this var
	// If this var turns into a function, then this value is the return type of this function
	TypeStruct varType; 
	// The linked record in the symbol table for this var
	SymbolTableRecord* record;
	// the location index of this var in the source code
	int location;
	// Whether this a function call
	bool isFuncCall;
	// if it is a function call
	// then the list of arguments for this function call in order
	std::vector<Expression> arguments;
};

#endif
