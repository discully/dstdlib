#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/algorithm.hxx"



bool oddLessThan(const int& a, const int& b) // sort such that odd numbers come before even, then smaller numbers before larger
{
	if( a % 2  == b % 2 ) return a < b;
	else if( a % 2 == 0 ) return false;
	else return true;
}



int main()
{
	dstd::Tester t("<algorithm>");
	
	
	//
	// Min/max
	
	
	{
		const double a = 1.1, b = 2.2;
		const int a = 4, b = 5, c = 6;
		
		t.testEqual("min 1a", dstd::min(a,b), a);
		t.testEqual("min 1b", dstd::min(b,a), a);
		t.testEqual("min 2a", dstd::min(a, b, oddLessThan), b);
		t.testEqual("min 2b", dstd::min(a, c, oddLessThan), a);
		t.testEqual("min 2c", dstd::min(b, c, oddLessThan), b);
		
		t.testEqual("max 1a", dstd::max(a,b), b);
		t.testEqual("max 1b", dstd::max(b,a), b);
		t.testEqual("max 2a", dstd::max(a, b, oddLessThan), a);
		t.testEqual("max 2b", dstd::max(a, c, oddLessThan), c);
		t.testEqual("max 2c", dstd::max(b, c, oddLessThan), c);
	}
	
	
	t.report();
	
	
	return 0;
}
