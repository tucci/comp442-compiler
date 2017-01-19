#include "stdafx.h"


using namespace std;

int main()
{
	specification spec;
	lexer lex(spec);
	if (lex.set_source("SourceCode.txt")) {
		while (lex.has_more_tokens()) {
			token t = lex.next_token();
			cout << t.lexeme << ", " 
				<< t.type << ", " 
				<< t.token_line << ", " 
				<< t.token_location;
		}
	} else {
		cout << "Source code not found" << endl;
	}
	
	
    return 0;
}






