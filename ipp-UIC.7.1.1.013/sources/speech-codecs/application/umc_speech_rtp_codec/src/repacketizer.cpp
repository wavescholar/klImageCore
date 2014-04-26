//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//

#include "repacketizer.h"

UMC::Status Repacker::UnPackToUSC(UMC::MediaData *pIn, UMC::LinkedList<UMC::SpeechData *> *pOut)
{
   UMC::SpeechData tmpData(MAX_UDP_PCKT_LEN);
   m_pDepacketizer->SetPacket(pIn);

   while(UMC::UMC_ERR_NOT_ENOUGH_DATA != m_pDepacketizer->GetNextFrame(&tmpData)) {
      UMC::SpeechData *newData = new UMC::SpeechData(tmpData.GetNBytes());
      tmpData.MoveDataTo(newData);
      pOut->Add(newData);
   }

   return UMC::UMC_OK;
}

UMC::Status Repacker::PackToRTP(UMC::LinkedList<UMC::SpeechData *> *pIn, UMC::MediaData *pOut)
{
   Ipp32u i;

   for(i=0;i < pIn->Size();i++) {
      UMC::SpeechData *pIt;
      pIn->Get(pIt,i);
      m_pPacketizer->AddFrame(pIt);
      if(i==0) pOut->SetTime(pIt->GetTime());
   }
   m_pPacketizer->GetPacket(pOut);
   return UMC::UMC_OK;
}
