#ifndef EXPRESSION_H
#define EXPRESSION_H



enum ExpressionSign { sign_plus, sign_minus };
enum ExpressionFragmentType { fragment_var, fragment_numeric, fragment_operator, fragment_left_paren, fragment_right_paren };


struct ExpressionFragment {
	ExpressionFragmentType type;
	Variable var;
	ExpressionSign sign;
	std::string numericValue;
	std::string operatorValue;
};

enum NodeType { node_value, node_operator };

class ExpressionElementNode {
public:
	NodeType nodeType;
	ExpressionElementNode(NodeType type) : nodeType(type){};
	
};


class ValueExpressionNode : public ExpressionElementNode {
public:
	ValueExpressionNode() : ExpressionElementNode(node_value) {}
	ExpressionFragment value;
	TypeStruct valueType;
	std::string toString();
};

class OperatorExpressionNode : public ExpressionElementNode {
public:
	OperatorExpressionNode() : ExpressionElementNode(node_operator) {}
	ExpressionFragment operatorValue;
	std::shared_ptr<ExpressionElementNode> left;
	std::shared_ptr<ExpressionElementNode> right;
	std::string toString();
	
};



class Expression {	
public:

	Expression();
	~Expression();
	void addVar(Variable var);
	void setSign(std::string sign);
	void addNumeric(std::string numeric);
	void addOperator(std::string op);
	void addParen(std::string paren);
	void buildExpressionTree();
	std::pair<bool, TypeStruct> typeCheckExpression();
	std::string toFullName();
	std::shared_ptr<ExpressionElementNode> root;
	TypeStruct type;
private:
	ExpressionSign signToAddToFragment; // Used to add to the current expression we are building
	std::vector<ExpressionFragment> expressionStack;
	std::vector<ExpressionFragment> postfixExpression;
	static int precedenceOf(std::string op);
	std::pair<bool, TypeStruct> typeCheckExpression(std::shared_ptr<ExpressionElementNode> root);
	void toFullName(std::string& str, std::shared_ptr<ExpressionElementNode> root);

};









#endif