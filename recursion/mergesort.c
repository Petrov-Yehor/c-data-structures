#include "mergesort.h"
#include <stdio.h>

void mergeSort(int* arr, size_t len)
{
    if (!arr || len < 2)
        return;

    mergeSortRecursive(arr, 0, len - 1);
}

void mergeSortRecursive(int* arr, size_t left, size_t right)
{
    if (left >= right)
        return;

    size_t mid = left + (right - left) / 2;
    mergeSortRecursive(arr, left, mid);
    mergeSortRecursive(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void merge(int* arr, size_t l, size_t m, size_t r)
{
    if (!arr)
        return;

    size_t i = l;
    size_t j = m+1;
    size_t k = 0;
    size_t nrOfElements = r - l + 1;

    int *result = (int*)malloc(nrOfElements * sizeof(int));

    while (i <= m && j <= r)
    {
        if (arr[i] < arr[j])
            result[k++] = arr[i++];
        else
            result[k++] = arr[j++];
    }
    while (i <= m)
        result[k++] = arr[i++];
    while (j <= r)
        result[k++] = arr[j++];
    
    for (size_t x = 0; x < nrOfElements; x++)
        arr[x + l] = result[x];
}