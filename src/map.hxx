#ifndef DSTD_MAP_HXX
#define DSTD_MAP_HXX



#include <cstdlib>
#include <limits>

#include "impl/map_base.hxx"

#include "functional.hxx"
#include "iterator.hxx"
#include "memory.hxx"
#include "utility.hxx"



namespace dstd
{
	template < class Key, class T, class Compare, class Allocator > class map;
	template < class Key, class T, class Compare, class Allocator > class multimap;
}



//
// dstd::map

template < class Key, class T, class Compare = dstd::less<Key>, class Allocator = dstd::allocator<T> >
class dstd::map : public dstd::impl::map_base<Key, T, Compare, Allocator, false>
{
	private:
	
		typedef dstd::impl::map_base<Key, T, Compare, Allocator, false> map_base;
		typedef typename map_base::node node;
	
	public:
		
		typedef typename map_base::key_type key_type;
		typedef typename map_base::value_type value_type;
		typedef typename map_base::mapped_type mapped_type;
		typedef typename map_base::key_compare key_compare;
		typedef typename map_base::value_compare value_compare;
		typedef typename map_base::size_type size_type;
		typedef typename map_base::difference_type difference_type;
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
		map(InputIterator first, InputIterator last, const key_compare& compare = key_compare(), const allocator_type& alloc = allocator_type())
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
			size_type n = this->size();
			iterator new_node = this->insert_node(value);
			return dstd::pair<iterator, bool>( new_node, (n < this->size()) );
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
		void swap(map& x) { dstd::swap( this->tree, x.tree ); }
		
		
		//
		// Lookup
		
		
		/// Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
		/// otherwise it returns an iterator to map::end.
		iterator find(const key_type& k)
		{
			node* n = const_cast<node*>( this->find_node(k) );
			return (n == 0) ? this->end() : iterator(n);
		}
		const_iterator find(const key_type& k) const
		{
			const node* n = this->find_node(k);
			return (n == 0) ? this->end() : const_iterator(n);
		}
		
		
		/// Searches the container for elements with a key equivalent to k and returns the number of matches (1 or 0).
		size_type count(const key_type& k) const { return (this->find(k) == this->end()) ? 0 : 1; }
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



template <class Key, class T>
bool operator!=(const dstd::map<Key,T>& a, const dstd::map<Key,T>& b)
{
	return ! (a == b);
}



//
// dstd::multimap

template < class Key, class T, class Compare = dstd::less<Key>, class Allocator = dstd::allocator<T> >
class dstd::multimap : public dstd::impl::map_base<Key, T, Compare, Allocator, true>
{
	private:
	
		typedef dstd::impl::map_base<Key, T, Compare, Allocator, true> map_base;
		typedef typename map_base::node node;
	
	public:
		
		typedef typename map_base::key_type key_type;
		typedef typename map_base::value_type value_type;
		typedef typename map_base::mapped_type mapped_type;
		typedef typename map_base::key_compare key_compare;
		typedef typename map_base::value_compare value_compare;
		typedef typename map_base::size_type size_type;
		typedef typename map_base::difference_type difference_type;
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
			return iterator( this->insert_node(value) );
			//node* n = this->find_insert_parent(value.first);
			//n = this->insert_node(value, n);
			//return iterator(n);
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
		// Lookup
		
		
		/// Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
		/// otherwise it returns an iterator to map::end.
		iterator find(const key_type& k)
		{
			node* n = const_cast<node*>( this->find_node(k) );
			return (n == 0) ? this->end() : iterator(n);
		}
		const_iterator find(const key_type& k) const
		{
			const node* n = this->find_node(k);
			return (n == 0) ? this->end() : const_iterator(n);
		}
		
		
		/// Searches the container for elements with a key equivalent to k and returns the number of matches.
		size_type count(const key_type& k) const
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



template <class Key, class T>
bool operator!=(const dstd::multimap<Key,T>& a, const dstd::multimap<Key,T>& b)
{
	return ! (a == b);
}



#endif
