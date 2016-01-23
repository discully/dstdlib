#ifndef DSTD_IMPL_VECTOR_BASE_HXX
#define DSTD_IMPL_VECTOR_BASE_HXX

#include <cstddef>
#include <limits>



namespace dstd
{
	namespace impl
	{
		class vector_base;
	}
}



class dstd::impl::vector_base
{
	public:

		typedef size_t size_type;


		size_t size() const
		{
			return n_data;
		}


		size_t max_size() const
		{
			return std::numeric_limits<size_t>::max();
		}


		size_t capacity() const
		{
			return n_memory;
		}


		bool empty() const
		{
			return (size() == 0);
		}


	protected:
		
		vector_base()
			: n_data(0), n_memory(0)
		{}


		~vector_base()
		{}


		size_type n_data;

		size_type n_memory;


	private:
		

		/// Not implemented
		vector_base(const vector_base&);


		/// Not implemented
		vector_base& operator= (const vector_base&);
};



#endif

