#ifndef DSTD_SSTREAM_HXX
#define DSTD_SSTREAM_HXX



#include "ios.hxx"
#include "memory.hxx"
#include "streambuf.hxx"
#include "string.hxx"



namespace dstd
{
	template <class Character, class Traits = dstd::char_traits<Character>, class Allocator = dstd::allocator<Character> > class basic_stringbuf;
	typedef basic_stringbuf<char> stringbuf;
	typedef basic_stringbuf<wchar_t> wstringbuf;
}



template <class Character, class Traits, class Allocator>
class dstd::basic_stringbuf : public dstd::basic_streambuf<Character, Traits>
{
	public:
		
		typedef Character char_type;
		typedef Traits traits_type;
		typedef typename traits_type::int_type int_type;
		typedef typename traits_type::pos_type pos_type;
		typedef typename traits_type::off_type off_type;
		typedef Allocator allocator_type;
		
		
	protected:
		
		typedef dstd::basic_string<Character, Traits, Allocator> string_type;
	
	
	public:
		
		explicit basic_stringbuf(dstd::ios_base::openmode open_mode = dstd::ios_base::in | dstd::ios_base::out)
			: data(""), mode(open_mode)
		{
			this->update(0, 0);
		}
		
		
		explicit basic_stringbuf(const string_type& s, dstd::ios_base::openmode open_mode = dstd::ios_base::in | dstd::ios_base::out)
			: data(s), mode(open_mode)
		{
			if( this->is_mode(dstd::ios_base::trunc) )
			{
				this->str("");
			}
			else if( this->is_mode(dstd::ios_base::ate) )
			{
				this->update(this->data.size(), this->data.size());
			}
			else if( this->is_mode(dstd::ios_base::app) )
			{
				this->update(0, this->data.size());
			}
			else
			{
				this->update(0, 0);
			}
		}
		
		
		virtual ~basic_stringbuf() {}
		
		
		/// Returns the current contents of the buffer as a string.
		string_type str() const
		{
			if( this->pptr() != 0 )
			{
				// Stream is write-able, so characters may have been put...
				
				if( this->pptr() > this->egptr() )
				{
					// Characters have been put beyond the current get area.
					// Return all the characters from the start of the buffer to the last put character.
					return string_type(this->pbase(), this->pptr());
				}
				else
				{
					// Characters are being put inside the current get area.
					// Return the entire get area.
					return string_type(this->pbase(), this->egptr());
				}
			}
			else
			{
				// Stream is read-only.
				return this->data;
			}
		}
		
		
		/// Replaces the current contents of the buffer with \p s.
		void str(const string_type& s)
		{
			this->data.assign(s);
			if( this->is_mode(dstd::ios_base::app) )
			{
				this->update(0, this->data.size());
			}
			else
			{
				this->update(0, 0);
			}
		}
	
	
	protected:
		
		virtual int_type underflow()
		{
			if( this->gptr() == 0 )
			{
				// not open for reading, so failure
				return traits_type::eof();
			}
			else if( this->gptr() < this->egptr() )
			{
				// there's a character to read
				return this->sgetc();
			}
			else if( this->egptr() < this->pptr() )
			{
				// characters have been inserted since the last time egptr was updated
				this->setg(this->eback(), this->gptr(), this->pptr());
				return this->sgetc();
			}
			else
			{
				// failure
				return traits_type::eof();
			}
		}
		
		
		virtual int_type pbackfail(int_type i = traits_type::eof())
		{
			if( this->gptr() > this->eback() )
			{
				// There's space to back-track.
				
				if( i == traits_type::eof() )
				{
					// Just need to back-track, no character to return.
					this->gbump(-1);
					return traits_type::not_eof(i);
				}
				else if( traits_type::eq(traits_type::to_char_type(i), this->gptr()[-1]) )
				{
					// The character to put-back matches the previous one in the stream.
					this->gbump(-1);
					return this->sgetc();
				}
				else if( this->is_mode(dstd::ios_base::out) )
				{
					// The character to put-back need to overwrite this previous one in the stream,
					// and we can do that because we're in write mode.
					this->gbump(-1);
					*(this->gptr()) = traits_type::to_char_type(i);
					return i;
				}
				else
				{
					// The character to put-back does not match the previous on in the stream,
					// and we can't write over it.
					return traits_type::eof();
				}
			}
			else
			{
				// Not in writing mode, or at the start of the buffer
				return traits_type::eof();
			}
		}
		
		
		virtual int_type overflow(int_type i = traits_type::eof())
		{
			if( this->pptr() == 0 )
			{
				// Not open for writing, so failure.
				return traits_type::eof();
			}
			else if( traits_type::eq_int_type(i, traits_type::eof()) )
			{
				// No character to append.
				return traits_type::not_eof(i);
			}
			else
			{
				// Open for writing.
				
				if( this->pptr() < this->epptr() )
				{
					// There's space to insert another character.
					this->data.push_back( traits_type::to_char_type(i) );
					this->pbump(1);
					return i;
				}
				else if( this->pptr() == this->epptr() )
				{
					// The string is at capacity and a reallocation is about to occur.
					this->data.push_back( traits_type::to_char_type(i) );
					this->update( (this->gptr() - this->eback()), (this->pptr() - this->eback()) );
					return i;
				}
				else
				{
					// pptr > epptr
					// This should not occur.
					// But to be safe, indicate failure.
					return traits_type::eof();
				}
			}
		}
		
		
		// Not currently implemented
		// Not sure if it's necessary (apparently other implementations don't use it
		//virtual std::basic_streambuf<CharT, Traits>* setbuf( char_type* s, std::streamsize n )
		
		
		virtual pos_type seekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which = ios_base::in | ios_base::out)
		{
			off_type g_off = ((which & ios_base::in)  == ios_base::in ) ? this->get_seekoff(off, dir, ios_base::in)  : 0;
			off_type p_off = ((which & ios_base::out) == ios_base::out) ? this->get_seekoff(off, dir, ios_base::out) : 0;
			
			if( this->is_mode(ios_base::in)  && g_off != 0 ) return pos_type(off_type(-1));
			if( this->is_mode(ios_base::out) && p_off != 0 ) return pos_type(off_type(-1));
			
			if( g_off < 0 ) return pos_type(off_type(-1));
			if( p_off < 0 ) return pos_type(off_type(-1));
			
			this->update(g_off, p_off);
		}
		
		
	private:
		
		/// Not Implemented. No copy construction allowed.
		basic_stringbuf& operator=(const basic_stringbuf& x);
		
		
		/// Determines the absolute offset from the beginning for a given relative offset.
		/// \param off The relative offset from \p dir.
		/// \param dir The position from which \p off is measeured.
		/// \param which Must be either dstd::ios_base::in or dstd::ios_base::out.
		pos_type get_seekoff(off_type off, dstd::ios_base::seekdir dir, dstd::ios_base::openmode which)
		{
			if( dir == ios_base::beg )
			{
				return off;
			}
			
			const char_type* start = (which == ios_base::in) ? this->eback() : this->pbase();
			
			if( dir == ios_base::cur )
			{
				const char_type* curr = (which == ios_base::in) ? this->gptr() : this->pptr();
				return (curr - start) + off;
			}
			else // so dir == ios_base::end
			{
				const char_type* end = start + this->data.size();
				return (end - start) + off;
			}
		}
		
		
		/// Checks whether this buffer's mode includes a given open mode.
		/// \param m The open mode to check against.
		bool is_mode(const dstd::ios_base::openmode& m) const
		{
			return ( (m & this->mode) == m );
		}
	
		
		/// Updates all the internal pointers.
		/// \param get_current The new absolute offset for the current get pointer
		/// \param put_current The new absolute offset for the current put pointer
		void update(off_type get_current, off_type put_current)
		{
			char_type* base = this->data.data();
			
			if( this->is_mode(dstd::ios_base::in) )
			{
				this->setg(base, base+get_current, base+this->data.size());
			}
			else
			{
				this->setg(0, 0, 0);
			}
			
			if( this->is_mode(dstd::ios_base::out) )
			{
				this->setp(base, base+this->data.capacity());
				this->pbump(put_current);
			}
			else
			{
				this->setp(0, 0);
			}
		}
		
		
		dstd::ios_base::openmode mode;
		dstd::basic_string<Character, Traits, Allocator> data;
};


#endif
