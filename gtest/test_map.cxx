#include <gtest/gtest.h>
#include "../src/map.hxx"


// Google test cannot deal with class and operators being in different namespaces
namespace dstd
{
	template <class Key, class Value> bool operator== (const dstd::map<Key,Value>& v1, const dstd::map<Key,Value>& v2)
	{
		return ::operator==(v1, v2);
	}
	
	template <class Key, class Value> bool operator!= (const dstd::map<Key,Value>& v1, const dstd::map<Key,Value>& v2)
	{
		return ::operator!=(v1, v2);
	}
	
	template <class Key, class T>
	void PrintTo(const map<Key,T>& m, ::std::ostream* stream)
	{
		*stream << "dstd::map[";
		for(typename dstd::map<Key,T>::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			*stream << "(" << it->first << "," << it->second << "), ";
		}
		*stream << "]";
	}
}





class Map : public ::testing::Test
{
	protected:
	
		void SetUp()
		{
			pi = 3.14159;
			
			empty_d.clear();
			empty_i.clear();
			twenty_squares.clear();
			negative_squares.clear();
			
			twenty_squares[6] = 36;		twenty_squares[7] = 49;		twenty_squares[0] = 0;
			twenty_squares[2] = 4;		twenty_squares[9] = 81;		twenty_squares[5] = 25;
			twenty_squares[1] = 1;		twenty_squares[3] = 9;		twenty_squares[4] = 16;
			twenty_squares[8] = 64;		twenty_squares[10] = 100;	twenty_squares[19] = 361;
			twenty_squares[17] = 289;	twenty_squares[15] = 225;	twenty_squares[11] = 121;
			twenty_squares[12] = 144;	twenty_squares[18] = 324;	twenty_squares[13] = 169;
			twenty_squares[14] = 196;	twenty_squares[16] = 256;
			
			negative_squares[-3] = 9;	negative_squares[-5] = 25;	negative_squares[-1] = 1;
			negative_squares[-2] = 4;	negative_squares[-4] = 16;
		}
		
		double pi;
		dstd::map<int, double> empty_d;
		dstd::map<int, int> empty_i;
		dstd::map<int, int> twenty_squares;
		dstd::map<int, int> negative_squares;
};



////
//// Map
//
////(constructor)
//
//{
//	const std::string test_name("map::map()");
//	try
//	{
//		dstd::map<double,int> m();
//		t.registerTestPassed(test_name);
//	}
//	catch(...)
//	{
//		t.registerTestFailed(test_name, "an exception was thrown");
//	}
//}
//
//{
//	const std::string test_name("map::map(c,a)");
//	try
//	{
//		dstd::less<double> c;
//		dstd::allocator<int> a;
//		dstd::map<double,int> m(c, a);
//		t.registerTestPassed(test_name);
//	}
//	catch(...)
//	{
//		t.registerTestFailed(test_name, "an exception was thrown");
//	}
//}
//
//{
//	dstd::map<double, int> m1;
//	const double x = 3.14;
//	for(unsigned int i = 0; i < 10; ++i)
//	{
//		m1[x*i] = i;
//	}
//	
//	const std::string test_name("map::map(first,last) 1");
//	try
//	{
//		dstd::map<double,int> m2(m1.begin(),m1.end());
//		t.registerTestPassed(test_name);
//	}
//	catch(...)
//	{
//		t.registerTestFailed(test_name, "an exception was thrown");
//	}
//	
//	dstd::map<double,int> m2(m1.begin(),m1.end());
//	t.testEqual("map::map(first,last) 2", m1, m2);
//}
//
//{
//	dstd::map<double, int> m1;
//	const double x = 3.14;
//	for(unsigned int i = 0; i < 10; ++i)
//	{
//		m1[x*i] = i;
//	}
//	
//	const std::string test_name("map::map(map) 1");
//	try
//	{
//		dstd::map<double,int> m2(m1);
//		t.registerTestPassed(test_name);
//	}
//	catch(...)
//	{
//		t.registerTestFailed(test_name, "an exception was thrown");
//	}
//	
//	dstd::map<double,int> m2(m1);
//	t.testEqual("map::map(map) 2", m1, m2);
//}
//
////
//// Constructors work, so take time-out to make some reference maps for later tests
//
//dstd::map<int, double> empty;
//const dstd::map<int, double> const_empty;
//const size_t empty_size = 0;
//
//const unsigned int a_n = 20;
//dstd::map<int, double> a_construction;
//for(unsigned int i = 0; i < a_n; ++i)
//{
//	a_construction[rand()] = 3.14 * rand();
//}
//const dstd::map<int,double> a( a_construction );
//
//
//const unsigned int b_n = 20;
//dstd::map<int, int> b_construction;
//for(unsigned int i = 0; i < b_n; ++i)
//{
//	b_construction[i] = i;
//}
//const dstd::map<int,int> b( b_construction );
//
//unsigned int c_n = b_n;
//dstd::map<int,int> c(b);
//
//// Ok, back to testing...
//
//
////(destructor)
//
//{
//	const std::string test_name("map::~map");
//	try
//	{
//		dstd::map<int, double>* m = new dstd::map<int, double>(a.begin(), a.end());
//		delete(m);
//		t.registerTestPassed(test_name);
//	}
//	catch(...)
//	{
//		t.registerTestFailed(test_name, "an exception was thrown");
//	}
//}
//
////operator=
//
//{
//	dstd::map<int,double> empty_copy = empty;
//	t.testEqual("map::operator= [empty]", empty_copy, empty);
//}
//
//{
//	dstd::map<int,double> a_copy = a;
//	t.testEqual("map::operator= [double,const]", a_copy, a);
//}
//
//{
//	dstd::map<int,int> b_copy = b;
//	t.testEqual("map::operator= [int,const]", b_copy, b);
//}
//
//{
//	dstd::map<int,int> c_copy = c;
//	t.testEqual("map::operator= [int,normal]", c_copy, c);
//}
//
//
////begin
//
//t.testEqual( "map::begin [empty]", empty.begin(), empty.end());
//
//t.testEqual( "map::begin [const]", *(b.begin()), dstd::pair<int,int>(0,0) );
//
//t.testEqual( "map::begin [normal]", *(c.begin()), dstd::pair<int,int>(0,0) );
//
//
////end
//
//t.testEqual( "map::end [empty]", empty.end(), empty.begin());
//
//{
//	dstd::map<int,int>::const_iterator it = b.begin();
//	for(unsigned int i = 0; i < b_n; ++i)
//	{
//		++it;
//	}
//	t.testEqual( "map::end [const]", b.end(), it );
//}
//
//{
//	dstd::map<int,int>::iterator it = c.begin();
//	for(unsigned int i = 0; i < c_n; ++i)
//	{
//		++it;
//	}
//	t.testEqual( "map::end [normal]", c.end(), it );
//}
//
//
////rbegin
//
//t.testEqual( "map::rbegin [empty]", empty.rbegin(), empty.rend() );
//
//{
//	dstd::map<int, double>::const_iterator a_fwd = a.end();
//	--a_fwd;
//	dstd::map<int, double>::const_reverse_iterator a_bwd = a.rbegin();
//	
//	t.testEqual("map::rbegin 1" , *a_bwd, *a_fwd);
//}
//
//{
//	dstd::map<int,int>::const_reverse_iterator it = b.rend();
//	for(unsigned int i = 0; i < b_n; ++i)
//	{
//		--it;
//	}
//	t.testEqual( "map::rbegin 3 [const]", b.rbegin(), it );
//}
//
//{
//	dstd::map<int,int>::reverse_iterator it = c.rend();
//	for(unsigned int i = 0; i < c_n; ++i)
//	{
//		--it;
//	}
//	dstd::map<int,int>::reverse_iterator it_begin = c.rbegin();
//	t.testEqual( "map::rbegin 3 [normal]", it_begin, it );
//}
//
//
////rend
//
//t.testEqual( "map::rend [empty]", empty.rend(), empty.rbegin() );
//
//{
//	dstd::map<int, double>::const_reverse_iterator a_rit = a.rend();
//	--a_rit;
//	t.testEqual( "map::rend [const]", *(a_rit), *(a.begin()) );
//}
//
//{
//	dstd::map<int, int>::reverse_iterator c_rit = c.rend();
//	--c_rit;
//	t.testEqual( "map::rend [normal]", *(c_rit), *(c.begin()) );
//}
//
//


// void empty() const


TEST_F(Map, emptyReturnsTrueForEmptyMap)
{
	ASSERT_TRUE( empty_d.empty() );
}


TEST_F(Map, emptyReturnsFalseForNonEmptyMap)
{
	ASSERT_FALSE( twenty_squares.empty() );
}


// size_type size() const


TEST_F(Map, sizeReturnsZeroForEmptyMap)
{
	ASSERT_EQ(0, empty_d.size());
}


TEST_F(Map, sizeReturnsTwentyForTwentySquares)
{
	ASSERT_EQ(20, twenty_squares.size());
}


// size_type max_size() const
//
//
// todo


// mapped_type& operator[] (const key_type& k)


TEST_F(Map, operatorSQBReturnsExistingValueForExistingKey)
{
	const int existing_key = 10;
	const int existing_value = existing_key*existing_key;
	
	ASSERT_EQ( existing_value, twenty_squares[existing_key] );
}


TEST_F(Map, operatorSQBConstructsNewValueForNewKey)
{
	const int new_key = 100;
	const int new_value = 0;
	const size_t size_before = twenty_squares.size();
	
	ASSERT_EQ( new_value, twenty_squares[new_key] );
	ASSERT_EQ( size_before + 1, twenty_squares.size() );
}


TEST_F(Map, operatorSQBSetsValueForExistingKey)
{
	const int existing_key = 10;
	const int new_value = 99;
	
	twenty_squares[existing_key] = new_value;
	
	ASSERT_EQ( new_value, twenty_squares[existing_key] );
}


TEST_F(Map, operatorSQBSetsNewValueForNewKey)
{
	const int new_key = 999;
	const int new_value = 99;
	const size_t size_before = twenty_squares.size();
	
	twenty_squares[new_key] = new_value;
	
	ASSERT_EQ( new_value, twenty_squares[new_key] );
	ASSERT_EQ( size_before + 1, twenty_squares.size() );
}


// dstd::pair< iterator, bool > insert(const value_type& value)


TEST_F(Map, insertValueDoesNotInsertForExistingKey)
{
	const size_t size_before = twenty_squares.size();
	const int existing_key = 8;
	const int existing_mapped = twenty_squares[existing_key];
	dstd::pair<int,int> new_value( existing_key, 99 );
	
	dstd::pair< dstd::map<int,int>::iterator, bool > result = twenty_squares.insert( new_value );
	
	ASSERT_EQ( size_before, twenty_squares.size() );
	ASSERT_FALSE( result.second );
	ASSERT_EQ( existing_key, result.first->first );
	ASSERT_EQ( existing_mapped, result.first->second );
}


TEST_F(Map, insertValueDoesInsertForNewKey)
{
	const size_t size_before = twenty_squares.size();
	const int new_key = 99;
	const int new_mapped = new_key*new_key;
	dstd::pair<int,int> new_value( new_key, new_mapped );
	
	dstd::pair< dstd::map<int,int>::iterator, bool > result = twenty_squares.insert( new_value );
	
	ASSERT_EQ( size_before+1, twenty_squares.size() );
	ASSERT_TRUE( result.second );
	ASSERT_EQ( new_key, result.first->first );
	ASSERT_EQ( new_mapped, result.first->second );
}


// iterator insert(iterator position, const value_type& value)


TEST_F(Map, insertIteratorValueDoesNotInsertForExistingKey)
{
	const size_t size_before = twenty_squares.size();
	const int existing_key = 8;
	const int existing_mapped = twenty_squares[existing_key];
	dstd::pair<int,int> new_value( existing_key, 99 );
	
	const dstd::map<int,int>::iterator result = twenty_squares.insert( twenty_squares.begin(), new_value );
	
	ASSERT_EQ( size_before, twenty_squares.size() );
	ASSERT_EQ( existing_key, result->first );
	ASSERT_EQ( existing_mapped, result->second );
}


TEST_F(Map, insertIteratorValueDoesInsertForNewKey)
{
	const size_t size_before = twenty_squares.size();
	const int new_key = 99;
	const int new_mapped = new_key*new_key;
	dstd::pair<int,int> new_value( new_key, new_mapped );
	
	const dstd::map<int,int>::iterator result = twenty_squares.insert( twenty_squares.begin(), new_value );
	
	ASSERT_EQ( size_before+1, twenty_squares.size() );
	ASSERT_EQ( new_key, result->first );
	ASSERT_EQ( new_mapped, result->second );
}


// template <class InputIterator> void insert(InputIterator first, InputIterator last)


TEST_F(Map, insertIteratorIteratorDoesNotInsertForEmptyRange)
{
	const size_t size_before = twenty_squares.size();
	
	twenty_squares.insert( empty_i.begin(), empty_i.end() );
	
	ASSERT_EQ( size_before, twenty_squares.size() );
}


TEST_F(Map, insertIteratorIteratorInsertsEntireRange)
{
	const size_t size_before = twenty_squares.size();
	
	twenty_squares.insert( negative_squares.begin(), negative_squares.end() );
	
	ASSERT_EQ( size_before + negative_squares.size(), twenty_squares.size() );
	for(dstd::map<int,int>::const_iterator it = negative_squares.begin(); it != negative_squares.end(); ++it)
	{
		ASSERT_EQ( it->second, twenty_squares[ it->first ] );
	}
}


// void erase(iterator position)


TEST_F(Map, eraseIteratorRemovesElement)
{
	const size_t size_before = twenty_squares.size();
	const int existing_key = 13;
	const dstd::map<int,int>::iterator existing_value = twenty_squares.find(existing_key);
	
	twenty_squares.erase( existing_value );
	
	ASSERT_EQ( size_before-1, twenty_squares.size() );
	ASSERT_EQ( 0, twenty_squares.count(existing_key) );
}


// size_type erase(const key_type& k)


TEST_F(Map, eraseKeyRemovesElement)
{
	const size_t size_before = twenty_squares.size();
	const int existing_key = 13;
	
	twenty_squares.erase( existing_key );
	
	ASSERT_EQ( size_before-1, twenty_squares.size() );
	ASSERT_EQ( 0, twenty_squares.count(existing_key) );
}


TEST_F(Map, eraseKeyReturnsOne)
{	
	ASSERT_EQ( 1, twenty_squares.erase(13) );
}


TEST_F(Map, eraseKeyWithNonexistantKeyHasNoEffect)
{
	const dstd::map<int,int> map_before = twenty_squares;
	
	twenty_squares.erase( 99 );
	
	ASSERT_EQ( map_before, twenty_squares );
}


TEST_F(Map, eraseKeyWithNonexistantKeyReturnsZero)
{
	ASSERT_EQ( 0, twenty_squares.erase( 99 ) );
}


// void erase(iterator first, iterator last)


TEST_F(Map, eraseIteratorIteratorWithEmptyRangeHasNoEffect)
{
	const dstd::map<int,int> map_before = twenty_squares;
	
	twenty_squares.erase( twenty_squares.begin(), twenty_squares.begin() );
	
	ASSERT_EQ( map_before, twenty_squares );
}


TEST_F(Map, eraseIteratorIteratorRemovesOnlyElementsInRange)
{
	const size_t size_before = twenty_squares.size();
	const int begin_key = size_before/3;
	const int end_key = 2 * (size_before/3);
	const dstd::map<int,int>::iterator begin_it = twenty_squares.find(begin_key);
	const dstd::map<int,int>::iterator end_it = twenty_squares.find(end_key);
	
	twenty_squares.erase( begin_it, end_it );
	
	ASSERT_EQ( size_before - (end_key - begin_key), twenty_squares.size() );
	for(int key = 0; key < size_before; ++key)
	{
		const int expected = ( key >= begin_key && key < end_key ) ? 0 : 1;
		ASSERT_EQ( expected, twenty_squares.count(key) );
	}
}


TEST_F(Map, eraseIteratorIteratorForBeginEndLeavesMapEmpty)
{
	twenty_squares.erase( twenty_squares.begin(), twenty_squares.end() );
	
	ASSERT_TRUE( twenty_squares.empty() );
}


// void swap(map& x)


TEST_F(Map, swapSucceeds)
{
	const dstd::map<int,int> twenty_squares_before = twenty_squares;
	const dstd::map<int,int> negative_squares_before = negative_squares;
	
	twenty_squares.swap( negative_squares );
	
	ASSERT_EQ( twenty_squares_before, negative_squares );
	ASSERT_EQ( negative_squares_before, twenty_squares );
}


TEST_F(Map, swapToEmptySucceeds)
{
	const dstd::map<int,int> twenty_squares_before = twenty_squares;
	const dstd::map<int,int> empty_i_before = empty_i;
	
	twenty_squares.swap( empty_i );
	
	ASSERT_EQ( twenty_squares_before, empty_i );
	ASSERT_EQ( empty_i_before, twenty_squares );
}


TEST_F(Map, swapByEmptySucceeds)
{
	const dstd::map<int,int> twenty_squares_before = twenty_squares;
	const dstd::map<int,int> empty_i_before = empty_i;
	
	empty_i.swap( twenty_squares );
	
	ASSERT_EQ( twenty_squares_before, empty_i );
	ASSERT_EQ( empty_i_before, twenty_squares );
}


////swap
//
//{
//	dstd::map<int, double> m1, m2;
//	m1.swap(m2);
//	t.testEqual("map::swap [empty,empty]", m1, empty);
//	t.testEqual("map::swap [empty,empty]", m2, empty);
//}
//
//{
//	const unsigned int n = 32;
//	dstd::map<int, double> a1, a2, b1;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		a1[i] = i;
//	}
//	a2 = a1;
//	
//	// swap empty in
//	a1.swap(b1);
//	t.testEqual("map::swap [empty] 1a", a1, empty);
//	t.testEqual("map::swap [empty] 1b", b1, a2);
//	
//	// swap into empty
//	a1.swap(b1);
//	t.testEqual("map::swap [empty] 2a", b1, empty);
//	t.testEqual("map::swap [empty] 2b", a1, a2);
//}
//
//{
//	const unsigned int n = 23;
//	dstd::map<int,int> a1, a2, b1, b2;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		if( i % 2 == 0 )
//		{
//			a1.insert(dstd::pair<int,int>(i, i));
//		}
//		else
//		{
//			b1.insert(dstd::pair<int,int>(i,i));
//		}
//	}
//	a2 = a1;
//	b2 = b1;
//	
//	a1.swap(b1);
//	
//	t.testEqual("map::swap 1a", a1, b2);
//	t.testEqual("map::swap 1a", b1, a2);
//}
//
////clear
//
//{
//	dstd::map<int, double> m;
//	m.clear();
//	t.testEqual("map::clear [empty]", m, empty);
//	t.testEqual("map::clear [empty] size", m.size(), empty_size);
//	t.testEqual("map::clear [empty] iter", m.begin(), m.end());
//	t.testEqual("map::clear [empty] empty", m.empty(), true);
//}
//
//{
//	const unsigned int n = 32;
//	dstd::map<int, double> m;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		m[i] = i;
//	}
//	m.clear();
//	
//	t.testEqual("map::clear 1", m, empty);
//	t.testEqual("map::clear 1 size", m.size(), empty_size);
//	t.testEqual("map::clear 1 iter", m.begin(), m.end());
//	t.testEqual("map::clear 1 empty", m.empty(), true);
//}
//
////key_comp
//
//{
//	const double a = 1.1;
//	const double b = 2.2;
//	const double c = 3.3;
//	const double cc = c;
//	dstd::map<double, char> m;
//	dstd::map<double, char>::key_compare comp = m.key_comp();
//	
//	t.testEqual( "map::key_comp 1", comp(a, a), (a < a) );
//	t.testEqual( "map::key_comp 2", comp(a, b), (a < b) );
//	t.testEqual( "map::key_comp 3", comp(b, a), (b < a) );
//	t.testEqual( "map::key_comp 4", comp(c, cc), (c < cc) );
//}
//
////value_comp
//
//{
//	const double a = 1.1;
//	const double b = 2.2;
//	const double c = 3.3;
//	const double cc = c;
//	
//	const dstd::pair<double,char> pa( a, 'a' );
//	const dstd::pair<double,char> pb( b, 'b' );
//	const dstd::pair<double,char> pc( c, 'c' );
//	const dstd::pair<double,char> pcc( c, 'd' );
//	
//	dstd::map<double, char> m;
//	dstd::map<double, char>::value_compare vcomp = m.value_comp();
//	dstd::map<double, char>::key_compare kcomp = m.key_comp();
//	
//	t.testEqual( "map::value_comp 1", vcomp(pa, pa), (a < a) );
//	t.testEqual( "map::value_comp 2", vcomp(pa, pb), (a < b) );
//	t.testEqual( "map::value_comp 3", vcomp(pb, pa), (b < a) );
//	t.testEqual( "map::value_comp 4", vcomp(pc, pcc), (c < cc) );
//	
//	t.testEqual( "map::value_comp 5", vcomp(pa, pa),  kcomp(a, a) );
//	t.testEqual( "map::value_comp 6", vcomp(pa, pb),  kcomp(a, b) );
//	t.testEqual( "map::value_comp 7", vcomp(pb, pa),  kcomp(b, a) );
//	t.testEqual( "map::value_comp 8", vcomp(pc, pcc), kcomp(c, cc) );
//}
//
////find
//
//{
//	dstd::map<char, double> m;
//	t.testEqual("map::find [empty]", m.find('d'), m.end());
//}
//
//{
//	const double x = 3.14;
//	const int n = 10;
//	const int k = 2;
//	dstd::map<int, double> m;
//	for(int i = 0; i < n; ++i)
//	{
//		m[k*i] = x*i;
//	}
//	
//	const int z = k*(n/2);
//	const dstd::map<int, double>::iterator it = m.insert(dstd::pair<int,double>(z, x*z)).first;
//	
//	t.testEqual("map::find 1", m.find(z), it);
//	t.testEqual("map::find 2", m.find(-1), m.end());
//}
//
////count
//
//{
//	dstd::map<int, double> m;
//	t.testEqual("map::count [empty]", m.count(3), empty_size);
//}
//
//{
//	const unsigned int n = 4;
//	dstd::map<unsigned int, double> m;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		for(unsigned int j = 0; j < i; ++j)
//		{
//			m.insert(dstd::pair<unsigned int,double>(i, 3.14));
//		}
//	}
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		t.testEqual("map::count 0-n", m.count(i), size_t( i > 0 ) );
//	}
//}
//
////lower_bound
//
//{
//	dstd::map<int, char> m;
//	t.testEqual("map::lower_bound [empty]", m.lower_bound(3), m.end());
//}
//
//{
//	const int n = 10;
//	//const double x = 3.14;
//	dstd::map<int, double> m;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
//	}
//	
//	dstd::map<int, double>::iterator it;
//	
//	it = m.lower_bound(-1);
//	t.testEqual( "map::lower_bound 1", it->first, 0 );
//	
//	it = m.lower_bound(1);
//	t.testEqual( "map::lower_bound 2", it->first, 1 );
//	
//	it = m.lower_bound(n/2);
//	t.testEqual( "map::lower_bound 3", it->first, (n/2)+1 );
//	
//	it = m.lower_bound(2*n);
//	t.testEqual( "map::lower_bound 4", it, m.end() );
//}
//
////upper_bound
//
//{
//	dstd::map<int, char> m;
//	t.testEqual("map::upper_bound [empty]", m.upper_bound(3), m.end());
//}
//
//{
//	const int n = 10;
//	//const double x = 3.14;
//	dstd::map<int, double> m;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
//	}
//	
//	dstd::map<int, double>::iterator it;
//	
//	it = m.upper_bound(-1);
//	t.testEqual( "map::upper_bound 1", it->first, 0 );
//	
//	it = m.upper_bound(1);
//	t.testEqual( "map::upper_bound 2", it->first, 2 );
//	
//	it = m.upper_bound(n/2);
//	t.testEqual( "map::upper_bound 3", it->first, (n/2)+1 );
//	
//	it = m.upper_bound(2*n);
//	t.testEqual( "map::upper_bound 4", it, m.end() );
//}
//
////equal_range
//
//{
//	const int n = 10;
//	//const double x = 3.14;
//	dstd::map<int, double> m;
//	for(unsigned int i = 0; i < n; ++i)
//	{
//		if( i != n/2 ) m.insert( dstd::pair<int,double>( i, i*n ) );
//	}
//	
//	dstd::pair< dstd::map<int, double>::iterator, dstd::map<int, double>::iterator > its;
//	
//	its = m.equal_range(-1);
//	t.testEqual( "map::equal_range 1a", its.first->first, 0 );
//	t.testEqual( "map::equal_range 1b", its.second->first, 0 );
//	t.testEqual( "map::equal_range 1c", its.first, its.second );
//	
//	its = m.equal_range(1);
//	t.testEqual( "map::equal_range 2a", its.first->first, 1 );
//	t.testEqual( "map::equal_range 2b", its.second->first, 2 );
//	
//	its = m.equal_range(n/2);
//	t.testEqual( "map::equal_range 3a", its.first->first, (n/2)+1 );
//	t.testEqual( "map::equal_range 3b", its.second->first, (n/2)+1 );
//	t.testEqual( "map::equal_range 3c", its.first, its.second );
//	
//	its = m.equal_range(2*n);
//	t.testEqual( "map::equal_range 4a", its.first, m.end() );
//	t.testEqual( "map::equal_range 4b", its.second, m.end() );
//	t.testEqual( "map::equal_range 4c", its.first, its.second );
//}
//
////get_allocator
//// TODO

	
//	t.report();
//	
//	
//	return 0;
//}

