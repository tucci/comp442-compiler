#include "stdafx.h"
#include "AssignStatement.h"
#include "Instructions.h"


AssignStatement::AssignStatement() {
}



AssignStatement::~AssignStatement() {
}

std::string AssignStatement::_toMoonCode() {
	std::string instrs;

	// Get a register that we can use
	Register ir = generator->getUnusedRegister();

	ExpressionElementNode* exprRoot = rhs.root.get();
	if (exprRoot->nodeType == node_value) {
		
		// This is a single node expression
		ValueExpressionNode* valueNode = static_cast<ValueExpressionNode*>(exprRoot);
		// Check to see if this is a simple value or var
		if (valueNode->value.type == fragment_numeric) {
			// This is a simple numeric value
			instrs.append(SubtractInstruction(ir, ir, ir)._toMoonCode());
			instrs.append(AddImmediateInstruction(ir, ir, valueNode->value.numericValue).setComment("load " + valueNode->value.numericValue + " into register"). _toMoonCode());
		} else if (valueNode->value.type == fragment_var) {
			Variable var = valueNode->value.var;
			if (var.isFunc) {
				// TODO: implement function calls
				
			} else {
				// This is a variable that needs to be loaded
				instrs.append(LoadWordInstruction(ir, r0, valueNode->value.var.record->label).setComment("load " + valueNode->value.var.toFullName() + " into register")._toMoonCode());
			}
		}
	} else {
		// This is a full expression that needs to be evaluated
		// TODO: implement expression evals

	}

	Register resultRegister = generator->getUnusedRegister();
	

	instrs.append(StoreWordInstruction(r0, ir, var.record->label).setComment(var.record->name + " assignment operation")._toMoonCode());
	instrs.append(ClearRegisterInstruction(ir)._toMoonCode());
	generator->freeRegister(ir);
	// Clear the r1 register
	return instrs;

}
