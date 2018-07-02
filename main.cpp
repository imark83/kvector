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


std::ostream & operator<<(std::ostream &output,
        const std::vector<int64_t> &op) {
  if(op.size() == 0) return output;
  output << "(" << op[0];
  for(int64_t i=1; i<(int64_t) op.size(); ++i)
    output << ", " << op[i];
  output << ")";
  return output;
}

template<int N, class T>
std::ostream & operator<<(std::ostream &output, const Data_t<N,T> &op) {
  output << "(" << op.x[0] << " " << op.x[1] << " " << op.x[2] << ") " << op.score << " (" << op.prev_boss << ","
        << op.this_boss << "," << op.next_boss << ") elements = (" << op.elements_per_dim[0] << "," << op.elements_per_dim[1] << ")";
  return output;
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

};

template<int N, class T>
void mergesort(Database<N,T> &database) {
  mergeSort(database.vec);
}


template<int N, class T>
int64_t getPositionOfScore(Database<N,T> &database,
          std::vector<int64_t> &score) {
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

int64_t maxLeap(const std::vector<int64_t> &score0, const std::vector<int64_t> &score1) {
  for(int64_t i=score0.size()-1; i>=0; --i) {
    if (score0[i] != score1[i])
      return i;
  }
  return 0;
}

template<int N, class T>
void computeElementsPerDimension(Database<N,T> &database) {
  int ml;
  int p;
  std::vector<int64_t> elements_per_dim(N-1, 0);
  for(int64_t pos=database[0].next_boss;
              pos<(int64_t) database.size(); pos = database[pos].next_boss) {

    for(int k=0; k<N-1; ++k)
      elements_per_dim[k] += pos - database[pos].prev_boss;

    ml = maxLeap(database[pos].score, database[pos-1].score);
    if(ml>0) {
      p = pos-1;
      while(database[p].elements_per_dim[ml-1] == -1) {
        for(int k=0; k<ml; ++k) {
          if(database[p].elements_per_dim[k] == -1)
            database[p].elements_per_dim[k] = elements_per_dim[k];
        }
        --p;
        if(p<0) break;
      }
      for(int k=0; k<ml; ++k)
        elements_per_dim[k] = 0;
    }

  }

  for(int k=0; k<N-1; ++k)
    elements_per_dim[k] +=
          database.size() - database[database.size()-1].this_boss;

  p = database.size() - 1;
  while(database[p].elements_per_dim[N-2] == -1) {
    for(int k=0; k<N-1; ++k) {
      if(database[p].elements_per_dim[k] == -1)
        database[p].elements_per_dim[k] = elements_per_dim[k];
    }
    --p;
    if(p<0) break;
  }
}

int main(int argc, char const *argv[]) {
  int64_t nBox = 3;
  int64_t dataBaseSize = 20;

  std::cout.precision(2);

  Database<3,double> database(dataBaseSize);

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
  std::vector<int64_t> v(0);
  for(int64_t i=0; i<database.size(); ++i) {
    if(database[i][2] > 0.5) continue;
    v.push_back(i);
  }


  std::cout << "rop = ";
  for(int64_t i=0; i<(int64_t) v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;

  v.clear();





  // // KVECTOR
  // int64_t score0[3];
  // int64_t score1[3];
  // int64_t pos0[3];
  // int64_t pos1[3];






  return 0;
}
