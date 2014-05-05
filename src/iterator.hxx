#ifndef DSTD_ITERATOR_HXX
#define DSTD_ITERATOR_HXX



namespace dstd
{
	template <class I> class iterator_traits;
	template <class I> class reverse_iterator;
}



template <class I>
class dstd::iterator_traits
{
	public:
	typedef typename I::value_type value_type;
	typedef typename I::pointer pointer;
	typedef typename I::reference reference;
};



template <class I>
class dstd::reverse_iterator
{
	public:
	
		typedef I iterator_type;
		typedef typename iterator_traits<I>::value_type value_type;
		typedef typename iterator_traits<I>::pointer pointer;
		typedef typename iterator_traits<I>::pointer reference;
		
		// Constructors
		
		reverse_iterator();
		
		explicit reverse_iterator(iterator_type forward_it)
		{
			this->it = forward_it;
		}
		
		reverse_iterator(const reverse_iterator<I>& reverse_it)
		{
			this->it = reverse_it.base();
		}
		
		// Methods
		
		iterator_type base() const
		{
			return this->it;
		}
		
		// Operators
		
		reference operator*()
		{
			return *(this->it - 1);
		}
		
		reverse_iterator operator+(const int& n) const
		{
			return reverse_iterator< iterator_type >( this->it - n );
		}
		
		reverse_iterator& operator++()
		{
			--(this->it);
			return this;
		}
		
		reverse_iterator operator++(int)
		{
			return reverse_iterator( (this->it)-- );
		}
		
		reverse_iterator& operator+=(int n)
		{
			this->it -= n;
			return this;
		}
		
		reverse_iterator operator-(const int& n) const
		{
			return reverse_iterator( this->it + n );
		}
		
		reverse_iterator& operator--()
		{
			++(this->it);
			return this;
		}
		
		reverse_iterator operator--(int)
		{
			return reverse_iterator( (this->it)++ );
		}
		
		reverse_iterator& operator-=(int n)
		{
			this->it += n;
			return this;
		}
		
		pointer operator->()
		{
			return &(*this);
		}
		
	private:
		
		I it;
};

#endif
