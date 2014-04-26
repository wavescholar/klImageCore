
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChRegExpDlg.h: interface for CParmChRegExpDlg class.
// CParmChRegExpDlg dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined __PARMCHREGEXPDLG_H__
#define __PARMCHREGEXPDLG_H__

#pragma once

#include "ParamDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
class CRunChRegExp;


class CParmChRegExpDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmChRegExpDlg)

public:
   CParmChRegExpDlg(CRunChRegExp* pRun, UINT nID = IDD);
   virtual ~CParmChRegExpDlg();

// Dialog Data
   enum { IDD = IDD_PARMCH_REGEXP };

protected:
   CRunChRegExp* m_pRun;
   void UpdateMyData(BOOL bSave = TRUE);
   void UpdateOptions(BOOL bSave = TRUE);
   void SetFindString();
   void ShowFindResult(BOOL result);

   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   DECLARE_MESSAGE_MAP()
public:
   enum {OPT_NUM = 5};
   CMyString      m_pOptions;
   CMyString      m_pPattern;
   int            m_numFindIn;
   IppRegExpFind* m_pFind;
   int            m_errOffset;
   int            m_numFind;

   CSpinButtonCtrl m_LimitSpin;
   BOOL m_Option[OPT_NUM];
   CStatic m_FindNumStatic;
   CMyString m_FindNumStr;
   CEdit m_FindNumEdit;
   CEdit m_FindTitleEdit;
   CMyString m_FindTitle;
   CEdit m_FindEdit;
   CMyString m_FindString;
   CEdit m_PatternEdit;
   CButton m_PatternButton;

   virtual BOOL OnInitDialog();
   afx_msg void OnBnClickedFindButton();
   afx_msg void OnChange();
   afx_msg void OnBnClickedPatternButton();
   afx_msg void OnUpdatePattern(CCmdUI* pCmdUI);
   afx_msg void OnPattern00();
   afx_msg void OnPattern01();
   afx_msg void OnPattern02();
   afx_msg void OnPattern03();
   afx_msg void OnPattern04();
   afx_msg void OnPattern05();
   afx_msg void OnPattern06();
   afx_msg void OnPattern07();
   afx_msg void OnPattern08();
   afx_msg void OnPattern09();
   afx_msg void OnPattern10();
   afx_msg void OnPattern11();
   afx_msg void OnPattern12();
   afx_msg void OnPattern13();
   afx_msg void OnPattern14();
   afx_msg void OnPattern15();
   afx_msg void OnPattern16();
   afx_msg void OnPattern17();
   afx_msg void OnPattern18();
   afx_msg void OnPattern19();
   afx_msg void OnPattern20();
   afx_msg void OnPattern21();
   afx_msg void OnPattern22();
   afx_msg void OnPattern23();
   afx_msg void OnPattern24();
   afx_msg void OnPattern25();
   afx_msg void OnPattern26();
   afx_msg void OnPattern27();
   afx_msg void OnPattern28();
   afx_msg void OnPattern29();
   afx_msg void OnPattern30();
   afx_msg void OnPattern31();
   afx_msg void OnPattern32();
   afx_msg void OnPattern33();
   afx_msg void OnPattern34();
   afx_msg void OnPattern35();
   afx_msg void OnPattern36();
   afx_msg void OnPattern37();
   afx_msg void OnPattern38();
   afx_msg void OnPattern39();


   void OnPatternInsert(int i);
};

#endif
