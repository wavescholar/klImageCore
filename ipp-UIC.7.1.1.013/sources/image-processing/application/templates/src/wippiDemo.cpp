/* ////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//                 Intel(R) Integrated Performance Primitives
//                    C++ template based wrappers for IPPi
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  SYNOPSIS
//      Contains demo-calls of template wrappers covering most of IPPi.
//
//  DESCRIPTION
//      Serves to demonstrate capacity of using IPPi through templates wrapper as a type of
//      high level interface for IPP.
//      Call examples are given for each wrapper. Full list of template names see in wippi.hpp
//      All calls are grouped according to 'ippiman.pdf' chapters.
//      Calls themselves have no any calculating sense.
//
//  UPDATE for IPP 5.1 Samples
//      109 new demo-calls were added (in addition to the existing 2475 ones) regarding last IPPi updating:
//      1. Dup (1)
//      2. DeconvFFT[InitAlloc|Free] (6)
//      3. DeconvLR[InitAlloc|Free] (6)
//      4. not-in-place MulPackConj (4)
//      5. in-place [DFT|FFT][Fwd_RToPack|Inv_PackToR] for 32f (20)
//      6. in-place [DFT|FFT][Fwd|Inv]_CToC for 32fc C1 (6)
//      7. 16u in WarpAffine/WarpAffineBack/WarpAffineQuad (pixel/planar-order data) (18)
//      8. 16u in Add/Sub/Mul (NI/I) (48)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "wippi.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>


#define M  32    // img width  by default
#define N  48    // img height by default
#define MxN  M,N

#define L  4     // max channel num

#define ICH1  0
#define ICH2  1
#define ICH3  2
#define ICH4  3

#define NLEVELS_LUT  5
#define NLEVELS     25

#define THRESHOLD(dt)    value_##dt
#define MAX(dt)          value_##dt
#define MIN(dt)          value_##dt
#define MEAN(dt)         value_##dt
#define LOWER_BOUND(dt)  value_##dt
#define UPPER_BOUND(dt)  value_##dt

#define LOWER_LEVEL  value1_32s
#define UPPER_LEVEL  value2_32s
#define X            value1_32s
#define Y            value2_32s
#define COUNTS       value2_32s
#define X1           val1_32s
#define Y1           val2_32s

#define VAL_32S      value1_32s[3]
#define PVAL(dt)    (Ipp##dt*)value1_32s

#define scale_8u   4  // 8u  in 32s
#define scale_16s  2  // 16s in 32s
#define scale_32s  1
#define scale_32f  1
#define LAST(N,dt) L*scale_##dt-N  // last N elements

#define MASK  (Ipp8u*)pKern_32f

#define DATA_IMG( tp )       \
    GIMG<Ipp##tp>            \
        src1_##tp##_C1(MxN), \
        src2_##tp##_C1(MxN), \
        src_##tp##_C1(MxN),  \
        img_##tp##_C1(MxN),  \
        dst_##tp##_C1(MxN);  \
                             \
    IMG3<Ipp##tp>            \
        src1_##tp##_C3(MxN), \
        src2_##tp##_C3(MxN), \
        src_##tp##_C3(MxN),  \
        img_##tp##_C3(MxN),  \
        dst_##tp##_C3(MxN);  \
                             \
    AIMG<Ipp##tp>            \
        src1_##tp##_CA(MxN), \
        src2_##tp##_CA(MxN), \
        src_##tp##_CA(MxN),  \
        img_##tp##_CA(MxN),  \
        dst_##tp##_CA(MxN);  \
                             \
    IMG4<Ipp##tp>            \
        src1_##tp##_C4(MxN), \
        src2_##tp##_C4(MxN), \
        src_##tp##_C4(MxN),  \
        img_##tp##_C4(MxN),  \
        dst_##tp##_C4(MxN)


 // for filtering functions:

class imgKernel {
public:
  IppiSize
        size,    // mask size
        border;  // full extra pixels using to resize dst image
        IppiPoint anchor;
  int
        addL, addT, addR, addB; // shifts (from left, top... of src edges) to get fitting subimage according to size and anchor

        void init( int w, int h );
        void setAnchor( int x, int y );
};

class Convolution {
public:
    IppiSize
        srcSize[2], // srcSize.x[0] X srcSize.y[0] is the size of src1 ; srcSize[1] - the same for src2
        dstSize[2]; // [0] - for full conv. / [1] - for valid conv

    Convolution();
};

void imgKernel::init(int w, int h ){

    size.width  =w;
    size.height =h;

    border.width  = size.width-1;
    border.height = size.height-1;

    setAnchor( size.width/2, size.height/2 ); /* by default */
}

void imgKernel::setAnchor( int x, int y ){

    anchor.x = (x >= size.width)  ? size.width-1  : x;
    anchor.y = (y >= size.height) ? size.height-1 : y;

    addL = anchor.x;
    addT = anchor.y;
    addR = anchor.x-border.width;
    addB = anchor.y-border.height;
}

Convolution::Convolution(){

    srcSize[0].width =M/4*3;
    srcSize[1].width =M+1-srcSize[0].width;

    srcSize[0].height =N/2;
    srcSize[1].height =N/4;

    dstSize[0].width  =M;
    dstSize[0].height =srcSize[0].height + srcSize[1].height -1;

    dstSize[1].width  =srcSize[0].width  - srcSize[1].width  +1;
    dstSize[1].height =srcSize[0].height - srcSize[1].height +1;
}


  class libIppVer{
  private:
    const IppLibraryVersion* lib;
    const char *name;
    const char *ver;
    const char *bdate;
    int getLen( const char *str ) { return (int)strlen( str ); }
  public:
    libIppVer() : lib(ippiGetLibVersion()) { name=lib->Name; ver=lib->Version; bdate=lib->BuildDate; }
    const char *getName() {return name;}
    const char *getVersion() {return ver;}
    const char *getBuildDate() {return bdate;}
    void pr();
  };

  void libIppVer::pr(){
    int len = getLen( name ) + getLen( ver ) + getLen( bdate ) + 2;
    int i;

    printf( "\n  <%s %s %s> \n   ", name, ver, bdate );

    for( i=0; i<len; i++ )
       putchar( '-' );

    puts( "\n" );
  }

  typedef unsigned short  ushort;


// ____________________________________________________________________________________________________________
//

int main(){
//
// Concerns only those functions which flavours are declared in ippi.h file:

//  _CA == _AC4

// Common data:

    DATA_IMG(8u);
    DATA_IMG(8s);
    DATA_IMG(16u);
    DATA_IMG(16s);
    DATA_IMG(16sc);
    DATA_IMG(32s);
    DATA_IMG(32sc);

    DATA_IMG(32f);
    DATA_IMG(32fc);

    IppStatus stat =ippStsNoOperation;

    int
        scale =7,
        size, *pSize=new Ipp32s[10],
       *pBuffer;

    Ipp32fc val_32fc[] = { 0., 0., 85., 85., 170., 170., 255., 255. };
    Ipp64f  val_64f = 3.3333e+0;

    IppiSize roiSize = { (M>>1) - 1, (N>>1) - 1 };


// Additional for data exchange and initialization functions and compare operations:

    IMG4<Ipp8u> mask_8u_C4(MxN);

    int
        lftBorderW = M/4,
        topBorderW = N/4,

        w = (M > N ? N : M),
        h = w/4*3,

        dstOrder[] = { 0,1,2, 2,1,0, 1,2,0, 1,1,1 };

    unsigned int  seed = 372;


// Additional data for LUT:
    int i, j;

    Ipp32s
        nLevels_LUT[L] = { NLEVELS_LUT, NLEVELS_LUT, NLEVELS_LUT, NLEVELS_LUT },
       *pLevels[L] = { new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT] },
       *pValues[L] = { new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT], new Ipp32s[NLEVELS_LUT] };

    int nBitSize =8;
    Ipp32u *pTable = new Ipp32u[(Ipp32u)(1<<nBitSize)];


// Additional data for threshold and compare operations:

    IppCmpOp
        cmpGT = ippCmpGreater,
        cmpLT = ippCmpLess;

// Additional data for filtering and morphological functions:

    imgKernel kern;
    Convolution conv;

    Ipp32f
       *pKern_32f,
        noise[L] = { 0.05f, .1f, .5f, .9f };


// Special or additional data for DCT/DFT/FFT/DWT/DeconvFFT:

    IppiDCTFwdSpec_32f  *pDCTFwdSpec_32f[1], *pDCTSpec_32f[1];

    IppiDFTSpec_C_32fc  *pDFTSpec_32fc_useR_32s[1], *pDFTSpec_32fc[1];
    IppiFFTSpec_R_32s   *pFFTSpec_32s[1], *pFFTSpec_32s_useR_32f[1], *pFFTSpec_32s_useC_32fc[1];

//
    IppiDeconvFFTState_32f_C3R   *pDeconvFFTState_32f_C3R[1];
    IppiSize maxroi = { M+1, N+1 };
//

    GIMG<Ipp32f> aux_32f_C1(MxN), aux1_32f_C1(MxN), aux2_32f_C1(M,N);
    IMG3<Ipp32f> aux_32f_C3(M,N), aux1_32f_C3(M,N), aux2_32f_C3(M,N);
    AIMG<Ipp32f> aux_32f_CA(M,N), aux1_32f_CA(M,N), aux2_32f_CA(M,N);
    IMG4<Ipp32f> aux_32f_C4(M,N), aux1_32f_C4(M,N), aux2_32f_C4(M,N);


// Additional data for statistics:

    IppiMomentState_64s  *pMomentState_64s[1];
    IppiMomentState_64f  *pMomentState_64f[1];

    Ipp8u   value_8u[L]   = { IPP_MIN_8U, IPP_MIN_8U, IPP_MIN_8U, 64 };
    Ipp16s  value_16s[L]  = { IPP_MAX_8U, IPP_MAX_8U, IPP_MAX_8U, 128 };
    Ipp32s  value1_32s[L] = { IPP_MIN_8U, IPP_MIN_8U, IPP_MIN_8U, IPP_MIN_8U }, val1_32s;
    Ipp32s  value2_32s[L] = { IPP_MAX_8U, IPP_MAX_8U, IPP_MAX_8U, IPP_MAX_8U }, val2_32s;
    Ipp32f  value_32f[L]  = { IPP_MIN_32S, IPP_MIN_32S, IPP_MIN_32S, IPP_MIN_32S };
    Ipp64f  value_64f[L]  = { IPP_MAX_32S, IPP_MAX_32S, IPP_MAX_32S, IPP_MAX_32S }, norm;

    const Ipp32f
       *pLevels_32f[L] = { new Ipp32f[NLEVELS], new Ipp32f[NLEVELS], new Ipp32f[NLEVELS], new Ipp32f[NLEVELS] };

    Ipp32s
        nLevels[L] = { NLEVELS, NLEVELS, NLEVELS, NLEVELS },
       *pHist[L]   = { new Ipp32s[NLEVELS-1], new Ipp32s[NLEVELS-1], new Ipp32s[NLEVELS-1], new Ipp32s[NLEVELS-1] };

    IppiSize
        srcNorm = { 6, 12 },
        tplNorm = { 3,  5 };


// Additional data for geometric transformation:

    IppiRect
        srcRect = { 2, 1, 5, 4 },
        dstRect = { 0, 0, 3, 3 };

    Ipp64f
        coeffs_2x3[2][3] = {0},
        coeffs_2x4[2][4] = {0},
        coeffs_3x3[3][3] = {0},
        srcQuad[4][2] = {0},
        dstQuad[4][2] = {0};


// For images with data in planar format:

    IMG3P<Ipp8u > src_8u_P3 (MxN), dst_8u_P3 (MxN);
    IMG4P<Ipp8u > src_8u_P4 (MxN), dst_8u_P4 (MxN);
    AIMGP<Ipp8u > src_8u_PA (MxN), dst_8u_PA (MxN);
    IMG3P<Ipp16u> src_16u_P3(MxN), dst_16u_P3(MxN);
    IMG4P<Ipp16u> src_16u_P4(MxN), dst_16u_P4(MxN);
    AIMGP<Ipp16u> src_16u_PA(MxN), dst_16u_PA(MxN);
    IMG3P<Ipp32f> src_32f_P3(MxN), dst_32f_P3(MxN);
    IMG4P<Ipp32f> src_32f_P4(MxN), dst_32f_P4(MxN);

// Additional data Alpha Composition section
    AIMGP<Ipp8u > src1_8u_PA (MxN), src2_8u_PA (MxN), img_8u_PA (MxN);
    AIMGP<Ipp16u> src1_16u_PA(MxN), src2_16u_PA(MxN), img_16u_PA(MxN);

// Additional data for Copy:
    IMG3P<Ipp32s> dst_32s_P3(MxN);
    IMG4P<Ipp32s> dst_32s_P4(MxN);


// Additional data for demo:
    int io =0;
    libIppVer ippVer;

    ippVer.pr();


/* #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4
#4                           _     _  ___   _       ___ _  _   ___  _  _   _  _  _  ___  ___                         #4
#4                          / \  /_ /  /  /_ /     /_    |/   /     /_ / /_ / /| / /    /_                           #4
#4                         /_ / /  /  /  /  /     /__  _/|_  /__  _/  / /  / / |/ /__- /__                           #4
#4                                                     _  _  _  _                                                    #4
#4                                                   /_ / /| / / \                                                   #4
#4                                                  /  / / |/ /_ /                                                   #4
#4      _  _  _  _  ___  _   _  _    _ ___    _  ___  _   _  _  _       ___ _  _ _  _  ___ ___ _   _  _  _  ___      #4
#4      /  /| /  /   /   / /_ / /    /   /  /_ /  /   / /  / /| /      /_   /  / /| / /     /  / /  / /| / /_        #4
#4    _/ _/ |/ _/   /  _/ /  / /__ _/  /__ /  /  /  _/  \_/ / |/      /     \_/ / |/ /__   / _/  \_/ / |/ ___/       #4
#4                                                                                                                   #4
#4                           < Image Data Exchange and Initialization Functions (#4) >                               #4
#4                                                                                                                   #4
#4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 #4 */

// ---- ADD_RAND_GAUSS__DIRECT:

    stat = AddRandGauss_Direct( img_8u_C1 ,         IPP_MIN_8U,                   IPP_MAX_8U/2, &seed );  io++;
    stat = AddRandGauss_Direct( img_8u_C3 ,       IPP_MAX_8U/2,                    (Ipp32s)10., &seed );  io++;
    stat = AddRandGauss_Direct( img_8u_CA ,                 10,                    (Ipp32s)10., &seed );  io++;
    stat = AddRandGauss_Direct( img_8u_C4 ,       IPP_MAX_8U/2,                   IPP_MAX_8U/4, &seed );  io++;

    stat = AddRandGauss_Direct( img_16s_C1,        IPP_MIN_16S,                  IPP_MAX_16U/2, &seed );  io++;
    stat = AddRandGauss_Direct( img_16s_C3,                  0,                            117, &seed );  io++;
    stat = AddRandGauss_Direct( img_16s_CA,               -100,                   (Ipp32s)100., &seed );  io++;
    stat = AddRandGauss_Direct( img_16s_C4,         IPP_MIN_8U,                  IPP_MAX_16U/4, &seed );  io++;

    stat = AddRandGauss_Direct( img_32f_C1,     IPP_MINABS_32F,                           3.7f, &seed );  io++;
    stat = AddRandGauss_Direct( img_32f_C3,                0.f,                        56.378f, &seed );  io++;
    stat = AddRandGauss_Direct( img_32f_CA,               -.1f,                            1.f, &seed );  io++;
    stat = AddRandGauss_Direct( img_32f_C4,             -52.7f, (Ipp32f)(IPP_MAXABS_32F*10e-4), &seed );  io++;


// ---- ADD_RAND_UNIFORM__DIRECT:

    stat = AddRandUniform_Direct( img_8u_C1 ,         IPP_MIN_8U,          IPP_MAX_8U, &seed );  io++;
    stat = AddRandUniform_Direct( img_8u_C3 ,           (Ipp8u)1,          (Ipp8u)10., &seed );  io++;
    stat = AddRandUniform_Direct( img_8u_CA ,                -10,         (Ipp32s)10., &seed );  io++;
    stat = AddRandUniform_Direct( img_8u_C4 ,         IPP_MIN_8U,        IPP_MIN_8U/2, &seed );  io++;

    stat = AddRandUniform_Direct( img_16s_C1,         IPP_MIN_8S,         IPP_MAX_16S, &seed );  io++;
    stat = AddRandUniform_Direct( img_16s_C3,      IPP_MIN_16S/2,               0xfff, &seed );  io++;
    stat = AddRandUniform_Direct( img_16s_CA,               -100,        (Ipp32s)100., &seed );  io++;
    stat = AddRandUniform_Direct( img_16s_C4,         IPP_MIN_8S,          IPP_MIN_8U, &seed );  io++;

    stat = AddRandUniform_Direct( img_32f_C1, (Ipp32f)IPP_MIN_8S, (Ipp32f)IPP_MAX_16S, &seed );  io++;
    stat = AddRandUniform_Direct( img_32f_C3,    IPP_MIN_16S/2.f,       (Ipp32f)0xfff, &seed );  io++;
    stat = AddRandUniform_Direct( img_32f_CA,               -.1f,                 1.f, &seed );  io++;
    stat = AddRandUniform_Direct( img_32f_C4,     IPP_MINABS_32F,      IPP_MAXABS_32F, &seed );  io++;


// ---- CONVERT:

    Convert( src_8u_C1 , 7, dst_8u_C1 );    /* 1u8u_C1R */  io++;
    Convert( src_8u_C1 , dst_8u_C1, 0, 1 ); /* 8u1u_C1R */  io++;

    Convert( src_8u_C1 , *(GIMG<Ipp16u>*)&dst_16s_C1 );  io++;
    Convert( src_8u_C3 , dst_16u_C3 );  io++;
    Convert( src_8u_CA , dst_16u_CA );  io++;
    Convert( src_8u_C4 , dst_16u_C4 );  io++;

    Convert( src_8u_C1 , dst_16s_C1 );  io++;
    Convert( src_8u_C3 , dst_16s_C3 );  io++;
    Convert( src_8u_CA , dst_16s_CA );  io++;
    Convert( src_8u_C4 , dst_16s_C4 );  io++;

    Convert( src_8u_C1 , dst_32s_C1 );  io++;
    Convert( src_8u_C3 , dst_32s_C3 );  io++;
    Convert( src_8u_CA , dst_32s_CA );  io++;
    Convert( src_8u_C4 , dst_32s_C4 );  io++;

    Convert( src_8u_C1 , dst_32f_C1 );  io++;
    Convert( src_8u_C3 , dst_32f_C3 );  io++;
    Convert( src_8u_CA , dst_32f_CA );  io++;
    Convert( src_8u_C4 , dst_32f_C4 );  io++;

    Convert( src_8s_C1 , dst_32s_C1 );  io++;
    Convert( src_8s_C3 , dst_32s_C3 );  io++;
    Convert( src_8s_CA , dst_32s_CA );  io++;
    Convert( src_8s_C4 , dst_32s_C4 );  io++;

    Convert( src_8s_C1 , dst_32f_C1 );  io++;
    Convert( src_8s_C3 , dst_32f_C3 );  io++;
    Convert( src_8s_CA , dst_32f_CA );  io++;
    Convert( src_8s_C4 , dst_32f_C4 );  io++;

    Convert( src_16u_C1, dst_8u_C1  );  io++;
    Convert( src_16u_C3, dst_8u_C3  );  io++;
    Convert( src_16u_CA, dst_8u_CA  );  io++;
    Convert( src_16u_C4, dst_8u_C4  );  io++;

    Convert( src_16u_C1, dst_32s_C1 );  io++;
    Convert( src_16u_C3, dst_32s_C3 );  io++;
    Convert( src_16u_CA, dst_32s_CA );  io++;
    Convert( src_16u_C4, dst_32s_C4 );  io++;

    Convert( src_16u_C1, dst_32f_C1 );  io++;
    Convert( src_16u_C3, dst_32f_C3 );  io++;
    Convert( src_16u_CA, dst_32f_CA );  io++;
    Convert( src_16u_C4, dst_32f_C4 );  io++;

    Convert( src_16s_C1, dst_8u_C1 );   io++;
    Convert( src_16s_C3, dst_8u_C3 );   io++;
    Convert( src_16s_CA, dst_8u_CA );   io++;
    Convert( src_16s_C4, dst_8u_C4 );   io++;

    Convert( src_16s_C1, dst_32f_C1 );  io++;
    Convert( src_16s_C3, dst_32f_C3 );  io++;
    Convert( src_16s_CA, dst_32f_CA );  io++;
    Convert( src_16s_C4, dst_32f_C4 );  io++;

    Convert( src_32s_C1, dst_8u_C1 );   io++;
    Convert( src_32s_C3, dst_8u_C3 );   io++;
    Convert( src_32s_CA, dst_8u_CA );   io++;
    Convert( src_32s_C4, dst_8u_C4 );   io++;

    Convert( src_32s_C1, dst_8s_C1 );   io++;
    Convert( src_32s_C3, dst_8s_C3 );   io++;
    Convert( src_32s_CA, dst_8s_CA );   io++;
    Convert( src_32s_C4, dst_8s_C4 );   io++;

    stat = Convert( src_32f_C1, dst_8u_C1 ,      ippRndZero );  io++;
    stat = Convert( src_32f_C3, dst_8u_C3 , (IppRoundMode)0 );  io++;
    stat = Convert( src_32f_CA, dst_8u_CA ,      ippRndNear );  io++;
    stat = Convert( src_32f_C4, dst_8u_C4 , (IppRoundMode)2 );  io++;

    stat = Convert( src_32f_C1, dst_8s_C1 ,      ippRndZero );  io++;
    stat = Convert( src_32f_C3, dst_8s_C3 , (IppRoundMode)0 );  io++;
    stat = Convert( src_32f_CA, dst_8s_CA ,      ippRndNear );  io++;
    stat = Convert( src_32f_C4, dst_8s_C4 , (IppRoundMode)2 );  io++;

    stat = Convert( src_32f_C1, dst_16u_C1,      ippRndZero );  io++;
    stat = Convert( src_32f_C3, dst_16u_C3, (IppRoundMode)0 );  io++;
    stat = Convert( src_32f_CA, dst_16u_CA,      ippRndNear );  io++;
    stat = Convert( src_32f_C4, dst_16u_C4, (IppRoundMode)2 );  io++;

    stat = Convert( src_32f_C1, dst_16s_C1,      ippRndZero );  io++;
    stat = Convert( src_32f_C3, dst_16s_C3, (IppRoundMode)0 );  io++;
    stat = Convert( src_32f_CA, dst_16s_CA,      ippRndNear );  io++;
    stat = Convert( src_32f_C4, dst_16s_C4, (IppRoundMode)2 );  io++;


// ---- COPY:

// All pixels of all color channels:
    stat = Copy(                  src_8u_C1 ,  *(GIMG<Ipp8u>*)&dst_16s_C1 ); /* 8u_C1R     */  io++;
    stat = Copy(                  src_8u_C3 ,  *(IMG3<Ipp8u>*)&dst_16s_C3 ); /* 8u_C3R     */  io++;
    stat = Copy(                  src_8u_C3 ,                  dst_8u_CA  ); /* 8u_C3AC4R  */  io++;
    stat = Copy(  *(AIMG<Ipp8u>*)&src_8u_C4 ,                  dst_8u_CA  ); /* 8u_AC4R    */  io++;
    stat = Copy(                  src_8u_CA ,                  dst_8u_C3  ); /* 8u_AC4C3R  */  io++;
    stat = Copy(  *(IMG4<Ipp8u>*)&src_32s_C1,  *(IMG4<Ipp8u>*)&dst_8u_CA  ); /* 8u_C4R     */  io++;

    stat = Copy( *(GIMG<Ipp16s>*)&src_32s_C1,                  dst_16s_C1 ); /* 16s_C1R    */  io++;
    stat = Copy( *(IMG3<Ipp16s>*)&src_32f_C3,                  dst_16s_C3 ); /* 16s_C3R    */  io++;
    stat = Copy(                  src_16s_C3,                  dst_16s_CA ); /* 16s_C3AC4R */  io++;
    stat = Copy(                  src_16s_CA, *(AIMG<Ipp16s>*)&dst_16s_C4 ); /* 16s_AC4R   */  io++;
    stat = Copy(                  src_16s_CA,                  dst_16s_C3 ); /* 16s_AC4C3R */  io++;
    stat = Copy( *(IMG4<Ipp16s>*)&src_32f_CA, *(IMG4<Ipp16s>*)&dst_16s_CA ); /* 16s_C4R    */  io++;

    stat = Copy(                  src_32s_C1, *(GIMG<Ipp32s>*)&dst_8u_C4  ); /* 32s_C1R    */  io++;
    stat = Copy(                  src_32s_C3, *(IMG3<Ipp32s>*)&dst_32f_C3 ); /* 32s_C3R    */  io++;
    stat = Copy(                  src_32s_C3,                  dst_32s_CA ); /* 32s_C3AC4R */  io++;
    stat = Copy( *(AIMG<Ipp32s>*)&src_32s_C4,                  dst_32s_CA ); /* 32s_AC4R   */  io++;
    stat = Copy(                  src_32s_CA,                  dst_32s_C3 ); /* 32s_AC4C3R */  io++;
    stat = Copy(                  src_32s_C4, *(IMG4<Ipp32s>*)&dst_32s_CA ); /* 32s_C4R    */  io++;

    stat = Copy(                  src_32f_C1, *(GIMG<Ipp32f>*)&dst_8u_C4  ); /* 32f_C1R    */  io++;
    stat = Copy( *(IMG3<Ipp32f>*)&src_32s_C3,                  dst_32f_C3 ); /* 32f_C3R    */  io++;
    stat = Copy(                  src_32f_C3,                  dst_32f_CA ); /* 32f_C3AC4R */  io++;
    stat = Copy( *(AIMG<Ipp32f>*)&src_32f_C4,                  dst_32f_CA ); /* 32f_AC4R   */  io++;
    stat = Copy(                  src_32f_CA,                  dst_32f_C3 ); /* 32f_AC4C3R */  io++;
    stat = Copy(                  src_32f_C4, *(IMG4<Ipp32f>*)&dst_32f_CA ); /* 32f_C4R    */  io++;

// Masked pixels only:
    stat = Copy(                  src_8u_C1 ,  *(GIMG<Ipp8u>*)&dst_16s_C1, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_C1MR   */  io++;
    stat = Copy(                  src_8u_C3 ,  *(IMG3<Ipp8u>*)&dst_16s_C3, *(IMG3<Ipp8u>*)&mask_8u_C4 ); /* 8u_C3MR   */  io++;
    stat = Copy(  *(AIMG<Ipp8u>*)&src_8u_C4 ,                  dst_8u_CA , *(AIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_AC4MR  */  io++;
    stat = Copy(  *(IMG4<Ipp8u>*)&src_32s_C1,  *(IMG4<Ipp8u>*)&dst_8u_CA ,                 mask_8u_C4 ); /* 8u_C4MR   */  io++;

    stat = Copy( *(GIMG<Ipp16s>*)&src_32s_C1,                  dst_16s_C1, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_C1MR  */  io++;
    stat = Copy( *(IMG3<Ipp16s>*)&src_32f_C3,                  dst_16s_C3, *(IMG3<Ipp8u>*)&mask_8u_C4 ); /* 16s_C3MR  */  io++;
    stat = Copy(                  src_16s_CA, *(AIMG<Ipp16s>*)&dst_16s_C4, *(AIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_AC4MR */  io++;
    stat = Copy( *(IMG4<Ipp16s>*)&src_32f_CA, *(IMG4<Ipp16s>*)&dst_16s_CA,                 mask_8u_C4 ); /* 16s_C4MR  */  io++;

    stat = Copy(                  src_32s_C1, *(GIMG<Ipp32s>*)&dst_8u_C4 , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_C1MR  */  io++;
    stat = Copy(                  src_32s_C3, *(IMG3<Ipp32s>*)&dst_32f_C3, *(IMG3<Ipp8u>*)&mask_8u_C4 ); /* 32s_C3MR  */  io++;
    stat = Copy( *(AIMG<Ipp32s>*)&src_32s_C4,                  dst_32s_CA, *(AIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_AC4MR */  io++;
    stat = Copy(                  src_32s_C4, *(IMG4<Ipp32s>*)&dst_32s_CA,                 mask_8u_C4 ); /* 32s_C4MR  */  io++;

    stat = Copy(                  src_32f_C1, *(GIMG<Ipp32f>*)&dst_8u_C4 , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_C1MR  */  io++;
    stat = Copy( *(IMG3<Ipp32f>*)&src_32s_C3,                  dst_32f_C3, *(IMG3<Ipp8u>*)&mask_8u_C4 ); /* 32f_C3MR  */  io++;
    stat = Copy( *(AIMG<Ipp32f>*)&src_32f_C4,                  dst_32f_CA, *(AIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_AC4MR */  io++;
    stat = Copy(                  src_32f_C4, *(IMG4<Ipp32f>*)&dst_32f_CA,                 mask_8u_C4 ); /* 32f_C4MR  */  io++;

// Selected channel to selected channel:
    stat = Copy(    1,    1,                 src_8u_C3  ,  *(IMG3<Ipp8u>*)&dst_16s_C3 ); /* 8u_C3CR:  2nd -> 2nd */  io++;
    stat = Copy(    0,    2, *(IMG4<Ipp8u>*)&src_32s_C1 ,  *(IMG4<Ipp8u>*)&dst_8u_CA  ); /* 8u_C4CR:  1st -> 3d  */  io++;

    stat = Copy( ICH1, ICH1, *(IMG3<Ipp16s>*)&src_32f_C3,                  dst_16s_C3 ); /* 16s_C3CR: 1st -> 1st */  io++;
    stat = Copy( ICH4, ICH1, *(IMG4<Ipp16s>*)&src_32f_CA, *(IMG4<Ipp16s>*)&dst_16s_CA ); /* 16s_C4CR: 4th -> 1st */  io++;

    stat = Copy(    2,    2,                  src_32s_C3, *(IMG3<Ipp32s>*)&dst_32f_C3 ); /* 32s_C3CR: 3d  -> 3d  */  io++;
    stat = Copy( ICH4, ICH4,                  src_32s_C4, *(IMG4<Ipp32s>*)&dst_32s_CA ); /* 32s_C4CR: 4th -> 4th */  io++;

    stat = Copy( ICH3, ICH1, *(IMG3<Ipp32f>*)&src_32s_C3,                  dst_32f_C3 ); /* 32f_C3CR: 3d  -> 1st */  io++;
    stat = Copy(    2,    0,                  src_32f_C4, *(IMG4<Ipp32f>*)&dst_32f_CA ); /* 32f_C4CR: 3d  -> 1st */  io++;

// Selected channel to a one-channel image:
    stat = Copy(  ICH1,                 src_8u_C3 ,                   src_8u_C1 ); /* 8u_C3C1R:  1st -> */  io++;
    stat = Copy(     0, *(AIMG<Ipp8u>*)&src_8u_C4 ,  *(GIMG<Ipp8u>*)&dst_16s_C1 ); /* 8u_C4C1R:  1st -> */  io++;

    stat = Copy( ICH3,                  src_16s_C3,                  dst_16s_C1 ); /* 16s_C3C1R: 3d  -> */  io++;
    stat = Copy( ICH4, *(IMG4<Ipp16s>*)&src_32f_CA,                  dst_16s_C1 ); /* 16s_C4C1R: 4th -> */  io++;

    stat = Copy(    2,                  src_32s_C3, *(GIMG<Ipp32s>*)&dst_8u_C4  ); /* 32s_C3C1R: 3d  -> */  io++;
    stat = Copy(    3, *(AIMG<Ipp32s>*)&src_32s_C4,                  src_32s_C1 ); /* 32s_C4C1R: 4th -> */  io++;

    stat = Copy( ICH2,                  src_32f_C3,                  dst_32f_C1 ); /* 32f_C3C1R: 2nd -> */  io++;
    stat = Copy( ICH3,                  src_32f_C4, *(GIMG<Ipp32f>*)&dst_8u_C4  ); /* 32f_C4C1R: 3d  -> */  io++;

// One-channel image to selected channel:
    stat = Copy( ICH1,                  src_8u_C1 ,  *(IMG3<Ipp8u>*)&dst_16s_C3 ); /* 8u_C1C3R:  -> 1st */  io++;
    stat = Copy(    0,  *(GIMG<Ipp8u>*)&src_16s_C1,  *(IMG4<Ipp8u>*)&dst_8u_CA  ); /* 8u_C1C4R:  -> 1st */  io++;

    stat = Copy( ICH3, *(GIMG<Ipp16s>*)&src_32s_C1,                  dst_16s_C3 ); /* 16s_C1C3R: -> 3d  */  io++;
    stat = Copy( ICH4,                  src_16s_C1, *(IMG4<Ipp16s>*)&dst_16s_CA ); /* 16s_C1C4R: -> 4th */  io++;

    stat = Copy(    2,                  src_32s_C1, *(IMG3<Ipp32s>*)&dst_32f_C3 ); /* 32s_C1C3R: -> 3d  */  io++;
    stat = Copy(    3, *(GIMG<Ipp32s>*)&src_32f_C1, *(IMG4<Ipp32s>*)&dst_32s_CA ); /* 32s_C1C4R: -> 4th */  io++;

    stat = Copy( ICH2, *(GIMG<Ipp32f>*)&src_32s_C1,                  dst_32f_C3 ); /* 32f_C1C3R: -> 2nd */  io++;
    stat = Copy( ICH3,                  src_32f_C1, *(IMG4<Ipp32f>*)&dst_32f_CA ); /* 32f_C1C4R: -> 3d */   io++;

// Splitting color image into separate planes:

    stat = Copy(                 src_8u_C3  , *(IMG3P<Ipp8u>*)&dst_16u_P3  ); /* 8u_C3P3R */  io++;
    stat = Copy( *(IMG4<Ipp8u>*)&src_32s_C1 , *(IMG4P<Ipp8u>*)&dst_8u_PA   ); /* 8u_C4P4R */  io++;

    stat = Copy(                  src_16s_C3, *(IMG3P<Ipp16s>*)&dst_16u_P3 ); /* 8u_C3P3R */  io++;
    stat = Copy( *(IMG4<Ipp16s>*)&src_16s_CA, *(IMG4P<Ipp16s>*)&dst_16u_P4 ); /* 8u_C4P4R */  io++;

    stat = Copy(                  src_32s_C3,                   dst_32s_P3 ); /* 8u_C3P3R */  io++;
    stat = Copy( *(IMG4<Ipp32s>*)&src_32f_CA,                   dst_32s_P4 ); /* 8u_C4P4R */  io++;

    stat = Copy(                  src_32f_C3,                   dst_32f_P3 ); /* 8u_C3P3R */  io++;
    stat = Copy( *(IMG4<Ipp32f>*)&src_32f_CA,                   dst_32f_P4 ); /* 8u_C4P4R */  io++;

// Composing color image from separate planes:
    stat = Copy( *(IMG3P<Ipp8u>*)&dst_16u_P3 ,                 src_8u_C3   ); /* 8u_P3C3R */  io++;
    stat = Copy( *(IMG4P<Ipp8u>*)&dst_8u_PA  , *(IMG4<Ipp8u>*)&src_32s_C1  ); /* 8u_P4C4R */  io++;

    stat = Copy( *(IMG3P<Ipp16s>*)&dst_16u_P3,                  src_16s_C3 ); /* 8u_P3C3R */  io++;
    stat = Copy( *(IMG4P<Ipp16s>*)&dst_16u_P4, *(IMG4<Ipp16s>*)&src_16s_CA ); /* 8u_P4C4R */  io++;

    stat = Copy(                   dst_32s_P3,                  src_32s_C3 ); /* 8u_P3C3R */  io++;
    stat = Copy(                   dst_32s_P4, *(IMG4<Ipp32s>*)&src_32f_CA ); /* 8u_P4C4R */  io++;

    stat = Copy(                   dst_32f_P3,                  src_32f_C3 ); /* 8u_P3C3R */  io++;
    stat = Copy(                   dst_32f_P4, *(IMG4<Ipp32f>*)&src_32f_CA ); /* 8u_P4C4R */  io++;


// ---- COPY_[CONST|REPLICATE|WRAP]_BORDER:

// Images preparing:
    src_8u_C1.roiSize.resize(  -lftBorderW*2, -topBorderW*2 );
    src_8u_C3.roiSize.resize(  -lftBorderW*2, -topBorderW*2 );
    src_8u_CA.roiSize.resize(  -lftBorderW*2, -topBorderW*2 );
    src_8u_C4.roiSize.resize(  -lftBorderW*2, -topBorderW*2 );

    src_16s_C1.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_16s_C3.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_16s_CA.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_16s_C4.roiSize.resize( -lftBorderW*2, -topBorderW*2 );

    src_32s_C1.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_32s_C3.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_32s_CA.roiSize.resize( -lftBorderW*2, -topBorderW*2 );
    src_32s_C4.roiSize.resize( -lftBorderW*2, -topBorderW*2 );

// CopyConstBorder:
    stat = CopyConstBorder( src_8u_C1 , dst_8u_C1 , topBorderW, lftBorderW, 3 );                      io++;
    stat = CopyConstBorder( src_8u_C3 , dst_8u_C3 , topBorderW, lftBorderW, PVAL(8u)+LAST(3,8u) );    io++;
    stat = CopyConstBorder( src_8u_CA , dst_8u_CA , topBorderW, lftBorderW, PVAL(8u)+LAST(3,8u) );    io++;
    stat = CopyConstBorder( src_8u_C4 , dst_8u_C4 , topBorderW, lftBorderW, PVAL(8u)+LAST(4,8u) );    io++;

    stat = CopyConstBorder( src_16s_C1, dst_16s_C1, topBorderW, lftBorderW, 3 );                      io++;
    stat = CopyConstBorder( src_16s_C3, dst_16s_C3, topBorderW, lftBorderW, PVAL(16s)+LAST(3,16s) );  io++;
    stat = CopyConstBorder( src_16s_CA, dst_16s_CA, topBorderW, lftBorderW, PVAL(16s)+LAST(3,16s) );  io++;
    stat = CopyConstBorder( src_16s_C4, dst_16s_C4, topBorderW, lftBorderW, PVAL(16s)+LAST(4,16s) );  io++;

    stat = CopyConstBorder( src_32s_C1, dst_32s_C1, topBorderW, lftBorderW, 3 );                      io++;
    stat = CopyConstBorder( src_32s_C3, dst_32s_C3, topBorderW, lftBorderW, PVAL(32s)+LAST(3,32s) );  io++;
    stat = CopyConstBorder( src_32s_CA, dst_32s_CA, topBorderW, lftBorderW, PVAL(32s)+LAST(3,32s) );  io++;
    stat = CopyConstBorder( src_32s_C4, dst_32s_C4, topBorderW, lftBorderW, PVAL(32s)+LAST(4,32s) );  io++;


// CopyReplicateBorder:
    stat = CopyReplicateBorder( src_8u_C1 , dst_8u_C1 , topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_8u_C3 , dst_8u_C3 , topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_8u_CA , dst_8u_CA , topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_8u_C4 , dst_8u_C4 , topBorderW, lftBorderW );  io++;

    stat = CopyReplicateBorder( src_16s_C1, dst_16s_C1, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_16s_C3, dst_16s_C3, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_16s_CA, dst_16s_CA, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_16s_C4, dst_16s_C4, topBorderW, lftBorderW );  io++;

    stat = CopyReplicateBorder( src_32s_C1, dst_32s_C1, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_32s_C3, dst_32s_C3, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_32s_CA, dst_32s_CA, topBorderW, lftBorderW );  io++;
    stat = CopyReplicateBorder( src_32s_C4, dst_32s_C4, topBorderW, lftBorderW );  io++;

// CopyWrapBorder (NI):
    stat = CopyWrapBorder( src_32s_C1, dst_32s_C1, topBorderW, lftBorderW );       io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32s_C1.restoreRect();
    src_32s_C3.restoreRect();
    src_32s_CA.restoreRect();
    src_32s_C4.restoreRect();


// Images preparing:
    img_32s_C1.border( lftBorderW, topBorderW );

// CopyWrapBorder (I)
    stat = CopyWrapBorder( img_32s_C1, roiSize, topBorderW, lftBorderW );  io++;

// Images restoring:
    img_32s_C1.restoreRect();


// ---- Dup:
    stat = Dup( src_8u_C1 ,  *(IMG3<Ipp8u>*)&dst_16s_C3 ); /* 8u_C1C3R */  io++;

// ---- ImageJaehne:

    ImageJaehne( src_8u_C1 );   io++;
    ImageJaehne( src_8u_C3 );   io++;
    ImageJaehne( src_8u_CA );   io++;
    ImageJaehne( src_8u_C4 );   io++;

    ImageJaehne( src_8s_C1 );   io++;
    ImageJaehne( src_8s_C3 );   io++;
    ImageJaehne( src_8s_CA );   io++;
    ImageJaehne( src_8s_C4 );   io++;

    ImageJaehne( src_16u_C1 );  io++;
    ImageJaehne( src_16u_C3 );  io++;
    ImageJaehne( src_16u_CA );  io++;
    ImageJaehne( src_16u_C4 );  io++;

    ImageJaehne( src_16s_C1 );  io++;
    ImageJaehne( src_16s_C3 );  io++;
    ImageJaehne( src_16s_CA );  io++;
    ImageJaehne( src_16s_C4 );  io++;

    ImageJaehne( src_32s_C1 );  io++;
    ImageJaehne( src_32s_C3 );  io++;
    ImageJaehne( src_32s_CA );  io++;
    ImageJaehne( src_32s_C4 );  io++;

    ImageJaehne( src_32f_C1 );  io++;
    ImageJaehne( src_32f_C3 );  io++;
    ImageJaehne( src_32f_CA );  io++;
    ImageJaehne( src_32f_C4 );  io++;


// ---- IMAGE_RAMP:

    stat = ImageRamp( img_8u_C1 , IPP_MIN_8U , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8u_C3 , IPP_MIN_8U , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8u_CA , IPP_MIN_8U , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8u_C4 , IPP_MIN_8U , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;

    stat = ImageRamp( img_8s_C1 , IPP_MIN_8S , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8s_C3 , IPP_MIN_8S , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8s_CA , IPP_MIN_8S , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;
    stat = ImageRamp( img_8s_C4 , IPP_MIN_8S , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );  io++;

    stat = ImageRamp( img_16u_C1, IPP_MIN_16U, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16u_C3, IPP_MIN_16U, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16u_CA, IPP_MIN_16U, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16u_C4, IPP_MIN_16U, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;

    stat = ImageRamp( img_16s_C1, IPP_MIN_16S, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16s_C3, IPP_MIN_16S, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16s_CA, IPP_MIN_16S, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_16s_C4, IPP_MIN_16S, IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );  io++;

    stat = ImageRamp( img_32s_C1, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32s_C3, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32s_CA, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32s_C4, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;

    stat = ImageRamp( img_32f_C1, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32f_C3, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32f_CA, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;
    stat = ImageRamp( img_32f_C4, IPP_MIN_32S, IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );  io++;


// ---- SCALE:

    Scale( src_8u_C1, *(GIMG<Ipp16u>*)&dst_16s_C1 );  io++;
    Scale( src_8u_C3, dst_16u_C3 );  io++;
    Scale( src_8u_CA, dst_16u_CA );  io++;
    Scale( src_8u_C4, dst_16u_C4 );  io++;

    Scale( src_8u_C1, dst_16s_C1 );  io++;
    Scale( src_8u_C3, dst_16s_C3 );  io++;
    Scale( src_8u_CA, dst_16s_CA );  io++;
    Scale( src_8u_C4, dst_16s_C4 );  io++;

    Scale( src_8u_C1, dst_32s_C1 );  io++;
    Scale( src_8u_C3, dst_32s_C3 );  io++;
    Scale( src_8u_CA, dst_32s_CA );  io++;
    Scale( src_8u_C4, dst_32s_C4 );  io++;

    stat = Scale( src_8u_C1, dst_32f_C1, (Ipp32f)IPP_MIN_16S, (Ipp32f)IPP_MAX_16S );  io++;
    stat = Scale( src_8u_C3, dst_32f_C3, (Ipp32f)IPP_MIN_32S, (Ipp32f)IPP_MAX_32S );  io++;
    stat = Scale( src_8u_CA, dst_32f_CA, (Ipp32f)IPP_MIN_8S , (Ipp32f)IPP_MAX_32S );  io++;
    stat = Scale( src_8u_C4, dst_32f_C4, (Ipp32f)IPP_MIN_32S, (Ipp32f)IPP_MAX_8S  );  io++;

    stat = Scale( src_16u_C1, dst_8u_C1,  ippAlgHintNone     );  io++;
    stat = Scale( src_16u_C3, dst_8u_C3,  ippAlgHintFast     );  io++;
    stat = Scale( src_16u_CA, dst_8u_CA,  ippAlgHintAccurate );  io++;
    stat = Scale( src_16u_C4, dst_8u_C4, (IppHintAlgorithm)3 );  io++;

    stat = Scale( src_16s_C1, dst_8u_C1,  ippAlgHintNone     );  io++;
    stat = Scale( src_16s_C3, dst_8u_C3,  ippAlgHintFast     );  io++;
    stat = Scale( src_16s_CA, dst_8u_CA,  ippAlgHintAccurate );  io++;
    stat = Scale( src_16s_C4, dst_8u_C4, (IppHintAlgorithm)3 );  io++;

    stat = Scale( src_32s_C1, dst_8u_C1,  ippAlgHintNone     );  io++;
    stat = Scale( src_32s_C3, dst_8u_C3,  ippAlgHintFast     );  io++;
    stat = Scale( src_32s_CA, dst_8u_CA,  ippAlgHintAccurate );  io++;
    stat = Scale( src_32s_C4, dst_8u_C4, (IppHintAlgorithm)3 );  io++;

    stat = Scale( src_32f_C1, dst_8u_C1, (Ipp32f)IPP_MIN_16S, (Ipp32f)IPP_MAX_16S );  io++;
    stat = Scale( src_32f_C3, dst_8u_C3, (Ipp32f)IPP_MIN_32S, (Ipp32f)IPP_MAX_32S );  io++;
    stat = Scale( src_32f_CA, dst_8u_CA, (Ipp32f)IPP_MIN_8S , (Ipp32f)IPP_MAX_32S );  io++;
    stat = Scale( src_32f_C4, dst_8u_C4, (Ipp32f)IPP_MIN_32S, (Ipp32f)IPP_MAX_8S  );  io++;


// ---- Set:

// All pixels of all color channels:
    stat = Set(                      0,                  src_8u_C1  ); /* 8u_C1R   */  io++;
    stat = Set(  (Ipp8u*)&value_16s[2],                  src_8u_C3  ); /* 8u_C3R   */  io++;
    stat = Set(          &value_8u[1] ,  *(AIMG<Ipp8u>*)&src_8u_C4  ); /* 8u_AC4R  */  io++;
    stat = Set(           value_8u    ,  *(IMG4<Ipp8u>*)&src_8u_CA  ); /* 8u_C4R   */  io++;

    stat = Set(            IPP_MAX_16S,                  src_16s_C1 ); /* 16s_C1R  */  io++;
    stat = Set(          &value_16s[1],                  src_16s_C3 ); /* 16s_C3R  */  io++;
    stat = Set(          &value_16s[1], *(AIMG<Ipp16s>*)&src_16s_C4 ); /* 16s_AC4R */  io++;
    stat = Set(           value_16s   , *(IMG4<Ipp16s>*)&src_16s_CA ); /* 16s_C4R  */  io++;

    stat = Set(            IPP_MAX_32S,                  src_32s_C1 ); /* 32s_C1R  */  io++;
    stat = Set( (Ipp32s*)&value_32f[1],                  src_32s_C3 ); /* 32s_C3R  */  io++;
    stat = Set( (Ipp32s*)&value_32f[1], *(AIMG<Ipp32s>*)&src_32s_C4 ); /* 32s_AC4R */  io++;
    stat = Set( (Ipp32s*)value_32f    , *(IMG4<Ipp32s>*)&src_32s_CA ); /* 32s_C4R  */  io++;

    stat = Set(              (Ipp32f)0,                  src_32f_C1 ); /* 32f_C1R  */  io++;
    stat = Set(          &value_32f[1],                  src_32f_C3 ); /* 32f_C3R  */  io++;
    stat = Set(          &value_32f[1], *(AIMG<Ipp32f>*)&src_32f_C4 ); /* 32f_AC4R */  io++;
    stat = Set(           value_32f   , *(IMG4<Ipp32f>*)&src_32f_CA ); /* 32f_C4R  */  io++;

// Masked pixels only:
    stat = Set(                      0,                  src_8u_C1 , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_C1MR   */  io++;
    stat = Set(  (Ipp8u*)&value_16s[2],                  src_8u_C3 , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_C3MR   */  io++;
    stat = Set(          &value_8u[1] ,  *(AIMG<Ipp8u>*)&src_8u_C4 , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_AC4MR  */  io++;
    stat = Set(           value_8u    ,  *(IMG4<Ipp8u>*)&src_8u_CA , *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 8u_C4MR   */  io++;

    stat = Set(            IPP_MAX_16S,                  src_16s_C1, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_C1MR  */  io++;
    stat = Set(          &value_16s[1],                  src_16s_C3, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_C3MR  */  io++;
    stat = Set(          &value_16s[1], *(AIMG<Ipp16s>*)&src_16s_C4, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_AC4MR */  io++;
    stat = Set(           value_16s   , *(IMG4<Ipp16s>*)&src_16s_CA, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 16s_C4MR  */  io++;

    stat = Set(            IPP_MAX_32S,                  src_32s_C1, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_C1MR  */  io++;
    stat = Set( (Ipp32s*)&value_32f[1],                  src_32s_C3, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_C3MR  */  io++;
    stat = Set( (Ipp32s*)&value_32f[1], *(AIMG<Ipp32s>*)&src_32s_C4, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_AC4MR */  io++;
    stat = Set( (Ipp32s*)value_32f    , *(IMG4<Ipp32s>*)&src_32s_CA, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32s_C4MR  */  io++;

    stat = Set(              (Ipp32f)0,                  src_32f_C1, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_C1MR  */  io++;
    stat = Set(          &value_32f[1],                  src_32f_C3, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_C3MR  */  io++;
    stat = Set(          &value_32f[1], *(AIMG<Ipp32f>*)&src_32f_C4, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_AC4MR */  io++;
    stat = Set(           value_32f   , *(IMG4<Ipp32f>*)&src_32f_CA, *(GIMG<Ipp8u>*)&mask_8u_C4 ); /* 32f_C4MR  */  io++;


// Selected channel:
    stat = Set( ICH1, ((Ipp8u*)value_16s)[2] ,                  src_8u_C3  ); /* 8u_C3CR   1st */  io++;
    stat = Set(    0,                      0 ,  *(IMG4<Ipp8u>*)&src_8u_CA  ); /* 8u_C4CR   1st */  io++;

    stat = Set( ICH3,           value_16s[1] ,                  src_16s_C3 ); /* 16s_C3CR  3d  */  io++;
    stat = Set( ICH4,            IPP_MIN_16S , *(IMG4<Ipp16s>*)&src_16s_CA ); /* 16s_C4CR  4th */  io++;

    stat = Set(    2, *(Ipp32s*)&value_32f[1],                  src_32s_C3 ); /* 32s_C3CR  3d  */  io++;
    stat = Set(    3,           IPP_MAX_32S/2, *(IMG4<Ipp32s>*)&src_32s_CA ); /* 32s_C4CR  4th */  io++;

    stat = Set( ICH2,            value_32f[1],                  src_32f_C3 ); /* 32f_C3CR  2nd */  io++;
    stat = Set( ICH3,         IPP_MAX_32S/2.f, *(IMG4<Ipp32f>*)&src_32f_CA ); /* 32f_C4CR  3d  */  io++;


// ---- SWAP_CHANNELS (NI):

    stat = SwapChannels( src_8u_C3 , dst_8u_C3 ,  dstOrder    ); /* RGB  -> RGB  */  io++;
    stat = SwapChannels( src_8u_CA , dst_8u_CA , &dstOrder[3] ); /* RGBA -> BGRA */  io++;

    stat = SwapChannels( src_16u_C3, dst_16u_C3, &dstOrder[6] ); /* RGB  -> GBR  */  io++;
    stat = SwapChannels( src_16u_CA, dst_16u_CA, &dstOrder[9] ); /* RGBA -> RBBA */  io++;

    stat = SwapChannels( src_32s_C3, dst_32s_C3,  dstOrder    ); /* RGB  -> RGB  */  io++;
    stat = SwapChannels( src_32s_CA, dst_32s_CA, &dstOrder[3] ); /* RGBA -> BGRA */  io++;

    stat = SwapChannels( src_32f_C3, dst_32f_C3, &dstOrder[6] ); /* RGB  -> GBR  */  io++;
    stat = SwapChannels( src_32f_CA, dst_32f_CA, &dstOrder[9] ); /* RGBA -> RBBA */  io++;

// ---- SWAP_CHANNELS (I):

    stat = SwapChannels( img_8u_C3, &dstOrder[3] );              /* RGB -> BGR   */  io++;


// ---- TRANSPOSE:

// Images preparing:

    src_8u_C1.roiSize.set(  w, h );
    src_8u_C3.roiSize.set(  w, h );
    src_8u_C4.roiSize.set(  w, h );

    src_16u_C1.roiSize.set( w, h );
    src_16u_C3.roiSize.set( w, h );
    src_16u_C4.roiSize.set( w, h );

    src_32s_C1.roiSize.set( w, h );
    src_32s_C3.roiSize.set( w, h );
    src_32s_C4.roiSize.set( w, h );

    dst_8u_C1.roiSize.set(  h, w );
    dst_8u_C3.roiSize.set(  h, w );
    dst_8u_C4.roiSize.set(  h, w );

    dst_16u_C1.roiSize.set( h, w );
    dst_16u_C3.roiSize.set( h, w );
    dst_16u_C4.roiSize.set( h, w );

    dst_32s_C1.roiSize.set( h, w );
    dst_32s_C3.roiSize.set( h, w );
    dst_32s_C4.roiSize.set( h, w );

// Transpose (NI):
    stat = Transpose( src_8u_C1 , dst_8u_C1 );   io++;
    stat = Transpose( src_8u_C3 , dst_8u_C3 );   io++;
    stat = Transpose( src_8u_C4 , dst_8u_C4 );   io++;

    stat = Transpose( src_16u_C1, dst_16u_C1 );  io++;
    stat = Transpose( src_16u_C3, dst_16u_C3 );  io++;
    stat = Transpose( src_16u_C4, dst_16u_C4 );  io++;

    stat = Transpose( src_32s_C1, dst_32s_C1 );  io++;
    stat = Transpose( src_32s_C3, dst_32s_C3 );  io++;
    stat = Transpose( src_32s_C4, dst_32s_C4 );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_C4.restoreRect();

    src_16u_C1.restoreRect();
    src_16u_C3.restoreRect();
    src_16u_C4.restoreRect();

    src_32s_C1.restoreRect();
    src_32s_C3.restoreRect();
    src_32s_C4.restoreRect();

    dst_8u_C1.restoreRect();
    dst_8u_C3.restoreRect();
    dst_8u_C4.restoreRect();

    dst_16u_C1.restoreRect();
    dst_16u_C3.restoreRect();
    dst_16u_C4.restoreRect();

    dst_32s_C1.restoreRect();
    dst_32s_C3.restoreRect();
    dst_32s_C4.restoreRect();


// Images preparing:
    img_8u_C1.roiSize.set(  w, w );
    img_8u_C3.roiSize.set(  w, w );
    img_8u_C4.roiSize.set(  w, w );

    img_16u_C1.roiSize.set( w, w );
    img_16u_C3.roiSize.set( w, w );
    img_16u_C4.roiSize.set( w, w );

    img_32s_C1.roiSize.set( w, w );
    img_32s_C3.roiSize.set( w, w );
    img_32s_C4.roiSize.set( w, w );

// Transpose (I):
    stat = Transpose( img_8u_C1 );   io++;
    stat = Transpose( img_8u_C3 );   io++;
    stat = Transpose( img_8u_C4 );   io++;

    stat = Transpose( img_16u_C1 );  io++;
    stat = Transpose( img_16u_C3 );  io++;
    stat = Transpose( img_16u_C4 );  io++;

    stat = Transpose( img_32s_C1 );  io++;
    stat = Transpose( img_32s_C3 );  io++;
    stat = Transpose( img_32s_C4 );  io++;

// Images restoring:
    img_8u_C1.restoreRect();
    img_8u_C3.restoreRect();
    img_8u_C4.restoreRect();

    img_16u_C1.restoreRect();
    img_16u_C3.restoreRect();
    img_16u_C4.restoreRect();

    img_32s_C1.restoreRect();
    img_32s_C3.restoreRect();
    img_32s_C4.restoreRect();


/* #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5
#5             _   __   _ ___ _  _ _  _   ___ ___ _  ___      _  _  _  _      _    _   ___  _  ___   _  _            #5
#5           /_ / /_ /  /  /  /_ / /|/|  /_    /  / /       /_ / /| / / \     /  /  / /     / /    /_ / /            #5
#5          /  / /  \ _/  / _/  / / | | /__   / _/ /__     /  / / |/ /_ /    /__ \_/ /__- _/ /__  /  / /__           #5
#5                                    _   __   ___  __    _  ___ _   _  _  _  ___                                    #5
#5                                  /  / /_ / /_   /_ / /_ /  /  / /  / /| / /_                                      #5
#5                                  \_/ /    /__  /  \ /  /  / _/  \_/ / |/ ___/                                     #5
#5                                                                                                                   #5
#5                                < Image Arithmetic and Logical Operations (#5) >                                   #5
#5                                                                                                                   #5
#5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 */


/* #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5
#5                                                                                                                   #5
#5                                     -= Arithmetic Operations (from #5) =-                                         #5
#5                                                                                                                   #5
#5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 */

// ---- ABS:

// dst = |src|  (NI)
    Abs( src_16s_C1, dst_16s_C1 );  io++;
    Abs( src_16s_C3, dst_16s_C3 );  io++;
    Abs( src_16s_CA, dst_16s_CA );  io++;
    Abs( src_16s_C4, dst_16s_C4 );  io++;

    Abs( src_32f_C1, dst_32f_C1 );  io++;
    Abs( src_32f_C3, dst_32f_C3 );  io++;
    Abs( src_32f_CA, dst_32f_CA );  io++;
    Abs( src_32f_C4, dst_32f_C4 );  io++;

// img = |img|  (I)
    Abs( img_16s_C1 );  io++;
    Abs( img_16s_C3 );  io++;
    Abs( img_16s_CA );  io++;
    Abs( img_16s_C4 );  io++;

    Abs( img_32f_C1 );  io++;
    Abs( img_32f_C3 );  io++;
    Abs( img_32f_CA );  io++;
    Abs( img_32f_C4 );  io++;


// ---- ADD(ADD_C):

// dst = src1 + src2  (NI)
    Add( src1_8u_C1  , src2_8u_C1  , dst_8u_C1  , scale   );  io++;
    Add( src1_8u_C3  , src2_8u_C3  , dst_8u_C3  , scale+3 );  io++;
    Add( src1_8u_CA  , src2_8u_CA  , dst_8u_CA  , scale/3 );  io++;
    Add( src1_8u_C4  , src2_8u_C4  , dst_8u_C4  , 3       );  io++;

    Add( src1_16u_C1 , src2_16u_C1 , dst_16u_C1 , scale   );  io++;
    Add( src1_16u_C3 , src2_16u_C3 , dst_16u_C3 , scale+3 );  io++;
    Add( src1_16u_CA , src2_16u_CA , dst_16u_CA , scale/3 );  io++;
    Add( src1_16u_C4 , src2_16u_C4 , dst_16u_C4 , 3       );  io++;

    Add( src1_16s_C1 , src2_16s_C1 , dst_16s_C1 , scale   );  io++;
    Add( src1_16s_C3 , src2_16s_C3 , dst_16s_C3 , scale+3 );  io++;
    Add( src1_16s_CA , src2_16s_CA , dst_16s_CA , scale/3 );  io++;
    Add( src1_16s_C4 , src2_16s_C4 , dst_16s_C4 , 3       );  io++;

    Add( src1_16sc_C1, src2_16sc_C1, dst_16sc_C1, scale+3 );  io++;
    Add( src1_16sc_C3, src2_16sc_C3, dst_16sc_C3, scale/3 );  io++;
    Add( src1_16sc_CA, src2_16sc_CA, dst_16sc_CA, 3       );  io++;

    Add( src1_32sc_C1, src2_32sc_C1, dst_32sc_C1, scale+3 );  io++;
    Add( src1_32sc_C3, src2_32sc_C3, dst_32sc_C3, scale/3 );  io++;
    Add( src1_32sc_CA, src2_32sc_CA, dst_32sc_CA, 3       );  io++;

    Add( src1_32f_C1 , src2_32f_C1 , dst_32f_C1 );   io++;
    Add( src1_32f_C3 , src2_32f_C3 , dst_32f_C3 );   io++;
    Add( src1_32f_CA , src2_32f_CA , dst_32f_CA );   io++;
    Add( src1_32f_C4 , src2_32f_C4 , dst_32f_C4 );   io++;

    Add( src1_32fc_C1, src2_32fc_C1, dst_32fc_C1 );  io++;
    Add( src1_32fc_C3, src2_32fc_C3, dst_32fc_C3 );  io++;
    Add( src1_32fc_CA, src2_32fc_CA, dst_32fc_CA );  io++;

// dst += src  (I)
    Add( src_8u_C1  , dst_8u_C1  , scale   );  io++;
    Add( src_8u_C3  , dst_8u_C3  , scale+3 );  io++;
    Add( src_8u_CA  , dst_8u_CA  , scale/3 );  io++;
    Add( src_8u_C4  , dst_8u_C4  , 3       );  io++;

    Add( src_16u_C1 , dst_16u_C1 , scale   );  io++;
    Add( src_16u_C3 , dst_16u_C3 , scale+3 );  io++;
    Add( src_16u_CA , dst_16u_CA , scale/3 );  io++;
    Add( src_16u_C4 , dst_16u_C4 , 3       );  io++;

    Add( src_16s_C1 , dst_16s_C1 , scale   );  io++;
    Add( src_16s_C3 , dst_16s_C3 , scale+3 );  io++;
    Add( src_16s_CA , dst_16s_CA , scale/3 );  io++;
    Add( src_16s_C4 , dst_16s_C4 , 3       );  io++;

    Add( src_16sc_C1, dst_16sc_C1, scale+3 );  io++;
    Add( src_16sc_C3, dst_16sc_C3, scale/3 );  io++;
    Add( src_16sc_CA, dst_16sc_CA, 3       );  io++;

    Add( src_32sc_C1, dst_32sc_C1, scale+3 );  io++;
    Add( src_32sc_C3, dst_32sc_C3, scale/3 );  io++;
    Add( src_32sc_CA, dst_32sc_CA, 3       );  io++;

    Add( src_32f_C1 , dst_32f_C1 );   io++;
    Add( src_32f_C3 , dst_32f_C3 );   io++;
    Add( src_32f_CA , dst_32f_CA );   io++;
    Add( src_32f_C4 , dst_32f_C4 );   io++;

    Add( src_32fc_C1, dst_32fc_C1 );  io++;
    Add( src_32fc_C3, dst_32fc_C3 );  io++;
    Add( src_32fc_CA, dst_32fc_CA );  io++;

// dst = src + val  (NI)
    Add( src_8u_C1  , *(Ipp8u*)val_32fc     , dst_8u_C1  , scale   );  io++;
    Add( src_8u_C1  ,  255                  , dst_8u_C1  , (int)sin((double)scale) );  io++;
    Add( src_8u_C1  ,  (Ipp8u)(255/val_64f) , dst_8u_C1  , scale/(int)val_64f );       io++;
    Add( src_8u_C3  ,  (Ipp8u*)val_32fc     , dst_8u_C3  , scale+3 );  io++;
    Add( src_8u_C3  ,  (Ipp8u*)(val_32fc+1) , dst_8u_C3  , scale%3 );  io++;
    Add( src_8u_CA  ,  (Ipp8u*)val_32fc+4   , dst_8u_CA  , scale/3 );  io++;
    Add( src_8u_C4  , &((Ipp8u*)val_32fc)[4], dst_8u_C4  , 3       );  io++;

    Add( src_16u_C1 , *(Ipp16u*)val_32fc    , dst_16u_C1 , scale   );  io++;
    Add( src_16u_C3 ,  (ushort*)val_32fc    , dst_16u_C3 , scale+3 );  io++;
    Add( src_16u_CA ,  (Ipp16u*)val_32fc    , dst_16u_CA , scale/3 );  io++;
    Add( src_16u_C4 ,  (Ipp16u*)val_32fc    , dst_16u_C4 , 3       );  io++;

    Add( src_16s_C1 , *(Ipp16s*)val_32fc    , dst_16s_C1 , scale   );  io++;
    Add( src_16s_C3 ,   (short*)val_32fc    , dst_16s_C3 , scale+3 );  io++;
    Add( src_16s_CA ,  (Ipp16s*)val_32fc    , dst_16s_CA , scale/3 );  io++;
    Add( src_16s_C4 ,  (Ipp16s*)val_32fc    , dst_16s_C4 , 3       );  io++;

    Add( src_16sc_C1, *(Ipp16sc*)val_32fc   , dst_16sc_C1, scale+3 );  io++;
    Add( src_16sc_C3,  (Ipp16sc*)val_32fc   , dst_16sc_C3, scale/3 );  io++;
    Add( src_16sc_CA,  (Ipp16sc*)val_32fc   , dst_16sc_CA, 3       );  io++;

    Add( src_32sc_C1, *(Ipp32sc*)val_32fc   , dst_32sc_C1, scale+3 );  io++;
    Add( src_32sc_C3,  (Ipp32sc*)val_32fc   , dst_32sc_C3, scale/3 );  io++;
    Add( src_32sc_CA,  (Ipp32sc*)val_32fc   , dst_32sc_CA, 3       );  io++;

    Add( src_32f_C1 , *(Ipp32f*)val_32fc    , dst_32f_C1 );            io++;
    Add( src_32f_C3 ,  (Ipp32f*)val_32fc    , dst_32f_C3 );            io++;
    Add( src_32f_CA ,  (Ipp32f*)val_32fc    , dst_32f_CA );            io++;
    Add( src_32f_C4 ,  (Ipp32f*)val_32fc    , dst_32f_C4 );            io++;

    Add( src_32fc_C1, *val_32fc             , dst_32fc_C1 );           io++;
    Add( src_32fc_C3,  val_32fc             , dst_32fc_C3 );           io++;
    Add( src_32fc_CA,  val_32fc             , dst_32fc_CA );           io++;

// dst += val  (I)
    Add(  *(Ipp8u*)val_32fc     , dst_8u_C1  , scale   );  io++;
    Add(   (Ipp8u*)val_32fc     , dst_8u_C3  , scale+3 );  io++;
    Add(   (Ipp8u*)val_32fc     , dst_8u_CA  , scale/3 );  io++;
    Add(   (Ipp8u*)val_32fc     , dst_8u_C4  , 3       );  io++;

    Add( *(Ipp16u*)val_32fc     , dst_16u_C1 , scale   );  io++;
    Add(  (Ipp16u*)val_32fc     , dst_16u_C3 , scale+3 );  io++;
    Add(  (Ipp16u*)val_32fc     , dst_16u_CA , scale/3 );  io++;
    Add(  (Ipp16u*)val_32fc     , dst_16u_C4 , 3       );  io++;

    Add( *(Ipp16s*)val_32fc     , dst_16s_C1 , scale   );  io++;
    Add(  255                   , dst_16s_C1 , (int)sin((double)scale) );  io++;
    Add(  (Ipp16s)(255/val_64f) , dst_16s_C1 , scale/(int)val_64f );       io++;
    Add(  (Ipp16s*)val_32fc     , dst_16s_C3 , scale+3 );  io++;
    Add(  (Ipp16s*)(val_32fc+1) , dst_16s_C3 , scale%3 );  io++;
    Add(  (Ipp16s*)val_32fc+2   , dst_16s_CA , scale/3 );  io++;
    Add( &((Ipp16s*)val_32fc)[2], dst_16s_C4 , 3       );  io++;

    Add( *(Ipp16sc*)val_32fc    , dst_16sc_C1, scale+3 );  io++;
    Add(  (Ipp16sc*)val_32fc    , dst_16sc_C3, scale   );  io++;
    Add(  (Ipp16sc*)val_32fc    , dst_16sc_CA, 3       );  io++;

    Add( *(Ipp32sc*)val_32fc    , dst_32sc_C1, scale+3 );  io++;
    Add(  (Ipp32sc*)val_32fc    , dst_32sc_C3, scale/3 );  io++;
    Add(  (Ipp32sc*)val_32fc    , dst_32sc_CA, 3       );  io++;

    Add( *(Ipp32f*)val_32fc     , dst_32f_C1 );            io++;
    Add(  (Ipp32f*)val_32fc     , dst_32f_C3 );            io++;
    Add(  (Ipp32f*)val_32fc     , dst_32f_CA );            io++;
    Add(  (Ipp32f*)val_32fc     , dst_32f_C4 );            io++;

    Add( *val_32fc              , dst_32fc_C1 );           io++;
    Add(  val_32fc              , dst_32fc_C3 );           io++;
    Add(  val_32fc              , dst_32fc_CA );           io++;


// ---- COMPLEMENT:
// (Exists only as in-place for 32sc_C1)

    Complement( img_32s_C1 );  io++;


// ---- DIV(DIV_C):

// dst = src1 / src2  (NI)
    Div( src1_8u_C1  , src2_8u_C1  , dst_8u_C1  , scale );  io++;
    Div( src1_8u_C3  , src2_8u_C3  , dst_8u_C3  , scale );  io++;
    Div( src1_8u_CA  , src2_8u_CA  , dst_8u_CA  , scale );  io++;
    Div( src1_8u_C4  , src2_8u_C4  , dst_8u_C4  , scale );  io++;

    Div( src1_16s_C1 , src2_16s_C1 , dst_16s_C1 , scale );  io++;
    Div( src1_16s_C3 , src2_16s_C3 , dst_16s_C3 , scale );  io++;
    Div( src1_16s_CA , src2_16s_CA , dst_16s_CA , scale );  io++;
    Div( src1_16s_C4 , src2_16s_C4 , dst_16s_C4 , scale );  io++;

    Div( src1_16sc_C1, src2_16sc_C1, dst_16sc_C1, scale );  io++;
    Div( src1_16sc_C3, src2_16sc_C3, dst_16sc_C3, scale );  io++;
    Div( src1_16sc_CA, src2_16sc_CA, dst_16sc_CA, scale );  io++;

    Div( src1_32sc_C1, src2_32sc_C1, dst_32sc_C1, scale );  io++;
    Div( src1_32sc_C3, src2_32sc_C3, dst_32sc_C3, scale );  io++;
    Div( src1_32sc_CA, src2_32sc_CA, dst_32sc_CA, scale );  io++;

    Div( src1_32f_C1 , src2_32f_C1 , dst_32f_C1 );          io++;
    Div( src1_32f_C3 , src2_32f_C3 , dst_32f_C3 );          io++;
    Div( src1_32f_CA , src2_32f_CA , dst_32f_CA );          io++;
    Div( src1_32f_C4 , src2_32f_C4 , dst_32f_C4 );          io++;

    Div( src1_32fc_C1, src2_32fc_C1, dst_32fc_C1 );         io++;
    Div( src1_32fc_C3, src2_32fc_C3, dst_32fc_C3 );         io++;
    Div( src1_32fc_CA, src2_32fc_CA, dst_32fc_CA );         io++;

// dst /= src  (I)
    Div( src_8u_C1  , dst_8u_C1  , scale );  io++;
    Div( src_8u_C3  , dst_8u_C3  , scale );  io++;
    Div( src_8u_CA  , dst_8u_CA  , scale );  io++;
    Div( src_8u_C4  , dst_8u_C4  , scale );  io++;

    Div( src_16s_C1 , dst_16s_C1 , scale );  io++;
    Div( src_16s_C3 , dst_16s_C3 , scale );  io++;
    Div( src_16s_CA , dst_16s_CA , scale );  io++;
    Div( src_16s_C4 , dst_16s_C4 , scale );  io++;

    Div( src_16sc_C1, dst_16sc_C1, scale );  io++;
    Div( src_16sc_C3, dst_16sc_C3, scale );  io++;
    Div( src_16sc_CA, dst_16sc_CA, scale );  io++;

    Div( src_32sc_C1, dst_32sc_C1, scale );  io++;
    Div( src_32sc_C3, dst_32sc_C3, scale );  io++;
    Div( src_32sc_CA, dst_32sc_CA, scale );  io++;

    Div( src_32f_C1 , dst_32f_C1 );          io++;
    Div( src_32f_C3 , dst_32f_C3 );          io++;
    Div( src_32f_CA , dst_32f_CA );          io++;
    Div( src_32f_C4 , dst_32f_C4 );          io++;

    Div( src_32fc_C1, dst_32fc_C1 );         io++;
    Div( src_32fc_C3, dst_32fc_C3 );         io++;
    Div( src_32fc_CA, dst_32fc_CA );         io++;

// dst = src / val  (NI)
    Div( src_8u_C1  , *(Ipp8u*)val_32fc  , dst_8u_C1  , scale );  io++;
    Div( src_8u_C3  ,  (Ipp8u*)val_32fc  , dst_8u_C3  , scale );  io++;
    Div( src_8u_CA  ,  (Ipp8u*)val_32fc  , dst_8u_CA  , scale );  io++;
    Div( src_8u_C4  ,  (Ipp8u*)val_32fc  , dst_8u_C4  , scale );  io++;

    Div( src_16s_C1 , *(Ipp16s*)val_32fc , dst_16s_C1 , scale );  io++;
    Div( src_16s_C3 ,  (Ipp16s*)val_32fc , dst_16s_C3 , scale );  io++;
    Div( src_16s_CA ,  (Ipp16s*)val_32fc , dst_16s_CA , scale );  io++;
    Div( src_16s_C4 ,  (Ipp16s*)val_32fc , dst_16s_C4 , scale );  io++;

    Div( src_16sc_C1, *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Div( src_16sc_C3,  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Div( src_16sc_CA,  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Div( src_32sc_C1, *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Div( src_32sc_C3,  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Div( src_32sc_CA,  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Div( src_32f_C1 , *(Ipp32f*)val_32fc , dst_32f_C1 );          io++;
    Div( src_32f_C3 ,  (Ipp32f*)val_32fc , dst_32f_C3 );          io++;
    Div( src_32f_CA ,  (Ipp32f*)val_32fc , dst_32f_CA );          io++;
    Div( src_32f_C4 ,  (Ipp32f*)val_32fc , dst_32f_C4 );          io++;

    Div( src_32fc_C1, *val_32fc          , dst_32fc_C1 );         io++;
    Div( src_32fc_C3,  val_32fc          , dst_32fc_C3 );         io++;
    Div( src_32fc_CA,  val_32fc          , dst_32fc_CA );         io++;

// dst /= val  (I)
    Div(   *(Ipp8u*)val_32fc, dst_8u_C1  , scale );  io++;
    Div(    (Ipp8u*)val_32fc, dst_8u_C3  , scale );  io++;
    Div(    (Ipp8u*)val_32fc, dst_8u_CA  , scale );  io++;
    Div(    (Ipp8u*)val_32fc, dst_8u_C4  , scale );  io++;

    Div(  *(Ipp16s*)val_32fc, dst_16s_C1 , scale );  io++;
    Div(   (Ipp16s*)val_32fc, dst_16s_C3 , scale );  io++;
    Div(   (Ipp16s*)val_32fc, dst_16s_CA , scale );  io++;
    Div(   (Ipp16s*)val_32fc, dst_16s_C4 , scale );  io++;

    Div( *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Div(  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Div(  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Div( *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Div(  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Div(  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Div(  *(Ipp32f*)val_32fc, dst_32f_C1 );          io++;
    Div(   (Ipp32f*)val_32fc, dst_32f_C3 );          io++;
    Div(   (Ipp32f*)val_32fc, dst_32f_CA );          io++;
    Div(   (Ipp32f*)val_32fc, dst_32f_C4 );          io++;

    Div(           *val_32fc, dst_32fc_C1 );         io++;
    Div(            val_32fc, dst_32fc_C3 );         io++;
    Div(            val_32fc, dst_32fc_CA );         io++;


// ---- EXP:

// dst = e^src  (NI)
    Exp( src_8u_C1 , dst_8u_C1 , scale );  io++;
    Exp( src_8u_C3 , dst_8u_C3 , scale );  io++;

    Exp( src_16s_C1, dst_16s_C1, scale );  io++;
    Exp( src_16s_C3, dst_16s_C3, scale );  io++;

    Exp( src_32f_C1, dst_32f_C1 );         io++;
    Exp( src_32f_C3, dst_32f_C3 );         io++;

// img = e^img  (I)
    Exp( img_8u_C1 , scale );  io++;
    Exp( img_8u_C3 , scale );  io++;

    Exp( img_16s_C1, scale );  io++;
    Exp( img_16s_C3, scale );  io++;

    Exp( img_32f_C1 );         io++;
    Exp( img_32f_C3 );         io++;


// ---- LN:

// dst = ln(src)  (NI)
    Ln( src_8u_C1 , dst_8u_C1 , scale );  io++;
    Ln( src_8u_C3 , dst_8u_C3 , scale );  io++;

    Ln( src_16s_C1, dst_16s_C1, scale );  io++;
    Ln( src_16s_C3, dst_16s_C3, scale );  io++;

    Ln( src_32f_C1, dst_32f_C1 );         io++;
    Ln( src_32f_C3, dst_32f_C3 );         io++;

// img = ln(img)  (I)
    Ln( img_8u_C1 , scale );  io++;
    Ln( img_8u_C3 , scale );  io++;

    Ln( img_16s_C1, scale );  io++;
    Ln( img_16s_C3, scale );  io++;

    Ln( img_32f_C1 );         io++;
    Ln( img_32f_C3 );         io++;


// ---- MUL(MUL_C):

// dst = src1 * src2  (NI)
    Mul( src1_8u_C1  , src2_8u_C1  , dst_8u_C1  , scale );  io++;
    Mul( src1_8u_C3  , src2_8u_C3  , dst_8u_C3  , scale );  io++;
    Mul( src1_8u_CA  , src2_8u_CA  , dst_8u_CA  , scale );  io++;
    Mul( src1_8u_C4  , src2_8u_C4  , dst_8u_C4  , scale );  io++;

    Mul( src1_16u_C1 , src2_16u_C1 , dst_16u_C1 , scale );  io++;
    Mul( src1_16u_C3 , src2_16u_C3 , dst_16u_C3 , scale );  io++;
    Mul( src1_16u_CA , src2_16u_CA , dst_16u_CA , scale );  io++;
    Mul( src1_16u_C4 , src2_16u_C4 , dst_16u_C4 , scale );  io++;

    Mul( src1_16s_C1 , src2_16s_C1 , dst_16s_C1 , scale );  io++;
    Mul( src1_16s_C3 , src2_16s_C3 , dst_16s_C3 , scale );  io++;
    Mul( src1_16s_CA , src2_16s_CA , dst_16s_CA , scale );  io++;
    Mul( src1_16s_C4 , src2_16s_C4 , dst_16s_C4 , scale );  io++;

    Mul( src1_16sc_C1, src2_16sc_C1, dst_16sc_C1, scale );  io++;
    Mul( src1_16sc_C3, src2_16sc_C3, dst_16sc_C3, scale );  io++;
    Mul( src1_16sc_CA, src2_16sc_CA, dst_16sc_CA, scale );  io++;

    Mul( src1_32sc_C1, src2_32sc_C1, dst_32sc_C1, scale );  io++;
    Mul( src1_32sc_C3, src2_32sc_C3, dst_32sc_C3, scale );  io++;
    Mul( src1_32sc_CA, src2_32sc_CA, dst_32sc_CA, scale );  io++;

    Mul( src1_32f_C1 , src2_32f_C1 , dst_32f_C1 );          io++;
    Mul( src1_32f_C3 , src2_32f_C3 , dst_32f_C3 );          io++;
    Mul( src1_32f_CA , src2_32f_CA , dst_32f_CA );          io++;
    Mul( src1_32f_C4 , src2_32f_C4 , dst_32f_C4 );          io++;

    Mul( src1_32fc_C1, src2_32fc_C1, dst_32fc_C1 );         io++;
    Mul( src1_32fc_C3, src2_32fc_C3, dst_32fc_C3 );         io++;
    Mul( src1_32fc_CA, src2_32fc_CA, dst_32fc_CA );         io++;

// dst *= src  (I)
    Mul( src_8u_C1  , dst_8u_C1  , scale );  io++;
    Mul( src_8u_C3  , dst_8u_C3  , scale );  io++;
    Mul( src_8u_CA  , dst_8u_CA  , scale );  io++;
    Mul( src_8u_C4  , dst_8u_C4  , scale );  io++;

    Mul( src_16u_C1 , dst_16u_C1 , scale );  io++;
    Mul( src_16u_C3 , dst_16u_C3 , scale );  io++;
    Mul( src_16u_CA , dst_16u_CA , scale );  io++;
    Mul( src_16u_C4 , dst_16u_C4 , scale );  io++;

    Mul( src_16s_C1 , dst_16s_C1 , scale );  io++;
    Mul( src_16s_C3 , dst_16s_C3 , scale );  io++;
    Mul( src_16s_CA , dst_16s_CA , scale );  io++;
    Mul( src_16s_C4 , dst_16s_C4 , scale );  io++;

    Mul( src_16sc_C1, dst_16sc_C1, scale );  io++;
    Mul( src_16sc_C3, dst_16sc_C3, scale );  io++;
    Mul( src_16sc_CA, dst_16sc_CA, scale );  io++;

    Mul( src_32sc_C1, dst_32sc_C1, scale );  io++;
    Mul( src_32sc_C3, dst_32sc_C3, scale );  io++;
    Mul( src_32sc_CA, dst_32sc_CA, scale );  io++;

    Mul( src_32f_C1 , dst_32f_C1 );          io++;
    Mul( src_32f_C3 , dst_32f_C3 );          io++;
    Mul( src_32f_CA , dst_32f_CA );          io++;
    Mul( src_32f_C4 , dst_32f_C4 );          io++;

    Mul( src_32fc_C1, dst_32fc_C1 );         io++;
    Mul( src_32fc_C3, dst_32fc_C3 );         io++;
    Mul( src_32fc_CA, dst_32fc_CA );         io++;

// dst = src * val  (NI)
    Mul( src_8u_C1  ,   *(Ipp8u*)val_32fc, dst_8u_C1  , scale );  io++;
    Mul( src_8u_C3  ,    (Ipp8u*)val_32fc, dst_8u_C3  , scale );  io++;
    Mul( src_8u_CA  ,    (Ipp8u*)val_32fc, dst_8u_CA  , scale );  io++;
    Mul( src_8u_C4  ,    (Ipp8u*)val_32fc, dst_8u_C4  , scale );  io++;

    Mul( src_16u_C1 ,  *(Ipp16u*)val_32fc, dst_16u_C1 , scale );  io++;
    Mul( src_16u_C3 ,   (Ipp16u*)val_32fc, dst_16u_C3 , scale );  io++;
    Mul( src_16u_CA ,   (Ipp16u*)val_32fc, dst_16u_CA , scale );  io++;
    Mul( src_16u_C4 ,   (Ipp16u*)val_32fc, dst_16u_C4 , scale );  io++;

    Mul( src_16s_C1 ,  *(Ipp16s*)val_32fc, dst_16s_C1 , scale );  io++;
    Mul( src_16s_C3 ,   (Ipp16s*)val_32fc, dst_16s_C3 , scale );  io++;
    Mul( src_16s_CA ,   (Ipp16s*)val_32fc, dst_16s_CA , scale );  io++;
    Mul( src_16s_C4 ,   (Ipp16s*)val_32fc, dst_16s_C4 , scale );  io++;

    Mul( src_16sc_C1, *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Mul( src_16sc_C3,  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Mul( src_16sc_CA,  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Mul( src_32sc_C1, *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Mul( src_32sc_C3,  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Mul( src_32sc_CA,  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Mul( src_32f_C1 ,  *(Ipp32f*)val_32fc, dst_32f_C1 );          io++;
    Mul( src_32f_C3 ,   (Ipp32f*)val_32fc, dst_32f_C3 );          io++;
    Mul( src_32f_CA ,   (Ipp32f*)val_32fc, dst_32f_CA );          io++;
    Mul( src_32f_C4 ,   (Ipp32f*)val_32fc, dst_32f_C4 );          io++;

    Mul( src_32fc_C1,           *val_32fc, dst_32fc_C1 );         io++;
    Mul( src_32fc_C3,            val_32fc, dst_32fc_C3 );         io++;
    Mul( src_32fc_CA,            val_32fc, dst_32fc_CA );         io++;

// dst *= val  (I)
    Mul(   *(Ipp8u*)val_32fc, dst_8u_C1  , scale );  io++;
    Mul(    (Ipp8u*)val_32fc, dst_8u_C3  , scale );  io++;
    Mul(    (Ipp8u*)val_32fc, dst_8u_CA  , scale );  io++;
    Mul(    (Ipp8u*)val_32fc, dst_8u_C4  , scale );  io++;

    Mul(  *(Ipp16u*)val_32fc, dst_16u_C1 , scale );  io++;
    Mul(   (Ipp16u*)val_32fc, dst_16u_C3 , scale );  io++;
    Mul(   (Ipp16u*)val_32fc, dst_16u_CA , scale );  io++;
    Mul(   (Ipp16u*)val_32fc, dst_16u_C4 , scale );  io++;

    Mul(  *(Ipp16s*)val_32fc, dst_16s_C1 , scale );  io++;
    Mul(   (Ipp16s*)val_32fc, dst_16s_C3 , scale );  io++;
    Mul(   (Ipp16s*)val_32fc, dst_16s_CA , scale );  io++;
    Mul(   (Ipp16s*)val_32fc, dst_16s_C4 , scale );  io++;

    Mul( *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Mul(  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Mul(  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Mul( *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Mul(  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Mul(  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Mul(  *(Ipp32f*)val_32fc, dst_32f_C1 );          io++;
    Mul(   (Ipp32f*)val_32fc, dst_32f_C3 );          io++;
    Mul(   (Ipp32f*)val_32fc, dst_32f_CA );          io++;
    Mul(   (Ipp32f*)val_32fc, dst_32f_C4 );          io++;

    Mul(           *val_32fc, dst_32fc_C1 );         io++;
    Mul(            val_32fc, dst_32fc_C3 );         io++;
    Mul(            val_32fc, dst_32fc_CA );         io++;


// ---- MUL_SCALE:

// dst = src1 * src2 / maxval (NI)
    MulScale( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 );   io++;
    MulScale( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 );   io++;
    MulScale( src1_8u_CA , src2_8u_CA , dst_8u_CA );   io++;
    MulScale( src1_8u_C4 , src2_8u_C4 , dst_8u_C4 );   io++;

    MulScale( src1_16u_C1, src2_16u_C1, dst_16u_C1 );  io++;
    MulScale( src1_16u_C3, src2_16u_C3, dst_16u_C3 );  io++;
    MulScale( src1_16u_CA, src2_16u_CA, dst_16u_CA );  io++;
    MulScale( src1_16u_C4, src2_16u_C4, dst_16u_C4 );  io++;


// img *= src / maxval  (I)
    MulScale( src_8u_C1 , img_8u_C1 );  io++;
    MulScale( src_8u_C3 , img_8u_C3 );  io++;
    MulScale( src_8u_CA , img_8u_CA );  io++;
    MulScale( src_8u_C4 , img_8u_C4 );  io++;

    MulScale( src_16u_C1, img_16u_C1 ); io++;
    MulScale( src_16u_C3, img_16u_C3 ); io++;
    MulScale( src_16u_CA, img_16u_CA ); io++;
    MulScale( src_16u_C4, img_16u_C4 ); io++;


// ---- MULC_SCALE:

// dst = src * val / maxval  (NI)
    MulCScale( src_8u_C1 , *(Ipp8u*)val_32fc , dst_8u_C1 );  io++;
    MulCScale( src_8u_C3 ,  (Ipp8u*)val_32fc , dst_8u_C3 );  io++;
    MulCScale( src_8u_CA ,  (Ipp8u*)val_32fc , dst_8u_CA );  io++;
    MulCScale( src_8u_C4 ,  (Ipp8u*)val_32fc , dst_8u_C4 );  io++;

    MulCScale( src_16u_C1, *(Ipp16u*)val_32fc, dst_16u_C1 ); io++;
    MulCScale( src_16u_C3,  (Ipp16u*)val_32fc, dst_16u_C3 ); io++;
    MulCScale( src_16u_CA,  (Ipp16u*)val_32fc, dst_16u_CA ); io++;
    MulCScale( src_16u_C4,  (Ipp16u*)val_32fc, dst_16u_C4 ); io++;

// img *= val / maxval  (I)
    MulCScale( *(Ipp8u*)val_32fc , img_8u_C1 );  io++;
    MulCScale(  (Ipp8u*)val_32fc , img_8u_C3 );  io++;
    MulCScale(  (Ipp8u*)val_32fc , img_8u_CA );  io++;
    MulCScale(  (Ipp8u*)val_32fc , img_8u_C4 );  io++;

    MulCScale( *(Ipp16u*)val_32fc, img_16u_C1 ); io++;
    MulCScale(  (Ipp16u*)val_32fc, img_16u_C3 ); io++;
    MulCScale(  (Ipp16u*)val_32fc, img_16u_CA ); io++;
    MulCScale(  (Ipp16u*)val_32fc, img_16u_C4 ); io++;


// ---- SQR:

// dst = src^2  (NI)
    Sqr( src_8u_C1 , dst_8u_C1 , scale );  io++;
    Sqr( src_8u_C3 , dst_8u_C3 , scale );  io++;
    Sqr( src_8u_CA , dst_8u_CA , scale );  io++;
    Sqr( src_8u_C4 , dst_8u_C4 , scale );  io++;

    Sqr( src_16u_C1, dst_16u_C1, scale );  io++;
    Sqr( src_16u_C3, dst_16u_C3, scale );  io++;
    Sqr( src_16u_CA, dst_16u_CA, scale );  io++;
    Sqr( src_16u_C4, dst_16u_C4, scale );  io++;

    Sqr( src_16s_C1, dst_16s_C1, scale );  io++;
    Sqr( src_16s_C3, dst_16s_C3, scale );  io++;
    Sqr( src_16s_CA, dst_16s_CA, scale );  io++;
    Sqr( src_16s_C4, dst_16s_C4, scale );  io++;

    Sqr( src_32f_C1, dst_32f_C1 );         io++;
    Sqr( src_32f_C3, dst_32f_C3 );         io++;
    Sqr( src_32f_CA, dst_32f_CA );         io++;
    Sqr( src_32f_C4, dst_32f_C4 );         io++;

// img = img^2  (I)
    Sqr( img_8u_C1 , scale );  io++;
    Sqr( img_8u_C3 , scale );  io++;
    Sqr( img_8u_CA , scale );  io++;
    Sqr( img_8u_C4 , scale );  io++;

    Sqr( img_16u_C1, scale );  io++;
    Sqr( img_16u_C3, scale );  io++;
    Sqr( img_16u_CA, scale );  io++;
    Sqr( img_16u_C4, scale );  io++;

    Sqr( img_16s_C1, scale );  io++;
    Sqr( img_16s_C3, scale );  io++;
    Sqr( img_16s_CA, scale );  io++;
    Sqr( img_16s_C4, scale );  io++;

    Sqr( img_32f_C1 );         io++;
    Sqr( img_32f_C3 );         io++;
    Sqr( img_32f_CA );         io++;
    Sqr( img_32f_C4 );         io++;


// ---- SQRT:
//        ____
// dst = V src  (NI)
    Sqrt( src_8u_C1 , dst_8u_C1 , scale );  io++;
    Sqrt( src_8u_C3 , dst_8u_C3 , scale );  io++;
    Sqrt( src_8u_CA , dst_8u_CA , scale );  io++;

    Sqrt( src_16u_C1, dst_16u_C1, scale );  io++;
    Sqrt( src_16u_C3, dst_16u_C3, scale );  io++;
    Sqrt( src_16u_CA, dst_16u_CA, scale );  io++;

    Sqrt( src_16s_C1, dst_16s_C1, scale );  io++;
    Sqrt( src_16s_C3, dst_16s_C3, scale );  io++;
    Sqrt( src_16s_CA, dst_16s_CA, scale );  io++;

    Sqrt( src_32f_C1, dst_32f_C1 );         io++;
    Sqrt( src_32f_C3, dst_32f_C3 );         io++;
    Sqrt( src_32f_CA, dst_32f_CA );         io++;
//        ____
// img = V img  (I)
    Sqrt( img_8u_C1 , scale );  io++;
    Sqrt( img_8u_C3 , scale );  io++;
    Sqrt( img_8u_CA , scale );  io++;

    Sqrt( img_16u_C1, scale );  io++;
    Sqrt( img_16u_C3, scale );  io++;
    Sqrt( img_16u_CA, scale );  io++;

    Sqrt( img_16s_C1, scale );  io++;
    Sqrt( img_16s_C3, scale );  io++;
    Sqrt( img_16s_CA, scale );  io++;

    Sqrt( img_32f_C1 );         io++;
    Sqrt( img_32f_C3 );         io++;
    Sqrt( img_32f_CA );         io++;
    Sqrt( img_32f_C4 );         io++;


// ---- SUB(SUB_C):

// dst = src1 - src2  (NI)
    Sub( src1_8u_C1  , src2_8u_C1  , dst_8u_C1  , scale );  io++;
    Sub( src1_8u_C3  , src2_8u_C3  , dst_8u_C3  , scale );  io++;
    Sub( src1_8u_CA  , src2_8u_CA  , dst_8u_CA  , scale );  io++;
    Sub( src1_8u_C4  , src2_8u_C4  , dst_8u_C4  , scale );  io++;

    Sub( src1_16u_C1 , src2_16u_C1 , dst_16u_C1 , scale );  io++;
    Sub( src1_16u_C3 , src2_16u_C3 , dst_16u_C3 , scale );  io++;
    Sub( src1_16u_CA , src2_16u_CA , dst_16u_CA , scale );  io++;
    Sub( src1_16u_C4 , src2_16u_C4 , dst_16u_C4 , scale );  io++;

    Sub( src1_16s_C1 , src2_16s_C1 , dst_16s_C1 , scale );  io++;
    Sub( src1_16s_C3 , src2_16s_C3 , dst_16s_C3 , scale );  io++;
    Sub( src1_16s_CA , src2_16s_CA , dst_16s_CA , scale );  io++;
    Sub( src1_16s_C4 , src2_16s_C4 , dst_16s_C4 , scale );  io++;

    Sub( src1_16sc_C1, src2_16sc_C1, dst_16sc_C1, scale );  io++;
    Sub( src1_16sc_C3, src2_16sc_C3, dst_16sc_C3, scale );  io++;
    Sub( src1_16sc_CA, src2_16sc_CA, dst_16sc_CA, scale );  io++;

    Sub( src1_32sc_C1, src2_32sc_C1, dst_32sc_C1, scale );  io++;
    Sub( src1_32sc_C3, src2_32sc_C3, dst_32sc_C3, scale );  io++;
    Sub( src1_32sc_CA, src2_32sc_CA, dst_32sc_CA, scale );  io++;

    Sub( src1_32f_C1 , src2_32f_C1 , dst_32f_C1 );          io++;
    Sub( src1_32f_C3 , src2_32f_C3 , dst_32f_C3 );          io++;
    Sub( src1_32f_CA , src2_32f_CA , dst_32f_CA );          io++;
    Sub( src1_32f_C4 , src2_32f_C4 , dst_32f_C4 );          io++;

    Sub( src1_32fc_C1, src2_32fc_C1, dst_32fc_C1 );         io++;
    Sub( src1_32fc_C3, src2_32fc_C3, dst_32fc_C3 );         io++;
    Sub( src1_32fc_CA, src2_32fc_CA, dst_32fc_CA );         io++;

// dst -= src  (I)
    Sub( src_8u_C1  , dst_8u_C1  , scale );  io++;
    Sub( src_8u_C3  , dst_8u_C3  , scale );  io++;
    Sub( src_8u_CA  , dst_8u_CA  , scale );  io++;
    Sub( src_8u_C4  , dst_8u_C4  , scale );  io++;

    Sub( src_16u_C1 , dst_16u_C1 , scale );  io++;
    Sub( src_16u_C3 , dst_16u_C3 , scale );  io++;
    Sub( src_16u_CA , dst_16u_CA , scale );  io++;
    Sub( src_16u_C4 , dst_16u_C4 , scale );  io++;

    Sub( src_16s_C1 , dst_16s_C1 , scale );  io++;
    Sub( src_16s_C3 , dst_16s_C3 , scale );  io++;
    Sub( src_16s_CA , dst_16s_CA , scale );  io++;
    Sub( src_16s_C4 , dst_16s_C4 , scale );  io++;

    Sub( src_16sc_C1, dst_16sc_C1, scale );  io++;
    Sub( src_16sc_C3, dst_16sc_C3, scale );  io++;
    Sub( src_16sc_CA, dst_16sc_CA, scale );  io++;

    Sub( src_32sc_C1, dst_32sc_C1, scale );  io++;
    Sub( src_32sc_C3, dst_32sc_C3, scale );  io++;
    Sub( src_32sc_CA, dst_32sc_CA, scale );  io++;

    Sub( src_32f_C1 , dst_32f_C1 );          io++;
    Sub( src_32f_C3 , dst_32f_C3 );          io++;
    Sub( src_32f_CA , dst_32f_CA );          io++;
    Sub( src_32f_C4 , dst_32f_C4 );          io++;

    Sub( src_32fc_C1, dst_32fc_C1 );         io++;
    Sub( src_32fc_C3, dst_32fc_C3 );         io++;
    Sub( src_32fc_CA, dst_32fc_CA );         io++;

// dst = src - val  (NI)
    Sub( src_8u_C1  ,   *(Ipp8u*)val_32fc, dst_8u_C1  , scale );  io++;
    Sub( src_8u_C3  ,    (Ipp8u*)val_32fc, dst_8u_C3  , scale );  io++;
    Sub( src_8u_CA  ,    (Ipp8u*)val_32fc, dst_8u_CA  , scale );  io++;
    Sub( src_8u_C4  ,    (Ipp8u*)val_32fc, dst_8u_C4  , scale );  io++;

    Sub( src_16u_C1 ,  *(Ipp16u*)val_32fc, dst_16u_C1 , scale );  io++;
    Sub( src_16u_C3 ,   (Ipp16u*)val_32fc, dst_16u_C3 , scale );  io++;
    Sub( src_16u_CA ,   (Ipp16u*)val_32fc, dst_16u_CA , scale );  io++;
    Sub( src_16u_C4 ,   (Ipp16u*)val_32fc, dst_16u_C4 , scale );  io++;

    Sub( src_16s_C1 ,  *(Ipp16s*)val_32fc, dst_16s_C1 , scale );  io++;
    Sub( src_16s_C3 ,   (Ipp16s*)val_32fc, dst_16s_C3 , scale );  io++;
    Sub( src_16s_CA ,   (Ipp16s*)val_32fc, dst_16s_CA , scale );  io++;
    Sub( src_16s_C4 ,   (Ipp16s*)val_32fc, dst_16s_C4 , scale );  io++;

    Sub( src_16sc_C1, *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Sub( src_16sc_C3,  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Sub( src_16sc_CA,  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Sub( src_32sc_C1, *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Sub( src_32sc_C3,  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Sub( src_32sc_CA,  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Sub( src_32f_C1 ,  *(Ipp32f*)val_32fc, dst_32f_C1 );          io++;
    Sub( src_32f_C3 ,   (Ipp32f*)val_32fc, dst_32f_C3 );          io++;
    Sub( src_32f_CA ,   (Ipp32f*)val_32fc, dst_32f_CA );          io++;
    Sub( src_32f_C4 ,   (Ipp32f*)val_32fc, dst_32f_C4 );          io++;

    Sub( src_32fc_C1,           *val_32fc, dst_32fc_C1 );         io++;
    Sub( src_32fc_C3,            val_32fc, dst_32fc_C3 );         io++;
    Sub( src_32fc_CA,            val_32fc, dst_32fc_CA );         io++;

// dst -= val  (I)
    Sub(   *(Ipp8u*)val_32fc, dst_8u_C1  , scale );  io++;
    Sub(    (Ipp8u*)val_32fc, dst_8u_C3  , scale );  io++;
    Sub(    (Ipp8u*)val_32fc, dst_8u_CA  , scale );  io++;
    Sub(    (Ipp8u*)val_32fc, dst_8u_C4  , scale );  io++;

    Sub(  *(Ipp16u*)val_32fc, dst_16u_C1 , scale );  io++;
    Sub(   (Ipp16u*)val_32fc, dst_16u_C3 , scale );  io++;
    Sub(   (Ipp16u*)val_32fc, dst_16u_CA , scale );  io++;
    Sub(   (Ipp16u*)val_32fc, dst_16u_C4 , scale );  io++;

    Sub(  *(Ipp16s*)val_32fc, dst_16s_C1 , scale );  io++;
    Sub(   (Ipp16s*)val_32fc, dst_16s_C3 , scale );  io++;
    Sub(   (Ipp16s*)val_32fc, dst_16s_CA , scale );  io++;
    Sub(   (Ipp16s*)val_32fc, dst_16s_C4 , scale );  io++;

    Sub( *(Ipp16sc*)val_32fc, dst_16sc_C1, scale );  io++;
    Sub(  (Ipp16sc*)val_32fc, dst_16sc_C3, scale );  io++;
    Sub(  (Ipp16sc*)val_32fc, dst_16sc_CA, scale );  io++;

    Sub( *(Ipp32sc*)val_32fc, dst_32sc_C1, scale );  io++;
    Sub(  (Ipp32sc*)val_32fc, dst_32sc_C3, scale );  io++;
    Sub(  (Ipp32sc*)val_32fc, dst_32sc_CA, scale );  io++;

    Sub(  *(Ipp32f*)val_32fc, dst_32f_C1 );          io++;
    Sub(   (Ipp32f*)val_32fc, dst_32f_C3 );          io++;
    Sub(   (Ipp32f*)val_32fc, dst_32f_CA );          io++;
    Sub(   (Ipp32f*)val_32fc, dst_32f_C4 );          io++;

    Sub(           *val_32fc, dst_32fc_C1 );         io++;
    Sub(            val_32fc, dst_32fc_C3 );         io++;
    Sub(            val_32fc, dst_32fc_CA );         io++;


/* #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5
#5                                                                                                                   #5
#5                                      -= Logical  Operations (from #5) =-                                          #5
#5                                                                                                                   #5
#5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 */

// ---- AND(AND_C):

// dst = src1 & src2  (NI)
    And( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 );   io++;
    And( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 );   io++;
    And( src1_8u_CA , src2_8u_CA , dst_8u_CA );   io++;
    And( src1_8u_C4 , src2_8u_C4 , dst_8u_C4 );   io++;

    And( src1_16u_C1, src2_16u_C1, dst_16u_C1 );  io++;
    And( src1_16u_C3, src2_16u_C3, dst_16u_C3 );  io++;
    And( src1_16u_CA, src2_16u_CA, dst_16u_CA );  io++;
    And( src1_16u_C4, src2_16u_C4, dst_16u_C4 );  io++;

    And( src1_32s_C1, src2_32s_C1, dst_32s_C1 );  io++;
    And( src1_32s_C3, src2_32s_C3, dst_32s_C3 );  io++;
    And( src1_32s_CA, src2_32s_CA, dst_32s_CA );  io++;
    And( src1_32s_C4, src2_32s_C4, dst_32s_C4 );  io++;

// img &= src  (I)
    And( src_8u_C1 , img_8u_C1 );   io++;
    And( src_8u_C3 , img_8u_C3 );   io++;
    And( src_8u_CA , img_8u_CA );   io++;
    And( src_8u_C4 , img_8u_C4 );   io++;

    And( src_16u_C1, img_16u_C1 );  io++;
    And( src_16u_C3, img_16u_C3 );  io++;
    And( src_16u_CA, dst_16u_CA );  io++;
    And( src_16u_C4, dst_16u_C4 );  io++;

    And( src_32s_C1, dst_32s_C1 );  io++;
    And( src_32s_C3, dst_32s_C3 );  io++;
    And( src_32s_CA, dst_32s_CA );  io++;
    And( src_32s_C4, dst_32s_C4 );  io++;

// dst = src & val  (NI)
    And( src_8u_C1 ,   (Ipp8u)(255/val_64f), dst_8u_C1 );   io++;
    And( src_8u_C3 ,   (Ipp8u*)(val_32fc+1), dst_8u_C3 );   io++;
    And( src_8u_CA ,   (Ipp8u*)val_32fc+4  , dst_8u_CA );   io++;
    And( src_8u_C4 , &((Ipp8u*)val_32fc)[4], dst_8u_C4 );   io++;

    And( src_16u_C1, *(Ipp16u*)val_32fc    , dst_16u_C1 );  io++;
    And( src_16u_C3,  (ushort*)val_32fc    , dst_16u_C3 );  io++;
    And( src_16u_CA,  (Ipp16u*)val_32fc    , dst_16u_CA );  io++;
    And( src_16u_C4,  (Ipp16u*)val_32fc    , dst_16u_C4 );  io++;

    And( src_32s_C1, *(Ipp32s*)val_32fc    , dst_32s_C1 );  io++;
    And( src_32s_C3,  (Ipp32s*)val_32fc    , dst_32s_C3 );  io++;
    And( src_32s_CA,  (Ipp32s*)val_32fc    , dst_32s_CA );  io++;
    And( src_32s_C4,  (Ipp32s*)val_32fc    , dst_32s_C4 );  io++;

// img &= val  (I)
    And(   *(Ipp8u*)val_32fc    , img_8u_C1 );   io++;
    And(    (Ipp8u*)val_32fc    , img_8u_C3 );   io++;
    And(    (Ipp8u*)val_32fc    , img_8u_CA );   io++;
    And(    (Ipp8u*)val_32fc    , img_8u_C4 );   io++;

    And(                     255, img_16u_C1 );  io++;
    And(   (Ipp16u)(255/val_64f), img_16u_C1 );  io++;
    And(   (Ipp16u*)(val_32fc+1), img_16u_C3 );  io++;
    And(   (Ipp16u*)val_32fc+2  , img_16u_CA );  io++;
    And( &((Ipp16u*)val_32fc)[2], img_16u_C4 );  io++;

    And(  *(Ipp32s*)val_32fc    , img_32s_C1 );  io++;
    And(   (Ipp32s*)val_32fc    , img_32s_C3 );  io++;
    And(   (Ipp32s*)val_32fc    , img_32s_CA );  io++;
    And(   (Ipp32s*)val_32fc    , img_32s_C4 );  io++;


// ---- LSHIFT_C:

// dst = src << val_32u  (NI)
    stat = LShift( src_8u_C1 ,       (Ipp32u)(255/val_64f), dst_8u_C1 );   io++;
    stat = LShift( src_8u_C3 ,       (Ipp32u*)(val_32fc+1), dst_8u_C3 );   io++;
    stat = LShift( src_8u_CA ,       (Ipp32u*)val_32fc+4  , dst_8u_CA );   io++;
    stat = LShift( src_8u_C4 ,     &((Ipp32u*)val_32fc)[4], dst_8u_C4 );   io++;

    stat = LShift( src_16u_C1,      *(Ipp32u*)val_32fc    , dst_16u_C1 );  io++;
    stat = LShift( src_16u_C3, (unsigned int*)val_32fc    , dst_16u_C3 );  io++;
    stat = LShift( src_16u_CA,       (Ipp32u*)val_32fc    , dst_16u_CA );  io++;
    stat = LShift( src_16u_C4,       (Ipp32u*)val_32fc    , dst_16u_C4 );  io++;

    stat = LShift( src_32s_C1,      *(Ipp32u*)val_32fc    , dst_32s_C1 );  io++;
    stat = LShift( src_32s_C3,       (Ipp32u*)val_32fc    , dst_32s_C3 );  io++;
    stat = LShift( src_32s_CA,       (Ipp32u*)val_32fc    , dst_32s_CA );  io++;
    stat = LShift( src_32s_C4,       (Ipp32u*)val_32fc    , dst_32s_C4 );  io++;

// img &= val  (I)
    stat = LShift(  *(Ipp32u*)val_32fc    , img_8u_C1 );   io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_8u_C3 );   io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_8u_CA );   io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_8u_C4 );   io++;

    stat = LShift(                     255, img_16u_C1 );  io++;
    stat = LShift(   (Ipp32u)(255/val_64f), img_16u_C1 );  io++;
    stat = LShift(   (Ipp32u*)(val_32fc+1), img_16u_C3 );  io++;
    stat = LShift(   (Ipp32u*)val_32fc+2  , img_16u_CA );  io++;
    stat = LShift( &((Ipp32u*)val_32fc)[2], img_16u_C4 );  io++;

    stat = LShift(  *(Ipp32u*)val_32fc    , img_32s_C1 );  io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_32s_C3 );  io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_32s_CA );  io++;
    stat = LShift(   (Ipp32u*)val_32fc    , img_32s_C4 );  io++;


// ---- NOT:

// dst = ~src  (NI)
    Not( src_8u_C1, dst_8u_C1 );  io++;
    Not( src_8u_C3, dst_8u_C3 );  io++;
    Not( src_8u_CA, dst_8u_CA );  io++;
    Not( src_8u_C4, dst_8u_C4 );  io++;

// img ~= img  (I)
    Not( img_8u_C1 );  io++;
    Not( img_8u_C3 );  io++;
    Not( img_8u_CA );  io++;
    Not( img_8u_C4 );  io++;


// ---- OR(OR_C):

// dst = src1 | src2  (NI)
    Or( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 );   io++;
    Or( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 );   io++;
    Or( src1_8u_CA , src2_8u_CA , dst_8u_CA );   io++;
    Or( src1_8u_C4 , src2_8u_C4 , dst_8u_C4 );   io++;

    Or( src1_16u_C1, src2_16u_C1, dst_16u_C1 );  io++;
    Or( src1_16u_C3, src2_16u_C3, dst_16u_C3 );  io++;
    Or( src1_16u_CA, src2_16u_CA, dst_16u_CA );  io++;
    Or( src1_16u_C4, src2_16u_C4, dst_16u_C4 );  io++;

    Or( src1_32s_C1, src2_32s_C1, dst_32s_C1 );  io++;
    Or( src1_32s_C3, src2_32s_C3, dst_32s_C3 );  io++;
    Or( src1_32s_CA, src2_32s_CA, dst_32s_CA );  io++;
    Or( src1_32s_C4, src2_32s_C4, dst_32s_C4 );  io++;

// img |= src  (I)
    Or( src_8u_C1 , img_8u_C1 );   io++;
    Or( src_8u_C3 , img_8u_C3 );   io++;
    Or( src_8u_CA , img_8u_CA );   io++;
    Or( src_8u_C4 , img_8u_C4 );   io++;

    Or( src_16u_C1, img_16u_C1 );  io++;
    Or( src_16u_C3, img_16u_C3 );  io++;
    Or( src_16u_CA, img_16u_CA );  io++;
    Or( src_16u_C4, img_16u_C4 );  io++;

    Or( src_32s_C1, img_32s_C1 );  io++;
    Or( src_32s_C3, img_32s_C3 );  io++;
    Or( src_32s_CA, img_32s_CA );  io++;
    Or( src_32s_C4, img_32s_C4 );  io++;

// dst = src | val  (NI)
    Or( src_8u_C1 ,   (Ipp8u)(255/val_64f), dst_8u_C1 );   io++;
    Or( src_8u_C3 ,   (Ipp8u*)(val_32fc+1), dst_8u_C3 );   io++;
    Or( src_8u_CA ,   (Ipp8u*)val_32fc+4  , dst_8u_CA );   io++;
    Or( src_8u_C4 , &((Ipp8u*)val_32fc)[4], dst_8u_C4 );   io++;

    Or( src_16u_C1, *(Ipp16u*)val_32fc    , dst_16u_C1 );  io++;
    Or( src_16u_C3,  (ushort*)val_32fc    , dst_16u_C3 );  io++;
    Or( src_16u_CA,  (Ipp16u*)val_32fc    , dst_16u_CA );  io++;
    Or( src_16u_C4,  (Ipp16u*)val_32fc    , dst_16u_C4 );  io++;

    Or( src_32s_C1, *(Ipp32s*)val_32fc    , dst_32s_C1 );  io++;
    Or( src_32s_C3,  (Ipp32s*)val_32fc    , dst_32s_C3 );  io++;
    Or( src_32s_CA,  (Ipp32s*)val_32fc    , dst_32s_CA );  io++;
    Or( src_32s_C4,  (Ipp32s*)val_32fc    , dst_32s_C4 );  io++;

// img |= val  (I)
    Or(   *(Ipp8u*)val_32fc    , img_8u_C1 );   io++;
    Or(    (Ipp8u*)val_32fc    , img_8u_C3 );   io++;
    Or(    (Ipp8u*)val_32fc    , img_8u_CA );   io++;
    Or(    (Ipp8u*)val_32fc    , img_8u_C4 );   io++;

    Or(                     255, img_16u_C1 );  io++;
    Or(   (Ipp16u)(255/val_64f), img_16u_C1 );  io++;
    Or(   (Ipp16u*)(val_32fc+1), img_16u_C3 );  io++;
    Or(   (Ipp16u*)val_32fc+2  , img_16u_CA );  io++;
    Or( &((Ipp16u*)val_32fc)[2], img_16u_C4 );  io++;

    Or(  *(Ipp32s*)val_32fc    , img_32s_C1 );  io++;
    Or(   (Ipp32s*)val_32fc    , img_32s_C3 );  io++;
    Or(   (Ipp32s*)val_32fc    , img_32s_CA );  io++;
    Or(   (Ipp32s*)val_32fc    , img_32s_C4 );  io++;


// ---- RSHIFT_C:

// dst = val_32u >> src  (NI)
    stat = RShift( src_8u_C1 ,      (Ipp32u)(255/val_64f), dst_8u_C1 );   io++;
    stat = RShift( src_8u_C3 ,      (Ipp32u*)(val_32fc+1), dst_8u_C3 );   io++;
    stat = RShift( src_8u_CA ,      (Ipp32u*)val_32fc+4  , dst_8u_CA );   io++;
    stat = RShift( src_8u_C4 ,    &((Ipp32u*)val_32fc)[4], dst_8u_C4 );   io++;

    stat = RShift( src_16u_C1,     *(Ipp32u*)val_32fc    , dst_16u_C1 );  io++;
    stat = RShift( src_16u_C3,(unsigned int*)val_32fc    , dst_16u_C3 );  io++;
    stat = RShift( src_16u_CA,      (Ipp32u*)val_32fc    , dst_16u_CA );  io++;
    stat = RShift( src_16u_C4,      (Ipp32u*)val_32fc    , dst_16u_C4 );  io++;

    stat = RShift( src_32s_C1,     *(Ipp32u*)val_32fc    , dst_32s_C1 );  io++;
    stat = RShift( src_32s_C3,      (Ipp32u*)val_32fc    , dst_32s_C3 );  io++;
    stat = RShift( src_32s_CA,      (Ipp32u*)val_32fc    , dst_32s_CA );  io++;
    stat = RShift( src_32s_C4,      (Ipp32u*)val_32fc    , dst_32s_C4 );  io++;

// img &= val  (I)
    stat = RShift(  *(Ipp32u*)val_32fc    , img_8u_C1 );   io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_8u_C3 );   io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_8u_CA );   io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_8u_C4 );   io++;

    stat = RShift(                     255, img_16u_C1 );  io++;
    stat = RShift(   (Ipp32u)(255/val_64f), img_16u_C1 );  io++;
    stat = RShift(   (Ipp32u*)(val_32fc+1), img_16u_C3 );  io++;
    stat = RShift(   (Ipp32u*)val_32fc+2  , img_16u_CA );  io++;
    stat = RShift( &((Ipp32u*)val_32fc)[2], img_16u_C4 );  io++;

    stat = RShift(  *(Ipp32u*)val_32fc    , img_32s_C1 );  io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_32s_C3 );  io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_32s_CA );  io++;
    stat = RShift(   (Ipp32u*)val_32fc    , img_32s_C4 );  io++;


// ---- XOR(XOR_C):

// dst = src1 ^ src2  (NI)
    Xor( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 );   io++;
    Xor( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 );   io++;
    Xor( src1_8u_CA , src2_8u_CA , dst_8u_CA );   io++;
    Xor( src1_8u_C4 , src2_8u_C4 , dst_8u_C4 );   io++;

    Xor( src1_16u_C1, src2_16u_C1, dst_16u_C1 );  io++;
    Xor( src1_16u_C3, src2_16u_C3, dst_16u_C3 );  io++;
    Xor( src1_16u_CA, src2_16u_CA, dst_16u_CA );  io++;
    Xor( src1_16u_C4, src2_16u_C4, dst_16u_C4 );  io++;

    Xor( src1_32s_C1, src2_32s_C1, dst_32s_C1 );  io++;
    Xor( src1_32s_C3, src2_32s_C3, dst_32s_C3 );  io++;
    Xor( src1_32s_CA, src2_32s_CA, dst_32s_CA );  io++;
    Xor( src1_32s_C4, src2_32s_C4, dst_32s_C4 );  io++;

// img ^= src  (I)
    Xor( src_8u_C1 , img_8u_C1 );   io++;
    Xor( src_8u_C3 , img_8u_C3 );   io++;
    Xor( src_8u_CA , img_8u_CA );   io++;
    Xor( src_8u_C4 , img_8u_C4 );   io++;

    Xor( src_16u_C1, img_16u_C1 );  io++;
    Xor( src_16u_C3, img_16u_C3 );  io++;
    Xor( src_16u_CA, img_16u_CA );  io++;
    Xor( src_16u_C4, img_16u_C4 );  io++;

    Xor( src_32s_C1, img_32s_C1 );  io++;
    Xor( src_32s_C3, img_32s_C3 );  io++;
    Xor( src_32s_CA, img_32s_CA );  io++;
    Xor( src_32s_C4, img_32s_C4 );  io++;

// dst = src ^ val  (NI)
    Xor( src_8u_C1 ,   (Ipp8u)(255/val_64f), dst_8u_C1 );   io++;
    Xor( src_8u_C3 ,   (Ipp8u*)(val_32fc+1), dst_8u_C3 );   io++;
    Xor( src_8u_CA ,   (Ipp8u*)val_32fc+4  , dst_8u_CA );   io++;
    Xor( src_8u_C4 , &((Ipp8u*)val_32fc)[4], dst_8u_C4 );   io++;

    Xor( src_16u_C1, *(Ipp16u*)val_32fc    , dst_16u_C1 );  io++;
    Xor( src_16u_C3,  (ushort*)val_32fc    , dst_16u_C3 );  io++;
    Xor( src_16u_CA,  (Ipp16u*)val_32fc    , dst_16u_CA );  io++;
    Xor( src_16u_C4,  (Ipp16u*)val_32fc    , dst_16u_C4 );  io++;

    Xor( src_32s_C1, *(Ipp32s*)val_32fc    , dst_32s_C1 );  io++;
    Xor( src_32s_C3,  (Ipp32s*)val_32fc    , dst_32s_C3 );  io++;
    Xor( src_32s_CA,  (Ipp32s*)val_32fc    , dst_32s_CA );  io++;
    Xor( src_32s_C4,  (Ipp32s*)val_32fc    , dst_32s_C4 );  io++;

// img ^= val  (I)
    Xor(  *(Ipp8u*)val_32fc     , img_8u_C1 );   io++;
    Xor(   (Ipp8u*)val_32fc     , img_8u_C3 );   io++;
    Xor(   (Ipp8u*)val_32fc     , img_8u_CA );   io++;
    Xor(   (Ipp8u*)val_32fc     , img_8u_C4 );   io++;

    Xor(                     255, img_16u_C1 );  io++;
    Xor(   (Ipp16u)(255/val_64f), img_16u_C1 );  io++;
    Xor(   (Ipp16u*)(val_32fc+1), img_16u_C3 );  io++;
    Xor(   (Ipp16u*)val_32fc+2  , img_16u_CA );  io++;
    Xor( &((Ipp16u*)val_32fc)[2], img_16u_C4 );  io++;

    Xor(  *(Ipp32s*)val_32fc    , img_32s_C1 );  io++;
    Xor(   (Ipp32s*)val_32fc    , img_32s_C3 );  io++;
    Xor(   (Ipp32s*)val_32fc    , img_32s_CA );  io++;
    Xor(   (Ipp32s*)val_32fc    , img_32s_C4 );  io++;


/* #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5
#5                                                                                                                   #5
#5                                       -= Alpha Composition (from #5) =-                                           #5
#5                                                                                                                   #5
#5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 #5 */

// ---- ALPHA_COMP:

// Composition using a-channel values:
// Pixel-order data
    stat = AlphaComp( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 , ippAlphaInPremul );    io++;
    stat = AlphaComp( src1_8u_CA , src2_8u_CA , dst_8u_CA , ippAlphaOver );        io++;

    stat = AlphaComp( src1_16u_C1, src2_16u_C1, dst_16u_C1, ippAlphaOutPremul );   io++;
    stat = AlphaComp( src1_16u_CA, src2_16u_CA, dst_16u_CA, ippAlphaXorPremul );   io++;

// Planar-order data (P)
    stat = AlphaComp( src1_8u_PA , src2_8u_PA,  dst_8u_PA , ippAlphaATopPremul );  io++;
    stat = AlphaComp( src1_16u_PA, src2_16u_PA, dst_16u_PA, ippAlphaATopPremul );  io++;

// Composition using constant values:
// Pixel-order data
    stat = AlphaComp( src1_8u_C1 , 0            , src2_8u_C1,  IPP_MAX_8S   , dst_8u_C1 , ippAlphaATop );        io++;
    stat = AlphaComp( src1_8u_C3 , IPP_MAX_8U   , src2_8u_C3,  IPP_MAX_16U  , dst_8u_C3 , ippAlphaATop );        io++;
    stat = AlphaComp( src1_8u_CA , IPP_MAX_8U   , src2_8u_CA,  0            , dst_8u_CA , ippAlphaATopPremul );  io++;
    stat = AlphaComp( src1_8u_C4 , IPP_MAX_8U/2 , src2_8u_C4,  IPP_MAX_16U/2, dst_8u_C4 , ippAlphaATopPremul );  io++;

    stat = AlphaComp( src1_16u_C1, 0            , src2_16u_C1, IPP_MAX_16U  , dst_16u_C1, ippAlphaATop );        io++;
    stat = AlphaComp( src1_16u_C3, IPP_MAX_16U  , src2_16u_C3, IPP_MAX_16U*2, dst_16u_C3, ippAlphaATop );        io++;
    stat = AlphaComp( src1_16u_CA, IPP_MAX_16U  , src2_16u_CA, 0            , dst_16u_CA, ippAlphaATopPremul );  io++;
    stat = AlphaComp( src1_16u_C4, IPP_MAX_16U/2, src2_16u_C4, IPP_MAX_16S/2, dst_16u_C4, ippAlphaATopPremul );  io++;

// Planar-order data (P)
    stat = AlphaComp( src1_8u_PA , IPP_MAX_8U/2 , src2_8u_PA,  IPP_MAX_16U/2, dst_8u_PA , ippAlphaATopPremul );  io++;
    stat = AlphaComp( src1_16u_PA, IPP_MAX_16U/2, src2_16u_PA, IPP_MAX_16S/2, dst_16u_PA, ippAlphaATopPremul );  io++;


// ---- ALPHA_PREMUL:

// dst(r,g,b,a) = src(r*a', g*a', b*a', a)  (NI)
// Pixel-order data
    AlphaPremul( src_8u_CA , dst_8u_CA );   io++;
    AlphaPremul( src_16u_CA, dst_16u_CA );  io++;

// Planar-order data (P)
    AlphaPremul( src_8u_PA , dst_8u_PA );   io++;
    AlphaPremul( src_16u_PA, dst_16u_PA );  io++;


// img(r,g,b,a) = img(r*a', g*a', b*a', a)  (I)
// Pixel-order data
    AlphaPremul( img_8u_CA );   io++;
    AlphaPremul( img_16u_CA );  io++;

// Planar-order data (P)
    AlphaPremul( img_8u_PA );   io++;
    AlphaPremul( img_16u_PA );  io++;


// dst(r[,...]) = src(r*a'[...])  (NI)
// Pixel-order data
    AlphaPremul( src_8u_C1 , IPP_MAX_8U/2             , dst_8u_C1 );   io++;
    AlphaPremul( src_8u_C1 , ((Ipp8u*)val_32fc)[7]    , dst_8u_C1 );   io++;
    AlphaPremul( src_8u_C1 , ((Ipp8u*)(val_32fc+1))[3], dst_8u_C1 );   io++;
    AlphaPremul( src_8u_C1 , (Ipp8u)val_64f           , dst_8u_C1 );   io++;
    AlphaPremul( src_8u_C3 , IPP_MAX_8U/2             , dst_8u_C3 );   io++;
    AlphaPremul( src_8u_CA , IPP_MAX_8U/2             , dst_8u_CA );   io++;
    AlphaPremul( src_8u_C4 , IPP_MAX_8U/2             , dst_8u_C4 );   io++;

    AlphaPremul( src_16u_C1, IPP_MAX_16U/2            , dst_16u_C1 );  io++;
    AlphaPremul( src_16u_C3, IPP_MAX_16U/2            , dst_16u_C3 );  io++;
    AlphaPremul( src_16u_CA, IPP_MAX_16U/2            , dst_16u_CA );  io++;
    AlphaPremul( src_16u_C4, IPP_MAX_16U/2            , dst_16u_C4 );  io++;

// Planar-order data (P)
    AlphaPremul( src_8u_PA , ((Ipp8u*)val_32fc)[5]    , dst_8u_PA );   io++;
    AlphaPremul( src_16u_PA, ((Ipp8u*)(val_32fc+1))[3], dst_16u_PA );  io++;

// img(r[,...]) = img(r*a'[...])  (I)
// Pixel-order data
    AlphaPremul( IPP_MAX_8U/2              , img_8u_C1 );   io++;
    AlphaPremul( ((Ipp8u*)val_32fc)[7]     , img_8u_C1 );   io++;
    AlphaPremul( ((Ipp8u*)(val_32fc+1))[3] , img_8u_C1 );   io++;
    AlphaPremul( (Ipp8u)val_64f            , img_8u_C1 );   io++;
    AlphaPremul( IPP_MAX_8U/2              , img_8u_C3 );   io++;
    AlphaPremul( IPP_MAX_8U/2              , img_8u_CA );   io++;
    AlphaPremul( IPP_MAX_8U/2              , img_8u_C4 );   io++;

    AlphaPremul( IPP_MAX_16U/2             , img_16u_C1 );  io++;
    AlphaPremul( IPP_MAX_16U/2             , img_16u_C3 );  io++;
    AlphaPremul( IPP_MAX_16U/2             , img_16u_CA );  io++;
    AlphaPremul( ((Ipp16u*)val_32fc)[3]    , img_16u_CA );  io++;
    AlphaPremul( ((Ipp16u*)(val_32fc+1))[1], img_16u_CA );  io++;
    AlphaPremul( (Ipp16u)val_64f           , img_16u_CA );  io++;
    AlphaPremul( IPP_MAX_16U/2             , img_16u_C4 );  io++;

// Planar-order data (P)
    AlphaPremul( ((Ipp8u*)val_32fc)[5]     , img_8u_PA );   io++;
    AlphaPremul( ((Ipp8u*)(val_32fc+1))[3] , img_16u_PA );  io++;


/* #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6
#6                        ___  _  _    _   __       ___  _   _  _ _  _  ___  __   ___  _   _  _  _                    #6
#6                       /   /  / /  /  / /_ /     /   /  /  /| / /  / /_   /_ / /_    / /  / /| /                    #6
#6                      /__  \_/ /__ \_/ /  \     /__  \_/ _/ |/  \_/ /__  /  \ ___/ _/  \_/ / |/                     #6
#6                                                                                                                   #6
#6                                        < Image Color Conversion (#6) >                                            #6
#6                                                                                                                   #6
#6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 */


/* #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6
#6                                                                                                                   #6
#6                                    -= Lookup Table Conversion (from #6) =-                                        #6
#6                                                                                                                   #6
#6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 #6 */

// Data preparing (otherwise [i.e. without init. of pLevels/pValues] one gets exception with access violation):

    for( i =0; i<L; i++ ){

        pLevels[i][0] =  0;
        pValues[i][0] = 20;

        for( j =1; j<NLEVELS_LUT; j++ ){

            pLevels[i][j] = pLevels[i][j-1] + 64;
            pValues[3][0] = pValues[i][j-1] + 50;
        }
    }

// ---- LUT  (NI):

    stat = LUT( src_8u_C1 , dst_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT( src_8u_C3 , dst_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( src_8u_CA , dst_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( src_8u_C4 , dst_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT( src_16s_C1, dst_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT( src_16s_C3, dst_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( src_16s_CA, dst_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( src_16s_C4, dst_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT( src_32f_C1, dst_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT( src_32f_C3, dst_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( src_32f_CA, dst_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( src_32f_C4, dst_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT  (I):

    stat = LUT( img_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT( img_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( img_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( img_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT( img_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT( img_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( img_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( img_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT( img_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT( img_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT( img_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT( img_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT__LINER  (NI):

    stat = LUT_Linear( src_8u_C1 , dst_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT_Linear( src_8u_C3 , dst_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_8u_CA , dst_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_8u_C4 , dst_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Linear( src_16s_C1, dst_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT_Linear( src_16s_C3, dst_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_16s_CA, dst_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_16s_C4, dst_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Linear( src_32f_C1, dst_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_32f_C3, dst_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_32f_CA, dst_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( src_32f_C4, dst_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT__LINER  (I):

    stat = LUT_Linear( img_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT_Linear( img_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Linear( img_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT_Linear( img_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Linear( img_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Linear( img_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT__CUBIC  (NI):

    stat = LUT_Cubic( src_8u_C1 , dst_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT_Cubic( src_8u_C3 , dst_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_8u_CA , dst_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_8u_C4 , dst_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Cubic( src_16s_C1, dst_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT_Cubic( src_16s_C3, dst_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_16s_CA, dst_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_16s_C4, dst_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Cubic( src_32f_C1, dst_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_32f_C3, dst_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_32f_CA, dst_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( src_32f_C4, dst_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT__CUBIC  (I):

    stat = LUT_Cubic( img_8u_C1 ,                   pValues[3] ,                   pLevels[3] ,  NLEVELS_LUT );     io++;
    stat = LUT_Cubic( img_8u_C3 , &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_8u_CA ,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_8u_C4 ,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Cubic( img_16s_C1,                   pValues[3] ,                   pLevels[3] ,  nLevels_LUT[3] );  io++;
    stat = LUT_Cubic( img_16s_C3, &((const Ipp32s**)pValues)[1], &((const Ipp32s**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_16s_CA,  ((const Ipp32s**)pValues)+1 ,  ((const Ipp32s**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_16s_C4,   (const Ipp32s**)pValues    ,   (const Ipp32s**)pLevels    ,  nLevels_LUT );     io++;

    stat = LUT_Cubic( img_32f_C1,  ((const Ipp32f**)pValues)[3],  ((const Ipp32f**)pLevels)[3],  nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_32f_C3, &((const Ipp32f**)pValues)[1], &((const Ipp32f**)pLevels)[1], &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_32f_CA,  ((const Ipp32f**)pValues)+1 ,  ((const Ipp32f**)pLevels)+1 , &nLevels_LUT[1] );  io++;
    stat = LUT_Cubic( img_32f_C4,   (const Ipp32f**)pValues    ,   (const Ipp32f**)pLevels    ,  nLevels_LUT );     io++;


// ---- LUT_PALETTE:

    stat = LUTPalette( src_8u_C1 , *(GIMG<Ipp32u>*)&dst_32s_C1, pTable, nBitSize );            io++;
    stat = LUTPalette( src_16u_C1,                  dst_8u_C1 , (Ipp8u*)pTable, nBitSize+2 );  io++;
    stat = LUTPalette( src_16u_C1, *(GIMG<Ipp32u>*)&dst_32s_C1, pTable, nBitSize );            io++;


/* #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7
#7         ___ _  _  __   ___  ___  _  _   _  _    _        _  _  _  _       ___  _  _  _   __    _   __   ___       #7
#7          /  /_ / /_ / /_   /_    /_ / /  / /   / \     /_ / /| / / \     /   /  / /|/|  /_ / /_ / /_ / /_         #7
#7         / _/  / /  \ /__  ___/ _/  /  \_/ /__ /_ /    /  / / |/ /_ /    /__  \_/ / | | /    /  / /  \ /__         #7
#7                                    _   __   ___  __    _  ___ _   _  _  _  ___                                    #7
#7                                  /  / /_ / /_   /_ / /_ /  /  / /  / /| / /_                                      #7
#7                                  \_/ /    /__  /  \ /  /  / _/  \_/ / |/ ___/                                     #7
#7                                                                                                                   #7
#7                                     < Threshold and Compare Operations (#7) >                                     #7
#7                                                                                                                   #7
#7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 */


/* #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7
#7                                                                                                                   #7
#7                                           -= Thresholding (from #7) =-                                            #7
#7                                                                                                                   #7
#7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 */

// THRESHOLD

// (NI)
    stat = Threshold( src_8u_C1 , dst_8u_C1 ,  IPP_MAX_8U/2      , ippCmpLess );       io++;
    stat = Threshold( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1]  , cmpGT );            io++;
    stat = Threshold( src_8u_CA , dst_8u_CA ,  THRESHOLD(8u)     , ippCmpEq );         io++;

    stat = Threshold( src_16s_C1, dst_16s_C1,  0                 , ippCmpGreater );    io++;
    stat = Threshold( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1] , cmpLT );            io++;
    stat = Threshold( src_16s_CA, dst_16s_CA,  THRESHOLD(16s)    , ippCmpLessEq );     io++;

    stat = Threshold( src_32f_C1, dst_32f_C1, (Ipp32f)IPP_MAX_16U, ippCmpGreater );    io++;
    stat = Threshold( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1] , cmpGT );            io++;
    stat = Threshold( src_32f_CA, dst_32f_CA,  THRESHOLD(32f)    , ippCmpGreaterEq );  io++;

// (I)
    stat = Threshold( img_8u_C1 ,  IPP_MAX_8U/2      , ippCmpLess );       io++;
    stat = Threshold( img_8u_C3 , &THRESHOLD(8u)[1]  , cmpGT );            io++;
    stat = Threshold( img_8u_CA ,  THRESHOLD(8u)     , ippCmpEq );         io++;

    stat = Threshold( img_16s_C1,  0                 , ippCmpGreater );    io++;
    stat = Threshold( img_16s_C3, &THRESHOLD(16s)[1] , cmpLT );            io++;
    stat = Threshold( img_16s_CA,  THRESHOLD(16s)    , ippCmpLessEq );     io++;

    stat = Threshold( img_32f_C1, (Ipp32f)IPP_MAX_16U, ippCmpGreater );    io++;
    stat = Threshold( img_32f_C3, &THRESHOLD(32f)[1] , cmpGT );            io++;
    stat = Threshold( img_32f_CA,  THRESHOLD(32f)    , ippCmpGreaterEq );  io++;


// THRESHOLD__GT

// (NI)
    stat = Threshold_GT( src_8u_C1 , dst_8u_C1 , IPP_MAX_8U/2  );        io++;
    stat = Threshold_GT( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1] );    io++;
    stat = Threshold_GT( src_8u_CA , dst_8u_CA ,  THRESHOLD(8u) );       io++;

    stat = Threshold_GT( src_16s_C1, dst_16s_C1,  0  );                  io++;
    stat = Threshold_GT( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1] );   io++;
    stat = Threshold_GT( src_16s_CA, dst_16s_CA,  THRESHOLD(16s) );      io++;

    stat = Threshold_GT( src_32f_C1, dst_32f_C1, (Ipp32f)IPP_MAX_16U );  io++;
    stat = Threshold_GT( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1] );   io++;
    stat = Threshold_GT( src_32f_CA, dst_32f_CA,  THRESHOLD(32f) );      io++;

// (I)
    stat = Threshold_GT( img_8u_C1 ,  IPP_MAX_8U/2 );       io++;
    stat = Threshold_GT( img_8u_C3 , &THRESHOLD(8u)[1] );   io++;
    stat = Threshold_GT( img_8u_CA ,  THRESHOLD(8u) );      io++;

    stat = Threshold_GT( img_16s_C1,  0  );                 io++;
    stat = Threshold_GT( img_16s_C3, &THRESHOLD(16s)[1] );  io++;
    stat = Threshold_GT( img_16s_CA,  THRESHOLD(16s)  );    io++;

    stat = Threshold_GT( img_32f_C1, (Ipp32f)IPP_MAX_16U ); io++;
    stat = Threshold_GT( img_32f_C3, &THRESHOLD(32f)[1] );  io++;
    stat = Threshold_GT( img_32f_CA,  THRESHOLD(32f) );     io++;


// THRESHOLD__LT

// (NI)
    stat = Threshold_LT( src_8u_C1 , dst_8u_C1 ,  IPP_MAX_8U/2  );        io++;
    stat = Threshold_LT( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1] );     io++;
    stat = Threshold_LT( src_8u_CA , dst_8u_CA ,  THRESHOLD(8u) );        io++;

    stat = Threshold_LT( src_16s_C1, dst_16s_C1,  0  );                   io++;
    stat = Threshold_LT( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1] );    io++;
    stat = Threshold_LT( src_16s_CA, dst_16s_CA,  THRESHOLD(16s)  );      io++;

    stat = Threshold_LT( src_32f_C1, dst_32f_C1,  (Ipp32f)IPP_MAX_16U );  io++;
    stat = Threshold_LT( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1] );    io++;
    stat = Threshold_LT( src_32f_CA, dst_32f_CA,  THRESHOLD(32f) );       io++;

// (I)
    stat = Threshold_LT( img_8u_C1 ,  IPP_MAX_8U/2 );         io++;
    stat = Threshold_LT( img_8u_C3 , &THRESHOLD(8u)[1] );     io++;
    stat = Threshold_LT( img_8u_CA ,  THRESHOLD(8u) );        io++;

    stat = Threshold_LT( img_16s_C1,  0  );                   io++;
    stat = Threshold_LT( img_16s_C3, &THRESHOLD(16s)[1] );    io++;
    stat = Threshold_LT( img_16s_CA,  THRESHOLD(16s)  );      io++;

    stat = Threshold_LT( img_32f_C1,  (Ipp32f)IPP_MAX_16U );  io++;
    stat = Threshold_LT( img_32f_C3, &THRESHOLD(32f)[1] );    io++;
    stat = Threshold_LT( img_32f_CA,  THRESHOLD(32f) );       io++;


// THRESHOLD__VAL

// (NI)
    stat = Threshold_Val( src_8u_C1 , dst_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S       , ippCmpLess );       io++;
    stat = Threshold_Val( src_8u_C1 , dst_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S)      , ippCmpLess );       io++;
    stat = Threshold_Val( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u)  , cmpGT );            io++;
    stat = Threshold_Val( src_8u_CA , dst_8u_CA , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(4,8u)  , ippCmpEq );         io++;

    stat = Threshold_Val( src_16s_C1, dst_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S)     , ippCmpGreater );    io++;
    stat = Threshold_Val( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s), cmpLT );            io++;
    stat = Threshold_Val( src_16s_CA, dst_16s_CA,  THRESHOLD(16s)      ,  PVAL(16s)+LAST(4,16s), ippCmpLessEq );     io++;

    stat = Threshold_Val( src_32f_C1, dst_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S)     , ippCmpGreater );    io++;
    stat = Threshold_Val( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f), cmpGT );            io++;
    stat = Threshold_Val( src_32f_CA, dst_32f_CA,  THRESHOLD(32f)      ,  PVAL(32f)+LAST(4,32f), ippCmpGreaterEq );  io++;

// (I)
    stat = Threshold_Val( img_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S      , ippCmpLess );       io++;
    stat = Threshold_Val( img_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S)     , ippCmpLess );       io++;
    stat = Threshold_Val( img_8u_C3 , &THRESHOLD(8u)[1]    , PVAL(8u)+LAST(3,8u)  , cmpGT );            io++;
    stat = Threshold_Val( img_8u_CA ,  THRESHOLD(8u)       , PVAL(8u)+LAST(4,8u)  , ippCmpEq );         io++;

    stat = Threshold_Val( img_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S)    , ippCmpGreater );    io++;
    stat = Threshold_Val( img_16s_C3, &THRESHOLD(16s)[1]   , PVAL(16s)+LAST(3,16s), cmpLT );            io++;
    stat = Threshold_Val( img_16s_CA,  THRESHOLD(16s)      , PVAL(16s)+LAST(4,16s), ippCmpLessEq );     io++;

    stat = Threshold_Val( img_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S)    , ippCmpGreater );    io++;
    stat = Threshold_Val( img_32f_C3, &THRESHOLD(32f)[1]   , PVAL(32f)+LAST(3,32f), cmpGT );            io++;
    stat = Threshold_Val( img_32f_CA,  THRESHOLD(32f)      , PVAL(32f)+LAST(4,32f), ippCmpGreaterEq );  io++;


// THRESHOLD__GTVAL

// (NI)
    stat = Threshold_GTVal( src_8u_C1 , dst_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S );         io++;
    stat = Threshold_GTVal( src_8u_C1 , dst_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_GTVal( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_GTVal( src_8u_CA , dst_8u_CA , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_GTVal( src_8u_C4 , dst_8u_C4 ,  THRESHOLD(8u)       ,  PVAL(8u)+LAST(4,8u) );    io++;

    stat = Threshold_GTVal( src_16s_C1, dst_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_GTVal( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_GTVal( src_16s_CA, dst_16s_CA, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_GTVal( src_16s_C4, dst_16s_C4,  THRESHOLD(16s)      ,  PVAL(16s)+LAST(4,16s) );  io++;

    stat = Threshold_GTVal( src_32f_C1, dst_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_GTVal( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_GTVal( src_32f_CA, dst_32f_CA, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_GTVal( src_32f_C4, dst_32f_C4,  THRESHOLD(32f)      ,  PVAL(32f)+LAST(4,32f) );  io++;

// (I)
    stat = Threshold_GTVal( img_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S );         io++;
    stat = Threshold_GTVal( img_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_GTVal( img_8u_C3 , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_GTVal( img_8u_CA , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_GTVal( img_8u_C4 ,  THRESHOLD(8u)       ,  PVAL(8u)+LAST(4,8u) );    io++;

    stat = Threshold_GTVal( img_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_GTVal( img_16s_C3, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_GTVal( img_16s_CA, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_GTVal( img_16s_C4,  THRESHOLD(16s)      ,  PVAL(16s)+LAST(4,16s) );  io++;

    stat = Threshold_GTVal( img_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_GTVal( img_32f_C3, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_GTVal( img_32f_CA, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_GTVal( img_32f_C4,  THRESHOLD(32f)      ,  PVAL(32f)+LAST(4,32f) );  io++;


// THRESHOLD__LTVAL

// (NI)
    stat = Threshold_LTVal( src_8u_C1 , dst_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S );         io++;
    stat = Threshold_LTVal( src_8u_C1 , dst_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_LTVal( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_LTVal( src_8u_CA , dst_8u_CA , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_LTVal( src_8u_C4 , dst_8u_C4 ,  THRESHOLD(8u)       ,  PVAL(8u)+LAST(4,8u) );    io++;

    stat = Threshold_LTVal( src_16s_C1, dst_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_LTVal( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_LTVal( src_16s_CA, dst_16s_CA, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_LTVal( src_16s_C4, dst_16s_C4,  THRESHOLD(16s)      ,  PVAL(16s)+LAST(4,16s) );  io++;

    stat = Threshold_LTVal( src_32f_C1, dst_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_LTVal( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTVal( src_32f_CA, dst_32f_CA, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTVal( src_32f_C4, dst_32f_C4,  THRESHOLD(32f)      ,  PVAL(32f)+LAST(4,32f) );  io++;

// (I)
    stat = Threshold_LTVal( img_8u_C1 ,         IPP_MAX_8U/2 ,         VAL_32S );         io++;
    stat = Threshold_LTVal( img_8u_C1 , (Ipp8u)(IPP_MAX_8U/2), (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_LTVal( img_8u_C3 , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_LTVal( img_8u_CA , &THRESHOLD(8u)[1]    ,  PVAL(8u)+LAST(3,8u) );    io++;
    stat = Threshold_LTVal( img_8u_C4 ,  THRESHOLD(8u)       ,  PVAL(8u)+LAST(4,8u) );    io++;

    stat = Threshold_LTVal( img_16s_C1, (Ipp16s)0            , (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_LTVal( img_16s_C3, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_LTVal( img_16s_CA, &THRESHOLD(16s)[1]   ,  PVAL(16s)+LAST(3,16s) );  io++;
    stat = Threshold_LTVal( img_16s_C4,  THRESHOLD(16s)      ,  PVAL(16s)+LAST(4,16s) );  io++;

    stat = Threshold_LTVal( img_32f_C1, (Ipp32f)IPP_MAX_16U  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_LTVal( img_32f_C3, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTVal( img_32f_CA, &THRESHOLD(32f)[1]   ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTVal( img_32f_C4,  THRESHOLD(32f)      ,  PVAL(32f)+LAST(4,32f) );  io++;


// THRESHOLD__LTVAL_GTVAL

// (NI)
    stat = Threshold_LTValGTVal( src_8u_C1 , dst_8u_C1 ,         IPP_MAX_8U/4   ,         VAL_32S       ,         IPP_MAX_8U/2   ,         VAL_32S );         io++;
    stat = Threshold_LTValGTVal( src_8u_C1 , dst_8u_C1 , (Ipp8u)(IPP_MAX_8U/4)  , (Ipp8u)(VAL_32S)      , (Ipp8u)(IPP_MAX_8U/2)  , (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_LTValGTVal( src_8u_C3 , dst_8u_C3 , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(3,8u)  , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(7,8u) );    io++;
    stat = Threshold_LTValGTVal( src_8u_CA , dst_8u_CA , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(3,8u)  , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(8,8u) );    io++;

    stat = Threshold_LTValGTVal( src_16s_C1, dst_16s_C1, (Ipp16s)(IPP_MIN_16S/2), (Ipp16s)(VAL_32S)     , (Ipp16s)(IPP_MAX_16S/2), (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_LTValGTVal( src_16s_C3, dst_16s_C3, &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(3,16s), &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(7,16s) );  io++;
    stat = Threshold_LTValGTVal( src_16s_CA, dst_16s_CA, &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(3,16s), &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(8,16s) );  io++;

    stat = Threshold_LTValGTVal( src_32f_C1, dst_32f_C1, (Ipp32f)0              , (Ipp32f)(VAL_32S)     , (Ipp32f)IPP_MAX_16U/2  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_LTValGTVal( src_32f_C3, dst_32f_C3, &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f), &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTValGTVal( src_32f_CA, dst_32f_CA, &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f), &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(4,32f) );  io++;

// (I)
    stat = Threshold_LTValGTVal( img_8u_C1 ,         IPP_MAX_8U/4   ,         VAL_32S       ,         IPP_MAX_8U/2   ,         VAL_32S );         io++;
    stat = Threshold_LTValGTVal( img_8u_C1 , (Ipp8u)(IPP_MAX_8U/4)  , (Ipp8u)(VAL_32S)      , (Ipp8u)(IPP_MAX_8U/2)  , (Ipp8u)(VAL_32S) );        io++;
    stat = Threshold_LTValGTVal( img_8u_C3 , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(3,8u)  , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(7,8u) );    io++;
    stat = Threshold_LTValGTVal( img_8u_CA , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(3,8u)  , &THRESHOLD(8u)[1]      ,  PVAL(8u)+LAST(8,8u) );    io++;

    stat = Threshold_LTValGTVal( img_16s_C1, (Ipp16s)(IPP_MIN_16S/2), (Ipp16s)(VAL_32S)     , (Ipp16s)(IPP_MAX_16S/2), (Ipp16s)(VAL_32S) );       io++;
    stat = Threshold_LTValGTVal( img_16s_C3, &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(3,16s), &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(7,16s) );  io++;
    stat = Threshold_LTValGTVal( img_16s_CA, &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(3,16s), &THRESHOLD(16s)[1]     ,  PVAL(16s)+LAST(8,16s) );  io++;

    stat = Threshold_LTValGTVal( img_32f_C1, (Ipp32f)0              , (Ipp32f)(VAL_32S)     , (Ipp32f)IPP_MAX_16U/2  , (Ipp32f)(VAL_32S) );       io++;
    stat = Threshold_LTValGTVal( img_32f_C3, &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f), &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f) );  io++;
    stat = Threshold_LTValGTVal( img_32f_CA, &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(3,32f), &THRESHOLD(32f)[1]     ,  PVAL(32f)+LAST(4,32f) );  io++;


/* #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7
#7                                                                                                                   #7
#7                                        -= Compare Operations (from #7) =-                                         #7
#7                                                                                                                   #7
#7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 #7 */

// COMPARE

    stat = Compare( src1_8u_C1  , src2_8u_C1  , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src1_8u_C3  , src2_8u_C3  , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src1_8u_CA  , src2_8u_CA  , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src1_8u_C4  , src2_8u_C4  , dst_8u_C4, ippCmpGreater );  io++;

    stat = Compare( src1_16s_C1 , src2_16s_C1 , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src1_16s_C3 , src2_16s_C3 , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src1_16s_CA , src2_16s_CA , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src1_16s_C4 , src2_16s_C4 , dst_8u_C4, ippCmpGreater );  io++;

    stat = Compare( src1_32f_C1 , src2_32f_C1 , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src1_32f_C3 , src2_32f_C3 , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src1_32f_CA , src2_32f_CA , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src1_32f_C4 , src2_32f_C4 , dst_8u_C4, ippCmpGreater );  io++;


// COMPARE_C

    stat = Compare( src_8u_C1 ,  (Ipp8u)(255/val_64f) , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src_8u_C3 ,  (Ipp8u*)val_32fc     , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src_8u_CA ,  (Ipp8u*)val_32fc+4   , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src_8u_C4 , &((Ipp8u*)val_32fc)[4], dst_8u_C4, ippCmpGreater );  io++;

    stat = Compare( src_16s_C1, *(Ipp16s*)val_32fc    , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src_16s_C3,   (short*)val_32fc    , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src_16s_CA,  (Ipp16s*)val_32fc    , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src_16s_C4,  (Ipp16s*)val_32fc    , dst_8u_C4, ippCmpGreater );  io++;

    stat = Compare( src_32f_C1 , *(Ipp32f*)val_32fc   , dst_8u_C1, ippCmpLess );     io++;
    stat = Compare( src_32f_C3 ,  (Ipp32f*)val_32fc   , dst_8u_C3, cmpGT );          io++;
    stat = Compare( src_32f_CA ,  (Ipp32f*)val_32fc   , dst_8u_CA, ippCmpEq );       io++;
    stat = Compare( src_32f_C4 ,  (Ipp32f*)val_32fc   , dst_8u_C4, ippCmpGreater );  io++;


// COMPARE_EQUAL_EPS

    stat = CompareEqualEps( src1_32f_C1 , src2_32f_C1 , dst_8u_C1, .1f );                     io++;
    stat = CompareEqualEps( src1_32f_C3 , src2_32f_C3 , dst_8u_C3, noise[0] );                io++;
    stat = CompareEqualEps( src1_32f_CA , src2_32f_CA , dst_8u_CA,   (Ipp32f)val_64f );       io++;
    stat = CompareEqualEps( src1_32f_C4 , src2_32f_C4 , dst_8u_C4, ((Ipp32f*)val_32fc)[7] );  io++;


// COMPARE_EQUAL_EPS_C

    stat = CompareEqualEps( src_32f_C1 , *(Ipp32f*)val_32fc   , dst_8u_C1, .1f );                           io++;
    stat = CompareEqualEps( src_32f_C3 ,  (Ipp32f*)val_32fc   , dst_8u_C3, noise[0] );                      io++;
    stat = CompareEqualEps( src_32f_CA ,  (Ipp32f*)val_32fc   , dst_8u_CA,  (Ipp32f)val_64f );              io++;
    stat = CompareEqualEps( src_32f_C4 ,  (Ipp32f*)val_32fc   , dst_8u_C4, ((Ipp32f*)&(val_32fc[3]))[1] );  io++;


/* #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8
#5    _  _   _   __   __  _  _   _  _    _   ___  _  ___   _  _         _   __   ___  __    _  ___ _   _  _  _  ___  #5
#5    /|/| /  / /_ / /_ / /_ / /  / /  /  / /     / /    /_ / /       /  / /_ / /_   /_ / /_ /  /  / /  / /| / /_    #5
#5   / | | \_/ /  \ /   _/  /  \_/ /__ \_/ /__- _/ /__  /  / /__      \_/ /    /__  /  \ /  /  / _/  \_/ / |/ ___/   #5
#8                                                                                                                   #8
#8                                       < Morphological Operations (#8) >                                           #8
#8                                                                                                                   #8
#8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 #8 */

// ---- [DILATE|ERODE][3X3|]:

// Images preparing:
    kern.init(3,3);  // the same for Dilate/Erode for simplicity

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// Dilate3x3 (NI):
    stat = Dilate3x3( src_8u_C1 , dst_8u_C1 );   io++;
    stat = Dilate3x3( src_8u_C3 , dst_8u_C3 );   io++;
    stat = Dilate3x3( src_8u_CA , dst_8u_CA );   io++;
    stat = Dilate3x3( src_8u_C4 , dst_8u_C4 );   io++;

    stat = Dilate3x3( src_32f_C1, dst_32f_C1 );  io++;
    stat = Dilate3x3( src_32f_C3, dst_32f_C3 );  io++;
    stat = Dilate3x3( src_32f_CA, dst_32f_CA );  io++;
    stat = Dilate3x3( src_32f_C4, dst_32f_C4 );  io++;

// Erode3x3 (NI):
    stat = Erode3x3( src_8u_C1 , dst_8u_C1 );   io++;
    stat = Erode3x3( src_8u_C3 , dst_8u_C3 );   io++;
    stat = Erode3x3( src_8u_CA , dst_8u_CA );   io++;
    stat = Erode3x3( src_8u_C4 , dst_8u_C4 );   io++;

    stat = Erode3x3( src_32f_C1, dst_32f_C1 );  io++;
    stat = Erode3x3( src_32f_C3, dst_32f_C3 );  io++;
    stat = Erode3x3( src_32f_CA, dst_32f_CA );  io++;
    stat = Erode3x3( src_32f_C4, dst_32f_C4 );  io++;

// Dilate3x3 (I) (below 'src_...' images are in-place ones for simplicity):
    stat = Dilate3x3( src_8u_C1 );   io++;
    stat = Dilate3x3( src_8u_C3 );   io++;
    stat = Dilate3x3( src_8u_CA );   io++;
    stat = Dilate3x3( src_8u_C4 );   io++;

    stat = Dilate3x3( src_32f_C1 );  io++;
    stat = Dilate3x3( src_32f_C3 );  io++;
    stat = Dilate3x3( src_32f_CA );  io++;
    stat = Dilate3x3( src_32f_C4 );  io++;

// Erode3x3 (I) (below 'src_...' images are in-place ones for simplicity):
    stat = Erode3x3( src_8u_C1 );   io++;
    stat = Erode3x3( src_8u_C3 );   io++;
    stat = Erode3x3( src_8u_CA );   io++;
    stat = Erode3x3( src_8u_C4 );   io++;

    stat = Erode3x3( src_32f_C1 );  io++;
    stat = Erode3x3( src_32f_C3 );  io++;
    stat = Erode3x3( src_32f_CA );  io++;
    stat = Erode3x3( src_32f_C4 );  io++;

    pKern_32f = new Ipp32f[kern.size.width*kern.size.height];

// Dilate (NI):
    stat = Dilate( src_8u_C1 , dst_8u_C1 , MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_8u_C3 , dst_8u_C3 , MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_8u_CA , dst_8u_CA , MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_8u_C4 , dst_8u_C4 , MASK, kern.size, kern.anchor );  io++;

    stat = Dilate( src_32f_C1, dst_32f_C1, MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_32f_C3, dst_32f_C3, MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_32f_CA, dst_32f_CA, MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_32f_C4, dst_32f_C4, MASK, kern.size, kern.anchor );  io++;

// Erode (NI):
    stat = Erode( src_8u_C1 , dst_8u_C1 , MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_8u_C3 , dst_8u_C3 , MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_8u_CA , dst_8u_CA , MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_8u_C4 , dst_8u_C4 , MASK, kern.size, kern.anchor );  io++;

    stat = Erode( src_32f_C1, dst_32f_C1, MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_32f_C3, dst_32f_C3, MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_32f_CA, dst_32f_CA, MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_32f_C4, dst_32f_C4, MASK, kern.size, kern.anchor );  io++;

// Dilate (I) (below 'src_...' images are in-place ones for simplicity):
    stat = Dilate( src_8u_C1 , MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_8u_C3 , MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_8u_CA , MASK, kern.size, kern.anchor );  io++;

    stat = Dilate( src_32f_C1, MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_32f_C3, MASK, kern.size, kern.anchor );  io++;
    stat = Dilate( src_32f_CA, MASK, kern.size, kern.anchor );  io++;

// Erode (I) (below 'src_...' images are in-place ones for simplicity):
    stat = Erode( src_8u_C1 , MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_8u_C3 , MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_8u_CA , MASK, kern.size, kern.anchor );  io++;

    stat = Erode( src_32f_C1, MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_32f_C3, MASK, kern.size, kern.anchor );  io++;
    stat = Erode( src_32f_CA, MASK, kern.size, kern.anchor );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );

    delete[] pKern_32f;

/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                 ___  _ _   ___  ___  __   _  _  _  ___      ___ _  _  _  _  ___ ___ _   _  _  _  ___              #9
#9                /_    / /    /  /_   /_ /  /  /| / /        /_   /  /  /| / /     /  / /  / /| / /_                #9
#9               /    _/ /__  /  /__  /  \ _/ _/ |/ /__-     /     \_/ _/ |/ /__   / _/  \_/ / |/ ___/               #9
#9                                                                                                                   #9
#9                                         < Filtering Functions (#9) >                                              #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// ---- FILTER_[BOX|MAX|MIN]:

// Images preparing:
    kern.init(3,3);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterBox (NI):
    stat = FilterBox( src_8u_C1 , dst_8u_C1 , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_C3 , dst_8u_C3 , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_CA , dst_8u_CA , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_C4 , dst_8u_C4 , kern.size, kern.anchor );  io++;

    stat = FilterBox( src_16s_C1, dst_16s_C1, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_C3, dst_16s_C3, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_CA, dst_16s_CA, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_C4, dst_16s_C4, kern.size, kern.anchor );  io++;

    stat = FilterBox( src_32f_C1, dst_32f_C1, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_C3, dst_32f_C3, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_CA, dst_32f_CA, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_C4, dst_32f_C4, kern.size, kern.anchor );  io++;

// FilterBox (I) (below 'src_...' images are in-place ones for simplicity):
    stat = FilterBox( src_8u_C1 , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_C3 , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_CA , kern.size, kern.anchor );  io++;
    stat = FilterBox( src_8u_C4 , kern.size, kern.anchor );  io++;

    stat = FilterBox( src_16s_C1, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_C3, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_CA, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_16s_C4, kern.size, kern.anchor );  io++;

    stat = FilterBox( src_32f_C1, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_C3, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_CA, kern.size, kern.anchor );  io++;
    stat = FilterBox( src_32f_C4, kern.size, kern.anchor );  io++;

// FilterMax:
    stat = FilterMax( src_8u_C1 , dst_8u_C1 , kern.size, kern.anchor );  io++;
    stat = FilterMax( src_8u_C3 , dst_8u_C3 , kern.size, kern.anchor );  io++;
    stat = FilterMax( src_8u_CA , dst_8u_CA , kern.size, kern.anchor );  io++;
    stat = FilterMax( src_8u_C4 , dst_8u_C4 , kern.size, kern.anchor );  io++;

    stat = FilterMax( src_16s_C1, dst_16s_C1, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_16s_C3, dst_16s_C3, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_16s_CA, dst_16s_CA, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_16s_C4, dst_16s_C4, kern.size, kern.anchor );  io++;

    stat = FilterMax( src_32f_C1, dst_32f_C1, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_32f_C3, dst_32f_C3, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_32f_CA, dst_32f_CA, kern.size, kern.anchor );  io++;
    stat = FilterMax( src_32f_C4, dst_32f_C4, kern.size, kern.anchor );  io++;

// FilterMin:
    stat = FilterMin( src_8u_C1 , dst_8u_C1 , kern.size, kern.anchor );  io++;
    stat = FilterMin( src_8u_C3 , dst_8u_C3 , kern.size, kern.anchor );  io++;
    stat = FilterMin( src_8u_CA , dst_8u_CA , kern.size, kern.anchor );  io++;
    stat = FilterMin( src_8u_C4 , dst_8u_C4 , kern.size, kern.anchor );  io++;

    stat = FilterMin( src_16s_C1, dst_16s_C1, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_16s_C3, dst_16s_C3, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_16s_CA, dst_16s_CA, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_16s_C4, dst_16s_C4, kern.size, kern.anchor );  io++;

    stat = FilterMin( src_32f_C1, dst_32f_C1, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_32f_C3, dst_32f_C3, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_32f_CA, dst_32f_CA, kern.size, kern.anchor );  io++;
    stat = FilterMin( src_32f_C4, dst_32f_C4, kern.size, kern.anchor );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                     -= Fixed Filtering Functions (from #9) =-                                     #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// =======================================   FIXED FILTERS with 3x3 mask size   =======================================

// ---- FILTER_[ [PREWITT|SCHARR|SOBEL][HORIZ|VERT] | ROBERTS[UP|DOWN] | SHARPEN ]:

// Images preparing:
    kern.init(3,3);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterPrewittHoriz:
    stat = FilterPrewittHoriz( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterPrewittHoriz( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterPrewittHoriz( src_8u_CA , dst_8u_CA  );  io++;
    stat = FilterPrewittHoriz( src_8u_C4 , dst_8u_C4  );  io++;

    stat = FilterPrewittHoriz( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterPrewittHoriz( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterPrewittHoriz( src_16s_CA, dst_16s_CA );  io++;
    stat = FilterPrewittHoriz( src_16s_C4, dst_16s_C4 );  io++;

    stat = FilterPrewittHoriz( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterPrewittHoriz( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterPrewittHoriz( src_32f_CA, dst_32f_CA );  io++;
    stat = FilterPrewittHoriz( src_32f_C4, dst_32f_C4 );  io++;

// FilterPrewittVert:
    stat = FilterPrewittVert( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterPrewittVert( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterPrewittVert( src_8u_CA , dst_8u_CA  );  io++;
    stat = FilterPrewittVert( src_8u_C4 , dst_8u_C4  );  io++;

    stat = FilterPrewittVert( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterPrewittVert( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterPrewittVert( src_16s_CA, dst_16s_CA );  io++;
    stat = FilterPrewittVert( src_16s_C4, dst_16s_C4 );  io++;

    stat = FilterPrewittVert( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterPrewittVert( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterPrewittVert( src_32f_CA, dst_32f_CA );  io++;
    stat = FilterPrewittVert( src_32f_C4, dst_32f_C4 );  io++;

// FilterRobertsDown:
    stat = FilterRobertsDown( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterRobertsDown( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterRobertsDown( src_8u_CA , dst_8u_CA  );  io++;

    stat = FilterRobertsDown( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterRobertsDown( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterRobertsDown( src_16s_CA, dst_16s_CA );  io++;

    stat = FilterRobertsDown( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterRobertsDown( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterRobertsDown( src_32f_CA, dst_32f_CA );  io++;

// FilterRobertsDown:
    stat = FilterRobertsUp( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterRobertsUp( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterRobertsUp( src_8u_CA , dst_8u_CA  );  io++;

    stat = FilterRobertsUp( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterRobertsUp( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterRobertsUp( src_16s_CA, dst_16s_CA );  io++;

    stat = FilterRobertsUp( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterRobertsUp( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterRobertsUp( src_32f_CA, dst_32f_CA );  io++;

// FilterScharrHoriz:
    stat = FilterScharrHoriz(                 src_8u_C1 , dst_16s_C1 );  io++;
    stat = FilterScharrHoriz( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1 );  io++;
    stat = FilterScharrHoriz(                 src_32f_C1, dst_32f_C1 );  io++;

// FilterScharrVert:
    stat = FilterScharrVert(                 src_8u_C1 , dst_16s_C1 );   io++;
    stat = FilterScharrVert( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1 );   io++;
    stat = FilterScharrVert(                 src_32f_C1, dst_32f_C1 );   io++;

// FilterSharpen:
    stat = FilterSharpen( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterSharpen( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterSharpen( src_8u_CA , dst_8u_CA  );  io++;
    stat = FilterSharpen( src_8u_C4 , dst_8u_C4  );  io++;

    stat = FilterSharpen( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterSharpen( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterSharpen( src_16s_CA, dst_16s_CA );  io++;
    stat = FilterSharpen( src_16s_C4, dst_16s_C4 );  io++;

    stat = FilterSharpen( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterSharpen( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterSharpen( src_32f_CA, dst_32f_CA );  io++;
    stat = FilterSharpen( src_32f_C4, dst_32f_C4 );  io++;

// FilterSobelHoriz:
    stat = FilterSobelHoriz( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterSobelHoriz( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterSobelHoriz( src_8u_CA , dst_8u_CA  );  io++;
    stat = FilterSobelHoriz( src_8u_C4 , dst_8u_C4  );  io++;

    stat = FilterSobelHoriz( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterSobelHoriz( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterSobelHoriz( src_16s_CA, dst_16s_CA );  io++;
    stat = FilterSobelHoriz( src_16s_C4, dst_16s_C4 );  io++;

    stat = FilterSobelHoriz( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterSobelHoriz( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterSobelHoriz( src_32f_CA, dst_32f_CA );  io++;
    stat = FilterSobelHoriz( src_32f_C4, dst_32f_C4 );  io++;

// FilterSobelVert:
    stat = FilterSobelVert( src_8u_C1 , dst_8u_C1  );  io++;
    stat = FilterSobelVert( src_8u_C3 , dst_8u_C3  );  io++;
    stat = FilterSobelVert( src_8u_CA , dst_8u_CA  );  io++;
    stat = FilterSobelVert( src_8u_C4 , dst_8u_C4  );  io++;

    stat = FilterSobelVert( src_16s_C1, dst_16s_C1 );  io++;
    stat = FilterSobelVert( src_16s_C3, dst_16s_C3 );  io++;
    stat = FilterSobelVert( src_16s_CA, dst_16s_CA );  io++;
    stat = FilterSobelVert( src_16s_C4, dst_16s_C4 );  io++;

    stat = FilterSobelVert( src_32f_C1, dst_32f_C1 );  io++;
    stat = FilterSobelVert( src_32f_C3, dst_32f_C3 );  io++;
    stat = FilterSobelVert( src_32f_CA, dst_32f_CA );  io++;
    stat = FilterSobelVert( src_32f_C4, dst_32f_C4 );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );


// ==================================   FIXED FILTERS with 3x3 and 5x5 mask sizes   ===================================

// ---- FILTER_[ GAUSS | [HI|LOW]PASS | LAPLACE | SOBEL[CROSS|HORIZ|VERT][|SECOND|MASK] ]:

// Images preparing:
    kern.init(5,5);  // verifying for this mask size only

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterGauss:
    stat = FilterGauss( src_8u_C1 , dst_8u_C1 , ippMskSize5x5 );  io++;
    stat = FilterGauss( src_8u_C3 , dst_8u_C3 , ippMskSize5x5 );  io++;
    stat = FilterGauss( src_8u_CA , dst_8u_CA , ippMskSize5x5 );  io++;
    stat = FilterGauss( src_8u_C4 , dst_8u_C4 , ippMskSize5x5 );  io++;

    stat = FilterGauss( src_16s_C1, dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_16s_C3, dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_16s_CA, dst_16s_CA, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_16s_C4, dst_16s_C4, ippMskSize5x5 );  io++;

    stat = FilterGauss( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_32f_C3, dst_32f_C3, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_32f_CA, dst_32f_CA, ippMskSize5x5 );  io++;
    stat = FilterGauss( src_32f_C4, dst_32f_C4, ippMskSize5x5 );  io++;

// FilterHipass:
    stat = FilterHipass( src_8u_C1 , dst_8u_C1 , ippMskSize5x5 );  io++;
    stat = FilterHipass( src_8u_C3 , dst_8u_C3 , ippMskSize5x5 );  io++;
    stat = FilterHipass( src_8u_CA , dst_8u_CA , ippMskSize5x5 );  io++;
    stat = FilterHipass( src_8u_C4 , dst_8u_C4 , ippMskSize5x5 );  io++;

    stat = FilterHipass( src_16s_C1, dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_16s_C3, dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_16s_CA, dst_16s_CA, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_16s_C4, dst_16s_C4, ippMskSize5x5 );  io++;

    stat = FilterHipass( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_32f_C3, dst_32f_C3, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_32f_CA, dst_32f_CA, ippMskSize5x5 );  io++;
    stat = FilterHipass( src_32f_C4, dst_32f_C4, ippMskSize5x5 );  io++;

// FilterLaplace:
    stat = FilterLaplace( src_8u_C1 , dst_8u_C1 , ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_8u_C3 , dst_8u_C3 , ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_8u_CA , dst_8u_CA , ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_8u_C4 , dst_8u_C4 , ippMskSize5x5 );  io++;

    stat = FilterLaplace( src_16s_C1, dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_16s_C3, dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_16s_CA, dst_16s_CA, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_16s_C4, dst_16s_C4, ippMskSize5x5 );  io++;

    stat = FilterLaplace( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_32f_C3, dst_32f_C3, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_32f_CA, dst_32f_CA, ippMskSize5x5 );  io++;
    stat = FilterLaplace( src_32f_C4, dst_32f_C4, ippMskSize5x5 );  io++;

    stat = FilterLaplace(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterLaplace( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;

// FilterLowpass:
    stat = FilterLowpass( src_8u_C1 , dst_8u_C1 , ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_8u_C3 , dst_8u_C3 , ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_8u_CA , dst_8u_CA , ippMskSize5x5 );  io++;

    stat = FilterLowpass( src_16s_C1, dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_16s_C3, dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_16s_CA, dst_16s_CA, ippMskSize5x5 );  io++;

    stat = FilterLowpass( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_32f_C3, dst_32f_C3, ippMskSize5x5 );  io++;
    stat = FilterLowpass( src_32f_CA, dst_32f_CA, ippMskSize5x5 );  io++;

// FilterSobelCross:
    stat = FilterSobelCross(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelCross( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelCross(                 src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;

// FilterSobelHoriz:
    stat = FilterSobelHoriz(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelHoriz( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;

// FilterSobelHorizMask:
    stat = FilterSobelHorizMask( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;

// FilterSobelHorizSecond:
    stat = FilterSobelHorizSecond(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelHorizSecond( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelHorizSecond(                 src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;

// FilterSobelVert:
    stat = FilterSobelVert(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelVert( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;

// FilterSobelVertMask:
    stat = FilterSobelVertMask( src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;

// FilterSobelVertSecond:
    stat = FilterSobelVertSecond(                 src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelVertSecond( *(GIMG<Ipp8s>*)&src_8u_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterSobelVertSecond(                 src_32f_C1, dst_32f_C1, ippMskSize5x5 );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                          -= 2D Convolution (from #9) =-                                           #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// Src images preparing (resizing; beginning of subimages matches images one for simplicity):

    src1_8u_C1.roiSize.set(  conv.srcSize[0] );
    src1_8u_C3.roiSize.set(  conv.srcSize[0] );
    src1_8u_CA.roiSize.set(  conv.srcSize[0] );
    src2_8u_C1.roiSize.set(  conv.srcSize[1] );
    src2_8u_C3.roiSize.set(  conv.srcSize[1] );
    src2_8u_CA.roiSize.set(  conv.srcSize[1] );

    src1_16s_C1.roiSize.set( conv.srcSize[0] );
    src1_16s_C3.roiSize.set( conv.srcSize[0] );
    src1_16s_CA.roiSize.set( conv.srcSize[0] );
    src2_16s_C1.roiSize.set( conv.srcSize[1] );
    src2_16s_C3.roiSize.set( conv.srcSize[1] );
    src2_16s_CA.roiSize.set( conv.srcSize[1] );

    src1_32f_C1.roiSize.set( conv.srcSize[0] );
    src1_32f_C3.roiSize.set( conv.srcSize[0] );
    src1_32f_CA.roiSize.set( conv.srcSize[0] );
    src2_32f_C1.roiSize.set( conv.srcSize[1] );
    src2_32f_C3.roiSize.set( conv.srcSize[1] );
    src2_32f_CA.roiSize.set( conv.srcSize[1] );

// ---- CONV_FULL:
// Dst images preparing (resizing; beginning of subimages matches images one for simplicity):

    dst_8u_C1.roiSize.set(  conv.dstSize[0] );
    dst_8u_C3.roiSize.set(  conv.dstSize[0] );
    dst_8u_CA.roiSize.set(  conv.dstSize[0] );

    dst_16s_C1.roiSize.set( conv.dstSize[0] );
    dst_16s_C3.roiSize.set( conv.dstSize[0] );
    dst_16s_CA.roiSize.set( conv.dstSize[0] );

    dst_32f_C1.roiSize.set( conv.dstSize[0] );
    dst_32f_C3.roiSize.set( conv.dstSize[0] );
    dst_32f_CA.roiSize.set( conv.dstSize[0] );

// ConvFull:

    stat = ConvFull( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 , scale );    io++;
    stat = ConvFull( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 , scale+3 );  io++;
    stat = ConvFull( src1_8u_CA , src2_8u_CA , dst_8u_CA , scale/3 );  io++;

    stat = ConvFull( src1_16s_C1, src2_16s_C1, dst_16s_C1, scale );    io++;
    stat = ConvFull( src1_16s_C3, src2_16s_C3, dst_16s_C3, scale+3 );  io++;
    stat = ConvFull( src1_16s_CA, src2_16s_CA, dst_16s_CA, scale/3 );  io++;

    stat = ConvFull( src1_32f_C1, src2_32f_C1, dst_32f_C1 );  io++;
    stat = ConvFull( src1_32f_C3, src2_32f_C3, dst_32f_C3 );  io++;
    stat = ConvFull( src1_32f_CA, src2_32f_CA, dst_32f_CA );  io++;

// ---- CONV_VALID:
// Dst images preparing (resizing; beginning of subimages matches images one for simplicity):

    dst_8u_C1.roiSize.set(  conv.dstSize[1] );
    dst_8u_C3.roiSize.set(  conv.dstSize[1] );
    dst_8u_CA.roiSize.set(  conv.dstSize[1] );

    dst_16s_C1.roiSize.set( conv.dstSize[1] );
    dst_16s_C3.roiSize.set( conv.dstSize[1] );
    dst_16s_CA.roiSize.set( conv.dstSize[1] );

    dst_32f_C1.roiSize.set( conv.dstSize[1] );
    dst_32f_C3.roiSize.set( conv.dstSize[1] );
    dst_32f_CA.roiSize.set( conv.dstSize[1] );

// ConvValid:

    stat = ConvValid( src1_8u_C1 , src2_8u_C1 , dst_8u_C1 , scale );    io++;
    stat = ConvValid( src1_8u_C3 , src2_8u_C3 , dst_8u_C3 , scale+3 );  io++;
    stat = ConvValid( src1_8u_CA , src2_8u_CA , dst_8u_CA , scale/3 );  io++;

    stat = ConvValid( src1_16s_C1, src2_16s_C1, dst_16s_C1, scale );    io++;
    stat = ConvValid( src1_16s_C3, src2_16s_C3, dst_16s_C3, scale+3 );  io++;
    stat = ConvValid( src1_16s_CA, src2_16s_CA, dst_16s_CA, scale/3 );  io++;

    stat = ConvValid( src1_32f_C1, src2_32f_C1, dst_32f_C1 );  io++;
    stat = ConvValid( src1_32f_C3, src2_32f_C3, dst_32f_C3 );  io++;
    stat = ConvValid( src1_32f_CA, src2_32f_CA, dst_32f_CA );  io++;

// Images restoring:
    src1_8u_C1.restoreRect();
    src1_8u_C3.restoreRect();
    src1_8u_CA.restoreRect();
    src2_8u_C1.restoreRect();
    src2_8u_C3.restoreRect();
    src2_8u_CA.restoreRect();

    src1_16s_C1.restoreRect();
    src1_16s_C3.restoreRect();
    src1_16s_CA.restoreRect();
    src2_16s_C1.restoreRect();
    src2_16s_C3.restoreRect();
    src2_16s_CA.restoreRect();

    src1_32f_C1.restoreRect();
    src1_32f_C3.restoreRect();
    src1_32f_CA.restoreRect();
    src2_32f_C1.restoreRect();
    src2_32f_C3.restoreRect();
    src2_32f_CA.restoreRect();

    dst_8u_C1.restoreRect();
    dst_8u_C3.restoreRect();
    dst_8u_CA.restoreRect();

    dst_16s_C1.restoreRect();
    dst_16s_C3.restoreRect();
    dst_16s_CA.restoreRect();

    dst_32f_C1.restoreRect();
    dst_32f_C3.restoreRect();
    dst_32f_CA.restoreRect();


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                      -= General Linear Filters (from #9) =-                                       #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// ---- FILTER:

// Images preparing for 5x5 kernel:
    kern.init(5,5);
    pKern_32f = new Ipp32f[kern.size.width*kern.size.height];

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// Filter:
    stat = Filter( src_8u_C1 , dst_8u_C1 , (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale );    io++;
    stat = Filter( src_8u_C3 , dst_8u_C3 , (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale+3 );  io++;
    stat = Filter( src_8u_CA , dst_8u_CA , (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale/3 );  io++;
    stat = Filter( src_8u_C4 , dst_8u_C4 , (Ipp32s*)pKern_32f, kern.size, kern.anchor, 3 );        io++;

    stat = Filter( src_16s_C1, dst_16s_C1, (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale );    io++;
    stat = Filter( src_16s_C3, dst_16s_C3, (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale+3 );  io++;
    stat = Filter( src_16s_CA, dst_16s_CA, (Ipp32s*)pKern_32f, kern.size, kern.anchor, scale/3 );  io++;
    stat = Filter( src_16s_C4, dst_16s_C4, (Ipp32s*)pKern_32f, kern.size, kern.anchor, 3 );        io++;

    stat = Filter( src_32f_C1, dst_32f_C1,          pKern_32f, kern.size, kern.anchor );           io++;
    stat = Filter( src_32f_C3, dst_32f_C3,          pKern_32f, kern.size, kern.anchor );           io++;
    stat = Filter( src_32f_CA, dst_32f_CA,          pKern_32f, kern.size, kern.anchor );           io++;
    stat = Filter( src_32f_C4, dst_32f_C4,          pKern_32f, kern.size, kern.anchor );           io++;

// Filter32f:
    stat = Filter32f( src_8u_C1 , dst_8u_C1 , pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_8u_C3 , dst_8u_C3 , pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_8u_CA , dst_8u_CA , pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_8u_C4 , dst_8u_C4 , pKern_32f, kern.size, kern.anchor );  io++;

    stat = Filter32f( src_16s_C1, dst_16s_C1, pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_16s_C3, dst_16s_C3, pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_16s_CA, dst_16s_CA, pKern_32f, kern.size, kern.anchor );  io++;
    stat = Filter32f( src_16s_C4, dst_16s_C4, pKern_32f, kern.size, kern.anchor );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );

    delete[] pKern_32f;


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                          -= Median Filters (from #9) =-                                           #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// ---- FILTER_MEDIAN:

// Images preparing:
    kern.init(3,3);
    kern.setAnchor(0,0);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterMedian:
    stat = FilterMedian( src_8u_C1 , dst_8u_C1 , kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_8u_C3 , dst_8u_C3 , kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_8u_CA , dst_8u_CA , kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_8u_C4 , dst_8u_C4 , kern.size , kern.anchor );  io++;

    stat = FilterMedian( src_16s_C1, dst_16s_C1, kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_16s_C3, dst_16s_C3, kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_16s_CA, dst_16s_CA, kern.size , kern.anchor );  io++;
    stat = FilterMedian( src_16s_C4, dst_16s_C4, kern.size , kern.anchor );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );


// ---- FILTER_MEDIAN_[COLOR|CROSS]:

// Images preparing for ippMskSize3x3:
    kern.init(3,3);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );

// FilterMedianColor:
    stat = FilterMedianColor( src_8u_C3 , dst_8u_C3 , ippMskSize3x3 );  io++;
    stat = FilterMedianColor( src_8u_CA , dst_8u_CA , ippMskSize3x3 );  io++;

// FilterMedianCross:
    stat = FilterMedianCross( src_8u_C1 , dst_8u_C1 , ippMskSize3x3 );  io++;
    stat = FilterMedianCross( src_8u_C3 , dst_8u_C3 , ippMskSize3x3 );  io++;
    stat = FilterMedianCross( src_8u_CA , dst_8u_CA , ippMskSize3x3 );  io++;

// Images restoring:
    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );

    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();

// Images preparing for ippMskSize5x5:
    kern.init(5,5);

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterMedianColor:
    stat = FilterMedianColor( src_16s_C3 , dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterMedianCross( src_16s_CA , dst_16s_CA, ippMskSize5x5 );  io++;

// FilterMedianCross:
    stat = FilterMedianCross( src_16s_C1 , dst_16s_C1, ippMskSize5x5 );  io++;
    stat = FilterMedianCross( src_16s_C3 , dst_16s_C3, ippMskSize5x5 );  io++;
    stat = FilterMedianCross( src_16s_CA , dst_16s_CA, ippMskSize5x5 );  io++;

// Images restoring:
    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();


// ---- FILTER_MEDIAN_HORIZ:

// Images preparing for ippMskSize3x1:
    kern.init(3,1);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

// FilterMedianHoriz:
    stat = FilterMedianHoriz( src_8u_C1 , dst_8u_C1 , ippMskSize3x1 );  io++;
    stat = FilterMedianHoriz( src_8u_C3 , dst_8u_C3 , ippMskSize3x1 );  io++;
    stat = FilterMedianHoriz( src_8u_CA , dst_8u_CA , ippMskSize3x1 );  io++;
    stat = FilterMedianHoriz( src_8u_C4 , dst_8u_C4 , ippMskSize3x1 );  io++;

// Images restoring:
    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

// Images preparing for ippMskSize5x1:
    kern.init(5,1);

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterMedianHoriz:
    stat = FilterMedianHoriz( src_16s_C1 , dst_16s_C1 , ippMskSize5x1 );  io++;
    stat = FilterMedianHoriz( src_16s_C3 , dst_16s_C3 , ippMskSize5x1 );  io++;
    stat = FilterMedianHoriz( src_16s_CA , dst_16s_CA , ippMskSize5x1 );  io++;
    stat = FilterMedianHoriz( src_16s_C4 , dst_16s_C4 , ippMskSize5x1 );  io++;

// Images restoring:
    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();


// ---- FILTER_MEDIAN_VERT:

// Images preparing for ippMskSize1x3:
    kern.init(1,3);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

// FilterMedianVert:
    stat = FilterMedianVert( src_8u_C1 , dst_8u_C1 , ippMskSize1x3 );  io++;
    stat = FilterMedianVert( src_8u_C3 , dst_8u_C3 , ippMskSize1x3 );  io++;
    stat = FilterMedianVert( src_8u_CA , dst_8u_CA , ippMskSize1x3 );  io++;
    stat = FilterMedianVert( src_8u_C4 , dst_8u_C4 , ippMskSize1x3 );  io++;

// Images restoring:
    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

// Images preparing for ippMskSize1x5:
    kern.init(1,5);

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterMedianVert:
    stat = FilterMedianVert( src_16s_C1 , dst_16s_C1 , ippMskSize1x5 );  io++;
    stat = FilterMedianVert( src_16s_C3 , dst_16s_C3 , ippMskSize1x5 );  io++;
    stat = FilterMedianVert( src_16s_CA , dst_16s_CA , ippMskSize1x5 );  io++;
    stat = FilterMedianVert( src_16s_C4 , dst_16s_C4 , ippMskSize1x5 );  io++;

// Images restoring:
    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                         -= Separable Filters (from #9) =-                                         #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// ---- FILTER_COLUMN:

// Images preparing for 1x5 kernel:
    kern.init(1,5);
    pKern_32f = new Ipp32f[kern.size.height];

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );


// FilterColumn:
    stat = FilterColumn( src_8u_C1 , dst_8u_C1 , (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale );    io++;
    stat = FilterColumn( src_8u_C3 , dst_8u_C3 , (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale+3 );  io++;
    stat = FilterColumn( src_8u_CA , dst_8u_CA , (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale/3 );  io++;
    stat = FilterColumn( src_8u_C4 , dst_8u_C4 , (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, 3 );        io++;

    stat = FilterColumn( src_16s_C1, dst_16s_C1, (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale );    io++;
    stat = FilterColumn( src_16s_C3, dst_16s_C3, (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale+3 );  io++;
    stat = FilterColumn( src_16s_CA, dst_16s_CA, (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, scale/3 );  io++;
    stat = FilterColumn( src_16s_C4, dst_16s_C4, (Ipp32s*)pKern_32f, kern.size.height, kern.anchor.y, 3 );        io++;

    stat = FilterColumn( src_32f_C1, dst_32f_C1,          pKern_32f, kern.size.height, kern.anchor.y );           io++;
    stat = FilterColumn( src_32f_C3, dst_32f_C3,          pKern_32f, kern.size.height, kern.anchor.y );           io++;
    stat = FilterColumn( src_32f_CA, dst_32f_CA,          pKern_32f, kern.size.height, kern.anchor.y );           io++;
    stat = FilterColumn( src_32f_C4, dst_32f_C4,          pKern_32f, kern.size.height, kern.anchor.y );           io++;

// FilterColumn32f:
    stat = FilterColumn32f( src_8u_C1 , dst_8u_C1 , pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_8u_C3 , dst_8u_C3 , pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_8u_CA , dst_8u_CA , pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_8u_C4 , dst_8u_C4 , pKern_32f, kern.size.height, kern.anchor.y );  io++;

    stat = FilterColumn32f( src_16s_C1, dst_16s_C1, pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_16s_C3, dst_16s_C3, pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_16s_CA, dst_16s_CA, pKern_32f, kern.size.height, kern.anchor.y );  io++;
    stat = FilterColumn32f( src_16s_C4, dst_16s_C4, pKern_32f, kern.size.height, kern.anchor.y );  io++;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );

    delete[] pKern_32f;


/* #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9
#9                                                                                                                   #9
#9                                           -= Wiener Filters (from #9) =-                                          #9
#9                                                                                                                   #9
#9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 #9 */

// ---- FILTER_WIENER:

// Images preparing for 3x3 kernel:
    kern.init(3,3);

    src_8u_C1.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C3.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_CA.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );
    src_8u_C4.roiRect(  kern.addL, kern.addT, kern.addR, kern.addB );

    src_16s_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_16s_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_CA.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C4.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_8u_C1.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C3.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_CA.roiSize.resize(  -kern.border.width, -kern.border.height );
    dst_8u_C4.roiSize.resize(  -kern.border.width, -kern.border.height );

    dst_16s_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_16s_C4.roiSize.resize( -kern.border.width, -kern.border.height );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_CA.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C4.roiSize.resize( -kern.border.width, -kern.border.height );

// FilterWiener:
    stat = ippiFilterWienerGetBufferSize( dst_8u_C1.roiSize, kern.size, 1, &size );
    pBuffer = new Ipp32s[(size+2)/4];
    stat = FilterWiener( src_8u_C1 , dst_8u_C1 , kern.size, kern.anchor, &noise[3], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_16s_C1, dst_16s_C1, kern.size, kern.anchor, &noise[3], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_32f_C1, dst_32f_C1, kern.size, kern.anchor, &noise[3], (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

    stat = ippiFilterWienerGetBufferSize( dst_8u_C3.roiSize, kern.size, 3, &size );
    pBuffer = (Ipp32s*)(new Ipp8u[(size+2)/4*4]);
    stat = FilterWiener( src_8u_C3 , dst_8u_C3 , kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_16s_C3, dst_16s_C3, kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_32f_C3, dst_32f_C3, kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

    stat = ippiFilterWienerGetBufferSize( dst_8u_CA.roiSize, kern.size, 4, &size );
    pBuffer = new Ipp32s[(size+2)/4];
    stat = FilterWiener( src_8u_CA , dst_8u_CA , kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_8u_C4 , dst_8u_C4 , kern.size, kern.anchor,  noise   , (Ipp8u*)pBuffer );  io++;

    stat = FilterWiener( src_16s_CA, dst_16s_CA, kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_16s_C4, dst_16s_C4, kern.size, kern.anchor,  noise   , (Ipp8u*)pBuffer );  io++;

    stat = FilterWiener( src_32f_CA, dst_32f_CA, kern.size, kern.anchor, &noise[1], (Ipp8u*)pBuffer );  io++;
    stat = FilterWiener( src_32f_C4, dst_32f_C4, kern.size, kern.anchor,  noise   , (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

// Images restoring:
    src_8u_C1.restoreRect();
    src_8u_C3.restoreRect();
    src_8u_CA.restoreRect();
    src_8u_C4.restoreRect();

    src_16s_C1.restoreRect();
    src_16s_C3.restoreRect();
    src_16s_CA.restoreRect();
    src_16s_C4.restoreRect();

    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();
    src_32f_CA.restoreRect();
    src_32f_C4.restoreRect();

    dst_8u_C1.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C3.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_CA.roiSize.resize(  kern.border.width, kern.border.height );
    dst_8u_C4.roiSize.resize(  kern.border.width, kern.border.height );

    dst_16s_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_16s_C4.roiSize.resize( kern.border.width, kern.border.height );

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_CA.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C4.roiSize.resize( kern.border.width, kern.border.height );


// ---- DECONV_FFT(conceivably from #9):

// Images preparing for 3x3 kernel:
    kern.init(3,3);
    pKern_32f = new Ipp32f[kern.size.width*kern.size.height];

    src_32f_C1.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );
    src_32f_C3.roiRect( kern.addL, kern.addT, kern.addR, kern.addB );

    dst_32f_C1.roiSize.resize( -kern.border.width, -kern.border.height );
    dst_32f_C3.roiSize.resize( -kern.border.width, -kern.border.height );

// ---- DeconvFFT:

    stat = DeconvFFTInitAlloc( (IppiDeconvFFTState_32f_C1R**)pDeconvFFTState_32f_C3R , pKern_32f, kern.size.width, 5, (Ipp32f)IPP_MAX_16U );  io++;
    stat = DeconvFFT( src_32f_C1, dst_32f_C1, (IppiDeconvFFTState_32f_C1R*)(pDeconvFFTState_32f_C3R[0]) );  io++;
    stat = DeconvFFTFree(  (IppiDeconvFFTState_32f_C1R*)(pDeconvFFTState_32f_C3R[0]) );  io++;

    stat = DeconvFFTInitAlloc( pDeconvFFTState_32f_C3R , pKern_32f, kern.size.width, 7, (Ipp32f)IPP_MAX_16U );  io++;
    stat = DeconvFFT( src_32f_C3, dst_32f_C3, pDeconvFFTState_32f_C3R[0] );  io++;
    stat = DeconvFFTFree(  pDeconvFFTState_32f_C3R[0] );  io++;

// ---- DeconvLR:

    stat = DeconvLRInitAlloc( (IppiDeconvLR_32f_C1R**)pDeconvFFTState_32f_C3R , pKern_32f, kern.size.width, maxroi, (Ipp32f)IPP_MAX_16U );  io++;
    stat = DeconvLR( src_32f_C1, dst_32f_C1, 2, (IppiDeconvLR_32f_C1R*)(pDeconvFFTState_32f_C3R[0]) );  io++;
    stat = DeconvLRFree(  (IppiDeconvLR_32f_C1R*)(pDeconvFFTState_32f_C3R[0]) );  io++;

    stat = DeconvLRInitAlloc( (IppiDeconvLR_32f_C3R**)pDeconvFFTState_32f_C3R , pKern_32f, kern.size.width, maxroi, (Ipp32f)IPP_MAX_16U );  io++;
    stat = DeconvLR( src_32f_C3, dst_32f_C3, 3, (IppiDeconvLR_32f_C3R*)pDeconvFFTState_32f_C3R[0] );  io++;
    stat = DeconvLRFree(  (IppiDeconvLR_32f_C3R*)pDeconvFFTState_32f_C3R[0] );  io++;

// Images restoring:
    src_32f_C1.restoreRect();
    src_32f_C3.restoreRect();

    dst_32f_C1.roiSize.resize( kern.border.width, kern.border.height );
    dst_32f_C3.roiSize.resize( kern.border.width, kern.border.height );

    delete[] pKern_32f;


/* #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A
#5                   _    _ _  _  ___   _   __     ___  __    _  _  _  ___  ___   _  _  _   __   ___                 #A
#5                   /    / /| / /_   /_ / /_ /     /  /_ / /_ / /| / /_   /_   /  / /|/|  /_ / /_                   #A
#5                  /__ _/ / |/ /__  /  / /  \     /  /  \ /  / / |/ ___/ /     \_/ / | | /  \ ___/                  #A
#A                                                                                                                   #A
#A                                         < Image Linear Transforms (#10) >                                         #A
#A                                                                                                                   #A
#A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A */


/* #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A
#A                                                                                                                   #A
#A                                    -= Discrete Cosine Transforms (from #10) =-                                    #A
#A                                                                                                                   #A
#A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A */

// ---- DCT_[FWD|INV]_INIT_ALLOC:

    stat = DCTFwdInitAlloc(                    pDCTFwdSpec_32f, roiSize, ippAlgHintNone );  io++;
    stat = DCTInvInitAlloc( (IppiDCTInvSpec_32f**)pDCTSpec_32f, roiSize, ippAlgHintFast );  io++;

// ---- DCT_[FWD|INV]_GET_BUFSIZE:

    stat = DCTFwdGetBufSize(                         pDCTFwdSpec_32f[0], &size );  io++;
    stat = DCTInvGetBufSize( *(IppiDCTInvSpec_32f**)(pDCTSpec_32f)     , pSize );  io++;


// ---- DCT_[FWD|INV]:

    stat = DCTFwd( src_32f_C1, dst_32f_C1, pDCTFwdSpec_32f[0], 0 );  io++;
    stat = DCTFwd( src_32f_C3, dst_32f_C3, pDCTFwdSpec_32f[0], 0 );  io++;
    stat = DCTFwd( src_32f_CA, dst_32f_CA, pDCTFwdSpec_32f[0], 0 );  io++;
    stat = DCTFwd( src_32f_C4, dst_32f_C4, pDCTFwdSpec_32f[0], 0 );  io++;

    pBuffer = new Ipp32s[size];
    stat = DCTFwd( src_32f_C1, dst_32f_C1, pDCTFwdSpec_32f[0], (Ipp8u*)pBuffer );        io++;
    stat = DCTFwd( src_32f_C3, dst_32f_C3, pDCTFwdSpec_32f[0], (Ipp8u*)pBuffer+1 );      io++;
    stat = DCTFwd( src_32f_CA, dst_32f_CA, pDCTFwdSpec_32f[0], (Ipp8u*)(pBuffer+1) );    io++;
    stat = DCTFwd( src_32f_C4, dst_32f_C4, pDCTFwdSpec_32f[0], (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

    pBuffer = new Ipp32s[pSize[0]];
    stat = DCTInv( src_32f_C1, dst_32f_C1,  (IppiDCTInvSpec_32f*)(pDCTSpec_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = DCTInv( src_32f_C3, dst_32f_C3, *(IppiDCTInvSpec_32f**)(pDCTSpec_32f)  , (Ipp8u*)pBuffer+1 );      io++;
    stat = DCTInv( src_32f_CA, dst_32f_CA,  (IppiDCTInvSpec_32f*)(pDCTSpec_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = DCTInv( src_32f_C4, dst_32f_C4, *(IppiDCTInvSpec_32f**)(pDCTSpec_32f)  , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;


// ---- DCT_[FWD|INV]_FREE:

    stat = DCTFwdFree(                       pDCTFwdSpec_32f[0] );  io++;
    stat = DCTInvFree( (IppiDCTInvSpec_32f*)(pDCTSpec_32f[0]) );    io++;


/* #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A
#A                                                                                                                   #A
#A                                      -= Fourier Transforms (from #10) =-                                          #A
#A                                                                                                                   #A
#A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A */

// ---- DFT_ITNIT_ALLOC:

    stat = DFTInitAlloc( (IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s, roiSize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone );      io++;
    stat = DFTInitAlloc( (IppiDFTSpec_R_32f**)pFFTSpec_32s_useR_32f , roiSize,                   -1, ippAlgHintAccurate );  io++;
    stat = DFTInitAlloc( (IppiDFTSpec_R_32f**)pFFTSpec_32s_useR_32f , roiSize, IPP_FFT_NODIV_BY_ANY, ippAlgHintAccurate );  io++;
    stat = DFTInitAlloc(  pDFTSpec_32fc                             , roiSize,     (int)(val_64f+1), ippAlgHintFast );      io++;


// ---- DFT_GET_BUFSIZE:

    stat = DFTGetBufSize(   (IppiDFTSpec_R_32s*)(pDFTSpec_32fc_useR_32s[0]), &size );      io++;
    stat = DFTGetBufSize( *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)    , pSize );      io++;
    stat = DFTGetBufSize(                        pDFTSpec_32fc[0]          , &pSize[1] );  io++;


// ---- DFT_[FWD|INV]:

    stat = DFTFwd_RToPack( src_8u_C4 , dst_32s_C4, *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s, 3, 0 );  io++;
    stat = DFTInv_PackToR( src_32s_C4, dst_8u_C4 , *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s, 3, 0 );  io++;

    pBuffer = new Ipp32s[size];
    stat = DFTFwd_RToPack( src_8u_C1, dst_32s_C1,  (IppiDFTSpec_R_32s*)(pDFTSpec_32fc_useR_32s[0]), scale  , (Ipp8u*)pBuffer );        io++;
    stat = DFTFwd_RToPack( src_8u_C3, dst_32s_C3, *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s    , scale+3, (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTFwd_RToPack( src_8u_CA, dst_32s_CA,  (IppiDFTSpec_R_32s*)(pDFTSpec_32fc_useR_32s[0]), scale/3, (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTFwd_RToPack( src_8u_C4, dst_32s_C4, *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s    , 3      , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = DFTInv_PackToR( src_32s_C1, dst_8u_C1,  (IppiDFTSpec_R_32s*)(pDFTSpec_32fc_useR_32s[0]), scale  , (Ipp8u*)pBuffer );        io++;
    stat = DFTInv_PackToR( src_32s_C3, dst_8u_C3, *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s    , scale+3, (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTInv_PackToR( src_32s_CA, dst_8u_CA,  (IppiDFTSpec_R_32s*)(pDFTSpec_32fc_useR_32s[0]), scale/3, (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTInv_PackToR( src_32s_C4, dst_8u_C4, *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s    , 3      , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

// (NI)
    stat = DFTFwd_RToPack( src_32f_C4, dst_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f), 0 );  io++;
    stat = DFTInv_PackToR( src_32f_C4, dst_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f), 0 );  io++;

    pBuffer = new Ipp32s[pSize[0]];
    stat = DFTFwd_RToPack( src_32f_C1, dst_32f_C1,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = DFTFwd_RToPack( src_32f_C3, dst_32f_C3, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTFwd_RToPack( src_32f_CA, dst_32f_CA,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTFwd_RToPack( src_32f_C4, dst_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = DFTInv_PackToR( src_32f_C1, dst_32f_C1,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = DFTInv_PackToR( src_32f_C3, dst_32f_C3, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTInv_PackToR( src_32f_CA, dst_32f_CA,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTInv_PackToR( src_32f_C4, dst_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

    stat = DFTFwd_CToC( src_32fc_C1, dst_32fc_C1, pDFTSpec_32fc[0], 0 );  io++;
    pBuffer = new Ipp32s[pSize[1]];
    stat = DFTFwd_CToC( src_32fc_C1, dst_32fc_C1, pDFTSpec_32fc[0], (Ipp8u*)pBuffer );  io++;
    stat = DFTInv_CToC( src_32fc_C1, dst_32fc_C1, pDFTSpec_32fc[0], (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

// (I)
    stat = DFTFwd_RToPack( src_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f), 0 );  io++;
    stat = DFTInv_PackToR( src_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f), 0 );  io++;

    pBuffer = new Ipp32s[pSize[0]];
    stat = DFTFwd_RToPack( img_32f_C1,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = DFTFwd_RToPack( img_32f_C3, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTFwd_RToPack( img_32f_CA,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTFwd_RToPack( img_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = DFTInv_PackToR( img_32f_C1,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = DFTInv_PackToR( img_32f_C3, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)pBuffer+1 );      io++;
    stat = DFTInv_PackToR( img_32f_CA,  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = DFTInv_PackToR( img_32f_C4, *(IppiDFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)  , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

    stat = DFTFwd_CToC( src_32fc_C1, pDFTSpec_32fc[0], 0 );  io++;
    pBuffer = new Ipp32s[pSize[1]];
    stat = DFTFwd_CToC( src_32fc_C1, pDFTSpec_32fc[0], (Ipp8u*)pBuffer );  io++;
    stat = DFTInv_CToC( src_32fc_C1, pDFTSpec_32fc[0], (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;


// ---- DFT_FREE:

    stat = DFTFree( *(IppiDFTSpec_R_32s**)pDFTSpec_32fc_useR_32s );     io++;
    stat = DFTFree(  (IppiDFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]) );  io++;
    stat = DFTFree(                       pDFTSpec_32fc[0] );           io++;


// ---- FFT_INIT_ALLOC:

    stat = FFTInitAlloc(                       pFFTSpec_32s          ,  1                                , 2             , IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone );      io++;
    stat = FFTInitAlloc(  (IppiFFTSpec_R_32f**)pFFTSpec_32s_useR_32f , -1                                , 0             , IPP_FFT_DIV_BY_SQRTN, ippAlgHintAccurate );  io++;
    stat = FFTInitAlloc(  (IppiFFTSpec_R_32f**)pFFTSpec_32s_useR_32f , (Ipp32s)(*((float*)val_32fc+2)/20), (int)val_64f  , IPP_FFT_NODIV_BY_ANY, ippAlgHintFast );      io++;
    stat = FFTInitAlloc( (IppiFFTSpec_C_32fc**)pFFTSpec_32s_useC_32fc, (int)((Ipp32f*)val_32fc)[2]-82    , (int)val_64f+1, IPP_FFT_DIV_INV_BY_N, ippAlgHintFast );      io++;


// ---- FFT_GET_BUFSIZE:
    stat = FFTGetBufSize(                         pFFTSpec_32s[0]          , &size );      io++;
    stat = FFTGetBufSize(    (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), pSize );      io++;
    stat = FFTGetBufSize( *(IppiFFTSpec_C_32fc**)(pFFTSpec_32s_useC_32fc)  , &pSize[9] );  io++;


// ---- FFT_[FWD|INV]:

    stat = FFTFwd_RToPack( src_8u_C1 , dst_32s_C1, pFFTSpec_32s[0], scale, 0 );  io++;
    stat = FFTInv_PackToR( src_32s_C1, dst_8u_C1 , pFFTSpec_32s[0], scale, 0 );  io++;

    pBuffer = new Ipp32s[size];
    stat = FFTFwd_RToPack( src_8u_C1, dst_32s_C1, pFFTSpec_32s[0], scale  , (Ipp8u*)pBuffer );        io++;
    stat = FFTFwd_RToPack( src_8u_C3, dst_32s_C3, pFFTSpec_32s[0], scale+3, (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTFwd_RToPack( src_8u_CA, dst_32s_CA, pFFTSpec_32s[0], scale/3, (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTFwd_RToPack( src_8u_C4, dst_32s_C4, pFFTSpec_32s[0], 3      , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = FFTInv_PackToR( src_32s_C1, dst_8u_C1, pFFTSpec_32s[0], scale  , (Ipp8u*)pBuffer );        io++;
    stat = FFTInv_PackToR( src_32s_C3, dst_8u_C3, pFFTSpec_32s[0], scale+3, (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTInv_PackToR( src_32s_CA, dst_8u_CA, pFFTSpec_32s[0], scale/3, (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTInv_PackToR( src_32s_C4, dst_8u_C4, pFFTSpec_32s[0], 3      , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

// (NI)
    stat = FFTFwd_RToPack( src_32f_C1, dst_32f_C1, (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), 0 );  io++;
    stat = FFTInv_PackToR( src_32f_C1, dst_32f_C1, (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), 0 );  io++;

    pBuffer = new Ipp32s[pSize[0]];
    stat = FFTFwd_RToPack( src_32f_C1, dst_32f_C1,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = FFTFwd_RToPack( src_32f_C3, dst_32f_C3, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTFwd_RToPack( src_32f_CA, dst_32f_CA,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTFwd_RToPack( src_32f_C4, dst_32f_C4, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = FFTInv_PackToR( src_32f_C1, dst_32f_C1,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = FFTInv_PackToR( src_32f_C3, dst_32f_C3, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTInv_PackToR( src_32f_CA, dst_32f_CA,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTInv_PackToR( src_32f_C4, dst_32f_C4, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

    stat = FFTFwd_CToC( src_32fc_C1, dst_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), 0 );  io++;
    pBuffer = new Ipp32s[pSize[9]];
    stat = FFTFwd_CToC( src_32fc_C1, dst_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), (Ipp8u*)pBuffer );  io++;
    stat = FFTInv_CToC( src_32fc_C1, dst_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

// (I)
    stat = FFTFwd_RToPack( img_32f_C1, (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), 0 );  io++;
    stat = FFTInv_PackToR( img_32f_C1, (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), 0 );  io++;

    pBuffer = new Ipp32s[pSize[0]];
    stat = FFTFwd_RToPack( img_32f_C1,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = FFTFwd_RToPack( img_32f_C3, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTFwd_RToPack( img_32f_CA,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTFwd_RToPack( img_32f_C4, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)(pBuffer+1)-4 );  io++;

    stat = FFTInv_PackToR( img_32f_C1,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)pBuffer );        io++;
    stat = FFTInv_PackToR( img_32f_C3, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)pBuffer+1 );      io++;
    stat = FFTInv_PackToR( img_32f_CA,   (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]), (Ipp8u*)(pBuffer+1) );    io++;
    stat = FFTInv_PackToR( img_32f_C4, *(IppiFFTSpec_R_32f**)(pFFTSpec_32s_useR_32f)   , (Ipp8u*)(pBuffer+1)-4 );  io++;
    delete[] pBuffer;

    stat = FFTFwd_CToC( src_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), 0 );  io++;
    pBuffer = new Ipp32s[pSize[9]];
    stat = FFTFwd_CToC( src_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), (Ipp8u*)pBuffer );  io++;
    stat = FFTInv_CToC( src_32fc_C1, (IppiFFTSpec_C_32fc*)(pFFTSpec_32s_useC_32fc[0]), (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;


// ---- FFT_FREE:

    stat = FFTFree(                       pFFTSpec_32s[0] );            io++;
    stat = FFTFree(  (IppiFFTSpec_R_32f*)(pFFTSpec_32s_useR_32f[0]) );  io++;
    stat = FFTFree( *(IppiFFTSpec_C_32fc**)(pFFTSpec_32s_useC_32fc) );  io++;


// ---- MAGNITUDE:

    Magnitude( src_16sc_C1, dst_16s_C1, scale );    io++;
    Magnitude( src_16sc_C3, dst_16s_C3, scale+3 );  io++;
    Magnitude( src_32sc_C1, dst_32s_C1, scale/3 );  io++;
    Magnitude( src_32sc_C3, dst_32s_C3, 3 );        io++;

    Magnitude( src_32fc_C1, dst_32f_C1 );  io++;
    Magnitude( src_32fc_C3, dst_32f_C3 );  io++;


// ---- MAGNITUDE_PACK:

    MagnitudePack( src_16s_C1, dst_16s_C1, scale );    io++;
    MagnitudePack( src_16s_C3, dst_16s_C3, scale+3 );  io++;
    MagnitudePack( src_32s_C1, dst_32s_C1, scale/3 );  io++;
    MagnitudePack( src_32s_C3, dst_32s_C3, 3 );        io++;

    MagnitudePack( src_32f_C1, dst_32f_C1 );  io++;
    MagnitudePack( src_32f_C3, dst_32f_C3 );  io++;


// ---- MULPACK:

// dst = src1 * src2  (NI)
    MulPack( src1_16s_C1, src2_16s_C1, dst_16s_C1, scale );    io++;
    MulPack( src1_16s_C3, src2_16s_C3, dst_16s_C3, scale+3 );  io++;
    MulPack( src1_16s_CA, src2_16s_CA, dst_16s_CA, scale/3 );  io++;
    MulPack( src1_16s_C4, src2_16s_C4, dst_16s_C4, 3 );        io++;

    MulPack( src1_32s_C1, src2_32s_C1, dst_32s_C1, scale );    io++;
    MulPack( src1_32s_C3, src2_32s_C3, dst_32s_C3, scale+3 );  io++;
    MulPack( src1_32s_CA, src2_32s_CA, dst_32s_CA, scale/3 );  io++;
    MulPack( src1_32s_C4, src2_32s_C4, dst_32s_C4, 3 );        io++;

    MulPack( src1_32f_C1, src2_32f_C1, dst_32f_C1 );  io++;
    MulPack( src1_32f_C3, src2_32f_C3, dst_32f_C3 );  io++;
    MulPack( src1_32f_CA, src2_32f_CA, dst_32f_CA );  io++;
    MulPack( src1_32f_C4, src2_32f_C4, dst_32f_C4 );  io++;

// img *= src  (I)
    MulPack( src_16s_C1, img_16s_C1, scale );    io++;
    MulPack( src_16s_C3, img_16s_C3, scale+3 );  io++;
    MulPack( src_16s_CA, img_16s_CA, scale/3 );  io++;
    MulPack( src_16s_C4, img_16s_C4, 3 );        io++;

    MulPack( src_32s_C1, img_32s_C1, scale );    io++;
    MulPack( src_32s_C3, img_32s_C3, scale+3 );  io++;
    MulPack( src_32s_CA, img_32s_CA, scale/3 );  io++;
    MulPack( src_32s_C4, img_32s_C4, 3 );        io++;

    MulPack( src_32f_C1, img_32f_C1 );  io++;
    MulPack( src_32f_C3, img_32f_C3 );  io++;
    MulPack( src_32f_CA, img_32f_CA );  io++;
    MulPack( src_32f_C4, img_32f_C4 );  io++;


// ---- MULPACK_CONJ:

//  (NI)
    MulPackConj( src1_32f_C1, src2_32f_C1, dst_32f_C1 );  io++;
    MulPackConj( src1_32f_C3, src2_32f_C3, dst_32f_C3 );  io++;
    MulPackConj( src1_32f_CA, src2_32f_CA, dst_32f_CA );  io++;
    MulPackConj( src1_32f_C4, src2_32f_C4, dst_32f_C4 );  io++;

//  (I)
    MulPackConj( src_32f_C1, dst_32f_C1 );  io++;
    MulPackConj( src_32f_C3, dst_32f_C3 );  io++;
    MulPackConj( src_32f_CA, dst_32f_CA );  io++;
    MulPackConj( src_32f_C4, dst_32f_C4 );  io++;


// ---- PACKtoCPLX_EXTEND:

    PackToCplxExtend( src_32s_C1, dst_32sc_C1 );  io++;
    PackToCplxExtend( src_32f_C1, dst_32fc_C1 );  io++;


// ---- PHASE:

    Phase( dst_16sc_C1, dst_16s_C1, scale );    io++;
    Phase( src_16sc_C3, dst_16s_C3, scale+3 );  io++;
    Phase( src_32sc_C1, dst_32s_C1, scale/3 );  io++;
    Phase( src_32sc_C3, dst_32s_C3, 3 );        io++;

    Phase( src_32fc_C1, dst_32f_C1 );  io++;
    Phase( src_32fc_C3, dst_32f_C3 );  io++;


// ---- PHASE_PACK:

    PhasePack( src_16s_C1, dst_16s_C1, scale );  io++;
    PhasePack( src_16s_C3, dst_16s_C3, scale );  io++;
    PhasePack( src_32s_C1, dst_32s_C1, scale );  io++;
    PhasePack( src_32s_C3, dst_32s_C3, scale );  io++;

    PhasePack( src_32f_C1, dst_32f_C1 );  io++;
    PhasePack( src_32f_C3, dst_32f_C3 );  io++;


/* #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A
#A                                                                                                                   #A
#A                                       -= Windowing Functions (from #10) =-                                        #A
#A                                                                                                                   #A
#A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A #A */

// ---- WIN_BARLETT:

// (NI)
    stat = WinBartlett( src_8u_C1 , dst_8u_C1 );   io++;
    stat = WinBartlett( src_32f_C1, dst_32f_C1 );  io++;

// (I):

    stat = WinBartlett( img_8u_C1 );   io++;
    stat = WinBartlett( img_32f_C1 );  io++;


// ---- WIN_BARLETT_SEP:

// (NI)
    stat = WinBartlettSep( src_8u_C1 , dst_8u_C1 );   io++;
    stat = WinBartlettSep( src_32f_C1, dst_32f_C1 );  io++;

// (I) (below 'src_...' images are in-place ones for simplicity):

    stat = WinBartlettSep( img_8u_C1 );   io++;
    stat = WinBartlettSep( img_32f_C1 );  io++;


// ---- WIN_HAMMING:

// (NI)
    stat = WinHamming( src_8u_C1 , dst_8u_C1 );   io++;
    stat = WinHamming( src_32f_C1, dst_32f_C1 );  io++;

// (I):

    stat = WinHamming( img_8u_C1 );   io++;
    stat = WinHamming( img_32f_C1 );  io++;


// ---- WIN_BARLETT_SEP:

// (NI)
    stat = WinHammingSep( src_8u_C1 , dst_8u_C1 );   io++;
    stat = WinHammingSep( src_32f_C1, dst_32f_C1 );  io++;

// (I:

    stat = WinHammingSep( img_8u_C1 );   io++;
    stat = WinHammingSep( img_32f_C1 );  io++;


/* #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B
#B              ___ ___   _  ___  _  ___ ___  _  ___  ___      ___ _  _  _  _  ___ ___  _   _   _  _  ___            #B
#B             /_    /  /_ /  /   / /_    /   / /    /_       /_   /  /  /| / /     /   / /  /  /| / /_              #B
#B            ___/  /  /  /  /  _/ ___/  /  _/ /__  ___/     /     \_/ _/ |/ /__   /  _/  \_/ _/ |/ ___/             #B
#B                                                                                                                   #B
#B                                    -- Image Statistics Functions (#11) --                                         #B
#B                                                                                                                   #B
#B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B */

// ---- COUNT_IN_RANGE:

    Set( *(LOWER_BOUND(8u)+3), img_8u_C1 );
    stat = CountInRange( img_8u_C1 ,  &COUNTS[3], *(LOWER_BOUND(8u)+3) , ((Ipp8u*)UPPER_BOUND(16s))[6] );    io++;
    Set(  0, img_8u_C1 );

    Set(  (LOWER_BOUND(8u)+1), img_8u_C3 );
    stat = CountInRange( img_8u_C3 ,   COUNTS   ,   LOWER_BOUND(8u)+1  , (Ipp8u*)&(UPPER_BOUND(16s)[1]) );   io++;
    Set(  LOWER_BOUND(8u), img_8u_C3 );

    Set(  (LOWER_BOUND(8u)+1), img_8u_CA );
    stat = CountInRange( img_8u_CA , &COUNTS[1] ,   LOWER_BOUND(8u)+1  , (Ipp8u*)&(UPPER_BOUND(16s)[1]) );   io++;
    Set(  LOWER_BOUND(8u), img_8u_CA );

    stat = CountInRange( img_32f_C1,  &COUNTS[3], *(LOWER_BOUND(32f)+3),   (Ipp32f)(UPPER_BOUND(64f)[3]) );  io++;
    stat = CountInRange( img_32f_C3,   COUNTS   ,   LOWER_BOUND(32f)+1 , (Ipp32f*)&(UPPER_BOUND(64f)[1]) );  io++;
    stat = CountInRange( img_32f_CA, &COUNTS[1] ,   LOWER_BOUND(32f)+1 , (Ipp32f*)&(UPPER_BOUND(64f)[1]) );  io++;


// ---- HISTOGRAM_RANGE:

    stat = HistogramRange( img_8u_C1 ,  pHist[3],        ((Ipp32s**)pLevels_32f)[3],  NLEVELS );     io++;
    stat = HistogramRange( img_8u_C3 , &pHist[1], &((const Ipp32s**)pLevels_32f)[1], &nLevels[1] );  io++;
    stat = HistogramRange( img_8u_CA ,  pHist+1 ,  ((const Ipp32s**)pLevels_32f)+1 , &nLevels[1] );  io++;
    stat = HistogramRange( img_8u_C4 ,  pHist   ,   (const Ipp32s**)pLevels_32f    ,  nLevels );     io++;

    stat = HistogramRange( img_16s_C1,  pHist[3],        ((Ipp32s**)pLevels_32f)[3],  nLevels[3] );  io++;
    stat = HistogramRange( img_16s_C3, &pHist[1], &((const Ipp32s**)pLevels_32f)[1], &nLevels[1] );  io++;
    stat = HistogramRange( img_16s_CA,  pHist+1 ,  ((const Ipp32s**)pLevels_32f)+1 , &nLevels[1] );  io++;
    stat = HistogramRange( img_16s_C4,  pHist   ,   (const Ipp32s**)pLevels_32f    ,  nLevels );     io++;

    stat = HistogramRange( img_32f_C1,  pHist[3],                   pLevels_32f[3] ,  nLevels[3] );  io++;
    stat = HistogramRange( img_32f_C3, &pHist[1],                  &pLevels_32f[1] , &nLevels[1] );  io++;
    stat = HistogramRange( img_32f_CA,  pHist+1 ,                   pLevels_32f+1  , &nLevels[1] );  io++;
    stat = HistogramRange( img_32f_C4,  pHist   ,                   pLevels_32f    ,  nLevels );     io++;


// ---- HISTOGRAM_EVEN:

    stat = HistogramEven( img_8u_C1 ,  pHist[3],  ((Ipp32s**)pLevels_32f)[3],  NLEVELS   ,  LOWER_LEVEL[3],  UPPER_LEVEL[3] );  io++;
    stat = HistogramEven( img_8u_C3 , &pHist[1], &((Ipp32s**)pLevels_32f)[1], &nLevels[1], &LOWER_LEVEL[1], &UPPER_LEVEL[1] );  io++;
    stat = HistogramEven( img_8u_CA ,  pHist+1 ,  ((Ipp32s**)pLevels_32f)+1 , &nLevels[1], &LOWER_LEVEL[0], &UPPER_LEVEL[0] );  io++;
    stat = HistogramEven( img_8u_C4 ,  pHist   ,   (Ipp32s**)pLevels_32f    ,  nLevels   ,  LOWER_LEVEL   ,  UPPER_LEVEL );     io++;

    stat = HistogramEven( img_16s_C1,  pHist[3],  ((Ipp32s**)pLevels_32f)[3],  nLevels[3],  LOWER_LEVEL[3],  UPPER_LEVEL[3] );  io++;
    stat = HistogramEven( img_16s_C3, &pHist[1], &((Ipp32s**)pLevels_32f)[1], &nLevels[1], &LOWER_LEVEL[1], &UPPER_LEVEL[1] );  io++;
    stat = HistogramEven( img_16s_CA,  pHist+1 ,  ((Ipp32s**)pLevels_32f)+1 , &nLevels[1], &LOWER_LEVEL[0], &UPPER_LEVEL[0] );  io++;
    stat = HistogramEven( img_16s_C4,  pHist   ,   (Ipp32s**)pLevels_32f    ,  nLevels   ,  LOWER_LEVEL   ,  UPPER_LEVEL );     io++;


// ---- MAX:

// Preparing U8 due data for simplicity of results checking (in debugger):
    stat = ImageRamp( img_8u_C1  , IPP_MIN_8U  , IPP_MAX_8U/(M-1.0f)    , ippAxsHorizontal );
    stat = ImageRamp( img_8u_C3  , IPP_MIN_8U  , IPP_MAX_8U/(M-1.0f)/2  , ippAxsHorizontal );
    stat = ImageRamp( img_8u_CA  , IPP_MIN_8U  , IPP_MAX_8U/(M-1.0f)/3  , ippAxsHorizontal );
    stat = ImageRamp( img_8u_C4  , IPP_MIN_8U  , IPP_MAX_8U/(M-1.0f)/4  , ippAxsHorizontal );

    stat = Max( img_8u_C1 , &MAX(8u)[3] );   io++;
    stat = Max( img_8u_C3 ,  MAX(8u) );      io++;
    stat = Max( img_8u_CA , &MAX(8u)[1] );   io++;
    stat = Max( img_8u_C4 ,  MAX(8u) );      io++;

    stat = Max( img_16s_C1, &MAX(16s)[3] );  io++;
    stat = Max( img_16s_C3,  MAX(16s) );     io++;
    stat = Max( img_16s_CA, &MAX(16s)[1] );  io++;
    stat = Max( img_16s_C4,  MAX(16s) );     io++;

    stat = Max( img_32f_C1, &MAX(32f)[3] );  io++;
    stat = Max( img_32f_C3,  MAX(32f) );     io++;
    stat = Max( img_32f_CA, &MAX(32f)[1] );  io++;
    stat = Max( img_32f_C4,  MAX(32f) );     io++;


// ---- MAX_INDEX:

    stat = MaxIndx( img_8u_C1 , &MAX(8u)[3] , &X1  , &Y1 );    io++;
    stat = MaxIndx( img_8u_C3 ,  MAX(8u)    , &X[1], &Y[1] );  io++;
    stat = MaxIndx( img_8u_CA , &MAX(8u)[1] , &X[0], &Y[0] );  io++;
    stat = MaxIndx( img_8u_C4 ,  MAX(8u)    ,  X,     Y );     io++;

    stat = MaxIndx( img_16s_C1, &MAX(16s)[3], &X1  , &Y1 );    io++;
    stat = MaxIndx( img_16s_C3,  MAX(16s)   , &X[1], &Y[1] );  io++;
    stat = MaxIndx( img_16s_CA, &MAX(16s)[1], &X[0], &Y[0] );  io++;
    stat = MaxIndx( img_16s_C4,  MAX(16s)   ,  X,     Y );     io++;

    stat = MaxIndx( img_32f_C1, &MAX(32f)[3], &X1  , &Y1 );    io++;
    stat = MaxIndx( img_32f_C3,  MAX(32f)   , &X[1], &Y[1] );  io++;
    stat = MaxIndx( img_32f_CA, &MAX(32f)[1], &X[0], &Y[0] );  io++;
    stat = MaxIndx( img_32f_C4,  MAX(32f)   ,  X,     Y );     io++;


// ---- MEAN:

// Preparing due data for simplicity of results checking (in debugger):
    stat = ImageRamp( img_8u_C1  , IPP_MIN_8U  , IPP_MAX_8U/(M-1.0f) , ippAxsHorizontal );
    stat = ImageRamp( img_16s_C1 , IPP_MIN_16S , IPP_MAX_16U/(M-1.0f), ippAxsHorizontal );
    stat = ImageRamp( img_32f_C1 , IPP_MIN_32S , IPP_MAX_32U/(M-1.0f), ippAxsHorizontal );

    stat = Mean( img_8u_C1 , &MEAN(64f)[3] );  io++;
    stat = Mean( img_8u_C3 ,  MEAN(64f) );     io++;
    stat = Mean( img_8u_CA , &MEAN(64f)[1] );  io++;
    stat = Mean( img_8u_C4 ,  MEAN(64f) );     io++;

    stat = Mean( img_16s_C1, &MEAN(64f)[3] );  io++;
    stat = Mean( img_16s_C3,  MEAN(64f) );     io++;
    stat = Mean( img_16s_CA, &MEAN(64f)[1] );  io++;
    stat = Mean( img_16s_C4,  MEAN(64f) );     io++;

    stat = Mean( img_32f_C1, &MEAN(64f)[3], ippAlgHintNone );      io++;
    stat = Mean( img_32f_C3,  MEAN(64f)   , ippAlgHintAccurate );  io++;
    stat = Mean( img_32f_CA, &MEAN(64f)[1], ippAlgHintFast );      io++;
    stat = Mean( img_32f_C4,  MEAN(64f)   , ippAlgHintAccurate );  io++;


// ---- MIN:

// Preparing U8 due data for simplicity of results checking (in debugger):
    stat = ImageRamp( img_8u_C1, IPP_MIN_8U  , IPP_MAX_8U/(N-1.0f), ippAxsBoth );
    stat = ImageRamp( img_8u_C3, IPP_MIN_8U+1, IPP_MAX_8U/(N-1.0f), ippAxsBoth );
    stat = ImageRamp( img_8u_CA, IPP_MIN_8U+2, IPP_MAX_8U/(N-1.0f), ippAxsBoth );
    stat = ImageRamp( img_8u_C4, IPP_MIN_8U+3, IPP_MAX_8U/(N-1.0f), ippAxsBoth );

    stat = Min( img_8u_C1 , &MIN(8u)[3] );   io++;
    stat = Min( img_8u_C3 ,  MIN(8u) );      io++;
    stat = Min( img_8u_CA , &MIN(8u)[1] );   io++;
    stat = Min( img_8u_C4 ,  MIN(8u) );      io++;

    stat = Min( img_16s_C1, &MIN(16s)[3] );  io++;
    stat = Min( img_16s_C3,  MIN(16s) );     io++;
    stat = Min( img_16s_CA, &MIN(16s)[1] );  io++;
    stat = Min( img_16s_C4,  MIN(16s) );     io++;

    stat = Min( img_32f_C1, &MIN(32f)[3] );  io++;
    stat = Min( img_32f_C3,  MIN(32f) );     io++;
    stat = Min( img_32f_CA, &MIN(32f)[1] );  io++;
    stat = Min( img_32f_C4,  MIN(32f) );     io++;


// ---- MIN_INDEX:

    stat = MinIndx( img_8u_C1 , &value_8u[3], &X1  , &Y1 );    io++;
    stat = MinIndx( img_8u_C3 ,  value_8u   , &X[1], &Y[1] );  io++;
    stat = MinIndx( img_8u_CA , &value_8u[1], &X[0], &Y[0] );  io++;
    stat = MinIndx( img_8u_C4 ,  value_8u   ,  X,     Y );     io++;

    stat = MinIndx( img_16s_C1, &MIN(16s)[3], &X1  , &Y1 );    io++;
    stat = MinIndx( img_16s_C3,  MIN(16s)   , &X[1], &Y[1] );  io++;
    stat = MinIndx( img_16s_CA, &MIN(16s)[1], &X[0], &Y[0] );  io++;
    stat = MinIndx( img_16s_C4,  MIN(16s)   ,  X,     Y );     io++;

    stat = MinIndx( img_32f_C1, &MIN(32f)[3], &X1  , &Y1 );    io++;
    stat = MinIndx( img_32f_C3,  MIN(32f)   , &X[1], &Y[1] );  io++;
    stat = MinIndx( img_32f_CA, &MIN(32f)[1], &X[0], &Y[0] );  io++;
    stat = MinIndx( img_32f_C4,  MIN(32f)   ,  X,     Y );     io++;


// ---- MOMENTS_64F:

    stat = ippiMomentInitAlloc_64f( pMomentState_64f, ippAlgHintAccurate );

    stat = Moments64f( img_8u_C1 , pMomentState_64f[0] );  io++;
    stat = Moments64f( img_8u_C3 , pMomentState_64f[0] );  io++;
    stat = Moments64f( img_8u_CA , pMomentState_64f[0] );  io++;

    stat = Moments64f( img_32f_C1, pMomentState_64f[0] );  io++;
    stat = Moments64f( img_32f_C3, pMomentState_64f[0] );  io++;
    stat = Moments64f( img_32f_CA, pMomentState_64f[0] );  io++;

    stat = ippiMomentFree_64f( pMomentState_64f[0] );


// ---- MOMENTS_64S:

    stat = ippiMomentInitAlloc_64s( pMomentState_64s, ippAlgHintNone );

    stat = Moments64s( img_8u_C1, pMomentState_64s[0] );  io++;
    stat = Moments64s( img_8u_C3, pMomentState_64s[0] );  io++;
    stat = Moments64s( img_8u_CA, pMomentState_64s[0] );  io++;

    stat = ippiMomentFree_64s( pMomentState_64s[0] );


// ---- QUALITY_INDEX:

    stat = QualityIndex( src1_8u_C1 , src2_8u_C1 , &value_32f[3] );  io++;
    stat = QualityIndex( src1_8u_C3 , src2_8u_C3 ,  value_32f );     io++;
    stat = QualityIndex( src1_8u_CA , src2_8u_CA , &value_32f[1] );  io++;

    stat = QualityIndex( src1_32f_C1, src2_32f_C1, &value_32f[3] );  io++;
    stat = QualityIndex( src1_32f_C3, src2_32f_C3,  value_32f );     io++;
    stat = QualityIndex( src1_32f_CA, src2_32f_CA, &value_32f[1] );  io++;


// ---- SUM:

    stat = Sum( img_8u_C1 , &value_64f[3] );  io++;
    stat = Sum( img_8u_C3 ,  value_64f );     io++;
    stat = Sum( img_8u_CA , &value_64f[1] );  io++;
    stat = Sum( img_8u_C4 ,  value_64f );     io++;

    stat = Sum( img_16s_C1, &value_64f[3] );  io++;
    stat = Sum( img_16s_C3,  value_64f );     io++;
    stat = Sum( img_16s_CA, &value_64f[1] );  io++;
    stat = Sum( img_16s_C4,  value_64f );     io++;

    stat = Sum( img_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = Sum( img_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = Sum( img_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = Sum( img_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


/* #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B
#B                                                                                                                   #B
#B                                        -- Image Norms (from #11) --                                               #B
#B                                                                                                                   #B
#B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B */

// ---- NORM__INF:

    stat = Norm_Inf( img_8u_C1 , &value_64f[3] );  io++;
    stat = Norm_Inf( img_8u_C3 ,  value_64f );     io++;
    stat = Norm_Inf( img_8u_CA , &value_64f[1] );  io++;
    stat = Norm_Inf( img_8u_C4 ,  value_64f );     io++;

    stat = Norm_Inf( img_16s_C1, &value_64f[3] );  io++;
    stat = Norm_Inf( img_16s_C3,  value_64f );     io++;
    stat = Norm_Inf( img_16s_CA, &value_64f[1] );  io++;
    stat = Norm_Inf( img_16s_C4,  value_64f );     io++;

    stat = Norm_Inf( img_32s_C1,  &norm );         io++;

    stat = Norm_Inf( img_32f_C1, &value_64f[3] );  io++;
    stat = Norm_Inf( img_32f_C3,  value_64f );     io++;
    stat = Norm_Inf( img_32f_CA, &value_64f[1] );  io++;
    stat = Norm_Inf( img_32f_C4,  value_64f );     io++;


// ---- NORM__L1:

    stat = Norm_L1( img_8u_C1 , &value_64f[3] );  io++;
    stat = Norm_L1( img_8u_C3 ,  value_64f );     io++;
    stat = Norm_L1( img_8u_CA , &value_64f[1] );  io++;
    stat = Norm_L1( img_8u_C4 ,  value_64f );     io++;

    stat = Norm_L1( img_16s_C1, &value_64f[3] );  io++;
    stat = Norm_L1( img_16s_C3,  value_64f );     io++;
    stat = Norm_L1( img_16s_CA, &value_64f[1] );  io++;
    stat = Norm_L1( img_16s_C4,  value_64f );     io++;

    stat = Norm_L1( img_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = Norm_L1( img_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = Norm_L1( img_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = Norm_L1( img_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


// ---- NORM__L2:

    stat = Norm_L2( img_8u_C1 , &value_64f[3] );  io++;
    stat = Norm_L2( img_8u_C3 ,  value_64f );     io++;
    stat = Norm_L2( img_8u_CA , &value_64f[1] );  io++;
    stat = Norm_L2( img_8u_C4 ,  value_64f );     io++;

    stat = Norm_L2( img_16s_C1, &value_64f[3] );  io++;
    stat = Norm_L2( img_16s_C3,  value_64f );     io++;
    stat = Norm_L2( img_16s_CA, &value_64f[1] );  io++;
    stat = Norm_L2( img_16s_C4,  value_64f );     io++;

    stat = Norm_L2( img_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = Norm_L2( img_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = Norm_L2( img_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = Norm_L2( img_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


// ---- NORMS_DIFF__INF:

    stat = NormDiff_Inf( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormDiff_Inf( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormDiff_Inf( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormDiff_Inf( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormDiff_Inf( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormDiff_Inf( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormDiff_Inf( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormDiff_Inf( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormDiff_Inf( src1_32f_C1, src2_32f_C1, &value_64f[3] );  io++;
    stat = NormDiff_Inf( src1_32f_C3, src2_32f_C3,  value_64f );     io++;
    stat = NormDiff_Inf( src1_32f_CA, src2_32f_CA, &value_64f[1] );  io++;
    stat = NormDiff_Inf( src1_32f_C4, src2_32f_C4,  value_64f );     io++;


// ---- NORMS_DIFF__L1:

    stat = NormDiff_L1( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormDiff_L1( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormDiff_L1( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormDiff_L1( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormDiff_L1( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormDiff_L1( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormDiff_L1( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormDiff_L1( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormDiff_L1( src1_32f_C1, src2_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = NormDiff_L1( src1_32f_C3, src2_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = NormDiff_L1( src1_32f_CA, src2_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = NormDiff_L1( src1_32f_C4, src2_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


// ---- NORMS_DIFF__L2:

    stat = NormDiff_L2( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormDiff_L2( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormDiff_L2( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormDiff_L2( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormDiff_L2( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormDiff_L2( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormDiff_L2( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormDiff_L2( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormDiff_L2( src1_32f_C1, src2_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = NormDiff_L2( src1_32f_C3, src2_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = NormDiff_L2( src1_32f_CA, src2_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = NormDiff_L2( src1_32f_C4, src2_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


// ---- NORMS_REL_INF:

    stat = NormRel_Inf( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormRel_Inf( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormRel_Inf( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormRel_Inf( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormRel_Inf( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormRel_Inf( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormRel_Inf( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormRel_Inf( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormRel_Inf( src1_32f_C1, src2_32f_C1, &value_64f[3] );  io++;
    stat = NormRel_Inf( src1_32f_C3, src2_32f_C3,  value_64f );     io++;
    stat = NormRel_Inf( src1_32f_CA, src2_32f_CA, &value_64f[1] );  io++;
    stat = NormRel_Inf( src1_32f_C4, src2_32f_C4,  value_64f );     io++;


// ---- NORMS_REL_L1:

    stat = NormRel_L1( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormRel_L1( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormRel_L1( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormRel_L1( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormRel_L1( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormRel_L1( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormRel_L1( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormRel_L1( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormRel_L1( src1_32f_C1, src2_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = NormRel_L1( src1_32f_C3, src2_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = NormRel_L1( src1_32f_CA, src2_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = NormRel_L1( src1_32f_C4, src2_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


// ---- NORMS_REL_L2:

    stat = NormRel_L2( src1_8u_C1 , src2_8u_C1 , &value_64f[3] );  io++;
    stat = NormRel_L2( src1_8u_C3 , src2_8u_C3 ,  value_64f );     io++;
    stat = NormRel_L2( src1_8u_CA , src2_8u_CA , &value_64f[1] );  io++;
    stat = NormRel_L2( src1_8u_C4 , src2_8u_C4 ,  value_64f );     io++;

    stat = NormRel_L2( src1_16s_C1, src2_16s_C1, &value_64f[3] );  io++;
    stat = NormRel_L2( src1_16s_C3, src2_16s_C3,  value_64f );     io++;
    stat = NormRel_L2( src1_16s_CA, src2_16s_CA, &value_64f[1] );  io++;
    stat = NormRel_L2( src1_16s_C4, src2_16s_C4,  value_64f );     io++;

    stat = NormRel_L2( src1_32f_C1, src2_32f_C1, &value_64f[3], ippAlgHintNone );      io++;
    stat = NormRel_L2( src1_32f_C3, src2_32f_C3,  value_64f   , ippAlgHintAccurate );  io++;
    stat = NormRel_L2( src1_32f_CA, src2_32f_CA, &value_64f[1], ippAlgHintFast );      io++;
    stat = NormRel_L2( src1_32f_C4, src2_32f_C4,  value_64f   , ippAlgHintAccurate );  io++;


/* #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B
#B                                                                                                                   #B
#B                                  -= Image Proximity Measures (from #11) =-                                        #B
#B                                                                                                                   #B
#B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B #B */

// Src images preparing:

    src1_8u_C1.roiSize.set(  srcNorm );
    src1_8u_C3.roiSize.set(  srcNorm );
    src1_8u_CA.roiSize.set(  srcNorm );
    src1_8u_C4.roiSize.set(  srcNorm );

    src2_8u_C1.roiSize.set(  tplNorm );
    src2_8u_C3.roiSize.set(  tplNorm );
    src2_8u_CA.roiSize.set(  tplNorm );
    src2_8u_C4.roiSize.set(  tplNorm );

    src1_32f_C1.roiSize.set( srcNorm );
    src1_32f_C3.roiSize.set( srcNorm );
    src1_32f_CA.roiSize.set( srcNorm );
    src1_32f_C4.roiSize.set( srcNorm );

    src2_32f_C1.roiSize.set( tplNorm );
    src2_32f_C3.roiSize.set( tplNorm );
    src2_32f_CA.roiSize.set( tplNorm );
    src2_32f_C4.roiSize.set( tplNorm );

// Dst images preparing:

    dst_8u_C1.roiSize.set(  srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_8u_C3.roiSize.set(  srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_8u_CA.roiSize.set(  srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_8u_C4.roiSize.set(  srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );

    dst_32f_C1.roiSize.set( srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_32f_C3.roiSize.set( srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_32f_CA.roiSize.set( srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );
    dst_32f_C4.roiSize.set( srcNorm.width+tplNorm.width-1, srcNorm.height+tplNorm.height-1 );

// ---- CROSS_CORR_FULL__NORM:

    CrossCorrFull_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrFull_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrFull_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrFull_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrFull_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrFull_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrFull_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrFull_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrFull_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrFull_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrFull_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrFull_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrFull_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrFull_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrFull_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrFull_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- CROSS_CORR_FULL__NORM_LEVEL:

    CrossCorrFull_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrFull_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrFull_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrFull_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrFull_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrFull_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrFull_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrFull_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrFull_NormLevel( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrFull_NormLevel( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrFull_NormLevel( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrFull_NormLevel( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrFull_NormLevel(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrFull_NormLevel(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrFull_NormLevel(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrFull_NormLevel(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- SQR_DISTANCE_FULL__NORM:

    SqrDistanceFull_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    SqrDistanceFull_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    SqrDistanceFull_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    SqrDistanceFull_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    SqrDistanceFull_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceFull_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceFull_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceFull_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceFull_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceFull_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceFull_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceFull_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceFull_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    SqrDistanceFull_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    SqrDistanceFull_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    SqrDistanceFull_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;

// Dst images preparing:

    dst_8u_C1.roiSize.set(  srcNorm );
    dst_8u_C3.roiSize.set(  srcNorm );
    dst_8u_CA.roiSize.set(  srcNorm );
    dst_8u_C4.roiSize.set(  srcNorm );

    dst_32f_C1.roiSize.set( srcNorm );
    dst_32f_C3.roiSize.set( srcNorm );
    dst_32f_CA.roiSize.set( srcNorm );
    dst_32f_C4.roiSize.set( srcNorm );

// ---- CROSS_CORR_SAME__NORM:

    CrossCorrSame_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrSame_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrSame_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrSame_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrSame_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrSame_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrSame_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrSame_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrSame_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrSame_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrSame_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrSame_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrSame_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrSame_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrSame_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrSame_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- CROSS_CORR_SAME__NORM_LEVEL:

    CrossCorrSame_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrSame_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrSame_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrSame_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrSame_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrSame_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrSame_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrSame_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrSame_NormLevel( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrSame_NormLevel( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrSame_NormLevel( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrSame_NormLevel( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrSame_NormLevel(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrSame_NormLevel(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrSame_NormLevel(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrSame_NormLevel(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- SQR_DISTANCE_SAME__NORM:

    SqrDistanceSame_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    SqrDistanceSame_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    SqrDistanceSame_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    SqrDistanceSame_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    SqrDistanceSame_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceSame_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceSame_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceSame_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceSame_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceSame_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceSame_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceSame_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceSame_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    SqrDistanceSame_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    SqrDistanceSame_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    SqrDistanceSame_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;

// Dst images preparing:

    dst_8u_C1.roiSize.set(  srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_8u_C3.roiSize.set(  srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_8u_CA.roiSize.set(  srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_8u_C4.roiSize.set(  srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );

    dst_32f_C1.roiSize.set( srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_32f_C3.roiSize.set( srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_32f_CA.roiSize.set( srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );
    dst_32f_C4.roiSize.set( srcNorm.width-tplNorm.width+1, srcNorm.height-tplNorm.height+1 );

// ---- CROSS_CORR_VALID__NORM:

    CrossCorrValid_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrValid_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrValid_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrValid_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrValid_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrValid_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrValid_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrValid_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrValid_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrValid_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrValid_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrValid_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrValid_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrValid_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrValid_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrValid_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- CROSS_CORR_VALID__NORM_LEVEL:

    CrossCorrValid_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    CrossCorrValid_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    CrossCorrValid_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    CrossCorrValid_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    CrossCorrValid_NormLevel(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrValid_NormLevel(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrValid_NormLevel(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrValid_NormLevel(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrValid_NormLevel( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    CrossCorrValid_NormLevel( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    CrossCorrValid_NormLevel( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    CrossCorrValid_NormLevel( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    CrossCorrValid_NormLevel(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    CrossCorrValid_NormLevel(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    CrossCorrValid_NormLevel(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    CrossCorrValid_NormLevel(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;


// ---- SQR_DISTANCE_VALID__NORM:

    SqrDistanceValid_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_8u_C1, scale );  io++;
    SqrDistanceValid_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_8u_C3, scale );  io++;
    SqrDistanceValid_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_8u_CA, scale );  io++;
    SqrDistanceValid_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_8u_C4, scale );  io++;

    SqrDistanceValid_Norm(                 src1_8u_C1 ,                 src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceValid_Norm(                 src1_8u_C3 ,                 src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceValid_Norm(                 src1_8u_CA ,                 src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceValid_Norm(                 src1_8u_C4 ,                 src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceValid_Norm( *(GIMG<Ipp8s>*)&src1_8u_C1 , *(GIMG<Ipp8s>*)&src2_8u_C1 , dst_32f_C1 );        io++;
    SqrDistanceValid_Norm( *(IMG3<Ipp8s>*)&src1_8u_C3 , *(IMG3<Ipp8s>*)&src2_8u_C3 , dst_32f_C3 );        io++;
    SqrDistanceValid_Norm( *(AIMG<Ipp8s>*)&src1_8u_CA , *(AIMG<Ipp8s>*)&src2_8u_CA , dst_32f_CA );        io++;
    SqrDistanceValid_Norm( *(IMG4<Ipp8s>*)&src1_8u_C4 , *(IMG4<Ipp8s>*)&src2_8u_C4 , dst_32f_C4 );        io++;

    SqrDistanceValid_Norm(                 src1_32f_C1,                 src2_32f_C1, dst_32f_C1 );        io++;
    SqrDistanceValid_Norm(                 src1_32f_C3,                 src2_32f_C3, dst_32f_C3 );        io++;
    SqrDistanceValid_Norm(                 src1_32f_CA,                 src2_32f_CA, dst_32f_CA );        io++;
    SqrDistanceValid_Norm(                 src1_32f_C4,                 src2_32f_C4, dst_32f_C4 );        io++;

// Images restoring:
    src1_8u_C1.restoreRect();
    src1_8u_C3.restoreRect();
    src1_8u_CA.restoreRect();
    src1_8u_C4.restoreRect();

    src2_8u_C1.restoreRect();
    src2_8u_C3.restoreRect();
    src2_8u_CA.restoreRect();
    src2_8u_C4.restoreRect();

    src1_32f_C1.restoreRect();
    src1_32f_C3.restoreRect();
    src1_32f_CA.restoreRect();
    src1_32f_C4.restoreRect();

    src2_32f_C1.restoreRect();
    src2_32f_C3.restoreRect();
    src2_32f_CA.restoreRect();
    src2_32f_C4.restoreRect();

    dst_8u_C1.restoreRect();
    dst_8u_C3.restoreRect();
    dst_8u_CA.restoreRect();
    dst_8u_C4.restoreRect();

    dst_32f_C1.restoreRect();
    dst_32f_C3.restoreRect();
    dst_32f_CA.restoreRect();
    dst_32f_C4.restoreRect();


/* #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C
#C           ___  ___  _  _  _   ___ ___  __   _  ___    ___  __    _  _  _  ___  ___   _   __  _  _   ___           #C
#C          /    /_  /  / /|/|  /_    /  /_ /  / /        /  /_ / /_ / /| / /_   /_   /  / /_ / /|/|  /_             #C
#C         /__- /__  \_/ / | | /__   /  /  \ _/ /__      /  /  \ /  / / |/ ___/ /     \_/ /  \ / | | ___/            #C
#C                                                                                                                   #C
#C                                     < Image Geometric Transforms (#12) >                                          #C
#C                                                                                                                   #C
#C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C #C */


// ---- MIRROR:

// dst = crs  (NI)
    Mirror( src_8u_C1 , dst_8u_C1 , ippAxsHorizontal );  io++;
    Mirror( src_8u_C3 , dst_8u_C3 , ippAxsVertical );    io++;
    Mirror( src_8u_CA , dst_8u_CA , ippAxsVertical );    io++;
    Mirror( src_8u_C4 , dst_8u_C4 , ippAxsBoth );        io++;

    Mirror( src_16u_C1, dst_16u_C1, ippAxsHorizontal );  io++;
    Mirror( src_16u_C3, dst_16u_C3, ippAxsVertical );    io++;
    Mirror( src_16u_CA, dst_16u_CA, ippAxsVertical );    io++;
    Mirror( src_16u_C4, dst_16u_C4, ippAxsBoth );        io++;

    Mirror( src_32s_C1, dst_32s_C1, ippAxsHorizontal );  io++;
    Mirror( src_32s_C3, dst_32s_C3, ippAxsVertical );    io++;
    Mirror( src_32s_CA, dst_32s_CA, ippAxsVertical );    io++;
    Mirror( src_32s_C4, dst_32s_C4, ippAxsBoth );        io++;

// img = gmi  (I)
    Mirror( img_8u_C1 , ippAxsHorizontal );  io++;
    Mirror( img_8u_C3 , ippAxsVertical );    io++;
    Mirror( img_8u_CA , ippAxsVertical );    io++;
    Mirror( img_8u_C4 , ippAxsBoth );        io++;

    Mirror( img_16u_C1, ippAxsHorizontal );  io++;
    Mirror( img_16u_C3, ippAxsVertical );    io++;
    Mirror( img_16u_CA, ippAxsVertical );    io++;
    Mirror( img_16u_C4, ippAxsBoth );        io++;

    Mirror( img_32s_C1, ippAxsHorizontal );  io++;
    Mirror( img_32s_C3, ippAxsVertical );    io++;
    Mirror( img_32s_CA, ippAxsVertical );    io++;
    Mirror( img_32s_C4, ippAxsBoth );        io++;


// ---- REMAP:

// Pixel-order data
    stat = Remap( src_8u_C1 , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_C1 , IPPI_INTER_NN );      io++;
    stat = Remap( src_8u_C3 , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_C3 , IPPI_INTER_LINEAR );  io++;
    stat = Remap( src_8u_CA , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_CA , IPPI_INTER_CUBIC );   io++;
    stat = Remap( src_8u_C4 , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_C4 , 3 );                  io++;

    stat = Remap( src_32f_C1, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_C1, IPPI_INTER_NN );      io++;
    stat = Remap( src_32f_C3, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_C3, IPPI_INTER_LINEAR );  io++;
    stat = Remap( src_32f_CA, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_CA, IPPI_INTER_CUBIC );   io++;
    stat = Remap( src_32f_C4, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_C4, 3 );                  io++;

// Planar-order data (P)
    stat = Remap( src_8u_P3 , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_P3 , IPPI_INTER_LINEAR );  io++;
    stat = Remap( src_8u_P4 , srcRect, aux1_32f_C1, aux2_32f_C1, dst_8u_P4 , 3 );                  io++;

    stat = Remap( src_32f_P3, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_P3, IPPI_INTER_LINEAR );  io++;
    stat = Remap( src_32f_P4, srcRect, aux1_32f_C1, aux2_32f_C1, dst_32f_P4, 3 );                  io++;


// ---- ROTATE:

// Pixel-order data
    stat = Rotate( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, 90,  0,  0, IPPI_INTER_NN );                                   io++;
    stat = Rotate( src_8u_C3 , srcRect, dst_8u_C3 , dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = Rotate( src_8u_CA , srcRect, dst_8u_CA , dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = Rotate( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

    stat = Rotate( src_16u_C1, srcRect, dst_16u_C1, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = Rotate( src_16u_C3, srcRect, dst_16u_C3, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = Rotate( src_16u_CA, srcRect, dst_16u_CA, dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = Rotate( src_16u_C4, srcRect, dst_16u_C4, dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

    stat = Rotate( src_32f_C1, srcRect, dst_32f_C1, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = Rotate( src_32f_C3, srcRect, dst_32f_C3, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = Rotate( src_32f_CA, srcRect, dst_32f_CA, dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = Rotate( src_32f_C4, srcRect, dst_32f_C4, dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

// Planar-order data (P)
    stat = Rotate( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, 90, 0, 0, IPPI_INTER_NN );                                  io++;
    stat = Rotate( src_8u_P4 , srcRect, dst_8u_P4 , dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;

    stat = Rotate( src_16u_P3, srcRect, dst_16u_P3, dstRect, 90, 0, 0, IPPI_INTER_NN );                                  io++;
    stat = Rotate( src_16u_P4, srcRect, dst_16u_P4, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;

    stat = Rotate( src_32f_P3, srcRect, dst_32f_P3, dstRect, 90, 0, 0, IPPI_INTER_NN );                                  io++;
    stat = Rotate( src_32f_P4, srcRect, dst_32f_P4, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;


// ---- ROTATE_CENTER:

// Pixel-order data
    stat = RotateCenter( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_8u_C3 , srcRect, dst_8u_C3 , dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = RotateCenter( src_8u_CA , srcRect, dst_8u_CA , dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = RotateCenter( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

    stat = RotateCenter( src_16u_C1, srcRect, dst_16u_C1, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_16u_C3, srcRect, dst_16u_C3, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = RotateCenter( src_16u_CA, srcRect, dst_16u_CA, dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = RotateCenter( src_16u_C4, srcRect, dst_16u_C4, dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

    stat = RotateCenter( src_32f_C1, srcRect, dst_32f_C1, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_32f_C3, srcRect, dst_32f_C3, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;
    stat = RotateCenter( src_32f_CA, srcRect, dst_32f_CA, dstRect, -val_64f, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = RotateCenter( src_32f_C4, srcRect, dst_32f_C4, dstRect, IPP_PI2/3/IPP_PI180, MxN, 3 );                                 io++;

// Planar-order data (P)
    stat = RotateCenter( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_8u_P4 , srcRect, dst_8u_P4 , dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;

    stat = RotateCenter( src_16u_P3, srcRect, dst_16u_P3, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_16u_P4, srcRect, dst_16u_P4, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;

    stat = RotateCenter( src_32f_P3, srcRect, dst_32f_P3, dstRect, 90, 0, 0, IPPI_INTER_NN );                                     io++;
    stat = RotateCenter( src_32f_P4, srcRect, dst_32f_P4, dstRect,  0, srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );     io++;


// ---- SHEAR:

// Pixel-order data
    stat = Shear( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, 0, 0, 0, 0, IPPI_INTER_NN );                              io++;
    stat = Shear( src_8u_C3 , srcRect, dst_8u_C3 , dstRect, fmod((Ipp64f)roiSize.width,val_64f), fmod((Ipp64f)roiSize.height,val_64f), srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;
    stat = Shear( src_8u_CA , srcRect, dst_8u_CA , dstRect, MxN, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = Shear( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, -roiSize.width, roiSize.height, MxN, 3 );                 io++;

    stat = Shear( src_32f_C1, srcRect, dst_32f_C1, dstRect, 0, 0, 0, 0, IPPI_INTER_NN );                              io++;
    stat = Shear( src_32f_C3, srcRect, dst_32f_C3, dstRect, fmod((Ipp64f)roiSize.width,val_64f), fmod((Ipp64f)roiSize.height,val_64f), srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;
    stat = Shear( src_32f_CA, srcRect, dst_32f_CA, dstRect, MxN, -roiSize.width, roiSize.height, IPPI_INTER_CUBIC );  io++;
    stat = Shear( src_32f_C4, srcRect, dst_32f_C4, dstRect, -roiSize.width, roiSize.height, MxN, 3 );                 io++;

// Planar-order data (P)
    stat = Shear( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, 0, 0, 0, 0, IPPI_INTER_NN );                              io++;
    stat = Shear( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, fmod((Ipp64f)roiSize.width,val_64f), fmod((Ipp64f)roiSize.height,val_64f), srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;

    stat = Shear( src_32f_P3, srcRect, dst_32f_P3, dstRect, 0, 0, 0, 0, IPPI_INTER_NN );                              io++;
    stat = Shear( src_32f_P4, srcRect, dst_32f_P4, dstRect, fmod((Ipp64f)roiSize.width,val_64f), fmod((Ipp64f)roiSize.height,val_64f), srcRect.x/2, (double)srcRect.y/2, IPPI_INTER_LINEAR );  io++;


// ---- WARP_AFFINE:

// Pixel-order data
    stat = WarpAffine( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffine( src_8u_C3 , srcRect, dst_8u_C3 , dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffine( src_8u_CA , srcRect, dst_8u_CA , dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffine( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, coeffs_2x3, 3 );                  io++;

    stat = WarpAffine( src_16u_C1, srcRect, dst_16u_C1, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffine( src_16u_C3, srcRect, dst_16u_C3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffine( src_16u_CA, srcRect, dst_16u_CA, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffine( src_16u_C4, srcRect, dst_16u_C4, dstRect, coeffs_2x3, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffine( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffine( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffine( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffine( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_2x3, 3 );                  io++;

// Planar-order data (P)
    stat = WarpAffine( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffine( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;

    stat = WarpAffine( src_16u_P3, srcRect, dst_16u_P3, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffine( src_16u_P4, srcRect, dst_16u_P4, dstRect, coeffs_2x3, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffine( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffine( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;


// ---- WARP_AFFINE_BACK:

// Pixel-order data
    stat = WarpAffineBack( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffineBack( src_8u_C3 , srcRect, dst_8u_C3 , dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineBack( src_8u_CA , srcRect, dst_8u_CA , dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineBack( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, coeffs_2x3, 3 );                  io++;

    stat = WarpAffineBack( src_16u_C1, srcRect, dst_16u_C1, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffineBack( src_16u_C3, srcRect, dst_16u_C3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineBack( src_16u_CA, srcRect, dst_16u_CA, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineBack( src_16u_C4, srcRect, dst_16u_C4, dstRect, coeffs_2x3, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffineBack( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffineBack( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineBack( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineBack( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_2x3, 3 );                  io++;

// Planar-order data (P)
    stat = WarpAffineBack( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineBack( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;

    stat = WarpAffineBack( src_16u_P3, srcRect, dst_16u_P3, dstRect, coeffs_2x3, IPPI_INTER_NN );      io++;
    stat = WarpAffineBack( src_16u_P4, srcRect, dst_16u_P4, dstRect, coeffs_2x3, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffineBack( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_2x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineBack( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_2x3, IPPI_INTER_CUBIC );   io++;


// ---- WARP_AFFINE_QUAD:

// Pixel-order data
    stat = WarpAffineQuad( src_8u_C1 , srcRect, srcQuad, dst_8u_C1 , dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpAffineQuad( src_8u_C3 , srcRect, srcQuad, dst_8u_C3 , dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineQuad( src_8u_CA , srcRect, srcQuad, dst_8u_CA , dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineQuad( src_8u_C4 , srcRect, srcQuad, dst_8u_C4 , dstRect, dstQuad, 3 );                  io++;

    stat = WarpAffineQuad( src_16u_C1, srcRect, srcQuad, dst_16u_C1, dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpAffineQuad( src_16u_C3, srcRect, srcQuad, dst_16u_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineQuad( src_16u_CA, srcRect, srcQuad, dst_16u_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineQuad( src_16u_C4, srcRect, srcQuad, dst_16u_C4, dstRect, dstQuad, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffineQuad( src_32f_C1, srcRect, srcQuad, dst_32f_C1, dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpAffineQuad( src_32f_C3, srcRect, srcQuad, dst_32f_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineQuad( src_32f_CA, srcRect, srcQuad, dst_32f_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;
    stat = WarpAffineQuad( src_32f_C4, srcRect, srcQuad, dst_32f_C4, dstRect, dstQuad, 3 );                  io++;

// Planar-order data (P)
    stat = WarpAffineQuad( src_8u_P3 , srcRect, srcQuad, dst_8u_P3 , dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineQuad( src_8u_P4 , srcRect, srcQuad, dst_8u_P4 , dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;

    stat = WarpAffineQuad( src_16u_P3, srcRect, srcQuad, dst_16u_P3, dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpAffineQuad( src_16u_P4, srcRect, srcQuad, dst_16u_P4, dstRect, dstQuad, IPPI_SMOOTH_EDGE );   io++;

    stat = WarpAffineQuad( src_32f_P3, srcRect, srcQuad, dst_32f_P3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpAffineQuad( src_32f_P4, srcRect, srcQuad, dst_32f_P4, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;


// ---- WARP_BILINEAR:

// Pixel-order data
    stat = WarpBilinear( src_8u_C1 , srcRect, dst_8u_C1 , dstRect, coeffs_2x4, IPPI_INTER_NN );      io++;
    stat = WarpBilinear( src_8u_C3 , srcRect, dst_8u_C3 , dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinear( src_8u_CA , srcRect, dst_8u_CA , dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;
    stat = WarpBilinear( src_8u_C4 , srcRect, dst_8u_C4 , dstRect, coeffs_2x4, 3 );                  io++;

    stat = WarpBilinear( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_2x4, IPPI_INTER_NN );      io++;
    stat = WarpBilinear( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinear( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;
    stat = WarpBilinear( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_2x4, 3 );                  io++;

// Planar-order data (P)
    stat = WarpBilinear( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinear( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;

    stat = WarpBilinear( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinear( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;


// ---- WARP_BILINEAR_BACK:

// Pixel-order data
    stat = WarpBilinearBack( src_8u_C1 , srcRect, dst_8u_C1, dstRect, coeffs_2x4, IPPI_INTER_NN );       io++;
    stat = WarpBilinearBack( src_8u_C3 , srcRect, dst_8u_C3, dstRect, coeffs_2x4, IPPI_INTER_LINEAR );   io++;
    stat = WarpBilinearBack( src_8u_CA , srcRect, dst_8u_CA, dstRect, coeffs_2x4, IPPI_INTER_CUBIC );    io++;
    stat = WarpBilinearBack( src_8u_C4 , srcRect, dst_8u_C4, dstRect, coeffs_2x4, 3 );                   io++;

    stat = WarpBilinearBack( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_2x4, IPPI_INTER_NN );      io++;
    stat = WarpBilinearBack( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearBack( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;
    stat = WarpBilinearBack( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_2x4, 3 );                  io++;

// Planar-order data (P)
    stat = WarpBilinearBack( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearBack( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;

    stat = WarpBilinearBack( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_2x4, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearBack( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_2x4, IPPI_INTER_CUBIC );   io++;


// ---- WARP_BILINEAR_QUAD:

// Pixel-order data
    stat = WarpBilinearQuad( src_8u_C1 , srcRect, srcQuad, dst_8u_C1, dstRect, dstQuad, IPPI_INTER_NN );       io++;
    stat = WarpBilinearQuad( src_8u_C3 , srcRect, srcQuad, dst_8u_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );   io++;
    stat = WarpBilinearQuad( src_8u_CA , srcRect, srcQuad, dst_8u_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );    io++;
    stat = WarpBilinearQuad( src_8u_C4 , srcRect, srcQuad, dst_8u_C4, dstRect, dstQuad, 3 );                   io++;

    stat = WarpBilinearQuad( src_32f_C1, srcRect, srcQuad, dst_32f_C1, dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpBilinearQuad( src_32f_C3, srcRect, srcQuad, dst_32f_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearQuad( src_32f_CA, srcRect, srcQuad, dst_32f_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;
    stat = WarpBilinearQuad( src_32f_C4, srcRect, srcQuad, dst_32f_C4, dstRect, dstQuad, 3 );                  io++;

// Planar-order data (P)
    stat = WarpBilinearQuad( src_8u_P3 , srcRect, srcQuad, dst_8u_P3 , dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearQuad( src_8u_P4 , srcRect, srcQuad, dst_8u_P4 , dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;

    stat = WarpBilinearQuad( src_32f_P3, srcRect, srcQuad, dst_32f_P3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpBilinearQuad( src_32f_P4, srcRect, srcQuad, dst_32f_P4, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;


// ---- WARP_PERSPECTIVE:

// Pixel-order data
    stat = WarpPerspective( src_8u_C1, srcRect, dst_8u_C1, dstRect, coeffs_3x3, IPPI_INTER_NN );        io++;
    stat = WarpPerspective( src_8u_C3, srcRect, dst_8u_C3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );    io++;
    stat = WarpPerspective( src_8u_CA, srcRect, dst_8u_CA, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );     io++;
    stat = WarpPerspective( src_8u_C4, srcRect, dst_8u_C4, dstRect, coeffs_3x3, 3 );                    io++;

    stat = WarpPerspective( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_3x3, IPPI_INTER_NN );      io++;
    stat = WarpPerspective( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspective( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpPerspective( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_3x3, 3 );                  io++;

// Planar-order data (P)
    stat = WarpPerspective( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspective( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;

    stat = WarpPerspective( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspective( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;


// ---- WARP_PERSPECTIVE_BACK:

// Pixel-order data
    stat = WarpPerspectiveBack( src_8u_C1, srcRect, dst_8u_C1, dstRect, coeffs_3x3, IPPI_INTER_NN );        io++;
    stat = WarpPerspectiveBack( src_8u_C3, srcRect, dst_8u_C3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );    io++;
    stat = WarpPerspectiveBack( src_8u_CA, srcRect, dst_8u_CA, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );     io++;
    stat = WarpPerspectiveBack( src_8u_C4, srcRect, dst_8u_C4, dstRect, coeffs_3x3, 3 );                    io++;

    stat = WarpPerspectiveBack( src_32f_C1, srcRect, dst_32f_C1, dstRect, coeffs_3x3, IPPI_INTER_NN );      io++;
    stat = WarpPerspectiveBack( src_32f_C3, srcRect, dst_32f_C3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveBack( src_32f_CA, srcRect, dst_32f_CA, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;
    stat = WarpPerspectiveBack( src_32f_C4, srcRect, dst_32f_C4, dstRect, coeffs_3x3, 3 );                  io++;

// Planar-order data (P)
    stat = WarpPerspectiveBack( src_8u_P3 , srcRect, dst_8u_P3 , dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveBack( src_8u_P4 , srcRect, dst_8u_P4 , dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;

    stat = WarpPerspectiveBack( src_32f_P3, srcRect, dst_32f_P3, dstRect, coeffs_3x3, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveBack( src_32f_P4, srcRect, dst_32f_P4, dstRect, coeffs_3x3, IPPI_INTER_CUBIC );   io++;


// ---- WARP_PERSPECTIVE_QUAD:

// Pixel-order data
    stat = WarpPerspectiveQuad( src_8u_C1, srcRect, srcQuad, dst_8u_C1, dstRect, dstQuad, IPPI_INTER_NN );        io++;
    stat = WarpPerspectiveQuad( src_8u_C3, srcRect, srcQuad, dst_8u_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );    io++;
    stat = WarpPerspectiveQuad( src_8u_CA, srcRect, srcQuad, dst_8u_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );     io++;
    stat = WarpPerspectiveQuad( src_8u_C4, srcRect, srcQuad, dst_8u_C4, dstRect, dstQuad, 3 );                    io++;

    stat = WarpPerspectiveQuad( src_32f_C1, srcRect, srcQuad, dst_32f_C1, dstRect, dstQuad, IPPI_INTER_NN );      io++;
    stat = WarpPerspectiveQuad( src_32f_C3, srcRect, srcQuad, dst_32f_C3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveQuad( src_32f_CA, srcRect, srcQuad, dst_32f_CA, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;
    stat = WarpPerspectiveQuad( src_32f_C4, srcRect, srcQuad, dst_32f_C4, dstRect, dstQuad, 3 );                  io++;

// Planar-order data (P)
    stat = WarpPerspectiveQuad( src_8u_P3 , srcRect, srcQuad, dst_8u_P3 , dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveQuad( src_8u_P4 , srcRect, srcQuad, dst_8u_P4 , dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;

    stat = WarpPerspectiveQuad( src_32f_P3, srcRect, srcQuad, dst_32f_P3, dstRect, dstQuad, IPPI_INTER_LINEAR );  io++;
    stat = WarpPerspectiveQuad( src_32f_P4, srcRect, srcQuad, dst_32f_P4, dstRect, dstQuad, IPPI_INTER_CUBIC );   io++;



/* #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D
#D                                                                                                                   #D
#D                                         < Wavelet Transforms (#13) >                                              #D
#D                                                                                                                   #D
#D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D #D */

    int lenLow = w >> 3;
    int lenHigh = w >> 3;
    int anchorLow  = lenLow  >> 1;
    int anchorHigh = lenHigh >> 1;
    int border = IPP_MAX(anchorLow + 1 ,anchorHigh + 1);
    IppiSize fullSize = {M - border*2, N - border*2};
    IppiSize detailSize = {fullSize.width >> 1, fullSize.height >> 1};
    fullSize.width = detailSize.width << 1;
    fullSize.height = detailSize.height << 1;
    Ipp32f *pTabsLow  = new Ipp32f[lenLow];
    Ipp32f *pTabsHigh = new Ipp32f[lenHigh];
    IppiSize setSizeLow = {lenLow, 1};
    IppiSize setSizeHigh = {lenHigh, 1};
    ippiSet_C1R(1, pTabsLow, lenLow<<2, setSizeLow);
    ippiSet_C1R(1, pTabsHigh, lenHigh<<2, setSizeHigh);
    IppiWTFwdSpec_32f_C1R  *pWTFwdSpec_C1R;
    IppiWTFwdSpec_32f_C3R  *pWTFwdSpec_C3R;
    IppiWTInvSpec_32f_C1R  *pWTInvSpec_C1R;
    IppiWTInvSpec_32f_C3R  *pWTInvSpec_C3R;
    
    src_32f_C1.setRect(border, border, fullSize.width, fullSize.height );
    src_32f_C3.setRect(border, border, fullSize.width, fullSize.height );
    dst_32f_C1.setRect(border, border, detailSize.width, detailSize.height );
    dst_32f_C3.setRect(border, border, detailSize.width, detailSize.height );
    aux1_32f_C1.setRect(border, border, detailSize.width, detailSize.height );
    aux1_32f_C3.setRect(border, border, detailSize.width, detailSize.height );
    aux2_32f_C1.setRect(border, border, detailSize.width, detailSize.height );
    aux2_32f_C3.setRect(border, border, detailSize.width, detailSize.height );
    aux_32f_C1.setRect(border, border, detailSize.width, detailSize.height );
    aux_32f_C3.setRect(border, border, detailSize.width, detailSize.height );

// ---- WT_[FWD|INV]_INIT_ALLOC:

    stat = WTFwdInitAlloc( &pWTFwdSpec_C1R, pTabsLow, lenLow, anchorLow, pTabsHigh, lenHigh, anchorHigh );  io++;
    stat = WTFwdInitAlloc( &pWTFwdSpec_C3R, pTabsLow, lenLow, anchorLow, pTabsHigh, lenHigh, anchorHigh );  io++;
    stat = WTInvInitAlloc( &pWTInvSpec_C1R, pTabsLow, lenLow, anchorLow, pTabsHigh, lenHigh, anchorHigh );  io++;
    stat = WTInvInitAlloc( &pWTInvSpec_C3R, pTabsLow, lenLow, anchorLow, pTabsHigh, lenHigh, anchorHigh );  io++;


// ---- WT_[FWD|INV]_INIT_ALLOC/WT_[FWD|INV]_FREE/WT_[FWD|INV]_GET_BUFSIZE:

    stat = WTFwdGetBufSize( pWTFwdSpec_C1R, pSize );  io++;
    stat = WTFwdGetBufSize( pWTFwdSpec_C3R, pSize+1 );  io++;
    stat = WTInvGetBufSize( pWTInvSpec_C1R, pSize+2 );  io++;
    stat = WTInvGetBufSize( pWTInvSpec_C3R, pSize+3 );  io++;

// ---- WT_[FWD|INV]:

    stat = WTFwd( src_32f_C1, dst_32f_C1, aux1_32f_C1, aux2_32f_C1, aux_32f_C1, pWTFwdSpec_C1R, 0 );  io++;
    pBuffer = (int*)new Ipp8u[pSize[0]];
    dst_32f_C1.roiSize.resize(-M/2, -N/2);
    stat = WTFwd( src_32f_C1, dst_32f_C1, aux1_32f_C1, aux2_32f_C1, aux_32f_C1, pWTFwdSpec_C1R, (Ipp8u*)pBuffer );  io++;
    delete[] pBuffer;

    stat = WTFwd( src_32f_C3, dst_32f_C3, aux1_32f_C3, aux2_32f_C3, aux_32f_C3, pWTFwdSpec_C3R, 0 );  io++;
    pBuffer = (int*)new Ipp8u[pSize[1]];
    dst_32f_C3.roiSize.resize(-M/2, -N/2);
    stat = WTFwd( src_32f_C3, dst_32f_C3, aux1_32f_C3, aux2_32f_C3, aux_32f_C3, pWTFwdSpec_C3R, (Ipp8u*)pBuffer);  io++;
    delete[] pBuffer;

    stat = WTInv( dst_32f_C1, aux1_32f_C1, aux2_32f_C1, aux_32f_C1, src_32f_C1, pWTInvSpec_C1R, 0 );  io++;
    pBuffer = (int*)new Ipp8u[pSize[2]];
    src_32f_C1.roiSize.resize(-M/2, -N/2);
    stat = WTInv( dst_32f_C1, aux1_32f_C1, aux2_32f_C1, aux_32f_C1, src_32f_C1, pWTInvSpec_C1R, (Ipp8u*)pBuffer);  io++;
    delete[] pBuffer;

    stat = WTInv( dst_32f_C3, aux1_32f_C3, aux2_32f_C3, aux_32f_C3, src_32f_C3, pWTInvSpec_C3R, 0 );  io++;
    pBuffer = (int*)new Ipp8u[pSize[3]];
    stat = WTInv( dst_32f_C3, aux1_32f_C3, aux2_32f_C3, aux_32f_C3, src_32f_C3, pWTInvSpec_C3R, (Ipp8u*)pBuffer);  io++;
    delete[] pBuffer;

// ---- WT_[FWD|INV]_FREE:

    WTFwdFree( pWTFwdSpec_C1R );  io++;
    WTFwdFree( pWTFwdSpec_C3R );  io++;
    WTInvFree( pWTInvSpec_C1R );  io++;
    WTInvFree( pWTInvSpec_C3R );  io++;

    src_32f_C1 .restoreRect();
    src_32f_C3 .restoreRect();
    dst_32f_C1 .restoreRect();
    dst_32f_C3 .restoreRect();
    aux1_32f_C1.restoreRect();
    aux1_32f_C3.restoreRect();
    aux2_32f_C1.restoreRect();
    aux2_32f_C3.restoreRect();
    aux_32f_C1 .restoreRect();
    aux_32f_C3 .restoreRect();

// ................................................................

    delete[] pSize;

    delete[] pTabsLow;
    delete[] pTabsHigh;

    delete[] pLevels[0];
    delete[] pLevels[1];
    delete[] pLevels[2];
    delete[] pLevels[3];
    delete[] pValues[0];
    delete[] pValues[1];
    delete[] pValues[2];
    delete[] pValues[3];

    delete[] pTable;

    delete[] pLevels_32f[0];
    delete[] pLevels_32f[1];
    delete[] pLevels_32f[2];
    delete[] pLevels_32f[3];
    delete[] pHist[0];
    delete[] pHist[1];
    delete[] pHist[2];
    delete[] pHist[3];


// ____________________________________________________________________________________________________________
//
    printf( "All %i main demo-calls for template wrappers finished successfully.\n\n", io );

    return 0;
}
