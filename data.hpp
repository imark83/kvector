#ifndef __DATA_HPP
#define __DATA_HPP


#include <stdint.h>
#include <vector>

template<int N, class T = double>
class Data_t {
public:
  std::vector<T> x;
  std::vector<int64_t> score;
  int64_t this_boss;
  int64_t prev_boss;
  int64_t next_boss;


  std::vector<int64_t> elements_per_dim;


  Data_t() : x(N, T(0)), score(N,0), elements_per_dim(N-1,-1) {
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


template<int N, class T>
inline bool operator<(const Data_t<N,T> &op1, const Data_t<N,T> &op2) {
  for(int i=N-1; i>=0; --i) {
    if(op1.score[i] < op2.score[i])
      return 1;
    if(op1.score[i] > op2.score[i])
      return 0;
  }
  return 0;
}


template<int N, class T>
inline bool operator>(const Data_t<N,T> &op1, const Data_t<N,T> &op2) {
  for(int i=N-1; i>=0; --i) {
    if(op1.score[i] > op2.score[i])
      return 1;
    if(op1.score[i] < op2.score[i])
      return 0;
  }
  return 0;
}


template<int N, class T>
inline void swap(Data_t<N,T> &op1, Data_t<N,T> &op2) {
  Data_t<N,T> aux;
  aux = op1; op1 = op2; op2 = aux;
  return;
}



inline bool operator==(const std::vector<int64_t> &op1,
          const std::vector<int64_t> &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] != op2[i])
      return 0;
  }
  return 1;
}


inline bool operator>(const std::vector<int64_t> &op1,
          const std::vector<int64_t> &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] > op2[i])
      return 1;
    if(op1[i] < op2[i])
      return 0;
  }
  return 0;
}


inline bool operator<(const std::vector<int64_t> &op1,
          const std::vector<int64_t> &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] < op2[i])
      return 1;
    if(op1[i] > op2[i])
      return 0;
  }
  return 0;
}


#endif
