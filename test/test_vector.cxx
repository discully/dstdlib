#include <iostream>
#include "../src/vector.hxx"

template <class T>
void printVector(dstd::vector<T>& v)
{
	typename dstd::vector<T>::iterator it = v.begin();
	const typename dstd::vector<T>::iterator end = v.end();
	for( ; it != end; ++it)
	{
		std::cout << *it << ", ";
	}
	std::cout << std::endl;
}

int main()
{
	dstd::vector<int> v_int;
	for(int i = 0; i != 6; ++i)
	{
		v_int.push_back(i);
	}
	
	printVector<int>( v_int );
	
	
	
	return 0;
}
