#ifndef __DATA_HPP
#define __DATA_HPP


#include <stdint.h>
#include <vector>

template<int N, class T = double, class S = int64_t>
class Data_t {
public:
  std::vector<T> x;
  S score; // TO CONVERT TO LIBGMP BIG INT
  int64_t this_boss;
  int64_t prev_boss;
  int64_t next_boss;


  std::vector<int64_t> elements_per_dim;


  Data_t() : x(N, T(0)), score(0), elements_per_dim(N-1) {
    next_boss = this_boss = prev_boss = -1;
    this_boss = -1;
  }
  int nDim() const {
    return N;
  }


  T & operator[](int64_t i) {
    return x[i];
  }
  T & operator[](int64_t i) const {
    return x[i];
  }


};

template<int N, class T, class S>
inline void swap(Data_t<N,T,S> &op1, Data_t<N,T,S> &op2) {
  Data_t<N,T,S> aux;
  aux = op1; op1 = op2; op2 = aux;
  return;
}

template<int N, class T, class S>
inline bool operator<(Data_t<N,T,S> &op1, Data_t<N,T,S> &op2) {
  return op1.score < op2.score;
}

template<int N, class T, class S>
inline bool operator>(Data_t<N,T,S> &op1, Data_t<N,T,S> &op2) {
  return op1.score > op2.score;
}

#endif
