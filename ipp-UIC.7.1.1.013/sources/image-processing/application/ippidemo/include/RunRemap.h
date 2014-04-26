/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRemap.h: interface for the CRunRemap class.
// CRunRemap class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNREMAP_H__64C0D652_FBB4_11D2_8EFE_00AA00A03C3C__INCLUDED_)
#define AFX_RUNREMAP_H__64C0D652_FBB4_11D2_8EFE_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"
#include "ParmRemapDlg.h"

typedef float (*FGetMap) (int x, int y, BOOL bDstY);

class CRunRemap : public CippiRun
{
public:
   CRunRemap();
   virtual ~CRunRemap();

protected:
    int m_Type;
    int m_Trans;
    BOOL m_ShowMaps;
    int m_interpolate;

    CippiDemoDoc* m_pDocXMap;
    CippiDemoDoc* m_pDocYMap;
    CImage*   m_pImgXMap;
    CImage*   m_pImgYMap;
    float* m_xMap;
    float* m_yMap;
    int    m_xMapStep;
    int    m_yMapStep;

    CMapPtrArray m_MapPtrArray;

    virtual CParamDlg* CreateDlg() { return new CParmRemapDlg;}
    virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual void CopyDataToNewDst() {}
    virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual CString GetHistoryInfo();
    virtual BOOL PrepareDst();
   virtual void ActivateDst();
    virtual BOOL AfterCall(BOOL bOK);

private:
   IppiRect GetDstRoi();
   CImage* Create32fC1(ImgHeader header, IppiRect roi);
    void Delete32fC1(CImage* img);
    CImage* CreateMapImage(BOOL bY);
    void DeleteMapImage(CImage* img);
    void DeleteMaps();
    BOOL PrepareMaps();
    void SetMapsTitle();
    void PrepareMapsParms();
};

#endif // !defined(AFX_RUNREMAP_H__64C0D652_FBB4_11D2_8EFE_00AA00A03C3C__INCLUDED_)
