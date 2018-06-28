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
  output << "(" << op.x[0] << " " << op.x[1] << " " << op.x[2] << ") " << op.score << " (" << op.prev_boss << ","
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

  void setScores(int64_t nBox) {
    assignNavScores(vec, nBox);
    this->nBox = nBox;
  }

};

template<int N, class T, class S>
void mergesort(Database<N,T,S> &database) {
  mergeSort(database.vec);
}


template<int N, class T, class S>
int64_t getPositionOfScore(Database<N,T,S> &database, S &score) {
  return getPositionOfScore(database.vec, score);
}



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
  int64_t nBox = 4;
  int64_t dataBaseSize = 24;

  Database<3,double,int64_t> database(dataBaseSize);

  for(int64_t i=0; i<database.size(); ++i) {
    for(int64_t j=0; j<3; ++j)
      database[i].x[j] = ((double) rand()) / RAND_MAX;
    database[i].score = getScore(database[i], nBox);
  }

  mergesort(database);
  database.setScores(nBox);
  computeElementsPerDimension(database);

  for(int64_t i=0; i<database.size(); ++i) {
    std::cout << i << " -> " << database[i] << std::endl;
  }


  // SEARCH RANGE [0,0.7]x[0,0.7]x[0,0.7]
  // brute force
  std::vector<int> v(0);
  for(int64_t i=0; i<database.size(); ++i) {
    if(database[i][2] > 0.5) continue;
    v.push_back(i);
  }


  std::cout << "rop = ";
  for(int64_t i=0; i<(int64_t) v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;

  v.clear();


  int64_t line0 = 0;
  int64_t line1 = 4;
  int64_t row0 = 0;
  int64_t row1 = 4;
  int64_t floor0 = 0;
  int64_t floor1 = 2;

  int64_t score = line0*nBox + row0 + floor0*nBox*nBox;
  int64_t endScore = line0*nBox + row1 + floor0*nBox*nBox;
  int64_t pos0 = getPositionOfScore(database, score);
  int64_t pos1 = getPositionOfScore(database, endScore);
  int64_t zeroPos0 = pos0;
  int64_t zeroPos1 = pos1;

  for(int level = 0; level < floor1; ++level) {
    pos0 = zeroPos0;
    pos1 = zeroPos1;
    for(score = line0*nBox + row0 + (nBox*nBox)*level,
        endScore = line0*nBox + row1 + level*nBox*nBox;
        score < row1; score +=nBox, endScore +=nBox) {

      // corregimos la posicion
      while(database[database[pos0].prev_boss].score > score
                  && database[pos0].this_boss != 0)
          pos0 = database[pos0].prev_boss;

      while(database[database[pos0].this_boss].score < score)
        pos0 = database[pos0].next_boss;

      while(database[database[pos1].prev_boss].score > endScore
            && database[pos1].this_boss != 0)
        pos1 = database[pos1].prev_boss;

      while(database[database[pos1].this_boss].score < endScore)
        pos1 = database[pos1].next_boss;


      if(database[pos0].score >= endScore) continue;

      for(int64_t i=pos0; i<pos1; ++i)
        v.push_back(i);

      pos0 += database[pos0].elements_per_dim[0];
      pos1 += database[pos1].elements_per_dim[0];
    }
    if(level < 2) {
      zeroPos0 += database[zeroPos0].elements_per_dim[level+1];
      zeroPos1 += database[zeroPos1].elements_per_dim[level+1];
    }
  }

  std::cout << "rop = ";
  for(int64_t i=0; i<(int64_t) v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;

  return 0;
}
