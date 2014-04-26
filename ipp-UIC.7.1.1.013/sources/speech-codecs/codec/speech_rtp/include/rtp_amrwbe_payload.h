/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __RTP_AMRWBE_PAYLOAD_H__
#define __RTP_AMRWBE_PAYLOAD_H__

#include "umc_linked_list.h"
#include "base_rtp_cnvrt.h"
#include "umc_speech_data.h"

namespace UMC
{
   typedef enum {
      BasicMode = 0,
      InterleavingMode = 1
   } AMRWBEpayloadType;


   class AMRWBEControlParams : public RTPBaseControlParams {
      DYNAMIC_CAST_DECL(AMRWBEControlParams, RTPBaseControlParams)
         AMRWBEControlParams() : RTPBaseControlParams()
      {
         m_isLongDisp = 0;
      }
   public:
      Ipp32s m_isLongDisp;
   };

   class AMRWBEPacketizerParams : public RTPBasePacketizerParams{
      DYNAMIC_CAST_DECL(AMRWBEPacketizerParams, RTPBasePacketizerParams)
   public:

      AMRWBEPacketizerParams()  : RTPBasePacketizerParams () {
         m_ptType = BasicMode;
         m_InterleavingFlag = 0;
      }
      AMRWBEpayloadType m_ptType;
      Ipp32s m_InterleavingFlag;
   };

   class AMRWBEPacketizer : public RTPBasePacketizer{
      DYNAMIC_CAST_DECL(AMRWBEPacketizer, RTPBasePacketizer)
   public:
      AMRWBEPacketizer();
      ~AMRWBEPacketizer();

      Status Init(RTPBasePacketizerParams *pParams);

      Status GetPacket(MediaData *pPacket);
      Status AddFrame(MediaData *pFrame);
      Status SetControls(RTPBaseControlParams *pControls);

   protected:
      AMRWBEpayloadType m_ptType;
      Ipp32s m_InterleavingFlag;
      LinkedList<SpeechData*> m_List;
      AMRWBEControlParams m_Ctrls;
   };

   class AMRWBEDePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(AMRWBEDePacketizerParams, RTPBaseDePacketizerParams)
   public:

      AMRWBEDePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_ptType = BasicMode;
         m_InterleavingFlag = 0;
      }

      AMRWBEpayloadType m_ptType;
      Ipp32s m_InterleavingFlag;
   };

   class AMRWBEDePacketizer  : public RTPBaseDepacketizer{
      DYNAMIC_CAST_DECL(AMRWBEDePacketizer, RTPBaseDepacketizer)
   public:
      AMRWBEDePacketizer();
      ~AMRWBEDePacketizer();

      virtual Status Init(RTPBaseDePacketizerParams *pParams);
      virtual Status GetNextFrame(MediaData *pFrame);
      virtual Status SetPacket(MediaData *pPacket);
      virtual Status GetControls(RTPBaseControlParams *pControls);

   protected:
      AMRWBEpayloadType m_ptType;
      Ipp32s m_InterleavingFlag;
      LinkedList<SpeechData*> m_List;
      Ipp32s m_PrevBitrate;
      AMRWBEControlParams m_Ctrls;
   };
}


#endif /*__RTP_AMRWBE_PAYLOAD_H__*/
