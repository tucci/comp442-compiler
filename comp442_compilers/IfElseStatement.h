#ifndef IFELSE_STATEMENT_H
#define IFELSE_STATEMENT_H

// an intermediate representtion for an if/else statement
class IfElseStatement : public Statement {
public:
	IfElseStatement();
	~IfElseStatement();

	// whether we are in the else block
	bool inElseBlock;
	// the condition for the if 
	Expression condition;
	// includes all the instructions/statments in the if block
	StatementBlock ifStatements;
	// includes all the instructions/statments in the else block
	StatementBlock elseStatements;
	std::string _toMoonCode() override;
};



#endif

