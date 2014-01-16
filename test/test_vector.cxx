#include "vector.hxx"

int main()
{
	dstd::vector<int> v_int();
	for(int i = 0; i != 6; ++i)
	{
		v_int.push_back(i);
	}
	
	dstd::vector<int>::iterator it = v_int.begin();
	const dstd::vector<int>::iterator end = v_int.end();
	for( ; it != end; ++it)
	{
		std::cout << *it << std::endl;
	}
	
	return 0;
}
