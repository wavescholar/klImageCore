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

// Applies a dither pattern to an image.
// The dither pattern is another image, in which each
// pixel is treated as a threshold.
//
// The thresholds are scaled from the usual range (0.0 to 1.0)
// to 8-bit bytes, (0 to 255).
//
// The three (r g b) components of each pixel in the
// dither pattern are averaged to obtain the threshold
// (this is a simplistic color-to-gray transformation).
//
// Unlike dither.cpp, we don't re-use the same dither matrix.
// Instead, a list of dither images is input, and a different
// entry from this list is chosen at each tile position.
//
// Author: Alejo Hausner (ahausner@truehaus.net) 2008.

#ifdef WIN32
#include <windows.h>
#endif

typedef unsigned char byte;

#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Colour.h"
#include "Image.h"

extern FILE *dbgfp;

bool doRGB = true;

void doDither(Image& in,
              int nThreshImages, Image* threshImages,
              int sequenceLength, int* sequence,
              Image& out) {
  int w = in.getWidth();
  int h = in.getHeight();
  byte **inBuf = in.getBuf();

  int thW = threshImages[0].getWidth();
  int thH = threshImages[0].getHeight();

  //  fprintf(dbgfp,"(thW thH)=(%d %d)\n",thW,thH);

  out.ensureSpace(w,h);
  byte **oBuf = out.getBuf();
  byte **tBuf;

  int tile = -1;

  for (int row=h-1; row>=0; row--) {
    byte *inP = inBuf[row];
    byte *oP  = oBuf[row];

    tile = ((h-1-row) / thH) * (w / thW);
    tile = tile % sequenceLength;

    fprintf(dbgfp,"row=%d tile=%d\n",row,tile);
    fflush(dbgfp);

    tBuf = threshImages[sequence[tile]].getBuf();

    byte *thP;
    for (int col=0; col<w; col++) {
      if (col % thW == 0) {
        thP = tBuf[row % thH];
      }

      //      fprintf(dbgfp,"(row col)=(%d %d) tile=%d\n",row,col,tile);
      //      fflush(dbgfp);

      int r = *inP++;
      int g = *inP++;
      int b = *inP++;
      inP++;

      int tr = *thP++;
      int tg = *thP++;
      int tb = *thP++;
      thP++; // skip alpha byte

      if (col % thW == (thW-1)) {
        tile = ((h-1-row) / thH) * (w / thW) + ((col+1) / thW);
        tile = tile % sequenceLength;

        fprintf(dbgfp,"(row col)=(%d %d) tile=%d\n",row,col,tile);
        fflush(dbgfp);

        tBuf = threshImages[sequence[tile]].getBuf();
        thP = tBuf[row % thH];
      }

      int gray = (r + g + b) / 3;
      int thresh = (tr + tg + tb) / 3;

      if (doRGB) {

        int alpha,beta,gamma,delta;    // the barycentric coords
        int Ar,Ag,Ab;   // the four vertices of the tet.
        int Br,Bg,Bb;
        int Cr,Cg,Cb;
        int Dr,Dg,Db;

        //        fprintf(dbgfp,"\n(row col)=(%d %d) (r g b)=(%d %d %d) thresh=%d\n",
        //                row,col,r,g,b,thresh);

        if (r >= g && g >= b) {
          //
          // We're in tet KRYW
          //
          alpha = (255 - r);
          beta  = (r - g);
          gamma = (g - b);
          delta = b;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br = 255; Bg =   0; Bb =   0; // R
          Cr = 255; Cg = 255; Cb =   0; // Y
          Dr = 255; Dg = 255; Db = 255; // W
        }
        else if (r >= b && b >= g) {
          //
          // tet KRMW
          //
          alpha = (255 - r);
          beta = (r - b);
          gamma = (b - g);
          delta = g;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br = 255; Bg =   0; Bb =   0; // R
          Cr = 255; Cg =   0; Cb = 255; // M
          Dr = 255; Dg = 255; Db = 255; // W
        }
        else if (b >= r && r >= g) {
          //
          // tet KBMW
          //
          alpha = (255 - b);
          beta  = (b - r);
          gamma = (r - g);
          delta = g;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br =   0; Bg =   0; Bb = 255; // B
          Cr = 255; Cg =   0; Cb = 255; // M
          Dr = 255; Dg = 255; Db = 255; // W
        }
        else if (b >= g && g >= r) {
          //
          // tet KBCW
          //
          alpha = (255 - b);
          beta  = (b - g);
          gamma = (g - r);
          delta = r;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br =   0; Bg =   0; Bb = 255; // B
          Cr =   0; Cg = 255; Cb = 255; // C
          Dr = 255; Dg = 255; Db = 255; // W
        }
        else if (g >= b && b >= r) {
          //
          // tet KGCW
          //
          alpha = (255 - g);
          beta  = (g - b);
          gamma = (b - r);
          delta = r;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br =   0; Bg = 255; Bb =   0; // G
          Cr =   0; Cg = 255; Cb = 255; // C
          Dr = 255; Dg = 255; Db = 255; // W
        }
        else if (g >= r && r >= b) {
          //
          // tet KGYW
          //
          alpha = (255 - g);
          beta  = (g - r);
          gamma = (r - b);
          delta = b;

          Ar =   0; Ag =   0; Ab =   0; // K
          Br =   0; Bg = 255; Bb =   0; // G
          Cr = 255; Cg = 255; Cb =   0; // Y
          Dr = 255; Dg = 255; Db = 255; // W
        }


        //     fprintf(dbgfp,"(a b c d)=(%d %d %d %d) (a+b a+b+c)=(%d %d)\n",
        //                alpha,beta,gamma,delta,alpha+beta,alpha+beta+gamma);
        //        fprintf(dbgfp,"A=(%d %d %d)\n",Ar,Ag,Ab);
        //        fprintf(dbgfp,"B=(%d %d %d)\n",Br,Bg,Bb);
        //        fprintf(dbgfp,"C=(%d %d %d)\n",Cr,Cg,Cb);
        //        fprintf(dbgfp,"D=(%d %d %d)\n",Dr,Dg,Db);

        if (thresh < alpha) {
          r = Ar;
          g = Ag;
          b = Ab;
        }
        else if (thresh < alpha + beta) {
          r = Br;
          g = Bg;
          b = Bb;
        }
        else if (thresh < alpha + beta + gamma) {
          r = Cr;
          g = Cg;
          b = Cb;
        }
        else {
          r = Dr;
          g = Dg;
          b = Db;
        }
      }
      else { // not doRGB

          //      fprintf(dbgfp,"thresh=%d r=%d\n",thresh,r);

        if (thresh <= gray) {
          r = g = b = 255;
        }
        else {
          r = g = b = 0;
        }

      }

      *oP++ = r;
      *oP++ = g;
      *oP++ = b;
      *oP++ = 255;
    }
  }
}

void readDitherMatrices(char *filename, int& nFiles, Image*& images) {
  ifstream file(filename);
  if (!file) {
    cerr << "Can't read " << filename << endl;
    exit(EXIT_FAILURE);
  }

  int tileW, tileH;

  file >> nFiles;
  images = new Image[nFiles];
  for (int i=0; i<nFiles; i++) {
    char imageName[80];
    file >> imageName;
    images[i].readFile(imageName);
    if (i == 0) {
      tileW = images[i].getWidth();
      tileH = images[i].getHeight();
    }
    else {
      int w = images[i].getWidth();
      int h = images[i].getHeight();
      if (w != tileW || h != tileH) {
        cerr << "FATAL! image #" << i
             << " has (w h)=(" << w << " " << h
             << ") but first image has (w h)=("
             << tileW << " " << tileH << ")\n";
        exit(EXIT_FAILURE);
      }
    }
  }

  file.close();
}

void readSequence(char *filename, int& nTiles, int*& tiles,
                  int nThreshImages) {
  ifstream file(filename);
  if (!file) {
    cerr << "Can't read " << filename << endl;
    exit(EXIT_FAILURE);
  }

  file >> nTiles;
  tiles = new int[nTiles];
  for (int i=0; i<nTiles; i++) {
    int k;
    file >> k;
    if (k >= nThreshImages) {
      cerr << "BAD! found image # " << k
           << " in sequence, but there are only\n";
      cerr << nThreshImages << " available!  Will use 0 instead.\n";
      k = 0;
    }
    tiles[i] = k;

    fprintf(dbgfp,"tile[%d]=%d\n",i,k);
    fflush(dbgfp);
  }

  file.close();
}

void multi_dither(char* inputFile, char* ditherFileList,char* sequenceFileList,char* outputFile) 
{
 
  Image inImg;
  inImg.readFile(inputFile);

  fprintf(dbgfp,"Read input file %s ok\n",inputFile);
  fflush(dbgfp);

  int nThreshImages;
  Image *threshImages;
  readDitherMatrices(ditherFileList, nThreshImages, threshImages);

  fprintf(dbgfp,"Read %d threshold images from %s ok\n",nThreshImages,ditherFileList);
  fflush(dbgfp);

  int sequenceLength;
  int *sequence;
  readSequence(sequenceFileList, sequenceLength, sequence, nThreshImages);

  fprintf(dbgfp,"Read sequence length %d from %s ok\n",sequenceLength,sequenceFileList);
  fflush(dbgfp);


  Image outImg;
  doDither(inImg, nThreshImages, threshImages,
           sequenceLength, sequence,
           outImg);

  outImg.writeFile(outputFile);
}

