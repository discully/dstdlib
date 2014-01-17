#ifndef DSTD_ALLOCATOR_HXX
#define DSTD_ALLOCATOR_HXX

namespace dstd
{
	template <class T>
	class allocator
	{
		public:
		
			allocator(){};
			~allocator(){};
			
			T* address(T& obj) const;
			const T* address(const T& obj) const;
			
			/// Attempts to allocate a block of storage with a size large enough to contain n elements and returns a pointer to the first element.
			/// Throws bad_alloc if it cannot allocate the total amount of storage requested.
			T* allocate(unsigned int n);//, const T* hint = 0);
			
			/// Releases a block of storage previously allocated with member allocate and not yet released
			/// The elements in the array are not destroyed by a call to this member function.
			void deallocate(T* p, unsigned int n);
			
			/// Constructs an element object on the location pointed by p.
			void construct(T* p, const T& value);
			
			/// Destroys in-place the object pointed by p.
			void destroy(T* p);
	};
}

#include "allocator.cxx"

#endif
