#ifndef GET_STATEMENT_H
#define GET_STATEMENT_H
#include "Register.h"

// an intermediate for a get statement
class GetStatement : public Statement {
public:
	GetStatement();
	~GetStatement();
	Variable var;
	std::string _toMoonCode() override;
};
#endif

