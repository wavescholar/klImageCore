/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, MB layer in I picture for simple\main profiles
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_zigzag_tbl.h"

#include "umc_vc1_dec_time_statistics.h"
#include "umc_vc1_common_zigzag_tbl.h"


typedef void (*IntraPrediction)(VC1Context* pContext);
static const IntraPrediction IntraPredictionTable[] =
{
        (IntraPrediction)(GetIntraDCPredictors),
        (IntraPrediction)(GetIntraScaleDCPredictors),
        (IntraPrediction)(GetIntraScaleDCPredictors)
};

//Figure 15: Syntax diagram for macroblock layer bitstream in
//Progressive-coded I picture
VC1Status MBLayer_ProgressiveIpicture(VC1Context* pContext)
{
    Ipp32s i;
    Ipp32s CBPCY;//decoded_cbpy
    IppStatus ret;
    VC1Status vc1Res = VC1_OK;
    Ipp32u ACPRED = 0;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    pCurrMB->bias = 0;

    //pContext->m_pSingleMB->m_currMBXpos++;

    pCurrMB->mbType = VC1_MB_INTRA;

    if (pContext->m_picLayerHeader->PQUANT >= 9)
    {
        pCurrMB->Overlap = (Ipp8u)pContext->m_seqLayerHeader->OVERLAP;
        pCurrMB->bias = 128 * pCurrMB->Overlap;
    }
    else
        pCurrMB->Overlap =0;

    //CBPCY is a variable-length field present in both I picture and P
    //picture macroblock layers.
    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &CBPCY,
                                     pContext->m_vlcTbl->m_pCBPCY_Ipic);

    VM_ASSERT(ret == ippStsNoErr);

    pCurrMB->LeftTopRightPositionFlag = CalculateLeftTopRightPositionFlag(sMB);


    pCurrMB->m_cbpBits = CalculateCBP(pCurrMB, CBPCY, sMB->widthMB);

    //3.2.2.4
    //The ACPRED field is present in all I picture macroblocks and in 1MV
    //Intra macroblocks in P pictures (see section 4.4.5.1 for a description
    //of the macroblock types). This is a 1-bit field that specifies whether
    //the blocks were coded using AC prediction. ACPRED = 0 indicates that
    //AC prediction is not used. ACPRED = 1 indicates that AC prediction is
    //used.  See section 4.1.2.2 for a description of the ACPRED field in I
    //pictures and section 4.4.6.1 for a description of the ACPRED field in
    //P pictures.
    VC1_GET_BITS(1, ACPRED);

    memset(pContext->m_pBlock, 0, sizeof(Ipp16s)*8*8*6);

    pContext->CurrDC->DoubleQuant = (Ipp8u)(2*pContext->m_picLayerHeader->PQUANT +
                                                pContext->m_picLayerHeader->HALFQP);

    pContext->CurrDC->DCStepSize = GetDCStepSize(pContext->m_picLayerHeader->PQUANT);


    //all bloks are INTRA
    pCurrMB->IntraFlag=0x3F;

      //Y
    pCurrMB->currYPitch = sMB->currYPitch;
    pCurrMB->currYPlane = sMB->currYPlane + pCurrMB->currYPitch * (sMB->m_currMBYpos << 4)//*VC1_PIXEL_IN_LUMA
                                          + (sMB->m_currMBXpos << 4); //*VC1_PIXEL_IN_LUMA;

    //U
    pCurrMB->currUPitch = sMB->currUPitch;
    pCurrMB->currUPlane = sMB->currUPlane   + pCurrMB->currUPitch*(sMB->m_currMBYpos << 3) // * VC1_PIXEL_IN_CHROMA
                                            + (sMB->m_currMBXpos << 3); //* VC1_PIXEL_IN_CHROMA;

    //V
    pCurrMB->currVPitch = sMB->currVPitch;
    pCurrMB->currVPlane = sMB->currVPlane + pCurrMB->currVPitch*(sMB->m_currMBYpos << 3) // * VC1_PIXEL_IN_CHROMA
                                          + (sMB->m_currMBXpos << 3); //* VC1_PIXEL_IN_CHROMA;

    IntraPredictionTable[pContext->m_seqLayerHeader->DQUANT](pContext);
    sMB->ZigzagTable = ZigZagTables_I_luma[ACPRED];

    for(i = 0; i < VC1_NUM_OF_LUMA; i++)
    {
        //all MB intra in I picture
        vc1Res = BLKLayer_Intra_Luma(pContext, i, pCurrMB->bias, ACPRED);
        if(vc1Res != VC1_OK)
        {
            VM_ASSERT(0);
            break;
        }
    }

    sMB->ZigzagTable = ZigZagTables_I_chroma[ACPRED];

    for(i = VC1_NUM_OF_LUMA; i < VC1_NUM_OF_BLOCKS; i++)
    {
        //all MB intra in I picture
        vc1Res = BLKLayer_Intra_Chroma(pContext, i, pCurrMB->bias, ACPRED);
        if(vc1Res != VC1_OK)
        {
            VM_ASSERT(0);
            break;
        }
    }


    return vc1Res;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
