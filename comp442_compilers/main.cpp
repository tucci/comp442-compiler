#include "stdafx.h"


using namespace std;

int main() {

	bool writeToFile = true;
	ofstream output;
	ofstream error;

	if (writeToFile) {
		output.open("output.txt");
		error.open("errors.txt");
	}

	Specification spec;
	Lexer lex(&spec);
	lex.setSource("SourceCode.txt");

	while (lex.hasMoreTokens()) {
		Token token = lex.nextToken();
		string tokenOutput = "lexeme: " + token.lexeme +  "\t\ttype: " + to_string(token.type) + "\t\tline number: " + to_string(token.tokenLine) + "\n";

		if (writeToFile) {
			// write to output/error file
			if (token.type == TokenType::error_token) {
				error << tokenOutput;
			} else {
				output << tokenOutput;
			}
		} else {
			// Write to console
			cout << tokenOutput;
		}
	}


	if (writeToFile) {
		output.close();
		error.close();
	}
	


	
	
    return 0;
}






