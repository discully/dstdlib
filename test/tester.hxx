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
			this->fail();
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
			this->startTest(test_name);
			if( test_value == expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value did not equal expected");
				return false;
			}
		}
		template <class T1, class T2>
		bool testNotEqual(const std::string& test_name, const T1& test_value, const T2& expected_value)
		{
			this->startTest(test_name);
			if( test_value != expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value was equal to expected");
				return false;
			}
		}
		template <class T>
		bool testLessThan(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			this->startTest(test_name);
			if( test_value < expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value was not less than expected");
				return false;
			}
		}
		template <class T>
		bool testGreaterThan(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			this->startTest(test_name);
			if( test_value > expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value was not greater than expected");
				return false;
			}
		}
		template <class T>
		bool testLessThanOrEqual(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			this->startTest(test_name);
			if( test_value <= expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value was greater than expected");
				return false;
			}
		}
		template <class T>
		bool testGreaterThanOrEqual(const std::string& test_name, const T& test_value, const T& expected_value)
		{
			this->startTest(test_name);
			if( test_value >= expected_value )
			{
				this->testPassed();
				return true;
			}
			else
			{
				this->testFailed("value was less than expected");
				return false;
			}
		}
		
		bool registerTestFailed(const std::string& test_name, const std::string& message = "")
		{
			this->startTest(test_name);
			this->testFailed(message);
			return false;
		}
		
		bool registerTestPassed(const std::string& test_name)
		{
			this->startTest(test_name);
			this->testPassed();
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
