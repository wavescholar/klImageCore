/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterCol.h: interface for the CRunFilterCol class.
// CRunFilterCol class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTERCOL_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_)
#define AFX_RUNFILTERCOL_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFilterRow.h"

class CRunFilterCol : public CRunFilterRow
{
public:
    CRunFilterCol();
    virtual ~CRunFilterCol();
protected:
    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUNFILTERCOL_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_)
