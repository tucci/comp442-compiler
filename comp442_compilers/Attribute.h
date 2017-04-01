#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H



enum AttributeType {
	attr_expr,
	attr_statement,
	attr_var
};

struct Attributes {
	AttributeType type;
	Expression expr;
	Statement statemenent;
	Variable var;
};
#endif
