#include "stdafx.h"


using namespace std;

int main() {
	specification spec;
	lexer lex(&spec);
	// TODO: write tokens out to file
	if (lex.set_source("SourceCode.txt")) {
		while (lex.has_more_tokens()) {
			token t = lex.next_token();
			cout << "lexeme: " << t.lexeme << ", "
				<< "type: " << t.type << ", "
				<< "line number: " << t.token_line << endl;
		}
	} else {
		cout << "Source code not found" << endl;
	}


	
	
    return 0;
}






