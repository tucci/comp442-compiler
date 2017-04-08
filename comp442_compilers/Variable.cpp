#include "stdafx.h"


Variable::Variable() {
	isFuncCall = false;
	location = -1;
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
	if (isFuncCall) {
		varName.append("(");
		if (arguments.size() != 0) {
			for (std::vector<Expression>::value_type expr: arguments) {
				varName.append(expr.type.toString()  + ", ");
			}
			
			
		}
		varName.append(")");

	}

	return varName;
}


