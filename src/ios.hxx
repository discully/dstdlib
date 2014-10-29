#ifndef DSTD_IOS_HXX
#define DSTD_IOS_HXX



#include "iosfwd.hxx"
#include "exception.hxx"



namespace dstd
{
	typedef std::locale locale;
	
	class ios_base;
	
	template <class Character, class Traits > class basic_ios;
	
	template <class State> class fpos;
	
	typedef std::streamoff streamoff;
	typedef std::streamsize streamsize;
	
	dstd::ios_base& boolalpha   (dstd::ios_base& x);
	dstd::ios_base& showbase    (dstd::ios_base& x);
	dstd::ios_base& showpoint   (dstd::ios_base& x);
	dstd::ios_base& showpos     (dstd::ios_base& x);
	dstd::ios_base& skipws      (dstd::ios_base& x);
	dstd::ios_base& uppercase   (dstd::ios_base& x);
	dstd::ios_base& unitbuf     (dstd::ios_base& x);
	dstd::ios_base& noboolalpha (dstd::ios_base& x);
	dstd::ios_base& noshowbase  (dstd::ios_base& x);
	dstd::ios_base& noshowpoint (dstd::ios_base& x);
	dstd::ios_base& noshowpos   (dstd::ios_base& x);
	dstd::ios_base& noskipws    (dstd::ios_base& x);
	dstd::ios_base& nouppercase (dstd::ios_base& x);
	dstd::ios_base& nounitbuf   (dstd::ios_base& x);
	dstd::ios_base& internal    (dstd::ios_base& x);
	dstd::ios_base& left        (dstd::ios_base& x);
	dstd::ios_base& right       (dstd::ios_base& x);
	dstd::ios_base& dec         (dstd::ios_base& x);
	dstd::ios_base& hex         (dstd::ios_base& x);
	dstd::ios_base& oct         (dstd::ios_base& x);
	dstd::ios_base& fixed       (dstd::ios_base& x);
	dstd::ios_base& scientific  (dstd::ios_base& x);
}



class dstd::ios_base
{
	public:
		
		//
		// Flags
		
		// Format Flags
		typedef unsigned long fmtflags;
		static const fmtflags internal   = 1 <<  0;
		static const fmtflags left       = 1 <<  1;
		static const fmtflags right      = 1 <<  2;
		static const fmtflags dec        = 1 <<  3; ///< decimal numbers
		static const fmtflags hex        = 1 <<  4; ///< hexadecimal numbers
		static const fmtflags oct        = 1 <<  5; ///< octal numbers
		static const fmtflags fixed      = 1 <<  6;
		static const fmtflags scientific = 1 <<  7;
		static const fmtflags boolalpha  = 1 <<  8;
		static const fmtflags showbase   = 1 <<  9;
		static const fmtflags showpoint  = 1 << 10;
		static const fmtflags showpos    = 1 << 11;
		static const fmtflags skipws     = 1 << 12;
		static const fmtflags unitbuf    = 1 << 13;
		static const fmtflags uppercase  = 1 << 14;
		static const fmtflags adjustfield = internal | left | right;
		static const fmtflags basefield = dec | hex | oct;
		static const fmtflags floatfield = fixed | scientific;
		
		// State flags
		typedef unsigned long iostate;
		static const iostate goodbit = 0;
		static const iostate eofbit  = 1 << 0;
		static const iostate failbit = 1 << 1;
		static const iostate badbit  = 1 << 2;
		
		// Open mode flags
		typedef unsigned long openmode;
		static const openmode app    = 1 << 0; ///< append - seek to the end of the stream before each write
		static const openmode ate    = 1 << 1; ///< at the end - seek to the end of the stream immediately after open
		static const openmode binary = 1 << 2; ///< open in binary mode
		static const openmode in     = 1 << 3; ///< open for reading
		static const openmode out    = 1 << 4; ///< open for writing
		static const openmode trunc  = 1 << 5; ///< truncate - discards the contents of the stream when opening
		
		// Seek direction
		typedef unsigned long seekdir;
		static const seekdir beg = 1 << 0; ///< beginning
		static const seekdir cur = 1 << 1; ///< current
		static const seekdir end = 1 << 2; ///< end
		
		// Event types
		
		enum event { copyfmt_event, erase_event, imbue_event };
		typedef void (*event_callback)(event type, ios_base& ios, int index);
		
		class failure : public dstd::exception
		{
			public:
				virtual const char* what() const { return "dstd::ios_base::failure"; }
		};
		
		// toto
		//class Init;
		
		
		//
		// Methods
		
		virtual ~ios_base()
		{
			this->callback(dstd::ios_base::erase_event);
		};
		
		
		// Formatting
		
		/// Returns the current formatting flags.
		fmtflags flags() const
		{
			return this->frmt;
		}
		
		
		/// Set the formatting flags.
		/// \flags The new format flags.
		/// \returns The formatting flags before this change.
		fmtflags flags(fmtflags flags)
		{
			fmtflags old = this->frmt;
			this->frmt = flags;
			return old;
		}
		
		
		/// Set additional formatting flags to \p true.
		/// \param flags Bit mask indicating which formatting flags should be set to \p true.
		/// \returns The formatting flags before this change.
		fmtflags setf(fmtflags flags)
		{
			fmtflags old = this->frmt;
			this->frmt |= flags;
			return old;
		}
		
		
		/// Set specific formatting flags.
		/// \param flags The new values of formatting flags in \p mask.
		/// \param mask Bit mask indicating which formatting flags should be set according to \p flags.
		/// \returns The formatting flags before this change.
		fmtflags setf(fmtflags flags, fmtflags mask)
		{
			fmtflags old = this->frmt;
			this->frmt = ( (this->frmt & ~mask) | (flags & mask) );
			return old;
		}
		
		
		/// Set specific format flags 
		void unsetf(fmtflags flags )
		{
			this->frmt &= ~flags;
		}
		
		
		streamsize precision() const
		{
			return this->prec;
		}
		
		
		streamsize precision(streamsize new_precision)
		{
			streamsize old = this->prec;
			this->prec = new_precision;
			return old;
		}
		
		
		streamsize width() const
		{
			return this->wide;
		}
		
		
		streamsize width(streamsize new_width)
		{
			streamsize old = this->wide;
			this->wide = new_width;
			return old;
		}
		
		
		// Locales
		
		dstd::locale imbue(const dstd::locale& new_locale)
		{
			dstd::locale old_locale = this->loc;
			this->loc = new_locale;
			this->callback(dstd::ios_base::imbue_event);
			return old_locale;
		}
		
		
		dstd::locale getloc() const
		{
			return this->loc;
		}
		
		
		// Internal arrays
		
		static int xalloc()
		{
			static size_t xalloc_index = 0;
			return xalloc_index++;
		}
		
		
		long& iword(int index)
		{
			if( index >= llength )
			{
				this->l_expand(index);
			}
			return this->larray[index];
		}
		
		
		void*& pword(int index)
		{
			if( index >= plength )
			{
				this->p_expand(index);
			}
			return this->parray[index];
		}
		
		
		// Miscellaneous
		
		void register_callback(event_callback function, int index)
		{
			if( this->clength == this->ccapacity ) this->c_expand(this->clength + 1);
			
			callback_pair new_callback;
			new_callback.func = function;
			new_callback.index = index;
			
			this->carray[this->clength] = new_callback;
			++(this->clength);
		}
		
		
		// todo
		//static bool sync_with_stdio(bool sync = true);
		
		
	protected:
		
		/// The internal state is undefined after the construction.
		/// The derived class must call basic_ios::init() to complete initialization before first use or before destructor,
		/// otherwise the behaviour is undefined.
		ios_base()
			: clength(0), ccapacity(0), llength(0), plength(0)
		{};
		
		
		void callback(dstd::ios_base::event e)
		{
			for(size_t i = this->clength; i > 0; --i)
			{
				this->carray[i-1].func(e, *this, this->carray[i-1].index);
			}
		}
		
		
	private:
		
		struct callback_pair
		{
			event_callback func;
			int index;
		};
		
		
		/// Not implemented. Copy construction is prohibited.
		ios_base(const ios_base& x);
		
		
		void c_expand(const size_t size)
		{
			// determine the new size required
			size_t new_ccapacity = (this->ccapacity > 0) ? this->ccapacity : 4;
			while( new_ccapacity <= size )
			{
				new_ccapacity *= 2;
			}
			// get the new array
			callback_pair* new_carray = new callback_pair[new_ccapacity];
			if( new_carray == 0 )
			{
				// todo: complain
			}
			// copy existing values over
			for(size_t i = 0; i < this->clength; ++i)
			{
				new_carray[i] = this->carray[i];
			}
			// update class members
			this->carray = new_carray;
			this->ccapacity = new_ccapacity;
		}
		
		
		void l_expand(const size_t index)
		{
			// determine the new size required
			size_t new_llength = (this->llength > 0) ? this->llength : 4;
			while( new_llength <= index )
			{
				new_llength *= 2;
			}
			// get the new array
			long* new_larray = new long[new_llength];
			if( new_larray == 0 )
			{
				// todo: complain
			}
			// copy existing values over
			for(size_t i = 0; i < this->llength; ++i)
			{
				new_larray[i] = this->larray[i];
			}
			// update class members
			this->larray = new_larray;
			this->llength = new_llength;
		}
		
		
		void p_expand(const size_t index)
		{
			// determine the new size required
			size_t new_plength = (this->plength > 0) ? this->plength : 4;
			while( new_plength <= index )
			{
				new_plength *= 2;
			}
			// get the new array
			void** new_parray = new void*[new_plength];
			if( new_parray == 0 )
			{
				// todo: complain
			}
			// copy existing values over
			for(size_t i = 0; i < this->plength; ++i)
			{
				new_parray[i] = this->parray[i];
			}
			// update class members
			this->parray = new_parray;
			this->plength = new_plength;
		}
		
		
		dstd::locale loc; ///< Current locale.
		streamsize wide; ///< Field width.
		streamsize prec; ///< Field precision.
		fmtflags frmt; ///< Field format.
		callback_pair* carray; ///< Array of callback functions.
		void** parray; ///< Array of generic void pointers.
		long* larray; ///< Array of generic longs.
		size_t clength; ///< Number of entries in the array of callback functions (\p carray).
		size_t ccapacity; ///< Length of the array of callback functions (\p carray).
		size_t llength; ///< Length of the array of long (\p larray).
		size_t plength; ///< Length of the array of void pointers (\p parray).
};



// Have declared the flags, now need to define them:
// Format flags
const dstd::ios_base::fmtflags dstd::ios_base::internal;
const dstd::ios_base::fmtflags dstd::ios_base::left;
const dstd::ios_base::fmtflags dstd::ios_base::right;
const dstd::ios_base::fmtflags dstd::ios_base::dec;
const dstd::ios_base::fmtflags dstd::ios_base::hex;
const dstd::ios_base::fmtflags dstd::ios_base::oct;
const dstd::ios_base::fmtflags dstd::ios_base::fixed;
const dstd::ios_base::fmtflags dstd::ios_base::scientific;
const dstd::ios_base::fmtflags dstd::ios_base::boolalpha;
const dstd::ios_base::fmtflags dstd::ios_base::showbase;
const dstd::ios_base::fmtflags dstd::ios_base::showpoint;
const dstd::ios_base::fmtflags dstd::ios_base::showpos;
const dstd::ios_base::fmtflags dstd::ios_base::skipws;
const dstd::ios_base::fmtflags dstd::ios_base::unitbuf;
const dstd::ios_base::fmtflags dstd::ios_base::uppercase;
const dstd::ios_base::fmtflags dstd::ios_base::adjustfield;
const dstd::ios_base::fmtflags dstd::ios_base::basefield;
const dstd::ios_base::fmtflags dstd::ios_base::floatfield;
// State flags
const dstd::ios_base::iostate dstd::ios_base::eofbit;
const dstd::ios_base::iostate dstd::ios_base::failbit;
const dstd::ios_base::iostate dstd::ios_base::badbit;
const dstd::ios_base::iostate dstd::ios_base::goodbit;
// Open mode flags
const dstd::ios_base::openmode dstd::ios_base::app;
const dstd::ios_base::openmode dstd::ios_base::ate;
const dstd::ios_base::openmode dstd::ios_base::binary;
const dstd::ios_base::openmode dstd::ios_base::in;
const dstd::ios_base::openmode dstd::ios_base::out;
const dstd::ios_base::openmode dstd::ios_base::trunc;
// Seek direction
const dstd::ios_base::seekdir dstd::ios_base::beg;
const dstd::ios_base::seekdir dstd::ios_base::cur;
const dstd::ios_base::seekdir dstd::ios_base::end;



template <class Character, class Traits >
class dstd::basic_ios : public dstd::ios_base
{
	public:
		
		typedef Character char_type;
		typedef Traits traits_type;
		typedef typename Traits::int_type int_type;
		typedef typename Traits::pos_type pos_type;
		typedef typename Traits::off_type off_type;
		
		
		explicit basic_ios(dstd::basic_streambuf<Character, Traits>* read_buffer)
		{
			this->init(read_buffer);
		}
		
		
		virtual ~basic_ios() {}
		
		
		// State functions
		
		
		/// Checks if no error has occurred.
		bool good() const
		{
			return (this->rdstate() == dstd::ios_base::goodbit);
		}
		
		
		/// Checks if end-of-file has been reached .
		/// \returns True if dstd::ios_base::eofbit has been set, false otherwise.
		bool eof() const
		{
			return ( (this->rdstate() & dstd::ios_base::eofbit) != 0 );
		}
		
		
		/// Checks if a recoverable error has occurred 
		/// \returns True if dstd::ios_base::badbit or dstd::ios_base::failbit have been set.
		bool fail() const
		{
			return ( (this->rdstate() & (dstd::ios_base::badbit | dstd::ios_base::failbit)) != 0 );
		}
		
		
		/// Checks if a non-recoverable error has occurred.
		/// \returns True if dstd::ios_base::badbit has been set, or false otherwise.
		bool bad() const
		{
			return ( (this->rdstate() & dstd::ios_base::badbit) != 0 );
		}
		
		
		/// Synonym of fail()
		bool operator! () const
		{
			return this->fail();
		}
		
		
		/// Synonym of ! fail()
		/// \returns A null pointer if fail() is true, or a non-null pointer otherwise.
		operator void* () const
		{
			return (this->fail() ? 0 : 1);
		}
		
		
		dstd::ios_base::iostate rdstate() const
		{
			return this->state;
		}
		
		
		void setstate(dstd::ios_base::iostate read_state)
		{
			this->state |= read_state;
		}
		
		
		void clear(dstd::ios_base::iostate read_state = dstd::ios_base::goodbit)
		{
			if( this->rdbuf() == 0 )
			{
				this->state = read_state | dstd::ios_base::badbit;
			}
			else
			{
				this->state = read_state;
			}
		}
		
		
		// Formatting
		
		basic_ios& copyfmt(const basic_ios& x)
		{
			this->callback(dstd::ios_base::erase_event);
			
			this->flags( x.flags() );
			this->width( x.width() );
			this->precision( x.precision() );
			this->loc = x.loc; // Can't call imbue: don't want to fire the imbue_event callbacks.
			for(size_t i = 0; i < x.llength; ++i) // todo: could be more efficient, avoiding the function calls?
			{
				this->iword(i) = x.iword(i);
			}
			for(size_t i = 0; i < x.plength; ++i)// todo: could be more efficient, avoiding the function calls?
			{
				this->pword(i) = x.pword(i);
			}
			this->fill( x.fill() );
			this->tie( x.tie() );
			
			this->callback(dstd::ios_base::copyfmt_event);
			
			this->exceptions( x.exceptions() );
			
			return *this;
		}
		
		
		Character fill() const
		{
			return this->fill_char;
		}
		
		
		Character fill(Character c)
		{
			char_type old_fill = this->fill_char;
			this->fill_char = c;
			return old_fill;
		}
		
		
		// Miscellaneous
		
		/// Returns the current exception mask.
		dstd::ios_base::iostate exceptions() const
		{
			return this->take_exception;
		}
		
		
		/// Sets the current exception mask.
		void exceptions(dstd::ios_base::iostate except)
		{
			this->take_exception = except;
		}
		
		
		/// Replaces the current locale.
		/// \returns The previous locale.
		dstd::locale imbue(const dstd::locale& new_locale)
		{
			dstd::locale old_locale = this->ios_base::imbue(new_locale);
			if( this->rdbuf() != 0 ) this->rdbuf()->pubimbue(new_locale);
			return old_locale;
		}
		
		
		dstd::basic_streambuf<Character, Traits>* rdbuf() const
		{
			return this->buffer;
		}
		
		
		dstd::basic_streambuf<Character, Traits>* rdbuf(dstd::basic_streambuf<Character, Traits>* new_buffer)
		{
			dstd::basic_streambuf<Character, Traits>* old_buffer = this->rdbuf();
			this->buffer = new_buffer;
			return old_buffer;
		}
		
		
		dstd::basic_ostream<Character, Traits>* tie() const
		{
			return this->stream;
		}
		
		
		dstd::basic_ostream<Character, Traits>* tie(dstd::basic_ostream<Character, Traits>* new_ostream)
		{
			dstd::basic_ostream<Character, Traits>* old_ostream = this->tie();
			this->ostream = new_ostream;
			return old_ostream;
		}
		
		
		// todo
		// char narrow( char_type c, char dfault ) const
		
		
		// todo
		// char_type widen( char c ) const
		
		
	protected:
		
		/// Default constructor. 
		/// The internal state is not initialized.
		/// init() must be called before the first use of the object or before destructor, otherwise the behaviour is undefined.
		basic_ios() {};
		
		
		void init(dstd::basic_streambuf<Character, Traits>* read_buffer)
		{
			this->rdbuf(read_buffer);
			this->tie(0);
			this->clear();
			this->exceptions(dstd::ios_base::goodbit);
			this->flags(dstd::ios_base::skipws | dstd::ios_base::dec);
			this->width(0);
			this->precision(6);
			// todo
			// this->fill( this->widen(' ') );
			this->imbue( dstd::locale() );
		}
		
		
		void set_rdbuf(dstd::basic_streambuf<Character, Traits>* new_buffer)
		{
			this->buffer = new_buffer;
		}
	
	
	private:
		
		/// Not implemented. Copy construction is not permitted.
		basic_ios(const basic_ios&);
		
		
		/// Not implemented. Copy assignment is not permitted.
		basic_ios& operator= (const basic_ios& x);
		
		
		dstd::basic_streambuf<Character, Traits>* buffer;
		char_type fill_char;
		dstd::ios_base::iostate state;
		dstd::basic_ostream<Character, Traits>* ostream;
		dstd::ios_base::iostate take_exception;
};



template <class State>
class dstd::fpos
{
	public:
		
		fpos(int state)
			: s(state)
		{}
		
		
		fpos(const dstd::streamoff& off)
			: s(off)
		{}
		
		
		operator dstd::streamoff() const
		{
			return static_cast<dstd::streamoff>( this->state() );
		}
		
		
		State state() const
		{
			return this->s;
		}
		
		
		void state(State new_state)
		{
			this->s = new_state;
		}
		
		
		bool operator== (const fpos& x)
		{
			return (this->state() == x.state());
		}
		
		
		bool operator!= (const fpos& x)
		{
			return ! (*this == x);
		}
		
		
		fpos& operator+= (const dstd::streamoff& off)
		{
			this->s += off;
			return *this;
		}
		
		
		fpos operator+ (const dstd::streamoff& off)
		{
			fpos temp(*this);
			temp += off;
			return temp;
		}
		
		
		fpos& operator-= (const dstd::streamoff& off)
		{
			this->s -= off;
			return *this;
		}
		
		
		dstd::streamoff operator- (const fpos& rhs)
		{
			return static_cast<dstd::streamoff>( this->state() - rhs.state() );
		}
		
		
		fpos operator- (const dstd::streamoff& off)
		{
			fpos temp(*this);
			temp -= off;
			return temp;
		}
		
		
	private:
		
		State s;
};


	
dstd::ios_base& boolalpha   (dstd::ios_base& x) { x.setf(dstd::ios_base::boolalpha);   return x; }
dstd::ios_base& showbase    (dstd::ios_base& x) { x.setf(dstd::ios_base::showbase);    return x; }
dstd::ios_base& showpoint   (dstd::ios_base& x) { x.setf(dstd::ios_base::showpoint);   return x; }
dstd::ios_base& showpos     (dstd::ios_base& x) { x.setf(dstd::ios_base::showpos);     return x; }
dstd::ios_base& skipws      (dstd::ios_base& x) { x.setf(dstd::ios_base::skipws);      return x; }
dstd::ios_base& uppercase   (dstd::ios_base& x) { x.setf(dstd::ios_base::uppercase);   return x; }
dstd::ios_base& unitbuf     (dstd::ios_base& x) { x.setf(dstd::ios_base::unitbuf);     return x; }
dstd::ios_base& noboolalpha (dstd::ios_base& x) { x.unsetf(dstd::ios_base::boolalpha); return x; }
dstd::ios_base& noshowbase  (dstd::ios_base& x) { x.unsetf(dstd::ios_base::showbase);  return x; }
dstd::ios_base& noshowpoint (dstd::ios_base& x) { x.unsetf(dstd::ios_base::showpoint); return x; }
dstd::ios_base& noshowpos   (dstd::ios_base& x) { x.unsetf(dstd::ios_base::showpos);   return x; }
dstd::ios_base& noskipws    (dstd::ios_base& x) { x.unsetf(dstd::ios_base::skipws);    return x; }
dstd::ios_base& nouppercase (dstd::ios_base& x) { x.unsetf(dstd::ios_base::uppercase); return x; }
dstd::ios_base& nounitbuf   (dstd::ios_base& x) { x.unsetf(dstd::ios_base::unitbuf);   return x; }
dstd::ios_base& internal    (dstd::ios_base& x) { x.setf(dstd::ios_base::internal,   dstd::ios_base::adjustfield); return x; }
dstd::ios_base& left        (dstd::ios_base& x) { x.setf(dstd::ios_base::left,       dstd::ios_base::adjustfield); return x; }
dstd::ios_base& right       (dstd::ios_base& x) { x.setf(dstd::ios_base::right,      dstd::ios_base::adjustfield); return x; }
dstd::ios_base& dec         (dstd::ios_base& x) { x.setf(dstd::ios_base::dec,        dstd::ios_base::basefield);   return x; }
dstd::ios_base& hex         (dstd::ios_base& x) { x.setf(dstd::ios_base::hex,        dstd::ios_base::basefield);   return x; }
dstd::ios_base& oct         (dstd::ios_base& x) { x.setf(dstd::ios_base::oct,        dstd::ios_base::basefield);   return x; }
dstd::ios_base& fixed       (dstd::ios_base& x) { x.setf(dstd::ios_base::fixed,      dstd::ios_base::floatfield);  return x; }
dstd::ios_base& scientific  (dstd::ios_base& x) { x.setf(dstd::ios_base::scientific, dstd::ios_base::floatfield);  return x; }



#endif
