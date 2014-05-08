#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/utility.hxx"


template <class T1, class T2>
std::ostream& operator<<(std::ostream& stream, const dstd::pair<T1, T2>& p)
{
	stream << "dstd::pair(" << p.first << ", " << p.second << ")";
	return stream;
}


int main()
{
	dstd::Tester t("<utility>");
	
	std::string test_name;
	
	
	//
	// pair
	
	
	test_name = "pair::pair()";
	try
	{
		dstd::pair<double, int> p();
		t.registerTestPassed(test_name);
	}
	catch(...)
	{
		t.registerTestFailed(test_name, "Exception was thrown");
	}
	
	
	test_name = "pair::pair(a,b)";
	try
	{
		dstd::pair<int, double> p(4, 3.141);
		t.registerTestPassed(test_name);
	}
	catch(...)
	{
		t.registerTestFailed(test_name, "Exception was thrown");
	}
	
	
	{
		dstd::pair<int, double> p1(4, 3.141);
		dstd::pair<int, double> p2(4, 3.141);
		dstd::pair<int, double> p3(4, 3.14 );
		dstd::pair<int, double> p4(5, 3.141);
		dstd::pair<int, double> p5(5, 3.14 );
		
		t.testEqual("pair operator== 1", (p1 == p1), true);
		t.testEqual("pair operator== 2", (p1 == p2), true);
		t.testEqual("pair operator== 3", (p1 == p3), false);
		t.testEqual("pair operator== 4", (p1 == p4), false);
		t.testEqual("pair operator== 5", (p1 == p5), false);
		
		t.testEqual("pair operator!= 1", (p1 != p1), false);
		t.testEqual("pair operator!= 2", (p1 != p2), false);
		t.testEqual("pair operator!= 3", (p1 != p3), true);
		t.testEqual("pair operator!= 4", (p1 != p4), true);
		t.testEqual("pair operator!= 5", (p1 != p5), true);
	}
	
	
	{
		dstd::pair<int, double> p1(4, 3.141);
		t.testEqual("pair::pair(p)", p1, dstd::pair<int, double>(p1));
	}
	
	
	{
		dstd::pair<int, double> p1(4, 3.141);
		dstd::pair<int, double> p2 = p1;
		t.testEqual("pair::operator=", p1, p2);
	}
	
	
	{
		dstd::pair<char, char> aa( 'a', 'a' );
		dstd::pair<char, char> ab( 'a', 'b' );
		dstd::pair<char, char> ba( 'b', 'a' );
		
		t.testEqual("pair operator< 1", (aa < aa), false);
		t.testEqual("pair operator< 2", (aa < ab), true);
		t.testEqual("pair operator< 3", (ab < aa), false);
		
		t.testEqual("pair operator<= 1", (aa <= aa), true);
		t.testEqual("pair operator<= 2", (aa <= ab), true);
		t.testEqual("pair operator<= 3", (ab <= aa), false);
		
		t.testEqual("pair operator> 1", (aa > aa), false);
		t.testEqual("pair operator> 2", (aa > ab), false);
		t.testEqual("pair operator> 3", (ab > aa), true);
		
		t.testEqual("pair operator>= 1", (aa >= aa), true);
		t.testEqual("pair operator>= 2", (aa >= ab), false);
		t.testEqual("pair operator>= 3", (ab >= aa), true);
	}
	
	
	{
		int i = 4;
		double pi = 3.141;
		dstd::pair<int, double> p1(i, pi);
		dstd::pair<int, double> p2 = dstd::make_pair(i, pi);
		t.testEqual("make_pair", p1, p2);
	}
	
	
	t.report();
	
	
	return 0;
}
