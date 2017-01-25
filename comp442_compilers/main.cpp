#include "stdafx.h"


using namespace std;

int main() {

	bool write_to_file = true;
	ofstream output;
	ofstream error;

	if (write_to_file) {
		output.open("output.txt");
		error.open("errors.txt");
	}

	specification spec;
	lexer lex(&spec);
	lex.set_source("SourceCode.txt");

	while (lex.has_more_tokens()) {
		token t = lex.next_token();
		string token_output = "lexeme: " + t.lexeme +  "\t\ttype: " + to_string(t.type) + "\t\tline number: " + to_string(t.token_line) + "\n";

		if (write_to_file) {
			// write to output/error file
			if (t.type == token_type::error_token) {
				error << token_output;
			} else {
				output << token_output;
			}
		} else {
			// Write to console
			cout << token_output;
		}
	}


	if (write_to_file) {
		output.close();
		error.close();
	}
	


	
	
    return 0;
}






