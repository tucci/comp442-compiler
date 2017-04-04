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
	StoreWordInstruction(Register rj, Register ri, std::string offset) {
		this->rj = rj;
		this->ri = ri;
		this->offset = offset;
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("sw\t" + offset + "(" + registerToString(rj) + ")," + registerToString(ri));	
	};
};



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



/* -------------------------- Arithmetic Instruction instructions  ------------------------------- */
class ArithmeticInstruction : public Instruction {
public:
	Register dest;
	Register rj;
	Register rk;
	MathOpCode opcode;
	
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
	GetCInstruction(Register ri) {
		this->ri = ri;
		this->setComment("Outputs the ascii value from " + registerToString(ri));
	};
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("getc\t" + registerToString(ri));
	};
};

class PutCInstruction : public Instruction {
public:
	Register ri;
	PutCInstruction(Register ri) {
		this->ri = ri;
		this->setComment("Read the ascii value from input and assigns it to " + registerToString(ri));
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
	BranchIfZeroInstruction(Register ri, const std::string& cs): ri(ri),k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("bz\t" + registerToString(ri) + "," + k);
	};
};

class BranchIfNonZeroInstruction : public Instruction {
public:
	Register ri;
	std::string k;
	BranchIfNonZeroInstruction(Register ri, const std::string& cs) : ri(ri), k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("bnz\t" + registerToString(ri) + "," + k);
	};
};

class JumpInstruction : public Instruction {
public:
	std::string k;
	JumpInstruction(const std::string& cs): k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("j\t" + k);
	};
};

class JumpRegisterInstruction : public Instruction {
public:
	Register ri;
	JumpRegisterInstruction(Register ri): ri(ri) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jr\t" + registerToString(ri));
	};
};

class JumpAndLinkInstruction : public Instruction {
public:
	Register ri;
	std::string k;
	JumpAndLinkInstruction(Register ri, const std::string& cs): ri(ri),k(cs) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jl\t" + registerToString(ri) + "," + k);
	};
};

class JumpAndLinkRegisterInstruction : public Instruction {
public:
	Register ri;
	Register rj;
	JumpAndLinkRegisterInstruction(Register ri, Register rj): ri(ri),rj(rj) {}
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("jlr\t" + registerToString(ri) + "," + registerToString(rj));
	};
};

class NoopInstruction : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("nop");
	};
};

class HaltInstruction : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("hlt");
	};
};

/* --------------------------  Directives ------------------------------- */

class AlignDirective : public Instruction {
public:
	std::string _toMoonCode() override {
		return Instruction::_toMoonCode("align");
	};
};

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


	DefineWordDirective(SymbolTable* global_table, SymbolTableRecord* record)
		: globalTable(global_table),
		  record(record) {
	}

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

	explicit CommentInstruction(const std::string& comment)
		: comment(comment) {
	}

	std::string _toMoonCode() override { 
		return "% ----------------" + comment + " ----------------\n";
	}
};

class ExpressionEvalulationInstruction : public Instruction {
	Expression& expr;

public:
	ExpressionEvalulationInstruction(Expression& expr): expr(expr) {}

	std::string _toMoonCode() override {
		// TODO: implement

	}
};

#endif

