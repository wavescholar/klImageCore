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

FILE *dbgfp = fopen("debug.dat","w");

void doDither(Image& in, Image& th, Image& out) {
  int w = in.getWidth();
  int h = in.getHeight();
  byte **inBuf = in.getBuf();

  int thW = th.getWidth();
  int thH = th.getHeight();

  byte **thBuf = th.getBuf();

  out.ensureSpace(w,h);
  byte **oBuf = out.getBuf();

  for (int row=0; row<h; row++) {
    byte *inP = inBuf[row];
    byte *oP  = oBuf[row];
    byte *thP = thBuf[row % thH];

    for (int col=0; col<w; col++) {
      int r = *inP++;
      int g = *inP++;
      int b = *inP++;
      inP++;

      int tr = *thP++;
      int tg = *thP++;
      int tb = *thP++;
      thP++; // skip alpha byte

      if (col % thW == (thW-1))
        thP = thBuf[row % thH];

      int gray = (r + g + b) / 3;
      int thresh = (tr + tg + tb) / 3;

      if (gray > thresh) {
        *oP++ = 255;
        *oP++ = 255;
        *oP++ = 255;
        *oP++ = 255;
      }
      else {
        *oP++ = 0;
        *oP++ = 0;
        *oP++ = 0;
        *oP++ = 255;
      }
    }
  }
}

void dither_decroative(char* imgFile,char* decorativeFile,char* outputFile) 
{
 

  Image inImg;
  inImg.readFile(imgFile);

  Image threshImg;
  threshImg.readFile(decorativeFile);

  Image outImg;
  doDither(inImg, threshImg, outImg);

  outImg.writeFile(outputFile);
}

