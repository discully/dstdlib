#ifndef DSTD_IMPL_VECTOR_IMPL_HXX
#define DSTD_IMPL_VECTOR_IMPL_HXX

#include "vector_base.hxx"
#include "../iterator.hxx"



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


		iterator begin()
		{
			return iterator(this->p);
		}


		const_iterator begin() const
		{
			return const_iterator(this->p);
		}


		iterator end()
		{
			return iterator(this->p + this->size());
		}


		const_iterator end() const
		{
			return const_iterator(this->p + this->size());
		}


		reverse_iterator rbegin()
		{
			return reverse_iterator(this->end());
		}


		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(this->end());
		}


		reverse_iterator rend()
		{
			return reverse_iterator(this->begin());
		}


		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(this->begin());
		}

		
		void reserve(size_t n)
		{
			// Check if this request is necessary and valid
			if (n <= this->capacity()) return;
			if (n > this->max_size()) throw dstd::length_error();

			// Determine how much memory to request
			unsigned int n_request = dstd::max<unsigned int>(this->capacity(), 2);
			while (n_request < n)
			{
				n_request = dstd::min<unsigned int>(this->max_size(), n_request * 2);
			}

			assert(n_request >= n);

			// Request new memory, and copy existing values to new memory
			T* new_p = this->a.allocate(n_request);
			for (unsigned int i = 0; i != this->size(); ++i)
			{
				this->a.construct(&(new_p[i]), p[i]);
			}

			// Move the vector to the new memory
			unsigned int old_n_memory = this->n_memory;
			T* old_p = this->p;
			this->p = new_p;
			this->n_memory = n_request;

			if (old_p != 0)
			{
				// Clean up the old memory
				for (unsigned int i = 0; i != this->size(); ++i)
				{
					this->a.destroy(old_p + i);
				}
				this->a.deallocate(old_p, old_n_memory);
			}
		}


	protected:

		vector_impl()
			: p(0)
		{}


		iterator erase_range(iterator first, iterator last)
		{
			if (first == last)
			{
				return last;
			}

			unsigned int n_remove = static_cast<unsigned int>(last - first);

			iterator it_erase = first;
			iterator it_shuffle = last;
			const iterator it_end = this->end();

			// erase existing elements, and shuffle elements after last into the gap left behind
			while (it_erase < it_end)
			{
				this->a.destroy(&(*it_erase));

				if (it_shuffle < this->end())
				{
					this->a.construct(&(*it_erase), *it_shuffle);
					this->a.destroy(&(*it_shuffle));
				}

				++it_erase;
				++it_shuffle;
			}

			// update size
			this->n_data -= n_remove;

			return first;
		}


		iterator insert_value(iterator position, size_t n, const value_type& value)
		{
			if (position > this->end() || position < this->begin())
			{
				throw dstd::out_of_range();
			}

			// Make sure there is sufficient capacity
			if ((this->size() + n) > this->capacity())
			{
				// If a reallocation occurs during reserve, position is invalidated
				unsigned int i_position = static_cast<unsigned int>(position - this->begin());
				this->reserve(this->size() + n);
				position = this->begin() + i_position;
			}

			assert(this->capacity() >= (this->size() + n));

			if (position < this->end())
			{
				// Shuffle the last n elements n positions along
				iterator it_from = this->end() - 1;
				iterator it_to = it_from + n;
				while (it_from >= position)
				{
					this->a.construct(&(*it_to), *it_from);
					this->a.destroy(&(*it_from));
					--it_from;
					--it_to;
				}
			}

			// Insert the new elements into the gap
			iterator it_to = position;
			for (unsigned int i = 0; i != n; ++i)
			{
				this->a.construct(&(*it_to), value);
				++it_to;
			}

			// The vector is now longer
			this->n_data += n;

			return position;
		}


		template <class InputIterator>
		iterator insert_range(iterator position, InputIterator first, InputIterator last)
		{
			if (position > this->end() || position < this->begin())
			{
				throw dstd::out_of_range();
			}
			else if (last < first)
			{
				throw dstd::out_of_range();
			}

			unsigned int n = static_cast< unsigned int >(last - first);

			// Make sure there is sufficient capacity
			if ((this->size() + n) > this->capacity())
			{
				// If a reallocation occurs during reserve, position is invalidated
				unsigned int i_position = static_cast<unsigned int>(position - this->begin());
				this->reserve(this->size() + n);
				position = this->begin() + i_position;
			}

			if (position < this->end())
			{
				// Shuffle the last n elements n positions along
				iterator it_from = this->end() - 1;
				iterator it_to = it_from + n;
				while (it_from >= position)
				{
					this->a.construct(&(*it_to), *it_from);
					this->a.destroy(&(*it_from));
					--it_from;
					--it_to;
				}
			}

			// Insert the new elements into the gap
			InputIterator it_from = first;
			iterator it_to = position;
			while (it_from < last)
			{
				this->a.construct(&(*it_to), *it_from);
				++it_from;
				++it_to;
			}

			// The vector is now longer
			this->n_data += n;

			return position;
		}


		allocator_type a;
		
		pointer p;
		
	private:

		vector_impl(const vector_impl&);
		vector_impl& operator= (const vector_impl&);
};



#endif
