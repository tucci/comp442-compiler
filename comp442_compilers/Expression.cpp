#include "stdafx.h"
#include "Expression.h"



std::string ValueExpressionNode::toString() {
	if (this->value.type == ExpressionFragmentType::fragment_numeric) {
		return value.numericValue;
	} else if (this->value.type == ExpressionFragmentType::fragment_var) {
		return value.var.toFullName();
	}
}

std::string OperatorExpressionNode::toString() {
	return this->operatorValue.operatorValue;
}

Expression::Expression() {
}



Expression::~Expression() {
}

void Expression::addVar(Variable var) {
	// Create a fragment and add the sign and var
	ExpressionFragment fragment;
	fragment.sign = signToAddToFragment;
	fragment.type = ExpressionFragmentType::fragment_var;
	fragment.var = var;
	postfixExpression.push_back(fragment);
	// Reset sign for the next time
	signToAddToFragment = sign_plus;

}

void Expression::setSign(std::string sign) {
	if (sign == "-") {
		signToAddToFragment = sign_minus;
	} else {
		signToAddToFragment = sign_plus;
	}
}

void Expression::addNumeric(std::string numeric) {
	// Create a fragment and add the sign and the numreric literal
	ExpressionFragment fragment;
	fragment.sign = signToAddToFragment;
	fragment.type = ExpressionFragmentType::fragment_numeric;
	fragment.numericValue = numeric;
	postfixExpression.push_back(fragment);
	// Reset sign for the next time
	signToAddToFragment = sign_plus;

}

void Expression::addOperator(std::string op) {
	// Create a fragment and add the sign and the operator
	ExpressionFragment fragment;
	fragment.type = ExpressionFragmentType::fragment_operator;
	fragment.operatorValue = op;
	
	// Start pushing to the expression stack and postfix expression
	if (expressionStack.empty() || expressionStack.back().operatorValue == "(") {
		expressionStack.push_back(fragment);
	} else {
		while (!expressionStack.empty() && expressionStack.back().operatorValue != "(" && precedenceOf(op) <= precedenceOf(expressionStack.back().operatorValue)) {
			ExpressionFragment popped = expressionStack.back();
			expressionStack.pop_back();
			postfixExpression.push_back(popped);
		}
		// push operator onto the stack
		expressionStack.push_back(fragment);
	}
}

void Expression::addParen(std::string paren) {
	ExpressionFragment fragment;
	fragment.operatorValue = paren;
	if (paren == "(") {
		fragment.type = ExpressionFragmentType::fragment_left_paren;
		expressionStack.push_back(fragment);
		
	} else if (paren == ")") {
		while (!expressionStack.empty() && expressionStack.back().operatorValue != "(") {
			ExpressionFragment popped = expressionStack.back();
			expressionStack.pop_back();
			postfixExpression.push_back(popped);
		}
		// pop the left paren
		expressionStack.pop_back();
	}
}



void Expression::buildExpressionTree() {
	// Convert our internal postfix expression to a tree
	// the output of the tree will be the root
	while (!expressionStack.empty()) {
		ExpressionFragment popped = expressionStack.back();
		expressionStack.pop_back();
		postfixExpression.push_back(popped);
	}

	std::vector<std::shared_ptr<ExpressionElementNode>> stack;
	for (ExpressionFragment fragment : postfixExpression) {
		if (fragment.type == fragment_var || fragment.type == fragment_numeric) {
			std::shared_ptr<ValueExpressionNode> valueNode = std::make_shared<ValueExpressionNode>();
			valueNode->value = fragment;

			if (fragment.type == fragment_numeric) {
				// either int or float
				if (isFloat(fragment.numericValue)) {
					TypeStruct valueTypeStruct;
					valueTypeStruct.type = SymbolType::type_float;
					valueTypeStruct.structure = SymbolStructure::struct_simple;
					valueNode->valueType = valueTypeStruct;
				} else {
					TypeStruct valueTypeStruct;
					valueTypeStruct.type = SymbolType::type_int;
					valueTypeStruct.structure = SymbolStructure::struct_simple;
					valueNode->valueType = valueTypeStruct;
				}
			}
			else if (fragment.type == fragment_var) {
				valueNode->valueType = fragment.var.varType;
			}

			
			stack.push_back(valueNode);
		} else if (fragment.type == fragment_operator) {
			std::shared_ptr<ExpressionElementNode> right = stack.back();
			stack.pop_back();
			std::shared_ptr<ExpressionElementNode> left = stack.back();
			stack.pop_back();
			std::shared_ptr<OperatorExpressionNode> operatorNode = std::make_shared<OperatorExpressionNode>();
			operatorNode->operatorValue = fragment;
			operatorNode->left = left;
			operatorNode->right = right;
			stack.push_back(operatorNode);
		}
	}
	root = stack.back();
	stack.pop_back();
	
}

std::pair<bool, TypeStruct> Expression::typeCheckExpression() {
	// go through the tree and make sure all the types are the same
	NodeType nt = root->nodeType;
	if (nt == node_operator) {
		return typeCheckExpression(root);
	} else if (nt == node_value) {
		// this is a single node tree
		std::shared_ptr<ValueExpressionNode> valueNode = std::static_pointer_cast<ValueExpressionNode>(root);
		return std::make_pair(true, valueNode->valueType);
		

	}
}


std::string Expression::toFullName() {
	std::string fullName;
	toFullName(fullName, root);
	return fullName;
}



int Expression::precedenceOf(std::string op) {
	TokenType opType = Specification::TOKEN_MAP.at(op);
	switch (opType) {
	case multop: return 6; 
	case divop: return 6;
	case addop: return 5;
	case subtractop: return 5;
	case or : return 5;
	case and: return 5;
	case gt: return 4;
	case greateq: return 4;
	case lt: return 4;
	case lesseq: return 4;
	case comparison: return 3;
	case noteq: return 3;
	}


}

std::pair<bool, TypeStruct> Expression::typeCheckExpression(std::shared_ptr<ExpressionElementNode> root) {
	NodeType nt = root->nodeType;
	if (nt == node_operator) {
		std::shared_ptr<OperatorExpressionNode> operatorNode = std::static_pointer_cast<OperatorExpressionNode>(root);
		std::shared_ptr<ExpressionElementNode> left = operatorNode->left;
		std::shared_ptr<ExpressionElementNode> right = operatorNode->right;

		std::pair<bool, TypeStruct> leftPair = typeCheckExpression(left);
		std::pair<bool, TypeStruct> rightPair = typeCheckExpression(right);
		if (leftPair.first && rightPair.first && leftPair.second == rightPair.second) {
			return std::make_pair(true, leftPair.second);
		} else {
			TypeStruct valueTypeStruct;
			valueTypeStruct.type = SymbolType::type_none;
			valueTypeStruct.structure = SymbolStructure::struct_simple;
			return std::make_pair(false, valueTypeStruct);
		}

	} else if (nt == node_value) {
		// this is a single node tree
		std::shared_ptr<ValueExpressionNode> valueNode = std::static_pointer_cast<ValueExpressionNode>(root);
		return std::make_pair(true, valueNode->valueType);


	}
}

void Expression::toFullName(std::string& str, std::shared_ptr<ExpressionElementNode> root) {
	if (root != NULL) {
		if (root->nodeType == NodeType::node_operator) {
			str += "(";
			std::shared_ptr<OperatorExpressionNode> operatorNode = std::static_pointer_cast<OperatorExpressionNode>(root);
			toFullName(str, operatorNode->left);
			str += operatorNode->toString();
			toFullName(str, operatorNode->right);
			str += ")";
		} else if (root->nodeType == NodeType::node_value) {
			std::shared_ptr<ValueExpressionNode> valueNode = std::static_pointer_cast<ValueExpressionNode>(root);
			str += valueNode->toString();
		}
		
	}
	
}


