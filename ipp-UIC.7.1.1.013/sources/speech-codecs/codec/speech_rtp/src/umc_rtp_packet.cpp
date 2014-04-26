/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
#include "ippdefs.h"
#include "ipps.h"
#include "umc_rtp_packet.h"

namespace UMC
{
   RTP_Packet::RTP_Packet(Ipp32u len) :MediaData(len)
   {
      m_Len=len;
      m_Header = (sRTP_Header *)m_pDataPointer;
   }

   Ipp32s RTP_Packet::isValidPacket()
   {
      if(m_Header->version!=RTP_VERSION) return 0;
      if(m_Header->pt > 127 /*Max payload type*/) return 0;
      return 1;
   }
   Status RTP_Packet::SetBufferPointer(Ipp8u *ptr, size_t bytes)
   {
      Status st;
      st=MediaData::SetBufferPointer(ptr, bytes);
      if(st==UMC_OK) {
         m_Header = (sRTP_Header *)m_pDataPointer;
         m_Len=(Ipp32s)bytes;
      }
      return st;
   }
   Ipp32u RTP_Packet::GetVersion()
   {
      if(m_Header) return m_Header->version;
      return 0;
   }
   Status RTP_Packet::SetVersion(Ipp32u version)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->version = version;
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetPadding()
   {
      if(m_Header) return m_Header->p;
      return 0;
   }
   Status RTP_Packet::SetPadding(Ipp32u paddind)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->p = paddind;
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetExtension()
   {
      if(m_Header) return m_Header->x;
      return 0;
   }
   Status RTP_Packet::SetExtension(Ipp32u extension)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->x = extension;
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetCSRCCount()
   {
      if(m_Header) return m_Header->cc;
      return 0;
   }
   Status RTP_Packet::SetCSRCCount(Ipp32u CSRCCount)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->cc = CSRCCount;
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetMarked()
   {
      if(m_Header) return m_Header->m;
      return 0;
   }
   Status RTP_Packet::SetMarked(Ipp32u marked)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->m = marked;
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetPayloadType()
   {
      if(m_Header) return m_Header->pt;
      return 0;
   }
   Status RTP_Packet::SetPayloadType(Ipp32u payloadType)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->pt = payloadType;
      return UMC_OK;
   }
   Ipp16u RTP_Packet::GetSequenceNumber()
   {
      if(m_Header) return LITTLE_ENDIAN_SWAP16(m_Header->seq);
      return 0;
   }
   Status RTP_Packet::SetSequenceNumber(Ipp16u sequenceNumber)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->seq = LITTLE_ENDIAN_SWAP16(sequenceNumber);
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetTimeStamp()
   {
      if(m_Header) return LITTLE_ENDIAN_SWAP32(m_Header->ts);
      return 0;
   }
   Status RTP_Packet::SetTimeStamp(Ipp32u timeStamp)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->ts = LITTLE_ENDIAN_SWAP32(timeStamp);
      return UMC_OK;
   }
   Ipp32u RTP_Packet::GetSyncSource()
   {
      if(m_Header) return LITTLE_ENDIAN_SWAP32(m_Header->ssrc);
      return 0;
   }
   Status RTP_Packet::SetSyncSource(Ipp32u syncSource)
   {
      if(m_Header==NULL) return UMC_ERR_NOT_INITIALIZED;
      m_Header->ssrc = LITTLE_ENDIAN_SWAP32(syncSource);
      return UMC_OK;
   }
   Ipp32s RTP_Packet::GetPayloadDataLen()
   {
      Ipp32s len = 0;
      if(m_Header) {
         len = m_Len-sizeof(sRTP_Header)+((m_Header->cc+1)*sizeof(Ipp32u));
         if(m_Header->p==1) {
            Ipp8u paddingLen = *(m_pDataPointer + m_Len);
            len = len - paddingLen;
         }
      }
      return len;
   }

   void *RTP_Packet::GetPayloadDataPointer(void)
   {
      return (void *)((Ipp8s*)m_pDataPointer+(sizeof(sRTP_Header)+((m_Header->cc-1)*sizeof(Ipp32u))));
   }
   Status RTP_Packet::SetPayloadData(Ipp8u *ptr, size_t bytes)
   {
      Ipp32s headerlen;
      headerlen = GetHeaderSize();
      //memcpy((Ipp8s*)m_pDataPointer+headerlen,ptr,bytes);
      ippsCopy_8u((const Ipp8u*)ptr,(Ipp8u*)(m_pDataPointer+headerlen),(Ipp32s)bytes);
      m_Len = headerlen+(Ipp32s)bytes;
      SetDataSize(m_Len);
      return UMC_OK;
   }

   Ipp32s RTP_Packet::GetHeaderSize()
   {
      return (sizeof(sRTP_Header)-(1*sizeof(Ipp32u)));
   }
}
