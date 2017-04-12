#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H

// An intermediate representation of an assignment statement 
class AssignStatement : public Statement {
public:

	AssignStatement();
	~AssignStatement();
	// the lhs variable we are assigning to
	Variable var;
	// the rhs expression that is being evaled
	Expression rhs;
	std::string _toMoonCode() override;
};

#endif
