#include "stdafx.h"


using namespace std;
int main() {
	Compiler cp;
 	cp.setSourceFile("sourcefile.txt");
	try {
		//  Hold try catch for special cases
		// inorder to free memory
		cp.compile();
	} catch (std::exception e) {
		std::cerr << e.what();
	}



	


	return 0;
}






