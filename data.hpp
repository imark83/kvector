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
  int64_t next_boss[N];

  Data_t() : x(N, T(0)), score(0) {
    for(int64_t i=0; i<N; ++i) {
      next_boss[i] = -1;
    }
    this_boss = -1;
  }



  int nDim() const {
    return N;
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
