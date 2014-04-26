/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
//                 Intel(R) Integrated Performance Primitives
//                    C++ template based wrappers for IPPi
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  SYNOPSIS
//      Contains template wrappers interface for IPPi.
//      Template wrappers are created for those IPPi which prototipes are placed in ippi.h.
//
//  DESCRIPTION
//      Full list of template function name:
//
//      #4 Image Data Exchange and Initialization Functions 
//     =====================================================
//              AddRandGauss_Direct
//              AddRandUniform_Direct
//              Convert
//              Copy
//              CopyConstBorder
//              CopyReplicateBorder
//              CopyWrapBorder
//              Dup
//              ImageJaehne
//              ImageRamp
//              Scale
//              Set
//              SwapChannels
//              Transpose
//
//      #5 Image Arithmetic and Logical Operations
//     =============================================
//          Arithmetic Operations
//         -----------------------
//              Abs
//              Add
//              Complement
//              Div
//              Exp
//              Ln
//              Mul
//              MulScale
//              MulCScale
//              Sqr
//              Sqrt
//              Sub
//
//          Logical  Operations
//         ---------------------
//              And
//              LShift
//              Not
//              Or
//              RShift
//              Xor
//
//          Alpha Composition
//         -------------------
//              AlphaComp
//              AlphaPremul
//
//      #6 Image Color Conversion
//     ===========================
//          Lookup Table Conversion
//         -------------------------
//              LUT
//              LUT_Linear
//              LUT_Cubic
//              LUTPalette
//
//      #7 Threshold and Compare Operations
//     =====================================
//          Thresholding
//         --------------
//              Threshold
//              Threshold_GT
//              Threshold_LT
//              Threshold_Val
//              Threshold_GTVal
//              Threshold_LTVal
//              Threshold_LTValGTVal
//
//          Compare Operations
//         --------------------
//              Compare
//              CompareEqualEps
//
//      #8 Morphological Operations
//     =============================
//              Dilate3x3
//              Erode3x3
//              Dilate3x3
//              Erode3x3
//              Dilate
//              Erode
//              Dilate
//              Erode
//
//      #9 Filtering Functions
//     ========================
//              FilterBox
//              FilterMax
//              FilterMin
//
//          Fixed Filtering Functions (with 3x3 mask size)
//         ------------------------------------------------
//              FilterPrewittHoriz
//              FilterPrewittVert
//              FilterRobertsDown
//              FilterRobertsUp
//              FilterScharrHoriz
//              FilterScharrVert
//              FilterSharpen
//              FilterSobelHoriz
//              FilterSobelVert
//
//          Fixed Filtering Functions (with 3x3 and 5x5 mask sizes)
//         ---------------------------------------------------------
//              FilterGauss
//              FilterHipass
//              FilterLaplace
//              FilterLowpass
//              FilterSobelCross
//              FilterSobelHoriz
//              FilterSobelHorizMask
//              FilterSobelHorizSecond
//              FilterSobelVert
//              FilterSobelVertMask
//              FilterSobelVertSecond
//
//          2D Convolution
//         ----------------
//              ConvFull
//              ConvValid
//
//          General Linear Filters
//         ------------------------
//              Filter
//              Filter32f
//
//          Median Filters
//         ----------------
//              FilterMedian
//              FilterMedianColor
//              FilterMedianCross
//              FilterMedianColor
//              FilterMedianCross
//              FilterMedianHoriz
//              FilterMedianVert
//
//          Separable Filters
//         -------------------
//              FilterColumn
//              FilterColumn32f
//
//          Wiener Filters
//         ----------------
//              FilterWiener
//              FilterWiener
//              FilterWiener
//
//         -------------------------
//              DeconvFFTInitAlloc
//              DeconvFFT
//              DeconvFFTFree
//
//              DeconvLRInitAlloc
//              DeconvLR
//              DeconvLRFree
//
//      #10 Image Linear Transforms
//     =============================
//          Discrete Cosine Transforms
//         ----------------------------
//              DCTFwdInitAlloc
//              DCTInvInitAlloc
//              DCTFwdGetBufSize
//              DCTInvGetBufSize
//              DCTFwd
//              DCTInv
//              DCTFwdFree
//              DCTInvFree
//
//          Fourier Transforms
//         --------------------
//              DFTInitAlloc
//              DFTGetBufSize
//              DFTFwd_RToPack
//              DFTInv_PackToR
//              DFTFwd_CToC
//              DFTInv_CToC
//              DFTFree
//
//              FFTInitAlloc
//              FFTGetBufSize
//              FFTFwd_RToPack
//              FFTInv_PackToR
//              FFTFwd_CToC
//              FFTInv_CToC
//              FFTFree
//
//              Magnitude
//              MagnitudePack
//              MulPack
//              MulPackConj
//              PackToCplxExtend
//              Phase
//              PhasePack
//
//          Windowing Functions
//         ---------------------
//              WinBartlett
//              WinBartlettSep
//              WinHamming
//              WinHammingSep
//
//      #11 Image Statistics Functions
//     ================================
//              CountInRange
//              HistogramRange
//              HistogramEven
//              Max
//              MaxIndx
//              Mean
//              Min
//              MinIndx
//              Moments64f
//              Moments64s
//              QualityIndex
//              Sum
//
//          Image Norms
//         -------------
//              Norm_Inf
//              Norm_L1
//              Norm_L2
//
//              NormDiff_Inf
//              NormDiff_L1
//              NormDiff_L2
//
//              NormRel_Inf
//              NormRel_L1
//              NormRel_L2
//
//          Image Proximity Measures
//         --------------------------
//              CrossCorrFull_Norm
//              CrossCorrFull_NormLevel
//              SqrDistanceFull_Norm
//
//              CrossCorrSame_Norm
//              CrossCorrSame_NormLevel
//              SqrDistanceSame_Norm
//
//              CrossCorrValid_Norm
//              CrossCorrValid_NormLevel
//              SqrDistanceValid_Norm
//
//      #12 Image Statistics Functions
//     ================================
//              GetResizeFract
//              Mirror
//              Remap
//              Resize
//              ResizeCenter
//              ResizeShift
//              Rotate
//              RotateCenter
//              Shear
//              WarpAffine
//              WarpAffineBack
//              WarpAffineQuad
//              WarpBilinear
//              WarpBilinearBack
//              WarpBilinearQuad
//              WarpPerspective
//              WarpPerspectiveBack
//              WarpPerspectiveQuad
//
//      #13 Image Statistics Functions
//     ================================
//              WTFwdInitAlloc
//              WTInvInitAlloc
//              WTFwdGetBufSize
//              WTInvGetBufSize
//              WTFwd
//              WTInv
//              WTFwdFree
//              WTInvFree
//
//  UPDATES for IPP 5.1 Samples
//     Several template wrappers were added regarding last IPPi updating:
//     1. New Dup
//     2. New DeconvFFT[InitAlloc|Free]
//     3. New DeconvLR[InitAlloc|Free]
//     4. New not-in-place operations for MulPackConj (float data only)
//     6. New in-place [DFT|FFT][Fwd_RToPack|Inv_PackToR] (float data only)
//     7. in-place [DFT|FFT][Fwd|Inv]_CToC
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#if !defined( __WIPPI_HPP__ )
#define   __WIPPI_HPP__

#include "ippi.hpp"
#include "ippcc.h"

/* ------------------------------------------------------------------------------------------------------------------- */

using namespace IPPIP;

struct IPSize : public IppiSize {
    IPSize(int w,int h) {width=w;height=h;}
    IPSize(const IppiSize& sz) {width=sz.width;height=sz.height;}

    IPSize& resize(int addw,int addh) {width+=addw;height+=addh; return *this;}
    IPSize& set(int w,int h) {width=w;height=h; return *this;}
    IPSize& set(IppiSize sz) {width=sz.width;height=sz.height; return *this;}
};

template<class T,int N> struct IMG {
private:
    T *img_mem; // ptr to allocated memory
protected:
    T *img;     // holds current ROI start position (left-top point)
public:
    int wstep;               // image pitch, set in constructor
    IPSize imgSize;          // memory image size, set in constructor
    IPSize roiSize;          // ROI, initially cover the whole image,
    IppStatus lastStatus;    // status of the last operation, where the image is a destination

    IMG(int width, int height) :
        imgSize(width,height),
        roiSize(width,height),
        lastStatus(ippStsNoOperation)
    {
        wstep = imgSize.width * N * sizeof(T);
        img = img_mem = (T*)ippiMalloc_8u_C1(wstep,imgSize.height,&wstep); // malloc in bytes
    }

    // moves left-top point of the image, modifies roiSize correspondently
    virtual void border(int wpixels,int hpixels)
    {
        img = (T*)((Ipp8u*)img+wstep*hpixels+wpixels*(N*sizeof(T)));
        roiSize.resize(-wpixels,-hpixels);
    }
    // modifies current roi rectangle - shifts left, top, right, bottom borders
    void roiRect(int addl, int addt, int addr, int addb)
    {
        border(addl, addt);
        roiSize.resize(addr, addb);
    }
    // modifies current roi rectangle - shifts left, top, right, bottom borders
    void setRect(int x, int y, int w, int h)
    {
        border(x, y);
        roiSize.set(w, h);
    }
    //restores ROI to the whole image
    virtual void restoreRect()
    {
        img = img_mem;
        roiSize = imgSize;
    }

    virtual ~IMG() { ippiFree(img_mem); }

    // returns current ROI start position (left-top point)
    inline T *ptr() const { return img; }
};


// plane image
template<class T,int N> struct IMGP : IMG<T,1> {
protected:
    T *plane[N]; // pointers to planes. Planes are always treated as consecutive parts of xN buffer
public:
    // use C1 as base to malloc but with height*N
    IMGP(int width, int height) : IMG<T,1>(width, height*N) {
        imgSize.height = roiSize.height = height; // reset from height*N
        reset_planes();
    }

    // set planes pointers based on current T* img
    // should be called after each T* img modification
    void reset_planes() {for(int i=0; i<N; i++) plane[i] = (T*)((Ipp8u*)img + i * roiSize.height * wstep);}

    virtual void border(int wpixels,int hpixels)
    {
        IMG<T,1>::border(wpixels,hpixels);
        reset_planes();
    }

    virtual void restoreRect()
    {
        IMG<T,1>::restoreRect();
        reset_planes();
    }

    inline T* const* ptr() const { return plane; }
};

/* ------------------------------------------------------------------------------------------------------------------- */

/// now have 7 derived image classes
/// it allows to support AC4, different arguments in ippi calls for
/// functions for float and integer data, value as a constant for 1 channel images in contradiction
/// to pointer in multichannel images.

#define IMG_DERIVE(From, Name, Nch) \
    template<class T> struct Name : From<T,Nch> { \
        Name(int w, int h):From<T,Nch>(w,h) {} };

// all channel type images
IMG_DERIVE(IMG, GIMG, 1)
IMG_DERIVE(IMG, IMG3, 3)
IMG_DERIVE(IMG, IMG4, 4)
IMG_DERIVE(IMG, AIMG, 4)

IMG_DERIVE(IMGP, IMG3P, 3)
IMG_DERIVE(IMGP, IMG4P, 4)
IMG_DERIVE(IMGP, AIMGP, 4)

/*
 * Name designing rules for macros with template:
 *
 * DECL_['CHAR_SET'|'FUN_NAME']_TP
 *
 * 1. 'CHAR_SET' is formed according to the IPPi parameters (and template ones if both differ).
 *    For every parameter(as a rule) there is a certain character in the name. Correspondence is the following:
 *
 *        S/rc image/      s.ptr()
 *        D/st image/      dd.ptr()
 *        T                for a parameter which data type passes through typename
 *        z                dd.roiSize | s.roiSize
 *        Z                s.imgSize
 *        r/ect/           srcRect | dstRect
 *        i                int data type of parameter
 *        d                double data type of parameter
 *        M/ask/           IppiSize maskSize, IppiPoint anchor | IppiSize kernelSize, IppiPoint anchor
 *        B/uffer/         mx.ptr(), mx.wstep | my.ptr(), my.wstep | etc
 *        Anm              2d array, where n,m = dimension size; for example A24 or A33
 *        x                otherwise
 *
 *    Addition:
 *        1.1  dd.wstep | s.wstep | s1.wstep | s2.wstep is not reflected in the name
 *        1.2  neighbour T(for example 2):
 *               - T2 for the same typename
 *               - TT otherwise
 *        1.3  if src and dst has different data type then S1 and D2
 *        1.4  neighbour i,d,x (>2) is wrapped to i3, d3, x3 (for sequence of 3)
 *        1.5  S6/D6 - special case for s.ptr()+si/dd.ptr()+di
 *        1.6  D32f/D8u - dst has individual data type Ipp32f/Ipp8u accordingly
 *
 * 2. 'FUN_NAME' - short IPPi name (for individual macros with template)
 */

/* ----------------------------------------  The most commonly used templates  --------------------------------------- */

#define DECL_SDz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize);}

#define DECL_SDzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, int scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize, scale);}

#define DECL_SDzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize, scale);}

#define DECL_StDz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s,  C pval, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, pval, dd.ptr(), dd.wstep, dd.roiSize);}

#define DECL_Dz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( IM_TP<T>&dd ){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize);}

#define DECL_DzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(IM_TP<T>&dd, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, scale);}

#define DECL_TDz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun( C pval, IM_TP<T>&dd ){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(pval, dd.ptr(), dd.wstep, dd.roiSize);}

#define DECL_SSDzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd, int scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, scale);}

#define DECL_SSDz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize);}

// different data with same channels
#define DECL_S1D2z_TP(Afun,IM_TP,F_SUFF) \
    template<typename T1,typename T2> \
    inline IppStatus Afun(const IM_TP<T1>&s, IM_TP<T2>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R((const T1*)s.ptr(), s.wstep, (T2*)dd.ptr(), dd.wstep, dd.roiSize);}

#define DECL_S1D2zT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T1,typename T2,typename P> \
    inline IppStatus Afun(const IM_TP<T1>&s, IM_TP<T2>&dd, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R((const T1*)s.ptr(), s.wstep, \
                                                        (T2*)dd.ptr(), dd.wstep, dd.roiSize, scale);}
// Is not used anywhere (commented):
//#define DECL_SCDRt_TP(Afun,IM_TP,F_SUFF) \
//    template<typename T,typename C,typename P> \
//    inline IppStatus Afun(const IM_TP<T>&s, C pval, IM_TP<T>&dd, P par){ \
//        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, pval, dd.ptr(), dd.wstep, dd.roiSize, par);}


/* ---------------------------------------  The most commonly used definitions  --------------------------------------- */

#define DECL_FORALLCH(Afun,DECL) \
    DECL(Afun,AIMG,AC4) \
    DECL(Afun,IMG3,C3) \
    DECL(Afun,IMG4,C4) \
    DECL(Afun,GIMG,C1)

#define DECL_FOR13CH(Afun,DECL) \
    DECL(Afun,IMG3,C3) \
    DECL(Afun,GIMG,C1)

#define DECL_FOR13ACH(Afun,DECL) \
    DECL(Afun,AIMG,AC4) \
    DECL(Afun,IMG3,C3) \
    DECL(Afun,GIMG,C1)

//binary
#define DECL_AFUN(Afun,IM_TP,F_SUFF) \
    DECL_SDzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_SSDzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_SDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_SSDz_TP(Afun,IM_TP,F_SUFF)

#define DECL_FORALLCH_M_1(Afun,DECLM,DECL1) \
    DECLM(Afun,AIMG,AC4) \
    DECLM(Afun,IMG3,C3) \
    DECLM(Afun,IMG4,C4) \
    DECL1(Afun,GIMG,C1)

/* ----------------------------------------  Function declarations themselves  --------------------------------------- */

// Rule: one template for one set of parameter (at least inside function group)

//- Arithmetic Operations (from #5)

// Add, Sub, Mul, Div: all types/channels/incl. complex

// declaration for all channel types
#define DECL_AFUN_ALLCH(Afun) \
    DECL_FORALLCH(Afun, DECL_AFUN) \
    DECL_FORALLCH(Afun, DECL_ACFUN)

#define DECL_TDzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun( C pval, IM_TP<T>&dd, int scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(pval, dd.ptr(), dd.wstep, dd.roiSize, scale);}

#define DECL_StDzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, C pval, IM_TP<T>&dd, int scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, pval, dd.ptr(), dd.wstep, dd.roiSize, scale);}

// with const values for C1
#define DECL_ACFUN(Afun,IM_TP,F_SUFF) \
    DECL_TDzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_StDzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_TDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_StDz_TP(Afun,IM_TP,F_SUFF)

DECL_AFUN_ALLCH(Add)
DECL_AFUN_ALLCH(Sub)
DECL_AFUN_ALLCH(Mul)
DECL_AFUN_ALLCH(Div)

// Abs
DECL_FORALLCH(Abs, DECL_SDz_TP)
DECL_FORALLCH(Abs, DECL_Dz_TP)

// Sqr, Sqrt, Ln, Exp
#define DECL_Dzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(IM_TP<T>&dd, int scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, scale);}

// unary
#define DECL_UFUN(Afun,IM_TP,F_SUFF) \
    DECL_Dzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_SDzi_TP(Afun,IM_TP,F_SUFF) \
    DECL_Dz_TP(Afun,IM_TP,F_SUFF) \
    DECL_SDz_TP(Afun,IM_TP,F_SUFF)


DECL_FORALLCH(Sqr, DECL_UFUN)
DECL_FORALLCH(Sqrt, DECL_UFUN)
DECL_FOR13CH(Ln, DECL_UFUN)
DECL_FOR13CH(Exp, DECL_UFUN)

// MulScale, MulCScale
DECL_FORALLCH(MulScale, DECL_SSDz_TP)
DECL_FORALLCH(MulScale, DECL_SDz_TP)

DECL_FORALLCH(MulCScale,DECL_StDz_TP)
DECL_FORALLCH(MulCScale,DECL_TDz_TP)

// Complement (32s_C1 only)
DECL_Dz_TP(Complement,GIMG,C1)


//- Fourier Transforms (from #10)

// MulPack (16s, 32s, 32f)
DECL_FORALLCH(MulPack, DECL_AFUN)

// MulPackConj (only 32f)
DECL_FORALLCH(MulPackConj, DECL_SDz_TP)
DECL_FORALLCH(MulPackConj, DECL_SSDz_TP)

// PackToCplxExtend (C1, 32s, 32f)
template<typename T1, typename T2> \
inline IppStatus PackToCplxExtend(const GIMG<T1>&s1, GIMG<T2>&dd){
    return dd.lastStatus = ippiPackToCplxExtend_C1R((const T1*)s1.ptr(), s1.roiSize, s1.wstep, (T2*)dd.ptr(), dd.wstep);}

// PhasePack, MagnitudePack (C1, C3, s16, s32, f32)
#define DECL_CPMPACK(Afun) \
    DECL_SDzi_TP(Afun,GIMG,C1) \
    DECL_SDz_TP(Afun,GIMG,C1) \
    DECL_SDzi_TP(Afun,IMG3,C3) \
    DECL_SDz_TP(Afun,IMG3,C3)

DECL_CPMPACK(PhasePack)
DECL_CPMPACK(MagnitudePack)

// Phase, Magnitude
#define DECL_CPM(Afun) \
    DECL_S1D2zT_TP(Afun,GIMG,C1) \
    DECL_S1D2z_TP(Afun,GIMG,C1) \
    DECL_S1D2zT_TP(Afun,IMG3,C3) \
    DECL_S1D2z_TP(Afun,IMG3,C3)

DECL_CPM(Phase)
DECL_CPM(Magnitude)


//- Alpha Composition (from #5)

// AlphaPremul (8u, 16u, unsupported here AP4 see below in appropriate FOR PLANES part)
DECL_SDz_TP(AlphaPremul,AIMG,AC4)
DECL_Dz_TP(AlphaPremul,AIMG,AC4)
DECL_FORALLCH(AlphaPremul,DECL_StDz_TP)
DECL_FORALLCH(AlphaPremul,DECL_TDz_TP)

// AlphaComp (8u, 16u, unsupported here AP4 see below in appropriate FOR PLANES part)

//Comments for DECL_SSDzT_TP: P=IppiAlphaType only, and 2 following lines may be used instead of existing ones:
/*    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd, IppiAlphaType alphaType){ \ */
#define DECL_SSDzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd, P par){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, par);}

#define DECL_StStDzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s1, C val1, const IM_TP<T>&s2, C val2, IM_TP<T>&dd, P par){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, val1, s2.ptr(), s2.wstep, val2, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, par);}
DECL_SSDzT_TP(AlphaComp,AIMG,AC4)
DECL_SSDzT_TP(AlphaComp,GIMG,AC1)
DECL_FORALLCH(AlphaComp,DECL_StStDzT_TP)


// FFT/DFT/DCTFwd/DCTInv
// InitAlloc, Free, GetBufsize

template<typename T> \
inline IppStatus FFTInitAlloc( T** pFTSpec, int orderX, int orderY, int flag, IppHintAlgorithm hint) { \
    return ippiFFTInitAlloc( pFTSpec, orderX, orderY, flag, hint ); }

template<typename T> \
inline IppStatus DFTInitAlloc( T** pFTSpec, IppiSize roiSize, int flag, IppHintAlgorithm hint) { \
    return ippiDFTInitAlloc( pFTSpec, roiSize, flag, hint ); }

#define DECL_DCTX_INIT_TP(DCTX) \
    static inline IppStatus DCTX##InitAlloc( Ippi##DCTX##Spec_32f** pFTSpec, IppiSize roiSize, IppHintAlgorithm hint) { \
        return ippi##DCTX##InitAlloc( pFTSpec, roiSize, hint ); }

DECL_DCTX_INIT_TP(DCTFwd)
DECL_DCTX_INIT_TP(DCTInv)

#define DECL_XFT_SUPP_TP(XFT) \
    template<typename T> \
    inline IppStatus XFT##Free( T* pFTSpec) { \
        return ippi##XFT##Free( pFTSpec ); } \
    template<typename T> \
    inline IppStatus XFT##GetBufSize( const T* pFTSpec, int* pSize) { \
        return ippi##XFT##GetBufSize( pFTSpec, pSize ); }

DECL_XFT_SUPP_TP(FFT)
DECL_XFT_SUPP_TP(DFT)
DECL_XFT_SUPP_TP(DCTFwd)
DECL_XFT_SUPP_TP(DCTInv)

// cmplx float C1
#define DECL_XFTC_TP(XFT,FI) \
    static inline IppStatus XFT##FI##_CToC(const GIMG<Ipp32fc>&s, GIMG<Ipp32fc>&dd, \
                                           const Ippi##XFT##Spec_C_32fc* pFTSpec, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##XFT##FI##_CToC_C1R((const Ipp32fc*)s.ptr(), s.wstep, \
                                                        (Ipp32fc*)dd.ptr(), dd.wstep, pFTSpec, pBuffer); }
// in-place cmplx float C1
#define DECL_XFTCI_TP(XFT,FI) \
    static inline IppStatus XFT##FI##_CToC(GIMG<Ipp32fc>&dd, \
                                           const Ippi##XFT##Spec_C_32fc* pFTSpec, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##XFT##FI##_CToC_C1R((Ipp32fc*)dd.ptr(), dd.wstep, pFTSpec, pBuffer); }


// packed float (FI+TO: Fwd+RToPack or Inv+PackToR)
#define DECL_XFTF_TP(XFT,FI,TO,IM_TP,F_SUFF) \
    static inline IppStatus XFT##FI##_##TO(const IM_TP<Ipp32f>&s, IM_TP<Ipp32f>&dd, \
                                           const Ippi##XFT##Spec_R_32f* pFTSpec, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##XFT##FI##_##TO##_##F_SUFF##R((const Ipp32f*)s.ptr(), s.wstep, \
                                                                  (Ipp32f*)dd.ptr(), dd.wstep, pFTSpec, pBuffer); }
// in-place packed float (FI+TO: Fwd+RToPack or Inv+PackToR)
#define DECL_XFTFI_TP(XFT,FI,TO,IM_TP,F_SUFF) \
    static inline IppStatus XFT##FI##_##TO(IM_TP<Ipp32f>&dd, \
                                           const Ippi##XFT##Spec_R_32f* pFTSpec, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##XFT##FI##_##TO##_##F_SUFF##R((Ipp32f*)dd.ptr(), dd.wstep, pFTSpec, pBuffer); }

// 32s<->8u (FI+TO: Fwd+RToPack or Inv+PackToR)
#define DECL_XFTI_TP(XFT,FI,TO,IM_TP,F_SUFF) \
    template<typename T1, typename T2> \
    inline IppStatus XFT##FI##_##TO(const IM_TP<T1>&s, IM_TP<T2>&dd, \
                                    const Ippi##XFT##Spec_R_32s* pFTSpec, int scaleFactor, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##XFT##FI##_##TO##_##F_SUFF##R((const T1*)s.ptr(), s.wstep, (T2*)dd.ptr(), dd.wstep, \
                                                                  pFTSpec, scaleFactor, pBuffer); }

#define DECL_XFT_SET(XFT,IM_TP,F_SUFF) \
    DECL_XFTF_TP(XFT,Fwd,RToPack,IM_TP,F_SUFF) \
    DECL_XFTF_TP(XFT,Inv,PackToR,IM_TP,F_SUFF) \
    DECL_XFTFI_TP(XFT,Fwd,RToPack,IM_TP,F_SUFF) \
    DECL_XFTFI_TP(XFT,Inv,PackToR,IM_TP,F_SUFF) \
    DECL_XFTI_TP(XFT,Fwd,RToPack,IM_TP,F_SUFF) \
    DECL_XFTI_TP(XFT,Inv,PackToR,IM_TP,F_SUFF)

#define DECL_XFT(XFT) \
    DECL_XFTC_TP(XFT,Fwd) \
    DECL_XFTC_TP(XFT,Inv) \
    DECL_XFTCI_TP(XFT,Fwd) \
    DECL_XFTCI_TP(XFT,Inv) \
    DECL_FORALLCH(XFT,DECL_XFT_SET)

DECL_XFT(FFT)
DECL_XFT(DFT)


// DeconvFFT[InitAlloc|Free]

#define DECL_DECONV_FFT_SUPP_TP(F_SUFF) \
    static inline IppStatus DeconvFFTInitAlloc( IppiDeconvFFTState_32f_##F_SUFF##R** pFTSpec, \
        const Ipp32f* pKernel, int kernelSize, int FFTorder, Ipp32f threshold) { \
            return ippiDeconvFFTInitAlloc_32f_##F_SUFF##R( pFTSpec, pKernel, kernelSize, FFTorder, threshold ); } \
    static inline IppStatus DeconvFFTFree( IppiDeconvFFTState_32f_##F_SUFF##R* pFTSpec) { \
        return ippiDeconvFFTFree_32f_##F_SUFF##R( pFTSpec ); }

DECL_DECONV_FFT_SUPP_TP(C1)
DECL_DECONV_FFT_SUPP_TP(C3)

DECL_SDzT_TP(DeconvFFT,GIMG,C1)
DECL_SDzT_TP(DeconvFFT,IMG3,C3)


// DeconvLR[InitAlloc|Free]

#define DECL_DECONV_LR_SUPP_TP(F_SUFF) \
    static inline IppStatus DeconvLRInitAlloc( IppiDeconvLR_32f_##F_SUFF##R** pFTSpec, \
        const Ipp32f* pKernel, int kernelSize, IppiSize maxroi, Ipp32f threshold) { \
            return ippiDeconvLRInitAlloc_32f_##F_SUFF##R( pFTSpec, pKernel, kernelSize, maxroi, threshold ); } \
    static inline IppStatus DeconvLRFree( IppiDeconvLR_32f_##F_SUFF##R* pFTSpec) { \
        return ippiDeconvLRFree_32f_##F_SUFF##R( pFTSpec ); }

DECL_DECONV_LR_SUPP_TP(C1)
DECL_DECONV_LR_SUPP_TP(C3)

#define DECL_SDziT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, int numiter, P value){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize, numiter, value);}
DECL_SDziT_TP(DeconvLR,GIMG,C1)
DECL_SDziT_TP(DeconvLR,IMG3,C3)


//dct
#define DECL_DCTX_TP(DCTX,IM_TP,F_SUFF) \
    static inline IppStatus DCTX(const IM_TP<Ipp32f>&s, IM_TP<Ipp32f>&dd, \
                                 const Ippi##DCTX##Spec_32f* pFTSpec, Ipp8u* pBuffer) { \
        return dd.lastStatus = ippi##DCTX##_##F_SUFF##R((const Ipp32f*)s.ptr(), s.wstep, (Ipp32f*)dd.ptr(), dd.wstep, \
                                                        pFTSpec, pBuffer); }

DECL_FORALLCH(DCTFwd,DECL_DCTX_TP)
DECL_FORALLCH(DCTInv,DECL_DCTX_TP)

// DCT 8x8 isn't wrapped here, because it doesn't use image, but only 8x8 block, usually as 1D array

// Wavelets
#define DECL_WTX_SUPP_TP(WTX,IM_TP,F_SUFF) \
    static inline IppStatus WTX##InitAlloc(Ippi##WTX##Spec_32f_##F_SUFF##R** pSpec, \
                                           const Ipp32f* pTapsLow,  int lenLow,  int anchorLow, \
                                           const Ipp32f* pTapsHigh, int lenHigh, int anchorHigh) { \
        return ippi##WTX##InitAlloc_##F_SUFF##R( pSpec, pTapsLow, lenLow, anchorLow, pTapsHigh, lenHigh, anchorHigh); } \
    static inline IppStatus WTX##Free( Ippi##WTX##Spec_32f_##F_SUFF##R* pFTSpec) { \
        return ippi##WTX##Free_##F_SUFF##R( pFTSpec ); } \
    static inline IppStatus WTX##GetBufSize( const Ippi##WTX##Spec_32f_##F_SUFF##R* pFTSpec, int* pSize) { \
        return ippi##WTX##GetBufSize_##F_SUFF##R( pFTSpec, pSize ); }

DECL_FOR13CH(WTFwd,DECL_WTX_SUPP_TP)
DECL_FOR13CH(WTInv,DECL_WTX_SUPP_TP)

// WTX unused, to have same params
#define DECL_WTF_TP(WTX,IM_TP,F_SUFF) \
    inline IppStatus WTFwd(const IM_TP<Ipp32f>&s, IM_TP<Ipp32f>&ad, IM_TP<Ipp32f>&xd, IM_TP<Ipp32f>&yd, \
                           IM_TP<Ipp32f>&xyd, const IppiWTFwdSpec_32f_##F_SUFF##R* pSpec, Ipp8u* pBuffer) { \
        return ippiWTFwd_##F_SUFF##R( (const Ipp32f*)s.ptr(), s.wstep, (Ipp32f*)ad.ptr(), ad.wstep, \
                                      (Ipp32f*)xd.ptr(), xd.wstep, (Ipp32f*)yd.ptr(), yd.wstep, \
                                      (Ipp32f*)xyd.ptr(), xyd.wstep, ad.roiSize, pSpec, pBuffer); }

#define DECL_WTI_TP(WTX,IM_TP,F_SUFF) \
    inline IppStatus WTInv(const IM_TP<Ipp32f>&ad, const IM_TP<Ipp32f>&xd, const IM_TP<Ipp32f>&yd, const IM_TP<Ipp32f>&xyd, \
                           IM_TP<Ipp32f>&s, const IppiWTInvSpec_32f_##F_SUFF##R* pSpec, Ipp8u* pBuffer) { \
        return ippiWTInv_##F_SUFF##R( (const Ipp32f*)ad.ptr(), ad.wstep, (const Ipp32f*)xd.ptr(), xd.wstep, \
                                      (const Ipp32f*)yd.ptr(), yd.wstep, (const Ipp32f*)xyd.ptr(), xyd.wstep, ad.roiSize, \
                                      (Ipp32f*)s.ptr(), s.wstep, pSpec, pBuffer); }

DECL_FOR13CH(WTFwd,DECL_WTF_TP)
DECL_FOR13CH(WTInv,DECL_WTI_TP)


// == Image Geometric Transforms (#12)

// Mirror
DECL_FORALLCH(Mirror, DECL_SDzT_TP)
DECL_FORALLCH(Mirror, DECL_DzT_TP)

// Remap (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SzrBBDzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> inline IppStatus Afun(const IM_TP<T>&s, IppiRect rect, const GIMG<Ipp32f>&mx, \
                                                          const GIMG<Ipp32f>&my, IM_TP<T>&dd, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, rect, \
                                                        mx.ptr(), mx.wstep, my.ptr(), my.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, scale); }

DECL_FORALLCH(Remap, DECL_SzrBBDzT_TP)

// Resize (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDzddT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect rect, IM_TP<T>&dd, double xFactor, double yFactor, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, rect, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, xFactor, yFactor, scale); }

// ResizeCenter (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDzd4T_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect rect, IM_TP<T>&dd, double xFactor, double yFactor, \
                          double xCenter, double yCenter, P scale){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, rect, dd.ptr(), dd.wstep, dd.roiSize, \
                                                        xFactor, yFactor, xCenter, yCenter, scale); }

// WarpAffine (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDrA23i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, IM_TP<T>&dd, IppiRect dstRect, \
                          const double coeffs[2][3], int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect,\
                                                        dd.ptr(), dd.wstep, dstRect, coeffs, interpolation); }

DECL_FORALLCH(WarpAffine, DECL_SZrDrA23i_TP)
DECL_FORALLCH(WarpAffineBack, DECL_SZrDrA23i_TP)

#define DECL_SZrA42DrA42i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, const double srcQuad[4][2], \
                          IM_TP<T>&dd, IppiRect dstRect, const double dstQuad[4][2], int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect, srcQuad, \
                                                        dd.ptr(), dd.wstep, dstRect, dstQuad, interpolation); }

DECL_FORALLCH(WarpAffineQuad, DECL_SZrA42DrA42i_TP)

// Rotate (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDrd3i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, IM_TP<T>&dd, IppiRect dstRect, \
                          double angle, double xShift, double yShift, int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect, dd.ptr(), dd.wstep, dstRect, \
                                                        angle, xShift, yShift, interpolation); }

DECL_FORALLCH(Rotate, DECL_SZrDrd3i_TP)
DECL_FORALLCH(RotateCenter, DECL_SZrDrd3i_TP)

// Shear (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDrd4i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, IM_TP<T>&dd, IppiRect dstRect, \
                          double xShear, double yShear, double xShift, double yShift, int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect, dd.ptr(), dd.wstep, dstRect, \
                                                        xShear, yShear, xShift, yShift, interpolation); }

DECL_FORALLCH(Shear, DECL_SZrDrd4i_TP)

// WarpPerspective (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDrA33i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, IM_TP<T>&dd, IppiRect dstRect, \
                          const double coeffs[3][3], int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect, \
                                                        dd.ptr(), dd.wstep, dstRect, coeffs, interpolation); }

DECL_FORALLCH(WarpPerspective, DECL_SZrDrA33i_TP)
DECL_FORALLCH(WarpPerspectiveBack, DECL_SZrDrA33i_TP)

DECL_FORALLCH(WarpPerspectiveQuad, DECL_SZrA42DrA42i_TP)

// WarpBilinear (without planes here, planes see below in appropriate FOR PLANES part)
#define DECL_SZrDrA24i_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IppiRect srcRect, IM_TP<T>&dd, IppiRect dstRect, \
                          const double coeffs[2][4], int interpolation){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.imgSize, s.wstep, srcRect, \
                                                        dd.ptr(), dd.wstep, dstRect, coeffs, interpolation); }

DECL_FORALLCH(WarpBilinear, DECL_SZrDrA24i_TP)
DECL_FORALLCH(WarpBilinearBack, DECL_SZrDrA24i_TP)

DECL_FORALLCH(WarpBilinearQuad, DECL_SZrA42DrA42i_TP)


// == Image Statistics Functions (#11)/

// ippiMomentInitAlloc ippiMomentFree ippiMomentGetStateSize ippiMomentInit - no wrapping
// Moments ([8u,32f]+64f or 8u+64s

#define DECL_SzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename M> \
    inline IppStatus Afun(const IM_TP<T>&s, M* pCtx){ \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pCtx); }

DECL_FOR13ACH(Moments64s, DECL_SzT_TP)
DECL_FOR13ACH(Moments64f, DECL_SzT_TP)

// ippiGet[Normalized][Spatial,Central]Moment, ippiGetHuMoment - no wrapping


// Norm. Masked from ippcv not included

#define DECL_Sztx_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename M> \
    inline IppStatus Afun(const IM_TP<T>&s, M* pCtx, IppHintAlgorithm hint){ \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pCtx, hint); }

DECL_FORALLCH(Norm_Inf, DECL_SzT_TP)
DECL_FORALLCH(Norm_L1, DECL_SzT_TP)
DECL_FORALLCH(Norm_L1, DECL_Sztx_TP)
DECL_FORALLCH(Norm_L2, DECL_SzT_TP)
DECL_FORALLCH(Norm_L2, DECL_Sztx_TP)

// NormDiff, NormRel function set
// Uses own template for a while

#define DECL_SzSzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, P* vals){ \
        return ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, s2.roiSize, vals);} \

#define DECL_SzSzth_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, P* vals, IppHintAlgorithm hint){ \
        return ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, s2.roiSize, vals, hint);}

#define DECL_CMPFUN_TP(Afun,IM_TP,F_SUFF) \
    DECL_SzSzT_TP(Afun,IM_TP,F_SUFF) \
    DECL_SzSzth_TP(Afun,IM_TP,F_SUFF)

// declare all templates for the group
DECL_FORALLCH(NormDiff_Inf, DECL_CMPFUN_TP)
DECL_FORALLCH(NormDiff_L1, DECL_CMPFUN_TP)
DECL_FORALLCH(NormDiff_L2, DECL_CMPFUN_TP)

DECL_FORALLCH(NormRel_Inf, DECL_CMPFUN_TP)
DECL_FORALLCH(NormRel_L1, DECL_CMPFUN_TP)
DECL_FORALLCH(NormRel_L2, DECL_CMPFUN_TP)

// Sum
DECL_FORALLCH(Sum, DECL_SzT_TP)
DECL_FORALLCH(Sum, DECL_Sztx_TP)

// Mean
DECL_FORALLCH(Mean, DECL_SzT_TP)
DECL_FORALLCH(Mean, DECL_Sztx_TP)

// QualityIndex
DECL_FOR13ACH(QualityIndex, DECL_SzSzT_TP)

// HistogramRange

#define DECL_HST1_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, Ipp32s* pHist, const L* pLevels, int nLevels) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pHist, pLevels, nLevels); }

#define DECL_HSTM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, Ipp32s** pHist, const L** pLevels, int* nLevels) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pHist, pLevels, nLevels); }

DECL_FORALLCH_M_1(HistogramRange,DECL_HSTM_TP,DECL_HST1_TP)

// HistogramEven

#define DECL_HSTE1_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, Ipp32s* pHist, L* pLevels, int nLevels, L lowerLevel, L upperLevel) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pHist, pLevels, nLevels, lowerLevel, upperLevel); }

#define DECL_HSTEM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, Ipp32s** pHist, L** pLevels, int* nLevels, L* lowerLevel, L* upperLevel) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, pHist, pLevels, nLevels, lowerLevel, upperLevel); }

DECL_FORALLCH_M_1(HistogramEven,DECL_HSTEM_TP,DECL_HSTE1_TP)


//- Lookup Table Conversion (from #6)

#define DECL_LUT1_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, const L* pValues, const L* pLevels, int nLevels) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, s.roiSize, pValues, pLevels, nLevels); }

#define DECL_LUTM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, const L** pValues, const L** pLevels, int* nLevels) { \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, s.roiSize, pValues, pLevels, nLevels); }

#define DECL_LUTI1_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(IM_TP<T>&dd, const L* pValues, const L* pLevels, int nLevels) { \
        return ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, pValues, pLevels, nLevels); }

#define DECL_LUTIM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename L> \
    inline IppStatus Afun(IM_TP<T>&dd, const L** pValues, const L** pLevels, int* nLevels) { \
        return ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, pValues, pLevels, nLevels); }

#define DECL_LUT(Afun) \
DECL_FORALLCH_M_1(Afun,DECL_LUTM_TP,DECL_LUT1_TP) \
DECL_FORALLCH_M_1(Afun,DECL_LUTIM_TP,DECL_LUTI1_TP)

DECL_LUT(LUT)
DECL_LUT(LUT_Linear)
DECL_LUT(LUT_Cubic)

// LUTPalette (not implemented for 8u24u and 16u24u

#define DECL_S1D2zTi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T1,typename T2> \
    inline IppStatus Afun(const IM_TP<T1>&s, IM_TP<T2>&dd, const T2* ptab, int sbit){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R((const T1*)s.ptr(), s.wstep, \
                                                        (T2*)dd.ptr(), dd.wstep, s.roiSize, ptab, sbit);}
DECL_S1D2zTi_TP(LUTPalette,GIMG,C1)


// CountInRange /from Image Statistics Functions (#11)/

#define DECL_SzitT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename B> \
    inline IppStatus Afun(const IM_TP<T>&s, int* counts, B lowerBound, B upperBound){ \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, counts, lowerBound, upperBound); }

DECL_FOR13ACH(CountInRange, DECL_SzitT_TP)


// == Filtering Functions (#9)
#define DECL_SDzM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, IppiSize maskSize, IppiPoint anchor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, maskSize, anchor);}

#define DECL_DzM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(IM_TP<T>&dd, IppiSize maskSize, IppiPoint anchor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, maskSize, anchor);}

// FilterMedianHoriz/Vert
DECL_FORALLCH(FilterMedianHoriz, DECL_SDzT_TP)
DECL_FORALLCH(FilterMedianVert, DECL_SDzT_TP)

// FilterMedian, FilterMedianCross, FilterMedianColor

#define DECL_FORCOLCH(Afun,DECL) \
    DECL(Afun,AIMG,AC4) \
    DECL(Afun,IMG3,C3)

DECL_FORALLCH(FilterMedian, DECL_SDzM_TP)
DECL_FOR13ACH(FilterMedianCross, DECL_SDzT_TP)
DECL_FORCOLCH(FilterMedianColor, DECL_SDzT_TP)

// FilterMax, FilterMin
DECL_FORALLCH(FilterMax, DECL_SDzM_TP)
DECL_FORALLCH(FilterMin, DECL_SDzM_TP)

// FilterBox
DECL_FORALLCH(FilterBox, DECL_SDzM_TP)
DECL_FORALLCH(FilterBox, DECL_DzM_TP)

// Fixed Kernel
DECL_FORALLCH(FilterPrewittVert, DECL_SDz_TP)
DECL_FORALLCH(FilterPrewittHoriz, DECL_SDz_TP)
DECL_FORALLCH(FilterSobelVert, DECL_SDz_TP)
DECL_FORALLCH(FilterSobelHoriz, DECL_SDz_TP)
DECL_FOR13ACH(FilterRobertsUp, DECL_SDz_TP)
DECL_FOR13ACH(FilterRobertsDown, DECL_SDz_TP)
DECL_FORALLCH(FilterSharpen, DECL_SDz_TP)
DECL_S1D2z_TP(FilterScharrVert, GIMG, C1)
DECL_S1D2z_TP(FilterScharrHoriz, GIMG, C1)

// Fixed Kernel with var size
DECL_FORALLCH(FilterLaplace, DECL_SDzT_TP)
DECL_FORALLCH(FilterGauss, DECL_SDzT_TP)
DECL_FORALLCH(FilterHipass, DECL_SDzT_TP)
DECL_FOR13ACH(FilterLowpass, DECL_SDzT_TP)

DECL_S1D2zT_TP(FilterLaplace, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelVert, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelHoriz, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelVertSecond, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelHorizSecond, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelCross, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelVertMask, GIMG, C1)
DECL_S1D2zT_TP(FilterSobelHorizMask, GIMG, C1)


#define DECL_SDzTTT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename K,typename P1,typename P2> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, const K* pKernel, P1 kernelSize, P2 anchor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize, \
                                                        pKernel, kernelSize, anchor);}

#define DECL_SDzTTTi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename K,typename P1,typename P2> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, const K* pKernel, P1 kernelSize, P2 anchor, int divisor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, dd.roiSize, \
                                                        pKernel, kernelSize, anchor, divisor);}

// General Linear Filters
DECL_FORALLCH(Filter, DECL_SDzTTTi_TP)
DECL_FORALLCH(Filter, DECL_SDzTTT_TP)
DECL_FORALLCH(Filter32f, DECL_SDzTTT_TP)

// Separable Filters
DECL_FORALLCH(FilterColumn, DECL_SDzTTTi_TP)
DECL_FORALLCH(FilterColumn, DECL_SDzTTT_TP)
DECL_FORALLCH(FilterColumn32f, DECL_SDzTTT_TP)

DECL_FORALLCH(FilterRow, DECL_SDzTTTi_TP)
DECL_FORALLCH(FilterRow, DECL_SDzTTT_TP)
DECL_FORALLCH(FilterRow32f, DECL_SDzTTT_TP)

// FilterWiener
#define DECL_SDzMxx_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, IppiSize maskSize, IppiPoint anchor, \
                          Ipp32f* noise, Ipp8u* pBuffer){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, maskSize, anchor, noise, pBuffer);}

DECL_FORALLCH(FilterWiener, DECL_SDzMxx_TP)


//- 2D Convolution (from #9)
#define DECL_SzSzi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd, int divisor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s1.roiSize, \
                                                        s2.ptr(), s2.wstep, s2.roiSize, dd.ptr(), dd.wstep, divisor);}

#define DECL_SzSzD_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s1.roiSize, \
                                                        s2.ptr(), s2.wstep, s2.roiSize, dd.ptr(), dd.wstep);}

DECL_FOR13ACH(ConvFull, DECL_SzSzD_TP)
DECL_FOR13ACH(ConvFull, DECL_SzSzi_TP)
DECL_FOR13ACH(ConvValid, DECL_SzSzD_TP)
DECL_FOR13ACH(ConvValid, DECL_SzSzi_TP)


//- Image Proximity Measures (from #11)

#define DECL_SzSzD32f_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<Ipp32f>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s1.roiSize, \
                                                        s2.ptr(), s2.wstep, s2.roiSize, dd.ptr(), dd.wstep);}

DECL_FORALLCH(CrossCorrFull_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrFull_Norm, DECL_SzSzi_TP)
DECL_FORALLCH(CrossCorrValid_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrValid_Norm, DECL_SzSzi_TP)
DECL_FORALLCH(CrossCorrSame_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrSame_Norm, DECL_SzSzi_TP)

DECL_FORALLCH(CrossCorrFull_NormLevel, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrFull_NormLevel, DECL_SzSzi_TP)
DECL_FORALLCH(CrossCorrValid_NormLevel, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrValid_NormLevel, DECL_SzSzi_TP)
DECL_FORALLCH(CrossCorrSame_NormLevel, DECL_SzSzD32f_TP)
DECL_FORALLCH(CrossCorrSame_NormLevel, DECL_SzSzi_TP)

DECL_FORALLCH(SqrDistanceFull_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(SqrDistanceFull_Norm, DECL_SzSzi_TP)
DECL_FORALLCH(SqrDistanceValid_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(SqrDistanceValid_Norm, DECL_SzSzi_TP)
DECL_FORALLCH(SqrDistanceSame_Norm, DECL_SzSzD32f_TP)
DECL_FORALLCH(SqrDistanceSame_Norm, DECL_SzSzi_TP)

//- Thresholding (from #7)
#define DECL_SDzTx_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, C threshold, IppCmpOp ippCmpOp){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, threshold, ippCmpOp);}

#define DECL_DzTx_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(IM_TP<T>&dd, C threshold, IppCmpOp ippCmpOp){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, threshold, ippCmpOp);}

#define DECL_SDzT2x_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, C threshold, C value, IppCmpOp ippCmpOp){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, threshold, value, ippCmpOp);}

#define DECL_DzT2x_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(IM_TP<T>&dd, C threshold, C value, IppCmpOp ippCmpOp){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, threshold, value, ippCmpOp);}

#define DECL_SDzT2_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, C threshold, C value){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, threshold, value);}

#define DECL_DzT2_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(IM_TP<T>&dd, C threshold, C value){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, threshold, value);}

#define DECL_SDzT4_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, C threshold1, C value1, C threshold2, C value2){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, \
                                                        dd.ptr(), dd.wstep, dd.roiSize, \
                                                        threshold1, value1, threshold2, value2);}

#define DECL_DzT4_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(IM_TP<T>&dd, C threshold1, C value1, C threshold2, C value2){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R( dd.ptr(), dd.wstep, dd.roiSize, \
                                                         threshold1, value1, threshold2, value2);}

DECL_FOR13ACH(Threshold, DECL_SDzTx_TP)
DECL_FOR13ACH(Threshold, DECL_DzTx_TP)
DECL_FOR13ACH(Threshold_GT, DECL_SDzT_TP)
DECL_FOR13ACH(Threshold_GT, DECL_DzT_TP)
DECL_FOR13ACH(Threshold_LT, DECL_SDzT_TP)
DECL_FOR13ACH(Threshold_LT, DECL_DzT_TP)
DECL_FOR13ACH(Threshold_Val, DECL_SDzT2x_TP)
DECL_FOR13ACH(Threshold_Val, DECL_DzT2x_TP)
DECL_FORALLCH(Threshold_GTVal, DECL_SDzT2_TP)
DECL_FORALLCH(Threshold_GTVal, DECL_DzT2_TP)
DECL_FORALLCH(Threshold_LTVal, DECL_SDzT2_TP)
DECL_FORALLCH(Threshold_LTVal, DECL_DzT2_TP)
DECL_FOR13ACH(Threshold_LTValGTVal, DECL_SDzT4_TP)
DECL_FOR13ACH(Threshold_LTValGTVal, DECL_DzT4_TP)


// == Image Data Exchange and Initialization Functions (#4)

// Copy (no complex)
// case of complete copy
#define DECL_CPYFUN_TP(Afun,IM_TPS,IM_TPD,F_SUFFS,F_SUFFD) \
    template<typename T> \
    inline IppStatus Afun( const IM_TPS<T>&s, IM_TPD<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFFS##F_SUFFD##R(s.ptr(),s.wstep, dd.ptr(),dd.wstep, dd.roiSize);}

// with mask
#define DECL_SDzB_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( const IM_TP<T>&s, IM_TP<T>&dd, const IM_TP<Ipp8u>&msk){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##MR(s.ptr(),s.wstep, dd.ptr(),dd.wstep, dd.roiSize, \
                                                         (const Ipp8u*)msk.ptr(),msk.wstep);}

// copy only one channel, channel indexes provided, c3, c4, additionally ac4
#define DECL_iiS6D6z_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( int si, int di, const IM_TP<T>&s, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##CR(s.ptr()+si,s.wstep, dd.ptr()+di,dd.wstep, dd.roiSize);}

// provided channel to 1C
#define DECL_iS6Dz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( int si, const IM_TP<T>&s, GIMG<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##C1R(s.ptr()+si,s.wstep, dd.ptr(),dd.wstep, dd.roiSize);}

// provided channel from 1C
#define DECL_iSD6z_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( int di, const GIMG<T>&s, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##C1##F_SUFF##R(s.ptr(),s.wstep, dd.ptr()+di,dd.wstep, dd.roiSize);}

// pair equal images and with mask
#define DECL_CPYFUN(Afun,IM_TP,F_SUFF) \
    DECL_SDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_SDzB_TP(Afun,IM_TP,F_SUFF)

//  DECL_CPYFUN_TP(Afun,IM_TP,IM_TP,F_SUFF, ) \

// pair C3 <-> AC4
#define DECL_CPYFUN34(Afun,IM_TPS,IM_TPD,F_SUFFS,F_SUFFD) \
    DECL_CPYFUN_TP(Afun,IM_TPS,IM_TPD,F_SUFFS,F_SUFFD) \
    DECL_CPYFUN_TP(Afun,IM_TPD,IM_TPS,F_SUFFD,F_SUFFS)

// to from 1C and selected by offsets one channel copy
#define DECL_CPY1FUN(Afun,IM_TP,F_SUFF) \
    DECL_iS6Dz_TP(Afun,IM_TP,F_SUFF) \
    DECL_iSD6z_TP(Afun,IM_TP,F_SUFF) \
    DECL_iiS6D6z_TP(Afun,IM_TP,F_SUFF)

// complete set of definitions
#define DECL_CPYFUN_ALLCH(Afun) \
    DECL_FORALLCH(Afun,DECL_CPYFUN) \
    DECL_CPYFUN34(Afun,AIMG,IMG3,AC4,C3) \
    DECL_CPY1FUN(Afun,IMG3,C3) \
    DECL_CPY1FUN(Afun,IMG4,C4) \
    DECL_CPY1FUN(Afun,AIMG,C4) /* use c4 for ac4 */

DECL_CPYFUN_ALLCH(Copy)

// CopyReplicateBorder, CopyConstBorder, CopyWarpBorder

#define DECL_SDzii_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, int topBorderWidth, int leftBorderWidth){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, dd.ptr(), dd.wstep, dd.roiSize, \
                                                        topBorderWidth, leftBorderWidth);}

#define DECL_SzDziiT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd, int topBorderWidth, int leftBorderWidth, C value){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, dd.ptr(), dd.wstep, dd.roiSize, \
                                                        topBorderWidth, leftBorderWidth, value);}

DECL_FORALLCH(CopyReplicateBorder, DECL_SDzii_TP)
DECL_FORALLCH(CopyConstBorder, DECL_SzDziiT_TP)

static inline IppStatus CopyWrapBorder(const GIMG<Ipp32s>&s, GIMG<Ipp32s>&dd,
                                       int topBorderWidth, int leftBorderWidth) {
    return dd.lastStatus = ippiCopyWrapBorder_C1R( (const Ipp32s*)s.ptr(), s.wstep, s.roiSize, (Ipp32s*)dd.ptr(), \
                                                   dd.wstep, dd.roiSize, topBorderWidth, leftBorderWidth); }

static inline IppStatus CopyWrapBorder(GIMG<Ipp32s>&dd, IppiSize srcRoiSize,
                                       int topBorderWidth, int leftBorderWidth) {
    return dd.lastStatus = ippiCopyWrapBorder_C1R( (const Ipp32s*)dd.ptr(), \
                                                   dd.wstep, srcRoiSize, dd.roiSize, topBorderWidth, leftBorderWidth); }

// Dup (8u_C1C3 only)
#define DECL_FROMC1_SDz_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( const GIMG<T>&s, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##C1##F_SUFF##R(s.ptr(),s.wstep, dd.ptr(),dd.wstep, dd.roiSize);}
DECL_FROMC1_SDz_TP(Dup,IMG3,C3)

// "Set" function set
// 3 subtypes - separate values for channels, the same with mask, common value for channels
/// UNRESOLVED issue - no ippi functions to Set complex data types
/// have problems with specifying "const C* vals" in DECL_PDR_TP with C3 i.e., can't use common template

// for Set one const for all channels (C3C etc)
//#define DECL_TDz_TP(Afun,IM_TP,F_SUFF) \
//    template<typename T,typename C> \
//    inline IppStatus Afun( C *pval, IM_TP<T>&dd){ \
//        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(pval, dd.ptr(), dd.wstep, dd.roiSize);}

// with mask image
#define DECL_tDzB_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun( C vals, IM_TP<T>&dd, GIMG<Ipp8u>&msk){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##MR(vals, dd.ptr(), dd.wstep, dd.roiSize, msk.ptr(), msk.wstep);}

// for selected channel
#define DECL_iD6z_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun( int di, C vals, IM_TP<T>&dd){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##CR(vals, dd.ptr()+di, dd.wstep, dd.roiSize);}

// complete set of definitions
#define DECL_SETFUN_ALLCH(Afun) \
    DECL_FORALLCH_M_1(Afun, DECL_TDz_TP, DECL_TDz_TP) \
    DECL_FORALLCH(Afun, DECL_tDzB_TP) \
    DECL_iD6z_TP(Afun,IMG3,C3) \
    DECL_iD6z_TP(Afun,IMG4,C4)

// the declaration
DECL_SETFUN_ALLCH(Set)

// AddRandUniform_Direct
#define DECL_DzTTi_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C> \
    inline IppStatus Afun( IM_TP<T>&dd, C low, C high, unsigned int* pSeed){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, low, high, pSeed);}

DECL_FORALLCH(AddRandUniform_Direct, DECL_DzTTi_TP)
DECL_FORALLCH(AddRandGauss_Direct, DECL_DzTTi_TP)

// ImageJaehne
DECL_FORALLCH(ImageJaehne, DECL_Dz_TP)

// ImageRamp
#define DECL_Dzffx_TP(Afun,IM_TP,F_SUFF) \
    template<typename T> \
    inline IppStatus Afun( IM_TP<T>&dd, float offset, float slope, IppiAxis axis){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, offset, slope, axis);}

DECL_FORALLCH(ImageRamp, DECL_Dzffx_TP)

// Convert
DECL_FORALLCH(Convert, DECL_S1D2z_TP)
// Convert from f32 with roundMode
DECL_FORALLCH(Convert, DECL_S1D2zT_TP)
// Convert 8u <-> 1u
static inline IppStatus Convert( GIMG<Ipp8u>&s, int srcBitOffset, GIMG<Ipp8u>&dd) {
    return dd.lastStatus = ippiConvert_1u8u_C1R( (const Ipp8u*)s.ptr(), s.wstep, srcBitOffset,
                                                 (Ipp8u*)dd.ptr(), dd.wstep, dd.roiSize ); }
static inline IppStatus Convert( GIMG<Ipp8u>&s, GIMG<Ipp8u>&dd, int srcBitOffset, Ipp8u threshold) {
    return dd.lastStatus = ippiReduceBits_8u1u_C1R((const Ipp8u*)s.ptr(), s.wstep, (Ipp8u*)dd.ptr(), dd.wstep, srcBitOffset, dd.roiSize, 0, 0, ippDitherStucki, threshold); }

// SwapChannels
DECL_SDzT_TP(SwapChannels,IMG3,C3)
DECL_SDzT_TP(SwapChannels,AIMG,AC4)
DECL_DzT_TP(SwapChannels,IMG3,C3)

// Scale
#define DECL_S1D2zT2_TP(Afun,IM_TP,F_SUFF) \
    template<typename T1,typename T2,typename P> \
    inline IppStatus Afun(const IM_TP<T1>&s, IM_TP<T2>&dd, P a, P b){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R((const T1*)s.ptr(), s.wstep, \
                                                        (T2*)dd.ptr(), dd.wstep, dd.roiSize, a, b);}
DECL_FORALLCH(Scale, DECL_S1D2z_TP)
DECL_FORALLCH(Scale, DECL_S1D2zT_TP)
DECL_FORALLCH(Scale, DECL_S1D2zT2_TP)

// Min, MinIndx, Max, MaxIndx, MinMax
#define DECL_SzTT2_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename M,typename I> \
    inline IppStatus Afun(const IM_TP<T>&s, M a, I b1, I b2){ \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, a, b1, b2); }

#define DECL_Szt2_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename I> \
    inline IppStatus Afun(const IM_TP<T>&s, I a1, I a2){ \
        return ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, s.roiSize, a1, a2); }

DECL_FORALLCH(Min, DECL_SzT_TP)
DECL_FORALLCH(MinIndx, DECL_SzTT2_TP)
DECL_FORALLCH(Max, DECL_SzT_TP)
DECL_FORALLCH(MaxIndx, DECL_SzTT2_TP)
DECL_FORALLCH(MinMax, DECL_Szt2_TP)


//- Logical Operations (from #5)

// And, Or, Xor
#define DECL_LFUN(Afun,IM_TP,F_SUFF) \
    DECL_SDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_SSDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_TDz_TP(Afun,IM_TP,F_SUFF) \
    DECL_StDz_TP(Afun,IM_TP,F_SUFF)

DECL_FORALLCH(And, DECL_LFUN)
DECL_FORALLCH(Or, DECL_LFUN)
DECL_FORALLCH(Xor, DECL_LFUN)

// Not
DECL_FORALLCH(Not, DECL_SDz_TP)
DECL_FORALLCH(Not, DECL_Dz_TP)

// LShift, RShift
DECL_FORALLCH(LShift, DECL_StDz_TP)
DECL_FORALLCH(LShift, DECL_TDz_TP)
DECL_FORALLCH(RShift, DECL_StDz_TP)
DECL_FORALLCH(RShift, DECL_TDz_TP)

//- Compare Operations (from #7)
#define DECL_SSD8uzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s1, const IM_TP<T>&s2, IM_TP<Ipp8u>&dd, P par){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s1.ptr(), s1.wstep, s2.ptr(), s2.wstep, \
                                                        (Ipp8u*)dd.ptr(), dd.wstep, dd.roiSize, par);}

#define DECL_SStD8uzT_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename C,typename P> \
    inline IppStatus Afun(const IM_TP<T>&s, C pval, IM_TP<Ipp8u>&dd, P par){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, pval, \
                                                        (Ipp8u*)dd.ptr(), dd.wstep, dd.roiSize, par);}

DECL_FORALLCH(Compare, DECL_SSD8uzT_TP)
DECL_FORALLCH(Compare, DECL_SStD8uzT_TP)
DECL_FORALLCH(CompareEqualEps, DECL_SSD8uzT_TP)
DECL_FORALLCH(CompareEqualEps, DECL_SStD8uzT_TP)


// == Morphological Operations (#8)

#define DECL_DzTM_TP(Afun,IM_TP,F_SUFF) \
    template<typename T,typename K> \
    inline IppStatus Afun(IM_TP<T>&dd, const K* pKernel, IppiSize kernelSize, IppiPoint anchor){ \
        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(dd.ptr(), dd.wstep, dd.roiSize, pKernel, kernelSize, anchor);}

DECL_FORALLCH(Erode3x3,  DECL_SDz_TP)
DECL_FORALLCH(Erode3x3,  DECL_Dz_TP)
DECL_FORALLCH(Dilate3x3, DECL_SDz_TP)
DECL_FORALLCH(Dilate3x3, DECL_Dz_TP)

DECL_FORALLCH(Erode,  DECL_SDzTTT_TP)
DECL_FOR13ACH(Erode,  DECL_DzTM_TP)
DECL_FORALLCH(Dilate, DECL_SDzTTT_TP)
DECL_FOR13ACH(Dilate, DECL_DzTM_TP)

// Zigzag - no wrapping

//- Windowing Functions (from #10)
DECL_Dz_TP(WinBartlett,GIMG,C1)
DECL_SDz_TP(WinBartlett,GIMG,C1)
DECL_Dz_TP(WinBartlettSep,GIMG,C1)
DECL_SDz_TP(WinBartlettSep,GIMG,C1)
DECL_Dz_TP(WinHamming,GIMG,C1)
DECL_SDz_TP(WinHamming,GIMG,C1)
DECL_Dz_TP(WinHammingSep,GIMG,C1)
DECL_SDz_TP(WinHammingSep,GIMG,C1)

// Transpose /from Image Statistics Functions (#11)/
// As a result of new name rules applying the following define matches with existing one (commented):
//#define DECL_SDz_TP(Afun,IM_TP,F_SUFF) \
//    template<typename T> \
//    inline IppStatus Afun(const IM_TP<T>&s, IM_TP<T>&dd){ \
//        return dd.lastStatus = ippi##Afun##_##F_SUFF##R(s.ptr(), s.wstep, dd.ptr(), dd.wstep, s.roiSize);}

DECL_FORALLCH(Transpose, DECL_SDz_TP)
DECL_FORALLCH(Transpose, DECL_Dz_TP)

/* ------------------------------------------------------------------------------------------------------------------- */
// FOR PLANES

#define DECL_P_FOR34CH(Afun,DECL) \
    DECL(Afun,IMG3P,P3) \
    DECL(Afun,IMG4P,P4)

DECL_P_FOR34CH(Remap, DECL_SzrBBDzT_TP)

DECL_P_FOR34CH(Resize, DECL_SZrDzddT_TP)

DECL_P_FOR34CH(ResizeCenter, DECL_SZrDzd4T_TP)

DECL_P_FOR34CH(ResizeShift, DECL_SZrDzd4T_TP)

DECL_P_FOR34CH(WarpAffine, DECL_SZrDrA23i_TP)
DECL_P_FOR34CH(WarpAffineBack, DECL_SZrDrA23i_TP)
DECL_P_FOR34CH(WarpAffineQuad, DECL_SZrA42DrA42i_TP)

DECL_P_FOR34CH(Rotate, DECL_SZrDrd3i_TP)
DECL_P_FOR34CH(RotateCenter, DECL_SZrDrd3i_TP)

DECL_P_FOR34CH(Shear, DECL_SZrDrd4i_TP)

DECL_P_FOR34CH(WarpPerspective, DECL_SZrDrA33i_TP)
DECL_P_FOR34CH(WarpPerspectiveBack, DECL_SZrDrA33i_TP)
DECL_P_FOR34CH(WarpPerspectiveQuad, DECL_SZrA42DrA42i_TP)

DECL_P_FOR34CH(WarpBilinear, DECL_SZrDrA24i_TP)
DECL_P_FOR34CH(WarpBilinearBack, DECL_SZrDrA24i_TP)
DECL_P_FOR34CH(WarpBilinearQuad, DECL_SZrA42DrA42i_TP)

DECL_CPYFUN34(Copy,IMG3,IMG3P,C3,P3)
DECL_CPYFUN34(Copy,IMG4,IMG4P,C4,P4)

DECL_SDz_TP(AlphaPremul,AIMGP,AP4)
DECL_Dz_TP(AlphaPremul,AIMGP,AP4)
DECL_StDz_TP(AlphaPremul,AIMGP,AP4)
DECL_TDz_TP(AlphaPremul,AIMGP,AP4)

DECL_SSDzT_TP(AlphaComp,AIMGP,AP4)
DECL_StStDzT_TP(AlphaComp,AIMGP,AP4)


#endif
#endif

/// End of include file with wrappers

