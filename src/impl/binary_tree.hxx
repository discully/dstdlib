#ifndef DSTD_IMPL_BINARY_TREE_HXX
#define DSTD_IMPL_BINARY_TREE_HXX



#include <cstddef>



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
					this->up = x.up;
					this->left = x.left;
					this->right = x.right;
					return *this;
				}
				
				bool is_header() const
				{
					// If the tree is empty, or has 1 node, then this check will be true:
					if( this->left != 0 && this->right != 0 && this->left == this->right ) return true;
					
					// If there is more than one node in the tree:
					if( ( this->right != 0  && this->right->left == this ) && ( this->left != 0 && this->left->right == this ) ) return false;
					
					return false;
				}
				
				bool is_null() const { return this->is_header(); } // null means either 0 or the header, and only the latter can be checked
				
				node* left_child() { return this->left; }
				const node* left_child() const { return this->left; }
				
				node* parent() { return this->up; }
				const node* parent() const { return this->up; }
				
				node* right_child() { return this->right; }
				const node* right_child() const { return this->right; }
				
				node* prev() { return const_cast<node*>( this->prev_node() ); }
				const node* prev() const { return this->prev_node(); }
				
				node* next() { return const_cast<node*>( this->next_node() ); }
				const node* next() const { return this->next_node(); }
			
			
			private:
				
				const node* prev_node() const
				{
					if( this->is_header() )
					{
						// this is the header, so return the last node in the tree
						return this->left;
					}
					else if( this->left != 0 ) // either this is not a leaf node, or it is the first node
					{
						if( this->left->is_header() ) return this->left; // this is the first node, so return the header
						
						// find the right-most node in the left subtree
						const node* p = this->left;
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
					if( this->is_header() )
					{
						// this is the header, so return the first node in the tree
						return this->right;
					}
					else if( this->right != 0 ) // either this is not a leaf node, or it is the last node
					{
						if( this->right->is_header() ) return this->right; // this is the last node, so return the header
						
						// this has a right subtree, so find the left-most node in the right subtree
						const node* p = this->right;
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
			this->head->up = this->head;
			this->head->left = this->head;
			this->head->right = this->head;
		}
		
		
		~binary_tree()
		{
			delete(this->head);
		}
		
		
		bool empty() const
		{
			return this->is_null( this->root() );
		}
		
		
		node* header() { return this->head; }
		const node* header() const { return this->head; }
		
		
		void insert_left(node* new_node, node* parent)
		{
			if( this->is_null( parent ) )
			{
				new_node->up = this->head;
				new_node->left = this->head;
				new_node->right = this->head;
				this->head->up = new_node;
				this->head->left = new_node;
				this->head->right = new_node;
				++(this->n);
			}
			else if( this->is_null( parent->left ) )			
			{
				new_node->up = parent;
				parent->left = new_node;
				
				if( parent == this->left_most() )
				{
					new_node->left = this->head;
					this->head->right = new_node;
				}
				++(this->n);
			}
		}
		
		
		void insert_right(node* new_node, node* parent)
		{
			if( this->is_null( parent ) )
			{
				new_node->up = this->head;
				new_node->left = this->head;
				new_node->right = this->head;
				this->head->up = new_node;
				this->head->left = new_node;
				this->head->right = new_node;
				++(this->n);
			}
			else if( this->is_null( parent->right ) )
			{
				parent->right = new_node;
				new_node->up = parent;
				
				if( parent == this->right_most() )
				{
					new_node->right = this->head;
					this->head->left = new_node;
				}
				++(this->n);
			}
		}
		
		
		bool is_null(const node* n) const
		{
			return( n == 0 || n == this->head );
		}
		
		
		node* left_most() { return this->head->right; }
		const node* left_most() const { return this->head->right; }
		
		
		void remove(node* n)
		{
			if( this->is_null( n->left ) && this->is_null( n->right ) )
			{
				if( n->up->is_header() ) // removing the root node, leaving the tree empty
				{
					this->head->up = this->head;
					this->head->left = this->head;
					this->head->right = this->head;
				}
				else if( n == n->up->left )
				{
					n->up->left = n->left;
				}
				else
				{
					n->up->right = n->right;
				}
				
				if( this->head->right == n ) this->head->right = n->up;
				if( this->head->left == n ) this->head->left = n->up;
				
				--(this->n);
			}
			else if( ! this->is_null( n->left ) || ! this->is_null( n->right ) )
			{
				node* child = this->is_null(n->left) ? n->right : n->left;
				
				if( this->head->right == n ) this->head->right = n->next();
				if( this->head->left == n ) this->head->left = n->prev();
				
				child->up = n->up;
				if( n->up->is_header() )
				{
					n->up->up = child;
				}
				else if( n->up->left == n )
				{
					n->up->left = child;
				}
				else
				{
					n->up->right = child;
				}
			}
		}
		
		
		node* right_most() { return this->head->left; }
		const node* right_most() const { return this->head->left; }
		
		
		node* root() { return this->head->up; }
		const node* root() const { return this->head->up; }
		
		
		size_type size() const { return this->n; }
		
		
		void swap(node* n1, node* n2)
		{
			if( n1->up == n2 ) // n2 is n1's parent
			{
				this->swap(n2, n1); // try again, with the order reversed
			}
			else
			{
				if( n2->up == n1 ) // n1 is n2's parent
				{
					const bool left_child = (n1->left == n2);
					
					// sort out the (non-header) things which point to n1 and n2	
					if( ! this->is_null(n1->up) )
					{
						if     ( n1->up->left  == n1 ) n1->up->left  = n2;
						else if( n1->up->right == n1 ) n1->up->right = n2;
					}
					
					if( left_child )
					{
						if( ! this->is_null(n1->right) ) n1->right->up = n2;
						if( ! this->is_null(n2->right) ) n2->right->up = n1;
						if( ! this->is_null(n2->left ) ) n2->left->up  = n1;
					}
					else
					{
						if( ! this->is_null(n1->left ) ) n1->left->up  = n2;
						if( ! this->is_null(n2->left ) ) n2->left->up  = n1;
						if( ! this->is_null(n2->right) ) n2->right->up = n1;
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
					if( ! this->is_null(n1->up) )
					{
						if     ( n1->up->left  == n1 ) n1->up->left  = n2;
						else if( n1->up->right == n1 ) n1->up->right = n2;
					}
					if( ! this->is_null(n2->up) )
					{
						if     ( n2->up->left  == n2 ) n2->up->left  = n1;
						else if( n2->up->right == n2 ) n2->up->right = n1;
					}
					if( ! this->is_null(n1->left) )  n1->left->up  = n2;
					if( ! this->is_null(n1->right) ) n1->right->up = n2;
					if( ! this->is_null(n2->left) )  n2->left->up  = n1;
					if( ! this->is_null(n2->right) ) n2->right->up = n1;
					
					// sort out n1 and n2
					node temp(*n1);
					*n1 = *n2;
					*n2 = temp;
				}
				
				// sort out the header things which point to n1 or n2
				
				if     ( this->head->up == n1 ) this->head->up = n2;
				else if( this->head->up == n2 ) this->head->up = n1;
				
				if     ( this->head->left == n1 ) this->head->left = n2;
				else if( this->head->left == n2 ) this->head->left = n1;
				
				if     ( this->head->right == n1 ) this->head->right = n2;
				else if( this->head->right == n2 ) this->head->right = n1;
			}
		}
	
	
	private:
		
		node* head;
		size_type n;
};







#endif
