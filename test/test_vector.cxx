#include <iostream>
#include <string>
#include "../src/vector.hxx"
#include "tester.hxx"

template <class T>
void printVector(const std::string& str, dstd::vector<T>& v)
{
	std::cout << str << " [";
	for(unsigned int i = 0; i != v.size(); ++i)
	{
		if( i != 0 ) std::cout << ", ";
		std::cout << v.at(i);
	}
	std::cout << "]" << std::endl;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::vector<T>& v)
{
	stream << "dstd::vector(" << v.size() << ", " << v.capacity() << ")";
	return stream;
}

int main()
{	
	dstd::Tester t("dstd::vector");
	
	const double pi = 3.14159;
	dstd::vector<double> v;
	for(unsigned int i = 0; i != 99; ++i)
	{
		v.push_back( i * pi );
	}
	
	
	//
	// Element access
	//
	
	//T& operator[](unsigned int i);
	//const T& operator[](unsigned int i) const;
	
	t.testEqual<double>( "operator[]", v[24], 24*pi );
	
	// This should not throw an exception
	try
	{
		double temp = v[9999];
		t.registerTestPassed("operator[]_OutOfRange");
		temp *=  2.0; // avoid compiler warning
	}
	catch(...)
	{
		t.registerTestFailed("operator[]_OutOfRange", "An exception was thrown");
	}
	
	//T& at(unsigned int i);
	//const T& at(unsigned int i) const;
	
	t.testEqual<double>( "at", v[24], 24*pi );
	
	// This should throw an exception
	try
	{
		double temp = v.at(9999);
		t.registerTestFailed("at_OutOfRange", "No exception thrown");
		temp *=  2.0; // avoid compiler warning
	}
	catch(...)
	{
		t.registerTestPassed("at_OutOfRange");
	}
	
	//T& front();
	//const T& front() const;
	
	t.testEqual<double>( "front", v.front(), 0*pi );
	
	//T& back();
	//const T& back() const;
	
	t.testEqual<double>( "back", v.back(), (v.size()-1)*pi );
	
	
	
	//
	// Capacity methods
	//
	
	// unsigned int size() const;
	
	t.testEqual<unsigned int>("size", v.size(), 99 );
	
	// unsigned int max_size() const;
	
	t.testGreaterThanOrEqual<unsigned int>("max_size", v.max_size(), v.size());
	
	//void resize(unsigned int n, const T& value = T());
	{
		dstd::vector<double> v_temp(v);
		v_temp.resize(200, pi);
		t.testEqual<unsigned int>("resize_size",   v_temp.size(), 200 );
		t.testEqual<double>("resize_compate", v_temp.at(10), v.at(10) );
		t.testEqual<double>("resize_at(136)", v_temp.at(136), pi );
	}
	
	//unsigned int capacity() const;
	
	t.testGreaterThanOrEqual<unsigned int>("capacity", v.capacity(), v.size());
	
	//bool empty() const;
	
	t.testEqual<bool>("empty_v", v.empty(), false);
	t.testEqual<bool>("empty_new", dstd::vector<float>().empty(), true);
	
	//void reserve(unsigned int n);
	
	{
		dstd::vector<double> v_temp(v);	
		unsigned int capacity_new = v_temp.capacity() * 2;
		v_temp.reserve( capacity_new );
		t.testGreaterThanOrEqual("reserve", v_temp.capacity(), capacity_new);
	}
	
	
	//
	// Modifiers
	//
	
	//void assign(const_iterator first, const_iterator last);
	{
		dstd::vector<double> v_temp;
		v_temp.assign(v.begin(), v.end());
		t.testEqual< dstd::vector<double> >("assign_iterators", v_temp, v);
	}
	
	//void assign(unsigned int n, const T& value);
	{
		dstd::vector<double> v_temp;
		v_temp.assign(10, pi);
		t.testEqual< unsigned int >("assign_size",  v_temp.size(), 10);
		t.testEqual< double >("assign_value", v_temp.at(4),  pi);
	}
	
	{
		dstd::vector<double> v_temp(v);
		unsigned int n_old = v_temp.size();
		
		//void push_back(const T& value);
		v_temp.push_back(pi);
		t.testEqual< unsigned int >("push_back_size",  v_temp.size(), n_old+1);
		t.testEqual< double >("push_back_value", v_temp.back(),  pi);
		
		//void pop_back();
		v_temp.pop_back();
		t.testEqual< unsigned int >("pop_back_size",  v_temp.size(), n_old);
	}
	
	//iterator insert(iterator position, const T& value);
	{
		dstd::vector<double> v_temp;
		v_temp.assign(100, pi);
		
		dstd::vector<double>::iterator it = v_temp.begin() + 50;
		v_temp.insert(it, 0.0);
		t.testEqual<unsigned int>("insert_it_size", v_temp.size(), 101);
		t.testEqual<double>("insert_it_at", v_temp.at(50), 0.0);
	}
	
	//iterator insert(iterator position, unsigned int n, const T& value);
	{
		dstd::vector<double> v_temp;
		v_temp.assign(100, pi);
		
		dstd::vector<double>::iterator it = v_temp.begin() + 50;
		v_temp.insert(it, 10, 0.0);
		t.testEqual<unsigned int>("insert_itN_size", v_temp.size(), 110);
		t.testEqual<double>("insert_itN_at1", v_temp.at(59), 0.0);
		t.testEqual<double>("insert_itN_at2", v_temp.at(60), pi);
	}
	
	//iterator insert(iterator position, iterator first, iterator last);
	{
		dstd::vector<double> v_temp;
		v_temp.insert(v_temp.begin(), v.begin(), v.end());
		t.testEqual< dstd::vector<double> >("insert_ititit", v_temp, v);
	}
	
	//iterator erase(iterator position);
	{
		dstd::vector<int> v_temp;
		for(unsigned int i = 0; i != 3; ++i)
		{
			v_temp.push_back(i);
		}
		v_temp.erase( v_temp.begin() + 1 );
		t.testEqual<unsigned int>("erase_it", v_temp.size(), 2);
		t.testEqual<int>("erase_it_at0", v_temp.at(0), 0);
		t.testEqual<int>("erase_it_at1", v_temp.at(1), 2);
	}
	
	//iterator erase(iterator first, iterator last);
	{
		dstd::vector<int> v_temp;
		for(unsigned int i = 0; i != 4; ++i)
		{
			v_temp.push_back(i);
		}
		v_temp.erase(v_temp.begin() + 1, v_temp.begin() + 3);
		t.testEqual<unsigned int>("erase_itit", v_temp.size(), 2);
		t.testEqual<int>("erase_itit_at0", v_temp.at(0), 0);
		t.testEqual<int>("erase_itit_at1", v_temp.at(1), 3);
	}
	
	//void swap(vector& v);
	{
		dstd::vector<double> v_temp(10, pi);
		
		dstd::vector<double> v_old(v);
		dstd::vector<double> v_temp_old(v_temp);
		
		dstd::swap(v, v_temp);
		t.testEqual< dstd::vector<double> >("swap_1a", v, v_temp_old);
		t.testEqual< dstd::vector<double> >("swap_1b", v_temp, v_old);
		
		v.swap(v_temp);
		t.testEqual< dstd::vector<double> >("swap_2a", v, v_old);
		t.testEqual< dstd::vector<double> >("swap_2b", v_temp, v_temp_old);
	}
	
	//void clear();
	v.clear();
	t.testEqual<unsigned int>("clear_size", v.size(), 0);
	t.testEqual< dstd::vector<double> >("clear_value", v, dstd::vector<double>() );
	
	
	t.report();
	
	
	return 0;
}
