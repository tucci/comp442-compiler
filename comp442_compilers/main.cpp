#include "stdafx.h"


using namespace std;

int main()
{
	specification spec;
	lexer lex(spec);
	token t = lex.next_token();

	cout << "token type: " << t.type<< endl;
	cout << token_type::non_zero_t;
	


    return 0;
}






