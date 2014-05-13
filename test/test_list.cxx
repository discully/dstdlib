#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/list.hxx"


//
// Functions required for tester


template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::list<T>& l)
{
	stream << "dstd::list(";
	for(typename dstd::list<T>::const_iterator it = l.begin(); it != l.end(); ++it)
	{
		stream << *it << ", ";
	}
	stream << ")";
	return stream;
}


template <class T>
std::ostream& operator<<(std::ostream& stream, const typename dstd::list<T>::iterator& it)
{
	stream << "dstd::list::iterator(" << *it << ")";
	return stream;
}


template <class T>
std::ostream& operator<<(std::ostream& stream, const typename dstd::list<T>::const_iterator& it)
{
	stream << "dstd::list::const_iterator(" << *it << ")";
	return stream;
}


template <class T>
bool operator==(const dstd::list<T>& l1, const dstd::list<T>& l2)
{
	if( l1.size() != l2.size() ) return false;
	typename dstd::list<T>::const_iterator it1 = l1.begin();
	typename dstd::list<T>::const_iterator it2 = l2.begin();
	for(; it1 != l1.end(); ++it1, ++it2)
	{
		if( *it1 != *it2 ) return false;
	}
	return true;
}


template <class T>
bool operator!=(const dstd::list<T>& l1, const dstd::list<T>& l2)
{
	return ! (l1 == l2);
}


//
// Functions passed to test methods


bool mergeTest(int a, int b) // sort such that odd numbers come before even, then smaller numbers before larger
{
	if( a % 2  == b % 2 )
	{
		return a < b;
	}
	else if( a % 2 == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool uniqueTest(int a, int b)
{
	return ( a == b );
}


bool removeIfTest(int a)
{
	return (a % 2 != 0); // remove odd numbers
}


int main()
{
	dstd::Tester t("<list>");
	
	
	dstd::list<double> zero;
	const dstd::list<double> empty;
	
	
	//
	// Constructors
	
	
	{
		const size_t n = 5;
		const double val = 1.1;
		
		dstd::list<double> l1;
		dstd::list<double> l2;
		for(unsigned int i = 0; i < n; ++i)
		{
			l1.push_back( val );
			l2.push_back( i * val );
		}
		
		{
			dstd::list<double> a(n, val);
			t.testEqual("list::list(n, val)", a, l1);
		}
		
		{
			dstd::list<double> b(l2.begin(), l2.end());
			t.testEqual("list::list(first, last)", b, l2);
		}
		
		{
			dstd::list<double> c(l2);
			t.testEqual("list::list(list)", c, l2);
		}
	}
	
	
	{
		dstd::list<int> l1;
		for(int i = 0; i < 5; ++i)
		{
			l1.push_back(i);
		}
		dstd::list<int> l2(l1);
		t.testEqual("list::list(list)", l1, l2);
	}
	
	
	{
		dstd::list<int> l1;
		for(int i = 0; i < 5; ++i)
		{
			l1.push_back(i);
		}
		dstd::list<int> l2 = l1;
		t.testEqual("list::operator=(list)", l1, l2);
	}
	
	
	{
		dstd::list<int> l;
		for(int i = 0; i < 5; ++i)
		{
			l.push_back(i);
		}
		dstd::list<int>::iterator b = l.begin();
		t.testEqual("list::begin 1", *b, 0 );
		--b;
		dstd::list<int>::iterator e = l.end();
		t.testEqual("list::begin 2", b, e);
	}
	
	
	//
	// Size
	
	
	t.testEqual("list::size empty", empty.size(), size_t(0));
	
	
	{
		const size_t n1 = 1;
		const size_t n2 = 2;
		const size_t n3 = 10;
		dstd::list<double> l1(n1, 99.9);
		dstd::list<double> l2(n2, 99.9);
		dstd::list<double> l3(n3, 99.9);
		t.testEqual("list::size 1", l1.size(), n1);
		t.testEqual("list::size 2", l2.size(), n2);
		t.testEqual("list::size 3", l3.size(), n3);
	}
	
	
	//
	// Push back
	
	
	{
		const double v = 6.6;
		dstd::list<double> l;
		l.push_back(v);
		t.testEqual("list::push_back 1a", l.size(), size_t(1));
		t.testEqual("list::push_back 1b", l.back(), v);
		t.testNotEqual("list::push_back 1c", l.begin(), l.end());
		t.testEqual("list::push_back 1d", l.front(), l.back());
	}
	
	
	{
		const size_t n = 3;
		const double v = 6.6;
		dstd::list<double> l(n,7.7);
		l.push_back(v);
		t.testEqual("list::push_back 2a", l.size(), n+1);
		t.testEqual("list::push_back 2b", l.back(), v);
	}
	
	
	//
	// Pop Back
	
	
	{
		const std::string test_name("list::pop_back");
		try
		{
			zero.pop_back();
			t.registerTestFailed(test_name, "no exception thrown");
		}
		catch( dstd::out_of_range e )
		{
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "wrong exception thrown");
		}
	}
	
	
	{
		size_t n = 5;
		dstd::list<int> l1;
		dstd::list<int> l2;
		for(unsigned int i = 0; i < n; ++i)
		{
			l1.push_back(i);
			if( i < n-1 ) l2.push_back(i);
		}
		l1.pop_back();
		t.testEqual("list::pop_back", l1, l2);
		t.testEqual("list::pop_back size", l1.size(), n-1);
		t.testEqual("list::pop_back back", l1.back(), int(n-2) );
	}
	
	
	//
	// Insert (position, val)
	
	
	{
		const double v = 3.14;
		
		dstd::list<double> l1;
		dstd::list<double>::iterator it1 = l1.insert(l1.begin(), v);
		
		dstd::list<double> l2;
		dstd::list<double>::iterator it2 = l2.insert(l2.end(), v);
		
		t.testEqual("list::insert(pos, val)", l1, l2);
		t.testEqual("list::insert(pos, val) return 1", *it1, v);
		t.testEqual("list::insert(pos, val) return 2", *it2, v);
	}
	
	
	//
	// Insert (position, n, val)
	
	
	zero.insert(zero.begin(), 0, 99.9);
	t.testEqual("list::insert(pos, n, val)", zero, empty);
	
	
	{
		const double v = 3.14;
		const unsigned int n = 4;
		
		dstd::list<double> l1, l2;
		
		for(unsigned int i = 0; i < n; ++i)
		{
			l1.push_back(v);
		}
		
		l2.insert(l2.begin(), n, v);
		
		t.testEqual("list::insert(pos, n, val)", l2, l1);
	}
	
	
	{
		const double a = 3.14;
		const double b = 66.6;
		const unsigned int n = 4;
		
		dstd::list<double> l1, l2;
		
		l1.push_back(a);
		l2.push_back(a);
		for(unsigned int i = 0; i < n; ++i)
		{
			l1.push_back(b);
		}
		l1.push_back(a);
		l2.push_back(a);
		
		l2.insert(++l2.begin(), n, b);
		
		t.testEqual("list::insert(pos, n, val)", l2, l1);
	}
	
	
	//
	// Insert (position, first, last)
	
	
	{
		const double a = 6.6;
		const double b = 1.0;
		dstd::list<double> l1(2, a);
		dstd::list<double> l2;
		dstd::list<double> l3;
		dstd::list<double> l4;
		dstd::list<double> l5;
		
		l3.push_back(a);
		l4.push_back(a);
		for(int i = 0; i < 3; ++i)
		{
			l2.push_back(i*b);
			l3.push_back(i*b);
			l4.push_back(i*b);
		}
		l3.push_back(a);
		
		// l1 = [a, a]
		// l2 = [0b, 1b, 2b]
		// l3 = [a, 0b, 1b, 2b, a]
		// l4 = [a, 0b, 1b, 2b]
		// l5 = []
		
		l1.insert( ++l1.begin(), l2.begin(), l2.end() );
		t.testEqual("list::insert(pos, first, last) middle", l1, l3);
		
		l2.insert(l2.begin(), l1.begin(), ++l1.begin());
		t.testEqual("list::insert(pos, first, last) beginning", l2, l4);
		
		l2.insert(l2.end(), l1.begin(), ++l1.begin());
		t.testEqual("list::insert(pos, first, last) end", l2, l3);
		
		l5.insert(l5.begin(), l3.begin(), l3.end());
		t.testEqual("list::insert(pos, first, last) empty", l5, l3);
	}
	
	
	//
	// Erase
	
	
	{
		const std::string test_name("list::erase(pos) [empty]");
		try
		{
			zero.erase(zero.begin());
			t.registerTestFailed(test_name, "no exception was thrown");
		}
		catch( dstd::out_of_range e )
		{
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "the wrong exception was thrown");
		}
	}
	
	
	{
		dstd::list<double> l(1, 1.1);
		dstd::list<double>::iterator it = l.erase(l.begin());
		t.testEqual("list::erase(pos) 1", l, empty);
		t.testEqual("list::erase(pos) 1 return", it, l.end());
	}
	
	
	{
		const double v = 1.1;
		const int x = 5;
		
		dstd::list<double> l1;
		dstd::list<double> l2;
		
		for(int i = 0; i < 2*x; ++i)
		{
			l1.push_back(i*v);
			if( i != x )l2.push_back(i*v);
		}
		
		dstd::list<double>::iterator it = l1.begin();
		while( *it != x*v )
		{
			++it;
		}
		
		dstd::list<double>::iterator rit = l1.erase(it);
		
		t.testEqual("list::erase(pos) 2", l1, l2);
		t.testEqual("list::erase(pos) 2 return", *rit, ((x+1)*v));
	}
	
	
	{
		dstd::list<double>::iterator it = zero.erase(zero.begin(), zero.end());
		t.testEqual("list::erase(first, last) [empty]", zero, empty);
		t.testEqual("list::erase(first, last) [empty] return", it, zero.end());
	}
	
	
	{
		dstd::list<double> l(1, 1.1);
		dstd::list<double>::iterator it = l.erase(l.begin(), l.end());
		t.testEqual("list::erase(first, last) 1", l, empty);
		t.testEqual("list::erase(first, last) 1 return", it, l.end());
	}
	
	
	{
		const double v = 1.1;
		const int x = 5;
		
		dstd::list<double> l1, l2, l3;
		
		for(int i = 0; i < 2*x; ++i)
		{
			l1.push_back(i*v);
			if( i != x ) l2.push_back(i*v);
			if( i < x  ) l3.push_back(i*v);
		}
		
		dstd::list<double>::iterator it = l1.begin();
		while( *it != x*v )
		{
			++it;
		}
		dstd::list<double>::iterator it2 = it;
		++it2;
		
		dstd::list<double>::iterator rit1 = l1.erase(it, it2);
		t.testEqual("list::erase(first, last) 2", l1, l2);
		t.testEqual("list::erase(first, last) 2 return", *rit1, (x+1)*v);
		
		l1.erase(it2, l1.end());
		t.testEqual("list::erase(first, last) 3", l1, l3);
	}
	
	
	//
	// Swap
	
	
	{
		dstd::list<double> l(5, 99.9);
		zero.swap(l);
		t.testNotEqual("list::swap [empty] 1", zero, empty);
		zero.swap(l);
		t.testEqual("list::swap [empty] 2", zero, empty);
		
		dstd::list<double> zero_2;
		zero_2.swap(zero);
		t.testEqual("list::swap [empty] 3", zero, empty);
		t.testEqual("list::swap [empty] 4", zero_2, empty);
	}
	
	
	{
		dstd::list<double> a(5, 81.2);
		const dstd::list<double> aa(a);
		dstd::list<double> b(19, 14.1);
		const dstd::list<double> bb(b);
		
		a.swap(b);
		t.testEqual("list::swap 1", a, bb);
		t.testEqual("list::swap 2", b, aa);
		
		b.swap(a);
		t.testEqual("list::swap 3", a, aa);
		t.testEqual("list::swap 4", b, bb);
	}
	
	
	//
	// Resize
	
	
	zero.resize(0, 9.9);
	t.testEqual("list::resize [empty]", zero, empty);
	
	
	{
		size_t n = 10;
		int val = 99;
		dstd::list<int> l(n, val);
		
		// get bigger
		{
			dstd::list<int> l_smaller(n/2, val);
			l_smaller.resize(n, val);
			t.testEqual("list::resize enlarge", l_smaller, l);
		}
		
		// get smaller
		{
			dstd::list<int> l_bigger(2*n, val);
			l_bigger.resize(n, val);
			t.testEqual("list::resize shrink", l_bigger, l);
		}
		
		// stay the same
		{
			dstd::list<int> l_same(n, val);
			l_same.resize(n, val);
			t.testEqual("list::resize same", l_same, l);
		}
	}
	
	
	//
	// Clear
	
	
	zero.clear();
	t.testEqual("list::clear [empty]", zero, empty);
	
	
	{
		dstd::list<double> l(5, 99.9);
		l.clear();
		t.testEqual("list::clear", l, empty);
	}
	
	
	//
	// Splice
	
	
	//////////////////////////////////////////////////
	
	
	//
	// Remove
	
	
	zero.remove(3);
	t.testEqual("list::remove [empty]", zero, empty);
	
	
	{
		dstd::list<int> l1, l2;
		for(int i = 0; i < 3; ++i)
		{
			l1.push_back(i);
			if( i == 3 ) l1.push_back(i);
			if( i != 3 ) l2.push_back(i);
		}
		l1.remove(3);
		t.testEqual("list::remove", l1, l2);
	}
	
	
	zero.remove_if(removeIfTest);
	t.testEqual("list::remove_if [empty]", zero, empty);
	
	
	{
		int n = 5;
		
		dstd::list<int> l1;
		for(int i = 0; i < 2*n; ++i)
		{
			l1.push_back(i);
		}
		
		dstd::list<int> l2;
		for(int i = 0; i < n; ++i)
		{
			l2.push_back(2*i);
		}
		
		l1.remove_if(removeIfTest);
		
		t.testEqual("list::remove_if", l1, l2);
	}
	
	
	//
	// Unique
	
	
	zero.unique();
	t.testEqual("list::unique [empty]", zero, empty);
	
	
	{
		dstd::list<int> l1;
		dstd::list<int> l2;
		for(int i = 0; i < 5; ++i)
		{
			l1.push_back(i);
			for(int j = 0; j < i+1; ++j)
			{
				l2.push_back(i);
			}
		}
		
		l2.unique();
		
		t.testEqual("list::unique", l2, l1);
	}
	
	
	zero.unique(uniqueTest);
	t.testEqual("list::unique(binary_pred) [empty]", zero, empty);
	
	
	{
		dstd::list<int> l1;
		dstd::list<int> l2;
		for(int i = 0; i < 5; ++i)
		{
			l1.push_back(i);
			for(int j = 0; j < i+1; ++j)
			{
				l2.push_back(i);
			}
		}
		
		l2.unique(uniqueTest);
		
		t.testEqual("list::unique(binary_pred)", l2, l1);
	}
	
	
	//
	// Merge
	
	
	{
		dstd::list<double> zero_b(zero);
		zero.merge(zero_b);
		t.testEqual("list::merge(list) [empty]", zero, empty);
	}
	
	
	{
		dstd::list<int> odds;
		dstd::list<int> evens;
		dstd::list<int> all;
		for(int i = 0; i < 10; ++i)
		{
			if( i%2 == 0 )
			{
				evens.push_back(i);
			}
			else
			{
				odds.push_back(i);
			}
			all.push_back(i);
		}
		evens.merge(odds);
		
		t.testEqual("list::merge(list) 1", evens, all);
		t.testEqual("list::merge(list) 2", odds.size(), size_t(0));
	}
	
	
	{
		dstd::list<double> zero_b(zero);
		zero.merge(zero_b, mergeTest);
		t.testEqual("list::merge(list, comp) [empty]", zero, empty);
	}
	
	
	{
		dstd::list<int> odds;
		dstd::list<int> evens;
		for(int i = 0; i < 10; ++i)
		{
			if( i%2 == 0 )
			{
				evens.push_back(i);
			}
			else
			{
				odds.push_back(i);
			}
		}
		dstd::list<int> all(odds);
		all.insert(all.end(), evens.begin(), evens.end());
		
		evens.merge(odds, mergeTest);
		
		t.testEqual("list::merge(list, comp) 1", evens, all);
		t.testEqual("list::merge(list, comp) 2", odds.size(), size_t(0));
	}
	
	
	//
	// Reverse
	
	zero.reverse();
	t.testEqual("list::reverse [empty]", zero, empty);
	
	
	{
		dstd::list<int> fwd;
		dstd::list<int> bwd;
		const unsigned int n = 5;
		for(unsigned int i = 0; i < n; ++i)
		{
			fwd.push_back(i);
			bwd.push_back(n-1-i);
		}
		bwd.reverse();
		t.testEqual("list::reverse", fwd, bwd);
	}
	
	
	t.report();
	return 0;
}