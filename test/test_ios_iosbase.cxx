#include <iostream>
#include "tester.hxx"
#include "../src/ios.hxx"


static int global_last_callback = 0;
static dstd::ios_base::event global_f1_event = dstd::ios_base::erase_event;
static int global_f1_calls = 0;
static int global_f1_index = 0;
static dstd::ios_base* global_f1_ios = 0;
void callbackFunction1(dstd::ios_base::event type, dstd::ios_base& ios, int index)
{
	++global_f1_calls;
	global_f1_event = type;
	global_f1_index = index;
	global_last_callback = 1;
	global_f1_ios = &ios;
}
void callbackFunction2(dstd::ios_base::event type, dstd::ios_base& ios, int index)
{
	global_last_callback = 2;
}


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
	
	// virtual ~ios_base()
	
	{
		global_f1_event = dstd::ios_base::imbue_event;
		const int before = global_f1_calls;
		
		{
			ios_base_test b;
			b.register_callback(callbackFunction1, 99);
		}
		
		t.testEqual("destructor:shouldTriggerCallback", global_f1_calls, before+1);
		t.testEqual("destructor:shouldTriggerCallbackWithEraseEvent", global_f1_event, dstd::ios_base::erase_event);
	}
	
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
	
	{
		const dstd::ios_base::fmtflags f1 = dstd::ios_base::dec | dstd::ios_base::showbase;
		const dstd::ios_base::fmtflags f2 = dstd::ios_base::left | dstd::ios_base::showbase;
		
		ios_base_test b;
		b.flags(f1);
		
		t.testEqual("setf(fmtflags):shouldReturnPreviousValue", b.setf(f2), f1);
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
	
	{
		ios_base_test b;
		const int p = 9;
		b.precision(p);
		t.testEqual("precision(streamsize):shouldReturnPreviousValue", b.precision(p+2), p);
	}
	
	// streamsize width() const
	// streamsize width(streamsize new_width)
	
	{
		ios_base_test b;
		const int w = 45;
		b.width(w);
		t.testEqual("width:shouldSetAndReturnValue", b.width(), w);
	}
	
	{
		ios_base_test b;
		const int w = 45;
		b.width(w);
		t.testEqual("width(streamsize):shouldReturnPreviousValue", b.width(w+4), w);
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
	
	{
		ios_base_test b;
		const dstd::locale e("en_GB.UTF-8");
		
		b.register_callback(callbackFunction1, 99);
		global_f1_event = dstd::ios_base::erase_event;	
		const int before = global_f1_calls;
		
		b.imbue(e);
		
		t.testEqual("imbue:shouldTriggerCallback", global_f1_calls, before+1);
		t.testEqual("imbue:shouldTriggerCallbackWithImbueEvent", global_f1_event, dstd::ios_base::imbue_event);
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
	
	//void register_callback(event_callback function, int index);
	
	{
		ios_base_test b;
		
		const int index = 66;
		b.register_callback(callbackFunction1, index);
		global_f1_index = index - 3;
		global_f1_ios = 0;
		
		b.imbue( dstd::locale("C") );
		
		t.testEqual("register_callback:functionsShouldBePassedCorrectIndex", global_f1_index, index);
		t.testEqual("register_callback:functionsShouldBePassedCorrectEvent", global_f1_event, dstd::ios_base::imbue_event);
		t.testEqual("register_callback:functionsShouldBePassedCorrectIosBase", global_f1_ios, &b);
	}
	
	{
		ios_base_test b;
		
		b.register_callback(callbackFunction1, 99);
		b.register_callback(callbackFunction2, 98);
		
		b.imbue( dstd::locale("C") );
		
		t.testEqual("register_callback:functionsShouldBeCalledInReverseOrder", global_last_callback, 1);
	}
	
	{
		ios_base_test b;
		
		b.register_callback(callbackFunction1, 99);
		b.register_callback(callbackFunction1, 98);
		
		const int before = global_f1_calls;
		b.imbue( dstd::locale("C") );
		
		t.testEqual("register_callback:identicalFunctionsShouldEachBeCalled", global_f1_calls, before + 2);
	}
	
	// todo
	//static bool sync_with_stdio(bool sync = true);
	
	t.report();
	return 0;
}

