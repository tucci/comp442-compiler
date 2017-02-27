#include "stdafx.h"


using namespace std;
int main() {
	Compiler cp;
	cp.setSourceFile("sourcefile.txt");
	try {
		//  Hold try catch for special cases
		// inorder to free memory
		cp.analyzeSyntax();
	} catch (std::exception e) {
	
	}


	return 0;
}






