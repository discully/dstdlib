#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/algorithm.hxx"
#include "../src/list.hxx"
#include "../src/utility.hxx"
#include "../src/vector.hxx"



template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::vector<T>& v)
{
	stream << "dstd::vector(";
	for(typename dstd::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
	{
		stream << *it << ", ";
	}
	stream << ")";
	return stream;
}



bool oddEqual(const int& a, const int& b) // returns true if both numbers are odd, or both numbers are even
{
	return ( a%2 == b%2 );
}


bool oddLessThan(const int& a, const int& b) // sort such that odd numbers come before even, then smaller numbers before larger
{
	if( a % 2  == b % 2 ) return a < b;
	else if( a % 2 == 0 ) return false;
	else return true;
}


bool areEqual(const int& a, const int& b)
{
	return ( a == b );
}


bool isOdd(const int& a)
{
	return ! ( (a % 2) == 0 );
}


int increment(int& i)
{
	++i;
	return 0;
}


template <class Container>
bool is_heap(Container c, unsigned int size, unsigned int n)
{
	if( n < size )
	{
		unsigned int n_child = 2*n + 1;
		if( n_child < size )
		{
			if( ! is_heap(c, size, n_child) ) return false;
			if( c.at(n) < c.at(n_child) ) return false;
		
			++n_child;
			if( n_child < size )
			{
				if( ! is_heap(c, size, n_child) ) return false;
				if( c.at(n) < c.at(n_child) ) return false;
			}
		}
	}
	return true;
}

template <class Container>
bool is_heap(Container c)
{
	return is_heap(c, c.size(), 0);
}



class IntSummation
{
	public:
		IntSummation() : n(0) {}
		void operator() (int i) { this->n += i; }
		int n;
};



int main()
{
	dstd::Tester t("<algorithm>");
	
	
	//
	//Non-modifying sequence operations
	//
	
	// for_each
	
	{
		int sum = 0;
		dstd::vector<int> v, vv;
		for(int i = 0; i < 10; ++i)
		{
			sum += i;
			v.push_back(i);
			vv.push_back(i + 1);
		}
		
		IntSummation s = dstd::for_each(v.begin(), v.end(), IntSummation());
		t.testEqual("for_each 1", s.n, sum);
		
		s = dstd::for_each(v.begin(), v.begin(), IntSummation());
		t.testEqual("for_each 2", s.n, 0);
		
		dstd::for_each(v.begin(), v.end(), increment);
		t.testEqual("for_each 3", v, vv);
	}
	
	// count
	
	{
		dstd::vector<int> v;
		int odd = 0;
		const int n = 10;
		const int x = n/2;
		for(int i = 0; i < n; ++i)
		{
			v.push_back(i);
			if( isOdd(i) ) ++odd;
			if( i == x )
			{
				v.push_back(i);
				if( isOdd(i) ) ++odd;
			}
		}
		
		t.testEqual("count 1", dstd::count(v.begin(), v.end(), x+1), 1);
		t.testEqual("count 2", dstd::count(v.begin(), v.end(), x), 2);
		t.testEqual("count 3", dstd::count(v.begin(), v.end(), n), 0);
		t.testEqual("count 4", dstd::count(v.begin(), v.begin(), x), 0);
	}
	
	// count_if
	
	{
		dstd::vector<int> v;
		int odd = 0;
		const int n = 10;
		const int x = n/2;
		for(int i = 0; i < n; ++i)
		{
			v.push_back(i);
			if( isOdd(i) ) ++odd;
			if( i == x )
			{
				v.push_back(i);
				if( isOdd(i) ) ++odd;
			}
		}
		
		t.testEqual("count_if 1", dstd::count_if(v.begin(), v.end(),   isOdd), odd);
		t.testEqual("count_if 2", dstd::count_if(v.begin(), v.begin(), isOdd), 0);
	}
	
	// mismatch
	
	{
		dstd::pair< dstd::list<int>::const_iterator, dstd::list<int>::const_iterator > result;
		const int n = 10;
		dstd::list<int> l1;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(i);
		}
		const dstd::list<int> l2(l1), l0a, l0b;
		
		// empty containers
		result = dstd::mismatch(l0a.begin(), l0a.end(), l0b.begin());
		t.testEqual("mismatch 1a", result.first,  l0a.end());
		t.testEqual("mismatch 1b", result.second, l0b.end());
		
		// empty ranges
		result = dstd::mismatch(l1.begin(), l1.begin(), l2.begin());
		t.testEqual("mismatch 2a", result.first,  l1.begin());
		t.testEqual("mismatch 2b", result.second, l2.begin());
		
		// no mismatch
		result = dstd::mismatch(l1.begin(), l1.end(), l2.begin());
		t.testEqual("mismatch 3a", result.first,  l1.end());
		t.testEqual("mismatch 3b", result.second, l2.end());
		
		// find mismatch
		l1.remove(n/2);
		result = dstd::mismatch(l1.begin(), l1.end(), l2.begin());
		t.testEqual("mismatch 4a", *(result.first), (n/2) + 1);
		t.testEqual("mismatch 4b", *(result.second), n/2);
	}
	{
		dstd::pair< dstd::vector<int>::const_iterator, dstd::vector<int>::const_iterator > result;
		
		const int n = 20;
		dstd::vector<int> v0a, v0b, v1, v2, v3;
		for(int i = 0; i < n; ++i)
		{
			v1.push_back(i);
			v2.push_back(i+2);
			if( i != n/2 )
			{
				v3.push_back(i+2);
			}
			else
			{
				v3.push_back(i+1);
			}
		}
		
		// empty containers
		result = dstd::mismatch(v0a.begin(), v0a.end(), v0b.begin(), oddEqual);
		t.testEqual("mismatch(p) 1a", result.first,  v0a.end());
		t.testEqual("mismatch(p) 1b", result.second, v0b.end());
		
		// empty ranges
		result = dstd::mismatch(v1.begin(), v1.begin(), v2.begin(), oddEqual);
		t.testEqual("mismatch(p) 2a", result.first,  v1.begin());
		t.testEqual("mismatch(p) 2b", result.second, v2.begin());
		
		// no mismatch
		result = dstd::mismatch(v1.begin(), v1.end(), v2.begin(), oddEqual);
		t.testEqual("mismatch(p) 3a", result.first,  v1.end());
		t.testEqual("mismatch(p) 3b", result.second, v2.end());
		
		// find mismatch
		result = dstd::mismatch(v1.begin(), v1.end(), v3.begin(), oddEqual);
		t.testEqual("mismatch 4a", *(result.first), (n/2));
		t.testEqual("mismatch 4b", *(result.second),(n/2) + 1);
	}
	
	// equal
	
	{
		dstd::list<int> l0, l1, l2;
		dstd::vector<int> v0, v1, v2;
		for(int i = 0; i < 20; ++i)
		{
			l1.push_back(i);
			v1.push_back(i);
			l2.push_back(i/2);
			v2.push_back(i/2);
		}
		
		t.testEqual("equal 1", dstd::equal(l1.begin(), l1.end(), v1.begin()), true);
		t.testEqual("equal 2", dstd::equal(l1.begin(), l1.end(), v2.begin()), false);
		t.testEqual("equal 3", dstd::equal(l2.begin(), l2.end(), v1.begin()), false);
		t.testEqual("equal 4", dstd::equal(l0.begin(), l0.end(), v0.begin()), true);
		t.testEqual("equal 5", dstd::equal(l1.begin(), l1.begin(), v1.begin()), true);
		
		t.testEqual("equal(p) 1", dstd::equal(l1.begin(), l1.end(), v1.begin(), oddEqual), true);
		t.testEqual("equal(p) 2", dstd::equal(l1.begin(), l1.end(), v2.begin(), oddEqual), false);
		t.testEqual("equal(p) 3", dstd::equal(l2.begin(), l2.end(), v1.begin(), oddEqual), false);
		t.testEqual("equal(p) 4", dstd::equal(l0.begin(), l0.end(), v0.begin(), oddEqual), true);
		t.testEqual("equal(p) 5", dstd::equal(l1.begin(), l1.begin(), v1.begin(), oddEqual), true);
	}
	
	// find
	
	{
		const int n = 20;
		const double x1 = 3.14;
		const double x2 = 2 * x1;
		const double x3 = 2 * x2;
		dstd::list<double> l1, l0;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(x1);
			if( i == n/2 ) l1.push_back(x2);
		}
		
		dstd::list<double>::iterator result;
		
		// empty container
		result = dstd::find(l0.begin(), l0.end(), x1);
		t.testEqual("find 1", result, l0.end());
		
		// empty range
		result = dstd::find(l1.begin(), l1.begin(), x1);
		t.testEqual("find 2", result, l1.begin());
		
		// multiple entries
		result = dstd::find(l1.begin(), l1.end(), x1);
		t.testEqual( "find 3a", result, l1.begin() );
		t.testEqual( "find 3b", *result, x1 );
		
		// single entry
		result = dstd::find(l1.begin(), l1.end(), x2);
		t.testNotEqual( "find 4a", result, l1.end() );
		t.testEqual( "find 4b", *result, x2 );
		
		// no entries
		result = dstd::find(l1.begin(), l1.end(), x3);
		t.testEqual( "find 5", result, l1.end() );
	}
	
	// find_if
	
	{
		const int n = 20;
		const int x1 = 2;
		const int x2 = 3;
		dstd::list<int> l0, l1, l2, l3;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(x1);
			l2.push_back(x2);
			l3.push_back(x1);
			if( i == n/2 )
			{
				l1.push_back(x2);
				l2.push_back(x1);
			}
		}
		
		dstd::list<int>::iterator result;
		
		// empty container
		result = dstd::find_if(l0.begin(), l0.end(), isOdd);
		t.testEqual("find_if 1", result, l0.end());
		
		// empty range
		result = dstd::find_if(l1.begin(), l1.begin(), isOdd);
		t.testEqual("find_if 2", result, l1.begin());
		
		// multiple entries
		result = dstd::find_if(l2.begin(), l2.end(), isOdd);
		t.testEqual( "find_if 3a", result, l2.begin() );
		t.testEqual( "find_if 3b", *result, x2 );
		
		// single entry
		result = dstd::find_if(l1.begin(), l1.end(), isOdd);
		t.testNotEqual( "find_if 4a", result, l1.end() );
		t.testEqual( "find_if 4b", *result, x2 );
		
		// no entries
		result = dstd::find_if(l3.begin(), l3.end(), isOdd);
		t.testEqual( "find 5", result, l3.end() );
	}
	
	// find_end
	
	{
		const int n = 10;
		dstd::list<int> l0, l1, l2, l3, l;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(i);
			l2.push_back(n);
			l3.push_back(0);
		}
		
		l.insert(l.end(), l1.begin(), l1.end());
		l.insert(l.end(), l2.begin(), l2.end());
		l.insert(l.end(), l1.begin(), l1.end());
		dstd::list<int>::iterator it2 = --(l.end());
		l.insert(l.end(), l2.begin(), l2.end());
		++it2;
		dstd::list<int>::iterator it1 = --(l.end());
		l.insert(l.end(), l1.begin(), l1.end());
		++it1;
		
		t.testEqual("find_end 1", dstd::find_end(l.begin(), l.end(), l1.begin(), l1.end()), it1);
		t.testEqual("find_end 2", dstd::find_end(l.begin(), l.end(), l2.begin(), l2.end()), it2);
		t.testEqual("find_end 3", dstd::find_end(l.begin(), l.end(), l2.begin(), l2.end()), it2);
		t.testEqual("find_end 4", dstd::find_end(l.begin(), l.end(), l3.begin(), l3.end()), l.end());
		
		// empty search-container
		t.testEqual("find_end 5a", dstd::find_end(l0.begin(), l0.end(), l1.begin(), l1.end()), l0.end());
		// empty sequence-container
		t.testEqual("find_end 5b", dstd::find_end(l.begin(), l.end(), l0.begin(), l0.end()), l.end());
		// empty search-container and sequence-container
		t.testEqual("find_end 5c", dstd::find_end(l0.begin(), l0.end(), l0.begin(), l0.end()), l0.end());
		
		// empty search-range
		t.testEqual("find_end 6a", dstd::find_end(l.begin(), l.begin(), l1.begin(), l1.end()), l.begin());
		// empty sequence-range
		t.testEqual("find_end 6b", dstd::find_end(l.begin(), l.end(), l1.begin(), l1.begin()), l.end());
		// empty search-range and sequence-range
		t.testEqual("find_end 6c", dstd::find_end(l.begin(), l.begin(), l1.begin(), l1.begin()), l.begin());
	}
	{
		const int n = 10;
		dstd::list<int> l0, l1, l2, l3, l;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(i);
			l2.push_back(n);
			l3.push_back(0);
		}
		
		l.insert(l.end(), l1.begin(), l1.end());
		l.insert(l.end(), l2.begin(), l2.end());
		l.insert(l.end(), l1.begin(), l1.end());
		dstd::list<int>::iterator it2 = --(l.end());
		l.insert(l.end(), l2.begin(), l2.end());
		++it2;
		dstd::list<int>::iterator it1 = --(l.end());
		l.insert(l.end(), l1.begin(), l1.end());
		++it1;
		
		t.testEqual("find_end(p) 1", dstd::find_end(l.begin(), l.end(), l1.begin(), l1.end(), areEqual), it1);
		t.testEqual("find_end(p) 2", dstd::find_end(l.begin(), l.end(), l2.begin(), l2.end(), areEqual), it2);
		t.testEqual("find_end(p) 3", dstd::find_end(l.begin(), l.end(), l2.begin(), l2.end(), areEqual), it2);
		t.testEqual("find_end(p) 4", dstd::find_end(l.begin(), l.end(), l3.begin(), l3.end(), areEqual), l.end());
		
		// empty search-container
		t.testEqual("find_end(p) 5a", dstd::find_end(l0.begin(), l0.end(), l1.begin(), l1.end(), areEqual), l0.end());
		// empty sequence-container
		t.testEqual("find_end(p) 5b", dstd::find_end(l.begin(), l.end(), l0.begin(), l0.end(), areEqual), l.end());
		// empty search-container and sequence-container
		t.testEqual("find_end(p) 5c", dstd::find_end(l0.begin(), l0.end(), l0.begin(), l0.end(), areEqual), l0.end());
		
		// empty search-range
		t.testEqual("find_end(p) 6a", dstd::find_end(l.begin(), l.begin(), l1.begin(), l1.end(), areEqual), l.begin());
		// empty sequence-range
		t.testEqual("find_end(p) 6b", dstd::find_end(l.begin(), l.end(), l1.begin(), l1.begin(), areEqual), l.end());
		// empty search-range and sequence-range
		t.testEqual("find_end(p) 6c", dstd::find_end(l.begin(), l.begin(), l1.begin(), l1.begin(), areEqual), l.begin());
	}
	
	// find_first_of
	
	{
		dstd::vector<int> odds, evens, all, none;
		for(int i = 0; i < 10; ++i)
		{
			odds.push_back( (2*i) + 1 );
			evens.push_back( (2*i) );
			all.push_back( i );
		}
		
		t.testEqual("find_first_of 1", *(dstd::find_first_of(all.begin(),  all.end(),  odds.begin(),  odds.end()) ), odds.front());
		t.testEqual("find_first_of 2", *(dstd::find_first_of(all.begin(),  all.end(),  evens.begin(), evens.end())), evens.front());
		t.testEqual("find_first_of 3",   dstd::find_first_of(odds.begin(), odds.end(), evens.begin(), evens.end() ), odds.end());
		
		t.testEqual("find_first_of 4a",   dstd::find_first_of(odds.begin(), odds.end(), odds.begin(), odds.begin() ), odds.end());
		t.testEqual("find_first_of 4b",   dstd::find_first_of(odds.begin(), odds.begin(), odds.begin(), odds.end() ), odds.begin());
		t.testEqual("find_first_of 4c",   dstd::find_first_of(odds.begin(), odds.begin(), odds.begin(), odds.begin() ), odds.begin());
		
		t.testEqual("find_first_of 5a",   dstd::find_first_of(none.begin(), none.end(), odds.begin(), odds.end()), none.end());
		t.testEqual("find_first_of 5b",   dstd::find_first_of(odds.begin(), odds.end(), none.begin(), none.end()), odds.end());
		t.testEqual("find_first_of 5b",   dstd::find_first_of(none.begin(), none.end(), none.begin(), none.end()), none.end());
	}
	{
		dstd::vector<int> odds, evens, all, none;
		for(int i = 0; i < 10; ++i)
		{
			odds.push_back( (2*i) + 1 );
			evens.push_back( (2*i) );
			all.push_back( i );
		}
		
		t.testEqual("find_first_of(p) 1", *(dstd::find_first_of(all.begin(),  all.end(),  odds.begin(),  odds.end(), areEqual) ), odds.front());
		t.testEqual("find_first_of(p) 2", *(dstd::find_first_of(all.begin(),  all.end(),  evens.begin(), evens.end(), areEqual)), evens.front());
		t.testEqual("find_first_of(p) 3",   dstd::find_first_of(odds.begin(), odds.end(), evens.begin(), evens.end(), areEqual ), odds.end());
		
		t.testEqual("find_first_of(p) 4a",   dstd::find_first_of(odds.begin(), odds.end(), odds.begin(), odds.begin(), areEqual ), odds.end());
		t.testEqual("find_first_of(p) 4b",   dstd::find_first_of(odds.begin(), odds.begin(), odds.begin(), odds.end(), areEqual ), odds.begin());
		t.testEqual("find_first_of(p) 4c",   dstd::find_first_of(odds.begin(), odds.begin(), odds.begin(), odds.begin(), areEqual ), odds.begin());
		
		t.testEqual("find_first_of(p) 5a",   dstd::find_first_of(none.begin(), none.end(), odds.begin(), odds.end(), areEqual), none.end());
		t.testEqual("find_first_of(p) 5b",   dstd::find_first_of(odds.begin(), odds.end(), none.begin(), none.end(), areEqual), odds.end());
		t.testEqual("find_first_of(p) 5b",   dstd::find_first_of(none.begin(), none.end(), none.begin(), none.end(), areEqual), none.end());
	}
	
	// adjacent_find
	
	{
		dstd::vector<int> v1, v2, empty;
		const int x = 5;
		bool pass = true;
		for(int i = 0; i < 2*x; ++i)
		{
			v1.push_back(i);
			if( i == x ) v1.push_back(i);
			
			v2.push_back(i);
			if( dstd::adjacent_find(v2.begin(), v2.end()) != v2.end() ) pass = false;
		}
		t.testEqual("adjacent_find 1", pass, true);
		
		dstd::vector<int>::iterator it = dstd::adjacent_find(v1.begin(), v1.end());
		t.testEqual("adjacent_find 2a", *it, x);
		t.testEqual("adjacent_find 2b", *it, *(it+1));
		t.testEqual("adjacent_find 2c", (it - v1.begin()), x);
		
		t.testEqual("adjacent_find 3", dstd::adjacent_find(empty.begin(), empty.end()), empty.end());
	}
	{
		const int x = 5;
		dstd::vector<int> v1, v2, empty;
		for(int i = 0; i < 2*x; ++i)
		{
			v1.push_back(i);
			v2.push_back(i);
			if( i == x )
			{
				v1.push_back(i);
			}
		}
		dstd::vector<int>::iterator it = v1.begin() + x;
		
		t.testEqual("adjacent_find(p) 1a",   dstd::adjacent_find(v1.begin(), v1.end(), oddEqual),  it);
		t.testEqual("adjacent_find(p) 1b", *(dstd::adjacent_find(v1.begin(), v1.end(), oddEqual)), x);
		t.testEqual("adjacent_find(p) 2",    dstd::adjacent_find(v1.begin(), v1.begin(), oddEqual),  v1.begin());
		t.testEqual("adjacent_find(p) 3",    dstd::adjacent_find(empty.begin(), empty.end(), oddEqual), empty.end());
	}
	
	// search
	
	{
		const int n = 10;
		dstd::list<int> l0, l1, l2, l3, l;
		for(int i = 0; i < n; ++i)
		{
			l1.push_back(i);
			l2.push_back(n);
			l3.push_back(0);
		}
		
		l.insert(l.end(), l1.begin(), l1.end());
		dstd::list<int>::iterator it1 = l.begin();
		dstd::list<int>::iterator it2 = --(l.end());
		l.insert(l.end(), l2.begin(), l2.end());
		++it2;
		l.insert(l.end(), l1.begin(), l1.end());
		l.insert(l.end(), l2.begin(), l2.end());
		l.insert(l.end(), l1.begin(), l1.end());
		
		
		t.testEqual("search 1", dstd::search(l.begin(), l.end(), l1.begin(), l1.end()), it1);
		t.testEqual("search 2", dstd::search(l.begin(), l.end(), l2.begin(), l2.end()), it2);
		t.testEqual("search 3", dstd::search(l.begin(), l.end(), l2.begin(), l2.end()), it2);
		t.testEqual("search 4", dstd::search(l.begin(), l.end(), l3.begin(), l3.end()), l.end());
		
		// empty search-container
		t.testEqual("search 5a", dstd::search(l0.begin(), l0.end(), l1.begin(), l1.end()), l0.end());
		// empty sequence-container
		t.testEqual("search 5b", dstd::search(l.begin(), l.end(), l0.begin(), l0.end()), l.begin());
		// empty search-container and sequence-container
		t.testEqual("search 5c", dstd::search(l0.begin(), l0.end(), l0.begin(), l0.end()), l0.end());
		
		// empty search-range
		t.testEqual("search 6a", dstd::search(l.begin(), l.begin(), l1.begin(), l1.end()), l.begin());
		// empty sequence-range
		t.testEqual("search 6b", dstd::search(l.begin(), l.end(), l1.begin(), l1.begin()), l.begin());
		// empty search-range and sequence-range
		t.testEqual("search 6c", dstd::search(l.begin(), l.begin(), l1.begin(), l1.begin()), l.begin());
	}
	
	// search_n
	
	{
		dstd::vector<int> v0, v;
		const int x = 3;
		for(int i = 0; i < 4*x; ++i)
		{
			for(int j = 0; j < i; ++j)
			{
				v.push_back(i);
			}
		}
		
		dstd::vector<int>::iterator it = v.begin();
		while( *it != x )
		{
			++it;
		}
		
		t.testEqual("search_n 1", dstd::search_n(v.begin(), v.end(), x, x-1), v.end());
		t.testEqual("search_n 2", dstd::search_n(v.begin(), v.end(), x, x), it);
		t.testEqual("search_n 3", dstd::search_n(v.begin(), v.end(), x, x+1), it+x);
		t.testEqual("search_n 3", dstd::search_n(v0.begin(), v0.end(), 10, 3), v0.end());
	}
	
	
	//
	// Min/max
	
	{
		const int a = 4, b = 5, c = 6;
		
		t.testEqual("min 1a", dstd::min(a,b), a);
		t.testEqual("min 1b", dstd::min(b,a), a);
		t.testEqual("min 2a", dstd::min(a, b, oddLessThan), b);
		t.testEqual("min 2b", dstd::min(a, c, oddLessThan), a);
		t.testEqual("min 2c", dstd::min(b, c, oddLessThan), b);
		
		t.testEqual("max 1a", dstd::max(a,b), b);
		t.testEqual("max 1b", dstd::max(b,a), b);
		t.testEqual("max 2a", dstd::max(a, b, oddLessThan), a);
		t.testEqual("max 2b", dstd::max(a, c, oddLessThan), c);
		t.testEqual("max 2c", dstd::max(b, c, oddLessThan), c);
		
		dstd::vector<int> v;
		v.push_back(b);
		v.push_back(a);
		v.push_back(c);
		const dstd::vector<int> empty;
		
		t.testEqual("min_element 1", *(dstd::min_element(v.begin(), v.end())), a);
		t.testEqual("min_element 2", *(dstd::min_element(v.begin(), v.end(), oddLessThan)), b);
		t.testEqual("min_element 3", dstd::min_element(empty.begin(), empty.end()), empty.end());
		t.testEqual("min_element 4", dstd::min_element(empty.begin(), empty.end(), oddLessThan), empty.end());
		
		t.testEqual("max_element 1", *(dstd::max_element(v.begin(), v.end())), c);
		t.testEqual("max_element 2", *(dstd::max_element(v.begin(), v.end(), oddLessThan)), c);
		t.testEqual("max_element 3", dstd::max_element(empty.begin(), empty.end()), empty.end());
		t.testEqual("max_element 4", dstd::max_element(empty.begin(), empty.end(), oddLessThan), empty.end());
	}
	
	
	//
	// Heap
	
	{
		dstd::vector<int> v;
		v.push_back(4);
		v.push_back(1);
		v.push_back(9);
		v.push_back(-3);
		v.push_back(5);
		v.push_back(4);
		v.push_back(7);
		v.push_back(99);
		
		// make heap
		{
			dstd::vector<int>::iterator first = v.begin();
			dstd::vector<int>::iterator last = v.begin();
			bool pass = true;
			while( last != v.end() )
			{
				dstd::vector<int> v_heap(first, last);
				
				try
				{
					make_heap(v_heap.begin(), v_heap.end());
					if( ! is_heap(v_heap) )
					{
						pass = false;
						break;
					}
				}
				catch(...)
				{
					pass = false;
					break;
				}
				
				++last;
			}
			t.testEqual("make_heap", pass, true);
		}
		
		// sort_heap
		{
			dstd::vector<int>::iterator first = v.begin();
			dstd::vector<int>::iterator last = v.begin();
			bool pass = true;
			while( last != v.end() )
			{
				dstd::vector<int> v_heap(first, last);
				
				dstd::make_heap(v_heap.begin(), v_heap.end());
				dstd::vector<int> v_sorted(v_heap);
				dstd::sort_heap(v_sorted.begin(), v_sorted.end());
				bool pass = true;
				for(unsigned int i = 1; i < v_sorted.size(); ++i)
				{
					if( v_sorted[i-1] > v_sorted[i] )
					{
						pass = false;
						break;
					}
				}
				
				if( ! pass ) break;
				++last;
			}
			t.testEqual("sort_heap", pass, true);
		}
	}
	
	
	t.report();
	
	
	return 0;
}
