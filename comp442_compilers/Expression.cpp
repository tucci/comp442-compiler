#include "stdafx.h"
#include "Expression.h"


Expression::Expression() {
}



Expression::~Expression() {
}

void Expression::addVar(Variable var) {
	ExpressionFragment exprFragment;
	exprFragment.type = ExpressionFragmentType::fragment_var;
	exprFragment.var = var;
	fragments.push_back(exprFragment);

}

void Expression::addNumeric(std::string numeric) {
	ExpressionFragment fragment;
	fragment.type = ExpressionFragmentType::fragment_numeric;
	fragment.numericValue = numeric;
	fragments.push_back(fragment);
}

void Expression::addOperator(std::string op) {
	ExpressionFragment fragment;
	fragment.type = ExpressionFragmentType::fragment_operator;
	fragment.operatorValue = op;
	fragments.push_back(fragment);
}



std::string Expression::toFullName() {
	std::string fullName;
	for (int i = 0; i < fragments.size(); ++i) {
		ExpressionFragment fragment = fragments[i];
		
		if (fragment.type == ExpressionFragmentType::fragment_var) {
			if (i != 0 && fragments.size() > 0 && fragments[i - 1].type != ExpressionFragmentType::fragment_operator) {
				fullName.append(".");
			}
			fullName.append(fragment.var.toFullName());
		} else if (fragment.type == ExpressionFragmentType::fragment_numeric) {
			fullName.append(fragment.numericValue);
		} else if (fragment.type == ExpressionFragmentType::fragment_operator) {
			fullName.append(fragment.operatorValue);
		}
		
	}
	return fullName;
}

const std::vector<ExpressionFragment>& Expression::getFragments() {
	return fragments;
}


Variable::Variable() {
	isFunc = false;
}

Variable::~Variable() {
}

std::string Variable::toFullName() {
	std::string varName;
	for (int i = 0; i < vars.size(); ++i) {
		VariableFragment frag = vars[i];

		if (i != 0) {
			varName.append(".");
		} 
		varName.append(frag.identifier);
		
		for (Expression& indice : frag.indices) {
			varName.append("[" + indice.toFullName() + "]");
		}
		
	}
	if (isFunc) {
		varName.append("()");
	}
	
	return varName;
}


