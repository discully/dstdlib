#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/deque.hxx"
#include "../src/list.hxx"
#include "../src/queue.hxx"
#include "../src/vector.hxx"



template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::queue<T>& s)
{
	stream << "dstd::queue(" << s.top() << ")";
	return stream;
}



template <class Container>
void testQueue(dstd::Tester& t)
{
	typedef dstd::queue<int, Container> iqueue;
	
	
	// constructor
	
	{
		const std::string test_name = "queue::queue()";
		try
		{
			iqueue();
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	// copy
	
	{
		iqueue is1;
		is1.push(3);
		const iqueue is2(is1);
		t.testEqual("queue::queue(stack)", is1, is2);
	}
	
	
	
	// operator=
	
	{
		iqueue is1, is2;
		is1.push(3);
		is2 = is1;
		t.testEqual("queue::operator=", is1, is2);
	}
	
	
	// front
	
	{
		const int xf = 1;
		const int xb = 2;
		iqueue is;
		is.push(xf);
		is.push(xf+xb);
		is.push(xb);
		t.testEqual("queue::front", is.front(), xf);
		t.testEqual("queue::back",  is.back(), xb);
	}
	
	
	// empty
	
	{
		iqueue is;
		
		t.testEqual("queue::empty 1", is.empty(), true);
		
		is.push(3);
		t.testEqual("queue::empty 2", is.empty(), false);
	}
	
	
	// size
	
	{
		bool pass = true;
		iqueue is;
		for(size_t i = 0; i < 30; ++i)
		{
			if( is.size() != i )
			{
				pass = false;
				break;
			}
			is.push(i);
		}
		t.testEqual("queue::size", pass, true);
	}
	
	
	// push
	
	{
		bool pass = true;
		iqueue is;
		for(int i = 0; i < 30; ++i)
		{
			is.push(i);
			if( is.back() != i )
			{
				pass = false;
				break;
			}
		}
		t.testEqual("queue::push", pass, true);
	}
	
	
	// pop
	
	{
		const int n = 30;
		bool pass = true;
		iqueue is;
		for(int i = 0; i < n; ++i)
		{
			is.push(i);
		}
		for(int i = 0; i < n; ++i)
		{
			if( is.front() != i )
			{
				pass = false;
				break;
			}
			if( is.size() != size_t(n-i) )
			{
				pass = false;
				break;
			}
			is.pop();
		}
		t.testEqual("queue::pop", pass, true);
	}
}



int main()
{
	dstd::Tester t_deque("<queue> queue [deque]");
	dstd::Tester t_list( "<queue> queue [list]");
	
	testQueue< dstd::deque<int> >( t_deque );
	testQueue< dstd::list<int>   >( t_list );
	
	t_deque.report();
	t_list.report();
	
	return 0;
}
