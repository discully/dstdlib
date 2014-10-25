#ifndef DSTD_STREAMBUF_HXX
#define DSTD_STREAMBUF_HXX



#include "ios.hxx"
#include "string.hxx"



namespace dstd
{
	template <class Character, class Traits> class basic_streambuf;
	typedef basic_streambuf<char, dstd::char_traits<char> > streambuf;
	//typedef basic_streambuf<wchar_t> wstreambuf;
	
	// stuff I don't want to implement myself
	typedef std::locale locale;
	typedef std::streamsize streamsize;
}



template <class Character, class Traits = dstd::char_traits<Character> >
class dstd::basic_streambuf
{
	public:
		
		typedef Character char_type;
		typedef Traits traits_type;
		typedef typename Traits::int_type int_type;
		typedef typename Traits::pos_type pos_type;
		typedef typename Traits::off_type off_type;
		
		virtual ~basic_streambuf() {}
		
		
		// Locales
		
		
		/// Changes the associated locale.
		/// \note Calls imbue(const dstd::locale&) of the most derived class.
		dstd::locale pubimbue(const dstd::locale& new_locale)
		{
			const dstd::locale old_loc = this->getloc();
			this->loc = new_locale;
			this->imbue(this->loc);
			return old_loc;
		}
		
		
		/// Returns the locale associated with this buffer.
		dstd::locale getloc() const { return this->loc; }
		
		
		// Positioning
		
		
		/// Classes may allow replacement of the controlled character sequence (the buffer) with a user-provided array.
		/// \note Calls setbuf(char_type*, dstd::streamsize) of the most derived class
		basic_streambuf* pubsetbuf(char_type* s, dstd::streamsize n)
		{
			return this->setbuf(s, n);
		}
		
		
		/// Sets the position indicator of the input and/or output sequence relative to some other position.
		/// \param off The offset from \p dir to which the stream position should be set.
		/// \param dir The position in the stream relative to which \p off is measured.
		/// \param which Specifies whether the input, output or both positions of the stream should be set.
		/// \note Calls seekoff(off_type,ios_base::seekdir,ios_base::openmode) of the most derived class.
		pos_type pubseekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which = ios_base::in | ios_base::out)
		{
			return this->seekoff(off, dir, which);
		}
		
		
		/// Sets the position indicator of the input and/or output sequence to an absolute position. 
		/// \param pos The absolute position to which the stream should be set.
		/// \param which Specifies whether the input, output or both positions of the stream should be set.
		/// \note Calls seekpos(pos_type,dstd::ios_base::openmode) of the most derived class.
		pos_type pubseekpos(pos_type pos, dstd::ios_base::openmode which = dstd::ios_base::in | dstd::ios_base::out)
		{
			return this->seekpos(pos, which);
		}
		
		
		/// Synchronizes the controlled character sequence (the buffers) with the associated character sequence. 
		/// \note Calls sync() of the most derived class.
		int pubsync()
		{
			return this->sync();
		}
		
		
		// Get Area
		
		
		/// Returns the number of characters available to read.
		dstd::streamsize in_avail()
		{
			if( this->gptr() != 0 && this->gptr() < this->egptr() )
			{
				return (this->egptr() - this->gptr());
			}
			else
			{
				return this->showmanyc();
			}
		}
		
		
		/// Advances the input sequence, then reads one character without advancing again.
		int_type snextc()
		{
			if( this->sbumpc() != traits_type::eof() )
			{
				return this->sgetc();
			}
			else
			{
				return traits_type::eof();
			}
		}
		
		
		/// Reads one character from the input sequence and advances the sequence.
		/// \returns The next character in the input sequence.
		int_type sbumpc()
		{
			if( this->gptr() < this->egptr() )
			{
				const int_type result = this->sgetc();
				this->gbump(1);
				return result;
			}
			else
			{
				return this->uflow();
			}
		}
		
		
		/// Reads one character from the input sequence without advancing the sequence.
		/// \returns The next character in the input sequence.
		int_type sgetc()
		{
			if( this->gptr() < this->egptr() )
			{
				return *(this->gptr());
			}
			else
			{
				return this->underflow();
			}
		}
		
		
		/// Reads characters from the input sequence to a character array.
		/// Calls xsgetn(s, count) of the most derived class.
		/// \param s An array sufficient in size to store at least \p count characters.
		/// \param count The maximum number of characters to read.
		/// \returns The number of characters successfully read.
		dstd::streamsize sgetn(char_type* s, dstd::streamsize count)
		{
			return this->xsgetn(s, count);
		}
		
		
		// Put Area
		
		
		/// Writes one character to the put area and advances the current pointer.
		/// \param c The character to be written to the output sequence.
		/// \returns The integer representation of \p c on success, or traits_type::eof() on failure.
		int_type sputc(char_type c)
		{
			if( this->pptr() < this->epptr() )
			{
				*(this->pptr()) = c;
				this->pbump(1);
				return traits_type::to_int_type(c);
			}
			else
			{
				return this->overflow(c);
			}
		}
		
		
		/// Writes characters from an array to the output sequence.
		/// Writing stops when either \p count characters are written or a call to sputc() would have returned Traits::eof().
		/// \note Calls xsputn() of the most derived class.
		/// \param s Character array of at least \p count size holding the characters to be written.
		/// \param count The maximum number of characters available to be written.
		dstd::streamsize sputn(const char_type* s, dstd::streamsize count)
		{
			return this->xsputn(s, count);
		}
		
		
		// Put Back
		
		
		/// Puts back a character back to the get area.
		int_type sputbackc(char_type c)
		{
			if( this->gptr() > this->eback() && traits_type::eq(c, this->gptr()[-1]) )
			{
				this->gbump(-1);
				return this->sgetc();
			}
			else
			{
				return this->pbackfail( traits_type::to_int_type(c) );
			}
		}
		
		
		/// If a putback position is available in the get area (gptr() > eback()),
		/// then decrements the next pointer (gptr()) and returns the character it now points to.
		/// If a putback position is not available, then calls pbackfail() to back up the input sequence if possible.
		/// The I/O stream function basic_istream::unget is implemented in terms of this function.
		/// \returns The new current character on success, of traits_type::eof() on failure.
		int_type sungetc()
		{
			if( this->gptr() > this->eback() )
			{
				this->gbump(-1);
				return this->sgetc();
			}
			else
			{
				return this->pbackfail();
			}
		}
		
	
	protected:
		
		
		/// Constructor
		/// Protected so that no instance of basic_streambuf can be constructed, just
		/// instances of derived classes.
		basic_streambuf()
			: get_begin(0), get_current(0), get_end(0), put_begin(0), put_current(0), put_end(0), loc(dstd::locale())
		{}
		
		
		// Locales
		
		
		/// Derived classes may override this function in order to be informed about changes of the locale.
		/// The derived class may cache the locale and member facets between calls to imbue(). 
		virtual void imbue(const dstd::locale& loc) {};
		
		
		// Positioning
		
		
		/// Derived classes may override this function to allow removal or replacement 
		/// of the controlled character sequence (the buffer) with a user-provided array,
		/// or for any other implementation-specific purpose.
		virtual basic_streambuf* setbuf(char_type* s, dstd::streamsize n)
		{
			return this;
		}
		
		
		/// Sets the position indicator of the input and/or output sequence relative to some other position.
		/// The base class version of this function has no effect.
		/// Derived classes may override this function to allow relative positioning of the position indicator.
		virtual pos_type seekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which = ios_base::in | ios_base::out)
		{
			return -1;
		}
		
		
		/// Sets the position indicator of the input and/or output sequence to an absolute position. 
		/// The base class version of this function has no effect.
		/// Derived classes may override this function to allow absolute positioning of the position indicator.
		virtual pos_type seekpos(pos_type pos, ios_base::openmode which = ios_base::in | ios_base::out)
		{
			return -1;
		}
		
		
		/// Synchronizes the controlled character sequence (the buffers) with the associated character sequence. 
		/// The base class version of this function has no effect.
		/// Derived classes may override this function to allow synchronizing the underlying device with the buffers. 
		virtual int sync()
		{
			return 0;
		}
		
		
		// Get Area
		
		
		/// Estimates the number of characters available for input in the associated character sequence.
		virtual dstd::streamsize showmanyc()
		{
			return 0;
		}
		
		
		/// Ensures that, if possible, there is at least one character available to read from the input area.
		/// Updates input pointers (if needed) and reads more data in from the input sequence (if applicable).
		/// \note The base class version of this function just fails.
		/// \returns The next character in the input sequence on success, or traits_type::eof() on failure.
		virtual int_type underflow()
		{
			return traits_type::eof();
		}
		
		
		/// Reads a character from the input sequence.
		/// \returns The next input character on success, or traits_type::eof() on failure.
		virtual int_type uflow()
		{
			const int_type result = this->underflow();
			if( result != traits_type::eof() )
			{
				this->gbump(1);
			}
			return result;
		}
		
		
		/// Reads characters from the input sequence into a character array.
		/// Up to \p count characters are read if possible, or until traits_type::eof() is encountered.
		/// \param s An array sufficient in size to store at least \p count characters.
		/// \param count The maximum number of characters to read into the array.
		/// \returns The number of characters successfully read.
		virtual dstd::streamsize xsgetn(char_type* s, dstd::streamsize count)
		{
			int_type c;
			for(dstd::streamsize i = 0; i < count; ++i)
			{
				c = this->sbumpc();
				
				if( c == traits_type::eof() )
				{
					return i;
				}
				
				s[i] = traits_type::to_char_type(c);
			}
			return count;
		}
		
		
		/// Returns the pointer to the beginning of the get area. 
		char_type* eback() const
		{
			return this->get_begin;
		}
		
		
		/// Returns the pointer to the current character (get pointer) in the get area. 
		char_type* gptr() const
		{
			return this->get_current;
		}
		
		
		/// Returns the pointer to the end of the get area. 
		char_type* egptr() const
		{
			return this->get_end;
		}
		
		
		/// Skips count characters in the get area.
		/// This is done by advancing the get pointer by count characters.
		/// No checks are done for underflow. 
		void gbump(int count)
		{
			this->get_current += count;
		}
		
		
		/// Sets the values of the pointers defining the get area.
		void setg(char_type* begin, char_type* current, char_type* end)
		{
			this->get_begin = begin;
			this->get_current = current;
			this->get_end = end;
		}
		
		
		// Put Area
		
		
		/// Writes count characters to the output sequence from the character array whose first element is pointed to by s.
		/// The characters are written as if by repeated calls to sputc().
		/// Writing stops when either count characters are written or a call to sputc() would have returned Traits::eof().
		virtual dstd::streamsize xsputn(const char_type* s, dstd::streamsize count)
		{
			int_type result;
			for(dstd::streamsize i = 0; i < count; ++i)
			{
				result = this->sputc(s[i]);
				
				if( result == traits_type::eof() ) return i;
				
				this->pbump(1);
			}
			return count;
		}
		
		
		/// Ensures that there is space at the put area for at least one character 
		/// by saving some initial subsequence of characters starting at pbase() to the output sequence 
		/// and updating the pointers to the put area (if needed). 
		/// \returns An unspecified value not equal to Traits::eof() on success, or Traits::eof() otherwise.
		virtual int_type overflow(int_type ch = traits_type::eof() )
		{
			return traits_type::eof();
		}
		
		
		/// Returns the pointer to the beginning of the put area. 
		char_type* pbase() const
		{
			return this->put_begin;
		}
		
		
		/// Returns the pointer to the current character (put pointer) in the put area. 
		char_type* pptr() const
		{
			return this->put_current;
		}
		
		
		/// Returns the pointer to the end of the put area. 
		char_type* epptr() const
		{
			return this->put_end;
		}
		
		
		/// Repositions the put pointer (pptr()) by count characters, where count may be positive or negative.
		/// No checks are done for moving the pointer outside the put area [pbase(), epptr()). 
		void pbump(int count)
		{
			this->put_current += count;
		}
		
		
		/// Sets the values of the pointers defining the put area.
		void setp(char_type* begin, char_type* end)
		{
			this->put_begin = begin;
			this->put_current = begin;
			this->put_end = end;
		}
		
		
		/// This protected virtual function is called by the public functions sungetc() and sputbackc()
		/// (which, in turn, are called by basic_istream::unget and basic_istream::putback) when either: 
		///    1) There is no putback position in the get area
		///       (pbackfail() is called with no arguments).
		///    2) The caller attempts to putback a different character from the one retrieved earlier
		///       (pbackfail() is called with the character that needs to be put back)
		virtual int_type pbackfail(int_type c = traits_type::eof() )
		{
			return traits_type::eof();
		}
		
		
	private:
		
		char_type* get_begin;
		char_type* get_current;
		char_type* get_end;
		char_type* put_begin;
		char_type* put_current;
		char_type* put_end;
		dstd::locale loc;
};



#endif
