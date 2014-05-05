#ifndef DSTD_ALLOCATOR_HXX
#define DSTD_ALLOCATOR_HXX

#include <new>

#include "exception.hxx"


namespace dstd
{
	template <class T>
	class allocator
	{
		public:
			
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef const T* const_pointer;
			typedef const T& const_reference;
			
			
			allocator(){};
			
			
			~allocator(){};
			
			
			T* address(T& obj) const
			{
				// this breaks if T overloaded operator&
				return &obj;
			}
			
			
			const T* address(const T& obj) const
			{
				// this breaks if T overloaded operator&
				return &obj;
			}

			
			/// Attempts to allocate a block of storage with a size large enough to contain n elements and returns a pointer to the first element.
			/// Throws bad_alloc if it cannot allocate the total amount of storage requested.
			T* allocate(unsigned int n) //, const T* hint = 0)
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
			
			
			/// Releases a block of storage previously allocated with member allocate and not yet released
			/// The elements in the array are not destroyed by a call to this member function.
			void deallocate(T* p, unsigned int n)
			{
				::operator delete( p );
			}
			
			
			/// Constructs an element object on the location pointed by p.
			void construct(T* p, const T& value)
			{
				new(p) T(value);
			}
			
			
			/// Destroys in-place the object pointed by p.
			void destroy(T* p)
			{
				p->~T();
			}
	};
}

#endif
