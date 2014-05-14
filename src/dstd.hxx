#ifndef DSTD_DSTD_HXX
#define DSTD_DSTD_HXX



namespace dstd
{
	namespace impl
	{
		class TrueType;
		class FalseType;
		
		template <bool T> class BoolType;
		
		template <>
		class BoolType<true>
		{
			public:
				typedef dstd::impl::TrueType bool_type;
		};


		template <>
		class BoolType<false>
		{
			public:
				typedef dstd::impl::FalseType bool_type;
		};
	}
}



class dstd::impl::TrueType
{
	public:
		TrueType() : value(true) {}
		bool value;
};



class dstd::impl::FalseType
{
	public:
		FalseType() : value(false) {}
		bool value;
};



#endif
