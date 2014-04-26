/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "ipps.h"
#include "rtp_amrwbe_payload.h"

namespace UMC
{

   #define AMR_WBE_SID_RTP_FT           9
   #define AMR_WBE_UNTR_RTP_FT            15

   static const Ipp16s idxToWBENodesTbl[3 * 48] = {
      /*WB|mono|stereo*/
        0, -1, -1, //0
        1, -1, -1, //1
        2, -1, -1, //2
        3, -1, -1, //3
        4, -1, -1, //4
        5, -1, -1, //5
        6, -1, -1, //6
        7, -1, -1, //7
        8, -1, -1, //8
        9, -1, -1, //WB SID (==DTX?)
       -1,  2, -1, //10
       -1,  2,  6, //11
       -1,  7, -1, //12
       -1,  5,  7, //13
       -2, -2, -2, //14 - FRAME_ERASURE
       -3, -3, -3, //15 - NO_DATA
       -1,  0, -1, //16
       -1,  1, -1, //17
       -1,  2, -1, //18
       -1,  3, -1, //19
       -1,  4, -1, //20
       -1,  5, -1, //21
       -1,  6, -1, //22
       -1,  7, -1, //23
       -1,  0,  0, //24
       -1,  0,  1, //25
       -1,  0,  4, //26
       -1,  1,  1, //27
       -1,  1,  3, //28
       -1,  1,  5, //29
       -1,  2,  2, //30
       -1,  2,  4, //31
       -1,  2,  6, //32
       -1,  3,  3, //33
       -1,  3,  5, //34
       -1,  3,  7, //35
       -1,  4,  4, //36
       -1,  4,  6, //37
       -1,  4,  9, //38
       -1,  5,  5, //39
       -1,  5,  7, //40
       -1,  5, 11, //41
       -1,  6,  8, //42
       -1,  6, 10, //43
       -1,  6, 15, //44
       -1,  7,  9, //45
       -1,  7, 10, //46
       -1,  7, 15  //47
   };

   static const Ipp32s tblPacketSizeMMS_WB[16]= {18, 24, 33, 37, 41, 47, 51, 59, 61, 6, 0, 0, 0, 0, 1, 1};

   static const Ipp16s tblNBitsAMRWBCore_16s[10] = {
       528, // 6k6
       708, // 8k85
      1012, // 12k65
      1140, // 14k25
      1268, // 15k85
      1460, // 18k25
      1588, // 19k85
      1844, // 23k05
      1908, // 23k85
       140  // DTX
   };
   static const Ipp16s tblNBitsMonoCore_16s[8] = {
       768, // 9k6
       896, // 11k2
      1024, // 12k8
      1152, // 14k4
      1280, // 16k0
      1472, // 18k4
      1600, // 20k0
      1856  // 23k2
   };
   static const Ipp16s tblNBitsStereoCore_16s[16] = {
       96, // 2k
      128, // 2k4
      160, // 2k8
      192, // 3k2
      224, // 3k6
      256, // 4k0
      288, // 4k4
      320, // 4k8
      352, // 5k2
      384, // 5k6
      416, // 6k0
      448, // 6k4
      480, // 6k8
      512, // 7k2
      544, // 7k6
      576  // 8k0
   };

   #define  NBITS_BWE    (4*16)     /* 4 packets x 16 bits = 0.8 kbps */

   static Ipp32s get_nb_bits(Ipp32s idxMode, Ipp32s idxRateWB, Ipp32s idxRateMono, Ipp32s idxRateStereo)
   {
      Ipp32s nb_bits;

      if ((idxMode!=14) && (idxMode!=15)) {
         if (idxRateWB < 0) {
            nb_bits = tblNBitsMonoCore_16s[idxRateMono] + NBITS_BWE;
            if (idxRateStereo >= 0) {
               nb_bits = nb_bits + (tblNBitsStereoCore_16s[idxRateStereo] + NBITS_BWE);
            }
         } else {
            nb_bits = tblNBitsAMRWBCore_16s[idxRateWB]; /*AMRWB rates*/
         }
      } else {
         nb_bits = 0;
      }
      return (Ipp32s)nb_bits;
   }

   static Ipp32s GetFrameLenBytesByFT(Ipp32s FT) {
      Ipp32s idxRateWB = idxToWBENodesTbl[FT*3];
      Ipp32s idxRateMono = idxToWBENodesTbl[FT*3+1];
      Ipp32s idxRateStereo = idxToWBENodesTbl[FT*3+2];
      Ipp32s nBytes;

      if (idxRateWB < 0) { /* WB+ mode */
         Ipp32s nbits = get_nb_bits(FT, idxRateWB, idxRateMono, idxRateStereo);
         nBytes = (nbits/4+7)/8;
      } else { /* WB modes, read 20ms frame and return */
         nBytes = tblPacketSizeMMS_WB[idxRateWB];
      }
      return nBytes;
   }

   static Ipp32s CovertFrametypeToBitrate (Ipp32s FT, Ipp32s* pBitrate)
   {
      switch (FT) {
         /* AMRWB modes*/
         case 0 : *pBitrate=  6600;  break;
         case 1 : *pBitrate=  8850;  break;
         case 2 : *pBitrate= 12650;  break;
         case 3 : *pBitrate= 14250;  break;
         case 4 : *pBitrate= 15850;  break;
         case 5 : *pBitrate= 18250;  break;
         case 6 : *pBitrate= 19850;  break;
         case 7 : *pBitrate= 23050;  break;
         case 8 : *pBitrate= 23850;  break;
         /* ARMWB character modes (for WB <-> WB+) */
         case 10 : *pBitrate=13600;  break;
         case 11 : *pBitrate=18000;  break;
         case 12 : *pBitrate=24000;  break;
         case 13 : *pBitrate=24000;  break;
         /* AMRWB+ mono modes */
         case 16 : *pBitrate=10400;  break;
         case 17 : *pBitrate=12000;  break;
         case 18 : *pBitrate=13600;  break;
         case 19 : *pBitrate=15200;  break;
         case 20 : *pBitrate=16800;  break;
         case 21 : *pBitrate=19200;  break;
         case 22 : *pBitrate=20800;  break;
         case 23 : *pBitrate=24000;  break;
         /* AMRWB+ stereo modes */
         case 24 : *pBitrate=12400;  break;
         case 25 : *pBitrate=12800;  break;
         case 26 : *pBitrate=14000;  break;
         case 27 : *pBitrate=14400;  break;
         case 28 : *pBitrate=15200;  break;
         case 29 : *pBitrate=16000;  break;
         case 30 : *pBitrate=16400;  break;
         case 31 : *pBitrate=17200;  break;
         case 32 : *pBitrate=18000;  break;
         case 33 : *pBitrate=18400;  break;
         case 34 : *pBitrate=19200;  break;
         case 35 : *pBitrate=20000;  break;
         case 36 : *pBitrate=20400;  break;
         case 37 : *pBitrate=21200;  break;
         case 38 : *pBitrate=22400;  break;
         case 39 : *pBitrate=23200;  break;
         case 40 : *pBitrate=24000;  break;
         case 41 : *pBitrate=25600;  break;
         case 42 : *pBitrate=26000;  break;
         case 43 : *pBitrate=26800;  break;
         case 44 : *pBitrate=28800;  break;
         case 45 : *pBitrate=29600;  break;
         case 46 : *pBitrate=30000;  break;
         case 47 : *pBitrate=32000;  break;
         default :
            return -1;
      }
      return 0;
   }

   AMRWBEPacketizer::AMRWBEPacketizer()
   {
   }
   AMRWBEPacketizer::~AMRWBEPacketizer()
   {
      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }
   }

   Status AMRWBEPacketizer::Init(RTPBasePacketizerParams *pParams)
   {
      if(pParams==NULL) return UMC_ERR_NULL_PTR;

      AMRWBEPacketizerParams *pAMRWBEParams = DynamicCast<AMRWBEPacketizerParams, RTPBasePacketizerParams>(pParams);

      if(pAMRWBEParams==NULL) return UMC_ERR_NULL_PTR;

      m_ptType = pAMRWBEParams->m_ptType;
      m_InterleavingFlag = pAMRWBEParams->m_InterleavingFlag;

      return UMC_OK;
   }

   Status AMRWBEPacketizer::SetControls(RTPBaseControlParams *pControls)
   {
      if(pControls==NULL) return UMC_ERR_NULL_PTR;

      AMRWBEControlParams *pAMRWBECtrls = DynamicCast<AMRWBEControlParams, RTPBaseControlParams>(pControls);

      if(pAMRWBECtrls==NULL) return UMC_ERR_NULL_PTR;

      if(m_ptType==InterleavingMode) {
         m_Ctrls.m_isLongDisp = pAMRWBECtrls->m_isLongDisp;
      } else {
         m_Ctrls.m_isLongDisp = 0;
      }

      return UMC_OK;
   }

   Status AMRWBEPacketizer::AddFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pSpeechFrame==NULL) return UMC_ERR_NULL_PTR;

      SpeechData *pNewFrame = NULL;

      pNewFrame = new SpeechData((Ipp32u)pFrame->GetDataSize());
      if(NULL == pNewFrame) return UMC_ERR_ALLOC;

      pSpeechFrame->MoveDataTo(pNewFrame);

      m_List.Add(pNewFrame);

      return UMC_OK;
   }

   Status AMRWBEPacketizer::GetPacket(MediaData *pPacket)
   {
      Ipp8u *pPacketPayload;
      Ipp8u *pSuperFrameContent, *pTmpFrameContent;
      Ipp32s PayloadSize = 0;
      Ipp32u i,j;
      SpeechData *pIt;
      Ipp8u firstIsf, currIsf;
      Ipp8u firstTfi, currTfi;
      Ipp8u firstFT, currFT;
      Ipp8u nFrames = 0;
      Ipp32s uscBtstrmOffset=0;
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;
      if(m_List.Size()==0) return UMC_ERR_NOT_ENOUGH_DATA;

      pPacketPayload = (Ipp8u *)pPacket->GetDataPointer();

      /*Read info from 1st super frame*/

      m_List.Get(pIt,0);

      pSuperFrameContent = (Ipp8u *)pIt->GetDataPointer();

      firstFT = pSuperFrameContent[0] & 0x7F;
      firstIsf = pSuperFrameContent[1] & 0x1f;
      firstTfi = pSuperFrameContent[1] >> 6;
      if(firstFT >=0 && firstFT <=9 ) {
         /*AMRWB bitrates. Pack as FRC3267 as octet-align but don't do reordering*/
         return UMC_OK;
      }
      /*Write packet header*/
      pPacketPayload[0] = (m_Ctrls.m_isLongDisp << 7) | (firstTfi << 5) | firstIsf;

      pPacketPayload++;
      PayloadSize++;

      nFrames = 0;

      /*Write ToC*/
      for(i=0;i<m_List.Size();i++) {
         m_List.Get(pIt,i);
         pSuperFrameContent = (Ipp8u *)pIt->GetDataPointer();
         pTmpFrameContent = pSuperFrameContent;
         for(j=0;j<4;j++) {
            currFT = pTmpFrameContent[0] & 0x7F;
            currIsf = pTmpFrameContent[1] & 0x1f;
            currTfi = pTmpFrameContent[1] >> 6;
            if((currFT != firstFT) || (currIsf != firstIsf)) {
               /*Write entry*/
               *pPacketPayload = (firstFT << 1) | 1;
               pPacketPayload++;
               PayloadSize++;
               *pPacketPayload = nFrames;
               pPacketPayload++;
               PayloadSize++;

               firstFT = currFT;
               firstIsf = currIsf;
               firstTfi = currTfi;
            } else {
               nFrames++;
            }
            uscBtstrmOffset = GetFrameLenBytesByFT(firstFT);
            pTmpFrameContent = pTmpFrameContent + uscBtstrmOffset + 2/*Header size*/;
         }
      }
      /*Last entry*/
      *pPacketPayload = firstFT << 1;
      pPacketPayload++;
      PayloadSize++;
      *pPacketPayload = nFrames;
      pPacketPayload++;
      PayloadSize++;

      /*Write audio data*/

      for(i=0;i<m_List.Size();i++) {
         Ipp32s bitSize;
         m_List.Get(pIt,i);
         pSuperFrameContent = (Ipp8u *)pIt->GetDataPointer();
         pTmpFrameContent = pSuperFrameContent;

         for(j=0;j<4;j++) {
            currFT = pSuperFrameContent[0] & 0x7F;
            bitSize = GetFrameLenBytesByFT(currFT);

            if((currFT != 14) && (currFT != 15)) {
               ippsCopy_8u(pTmpFrameContent + 2, pPacketPayload,bitSize);
               pPacketPayload += bitSize;
               PayloadSize += bitSize;
            }
            pTmpFrameContent = pTmpFrameContent + bitSize + 2/*Header size*/;
         }
      }

      pPacket->SetDataSize(PayloadSize);

      while(m_List.Size() > 0) { /*Clean list*/
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }

      return UMC_OK;
   }

   AMRWBEDePacketizer::AMRWBEDePacketizer() : RTPBaseDepacketizer()
   {
   }

   AMRWBEDePacketizer::~AMRWBEDePacketizer()
   {
      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }
   }

   Status AMRWBEDePacketizer::Init(RTPBaseDePacketizerParams *pParams)
   {
      if(pParams==NULL) return UMC_ERR_NULL_PTR;

      AMRWBEDePacketizerParams *pAMRParams = DynamicCast<AMRWBEDePacketizerParams, RTPBaseDePacketizerParams>(pParams);

      if(pAMRParams==NULL) return UMC_ERR_NULL_PTR;

      m_ptType = pAMRParams->m_ptType;

      m_InterleavingFlag = pAMRParams->m_InterleavingFlag;

      /*if(m_CodecType==NB) {
         m_PrevBitrate = 12200;
      } else {
         m_PrevBitrate = 6600;
      }*/

      return UMC_OK;
   }

   Status AMRWBEDePacketizer::GetControls(RTPBaseControlParams *pControls)
   {
      if(pControls==NULL) return UMC_ERR_NULL_PTR;

      AMRWBEControlParams *pAMRCtrls = DynamicCast<AMRWBEControlParams, RTPBaseControlParams>(pControls);

      if(pAMRCtrls==NULL) return UMC_ERR_NULL_PTR;
      pAMRCtrls->m_isLongDisp = m_Ctrls.m_isLongDisp;

      return UMC_OK;
   }

   Status AMRWBEDePacketizer::GetNextFrame(MediaData *pFrame)
   {
      SpeechData *pSpeechFrame = DynamicCast<SpeechData, MediaData>(pFrame);
      if(pFrame==NULL) return UMC_ERR_NULL_PTR;

      if(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.First(pIt);
         pIt->MoveDataTo(pSpeechFrame);
         delete pIt;
         m_List.Remove(0);
         /*remove next three*/
         m_List.First(pIt);
         delete pIt;
         m_List.Remove(0);

         m_List.First(pIt);
         delete pIt;
         m_List.Remove(0);

         m_List.First(pIt);
         delete pIt;
         m_List.Remove(0);
      } else {
         return UMC_ERR_NOT_ENOUGH_DATA;
      }
      return UMC_OK;
   }

   Status AMRWBEDePacketizer::SetPacket(MediaData *pPacket)
   {
      if(pPacket==NULL) return UMC_ERR_NULL_PTR;

      while(m_List.Size() > 0) {
         SpeechData *pIt;
         m_List.Last(pIt);
         delete pIt;
         m_List.Remove();
      }

      Ipp8u *pPacketPayload = (Ipp8u *)pPacket->GetDataPointer();
      Ipp8u firstIsf;
      Ipp8u firstTfi;
      Ipp8u FT, nFrames, Fbit = 1;
      Ipp32s i,j;

      /*Read packet header*/
      m_Ctrls.m_isLongDisp = pPacketPayload[0] >> 7;
      firstTfi = (pPacketPayload[0] >> 5) & 0x3;
      firstIsf = pPacketPayload[0] & 0x1F;

      pPacketPayload++;

      while(Fbit==1) {
         Fbit = pPacketPayload[0] & 0x1;
         FT = pPacketPayload[0] >> 1;
         nFrames = pPacketPayload[1];

         pPacketPayload += 2;

         for(i=0;i<nFrames;i++) {
            int bitrate, frmSize;
            Ipp8u *pFrameContent;

            UMC::SpeechData *pNewFrame = NULL;

            frmSize = GetFrameLenBytesByFT(FT) + 2;

            pNewFrame = new SpeechData(frmSize*4);

            if(NULL == pNewFrame) return UMC_ERR_ALLOC;

            if(CovertFrametypeToBitrate(FT, &bitrate) < 0) {
               bitrate = m_PrevBitrate;
            } else {
               m_PrevBitrate = bitrate;
            }

            pNewFrame->SetFrameType(0);
            pNewFrame->SetBitrate(bitrate);
            pNewFrame->SetNBytes(frmSize);
            pNewFrame->SetDataSize(frmSize);

            /*Prepare IF2header*/
            pFrameContent = (Ipp8u*)pNewFrame->GetDataPointer();

            pFrameContent[0] = FT;  /*write mode index information */
            pFrameContent[1] = (firstTfi << 6) | (firstIsf & 0x1F);

            m_List.Add(pNewFrame);
         }
      }

      for(i=0;i<(Ipp32s)m_List.Size();i++) {
         SpeechData *pIt;
         Ipp8u *pFrameContent;
         Ipp8u FT;
         int frmSize;

         m_List.Get(pIt,i);

         pFrameContent = (Ipp8u*)pIt->GetDataPointer();
         FT = pFrameContent[0];

         if((FT==14) || (FT==15)) {
            continue;
         }

         frmSize = GetFrameLenBytesByFT(FT);
         ippsCopy_8u(pPacketPayload,pFrameContent + 2,frmSize);
         pPacketPayload += frmSize;
      }

      /*Merge frames to super-frame*/
      for(i=0;i<(Ipp32s)m_List.Size();i+=4) {
         SpeechData *pIt, *pIt1;
         Ipp8u *pFrameContent, *pFrameContent1;
         int superFrmDataSize,superFrmDataSize1;

         m_List.Get(pIt,i);

         for(j=1;j<4;j++) {
            m_List.Get(pIt1,i+j);

            superFrmDataSize = (Ipp32s)pIt->GetDataSize();
            superFrmDataSize1 = (Ipp32s)pIt1->GetDataSize();

            pFrameContent  = (Ipp8u*)pIt->GetDataPointer();
            pFrameContent1 = (Ipp8u*)pIt1->GetDataPointer();

            ippsCopy_8u(pFrameContent1,pFrameContent + superFrmDataSize,superFrmDataSize1);
            pIt->SetDataSize(superFrmDataSize + superFrmDataSize1);
            pIt->SetNBytes(superFrmDataSize + superFrmDataSize1);
         }
      }

      return UMC_OK;
   }
}
