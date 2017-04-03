#include "stdafx.h"
#include "Directive.h"


Directive::Directive() {
}


Directive::~Directive() {
}

Directive::Directive(SymbolTable* globalTable) {
	this->globalTable = globalTable;
}
