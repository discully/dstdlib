#ifndef DSTD_VECTOR_HXX
#define DSTD_VECTOR_HXX

#include <limits>
#include <cstddef>

#include "impl/bool_type.hxx"
#include "impl/vector_impl.hxx"
#include "impl/vector_iterator.hxx"
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
class dstd::vector : public dstd::impl::vector_impl<T, Allocator>
{
	public:
	
	typedef typename impl::vector_impl<T,Allocator>::value_type value_type;
	typedef typename impl::vector_impl<T,Allocator>::allocator_type allocator_type;
	typedef typename impl::vector_impl<T,Allocator>::reference reference;
	typedef typename impl::vector_impl<T,Allocator>::const_reference const_reference;
	typedef typename impl::vector_impl<T,Allocator>::pointer pointer;
	typedef typename impl::vector_impl<T,Allocator>::const_pointer const_pointer;
	typedef typename impl::vector_impl<T,Allocator>::size_type size_type;
	typedef typename impl::vector_impl<T,Allocator>::difference_type difference_type;

	typedef impl::vector_impl<T,Allocator>::iterator iterator;
	typedef impl::vector_impl<T,Allocator>::const_iterator const_iterator;
	typedef dstd::reverse_iterator< iterator > reverse_iterator;
	typedef dstd::reverse_iterator< const_iterator > const_reverse_iterator;
	
	
	//
	// Constructors
	
	explicit vector()
	{}
	
	
	explicit vector(unsigned int n, const T& value = T())
	{
		this->assign(n, value);
	}
	
	
	vector(iterator first, iterator last)
	{
		this->assign(first, last);
	}
	
	
	vector(const vector& v)
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
		this->clear();
		this->insert(this->begin(), first, last);
	}
	
	
	void assign(size_t n, const value_type& value)
	{
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
		return insert_value(position, n, value);
	}
	
	
	template <class InputIterator>
	iterator insertFix(iterator position, InputIterator first, InputIterator last, dstd::impl::FalseType)
	{
		return insert_range(position, first, last);
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
		return erase_range(position, position+1);
	}


	iterator erase(iterator first, iterator last)
	{
		return erase_range(first, last);
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
