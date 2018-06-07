#ifndef __MERGESORT_HPP
#define __MERGESORT_HPP
// #include<cstdlib>
// #include<cstdio>

#include <vector>
#include <stdint.h>


template<class T>
void merge(std::vector<T> &vec, int64_t left, int64_t mid, int64_t right) {
    // int64_t i, j, k;
  int64_t n1 = mid - left;
  int64_t n2 =  right - mid;

  /* create temp arrs */
  std::vector<T> leftArr(vec.begin()+left, vec.begin()+mid);
  std::vector<T> rightArr(vec.begin()+mid, vec.begin()+right);


  /* Merge the temp arrs back int64_to vec[l..r]*/
  int64_t i = 0; // Initial index of first subarr
  int64_t j = 0; // Initial index of second subarr
  int64_t k = left; // Initial index of merged subarr
  while (i < leftArr.size() && j < rightArr.size()) {
    if (leftArr[i] < rightArr[j]) {
      vec[k] = leftArr[i];
      ++i;
    } else {
      vec[k] = rightArr[j];
      ++j;
    }
    ++k;
  }
  while (i < n1) {
        vec[k] = leftArr[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        vec[k] = rightArr[j];
        j++;
        k++;
    }
}

template<class T>
void mergeSort(std::vector<T> &vec, int64_t left, int64_t right) {
  if (left < right-1) {

    int64_t mid = left+(right-left)/2;

    // Sort first and second halves
    mergeSort(vec, left, mid);
    mergeSort(vec, mid, right);
    merge(vec, left, mid, right);
  }
}


template<class T>
void mergeSort(std::vector<T> &vec) {
  mergeSort(vec, 0, vec.size());
  return;
}

#endif
