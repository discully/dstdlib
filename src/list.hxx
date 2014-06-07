#ifndef DSTD_LIST_HXX
#define DSTD_LIST_HXX



#include <cstdlib> // size_t
#include <limits>

#include "dstd.hxx"
#include "memory.hxx"
#include "exception.hxx"
#include "iterator.hxx"



namespace dstd
{
	template <class T, class Allocator> class list;
	template <class T, class Allocator> void swap(dstd::list<T,Allocator>& l1, dstd::list<T,Allocator>& l2);
}



//
// List
//

template <class T, class Allocator = dstd::allocator<T> >
class dstd::list
{
	public:
		
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		class iterator;
		class const_iterator;
		typedef dstd::reverse_iterator<iterator> reverse_iterator;
		typedef dstd::reverse_iterator<const_iterator> const_reverse_iterator;
	
	
	private:
		
		class node_base;
		class node;
		typedef typename Allocator::template rebind<node>::other node_allocator_type;
		
	
	
	public:
		
		
		explicit list (const allocator_type& alloc = allocator_type())
			: a( node_allocator_type(alloc) ), n(new node_base(0,0))
		{
			this->n->prev = this->n;
			this->n->next = this->n;
		}
		
		
		explicit list(size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
			: a( node_allocator_type(alloc) ), n(new node_base(0,0))
		{
			this->n->prev = this->n;
			this->n->next = this->n;
			this->assign(n, val);
		}
		
		
		template <class InputIterator> list (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
			: a( node_allocator_type(alloc) ), n(new node_base(0,0))
		{
			this->n->prev = this->n;
			this->n->next = this->n;
			this->assign(first, last);
		}
		
		
		list(const list& x)
			: a( node_allocator_type(x.get_allocator()) ), n(new node_base(0,0))
		{
			this->n->prev = this->n;
			this->n->next = this->n;
			this->assign(x.begin(), x.end());
		}
		
		
		~list()
		{
			this->clear();
			delete(this->n);
		}
		
		
		list& operator= (const list& x)
		{
			this->assign(x.begin(), x.end());
			return *this;
		}
		
		
		iterator begin()
		{
			return iterator(this->n->next);
		}
		
		
		const_iterator begin() const
		{
			return const_iterator(this->n->next);
		}
		
		
		iterator end()
		{
			return iterator(this->n);
		}
		
		
		const_iterator end() const
		{
			return const_iterator(this->n);
		}
		
		
		reverse_iterator rbegin()
		{
			return reverse_iterator(this->end());
		}
		
		
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(this->end());
		}
		
		
		reverse_iterator rend()
		{
			return reverse_iterator(this->begin());
		}
		
		
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(this->begin());
		}
		
		
		bool empty() const
		{
			return (this->n->next == this->n);
		}
		
		
		size_t size() const
		{
			size_t n = 0;
			for(const_iterator it = this->begin(); it != this->end(); ++it)
			{
				++n;
			}
			return n;
		}
		
		
		size_t max_size() const
		{
			return std::numeric_limits<size_t>::max();
		}
		
		
		reference front()
		{
			return *(this->begin());
		}
		
		
		const_reference front() const
		{
			return *(this->begin());
		}
		
		
		reference back()
		{
			return *( --(this->end()) );
		}
		
		
		const_reference back() const
		{
			return *( --(this->end()) );
		}
		
		
		template <class InputIterator> void assign(InputIterator first, InputIterator last)
		{
			this->clear();
			this->insert(this->begin(), first, last);
		}
		
		
		void assign(size_t n, const value_type& val)
		{
			this->clear();
			this->insert(this->begin(), n, val);
		}
		
		
		void push_front(const value_type& val)
		{
			this->insert(this->begin(), val);
		}
		
		
		void pop_front()
		{
			this->erase(this->begin());
		}
		
		
		void push_back(const value_type& val)
		{
			this->insert( this->end(), val );
		}
		
		
		void pop_back()
		{
			this->erase(iterator(this->n->prev));
		}
		
		
		iterator insert (iterator position, const value_type& val)
		{
			// create new node
			node* new_node = this->a.allocate(1);
			this->a.construct( new_node, node(val, position.p->prev, position.p) );
			
			// put new node in list
			position.p->prev->next = new_node;
			position.p->prev = new_node;
			
			return iterator(new_node);
		}
		
		
	private:
		template <class Integer>
		void insertFix(iterator position, Integer n, const value_type& val, dstd::impl::TrueType)
		{
			for(Integer i = 0; i < n; ++i)
			{
				this->insert(position, val);
			}
		}
		
		template <class InputIterator>
		void insertFix (iterator position, InputIterator first, InputIterator last, dstd::impl::FalseType)
		{
			while(first != last)
			{
				this->insert(position, *first);
				++first;
			}
		}
	public:
		
		
		void insert (iterator position, size_t n, const value_type& val)
		{
			this->insertFix( position, n, val, dstd::impl::TrueType());
		}
		
		
		template <class InputIterator> void insert (iterator position, InputIterator first, InputIterator last)
		{
			typedef typename dstd::impl::BoolType< std::numeric_limits<InputIterator>::is_integer >::bool_type is_integer;
			this->insertFix( position, first, last, is_integer() );
		}
		
		
		iterator erase(iterator position)
		{
			if( position == this->end() )
			{
				throw dstd::out_of_range();
			}
			
			node* to_go = static_cast<node*>( position.p );
			++position;
			
			// Remove from list
			to_go->prev->next = to_go->next;
			to_go->next->prev = to_go->prev;
			
			// Delete it
			this->a.destroy(to_go);
			this->a.deallocate(to_go, 1);
			
			return position;
		}
		
		
		iterator erase(iterator first, iterator last)
		{
			while( first != last )
			{
				this->erase( first++ );
			}
			
			return first;
		}
		
		
		void swap(list& x)
		{
			node_base* temp( this->n );
			this->n = x.n;
			x.n = temp;
		}
		
		
		void resize(size_t n, value_type val = value_type())
		{
			size_t size = this->size();
			while( n < size )
			{
				this->pop_back();
				--size;
			}
			if( n > size )
			{
				this->insert(this->end(), (n-size), val);
			}
		}
		
		
		void clear()
		{
			this->erase(this->begin(), this->end());
		}
		
		
		void splice(iterator position, list& x)
		{
			this->splice(position, x, x.begin(), x.end());
		}
		
		
		void splice (iterator position, list& x, iterator i)
		{
			x.unhook(i);
			this->hook(position, i);
		}
		
		
		void splice (iterator position, list& x, iterator first, iterator last)
		{
			if( first != last )
			{
				x.unhook(first, last);
				this->hook(position, first, last);
			}
		}
		
		
		void remove(const value_type& val)
		{
			iterator it = this->begin();
			while( it != this->end() )
			{
				if( *it == val )
				{
					this->erase( it++ );
				}
				else
				{
					++it;
				}
			}
		}
		
		
		template <class Predicate> void remove_if(Predicate pred)
		{
			iterator it = this->begin();
			while( it != this->end() )
			{
				if( pred(*it) )
				{
					this->erase( it++ );
				}
				else
				{
					++it;
				}
			}
		}
		
		
		void unique()
		{
			iterator first = this->begin();
			iterator next = this->begin();
			++next;
			while( next != this->end() )
			{
				if( *next == *first )
				{
					this->erase( next++ );
				}
				else
				{
					++first;
					++next;
				}
			}
		}
		
		
		template <class BinaryPredicate> void unique (BinaryPredicate binary_pred)
		{
			iterator first = this->begin();
			iterator next = this->begin();
			++next;
			while( next != this->end() )
			{
				if( binary_pred(*next, *first) )
				{
					this->erase( next++ );
				}
				else
				{
					++first;
					++next;
				}
			}
		}
		
		
		void merge (list& x)
		{
			iterator in = this->begin();
			iterator from = x.begin();
			while( from != x.end() )
			{
				while( in != this->end() && *in < *from )
				{
					++in;
				}
				this->insert(in, *from);
				x.erase( from++ );
			}
		}
		
		
		template <class Compare> void merge (list& x, Compare comp)
		{
			iterator in = this->begin();
			iterator from = x.begin();
			while( from != x.end() )
			{
				while( in != this->end() && comp(*in,*from) )
				{
					++in;
				}
				this->insert(in, *from);
				x.erase( from++ );
			}
		}
		
		
		//TODO void sort()
		//TODO template <class Compare> void sort (Compare comp)
		
		
		void reverse()
		{
			node_base* temp = 0;
			for(iterator it = this->begin(); it != this->end(); --it)
			{
				temp = it.p->next;
				it.p->next = it.p->prev;
				it.p->prev = temp;
			}
			temp = this->n->next;
			this->n->next = this->n->prev;
			this->n->prev = temp;
		}
		
		
		allocator_type get_allocator() const
		{
			return allocator_type(this->a);
		}
		
		
	private:
		
		
		/// Hooks in an existing node to this list
		/// The user should ensure 'in' is not present in any other lists
		void hook(iterator position, iterator in)
		{
			position.p->prev->next = in.p;
			in.p->prev = position.p->prev;
			position.p->prev = in.p;
			in.p->next = position.p;
		}
		
		
		/// Hooks in existing nodes [first,last) to this list
		/// The user should ensure no nodes other than 'last' are in any other lists
		void hook(iterator position, iterator first, iterator last)
		{
			position.p->prev->next = first.p;
			first.p->prev = position.p->prev;
			
			while( first.p->next != last.p )
			{
				++first;
			}
			
			position.p->prev = first.p;
			first.p->next = position.p;
		}
		
		
		/// Remove a node from this list, without deleting it
		void unhook(iterator position)
		{
			if( position == this->end() )
			{
				throw dstd::out_of_range();
			}
			
			node_base* to_unhook( position.p );
			
			// Remove from list
			to_unhook->prev->next = to_unhook->next;
			to_unhook->next->prev = to_unhook->prev;
		}
		
		
		/// Remove nodes [first, last) from this list, without deleting them
		void unhook(iterator first, iterator last)
		{
			if( first == this->end() )
			{
				throw dstd::out_of_range();
			}
			
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}
		
		
		node_allocator_type a;
		node_base* n;
};



//
// List Node
//

template <class T, class Allocator>
class dstd::list<T, Allocator>::node_base
{
	public:
		
		node_base(node_base* prev_node, node_base* next_node)
			: prev(prev_node), next(next_node)
		{}
		
		node_base* prev;
		node_base* next;
};



template <class T, class Allocator>
class dstd::list<T, Allocator>::node : public dstd::list<T, Allocator>::node_base
{
	public:
		
		node(T node_value, node_base* prev_node, node_base* next_node)
			: node_base(prev_node, next_node), value(node_value)
		{}
		
		T value;
};



//
// List Iterators
//

template <class T, class Allocator>
class dstd::list<T, Allocator>::iterator
{
	private:
	
		typedef typename dstd::list<T, Allocator>::node_base node_base;
		typedef typename dstd::list<T, Allocator>::node node;
	
	
	public:
		
		typedef typename dstd::list<T, Allocator>::value_type value_type;
		typedef typename dstd::list<T, Allocator>::reference reference;
		typedef typename dstd::list<T, Allocator>::pointer pointer;
		
		
		iterator(node_base* node_ptr = 0)
			: p(node_ptr)
		{}
		
		
		operator dstd::list<T, Allocator>::const_iterator ()
		{
			return dstd::list<T, Allocator>::const_iterator(this->p);
		}
		
		
		reference operator*()
		{
			return ( static_cast<node*>(this->p) )->value;
		}
		
		
		const reference operator*() const
		{
			return ( static_cast<node*>(this->p) )->value;
		}
		
		
		pointer operator->()
		{
			return &( ( static_cast<node*>(this->p) )->value );
		}
		
		
		const pointer operator->() const
		{
			return &( ( static_cast<node*>(this->p) )->value );
		}
		
		
		iterator operator--()
		{
			this->p = this->p->prev;
			return *this;
		}
		
		
		iterator operator--(int)
		{
			iterator temp(*this);
			--(*this);
			return temp;
		}
		
		
		iterator operator++()
		{
			this->p = this->p->next;
			return *this;
		}
		
		
		iterator operator++(int)
		{
			iterator temp(*this);
			++(*this);
			return temp;
		}
		
		
		bool operator==(const iterator& it) const
		{
			return (this->p == it.p);
		}
		
		
		bool operator!=(const iterator& it) const
		{
			return ! (*this == it);
		}
	
	
	private:
		
		node_base* p;
	
	
	friend class dstd::list<T, Allocator>;
};



template <class T, class Allocator>
class dstd::list<T, Allocator>::const_iterator
{
	private:
	
		typedef typename dstd::list<T, Allocator>::node_base node_base;
		typedef typename dstd::list<T, Allocator>::node node;
	
	
	public:
		
		typedef typename dstd::list<T, Allocator>::value_type value_type;
		typedef typename dstd::list<T, Allocator>::const_reference reference;
		typedef typename dstd::list<T, Allocator>::const_pointer pointer;
		
		
		const_iterator(node_base* node_ptr = 0)
			: p(node_ptr)
		{}
		
		
		reference operator*() const
		{
			return ( static_cast<node*>(this->p) )->value;
		}
		
		
		pointer operator->() const
		{
			return &( ( static_cast<node*>(this->p) )->value );
		}
		
		
		const_iterator operator--()
		{
			this->p = this->p->prev;
			return *this;
		}
		
		
		const_iterator operator--(int)
		{
			iterator temp(*this);
			--(*this);
			return temp;
		}
		
		
		const_iterator operator++()
		{
			this->p = this->p->next;
			return *this;
		}
		
		
		const_iterator operator++(int)
		{
			iterator temp(*this);
			++(*this);
			return temp;
		}
		
		
		bool operator==(const const_iterator& it) const
		{
			return (this->p == it.p);
		}
		
		
		bool operator!=(const const_iterator& it) const
		{
			return ! (*this == it);
		}
	
	
	private:
		
		node_base* p;
	
	
	friend class dstd::list<T, Allocator>;
};



//
// List comparison operators
//

template<class T, class Allocator>
bool operator==( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	if( lhs.size() != rhs.size() ) return false;
	typename dstd::list<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::list<T>::const_iterator rhs_it = rhs.begin();
	const typename dstd::list<T>::const_iterator lhs_end = lhs.end();
	const typename dstd::list<T>::const_iterator rhs_end = rhs.end();
	while( lhs_it != lhs_end && rhs_it != rhs_end )
	{
		if( *lhs_it != *rhs_it ) return false;
		++lhs_it;
		++rhs_it;
	}
	return true;
}


template< class T, class Allocator >
bool operator!=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	return ! (lhs == rhs);
}


template< class T, class Allocator >
bool operator< (const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	typename dstd::list<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::list<T>::const_iterator rhs_it = rhs.begin();
	const typename dstd::list<T>::const_iterator lhs_end = lhs.end();
	const typename dstd::list<T>::const_iterator rhs_end = rhs.end();
	
	while( lhs_it != lhs_end && rhs_it != rhs_end )
	{
		if( *lhs_it < *rhs_it )
		{
			return true;
		}
		else if( *lhs_it > *rhs_it )
		{
			return false;
		}
		else
		{
			++lhs_it;
			++rhs_it;
		}
	}
	
	if( lhs_it == lhs_end && rhs_it != rhs_end )
	{
		return true;
	}
	else
	{
		return false;
	}
}


template< class T, class Allocator >
bool operator<=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	typename dstd::list<T>::const_iterator lhs_it = lhs.begin();
	typename dstd::list<T>::const_iterator rhs_it = rhs.begin();
	const typename dstd::list<T>::const_iterator lhs_end = lhs.end();
	const typename dstd::list<T>::const_iterator rhs_end = rhs.end();
	
	while( lhs_it != lhs_end && rhs_it != rhs_end )
	{
		if( *lhs_it < *rhs_it )
		{
			return true;
		}
		else if( *lhs_it > *rhs_it )
		{
			return false;
		}
		else
		{
			++lhs_it;
			++rhs_it;
		}
	}
	
	if( lhs_it == lhs_end )
	{
		return true;
	}
	else
	{
		return false;
	}
}


template< class T, class Allocator >
bool operator>( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	return ! (lhs <= rhs);
}


template< class T, class Allocator >
bool operator>=( const dstd::list<T,Allocator>& lhs, const dstd::list<T,Allocator>& rhs )
{
	return ! (lhs < rhs);
}



//
// Swap
//

template <class T, class Allocator>
void dstd::swap(dstd::list<T,Allocator>& l1, dstd::list<T,Allocator>& l2)
{
	l1.swap(l2);
}


#endif
