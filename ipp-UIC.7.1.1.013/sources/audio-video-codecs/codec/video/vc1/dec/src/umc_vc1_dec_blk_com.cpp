/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, block layer, common file
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_blk_order_tbl.h"
#include "umc_vc1_dec_time_statistics.h"
#include "assert.h"
#include "umc_vc1_dec_exception.h"

using namespace UMC;
using namespace UMC::VC1Exceptions;


inline Ipp32s DecodeSymbol(VC1Bitstream* pBitstream,
                           Ipp16s* run,
                           Ipp16s* level,
                           const VC1ACDecodeSet * decodeSet,
                           VC1EscInfo* EscInfo)
{
    IppStatus ret;
    Ipp32s sign = 0;
    Ipp32s code = 0;
    Ipp32s escape_mode = 0;
    Ipp32s ESCLR = 0;

    Ipp32s tmp_run = 0;
    Ipp32s tmp_level = 0;
    Ipp32s last = 0;

    ret = ippiDecodeHuffmanOne_1u32s (&pBitstream->pBitstream,
                                      &pBitstream->bitOffset,
                                      &code, decodeSet->pRLTable);
#ifdef VC1_VLD_CHECK
    if (ret != ippStsNoErr)
        throw vc1_exception(vld);
#endif



    if(code != IPPVC_ESCAPE)
    {
        tmp_run      = (code & 0X0000FF00)>>8;
        tmp_level    = code & 0X000000FF;
        last = code >> 16;

        VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, sign);

        tmp_level    = (1-(sign<<1))*tmp_level;
    }
    else
    {
        VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, escape_mode);

        if(escape_mode == 1)
        {

            ret = ippiDecodeHuffmanOne_1u32s ((&pBitstream->pBitstream), (&pBitstream->bitOffset), &code,
                                              decodeSet->pRLTable);

#ifdef VC1_VLD_CHECK
            if (ret != ippStsNoErr)
                throw vc1_exception(vld);
#endif


            tmp_run      = (code & 0X0000FF00)>>8;
            tmp_level    = code & 0X000000FF;
            last = code>>16;

            if (last)
                tmp_level = tmp_level + decodeSet->pDeltaLevelLast1[tmp_run];
            else
                tmp_level = tmp_level + decodeSet->pDeltaLevelLast0[tmp_run];

            VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, sign);
            tmp_level = (1-(sign<<1))*tmp_level;
        }
        else
        {
            VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, escape_mode);
            if(escape_mode == 1)
            {

                ret = ippiDecodeHuffmanOne_1u32s ((&pBitstream->pBitstream), (&pBitstream->bitOffset),
                                                    &code, decodeSet->pRLTable);

#ifdef VC1_VLD_CHECK
                if (ret != ippStsNoErr)
                    throw vc1_exception(vld);
#endif

                tmp_run      = (code & 0X0000FF00)>>8;
                tmp_level    = code & 0X000000FF;
                last = (code&0x00FF0000)>>16;

                if (last)
                    tmp_run = tmp_run + decodeSet->pDeltaRunLast1[tmp_level] + 1;
                else
                    tmp_run = tmp_run + decodeSet->pDeltaRunLast0[tmp_level] + 1;

                VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, sign);
                tmp_level = (1-(sign<<1))*tmp_level;
            }
            else
            {
                VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, ESCLR);

                last = ESCLR;

                if (EscInfo->levelSize + EscInfo->runSize == 0)
                {
                    if(EscInfo->bEscapeMode3Tbl == VC1_ESCAPEMODE3_Conservative)
                    {
                        VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 3, EscInfo->levelSize);
                        if(EscInfo->levelSize == 0)
                        {
                            VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 2, EscInfo->levelSize);
                            EscInfo->levelSize += 8;
                        }
                    }
                    else
                    {
                        Ipp32s bit_count = 1;
                        VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, EscInfo->levelSize);
                        while((EscInfo->levelSize == 0) && (bit_count < 6))
                        {
                            VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, EscInfo->levelSize);
                            bit_count++;
                        }

                        if(bit_count == 6 && EscInfo->levelSize == 0)
                            bit_count++;

                        EscInfo->levelSize = bit_count + 1;
                    }

                    VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 2, EscInfo->runSize);
                    EscInfo->runSize = 3 + EscInfo->runSize;
                }

                VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), EscInfo->runSize, tmp_run);

                VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), 1, sign);

                VC1BitstreamParser::GetNBits((pBitstream->pBitstream), (pBitstream->bitOffset), EscInfo->levelSize,tmp_level);
                tmp_level = (1-(sign<<1))*tmp_level;
            }
        }
    }
    (*run) = (Ipp16s)tmp_run;
    (*level) = (Ipp16s)tmp_level;
    return last;
}


IppStatus DecodeBlockACIntra_VC1(VC1Bitstream* pBitstream, Ipp16s* pDst,
                 const  Ipp8u* pZigzagTbl,  const VC1ACDecodeSet * pDecodeSet,
                 VC1EscInfo* pEscInfo)
{
    Ipp32s last_flag = 0;
    Ipp16s run = 0, level = 0;
    Ipp32s curr_position = 1;
    IppStatus sts = ippStsNoErr;

#ifdef VC1_VLD_CHECK
    if((!pBitstream)||(!pDst) || (!pDecodeSet) || (!pZigzagTbl) || (!pEscInfo) || (!pBitstream->pBitstream))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more

    if((pBitstream->bitOffset < 0) || (pBitstream->bitOffset>31))
        throw vc1_exception(internal_pipeline_error);// Global problem, no need to decode more
#endif

    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            throw vc1_exception(vld);
        }
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position]] = pDst[pZigzagTbl[curr_position]] + level;

        curr_position++;
    } while (last_flag == 0);

    return sts;
}


IppStatus DecodeBlockInter8x8_VC1(VC1Bitstream* pBitstream, Ipp16s* pDst,
                      const  Ipp8u* pZigzagTbl,  const VC1ACDecodeSet * pDecodeSet,
                      VC1EscInfo* pEscInfo, Ipp32s subBlockPattern)
{
    Ipp32s last_flag = 0;
    Ipp16s run = 0, level = 0;
    Ipp32s curr_position = 0;
    IppStatus sts = ippStsNoErr;

#ifdef VC1_VLD_CHECK
    if((!pBitstream)||(!pDst) || (!pDecodeSet) || (!pZigzagTbl) || (!pEscInfo) || (!pBitstream->pBitstream))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more

    if((pBitstream->bitOffset < 0) || (pBitstream->bitOffset>31))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more
#endif

    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            throw vc1_exception(vld);
        }
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position]] = pDst[pZigzagTbl[curr_position]] + level;

        curr_position++;
    } while (last_flag == 0);

    return sts;
}

IppStatus DecodeBlockInter4x8_VC1(VC1Bitstream* pBitstream, Ipp16s* pDst,
                      const  Ipp8u* pZigzagTbl,  const VC1ACDecodeSet * pDecodeSet,
                      VC1EscInfo* pEscInfo, Ipp32s subBlockPattern)
{
    Ipp32s last_flag = 0;
    Ipp16s run = 0, level = 0;
    Ipp32s curr_position = 0;
    IppStatus sts = ippStsNoErr;

#ifdef VC1_VLD_CHECK
    if((!pBitstream)||(!pDst) || (!pDecodeSet) || (!pZigzagTbl) || (!pEscInfo) || (!pBitstream->pBitstream))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more

    if((pBitstream->bitOffset < 0) || (pBitstream->bitOffset>31))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more
#endif

    if(subBlockPattern & VC1_SBP_0)
        do
        {
            last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            // we can decode next MB
            throw vc1_exception(vld);
        }
#endif

            curr_position = curr_position + run;
            pDst[pZigzagTbl[curr_position]] = pDst[pZigzagTbl[curr_position]] + level;

            curr_position++;
        } while (last_flag == 0);

    curr_position = 0;
    if(subBlockPattern & VC1_SBP_1)
        do
        {
            last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
            if(curr_position > 64)
                throw vc1_exception(vld);
#endif

            curr_position = curr_position + run;
            pDst[pZigzagTbl[curr_position + 32]] = pDst[pZigzagTbl[curr_position + 32]] + level;

            curr_position++;
        } while (last_flag == 0);

        return sts;
}

IppStatus DecodeBlockInter8x4_VC1(VC1Bitstream* pBitstream, Ipp16s* pDst,
                      const  Ipp8u* pZigzagTbl,  const VC1ACDecodeSet * pDecodeSet,
                      VC1EscInfo* pEscInfo, Ipp32s subBlockPattern)
{
    Ipp32s last_flag = 0;
    Ipp16s run = 0, level = 0;
    Ipp32s curr_position = 0;
    IppStatus sts = ippStsNoErr;

#ifdef VC1_VLD_CHECK
    if((!pBitstream)||(!pDst) || (!pDecodeSet) || (!pZigzagTbl) || (!pEscInfo) || (!pBitstream->pBitstream))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more

    if((pBitstream->bitOffset < 0) || (pBitstream->bitOffset>31))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more
#endif

    if(subBlockPattern & VC1_SBP_0)
        do
        {
            last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            throw vc1_exception(vld);
        }
#endif
            curr_position = curr_position + run;
            pDst[pZigzagTbl[curr_position]] = pDst[pZigzagTbl[curr_position]] + level;

            curr_position++;
        } while (last_flag == 0);


    curr_position = 0;
    if(subBlockPattern & VC1_SBP_1)
        do
        {
            last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            throw vc1_exception(vld);
        }
#endif


            curr_position = curr_position + run;
            pDst[pZigzagTbl[curr_position + 32]] = pDst[pZigzagTbl[curr_position + 32]] + level;

            curr_position++;
        } while (last_flag == 0);

        return sts;
}

IppStatus DecodeBlockInter4x4_VC1(VC1Bitstream* pBitstream, Ipp16s* pDst,
                      const  Ipp8u* pZigzagTbl,  const VC1ACDecodeSet * pDecodeSet,
                      VC1EscInfo* pEscInfo, Ipp32s subBlockPattern)
{
    Ipp32s last_flag = 0;
    Ipp16s run = 0, level = 0;
    Ipp32s curr_position = 0;
    IppStatus sts = ippStsNoErr;

#ifdef VC1_VLD_CHECK
    if((!pBitstream)||(!pDst) || (!pDecodeSet) || (!pZigzagTbl) || (!pEscInfo) || (!pBitstream->pBitstream))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more

    if((pBitstream->bitOffset < 0) || (pBitstream->bitOffset>31))
        throw vc1_exception(internal_pipeline_error); // Global problem, no need to decode more
#endif

    if(subBlockPattern & VC1_SBP_0)
    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
        {
            throw vc1_exception(vld);
        }
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position]] = pDst[pZigzagTbl[curr_position]] + level;

        curr_position++;
    } while (last_flag == 0);


    curr_position = 0;

    if(subBlockPattern & VC1_SBP_1)
    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
            throw vc1_exception(vld);
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position + 16]] = pDst[pZigzagTbl[curr_position + 16]] + level;

        curr_position++;
    } while (last_flag == 0);


    curr_position = 0;

    if(subBlockPattern & VC1_SBP_2)
    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
            throw vc1_exception(vld);
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position + 32]] = pDst[pZigzagTbl[curr_position + 32]] + level;

        curr_position++;
    } while (last_flag == 0);


    curr_position = 0;
    if(subBlockPattern & VC1_SBP_3)
    do
    {
        last_flag = DecodeSymbol(pBitstream, &run, &level, pDecodeSet, pEscInfo);


#ifdef VC1_VLD_CHECK
        if(curr_position > 64)
            throw vc1_exception(vld);
#endif

        curr_position = curr_position + run;
        pDst[pZigzagTbl[curr_position + 48]] = pDst[pZigzagTbl[curr_position + 48]] + level;

        curr_position++;
    } while (last_flag == 0);

    return sts;
}

Ipp8u GetSubBlockPattern_8x4_4x8(VC1Context* pContext,Ipp32s blk_num)
{
    Ipp32s   Value;

    //Table 67: 8x4 and 4x8 Transform sub-block pattern code-table for Progressive pictures
    //              8x4 Sub-block pattern       4x8 Sub-block pattern
    //SUBBLKPAT VLC     Top Bottom              Left Right
    //10                      X                        X
    //0                  X    X                   X    X
    //11                 X                        X

    VC1_GET_BITS(1, Value);
    if(0 != Value)
    {

        VC1_GET_BITS(1, Value);
        Value++;

        if(1 == Value)//for codeword "10b"
            Value = VC1_SBP_1;
        else //for codeword "11b"
            Value = VC1_SBP_0;
    }
    else
    {
        Value = VC1_SBP_0|VC1_SBP_1;
    }


    pContext->m_pSingleMB->m_pSingleBlock[blk_num].numCoef = (Ipp8u)Value;
    return ((Ipp8u)Value);
}

Ipp8u GetSubBlockPattern_4x4(VC1Context* pContext,Ipp32s blk_num)
{
    IppStatus ret;
    Ipp32s   Value;

    ret = ippiDecodeHuffmanOne_1u32s(   &pContext->m_bitstream.pBitstream,
        &pContext->m_bitstream.bitOffset,
        &Value,
        pContext->m_picLayerHeader->m_pCurrSBPtbl);
    //VM_ASSERT(ret == ippStsNoErr);

#ifdef VC1_VLD_CHECK
    if (ret != ippStsNoErr)
        throw vc1_exception(vld);
#endif

    pContext->m_pSingleMB->m_pSingleBlock[blk_num].numCoef = (Ipp8u)Value;


    return ((Ipp8u)Value);
}

Ipp8u GetTTBLK(VC1Context* pContext, Ipp32s blk_num)
{
    IppStatus ret;
    Ipp32s Value;
    VC1MB *pMB = pContext->m_pCurrMB;
    VC1Block *pBlock = &pMB->m_pBlocks[blk_num];
    Ipp8u numCoef = 0;

    ret = ippiDecodeHuffmanOne_1u32s(   &pContext->m_bitstream.pBitstream,
                                        &pContext->m_bitstream.bitOffset,
                                        &Value,
                                        pContext->m_picLayerHeader->m_pCurrTTBLKtbl);
    //VM_ASSERT(ret == ippStsNoErr);

#ifdef VC1_VLD_CHECK
    if (ret != ippStsNoErr)
        throw vc1_exception(vld);
#endif

    switch(Value)
    {
        case VC1_SBP_8X8_BLK:
            numCoef = VC1_SBP_0;
            pBlock->blkType  = VC1_BLK_INTER8X8;
        break;

        case VC1_SBP_8X4_BOTTOM_BLK:
            numCoef = VC1_SBP_1;
            pBlock->blkType = VC1_BLK_INTER8X4;
        break;

        case VC1_SBP_8X4_TOP_BLK:
            numCoef = VC1_SBP_0;
            pBlock->blkType = VC1_BLK_INTER8X4;
        break;

        case VC1_SBP_8X4_BOTH_BLK:
            numCoef = VC1_SBP_0|VC1_SBP_1;
            pBlock->blkType = VC1_BLK_INTER8X4;
        break;

        case VC1_SBP_4X8_RIGHT_BLK:
            numCoef = VC1_SBP_1;
            pBlock->blkType = VC1_BLK_INTER4X8;
        break;

        case VC1_SBP_4X8_LEFT_BLK:
            numCoef = VC1_SBP_0;
            pBlock->blkType = VC1_BLK_INTER4X8;
        break;

        case VC1_SBP_4X8_BOTH_BLK:
            numCoef = VC1_SBP_0|VC1_SBP_1;
            pBlock->blkType = VC1_BLK_INTER4X8;
        break;

        case VC1_SBP_4X4_BLK:
            numCoef = VC1_SBP_0|VC1_SBP_1|VC1_SBP_2|VC1_SBP_3;
            pBlock->blkType = VC1_BLK_INTER4X4;
        break;

        default:
            VM_ASSERT(0);
    }

    pContext->m_pSingleMB->m_pSingleBlock[blk_num].numCoef = numCoef;
    return numCoef;
}

Ipp32u GetDCStepSize(Ipp32s MQUANT)
{
    Ipp32u DCStepSize;

    if(MQUANT < 4)
        DCStepSize = 1 << MQUANT;
    else
        DCStepSize = MQUANT/2 + 6;

    return DCStepSize;
}


#endif //UMC_ENABLE_VC1_VIDEO_DECODER
