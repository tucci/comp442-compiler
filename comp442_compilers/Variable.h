#ifndef VARIABLE_H
#define VARIABLE_H


class Expression;
class SymbolTableRecord;

struct VariableFragment {
	std::string identifier;
	std::vector<Expression> indices;
};



class Variable {
public:
	Variable();
	~Variable();
	std::vector<VariableFragment> vars;
	std::string toFullName();
	TypeStruct varType; // If this var turns into a function, then this value is the return type of this function
	SymbolTableRecord* record;
	int location;
	bool isFunc;
	std::vector<Expression> arguments;
};

#endif
