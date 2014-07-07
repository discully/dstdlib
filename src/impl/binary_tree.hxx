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
					
					// If there is more than one node in the tree, then either of these checks could be used:
					if( this->left != 0  && this->left->up  != this ) return true;
					//if( this->right != 0 && this->right->up != this ) return true;
					
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
						return this->left;
					}
					else if( this->left != 0 ) // either this is not a leaf node, or it is the first node
					{
						if( this->left->is_header() ) return this->left; // We are the first node, so return the header node
						
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
						return this->right;
					}
					else if( this->right != 0 ) // either this is not a leaf node, or it is the last node
					{
						if( this->right->is_header() ) return this->right; // We are the last node, so return the header node
						
						// find the left-most node in the right subtree
						const node* p = this->right;
						while( p->left != 0 )
						{
							p = p->left;
						}
						return p;
					}
					else
					{
						// finr the first ancestor for which this is a left-child
						const node* p = this;
						while( ! p->up->is_null() ) // condition is for safety, but ois should never be true
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
				std::cout << "i1" << std::endl;
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
				if( this->is_null( n->up ) ) // removing the root node, leaving the tree empty
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
					n2->up = n1->up;
					n1->up = n2;
					
					node temp = *n1;
					if( n1->left == n2 )
					{
						n1->right = n2->right;
						n2->right = temp.right;	
						n1->left = n2->left;
						n2->left = n1;
					}
					else
					{
						n1->left = n2->left;
						n2->left = temp.left;
						n1->right = n2->right;
						n2->right = n1;
					}
				}
				else // they are not connected
				{
					node temp = *n1;
					*n1 = *n2;
					*n2 = temp;
				}
				
				if( n1->up == this->head )
				{
					this->head->up = n1;
				}
				else if( n2->up == this->head )
				{
					this->head->up = n2;
				}
				
				if( n1->left == this->head )
				{
					this->head->right = n1;
				}
				else if( n2->left == this->head )
				{
					this->head->right = n2;
				}
				
				if( n1->right == this->head )
				{
					this->head->left = n1;
				}
				else if( n2->right == this->head )
				{
					this->head->left = n2;
				}
			}
		}
	
	
	private:
		
		node* head;
		size_type n;
};







#endif
