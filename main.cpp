#include <iostream>
#include <cstdlib>
#include <cmath>
#include "data.hpp"
#include "database.hpp"
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


Score_t operator+(const Score_t &op1, const Score_t &op2) {
  Score_t rop(op1);
  for(size_t i=0; i<op1.size(); ++i)
    rop[i] += op2[i];
  return rop;
}
Score_t operator-(const Score_t &op1, const Score_t &op2) {
  Score_t rop(op1);
  for(size_t i=0; i<op1.size(); ++i)
    rop[i] -= op2[i];
  return rop;
}


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
int64_t getPositionOfScore(Database<N,T> &database,
          std::vector<int64_t> &score) {
  return getPositionOfScore(database.vec, score);
}




int64_t maxLeap(const Score_t &score0, const Score_t &score1) {
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
  int64_t nBox = 4;
  int64_t dataBaseSize = 250;

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


  // SEARCH RANGE [0.25,0.75)x[0.25,0.75)x[0.25,0.75)
  // brute force
  std::vector<int64_t> v(0);

  for(int64_t i=0; i<database.size(); ++i) {
    bool valid = true;
    for(int64_t j=0; j<3; ++j) {
      if(database[i][j] >= 0.75) {
        valid = false;
        break;
      }
      if(database[i][j] < 0.25) {
        valid = false;
        break;
      }
    }
    if(valid) v.push_back(i);
  }


  std::cout << "rop = ";
  for(int64_t i=0; i<(int64_t) v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;

  v.clear();

  // database.print();


  Score_t box0(3);
  Score_t box1(3);
  // INITIALIZE BOX SEARCH box0 <= val < box1
  for(int i=0; i<3; ++i) {
    box0[i] = 1;
    box1[i] = 3;
  }


  Score_t box = box0;
  Score_t box_left(box0);
  Score_t box_right(box0);
    box_right[0] = box1[0];

  // std::cout << "box0 = " << box0 << std::endl;
  // std::cout << "box1 = " << box1 << std::endl;

  int64_t pos_left = getPositionOfScore(database, box_left);
  // int64_t pos_right = getPositionOfScore(database, box_right);

  int leap = -1;
  do {
    // std::cout << leap << " -> " << box_left << ", " << box_right << std::endl;
    // std::cout << leap << " -> " << database[pos_left].score << std::endl;

    // CAJA CON PUNTOS
    if(database[pos_left].score < box_right) {
      int64_t pos = pos_left;
      while (database[pos].score < box_right)
      v.push_back(pos++);
    }



    leap = next(box_left, box0, box1, 1);
    if(leap != -1) {
      box_right = box_left; box_right[0] = box1[0];

      Score_t diff = box_left - database[pos_left].score;
      // std::cout << "\tnext left = " << box_left << std::endl;
      // std::cout << "\tdiff = " << diff << std::endl;
      for(int64_t dim = 1; dim<(int64_t) diff.size(); ++dim) {
        while(diff[dim]>0) {
          pos_left += database[pos_left].elements_per_dim[dim-1];
          if(pos_left >= database.size()) {
            pos_left = database.size()-1;
            break;
          }
          --diff[dim];
        }
        while(diff[dim]<0) {
          pos_left -= database[pos_left].elements_per_dim[dim-1];
          if(pos_left < 0) {
            pos_left = 0;
            break;
          }
          ++diff[dim];
        }
        pos_left = database[pos_left].this_boss;
      }
      // AJUSTAR LEFT Y RIGHT
       while(database[database[pos_left].prev_boss].score > box_left
                           && database[pos_left].this_boss != 0)
                                     pos_left = database[pos_left].prev_boss;

       while(database[database[pos_left].this_boss].score < box_left)
               pos_left = database[pos_left].next_boss;
      //
      // while(database[database[pos_right].prev_boss].score > box_right
      //               && database[pos_right].this_boss != 0)
      //                       pos_right = database[pos_right].prev_boss;
      //
      //  while(database[database[pos_right].this_boss].score < box_right)
      //          pos_right = database[pos_right].next_boss;

    }

  } while (leap != -1);



  std::cout << "rop = ";
  for(int64_t i=0; i<(int64_t) v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;




  return 0;
}
