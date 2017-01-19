#include "stdafx.h"


using namespace std;

int main()
{
	specification spec;
	lexer lex(spec);
	lex.set_source("SourceCode.txt");

	char* c = lex.next_char();
	while (c != NULL) {
		cout << *c;
		c = lex.next_char();
	}
	cout << "end of source";
	
    return 0;
}






