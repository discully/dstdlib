#include <cstdlib>
#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/map.hxx"


template <class Key, class T>
std::ostream& operator<<(std::ostream& stream, const dstd::map<Key,T>& m)
{
	stream << "dstd::map[";
	for(typename dstd::map<Key,T>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		stream << "(" << it->first << "," << it->second << "), ";
	}
	stream << "]";
	return stream;
}



int main()
{
	dstd::Tester t("<map> map");
	
	
	//
	// Map
	
	//(constructor)
	
	{
		const std::string test_name("map::map()");
		try
		{
			dstd::map<double,int> m();
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	{
		const std::string test_name("map::map(c,a)");
		try
		{
			dstd::less<double> c;
			dstd::allocator<int> a;
			dstd::map<double,int> m(c, a);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	{
		dstd::map<double, int> m1;
		const double x = 3.14;
		for(unsigned int i = 0; i < 10; ++i)
		{
			m1[x*i] = i;
		}
		
		const std::string test_name("map::map(first,last) 1");
		try
		{
			dstd::map<double,int> m2(m1.begin(),m1.end());
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
		
		dstd::map<double,int> m2(m1.begin(),m1.end());
		t.testEqual("map::map(first,last) 2", m1, m2);
	}
	
	{
		dstd::map<double, int> m1;
		const double x = 3.14;
		for(unsigned int i = 0; i < 10; ++i)
		{
			m1[x*i] = i;
		}
		
		const std::string test_name("map::map(map) 1");
		try
		{
			dstd::map<double,int> m2(m1);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
		
		dstd::map<double,int> m2(m1);
		t.testEqual("map::map(map) 2", m1, m2);
	}
	
	//
	// Constructors work, so take time-out to make some reference maps for later tests
	
	dstd::map<int, double> empty;
	const dstd::map<int, double> const_empty;
	const size_t empty_size = 0;
	
	const unsigned int a_n = 20;
	dstd::map<int, double> a_construction;
	for(unsigned int i = 0; i < a_n; ++i)
	{
		a_construction[rand()] = 3.14 * rand();
	}
	const dstd::map<int,double> a( a_construction );
	
	
	const unsigned int b_n = 20;
	dstd::map<int, int> b_construction;
	for(unsigned int i = 0; i < b_n; ++i)
	{
		b_construction[i] = i;
	}
	const dstd::map<int,int> b( b_construction );
	
	unsigned int c_n = b_n;
	dstd::map<int,int> c(b);
	
	// Ok, back to testing...
	
	
	//(destructor)
	
	{
		const std::string test_name("map::~map");
		try
		{
			dstd::map<int, double>* m = new dstd::map<int, double>(a.begin(), a.end());
			delete(m);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	//operator=
	
	{
		dstd::map<int,double> empty_copy = empty;
		t.testEqual("map::operator= [empty]", empty_copy, empty);
	}
	
	{
		dstd::map<int,double> a_copy = a;
		t.testEqual("map::operator= [double,const]", a_copy, a);
	}
	
	{
		dstd::map<int,int> b_copy = b;
		t.testEqual("map::operator= [int,const]", b_copy, b);
	}
	
	{
		dstd::map<int,int> c_copy = c;
		t.testEqual("map::operator= [int,normal]", c_copy, c);
	}
	
	
	//begin
	
	t.testEqual( "map::begin [empty]", empty.begin(), empty.end());
	
	t.testEqual( "map::begin [const]", *(b.begin()), dstd::pair<int,int>(0,0) );
	
	t.testEqual( "map::begin [normal]", *(c.begin()), dstd::pair<int,int>(0,0) );
	
	
	//end
	
	t.testEqual( "map::end [empty]", empty.end(), empty.begin());
	
	{
		dstd::map<int,int>::const_iterator it = b.begin();
		for(unsigned int i = 0; i < b_n; ++i)
		{
			++it;
		}
		t.testEqual( "map::end [const]", b.end(), it );
	}
	
	{
		dstd::map<int,int>::iterator it = c.begin();
		for(unsigned int i = 0; i < c_n; ++i)
		{
			++it;
		}
		t.testEqual( "map::end [normal]", c.end(), it );
	}
	
	
	//rbegin
	
	t.testEqual( "map::rbegin [empty]", empty.rbegin(), empty.rend() );
	
	{
		dstd::map<int, double>::const_iterator a_fwd = a.end();
		--a_fwd;
		dstd::map<int, double>::const_reverse_iterator a_bwd = a.rbegin();
		
		t.testEqual("map::rbegin 1" , *a_bwd, *a_fwd);
	}
	
	{
		dstd::map<int,int>::const_reverse_iterator it = b.rend();
		for(unsigned int i = 0; i < b_n; ++i)
		{
			--it;
		}
		t.testEqual( "map::rbegin 3 [const]", b.rbegin(), it );
	}
	
	{
		dstd::map<int,int>::reverse_iterator it = c.rend();
		for(unsigned int i = 0; i < c_n; ++i)
		{
			--it;
		}
		dstd::map<int,int>::reverse_iterator it_begin = c.rbegin();
		t.testEqual( "map::rbegin 3 [normal]", it_begin, it );
	}
	
	
	//rend
	
	t.testEqual( "map::rend [empty]", empty.rend(), empty.rbegin() );
	
	{
		dstd::map<int, double>::const_reverse_iterator a_rit = a.rend();
		--a_rit;
		t.testEqual( "map::rend [const]", *(a_rit), *(a.begin()) );
	}
	
	{
		dstd::map<int, int>::reverse_iterator c_rit = c.rend();
		--c_rit;
		t.testEqual( "map::rend [normal]", *(c_rit), *(c.begin()) );
	}
	
	
	// empty
	
	t.testEqual( "map::empty [empty]", empty.empty(), true );
	
	t.testEqual( "map::empty [const]", a.empty(), false );
	
	t.testEqual( "map::empty [normal]", c.empty(), false );
	
	
	// size
	
	t.testEqual( "map::size [empty]", empty.size(), size_t(0) );
	
	t.testEqual( "map::size [const]", a.size(), a_n);
	
	t.testEqual( "map::size [normal]", c.size(), c_n);
	
	
	// max_size
	
	t.testGreaterThan( "map::max_size 1", a.max_size(), size_t(0) );
	
	t.testEqual( "map::max_size 2", a.max_size(), empty.max_size() );
	
	
	// operator[]
	
	{
		dstd::map<int,int> bb(b);
		
		t.testEqual( "map::operator[] 1", bb[2], 2 );
		
		const int x = 99;
		bb[2] = x;
		t.testEqual( "map::operator[] 2", bb[2], x );
		
		t.testEqual( "map::operator[] 3", bb[2*b_n], int() );
		
		t.testEqual( "map::operator[] 4", bb.size(), b.size()+1 );
	}
	
	
	//insert(value)
	
	{
		dstd::map<int, double> m;
		dstd::pair< dstd::map<int, double>::iterator, bool > r1, r2, r3;
		
		const dstd::pair<int, double> v1( 3, 3.14 );
		const dstd::pair<int, double> v2( 3, 6.78 );
		const dstd::pair<int, double> v3( 4, 8.34 );
		
		r1 = m.insert(v1);
		
		t.testEqual("map::insert(value) 1a", r1.first, m.begin());
		t.testEqual("map::insert(value) 1b", r1.second, true);
		t.testEqual("map::insert(value) 1c", *(r1.first), v1);
		
		r2 = m.insert(v2);
		
		t.testEqual("map::insert(value) 2a", r2.first, m.begin());
		t.testEqual("map::insert(value) 2b", r2.first, r1.first);
		t.testEqual("map::insert(value) 2c", r2.second, false);
		t.testEqual("map::insert(value) 2d", *(r2.first), v1);
		
		r3 = m.insert(v3);
		
		t.testNotEqual("map::insert(value) 3a", r3.first, m.begin());
		t.testEqual("map::insert(value) 3b", r3.second, true);
		t.testEqual("map::insert(value) 3c", *(r3.first), v3);
	}
	
	//insert(position, value)
	
	{
		dstd::map<int, double> m;
		dstd::map<int, double>::iterator it1, it2, it3;
		
		const dstd::pair<int, double> v1( 3, 3.14 );
		const dstd::pair<int, double> v2( 3, 6.78 );
		const dstd::pair<int, double> v3( 4, 8.34 );
		
		it1 = m.insert(m.begin(), v1);
		
		t.testEqual("map::insert(position,value) 1a", it1, m.begin());
		t.testEqual("map::insert(position,value) 1b", *it1, v1);
		
		it2 = m.insert(m.begin(), v2);
		
		t.testEqual("map::insert(position,value) 2a", it2, m.begin());
		t.testEqual("map::insert(position,value) 2b", it2, it1);
		t.testEqual("map::insert(position,value) 2c", *(it2), v1);
		
		it3 = m.insert(m.end(), v3);
		
		t.testNotEqual("map::insert(position,value) 3a", it3, m.begin());
		t.testEqual("map::insert(position,value) 3c", *it3, v3);
	}
	
	//insert(first, last)
	
	{
		dstd::map<int,double> m;
		m.insert(empty.begin(), empty.end());
		t.testEqual("map::insert(first,last) [empty]", m, const_empty);
	}
	
	{
		dstd::map<int,int> m;
		m.insert(b.begin(), b.end());
		t.testEqual("map::insert(first,last) 1", m, b);
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::map<int, double> m1, m2, m3, m4, m5;
		const int m4_low(2), m4_high(n-2);
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i % 2 == 0 )
			{
				m1.insert( dstd::pair<int,double>(i, i*x) );
			}
			else
			{
				m2.insert( dstd::pair<int,double>(i, i*x) );
			}
			
			if( i >= m4_low && i <= m4_high ) m4.insert( dstd::pair<int,double>(i, i*x) );
			
			m3.insert( dstd::pair<int,double>(i, i*x) );
		}
		
		m5.insert(m1.begin(), m1.begin());
		t.testEqual("map::interst(first,last 2a", m5, const_empty);
		
		m5.insert(m2.end(), m2.end());
		t.testEqual("map::interst(first,last 2b", m5, const_empty);
		
		m5.insert(m1.begin(), m1.end());
		t.testEqual("map::insert(first,last) 2c", m5, m1);
		
		m5.insert(m2.begin(), m2.end());
		t.testEqual("map::insert(first,last) 2d", m5, m3);
		
		m5.clear();
		m5.insert( m3.lower_bound(m4_low), m3.upper_bound(m4_high) );
		t.testEqual("map::insert(first,last) 2e", m5, m4);
	}
	
	// erase(it)
	
	{
		const unsigned int n = 10;
		dstd::map<int,int> m1, m2;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i%2 != 0 ) m1.insert(dstd::pair<int,int>(i,i));
			m2.insert(dstd::pair<int,int>(i,i));
		}
		for(dstd::map<int,int>::iterator it = m2.begin(); it != m2.end(); ++it)
		{
			m2.erase(it++);
		}
		t.testEqual("map::erase(it)", m1, m2);
	}
	
	// erase(key)
	
	{
		// erase non-existent key from empty map
		dstd::map<int,double> zero;
		zero.erase(4);
		t.testEqual( "map::erase(key) [empty]", zero, empty );
	}
	
	{
		// erase non-existent key from non-empty map
		const unsigned int n = 10;
		dstd::map<int,double> m1, m2;
		for(unsigned int i = 0; i < n; ++i )
		{
			m1.insert( dstd::pair<int, double>( i, 3.14*i ) );
			m2.insert( dstd::pair<int, double>( i, 3.14*i ) );
		}
		m1.erase( 2*n );
		t.testEqual( "map::erase(key) 1", m1, m2 );
	}
	
	{
		// erase single node from a map
		const unsigned int x = 4;
		dstd::map<int,double> m1, m2;
		for(unsigned int i = 0; i < 2*x; ++i)
		{
			m1.insert( dstd::pair<int,double>(i,3.14*i) );
			if( i != x ) m2.insert( dstd::pair<int,double>(i,3.14*i) );
		}
		m1.erase(x);
		t.testEqual( "map::erase(key) 2", m1, m2 );
	}
	
	{
		// erase last node from a map
		const int x = 86;
		dstd::map<int,double> m;
		m.insert( dstd::pair<int,double>( x, 3.14*x ) );
		m.erase( x );
		t.testEqual("map::erase(key) 3", m, empty);
	}
	
	// erase(first,last)
	
	{
		dstd::map<int,double> zero;
		zero.erase(zero.begin(), zero.begin());
		t.testEqual( "map::erase(first,last) [empty]", zero, empty );
	}
	
	{
		dstd::map<int,int> bb( b );
		bb.erase( bb.begin(), bb.begin() );
		t.testEqual( "map::erase(first,last) 1", bb, b );
	}
	
	{
		dstd::map<int,int> m1, m2;
		const unsigned int n = 12;
		for(unsigned int i = 0; i < n; ++i)
		{
			m1.insert(dstd::pair<int,int>(i,2*i));
			if( i == 0 || i == n-1 ) m2.insert( dstd::pair<int,int>(i,2*i) );
		}
		dstd::map<int,int>::iterator first = m1.find(1);
		dstd::map<int,int>::iterator last = m1.find(n-1);
		m1.erase(first, last);
		
		t.testEqual( "map::erase(first,last) 2", m1, m2 );
		
		m1.erase(m1.begin(), m1.end());
		dstd::map<int,int> empty_int;
		t.testEqual( "map::erase(first,last) 3", m1, empty_int);
	}
	
	//swap
	
	{
		dstd::map<int, double> m1, m2;
		m1.swap(m2);
		t.testEqual("map::swap [empty,empty]", m1, empty);
		t.testEqual("map::swap [empty,empty]", m2, empty);
	}
	
	{
		const unsigned int n = 32;
		dstd::map<int, double> a1, a2, b1;
		for(unsigned int i = 0; i < n; ++i)
		{
			a1[i] = i;
		}
		a2 = a1;
		
		// swap empty in
		a1.swap(b1);
		t.testEqual("map::swap [empty] 1a", a1, empty);
		t.testEqual("map::swap [empty] 1b", b1, a2);
		
		// swap into empty
		a1.swap(b1);
		t.testEqual("map::swap [empty] 2a", b1, empty);
		t.testEqual("map::swap [empty] 2b", a1, a2);
	}
	
	{
		const unsigned int n = 23;
		dstd::map<int,int> a1, a2, b1, b2;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i % 2 == 0 )
			{
				a1.insert(dstd::pair<int,int>(i, i));
			}
			else
			{
				b1.insert(dstd::pair<int,int>(i,i));
			}
		}
		a2 = a1;
		b2 = b1;
		
		a1.swap(b1);
		
		t.testEqual("map::swap 1a", a1, b2);
		t.testEqual("map::swap 1a", b1, a2);
	}
	
	//clear
	
	{
		dstd::map<int, double> m;
		m.clear();
		t.testEqual("map::clear [empty]", m, empty);
		t.testEqual("map::clear [empty] size", m.size(), empty_size);
		t.testEqual("map::clear [empty] iter", m.begin(), m.end());
		t.testEqual("map::clear [empty] empty", m.empty(), true);
	}
	
	{
		const unsigned int n = 32;
		dstd::map<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			m[i] = i;
		}
		m.clear();
		
		t.testEqual("map::clear 1", m, empty);
		t.testEqual("map::clear 1 size", m.size(), empty_size);
		t.testEqual("map::clear 1 iter", m.begin(), m.end());
		t.testEqual("map::clear 1 empty", m.empty(), true);
	}
	
	//key_comp
	
	{
		const double a = 1.1;
		const double b = 2.2;
		const double c = 3.3;
		const double cc = c;
		dstd::map<double, char> m;
		dstd::map<double, char>::key_compare comp = m.key_comp();
		
		t.testEqual( "map::key_comp 1", comp(a, a), (a < a) );
		t.testEqual( "map::key_comp 2", comp(a, b), (a < b) );
		t.testEqual( "map::key_comp 3", comp(b, a), (b < a) );
		t.testEqual( "map::key_comp 4", comp(c, cc), (c < cc) );
	}
	
	//value_comp
	
	{
		const double a = 1.1;
		const double b = 2.2;
		const double c = 3.3;
		const double cc = c;
		
		const dstd::pair<double,char> pa( a, 'a' );
		const dstd::pair<double,char> pb( b, 'b' );
		const dstd::pair<double,char> pc( c, 'c' );
		const dstd::pair<double,char> pcc( c, 'd' );
		
		dstd::map<double, char> m;
		dstd::map<double, char>::value_compare vcomp = m.value_comp();
		dstd::map<double, char>::key_compare kcomp = m.key_comp();
		
		t.testEqual( "map::value_comp 1", vcomp(pa, pa), (a < a) );
		t.testEqual( "map::value_comp 2", vcomp(pa, pb), (a < b) );
		t.testEqual( "map::value_comp 3", vcomp(pb, pa), (b < a) );
		t.testEqual( "map::value_comp 4", vcomp(pc, pcc), (c < cc) );
		
		t.testEqual( "map::value_comp 5", vcomp(pa, pa),  kcomp(a, a) );
		t.testEqual( "map::value_comp 6", vcomp(pa, pb),  kcomp(a, b) );
		t.testEqual( "map::value_comp 7", vcomp(pb, pa),  kcomp(b, a) );
		t.testEqual( "map::value_comp 8", vcomp(pc, pcc), kcomp(c, cc) );
	}
	
	//find
	
	{
		dstd::map<char, double> m;
		t.testEqual("map::find [empty]", m.find('d'), m.end());
	}
	
	{
		const double x = 3.14;
		const int n = 10;
		const int k = 2;
		dstd::map<int, double> m;
		for(int i = 0; i < n; ++i)
		{
			m[k*i] = x*i;
		}
		
		const int z = k*(n/2);
		const dstd::map<int, double>::iterator it = m.insert(dstd::pair<int,double>(z, x*z)).first;
		
		t.testEqual("map::find 1", m.find(z), it);
		t.testEqual("map::find 2", m.find(-1), m.end());
	}
	
	//count
	
	{
		dstd::map<int, double> m;
		t.testEqual("map::count [empty]", m.count(3), empty_size);
	}
	
	{
		const unsigned int n = 4;
		dstd::map<unsigned int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			for(unsigned int j = 0; j < i; ++j)
			{
				m.insert(dstd::pair<unsigned int,double>(i, 3.14));
			}
		}
		for(unsigned int i = 0; i < n; ++i)
		{
			t.testEqual("map::count 0-n", m.count(i), size_t( i > 0 ) );
		}
	}
	
	//lower_bound
	
	{
		dstd::map<int, char> m;
		t.testEqual("map::lower_bound [empty]", m.lower_bound(3), m.end());
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::map<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::map<int, double>::iterator it;
		
		it = m.lower_bound(-1);
		t.testEqual( "map::lower_bound 1", it->first, 0 );
		
		it = m.lower_bound(1);
		t.testEqual( "map::lower_bound 2", it->first, 1 );
		
		it = m.lower_bound(n/2);
		t.testEqual( "map::lower_bound 3", it->first, (n/2)+1 );
		
		it = m.lower_bound(2*n);
		t.testEqual( "map::lower_bound 4", it, m.end() );
	}
	
	//upper_bound
	
	{
		dstd::map<int, char> m;
		t.testEqual("map::upper_bound [empty]", m.upper_bound(3), m.end());
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::map<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::map<int, double>::iterator it;
		
		it = m.upper_bound(-1);
		t.testEqual( "map::upper_bound 1", it->first, 0 );
		
		it = m.upper_bound(1);
		t.testEqual( "map::upper_bound 2", it->first, 2 );
		
		it = m.upper_bound(n/2);
		t.testEqual( "map::upper_bound 3", it->first, (n/2)+1 );
		
		it = m.upper_bound(2*n);
		t.testEqual( "map::upper_bound 4", it, m.end() );
	}
	
	//equal_range
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::map<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::pair< dstd::map<int, double>::iterator, dstd::map<int, double>::iterator > its;
		
		its = m.equal_range(-1);
		t.testEqual( "map::equal_range 1a", its.first->first, 0 );
		t.testEqual( "map::equal_range 1b", its.second->first, 0 );
		t.testEqual( "map::equal_range 1c", its.first, its.second );
		
		its = m.equal_range(1);
		t.testEqual( "map::equal_range 2a", its.first->first, 1 );
		t.testEqual( "map::equal_range 2b", its.second->first, 2 );
		
		its = m.equal_range(n/2);
		t.testEqual( "map::equal_range 3a", its.first->first, (n/2)+1 );
		t.testEqual( "map::equal_range 3b", its.second->first, (n/2)+1 );
		t.testEqual( "map::equal_range 3c", its.first, its.second );
		
		its = m.equal_range(2*n);
		t.testEqual( "map::equal_range 4a", its.first, m.end() );
		t.testEqual( "map::equal_range 4b", its.second, m.end() );
		t.testEqual( "map::equal_range 4c", its.first, its.second );
	}
	
	//get_allocator
	// TODO
	
	
	t.report();
	
	
	return 0;
}

