#ifndef DSTD_DEQUE_HXX
#define DSTD_DEQUE_HXX



#include <cstddef>
#include <limits>
#include <iostream>

#include "dstd.hxx"
#include "iterator.hxx"
#include "memory.hxx"
#include "utility.hxx"



namespace dstd
{
	template <class T, class Allocator> class deque;
	
	template <class T, class Allocator> void swap(dstd::deque<T,Allocator>& d1, dstd::deque<T,Allocator>& d2);
	
	namespace impl
	{
		/// Returns the number of elements stored in a single chunk in the deque
		template <class T>
		inline size_t deque_chunk_size()
		{
			return (512 > sizeof(T)) ? (512 / sizeof(T)) : 1; // 512 is the number used in GCC's STL implementation
		}
	}
}



template < class T, class Allocator = dstd::allocator<T> >
class dstd::deque
{
	public:
	
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		class iterator;
		class const_iterator;
		typedef dstd::reverse_iterator< iterator > reverse_iterator;
		typedef dstd::reverse_iterator< const_iterator > const_reverse_iterator;
		
		
		//
		// Constructors
		
		explicit deque(const allocator_type& alloc = Allocator())
			: a(alloc), chunks(0), chunks_capacity(0), chunks_first(0), chunks_last(0), first(), last()
		{
			this->initialise();
		}
		
		
		explicit deque(size_type count, const value_type& value = value_type(), const allocator_type alloc = Allocator())
			: a(alloc), chunks(0), chunks_capacity(0), chunks_first(0), chunks_last(0), first(), last()
		{
			this->initialise();
			this->insert(this->begin(), count, value);
		}
		
		
		template <class InputIterator>
		deque(InputIterator first, InputIterator last, const allocator_type& alloc = Allocator())
			: a(alloc), chunks(0), chunks_capacity(0), chunks_first(0), chunks_last(0), first(), last()
		{
			this->initialise();
			this->insert(this->begin(), first, last);
		}
		
		
		deque(const deque& x)
			: a(x.get_allocator()), chunks(0), chunks_capacity(0), chunks_first(0), chunks_last(0), first(), last()
		{
			this->initialise();
			this->insert(this->begin(), x.begin(), x.end());
		}
		
		
		//
		// Destructor
		
		~deque()
		{
			this->clear();
			typename Allocator::template rebind<pointer>::other a_chunks;
			a_chunks.deallocate( this->chunks );
		}
		
		
		//
		// Assignment
		
		deque& operator= (const deque& x)
		{
			this->assign(x.begin(),x.end());
		}
		
		
		//
		// Iterators
		
		iterator begin() { return this->first; }
		
		const_iterator begin() const { return const_iterator(this->first); }
		
		iterator end() { return this->last; }
		
		const_iterator end() const { return const_iterator(this->last); }
		
		reverse_iterator rbegin() { return reverse_iterator(this->end()); }
		
		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }
		
		reverse_iterator rend() { return reverse_iterator(this->begin()); }
		
		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }
		
		
		//
		// Capacity
		
		size_type size() const
		{
			return ( this->end() - this->begin() );
		}
		
		
		size_type max_size() const
		{
			return std::numeric_limits< size_type >::max();
		}
		
		
		void resize(size_type n, const T& value = T())
		{
			while( this->size() > n )
			{
				this->pop_back();
			}
			while( this->size() < n )
			{
				this->push_back(value);
			}
		}
		
		
		bool empty() const
		{
			return (this->first == this->last);
		}
		
		
		//
		// Element Access
		
		
		reference operator[] (size_type i)
		{
			return this->begin()[i];
		}
		
		const_reference operator[] (size_type i) const
		{
			return this->begin()[i];
		}
		
		
		reference at(size_type i)
		{
			if( i > this->size() ) throw dstd::out_of_range();
			return this->begin()[i];
		}
		
		const_reference at(size_type i) const
		{
			if( i > this->size() ) throw dstd::out_of_range();
			return this->begin()[i];
		}
		
		reference front()
		{
			return *(this->begin());
		}
		
		const_reference front() const
		{
			return *(this->begin());
		}
		
		
		reference back()
		{
			return this->end()[-1];
		}
		
		const_reference back() const
		{
			return this->end()[-1];
		}
		
		
		//
		// Modifiers
		
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last)
		{
			this->clear();
			this->insert(this->begin(), first, last);
		}
		
		
		void assign(size_type n, const value_type& value)
		{
			this->clear();
			this->insert(this->begin(), n, value);
		}
		
		
		void push_back(const T& value)
		{
			
			this->a.construct(this->last.p, value);
			++(this->last);
			if( this->last.c == (this->chunks_last - 2) )
			{
				this->chunks_push_back(1);
			}
		}
		
		
		void push_front(const T& value)
		{
			if( this->first.p == this->first.first && this->first.c == this->chunks_first )
			{
				this->chunks_push_front(1);
			}
			--(this->first);
			this->a.construct(this->first.p, value);
		}
		
		
		void pop_back()
		{
			if( ! this->empty() )
			{
				--(this->last);
				this->a.destroy(this->last.p);
			}
		}
		
		
		/// All iterators are invalidated
		void pop_front()
		{
			if( ! this->empty() )
			{
				this->a.destroy( this->first.p );
				++(this->first);
			}
		}
		
		
	private:
		
		iterator insert_fix(iterator position, const size_type n, const value_type& value, dstd::impl::TrueType)
		{
			if( position == this->begin() )
			{
				for(size_type i = 0; i < n; ++i)
				{
					this->push_front(value);
				}
				return this->begin();
			}
			else if( position == this->end() )
			{
				for(size_type i = 0; i < n; ++i)
				{
					this->push_back(value);
				}
				return this->end() - 1;
			}
			else
			{
				// Ensure any allocation requried is done here.
				// Then, if an exception is thrown, the deque is left in a valid state.
				if( this->last.p + n > this->last.last )
				{
					// Iterators can be invalidated if allocations occur.
					// So save the iterator's offset from the start, and restore position
					// to a valid iterator afterwards.
					const difference_type offset = position - this->begin();
					
					const size_type chunk_size = dstd::impl::deque_chunk_size<value_type>();
					
					// Allocation of new chunks occurs when the penultimate allocated chunk is full.
					//     penulatimate_allocated_chunk = this->chunks_last - 2;
					// Number of elements remaining before this->last goes into a new chunk:
					//     current_chunk_capacity = this->last.p - this->last.last;
					// Number of elements available before allocation:
					//     n_remaining = current_chunk_capacity + (penultimate_allocated_chunk - this->last.c)*chunk_size
					size_type n_remaining = (this->last.p - this->last.last) + (((this->chunks_last - 2) - this->last.c)*chunk_size);
					
					if( n_remaining < n )
					{
						this->chunks_push_back( 1 + ((n - n_remaining)/chunk_size) );
					}
					
					position = this->begin() + offset;
				}
				
				// Can now proceed, safe in the knowledge that no allocations will occur,
				// and therefore, this will definitely all succeed.
				iterator it;
				const iterator old_end = this->end();
				for(it = old_end - n; it != old_end; ++it)
				{
					this->push_back( *it );
				}
				for(it = old_end - 1; it >= position + n; --it)
				{
					*it = it[-n];
				}
				for(size_type i = 0; i < n; ++i)
				{
					position[i] = value;
				}
				
				return position;
			}
		}

		template <class InputIterator>
		void insert_fix(iterator position, InputIterator first, InputIterator last, dstd::impl::FalseType)
		{
			if( position == this->begin() )
			{
				while( first != last )
				{
					--last;
					this->push_front(*last);
				}
			}
			else if( position == this->end() )
			{
				while( first != last )
				{
					this->push_back(*first);
					++first;
				}
			}
			else
			{
				const size_type n = last - first;
				
				// Ensure any allocation requried is done here.
				// Then, if an exception is thrown, the deque is left in a valid state.
				if( this->last.p + n > this->last.last )
				{
					// Iterators can be invalidated if allocations occur.
					// So save the iterator's offset from the start, and restore position
					// to a valid iterator afterwards.
					const difference_type offset = position - this->begin();
					
					const size_type chunk_size = dstd::impl::deque_chunk_size<value_type>();
					
					// Allocation of new chunks occurs when the penultimate allocated chunk is full.
					//     penulatimate_allocated_chunk = this->chunks_last - 2;
					// Number of elements remaining before this->last goes into a new chunk:
					//     current_chunk_capacity = this->last.p - this->last.last;
					// Number of elements available before allocation:
					//     n_remaining = current_chunk_capacity + (penultimate_allocated_chunk - this->last.c)*chunk_size
					size_type n_remaining = (this->last.p - this->last.last) + (((this->chunks_last - 2) - this->last.c)*chunk_size);
					
					if( n_remaining < n )
					{
						this->chunks_push_back( 1 + ((n - n_remaining)/chunk_size) );
					}
					
					position = this->begin() + offset;
				}
				
				// Can now proceed, safe in the knowledge that no allocations will occur,
				// and therefore, this will definitely all succeed.
				iterator it;
				const iterator old_end = this->end();
				for(it = old_end - n; it != old_end; ++it)
				{
					this->push_back( *it );
				}
				for(it = old_end - 1; it >= position + n; --it)
				{
					*it = it[-n];
				}
				for(it = position; first != last; ++first, ++it)
				{
					*it = *first;
				}
			}
		}
		
	public:
		
		
		iterator insert(iterator position, const value_type& value)
		{
			return this->insert_fix( position, 1, value, dstd::impl::TrueType() );
		}
		
		
		void insert(iterator position, size_type n, const value_type& value)
		{
			this->insert_fix( position, n, value, dstd::impl::TrueType() );
		}
		
		
		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last)
		{
			typedef typename dstd::impl::BoolType< std::numeric_limits<InputIterator>::is_integer >::bool_type is_integer;
			this->insert_fix( position, first, last, is_integer() );
		}
		
		
		/// Removes the element in position from the deque.
		/// @returns An iterator pointing to the new location of the element that followed the last element erased by the function call. This is the container end if the operation erased the last element in the sequence.
		iterator erase(iterator position)
		{
			if( position == this->begin() )
			{
				this->pop_front();
				return this->begin();
			}
			else if( position == this->end() - 1 )
			{
				this->pop_back();
				return this->end();
			}
			else
			{
				iterator temp(position);
				while(position != this->end())
				{
					*position = position[1];
					++position;
				}
				this->pop_back();
				return temp;
			}
		}
		
		
		/// Removes elements in the range (first,last] from the deque.
		/// @returns An iterator pointing to the new location of the element that followed the last element erased by the function call. This is the container end if the operation erased the last element in the sequence.
		iterator erase(iterator first, iterator last)
		{
			if( last == this->end() )
			{
				while( last != first )
				{
					--last;
					this->pop_back();
				}
				return this->end();
			}
			else if( first == this->begin() )
			{
				while( last != first )
				{
					++first;
					this->pop_front();
				}
				return this->begin();
			}
			else
			{
				iterator it = first;
				while( last != this->end() )
				{
					*first = *last;
					++first;
					++last;
				}
				while( first != this->end() )
				{
					this->pop_back();
				}
				return it;
			}
		}
		
		
		void swap(deque& x)
		{
			dstd::swap( this->chunks, x.chunks );
			dstd::swap( this->chunks_capacity, x.chunks_capacity );
			dstd::swap( this->chunks_first, x.chunks_first );
			dstd::swap( this->chunks_last, x.chunks_last );
			dstd::swap( this->first, x.first );
			dstd::swap( this->last, x.last );
		}
		
		
		void clear()
		{
			for(iterator it = this->begin(); it != this->end(); ++it)
			{
				this->a.destroy(it.p);
			}
			// TODO: leave first/last in the middle of the available chunks
			this->last = this->first;
		}
		
		
		//
		// Allocator
		
		allocator_type get_allocator() const { return this->a; }
		
	
	private:
	
		void initialise()
		{
			const size_type n_chunks = 5;
			const size_type chunk_size = dstd::impl::deque_chunk_size<value_type>();
			
			typename Allocator::template rebind<pointer>::other a_chunks;
			this->chunks = a_chunks.allocate(n_chunks);
			
			for(size_type i = 0; i < n_chunks; ++i)
			{
				this->chunks[i] = this->a.allocate( chunk_size );
			}
			
			this->chunks_capacity = n_chunks;
			this->chunks_first = this->chunks;
			this->chunks_last = this->chunks_first + n_chunks;
			
			this->first.set_chunk( this->chunks + (n_chunks / 2) );
			this->first.p = this->first.first + (chunk_size / 2);
			
			this->last = this->first;
		}
		
		
		void chunks_expand_back()
		{
			const size_type new_capacity = 2 * this->chunks_capacity;
			
			typename Allocator::template rebind<pointer>::other a_chunks;
			pointer* new_chunks = a_chunks.allocate(new_capacity);
			pointer* new_first = new_chunks + (this->chunks_first - this->chunks);
			
			pointer* new_p = new_first;
			for(pointer* old_p = this->chunks_first; old_p != this->chunks_last; ++old_p)
			{
				*new_p = *old_p;
				if( this->first.c == old_p ) this->first.c = new_p;
				if( this->last.c  == old_p ) this->last.c  = new_p;
				++new_p;
			}
			
			this->chunks = new_chunks;
			this->chunks_capacity = new_capacity;
			this->chunks_first = new_first;
			this->chunks_last = new_p;
		}
		
		
		void chunks_expand_front()
		{
			const size_type new_capacity = 2 * this->chunks_capacity;
			const difference_type offset = new_capacity - this->chunks_capacity;
			
			typename Allocator::template rebind<pointer>::other a_chunks;
			pointer* new_chunks = a_chunks.allocate(new_capacity);
			pointer* new_first = new_chunks + offset + (this->chunks_first - this->chunks);
			
			pointer* new_p = new_first;
			for(pointer* old_p = this->chunks_first; old_p != this->chunks_last; ++old_p)
			{
				*new_p = *old_p;
				if( this->first.c == old_p ) this->first.c = new_p;
				if( this->last.c  == old_p ) this->last.c  = new_p;
				++new_p;
			}
			
			this->chunks = new_chunks;
			this->chunks_first = new_first;
			this->chunks_last = new_p;
			this->chunks_capacity = new_capacity;
		}
		
		void chunks_push_back(size_type n)
		{
			while( (this->chunks_last - this->chunks) + n > this->chunks_capacity )
			{
				this->chunks_expand_back();
			}
			
			for(size_type i = 0; i < n; ++i)
			{
				*(this->chunks_last) = this->a.allocate( dstd::impl::deque_chunk_size<value_type>() );
				++(this->chunks_last);
			}
		}
		
		void chunks_push_front(size_type n)
		{
			while( this->chunks + n > this->chunks_first )
			{
				this->chunks_expand_front();
			}
			
			for(size_type i = 0; i < n; ++i)
			{
				pointer new_chunk = this->a.allocate( dstd::impl::deque_chunk_size<value_type>() );
				--(this->chunks_first);
				*(this->chunks_first) = new_chunk;
			}
		}
		
		void chunks_pop_back(size_type n)
		{
			for(size_type i = 0; i < n; ++i)
			{
				--(this->chunks_last);
				this->a.deallocate( this->chunks_last );
			}
		}
		
		
		void chunks_pop_front(size_type n)
		{
			for(size_type i = 0; i < n; ++i)
			{
				this->a.deallocate( this->chunks_first );
				++(this->chunks_first);
			}
		}
		
		allocator_type a;
		pointer* chunks;
		size_type chunks_capacity;
		pointer* chunks_first;
		pointer* chunks_last;
		iterator first;
		iterator last;
};



//
// Deque: Relational Operators

template <class T>
bool operator== (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	if( lhs.size() != rhs.size() ) return false;
	typename dstd::deque<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::deque<T>::const_iterator rhs_it = rhs.begin();
	typename dstd::deque<T>::const_iterator lhs_end = lhs.end();
	typename dstd::deque<T>::const_iterator rhs_end = rhs.end();
	while( lhs_it != lhs_end )
	{
		if( *lhs_it != *rhs_it ) return false;
		++lhs_it;
		++rhs_it;
	}
	return true;
}


template <class T>
bool operator!= (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	return ! (lhs == rhs);
}


template <class T>
bool operator< (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	typename dstd::deque<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::deque<T>::const_iterator rhs_it = rhs.begin();
	typename dstd::deque<T>::const_iterator lhs_end = lhs.end();
	typename dstd::deque<T>::const_iterator rhs_end = rhs.end();
	while( lhs_it != lhs_end && rhs_it != rhs_end )
	{
		if( *lhs_it < *rhs_it ) return true;
		if( *lhs_it > *rhs_it ) return false;
		++lhs_it;
		++rhs_it;
	}
	if( rhs_it == rhs_end ) return false;
	return true;
	
}


template <class T>
bool operator<= (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	typename dstd::deque<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::deque<T>::const_iterator rhs_it = rhs.begin();
	typename dstd::deque<T>::const_iterator lhs_end = lhs.end();
	typename dstd::deque<T>::const_iterator rhs_end = rhs.end();
	while( lhs_it != lhs_end && rhs_it != rhs_end )
	{
		if( *lhs_it < *rhs_it ) return true;
		if( *lhs_it > *rhs_it ) return false;
		++lhs_it;
		++rhs_it;
	}
	if( lhs_it == lhs_end ) return true;
	return false;
}


template <class T>
bool operator> (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	return ! (lhs <= rhs);
}


template <class T>
bool operator>= (const dstd::deque<T>& lhs, const dstd::deque<T>& rhs)
{
	return ! (lhs < rhs);
}



//
// Deque: Swap

template <class T, class Allocator>
void dstd::swap(dstd::deque<T,Allocator>& d1, dstd::deque<T,Allocator>& d2)
{
	d1.swap(d2);
}



//
// Iterators


template <class T, class Allocator>
class dstd::deque<T, Allocator>::iterator
{
	public:
		
		typedef typename dstd::deque<T, Allocator>::value_type value_type;
		typedef typename dstd::deque<T, Allocator>::reference reference;
		typedef typename dstd::deque<T, Allocator>::pointer pointer;
		typedef typename dstd::deque<T, Allocator>::size_type size_type;
		typedef typename dstd::deque<T, Allocator>::difference_type difference_type;
		
		// constructors
		iterator() : c(0), first(0), last(0), p(0) {}
		iterator(const iterator& x) : c(x.c), first(x.first), last(x.last), p(x.p) {}
		// destructor
		~iterator(){}
		// assignment
		iterator& operator= (const iterator& x)
		{
			this->c = x.c;
			this->first = x.first;
			this->last = x.last;
			this->p = x.p;
			return *this;
		}
		// access
		reference operator* () { return *(this->p); }
		const reference operator* () const { return *(this->p); }
		pointer operator-> () { return this->p; };
		const pointer operator-> () const { return this->p; };
		reference operator[] (difference_type n) { return *( *this + n ); }
		const reference operator[] (difference_type n) const { return *( *this + n ); }
		// increment/decrement
		iterator& operator++() { return ( (*this) += 1 ); }
		iterator& operator--() { return ( (*this) -= 1 ); }
		iterator& operator++(int) { iterator temp(*this); ++(*this); return temp; }
		iterator& operator--(int) { iterator temp(*this); --(*this); return temp; }
		// addition/subtraction of ints
		iterator& operator+= (difference_type n)
		{
			if( n >= (this->first - this->p) && n < (this->last - this->p) )
			{
				// staying within the same chunk
				this->p += n;
			}
			else
			{
				const size_type chunk_size = dstd::impl::deque_chunk_size<value_type>();
				
				if( n > 0 )
				{
					n -= ((this->last) - this->p);
					this->set_chunk( this->c + (1 + (n/chunk_size)) );
					this->p = this->first + (n % chunk_size);
				}
				else
				{
					n = -n;
					n -= (this->p - this->first);
					this->set_chunk( this->c - (1 + (n / chunk_size)) );
					this->p = this->last - (n % chunk_size);
				}
			}
			return *this;
		}
		iterator& operator-= (difference_type n) { return ( (*this) += (-n) ); }
		iterator  operator+  (difference_type n) const { iterator temp(*this); temp += n; return temp; }
		iterator  operator-  (difference_type n) const { iterator temp(*this); temp -= n; return temp; }
		// addition/subtraction with iterators
		difference_type operator- (const iterator& x) const
		{
			return ((this->c - x.c)*dstd::impl::deque_chunk_size<value_type>()) + (this->p - this->first) - (x.p - x.first);
		}
		// comparison operators
		bool operator== (const iterator& x) const { return (this->p == x.p); }
		bool operator!= (const iterator& x) const { return ! (*this == x); }
		bool operator<  (const iterator& x) const { return ( (this->c <= x.c) && ( (this->c < x.c) || (this->p < x.p) ) ); }
		bool operator>  (const iterator& x) const { return ( (this->c >= x.c) && ( (this->c > x.c) || (this->p > x.p) ) ); }
		bool operator<= (const iterator& x) const { return ! ( *this > x ); }
		bool operator>= (const iterator& x) const { return ! ( *this < x ); }
	
	
	private:
		
		void set_chunk(pointer* new_chunk)
		{
			this->c = new_chunk;
			this->first = *(this->c);
			this->last = this->first + dstd::impl::deque_chunk_size<value_type>();
		}
		
		pointer* c; // pointer to current chunk
		pointer first; // first element of current chunk
		pointer last; // beyond-the-end of current chunk
		pointer p; // current element in current chunk
	
	
	friend class dstd::deque<T, Allocator>;
};



template <class T, class Allocator>
class dstd::deque<T, Allocator>::const_iterator
{	
	public:
		
		typedef const typename dstd::deque<T, Allocator>::value_type value_type;
		typedef typename dstd::deque<T, Allocator>::const_reference reference;
		typedef typename dstd::deque<T, Allocator>::const_pointer pointer;
		typedef typename dstd::deque<T, Allocator>::size_type size_type;
		typedef typename dstd::deque<T, Allocator>::difference_type difference_type;
		
		// constructors
		const_iterator() : c(0), first(0), last(0), p(0) {}
		const_iterator(const const_iterator& x) : c(x.c), first(x.first), last(x.last), p(x.p) {}
		const_iterator(const iterator& x) : c(x.c), first(x.first), last(x.last), p(x.p) {}
		// destructor
		~const_iterator(){}
		// assignment
		const_iterator& operator= (const const_iterator& x)
		{
			this->c = x.c;
			this->first = x.first;
			this->last = x.last;
			this->p = x.p;
			return *this;
		}
		const_iterator& operator= (const iterator& x)
		{
			this->c = x.c;
			this->first = x.first;
			this->last = x.last;
			this->p = x.p;
			return *this;
		}
		// access
		reference operator* () { return *(this->p); }
		pointer operator-> () { return this->p; };
		reference operator[] (difference_type n) { return *( *this + n ); }
		// increment/decrement
		const_iterator& operator++() { return ( (*this) += 1 ); }
		const_iterator& operator--() { return ( (*this) -= 1 ); }
		const_iterator& operator++(int) { const_iterator temp(*this); ++(*this); return temp; }
		const_iterator& operator--(int) { const_iterator temp(*this); --(*this); return temp; }
		// addition/subtraction of ints
		const_iterator& operator+= (difference_type n)
		{
			if( n >= (this->first - this->p) && n < (this->last - this->p) )
			{
				// staying within the same chunk
				this->p += n;
			}
			else
			{
				const size_type chunk_size = dstd::impl::deque_chunk_size<value_type>();
				
				if( n > 0 )
				{
					n -= ((this->last) - this->p);
					this->set_chunk( this->c + (1 + (n/chunk_size)) );
					this->p = this->first + (n % chunk_size);
				}
				else
				{
					n = -n;
					n -= (this->p - this->first);
					this->set_chunk( this->c - (1 + (n / chunk_size)) );
					this->p = this->last - (n % chunk_size);
				}
			}
			return *this;
		}
		const_iterator& operator-= (difference_type n) { return ( (*this) += (-n) ); }
		const_iterator  operator+  (difference_type n) const { const_iterator temp(*this); temp += n; return temp; }
		const_iterator  operator-  (difference_type n) const { const_iterator temp(*this); temp -= n; return temp; }
		// addition/subtraction with iterators
		difference_type operator- (const const_iterator& x) const
		{
			return ((this->c - x.c)*dstd::impl::deque_chunk_size<value_type>()) + (this->p - this->first) - (x.p - x.first);
		}
		// comparison operators
		bool operator== (const const_iterator& x) const { return (this->p == x.p); }
		bool operator!= (const const_iterator& x) const { return ! (*this == x); }
		bool operator<  (const const_iterator& x) const { return ( (this->c <= x.c) && ( (this->c < x.c) || (this->p < x.p) ) ); }
		bool operator>  (const const_iterator& x) const { return ( (this->c >= x.c) && ( (this->c > x.c) || (this->p > x.p) ) ); }
		bool operator<= (const const_iterator& x) const { return ! ( *this > x ); }
		bool operator>= (const const_iterator& x) const { return ! ( *this < x ); }
	
	
	private:
		
		void set_chunk(pointer const* new_chunk)
		{
			this->c = new_chunk;
			this->first = *(this->c);
			this->last = this->first + dstd::impl::deque_chunk_size<value_type>();
		}
		
		pointer const* c; // pointer to current chunk
		pointer first; // first element of current chunk
		pointer last; // beyond-the-end of current chunk
		pointer p; // current element in current chunk
	
	
	friend class dstd::deque<T, Allocator>;
};

#endif
