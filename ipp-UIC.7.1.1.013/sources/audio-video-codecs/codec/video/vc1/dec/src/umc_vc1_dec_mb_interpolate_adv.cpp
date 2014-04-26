/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, MB Layer common for simple\main profiles
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_defs.h"

#include "ippvc.h"


static const Ipp32s blk_table[] = {0,0,0,0,1,1};

typedef void (*CropMV) (VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV);

static const CropMV CropLuma_tbl[] = {
    (CropMV)(CropLumaPullBack),
    (CropMV)(CropLumaPullBack),
    (CropMV)(NULL),
    (CropMV)(CropLumaPullBack_Adv)
};

static const CropMV CropChroma_tbl[] = {
    (CropMV)(CropChromaPullBack),
    (CropMV)(CropChromaPullBack),
    (CropMV)(NULL),
    (CropMV)(CropChromaPullBack_Adv)
};


static const Ipp16s offset_table_x[] = {0,32,0, 32};
static const Ipp16s offset_table_y[] = {0,0, 32,32};


// Progressive Simple/Main/Advanced
static void CalcMVInterpolateProgLuma(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    CropLuma_tbl[pContext->m_seqLayerHeader->PROFILE](pContext, xMV, yMV);

    *xMV = (Ipp16s)((X << 6) + *xMV);
    *yMV = (Ipp16s)((Y << 6) + *yMV);

}
static void CalcMVInterpolateProgChroma1MV(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    if (pContext->m_picLayerHeader->FCM == VC1_FrameInterlace)
        DeriveSecondStageChromaMV_Interlace(pContext, xMV, yMV);
    else
        DeriveSecondStageChromaMV(pContext, xMV, yMV);

    CropChroma_tbl[pContext->m_seqLayerHeader->PROFILE](pContext, xMV, yMV);

    *xMV = (Ipp16s)((X << 5) + *xMV);
    *yMV = (Ipp16s)((Y << 5) + *yMV);
}

static void CalcMVInterpolateProgChroma4MV(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    Derive4MV(sMB, xMV,yMV);

    if (pContext->m_picLayerHeader->FCM == VC1_FrameInterlace)
        DeriveSecondStageChromaMV_Interlace(pContext, xMV, yMV);
    else
        DeriveSecondStageChromaMV(pContext, xMV, yMV);

    CropChroma_tbl[pContext->m_seqLayerHeader->PROFILE](pContext, xMV, yMV);

    *xMV = (Ipp16s)((X << 5) + *xMV);
    *yMV = (Ipp16s)((Y << 5) + *yMV);

}
// Interlaced Frame
static Ipp32s CalcMVInterpolateInterlacedLuma(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s f = 0;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    CropLumaPullBack_Adv(pContext, xMV, yMV);

    X = (X << 6) + *xMV;
    Y = (Y << 6) + *yMV;

    f = (Y&4)>>2;

    Y>>=1;

    *xMV = (Ipp16s)X;
    *yMV = (Ipp16s)((Y & 0xFFFFFC) + (*yMV&3));

    return f;
}
static Ipp32s CalcMVInterpolateInterlacedChroma2MV(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s f = 0;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    DeriveSecondStageChromaMV_Interlace(pContext, xMV, yMV);
    CropChromaPullBack_Adv(pContext, xMV, yMV);

    X = (X << 5) + *xMV;
    Y = (Y << 5) + *yMV;

    f = (Y&4)>>2;

    Y>>=1;

    *xMV = (Ipp16s)X;
    *yMV = (Ipp16s)((Y & 0xFFFFFC) + (*yMV&3));

    return f;
}
// Interlace Field
static Ipp32s CalcMVInterpolateFieldLuma(VC1Context* pContext, Ipp32s polarity,
                                         Ipp16s* xMV, Ipp16s* yMV)
{
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;
    Ipp32s X, Y;
    Ipp32s f = 0;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    if (pPicHeader->CurrField)
       Y -= pContext->m_pSingleMB->heightMB/2;
    if (!polarity)
    {
        *yMV = *yMV + (pPicHeader->BottomField<<2) - 2;
        f = !pPicHeader->BottomField;
    }
    else
    {
        f = pPicHeader->BottomField;
    }

    CropLumaPullBackField_Adv(pContext, xMV, yMV);

    *xMV = (Ipp16s)((X << 6) + *xMV);
    *yMV = (Ipp16s)((Y << 6) + *yMV);

    return f;
}


static Ipp32s CalcMVInterpolateFieldChroma(VC1Context* pContext, Ipp32s polarity,
                                           Ipp16s* xMV, Ipp16s* yMV)
{
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;
    Ipp32s X, Y;
    Ipp32s f = 0;

    X = pContext->m_pSingleMB->m_currMBXpos;
    Y = pContext->m_pSingleMB->m_currMBYpos;

   if (pPicHeader->CurrField)
       Y -= sMB->heightMB/2;

  DeriveSecondStageChromaMV(pContext, xMV, yMV);

  if(!polarity)
   {
       f = !pPicHeader->BottomField;
       *yMV = *yMV + (pPicHeader->BottomField<<2) - 2;
   }
   else
   {
       f = pPicHeader->BottomField;
   }

   CropChromaPullBack_Adv(pContext, xMV, yMV);

   *xMV = (Ipp16s)((X << 5) + *xMV);
   *yMV = (Ipp16s)((Y << 5) + *yMV);

    return f;
}

static Ipp32s CalcMVInterpolateInterlacedChroma4MV(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV,
                                                   Ipp32s field, Ipp32s sblk_num)
{
    Ipp32s f = 0;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s X, Y;

    X = sMB->m_currMBXpos;
    Y = sMB->m_currMBYpos;

    DeriveSecondStageChromaMV_Interlace(pContext, xMV, yMV);
    CropChromaPullBack_Adv(pContext, xMV, yMV);

    X = (X << 5) + *xMV;
    Y = (Y << 5) + *yMV;

    if (field)
    {
        f  = (Y & 4) >> 2;
        Y = Y >> 1;
    }

    if (sblk_num & 1)
        X += 16;

    if (sblk_num & 2)
    {
        if (field)
        {
            if (f==1)
                Y = Y + 4;
            f ^= 1;
        }
        else
            Y +=16;
    }

    *xMV = (Ipp16s) X;
    *yMV = (Ipp16s)((Y & 0xFFFFFC) + (*yMV & 3));
    return f;
}
static void SetChromaTblPrev(VC1Context* pContext, Ipp8u** pTopTbl, Ipp8u** pBottomTbl, Ipp32s index)
{
    *pTopTbl = pContext->m_frmBuff.m_pFrames[index].ChromaTablePrev[0];
    *pBottomTbl = pContext->m_frmBuff.m_pFrames[index].ChromaTablePrev[1];
}
static void SetChromaTblCurr(VC1Context* pContext, Ipp8u** pTopTbl, Ipp8u** pBottomTbl, Ipp32s index)
{
    *pTopTbl = pContext->m_frmBuff.m_pFrames[index].ChromaTableCurr[0];
    *pBottomTbl = pContext->m_frmBuff.m_pFrames[index].ChromaTableCurr[1];
}
static void SetLumaTblPrev(VC1Context* pContext, Ipp8u** pTopTbl, Ipp8u** pBottomTbl, Ipp32s index)
{
    *pTopTbl = pContext->m_frmBuff.m_pFrames[index].LumaTablePrev[0];
    *pBottomTbl = pContext->m_frmBuff.m_pFrames[index].LumaTablePrev[1];
}
static void SetLumaTblCurr(VC1Context* pContext, Ipp8u** pTopTbl, Ipp8u** pBottomTbl, Ipp32s index)
{
    *pTopTbl = pContext->m_frmBuff.m_pFrames[index].LumaTableCurr[0];
    *pBottomTbl = pContext->m_frmBuff.m_pFrames[index].LumaTableCurr[1];
}

static VC1Status InterpolateBlock_ProgressivePictureLuma1MV_P_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst,
                                                                               Ipp32u dstStep)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;
    VM_ASSERT (index>-1);

    Ipp16s xMV = pMB->m_pBlocks[0].mv[0][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[0][1];

    CalcMVInterpolateProgLuma(pContext, &xMV, &yMV);

    pContext->interp_params_luma.oppositePadding= (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?0:1;

    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;
    pContext->interp_params_luma.srcStep = dstStep;
    pContext->interp_params_luma.fieldPrediction = 0;
    pContext->interp_params_luma.isPredBottom = 0;

    SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);



    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = pContext->interp_params_luma.dstStep;
    if (VC1_PROFILE_ADVANCED == pContext->m_seqLayerHeader->PROFILE)
    {
        pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
        pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    }
    else
    {
        pContext->interp_params_luma.sizeFrame.width = pContext->m_seqLayerHeader->widthMB * 16;
        pContext->interp_params_luma.sizeFrame.height = pContext->m_seqLayerHeader->heightMB * 16;
    }
    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_LUMA;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_LUMA;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    if (VC1_MVMODE_HPELBI_1MV == pContext->m_picLayerHeader->MVMODE)
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    else
        ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

   return VC1_OK;
}


static VC1Status InterpolateBlock_ProgressivePictureLuma1MV_B_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst,
                                                                               Ipp32u dstStep,
                                                                               Ipp32s back)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    VM_ASSERT (index>-1);

    Ipp16s xMV = pMB->m_pBlocks[0].mv[back][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[back][1];



    CalcMVInterpolateProgLuma(pContext, &xMV, &yMV);

    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?0:1;
    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;
    pContext->interp_params_luma.srcStep = pContext->interp_params_luma.srcStep;
    pContext->interp_params_luma.fieldPrediction = 0;
    pContext->interp_params_luma.isPredBottom = 0;

    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;
    if (VC1_PROFILE_ADVANCED == pContext->m_seqLayerHeader->PROFILE)
    {
        pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
        pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    }
    else
    {
        pContext->interp_params_luma.sizeFrame.width = pContext->m_seqLayerHeader->widthMB * 16;
        pContext->interp_params_luma.sizeFrame.height = pContext->m_seqLayerHeader->heightMB * 16;
    }
    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_LUMA;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_LUMA;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    if (back == 0)
        SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    else
        SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);



    if (VC1_MVMODE_HPELBI_1MV == pContext->m_picLayerHeader->MVMODE)
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    else
        ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    return VC1_OK;
}


static VC1Status InterpolateBlock_ProgressivePictureLuma4MV_NewInterpolation(VC1Context* pContext,
                                                                             Ipp8u *pDst,
                                                                             Ipp32u dstStep,
                                                                             Ipp32s blk_num)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;

    Ipp16s xMV = pMB->m_pBlocks[blk_num].mv[0][0];
    Ipp16s yMV = pMB->m_pBlocks[blk_num].mv[0][1];

    VM_ASSERT (index>-1);

    CalcMVInterpolateProgLuma(pContext, &xMV, &yMV);


    xMV = xMV + offset_table_x[blk_num];
    yMV = yMV + offset_table_y[blk_num];


    pContext->interp_params_luma.oppositePadding  = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?0:1;
    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;
    pContext->interp_params_luma.srcStep = pContext->interp_params_luma.srcStep;
    pContext->interp_params_luma.fieldPrediction = 0;
    pContext->interp_params_luma.isPredBottom = 0;

    SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;
    if (VC1_PROFILE_ADVANCED == pContext->m_seqLayerHeader->PROFILE)
    {
        pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
        pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    }
    else
    {
        pContext->interp_params_luma.sizeFrame.width = pContext->m_seqLayerHeader->widthMB * 16;
        pContext->interp_params_luma.sizeFrame.height = pContext->m_seqLayerHeader->heightMB * 16;
    }
    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    return VC1_OK;
}


static VC1Status InterpolateBlock_ProgressivePictureChroma1MV_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst1,
                                                                               Ipp32s dstStep1,
                                                                               Ipp8u *pDst2,
                                                                               Ipp32s dstStep2,
                                                                               Ipp32s back)
{
    VC1MB *pCurrMB = pContext->m_pCurrMB;

    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    Ipp8u* pUPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pU;
    Ipp8u* pVPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pV;

    Ipp16s xMV = pCurrMB->m_pBlocks[0].mv[back][0];
    Ipp16s yMV = pCurrMB->m_pBlocks[0].mv[back][1];

    VM_ASSERT (index>-1);


    CalcMVInterpolateProgChroma1MV(pContext, &xMV, &yMV);


    //IppVCInterpolateBlock_8u interpolateInfo;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?0:1;
    pContext->interp_params_chroma.isPredBottom = 0;
    pContext->interp_params_chroma.fieldPrediction = 0;

    if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
    {
        if (0 == back)
            SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        else
            SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    }
    else
    {
        SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);
    }


    if (VC1_PROFILE_ADVANCED == pContext->m_seqLayerHeader->PROFILE)
    {
        pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
        pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    }
    else
    {
        pContext->interp_params_chroma.sizeFrame.width = pContext->m_seqLayerHeader->widthMB * VC1_PIXEL_IN_BLOCK;
        pContext->interp_params_chroma.sizeFrame.height = pContext->m_seqLayerHeader->heightMB * VC1_PIXEL_IN_BLOCK;
    }
    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;


    //U channel
    pContext->interp_params_chroma.pSrc = pUPlaneRef;
    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    //!!!!. Not begin of image, already current mb
    pContext->interp_params_chroma.pDst = pDst1;
    pContext->interp_params_chroma.dstStep = dstStep1;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    //V channel
    pContext->interp_params_chroma.pSrc = pVPlaneRef;
    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    //!!!!. Not begin of image, already current mb
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    return VC1_OK;
}


static VC1Status InterpolateBlock_ProgressivePictureChroma4MV_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst1,
                                                                               Ipp32s dstStep1,
                                                                               Ipp8u *pDst2,
                                                                               Ipp32s dstStep2,
                                                                               Ipp32s back)
{
    VC1MB *pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;

    Ipp8u* pUPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pU;
    Ipp8u* pVPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pV;

    Ipp16s xMV, yMV;
    VM_ASSERT (index>-1);

    //U channel
    xMV = pCurrMB->m_pBlocks[4].mv[back][0];
    yMV = pCurrMB->m_pBlocks[4].mv[back][1];

    Ipp32s count=0;
    sMB->MVcount =0;
    for (count = 0; count < 4; count++)
    {
        if (VC1_BLK_INTER & pCurrMB->m_pBlocks[count].blkType)
        {
            sMB->xLuMV[sMB->MVcount] = pCurrMB->m_pBlocks[count].mv[back][0];
            sMB->yLuMV[sMB->MVcount] = pCurrMB->m_pBlocks[count].mv[back][1];
            ++sMB->MVcount;
        }
    }

    CalcMVInterpolateProgChroma4MV(pContext, &xMV, &yMV);
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?0:1;
    pContext->interp_params_chroma.isPredBottom = 0;
    pContext->interp_params_chroma.fieldPrediction = 0;
    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;


    SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    if (VC1_PROFILE_ADVANCED == pContext->m_seqLayerHeader->PROFILE)
    {
        pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
        pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    }
    else
    {
        pContext->interp_params_chroma.sizeFrame.width = pContext->m_seqLayerHeader->widthMB * VC1_PIXEL_IN_BLOCK;
        pContext->interp_params_chroma.sizeFrame.height = pContext->m_seqLayerHeader->heightMB * VC1_PIXEL_IN_BLOCK;
    }
    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;


    //U channel
    pContext->interp_params_chroma.pSrc = pUPlaneRef;
    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    //!!!!. Not begin of image, already current mb
    pContext->interp_params_chroma.pDst = pDst1;
    pContext->interp_params_chroma.dstStep = dstStep1;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    //V channel
    pContext->interp_params_chroma.pSrc = pVPlaneRef;
    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    //!!!!. Not begin of image, already current mb
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
    return VC1_OK;
}




static VC1Status InterpolateBlock_InterlacePictureLuma2MV_NewInterpolation(VC1Context* pContext,
                                                                           Ipp8u *pDst,
                                                                           Ipp32s dstStep,
                                                                           Ipp32s back1,
                                                                           Ipp32s back2)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = (back1)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    Ipp16s xMV, yMV;
    Ipp32s f = 0;



    xMV = pMB->m_pBlocks[0].mv[back1][0];
    yMV = pMB->m_pBlocks[0].mv[back1][1];

    //first field
    f = CalcMVInterpolateInterlacedLuma(pContext, &xMV, &yMV);
    pContext->interp_params_luma.fieldPrediction = 1;
    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM == VC1_Progressive)?1:0;
    pContext->interp_params_luma.pLUTTop = 0;
    pContext->interp_params_luma.pLUTBottom = 0;
    pContext->interp_params_luma.isPredBottom = f;
    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;


    if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
    {
        if (0 == back1)
            SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        else
            SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    }
    else
    {
        SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);
    }

    if (f)
    {
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;
    }


    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;
    pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_LUMA;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

     //second field
     index = (back2)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
     pContext->interp_params_luma.pLUTTop = 0;
     pContext->interp_params_luma.pLUTBottom = 0;
     pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM == VC1_Progressive)?1:0;
     pContext->interp_params_luma.fieldPrediction = 1;
     if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
     {
         if (0 == back2)
             SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
         else
             SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
     }
     else
     {
         SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);
     }
     xMV = pMB->m_pBlocks[2].mv_bottom[back2][0];
     yMV = pMB->m_pBlocks[2].mv_bottom[back2][1];

     f = CalcMVInterpolateInterlacedLuma(pContext, &xMV, &yMV);

     pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;
     pContext->interp_params_luma.isPredBottom = 1 - f;

     if (f == 1)
        yMV = yMV + 4;
     else
        pContext->interp_params_luma.pSrc += (pContext->interp_params_luma.srcStep);

    pContext->interp_params_luma.pDst= pDst + 8*dstStep;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;
    
    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    return VC1_OK;
}



static VC1Status InterpolateBlock_InterlacePictureLuma_NewInterpolation(VC1Context* pContext,
                                                                        Ipp8u *pDst,
                                                                        Ipp32s dstStep,
                                                                        Ipp32s blk_num)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;

    Ipp16s xMV;
    Ipp16s yMV;

    Ipp32s f = 0;


    xMV = pMB->m_pBlocks[blk_num].mv[0][0];
    yMV = pMB->m_pBlocks[blk_num].mv[0][1];

    f = CalcMVInterpolateInterlacedLuma(pContext, &xMV, &yMV);

    if(blk_num & 1)
        xMV = xMV + 32;

    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;
    pContext->interp_params_luma.fieldPrediction = 1;
    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM == VC1_Progressive)?1:0;
    pContext->interp_params_luma.pLUTTop = 0;
    pContext->interp_params_luma.pLUTBottom = 0;
    pContext->interp_params_luma.isPredBottom = f;

    if (f)
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;

    SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;

    pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    //second field
    xMV = pMB->m_pBlocks[blk_num + 2].mv_bottom[0][0];
    yMV = pMB->m_pBlocks[blk_num + 2].mv_bottom[0][1];

    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;

    f = CalcMVInterpolateInterlacedLuma(pContext, &xMV, &yMV);

    if (f == 1)
        yMV = yMV + 4;
    else
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;

    if(blk_num&1)
        xMV = xMV + 32;

    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst  = pDst + 8*dstStep;
    pContext->interp_params_luma.isPredBottom = 1 - f;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    return VC1_OK;
}



static VC1Status InterpolateBlock_InterlacePictureChroma2MV_P_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst1,
                                                                               Ipp32s dstStep1,
                                                                               Ipp8u *pDst2,
                                                                               Ipp32s dstStep2)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;

    Ipp8u* pUPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pU;
    Ipp8u* pVPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pV;

    Ipp8u* pRefData = NULL;
    Ipp8u* pCurrData = NULL;

    Ipp32s UPitch = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    Ipp32s VPitch = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;

    Ipp16s xMV=0;
    Ipp16s yMV=0;
    Ipp32s f;

    Ipp32s refPitch;


    //TOP FIELD
    xMV = pMB->m_pBlocks[0].mv[0][0];
    yMV = pMB->m_pBlocks[0].mv[0][1];

    f = CalcMVInterpolateInterlacedChroma2MV(pContext, &xMV, &yMV);

    //4 BLOCK
    refPitch  = UPitch;
    pRefData  = pUPlaneRef;
    pCurrData = pDst1;

    if (f)
        pRefData += refPitch;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.srcStep = refPitch;

    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 1;

    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;
    SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    //!!!!. Not begin of image, already current mb
    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep1*2;

    pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    
    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_CHROMA;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_CHROMA/2;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);




    //5 BLOCK
    refPitch  = VPitch;
    pRefData  = pVPlaneRef;

    if (f)
        pRefData += refPitch;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2*2;


    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);


    //bottom field
    xMV = pMB->m_pBlocks[0].mv_bottom[0][0];
    yMV = pMB->m_pBlocks[0].mv_bottom[0][1];

    f = CalcMVInterpolateInterlacedChroma2MV(pContext, &xMV, &yMV);

    if (f == 1)
        yMV = yMV + 4;

    f ^= 1;

    //BLOCK 4
    refPitch  = UPitch;
    pRefData  = pUPlaneRef;
    pCurrData = pDst1 + dstStep1;

    if (f)
        pRefData += refPitch;

    //refPitch  <<= 1;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;
    pContext->interp_params_chroma.isPredBottom = f;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep1*2;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);


    //BLOCK 5
    refPitch  = VPitch;
    pRefData  = pVPlaneRef;
    pCurrData = pDst2 + dstStep2;

    if (f)
        pRefData += refPitch;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep2*2;
    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
    return VC1_OK;
}

static VC1Status InterpolateBlock_InterlacePictureChroma2MV_B_NewInterpolation(VC1Context* pContext,
                                                                               Ipp8u *pDst1,
                                                                               Ipp32s dstStep1,
                                                                               Ipp8u *pDst2,
                                                                               Ipp32s dstStep2,
                                                                               Ipp32s back1,
                                                                               Ipp32s back2)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = (back1)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    Ipp8u* pUPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pU;
    Ipp8u* pVPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pV;

    Ipp8u* pRefData = NULL;
    Ipp8u* pCurrData = NULL;

    Ipp32s UPitch = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    Ipp32s VPitch = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;

    Ipp16s xMV=0;
    Ipp16s yMV=0;
    Ipp32s f;

    Ipp32s refPitch;


    //TOP FIELD
    xMV = pMB->m_pBlocks[0].mv[back1][0];
    yMV = pMB->m_pBlocks[0].mv[back1][1];

    f = CalcMVInterpolateInterlacedChroma2MV(pContext, &xMV, &yMV);

    //4 BLOCK
    refPitch  = UPitch;
    pRefData  = pUPlaneRef;
    pCurrData = pDst1;

    if (f)
        pRefData += refPitch;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.srcStep = refPitch;

    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 1;

    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;



    if (0 == back1)
        SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    else
        SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);


    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep1*2;
    pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_CHROMA;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_CHROMA/2;
    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;
    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
    //5 BLOCK
    refPitch  = VPitch;
    pRefData  = pVPlaneRef;
    if (f)
        pRefData += refPitch;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2*2;
    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
    //BOTTOM FIELD
    index = (back2)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    pUPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pU;
    pVPlaneRef = pContext->m_frmBuff.m_pFrames[index].m_pV;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 1;
    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;
    if (0 == back2)
        SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    else
        SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);

    pRefData  = pUPlaneRef;

    xMV = pMB->m_pBlocks[0].mv_bottom[back2][0];
    yMV = pMB->m_pBlocks[0].mv_bottom[back2][1];

    f = CalcMVInterpolateInterlacedChroma2MV(pContext, &xMV, &yMV);

    if (f == 1)
        yMV = yMV + 4;
    else
        pRefData += refPitch;


    f ^= 1;

    //BLOCK 4
    refPitch  = UPitch;
    pCurrData = pDst1 + dstStep1;

    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.srcStep = refPitch;

    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 1;


    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;
    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep1*2;
    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);


    //BLOCK 5
    refPitch  = VPitch;
    pRefData  = pVPlaneRef;
    pCurrData = pDst2 + dstStep2;

    if (f)
        pRefData += refPitch;
    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pCurrData;
    pContext->interp_params_chroma.dstStep = dstStep2*2;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    return VC1_OK;
}

static VC1Status InterpolateBlock_InterlaceFieldPictureLuma1MV_P_NewInterpolation(VC1Context* pContext,
                                                                                  Ipp8u *pDst,
                                                                                  Ipp32s dstStep)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;

    Ipp32s f = 0;

    Ipp16s xMV = pMB->m_pBlocks[0].mv[0][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[0][1];

    pContext->interp_params_luma.pLUTTop = 0;
    pContext->interp_params_luma.pLUTBottom = 0;


    if ((pPicHeader->CurrField == 1) &&
        (((pPicHeader->NUMREF + pPicHeader->REFFIELD) ==0) ||
         (pPicHeader->NUMREF == 1) && (!pMB->m_pBlocks[0].mv_s_polarity[0])))
    {
        index = pContext->m_frmBuff.m_iCurrIndex;
    }

    if (index == pContext->m_frmBuff.m_iCurrIndex)
        SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);
    else
        SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);



    f = CalcMVInterpolateFieldLuma(pContext, pMB->m_pBlocks[0].mv_s_polarity[0], &xMV, &yMV);

    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_luma.fieldPrediction = 1;
    pContext->interp_params_luma.isPredBottom = f;


    if (index == pContext->m_frmBuff.m_iCurrIndex)
         pContext->interp_params_luma.oppositePadding = 0;

    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;

    if (f)
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;





    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;

    pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_LUMA;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_LUMA;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    if (VC1_MVMODE_HPELBI_1MV == pContext->m_picLayerHeader->MVMODE)
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    else
        ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    return VC1_OK;
}
static VC1Status InterpolateBlock_InterlaceFieldPictureLuma1MV_B_NewInterpolation(VC1Context* pContext,
                                                                                  Ipp8u *pDst,
                                                                                  Ipp32s dstStep,
                                                                                  Ipp32s back)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;

    Ipp8u* pRefData = NULL;
    Ipp32s refPitch;
    Ipp32s f = 0;

    Ipp16s xMV = pMB->m_pBlocks[0].mv[back][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[back][1];

    pContext->interp_params_luma.pLUTTop = 0;
    pContext->interp_params_luma.pLUTBottom = 0;

    if ((pPicHeader->CurrField == 1) &&
        (((pPicHeader->NUMREF + pPicHeader->REFFIELD) ==0) ||
         (pPicHeader->NUMREF == 1) && (!pMB->m_pBlocks[0].mv_s_polarity[back]) && (!back)))
    {
        index = pContext->m_frmBuff.m_iCurrIndex;
    }

    if (index != pContext->m_frmBuff.m_iCurrIndex)
    {
        if (back == 0)
            SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        else
            SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
    }

    pRefData = pContext->m_frmBuff.m_pFrames[index].m_pY;
    refPitch = pContext->m_frmBuff.m_pFrames[index].m_iYPitch;

    f = CalcMVInterpolateFieldLuma(pContext, pMB->m_pBlocks[0].mv_s_polarity[back], &xMV, &yMV);

    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_luma.fieldPrediction = 1;
    pContext->interp_params_luma.isPredBottom = f;

    pContext->interp_params_luma.srcStep = pContext->interp_params_luma.srcStep;
    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;

    if (index == pContext->m_frmBuff.m_iCurrIndex)
        pContext->interp_params_luma.oppositePadding = 0;

    if (f)
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;


    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;
    pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_LUMA;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_LUMA;

    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    if (VC1_MVMODE_HPELBI_1MV == pContext->m_picLayerHeader->MVMODE)
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_luma);
    else
        ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    return VC1_OK;
}


static VC1Status InterpolateBlock_InterlaceFieldPictureLuma4MV_NewInterpolation(VC1Context* pContext,
                                                                                Ipp8u *pDst,
                                                                                Ipp32s dstStep,
                                                                                Ipp32s blk_num,
                                                                                Ipp32s back)
{
    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    VC1MB *pMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;

    Ipp8u* pRefData = NULL;
    Ipp32s refPitch;
    Ipp32s f = 0;

    Ipp16s xMV = pMB->m_pBlocks[blk_num].mv[back][0];
    Ipp16s yMV = pMB->m_pBlocks[blk_num].mv[back][1];

    pContext->interp_params_luma.pLUTTop = 0;
    pContext->interp_params_luma.pLUTBottom = 0;

    if ((pPicHeader->CurrField == 1) &&
        (((pPicHeader->NUMREF + pPicHeader->REFFIELD) ==0) ||
         (pPicHeader->NUMREF == 1) && (!pMB->m_pBlocks[blk_num].mv_s_polarity[back]) && (!back)))
    {
        index = pContext->m_frmBuff.m_iCurrIndex;
    }

    if (index != pContext->m_frmBuff.m_iCurrIndex)
    {
        if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
        {
            if (back == 0)
                SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
            else
                SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        }
        else
        {
            SetLumaTblPrev(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);
        }
    }
    else if (VC1_P_FRAME == pContext->m_picLayerHeader->PTYPE) // we don't do it for B/B field
    {
        SetLumaTblCurr(pContext, &pContext->interp_params_luma.pLUTTop, &pContext->interp_params_luma.pLUTBottom, index);
    }

    pRefData = pContext->m_frmBuff.m_pFrames[index].m_pY;
    refPitch = pContext->m_frmBuff.m_pFrames[index].m_iYPitch;

    f = CalcMVInterpolateFieldLuma(pContext, pMB->m_pBlocks[blk_num].mv_s_polarity[back], &xMV, &yMV);


    xMV = xMV + offset_table_x[blk_num];
    yMV = yMV + offset_table_y[blk_num];

    pContext->interp_params_luma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_luma.fieldPrediction = 2;
    pContext->interp_params_luma.isPredBottom = f;
    pContext->interp_params_luma.pSrc = pContext->m_frmBuff.m_pFrames[index].m_pY;

    if (f)
        pContext->interp_params_luma.pSrc += pContext->interp_params_luma.srcStep;


    if (index == pContext->m_frmBuff.m_iCurrIndex)
        pContext->interp_params_luma.oppositePadding = 0;

    pContext->interp_params_luma.srcStep = pContext->interp_params_luma.srcStep;
    //!!!!. Not begin of image, already current mb
    pContext->interp_params_luma.pDst = pDst;
    pContext->interp_params_luma.dstStep = dstStep;
    pContext->interp_params_luma.sizeFrame.width = 2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_luma.sizeFrame.height = 2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);
    pContext->interp_params_luma.sizeBlock.width = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_luma.sizeBlock.height = VC1_PIXEL_IN_BLOCK;
    pContext->interp_params_luma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_luma.pointRefBlockPos.y = yMV >> 2;
    pContext->interp_params_luma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_luma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBicubicBlock_VC1_8u_C1R(&pContext->interp_params_luma);

    return VC1_OK;
}

static VC1Status InterpolateBlock_InterlaceFieldPictureChroma1MV_NewInterpolation(VC1Context* pContext,
                                                                                  Ipp8u* pDst1,
                                                                                  Ipp32s dstStep1,
                                                                                  Ipp8u* pDst2,
                                                                                  Ipp32s dstStep2,
                                                                                  Ipp32s back)
{
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;

    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;

    Ipp8u* pRefData = NULL;

    Ipp16s xMV = pMB->m_pBlocks[0].mv[back][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[back][1];

    Ipp32s f = 0;
    Ipp32s refPitch;

    if ((pPicHeader->CurrField == 1) &&
        (((pPicHeader->NUMREF + pPicHeader->REFFIELD) ==0) ||
         (pPicHeader->NUMREF == 1) && (!pMB->m_pBlocks[0].mv_s_polarity[back]) && (!back)))
    {
        index = pContext->m_frmBuff.m_iCurrIndex;
    }

    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;

    if (index != pContext->m_frmBuff.m_iCurrIndex)
    {
        if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
        {
            if (back == 0)
                SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
            else
                SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        }
        else
            SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    }
    else if (VC1_P_FRAME == pContext->m_picLayerHeader->PTYPE) // we don't do it for B/B field
    {
        SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, index);
    }


    //1MV
    xMV = pMB->m_pBlocks[0].mv[back][0];
    yMV = pMB->m_pBlocks[0].mv[back][1];

    pMB->m_pBlocks[4].mv_s_polarity[back] = pMB->m_pBlocks[0].mv_s_polarity[back];
    pMB->m_pBlocks[5].mv_s_polarity[back] = pMB->m_pBlocks[0].mv_s_polarity[back];


    refPitch  = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    pRefData  = pContext->m_frmBuff.m_pFrames[index].m_pU;

    f = CalcMVInterpolateFieldChroma(pContext, pMB->m_pBlocks[4].mv_s_polarity[back], &xMV, &yMV);

    if (f)
        pRefData += refPitch;
    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 1;
    pContext->interp_params_chroma.pSrc = pRefData; 

    if (index == pContext->m_frmBuff.m_iCurrIndex)
        pContext->interp_params_chroma.oppositePadding = 0;

    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    pContext->interp_params_chroma.pDst = pDst1;
    pContext->interp_params_chroma.dstStep = dstStep1;

    pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_CHROMA;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_CHROMA;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);


    //BLOCK 5
    refPitch  = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;
    pRefData  = pContext->m_frmBuff.m_pFrames[index].m_pV;

    if (f)
        pRefData += refPitch;
  
    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2;
    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    return VC1_OK;
}
static VC1Status InterpolateBlock_InterlaceFieldPictureChroma4MV_NewInterpolation(VC1Context* pContext,
                                                                                  Ipp8u* pDst1,
                                                                                  Ipp32s dstStep1,
                                                                                  Ipp8u* pDst2,
                                                                                  Ipp32s dstStep2,
                                                                                  Ipp32s back)
{
    Ipp32s index = (back)? pContext->m_frmBuff.m_iNextIndex:pContext->m_frmBuff.m_iPrevIndex;
    VC1MB *pMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* pPicHeader = pContext->m_picLayerHeader;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    Ipp8u* pRefData = NULL;

    Ipp16s xMV = pMB->m_pBlocks[0].mv[back][0];
    Ipp16s yMV = pMB->m_pBlocks[0].mv[back][1];

    Ipp32u MVBcount = 0;
    Ipp32u MVTcount = 0;

    Ipp32s f = 0;
    Ipp32s refPitch;

    if (pPicHeader->NUMREF)
    {
        //4MV NUMREF == 1
        Ipp32s count=0;
        sMB->MVcount =0;
        for (count = 0; count < 4; count++)
        {
            if((pMB->m_pBlocks[count].mv_s_polarity[back] + pPicHeader->BottomField) & 0x1)
            {
                sMB->xLuMVT[MVTcount] = pMB->m_pBlocks[count].mv[back][0];
                sMB->yLuMVT[MVTcount] = pMB->m_pBlocks[count].mv[back][1];
                ++MVTcount;
            }
            else
            {
                sMB->xLuMVB[MVBcount] = pMB->m_pBlocks[count].mv[back][0];
                sMB->yLuMVB[MVBcount] = pMB->m_pBlocks[count].mv[back][1];
                ++MVBcount;
            }
        }
        if (MVBcount == MVTcount)
        {
            sMB->MVcount = 2;
            pMB->m_pBlocks[4].mv_s_polarity[back] = 1;
            pMB->m_pBlocks[5].mv_s_polarity[back] = 1;

            if (pPicHeader->BottomField)
            {
                    sMB->x_LuMV = sMB->xLuMVB;
                    sMB->y_LuMV = sMB->yLuMVB;
            }
            else
            {
                    sMB->x_LuMV = sMB->xLuMVT;
                    sMB->y_LuMV = sMB->yLuMVT;
            }
        }
        else if (MVBcount > MVTcount)
        {
            pMB->m_pBlocks[4].mv_s_polarity[back] = pPicHeader->BottomField;
            pMB->m_pBlocks[5].mv_s_polarity[back] = pPicHeader->BottomField;

            sMB->MVcount = MVBcount;
            sMB->x_LuMV  = sMB->xLuMVB;
            sMB->y_LuMV  = sMB->yLuMVB;
        }
        else
        {
            pMB->m_pBlocks[4].mv_s_polarity[back] = !pPicHeader->BottomField;
            pMB->m_pBlocks[5].mv_s_polarity[back] = !pPicHeader->BottomField;

            sMB->MVcount = MVTcount;
            sMB->x_LuMV  = sMB->xLuMVT;
            sMB->y_LuMV  = sMB->yLuMVT;
        }

        Derive4MV_Field(sMB->MVcount,&xMV,&yMV, sMB->x_LuMV, sMB->y_LuMV);
    }
    else
    {
        //4MV NUMREF == 0
        Ipp32s count;
        for (count=0;count<4;count++)
        {
            sMB->xLuMV[count] = pMB->m_pBlocks[count].mv[back][0];
            sMB->yLuMV[count] = pMB->m_pBlocks[count].mv[back][1];
        }
         //NEED 4mv field one reference picture
        xMV = median4(sMB->xLuMV);
        yMV = median4(sMB->yLuMV);

        pMB->m_pBlocks[4].mv_s_polarity[back] = pMB->m_pBlocks[0].mv_s_polarity[back];
        pMB->m_pBlocks[5].mv_s_polarity[back] = pMB->m_pBlocks[0].mv_s_polarity[back];
    }

    if ((pPicHeader->CurrField == 1) &&
        (((pPicHeader->NUMREF + pPicHeader->REFFIELD) ==0) ||
         (pPicHeader->NUMREF == 1) && (!pMB->m_pBlocks[4].mv_s_polarity[back]) && (!back)))
    {
        index = pContext->m_frmBuff.m_iCurrIndex;
    }
    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;

    if (index != pContext->m_frmBuff.m_iCurrIndex)
    {
        if (VC1_B_FRAME == pContext->m_picLayerHeader->PTYPE)
        {
            if (back == 0)
                SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
            else
                SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iNextIndex);
        }
        else
            SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    }
    else if (VC1_P_FRAME == pContext->m_picLayerHeader->PTYPE) // we don't do it for B/B field
    {
        SetChromaTblCurr(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, index);
    }
    refPitch  = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    pRefData  = pContext->m_frmBuff.m_pFrames[index].m_pU;

    f = CalcMVInterpolateFieldChroma(pContext, pMB->m_pBlocks[4].mv_s_polarity[back], &xMV, &yMV);
    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 2;

    if (index == pContext->m_frmBuff.m_iCurrIndex)
        pContext->interp_params_chroma.oppositePadding = 0;

    pContext->interp_params_chroma.srcStep = pContext->interp_params_chroma.srcStep;
    pContext->interp_params_chroma.pSrc = pRefData;

    if (f)
        pContext->interp_params_chroma.pSrc += refPitch;

    pContext->interp_params_chroma.pDst = pDst1;
    pContext->interp_params_chroma.dstStep = dstStep1;

    pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_CHROMA;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_CHROMA;

    pContext->interp_params_chroma.pointRefBlockPos.x = xMV >> 2;
    pContext->interp_params_chroma.pointRefBlockPos.y = yMV >> 2;

    pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV&3;
    pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV&3;

    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);


    //BLOCK 5
    refPitch  = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;
    pRefData  = pContext->m_frmBuff.m_pFrames[index].m_pV;

    if (f)
        pRefData += refPitch;
    pContext->interp_params_chroma.pSrc = pRefData;
    pContext->interp_params_chroma.pDst = pDst2;
    pContext->interp_params_chroma.dstStep = dstStep2;


    ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);

    return VC1_OK;
}




static VC1Status InterpolateBlock_InterlaceChroma4MV_NewInterpolation(VC1Context* pContext,
                                                                      Ipp8u* pDst1,
                                                                      Ipp32u dstStep1,
                                                                      Ipp8u* pDst2,
                                                                      Ipp32u dstStep2,
                                                                      Ipp32s field)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s index = pContext->m_frmBuff.m_iPrevIndex;

    Ipp8u* pRefUData = NULL;
    Ipp32s refUPitch;

    Ipp8u* pRefVData = NULL;
    Ipp32s refVPitch;

    Ipp16s xMV[4] = {0};
    Ipp16s yMV[4] = {0};

    Ipp32s refCurUPitch = 0;
    Ipp32s refCurVPitch = 0;
    Ipp8u* pRefCurUData = NULL;
    Ipp8u* pRefCurVData = NULL;

    Ipp32s f = 0;
    Ipp32s sblk_num = 0;
    Ipp32u offset_tableU[4] ={0};
    Ipp32u offset_tableV[4] ={0};

    pContext->interp_params_chroma.pLUTTop = 0;
    pContext->interp_params_chroma.pLUTBottom = 0;

    SetChromaTblPrev(pContext, &pContext->interp_params_chroma.pLUTTop, &pContext->interp_params_chroma.pLUTBottom, pContext->m_frmBuff.m_iCurrIndex);

    pContext->interp_params_chroma.isPredBottom = f;
    pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  == VC1_Progressive)?1:0;
    pContext->interp_params_chroma.fieldPrediction = 0;




    //BLOCK 4
    refUPitch  = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    pRefUData  = pContext->m_frmBuff.m_pFrames[index].m_pU;

    //BLOCK 5
    refVPitch  = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;
    pRefVData  = pContext->m_frmBuff.m_pFrames[index].m_pV;


    if (field)
    {
        xMV[0] = pMB->m_pBlocks[0].mv[0][0];
        xMV[1] = pMB->m_pBlocks[1].mv[0][0];
        xMV[2] = pMB->m_pBlocks[0].mv_bottom[0][0];
        xMV[3] = pMB->m_pBlocks[1].mv_bottom[0][0];
        yMV[0] = pMB->m_pBlocks[0].mv[0][1];
        yMV[1] = pMB->m_pBlocks[1].mv[0][1];
        yMV[2] = pMB->m_pBlocks[0].mv_bottom[0][1];
        yMV[3] = pMB->m_pBlocks[1].mv_bottom[0][1];

        //BLOCK 4
        refCurUPitch = refUPitch;
        offset_tableU[0] = 0;
        offset_tableU[1] = 4;
        offset_tableU[2] = dstStep1;
        offset_tableU[3] = dstStep1 + 4;
        dstStep1 <<=1;

        //BLOCK 5
        refCurVPitch = refVPitch;
        offset_tableV[0] = 0;
        offset_tableV[1] = 4;
        offset_tableV[2] = dstStep2;
        offset_tableV[3] = dstStep2 + 4;
        dstStep2 <<=1;

        pContext->interp_params_chroma.fieldPrediction = 1; 
    }
    else
    {

        xMV[0] = pMB->m_pBlocks[0].mv[0][0];
        xMV[1] = pMB->m_pBlocks[1].mv[0][0];
        xMV[2] = pMB->m_pBlocks[2].mv[0][0];
        xMV[3] = pMB->m_pBlocks[3].mv[0][0];

        yMV[0] = pMB->m_pBlocks[0].mv[0][1];
        yMV[1] = pMB->m_pBlocks[1].mv[0][1];
        yMV[2] = pMB->m_pBlocks[2].mv[0][1];
        yMV[3] = pMB->m_pBlocks[3].mv[0][1];

        //BLOCK 4
        refCurUPitch = refUPitch;
        offset_tableU[0] = 0;
        offset_tableU[1] = 4;
        offset_tableU[2] = dstStep1*4;
        offset_tableU[3] = dstStep1*4 + 4;

        //BLOCK 5
        refCurVPitch = refVPitch;
        offset_tableV[0] = 0;
        offset_tableV[1] = 4;
        offset_tableV[2] = dstStep2*4;
        offset_tableV[3] = dstStep2*4 + 4;
        //isFieldPadding = (pContext->m_frmBuff.m_pFrames[index].FCM  != VC1_Progressive)?1:0;
        pContext->interp_params_chroma.oppositePadding = (pContext->m_frmBuff.m_pFrames[index].FCM  != VC1_Progressive)?1:0;
    }

    pContext->interp_params_chroma.sizeFrame.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    pContext->interp_params_chroma.sizeFrame.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);

    pContext->interp_params_chroma.sizeBlock.width = VC1_PIXEL_IN_BLOCK/2;
    pContext->interp_params_chroma.sizeBlock.height = VC1_PIXEL_IN_BLOCK/2;


    for(sblk_num = 0; sblk_num < 4; sblk_num++)
    {
        pRefCurUData = pRefUData;
        pRefCurVData = pRefVData;

        f = CalcMVInterpolateInterlacedChroma4MV(pContext, &xMV[sblk_num], &yMV[sblk_num], field, sblk_num);

        if (f && field)
        {
            pRefCurUData = pRefCurUData + refUPitch;
            pRefCurVData = pRefCurVData + refVPitch;
        }
        pContext->interp_params_chroma.pSrc = pRefCurUData;
        pContext->interp_params_chroma.srcStep = refCurUPitch;

        pContext->interp_params_chroma.pDst = pDst1 + offset_tableU[sblk_num];
        pContext->interp_params_chroma.dstStep = dstStep1;

        pContext->interp_params_chroma.pointRefBlockPos.x = xMV[sblk_num] >> 2;
        pContext->interp_params_chroma.pointRefBlockPos.y = yMV[sblk_num] >> 2;

        pContext->interp_params_chroma.pointVectorQuarterPix.x = xMV[sblk_num]&3;
        pContext->interp_params_chroma.pointVectorQuarterPix.y = yMV[sblk_num]&3;
        pContext->interp_params_chroma.isPredBottom = f && field;

        //BLOCK 4
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
        //BLOCK 5
        pContext->interp_params_chroma.pSrc = pRefCurVData;
        pContext->interp_params_chroma.srcStep = refCurVPitch;
        pContext->interp_params_chroma.pDst = pDst2 + offset_tableV[sblk_num];
        pContext->interp_params_chroma.dstStep = dstStep2;
        
        ippiInterpolateICBilinearBlock_VC1_8u_C1R(&pContext->interp_params_chroma);
    }
    return VC1_OK;
}

#define InterpolateBlock_ProgressivePictureLuma1MV_P InterpolateBlock_ProgressivePictureLuma1MV_P_NewInterpolation
#define InterpolateBlock_ProgressivePictureLuma1MV_B InterpolateBlock_ProgressivePictureLuma1MV_B_NewInterpolation
#define InterpolateBlock_ProgressivePictureLuma4MV InterpolateBlock_ProgressivePictureLuma4MV_NewInterpolation
#define InterpolateBlock_ProgressivePictureChroma1MV InterpolateBlock_ProgressivePictureChroma1MV_NewInterpolation
#define InterpolateBlock_ProgressivePictureChroma4MV InterpolateBlock_ProgressivePictureChroma4MV_NewInterpolation
#define InterpolateBlock_InterlacePictureLuma2MV InterpolateBlock_InterlacePictureLuma2MV_NewInterpolation
#define InterpolateBlock_InterlacePictureLuma InterpolateBlock_InterlacePictureLuma_NewInterpolation
#define InterpolateBlock_InterlacePictureChroma2MV_P InterpolateBlock_InterlacePictureChroma2MV_P_NewInterpolation
#define InterpolateBlock_InterlacePictureChroma2MV_B InterpolateBlock_InterlacePictureChroma2MV_B_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureLuma1MV_P InterpolateBlock_InterlaceFieldPictureLuma1MV_P_NewInterpolation
#define InterpolateBlock_InterlacePictureChroma2MV_P InterpolateBlock_InterlacePictureChroma2MV_P_NewInterpolation
#define InterpolateBlock_InterlacePictureChroma2MV_B InterpolateBlock_InterlacePictureChroma2MV_B_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureLuma1MV_P InterpolateBlock_InterlaceFieldPictureLuma1MV_P_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureLuma1MV_B InterpolateBlock_InterlaceFieldPictureLuma1MV_B_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureLuma4MV InterpolateBlock_InterlaceFieldPictureLuma4MV_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureChroma1MV InterpolateBlock_InterlaceFieldPictureChroma1MV_NewInterpolation
#define InterpolateBlock_InterlaceFieldPictureChroma4MV InterpolateBlock_InterlaceFieldPictureChroma4MV_NewInterpolation
#define InterpolateBlock_InterlaceChroma4MV InterpolateBlock_InterlaceChroma4MV_NewInterpolation


VC1Status PredictBlock_P(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    Ipp32u count = 0;
    Ipp8u IntraFlag = pCurrMB->IntraFlag;
    Ipp32s plane_offset;

    pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
    pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

    pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
    pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

    pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
    pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

    if ( (pCurrMB->mbType&0x03) == VC1_MB_1MV_INTER)
    {
        //1 MV
        //luma
        InterpolateBlock_ProgressivePictureLuma1MV_P(pContext, pCurrMB->currYPlane,
                                                               pCurrMB->currYPitch);
        //chroma
        InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                               pCurrMB->currUPitch,
                                                               pCurrMB->currVPlane,
                                                               pCurrMB->currVPitch, 0);
    }
    else
    {
        //4 MV
        for (count = 0; count < VC1_NUM_OF_LUMA; count++)
        {
            if(IntraFlag & 1)
            {
                IntraFlag >>= 1;
                continue;
            }

            plane_offset = (count&1)*8 + (count&2)*4*pCurrMB->currYPitch;
            InterpolateBlock_ProgressivePictureLuma4MV(pContext, pCurrMB->currYPlane + plane_offset,
                                                                 pCurrMB->currYPitch, count);

            IntraFlag >>= 1;
        }

        if(!(IntraFlag & 1))
        {
            InterpolateBlock_ProgressivePictureChroma4MV(pContext, pCurrMB->currUPlane,
                                                                   pCurrMB->currUPitch,
                                                                   pCurrMB->currVPlane,
                                                                   pCurrMB->currVPitch, 0);
        }
    }
    return VC1_OK;
}

VC1Status PredictBlock_B(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    Ipp32u _predict_type = pCurrMB->mbType;
    Ipp32u predict_type = _predict_type >> 4;


    //1 MV
    if ( (VC1_GET_PREDICT(_predict_type) != VC1_MB_INTERP) && (VC1_GET_PREDICT(_predict_type) != VC1_MB_DIRECT) )
    {
        pCurrMB->pInterpolLumaSrc[predict_type] = pCurrMB->currYPlane;
        pCurrMB->InterpolsrcLumaStep[predict_type] = pCurrMB->currYPitch;

        pCurrMB->pInterpolChromaUSrc[predict_type] = pCurrMB->currUPlane;
        pCurrMB->InterpolsrcChromaUStep[predict_type] = pCurrMB->currUPitch;

        pCurrMB->pInterpolChromaVSrc[predict_type] = pCurrMB->currVPlane;
        pCurrMB->InterpolsrcChromaVStep[predict_type] = pCurrMB->currVPitch;

        //luma
        InterpolateBlock_ProgressivePictureLuma1MV_B(pContext, pCurrMB->currYPlane,
                                                   pCurrMB->currYPitch, predict_type);

        //chroma
        InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                               pCurrMB->currUPitch,
                                                               pCurrMB->currVPlane,
                                                               pCurrMB->currVPitch,
                                                               predict_type);

        pCurrMB->pInterpolLumaSrc[1 - predict_type] = NULL;
        pCurrMB->InterpolsrcLumaStep[1 - predict_type] = 0;
        pCurrMB->pInterpolChromaUSrc[1 - predict_type] =NULL;
        pCurrMB->InterpolsrcChromaUStep[1 - predict_type] = 0;
        pCurrMB->pInterpolChromaVSrc[1 - predict_type] = NULL;
        pCurrMB->InterpolsrcChromaVStep[1 - predict_type] = 0;
    }
    else
    {
        Ipp8u pPredBlock[384];


        pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
        pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

        pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
        pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

        pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
        pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

        pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
        pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;

        pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
        pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

        pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
        pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

        //luma
        //forward
        InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,
                                                     pCurrMB->currYPlane,
                                                     pCurrMB->currYPitch,
                                                     0);

        if(pCurrMB->pInterpolLumaSrc[0] == pCurrMB->currYPlane)
        {
            //backward
            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,
                                                         pPredBlock,
                                                         VC1_PIXEL_IN_LUMA,
                                                         1);

            // we should average and save data to plane
            //if (pCurrMB->pInterpolLumaSrc[0] == pPredBlock)
            if (pCurrMB->pInterpolLumaSrc[1] == pPredBlock)
            {
                pCurrMB->pInterpolLumaSrc[1] = NULL;
                pCurrMB->InterpolsrcLumaStep[1] = 0;
                ippiAverage16x16_8u_C1IR(pPredBlock,
                                         VC1_PIXEL_IN_LUMA,
                                         pCurrMB->currYPlane,
                                         pCurrMB->currYPitch);
            }
        }
        else
        {
            //backward
            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,
                                                         pCurrMB->currYPlane,
                                                         pCurrMB->currYPitch,
                                                         1);

            //if(pCurrMB->pInterpolLumaSrc[1] == pCurrMB->currYPlane)
            if(pCurrMB->pInterpolLumaSrc[1] == pPredBlock)
            {
                pCurrMB->pInterpolLumaSrc[1] = pCurrMB->currYPlane;
                pCurrMB->InterpolsrcLumaStep[1] = pCurrMB->currYPitch;
            }
        }

         //chroma U, V
         InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                                                     pCurrMB->currUPlane,
                                                     pCurrMB->currUPitch,
                                                     pCurrMB->currVPlane,
                                                     pCurrMB->currVPitch, 0);

         // check chroma
         if(pCurrMB->pInterpolChromaUSrc[0] == pCurrMB->currUPlane)
         {
             InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                                                          pPredBlock + 64*4,
                                                          VC1_PIXEL_IN_CHROMA,
                                                          pPredBlock + 64*5,
                                                          VC1_PIXEL_IN_CHROMA, 1);
             // we should average and save data to plane
             if (pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
             {
                 pCurrMB->pInterpolChromaUSrc[1] = NULL;
                 pCurrMB->pInterpolChromaVSrc[1] = NULL;

                 ippiAverage8x8_8u_C1IR(pPredBlock + 64*4,
                                         VC1_PIXEL_IN_CHROMA,
                                         pCurrMB->currUPlane,
                                         pCurrMB->currUPitch);

                 ippiAverage8x8_8u_C1IR(pPredBlock + 64*5,
                                         VC1_PIXEL_IN_CHROMA,
                                         pCurrMB->currVPlane,
                                         pCurrMB->currVPitch);
             }
         }
         else
         {
             //backward
             InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                                                          pCurrMB->currUPlane,
                                                          pCurrMB->currUPitch,
                                                          pCurrMB->currVPlane,
                                                          pCurrMB->currVPitch, 1);

             if(pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
             {
                    pCurrMB->pInterpolChromaUSrc[1] = pCurrMB->currUPlane;
                    pCurrMB->pInterpolChromaVSrc[1] = pCurrMB->currVPlane;
                    pCurrMB->InterpolsrcChromaUStep[1] = pCurrMB->currUPitch;
                    pCurrMB->InterpolsrcChromaVStep[1] = pCurrMB->currVPitch;
             }
         }
    }

    return VC1_OK;
}

VC1Status PredictBlock_InterlacePPicture(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    Ipp32s plane_offset;
    Ipp32u count= 0;
    pCurrMB->pInterpolLumaSrc[0] = NULL;
    pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
    pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;
    pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
    pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;


    //prediction phase: Derive MV and interpolation blocks
    switch(VC1_GET_MBTYPE(pCurrMB->mbType))
    {
    case  VC1_MB_4MV_INTER:
        for (count = 0; count < VC1_NUM_OF_LUMA; count++)
        {
           plane_offset = (count&1)*8 + (count&2)*4*pCurrMB->currYPitch;
           InterpolateBlock_ProgressivePictureLuma4MV(pContext, pCurrMB->currYPlane + plane_offset,
                                                             pCurrMB->currYPitch,count);
        }

        InterpolateBlock_InterlaceChroma4MV(pContext, pCurrMB->currUPlane,
                                                      pCurrMB->currUPitch,
                                                      pCurrMB->currVPlane,
                                                      pCurrMB->currVPitch, 0);
         break;

    case VC1_MB_4MV_FIELD_INTER:
        InterpolateBlock_InterlacePictureLuma(pContext, pCurrMB->currYPlane,
                                                        pCurrMB->currYPitch, 0);

        InterpolateBlock_InterlacePictureLuma(pContext, pCurrMB->currYPlane + 8,
                                                        pCurrMB->currYPitch, 1);

        InterpolateBlock_InterlaceChroma4MV(pContext, pCurrMB->currUPlane,
                                                      pCurrMB->currUPitch,
                                                      pCurrMB->currVPlane,
                                                      pCurrMB->currVPitch, 1);
         break;

    case VC1_MB_2MV_INTER:
        InterpolateBlock_InterlacePictureLuma2MV(pContext, pCurrMB->currYPlane,
                                                           pCurrMB->currYPitch, 0, 0);

        InterpolateBlock_InterlacePictureChroma2MV_P(pContext, pCurrMB->currUPlane,
                                                             pCurrMB->currUPitch,
                                                             pCurrMB->currVPlane,
                                                             pCurrMB->currVPitch);
        break;

    case VC1_MB_1MV_INTER:
        //luma
        InterpolateBlock_ProgressivePictureLuma1MV_P(pContext, pCurrMB->currYPlane,
                                                             pCurrMB->currYPitch);
        //chroma
        InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                            pCurrMB->currUPitch,
                                                            pCurrMB->currVPlane,
                                                            pCurrMB->currVPitch, 0);
        break;
    default:
        VM_ASSERT(0);
        break;
    }

    return VC1_OK;
}

VC1Status PredictBlock_InterlaceFieldPPicture(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    Ipp32u count = 0;
    Ipp32s plane_offset;
    pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
    pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;
    pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
    pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

    pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
    pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

    if ( (pCurrMB->mbType&0x03) == VC1_MB_1MV_INTER)
    {
        //1 MV
        InterpolateBlock_InterlaceFieldPictureLuma1MV_P(pContext, pCurrMB->currYPlane,
                                                             pCurrMB->currYPitch);

        InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                                  pCurrMB->currUPitch,
                                                                  pCurrMB->currVPlane,
                                                                  pCurrMB->currVPitch, 0);
    }
    else
    {
        //4 MV
        for (count = 0; count < VC1_NUM_OF_LUMA; count++)
        {
            plane_offset = (count&1)*8 + (count&2)*4*pCurrMB->currYPitch;
            InterpolateBlock_InterlaceFieldPictureLuma4MV(pContext, pCurrMB->currYPlane + plane_offset,
                                                       pCurrMB->currYPitch, count, 0);
        }

        InterpolateBlock_InterlaceFieldPictureChroma4MV(pContext, pCurrMB->currUPlane,
                                                                  pCurrMB->currUPitch,
                                                                  pCurrMB->currVPlane,
                                                                  pCurrMB->currVPitch, 0);
    }

    return VC1_OK;
}


VC1Status PredictBlock_InterlaceBPicture(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    //Ipp8u* pPredBlock = pContext->m_pPredBlock;
    //assert(pPredBlock != NULL);

    //prediction phase: Derive MV and interpolation blocks
    switch(pCurrMB->mbType)
    {
    case  VC1_MB_FORWARD|VC1_MB_1MV_INTER:
        {
            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;


            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext, pCurrMB->currYPlane,
                                                         pCurrMB->currYPitch, 0);

            InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                                   pCurrMB->currUPitch,
                                                                   pCurrMB->currVPlane,
                                                                   pCurrMB->currVPitch, 0);

            pCurrMB->pInterpolLumaSrc[1] = NULL;
            pCurrMB->InterpolsrcLumaStep[1] = 0;
            pCurrMB->pInterpolChromaUSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1] = 0;
            pCurrMB->pInterpolChromaVSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1] = 0;

        }
        break;

    case  VC1_MB_BACKWARD|VC1_MB_1MV_INTER:
        {
            pCurrMB->pInterpolLumaSrc[1] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[1] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[1] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[1] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[1] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[1] = pCurrMB->currVPitch;

            //luma
            //backward
            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext, pCurrMB->currYPlane,
                                                         pCurrMB->currYPitch, 1);

            InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                                pCurrMB->currUPitch,
                                                                pCurrMB->currVPlane,
                                                                pCurrMB->currVPitch, 1);
            pCurrMB->pInterpolLumaSrc[0] = NULL;
            pCurrMB->InterpolsrcLumaStep[0] = 0;
            pCurrMB->pInterpolChromaUSrc[0] = NULL;
            pCurrMB->InterpolsrcChromaUStep[0] = 0;
            pCurrMB->pInterpolChromaVSrc[0] = NULL;
            pCurrMB->InterpolsrcChromaVStep[0] = 0;

        }
        break;
    case  VC1_MB_DIRECT|VC1_MB_1MV_INTER:
    case  VC1_MB_INTERP|VC1_MB_1MV_INTER:
        {
            //pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            //pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

            //pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
            //pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;

            //pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            //pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            //pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            //pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            //pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
            //pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

            //pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
            //pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

            ////luma
            ////forward
            //InterpolateBlock_ProgressivePictureLuma1MV_B(pContext, pCurrMB->currYPlane,
            //                                                   pCurrMB->currYPitch, 0);

            //InterpolateBlock_ProgressivePictureChroma1MV(pContext, pCurrMB->currUPlane,
            //                                                    pCurrMB->currUPitch,
            //                                                    pCurrMB->currVPlane,
            //                                                    pCurrMB->currVPitch, 0);
            ////backward
            //InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,   pPredBlock, VC1_PIXEL_IN_LUMA, 1);

            //InterpolateBlock_ProgressivePictureChroma1MV(pContext, pPredBlock + 64*4,
            //                                                       VC1_PIXEL_IN_CHROMA,
            //                                                       pPredBlock + 64*5,
            //                                                       VC1_PIXEL_IN_CHROMA, 1);

            Ipp8u pPredBlock[384];

            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
            pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;

            pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
            pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

            pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
            pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

            //luma
            //forward
            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,
                                                         pCurrMB->currYPlane,
                                                         pCurrMB->currYPitch,
                                                         0);
            //backward
            InterpolateBlock_ProgressivePictureLuma1MV_B(pContext,
                                                         pPredBlock,
                                                         VC1_PIXEL_IN_LUMA,
                                                         1);

            ippiAverage16x16_8u_C1R(pCurrMB->pInterpolLumaSrc[0],
                                    pCurrMB->InterpolsrcLumaStep[0],
                                    pCurrMB->pInterpolLumaSrc[1],
                                    pCurrMB->InterpolsrcLumaStep[1],
                                    pCurrMB->currYPlane,
                                    pCurrMB->currYPitch);

             pCurrMB->pInterpolLumaSrc[1] = pCurrMB->currYPlane;





            //chroma U, V
            InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                pCurrMB->currUPlane,
                pCurrMB->currUPitch,
                pCurrMB->currVPlane,
                pCurrMB->currVPitch, 0);

            // check chroma
            if(pCurrMB->pInterpolChromaUSrc[0] == pCurrMB->currUPlane)
            {
                InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                    pPredBlock + 64*4,
                    VC1_PIXEL_IN_CHROMA,
                    pPredBlock + 64*5,
                    VC1_PIXEL_IN_CHROMA, 1);
                // we should average and save data to plane
                if (pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
                {
                    pCurrMB->pInterpolChromaUSrc[1] = NULL;
                    pCurrMB->pInterpolChromaVSrc[1] = NULL;

                    ippiAverage8x8_8u_C1IR(pPredBlock + 64*4,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currUPlane,
                        pCurrMB->currUPitch);

                    ippiAverage8x8_8u_C1IR(pPredBlock + 64*5,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currVPlane,
                        pCurrMB->currVPitch);
                }
            }
            else
            {
                //backward
                InterpolateBlock_ProgressivePictureChroma1MV(pContext,
                    pCurrMB->currUPlane,
                    pCurrMB->currUPitch,
                    pCurrMB->currVPlane,
                    pCurrMB->currVPitch, 1);

                if(pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
                {
                    pCurrMB->pInterpolChromaUSrc[1] = pCurrMB->currUPlane;
                    pCurrMB->pInterpolChromaVSrc[1] = pCurrMB->currVPlane;
                    pCurrMB->InterpolsrcChromaUStep[1] = pCurrMB->currUPitch;
                    pCurrMB->InterpolsrcChromaVStep[1] = pCurrMB->currVPitch;
                }
            }
        }
        break;
    case  VC1_MB_FORWARD|VC1_MB_2MV_INTER:
        {
            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            //luma
            InterpolateBlock_InterlacePictureLuma2MV(pContext, pCurrMB->currYPlane,
                                                               pCurrMB->currYPitch,
                                                               0, pCurrMB->MVSW);
            //Chroma
            InterpolateBlock_InterlacePictureChroma2MV_B(pContext, pCurrMB->currUPlane,
                                                                    pCurrMB->currUPitch,
                                                                    pCurrMB->currVPlane,
                                                                    pCurrMB->currVPitch, 0,
                                                                    pCurrMB->MVSW);
            pCurrMB->pInterpolLumaSrc[1] = NULL;
            pCurrMB->InterpolsrcLumaStep[1] = 0;
            pCurrMB->pInterpolChromaUSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1] = 0;
            pCurrMB->pInterpolChromaVSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1] = 0;
        }
        break;
    case  VC1_MB_BACKWARD|VC1_MB_2MV_INTER:
        {
            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            InterpolateBlock_InterlacePictureLuma2MV(pContext, pCurrMB->currYPlane,
                                                               pCurrMB->currYPitch,
                                                               1, 1 - pCurrMB->MVSW);

            //Chroma
            InterpolateBlock_InterlacePictureChroma2MV_B(pContext, pCurrMB->currUPlane,
                                                                   pCurrMB->currUPitch,
                                                                   pCurrMB->currVPlane,
                                                                   pCurrMB->currVPitch, 1,
                                                                   1 - pCurrMB->MVSW);
            pCurrMB->pInterpolLumaSrc[1] = NULL;
            pCurrMB->InterpolsrcLumaStep[1] = 0;
            pCurrMB->pInterpolChromaUSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1] = 0;
            pCurrMB->pInterpolChromaVSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1] = 0;
        }
        break;
    case  VC1_MB_INTERP|VC1_MB_2MV_INTER:
    case  VC1_MB_DIRECT|VC1_MB_2MV_INTER:
        {
            Ipp8u pPredBlock[384];

            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

            pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
            pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;
            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
            pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

            pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
            pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

            //luma
            InterpolateBlock_InterlacePictureLuma2MV(pContext, pCurrMB->currYPlane,
                                                               pCurrMB->currYPitch,
                                                               0, 0);

            InterpolateBlock_InterlacePictureLuma2MV(pContext, pPredBlock,
                                                               VC1_PIXEL_IN_LUMA,
                                                               1, 1);

            ippiAverage16x16_8u_C1IR(pPredBlock,
                                     VC1_PIXEL_IN_LUMA,
                                     pCurrMB->currYPlane,
                                     pCurrMB->currYPitch);

            //chroma
            InterpolateBlock_InterlacePictureChroma2MV_B(pContext, pCurrMB->currUPlane,
                                                                   pCurrMB->currUPitch,
                                                                   pCurrMB->currVPlane,
                                                                   pCurrMB->currVPitch, 0, 0);

            InterpolateBlock_InterlacePictureChroma2MV_B(pContext, pPredBlock + 64*4,
                                                                   VC1_PIXEL_IN_CHROMA,
                                                                   pPredBlock + 64*5,
                                                                   VC1_PIXEL_IN_CHROMA, 1, 1);

             ippiAverage8x8_8u_C1IR(pPredBlock + 64*4,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currUPlane,
                        pCurrMB->currUPitch);

             ippiAverage8x8_8u_C1IR(pPredBlock + 64*5,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currVPlane,
                        pCurrMB->currVPitch);

            pCurrMB->pInterpolChromaUSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1] = 0;
            pCurrMB->pInterpolChromaVSrc[1] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1] = 0;
        }
         break;
    }

    return VC1_OK;
}


VC1Status PredictBlock_InterlaceFieldBPicture(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    Ipp32u _predict_type = pCurrMB->mbType;
    //Ipp8u* pPredBlock = pContext->m_pPredBlock;

    //assert(pPredBlock != NULL);

    Ipp32u predict_type = _predict_type >> 4;
    Ipp32u count = 0;
    Ipp32s plane_offset;

    if((pCurrMB->mbType&0x03) == VC1_MB_1MV_INTER)
    {
        //1MV
        if ((VC1_GET_PREDICT(_predict_type) != VC1_MB_INTERP) && (VC1_GET_PREDICT(_predict_type) != VC1_MB_DIRECT) )
        {
            pCurrMB->pInterpolLumaSrc[predict_type] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[predict_type] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[predict_type] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[predict_type] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[predict_type] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[predict_type] = pCurrMB->currVPitch;

            InterpolateBlock_InterlaceFieldPictureLuma1MV_B( pContext,  pCurrMB->currYPlane,
                                                             pCurrMB->currYPitch, predict_type);

            InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext, pCurrMB->currUPlane,
                                                                      pCurrMB->currUPitch,
                                                                      pCurrMB->currVPlane,
                                                                      pCurrMB->currVPitch,
                                                                      predict_type);

            pCurrMB->pInterpolLumaSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcLumaStep[1 - predict_type] = 0;
            pCurrMB->pInterpolChromaUSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1 - predict_type] = 0;
            pCurrMB->pInterpolChromaVSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1 - predict_type] = 0;

        }
        else
        {
            //pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            //pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

            //pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
            //pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;

            //pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
            //pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

            //pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
            //pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

            //pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            //pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            //pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            //pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;


            ////luma
            //InterpolateBlock_InterlaceFieldPictureLuma1MV_B(pContext, pCurrMB->currYPlane,
            //                                                       pCurrMB->currYPitch, 0);

            //InterpolateBlock_InterlaceFieldPictureLuma1MV_B(pContext, pPredBlock, 16, 1);

            ////chroma
            //InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext,  pCurrMB->currUPlane,
            //                                                           pCurrMB->currUPitch,
            //                                                           pCurrMB->currVPlane,
            //                                                           pCurrMB->currVPitch, 0);

            //InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext,  pPredBlock + 64*4, 8,
            //                                                           pPredBlock + 64*5, 8, 1);

            Ipp8u pPredBlock[384];

            pCurrMB->pInterpolLumaSrc[0] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[0] = pCurrMB->currYPitch;

            pCurrMB->pInterpolChromaUSrc[0] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[0] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[0] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[0] = pCurrMB->currVPitch;

            pCurrMB->pInterpolLumaSrc[1] = pPredBlock;
            pCurrMB->InterpolsrcLumaStep[1] = VC1_PIXEL_IN_LUMA;

            pCurrMB->pInterpolChromaUSrc[1] = pPredBlock + 64*4;
            pCurrMB->InterpolsrcChromaUStep[1] = VC1_PIXEL_IN_CHROMA;

            pCurrMB->pInterpolChromaVSrc[1] = pPredBlock + 64*5;
            pCurrMB->InterpolsrcChromaVStep[1] = VC1_PIXEL_IN_CHROMA;

            //luma
            //forward
            InterpolateBlock_InterlaceFieldPictureLuma1MV_B(pContext,
                pCurrMB->currYPlane,
                pCurrMB->currYPitch,
                0);

            if(pCurrMB->pInterpolLumaSrc[0] == pCurrMB->currYPlane)
            {
                //backward
                InterpolateBlock_InterlaceFieldPictureLuma1MV_B(pContext,
                    pPredBlock,
                    VC1_PIXEL_IN_LUMA,
                    1);

                // we should average and save data to plane
                //if (pCurrMB->pInterpolLumaSrc[0] == pPredBlock)
                if (pCurrMB->pInterpolLumaSrc[1] == pPredBlock)
                {
                    pCurrMB->pInterpolLumaSrc[1] = NULL;
                    pCurrMB->InterpolsrcLumaStep[1] = 0;
                    ippiAverage16x16_8u_C1IR(pPredBlock,
                        VC1_PIXEL_IN_LUMA,
                        pCurrMB->currYPlane,
                        pCurrMB->currYPitch);
                }
            }
            else
            {
                //backward
                InterpolateBlock_InterlaceFieldPictureLuma1MV_B(pContext,
                    pCurrMB->currYPlane,
                    pCurrMB->currYPitch,
                    1);

                //if(pCurrMB->pInterpolLumaSrc[1] == pCurrMB->currYPlane)
                if(pCurrMB->pInterpolLumaSrc[1] == pPredBlock)
                {
                    pCurrMB->pInterpolLumaSrc[1] = pCurrMB->currYPlane;
                    pCurrMB->InterpolsrcLumaStep[1] = pCurrMB->currYPitch;
                }
            }

            //chroma U, V
            InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext,
                pCurrMB->currUPlane,
                pCurrMB->currUPitch,
                pCurrMB->currVPlane,
                pCurrMB->currVPitch, 0);

            // check chroma
            if(pCurrMB->pInterpolChromaUSrc[0] == pCurrMB->currUPlane)
            {
                InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext,
                    pPredBlock + 64*4,
                    VC1_PIXEL_IN_CHROMA,
                    pPredBlock + 64*5,
                    VC1_PIXEL_IN_CHROMA, 1);
                // we should average and save data to plane
                if (pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
                {
                    pCurrMB->pInterpolChromaUSrc[1] = NULL;
                    pCurrMB->pInterpolChromaVSrc[1] = NULL;

                    ippiAverage8x8_8u_C1IR(pPredBlock + 64*4,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currUPlane,
                        pCurrMB->currUPitch);

                    ippiAverage8x8_8u_C1IR(pPredBlock + 64*5,
                        VC1_PIXEL_IN_CHROMA,
                        pCurrMB->currVPlane,
                        pCurrMB->currVPitch);
                }
            }
            else
            {
                //backward
                InterpolateBlock_InterlaceFieldPictureChroma1MV(pContext,
                    pCurrMB->currUPlane,
                    pCurrMB->currUPitch,
                    pCurrMB->currVPlane,
                    pCurrMB->currVPitch, 1);

                if(pCurrMB->pInterpolChromaUSrc[1] == (pPredBlock + 64*4))
                {
                    pCurrMB->pInterpolChromaUSrc[1] = pCurrMB->currUPlane;
                    pCurrMB->pInterpolChromaVSrc[1] = pCurrMB->currVPlane;
                    pCurrMB->InterpolsrcChromaUStep[1] = pCurrMB->currUPitch;
                    pCurrMB->InterpolsrcChromaVStep[1] = pCurrMB->currVPitch;

                }
            }

        }
    }
    else
    {
            pCurrMB->pInterpolLumaSrc[predict_type] = pCurrMB->currYPlane;
            pCurrMB->InterpolsrcLumaStep[predict_type] = pCurrMB->currYPitch;
            pCurrMB->pInterpolChromaUSrc[predict_type] = pCurrMB->currUPlane;
            pCurrMB->InterpolsrcChromaUStep[predict_type] = pCurrMB->currUPitch;

            pCurrMB->pInterpolChromaVSrc[predict_type] = pCurrMB->currVPlane;
            pCurrMB->InterpolsrcChromaVStep[predict_type] = pCurrMB->currVPitch;



        //4MV
            for (count = 0; count < VC1_NUM_OF_LUMA; count++)
            {
                plane_offset = (count&1)*8 + (count&2)*4*pCurrMB->currYPitch;
                InterpolateBlock_InterlaceFieldPictureLuma4MV(pContext, pCurrMB->currYPlane + plane_offset,
                                                              pCurrMB->currYPitch, count, predict_type);
            }

            InterpolateBlock_InterlaceFieldPictureChroma4MV(pContext,  pCurrMB->currUPlane,
                                                         pCurrMB->currUPitch,
                                                         pCurrMB->currVPlane,
                                                         pCurrMB->currVPitch, predict_type);
            VM_ASSERT((VC1_GET_PREDICT(_predict_type) != VC1_MB_INTERP) && (VC1_GET_PREDICT(_predict_type) != VC1_MB_DIRECT));

            pCurrMB->pInterpolLumaSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcLumaStep[1 - predict_type] = 0;
            pCurrMB->pInterpolChromaUSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcChromaUStep[1 - predict_type] = 0;
            pCurrMB->pInterpolChromaVSrc[1 - predict_type] = NULL;
            pCurrMB->InterpolsrcChromaVStep[1 - predict_type] = 0;


    }

    return VC1_OK;
}



#endif //UMC_ENABLE_VC1_VIDEO_DECODER

