//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Generates Bayer's dither matrices, of various sizes.
//
// Example matrices:
// 2x2:
//  0 2
//  3 1
//
// 4x4:
//   0  8  2 10
//  12  4 14  6
//   3 11  1  9
//  15  7 13  5
//
// Method:
// - Start with 2x2 matrix, M0
//   0 2
//   3 1
// - Make matrices, each twice the size of the previous one.
//   The i-th matrix, Mi, is
//   A B
//   C D
//   where A,B,C,D are sub-matrices half the size (the size of M(i-1)),
//   A = 4*M(i-1)
//   D = 4*M(i-1)+1
//   B = 4*M(i-1)+2
//   C = 4*M(i-1)+3
//
// Author: Alejo Hausner (ahausner@truehaus.net) 2008.

#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "Image.h"

using namespace std;

FILE *dbgfp = fopen("debug.dat","w");



int **doubleUp(int **Mk1, int k) {
  int w = (int)pow(2.0, (double)(k+1));
  int **Mk = new int*[w];
  for (int i=0; i<w; i++)
    Mk[i] = new int[w];

  fprintf(dbgfp,"doubleUp: k=%d\n",k);

  // sub-matrix A
  for (int i=0; i<w/2; i++) {
    for (int j=0; j<w/2; j++) {
      Mk[i][j] = Mk1[i][j] * 4;
    }
  }

  // sub-matrix D
  for (int i=0; i<w/2; i++) {
    for (int j=0; j<w/2; j++) {
      Mk[i+w/2][j+w/2] = Mk1[i][j] * 4 + 1;
    }
  }

  // sub-matrix B
  for (int i=0; i<w/2; i++) {
    for (int j=0; j<w/2; j++) {
      Mk[i][j+w/2] = Mk1[i][j] * 4 + 2;
    }
  }

  // sub-matrix C
  for (int i=0; i<w/2; i++) {
    for (int j=0; j<w/2; j++) {
      Mk[i+w/2][j] = Mk1[i][j] * 4 + 3;
    }
  }

  fprintf(dbgfp,"k=%d w=%d\n",k,w);
  for (int i=0; i<w; i++) {
    for (int j=0; j<w; j++) {
      fprintf(dbgfp," %3d",Mk[i][j]);
    }
    fprintf(dbgfp,"\n");
  }

  return Mk;
}

void output(int **Mk, int k) {
  Image out;
  int w = (int)pow(2.0, (double)k);
  out.ensureSpace(w,w);
  double scale = 255.0 / (w*w-1);

  byte **buf = out.getBuf();
  for (int i=0; i<w; i++) {
    byte *p = buf[i];
    for (int j=0; j<w; j++) {
      *p++ = (int)(Mk[i][j] * scale + 0.1);
      *p++ = (int)(Mk[i][j] * scale + 0.1);
      *p++ = (int)(Mk[i][j] * scale + 0.1);
      *p++ = 255;
    }
  }

  char filename[80];
  sprintf(filename,"bayer%d.ppm",k);
  out.writeFile(filename);
}

int mainBayer(int argc, char *argv[]) {
  if (argc != 1) {
    cerr << "Usage:\n";
    cerr << "  bayer\n";
    exit (EXIT_FAILURE);
  }

  int **M0 = new int*[2];
  M0[0] = new int[2];
  M0[1] = new int[2];
  M0[0][0] = 0;
  M0[0][1] = 2;
  M0[1][0] = 3;
  M0[1][1] = 1;

  output(M0, 1);

  int **M1 = doubleUp(M0, 1);
  output(M1, 2);

  int **M2 = doubleUp(M1, 2);
  output(M2, 3);

  int **M3 = doubleUp(M2, 3);
  output(M3, 4);

  int **M4 = doubleUp(M3, 4);
  output(M4, 5);

  int **M5 = doubleUp(M4, 5);
  output(M5, 6);

  int **M6 = doubleUp(M5, 6);
  output(M6, 7);

  int **M7 = doubleUp(M6, 7);
  output(M7, 8);
}


