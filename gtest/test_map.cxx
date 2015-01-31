#include <gtest/gtest.h>
#include "../src/map.hxx"


// Google test cannot deal with class and operators being in different namespaces
namespace dstd
{
	template <class Key, class T> bool operator== (const dstd::map<Key,T>& m1, const dstd::map<Key,T>& m2)
	{
		return ::operator==(m1, m2);
	}
	
	template <class Key, class T> bool operator!= (const dstd::map<Key,T>& m1, const dstd::map<Key,T>& m2)
	{
		return ::operator!=(m1, m2);
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


template <class Key, class T>
bool equals(const dstd::pair<Key, T>& p1, const dstd::pair<Key, T>& p2)
{
	return (p1 == p2);
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


// explicit map(const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())


TEST_F(Map, constructorDefaultCreatesEmptyMap)
{
	dstd::map<int, double> test_map;
	ASSERT_EQ( 0, test_map.size() );
}


// template <class InputIterator>
// map(InputIterator first, InputIterator last, const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())


TEST_F(Map, constructorIteatorIteratorCopiesAllButLastElement)
{
	const int first_key = 10;
	const int last_key = 13;
	dstd::map<int,int>::iterator first = twenty_squares.find(first_key);
	dstd::map<int,int>::iterator last = twenty_squares.find(last_key);
	dstd::map<int,int> test_map(first, last);
	
	ASSERT_EQ( last_key - first_key, test_map.size() );
	for(int test_key = first_key; test_key != last_key; ++test_key)
	{
		ASSERT_EQ( 1, test_map.count(test_key) );
	}
}


// map (const map& x)


TEST_F(Map, constructorCopyCreatesIdenticalMap)
{
	dstd::map<int,int> test_map( twenty_squares );
	
	ASSERT_EQ( twenty_squares, test_map );
}


// map& operator= (const map& x)


TEST_F(Map, assignmentOperatorCopiesValue)
{
	negative_squares = twenty_squares;
	ASSERT_EQ( twenty_squares, negative_squares );
}


// iterator begin()
// const_iterator begin() const


TEST_F(Map, beginReturnsIteratorToFirstNode)
{
	ASSERT_EQ( 0, twenty_squares.begin()->first );
}


// todo
//
//TEST_F(Map, beginReturnsEndForEmptyMap)
//{
//	ASSERT_EQ( empty_d.end(), empty_d.begin() );
//}


// iterator end()
// const_iterator end() const


TEST_F(Map, endReturnsIteratorBeyondTheEnd)
{
	dstd::map<int,int>::iterator end = twenty_squares.end();
	--end;
	
	ASSERT_EQ( 19, end->first );
}


// reverse_iterator rbegin()
// const_reverse_iterator rbegin() const


TEST_F(Map, rbeginReturnsReverseIteratorToLastNode)
{
	ASSERT_EQ( 19, twenty_squares.rbegin()->first );
}


// todo
//
//TEST_F(Map, rbeginReturnsRendForEmptyMap)
//{
//	ASSERT_TRUE( equals(empty_d.rend(), empty_d.rbegin()) );
//}


// reverse_iterator rend()
// const_reverse_iterator rend() const


TEST_F(Map, rendReturnsReverseIteratorBeyondTheEnd)
{
	dstd::map<int,int>::reverse_iterator rend = twenty_squares.rend();
	--rend;
	
	ASSERT_EQ( 0, rend->first );
}


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


TEST_F(Map, maxSizeReturnsLargePositiveInteger)
{
	ASSERT_GT(twenty_squares.max_size(), 9999);
}


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


// void swap(map& x)


TEST_F(Map, swapExchangesMapContents)
{
	const dstd::map<int,int> original_twenty_squares = twenty_squares;
	const dstd::map<int,int> original_negative_squares = negative_squares;
	
	twenty_squares.swap(negative_squares);
	
	ASSERT_EQ( original_twenty_squares, negative_squares );
	ASSERT_EQ( original_negative_squares, twenty_squares );
}


// void clear()


TEST_F(Map, clearLeavesMapEmpty)
{
	twenty_squares.clear();
	
	ASSERT_TRUE(twenty_squares.empty());
}


TEST_F(Map, clearHasNoEffectOnEmptyMap)
{
	empty_d.clear();
	
	ASSERT_TRUE(empty_d.empty());
}


// key_compare key_comp() const


TEST_F(Map, keyCompareReturnsFalseForEqualKeys)
{
	dstd::map<int,int>::key_compare comp = twenty_squares.key_comp();
	ASSERT_FALSE( comp(1, 1) );
}


TEST_F(Map, keyCompareReturnsTrueForFirstSmallerThanSecond)
{
	dstd::map<int,int>::key_compare comp = twenty_squares.key_comp();
	ASSERT_TRUE( comp(1, 2) );
}


TEST_F(Map, keyCompareReturnsFalseForFirstLargerThanSecond)
{
	dstd::map<int,int>::key_compare comp = twenty_squares.key_comp();
	ASSERT_FALSE( comp(2, 1) );
}


// value_compare value_comp() const


TEST_F(Map, valueCompareReturnsFalseForEqualKeys)
{
	dstd::map<int,int>::value_compare comp = twenty_squares.value_comp();
	dstd::map<int,int>::value_type v1( 10, 99 );
	dstd::map<int,int>::value_type v2( 10, 88 );
	
	ASSERT_FALSE( comp(v1, v2) );
}


TEST_F(Map, valueCompareReturnsTrueForFirstKeySmallerThanSecondKey)
{
	dstd::map<int,int>::value_compare comp = twenty_squares.value_comp();
	dstd::map<int,int>::value_type v1( 10, 99 );
	dstd::map<int,int>::value_type v2( 11, 88 );
	
	ASSERT_TRUE( comp(v1, v2) );
}


TEST_F(Map, valueCompareReturnsFalseForFirstKeyLargerThanSecondKey)
{
	dstd::map<int,int>::value_compare comp = twenty_squares.value_comp();
	dstd::map<int,int>::value_type v1( 11, 99 );
	dstd::map<int,int>::value_type v2( 10, 88 );
	
	ASSERT_FALSE( comp(v1, v2) );
}


// iterator find(const key_type& k)
// const_iterator find(const key_type& k) const


TEST_F(Map, findReturnsEndForNonExistantKey)
{
	ASSERT_EQ( twenty_squares.end(), twenty_squares.find(-1) );
}


TEST_F(Map, findReturnsIteratorToCorrectElement)
{
	const int i = 12;
	const dstd::map<int,int>::value_type expected( i, i*i );
	
	const dstd::map<int,int>::iterator result = twenty_squares.find(i);
	
	//ASSERT_EQ( expected , *result );
	ASSERT_TRUE( equals(expected, *result) );
}


// size_type count(const key_type& k) const


TEST_F(Map, countReturnsOneForExistingKey)
{
	ASSERT_EQ( 1, twenty_squares.count(10) );
}


TEST_F(Map, countReturnsOneForNonexistingKey)
{
	ASSERT_EQ( 0, twenty_squares.count(-10) );
}


// iterator lower_bound(const key_type& key)
// const_iterator lower_bound(const key_type& key) const


TEST_F(Map, lowerBoundReturnsIteratorToValueForExistingKey)
{
	const int existing_key = 10;
	dstd::map<int,int>::const_iterator expected = twenty_squares.find(existing_key);
	
	ASSERT_EQ( existing_key, twenty_squares.lower_bound(existing_key)->first );
}


TEST_F(Map, lowerBoundReturnsIteratorToFirstKeyNotLessThanNonexistingKey)
{
	dstd::map<int,int> test_map;
	test_map.insert( dstd::pair<int,int>(10,10) );
	test_map.insert( dstd::pair<int,int>(20,20) );
	test_map.insert( dstd::pair<int,int>(30,30) );
	
	ASSERT_EQ( 20, twenty_squares.lower_bound(15)->first );
}


TEST_F(Map, lowerBoundReturnsIteratorToBeginForNonexistingKeyBeforeAllOtherKeys)
{
	ASSERT_EQ( twenty_squares.begin()->first, twenty_squares.lower_bound(-10)->first );
}


TEST_F(Map, lowerBoundReturnsIteratorToEndForNonexistingKeyAfterAllOtherKeys)
{
	ASSERT_EQ( twenty_squares.end(), twenty_squares.lower_bound(999) );
}


// iterator upper_bound(const key_type& key)
// const_iterator upper_bound(const key_type& key) const


TEST_F(Map, upperBoundReturnsIteratorToFirstKeyAfterExistingKey)
{
	ASSERT_EQ( 11, twenty_squares.upper_bound(10)->first );
}


TEST_F(Map, upperBoundReturnsIteratorToFirstKeyGreaterThanNonexistingKey)
{
	dstd::map<int,int> test_map;
	test_map.insert( dstd::pair<int,int>(10,10) );
	test_map.insert( dstd::pair<int,int>(20,20) );
	test_map.insert( dstd::pair<int,int>(30,30) );
	
	ASSERT_EQ( 20, twenty_squares.lower_bound(15)->first );
}


TEST_F(Map, upperBoundReturnsIteratorToBeginForNonexistingKeyBeforeAllOtherKeys)
{
	ASSERT_EQ( twenty_squares.begin()->first, twenty_squares.lower_bound(-10)->first );
}


TEST_F(Map, upperBoundReturnsIteratorToEndForNonexistingKeyAfterAllOtherKeys)
{
	ASSERT_EQ( twenty_squares.end(), twenty_squares.lower_bound(999) );
}


// dstd::pair<iterator,iterator> equal_range(const key_type& key)
// dstd::pair<const_iterator,const_iterator> equal_range(const key_type& key) const


TEST_F(Map, equalRangeReturnsExpectedRangeForExistingKey)
{
	typedef dstd::pair<dstd::map<int,int>::iterator,dstd::map<int,int>::iterator> Range;
	const int existing_key = 6;
	
	Range result = twenty_squares.equal_range(existing_key);
	
	ASSERT_EQ( existing_key, result.first->first );
	ASSERT_EQ( existing_key+1, result.second->first );
}


TEST_F(Map, equalRangeReturnsExpectedRangeForNonexistingKey)
{
	typedef dstd::pair<dstd::map<int,int>::iterator,dstd::map<int,int>::iterator> Range;
	dstd::map<int,int> test_map;
	test_map.insert( dstd::pair<int,int>(10,10) );
	test_map.insert( dstd::pair<int,int>(20,20) );
	test_map.insert( dstd::pair<int,int>(30,30) );
	
	Range result = twenty_squares.equal_range(15);
	
	ASSERT_EQ( 20, result.first->first );
	ASSERT_EQ( 20, result.second->first );
}


TEST_F(Map, equalRangeReturnsBeginBeginForNonexistantKeyBeforeAllOtherKeys)
{
	typedef dstd::pair<dstd::map<int,int>::iterator,dstd::map<int,int>::iterator> Range;
	
	Range expected( twenty_squares.begin(), twenty_squares.begin() );
	
	ASSERT_TRUE( equals( expected, twenty_squares.equal_range(-10) ) );
}


TEST_F(Map, equalRangeReturnsEndEndForNonexistantKeyAfterAllOtherKeys)
{
	typedef dstd::pair<dstd::map<int,int>::iterator,dstd::map<int,int>::iterator> Range;
	
	Range expected( twenty_squares.end(), twenty_squares.end() );
	
	ASSERT_TRUE( equals( expected, twenty_squares.equal_range(999) ) );
}


////get_allocator
//// TODO

