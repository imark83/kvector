#ifndef __MERGESORT_HPP
#define __MERGESORT_HPP
// #include<cstdlib>
// #include<cstdio>

#include <vector>
#include <stdint.h>


template<class T>
void merge(std::vector<T> &vec, int64_t left, int64_t mid, int64_t right) {

  // split vec[left:right] into two arrays
  std::vector<T> leftArr(vec.begin()+left, vec.begin()+mid);
  std::vector<T> rightArr(vec.begin()+mid, vec.begin()+right);


  /* Merge the temp arrs back into vec[left:right]*/
  int64_t i = 0; // Initial index of leftArr
  int64_t j = 0; // Initial index of rightArr
  int64_t k = left; // Initial index of vec[left:right]
  while (i < (int64_t) leftArr.size() && j < (int64_t) rightArr.size()) {
    if (leftArr[i] < rightArr[j]) {
      vec[k] = leftArr[i];
      ++i;
    } else {
      vec[k] = rightArr[j];
      ++j;
    }
    ++k;
  }
  // Copy the remaining  if there
  while (i < (int64_t) leftArr.size()) {
    vec[k] = leftArr[i];
    ++i;
    ++k;
  }
  while (j < (int64_t) rightArr.size()) {
    vec[k] = rightArr[j];
    ++j;
    ++k;
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
