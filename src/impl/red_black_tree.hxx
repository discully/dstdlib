#ifndef DSTD_IMPL_RED_BLACK_TREE_HXX
#define DSTD_IMPL_RED_BLACK_TREE_HXX



#include "binary_search_tree.hxx"



// TODO



namespace dstd
{
	namespace impl
	{
		template <class T, class Compare, bool AllowEqual> class red_black_tree;
	}
}



template <class T, class Compare, bool AllowEqual>
class dstd::impl::red_black_tree : public dstd::impl::binary_search_tree<T, Compare, AllowEqual>
{
	public:
		
		typedef binary_search_tree<T, Compare, AllowEqual> tree_base;
		
		class node : public tree_base::node
		{
			public:
				
				bool colour;
		};
		
		node* insert(node* n, node* parent = 0)
		{
			node* new_node = this->tree_base::insert(n, parent);
			if( new_node == n )
			{
				// this is where the tree must be re-ordered in response
			}
			return new_node;
		}
		
		void remove(node* n)
		{
			this->tree_base::remove(n);
			// this is where the tree mush be re-ordered in response
		}
};



#endif
