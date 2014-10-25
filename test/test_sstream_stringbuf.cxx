#include <iostream>
#include "tester.hxx"
#include "../src/ios.hxx"
#include "../src/sstream.hxx"



std::ostream& operator<< (std::ostream& stream, const dstd::string& s)
{
	stream << s.c_str();
	return stream;
}



int main()
{
	dstd::Tester t("<sstream> stringbuf");
	
	dstd::stringbuf s;
	
	
	// constructor
	
	// explicit basic_stringbuf(std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	
	{
		const dstd::stringbuf sb;
		t.testEqual("constructor()::defaultConstructorShouldGiveEmptyBuffer", sb.str(), "");
	}
	
	// explicit basic_stringbuf (const std::basic_string<CharT, traits, Allocator>& new_str, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	
	{
		const dstd::string str("hello");
		const dstd::stringbuf sb(str);
		t.testEqual("constructor(string)::constructorShouldCorrectlyInitialiseString", sb.str(), str);
	}
	
	//std::basic_string<CharT, Traits, Allocator> str() const;
	
	{
		const dstd::string str("hello");
		const dstd::stringbuf sb(str);
		t.testEqual("str()::strShouldReturnInitialisedValue", sb.str(), str);
	}
	
	//void str( const std::basic_string<CharT, Traits, Allocator>& s);
	
	{
		dstd::stringbuf sb("hello");
		
		const dstd::string str("world");	
		sb.str(str);
		
		t.testEqual("str(string)::strShouldUpdateString", sb.str(), str);
	}
	
	t.report();
	return 0;
}

