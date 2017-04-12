#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

// an intermediate for a for statment
// Include all the inner loop statments
class ForStatement : public Statement {
public:
	ForStatement();
	~ForStatement();
	// the initializer Assign Statement
	AssignStatement initializerAssignStatement;
	// the condition of the for loop
	Expression condition;
	// the incrment assignment statment for the loop
	AssignStatement incrementer;
	// the inner statements for this loop
	StatementBlock statements;
	std::string _toMoonCode() override;
};

#endif
