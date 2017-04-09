#include "stdafx.h"
#include "AssignStatement.h"
#include "Instructions.h"


AssignStatement::AssignStatement() {
}



AssignStatement::~AssignStatement() {
}

std::string AssignStatement::_toMoonCode() {
	std::string instrs;
	
	// expression that needs to be evaluated	
	ExpressionEvalulationInstruction rhsInstr(generator, rhs);
	instrs.append(rhsInstr._toMoonCode());
	if (var.varType.structure == struct_simple) {
		instrs.append(StoreWordInstruction(r0, rhsInstr.outputRegister, var.record->label).setComment(var.record->name + " assignment operation")._toMoonCode());
	} else if (var.varType.structure == struct_array) {
		// Since this is an array, we want to just calculate the size of the type and not the actual array
		TypeStruct stripedType = var.varType;
		stripedType.structure = struct_simple;
				
		Register offsetRegister = generator->getUnusedRegister();
		// Make sure our sum register is 0
		instrs.append(ClearRegisterInstruction(offsetRegister)._toMoonCode());
		Register multRegister = generator->getUnusedRegister();
		int typeSize = SymbolTable::_sizeOf(generator->globalTable, stripedType);
		int dimBlockSize;
		int offset = 0;
		for (int dim = 0 ; dim < var.varType.dimensions.size(); dim++) {
			int nextDimSize = 1; 
			if (dim < var.varType.dimensions.size() - 2) {
				nextDimSize = var.record->typeStructure.dimensions[dim + 2];
			}
			if (dim < var.varType.dimensions.size() - 1) {
				dimBlockSize = var.record->typeStructure.dimensions[dim + 1] * nextDimSize;
			} else {
				dimBlockSize = 1;
			}
			ExpressionEvalulationInstruction indexExpr(generator, var.vars.back().indices[dim]);
			instrs.append(indexExpr._toMoonCode());
			instrs.append(MultiplyImmediateInstruction(multRegister, indexExpr.outputRegister, std::to_string(typeSize * dimBlockSize))._toMoonCode());
			instrs.append(AddInstruction(offsetRegister, offsetRegister, multRegister)._toMoonCode());
			generator->freeRegister(indexExpr.outputRegister);
		};
		generator->freeRegister(multRegister);		
		instrs.append(StoreWordInstruction(offsetRegister, rhsInstr.outputRegister, var.record->label).setComment(var.record->name + " assignment operation")._toMoonCode());

		generator->freeRegister(offsetRegister);
	}
	
	generator->freeRegister(rhsInstr.outputRegister);

	return instrs;

}
