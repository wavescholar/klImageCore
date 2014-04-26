/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, block layer advanced profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_common_zigzag_tbl.h"
#include "umc_vc1_common_blk_order_tbl.h"
#include "umc_vc1_dec_run_level_tbl.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_dec_time_statistics.h"
#include "umc_vc1_dec_exception.h"

#include "ippi.h"

using namespace UMC::VC1Exceptions;

typedef Ipp8u (*DCPrediction)(VC1DCBlkParam* CurrBlk, VC1DCPredictors* PredData,
                              Ipp32s blk_num, Ipp16s* pBlock, Ipp32u FCM);

//static IppiSize QuantSize[4] = {VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK,
//                                VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK,
//                                VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK/2,
//                               VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK/2};

typedef IppStatus (*Reconstruct)(Ipp16s* pSrcDst,
                                 Ipp32s srcDstStep,
                                 Ipp32s doubleQuant,
                                 Ipp32u BlkType);

static Reconstruct Reconstruct_table[] = {
        _ownReconstructInterUniform_VC1_16s_C1IR,
        _ownReconstructInterNonuniform_VC1_16s_C1IR
};

IppStatus _ownReconstructIntraUniform_VC1_16s_C1IR    (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant)
{
    IppiSize  DstSizeNZ;
    ippiQuantInvIntraUniform_VC1_16s_C1IR(pSrcDst,
                                               srcDstStep,
                                               doubleQuant,
                                               &DstSizeNZ);

    //transformation
    ippiTransform8x8Inv_VC1_16s_C1IR(pSrcDst,
                                     srcDstStep,
                                     DstSizeNZ);
    return ippStsNoErr;
}
IppStatus _ownReconstructIntraNonuniform_VC1_16s_C1IR (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant)
{
    IppiSize  DstSizeNZ;
    ippiQuantInvIntraNonuniform_VC1_16s_C1IR(pSrcDst,
                                                  srcDstStep,
                                                  doubleQuant,
                                                  &DstSizeNZ);

   //transformation
    ippiTransform8x8Inv_VC1_16s_C1IR(pSrcDst,
                                     srcDstStep,
                                     DstSizeNZ);
    return ippStsNoErr;
}
// Ipp32u BlkType -     VC1_BLK_INTER8X8   = 0x1,
//                      VC1_BLK_INTER8X4   = 0x2,
//                      VC1_BLK_INTER4X8   = 0x4,
//                      VC1_BLK_INTER4X4   = 0x8,
IppStatus _ownReconstructInterUniform_VC1_16s_C1IR    (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant,Ipp32u BlkType)
{
    IppiSize  DstSizeNZ[4];
    static IppiSize QuantSize[8] = {VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK,
                                    VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK,
                                    VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK/2,
                                    VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK/2};

    //quantization and transformation
    if (VC1_BLK_INTER8X8 == BlkType)
    {
        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[0],
                                                   &DstSizeNZ[0]);

        ippiTransform8x8Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);
    }
    else if(VC1_BLK_INTER4X8 == BlkType)
    {
        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[1],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst+4,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[1],
                                                   &DstSizeNZ[1]);

        ippiTransform4x8Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform4x8Inv_VC1_16s_C1IR(pSrcDst+4,
                                         srcDstStep,
                                         DstSizeNZ[1]);
    }
    else if(VC1_BLK_INTER8X4 == BlkType)
    {
        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[2],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[2],
                                                   &DstSizeNZ[1]);



        ippiTransform8x4Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform8x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                         srcDstStep,
                                         DstSizeNZ[1]);
    }
    else if(VC1_BLK_INTER4X4 == BlkType)
    {
        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst + 4,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[1]);

        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[2]);

        ippiQuantInvInterUniform_VC1_16s_C1IR(pSrcDst + 4 + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[3]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst+ 4,
                                         srcDstStep,
                                         DstSizeNZ[1]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep<<1),
                                         srcDstStep,
                                         DstSizeNZ[2]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep<<1) + 4,
                                         srcDstStep,
                                         DstSizeNZ[3]);
    }
    return ippStsNoErr;
}
IppStatus _ownReconstructInterNonuniform_VC1_16s_C1IR (Ipp16s* pSrcDst, Ipp32s srcDstStep, Ipp32s doubleQuant,Ipp32u BlkType)
{
    IppiSize  DstSizeNZ[4];
    static IppiSize QuantSize[8] = {VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK,
                                    VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK,
                                    VC1_PIXEL_IN_BLOCK, VC1_PIXEL_IN_BLOCK/2,
                                    VC1_PIXEL_IN_BLOCK/2, VC1_PIXEL_IN_BLOCK/2};

    //quantization and transformation
    if (VC1_BLK_INTER8X8 == BlkType)
    {
        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[0],
                                                   &DstSizeNZ[0]);

        ippiTransform8x8Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);


    }
    else if(VC1_BLK_INTER4X8 == BlkType)
    {
        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[1],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst+4,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[1],
                                                   &DstSizeNZ[1]);

        ippiTransform4x8Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform4x8Inv_VC1_16s_C1IR(pSrcDst+4,
                                         srcDstStep,
                                         DstSizeNZ[1]);
    }
    else if(VC1_BLK_INTER8X4 == BlkType)
    {
        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[2],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[2],
                                                   &DstSizeNZ[1]);



        ippiTransform8x4Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform8x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                         srcDstStep,
                                         DstSizeNZ[1]);
    }
    else if(VC1_BLK_INTER4X4 == BlkType)
    {
        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[0]);

        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst + 4,
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[1]);

        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[2]);

        ippiQuantInvInterNonuniform_VC1_16s_C1IR(pSrcDst + 4 + (srcDstStep << 1),
                                                   srcDstStep,
                                                   doubleQuant,
                                                   QuantSize[3],
                                                   &DstSizeNZ[3]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst,
                                         srcDstStep,
                                         DstSizeNZ[0]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst+ 4,
                                         srcDstStep,
                                         DstSizeNZ[1]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep<<1),
                                         srcDstStep,
                                         DstSizeNZ[2]);

        ippiTransform4x4Inv_VC1_16s_C1IR(pSrcDst + (srcDstStep<<1) + 4,
                                         srcDstStep,
                                         DstSizeNZ[3]);
    }
    return ippStsNoErr;
}
inline static void PredictACLeft(Ipp16s* pCurrAC, Ipp32u CurrQuant,
                                 Ipp16s* pPredAC, Ipp32u PredQuant,
                                 Ipp32s Step)
{
    Ipp32s i;
    Ipp32s Scale = VC1_DQScaleTbl[CurrQuant-1] * (PredQuant-1);
    Ipp32u step = Step;

    for (i = 1; i<VC1_PIXEL_IN_BLOCK; i++, step+=Step)
        pCurrAC[step] = pCurrAC[step] + (Ipp16s)((pPredAC[i] * Scale + 0x20000)>>18);
}

inline static void PredictACTop(Ipp16s* pCurrAC, Ipp32u CurrQuant,
                                Ipp16s* pPredAC, Ipp32u PredQuant)
{
    Ipp32s i;
    Ipp32s Scale = VC1_DQScaleTbl[CurrQuant-1] * (PredQuant-1);

    for (i = 1; i<VC1_PIXEL_IN_BLOCK; i++)
        pCurrAC[i] = pCurrAC[i] + (Ipp16s)((pPredAC[i] * Scale + 0x20000)>>18);
}


static Ipp8u GetDCACPrediction(VC1DCBlkParam* CurrBlk, VC1DCPredictors* PredData,
                               Ipp32s blk_num, Ipp16s* pBlock, Ipp32u FCM)
{
    Ipp8u blkType = VC1_BLK_INTRA;

    VC1DCPredictors DCPred;
    Ipp8u PredPattern;
    Ipp32u CurrQuant = PredData->DoubleQuant[2];

    Ipp16s DCA, DCB, DCC, DC = 0;
    Ipp32u step = VC1_pixel_table[blk_num];

    memcpy(&DCPred, PredData, sizeof(VC1DCPredictors));
    PredPattern = DCPred.BlkPattern[blk_num];

    switch(PredPattern)
    {
    case 7:
        {
            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];
            DCB = DCPred.DC[VC1_PredDCIndex[1][blk_num]];
            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];

            if (vc1_abs_16s(DCB - DCA) <= vc1_abs_16s(DCB - DCC))
            {

                DC = CurrBlk->DC + DCC;
                PredictACLeft(pBlock, CurrQuant, DCPred.ACLEFT[VC1_PredDCIndex[2][blk_num]],
                    DCPred.DoubleQuant[VC1_QuantIndex[1][blk_num]], step);

                blkType =  VC1_BLK_INTRA_LEFT;
            }
            else
            {

                DC = CurrBlk->DC + DCA;
                PredictACTop(pBlock, CurrQuant, DCPred.ACTOP[VC1_PredDCIndex[0][blk_num]],
                    DCPred.DoubleQuant[VC1_QuantIndex[0][blk_num]]);

                blkType = VC1_BLK_INTRA_TOP;
            }
        }
        break;
    case 4:
    case 6:
        {
            //A is available, C - not

            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];
            DC = CurrBlk->DC + DCA;

            PredictACTop(pBlock, CurrQuant, DCPred.ACTOP[VC1_PredDCIndex[0][blk_num]],
                DCPred.DoubleQuant[VC1_QuantIndex[0][blk_num]]);
            blkType = VC1_BLK_INTRA_TOP;
        }
        break;
    case 1:
    case 3:
        {
            //C is available, A - not

            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];
            DC = CurrBlk->DC + DCC;
            PredictACLeft(pBlock, CurrQuant, DCPred.ACLEFT[VC1_PredDCIndex[2][blk_num]],
                DCPred.DoubleQuant[VC1_QuantIndex[1][blk_num]], step);
            blkType = VC1_BLK_INTRA_LEFT;
        }
        break;
    case 5:
        {
            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];
            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];

            if (vc1_abs_16s(DCA) <= vc1_abs_16s(DCC))
            {

                DC= CurrBlk->DC + DCC;
                PredictACLeft(pBlock, CurrQuant, DCPred.ACLEFT[VC1_PredDCIndex[2][blk_num]],
                    DCPred.DoubleQuant[VC1_QuantIndex[1][blk_num]], step);
                blkType = VC1_BLK_INTRA_LEFT;
            }
            else
            {

                DC = CurrBlk->DC + DCA;
                PredictACTop(pBlock, CurrQuant, DCPred.ACTOP[VC1_PredDCIndex[0][blk_num]],
                    DCPred.DoubleQuant[VC1_QuantIndex[0][blk_num]]);
                blkType = VC1_BLK_INTRA_TOP;
            }
        }
        break;
    case 0:
    case 2:
        {
            // A, C unavailable

            DC = CurrBlk->DC;
            blkType = VC1_BLK_INTRA_LEFT;

            if(FCM)
                blkType = VC1_BLK_INTRA;
        }
        break;
    }

    pBlock[0] = DC;
    PredData->DC[blk_num] = DC;
    CurrBlk->DC = DC;
    return blkType;
}

static Ipp8u GetDCPrediction(VC1DCBlkParam* CurrBlk,VC1DCPredictors* PredData,
                             Ipp32s blk_num, Ipp16s* pBlock, Ipp32u FCM)
{
    Ipp8u blkType = VC1_BLK_INTRA;

    VC1DCPredictors DCPred;
    Ipp8u PredPattern;

    Ipp16s DCA, DCB, DCC = 0;


    memcpy(&DCPred, PredData, sizeof(VC1DCPredictors));

    PredPattern = DCPred.BlkPattern[blk_num];

    switch(PredPattern)
    {
    case 7:
        {
            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];
            DCB = DCPred.DC[VC1_PredDCIndex[1][blk_num]];
            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];

            if (vc1_abs_16s(DCB - DCA) <= vc1_abs_16s(DCB - DCC))
            {

                CurrBlk->DC = CurrBlk->DC + DCC;
                blkType =  VC1_BLK_INTRA_LEFT;
            }
            else
            {

                CurrBlk->DC = CurrBlk->DC + DCA;
                blkType = VC1_BLK_INTRA_TOP;
            }
        }
        break;
    case 4:
    case 6:
        {
            //A is available, C - not

            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];

            CurrBlk->DC = CurrBlk->DC + DCA;
            blkType = VC1_BLK_INTRA_TOP;
        }
        break;
    case 1:
    case 3:
        {
            //C is available, A - not

            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];
            CurrBlk->DC = CurrBlk->DC + DCC;
            blkType = VC1_BLK_INTRA_LEFT;
        }
        break;
    case 5:
        {
            DCA = DCPred.DC[VC1_PredDCIndex[0][blk_num]];
            DCC = DCPred.DC[VC1_PredDCIndex[2][blk_num]];

            if (vc1_abs_16s(DCA) <= vc1_abs_16s(DCC))
            {

                CurrBlk->DC = CurrBlk->DC + DCC;
                blkType = VC1_BLK_INTRA_LEFT;
            }
            else
            {

                CurrBlk->DC = CurrBlk->DC + DCA;
                blkType = VC1_BLK_INTRA_TOP;
            }
        }
        break;
    case 0:
    case 2:
        {
            // A, C unavailable

            blkType = VC1_BLK_INTRA_LEFT;
            if(FCM)
                blkType = VC1_BLK_INTRA;
        }
        break;
    }

    pBlock[0] = CurrBlk->DC;
    PredData->DC[blk_num] = CurrBlk->DC;
    return blkType;
}

static const DCPrediction DCPredictionTable[] =
{
        (DCPrediction)(GetDCPrediction),
        (DCPrediction)(GetDCACPrediction)
};

VC1Status BLKLayer_Intra_Luma_Adv(VC1Context* pContext, Ipp32s blk_num, Ipp32u ACPRED)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    VC1DCMBParam*  CurrDC = pContext->CurrDC;
    VC1DCBlkParam* CurrBlk = &CurrDC->DCBlkPred[blk_num];
    VC1DCPredictors* DCPred = &pContext->DCPred;

    IppStatus ret;
    Ipp32s DCCOEF;
    Ipp32s DCSIGN;
    Ipp32u i = 0;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Intra_StartTime);

// need to calculate bits for residual data
#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &DCCOEF,
                                     pContext->m_picLayerHeader->m_pCurrLumaDCDiff);
    //VM_ASSERT(ret == ippStsNoErr);

#ifdef VC1_VLD_CHECK
    if (ret != ippStsNoErr)
        throw vc1_exception(vld);
#endif

    if(DCCOEF != 0)
    {
        Ipp32u quant =  (CurrDC->DoubleQuant >> 1);

        if(DCCOEF == IPPVC_ESCAPE)
        {
           if(quant == 1)
           {
              VC1_GET_BITS(10, DCCOEF);
           }
           else if(quant == 2)
           {
              VC1_GET_BITS(9, DCCOEF);
           }
           else // pContext->m_pCurrMB->MQUANT is > 2
           {
              VC1_GET_BITS(8, DCCOEF);
           }
        }
        else
        {  // DCCOEF is not IPPVC_ESCAPE
           Ipp32s tmp;
           if(quant == 1)
           {
              VC1_GET_BITS(2, tmp);
               DCCOEF = DCCOEF*4 + tmp - 3;
           }
           else if(quant == 2)
           {
              VC1_GET_BITS(1, tmp);
              DCCOEF = DCCOEF*2 + tmp - 1;
           }
        }

        VC1_GET_BITS(1, DCSIGN);
         DCCOEF = (1 - (DCSIGN<<1))* DCCOEF;
    }

    CurrBlk->DC = (Ipp16s)DCCOEF;

    pBlock->blkType = DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num, m_pBlock, pContext->m_picLayerHeader->FCM);


    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        const Ipp8u* curr_scan = pContext->m_pSingleMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
        if(curr_scan==NULL)
            return VC1_FAIL;

        DecodeBlockACIntra_VC1(&pContext->m_bitstream,
                    m_pBlock,
                    curr_scan, pContext->m_picLayerHeader->m_pCurrIntraACDecSet,
                    &pContext->m_pSingleMB->EscInfo);
    }

    for(i = 1; i < 8; i++)
    {
        CurrBlk->ACLEFT[i] = m_pBlock[i*16];
        CurrBlk->ACTOP[i]  = m_pBlock[i];
    }


   STATISTICS_END_TIME(m_timeStatistics->decoding_Intra_StartTime,
                        m_timeStatistics->decoding_Intra_EndTime,
                        m_timeStatistics->decoding_Intra_TotalTime);



#ifdef UMC_STREAM_ANALYZER
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsRes += CalculateUsedBits(Bitsream,pContext->m_bitstream);
#endif

    return VC1_OK;
}

VC1Status BLKLayer_Intra_Chroma_Adv(VC1Context* pContext, Ipp32s blk_num,Ipp32u ACPRED)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    VC1DCMBParam*  CurrDC = pContext->CurrDC;
    VC1DCBlkParam* CurrBlk = &CurrDC->DCBlkPred[blk_num];

    VC1DCPredictors* DCPred = &pContext->DCPred;

    IppStatus ret;
    Ipp32s DCCOEF;
    Ipp32s DCSIGN;
    Ipp32u i = 0;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Intra_StartTime);

    // need to calculate bits for residual data
#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &DCCOEF,
                                     pContext->m_picLayerHeader->m_pCurrChromaDCDiff);
    //VM_ASSERT(ret == ippStsNoErr);

#ifdef VC1_VLD_CHECK
    if (ret != ippStsNoErr)
        throw vc1_exception(vld);
#endif

    if(DCCOEF != 0)
    {
        Ipp32u quant =  (CurrDC->DoubleQuant >> 1);

        if(DCCOEF == IPPVC_ESCAPE)
        {
           if(quant == 1)
           {
              VC1_GET_BITS(10, DCCOEF);
           }
           else if(quant == 2)
           {
              VC1_GET_BITS(9, DCCOEF);
           }
           else // pContext->m_pCurrMB->MQUANT is > 2
           {
              VC1_GET_BITS(8, DCCOEF);
           }
        }
        else
        {  // DCCOEF is not IPPVC_ESCAPE
           Ipp32s tmp;
           if(quant == 1)
           {
              VC1_GET_BITS(2, tmp);
              DCCOEF = DCCOEF*4 + tmp - 3;
           }
           else if(quant == 2)
           {
              VC1_GET_BITS(1, tmp);
              DCCOEF = DCCOEF*2 + tmp - 1;
           }
        }

        VC1_GET_BITS(1, DCSIGN);

        DCCOEF = (1 - (DCSIGN<<1))* DCCOEF;
        //if (DCSIGN == 1)
        //    DCCOEF = -DCCOEF;
    }

    CurrBlk->DC = (Ipp16s)DCCOEF;


    pBlock->blkType = DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num, m_pBlock, pContext->m_picLayerHeader->FCM);


    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        const Ipp8u* curr_scan = pContext->m_pSingleMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
        if(curr_scan==NULL)
            return VC1_FAIL;

        DecodeBlockACIntra_VC1(&pContext->m_bitstream, m_pBlock,
                     curr_scan, pContext->m_picLayerHeader->m_pCurrInterACDecSet,
                     &pContext->m_pSingleMB->EscInfo);
   }

    for(i = 1; i < 8; i++)

    {
        CurrBlk->ACLEFT[i] = m_pBlock[i*8];
        CurrBlk->ACTOP[i]  = m_pBlock[i];
    }

STATISTICS_END_TIME(m_timeStatistics->decoding_Intra_StartTime,
                        m_timeStatistics->decoding_Intra_EndTime,
                        m_timeStatistics->decoding_Intra_TotalTime);

#ifdef UMC_STREAM_ANALYZER
    Ipp32u bit_shift = CalculateUsedBits(Bitsream,pContext->m_bitstream);
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsRes += bit_shift;
#endif

    return VC1_OK;
}

VC1Status VC1ProcessDiffIntra(VC1Context* pContext, Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock; //memory for 16s diffs
    IppiSize  roiSize;
    roiSize.height = VC1_PIXEL_IN_BLOCK;
    roiSize.width = VC1_PIXEL_IN_BLOCK;
    Ipp16s bias = 128;


    if ((pContext->m_seqLayerHeader->PROFILE != VC1_PROFILE_ADVANCED)&&
        ((pContext->m_picLayerHeader->PTYPE == VC1_I_FRAME)||
         (pContext->m_picLayerHeader->PTYPE == VC1_BI_FRAME)))
         bias = pContext->m_pCurrMB->bias;

STATISTICS_START_TIME(m_timeStatistics->reconstruction_StartTime);

     *(pContext->m_pBlock+VC1_BlkStart[blk_num]) = *(pContext->m_pBlock+VC1_BlkStart[blk_num])
                                                   * (Ipp16s)pContext->CurrDC->DCStepSize;

     if(pContext->m_picLayerHeader->QuantizationType == VC1_QUANTIZER_UNIFORM)
         _ownReconstructIntraUniform_VC1_16s_C1IR(pContext->m_pBlock+ VC1_BlkStart[blk_num],
                                                  VC1_pixel_table[blk_num]*2,
                                                  pContext->CurrDC->DoubleQuant);
     else
         _ownReconstructIntraNonuniform_VC1_16s_C1IR(pContext->m_pBlock+ VC1_BlkStart[blk_num],
                                                  VC1_pixel_table[blk_num]*2,
                                                  pContext->CurrDC->DoubleQuant);
        roiSize.height = VC1_PIXEL_IN_BLOCK;
        roiSize.width = VC1_PIXEL_IN_BLOCK;


        ippiAddC_16s_C1IRSfs(bias, m_pBlock + VC1_BlkStart[blk_num],
                                    2*VC1_pixel_table[blk_num], roiSize, 0);
STATISTICS_END_TIME(m_timeStatistics->reconstruction_StartTime,
                    m_timeStatistics->reconstruction_EndTime,
                    m_timeStatistics->reconstruction_TotalTime);
    return VC1_OK;
}

VC1Status BLKLayer_Inter_Luma_Adv(VC1Context* pContext, Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    const Ipp8u* curr_scan = NULL;
    Ipp32u numCoef = 0;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader * picHeader = pContext->m_picLayerHeader;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Inter_StartTime);

#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        switch (pBlock->blkType)
        {
        case VC1_BLK_INTER8X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan == NULL)
                    return VC1_FAIL;
#endif

                sMB->m_pSingleBlock[blk_num].numCoef = VC1_SBP_0;
                DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                            curr_scan,picHeader->m_pCurrInterACDecSet,
                            &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
            }
            break;
        case VC1_BLK_INTER8X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM ==  pBlock->blkType)
                     numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                 DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;
        case VC1_BLK_INTER4X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM == pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                 DecodeBlockInter4x8_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);

            }
            break;
        case VC1_BLK_INTER4X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                numCoef = GetSubBlockPattern_4x4(pContext, blk_num);

                DecodeBlockInter4x4_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;

        case VC1_BLK_INTER:
            {
                numCoef = GetTTBLK(pContext, blk_num);

                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                switch (pBlock->blkType)
                {
                case VC1_BLK_INTER8X8:
                    {
                        DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                                    curr_scan, picHeader->m_pCurrInterACDecSet,
                                    &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
                    }
                    break;
                case VC1_BLK_INTER8X4:
                    {
                        DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                        curr_scan, picHeader->m_pCurrInterACDecSet,
                                        &pContext->m_pSingleMB->EscInfo, numCoef);
                    }
                    break;
                case VC1_BLK_INTER4X8:
                    {
                     DecodeBlockInter4x8_VC1(&pContext->m_bitstream, m_pBlock,
                                        curr_scan, picHeader->m_pCurrInterACDecSet,
                                        &pContext->m_pSingleMB->EscInfo, numCoef);
                    }
                    break;
                case VC1_BLK_INTER4X4:
                    {
                        numCoef = GetSubBlockPattern_4x4(pContext, blk_num);

                        DecodeBlockInter4x4_VC1(&pContext->m_bitstream, m_pBlock,
                                        curr_scan, picHeader->m_pCurrInterACDecSet,
                                        &pContext->m_pSingleMB->EscInfo, numCoef);
                    }
                    break;
                }
            }
            break;
        default:
            VM_ASSERT(0);
        }

STATISTICS_END_TIME(m_timeStatistics->decoding_Inter_StartTime,
                        m_timeStatistics->decoding_Inter_EndTime,
                        m_timeStatistics->decoding_Inter_TotalTime);

    }

#ifdef UMC_STREAM_ANALYZER
    Ipp32u bit_shift = CalculateUsedBits(Bitsream,pContext->m_bitstream);
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsRes += bit_shift;
#endif

    return VC1_OK;
}

VC1Status BLKLayer_Inter_Chroma_Adv(VC1Context* pContext, Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    const Ipp8u* curr_scan = NULL;
    Ipp32u numCoef = 0;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader * picHeader = pContext->m_picLayerHeader;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Inter_StartTime);

#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        switch (pBlock->blkType)
        {
        case VC1_BLK_INTER8X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                sMB->m_pSingleBlock[blk_num].numCoef = VC1_SBP_0;
                DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                            &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
            }
            break;
        case VC1_BLK_INTER8X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM ==  pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                 DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;
        case VC1_BLK_INTER4X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM ==  pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                 DecodeBlockInter4x8_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;

        case VC1_BLK_INTER4X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                numCoef = GetSubBlockPattern_4x4(pContext, blk_num);

                DecodeBlockInter4x4_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan,picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;

        case VC1_BLK_INTER:
            {
                numCoef = GetTTBLK(pContext, blk_num);

                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];

#ifdef VC1_VLD_CHECK
                if(curr_scan==NULL)
                    return VC1_FAIL;
#endif

                switch (pBlock->blkType)
                {
                case VC1_BLK_INTER8X8:
                    DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
                    break;
                case VC1_BLK_INTER8X4:
                    DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                    curr_scan, picHeader->m_pCurrInterACDecSet,
                                    &pContext->m_pSingleMB->EscInfo, numCoef);
                    break;
                case VC1_BLK_INTER4X8:
                     DecodeBlockInter4x8_VC1(&pContext->m_bitstream, m_pBlock,
                                    curr_scan, picHeader->m_pCurrInterACDecSet,
                                    &pContext->m_pSingleMB->EscInfo, numCoef);
                    break;
                case VC1_BLK_INTER4X4:
                    numCoef = GetSubBlockPattern_4x4(pContext, blk_num);

                    DecodeBlockInter4x4_VC1(&pContext->m_bitstream, m_pBlock,
                                    curr_scan, picHeader->m_pCurrInterACDecSet,
                                    &pContext->m_pSingleMB->EscInfo, numCoef);
                    break;
                }
            }
            break;

        default:
            VM_ASSERT(0);
        }

STATISTICS_END_TIME(m_timeStatistics->decoding_Inter_StartTime,
                        m_timeStatistics->decoding_Inter_EndTime,
                        m_timeStatistics->decoding_Inter_TotalTime);
    }

#ifdef UMC_STREAM_ANALYZER
    Ipp32u bit_shift = CalculateUsedBits(Bitsream,pContext->m_bitstream);
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsRes += bit_shift;
#endif

    return VC1_OK;
}

VC1Status VC1ProcessDiffInter(VC1Context* pContext,Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num]; //memory for 16s diffs

    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        //quantization and transformation
STATISTICS_START_TIME(m_timeStatistics->reconstruction_StartTime);
        Reconstruct_table[pContext->m_picLayerHeader->QuantizationType](m_pBlock,
                                                                        VC1_pixel_table[blk_num]*2,
                                                                        pContext->CurrDC->DoubleQuant,
                                                                        pContext->m_pCurrMB->m_pBlocks[blk_num].blkType);
STATISTICS_END_TIME(m_timeStatistics->reconstruction_StartTime,
                    m_timeStatistics->reconstruction_EndTime,
                    m_timeStatistics->reconstruction_TotalTime);
    }
    return VC1_OK;
}
VC1Status VC1ProcessDiffSpeedUpIntra(VC1Context* pContext,Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[0]; //memory for 16s diffs
    IppiSize  roiSize;
    roiSize.height = VC1_PIXEL_IN_BLOCK;
    roiSize.width = VC1_PIXEL_IN_BLOCK;
    IppiSize  DstSizeNZ;
    Ipp16s bias = 128;

    m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    //DC
    *(pContext->m_pBlock+VC1_BlkStart[blk_num]) = *(pContext->m_pBlock+VC1_BlkStart[blk_num])
        * (Ipp16s)pContext->CurrDC->DCStepSize;

    if ((pContext->m_seqLayerHeader->PROFILE != VC1_PROFILE_ADVANCED)&&
        ((pContext->m_picLayerHeader->PTYPE == VC1_I_FRAME)||
        (pContext->m_picLayerHeader->PTYPE == VC1_BI_FRAME)))
        bias = pContext->m_pCurrMB->bias;

    if (pContext->m_picLayerHeader->QuantizationType == VC1_QUANTIZER_UNIFORM)
    {
        ippiQuantInvIntraUniform_VC1_16s_C1IR(m_pBlock,
            VC1_pixel_table[blk_num]*2,
            pContext->CurrDC->DoubleQuant,
            &DstSizeNZ);
    }
    else
    {
        ippiQuantInvIntraNonuniform_VC1_16s_C1IR(m_pBlock,
            VC1_pixel_table[blk_num]*2,
            pContext->CurrDC->DoubleQuant,
            &DstSizeNZ);

    }

    ippiTransform8x8Inv_VC1_16s_C1IR(m_pBlock,
                                     VC1_pixel_table[blk_num]*2,
                                     DstSizeNZ);
    ippiAddC_16s_C1IRSfs(bias, m_pBlock, 2*VC1_pixel_table[blk_num], roiSize, 0);
    return VC1_OK;

}
VC1Status VC1ProcessDiffSpeedUpInter(VC1Context* pContext,Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num]; //memory for 16s diffs
    IppiSize  roiSize;
    roiSize.height = VC1_PIXEL_IN_BLOCK;
    roiSize.width = VC1_PIXEL_IN_BLOCK;
    IppiSize  DstSizeNZ;
    IppiSize QuantSize = {4,4};
    m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        if (pContext->m_picLayerHeader->QuantizationType == VC1_QUANTIZER_UNIFORM)
        {
            ippiQuantInvInterUniform_VC1_16s_C1IR(m_pBlock,
                VC1_pixel_table[blk_num]*2,
                pContext->CurrDC->DoubleQuant,
                QuantSize,
                &DstSizeNZ);
        }
        else
        {
            ippiQuantInvInterNonuniform_VC1_16s_C1IR(m_pBlock,
                VC1_pixel_table[blk_num]*2,
                pContext->CurrDC->DoubleQuant,
                QuantSize,
                &DstSizeNZ);
        }

        if (VC1_BLK_INTER8X8 == pBlock->blkType)
        {
            ippiTransform8x8Inv_VC1_16s_C1IR(m_pBlock,
                                            VC1_pixel_table[blk_num]*2,
                                            DstSizeNZ);
        }
        else if(VC1_BLK_INTER4X8 == pBlock->blkType)
        {
            ippiTransform4x8Inv_VC1_16s_C1IR(m_pBlock,
                                            VC1_pixel_table[blk_num]*2,
                                            DstSizeNZ);

        }
        else if(VC1_BLK_INTER8X4 == pBlock->blkType)
        {

            ippiTransform8x4Inv_VC1_16s_C1IR(m_pBlock,
                                            VC1_pixel_table[blk_num]*2,
                                            DstSizeNZ);

        }
        else if(VC1_BLK_INTER4X4 == pBlock->blkType)
        {
        ippiTransform4x4Inv_VC1_16s_C1IR(m_pBlock,
                                        VC1_pixel_table[blk_num]*2,
                                        DstSizeNZ);
        }
        }
    return VC1_OK;
}
void write_Intraluma_to_interlace_frame_Adv(VC1MB * pCurrMB, Ipp16s* pBlock)
{
    IppiSize roiSize;
    Ipp32u planeStep[2] = {pCurrMB->currYPitch,
                           pCurrMB->currYPitch << 1};

    Ipp32u planeOffset[2] = {pCurrMB->currYPitch << 3,
                             pCurrMB->currYPitch};

    roiSize.height = VC1_PIXEL_IN_BLOCK;
    roiSize.width = VC1_PIXEL_IN_LUMA;

    ippiConvert_16s8u_C1R(pBlock,
                          VC1_PIXEL_IN_LUMA << 1,
                          pCurrMB->currYPlane,
                          planeStep[pCurrMB->FIELDTX],
                          roiSize);

    ippiConvert_16s8u_C1R(pBlock + 128,
                          VC1_PIXEL_IN_LUMA << 1,
                          pCurrMB->currYPlane +  planeOffset[pCurrMB->FIELDTX],
                          planeStep[pCurrMB->FIELDTX],
                          roiSize);
}


void write_Interluma_to_interlace_frame_MC_Adv(VC1MB * pCurrMB,
                                               const Ipp8u* pDst,
                                               Ipp32u dstStep,
                                               Ipp16s* pBlock)
{
    Ipp8u fieldFlag = (Ipp8u)(pCurrMB->FIELDTX*2 + VC1_IS_MVFIELD(pCurrMB->mbType));

    Ipp32u predOffset[4] = {dstStep << 3, dstStep << 3, dstStep, dstStep << 3};

    Ipp32s predStep[4] = {dstStep,   dstStep,
                          dstStep << 1, dstStep};

    Ipp32u planeOffset[4] = {pCurrMB->currYPitch << 3 ,  pCurrMB->currYPitch,
                               pCurrMB->currYPitch,  pCurrMB->currYPitch};

    Ipp32u planeStep[4] = {pCurrMB->currYPitch,      pCurrMB->currYPitch << 1,
                           pCurrMB->currYPitch << 1,    pCurrMB->currYPitch << 1};
    // Skip MB
    if (pCurrMB->SkipAndDirectFlag & 2)
    {
        IppiSize  roiSize;
        roiSize.width = 16;
        roiSize.height = 8;
        ippiCopy_8u_C1R(pDst,
                        predStep[fieldFlag],
                        pCurrMB->currYPlane,
                        planeStep[fieldFlag],
                        roiSize);
        ippiCopy_8u_C1R(pDst + predOffset[fieldFlag],
                        predStep[fieldFlag],
                        pCurrMB->currYPlane + planeOffset[fieldFlag],
                        planeStep[fieldFlag],
                        roiSize);

    }
    else
    {
        Ipp16u blockOffset[4] = {128, VC1_PIXEL_IN_LUMA, 128, 128};

        Ipp16u blockStep[4] = {VC1_PIXEL_IN_LUMA << 1,   VC1_PIXEL_IN_LUMA << 2,
            VC1_PIXEL_IN_LUMA << 1,   VC1_PIXEL_IN_LUMA << 1};

        ippiMC16x8_8u_C1(pDst,  predStep[fieldFlag],
                         pBlock, blockStep[fieldFlag],
                         pCurrMB->currYPlane,
                         planeStep[fieldFlag], 0, 0);

        ippiMC16x8_8u_C1(pDst + predOffset[fieldFlag],
                         predStep[fieldFlag],
                         pBlock + blockOffset[fieldFlag],
                         blockStep[fieldFlag],
                         pCurrMB->currYPlane + planeOffset[fieldFlag],
                         planeStep[fieldFlag], 0, 0);
    }
}


void write_Interluma_to_interlace_frame_MC_Adv_Copy(VC1MB * pCurrMB,
                                                   Ipp16s* pBlock)
{
    Ipp8u pPred[64*4];
    Ipp8u fieldFlag = (Ipp8u)(pCurrMB->FIELDTX*2 + VC1_IS_MVFIELD(pCurrMB->mbType));

    Ipp16u predOffset[4] = {64*2, 64*2, VC1_PIXEL_IN_LUMA, 64*2};
    Ipp16u predStep[4] = {VC1_PIXEL_IN_LUMA,
        VC1_PIXEL_IN_LUMA,
        2*VC1_PIXEL_IN_LUMA,
        VC1_PIXEL_IN_LUMA
    };
    Ipp16u blockOffset[4] = {64*2, VC1_PIXEL_IN_LUMA, 64*2, 64*2};
    Ipp16u blockStep[4] = {2*VC1_PIXEL_IN_LUMA,
        2*2*VC1_PIXEL_IN_LUMA,
        2*VC1_PIXEL_IN_LUMA,
        2*VC1_PIXEL_IN_LUMA
    };

    Ipp32u planeOffset[4] = {8*pCurrMB->currYPitch,
        pCurrMB->currYPitch,
        pCurrMB->currYPitch,
        pCurrMB->currYPitch};
    Ipp32u planeStep[4] = {pCurrMB->currYPitch,
        pCurrMB->currYPitch*2,
        pCurrMB->currYPitch*2,
        pCurrMB->currYPitch*2
    };

    ippiCopy16x16_8u_C1R(pCurrMB->currYPlane,
                         pCurrMB->currYPitch,
                         pPred,
                         VC1_PIXEL_IN_LUMA);

    ippiMC16x8_8u_C1(pPred,
        predStep[fieldFlag],
        pBlock,
        blockStep[fieldFlag],
        pCurrMB->currYPlane,
        planeStep[fieldFlag], 0, 0);

    ippiMC16x8_8u_C1(pPred + predOffset[fieldFlag],
        predStep[fieldFlag],
        pBlock + blockOffset[fieldFlag],
        blockStep[fieldFlag],
        pCurrMB->currYPlane + planeOffset[fieldFlag],
        planeStep[fieldFlag], 0, 0);
}
void write_Interluma_to_interlace_B_frame_MC_Adv(VC1MB * pCurrMB,
                                               const Ipp8u* pDst1, Ipp32u dstStep1,
                                               const Ipp8u* pDst2, Ipp32u dstStep2,
                                               Ipp16s* pBlock)
{
    Ipp8u pPred[256]={0};

    Ipp8u fieldFlag = (Ipp8u)(pCurrMB->FIELDTX*2 + VC1_IS_MVFIELD(pCurrMB->mbType));

    Ipp16u predOffset[4] = {8*VC1_PIXEL_IN_LUMA, 8*VC1_PIXEL_IN_LUMA, VC1_PIXEL_IN_LUMA, 8*VC1_PIXEL_IN_LUMA};

    Ipp16u predStep[4] = {VC1_PIXEL_IN_LUMA,   VC1_PIXEL_IN_LUMA,   2*VC1_PIXEL_IN_LUMA, VC1_PIXEL_IN_LUMA};

    Ipp32u planeOffset[4] = {8*pCurrMB->currYPitch,  pCurrMB->currYPitch,
                               pCurrMB->currYPitch,  pCurrMB->currYPitch};
    Ipp32u planeStep[4] = {pCurrMB->currYPitch,      pCurrMB->currYPitch*2,
                           pCurrMB->currYPitch*2,    pCurrMB->currYPitch*2};

    ippiAverage16x16_8u_C1R(pDst1, dstStep1,  pDst2, dstStep2,
                            pPred, VC1_PIXEL_IN_LUMA);

        // Skip MB
    if (pCurrMB->SkipAndDirectFlag & 2)
    {
        IppiSize  roiSize;
        roiSize.width = 16;
        roiSize.height = 8;
        ippiCopy_8u_C1R(pPred,
                        predStep[fieldFlag],
                        pCurrMB->currYPlane,
                        planeStep[fieldFlag],
                        roiSize);
        ippiCopy_8u_C1R(pPred + predOffset[fieldFlag],
                        predStep[fieldFlag],
                        pCurrMB->currYPlane + planeOffset[fieldFlag],
                        planeStep[fieldFlag],
                        roiSize);
    }
    else
    {
        Ipp16u blockOffset[4] = {64*2, VC1_PIXEL_IN_LUMA, 64*2, 64*2};
        Ipp16u blockStep[4] = {2*VC1_PIXEL_IN_LUMA,   2*2*VC1_PIXEL_IN_LUMA,
                           2*VC1_PIXEL_IN_LUMA,   2*VC1_PIXEL_IN_LUMA};

        ippiMC16x8_8u_C1(pPred,  predStep[fieldFlag],
                         pBlock, blockStep[fieldFlag],
                         pCurrMB->currYPlane,
                         planeStep[fieldFlag], 0, 0);
        ippiMC16x8_8u_C1(pPred + predOffset[fieldFlag],
                         predStep[fieldFlag],
                         pBlock + blockOffset[fieldFlag],
                         blockStep[fieldFlag],
                         pCurrMB->currYPlane + planeOffset[fieldFlag],
                         planeStep[fieldFlag], 0, 0);
    }
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
