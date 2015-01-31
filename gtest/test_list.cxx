#include <gtest/gtest.h>
#include "../src/list.hxx"





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
		
		
// explicit list (const allocator_type& alloc = allocator_type())


// explicit list(size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())


// template <class InputIterator> list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())


// list(const list& x)


// ~list()


// list& operator= (const list& x)


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


// void assign(size_t n, const value_type& val)


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


// void insert (iterator position, size_t n, const value_type& val)


// template <class InputIterator> void insert (iterator position, InputIterator first, InputIterator last)


// iterator erase(iterator position)


// iterator erase(iterator first, iterator last)


// void swap(list& x)


// void resize(size_t n, value_type val = value_type())


// void clear()


TEST_F(List, clearLeaveListEmpty)
{
	one_to_ten.clear();
	
	ASSERT_TRUE( one_to_ten.empty() );
}


// void splice(iterator position, list& x)


// void splice (iterator position, list& x, iterator i)


// void splice (iterator position, list& x, iterator first, iterator last)


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


// template <class Predicate> void remove_if(Predicate pred)


// void unique()


// template <class BinaryPredicate> void unique (BinaryPredicate binary_pred)


// void merge (list& x)


// template <class Compare> void merge (list& x, Compare comp)


//TODO void sort()
//TODO template <class Compare> void sort (Compare comp)


// void reverse()


// allocator_type get_allocator() const





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
