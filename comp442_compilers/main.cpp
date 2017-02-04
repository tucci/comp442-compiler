#include "stdafx.h"


using namespace std;

int main() {

	//bool writeToFile = false;
	//ofstream output;
	//ofstream error;

	//if (writeToFile) {
	//	output.open("output.txt");
	//	error.open("errors.txt");
	//}

	//Specification spec(true);
	//Lexer lex(&spec);
	//lex.setSource("SourceCode.txt");

	//while (lex.hasMoreTokens()) {
	//	Token token = lex.nextToken();
	//
	//	if (writeToFile) {
	//		// write to output/error file
	//		if (token.type == TokenType::error_token) {
	//			error << token;
	//		} else {
	//			output << token;
	//		}
	//	} else {			
	//		cout << token;
	//	}
	//}


	//if (writeToFile) {
	//	output.close();
	//	error.close();
	//	std::cout << "Successfully Wrote to output.txt and errors.txt" << std::endl;
	//}
	
	Terminal t("s");
	NonTerminal n("<test>");

	cout << t << " " << t.isTerminal() << endl;
	cout << n << " " << n.isTerminal() << endl;


	
	
    return 0;
}






