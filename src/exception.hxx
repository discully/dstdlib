#ifndef DSTD_EXCEPTION_HXX
#define DSTD_EXCEPTION_HXX

namespace dstd
{
	class exception
	{
		public:
		
		exception(){}
		virtual ~exception(){}
		virtual const char* what() const { return "dstd::exception"; }
	};
	
	class bad_alloc : public exception
	{
		public:
		
		virtual const char* what() const { return "dstd::bad_alloc"; }
	};
	
	class invalid_argument : public exception
	{
		public:
		
		virtual const char* what() const { return "dstd::invalid_argument"; }
	};
	
	class out_of_range : public exception
	{
		public:
		
		virtual const char* what() const { return "dstd::out_of_range"; }
	};
	
	class length_error : public exception
	{
		public:
		
		virtual const char* what() const { return "dstd::length_error"; }
	};
}

#endif
