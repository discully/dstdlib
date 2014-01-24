#include <iostream>

#include "../src/exception.hxx"
#include "tester.hxx"

template <class T>
void exceptionTest(dstd::Tester& t, const std::string& e_name, const T& e_instance)
{
	std::string e_what("dstd::" + e_name);
	std::string t_what(e_name + "_what");
	try
	{
		throw e_instance;
		t.registerTestFailed(e_name, "no exception thrown");
		t.registerTestFailed(t_what, "no exception thrown");
	}
	catch(dstd::exception& e)
	{
		t.registerTestPassed(e_name);
		t.testEqual<std::string>(t_what, e.what(), e_what);
	}
	catch(...)
	{
		t.registerTestFailed(e_name, "incorrect exception thrown");
		t.registerTestFailed(t_what, "incorrect exception thrown");
	}
}

int main()
{
	dstd::Tester t("dstd::exception");
	
	exceptionTest(t, "exception", dstd::exception());
	exceptionTest(t, "out_of_range", dstd::out_of_range());
	exceptionTest(t, "bad_alloc", dstd::bad_alloc());
	exceptionTest(t, "length_error", dstd::length_error());
	
	t.report();
	
	return 0;
}
