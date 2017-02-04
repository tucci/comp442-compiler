#include "stdafx.h"


Symbol::Symbol(std::string name, bool isTerminal) {
	this->name = name;
	this->isTerminalSymbol = isTerminal;
}

Symbol::~Symbol() {
}

std::string Symbol::getName() {
	return name;
}

bool Symbol::isTerminal() {
	return isTerminalSymbol;
}

std::ostream& operator <<(std::ostream& os, const Symbol& s) {
	// <symbolname>
	os << "<" << s.name << ">";
	return os;
}