#ifndef DSTD_IMPL_MAP_BASE_HXX
#define DSTD_IMPL_MAP_BASE_HXX



#include <cstdlib>
#include <limits>

#include "binary_search_tree.hxx"

#include "../functional.hxx"
#include "../iterator.hxx"
#include "../memory.hxx"
#include "../utility.hxx"



namespace dstd
{
	namespace impl
	{
		template < class Key, class T, class Compare, class Allocator, bool AllowMultiple > class map_base;
	}
}



template <class Key, class T, class Compare, class Allocator, bool AllowMultiple>
class dstd::impl::map_base
{
	public:
		
		typedef Key key_type;
		typedef T mapped_type;
		typedef dstd::pair< key_type, mapped_type > value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Compare key_compare;
		class value_compare;
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
		
		typedef dstd::impl::binary_search_tree<value_type, value_compare, AllowMultiple> tree_type;
		typedef typename tree_type::node node;
		typedef typename Allocator::template rebind<typename tree_type::node>::other node_allocator_type;
		
		
	public:
		
		
		/// Constructs an empty map with no elements.
		explicit map_base(const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: a(alloc), comp(compare), tree(new tree_type(value_compare(compare)))
		{}
		
		
		/// Destroys the map including all its elements
		virtual ~map_base()
		{
			this->clear();
			delete(this->tree);
		}
		
		
		//
		// Iterators
		
		
		/// Returns an iterator referring to the first element in the map.
		/// If the container is empty, the returned iterator value shall not be dereferenced.
		iterator begin()
		{
			return iterator( this->tree->left_most() );
		}
		
		
		const_iterator begin() const
		{
			return const_iterator( this->tree->left_most() );
		}
		
		
		/// Returns an iterator referring to the past-the-end element in the map container.
		/// If the container is empty, this function returns the same as map::begin.
		iterator end()
		{
			return iterator( this->tree->header() );
		}
		
		
		const_iterator end() const
		{
			return iterator( this->tree->header() );
		}
		
		
		/// Returns a reverse iterator pointing to the last element in the container (i.e., its reverse beginning).
		reverse_iterator rbegin()
		{
			return reverse_iterator( this->end() );
		}
		
		
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator( this->end() );
		}
		
		
		/// Returns a reverse iterator pointing to the theoretical element right before the first element in the map container
		/// (which is considered its reverse end).
		reverse_iterator rend()
		{
			return reverse_iterator( this->begin() );
		}
		
		
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator( this->begin() );
		}
		
		
		//
		// Capacity
		
		
		/// Returns whether the map container is empty (i.e. whether its size is 0).
		bool empty() const
		{
			return( this->tree->empty() );
		}
		
		
		/// Returns the number of elements in the map container.
		size_type size() const
		{
			return this->tree->size();
		}
		
		
		/// Returns the maximum number of elements that the map container can hold.
		/// This is the maximum potential size the container can reach due to known system or library implementation limitations,
		/// but the container is by no means guaranteed to be able to reach that size:
		/// it can still fail to allocate storage at any point before that size is reached.
		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}
		
		
		//
		// Modifiers
		
		
		/// Removes and destroys an element from the map.
		/// @param position Iterator to the element to be removed.
		void erase(iterator position)
		{
			node* n = static_cast<node*>( position.p );
			this->tree->remove( n );
			this->a.destroy( n );
			this->a.deallocate( n );
		}
		
		
		/// Removes and deletes elements with key k from the map.
		/// @param k Key of the elements to be erased.
		/// @param k Key of the elements to be erased.
		/// @returns The number of elements erased
		size_type erase(const key_type& k)
		{
			dstd::pair<iterator,iterator> range = this->equal_range(k);
			size_type n = this->size();
			this->erase(range.first, range.second);
			return (n - this->size());
		}
		
		
		/// Removes and deletes elements in the range [first,last) from the map.
		/// @param first Iterator to the first element to be removed.
		/// @param last Iterator to the first element not to be removed.
		void erase(iterator first, iterator last)
		{
			while( first != last )
			{
				this->erase( first++ );
			}
		}
		
		
		/// Removes all elements from the map container (which are destroyed), leaving the container with a size of 0.
		void clear()
		{
			// since removing nodes from a balanced tree could be expensive, instead just
			// delete all nodes and reset the tree
			this->delete_nodes( this->tree->root() );
			delete(this->tree);
			this->tree = new tree_type(this->value_comp());
		}
		
		
		//
		// Observers
		
		
		/// Returns a copy of the comparison object used by the container to compare keys.
		key_compare key_comp() const
		{
			return this->comp;
		}
		
		
		/// Returns a comparison object that can be used to compare two elements to get whether the key of the first one goes before the second.
		value_compare value_comp() const
		{
			return value_compare(this->comp);
		}
		
		
		//
		// Lookup
		
		
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
		// Allocator
		
		
		/// Returns a copy of the allocator object associated with the map.
		allocator_type get_allocator() const
		{
			return allocator_type(this->a);
		}
		
		
	protected:
		
		
//		/// Returns a pointer to the highest node in the tree with key equal to k, or 0 if there is no such node.
//		node* find_node(const key_type& k, node* start = 0)
//		{
//			if( this->tree->empty() ) return 0;
//			if( start == 0 ) start = static_cast<node*>( this->tree->root() );
//			node* n = start;
//			while( ! this->tree->is_null(n) )
//			{
//				if( this->comp( k, n->value.first ) )
//				{
//					n = static_cast<node*>( n->left() );
//				}
//				else if( this->comp( n->value.first, k ) )
//				{
//					n = static_cast<node*>( n->right() );
//				}
//				else
//				{
//					return n;
//				}
//			}
//			return 0;
//		}
//		
//		
//		const node* find_node(const key_type& k, const node* start = 0) const
//		{
//			if( this->tree->empty() ) return 0;
//			if( start == 0 ) start = static_cast<const node*>( this->tree->root() );
//			const node* n = start;
//			while( ! this->tree->is_null(n) )
//			{
//				if( this->comp( k, n->value.first ) )
//				{
//					n = static_cast<const node*>( n->left() );
//				}
//				else if( this->comp( n->value.first, k ) )
//				{
//					n = static_cast<const node*>( n->right() );
//				}
//				else
//				{
//					return n;
//				}
//			}
//			return 0;
//		}
//		
//		
//		/// Under the assumption that the insertion of a node with key k is legal, finds
//		/// the parent node onto which it should be inserted.
//		/// Returns 0 if the tree is empty.
//		node* find_insert_parent(const key_type& k, node* start = 0)
//		{
//			if( this->tree->empty() ) return 0;
//			if( start == 0 ) start = static_cast<node*>( this->tree->root() );
//			node* n = start;
//			while( true )
//			{
//				if( this->comp( n->value.first, k ) )
//				{
//					if( this->tree->is_null( n->right() ) ) return n;
//					else n = static_cast<node*>( n->right() );
//				}
//				else
//				{
//					if( this->tree->is_null( n->left() ) ) return n;
//					else n = static_cast<node*>( n->left() );
//				}
//			}
//		}
		
		
//		/// Searches the tree for a node with key k, if that node does not exist it returns
//		/// the parent under which it would be created.
//		node* get_node(const key_type& k)
//		{
//			if( this->tree->empty() ) return 0;
//			node* n = static_cast<node*>( this->tree->root() );
//			while( true )
//			{
//				if( this->comp( k, n->value.first ) )
//				{
//					if( this->tree->is_null(n->left()) ) return n;
//					else n = static_cast<node*>( n->left() );
//				}
//				else if( this->comp( n->value.first, k ) )
//				{
//					if( this->tree->is_null(n->right()) ) return n;
//					else n = static_cast<node*>( n->right() );
//				}
//				else
//				{
//					return n;
//				}
//			}
//		}
//		
//		
//		const node* get_node(const key_type& k) const
//		{
//			if( this->tree->empty() ) return 0;
//			node* n = static_cast<node*>( this->tree->root() );
//			while( true )
//			{
//				if( this->comp( k, n->value.first ) )
//				{
//					if( this->tree->is_null(n->left()) ) return n;
//					else n = static_cast<node*>( n->left() );
//				}
//				else if( this->comp( n->value.first, k ) )
//				{
//					if( this->tree->is_null(n->right()) ) return n;
//					else n = static_cast<node*>( n->right() );
//				}
//				else
//				{
//					return n;
//				}
//			}
//		}


		const node* find_node(const key_type& k) const
		{
			const node* n = this->tree->root();
			while( ! this->tree->is_null(n) )
			{
				if( this->comp(k, n->value.first) )
				{
					n = n->left_child();
				}
				else if( this->comp(n->value.first, k) )
				{
					n = n->right_child();
				}
				else
				{
					return n;
				}
			}
			
			// we didn't find it
			return 0;
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
		
		
		void delete_nodes(node* root)
		{
			if( ! this->tree->is_null( root ) )
			{
				this->delete_nodes( root->left_child() );
				this->delete_nodes( root->right_child() );
				this->a.destroy(root);
				this->a.deallocate(root);
			}
		}
		
		
		const node* lower_bound_node(const key_type& k) const
		{
			if( this->empty() ) return 0;
			
			const node* n = this->tree->root();
			while( true )
			{
				if( this->comp( n->value.first, k ) )
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
		
		
		const node* upper_bound_node(const key_type& k) const
		{
			const node* n = this->lower_bound_node(k);
			while( ! this->tree->is_null(n) && ! this->comp(k, n->value.first) )
			{
				n = n->next();
			}
			return n;
		}
		
		
		dstd::pair<const node*, const node*> equal_range_nodes(const key_type& k) const
		{
			const node* lower = this->lower_bound_node(k);
			const node* upper = lower;
			while( ! this->tree->is_null(upper) && ! this->comp(k, upper->value.first) )
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



template < class Key, class T, class Compare, class Allocator, bool AllowMultiple >
class dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::value_compare
{
	private:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::key_compare key_compare;
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::value_type value_type;
		
		const key_compare comp;
		
		value_compare(const key_compare& c)
			: comp(c)
		{}
		
	public:
	
		bool operator() (const value_type& a, const value_type& b) const
		{
			return comp( a.first, b.first );
		}
	
	friend class dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>;
};



template < class Key, class T, class Compare, class Allocator, bool AllowMultiple >
class dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::iterator
{
	private:
		
		typedef dstd::impl::binary_tree::node node_base;
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::tree_type::node node;
		
		
	public:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef typename dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::size_type size_type;
		typedef typename dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::difference_type difference_type;
		
		
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



template <class Key, class T, class Compare, class Allocator, bool AllowMultiple>
class dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::const_iterator
{
	private:
		
		typedef const typename dstd::impl::binary_tree::node node_base;
		typedef const typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::node node;
		
		
	public:
		
		typedef const typename dstd::impl::map_base< Key, T, Compare, Allocator, AllowMultiple >::value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		typedef typename dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::size_type size_type;
		typedef typename dstd::impl::map_base<Key, T, Compare, Allocator, AllowMultiple>::difference_type difference_type;
		
		
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
	
	
	friend class map_base;
};



#endif
