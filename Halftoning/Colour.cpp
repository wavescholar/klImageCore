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
// A Simple rgb color triplet.
//
// Author: Alejo Hausner (ahausner@truehaus.net) 2008.

#include "Colour.h"

/*
static void Colour::setDefaultColours() {
  Colour::white  .set(255,255,255);
  Colour::black  .set(  0,  0,  0);
  Colour::red    .set(128,  0,  0);
  Colour::yellow .set(128,128,  0);
  Colour::green  .set(0,  128,  0);
  Colour::indigo .set(0,  128,128);
  Colour::blue   .set(0,    0,128);
  Colour::magenta.set(128,  0,128);
  Colour::orange .set(128, 64,128);
}
*/

Colour::Colour() {
//  cout << "Colour default constructor." << endl;
  Colour(255,255,255);
}

int Colour::dist(int ar, int ag, int ab) {
  int dr = r - ar;
  int dg = g - ag;
  int db = b - ab;

  dr = dr > 0 ? dr : -dr;
  dg = dg > 0 ? dg : -dg;
  db = db > 0 ? db : -db;

  return dr + dg + db;
}

Colour::Colour(int r, int g, int b) {
//  cout << "Colour explicit constructor. (r g b)=" << r << " " << g << " " << b << ")" << endl;

  this->r = r;
  this->g = g;
  this->b = b;
}

void Colour::set(int newR, int newG, int newB) {
  r = newR;
  g = newG;
  b = newB;
}

void Colour::set(float newR, float newG, float newB) {
  r = (byte)(newR*255.0);
  g = (byte)(newG*255.0);
  b = (byte)(newB*255.0);
}

void Colour::set(Colour &c) {
  r = c.getR();
  g = c.getG();
  b = c.getB();
}


