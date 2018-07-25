#include <iostream>
#include <cstdlib>
#include <cmath>
#include "data.hpp"
#include "database.hpp"
#include "misc.hpp"

// #include <gmpxx.h>
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


int main(int argc, char const *argv[]) {

  // MEASURE TIME
  clock_t c0, c1;


  int64_t ndim = 10;
  int64_t nBox = 5;
  int64_t dataBaseSize = 10000000;

  std::cout.precision(2);

  Database<double> database(dataBaseSize, ndim);

  for(int64_t i=0; i<database.size(); ++i) {
    for(int64_t j=0; j<ndim; ++j)
      database[i].x[j] = ((double) rand()) / RAND_MAX;
    database[i].score = getScore(database[i], nBox);
  }

  std::cout << "start preprocessing..." << std::endl;
  mergesort(database);
  database.setScores(nBox);
  computeElementsPerDimension(database);



  // for(int64_t i=0; i<database.size(); ++i) {
  //   std::cout << i << " -> " << database[i] << std::endl;
  // }

  std::cout << "done. Start brute force..." << std::endl;

  // SEARCH RANGE [0.25,0.75)x[0.25,0.75)x[0.25,0.75)
  // brute force
  std::vector<int64_t> v(0);

  c0 = clock();
  for(int64_t i=0; i<database.size(); ++i) {
    bool valid = true;
    for(int64_t j=0; j<ndim; ++j) {
      if(database[i][j] >= 0.8) {
        valid = false;
        break;
      }
      if(database[i][j] < 0.6) {
        valid = false;
        break;
      }
    }
    if(valid) v.push_back(i);
  }
  c1 = clock();

  std::cout << "rop = " << v.size() << std::endl;
  std::cout << std::scientific;
  std::cout << "\t" << (((float) (c1-c0))/CLOCKS_PER_SEC) << std::endl;
  std::cout << "start k-pollas" << std::endl;



  // std::cout << v << std::endl;
  v.clear();

  // database.print();



  Score_t box0(ndim);
  Score_t box1(ndim);
  // INITIALIZE BOX SEARCH box0 <= val < box1
  for(int i=0; i<ndim; ++i) {
    box0[i] = 3;
    box1[i] = 4;
  }

  c0 = clock();
  rangeSearch(v, database, box0, box1);
  c1 = clock();

  // std::cout << v << std::endl;


  std::cout << "rop = " << v.size() << std::endl;
  std::cout << std::scientific;
  std::cout << "\t" << (((float) (c1-c0))/CLOCKS_PER_SEC) << std::endl;




  return 0;
}
