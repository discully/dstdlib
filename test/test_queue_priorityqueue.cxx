#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/deque.hxx"
#include "../src/queue.hxx"
#include "../src/vector.hxx"



template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::priority_queue<T>& q)
{
	stream << "dstd::priority_queue(" << q.top() << ")";
	return stream;
}


template <class T, class C, class K>
bool operator==(dstd::priority_queue<T,C,K> lhs, dstd::priority_queue<T,C,K> rhs)
{
	if( lhs.size() != rhs.size() ) return false;
	
	while( ! lhs.empty() )
	{
		if( lhs.top() != rhs.top() ) return false;
		lhs.pop();
		rhs.pop();
	}
	
	return true;
}



template <class Container>
void testPriorityQueue(dstd::Tester& t)
{
	typedef dstd::priority_queue<int, Container> ipq;
	
	// constructors
	{
		const std::string test_name("priority_queue::priority_queue");
		try
		{
			Container empty;
			Container not_empty(4, 5);
			
			ipq* a = new ipq();
			ipq* b = new ipq(dstd::less<int>());
			ipq* c = new ipq(dstd::less<int>(), empty);
			ipq* d = new ipq(dstd::less<int>(), not_empty);
			delete(a);
			delete(b);
			delete(c);
			delete(d);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	// copy-constructor
	{
		Container x;
		x.push_back(5);
		x.push_back(3);
		x.push_back(7);
		x.push_back(1);
		x.push_back(9);
		
		const ipq a(dstd::less<int>(), x), empty;
		
		{
			ipq b(a);
			t.testEqual("priority_queue::priority_queue(priority_queue)", b, a);
		}
		
		{
			ipq b(empty);
			t.testEqual("priority_queue::priority_queue(priority_queue)", b, empty);
		}
	}
	
	// assignment
	{
		Container x;
		x.push_back(5);
		x.push_back(3);
		x.push_back(7);
		x.push_back(1);
		x.push_back(9);
		
		const ipq a(dstd::less<int>(), x), empty;
		ipq b;
		
		b = a;
		t.testEqual("priority_queue::operator= 1", b, a);
		
		b = empty;
		t.testEqual("priority_queue::operator= 2", b, empty);
	}
	
	// top
	{
		const int top = 144;
		Container x;
		x.push_back(top/2);
		x.push_back(top/3);
		x.push_back(top);
		x.push_back(top/5);
		x.push_back(top/4);
		
		const ipq a(dstd::less<int>(), x);
		
		t.testEqual("priority_queue::top", a.top(), top);
	}
	
	// empty
	{
		Container x;
		x.push_back(5);
		x.push_back(3);
		x.push_back(7);
		x.push_back(1);
		x.push_back(9);
		
		const ipq empty, a(dstd::less<int>(), x);
		
		t.testEqual("priority_queue::empty 1", empty.empty(), true);
		t.testEqual("priority_queue::empty 2", a.empty(), false);
	}
	
	// size
	{
		Container x;
		const unsigned int n = 5;
		for(unsigned int i = 0; i < n; ++i)
		{
			x.push_back(i);
		}
		
		const ipq empty, a(dstd::less<int>(), x);
		
		t.testEqual("priority_queue::size 1", empty.size(), uint(0));
		t.testEqual("priority_queue::size 2", a.size(), n);
	}
	
	// push
	{
		Container x;
		x.push_back(5);
		x.push_back(3);
		x.push_back(7);
		x.push_back(1);
		x.push_back(9);
		int max = 0;
		
		bool pass_top = true;
		bool pass_size = true;
		ipq a;
		unsigned int i = 0;
		for(typename Container::iterator it = x.begin(); it != x.end(); ++it)
		{
			max = dstd::max(max, *it);
			a.push(*it);
			++i;
			
			if( a.top() != max )
			{
				pass_top = false;
			}
			
			if( a.size() != i )
			{
				pass_size = false;
			}
		}
		
		t.testEqual("priority_queue::push 1a", pass_top, true);
		t.testEqual("priority_queue::push 1b", pass_size, true);
	}
	
	// pop
	{
		Container x;
		x.push_back(5);
		x.push_back(3);
		x.push_back(7);
		x.push_back(1);
		x.push_back(9);
		
		ipq a(dstd::less<int>(), x);
		
		bool pass_top = true;
		bool pass_size = true;
		while( ! x.empty() )
		{
			typename Container::iterator it = dstd::max_element(x.begin(), x.end());
			
			if( a.top() != *it )
			{
				pass_top = false;
			}
			
			x.erase(it);
			a.pop();
			
			if( a.size() != x.size() )
			{
				pass_size = false;
			}
		}
		
		t.testEqual("priority_queue::pop 1a", pass_top, true);
		t.testEqual("priority_queue::pop 1b", pass_size, true);
	}
	
	// swap
	{
		ipq a, b;
		a.push(5);
		a.push(3);
		a.push(7);
		a.push(1);
		a.push(9);
		b.push(12);
		b.push(11);
		b.push(10);
		
		const ipq empty, aa(a), bb(b);
		ipq none, q;
		
		// empty,empty
		q.swap(none);
		t.testEqual("priority_queue::swap 1a", q, empty);
		t.testEqual("priority_queue::swap 1b", none, empty);
		
		// empty,filled
		q.swap(a);
		t.testEqual("priority_queue::swap 1a", q, aa);
		t.testEqual("priority_queue::swap 1b", a, empty);
		
		// filled,empty
		q.swap(none);
		t.testEqual("priority_queue::swap 1a", q, empty);
		t.testEqual("priority_queue::swap 1b", none, aa);
		
		// filled,filled
		q = aa;
		q.swap(b);
		t.testEqual("priority_queue::swap 1a", q, bb);
		t.testEqual("priority_queue::swap 1b", b, aa);
	}
	
	// dstd::swap
	{
		ipq a, b;
		a.push(5);
		a.push(3);
		a.push(7);
		a.push(1);
		a.push(9);
		b.push(12);
		b.push(11);
		b.push(10);
		
		const ipq empty, aa(a), bb(b);
		ipq none, q;
		
		// empty,empty
		dstd::swap(q, none);
		t.testEqual("priority_queue::swap 1a", q, empty);
		t.testEqual("priority_queue::swap 1b", none, empty);
		
		// empty,filled
		dstd::swap(q, a);
		t.testEqual("priority_queue::swap 1a", q, aa);
		t.testEqual("priority_queue::swap 1b", a, empty);
		
		// filled,empty
		dstd::swap(q, none);
		t.testEqual("priority_queue::swap 1a", q, empty);
		t.testEqual("priority_queue::swap 1b", none, aa);
		
		// filled,filled
		q = aa;
		dstd::swap(q, b);
		t.testEqual("priority_queue::swap 1a", q, bb);
		t.testEqual("priority_queue::swap 1b", b, aa);
	}
}



int main()
{
	dstd::Tester t_deque("<priority_queue> priority_queue [deque]");
	dstd::Tester t_vector( "<priority_queue> priority_queue [vector]");
	
	testPriorityQueue< dstd::deque<int> >( t_deque );
	testPriorityQueue< dstd::vector<int>   >( t_vector );
	
	t_deque.report();
	t_vector.report();
	
	return 0;
}
