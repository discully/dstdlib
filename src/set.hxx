#ifndef DSTD_SET_HXX
#define DSTD_SET_HXX



#include <cstddef>
#include <limits>

#include "impl/binary_search_tree.hxx"
#include "functional.hxx"
#include "iterator.hxx"
#include "memory.hxx"
#include "utility.hxx"



namespace dstd
{
	namespace impl
	{
		template <class T, class Compare, class Allocator, bool AllowMultiple> class set_base;
	}
	template <class T, class Compare, class Allocator> class set;
	template <class T, class Compare, class Allocator> class multiset;
}



template <class T, class Compare, class Allocator, bool AllowMultiple>
class dstd::impl::set_base
{
	public:
		
		typedef T key_type;
		typedef T value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef Allocator allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		class iterator;
		class const_iterator;
		typedef dstd::reverse_iterator<iterator> reverse_iterator;
		typedef dstd::reverse_iterator<const_iterator> const_reverse_iterator;
	
	
	protected:
		
		typedef dstd::impl::binary_search_tree<T, Compare, AllowMultiple> tree_type;
		typedef typename tree_type::node node;
		typedef typename Allocator::template rebind<typename tree_type::node>::other node_allocator_type;
	
	
	public:
		
		
		//
		// Iterators
		
		
		iterator begin() { return iterator(this->tree->left_most()); }
		const_iterator begin() const { return const_iterator(this->tree->left_most()); }
		
		
		iterator end() { return iterator(this->tree->header()); }
		const_iterator end() const { return const_iterator(this->tree->header()); }
		
		
		reverse_iterator rbegin() { return reverse_iterator(this->end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }
		
		
		reverse_iterator rend() { return reverse_iterator(this->begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }
		
		
		//
		// Capacity
		
		
		bool empty() const { return this->tree->empty(); }
		
		
		size_type size() const { return this->tree->size(); }
		
		
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }
		
		
		//
		// Modifiers
		
		
		void clear()
		{
			this->delete_node(this->tree->root());
			delete(this->tree);
			this->tree = new tree_type();
		}
		
		
		void erase(iterator pos) { this->tree->remove(pos.p); }
		
		
		void erase(iterator first, iterator last)
		{
			while(first != last)
			{
				this->erase( first++ );
			}
		}
		
		
		size_type erase(const key_type key)
		{
			size_type n = this->size();
			dstd::pair<iterator,iterator> range = this->equal_range(key);
			this->erase(range.first, range.second);
			return (this->size() - n);
		}
		
		
		//
		// Lookup
		
		
		iterator find(const key_type& key)
		{
			return iterator( const_cast<node*>( this->tree->find(key) ) );
		}
		const_iterator find(const key_type& key) const
		{
			return const_iterator( this->tree->find(key) );
		}
		
		
		dstd::pair<iterator,iterator> equal_range(const key_type& key)
		{
			dstd::pair<const node*,const node*> equal = this->equal_range_nodes(key);
			iterator lower = equal.first == 0 ? this->end() : iterator(const_cast<node*>(equal.first));
			iterator upper = equal.second == 0 ? this->end() : iterator(const_cast<node*>(equal.second));
			return dstd::pair<iterator,iterator>( lower, upper );
		}
		dstd::pair<const_iterator,const_iterator> equal_range(const key_type& key) const
		{
			dstd::pair<const node*,const node*> equal = this->equal_range_nodes(key);
			const_iterator lower = equal.first == 0 ? this->end() : const_iterator(equal.first);
			const_iterator upper = equal.second == 0 ? this->end() : const_iterator(equal.second);
			return dstd::pair<const_iterator,const_iterator>( lower, upper );
		}
		
		
		/// Returns an iterator pointing to the first element in the container whose key is not considered to go before k
		/// (i.e., either it is equivalent or goes after).
		iterator lower_bound(const key_type& key)
		{
			node* n = const_cast<node*>( this->lower_bound_node(key) );
			return ( n == 0 ? this->end() : iterator(n) );
		}
		const_iterator lower_bound(const key_type& key) const
		{
			const node* n = this->lower_bound_node(key);
			return ( n == 0 ? this->end() : const_iterator(n) );
		}
		
		
		/// Returns an iterator pointing to the first element in the container whose key is considered to go after k.
		iterator upper_bound(const key_type& key)
		{
			node* n = const_cast<node*>( this->upper_bound_node(key) );
			return ( n == 0 ? this->end() : iterator(n) );
		}
		const_iterator upper_bound(const key_type& key) const
		{
			const node* n = this->upper_bound_node(key);
			return ( n == 0 ? this->end() : const_iterator(n) );
		}
		
		
		//
		// Observers
		
		
		key_compare key_comp() const { return this->comp; }
		
		
		value_compare value_comp() const { return this->comp; }
		
		
		allocator_type get_allocator() const { return allocator_type(this->a); }
	
	
	protected:
		
		
		void delete_nodes(node* root)
		{
			if( ! this->tree->is_null(root->left) ) this->delete_nodes(root->left_child());
			if( ! this->tree->is_null(root->right) ) this->delete_nodes(root->left_child());
			this->a.destroy(root);
			this->a.deallocate(root);
		}
		
		
		node* insert_node(const value_type& value, node* parent = 0)
		{
			// TODO: this is messy. if the value alread exists and we're
			// not allowed multiple, we shouldn't allocate/construct a new node
			
			node* new_node = this->a.allocate(1);
			this->a.construct(new_node, node(value));
			
			node* inserted_node = this->tree->insert( new_node );
			
			if( inserted_node != new_node )
			{
				this->a.destroy(new_node);
				this->a.deallocate(new_node);
			}
			
			return inserted_node;
		}
		
		
		const node* lower_bound_node(const key_type& key) const
		{
			if( this->empty() ) return 0;
			
			const node* n = this->tree->root();
			while( true )
			{
				if( this->comp( n->value, key ) )
				{
					if( this->tree->is_null( n->right ) )
					{
						// We have reached a leaf. k doesn't exist.
						// But this leaf goes before k, so increment before returning
						return n->next();
					}
					else
					{
						n = n->right_child();
					}
				}
				else
				{
					if( this->tree->is_null( n->left ) )
					{
						// we have reached a leaf. k may or may not exist.
						return n;
					}
					else
					{
						n = n->left_child();
					}
				}
			}
		}
		
		
		const node* upper_bound_node(const key_type& key) const
		{
			const node* n = this->lower_bound_node(key);
			while( ! this->tree->is_null(n) && ! this->comp(key, n->value) )
			{
				n = n->next();
			}
			return n;
		}
		
		
		dstd::pair<const node*, const node*> equal_range_nodes(const key_type& key) const
		{
			const node* lower = this->lower_bound_node(key);
			const node* upper = lower;
			while( ! this->tree->is_null(upper) && ! this->comp(key, upper->value.first) )
			{
				upper = upper->next();
			}
			return dstd::pair<const node*, const node*>(lower, upper);
		}
		
		
		/// Returns true if the two keys are considered to be equivalent
		bool keys_equal(const key_type& a, const key_type& b) const
		{
			return ( ! this->comp(a,b) && ! this->comp(b,a) );
		}
		
		
		node_allocator_type a;
		key_compare comp;
		tree_type* tree;
};



template <class T, class Compare, class Allocator, bool AllowMultiple>
class dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::iterator
{
	private:
		
		typedef dstd::impl::binary_tree::node node_base;
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::tree_type::node node;
		
		
	public:
		
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::size_type size_type;
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::difference_type difference_type;
		
		
		iterator()
			: p(0)
		{}
		
		
		iterator(node_base* ptr)
			: p(ptr)
		{}
		
		
		iterator(node* ptr)
			: p(static_cast<node_base*>(ptr))
		{}
		
		
		iterator(const iterator& it)
			: p(it.p)
		{}
		
		
		~iterator(){}
		
		
		iterator& operator= (const iterator& it)
		{
			this->p = it.p;
			return *this;
		}
		
		
		bool operator== (const iterator& it) const
		{
			return (this->p == it.p);
		}
		
		
		bool operator!= (const iterator& it) const
		{
			return ! (*this == it);
		}
		
		
		reference operator*()
		{
			return static_cast<node*>( this->p )->value;
		}
		
		
		const reference operator*() const
		{
			return static_cast<node*>( this->p )->value;
		}
		
		
		pointer operator->()
		{
			return &(static_cast<node*>( this->p )->value);
		}
		
		
		const pointer operator->() const
		{
			return &(static_cast<node*>( this->p )->value);
		}
		
		
		iterator operator++()
		{
			this->p = this->p->next();
			return *this;
		}
		
		
		iterator operator++(int)
		{
			iterator temp(*this);
			++(*this);
			return temp;
		}
		
		
		iterator operator--()
		{
			this->p = this->p->prev();
			return *this;
		}
		
		
		iterator operator--(int)
		{
			iterator temp(*this);
			--(*this);
			return temp;
		}
		
		
	private:
		
		node_base* p;
	
	
	friend class map_base;
};



template <class T, class Compare, class Allocator, bool AllowMultiple>
class dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::const_iterator
{
	private:
		
		typedef const typename dstd::impl::binary_tree::node node_base;
		typedef const typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::node node;
		
		
	public:
		
		typedef const typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::size_type size_type;
		typedef typename dstd::impl::set_base<T, Compare, Allocator, AllowMultiple>::difference_type difference_type;
		
		
		const_iterator()
			: p(0)
		{}
		
		
		const_iterator(node_base* ptr)
			: p(ptr)
		{}
		
		
		const_iterator(node* ptr)
			: p(static_cast<node_base*>(ptr))
		{}
		
		
		const_iterator(const const_iterator& it)
			: p(it.p)
		{}
		
		
		const_iterator(const iterator& it)
			: p(it.p)
		{}
		
		
		~const_iterator(){}
		
		
		const_iterator& operator= (const iterator& it)
		{
			this->p = it.p;
			return *this;
		}
		
		
		const_iterator& operator= (const const_iterator& it)
		{
			this->p = it.p;
			return *this;
		}
		
		
		bool operator== (const const_iterator& it) const
		{
			return (this->p == it.p);
		}
		
		
		bool operator!= (const const_iterator& it) const
		{
			return ! (*this == it);
		}
		
		
		reference operator*() const
		{
			return static_cast<node*>( this->p )->value;
		}
		
		
		pointer operator->()
		{
			return &(static_cast<node*>( this->p )->value);
		}
		
		
		const pointer operator->() const
		{
			return &(static_cast<node*>( this->p )->value);
		}
		
		
		const_iterator operator++()
		{
			this->p = this->p->next();
			return *this;
		}
		
		
		const_iterator operator++(int)
		{
			const_iterator temp(*this);
			++(*this);
			return temp;
		}
		
		
		const_iterator operator--()
		{
			this->p = this->p->prev();
			return *this;
		}
		
		
		const_iterator operator--(int)
		{
			const_iterator temp(*this);
			--(*this);
			return temp;
		}
		
		
	private:
		
		node_base* p;
	
	
	friend class set_base;
};



template < class T, class Compare = dstd::less<T>, class Allocator = dstd::allocator<T> >
class dstd::set : public dstd::impl::set_base<T, Compare, Allocator, false>
{
	private:
	
		typedef dstd::impl::set_base<T, Compare, Allocator, false> set_base;
		typedef typename set_base::node node;
	
	
	public:
		
		typedef typename set_base::key_type key_type;
		typedef typename set_base::value_type value_type;
		typedef typename set_base::mapped_type mapped_type;
		typedef typename set_base::key_compare key_compare;
		typedef typename set_base::value_compare value_compare;
		typedef typename set_base::size_type size_type;
		typedef typename set_base::difference_type difference_type;
		typedef typename set_base::allocator_type allocator_type;
		typedef typename set_base::iterator iterator;
		typedef typename set_base::const_iterator const_iterator;
		typedef typename set_base::reverse_iterator reverse_iterator;
		typedef typename set_base::const_reverse_iterator const_reverse_iterator;
		
		
		set& operator= (const set& x)
		{
			this->clear();
			this->insert(x.begin(), x.end());
		}
		
		
		//
		// Modifiers
		
		
		dstd::pair<iterator, bool> insert(const value_type& value)
		{
			size_type n = this->size();
			iterator it = this->insert_node(value);
			return dstd::pair<iterator,bool>( it, (n == this->size()) );
		}
		
		
		// TODO: optimise using hint
		iterator insert(iterator hint, const value_type& value) { return this->insert(value).first; }
		
		
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while( first != last )
			{
				this->insert(*first);
				++first;
			}
		}
		
		
		void swap(set& x)
		{
			dstd::swap(this->tree, x.tree);
		}
		
		
		//
		// Lookup
		
		
		size_type count(const key_type& key) const { return (this->find(key) == this->end()) ? 0 : 1; }
};



template < class T, class Compare = dstd::less<T>, class Allocator = dstd::allocator<T> >
class dstd::multiset : public dstd::impl::set_base<T, Compare, Allocator, true>
{
	private:
	
		typedef dstd::impl::set_base<T, Compare, Allocator, false> set_base;
		typedef typename set_base::node node;
	
	
	public:
		
		typedef typename set_base::key_type key_type;
		typedef typename set_base::value_type value_type;
		typedef typename set_base::mapped_type mapped_type;
		typedef typename set_base::key_compare key_compare;
		typedef typename set_base::value_compare value_compare;
		typedef typename set_base::size_type size_type;
		typedef typename set_base::difference_type difference_type;
		typedef typename set_base::allocator_type allocator_type;
		typedef typename set_base::iterator iterator;
		typedef typename set_base::const_iterator const_iterator;
		typedef typename set_base::reverse_iterator reverse_iterator;
		typedef typename set_base::const_reverse_iterator const_reverse_iterator;
		
		
		multiset& operator= (const multiset& x)
		{
			this->clear();
			this->insert(x.begin(), x.end());
		}
		
		
		//
		// Modifiers
		
		
		iterator insert(const value_type& value) { return iterator(this->insert_node(value)); }
		
		
		// TODO: optimise using hint
		iterator insert(iterator hint, const value_type& value) { return this->insert(value); }
		
		
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while( first != last )
			{
				this->insert(*first);
				++first;
			}
		}
		
		
		void swap(multiset& x)
		{
			dstd::swap(this->tree, x.tree);
		}
		
		
		//
		// Lookup
		
		
		size_type count(const key_type& key) const
		{
			iterator it = this->find(key);
			size_type n = 0;
			while( ! this->comp(key, *it) && it != this->end() )
			{
				++n;
				++it;
			}
			return n;
		}
};



template <class T, class Compare, class Allocator>
void dstd::swap(dstd::set<T, Compare, Allocator>& lhs, dstd::set<T, Compare, Allocator>& rhs)
{
	lhs.swap(rhs);
}



#endif
