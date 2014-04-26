/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "jitterbuffer.h"

namespace UMC
{
#define SIZE_OFFSET           300
#define END_BUFFER             -3
#define END_STREAM             -1
#define OFFSET_1    sizeof(Ipp16s)
#define OFFSET_2 (sizeof(Ipp16s)+sizeof(Ipp32s))
#define OFFSET_3 (sizeof(Ipp16s)+sizeof(Ipp32s)+sizeof(Ipp64f))

JPacketMediaData::JPacketMediaData(Ipp32u length) : SpeechData(length)
{
  m_marker = 0;
}

Status JPacketMediaData::SetMarker(Ipp32u marker)
{
  m_marker = marker;
  return UMC::UMC_OK;
}

JitterBuffer::JitterBuffer(Ipp32s memsize)
: pBitStream(NULL),
  pIndex(NULL),
  pPutBit(NULL),
  pGetBit(NULL),
  pMaxBit(NULL),
  pLostFrame(NULL),
  size_buffer(0),
  max_offset(0)
{
#if defined(WIN32) || defined(_WIN32_WCE)
  start.QuadPart = finish.QuadPart = 0;
#else
  start = finish = 0;
#endif
  m_flagInit = false;
  max_offset = SIZE_OFFSET;
  size_buffer = memsize + SIZE_OFFSET;
  m_maxOffset = SIZE_OFFSET;

  flagRelease = false;
  Npacket = Nres = 0;
  Tpacket = Tdelta = Tjbuf = 0.0;
  flagJumpPut = flagJumpGet = false;

  return;
}

JitterBuffer::~JitterBuffer()
{

   if(pBitStream) {
     free(pBitStream);
     pBitStream = NULL;
     pPutBit = NULL;
     pGetBit = NULL;
     pMaxBit = NULL;
   }
   if(pIndex) {
     free(pIndex);
     pIndex = NULL;
     numIndex = 0;
   }

   return;
}

void JitterBuffer::SetJBuffer(void)
{
   flagRelease = false;
   Nres = Npacket;
#if defined(WIN32) || defined(_WIN32_WCE)
   QueryPerformanceCounter((LARGE_INTEGER*)&start);
#else
   struct timeval tv;
   gettimeofday(&tv,NULL);
   start = (tv.tv_sec*(long)1000000) + tv.tv_usec;
#endif
   Tjbuf = (Tpacket * (Ipp64f)Nres) - (Tdelta * (Ipp64f)Nres);
}

Ipp64f JitterBuffer::SetTime(void)
{
   Ipp64f t_used;

#if defined(WIN32) || defined(_WIN32_WCE)
   QueryPerformanceCounter((LARGE_INTEGER*)&finish);
   t_used = (Ipp64f)(finish.QuadPart - start.QuadPart) / (Ipp64f)freq.QuadPart;
#else
   struct timeval tv;
   gettimeofday(&tv,NULL);
   finish = (tv.tv_sec*(long)1000000) + tv.tv_usec;
   t_used = (Ipp64f)(finish - start)/(Ipp64f)1000000.0;
#endif

   return t_used;
}

Status JitterBuffer::Init(Ipp32s msSizePacket, Ipp32s numSizeJitter, Ipp32s numThresholdDiscard)
{

   if(pBitStream) { free(pBitStream); pBitStream = NULL; }
   pBitStream = (Ipp8s *)malloc(size_buffer);
   if(pBitStream == NULL) return UMC_ERR_ALLOC;

   pPutBit = pGetBit = pBitStream;
   memset(pPutBit, 0, size_buffer);
   pMaxBit = pBitStream + size_buffer;

   if(pIndex) { free(pIndex); pIndex = NULL; }
   pIndex = (Ipp32u *)malloc(numThresholdDiscard*2*sizeof(Ipp32s));
   if(pIndex == NULL) {
     free(pBitStream);
     pBitStream = NULL;
     pPutBit = NULL;
     pGetBit = NULL;
     pMaxBit = NULL;
     return UMC_ERR_ALLOC;
   }
   numIndex = 0;


#if defined(WIN32) || defined(_WIN32_WCE)
   freq_status = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
   if((freq.QuadPart <= 0) || !freq_status) return UMC_ERR_FAILED;
#endif

   Tpacket = (Ipp64f)(msSizePacket)/1000.0;
   Tdelta = 0.0;
   /*Tdelta = (Tpacket/100.0) * 20.0;//2.0;*/
   Npacket = numSizeJitter;
   m_numDiscard = numThresholdDiscard;
   m_maxOffset = numThresholdDiscard * 4;

   m_countGetPacket = 0;
   m_countLost = m_countReceive = m_countDiscard = 0;
   flagLost = flagReceive = flagDiscard = false;
   m_prevPutMarker = 0;

   SetJBuffer();

   flagJumpPut = flagJumpGet = false;
   m_flagInit = true;

   return UMC_OK;

}

Ipp32s JitterBuffer::SetLength(Ipp16s len, Ipp8s *pBuffer)
{
  Ipp8u len_l, len_h;

  if(pBuffer == NULL) return -1;
  len_l = (Ipp8u)(len & 0xff);
  len_h = (Ipp8u)((len>>8) & 0xff);
  *(pBuffer)   = (Ipp8s)len_l;
  *(pBuffer+1) = (Ipp8s)len_h;

  return 0;
}

Ipp16s JitterBuffer::GetLength(Ipp8s *pBuffer)
{
  Ipp16s len;
  Ipp8u len_l, len_h;

  if(pBuffer == NULL) return -3;
  len_l = (Ipp8u)pBuffer[0];
  len_h = (Ipp8u)pBuffer[1];
  len = (Ipp16s)(((len_h << 8) & 0xff00) | (len_l & 0xff));

  return len;
}


Ipp32s JitterBuffer::SetTimeStamp(Ipp64f time, Ipp8s *pBuffer)
{
  Ipp64f buf_time[2];

  if(pBuffer == NULL) return -1;
  buf_time[0] = time;
  memcpy(pBuffer, buf_time, sizeof(Ipp64f));

  return 0;
}

Ipp64f JitterBuffer::GetTimeStamp(Ipp8s *pBuffer)
{
  Ipp64f buf_time[2];

  if(pBuffer == NULL) return 0.0;
  memcpy(buf_time, pBuffer, sizeof(Ipp64f));

  return buf_time[0];
}

Ipp32s JitterBuffer::SetMarker(Ipp32u mark, Ipp8s *pBuffer)
{
  Ipp8u mark_1, mark_2, mark_3, mark_4;

  if(pBuffer == NULL) return -1;
  mark_1 = (Ipp8u)(mark & 0xff);
  mark_2 = (Ipp8u)((mark>>8) & 0xff);
  mark_3 = (Ipp8u)((mark>>16) & 0xff);
  mark_4 = (Ipp8u)((mark>>24) & 0xff);
  *(pBuffer)   = (Ipp8s)mark_1;
  *(pBuffer+1) = (Ipp8s)mark_2;
  *(pBuffer+2) = (Ipp8s)mark_3;
  *(pBuffer+3) = (Ipp8s)mark_4;

  return 0;
}

Ipp32u JitterBuffer::GetMarker(Ipp8s *pBuffer)
{
  Ipp32u mark;
  Ipp8u mark_1, mark_2, mark_3, mark_4;

  if(pBuffer == NULL) return 0;
  mark_1 = (Ipp8u)pBuffer[0];
  mark_2 = (Ipp8u)pBuffer[1];
  mark_3 = (Ipp8u)pBuffer[2];
  mark_4 = (Ipp8u)pBuffer[3];
  mark = (Ipp32u)(((mark_4 << 24) & 0xff000000) | ((mark_3 << 16) & 0xff0000) | ((mark_2 << 8) & 0xff00) | (mark_1 & 0xff));

  return mark;
}

bool JitterBuffer::CheckGetPointer(Ipp32u mark, Ipp32s offset)
{
    Ipp32s i, idx =-1;

    /*???if(numIndex == 0) return false;*/
    if((offset < m_maxOffset) && (numIndex == 0)) return false;

    for(i=0;i<numIndex;i++) {
      if(mark == pIndex[i]) { idx = i; break; }
    }
    if(idx != -1) {
     for(i=idx;i<(numIndex-1);i++) pIndex[i] = pIndex[i+1];
     numIndex--;
      return true;
   } else {
     if(mark < m_countGetPacket) return true;
     else return false;
   }
}

bool JitterBuffer::CheckExistPacket(Ipp32u mark)
{
    Ipp32u ret_count;
    Ipp8s *pCurGetBit;
    Ipp16s len;
    bool flag;
    Ipp64f time;

    pCurGetBit = pGetBit;

    flag = false;
    while(pCurGetBit < pPutBit) {
       len = (Ipp16s)GetLength((Ipp8s*)(pCurGetBit));
       ret_count = GetMarker((Ipp8s*)(pCurGetBit+OFFSET_1));
       time = GetTimeStamp((Ipp8s*)(pCurGetBit+OFFSET_2)); // SET TIME
       /*** SET TIME
       if(ret_count == mark) { flag = true; break; }
       pCurGetBit = pCurGetBit+OFFSET_2+len;
       ****/
       if(ret_count == mark) { flag = true; break; }
       pCurGetBit = pCurGetBit+OFFSET_3+len;
    }

    return flag;
}

bool JitterBuffer::MoveGetPointer(Ipp8s *ptrCurPut, Ipp32s offset)
{
    Ipp16s len;
    Ipp32u ret_count;
    Ipp64f time;
    bool ret;
    bool ret_fun = false;
    bool flag_order = true;

    if(ptrCurPut == NULL) {
      while(flag_order) {
        len = (Ipp16s)GetLength(pGetBit);
        if(len == END_BUFFER) {
            pGetBit = pBitStream;
            //flagJumpGet = !flagJumpGet;
            if(flagJumpGet) flagJumpGet = false;
            else flagJumpGet = true;
            ret_fun = true;
            break;
        }
        ret_count = GetMarker((Ipp8s*)(pGetBit+OFFSET_1));
        time = GetTimeStamp((Ipp8s*)(pGetBit+OFFSET_2)); // SET TIME
        ret = CheckGetPointer(ret_count, offset);
        /*** SET TIME
        if(ret) pGetBit = pGetBit+OFFSET_2+len;
        else flag_order = false;
        ***/
        if(ret) pGetBit = pGetBit+OFFSET_3+len;
        else flag_order = false;
      }
    } else {
      while(flag_order) {
        if(pGetBit >= ptrCurPut) break;
        len = (Ipp16s)GetLength(pGetBit);
        if(len == END_BUFFER) {
            pGetBit = pBitStream;
            //flagJumpGet = !flagJumpGet;
            if(flagJumpGet) flagJumpGet = false;
            else flagJumpGet = true;
            ret_fun = true;
            break;
        }
        ret_count = GetMarker((Ipp8s*)(pGetBit+OFFSET_1));
        time = GetTimeStamp((Ipp8s*)(pGetBit+OFFSET_2)); // SET TIME
        ret = CheckGetPointer(ret_count, offset);
        /*** SET TIME
        if(ret) pGetBit = pGetBit+OFFSET_2+len;
        else flag_order = false;
        ***/
        if(ret) pGetBit = pGetBit+OFFSET_3+len;
        else flag_order = false;
      }
    }
    return ret_fun;
}

eBlkData JitterBuffer::PutData(Ipp8s *pBuffer, Ipp32s length, Ipp32u count, Ipp64f time)
{
    Ipp16s end = END_BUFFER;

    if(pBuffer == NULL) return DATA_NO;
    if((Ipp32s)(pMaxBit - pPutBit) < max_offset) {
        SetLength(end, pPutBit);
        pPutBit = pBitStream;
        //flagJumpPut = !flagJumpPut;
        if(flagJumpPut) flagJumpPut = false;
        else flagJumpPut = true;
    }
    SetLength((Ipp16s)length, pPutBit);
    SetMarker(count, (pPutBit+OFFSET_1));
    /**** SET TIME
    memcpy((Ipp8s*)(pPutBit+OFFSET_2), (Ipp8s*)pBuffer, length);
    pPutBit = pPutBit + OFFSET_2 + length;
    *****/
    SetTimeStamp(time, (pPutBit+OFFSET_2));
    memcpy((Ipp8s*)(pPutBit+OFFSET_3), (Ipp8s*)pBuffer, length);
    pPutBit = pPutBit + OFFSET_3 + length;


    return DATA_OK;
}

eBlkData JitterBuffer::GetData(Ipp8s **ppBuffer, Ipp32s *length, Ipp64f *time, Ipp32u count)
{
   Ipp8s *pCurPutBit, *pGetTemp;
   Ipp16s len, len_out;
   Ipp32u ret_count;
   Ipp32s i, offset;
   bool flag, flag1, order;
   bool ret;
   Ipp64f start_time;

Start_Data:

   pCurPutBit = pPutBit;

   if(pGetBit > pCurPutBit) {

Overflow:
     len = (Ipp16s)GetLength(pGetBit);
     if(len == END_BUFFER) {
         pGetBit = pBitStream;
         //flagJumpGet = !flagJumpGet;
         if(flagJumpGet) flagJumpGet = false;
         else flagJumpGet = true;
         goto Start_Data;
     }

     flag = true;
     i = 0;
     offset = 0;
     len_out = 0;
     while(flag) {
       len = (Ipp16s)GetLength((Ipp8s*)(pGetBit+offset));
       if(len == END_BUFFER) break;
       ret_count = GetMarker((Ipp8s*)(pGetBit+offset+OFFSET_1));
       start_time = GetTimeStamp((Ipp8s*)(pGetBit+offset+OFFSET_2)); // SET TIME
       /*** SET TIME
       if(ret_count == count) { len_out = len; flag = false; }
       else { offset = offset+OFFSET_2+len; i++; }
       ***/
       if(ret_count == count) { len_out = len; flag = false; }
       else { offset = offset+OFFSET_3+len; i++; }
     }

     if(!flag) {
       /* Data have been found */
       /*** SET TIME
       *ppBuffer = (Ipp8s*)(pGetBit+offset+OFFSET_2);
       ***/
       *ppBuffer = (Ipp8s*)(pGetBit+offset+OFFSET_3);
       *length = len_out;
       *time = start_time;

       order = false;
       /*** SET TIME
       if(offset == 0) { pGetBit = pGetBit+OFFSET_2+len; order = true; }
       else            { pIndex[numIndex] = count; numIndex++; }
       ***/
       if(offset == 0) { pGetBit = pGetBit+OFFSET_3+len; order = true; }
       else            { pIndex[numIndex] = count; numIndex++; }

       MoveGetPointer(NULL, i);

       /* Get packet */
       return DATA_OK;
     } else {
       ///???ret = MoveGetPointer(NULL, i);
       ret = MoveGetPointer(NULL, m_maxOffset);
       if(ret) goto Start_Data;

       pGetTemp = pBitStream;

       flag1 = true;
       i = 0;
       offset = 0;
       len_out = 0;
       CT_PACKET_JB = 0;

       while(flag1) {
         if((pGetTemp+offset) >= pCurPutBit) break;
         len = (Ipp16s)GetLength((Ipp8s*)(pGetTemp+offset));
         ret_count = GetMarker((Ipp8s*)(pGetTemp+offset+OFFSET_1));
         start_time = GetTimeStamp((Ipp8s*)(pGetTemp+offset+OFFSET_2)); // SET TIME
         /*** SET TIME
         if(ret_count == count) { len_out = len; flag1 = false; }
         else { offset = offset+OFFSET_2+len; i++; }
         ***/
         if(ret_count == count) { len_out = len; flag1 = false; }
         else { offset = offset+OFFSET_3+len; i++; }
         CT_PACKET_JB++;
       }
       if(!flag1) {
         /*** SET TIME
         *ppBuffer = (Ipp8s*)(pGetTemp+offset+OFFSET_2);
         ***/
         *ppBuffer = (Ipp8s*)(pGetTemp+offset+OFFSET_3);
         *length = len_out;
         *time = start_time;
         /* Data have been found */
         pIndex[numIndex] = count; numIndex++;
         /* Get packet */
         return DATA_OK;
       } else {
         /* Data have not been found */

         *ppBuffer = NULL;
         *length = 0;
         FLAG_PACKET_JB = 1;
         return DATA_WAIT;
       }
     }

   } else if(pGetBit < pCurPutBit) {

     flag = true;
     i = 0;
     offset = 0;
     len_out = 0;
     CT_PACKET_JB = 0;

     while(flag) {
       if((pGetBit+offset) >= pCurPutBit) break;
       len = (Ipp16s)GetLength((Ipp8s*)(pGetBit+offset));
       ret_count = GetMarker((Ipp8s*)(pGetBit+offset+OFFSET_1));
       start_time = GetTimeStamp((Ipp8s*)(pGetBit+offset+OFFSET_2)); // SET TIME
       /*** SET TIME
       if(ret_count == count) { len_out = len; flag = false; }
       else { offset = offset+OFFSET_2+len; i++; }
       ***/
       if(ret_count == count) { len_out = len; flag = false; }
       else { offset = offset+OFFSET_3+len; i++; }
       CT_PACKET_JB++;
     }

     if(!flag) {
       /* Data have been found */
       /*** SET TIME
       *ppBuffer = (Ipp8s*)(pGetBit+offset+OFFSET_2);
       ***/
       *ppBuffer = (Ipp8s*)(pGetBit+offset+OFFSET_3);
       *length = len_out;
       *time = start_time;

       order = false;
       /*** SET TIME
       if(offset == 0) { pGetBit = pGetBit+OFFSET_2+len; order = true; }
       else            { pIndex[numIndex] = count; numIndex++; }
       ***/
       if(offset == 0) { pGetBit = pGetBit+OFFSET_3+len; order = true; }
       else            { pIndex[numIndex] = count; numIndex++; }

       MoveGetPointer(pCurPutBit, i);
       /* Get packet */
       return DATA_OK;

     } else {
       /* Data have not been found */

       MoveGetPointer(pCurPutBit, i);

       *ppBuffer = NULL;
       *length = 0;
       if(pGetBit == pCurPutBit) return DATA_NO;
       else {
           FLAG_PACKET_JB = 2;
           return DATA_WAIT;
       }
     }
   } else {
     if(flagJumpGet == flagJumpPut) {
      /* Data have not been found */
      *ppBuffer = NULL;
      *length = 0;
      return DATA_NO;
     } else {
      /* Overflow */
      goto Overflow;
     }
   }

}

Status JitterBuffer::PutPacket(JPacketMediaData *packet)
{
  /*******************/
  Ipp32u count;
  /*******************/
  Ipp8s *pCurGetBit;
  bool exist_flag;
  bool fGet;
  Ipp8s *ptrInBuff;
  Ipp32s in_len;
  Ipp32u in_marker;
  Ipp64f start;

  if(!m_flagInit) return UMC_ERR_NOT_INITIALIZED;
  if(packet == NULL) return UMC_ERR_NULL_PTR;

  ptrInBuff = (Ipp8s*)packet->GetDataPointer();
  in_len = packet->GetDataSize();
  in_marker = packet->GetMarker();
  start = packet->GetTime();

  if(ptrInBuff == NULL) return UMC_ERR_INVALID_PARAMS;
  if(in_len == 0) return UMC_ERR_INVALID_PARAMS;

  /***************** OVERFLOW BUFFER *******************/
  pCurGetBit = pGetBit;
  fGet = flagJumpGet;
  if(((pPutBit+in_len) >= pCurGetBit) && (flagJumpPut != fGet)) {
    /*m_countDiscard++;*/
    if(!flagDiscard) m_countDiscard++;
    else { m_countDiscard = 0; flagDiscard = false; }
    return UMC_OVERFLOW;
  }
  /*****************************************************/
  if(in_marker > (m_prevPutMarker+m_numDiscard)) {
    /*m_countDiscard++;*/
    if(!flagDiscard) m_countDiscard++;
    else { m_countDiscard = 0; flagDiscard = false; }
    return UMC_DISCARDGT;
  }
  exist_flag = CheckExistPacket(in_marker);
  if(exist_flag) {
    /*m_countDiscard++;*/
    if(!flagDiscard) m_countDiscard++;
    else { m_countDiscard = 0; flagDiscard = false; }
    return UMC_DISCARDEQ;
  }

  /********************************/
  count = m_countGetPacket;
  if(in_marker < count) {
      ///???m_countDiscard++;
      if(!flagDiscard) m_countDiscard++;
      else { m_countDiscard = 0; flagDiscard = false; }
      return UMC_DISCARDLT;
  }
  /**********************************/

  PutData(ptrInBuff, in_len, in_marker, start);
  /*m_countReceive++;*/
  if(!flagReceive) m_countReceive++;
  else { m_countReceive = 0; flagReceive = false; }
  m_prevPutMarker = in_marker;
  return UMC_OK;
}

Status JitterBuffer::GetPacket(JPacketMediaData *packet, StatusData *status)
{
   Ipp64f Tget;
   eBlkData data;
   Status ret_status;
   Ipp8s *ptrOutBuff;
   Ipp32s out_len;
   Ipp64f s_time;

    if(!m_flagInit) return UMC_ERR_NOT_INITIALIZED;
    /***** Old version *******
    if(Nres == 0) SetJBuffer();
    **************************/
    if(packet == NULL) {
      data = GetData((Ipp8s**)&ptrOutBuff, (Ipp32s*)&out_len, &s_time, m_countGetPacket);
      if(!flagLost) m_countLost++;
      else { m_countLost = 0; flagLost = false; }
      m_countGetPacket++;
      Nres--;
      *status = SJB_LOST;
      if(data == DATA_OK) ret_status = UMC_OK;
      else if(data == DATA_WAIT) ret_status = UMC_OK;
      else ret_status = UMC_ERR_NOT_ENOUGH_DATA;

      /***** New version *******/
      if(Nres == 0) SetJBuffer();
      /**************************/
      return ret_status;
    }

    Tget = SetTime();

    if(!flagRelease) {
      if(Tget <= Tjbuf) {
        data = GetData((Ipp8s**)&ptrOutBuff, (Ipp32s*)&out_len, &s_time, m_countGetPacket);
        packet->SetBufferPointer((Ipp8u*)ptrOutBuff, out_len);
        packet->SetDataSize(out_len);
        packet->SetTime(s_time);
        if(data == DATA_OK) {
          packet->SetMarker(m_countGetPacket);
          m_countGetPacket++;
          Nres--;
          Tjbuf = Tjbuf + Tdelta;
          *status = SJB_OK;
          ret_status = UMC_OK;
        } else if(data == DATA_WAIT) {
          *status = SJB_WAIT;
          ret_status = UMC_OK;
        } else {
          *status = SJB_WAIT;
          ret_status = UMC_ERR_NOT_ENOUGH_DATA;
        }
      } else {
         flagRelease = true;
        data = GetData((Ipp8s**)&ptrOutBuff, (Ipp32s*)&out_len, &s_time, m_countGetPacket);
        packet->SetBufferPointer((Ipp8u*)ptrOutBuff, out_len);
        packet->SetDataSize(out_len);
        packet->SetTime(s_time);
        if(data == DATA_OK) {
          packet->SetMarker(m_countGetPacket);
          m_countGetPacket++;
          Nres--;
          *status = SJB_OK;
          ret_status = UMC_OK;
        } else if(data == DATA_WAIT) {
          /*m_countLost++;*/
          if(!flagLost) m_countLost++;
          else { m_countLost = 0; flagLost = false; }
          packet->SetMarker(m_countGetPacket);
          m_countGetPacket++;
          Nres--;
          *status = SJB_LOST;
          ret_status = UMC_OK;
        } else {
          /*m_countLost++;*/
          if(!flagLost) m_countLost++;
          else { m_countLost = 0; flagLost = false; }
          packet->SetMarker(m_countGetPacket);
          m_countGetPacket++;
          Nres--;
          *status = SJB_LOST;
          ret_status = UMC_ERR_NOT_ENOUGH_DATA;
        }
      }
    } else {
      data = GetData((Ipp8s**)&ptrOutBuff, (Ipp32s*)&out_len, &s_time, m_countGetPacket);
      packet->SetBufferPointer((Ipp8u*)ptrOutBuff, out_len);
      packet->SetDataSize(out_len);
      packet->SetTime(s_time);
      if(data == DATA_OK) {
        packet->SetMarker(m_countGetPacket);
        m_countGetPacket++;
        Nres--;
        *status = SJB_OK;
        ret_status = UMC_OK;
      } else if(data == DATA_WAIT) {
        /*m_countLost++;*/
        if(!flagLost) m_countLost++;
        else { m_countLost = 0; flagLost = false; }
        packet->SetMarker(m_countGetPacket);
        m_countGetPacket++;
        Nres--;
        *status = SJB_LOST;
        ret_status = UMC_OK;

      } else {
        /*m_countLost++;*/
        if(!flagLost) m_countLost++;
        else { m_countLost = 0; flagLost = false; }
        packet->SetMarker(m_countGetPacket);
        m_countGetPacket++;
        Nres--;
        *status = SJB_LOST;
        ret_status = UMC_ERR_NOT_ENOUGH_DATA;
      }
    }

    /***** New version *******/
    if(Nres == 0) {
      SetJBuffer();
    }
    /*************************/
    return ret_status;
}


Status JitterBuffer::SetJBufferSize(Ipp32s numSizeJitter)
{
  Ipp32s Np;

  if(numSizeJitter < 1) return UMC_ERR_FAILED;
  Np = Npacket - Nres;
  if(numSizeJitter > Np) {
    Npacket = numSizeJitter;
    Tjbuf = (Tpacket * (Ipp64f)Npacket) - (Tdelta * (Ipp64f)Npacket);
  } else {
    Npacket = numSizeJitter;
    Tjbuf = (Tpacket * (Ipp64f)Npacket) - (Tdelta * (Ipp64f)Npacket);
    Nres = 0;
  }

  return UMC_OK;
}

Status JitterBuffer::GetStatistics(JBufferStat *pStat)
{
  if(!m_flagInit) return UMC_ERR_NOT_INITIALIZED;
  if(pStat == NULL) return UMC_ERR_NULL_PTR;
  pStat->nReceivedPacket = m_countReceive;
  pStat->nLostPacket = m_countLost;
  pStat->nDiscardedPacket = m_countDiscard;

  return UMC_OK;
}

Status JitterBuffer::InitStatistics(void)
{
  if(!m_flagInit) return UMC_ERR_NOT_INITIALIZED;
  /*m_countReceive = 0;
  m_countLost = 0;
  m_countDiscard = 0;*/
  flagLost = true;
  flagReceive = true;
  flagDiscard = true;

  return UMC_OK;
}

Status JitterBuffer::CompensateDelay(Ipp32s *numLostFrames)
{
   Ipp32u put_marker, get_marker, delta;
   Ipp32s nPackets = 0;

   put_marker = m_prevPutMarker;
   get_marker = m_countGetPacket;

   if(get_marker > put_marker) {
     delta = get_marker - put_marker;
     if(delta > (Ipp32u)(Npacket*2)) {
       m_countGetPacket = (m_prevPutMarker+1);
       nPackets = delta;
     }
   }
   *numLostFrames = nPackets;

   return UMC_OK;
}



}
