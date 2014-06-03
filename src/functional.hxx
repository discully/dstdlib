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
		public:
		
			bool operator() (const T& a, const T& b) const
			{
				return (a < b);
			}
	};
	
	
}



#endif
