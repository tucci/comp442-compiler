#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

class ForStatement : public Statement {
public:
	ForStatement();
	~ForStatement();

	AssignStatement initializer;
	Expression condition;
	AssignStatement incrementer;

	StatementBlock statements;
	std::string _toMoonCode() override;
};

#endif
