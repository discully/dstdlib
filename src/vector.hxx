#ifndef DSTD_VECTOR_HXX
#define DSTD_VECTOR_HXX

#include "impl/bool_type.hxx"
#include "impl/vector_impl.hxx"
#include "impl/vector_iterator.hxx"
#include "algorithm.hxx"



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
		assign(n, value);
	}
	
	
	vector(iterator first, iterator last)
	{
		assign(first, last);
	}
	
	
	vector(const vector& v)
	{
		assign<const_iterator>(v.begin(), v.end());
	}
	
	
	//
	// Destructor
	
	
	~vector()
	{
		clear();
		if( p != 0 ) a.deallocate(p, n_memory);
	}
	
	
	//
	// Assignment
	
	
	vector& operator= (const vector& v)
	{
		assign(v.begin(), v.end());
		return *this;
	}
	
	
	//
	// Capacity
	
	
	void resize(size_t n, const T& value = T())
	{
		if( n < size() )
		{
			iterator first = begin() + n;
			erase( first, end() );
		}
		else if( n > size() )
		{
			size_t n_add = n - size();
			for(unsigned int i = 0; i != n_add; ++i)
			{
				push_back(value);
			}
		}
	}
	
	
	//
	// Element access
	
	
	reference operator[](size_t i)
	{
		return p[i];
	}
	
	
	const_reference operator[](size_t i) const
	{
		return p[i];
	}
	
	
	reference at(size_t i)
	{
		if( i >= size() )
		{
			throw dstd::out_of_range();
		}
		return p[i];
	}
	
	
	const_reference at(size_t i) const
	{
		if( i >= size() )
		{
			throw dstd::out_of_range();
		}
		return p[i];
	}
	
	
	reference front()
	{
		return p[0];
	}
	
	
	const_reference front() const
	{
		return p[0];
	}


	reference back()
	{
		if( empty() )
		{
			return front();
		}
		return p[size() - 1];
	}
	
	
	const_reference back() const
	{
		if( empty() )
		{
			return front();
		}
		return p[size() - 1];
	}
	
	
	//
	// Modifiers
	
	
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		clear();
		insert(begin(), first, last);
	}
	
	
	void assign(size_t n, const value_type& value)
	{
		clear();
		insert(begin(), n, value);
	}
	
	
	void push_back(const value_type& value)
	{
		insert(end(), value);
	}
	
	
	void pop_back()
	{
		erase(end() - 1);
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
		return insert_value(position, 1, value);
	}
	
	
	void insert(iterator position, size_t n, const value_type& value)
	{
		insert_value(position, n, value);
	}
	
	
	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		typedef typename dstd::impl::BoolType< std::numeric_limits<InputIterator>::is_integer >::bool_type is_integer;
		insertFix( position, first, last, is_integer() );
	}
	
	
	iterator erase(iterator position)
	{
		return erase_range(position, position + 1);
	}


	iterator erase(iterator first, iterator last)
	{
		return erase_range(first, last);
	}


	void swap(vector& v)
	{
		pointer temp_p = p;
		size_t temp_n_data = size();
		size_t temp_n_memory = capacity();
		
		p = v.p;
		n_data = v.n_data;
		n_memory = v.n_memory;
		
		v.p = temp_p;
		v.n_data = temp_n_data;
		v.n_memory = temp_n_memory;
	}
	
	
	void clear()		
	{
		erase(begin(), end());
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
