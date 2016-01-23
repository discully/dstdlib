#ifndef DSTD_IMPL_VECTOR_IMPL_HXX
#define DSTD_IMPL_VECTOR_IMPL_HXX

#include "vector_base.hxx"
#include "../iterator.hxx"
#include "../memory.hxx"



namespace dstd
{
	namespace impl
	{
		template <class T, class Allocator> class vector_impl;
	}
}



template <class T, class Allocator>
class dstd::impl::vector_impl : public dstd::impl::vector_base
{
	public:

		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef impl::vector_base::size_type size_type;
		typedef ptrdiff_t difference_type;

		class iterator;
		class const_iterator;
		typedef dstd::reverse_iterator< iterator > reverse_iterator;
		typedef dstd::reverse_iterator< const_iterator > const_reverse_iterator;
		

		~vector_impl()
		{
			erase_range(begin(), end());
			if( p != 0 ) a.deallocate(p, n_memory);
		}


		iterator begin()
		{
			return iterator(p);
		}


		const_iterator begin() const
		{
			return const_iterator(p);
		}


		iterator end()
		{
			return iterator(p + size());
		}


		const_iterator end() const
		{
			return const_iterator(p + size());
		}


		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}


		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}


		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}


		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		
		void reserve(size_type n)
		{
			// Check if this request is necessary and valid
			if( n <= capacity() ) return;
			if( n > max_size() ) throw dstd::length_error();

			// Determine how much memory to request
			unsigned int n_request = dstd::max<unsigned int>(capacity(), 2);
			while( n_request < n )
			{
				n_request = dstd::min<unsigned int>(max_size(), n_request * 2);
			}

			assert(n_request >= n);

			// Request new memory, and copy existing values to new memory
			T* new_p = a.allocate(n_request);
			for(size_type i = 0; i != size(); ++i)
			{
				a.construct(new_p + i, p[i]);
			}

			// Move the vector to the new memory
			unsigned int old_n_memory = n_memory;
			T* old_p = p;
			p = new_p;
			n_memory = n_request;

			if( old_p != 0 )
			{
				// Clean up the old memory
				for(size_type i = 0; i != size(); ++i)
				{
					a.destroy(old_p + i);
				}
				a.deallocate(old_p, old_n_memory);
			}
		}


	protected:

		vector_impl()
			: p(0)
		{}


		iterator erase_range(iterator first, iterator last)
		{
			if( first == last )
			{
				return last;
			}

			const size_type n_remove = static_cast<size_type>(last - first);

			iterator it_erase = first;
			iterator it_shuffle = last;
			const iterator it_end = end();

			// erase existing elements, and shuffle elements after last into the gap left behind
			while( it_erase < it_end )
			{
				a.destroy(&(*it_erase));

				if( it_shuffle < end() )
				{
					a.construct(&(*it_erase), *it_shuffle);
					a.destroy(&(*it_shuffle));
				}

				++it_erase;
				++it_shuffle;
			}

			// update size
			n_data -= n_remove;

			return first;
		}


		iterator insert_value(iterator position, size_type n, const value_type& value)
		{
			if( position > end() || position < begin() )
			{
				throw dstd::out_of_range();
			}

			// Make sure there is sufficient capacity
			if( (size() + n) > capacity() )
			{
				// If a reallocation occurs during reserve, position is invalidated
				const size_type i_position = static_cast<size_type>(position - begin());
				reserve(size() + n);
				position = begin() + i_position;
			}

			assert(capacity() >= (size() + n));

			if( position < end() )
			{
				// Shuffle the last n elements n positions along
				iterator it_from = end() - 1;
				iterator it_to = it_from + n;
				while( it_from >= position )
				{
					a.construct(&(*it_to), *it_from);
					a.destroy(&(*it_from));
					--it_from;
					--it_to;
				}
			}

			// Insert the new elements into the gap
			iterator it_to = position;
			for(size_type i = 0; i != n; ++i)
			{
				a.construct(&(*it_to), value);
				++it_to;
			}

			// The vector is now longer
			n_data += n;

			return position;
		}


		template <class InputIterator>
		iterator insert_range(iterator position, InputIterator first, InputIterator last)
		{
			if( position > end() || position < begin() || last < first )
			{
				throw dstd::out_of_range();
			}

			const size_type n = static_cast<size_type>(last - first);

			// Make sure there is sufficient capacity
			if( (size() + n) > capacity() )
			{
				// If a reallocation occurs during reserve, position is invalidated
				size_type i_position = static_cast<size_type>(position - begin());
				reserve(size() + n);
				position = begin() + i_position;
			}

			if( position < end() )
			{
				// Shuffle the last n elements n positions along
				iterator it_from = end() - 1;
				iterator it_to = it_from + n;
				while( it_from >= position )
				{
					a.construct(&(*it_to), *it_from);
					a.destroy(&(*it_from));
					--it_from;
					--it_to;
				}
			}

			// Insert the new elements into the gap
			InputIterator it_from = first;
			iterator it_to = position;
			while( it_from < last )
			{
				a.construct(&(*it_to), *it_from);
				++it_from;
				++it_to;
			}

			// The vector is now longer
			n_data += n;

			return position;
		}


		allocator_type a;
		

		pointer p;
		

	private:
		
		/// Not implemented
		vector_impl(const vector_impl&);

		/// Not implemented
		vector_impl& operator= (const vector_impl&);
};



#endif
