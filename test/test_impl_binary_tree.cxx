#include <iostream>
#include <string>
#include "tester.hxx"
#include "../src/impl/binary_tree.hxx"



size_t subtree_count(const dstd::impl::binary_tree::node* n)
{
	if( n == 0 || n->is_header() )
	{
		return 0;
	}
	else
	{
		return 1 + subtree_count(n->left) + subtree_count(n->right);
	}
}

size_t traverse_tree(const dstd::impl::binary_tree::node* first, const dstd::impl::binary_tree::node* last)
{
	size_t n = 1;
	while( first != 0 && first != last && ! first->is_header() )
	{
		first = first->next();
		++n;
	}
	return n;
}


void tests(
	dstd::Tester& t,
	const std::string name,
	const dstd::impl::binary_tree& tree,
	const dstd::impl::binary_tree::node* root,
	const dstd::impl::binary_tree::node* left_most,
	const dstd::impl::binary_tree::node* right_most,
	size_t n
	)
{
	t.testEqual( name+" root",          tree.root(),                          root       );
	t.testEqual( name+" left-most",     tree.left_most(),                     left_most  );
	t.testEqual( name+" right-most",    tree.right_most(),                    right_most );
	t.testEqual( name+" empty",         tree.empty(),                         (n == 0)   );
	t.testEqual( name+" count",         tree.size(),                          n          );
	t.testEqual( name+" crawl down",    subtree_count(tree.root()),           n          );
	t.testEqual( name+" crawl accross", traverse_tree(left_most, right_most), n          );
}


std::ostream& operator<< (std::ostream& os, const dstd::impl::binary_tree::node& n)
{
	os << &n << " ( " << n.up << " )( " << n.left << ", " << n.right << " )";
	if( ! n.is_header() )
	{
		if ( n.left != 0 && n.left->is_header() ) os << " LEFT-MOST";
		if ( n.right != 0 && n.right->is_header() ) os << " RIGHT-MOST";
	}
	os << "\n";
	if( ! n.is_header() )
	{
		if( n.left != 0 && ! n.left->is_header() ) os << *(n.left);
		if( n.right != 0 && ! n.right->is_header() ) os << *(n.right);
	}
	return os;
}

std::ostream& operator<< (std::ostream& os, const dstd::impl::binary_tree& t)
{
	os << "HEAD " << t.header() << std::endl;
	os << *(t.header()) << std::endl;
	os << "ROOT " << t.root() << std::endl;
	os << *(t.root()) << std::endl;
}



int main()
{
	dstd::Tester t("dstd::impl::binary_tree");
	
	using namespace dstd::impl;
	
	binary_tree tree;
	
	binary_tree::node* n1   = new binary_tree::node();
	binary_tree::node* n2a  = new binary_tree::node();
	binary_tree::node* n2b  = new binary_tree::node();
	binary_tree::node* n3a1 = new binary_tree::node();
	binary_tree::node* n3a2 = new binary_tree::node();
	binary_tree::node* n3b1 = new binary_tree::node();
	binary_tree::node* n3b2 = new binary_tree::node();
	
	tree.insert_left( n1, 0);
	tree.insert_left( n2a, n1);
	tree.insert_right(n2b, n1);
	tree.insert_left( n3a1, n2a);
	tree.insert_right( n3a2, n2a);
	tree.insert_left( n3b1, n2b);
	tree.insert_right( n3b2, n2b);
	
	tests(t, "created", tree, n1, n3a1, n3b2, 7);
	
	
	
	tree.remove(n3a1);
	tests(t, "removeLeftMostLeaf", tree, n1, n2a, n3b2, 6);
	
	tree.remove(n2a);
	//std::cout << tree << std::endl;
	tests(t, "removeLeftMostPartialLeaf", tree, n1, n3a2, n3b2, 5);
	
	tree.remove(n3b1);
	tests(t, "removeCentralLeaf", tree, n1, n3a2, n3b2, 4);
	
	
	//
	//
	//tree.swap(n2a, n3a2);
	//tests(t, "swapLParent", tree, n1, n3a2, n3b2, 6);
	//
	//tree.swap(n1, n3b1);
	//tests(t, "swapRootLeaf", tree, n3b1, n3a2, n3b2, 6);
	
	//tree.remove(n1)
}
