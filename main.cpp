#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <vector>

template<class T, int64_t N>
class Data_t {
public:
  std::vector<T> x;
  int64_t score; // TO CONVERT TO LIBGMP BIG INT
  int64_t this_boss;
  int64_t next_boss[N];
  int64_t prev_boss[N];

  // Data_t(T x[N]) {
  //   for(int64_t i=0; i<N; ++i) : x(N) {
  //     this->x[i] = x[i];
  //     next_boss[i] = prev_boss[i] = -1;
  //   }
  //   this_boss = -1;
  // }

  Data_t() : x(N, T(0)){
    for(int64_t i=0; i<N; ++i) {
      next_boss[i] = prev_boss[i] = -1;
    }
    this_boss = -1;
  }

  void swap(Data_t<T,N> &op) {
    T aux;
    int64_t aux2;
    for(int64_t i=0; i<N; ++i) {
      aux = x[i]; x[i] = op.x[i]; op.x[i] = aux;
      aux2 = prev_boss[i];
          prev_boss[i] = op.prev_boss[i]; op.prev_boss[i] = aux2;
      aux2 = next_boss[i];
          next_boss[i] = op.next_boss[i]; op.next_boss[i] = aux2;
    }
    aux2 = this_boss; this_boss = op.this_boss; op.this_boss = aux2;
  }

};

typedef Data_t<double, 2> Data;

void quickSort(std::vector<Data> &vec);
void quickSort(std::vector<Data> &vec, int left, int right);
void assignNavScores(std::vector<Data> &vec);
int64_t getPositionOfTag(std::vector<Data> &vec, int64_t score);

int64_t getUpperBox(int64_t, int dim, int) {

}


int main(int argc, char const *argv[]) {
  int64_t nBox = 2;
  int64_t dataBaseSize = 20;

  std::vector<Data> vec(dataBaseSize);

  for(int i=0; i<vec.size(); ++i) {
    double x = ((double) rand()) / RAND_MAX;
    double y = ((double) rand()) / RAND_MAX;
    vec[i].x[0] = x;
    vec[i].x[1] = y;
    vec[i].score = (int64_t) floor(x*nBox) + nBox*floor(y*nBox);
  }

  quickSort(vec);
  assignNavScores(vec);



  return 0;
}






void quickSort(std::vector<Data> &arr) {
  quickSort(arr, 0, arr.size());
}



void quickSort(std::vector<Data> &arr, int left, int right) {
  int i = left, j = right;
  Data tmp;
  Data pivot = arr[(left + right) / 2];

  /* partition */
  while(i <= j) {
    while(arr[i].score < pivot.score)
      ++i;
    while(arr[j].score > pivot.score)
      --j;
    if(i <= j) {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      ++i;
      --j;
    }
  }
  /* recursion */
  if(left < j)
    quickSort(arr, left, j);
  if(i < right)
    quickSort(arr, i, right);
}




void assignNavScores(std::vector<Data> &vec) {

  vec[0].prev_boss[0] = -1;
  vec[0].this_boss = vec[0].score;
  for(int64_t i=1; i<vec.size(); ++i){
    if(vec[i].score != vec[i-1].score) {
      vec[i].prev_boss[0] = vec[i-1].this_boss;
      vec[i].this_boss = i;
      for(int64_t j=i-1; j>=0 && vec[j].score == vec[i-1].score; --j)
        vec[j].next_boss[0] = i;
    } else {
      vec[i].prev_boss[0] = vec[i-1].prev_boss[0];
      vec[i].this_boss = vec[i-1].this_boss;
    }
  }
  for(int64_t j=vec.size()-1;
            j>=0 && vec[j].score == vec[vec.size()-1].score; --j)
    vec[j].next_boss[0] = vec.size();



  // NOW ASSIGN NEXT AND PREV IN OTHER DIMENSIONS
  int64_t currentScore = vec[0].score;
  int NDIM = vec[0].x.size();
  int64_t curretnPrevBoss[NDIM];
  int64_t curretnNextBoss[NDIM];
  for(int64_t i=0; i<vec.size; ++i);


  return;
}

int64_t getPositionOfTag(std::vector<Data> &vec, int64_t score) {

  // CHECK LEFT
  if(vec[0].score>score) return 0;
  if(vec[vec.size()-1].score<score) return vec.size();
  int64_t left = 0, right = vec.size()-1;
  int64_t mid = (left + right) / 2;
  do {
    if(vec[mid].score == score)
      return vec[mid].this_boss;

    if(vec[mid].score>score)
      right = mid;
    else
      left = mid;
    mid = (left + right) / 2;
  } while (left != mid);

  if(vec[mid].score == score) return vec[mid].this_boss;

  return vec[right].this_boss;
}
