# ifndef __MERGESORT_H_
# define __MERGESORT_H_

#include <stdio.h>

void mergeSort(int* arr, size_t len);
void mergeSortRecursive(int* arr, size_t left, size_t right);

// Merge two segments of an array defined by left(l)-start of left segment, 
//                                           middle(m)-end of left segment (included),
//                                           right(r)-end of right segment (included)
void merge(int* arr, size_t l, size_t m, size_t r);

#endif