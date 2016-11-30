/*
Copyright 2011. All rights reserved.
Institute of Measurement and Control Systems
Karlsruhe Institute of Technology, Germany

This file is part of libicp.
Authors: Andreas Geiger

libicp is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or any later version.

libicp is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
libicp; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

// Demo program showing how libicp can be used
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "icpPointToPlane.h"

using namespace std;

bool Getdata(string filename,vector<vector<double> >& data){
  ifstream reading_file;
  reading_file.open(filename,ios::in);
  string reading_line_buffer;

  double nm;
  char comma;

  while(!reading_file.eof()){
    vector<double> temp_data;
    getline(reading_file,reading_line_buffer);
    istringstream is(reading_line_buffer);

    while(!is.eof()){
      is >> nm >> comma;
      temp_data.push_back(nm);
    }
    data.push_back(temp_data);

  }

  return true;

}

int main (int argc, char** argv) {

  vector<vector<double> > data1;//読み込み先2次元vectorを用意する
  vector<vector<double> > data2;
  Getdata("output341c.csv",data1);
  Getdata("output343c.csv",data2);

  // define a 3 dim problem with 10000 model points
  // and 10000 template points:
  int32_t dim = 2;
  int32_t num = 682;
  // int32_t num = 10000;

  // allocate model and template memory
  double* M = (double*)calloc(2*num,sizeof(double));
  double* T = (double*)calloc(2*num,sizeof(double));


  // set model and template points
  cout << endl << "Creating model with 10000 points ..." << endl;
  cout << "Creating template by shifting model by (1,1,1) ..." << endl;
  int32_t k=0;
  // for (double x= 0 ; x< 0.682; x+=0.001) {
  cout << data1.size() << endl;
    for (int32_t k = 0; k < 682; k++) {
    // for (double y=-2; y<2; y+=0.04) {
      M[k*2+0] = data1[k][0]/10000;
      M[k*2+1] = data1[k][1]/10000;
      T[k*2+0] = data2[k][0]/10000;
      T[k*2+1] = data2[k][1]/10000;
      cout <<k*2<<","<<k*2+1<<" "<< M[k*2+0] <<" "<< M[k*2+1]<<" "<< T[k*2+0]<<" "<< T[k*2+1]<<endl;

      // double y = 100*x*exp(-x*x);
      // M[k*2+0] = x;
      // M[k*2+1] = y;
      // T[k*2+0] = x-10;
      // T[k*2+1] = y-10;
      // cout << M[k*2+0] <<" "<< M[k*2+1]<<" "<< T[k*2+0]<<" "<< T[k*2+1]<<endl;

      // double z=5*x*exp(-x*x-y*y);
      // M[k*3+0] = x;
      // M[k*3+1] = y;
      // M[k*3+2] = z;
      // T[k*3+0] = x-1;
      // T[k*3+1] = y-1;
      // T[k*3+2] = z-1;
      // k++;
    // }
  }


  // start with identity as initial transformation
  // in practice you might want to use some kind of prediction here
  Matrix R = Matrix::eye(2);
  Matrix t(2,1);

  // run point-to-plane ICP (-1 = no outlier threshold)
  cout << endl << "Running ICP (point-to-plane, no outliers)" << endl;
  IcpPointToPlane icp(M,num,dim);
  icp.fit(T,num,R,t,-1);
  // results
  cout << endl << "Transformation results:" << endl;
  cout << "R:" << endl << R << endl << endl;
  cout << "t:" << endl << t << endl << endl;

  // free memory
  free(M);
  free(T);



  // success
  return 0;
}
