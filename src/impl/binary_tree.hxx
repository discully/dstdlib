#ifndef DSTD_IMPL_BINARY_TREE_HXX
#define DSTD_IMPL_BINARY_TREE_HXX

#include "../cstddef.hxx"



namespace dstd
{
	namespace impl
	{
		class binary_tree;
	}
}



class dstd::impl::binary_tree
{
	public:
		
		typedef size_t size_type;
		
		
		class node
		{
			public:
				
				node()
					: up(0), left(0), right(0)
				{}
				
				node(const node& x)
					: up(x.up), left(x.left), right(x.right)
				{}
				
				node& operator= (const node& x)
				{
					up = x.up;
					left = x.left;
					right = x.right;
					return *this;
				}
				
				void attach_child(node* child, bool left_child)
				{
					(left_child ? left : right) = child;
					if( child != 0 ) child->up = this;
				}
				
				bool is_header() const
				{
					// If the tree is empty, or has 1 node, then this check will be true:
					if( left != 0 && right != 0 && left == right ) return true;
					
					// If there is more than one node in the tree:
					if( ( right != 0  && right->left == this ) && ( left != 0 && left->right == this ) ) return true;
					
					return false;
				}
				
				bool is_null() const { return is_header(); } // null means either 0 or the header, and only the latter can be checked
				
				node* left_child() { return left; }
				const node* left_child() const { return left; }
				
				node* parent() { return up; }
				const node* parent() const { return up; }
				
				node* right_child() { return right; }
				const node* right_child() const { return right; }
				
				node* prev() { return const_cast<node*>( prev_node() ); }
				const node* prev() const { return prev_node(); }
				
				node* next() { return const_cast<node*>( next_node() ); }
				const node* next() const { return next_node(); }
			
			
			private:
				
				const node* prev_node() const
				{
					if( is_header() )
					{
						// this is the header, so return the last node in the tree
						return left;
					}
					else if( left != 0 ) // either this is not a leaf node, or it is the first node
					{
						if( left->is_header() ) return left; // this is the first node, so return the header
						
						// find the right-most node in the left subtree
						const node* p = left;
						while( p->right != 0 )
						{
							p = p->right;
						}
						return p;
					}
					else
					{
						// find the first ancestor for which this is a right-child
						const node* p = this;
						while( ! p->up->is_null() ) // condition is for safety, but it should never be true
						{
							if( p->up->right == p ) return p->up;
							p = p->up;
						}
					}
					
					// we should never get here
					return 0;
				}
				
				
				const node* next_node() const
				{
					if( is_header() )
					{
						// this is the header, so return the first node in the tree
						return right;
					}
					else if( right != 0 ) // either this is not a leaf node, or it is the last node
					{
						if( right->is_header() ) return right; // this is the last node, so return the header
						
						// this has a right subtree, so find the left-most node in the right subtree
						const node* p = right;
						while( p->left != 0 )
						{
							p = p->left;
						}
						return p;
					}
					else
					{
						// find the first ancestor for which this is a left-child
						const node* p = this;
						while( ! p->up->is_null() ) // condition is for safety, but it should never be true
						{
							if( p->up->left == p ) return p->up;
							p = p->up;
						}
					}
					
					// we should never get here
					return 0;
				}
				
				
			public:
				
				node* up;
				node* left;
				node* right;
		};
		
		
		
		binary_tree()
			: head( new node() ), n(0)
		{
			reset();
		}
		
		
		~binary_tree()
		{
			delete(head);
		}
		
		
		bool empty() const
		{
			return (size() == 0);
		}
		
		
		node* header() { return head; }
		const node* header() const { return head; }
		
		
		void insert_left(node* new_node, node* parent)
		{
			if( is_null( parent ) )
			{
				set_root(new_node);
				set_left_most(new_node);
				set_right_most(new_node);
				++(n);
			}
			else if( is_null( parent->left ) )			
			{
				parent->attach_child(new_node, true);
				if( parent == left_most() ) set_left_most(new_node);
				++(n);
			}
		}
		
		
		void insert_right(node* new_node, node* parent)
		{
			if( is_null( parent ) )
			{
				set_root(new_node);
				set_left_most(new_node);
				set_right_most(new_node);
				++(n);
			}
			else if( is_null( parent->right ) )
			{
				parent->attach_child(new_node, false);
				if( parent == right_most() ) set_right_most(new_node);
				++(n);
			}
		}
		
		
		bool is_null(const node* n) const
		{
			return( n == 0 || n == head );
		}
		
		
		node* left_most() { return head->right; }
		const node* left_most() const { return head->right; }
		
		
		/// Removes a node from the tree.
		/// Only nodes which do not have two children can be removed.
		void remove(node* n)
		{
			if( is_null( n->left ) && is_null( n->right ) )
			{
				// Removing a leaf-node (ie: one without any children)
				
				if( n == root() ) // removing the root node, leaving the tree empty
				{
					reset();
				}
				else // removing a regular leaf-node
				{					
					const bool left_child = (n == n->up->left);
					
					if( n == left_most() )
					{
						set_left_most( n->next() );
					}
					else if( left_child )
					{
						n->up->attach_child( 0, true );
					}
					
					if( n == right_most() )
					{
						set_right_most( n->prev() );
					}
					else if( ! left_child )
					{
						n->up->attach_child( 0, false );
					}
				}
				
				--(n);
			}
			else if( is_null(n->left) != is_null(n->right) ) // != behaves like xor
			{
				// n has exactly one child
				node* child = is_null(n->right) ? n->left : n->right;
				
				if( n == left_most()  ) set_left_most(  n->next() );
				if( n == right_most() ) set_right_most( n->prev() );
				if( n == root() ) set_root( child );
				
				if( child != root() ) n->up->attach_child(child, (n->up->left == n));
				
				--(n);
			}
		}
		
		
		node* right_most() { return head->left; }
		const node* right_most() const { return head->left; }
		
		
		node* root() { return head->up; }
		const node* root() const { return head->up; }
		
		
		size_type size() const { return n; }
		
		
		void swap(node* n1, node* n2)
		{
			if( n1->up == n2 ) // n2 is n1's parent
			{
				swap(n2, n1); // try again, with the order reversed
			}
			else
			{
				if( n2->up == n1 ) // n1 is n2's parent
				{
					const bool left_child = (n1->left == n2);
					
					// sort out the (non-header) things which point to n1 and n2	
					if( n1 != root() )
					{
						if     ( n1->up->left  == n1 ) n1->up->left  = n2;
						else if( n1->up->right == n1 ) n1->up->right = n2;
					}
					
					if( left_child )
					{
						if( ! is_null(n1->right) ) n1->right->up = n2;
						if( ! is_null(n2->right) ) n2->right->up = n1;
						if( ! is_null(n2->left ) ) n2->left->up  = n1;
					}
					else
					{
						if( ! is_null(n1->left ) ) n1->left->up  = n2;
						if( ! is_null(n2->left ) ) n2->left->up  = n1;
						if( ! is_null(n2->right) ) n2->right->up = n1;
					}
					
					// sort out n1 and n2
					node temp(*n1);
					n1->up = n2;
					n1->left  = n2->left;
					n1->right = n2->right;
					n2->up = temp.up;
					n2->left  = left_child ? n1 : temp.left;
					n2->right = left_child ? temp.right : n1;
				}
				else // n1 and n2 are not connected
				{
					// sort out the (non-header) things which point to n1 and n2	
					if( ! is_null(n1->up) )
					{
						if     ( n1->up->left  == n1 ) n1->up->left  = n2;
						else if( n1->up->right == n1 ) n1->up->right = n2;
					}
					if( ! is_null(n2->up) )
					{
						if     ( n2->up->left  == n2 ) n2->up->left  = n1;
						else if( n2->up->right == n2 ) n2->up->right = n1;
					}
					if( ! is_null(n1->left) )  n1->left->up  = n2;
					if( ! is_null(n1->right) ) n1->right->up = n2;
					if( ! is_null(n2->left) )  n2->left->up  = n1;
					if( ! is_null(n2->right) ) n2->right->up = n1;
					
					// sort out n1 and n2
					node temp(*n1);
					*n1 = *n2;
					*n2 = temp;
				}
				
				// sort out the header things which point to n1 or n2
				
				if     ( head->up == n1 ) head->up = n2;
				else if( head->up == n2 ) head->up = n1;
				
				if     ( head->left == n1 ) head->left = n2;
				else if( head->left == n2 ) head->left = n1;
				
				if     ( head->right == n1 ) head->right = n2;
				else if( head->right == n2 ) head->right = n1;
			}
		}
	
	
	private:
		
		void reset()
		{
			head->up = head;
			head->left = head;
			head->right = head;
		}
		
		void set_left_most(node* n)
		{
			head->right = n;
			n->left = head;
		}
		
		void set_right_most(node* n)
		{
			head->left = n;
			n->right = head;
		}
		
		void set_root(node* n)
		{
			head->up = n;
			n->up = head;
		}
		
		node* head;
		size_type n;
};



#endif
