#ifndef INSTRUCTION_H
#define INSTRUCTION_H


class MoonGenerator;

enum InstructionType {};

class Instruction {
public:
	InstructionType instrType;
	~Instruction();
	Instruction& setLabel(std::string label);
	Instruction& setComment(std::string comment);
	// Give the code generator to each instruction, to allow them to get register values from the code generatror
	void setCodeGenerator(MoonGenerator* generator);
	virtual std::string _toMoonCode() = 0;
protected:
	Instruction();
	Instruction(std::string label, std::string comment);
	// Wraps the instruction string with the comments,labels, and newline
	std::string _toMoonCode(std::string instr);
	MoonGenerator* generator;
	std::string label;
	std::string comment;
	


};

#endif
