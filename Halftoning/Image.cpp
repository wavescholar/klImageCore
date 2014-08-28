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
// An rgb image.  Includes code to read the image from the
// screen, and save/read the image from a PPM file on disk.
//
// Author: Alejo Hausner (ahausner@truehaus.net) 2008.


#include "Image.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <fstream>
#include <iostream>

extern FILE *dbgfp;

Image::Image() {
  //  cout << "Image constr." << endl;
  buf = NULL;
  height = width = 0;
}

/////////////////////////////////////////////////
//
// Destructor.  Release the image array.
//
Image::~Image() {
  if (height > 0) {
    for (int row=0; row<height; row++)
      delete[] buf[row];
    delete[] buf;
  }
}

void Image::init(Image& other) {
  int w = other.getWidth();
  int h = other.getHeight();

  ensureSpace(w,h);

  byte **srcbuf = other.getBuf();
  byte **dstbuf = getBuf();
  byte *src,*dst;
  for (int row=0; row<h; row++) {
    src = srcbuf[row]; // points to start of row.
    dst = dstbuf[row]; // points to start of row.
    for (int col=0; col<w; col++) {
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
    }
  }
}

void Image::writeFile() {
  writeFile("scene.ppm");
}

void Image::writeFile(char *filename) {
  FILE *fp;
  int row,col;
  byte *scanLine;
  byte *src,*dst;

  //  fprintf(dbgfp,"Saving image %s: %d x %d\n", filename,width,height);
  fp = fopen(filename,"wb");
  if (fp == NULL) {
    fprintf(stderr,"Can't write image file %s\n",filename);
    exit(1);
  }

  char header[50];
  sprintf(header,"P6\n%d %d\n255\n",width,height);
  fwrite(header,1,strlen(header),fp);

  /*
   *  fprintf(fp, "P3\n");
   *  fprintf(fp, "%d %d\n", width, height);
   *  fprintf(fp, "255\n");
   */

  //  fprintf(dbgfp,"Image::writeFile. first 10 bytes:\n");
  //  for (col=0; col<10; col++)
  //    fprintf(dbgfp,"%d ",buf[0][col]);
  //  fprintf(dbgfp,"\n");

  scanLine = new byte[width*3];
  for ( row = height-1; row>=0; row-- ) {
    src = buf[row];
    dst = scanLine;
    for (col=0; col<width; col++) {
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
      src++;   // 4 bytes per pixel, not 3 (rgba)
    }
        /*
         *      dst = scanLine;
         *    unsigned char r,g,b;
         *        for (col=0; col<width; col++) {
         *          r = *dst++;
         *          g = *dst++;
         *          b = *dst++;
         *          fprintf(fp,"%d %d %d ",r,g,b);
         *          if (col % 6 == 5)
         *                fprintf(fp,"\n");
         *        }
         */

    fwrite(scanLine,3,width,fp);
  }

  delete[] scanLine;
  fclose(fp);
}

void Image::rc2xy(int row, int col, double *x, double *y) {
  double dx = maxX - minX;
  double dy = maxY - minY;
  *x = minX + (col+0.5) * dx / width;
  *y = minY + (row+0.5) * dy / height;
}

void Image::xy2rc(double x, double y, int *row, int *col) {
  double dx = maxX - minX;
  double dy = maxY - minY;
  *col = (int)((x - minX)*width /dx - 0.5);
  *row = (int)((y - minY)*height/dy - 0.5);
}

Colour Image::getValue(double x, double y) {
  int row,col;

  //  fprintf(dbgfp,"Image::getValue(double) (x y)=(%5.3lf %5.3lf)\n",x,y);

  xy2rc(x,y, &row,&col);

  if (row < 0)        row = 0;
  if (row > height-1) row = height-1;
  if (col < 0)        col = 0;
  if (col > width-1)  col = width-1;

  byte* p = buf[row] + 4*col;
  Colour c;
  int r = *p++;
  int g = *p++;
  int b = *p;

  //  fprintf(dbgfp,"(row col)=(%d %d) rgb=(%d %d %d)\n",row,col,r,g,b);

  c.set(r,g,b);

  return c;
}

void Image::ensureSpace(int newWidth, int newHeight) {

  if (height != newHeight || width != newWidth) {
    int y;

    if (buf != NULL) {
      for (y=0; y<height; y++)
        delete[] buf[y];
      delete[] buf;
    }

    height = newHeight;
    width  = newWidth;
    buf = new byte* [height];
    for (y=0; y<height; y++)
      buf[y] = new byte[4*width];

    //    fprintf(dbgfp,"Image::ensureSpace. (w h): old=(%d %d) new=(%d %d)\n",
    //      width,height,newWidth,newHeight);
    //    fprintf(dbgfp,"reallocated image buf\n");

  }
}

void Image::readScreen(int width, int height) {
  int y;

  ensureSpace(width, height);

  for (y=0; y<height; y++) {
    glReadPixels(0,y,width,1,GL_RGBA,GL_UNSIGNED_BYTE,
               (GLvoid *) buf[y]);

    //    fprintf(dbgfp,"read screen: row %d (%d %d %d %d) (%d %d %d %d) (%d %d %d %d) (%d %d %d %d)\n",
    //      y,
    //      buf[y][0],buf[y][1],buf[y][2],buf[y][3],
    //      buf[y][4],buf[y][5],buf[y][6],buf[y][7],
    //      buf[y][8],buf[y][9],buf[y][10],buf[y][11],
    //      buf[y][12],buf[y][13],buf[y][14],buf[y][15]);
  }

//  for ( y = 0; y<height; y++ ) {
//      glReadPixels(0,y,width,1,
//                   GL_RGBA,GL_UNSIGNED_BYTE,
//                   (GLvoid *) (buf+y*width*4));
//  }
}

void Image::setCoordSystem(double xMin, double xMax,
                           double yMin, double yMax) {
  maxX = xMax;
  minX = xMin;
  maxY = yMax;
  minY = yMin;
  xC = (minX+maxX) / 2.0;
  yC = (minY+maxY) / 2.0;
  double dx = maxX-minX;
  double dy = maxY-minY;
  radius = dx > dy ? dy/2.0 : dx/2.0;
}

void Image::drawScreen(int width, int height) {
  int row,col=0;
  double x,y;

  for (row=0; row<height; row++) {
    rc2xy(row,col, &x,&y);
    glRasterPos3d(x,y,0.0);
    glDrawPixels(width,1,GL_RGBA,GL_UNSIGNED_BYTE,buf[row]);
  }
}

void Image::readFile_OLD(char *filename) {
  int fileWidth,fileHeight;
  char *scanLine;
  char *src;
  byte *dst;

  ifstream file(filename);
  if (!file) {
    cerr << "Can't read from " << filename << endl;
    exit(EXIT_FAILURE);
  }
  int found=false;
  int row;
  char* line = new char[80];
  file.getline(line,78);
  line[2] = '\0';
  if (strcmp(line,"P6") != 0) {
    cerr << "Not a raw PPM image!  Magic=" << line << endl;
    exit(1);
  }
  while (!found) {
    file.getline(line,78);
    if (line[0] != '#')
      found = true;
  }
  sscanf(line,"%d %d",&fileWidth,&fileHeight);

  found = false;
  while (!found) {
    file.getline(line,78);
    if (line[0] != '#')
      found = true;
  }

  // Ok, now we've read the header.  Time to grab the bytes.

  ensureSpace(fileWidth,fileHeight);

  scanLine = new char[width*3];

  //
  // read the image file top-to-bottom, but store it bottom-to-top
  // (this simplifies the addressing)
  //
  for (row=0; row<height; row++) {
    file.read(scanLine, 3*width);
    src = scanLine;
    dst = buf[height-1-row];
    for (int col=0; col<width; col++) {
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = *src++;
      *dst++ = 255;  // 4 bytes per pixel, not 3 (rgba)
    }

    fprintf(dbgfp,"row %d: ",row);
    for (int x=0; x<width*4; x++)
      fprintf(dbgfp,"%d ",buf[height-1-row][x]);
    fprintf(dbgfp,"\n");
  }

  delete[] scanLine;
  delete[] line;
  file.close();
}

void Image::readFile(char *filename) {
  int fileWidth,fileHeight;
  byte *scanLine;
  byte *src,*dst;

  FILE *fp = fopen(filename,"rb");
  if (fp == NULL) {
    fprintf(stderr,"Can't read image file %s\n",filename);
    exit(1);
  }
  int found=false;
  int row;
  char* line = new char[80];
  fgets(line,78,fp);
  line[2] = '\0';
  if (strcmp(line,"P6") != 0) {
    fprintf(stderr,"Not a raw PPM image!  Magic=%s\n",line);
    exit(1);
  }
  while (!found) {
    fgets(line,79,fp);
    if (line[0] != '#')
      found = true;
  }
  sscanf(line,"%d %d",&fileWidth,&fileHeight);

  found = false;
  while (!found) {
    fgets(line,78,fp);
    if (line[0] != '#')
      found = true;
  }

  // Ok, now we've read the header.  Time to grab the bytes.

  ensureSpace(fileWidth,fileHeight);

  scanLine = new byte[width*3];

  //
  // read the image file top-to-bottom, but store it bottom-to-top
  // (this simplifies the addressing)
  //
  for (row=0; row<height; row++) {
        fread(scanLine,3,width,fp);
        src = scanLine;
        dst = buf[height-1-row];
        for (int col=0; col<width; col++) {
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = *src++;
          *dst++ = 255;  // 4 bytes per pixel, not 3 (rgba)
        }
  }

  delete[] scanLine;
  delete[] line;
  fclose(fp);
}

void Image::clear(int r, int g, int b) {
  int row,col;
  byte *dst;

  for (row = 0; row<height; row++) {
    dst = buf[row];
    for (col = 0; col<width; col++) {
      *dst++ = r;
      *dst++ = g;
      *dst++ = b;
      *dst++ = 255;
    }
  }
}

////////////////////////////////////////////////////
//
// Clear the alpha byte for any pixels matching the
// given rgb.
//
void Image::setTransparency(int transR, int transG, int transB) {
  int row,col;
  byte *dst;

  for (row = 0; row<height; row++) {
    dst = buf[row];
    for (col = 0; col<width; col++) {
      int r = *dst++;
      int g = *dst++;
      int b = *dst++;
      if (r == transR &&
          g == transG &&
          b == transB)
        *dst = 0;
      dst++;
    }
  }
}


///////////////////////////////////////////////////
//
// Reflect the image about the vertical axis.
///
void Image::flipHorizontal() {
  int row,col;
  byte *left,*right;
  int pl,pr;

  for (row = 0; row<height; row++) {
    left  = buf[row];
    right = buf[row] + (width-1)*4;
    for (col = width-2; col>width/2; col--) {
      pr = *right;
      pl = *left;
      *right++ = pl;
      *left++ = pr;

      pr = *right;
      pl = *left;
      *right++ = pl;
      *left++ = pr;

      pr = *right;
      pl = *left;
      *right++ = pl;
      *left++ = pr;

      pr = *right;
      pl = *left;
      *right++ = pl;
      *left++ = pr;

      right -= 8;
    }
  }
}
