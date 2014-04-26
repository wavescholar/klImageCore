/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __BASE_RTP_CNVRT_H__
#define __BASE_RTP_CNVRT_H__

#include "umc_media_data.h"

namespace UMC
{

   class RTPBaseControlParams {
      DYNAMIC_CAST_DECL_BASE(RTPBaseControlParams)
   public:
      RTPBaseControlParams() {}
       virtual ~RTPBaseControlParams(){};
   };
/*Packetization API*/
   class RTPBasePacketizerParams {
      DYNAMIC_CAST_DECL_BASE(RTPBasePacketizerParams)
   public:
      RTPBasePacketizerParams() {}
      virtual ~RTPBasePacketizerParams(){};
   };

   class RTPBasePacketizer {
      DYNAMIC_CAST_DECL_BASE(RTPBasePacketizer)
   public:
      RTPBasePacketizer(){}
      virtual ~RTPBasePacketizer(){};

      // Init
      virtual Status Init(RTPBasePacketizerParams *pParams) = 0;
      // Get builded packet without RTP header
      virtual Status GetPacket(MediaData *pPacket) = 0;
      // Add frame to the packet
      virtual Status AddFrame(MediaData *pFrame) = 0;
      // Control packetizer
      virtual Status SetControls(RTPBaseControlParams *pControls) {pControls=pControls; return UMC_OK;}

   protected:
   };
/*Depacketization API*/
   class RTPBaseDePacketizerParams {
      DYNAMIC_CAST_DECL_BASE(RTPBaseDePacketizerParams)
   public:
      RTPBaseDePacketizerParams() {}
      virtual ~RTPBaseDePacketizerParams(){};
   };

   class RTPBaseDepacketizer {
      DYNAMIC_CAST_DECL_BASE(RTPBaseDepacketizer)
   public:
      RTPBaseDepacketizer(){}
      virtual ~RTPBaseDepacketizer(){};

      // Init
      virtual Status Init(RTPBaseDePacketizerParams *pParams) = 0;
      // Extract frame from the packet
      virtual Status GetNextFrame(MediaData *pFrame) = 0;
      // Set packet without RTP header
      virtual Status SetPacket(MediaData *pPacket) = 0;
      // Retrive controls
      virtual Status GetControls(RTPBaseControlParams *pControls) {pControls=pControls; return UMC_OK;}
   };
}

#endif /*__BASE_RTP_CNVRT_H__*/
