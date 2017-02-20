#include "stdafx.h"

const Symbol Symbol::EPSILON("EPSILON", false);
const Symbol Symbol::END_OF_FILE("$", false);


Symbol::Symbol(std::string name, bool isTerminal) {
	mName = name;
	mIsTerminalSymbol = isTerminal;
}

Symbol::~Symbol() {
}

const std::string& Symbol::getName() const {
	return mName;
}

bool Symbol::isTerminal() {
	return mIsTerminalSymbol;
}

std::ostream& operator <<(std::ostream& os, const Symbol& s) {
	// <symbolname>
	os << "<" << s.mName << ">";
	return os;
}
