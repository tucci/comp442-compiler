#ifndef VARIABLE_H
#define VARIABLE_H

class Expression;

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
	TypeStruct varType;
	int location;
	bool isFunc;
	FunctionData functionData;
};

#endif
