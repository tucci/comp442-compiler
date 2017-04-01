#ifndef MOON_GENERATOR_H
#define MOON_GENERATOR_H


class MoonGenerator {
public:
	MoonGenerator();
	~MoonGenerator();
	void addInstruction(std::shared_ptr<Instruction> instruction);
	void generateCode();
	std::string getMoonFile();
	void setOutputFileName(std::string outputfile);
	
private:
	std::vector<std::shared_ptr<Instruction>> instructions;
	std::string outputMoonFile;

};

#endif
