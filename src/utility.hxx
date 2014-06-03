#ifndef DSTD_UTILITY_HXX
#define DSTD_UTILITY_HXX



namespace dstd
{
	template <class T1, class T2> class pair;
	template <class T1, class T2> pair<T1, T2> make_pair(const T1& a, const T2& b);
	template <class T> void swap(const T& a, const T& b);
	
	namespace rel_ops
	{
		template <class T> bool operator!= (const T& x, const T& y);
		template <class T> bool operator> (const T& x, const T& y);
		template <class T> bool operator<= (const T& x, const T& y);
		template <class T> bool operator>= (const T& x, const T& y);
		template <class T> bool foo (const T& x, const T& y);
	}
}



template <class T1, class T2>
class dstd::pair
{
	public:
		
		typedef T1 first_type;
		typedef T2 second_type;
		
		
		//
		// Constructors
		
		
		pair(){};
		
		
		template <class U1, class U2>
		pair(const pair<U1, U2>& p): first( p.first ), second( p.second ) {}
		
		
		pair(const first_type& a, const second_type& b) : first(a), second(b) {}
		
		
		//
		// Operations
		
		
		pair& operator= (const pair& p)
		{
			this->first = p.first;
			this->second = p.second;
			return *this;
		}
		
		
		//
		// Data Members
		
		first_type first;
		second_type second;
};



//
// Pair Relational Operators


template <class T1, class T2>
bool operator== (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ( lhs.first == rhs.first && lhs.second == rhs.second );
}


template <class T1, class T2>
bool operator!= (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ! ( lhs == rhs );
}


template <class T1, class T2>
bool operator< (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ( ( lhs.first < rhs.first ) || (lhs.first == rhs.first && lhs.second < rhs.second) );
}


template <class T1, class T2>
bool operator<= (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ( lhs < rhs || lhs == rhs );
}


template <class T1, class T2>
bool operator> (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ( lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second ) );
}


template <class T1, class T2>
bool operator>= (const dstd::pair<T1,T2>& lhs, const dstd::pair<T1,T2>& rhs)
{
	return ( lhs > rhs || lhs == rhs );
}



//
// Other Pair Functions


template <class T1, class T2> dstd::pair<T1, T2> dstd::make_pair(const T1& a, const T2& b)
{
	return dstd::pair<T1, T2>( a, b );
}



//
// swap


template <class T>
void dstd::swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}



//
// rel_ops


template <class T>
bool dstd::rel_ops::operator!=(const T& x, const T& y)
{
	return ! (x == y);
}


template <class T>
bool dstd::rel_ops::operator> (const T& x, const T& y)
{
	return ! (x == y || x < y);
}


template <class T>
bool dstd::rel_ops::operator<= (const T& x, const T& y)
{
	return (x == y || x < y);
}


template <class T>
bool dstd::rel_ops::operator>= (const T& x, const T& y)
{
	return ( x == y || dstd::rel_ops::operator>(x, y) );
}



#endif
