#include "binary_search_tree.hxx"

#include "../exception.hxx"



using namespace dstd::impl;



//
// Tree


void binary_search_tree::insert(node* new_node, node* parent_node, bool to_left)
{
	if( parent_node == 0 )
	{
		if( this->root() != 0 ) throw dstd::out_of_range();
		
		this->header->up = new_node;
		this->header->l = new_node;
		this->header->r = new_node;
		
		new_node->l = this->header;
		new_node->r = this->header;
	}
	else 
	{
		if( to_left )
		{
			if( ! this->is_null( parent_node->left() ) ) throw dstd::out_of_range();
			parent_node->l = new_node;
			if( this->smallest() == parent_node )
			{
				this->header->r = new_node;
				new_node->l = this->header;
			}
		}
		else
		{
			if( ! this->is_null( parent_node->right() ) ) throw dstd::out_of_range();
			parent_node->r = new_node;
			if( this->largest() == parent_node )
			{
				this->header->l = new_node;
				new_node->r = this->header;
			}
		}
		
		new_node->up = parent_node;
	}
	++(this->n);
}



void binary_search_tree::remove(node* n)
{
	if( this->is_null(n) ) throw dstd::out_of_range();
	
	if( ! this->is_null( n->left() ) && ! this->is_null( n->right() ) )
	{
		// If n has two non-null children, then swap it's position with its in-order predecessor (c).
		// Then remove it from its new position.
		
		node* c = n->predecessor();
		node* temp_up = c->up;
		node* temp_left = c->l;
		node* temp_right = c->r;
		
		c->up = n->up;
		c->l = n->l;
		c->r = n->r;
		n->up = temp_up;
		n->l = temp_left;
		n->r = temp_right;
		
		if( n->up->l == c )
		{
			n->up->l = n;
		}
		else
		{
			n->up->r = n;
		}
		
		if( ! this->is_null(n->l) ) n->l->up = n;
		if( ! this->is_null(n->r) ) n->r->up = n;
		
		// n could have been root, but could not have been smallest or largest
		if( this->root() == n )
		{
			this->header->up = c;
		}
		else if( c->up->l == n )
		{
			c->up->l = c;
		}
		else
		{
			c->up->r = c;
		}
		
		this->remove(n);
	}
	else
	{	
		node *const parent = n->up;
		node *const replacement = this->is_null(n->l) ? n->r : n->l;
		
		if( ! this->is_null(replacement) )
		{
			replacement->up = parent;
		}
		
		if( ! this->is_null(parent) )
		{
			if( n == parent->l )
			{
				parent->l = replacement;
			}
			else
			{
				parent->r = replacement;
			}
		}
		
		// n could be root, smallest or largest
		if( this->root() == n ) this->header->up = replacement;
		if( this->smallest() == n ) this->header->r = parent;
		if( this->largest()  == n ) this->header->l = parent;
		
		--(this->n);
	}
}



//
// Node


binary_search_tree::node* binary_search_tree::node::next()
{
	node* n = this;
	if( n->r != 0 )
	{
		if( n->r->l == n )
		{
			// n->r is the header node
			return n->r;
		}
		else
		{
			// Get the left-most node in n's right-subtree
			n = n->r;
			while( n->l != 0 )
			{
				n = n->l;
			}
			return n;
		}
	}
	else if( n->up != 0 && n->up->l == n )
	{
		return n->up;
	}
	else
	{
		// return the parent of the first left-child, or the root node
		while( n->up != 0 && n->up->r == n )
		{
			n = n->up;
		}
		return n->up;
	}
}


binary_search_tree::node* binary_search_tree::node::predecessor()
{
	if( this->l == 0 || this->l->r == this ) return 0;
	
	binary_search_tree::node* p = this->l;
	while( p->r != 0 )
	{
		p = p->r;
	}
	return p;
}


binary_search_tree::node* binary_search_tree::node::prev()
{
	node* n = this;
	if( n->l != 0 )
	{
		if( n->l->r == n )
		{
			// n->l is the header node
			return n->l;
		}
		else
		{
			// Get the right-most node in n's left-subtree
			n = n->l;
			while( n->r != 0 )
			{
				n = n->r;
			}
			return n;
		}
	}
	else if( n->up != 0 && n->up->r == n )
	{
		return n->up;
	}
	else
	{
		// return the parent of the first right-child, or the root node
		while( n->up != 0 && n->up->l == n )
		{
			n = n->up;
		}
		return n->up;
	}
}


binary_search_tree::node* binary_search_tree::node::successor()
{
	if( this->r == 0 || this->r->l == this ) return 0;
	
	binary_search_tree::node* p = this->r;
	while( p->l != 0 )
	{
		p = p->l;
	}
	return p;
}
