#include "stdafx.h"


Terminal::Terminal(std::string name) : Symbol(name, true){
}

Terminal::~Terminal() {
}


const SpecialTerminal SpecialTerminal::EPSILON("EPSILON");
const SpecialTerminal SpecialTerminal::END_OF_FILE("$");

SpecialTerminal::SpecialTerminal(std::string name) : Terminal(name) {

}

SpecialTerminal::~SpecialTerminal() {
}


bool SpecialTerminal::isEpsilon(const std::string& symbolString) {
	return symbolString == SpecialTerminal::EPSILON.getName();
}

