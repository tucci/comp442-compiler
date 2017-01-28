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

	Specification spec(true);
	Lexer lex(&spec);
	lex.setSource("SourceCode.txt");

	while (lex.hasMoreTokens()) {
		Token token = lex.nextToken();
	
		if (writeToFile) {
			// write to output/error file
			if (token.type == TokenType::error_token) {
				error << token;
			} else {
				output << token;
			}
		} else {			
			cout << token;
		}
	}


	if (writeToFile) {
		output.close();
		error.close();
		std::cout << "Successfully written to files: output.txt and errors.txt" << std::endl;
	}
	


	
	
    return 0;
}






