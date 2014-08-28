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

// Implements the quick-and-dirty artistic halftoning algorithm.
// See readme.txt for a description.
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

class Pixel {
public:
  int x, y, t;

  void setPosition(int xx, int yy) {x = xx; y = yy;}

  void setThreshold(int tt) {t = tt;}

  Pixel& operator=(const Pixel& other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
      t = other.t;
    }
    return *this;
  }
};

int comparePixelsByThreshold( const void *arg1, const void *arg2 ) {
  Pixel &pix1 = *(Pixel *) arg1;
  Pixel &pix2 = *(Pixel *) arg2;

  if (pix1.t < pix2.t)
    return -1;
  else if (pix1.t > pix2.t)
    return +1;
  else
    return 0;
}

class PixelSubset {
public:
  PixelSubset(int capacity) {
    pixels = new Pixel[capacity];
    size = 0;
  }

  void append(int x, int y) {
    pixels[size++].setPosition(x,y);
  }

  void setThreshold(int i, int t) {
    pixels[i].setThreshold(t);
  }

  Pixel operator[](int index) {
    return pixels[index];
  }

  int getSize() { return size; }

  void sort() {
    qsort( (void *)pixels, (size_t)size, sizeof(Pixel),
           comparePixelsByThreshold );
  }

private:
  Pixel *pixels;
  int size;
};


int getGrayLevels(Image& motif, int *grays, int *grayCounts) {
  for (int i=0; i<256; i++)
    grayCounts[i] = 0;

  int w = motif.getWidth();
  int h = motif.getHeight();
  byte **buf = motif.getBuf();
  for (int y=0; y<h; y++) {
    byte *p = buf[y];
    for (int x=0; x<w; x++) {
      int r = *p++;
      int g = *p++;
      int b = *p++;
      p++;
      int gray = (r + g + b) / 3;
      grayCounts[gray]++;
    }
  }

  int nGrays=0;
  for (int i=0; i<256; i++) {
    if (grayCounts[i] != 0) {
      grays[nGrays++] = i;
    }
  }

  //  for (int i=0; i<nGrays; i++) {
  //    fprintf(dbgfp,"gray[%d]=%d (%d pixels)\n",i,grays[i],grayCounts[grays[i]]);
  //  }

  return nGrays;
}

PixelSubset** getGraySubsets(Image &motif, int nGrays, int *grays, int *grayCounts) {

  PixelSubset **S = new PixelSubset*[nGrays];

  for (int iGray = 0; iGray<nGrays; iGray++) {
    // will hold subset of pixels with this gray level.
    int graySought = grays[iGray];
    int nPixels = grayCounts[graySought];

    //    fprintf(dbgfp,"Subset %d (gray=%d) will have %d pixels\n",iGray,graySought,nPixels);

    S[iGray] = new PixelSubset(nPixels);
  }

  int indexFromGray[256];
  for (int gray=0; gray<256; gray++)
    indexFromGray[gray] = -1; // initially all missing.
  for (int iGray=0; iGray<nGrays; iGray++) {
    int gray = grays[iGray];
    indexFromGray[gray] = iGray;
  }

  //  for (int i=0; i<256; i++) {
  //    fprintf(dbgfp,"indexFromGray[%d]=%d\n",i,indexFromGray[i]);
  //    fflush(dbgfp);
  //  }

  int w = motif.getWidth();
  int h = motif.getHeight();
  byte **buf = motif.getBuf();
  int j=0;
  for (int y=0; y<h; y++) {
    byte *p = buf[y];
    for (int x=0; x<w; x++) {
      int r = *p++;
      int g = *p++;
      int b = *p++;
      p++;
      int gray = (r + g + b) / 3;
      int index = indexFromGray[gray];

      //      fprintf(dbgfp,"(x y) (%d %d) gray=%d index=%d\n",x,y,gray,index);
      //      fflush(dbgfp);

      S[index]->append(x,y);
    }
  }

  //  for (int i=0; i<nGrays; i++) {
  //    int n = S[i]->getSize();
  //    for (int j=0; j<n; j++) {
  //      fprintf(dbgfp,"Subset %d [%d] : (x y t)=(%d %d %d)\n",
  //              i,j,(*S[i])[j].x,(*S[i])[j].y,(*S[i])[j].t);
  //    }
  //  }

  return S;
}

void setSubsetThresholds(PixelSubset **S, int nGrays, Image& dither) {
  int w = dither.getWidth();
  int h = dither.getHeight();

  byte **buf = dither.getBuf();
  for (int i=0; i<nGrays; i++) {
    int n = S[i]->getSize();
    for (int j=0; j<n; j++) {
      Pixel pix = (*S[i])[j];
      int x = pix.x;
      int y = pix.y;
      byte *p = buf[y] + x*4;
      int r = *p++;
      int g = *p++;
      int b = *p++;
      int threshold = (r + g + b) / 3;

      //      fprintf(dbgfp,"i=%d j=%d (x y)=(%d %d) t=%d\n",
      //              i,j,x,y,threshold);

      S[i]->setThreshold(j,threshold);
    }
  }
}

void buildOutput(int nGrays, PixelSubset **S, Image& dither, Image& output) {
  int w = dither.getWidth();
  int h = dither.getHeight();

  output.ensureSpace(w,h);
  int nPixels = w*h;
  int k=0;

  byte **buf = output.getBuf();
  for (int i=0; i<nGrays; i++) {
    int n = S[i]->getSize();
    for (int j=0; j<n; j++) {
      Pixel pix = (*S[i])[j];
      int x = pix.x;
      int y = pix.y;
      int threshold = (k++ * 256)/nPixels;
      byte *p = buf[y] + x*4;
      *p++ = threshold;
      *p++ = threshold;
      *p++ = threshold;
      *p++ = 255;
    }
  }
}

void qdah(Image& motif, Image& dither, Image& output) {
  int grays[256];
  int grayCounts[256];

  //
  // Count the number of gray levels in the motif.
  //
  int nGrays = getGrayLevels(motif, grays, grayCounts);

  //  fprintf(dbgfp,"got %d gray levels\n",nGrays);
  //  fflush(dbgfp);

  //
  // Extract the subset of pixels, one subset per gray level.
  //
  PixelSubset **S = getGraySubsets(motif, nGrays, grays, grayCounts);

  //  fprintf(dbgfp,"got subsets\n");
  //  fflush(dbgfp);

  //
  // Set the threshold for each pixel in each subset,
  // from location in the dither matrix corresponding to it.
  //
  setSubsetThresholds(S, nGrays, dither);

  //  fprintf(dbgfp,"set all the thresholds\n");
  //  fflush(dbgfp);

  //
  // Ok, got all the subsets Si.  Now, for each subset, sort it
  // by threshold.
  //

  for (int i=0; i<nGrays; i++) {
    S[i]->sort();

    //    fprintf(dbgfp,"After sorting subset %d:\n",i);
    //    int n = S[i]->getSize();
    //    for (int j=0; j<n; j++)
    //      fprintf(dbgfp," (i j)=(%d %d): (x y t)=(%d %d %d)\n",
    //              i,j,(*S[i])[j].x,(*S[i])[j].y,(*S[i])[j].t);
  }

  //  fprintf(dbgfp,"sorted ok\n");
  //  fflush(dbgfp);

  //
  // And, finally, set the thresholds in the output image
  // sequentially, in sorted order when all the subsets
  // are concatenated into a master list of pixels.
  //

  buildOutput(nGrays, S, dither, output);

  //  fprintf(dbgfp,"output built ok\n");
  //  fflush(dbgfp);

}

void checkImageSizes(Image& motif, Image& dither) {
  int wm = motif.getWidth();
  int hm = motif.getHeight();
  int wd = dither.getWidth();
  int hd = dither.getHeight();

  if (wm != wd || hm != hd) {
    cerr << "Error: motif and dither images have different dimensions\n";
    exit(EXIT_FAILURE);
  }
}

void qdah(char* motifFile, char* ditherFile,char* outputFile)
{

  Image motif;
  motif.readFile(motifFile);


  Image dither;
  dither.readFile(ditherFile);

  //  fprintf(dbgfp,"read dither ok\n");

  Image output;

  checkImageSizes(motif, dither);

  //  fprintf(dbgfp,"image sizes ok\n");

  qdah(motif, dither, output);;

  //  fprintf(dbgfp,"qdah succeeded\n");
  //  fflush(dbgfp);

  output.writeFile(outputFile);

  //  fprintf(dbgfp,"wrote output\n");
}
