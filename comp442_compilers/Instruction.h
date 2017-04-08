#ifndef INSTRUCTION_H
#define INSTRUCTION_H


// Forward declare the generator
class MoonGenerator;

// An instruction object is a instruction or set of instructions to be generated in the moon code
// If you want to create an instruction, subclass the instruction class and override the _toMoonCode() method
// the _toMoonCode() should return the actual instruction string to be placed in the output file
// Also subclassed _toMoonCode() should call the parent _toMoonCode() inorder for labels/comments to be outputted
class Instruction {
public:	
	~Instruction();
	// Set the label for this instruction
	// it returns the instruction back in case you want to build/chain label/comments calls
	Instruction& setLabel(std::string label);
	// Set the comment for this instruction
	// it returns the instruction back in case you want to build/chain label/comments calls
	Instruction& setComment(std::string comment);
	// Give the code generator to each instruction, to allow them to get register values from the code generatror
	void setCodeGenerator(MoonGenerator* generator);
	// method for subclasses to override
	// this method creates the actual moon code that will be added to the output file
	virtual std::string _toMoonCode() = 0;
protected:
	Instruction();
	// Create an instruction with the given label and comment
	Instruction(std::string label, std::string comment);
	// Wraps the instruction string with the comments,labels, and newline
	std::string _toMoonCode(std::string instr);
	// The attached gen for the instruction to use
	MoonGenerator* generator;
	// the label for this instruction
	std::string label;
	// the comment for this instruction
	std::string comment;
	


};

#endif
