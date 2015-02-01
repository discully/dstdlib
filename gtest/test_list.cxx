#include <gtest/gtest.h>
#include "../src/list.hxx"



namespace dstd
{
	template <class T> bool operator== (const dstd::list<T>& x1, const dstd::list<T>& x2)
	{
		return ::operator==(x1, x2);
	}
	template <class T> bool operator!= (const dstd::list<T>& x1, const dstd::list<T>& x2)
	{
		return ::operator!=(x1, x2);
	}
}



class List : public ::testing::Test
{
	protected:
	
		void SetUp()
		{
			empty_d.clear();
			empty_f.clear();
			empty_i.clear();
			
			one_to_ten.clear();
			for(size_t i = 1; i <= 10; ++i)
			{
				one_to_ten.push_back(i);
			}
			
			ntimesn.clear();
			for(size_t i = 0; i < 5; ++i)
			{
				for(size_t j = i; j < 5; ++j)
				{
					ntimesn.push_back(j);
				}
			}
		}
		
		dstd::list<double> empty_d;
		dstd::list<float> empty_f;
		dstd::list<int> empty_i;
		dstd::list<int> one_to_ten;
		dstd::list<int> ntimesn;
};


class BothOdd
{
	public:
		bool operator() (const int& lhs, const int& rhs) const
		{
			return ( ((lhs % 2) != 0) && ((rhs % 2) != 0) );
		}
};


	
// explicit list (const allocator_type& alloc = allocator_type())
//
// todo


// explicit list(size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
//
// todo


// template <class InputIterator> list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
//
// todo


// list(const list& x)


TEST_F(List, copyConstructorCreatesCopy)
{
	dstd::list<int> test_list(one_to_ten);
	
	ASSERT_EQ( one_to_ten, test_list );
}


// list& operator= (const list& x)


TEST_F(List, asignmentOperatorCopiesList)
{
	dstd::list<int> test_list;
	
	test_list = one_to_ten;
	
	ASSERT_EQ( one_to_ten, test_list );
}


// iterator begin()
// const_iterator begin() const


TEST_F(List, beginReturnsIteratorToFirstElement)
{
	ASSERT_EQ( 1, *(one_to_ten.begin()) );
}


TEST_F(List, beginReturnsEndForEmptyList)
{
	ASSERT_EQ( empty_d.end(), empty_d.begin() );
}


// iterator end()
// const_iterator end() const


TEST_F(List, endReturnsIteratorBeyondTheEnd)
{
	dstd::list<int>::iterator end = one_to_ten.end();
	--end;
	
	ASSERT_EQ( 10, *end );
}


// reverse_iterator rbegin()
// const_reverse_iterator rbegin() const


TEST_F(List, rbeginReturnsReverseIteratorToLastElement)
{
	ASSERT_EQ( 10, *(one_to_ten.rbegin()) );
}


/// todo
///TEST_F(List, rbeginReturnsRendForEmptyList)
///{
///	ASSERT_EQ( empty_d.rend(), empty_d.rbegin() );
///}


// reverse_iterator rend()
// const_reverse_iterator rend() const


TEST_F(List, rendReturnsReverseIteratorBeyondTheEnd)
{
	dstd::list<int>::reverse_iterator rend = one_to_ten.rend();
	--rend;
	
	ASSERT_EQ( 1, *rend );
}


// bool empty() const


TEST_F(List, emptyReturnsTrueForEmtpyList)
{
	ASSERT_TRUE( empty_d.empty() );
}


TEST_F(List, emptyReturnsFalseForNonEmptyList)
{
	ASSERT_FALSE( one_to_ten.empty() );
}


// size_t size() const


TEST_F(List, sizeReturnsZeroForEmptyList)
{
	ASSERT_EQ( 0, empty_d.size() );
}


TEST_F(List, sizeReturnsNumberOfElementsInList)
{
	ASSERT_EQ( 10, one_to_ten.size() );
}


// size_t max_size() const


TEST_F(List, maxSizeReturnsLargePositiveInteger)
{
	ASSERT_GT( one_to_ten.max_size(), 9999 );
}


// reference front()
// const_reference front() const


TEST_F(List, frontReturnsFirstElementInList)
{
	ASSERT_EQ( 1, one_to_ten.front() );
}


// reference back()
// const_reference back() const


TEST_F(List, backReturnsLastElementInList)
{
	ASSERT_EQ( 10, one_to_ten.back() );
}


// template <class InputIterator> void assign(InputIterator first, InputIterator last)


TEST_F(List, assignIteratorIteratorRemovesExistingElements)
{
	dstd::list<int> temp;
	
	one_to_ten.assign( temp.begin(), temp.begin() );
	
	ASSERT_TRUE( one_to_ten.empty() );
}


TEST_F(List, assignIteratorIteratorInsertsElementsInRange)
{
	one_to_ten.assign( ntimesn.begin(), ntimesn.end() );
	
	ASSERT_EQ( ntimesn, one_to_ten );
}


// void assign(size_t n, const value_type& val)


TEST_F(List, assignSizeValueRemovesExistingElements)
{
	one_to_ten.assign( 0, 99 );
	
	ASSERT_TRUE( one_to_ten.empty() );
}


TEST_F(List, assignSizeValueCreatesSizeElements)
{
	const size_t n = 33;
	
	one_to_ten.assign( n, 99 );
	
	ASSERT_EQ( n, one_to_ten.size() );
}


TEST_F(List, assignSizeValueCreatesElementsEqualToValue)
{
	const int value = 19;
	
	one_to_ten.assign( 10, value );
	
	for(dstd::list<int>::iterator it = one_to_ten.begin(); it != one_to_ten.end(); ++it)
	{
		ASSERT_EQ( value, *it );
	}
}


// void push_front(const value_type& val)


TEST_F(List, pushFrontInsertsNewElementAtFront)
{
	const int new_value = 99;
	
	one_to_ten.push_front(new_value);
	
	ASSERT_EQ( new_value, one_to_ten.front() );
}


TEST_F(List, pushFrontIncreasesSizeByOne)
{
	size_t before = one_to_ten.size();
	
	one_to_ten.push_front(99);
	
	ASSERT_EQ( before+1, one_to_ten.size() );
}


// void pop_front()


TEST_F(List, popFrontRemovesFirstElement)
{
	one_to_ten.pop_front();
	
	ASSERT_EQ( 2, one_to_ten.front() );
}


TEST_F(List, popFrontReducesSizeByOne)
{
	size_t before = one_to_ten.size();
	
	one_to_ten.pop_front();
	
	ASSERT_EQ( before-1, one_to_ten.size() );
}


// void push_back(const value_type& val)


TEST_F(List, pushBackInsertsNewElementAtBack)
{
	const int new_value = 99;
	
	one_to_ten.push_back(new_value);
	
	ASSERT_EQ( new_value, one_to_ten.back() );
}


TEST_F(List, pushBackIncreasesSizeByOne)
{
	size_t before = one_to_ten.size();
	
	one_to_ten.push_back(99);
	
	ASSERT_EQ( before+1, one_to_ten.size() );
}


// void pop_back()


TEST_F(List, popBackRemovesLastElement)
{
	one_to_ten.pop_back();
	
	ASSERT_EQ( 9, one_to_ten.back() );
}


TEST_F(List, popBackReducesSizeByOne)
{
	size_t before = one_to_ten.size();
	
	one_to_ten.pop_back();
	
	ASSERT_EQ( before-1, one_to_ten.size() );
}


// iterator insert (iterator position, const value_type& val)


TEST_F(List, insertIteratorValueInsertsBeforeIterator)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	++before;
	
	dstd::list<int>::iterator after = before;
	++after;
	
	one_to_ten.insert( after, 99 );
	
	dstd::list<int>::iterator it = before;
	++it;
	ASSERT_NE( before, it );
	++it;
	ASSERT_EQ( after, it );
}


TEST_F(List, insertIteratorValueInsertsSpecifiedValue)
{
	dstd::list<int>::iterator it = one_to_ten.begin();
	++it;
	++it;
	
	const int value = 18;
	one_to_ten.insert( it, value );
	
	--it;
	ASSERT_EQ( value, *it );
}


TEST_F(List, insertIteratorValueReturnsIteratorToNewElement)
{
	dstd::list<int>::iterator position = one_to_ten.begin();
	++position;
	++position;
	const int value = 192;
	
	dstd::list<int>::iterator result = one_to_ten.insert( position, value );
	
	ASSERT_EQ( value, *result );
	ASSERT_EQ( position, ++result );
}


// void insert (iterator position, size_t n, const value_type& val)


TEST_F(List, insertIteratorSizeValueInsertsBeforeIterator)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	++before;
	dstd::list<int>::iterator after = before;
	++after;
	const size_t n = 3;
	
	one_to_ten.insert( after, n, 99 );
	
	dstd::list<int>::iterator it = before;
	for(size_t i = 0; i < n; ++i)
	{
		++it;
		ASSERT_NE( before, it );
	}
	++it;
	ASSERT_EQ( after, it );
}


TEST_F(List, insertIteratorSizeValueIncreasesSizeBySpecifiedSize)
{
	const size_t before = one_to_ten.size();
	const size_t n = 15;
	
	one_to_ten.insert( one_to_ten.begin(), n, 99 );
	
	ASSERT_EQ( before+n, one_to_ten.size() );
}


TEST_F(List, insertIteratorSizeValueInsertsSpecifiedValue)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	++before;
	dstd::list<int>::iterator after = before;
	++after;
	const size_t n = 4;
	const int value = 18;
	
	one_to_ten.insert( after, n, value );
	
	dstd::list<int>::iterator it = after;
	--it;
	while(it != before)
	{
		ASSERT_EQ( value, *it );
		--it;
	}
}


TEST_F(List, insertIteratorSizeValueWithSizeZeroLeavesListUnchanged)
{
	const dstd::list<int> original = one_to_ten;
	
	one_to_ten.insert( one_to_ten.begin(), 0, 99 );
	
	ASSERT_EQ( original, one_to_ten );
}


// template <class InputIterator> void insert (iterator position, InputIterator first, InputIterator last)


TEST_F(List, insertIteratorIteratorIteratorInsertsBeforePosition)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	++before;
	dstd::list<int>::iterator after = before;
	++after;
	
	one_to_ten.insert( after, ntimesn.begin(), ntimesn.end() );
	
	dstd::list<int>::iterator it = before;
	for(size_t i = 0; i < ntimesn.size(); ++i)
	{
		++it;
		ASSERT_NE( before, it );
	}
	++it;
	ASSERT_EQ( after, it );
}


TEST_F(List, insertIteratorIteratorIteratorInsertsSpecifiedValues)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	++before;
	dstd::list<int>::iterator after = before;
	++after;
	
	one_to_ten.insert( after, ntimesn.begin(), ntimesn.end() );
	
	dstd::list<int>::iterator it = before;
	++it;
	dstd::list<int>::iterator expected = ntimesn.begin();
	while(it != after)
	{
		ASSERT_EQ( *expected, *it );
		++it;
		++expected;
	}
}


// iterator erase(iterator position)


TEST_F(List, eraseIteratorReducesSizeByOne)
{
	dstd::list<int>::iterator pos = one_to_ten.begin();
	++pos;
	++pos;
	const size_t before = one_to_ten.size();
	
	one_to_ten.erase(pos);
	
	ASSERT_EQ( before-1, one_to_ten.size() );
}


TEST_F(List, eraseIteratorRemoveElement)
{
	dstd::list<int>::iterator pos = one_to_ten.begin();
	++pos;
	++pos;
	dstd::list<int>::iterator before = pos;
	++pos;
	dstd::list<int>::iterator after = pos;
	++after;
	
	one_to_ten.erase(pos);
	
	ASSERT_EQ( after, ++before );
}


TEST_F(List, eraseIteratorReturnsElementAfterRemovedElement)
{
	dstd::list<int>::iterator pos = one_to_ten.begin();
	++pos;
	++pos;
	dstd::list<int>::iterator before = pos;
	++pos;
	dstd::list<int>::iterator after = pos;
	++after;
	
	dstd::list<int>::iterator result = one_to_ten.erase(pos);
	
	ASSERT_EQ( after, result );
}


// iterator erase(iterator first, iterator last)


TEST_F(List, eraseIteratorIteratorReducesSizeByNumberOfElementsInRange)
{
	const size_t before = one_to_ten.size();
	dstd::list<int>::iterator first = one_to_ten.begin();
	++first;
	dstd::list<int>::iterator last = first;
	const size_t n = 3;
	for(size_t i = 0; i < n; ++i)
	{
		++last;
	}
	
	one_to_ten.erase(first, last);
	
	ASSERT_EQ( before-n, one_to_ten.size() );
}


TEST_F(List, eraseIteratorIteratorRemovesElementsInRange)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	dstd::list<int>::iterator first = before;
	++first;
	dstd::list<int>::iterator last = first;
	++last;
	++last;
	++last;
	
	one_to_ten.erase(first, last);
	
	ASSERT_EQ( last, ++before );
}


TEST_F(List, eraseIteratorIteratorReturnsIteratorToEndIteratorOfRange)
{
	dstd::list<int>::iterator before = one_to_ten.begin();
	++before;
	dstd::list<int>::iterator first = before;
	++first;
	dstd::list<int>::iterator last = first;
	++last;
	++last;
	++last;
	
	dstd::list<int>::iterator result = one_to_ten.erase(first, last);
	
	ASSERT_EQ( last, result );
}


// void swap(list& x)


TEST_F(List, swapExchangesListContents)
{
	dstd::list<int> original_one_to_ten = one_to_ten;
	dstd::list<int> original_ntimesn = ntimesn;
	
	one_to_ten.swap(ntimesn);
	
	ASSERT_EQ( original_one_to_ten, ntimesn );
	ASSERT_EQ( original_ntimesn, one_to_ten );
}


// void resize(size_t n, value_type val = value_type())


TEST_F(List, resizeWithNSmallerThanSizeReducesListSize)
{
	const size_t n = one_to_ten.size() / 2;
	
	one_to_ten.resize(n);
	
	ASSERT_EQ( n, one_to_ten.size() );
}


TEST_F(List, resizeWithNSmallerThanSizeReducesListFromBack)
{
	const size_t n = one_to_ten.size() / 2;
	const size_t diff = one_to_ten.size() - n;
	dstd::list<int>::iterator it = one_to_ten.end();
	for(size_t i = 0; i <= diff; ++i)
	{
		--it;
	}
	
	one_to_ten.resize(n);
	
	ASSERT_EQ( one_to_ten.end(), ++it );
}


TEST_F(List, resizeWithNEqualToSizeLeavesListUnaffected)
{
	const dstd::list<int> original = one_to_ten;
	
	one_to_ten.resize( one_to_ten.size() );
	
	ASSERT_EQ( original, one_to_ten );
}


TEST_F(List, resizeWithNLargerThanSizeIncreasesListSize)
{
	const size_t n = one_to_ten.size() * 2;
	
	one_to_ten.resize(n);
	
	ASSERT_EQ( n, one_to_ten.size() );
}


TEST_F(List, resizeWithNLargerThanSizeIncreasesListFromBack)
{
	const size_t n = one_to_ten.size() * 2;
	const size_t diff = n - one_to_ten.size();
	dstd::list<int>::iterator it = one_to_ten.end();
	--it;
	
	one_to_ten.resize(n);
	
	for(size_t i = 0; i <= diff; ++i)
	{
		++it;
	}
	ASSERT_EQ( one_to_ten.end(), it );
}


// void clear()


TEST_F(List, clearLeavesListEmpty)
{
	one_to_ten.clear();
	
	ASSERT_TRUE( one_to_ten.empty() );
}


// void splice(iterator position, list& x)
//
// todo


// void splice (iterator position, list& x, iterator i)
//
// todo


// void splice (iterator position, list& x, iterator first, iterator last)
//
// todo


// void remove(const value_type& val)


TEST_F(List, removeValueRemovesAllOccuranceOfThatValue)
{
	const int to_remove = 3;
	
	ntimesn.remove(to_remove);
	
	for(dstd::list<int>::iterator it = ntimesn.begin(); it != ntimesn.end(); ++it)
	{
		ASSERT_NE( to_remove, *it );
	}
}


TEST_F(List, removeWithNonexistantValueLeavesListUnchanged)
{
	const dstd::list<int> original = one_to_ten;
	
	one_to_ten.remove(-1);
	
	ASSERT_EQ( original, one_to_ten );
}


// template <class Predicate> void remove_if(Predicate pred)
//
// todo


// void unique()


TEST_F(List, uniqueLeavesListUnchangedIfNoRepeatedElements)
{
	const size_t before = one_to_ten.size();
	
	one_to_ten.unique();
	
	ASSERT_EQ( before, one_to_ten.size() );
}


TEST_F(List, uniqueRemovesConsecutiveRepeatedElements)
{
	dstd::list<int> test_list;
	test_list.push_back(1);
	test_list.push_back(2);
	test_list.push_back(3);
	test_list.push_back(4);
	const dstd::list<int> expected = test_list;
	test_list.push_back(4);
	test_list.push_back(4);
	test_list.push_back(4);
	
	test_list.unique();
	
	ASSERT_EQ( expected, test_list );
}


TEST_F(List, uniqueDoesNotRemoveNonConsecutiveRepeatedElements)
{
	dstd::list<int> test_list;
	test_list.push_back(1);
	test_list.push_back(2);
	test_list.push_back(3);
	test_list.push_back(4);
	test_list.push_back(3);
	test_list.push_back(4);
	test_list.push_back(3);
	const dstd::list<int> expected = test_list;
	
	test_list.unique();
	
	ASSERT_EQ( expected, test_list );
}


// template <class BinaryPredicate> void unique (BinaryPredicate binary_pred)


TEST_F(List, uniquePredLeavesListUnchangedIfNoRepeatedPredElements)
{
	dstd::list<int> test_list;
	test_list.push_back(2);
	test_list.push_back(2);
	test_list.push_back(1);
	test_list.push_back(2);
	test_list.push_back(2);
	const dstd::list<int> expected = test_list;
	
	test_list.unique( BothOdd() );
	
	ASSERT_EQ( expected, test_list );
}


TEST_F(List, uniquePredRemovesConsecutivePredElements)
{
	dstd::list<int> test_list;
	test_list.push_back(2);
	test_list.push_back(2);
	test_list.push_back(2);
	test_list.push_back(1);
	const dstd::list<int> expected = test_list;
	test_list.push_back(1);
	test_list.push_back(1);
	
	test_list.unique( BothOdd() );
	
	ASSERT_EQ( expected, test_list );
}


TEST_F(List, uniquePredDoesNotRemoveNonConsecutivePredElements)
{
	dstd::list<int> test_list;
	test_list.push_back(2);
	test_list.push_back(2);
	test_list.push_back(2);
	test_list.push_back(1);
	test_list.push_back(2);
	test_list.push_back(1);
	const dstd::list<int> expected = test_list;
	
	test_list.unique( BothOdd() );
	
	ASSERT_EQ( expected, test_list );
}


// void merge (list& x)
//
// todo
//
// todo
// for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
//
// todo
// the order of equivalent elements of *this and other does not change.


// template <class Compare> void merge (list& x, Compare comp)
//
// todo


//TODO void sort()
//TODO template <class Compare> void sort (Compare comp)


// void reverse()
//
// todo


// allocator_type get_allocator() const
//
// todo





//
// todo
//
///////
/////// List comparison operators
///////
/////
/////template<class T, class Allocator>
/////bool operator==( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
/////template< class T, class Allocator >
/////bool operator!=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
/////template< class T, class Allocator >
/////bool operator< (const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
/////template< class T, class Allocator >
/////bool operator<=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
/////template< class T, class Allocator >
/////bool operator>( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
/////template< class T, class Allocator >
/////bool operator>=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
/////
/////
///////
/////// Swap
///////
/////
/////template <class T, class Allocator>
/////void dstd::swap(dstd::list<T,Allocator>& l1, dstd::list<T,Allocator>& l2)
