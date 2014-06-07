#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/algorithm.hxx"
#include "../src/vector.hxx"



bool oddLessThan(const int& a, const int& b) // sort such that odd numbers come before even, then smaller numbers before larger
{
	if( a % 2  == b % 2 ) return a < b;
	else if( a % 2 == 0 ) return false;
	else return true;
}


class IntSummation
{
	public:
		IntSummation() : n(0) {}
		void operator() (int i) { this->n += i; }
		int n;
};



int main()
{
	dstd::Tester t("<algorithm>");
	
	
	//
	//Non-modifying sequence operations
	//
	
	// for_each
	{
		int sum = 0;
		dstd::vector<int> v;
		for(int i = 0; i < 10; ++i)
		{
			sum += i;
			v.push_back(i);
		}
		
		IntSummation s = dstd::for_each(v.begin(), v.end(), IntSummation());
		t.testEqual("for_each 1", s.n, sum);
		
		s = dstd::for_each(v.begin(), v.begin(), IntSummation());
		t.testEqual("for_each 2", s.n, 0);
	}
	
	
	//
	// Min/max
	
	{
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
		
		dstd::vector<int> v;
		v.push_back(b);
		v.push_back(a);
		v.push_back(c);
		const dstd::vector<int> empty;
		
		t.testEqual("min_element 1", *(dstd::min_element(v.begin(), v.end())), a);
		t.testEqual("min_element 2", *(dstd::min_element(v.begin(), v.end(), oddLessThan)), b);
		t.testEqual("min_element 3", dstd::min_element(empty.begin(), empty.end()), empty.end());
		t.testEqual("min_element 4", dstd::min_element(empty.begin(), empty.end(), oddLessThan), empty.end());
		
		t.testEqual("max_element 1", *(dstd::max_element(v.begin(), v.end())), c);
		t.testEqual("max_element 2", *(dstd::max_element(v.begin(), v.end(), oddLessThan)), c);
		t.testEqual("max_element 3", dstd::max_element(empty.begin(), empty.end()), empty.end());
		t.testEqual("max_element 4", dstd::max_element(empty.begin(), empty.end(), oddLessThan), empty.end());
	}
	
	
	t.report();
	
	
	return 0;
}
