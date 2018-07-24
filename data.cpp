#include "data.hpp"

bool operator==(const Score_t &op1,
          const Score_t &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] != op2[i])
      return 0;
  }
  return 1;
}



bool operator>(const Score_t &op1,
          const Score_t &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] > op2[i])
      return 1;
    if(op1[i] < op2[i])
      return 0;
  }
  return 0;
}


bool operator<(const Score_t &op1,
          const Score_t &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] < op2[i])
      return 1;
    if(op1[i] > op2[i])
      return 0;
  }
  return 0;
}


bool operator<=(const Score_t &op1,
          const Score_t &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] < op2[i])
      return 1;
    if(op1[i] > op2[i])
      return 0;
  }
  return 1;
}


bool operator>=(const Score_t &op1,
          const Score_t &op2) {
  for(int i=op1.size()-1; i>=0; --i) {
    if(op1[i] > op2[i])
      return 1;
    if(op1[i] < op2[i])
      return 0;
  }
  return 1;
}



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



int64_t maxLeap(const Score_t &score0, const Score_t &score1) {
  for(int64_t i=score0.size()-1; i>=0; --i) {
    if (score0[i] != score1[i])
      return i;
  }
  return 0;
}


// RETURNS WHETHER THERE IS (NEXT BOX DIMENSION) NEXT BOX OR NOT (-1)
int next(Score_t &box, const Score_t &box0,
        const Score_t &box1, const int i) {
  box[i] += 1;
  if(box[i] == box1[i]) {
    if(i==(int64_t) box.size()-1) return -1;
    box[i] = box0[i];
    return next(box, box0, box1, i+1);
  }
  return i;
}

// RETURNS WHETHER THERE IS NEXT BOX
int next(Score_t &box, const Score_t &box0,
  const Score_t &box1) {
    return next(box, box0, box1, 0);
}
