#ifndef DSTD_BITSET_HXX
#define DSTD_BITSET_HXX



#include <climits>
#include <cstddef>
#include <limits>
#include <string>

#include "algorithm.hxx"
#include "exception.hxx"



namespace dstd
{
	template <size_t N> class bitset;
}



template <size_t N>
class dstd::bitset
{
	public:
		
		class reference;
		
		
		bitset()
		{
			this->reset();
		}
		
		
		bitset(unsigned long long value)
		{
			// TODO: could be more efficient, by just bitshifting value, and copying into data
			this->reset();
			const size_t n = std::numeric_limits<unsigned long long>::digits < N ? std::numeric_limits<unsigned long long>::digits : N;
			unsigned long long mask = 1;
			for(size_t i = 0; i < n; ++i)
			{
				this->set(i, ((value & mask) == mask));
				mask <<= 1;
			}
		}
		
		
		template< class Character, class Traits, class Allocator >
		explicit bitset(
			const std::basic_string<Character,Traits,Allocator>& str,
		 	typename std::basic_string<Character,Traits,Allocator>::size_type pos = 0,
		 	typename std::basic_string<Character,Traits,Allocator>::size_type n = std::basic_string<Character,Traits,Allocator>::npos
			)
		{
			n = dstd::min(N, str.size() - pos);
			this->reset();
			for(size_t i = 0; i < n; ++i)
			{
				if( Traits::eq(str[pos+i], '1') )
				{
					this->set( (n-1) - i);
				}
				else if( ! Traits::eq(str[pos+i], '0') )
				{
					throw dstd::invalid_argument();
				}
			}
		}
		
		
		~bitset(){}
		
		
		bool operator== (const bitset& x) const
		{
			const size_t size = bitset::data_size();
			unsigned char mask(-1);
			if( (N % CHAR_BIT) != 0 ) mask >>= (CHAR_BIT - (N % CHAR_BIT));
			for(size_t i = 0; i < size; ++i)
			{
				if( (this->data[i] & mask) != (x.data[i] & mask) ) return false;
				
				if( i == 0 ) mask = -1;
			}
			return true;
		}
		
		
		bool operator!= (const bitset& x) const
		{
			return ! (*this == x);
		}
		
		
		bool operator[] (size_t bit) const
		{
			return this->get_bit(bit);
		}
		
		
		reference operator[] (size_t bit)
		{
			return this->get_reference(bit);
		}
		
		
		bool test(size_t bit) const
		{
			if( bit >= N ) throw dstd::out_of_range();
			return this->get_bit(bit);
		}
		
		
		bool any() const
		{
			return ! this->none();
		}
		
		
		bool none() const
		{
			const size_t size = bitset::data_size();
			unsigned char mask = static_cast<unsigned char>(-1);
			if( (N % CHAR_BIT) != 0 ) mask >>= (CHAR_BIT - (N % CHAR_BIT));
			for(size_t i = 0; i < size; ++i)
			{
				if( (this->data[i] & mask) != 0 ) return false;
				
				if( i == 0 ) mask = -1;
			}
			return true;
		}
		
		
		size_t count() const
		{
			size_t n = 0;
			for(size_t i = 0; i < N; ++i)
			{
				if( this->get_bit(i) ) ++n;
			}
			return n;
		}
		
		
		size_t size() const
		{
			return N;
		}
		
		
		bitset& operator&= (const bitset& x)
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] &= x.data[i];
			}
			return *this;
		}
		
		
		bitset& operator|= (const bitset& x)
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] |= x.data[i];
			}
			return *this;
		}
		
		
		bitset& operator^= (const bitset& x)
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] ^= x.data[i];
			}
			return *this;
		}
		
		
		bitset operator~ () const
		{
			bitset res(*this);
			res.flip();
			return res;
		}
		
		
		bitset& operator<<= (size_t n)
		{
			if( n >= this->size() )
			{
				this->reset();
			}
			else if( n > 0 )
			{
				const size_t bit_shift = n % CHAR_BIT;
				const size_t byte_shift = n / CHAR_BIT;
				
				unsigned char* from = this->data;
				unsigned char* to = (from - 1) - byte_shift;
				unsigned char* end = this->data + bitset::data_size();
				
				while( to < end )
				{
					if( to >= this->data )
					{
						if( from < end )
						{
							*to |= ( *from >> (CHAR_BIT - bit_shift) );
						}
						else
						{
							*to &= ( static_cast<unsigned char>(-1) << bit_shift );
						}
					}
					
					++to;
					
					if( to >= this->data )
					{
						if( from < end )
						{
							*to = (*from << bit_shift);
						}
						else if( to < end )
						{
							*to = 0;
						}
					}
					
					++from;
				}
			}
			
			return *this;
		}
		
		
		bitset& operator>>= (size_t n)
		{
			if( n >= this->size() )
			{
				this->reset();
			}
			else if( n > 0 )
			{
				const size_t bit_shift = n % CHAR_BIT;
				const size_t byte_shift = n / CHAR_BIT;
				
				unsigned char* end = this->data + bitset::data_size();
				unsigned char* from = end - 1;
				unsigned char* to = (from + 1) + byte_shift;
				
				while( to >= this->data )
				{
					if( to < end )
					{
						if( from >= this->data )
						{
							*to |= ( *from << (CHAR_BIT - bit_shift) );
						}
						else
						{
							*to &= ( static_cast<unsigned char>(-1) >> bit_shift );
						}
					}
					
					--to;
					
					if( to < end )
					{
						if( from >= this->data )
						{
							*to = (*from >> bit_shift);
						}
						else if( to >= this->data )
						{
							*to = 0;
						}
					}
					
					--from;
				}
			}
			
			return *this;
		}
		
		
		bitset operator<< (size_t n) const
		{
			bitset x(*this);
			x <<= n;
			return x;
		}
		
		
		bitset operator>> (size_t n) const
		{
			bitset x(*this);
			x >>= n;
			return x;
		}
		
		
		bitset& set()
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] = -1;
			}
			return *this;
		}
		
		
		bitset& set(size_t bit, bool value = true)
		{
			if( bit >= N ) throw dstd::out_of_range();
			reference bit_ref = this->get_reference(bit);
			bit_ref = value;
			return *this;
		}
		
		
		/// Set all bits to false
		bitset& reset()
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] = 0;
			}
			return *this;
		}
		
		
		/// Set specified bit to false
		bitset& reset(size_t bit)
		{
			if( bit >= N ) throw dstd::out_of_range();
			reference bit_ref = this->get_reference(bit);
			bit_ref = false;
			return *this;
		}
		
		
		/// Flip the value of all bits
		bitset& flip()
		{
			const size_t size = bitset::data_size();
			for(size_t i = 0; i < size; ++i)
			{
				this->data[i] = ~(this->data[i]);
			}
			return *this;
		}
		
		
		/// Flips the value of the specified bit
		bitset& flip(size_t bit)
		{
			if( bit >= N ) throw dstd::out_of_range();
			reference bit_ref = this->get_reference(bit);
			bit_ref.flip();
			return *this;
		}
		
		
		/// Converts
		template <class Character, class Traits, class Allocator>
		std::basic_string<Character, Traits, Allocator> to_string() const
		{
			std::basic_string<Character,Traits,Allocator> s( N, '0' );
			for(size_t i = 0; i < N; ++i)
			{
				if( this->test(i) ) s[(N-1)-i] = '1';
			}
			return s;
		}
		
		
		unsigned long to_ulong() const
		{
			if( this->none() ) return 0;
			
			if( std::numeric_limits<unsigned long>::digits < N )
			{
				for(size_t i = std::numeric_limits<unsigned long>::digits; i < N; ++i)
				{
					if( this->test(i) ) throw dstd::overflow_error();
				}
			}
			
			const size_t size = bitset::data_size();
			unsigned char mask(-1);
			if( (N % CHAR_BIT) != 0 ) mask >>= (CHAR_BIT - (N % CHAR_BIT));
			
			unsigned long result = 0;
			for(size_t i = 0; i < size; ++i)
			{
				result <<= CHAR_BIT;
				result |= static_cast<unsigned long>( this->data[i] & mask );
				
				if( i == 0 ) mask = -1;
			}
			
			return result;
		}
		
		
	private:
		
		/// Returns the number of elements in the data array
		static size_t data_size()
		{
			return (N / CHAR_BIT) + (N % CHAR_BIT > 0);
		}
		
		
		/// Returns the value of the specified bit
		bool get_bit(size_t bit) const
		{
			const unsigned char mask = 1 << (bit % CHAR_BIT);
			return ((this->data[(bitset::data_size()-1) - (bit / CHAR_BIT)] & mask) == mask);
		}
		
		
		/// Returns a reference to the specified bit
		reference get_reference(size_t bit)
		{
			return reference( this->data[(bitset::data_size()-1) - (bit / CHAR_BIT)], (bit % CHAR_BIT) );
		}
		
		
		/// The bits.
		/// Sufficient in size to hold N bits, stored contiguously from the left-most bit.
		unsigned char data[ (N / CHAR_BIT) + (N % CHAR_BIT > 0) ];
};



template <size_t N>
class dstd::bitset<N>::reference
{
	public:
		
		~reference() {}
		
		reference& operator= (bool value)
		{
			if( value )
			{
				this->c = this->c | this->mask;
			}
			else
			{
				this->c = this->c & ~(this->mask);
			}
			return *this;
		}
		
		reference& operator= (const reference& x)
		{
			(*this) = bool(x);
			return *this;
		}
		
		operator bool() const
		{
			return ( ( this->c & this->mask ) == this->mask );
		}
		
		bool operator~ () const
		{
			return ! bool(*this);
		}
		
		reference& flip()
		{
			(*this) = ~(*this);
			return *this;
		}
	
	
	private:
		
		reference(unsigned char& data, size_t bit_index)
			: c(data), mask(1 << bit_index)
		{}
		
		unsigned char& c;
		unsigned char mask;
	
	
	friend class dstd::bitset<N>;
};



template <size_t N>
dstd::bitset<N> operator& (const dstd::bitset<N>& lhs, const dstd::bitset<N>& rhs)
{
	dstd::bitset<N> res(lhs);
	res &= rhs;
	return res;
}



template <size_t N>
dstd::bitset<N> operator| (const dstd::bitset<N>& lhs, const dstd::bitset<N>& rhs)
{
	dstd::bitset<N> res(lhs);
	res |= rhs;
	return res;
}



template <size_t N>
dstd::bitset<N> operator^ (const dstd::bitset<N>& lhs, const dstd::bitset<N>& rhs)
{
	dstd::bitset<N> res(lhs);
	res ^= rhs;
	return res;
}


// TODO: stream operators



#endif
