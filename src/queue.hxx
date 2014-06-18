#ifndef DSTD_QUEUE_HXX
#define DSTD_QUEUE_HXX



#include "algorithm.hxx"
#include "functional.hxx"
#include "deque.hxx"
#include "utility.hxx"
#include "vector.hxx"



namespace dstd
{
	template <class T, class Container> class queue;
	template <class T, class Container, class Compare> class priority_queue;
	template <class T, class Container> void swap(queue<T,Container> a, queue<T,Container> b);
	template <class T, class Container, class Compare> void swap(priority_queue<T,Container,Compare>& a, priority_queue<T,Container,Compare>& b);
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
// Priority Queue

template < class T,	class Container = dstd::vector<T>, class Compare = dstd::less< typename Container::value_type > >
class dstd::priority_queue
{
	public:
		
		typedef Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		
		explicit priority_queue(const Compare& compare = Compare(), const container_type& container = container_type())
			: c(container), comp(compare)
		{
			dstd::make_heap(this->c.begin(), this->c.end());
		}
		
		priority_queue(const priority_queue& x)
			: c(x.c), comp(x.comp)
		{}
		
		~priority_queue(){}
		
		priority_queue& operator= (const priority_queue& x)
		{
			this->c = x.c;
			this->comp = x.comp;
			return *this;
		}
		
		const_reference top() const
		{
			return this->c.front();
		}
		
		bool empty() const
		{
			return this->c.empty();
		}
		
		size_type size() const
		{
			return this->c.size();
		}
		
		void push(const value_type& value)
		{
			this->c.push_back(value);
			dstd::push_heap(this->c.begin(), this->c.end(), this->comp);
		}
		
		void pop()
		{
			dstd::pop_heap(this->c.begin(), this->c.end(), this->comp);
			this->c.pop_back();
		}
		
		void swap(priority_queue& x)
		{
			dstd::swap(this->c, x.c);
			dstd::swap(this->comp, x.comp);
		}
	
	
	private:
		
		container_type c;
		Compare comp;
};



//
// Queue Relational Operators

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
void dstd::swap(dstd::queue<T,Container>& a, dstd::queue<T,Container>& b)
{
	a.swap(b);
}

template <class T, class Container, class Compare>
void dstd::swap(dstd::priority_queue<T, Container, Compare>& a, dstd::priority_queue<T, Container, Compare>& b)
{
	a.swap(b);
}



#endif
