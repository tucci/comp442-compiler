
#ifndef PUTSTATEMENT_H
#define PUTSTATEMENT_H
class PutStatement : public Statement{
public:
	PutStatement();
	~PutStatement();
	Expression expression;
	std::string _toMoonCode() override;
};

#endif
