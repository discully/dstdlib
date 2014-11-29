#include <gtest/gtest.h>
#include "../src/vector.hxx"


// Google test cannot deal with class and operators being in different namespaces
namespace dstd
{
	template <class T> bool operator== (const dstd::vector<T>& v1, const dstd::vector<T>& v2)
	{
		return ::operator==(v1, v2);
	}
	template <class T> bool operator!= (const dstd::vector<T>& v1, const dstd::vector<T>& v2)
	{
		return ::operator!=(v1, v2);
	}
}


class Vector : public ::testing::Test
{
	protected:
	
		void SetUp()
		{
			pi = 3.14159;
			
			for(size_t i = 0; i < 50; ++i)
			{
				fifty_times_pi.push_back( i * pi );
			}
			
			for(size_t i = 1; i <= 10; ++i)
			{
				one_to_ten.push_back(i);
			}
			
			for(size_t i = 0; i < 9; ++i)
			{
				nine_nines.push_back(9);
			}
			
			for(size_t i = 30; i <= 40; ++i)
			{
				thirty_to_forty.push_back(i);
			}
		}
	
	double pi;
	dstd::vector<double> empty_d;
	dstd::vector<float> empty_f;
	dstd::vector<int> empty_i;
	dstd::vector<double> fifty_times_pi;
	dstd::vector<int> one_to_ten;
	dstd::vector<int> thirty_to_forty;
	dstd::vector<int> nine_nines;
};


//
// Element access
//


// T& operator[](size_t i);
// const T& operator[](size_t i) const;


TEST_F(Vector, operatorSQBReturnsExpectedElement)
{
	const int i = 5;
	ASSERT_EQ((i+1), one_to_ten[i]);
}


TEST_F(Vector, operatorSQBBeyondEndDoesNotThrow)
{
	ASSERT_NO_THROW( one_to_ten[ one_to_ten.size() ] );
}


TEST_F(Vector, operatorSQBIsAssignable)
{
	dstd::vector<int> v = nine_nines;
	v[3] = 3;
	ASSERT_NE(nine_nines, v);
}


// T& at(size_t i)
// const T& at(size_t i) const


TEST_F(Vector, atReturnsExpectedElement)
{
	const int i = 6;
	ASSERT_EQ((i+1), one_to_ten.at(i));
}


TEST_F(Vector, atBeyondEndThrowsOutOfRange)
{
	ASSERT_THROW(fifty_times_pi.at( fifty_times_pi.size() ), dstd::out_of_range);
}


TEST_F(Vector, atIsAssignable)
{
	dstd::vector<int> v = nine_nines;
	v.at(3) = 3;
	ASSERT_NE(nine_nines, v);
}


// T& front();
// const T& front() const;


TEST_F(Vector, frontReturnsFirstElement)
{
	ASSERT_EQ(1, one_to_ten.front());
}


TEST_F(Vector, frontReferenceIsAssignable)
{
	dstd::vector<int> v = one_to_ten;
	v.front() = 99;
	ASSERT_NE(one_to_ten, v);
}


// T& back();
// const T& back() const;


TEST_F(Vector, backReturnsLastElement)
{
	ASSERT_EQ(10, one_to_ten.back());
}


TEST_F(Vector, backReferenceIsAssignable)
{
	dstd::vector<int> v = one_to_ten;
	v.back() = 99;
	ASSERT_NE(one_to_ten, v);
}


//
// Capacity methods
//


// size_t size() const;


TEST_F(Vector, sizeOfEmptyVectorIsZero)
{
	ASSERT_EQ(0, empty_f.size());
}


TEST_F(Vector, sizeReturnsNumberOfEntries)
{
	ASSERT_EQ(50, fifty_times_pi.size());
}


// size_t max_size() const


TEST_F(Vector, maxSizeIsALargeValue)
{
	ASSERT_GT(empty_f.max_size(), 1e9);
}


// void resize(size_t n, const T& value = T())


TEST_F(Vector, resizeToExistingSizeHasNoEffect)
{
	dstd::vector<double> v = fifty_times_pi;
	
	v.resize(fifty_times_pi.size(), 12.3);
	
	ASSERT_EQ(fifty_times_pi, v);
}


TEST_F(Vector, resizeToLargerSizeAppendsSuppliedValue)
{
	dstd::vector<double> v_push = fifty_times_pi;
	for(size_t i = 0; i < 10; ++i)
	{
		v_push.push_back(pi);
	}
	
	dstd::vector<double> v_resize = fifty_times_pi;
	v_resize.resize(v_push.size(), pi);
	
	ASSERT_EQ(v_push, v_resize);
}


TEST_F(Vector, resizeToSmallerSizeShrinksVector)
{
	dstd::vector<int> v = nine_nines;
	for(size_t i = 0; i < 10; ++i)
	{
		v.push_back(i);
	}
	
	v.resize(nine_nines.size(), 20);
	
	ASSERT_EQ(nine_nines, v);
}


// size_t capacity() const


TEST_F(Vector, capacityIsGreaterThanOrEqualToSize)
{
	ASSERT_GE(one_to_ten.capacity(), one_to_ten.size());
}


// bool empty() const


TEST_F(Vector, emptyReturnsTrueForDefaultConstruction)
{
	ASSERT_TRUE(empty_f.empty());
}


TEST_F(Vector, emptyReturnsFalseAfterPushBack)
{
	dstd::vector<int> v;
	
	v.push_back(4);
	
	ASSERT_FALSE(v.empty());
}


// void reserve(size_t n)


TEST_F(Vector, reserveIncreasesCapactiy)
{
	const size_t new_capacity = 4 * nine_nines.capacity();
	nine_nines.reserve(new_capacity);
	ASSERT_GE(nine_nines.capacity(), new_capacity);
}


TEST_F(Vector, reserveDoesNothingIfLessThanCapacity)
{
	const size_t before = fifty_times_pi.capacity();
	fifty_times_pi.reserve( before / 2 );
	ASSERT_EQ(before, fifty_times_pi.capacity());
}


//
// Modifiers
//


// void assign(const_iterator first, const_iterator last)


TEST_F(Vector, assignIteratorCopiesValuesOntoEmptyVector)
{
	empty_i.assign(one_to_ten.begin(), one_to_ten.end());
	ASSERT_EQ(one_to_ten, empty_i);
}


TEST_F(Vector, assignIteratorCopiesValuesAndDiscardsExisting)
{
	nine_nines.assign(one_to_ten.begin(), one_to_ten.end());
	ASSERT_EQ(one_to_ten, nine_nines);
}


TEST_F(Vector, assignIteratorResultInEmptyVectorForNoRange)
{
	nine_nines.assign(one_to_ten.begin(), one_to_ten.begin());
	ASSERT_EQ(empty_i, nine_nines);
}


// void assign(size_t n, const T& value)


TEST_F(Vector, assignSizeValueSetsValuesOntoEmptyVector)
{
	empty_i.assign(9, 9);
	ASSERT_EQ(nine_nines, empty_i);
}


TEST_F(Vector, assignSizeValueSetsValuesAndDiscardsExisting)
{
	one_to_ten.assign(9, 9);
	ASSERT_EQ(nine_nines, one_to_ten);
}


TEST_F(Vector, assignSizeValueResultsInEmptyVectorForZero)
{
	one_to_ten.assign(0, 9);
	ASSERT_EQ(empty_i, one_to_ten);
}


// iterator insert(iterator position, const T& value)


TEST_F(Vector, insertIteratorValueAtStart)
{
	const int x = -99;
	
	dstd::vector<int> expected;
	expected.push_back(x);
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.insert(one_to_ten.begin(), x);
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertIteratorValueInMiddle)
{
	const int x = -99;
	const size_t pos = 3;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
		if( i == pos ) expected.push_back(x);
	}
	
	one_to_ten.insert(one_to_ten.begin()+pos, x);
		
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertIteratorValueAtEnd)
{
	const double x = -99.9;
	dstd::vector<double> v = fifty_times_pi;
	fifty_times_pi.push_back(x);
	
	v.insert(v.end(), x);
	
	ASSERT_EQ(fifty_times_pi, v);
}


TEST_F(Vector, insertIteratorValueBeyondEndThrowsOutOfRange)
{
	ASSERT_THROW( nine_nines.insert(nine_nines.end()+1, 3), dstd::out_of_range );
}


TEST_F(Vector, insertIteratorValueBeforeBeginThrowsOutOfRange)
{
	ASSERT_THROW( nine_nines.insert(nine_nines.begin()-1, 3), dstd::out_of_range );
}


TEST_F(Vector, insertIteratorValueReturnsIteratorToValue)
{
	const int x = -99;
	dstd::vector<int>::iterator result = one_to_ten.insert(one_to_ten.begin(), x);
	ASSERT_EQ(x, *result);
}


// void insert(iterator position, unsigned int n, const T& value)


TEST_F(Vector, insertIteratorNValueAtStart)
{
	const size_t n = 3;
	const int x = -99;
	
	dstd::vector<int> expected;
	for(size_t i = 0; i < n; ++i)
	{
		expected.push_back(x);
	}
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.insert(one_to_ten.begin(), n, x);
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertIteratorNValueInMiddle)
{
	const int x = -99;
	const size_t pos = 5;
	const size_t n = 3;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
		if( i == pos )
		{
			for(size_t j = 0; j < n; ++j)
			{
				expected.push_back(x);
			}
		}
	}
	
	one_to_ten.insert(one_to_ten.begin()+pos, n, x);
		
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertIteratorNValueAtEnd)
{
	const double x = -99.9;
	const size_t n = 3;
	
	dstd::vector<double> v = fifty_times_pi;
	for(size_t i = 0; i < n; ++i)
	{
		fifty_times_pi.push_back(x);
	}
	
	v.insert(v.end(), n, x);
	
	ASSERT_EQ(fifty_times_pi, v);
}


TEST_F(Vector, insertIteratorNValueBeyondEndThrowsOutOfRange)
{
	ASSERT_THROW( nine_nines.insert(nine_nines.end()+1, 3, 3), dstd::out_of_range );
}


TEST_F(Vector, insertIteratorNValueBeforeBeginThrowsOutOfRange)
{
	ASSERT_THROW( nine_nines.insert(nine_nines.begin()-1, 3, 3), dstd::out_of_range );
}


// void insert(iterator position, iterator first, iterator last)


TEST_F(Vector, insertItItItAtStart)
{
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	for(size_t i = 30; i <= 40; ++i)
	{
		expected.push_back(i);
	}
	
	thirty_to_forty.insert(thirty_to_forty.begin(), one_to_ten.begin(), one_to_ten.end());
	
	ASSERT_EQ(expected, thirty_to_forty);
}


TEST_F(Vector, insertItItItAtMiddle)
{
	const size_t pos = 4;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
		if( i == pos )
		{
			for(size_t i = 30; i <= 40; ++i)
			{
				expected.push_back(i);
			}
		}
	}
	
	one_to_ten.insert(one_to_ten.begin()+pos, thirty_to_forty.begin(), thirty_to_forty.end());
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertItItItAtEnd)
{
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	for(size_t i = 30; i <= 40; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.insert(one_to_ten.end(), thirty_to_forty.begin(), thirty_to_forty.end());
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, insertItItItBeforeBeginThrowsOutOfRange)
{
	ASSERT_THROW( one_to_ten.insert(one_to_ten.begin()-1, thirty_to_forty.begin(), thirty_to_forty.end()), dstd::out_of_range );
}


TEST_F(Vector, insertItItItAfterEndThrowsOutOfRange)
{
	ASSERT_THROW( one_to_ten.insert(one_to_ten.end()+1, thirty_to_forty.begin(), thirty_to_forty.end()), dstd::out_of_range );
}


TEST_F(Vector, insertItItItWithEmptyRangeHasNoEffect)
{
	dstd::vector<int> v = one_to_ten;
	v.insert( v.begin(), thirty_to_forty.begin(), thirty_to_forty.begin());
	ASSERT_EQ(one_to_ten, v);
}


// iterator erase(iterator position)


TEST_F(Vector, eraseIteratorAtStart)
{
	dstd::vector<int> expected;
	for(size_t i = 2; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.erase(one_to_ten.begin());
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseIteratorAtMiddle)
{
	const size_t pos = 4;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= 10; ++i)
	{
		if( i != (pos+1) ) expected.push_back(i);
	}
	
	one_to_ten.erase(one_to_ten.begin()+pos);
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseReturnsIteratorToNextElement)
{
	dstd::vector<int>::iterator pos = one_to_ten.begin() + 4;
	const int expected = *(pos + 1);
	
	dstd::vector<int>::iterator result = one_to_ten.erase(pos);
	
	ASSERT_EQ(expected, *result);
}


// iterator erase(iterator first, iterator last)


TEST_F(Vector, eraseItItBeginToEndResultsInEmptyVector)
{
	one_to_ten.erase( one_to_ten.begin(), one_to_ten.end() );
	ASSERT_EQ(empty_i, one_to_ten);
}


TEST_F(Vector, eraseItItBeginToMiddle)
{
	const size_t n = 4;
	
	dstd::vector<int> expected;
	for(size_t i = n+1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.erase(one_to_ten.begin(), one_to_ten.begin()+n);
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseItItMiddleToEnd)
{
	const size_t n = 2;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= n; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.erase(one_to_ten.begin()+n, one_to_ten.end());
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseItItMiddleToMiddle)
{
	const size_t n = 2;
	
	dstd::vector<int> expected;
	for(size_t i = 1; i <= n; ++i)
	{
		expected.push_back(i);
	}
	for(size_t i = 10-n+1; i <= 10; ++i)
	{
		expected.push_back(i);
	}
	
	one_to_ten.erase(one_to_ten.begin()+n, one_to_ten.end()-n);
	
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseItItEmptyRangeHasNoEffect)
{
	dstd::vector<int> expected = one_to_ten;
	one_to_ten.erase(one_to_ten.begin(), one_to_ten.begin());
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseItItInvalidEmptyRangeHasNoEffect)
{
	dstd::vector<int> expected = one_to_ten;
	one_to_ten.erase(thirty_to_forty.begin(), thirty_to_forty.begin() );
	ASSERT_EQ(expected, one_to_ten);
}


TEST_F(Vector, eraseItItReturnsIteratorToNextElement)
{
	dstd::vector<int>::iterator last = one_to_ten.begin() + 3;
	const int expected = *last;
	dstd::vector<int>::iterator result = one_to_ten.erase(one_to_ten.begin(), last);
	ASSERT_EQ(expected, *result);
}


// void swap(vector& v)


TEST_F(Vector, swapExchangesContents)
{
	dstd::vector<int> ref_one_to_ten = one_to_ten;
	dstd::vector<int> ref_thirty_to_forty = thirty_to_forty;
	
	one_to_ten.swap(thirty_to_forty);
	
	ASSERT_EQ(ref_one_to_ten, thirty_to_forty);
	ASSERT_EQ(ref_thirty_to_forty, one_to_ten);
}


// void clear()


TEST_F(Vector, clearEmptiesVector)
{
	one_to_ten.clear();
	ASSERT_EQ(empty_i, one_to_ten);
}


TEST_F(Vector, clearLeavesCapactiyUnchanged)
{
	const size_t expected = one_to_ten.capacity();
	one_to_ten.clear();
	ASSERT_EQ(expected, one_to_ten.capacity());
}


//
// Vector Operators
//


class VectorOperators : public ::testing::Test
{
	protected:
	
		void SetUp()
		{		
			for(char x = 'a'; x != 'f'; ++x)
			{
				abcde.push_back(x);
				if( x != 'd' && x != 'e' ) abc.push_back(x);
				if( x == 'c' || x == 'e' )
				{
					abddd.push_back('d');
				}
				else
				{
					abddd.push_back(x);
				}
			}
			ae = abcde;
		}
	
	dstd::vector<char> abcde, abc, abddd, empty, ae;
};


// operator==


TEST_F(VectorOperators, equalsReturnsTrueForIdenticalVectors)
{
	ASSERT_TRUE( abcde == ae );
}


TEST_F(VectorOperators, equalsReturnsFalseForABCDEandABC)
{
	ASSERT_FALSE( abcde == abc );
}


TEST_F(VectorOperators, equalsReturnsFalseForABCDEandABDDD)
{
	ASSERT_FALSE( abcde == abddd );
}


TEST_F(VectorOperators, equalsReturnsFalseForABCDEandEmpty)
{
	ASSERT_FALSE( abcde == empty );
}


// operator!=


TEST_F(VectorOperators, notequalsReturnsFalseForIdenticalVectors)
{
	ASSERT_FALSE( abcde != ae );
}


TEST_F(VectorOperators, notequalsReturnsTrueForABCDEandABC)
{
	ASSERT_TRUE( abcde != abc );
}


TEST_F(VectorOperators, notequalsReturnsTrueForABCDEandABDDD)
{
	ASSERT_TRUE( abcde != abddd );
}


TEST_F(VectorOperators, notequalsReturnsTrueForABCDEandEmpty)
{
	ASSERT_TRUE( abcde != empty );
}


// operator<


TEST_F(VectorOperators, lessthanReturnsFalseForIdenticalVectors)
{
	ASSERT_FALSE( abcde < ae );
}


TEST_F(VectorOperators, lessthanReturnsFalseForABCDEandABC)
{
	ASSERT_FALSE( abcde < abc );
}


TEST_F(VectorOperators, lessthanReturnsTrueForABCDEandABDDD)
{
	ASSERT_TRUE( abcde < abddd );
}


TEST_F(VectorOperators, lessthanReturnsFalseForABCDEandEmpty)
{
	ASSERT_FALSE( abcde < empty );
}


// operator<=


TEST_F(VectorOperators, lessthanorequalsReturnsTrueForIdenticalVectors)
{
	ASSERT_TRUE( abcde <= ae );
}


TEST_F(VectorOperators, lessthanorequalsReturnsFalseForABCDEandABC)
{
	ASSERT_FALSE( abcde <= abc );
}


TEST_F(VectorOperators, lessthanorequalsReturnsTrueForABCDEandABDDD)
{
	ASSERT_TRUE( abcde <= abddd );
}


TEST_F(VectorOperators, lessthanorequalsReturnsFalseForABCDEandEmpty)
{
	ASSERT_FALSE( abcde <= empty );
}


// operator>


TEST_F(VectorOperators, greaterthanReturnsFalseForIdenticalVectors)
{
	ASSERT_FALSE( abcde > ae );
}


TEST_F(VectorOperators, greaterthanReturnsTrueForABCDEandABC)
{
	ASSERT_TRUE( abcde > abc );
}


TEST_F(VectorOperators, greaterthanReturnsFalseForABCDEandABDDD)
{
	ASSERT_FALSE( abcde > abddd );
}


TEST_F(VectorOperators, greaterthanReturnsTrueForABCDEandEmpty)
{
	ASSERT_TRUE( abcde > empty );
}


// operator>=


TEST_F(VectorOperators, greaterthanorequalReturnsTrueForIdenticalVectors)
{
	ASSERT_TRUE( abcde >= ae );
}


TEST_F(VectorOperators, greaterthanorequalReturnsTrueForABCDEandABC)
{
	ASSERT_TRUE( abcde >= abc );
}


TEST_F(VectorOperators, greaterthanorequalReturnsFalseForABCDEandABDDD)
{
	ASSERT_FALSE( abcde >= abddd );
}


TEST_F(VectorOperators, greaterthanorequalReturnsTrueForABCDEandEmpty)
{
	ASSERT_TRUE( abcde >= empty );
}


//
// Swap
//


TEST(VectorSwap, swapExchangesContents)
{
	dstd::vector<int> one_to_ten, ref_one_to_ten, thirty_to_forty, ref_thirty_to_forty;
	for(size_t i = 1; i <= 10; ++i)
	{
		one_to_ten.push_back(i);
		ref_one_to_ten.push_back(i);
	}
	for(size_t i = 30; i <= 40; ++i)
	{
		thirty_to_forty.push_back(i);
		ref_thirty_to_forty.push_back(i);
	}
	
	dstd::swap(one_to_ten,thirty_to_forty);
	
	ASSERT_EQ(ref_one_to_ten, thirty_to_forty);
	ASSERT_EQ(ref_thirty_to_forty, one_to_ten);
}
