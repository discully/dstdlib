#ifndef DSTD_TESTER_HXX
#define DSTD_TESTER_HXX

#include <iostream>
#include <string>

namespace dstd
{
	class Tester
	{
		public:
		
		explicit Tester(std::string test_name) :n_fail(0), n_pass(0), name(test_name), current_test("") {}
		
		void startTest(const std::string& test_name)
		{
			this->current_test = test_name;
		}
		
		void testPassed()
		{
			this->pass();
			std::cout << "[      ] " << this->current_test << std::endl;
			this->endTest();
		}
		
		void testFailed(const std::string& message = "")
		{
			this->fail()
			std::cout << "[FAILED] " << this->current_test << ( (message != "") ? " : " : "" ) << message << std::endl;
			this->endTest();
		}
		
		void endTest()
		{
			this->current_test = "";
		}
		
		template <class T1, class T2>
		bool testEqual(const std::string& test_name, const T1& test_value, const T2& expected_value)
		{
			if( test_value == expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				return this->pass();
			}
			else
			{
				std::cout << "[FAILED] " << test_name << std::endl; //" Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				return this->fail();
			}
		}
		template <class T1, class T2>
		bool testNotEqual(const std::string& test_name, const T1& test_value, const T2& expected_value)
		{
			if( test_value != expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				this->pass();
				return true;
			}
			else
			{
				std::cout << "[FAILED] " << test_name << std::endl; //" Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				this->fail();
				return false;
			}
		}
		template <class T>
		bool testLessThan(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			if( test_value < expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				this->pass();
				return true;
			}
			else
			{
				std::cout << "[FAILED] " << test_name << " Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				this->fail();
				return false;
			}
		}
		template <class T>
		bool testGreaterThan(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			if( test_value > expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				this->pass();
				return true;
			}
			else
			{
				std::cout << "[FAILED] " << test_name << " Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				this->fail();
				return false;
			}
		}
		template <class T>
		bool testLessThanOrEqual(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			if( test_value <= expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				this->pass();
				return true;
			}
			else
			{
				std::cout << "[FAILED] " << test_name << " Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				this->fail();
				return false;
			}
		}
		template <class T>
		bool testGreaterThanOrEqual(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			if( test_value >= expected_value )
			{
				std::cout << "[PASSED] " << test_name << std::endl;
				this->pass();
				return true;
			}
			else
			{
				std::cout << "[FAILED] " << test_name << " Value(" << test_value << ") Expected(" << expected_value << ")" << std::endl;
				this->fail();
				return false;
			}
		}
		
		bool registerTestFailed(const std::string& test_name, const std::string& message = "")
		{
			std::cout << "[FAILED] " << test_name << " " << message << std::endl;
			this->fail();
			return false;
		}
		
		bool registerTestPassed(const std::string& test_name)
		{
			std::cout << "[PASSED] " << test_name << std::endl;
			this->pass();
			return true;
		}
		
		bool report()
		{
			std::cout << "[REPORT] " << this->name << " Pass(" << this->n_pass << ") Fail(" << this->n_fail << ")" << std::endl;
			return (this->n_fail == 0 && this->n_pass > 0);
		}
		
		private:
		
		bool fail()
		{
			++(this->n_fail);
			return false;
		}
		
		bool pass()
		{
			++(this->n_pass);
			return true;
		}
		
		unsigned int n_fail;
		unsigned int n_pass;
		const std::string name;
		std::string current_test;
	};
}

#endif
