#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/bitset.hxx"



template <size_t N>
std::ostream& operator<<(std::ostream& stream, const dstd::bitset<N>& b)
{
	std::basic_string<char> s = b.template to_string< char, std::char_traits<char>, std::allocator<char> >();
	stream << "dstd::bitset(" << s << ")";
	
	size_t n = CHAR_BIT;
	while( n < N )
	{
		n += CHAR_BIT;
	}
	stream << "[";
	for(size_t i = 1; i <= n; ++i)
	{
		if( (n-i) == N-1 ) stream << "|";
		if( ((i-1) % CHAR_BIT) == 0 ) stream << " ";
		stream << b[n-i];
	}
	stream << " ]";
	
	return stream;
}



template <size_t S>
dstd::bitset<S> get_all_bitset()
{
	dstd::bitset<S> b;
	b.set();
	return b;
}


template <size_t S>
void testBitset(dstd::Tester& t)
{
	const dstd::bitset<S> all_bits = get_all_bitset<S>();
	const dstd::bitset<S> no_bits;
	
	
	// Default Constructor
	
	{
		const std::string test_name = "bitset::bitset()";
		try
		{
			dstd::bitset<S> b;
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "An exception was thrown");
		}
	}
	
	// ULL Constructor
	
	{
		const dstd::bitset<S> b_0;
		
		const unsigned long long a = 2;
		const unsigned long long b = static_cast<unsigned char>( -1 );
		const unsigned long long c = 0b101010101010101010101010;
		
		const std::string test_exception = "An exception was thrown";
		const std::string test_name_0 = "bitset::bitset(ull) ";
		std::string test_name;
		
		test_name = test_name_0 + "1a";
		try
		{
			dstd::bitset<S> b_1(a);
			t.registerTestPassed(test_name);
			t.testNotEqual(test_name_0 + "2a", b_1, b_0);
		}
		catch(...) { t.registerTestFailed(test_name, test_exception); }
		
		test_name = test_name_0 + "1b";
		try
		{
			dstd::bitset<S> b_1(b);
			t.registerTestPassed(test_name);
			t.testNotEqual(test_name_0 + "2b", b_1, b_0);
		}
		catch(...) { t.registerTestFailed(test_name, test_exception); }
		
		test_name = test_name_0 + "1c";
		try
		{
			dstd::bitset<S> b_1(c);
			t.registerTestPassed(test_name);
			t.testNotEqual(test_name_0 + "2c", b_1, b_0);
		}
		catch(...) { t.registerTestFailed(test_name, test_exception); }
	}
	
	// TODO: String Constructor
	
	// bool operator[]
	
	{
		bool pass0 = true, pass1 = true, pass2 = true, pass3 = true;
		const dstd::bitset<S> b0, b1(1), b2(4), b3(all_bits);
		for(size_t i = 0; i < S; ++i)
		{
			if( b0[i] ) pass0 = false;
			if( b1[i] != (i == 0) ) pass1 = false;
			if( b2[i] != (i == 2) ) pass2 = false;
			if( ! b3[i] ) pass3 = false;
		}
		t.testEqual("bitset::operator[](bool) 0", pass0, true);
		t.testEqual("bitset::operator[](bool) 1", pass1, true);
		t.testEqual("bitset::operator[](bool) 2", pass2, true);
		t.testEqual("bitset::operator[](bool) 3", pass3, true);
	}
	
	// reference operator[]
	
	{
		bool pass = true;
		dstd::bitset<S> b1, b2, b3(all_bits);
		const dstd::bitset<S> b2_1(all_bits), b3_0;
		for(size_t i = 0; i < S; ++i)
		{
			b1[i] = ((i % 3) == 0);
			if( b1[i] != ((i % 3) == 0) ) pass = false;
			
			b2[i] = true;
			
			b3[i] = false;
		}
		t.testEqual("bitset::operator[](ref) 1", pass, true);
		t.testEqual("bitset::operator[](ref) 2", b2, b2_1);
		t.testEqual("bitset::operator[](ref) 3", b3, b3_0);
	}
	
	// test
	
	{
		const dstd::bitset<S> b0, b1(all_bits), b2(2398723940134);
		bool pass0 = true, pass1 = true, pass2 = true;
		for(size_t i = 0; i < S; ++i)
		{
			if( b0.test(i) ) pass0 = false;
			if( ! b1.test(i) ) pass1 = false;
			if( b2.test(i) != b2[i] ) pass2 = false;
		}
		t.testEqual("bitset::test 0", pass0, true);
		t.testEqual("bitset::test 1", pass1, true);
		t.testEqual("bitset::test 2", pass2, true);
	}
	
	// any
	
	{
		const dstd::bitset<S> b0, b1(all_bits), b2(2398723940134);
		
		t.testEqual("bitset::any 0", b0.any(), false);
		t.testEqual("bitset::any 1", b1.any(), true);
		t.testEqual("bitset::any 2", b2.any(), true);
	}
	
	// none
	
	{
		const dstd::bitset<S> b0, b1(all_bits), b2(2398723940134);
		
		t.testEqual("bitset::none 0", b0.none(), true);
		t.testEqual("bitset::none 1", b1.none(), false);
		t.testEqual("bitset::none 2", b2.none(), false);
	}
	
	// count
	
	{
		const size_t zero = 0;
		const dstd::bitset<S> b0, b1(all_bits);
		t.testEqual("bitset::count 0", b0.count(), zero);
		t.testEqual("bitset::count 1", b1.count(), S);
		
		size_t c2 = 0, c3 = 0;
		dstd::bitset<S> b2, b3;
		for(size_t i = 0; i < S; ++i)
		{
			if( i % 2)
			{
				b2[i] = true;
				++c2;
			}
			
			if( i % 3)
			{
				b3[i] = true;
				++c3;
			}
		}
		
		t.testEqual("bitset::count 2", b2.count(), c2);
		t.testEqual("bitset::count 3", b3.count(), c3);
	}
	
	// size
	
	{
		const dstd::bitset<S> b0, b1(all_bits), b2(2398723940134);
		t.testEqual("bitset::size 0", b0.size(), S);
		t.testEqual("bitset::size 1", b1.size(), S);
		t.testEqual("bitset::size 2", b2.size(), S);
	}
	
	// bitwise and assignment
	
	{
		const unsigned long ul1 = 0b1010101010101010101;
		const unsigned long ul2 = 0b1101101101101101101;
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0;
			
			b1 &= b0;
			t.testEqual("bitset::operator&= 1a", b1, b0);
			
			b2 &= b0;
			t.testEqual("bitset::operator&= 2a", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1_0(b1), b2_0(b2);
			const dstd::bitset<S> b0(all_bits);
			
			b1 &= b0;
			t.testEqual("bitset::operator&= 1b", b1, b1_0);
			
			b2 &= b0;
			t.testEqual("bitset::operator&= 2b", b2, b2_0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1_0(b1), b2_0(b2);
			
			b1 &= b1_0;
			t.testEqual("bitset::operator&= 1c", b1, b1_0);
			
			b2 &= b2_0;
			t.testEqual("bitset::operator&= 2c", b2, b2_0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0, b1x(~ul1), b2x(~ul2);
			
			b1 &= b1x;
			t.testEqual("bitset::operator&= 1d", b1, b0);
			
			b2 &= b2x;
			t.testEqual("bitset::operator&= 2d", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1c(ul1), b2c(ul2);
			const dstd::bitset<S> b12(ul1 & ul2), b21(ul2 & ul1);
			
			b1 &= b2c;
			t.testEqual("bitset::operator&= 1e", b1, b12);
			
			b2 &= b1c;
			t.testEqual("bitset::operator&= 2e", b2, b21);
		}
	}
	
	// bitwise or assignment
	
	{
		const unsigned long ul1 = 0b1010101010101010101;
		const unsigned long ul2 = 0b1101101101101101101;
		const unsigned long ulx = -1;
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0, b1_0(b1), b2_0(b2);
			
			b1 |= b0;
			t.testEqual("bitset::operator|= 1a", b1, b1_0);
			
			b2 |= b0;
			t.testEqual("bitset::operator|= 2a", b2, b2_0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1_0(b1), b2_0(b2);
			const dstd::bitset<S> b0(all_bits);
			
			b1 |= b0;
			t.testEqual("bitset::operator|= 1b", b1, b0);
			
			b2 |= b0;
			t.testEqual("bitset::operator|= 2b", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1_0(b1), b2_0(b2);
			
			b1 |= b1_0;
			t.testEqual("bitset::operator|= 1c", b1, b1_0);
			
			b2 |= b2_0;
			t.testEqual("bitset::operator|= 2c", b2, b2_0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0(ulx), b1x(~ul1), b2x(~ul2);
			
			b1 |= b1x;
			t.testEqual("bitset::operator|= 1d", b1, b0);
			
			b2 |= b2x;
			t.testEqual("bitset::operator|= 2d", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1c(ul1), b2c(ul2);
			const dstd::bitset<S> b12(ul1 | ul2), b21(ul2 | ul1);
			
			b1 |= b2c;
			t.testEqual("bitset::operator|= 1e", b1, b12);
			
			b2 |= b1c;
			t.testEqual("bitset::operator|= 2e", b2, b21);
		}
	}
	
	// bitwise xor assignment
	
	{
		const unsigned long ul1 = 0b1010101010101010101;
		const unsigned long ul2 = 0b1101101101101101101;
		const unsigned long ulx = -1;
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0, b1_0(b1), b2_0(b2);
			
			b1 ^= b0;
			t.testEqual("bitset::operator^= 1a", b1, b1_0);
			
			b2 ^= b0;
			t.testEqual("bitset::operator^= 2a", b2, b2_0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0(ulx), b1x(~ul1), b2x(~ul2);
			
			b1 ^= b0;
			t.testEqual("bitset::operator^= 1b", b1, b1x);
			
			b2 ^= b0;
			t.testEqual("bitset::operator^= 2b", b2, b2x);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1_0(b1), b2_0(b2), b0;
			
			b1 ^= b1_0;
			t.testEqual("bitset::operator^= 1c", b1, b0);
			
			b2 ^= b2_0;
			t.testEqual("bitset::operator^= 2c", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b0(ulx), b1x(~ul1), b2x(~ul2);
			
			b1 ^= b1x;
			t.testEqual("bitset::operator^= 1d", b1, b0);
			
			b2 ^= b2x;
			t.testEqual("bitset::operator^= 2d", b2, b0);
		}
		
		{
			dstd::bitset<S> b1(ul1), b2(ul2);
			const dstd::bitset<S> b1c(ul1), b2c(ul2);
			const dstd::bitset<S> b12(ul1 ^ ul2), b21(ul2 ^ ul1);
			
			b1 ^= b2c;
			t.testEqual("bitset::operator^= 1e", b1, b12);
			
			b2 ^= b1c;
			t.testEqual("bitset::operator^= 2e", b2, b21);
		}
	}
	
	// bitwise flip
	
	{
		const unsigned long ul1 = -1;
		const unsigned long ul2 = 0b1010101010101010101;
		const unsigned long ul3 = 0b1101101101101101101;
		
		const dstd::bitset<S> b0, b1(all_bits), b2(ul2), b3(ul3);
		const dstd::bitset<S> b0x(all_bits), b1x;
		
		dstd::bitset<S> b2x, b3x;
		for(size_t i = 0; i < S; ++i)
		{
			b2x[i] = ! b2[i];
			b3x[i] = ! b3[i];
		}
		
		t.testEqual("bitset::operator~ 0", ~b0, b0x);
		t.testEqual("bitset::operator~ 1", ~b1, b1x);
		t.testEqual("bitset::operator~ 2", ~b2, b2x);
		t.testEqual("bitset::operator~ 3", ~b3, b3x);
	}
	
	// left shift assignment
	
	{
		const size_t s1 = 1, s2 = 2, s3 = (S/2), s4 = (S-1), s5 = S, s6 = S+1;
		const unsigned long u = 0b10110111011110111110;
		const unsigned long u1 = u << s1;
		const unsigned long u2 = u << s2;
		const unsigned long u3 = u << s3;
		const unsigned long u4 = u << s4;
		const unsigned long u5 = u << s5;
		
		const dstd::bitset<S> b0(u), b1(u1), b2(u2), b3(u3), b4(u4), b5(u5), b6(0);
		
		{
			dstd::bitset<S> b(u);
			b <<= 0;
			t.testEqual("bitset::operator<<= 0", b, b0);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s1;
			t.testEqual("bitset::operator<<= 1", b, b1);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s2;
			t.testEqual("bitset::operator<<= 2", b, b2);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s3;
			t.testEqual("bitset::operator<<= 3", b, b3);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s4;
			t.testEqual("bitset::operator<<= 4", b, b4);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s5;
			t.testEqual("bitset::operator<<= 5", b, b5);
		}
		
		{
			dstd::bitset<S> b(u);
			b <<= s6;
			t.testEqual("bitset::operator<<= 6", b, b6);
		}
	}
	
	// right shift assignment
	
	{
		const size_t s1 = 1, s2 = 2, s3 = (S/2), s4 = (S-1), s5 = S, s6 = S+1;
		const unsigned long u = 0b10110111011110111110;
		const unsigned long mask = ~(static_cast<unsigned long>(-1) << S);
		const unsigned long u1 = (u & mask) >> s1;
		const unsigned long u2 = (u & mask) >> s2;
		const unsigned long u3 = (u & mask) >> s3;
		const unsigned long u4 = (u & mask) >> s4;
		const unsigned long u5 = (u & mask) >> s5;
		
		const dstd::bitset<S> b0(u), b1(u1), b2(u2), b3(u3), b4(u4), b5(u5), b6(0);
		
		{
			dstd::bitset<S> b(u);
			b >>= 0;
			t.testEqual("bitset::operator>>= 0", b, b0);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s1;
			t.testEqual("bitset::operator>>= 1", b, b1);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s2;
			t.testEqual("bitset::operator>>= 2", b, b2);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s3;
			t.testEqual("bitset::operator>>= 3", b, b3);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s4;
			t.testEqual("bitset::operator>>= 4", b, b4);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s5;
			t.testEqual("bitset::operator>>= 5", b, b5);
		}
		
		{
			dstd::bitset<S> b(u);
			b >>= s6;
			t.testEqual("bitset::operator>>= 6", b, b6);
		}
	}
	
	// left shift
	
	{
		const size_t s1 = 1, s2 = 2, s3 = (S/2), s4 = (S-1), s5 = S, s6 = S+1;
		const unsigned long u = 0b10110111011110111110;
		const unsigned long u1 = u << s1;
		const unsigned long u2 = u << s2;
		const unsigned long u3 = u << s3;
		const unsigned long u4 = u << s4;
		const unsigned long u5 = u << s5;
		
		const dstd::bitset<S> b0(u), b1(u1), b2(u2), b3(u3), b4(u4), b5(u5), b6(0);
		
		{
			dstd::bitset<S> b = b0 << 0;
			t.testEqual("bitset::operator<< 0", b, b0);
		}
		
		{
			dstd::bitset<S> b = b0 << s1;
			t.testEqual("bitset::operator<< 1", b, b1);
		}
		
		{
			dstd::bitset<S> b = b0 << s2;
			t.testEqual("bitset::operator<< 2", b, b2);
		}
		
		{
			dstd::bitset<S> b = b0 << s3;
			t.testEqual("bitset::operator<< 3", b, b3);
		}
		
		{
			dstd::bitset<S> b = b0 << s4;
			t.testEqual("bitset::operator<< 4", b, b4);
		}
		
		{
			dstd::bitset<S> b = b0 << s5;
			t.testEqual("bitset::operator<< 5", b, b5);
		}
		
		{
			dstd::bitset<S> b = b0 << s6;
			t.testEqual("bitset::operator<< 6", b, b6);
		}
	}
	
	// right shift
	
	{
		const size_t s1 = 1, s2 = 2, s3 = (S/2), s4 = (S-1), s5 = S, s6 = S+1;
		const unsigned long u = 0b10110111011110111110;
		const unsigned long mask = ~(static_cast<unsigned long>(-1) << S);
		const unsigned long u1 = (u & mask) >> s1;
		const unsigned long u2 = (u & mask) >> s2;
		const unsigned long u3 = (u & mask) >> s3;
		const unsigned long u4 = (u & mask) >> s4;
		const unsigned long u5 = (u & mask) >> s5;
		
		const dstd::bitset<S> b0(u), b1(u1), b2(u2), b3(u3), b4(u4), b5(u5), b6(0);
		
		{
			dstd::bitset<S> b = b0 >> 0;
			t.testEqual("bitset::operator>> 0", b, b0);
		}
		
		{
			dstd::bitset<S> b = b0 >> s1;
			t.testEqual("bitset::operator>> 1", b, b1);
		}
		
		{
			dstd::bitset<S> b = b0 >> s2;
			t.testEqual("bitset::operator>> 2", b, b2);
		}
		
		{
			dstd::bitset<S> b = b0 >> s3;
			t.testEqual("bitset::operator>> 3", b, b3);
		}
		
		{
			dstd::bitset<S> b = b0 >> s4;
			t.testEqual("bitset::operator>> 4", b, b4);
		}
		
		{
			dstd::bitset<S> b = b0 >> s5;
			t.testEqual("bitset::operator>> 5", b, b5);
		}
		
		{
			dstd::bitset<S> b = b0 >> s6;
			t.testEqual("bitset::operator>> 6", b, b6);
		}
	}
	
	// set
	
	{
		const dstd::bitset<S> bx(all_bits);
		dstd::bitset<S> b1, b2(all_bits), b3(23984273894);
		
		b1.set();
		t.testEqual("bitset::set() 1", b1, bx);
		
		b2.set();
		t.testEqual("bitset::set() 2", b2, bx);
		
		b3.set();
		t.testEqual("bitset::set() 3", b3, bx);
	}
	
	// set i bool
	
	{
		bool pass1 = true, pass2 = true;
		dstd::bitset<S> b1, b2(3458937845), b3(2349072389472), b4(98548924), b5(234982705);
		const dstd::bitset<S> b0, bx(all_bits);
		
		for(size_t i = 0; i < S; ++i)
		{
			if( i%3 == 0 ) b1.set(i);
			b2.set( i, (i%3 == 0) );
			
			if( b1[i] != ((i % 3) == 0) ) pass1 = false;
			if( b2[i] != ((i % 3) == 0) ) pass2 = false;
			
			b3.set(i);
			b4.set(i, true);
			b5.set(i, false);
		}
		t.testEqual("bitset::set(i,bool) 1", pass1, true);
		t.testEqual("bitset::set(i,bool) 2", pass2, true);
		t.testEqual("bitset::set(i,bool) 3", b3, bx);
		t.testEqual("bitset::set(i,bool) 3", b4, bx);
		t.testEqual("bitset::set(i,bool) 3", b5, b0);
	}
	
	// reset
	
	{
		const dstd::bitset<S> bx;
		dstd::bitset<S> b1, b2(all_bits), b3(23984273894);
		
		b1.reset();
		t.testEqual("bitset::reset() 1", b1, bx);
		
		b2.reset();
		t.testEqual("bitset::reset() 2", b2, bx);
		
		b3.reset();
		t.testEqual("bitset::reset() 3", b3, bx);
	}
	
	// reset i bool
	
	{
		bool pass = true;
		dstd::bitset<S> b1(all_bits), b2(3458937845);
		const dstd::bitset<S> b0;
		
		for(size_t i = 0; i < S; ++i)
		{
			if( i%3 == 0 ) b1.reset(i);
			if( b1[i] != ((i % 3) != 0) ) pass = false;
			
			b2.reset(i);
		}
		t.testEqual("bitset::reset(i,bool) 1", pass, true);
		t.testEqual("bitset::reset(i,bool) 2", b2, b0);
	}
	
	// flip
	
	{
		unsigned long u = 20974509834234;
		const dstd::bitset<S> b0, b1(all_bits), bu(u), bv(~u);
		
		dstd::bitset<S> bx0(all_bits), bx1, bxu, bxv;
		for(size_t i = 0; i < S; ++i)
		{
			bxu[i] = ! bu[i];
			bxv[i] = ! bv[i];
		}
		
		bx0.flip();
		bx1.flip();
		bxu.flip();
		bxv.flip();
		
		t.testEqual("bitset::flip 0", bx0, b0);
		t.testEqual("bitset::flip 1", bx1, b1);
		t.testEqual("bitset::flip 2", bxu, bu);
		t.testEqual("bitset::flip 3", bxv, bv);
	}
	
	// flip bit
	
	{
		unsigned long u = 20974509834234;
		const dstd::bitset<S> b0, b1(all_bits), bu(u), bv(~u);
		
		dstd::bitset<S> bx0(all_bits), bx1, bxu, bxv;
		for(size_t i = 0; i < S; ++i)
		{
			bxu[i] = ! bu[i];
			bxv[i] = ! bv[i];
		}
		
		for(size_t i = 0; i < S; ++i)
		{
			bx0.flip(i);
			bx1.flip(i);
			bxu.flip(i);
			bxv.flip(i);
		}
		
		t.testEqual("bitset::flip(bit) 0", bx0, b0);
		t.testEqual("bitset::flip(bit) 1", bx1, b1);
		t.testEqual("bitset::flip(bit) 2", bxu, bu);
		t.testEqual("bitset::flip(bit) 3", bxv, bv);
	}
	
	// TODO: to_string
	//{
	//	const std::string empty(S, '0');
	//	
	//	const std::string s0 = empty;
	//	const std::string s1 = empty + "1101";
	//	const std::string s2 = empty + "11001010";
	//	const std::string s3 = empty + "0101100111001011011011100101001010110011111001010";
	//	
	//	const dstd::bitset<S> b0(s0), b1(s1), b2(s2), b3(s3);
	//	
	//	t.testEqual("bitset::to_string 0", std::string( b0.template to_string<char,std::char_traits<char>,std::allocator<char> >() ), s0);
	//	t.testEqual("bitset::to_string 1", std::string( b1.template to_string<char,std::char_traits<char>,std::allocator<char> >() ), s1);
	//	t.testEqual("bitset::to_string 2", std::string( b2.template to_string<char,std::char_traits<char>,std::allocator<char> >() ), s2);
	//	t.testEqual("bitset::to_string 3", std::string( b3.template to_string<char,std::char_traits<char>,std::allocator<char> >() ), s3);
	//}
	
	// to_ulong
	
	{
		const unsigned long u0 = 0;
		
		const unsigned long mask = ~(static_cast<unsigned long>(-1) << S);
		const unsigned long un = 0b10110111011110111110 & mask;
		const unsigned long ux = static_cast<unsigned long>(-1) & mask;
		
		const dstd::bitset<S> b0(0), bn(un);
		const dstd::bitset<S> bx(all_bits);
		
		const std::string test_exception = "An exception was thrown";
		std::string test_name;
		
		test_name = "bitset::to_ulong 1a";
		try
		{
			const unsigned long ub0 = b0.to_ulong();
			t.registerTestPassed(test_name);
			t.testEqual("bitset::to_ulong 1b", ub0, u0);
		}
		catch(...) { t.registerTestFailed(test_name, test_exception); }
		
		test_name = "bitset::to_ulong 2a";
		try
		{
			const unsigned long ubn = bn.to_ulong();
			t.registerTestPassed(test_name);
			t.testEqual("bitset::to_ulong 2b", ubn, un);
		}
		catch(...) { t.registerTestFailed(test_name, test_exception); }
		
		test_name = "bitset::to_ulong 3a";
		try
		{
			const unsigned long ubx = bx.to_ulong();
			if( S < std::numeric_limits<unsigned long>::digits )
			{
				t.registerTestPassed(test_name);
				t.testEqual("bitset::to_ulong 3b", ubx, ux);
			}
			else
			{
				t.registerTestFailed(test_name, "No exception was thrown");
			}
		}
		catch(dstd::overflow_error& e)
		{
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "An incorrect exception was thrown");
		}
	}
}


int main()
{
	dstd::Tester t1("<bitset> [1]");
	dstd::Tester t2("<bitset> [2]");
	dstd::Tester t3("<bitset> [3]");
	dstd::Tester t4("<bitset> [4]");
	dstd::Tester t5("<bitset> [5]");
	
	const size_t s1 = CHAR_BIT/2; // less than 1 byte
	const size_t s2 = CHAR_BIT; // 1 byte
	const size_t s3 = 3*CHAR_BIT; // integer number of bytes
	const size_t s4 = (2*CHAR_BIT) + (CHAR_BIT/2); // non-integer number of bytes
	const size_t s5 = std::numeric_limits<unsigned long>::digits + 12;
	
	testBitset<s1>( t1 );
	testBitset<s2>( t2 );
	testBitset<s3>( t3 );
	testBitset<s4>( t4 );
	testBitset<s5>( t5 );
	
	t1.report();
	t2.report();
	t3.report();
	t4.report();
	t5.report();
	
	return 0;
}
