/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//                     motion estimation
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_me_vc1.h"
#include "umc_vc1_common_enc_tables.h"

#include "ippvc.h"

namespace UMC
{

using namespace UMC_VC1_ENCODER;

Ipp16s VC1ABS(Ipp16s value)
{
  Ipp16u s = value>>15;
  s = (value + s)^s;
  return s;
}

MeMV MePredictCalculatorVC1::GetPrediction(MeMV mv)
{
    // TODO: add other type of predictor here
    MeBase::SetError((vm_char*)"Wrong predictor in MePredictCalculatorVC1::GetPrediction", m_pMeParams->PredictionType!=ME_VC1Hybrid);

    MeMV res;
    if(m_pMeParams->PredictionType == ME_VC1Hybrid)
        GetPredictorVC1_hybrid(mv, &res);
    else
        res = m_pCur->PredMV[m_pCur->RefDir][m_pCur->RefIdx][m_pCur->BlkIdx];
    return res;
}
void MePredictCalculatorVC1::SetDefFrwBkw(MeMV &mvF, MeMV &mvB)
{
    if(m_pMeParams->PredictionType != ME_VC1Field2)
    {
        mvF = m_pMeParams->pRefF[m_pCur->RefIdx]->MVDirect[m_pCur->adr];
        mvB = m_pMeParams->pRefB[m_pCur->RefIdx]->MVDirect[m_pCur->adr];
        m_pCur->BestIdx[frw][0] = 0;
        m_pCur->BestIdx[bkw][0] = 0;
    }
    else //if(PredictionType == ME_VC1Field2)
    {
        mvF = GetDef_FieldMV(frw,0);
        mvB = GetDef_FieldMV(bkw,0);
        m_pCur->BestIdx[frw][0] = (GetDef_Field(frw,0) == false? 0 : 1);
        m_pCur->BestIdx[bkw][0] = (GetDef_Field(bkw,0) == false? 0 : 1);
    }
}

MeMV MePredictCalculatorVC1::GetPrediction16x16()
{
    if(!m_CurPrediction[m_pCur->RefDir][0][m_pCur->RefIdx].IsInvalid())
        return m_CurPrediction[m_pCur->RefDir][0][m_pCur->RefIdx];

    switch(m_pMeParams->PredictionType)
    {
    case ME_MPEG2:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorMPEG2;
        break;
    case ME_VC1:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1;
        break;
    case ME_VC1Hybrid:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1_hybrid;
        break;
    case ME_VC1Field1:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field1;
        break;
    case ME_VC1Field2:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field2;
        break;
    case ME_VC1Field2Hybrid:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field2Hybrid;
        break;
    default:
        return false;
    }

     // Edge MBs definitions.
    SetMbEdges();

    m_FieldPredictPrefer[m_pCur->RefDir][0] = (this->*(GetPredictor))(ME_Macroblock,m_pCur->RefDir,m_CurPrediction[m_pCur->RefDir][0]);
    return m_CurPrediction[m_pCur->RefDir][0][m_pCur->RefIdx];
}

MeMV MePredictCalculatorVC1::GetPrediction8x8()
{

    if(!m_CurPrediction[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx].IsInvalid())
        return m_CurPrediction[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx];

    switch(m_pMeParams->PredictionType)
    {
    case ME_MPEG2:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorMPEG2;
        break;
    case ME_VC1:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1;
        break;
    case ME_VC1Hybrid:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1_hybrid;
        break;
    case ME_VC1Field1:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field1;
        break;
    case ME_VC1Field2:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field2;
        break;
    case ME_VC1Field2Hybrid:
        GetPredictor = &UMC::MePredictCalculatorVC1::GetPredictorVC1Field2Hybrid;
        break;
    default:
        return 0;
    }

     // Edge MBs definitions.
    SetMbEdges();

    m_FieldPredictPrefer[m_pCur->RefDir][m_pCur->BlkIdx] = (this->*(GetPredictor))(m_pCur->BlkIdx,m_pCur->RefDir,m_CurPrediction[m_pCur->RefDir][m_pCur->BlkIdx]);
    return m_CurPrediction[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx];
}


void MePredictCalculatorVC1::GetBlockVectorsABC_0(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        CMV = m_pRes[m_pCur->adr-1].MV[isBkw][1];
    }
    else
    {
        isCOutOfBound = true;
    }
    if (!MbTopEnable)
    {
        isAOutOfBound = true;
        isBOutOfBound = true;
        return;
    }
    //MbTopEnable == true:
    AMV = m_pRes[m_pCur->adr - m_pMeParams->pSrc->WidthMB].MV[isBkw][2];

    if (MbTopLeftEnable)
    {
        BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].MV[isBkw][3];
    }
    else
    {
        //MbTopEnable == true:
        BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][3];
    }
}


void MePredictCalculatorVC1::GetBlockVectorsABC_1(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

//    CMV = m_pRes[m_pCur->adr].MV[isBkw][0];
    CMV = GetCurrentBlockMV(isBkw,0);

    if (!MbTopEnable)
    {
        isAOutOfBound = true;
        isBOutOfBound = true;
        return;
    }
    //MbTopEnable == true:
    AMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][3];

    if (MbTopRightEnable)
    {
        BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].MV[isBkw][2];
    }
    else
    {
        //MbTopEnable == true:
        BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][2];
    }
}

void MePredictCalculatorVC1::GetBlockVectorsABC_2(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        CMV = m_pRes[m_pCur->adr-1].MV[isBkw][3];
    }
    else
    {
        isCOutOfBound = true;
    }
//    AMV = m_pRes[m_pCur->adr].MV[isBkw][0];
//    BMV = m_pRes[m_pCur->adr].MV[isBkw][1];
    AMV = GetCurrentBlockMV(isBkw,0);
    BMV = GetCurrentBlockMV(isBkw,1);
}

void MePredictCalculatorVC1::GetBlockVectorsABC_3(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

//    CMV = m_pRes[m_pCur->adr].MV[isBkw][2];
//    AMV = m_pRes[m_pCur->adr].MV[isBkw][1];
//    BMV = m_pRes[m_pCur->adr].MV[isBkw][0];
    CMV = GetCurrentBlockMV(isBkw,2);
    AMV = GetCurrentBlockMV(isBkw,1);
    BMV = GetCurrentBlockMV(isBkw,0);
}

void MePredictCalculatorVC1::GetMacroBlockVectorsABC(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        CMV = m_pRes[m_pCur->adr-1].MV[isBkw][1];
    }
    else
    {
        isCOutOfBound = true;
    }

    if (MbTopEnable)
    {
        AMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][2];
    }
    else
    {
        isAOutOfBound = true;
    }

    if (MbTopRightEnable)
    {
        BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].MV[isBkw][2];
    }
    else
    {
        if(MbTopLeftEnable)
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].MV[isBkw][3];
        }
        else
        {
            isBOutOfBound = true;
        }
    }
}

void MePredictCalculatorVC1::GetBlockVectorsABCField_0(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);


    AMVbSecond  = false;
    BMVbSecond  = false;
    CMVbSecond  = false;
    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        if(m_pRes[m_pCur->adr-1].BlockType[1] == ME_MbIntra)
        {
            isCOutOfBound = true;
        }
        else
        {
            CMV = m_pRes[m_pCur->adr-1].MV[isBkw][1];
            CMVbSecond =  m_pRes[m_pCur->adr-1].Refindex[isBkw][1] == 1;
        }
    }
    else
    {
        isCOutOfBound = true;
    }
    if(!MbTopEnable)
    {
        isAOutOfBound = true;
        isBOutOfBound = true;
        return;
    }
    //MbTopEnable == true;
    if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].BlockType[2] == ME_MbIntra)
    {
        isAOutOfBound = true;
    }
    else
    {
        AMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][2];
        AMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].Refindex[isBkw][2] == 1;
    }

    if (MbTopLeftEnable)
    {
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].BlockType[3] == ME_MbIntra)
        {
            isBOutOfBound = true;
        }
        else
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].MV[isBkw][3];
            BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].Refindex[isBkw][3] == 1;
        }
    }
    else
    {
        //MbTopEnable == true:
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].BlockType[3] == ME_MbIntra)
        {
            isBOutOfBound = true;
        }
        else
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][3];
            BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].Refindex[isBkw][3] == 1;
        }
    }

}

void MePredictCalculatorVC1::GetBlockVectorsABCField_1(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    AMVbSecond  = false;
    BMVbSecond  = false;
    CMVbSecond  = false;
    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if(m_pRes[m_pCur->adr].BlockType[0] == ME_MbIntra)
    {
        isCOutOfBound = true;
    }
    else
    {
        CMV = m_pRes[m_pCur->adr].MV[isBkw][0];
        CMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][0] == 1;
    }

    if(!MbTopEnable)
    {
        isAOutOfBound = true;
        isBOutOfBound = true;
        return;
    }
    //MbTopEnable == true:
    if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].BlockType[3] == ME_MbIntra)
    {
        isAOutOfBound = true;
    }
    else
    {
        AMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][3];
        AMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].Refindex[isBkw][3] == 1;
    }

    if (MbTopRightEnable)
    {
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].BlockType[2] == ME_MbIntra)
        {
            isBOutOfBound = true;
        }
        else
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].MV[isBkw][2];
            BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].Refindex[isBkw][2] == 1;
        }
    }
    else //MbTopEnable == true:
    {
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].BlockType[2] == ME_MbIntra)
        {
            isBOutOfBound = true;
        }
        else
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][2];
            BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].Refindex[isBkw][2] == 1;
        }
    }
}

void MePredictCalculatorVC1::GetBlockVectorsABCField_2(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    AMVbSecond  = false;
    BMVbSecond  = false;
    CMVbSecond  = false;
    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        if(m_pRes[m_pCur->adr-1].BlockType[3] == ME_MbIntra)
        {
            isCOutOfBound = true;
        }
        else
        {
            CMV = m_pRes[m_pCur->adr-1].MV[isBkw][3];
            CMVbSecond =  m_pRes[m_pCur->adr-1].Refindex[isBkw][3] == 1;
        }
    }
    else
    {
        isCOutOfBound = true;
    }

    if(m_pRes[m_pCur->adr].BlockType[0] == ME_MbIntra)
    {
        isAOutOfBound = true;
    }
    else
    {
        AMV = m_pRes[m_pCur->adr].MV[isBkw][0];
        AMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][0] == 1;
    }

    if(m_pRes[m_pCur->adr].BlockType[1] == ME_MbIntra)
    {
        isBOutOfBound = true;
    }
    else
    {
        BMV = m_pRes[m_pCur->adr].MV[isBkw][1];
        BMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][1] == 1;
    }
}

void MePredictCalculatorVC1::GetBlockVectorsABCField_3(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    AMVbSecond  = false;
    BMVbSecond  = false;
    CMVbSecond  = false;
    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if(m_pRes[m_pCur->adr].BlockType[2] == ME_MbIntra)
    {
        isCOutOfBound = true;
    }
    else
    {
        CMV = m_pRes[m_pCur->adr].MV[isBkw][2];
        CMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][2] == 1;
    }

    if(m_pRes[m_pCur->adr].BlockType[1] == ME_MbIntra)
    {
        isAOutOfBound = true;
    }
    else
    {
        AMV = m_pRes[m_pCur->adr].MV[isBkw][1];
        AMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][1] == 1;
    }

    if(m_pRes[m_pCur->adr].BlockType[0] == ME_MbIntra)
    {
        isBOutOfBound = true;
    }
    else
    {
        BMV = m_pRes[m_pCur->adr].MV[isBkw][0];
        BMVbSecond =  m_pRes[m_pCur->adr].Refindex[isBkw][0]  == 1;
    }
}

void MePredictCalculatorVC1::GetMacroBlockVectorsABCField(int isBkw)
{
    AMV = MeMV(0);
    BMV = MeMV(0);
    CMV = MeMV(0);

    AMVbSecond = false;
    BMVbSecond = false;
    CMVbSecond = false;
    isAOutOfBound = false;
    isBOutOfBound = false;
    isCOutOfBound = false;

    if (MbLeftEnable)
    {
        if(m_pRes[m_pCur->adr-1].BlockType[1] == ME_MbIntra)
        {
            isCOutOfBound = true;
        }
        else
        {
            CMV = m_pRes[m_pCur->adr-1].MV[isBkw][1];
            CMVbSecond =  m_pRes[m_pCur->adr-1].Refindex[isBkw][1] == 1;
        }

    }
    else
    {
        isCOutOfBound = true;
    }

    if (MbTopEnable)
    {
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].BlockType[2] == ME_MbIntra)
        {
            isAOutOfBound = true;
        }
        else
        {
            AMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].MV[isBkw][2];
            AMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB].Refindex[isBkw][2] == 1;
        }
    }
    else
    {
        isAOutOfBound = true;
    }
    if (MbTopRightEnable)
    {
        if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].BlockType[2] == ME_MbIntra)
        {
            isBOutOfBound = true;
        }
        else
        {
            BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].MV[isBkw][2];
            BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB+1].Refindex[isBkw][2] == 1;
        }
    }
    else
    {
        if(MbTopLeftEnable)
        {
            if(m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].BlockType[2] == ME_MbIntra)
            {
                isBOutOfBound = true;
            }
            else
            {
                BMV = m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].MV[isBkw][2];
                BMVbSecond =  m_pRes[m_pCur->adr-m_pMeParams->pSrc->WidthMB-1].Refindex[isBkw][2] == 1;
            }
        }
        else
        {
            isBOutOfBound = true;
        }
    }
}

bool MePredictCalculatorVC1::GetPredictorVC1(int index, int isBkw, MeMV* res)
{
    switch(index)
    {
    case ME_Block0:
        GetBlockVectorsABC_0(isBkw);
        break;
    case ME_Block1:
        GetBlockVectorsABC_1(isBkw);
        break;
    case ME_Block2:
        GetBlockVectorsABC_2(isBkw);
        break;
    case ME_Block3:
        GetBlockVectorsABC_3(isBkw);
        break;
    case ME_Macroblock:
        GetMacroBlockVectorsABC(isBkw);
        break;
    default:
        VM_ASSERT(false);
        break;
    }

    Ipp32s pIdx = index == ME_Macroblock ? 0:index;

    m_AMV[isBkw][pIdx][0] = AMV;
    m_BMV[isBkw][pIdx][0] = BMV;
    m_CMV[isBkw][pIdx][0] = CMV;

    if(!isAOutOfBound)
    {
        if(isCOutOfBound && isBOutOfBound)
        {
            res->x = AMV.x;
            res->y = AMV.y;
        }
        else
        {
            res->x=median3(AMV.x, BMV.x, CMV.x);
            res->y=median3(AMV.y, BMV.y, CMV.y);
        }
    }
    else
    {
        if(!isCOutOfBound)
        {
            res->x = CMV.x;
            res->y = CMV.y;
        }
        else
        {
            res->x = 0;
            res->y = 0;
        }
    }
    return false;
}

bool MePredictCalculatorVC1::GetPredictorVC1_hybrid(int index,int isBkw, MeMV* res)
{
    Ipp32s sumA = 0, sumC = 0;
    //forward direction only

    switch(index)
    {
    case ME_Block0:
        GetBlockVectorsABC_0(0);
        break;
    case ME_Block1:
        GetBlockVectorsABC_1(0);
        break;
    case ME_Block2:
        GetBlockVectorsABC_2(0);
        break;
    case ME_Block3:
        GetBlockVectorsABC_3(0);
        break;
    case ME_Macroblock:
        GetMacroBlockVectorsABC(0);
        break;
    default:
        VM_ASSERT(false);
        break;
    }

    Ipp32s pIdx = index == ME_Macroblock ? 0:index;

    m_AMV[isBkw][pIdx][0] = AMV;
    m_BMV[isBkw][pIdx][0] = BMV;
    m_CMV[isBkw][pIdx][0] = CMV;

    if(!isAOutOfBound)
    {
        if(isCOutOfBound && isBOutOfBound)
        {
            res->x = AMV.x;
            res->y = AMV.y;
        }
        else
        {
            res->x=median3(AMV.x, BMV.x, CMV.x);
            res->y=median3(AMV.y, BMV.y, CMV.y);
        }
    }
    else
    {
        if(!isCOutOfBound)
        {
            res->x = CMV.x;
            res->y = CMV.y;
        }
        else
        {
            res->x = 0;
            res->y = 0;
        }
    }

    if(!isAOutOfBound && !isCOutOfBound)
    {
        sumA = VC1ABS(AMV.x - res->x) + VC1ABS(AMV.y - res->y);
        sumC = VC1ABS(CMV.x - res->x) + VC1ABS(CMV.y - res->y);
        if (sumA > 32 || sumC>32)
        {
            sumA < sumC ? res[0] = AMV : res[0] = CMV;
        }
    }
    return false;
}

void MePredictCalculatorVC1::GetPredictorVC1_hybrid(MeMV cur, MeMV* res)
{
    //should be called only after GetPredictorVC1_hybrid

    Ipp32s sumA = 0, sumC = 0;
    //forward direction only

    if(!isAOutOfBound)
    {
        if(isCOutOfBound && isBOutOfBound)
        {
            res->x = AMV.x;
            res->y = AMV.y;
        }
        else
        {
            res->x=median3(AMV.x, BMV.x, CMV.x);
            res->y=median3(AMV.y, BMV.y, CMV.y);
        }
    }
    else
    {
        if(!isCOutOfBound)
        {
            res->x = CMV.x;
            res->y = CMV.y;
        }
        else
        {
            res->x = 0;
            res->y = 0;
        }
    }

    m_pCur->HybridPredictor=false;
    if(!isAOutOfBound && !isCOutOfBound)
    {
        sumA = VC1ABS(AMV.x - res->x) + VC1ABS(AMV.y - res->y);
        sumC = VC1ABS(CMV.x - res->x) + VC1ABS(CMV.y - res->y);
        if (sumA > 32 || sumC>32)
        {
            sumA < sumC ? res[0] = AMV : res[0] = CMV;

            if (VC1ABS(AMV.x - cur.x) + VC1ABS(AMV.y - cur.y)<
                VC1ABS(CMV.x - cur.x) + VC1ABS(CMV.y - cur.y))
            {
                //hybrid = 2;
                res->x = AMV.x;
                res->y = AMV.y;
                m_pCur->HybridPredictor=true;
            }
            else
            {
               // hybrid = 1;
                res->x = CMV.x;
                res->y = CMV.y;
                m_pCur->HybridPredictor=true;
            }
        }
    }
}


static Ipp16s scale_sameX(Ipp16s n,MeVC1fieldScaleInfo* pInfo, bool bHalf)
{
    Ipp32s shift = (bHalf)?1:0;
    Ipp16s abs_n = VC1ABS(n = (Ipp16s)(n >> shift));
    Ipp32s s;
    if (abs_n>255)
    {
        return (Ipp16s)(n << shift);
    }
    else if (abs_n<pInfo->ME_ScaleZoneX)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame2))>>8);
        s = (n<0)? s - pInfo->ME_ZoneOffsetX:s + pInfo->ME_ZoneOffsetX;
    }
    s = (s>pInfo->ME_RangeX-1)? pInfo->ME_RangeX-1:s;
    s = (s<-pInfo->ME_RangeX) ? -pInfo->ME_RangeX :s;

    return (Ipp16s) (s << shift);
}
static Ipp16s scale_oppX(Ipp16s n,MeVC1fieldScaleInfo* pInfo, bool bHalf)
{
    Ipp32s shift = (bHalf)?1:0;
    Ipp32s s = (((Ipp32s)((n >> shift)*pInfo->ME_ScaleOpp))>>8);
    return (Ipp16s) (s << shift);
}
/*
static Ipp16s scale_sameY(Ipp16s n,MeVC1fieldScaleInfo* pInfo, bool bHalf)
{
    Ipp16s abs_n = VC1ABS(n = (n >> bHalf));
    Ipp32s s     = 0;
    if (abs_n>63)
    {
        return n << bHalf;
    }
    else if (abs_n<pInfo->ME_ScaleZoneY)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame2))>>8);
        s = (n<0)? s - pInfo->ME_ZoneOffsetY:s + pInfo->ME_ZoneOffsetY;
    }
    if (pInfo->ME_Bottom)
    {
        s = (s>pInfo->ME_RangeY/2)? pInfo->ME_RangeY/2:s;
        s = (s<-pInfo->ME_RangeY/2+1) ? -pInfo->ME_RangeY/2+1 :s;
    }
    else
    {
        s = (s>pInfo->ME_RangeY/2-1)? pInfo->ME_RangeY/2-1:s;
        s = (s<-pInfo->ME_RangeY/2) ? -pInfo->ME_RangeY/2 :s;
    }
    return (Ipp16s) (s << bHalf);
}
*/

static Ipp16s scale_sameY(Ipp16s n,MeVC1fieldScaleInfo* pInfo, bool bHalf)
{
    Ipp16s abs_n = VC1ABS(n = (n>>((Ipp16u)bHalf)));
    Ipp32s s     = 0;
    if (abs_n>63)
    {
        return n<<((Ipp16u)bHalf);
    }

    else if (abs_n<pInfo->ME_ScaleZoneY)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame2))>>8);
        s = (n<0)? s - pInfo->ME_ZoneOffsetY:s + pInfo->ME_ZoneOffsetY;
    }

    s = (s>pInfo->ME_RangeY/2-1)? pInfo->ME_RangeY/2-1:s;
    s = (s<-pInfo->ME_RangeY/2) ? -pInfo->ME_RangeY/2 :s;

    return (Ipp16s) (s<<((Ipp16u)bHalf));
}

static Ipp16s scale_sameY_B(Ipp16s n,MeVC1fieldScaleInfo* pInfo, bool bHalf)
{
    Ipp32s shift = (bHalf)?1:0;
    Ipp16s abs_n = VC1ABS(n = (Ipp16s)(n>>shift));
    Ipp32s s     = 0;
    if (abs_n>63)
    {
        return (Ipp16s)(n<<shift);
    }
    else if (abs_n<pInfo->ME_ScaleZoneY)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->ME_ScaleSame2))>>8);
        s = (n<0)? s - pInfo->ME_ZoneOffsetY:s + pInfo->ME_ZoneOffsetY;
    }
    if (pInfo->ME_Bottom)
    {
        s = (s>pInfo->ME_RangeY/2)? pInfo->ME_RangeY/2:s;
        s = (s<-pInfo->ME_RangeY/2+1) ? -pInfo->ME_RangeY/2+1 :s;
    }
    else
    {
        s = (s>pInfo->ME_RangeY/2-1)? pInfo->ME_RangeY/2-1:s;
        s = (s<-pInfo->ME_RangeY/2) ? -pInfo->ME_RangeY/2 :s;
    }
    return (Ipp16s) (s<<shift);
}

static Ipp16s scale_oppY(Ipp16s n,MeVC1fieldScaleInfo* pInfo,  bool bHalf)
{
    Ipp32s shift = (bHalf)?1:0;
    Ipp32s s = (((Ipp32s)((n >> shift)*pInfo->ME_ScaleOpp))>>8);
    return (Ipp16s) (s << shift);

}
typedef Ipp16s (*fScaleX)(Ipp16s n, MeVC1fieldScaleInfo* pInfo, bool bHalf);
typedef Ipp16s (*fScaleY)(Ipp16s n, MeVC1fieldScaleInfo* pInfo, bool bHalf);

static fScaleX pScaleX[2][2] = {{scale_oppX, scale_sameX},
                                {scale_sameX,scale_oppX}};
static fScaleY pScaleY[2][2] = {{scale_oppY, scale_sameY},
                                {scale_sameY_B, scale_oppY}};

bool MePredictCalculatorVC1::GetPredictorVC1Field2Hybrid(int index, int isBkw, MeMV* res)
{
    bool rs = false;
    MeMV A[2];
    MeMV B[2];
    MeMV C[2];

    switch(index)
    {
    case ME_Block0:
        GetBlockVectorsABCField_0(isBkw);
        break;
    case ME_Block1:
        GetBlockVectorsABCField_1(isBkw);
        break;
    case ME_Block2:
        GetBlockVectorsABCField_2(isBkw);
        break;
    case ME_Block3:
        GetBlockVectorsABCField_3(isBkw);
        break;
    case ME_Macroblock:
        GetMacroBlockVectorsABCField(isBkw);
        break;
    default:
        VM_ASSERT(false);
        break;
    }

    Ipp32s pIdx = index == ME_Macroblock ? 0:index;

    m_AMV[isBkw][pIdx][0] = MeMV(0);
    m_BMV[isBkw][pIdx][0] = MeMV(0);
    m_CMV[isBkw][pIdx][0] = MeMV(0);

    m_AMV[isBkw][pIdx][1] = MeMV(0);
    m_BMV[isBkw][pIdx][1] = MeMV(0);
    m_CMV[isBkw][pIdx][1] = MeMV(0);

    bool bBackwardFirst = (isBkw && !m_pMeParams->bSecondField);
    bool bHalf = (m_pMeParams->PixelType == ME_HalfPixel);
    Ipp8u n = ((isAOutOfBound)<<2) + ((isBOutOfBound)<<1) + (isCOutOfBound);

    switch (n)
    {
    case 7: //111
        {
            res[0].x=0;
            res[0].y=0;
            res[1].x=0;
            res[1].y=0;
            rs =  false; //opposite field
            break;
        }
    case 6: //110
        {
            bool  bSame = CMVbSecond;
            res[CMVbSecond].x  =  CMV.x;
            res[CMVbSecond].y  =  CMV.y;
            m_CMV[isBkw][pIdx][!CMVbSecond].x = res[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_CMV[isBkw][pIdx][!CMVbSecond].y = res[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_CMV[isBkw][pIdx][CMVbSecond]  = CMV;
            rs = CMVbSecond;
            break;
        }
    case 5: //101
        {
            bool  bSame = BMVbSecond;
            res[BMVbSecond].x  =  BMV.x;
            res[BMVbSecond].y  =  BMV.y;
            m_BMV[isBkw][pIdx][!BMVbSecond].x = res[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_BMV[isBkw][pIdx][!BMVbSecond].y = res[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_BMV[isBkw][pIdx][BMVbSecond]  = BMV;
            rs = BMVbSecond;
            break;
        }
    case 3: //011
        {
            bool  bSame = AMVbSecond;
            res[AMVbSecond].x  =  AMV.x;
            res[AMVbSecond].y  =  AMV.y;
            m_AMV[isBkw][pIdx][!BMVbSecond].x = res[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_AMV[isBkw][pIdx][!BMVbSecond].y = res[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_AMV[isBkw][pIdx][BMVbSecond]  = AMV;
            rs = AMVbSecond;
            break;
        }
    case 4: //100
        {
            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = BMVbSecond;
            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(C[0].x,B[0].x,0);
            res[1].x = median3(C[1].x,B[1].x,0);
            res[0].y = median3(C[0].y,B[0].y,0);
            res[1].y = median3(C[1].y,B[1].y,0);
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];
            rs = (BMVbSecond != CMVbSecond)? false : CMVbSecond;
            break;
        }
    case 2: //010
        {
            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond ;
            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(C[0].x,A[0].x,0);
            res[1].x = median3(C[1].x,A[1].x,0);
            res[0].y = median3(C[0].y,A[0].y,0);
            res[1].y = median3(C[1].y,A[1].y,0);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];
            rs = (AMVbSecond != CMVbSecond)? false : AMVbSecond;
            break;
        }
    case 1: //001
        {
            bool  bSame = BMVbSecond;

            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond;
            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(B[0].x,A[0].x,0);
            res[1].x = median3(B[1].x,A[1].x,0);
            res[0].y = median3(B[0].y,A[0].y,0);
            res[1].y = median3(B[1].y,A[1].y,0);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            rs = (AMVbSecond != BMVbSecond)? false : BMVbSecond;
            break;
        }
    case 0:
        {
            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = BMVbSecond;

            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond;

            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(B[0].x,A[0].x,C[0].x);
            res[1].x = median3(B[1].x,A[1].x,C[1].x);
            res[0].y = median3(B[0].y,A[0].y,C[0].y);
            res[1].y = median3(B[1].y,A[1].y,C[1].y);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];

            rs = (AMVbSecond + BMVbSecond + CMVbSecond > 1)? true : false;
            break;
        }
    default:
        {
            VM_ASSERT(0);
        }
    }
    if (!isAOutOfBound && !isCOutOfBound)
    {
       // Ipp32u th = m_par->PixelType ==  ME_HalfPixel ? 16:32;
        Ipp32u th = 32;
        //0 field
        MeMV  mvCPred = C[0];
        MeMV  mvAPred = A[0];

        Ipp32u sumA = VC1ABS(mvAPred.x - res[0].x) + VC1ABS(mvAPred.y - res[0].y);
        Ipp32u sumC = VC1ABS(mvCPred.x - res[0].x) + VC1ABS(mvCPred.y - res[0].y);
        if (sumA > th)
        {
            res[0].x = mvAPred.x;
            res[0].y = mvAPred.y;
        }
        else if (sumC > th)
        {
            res[0].x = mvCPred.x;
            res[0].y = mvCPred.y;
        }

        //1 field
        mvCPred = C[1];
        mvAPred = A[1];

        sumA = VC1ABS(mvAPred.x - res[1].x) + VC1ABS(mvAPred.y - res[1].y);
        sumC = VC1ABS(mvCPred.x - res[1].x) + VC1ABS(mvCPred.y - res[1].y);
        if (sumA > th)
        {
            res[1].x = mvAPred.x;
            res[1].y = mvAPred.y;
        }
        else if (sumC > th)
        {
            res[1].x = mvCPred.x;
            res[1].y = mvCPred.y;
        }
    }//if (!isAOutOfBound && !isCOutOfBound)
    return rs;
}

bool MePredictCalculatorVC1::GetPredictorVC1Field2(int index, int isBkw, MeMV* res)
{
    bool rs = false;
    switch(index)
    {
    case ME_Block0:
        GetBlockVectorsABCField_0(isBkw);
        break;
    case ME_Block1:
        GetBlockVectorsABCField_1(isBkw);
        break;
    case ME_Block2:
        GetBlockVectorsABCField_2(isBkw);
        break;
    case ME_Block3:
        GetBlockVectorsABCField_3(isBkw);
        break;
    case ME_Macroblock:
        GetMacroBlockVectorsABCField(isBkw);
        break;
    default:
        VM_ASSERT(false);
        break;
    }

    Ipp32s pIdx = index == ME_Macroblock ? 0:index;

    m_AMV[isBkw][pIdx][0] = MeMV(0);
    m_BMV[isBkw][pIdx][0] = MeMV(0);
    m_CMV[isBkw][pIdx][0] = MeMV(0);

    m_AMV[isBkw][pIdx][1] = MeMV(0);
    m_BMV[isBkw][pIdx][1] = MeMV(0);
    m_CMV[isBkw][pIdx][1] = MeMV(0);

    bool bBackwardFirst = (isBkw && !m_pMeParams->bSecondField);
    bool bHalf = (m_pMeParams->PixelType == ME_HalfPixel);
    Ipp8u n = ((isAOutOfBound)<<2) + ((isBOutOfBound)<<1) + (isCOutOfBound);

    switch (n)
    {
    case 7: //111
        {
            res[0].x=0;
            res[0].y=0;
            res[1].x=0;
            res[1].y=0;
            rs =  false; //opposite field
            break;
        }
    case 6: //110
        {
            bool  bSame = CMVbSecond;
            res[CMVbSecond].x  =  CMV.x;
            res[CMVbSecond].y  =  CMV.y;
            m_CMV[isBkw][pIdx][!CMVbSecond].x = res[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_CMV[isBkw][pIdx][!CMVbSecond].y = res[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_CMV[isBkw][pIdx][CMVbSecond]  = CMV;
            rs = CMVbSecond;
            break;
        }
    case 5: //101
        {
            MeMV B;
            bool  bSame = BMVbSecond;
            res[BMVbSecond].x  =  BMV.x;
            res[BMVbSecond].y  =  BMV.y;
            B.x = res[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B.y = res[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_BMV[isBkw][pIdx][BMVbSecond]  = BMV;
            m_BMV[isBkw][pIdx][!BMVbSecond] = B;
            rs = BMVbSecond;
            break;
        }
    case 3: //011
        {
            bool  bSame = AMVbSecond;
            res[AMVbSecond].x  =  AMV.x;
            res[AMVbSecond].y  =  AMV.y;
            m_AMV[isBkw][pIdx][!BMVbSecond].x = res[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_AMV[isBkw][pIdx][!BMVbSecond].y = res[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            m_AMV[isBkw][pIdx][BMVbSecond]  = AMV;
            rs = AMVbSecond;
            break;
        }
    case 4: //100
        {
            MeMV B[2];
            MeMV C[2];
            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = BMVbSecond;
            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(C[0].x,B[0].x,0);
            res[1].x = median3(C[1].x,B[1].x,0);
            res[0].y = median3(C[0].y,B[0].y,0);
            res[1].y = median3(C[1].y,B[1].y,0);
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];
            rs = (BMVbSecond != CMVbSecond)? false : CMVbSecond;
            break;
        }
    case 2: //010
        {
            MeMV A[2];
            MeMV C[2];
            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond ;
            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(C[0].x,A[0].x,0);
            res[1].x = median3(C[1].x,A[1].x,0);
            res[0].y = median3(C[0].y,A[0].y,0);
            res[1].y = median3(C[1].y,A[1].y,0);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];
            rs = (AMVbSecond != CMVbSecond)? false : AMVbSecond;
            break;
        }
    case 1: //001
        {
            MeMV A[2];
            MeMV B[2];
            bool  bSame = BMVbSecond;

            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond;
            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(B[0].x,A[0].x,0);
            res[1].x = median3(B[1].x,A[1].x,0);
            res[0].y = median3(B[0].y,A[0].y,0);
            res[1].y = median3(B[1].y,A[1].y,0);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            rs = (AMVbSecond != BMVbSecond)? false : BMVbSecond;
            break;
        }
    case 0:
        {
            MeMV A[2];
            MeMV B[2];
            MeMV C[2];

            bool  bSame = CMVbSecond;

            C[CMVbSecond].x  =  CMV.x;
            C[CMVbSecond].y  =  CMV.y;
            C[!CMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](CMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            C[!CMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](CMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = BMVbSecond;

            B[BMVbSecond].x  =  BMV.x;
            B[BMVbSecond].y  =  BMV.y;
            B[!BMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](BMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            B[!BMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](BMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            bSame = AMVbSecond;

            A[AMVbSecond].x  =  AMV.x;
            A[AMVbSecond].y  =  AMV.y;
            A[!AMVbSecond].x =  pScaleX[bBackwardFirst][!bSame](AMV.x,&m_pMeParams->ScaleInfo[isBkw], bHalf);
            A[!AMVbSecond].y =  pScaleY[bBackwardFirst][!bSame](AMV.y,&m_pMeParams->ScaleInfo[isBkw], bHalf);

            res[0].x = median3(B[0].x,A[0].x,C[0].x);
            res[1].x = median3(B[1].x,A[1].x,C[1].x);
            res[0].y = median3(B[0].y,A[0].y,C[0].y);
            res[1].y = median3(B[1].y,A[1].y,C[1].y);
            m_AMV[isBkw][pIdx][0] = A[0];
            m_AMV[isBkw][pIdx][1] = A[1];
            m_BMV[isBkw][pIdx][0] = B[0];
            m_BMV[isBkw][pIdx][1] = B[1];
            m_CMV[isBkw][pIdx][0] = C[0];
            m_CMV[isBkw][pIdx][1] = C[1];

            rs = (AMVbSecond + BMVbSecond + CMVbSecond > 1)? true : false;
            break;
        }
    default:
        {
            VM_ASSERT(0);
        }
    }
    return rs;
}

bool MePredictCalculatorVC1::GetPredictorVC1Field1(int index, int isBkw, MeMV* res)
{
    switch(index)
    {
    case ME_Block0:
        GetBlockVectorsABCField_0(isBkw);
        break;
    case ME_Block1:
        GetBlockVectorsABCField_1(isBkw);
        break;
    case ME_Block2:
        GetBlockVectorsABCField_2(isBkw);
        break;
    case ME_Block3:
        GetBlockVectorsABCField_3(isBkw);
        break;
    case ME_Macroblock:
        GetMacroBlockVectorsABCField(isBkw);
        break;
    default:
        VM_ASSERT(false);
        break;
    }

    Ipp8u n = ((isAOutOfBound)<<2) + ((isBOutOfBound)<<1) + (isCOutOfBound);
    switch (n)
    {
    case 7: //111
        {
            res[0].x = 0;
            res[0].y = 0;
            break;
        }
    case 6: //110
        {
            res[0].x  =  CMV.x;
            res[0].y  =  CMV.y;
            break;
        }
    case 5: //101
        {
            res[0].x  =  BMV.x;
            res[0].y  =  BMV.y;
            break;
        }
    case 3: //011
        {
            res[0].x  =  AMV.x;
            res[0].y  =  AMV.y;
            break;
        }
    case 4: //100
        {
            res[0].x = median3(CMV.x,BMV.x,0);
            res[0].y = median3(CMV.y,BMV.y,0);
            break;
        }
    case 2: //010
        {
            res[0].x = median3(CMV.x,AMV.x,0);
            res[0].y = median3(CMV.y,AMV.y,0);
            break;
        }
    case 1: //001
        {
            res[0].x = median3(BMV.x,AMV.x,0);
            res[0].y = median3(BMV.y,AMV.y,0);
            break;
        }
    case 0:
        {
            res[0].x = median3(BMV.x,AMV.x,CMV.x);
            res[0].y = median3(BMV.y,AMV.y,CMV.y);
            break;
        }
    default:
        {
            VM_ASSERT(0);
        }
    }
    Ipp32s pIdx = index == ME_Macroblock ? 0:index;

    m_AMV[isBkw][pIdx][0] = AMV;
    m_BMV[isBkw][pIdx][0] = BMV;
    m_CMV[isBkw][pIdx][0] = CMV;

    if (!isAOutOfBound && !isCOutOfBound)
    {
        Ipp32u th = 32;
        if(m_pMeParams->SearchDirection == ME_BidirSearch)
            th = (m_pMeParams->PixelType ==  ME_HalfPixel) ? 16:32;
        //0 field
        Ipp32u sumA = VC1ABS(m_AMV[isBkw][pIdx][0].x - res[0].x) + VC1ABS(m_AMV[isBkw][pIdx][0].y - res[0].y);
        Ipp32u sumC = VC1ABS(m_CMV[isBkw][pIdx][0].x - res[0].x) + VC1ABS(m_CMV[isBkw][pIdx][0].y - res[0].y);
        if (sumA > th)
        {
            res[0].x = m_AMV[isBkw][pIdx][0].x;
            res[0].y = m_AMV[isBkw][pIdx][0].y;
        }
        else if (sumC > th)
        {
            res[0].x = m_CMV[isBkw][pIdx][0].x;
            res[0].y = m_CMV[isBkw][pIdx][0].y;
        }
    }
    return false;
}


bool MePredictCalculatorVC1::GetPredictorMPEG2(int index,int isBkw, MeMV* res)
{
    index = index;
    if (MbLeftEnable)
    {
        if(isBkw != 0) res[0] = m_pRes[m_pCur->adr-1].MV[1][0];
        res[0] = m_pRes[m_pCur->adr-1].MV[0][0];
    }
    else
    {
        res[0] = MeMV(0);
    }
    return true;
}



void MeVC1::SetInterpPixelType()
{
    Ipp32s cost = IPP_MIN(CostOnInterpolation[1], CostOnInterpolation[3]);
    if(cost == CostOnInterpolation[1])
    {
        m_par->OutInterpolation = ME_VC1_Bilinear;
        m_par->OutPixelType     = ME_HalfPixel;
        return;
    }

    m_par->OutInterpolation = ME_VC1_Bicubic;
    m_par->OutPixelType     = ME_QuarterPixel;
}

bool MeVC1::EstimateSkip()
{
    if(!IsSkipEnabled())
        return false;

    if(m_cur.MbPart == ME_Mb16x16)
    {
        return EstimateSkip16x16();
    }

    if(m_cur.MbPart == ME_Mb8x8)
    {
        if(m_cur.BlkIdx > 0)
            return false;
        return EstimateSkip8x8();
    }
    return false;
}

bool MeVC1::EstimateSkip16x16()
{
    Ipp32s i, j;
    Ipp32s tmpCost;
    bool OutBoundF = false;
    bool OutBoundB = false;
        //P & B
    for(i=0; i< m_par->FRefFramesNum; i++){
        SetReferenceFrame(frw,i);
        m_cur.PredMV[frw][i][m_cur.BlkIdx]=m_PredCalc->GetPrediction();
        tmpCost = EstimatePoint(m_cur.MbPart, m_par->PixelType, m_par->SkippedMetrics, m_cur.PredMV[frw][i][m_cur.BlkIdx]);
        if(m_cur.SkipCost[0] > tmpCost)
        {
            m_cur.SkipCost[0]     = tmpCost;
            m_cur.SkipType[0]     = ME_MbFrwSkipped;
            m_cur.SkipIdx[frw][0] = i;
        }
        OutBoundF = (tmpCost == ME_BIG_COST ? true: false);

        //B
        if(m_par->SearchDirection ==ME_BidirSearch){
            for(j=0; j<m_par->BRefFramesNum; j++){

                SetReferenceFrame(bkw,j);
                m_cur.PredMV[bkw][j][m_cur.BlkIdx] = m_PredCalc->GetPrediction();
                tmpCost = EstimatePoint(m_cur.MbPart, m_par->PixelType, m_par->SkippedMetrics, m_cur.PredMV[bkw][j][0]);
                if(tmpCost<m_cur.SkipCost[m_cur.BlkIdx]){
                    m_cur.SkipCost[0]     = tmpCost;
                    m_cur.SkipType[0]     = ME_MbBkwSkipped;
                    m_cur.SkipIdx[bkw][0] = j;
                }
                OutBoundB = (tmpCost == ME_BIG_COST ? true: false);

                if(!OutBoundF && !OutBoundB)
                {
                    tmpCost = EstimatePointAverage(m_cur.MbPart, m_par->PixelType, m_par->SkippedMetrics, frw, 0, m_cur.PredMV[frw][i][0], bkw, 0, m_cur.PredMV[bkw][j][0]);
                    tmpCost +=32;
                    if(tmpCost<m_cur.SkipCost[0]){
                        m_cur.SkipCost[0]     = tmpCost;
                        m_cur.SkipType[0]     = ME_MbBidirSkipped;
                        m_cur.SkipIdx[bkw][0] = j;
                    }
                }
            }//for(j)
        }//B
    }//for(i)
    return EarlyExitForSkip();
}

bool MeVC1::EstimateSkip8x8()
{
    Ipp32s i;
    Ipp32s tmpCostF[ME_NUM_OF_BLOCKS];
    Ipp32s tmpCostB[ME_NUM_OF_BLOCKS];
    Ipp32s tmpIdxF[ME_NUM_OF_BLOCKS];
    Ipp32s tmpIdxB[ME_NUM_OF_BLOCKS];
    Ipp32s tmpCost;

    for(i = 0; i < ME_NUM_OF_BLOCKS; i++)
    {
        tmpCostF[i] = tmpCostB[i] = ME_BIG_COST;
    }

    //P & B
    for(m_cur.BlkIdx=0; m_cur.BlkIdx<4; m_cur.BlkIdx++)
    {
        for(i=0; i< m_par->FRefFramesNum; i++)
        {
            SetReferenceFrame(frw,i);
            m_cur.PredMV[frw][i][m_cur.BlkIdx] = m_PredCalc->GetPrediction();
            tmpCost = EstimatePoint(m_cur.MbPart, m_par->PixelType, m_par->SkippedMetrics, m_cur.PredMV[frw][i][m_cur.BlkIdx]);
            if(tmpCost < tmpCostF[m_cur.BlkIdx])
            {
                tmpCostF[m_cur.BlkIdx] = tmpCost;
                tmpIdxF[m_cur.BlkIdx]  = i;
            }

        }//for(i)
        if(!EarlyExitForSkip())
        {
            for(i=0; i<ME_NUM_OF_BLOCKS; i++)
                tmpCostF[i]=ME_BIG_COST;
            break;
        }
    }//for(m_cur.BlkIdx)

    if(m_par->SearchDirection ==ME_BidirSearch)
    {
        for(m_cur.BlkIdx=0; m_cur.BlkIdx<4; m_cur.BlkIdx++)
        {
            for(i=0; i< m_par->BRefFramesNum; i++)
            {
                SetReferenceFrame(bkw,i);
                m_cur.PredMV[bkw][i][m_cur.BlkIdx] = m_PredCalc->GetPrediction();
                tmpCost = EstimatePoint(m_cur.MbPart, m_par->PixelType, m_par->SkippedMetrics, m_cur.PredMV[frw][i][m_cur.BlkIdx]);
                if(tmpCost < tmpCostB[m_cur.BlkIdx])
                {
                    tmpCostB[m_cur.BlkIdx] = tmpCost;
                    tmpIdxB[m_cur.BlkIdx]  = i;
                }

            }//for(i)
            if(!EarlyExitForSkip())
            {
                for(i=0; i<ME_NUM_OF_BLOCKS; i++)
                    tmpCostB[i]=ME_BIG_COST;
                break;
            }
        }//for(m_cur.BlkIdx)
    }//if(m_par->SearchDirection == ME_BidirSearch)

    m_cur.BlkIdx = 0;

    if(tmpCostF[0] == ME_BIG_COST && tmpCostB[0] == ME_BIG_COST)
        return false;

    if(tmpCostF[0] != ME_BIG_COST && tmpCostB[0] != ME_BIG_COST)
    {
        Ipp32s sumF = tmpCostF[0] + tmpCostF[1] + tmpCostF[2] + tmpCostF[3];
        Ipp32s sumB = tmpCostB[0] + tmpCostB[1] + tmpCostB[2] + tmpCostB[3];

        if(sumF < sumB)
        {
            for(i=0; i<ME_NUM_OF_BLOCKS; i++)
            {
                m_cur.SkipCost[i]     = tmpCostF[i];
                m_cur.SkipType[i]     = ME_MbFrwSkipped;
                m_cur.SkipIdx[frw][i] = tmpIdxF[i];
            }
        }
        else
        {
            for(i=0; i<ME_NUM_OF_BLOCKS; i++)
            {
                m_cur.SkipCost[i]     = tmpCostB[i];
                m_cur.SkipType[i]     = ME_MbBkwSkipped;
                m_cur.SkipIdx[bkw][i] = tmpIdxB[i];
            }
        }
        return true;
    }

    if(tmpCostF[0] != ME_BIG_COST)
    {
        for(i=0; i<ME_NUM_OF_BLOCKS; i++)
        {
            m_cur.SkipCost[i]     = tmpCostF[i];
            m_cur.SkipType[i]     = ME_MbFrwSkipped;
            m_cur.SkipIdx[frw][i] = tmpIdxF[i];
        }
        return true;
    }

    if(tmpCostB[0] != ME_BIG_COST)
    {
        for(i=0; i<ME_NUM_OF_BLOCKS; i++)
        {
            m_cur.SkipCost[i]     = tmpCostB[i];
            m_cur.SkipType[i]     = ME_MbBkwSkipped;
            m_cur.SkipIdx[bkw][i] = tmpIdxB[i];
        }
    }

    return true;
}//EstimateSkip8x8()

///Function select best transforming type for inter block.
MeCostRD MeVC1::MakeTransformDecisionInter(MeMbPart mt, MePixelType pix, MeMV mv)
{
    SetError((vm_char*)"Wrong SearchDirection in MakeTransformDecisionInter", m_par->SearchDirection != ME_ForwardSearch);
    SetError((vm_char*)"Wrong MbPart in MakeTransformDecisionInter", mt != ME_Mb16x16);
    SetError((vm_char*)"Wrong RefFrame in MakeTransformDecisionInter", m_cur.RefDir != frw || m_cur.RefIdx!=0||m_cur.BlkIdx!=0);

    //calculate adresses
    MeCostRD res=0;
    MeAdr src, ref; src.chroma=ref.chroma=true; //true means load also chroma plane addresses
    MakeRefAdrFromMV(mt, pix, mv, &ref);
    MakeSrcAdr(mt, pix, &src);

    //calculate transforming costs
    MeTransformType tr[4]={ME_Tranform8x8, ME_Tranform8x4, ME_Tranform4x8,ME_Tranform4x4};
    MeCostRD c[6][4]; 
    for(Ipp32s blk=0; blk<6; blk++){
        m_cur.BlkIdx = blk; //it is for DC prediction calculation
        if(blk<4){
            //luma
            src.channel=Y;
            for(Ipp32s t=0; t<4; t++){
                c[blk][t] = GetCostRD(ME_InterRD,ME_Mb8x8,tr[t],&src,&ref);
            }
            Ipp32s dx=8, dy=0;
            if(blk==1){dx=-dx; dy=8;}
            src.move(Y,dx,dy);
            ref.move(Y,dx,dy);
        }else{
            //chroma
            src.channel=(blk==4?U:V);
            c[blk][0] = GetCostRD(ME_InterRD,ME_Mb8x8,ME_Tranform8x8,&src,&ref);
        }
    }
    m_cur.BlkIdx =0; // TODO: restore original for 8x8 block

    m_cur.InterTransf[4] = ME_Tranform8x8;
    m_cur.InterTransf[5] = ME_Tranform8x8;
    Ipp32s BestCost=ME_BIG_COST;
    Ipp32s b0 = 0, b1 = 0, b2 = 0, b3 = 0;
    for(Ipp32s i0=0; i0<4; i0++){ //cycle for all transform types
        for(Ipp32s i1=0; i1<4; i1++){
            for(Ipp32s i2=0; i2<4; i2++){
                for(Ipp32s i3=0; i3<4; i3++){
                    Ipp32s CurCost=c[0][i0].J+c[1][i1].J+c[2][i2].J+c[3][i3].J;
                    if(CurCost>=BestCost)continue;
                    m_cur.InterTransf[0] = tr[i0];
                    m_cur.InterTransf[1] = tr[i1];
                    m_cur.InterTransf[2] = tr[i2];
                    m_cur.InterTransf[3] = tr[i3];
                    res=0;
                    res+=c[0][i0];
                    res+=c[1][i1];
                    res+=c[2][i2];
                    res+=c[3][i3];
                    res+=c[4][0];
                    res+=c[5][0];
                    AddHeaderCost(res, m_cur.InterTransf, ME_MbFrw);
                    CurCost=(Ipp32s)(res.D+m_lambda*res.R);
                    if(CurCost<BestCost){
                        BestCost=CurCost;
                        b0=i0; b1=i1;b2=i2;b3=i3;
                    }
                }
            }
        }
    }

    m_cur.InterTransf[0] = tr[b0];
    m_cur.InterTransf[1] = tr[b1];
    m_cur.InterTransf[2] = tr[b2];
    m_cur.InterTransf[3] = tr[b3];
    res=0;
    res+=c[0][b0];
    res+=c[1][b1];
    res+=c[2][b2];
    res+=c[3][b3];
    res+=c[4][0];
    res+=c[5][0];

    //add header cost
    AddHeaderCost(res, m_cur.InterTransf, ME_MbFrw);

    return res;
}

Ipp8u MeVC1::GetCoeffMode( Ipp32s &run, Ipp32s &level, const Ipp8u *pTableDR, const Ipp8u *pTableDL)
{
    bool sign  = level < 0;
    level = (sign)? -level : level;
    if (run <= pTableDR[1])
    {
        Ipp8u maxLevel = pTableDL[run];
        if (level <= maxLevel)
        {
            return 0;
        }
        if (level <= 2*maxLevel)
        {
            level = level - maxLevel;
            return 1;
        }
    }
    if (level <= pTableDL[0])
    {
        Ipp8u maxRun = pTableDR[level];
        if (run <= (Ipp8u)(2*maxRun + 1))
        {
            run = run - (Ipp8u)maxRun - 1;
            return 2;
        }
    }
    return 3;
}

//DC here is predicted value, only for Intra
Ipp32s MeVC1::GetAcCoeffSize(Ipp16s *ptr, Ipp32s DC, MeTransformType transf, Ipp32s QP, bool luma, bool intra)
{
    static const int DCEscIdx=119;
    const Ipp32u*  pDC;
    if(luma)pDC=m_par->DcTableLuma;
    else pDC=m_par->DcTableChroma;

    MeACTablesSet *tblSet;
    if(!intra){
        if(luma)tblSet = (MeACTablesSet*)&ACTablesSet[CodingSetsInter[m_cur.qp8][m_cur.AcTableIndex]];
        else tblSet = (MeACTablesSet*)&ACTablesSet[CodingSetsInter[m_cur.qp8][m_cur.AcTableIndex]];
    }else{
        if(luma)tblSet = (MeACTablesSet*)&ACTablesSet[LumaCodingSetsIntra[m_cur.qp8][m_cur.AcTableIndex]];
        else tblSet = (MeACTablesSet*)&ACTablesSet[ChromaCodingSetsIntra[m_cur.qp8][m_cur.AcTableIndex]];
    }

    const Ipp8u *scan = m_par->ScanTable[0];
    Ipp32s NumOfSubBlocks=1;
    Ipp32s SubBlockSize=64;
    switch(transf){
        case ME_Tranform4x4:
            NumOfSubBlocks=4;
            scan = m_par->ScanTable[3];
            SubBlockSize=16;
            break;
        case ME_Tranform4x8:
            NumOfSubBlocks=2;
            scan = m_par->ScanTable[2];
            SubBlockSize=32;
            break;
        case ME_Tranform8x4:
            NumOfSubBlocks=2;
            scan = m_par->ScanTable[1];
            SubBlockSize=32;
            break;
        case ME_Tranform8x8:
            //NumOfSubBlocks=1;
            //scan = m_par->ScanTable[0];
            //SubBlockSize=64;
            break;
    }

    Ipp32s length=0;
    for(Ipp32s sb=0; sb<NumOfSubBlocks; sb++,ptr+=SubBlockSize){
        Ipp32s run=0, level;
        Ipp32s first=(intra?1:0); //don't take intra DC into account
        Ipp32s last=-1;
        Ipp32s i;
        int mode;
        const Ipp32u    *pEnc   = tblSet->pEncTable;
        const Ipp8u     *pDR    = tblSet->pTableDR;
        const Ipp8u     *pDL    = tblSet->pTableDL;
        const Ipp8u     *pIdx   = tblSet->pTableInd;
         
        //process Intra DC
        if(intra){
            DC=abs(DC);
            Ipp32s AddOn=1, EscAddOn=9; 
            if(QP/2==1){DC=(DC+3)>>2;AddOn=3;EscAddOn=11;}
            else if(QP/2==2){DC=(DC+1)>>1;AddOn=2;EscAddOn=10;}

            if(DC==0) length+=pDC[1];
            else if(DC<DCEscIdx) length+=pDC[2*DC+1]+AddOn;
            else length+=pDC[2*DCEscIdx+1]+EscAddOn;
        }

        //find last coeff
        for(i=first; i<SubBlockSize; i++){
            if(ptr[scan[i]]!=0)last=i;
        }

        //process coeff
        for(i=first; i<=last; i++){ 
            if((level=ptr[scan[i]])==0){
                run++;
                continue;
            }
            
            //switch tables
            if(i==last){
                pDR    = tblSet->pTableDRLast;
                pDL    = tblSet->pTableDLLast;
                pIdx   = tblSet->pTableIndLast;
            }

            mode=GetCoeffMode(run, level, pDR, pDL);
            switch(mode){
                case 3:{
                    //write down run and level size, it is performed one time for  frame, field or slice so skip it!
                    //static bool FirstTime = true; //this is for frame, field, slice
                    //if(FirstTime){
                    //    FirstTime=false;
                    //    if(QP<=15) length+=5; //this is for level size, see WriteBlockAC and 7.1.4.10 
                    //    else length+=6;
                    //    length +=2; //this is for run size
                    //}

                    length += pEnc[1] + 2 + 1; //escape + mode + last
                    int LevelSize=8;
                    if(QP<=15) LevelSize=11; //see WriteBlockAC and 7.1.4.10 
                    length+=LevelSize+6 + 1; //6 is for run size, 1 is for level sign
                    break;
                }
                case 2:
                case 1:
                    length += pEnc[1]+mode; //escape
                case 0:
                    Ipp32s index = pIdx[run] + level;
                    length += pEnc[2*index + 1]+1;
                    break;
            }
            run=0;
        }
    }
    return length;
}

Ipp32s MeVC1::GetOneAcCoeffSize(Ipp32s level, Ipp32s run, Ipp32s QP, bool dc, bool luma, bool intra, bool last)
{
    static const int DCEscIdx=119;
    const Ipp32u*  pDC;
    if(luma)pDC=m_par->DcTableLuma;
    else pDC=m_par->DcTableChroma;

    MeACTablesSet *tblSet;
    if(!intra){
        if(luma)tblSet = (MeACTablesSet*)&ACTablesSet[CodingSetsInter[m_cur.qp8][m_cur.AcTableIndex]];
        else tblSet = (MeACTablesSet*)&ACTablesSet[CodingSetsInter[m_cur.qp8][m_cur.AcTableIndex]];
    }else{
        if(luma)tblSet = (MeACTablesSet*)&ACTablesSet[LumaCodingSetsIntra[m_cur.qp8][m_cur.AcTableIndex]];
        else tblSet = (MeACTablesSet*)&ACTablesSet[ChromaCodingSetsIntra[m_cur.qp8][m_cur.AcTableIndex]];
    }

    Ipp32s length=0;
    int mode;

    const Ipp32u    *pEnc   = tblSet->pEncTable;
    const Ipp8u     *pDR    = tblSet->pTableDR;
    const Ipp8u     *pDL    = tblSet->pTableDL;
    const Ipp8u     *pIdx   = tblSet->pTableInd;

    if(last){
        pDR    = tblSet->pTableDRLast;
        pDL    = tblSet->pTableDLLast;
        pIdx   = tblSet->pTableIndLast;
    }
         
    if(intra && dc){
        //process Intra DC
        //1 don't take DC rate into account for now due to predicition
        level = 1;
        level=abs(level);
        Ipp32s AddOn=1, EscAddOn=9; 
        if(QP/2==1){level=(level+3)>>2;AddOn=3;EscAddOn=11;}
        else if(QP/2==2){level=(level+1)>>1;AddOn=2;EscAddOn=10;}

        if(level==0) length+=pDC[1];
        else if(level<DCEscIdx) length+=pDC[2*level+1]+AddOn;
        else length+=pDC[2*DCEscIdx+1]+EscAddOn;
    }else{
        //inter
        mode=GetCoeffMode(run, level, pDR, pDL);
        switch(mode){
            case 3:{
                //write down run and level size, it is performed one time for  frame, field or slice so skip it!
                //static bool FirstTime = true; //this is for frame, field, slice
                //if(FirstTime){
                //    FirstTime=false;
                //    if(QP<=15) length+=5; //this is for level size, see WriteBlockAC and 7.1.4.10 
                //    else length+=6;
                //    length +=2; //this is for run size
                //}

                length += pEnc[1] + 2 + 1; //escape + mode + last
                int LevelSize=8;
                if(QP<=15) LevelSize=11; //see WriteBlockAC and 7.1.4.10 
                length+=LevelSize+6 + 1; //6 is for run size, 1 is for level sign
                break;
            }
            case 2:
            case 1:
                length += pEnc[1]+mode; //escape
            case 0:
                Ipp32s index = pIdx[run] + level;
                length += pEnc[2*index + 1]+1;
                break;
        }
        run=0;
    }
    return length;
}

#define DecreaseLevel(x,n) (((x)<0?(x)+(n):(x)-(n)))
#define DivAndRound(x,d) (x<0?(x-d/2)/d:(x+d/2)/d)
#define SaveNode(x) \
                        cur=&m_cur.trellis[c][CurNodeIdx++];\
                        *cur=(*(x));\
                        cur->level=level;\
                        cur->run++;\
                        cur->cost+=NormDif;\
                        cur->prev=(x);

void MeVC1::QuantTrellis(Ipp16s *ptr, Ipp32s /*DC*/, MeTransformType transf, Ipp32s QP, bool luma, bool intra)
{
    const Ipp32s scaling=16;
    const Ipp32s norm = (Ipp32s)(1.0*scaling); //it is to cast frequency domain distortion to spatial and to account for IntLambda scaling, 1.0 is good enough
    const Ipp32s IntLambda = (Ipp32s)(scaling*m_lambda);
    Ipp16s coeff[64];
    Ipp16s quant[64];
    Ipp32s SubBlockSize=64;
    const Ipp8u *scan = m_par->ScanTable[0];
    switch(transf){
        case ME_Tranform4x4:
            scan = m_par->ScanTable[3];
            SubBlockSize=16;
            break;
        case ME_Tranform4x8:
            scan = m_par->ScanTable[2];
            SubBlockSize=32;
            break;
        case ME_Tranform8x4:
            scan = m_par->ScanTable[1];
            SubBlockSize=32;
            break;
        case ME_Tranform8x8:
            //scan = m_par->ScanTable[0];
            //SubBlockSize=64;
            break;
    }

    //rearrange coefficient according to scan matrix
    for(Ipp32s i=0; i<SubBlockSize; i++)
        coeff[i] = ptr[scan[i]];
    
    //init trellis
    //set two start points, first one for "not last" node, second for "last" in VC1 VLC AC coefficients coding sense
    m_cur.trellis[0][0].Reset();
    m_cur.trellis[0][1].Reset();
    m_cur.trellis[0][1].last=true;
    
    for(Ipp32s c=1; c<SubBlockSize+1; c++){ //for all coeff
        MeTrellisNode BestLast;
        BestLast.Reset();
        BestLast.last=true;
        Ipp32s BestLastCost=IPP_MAX_32S;
        Ipp32s CurNodeIdx=0;
        MeTrellisNode *cur = 0;
        MeTrellisNode *prev;
        
        //3 for all levels of current coeff
        Ipp32s level=DivAndRound(coeff[c-1],QP);
        Ipp32s OrgLevel=level;
        for(Ipp32s n=0; n<NumOfLevelInTrellis; n++){
            Ipp32s dif=coeff[c-1] - QP*level;
            Ipp32s NormDif=norm*dif*dif;
           
            if(level == 0){
                if(OrgLevel!=0){
                        //3 coeff is zero
                        //copy all previous nodes excluding last, and nodes whose neighbour is better
                        //neighbour is better if its cost is less or eqaul and run is less or equal
                        MeTrellisNode*a=NULL, *b=NULL;
                        for(Ipp32s p=0; m_cur.trellis[c-1][p].active && !m_cur.trellis[c-1][p].last; p++){
                            if(a==NULL){
                                a=&m_cur.trellis[c-1][p];
                                continue;
                            }
                            b=&m_cur.trellis[c-1][p];

                            if(a->cost<=b->cost && a->run <=b->run){
                                //leave a
                                SaveNode(a);
                            }else if(a->cost>=b->cost && a->run>=b->run){
                                //leave b
                                SaveNode(b);
                                a=b;
                            }else{
                                //leave both
                                SaveNode(a);
                                a=b;
                                b=NULL;
                            }
                        }
                        if(b==NULL && a!=NULL) //there was only one previous node, save it
                            SaveNode(a);
                    }else{
                    //copy all previous nodes excluding last and increase their run and cost
                    for(Ipp32s p=0; m_cur.trellis[c-1][p].active && !m_cur.trellis[c-1][p].last; p++){
                        cur=&m_cur.trellis[c][CurNodeIdx++];
                        *cur=m_cur.trellis[c-1][p];
                        cur->level=level;
                        cur->run++;
                        cur->cost+=NormDif;
                        cur->prev=&m_cur.trellis[c-1][p];
                    }
                }
                break; //zero level was processed, there is nothing more to do, go to "last" node processing
            }else{
                //3 coeff is not zero
                //find best path
                Ipp32s BestCost=IPP_MAX_32S;
                //for all previous points
                cur=&m_cur.trellis[c][CurNodeIdx++];
                cur->Reset();
                cur->level=level;
                for(Ipp32s p=0; m_cur.trellis[c-1][p].active && !m_cur.trellis[c-1][p].last; p++){
                    //"not last" path
                    prev = &m_cur.trellis[c-1][p];
                    Ipp32s cost=NormDif+IntLambda*GetOneAcCoeffSize(level, prev->run, QP, c==1, luma, intra, false) + prev->cost;
                    if(cost<BestCost){
                        BestCost=cost; 
                        cur->cost=cost;
                        cur->prev=prev;
                    }
                    //"last" path
                    Ipp32s LastCost=NormDif+IntLambda*GetOneAcCoeffSize(level, prev->run, QP, c==1, luma, intra, true) + prev->cost;
                    if(LastCost<BestLastCost){
                        BestLastCost=LastCost; 
                        BestLast.level=level;
                        BestLast.cost=LastCost;
                        BestLast.prev=prev;
                    }
                }
            }
            level = DecreaseLevel(level,1);
        }

        //compare previous last with current last, leave the best
        cur=&m_cur.trellis[c][CurNodeIdx++];
        Ipp32s p;
        for(p=0; !m_cur.trellis[c-1][p].last; p++);
        if(m_cur.trellis[c-1][p].cost+norm*coeff[c-1]*coeff[c-1] < BestLastCost){
            //prev is better
            cur->active = true;
            cur->last = true;
            cur->level = 0;
            cur->cost =m_cur.trellis[c-1][p].cost+norm*coeff[c-1]*coeff[c-1];
            cur->prev = &m_cur.trellis[c-1][p];
        }else{
            //new is better
            *cur = BestLast;
        }
    }

    //find best "last" in last collumn, it is optimal solution 
    Ipp32s p;
    for(p=0; !m_cur.trellis[SubBlockSize][p].last; p++);
    MeTrellisNode *prev = &m_cur.trellis[SubBlockSize][p];

    for(int x=SubBlockSize-1; x>=0; x--){
        quant[x] = (Ipp16s)prev->level;
        prev=prev->prev;
    }

    //perform inverse scan
    for(Ipp32s i=0; i<SubBlockSize; i++)
        ptr[scan[i]]=quant[i]; 
}


void MeVC1::AddHeaderCost(MeCostRD &cost, MeTransformType *transf, MeMbType MbType)
{
    const Ipp16u*  pCBPCYTable=VLCTableCBPCY_PB[m_cur.CbpcyTableIndex];

    //Intra
    if(MbType==ME_MbIntra){
        Ipp32s         NotSkip =   (cost.BlkPat != 0)?1:0;
        const Ipp16u*   MVDiffTables=MVDiffTablesVLC[m_cur.MvTableIndex];
        cost.R+=MVDiffTables[2*(37*NotSkip + 36 - 1)+1];
        if(cost.BlkPat!=0){
            cost.R+=pCBPCYTable[2*cost.BlkPat+1];
        }
        return;
    }

    //Inter and there is no variable size transforming
    if((MbType==ME_MbFrw || MbType==ME_MbBkw || MbType==ME_MbBidir || MbType==ME_MbDirect)&& transf==NULL){
        if(cost.BlkPat!=0){
            cost.R+=pCBPCYTable[2*cost.BlkPat+1];
        }
        if(m_par->SearchDirection != ME_BidirSearch)
        return;
    }

    if(m_par->SearchDirection == ME_BidirSearch)
    {
        //temporary: Bfraction shold be transmitted!!!:
        Ipp32f Bfraction = 0.5;
        switch(MbType)
        {
        case ME_MbFrw:
            Bfraction < 0.5? cost.R+=3: cost.R+=4;
            break;
        case ME_MbBkw:
            Bfraction < 0.5? cost.R+=4: cost.R+=3;
            break;
        case ME_MbBidir:
            cost.R+=4;
            break;
        case ME_MbDirect:
            cost.R+=2;
            break;
        case ME_MbFrwSkipped:
            Bfraction < 0.5? cost.R=3: cost.R=4;
            break;
        case ME_MbBkwSkipped:
            Bfraction < 0.5? cost.R=4: cost.R=3;
            break;
        case ME_MbBidirSkipped:
            cost.R=4;
            break;
        case ME_MbDirectSkipped:
            cost.R=2;
            break;
        default:
            SetError((vm_char*)"Wrong macroblock type");
            break;
        }
        return;
    }//if(m_par->SearchDirection == ME_BidirSearch)

    //Inter, forward search:
    if(cost.BlkPat!=0){
        //3 CBPCY
        cost.R+=pCBPCYTable[2*cost.BlkPat+1];


        //3 VSTSMB
        const Ipp16s (*pTTMBVLC)[4][6]   = NULL;
        const Ipp8u  (* pTTBlkVLC)[6]    = NULL;
        const Ipp8u   *pSubPattern4x4VLC = NULL;

        if (m_par->Quant/2<5)
        {
            pTTMBVLC            =  TTMBVLC_HighRate;
            pTTBlkVLC           =  TTBLKVLC_HighRate;
            pSubPattern4x4VLC   =  SubPattern4x4VLC_HighRate;
        }
        else if (m_par->Quant/2<13)
        {
            pTTMBVLC            =  TTMBVLC_MediumRate;
            pTTBlkVLC           =  TTBLKVLC_MediumRate;
            pSubPattern4x4VLC   =  SubPattern4x4VLC_MediumRate;

        }
        else
        {
            pTTMBVLC            =  TTMBVLC_LowRate;
            pTTBlkVLC           =  TTBLKVLC_LowRate;
            pSubPattern4x4VLC   =  SubPattern4x4VLC_LowRate;
        }

        Ipp32s MbLevelTransf=(transf[0]==transf[1]&&transf[0]==transf[2]&&transf[0]==transf[3]&&transf[0]==transf[4]&& transf[0]==transf[5])?1:0;

        Ipp32s blk;
        for(blk=0; blk<6; blk++)
            if( cost.BlkPat & (1<<(5-blk)) ) break;
        Ipp32s FirstCodedBlk = blk;
        MeTransformType tr=transf[blk];    
    
        Ipp32s  subPat  = 0x3&(cost.SbPat>>(4*(5-blk))); //for 8x8 and 4x4 pattern doesn't matter so do it only for 8x4 4x8
        if(--subPat<0)subPat=0;

        cost.R+=pTTMBVLC[MbLevelTransf][tr][2*subPat+1];


        //3 VSTSBLK
        for(blk=0; blk<6; blk++){
            if((cost.BlkPat & (1<<(5-blk))) ==0)continue;
            tr=transf[blk]; 
            subPat  = 0xf&(cost.SbPat>>(4*(5-blk)));

            if (!MbLevelTransf && blk!= FirstCodedBlk){
                //block level transform type
                if(--subPat<0)subPat=0;
                cost.R+=pTTBlkVLC[tr][2*subPat+1];
            }
            subPat  = 0xf&(cost.SbPat>>(4*(5-blk)));
            if (tr == ME_Tranform4x4){
                cost.R+=pSubPattern4x4VLC[2*subPat+1];
            }
            else if (tr != ME_Tranform8x8 &&  (MbLevelTransf && blk!= FirstCodedBlk)){
                //if MB level or frame level
                subPat&=3;
                if(--subPat<0)subPat=0;
                cost.R+=SubPattern8x4_4x8VLC[2*subPat+1];
            }
        }
    }

}

Ipp32s MeVC1::GetMvSize(Ipp32s dx, Ipp32s dy)
{
    Ipp32s rangeIndex=m_par->MVRangeIndex;
    Ipp32s m_bMVHalf = m_par->PixelType == ME_HalfPixel ? true : false;
    static const int _VC1_ENC_HALF_MV_LIMIT=95;
    static const int _VC1_ENC_MV_LIMIT=159;
    Ipp32s NotSkip=(m_par->pSrc->MBs[m_cur.adr].NumOfNZ==0?0:1);

    if(dx==0 && dy==0 && NotSkip==0)
        return 0; //special case, residual equal to 0, mv equal to 0 - pure skip

    const Ipp16u* table = MVDiffTablesVLC[m_cur.MvTableIndex];;
    const Ipp8u* MVSizeOffset = m_par->MVSizeOffset;
    const Ipp8u* longMVLength=m_par->MVLengthLong;

    Ipp32s length=0;

    Ipp16s          index   =   0;
    bool            signX   =   (dx<0);
    bool            signY   =   (dy<0);
    Ipp8u           limit   =   (m_bMVHalf)? _VC1_ENC_HALF_MV_LIMIT : _VC1_ENC_MV_LIMIT;

    dx = dx*(1 -2*signX);
    dy = dy*(1 -2*signY);

    if (m_bMVHalf)
    {
        dx = dx >> 1;
        dy = dy >> 1;
    }

    index = (dx < limit && dy < limit)? 6*MVSizeOffset[3*dy]+ MVSizeOffset[3*dx] - 1:35;

    if (index < 34)
    {
        Ipp32s diffX = dx - MVSizeOffset[3*dx+1];
        Ipp32s diffY = dy - MVSizeOffset[3*dy+1];
        Ipp8u sizeX = MVSizeOffset[3*dx+2];
        Ipp8u sizeY = MVSizeOffset[3*dy+2];

        diffX =  (diffX<<1)+signX;
        diffY =  (diffY<<1)+signY;

        if (m_bMVHalf)
        {
            sizeX -= sizeX>>3;
            sizeY -= sizeY>>3;
        }
        index = (Ipp16s)(index+37*NotSkip);
        length += table[2*index+1] +sizeX+sizeY;
    }
    else
    {
         // escape mode
        Ipp8u sizeX = longMVLength[2*rangeIndex];
        Ipp8u sizeY = longMVLength[2*rangeIndex+1];

        index= (Ipp16s)(35 + 37*NotSkip - 1);

        if (m_bMVHalf)
        {
            sizeX -= sizeX>>3;
            sizeY -= sizeY>>3;
        }
        length += table[2*index+1]+sizeX+sizeY;
    }
    return length;
}


MeCostRD MeVC1::GetCostRD(MeDecisionMetrics CostMetric, MeMbPart mt, MeTransformType transf, MeAdr* src, MeAdr* ref)
{
    SetError((vm_char*)"Wrong metric in MeVC1::GetCostRD", CostMetric!=ME_InterRD && CostMetric!=ME_IntraRD);
    SetError((vm_char*)"Wrong partitioning in MeVC1::GetCostRD", (CostMetric==ME_InterRD && mt!=ME_Mb8x8 && mt!=ME_Mb16x16) || (CostMetric==ME_IntraRD && mt!=ME_Mb8x8 && mt!=ME_Mb16x16));

    static const Ipp8u DCQuantValues[32] =
    {
        0,  2,  4,  8,  8,  8,  9,  9,
        10, 10, 11, 11, 12, 12, 13, 13,
        14, 14, 15, 15, 16, 16, 17, 17,
        18, 18, 19, 19, 20, 20, 21, 21
    };

    Ipp32s qp=m_par->Quant;
    bool UnQp=m_par->UniformQuant;
    bool intra = (CostMetric==ME_IntraRD);
    bool luma=m_cur.BlkIdx<4;
    Ipp32s ch=src->channel;

    //1 Inter/Intra 16x16
    if(mt ==ME_Mb16x16){
        MeCostRD res=0;
        for(Ipp32s blk=0; blk<6;blk++){
            src->channel = (blk<4?Y:(blk==4?U:V));
            m_cur.BlkIdx = blk; //it is for DC prediction calculation
            res+=GetCostRD(CostMetric, ME_Mb8x8, transf, src, ref);
            Ipp32s dx=8, dy=0;
            if(blk==1){dx=-dx; dy=8;}
            src->move(Y,dx,dy);
            if(!intra)ref->move(Y,dx,dy);
        }
        m_cur.BlkIdx =0;
        return res;
    }

    //1 Inter/Intra 8x8
    Ipp16s buf1[64]; // TODO: move it to m_cur and allign
    Ipp16s buf2[64];
    Ipp8u rec[64]; //reconstructed block
    IppiSize roi={8,8};
    MeCostRD cost=0;
    Ipp16s dc;
    
    //3 dif
    if(intra)
        ippiConvert_8u16u_C1R(src->ptr[ch],src->step[ch],(Ipp16u*)buf1,16,roi);
    else
        ippiSub8x8_8u16s_C1R(src->ptr[ch],src->step[ch],ref->ptr[ch],ref->step[ch],buf1,16);

    if(intra) for(int i=0; i<64; i++)buf1[i]-=128;  // TODO: add ipp function
    

    if(!m_cur.VlcTableSearchInProgress){
        //3 transforming
        //destination buffer is considered linear here, i.e. one dimensional, so for first 4x4 block coefficients will occupy positions from 0 to 15
        switch(transf){
            case ME_Tranform4x4:
                ippiTransform4x4Fwd_VC1_16s_C1R(buf1+0, 16, buf2+0,8);  
                ippiTransform4x4Fwd_VC1_16s_C1R(buf1+4, 16, buf2+16,8);  
                ippiTransform4x4Fwd_VC1_16s_C1R(buf1+32, 16, buf2+32,8);  
                ippiTransform4x4Fwd_VC1_16s_C1R(buf1+36, 16, buf2+48,8);  
                break;
            case ME_Tranform8x4:
                ippiTransform8x4Fwd_VC1_16s_C1R(buf1, 16, buf2,16);
                ippiTransform8x4Fwd_VC1_16s_C1R(buf1+32, 16, buf2+32,16);
                break;
            case ME_Tranform4x8:
                ippiTransform4x8Fwd_VC1_16s_C1R(buf1, 16, buf2,8);
                ippiTransform4x8Fwd_VC1_16s_C1R(buf1+4, 16, buf2+32,8);
                break;
            case ME_Tranform8x8:
                ippiTransform8x8Fwd_VC1_16s_C1R(buf1, 16, buf2,16);
                break;
        }

        //3 quantize
        dc=buf2[0];
        if(!m_par->UseTrellisQuantization || (!m_par->UseTrellisQuantizationChroma && m_cur.BlkIdx>3)){
            if(UnQp)ippiQuantInterUniform_VC1_16s_C1IR(buf2, 16, qp, roi);
            else ippiQuantInterNonuniform_VC1_16s_C1IR(buf2, 16, qp, roi);
            //if(intra) buf2[0] = (dc-910)/DCQuantValues[qp/2]; //910 accounts for -128 in spatial domain, unfortunately it is not accurate enough
            if(intra) buf2[0] = dc/DCQuantValues[qp/2];
        }else{
            SetError((vm_char*)"Wrong quantization type!!! NonUniform!", !UnQp);
            switch(transf){
                case ME_Tranform4x4:
                    QuantTrellis(buf2+0, 0, transf, qp, luma, intra);
                    QuantTrellis(buf2+16, 0, transf, qp, luma, intra);
                    QuantTrellis(buf2+32, 0, transf, qp, luma, intra);
                    QuantTrellis(buf2+48, 0, transf, qp, luma, intra);
                    break;
                case ME_Tranform8x4:
                    QuantTrellis(buf2, 0, transf, qp, luma, intra);
                    QuantTrellis(buf2+32, 0, transf, qp, luma, intra);
                    break;
                case ME_Tranform4x8:
                    QuantTrellis(buf2, 0, transf, qp, luma, intra);
                    QuantTrellis(buf2+32, 0, transf, qp, luma, intra);
                    break;
                case ME_Tranform8x8:
                    QuantTrellis(buf2, 0, transf, qp, luma, intra);
                    break;
            }
            if(intra) buf2[0] = DivAndRound(dc,DCQuantValues[qp/2]);
        }

        //save quantized coefficients for future use
        if(m_par->UseTrellisQuantization)
            memcpy(m_cur.TrellisCoefficients[transf+(intra?4:0)][m_cur.BlkIdx],buf2,64*sizeof(buf2[0]));
    }else{
        //restore quantized coefficients
        memcpy(buf2,m_cur.TrellisCoefficients[transf+(intra?4:0)][m_cur.BlkIdx],64*sizeof(buf2[0]));
    }

    
    //3 predict DC for Intra
    Ipp32s DcPrediction=0;
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    if(intra){
        mb->DcCoeff[m_cur.BlkIdx]=buf2[0];
        DcPrediction = buf2[0] - GetDcPredictor(m_cur.BlkIdx);
    }
    
    //3 pattern
    //set block and subblock patterns. They are used for rate calculation including MV rate calculation.
    Ipp32s flag[4]={0};
    Ipp16s tmp=buf2[0];
    if(intra)buf2[0]=0;//don't take DC into account for intra
    for(Ipp32s i=0; i<4; i++)
        for(Ipp32s j=0; j<16; j++)
            flag[i]|=buf2[16*i+j];
    buf2[0]=tmp;
    for(Ipp32s i=0; i<4; i++)
        flag[i]=flag[i]==0?0:1;
    switch(transf){
        case ME_Tranform4x4:cost.SbPat=(flag[0]<<3)|(flag[1]<<2)|(flag[2]<<1)|(flag[3]<<0);break;
        case ME_Tranform8x4:
        case ME_Tranform4x8:cost.SbPat=(flag[0]<<1)|(flag[1]<<1)|(flag[2]<<0)|(flag[3]<<0);break;
        case ME_Tranform8x8:cost.SbPat=(flag[0]<<0)|(flag[1]<<0)|(flag[2]<<0)|(flag[3]<<0);break;
    }
        
    if(cost.SbPat!=0){
        cost.NumOfCoeff++;
        cost.BlkPat=(1<<(5-m_cur.BlkIdx));
        cost.SbPat=cost.SbPat<<(4*(5-m_cur.BlkIdx));
    }

    //3 Rate    
    cost.R=GetAcCoeffSize(buf2, DcPrediction, transf, qp, luma, intra);

    //skip distortion calculation for chroma
    if(luma || m_par->UseChromaForMD){
        //3 inverse quantize
        IppiSize DstSizeNZ;
        dc=buf2[0];
        if(UnQp)ippiQuantInvInterUniform_VC1_16s_C1R(buf2, 16, buf2, 16, qp, roi, &DstSizeNZ);
        else ippiQuantInvInterNonuniform_VC1_16s_C1R(buf2, 16, buf2, 16, qp, roi, &DstSizeNZ);
        if(intra) buf2[0] =dc*DCQuantValues[qp/2];

        //3 inv transform
        switch(transf){
            case ME_Tranform4x4:
                ippiTransform4x4Inv_VC1_16s_C1R(buf2+0, 8, buf1+0,16,roi);  
                ippiTransform4x4Inv_VC1_16s_C1R(buf2+16, 8, buf1+4,16,roi);  
                ippiTransform4x4Inv_VC1_16s_C1R(buf2+32, 8, buf1+32,16,roi);  
                ippiTransform4x4Inv_VC1_16s_C1R(buf2+48, 8, buf1+36,16,roi);  
                break;
            case ME_Tranform8x4:
                ippiTransform8x4Inv_VC1_16s_C1R(buf2, 16, buf1,16,roi);
                ippiTransform8x4Inv_VC1_16s_C1R(buf2+32, 16, buf1+32,16,roi);
                break;
            case ME_Tranform4x8:
                ippiTransform4x8Inv_VC1_16s_C1R(buf2, 8, buf1,16,roi);
                ippiTransform4x8Inv_VC1_16s_C1R(buf2+32, 8, buf1+4,16,roi);
                break;
            case ME_Tranform8x8:
                ippiTransform8x8Inv_VC1_16s_C1R(buf2, 16, buf1,16,roi);
                break;
        }
        if(intra) for(int i=0; i<64; i++)buf1[i]+=128; // TODO: add ipp function

        //3 inv dif
        if(intra)
            ippiConvert_16s8u_C1R(buf1,16,rec,8,roi);
        else
            ippiMC8x8_8u_C1(ref->ptr[ch],ref->step[ch],buf1,16,rec,8,0,0);

        //3 Distortion
        ippiSSD8x8_8u32s_C1R(rec,8,src->ptr[ch],src->step[ch],&cost.D,0);
    }
    cost.J=(Ipp32s)(cost.D+m_lambda*cost.R); //This is not exact RD cost calculation because there is no feedback here

    //check overflow
    if(cost.J<0)cost.J=ME_BIG_COST;
    
    return cost;
}

Ipp32s MeVC1::GetDcPredictor(Ipp32s BlkIdx)
{
    // TODO: add chroma here
    //B A
    //C . 
    MeMB *mbs=m_par->pSrc->MBs;
    Ipp32s w=m_par->pSrc->WidthMB;


    MeMB *MbA=&mbs[m_cur.adr-w];
    MeMB *MbB=&mbs[m_cur.adr-w-1];
    MeMB *MbC=&mbs[m_cur.adr-1];

    bool A=(m_cur.y>0 && MbA->MbType==ME_MbIntra);
    bool B=(m_cur.y>0 && m_cur.x>0 && MbB->MbType==ME_MbIntra);
    bool C=(m_cur.x>0 && MbC->MbType==ME_MbIntra);

    const Ipp32s unav = ME_BIG_COST; //available

    Ipp32s PredA = unav, PredB=unav, PredC=unav;


    switch(BlkIdx)
    {
    case 0:
        if(A) PredA = mbs[m_cur.adr-w].DcCoeff[2];
        if(B) PredB = mbs[m_cur.adr-w-1].DcCoeff[3];
        if(C) PredC = mbs[m_cur.adr-1].DcCoeff[1];
        break;
    case 1:
        if(A) PredA = mbs[m_cur.adr-w].DcCoeff[3];
        if(A) PredB = mbs[m_cur.adr-w].DcCoeff[2];
        PredC = mbs[m_cur.adr].DcCoeff[0];
        break;
    case 2:
        PredA = mbs[m_cur.adr].DcCoeff[0];
        if(C) PredB = mbs[m_cur.adr-1].DcCoeff[1];
        if(C) PredC = mbs[m_cur.adr-1].DcCoeff[3];
        break;
    case 3:
        PredA = mbs[m_cur.adr].DcCoeff[1];
        PredB = mbs[m_cur.adr].DcCoeff[0];
        PredC = mbs[m_cur.adr].DcCoeff[2];
        break;
    case 4:
    case 5:
        if(A) PredA = mbs[m_cur.adr-w].DcCoeff[BlkIdx];
        if(B) PredB = mbs[m_cur.adr-w-1].DcCoeff[BlkIdx];
        if(C) PredC = mbs[m_cur.adr-1].DcCoeff[BlkIdx];
        break;
    }

    Ipp32s pred;
    if(PredB==unav)PredB=0;
    if ((PredA!=unav) && (PredC!=unav)){
        if(abs(PredB-PredA)<=abs(PredB-PredC)) {
            pred=PredC;
        }else {
            pred=PredA;
        }
    }else if (PredA!=unav) {
        pred=PredA;
    }else if (PredC!=unav) {
        pred=PredC;
    }else {
        pred=0;
    }

    return pred;
       
}

MeMV MeVC1::GetChromaMV (MeMV mv)
{
    MeMV cmv;
    if(m_par->FastChroma) GetChromaMVFast(mv, &cmv);
    else GetChromaMV (mv, &cmv); 
    return cmv;
}

void MeVC1::GetChromaMV(MeMV LumaMV, MeMV* pChroma)
{
    static Ipp16s round[4]= {0,0,0,1};

    pChroma->x = (LumaMV.x + round[LumaMV.x&0x03])>>1;
    pChroma->y = (LumaMV.y + round[LumaMV.y&0x03])>>1;
}

void MeVC1::GetChromaMVFast(MeMV LumaMV, MeMV * pChroma)
{
    static Ipp16s round [4]= {0,0,0,1};
    static Ipp16s round1[2][2] = {
        {0, -1}, //sign = 0;
        {0,  1}  //sign = 1
    };

    pChroma->x = (LumaMV.x + round[LumaMV.x&0x03])>>1;
    pChroma->y = (LumaMV.y + round[LumaMV.y&0x03])>>1;
    pChroma->x = pChroma->x + round1[pChroma->x < 0][pChroma->x & 1];
    pChroma->y = pChroma->y + round1[pChroma->y < 0][pChroma->y & 1];
}

}

#endif
