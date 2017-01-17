#include "stdafx.h"


using namespace std;

int main()
{

	state s1(true, false);
	state s2;
	state s3(false, true);

	s1.add_transition_to("a", s2);
	s1.add_transition_to("a", s3);
	s2.add_transition_to("b", s3);

	cout << "test";


    return 0;
}


