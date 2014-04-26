/*
////////////////////////////////////////////////////////////////////////
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Speech codec and format library.
*/

#include "usc_codec_container.h"
#include "usc.h"

#ifdef _USC_ALL
   #undef _USC_ALL
   #define _USC_CODECS
#endif

#include "usc_objects_decl.h"

#include "rtp_amr_payload.h"
#include "common_rtp_payloads.h"
#include "umc_speech_jbuffer.h"
#include "umc_rta_jbuffer.h"

typedef struct _StaticCodecs {
   vm_char codecName[64];
   void *funcPtr;
   UMC::RTP_PayloadType *PTptr;
}StaticCodecsEntry;

static UMC::RTP_PayloadType G711APayloadType(UMC::ptPCMA);
static UMC::RTP_PayloadType G711UPayloadType(UMC::ptPCMU);
static UMC::RTP_PayloadType G723PayloadType(UMC::ptG723);
static UMC::RTP_PayloadType AMR_NBPayloadType(UMC::ptDynamicFirst,8000);
static UMC::RTP_PayloadType AMR_WBPayloadType(UMC::ptDynamicFirst+1,16000);
static UMC::RTP_PayloadType AMR_WBEPayloadType(UMC::ptDynamicFirst+2,72000);
static UMC::RTP_PayloadType G726PayloadType(UMC::ptDynamicFirst+3,72000);
static UMC::RTP_PayloadType G728PayloadType(UMC::ptG728);
static UMC::RTP_PayloadType G7221PayloadType(UMC::ptDynamicFirst+4,16000);
static UMC::RTP_PayloadType G729PayloadType(UMC::ptG729);
static UMC::RTP_PayloadType G729EPayloadType(UMC::ptDynamicFirst+5,8000);
static UMC::RTP_PayloadType G729DPayloadType(UMC::ptDynamicFirst+6,8000);
static UMC::RTP_PayloadType GSMPayloadType(UMC::ptGSM);
static UMC::RTP_PayloadType G722PayloadType(UMC::ptG722);
static UMC::RTP_PayloadType iLBCPayloadType(UMC::ptDynamicFirst+7,8000);
static UMC::RTP_PayloadType RTA_WBPayloadType(UMC::ptDynamicFirst+18,16000);
static UMC::RTP_PayloadType RTA_NBPayloadType(UMC::ptDynamicFirst+19,8000);


static StaticCodecsEntry LinkedCodecs[] =
{
#if (defined _USC_AMRWB || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_AMRWB"),&USC_AMRWB_Fxns, &AMR_WBPayloadType,
#endif
#if (defined _USC_GSMAMR || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_GSMAMR"),&USC_GSMAMR_Fxns, &AMR_NBPayloadType,
#endif
#if (defined _USC_G711 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G711A"),&USC_G711A_Fxns, &G711APayloadType,
   VM_STRING("IPP_G711U"),&USC_G711U_Fxns, &G711UPayloadType,
#endif
#if (defined _USC_G723 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G723.1"),&USC_G723_Fxns, &G723PayloadType,
#endif
#if (defined _USC_G726 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G726"),&USC_G726_Fxns, &G726PayloadType,
#endif
#if (defined _USC_G728 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G728"),&USC_G728_Fxns, &G728PayloadType,
#endif
#if (defined _USC_G729 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G729"),&USC_G729I_Fxns, &G729PayloadType,
   VM_STRING("IPP_G729A"),&USC_G729A_Fxns, &G729PayloadType,
   VM_STRING("IPP_G729E"),&USC_G729I_Fxns, &G729EPayloadType,
   VM_STRING("IPP_G729D"),&USC_G729I_Fxns, &G729DPayloadType,
#endif
#if (defined _USC_G729FP|| defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G729_FP"),&USC_G729IFP_Fxns, &G729PayloadType,
   VM_STRING("IPP_G729A_FP"),&USC_G729AFP_Fxns, &G729PayloadType,
   VM_STRING("IPP_G729E_FP"),&USC_G729IFP_Fxns, &G729EPayloadType,
   VM_STRING("IPP_G729D_FP"),&USC_G729IFP_Fxns, &G729DPayloadType,
#endif
#if (defined _USC_G722 || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G722.1"),&USC_G722_Fxns, &G7221PayloadType,
#endif
#if (defined _USC_GSMFR || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_GSMFR"),&USC_GSMFR_Fxns, &GSMPayloadType,
#endif
#if (defined _USC_G722SB || defined _USC_CODECS || defined _USC_ALL)
   VM_STRING("IPP_G722"),&USC_G722SB_Fxns, &G722PayloadType,
#endif
#if (defined _USC_ILBC)
   VM_STRING("IPP_iLBC"),&USC_ILBC_Fxns, &iLBCPayloadType,
#endif
#if (defined _USC_MSRTA)
   VM_STRING("IPP_MSRTAwb_FP"),&USC_MSRTAFP_Fxns, &RTA_WBPayloadType,
   VM_STRING("IPP_MSRTAnb_FP"),&USC_MSRTAFP_Fxns, &RTA_NBPayloadType,
#endif
   VM_STRING("END"),NULL,NULL
};

Ipp32s USC_CodecContainer::GetNumCodecs()
{
   Ipp32s i = 0;
   while(vm_string_strcmp(LinkedCodecs[i].codecName,VM_STRING("END"))!=0) {
      i++;
   }
   return i;
}

vm_char *USC_CodecContainer::GetCodecName(Ipp32s index)
{
   if(index<0) return NULL;
   return LinkedCodecs[index].codecName;
}

void USC_CodecContainer::SetCodecName(const vm_char *nameCodec)
{
   m_Inited = 0;

   if(nameCodec) {
      vm_string_strcpy((vm_char*)m_pCodecName,nameCodec);
      m_Inited = 1;
   }
}
void *USC_CodecContainer::TryToFindStatic(const vm_char *nameCodec)
{
   void *pAddr = NULL;
   Ipp32s i=0;
   while(vm_string_strcmp(LinkedCodecs[i].codecName,VM_STRING("END"))!=0) {
      if(vm_string_strcmp(nameCodec, LinkedCodecs[i].codecName) == 0) {
         pAddr = LinkedCodecs[i].funcPtr;
      }
      i++;
   }
   return pAddr;
}

UMC::AudioCodec* USC_CodecContainer::CreateUSCCodec(Ipp32s direction)
{
   USC_Fxns *pFuncTbl = NULL;
   UMC::AudioCodec* pCodec = NULL;

   if(m_Inited) {
      pFuncTbl = (USC_Fxns*)TryToFindStatic((const vm_char *)m_pCodecName);
      if(NULL == pFuncTbl) {
         return NULL;
      }
      if(direction == 1) {
         pCodec = (UMC::AudioCodec*)new UMC::USCEncoder(pFuncTbl);
      } else {
         pCodec = (UMC::AudioCodec*)new UMC::USCDecoder(pFuncTbl);
      }
   }
   return pCodec;
}

UMC::Status USC_CodecContainer::CreateRTPRePacketizer(Ipp32s direction, Ipp32s bitrate, Ipp32s btstrmFmt,
                                                UMC::RTPBasePacketizer** pPck,UMC::RTPBaseDepacketizer** pDePck)
{
   *pPck = NULL;
   *pDePck = NULL;

   if(!m_Inited) return UMC::UMC_ERR_NOT_INITIALIZED;

   #if (defined _USC_GSMAMR || defined _USC_CODECS || defined _USC_ALL)
   UMC::Status UMCret;
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_GSMAMR"))==0) {
      if(direction==1) {
         *pPck = new UMC::AMRPacketizer;
         UMC::AMRPacketizerParams pPckPrms;
         pPckPrms.m_CodecType = UMC::NB;
         if(btstrmFmt == 1) {
             pPckPrms.m_ptType = UMC::OctetAlign;
         }
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::AMRDePacketizer;
         UMC::AMRDePacketizerParams pDePckPrms;
         pDePckPrms.m_CodecType = UMC::NB;
         if(btstrmFmt == 1) {
             pDePckPrms.m_ptType = UMC::OctetAlign;
         }
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_AMRWB || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_AMRWB"))==0) {
      if(direction==1) {
         *pPck = new UMC::AMRPacketizer;
         UMC::AMRPacketizerParams pPckPrms;
         pPckPrms.m_CodecType = UMC::WB;
         if(btstrmFmt == 1) {
            pPckPrms.m_ptType = UMC::OctetAlign;
         }
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::AMRDePacketizer;
         UMC::AMRDePacketizerParams pDePckPrms;
         pDePckPrms.m_CodecType = UMC::WB;
         if(btstrmFmt == 1) {
            pDePckPrms.m_ptType = UMC::OctetAlign;
         }
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G711 || defined _USC_CODECS || defined _USC_ALL)
   if((vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G711A"))==0) ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G711U"))==0)) {
      if(direction==1) {
         *pPck = new UMC::G711Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G711DePacketizer;
         UMC::RTPBaseDePacketizerParams pDePckPrms;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G723 || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G723.1"))==0) {
      if(direction==1) {
         *pPck = new UMC::G723Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G723DePacketizer;
         UMC::RTPBaseDePacketizerParams pDePckPrms;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G726 || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G726"))==0) {
      if(direction==1) {
         *pPck = new UMC::G726Packetizer;
         UMC::G726PacketizerParams pPckPrms;
         pPckPrms.m_ptType = UMC::RTP;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G726DePacketizer;
         UMC::G726DePacketizerParams pDePckPrms;
         pDePckPrms.m_ptType = UMC::RTP;
         pDePckPrms.m_Bitrate = bitrate;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G728 || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G728"))==0) {
      if(direction==1) {
         *pPck = new UMC::G728Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G728DePacketizer;
         UMC::RTPBaseDePacketizerParams pDePckPrms;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G722 || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G722.1"))==0) {
      if(direction==1) {
         *pPck = new UMC::G7221Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G7221DePacketizer;
         UMC::G7221DePacketizerParams pDePckPrms;
         pDePckPrms.m_Bitrate = bitrate;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_G729 || defined _USC_CODECS || defined _USC_ALL)
   if((vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729"))==0)    ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729A"))==0)   ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729_FP"))==0) ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729A_FP"))==0)) {
      if(direction==1) {
         *pPck = new UMC::G729Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G729DePacketizer;
         UMC::G729DePacketizerParams pDePckPrms;
         pDePckPrms.m_ptType = UMC::G729;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   } else if((vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729D"))==0) ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729D_FP"))==0)) {
      if(direction==1) {
         *pPck = new UMC::G729Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G729DePacketizer;
         UMC::G729DePacketizerParams pDePckPrms;
         pDePckPrms.m_ptType = UMC::G729D;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   } else if((vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729E"))==0) ||
      (vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G729E_FP"))==0)) {
      if(direction==1) {
         *pPck = new UMC::G729Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G729DePacketizer;
         UMC::G729DePacketizerParams pDePckPrms;
         pDePckPrms.m_ptType = UMC::G729E;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_GSMFR || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_GSMFR"))==0) {
      if(direction==1) {
         *pPck = new UMC::GSMPacketizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::GSMDePacketizer;
         UMC::RTPBaseDePacketizerParams pDePckPrms;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_GSMFR || defined _USC_CODECS || defined _USC_ALL)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_G722"))==0) {
      if(direction==1) {
         *pPck = new UMC::G722Packetizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::G722DePacketizer;
         UMC::RTPBaseDePacketizerParams pDePckPrms;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_ILBC)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_iLBC"))==0) {
      if(direction==1) {
         *pPck = new UMC::iLBCPacketizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::iLBCDePacketizer;
         UMC::iLBCDePacketizerParams pDePckPrms;
         if(bitrate==13330) {
            pDePckPrms.m_mode = 30;
         } else { /*15200*/
            pDePckPrms.m_mode = 20;
         }
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif
   #if (defined _USC_MSRTA)
   if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAwb_FP"))==0 ||
      vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAnb_FP"))==0) {
      if(direction==1) {
         *pPck = new UMC::RTAPacketizer;
         UMC::RTPBasePacketizerParams pPckPrms;
         if((UMCret = (*pPck)->Init(&pPckPrms)) != UMC::UMC_OK) return UMCret;
      } else {
         *pDePck = new UMC::RTADePacketizer;
         UMC::RTADePacketizerParams pDePckPrms;
         pDePckPrms.m_Bitrate = bitrate;
         if((UMCret = (*pDePck)->Init(&pDePckPrms)) != UMC::UMC_OK) return UMCret;
      }
   }
   #endif

   return UMC::UMC_OK;
}

UMC::MediaBuffer* USC_CodecContainer::CreateJBuffer(Ipp32s direction)
{
   if(direction==1) {
      return NULL;
   }
   #if (defined _USC_MSRTA)
      if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAwb_FP"))==0 ||
          vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAnb_FP"))==0) {
         return new UMC::RTAJBuffer;
      }
   #endif
   return new UMC::SpeechJBuffer;
}

BaseTimer* USC_CodecContainer::CreatePlayoutTimer(Ipp32s direction)
{
   if(direction==1) {
      return NULL;
   }
   #if (defined _USC_MSRTA)
      if(vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAwb_FP"))==0 ||
          vm_string_strcmp((const vm_char*)m_pCodecName,VM_STRING("IPP_MSRTAnb_FP"))==0) {
         BaseTimer* timer = new SampleTimer;
         timer->SetTimeInc(10);
         return timer;
      }
   #endif
   return new FreeTimer;
}

UMC::RTP_PayloadType * USC_CodecContainer::GetPayloadType()
{
   UMC::RTP_PayloadType *pPT = NULL;
   Ipp32s i=0;
   if(m_Inited) {
      while(vm_string_strcmp(LinkedCodecs[i].codecName,VM_STRING("END"))!=0) {
         if(vm_string_strcmp((const vm_char*)m_pCodecName, LinkedCodecs[i].codecName) == 0) {
            pPT = LinkedCodecs[i].PTptr;
         }
         i++;
      }
   }
   return pPT;
}
