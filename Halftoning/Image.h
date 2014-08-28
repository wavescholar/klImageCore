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
//
// Author: Alejo Hausner (ahausner@truehaus.net) 2008.

#ifndef IMAGE_H
#define IMAGE_H

#ifdef WIN32
#include <windows.h>
#endif

#include "Colour.h"

typedef unsigned char byte;

#include <vector>
using namespace std;

class Image {
public:
  Image();
  ~Image();
  void ensureSpace(int width, int height);
  void readFile(char *filename);
  void readFile_OLD(char *filename);
  void readScreen(int width, int height);
  void drawScreen(int width, int height);
  void writeFile();
  void writeFile(char *filename);
  void setScale(double xC, double yC, double radius);
  void setDimensions(int height, int width);
  byte **getBuf() {return buf;};
  int getWidth() {return width;};
  int getHeight() {return height;};
  void rc2xy(int row, int col, double *x, double *y);
  void xy2rc(double x, double y, int *row, int *col);
  void setCoordSystem(double xMin, double xMax, double yMin, double yMax);
  double getRadius() {return radius;};
  Colour getValue(double x, double y);
  void clear(int r, int g, int b);
  void setTransparency(int r, int g, int b);
  void init(Image& other);
  void flipHorizontal();

protected:
  byte **buf;
  int width,height;
  double maxX,minX,maxY,minY;
  double xC,yC,radius;
};

#endif
