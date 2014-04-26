/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiRun.h : interface for the CippiRun class.
// CippiRun is derived from CRun class that provides CDemo documents
// processing by IPP functions.
// CippiRun is the base class for all ippiDemo Run classes that process
// images by concrete ippIP, ippCC, ippCV, ippJP functions.
// See CRun class for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_H__D03D5302_C033_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUN_H__D03D5302_C033_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Run.h"
#include "Image.h"
class CippiDemoDoc;

// Further macros use J_CALL, CC_CALL & CV_CALL macros that is defined
// in CippiDemo.h

// MATE_J_CALL macro produces ippJP function call if its name equals to Mate
#define MATE_J_CALL(Mate,Name,Params) { \
    if (Mate == #Name) { \
        return J_CALL(Name, Params); \
    } \
}

// FUNC_J_CALL should be used in CRun::CallIppFunction().
// FUNC_J_CALL produces ippJP function call if its name equals to m_Func
// but if CRun::m_Outstand == TRUE then FUNC_J_CALL adds
// function name to the list.
#define FUNC_J_CALL(Name, Params) { \
    if (m_Outstand) { \
        m_FuncList.AddTail((CFunc)(#Name)); \
    } else { \
        if (m_Func == #Name) \
            return J_CALL(Name, Params); \
    } \
}

// CHECK_J_CALL may be used in functions that return BOOL.
// CHECK_J_CALL macro produces ippJP function call and checks ippJP status
#define CHECK_J_CALL(Func, Params) { \
    IppStatus status = J_CALL(Func,Params); \
    CRun::IppErrorMessage(#Func, status); \
    if (status < 0) return FALSE; \
}

// MATE_CV_CALL macro produces ippCV function call if its name equals to Mate
#define MATE_CV_CALL(Mate,Name,Params) { \
    if (Mate == #Name) { \
        return CV_CALL(Name, Params); \
    } \
}

// FUNC_CV_CALL should be used in CRun::CallIppFunction().
// FUNC_CV_CALL produces ippCV function call if its name equals to m_Func
// but if CRun::m_Outstand == TRUE then FUNC_CV_CALL adds
// function name to the list.
#define FUNC_CV_CALL(Name, Params) { \
    if (m_Outstand) { \
        m_FuncList.AddTail((CFunc)(#Name)); \
    } else { \
        if (m_Func == #Name) \
            return CV_CALL(Name, Params); \
    } \
}

// CHECK_CV_CALL may be used in functions that return BOOL.
// CHECK_CV_CALL macro produces ippCV function call and checks ippCV status
#define CHECK_CV_CALL(Func, Params) { \
    IppStatus status = CV_CALL(Func,Params); \
    CRun::IppErrorMessage(#Func, status); \
    if (status < 0) return FALSE; \
}

// MATE_CC_CALL macro produces ippCC function call if its name equals to Mate
#define MATE_CC_CALL(Mate,Name,Params) { \
    if (Mate == #Name) { \
        return CC_CALL(Name, Params); \
    } \
}

// FUNC_CC_CALL should be used in CRun::CallIppFunction().
// FUNC_CC_CALL produces ippCC function call if its name equals to m_Func
// but if CRun::m_Outstand == TRUE then FUNC_CC_CALL adds
// function name to the list.
#define FUNC_CC_CALL(Name, Params) { \
    if (m_Outstand) { \
        m_FuncList.AddTail((CFunc)(#Name)); \
    } else { \
        if (m_Func == #Name) \
            return CC_CALL(Name, Params); \
    } \
}

// CHECK_CC_CALL may be used in functions that return BOOL.
// CHECK_CC_CALL macro produces ippCC function call and checks ippCC status
#define CHECK_CC_CALL(Func, Params) { \
    IppStatus status = CC_CALL(Func,Params); \
    CRun::IppErrorMessage(#Func, status); \
    if (status < 0) return FALSE; \
}

enum {
    RUNMOVIE_NO        = 0x00,
    RUNMOVIE_YES      = 0x01,
    RUNMOVIE_REDRAW  = 0x02,
    RUNMOVIE_COPYSRC = 0x04,
    RUNMOVIE_RECUR    = 0x08, };

enum {GEOM_CONTOUR, GEOM_ROI, GEOM_CUSTOM};
enum {BORDER_NONE, BORDER_REDUCE, BORDER_REPLICATE, BORDER_CONST, BORDER_ZERO};

class CippiRun : public CRun
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor
    CippiRun();
    virtual ~CippiRun();

/////////////////////////////////////////////////////////////////////////////
// CRun Public Operations
    // ProcessFunctionStart is called by CDirector class at the very beginning
    // of processing document by IPP function.
    // Returns TRUE if function with funcName may be called by this CRun class
    // instance.
    virtual BOOL  ProcessFunctionStart(CDemoDoc *pDoc, CString funcName);
    virtual void  ProcessFunctionFinish();

/////////////////////////////////////////////////////////////////////////////
// CRun Public Attributes
    // VectorTerm returns general name of document ("vector" or "image")
    virtual CString VectorTerm() { return "image";}

/////////////////////////////////////////////////////////////////////////////
// CRun Gets Border Options
    static int DoOptBorderDlg();

/////////////////////////////////////////////////////////////////////////////
// Movie interface
    // IsMovie returns if function "func" takes part in movie or not
    virtual int  IsMovie(CFunc func);
    // ProcessMovie is called by CMovie class. It applies function to the image
    // for movie
    virtual int ProcessMovie(CFunc func,
        CImage *pImgSrc, CImage* pImgDst, CImage* m_ImgTmp,
        int time, CMyString srcInfo, CMyString& dstInfo);

protected:

    void*    pSrc;
    void*    pSrc2;
    void*    pDst;
    void*    pMask;
    void* pSrcImgP[4];
    void* pSrcImg2P[4];
    void* pDstImgP[4];
    void* pSrcP[4];
    void* pSrc2P[4];
    void* pDstP[4];
    int srcStepP[4];
    int srcStep2P[4];
    int dstStepP[4];
    void*    pSrcImg;
    void*    pSrcImg2;
    void*    pDstImg;
    int      srcStep;
    int      srcStep2;
    int      maskStep;
    int      dstStep;
    IppiSize roiSize;
    IppiSize m_srcSize;
    IppiSize m_srcSize2;
    IppiSize m_dstSize;
    IppiSize m_srcRoiSize;
    IppiSize m_srcRoiSize2;
    IppiSize m_dstRoiSize;
    IppiRect m_srcROI;
    IppiRect m_srcROI2;
    IppiRect m_dstROI;

    virtual BOOL Open(CFunc func);
    virtual CMyString GetVectorName(int vecPos);
    virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
    virtual ImgHeader GetNewDstHeader();
    virtual int GetSrcBorder(int idx) { return 0;}
    virtual void CopyDataToNewDst();
    virtual void CopyContoursToNewDst();
    virtual BOOL PrepareDst();
    virtual BOOL PrepareSrc();
    virtual BOOL ProcessCall();
    virtual void SetCoiToDocs(int coi);
    virtual void PrepareParameters();
    virtual BOOL BeforeCall() { return TRUE;}
    virtual BOOL AfterCall(BOOL bOK) { return TRUE;}
    virtual void ActivateDst();
    virtual void SaveInplaceVector();
    virtual void SetInplaceParms();
    virtual void ResetInplaceParms();
    virtual double GetPerfDivisor();
    virtual CString GetPerfUnit();

    virtual void SetToHistoFunc(CDemoDoc* pDoc, int vecPos);
    virtual void SetToHistoDst(CDemoDoc* pDoc, int vecPos);

    static BOOL IsImageByPlanes(CImage* pImg);
    virtual BOOL ByPlanes();
    virtual int  MaxChannels();
    virtual int  MinChannels();
    virtual BOOL NoDst();
    void DeleteNewDst();
    void ActivateDoc(CDemoDoc* pDoc);
    void ActivateMask(int beforeVec);
    static void SetImageParameters(CImage* image,
                                             void*& imgPtr, void*& roiPtr, int& step,
                                             void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4],
                                             IppiRect& roiRect, IppiSize& roiSize, IppiSize& imgSize);
    static void SetImageParameters(CImage* image, void*& roiPtr, int& step);
    static void SetImagePlanes(CImage* image,
                                        void* imgPlanePtr[4], void* roiPlanePtr[4], int planeStep[4]);
    static void IntersectRoi(CImage* image, IppiSize& roiSize);
     BOOL     MyModes(CString modeStr);
     static CippiDemoDoc* CreateNewDoc(ImgHeader header, CString title = "", BOOL bMakeVisible = FALSE);
     static CippiDemoDoc* CreateNewDoc(CImage* ippImage, CString title = "", BOOL bMakeVisible = FALSE);

    int  GetInterpolate(int interDlg, BOOL smoothDlg = 0);
    void SetInterpolate(int interpolate, int& interDlg);
    void SetInterpolate(int interpolate, int& interDlg, BOOL& smoothDlg);
    CString GetInterParm(int interpolate);

    BOOL Complex();

    virtual int GetBorderType(CDemoDoc* pDoc);
    virtual BOOL FixBorderProblem(CDemoDoc** ppDoc);
    virtual void ReleaseBorderProblem();
    virtual BOOL CreateBorderRoi(CImage* pImg);
    virtual BOOL GetBorderExpansion(CImage* pImg, int expand[4]);

    BOOL GetSrcBorders(int borders[4]);
    BOOL GetRoiExpansion(CImage* pImg, int borders[4], int expand[4]);
    BOOL CreateBorderDoc(CDemoDoc** ppDoc, int borderType, int expand[4]);
    IppiRect GetReducedRoi(CImage* pImg, int expand[4]);
    BOOL SetReducedRoi(CImage* pImg, IppiRect roi, CString imgName = "");
    BOOL SetReducedRoi(CImage* pImg, int expand[4], CString imgName = "");
    BOOL SetReducedRoi(CDemoDoc* pDoc, IppiRect roi);
    BOOL SetReducedRoi(CDemoDoc* pDoc, int expand[4]);
    static int  m_borderType;
    static BOOL m_borderConstZero;
    static CVectorUnit m_borderConst;

    CImage        m_InplaceImage;

protected:

     void SetDoc(CString title, CippiDemoDoc * & pDoc);
     void CreateCompDoc(CippiDemoDoc* pDoc, double normC);

protected:
    int        m_scaleFactor_Movie;

    virtual void PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst);
    virtual int  SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                                      int& numReps);
    virtual void ResetMovieArgs();
    int GetMovieCycle();
    int GetMovieCycleReverse(int& tim);
    double GetMovieValue(int tim, double maxVal);
    double GetMovieValueReverse(int tim, double maxVal);
    int GetMovieStep(int& tim, int nSteps);
    int GetMovieStepReverse(int& tim, int nSteps);
    void SetMovieScaleFactor(int val) {
        m_scaleFactor_Movie = scaleFactor;
        scaleFactor = val;
    }
    void ResetMovieScaleFactor() { scaleFactor = m_scaleFactor_Movie;}
    void SetMovieBorderRoi(CImage* pImgSrc, CImage* pImgDst);
};

#endif // !defined(AFX_RUN_H__D03D5302_C033_11D1_AE6B_444553540000__INCLUDED_)
