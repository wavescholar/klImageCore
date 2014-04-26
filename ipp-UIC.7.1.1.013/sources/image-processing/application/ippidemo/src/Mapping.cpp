/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Mapping.cpp: implementation of the CMapping class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiRun.h"
#include "Mapping.h"

#include "math.h"
#include "float.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

static float RandScale = 1.0f / (float)RAND_MAX;

static float Rand(float min, float max)
{
    return (float)rand()*(max - min)*RandScale + min;
}

static int Rand(int min, int max)
{
    return (int)Rand((float)min,(float)max);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapping::CMapping()
{
    int i;
    m_Mode = -1;
    m_Name = "";
    for (i=0; i<NUM_RADIO; i++) {
       m_RadioName[i].Empty();
    }
    m_Radio = 0;
    m_RadioNum = 0;
    for (i=0; i<NUM_EDIT; i++) {
       m_TextName[i].Empty();
       m_Text[i].Empty();
    }
    m_EditStart = 0;
    m_EditEnd = 0;

    m_bXMap = TRUE;
    m_mapImage = NULL;
    m_srcImage = NULL;
    m_dstImage = NULL;
}

CMapping::CMapping(const CMapping& data)
{
    *this = data;
}

CMapping::~CMapping()
{
}

CMapping& CMapping::operator =(const CMapping& data)
{
    int i;
    m_Mode = data.m_Mode;
    m_Name = data.m_Name;
    for (i=0; i<NUM_RADIO; i++) {
       m_RadioName[i] = data.m_RadioName[i];
    }
    m_Radio    = data.m_Radio   ;
    m_RadioNum = data.m_RadioNum;
    for (i=0; i<NUM_EDIT; i++) {
       m_TextName[i] = data.m_TextName[i];
       m_Text[i]     = data.m_Text[i]    ;
    }
    m_EditStart = data.m_EditStart;
    m_EditEnd   = data.m_EditEnd  ;

    m_mapImage = data.m_mapImage;
    m_srcImage = data.m_srcImage;
    m_dstImage = data.m_dstImage;
    m_bXMap   = data.m_bXMap  ;
    return *this;
}

void CMapping::InitRadio(char* names[], int num, int value)
{
    if (num > NUM_RADIO) num = NUM_RADIO;
    m_RadioNum = num;
    m_Radio    = value;
    for (int i=0; i<num; i++) {
       m_RadioName[i] = names[i];
    }
}

void CMapping::InitText(char* names[], char* value[], int num, int start)
{
    m_EditStart = start;
    m_EditEnd   = start + num;
    if (m_EditEnd > NUM_EDIT) m_EditEnd = NUM_EDIT;
    for (int i=m_EditStart, j=0; i<m_EditEnd; i++, j++) {
       m_TextName[i] = names[j];
       m_Text[i]     = value[j];
    }
}

BOOL CMapping::SetData(CImage* img, BOOL bY)
{
    m_mapImage = img;
    m_bXMap = !bY;
    if (!InitMap()) return FALSE;
    IppiRect roi = img->GetActualRoi();
    float* dst = (float*)img->GetRoiPtr();
    int    step = img->Step() >> 2;
    for (int y=roi.y; y<roi.y+roi.height; y++) {
        for (int x=roi.x, i=0; x<roi.x+roi.width; x++,i++) {
            dst[i] = GetMap(x,y);
        }
        dst += step;
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CMapBilinear Class
//////////////////////////////////////////////////////////////////////

static void copyQuad(const double src[4][2], double dst[4][2])
{
    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++)
            dst[i][j] = src[i][j];
}

CMapBilinear::CMapBilinear()
{
    m_Mode = MOD_BILINEAR;
    m_Name = "Bilinear";
    char* transNames[] = {
        "Quad to Parl", "Parl to Quad",
        "Quad to Quad", "Parl to Parl",
    };
    InitRadio(transNames,ARRAY_LEN(transNames));
}

CMapBilinear::CMapBilinear(const CMapBilinear& data)
{
    *this = data;
}

CMapBilinear::~CMapBilinear()
{
}

CMapBilinear& CMapBilinear::operator =(const CMapBilinear& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    copyQuad(data.m_dstQuad, m_dstQuad);
    for (int i=0; i<2; i++)
        for (int j=0; j<4; j++)
            m_B[i][j] = data.m_B[i][j];
    return *this;
}

BOOL CMapBilinear::InitMap()
{
    int srcMode, dstMode;
    double srcQuad[4][2];
    double dstQuad[4][2];
    if (!GetQuadModes(m_Radio, srcMode, dstMode))
        return FALSE;
    SetRadioName(m_Radio, srcMode, dstMode);
    GetQuad(m_srcImage, srcMode, srcQuad);
    GetQuad(m_dstImage, dstMode, dstQuad);
    return GetTransform(srcQuad, dstQuad);
}

BOOL CMapBilinear::GetQuadModes(int radio, int& srcMode, int& dstMode)
{
    srcMode = dstMode = Q_ROI;
    switch (radio) {
    case PARL_TO_QUAD:
        if (m_dstImage->IsQuad())
        {
            dstMode = Q_QUAD;
            srcMode = m_srcImage->IsParl() ? Q_PARL : Q_ROI;
        }
        else if (m_dstImage->IsParl())
        {
            dstMode = Q_PARL;
            srcMode = Q_ROI;
        }
        else
            return FALSE;
        break;
    case QUAD_TO_PARL:
        if (m_srcImage->IsQuad())
        {
            srcMode = Q_QUAD;
            dstMode = m_dstImage->IsParl() ? Q_PARL : Q_ROI;
        }
        else if (m_srcImage->IsParl())
        {
            srcMode = Q_PARL;
            dstMode = Q_ROI;
        }
        else
            return FALSE;
        break;
    case QUAD_TO_QUAD:
        if (m_srcImage == m_dstImage)
            return FALSE;
        if (!m_srcImage->IsQuad() || !m_dstImage->IsQuad())
            return FALSE;
        srcMode = dstMode = Q_QUAD;
        break;
    case PARL_TO_PARL:
        if (m_srcImage == m_dstImage)
            return FALSE;
        if (!m_srcImage->IsParl() || !m_dstImage->IsParl())
            return FALSE;
        srcMode = dstMode = Q_PARL;
        break;
    }
    return TRUE;
}

void CMapBilinear::SetRadioName(int idx, int srcMode, int dstMode)
{
    char* name[3] = {"ROI", "Parl", "Quad"};
    CString to(" to ");
    m_RadioName[idx] = name[srcMode] + to + name[dstMode];
}

static void RoiToQuad(IppiRect roi, double quad[4][2])
{
    quad[0][0] = roi.x;
    quad[0][1] = roi.y;
    quad[1][0] = roi.x + roi.width - 1;
    quad[1][1] = roi.y;
    quad[2][0] = roi.x + roi.width - 1;
    quad[2][1] = roi.y + roi.height - 1;
    quad[3][0] = roi.x;
    quad[3][1] = roi.y + roi.height - 1;
}

BOOL CMapBilinear::GetQuad(CImage* img, int mode, double quad[4][2])
{
    switch (mode)
    {
    case Q_QUAD:
        return img->GetQuad(quad);
    case Q_PARL:
        return img->GetParl(quad);
    case Q_ROI:
        break;
    }
    IppiRect roi = img->GetActualRoi();
    RoiToQuad(roi, quad); 
    return TRUE;
}

struct Spot
{
    double x;
    double y;
};

template <class T>
void swap(T& a, T& b) 
{
    T t;
    t = a;
    a = b;
    b = t;
}

static BOOL scanY(double& x, double y, Spot a, Spot b)
{
    if (a.y > b.y)
        swap(a,b);
    if (y < a.y || y > b.y)
        return FALSE;
    x = (b.x*(a.y - y) - a.x*(b.y - y)) / ((a.y - y) - (b.y - y));
    return TRUE;
}


static BOOL scanY(double& x, double y, double quad[4][2], int idx)
{
    Spot a = {quad[idx][0], quad[idx][1]};
    Spot b = {quad[(idx + 1)%4][0], quad[(idx + 1)%4][1]};
    return scanY(x, y, a, b);
}


static BOOL inQuad(double x, double y, double quad[4][2])
{
    double xx[3];
    int n = 0;
    for (int i=0; i<4; i++)
    {
        if (scanY(xx[n], y, quad, i))
        {
            if (n == 2)
            {
                if (ABS(xx[0] - xx[1]) < 1.e-10)
                    xx[0] = xx[2];
                break;
            }
            n++;
        }
    }
    if (n < 2)
        return FALSE;
    if (xx[0] > xx[1])
        swap(xx[0], xx[1]);
    return (xx[0] <= x) && (x <= xx[1]);
}

BOOL CMapBilinear::GetTransform(double srcQuad[4][2], double dstQuad[4][2])
{
    copyQuad(dstQuad, m_dstQuad);
    return FALSE;
}

float CMapBilinear::GetMap(int x, int y)
{
    if (!inQuad(x, y, m_dstQuad))
        return -1;
    return (float)(m_bXMap ?
        m_B[0][0]*y*x + m_B[0][1]*x +  m_B[0][2]*y + m_B[0][3] :
        m_B[1][0]*y*x + m_B[1][1]*x +  m_B[1][2]*y + m_B[1][3]
        );
}

CString CMapBilinear::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapPerspective Class
//////////////////////////////////////////////////////////////////////

CMapPerspective::CMapPerspective() : CMapBilinear()
{
    m_Mode = MOD_PERSPECTIVE;
    m_Name = "Perspective";
}

CMapPerspective::CMapPerspective(const CMapPerspective& data)
{
    *this = data;
}

CMapPerspective::~CMapPerspective()
{
}

CMapPerspective& CMapPerspective::operator =(const CMapPerspective& data)
{
    *((CMapBilinear*)this) = *((CMapBilinear*)(&data));
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            m_P[i][j] = data.m_P[i][j];
    return *this;
}

BOOL CMapPerspective::GetTransform(double srcQuad[4][2], double dstQuad[4][2])
{
    copyQuad(dstQuad, m_dstQuad);
    return FALSE;
}

float CMapPerspective::GetMap(int x, int y)
{
    if (!inQuad(x, y, m_dstQuad))
        return -1;
    return (float)(m_bXMap ?
        (m_P[0][0]*x + m_P[0][1]*y + m_P[0][2])/
        (m_P[2][0]*x + m_P[2][1]*y + m_P[2][2]) :
        (m_P[1][0]*x + m_P[1][1]*y + m_P[1][2])/
        (m_P[2][0]*x + m_P[2][1]*y + m_P[2][2])
        );
}

//////////////////////////////////////////////////////////////////////
// CMapExplode Class
//////////////////////////////////////////////////////////////////////

CMapExplode::CMapExplode()
{
    m_Name = "Explode";
    m_Mode = MOD_EXPLODE;
    char* explodeNames[] = {"Neighbourhood"};
    char* explodeVals[] = {"6"};
    InitText(explodeNames,explodeVals,ARRAY_LEN(explodeNames));

}

CMapExplode::CMapExplode(const CMapExplode& data)
{
    *this = data;
}

CMapExplode::~CMapExplode()
{
}

CMapExplode& CMapExplode::operator =(const CMapExplode& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    return *this;
}

float CMapExplode::GetMap(int x, int y)
{
   int rx = m_srcImage->GetActualRoi().x;
   int ry = m_srcImage->GetActualRoi().y;
   int width  = m_mapImage->Width();
   int height = m_mapImage->Height();
    int ts = atoi(m_Text[m_EditStart]);
    if (m_bXMap) {
        int xl = x - ts; if (xl < 0) xl = 0;
        int xr = x + ts; if (xr >= width) xr = width-1;
        return (float)(rx + Rand(xl, xr));
    } else {
        int yt = y - ts; if (yt < 0) yt = 0;
        int yb = y + ts; if (yb >= height) yb = height-1;
        return (float)(ry + Rand(yt, yb));
    }
}

CString CMapExplode::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapFaceted Class
//////////////////////////////////////////////////////////////////////

CMapFaceted::CMapFaceted()
{
    m_Name = "Faceted Glass";
    m_Mode = MOD_FACETED;
    char* facetRadio[] = {"RandomAngle","90","180","270"};
    char* facetNames[] = {"Facet size"};
    char* facetVals[] = {"8"};
    InitRadio(facetRadio,ARRAY_LEN(facetRadio),1);
    InitText(facetNames,facetVals,ARRAY_LEN(facetNames),3);
}

CMapFaceted::CMapFaceted(const CMapFaceted& data)
{
    *this = data;
}

CMapFaceted::~CMapFaceted()
{
}

CMapFaceted& CMapFaceted::operator =(const CMapFaceted& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    return *this;
}

BOOL CMapFaceted::SetData(CImage* img, BOOL bY)
{
    IppiRect srcRoi = m_srcImage->GetActualRoi();
    m_mapImage = img;
    BOOL bX = !bY;
    int width  = m_mapImage->Width();
    int height = m_mapImage->Height();
    float* dst = (float*)img->GetRoiPtr();
    int    step = img->Step() >> 2;

    int fs = atoi(m_Text[m_EditStart]);
    int ft = m_Radio;
    int xd = (width / fs) * fs;
    int yd = (height / fs) * fs;
    int x, y, k;
    for (y = 0; y < yd; y += fs) {
        for (x = 0; x < xd; x += fs) {
            int r = (ft == 0) ? Rand(0, 3) : ft;
            for (int i = 0; i < fs; i ++) {
                for (int j = 0; j < fs; j ++) {
                    k = (i + y) * step + j + x;
                    switch (r) {
                    case 0:
                        dst[k] = bX ? (float)(x + j + srcRoi.x) :
                                      (float)(y + i + srcRoi.y);
                        break;
                    case 1:
                        dst[k] = bX ? (float)(x + fs - 1 - i + srcRoi.x) :
                                      (float)(y + j + srcRoi.y);
                        break;
                    case 2:
                        dst[k] = bX ? (float)(x + fs - 1 - j + srcRoi.x) :
                                      (float)(y + fs - 1 - i + srcRoi.y);
                        break;
                    default:
                        dst[k] = bX ? (float)(x + i + srcRoi.x) :
                                      (float)(y + fs - 1 - j + srcRoi.y);
                    }
                }
            }
        }
    }
    for (y = yd; y < height; y ++) {
        for (x = 0; x < width; x ++) {
            k = y * step + x;
            dst[k] = bX ? (float)(x + srcRoi.x) :
                          (float)(y + srcRoi.y);
        }
    }
    for (y = 0; y < yd; y ++) {
        for (x = xd; x < width; x ++) {
            k = y * step + x;
            dst[k] = bX ? (float)(x + srcRoi.x) :
                          (float)(y + srcRoi.y);
        }
    }
    return TRUE;
}

CString CMapFaceted::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapEye Class
//////////////////////////////////////////////////////////////////////

CMapEye::CMapEye()
{
    m_Name = "Fish Eye";
    m_Mode = MOD_EYE;
    char* eyeNames[] = {"Distortion (0 - 1)"};
    char* eyeVals[] = {"1"};
    InitText(eyeNames,eyeVals,ARRAY_LEN(eyeNames));

}

CMapEye::CMapEye(const CMapEye& data)
{
    *this = data;
}

CMapEye::~CMapEye()
{
}

CMapEye& CMapEye::operator =(const CMapEye& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    return *this;
}

float CMapEye::GetMap(int x, int y)
{
   int rx = m_srcImage->GetActualRoi().x;
   int ry = m_srcImage->GetActualRoi().y;
   int width  = m_mapImage->Width();
   int height = m_mapImage->Height();

    int distore = atoi(m_Text[m_EditStart]);
    if (distore < 0) distore = 0;
    if (distore > 1) distore = 1;

    double  sx, sy, aw, d, a, sr, sw, sh, ra, rb;

    ra = width * 0.5;
    rb = height * 0.5;
    sw = __min(ra, rb);
    sh = sw * distore;
    sx = ra;
    sy = rb;
    if (sh == 0)
        sr = aw = 0.0;
    else {
        sr = (sh * sh + sw * sw) / sh / 2;
        aw = 1.0 / asin((double)sw / (double)sr);
    }
    d = sqrt((x - sx) * (x - sx) + (y - sy) * (y - sy));
    if (d < sw) {
        if ( d > 0) {
            a = asin(d / (double)sr) * aw * sw / d;
            if (m_bXMap)
                return (float)(rx + sx + (x - sx) * a);
            else
                return (float)(ry + sy + (y - sy) * a);
        } else {
            if (m_bXMap)
                return (float)(rx + sx);
            else
                return (float)(ry + sy);
        }
    } else {
        if (m_bXMap)
            return (float)(rx + x);
        else
            return (float)(ry + y);
    }
}

CString CMapEye::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapWirl Class
//////////////////////////////////////////////////////////////////////

CMapWirl::CMapWirl()
{
    m_Name = "Wirlpool";
    m_Mode = MOD_WHIRL;
    char* whirlNames[] = {"Angle delta", "Width delta (0-10)"};
    char* whirlVals[] = {"0.5","0"};
    InitText(whirlNames,whirlVals,ARRAY_LEN(whirlNames));

}

CMapWirl::CMapWirl(const CMapWirl& data)
{
    *this = data;
}

CMapWirl::~CMapWirl()
{
}

CMapWirl& CMapWirl::operator =(const CMapWirl& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    return *this;
}

float CMapWirl::GetMap(int x, int y)
{
   int rx = m_srcImage->GetActualRoi().x;
   int ry = m_srcImage->GetActualRoi().y;
   int width  = m_mapImage->Width();
   int height = m_mapImage->Height();
    double angle = atof(m_Text[m_EditStart]);
    angle = angle / 180.0 * 3.14159265358979324;
    double ws = atof(m_Text[m_EditStart+1]);
    if (ws < 0) ws = 0;
    if (ws > 10) ws = 10;

    double  ra, rb, d, dx, dy;
    double  a, sina, cosa;

    ra = width * 0.5;
    rb = height * 0.5;
    dx = x - ra;
    dy = y - rb;
    d = sqrt(dx * dx + dy * dy);
    a = d * angle;
    dx /= (1 + a * ws);
    dy /= (1 + a * ws);
    cosa = cos(a);
    sina = sin(a);
    if (m_bXMap)
        return (float)(rx + dx * cosa - dy * sina + ra);
    else
        return (float)(ry + dx * sina + dy * cosa + rb);
}

CString CMapWirl::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapMosaic Class
//////////////////////////////////////////////////////////////////////

CMapMosaic::CMapMosaic()
{
    m_Name = "Mosaic";
    m_Mode = MOD_MOSAIC;
    char* mosaicRadio[] = {"Rectangle pieces","Quadrangle pieces"};
    char* mosaicNames[] = {"Piece width", "Piece Height",
                           "Gap between pieces", "Angle"};
    char* mosaicVals[] = {"10","10","4","45",};
    InitRadio(mosaicRadio,ARRAY_LEN(mosaicRadio));
    InitText(mosaicNames,mosaicVals,ARRAY_LEN(mosaicNames),2);
}

CMapMosaic::CMapMosaic(const CMapMosaic& data)
{
    *this = data;
}

CMapMosaic::~CMapMosaic()
{
}

CMapMosaic& CMapMosaic::operator =(const CMapMosaic& data)
{
    *((CMapping*)this) = *((CMapping*)(&data));
    return *this;
}

BOOL CMapMosaic::InitMap()
{
    return FALSE;
}

float CMapMosaic::GetMap(int x, int y)
{
    return 0;
}

CString CMapMosaic::GetInfo()
{
    CString info;
    return info;
}

//////////////////////////////////////////////////////////////////////
// CMapPtrArray Class
//////////////////////////////////////////////////////////////////////

CMapPtrArray::CMapPtrArray()
{
}

CMapPtrArray::~CMapPtrArray()
{
    RemoveAll();
}

void CMapPtrArray::RemoveAll()
{
    for (int i=0; i<GetSize(); i++) {
        delete GetAt(i);
    }
    CArray<CMapping*,CMapping*>::RemoveAll();
}

#define ADD_MAP(className) { \
    CMapping* pMap = (CMapping*)(new className); \
    Add(pMap); \
}

#define COPY_MAP(className,srcMap) { \
    CMapping* pMap = (CMapping*)(new className); \
    *((className*)pMap) = *((className*)srcMap); \
    Add(pMap); \
}

void CMapPtrArray::Create()
{
    RemoveAll();
    ADD_MAP(CMapPerspective);
    ADD_MAP(CMapBilinear);
    ADD_MAP(CMapExplode);
    ADD_MAP(CMapFaceted);
    ADD_MAP(CMapEye);
    ADD_MAP(CMapWirl);
    ADD_MAP(CMapMosaic);
}

CMapPtrArray& CMapPtrArray::operator =(const CMapPtrArray& array)
{
    RemoveAll();
    COPY_MAP(CMapPerspective, array[0]);
    COPY_MAP(CMapBilinear, array[1]);
    COPY_MAP(CMapExplode, array[2]);
    COPY_MAP(CMapFaceted, array[3]);
    COPY_MAP(CMapEye, array[4]);
    COPY_MAP(CMapWirl, array[5]);
    COPY_MAP(CMapMosaic, array[6]);
    return *this;
}

//
//BOOL CMapAffine::InitMap(IppiRect roi, double quad[4][2])
//{
//    IppStatus result = stsNoFunction; //!!CALL(ippiGetAffineTransform)(roi, m_A, quad);
//    if (result) {
//        return FALSE;
//    }
//    return TRUE;
//}
//
//float CMapAffine::GetMapA(int x, int y)
//{
//    if (m_bXMap)
//        return (float)(
//            (m_A[1][1]*x - m_A[0][1]*y - m_A[0][2]*m_A[1][1] + m_A[0][1]*m_A[1][2]) /
//            (m_A[0][0]*m_A[1][1] - m_A[0][1]*m_A[1][0])
//            );
//    else
//        return (float)(
//            (m_A[1][0]*x - m_A[0][0]*y - m_A[0][2]*m_A[1][0] + m_A[0][0]*m_A[1][2]) /
//            (m_A[0][1]*m_A[1][0] - m_A[0][0]*m_A[1][1])
//            );
//}

//float CMapAffine::GetMap(int x, int y)
//{
//    if (m_bXMap)
//        return (float)(m_A[0][0]*x + m_A[0][1]*y + m_A[0][2]);
//    else
//        return (float)(m_A[1][0]*x + m_A[1][1]*y + m_A[1][2]);
//}
