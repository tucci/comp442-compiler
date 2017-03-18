#ifndef EXPRESSION_H
#define EXPRESSION_H

// A fragment of an expression 
// Ex: if we have an expression student.parent[0].name()
// student, parent, 0, name would all fragments









struct ExpressionFragment; 

class Expression {	
public:
	Expression();
	// Builds this expression from a full name string
	// ex builds an expression object from the string student.parent[1].name()
	~Expression();
	void addVar(std::string id);
	void addIndice(Expression subExpr);
	void addNumeric(std::string numeric);
	void addOperator(std::string op);
	void setFunc(bool isFunc);
	std::string toFullName();
	const std::vector<ExpressionFragment>& getFragments();
	bool isFunction();
private:
	std::vector<ExpressionFragment> fragments;
	bool isFunc;
	
};



enum ExpressionFragmentType {
	fragment_var,
	fragment_indice,
	fragment_numeric,
	fragment_operator
};

struct ExpressionFragment {
	ExpressionFragmentType type;
	Expression indiceValue;
	Variable var;
	std::string numericValue;
	std::string operatorValue;
};


#endif