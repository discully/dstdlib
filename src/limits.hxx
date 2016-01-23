#ifndef DSTD_LIMITS_HXX
#define DSTD_LIMITS_HXX

#include <limits>



namespace dstd
{
	template <class T> class numeric_limits;
}



template <class T>
class dstd::numeric_limits : public std::numeric_limits<T>
{};



#endif
