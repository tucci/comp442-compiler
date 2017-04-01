#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum InstructionType {};

class Instruction {
public:
	InstructionType instrType;
	~Instruction();
	void setLabel(std::string label);
	void setComment(std::string comment);
	virtual std::string _toMoonCode() = 0;
protected:
	Instruction();
private:
	std::string label;
	std::string comment;

};

#endif
