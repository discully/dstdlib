#ifndef DSTD_ALGORITHM_HXX
#define DSTD_ALGORITHM_HXX



#include "iterator.hxx"
#include "utility.hxx"



namespace dstd
{
	namespace impl
	{
		template <class RandomIterator> void heap_drop(RandomIterator first, RandomIterator last, RandomIterator element);
		template <class RandomIterator, class Compare> void heap_drop(RandomIterator first, RandomIterator last, RandomIterator element, Compare comp);
		template <class RandomIterator>	RandomIterator heap_child(RandomIterator first, RandomIterator last, RandomIterator parent);
		template <class RandomIterator> RandomIterator heap_max_child(RandomIterator first, RandomIterator last, RandomIterator parent);
		template <class RandomIterator, class Compare> RandomIterator heap_max_child(RandomIterator first, RandomIterator last, RandomIterator parent, Compare comp);
		template <class RandomIterator> RandomIterator heap_parent(RandomIterator first, RandomIterator element);
		
		template <class RandomIterator> void make_heap(RandomIterator first, RandomIterator last, RandomIterator parent);
		template <class RandomIterator, class Compare> void make_heap(RandomIterator first, RandomIterator last, RandomIterator parent, Compare comp);
	}
	
	
	//
	// Non-modifying sequence operations
	
	
	template <class ForwardIterator, class UnaryFunction>
	UnaryFunction for_each(ForwardIterator first, ForwardIterator last, UnaryFunction f)
	{
		for( ; first != last; ++first)
		{
			f( *first );
		}
		return f;
	}
	
	
	template <class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for( ; first != last; ++first )
		{
			if( *first == value ) ++n;
		}
		return n;
	}
	
	
	template <class InputIterator, class UnaryPredicate>
	typename iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, const UnaryPredicate p)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for( ; first != last; ++first )
		{
			if( p(*first) ) ++n;
		}
		return n;
	}
	
	
	template <class InputIterator1, class InputIterator2>
	dstd::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first_1, InputIterator1 last_1, InputIterator2 first_2)
	{
		for( ; first_1 != last_1; ++first_1, ++first_2)
		{
			if( *first_1 != *first_2 ) break;
		}
		return dstd::pair<InputIterator1,InputIterator2>( first_1, first_2 );
	}
	
	
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	dstd::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first_1, InputIterator1 last_1, InputIterator2 first_2, BinaryPredicate p)
	{
		for( ; first_1 != last_1; ++first_1, ++first_2)
		{
			if( ! p(*first_1, *first_2) ) break;
		}
		return dstd::pair<InputIterator1,InputIterator2>( first_1, first_2 );
	}
	
	
	template <class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first_1, InputIterator1 last_1, InputIterator2 first_2)
	{
		for( ; first_1 != last_1; ++first_1, ++first_2)
		{
			if( *first_1 != *first_2 ) return false;
		}
		return true;
	}
	
	
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool equal(InputIterator1 first_1, InputIterator1 last_1, InputIterator2 first_2, BinaryPredicate p)
	{
		for( ; first_1 != last_1; ++first_1, ++first_2)
		{
			if( ! p(*first_1, *first_2) ) return false;
		}
		return true;
	}
	
	
	template <class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value)
	{
		for( ; first != last; ++first)
		{
			if( *first == value ) break;
		}
		return first;
	}
	
	
	template <class InputIterator, class UnaryPredicate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate p)
	{
		for( ; first != last; ++first)
		{
			if( p( *first ) ) break;
		}
		return first;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_first_of(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 set_first, ForwardIterator2 set_last)
	{
		ForwardIterator2 set_it;
		for( ; first != last; ++first)
		{
			for(set_it = set_first; set_it != set_last; ++set_it)
			{
				if( *first == *set_it ) return first;
			}
		}
		return first;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_first_of(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 set_first, ForwardIterator2 set_last, BinaryPredicate p)
	{
		ForwardIterator2 set_it;
		for( ; first != last; ++first)
		{
			for(set_it = set_first; set_it != set_last; ++set_it)
			{
				if( p(*first, *set_it) ) return first;
			}
		}
		return first;
	}
	
	
	template <class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	{
		if( first == last ) return last;
		
		ForwardIterator second = first;
		++second;
		
		for( ; second != last; ++first, ++second)
		{
			if( *first == *second ) return first;
		}
		return last;
	}
	
	
	template <class ForwardIterator, class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate p)
	{
		if( first == last ) return last;
		
		ForwardIterator second = first;
		++second;
		
		for( ; second != last; ++first, ++second)
		{
			if( p(*first, *second) ) return first;
		}
		return last;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sub_first, ForwardIterator2 sub_last)
	{
		if( sub_first == sub_last ) return first;
		
		ForwardIterator2 sub_it = sub_first;
		ForwardIterator1 result;
		
		for( ; first != last; ++first)
		{
			if( sub_it == sub_first ) result = first;
			
			if( *first == *sub_it )
			{
				++sub_it;
			}
			else
			{
				sub_it = sub_first;
			}
			
			if( sub_it == sub_last ) return result;
		}
		return last;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 search(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sub_first, ForwardIterator2 sub_last, BinaryPredicate p)
	{
		if( sub_first == sub_last ) return first;
		
		ForwardIterator2 sub_it = sub_first;
		ForwardIterator1 result;
		
		for( ; first != last; ++first)
		{
			if( sub_it == sub_first ) result = first;
			
			if( p(*first, *sub_it) )
			{
				++sub_it;
			}
			else
			{
				sub_it = sub_first;
			}
			
			if( sub_it == sub_last ) return result;
		}
		return last;
	}
	
	
	template <class ForwardIterator, class Size, class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value)
	{
		Size i = 0;
		ForwardIterator result;
		
		for( ; first != last; ++first)
		{
			if( i == 0 ) result = first;
			
			if( *first == value )
			{
				++i;
				if( i >= count ) return result; // >= instead of == deals with count = 0
			}
			else
			{
				i = 0;
			}
		}
		
		return last;
	}
	
	
	template <class ForwardIterator, class Size, class T, class BinaryPredicate>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value, BinaryPredicate p)
	{
		Size i = 0;
		ForwardIterator result;
		
		for( ; first != last; ++first)
		{
			if( i == 0 ) result = first;
			
			if( p(*first, value) )
			{
				++i;
				if( i >= count ) return result; // >= instead of == deals with count = 0
			}
			else
			{
				i = 0;
			}
		}
		
		return last;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sub_first, ForwardIterator2 sub_last)
	{
		if( sub_first == sub_last ) return last;
		ForwardIterator1 search_result;
		ForwardIterator1 result = last;
		while( true )
		{
			search_result = dstd::search(first, last, sub_first, sub_last);
			if( search_result == last ) return result;
			result = search_result;
			++first;
		}
		return result;
	}
	
	
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_end(ForwardIterator1 first, ForwardIterator1 last, ForwardIterator2 sub_first, ForwardIterator2 sub_last, BinaryPredicate p)
	{
		if( sub_first == sub_last ) return last;
		ForwardIterator1 search_result;
		ForwardIterator1 result = last;
		while( true )
		{
			search_result = dstd::search(first, last, sub_first, sub_last, p);
			if( search_result == last ) return result;
			result = search_result;
			++first;
		}
		return result;
	}



//
// Modifying sequence operations
//

//copy
//Copy range of elements (function template )
//copy_backward
//Copy range of elements backward (function template )
//swap
//Exchange values of two objects (function template )
//swap_ranges
//Exchange values of two ranges (function template )
	
	template <class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		dstd::swap(*a, *b);
	}
	
//transform
//Transform range (function template )
//replace
//Replace value in range (function template )
//replace_if
//Replace values in range (function template )
//replace_copy
//Copy range replacing value (function template )
//replace_copy_if
//Copy range replacing value (function template )
//fill
//Fill range with value (function template )
//fill_n
//Fill sequence with value (function template )
//generate
//Generate values for range with function (function template )
//generate_n
//Generate values for sequence with function (function template )
//remove
//Remove value from range (function template )
//remove_if
//Remove elements from range (function template )
//remove_copy
//Copy range removing value (function template )
//remove_copy_if
//Copy range removing values (function template )
//unique
//Remove consecutive duplicates in range (function template )
//unique_copy
//Copy range removing duplicates (function template )
//reverse
//Reverse range (function template )
//reverse_copy
//Copy range reversed (function template )
//rotate
//Rotate left the elements in range (function template )
//rotate_copy
//Copy range rotated left (function template )
//random_shuffle
//Randomly rearrange elements in range (function template )



//
// Partitions
//

//partition
//Partition range in two (function template )
//stable_partition
//Partition range in two - stable ordering (function template )



//
// Sorting
//

//sort
//Sort elements in range (function template )
//stable_sort
//Sort elements preserving order of equivalents (function template )
//partial_sort
//Partially sort elements in range (function template )
//partial_sort_copy
//Copy and partially sort range (function template )
//nth_element
//Sort element in range (function template )



//
// Binary search (operating on partitioned/sorted ranges)
//

//lower_bound
//Return iterator to lower bound (function template )
//upper_bound
//Return iterator to upper bound (function template )
//equal_range
//Get subrange of equal elements (function template )
//binary_search
//Test if value exists in sorted sequence (function template )



//
// Merge (operating on sorted ranges)
//

//merge
//Merge sorted ranges (function template )
//inplace_merge
//Merge consecutive sorted ranges (function template )
//includes
//Test whether sorted range includes another sorted range (function template )
//set_union
//Union of two sorted ranges (function template )
//set_intersection
//Intersection of two sorted ranges (function template )
//set_difference
//Difference of two sorted ranges (function template )
//set_symmetric_difference
//Symmetric difference of two sorted ranges (function template )



	//
	// Heap
	//
	
	
	template <class RandomIterator>
	void impl::heap_drop(RandomIterator first, RandomIterator last, RandomIterator element)
	{
		if( element == last ) return;
		
		RandomIterator child = dstd::impl::heap_max_child(first, last, element);
		
		while( child != last )
		{
			if( *element < *child )
			{
				dstd::iter_swap(element, child);
				element = child;
				child = dstd::impl::heap_max_child(first, last, element);
			}
			else
			{
				break;
			}
		}
	}
	
	
	template <class RandomIterator, class Compare>
	void impl::heap_drop(RandomIterator first, RandomIterator last, RandomIterator element, Compare comp)
	{
		if( element == last ) return;
		
		RandomIterator child = dstd::impl::heap_max_child(first, last, element, comp);
		
		while( child != last )
		{
			if( comp(*element, *child) )
			{
				dstd::iter_swap(element, child);
				element = child;
				child = dstd::impl::heap_max_child(first, last, element);
			}
			else
			{
				break;
			}
		}
	}
	
	
	template <class RandomIterator>
	RandomIterator impl::heap_child(RandomIterator first, RandomIterator last, RandomIterator parent)
	{
		RandomIterator child = first + ( 2*(parent - first) + 1 );
		return ( child < last ) ? child : last;
	}
	
	
	template <class RandomIterator>
	RandomIterator impl::heap_max_child(RandomIterator first, RandomIterator last, RandomIterator parent)
	{
		RandomIterator child1 = dstd::impl::heap_child(first, last, parent);
		
		if( child1 >= last ) return last;
		
		RandomIterator child2 = child1 + 1;
		
		if( child2 >= last ) return child1;
		
		if( *child1 < *child2 )
		{
			return child2;
		}
		else
		{
			return child1;
		}
	}
	
	
	template <class RandomIterator, class Compare>
	RandomIterator impl::heap_max_child(RandomIterator first, RandomIterator last, RandomIterator parent, Compare comp)
	{
		RandomIterator child1 = dstd::impl::heap_child(first, last, parent);
		
		if( child1 >= last ) return last;
		
		RandomIterator child2 = child1 + 1;
		
		if( child2 >= last ) return child1;
		
		if( comp(*child1, *child2) )
		{
			return child2;
		}
		else
		{
			return child1;
		}
	}
	
	
	template <class RandomIterator>
	RandomIterator impl::heap_parent(RandomIterator first, RandomIterator element)
	{
		return ( first + ( ((element - first)-1) / 2 ) );
	}
	
	
	template <class RandomIterator>
	void impl::make_heap(RandomIterator first, RandomIterator last, RandomIterator element)
	{
		if( element >= last ) return;
		RandomIterator child = dstd::impl::heap_child(first, last, element);
		dstd::impl::make_heap(first, last, child);
		dstd::impl::make_heap(first, last, child+1);
		dstd::impl::heap_drop(first, last, element);
	}
	
	
	template <class RandomIterator, class Compare>
	void impl::make_heap(RandomIterator first, RandomIterator last, RandomIterator element, Compare comp)
	{
		if( element >= last ) return;
		RandomIterator child = dstd::impl::heap_child(first, last, element);
		dstd::impl::make_heap(first, last, child);
		dstd::impl::make_heap(first, last, child+1);
		dstd::impl::heap_drop(first, last, element);
	}
	
	
	template <class RandomIterator>
	void push_heap(RandomIterator first, RandomIterator last)
	{
		RandomIterator new_element = last - 1;
		RandomIterator parent;
		
		while( new_element != first )
		{
			parent = dstd::impl::heap_parent(first, new_element);
			
			if( *parent < *new_element )
			{
				dstd::iter_swap(parent, new_element);
				new_element = parent;
			}
			else
			{
				break;
			}
		}
	}
	
	
	template <class RandomIterator, class Compare>
	void push_heap(RandomIterator first, RandomIterator last, Compare comp)
	{
		RandomIterator new_element = last - 1;
		RandomIterator parent;
		
		while( new_element != first )
		{
			parent = dstd::impl::heap_parent(first, new_element);
			
			if( comp(*parent, *new_element) )
			{
				dstd::iter_swap(parent, new_element);
				new_element = parent;
			}
			else
			{
				break;
			}
		}
	}
	
	
	template <class RandomIterator>
	void pop_heap(RandomIterator first, RandomIterator last)
	{
		// Put the element from the top of the heap at the end, and decrement 'last'
		// so that [first,last) contains the remaining elements
		--last;
		dstd::iter_swap(first, last);
		dstd::impl::heap_drop(first, last, first);
	}
	
	
	template <class RandomIterator, class Compare>
	void pop_heap(RandomIterator first, RandomIterator last, Compare comp)
	{
		// Put the element from the top of the heap at the end, and decrement 'last'
		// so that [first,last) contains the remaining elements
		--last;
		dstd::iter_swap(first, last);
		dstd::impl::heap_drop(first, last, first, comp);
	}
	
	
	template <class RandomIterator>
	void make_heap(RandomIterator first, RandomIterator last)
	{
		dstd::impl::make_heap(first, last, first);
	}
	
	
	template <class RandomIterator, class Compare>
	void make_heap(RandomIterator first, RandomIterator last, Compare comp)
	{
		dstd::impl::make_heap(first, last, first, comp);
	}
	
	
	template <class RandomIterator>
	void sort_heap(RandomIterator first, RandomIterator last)
	{
		for( ; last != first; --last)
		{
			pop_heap(first, last);
		}
	}
	
	
	template <class RandomIterator, class Compare>
	void sort_heap(RandomIterator first, RandomIterator last, Compare comp)
	{
		for( ; last != first; --last)
		{
			pop_heap(first, last, comp);
		}
	}
	
	
	//
	// Min/max
	//
	
	
	template <class T>
	const T& max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}
	
	
	template <class T, class Compare>
	const T& max(const T& a, const T& b, Compare comp)
	{
		return comp(a, b) ? b : a;
	}
	
	
	///Return largest element in range
	template <class ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	{
		ForwardIterator max = first;
		while(first != last)
		{
			if( *max < *first ) max = first;
			++first;
		}
		return max;
	}
	
	
	/// Return largest element in range
	template <class ForwardIterator, class Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		ForwardIterator max = first;
		while(first != last)
		{
			if( comp(*max, *first) ) max = first;
			++first;
		}
		return max;
	}
	
	
	template <class T>
	const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
	
	
	template <class T, class Compare>
	const T& min(const T& a, const T& b, Compare comp)
	{
		return comp(b, a) ? b : a;
	}
	
	
	/// Return smallest element in range
	template <class ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	{
		ForwardIterator min = first;
		while(first != last)
		{
			if( *first < *min ) min = first;
			++first;
		}
		return min;
	}
	
	
	/// Return smallest element in range
	template <class ForwardIterator, class Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		ForwardIterator min = first;
		while(first != last)
		{
			if( comp(*first, *min) ) min = first;
			++first;
		}
		return min;
	}



//
// Other
//

//lexicographical_compare
//Lexicographical less-than comparison (function template )
//next_permutation
//Transform range to next permutation (function template )
//prev_permutation
//Transform range to previous permutation (function template )


}



#endif
