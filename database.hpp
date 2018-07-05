#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__


#include "quicksort.hpp"
#include "mergesort.hpp"




// RETURNS WHETHER THERE IS (NEXT BOX DIMENSION) NEXT BOX OR NOT (-1)
int next(Score_t &box, const Score_t &box0,
        const Score_t &box1, int i) {
  box[i] += 1;
  if(box[i] == box1[i]) {
    if(i==(int64_t) box.size()-1) return -1;
    box[i] = box0[i];
    return next(box, box0, box1, i+1);
  }
  return i;
}

// RETURNS WHETHER THERE IS NEXT BOX
int next(Score_t &box, const Score_t &box0,
  const Score_t &box1) {
    return next(box, box0, box1, 0);
}

template<int N, class T>
class Database {
public:
  std::vector<Data_t<N,T> > vec;
  int64_t nBox;

  Database(int dataBaseSize) : vec(dataBaseSize) {
    nBox = -1;
  }


  Data_t<N,T> & operator[](int64_t i) {
    return vec[i];
  }
  Data_t<N,T> & operator[](int64_t i) const {
    return vec[i];
  }

  int64_t size() const {
    return vec.size();
  }

  void setScores(int64_t nBox) {
    assignNavScores(vec, nBox);
    this->nBox = nBox;
  }

  void print() {
    std::vector<int64_t> box0(N, 0);
    std::vector<int64_t> box1(N, nBox-1);
    std::vector<int64_t> boxOut(N, nBox);
    int leap = 0;
    int64_t prevN = 0;
    for(std::vector<int64_t> box = box0; box<=box1; leap=next(box, box0, boxOut)) {
      for(int i=0; i<leap; ++i) std::cout << std::endl;
      int64_t n = getPositionOfScore(*this, box);
      std::cout.width(5);
      std::cout.fill(' ');
      if(n < size()) {
        if(vec[n].score == box)
          std::cout << vec[n].next_boss - n;
        else
          std::cout << 0;
      } else
        std::cout << 0;
    }
    std::cout << std::endl;
  }

};



template<int N, class T>
void mergesort(Database<N,T> &database) {
  mergeSort(database.vec);
}

template<int N, class T>
void quicksort(Database<N,T> &database) {
  quickSort(database.vec);
}

#endif
