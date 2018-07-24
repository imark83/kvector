#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__


#include "data.hpp"
#include "quicksort.hpp"
#include "mergesort.hpp"



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



template<int N, class T>
int64_t getPositionOfScore(Database<N,T> &database,
          std::vector<int64_t> &score) {
  return getPositionOfScore(database.vec, score);
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




template<int N, class T>
void rangeSearch(std::vector<int64_t> &v, Database<N,T> &database,
            Score_t box0, Score_t box1) {


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

  return;
}


#endif
