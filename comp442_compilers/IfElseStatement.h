#ifndef IFELSE_STATEMENT_H
#define IFELSE_STATEMENT_H


class IfElseStatement : public Statement {
public:
	IfElseStatement();
	~IfElseStatement();

	bool inElseBlock;

	Expression condition;
	StatementBlock ifStatements;
	StatementBlock elseStatements;

	std::string _toMoonCode() override;
};



#endif

