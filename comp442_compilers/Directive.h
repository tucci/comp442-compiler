#ifndef DIRECTIVE_H
#define DIRECTIVE_H
class SymbolTable;

class Directive {
public:
	SymbolTable* globalTable;
	~Directive();
	virtual std::string _toMoonCode() = 0;
protected:
	Directive();
	Directive(SymbolTable* globalTable);
};

#endif
