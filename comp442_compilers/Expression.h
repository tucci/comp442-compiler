#ifndef EXPRESSION_H
#define EXPRESSION_H



class Variable;
struct ExpressionFragment; 

// TODO: we might want to re implement as a tree when we do the code generation
class Expression {	
public:

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


enum ExpressionFragmentType {
	fragment_var,
	fragment_numeric,
	fragment_operator
};

struct ExpressionFragment {
	ExpressionFragmentType type;
	Variable var;
	std::string numericValue;
	std::string operatorValue;
};


#endif