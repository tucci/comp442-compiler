#include "stdafx.h"


Variable::Variable() {
	isFunc = false;
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
	if (isFunc) {
		varName.append("(");
		if (arguments.size() != 0) {
			for (std::vector<TypeStruct>::value_type type : arguments) {
				varName.append(type.toString()  + ", ");
			}
			
			
		}
		varName.append(")");

	}

	return varName;
}


