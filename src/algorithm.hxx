#ifndef DSTD_ALGORITHM_HXX
#define DSTD_ALGORITHM_HXX



namespace dstd
{


//
//Non-modifying sequence operations
//

//for_each
//Apply function to range (function template )
//find
//Find value in range (function template )
//find_if
//Find element in range (function template )
//find_end
//Find last subsequence in range (function template )
//find_first_of
//Find element from set in range (function template )
//adjacent_find
//Find equal adjacent elements in range (function template )
//count
//Count appearances of value in range (function template )
//count_if
//Return number of elements in range satisfying condition (function template )
//mismatch
//Return first position where two ranges differ (function template )
//equal
//Test whether the elements in two ranges are equal (function template )
//search
//Search range for subsequence (function template )
//search_n
//Search range for elements (function template )



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
//iter_swap
//Exchange values of objects pointed by two iterators (function template )
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

// push_heap
// Push element into heap range (function template )
// pop_heap
// Pop element from heap range (function template )
// make_heap
// Make heap from range (function template )
// sort_heap
// Sort elements of heap (function template )



	//
	// Min/max
	//
	
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
