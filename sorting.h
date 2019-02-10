/*
 * sorting.h
 *
 *  Created on: 3 Dec 2013
 *      Author: andy
 */

#ifndef SORTING_H_
#define SORTING_H_

#include <vector>



template <typename T>
void quick_sort_internal(typename std::vector<T>::iterator start,
						 typename std::vector<T>::iterator end)
{
	if (end-start > 1)
	{
		typename std::vector<T>::iterator start_it = start;
		typename std::vector<T>::iterator end_it = end;
		typename std::vector<T>::size_type middle = (end-start)/2;
		T pivot = *(start_it + middle);
		while (start_it < end_it)
		{
			while ((*start_it < pivot) && (start_it < end_it))
			{
				start_it++;
			}
			while ((*end_it > pivot) && (end_it > start))
			{
				end_it--;
			}
			std::swap<T>(*start_it,*end_it);
		}
		quick_sort_internal<T>(start_it,start_it+middle);
		quick_sort_internal<T>(start_it+middle+1,end);
	}
}


template <class T>
void quick_sort(std::vector<T>& input)
{
	if (input.size() > 1)
	{
//		typename std::vector<T>::iterator start = input.begin();
//		typename std::vector<T>::iterator endit = input.end();

//		typename std::vector<T>::iterator start2 = input.begin();
//		typename std::vector<T>::iterator end_it = input.end();
//		--endit;
		quick_sort_internal<T>(std::begin(input),std::end(input));
	}
}



template <class T>
void insert_sort(std::vector<T>& input)
{
//	typename std::vector<T>::iterator start = input.begin();
//	typename std::vector<T>::iterator it = input.begin();
//	it++;
//	typename std::vector<T>::iterator end = input.end();
//	typename std::vector<T>::iterator place;


	int insertingIndex = 0;
	for (typename std::vector<T>::size_type sortedStartIndex =1 ;sortedStartIndex<input.size();++sortedStartIndex)
	{
		T key = input[sortedStartIndex];
		insertingIndex  = sortedStartIndex-1;

		while((insertingIndex >= 0) && input[insertingIndex] > key)
		{
			input[insertingIndex + 1] = input[insertingIndex ];
			insertingIndex -= 1;
		}
		input[insertingIndex+1] = key;
	}

}

// forward dec....

template<class T>
std::vector<T> merge(std::vector<T>& left, std::vector<T>& right)
{
	typename std::vector<T> result;
	typename std::vector<T>::iterator right_it = right.begin();
	typename std::vector<T>::iterator left_it = left.begin();

	while ((right_it != right.end()) && (left_it != left.end()))
	{
		if (*left_it < *right_it)
		{
			result.push_back(*left_it);
			++left_it;
		}
		else
		{
			result.push_back(*right_it);
			++right_it;
		}
	}
	for (;left_it!=left.end();++left_it)
	{
		result.push_back(*left_it);
	}
	for (;right_it != right.end(); ++right_it)
	{
		result.push_back(*right_it);
	}

	return result;
}

template<class T>
std::vector<T> merge_sort(std::vector<T>& input)
{
	typename std::vector<T>::size_type arrSize = input.size();
	if (arrSize < 2)
	{
		return input;
	}
	else
	{
		typename std::vector<T>::iterator middle = input.begin() + (arrSize / 2);
		typename std::vector<T> left(input.begin(), middle);
		typename std::vector<T> right(middle, input.end());

		left = merge_sort<T>(left);
		right = merge_sort<T>(right);
		return merge<T>(left,right);
	}
}


#endif /* SORTING_H_ */
