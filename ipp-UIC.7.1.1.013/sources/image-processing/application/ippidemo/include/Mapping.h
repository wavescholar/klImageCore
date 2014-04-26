/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Mapping.h: interface for the CMapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPPING_H__6FD674C3_FDEE_11D2_8F02_00AA00A03C3C__INCLUDED_)
#define AFX_MAPPING_H__6FD674C3_FDEE_11D2_8F02_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "ippiDemoDoc.h"

enum {MOD_BILINEAR, MOD_PERSPECTIVE, MOD_EXPLODE, MOD_FACETED,
      MOD_EYE, MOD_WHIRL, MOD_MOSAIC, MOD_NUM};

class CMapping
{
public:
    CMapping();
    CMapping(const CMapping& data);
    virtual ~CMapping();
    CMapping& operator =(const CMapping& data);

    void SetDocs(CDemoDoc* pDocSrc, CDemoDoc* pDocDst) {
        m_srcImage = pDocSrc->GetImage();
        m_dstImage = pDocDst ? pDocDst->GetImage() : m_srcImage;
    }
    virtual BOOL SetData(CImage* mapImg, BOOL bYMap);
    virtual CString GetInfo() {return "";}

    enum {NUM_EDIT = 6, NUM_RADIO = 4};

    int     m_Mode;
    CString m_Name;
    CString m_RadioName[NUM_RADIO];
    int     m_Radio;
    int     m_RadioNum;
    CString m_TextName[NUM_EDIT];
    CString m_Text[NUM_EDIT];
    int     m_EditStart, m_EditEnd;
protected:
    BOOL    m_bXMap;
    CImage* m_mapImage;
    CImage* m_srcImage;
    CImage* m_dstImage;
    virtual BOOL InitMap(){return TRUE;}
    virtual float GetMap(int x, int y) {return 0;}

    void InitRadio(char* names[], int num, int value = 0);
    void InitText(char* names[], char* value[], int num, int start = 0);
};

class CMapPtrArray : public CArray<CMapping*, CMapping*>
{
public:
    CMapPtrArray();
    ~CMapPtrArray();
    CMapPtrArray& operator =(const CMapPtrArray& array);
    void RemoveAll();
    void Create();
};

class CMapBilinear : public CMapping
{
public:
    CMapBilinear();
    CMapBilinear(const CMapBilinear& data);
    virtual ~CMapBilinear();
    CMapBilinear& operator =(const CMapBilinear& data);

    virtual CString GetInfo();
protected:
    enum {PARL_TO_QUAD, QUAD_TO_PARL, QUAD_TO_QUAD, PARL_TO_PARL};
    enum {Q_ROI, Q_PARL, Q_QUAD};

    double m_dstQuad[4][2];
    double m_B[2][4];

    virtual BOOL InitMap();
    virtual float GetMap(int x, int y);

    virtual BOOL GetQuadModes(int radio, int& srcMode, int& dstMode);
    virtual BOOL GetQuad(CImage* img, int mode, double quad[4][2]);
    virtual BOOL GetTransform(double srcQuad[4][2], double dstQuad[4][2]);
    virtual void SetRadioName(int idx, int srcMode, int dstMode);
};

class CMapPerspective : public CMapBilinear
{
public:
    CMapPerspective();
    CMapPerspective(const CMapPerspective& data);
    virtual ~CMapPerspective();
    CMapPerspective& operator =(const CMapPerspective& data);


    virtual float GetMap(int x, int y);

    virtual BOOL GetTransform(double srcQuad[4][2], double dstQuad[4][2]);
protected:
    double m_P[3][3];
};

class CMapExplode : public CMapping
{
public:
    CMapExplode();
    CMapExplode(const CMapExplode& data);
    virtual ~CMapExplode();
    CMapExplode& operator =(const CMapExplode& data);

    virtual CString GetInfo();
protected:
    virtual float GetMap(int x, int y);
};

class CMapFaceted : public CMapping
{
public:
    CMapFaceted();
    CMapFaceted(const CMapFaceted& data);
    virtual ~CMapFaceted();
    CMapFaceted& operator =(const CMapFaceted& data);

    virtual BOOL SetData(CImage* mapImg, BOOL bYMap);
    virtual CString GetInfo();
protected:
};

class CMapEye : public CMapping
{
public:
    CMapEye();
    CMapEye(const CMapEye& data);
    virtual ~CMapEye();
    CMapEye& operator =(const CMapEye& data);

    virtual CString GetInfo();
protected:
    virtual float GetMap(int x, int y);
};

class CMapWirl : public CMapping
{
public:
    CMapWirl();
    CMapWirl(const CMapWirl& data);
    virtual ~CMapWirl();
    CMapWirl& operator =(const CMapWirl& data);

    virtual CString GetInfo();
protected:
    virtual float GetMap(int x, int y);
};

class CMapMosaic : public CMapping
{
public:
    CMapMosaic();
    CMapMosaic(const CMapMosaic& data);
    virtual ~CMapMosaic();
    CMapMosaic& operator =(const CMapMosaic& data);

    virtual CString GetInfo();
protected:
    virtual BOOL InitMap();
    virtual float GetMap(int x, int y);
};

#endif // !defined(AFX_MAPPING_H__6FD674C3_FDEE_11D2_8F02_00AA00A03C3C__INCLUDED_)
