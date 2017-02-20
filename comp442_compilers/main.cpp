#include "stdafx.h"


using namespace std;


void createCode(std::string filename) {
	
	std::ifstream inputFile;
	try {
		inputFile.open(filename);
	

		std::stringstream inputStream;
		inputStream << inputFile.rdbuf();
		inputFile.close();

		
		std::string line;
		while (std::getline(inputStream, line, '\n')) {
			size_t splitIndex = line.find("->");
			std::string splitLine = line.substr(0, splitIndex);
			if (!splitLine.empty()) {
				cout << line << endl;
				//cout << "NonTerminal " << splitLine << " =" << "g.addNonTerminal(\"" << splitLine << "\");" << endl;
				//cout << trim(line.substr(splitIndex + 2)) << endl;

			}
			
		}
		
		
	} catch (std::ifstream::failure e) {
		std::cout << "Error reading sample file";
	}

}




int main() {

	Compiler cp;
	cp.setSourceFile("a2_sample.txt");

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




	

    return 0;
}






