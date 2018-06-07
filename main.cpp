#include <iostream>
#include <cstdlib>
#include <cmath>
#include "data.hpp"
#include "quicksort.hpp"
#include "mergesort.hpp"
#include "misc.hpp"

#include <gmpxx.h>




// void printRange(std::vector<Data> &vec, size_t N,
//           size_t first_score, size_t last_score) {
//   size_t position = getPositionOfTag(vector, N, first_score);
//   if(position == N) return;
//   if(vector[position].score > last_score) return;
//   do {
//     printf("(%f,%f)\n", vector[position].x[0], vector[position].x[1]);
//     ++position;
//   } while(vector[position].score <= last_score);
//
//   return;
// }


template<int N, class T, class S>
std::ostream & operator<<(std::ostream &output, const Data_t<N,T,S> &op) {
  output << op.score << " (" << op.prev_boss << ","
        << op.this_boss << "," << op.next_boss << ") elements = (" << op.elements_per_dim[0] << "," << op.elements_per_dim[1] << ")";
  return output;
}


template<int N, class T, class S>
class Database {
public:
  std::vector<Data_t<N,T,S> > vec;
  int64_t nBox;

  Database(int dataBaseSize) : vec(dataBaseSize) {
    nBox = -1;
  }


  Data_t<N,T,S> & operator[](int64_t i) {
    return vec[i];
  }
  Data_t<N,T,S> & operator[](int64_t i) const {
    return vec[i];
  }

  int64_t size() const {
    return vec.size();
  }

  void sort() {
    mergeSort(vec);
  }

  void setScores(int64_t nBox) {
    assignNavScores(vec, nBox);
    this->nBox = nBox;
  }

};

template<class S>
S myPow(S base, int64_t exponent) {
  if(exponent == 0) return 1;
  if(exponent == 1) return base;
  S temp = myPow(base,exponent/2);
  if(exponent%2 == 0) {
    return temp * temp;
  }
  return temp * temp * base;
}

template<int N, class T, class S>
void computeElementsPerDimension(Database<N,T,S> &database) {
  S boxStep = database.nBox;
  S endBox = database.nBox;
  endBox = myPow(endBox, N);
  S box0;
  S box1;
  int64_t pos0, pos1;
  for(int64_t dimension=0; dimension<N-1; ++dimension) {
    pos0 = 0;
    box0 = database[0].score - (database[0].score%boxStep);
    box1 = box0 + boxStep;
    do {
      pos1 = getPositionOfScore(database.vec, box1);
      for(int64_t j=pos0; j<pos1; ++j)
        database[j].elements_per_dim[dimension] = pos1 - pos0;
        if(pos1 == database.size()) break;

      box0 = box0 + boxStep*((database[pos1].score - box0)/boxStep);
      pos0 = pos1;
      box1 = box0 + boxStep;
    } while (box1 < endBox);
      pos1 = getPositionOfScore(database.vec, box1);
      for(int64_t j=pos0; j<pos1; ++j)
        database[j].elements_per_dim[dimension] = pos1 - pos0;

    boxStep *= database.nBox;
  }
}

int main(int argc, char const *argv[]) {
  int64_t nBox = 3;
  int64_t dataBaseSize = 20;

  Database<3,double,int64_t> database(dataBaseSize);

  for(int64_t i=0; i<database.size(); ++i) {
    for(int64_t j=0; j<3; ++j)
      database[i].x[j] = ((double) rand()) / RAND_MAX;
    database[i].score = getScore(database[i], nBox);
  }

  database.sort();
  database.setScores(nBox);
  computeElementsPerDimension(database);

  for(int64_t i=0; i<database.size(); ++i) {
    std::cout << i << " -> " << database[i] << std::endl;
  }




  return 0;
}
