/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __RTP_AMR_PAYLOAD_H__
#define __RTP_AMR_PAYLOAD_H__

#include "umc_linked_list.h"
#include "base_rtp_cnvrt.h"
#include "umc_speech_data.h"

namespace UMC
{
   typedef enum {
      BandEfficient = 0,
      OctetAlign = 1
   } AMRpayloadType;

   typedef enum {
      NB = 0,
      WB = 1
   } AMRCodecType;

   class AMRControlParams : public RTPBaseControlParams {
      DYNAMIC_CAST_DECL(AMRControlParams, RTPBaseControlParams)
         AMRControlParams() : RTPBaseControlParams()
      {
         m_CMR = 15;
         m_ILL = 1;
         m_ILP = 1;
      }
   public:
      Ipp32s m_CMR;
      Ipp32s m_ILL;
      Ipp32s m_ILP;
   };

   class AMRPacketizerParams : public RTPBasePacketizerParams{
      DYNAMIC_CAST_DECL(AMRPacketizerParams, RTPBasePacketizerParams)
   public:

      AMRPacketizerParams()  : RTPBasePacketizerParams () {
         m_ptType = BandEfficient;
         m_CodecType = NB;
         m_InterleavingFlag = 0;
      }
      AMRpayloadType m_ptType;
      AMRCodecType m_CodecType;
      Ipp32s m_InterleavingFlag;
   };

   class AMRPacketizer : public RTPBasePacketizer{
      DYNAMIC_CAST_DECL(AMRPacketizer, RTPBasePacketizer)
   public:
      AMRPacketizer();
      ~AMRPacketizer();

      Status Init(RTPBasePacketizerParams *pParams);

      Status GetPacket(MediaData *pPacket);
      Status AddFrame(MediaData *pFrame);
      Status SetControls(RTPBaseControlParams *pControls);

   protected:
      void USC2RTPActiveFrame(Ipp8u *pSrc, Ipp32s size, Ipp32s *pOrderMap, Ipp32s FrameLenBits,
                                    Ipp8u **pDst, Ipp32s *pDstSize, Ipp32s *bitOffset);
      void USC2RTPSIDFrame(Ipp8u *pSrc, Ipp32s size, Ipp32s STI, Ipp32s mode, Ipp8u **pDst, Ipp32s *pDstSize, Ipp32s *bitOffset);
      void USC2RFCParams(SpeechData *pSpData, Ipp32s *pFT,Ipp32s **pOrderMap, Ipp32s *FrameLenBits);
      void USC2RFCFrameType(SpeechData *pSpData, Ipp32s *pMode);
      AMRpayloadType m_ptType;
      AMRCodecType m_CodecType;
      Ipp32s m_InterleavingFlag;
      LinkedList<SpeechData*> m_List;
      AMRControlParams m_Ctrls;
   };

   class AMRDePacketizerParams : public RTPBaseDePacketizerParams{
      DYNAMIC_CAST_DECL(AMRDePacketizerParams, RTPBaseDePacketizerParams)
   public:

      AMRDePacketizerParams()  : RTPBaseDePacketizerParams () {
         m_ptType = BandEfficient;
         m_CodecType = NB;
         m_InterleavingFlag = 0;
      }

      AMRpayloadType m_ptType;
      AMRCodecType m_CodecType;
      Ipp32s m_InterleavingFlag;
   };

   class AMRDePacketizer  : public RTPBaseDepacketizer{
      DYNAMIC_CAST_DECL(AMRDePacketizer, RTPBaseDepacketizer)
   public:
      AMRDePacketizer();
      ~AMRDePacketizer();

      virtual Status Init(RTPBaseDePacketizerParams *pParams);
      virtual Status GetNextFrame(MediaData *pFrame);
      virtual Status SetPacket(MediaData *pPacket);
      virtual Status GetControls(RTPBaseControlParams *pControls);

   protected:
      void CnvrtRFC2USCParams(SpeechData *pSpData, Ipp32s FT);
      void CnvrtRFC2USCFrameType(SpeechData *pSpData, Ipp32s mode);
      void GetBitReordersTable(SpeechData *pSpData, Ipp32s **pOrderMap, Ipp32s *FrameLenBits);
      void RTP2USCActiveFrame(Ipp8u **pSrc, Ipp8u *pDst, Ipp32s size, Ipp32s *pOrderMap, Ipp32s FrameLenBits,
                                             Ipp32s *bitOffset);
      void RTP2USCSIDFrame(Ipp8u **pSrc, Ipp8u *pDst, Ipp32s size, Ipp32s *STI, Ipp32s *mode, Ipp32s *bitOffset);
      AMRpayloadType m_ptType;
      AMRCodecType m_CodecType;
      Ipp32s m_InterleavingFlag;
      LinkedList<SpeechData*> m_List;
      Ipp32s m_PrevBitrate;
      AMRControlParams m_Ctrls;
   };
}


#endif /*__RTP_AMR_PAYLOAD_H__*/
