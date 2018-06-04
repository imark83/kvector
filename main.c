#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _data_t {
  double x[2];
  size_t tag;
  long long int prev_tag;
  long long int this_tag;
  long long int next_tag;
} data_t;

void quickSort(data_t *arr, int left, int right);
void assignTag(data_t *vector, size_t N);


// returns first element with tag >= inputTag
size_t getPositionOfTag(data_t *vector,
        size_t N, size_t tag);

void printRange(data_t *vector, size_t N,
          size_t first_tag, size_t last_tag) {
  size_t position = getPositionOfTag(vector, N, first_tag);
  if(position == N) return;
  if(vector[position].tag > last_tag) return;
  do {
    printf("(%f,%f)\n", vector[position].x[0], vector[position].x[1]);
    ++position;
  } while(vector[position].tag <= last_tag);

  return;
}

void printBox(data_t *vector, size_t N, size_t nBox,
          double x0, double x1, double y0, double y1) {
  size_t minRow = floor(y0*nBox);
  size_t maxRow = floor(y1*nBox);
  size_t minColumn = floor(x0*nBox);
  size_t maxColumn = floor(x1*nBox);
  size_t i;
  size_t first_tag;
  size_t last_tag;
  for(i=minRow; i<=maxRow; ++i){
    first_tag = minColumn + i*nBox;
    last_tag = maxColumn + i*nBox;
    printRange(vector, N, first_tag, last_tag);
  }
}

int main(int argc, char const *argv[]) {
  size_t nBox = 1000;
  size_t N = 100000000;
  data_t *vector = (data_t *) malloc(N * sizeof(struct _data_t));
  // data_t vector[N];

  size_t i, j;

  for(i=0; i<N; ++i) {
    double x = ((double) rand()) / RAND_MAX;
    double y = ((double) rand()) / RAND_MAX;
    vector[i].x[0] = x;
    vector[i].x[1] = y;
    vector[i].tag = (size_t) floor(x*nBox) + nBox*floor(y*nBox);
  }

  quickSort(vector, 0, N);
  assignTag(vector, N);


  printBox(vector, N, nBox, 0.4, 0.4999999, 0.6, 0.7);

  return 0;
}




void quickSort(data_t *arr, int left, int right) {
  int i = left, j = right;
  data_t tmp;
  data_t pivot = arr[(left + right) / 2];

  /* partition */
  while(i <= j) {
    while(arr[i].tag < pivot.tag)
      ++i;
    while(arr[j].tag > pivot.tag)
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


void assignTag(data_t *vector, size_t N) {
  long long int i, j;

  vector[0].prev_tag = -1;
  vector[0].this_tag = vector[0].tag;
  for(i=1; i<N; ++i){
    if(vector[i].tag != vector[i-1].tag) {
      vector[i].prev_tag = vector[i-1].this_tag;
      vector[i].this_tag = i;
      for(j=i-1; j>=0 && vector[j].tag == vector[i-1].tag; --j)
        vector[j].next_tag = i;
    } else {
      vector[i].prev_tag = vector[i-1].prev_tag;
      vector[i].this_tag = vector[i-1].this_tag;
    }
  }
  for(j=N-1; j>=0 && vector[j].tag == vector[N-1].tag; --j)
    vector[j].next_tag = N;
  return;
}

size_t getPositionOfTag(data_t *vector,
        size_t N, size_t tag) {

  // CHECK LEFT
  if(vector[0].tag>tag) return 0;
  if(vector[N-1].tag<tag) return N;
  size_t left = 0, right = N-1;
  size_t mid = (left + right) / 2;
  do {
    if(vector[mid].tag == tag)
      return vector[mid].this_tag;

    if(vector[mid].tag>tag)
      right = mid;
    else
      left = mid;
    mid = (left + right) / 2;
  } while (left != mid);

  if(vector[mid].tag == tag) return vector[mid].this_tag;

  return vector[right].this_tag;
}
