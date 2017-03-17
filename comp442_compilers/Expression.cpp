#include "stdafx.h"
#include "Expression.h"


Expression::Expression() {
}

Expression::Expression(std::string fullName) {
	// TODO: implment this, or do we even need this
}


Expression::~Expression() {
}

void Expression::addIdNest(std::string id) {
	fragments.push_back({ true, false, id });
}

void Expression::addIndice(std::string id) {
	fragments.push_back({ false, true, id });
}

void Expression::setFunc(bool isFunc) {
	this->isFunc = isFunc;
}

std::string Expression::toFullName() {
	std::string fullName;
	for (int i = 0; i < fragments.size(); ++i) {
		ExpressionFragment fragment = fragments[i];
		if (i == 0) {
			fullName.append(fragment.value);
		} else {
			if (fragment.isId) {
				fullName.append("." + fragment.value);
			}
			if (fragment.isIndice) {
				fullName.append("[" + fragment.value + "]");
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

