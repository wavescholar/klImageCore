/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2008-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, advance profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_adv.h"
#include "umc_structures.h"
#include "umc_video_data.h"
#include "umc_vc1_enc_debug.h"
#include "umc_video_data_scene_info.h"
#include "umc_vc1_common_enc_tables.h"

namespace UMC_VC1_ENCODER
{
static UMC::FrameType GetUMCFrameType(ePType VC1Type)
{
    switch (VC1Type)
    {
    case VC1_ENC_I_FRAME:
    case VC1_ENC_I_I_FIELD:    
        return UMC::I_PICTURE;
    case VC1_ENC_P_FRAME:    
    case VC1_ENC_P_FRAME_MIXED:
    case VC1_ENC_SKIP_FRAME:
    case VC1_ENC_I_P_FIELD:
    case VC1_ENC_P_I_FIELD:
    case VC1_ENC_P_P_FIELD:
        return UMC::P_PICTURE;
    case VC1_ENC_B_FRAME:
    case VC1_ENC_BI_FRAME:
    case VC1_ENC_B_B_FIELD:
    case VC1_ENC_B_BI_FIELD:
    case VC1_ENC_BI_B_FIELD:
    case VC1_ENC_BI_BI_FIELD:
        return UMC::B_PICTURE;
    default:
        return UMC::NONE_PICTURE;
    }
}
UMC::Status  VC1EncoderADV::Init(UMC::VC1EncoderParams* pParams)
{
    UMC::Status     err = UMC::UMC_OK;
    Ipp32u          w=0, h=0;
    Ipp32s          memSize = 0;

    if(!m_pMemoryAllocator)
        return UMC::UMC_ERR_INIT;

    Close();

    m_bMixedMV      = pParams->m_bMixed;

    m_uiGOPLength   = (pParams->m_uiGOPLength!=0)?
        pParams->m_uiGOPLength:1;
    m_uiBFrmLength  = (pParams->m_uiBFrmLength < m_uiGOPLength)?
        pParams->m_uiBFrmLength:m_uiGOPLength-1;

    m_bSequenceHeader       = false;
    m_iFrameCount           = 0;
    m_uiPictuteQuantIndex   = 7;
    m_bHalfQuant            = false;
    m_bFrameRecoding        = pParams->m_bFrameRecoding;
    m_bFastUVMC             = pParams->m_bFastUVMC;

    m_uiOrigFramesUsingFlag    = (pParams->m_bOrigFramePred) ? VC1_ENC_ORIG_FRAMES_FOR_ME|VC1_ENC_ORIG_FRAMES_FOR_SKIP : 0; //  0 - Original (non-reconstructed frames aren't  used)
                                    //  (m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_ME): originalare used for ME
                                    //  (m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_SKIP): originalare used for skip frames detection

    if(pParams->m_bUseUpdateMeFeedback)
        m_uiOrigFramesUsingFlag |= VC1_ENC_ORIG_FRAMES_FOR_STAT;

    m_SH = new VC1EncoderSequenceADV;
    if (!m_SH)
        return UMC::UMC_ERR_ALLOC;

    err = m_SH->Init(pParams);
    if (err != UMC::UMC_OK)
        return err;

    err = m_SH->CheckParameters(m_cLastError);
    if (err != UMC::UMC_OK)
        return err;

    switch(pParams->m_uiReferenceFieldType)
    {
    case 0:
        m_uiReferenceFieldType = VC1_ENC_REF_FIELD_FIRST;
        break;
    case 1:
        m_uiReferenceFieldType = VC1_ENC_REF_FIELD_SECOND;
        break;
    case 2:
        m_uiReferenceFieldType = VC1_ENC_REF_FIELD_BOTH;
        break;
    }

    h = (m_SH->IsInterlace())?
        (((m_SH->GetPictureHeight()/2)+15)/16)*2:((m_SH->GetPictureHeight()+15)/16);
    w = (m_SH->GetPictureWidth() +15)/16;
    //------------Planes for decoding -------------------------------

    m_pStoredFrames  =  new StoredFrames;
    if (!m_pStoredFrames)
        return UMC::UMC_ERR_ALLOC;

    m_pGOP = new GOP;
    if (!m_pGOP)
        return UMC::UMC_ERR_ALLOC;

    m_pWaitingList = new WaitingList;
    if (!m_pWaitingList)
        return UMC::UMC_ERR_ALLOC;

    memSize = StoredFrames::CalcAllocatedMemSize(m_uiBFrmLength +((m_uiOrigFramesUsingFlag)?4:2),
        pParams->m_info.videoInfo.m_iWidth, pParams->m_info.videoInfo.m_iHeight, 32);

    if(m_pMemoryAllocator->Alloc(&m_StoredFramesID, memSize,
                    UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
    return UMC::UMC_ERR_ALLOC;
    m_pStoredFramesBuffer = (Ipp8u*)m_pMemoryAllocator->Lock(m_StoredFramesID);

    err = m_pStoredFrames->Init(m_pStoredFramesBuffer, memSize,
                                m_uiBFrmLength +((m_uiOrigFramesUsingFlag)?4:2),
                                pParams->m_info.videoInfo.m_iWidth, pParams->m_info.videoInfo.m_iHeight, 32);
    if (err !=UMC::UMC_OK)
            return err;

    err =m_pGOP->Init(m_uiBFrmLength);
    if (err !=UMC::UMC_OK)
            return err;

    err =m_pWaitingList->Init(m_uiBFrmLength);
    if (err !=UMC::UMC_OK)
            return err;

    if (m_uiBFrmLength)
    {
        /* we should save MV for direct prediction in B frames*/
        if(m_pMemoryAllocator->Alloc(&m_SavedMVID, w*h*2*sizeof(Ipp16s),
                        UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
        return UMC::UMC_ERR_ALLOC;
        m_pSavedMV = (Ipp16s*)m_pMemoryAllocator->Lock(m_SavedMVID);

        /* we should save MV for direct prediction in B frames*/
        if(m_pMemoryAllocator->Alloc(&m_RefTypeID, w*h*sizeof(Ipp8u),
                        UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
        return UMC::UMC_ERR_ALLOC;
         m_pRefType = (Ipp8u*)m_pMemoryAllocator->Lock(m_RefTypeID);
    }

    //---------------MBs-----------------------------------------------

    m_pMBs = new VC1EncoderMBs;
    if (!m_pMBs)
        return UMC::UMC_ERR_ALLOC;

    memSize = VC1EncoderMBs::CalcAllocMemorySize(w,2);
    if(m_pMemoryAllocator->Alloc(&m_MBsID, memSize,
                    UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
    return UMC::UMC_ERR_ALLOC;
    m_MBsBuffer = (Ipp8u*)m_pMemoryAllocator->Lock(m_MBsID);

    err = m_pMBs->Init(m_MBsBuffer, memSize, w, 2);
    if (err != UMC::UMC_OK)
        return err;

    m_pCodedMB =  new VC1EncoderCodedMB [w*h];
    if (!m_pCodedMB)
        return UMC::UMC_ERR_ALLOC;


    m_pCurrPicture = new VC1EncoderPictureADV;
    if (!m_pCurrPicture)
        return UMC::UMC_ERR_ALLOC;

    m_pCurrPicture->Init(m_SH, m_pMBs, m_pCodedMB);

    //---------------Motion estimation
    m_bUseMeFeedback = pParams->m_bUseMeFeedback;
    m_bUseUpdateMeFeedback = pParams->m_bUseUpdateMeFeedback;
    m_bUseFastMeFeedback   = pParams->m_bUseFastMeFeedback;

    m_bSelectVLCTables         = pParams->m_bSelectVLCTables;
    m_bChangeInterpPixelType   = pParams->m_bChangeInterpPixelType;
    m_bUseTreillisQuantization = pParams->m_bUseTreillisQuantization;

#ifndef VC1_ME_MB_STATICTICS
    if(m_uiGOPLength > 1)
#endif
    {
        UMC::MeInitParams MEParamsInit;
        memset(&MEParamsInit,0,sizeof(MEParamsInit));

        if(!pParams->m_bInterlace)
        {
            MEParamsInit.MaxNumOfFrame = 3;
        }
        else
        {
            MEParamsInit.MaxNumOfFrame = 5;
        }

        MEParamsInit.WidthMB    = (pParams->m_info.videoInfo.m_iWidth+15)/16;
        MEParamsInit.HeightMB   = (pParams->m_info.videoInfo.m_iHeight+15)/16;
        MEParamsInit.refPadding = 32;
        MEParamsInit.MbPart     = UMC::ME_Mb16x16;
        //MEParamsInit.CostMetrics     = UMC::ME_Sad;

#ifndef VC1_ME_MB_STATICTICS
        MEParamsInit.UseStatistics   = true;
#endif
        MEParamsInit.UseDownSampling = true;

        if(m_uiBFrmLength)
            MEParamsInit.SearchDirection = UMC::ME_BidirSearch;
        else
            MEParamsInit.SearchDirection = UMC::ME_ForwardSearch;

            m_pME = new UMC::MeVC1;
#ifdef ME_VME
            m_pME = (UMC::MeBase*)new UMC::MeVme;
#endif

        if(!m_pME)
            return err;

        if(!m_pME->Init(&MEParamsInit, NULL, memSize))
            return UMC::UMC_ERR_INIT;
        if(m_pMemoryAllocator->Alloc(&m_MEID, memSize,UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
            return UMC::UMC_ERR_ALLOC;
        m_pMEBuffer = (Ipp8u*)m_pMemoryAllocator->Lock(m_MEID);
        if(!m_pME->Init(&MEParamsInit, m_pMEBuffer, memSize))
            return UMC::UMC_ERR_INIT;

        m_MESearchSpeed = pParams->m_uiMESearchSpeed;

        Ipp32s numRefFrame = MEParamsInit.MaxNumOfFrame;
        for(Ipp32s i = 0; i < numRefFrame; i++)
        {
            m_MeFrame[i] = &MEParamsInit.pFrames[i];
        }
    }

    //--------------Bitrate-------------------------------------------
    m_pBitRateControl = new VC1BitRateControl();
    if (!m_pBitRateControl)
       return UMC::UMC_ERR_ALLOC;

    memSize = VC1BitRateControl::CalcAllocMemorySize(m_uiGOPLength, m_uiBFrmLength);

    if(m_pMemoryAllocator->Alloc(&m_BRCID, memSize,
                    UMC::UMC_ALLOC_PERSISTENT, 16) != UMC::UMC_OK )
    return UMC::UMC_ERR_ALLOC;

    m_pBRCBuffer = (Ipp8u*)m_pMemoryAllocator->Lock(m_BRCID);

    Ipp8u QuantMode = 3;

    switch (m_SH->GetQuantType())
    {
    case VC1_ENC_QTYPE_IMPL:
        QuantMode = 3;
        break;
    case VC1_ENC_QTYPE_UF:
        QuantMode = 1;
        break;
    case VC1_ENC_QTYPE_NUF:
       QuantMode = 2;
        break;
    }

    err = m_pBitRateControl->Init(m_pBRCBuffer, memSize, (pParams->m_info.videoInfo.m_iHeight*pParams->m_info.videoInfo.m_iWidth*3)/2,
            pParams->m_info.iBitrate, pParams->m_info.fFramerate, VC1_BRC_HIGHT_QUALITY_MODE,
            m_uiGOPLength, m_uiBFrmLength, pParams->m_iConstQuant, QuantMode);
        if (err != UMC::UMC_OK)
            return err;

    //if (buffer number < 0) InitBuffer function will set corresponding number and
    //initialize it as new buffer
    //if it is number of existing buffer, it will change buffer params
    //if buffer size <=0 or > maxsize, buffer size will be equal to buffer size

    err = m_pBitRateControl->InitBuffer(m_SH->GetProfile(),m_SH->GetLevel(),
        pParams->m_uiHRDBufferSize, pParams->m_uiHRDBufferInitFullness);
    if (err != UMC::UMC_OK)
        return err;

    m_SH->SetLevel(m_pBitRateControl->GetLevel(m_SH->GetProfile(),pParams->m_info.iBitrate, w, h));
    ///////////////////////

    //-----------Bit stream ----------------------------------------------------

    m_pCodedFrame = new VC1EncoderBitStreamAdv;
    if (!m_pCodedFrame)
        return UMC::UMC_ERR_ALLOC;

    if(pParams->m_bSceneAnalyzer)
    {
        UMC::SceneAnalyzerParams saParams;
        saParams.m_maxGOPLength = m_uiGOPLength;
        saParams.m_maxBLength   = m_uiBFrmLength;

        if(pParams->m_bInterlace)
            saParams.m_interlaceType = UMC::PS_TOP_FIELD_FIRST;
        else
            saParams.m_interlaceType = UMC::PS_PROGRESSIVE;

        m_pSceneAnalyzer = new UMC::SceneAnalyzer();

        if (!m_pSceneAnalyzer)
            return UMC::UMC_ERR_ALLOC;

       err =  m_pSceneAnalyzer->Init(&saParams);
        if (err != UMC::UMC_OK)
            return err;
    }

#ifdef VC1_ENC_DEBUG_ON
    pDebug = new VC1EncDebug;
    assert(pDebug != NULL);
    pDebug->Init(w, h);
#endif

    return UMC::UMC_OK;
}
UMC::Status VC1EncoderADV::Close()
{
    for(Ipp32u i = 0; i < 32; i++)
        m_MeFrame[i] = NULL;

    m_MeIndex.MeCurrIndex = 0;
    m_MeIndex.MeRefFIndex = 0;
    m_MeIndex.MeRefBIndex = 0;

    ResetPlanes();

    if  (m_SH)
    {
       delete m_SH;
       m_SH = 0;
    }
    if (m_pCurrPicture)
    {
        delete m_pCurrPicture;
        m_pCurrPicture = 0;
    }

    if(m_pCodedFrame)
    {
        delete m_pCodedFrame;
        m_pCodedFrame = NULL;
    }

    if(m_pBitRateControl)
    {
        delete m_pBitRateControl;
        m_pBitRateControl = NULL;
    }

    if(m_pSceneAnalyzer)
    {
        delete m_pSceneAnalyzer;
        m_pSceneAnalyzer = NULL;
    }

    if (m_pWaitingList)
    {
        delete m_pWaitingList;
        m_pWaitingList = 0;
    }
    if (m_pGOP)
    {
        delete m_pGOP;
        m_pGOP = 0;
    }

    if (m_pStoredFrames)
    {
        delete m_pStoredFrames;
        m_pStoredFrames = 0;
    }

    if(m_pMemoryAllocator)
    {
        m_pMemoryAllocator->Unlock(m_StoredFramesID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_StoredFramesID);
        }
    }
    m_StoredFramesID = (UMC::MemID)-1;
    m_pStoredFramesBuffer = NULL;

   if(m_pMemoryAllocator)
   {
        m_pMemoryAllocator->Unlock(m_SavedMVID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_SavedMVID);
        }
   }
   m_SavedMVID = (UMC::MemID)-1;
   m_pSavedMV = 0;

   if(m_pMemoryAllocator)
   {
        m_pMemoryAllocator->Unlock(m_RefTypeID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_RefTypeID);
        }
   }
   m_RefTypeID = (UMC::MemID)-1;
   m_pRefType = 0;

   if(m_pMemoryAllocator)
   {
        m_pMemoryAllocator->Unlock(m_MBsID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_MBsID);
        }
   }
   m_MBsID = (UMC::MemID)-1;
   m_MBsBuffer = 0;

   if(m_pMBs)
   {
       delete m_pMBs;
       m_pMBs = NULL;
   }

   if(m_pMemoryAllocator)
   {
        m_pMemoryAllocator->Unlock(m_BRCID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_BRCID);
        }
   }
   m_BRCID = (UMC::MemID)-1;
   m_pBRCBuffer = 0;


    if(m_pCodedMB)
    {
        delete [] m_pCodedMB;
        m_pCodedMB = 0;
    }


    if(m_pMemoryAllocator)
    {
        m_pMemoryAllocator->Unlock(m_MEID);
        if(m_bOwnAllocator == true)
        {
            m_pMemoryAllocator->Free(m_MEID);
        }
    }
    m_MEID = (UMC::MemID)-1;
    m_pMEBuffer = NULL;

    if(m_pME)
    {
        delete m_pME;
        m_pME = NULL;
    }

#ifdef VC1_ENC_DEBUG_ON
    if(pDebug)
    {
        delete pDebug;
        pDebug = NULL;
    }
#endif

    return UMC::UMC_OK;
}

UMC::Status VC1EncoderADV::GetFrame(UMC::MediaData *in, UMC::MediaData *out)
{
    UMC::Status         err             =  UMC::UMC_ERR_NOT_ENOUGH_DATA;
    UMC::VideoData*     pVideoData      = (in)? DynamicCast<UMC::VideoData, UMC::MediaData>(in):0;

    //double            time            = 0;
    Frame*              inFrame         = 0;
    Frame*              outFrame        = 0;
    bool                stored          = false;
    UMC::VideoData           dst; //for scene analyzer

    ePType              inputPictureType = VC1_ENC_I_FRAME;

    //only for debug
//    static int z_z = 0;

    ResetPlanes();

    if(m_pSceneAnalyzer && (pVideoData == NULL || pVideoData->m_frameType == UMC::NONE_PICTURE))
    {
        //scene analyzer
        err = m_pSceneAnalyzer->GetFrame(pVideoData, &dst);
        // usual case. we have something to encode
        if (UMC::UMC_OK == err)
        {
            pVideoData      = &dst;
        }
        // it can be the beginning either ending of the stream.
        else if (err == UMC::UMC_ERR_NOT_ENOUGH_DATA)
        {
            if (pVideoData)
            {
                return err;
            }
        }
        else
        {
            return err;
        }
    }
    /*------------------------------------------------------------------------------------------------------------*/
    if (pVideoData )
    {
        switch (pVideoData->m_frameType)
        {
        case UMC::I_PICTURE:
            //printf("I frame\n");
            if(pVideoData->m_picStructure != UMC::PS_PROGRESSIVE && m_SH->IsInterlace())
                inputPictureType = VC1_ENC_I_I_FIELD;
            else
                inputPictureType = VC1_ENC_I_FRAME;
            break;
        case UMC::P_PICTURE:
            //printf("P frame\n");
            if(pVideoData->m_picStructure != UMC::PS_PROGRESSIVE && m_SH->IsInterlace())
                inputPictureType = VC1_ENC_P_P_FIELD;
            else
                inputPictureType = VC1_ENC_P_FRAME;

            if (VC1_ENC_P_FRAME == inputPictureType && m_bMixedMV )
                   inputPictureType = VC1_ENC_P_FRAME_MIXED;
            break;
        case UMC::B_PICTURE:
            //printf("B frame\n");
            if(pVideoData->m_picStructure != UMC::PS_PROGRESSIVE && m_SH->IsInterlace())
                inputPictureType = VC1_ENC_B_B_FIELD;
            else
                inputPictureType = VC1_ENC_B_FRAME;
            break;
        default:
            inputPictureType = m_pGOP->GetPictureType(m_iFrameCount,m_uiGOPLength,m_uiBFrmLength);

            //printf("%d frame\n", inputPictureType);

            if (VC1_ENC_P_FRAME == inputPictureType && m_bMixedMV )
               inputPictureType = VC1_ENC_P_FRAME_MIXED;
            if (m_SH->IsInterlace() && inputPictureType == VC1_ENC_I_FRAME)
            {
               inputPictureType = VC1_ENC_I_I_FIELD;
            }
            else if (m_SH->IsInterlace() && (inputPictureType == VC1_ENC_P_FRAME || inputPictureType == VC1_ENC_P_FRAME_MIXED))
            {
               inputPictureType = VC1_ENC_P_P_FIELD;
            }
            else if (m_SH->IsInterlace() && inputPictureType == VC1_ENC_B_FRAME)
            {
               inputPictureType = VC1_ENC_B_B_FIELD;
            }

            break;
        };

       m_iFrameCount++;

       inFrame = m_pStoredFrames->GetFreeFramePointer();
       if (inFrame)
       {
            inFrame->CopyPlane( (Ipp8u*)pVideoData->GetPlaneDataPtr(0), (Ipp32u)pVideoData->GetPlanePitch(0),
                                 (Ipp8u*)pVideoData->GetPlaneDataPtr(1), (Ipp32u)pVideoData->GetPlanePitch(1),
                                 (Ipp8u*)pVideoData->GetPlaneDataPtr(2), (Ipp32u)pVideoData->GetPlanePitch(2),
                                 inputPictureType);

            if (!m_pWaitingList->AddFrame(inFrame))
                return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
            pVideoData->SetDataSize(0);
            stored = true;
       }
    }

    inFrame = 0;
    while ((inFrame = m_pWaitingList->GetCurrFrame())!=0)
    {
        outFrame = (m_uiOrigFramesUsingFlag)?m_pStoredFrames->GetFreeFramePointer():0;
        if (m_pGOP->isSkipFrame(inFrame,(m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_SKIP)!=0,m_LastQuant))
        {
           inFrame->SetType(VC1_ENC_SKIP_FRAME);
           if (m_uiOrigFramesUsingFlag)
           {
                outFrame->CopyPlane(inFrame);
           }
        }
        if (!m_pGOP->AddFrame(inFrame,outFrame))
            break;
        m_pWaitingList->MoveOnNextFrame();
    }

    /*------------------------------------------------------------------------------------------------------------*/

    if (!pVideoData)
    {
        m_pGOP->CloseGop((m_SH->IsInterlace())?VC1_ENC_I_I_FIELD:VC1_ENC_I_FRAME);
    }

    inFrame  = m_pGOP->GetInFrameForDecoding();
    outFrame = m_pGOP->GetOutFrameForDecoding();

    if (inFrame && outFrame)
    {
        Frame* pFrameRef = 0;
        m_pCodedFrame->Init(out);
        if (!m_bSequenceHeader)
        {
            err = m_SH->WriteSeqHeader(m_pCodedFrame);
            if (err != UMC::UMC_OK)
                return err;
            m_bSequenceHeader = true;
        }

        err = m_pCurrPicture->Init(m_SH,m_pMBs,m_pCodedMB);
        if (err != UMC::UMC_OK)
            return err;

        // only for advance profile
        if (m_uiOrigFramesUsingFlag)
        {
            if(!IsFieldPicture(inFrame->GetPictureType()))
                err = inFrame->PadFrameProgressive();
            else
                err = inFrame->PadFrameField();
            if (err != UMC::UMC_OK)
                return err;
        }
        else
        {
            if(!IsFieldPicture(inFrame->GetPictureType()))
                err = inFrame->PadPlaneProgressive();
            else
                err = inFrame->PadPlaneField();
        }

#ifdef VC1_BRC_DEBUG
        printf("PicType = %d\n", inFrame->GetPictureType());
#endif

        if (inFrame->GetPictureType()!= VC1_ENC_SKIP_FRAME)
        {
            m_pPlane = inFrame;

            if ((inFrame->isReferenceFrame() ||  IsFieldPicture(inFrame->GetPictureType()))&& m_uiGOPLength>1)
            {
                m_pRaisedPlane = outFrame;
            }
            if (!inFrame->isIntraFrame())
            {
                pFrameRef = m_pGOP->GetOutReferenceFrame();
                if (!pFrameRef)
                    return UMC::UMC_ERR_FAILED;

                m_pForwardPlane = pFrameRef;
                if (m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_ME)
                {
                    pFrameRef = m_pGOP->GetInReferenceFrame();
                }

                m_pForwardMEPlane = pFrameRef;
            }

            if (!inFrame->isReferenceFrame())
            {
                pFrameRef = m_pGOP->GetOutReferenceFrame(true);
                if (!pFrameRef)
                    return UMC::UMC_ERR_FAILED;

                m_pBackwardPlane = pFrameRef;

                if (m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_ME)
                {
                    pFrameRef = m_pGOP->GetInReferenceFrame(true);
                }

                m_pBackwardMEPlane = pFrameRef;
                m_MeIndex.MeCurrIndex = 2;
            }
            else
            {
                m_MeIndex.MeRefBIndex = m_MeIndex.MeRefFIndex;
                m_MeIndex.MeCurrIndex = m_MeIndex.MeRefFIndex;
                m_MeIndex.MeRefFIndex = 1 - m_MeIndex.MeRefFIndex;
            }

#ifdef VC1_BRC_DEBUG
            printf("PicType = %d\n", inFrame->GetPictureType());
#endif
        }

        Ipp32u CodedSize = m_pCodedFrame->GetDataLen();


        err = WriteFrame(inFrame->GetPictureType(), false, CodedSize);
        if (err != UMC::UMC_OK)
            return err;

        if(IsFieldPicture(inFrame->GetPictureType()))
            inFrame->PadField(false);

        err = WriteFrame(inFrame->GetPictureType(), true, CodedSize);
        if (err != UMC::UMC_OK)
            return err;

        err = m_pCodedFrame->AddLastBits();
        if (err != UMC::UMC_OK)
            return err;

#ifdef VC1_ENC_DEBUG_ON
        //debug
        pDebug->SetPicType(inFrame->GetPictureType());
        pDebug->SetFrameSize(m_pCodedFrame->GetDataLen() - CodedSize);
        pDebug->WriteFrameInfo();
#endif
        out->m_frameType = GetUMCFrameType(inFrame->GetPictureType());

        err = m_pCodedFrame->DataComplete(out);
        if (err != UMC::UMC_OK)
            return err;

        if (outFrame->isReferenceFrame() && m_uiGOPLength>1)
        {
            if(!IsFieldPicture(outFrame->GetPictureType()))
                err = outFrame->PadFrameProgressive();
            else
                err = outFrame->PadFrameField();
            if (err != UMC::UMC_OK)
                return err;
            if (m_uiOrigFramesUsingFlag)
            {
                if(!IsFieldPicture(inFrame->GetPictureType()))
                    err = inFrame->PadFrameProgressive();
                else
                    err = inFrame->PadFrameField();
                if (err != UMC::UMC_OK)
                    return err;
            }
        }

        m_pGOP->ReleaseCurrentFrame();
    }
    else if (!pVideoData)
    {
        return UMC::UMC_ERR_END_OF_STREAM;
    }

    /*------------------------------------------------------------------------------------------------------------*/

    if (pVideoData && !stored)
    {
        inFrame = m_pStoredFrames->GetFreeFramePointer();
        if (!inFrame)
            return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

        inFrame->CopyPlane( (Ipp8u*)pVideoData->GetPlaneDataPtr(0), (Ipp32u)pVideoData->GetPlanePitch(0),
                            (Ipp8u*)pVideoData->GetPlaneDataPtr(1), (Ipp32u)pVideoData->GetPlanePitch(1),
                            (Ipp8u*)pVideoData->GetPlaneDataPtr(2), (Ipp32u)pVideoData->GetPlanePitch(2),
                             inputPictureType);

        if (!m_pWaitingList->AddFrame(inFrame))
            return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
         pVideoData->SetDataSize(0);
    }
    while ((inFrame = m_pWaitingList->GetCurrFrame())!=0)
    {
        if (m_pGOP->isSkipFrame(inFrame,(m_uiOrigFramesUsingFlag & VC1_ENC_ORIG_FRAMES_FOR_SKIP)!=0,m_LastQuant))
        {
           inFrame->SetType(VC1_ENC_SKIP_FRAME);
           if (m_uiOrigFramesUsingFlag)
           {
                outFrame->CopyPlane(inFrame);
           }
        }
        outFrame = (m_uiOrigFramesUsingFlag)? m_pStoredFrames->GetFreeFramePointer():0;
        if (!m_pGOP->AddFrame(inFrame,outFrame))
            break;
        m_pWaitingList->MoveOnNextFrame();
    }
   /*------------------------------------------------------------------------------------------------------------*/

    return err;
}

UMC::Status VC1EncoderADV::WriteFrame(ePType InputPictureType, bool bSecondField, Ipp32u CodedSize)
{
    UMC::Status   err = UMC::UMC_OK;
    UMC::MeParams MEParams;
    Ipp8u uiQuantIndex = 31;
    bool  bHalfQuant = 0;
    bool  bUniform = true;
    Ipp32s RecodedFrameNum = 0;
    bool   frame_recoding  = true;
    bool   bFieldPicture = IsFieldPicture(InputPictureType);

    if(!bFieldPicture && bSecondField)
        return UMC::UMC_OK;

     while(frame_recoding && (RecodedFrameNum < VC1_ENC_RECODING_MAX_NUM))
     {
        //-----BRC---------------
        m_pBitRateControl->GetQuant(InputPictureType, &uiQuantIndex, &bHalfQuant, &bUniform);

        //set encoder params
        SetInitPictureParams(InputPictureType);
        SetVLCTablesIndex();

        STATISTICS_START_TIME(m_TStat->me_StartTime);
        err = SetMEParams(&MEParams, uiQuantIndex*2 + bHalfQuant, bUniform, bFieldPicture);
        VC1_ENC_CHECK(err);
        //----ME---------------
        switch (InputPictureType)
        {
        case VC1_ENC_I_FRAME:
            #ifdef VC1_ME_MB_STATICTICS
            err = SetMEParams_I(&MEParams);
            VC1_ENC_CHECK(err);
            #endif
            break;
        case VC1_ENC_P_FRAME:
            err = SetMEParams_P(&MEParams);
            VC1_ENC_CHECK(err);

            err = Enc_PFrame(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_P_FRAME_MIXED:
            err = SetMEParams_PMixed(&MEParams);
            VC1_ENC_CHECK(err);

            err = Enc_PFrameMixed(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_B_FRAME:
            err = SetMEParams_B(&MEParams);
            VC1_ENC_CHECK(err);

            err = Enc_BFrame(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_SKIP_FRAME:
            break;
        case VC1_ENC_I_I_FIELD:
            #ifdef VC1_ME_MB_STATICTICS
            err = SetMEParams_I(&MEParams);
            VC1_ENC_CHECK(err);
            #endif
            break;
        case VC1_ENC_I_P_FIELD:
            if(!bSecondField)
            {
                #ifdef VC1_ME_MB_STATICTICS
                err = SetMEParams_I_Field(&MEParams);
                VC1_ENC_CHECK(err);
                #endif
            }
            else
            {
                err = SetMEParams_P_Field(&MEParams, true, bSecondField);
                VC1_ENC_CHECK(err);

                err = Enc_P_Field(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_P_I_FIELD:
            if(!bSecondField)
            {
                err = SetMEParams_P_Field(&MEParams, true, bSecondField);
                VC1_ENC_CHECK(err);

                err = Enc_P_Field(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
            }
            else
            {
                #ifdef VC1_ME_MB_STATICTICS
                err = SetMEParams_I_Field(&MEParams);
                VC1_ENC_CHECK(err);
                #endif
            }
             break;
        case VC1_ENC_P_P_FIELD:
            err = SetMEParams_P_Field(&MEParams, true, bSecondField);
            VC1_ENC_CHECK(err);

            err = Enc_P_Field(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_BI_B_FIELD:
            if(!bSecondField)
            {
                #ifdef VC1_ME_MB_STATICTICS
                err = SetMEParams_I_Field(&MEParams);
                VC1_ENC_CHECK(err);
                #endif
            }
            else
            {
                err = SetMEParams_B_Field(&MEParams, true, bSecondField);
                VC1_ENC_CHECK(err);

                err = Enc_B_Field(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_B_BI_FIELD:
            if(!bSecondField)
            {
                err = SetMEParams_B_Field(&MEParams, true, bSecondField);
                VC1_ENC_CHECK(err);

                err = Enc_B_Field(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
            }
            else
            {
                #ifdef VC1_ME_MB_STATICTICS
                err = SetMEParams_I_Field(&MEParams);
                VC1_ENC_CHECK(err);
                #endif
            }
             break;
        case VC1_ENC_B_B_FIELD:
            err = SetMEParams_B_Field(&MEParams, true, bSecondField);
            VC1_ENC_CHECK(err);

            err = Enc_B_Field(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
         default:
            return UMC::UMC_ERR_FAILED;
        }
    STATISTICS_END_TIME(m_TStat->me_StartTime, m_TStat->me_EndTime, m_TStat->me_TotalTime);

        err = MEPictureParamsRefine(&MEParams);
        VC1_ENC_CHECK(err);

        //set pictire params
        m_pCurrPicture->SetInitPictureParams(&m_InitPicParam);
        m_pCurrPicture->SetVLCTablesIndex(&m_VLCIndex);

        err = m_pCurrPicture->SetPictureParams(InputPictureType, m_pSavedMV, m_pRefType, bSecondField);
        VC1_ENC_CHECK(err);

        err = m_pCurrPicture->SetPictureQuantParams(uiQuantIndex, bHalfQuant);
        VC1_ENC_CHECK(err);

        err = m_pCurrPicture->CheckParameters(m_cLastError,bSecondField);
        VC1_ENC_CHECK(err);

        if(m_pPlane)
        {
            err = m_pCurrPicture->SetPlaneParams (m_pPlane, VC1_ENC_CURR_PLANE);
            VC1_ENC_CHECK(err);
        }

        if(m_pRaisedPlane)
        {
            err = m_pCurrPicture->SetPlaneParams  (m_pRaisedPlane, VC1_ENC_RAISED_PLANE);
            VC1_ENC_CHECK(err);
        }

        if(m_pForwardPlane)
        {
            err = m_pCurrPicture->SetPlaneParams  (m_pForwardPlane, VC1_ENC_FORWARD_PLANE);
            VC1_ENC_CHECK(err);
        }

        if(m_pBackwardPlane)
        {
            err = m_pCurrPicture->SetPlaneParams  (m_pBackwardPlane, VC1_ENC_BACKWARD_PLANE);
            VC1_ENC_CHECK(err);
        }

        //----PAC---------------
        #ifdef VC1_ME_MB_STATICTICS
        m_pCurrPicture->SetMEStat(MEParams.pSrc->stat, bSecondField);
        #endif
        switch (InputPictureType)
        {
        case VC1_ENC_I_FRAME:
            {
                //----coding---------------
                err = m_pCurrPicture->PAC_IFrame(&MEParams);
                VC1_ENC_CHECK(err);

                //----VLC---------------
                err = m_pCurrPicture->VLC_IFrame(m_pCodedFrame);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_P_FRAME:
            {
                //----coding---------------
                err = m_pCurrPicture->PAC_PFrame(&MEParams);
                VC1_ENC_CHECK(err);

                //----VLC---------------
                err = m_pCurrPicture->VLC_PFrame(m_pCodedFrame);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_P_FRAME_MIXED:
            {
                //----coding---------------
                err = m_pCurrPicture->PAC_PFrameMixed(&MEParams);
                VC1_ENC_CHECK(err);

                //----VLC---------------
                err = m_pCurrPicture->VLC_PFrameMixed(m_pCodedFrame);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_B_FRAME:
            {
                //----coding---------------
                err = m_pCurrPicture->PAC_BFrame(&MEParams);
                VC1_ENC_CHECK(err);

                //----VLC---------------
                err = m_pCurrPicture->VLC_BFrame(m_pCodedFrame);
                VC1_ENC_CHECK(err);
            }
            break;
        case VC1_ENC_SKIP_FRAME:
            err = m_pCurrPicture->VLC_SkipFrame(m_pCodedFrame);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_I_I_FIELD:
                //----coding---------------

                err = m_pCurrPicture->PACIField(&MEParams,bSecondField);
                VC1_ENC_CHECK(err);

                //----VLC---------------
                err = m_pCurrPicture->VLC_I_FieldPic(m_pCodedFrame, bSecondField);
                VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_I_P_FIELD:
                //----coding---------------
                if(!bSecondField)
                {
                    err = m_pCurrPicture->PACIField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err = m_pCurrPicture->VLC_I_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
                else
                {
                    err =  m_pCurrPicture->PACPField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_P_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
            break;
        case VC1_ENC_P_I_FIELD:
                //----coding---------------
                if(!bSecondField)
                {
                    err =  m_pCurrPicture->PACPField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_P_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
                else
                {
                    err = m_pCurrPicture->PACIField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err = m_pCurrPicture->VLC_I_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
            break;
        case VC1_ENC_P_P_FIELD:
                //----coding---------------
                    err =  m_pCurrPicture->PACPField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_P_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);

                    break;
        case VC1_ENC_BI_B_FIELD:
                //----coding---------------
                if(!bSecondField)
                {
                    err = m_pCurrPicture->PACIField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err = m_pCurrPicture->VLC_I_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
                else
                {
                    err =  m_pCurrPicture->PACBField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_B_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
            break;
        case VC1_ENC_B_BI_FIELD:
                //----coding---------------
                if(!bSecondField)
                {
                    err =  m_pCurrPicture->PACBField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_B_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
                else
                {
                    err = m_pCurrPicture->PACIField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err = m_pCurrPicture->VLC_I_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);
                }
            break;
        case VC1_ENC_B_B_FIELD:
                //----coding---------------
                    err =  m_pCurrPicture->PACBField(&MEParams,bSecondField);
                    VC1_ENC_CHECK(err);

                    //----VLC---------------
                    err =  m_pCurrPicture->VLC_B_FieldPic(m_pCodedFrame, bSecondField);
                    VC1_ENC_CHECK(err);

                    break;
        default:
            return UMC::UMC_ERR_FAILED;

        }

        //--------ME-UPDATE---------------
 #ifdef VC1_ME_MB_STATICTICS
        if(m_pPlane)
        {
            MEParams.pSrc->SrcPlane.ptr[0]  = m_pPlane->GetYPlane();
            MEParams.pSrc->SrcPlane.step[0] = m_pPlane->GetYStep();
            MEParams.pSrc->SrcPlane.ptr[1]  = m_pPlane->GetUPlane();
            MEParams.pSrc->SrcPlane.step[1] = m_pPlane->GetUStep();
            MEParams.pSrc->SrcPlane.ptr[2]  = m_pPlane->GetVPlane();
            MEParams.pSrc->SrcPlane.step[2] = m_pPlane->GetVStep();

        }
        else
        {
            MEParams.pSrc->SrcPlane.ptr[0]  = NULL;
            MEParams.pSrc->SrcPlane.step[0] = NULL;
            MEParams.pSrc->SrcPlane.ptr[1]  = NULL;
            MEParams.pSrc->SrcPlane.step[1] = NULL;
            MEParams.pSrc->SrcPlane.ptr[2]  = NULL;
            MEParams.pSrc->SrcPlane.step[2] = NULL;
        }

        if(m_pRaisedPlane)
        {
            MEParams.pSrc->RecPlane.ptr[0]  = m_pRaisedPlane->GetYPlane();
            MEParams.pSrc->RecPlane.step[0] = m_pRaisedPlane->GetYStep();
            MEParams.pSrc->RecPlane.ptr[1]  = m_pRaisedPlane->GetUPlane();
            MEParams.pSrc->RecPlane.step[1] = m_pRaisedPlane->GetUStep();
            MEParams.pSrc->RecPlane.ptr[2]  = m_pRaisedPlane->GetVPlane();
            MEParams.pSrc->RecPlane.step[2] = m_pRaisedPlane->GetVStep();
        }
        else
        {
            MEParams.pSrc->RecPlane.ptr[0]  = NULL;
            MEParams.pSrc->RecPlane.step[0] = NULL;
            MEParams.pSrc->RecPlane.ptr[1]  = NULL;
            MEParams.pSrc->RecPlane.step[1] = NULL;
            MEParams.pSrc->RecPlane.ptr[2]  = NULL;
            MEParams.pSrc->RecPlane.step[2] = NULL;
        }
#endif
        switch (InputPictureType)
        {

        case VC1_ENC_I_FRAME:
            err = Enc_IFrameUpdate(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_P_FRAME:
            err = Enc_PFrameUpdate(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_P_FRAME_MIXED:
            err = Enc_PFrameUpdateMixed(m_pME, &MEParams);
            VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_B_FRAME:
            //err = Enc_BFrameUpdate(m_pME, &MEParams);
            //VC1_ENC_CHECK(err);
            break;
        case VC1_ENC_SKIP_FRAME:
            break;
        case VC1_ENC_I_I_FIELD:
             err = Enc_I_FieldUpdate(m_pME, &MEParams);
             VC1_ENC_CHECK(err);
             break;
        case VC1_ENC_I_P_FIELD:
             if(!bSecondField)
             {
                err = Enc_I_FieldUpdate(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
             }
             else
             {
                err = Enc_P_FieldUpdate(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
             }
             break;
        case VC1_ENC_P_I_FIELD:
             if(!bSecondField)
             {
                err = Enc_P_FieldUpdate(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
             }
             else
             {
                err = Enc_I_FieldUpdate(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
             }
             break;
        case VC1_ENC_P_P_FIELD:
             err = Enc_P_FieldUpdate(m_pME, &MEParams);
             VC1_ENC_CHECK(err);
             break;
        case VC1_ENC_BI_B_FIELD:
             //if(!bSecondField)
             //{
             //   err = Enc_I_FieldUpdate(m_pME, &MEParams);
             //   VC1_ENC_CHECK(err);
             //}
             //else
             //{
             //   err = Enc_B_FieldUpdate(m_pME, &MEParams);
             //   VC1_ENC_CHECK(err);
             //}
             break;
        case VC1_ENC_B_BI_FIELD:
             if(!bSecondField)
             {
                //err = Enc_B_FieldUpdate(m_pME, &MEParams);
                //VC1_ENC_CHECK(err);
             }
             else
             {
                err = Enc_I_FieldUpdate(m_pME, &MEParams);
                VC1_ENC_CHECK(err);
             }
             break;
        case VC1_ENC_B_B_FIELD:
             //err = Enc_B_FieldUpdate(m_pME, &MEParams);
             //VC1_ENC_CHECK(err);
             break;
        default:
            return UMC::UMC_ERR_FAILED;
        }

         //-----BRC---------------
        if(bSecondField || !IsFieldPicture(InputPictureType))
        {
            err = m_pBitRateControl->CheckFrameCompression(InputPictureType,
                m_pCodedFrame->GetDataLen() - CodedSize);

        }
        else
            err = UMC::UMC_OK;

        if(err == UMC::UMC_OK)
            frame_recoding = false;

        else if((err == UMC::UMC_ERR_NOT_ENOUGH_BUFFER)
            && (uiQuantIndex == 31))
            frame_recoding = false;
        else  if((err == UMC::UMC_ERR_NOT_ENOUGH_DATA)
            && (uiQuantIndex == 2))
            frame_recoding = false;

        frame_recoding = frame_recoding & m_bFrameRecoding;

        RecodedFrameNum++;
     }

        if(bSecondField || !IsFieldPicture(InputPictureType))
        {
            m_LastQuant = uiQuantIndex*2 + bHalfQuant;
            //-----BRC-UPDATE--------------
            err = m_pBitRateControl->CompleteFrame(InputPictureType);
        }

        //TODO: BRC error handling
        err = UMC::UMC_OK;

    return UMC::UMC_OK;
}

void VC1EncoderADV::SetMemoryAllocator(UMC::MemoryAllocator *pMemoryAllocator, bool bOwnAllocator)
    {
         m_pMemoryAllocator = pMemoryAllocator;
         m_bOwnAllocator = bOwnAllocator;
    };

UMC::Status VC1EncoderADV::Reset()
{
    UMC::Status umcSts = UMC::UMC_OK;

    m_nReferenceFrameDist = 0;
    m_pWaitingList->Reset();
    m_pGOP->Reset();
    m_pBitRateControl->Reset();
    m_pStoredFrames->Reset();
    m_iFrameCount = 0;
    ResetPlanes();

    return umcSts;
}


UMC::Status VC1EncoderADV::GetInfo(UMC::VC1EncoderParams* pInfo)
{
   UMC::Status umcSts = UMC::UMC_OK;
   VC1BRInfo BRInfo;
   VC1_hrd_OutData HRDParam;

   if(!m_pBitRateControl)
       return UMC::UMC_ERR_NOT_INITIALIZED;

   umcSts = m_pBitRateControl->GetCurrentHRDParams(0,&HRDParam);
   if(umcSts != UMC::UMC_OK)
       return umcSts;

   umcSts = m_pBitRateControl->GetBRInfo(&BRInfo);
   if(umcSts != UMC::UMC_OK)
       return umcSts;

   pInfo->m_iQuality = -1;

   pInfo->m_info.iBitrate      = BRInfo.bitrate;
   pInfo->m_info.fFramerate    = BRInfo.framerate;
   pInfo->m_iConstQuant     = BRInfo.constQuant;
   pInfo->m_uiHRDBufferSize = HRDParam.hrd_buffer;

   pInfo->m_uiGOPLength     = m_uiGOPLength;
   pInfo->m_uiBFrmLength    = m_uiBFrmLength;
   pInfo->m_uiMESearchSpeed = m_MESearchSpeed;
   pInfo->m_bFrameRecoding  = m_bFrameRecoding;

   pInfo->m_uiNumFrames     = m_iFrameCount;
   pInfo->m_iFramesCounter  = m_iFrameCount;

   pInfo->m_info.iProfile = VC1_ENC_PROFILE_A;
   pInfo->m_info.iLevel = m_SH->GetLevel();
   pInfo->m_info.streamType    = UMC::VC1_VIDEO;
   pInfo->m_info.streamSubtype = UMC::VC1_VIDEO_VC1;
   pInfo->m_info.videoInfo.m_picStructure = UMC::PS_PROGRESSIVE;
   pInfo->m_info.videoInfo.m_colorFormat = UMC::YUV420;
   pInfo->m_info.iStreamPID = 0;
   pInfo->m_info.fDuration  = 0;
   pInfo->m_iThreads     = 0;

   pInfo->m_info.videoInfo.m_iHeight = m_SH->GetPictureHeight();
   pInfo->m_info.videoInfo.m_iWidth  = m_SH->GetPictureWidth();
   pInfo->m_bDeblocking = m_SH->IsLoopFilter();
   pInfo->m_bVSTransform = m_SH->IsVSTransform();

   pInfo->m_info.videoInfo.m_iSAWidth  = 1;
   pInfo->m_info.videoInfo.m_iSAHeight = 1;

   pInfo->m_iSuggestedOutputSize = 0;
   pInfo->m_iSuggestedInputSize  =  0;

   pInfo->m_pData = NULL;

   return umcSts;
}



UMC::Status VC1EncoderADV::SetMEParams(UMC::MeParams* MEParams, Ipp8u doubleQuant, bool Uniform, bool FieldPicture)
{
    UMC::Status umcSts          =  UMC::UMC_OK;

    assert(m_pME!=NULL);

    if(m_pME == NULL)
        return UMC::UMC_ERR_NULL_PTR;

    MEParams->SetSearchSpeed(m_MESearchSpeed);

    MEParams->CostMetric      = UMC::ME_Sad;
    MEParams->SkippedMetrics  = UMC::ME_Sad;

    MEParams->ProcessSkipped = true;
    MEParams->ProcessDirect = true;

    MEParams->UseFeedback    = m_bUseMeFeedback;
    MEParams->UpdateFeedback = m_bUseUpdateMeFeedback;
    MEParams->UseFastFeedback= m_bUseFastMeFeedback;

    MEParams->SelectVlcTables        = m_bSelectVLCTables;
    MEParams->ChangeInterpPixelType  = m_bChangeInterpPixelType;
    MEParams->UseTrellisQuantization = m_bUseTreillisQuantization;

    MEParams->Quant        = doubleQuant;
    MEParams->UniformQuant = Uniform;

    //MEParams->MVDiffTablesVLC = MVDiffTablesVLC[m_VLCIndex.uiMVTab];
    MEParams->MVRangeIndex = m_InitPicParam.uiMVRangeIndex;

    //MEParams->MVHalf = (m_InitPicParam.uiMVMode == VC1_ENC_1MV_HALF_BILINEAR ||
    //                    m_InitPicParam.uiMVMode == VC1_ENC_1MV_HALF_BICUBIC) ? 1: 0;

    MEParams->MVSizeOffset = MVSizeOffset;
    MEParams->MVLengthLong = longMVLength;
    MEParams->FastChroma   = m_bFastUVMC;
    MEParams->UseVarSizeTransform = m_SH->IsVSTransform();

   // eCodingSet    LumaCodingSetIntra   = LumaCodingSetsIntra  [(doubleQuant>>1)>8][m_VLCIndex.uiDecTypeAC1];
    //MEParams->AcTableSetIntraLuma=(UMC::MeACTablesSet*)&ACTablesSet[LumaCodingSetIntra];

    //eCodingSet   ChromaCodingSetIntra = ChromaCodingSetsIntra[(doubleQuant>>1)>8][m_VLCIndex.uiDecTypeAC1];
    //MEParams->AcTableSetIntraChroma=(UMC::MeACTablesSet*)&ACTablesSet[ChromaCodingSetIntra];

    //eCodingSet   CodingSetInter = CodingSetsInter[(doubleQuant>>1)>8][m_VLCIndex.uiDecTypeAC1];
    //MEParams->AcTableSetInterLuma=(UMC::MeACTablesSet*)&ACTablesSet[CodingSetInter];
    //MEParams->AcTableSetInterChroma=(UMC::MeACTablesSet*)&ACTablesSet[CodingSetInter];

    MEParams->DcTableLuma   = DCTables[m_VLCIndex.uiDecTypeDCIntra][0];
    MEParams->DcTableChroma = DCTables[m_VLCIndex.uiDecTypeDCIntra][1];

    if(!FieldPicture)
    {
        MEParams->ScanTable[0] = VC1_Inter_8x8_Scan;
        MEParams->ScanTable[1] = VC1_Inter_8x4_Scan_Adv;
        MEParams->ScanTable[2] = VC1_Inter_4x8_Scan_Adv;
        MEParams->ScanTable[3] = VC1_Inter_4x4_Scan;
    }
    else
    {
        MEParams->ScanTable[0] = VC1_Inter_InterlaceIntra_8x8_Scan_Adv;
        MEParams->ScanTable[1] = VC1_Inter_Interlace_8x4_Scan_Adv;
        MEParams->ScanTable[2] = VC1_Inter_Interlace_4x8_Scan_Adv;
        MEParams->ScanTable[3] = VC1_Inter_Interlace_4x4_Scan_Adv;
    }

    return umcSts;
}

UMC::Status VC1EncoderADV::SetMEParams_I(UMC::MeParams* MEParams)
{
    UMC::Status umcSts          =  UMC::UMC_OK;

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!=NULL);
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmIntra;

    MEParams->pSrc->plane[0].ptr[0]  = m_pPlane->GetYPlane();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].ptr[1]  = m_pPlane->GetUPlane();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].ptr[2]  = m_pPlane->GetVPlane();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep();
    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight() + 15)/16;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->BRefFramesNum = 0;
    MEParams->FRefFramesNum = 0;
    return umcSts;
}

UMC::Status VC1EncoderADV::SetMEParams_P(UMC::MeParams* MEParams)
{
    UMC::Status umcSts         =  UMC::UMC_OK;

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!=NULL);
    VC1_NULL_PTR(MEParams->pSrc);

    MEParams->pSrc->plane[0].ptr[0] = m_pPlane->GetYPlane();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].ptr[1] = m_pPlane->GetUPlane();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].ptr[2] = m_pPlane->GetVPlane();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep();
    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight() + 15)/16;
    MEParams->pSrc->type    = UMC::ME_FrmFrw;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
    assert(MEParams->pRefF[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefF[0]);

    MEParams->pRefF[0]->plane[0].ptr[0] =  m_pForwardMEPlane->GetYPlane();
    MEParams->pRefF[0]->plane[0].step[0] = m_pForwardMEPlane->GetYStep();
    MEParams->pRefF[0]->plane[0].ptr[1] =  m_pForwardMEPlane->GetUPlane();
    MEParams->pRefF[0]->plane[0].step[1] = m_pForwardMEPlane->GetUStep();
    MEParams->pRefF[0]->plane[0].ptr[2] =  m_pForwardMEPlane->GetVPlane();
    MEParams->pRefF[0]->plane[0].step[2] = m_pForwardMEPlane->GetVStep();
    MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight() + 15)/16;
    MEParams->pRefF[0]->padding = 32;

    MEParams->Interpolation      = (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode) ?
                                    UMC::ME_VC1_Bilinear :UMC::ME_VC1_Bicubic;
    MEParams->ChromaInterpolation = UMC::ME_VC1_Bilinear;


    MEParams->SearchDirection    =  UMC::ME_ForwardSearch;
    MEParams->PixelType          =  (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode
                                   || VC1_ENC_1MV_HALF_BICUBIC == m_InitPicParam.uiMVMode) ?
                                        UMC::ME_HalfPixel : UMC::ME_QuarterPixel;
    MEParams->MbPart             =  UMC::ME_Mb16x16;

    MEParams->PredictionType     = UMC::ME_VC1Hybrid;

    MEParams->PicRange.top_left.x       = -15;
    MEParams->PicRange.top_left.y       = -15;
    MEParams->PicRange.bottom_right.x   = m_SH->GetPictureWidth()+15 ;
    MEParams->PicRange.bottom_right.y   = m_SH->GetPictureHeight()+15;

    MEParams->SearchRange.x             = MVRange[2*m_InitPicParam.uiMVRangeIndex];
    MEParams->SearchRange.y             = MVRange[2*m_InitPicParam.uiMVRangeIndex + 1];

    MEParams->BRefFramesNum             = 0;
    MEParams->FRefFramesNum             = 1;

    return umcSts;
}
UMC::Status VC1EncoderADV::SetMEParams_PMixed(UMC::MeParams* MEParams)
{
    UMC::Status umcSts          =  UMC::UMC_OK;

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!=NULL);
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmFrw;

    MEParams->pSrc->plane[0].ptr[0] = m_pPlane->GetYPlane();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].ptr[1] = m_pPlane->GetUPlane();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].ptr[2] = m_pPlane->GetVPlane();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep();
    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight() + 15)/16;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
    assert(MEParams->pRefF[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefF[0]);

    MEParams->pRefF[0]->plane[0].ptr[0] =  m_pForwardMEPlane->GetYPlane();
    MEParams->pRefF[0]->plane[0].step[0] = m_pForwardMEPlane->GetYStep();
    MEParams->pRefF[0]->plane[0].ptr[1] =  m_pForwardMEPlane->GetUPlane();
    MEParams->pRefF[0]->plane[0].step[1] = m_pForwardMEPlane->GetUStep();
    MEParams->pRefF[0]->plane[0].ptr[2] =  m_pForwardMEPlane->GetVPlane();
    MEParams->pRefF[0]->plane[0].step[2] = m_pForwardMEPlane->GetVStep();

    MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight() + 15)/16;
    MEParams->pRefF[0]->padding = 32;

    MEParams->Interpolation      =  UMC::ME_VC1_Bicubic;
    MEParams->ChromaInterpolation = UMC::ME_VC1_Bilinear;

    MEParams->SearchDirection    =  UMC::ME_ForwardSearch;
    MEParams->PixelType          =  UMC::ME_QuarterPixel;
    MEParams->MbPart             =  UMC::ME_Mb8x8;
    MEParams->PredictionType     =  UMC::ME_VC1Hybrid;

    MEParams->PicRange.top_left.x       = -7;
    MEParams->PicRange.top_left.y       = -7;
    MEParams->PicRange.bottom_right.x   = m_SH->GetPictureWidth() +7 ;
    MEParams->PicRange.bottom_right.y   = m_SH->GetPictureHeight()+7;

    MEParams->SearchRange.x             = MVRange[2*m_InitPicParam.uiMVRangeIndex];
    MEParams->SearchRange.y             = MVRange[2*m_InitPicParam.uiMVRangeIndex + 1];

    MEParams->BRefFramesNum             = 0;
    MEParams->FRefFramesNum             = 1;

    return umcSts;
}
UMC::Status VC1EncoderADV::SetMEParams_B(UMC::MeParams* MEParams)
{
    UMC::Status umcSts       = UMC::UMC_OK;

    Ipp32u       h = (m_SH->GetPictureHeight()+15)/16;
    Ipp32u       w = (m_SH->GetPictureWidth() +15)/16;

    Ipp32s      scaleFactor  = BFractionScaleFactor[m_InitPicParam.uiBFraction.denom][m_InitPicParam.uiBFraction.num];
    void        (* GetMVDirect) (Ipp16s x, Ipp16s y, Ipp32s scaleFactor,
                                 sCoordinate * mvF, sCoordinate *mvB);

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!=NULL);
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmBidir;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
    assert(MEParams->pRefF[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefF[0]);

    MEParams->pRefB[0] = m_MeFrame[m_MeIndex.MeRefBIndex];
    assert(MEParams->pRefB[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefB[0]);

    sCoordinate MVFDirect    = {0,0};
    sCoordinate MVBDirect    = {0,0};

    sCoordinate                 MVPredMin = {-60,-60};
    sCoordinate                 MVPredMax = {((Ipp16s)w*16 - 1)*4, ((Ipp16s)h*16 - 1)*4};

    Ipp16s*                     pSavedMV = m_pSavedMV;

    Ipp32u i = 0;
    Ipp32u j = 0;

    GetMVDirect = (m_InitPicParam.uiMVMode != VC1_ENC_1MV_QUARTER_BICUBIC)? GetMVDirectHalf : GetMVDirectQuarter;

    MEParams->pSrc->plane[0].ptr[0]  = m_pPlane->GetYPlane();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].ptr[1]  = m_pPlane->GetUPlane();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].ptr[2]  = m_pPlane->GetVPlane();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep();

    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight() + 15)/16;

    MEParams->pRefF[0]->plane[0].ptr[0] =  m_pForwardMEPlane->GetYPlane();
    MEParams->pRefF[0]->plane[0].step[0] = m_pForwardMEPlane->GetYStep();
    MEParams->pRefF[0]->plane[0].ptr[1] =  m_pForwardMEPlane->GetUPlane();
    MEParams->pRefF[0]->plane[0].step[1] = m_pForwardMEPlane->GetUStep();
    MEParams->pRefF[0]->plane[0].ptr[2] =  m_pForwardMEPlane->GetVPlane();
    MEParams->pRefF[0]->plane[0].step[2] = m_pForwardMEPlane->GetVStep();

    MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight() + 15)/16;
    MEParams->pRefF[0]->padding = 32;

    MEParams->pRefB[0]->plane[0].ptr[0] =  m_pBackwardMEPlane->GetYPlane();
    MEParams->pRefB[0]->plane[0].step[0] = m_pBackwardMEPlane->GetYStep();
    MEParams->pRefB[0]->plane[0].ptr[1] =  m_pBackwardMEPlane->GetUPlane();
    MEParams->pRefB[0]->plane[0].step[1] = m_pBackwardMEPlane->GetUStep();
    MEParams->pRefB[0]->plane[0].ptr[2] =  m_pBackwardMEPlane->GetVPlane();
    MEParams->pRefB[0]->plane[0].step[2] = m_pBackwardMEPlane->GetVStep();

    MEParams->pRefB[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefB[0]->HeightMB = (m_SH->GetPictureHeight() + 15)/16;
    MEParams->pRefB[0]->padding = 32;


    MEParams->Interpolation      =  (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode) ?
                                            UMC::ME_VC1_Bilinear :UMC::ME_VC1_Bicubic;
    MEParams->ChromaInterpolation = UMC::ME_VC1_Bilinear;

    MEParams->SearchDirection    = UMC::ME_BidirSearch;

    MEParams->PixelType          = (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode
                                 || VC1_ENC_1MV_HALF_BICUBIC == m_InitPicParam.uiMVMode) ?
                                   UMC::ME_HalfPixel : UMC::ME_QuarterPixel;
    MEParams->MbPart             = UMC::ME_Mb16x16;
    MEParams->PredictionType     = UMC::ME_VC1;
    MEParams->PicRange.top_left.x = -7;
    MEParams->PicRange.top_left.y = -7;
    MEParams->PicRange.bottom_right.x = m_SH->GetPictureWidth() ;
    MEParams->PicRange.bottom_right.y = m_SH->GetPictureHeight();


    MEParams->SearchRange.x   = MVRange[2*m_InitPicParam.uiMVRangeIndex];
    MEParams->SearchRange.y   = MVRange[2*m_InitPicParam.uiMVRangeIndex + 1];

    MEParams->BRefFramesNum             = 1;
    MEParams->FRefFramesNum             = 1;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            GetMVDirect (pSavedMV[0],pSavedMV[1],scaleFactor, &MVFDirect, &MVBDirect);
            ScalePredict(&MVFDirect, j*16*4,i*16*4,MVPredMin,MVPredMax);
            ScalePredict(&MVBDirect, j*16*4,i*16*4,MVPredMin,MVPredMax);

            MEParams->pRefF[0]->MVDirect[(j + i*w)].x = MVFDirect.x;
            MEParams->pRefF[0]->MVDirect[(j + i*w)].y = MVFDirect.y;

            MEParams->pRefB[0]->MVDirect[(j + i*w)].x = MVBDirect.x;
            MEParams->pRefB[0]->MVDirect[(j + i*w)].y = MVBDirect.y;

            pSavedMV +=2;
        }
    }

    return umcSts;
}

UMC::Status  VC1EncoderADV::SetMEParams_I_Field(UMC::MeParams* MEParams)
{
    UMC::Status umcSts          =  UMC::UMC_OK;
    memset(MEParams,0,sizeof(MEParams));

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!=NULL);
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmIntra;

    MEParams->pSrc->plane[0].ptr[0]  = m_pPlane->GetYPlane();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].ptr[1]  = m_pPlane->GetUPlane();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].ptr[2]  = m_pPlane->GetVPlane();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep();

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->BRefFramesNum             = 0;
    MEParams->FRefFramesNum             = 0;

    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight() + 15)/16;

    return umcSts;
}


UMC::Status VC1EncoderADV::SetMEParams_B_Field(UMC::MeParams* MEParams,
                                        bool bTopFieldFirst, bool bSecond)
{
    UMC::Status umcSts       = UMC::UMC_OK;
    bool bBottom        =  IsBottomField(bTopFieldFirst, bSecond);
    Ipp32u       h = (m_SH->GetPictureHeight()/2 +15)/16;
    Ipp32u       w = (m_SH->GetPictureWidth() +15)/16;

    Ipp8u      scaleFactor  = (Ipp8u)((BFractionScaleFactor[m_InitPicParam.uiBFraction.denom][m_InitPicParam.uiBFraction.num]*(Ipp32s)m_nReferenceFrameDist)>>8);
    Ipp8u      scaleFactor1 = BFractionScaleFactor[m_InitPicParam.uiBFraction.denom][m_InitPicParam.uiBFraction.num];

    sScaleInfo  scInfoForw;
    sScaleInfo  scInfoBackw;
    void        (* GetMVDirect) (Ipp16s x, Ipp16s y, Ipp32s scaleFactor,
                                 sCoordinate * mvF, sCoordinate *mvB);
    sCoordinate MVFDirect    = {0,0};
    sCoordinate MVBDirect    = {0,0};

    sCoordinate                 MVPredMin = {-60,-60};
    sCoordinate                 MVPredMax = {((Ipp16s)w*16 - 1)*4, ((Ipp16s)h*16 - 1)*4};

    Ipp16s*                     pSavedMV   = m_pSavedMV+w*h*2*bBottom;
    Ipp8u*                      pDirection = m_pRefType + w*h*bBottom;
    Ipp32u i = 0;
    Ipp32u j = 0;
    Ipp32u RefNum = 0;

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    assert(MEParams->pSrc!= NULL);
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmBidir;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
    assert(MEParams->pRefF[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefF[0]);

    MEParams->pRefF[1] = m_MeFrame[m_MeIndex.MeRefFIndex + VC1_ENC_MAX_REF_CURR_FRAME];
    assert(MEParams->pRefF[1]!=NULL);
    VC1_NULL_PTR(MEParams->pRefF[1]);

    MEParams->pRefB[0] = m_MeFrame[m_MeIndex.MeRefBIndex];
    assert(MEParams->pRefB[0]!=NULL);
    VC1_NULL_PTR(MEParams->pRefB[0]);

    MEParams->pRefB[1] = m_MeFrame[m_MeIndex.MeRefBIndex + VC1_ENC_MAX_REF_CURR_FRAME];
    assert(MEParams->pRefB[1]!=NULL);
    VC1_NULL_PTR(MEParams->pRefB[1]);

    MEParams->pSrc->plane[0].ptr[0]  = m_pPlane->GetYPlane() + bBottom * m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep()*2;
    MEParams->pSrc->plane[0].ptr[1]  = m_pPlane->GetUPlane() + bBottom * m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep()*2;
    MEParams->pSrc->plane[0].ptr[2]  = m_pPlane->GetVPlane() + bBottom * m_pPlane->GetVStep();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep()*2;
    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;

    MEParams->FRefFramesNum = 2;
    MEParams->BRefFramesNum = 2;

    MEParams->PredictionType     = UMC::ME_VC1Field2;
    //forward planes
    //Y
    umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[0],&MEParams->pRefF[0]->plane[0].step[0],
                              &MEParams->pRefF[1]->plane[0].ptr[0],    &MEParams->pRefF[1]->plane[0].step[0],
                              m_pForwardPlane->GetYPlane(),   m_pForwardMEPlane->GetYStep(),
                              m_pRaisedPlane->GetYPlane(),    m_pRaisedPlane->GetYStep(),
                              bSecond, bBottom);
    VC1_ENC_CHECK(umcSts);
    assert(MEParams->pRefF[0]->plane[0].ptr[0]!= NULL);
    assert(MEParams->pRefF[1]->plane[0].ptr[0]!= NULL);

    //U
    umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[1],&MEParams->pRefF[0]->plane[0].step[1],
                              &MEParams->pRefF[1]->plane[0].ptr[1],    &MEParams->pRefF[1]->plane[0].step[1],
                              m_pForwardPlane->GetUPlane(),   m_pForwardMEPlane->GetUStep(),
                              m_pRaisedPlane->GetUPlane(),    m_pRaisedPlane->GetUStep(),
                              bSecond, bBottom);
    VC1_ENC_CHECK(umcSts);
    assert(MEParams->pRefF[0]->plane[0].ptr[1]!= NULL);
    assert(MEParams->pRefF[1]->plane[0].ptr[1]!= NULL);

    //V
    umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[2],&MEParams->pRefF[0]->plane[0].step[2],
                              &MEParams->pRefF[1]->plane[0].ptr[2],    &MEParams->pRefF[1]->plane[0].step[2],
                              m_pForwardPlane->GetVPlane(),   m_pForwardMEPlane->GetVStep(),
                              m_pRaisedPlane->GetVPlane(),    m_pRaisedPlane->GetVStep(),
                              bSecond, bBottom);
    VC1_ENC_CHECK(umcSts);
    assert(MEParams->pRefF[0]->plane[0].ptr[2]!= NULL);
    assert(MEParams->pRefF[1]->plane[0].ptr[2]!= NULL);

    MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
    MEParams->pRefF[0]->padding = 16;

    MEParams->pRefF[1]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefF[1]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
    MEParams->pRefF[1]->padding = 16;

    //backward planes
    //Y
    umcSts = SetBkwFieldPlane(&MEParams->pRefB[0]->plane[0].ptr[0],   &MEParams->pRefB[0]->plane[0].step[0],
                              &MEParams->pRefB[1]->plane[0].ptr[0],   &MEParams->pRefB[1]->plane[0].step[0],
                              m_pBackwardPlane->GetYPlane(), m_pBackwardPlane->GetYStep(), bBottom);

    assert(MEParams->pRefB[0]->plane[0].ptr[0]!= NULL);
    assert(MEParams->pRefB[1]->plane[0].ptr[0]!= NULL);

    //U
    umcSts = SetBkwFieldPlane(&MEParams->pRefB[0]->plane[0].ptr[1],   &MEParams->pRefB[0]->plane[0].step[1],
                              &MEParams->pRefB[1]->plane[0].ptr[1],   &MEParams->pRefB[1]->plane[0].step[1],
                              m_pBackwardPlane->GetUPlane(), m_pBackwardPlane->GetUStep(), bBottom);

    assert(MEParams->pRefB[0]->plane[0].ptr[1]!= NULL);
    assert(MEParams->pRefB[1]->plane[0].ptr[1]!= NULL);

    //V
    umcSts = SetBkwFieldPlane(&MEParams->pRefB[0]->plane[0].ptr[2],   &MEParams->pRefB[0]->plane[0].step[2],
                              &MEParams->pRefB[1]->plane[0].ptr[2],   &MEParams->pRefB[1]->plane[0].step[2],
                              m_pBackwardPlane->GetVPlane(), m_pBackwardPlane->GetVStep(), bBottom);

    assert(MEParams->pRefB[0]->plane[0].ptr[2]!= NULL);
    assert(MEParams->pRefB[1]->plane[0].ptr[2]!= NULL);

    MEParams->pRefB[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefB[0]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
    MEParams->pRefB[0]->padding = 16;

    MEParams->pRefB[1]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pRefB[1]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
    MEParams->pRefB[1]->padding = 16;

    if (m_InitPicParam.uiMVMode != VC1_ENC_1MV_QUARTER_BICUBIC && m_InitPicParam.uiMVMode !=VC1_ENC_MIXED_QUARTER_BICUBIC)
            GetMVDirect =  GetMVDirectCurrHalfBackQuarter;
        else
            GetMVDirect = GetMVDirectCurrQuarterBackQuarter;

    MEParams->bSecondField       =  bSecond;
    MEParams->Interpolation      =  (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode) ?
                                            UMC::ME_VC1_Bilinear :UMC::ME_VC1_Bicubic;
    MEParams->ChromaInterpolation = UMC::ME_VC1_Bilinear;

    MEParams->SearchDirection    = UMC::ME_BidirSearch;

    MEParams->PixelType          = (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode
                                 || VC1_ENC_1MV_HALF_BICUBIC == m_InitPicParam.uiMVMode) ?
                                   UMC::ME_HalfPixel : UMC::ME_QuarterPixel;
    MEParams->MbPart             = UMC::ME_Mb16x16;
    MEParams->PredictionType     = UMC::ME_VC1Field2;

    MEParams->PicRange.top_left.x = -7;
    MEParams->PicRange.top_left.y = -7;
    MEParams->PicRange.bottom_right.x = m_SH->GetPictureWidth() ;
    MEParams->PicRange.bottom_right.y = m_SH->GetPictureHeight()/2;

    MEParams->FirstMB = 0;
    MEParams->LastMB  = (((m_SH->GetPictureHeight()/2)+15)/16) * ((m_SH->GetPictureWidth() +15)/16) - 1;

    InitScaleInfo(&scInfoForw,bSecond,bBottom,scaleFactor,
        m_InitPicParam.uiMVRangeIndex);

    MEParams->ScaleInfo[0].ME_Bottom = scInfoForw.bBottom;

    MEParams->ScaleInfo[0].ME_RangeX = scInfoForw.rangeX;
    MEParams->ScaleInfo[0].ME_RangeY = scInfoForw.rangeY;

    MEParams->ScaleInfo[0].ME_ScaleOpp = scInfoForw.scale_opp;

    MEParams->ScaleInfo[0].ME_ScaleSame1 = scInfoForw.scale_same1;
    MEParams->ScaleInfo[0].ME_ScaleSame2 = scInfoForw.scale_same2;

    MEParams->ScaleInfo[0].ME_ScaleZoneX = scInfoForw.scale_zoneX;
    MEParams->ScaleInfo[0].ME_ScaleZoneY = scInfoForw.scale_zoneY;

    MEParams->ScaleInfo[0].ME_ZoneOffsetX = scInfoForw.zone_offsetX;
    MEParams->ScaleInfo[0].ME_ZoneOffsetY = scInfoForw.zone_offsetY;

    InitScaleInfoBackward(&scInfoBackw,bSecond,bBottom,
                  ((m_nReferenceFrameDist - scaleFactor-1)>=0)?(m_nReferenceFrameDist - scaleFactor-1):0,
                  m_InitPicParam.uiMVRangeIndex);

    MEParams->ScaleInfo[1].ME_Bottom = scInfoBackw.bBottom;

    MEParams->ScaleInfo[1].ME_RangeX = scInfoBackw.rangeX;
    MEParams->ScaleInfo[1].ME_RangeY = scInfoBackw.rangeY;

    MEParams->ScaleInfo[1].ME_ScaleOpp = scInfoBackw.scale_opp;

    MEParams->ScaleInfo[1].ME_ScaleSame1 = scInfoBackw.scale_same1;
    MEParams->ScaleInfo[1].ME_ScaleSame2 = scInfoBackw.scale_same2;

    MEParams->ScaleInfo[1].ME_ScaleZoneX = scInfoBackw.scale_zoneX;
    MEParams->ScaleInfo[1].ME_ScaleZoneY = scInfoBackw.scale_zoneY;

    MEParams->ScaleInfo[1].ME_ZoneOffsetX = scInfoBackw.zone_offsetX;
    MEParams->ScaleInfo[1].ME_ZoneOffsetY = scInfoBackw.zone_offsetY;


    MEParams->SearchRange.x   = MVRange[2*m_InitPicParam.uiMVRangeIndex]-1;
    MEParams->SearchRange.y   = MVRange[2*m_InitPicParam.uiMVRangeIndex + 1]-1;

    MEParams->BRefFramesNum             = 2;
    MEParams->FRefFramesNum             = 2;

    for (i=0; i < h; i++)
    {
        for (j=0; j < w; j++)
        {
            GetMVDirect (pSavedMV[0],pSavedMV[1],scaleFactor1, &MVFDirect, &MVBDirect);

            ScalePredict(&MVFDirect, j*16*4,i*16*4,MVPredMin,MVPredMax);
            ScalePredict(&MVBDirect, j*16*4,i*16*4,MVPredMin,MVPredMax);

            if(pDirection[i*w +  j] == 0)
                RefNum = 1; // the same field
            else if (pDirection[i*w +  j] == 1)
                RefNum = 0;
            else
                RefNum= 1;

            MEParams->pRefF[RefNum]->MVDirect[(j + i*w)].x = MVFDirect.x;
            MEParams->pRefF[RefNum]->MVDirect[(j + i*w)].y = MVFDirect.y;

            MEParams->pRefF[1-RefNum]->MVDirect[(j + i*w)].SetInvalid();

            MEParams->pRefB[RefNum]->MVDirect[(j + i*w)].x = MVBDirect.x;
            MEParams->pRefB[RefNum]->MVDirect[(j + i*w)].y = MVBDirect.y;

            MEParams->pRefB[1-RefNum]->MVDirect[(j + i*w)].SetInvalid();

            pSavedMV +=2;

        }
    }
    return umcSts;
}


UMC::Status  VC1EncoderADV::SetMEParams_P_Field(UMC::MeParams* MEParams,
                                                bool bTopFieldFirst, bool bSecondField)
{
    UMC::Status umcSts  =  UMC::UMC_OK;
    sScaleInfo                  scInfo;
    bool bBottom        =  IsBottomField(bTopFieldFirst, bSecondField);

    MEParams->pSrc = m_MeFrame[m_MeIndex.MeCurrIndex];
    VC1_NULL_PTR(MEParams->pSrc);
    MEParams->pSrc->type    = UMC::ME_FrmFrw;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->StatValid = true;
    MEParams->pSrc->RecPlane.clear();
    MEParams->pSrc->SrcPlane.clear();
#endif

    //Y
    MEParams->pSrc->plane[0].ptr[0]  = m_pPlane->GetYPlane() + bBottom * m_pPlane->GetYStep();
    MEParams->pSrc->plane[0].step[0] = m_pPlane->GetYStep()*2;
    assert(MEParams->pSrc->plane[0].ptr[0]!= NULL);

    //U
    MEParams->pSrc->plane[0].ptr[1]  = m_pPlane->GetUPlane() + bBottom * m_pPlane->GetUStep();
    MEParams->pSrc->plane[0].step[1] = m_pPlane->GetUStep()*2;
    assert(MEParams->pSrc->plane[0].ptr[1]!= NULL);

    //V
    MEParams->pSrc->plane[0].ptr[2]  = m_pPlane->GetVPlane() + bBottom * m_pPlane->GetVStep();
    MEParams->pSrc->plane[0].step[2] = m_pPlane->GetVStep()*2;
    assert(MEParams->pSrc->plane[0].ptr[2]!= NULL);

    MEParams->pSrc->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
    MEParams->pSrc->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;

    if(m_uiReferenceFieldType != VC1_ENC_REF_FIELD_BOTH)
    {
        MEParams->FRefFramesNum = 1;
        MEParams->PredictionType     = UMC::ME_VC1Field1;

        MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
        assert(MEParams->pRefF[0]!=NULL);
        VC1_NULL_PTR(MEParams->pRefF[0]);

        //Y
        umcSts = Set1RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[0], &MEParams->pRefF[0]->plane[0].step[0],
                                    m_pForwardPlane->GetYPlane(), m_pForwardMEPlane->GetYStep(),
                                    m_pRaisedPlane->GetYPlane(), m_pRaisedPlane->GetYStep(),
                                    m_uiReferenceFieldType, bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[0]!= NULL);

        //U
        umcSts = Set1RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[1], &MEParams->pRefF[0]->plane[0].step[1],
                                    m_pForwardPlane->GetUPlane(), m_pForwardMEPlane->GetUStep(),
                                    m_pRaisedPlane->GetUPlane(), m_pRaisedPlane->GetUStep(),
                                    m_uiReferenceFieldType, bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[1]!= NULL);

        //V
        umcSts = Set1RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[2], &MEParams->pRefF[0]->plane[0].step[2],
                                    m_pForwardPlane->GetVPlane(), m_pForwardMEPlane->GetVStep(),
                                    m_pRaisedPlane->GetVPlane(), m_pRaisedPlane->GetVStep(),
                                    m_uiReferenceFieldType, bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[2]!= NULL);

        MEParams->BRefFramesNum             = 0;
        MEParams->FRefFramesNum             = 1;

        MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
        MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
        MEParams->pRefF[0]->padding = 16;
    }
    else
    {
         MEParams->FRefFramesNum = 2;
         MEParams->PredictionType     = UMC::ME_VC1Field2Hybrid;

        MEParams->pRefF[0] = m_MeFrame[m_MeIndex.MeRefFIndex];
        assert(MEParams->pRefF[0]!=NULL);
        VC1_NULL_PTR(MEParams->pRefF[0]);

        MEParams->pRefF[1] = m_MeFrame[m_MeIndex.MeRefFIndex + VC1_ENC_MAX_REF_CURR_FRAME];
        assert(MEParams->pRefF[1]!=NULL);
        VC1_NULL_PTR(MEParams->pRefF[1]);

        //Y
        umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[0],& MEParams->pRefF[0]->plane[0].step[0],
                              &MEParams->pRefF[1]->plane[0].ptr[0], &MEParams->pRefF[1]->plane[0].step[0],
                              m_pForwardPlane->GetYPlane(), m_pForwardMEPlane->GetYStep(),
                              m_pRaisedPlane->GetYPlane(), m_pRaisedPlane->GetYStep(),
                              bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[0]!= NULL);
        assert(MEParams->pRefF[1]->plane[0].ptr[0]!= NULL);

        //U
        umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[1],& MEParams->pRefF[0]->plane[0].step[1],
                              &MEParams->pRefF[1]->plane[0].ptr[1], &MEParams->pRefF[1]->plane[0].step[1],
                              m_pForwardPlane->GetUPlane(), m_pForwardMEPlane->GetUStep(),
                              m_pRaisedPlane->GetUPlane(), m_pRaisedPlane->GetUStep(),
                              bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[1]!= NULL);
        assert(MEParams->pRefF[1]->plane[0].ptr[1]!= NULL);

        //V
        umcSts = Set2RefFrwFieldPlane(&MEParams->pRefF[0]->plane[0].ptr[2],& MEParams->pRefF[0]->plane[0].step[2],
                              &MEParams->pRefF[1]->plane[0].ptr[2], &MEParams->pRefF[1]->plane[0].step[2],
                              m_pForwardPlane->GetVPlane(), m_pForwardMEPlane->GetVStep(),
                              m_pRaisedPlane->GetVPlane(), m_pRaisedPlane->GetVStep(),
                              bSecondField, bBottom);
        VC1_ENC_CHECK(umcSts);
        assert(MEParams->pRefF[0]->plane[0].ptr[2]!= NULL);
        assert(MEParams->pRefF[1]->plane[0].ptr[2]!= NULL);

        MEParams->BRefFramesNum             = 0;
        MEParams->FRefFramesNum             = 2;

        MEParams->pRefF[0]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
        MEParams->pRefF[0]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
        MEParams->pRefF[0]->padding = 16;

        MEParams->pRefF[1]->WidthMB = (m_SH->GetPictureWidth() + 15)/16;
        MEParams->pRefF[1]->HeightMB = (m_SH->GetPictureHeight()/2 + 15)/16;
        MEParams->pRefF[1]->padding = 16;
    }

    MEParams->bSecondField       = bSecondField;

    MEParams->Interpolation      = (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode) ?
                                     UMC::ME_VC1_Bilinear :UMC::ME_VC1_Bicubic;
    MEParams->ChromaInterpolation = UMC::ME_VC1_Bilinear;

    MEParams->SearchDirection    =  UMC::ME_ForwardSearch;
    MEParams->PixelType          =  (VC1_ENC_1MV_HALF_BILINEAR == m_InitPicParam.uiMVMode
                                   || VC1_ENC_1MV_HALF_BICUBIC == m_InitPicParam.uiMVMode) ?
                                        UMC::ME_HalfPixel : UMC::ME_QuarterPixel;
    MEParams->MbPart             =  UMC::ME_Mb16x16;

    MEParams->PicRange.top_left.x       = -15;
    MEParams->PicRange.top_left.y       = -15;

    MEParams->PicRange.bottom_right.x   = m_SH->GetPictureWidth()   +15 ;
    MEParams->PicRange.bottom_right.y   = m_SH->GetPictureHeight()/2+15;

    MEParams->FirstMB = 0;
    MEParams->LastMB  = (((m_SH->GetPictureHeight()/2)+15)/16) * ((m_SH->GetPictureWidth() +15)/16) - 1;

    MEParams->SearchRange.x             = MVRange[2*m_InitPicParam.uiMVRangeIndex]-1;
    MEParams->SearchRange.y             = MVRange[2*m_InitPicParam.uiMVRangeIndex + 1]-1;

    InitScaleInfo(&scInfo,bSecondField,bBottom,m_nReferenceFrameDist,m_InitPicParam.uiMVRangeIndex);

    //scale info
    MEParams->ScaleInfo[0].ME_Bottom = scInfo.bBottom;

    MEParams->ScaleInfo[0].ME_RangeX = scInfo.rangeX;
    MEParams->ScaleInfo[0].ME_RangeY = scInfo.rangeY;

    MEParams->ScaleInfo[0].ME_ScaleOpp = scInfo.scale_opp;

    MEParams->ScaleInfo[0].ME_ScaleSame1 = scInfo.scale_same1;
    MEParams->ScaleInfo[0].ME_ScaleSame2 = scInfo.scale_same2;

    MEParams->ScaleInfo[0].ME_ScaleZoneX = scInfo.scale_zoneX;
    MEParams->ScaleInfo[0].ME_ScaleZoneY = scInfo.scale_zoneY;

    MEParams->ScaleInfo[0].ME_ZoneOffsetX = scInfo.zone_offsetX;
    MEParams->ScaleInfo[0].ME_ZoneOffsetY = scInfo.zone_offsetY;

    return UMC::UMC_OK;
}

UMC::Status VC1EncoderADV::SetInitPictureParams(ePType PicType)
{
        m_InitPicParam.uiReferenceFieldType = m_uiReferenceFieldType;

        switch (PicType)
        {
        case VC1_ENC_I_FRAME:
            break;
        case VC1_ENC_P_FRAME:
            m_InitPicParam.uiMVMode = VC1_ENC_1MV_HALF_BILINEAR;
            m_InitPicParam.uiMVRangeIndex = 0;
            break;
        case VC1_ENC_P_FRAME_MIXED:
            m_InitPicParam.uiMVMode = VC1_ENC_MIXED_QUARTER_BICUBIC;
             m_InitPicParam.uiMVRangeIndex = 0;
            break;
        case VC1_ENC_B_FRAME:
            m_InitPicParam.uiMVMode = VC1_ENC_1MV_HALF_BILINEAR;
            m_InitPicParam.uiBFraction.num = 1;
            m_InitPicParam.uiBFraction.denom = 2;
            m_InitPicParam.uiReferenceFieldType = VC1_ENC_REF_FIELD_BOTH;

            break;
        case VC1_ENC_SKIP_FRAME:
            break;
        case VC1_ENC_I_I_FIELD:
            break;
        case VC1_ENC_I_P_FIELD:
            break;
        case VC1_ENC_P_I_FIELD:
            break;
        case VC1_ENC_P_P_FIELD:
            break;
        case VC1_ENC_B_BI_FIELD:
            m_InitPicParam.uiReferenceFieldType = VC1_ENC_REF_FIELD_BOTH;
            break;
        case VC1_ENC_BI_B_FIELD:
            m_InitPicParam.uiReferenceFieldType = VC1_ENC_REF_FIELD_BOTH;
            break;
        case VC1_ENC_B_B_FIELD:
            m_InitPicParam.uiReferenceFieldType = VC1_ENC_REF_FIELD_BOTH;
            break;
        default:
            return UMC::UMC_ERR_FAILED;
        }

        m_InitPicParam.nReferenceFrameDist = (Ipp8u)m_pGOP->GetNumberOfB();

        return UMC::UMC_OK;
}

UMC::Status VC1EncoderADV::MEPictureParamsRefine(UMC::MeParams* MEParams)
{
        if(MEParams->ChangeInterpPixelType)
        {
            if(MEParams->MbPart == UMC::ME_Mb16x16 && MEParams->PixelType == UMC::ME_HalfPixel
                && MEParams->Interpolation == UMC::ME_VC1_Bilinear)
                m_InitPicParam.uiMVMode = VC1_ENC_1MV_HALF_BILINEAR;
            else
            if(MEParams->MbPart == UMC::ME_Mb16x16 && MEParams->PixelType == UMC::ME_QuarterPixel
                && MEParams->Interpolation == UMC::ME_VC1_Bicubic)
                m_InitPicParam.uiMVMode = VC1_ENC_1MV_QUARTER_BICUBIC;
            else
            if(MEParams->MbPart == UMC::ME_Mb16x16 && MEParams->PixelType == UMC::ME_HalfPixel
                && MEParams->Interpolation == UMC::ME_VC1_Bicubic)
                m_InitPicParam.uiMVMode = VC1_ENC_1MV_HALF_BICUBIC;
            else
            if(MEParams->MbPart == UMC::ME_Mb8x8 && MEParams->PixelType == UMC::ME_QuarterPixel
                && MEParams->Interpolation == UMC::ME_VC1_Bicubic)
                m_InitPicParam.uiMVMode = VC1_ENC_MIXED_QUARTER_BICUBIC;
        }

        if(MEParams->SelectVlcTables)
        {
            m_VLCIndex.uiMVTab      = (Ipp8u)MEParams->OutMvTableIndex;
            m_VLCIndex.uiDecTypeAC1 = (Ipp8u)MEParams->OutAcTableIndex;
            m_VLCIndex.m_uiCBPTab   = (Ipp8u)MEParams->OutCbpcyTableIndex;
        }
    return UMC::UMC_OK;
}

UMC::Status VC1EncoderADV::SetVLCTablesIndex()
{
    m_VLCIndex.uiMVTab = 0;
    m_VLCIndex.uiDecTypeAC1 = 0;
    m_VLCIndex.uiDecTypeDCIntra = 0;
    m_VLCIndex.m_uiCBPTab = 0;

    return UMC::UMC_OK;
}
}

#endif //defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
