#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "Instruction.h"
#include <string>

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
#endif

