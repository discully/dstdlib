#ifndef DSTD_VECTOR_HXX
#define DSTD_VECTOR_HXX

#include "allocator.hxx"



//// Forward-declaring
//namespace dstd
//{
//	template <class T>
//	class allocator<T>;
//}



namespace dstd
{
	template <class T>
	class vector
	{
		public:
		
		class iterator
		{
			public:
			iterator(T* ptr = 0) : p(ptr), size(sizeof(T)) {}
			T& operator* () { return *(this->p); }
			const iterator& operator+= (const unsigned int& n) { this->p += (this->size * n); return (*this); }
			const iterator& operator-= (const unsigned int& n) { this->p -= (this->size * n); return (*this); }
			const iterator& operator+ (const unsigned int& n) { this->p += n; return this; }
			const iterator& operator- (const unsigned int& n) { this->p -= n; return this; }
			iterator& operator++() { (*this) += 1; return (*this); }
			iterator& operator++(int) { iterator temp(*this); ++(*this); return temp; }
			iterator& operator--() { (*this) -= 1; return (*this); }
			iterator& operator--(int) { iterator temp(*this); --(*this); return temp; }
			
			private:
			T* p;
			const unsigned int size;
		};
		class reverse_iterator
		{
			public:
			reverse_iterator(T* ptr = 0) : p(ptr), size(sizeof(T)) {}
			const reverse_iterator& operator+= (const unsigned int& n) { this->p -= (this->size * n); return this; }
			const reverse_iterator& operator-= (const unsigned int& n) { this->p += (this->size * n); return this; }
			const reverse_iterator& operator+ (const unsigned int& n) { this->p += n; return this; }
			const reverse_iterator& operator- (const unsigned int& n) { this->p -= n; return this; }
			reverse_iterator& operator++() { this->p += 1; return this; }
			reverse_iterator& operator++(int) { reverse_iterator temp(this); ++(*this); return temp; }
			reverse_iterator& operator--() { this->p -= 1; return this; }
			reverse_iterator& operator--(int) { reverse_iterator temp(*this); --(*this); return temp; }
			
			private:
			T* p;
			const unsigned int size;
		};
		
		// Constructors
		explicit vector();
		explicit vector(unsigned int n, const T& value = T());
		vector(iterator first, iterator last);
		vector(const vector& v);
		
		// Destructor
		~vector();
		
		// Assignment
		vector<T>& operator= (const vector<T>& v);
		
		// Iterators
		iterator begin();
		///const_iterator begin() const;
		iterator end();
		///const_iterator end() const;
		reverse_iterator rbegin();
		///const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		///const_reverse_iterator rend() const;
		
		// Capacity
		unsigned int size() const;
		unsigned int max_size() const;
		void resize(unsigned int n, const T& value = T());
		unsigned int capacity() const;
		bool empty() const;
		void reserve(unsigned int n);
		
		// Element access
		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;
		T& at(unsigned int i);
		const T& at(unsigned int i) const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;
		
		// Modifiers
		void assign(iterator first, iterator last);
		void assign(unsigned int n, const T& value);
		void push_back(const T& value);
		void pop_back();
		iterator insert(iterator position, const T& value);
		void insert(iterator position, unsigned int n, const T& value);
		void insert(iterator position, iterator first, iterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		///void swap(vector& v);
		void clear();
		
		
		
			
			
		private:
		
		allocator<T> a;
		unsigned int n_data;
		unsigned int n_memory;
		T* p;
		
		///// Friends
		///friend void dstd::swap(dstd::vector<T>& a, dstd::vector<T>& b);
	};
	
	///template <class T>
	///swap(dstd::vector<T>& a, dstd::vector<T>& b);
}
	
// Iterator operators

template <class T>
bool operator==(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

template <class T>
bool operator<(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

template <class T>
bool operator!=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

template <class T>
bool operator>(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

template <class T>
bool operator>=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

template <class T>
bool operator<=(const typename dstd::vector<T>::iterator& a, const typename dstd::vector<T>::iterator& b);

#include "vector.cxx"

#endif
