#include <iostream>
#include <string>
#include "../src/vector.hxx"

template <class T>
void printVector(const std::string& str, dstd::vector<T>& v)
{
	//typename dstd::vector<T>::iterator it = v.begin();
	//const typename dstd::vector<T>::iterator end = v.end();
	std::cout << str << " [";
	for(unsigned int i = 0; i != v.size(); ++i)
	{
		if( i != 0 ) std::cout << ", ";
		std::cout << v.at(i);
	}
	std::cout << "]" << std::endl;
}

int main()
{
	dstd::vector<int> v1;
	for(int i = 0; i != 6; ++i)
	{
		v1.push_back(i);
	}
	printVector<int>("v1", v1);
	
	dstd::vector<int> v2;
	for(int i = 0; i != 8; ++i)
	{
		v2.push_back(8);
	}
	printVector<int>("v2", v2);
	
	std::cout << "swap" << std::endl;
	dstd::swap(v1, v2);
	printVector<int>("v1", v1); printVector<int>("v2", v2);
	
	std::cout << "insert v2[2,4) into v1[2]" << std::endl;
	v1.insert( v1.begin() + 2, v2.begin() + 2, v2.begin() + 4 );
	printVector<int>("v1", v1); printVector<int>("v2", v2);
	
	return 0;
}
