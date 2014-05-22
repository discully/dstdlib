#ifndef DSTD_IMPL_BINARY_SEARCH_TREE_HXX
#define DSTD_IMPL_BINARY_SEARCH_TREE_HXX



#include <cstdlib>



namespace dstd
{
	namespace impl
	{
		class binary_search_tree;
	}
}



class dstd::impl::binary_search_tree
{
	public:
		
		/// A node in a binary_search_tree.
		/// Stores only relative positional information and not the data.
		/// Implementers should inherit from this class and store the data in the derived class.
		class node
		{
			public:
				
				/// Default Constructor. initialises all pointers to 0.
				node()
					: up(0), l(0), r(0)
				{}
				
				node(node* parent, node* left, node* right)
					: up(parent), l(left), r(right)
				{}
				
				/// Copy Constuctor
				node(const node& n)
					: up(n.up), l(n.l), r(n.r)
				{}
				
				virtual ~node(){}
				
				/// Returns this node's left-child node.
				/// @returns A pointer to this node's right-child, the header node if this
				/// is the largest node in the tree, or 0 if it has no right-child.
				node* left() { return this->l; }
				const node* left() const { return this->l; }
				
				/// Returns the next node in-order.
				/// @returns A pointer to the next in-order node, unless this is the largest
				/// node in the tree in which case it returns the header node.
				node* next();
				const node* next() const { return const_cast<const node*>(this->next()); }
				
				/// Returns this node's parent-node.
				/// @returns A pointer to the parent node, unless this is the root node
				/// in which case it returns 0.
				node* parent() { return this->up; }
				const node* parent() const { return this->up; }
				
				/// Returns this node's in-order predecessor.
				/// @returns a pointer to the node's in-order predecessor, or 0 if node is the smallest in its subtree.
				node* predecessor();
				const node* predecessor() const { return const_cast<const node*>(this->predecessor()); }
				
				/// Returns the previous node in-order.
				/// @returns A pointer to the previous in-order node, unless this is the smallest
				/// node in the tree in which case it returns the header node.
				node* prev();
				const node* prev() const { return const_cast<const node*>(this->prev()); }
				
				/// Returns this node's right-child node.
				/// @returns A pointer to this node's right-child, the header node if this
				/// is the largest node in the tree, or 0 if it has no right-child.
				const node* right() const { return this->r; }
				node* right() { return this->r; }
				
				/// Returns the node's in-order successor.
				/// @returns a pointer to the node's in-order predecessor, or 0 if node is the largest in its subtree.
				node* successor();
				const node* successor() const { return const_cast<const node*>(this->successor()); }
			
			
			private:
				
				/// Pointer to this node's parent-node, or 0 if it is the root node.
				node* up;
				
				/// Pointer to this node's left-child node, or the header node if it the smallest node in the tree
				node* l;
				
				/// Pointer to this node's right-child node, or the header node if it the largest node in the tree
				node* r;
			
			
			friend class binary_search_tree;
		};
		
		
		
		binary_search_tree()
			: header(new node(0,0,0)), n(0)
		{}
		
		const node* head() const { return this->header; }
		void insert(node* new_node, node* parent_node = 0, bool to_left = true);
		void insert_left(node* new_node, node* parent_node) { this->insert(new_node, parent_node, true); }
		void insert_right(node* new_node, node* parent_node) { this->insert(new_node, parent_node, false); }
		bool is_null(node* n) { return (n == 0 || n == this->header); }
		node* largest() { return this->header->l; }
		const node* largest() const { return this->header->l; }
		
		/// Remove a node from the tree's structure.
		/// @param n Pointer to the node to be removed.
		/// @throws dstd::out_of_range if n is 0 or the header.
		void remove(node* n);
		
		/// Returns the left-most node in the tree.
		/// @returns pointer to the left-most node in the tree, of 0 is the tree is empty
		node* smallest() { return this->header->r; }
		const node* smallest() const { return this->header->r; }
		
		node* root() { return this->header->up; }
		const node* root() const { return this->header->up; }
		
		size_t size() const { return this->n; }
		
	
	private:
		
		node* header;
		size_t n;
};


#endif
