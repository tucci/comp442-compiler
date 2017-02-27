#include "stdafx.h"


using namespace std;
int main() {
	Compiler cp;
	cp.setSourceFile("sourcefile.txt");
	try {
		cp.analyzeSyntax();
	} catch (std::exception e) {
	
	}


	return 0;
}






