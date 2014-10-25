#include "../src/string.hxx"
#include "tester.hxx"



std::ostream& operator<< (std::ostream& stream, const dstd::string& s)
{
	stream << s.c_str();
	return stream;
}


template <class Character> class Defaults;


template <>
class Defaults<char>
{
	public:
		typedef dstd::string string_type;
		string_type sc() { return string_type("P"); }
		string_type s0() { return string_type(); }
		string_type s1() { return string_type("Make"); }
		string_type s2() { return string_type(" it"); }
		string_type s3() { return string_type(" so!"); }
		string_type s()  { return string_type("Make it so!"); }
		string_type ss() { return string_type("Make it so!Make it so!"); }
		string_type s2b() { return string_type("it"); }
		string_type s3b() { return string_type(" so"); }
		string_type st1() { return string_type("Makeit so!"); }
		string_type st2() { return string_type("ake it so!"); }
		string_type st3bcde () { return string_type("bcde");}
		string_type st3bcd  () { return string_type("bcd"); }
		string_type st3abcd () { return string_type("abcd");}
		string_type st3abc  () { return string_type("abc"); }
		string_type st3ab   () { return string_type("ab");  }
		string_type st3zero () { return string_type("");    }
		char cc() { return 'P'; }
		const char* c0() { static const char ca[] = {'\0'}; return ca; }
		const char* c1() { static const char ca[] = {'M','a','k','e','\0'}; return ca; }
		const char* c2() { static const char ca[] = {' ','i','t','\0'}; return ca; }
		const char* c3() { static const char ca[] = {' ','s','o','!','\0'}; return ca; }
		const char* c () { static const char ca[] = {'M','a','k','e',' ','i','t',' ','s','o','!','\0'}; return ca; }
		size_t len1() { return 4; }
		size_t len2() { return 3; }
		size_t len3() { return 4; }
		size_t len()  { return this->len1() + this->len2() + this->len3(); }
};

template <>
class Defaults<wchar_t>
{
	public:
		typedef dstd::wstring string_type;
		string_type sc() { return string_type(L"P"); }
		string_type s0() { return string_type(); }
		string_type s1() { return string_type(L"Make"); }
		string_type s2() { return string_type(L" it"); }
		string_type s3() { return string_type(L" so!"); }
		string_type s()  { return string_type(L"Make it so!"); }
		string_type ss() { return string_type(L"Make it so!Make it so!"); }
		string_type s2b() { return string_type(L"it"); }
		string_type s3b() { return string_type(L" so"); }
		string_type st1() { return string_type(L"Makeit so!"); }
		string_type st2() { return string_type(L"ake it so!"); }
		string_type st3bcde () { return string_type(L"bcde");}
		string_type st3bcd  () { return string_type(L"bcd"); }
		string_type st3abcd () { return string_type(L"abcd");}
		string_type st3abc  () { return string_type(L"abc"); }
		string_type st3ab   () { return string_type(L"ab");  }
		string_type st3zero () { return string_type(L"");    }
		wchar_t cc() { return L'P'; }
		const wchar_t* c0() { static const wchar_t ca[] = {L'\0'}; return ca; }
		const wchar_t* c1() { static const wchar_t ca[] = {L'M',L'a',L'k',L'e',L'\0'}; return ca; }
		const wchar_t* c2() { static const wchar_t ca[] = {L' ',L'i',L't',L'\0'}; return ca; }
		const wchar_t* c3() { static const wchar_t ca[] = {L' ',L's',L'o',L'!',L'\0'}; return ca; }
		const wchar_t* c () { static const wchar_t ca[] = {L'M',L'a',L'k',L'e',L' ',L'i',L't',L' ',L's',L'o',L'!',L'\0'}; return ca; }
		size_t len1() { return 4; }
		size_t len2() { return 3; }
		size_t len3() { return 4; }
		size_t len()  { return this->len1() + this->len2() + this->len3(); }
};



template <class Character>
void testString(dstd::Tester& t)
{
	//dstd::Tester t("<string>");
	Defaults<Character> defaults;
	typedef Character char_type;
	typedef typename Defaults<Character>::string_type string_type;
	
	const string_type sc = defaults.sc();
	const string_type s0 = defaults.s0();
	const string_type s1 = defaults.s1();
	const string_type s2 = defaults.s2();
	const string_type s3 = defaults.s3();
	const string_type s = defaults.s();
	const string_type ss = defaults.ss();
	
	const string_type s2b = defaults.s2b();
	const string_type s3b = defaults.s3b();
	
	const Character cc = defaults.cc();
	const Character* c0 = defaults.c0();
	const Character* c1 = defaults.c1();
	const Character* c2 = defaults.c2();
	const Character* c3 = defaults.c3();
	const Character* c =  defaults.c();
	
	const size_t len1 = defaults.len1();
	const size_t len2 = defaults.len2();
	const size_t len3 = defaults.len3();
	const size_t len = defaults.len();
	
	//std::cout << s << std::endl;
	//std::cout << c << std::endl;
	
	// operator=
	
	{
		string_type x;
		
		// operator= (string)
		
		x = s0;
		t.testEqual("string::operator=(string) 0", x, s0);
		
		x = s;
		t.testEqual("string::operator=(string) 1", x, s);
	    
		// operator= (char*)
		
		x = c0;
		t.testEqual("string::operator=(char*) 0", x, s0);
		
		x = c;
		t.testEqual("string::operator=(char*) 1", x, s);
		
		// operator= (char)
		
		x = cc;
		t.testEqual("string::operator=(char)", x, sc);
	}
	
	// assign
	
	{
		string_type x;
		
		// basic_string& assign(size_type count, Character c)
		
		x.assign(0, cc);
		t.testEqual("string::assign(size,char) 0", x, s0);
		
		x.assign(1, cc);
		t.testEqual("string::assign(size,char) 1", x, sc);
		
		// basic_string& assign(const basic_string& s)
		
		x.assign(s0);
		t.testEqual("string::assign(string) 0", x, s0);
		
		x.assign(s);
		t.testEqual("string::assign(string) 1", x, s);
		
// TODO: x.assign(x)
		
		// basic_string& assign(const basic_string& s, size_type pos, size_type count)
		
		x.assign(s, 0, 0);
		t.testEqual("string::assign(string,size,size) 0", x, s0);
		
		x.assign(s, 0, string_type::npos);
		t.testEqual("string::assign(string,size,size) 1", x, s);
		
		x.assign(s, len1, len2);
		t.testEqual("string::assign(string,size,size) 2", x, s2);
		
		t.startTest("string::assign(string,size,size) 3");
		try { x.assign(s1, len1+2, len1); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("The wrong exception was thrown"); }
		
// TODO: x.assign(x, size, size)
		
		// basic_string& assign(const Character* s, size_type count)
		
		x.assign(0, 0);
		t.testEqual("string::assign(char*,size) 0", x, s0);
		
		x.assign(c, len1);
		t.testEqual("string::assign(char*,size) 1", x, s1);
		
		// basic_string& assign(const Character* s)
		
		x.assign(c);
		t.testEqual("string::assign(char*) 0", x, s);
		
		// basic_string& assign(InputIterator first, InputIterator last)
		
		x.assign(s.begin(), s.begin());
		t.testEqual("string::assign(it,it) 0", x, s0);
		
		x.assign(s.begin(), s.end());
		t.testEqual("string::assign(it,it) 1", x, s);
		
// TODO: x.assign(x.begin(), x.end())
	}
	
	// at
	
	t.startTest("string::at 1");
	try { Character xc = s.at(len + 3); ++xc; t.testFailed("No exception was thrown"); }
	catch( dstd::out_of_range& e ) { t.testPassed(); }
	catch( ... ) { t.testFailed("A wrong exception was thrown"); }
	
	t.testEqual("string::at 2", s.at(len/2), c[len/2]);
	
	{
		const Character xc = 'X';
		const size_t i = 4;
		string_type x(s);
		x.at(i) = xc;
		t.testEqual("string::at 3", x.at(i), xc);
	}
	
	// operator[]
	
	t.startTest("string::operator[] 1");
	try { Character xc = s[len + 3]; ++xc; t.testPassed(); }
	catch( ... ) { t.testFailed("An exception was thrown"); }
	
	t.testEqual("string::operator[] 2", s[len/2], c[len/2]);
	
	{
		const Character xc = 'X';
		const size_t i = 4;
		string_type x(s);
		x[i] = xc;
		t.testEqual("string::operator[] 3", x.at(i), xc);
	}
	
	// data
	
	// c_str
	
	// begin
	
	t.testEqual("string::begin", *(s.begin()), c[0]);
	
	// end
	
	t.testEqual("string::end", *(s.end()), '\0'); // hmm... I'm really testing implementation here, but whatever
	
	// rbegin
	
	// rend
	
	// empty
	
	t.testEqual("string::empty 1", s0.empty(), true);
	
	t.testEqual("string::empty 2", s.empty(), false);
	
	// size
	
	t.testEqual("string::size 1", s0.size(), 0);
	
	t.testEqual("string::size 2", s.size(), len);
	
	// length
	
	t.testEqual("string::length 1", s0.length(), 0);
	
	t.testEqual("string::length 2", s.length(), len);
	
	// max_size
	
	// reserve
	
	// capacity
	
	t.testGreaterThanOrEqual("string::capacity 1", s0.capacity(), 0);
	
	t.testGreaterThanOrEqual("string::capacity 2", s.capacity(), s.size());
	
	// clear
	
	{
		string_type x0;
		x0.clear();
		t.testEqual("string::clear 1", x0, s0);
		
		string_type x1(s);
		x1.clear();
		t.testEqual("string::clear 1", x1, s0);
	}
	
	// insert
	
	{
		const string_type make_so(s1 + s3);
		string_type x;
		
		// basic_string& insert(size_type index, size_type count, Character c)
		
		x = make_so;
		t.startTest("string::insert(size,size,char) 1");
		try { x.insert(x.length()+2, 0, char_type('X')); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		x = make_so;
		x.insert(len1, 0, char_type('X'));
		t.testEqual("string::insert(size,size,char) 1", x, make_so);
		
		x.insert(len1, 1, ' ');
		x.insert(len1+1, 1, 'i');
		x.insert(len1+2, 1, 't');
		t.testEqual("string::insert(size,size,char) 2", x, s);
		
		// basic_string& insert(size_type index, const Character* s)
		
		x = make_so;
		x.insert(len1, c2);
		t.testEqual("string::insert(size,char*) 1", x, s);
		
		x = s1;
		t.startTest("string::insert(size,char*) 2");
		try { x.insert(x.length()+2, c2); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		// basic_string& insert(size_type index, const Character* s, size_type count)
		
		x = make_so;
		x.insert(len1, c2, len2);
		t.testEqual("string::insert(size,char*,size) 1", x, s);
		
		x = make_so;
		x.insert(len1, c2, 0);
		t.testEqual("string::insert(size,char*,size) 2", x, make_so);
		
		x = s1;
		t.startTest("string::insert(size,char*,size) 3");
		try { x.insert(x.length()+2, c2, len2); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		// basic_string& insert(size_type index, const basic_string& s)
		
		x = s1;
		t.startTest("string::insert(size,string) 1");
		try { x.insert(x.length()+2, s2); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		x = make_so;
		x.insert(len1, s2);
		t.testEqual("string::insert(size,string) 2", x, s);
		
		x = make_so;
		x.insert(len1, s0);
		t.testEqual("string::insert(size,string) 3", x, make_so);
		
		// basic_string& insert(size_type index, const basic_string& s, size_type s_index, size_type count)
		
		x = s1;
		t.startTest("string::insert(size,string,size,size) 1");
		try { x.insert(x.length()+2, s2, 0, 2); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		x = s1;
		x.insert(2, s2, 0, 0);
		t.testEqual("string::insert(size,string,size,size) 2", x, s1);
		
		x = s1;
		t.startTest("string::insert(size,string,size,size) 3");
		try { x.insert(x.length()+2, s2, s2.length()+2, 2); t.testFailed("No exception was thrown"); }
		catch( dstd::out_of_range& e ) { t.testPassed(); }
		catch( ... ) { t.testFailed("A wrong exception was thrown"); }
		
		x = make_so;
		x.insert(len1, s, len1, len2);
		t.testEqual("string::insert(size,string,size,size) 4", x, s);
		
		x = make_so;
		x.insert(len1, s2, 0, 2*len2);
		t.testEqual("string::insert(size,string,size,size) 5", x, s);
		
		x = make_so;
		x.insert(len1, s2, 0, string_type::npos);
		t.testEqual("string::insert(size,string,size,size) 6", x, s);
		
		// basic_string& insert(iterator pos, Character c)
		
		x = make_so;
		x.insert(x.begin()+len1, ' ');
		x.insert(x.begin()+len1+1, 'i');
		x.insert(x.begin()+len1+2, 't');
		t.testEqual("string::insert(it,char)", x, s);
		
		// basic_string& insert(iterator pos, size_type count, Character c)
		
		x = make_so;
		x.insert(x.begin()+len1, 1, ' ');
		x.insert(x.begin()+len1+1, 1, 'i');
		x.insert(x.begin()+len1+2, 1, 't');
		t.testEqual("string::insert(it,size,char)", x, s);
		
		// basic_string& insert(iterator pos, InputIterator first, InputIterator last)
		
		x = make_so;
		x.insert(x.begin()+len1, s2.begin(), s2.end());
		t.testEqual("string::insert(it,it,it)", x, s);
	}
	
	// erase
	
	{
		string_type x;
		
		// basic_string& erase (size_type index = 0, size_type count = basic_string::npos)
		
		x = s;
		x.erase(len/2, 0);
		t.testEqual("string::erase(size,size) 0", x, s);
		
		x = s;
		x.erase(len1, string_type::npos);
		t.testEqual("string::erase(size,size) 1", x, s1);
		
		x = s;
		x.erase(len1, len2+len3);
		t.testEqual("string::erase(size,size) 2", x, s1);
		
		x = s;
		x.erase(0, len);
		t.testEqual("string::erase(size,size) 4", x, s0);
		
		x = s;
		x.erase(0, string_type::npos);
		t.testEqual("string::erase(size,size) 5", x, s0);
		
		x = s;
		x.erase(len1);
		t.testEqual("string::erase(size,size) 6", x, s1);
		
		x = s;
		x.erase();
		t.testEqual("string::erase(size,size) 7", x, s0);
		
		// iterator erase(iterator position)
		
		x = s;
		x.erase(x.begin() + len1);
		t.testEqual("string::erase(it) 1", x, defaults.st1() );
		
		x = s;
		x.erase(x.begin());
		t.testEqual("string::erase(it) 2", x, defaults.st2() );
		
		// iterator erase(iterator first, iterator last)
		
		x = s;
		x.erase(x.begin(), x.end());
		t.testEqual("string::erase(it,it) 1", x, s0);
		
		x = s;
		x.erase(x.begin() + len1, x.end());
		t.testEqual("string::erase(it,it) 2", x, s1);
		
		x = s;
		x.erase(x.begin() + len1, x.begin() + len1 + len2);
		t.testEqual("string::erase(it,it) 3", x, string_type(s1+s3));
		
		x = s;
		x.erase(x.begin() + 1, x.begin() + 1);
		t.testEqual("string::erase(it,it) 4", x, s);
	}
	
	// push_back
	
	{
		string_type x(s3b);
		x.push_back('!');
		t.testEqual("string::push_back", x, s3);
	}
	
	//append
	
	{
		string_type x;
		
		// basic_string& append(size_type count, Character c)
		
		x = s3b;
		x.append(1, '!');
		t.testEqual("string::append(size,char) 1", x, s3);
		
		x = s;
		x.append(0, char_type('x'));
		t.testEqual("string::append(size,char) 2", x, s);
		
		// basic_string& append(const basic_string& s)
		
		x = s1;
		x.append(s2).append(s3);
		t.testEqual("string::append(string) 1", x, s);
		
		x = s1;
		x.append(s0);
		t.testEqual("string::append(string) 2", x, s1);
		
		x = s;
		x.append(x);
		t.testEqual("string::append(string) 3", x, ss);
		
		// basic_string& append(const basic_string& s, size_type pos, size_type count)
		
		x = s1;
		x.append(s, len1, len2).append(s, len1+len2, len3);
		t.testEqual("string::append(string,size,size) 1", x, s);
		
		x = s1;
		x.append(s, len1, 0);
		t.testEqual("string::append(string,size,size) 2", x, s1);
		
		x = s1;
		x.append(s0, 0, 0);
		t.testEqual("string::append(string,size,size) 3", x, s1);
		
		x = s;
		x.append(x, 0, string_type::npos);
		t.testEqual("string::append(string,size,size) 4", x, ss);
		
		// basic_string& append(const Character* s, size_type count)
		
		x = s1;
		x.append(c2, len2).append(c3, len3);
		t.testEqual("string::append(char*,size) 1", x, s);
		
		x = s;
		x.append(c2, 0);
		t.testEqual("string::append(char*,size) 2", x, s);
		
		x = s;
		x.append(x.data(), x.size());
		t.testEqual("string::append(char*,size) 3", x, ss);
		
		// basic_string& append(const Character* s)
		
		x = s1;
		x.append(c2).append(c3);
		t.testEqual("string::append(char*) 1", x, s);
		
		x = s;
		x.append(x.data());
		t.testEqual("string::append(char*) 2", x, ss);
		
		// basic_string& append(InputIterator first, InputIterator last)
		
		x = s1;
		x.append(s.begin()+len1, s.begin()+len1+len2).append(s3.begin(), s3.end());
		t.testEqual("string::append(it,it) 1", x, s);
		
		x = s;
		x.append(s2.begin(), s2.begin());
		t.testEqual("string::append(it,it) 2", x, s);
		
		x = s;
		x.append(x.begin(), x.end());
		t.testEqual("string::append(it,it) 3", x, ss);
		
		x = s;
		x.append(x.begin(), x.end());
		t.testEqual("string::append(it,it) 4", x, ss);
	}
	
	// operator+=
	
	{
		string_type x;
		
		// basic_string& operator+= (const basic_string& s)
		
		x = s1;
		x += s2;
		x += s3;
		t.testEqual("string::operator+=(string) 1", x, s);
		
		x = s;
		x += s0;
		t.testEqual("string::operator+=(string) 2", x, s);
		
		x = s0;
		x += s;
		t.testEqual("string::operator+=(string) 3", x, s);
		
		// basic_string& operator+= (Character c)
		
		x = s3b;
		x += '!';
		t.testEqual("string::operator+=(char)", x, s3);
		
		// basic_string& operator+= (const Character* s)
		
		x = s1;
		x += c2;
		x += c3;
		t.testEqual("string::operator+=(char*) 1", x, s);
		
		x = s;
		x += c0;
		t.testEqual("string::operator+=(char*) 2", x, s);
		
		x = s0;
		x += c;
		t.testEqual("string::operator+=(char*) 3", x, s);
	}
	
	// compare
	
	{
		const string_type bcde = defaults.st3bcde();
		const string_type bcd  = defaults.st3bcd();
		const string_type abcd = defaults.st3abcd();
		const string_type abc  = defaults.st3abc();
		const string_type ab   = defaults.st3ab();
		const string_type zero = defaults.st3zero();
		
		// int compare(const basic_string& s) const
		
		t.testEqual("string::compare(string) 1", abc.compare(abcd), -1);
		t.testEqual("string::compare(string) 2", abc.compare(abc),   0);
		t.testEqual("string::compare(string) 3", abc.compare(ab),    1);
		t.testEqual("string::compare(string) 4", abc.compare(bcde), -1);
		
		t.testEqual("string::compare(string) 5", zero.compare(zero), 0);
		t.testEqual("string::compare(string) 6", zero.compare(abc), -1);
		t.testEqual("string::compare(string) 7", abc.compare(zero),  1);
		
		// int compare(size_type pos, size_type count, const basic_string s) const
		
		t.testEqual("string::compare(size,size,string)  1", abc.compare(0, 3, abcd), -1);
		t.testEqual("string::compare(size,size,string)  2", abc.compare(0, 3, abc),   0);
		t.testEqual("string::compare(size,size,string)  3", abc.compare(0, 3, ab),    1);
		t.testEqual("string::compare(size,size,string)  4", abc.compare(0, 3, bcde), -1);
		
		t.testEqual("string::compare(size,size,string)  5", zero.compare(0, 0, zero), 0);
		t.testEqual("string::compare(size,size,string)  6", zero.compare(0, 0, abc), -1);
		t.testEqual("string::compare(size,size,string)  7", abc.compare(0, 3, zero),  1);
		
		t.testEqual("string::compare(size,size,string)  8", abcd.compare(1, 3, bcde), -1);
		t.testEqual("string::compare(size,size,string)  9", abcd.compare(1, 3, bcd),   0);
		t.testEqual("string::compare(size,size,string) 10", abcd.compare(1, 3, abc),   1);
		
		// int compare(size_type pos, size_type count, const basic_string s, size_type s_pos, size_type s_count) const
		
		t.testEqual("string::compare(size,size,string,size,size) 1", abc.compare(0, 3, abcd, 0, 4), -1);
		t.testEqual("string::compare(size,size,string,size,size) 2", abc.compare(0, 3, abcd, 0, 3),  0);
		t.testEqual("string::compare(size,size,string,size,size) 3", abc.compare(0, 3, abcd, 0, 2),  1);
		t.testEqual("string::compare(size,size,string,size,size) 4", abc.compare(0, 3, abcd, 1, 3), -1);
		t.testEqual("string::compare(size,size,string,size,size) 5", abc.compare(2, 2, abcd, 0, 3),  1);
		
		// int compare(const Character* s) const
		
		t.testEqual("string::compare(char*) 1", abc.compare(abcd.data()), -1);
		t.testEqual("string::compare(char*) 2", abc.compare(abc.data()),   0);
		t.testEqual("string::compare(char*) 3", abc.compare(ab.data()),    1);
		t.testEqual("string::compare(char*) 4", abc.compare(bcde.data()), -1);
		
		t.testEqual("string::compare(char*) 5", zero.compare(zero.data()), 0);
		t.testEqual("string::compare(char*) 6", zero.compare(abc.data()), -1);
		t.testEqual("string::compare(char*) 7", abc.compare(zero.data()),  1);
		
		// int compare(size_type pos, size_type count, const Character* s) const
		
		t.testEqual("string::compare(size,size,char*)  1", abc.compare( 0, 3, abcd.data()), -1);
		t.testEqual("string::compare(size,size,char*)  2", abc.compare( 0, 3, abc.data()),   0);
		t.testEqual("string::compare(size,size,char*)  3", abc.compare( 0, 3, ab.data()),    1);
		t.testEqual("string::compare(size,size,char*)  4", abc.compare( 0, 3, bcde.data()), -1);
		t.testEqual("string::compare(size,size,char*)  5", zero.compare(0, 0, zero.data()), 0);
		t.testEqual("string::compare(size,size,char*)  6", zero.compare(0, 0, abc.data()), -1);
		t.testEqual("string::compare(size,size,char*)  7", abc.compare( 0, 3, zero.data()),  1);
		t.testEqual("string::compare(size,size,char*)  8", abcd.compare(1, 3, bcde.data()), -1);
		t.testEqual("string::compare(size,size,char*)  9", abcd.compare(1, 3, bcd.data()),   0);
		t.testEqual("string::compare(size,size,char*) 10", abcd.compare(1, 3, abc.data()),   1);
		
		// int compare(size_type pos, size_type count, const Character* s, size_type s_count) const
		
		t.testEqual("string::compare(size,size,char*,size) 1", abc.compare(0, 3, abcd.data(), 4), -1);
		t.testEqual("string::compare(size,size,char*,size) 2", abc.compare(0, 3, abcd.data(), 3),  0);
		t.testEqual("string::compare(size,size,char*,size) 3", abc.compare(0, 3, abcd.data(), 2),  1);
		t.testEqual("string::compare(size,size,char*,size) 4", abc.compare(0, 3, abcd.data()+1, 3), -1);
		t.testEqual("string::compare(size,size,char*,size) 5", abc.compare(2, 2, abcd.data(), 3),  1);
	}
}


int main()
{
	dstd::Tester tc("<string> string");
	testString<char>(tc);
	dstd::Tester tw("<string> wstring");
	testString<wchar_t>(tw);
	
	tc.report();
	tw.report();
	
	return 0;
}
