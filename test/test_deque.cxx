#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/deque.hxx"


//
// Functions required for tester


template <class T>
std::ostream& operator<<(std::ostream& stream, const dstd::deque<T>& d)
{
	stream << "dstd::deque(";
	for(typename dstd::deque<T>::const_iterator it = d.begin(); it != d.end(); ++it)
	{
		stream << *it << ", ";
	}
	stream << ")";
	return stream;
}


template <class T, class Allocator>
std::ostream& operator<<(std::ostream& stream, const typename dstd::deque<T,Allocator>::iterator& it)
{
	//stream << "dstd::deque::iterator( c[" << it.c << " (" << it.first << ", " << it.last << ")] " << (it.p - it.first) << " ) = " << *it;
	stream << "dstd::deque::const_iterator(" << *it << ")";
	return stream;
}


template <class T>
std::ostream& operator<<(std::ostream& stream, const typename dstd::deque<T>::const_iterator& it)
{
	stream << "dstd::deque::const_iterator(" << *it << ")";
	return stream;
}



int main()
{
	dstd::Tester t("<deque>");
	
	
	
	//dstd::deque<int> d;
	//for(size_t i = 0; i < 1000; ++i)
	//{
	//	d.push_back(i);
	//	d.push_front(i);
	//	if( d.size() != 2*(i + 1) )
	//	{
	//		std::cerr << "Error after inserting " << i << " (" << d.size() << ")" << std::endl;
	//		break;
	//	}
	//}
	//return 0;
	
	
	//(constructor)
	
	const size_t empty_size = 0;
	const dstd::deque<int> empty_i;
	const dstd::deque<double> empty_d;
	t.registerTestPassed("deque::deque()");
	
	const size_t a_n = 5;
	const int a_x_i = 7;
	const double a_x_d = 3.14;
	const dstd::deque<int> a_i(a_n, a_x_i);
	const dstd::deque<double> a_d(a_n, a_x_d);
	t.registerTestPassed("deque::deque(n,value) 1");
	
	{	
		const dstd::deque<int> d0(a_i.begin(), a_i.begin());
		t.testEqual("deque::deque(first,last) 0", d0, empty_i);
	}
	
	{	
		const dstd::deque<int> d00(empty_i.begin(), empty_i.end());
		t.testEqual("deque::deque(first,last) 1", d00, empty_i);
	}
	
	{	
		const dstd::deque<int> d5(a_i.begin(), a_i.end());
		t.testEqual("deque::deque(first,last) 2", d5, a_i);
	}	
		
	{
		const dstd::deque<int> a_smaller(a_n-1, a_x_i);
		const dstd::deque<int> d4(a_i.begin(), a_i.end() - 1);
		t.testEqual("deque::deque(first,last) 3", d4, a_smaller);
	}
	
	{
		const dstd::deque<int> ee(empty_i);
		t.testEqual("deque::deque(deque) 0", ee, empty_i);
		
		const dstd::deque<int> aa(a_i);
		t.testEqual("deque::deque(deque) 1", aa, a_i);
	}
	
	
	//(destructor)
	
	{
		const std::string test_name = "deque::~deque";
		try
		{
			dstd::deque<int>* d = new dstd::deque<int>(a_i);
			delete(d);
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	
	//operator=
	
	{
		const dstd::deque<int> aa = a_i;
		t.testEqual("deque::operator= 1", aa, a_i);
	}
	
	{
		const dstd::deque<int> ee = empty_i;
		t.testEqual("deque::operator= 2", ee, empty_i);
	}
	
	
	//
	// Iterators:
	
	//begin
	
	
	//end
	
	
	//rbegin
	
	t.testEqual("deque::rbegin ", *(a_i.rbegin()), *(a_i.end()-1));
	
	
	//rend
	
	t.testEqual("deque::rend ", *(a_i.rend()-1), *(a_i.begin()));
	
	
	// Iterators
	
	t.testEqual("deque::iterator 1", empty_i.begin(), empty_i.end());
	
	{
		bool pass = true;
		dstd::deque<int> z;
		int i;
		for(i = 0; i < 10; ++i)
		{
			z.push_back(i);
		}
		i = 0;
		for(dstd::deque<int>::const_iterator it = z.begin(); it != z.end(); ++it, ++i)
		{
			if( *it != i )
			{
				pass = false;
				break;
			}
		}
		t.testEqual("deque::iterator 2", pass, true);
	}
	
	t.testEqual("deque::reverse_iterator 1", empty_i.rbegin(), empty_i.rend());
	
	{
		bool pass = true;
		dstd::deque<int> z;
		for(int i = 0; i < 10; ++i)
		{
			z.push_back(i);
		}
		dstd::deque<int>::const_iterator it = z.end();
		dstd::deque<int>::const_reverse_iterator rit = z.rbegin();
		while( rit != z.rend() )
		{
			if( *rit != *(it-1) )
			{
				pass = false;
				break;
			}
			++rit;
			--it;
		}
		t.testEqual("deque::reverse_iterator 2", pass, true);
	}
	
	
	//
	// Capacity:
	
	//size
	
	t.testEqual("deque::size 0", empty_i.size(), empty_size);
	
	t.testEqual("deque::size 1", a_i.size(), a_n);
	
	{
		const size_t n = 3000;
		const dstd::deque<double> z(n, 3.14);
		t.testEqual("deque::operator= 2", z.size(), n);
	}
	
	
	//max_size
	
	t.testGreaterThan("deque::max_size", a_i.max_size(), empty_size);
	
	
	//resize
	
	{
		dstd::deque<int> aa(a_i);
		
		aa.resize(a_n - 1, a_x_i);
		t.testEqual("deque::resize 1", aa.size(), a_n - 1);
		
		aa.resize(a_n, a_x_i);
		t.testEqual("deque::resize 2a", aa.size(), a_n);
		t.testEqual("deque::resize 2b", aa, a_i);
		
		aa.resize(0);
		t.testEqual("deque::resize 3a", aa.size(), empty_size);
		t.testEqual("deque::resize 3b", aa, empty_i);
	}
	
	
	//empty
	
	t.testEqual("deque::empty 1", empty_i.empty(), true);
	
	t.testEqual("deque::empty 2", a_i.empty(), false);
	
	
	//
	//Element access:
	
	dstd::deque<char> az;
	for(char c = 'n'; c <= 'z'; ++c)
	{
		az.push_back(c);
	}
	for(char c = 'm'; c >= 'a'; --c)
	{
		az.push_front(c);
	}
	
	dstd::deque<int> b;
	const int b_n = 20;
	for(int i = 0; i < b_n; ++i)
	{
		b.push_back(i);
	}
	const dstd::deque<int> bb(b);
	
	
	//operator[]
	
	t.testEqual("deque::operator[] 1a", bb[0], 0);
	t.testEqual("deque::operator[] 1b", bb[b_n/2], b_n/2);
	
	b[b_n/2] = 2*b_n;
	t.testEqual("deque::operator[] 2", b[b_n/2], 2*b_n);
	b[b_n/2] = b_n/2;
	
	{
		const std::string test_name = "deque::operator[] 3";
		try
		{
			int beyond = bb[2*b_n];
			t.registerTestPassed(test_name);
			beyond += 1; // avoid unused variable warnings
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	
	//at
	
	t.testEqual("deque::at 1a", bb.at(0), 0);
	t.testEqual("deque::at 1b", bb.at(b_n/2), b_n/2);
	
	b.at(b_n/2) = 2*b_n;
	t.testEqual("deque::at 2", b.at(b_n/2), 2*b_n);
	b.at(b_n/2) = b_n/2;
	
	{
		const std::string test_name = "deque::at 3";
		try
		{
			int beyond = bb.at(2*b_n);
			t.registerTestFailed(test_name, "no exception was thrown");
			beyond += 1; // avoid unused variable warnings
		}
		catch(dstd::out_of_range& e)
		{
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "the wrong exception was thrown");
		}
	}
	
	
	//front
	
	t.testEqual("deque::front 1", az.front(), 'a');
	
	t.testEqual("deque::front 2", bb.front(), 0);
	
	
	//back
	
	t.testEqual("deque::back 1", az.back(), 'z');
	
	t.testEqual("deque::back 2", bb.back(), b_n-1);

	
	//
	// Modifiers:
	
	//assign
	
	{
		dstd::deque<int> z(20, 3);
		z.assign(bb.begin(), bb.end());
		t.testEqual("deque::assign(first,last) 1", z, bb);
	}
	
	{
		dstd::deque<double> z(20, 3.14);
		z.assign(empty_d.begin(), empty_d.end());
		t.testEqual("deque::assign(first,last) 2", z, empty_d);
	}
	
	{
		dstd::deque<int> z(20, 3);
		z.assign(a_n, a_x_i);
		t.testEqual("deque::assign(n,value) 1", z, a_i);
	}
	
	{
		dstd::deque<double> z(20, 3.14);
		z.assign(0, a_x_i);
		t.testEqual("deque::assign(n,value) 2", z, empty_d);
	}
	
	
	//push_back
	
	{
		const double z_x = 3.14;
		const int z_n = 4;
		
		dstd::deque<double> z1(z_n, z_x);
		z1.push_back(z_x);
		
		const dstd::deque<double> z2(z_n+1, z_x);
		
		t.testEqual("deque::push_back", z1, z2);
	}
	
	{
		const double z_x = 5.5;
		dstd::deque<double> z(10, 3.3);
		z.push_back(z_x);
		t.testEqual("deque::push_back 2", z.back(), z_x);
	}
	
	{
		const double z_x = 3.1582;
		
		dstd::deque<double> z1;
		z1.push_back(z_x);
		
		dstd::deque<double> z2(1, z_x);
		
		t.testEqual("deque::push_back 3", z2, z1);
	}
	
	
	//push_front
	
	{
		const double z_x = 3.14;
		const int z_n = 4;
		
		dstd::deque<double> z1(z_n, z_x);
		z1.push_front(z_x);
		
		const dstd::deque<double> z2(z_n+1, z_x);
		
		t.testEqual("deque::push_front 1", z1, z2);
	}
	
	{
		const double z_x = 5.5;
		dstd::deque<double> z(10, 3.3);
		z.push_front(z_x);
		t.testEqual("deque::push_front 2", z.front(), z_x);
	}
	
	
	//pop_back
	
	{
		dstd::deque<double> z(1, 3.14);
		z.pop_back();
		t.testEqual("deque::pop_back 1", z, empty_d);
	}
	
	{
		const int z_n = 7;
		dstd::deque<int> z1(z_n,z_n), z2(z_n,z_n);
		
		z1.push_back(1);
		z1.pop_back();
		
		t.testEqual("deque::pop_back 2", z1, z2);
	}
	
	{
		dstd::deque<double> z;
		z.push_back(1.1);
		z.pop_back();
		
		t.testEqual("deque::pop_back 3", z, empty_d);
	}
	
	
	//pop_front
	
	{
		dstd::deque<double> z(1, 3.14);
		z.pop_front();
		t.testEqual("deque::pop_front 1", z, empty_d);
	}
	
	{
		const int z_n = 7;
		dstd::deque<int> z1(z_n,z_n), z2(z_n,z_n);
		
		z1.push_front(1);
		z1.pop_front();
		
		t.testEqual("deque::pop_front 2", z1, z2);
	}
	
	{
		dstd::deque<double> z;
		z.push_front(1.1);
		z.pop_front();
		
		t.testEqual("deque::pop_front 3", z, empty_d);
	}
	
	{
		std::string test_name = "deque::pop_front 4";
		try
		{
			dstd::deque<double> z;
			z.pop_front();
			t.registerTestPassed(test_name);
		}
		catch(...)
		{
			t.registerTestFailed(test_name, "an exception was thrown");
		}
	}
	
	
	//insert(position,value)
	
	{
		const double x = 3.14;
		dstd::deque<double> z1, z2;
		const dstd::deque<double> z3(1,x);
		z1.insert(z1.begin(), x);
		z2.insert(z2.end(), x);
		t.testEqual("deque::insert(position,value) 1", z1, z3);
		t.testEqual("deque::insert(position,value) 2", z2, z3);
	}
	
	{
		dstd::deque<int> z, z1, z2, z3;
		z.push_back(1); z.push_back(2); z.push_back(3);
		z1.push_back(1); z1.push_back(2);
		z2.push_back(1); z2.push_back(3);
		z3.push_back(2); z3.push_back(3);
		
		z1.insert(z1.end(), 3);
		z2.insert(z2.begin() + 1, 2);
		z3.insert(z3.begin(), 1);
		
		t.testEqual("deque::insert(position, value) 3", z1, z);
		t.testEqual("deque::insert(position, value) 4", z2, z);
		t.testEqual("deque::insert(position, value) 5", z3, z);
	}
	
	
	//insert(position,n,value)
	
	{
		const double x = 3.14;
		const int n = 30;
		const dstd::deque<double> z(n, x);
		dstd::deque<double> z1, z2;
		z1.insert(z1.begin(), n, x);
		z2.insert(z2.end(), n, x);
		
		t.testEqual("deque::insert(position,n,value) 1", z1, z);
		t.testEqual("deque::insert(position,n,value) 2", z2, z);
	}
	
	{
		dstd::deque<double> z, z1;
		const double x1 = 1.1, x2 = 2.2, x3 = 3.3;
		const int n = 3000;
		const int n_gap_0 = n / 3;
		const int n_gap_1 = 2 * (n / 3);
		for(int i = 0; i < n; ++i)
		{
			if( i < n_gap_0 )
			{
				z1.push_back(x1);
				z.push_back(x1);
			}
			else if( i >= n_gap_1 )
			{
				z1.push_back(x3);
				z.push_back(x3);
			}
			else
			{
				z.push_back(x2);
			}
		}
		
		dstd::deque<double>::iterator insert_point = z1.begin();
		while( *insert_point == x1 )
		{
			++insert_point;
		}
		
		z1.insert( insert_point, (n_gap_1 - n_gap_0), x2 );
		
		t.testEqual("deque::insert(position,n,value) 3", z1, z);
	}
	
	
	//insert(position,first,last)
	
	{
		dstd::deque<double> z1(30, 4.5);
		const dstd::deque<double> z(z1);
		z1.insert(z1.begin(), empty_d.begin(), empty_d.end());
		t.testEqual("deque::insert(position,first,last)", z1, z);
	}
	
	{
		dstd::deque<double> z;
		z.insert(z.begin(), empty_d.begin(), empty_d.end());
		t.testEqual("deque::insert(position,first,last)", z, empty_d);
	}
	
	{
		dstd::deque<int> z, z1, z2;
		const int n = 3000;
		const int n_gap_0 = n / 3;
		const int n_gap_1 = 2 * (n / 3);
		for(int i = 0; i < n; ++i)
		{
			if( i < n_gap_0 || i >= n_gap_1 )
			{
				z1.push_back(i);
			}
			else
			{
				z2.push_back(i);
			}
			z.push_back(i);
		}
		
		dstd::deque<int>::iterator insert_point = z1.begin();
		for(int i = 0; i < n; ++i)
		{
			if( *insert_point != i ) break;
			++insert_point;
		}
		
		z1.insert( insert_point, z2.begin(), z2.end() );
		
		t.testEqual("deque::insert(position,first,last) ", z1, z);
	}
	
	
	// erase
	
	{
		const int n = 10;
		const int x = 5;
		dstd::deque<int> z, zx, zz;
		for(int i = 0; i < n; ++i)
		{
			z.push_back(i);
			if( i != x ) zx.push_back(i);
			if( i == 0 || i == n-1 ) zz.push_back(i);
		}
		const dstd::deque<int> z09(z);
		const dstd::deque<int> z08(z09.begin(), z09.end() - 1);
		const dstd::deque<int> z19(z09.begin() + 1, z09.end());
		const dstd::deque<int> z18(z09.begin() + 1, z09.end() - 1);
		const dstd::deque<int> z00(1, 0);
		
		z = z09;
		z.erase(z.end() - 1);
		t.testEqual("deque::erase(position) 1", z, z08);
		
		z = z09;
		z.erase(z.begin());
		t.testEqual("deque::erase(position) 2", z, z19);
		
		z = z09;
		z.erase(z.begin() + x);
		t.testEqual("deque::erase(position) 3", z, zx);
		
		z = z00;
		z.erase(z.begin());
		t.testEqual("deque::erase(position) 4", z, empty_i);
		
		z = z09;
		z.erase(z.begin()+1, z.end()-1);
		t.testEqual("deque::erase(first,last) 1", z, zz);
		
		z = z09;
		z.erase(z.begin(), z.end());
		t.testEqual("deque::erase(first,last) 2", z, empty_i);
		
		z = z09;
		z.erase(z.begin(), z.begin()+1);
		t.testEqual("deque::erase(first,last) 3", z, z19);
		
		z = z09;
		z.erase(z.end() - 1, z.end());
		t.testEqual("deque::erase(first,last) 4", z, z08);
		
		z = z09;
		z.erase(z.begin(), z.begin());
		t.testEqual("deque::erase(first,last) 5", z, z09);
		
		z = z09;
		z.erase(z.end()-1, z.end()-1);
		t.testEqual("deque::erase(first,last) 6", z, z09);
		
		z = z09;
		z.erase(z.begin()+x, z.begin()+x);
		t.testEqual("deque::erase(first,last) 7", z, z09);
		
		z = empty_i;
		z.erase(z.begin(), z.begin());
		t.testEqual("deque::erase(first,last) 8", z, empty_i);
	}
	
	
	// swap
	
	{
		dstd::deque<double> z_1(34, 3.14), z_2(99, 12.12);
		const dstd::deque<double> zz_1(z_1), zz_2(z_2);
		
		z_1.swap(z_2);
		t.testEqual("deque::swap 1a", z_1, zz_2);
		t.testEqual("deque::swap 1b", z_2, zz_1);
		
		dstd::deque<double> z_3;
		z_3.swap(z_2);
		t.testEqual("deque::swap 2a", z_2, empty_d);
		t.testEqual("deque::swap 2b", z_3, zz_1);
	}
	
	{
		dstd::deque<double> z1, z2;
		z1.swap(z2);
		t.testEqual("deque::swap 3a", z1, empty_d);
		t.testEqual("deque::swap 3b", z2, empty_d);
	}
	
	
	//clear
	
	{
		dstd::deque<double> z(39, 2.3);
		z.clear();
		t.testEqual("deque::clear 1", z, empty_d);
	}
	
	{
		dstd::deque<double> z;
		z.clear();
		t.testEqual("deque::clear 2", z, empty_d);
	}
	
	
	//
	//Non-member function overloads
	
	// ==, !=
	{
		const dstd::deque<double> z1;
		const dstd::deque<double> z2(3, 3.14);
		const dstd::deque<double> z3(z2);
		const dstd::deque<double> z4(5, 8.3);
		
		t.testEqual("deque::operator== 1", (z1 == empty_d), true);
		t.testEqual("deque::operator== 2", (z1 == z2), false);
		t.testEqual("deque::operator== 3", (z2 == z3), true);
		t.testEqual("deque::operator== 4", (z2 == z4), false);
		
		t.testEqual("deque::operator!= 1", (z1 != empty_d), false);
		t.testEqual("deque::operator!= 2", (z1 != z2), true);
		t.testEqual("deque::operator!= 3", (z2 != z3), false);
		t.testEqual("deque::operator!= 4", (z2 != z4), true);
	}
	
	// <, >, <=, >=
	{
		dstd::deque<char> ae, ad, be;
		
		for(char x = 'a'; x != 'f'; ++x)
		{
			ae.push_back(x);
			if( x != 'e' ) ad.push_back(x);
			if( x != 'a' ) be.push_back(x);
		}
		const dstd::deque<char> ae2(ae), empty_c, empty_c2;
		
		t.testEqual("deque::operator< 0", (ae < ae2),false);
		t.testEqual("deque::operator< 1", (ae < ad), false);
		t.testEqual("deque::operator< 2", (ad < ae), true);
		t.testEqual("deque::operator< 3", (ad < be), true);
		t.testEqual("deque::operator< 4", (be < ad), false);
		t.testEqual("deque::operator< 5", (ae < be), true);
		t.testEqual("deque::operator< 6", (be < ae), false);
		t.testEqual("deque::operator< 7", (ae < empty_c), false);
		t.testEqual("deque::operator< 8", (empty_c < ae), true);
		t.testEqual("deque::operator< 9", (empty_c < empty_c2), false);
		
		t.testEqual("deque::operator<= 0", (ae <= ae2),true);
		t.testEqual("deque::operator<= 1", (ae <= ad), false);
		t.testEqual("deque::operator<= 2", (ad <= ae), true);
		t.testEqual("deque::operator<= 3", (ad <= be), true);
		t.testEqual("deque::operator<= 4", (be <= ad), false);
		t.testEqual("deque::operator<= 5", (ae <= be), true);
		t.testEqual("deque::operator<= 6", (be <= ae), false);
		t.testEqual("deque::operator<= 7", (ae <= empty_c), false);
		t.testEqual("deque::operator<= 8", (empty_c <= ae), true);
		t.testEqual("deque::operator<= 9", (empty_c <= empty_c2), true);
		
		t.testEqual("deque::operator> 0", (ae > ae2),false);
		t.testEqual("deque::operator> 1", (ae > ad), true);
		t.testEqual("deque::operator> 2", (ad > ae), false);
		t.testEqual("deque::operator> 3", (ad > be), false);
		t.testEqual("deque::operator> 4", (be > ad), true);
		t.testEqual("deque::operator> 5", (ae > be), false);
		t.testEqual("deque::operator> 6", (be > ae), true);
		t.testEqual("deque::operator> 7", (ae > empty_c), true);
		t.testEqual("deque::operator> 8", (empty_c > ae), false);
		t.testEqual("deque::operator> 9", (empty_c > empty_c2), false);
		
		t.testEqual("deque::operator>= 0", (ae >= ae2),true);
		t.testEqual("deque::operator>= 1", (ae >= ad), true);
		t.testEqual("deque::operator>= 2", (ad >= ae), false);
		t.testEqual("deque::operator>= 3", (ad >= be), false);
		t.testEqual("deque::operator>= 4", (be >= ad), true);
		t.testEqual("deque::operator>= 5", (ae >= be), false);
		t.testEqual("deque::operator>= 6", (be >= ae), true);
		t.testEqual("deque::operator>= 7", (ae >= empty_c), true);
		t.testEqual("deque::operator>= 8", (empty_c >= ae), false);
		t.testEqual("deque::operator>= 9", (empty_c >= empty_c2), true);
	}
	
	//swap
	{
		dstd::deque<double> z_0;
		dstd::deque<double> z_1(3,3.14), z_2(5, 5.5);
		const dstd::deque<double> zz_0, zz_1(z_1), zz_2(z_2);
		
		dstd::swap(z_1, z_2);
		t.testEqual("swap(deque) 1a", z_1, zz_2);
		t.testEqual("swap(deque) 1b", z_2, zz_1);
		
		dstd::swap(z_1, z_0);
		t.testEqual("swap(deque) 2a", z_1, zz_0);
		t.testEqual("swap(deque) 2b", z_0, zz_2);
	}
	
	
	t.report();
	return 0;
}
