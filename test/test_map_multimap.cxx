#include <cstdlib>
#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/map.hxx"


template <class Key, class T>
std::ostream& operator<<(std::ostream& stream, const dstd::multimap<Key,T>& m)
{
	stream << "dstd::multimap[";
	for(typename dstd::multimap<Key,T>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		stream << "(" << it->first << "," << it->second << "), ";
	}
	stream << "]";
	return stream;
}



int main()
{
	dstd::Tester t("<multimap> multimap");
	
	
	//
	// multimap
	
	//(constructor)
	
	{
		const std::string test_name("multimap::multimap()");
		try
		{
			dstd::multimap<double,int> m();
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	{
		const std::string test_name("multimap::multimap(c,a)");
		try
		{
			dstd::less<double> c;
			dstd::allocator<int> a;
			dstd::multimap<double,int> m(c, a);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	{
		dstd::multimap<double, int> m1;
		const double x = 3.14;
		for(unsigned int i = 0; i < 10; ++i)
		{
			m1.insert( dstd::pair<double, int>(x*i, i) );
		}
		
		const std::string test_name("multimap::multimap(first,last) 1");
		try
		{
			dstd::multimap<double,int> m2(m1.begin(),m1.end());
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
		
		dstd::multimap<double,int> m2(m1.begin(),m1.end());
		t.testEqual("multimap::multimap(first,last) 2", m1, m2);
	}
	
	{
		dstd::multimap<double, int> m1;
		const double x = 3.14;
		for(unsigned int i = 0; i < 10; ++i)
		{
			m1.insert( dstd::pair<double, int>(x*i, i) );
		}
		
		const std::string test_name("multimap::multimap(multimap) 1");
		try
		{
			dstd::multimap<double,int> m2(m1);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
		
		dstd::multimap<double,int> m2(m1);
		t.testEqual("multimap::multimap(multimap) 2", m1, m2);
	}
	
	//
	// Constructors work, so take time-out to make some reference multimaps for later tests
	
	dstd::multimap<int, double> empty;
	const dstd::multimap<int, double> const_empty;
	const size_t empty_size = 0;
	
	const unsigned int a_n = 20;
	dstd::multimap<int, double> a_construction;
	for(unsigned int i = 0; i < a_n; ++i)
	{
		a_construction.insert( dstd::pair<int, double>(rand(), 3.14 * rand()) );
	}
	const dstd::multimap<int,double> a( a_construction );
	
	
	const unsigned int b_n = 20;
	dstd::multimap<int, int> b_construction;
	for(unsigned int i = 0; i < b_n; ++i)
	{
		b_construction.insert( dstd::pair<int, int>(i, i) );
		
	}
	const dstd::multimap<int,int> b( b_construction );
	
	unsigned int c_n = b_n;
	dstd::multimap<int,int> c(b);
	
	// Ok, back to testing...
	
	
	//(destructor)
	
	{
		const std::string test_name("multimap::~multimap");
		try
		{
			dstd::multimap<int, double>* m = new dstd::multimap<int, double>(a.begin(), a.end());
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
		dstd::multimap<int,double> empty_copy = empty;
		t.testEqual("multimap::operator= [empty]", empty_copy, empty);
	}
	
	{
		dstd::multimap<int,double> a_copy = a;
		t.testEqual("multimap::operator= [double,const]", a_copy, a);
	}
	
	{
		dstd::multimap<int,int> b_copy = b;
		t.testEqual("multimap::operator= [int,const]", b_copy, b);
	}
	
	{
		dstd::multimap<int,int> c_copy = c;
		t.testEqual("multimap::operator= [int,normal]", c_copy, c);
	}
	
	
	//begin
	
	t.testEqual( "multimap::begin [empty]", empty.begin(), empty.end());
	
	t.testEqual( "multimap::begin [const]", *(b.begin()), dstd::pair<int,int>(0,0) );
	
	t.testEqual( "multimap::begin [normal]", *(c.begin()), dstd::pair<int,int>(0,0) );
	
	
	//end
	
	t.testEqual( "multimap::end [empty]", empty.end(), empty.begin());
	
	{
		dstd::multimap<int,int>::const_iterator it = b.begin();
		for(unsigned int i = 0; i < b_n; ++i)
		{
			++it;
		}
		t.testEqual( "multimap::end [const]", b.end(), it );
	}
	
	{
		dstd::multimap<int,int>::iterator it = c.begin();
		for(unsigned int i = 0; i < c_n; ++i)
		{
			++it;
		}
		t.testEqual( "multimap::end [normal]", c.end(), it );
	}
	
	
	//rbegin
	
	t.testEqual( "multimap::rbegin [empty]", empty.rbegin(), empty.rend() );
	
	{
		dstd::multimap<int, double>::const_iterator a_fwd = a.end();
		--a_fwd;
		dstd::multimap<int, double>::const_reverse_iterator a_bwd = a.rbegin();
		
		t.testEqual("multimap::rbegin 1" , *a_bwd, *a_fwd);
	}
	
	{
		dstd::multimap<int,int>::const_reverse_iterator it = b.rend();
		for(unsigned int i = 0; i < b_n; ++i)
		{
			--it;
		}
		t.testEqual( "multimap::rbegin 3 [const]", b.rbegin(), it );
	}
	
	{
		dstd::multimap<int,int>::reverse_iterator it = c.rend();
		for(unsigned int i = 0; i < c_n; ++i)
		{
			--it;
		}
		dstd::multimap<int,int>::reverse_iterator it_begin = c.rbegin();
		t.testEqual( "multimap::rbegin 3 [normal]", it_begin, it );
	}
	
	
	//rend
	
	t.testEqual( "multimap::rend [empty]", empty.rend(), empty.rbegin() );
	
	{
		dstd::multimap<int, double>::const_reverse_iterator a_rit = a.rend();
		--a_rit;
		t.testEqual( "multimap::rend [const]", *(a_rit), *(a.begin()) );
	}
	
	{
		dstd::multimap<int, int>::reverse_iterator c_rit = c.rend();
		--c_rit;
		t.testEqual( "multimap::rend [normal]", *(c_rit), *(c.begin()) );
	}
	
	
	// empty
	
	t.testEqual( "multimap::empty [empty]", empty.empty(), true );
	
	t.testEqual( "multimap::empty [const]", a.empty(), false );
	
	t.testEqual( "multimap::empty [normal]", c.empty(), false );
	
	
	// size
	
	t.testEqual( "multimap::size [empty]", empty.size(), size_t(0) );
	
	t.testEqual( "multimap::size [const]", a.size(), a_n);
	
	t.testEqual( "multimap::size [normal]", c.size(), c_n);
	
	
	// max_size
	
	t.testGreaterThan( "multimap::max_size 1", a.max_size(), size_t(0) );
	
	t.testEqual( "multimap::max_size 2", a.max_size(), empty.max_size() );
	
	//insert(value)
	
	{
		dstd::multimap<int, double> m;
		dstd::multimap<int, double>::iterator it1, it2, it3;
		
		const dstd::pair<int, double> v1( 3, 3.14 );
		const dstd::pair<int, double> v2( 3, 6.78 );
		const dstd::pair<int, double> v3( 4, 8.34 );
		
		it1 = m.insert(v1);
		
		t.testEqual("multimap::insert(position,value) 1a", it1, m.begin());
		t.testEqual("multimap::insert(position,value) 1b", *it1, v1);
		
		it2 = m.insert(v2);
		
		t.testNotEqual("multimap::insert(position,value) 2a", it2, it1);
		t.testEqual("multimap::insert(position,value) 2b", *(it2), v2);
		t.testEqual("multimap::insert(position,value) 2c", m.size(), size_t(2));
		
		it3 = m.insert(v3);
		
		t.testNotEqual("multimap::insert(position,value) 3a", it3, m.begin());
		t.testEqual("multimap::insert(position,value) 3c", *it3, v3);
	}
	
	//insert(position, value)
	
	{
		dstd::multimap<int, double> m;
		dstd::multimap<int, double>::iterator it1, it2, it3;
		
		const dstd::pair<int, double> v1( 3, 3.14 );
		const dstd::pair<int, double> v2( 3, 6.78 );
		const dstd::pair<int, double> v3( 4, 8.34 );
		
		it1 = m.insert(m.begin(), v1);
		
		t.testEqual("multimap::insert(position,value) 1a", it1, m.begin());
		t.testEqual("multimap::insert(position,value) 1b", *it1, v1);
		const size_t s1 = m.size();
		
		it2 = m.insert(m.begin(), v2);
		
		t.testEqual("multimap::insert(position,value) 2a", it2, m.begin());
		t.testNotEqual("multimap::insert(position,value) 2b", it2, it1);
		t.testEqual("multimap::insert(position,value) 2c", *(it2), v2);
		t.testEqual("multimap::insert(position,value) 2d", m.size(), s1+1);
		
		it3 = m.insert(m.end(), v3);
		
		t.testNotEqual("multimap::insert(position,value) 3a", it3, m.begin());
		t.testEqual("multimap::insert(position,value) 3c", *it3, v3);
	}
	
	//insert(first, last)
	
	{
		dstd::multimap<int,double> m;
		m.insert(empty.begin(), empty.end());
		t.testEqual("multimap::insert(first,last) [empty]", m, const_empty);
	}
	
	{
		dstd::multimap<int,int> m;
		m.insert(b.begin(), b.end());
		t.testEqual("multimap::insert(first,last) 1", m, b);
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::multimap<int, double> m1, m2, m3, m4, m5;
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
		t.testEqual("multimap::interst(first,last) 2a", m5, const_empty);
		
		m5.insert(m2.end(), m2.end());
		t.testEqual("multimap::interst(first,last) 2b", m5, const_empty);
		
		m5.insert(m1.begin(), m1.end());
		t.testEqual("multimap::insert(first,last) 2c", m5, m1);
		
		m5.insert(m2.begin(), m2.end());
		t.testEqual("multimap::insert(first,last) 2d", m5, m3);
		
		m5.clear();
		m5.insert( m3.lower_bound(m4_low), m3.upper_bound(m4_high) );
		t.testEqual("multimap::insert(first,last) 2e", m5, m4);
	}
	
	// erase(it)
	
	{
		const unsigned int n = 10;
		dstd::multimap<int,int> m1, m2;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i%2 != 0 ) m1.insert(dstd::pair<int,int>(i,i));
			m2.insert(dstd::pair<int,int>(i,i));
		}
		for(dstd::multimap<int,int>::iterator it = m2.begin(); it != m2.end(); ++it)
		{
			m2.erase(it++);
		}
		t.testEqual("multimap::erase(it)", m1, m2);
	}
	
	// erase(key)
	
	{
		// erase non-existent key from empty multimap
		dstd::multimap<int,double> zero;
		zero.erase(4);
		t.testEqual( "multimap::erase(key) [empty]", zero, empty );
	}
	
	{
		// erase non-existent key from non-empty multimap
		const unsigned int n = 10;
		dstd::multimap<int,double> m1, m2;
		for(unsigned int i = 0; i < n; ++i )
		{
			m1.insert( dstd::pair<int, double>( i, 3.14*i ) );
			m2.insert( dstd::pair<int, double>( i, 3.14*i ) );
		}
		m1.erase( 2*n );
		t.testEqual( "multimap::erase(key) 1", m1, m2 );
	}
	
	{
		// erase single node from a multimap
		const unsigned int x = 4;
		dstd::multimap<int,double> m1, m2;
		for(unsigned int i = 0; i < 2*x; ++i)
		{
			m1.insert( dstd::pair<int,double>(i,3.14*i) );
			if( i != x ) m2.insert( dstd::pair<int,double>(i,3.14*i) );
		}
		m1.erase(x);
		t.testEqual( "multimap::erase(key) 2", m1, m2 );
	}
	
	{
		// erase last node from a multimap
		const int x = 86;
		dstd::multimap<int,double> m;
		m.insert( dstd::pair<int,double>( x, 3.14*x ) );
		m.erase( x );
		t.testEqual("multimap::erase(key) 3", m, empty);
	}
	
	// erase(first,last)
	
	{
		dstd::multimap<int,double> zero;
		zero.erase(zero.begin(), zero.begin());
		t.testEqual( "multimap::erase(first,last) [empty]", zero, empty );
	}
	
	{
		dstd::multimap<int,int> bb( b );
		bb.erase( bb.begin(), bb.begin() );
		t.testEqual( "multimap::erase(first,last) 1", bb, b );
	}
	
	{
		dstd::multimap<int,int> m1, m2;
		const unsigned int n = 12;
		for(unsigned int i = 0; i < n; ++i)
		{
			m1.insert(dstd::pair<int,int>(i,2*i));
			if( i == 0 || i == n-1 ) m2.insert( dstd::pair<int,int>(i,2*i) );
		}
		dstd::multimap<int,int>::iterator first = m1.find(1);
		dstd::multimap<int,int>::iterator last = m1.find(n-1);
		m1.erase(first, last);
		
		t.testEqual( "multimap::erase(first,last) 2", m1, m2 );
		
		m1.erase(m1.begin(), m1.end());
		dstd::multimap<int,int> empty_int;
		t.testEqual( "multimap::erase(first,last) 3", m1, empty_int);
	}
	
	//swap
	
	{
		dstd::multimap<int, double> m1, m2;
		m1.swap(m2);
		t.testEqual("multimap::swap [empty,empty]", m1, empty);
		t.testEqual("multimap::swap [empty,empty]", m2, empty);
	}
	
	{
		const unsigned int n = 32;
		dstd::multimap<int, double> a1, a2, b1;
		for(unsigned int i = 0; i < n; ++i)
		{
			a1.insert(dstd::pair<int,double>(i,i));
		}
		a2 = a1;
		
		// swap empty in
		a1.swap(b1);
		t.testEqual("multimap::swap [empty] 1a", a1, empty);
		t.testEqual("multimap::swap [empty] 1b", b1, a2);
		
		// swap into empty
		a1.swap(b1);
		t.testEqual("multimap::swap [empty] 2a", b1, empty);
		t.testEqual("multimap::swap [empty] 2b", a1, a2);
	}
	
	{
		const unsigned int n = 23;
		dstd::multimap<int,int> a1, a2, b1, b2;
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
		
		t.testEqual("multimap::swap 1a", a1, b2);
		t.testEqual("multimap::swap 1a", b1, a2);
	}
	
	//clear
	
	{
		dstd::multimap<int, double> m;
		m.clear();
		t.testEqual("multimap::clear [empty]", m, empty);
		t.testEqual("multimap::clear [empty] size", m.size(), empty_size);
		t.testEqual("multimap::clear [empty] iter", m.begin(), m.end());
		t.testEqual("multimap::clear [empty] empty", m.empty(), true);
	}
	
	{
		const unsigned int n = 32;
		dstd::multimap<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			m.insert(dstd::pair<int,double>(i,i));
		}
		m.clear();
		
		t.testEqual("multimap::clear 1", m, empty);
		t.testEqual("multimap::clear 1 size", m.size(), empty_size);
		t.testEqual("multimap::clear 1 iter", m.begin(), m.end());
		t.testEqual("multimap::clear 1 empty", m.empty(), true);
	}
	
	//key_comp
	
	{
		const double a = 1.1;
		const double b = 2.2;
		const double c = 3.3;
		const double cc = c;
		dstd::multimap<double, char> m;
		dstd::multimap<double, char>::key_compare comp = m.key_comp();
		
		t.testEqual( "multimap::key_comp 1", comp(a, a), (a < a) );
		t.testEqual( "multimap::key_comp 2", comp(a, b), (a < b) );
		t.testEqual( "multimap::key_comp 3", comp(b, a), (b < a) );
		t.testEqual( "multimap::key_comp 4", comp(c, cc), (c < cc) );
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
		
		dstd::multimap<double, char> m;
		dstd::multimap<double, char>::value_compare vcomp = m.value_comp();
		dstd::multimap<double, char>::key_compare kcomp = m.key_comp();
		
		t.testEqual( "multimap::value_comp 1", vcomp(pa, pa), (a < a) );
		t.testEqual( "multimap::value_comp 2", vcomp(pa, pb), (a < b) );
		t.testEqual( "multimap::value_comp 3", vcomp(pb, pa), (b < a) );
		t.testEqual( "multimap::value_comp 4", vcomp(pc, pcc), (c < cc) );
		
		t.testEqual( "multimap::value_comp 5", vcomp(pa, pa),  kcomp(a, a) );
		t.testEqual( "multimap::value_comp 6", vcomp(pa, pb),  kcomp(a, b) );
		t.testEqual( "multimap::value_comp 7", vcomp(pb, pa),  kcomp(b, a) );
		t.testEqual( "multimap::value_comp 8", vcomp(pc, pcc), kcomp(c, cc) );
	}
	
	//find
	
	{
		dstd::multimap<char, double> m;
		t.testEqual("multimap::find [empty]", m.find('d'), m.end());
	}
	
	{
		const double x = 3.14;
		const int n = 10;
		const int k = 2;
		dstd::multimap<int, double> m;
		for(int i = 0; i < n; ++i)
		{
			m.insert(dstd::pair<int,double>(k*i,x*i));
		}
		
		const int z1 = (k+1)*n;
		const int z2 = k*(n/2);
		const dstd::multimap<int, double>::iterator it1 = m.insert(dstd::pair<int,double>(z1, x*z1));
		const dstd::multimap<int, double>::iterator it2 = m.insert(dstd::pair<int,double>(z2, 3.14));
		
		t.testEqual("multimap::find 1a", m.find(z1), it1); // single existing element
		t.testEqual("multimap::find 1b", m.find(z1)->first, z1);
		t.testEqual("multimap::find 2b", m.find(z2)->first, z2); // two existing elements
		t.testEqual("multimap::find 1", m.find(-1), m.end()); // no existing element
	}
	
	//count
	
	{
		dstd::multimap<int, double> m;
		t.testEqual("multimap::count [empty]", m.count(3), empty_size);
	}
	
	{
		const unsigned int n = 4;
		dstd::multimap<unsigned int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			for(unsigned int j = 0; j < i; ++j)
			{
				m.insert(dstd::pair<unsigned int,double>(i, 3.14));
			}
		}
		for(unsigned int i = 0; i < n; ++i)
		{
			t.testEqual("multimap::count 0-n", m.count(i), size_t( i > 0 ) );
		}
	}
	
	//lower_bound
	
	{
		dstd::multimap<int, char> m;
		t.testEqual("multimap::lower_bound [empty]", m.lower_bound(3), m.end());
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::multimap<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::multimap<int, double>::iterator it;
		
		it = m.lower_bound(-1);
		t.testEqual( "multimap::lower_bound 1", it->first, 0 );
		
		it = m.lower_bound(1);
		t.testEqual( "multimap::lower_bound 2", it->first, 1 );
		
		it = m.lower_bound(n/2);
		t.testEqual( "multimap::lower_bound 3", it->first, (n/2)+1 );
		
		it = m.lower_bound(2*n);
		t.testEqual( "multimap::lower_bound 4", it, m.end() );
	}
	
	//upper_bound
	
	{
		dstd::multimap<int, char> m;
		t.testEqual("multimap::upper_bound [empty]", m.upper_bound(3), m.end());
	}
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::multimap<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::multimap<int, double>::iterator it;
		
		it = m.upper_bound(-1);
		t.testEqual( "multimap::upper_bound 1", it->first, 0 );
		
		it = m.upper_bound(1);
		t.testEqual( "multimap::upper_bound 2", it->first, 2 );
		
		it = m.upper_bound(n/2);
		t.testEqual( "multimap::upper_bound 3", it->first, (n/2)+1 );
		
		it = m.upper_bound(2*n);
		t.testEqual( "multimap::upper_bound 4", it, m.end() );
	}
	
	//equal_range
	
	{
		const int n = 10;
		const double x = 3.14;
		dstd::multimap<int, double> m;
		for(unsigned int i = 0; i < n; ++i)
		{
			if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
		}
		
		dstd::pair< dstd::multimap<int, double>::iterator, dstd::multimap<int, double>::iterator > its;
		
		its = m.equal_range(-1);
		t.testEqual( "multimap::equal_range 1a", its.first->first, 0 );
		t.testEqual( "multimap::equal_range 1b", its.second->first, 0 );
		t.testEqual( "multimap::equal_range 1c", its.first, its.second );
		
		its = m.equal_range(1);
		t.testEqual( "multimap::equal_range 2a", its.first->first, 1 );
		t.testEqual( "multimap::equal_range 2b", its.second->first, 2 );
		
		its = m.equal_range(n/2);
		t.testEqual( "multimap::equal_range 3a", its.first->first, (n/2)+1 );
		t.testEqual( "multimap::equal_range 3b", its.second->first, (n/2)+1 );
		t.testEqual( "multimap::equal_range 3c", its.first, its.second );
		
		its = m.equal_range(2*n);
		t.testEqual( "multimap::equal_range 4a", its.first, m.end() );
		t.testEqual( "multimap::equal_range 4b", its.second, m.end() );
		t.testEqual( "multimap::equal_range 4c", its.first, its.second );
	}
	
	//get_allocator
	// TODO
	
	
	t.report();
	
	
	return 0;
}

