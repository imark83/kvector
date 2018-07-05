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
