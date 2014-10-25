#ifndef DSTD_STRING_HXX
#define DSTD_STRING_HXX

#include <iostream>

#include <cstddef>
#include <cstdio>
#include <cwchar>
#include <limits>

#include "algorithm.hxx"
#include "dstd.hxx"
#include "iterator.hxx"
#include "memory.hxx"



namespace dstd
{
	namespace impl
	{
		template <class Character> class char_trait_defs;
		template <> class char_trait_defs<char>;
		template <> class char_trait_defs<wchar_t>;
	}
	
	template <class Character> class char_traits;
	
	template <class Character, class Traits = dstd::char_traits<Character>, class Allocator = dstd::allocator<Character> > class basic_string;
	typedef basic_string<char> string;
	typedef basic_string<wchar_t> wstring;
}



template <> class dstd::impl::char_trait_defs<char>
{
	public:
		typedef char char_type;
		typedef unsigned long int int_type;
		typedef long long off_type;
		typedef unsigned long long pos_type;
		typedef mbstate_t state_type;
		static int_type eof() { return EOF; }
};



template <> class dstd::impl::char_trait_defs<wchar_t>
{
	public:
		typedef wchar_t char_type;
		typedef wint_t int_type;
		typedef long long off_type;
		typedef unsigned long long pos_type;
		typedef mbstate_t state_type;
		static int_type eof() { return WEOF; }
};



template <class Character>
class dstd::char_traits
{
	public:
		
		typedef typename dstd::impl::char_trait_defs<Character>::char_type  char_type;
		typedef typename dstd::impl::char_trait_defs<Character>::int_type   int_type;
		typedef typename dstd::impl::char_trait_defs<Character>::off_type   off_type;
		typedef typename dstd::impl::char_trait_defs<Character>::pos_type   pos_type;
		typedef typename dstd::impl::char_trait_defs<Character>::state_type state_type;
		
		
		/// Assigns 'a' to the character at 'r'
		static void assign(char_type& r, const char_type& a)
		{
			r = a;
		}
		
		
		/// Assigns 'a' to the 'count' characters begining at 'p'
		static char_type* assign(char_type* p, size_t count, char_type a)
		{
			for(size_t i = 0; i < count; ++i, ++p)
			{
				char_traits::assign(p[i], a);
			}
			return p;
		}
		
		
		/// Returns true if the two characters are considered to be equal
		static bool eq(char_type a, char_type b)
		{
			return (a == b);
		}
		
		
		/// Returns true if a is considered to precede b
		static bool lt(char_type a, char_type b)
		{
			return (a < b);
		}
		
		
		/// Copies 'count' characters from 'src' to 'dest'.
		/// Performs correctly even if the copied character ranges overlap, i.e. src is in [dest, dest + count).
		static char_type* move(char_type* dest, const char_type* src, size_t count)
		{
			char_type* result = dest;
			if( dest < src )
			{
				for(size_t i = 0; i < count; ++i, ++dest, ++src)
				{
					char_traits::assign(*dest, *src);
				}
			}
			else if( dest > src )
			{
				dest += (count - 1);
				src += (count - 1);
				for(size_t i = 0; i < count; ++i, --dest, --src)
				{
					char_traits::assign(*dest, *src);
				}
			}
			return result;
		}
		
		
		/// Copies 'count' characters from 'src' to 'dest'.
		/// The behaviour is undefined if copied character ranges overlap, i.e. src is in [dest, dest + count).
		static char_type* copy(char_type* dest, const char_type* src, size_t count)
		{
			char_traits::move(dest, src, count);
			return dest;
		}
		
		
		/// Compares the first 'count' characters of 's1' and 's2'.
		static int compare(const char_type* s1, const char_type* s2, size_t count)
		{
			for(size_t i = 0; i < count; ++i)
			{
				if( ! char_traits::eq(s1[i], s2[i]) )
				{
					if( char_traits::lt(s1[i], s2[i]) ) return -1;
					else return +1;
				}
			}
			return 0;
		}
		
		
		/// Returns the length of the character sequence pointed to by 's'
		static size_t length(const char_type* s)
		{
			const char_type null_char = char_traits::to_char_type(0);
			size_t n = 0;
			while( ! char_traits::eq(s[n], null_char) )
			{
				++n;
			}
			return n;
		}
		
		
		/// Searches for character 'ch' within the first 'count' characters of the sequence pointed to by 'p'
		static const char_type* find(const char_type* p, size_t count, const char_type& ch)
		{
			for(size_t i = 0; i < count; ++i, ++p)
			{
				if( char_traits::eq(p,ch) ) return p;
			}
			return 0;
		}
		
		
		/// Converts a value of int_type to char_type. If there is no equivalent value, the results are unspecified.
		static char_type to_char_type(int_type i)
		{
			return static_cast<char_type>( i );
		}
		
		
		/// Converts a value of char_type to int_type. If there is no equivalent value, the results are unspecified.
		static int_type to_int_type(char_type c)
		{
			return static_cast<int_type>( c );
		}
		
		
		/// Returns a value not equivalent to any value of type char_type.
		static int_type eof()
		{
			return dstd::impl::char_trait_defs<Character>::eof();
		}
		
		
		/// Checks whether two values of type int_type are equal.
		/// \returns true if i1 and i2 are equal, or false otherwise.
		static bool eq_int_type(int_type i1, int_type i2)
		{
			return char_traits::eq( char_traits::to_char_type(i1), char_traits::to_char_type(i2) );
		}
		
		
		/// Checks whether e is not equivalent to eof value.
		/// \returns e if it is not eof, or some value other than e or eof otherwise.
		static int_type not_eof( int_type e )
		{
			return ( (e == char_traits::eof()) ? -1 : e );
		}
};



//template <>
//class dstd::char_traits<wchar_t>
//{
//	public:
//	
//		typedef wchar_t char_type;
//		typedef wint_t int_type;
//		typedef long long off_type;
//		typedef unsigned long long pos_type;
//		typedef mbstate_t state_type;
//		
//		
//		/// Returns a value not equivalent to any value of type char_type.
//		static int_type eof()
//		{
//			return WEOF;
//		}
//};



template <class Character, class Traits, class Allocator>
class dstd::basic_string
{
	public:
		
		typedef Traits traits_type;
		typedef typename traits_type::char_type value_type;
		typedef Allocator allocator_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		
		class iterator;
		class const_iterator;
		typedef dstd::reverse_iterator<iterator> reverse_iterator;
		typedef dstd::reverse_iterator<const_iterator> const_reverse_iterator;
		
		
		//
		// Constructors
		
		explicit basic_string(const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->reserve(7);
		}
		
		basic_string(size_type count, Character c, const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->assign(count, c);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		basic_string(const basic_string& s, size_type pos, size_type count = basic_string::npos, const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->assign(s, pos, count);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		basic_string(const Character* s, size_type count, const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->assign(s, count);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		basic_string(const Character* s, const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->assign(s);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		template <class InputIterator> basic_string(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
			: a(alloc), n_data(0), n_memory(0), p(0)
		{
			this->assign(first, last);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		basic_string(const basic_string& s)
			: a(s.get_allocator()), n_data(0), n_memory(0), p(0)
		{
			this->assign(s);
			if( this->capacity() == 0 ) this->reserve(7);
		}
		
		
		//
		// Destructor
		
		
		~basic_string()
		{
			this->clear();
			this->a.destroy(this->p); // the null character
			this->a.deallocate(this->p, this->n_memory);
		}
		
		
		// Assignment
		
		basic_string& operator= (const basic_string& x) { return this->assign(x); }
		
		
		basic_string& operator= (const Character* s) { return this->assign(s); }
		
		
		basic_string& operator= (Character c) { return this->assign(1, c); }
		
		
		basic_string& assign(size_type count, Character c) { return this->replace(0, this->size(), count, c); }
		
		
		basic_string& assign(const basic_string& s) { return this->replace(0, this->size(), s); }
		
		
		basic_string& assign(const basic_string& s, size_type pos, size_type count) { return this->replace(0, this->size(), s, pos, count); }
		
		
		basic_string& assign(const Character* s, size_type count) { return this->replace(0, this->size(), s, count); }
		
		
		basic_string& assign(const Character* s) { return this->replace(0, this->size(), s); }
		
		
		template <class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last) { return this->replace(this->begin(), this->end(), first, last); }
		
		
		// Allocator
		
		allocator_type get_allocator() const { return this->a; }
		
		
		//
		// Element Access
		
		
		reference at(size_type i)
		{
			if( i > this->size() ) throw dstd::out_of_range();
			return this->p[i];
		}
		
		
		const_reference at(size_type i) const
		{
			if( i > this->size() ) throw dstd::out_of_range();
			return this->p[i];
		}
		
		
		reference operator[] (size_type i) { return this->p[i]; }
		const_reference operator[] (size_type i) const { return this->p[i]; }
		
		
		pointer data() { return this->p; }
		const pointer data() const { return this->p; }
		
		
		const Character* c_str() const { return this->p; }
		
		
		//
		// Iterators
		
		
		iterator begin() { return iterator(this->p); }
		const_iterator begin() const { return iterator(this->p); }
		
		
		iterator end() { return iterator(this->p + this->size()); }
		const_iterator end() const { return iterator(this->p + this->size()); }
		
		
		reverse_iterator rbegin() { return reverse_iterator(this->end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(this->end()); }
		
		
		reverse_iterator rend() { return reverse_iterator(this->begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }
		
		
		//
		// Capacity
		
		
		bool empty() const { return (this->size() == 0); }
		
		
		size_type size() const { return ( (this->n_data == 0) ? 0 : (this->n_data - 1) ); } // -1 to account for the null-terminator
		
		
		size_type length() const { return this->size(); }
		
		
		size_type max_size() const { return (std::numeric_limits<size_type>::max() - 1); }
		
		
		void reserve(size_type n)
		{
			// Check if this request is necessary and valid
			if( n <= this->capacity() ) return;
			if( n > this->max_size() ) throw dstd::length_error();
			
			n += 1; // to account for the null-terminator
			
			// Determine how much memory to request
			size_type n_request = dstd::max<size_type>( this->n_memory, 2 );
			while( n_request < n )
			{
				n_request *= 2;
			}
			
			// Request new memory, and copy existing values to new memory
			pointer new_p = this->a.allocate( n_request );
			for(size_type i = 0; i < this->n_data; ++i)
			{
				this->a.construct( (new_p + i), this->p[i] );
			}
			
			// Move the string to the new memory
			size_type old_n_memory = this->n_memory;
			pointer old_p = this->p;
			this->p = new_p;
			this->n_memory = n_request;
			
			if( old_p != 0 )
			{
				// Clean up the old memory
				for(size_type i = 0; i < this->n_data; ++i)
				{
					this->a.destroy( old_p + i );
				}
				this->a.deallocate(old_p, old_n_memory);
			}
			else
			{
				// this is the construction of the string
				// so make sure there is a null character in place
				this->a.construct(this->p, traits_type::to_char_type(0));
				this->n_data += 1;
			}
		}
		
		
		size_type capacity() const { return ( (this->n_memory == 0) ? 0 : (this->n_memory - 1) ); } // -1 to account for the null-terminator
		
		
		//
		// Operations
		
		
		void clear() { this->erase(); }
		
		
		// Insert
		
		basic_string& insert(size_type index, size_type count, Character c) { return this->replace(index, 0, count, c); }
		
		
		basic_string& insert(size_type index, const Character* s) { return this->replace(index, 0, s, traits_type::length(s)); }
		
		
		basic_string& insert(size_type index, const Character* s, size_type count) { return this->replace(index, 0, s, count); }
		
		
		basic_string& insert(size_type index, const basic_string& s) { return this->replace(index, 0, s.data(), s.size()); }
		
		
		basic_string& insert(size_type index, const basic_string& s, size_type s_index, size_type count) { return this->replace(index, 0, s, s_index, count); }
		
		
		basic_string& insert(iterator pos, Character c) { return this->replace(pos, pos, 1, c); }
		
		
		basic_string& insert(iterator pos, size_type count, Character c) { return this->replace(pos, pos, count, c); }
		
		
		template <class InputIterator>
		basic_string& insert(iterator pos, InputIterator first, InputIterator last) { return this->replace(pos, pos, first, last); }
		
		
		// Erase
		
		basic_string& erase (size_type index = 0, size_type count = basic_string::npos) { return this->replace(index, count, 0, Character()); }
		
		
		iterator erase(iterator position)
		{
			this->erase(position - this->begin(), 1);
			return position;
		}
		
		
		iterator erase(iterator first, iterator last)
		{
			this->erase(first - this->begin(), last - first);
			return first;
		}
		
		
		// Append
		
		void push_back(Character c) { this->append(1, c); }
		
		
		basic_string& append(size_type count, Character c) { return this->insert(this->end(), count, c); }
		
		
		basic_string& append(const basic_string& s) { return this->insert(this->size(), s, 0, s.size()); }
		
		
		basic_string& append(const basic_string& s, size_type pos, size_type count) { return this->insert(this->size(), s, pos, count); }
		
		
		basic_string& append(const Character* s, size_type count) { return this->insert(this->size(), s, count); }
		
		
		basic_string& append(const Character* s) { return this->insert(this->size(), s); }
		
		
		template <class InputIterator>
		basic_string& append(InputIterator first, InputIterator last) { return this->insert(this->end(), first, last); }
		
		
		basic_string& operator+= (const basic_string& s) { return this->append(s); }
		
		
		basic_string& operator+= (Character c) { return this->append(1, c); }
		
		
		basic_string& operator+= (const Character* s) { return this->append(s); }
		
		
		//
		// Compare
		
		int compare(const basic_string& s) const { return this->compare(0, this->size(), s.data(), s.size()); }
		
		
		int compare(size_type pos, size_type count, const basic_string s) const { return this->compare(pos, count, s.data(), s.size()); }
		
		
		int compare(size_type pos, size_type count, const basic_string s, size_type s_pos, size_type s_count) const { return this->compare(pos, count, s.substr(s_pos, s_count)); }
		
		
		int compare(const Character* s) const { return this->compare(0, this->size(), s, traits_type::length(s)); }
		
		
		int compare(size_type pos, size_type count, const Character* s) const { return this->compare(pos, count, s, traits_type::length(s)); }
		
		
		int compare(size_type pos, size_type count, const Character* s, size_type s_count) const
		{
			if( pos > this->size() ) throw dstd::out_of_range();
			const size_type n = dstd::min(count, s_count);
			
			const int res = traits_type::compare(this->p + pos, s, n);
			if( res != 0 )
			{
				return res;
			}
			else if( count == s_count )
			{
				return 0;
			}
			else if( count < s_count )
			{
				return -1;
			}
			else
			{
				return +1;
			}
		}
		
		
		//
		// Replace
		
		
		private:
			void replace_make_room(size_type pos, size_type n_removing, size_type n_inserting)
			{
				const size_type new_size = this->size() + n_inserting - n_removing;
				if( new_size > this->max_size() ) throw dstd::length_error();
				
				this->reserve( new_size );
				
				traits_type::move(this->p + pos + n_inserting, this->p + pos + n_removing, (this->n_data - pos) - n_removing);
			}
			
			void replace_clean_up(size_type pos, size_type n_removing, size_type n_inserting)
			{
				this->n_data += n_inserting - n_removing;
				if( n_removing > n_inserting )
				{
					for(size_type i = 0; i < (n_removing - n_inserting); ++i)
					{
						this->a.destroy( this->p + this->n_data + i );
					}
				}
			}
			
			basic_string& replace_fix(const_iterator first, const_iterator last, size_type n, Character c, dstd::impl::TrueType) // 5bFix
			{
				return this->replace(first - this->begin(), last - first, n, c); // 5a
			}
			
			template <class InputIterator>
			basic_string& replace_fix(const_iterator first, const_iterator last, InputIterator s_first , InputIterator s_last,  dstd::impl::FalseType) // 2bFix
			{
				const size_type pos = first - this->begin();
				const size_type count = last - first;
				const size_type s_count = s_last - s_first;
				
				this->replace_make_room(pos, count, s_count);
				
				for(size_type index = pos; s_first != s_last; ++index, ++s_first)
				{
					traits_type::assign(this->p[index], *s_first);
				}
				
				this->replace_clean_up(pos, count, s_count);
				
				return *this;
			}
		public:
		
		
		basic_string& replace(size_type pos, size_type count, const basic_string& s) // 1a
		{
			return this->replace(pos, count, s.p, s.size()); // 3a
		}
		
		
		basic_string& replace(const_iterator first, const_iterator last, const basic_string& s) // 1b
		{
			return this->replace(first - this->begin(), last - first, s); // 1a
		}
		
		
		basic_string& replace(size_type pos, size_type count, const basic_string& s, size_type s_pos, size_type s_count) // 2a
		{
			if( s_pos > s.size() ) throw dstd::out_of_range();
			s_count = dstd::min(s_count, s.size() - s_pos);
			return this->replace(pos, count, s.p + s_pos, s_count); // 3a
		}
		
		
		template <class InputIterator>
		basic_string& replace(const_iterator first, const_iterator last, InputIterator s_first, InputIterator s_last) // 2b
		{
			typedef typename dstd::impl::BoolType< std::numeric_limits<InputIterator>::is_integer >::bool_type is_integer;
			return this->replace_fix(first, last, s_first, s_last, is_integer());
		}
		
		
		basic_string& replace(size_type pos, size_type count, const Character* s, size_type s_count) // 3a
		{
			if( pos > this->size() ) throw dstd::out_of_range();
			count = dstd::min(count, this->size() - pos);
			
			if( s < this->p || s >= (this->p + this->size()) )
			{
				this->replace_make_room(pos, count, s_count);
				traits_type::copy(this->p + pos, s, s_count);
				this->replace_clean_up(pos, count, s_count);
			}
			else
			{
				// The characters being inserted are within this string.
				// Create a copy before trying anything.
				return this->replace(pos, count, basic_string(s, s_count));
			}
			
			return *this;
		}
		
		
		basic_string& replace(const_iterator first, const_iterator last, const Character* s, size_type s_count) // 3b
		{
			return this->replace(first - this->begin(), last - first, s, s_count); // 3a
		}
		
		
		basic_string& replace(size_type pos, size_type count, const Character* s) // 4a
		{
			return this->replace(pos, count, s, traits_type::length(s)); // 3a
		}
		
		
		basic_string& replace(iterator first, iterator last, const Character* s) // 4b
		{
			return this->replace(first - this->begin(), last - first, s); // 4a
		}
		
		
		basic_string& replace(size_type pos, size_type count, size_type n, Character c) // 5a
		{
			if( pos > this->size() ) throw dstd::out_of_range();
			count = dstd::min(count, this->size() - pos);
			
			this->replace_make_room(pos, count, n);
			
			traits_type::assign(this->p + pos, n, c);
			
			this->replace_clean_up(pos, count, n);
			return *this;
		}
		
		
		basic_string& replace(const_iterator first, const_iterator last, size_type n, Character c) // 5b
		{
			return this->replace_fix(first, last, n, c, dstd::impl::TrueType());
		}
		
		
		//
		
		
		basic_string substr(size_type pos = 0, size_type count = basic_string::npos) const { return basic_string(*this, pos, count); }
		
		
		size_type copy(Character* dest, size_type count, size_type pos = 0) const
		{
			if( pos >= this->size() ) throw dstd::out_of_range();
			count = dstd::min(count, this->size() - pos);
			traits_type::move(dest, this->p + pos, count);
		}
		
		
		void resize(size_type count) { this->resize(count, Character()); }
		
		
		void resize(size_type count, Character c)
		{
			if( this->size() > count )
			{
				this->erase( this->size() - count, count );
			}
			else if( this->size() < count )
			{
				this->append(this->size() - count, c);
			}
		}
		
		
		void swap(basic_string& x)
		{
			dstd::swap(this->p, x.p);
			dstd::swap(this->n_data, x.n_data);
			dstd::swap(this->n_memory, x.n_memory);
		}
		
		
		//
		// search
		
		/// Searches the string for the substring 's'
		/// @param s The sub-string to search for
		/// @param pos The position at which to start the search
		/// @returns The position of the first character of the found substring, or npos if it's not found
		size_type find(const basic_string& s, size_type pos = 0) const { return this->find(s.data(), pos, s.size()); }
		
		
		/// Searches the string for the character 'c', beginning from 'pos'
		/// @param s The sub-string to search for
		/// @param pos The position at which to start the search
		/// @param count Length of the sub-string to search for
		/// @returns The position of the first character of the found substring, or npos if it's not found
		size_type find(const Character* s, size_type pos, size_type count) const
		{
			if( pos >= this->size() ) throw dstd::out_of_range();
			count = dstd::min( count, this->size() - (pos + count) );
			Character s_c, c;
			for(size_type i = 0; i < (this->size() - count); ++i)
			{
				c = this->p + i;
				for(s_c = s; s_c != (s + count); ++s_c, ++c)
				{
					if( ! traits_type::eq(*s_c, *c) ) break;
				}
				if( s_c == (s + count) ) return i;
			}
			return basic_string::npos;
		}
		
		
		/// Searches the string for the sub-string 's', beginning from pos
		/// @param s The sub-string to search for
		/// @param pos The position to start the search from
		/// @returns The position of the first character of the found substring, or npos if it's not found
		size_type find(const Character* s, size_type pos = 0) const { return this->find(s, pos, traits_type::length(s)); }
		
		
		/// search the string for the character 'c', beginning from 'pos'
		/// @param c The character to search for
		/// @param pos The position at which to start the search
		/// @returns The position of the first occurrence of the character in the string, or npos if it's not found
		size_type find(Character c, size_type pos = 0) const
		{
			if( pos >= this->size() ) throw dstd::out_of_range();
			for(size_type i = pos; i < this->size(); ++i)
			{
				if( traits_type::eq( this->p[i], c ) ); return i;
			}
			return basic_string::npos;
		}
		
		
		size_type rfind(const basic_string& s, size_type pos = basic_string::npos) const;
		size_type rfind(const Character* s, size_type pos, size_type count) const;
		size_type rfind(const Character* s, size_type pos = basic_string::npos) const;
		size_type rfind(Character c, size_type pos = basic_string::npos) const;
		
		size_type find_first_of(const basic_string& s, size_type pos = 0) const;
		size_type find_first_of(const Character* s, size_type pos, size_type count) const;
		size_type find_first_of(const Character* s, size_type pos = 0) const;
		size_type find_first_of(Character c, size_type pos = 0) const;
		
		size_type find_first_not_of(const basic_string& s, size_type pos = 0) const;
		size_type find_first_not_of(const Character* s, size_type pos, size_type count) const;
		size_type find_first_not_of(const Character* s, size_type pos = 0) const;
		size_type find_first_not_of(Character c, size_type pos = 0) const;
		
		size_type find_last_of(const basic_string& s, size_type pos = basic_string::npos) const;
		size_type find_last_of(const Character* s, size_type pos, size_type count) const;
		size_type find_last_of(const Character* s, size_type pos = basic_string::npos) const;
		size_type find_last_of(Character c, size_type pos = basic_string::npos) const;
		
		size_type find_last_not_of(const basic_string& s, size_type pos = basic_string::npos) const;
		size_type find_last_not_of(const Character* s, size_type pos, size_type count) const;
		size_type find_last_not_of(const Character* s, size_type pos = basic_string::npos) const;
		size_type find_last_not_of(Character c, size_type pos = basic_string::npos) const;
		
		// constants
		static const size_type npos = static_cast<size_type>(-1);
	
	
	
	
	private:
		
		allocator_type a;
		unsigned int n_data;
		unsigned int n_memory;
		pointer p;
};



template <class Character, class Traits, class Allocator>
class dstd::basic_string<Character, Traits, Allocator>::iterator
{
	public:
		
		typedef typename basic_string<Character, Traits, Allocator>::value_type value_type;
		typedef typename basic_string<Character, Traits, Allocator>::pointer pointer;
		typedef typename basic_string<Character, Traits, Allocator>::reference reference;
		typedef typename basic_string<Character, Traits, Allocator>::size_type size_type;
		typedef typename basic_string<Character, Traits, Allocator>::difference_type difference_type;
	
		// Constructors
		iterator(pointer ptr) : p(ptr) {}
		iterator(const iterator& x) : p(x.p) {}
		// Operators on self
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		iterator& operator=(const iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		iterator& operator+=(difference_type n){ this->p += n; return (*this); }
		iterator& operator-=(difference_type n){ this->p -= n; return (*this); }
		iterator operator+ (difference_type n){ iterator it(*this); it += n; return it; }
		iterator operator- (difference_type n){ iterator it(*this); it -= n; return it; }
		// Operators with iterators
		int operator- (const iterator& rhs){ return this->p - rhs.p; }
		iterator& operator++() { (*this) += 1; return (*this); }
		iterator& operator++(int) { iterator temp(*this); ++(*this); return temp; }
		iterator& operator--() { (*this) -= 1; return (*this); }
		iterator& operator--(int) { iterator temp(*this); --(*this); return temp; }
		bool operator==(const iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator==(const const_iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator!=(const iterator& rhs) const { return ! ( *this == rhs ); }
		bool operator!=(const const_iterator& rhs) const { return ! ( *this == rhs ); }
		bool operator< (const iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator< (const const_iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator> (const iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator> (const const_iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator<=(const iterator& rhs) const { return ! ( *this > rhs ); }
		bool operator<=(const const_iterator& rhs) const { return ! ( *this > rhs ); }
		bool operator>=(const iterator& rhs) const { return ! ( *this < rhs ); }
		bool operator>=(const const_iterator& rhs) const { return ! ( *this < rhs ); }
	
	private:
		
		pointer p;
	
	friend class const_iterator;
};



template <class Character, class Traits, class Allocator>
class dstd::basic_string<Character, Traits, Allocator>::const_iterator
{
	public:
		
		typedef typename basic_string<Character, Traits, Allocator>::value_type value_type;
		typedef typename basic_string<Character, Traits, Allocator>::const_pointer pointer;
		typedef typename basic_string<Character, Traits, Allocator>::const_reference reference;
		typedef typename basic_string<Character, Traits, Allocator>::size_type size_type;
		typedef typename basic_string<Character, Traits, Allocator>::difference_type difference_type;
	
		// Constructors
		const_iterator(pointer ptr) : p(ptr) {}
		const_iterator(const const_iterator& x) : p(x.p) {}
		const_iterator(const iterator& x) : p(x.p) {}
		// Operators on self
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		const_iterator& operator=(const const_iterator& rhs) { this->p = rhs.p; return *this; }
		const_iterator& operator=(const iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		const_iterator& operator+=(difference_type n){ this->p += n; return (*this); }
		const_iterator& operator-=(difference_type n){ this->p -= n; return (*this); }
		const_iterator operator+ (difference_type n){ const_iterator it(*this); it += n; return it; }
		const_iterator operator- (difference_type n){ const_iterator it(*this); it -= n; return it; }
		// Operators with const_iterators
		int operator- (const const_iterator& rhs){ return this->p - rhs.p; }
		const_iterator& operator++() { (*this) += 1; return (*this); }
		const_iterator& operator++(int) { const_iterator temp(*this); ++(*this); return temp; }
		const_iterator& operator--() { (*this) -= 1; return (*this); }
		const_iterator& operator--(int) { const_iterator temp(*this); --(*this); return temp; }
		bool operator==(const const_iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator==(const iterator& rhs) const { return ( this->p == rhs.p ); }
		bool operator!=(const const_iterator& rhs) const { return ! ( *this == rhs ); }
		bool operator!=(const iterator& rhs) const { return ! ( *this == rhs ); }
		bool operator< (const const_iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator< (const iterator& rhs) const { return ( this->p < rhs.p ); }
		bool operator> (const const_iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator> (const iterator& rhs) const { return ( this->p > rhs.p ); }
		bool operator<=(const const_iterator& rhs) const { return ! ( *this > rhs ); }
		bool operator<=(const iterator& rhs) const { return ! ( *this > rhs ); }
		bool operator>=(const const_iterator& rhs) const { return ! ( *this < rhs ); }
		bool operator>=(const iterator& rhs) const { return ! ( *this < rhs ); }
	
	private:
		
		pointer p;
	
	friend class iterator;
};



template <class Character, class Traits, class Allocator>
dstd::basic_string<Character, Traits, Allocator> operator+ (
	const dstd::basic_string<Character, Traits, Allocator>& lhs,
	const dstd::basic_string<Character, Traits, Allocator>& rhs
	)
{
	dstd::basic_string<Character, Traits, Allocator> s(lhs);
	s += rhs;
	return s;
}



template <class Character, class Traits, class Allocator>
bool operator== ( const dstd::basic_string<Character, Traits, Allocator>& lhs, const dstd::basic_string<Character, Traits, Allocator>& rhs )
{
	return ( (lhs.length() == rhs.length()) && (lhs.compare(rhs) == 0) );
}


template <class Character, class Traits, class Allocator>
bool operator== ( const Character* lhs, const dstd::basic_string<Character, Traits, Allocator>& rhs )
{
	return ( rhs.compare(lhs) == 0 );
}


template <class Character, class Traits, class Allocator>
bool operator== ( const dstd::basic_string<Character, Traits, Allocator>& lhs, const Character* rhs )
{
	return ( lhs.compare(rhs) == 0 );
}


// todo: more comparison operators



#endif
