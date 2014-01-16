#include <iostream>

#include "exception.hxx"

int main()
{
	try { throw dstd::exception(); }
	catch(dstd::out_of_bounds& e) { std::cout << e.what() << std::endl; }
	catch(dstd::exception& e) { std::cout << e.what() << std::endl; }
	
	try { throw dstd::out_of_bounds(); }
	catch(dstd::out_of_bounds& e) { std::cout << e.what() << std::endl; }
	catch(dstd::exception& e) { std::cout << e.what() << std::endl; }
	
	throw dstd::exception();
	
	return 0;
}
