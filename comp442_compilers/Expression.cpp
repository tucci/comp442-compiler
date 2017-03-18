#include "stdafx.h"
#include "Expression.h"


Expression::Expression() {
	isFunc = false;
}



Expression::~Expression() {
}

void Expression::addVar(std::string id) {
	ExpressionFragment fragment;
	fragment.type = ExpressionFragmentType::fragment_var;
	fragment.var.identifier = id;
	fragments.push_back(fragment);
}

void Expression::addIndice(Expression subExpr) {
	ExpressionFragment fragment;
	fragment.type = ExpressionFragmentType::fragment_indice;
	fragment.indiceValue = subExpr;
	fragments.push_back(fragment);
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


void Expression::setFunc(bool isFunc) {
	this->isFunc = isFunc;
}

std::string Expression::toFullName() {
	std::string fullName;
	for (int i = 0; i < fragments.size(); ++i) {
		ExpressionFragment fragment = fragments[i];
		if (i == 0) {
			if (fragment.type == ExpressionFragmentType::fragment_var) {
				fullName.append(fragment.var.identifier);
			}
			else if (fragment.type == ExpressionFragmentType::fragment_numeric) {
				fullName.append(fragment.numericValue);
			}
		} else {
			if (fragment.type == ExpressionFragmentType::fragment_var) {
				fullName.append("." + fragment.var.identifier);
			}
			else if (fragment.type == ExpressionFragmentType::fragment_indice) {
				fullName.append("[" + fragment.indiceValue.toFullName() + "]");
			}
		}
	}
	if (isFunc) {
		fullName.append("()");
	}
	return fullName;
}

const std::vector<ExpressionFragment>& Expression::getFragments() {
	return fragments;
}

bool Expression::isFunction() {
	return isFunc;
}


