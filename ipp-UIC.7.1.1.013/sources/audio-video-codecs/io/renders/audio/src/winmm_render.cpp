/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"

#if defined (UMC_ENABLE_WINMM_AUDIO_RENDER)

#include "winmm_render.h"

using namespace UMC;


void CALLBACK waveOutProc(HWAVEOUT /*m_hWO*/,
                          UINT uMsg,
                          DWORD_PTR dwInstance,
                          DWORD_PTR /*dwParam1*/,
                          Ipp32u) /*dwParam2*/
{
  WinMMAudioRender* pRender = (WinMMAudioRender*) dwInstance;

  switch(uMsg)
  {
   case WOM_OPEN:
       break;
   case WOM_DONE:
       pRender->Release();
       break;
   case WOM_CLOSE:
       break;
   default:
       break;
  }
}

WinMMAudioRender::BufArray::BufArray():
  m_uiBufSize(0),
  m_uiHdrNum(WINMM_BUF_NUM_MAX)
{
  memset(m_pArray, 0, sizeof(WAVEHDR) * m_uiHdrNum);
}

WinMMAudioRender::BufArray::~BufArray()
{
  for (Ipp32u i = 0; i < m_uiHdrNum; i++) {
    if (m_pArray[i].lpData) {
      delete[](m_pArray[i].lpData);
    }
    m_pArray[i].lpData = NULL;
  }
}

Status WinMMAudioRender::BufArray::Init(Ipp32u uiBufSize)
{
  Status umcRes = UMC_OK;
  m_uiBufSize = uiBufSize;

  for (Ipp32u i = 0; UMC_OK == umcRes && i < m_uiHdrNum; i++) {
    if (m_pArray[i].lpData) {
      delete[](m_pArray[i].lpData);
    }
    memset(&m_pArray[i], 0, sizeof(WAVEHDR));
    m_pArray[i].lpData = new char[WINMM_BUF_SIZE];
    if (NULL == m_pArray[i].lpData) {
      umcRes = UMC_ERR_ALLOC;
    }
    m_pArray[i].dwBufferLength = m_uiBufSize;
    memset(m_pArray[i].lpData, 0, m_uiBufSize);
  }
  return umcRes;
}

void WinMMAudioRender::BufArray::ZeroBuffers()
{
  for (Ipp32u i = 0; i < m_uiHdrNum; i++) {
    if (m_pArray[i].lpData) {
      memset(m_pArray[i].lpData, 0, m_uiBufSize);
    }
  }
}

WinMMAudioRender::WinMMAudioRender():
    m_hWO(NULL),
    m_dfStartPTS(-1.0),
    m_bHdrOK(false),
//    m_sm_free_buffers(0),
    m_dfSampleNorm(1.0),
    m_DataSize(0)
{}

Status WinMMAudioRender::Reset()
{
  Status umcRes = UMC_OK;
  MMRESULT mmres;

  if (m_hWO) {
    m_bStop = 1;
    vm_time_sleep(10);
    mmres = waveOutClose(m_hWO);
    if (mmres == WAVERR_STILLPLAYING) {  // need to reset buffers
      mmres = waveOutReset(m_hWO);
      mmres = waveOutClose(m_hWO);
    }
    m_hWO = NULL;
  }
  // all buffers free
  if (UMC_OK == umcRes)
    m_sm_free_buffers.Init(WINMM_BUF_NUM_MAX);
  

  m_Hdrs.ZeroBuffers();

  if (UMC_OK == umcRes &&
      MMSYSERR_NOERROR != waveOutOpen(&m_hWO,
                                      WAVE_MAPPER,
                                      &m_WaveFmtX,
                                      (DWORD_PTR) waveOutProc,
                                      (DWORD_PTR) this,
                                      CALLBACK_FUNCTION)) {
    umcRes = UMC_ERR_FAILED;
  }

  if (UMC_OK == umcRes) {
    Ipp32u wdVolume = m_dwVolume | (m_dwVolume << 16);
    if (MMSYSERR_NOERROR != waveOutSetVolume(m_hWO, wdVolume)) {
      umcRes = UMC_ERR_FAILED;
    }

    m_dfStartPTS = -1.0;
    m_iFreeBuf = WINMM_BUF_NUM_MAX - 1;
    m_iDoneBuf = 0;
    m_iOffset = -1;
  }

  if (UMC_OK == umcRes) {
    m_DataBuffer.Reset();
    //umcRes = BasicAudioRender::Reset();
  }

  m_bStop = 0;
  return umcRes;
}

Status WinMMAudioRender::Close()
{
  MMRESULT mmres;

  BasicAudioRender::Close();
  if (m_hWO) {
    vm_time_sleep(10);
    do 
    mmres = waveOutClose(m_hWO);
    while (mmres == WAVERR_STILLPLAYING);
    if (mmres == WAVERR_STILLPLAYING) {  // need to reset buffers
      mmres = waveOutReset(m_hWO);
      mmres = waveOutClose(m_hWO);
    }
    m_hWO = NULL;
  }
  // free all output buffers
  for( int i = 0; i < WINMM_BUF_NUM_MAX; ++i)
    waveOutUnprepareHeader(m_hWO, &m_Hdrs.m_pArray[i], sizeof(WAVEHDR));
  return UMC_OK;
}

WinMMAudioRender::~WinMMAudioRender()
{
  Close();
}

Status WinMMAudioRender::Init(MediaReceiverParams* pInit)
{
  Status umcRes = UMC_OK;
  AudioRenderParams* pParams = DynamicCast<AudioRenderParams, MediaReceiverParams>(pInit);

  if (NULL == pParams) {
    umcRes = UMC_ERR_NULL_PTR;
  }

  AudioStreamInfo *pASInfo = NULL;

  if (UMC_OK == umcRes) {
    pASInfo = &pParams->m_info;
    Close();
    umcRes = m_Hdrs.Init(WINMM_BUF_SIZE);
  }

  if (UMC_OK == umcRes) {
    // prepare waveout
    m_WaveFmtX.wFormatTag      = (Ipp16u)WAVE_FORMAT_PCM;
    m_WaveFmtX.nChannels       = (Ipp16u)pASInfo->audioInfo.m_iChannels;
    m_WaveFmtX.nSamplesPerSec  = pASInfo->audioInfo.m_iSampleFrequency;
    m_WaveFmtX.wBitsPerSample  = (Ipp16u)pASInfo->audioInfo.m_iBitPerSample;
    m_WaveFmtX.nBlockAlign     = (Ipp16u)((m_WaveFmtX.wBitsPerSample *
                                           m_WaveFmtX.nChannels) >> 3);
    m_WaveFmtX.nAvgBytesPerSec = pASInfo->audioInfo.m_iSampleFrequency * m_WaveFmtX.nBlockAlign;
    m_WaveFmtX.cbSize          = 0;
    m_dfStartPTS               = -1.0;
    m_dwVolume                 = 0xffffffff;
    m_dfSampleNorm             = 1.0 / pASInfo->audioInfo.m_iSampleFrequency;

    umcRes = Reset();
  }

  if (UMC_OK == umcRes) {
    umcRes = BasicAudioRender::Init(pInit);
  }

  return umcRes;
}

void WinMMAudioRender::Release()
{
    m_sm_free_buffers.Signal();
    if (!m_bHdrOK && (WINMM_BUF_NUM_MAX == ++m_iDoneBuf)) {
      m_bHdrOK = true;
      m_iDoneBuf = 0;
    }
}

Status WinMMAudioRender::UnLockInputBuffer(MediaData *in,
                                                     Status StreamStatus)
{
  Status umcRes = BasicAudioRender::UnLockInputBuffer(in,StreamStatus);

  if (UMC_OK == umcRes && m_dfStartPTS == -1.0) { // first frame
    m_dfStartPTS = in->m_fPTSStart;
    if (m_dfStartPTS < 0) {
      m_dfStartPTS = 0.0;
    }
  }

  return umcRes;
}

Status WinMMAudioRender::SendFrame(MediaData* pInData)
{
  Status    umcRes = UMC_OK;
  Ipp32u    uiBufSize = WINMM_BUF_SIZE;
  MMRESULT  res;
  Ipp32u    uiBytes = 0;
  Ipp8u*    pbBuffer = NULL;

  if (NULL == pInData) {
    umcRes = UMC_ERR_NULL_PTR;
  }

  if (UMC_OK == umcRes) {
    uiBytes = (Ipp32u)pInData->GetDataSize();
    pbBuffer = (Ipp8u*)pInData->GetDataPointer();
  }

  while (UMC_OK == umcRes && 0 < uiBytes) {
    if (-1 == m_iOffset) {
      m_sm_free_buffers.Wait();
//      WaitForSingleObject(m_sm_free_buffers, INFINITE);
        if (m_bStop) {
          break;
        }

        if (++m_iFreeBuf >= WINMM_BUF_NUM_MAX) {
          m_iFreeBuf = 0;
        }
        m_iOffset = 0;
    }

//    printf("m_iFreeBuf = %i\n", m_iFreeBuf);
    WAVEHDR *pHdr = &m_Hdrs.m_pArray[m_iFreeBuf];

    if (uiBufSize - m_iOffset > uiBytes) {
      memcpy(pHdr->lpData + m_iOffset, pbBuffer, uiBytes);
      m_iOffset += uiBytes;
      m_DataSize += uiBytes;
      uiBytes = 0;
    } else {
      memcpy(pHdr->lpData + m_iOffset, pbBuffer, uiBufSize - m_iOffset);
      uiBytes -= uiBufSize - m_iOffset;
      pbBuffer += (uiBufSize - m_iOffset);
      m_DataSize += (uiBufSize - m_iOffset);
      m_iOffset = -1;
    }

    if ((-1 == m_iOffset) && (!m_bStop)) {
      res = MMSYSERR_NOERROR;
      if (!m_bHdrOK)
        res = waveOutPrepareHeader(m_hWO, pHdr, sizeof(WAVEHDR));
      if (MMSYSERR_NOERROR == res) {
        res = waveOutWrite(m_hWO, pHdr, sizeof(WAVEHDR));
      }
      if (MMSYSERR_NOERROR != res) {
        umcRes = UMC_ERR_FAILED;
      }
    }
  }
  return umcRes;
}

Status WinMMAudioRender::DrainBuffer()
{
  Status    umcRes = UMC_OK;
  MMRESULT  res;

  WAVEHDR *pHdr = &m_Hdrs.m_pArray[m_iFreeBuf];

  if (m_iOffset != -1) {  // some data is not written
    res = waveOutPrepareHeader(m_hWO, pHdr, sizeof(WAVEHDR));
    if (MMSYSERR_NOERROR == res) {
      res = waveOutWrite(m_hWO, pHdr, sizeof(WAVEHDR));
      if (MMSYSERR_NOERROR != res) {
        umcRes = UMC_ERR_FAILED;
      }
    }
  }

  MMTIME wotime;
  wotime.wType = TIME_BYTES;
  wotime.u.cb = 0;
  /*** wait till the buffer is done ***/
  while (!(pHdr->dwFlags & WHDR_DONE)) {
    vm_time_sleep(100);
  }

  return umcRes;
}


Status WinMMAudioRender::Pause(bool bPause)
{
  MMRESULT mmRes = MMSYSERR_NOERROR;

  if (m_hWO ) {
    if (bPause) {
      mmRes = waveOutPause(m_hWO);
    } else {
      mmRes = waveOutRestart(m_hWO);
    }
  }
  return (MMSYSERR_NOERROR == mmRes) ? UMC_OK : UMC_ERR_FAILED;
}

Ipp64f WinMMAudioRender::GetTimeTick()
{
  if (m_hWO || -1.0 != m_dfStartPTS) {
    MMTIME wotime;
    wotime.wType    = TIME_SAMPLES;
    wotime.u.sample = 0;
    if (MMSYSERR_NOERROR == waveOutGetPosition(m_hWO, &wotime, sizeof(wotime))) {
      if (wotime.wType == TIME_SAMPLES) { // format is supported
        return m_dfStartPTS + wotime.u.sample * m_dfSampleNorm;
      }
    }
  }
  return 0;
}

#endif  // UMC_ENABLE_WINMM_AUDIO_RENDER
