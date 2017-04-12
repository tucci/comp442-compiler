#ifndef EXPRESSION_H
#define EXPRESSION_H


// the sign of this expression +/-
enum ExpressionSign { sign_plus, sign_minus };
// The type of expression this fragment is
enum ExpressionFragmentType { fragment_var, fragment_numeric, fragment_operator, fragment_left_paren, fragment_right_paren };


// A expression fragment is a piece of the entire expression
// for example 5 + 7 + b, all are fragments
// a fragment coule be a variable/function or it could be a operator, or a literal numeric value
struct ExpressionFragment {
	ExpressionFragmentType type;
	Variable var;
	ExpressionSign sign;
	std::string numericValue;
	std::string operatorValue;
};

// The type of node in the expression tree
// a node could either be a value or an operator
enum NodeType { node_value, node_operator };

// A node in our expression tree
class ExpressionElementNode {
public:
	NodeType nodeType;
	int registerCount; // used when building the instructions for the registers
	ExpressionElementNode(NodeType type) : nodeType(type){};
	
};

// A value expression node, is a node that holds a value such as a var/number fragment
class ValueExpressionNode : public ExpressionElementNode {
public:
	ValueExpressionNode() : ExpressionElementNode(node_value) {}
	ExpressionFragment value;
	TypeStruct valueType;
	std::string toString();
};

// An operator expression node is a node that holds an operator such as + - * / < > <= etc
// Operator nodes can also hold relational operators
class OperatorExpressionNode : public ExpressionElementNode {
public:
	OperatorExpressionNode() : ExpressionElementNode(node_operator) {}
	ExpressionFragment operatorValue;
	// the left subtree
	std::shared_ptr<ExpressionElementNode> left;
	// the right subtree
	std::shared_ptr<ExpressionElementNode> right;
	std::string toString();
	
};

// an intermediate representation of an expression in the high level code
class Expression {	
public:

	Expression();
	~Expression();
	// add a variable to our expression
	void addVar(Variable var);
	// sets the sign to our expression
	void setSign(std::string sign);
	// adds a numeric literal to our expression
	void addNumeric(std::string numeric);
	// adds a operator to our expression
	void addOperator(std::string op);
	// sets a left/right paren "(" or ")"
	void addParen(std::string paren);
	// Builds the inner expression tree 
	// the output of the tree is the root object of this object
	// ex 
	// expr.buildExpressionTree();
	// expr.root;

	void buildExpressionTree();
	// Type checks the expression to see if everything is the same type
	// Returns a pair, where the first value tells you if the expression is of the same type
	// if true, then the expression is typed checked and everything is the same value, false otherwise
	// the second value returns you the type of this expression
	std::pair<bool, TypeStruct> typeCheckExpression();
	// Returns the expression back in infix notation
	std::string toFullName();
	// the root of this expression tree.
	// This is created by calling buildExpressionTree()
	std::shared_ptr<ExpressionElementNode> root;
	// The end type of the expression
	TypeStruct type;
private:
	// Used to add to the current expression we are building
	ExpressionSign signToAddToFragment; 
	// As we add var/nums/operators, the postfix expression is being built
	// Once we are done with the expression
	// call buildExpressionTree() to build it from the postfix expression
	// the expression stack needed to build up the tree
	std::vector<ExpressionFragment> expressionStack; 
	// the postfix needed to build up the tree
	std::vector<ExpressionFragment> postfixExpression; 
	// returns the precedence of the given operator
	// high number means higher precedence
	static int precedenceOf(std::string op); 
	// Internal recursive method
	std::pair<bool, TypeStruct> typeCheckExpression(std::shared_ptr<ExpressionElementNode> root);
	// Internal recursive method
	void toFullName(std::string& str, std::shared_ptr<ExpressionElementNode> root);

};









#endif