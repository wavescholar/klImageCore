/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StoreWav.cpp: implementation of the CStoreWav class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "StoreWav.h"
#include "ippsDemoDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define WAVE_FORMAT_UNSIGN 10
#define WAVE_FORMAT_FLOAT  11 

BOOL CStoreWav::Serialize(CFile* pFile, BOOL isStoring)
{
   ASSERT(m_pDoc);
   m_bSave = isStoring;
   m_FilePath = pFile->GetFilePath();
   CString ext = m_FilePath.GetExt();
   ext.MakeLower();
   if (ext != ".wav") return FALSE;
   pFile->Close();
   if (isStoring) Save();
   else           Load();
   pFile->Open(m_FilePath,CFile::modeRead);
   return TRUE;
}

void CStoreWav::Save()
{
   MMCKINFO  ckInfo;

   m_hFile = mmioOpen(m_FilePath.GetBuffer(1), NULL, 
      MMIO_CREATE | MMIO_READWRITE);
   if (!m_hFile) {
     Error(""); 
     return;
   }
   TypeToFormat(m_pDoc->Type());
   m_Format.nSamplesPerSec = m_pDoc->SamplesPerSecond() ? 
      m_pDoc->SamplesPerSecond() : 22050; 
   m_Format.nAvgBytesPerSec = (m_Format.nChannels * m_Format.nSamplesPerSec
      *m_Format.wBitsPerSample) / 8; 
   m_Format.nBlockAlign = (m_Format.nChannels*m_Format.wBitsPerSample) / 8; 

   int fmtSize = sizeof(PCMWAVEFORMAT);
   int dataSize = m_pDoc->Size();

   ckInfo.ckid = MMIO_CREATERIFF;
   ckInfo.cksize = 20 + fmtSize + dataSize;
   ckInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   mmioCreateChunk(m_hFile, &ckInfo, MMIO_CREATERIFF);

   mmioWrite(m_hFile, "fmt ", 4); 
   mmioWrite(m_hFile, (HPSTR)&fmtSize, 4); 
   mmioWrite(m_hFile, (HPSTR)&m_Format, fmtSize); 
   mmioWrite(m_hFile, "data", 4); 
   mmioWrite(m_hFile, (HPSTR)&dataSize, 4); 
   mmioWrite(m_hFile, (HPSTR)m_pDoc->GetData(), dataSize); 

   Close();
}

void CStoreWav::Load()
{
   MMCKINFO    mmckinfoParent;     // parent chunk information 
   MMCKINFO    mmckinfoSubchunk;   // subchunk information structure 
   DWORD       dwFmtSize;          // size of "FMT" chunk 
   DWORD       dwDataSize;         // size of "DATA" chunk 

   m_hFile = mmioOpen(m_FilePath.GetBuffer(1), NULL, 
      MMIO_READ | MMIO_ALLOCBUF);
   if (!m_hFile) {
      Error(""); 
      return; 
   } 

   // Locate a "RIFF" chunk with a "WAVE" form type to make 
   // sure the file is a waveform-audio file. 
   mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E'); 
   if (mmioDescend(m_hFile, (LPMMCKINFO) &mmckinfoParent, NULL, 
                   MMIO_FINDRIFF)) { 
     Error("This is not a waveform-audio file."); 
     return; 
   } 
   // Find the "FMT" chunk (form type "FMT"); it must be 
   // a subchunk of the "RIFF" chunk. 
   mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
   if (mmioDescend(m_hFile, &mmckinfoSubchunk, &mmckinfoParent, 
     MMIO_FINDCHUNK)) { 
     Error("Waveform-audio file has no \"FMT\" chunk."); 
     return; 
   } 

   // Get the size of the "FMT" chunk. Allocate 
   // and lock memory for it. 
   dwFmtSize = mmckinfoSubchunk.cksize; 
   // Read the "FMT" chunk.
   if( dwFmtSize > sizeof(WAVEFORMATEX) ) {
      Error("Unsupported waveform-audio file.");
      return;
   }
   if (dwFmtSize > sizeof(WAVEFORMATEX) || 
       mmioRead(m_hFile, (HPSTR) &m_Format, dwFmtSize) != (int)dwFmtSize){ 
     Error("Failed to read format chunk."); 
     return; 
   } 

   // Ascend out of the "FMT" subchunk. 
   mmioAscend(m_hFile, &mmckinfoSubchunk, 0); 

   // Find the data subchunk. The current file position should be at 
   // the beginning of the data chunk; however, you should not make 
   // this assumption. Use mmioDescend to locate the data chunk. 
   mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
   if (mmioDescend(m_hFile, &mmckinfoSubchunk, &mmckinfoParent, 
     MMIO_FINDCHUNK)) { 
     Error("Waveform-audio file has no data chunk."); 
     return; 
   } 

   // Get the size of the data subchunk. 
   dwDataSize = mmckinfoSubchunk.cksize; 
   if (dwDataSize == 0L){ 
     Error("The data chunk contains no data."); 
     return; 
   } 

   // Allocate and lock memory for the waveform-audio data. 
   m_pDoc->Init(TypeFromFormat(), LengthFromFormat(dwDataSize));
   m_pDoc->SamplesPerSecond(m_Format.nSamplesPerSec);

   // Read the waveform-audio data subchunk. 
   if(mmioRead(m_hFile, (HPSTR)m_pDoc->GetData(), m_pDoc->Size()) 
      != m_pDoc->Size()) { 
     Error("Failed to read data chunk."); 
     return; 
   } 

   Close(); 
}

void CStoreWav::Error(CString message)
{
   Close();
   CString oper = m_bSave ? "save" : "open";
   AfxMessageBox("Can't " + oper + " file " 
      + m_FilePath + ":\n" + message);
}

void CStoreWav::Error(MMIOINFO& ioInfo)
{
   CString message;
   switch (ioInfo.wErrorRet) {
   case MMIOERR_ACCESSDENIED    : message = "Access is denied"; break;
   case MMIOERR_INVALIDFILE     : message = "Invalid file"; break;
   case MMIOERR_NETWORKERROR    : message = "Network error"; break;
   case MMIOERR_PATHNOTFOUND    : message = "Path is not found"; break;
   case MMIOERR_SHARINGVIOLATION: message = "Sharing violation"; break;
   case MMIOERR_TOOMANYOPENFILES: message = "Too many open files"; break;
   }
   Error(message);   
}

void CStoreWav::Close()
{
   if (m_hFile) {
      mmioClose(m_hFile, 0); 
      m_hFile = NULL;
   }
}

ppType CStoreWav::TypeFromFormat()
{
   int type = m_Format.wBitsPerSample;

   switch (m_Format.wFormatTag) {
   case WAVE_FORMAT_UNSIGN: 
      if (type == 64) type |= PP_SIGN; 
      break;
   case WAVE_FORMAT_FLOAT: 
      if (type < 32) type |= PP_SIGN;
      else           type |= PP_FLOAT; 
      break;
   case WAVE_FORMAT_PCM: 
   default:
      type |= PP_SIGN; 
      break;
   }

   if (m_Format.nChannels == 2)
      type |= PP_CPLX;

   return (ppType)type;
}

void CStoreWav::TypeToFormat(ppType type)
{
   m_Format.wBitsPerSample = type & PP_MASK;
   m_Format.nChannels = (type & PP_CPLX) ? 2 : 1;
   m_Format.wFormatTag = (type & PP_SIGN) ? WAVE_FORMAT_PCM :
                         (type & PP_FLOAT ) ? WAVE_FORMAT_FLOAT :
                                             WAVE_FORMAT_UNSIGN;
}
   

int CStoreWav::LengthFromFormat(DWORD dataSize)
{
   return dataSize / ((m_Format.wBitsPerSample/8) * m_Format.nChannels);
}
 