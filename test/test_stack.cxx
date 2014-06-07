#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/deque.hxx"
#include "../src/list.hxx"
#include "../src/stack.hxx"
#include "../src/vector.hxx"



template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::stack<T>& s)
{
	stream << "dstd::stack(" << s.top() << ")";
	return stream;
}



template <class Container>
void testStack(dstd::Tester& t)
{
	typedef dstd::stack<int, Container> istack;
	
	
	// constructor
	
	{
		const std::string test_name = "stack::stack()";
		try
		{
			istack();
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	// copy
	
	{
		istack is1;
		is1.push(3);
		const istack is2(is1);
		t.testEqual("stack::stack(stack)", is1, is2);
	}
	
	
	
	// operator=
	
	{
		istack is1, is2;
		is1.push(3);
		is2 = is1;
		t.testEqual("stack::operator=", is1, is2);
	}
	
	
	// top
	
	{
		const int x = 3;
		istack is;
		is.push(x+2);
		is.push(x+1);
		is.push(x);
		t.testEqual("stack::top", is.top(), x);
	}
	
	
	// empty
	
	{
		istack is;
		
		t.testEqual("stack::empty 1", is.empty(), true);
		
		is.push(3);
		t.testEqual("stack::empty 2", is.empty(), false);
	}
	
	
	// size
	
	{
		bool pass = true;
		istack is;
		for(size_t i = 0; i < 30; ++i)
		{
			if( is.size() != i )
			{
				pass = false;
				break;
			}
			is.push(i);
		}
		t.testEqual("stack::size", pass, true);
	}
	
	
	// push
	
	{
		bool pass = true;
		istack is;
		for(int i = 0; i < 30; ++i)
		{
			is.push(i);
			if( is.top() != i )
			{
				pass = false;
				break;
			}
		}
		t.testEqual("stack::push", pass, true);
	}
	
	
	// pop
	
	{
		const int n = 30;
		bool pass = true;
		istack is;
		for(int i = 0; i < n; ++i)
		{
			is.push(i);
		}
		for(int i = n-1; i > 0; --i)
		{
			if( is.top() != i )
			{
				pass = false;
				break;
			}
			is.pop();
			if( is.size() != size_t(i) )
			{
				pass = false;
				break;
			}
		}
		t.testEqual("stack::pop", pass, true);
	}
}



int main()
{
	dstd::Tester t_deque("<stack> [deque]");
	dstd::Tester t_list("<stack> [list]");
	dstd::Tester t_vector("<stack> [vector]");
	
	testStack< dstd::deque<int> >( t_deque );
	testStack< dstd::list<int>   >( t_list );
	testStack< dstd::vector<int> >( t_vector );
	
	t_deque.report();
	t_list.report();
	t_vector.report();
	
	return 0;
}
