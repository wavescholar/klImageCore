/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//          motion estimation
//
*/

#ifndef _UMC_ME_VC1_H_
#define _UMC_ME_VC1_H_

#include "umc_me.h"

namespace UMC
{

class MePredictCalculatorVC1 : public MePredictCalculator
{
public:
    DYNAMIC_CAST_DECL(MePredictCalculatorVC1, MePredictCalculator);

    virtual MeMV GetPrediction(MeMV mv);
    virtual MeMV GetPrediction16x16();
    virtual MeMV GetPrediction8x8();

    virtual void ResetPredictors(void)
    {
        for(Ipp32s i = 0; i < MAX_REF; i++)
            for(Ipp32s j = 0; j < 4; j++)
            {
                m_CurPrediction[0][j][i].SetInvalid();
                m_CurPrediction[1][j][i].SetInvalid();
            }
    }

    virtual MeMV GetMvA(){return m_AMV[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx];}
    virtual MeMV GetMvB(){return m_BMV[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx];}
    virtual MeMV GetMvC(){return m_CMV[m_pCur->RefDir][m_pCur->BlkIdx][m_pCur->RefIdx];}
    virtual MeMV GetDef_FieldMV(int RefDir, int BlkIdx)
        {return m_CurPrediction[RefDir][BlkIdx][m_FieldPredictPrefer[RefDir][BlkIdx]];}
    virtual bool GetDef_Field(int RefDir, int BlkIdx)
        {return m_FieldPredictPrefer[RefDir][BlkIdx];}
    virtual void SetDefFrwBkw(MeMV &mvF, MeMV &mvB);

protected:
    typedef bool (MePredictCalculatorVC1::*GetPredictorFunc)(int index,int isBkw, MeMV* res);
    GetPredictorFunc GetPredictor;

    bool GetPredictorMPEG2(int index,int isBkw, MeMV* res);
    bool GetPredictorVC1(int index,int isBkw, MeMV* res);
    bool GetPredictorVC1_hybrid(int index,int isBkw, MeMV* res);
    void GetPredictorVC1_hybrid(MeMV cur, MeMV* res);
    bool GetPredictorVC1Field1(int index, int isBkw, MeMV* res);
    bool GetPredictorVC1Field2(int index, int isBkw, MeMV* res);
    bool GetPredictorVC1Field2Hybrid(int index, int isBkw, MeMV* res);

    void GetBlockVectorsABC_0(int isBkw);
    void GetBlockVectorsABC_1(int isBkw);
    void GetBlockVectorsABC_2(int isBkw);
    void GetBlockVectorsABC_3(int isBkw);
    void GetMacroBlockVectorsABC(int isBkw);
    void GetBlockVectorsABCField_0(int isBkw);
    void GetBlockVectorsABCField_1(int isBkw);
    void GetBlockVectorsABCField_2(int isBkw);
    void GetBlockVectorsABCField_3(int isBkw);
    void GetMacroBlockVectorsABCField(int isBkw);

    MeMV  m_AMV[2][4][MAX_REF]; //current A, the first index = frw, bkw, the second index - block number
    MeMV  m_BMV[2][4][MAX_REF]; //current B, the first index = frw, bkw, the second index - block number
    MeMV  m_CMV[2][4][MAX_REF]; //current C, the first index = frw, bkw, the second index - block number

    //MVs for prediction
    MeMV  m_CurPrediction[2][4][MAX_REF]; //prediction for current MB,the first index = frw, bkw,
                                            //the second index - block number
    bool  m_FieldPredictPrefer[2][4];//for field picture vectors prediction; 0 - for the forward, 1 - for the backward
                                        //the second index - block number
};

class MeVC1 : public MeBase
{
public:
    DYNAMIC_CAST_DECL(MeVC1,MeBase);

    MeVC1()
    {
        m_PredCalc = &m_PredCalcVC1;
    }
    //top level functions
    virtual bool EstimateSkip();

protected:
    MePredictCalculatorVC1 m_PredCalcVC1;

    bool EstimateSkip16x16();
    bool EstimateSkip8x8();
    void SetInterpPixelType();
    virtual MeCostRD MakeTransformDecisionInter(MeMbPart mt, MePixelType pix, MeMV mv);

    Ipp8u GetCoeffMode( Ipp32s &run, Ipp32s &level, const Ipp8u *pTableDR, const Ipp8u *pTableDL);
    Ipp32s GetAcCoeffSize(Ipp16s *ptr, Ipp32s DC, MeTransformType transf, Ipp32s QP, bool luma, bool intra);
    Ipp32s GetOneAcCoeffSize(Ipp32s level, Ipp32s run, Ipp32s QP, bool dc, bool luma, bool intra, bool last);
    void QuantTrellis(Ipp16s *ptr, Ipp32s DC, MeTransformType transf, Ipp32s QP, bool luma, bool intra);
    virtual void AddHeaderCost(MeCostRD &cost, MeTransformType *tansf, MeMbType MbType);
    virtual Ipp32s GetMvSize(Ipp32s dx, Ipp32s dy);
    virtual MeCostRD GetCostRD(MeDecisionMetrics CostMetric, MeMbPart mt, MeTransformType transf, MeAdr* src, MeAdr* ref);

    Ipp32s GetDcPredictor(Ipp32s BlkIdx);
    virtual MeMV GetChromaMV (MeMV mv);
    void GetChromaMV (MeMV LumaMV, MeMV* pChroma);
    void GetChromaMVFast(MeMV LumaMV, MeMV * pChroma);
};

};

#endif
