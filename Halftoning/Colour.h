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

#ifndef COLOUR_H
#define COLOUR_H

//#ifdef WIN32
//#include <windows.h>
//#endif

typedef unsigned char byte;

class Colour {
public:
  Colour();
  Colour(byte r, byte g, byte b);
  Colour(int r, int g, int b);
  void set(int R, int G, int B);
  void set(float newR, float newG, float newB);
  void set(Colour &c);
  bool equals(Colour &c) {return r == c.r && g == c.g && b == c.b;};
  bool equals(int rr, int gg, int bb) {return r == rr && g == gg && b == bb;};
  inline byte getR() {return r;};
  inline byte getG() {return g;};
  inline byte getB() {return b;};
  int dist(int ar, int ag, int ab);
  bool operator==(Colour& c) { return equals(c); };

  static Colour Red() {
    Colour result;
    result.set(255,0,0);
    return result;
  }

  static Colour Green() {
    Colour result;
    result.set(0,255,0);
    return result;
  }

  static Colour Blue() {
    Colour result;
    result.set(0,0,255);
    return result;
  }

  static Colour White() {
    Colour result;
    result.set(255,255,255);
    return result;
  }

  static Colour Black() {
    Colour result;
    result.set(0,0,0);
    return result;
  }

  static Colour Gray25() {
    Colour result;
    result.set(64,64,64);
    return result;
  }

  static Colour Gray50() {
    Colour result;
    result.set(128,128,128);
    return result;
  }

  static Colour Gray75() {
    Colour result;
    result.set(192,192,192);
    return result;
  }

  static Colour Yellow() {
    Colour result;
    result.set(255,255,0);
    return result;
  }

  static Colour Indigo() {
    Colour result;
    result.set(0,255,255);
    return result;
  }

  static Colour Magenta() {
    Colour result;
    result.set(255,0,255);
    return result;
  }

  static Colour Orange() {
    Colour result;
    result.set(128,64,128);
    return result;
  }

private:
  byte r,g,b;
};


#endif
