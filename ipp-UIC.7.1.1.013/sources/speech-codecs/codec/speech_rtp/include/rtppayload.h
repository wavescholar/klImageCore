/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/
#ifndef __PTR_PAYLOAD_H__
#define __PTR_PAYLOAD_H__
namespace UMC
{
   enum RTPstaticPayloadType
   {
      ptPCMU = 0,
      ptGSM = 3,
      ptG723 = 4,
      ptDIV4_8 = 5,
      ptDIV4_16 = 6,
      ptLPC = 7,
      ptPCMA = 8,
      ptG722 = 9,
      ptL16_s = 10,
      ptL16 = 11,
      ptQCELP = 12,
      ptCN = 13,
      ptMPA = 14,
      ptG728 = 15,
      ptDIV4_11 = 16,
      ptDIV4_22 = 17,
      ptG729 = 18,
      ptLastAudioStatic = 19,
      ptDynamicFirst = 96,
      ptEnd = 127
   };
   static const Ipp32s RTPstaticClockRate [ptLastAudioStatic] = {
      8000, -1, -1, 8000, 8000, 8000, 16000, 8000, 8000, 8000,
      44100, 44100, 8000, 8000, 90000, 8000, 11025, 22050, 8000
   };

   static const Ipp32s RTPstaticDefPacketLenMS [ptLastAudioStatic] = {
      20, -1, -1, 20, 30, 20, 20, 20, 20, 20,
      20, 20, 20, -1, -1, 20, 20, 20, 20
   };

   class RTP_PayloadType{
   public:
      RTP_PayloadType(Ipp32s PayloadType, Ipp32s PayloadClockRate, Ipp32s DefPacketLenMS=20)
      {
         if((RTPstaticPayloadType)PayloadType < ptLastAudioStatic) {
            m_PayloadType = PayloadType;
            m_PayloadClockRate = RTPstaticClockRate[m_PayloadType];
            m_DefPacketLenMS = RTPstaticDefPacketLenMS[m_PayloadType];
         } else {
            m_PayloadType = PayloadType;
            m_PayloadClockRate = PayloadClockRate;
            m_DefPacketLenMS = DefPacketLenMS;
         }
      }
      RTP_PayloadType(RTPstaticPayloadType spt)
      {
         m_PayloadType = (Ipp32s)spt;
         if(spt < ptLastAudioStatic) {
            m_PayloadClockRate = RTPstaticClockRate[m_PayloadType];
            m_DefPacketLenMS = RTPstaticDefPacketLenMS[m_PayloadType];
         } else {
            m_PayloadClockRate = 0;
            m_DefPacketLenMS = 0;
         }
      }
      Ipp32s GetPayloadType() {return m_PayloadType;}
      Ipp32s GetClockRate() {return m_PayloadClockRate;}
      Ipp32s GetPacketLenMS() {return m_DefPacketLenMS;}
      void SetPacketLenMS(Ipp32s DefPacketLenMS) {m_DefPacketLenMS=DefPacketLenMS;return;}

      Ipp32s operator==( RTP_PayloadType &pt)
      {
         if((m_PayloadType == pt.m_PayloadType)&&
            (m_PayloadClockRate ==pt.m_PayloadClockRate)&&
            (m_DefPacketLenMS == pt.m_DefPacketLenMS)) return 1;
         return 0;
      }
   protected:
      Ipp32s m_PayloadType;
      Ipp32s m_PayloadClockRate;
      Ipp32s m_DefPacketLenMS;
   };

}
#endif/*__PTR_PAYLOAD_H__*/
