#ifndef DSTD_QUEUE_HXX
#define DSTD_QUEUE_HXX



#include "deque.hxx"



namespace dstd
{
	template <class T, class Container> class queue;
	template <class T, class Container> void swap(queue<T,Container> s1, queue<T,Container> s2);
}
template <class T, class Container> bool operator== (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);
template <class T, class Container> bool operator!= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);
template <class T, class Container> bool operator<  (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);
template <class T, class Container> bool operator<= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);
template <class T, class Container> bool operator>  (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);
template <class T, class Container> bool operator>= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs);



//
// Queue

template < class T, class Container = dstd::deque<T> >
class dstd::queue
{
	public:
		
		typedef T value_type;
		typedef Container container_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		
		explicit queue(const container_type& container = container_type())
			: c(container)
		{}
		
		queue(const queue& x)
			: c(x.c)
		{}
		
		~queue(){}
		
		queue& operator= (const queue& x)
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
		
		reference front()
		{
			return this->c.front();
		}
		
		const_reference front() const
		{
			return this->c.front();
		}
		
		reference back()
		{
			return this->c.back();
		}
		
		const_reference back() const
		{
			return this->c.back();
		}
		
		void push(const value_type& value)
		{
			this->c.push_back(value);
		}
		
		void pop()
		{
			this->c.pop_front();
		}
		
		void swap(queue& x)
		{
			dstd::swap(this->c, x.c);
		}
	
	
	private:
		
		container_type c;
	
	
	friend bool operator== <>(const queue& lhs, const queue& rhs);
	friend bool operator<  <>(const queue& lhs, const queue& rhs);
	friend bool operator>  <>(const queue& lhs, const queue& rhs);
};



//
// Relational Operators

template <class T, class Container>
bool operator== (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return (lhs.c == rhs.c);
}


template <class T, class Container>
bool operator!= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return ! (lhs == rhs);
}


template <class T, class Container>
bool operator<  (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return (lhs.c < rhs.c);
}


template <class T, class Container>
bool operator<= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return ! (lhs > rhs);
}


template <class T, class Container>
bool operator>  (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return (lhs.c > rhs.c);
}


template <class T, class Container>
bool operator>= (const dstd::queue<T,Container>& lhs, const dstd::queue<T,Container>& rhs)
{
	return ! (lhs < rhs);
}


//
// Swap

template <class T, class Container>
void dstd::swap(dstd::queue<T,Container> s1, dstd::queue<T,Container> s2)
{
	s1.swap(s2);
}



#endif
