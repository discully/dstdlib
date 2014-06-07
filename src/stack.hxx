#ifndef DSTD_STACK_HXX
#define DSTD_STACK_HXX



#include "deque.hxx"



namespace dstd
{
	template <class T, class Container> class stack;
	template <class T, class Container> void swap(stack<T,Container> s1, stack<T,Container> s2);
}
template <class T, class Container> bool operator== (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);
template <class T, class Container> bool operator!= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);
template <class T, class Container> bool operator<  (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);
template <class T, class Container> bool operator<= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);
template <class T, class Container> bool operator>  (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);
template <class T, class Container> bool operator>= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs);



//
// Stack

template < class T, class Container = dstd::deque<T> >
class dstd::stack
{
	public:
		
		typedef T value_type;
		typedef Container container_type;
		typedef size_t size_type;
		
		explicit stack(const container_type& container = container_type())
			: c(container)
		{}
		
		~stack(){}
		
		stack& operator= (const stack& x)
		{
			this->c = x.c;
			return *this;
		}
		
		bool empty() const
		{
			return this->c.empty();
		}
		
		size_type size() const
		{
			return this->c.size();
		}
		
		value_type& top()
		{
			return this->c.back();
		}
		
		const value_type& top() const
		{
			return this->c.back();
		}
		
		void push(const value_type& value)
		{
			this->c.push_back(value);
		}
		
		void pop()
		{
			this->c.pop_back();
		}
		
		void swap(stack& x)
		{
			dstd::swap(this->c, x.c);
		}
	
	
	private:
		
		container_type c;
	
	
	friend bool operator== <>(const stack& lhs, const stack& rhs);
	friend bool operator<  <>(const stack& lhs, const stack& rhs);
	friend bool operator>  <>(const stack& lhs, const stack& rhs);
};



//
// Stack: Relational Operators

template <class T, class Container>
bool operator== (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return (lhs.c == rhs.c);
}


template <class T, class Container>
bool operator!= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return ! (lhs == rhs);
}


template <class T, class Container>
bool operator<  (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return (lhs.c < rhs.c);
}


template <class T, class Container>
bool operator<= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return ! (lhs > rhs);
}


template <class T, class Container>
bool operator>  (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return (lhs.c > rhs.c);
}


template <class T, class Container>
bool operator>= (const dstd::stack<T,Container>& lhs, const dstd::stack<T,Container>& rhs)
{
	return ! (lhs < rhs);
}


//
// Stack: Swap

template <class T, class Container>
void dstd::swap(dstd::stack<T,Container> s1, dstd::stack<T,Container> s2)
{
	s1.swap(s2);
}



#endif
