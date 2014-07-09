#ifndef DSTD_IMPL_BINARY_SEARCH_TREE_HXX
#define DSTD_IMPL_BINARY_SEARCH_TREE_HXX



#include "binary_tree.hxx"
#include "../functional.hxx"



namespace dstd
{
	namespace impl
	{
		template <class T, class Compare, bool AllowMultiple> class binary_search_tree;
	}
}



template <class T, class Compare = dstd::less<T>, bool AllowMultiple = false >
class dstd::impl::binary_search_tree
{
	public:
		
		typedef T value_type;
		typedef typename dstd::impl::binary_tree::size_type size_type;
		class node;
		
		
		binary_search_tree(const Compare& compare = Compare())
			: tree( new dstd::impl::binary_tree() ), comp( compare )
		{}
		
		
		~binary_search_tree()
		{
			delete( this->tree );
		}
		
		
		bool empty() const { return this->tree->empty(); }
		
		
		node* find(const value_type& value)
		{
			node* n = this->root();
			while( ! this->is_null(n) )
			{
				if( this->comp(value, n->value) )
				{
					n = n->left_child();
				}
				else if( this->comp(n->value, value) )
				{
					n = n->right_child();
				}
				else
				{
					return n;
				}
			}
			
			// we didn't find it
			return this->header();
		}
		const node* find(const value_type& value) const
		{
			const node* n = this->root();
			while( ! this->is_null(n) )
			{
				if( this->comp(value, n->value) )
				{
					n = n->left_child();
				}
				else if( this->comp(n->value, value) )
				{
					n = n->right_child();
				}
				else
				{
					return n;
				}
			}
			
			// we didn't find it
			return this->header();
		}
		
		
		binary_tree::node* header() { return this->tree->header(); }
		const binary_tree::node* header() const { return this->tree->header(); }
		
		
		node* insert(node* n, node* parent = 0)
		{
			if( parent == 0 ) parent = this->root();
			
			if( this->is_null(parent) )
			{
				this->tree->insert_left(n, parent);
				return n;
			}
			else
			{
				while( true )
				{
					if( this->comp(parent->value, n->value) )
					{
						if( this->is_null(parent->right) )
						{
							this->tree->insert_right(n, parent);
							return n;
						}
						parent = parent->right_child();
					}
					else if( this->comp(n->value, parent->value) || AllowMultiple )
					{
						if( this->is_null(parent->left) )
						{
							this->tree->insert_left(n, parent);
							return n;
						}
						parent = parent->left_child();
					}
					else // the values are equal but AllowMultiple is false
					{
						return parent;
					}
				}
			}
		}
		
		
		bool is_null(const binary_tree::node* n) const { return this->tree->is_null(n); }
		
		
		node* left_most() { return static_cast<node*>( this->tree->left_most() ); }
		const node* left_most() const { return static_cast<const node*>( this->tree->left_most() ); }
		
		
		void remove(node* n)
		{
			if( ! this->is_null(n->left) && ! this->is_null(n->right) )
			{
				this->swap(n, n->prev());
			}
			this->tree->remove(n);
		}
		
		
		node* right_most() { return static_cast<node*>( this->tree->right_most() ); }
		const node* right_most() const { return static_cast<const node*>( this->tree->right_most() ); }
		
		
		node* root() {return static_cast<node*>( this->tree->root() ); }
		const node* root() const { return static_cast<const node*>( this->tree->root() ); }
		
		
		size_type size() const { return this->tree->size(); }
		
		
		void swap(node* n1, node* n2) { this->tree->swap( static_cast<binary_tree::node*>(n1), static_cast<binary_tree::node*>(n2) ); }
	
	
	private:
		
		dstd::impl::binary_tree* tree;
		Compare comp;
};



template <class T, class Compare, bool AllowMultiple>
class dstd::impl::binary_search_tree<T, Compare, AllowMultiple>::node : public dstd::impl::binary_tree::node
{
	public:
		
		typedef typename dstd::impl::binary_search_tree<T, Compare, AllowMultiple>::value_type value_type;
		
		
		node(const value_type& node_value = value_type())
			: binary_tree::node(), value( node_value )
		{}
		
		
		node* left_child() { return static_cast<node*>( this->left ); }
		const node* left_child() const { return static_cast<const node*>( this->left ); }
		
		
		node* next() { return static_cast<node*>( binary_tree::node::next() ); }
		const node* next() const { return static_cast<const node*>( binary_tree::node::next() ); }
		
		
		node* parent() { return static_cast<node*>( this->up ); }
		const node* parent() const { return static_cast<const node*>( this->up ); }
		
		
		node* prev() { return static_cast<node*>( binary_tree::node::prev() ); }
		const node* prev() const { return static_cast<const node*>( binary_tree::node::prev() ); }
		
		
		node* right_child() { return static_cast<node*>( this->right ); }
		const node* right_child() const { return static_cast<const node*>( this->right ); }
	
	public:	
		
		value_type value;
	
};


#endif
