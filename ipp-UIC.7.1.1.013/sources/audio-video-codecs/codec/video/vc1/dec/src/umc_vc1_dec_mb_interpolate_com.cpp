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
#include "umc_vc1_common_blk_order_tbl.h"


static const Ipp8u esbp_lut[] = {1,2,2,2,4,4,4,8};

void DecodeTransformInfo(VC1Context* pContext)
{
    Ipp32u i;
    if(pContext->m_seqLayerHeader->VSTRANSFORM)
    {
        if (pContext->m_picLayerHeader->TTMBF == 0 &&
            (pContext->m_pCurrMB->mbType != VC1_MB_INTRA) )
        {
            GetTTMB(pContext);
        }
        else
        {
            if (!pContext->m_pCurrMB->mbType == VC1_MB_INTRA)
            {
                for (i=0;i<VC1_NUM_OF_BLOCKS;i++)
                {
                    if (!VC1_IS_BLKINTRA(pContext->m_pCurrMB->m_pBlocks[i].blkType))
                        pContext->m_pCurrMB->m_pBlocks[i].blkType = (Ipp8u)pContext->m_picLayerHeader->TTFRM;
                }
            }
            else
                for (i=0;i<VC1_NUM_OF_BLOCKS;i++)
                    pContext->m_pCurrMB->m_pBlocks[i].blkType = VC1_BLK_INTRA;

        }
    }
    //else
    //{
    //    for (i=0;i<VC1_NUM_OF_BLOCKS;i++)
    //        pContext->m_pCurrMB->m_pBlocks[i].blkType = (Ipp8u)pContext->m_picLayerHeader->TTFRM;
    //}
}

VC1Status GetTTMB(VC1Context* pContext)
{
    IppStatus ret;
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s eSBP;
    Ipp8u Count, Limit, FirstBlock = 0;

    while ( (FirstBlock <  VC1_NUM_OF_BLOCKS)             &&
        ((0 == ((1 << (5-FirstBlock)) & pMB->m_cbpBits)) ||
        (pMB->m_pBlocks[FirstBlock].blkType == VC1_BLK_INTRA) )
        )
    {
        FirstBlock++;
    }

    if(FirstBlock == VC1_NUM_OF_BLOCKS)
    {
        return VC1_OK;
    }

    ret = ippiDecodeHuffmanOne_1u32s(  &pContext->m_bitstream.pBitstream,
        &pContext->m_bitstream.bitOffset,
        &eSBP,
        pContext->m_picLayerHeader->m_pCurrTTMBtbl);
    VM_ASSERT(ret == ippStsNoErr);

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_TT,VM_STRING("TTMB = %d\n"), eSBP);

    Limit = VC1_NUM_OF_BLOCKS;

    if(eSBP < VC1_SBP_8X8_MB)
    {
        Limit = FirstBlock + 1;
    }
    else
    {
        eSBP -= VC1_SBP_8X8_MB;
    }

    //printf("eSBP = %d\n",eSBP);
    //printf("offset = %d\n", pContext->m_bitOffset);

    pContext->m_pSingleMB->m_ubNumFirstCodedBlk = FirstBlock;

    pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = 0;

    switch(eSBP)
    {
    case VC1_SBP_8X8_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_0;
        break;

    case VC1_SBP_8X4_BOTTOM_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_1;
        break;

    case VC1_SBP_8X4_TOP_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_0;
        break;

    case VC1_SBP_8X4_BOTH_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_0|VC1_SBP_1;
        break;

    case VC1_SBP_4X8_RIGHT_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_1;
        break;

    case VC1_SBP_4X8_LEFT_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_0;
        break;

    case VC1_SBP_4X8_BOTH_BLK:
        pContext->m_pSingleMB->m_pSingleBlock[FirstBlock].numCoef = VC1_SBP_0|VC1_SBP_1;
        break;

    case VC1_SBP_4X4_BLK:
        //nothing to do
        break;

    default:
        return VC1_FAIL;
    }

    for(Count = FirstBlock; Count < Limit; Count++)
    {
        if(!(pMB->m_pBlocks[Count].blkType == VC1_BLK_INTRA))
        {
            VM_ASSERT(eSBP<8);
            pMB->m_pBlocks[Count].blkType = VC1_LUT_SET(eSBP,esbp_lut);
        }
    }

    for( ; Count < VC1_NUM_OF_BLOCKS; Count++)
    {
        if(!(pMB->m_pBlocks[Count].blkType == VC1_BLK_INTRA))
        {
            pMB->m_pBlocks[Count].blkType = VC1_BLK_INTER;
        }
    }

    return VC1_OK;
}

void CalculateIntraFlag(VC1Context* pContext)
{
    Ipp8u i;
    pContext->m_pCurrMB->IntraFlag=0;

    for (i = 0; i < VC1_NUM_OF_BLOCKS; i++)
    {
        if(pContext->m_pCurrMB->m_pBlocks[i].blkType & VC1_BLK_INTRA)
        {
            pContext->m_pCurrMB->IntraFlag=(1<<i)|pContext->m_pCurrMB->IntraFlag;
        }
    }
}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
