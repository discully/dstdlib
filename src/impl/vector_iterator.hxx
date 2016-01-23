#ifndef DSTD_IMPL_VECTOR_ITERATOR_HXX
#define DSTD_IMPL_VECTOR_ITERATOR_HXX

#include "vector_impl.hxx"
#include "../iterator.hxx"



template <class T, class Allocator>
class dstd::impl::vector_impl<T, Allocator>::iterator
{
	public:

		typedef typename dstd::impl::vector_impl<T, Allocator>::value_type value_type;
		typedef typename dstd::impl::vector_impl<T, Allocator>::reference reference;
		typedef typename dstd::impl::vector_impl<T, Allocator>::pointer pointer;
		typedef typename dstd::impl::vector_impl<T, Allocator>::size_type size_type;
		typedef typename dstd::impl::vector_impl<T, Allocator>::difference_type difference_type;

		iterator(pointer ptr = 0) : p(ptr) {}
		iterator(const iterator& it) : p(it.p) {}
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		iterator& operator=(const iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		iterator& operator+=(const int& n) { this->p += n; return (*this); }
		iterator& operator-=(const int& n) { this->p -= n; return (*this); }
		iterator operator+ (const int& n) { iterator it(*this); it += n; return it; }
		iterator operator- (const int& n) { iterator it(*this); it -= n; return it; }
		// Operators with iterators
		int operator- (const iterator& rhs) { return this->p - rhs.p; }
		iterator& operator++() { (*this) += 1; return (*this); }
		iterator& operator++(int) { iterator temp(*this); ++(*this); return temp; }
		iterator& operator--() { (*this) -= 1; return (*this); }
		iterator& operator--(int) { iterator temp(*this); --(*this); return temp; }
		bool operator==(const iterator& rhs) const { return (this->p == rhs.p); }
		bool operator==(const const_iterator& rhs) const { return (this->p == rhs.p); }
		bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
		bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }
		bool operator< (const iterator& rhs) const { return (this->p < rhs.p); }
		bool operator< (const const_iterator& rhs) const { return (this->p < rhs.p); }
		bool operator> (const iterator& rhs) const { return (this->p > rhs.p); }
		bool operator> (const const_iterator& rhs) const { return (this->p > rhs.p); }
		bool operator<=(const iterator& rhs) const { return !(*this > rhs); }
		bool operator<=(const const_iterator& rhs) const { return !(*this > rhs); }
		bool operator>=(const iterator& rhs) const { return !(*this < rhs); }
		bool operator>=(const const_iterator& rhs) const { return !(*this < rhs); }

	private:

		T* p;

		friend class const_iterator;
};



template <class T, class Allocator>
class dstd::impl::vector_impl<T, Allocator>::const_iterator
{
	public:

		typedef typename dstd::impl::vector_impl<T, Allocator>::value_type value_type;
		typedef typename dstd::impl::vector_impl<T, Allocator>::const_reference  reference;
		typedef typename dstd::impl::vector_impl<T, Allocator>::const_pointer  pointer;
		typedef typename dstd::impl::vector_impl<T, Allocator>::size_type size_type;
		typedef typename dstd::impl::vector_impl<T, Allocator>::difference_type difference_type;

		const_iterator(pointer ptr = 0) : p(ptr) {}
		const_iterator(const const_iterator& it) : p(it.p) {}
		const_iterator(const iterator& it) : p(it.p) {}
		reference operator* () const { return *(this->p); }
		pointer operator-> () const { return this->p; }
		const_iterator& operator=(const const_iterator& rhs) { this->p = rhs.p; return *this; }
		const_iterator& operator=(const iterator& rhs) { this->p = rhs.p; return *this; }
		// Operators with int
		const_iterator& operator+=(const int& n) { this->p += n; return (*this); }
		const_iterator& operator-=(const int& n) { this->p -= n; return (*this); }
		const_iterator operator+ (const int& n) { const_iterator it(*this); it += n; return it; }
		const_iterator operator- (const int& n) { const_iterator it(*this); it -= n; return it; }
		// Operators with iterators
		int operator- (const const_iterator& rhs) { return this->p - rhs.p; }
		const_iterator& operator++() { (*this) += 1; return (*this); }
		const_iterator& operator++(int) { const_iterator temp(*this); ++(*this); return temp; }
		const_iterator& operator--() { (*this) -= 1; return (*this); }
		const_iterator& operator--(int) { const_iterator temp(*this); --(*this); return temp; }

		bool operator==(const const_iterator& rhs) const { return (this->p == rhs.p); }
		bool operator==(const iterator& rhs) const { return (this->p == rhs.p); }
		bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }
		bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
		bool operator< (const const_iterator& rhs) const { return (this->p < rhs.p); }
		bool operator< (const iterator& rhs) const { return (this->p < rhs.p); }
		bool operator> (const const_iterator& rhs) const { return (this->p > rhs.p); }
		bool operator> (const iterator& rhs) const { return (this->p > rhs.p); }
		bool operator<=(const const_iterator& rhs) const { return !(*this > rhs); }
		bool operator<=(const iterator& rhs) const { return !(*this > rhs); }
		bool operator>=(const const_iterator& rhs) const { return !(*this < rhs); }
		bool operator>=(const iterator& rhs) const { return !(*this < rhs); }

	private:

		pointer p;

		friend class iterator;
};



#endif
