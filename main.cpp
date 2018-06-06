#include <iostream>
#include <cstdlib>
#include <cmath>
#include "data.hpp"
#include "quicksort.hpp"
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




int main(int argc, char const *argv[]) {
  int64_t nBox = 10;
  int64_t dataBaseSize = (1<<26);

  std::vector<Data_t<2,double,int64_t> > vec(dataBaseSize);

  for(int i=0; i<vec.size(); ++i) {
    double x = ((double) rand()) / RAND_MAX;
    double y = ((double) rand()) / RAND_MAX;
    vec[i].x[0] = x;
    vec[i].x[1] = y;
    vec[i].score = (int64_t) floor(x*nBox) + nBox*floor(y*nBox);
  }

  quickSort(vec);
  // assignNavScores(vec, nBox);



  return 0;
}
