#ifndef DSTD_VECTOR_HXX
#define DSTD_VECTOR_HXX

#include <algorithm>
#include <cassert>
#include <limits>
#include <cstddef>

#include "dstd.hxx"
#include "algorithm.hxx"
#include "memory.hxx"
#include "iterator.hxx"



namespace dstd
{
	template <class T, class Allocator > class vector;
	template <class T, class Allocator > void swap(vector<T, Allocator>& v1, vector<T, Allocator>& v2);
}



//
// Vector

template < class T, class Allocator = dstd::allocator<T> >
class dstd::vector
{
	public:
	
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef size_t size_type;
	typedef ptrdiff_t dfference_type;
	class iterator;
	class const_iterator;
	typedef dstd::reverse_iterator< iterator > reverse_iterator;
	typedef dstd::reverse_iterator< const_iterator > const_reverse_iterator;
	
	
	//
	// Constructors
	
	explicit vector()
		: n_data(0), n_memory(0), p(0)
	{}
	
	
	explicit vector(unsigned int n, const T& value = T())
		:n_data(0), n_memory(0), p(0)
	{
		this->assign(n, value);
	}
	
	
	vector(iterator first, iterator last)
		: n_data(0), n_memory(0), p(0)
	{
		this->assign(first, last);
	}
	
	
	vector(const vector& v)
		: n_data(0), n_memory(0), p(0)
	{
		this->assign<const_iterator>(v.begin(), v.end());
	}
	
	
	//
	// Destructor
	
	
	~vector()
	{
		this->clear();
		if( this->p != 0 ) this->a.deallocate(this->p, this->n_memory);
	}
	
	
	//
	// Assignment
	
	
	vector& operator= (const vector& v)
	{
		this->assign(v.begin(), v.end());
		return *this;
	}
	
	
	//
	// Iterators
	
	
	iterator begin()
	{
		return iterator( this->p );
	}
	
	
	const_iterator begin() const
	{
		return const_iterator( this->p );
	}


	iterator end()
	{
		return iterator( this->p + this->size() );
	}
	
	
	const_iterator end() const
	{
		return const_iterator( this->p + this->size() );
	}


	reverse_iterator rbegin()
	{
		return reverse_iterator( this->end() );
	}
	
	
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator( this->end() );
	}
	
	
	reverse_iterator rend()
	{
		return reverse_iterator( this->begin() );
	}
	
	
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( this->begin() );
	}
	
	
	//
	// Capacity
	
	
	size_t size() const
	{
		return this->n_data;
	}
	
	
	size_t max_size() const
	{
		return std::numeric_limits<size_t>::max();
	}
	
	
	void resize(size_t n, const T& value = T())
	{
		if( n < this->size() )
		{
			iterator first = this->begin() + n;
			this->erase( first, this->end() );
		}
		else if( n > this->size() )
		{
			size_t n_add = n - this->size();
			for(unsigned int i = 0; i != n_add; ++i)
			{
				this->push_back(value);
			}
		}
	}
	
	
	size_t capacity() const
	{
		return this->n_memory;
	}
	
	
	bool empty() const
	{
		return ( this->size() == 0 );
	}
	
	
	void reserve(size_t n)
	{
		// Check if this request is necessary and valid
		if( n <= this->capacity() ) return;
		if( n > this->max_size() ) throw dstd::length_error();
		
		// Determine how much memory to request
		unsigned int n_request = std::max<unsigned int>( this->capacity(), 2 );
		while( n_request < n )
		{
			n_request = std::min<unsigned int>( this->max_size(), n_request*2 );
		}
		
		assert( n_request >= n );
		
		// Request new memory, and copy existing values to new memory
		T* new_p = this->a.allocate( n_request );
		for(unsigned int i = 0; i != this->size(); ++i)
		{
			this->a.construct( &(new_p[i]), (*this)[i] );
		}
		
		// Move the vector to the new memory
		unsigned int old_n_memory = this->n_memory;
		T* old_p = this->p;
		this->p = new_p;
		this->n_memory = n_request;
		
		if( old_p != 0 )
		{
			// Clean up the old memory
			for(unsigned int i = 0; i != this->size(); ++i)
			{
				this->a.destroy( old_p + i );
			}
			this->a.deallocate(old_p, old_n_memory);
		}
	}
	
	
	//
	// Element access
	
	
	reference operator[](size_t i)
	{
		return this->p[i];
	}
	
	
	const_reference operator[](size_t i) const
	{
		return this->p[i];
	}
	
	
	reference at(size_t i)
	{
		if( i >= this->size() )
		{
			throw dstd::out_of_range();
		}
		return (this->p)[i];
	}
	
	
	const_reference at(size_t i) const
	{
		if( i >= this->size() )
		{
			throw dstd::out_of_range();
		}
		return (this->p)[i];
	}
	
	
	reference front()
	{
		return this->p[0];
	}
	
	
	const_reference front() const
	{
		return this->p[0];
	}


	reference back()
	{
		if( this->size() > 0 )
		{
			return *(this->end() - 1);
		}
		else
		{
			return this->front();
		}
	}
	
	
	const_reference back() const
	{
		if( this->empty() )
		{
			return this->front();
		}
		else
		{
			return *(this->end() - 1);
		}
	}
	
	
	//
	// Modifiers
	
	
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		//this->erase(this->begin(), this->end());
		//this->reserve( last - first );
		//InputIterator it_from = first;
		//while( it_from < last )
		//{
		//	this->push_back( *it_from );
		//	++it_from;
		//}
		this->clear();
		this->insert(this->begin(), first, last);
	}
	
	
	void assign(size_t n, const value_type& value)
	{
		//this->erase(this->begin(), this->end());
		//this->reserve(n);
		//for(unsigned int i = 0; i != n; ++i)
		//{
		//	this->p[i] = value;
		//	++(this->n_data);
		//}
		this->clear();
		this->insert(this->begin(), n, value);
	}
	
	
	void push_back(const value_type& value)
	{
		this->insert( this->end(), value );
	}
	
	
	void pop_back()
	{
		this->erase( this->end() - 1 );
	}
	
	
	private:
	iterator insertFix(iterator position, size_t n, const value_type& value, dstd::impl::TrueType)
	{
		if( position > this->end() || position < this->begin() )
		{
			throw dstd::out_of_range();
		}
		
		// Make sure there is sufficient capacity
		if( (this->size() + n) > this->capacity() )
		{
			// If a reallocation occurs during reserve, position is invalidated
			unsigned int i_position = static_cast<unsigned int>( position - this->begin() );
			this->reserve( this->size() + n );
			position = this->begin() + i_position;
		}
		
		assert( this->capacity() >= (this->size() + n) );
		
		if( position < this->end() )
		{
			// Shuffle the last n elements n positions along
			iterator it_from = iterator( &(this->back()) );
			iterator it_to = it_from + n;
			while( it_from > position )
			{
				this->a.construct( &(*it_to), *it_from );
				this->a.destroy( &(*it_from) );
				--it_from;
				--it_to;
			}
		}
		
		// Insert the new elements into the gap
		iterator it_to = position;
		for(unsigned int i = 0; i != n; ++i)
		{
			this->a.construct( &(*it_to), value );
			++it_to;
		}
		
		// The vector is now longer
		this->n_data += n;
		
		return position;
	}
	
	
	template <class InputIterator>
	iterator insertFix(iterator position, InputIterator first, InputIterator last, dstd::impl::FalseType)
	{
		if( position > this->end() || position < this->begin() )
		{
			throw dstd::out_of_range();
		}
		else if( last < first )
		{
			throw dstd::out_of_range();
		}
		
		unsigned int n = static_cast< unsigned int >( last - first );
		
		// Make sure there is sufficient capacity
		if( (this->size() + n) > this->capacity() )
		{
			// If a reallocation occurs during reserve, position is invalidated
			unsigned int i_position = static_cast<unsigned int>( position - this->begin() );
			this->reserve( this->size() + n );
			position = this->begin() + i_position; 
		}
		
		if( position < this->end() )
		{
			// Shuffle the last n elements n positions along
			iterator it_from = iterator( &(this->back()) );
			iterator it_to = it_from + n;
			while( it_from > position )
			{
				this->a.construct( &(*it_to), *it_from );
				this->a.destroy( &(*it_from) );
				--it_from;
				--it_to;
			}
		}
		
		// Insert the new elements into the gap
		InputIterator it_from = first;
		iterator it_to = position;
		while( it_from < last )
		{
			this->a.construct( &(*it_to), *it_from );
			++it_from;
			++it_to;
		}
		
		// The vector is now longer
		this->n_data += n;
		
		return position;
	}
	public:
	
	
	iterator insert(iterator position, const value_type& value)
	{
		return this->insertFix(position, 1, value, dstd::impl::TrueType());
	}
	
	
	void insert(iterator position, size_t n, const value_type& value)
	{
		this->insertFix(position, n, value, dstd::impl::TrueType());
	}
	
	
	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		typedef typename dstd::impl::BoolType< std::numeric_limits<InputIterator>::is_integer >::bool_type is_integer;
		this->insertFix( position, first, last, is_integer() );
	}
	
	
	iterator erase(iterator position)
	{
		return this->erase(position, position+1);
	}
	
	
	iterator erase(iterator first, iterator last)
	{
		// Check that first and last are in the expected order
		if( first == last )
		{
			return last;
		}
		else if( first < this->begin() || first > last || last > this->end() )
		{
			throw dstd::out_of_range();
		}
		
		unsigned int n_remove = static_cast<unsigned int>( last - first );
		
		iterator it_erase = first;
		iterator it_shuffle = last;
		const iterator it_end = this->end();
		
		// erase existing elements, and shuffle elements after last into the gap left behind
		while( it_erase < it_end )
		{
			this->a.destroy( &(*it_erase) );
			
			if( it_shuffle < this->end() )
			{
				this->a.construct( &(*it_erase), *it_shuffle);
				this->a.destroy( &(*it_shuffle) );
			}
			
			++it_erase;
			++it_shuffle;
		}
		
		// update size
		this->n_data -= n_remove;
		
		return first;
	}


	void swap(vector& v)
	{
		pointer temp_p = this->p;
		size_t temp_n_data = this->size();
		size_t temp_n_memory = this->capacity();
		
		this->p = v.p;
		this->n_data = v.n_data;
		this->n_memory = v.n_memory;
		
		v.p = temp_p;
		v.n_data = temp_n_data;
		v.n_memory = temp_n_memory;
	}
	
	
	void clear()		
	{
		this->erase(this->begin(), this->end());
	}
	
	
	private:
	
	allocator_type a;
	unsigned int n_data;
	unsigned int n_memory;
	pointer p;
};



//
// Vector Iterators
//

template <class T, class Allocator>
class dstd::vector<T, Allocator>::iterator
{
	public:
		
		typedef typename dstd::vector<T, Allocator>::value_type value_type;
		typedef typename dstd::vector<T, Allocator>::reference reference;
		typedef typename dstd::vector<T, Allocator>::pointer pointer;
		
		iterator(pointer ptr = 0) : p(ptr) {}
		iterator(const iterator& it) : p( it.p ) {}
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		iterator& operator=(const iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		iterator& operator+=(const int& n){ this->p += n; return (*this); }
		iterator& operator-=(const int& n){ this->p -= n; return (*this); }
		iterator operator+ (const int& n){ iterator it(*this); it += n; return it; }
		iterator operator- (const int& n){ iterator it(*this); it -= n; return it; }
		// Operators with iterators
		int operator- (const iterator& rhs){ return this->p - rhs.p; }
		iterator& operator++() { (*this) += 1; return (*this); }
		iterator& operator++(int) { iterator temp(*this); ++(*this); return temp; }
		iterator& operator--() { (*this) -= 1; return (*this); }
		iterator& operator--(int) { iterator temp(*this); --(*this); return temp; }
		bool operator==(const iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator!=(const iterator& rhs) const { return ! ( *this == rhs.p ); }
		bool operator< (const iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator> (const iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator<=(const iterator& rhs) const { return ! ( *this > rhs.p ); }
		bool operator>=(const iterator& rhs) const { return ! ( *this < rhs.p ); }
	
	private:
		
		T* p;
};



template <class T, class Allocator>
class dstd::vector<T, Allocator>::const_iterator
{
	public:
		
		typedef typename dstd::vector<T, Allocator>::value_type value_type;
		typedef typename dstd::vector<T, Allocator>::const_reference  reference;
		typedef typename dstd::vector<T, Allocator>::const_pointer  pointer;
		
		const_iterator(pointer ptr = 0) : p(ptr) {}
		const_iterator(const const_iterator& it) : p( it.p ) {}
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		const_iterator& operator=(const const_iterator& rhs) { this->p = rhs.p; return *this; }
		const_iterator& operator=(const dstd::vector<T, Allocator>::iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		const_iterator& operator+=(const int& n){ this->p += n; return (*this); }
		const_iterator& operator-=(const int& n){ this->p -= n; return (*this); }
		const_iterator operator+ (const int& n){ const_iterator it(*this); it += n; return it; }
		const_iterator operator- (const int& n){ const_iterator it(*this); it -= n; return it; }
		// Operators with iterators
		int operator- (const const_iterator& rhs){ return this->p - rhs.p; }
		const_iterator& operator++() { (*this) += 1; return (*this); }
		const_iterator& operator++(int) { const_iterator temp(*this); ++(*this); return temp; }
		const_iterator& operator--() { (*this) -= 1; return (*this); }
		const_iterator& operator--(int) { const_iterator temp(*this); --(*this); return temp; }
		bool operator==(const const_iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator!=(const const_iterator& rhs) const { return ! ( *this == rhs ); }
		bool operator< (const const_iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator> (const const_iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator<=(const const_iterator& rhs) const { return ! ( *this > rhs ); }
		bool operator>=(const const_iterator& rhs) const { return ! ( *this < rhs ); }
	
	private:
		
		pointer p;
};



//
// Vector Operators
//

template <class T, class Allocator>
bool operator== (const dstd::vector<T, Allocator>& v1, const dstd::vector<T, Allocator>& v2)
{
	if( v1.size() != v2.size() ) return false;
	for(unsigned int i = 0; i != v1.size(); ++i)
	{
		if( v1[i] != v2[i] ) return false;
	}
	return true;
}


template <class T, class Allocator>
bool operator!= (const dstd::vector<T,Allocator>& lhs, const dstd::vector<T,Allocator>& rhs)
{
	return ! (lhs == rhs);
}


template <class T, class Allocator>
bool operator< (const dstd::vector<T,Allocator>& lhs, const dstd::vector<T,Allocator>& rhs)
{
	size_t n = dstd::min(lhs.size(), rhs.size());
	
	for(unsigned int i = 0; i < n; ++i)
	{
		if( lhs[i] < rhs[i] )
		{
			return true;
		}
		else if( rhs[i] < rhs[i] )
		{
			return false;
		}
	}
	
	if( lhs.size() == rhs.size() )
	{
		return false; // they vectors are equal
	}
	else if( lhs.size() < rhs.size() )
	{
		return true;
	}
	else
	{
		return false;
	}
}


template <class T, class Allocator>
bool operator<= (const dstd::vector<T,Allocator>& lhs, const dstd::vector<T,Allocator>& rhs)
{
	size_t n = dstd::min(lhs.size(), rhs.size());
	
	for(unsigned int i = 0; i < n; ++i)
	{
		if( lhs[i] < rhs[i] )
		{
			return true;
		}
		else if( rhs[i] < rhs[i] )
		{
			return false;
		}
	}
	
	if( lhs.size() <= rhs.size() )
	{
		return true;
	}
	else
	{
		return false;
	}
}


template <class T, class Allocator>
bool operator> (const dstd::vector<T,Allocator>& lhs, const dstd::vector<T,Allocator>& rhs)
{
	return ! (lhs <= rhs);
}


template <class T, class Allocator>
bool operator>= (const dstd::vector<T,Allocator>& lhs, const dstd::vector<T,Allocator>& rhs)
{
	return ! (lhs < rhs);
}



//
// Swap

template <class T, class Allocator>
void dstd::swap(dstd::vector<T, Allocator>& v1, dstd::vector<T, Allocator>& v2)
{
	v1.swap(v2);
}



#endif
