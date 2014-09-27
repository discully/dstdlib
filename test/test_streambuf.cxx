#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/streambuf.hxx"



class test_streambuf : public dstd::streambuf
{
	public:
		test_streambuf() : dstd::streambuf() {}
};



int main()
{
	dstd::Tester t("<streambuf>");
	
	test_streambuf sb;
	typedef dstd::char_traits<char> ct;
	
	//dstd::locale getloc() const { return this->loc; }
	
	t.testEqual("getloc:shouldReturnDefaultLocale", sb.getloc(), dstd::locale());
	
	//dstd::locale pubimbue(const dstd::locale& new_locale)
	
	{
		dstd::locale a;
		t.testEqual("pubimbue:shouldReturnDefaultLocale", sb.pubimbue(a), dstd::locale());
	}
	
	//basic_streambuf* pubsetbuf(char_type* s, dstd::streamsize n)
	
	{
		const dstd::streamsize n = 5;
		char buffer[n];
		t.testEqual("pubsetbuf:shouldReturnThis", sb.pubsetbuf(buffer, n), &sb);
	}
	
	//pos_type pubseekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which = ios_base::in | ios_base::out)
	
	t.testEqual("pubseekoff:shouldReturnMinus1", sb.pubseekoff(10, dstd::ios_base::beg), -1);
	
	//pos_type pubseekpos(pos_type pos, dstd::ios_base::openmode which = dstd::ios_base::in | dstd::ios_base::out )
	
	t.testEqual("pubseekpos:shouldReturnMinus1", sb.pubseekpos(10), -1);
	
	//int pubsync()
	
	t.testEqual("pubsync:shouldReturn0", sb.pubsync(), 0);
	
	//dstd::streamsize in_avail()
	
	t.testEqual("in_avail:shouldReturn0", sb.in_avail(), 0);
	
	//int_type snextc()
	
	t.testEqual("snextc:shouldReturnEOF", sb.snextc(), ct::eof());
	
	//int_type sbumpc()
	
	t.testEqual("sbumpc:shouldReturnEOF", sb.sbumpc(), ct::eof());
	
	//int_type sgetc()
	
	t.testEqual("sgetc:shouldReturnEOF", sb.sgetc(), ct::eof());
	
	//dstd::streamsize sgetn(char_type* s, dstd::streamsize count)
	
	{
		const size_t n = 5;
		char buffer[n];
		t.testEqual("sgetn:shouldReturn0", sb.sgetn(buffer, n), 0);
	}
	
	//int_type sputc(char_type c)
	
	t.testEqual("sputc:shouldReturnEOF", sb.sputc('a'), ct::eof());
	
	//dstd::streamsize sputn(const char_type* s, dstd::streamsize count)
	
	{
		const size_t n = 5;
		char buffer[n];
		t.testEqual("sputn:shouldReturn0", sb.sputn(buffer, n), 0);
	}
	
	//int_type sputbackc(char_type c)
	
	t.testEqual("sputbackc:shouldReturnEOF", sb.sputbackc('a'), ct::eof());
	
	//int_type sungetc()
	
	t.testEqual("sungetc:shouldReturnEOF", sb.sungetc(), ct::eof());
	
	
	
	t.report();
	return 0;
}
