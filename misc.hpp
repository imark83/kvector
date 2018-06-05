#ifndef __MISC_HPP
#define __MISC_HPP

#include "data.hpp"
typedef Data_t<double, 2> Data;


void quickSort(std::vector<Data> &vec);
void quickSort(std::vector<Data> &vec, int left, int right);
void assignNavScores(std::vector<Data> &vec, int64_t nBox);
int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score);
int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score,
        int64_t minPos, int64_t maxPos);

#endif
