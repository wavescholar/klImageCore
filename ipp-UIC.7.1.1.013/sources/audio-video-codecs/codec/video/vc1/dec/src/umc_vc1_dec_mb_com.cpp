/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, MB Layer in P picture for advanced profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_blk_order_tbl.h"
#include "umc_vc1_dec_run_level_tbl.h"
#include "umc_vc1_dec_time_statistics.h"


inline
static Ipp16s GetScaleDC(Ipp16s DC, Ipp32u QntCurr, Ipp32u QntPred,
                         Ipp32s CurrDCStepSize, Ipp32s PredDCStepSize)
{
    /* Remove HalfStep */
    QntCurr >>= 1;
    QntPred >>= 1;

    if (QntCurr != QntPred)
        DC = (Ipp16s)((DC * VC1_DQScaleTbl[CurrDCStepSize] * PredDCStepSize + 0x20000)>>18);

    return DC;
}
void GetIntraScaleDCPredictors(VC1Context* pContext)
{
    VC1DCMBParam* CurrDC = pContext->CurrDC;
    VC1DCPredictors DCPred;
    Ipp32u QurrQuant = CurrDC->DoubleQuant;
    Ipp32s QurrDCStep = CurrDC->DCStepSize;

    Ipp32u LeftTopFlag = pContext->m_pCurrMB->LeftTopRightPositionFlag & 0xFF0;
    Ipp32s width = pContext->m_seqLayerHeader->widthMB;

    VC1DCMBParam* pA = NULL, *pB = NULL, *pC = NULL;

    memset(&DCPred.BlkPattern, 0, 6);

    DCPred.DoubleQuant[2] = QurrQuant;

    DCPred.ACTOP[0]  = CurrDC->DCBlkPred[0].ACTOP;
    DCPred.ACLEFT[0] = CurrDC->DCBlkPred[0].ACLEFT;

    DCPred.ACTOP[1]  = CurrDC->DCBlkPred[1].ACTOP;
    DCPred.ACLEFT[1] = CurrDC->DCBlkPred[1].ACLEFT;

    DCPred.ACTOP[2]  = CurrDC->DCBlkPred[2].ACTOP;
    DCPred.ACLEFT[2] = CurrDC->DCBlkPred[2].ACLEFT;

    switch(LeftTopFlag)
    {
    case (VC1_COMMON_MB):
        {
            pA = CurrDC - width;
            pB = CurrDC - width - 1;
            pC = CurrDC - 1;

            DCPred.BlkPattern[0] = 7;
            DCPred.BlkPattern[1] = 7;
            DCPred.BlkPattern[2] = 7;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 7;
            DCPred.BlkPattern[5] = 7;


            DCPred.DoubleQuant[0] = pA->DoubleQuant;
            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            //Luma
            DCPred.DC[6] = GetScaleDC(pA->DCBlkPred[2].DC,QurrQuant,
                                      pA->DoubleQuant, QurrDCStep,
                                      pA->DCStepSize);
            DCPred.ACTOP[6] = pA->DCBlkPred[2].ACTOP;
            DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

            DCPred.DC[7] = GetScaleDC(pA->DCBlkPred[3].DC,QurrQuant,
                                      pA->DoubleQuant, QurrDCStep,
                                      pA->DCStepSize);
            DCPred.ACTOP[7]  = pA->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;


            DCPred.DC[3] = GetScaleDC(pB->DCBlkPred[3].DC, QurrQuant,
                                      pB->DoubleQuant, QurrDCStep,
                                      pB->DCStepSize);
            DCPred.ACTOP[3] = pB->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[3] = pB->DCBlkPred[3].ACLEFT;

            DCPred.DC[8] = GetScaleDC(pC->DCBlkPred[1].DC,QurrQuant,
                                      pC->DoubleQuant, QurrDCStep,
                                      pC->DCStepSize);
            DCPred.ACTOP[8] = pC->DCBlkPred[1].ACTOP;
            DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

            DCPred.DC[9] = GetScaleDC(pC->DCBlkPred[3].DC,QurrQuant,
                                      pC->DoubleQuant, QurrDCStep,
                                      pC->DCStepSize);
            DCPred.ACTOP[9] = pC->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[10] = GetScaleDC(pA->DCBlkPred[4].DC,QurrQuant,
                                       pA->DoubleQuant, QurrDCStep,
                                       pA->DCStepSize);
            DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

            DCPred.DC[4] = GetScaleDC(pB->DCBlkPred[4].DC,QurrQuant,
                                      pB->DoubleQuant, QurrDCStep,
                                      pB->DCStepSize);
            DCPred.ACTOP[4] = pB->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[4] = pB->DCBlkPred[4].ACLEFT;

            DCPred.DC[11] = GetScaleDC(pC->DCBlkPred[4].DC,QurrQuant,
                                       pC->DoubleQuant, QurrDCStep,
                                       pC->DCStepSize);
            DCPred.ACTOP[11] = pC->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[12] = GetScaleDC(pA->DCBlkPred[5].DC,QurrQuant,
                                      pA->DoubleQuant, QurrDCStep,
                                      pA->DCStepSize);
            DCPred.ACTOP[12] = pA->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;

            DCPred.DC[5] = GetScaleDC(pB->DCBlkPred[5].DC,QurrQuant,
                                      pB->DoubleQuant, QurrDCStep,
                                      pB->DCStepSize);
            DCPred.ACTOP[5] = pB->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[5] = pB->DCBlkPred[5].ACLEFT;

            DCPred.DC[13] = GetScaleDC(pC->DCBlkPred[5].DC,QurrQuant,
                                       pC->DoubleQuant, QurrDCStep,
                                       pC->DCStepSize);
            DCPred.ACTOP[13] = pC->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
        }
        break;
    case(VC1_TOP_MB):
        {
            pC = CurrDC - 1;
            DCPred.BlkPattern[0] = 1;
            DCPred.BlkPattern[1] = 1;
            DCPred.BlkPattern[2] = 7;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 1;
            DCPred.BlkPattern[5] = 1;

            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            //Luma
            DCPred.DC[8] = GetScaleDC(pC->DCBlkPred[1].DC,QurrQuant,
                                      pC->DoubleQuant, QurrDCStep,
                                      pC->DCStepSize);
            DCPred.ACTOP[8] = pC->DCBlkPred[1].ACTOP;
            DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

            DCPred.DC[9] = GetScaleDC(pC->DCBlkPred[3].DC,QurrQuant,
                                      pC->DoubleQuant, QurrDCStep,
                                      pC->DCStepSize);
            DCPred.ACTOP[9] = pC->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[11] = GetScaleDC(pC->DCBlkPred[4].DC,QurrQuant,
                                       pC->DoubleQuant, QurrDCStep,
                                       pC->DCStepSize);
            DCPred.ACTOP[11] = pC->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[13] = GetScaleDC(pC->DCBlkPred[5].DC,QurrQuant,
                                      pC->DoubleQuant, QurrDCStep,
                                      pC->DCStepSize);
            DCPred.ACTOP[13] = pC->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
        }
        break;
    case (VC1_LEFT_MB):
        {
            pA = CurrDC - width;
            DCPred.BlkPattern[0] = 4;
            DCPred.BlkPattern[1] = 7;
            DCPred.BlkPattern[2] = 4;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 4;
            DCPred.BlkPattern[5] = 4;


            DCPred.DoubleQuant[0] = pA->DoubleQuant;

            //Luma
            DCPred.DC[6] = GetScaleDC(pA->DCBlkPred[2].DC,QurrQuant,
                                      pA->DoubleQuant, QurrDCStep,
                                      pA->DCStepSize);
            DCPred.ACTOP[6] = pA->DCBlkPred[2].ACTOP;
            DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

            DCPred.DC[7] = GetScaleDC(pA->DCBlkPred[3].DC,QurrQuant,
                                        pA->DoubleQuant, QurrDCStep,
                                        pA->DCStepSize);
            DCPred.ACTOP[7] = pA->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[10] = GetScaleDC(pA->DCBlkPred[4].DC,QurrQuant,
                                        pA->DoubleQuant, QurrDCStep,
                                        pA->DCStepSize);
            DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[12] = GetScaleDC(pA->DCBlkPred[5].DC,QurrQuant,
                                        pA->DoubleQuant, QurrDCStep,
                                        pA->DCStepSize);
            DCPred.ACTOP[12] =  pA->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;
        }
        break;
    case (VC1_TOP_LEFT_MB):
        {
            DCPred.BlkPattern[0] = 0;
            DCPred.BlkPattern[1] = 1;
            DCPred.BlkPattern[2] = 4;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 0;
            DCPred.BlkPattern[5] = 0;
        }
        break;
    }

    memcpy(&pContext->DCPred, &DCPred,sizeof(VC1DCPredictors));
}

void GetIntraDCPredictors(VC1Context* pContext)
{
    VC1DCMBParam* CurrDC = pContext->CurrDC;
    VC1DCPredictors DCPred;

    Ipp32u LeftTopFlag = pContext->m_pCurrMB->LeftTopRightPositionFlag & 0xFF0;
    Ipp32s width = pContext->m_seqLayerHeader->widthMB;
    Ipp32u QurrQuant = CurrDC->DoubleQuant;

    VC1DCMBParam* pA = NULL, *pB = NULL, *pC = NULL;

    memset(&DCPred.BlkPattern, 0, 6);
    DCPred.DoubleQuant[2] = QurrQuant;

    DCPred.ACTOP[0]  = CurrDC->DCBlkPred[0].ACTOP;
    DCPred.ACLEFT[0] = CurrDC->DCBlkPred[0].ACLEFT;

    DCPred.ACTOP[1]  = CurrDC->DCBlkPred[1].ACTOP;
    DCPred.ACLEFT[1] = CurrDC->DCBlkPred[1].ACLEFT;

    DCPred.ACTOP[2]  = CurrDC->DCBlkPred[2].ACTOP;
    DCPred.ACLEFT[2] = CurrDC->DCBlkPred[2].ACLEFT;

    switch(LeftTopFlag)
    {
    case (VC1_COMMON_MB):
        {
            pA = CurrDC - width;
            pB = CurrDC - width - 1;
            pC = CurrDC - 1;

            DCPred.BlkPattern[0] = 7;
            DCPred.BlkPattern[1] = 7;
            DCPred.BlkPattern[2] = 7;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 7;
            DCPred.BlkPattern[5] = 7;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;
            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            //Luma
            DCPred.DC[6] = pA->DCBlkPred[2].DC;
            DCPred.ACTOP[6] = pA->DCBlkPred[2].ACTOP;
            DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

            DCPred.DC[7] = pA->DCBlkPred[3].DC;
            DCPred.ACTOP[7] =  pA->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;


            DCPred.DC[3] =pB->DCBlkPred[3].DC;
            DCPred.ACTOP[3] =  pB->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[3] = pB->DCBlkPred[3].ACLEFT;

            DCPred.DC[8] = pC->DCBlkPred[1].DC;
            DCPred.ACTOP[8] =  pC->DCBlkPred[1].ACTOP;
            DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

            DCPred.DC[9] = pC->DCBlkPred[3].DC;
            DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[10] = pA->DCBlkPred[4].DC;
            DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

            DCPred.DC[4] = pB->DCBlkPred[4].DC;
            DCPred.ACTOP[4] =  pB->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[4] = pB->DCBlkPred[4].ACLEFT;

            DCPred.DC[11] = pC->DCBlkPred[4].DC;
            DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[12] = pA->DCBlkPred[5].DC;
            DCPred.ACTOP[12] =  pA->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;

            DCPred.DC[5] = pB->DCBlkPred[5].DC;
            DCPred.ACTOP[5] =  pB->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[5] = pB->DCBlkPred[5].ACLEFT;

            DCPred.DC[13] = pC->DCBlkPred[5].DC;
            DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
        }
        break;
    case(VC1_TOP_MB):
        {
            pC = CurrDC - 1;
            DCPred.BlkPattern[0] = 1;
            DCPred.BlkPattern[1] = 1;
            DCPred.BlkPattern[2] = 7;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 1;
            DCPred.BlkPattern[5] = 1;

            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            //Luma
            DCPred.DC[8] = pC->DCBlkPred[1].DC;
            DCPred.ACTOP[8] = pC->DCBlkPred[1].ACTOP;
            DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

            DCPred.DC[9] = pC->DCBlkPred[3].DC;
            DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[11] = pC->DCBlkPred[4].DC;
            DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[13] = pC->DCBlkPred[5].DC;
            DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
        }
        break;
    case (VC1_LEFT_MB):
        {
            pA = CurrDC - width;
            DCPred.BlkPattern[0] = 4;
            DCPred.BlkPattern[1] = 7;
            DCPred.BlkPattern[2] = 4;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 4;
            DCPred.BlkPattern[5] = 4;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;

            //Luma
            DCPred.DC[6] = pA->DCBlkPred[2].DC;
            DCPred.ACTOP[6] =  pA->DCBlkPred[2].ACTOP;
            DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

            DCPred.DC[7] = pA->DCBlkPred[3].DC;
            DCPred.ACTOP[7] =  pA->DCBlkPred[3].ACTOP;
            DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;

            //chroma
            //block 4
            DCPred.DC[10] = pA->DCBlkPred[4].DC;
            DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
            DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

            //block 5
            DCPred.DC[12] = pA->DCBlkPred[5].DC;
            DCPred.ACTOP[12] =  pA->DCBlkPred[5].ACTOP;
            DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;

        break;
    case (VC1_TOP_LEFT_MB):
        {
            DCPred.BlkPattern[0] = 0;
            DCPred.BlkPattern[1] = 1;
            DCPred.BlkPattern[2] = 4;
            DCPred.BlkPattern[3] = 7;
            DCPred.BlkPattern[4] = 0;
            DCPred.BlkPattern[5] = 0;
        }
        break;
    }

    }
    memcpy(&pContext->DCPred, &DCPred,sizeof(VC1DCPredictors));
}


void GetPScaleDCPredictors(VC1Context* pContext)
{
    VC1DCMBParam* CurrDC = pContext->CurrDC;
    VC1DCPredictors DCPred;
    Ipp32u QurrQuant = CurrDC->DoubleQuant;
    Ipp32u QurrDCStep = CurrDC->DCStepSize;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp32u LeftTopFlag = pCurrMB->LeftTopRightPositionFlag & 0xFF0;
    Ipp32s width = pContext->m_seqLayerHeader->widthMB;

    VC1DCMBParam* pA = NULL, *pB = NULL, *pC = NULL;

    memset(&DCPred.BlkPattern, 0, 6);

    Ipp8u AIntraFlag = 0, BIntraFlag = 0, CIntraFlag = 0;
    Ipp8u CurrIntraFlag = pCurrMB->IntraFlag;

    DCPred.DoubleQuant[2] = QurrQuant;
    DCPred.ACTOP[0]  = CurrDC->DCBlkPred[0].ACTOP;
    DCPred.ACLEFT[0] = CurrDC->DCBlkPred[0].ACLEFT;

    DCPred.ACTOP[1]  = CurrDC->DCBlkPred[1].ACTOP;
    DCPred.ACLEFT[1] = CurrDC->DCBlkPred[1].ACLEFT;

    DCPred.ACTOP[2]  = CurrDC->DCBlkPred[2].ACTOP;
    DCPred.ACLEFT[2] = CurrDC->DCBlkPred[2].ACLEFT;

    switch(LeftTopFlag)
    {
    case (VC1_COMMON_MB):
        {
            pA = CurrDC - width;
            AIntraFlag = (pCurrMB - width)->IntraFlag;

            pB = CurrDC - width - 1;
            BIntraFlag = (pCurrMB - width - 1)->IntraFlag;

            pC = CurrDC - 1;
            CIntraFlag = (pCurrMB - 1)->IntraFlag;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;
            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            if(AIntraFlag & 0x04)
            {
                DCPred.DC[6] = GetScaleDC(pA->DCBlkPred[2].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[6] =  pA->DCBlkPred[2].ACTOP;
                DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

                DCPred.BlkPattern[0]|=0x04;
                DCPred.BlkPattern[1]|=0x02;
            }

            if(AIntraFlag & 0x08)
            {
                DCPred.DC[7] = GetScaleDC(pA->DCBlkPred[3].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[7] =  pA->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[1]|=0x04;
            }

            if(BIntraFlag & 0x08)
            {
                DCPred.DC[3] = GetScaleDC(pB->DCBlkPred[3].DC,QurrQuant,
                                            pB->DoubleQuant, QurrDCStep,
                                            pB->DCStepSize);
                DCPred.ACTOP[3] =  pB->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[3] = pB->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[0]|=0x02;
            }

            if(CIntraFlag & 0x02)
            {
                DCPred.DC[8] = GetScaleDC(pC->DCBlkPred[1].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[8] =  pC->DCBlkPred[1].ACTOP;
                DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

                DCPred.BlkPattern[0]|=0x01;
                DCPred.BlkPattern[2]|=0x02;
           }

            if(CIntraFlag & 0x08)
            {
                DCPred.DC[9] = GetScaleDC(pC->DCBlkPred[3].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

                DCPred.BlkPattern[2]|=0x01;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(AIntraFlag & 0x10)
            {
                DCPred.DC[10] = GetScaleDC(pA->DCBlkPred[4].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

                DCPred.BlkPattern[4]|=0x04;
            }

            if(BIntraFlag & 0x10)
            {
                DCPred.DC[4] = GetScaleDC(pB->DCBlkPred[4].DC,QurrQuant,
                                            pB->DoubleQuant, QurrDCStep,
                                            pB->DCStepSize);
                DCPred.ACTOP[4] =  pB->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[4] = pB->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x02;
            }

            if(CIntraFlag & 0x10)
            {
                DCPred.DC[11] = GetScaleDC(pC->DCBlkPred[4].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x01;
            }

            //block 5
            if(AIntraFlag & 0x20)
            {
                DCPred.DC[12] = GetScaleDC(pA->DCBlkPred[5].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[12] =  pA->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x04;
            }

            if(BIntraFlag & 0x20)
            {
                DCPred.DC[5] = GetScaleDC(pB->DCBlkPred[5].DC,QurrQuant,
                                            pB->DoubleQuant, QurrDCStep,
                                            pB->DCStepSize);
                DCPred.ACTOP[5] = pB->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[5] = pB->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x02;
            }

            if(CIntraFlag & 0x20)
            {
                DCPred.DC[13] = GetScaleDC(pC->DCBlkPred[5].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x01;
            }
         }
        break;
    case(VC1_TOP_MB):
        {
            pC = CurrDC - 1;
            CIntraFlag = (pCurrMB - 1)->IntraFlag;

            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            if(CIntraFlag & 0x02)
            {
                DCPred.DC[8] = GetScaleDC(pC->DCBlkPred[1].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[8] =  pC->DCBlkPred[1].ACTOP;
                DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

                DCPred.BlkPattern[0]|=0x01;
                DCPred.BlkPattern[2]|=0x02;
           }

            if(CIntraFlag & 0x08)
            {
                DCPred.DC[9] = GetScaleDC(pC->DCBlkPred[3].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

                DCPred.BlkPattern[2]|=0x01;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(CIntraFlag & 0x10)
            {
                DCPred.DC[11] = GetScaleDC(pC->DCBlkPred[4].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x01;
            }

            //block 5
            if(CIntraFlag & 0x20)
            {
                DCPred.DC[13] = GetScaleDC(pC->DCBlkPred[5].DC,QurrQuant,
                                            pC->DoubleQuant, QurrDCStep,
                                            pC->DCStepSize);
                DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x01;
            }
        }
        break;
    case (VC1_LEFT_MB):
        {
            pA = CurrDC - width;
            AIntraFlag = (pCurrMB - width)->IntraFlag;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;

            if(AIntraFlag & 0x04)
            {
                DCPred.DC[6] = GetScaleDC(pA->DCBlkPred[2].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[6] =  pA->DCBlkPred[2].ACTOP;
                DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

                DCPred.BlkPattern[0]|=0x04;
                DCPred.BlkPattern[1]|=0x02;
            }

            if(AIntraFlag & 0x08)
            {
                DCPred.DC[7] = GetScaleDC(pA->DCBlkPred[3].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[7] =  pA->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[1]|=0x04;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(AIntraFlag & 0x10)
            {
                DCPred.DC[10] = GetScaleDC(pA->DCBlkPred[4].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[10] = pA->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

                DCPred.BlkPattern[4]|=0x04;
            }

            //block 5
            if(AIntraFlag & 0x20)
            {
                DCPred.DC[12] = GetScaleDC(pA->DCBlkPred[5].DC,QurrQuant,
                                            pA->DoubleQuant, QurrDCStep,
                                            pA->DCStepSize);
                DCPred.ACTOP[12] = pA->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x04;
            }
        }
        break;
    case (VC1_TOP_LEFT_MB):
        {
            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }
        }
        break;
    }

    memcpy(&pContext->DCPred, &DCPred,sizeof(VC1DCPredictors));

}

void GetPDCPredictors(VC1Context* pContext)
{
    VC1DCMBParam* CurrDC = pContext->CurrDC;
    VC1DCPredictors DCPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp32u LeftTopFlag = pCurrMB->LeftTopRightPositionFlag & 0xFF0;
    Ipp32s width = pContext->m_seqLayerHeader->widthMB;
    Ipp32u QurrQuant = CurrDC->DoubleQuant;

    VC1DCMBParam* pA = NULL, *pB = NULL, *pC = NULL;

    memset(&DCPred.BlkPattern, 0, 6);

    Ipp8u AIntraFlag = 0, BIntraFlag = 0, CIntraFlag = 0;
    Ipp8u CurrIntraFlag = pCurrMB->IntraFlag;

    DCPred.DoubleQuant[2] = QurrQuant;

    DCPred.ACTOP[0]  = CurrDC->DCBlkPred[0].ACTOP;
    DCPred.ACLEFT[0] = CurrDC->DCBlkPred[0].ACLEFT;

    DCPred.ACTOP[1]  = CurrDC->DCBlkPred[1].ACTOP;
    DCPred.ACLEFT[1] = CurrDC->DCBlkPred[1].ACLEFT;

    DCPred.ACTOP[2]  = CurrDC->DCBlkPred[2].ACTOP;
    DCPred.ACLEFT[2] = CurrDC->DCBlkPred[2].ACLEFT;

    switch(LeftTopFlag)
    {
    case (VC1_COMMON_MB):
        {
            pA = CurrDC - width;
            AIntraFlag = (pCurrMB - width)->IntraFlag;

            pB = CurrDC - width - 1;
            BIntraFlag = (pCurrMB - width - 1)->IntraFlag;

            pC = CurrDC - 1;
            CIntraFlag = (pCurrMB - 1)->IntraFlag;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;
            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            if(AIntraFlag & 0x04)
            {
                DCPred.DC[6] = pA->DCBlkPred[2].DC;
                DCPred.ACTOP[6] =  pA->DCBlkPred[2].ACTOP;
                DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

                DCPred.BlkPattern[0]|=0x04;
                DCPred.BlkPattern[1]|=0x02;
            }

            if(AIntraFlag & 0x08)
            {
                DCPred.DC[7] = pA->DCBlkPred[3].DC;
                DCPred.ACTOP[7] =  pA->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[1]|=0x04;
            }

            if(BIntraFlag & 0x08)
            {
                DCPred.DC[3] = pB->DCBlkPred[3].DC;
                DCPred.ACTOP[3] =  pB->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[3] = pB->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[0]|=0x02;
            }

            if(CIntraFlag & 0x02)
            {
                DCPred.DC[8] = pC->DCBlkPred[1].DC;
                DCPred.ACTOP[8] =  pC->DCBlkPred[1].ACTOP;
                DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

                DCPred.BlkPattern[0]|=0x01;
                DCPred.BlkPattern[2]|=0x02;
           }

            if(CIntraFlag & 0x08)
            {
                DCPred.DC[9] = pC->DCBlkPred[3].DC;
                DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

                DCPred.BlkPattern[2]|=0x01;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(AIntraFlag & 0x10)
            {
                DCPred.DC[10] = pA->DCBlkPred[4].DC;
                DCPred.ACTOP[10] =  pA->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

                DCPred.BlkPattern[4]|=0x04;
            }

            if(BIntraFlag & 0x10)
            {
                DCPred.DC[4] = pB->DCBlkPred[4].DC;
                DCPred.ACTOP[4] =  pB->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[4] = pB->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x02;
            }

            if(CIntraFlag & 0x10)
            {
                DCPred.DC[11] = pC->DCBlkPred[4].DC;
                DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x01;
            }

            //block 5
            if(AIntraFlag & 0x20)
            {
                DCPred.DC[12] = pA->DCBlkPred[5].DC;
                DCPred.ACTOP[12] =  pA->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x04;
            }

            if(BIntraFlag & 0x20)
            {
                DCPred.DC[5] = pB->DCBlkPred[5].DC;
                DCPred.ACTOP[5] =  pB->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[5] = pB->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x02;
            }

            if(CIntraFlag & 0x20)
            {
                DCPred.DC[13] = pC->DCBlkPred[5].DC;
                DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x01;
            }
        }
        break;
    case(VC1_TOP_MB):
        {
            pC = CurrDC - 1;
            CIntraFlag = (pCurrMB - 1)->IntraFlag;

            DCPred.DoubleQuant[1] = pC->DoubleQuant;

            if(CIntraFlag & 0x02)
            {
                DCPred.DC[8] = pC->DCBlkPred[1].DC;
                DCPred.ACTOP[8] =  pC->DCBlkPred[1].ACTOP;
                DCPred.ACLEFT[8] = pC->DCBlkPred[1].ACLEFT;

                DCPred.BlkPattern[0]|=0x01;
                DCPred.BlkPattern[2]|=0x02;
           }

            if(CIntraFlag & 0x08)
            {
                DCPred.DC[9] = pC->DCBlkPred[3].DC;
                DCPred.ACTOP[9] =  pC->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[9] = pC->DCBlkPred[3].ACLEFT;

                DCPred.BlkPattern[2]|=0x01;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(CIntraFlag & 0x10)
            {
                DCPred.DC[11] = pC->DCBlkPred[4].DC;
                DCPred.ACTOP[11] =  pC->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[11] = pC->DCBlkPred[4].ACLEFT;
                DCPred.BlkPattern[4]|=0x01;
            }

            //block 5
            if(CIntraFlag & 0x20)
            {
                DCPred.DC[13] = pC->DCBlkPred[5].DC;
                DCPred.ACTOP[13] =  pC->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[13] = pC->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x01;
            }
          }
        break;
    case (VC1_LEFT_MB):
        {
            pA = CurrDC - width;
            AIntraFlag = (pCurrMB - width)->IntraFlag;

            DCPred.DoubleQuant[0] = pA->DoubleQuant;

            if(AIntraFlag & 0x04)
            {
                DCPred.DC[6] = pA->DCBlkPred[2].DC;
                DCPred.ACTOP[6] =  pA->DCBlkPred[2].ACTOP;
                DCPred.ACLEFT[6] = pA->DCBlkPred[2].ACLEFT;

                DCPred.BlkPattern[0]|=0x04;
                DCPred.BlkPattern[1]|=0x02;
            }

            if(AIntraFlag & 0x08)
            {
                DCPred.DC[7] = pA->DCBlkPred[3].DC;
                DCPred.ACTOP[7] = pA->DCBlkPred[3].ACTOP;
                DCPred.ACLEFT[7] = pA->DCBlkPred[3].ACLEFT;
                DCPred.BlkPattern[1]|=0x04;
            }

            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }

            //block 4
            if(AIntraFlag & 0x10)
            {
                DCPred.DC[10] = pA->DCBlkPred[4].DC;
                DCPred.ACTOP[10] = pA->DCBlkPred[4].ACTOP;
                DCPred.ACLEFT[10] = pA->DCBlkPred[4].ACLEFT;

                DCPred.BlkPattern[4]|=0x04;
            }

            //block 5
            if(AIntraFlag & 0x20)
            {
                DCPred.DC[12] = pA->DCBlkPred[5].DC;
                DCPred.ACTOP[12] = pA->DCBlkPred[5].ACTOP;
                DCPred.ACLEFT[12] = pA->DCBlkPred[5].ACLEFT;
                DCPred.BlkPattern[5]|=0x04;
            }

       }
        break;
    case (VC1_TOP_LEFT_MB):
        {
            if(CurrIntraFlag & 0x01)
            {
                DCPred.BlkPattern[1]|=0x01;
                DCPred.BlkPattern[2]|=0x04;
                DCPred.BlkPattern[3]|=0x02;
            }

            if(CurrIntraFlag & 0x02)
            {
                DCPred.BlkPattern[3]|=0x04;
            }

            if(CurrIntraFlag & 0x04)
            {
                DCPred.BlkPattern[3]|=0x01;
            }
         }
        break;
    }

    memcpy(&pContext->DCPred, &DCPred,sizeof(VC1DCPredictors));
}


#endif //UMC_ENABLE_VC_VIDEO1_DECODER
