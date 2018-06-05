#include "misc.hpp"



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




void assignNavScores(std::vector<Data> &vec, int64_t nBox) {
  // vec[0].prev_boss[0] = -1;
  vec[0].this_boss = vec[0].score;
  for(int64_t i=1; i<vec.size(); ++i){
    if(vec[i].score != vec[i-1].score) {
      // vec[i].prev_boss[0] = vec[i-1].this_boss;
      vec[i].this_boss = i;
      for(int64_t j=i-1; j>=0 && vec[j].score == vec[i-1].score; --j)
        vec[j].next_boss[0] = i;
    } else {
      // vec[i].prev_boss[0] = vec[i-1].prev_boss[0];
      vec[i].this_boss = vec[i-1].this_boss;
    }
  }


  for(int64_t j=vec.size()-1;
            j>=0 && vec[j].score == vec[vec.size()-1].score; --j)
    vec[j].next_boss[0] = vec.size();



  // NOW ASSIGN NEXT AND PREV IN OTHER DIMENSIONS
  int NDIM = vec[0].nDim();
  int64_t accum_nBox = nBox;
  for(int j=1; j<NDIM; ++j) {
    vec[0].next_boss[j] = getPositionOfScore(vec, vec[0].score + accum_nBox);
    accum_nBox *= nBox;
  }
  for(int64_t i=1; i<vec.size(); ++i) {
    if(vec[i].score == vec[i-1].score) {
      for(int j=1; j<NDIM; ++j) {
        vec[i].next_boss[j] = vec[i-1].next_boss[j];
      }
    } else {
      accum_nBox = nBox;
      for(int j=1; j<NDIM; ++j) {
        vec[i].next_boss[j] =
                getPositionOfScore(vec, vec[i].score + accum_nBox);
        accum_nBox *= nBox;
      }
    }
  }
  return;
}


int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score) {
  return getPositionOfScore(vec, score, 0, vec.size());
}


// search element in range [minPos maxPos) with score
int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score, int64_t minPos, int64_t maxPos) {


  // CHECK LEFT
  if(vec[minPos].score>score) return minPos;
  if(vec[maxPos-1].score<score) return maxPos;
  int64_t left = minPos, right = maxPos;
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
