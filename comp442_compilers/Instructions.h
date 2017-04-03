#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "Instruction.h"
#include <string>
#include "Register.h"

class AlignInstruction : public Instruction {
public:
		std::string _toMoonCode() override {
			return "align";
		};
};

class EntryInstruction : public Instruction {
public:
	std::string _toMoonCode() override {
		return "entry";
	};
};

class HaltInstruction : public Instruction {
public:
	std::string _toMoonCode() override {
		return "hlt";
	};
};

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
		return "lw\t" + ToRegisterValue(ri) + "," + offset + "(" + ToRegisterValue(rj) + ")";
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
		return "sw\t" + offset + "(" + ToRegisterValue(rj) + ")," + ToRegisterValue(ri);
	};
};


// A math instruction is used for operations such as add,sub,mul,div, ==, !=, etc
class ArithmeticInstruction : public Instruction {
public:
	Register dest;
	Register rj;
	Register rk;
	
	ArithmeticInstruction(Register dest, Register rj, Register rk) {
		this->dest = dest;
		this->rj = rj;
		this->rk = rk;
	};
};



class AddInstruction : public ArithmeticInstruction {
public:
	AddInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "add\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	}
};


class SubtractInstruction : public ArithmeticInstruction {
public:
	SubtractInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "sub\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	}
};

class MultiplyInstruction : public ArithmeticInstruction {
public:
	MultiplyInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "mul\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	}
};

class DivisionInstruction : public ArithmeticInstruction {
public:
	DivisionInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "div\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	}
};
// TODO: implement these operators, Moons are bitwise, we dont want bitwise, we want logical
//
//class AndInstruction : public ArithmeticInstruction {
//
//public:
//	std::string _toMoonCode() override {
//		return "and\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
//	}
//};
//
//class OrInstruction : public ArithmeticInstruction {
//
//public:
//	std::string _toMoonCode() override {
//		return "and\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
//	}
//};

class EqualInstruction : public	ArithmeticInstruction {
public:
	EqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "ceq\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};

class NotEqualInstruction : public	ArithmeticInstruction {
public:
	NotEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "cne\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};

class LessThanInstruction : public	ArithmeticInstruction {
public:
	LessThanInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "clt\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};

class LessThanEqualInstruction : public	ArithmeticInstruction {
public:
	LessThanEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "cle\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};

class GreaterThanInstruction : public ArithmeticInstruction {
public:
	GreaterThanInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "cgt\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};

class GreatherThanEqualInstruction : public	ArithmeticInstruction {
public:
	GreatherThanEqualInstruction(Register dest, Register rj, Register rk): ArithmeticInstruction(dest, rj, rk) {}
	std::string _toMoonCode() override {
		return "cge\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(rk);
	};
};


/* --------------------- Immediate Instructions ------------------------------ */

class ArithmeticImmediateInstruction : public Instruction {
public:
	Register dest;
	Register rj;
	std::string k;

	ArithmeticImmediateInstruction(Register dest, Register rj, std::string k) {
		this->dest = dest;
		this->rj = rj;
		this->k = k;
	};
};

class AddImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	AddImmediateInstruction(Register dest, Register rj, std::string r): ArithmeticImmediateInstruction(dest, rj, r) {}
	std::string _toMoonCode() override {
		AddInstruction a(Register::r1, Register::r4, Register::r10);

		return "addi\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	}
};

class SubtractImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	SubtractImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "subi\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	}
};

class MultiplyImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	MultiplyImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "muli\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	}
};

class DivisionImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	DivisionImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "divi\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	}
};
// TODO: implement these immediate operators, Moons are bitwise, we dont want bitwise, we want logical
//
//class AndImmediateInstruction : public ArithmeticImmediateInstruction {
//
//public:
//	std::string _toMoonCode() override {
//		return "and\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(k);
//	}
//};
//
//class OrImmediateInstruction : public ArithmeticImmediateInstruction {
//
//public:
//	std::string _toMoonCode() override {
//		return "and\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + ToRegisterValue(k);
//	}
//};

class EqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	EqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "ceqi\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

class NotEqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	NotEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "cnei\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

class LessThanImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	LessThanImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "clti\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

class LessThanEqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	LessThanEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "clei\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

class GreaterThanImmediateInstruction : public ArithmeticImmediateInstruction {
public:
	GreaterThanImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "cgti\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

class GreatherThanEqualImmediateInstruction : public	ArithmeticImmediateInstruction {
public:
	GreatherThanEqualImmediateInstruction(Register dest, Register rj, std::string k) : ArithmeticImmediateInstruction(dest, rj, k) {}
	std::string _toMoonCode() override {
		return "cgei\t" + ToRegisterValue(dest) + "," + ToRegisterValue(rj) + "," + k;
	};
};

#endif

