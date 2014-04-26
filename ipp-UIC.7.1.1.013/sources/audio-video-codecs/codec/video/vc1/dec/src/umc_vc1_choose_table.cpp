/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1(VC1) decoder, choose table functions
//
*/
#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_run_level_tbl.h"
#include "umc_vc1_common_mvdiff_tbl.h"
#include "umc_vc1_common_interlace_mv_tables.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_debug.h"

#include "ipps.h"


void ChooseDCTable(VC1Context* pContext, Ipp32s transDCtableIndex)
{
    Ipp32s *lumaTable_lut[]  ={pContext->m_vlcTbl->m_pLowMotionLumaDCDiff,
                               pContext->m_vlcTbl->m_pHighMotionLumaDCDiff};
    Ipp32s *chromaTable_lut[]={pContext->m_vlcTbl->m_pLowMotionChromaDCDiff,
                               pContext->m_vlcTbl->m_pHighMotionChromaDCDiff};

    pContext->m_picLayerHeader->m_pCurrLumaDCDiff   = lumaTable_lut[transDCtableIndex];
    pContext->m_picLayerHeader->m_pCurrChromaDCDiff = chromaTable_lut[transDCtableIndex];
}

void ChooseACTable(VC1Context* pContext,
                   Ipp32s transACtableIndex1,
                   Ipp32s transACtableIndex2)
{
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1ACDecodeSet* IntraACDecodeSet_lut[] =
    {
        &pContext->m_vlcTbl->HighRateIntraACDecodeSet, &pContext->m_vlcTbl->HighMotionIntraACDecodeSet,
        &pContext->m_vlcTbl->MidRateIntraACDecodeSet,  &pContext->m_vlcTbl->LowMotionIntraACDecodeSet,
        &pContext->m_vlcTbl->HighMotionIntraACDecodeSet, &pContext->m_vlcTbl->MidRateIntraACDecodeSet
    };
    VC1ACDecodeSet* IntrerACDecodeSet_lut[] =
    {
        &pContext->m_vlcTbl->HighRateInterACDecodeSet,     &pContext->m_vlcTbl->HighMotionInterACDecodeSet,
        &pContext->m_vlcTbl->MidRateInterACDecodeSet,      &pContext->m_vlcTbl->LowMotionInterACDecodeSet,
        &pContext->m_vlcTbl->HighMotionInterACDecodeSet,   &pContext->m_vlcTbl->MidRateInterACDecodeSet
    };

    if(picLayerHeader->PQINDEX <= 8)
    {
        picLayerHeader->m_pCurrIntraACDecSet = IntraACDecodeSet_lut[transACtableIndex2];
        picLayerHeader->m_pCurrInterACDecSet = IntrerACDecodeSet_lut[transACtableIndex1];
    }
    else
    {
        picLayerHeader->m_pCurrIntraACDecSet = IntraACDecodeSet_lut[transACtableIndex2+3];
        picLayerHeader->m_pCurrInterACDecSet = IntrerACDecodeSet_lut[transACtableIndex1+3];
    }
}


void ChooseTTMB_TTBLK_SBP(VC1Context* pContext)
{
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1VLCTables* VLCTables = pContext->m_vlcTbl;

    if( picLayerHeader->PQUANT < 5)
    {
        picLayerHeader->m_pCurrTTMBtbl = VLCTables->TTMB_PB_TABLES[2];
        picLayerHeader->m_pCurrTTBLKtbl = VLCTables->TTBLK_PB_TABLES[2];
        picLayerHeader->m_pCurrSBPtbl = VLCTables->SBP_PB_TABLES[0];
    }
    else if(picLayerHeader->PQUANT < 13)
    {
        picLayerHeader->m_pCurrTTMBtbl = VLCTables->TTMB_PB_TABLES[1];
        picLayerHeader->m_pCurrTTBLKtbl =VLCTables->TTBLK_PB_TABLES[1];
        picLayerHeader->m_pCurrSBPtbl = VLCTables->SBP_PB_TABLES[1];
    }
    else
    {
        picLayerHeader->m_pCurrTTMBtbl = VLCTables->TTMB_PB_TABLES[0];
        picLayerHeader->m_pCurrTTBLKtbl = VLCTables->TTBLK_PB_TABLES[0];
        picLayerHeader->m_pCurrSBPtbl = VLCTables->SBP_PB_TABLES[2];
    }
}
void ExpandRows(Ipp8u* StartPtr,
                 Ipp32s width,
                 Ipp32s height,
                 Ipp32s pitch,
                 Ipp32s padSize,
                 Ipp8u  isFirstOrLastRow) // 0 - NoFirst/NoLast, 0x01 - First, 0x10 - Last
{
    Ipp32s i, m;
    Ipp8u *p, *p1, *pc;
    IppiSize roiSize;
    Ipp32s AddPad = 0;
    roiSize.height = padSize;


    if (2 & isFirstOrLastRow)
    {

        //Area (1)
        p = StartPtr + pitch * (height - 1);
        pc = p + pitch;

        roiSize.height = padSize;
        roiSize.width = width;
        ippiCopy_8u_C1R(p, 0, pc, pitch, roiSize);
        AddPad = padSize;
    }

    //Area (2)
    p = StartPtr;
    p1 = StartPtr + width;
    m = height + AddPad;

    for(i = 0; i < m; i++)
    {
        ippsSet_8u(p[0], p-padSize, padSize);
        ippsSet_8u(p1[-1], p1, padSize);
        p  += pitch;
        p1 += pitch;
    }

    if (1 & isFirstOrLastRow)
    {

        //Area (3)
        p = StartPtr - padSize;
        pc = p - pitch;
        m = width + 2*padSize;
        roiSize.width = m;
        ippiCopy_8u_C1R(p, 0, pc, -pitch, roiSize);
    }

}


void ExpandMBRowSM(VC1Context* pContext, Ipp32s StartMBPos, Ipp32s HeightMB)
{
    Ipp8u FirstLastMask = 0;
    IppiSize    roiSize;
    roiSize.width  =    pContext->m_seqLayerHeader->widthMB  * VC1_PIXEL_IN_LUMA;
    roiSize.height =    HeightMB * VC1_PIXEL_IN_LUMA;

    if (0 == StartMBPos)
        FirstLastMask |=    1;
    if (pContext->m_seqLayerHeader->heightMB == (StartMBPos + HeightMB))
        FirstLastMask |=    2;

    ExpandRows(pContext->m_pCurrMB->currYPlane, roiSize.width,   roiSize.height,   pContext->m_pCurrMB->currYPitch, 64, FirstLastMask);
    ExpandRows(pContext->m_pCurrMB->currUPlane, roiSize.width/2, roiSize.height/2, pContext->m_pCurrMB->currUPitch, 32, FirstLastMask);
    ExpandRows(pContext->m_pCurrMB->currVPlane, roiSize.width/2, roiSize.height/2, pContext->m_pCurrMB->currVPitch, 32, FirstLastMask);
}

void ExpandPlane(Ipp8u* StartPtr, Ipp32s width, Ipp32s height, Ipp32s pitch, Ipp32s padSize)
{
    Ipp32s i, m;
    Ipp8u *p, *p1, *pc;
    IppiSize roiSize;

    //Area (1)
    p = StartPtr + pitch * (height - 1);
    pc = p + pitch;

    roiSize.height = padSize;
    roiSize.width = width;
    ippiCopy_8u_C1R(p, 0, pc, pitch, roiSize);

    //Area (2)
    p = StartPtr;
    p1 = StartPtr + width;
    m = height + padSize;

    for(i = 0; i < m; i++)
    {
        ippsSet_8u(p[0], p-padSize, padSize);
        ippsSet_8u(p1[-1], p1, padSize);
        p  += pitch;
        p1 += pitch;
    }

    //Area (3)
    p = StartPtr - padSize;
    pc = p - pitch;
    m = width + 2*padSize;

    roiSize.width = m;
    ippiCopy_8u_C1R(p, 0, pc, -pitch, roiSize);

}

void ExpandPlane_Interlace(Ipp8u* StartPtr, Ipp32s width, Ipp32s height, Ipp32s pitch, Ipp32s padSize)
{
    Ipp32s i, m;// j;
    Ipp8u *p, *p1, *pc;
    IppiSize roiSize;

    //Area (1)
    p = StartPtr + pitch * (height - 1);
    pc = p + 2*pitch;

    roiSize.height = padSize/2;
    roiSize.width = width;
    ippiCopy_8u_C1R(p, 0, pc, 2*pitch, roiSize);


    p = StartPtr + pitch * (height - 2);
    pc = p + 2*pitch;

    ippiCopy_8u_C1R(p, 0, pc, 2*pitch, roiSize);

    //Area (2)
    p = StartPtr;
    p1 = StartPtr + width;
    m = height + padSize;

    for(i = 0; i < m; i++)
    {
        ippsSet_8u(p[0], p-padSize, padSize);
        ippsSet_8u(p1[-1], p1, padSize);
        p  += pitch;
        p1 += pitch;
    }

    //Area (3)
    p = StartPtr - padSize;
    pc = p - 2*pitch;
    m = width + 2*padSize;

    roiSize.width = m;
    ippiCopy_8u_C1R(p, 0, pc, -2*pitch, roiSize);


    p = StartPtr + pitch - padSize;
    pc = p - 2*pitch;
    m = width + 2*padSize;

    roiSize.width = m;
    ippiCopy_8u_C1R(p, 0, pc, -2*pitch, roiSize);
}


void ExpandPlane_Field(Ipp8u* StartPtr, Ipp32s width, Ipp32s height, Ipp32s pitch, Ipp32s padSize,Ipp32s bottom)
{
    Ipp32s i, m;
    Ipp8u *p, *p1, *pc;
    Ipp32s offset_up = 0, offset_down =0;
    IppiSize roiSize;

    if (bottom)
    {
        offset_down = 1;
        offset_up   = pitch;
    } else
    {
        offset_down = 2;
        offset_up   = 0;
    }

    //Area (1)
    p = StartPtr + pitch * (height - offset_down);
    pc = p + 2*pitch;

    roiSize.height = padSize/2;
    roiSize.width = width;
    ippiCopy_8u_C1R(p, 0, pc, 2*pitch, roiSize);

    //Area (2)
    p = StartPtr + offset_up;
    p1 = StartPtr + width + offset_up;
    m = height + padSize;

    for(i = 0; i < m/2; i++)
    {
        ippsSet_8u(p[0], p-padSize, padSize);
        ippsSet_8u(p1[-1], p1, padSize);
        p  += 2*pitch;
        p1 += 2*pitch;
    }

    //Area (3)
    p = StartPtr +offset_up - padSize;
    pc = p - 2*pitch;
    m = width + 2*padSize;

    roiSize.width = m;
    ippiCopy_8u_C1R(p, 0, pc, -2*pitch, roiSize);
}

void ExpandFrame(Frame* pFrame,IppiSize roiSize)
{
    //if(pFrame->m_bIsExpanded == 0)
    //{
    //    ExpandPlane(pFrame->m_pY, roiSize.width,   roiSize.height,   pFrame->m_iYPitch, 64);
    //    ExpandPlane(pFrame->m_pU, roiSize.width/2, roiSize.height/2, pFrame->m_iUPitch, 32);
    //    ExpandPlane(pFrame->m_pV, roiSize.width/2, roiSize.height/2, pFrame->m_iVPitch, 32);
    //    pFrame->m_bIsExpanded = 1;
    //}

}
void ExpandFrame_Adv(VC1Context* pContext, Frame* pFrame)
{
    //Ipp32s width  = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    //Ipp32s height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    //if(pFrame->m_bIsExpanded == 0)
    //{
    //    ExpandPlane(pFrame->m_pY, width, height,     pFrame->m_iYPitch, 64);
    //    ExpandPlane(pFrame->m_pU, width/2, height/2, pFrame->m_iUPitch, 32);
    //    ExpandPlane(pFrame->m_pV, width/2, height/2, pFrame->m_iVPitch, 32);
    //    pFrame->m_bIsExpanded = 1;
    //}
}

void ExpandFrame_Interlace(VC1Context* pContext, Frame* pFrame)
{
//    Ipp32s width  = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
//    Ipp32s height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
//
//    if((pFrame->m_bIsExpanded == 0)||(pContext->m_picLayerHeader->FCM == VC1_FieldInterlace))
//    {
//        ExpandPlane_Interlace(pFrame->m_pY, width, height,     pFrame->m_iYPitch, 64);
//        ExpandPlane_Interlace(pFrame->m_pU, width/2, height/2, pFrame->m_iUPitch, 32);
//        ExpandPlane_Interlace(pFrame->m_pV, width/2, height/2, pFrame->m_iVPitch, 32);
//        pFrame->m_bIsExpanded = 1;
//    }
}

void ExpandFrame_Field(VC1Context* pContext, Frame* pFrame, Ipp32u bottom_field)
{
    //Ipp32s width  = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    //Ipp32s height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    //ExpandPlane_Field(pFrame->m_pY, width, height,     pFrame->m_iYPitch, 64, bottom_field);
    //ExpandPlane_Field(pFrame->m_pU, width/2, height/2, pFrame->m_iUPitch, 32, bottom_field);
    //ExpandPlane_Field(pFrame->m_pV, width/2, height/2, pFrame->m_iVPitch, 32, bottom_field);
}

void ChooseMBModeInterlaceFrame(VC1Context* pContext,
                                Ipp32u MV4SWITCH,
                                Ipp32u MBMODETAB)
{
  if(MV4SWITCH == 1)
  {
    pContext->m_picLayerHeader->m_pMBMode =
        pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[MBMODETAB];
  }
  else
  {
    pContext->m_picLayerHeader->m_pMBMode =
        pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[MBMODETAB+4];
  }
}


void ChooseMBModeInterlaceField(VC1Context* pContext, Ipp32s MBMODETAB)
{
  if(pContext->m_picLayerHeader->MVMODE == VC1_MVMODE_MIXED_MV)
  {
    pContext->m_picLayerHeader->m_pMBMode =
        pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[MBMODETAB];
  }
  else
  {
    pContext->m_picLayerHeader->m_pMBMode =
        pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[MBMODETAB];
  }
}

 void ChoosePredScaleValuePPictbl(VC1PictureLayerHeader* picLayerHeader)
 {
    if(picLayerHeader->CurrField == 0)
    {
        picLayerHeader->m_pCurrPredScaleValuePPictbl
            = &VC1_PredictScaleValuesPPicTbl1[picLayerHeader->REFDIST];
    }
    else
        picLayerHeader->m_pCurrPredScaleValuePPictbl
            = &VC1_PredictScaleValuesPPicTbl2[picLayerHeader->REFDIST];
 }

 void ChoosePredScaleValueBPictbl(VC1PictureLayerHeader* picLayerHeader)
 {

     Ipp32u FREFDIST = ((picLayerHeader->ScaleFactor * picLayerHeader->REFDIST) >> 8);
     Ipp32s BREFDIST = picLayerHeader->REFDIST - FREFDIST - 1;

     if (BREFDIST < 0)
         BREFDIST = 0;

     picLayerHeader->m_pCurrPredScaleValueB_BPictbl = &VC1_PredictScaleValuesBPicTbl1[BREFDIST];
     picLayerHeader->m_pCurrPredScaleValueP_BPictbl[1] = &VC1_PredictScaleValuesPPicTbl1[BREFDIST];


    if(picLayerHeader->CurrField == 0)
    {
        picLayerHeader->m_pCurrPredScaleValueP_BPictbl[0] = &VC1_PredictScaleValuesPPicTbl1[FREFDIST];
    }
    else
    {
        picLayerHeader->m_pCurrPredScaleValueP_BPictbl[0] = &VC1_PredictScaleValuesPPicTbl2[FREFDIST];
    }

 }

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
