/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __JITTERBUFFER_H__
#define __JITTERBUFFER_H__

#if defined(WIN32) || defined(WIN64) || defined(_WIN32_WCE)
//#include <windows.h>
#else
#include <sys/time.h>
#include <sched.h>
#endif

#include "umc_structures.h"
#include "umc_speech_data.h"
namespace UMC
{

#define UMC_OVERFLOW -799
#define UMC_DISCARDGT -798
#define UMC_DISCARDLT -797
#define UMC_DISCARDEQ -796

typedef enum {
   SJB_OK         = 0,
   SJB_WAIT       = 1,
   SJB_LOST       = 2
} StatusData;

typedef enum {
   DATA_NO   = -1,
   DATA_OK   = 0,
   DATA_WAIT = 2
} eBlkData;

typedef struct {
   Ipp32u nReceivedPacket;
   Ipp32u nLostPacket;
   Ipp32u nDiscardedPacket;
} JBufferStat;


class JPacketMediaData : public UMC::SpeechData
{
    public:
      JPacketMediaData(Ipp32u length = 0);
      UMC::Status SetMarker(Ipp32u marker);
      Ipp32u  GetMarker() { return m_marker;};
   protected:
      Ipp32u m_marker;
};

class JitterBuffer
{
   DYNAMIC_CAST_DECL_BASE(JitterBuffer)
public:
   /*
      JitterBuffer() - constructor
         memsize - size in bytes of the memory storage
   */
   JitterBuffer(Ipp32s memsize);
   ~JitterBuffer();

   /*
      Init()- jitter buffer initialization
      Parameters:
         msSizePacket - packet size in milliseconds
         numSizeJitter - jitter buffer size in number of packets
                     (jitter in ms = numPacket * msSizePacket)
         numThresholdDiscard - it`s range determinative what packet will be discard (in packets)
      Returns:
         JB_OUTOFMEM - out of memory
   */
   Status Init(Ipp32s msSizePacket, Ipp32s numSizeJitter, Ipp32s numThresholdDiscard);
   /*
      PutPacket() - put packet into the jitter buffer
      Parameters:
         packet - pointer of the packet.
      Returns:
         USC_ERR_NULL_PTR - bad pointer
         USC_ERR_NOT_INITIALIZED - uninitialized jitter
         USC_BAD_PACKET - bad packet
   */
   Status PutPacket(JPacketMediaData *packet);
   /*
      GetPacket() - get packet from the jitter buffer
      Parameters:
         packet - pointer of the packet.
         status - data status
            SJB_OK - ready
            SJB_WAIT - not available
            SJB_LOST - lost packet
      Returns:
         USC_ERR_NULL_PTR - bad pointer
         USC_ERR_NOT_INITIALIZED - uninitialized jitter
   */
   Status GetPacket(JPacketMediaData *packet, StatusData *status);
   /*
      GetStatistics() - get statistics about current state the jitter buffer
      Parameters:
         pStat - pointer of the statistics object.
            nReceivedPacket  - number of received packets
            nLostPacket      - number of lost packets
            nDiscardedPacket - number of discarded packets
   */

   Status GetStatistics(JBufferStat *pStat);
   /*
      ClearStatistics() - clear statistics of the jitter buffer
   */
   Status InitStatistics(void);
   /*
      SetJBufferSize() - set size of the jitter buffer in packets
      Parameters:
         numSizeJitter - number packets in the jitter buffer.

   */
   Status SetJBufferSize(Ipp32s numSizeJitter);
   /*
      GetJBufferSize() - get current size of the jitter buffer in packets

   */
   Ipp32s GetJBufferSize(void) { return Npacket; }

   Status CompensateDelay(Ipp32s *numLostFrames);

private:
/*********************** Private functions *****************************/
   void     SetJBuffer(void);
   Ipp64f   SetTime(void);
   Ipp32s   SetLength(Ipp16s len, Ipp8s *pBuffer);
   Ipp16s   GetLength(Ipp8s *pBuffer);
   Ipp32s   SetMarker(Ipp32u mark, Ipp8s *pBuffer);
   Ipp32u GetMarker(Ipp8s *pBuffer);
   Ipp32s   SetTimeStamp(Ipp64f time, Ipp8s *pBuffer);
   Ipp64f   GetTimeStamp(Ipp8s *pBuffer);
   bool     CheckGetPointer(Ipp32u mark, Ipp32s offset);
   bool     CheckExistPacket(Ipp32u mark);
   bool     MoveGetPointer(Ipp8s *ptrCurPut, Ipp32s offset);

   eBlkData PutData(Ipp8s *pBuffer, Ipp32s length, Ipp32u count, Ipp64f time);
   eBlkData GetData(Ipp8s **ppBuffer, Ipp32s *length, Ipp64f *time, Ipp32u count);
/************************************************************************/

/************************ Private variables *****************************/
   Ipp32u *pLostFrame;
   Ipp32s numLostFrame;
   Ipp32u *pIndex;
   Ipp32s numIndex;

   bool   flagRelease;
   Ipp64f Tpacket, Tdelta, Tjbuf;
   Ipp32s Npacket, Nres;

   Ipp32u m_countGetPacket;
   Ipp32s m_numDiscard;
   Ipp32u m_prevPutMarker;
   Ipp32s m_maxOffset;

   Ipp32s size_buffer;
   Ipp32s max_offset;
   bool   m_flagInit;

   Ipp8s  *pBitStream;
   Ipp8s  *pPutBit;
   Ipp8s  *pGetBit;
   Ipp8s  *pMaxBit;

   Ipp32s m_countLost, m_countReceive, m_countDiscard;
   bool   flagLost, flagReceive, flagDiscard;
   bool   flagJumpPut, flagJumpGet;

#if defined(WIN32) || defined(_WIN32_WCE)
   LARGE_INTEGER freq;
   LARGE_INTEGER start, finish;
   BOOL freq_status;
#else
   long start, finish;
#endif

   Ipp32s FLAG_PACKET_JB, CT_PACKET_JB;
};

}

#endif /* __JITTERBUFFER_H__ */

