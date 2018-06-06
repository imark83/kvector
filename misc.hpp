#ifndef __MISC_HPP
#define __MISC_HPP

#include "data.hpp"
typedef Data_t<2, double, int64_t> Data;



void assignNavScores(std::vector<Data> &vec, int64_t nBox);
int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score);
int64_t getPositionOfScore(std::vector<Data> &vec, int64_t score,
        int64_t minPos, int64_t maxPos);

#endif
