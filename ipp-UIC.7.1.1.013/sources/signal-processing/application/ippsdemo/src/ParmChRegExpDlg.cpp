
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChRegExpDlg dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmChRegExpDlg.h"
#include "RunChRegExp.h"
#include ".\parmchregexpdlg.h"

static char* patternArray[] = {
   ".  Any single character",
   "[]   Any one character in the set",
   "[-] Any one character in the range",
   "[^] Any one character not in the set",
   "[^-] Any one character not in the range",
   NULL,
   "[:alpha:]  A letter",
   "[:upper:]  An upper-case letter",
   "[:lower:]  A lower-case letter",
   "[:digit:]  A decimal digit",
   "[:xdigit:] A hexadecimal digit",
   "[:alnum:]  A letter or a digit",
   "[:punct:]  A punctuation character",
   "[:blank:]  A space or tab character",
   "[:space:]  A character producing white space",
   "[:graph:]  A character with a visible representation",
   "[:cntrl:]  A control character",
   NULL,
   "",
   "^   Beginning of line",
   "$   End of line",
   "\\< Beginning of word",
   "\\> End of word",
   "\\   Escape special character",
   "",
   "?  Zero or one matches of preceding item",
   "*  Zero or more matches of preceding item",
   "+  One or more matches of preceding item",
   "{}  Number of matches",
   "{,} Range of matches",
   "",
   "|    Or",
};

enum {ID_REGEXP_00 = 50000,
      ID_REGEXP_01,
      ID_REGEXP_02,
      ID_REGEXP_03,
      ID_REGEXP_04,
      ID_REGEXP_05,
      ID_REGEXP_06,
      ID_REGEXP_07,
      ID_REGEXP_08,
      ID_REGEXP_09,
      ID_REGEXP_10,
      ID_REGEXP_11,
      ID_REGEXP_12,
      ID_REGEXP_13,
      ID_REGEXP_14,
      ID_REGEXP_15,
      ID_REGEXP_16,
      ID_REGEXP_17,
      ID_REGEXP_18,
      ID_REGEXP_19,
      ID_REGEXP_20,
      ID_REGEXP_21,
      ID_REGEXP_22,
      ID_REGEXP_23,
      ID_REGEXP_24,
      ID_REGEXP_25,
      ID_REGEXP_26,
      ID_REGEXP_27,
      ID_REGEXP_28,
      ID_REGEXP_29,
      ID_REGEXP_30,
      ID_REGEXP_31,
      ID_REGEXP_32,
      ID_REGEXP_33,
      ID_REGEXP_34,
      ID_REGEXP_35,
      ID_REGEXP_36,
      ID_REGEXP_37,
      ID_REGEXP_38,
      ID_REGEXP_39,
};

IMPLEMENT_DYNAMIC(CParmChRegExpDlg, CParamDlg)
CParmChRegExpDlg::CParmChRegExpDlg(CRunChRegExp* pRun, UINT nID)
   : CParamDlg(nID)
   , m_pRun(pRun)
   , m_pPattern(_T(""))
   , m_numFindIn(0)
   , m_numFind(0)
   , m_FindTitle(_T(""))
   , m_FindString(_T(""))
   , m_FindNumStr(_T(""))
{
}

CParmChRegExpDlg::~CParmChRegExpDlg()
{
}

void CParmChRegExpDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_PATTERN, m_pPattern);
   DDX_Text(pDX, IDC_LIMIT_EDIT, m_numFindIn);
   DDV_MinMaxInt(pDX, m_numFindIn, 1, INT_MAX);
   DDX_Control(pDX, IDC_LIMIT_SPIN, m_LimitSpin);
   DDX_Control(pDX, IDC_FIND_STATIC, m_FindNumStatic);
   DDX_Text(pDX, IDC_FIND_NUM, m_FindNumStr);
   DDX_Control(pDX, IDC_FIND_NUM, m_FindNumEdit);
   DDX_Control(pDX, IDC_FIND_TITLE, m_FindTitleEdit);
   DDX_Text(pDX, IDC_FIND_TITLE, m_FindTitle);
   DDX_Control(pDX, IDC_FIND_EDIT, m_FindEdit);
   DDX_Text(pDX, IDC_FIND_EDIT, m_FindString);
   DDX_Check(pDX, IDC_OPTION_0, m_Option[0]);
   DDX_Check(pDX, IDC_OPTION_1, m_Option[1]);
   DDX_Check(pDX, IDC_OPTION_2, m_Option[2]);
   DDX_Check(pDX, IDC_OPTION_3, m_Option[3]);
   DDX_Check(pDX, IDC_OPTION_4, m_Option[4]);
   DDX_Control(pDX, IDC_PATTERN_BUTTON, m_PatternButton);
   DDX_Control(pDX, IDC_PATTERN, m_PatternEdit);
}


BEGIN_MESSAGE_MAP(CParmChRegExpDlg, CParamDlg)
   ON_BN_CLICKED(IDC_FIND_BUTTON, OnBnClickedFindButton)
   ON_EN_CHANGE(IDC_PATTERN, OnChange)
   ON_EN_CHANGE(IDC_LIMIT_EDIT, OnChange)
   ON_BN_CLICKED(IDC_OPTION_0, OnChange)
   ON_BN_CLICKED(IDC_OPTION_1, OnChange)
   ON_BN_CLICKED(IDC_OPTION_2, OnChange)
   ON_BN_CLICKED(IDC_OPTION_3, OnChange)
   ON_BN_CLICKED(IDC_OPTION_4, OnChange)
   ON_BN_CLICKED(IDC_PATTERN_BUTTON, OnBnClickedPatternButton)
   ON_COMMAND(ID_REGEXP_00, OnPattern00 )
   ON_COMMAND(ID_REGEXP_01, OnPattern01 )
   ON_COMMAND(ID_REGEXP_02, OnPattern02 )
   ON_COMMAND(ID_REGEXP_03, OnPattern03 )
   ON_COMMAND(ID_REGEXP_04, OnPattern04 )
   ON_COMMAND(ID_REGEXP_05, OnPattern05 )
   ON_COMMAND(ID_REGEXP_06, OnPattern06 )
   ON_COMMAND(ID_REGEXP_07, OnPattern07 )
   ON_COMMAND(ID_REGEXP_08, OnPattern08 )
   ON_COMMAND(ID_REGEXP_09, OnPattern09 )
   ON_COMMAND(ID_REGEXP_10, OnPattern10 )
   ON_COMMAND(ID_REGEXP_11, OnPattern11 )
   ON_COMMAND(ID_REGEXP_12, OnPattern12 )
   ON_COMMAND(ID_REGEXP_13, OnPattern13 )
   ON_COMMAND(ID_REGEXP_14, OnPattern14 )
   ON_COMMAND(ID_REGEXP_15, OnPattern15 )
   ON_COMMAND(ID_REGEXP_16, OnPattern16 )
   ON_COMMAND(ID_REGEXP_17, OnPattern17 )
   ON_COMMAND(ID_REGEXP_18, OnPattern18 )
   ON_COMMAND(ID_REGEXP_19, OnPattern19 )
   ON_COMMAND(ID_REGEXP_20, OnPattern20 )
   ON_COMMAND(ID_REGEXP_21, OnPattern21 )
   ON_COMMAND(ID_REGEXP_22, OnPattern22 )
   ON_COMMAND(ID_REGEXP_23, OnPattern23 )
   ON_COMMAND(ID_REGEXP_24, OnPattern24 )
   ON_COMMAND(ID_REGEXP_25, OnPattern25 )
   ON_COMMAND(ID_REGEXP_26, OnPattern26 )
   ON_COMMAND(ID_REGEXP_27, OnPattern27 )
   ON_COMMAND(ID_REGEXP_28, OnPattern28 )
   ON_COMMAND(ID_REGEXP_29, OnPattern29 )
   ON_COMMAND(ID_REGEXP_30, OnPattern30 )
   ON_COMMAND(ID_REGEXP_31, OnPattern31 )
   ON_COMMAND(ID_REGEXP_32, OnPattern32 )
   ON_COMMAND(ID_REGEXP_33, OnPattern33 )
   ON_COMMAND(ID_REGEXP_34, OnPattern34 )
   ON_COMMAND(ID_REGEXP_35, OnPattern35 )
   ON_COMMAND(ID_REGEXP_36, OnPattern36 )
   ON_COMMAND(ID_REGEXP_37, OnPattern37 )
   ON_COMMAND(ID_REGEXP_38, OnPattern38 )
   ON_COMMAND(ID_REGEXP_39, OnPattern39 )
END_MESSAGE_MAP()

void CParmChRegExpDlg::UpdateMyData(BOOL bSave)
{
   if (bSave) {
      UpdateData();
      UpdateOptions();
   } else {
      UpdateOptions(FALSE);
      SetFindString();
      UpdateData(FALSE);
   }
}

void CParmChRegExpDlg::UpdateOptions(BOOL bSave)
{
   CMyString OptStr = "ismxg";
   if (bSave) {
      m_pOptions = "";
      for (int i=0; i<OPT_NUM; i++) {
         if(m_Option[i])
            m_pOptions += OptStr[i];
      }
   } else {
      for (int i=0; i<OPT_NUM; i++) {
         m_Option[i] = m_pOptions.Found(OptStr[i]);
      }
   }
}

void CParmChRegExpDlg::SetFindString()
{
   m_FindString = "";
   Ipp8u* pStart = (Ipp8u*)m_pDocSrc->GetVector()->GetDataVoi();
   for (int i=0; i<m_numFind; i++) {
      CString str;
      str.Format("%d\t%d\r\n",
                  (Ipp8u*)(m_pFind[i].pFind) - pStart,
                  m_pFind[i].lenFind);
      m_FindString += str;
   }
   m_FindNumStr.Format("%d", m_numFind);
}

void CParmChRegExpDlg::ShowFindResult(BOOL result)
{
}

// CParmChRegExpDlg message handlers

BOOL CParmChRegExpDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_LimitSpin.SetRange(1,SHRT_MAX);
   int tab = 40;
   m_FindTitleEdit.SetTabStops(1,&tab);
   m_FindEdit.SetTabStops(1,&tab);
   m_FindTitle = "posFind\tlenFind";

   UpdateMyData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
}

void CParmChRegExpDlg::OnBnClickedFindButton()
{
   UpdateMyData();
   BOOL result = m_pRun->SetValues(this);
   ShowFindResult(result);
   UpdateMyData(FALSE);
}

void CParmChRegExpDlg::OnChange()
{
   if (m_bInit) {
      ShowFindResult(TRUE);
      m_FindNumEdit.SetWindowText("");
      m_FindEdit.SetWindowText("");
   }
}


void CParmChRegExpDlg::OnBnClickedPatternButton()
{
   CRect rect;
   m_PatternButton.GetWindowRect(&rect);
   CPoint point(rect.right, rect.top);
   CMenu menu;
   CMenu subMenu;
   menu.CreatePopupMenu();
   subMenu.CreatePopupMenu();
   bool bSubMenu = false;
   for (int i=0; i<sizeof(patternArray)/sizeof(char*); i++) {
      UINT id = ID_REGEXP_00 + i;
      if (patternArray[i] == NULL) {
         if (bSubMenu) {
            bSubMenu = false;
         } else {
            bSubMenu = true;
            menu.AppendMenu(MF_POPUP,  (UINT)subMenu.GetSafeHmenu(), "More...");
         }
      } else if (bSubMenu) {
         CString string = patternArray[i];
         UINT flags = string.IsEmpty() ? MF_SEPARATOR : MF_STRING;
         subMenu.AppendMenu(flags, id, string);
      } else {
         CString string = patternArray[i];
         UINT flags = string.IsEmpty() ? MF_SEPARATOR : MF_STRING;
         menu.AppendMenu(flags, id, string);
      }
   }
   menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,
      point.x, point.y, this);
   menu.DestroyMenu();
   subMenu.DestroyMenu();
}

void CParmChRegExpDlg::OnPatternInsert(int idx)
{
   CString string = patternArray[idx];
   string = string.Mid(0, string.Find(' ')); 
   if (string.IsEmpty())
      return;

   UpdateData();
   int start, end;
   m_PatternEdit.GetSel(start,end);
   m_pPattern = m_pPattern.Mid(0,start) + string + m_pPattern.Mid(end);
   start = start + string.GetLength();
   if ((m_pPattern[start-1] == ']') ||
       (m_pPattern[start-1] == '}'))
       start --;
   if ((m_pPattern[start-1] == ':'))
       start ++;
   if ((m_pPattern[start-1] == ',') ||
       (m_pPattern[start-1] == '-'))
       start --;
   UpdateData(FALSE);
   m_PatternEdit.SetFocus();
   m_PatternEdit.SetSel(start,start);
   

}

void CParmChRegExpDlg::OnUpdatePattern(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(TRUE);
}

void CParmChRegExpDlg::OnPattern00() {  OnPatternInsert(0);}
void CParmChRegExpDlg::OnPattern01() {  OnPatternInsert(1);}
void CParmChRegExpDlg::OnPattern02() {  OnPatternInsert(2);}
void CParmChRegExpDlg::OnPattern03() {  OnPatternInsert(3);}
void CParmChRegExpDlg::OnPattern04() {  OnPatternInsert(4);}
void CParmChRegExpDlg::OnPattern05() {  OnPatternInsert(5);}
void CParmChRegExpDlg::OnPattern06() {  OnPatternInsert(6);}
void CParmChRegExpDlg::OnPattern07() {  OnPatternInsert(7);}
void CParmChRegExpDlg::OnPattern08() {  OnPatternInsert(8);}
void CParmChRegExpDlg::OnPattern09() {  OnPatternInsert(9);}
void CParmChRegExpDlg::OnPattern10() {  OnPatternInsert(10);}
void CParmChRegExpDlg::OnPattern11() {  OnPatternInsert(11);}
void CParmChRegExpDlg::OnPattern12() {  OnPatternInsert(12);}
void CParmChRegExpDlg::OnPattern13() {  OnPatternInsert(13);}
void CParmChRegExpDlg::OnPattern14() {  OnPatternInsert(14);}
void CParmChRegExpDlg::OnPattern15() {  OnPatternInsert(15);}
void CParmChRegExpDlg::OnPattern16() {  OnPatternInsert(16);}
void CParmChRegExpDlg::OnPattern17() {  OnPatternInsert(17);}
void CParmChRegExpDlg::OnPattern18() {  OnPatternInsert(18);}
void CParmChRegExpDlg::OnPattern19() {  OnPatternInsert(19);}
void CParmChRegExpDlg::OnPattern20() {  OnPatternInsert(20);}
void CParmChRegExpDlg::OnPattern21() {  OnPatternInsert(21);}
void CParmChRegExpDlg::OnPattern22() {  OnPatternInsert(22);}
void CParmChRegExpDlg::OnPattern23() {  OnPatternInsert(23);}
void CParmChRegExpDlg::OnPattern24() {  OnPatternInsert(24);}
void CParmChRegExpDlg::OnPattern25() {  OnPatternInsert(25);}
void CParmChRegExpDlg::OnPattern26() {  OnPatternInsert(26);}
void CParmChRegExpDlg::OnPattern27() {  OnPatternInsert(27);}
void CParmChRegExpDlg::OnPattern28() {  OnPatternInsert(28);}
void CParmChRegExpDlg::OnPattern29() {  OnPatternInsert(29);}
void CParmChRegExpDlg::OnPattern30() {  OnPatternInsert(30);}
void CParmChRegExpDlg::OnPattern31() {  OnPatternInsert(31);}
void CParmChRegExpDlg::OnPattern32() {  OnPatternInsert(32);}
void CParmChRegExpDlg::OnPattern33() {  OnPatternInsert(33);}
void CParmChRegExpDlg::OnPattern34() {  OnPatternInsert(34);}
void CParmChRegExpDlg::OnPattern35() {  OnPatternInsert(35);}
void CParmChRegExpDlg::OnPattern36() {  OnPatternInsert(36);}
void CParmChRegExpDlg::OnPattern37() {  OnPatternInsert(37);}
void CParmChRegExpDlg::OnPattern38() {  OnPatternInsert(38);}
void CParmChRegExpDlg::OnPattern39() {  OnPatternInsert(39);}
