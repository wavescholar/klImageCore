/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Coded block pattern
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"


void AssignCodedBlockPattern(VC1MB * pMB,VC1SingletonMB* sMB)
{
    Ipp32s CBPCY = pMB->m_cbpBits;
    Ipp32s Count;

    for(Count = 0; Count < VC1_NUM_OF_BLOCKS; Count++)
    {
        VC1Block *pBlk = &pMB->m_pBlocks[Count];
        Ipp32u Coded = (0 != (CBPCY & (1 << (5 - Count))));

        sMB->m_pSingleBlock[Count].Coded = (Ipp8u)Coded;

        if (!Coded && (pBlk->blkType < VC1_BLK_INTRA_TOP))
            pBlk->blkType = VC1_BLK_INTER8X8;
        //if (!Coded && (pBlk->blkType & 0xF))
        //    pBlk->blkType = VC1_BLK_INTER8X8;

        pBlk->SBlkPattern = SubBlockPattern(pBlk,&sMB->m_pSingleBlock[Count]);

    }
}
//4.1.2.1    Coded Block Pattern
// ____________________
//|    |    |    |    |
//|    |    | T0 | T1 |
//|____|____|____|____|
//|    |    |    |    |
//|    | LT3| T2 | T3 |
//|___ |____|____|____|
//|    |    |    |    |
//| L0 | L1 | Y0 | Y1 |
//|___ |____|____|____|
//|    |    |    |    |
//| L2 | L3 | Y2 | Y3 |
//|____|____|____|____|
//////////////////////////
//pCBPpredAbove is an 1 dim array for prediction of CBP from above MB
//T3 and T4 for corresponding i th MB are located in
//pCBPpredAbove[2i]   = T3
//pCBPpredAbove[2i+1] = T4
//pCBPpredLeft is an 1 dim array for prediction of CBP from left MB
//2 elements are L2 and L4
//pCBPpredLeft[0] = L2
//pCBPpredLeft[1] = L4
//LT4 could be found in pCBPpredAboveLeft

//For future prediction of the next mbcbp after prediction left
//predictors will be replaced with calculated values
//T4 moved to LT4 and top predictors will be replaced with calculated
//values as well for calculation mbcbp in the same position mb in next row
Ipp32s CalculateCBP(VC1MB* pCurrMB, Ipp32u decoded_cbpy, Ipp32s width)
{
    Ipp32s predicted_Y0;
    Ipp32s predicted_Y1;
    Ipp32s predicted_Y2;
    Ipp32s predicted_Y3;

    Ipp32s calculated_cbpy;

    Ipp32s LT3;
    Ipp32s T2 ;
    Ipp32s T3 ;
    Ipp32s L1 ;
    Ipp32s L3 ;

    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;


    if(VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag) && VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        LT3 = ((pCurrMB - width-1)->m_cbpBits & 4) >> 2;
    }
    else
    {
        LT3 = 0;
    }

    if(VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
    {
        T2  = ((pCurrMB - width)->m_cbpBits & 8) >> 3;
        T3  = ((pCurrMB - width)->m_cbpBits & 4) >> 2;
    }
    else
    {
        T2  = 0;
        T3  = 0;
    }

    if(VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        L1  = ((pCurrMB - 1)->m_cbpBits &16) >> 4;
        L3  = ((pCurrMB - 1)->m_cbpBits & 4) >> 2;
    }
    else
    {
        L1  = 0;
        L3  = 0;
    }

    if(LT3 == T2)
        predicted_Y0 = L1;
    else
        predicted_Y0 = T2;

    predicted_Y0 ^= ((decoded_cbpy >> 5) & 0x01);

    if(T2 == T3)
        predicted_Y1 = predicted_Y0;
    else
        predicted_Y1 = T3;

    predicted_Y1 ^= ((decoded_cbpy >> 4) & 0x01);

    if(L1 == predicted_Y0)
        predicted_Y2 = L3;
    else
        predicted_Y2 = predicted_Y0;

    predicted_Y2 ^= ((decoded_cbpy >> 3) & 0x01);

    if(predicted_Y0 == predicted_Y1)
        predicted_Y3 = predicted_Y2;
    else
        predicted_Y3 = predicted_Y1;

    predicted_Y3 ^= ((decoded_cbpy >> 2) & 0x01);

    calculated_cbpy = (predicted_Y0 << 5) |
                      (predicted_Y1 << 4) |
                      (predicted_Y2 << 3) |
                      (predicted_Y3 << 2) | (decoded_cbpy&3);


    return calculated_cbpy;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
