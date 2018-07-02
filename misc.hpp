#ifndef __MISC_HPP
#define __MISC_HPP

#include "data.hpp"

// typedef Data_t<2,double,int64_t> Data;

template<int N, class T>
void assignNavScores(std::vector<Data_t<N,T> > &vec, int64_t nBox)  {
  vec[0].prev_boss = -1;
  vec[0].this_boss = 0;
  for(int64_t i=1; i< (int64_t) vec.size(); ++i){
    if(vec[i].score == vec[i-1].score) {
      vec[i].prev_boss = vec[i-1].prev_boss;
      vec[i].this_boss = vec[i-1].this_boss;
    } else {
      vec[i].prev_boss = vec[i-1].this_boss;
      vec[i].this_boss = i;
      for(int64_t j=i-1; j>=0 && vec[j].score == vec[i-1].score; --j)
      vec[j].next_boss = i;
    }
  }

  for(int64_t j=vec.size()-1;
            j>=0 && vec[j].score == vec[vec.size()-1].score; --j)
    vec[j].next_boss = vec.size();
  return;
}



template<int N, class T>
int64_t getPositionOfScore(std::vector<Data_t<N,T> > &vec,
        std::vector<int64_t> &score, int64_t minPos, int64_t maxPos) {
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


template<int N, class T>
int64_t getPositionOfScore(std::vector<Data_t<N,T> > &vec,
            std::vector<int64_t> &score) {
  return getPositionOfScore(vec, score, 0, vec.size());
}


template<int N, class T>
std::vector<int64_t> getScore(Data_t<N,T> &data, int64_t nBox) {
  std::vector<T> &x = data.x;
  std::vector<int64_t> score(N);
  for(int64_t i=0; i<N; ++i)
    score[i] = floor(x[i]*nBox);
  return score;
}


#endif
