#ifndef DSTD_FUNCTIONAL_HXX
#define DSTD_FUNCTIONAL_HXX



namespace dstd
{
	//
	// Operator classes
	//
	
	
	//
	// Comparison operations
	
	
	template <class T>
	class less
	{
		bool operator() (const T& a, const T& b)
		{
			return (a < b);
		}
	};
	
	
}



#endif
