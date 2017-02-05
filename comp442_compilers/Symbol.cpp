#include "stdafx.h"


Symbol::Symbol(std::string name, bool isTerminal) {
	mName = name;
	mIsTerminalSymbol = isTerminal;
}

Symbol::~Symbol() {
}

std::string Symbol::getName() const {
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
