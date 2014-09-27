#ifndef DSTD_IOS_HXX
#define DSTD_IOS_HXX



#include "string.hxx"



namespace dstd
{
	class ios_base;
	template <class Character, class Traits> class basic_ios;
	class fpos;
	
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
	
	// Because I can't be bothered with implementing a locale right now,
	// but may do in the future:
	typedef std::locale locale;
}



class dstd::ios_base
{
	public:
		
		// Format Flags
		
		typedef unsigned long fmtflags;
		static const fmtflags internal   = 1 <<  0;
		static const fmtflags left       = 1 <<  1;
		static const fmtflags right      = 1 <<  2;
		static const fmtflags dec        = 1 <<  3;
		static const fmtflags hex        = 1 <<  4;
		static const fmtflags oct        = 1 <<  5;
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
		static const iostate eofbit  = 1 << 0;
		static const iostate failbit = 1 << 1;
		static const iostate badbit  = 1 << 2;
		static const iostate goodbit = 1 << 3;
		
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
		static const seekdir beg = 1 << 0;
		static const seekdir cur = 1 << 1;
		static const seekdir end = 1 << 2;
		
		// Event types
		
		enum event { copyfmt_event, erase_event, imbue_event };
		typedef void (*event_callback)(event type, ios_base& ios, int index);
		
		class failure : public dstd::exception
		{
			public:
				virtual const char* what() const { return "dstd::ios_base::failure"; }
		};
		
		// TODO: class Init;
		
		
		//
		// Methods
		
		virtual ~ios_base();
		
		// Formatting
		
		fmtflags flags() const { return this->ff; }
		fmtflags flags(fmtflags flags) { fmtflags old = this->ff; this->ff = flags; return old; }
		
		fmtflags setf(fmtflags flags) { fmtflags old = this->ff; this->ff |= flags; return old; }
		fmtflags setf(fmtflags flags, fmtflags mask) { fmtflags old = this->ff; this->ff = (this->ff & ~mask) | (flags & mask); return old; }
		
		void unsetf(fmtflags flags ) { this->ff &= ~flags; }
		
		streamsize precision() const { return this->prec; }
		streamsize precision(streamsize new_precision) { streamsize old = this->prec; this->prec = new_precision; return old; }
		
		streamsize width() const { return this->wide; }
		streamsize width(streamsize new_width) { streamsize old = this->wide; this->wide = new_width; return old; }
		
		// Locales
		
		dstd::locale imbue(const dstd::locale& new_locale) { dstd::locale old = this->loc; this->loc = new_locale; return old; }
		dstd::locale getloc() const { return this->loc; }
		
		// Internal array
		
		static int xalloc() { static size_t xalloc_index = 0; return xalloc_index++; }
		
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
		
		void register_callback(event_callback function, int index);
		static bool sync_with_stdio(bool sync = true);
		
		
	protected:
		
		ios_base();
		
		
	private:
		
		ios_base(const ios_base& x);
		
		void l_expand(const int index)
		{
			// determine the new size required
			size_t new_llength = llength;
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
		
		void p_expand(const int index)
		{
			// determine the new size required
			size_t new_plength = plength;
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
		
		dstd::locale loc;
		streamsize wide;
		streamsize prec;
		fmtflags ff;
		void** parray;
		long* larray;
		size_t llength;
		size_t plength;
};


/*
template <class Character, class Traits = dstd::char_traits<Character> >
class dstd::basic_ios : public dstd::ios_base
{
	public:
		
		typedef Character char_type;
		typedef Traits traits_type;
		typedef typename Traits::int_type int_type;
		typedef typename Traits::pos_type pos_type;
		typedef typename Traits::off_type off_type;
		
		explicit basic_ios(dstd::basic_streambuf<Character, Traits>* buffer);
		
	protected:
		
		basic_ios();
	
	private:
		
		basic_ios(const basic_ios&);
};
*/



	
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
