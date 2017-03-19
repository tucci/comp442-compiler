#ifndef EXPRESSION_H
#define EXPRESSION_H

// A fragment of an expression 
// Ex: if we have an expression student.parent[0].name()
// student, parent, 0, name would all fragments


class Variable;
struct ExpressionFragment; 

class Expression {	
public:// Builds this expression from a full name string
	// ex builds an expression object from the string student.parent[1].name()
	Expression();
	~Expression();
	void addVar(Variable var);
	void addNumeric(std::string numeric);
	void addOperator(std::string op);
	std::string toFullName();
	const std::vector<ExpressionFragment>& getFragments();
private:
	std::vector<ExpressionFragment> fragments;	
};



enum ExpressionFragmentType {
	fragment_var,
	fragment_numeric,
	fragment_operator
};


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
	int location;
	bool isFunc;

};

struct ExpressionFragment {
	ExpressionFragmentType type;
	Variable var;
	std::string numericValue;
	std::string operatorValue;
};


#endif