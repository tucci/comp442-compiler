#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "Instruction.h"
#include <string>
#include "Register.h"



/* -------------------------- Data access instructions  ------------------------------- */

class LoadWordInstruction : public Instruction {
public:
	Register ri;
	Register rj;
	std::string offset;
	// Creates "lw	ri,offset(rj)" instruction
	LoadWordInstruction(Register ri, Register rj, std::string offset) {
		this->ri = ri;
		this->rj = rj;
		this->offset = offset;
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("lw\t" + registerToString(ri) + "," + offset + "(" + registerToString(rj) + ")");
	};
};

class StoreWordInstruction : public Instruction {
public:
	Register rj;
	Register ri;
	std::string offset;
	// Creates "sw	offset(rj),ri" instruction
	StoreWordInstruction(Register rj, Register ri, std::string offset) {
		this->rj = rj;
		this->ri = ri;
		this->offset = offset;
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("sw\t" + offset + "(" + registerToString(rj) + ")," + registerToString(ri));	
	};
};



// Opcodes for our arithmetic instructions
enum MathOpCode {
	op_add, op_addi,
	op_sub, op_subi,
	op_mul, op_muli,
	op_div, op_divi,
	op_ceq, op_ceqi,
	op_cne, op_cnei,
	op_clt, op_clti,
	op_cle, op_clei,
	op_cgt, op_cgti,
	op_cge, op_cgei
};

// Converts the opcode object to the opcode string in the moon code
static std::string opcodeToString(MathOpCode opcode) {
	switch (opcode) {
		case op_add:  return "add";
		case op_addi: return "addi";
		case op_sub:  return "sub";
		case op_subi: return "subi";
		case op_mul:  return "mul";
		case op_muli: return "muli";
		case op_div:  return "div";
		case op_divi: return "divi";
		case op_ceq:  return "ceq";
		case op_ceqi: return "ceqi";
		case op_cne:  return "cne";
		case op_cnei: return "cnei";
		case op_clt:  return "clt";
		case op_clti: return "clti";
		case op_cle:  return "cle";
		case op_clei: return "clei";
		case op_cgt:  return "cgt";
		case op_cgti: return "cgti";
		case op_cge:  return "cge";
		case op_cgei: return "cgei";
		default: ;
	}
}


// Converts opertion string tokens to an Opcode object
// Example "+" token gets converted to op_add
//		   "-" to op_sub, etc
static MathOpCode operatorStringToMathOpCode(std::string opStr) {
	TokenType opToken = Specification::TOKEN_MAP.at(opStr);
	switch (opToken) {
		// TODO: implement and or
	/*case and: return;
		case or: break;*/
		//case not: break;
		case lt:		return op_clt;
		case lesseq:	return op_cle;
		case noteq:		return op_cne;
		case gt:		return op_cgt;
		case greateq:	return op_cge;
		case comparison:return op_ceq;
		case addop:		return op_add;
		case subtractop:return op_sub;
		case multop:	return op_mul;
		case divop:		return op_div;
		default: ;
	}

}



/* -------------------------- Arithmetic Instruction instructions  ------------------------------- */
class ArithmeticInstruction : public Instruction {
public:
	Register dest;
	Register rj;
	Register rk;
	MathOpCode opcode;
	
	// Creates the instruction "opcode dest,rj,rk"
	ArithmeticInstruction(Register dest, Register rj, Register rk, MathOpCode opcode) {
		this->dest = dest;
		this->rj = rj;
		this->rk = rk;
		this->opcode = opcode;
	};

	std::string _toMoonCode() override {
		return Instruction::_toMoonCode(opcodeToString(opcode) + "\t" + registerToString(dest) + "," + registerToString(rj) + "," + registerToString(rk));
		
	}
};



class AddInstruction : public ArithmeticInstruction {
public:
	AddInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_add) {}
};


class SubtractInstruction : public ArithmeticInstruction {
public:
	SubtractInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_sub) {}
};

class MultiplyInstruction : public ArithmeticInstruction {
public:
	MultiplyInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_mul) {}
};

class DivisionInstruction : public ArithmeticInstruction {
public:
	DivisionInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_div) {}
};
// TODO: implement logical operators, Moons are bitwise, we dont want bitwise, we want logical

class EqualInstruction : public	ArithmeticInstruction {
public:
	EqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_ceq) {}
};

class NotEqualInstruction : public	ArithmeticInstruction {
public:
	NotEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_cne) {}
};

class LessThanInstruction : public	ArithmeticInstruction {
public:
	LessThanInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_clt) {}
};

class LessThanEqualInstruction : public	ArithmeticInstruction {
public:
	LessThanEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_cle) {}
};

class GreaterThanInstruction : public ArithmeticInstruction {
public:
	GreaterThanInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_cgt) {}
};

class GreatherThanEqualInstruction : public	ArithmeticInstruction {
public:
	GreatherThanEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk, op_cge) {}
};


/* --------------------- Arithmetic Immediate Instructions ------------------------------ */

class ArithmeticImmediateInstruction : public Instruction {
public:
	Register dest;
	Register rj;
	std::string k;
	MathOpCode opcode;

	// Creates the instruction "opcode dest,rj,k"
	ArithmeticImmediateInstruction(Register dest, Register rj, std::string k, MathOpCode opcode) {
		this->dest = dest;
		this->rj = rj;
		this->k = k;
		this->opcode = opcode;
	};

	std::string _toMoonCode() override {
		return Instruction::_toMoonCode(opcodeToString(opcode) + "\t" + registerToString(dest) + "," + registerToString(rj) + "," + k);
	}
};

class AddImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	AddImmediateInstruction(Register dest, Register rj, std::string r): ArithmeticImmediateInstruction(dest, rj, r, op_addi) {
		this->setComment(registerToString(dest) + ":= " + registerToString(rj) + " + " + r);
	}
};

class SubtractImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	SubtractImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_subi) {}
};

class MultiplyImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	MultiplyImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_muli) {}
};

class DivisionImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	DivisionImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_divi) {}
};
// TODO: implement logical immediate operators, Moons are bitwise, we dont want bitwise, we want logical

class EqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	EqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_ceqi) {}
};

class NotEqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	NotEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_cnei) {}
};

class LessThanImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	LessThanImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_clti) {}
};

class LessThanEqualImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	LessThanEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_clei) {}
};

class GreaterThanImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	GreaterThanImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_cgti) {}
};

class GreatherThanEqualImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	GreatherThanEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k, op_cgei) {}
};


/* -------------------------- Input and output instructions  ------------------------------- */

class GetCInstruction : public Instruction {
public:
	Register ri;
	// Creates the instruction "getc ri"
	GetCInstruction(Register ri) {
		this->ri = ri;
		this->setComment("Read the ascii value from the input");
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("getc\t" + registerToString(ri));
	};
};

class PutCInstruction : public Instruction {
public:
	Register ri;
	// Creates the instruction "putc ri"
	PutCInstruction(Register ri) {
		this->ri = ri;
		this->setComment("put the ascii value to the output");
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("putc\t" + registerToString(ri));
	}
};

/* --------------------------  Contrrol Instructions ------------------------------- */
class BranchIfZeroInstruction : public Instruction {
public:

	Register ri;
	std::string k;
	// Creates the instruction "bz ri,k"
	BranchIfZeroInstruction(Register ri, const std::string& cs): ri(ri),k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("bz\t" + registerToString(ri) + "," + k);
	};
};

class BranchIfNonZeroInstruction : public Instruction {
public:
	Register ri;
	std::string k;
	// Creates the instruction "bnz ri,k"
	BranchIfNonZeroInstruction(Register ri, const std::string& cs) : ri(ri), k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("bnz\t" + registerToString(ri) + "," + k);
	};
};

class JumpInstruction : public Instruction {
public:
	std::string k;
	// Creates the instruction "j k"
	JumpInstruction(const std::string& cs): k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("j\t" + k);
	};
};

class JumpRegisterInstruction : public Instruction {
public:
	Register ri;
	// Creates the instruction "jr ri"
	JumpRegisterInstruction(Register ri): ri(ri) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jr\t" + registerToString(ri));
	};
};

class JumpAndLinkInstruction : public Instruction {
public:
	Register ri;
	std::string k;
	// Creates the instruction "jl ri,k"
	JumpAndLinkInstruction(Register ri, const std::string& cs): ri(ri),k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jl\t" + registerToString(ri) + "," + k);
	};
};

class JumpAndLinkRegisterInstruction : public Instruction {
public:
	Register ri;
	Register rj;
	// Creates the instruction "jlr ri,rj"
	JumpAndLinkRegisterInstruction(Register ri, Register rj): ri(ri),rj(rj) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jlr\t" + registerToString(ri) + "," + registerToString(rj));
	};
};

class NoopInstruction : public Instruction {
public:

	NoopInstruction() {
	}
	// Creates the instruction "nop"
	NoopInstruction(const std::string& label, const std::string& comment)
		: Instruction(label, comment) {
	}

	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("nop");
	};
};


// Creates the instruction "hlt"
class HaltInstruction : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("hlt");
	};
};

/* --------------------------  Directives ------------------------------- */

// Creates the instruction "align"
class AlignDirective : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("align");
	};
};

// Creates the instruction "entry"
class EntryDirective : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("entry");
	};
};

class DefineWordDirective : public Instruction {
	SymbolTable* globalTable;
	SymbolTableRecord* record;
	std::string label;
	bool isTempMemory;


public:

	// Creates the instruction "record->label	dw	0"
	// or if the record is an array or object "record->label	res		size", where size is determind at compile time
	DefineWordDirective(SymbolTable* global_table, SymbolTableRecord* record)
		: globalTable(global_table),
		  record(record) {
	}

	// Creates the instruction "label	dw	0"
	DefineWordDirective(std::string label) {
		isTempMemory = true;
		this->label = label;
	};
		
	

	std::string _toMoonCode() override {
		// Since this is temp memory, we dont care about size calculaution
		if (isTempMemory) {
			// ex tn	dw	0
			return Instruction::_toMoonCode(label + "\t" + "dw" + "\t" + "0");
		}
		
		// Create the entry in the moon code
		if (record->typeStructure.structure == SymbolStructure::struct_array || record->typeStructure.type == SymbolType::type_class) {
			int allocationSize = SymbolTable::_sizeOf(globalTable, record->typeStructure);
			return Instruction::_toMoonCode(record->label + "\t" + "res" + "\t" + std::to_string(allocationSize));

		} else {
			// Used for simple primative types ex simple int/ simple float
			// Ex	x	dw	0
			return Instruction::_toMoonCode(record->label + "\t" + "dw" + "\t" + "0");
		}
	}
};



// Helper Instructions

class ClearRegisterInstruction : public Instruction {

public:
	Register r;
	// Simply clears the value of that register
	// This is simply sub r,r,r instruction
	ClearRegisterInstruction(Register r) {
		this->r = r;
	}

	std::string _toMoonCode() override {
		return SubtractInstruction(r, r, r).setComment("Clear register")._toMoonCode();
	};
};

class CommentInstruction : public Instruction {

public:
	std::string comment;
	// Creates a comment in the moon code with the comment string
	explicit CommentInstruction(const std::string& comment)
		: comment(comment) {
	}

	std::string _toMoonCode() override { 
		return "% ----------------" + comment + " ----------------\n";
	}
};

// This instruction generates all the instructions needed to evaluate an instruction
// Give it an expression, and it will generate all the instructions, 
// and at the end give you the output register of the final value.
// However, once the value/register has been used, it is up to the caller to free the output register

//	An example usage of this instruction is

//		Expression expr = expr
//		ExpressionEvalulationInstruction exprEval(generator, expr);
//		// where instrBlock is the output string instruction block
//		instrBlock.append(exprEval._toMoonCode());
//		// Store the value of the output register into the memory
//		instrBlock.append(StoreWordInstruction(r0, exprEval.outputRegister, "tempMemoryName")._toMoonCode());
//		// free the output register
//		generator->freeRegister(argExpr.outputRegister);

// You create instruction
// pass the expression,
// call the _toMoonCode()
// then store the output register,
// then free it
class ExpressionEvalulationInstruction : public Instruction {
public:
	// the expression this instruction is going generate the instructions for
	Expression& expr;
	// This is the output temp register where the final value/result of the expression will be in
	// You should take the output of this register and save it your own temp memory
	// Once done, you will need to manually free this output register
	Register outputRegister;
	ExpressionEvalulationInstruction(MoonGenerator* gen, Expression& expr): expr(expr) {
		generator = gen;
	}

	// Converts the operator node to its given arithmetic instruction
	std::string operationToInstruction(OperatorExpressionNode* opNode, Register ra, Register rb, Register rc) {
		MathOpCode opcode = operatorStringToMathOpCode(opNode->operatorValue.operatorValue);
		return ArithmeticInstruction(ra, rb, rc, opcode)._toMoonCode();
	}

	std::string _toMoonCode() override {
		std::string instrBlock;
		// Label the register counts for this tree
		labelRegisters(expr.root.get());
		// Get how many register this tree will need to evaluate
		int registerNeedsCount = expr.root.get()->registerCount;

		std::vector<Register> reglist;
		// Fill our reg list with unused registers
		for (int i = 0; i < registerNeedsCount; ++i) {
			reglist.push_back(generator->getUnusedRegister());
		}
		// the output register will always be the first register
		outputRegister = reglist[0];
		// true values should evaluate to a non zero value
		// false it should evaluatio to zero
		// This is used for comparision instructions
		// Since expression can have relational operators in them
		// they will evalute to true/false values, but since we dont have true/false
		// number values will be used
		// so 0 evals to false
		// and non 0 evals to true
		generateInstructions(expr.root.get(), instrBlock, reglist);

		// Free the non ouput registers
		for (int i = 1; i < reglist.size(); ++i) {
			generator->freeRegister(reglist[i]);
		}
		// the caller of this should free the register
		return instrBlock;

	}

	// This labels the tree with how many registers each node will use
	void labelRegisters(ExpressionElementNode* root) {
		if (root->nodeType == node_value) {
			// Leaf node will always use 1 register
			root->registerCount = 1;
		} else {
			OperatorExpressionNode* opNode = static_cast<OperatorExpressionNode*>(root);
			// Label left and right trees
			labelRegisters(opNode->left.get());
			labelRegisters(opNode->right.get());

			if (opNode->left->registerCount == opNode->right->registerCount) {
				root->registerCount = opNode->right->registerCount + 1;
			} else {
				// the root count is the max of the children
				root->registerCount = std::max(opNode->left->registerCount, opNode->right->registerCount);
			}
		}
	}
	// This generates all the instructions for the given expression root
	// you must pass in the instruction block string, as this method builds the instruction block as we go
	// reglist, is the list of registers the instructions will need. 
	// How many registers we need will be created by calling labelRegisters on the tree
	void generateInstructions(ExpressionElementNode* root, std::string& instrBlock, std::vector<Register> reglist) {
		
		// Get the two registers we will need
		Register ra = reglist.front(); // head(reglist)
		Register rb; // head(tail(reglist))
		if (reglist.size() == 1) {
			rb = ra;
		} else {
			rb = sublist(reglist, 1, reglist.size()).front();
		}
		
		
		if (root->nodeType == node_value) {
			ValueExpressionNode* valueNode = static_cast<ValueExpressionNode*>(root);
			if (valueNode->value.type == fragment_var) {
				Variable var = valueNode->value.var;
				// TODO: we also need to handle negative values
				// We need to handle functions and arrays
				if (var.isFuncCall) {
					// this is a function call
					// get the param labels
					std::vector<std::string> paramLabels;
					std::unordered_map<std::string, SymbolTableRecord> table = var.record->scope.get()->getTable();
					for (auto param : table) {
						if (param.second.kind  == SymbolKind::kind_parameter) {
							paramLabels.push_back(param.second.label);
						}	
					}
					// Pass by values
					for (int i = 0; i < var.arguments.size(); ++i) {
						// For each arugment, eval the expression then get the output register
						Expression arg = var.arguments[i];
						ExpressionEvalulationInstruction argExpr(generator, arg);
						instrBlock.append(argExpr._toMoonCode());
						// Store the value of the output register into the param's memory
						instrBlock.append(StoreWordInstruction(r0, argExpr.outputRegister, paramLabels[i])._toMoonCode());
						// free the output register
						generator->freeRegister(argExpr.outputRegister);
					}
					// After the arugments have been loaded into the labels, jl to the function				
					instrBlock.append(JumpAndLinkInstruction(r15, var.record->label).setComment("Jump to " + var.record->label + " function")._toMoonCode());
					// Since we know the return value is in r14, we'll need to copy/mov the output register to our output expression register
					// get some temp space to store the value in
					TempMemory temp = generator->getTempMemory();
					// Store the output of the return value into the temp memory
					instrBlock.append(StoreWordInstruction(r0, r14, temp.label)._toMoonCode());
					// Load the temp memory into our outpute register
					instrBlock.append(LoadWordInstruction(outputRegister, r0, temp.label)._toMoonCode());
					// Free the memoery we just used
					generator->freeTempMemory(temp);

				} else {
					// load a variable
					instrBlock.append(LoadWordInstruction(ra, r0, valueNode->value.var.record->label).setComment("load " + valueNode->value.var.toFullName() + " into register")._toMoonCode());
				}
				
			} else if (valueNode->value.type == fragment_numeric) {
				// load a literal
				// TODO: we also need to handle negative values
				instrBlock.append(ClearRegisterInstruction(ra)._toMoonCode());
				instrBlock.append(AddImmediateInstruction(ra, ra, valueNode->value.numericValue).setComment("load " + valueNode->value.numericValue + " into register")._toMoonCode());
			}
		} else {
			// This is a binary operator
			OperatorExpressionNode* opNode = static_cast<OperatorExpressionNode*>(root);
			ExpressionElementNode* left = opNode->left.get();
			ExpressionElementNode* right = opNode->right.get();
			if (left->registerCount >= reglist.size() && right->registerCount >= reglist.size()) {
				// Spill into memory
				generateInstructions(left, instrBlock, reglist);
				TempMemory temp = generator->getTempMemory();
				// Generate sw
				instrBlock.append(StoreWordInstruction(r0, ra, temp.label)._toMoonCode());
				generateInstructions(right, instrBlock, reglist);
				// Generate lw
				instrBlock.append(LoadWordInstruction(ra, r0, temp.label)._toMoonCode());
				generator->freeTempMemory(temp);
				instrBlock.append(operationToInstruction(opNode, ra,rb,ra));
			} else {
				// There is enough registers
				if (left->registerCount >= right->registerCount) {
					generateInstructions(left, instrBlock, reglist);
					generateInstructions(right, instrBlock, sublist(reglist, 1, reglist.size()));
					// Generate the operation
					instrBlock.append(operationToInstruction(opNode, ra,ra,rb));
				} else {
					generateInstructions(right, instrBlock, reglist);
					generateInstructions(left, instrBlock, sublist(reglist, 1, reglist.size()));
					// Generate the operation
					instrBlock.append(operationToInstruction(opNode, ra,rb,ra));
				}
				

			}

		}
		
	}
};

#endif

