#include "stdafx.h"




Symbol::Symbol(std::string name, bool isTerminal, bool isSemantic) {
	mName = name;
	mIsTerminalSymbol = isTerminal;
	mIsSemanticSymbol = isSemantic;
}

Symbol::~Symbol() {
}

const std::string& Symbol::getName() const {
	return mName;
}

bool Symbol::isTerminal() const {
	return mIsTerminalSymbol;
}

bool Symbol::isSemantic() const {
	return mIsSemanticSymbol;
}

std::ostream& operator <<(std::ostream& os, const Symbol& s) {
	// <symbolname>
	os << "<" << s.mName << ">";
	return os;
}

