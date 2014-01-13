#include "allocator.hxx"
#include "exception.hxx"

#include <new>



template <class T>
T* dstd::allocator::address(T& obj) const
{
	// this breaks if T overloaded operator&
	return &obj;
}


template <class T>
const T* dstd::allocator::address(const T& obj) const
{
	// this breaks if T overloaded operator&
	return &obj;
}


template <class T>			
T* dstd::allocator::allocate(unsigned int n)
{
	T* a = 0;
	
	try
	{
		a = reinterpret_cast< T* >( ::operator new(n*sizeof(T)) );
	}
	catch( std::bad_alloc )
	{
		throw dstd::bad_alloc();
	}
	
	if( a == 0 )
	{
		throw dstd::bad_alloc();
	}
	
	return a;
}


template <class T>
void dstd::allocator::deallocate(T* p, unsigned int n)
{
	::operator delete(p, n*sizeof(T) );
}


template <class T>
void dstd::allocator::construct(T* p, const T& value)
{
	new(p) T(value);
}


template <class T>
void dstd::allocator::destroy(T* p)
{
	p->~T();
}
