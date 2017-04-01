#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H

class AssignStatement : public Statement {
public:
	AssignStatement();
	~AssignStatement();
	Variable var;
	Expression rhs;

	std::string _toMoonCode() override;
};

#endif
