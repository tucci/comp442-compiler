#include "stdafx.h"


using namespace std;
int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cout << "No input file given" << std::endl;	
		return 0;
	}

	std::string sourcefile = argv[1];
	
	Compiler cp;
 	cp.setSourceFile(sourcefile);

	try {
		//  Hold try catch for special cases
		// inorder to free memory
		cp.compile();
	} catch (std::exception e) {
		std::cerr << e.what();
	}
	


	return 0;
}






