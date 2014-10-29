#include <iostream>
#include "tester.hxx"
#include "../src/ios.hxx"


class ios_base_test : public dstd::ios_base
{
	public:
		ios_base_test()
		{
			this->flags(0);
		}
};


int main()
{
	dstd::Tester t("<ios> ios_base");
	
	// fmtflags flags() const
	// fmtflags flags(fmtflags flags)
	
	{
		ios_base_test b;
		t.testEqual("flags():shouldAllBeFalseOnConstruction", b.flags(), 0);
	}
	
	{
		ios_base_test b;
		const dstd::ios_base::fmtflags f = dstd::ios_base::dec | dstd::ios_base::showbase;
		b.flags(f);
		t.testEqual("flags():shouldSetAndReturnCurrentFlags", b.flags(), f);
	}
	
	{
		const dstd::ios_base::fmtflags f1 = dstd::ios_base::dec | dstd::ios_base::showbase;
		const dstd::ios_base::fmtflags f2 = dstd::ios_base::hex | dstd::ios_base::left;
		
		ios_base_test b;
		b.flags(f1);
		
		t.testEqual("flags(fmtflags):shouldReturnPreviousValue", b.flags(f2), f1);
	}
	
	// fmtflags setf(fmtflags flags)
	
	{
		const dstd::ios_base::fmtflags f1 = dstd::ios_base::dec | dstd::ios_base::showbase;
		const dstd::ios_base::fmtflags f2 = dstd::ios_base::left | dstd::ios_base::showbase;
		
		ios_base_test b;
		b.flags(f1);
		
		b.setf(f2);
		t.testEqual("setf(fmtflags):shouldSetAdditionalFlags", b.flags(), f1 | f2);
	}
	
	// fmtflags setf(fmtflags flags, fmtflags mask)
	
	{
		const dstd::ios_base::fmtflags f0 = dstd::ios_base::hex | dstd::ios_base::left;
		const dstd::ios_base::fmtflags f1 = dstd::ios_base::dec | dstd::ios_base::showbase;
		const dstd::ios_base::fmtflags f2 = dstd::ios_base::dec | dstd::ios_base::hex;
		
		ios_base_test b;
		b.flags(f0);
		
		t.testEqual("setf(fmtflags,fmtflags):shouldReturnPreviousValue",   b.setf(f1, f2), f0);
	}
	
	{
		const dstd::ios_base::fmtflags f0 = dstd::ios_base::hex | dstd::ios_base::left;
		const dstd::ios_base::fmtflags f1 = dstd::ios_base::dec | dstd::ios_base::showbase;
		const dstd::ios_base::fmtflags f2 = dstd::ios_base::dec | dstd::ios_base::hex;
		const dstd::ios_base::fmtflags f3 = dstd::ios_base::dec | dstd::ios_base::left;
		
		ios_base_test b;
		b.flags(f0);
		
		b.setf(f1, f2);
		t.testEqual("setf(fmtflags,fmtflags):shouldOnlySetSpecifiedFlags", b.flags(), f3);
	}
	
	// void unsetf(fmtflags flags )
	
	{
		ios_base_test b;
		b.setf(-1);
		b.unsetf(-1);
		t.testEqual("unsetf:shouldSetAllFlagsToZeroWithMinus1", b.flags(), 0);
	}
	
	{
		ios_base_test b;
		b.setf(dstd::ios_base::dec | dstd::ios_base::showbase);
		b.unsetf(dstd::ios_base::dec);
		t.testEqual("unsetf:shouldOnlyUnsetSpecifiedFlags", b.flags(), dstd::ios_base::showbase);
	}
	
	// streamsize precision() const
	// streamsize precision(streamsize new_precision)
	
	{
		ios_base_test b;
		const int p = 7;
		b.precision(p);
		t.testEqual("precision:shouldSetAndReturnValue", b.precision(), p);
	}
	
	// streamsize width() const
	// streamsize width(streamsize new_width)
	
	{
		ios_base_test b;
		const int w = 45;
		b.width(w);
		t.testEqual("width:shouldSetAndReturnValue", b.width(), w);
	}
	
	// dstd::locale imbue(const dstd::locale& new_locale)
	
	{
		ios_base_test b;
		const dstd::locale e("en_GB.UTF-8");
		
		t.testEqual("imbue:shouldReturnPreviousLocale", b.imbue(e).name(), dstd::locale().name());
	}
	
	{
		ios_base_test b;
		const dstd::locale e("en_GB.UTF-8");
		
		b.imbue(e);
		t.testEqual("imbue:shouldSetLocale", b.getloc().name(), e.name());
	}
	
	// dstd::locale getloc() const
	
	{
		ios_base_test b;
		t.testEqual("getloc:shouldReturnDefaultLocaleOnConstruction", b.getloc().name(), dstd::locale().name());
	}
	
	{
		ios_base_test b;
		const dstd::locale e("en_GB.UTF-8");
		
		b.imbue(e);
		t.testEqual("getloc:shouldReturnLocale", b.getloc().name(), e.name());
	}
	
	// static int xalloc()
	
	{
		ios_base_test b1;
		ios_base_test b2;
		ios_base_test b3;
		
		t.testNotEqual("xalloc:twoValuesFromSameInstanceShouldBeUnique", b1.xalloc(), b1.xalloc());
		t.testNotEqual("xalloc:twoValuesFromSeparateInstancesShouldBeUnique", b2.xalloc(), b3.xalloc());
	}
	
	// long& iword(int index)
	
	{
		ios_base_test b;
		const int x = b.xalloc();
		const int v = 34;
		b.iword(x) = v;
		t.testEqual("iword:shouldSetAndReturnValue", b.iword(x), v);
	}
	
	// void*& pword(int index)
	
	{
		ios_base_test b;
		const int x = b.xalloc();
		double* a = new double(3.3);
		void* v = static_cast<void*>( &a );
		b.pword(x) = v;
		t.testEqual("pword:shouldSetAndReturnValue", b.pword(x), v);
	}
	
	// todo
	//void register_callback(event_callback function, int index);
	
	// todo
	//static bool sync_with_stdio(bool sync = true);
	
	t.report();
	return 0;
}

