#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <stdint.h>
#include <vector>



typedef std::vector<int64_t> Score_t;

template<class T = double>
class Data_t {
public:
  std::vector<T> x;
  Score_t score;
  int64_t this_boss;
  int64_t prev_boss;
  int64_t next_boss;
  int64_t ndim;

  std::vector<int64_t> elements_per_dim;



  Data_t(int64_t N) : x(N, T(0)), score(N,0), ndim(N), elements_per_dim(N-1,-1) {
    next_boss = this_boss = prev_boss = -1;
    this_boss = -1;
  }
  int nDim() const {
    return ndim;
  }


  T & operator[](int64_t i) {
    return x[i];
  }
  T & operator[](int64_t i) const {
    return x[i];
  }

};


template<class T>
inline bool operator<(const Data_t<T> &op1, const Data_t<T> &op2) {
  for(int i=op1.ndim-1; i>=0; --i) {
    if(op1.score[i] < op2.score[i])
      return 1;
    if(op1.score[i] > op2.score[i])
      return 0;
  }
  return 0;
}


template<class T>
inline bool operator>(const Data_t<T> &op1, const Data_t<T> &op2) {
  for(int i=op1.ndim-1; i>=0; --i) {
    if(op1.score[i] > op2.score[i])
      return 1;
    if(op1.score[i] < op2.score[i])
      return 0;
  }
  return 0;
}


template<class T>
inline void swap(Data_t<T> &op1, Data_t<T> &op2) {
  Data_t<T> aux;
  aux = op1; op1 = op2; op2 = aux;
  return;
}



template<class T>
std::ostream & operator<<(std::ostream &output, const Data_t<T> &op) {
  output << "(" << op.x[0] << " " << op.x[1] << " " << op.x[2] << ") " << op.score << " (" << op.prev_boss << ","
        << op.this_boss << "," << op.next_boss << ") elements = (" << op.elements_per_dim[0] << "," << op.elements_per_dim[1] << ")";
  return output;
}


bool operator==(const Score_t &op1, const Score_t &op2);
bool operator>(const Score_t &op1, const Score_t &op2);
bool operator<(const Score_t &op1, const Score_t &op2);
bool operator<=(const Score_t &op1, const Score_t &op2);
bool operator>=(const Score_t &op1, const Score_t &op2);
Score_t operator+(const Score_t &op1, const Score_t &op2);
Score_t operator-(const Score_t &op1, const Score_t &op2);

// TIMER
// RETURNS WHETHER THERE IS (NEXT BOX DIMENSION) NEXT BOX OR NOT (-1)
int next(Score_t &box, const Score_t &box0,
        const Score_t &box1, const int i);
// RETURNS WHETHER THERE IS NEXT BOX
int next(Score_t &box, const Score_t &box0,
  const Score_t &box1);

int64_t maxLeap(const Score_t &score0, const Score_t &score1);

#endif
