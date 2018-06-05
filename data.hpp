#ifndef __DATA_HPP
#define __DATA_HPP


#include <cstdint>
#include <vector>

template<class T, int64_t N>
class Data_t {
public:
  std::vector<T> x;
  int64_t score; // TO CONVERT TO LIBGMP BIG INT
  int64_t this_boss;
  int64_t next_boss[N];
  // int64_t prev_boss[N];

  Data_t() : x(N, T(0)) {
    for(int64_t i=0; i<N; ++i) {
      next_boss[i] = -1;
      // next_boss[i] = prev_boss[i] = -1;
    }
    this_boss = -1;
  }

  void swap(Data_t<T,N> &op) {
    T aux;
    int64_t aux2;
    for(int64_t i=0; i<N; ++i) {
      aux = x[i]; x[i] = op.x[i]; op.x[i] = aux;
      // aux2 = prev_boss[i];
      //     prev_boss[i] = op.prev_boss[i]; op.prev_boss[i] = aux2;
      aux2 = next_boss[i];
          next_boss[i] = op.next_boss[i]; op.next_boss[i] = aux2;
    }
    aux2 = this_boss; this_boss = op.this_boss; op.this_boss = aux2;
  }

  int nDim() const {
    return N;
  }
};


#endif
