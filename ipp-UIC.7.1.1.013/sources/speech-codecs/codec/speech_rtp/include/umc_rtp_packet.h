/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
#include "umc_media_data.h"

#ifndef __UMC_RTP_PACKET_H__
#define __UMC_RTP_PACKET_H__

namespace UMC
{
   #define RTP_VERSION 2

   typedef struct {
#if defined (_BIG_ENDIAN)
       Ipp32u version:2;   /* protocol version */
       Ipp32u p:1;         /* padding flag */
       Ipp32u x:1;         /* header extension flag */
       Ipp32u cc:4;        /* CSRC count */
       Ipp32u m:1;         /* marker bit */
       Ipp32u pt:7;        /* payload type */
#else
      Ipp32u cc:4;        /* CSRC count */
      Ipp32u x:1;         /* header extension flag */
      Ipp32u p:1;         /* padding flag */
      Ipp32u version:2;   /* protocol version */
      Ipp32u pt:7;        /* payload type */
      Ipp32u m:1;         /* marker bit */
#endif
      Ipp32u seq:16;           /* sequence number */
      Ipp32u ts;               /* timestamp */
      Ipp32u ssrc;             /* synchronization source */
      Ipp32u csrc[1];          /* optional CSRC list */
   } sRTP_Header;

   class RTP_Packet :public MediaData{
      DYNAMIC_CAST_DECL(RTP_Packet, MediaData)
   public:
      RTP_Packet(Ipp32u len=0);
      Ipp32s isValidPacket();

      Ipp32u GetVersion();
      Status SetVersion(Ipp32u version);
      Ipp32u GetPadding();
      Status SetPadding(Ipp32u paddind);
      Ipp32u GetExtension();
      Status SetExtension(Ipp32u extension);
      Ipp32u GetCSRCCount();
      Status SetCSRCCount(Ipp32u CSRCCount);
      Ipp32u GetMarked();
      Status SetMarked(Ipp32u marked);
      Ipp32u GetPayloadType();
      Status SetPayloadType(Ipp32u payloadType);
      Ipp16u GetSequenceNumber();
      Status SetSequenceNumber(Ipp16u sequenceNumber);
      Ipp32u GetTimeStamp();
      Status SetTimeStamp(Ipp32u timeStamp);
      Ipp32u GetSyncSource();
      Status SetSyncSource(Ipp32u syncSource);

      Ipp32s GetPayloadDataLen();
      Ipp32s GetHeaderSize();

      void *GetPayloadDataPointer(void);
      Status SetPayloadData(Ipp8u *ptr, size_t bytes);
      Status SetBufferPointer(Ipp8u *ptr, size_t bytes);

   protected:
      sRTP_Header *m_Header;
      Ipp32s m_Len;
   };
}
#endif /*__UMC_RTP_PACKET_H__*/
