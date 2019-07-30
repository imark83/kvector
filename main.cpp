#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "data.hpp"
#include "database.hpp"
#include "misc.hpp"


#include <opencv2/opencv.hpp>

using namespace cv;

int64_t movidas = 0;

double scale = 0.05+pow(0.00001,1.0/3.0);

double minval[9] = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05};
double maxval[9] = {scale, scale, scale, scale, scale, scale, scale, scale, scale};


std::ostream & operator<<(std::ostream &output,
        const std::vector<int64_t> &op) {
  if(op.size() == 0) return output;
  output << "(" << op[0];
  for(int64_t i=1; i<(int64_t) op.size(); ++i)
    output << ", " << op[i];
  output << ")";
  return output;
}


int main(int argc, char const *argv[]) {

  std::ofstream fout;
  fout.open("tulips.txt");

  std::cout << "scale = " << scale << std::endl;

  // MEASURE TIME
  clock_t c0, c1;
  double timeBF, timeKP;
  int retrieved;



  std::cout.precision(2);

  Mat image0, image1, image2;
  image0 = imread("t00.jpg", IMREAD_COLOR);
  image1 = imread("t01.jpg", IMREAD_COLOR);
  image2 = imread("t02.jpg", IMREAD_COLOR);

  //imshow("prueba", image);
  //int key = waitKey(0);


  for (int64_t k = 2; k<=9; ++k) {

    int64_t ndim = k;
    int64_t nBox = 100;
    int64_t dataBaseSize = image0.rows*image0.cols;
    int64_t subdim = 
        (int64_t) floor(log(dataBaseSize)/log(nBox));

    if (subdim > ndim) subdim = ndim;
    std::cout << "subdim = " << subdim << std::endl;


    Database<double> database(dataBaseSize, ndim, nBox, subdim);

    for(int64_t i=0; i<database.size(); ++i) {
      for(int64_t j=0; j<ndim; ++j) {
        if(j<3) 
          database[i].x[j] = (image0.data[3*i+j])/255.0;
        else if (j<6)
          database[i].x[j] = (image1.data[3*i+j-3])/255.0;
        else if (j<9)
          database[i].x[j] = (image2.data[3*i+j-6])/255.0;
      }
      database[i].score = getScore(database[i], nBox);
    }

    std::cout << "database size = " << dataBaseSize << std::endl;
    std::cout << "start preprocessing..." << std::endl;
    mergesort(database);
    database.setScores();
    computeElementsPerDimension(database);




    // for(int64_t i=0; i<database.size(); ++i) {
    //   std::cout << i << " -> " << database[i] << std::endl;
    // }

    std::cout << "done. Start brute force for dim " << ndim << std::endl;

    for(scale=0.06; scale<=1; scale+=0.01) {
      double maxval[9] = {scale, scale, scale, scale, scale, scale, scale, scale, scale};


      // SEARCH RANGE [0.25,0.75)x[0.25,0.75)x[0.25,0.75)
      // brute force
      std::vector<int64_t> v(0), temp(0);

      c0 = clock();
      for(int64_t i=0; i<database.size(); ++i) {
        bool valid = true;
        for(int64_t j=0; j<ndim; ++j) {
          if(database[i][j] >= maxval[j]) {
            valid = false;
            break;
          }
          if(database[i][j] < minval[j]) {
            valid = false;
            break;
          }
        }
        if(valid) v.push_back(i);
      }
      c1 = clock();

      timeBF = (((float) (c1-c0))/CLOCKS_PER_SEC);
      retrieved = v.size();

      std::cout << "rop = " << v.size() << std::endl;
      std::cout << std::scientific;
      std::cout << "\t" << (((float) (c1-c0))/CLOCKS_PER_SEC) << std::endl;
      std::cout << "start k-pollas" << std::endl;



      // std::cout << v << std::endl;
      v.clear();

      // database.print();



      Score_t box0(subdim);
      Score_t box1(subdim);
      // INITIALIZE BOX SEARCH box0 <= val < box1
      for(int i=0; i<subdim; ++i) {
        box0[i] = floor(minval[i]*nBox);
        box1[i] = ceil(maxval[i]*nBox);
      }

      c0 = clock();
      rangeSearch(temp, database, box0, box1);
      for(int64_t i=0; i<(int64_t) temp.size(); ++i) {
        bool valid = true;
        for(int64_t j=ndim-1; j>=0; --j) {
          if(database[temp[i]][j] >= maxval[j]) {
            valid = false;
            break;
          }
          if(database[temp[i]][j] < minval[j]) {
            valid = false;
            break;
          }
        }
        if(valid) v.push_back(temp[i]);
      }
      c1 = clock();

      // std::cout << v << std::endl;


      std::cout << "rop = " << v.size() << std::endl;
      std::cout << std::scientific;
      std::cout << "\t" << (((float) (c1-c0))/CLOCKS_PER_SEC) << std::endl;

      std::cout << "movidas = " << movidas << std::endl << std::endl;

      v.clear();
      timeKP = (((float) (c1-c0))/CLOCKS_PER_SEC);


      fout << k << " " << retrieved << " " 
          << timeBF << " " << timeKP << std::endl;

    }

  }
  fout.close();
  return 0;
}
