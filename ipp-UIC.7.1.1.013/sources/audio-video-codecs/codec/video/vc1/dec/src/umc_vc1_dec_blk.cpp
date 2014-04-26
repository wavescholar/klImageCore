/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, block layer, simple\main profile
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

#include "ippi.h"

typedef Ipp8u (*DCPrediction)(VC1DCBlkParam* CurrBlk, VC1DCPredictors* PredData,
                               Ipp32s blk_num, Ipp16s* pBlock,Ipp16s defaultDC, Ipp32u PTYPE);

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
                               Ipp32s blk_num, Ipp16s* pBlock,Ipp16s defaultDC, Ipp32u PTYPE)
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

            if (!(DCA==defaultDC && (PTYPE == VC1_I_FRAME ||  PTYPE == VC1_BI_FRAME)))
            {
                PredictACTop(pBlock, CurrQuant, DCPred.ACTOP[VC1_PredDCIndex[0][blk_num]],
                    DCPred.DoubleQuant[VC1_QuantIndex[0][blk_num]]);
                blkType = VC1_BLK_INTRA_TOP;
            }
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
            DCB = defaultDC;
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

            blkType = VC1_BLK_INTRA_LEFT;
            DC = CurrBlk->DC + defaultDC;
         }
        break;
    }

    pBlock[0] = DC;
    PredData->DC[blk_num] = DC;
    CurrBlk->DC = DC;
    return blkType;
}

static Ipp8u GetDCPrediction(VC1DCBlkParam* CurrBlk,VC1DCPredictors* PredData,
                             Ipp32s blk_num, Ipp16s* pBlock,Ipp16s defaultDC, Ipp32u PTYPE)
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
            if (!(DCA==defaultDC && (PTYPE == VC1_I_FRAME ||  PTYPE == VC1_BI_FRAME)))
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

            CurrBlk->DC = CurrBlk->DC + defaultDC;
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

VC1Status BLKLayer_Intra_Luma(VC1Context* pContext, Ipp32s blk_num, Ipp32u bias, Ipp32u ACPRED)
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

    Ipp32u quant = CurrDC->DoubleQuant>>1;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Intra_StartTime);

    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &DCCOEF,
                                     pContext->m_picLayerHeader->m_pCurrLumaDCDiff);
    VM_ASSERT(ret == ippStsNoErr);

    if(DCCOEF != 0)
    {
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
           if(quant  == 1)
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
        //if (DCSIGN == 1)
        //    DCCOEF = -DCCOEF;
        DCCOEF = (1 - (DCSIGN<<1))* DCCOEF;
    }

    CurrBlk->DC = (Ipp16s)DCCOEF;
    if(!bias)
    {
        Ipp32s DCStepSize = pContext->CurrDC->DCStepSize;
        pBlock->blkType =  DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num, m_pBlock,
                                            (Ipp16s)((1024 +(DCStepSize>>1))/DCStepSize),
                                            pContext->m_picLayerHeader->PTYPE);
    }
    else
    pBlock->blkType = DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num,
                        m_pBlock,0,pContext->m_picLayerHeader->PTYPE);
   if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        const Ipp8u* curr_scan = pContext->m_pSingleMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
        if(curr_scan==NULL)
            return VC1_FAIL;

        DecodeBlockACIntra_VC1(&pContext->m_bitstream,
                    m_pBlock, curr_scan,
                   pContext->m_picLayerHeader->m_pCurrIntraACDecSet,
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


    return VC1_OK;
}

VC1Status BLKLayer_Intra_Chroma(VC1Context* pContext, Ipp32s blk_num, Ipp32u bias, Ipp32u ACPRED)
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

    Ipp32u quant =  CurrDC->DoubleQuant>>1;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Intra_StartTime);

    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &DCCOEF,
                                     pContext->m_picLayerHeader->m_pCurrChromaDCDiff);
    VM_ASSERT(ret == ippStsNoErr);

    if(DCCOEF != 0)
    {
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
        //if (DCSIGN == 1)
        //    DCCOEF = -DCCOEF;
        DCCOEF = (1 - (DCSIGN<<1))* DCCOEF;
    }

    CurrBlk->DC = (Ipp16s)DCCOEF;
    if(!bias)
    {
        Ipp32s DCStepSize = pContext->CurrDC->DCStepSize;
        pBlock->blkType = DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num,
                        m_pBlock,(Ipp16s)((1024 +(DCStepSize>>1))/DCStepSize),
                        pContext->m_picLayerHeader->PTYPE);
    }
    else
    pBlock->blkType = DCPredictionTable[ACPRED](CurrBlk, DCPred, blk_num, m_pBlock,0,
                        pContext->m_picLayerHeader->PTYPE);

    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        const Ipp8u* curr_scan = pContext->m_pSingleMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
        if(curr_scan==NULL)
            return VC1_FAIL;

        DecodeBlockACIntra_VC1(&pContext->m_bitstream,
                     m_pBlock, curr_scan,
                     pContext->m_picLayerHeader->m_pCurrInterACDecSet,
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
    return VC1_OK;
}

VC1Status BLKLayer_Inter_Luma(VC1Context* pContext, Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    const Ipp8u* curr_scan = NULL;
    Ipp8u numCoef = 0;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader * picHeader = pContext->m_picLayerHeader;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Inter_StartTime);
    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        switch (pBlock->blkType)
        {
        case VC1_BLK_INTER8X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

                sMB->m_pSingleBlock[blk_num].numCoef = VC1_SBP_0;
                DecodeBlockInter8x8_VC1(&pContext->m_bitstream,
                            m_pBlock, curr_scan,
                            picHeader->m_pCurrInterACDecSet,
                            &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
            }
            break;
        case VC1_BLK_INTER8X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM == pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                 DecodeBlockInter8x4_VC1(&pContext->m_bitstream,
                                m_pBlock,
                                curr_scan, picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;
        case VC1_BLK_INTER4X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

                if (sMB->m_ubNumFirstCodedBlk < blk_num ||  picHeader->TTFRM ==
                    pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                   DecodeBlockInter4x8_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan,
                                picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;
        case VC1_BLK_INTER4X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

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
                if(curr_scan==NULL)
                    return VC1_FAIL;

                switch (pBlock->blkType)
                {
                case VC1_BLK_INTER8X8:
                    {
                       DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                                    curr_scan,  picHeader->m_pCurrInterACDecSet,
                                    &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
                    }
                    break;
                case VC1_BLK_INTER8X4:
                    {
                        DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                        curr_scan,  picHeader->m_pCurrInterACDecSet,
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
                                        curr_scan,picHeader->m_pCurrInterACDecSet,
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
    return VC1_OK;
}
VC1Status BLKLayer_Inter_Chroma(VC1Context* pContext, Ipp32s blk_num)
{
    Ipp16s*   m_pBlock  = pContext->m_pBlock + VC1_BlkStart[blk_num];
    VC1Block* pBlock    = &pContext->m_pCurrMB->m_pBlocks[blk_num];
    const Ipp8u* curr_scan = NULL;
    Ipp8u numCoef = 0;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader * picHeader = pContext->m_picLayerHeader;

    STATISTICS_START_TIME(m_timeStatistics->decoding_Inter_StartTime);

    if(pContext->m_pCurrMB->m_cbpBits & (1<<(5-blk_num)))
    {
        switch (pBlock->blkType)
        {
        case VC1_BLK_INTER8X8:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

                sMB->m_pSingleBlock[blk_num].numCoef = VC1_SBP_0;
                DecodeBlockInter8x8_VC1(&pContext->m_bitstream, m_pBlock,
                            curr_scan,picHeader->m_pCurrInterACDecSet,
                            &pContext->m_pSingleMB->EscInfo, VC1_SBP_0);
            }
            break;
        case VC1_BLK_INTER8X4:
            {
                curr_scan = sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

                if (sMB->m_ubNumFirstCodedBlk < blk_num || picHeader->TTFRM ==  pBlock->blkType)
                    numCoef = GetSubBlockPattern_8x4_4x8(pContext, blk_num);
                else
                    numCoef = sMB->m_pSingleBlock[blk_num].numCoef;

                DecodeBlockInter8x4_VC1(&pContext->m_bitstream, m_pBlock,
                                curr_scan,picHeader->m_pCurrInterACDecSet,
                                &pContext->m_pSingleMB->EscInfo, numCoef);
            }
            break;
        case VC1_BLK_INTER4X8:
            {
                curr_scan =sMB->ZigzagTable[VC1_BlockTable[pBlock->blkType]];
                if(curr_scan==NULL)
                    return VC1_FAIL;

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
                if(curr_scan==NULL)
                    return VC1_FAIL;

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
                if(curr_scan==NULL)
                    return VC1_FAIL;

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
                                    curr_scan,picHeader->m_pCurrInterACDecSet,
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
    return VC1_OK;
}


#endif //UMC_ENABLE_VC1_VIDEO_DECODER
