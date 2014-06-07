#ifndef DSTD_MAP_HXX
#define DSTD_MAP_HXX



#include <cstdlib>
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
		template < class Key, class T, class Compare, class Allocator > class map_base;
	}
	template < class Key, class T, class Compare, class Allocator > class map;
	template < class Key, class T, class Compare, class Allocator > class multimap;
}



template <class Key, class T, class Compare, class Allocator>
class dstd::impl::map_base
{
	public:
		
		typedef Key key_type;
		typedef T mapped_type;
		typedef dstd::pair< key_type, mapped_type > value_type;
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
		
		class node;
		typedef typename Allocator::template rebind<node>::other node_allocator_type;
		typedef dstd::impl::binary_search_tree tree_type;
		
		
	public:
		
		
		/// Constructs an empty map with no elements.
		explicit map_base(const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: a(alloc), comp(compare), tree(new tree_type())
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
			return iterator( this->tree->smallest() );
		}
		
		
		const_iterator begin() const
		{
			return const_iterator( this->tree->smallest() );
		}
		
		
		/// Returns an iterator referring to the past-the-end element in the map container.
		/// If the container is empty, this function returns the same as map::begin.
		iterator end()
		{
			return iterator( this->tree->head() );
		}
		
		
		const_iterator end() const
		{
			return iterator( this->tree->head() );
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
			return( this->tree->root() == 0 );
		}
		
		
		/// Returns the number of elements in the map container.
		size_t size() const
		{
			return this->tree->size();
		}
		
		
		/// Returns the maximum number of elements that the map container can hold.
		/// This is the maximum potential size the container can reach due to known system or library implementation limitations,
		/// but the container is by no means guaranteed to be able to reach that size:
		/// it can still fail to allocate storage at any point before that size is reached.
		size_t max_size() const
		{
			return std::numeric_limits<size_t>::max();
		}
		
		
		//
		// Modifiers
		
		
		/// Removes and destroys an element from the map.
		/// @param position Iterator to the element to be removed.
		void erase(iterator position)
		{
			this->tree->remove( position.p );
			node* n = static_cast<node*>( position.p );
			this->a.destroy( n );
			this->a.deallocate( n );
		}
		
		
		/// Removes and deletes elements with key k from the map.
		/// @param k Key of the elements to be erased.
		/// @param k Key of the elements to be erased.
		/// @returns The number of elements erased
		size_t erase(const key_type& k)
		{
			dstd::pair<iterator,iterator> range = this->equal_range(k);
			size_t n = this->size();
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
			this->tree = new tree_type();
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
		// Operations
		
		
		/// Returns an iterator pointing to the first element in the container whose key is not considered to go before k
		/// (i.e., either it is equivalent or goes after).
		iterator lower_bound(const key_type& k)
		{
			node* n = static_cast<node*>( this->tree->root() );
			if( n == 0 ) return this->end();
			while( true )
			{
				if( this->comp( n->value.first, k ) )
				{
					if( this->tree->is_null( n->right() ) )
					{
						// We have reached a leaf. k doesn't exist.
						// But this leaf goes before k, so increment before returning
						iterator it(n);
						++it;
						return it;
					}
					else
					{
						n = static_cast<node*>( n->right() );
					}
				}
				else
				{
					if( this->tree->is_null( n->left() ) )
					{
						// we have reached a leaf. k may or may not exist.
						return iterator(n);
					}
					else
					{
						n = static_cast<node*>( n->left() );
					}
				}
			}
		}
		
		
		const_iterator lower_bound(const key_type& k) const
		{
			const node* n = static_cast<const node*>( this->tree->root() );
			if( n == 0 ) return this->end();
			while( true )
			{
				if( this->comp( n->value.first, k ) )
				{
					if( this->tree->is_null( n->right() ) )
					{
						// We have reached a leaf. k doesn't exist.
						// But this leaf goes before k, so increment before returning
						const_iterator it(n);
						++it;
						return it;
					}
					else
					{
						n = static_cast<const node*>( n->right() );
					}
				}
				else
				{
					if( this->tree->is_null( n->left() ) )
					{
						// we have reached a leaf. k may or may not exist.
						return const_iterator(n);
					}
					else
					{
						n = static_cast<const node*>( n->left() );
					}
				}
			}
		}
		
		
		/// Returns an iterator pointing to the first element in the container whose key is considered to go after k.
		iterator upper_bound(const key_type& k)
		{
			iterator it = this->lower_bound(k);
			while( it != this->end() && ! this->comp( k, it->first) )
			{
				++it;
			}
			return it;
		}
		
		
		const iterator upper_bound(const key_type& k) const
		{
			iterator it = this->lower_bound(k);
			while( it != this->end() && ! this->comp( k, it->first) )
			{
				++it;
			}
			return it;
		}
		
		
		/// Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.
		dstd::pair<iterator,iterator> equal_range(const key_type& k)
		{
			iterator first = this->lower_bound(k);
			iterator last = first;
			while( this->keys_equal(last->first, k) )
			{
				++last;
			}
			return dstd::pair<iterator, iterator>( first, last );
		}
		
		
		dstd::pair<const_iterator,const_iterator> equal_range(const key_type& k) const
		{
			const_iterator first = this->lower_bound(k);
			const_iterator last = first;
			while( this->keys_equal(last->first, k) )
			{
				++last;
			}
			return dstd::pair<const_iterator, const_iterator>( first, last );
		}
		
		
		//
		// Allocator
		
		
		/// Returns a copy of the allocator object associated with the map.
		allocator_type get_allocator() const
		{
			return allocator_type(this->a);
		}
		
		
	protected:
		
		
		/// Returns a pointer to the highest node in the tree with key equal to k, or 0 if there is no such node.
		node* find_node(const key_type& k, node* start = 0)
		{
			if( this->tree->empty() ) return 0;
			if( start == 0 ) start = static_cast<node*>( this->tree->root() );
			node* n = start;
			while( ! this->tree->is_null(n) )
			{
				if( this->comp( k, n->value.first ) )
				{
					n = static_cast<node*>( n->left() );
				}
				else if( this->comp( n->value.first, k ) )
				{
					n = static_cast<node*>( n->right() );
				}
				else
				{
					return n;
				}
			}
			return 0;
		}
		
		
		const node* find_node(const key_type& k, const node* start = 0) const
		{
			if( this->tree->empty() ) return 0;
			if( start == 0 ) start = static_cast<const node*>( this->tree->root() );
			const node* n = start;
			while( ! this->tree->is_null(n) )
			{
				if( this->comp( k, n->value.first ) )
				{
					n = static_cast<const node*>( n->left() );
				}
				else if( this->comp( n->value.first, k ) )
				{
					n = static_cast<const node*>( n->right() );
				}
				else
				{
					return n;
				}
			}
			return 0;
		}
		
		
		/// Under the assumption that the insertion of a node with key k is legal, finds
		/// the parent node onto which it should be inserted.
		/// Returns 0 if the tree is empty.
		node* find_insert_parent(const key_type& k, node* start = 0)
		{
			if( this->tree->empty() ) return 0;
			if( start == 0 ) start = static_cast<node*>( this->tree->root() );
			node* n = start;
			while( true )
			{
				if( this->comp( n->value.first, k ) )
				{
					if( this->tree->is_null( n->right() ) ) return n;
					else n = static_cast<node*>( n->right() );
				}
				else
				{
					if( this->tree->is_null( n->left() ) ) return n;
					else n = static_cast<node*>( n->left() );
				}
			}
		}
		
		
		/// Searches the tree for a node with key k, if that node does not exist it returns
		/// the parent under which it would be created.
		node* get_node(const key_type& k)
		{
			if( this->tree->empty() ) return 0;
			node* n = static_cast<node*>( this->tree->root() );
			while( true )
			{
				if( this->comp( k, n->value.first ) )
				{
					if( this->tree->is_null(n->left()) ) return n;
					else n = static_cast<node*>( n->left() );
				}
				else if( this->comp( n->value.first, k ) )
				{
					if( this->tree->is_null(n->right()) ) return n;
					else n = static_cast<node*>( n->right() );
				}
				else
				{
					return n;
				}
			}
		}
		
		
		const node* get_node(const key_type& k) const
		{
			if( this->tree->empty() ) return 0;
			node* n = static_cast<node*>( this->tree->root() );
			while( true )
			{
				if( this->comp( k, n->value.first ) )
				{
					if( this->tree->is_null(n->left()) ) return n;
					else n = static_cast<node*>( n->left() );
				}
				else if( this->comp( n->value.first, k ) )
				{
					if( this->tree->is_null(n->right()) ) return n;
					else n = static_cast<node*>( n->right() );
				}
				else
				{
					return n;
				}
			}
		}
		
		
		node* insert_node(const value_type& value, node* parent)
		{
			node* new_node = this->a.allocate(1);
			this->a.construct(new_node, node(value));
			if( parent == 0 )
			{
				this->tree->insert( static_cast<tree_type::node*>(new_node) );
			}
			else if( this->comp( parent->value.first, value.first ) )
			{
				this->tree->insert_right( static_cast<tree_type::node*>(new_node) , static_cast<tree_type::node*>(parent) );
			}
			else
			{
				this->tree->insert_left( static_cast<tree_type::node*>(new_node) , static_cast<tree_type::node*>(parent) );
			}
			return new_node;
		}
		
		
		void delete_nodes(tree_type::node* root)
		{
			if( ! this->tree->is_null( root ) )
			{
				this->delete_nodes( static_cast<node*>( root ) );
			}
		}
		
		
		void delete_nodes(node* root)
		{
			if( ! this->tree->is_null( static_cast<tree_type::node*>( root ) ) )
			{
				this->delete_nodes( root->left() );
				this->delete_nodes( root->right() );
				this->a.destroy(root);
				this->a.deallocate(root);
			}
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



template < class Key, class T, class Compare, class Allocator >
class dstd::impl::map_base< Key, T, Compare, Allocator >::value_compare
{
	private:
		
		typedef typename dstd::map< Key, T, Compare, Allocator >::key_compare key_compare;
		typedef typename dstd::map< Key, T, Compare, Allocator >::value_type value_type;
		
		const key_compare comp;
		
		value_compare(const key_compare& c)
			: comp(c)
		{}
		
	public:
	
		bool operator() (const value_type& a, const value_type& b) const
		{
			return comp( a.first, b.first );
		}
	
	friend class dstd::impl::map_base<Key, T, Compare, Allocator>;
};



template < class Key, class T, class Compare, class Allocator >
class dstd::impl::map_base<Key, T, Compare, Allocator>::node : public dstd::map<Key, T, Compare, Allocator>::tree_type::node
{
	private:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator >::tree_type::node node_base;
		
	public:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator >::value_type value_type;
		
		node(const value_type& v)
			: node_base(), value(v)
		{}
	
		value_type value;
};



template < class Key, class T, class Compare, class Allocator >
class dstd::impl::map_base<Key, T, Compare, Allocator>::iterator
{
	private:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator >::tree_type::node node_base;
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator >::node node;
		
		
	public:
		
		typedef typename dstd::impl::map_base< Key, T, Compare, Allocator >::value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		
		
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



template <class Key, class T, class Compare, class Allocator>
class dstd::impl::map_base<Key, T, Compare, Allocator>::const_iterator
{
	private:
		
		typedef const typename dstd::impl::map_base< Key, T, Compare, Allocator >::tree_type::node node_base;
		typedef const typename dstd::impl::map_base< Key, T, Compare, Allocator >::node node;
		
		
	public:
		
		typedef const typename dstd::impl::map_base< Key, T, Compare, Allocator >::value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		
		
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



//
// dstd::map

template < class Key, class T, class Compare = dstd::less<Key>, class Allocator = dstd::allocator<T> >
class dstd::map : public dstd::impl::map_base<Key, T, Compare, Allocator>
{
	private:
	
		typedef dstd::impl::map_base<Key, T, Compare, Allocator> map_base;
		typedef typename map_base::node node;
	
	public:
		
		typedef typename map_base::key_type key_type;
		typedef typename map_base::value_type value_type;
		typedef typename map_base::mapped_type mapped_type;
		typedef typename map_base::key_compare key_compare;
		typedef typename map_base::value_compare value_compare;
		typedef typename map_base::allocator_type allocator_type;
		typedef typename map_base::iterator iterator;
		typedef typename map_base::const_iterator const_iterator;
		typedef typename map_base::reverse_iterator reverse_iterator;
		typedef typename map_base::const_reverse_iterator const_reverse_iterator;
		
		
		//
		// Contructors
		
		
		/// Constructs an empty map with no elements.
		explicit map(const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: map_base(compare, alloc)
		{}
		
		
		/// Constructs a map with as many elements as the range [first,last),
		/// with each element constructed from its corresponding element in that range.
		template <class InputIterator>
		map(InputIterator first, InputIterator last,  const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: map_base(compare, alloc)
		{
			this->insert(first, last);
		}
		
		
		/// Copy constructor. Constructs a map with a copy of each of the elements in x.
		map (const map& x)
			: map_base(x.key_comp(), x.get_allocator())
		{
			// TODO: this might be quite time consuming for a balanced tree, because the
			// elements are being inserted in-order. Could be made more efficient?
			this->insert(x.begin(), x.end());
		}
		
		
		//
		// Destructors
		
		
		virtual ~map() {}
		
		
		//
		// Assignment
		
		
		map& operator= (const map& x)
		{
			this->clear();
			this->insert(x.begin(), x.end());
			return *this;
		}
		
		
		//
		// Element Access
		
		
		/// Access the mapped value of an element with key k.
		/// If k matches the key of an element in the container, the function returns a reference to its mapped value.
		/// If k does not match the key of any element in the container,
		/// the function inserts a new element with that key and returns a reference to its mapped value.
		mapped_type& operator[] (const key_type& k)
		{
			dstd::pair<iterator, bool> res = this->insert(value_type(k, mapped_type()));
			iterator location = res.first;
			mapped_type& v = location->second;
			return v;
			//return this->insert( value_type(k, mapped_type()) ).first->second;
		}
		
		
		//
		// Modifiers
		
		
		/// Extends the container by inserting a new element.
		/// If an element with the key value::first already exists in the map, no changes
		/// are made and an iterator to the existing value is returned.
		/// @param value The key/value pair to be inserted
		/// @returns A pair, with its first member an iterator pointing to either the newly inserted
		/// element or to the element with an equivalent key already in the map,
		/// and its second member set to true if a new element was inserted or false if an equivalent key already existed.
		dstd::pair< iterator, bool > insert(const value_type& value)
		{
			node* n = this->get_node(value.first);
			if( n == 0 )
			{
				n = this->insert_node(value, n);
				return dstd::pair<iterator,bool>( iterator(n), true );
			}
			else if( this->keys_equal(n->value.first, value.first) )
			{
				return dstd::pair<iterator,bool>( iterator(n), false );
			}
			else
			{
				n = this->insert_node(value, n);
				return dstd::pair<iterator,bool>( iterator(n), true );
			}
		}
		
		
		/// Extends the container by inserting a new element.
		/// If an element with the key value::first already exists in the map, no changes
		/// are made and an iterator to the existing value is returned.
		/// @param position An iterator pointing to an element in the map, hinting at a suitable
		/// location for the new element to be inserted.
		/// @param value The key/value pair to be inserted.
		/// @returns An iterator pointing to either the newly inserted element or
		/// to the element with an equivalent key already in the map.
		iterator insert(iterator position, const value_type& value)
		{
			// TODO
			// position is a hint as to where value can be inserted
			// making use of this could optimise insertion
			// for now just doing a non-optimised, but conforming, insert
			dstd::pair< iterator, bool > result = this->insert(value);
			return result.first;
		}
		
		
		/// Extends the container by copying elements in the range [first,last).
		/// If an element with the key value::first already exists in the map that element remains unchanged.
		/// @param first An iterator to the first element to be inserted.
		/// @param last An iterator to the first element not to be inserted.
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while( first != last )
			{
				this->insert( *first );
				++first;
			}
		}
		
		
		/// Exchanges the content of the container by the content of x, which is another map of the same type.
		void swap(map& x)
		{
			dstd::swap( this->tree, x.tree );
		}
		
		
		//
		// Operations
		
		
		/// Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
		/// otherwise it returns an iterator to map::end.
		iterator find(const key_type& k)
		{
			node* n = this->get_node(k);
			if( n != 0 && this->keys_equal(k, n->value.first) )
			{
				return iterator(n);
			}
			else
			{
				return this->end();
			}
		}
		
		
		const_iterator find(const key_type& k) const
		{
			const node* n = this->get_node(k);
			if( n != 0 && this->keys_equal(k, n->value.first) )
			{
				return const_iterator(n);
			}
			else
			{
				return this->end();
			}
		}
		
		
		/// Searches the container for elements with a key equivalent to k and returns the number of matches (1 or 0).
		size_t count(const key_type& k) const
		{
			return (this->find(k) == this->end()) ? 0 : 1;
		}
};



template <class Key, class T>
bool operator==(const dstd::map<Key,T>& a, const dstd::map<Key,T>& b)
{
	if( a.size() != b.size() ) return false;
	typename dstd::map<Key,T>::const_iterator a_it = a.begin();
	typename dstd::map<Key,T>::const_iterator b_it = b.begin();
	const typename dstd::map<Key,T>::const_iterator a_end = a.end();
	const typename dstd::map<Key,T>::const_iterator b_end = b.end();
	for( ; a_it != a_end && b_it != b_end; ++a_it, ++b_it)
	{
		if( *a_it != *b_it ) return false;
	}
	return true;
}



//
// dstd::multimap

template < class Key, class T, class Compare = dstd::less<Key>, class Allocator = dstd::allocator<T> >
class dstd::multimap : public dstd::impl::map_base<Key, T, Compare, Allocator>
{
	private:
	
		typedef dstd::impl::map_base<Key, T, Compare, Allocator> map_base;
		typedef typename map_base::node node;
	
	public:
		
		typedef typename map_base::key_type key_type;
		typedef typename map_base::value_type value_type;
		typedef typename map_base::mapped_type mapped_type;
		typedef typename map_base::key_compare key_compare;
		typedef typename map_base::value_compare value_compare;
		typedef typename map_base::allocator_type allocator_type;
		typedef typename map_base::iterator iterator;
		typedef typename map_base::const_iterator const_iterator;
		typedef typename map_base::reverse_iterator reverse_iterator;
		typedef typename map_base::const_reverse_iterator const_reverse_iterator;
		
		
		//
		// Contructors
		
		
		/// Constructs an empty map with no elements.
		explicit multimap(const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: map_base(compare, alloc)
		{}
		
		
		/// Constructs a map with as many elements as the range [first,last),
		/// with each element constructed from its corresponding element in that range.
		template <class InputIterator>
		multimap(InputIterator first, InputIterator last,  const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
			: map_base(compare, alloc)
		{
			this->insert(first, last);
		}
		
		
		/// Copy constructor. Constructs a map with a copy of each of the elements in x.
		multimap (const multimap& x)
			: map_base(x.key_comp(), x.get_allocator())
		{
			// TODO: this might be quite time consuming for a balanced tree, because the
			// elements are being inserted in-order. Could be made more efficient?
			this->insert(x.begin(), x.end());
		}
		
		
		//
		// Destructor
		
		
		virtual ~multimap() {}
		
		
		//
		// Assignment
		
		
		multimap& operator= (const multimap& x)
		{
			this->clear();
			this->insert(x.begin(), x.end());
			return *this;
		}
		
		
		//
		// Modifiers
		
		
		/// Extends the container by inserting a new element.
		/// @param value The key/value pair to be inserted.
		/// @returns An iterator to the newly inserted element.
		iterator insert(const value_type& value)
		{
			node* n = this->find_insert_parent(value.first);
			n = this->insert_node(value, n);
			return iterator(n);
		}
		
		
		/// Extends the container by inserting a new element.
		/// @param position An iterator pointing to an element in the map, hinting at a suitable
		/// location for the new element to be inserted.
		/// @param value The key/value pair to be inserted.
		/// @returns An iterator pointing to either the newly inserted element or
		/// to the element with an equivalent key already in the map.
		iterator insert(iterator position, const value_type& value)
		{
			// TODO
			// position is a hint as to where value can be inserted
			// making use of this could optimise insertion
			// for now just doing a non-optimised, but conforming, insert
			return this->insert(value);
		}
		
		
		/// Extends the container by copying elements in the range [first,last).
		/// @param first An iterator to the first element to be inserted.
		/// @param last An iterator to the first element not to be inserted.
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while( first != last )
			{
				this->insert( *first );
				++first;
			}
		}
		
		
		/// Exchanges the content of the container by the content of x, which is another map of the same type.
		void swap(multimap& x)
		{
			// TODO: why doesn't this work? Get linker error
			//dstd::swap( this->tree, x.tree );
			typename map_base::tree_type* temp = this->tree;
			this->tree = x.tree;
			x.tree = temp;
		}
		
		
		//
		// Operations
		
		
		/// Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
		/// otherwise it returns an iterator to map::end.
		iterator find(const key_type& k)
		{
			node* n = this->get_node(k);
			if( n != 0 && this->keys_equal(k, n->value.first) )
			{
				return iterator(n);
			}
			else
			{
				return this->end();
			}
		}
		
		
		const_iterator find(const key_type& k) const
		{
			const node* n = this->get_node(k);
			if( n != 0 && this->keys_equal(k, n->value.first) )
			{
				return const_iterator(n);
			}
			else
			{
				return this->end();
			}
		}
		
		
		/// Searches the container for elements with a key equivalent to k and returns the number of matches.
		size_t count(const key_type& k) const
		{
			size_t n = 0;
			const_iterator it = this->find(k);
			if( it != this->end() )
			{
				while( this->keys_equal( it->first, k ) )
				{
					++n;
					++it;
				}
			}
			return n;
		}
};



template <class Key, class T>
bool operator==(const dstd::multimap<Key,T>& a, const dstd::multimap<Key,T>& b)
{
	if( a.size() != b.size() ) return false;
	typename dstd::multimap<Key,T>::const_iterator a_it = a.begin();
	typename dstd::multimap<Key,T>::const_iterator b_it = b.begin();
	const typename dstd::multimap<Key,T>::const_iterator a_end = a.end();
	const typename dstd::multimap<Key,T>::const_iterator b_end = b.end();
	for( ; a_it != a_end && b_it != b_end; ++a_it, ++b_it)
	{
		if( *a_it != *b_it ) return false;
	}
	return true;
}



#endif
