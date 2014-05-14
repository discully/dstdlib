#ifndef DSTD_ITERATOR_HXX
#define DSTD_ITERATOR_HXX



namespace dstd
{
	template <class Iterator> class iterator_traits;
	template <class Iterator> class reverse_iterator;
}



template <class Iterator>
class dstd::iterator_traits
{
	public:
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};



template <class Iterator>
class dstd::reverse_iterator
{
	public:
	
		typedef Iterator iterator_type;
		typedef typename iterator_traits< iterator_type >::value_type value_type;
		typedef typename iterator_traits< iterator_type >::pointer pointer;
		typedef typename iterator_traits< iterator_type >::reference reference;
		
		// Constructors
		
		reverse_iterator();
		
		explicit reverse_iterator(iterator_type forward_it)
		{
			this->it = forward_it;
		}
		
		reverse_iterator(const reverse_iterator<iterator_type>& reverse_it)
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
			iterator_type temp(this->it);
			--temp;
			return *temp;
		}
		
		reverse_iterator operator+(const int& n) const
		{
			return reverse_iterator< iterator_type >( this->it - n );
		}
		
		reverse_iterator& operator++()
		{
			--(this->it);
			return *this;
		}
		
		reverse_iterator operator++(int)
		{
			return reverse_iterator( (this->it)-- );
		}
		
		reverse_iterator& operator+=(int n)
		{
			this->it -= n;
			return *this;
		}
		
		reverse_iterator operator-(const int& n) const
		{
			return reverse_iterator( this->it + n );
		}
		
		reverse_iterator& operator--()
		{
			++(this->it);
			return *this;
		}
		
		reverse_iterator operator--(int)
		{
			return reverse_iterator( (this->it)++ );
		}
		
		reverse_iterator& operator-=(int n)
		{
			this->it += n;
			return *this;
		}
		
		pointer operator->()
		{
			return &(*this);
		}
		
	private:
		
		iterator_type it;
};



template <class Iterator>
bool operator== (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (lhs.base() == rhs.base());
}


template <class Iterator>
bool operator!= (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return ! (lhs.base() == rhs.base());
}


template <class Iterator>
bool operator<  (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (lhs.base() > rhs.base());
}


template <class Iterator>
bool operator<= (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (lhs.base() >= rhs.base());
}


template <class Iterator>
bool operator>  (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (lhs.base() < rhs.base());
}


template <class Iterator>
bool operator>= (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (lhs.base() <= rhs.base());
}


template <class Iterator>
dstd::reverse_iterator<Iterator> operator+ (const int n, const dstd::reverse_iterator<Iterator>& rev_it)
{
	return (rev_it + n);
}


template <class Iterator>
unsigned int operator- (const dstd::reverse_iterator<Iterator>& lhs, const dstd::reverse_iterator<Iterator>& rhs)
{
	return (rhs.base() - lhs.base());
}



#endif
