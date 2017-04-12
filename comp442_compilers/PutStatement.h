
#ifndef PUTSTATEMENT_H
#define PUTSTATEMENT_H
#include "Register.h"
// an intermediate representation for a put statement
class PutStatement : public Statement{
public:
	
	PutStatement();
	~PutStatement();
	Expression expression;
	std::string _toMoonCode() override;
};

#endif
