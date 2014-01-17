#include "allocator.hxx"
#include "exception.hxx"
//#include "vector.hxx"

#include <cassert>
#include <algorithm>
#include <limits>
#include <iostream>



//
// Vector Class
//

// Constructors

template <class T>
dstd::vector<T>::vector()
	: p(0), n_data(0), n_memory(0)
{}


template <class T>
dstd::vector<T>::vector(unsigned int n, const T& value)
	: p(0), n_data(0), n_memory(0)
{
	this->assign(n, value);
}


template <class T>
dstd::vector<T>::vector(iterator first, iterator last)
	: p(0), n_data(0), n_memory(0)
{
	this->assign(first, last);
}


template <class T>
dstd::vector<T>::vector(const vector& v)
	: p(0), n_data(0), n_memory(0)
{
	this->assign(v.begin(), v.end());
}


// Destructor


template <class T>
dstd::vector<T>::~vector()
{
	this->clear();
	this->a.deallocate(this->p, this->n_memory);
}


// Assignment


template <class T>
dstd::vector<T>& dstd::vector<T>::operator= (const dstd::vector<T>& v)
{
	this->assign(v.begin(), v.end());
	return this;
}


// Iterators


template <class T>
typename dstd::vector<T>::iterator dstd::vector<T>::begin()
{
	return dstd::vector<T>::iterator( this->p );
}


///template <class T>
///dstd::vector<T>::const_iterator dstd::vector<T>::begin() const
///{
///	return dstd::vector<T>::const_iterator( this->p );
///}


template <class T>
typename dstd::vector<T>::iterator dstd::vector<T>::end()
{
	return dstd::vector<T>::iterator( &(this->p[ this->size() - 1]) );
}


///template <class T>
///dstd::vector<T>::const_iterator dstd::vector<T>::end() const
///{
///	return dstd::vector<T>::const_iterator( &(this->p[ this->size() - 1]) );
///}


template <class T>
typename dstd::vector<T>::reverse_iterator dstd::vector<T>::rbegin()
{
	return dstd::vector<T>::reverse_iterator( this->begin() );
}


///template <class T>
///dstd::vector<T>::const_reverse_iterator dstd::vector<T>::rbegin() const
///{
///	return dstd::vector<T>::const_reverse_iterator( this->begin() );
///}


template <class T>
typename dstd::vector<T>::reverse_iterator dstd::vector<T>::rend()
{
	return dstd::vector<T>::reverse_iterator( this->end() );
}


///template <class T>
///dstd::vector<T>::const_reverse_iterator dstd::vector<T>::rend() const
///{
///	return dstd::vector<T>::const_reverse_iterator( this->end() );
///}


// Capacity


template <class T>
unsigned int dstd::vector<T>::size() const
{
	return this->n_data;
}


template <class T>
unsigned int dstd::vector<T>::max_size() const
{
	return std::numeric_limits<unsigned int>::max();
}


template <class T>
void dstd::vector<T>::resize(unsigned int n, const T& value)
{
	if( n < this->size() )
	{
		dstd::vector<T>::iterator first = this->begin() + n;
		this->erase( first, this->end() );
	}
	else if( n > this->size() )
	{
		unsigned int n_add = static_cast< unsigned int >( n - this->size() );
		for(unsigned int i = 0; i != n_add; ++i)
		{
			this->push_back(value);
		}
	}
}


template <class T>
unsigned int dstd::vector<T>::capacity() const
{
	return this->n_memory;
}


template <class T>
bool dstd::vector<T>::empty() const
{
	return ( this->size() == 0 );
}


template <class T>
void dstd::vector<T>::reserve(unsigned int n)
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


// Element access


template <class T>
T& dstd::vector<T>::operator[](unsigned int i)
{
	return this->p[i];
}


template <class T>
const T& dstd::vector<T>::operator[](unsigned int i) const
{
	return this->p[i];
}


template <class T>
T& dstd::vector<T>::at(unsigned int i)
{
	if( i >= this->size() )
	{
		throw dstd::out_of_range();
	}
	return this->p[i];
}


template <class T>
const T& dstd::vector<T>::at(unsigned int i) const
{
	if( i >= this->size() )
	{
		throw dstd::out_of_range();
	}
	return this->p[i];
}


template <class T>
T& dstd::vector<T>::front()
{
	return this->p[0];
}


template <class T>
const T& dstd::vector<T>::front() const
{
	return this->p[0];
}


template <class T>
T& dstd::vector<T>::back()
{
	return this->p[ this->size() - 1 ];
}


template <class T>
const T& dstd::vector<T>::back() const
{
	return this->p[ this->size() - 1 ];
}


// Modifiers


template <class T>
void dstd::vector<T>::assign(iterator first, iterator last)
{
	this->erase();
	this->reserve( last - first );
	dstd::vector<T>::iterator it_from = first;
	while( it_from < last )
	{
		this->push_back( *it_from );
		++it_from;
	}
}


template <class T>
void dstd::vector<T>::assign(unsigned int n, const T& value)
{
	this->erase();
	this->reserve(n);
	for(unsigned int i = 0; i != n; ++i)
	{
		this->p[i] = value;
		++(this->n_data);
	}
}


template <class T>
void dstd::vector<T>::push_back(const T& value)
{
	this->insert( this->end(), value );
}


template <class T>
void dstd::vector<T>::pop_back()
{
	this->erase( this->end() - 1 );
}


template <class T>
typename dstd::vector<T>::iterator dstd::vector<T>::insert(typename dstd::vector<T>::iterator position, const T& value)
{
	return this->insert(position, 1, value);
}


template <class T>
typename dstd::vector<T>::iterator
dstd::vector<T>::insert(typename dstd::vector<T>::iterator position, unsigned int n, const T& value)
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
	
	// Shuffle the last n elements n positions along
	dstd::vector<T>::iterator it_from = dstd::vector<T>::iterator( &(this->back()) );
	dstd::vector<T>::iterator it_to = it_from + n;
	while( it_from > position )
	{
		this->a.construct( &(*it_to), *it_from );
		this->a.destroy( &(*it_from) );
		--it_from;
		--it_to;
	}
	
	// Insert the new elements into the gap behind
	while( it_to > position )
	{
		this->a.construct( &(*it_to), value );
		--it_to;
	}
	
	// The vector is now longer
	this->n_data += n;
	
	return position;
}


template <class T>
typename dstd::vector<T>::iterator
dstd::vector<T>::insert(iterator position, iterator first, iterator last)
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
	
	// Shuffle the last n elements n positions along
	dstd::vector<T>::iterator it_from = dstd::vector<T>::iterator( &this->back() );
	dstd::vector<T>::iterator it_to = it_from + n;
	while( it_from > position )
	{
		this->a.construct( &(*it_to), *it_from );
		this->a.destroy( &(*it_from) );
		--it_from;
		--it_to;
	}
	
	it_from = last - 1;
	
	// Insert the new elements into the gap behind
	while( it_to > position )
	{
		this->a.construct( &(*it_to), *it_from );
		--it_from;
		--it_to;
		assert( it_from >= first );
	}
	
	// The vector is now longer
	this->n_data += n;
	
	return position;
}


template <class T>
typename dstd::vector<T>::iterator dstd::vector<T>::erase(iterator position)
{
	return this->erase(position, position+1);
}


template <class T>
typename dstd::vector<T>::iterator dstd::vector<T>::erase(iterator first, iterator last)
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
	
	dstd::vector<T>::iterator it_erase = first;
	dstd::vector<T>::iterator it_shuffle = last;
	const dstd::vector<T>::iterator it_end = this->end();
	
	// erase existing elements, and shuffle elements after last into the gap left behind
	while( it_erase < it_end )
	{
		this->a.destroy( &(*it_erase) );
		
		if( it_shuffle < this->end() )
		{
			*it_erase = *it_shuffle;
		}
		
		++it_erase;
		++it_shuffle;
	}
	
	// update size
	this->n_data -= n_remove;
	
	return first;
}


///template <class T>
///void swap(dstd::vector<T>& v)
///{
///	dstd::swap(*this, v)
///}


template <class T>
void dstd::vector<T>::clear()
{
	this->erase(this->begin(), this->end());
}



//
// Vector Functions
//

///template <class T>
///void dstd::swap(dstd::vector<T>& a, dstd::vector<T>& b)
///{
///	T* temp_p = a.p;
///	unsigned int temp_n_data = a.size();
///	unsigned int temp_n_memory = a.capacity();
///	
///	a.p = b.p;
///	a.n_data = b.n_data;
///	a.n_momory = b.n_memory;
///	
///	b.p = temp.p;
///	b.n_data = temp.n_data;
///	b.n_momory = temp.n_memory;
///}



//
// Iterator Class
//

//
// Iterator functions
//
	
///template <class T>
///bool operator==(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ( &(*a) == &(*b) );
///}
///
///template <class T>
///bool operator<(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ( &(*a) < &(*b) );
///}
///
///template <class T>
///bool operator!=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ! ( a == b );
///}
///
///template <class T>
///bool operator>(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ( &(*a) > &(*b) );
///}
///
///template <class T>
///bool operator>=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ! ( a < b );
///}
///
///template <class T>
///bool operator<=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b)
///{
///	return ! ( a > b );
///}
