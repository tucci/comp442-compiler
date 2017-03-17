#ifndef EXPRESSION_H
#define EXPRESSION_H

// A fragment of an expression 
// Ex: if we have an expression student.parent[0].name()
// student, parent, 0, name would all fragments
struct ExpressionFragment {
	bool isId;
	bool isIndice;
	std::string value;
};

class Expression {
public:
	Expression();
	// Builds this expression from a full name string
	// ex builds an expression object from the string student.parent[1].name()
	Expression(std::string fullName);
	~Expression();
	void addIdNest(std::string id);
	void addIndice(std::string id);
	void setFunc(bool isFunc);
	std::string toFullName();
	const std::vector<ExpressionFragment>& getFragments();
	bool isFunction();

private:
	std::vector<ExpressionFragment> fragments;
	bool isFunc;
	
};


#endif