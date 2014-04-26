/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "ipp.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern void* ipView_8u_C3R(const Ipp8u* pData, int step, IppiSize roi, 
   const char* caption, int isModalView);

IppStatus a(int), b(int);
/// to have a constructor
struct Size : IppiSize {
   Size() { width = height = 0; }
   Size( int side ) { width = height = side; }
   Size( int w, int h ) { width = w; height = h; }
   int area() { return width * height; }
   Size and( Size sz ) { return Size( IPP_MIN(width,sz.width), IPP_MIN(height,sz.height) ); }
};
/// to have a constructor
struct Rect : IppiRect {
   Rect() { x = y = width = height = 0; }
   Rect( int xx, int yy, int w, int h ) { x = xx; y = yy; width = w; height = h; }
};
/// a tile index
struct Index : IppiPoint {
   Index() { x = y = 0; }
   Index( int i ) { x = y = i; }
   Index( int xx, int yy ) { x = xx; y = yy; }
   bool inside( Size sz ) { return x<sz.width && y<sz.height && x>=0 && y>=0; }
   Index operator+(Index index) { Index i(x+index.x,y+index.y); return i; }
   /// return offset in bytes for the current tile index
   unsigned int offcenter( Size tilesz, int wstep ) { return y*tilesz.height*wstep+x*tilesz.width*3; }
};

/// a square, 3-channel, black tile
class Tile {
   Ipp8u* m_tile;
   static const int m_side;
public:
   Tile() { 
      m_tile = new Ipp8u[m_side*m_side*3];
      ippsZero_8u( m_tile, m_side*m_side*3 );
   }
   ~Tile() { delete m_tile; }
   Ipp8u* black() { return m_tile; }
   static int side() { return m_side; }
   static int wstep() { return m_side*3; }
   static Size size() { return Size(m_side); }
};
const int Tile::m_side = 32;

/// to work with tiles. Returns a black tile if there is no one in the storage
class Butler {
protected:
   static Tile m_blackTile;      /// black tile, one for all instances
   Size m_imgSz, m_imgSzPixel;   /// image size in tiles, original image size in pixels
   Ipp8u** m_storage;            /// were the tiles are stored
public:
   Butler() : m_imgSz(0), m_imgSzPixel(0), m_storage(0) {}
   Butler( int imgw, int imgh ) { init(imgw,imgh); }
   ~Butler() { deleteStorage(); }
   Size imageSize() {                       /// image size in pixels for tiles
      return Size(m_imgSz.width*Tile::side(), m_imgSz.height*Tile::side());}
   Size imageSizeTiles() { return m_imgSz; }
   static Index incIndex( Index index, Size size ) {
      index.x++;
      if( index.x >= size.width ) { index.x = 0; index.y++; }
      return index;
   }
   Index incIndex( Index index ) { return incIndex( index, m_imgSz ); }
   /// the following two virtual functions can have another implementation
   /// to provide a different method to store tiles
   virtual Ipp8u* getTilePtr( Index index ) {
      if( index.inside( m_imgSz ) )
         return m_storage[index.y * m_imgSz.width + index.x];
      return m_blackTile.black();
   }
   virtual bool putTilePtr( Ipp8u* tile, Index index ) {
      if( index.inside( m_imgSz ) ) {
         int actualindex = index.y * m_imgSz.width + index.x;
         if( m_storage[actualindex] )
            delete m_storage[actualindex];
         m_storage[actualindex] = tile;
         return true;
      }
      return false;
   }
   /// tile operation to create sample image with ipp Ramp
   bool createTiledImageRamp( int w, int h ) {
      deleteStorage();
      init( w, h );
      Index index;
      float slope = 255.0f/m_imgSzPixel.width;
      Ipp8u white[3] = {255,255,255};
      int st = ippStsNoErr;
      while( index.inside( m_imgSz ) && ippStsNoErr==st ) {
         Ipp8u* tile = new Ipp8u[Tile::size().area()*3];
         ippiSet_8u_C3R( white, tile, Tile::wstep(), Tile::size() );
         float ofs = index.x * Tile::side() * slope;
         st = ippiImageRamp_8u_C3R( tile, Tile::wstep(), sizeToProcess(index), ofs, slope, ippAxsHorizontal );
         putTilePtr( tile, index );
         index = incIndex( index );
      }  
      return ippStsNoErr == st;
   }
   /// tile operation to create sample image with non-ipp functions
   bool createTiledImageGrid( int w, int h ) {
      const Ipp8u white[3] = {255,255,255}, black[3] = {0,0,0};
      deleteStorage();
      init( w, h );
      Index index;
      int st = ippStsNoErr;
      while( index.inside( m_imgSz ) && ippStsNoErr==st ) {
         Ipp8u* tile = new Ipp8u[Tile::size().area()*3];
         st |= ippiSet_8u_C3R( white, tile, Tile::wstep(), Tile::size() );
         if( index.x>0 || index.y>0 ) 
            st |= ippiSet_8u_C3R( black, tile, Tile::wstep(), 
               Size(Tile::side()-index.x,Tile::side()-index.y).and(sizeToProcess(index)) );
         putTilePtr( tile, index );
         index = incIndex( index );
      }
      return ippStsNoErr == st;
   }
   /// create a solid image to draw and show, user is responsible for freeing
   Ipp8u* makeSolidImage( int* wstep ) {
      Ipp8u* img = ippiMalloc_8u_C3( Tile::side()*m_imgSz.width, Tile::side()*m_imgSz.height, wstep );
      Index index;
      while( index.inside( m_imgSz ) ) {
         ippiCopy_8u_C3R( getTilePtr( index ), Tile::wstep(),
            img+index.offcenter( Tile::size(), *wstep ), *wstep, Tile::size() );
         index = incIndex( index );
      }
      return img;
   }
   /// tile at the edge could have less size to process
   /// even we have a full size tile in the tile storage
   Size sizeToProcess( Index index ) {
      Size sz = Tile::size();
      if( index.x == m_imgSz.width-1 && index.x > 0 )
         sz.width = m_imgSzPixel.width - index.x * Tile::side();
      if( index.y == m_imgSz.height-1 && index.y > 0 )
         sz.height = m_imgSzPixel.height - index.y * Tile::side();
      return sz;
   }
   Rect rectOfTile( Index index ) {
      return Rect( index.x*Tile::side(), index.y*Tile::side(), Tile::side(), Tile::side() );
   }
   /// num of tiles which contain the rectangle
   Size howManyTiles( double bound[2][2]) {
      return Size ( (int)ceil((bound[1][0]-bound[0][0]+Tile::side()-1) / Tile::side()),
         (int)ceil((bound[1][1]-bound[0][1]+Tile::side()-1) / Tile::side()) );
   }
   /// create rectangle, with a border if it is needed
   Rect boundToRectBorder(double bound[2][2], int xy, int isz ) {
      Rect r;
      bound[0][0] -= xy;
      bound[0][1] -= xy;
      bound[1][0] += isz;
      bound[1][1] += isz;
      r.x = IPP_MAX( (int)(bound[0][0]), 0 );
      r.y = IPP_MAX( (int)(bound[0][1]), 0 );
      r.width = (int)(bound[1][0] - r.x + 1);
      if( r.width+r.x > m_imgSzPixel.width ) r.width = m_imgSzPixel.width - r.x;
      r.height = (int)(bound[1][1] - r.y + 1);
      if( r.height+r.y > m_imgSzPixel.height ) r.height = m_imgSzPixel.height - r.y;
      return r;
   }
   /// make a rectangle represented by two points
   /// the function has another implementation in derived butler for rotate op
   virtual bool rectToBound( Rect r, double bound[2][2], double, double, double ) {
      bound[0][0] = r.x; bound[1][0] = r.x+r.width;
      bound[0][1] = r.y; bound[1][1] = r.y+r.height;
      return true;
   }
   /// create an area of pixels that have to be processed to get dst tile   
   virtual Ipp8u* copyTiles( Index dstTile, Size nofTiles, int* wstep ) {
      Ipp8u *srcTile = 0;
      Ipp8u *srcTiles = ippiMalloc_8u_C3( nofTiles.width*Tile::side(), 
         nofTiles.height*Tile::side(), wstep );
      Index srcTileIndex;
      while( srcTileIndex.inside( nofTiles ) ) {
         /// black tile is returned if there is no tile in the butler
         srcTile = getTilePtr( srcTileIndex + dstTile + Index(-1) );
         ippiCopy_8u_C3R( srcTile, Tile::side()*3, /// ignore bad status for null ptr
            srcTiles+srcTileIndex.offcenter(Tile::size(),*wstep), *wstep, Tile::size() );
         srcTileIndex = incIndex( srcTileIndex, nofTiles );
      }
      return srcTiles;
   }
   /// show image using special function ipView from the tool
   void draw( const char* title, int modal ) {
      int wstep;
      Ipp8u* img = makeSolidImage( &wstep);
      ippiMirror_8u_C3IR( img, wstep, imageSize(), ippAxsHorizontal );
      ipView_8u_C3R( img, wstep, imageSize(), title, modal );
      ippiFree( img );
   }
   /// the storage is an array of the pointers to tiles
private:
   void deleteStorage() {
      if( m_storage ) {
         for( int n=0; n<(int)(m_imgSz.area()); n++ )
            if( m_storage[n] && m_storage[n]!=m_blackTile.black() ) delete m_storage[n];
         delete m_storage;
         m_storage = 0;
      }
   }
   void init( int imgw, int imgh ) {
      m_imgSzPixel = Size( imgw, imgh );
      m_imgSz = Size( (imgw+Tile::side()-1)/Tile::side(), (imgh+Tile::side()-1)/Tile::side() );
      m_storage = new Ipp8u* [m_imgSz.area()*sizeof(Ipp8u*)/4];
      ippsSet_32s( 0, (Ipp32s*)m_storage, m_imgSz.area()*sizeof(Ipp8u*)/4 );
   }
};

Tile Butler::m_blackTile=Tile();

/// derived class to return borders as reflected data instead of black data
/// could be used in the filtering operations
class ButlerReflect : public Butler {
public:
   ButlerReflect() : Butler() {}
   ButlerReflect( int w, int h ) : Butler(w,h) {}
   /// create a number of tiles surrounding the source tile
   /// not existed tile is created as one with reflected data
   /// with using the mirror function for a tile at the edge
   virtual Ipp8u* copyTiles( Index dstTile, Size nofTiles, int* wstep ) {
      Ipp8u *srcTile;
      Ipp8u* srcTiles = ippiMalloc_8u_C3( nofTiles.width*Tile::side(), 
         nofTiles.height*Tile::side(), wstep );
      Index srcTileIndex;
      while( srcTileIndex.inside( nofTiles ) ) {
         Index sumIndex = srcTileIndex + dstTile, index = dstTile;
         IppiAxis flip = ippAxsBoth;
         if( sumIndex.x <= m_imgSz.width && sumIndex.y <= m_imgSz.height ) {
            /// the tile is not at the right or bottom edge
            if( sumIndex.x > 0 && sumIndex.y > 0 ) /// inside
               srcTile = getTilePtr( sumIndex + Index(-1) );
            else {                          /// at the left or top edge
               srcTile = new Ipp8u[Tile::size().area()*3];
               if( sumIndex.x==0 ) {
                  index.y = sumIndex.y > 0 ? sumIndex.y-1 : 0;
                  flip = ippAxsVertical;
               } else if( sumIndex.y==0 ) {
                  index.x = sumIndex.x > 0 ? sumIndex.x-1 : 0;
                  flip = ippAxsHorizontal;
               }
               ippiMirror_8u_C3R( getTilePtr(index), Tile::wstep(), srcTile, Tile::wstep(), 
                  Tile::size(), flip );
            }
         } else {                           /// at the right or bottom edge
            srcTile = new Ipp8u[Tile::size().area()*3];
            Index index(-2);
            if( sumIndex.x != sumIndex.y ) {
               if( sumIndex.x > m_imgSz.width ) {
                  flip = ippAxsVertical;
                  index.y = -1;
               } else {
                  flip = ippAxsHorizontal;
                  index.x = -1;
               }
            }
            ippiMirror_8u_C3R( getTilePtr(sumIndex+index), Tile::wstep(), srcTile, 
               Tile::wstep(), Tile::size(), flip );
         }
         /// copy the tile to the source which is an area surrounding the source
         ippiCopy_8u_C3R( srcTile, Tile::wstep(), 
            srcTiles+srcTileIndex.offcenter(Tile::size(),*wstep), *wstep, Tile::size() );
         srcTileIndex = incIndex( srcTileIndex, nofTiles );
      }
      return srcTiles;
   }
};

class ButlerRotate : public Butler {
public:
   ButlerRotate() : Butler() {}
   ButlerRotate( int w, int h ) : Butler(w,h) {}
   virtual bool rectToBound( Rect r, double bound[2][2], double angle, double xshift, double yshift ) {
      return ippStsNoErr == ippiGetRotateBound( r, bound, angle, xshift, yshift );
   }
   virtual Ipp8u* copyTiles( Index startTile, Size nofTiles, int* wstep ) {
      Size sz( nofTiles.width*Tile::side(), nofTiles.height*Tile::side() );
      Ipp8u* srcTiles = ippiMalloc_8u_C3( sz.width, sz.height, wstep );
      Index srcTileIndex;
      while( srcTileIndex.inside( nofTiles ) ) {
         Ipp8u* srcTile = getTilePtr( srcTileIndex + startTile );
         ippiCopy_8u_C3R( srcTile, Tile::wstep(), 
            srcTiles+srcTileIndex.offcenter(Tile::size(),*wstep), *wstep, Tile::size() );
         srcTileIndex = incIndex( srcTileIndex, nofTiles );
      }
      return srcTiles;
   }
};

class ButlerResize : public Butler {
public:
   ButlerResize() : Butler() {}
   ButlerResize( int w, int h ) : Butler(w,h) {}
   virtual bool rectToBound( Rect r, double bound[2][2], double xFr, double yFr, double xShift, double yShift)
   {
      bound[0][0] = (r.x - xShift) * xFr;
      bound[0][1] = (r.y - yShift) * yFr;
      bound[1][0] = (r.x - xShift + r.width) * xFr;
      bound[1][1] = (r.y - yShift + r.height) * yFr;
      return 1;
   }
   virtual Ipp8u* copyTiles( Index startTile, Size nofTiles, int* wstep )
   {
      Size sz( nofTiles.width*Tile::side(), nofTiles.height*Tile::side() );
      Ipp8u* srcTiles = ippiMalloc_8u_C3( sz.width, sz.height, wstep );
      Index srcTileIndex;
      while( srcTileIndex.inside( nofTiles ) ) {
         Ipp8u* srcTile = getTilePtr( srcTileIndex + startTile );
         ippiCopy_8u_C3R( srcTile, Tile::wstep(), 
            srcTiles+srcTileIndex.offcenter(Tile::size(),*wstep), *wstep, Tile::size() );
         srcTileIndex = incIndex( srcTileIndex, nofTiles );
      }
      return srcTiles;
   }
};

/// check tile processing with a simple function which doesn't require border
/// base butler class is used
int t_xorc( int w = 300, int h = 200 ) {
   Butler srcButler;
   srcButler.createTiledImageRamp( w, h );
   Butler dstButler( w, h );
   Ipp8u val[3] = { 255, 255, 0 };
   Index index;
   while( index.inside( dstButler.imageSizeTiles() ) ) {
      Ipp8u* srcTile = srcButler.getTilePtr( index );
      Ipp8u* dstTile = new Ipp8u[Tile::size().area()*3];
      ippiXorC_8u_C3R( srcTile, Tile::wstep(), val, dstTile, Tile::wstep(), dstButler.sizeToProcess(index) );
      dstButler.putTilePtr( dstTile, index );
      index = dstButler.incIndex( index );
   }
   srcButler.draw( " original", 0 );
   dstButler.draw( " xored", 1 );
   return 0;
}

/// an example with the rotate function, derived butler class is used
int t_rotate( double angle, int interpolation, int w = 320, int h = 256 ) {
    double bound[2][2];
    double xshift, yshift, xshiftback, yshiftback;
    Size imgSz( w, h );
    ButlerRotate srcButler;
    srcButler.createTiledImageGrid(w,h);
    // for Visualization of the Tile Boundaries (commented below)
    //srcButler.createTiledImageRamp(w,h);
    ButlerRotate dstButler( w, h );
    /// shift values for the forward and inverse transform
    ippiGetRotateShift( 0.5*w, 0.5*h, angle, &xshift, &yshift );
    ippiGetRotateShift( 0.5*w, 0.5*h, -angle, &xshiftback, &yshiftback );

    Index dstTileIndex;
    IppStatus st = ippStsNoErr;
    while( dstTileIndex.inside( dstButler.imageSizeTiles() ) && (ippStsNoErr==st || ippStsWrongIntersectQuad==st)) {
        int wstep;
        Rect tileRect = dstButler.rectOfTile( dstTileIndex );
        srcButler.rectToBound( tileRect, bound, -angle, xshiftback, yshiftback );
        Rect srcRect = srcButler.boundToRectBorder( bound, 3, 6 );
        Index startTile( srcRect.x/Tile::side(), srcRect.y/Tile::side() );
        /// create an area of points (tiles) needed to get the destination tile
        Ipp8u* srcTiles = srcButler.copyTiles( startTile, srcButler.howManyTiles(bound), &wstep );
        Ipp8u* dstTile = new Ipp8u[Tile::size().area()*3];
        ippsZero_8u( dstTile, Tile::size().area()*3 );
        // for Visualization of the Tile Boundaries (commented below)
        //tileRect.width--;
        //tileRect.height--;
        st = ippiRotate_8u_C3R( srcTiles-startTile.offcenter(Tile::size(),wstep), 
            imgSz, wstep, srcRect,
            dstTile-dstTileIndex.offcenter(Tile::size(),Tile::wstep()), 
            Tile::wstep(), tileRect, 
            angle, xshift, yshift, interpolation 
            );
        dstButler.putTilePtr( dstTile, dstTileIndex );
        dstTileIndex = dstButler.incIndex( dstTileIndex );
        ippiFree( srcTiles );
    }
    srcButler.draw(" original", 0 );
    dstButler.draw( " rotated", 1 );
    return st;
}

/// an example with function which needs a border to process points at the edge
int t_blur( Butler& srcButler, int masksz=3, int w=320, int h=256 ) {
   double bound[2][2];
   Size maskSz(masksz);
   srcButler.createTiledImageGrid( w, h );
   Butler dstButler( w, h );
   Index anchor(masksz/2), dstTileIndex;
   IppStatus st = ippStsNoErr;
   while( dstTileIndex.inside(dstButler.imageSizeTiles()) && ippStsNoErr==st ) {
      int wstep;
      bound[0][0] = dstTileIndex.x * Tile::side() - (masksz - anchor.x);
      bound[0][1] = dstTileIndex.y * Tile::side() - (masksz - anchor.y);
      bound[1][0] = bound[0][0] + Tile::side() + masksz -1;
      bound[1][1] = bound[0][1] + Tile::side() + masksz -1;
      Ipp8u* srcTiles = srcButler.copyTiles( dstTileIndex, srcButler.howManyTiles(bound), &wstep );
      Ipp8u* dstTile = new Ipp8u[Tile::size().area()*3];
      st = ippiFilterBox_8u_C3R( srcTiles+Index(1).offcenter(Tile::size(),wstep), wstep, 
         dstTile, Tile::wstep(), dstButler.sizeToProcess(dstTileIndex), maskSz, anchor 
      );
      dstButler.putTilePtr( dstTile, dstTileIndex );
      dstTileIndex = dstButler.incIndex( dstTileIndex );
      ippiFree( srcTiles );
   }
   srcButler.draw( " original", 0 );
   dstButler.draw( " filtered", 1 );
   return st;
}

/// an example with the function ippiResizeSqrPixel, derived butler class is used
int t_resize (
    double xFactor, double yFactor, double xShift, double yShift,
    int interpolation, int w = 256, int h = 256)
{
    double bound[2][2];
    double xFr = 1./xFactor, yFr = 1./yFactor;
    Size imgSz(w, h);
    Rect srcRect(0, 0, w, h);
    Rect dstRect(0, 0, Tile::size().width, Tile::size().height);
    ButlerResize srcButler;
    srcButler.createTiledImageGrid(w, h);
    ButlerResize dstButler(w, h);
    Index dstTileIndex;
    IppStatus st = ippStsNoErr;

    /// calculation of work buffer size (for single tile!)
    int sizeBuffer;
    Ipp8u* pWorkBuffer;
    st = ippiResizeGetBufSize(srcRect, dstRect, 3, interpolation, &sizeBuffer);
    pWorkBuffer = ippsMalloc_8u(sizeBuffer);
    if (NULL == pWorkBuffer) return ippStsMemAllocErr;

    while (dstTileIndex.inside(dstButler.imageSizeTiles()))
    {
        int wstep;
        Rect tileRect = dstButler.rectOfTile(dstTileIndex);
        srcButler.rectToBound(tileRect, bound, xFr, yFr, xShift, yShift);
        Rect srcRect = srcButler.boundToRectBorder(bound, 0, 0);
        Index startTile(srcRect.x/Tile::side(), srcRect.y/Tile::side());
        /// create an area of points (tiles) needed to get the destination tile
        Ipp8u* srcTiles = srcButler.copyTiles(startTile, srcButler.howManyTiles(bound), &wstep);
        Ipp8u* dstTile = new Ipp8u[Tile::size().area()*3];
        ippsZero_8u(dstTile, Tile::size().area()*3);
        ippiResizeSqrPixel_8u_C3R (
            srcTiles-startTile.offcenter(Tile::size(),wstep), imgSz, wstep, srcRect,
            dstTile-dstTileIndex.offcenter(Tile::size(),Tile::wstep()), Tile::wstep(), tileRect, 
            xFactor, yFactor, xShift, yShift, interpolation, pWorkBuffer);
        dstButler.putTilePtr(dstTile, dstTileIndex);
        dstTileIndex = dstButler.incIndex(dstTileIndex);
        ippiFree(srcTiles);
    }
    srcButler.draw(" original image", 0);
    dstButler.draw(" resized image ", 1);
    return st;
}

/* examples of command line to run tile processing demo
   tiler.exe xor
   tiler.exe blur
   tiler.exe blurref
   tiler.exe rot
   tiler.exe resize
   tiler.exe
*/
int main (int argc, char** argv)
{
   const int w = 256, h = 256;
   if (argc > 1) {
      if (strstr(argv[1], "xor"))
          return t_xorc();
      else if (strstr(argv[1], "blur"))
          return t_blur(Butler(), 7, w, h);
      else if (strstr(argv[1], "blurref"))
          return t_blur(ButlerReflect(), 7, w, h);
      else if (strstr(argv[1], "rot"))
          return t_rotate(3.5, IPPI_INTER_LINEAR|IPPI_SMOOTH_EDGE, w, h);
      else if (strstr(argv[1], "resize"))
          return t_resize(0.7, 1.1, 15.5, 4.2, IPPI_INTER_LANCZOS, w, h);
   }
   return t_resize(0.7, 1.1, 15.5, 4.2, IPPI_INTER_LANCZOS, w, h);
}
