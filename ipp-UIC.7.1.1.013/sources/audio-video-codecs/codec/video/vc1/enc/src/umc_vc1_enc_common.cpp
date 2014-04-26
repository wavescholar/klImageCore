/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, common declarations
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_def.h"
#include "umc_vc1_common_defs.h"
#include "stdio.h"
#include "assert.h"
#include "umc_vc1_enc_common.h"
#include "umc_vc1_common_enc_tables.h"
#include "umc_vc1_enc_debug.h"


namespace UMC_VC1_ENCODER
{

bool    IsBottomField(bool bTopFieldFirst, bool bSecondField)
{
    return ((bTopFieldFirst && bSecondField) || ((!bTopFieldFirst)&&(!bSecondField)));
}

UMC::Status SetFieldPlane(Ipp8u* pFieldPlane[3],  Ipp8u*  pPlane[3], Ipp32u planeStep[3], bool bBottom)
{
     if(pPlane[0])
        pFieldPlane[0] = pPlane[0]+ bBottom*planeStep[0];
     else
         return UMC::UMC_ERR_NULL_PTR;

     if(pPlane[1])
        pFieldPlane[1] = pPlane[1]+ bBottom*planeStep[1];
     else
         return UMC::UMC_ERR_NULL_PTR;

     if(pPlane[2])
        pFieldPlane[2] = pPlane[2]+ bBottom*planeStep[2];
     else
         return UMC::UMC_ERR_NULL_PTR;

     return UMC::UMC_OK;
}

UMC::Status SetFieldPlane(Ipp8u** pFieldPlane,  Ipp8u*  pPlane, Ipp32u planeStep, bool bBottom)
{
     if(pPlane)
        *pFieldPlane = pPlane+ bBottom*planeStep;
     else
         return UMC::UMC_ERR_NULL_PTR;
     return UMC::UMC_OK;
}

UMC::Status   SetFieldStep(Ipp32u fieldStep[3], Ipp32u planeStep[3])
{
    fieldStep[0] = planeStep[0] * 2;
    fieldStep[1] = planeStep[1] * 2;
    fieldStep[2] = planeStep[2] * 2;

    return UMC::UMC_OK;
}

UMC::Status   SetFieldStep(Ipp32s* fieldStep, Ipp32u planeStep)
{
    fieldStep[0] = planeStep * 2;
    return UMC::UMC_OK;
}

UMC::Status   Set1RefFrwFieldPlane(Ipp8u* pPlane[3],       Ipp32u planeStep[3],
                                   Ipp8u* pFrwPlane[3],    Ipp32u frwStep[3],
                                   Ipp8u* pRaisedPlane[3], Ipp32u raisedStep[3],
                                   eReferenceFieldType     uiReferenceFieldType,
                                   bool bSecondField,      bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    if (bSecondField)
    {
        if (uiReferenceFieldType == VC1_ENC_REF_FIELD_FIRST)
        {
            err = SetFieldPlane(pPlane,  pRaisedPlane, raisedStep, (!bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, raisedStep);
        }
        else
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
    }
    else
    {
        if (uiReferenceFieldType == VC1_ENC_REF_FIELD_FIRST)
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (!bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
        else
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
    }

    return err;
}


UMC::Status   Set1RefFrwFieldPlane(Ipp8u** pPlane,       Ipp32s* planeStep,
                                   Ipp8u* pFrwPlane,    Ipp32u frwStep,
                                   Ipp8u* pRaisedPlane, Ipp32u raisedStep,
                                   eReferenceFieldType     uiReferenceFieldType,
                                   bool bSecondField,      bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    if (bSecondField)
    {
        if (uiReferenceFieldType == VC1_ENC_REF_FIELD_FIRST)
        {
            err = SetFieldPlane(pPlane,  pRaisedPlane, raisedStep, (!bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, raisedStep);
        }
        else
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
    }
    else
    {
        if (uiReferenceFieldType == VC1_ENC_REF_FIELD_FIRST)
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (!bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
        else
        {
            err = SetFieldPlane(pPlane,  pFrwPlane, frwStep, (bBottom));
            VC1_ENC_CHECK(err);
            SetFieldStep(planeStep, frwStep);
        }
    }

    return err;
}

UMC::Status   Set2RefFrwFieldPlane(Ipp8u* pPlane[2][3], Ipp32u planeStep[2][3],
                                   Ipp8u* pFrwPlane[3], Ipp32u frwStep[3],
                                   Ipp8u* pRaisedPlane[3], Ipp32u raisedStep[3],
                                   bool bSecondField, bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    if (bSecondField)
    {
        err = SetFieldPlane(pPlane[0],  pRaisedPlane, raisedStep, (!bBottom));
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep[0], raisedStep);

        err = SetFieldPlane(pPlane[1],  pFrwPlane, frwStep, bBottom);
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep[1], frwStep);
    }
    else
    {
        err = SetFieldPlane(pPlane[0],  pFrwPlane, frwStep, (!bBottom));
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep[0], frwStep);

        err = SetFieldPlane(pPlane[1],  pFrwPlane, frwStep, bBottom);
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep[1], frwStep);
    }
    return err;
}

UMC::Status   Set2RefFrwFieldPlane(Ipp8u** pPlane1, Ipp32s* planeStep1,
                                   Ipp8u** pPlane2, Ipp32s* planeStep2,
                                   Ipp8u* pFrwPlane, Ipp32u frwStep,
                                   Ipp8u* pRaisedPlane, Ipp32u raisedStep,
                                   bool bSecondField, bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    if (bSecondField)
    {
        err = SetFieldPlane(pPlane1,  pRaisedPlane, raisedStep, (!bBottom));
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep1, raisedStep);

        err = SetFieldPlane(pPlane2,  pFrwPlane, frwStep, bBottom);
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep2, frwStep);
    }
    else
    {
        err = SetFieldPlane(pPlane1,  pFrwPlane, frwStep, (!bBottom));
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep1, frwStep);

        err = SetFieldPlane(pPlane2,  pFrwPlane, frwStep, bBottom);
        VC1_ENC_CHECK(err);
        SetFieldStep(planeStep2, frwStep);
    }
    return err;
}

UMC::Status   SetBkwFieldPlane(Ipp8u* pPlane[2][3], Ipp32u planeStep[2][3],
                               Ipp8u* pBkwPlane[3], Ipp32u bkwStep[3],
                               bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    err = SetFieldPlane(pPlane[0],  pBkwPlane, bkwStep, (!bBottom));
    VC1_ENC_CHECK(err);
    SetFieldStep(planeStep[0], bkwStep);

    err = SetFieldPlane(pPlane[1],  pBkwPlane, bkwStep, (bBottom));
    VC1_ENC_CHECK(err);
    SetFieldStep(planeStep[1], bkwStep);

    return err;
}

UMC::Status   SetBkwFieldPlane(Ipp8u** pPlane1, Ipp32s* planeStep1,
                               Ipp8u** pPlane2, Ipp32s* planeStep2,
                               Ipp8u* pBkwPlane, Ipp32u bkwStep,
                               bool bBottom)
{
    UMC::Status err = UMC::UMC_OK;

    err = SetFieldPlane(pPlane1,  pBkwPlane, bkwStep, (!bBottom));
    VC1_ENC_CHECK(err);
    SetFieldStep(planeStep1, bkwStep);

    err = SetFieldPlane(pPlane2,  pBkwPlane, bkwStep, (bBottom));
    VC1_ENC_CHECK(err);
    SetFieldStep(planeStep2, bkwStep);

    return err;
}

UMC::Status Enc_IFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status   err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->type = UMC::ME_FrmIntra;
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}

UMC::Status Enc_I_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status   err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}

UMC::Status Enc_PFrame(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status   err = UMC::UMC_OK;

    if(!(pME->EstimateFrame(MEParams)))
        return UMC::UMC_ERR_INVALID_PARAMS;

    return err;
}

UMC::Status Enc_PFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status   err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->type = UMC::ME_FrmFrw;
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}

UMC::Status Enc_PFrameMixed(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

    if(!(pME->EstimateFrame(MEParams)))
        return UMC::UMC_ERR_INVALID_PARAMS;

    return err;
}

UMC::Status Enc_PFrameUpdateMixed(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->type = UMC::ME_FrmFrw;
    pME->ProcessFeedback(MEParams);
#endif
    return err;
}

UMC::Status     Enc_P_Field(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

    if(!(pME->EstimateFrame(MEParams)))
        return UMC::UMC_ERR_INVALID_PARAMS;

    return err;
}

UMC::Status     Enc_P_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}

UMC::Status Enc_BFrame(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status   err = UMC::UMC_OK;

    if(!(pME->EstimateFrame(MEParams)))
        return UMC::UMC_ERR_INVALID_PARAMS;

      return err;
}

UMC::Status Enc_BFrameUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    MEParams->pSrc->type = UMC::ME_FrmBidir;
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}

UMC::Status     Enc_B_Field(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

    if(!(pME->EstimateFrame(MEParams)))
        return UMC::UMC_ERR_INVALID_PARAMS;

    return err;
}

UMC::Status     Enc_B_FieldUpdate(UMC::MeBase* pME, UMC::MeParams* MEParams)
{
    UMC::Status                 err = UMC::UMC_OK;

#ifdef VC1_ME_MB_STATICTICS
    pME->ProcessFeedback(MEParams);
#endif

    return err;
}
bool IsFieldPicture(ePType PicType)
{
    return ((PicType == VC1_ENC_I_I_FIELD) || (PicType == VC1_ENC_P_I_FIELD)
        || (PicType == VC1_ENC_I_P_FIELD) || (PicType == VC1_ENC_P_P_FIELD)
        || (PicType == VC1_ENC_B_B_FIELD) || (PicType == VC1_ENC_B_BI_FIELD)
        || (PicType == VC1_ENC_BI_B_FIELD) || (PicType == VC1_ENC_BI_BI_FIELD));
}

static const Ipp16s TableFwdTransform8x8[64] =
{
    21845,   21845,  21845,  21845,  21845,  21845,  21845,  21845,
    29026,   27212,  16327,  7257,  -7257,  -16327, -27212, -29026,
    28728,   10773, -10773, -28728, -28728, -10773,  10773,  28728,
    27212,  -7257,  -29026, -16327,  16327,  29026,  7257,  -27212,
    21845,  -21845, -21845,  21845,  21845, -21845, -21845,  21845,
    16327,  -29026,  7257,   27212, -27212, -7257,   29026, -16327,
    10773,  -28728,  28728, -10773, -10773,  28728, -28728,  10773,
    7257,   -16327,  27212, -29026,  29026, -27212,  16327, -7257
};

static const Ipp16s TableFwdTransform4x4[16] =
{
    15420,  15420,      15420,      15420,
    19751,  8978,       -8978,      -19751,
    15420,  -15420,     -15420,     15420,
    8978 ,  -19751,     19751 ,     -8978

};




Ipp8u Get_CBPCY(Ipp32u MBPatternCur, Ipp32u CBPCYTop, Ipp32u CBPCYLeft, Ipp32u CBPCYULeft)
{

    Ipp32u LT3   = (CBPCYULeft >> VC1_ENC_PAT_POS_Y3) & 0x01;
    Ipp32u T2    = (CBPCYTop   >> VC1_ENC_PAT_POS_Y2) & 0x01;
    Ipp32u T3    = (CBPCYTop   >> VC1_ENC_PAT_POS_Y3) & 0x01;
    Ipp32u L1    = (CBPCYLeft  >> VC1_ENC_PAT_POS_Y1) & 0x01;
    Ipp32u L3    = (CBPCYLeft  >> VC1_ENC_PAT_POS_Y3) & 0x01;
    Ipp32u CBPCY = MBPatternCur;
    Ipp32u Y0    = (CBPCY>>VC1_ENC_PAT_POS_Y0)&0x01;


    CBPCY ^=  (Ipp8u)(((LT3==T2)? L1:T2) << VC1_ENC_PAT_POS_Y0);
    CBPCY ^=  (Ipp8u)(((T2 ==T3)? Y0:T3) << VC1_ENC_PAT_POS_Y1);
    CBPCY ^=  (Ipp8u)(((L1 ==Y0)? L3:Y0) << VC1_ENC_PAT_POS_Y2);
    CBPCY ^=  (Ipp8u)(((Y0 ==((MBPatternCur>>VC1_ENC_PAT_POS_Y1)&0x01))?
                (MBPatternCur>>VC1_ENC_PAT_POS_Y2)&0x01:(MBPatternCur>>VC1_ENC_PAT_POS_Y1)&0x01) << VC1_ENC_PAT_POS_Y3);
    return (Ipp8u)CBPCY;
}



Ipp32s SumSqDiff_1x7_16s(Ipp16s* pSrc, Ipp32u step, Ipp16s* pPred)
{
    Ipp32s sum=0;

    for(int i=0; i<7 ;i++)
    {
        pPred  = (Ipp16s*)((Ipp8u*)pPred+step);
        pSrc  = (Ipp16s*)((Ipp8u*)pSrc+step);
        sum  += (pSrc[0]-pPred[0])*(pSrc[0]- pPred[0]) - pSrc[0]*pSrc[0];
    }
    return sum;
}
Ipp32s SumSqDiff_7x1_16s(Ipp16s* pSrc, Ipp16s* pPred)
{
    Ipp32s sum=0;

    for(int i=1; i<8 ;i++)
    {
        sum  += (pSrc[i]-pPred[i])*(pSrc[i]- pPred[i]) - pSrc[i]*pSrc[i];
    }
    return sum;
}
void Diff8x8 (const Ipp16s* pSrc1, Ipp32s src1Step,
              const Ipp16s* pSrc2, Ipp32s src2Step,
              const Ipp8u*  pCoeff,Ipp32s coefStep,
              Ipp16s* pDst,  Ipp32s dstStep)

{
    int i, j;
    for (i = 0; i<8; i++)
    {
        for (j = 0;j<8;j++)
        {
            pDst[j] = pSrc1[j] - pSrc2[j]*pCoeff[j];
        }
        pDst = (Ipp16s*)((Ipp8u*)pDst + dstStep);
        pSrc1 = (Ipp16s*)((Ipp8u*)pSrc1 + src1Step);
        pSrc2 = (Ipp16s*)((Ipp8u*)pSrc2 + src2Step);
        pCoeff  += coefStep;
    }
}

 //---BEGIN------------------------Copy block, MB functions-----------------------------------------------
//---END------------------------Copy block, MB functions-----------------------------------------------

//Ipp32u GetNumZeros(Ipp16s* pSrc, Ipp32u srcStep, bool bIntra)
//{
//    int         i,j;
//    Ipp32u      s=0;
//    Ipp16s*     pBlock = pSrc;
//
//    for(i = 0; i<8; i++)
//    {
//        for (j = bIntra; j<8; j++)
//        {
//            s+= !pBlock[j];
//        }
//        bIntra  = false;
//        pBlock = (Ipp16s*)((Ipp8u*) pBlock + srcStep);
//    }
//    //printf ("intra MB: num of coeff %d\n",64 - s - bIntra);
//    return s;
//}
//Ipp32u GetNumZeros(Ipp16s* pSrc, Ipp32u srcStep)
//{
//    int         i,j;
//    Ipp32u      s=0;
//    Ipp16s*     pBlock = pSrc;
//
//    for(i = 0; i<8; i++)
//    {
//        for (j = 0; j<8; j++)
//        {
//            s+= !pBlock[j];
//        }
//        pBlock = (Ipp16s*)((Ipp8u*) pBlock + srcStep);
//    }
//    //printf ("intra MB: num of coeff %d\n",64 - s);
//    return s;
//}
//Ipp8u GetBlockPattern(Ipp16s* pBlock, Ipp32u step)
//{
//    int i,j;
//    Ipp8u s[4]={0};
//    for (i=0;i<8;i++)
//    {
//        for(j=0;j<8;j++)
//        {
//            s[((i>>2)<<1)+(j>>2)] += !(pBlock[j]);
//        }
//        pBlock = (Ipp16s*)((Ipp8u*)pBlock + step);
//    }
//    //printf ("inter MB: num of coeff %d\n",64 - (s[0]+s[1]+s[2]+s[3]));
//    return (((s[0]<16)<<3)|((s[1]<16)<<2)|((s[2]<16)<<1)| (s[3]<16));
//
//}

Ipp8u GetMode( Ipp8u &run, Ipp16s &level, const Ipp8u *pTableDR, const Ipp8u *pTableDL, bool& sign)
{
    sign  = level < 0;
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
        if (run <= (Ipp8u)(2*maxRun + 1)) // level starts from 0
        {
            run = run - (Ipp8u)maxRun - 1;
            return 2;
        }
    }
    return 3;
}
//bool GetRLCode(Ipp32s run, Ipp32s level, IppVCHuffmanSpec_32s *pTable, Ipp32s &code, Ipp32s &len)
//{
//    Ipp32s  maxRun = pTable[0] >> 20;
//    Ipp32s  addr;
//    Ipp32s  *table;
//
//    if(run > maxRun)
//    {
//       return false;
//    }
//    addr  = pTable[run + 1];
//    table = (Ipp32s*)((Ipp8s*)pTable + addr);
//    if(level <= table[0])
//    {
//        len  = *(table  + level) & 0x1f;
//        code = (*(table + level) >> 16);
//        return true;
//    }
//    return false;
//}
//Ipp8u        Zigzag       ( Ipp16s*                pBlock,
//                            Ipp32u                 blockStep,
//                            bool                   bIntra,
//                            const Ipp8u*           pScanMatrix,
//                            Ipp8u*                 pRuns,
//                            Ipp16s*                pLevels)
//{
//    Ipp32s      i = 0, pos = 0;
//    Ipp16s      value = 0;
//    Ipp8u       n_pairs = 0;
//
//    pRuns[n_pairs]  = 0;
//    for (i = bIntra; i<64; i++)
//    {
//        pos    = pScanMatrix[i];
//        value = *((Ipp16s*)((Ipp8u*)pBlock + blockStep*(pos/8)) + pos%8);
//        if (!value)
//        {
//            pRuns[n_pairs]++;
//        }
//        else
//        {
//            pLevels [n_pairs++] = value;
//            pRuns   [n_pairs]  = 0;
//        }
//
//    }
//    return n_pairs;
//}
//Ipp8u GetLength_16s(Ipp16s value)
//{
//    int    i=0;
//    Ipp16u n = (Ipp16u)value;
//    while (n>>i)
//    {
//        i++;
//    }
//    return i;
//
//}


//---BEGIN------------------------Add constant to block-----------------------------------------------
//---End------------------------Add constant to block-----------------------------------------------

//---Begin------------------------Get difference-----------------------------------------------
//---End------------------------Get difference-----------------------------------------------

//---Begin---------------------Motion compensation-----------------------------------------------
//---End---------------------Motion compensation-----------------------------------------------

Ipp16s median3(Ipp16s a, Ipp16s b, Ipp16s c)
{
  Ipp32u d  = ((((Ipp32u)(a-b))&0x80000000) |
              ((((Ipp32u)(b-c))&0x80000000)>>1)|
              ((((Ipp32u)(a-c))&0x80000000)>>2));

  switch(d)
  {
  case 0x60000000:
  case 0x80000000:
       return a;
  case 0xe0000000:
  case 0x00000000:
       return b;
  case 0x40000000:
  case 0xa0000000:
      return c;
  default:
      assert(0);
  }
  return 0;

}
Ipp16s median4(Ipp16s a, Ipp16s b, Ipp16s c, Ipp16s d)
{
  Ipp32u e  = ((((Ipp32u)(a-b))&0x80000000) |
              ((((Ipp32u)(b-c))&0x80000000)>>1)|
              ((((Ipp32u)(c-d))&0x80000000)>>2)|
              ((((Ipp32u)(a-c))&0x80000000)>>3)|
              ((((Ipp32u)(a-d))&0x80000000)>>4)|
              ((((Ipp32u)(b-d))&0x80000000)>>5));

  switch(e)
  {
  case 0xAC000000:
  case 0xD0000000:
  case 0x2C000000:
  case 0x50000000:
       return (a+b)/2;
  case 0x7C000000:
  case 0x90000000:
  case 0x6C000000:
  case 0x80000000:
       return (a+c)/2;
  case 0x5C000000:
  case 0xA8000000:
  case 0x54000000:
  case 0xA0000000:
       return (a+d)/2;
  case 0xFC000000:
  case 0xBC000000:
  case 0x40000000:
  case 0x00000000:
       return (b+c)/2;
  case 0xDC000000:
  case 0xD8000000:
  case 0x24000000:
  case 0x20000000:
       return (b+d)/2;
  case 0xB8000000:
  case 0x98000000:
  case 0x64000000:
  case 0x44000000:
      return (c+d)/2;
  default:
      assert(0);
  }
  return 0;

}
void PredictMV(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC, sCoordinate* res)
{
    res->x=0;
    res->y=0;

    if (predA)
    {
        predB  = (!predB)? res: predB;
        predC  = (!predC)? res: predC;
        res->x = median3(predA->x, predB->x, predC->x);
        res->y = median3(predA->y, predB->y, predC->y);
    }
    else if (predC)
    {
        res->x = predC->x;
        res->y = predC->y;
    }
}
Ipp16s VC1abs(Ipp16s value)
{
  Ipp16u s = value>>15;
  s = (value + s)^s;
  return s;
}

void InitScaleInfo(sScaleInfo* pInfo, bool bCurSecondField ,bool bBottom,
                   Ipp8u ReferenceFrameDist, Ipp8u MVRangeIndex)
{
    static Ipp16s SCALEOPP[2][4]   = {{128,     192,    213,    224},
                                      {128,     64,     43,     32}};
    static Ipp16s SCALESAME1[2][4] = {{512,     341,    307,    293},
                                      {512,     1024,   1563,   2048}};
    static Ipp16s SCALESAME2[2][4] = {{219,     236,    242,    245},
                                      {219,     204,    200,    198}};
    static Ipp16s SCALEZONEX1[2][4] = {{32,     48,     53,     56},
                                       {32,     16,     11,     8}};
    static Ipp16s SCALEZONEY1[2][4] = {{ 8,     12,     13,     14},
                                       { 8,     4,      3,      2}};
    static Ipp16s ZONEOFFSETX1[2][4] = {{37,    20,     14,     11},
                                        {37,    52,     56,     58}};
    static Ipp16s ZONEYOFFSET1[2][4] = {{10,    5,      4,      3},
                                        {10,    13,     14,     15}};

    ReferenceFrameDist  = (ReferenceFrameDist>3)? 3:ReferenceFrameDist;

    pInfo->scale_opp    = SCALEOPP    [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_same1  = SCALESAME1  [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_same2  = SCALESAME2  [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_zoneX  = SCALEZONEX1 [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_zoneY  = SCALEZONEY1 [bCurSecondField][ReferenceFrameDist];
    pInfo->zone_offsetX = ZONEOFFSETX1[bCurSecondField][ReferenceFrameDist];
    pInfo->zone_offsetY = ZONEYOFFSET1[bCurSecondField][ReferenceFrameDist];

    pInfo->bBottom      =  bBottom;
    pInfo->rangeX       =  MVRange[2*MVRangeIndex]*4;
    pInfo->rangeY       =  MVRange[2*MVRangeIndex + 1]*4;
}
void InitScaleInfoBackward (sScaleInfo* pInfo, bool bCurSecondField ,bool bBottom,
                   Ipp8u ReferenceFrameDist, Ipp8u MVRangeIndex)
{
    static Ipp16s SCALEOPP[2][4]   = {{171,     205,    219,    288},
                                      {128,     64,     43,     32}};
    static Ipp16s SCALESAME1[2][4] = {{384,     320,    299,    288},
                                      {512,     1024,   1563,   2048}};
    static Ipp16s SCALESAME2[2][4] = {{230,     239,    244,    246},
                                      {219,     204,    200,    198}};
    static Ipp16s SCALEZONEX1[2][4] = {{43,     51,     55,     57},
                                       {32,     16,     11,     8}};
    static Ipp16s SCALEZONEY1[2][4] = {{11,     13,     14,     14},
                                       { 8,     4,      3,      2}};
    static Ipp16s ZONEOFFSETX1[2][4] = {{26,    17,     12,     10},
                                        {37,    52,     56,     58}};
    static Ipp16s ZONEYOFFSET1[2][4] = {{7,     4,      3,      3},
                                        {10,    13,     14,     15}};

    ReferenceFrameDist  = (ReferenceFrameDist>3)? 3:ReferenceFrameDist;

    pInfo->scale_opp    = SCALEOPP    [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_same1  = SCALESAME1  [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_same2  = SCALESAME2  [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_zoneX  = SCALEZONEX1 [bCurSecondField][ReferenceFrameDist];
    pInfo->scale_zoneY  = SCALEZONEY1 [bCurSecondField][ReferenceFrameDist];
    pInfo->zone_offsetX = ZONEOFFSETX1[bCurSecondField][ReferenceFrameDist];
    pInfo->zone_offsetY = ZONEYOFFSET1[bCurSecondField][ReferenceFrameDist];

    pInfo->bBottom      =  bBottom;
    pInfo->rangeX       =  MVRange[2*MVRangeIndex]*4;
    pInfo->rangeY       =  MVRange[2*MVRangeIndex + 1]*4;
}
static Ipp16s scale_sameX(Ipp16s n,sScaleInfo* pInfo, bool bHalf)
{
    Ipp16s abs_n = VC1abs(n = (n>>((Ipp16u)bHalf)));
    Ipp32s s;
    if (abs_n>255)
    {
        return n<<((Ipp16u)bHalf);
    }
    else if (abs_n<pInfo->scale_zoneX)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same2))>>8);
        s = (n<0)? s - pInfo->zone_offsetX:s + pInfo->zone_offsetX;
    }
    s = (s>pInfo->rangeX-1)? pInfo->rangeX-1:s;
    s = (s<-pInfo->rangeX) ? -pInfo->rangeX :s;

    return (Ipp16s) (s<<((Ipp16u)bHalf));
}
static Ipp16s scale_oppX(Ipp16s n,sScaleInfo* pInfo, bool bHalf)
{
    Ipp32s s = (((Ipp32s)((n>>((Ipp32u)bHalf))*pInfo->scale_opp))>>8);
    return (Ipp16s) (s<<((Ipp16u)bHalf));
}
static Ipp16s scale_sameY(Ipp16s n,sScaleInfo* pInfo, bool bHalf)
{
    Ipp16s abs_n = VC1abs(n = (n>>((Ipp16u)bHalf)));
    Ipp32s s     = 0;
    if (abs_n>63)
    {
        return n<<((Ipp16u)bHalf);
    }
    else if (abs_n<pInfo->scale_zoneY)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same2))>>8);
        s = (n<0)? s - pInfo->zone_offsetY:s + pInfo->zone_offsetY;
    }
    s = (s>pInfo->rangeY/2-1)? pInfo->rangeY/2-1:s;
    s = (s<-pInfo->rangeY/2) ? -pInfo->rangeY/2 :s;

    return (Ipp16s) (s<<((Ipp16u)bHalf));
}
static Ipp16s scale_sameY_B(Ipp16s n,sScaleInfo* pInfo, bool bHalf)
{
    Ipp16s abs_n = VC1abs(n = (n>>((Ipp16u)bHalf)));
    Ipp32s s     = 0;
    if (abs_n>63)
    {
        return n<<((Ipp16u)bHalf);
    }
    else if (abs_n<pInfo->scale_zoneY)
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same1))>>8);
    }
    else
    {
        s = (Ipp16s)(((Ipp32s)(n*pInfo->scale_same2))>>8);
        s = (n<0)? s - pInfo->zone_offsetY:s + pInfo->zone_offsetY;
    }
    if (pInfo->bBottom)
    {
        s = (s>pInfo->rangeY/2)? pInfo->rangeY/2:s;
        s = (s<-pInfo->rangeY/2+1) ? -pInfo->rangeY/2+1 :s;
    }
    else
    {
        s = (s>pInfo->rangeY/2-1)? pInfo->rangeY/2-1:s;
        s = (s<-pInfo->rangeY/2) ? -pInfo->rangeY/2 :s;
    }
    return (Ipp16s) (s<<((Ipp16u)bHalf));
}
static Ipp16s scale_oppY(Ipp16s n,sScaleInfo* pInfo,  bool bHalf)
{
    Ipp32s s = (((Ipp32s)((n>>((Ipp16u)bHalf))*pInfo->scale_opp))>>8);

    return (Ipp16s) (s<<((Ipp16u)bHalf));

}
typedef Ipp16s (*fScaleX)(Ipp16s n, sScaleInfo* pInfo, bool bHalf);
typedef Ipp16s (*fScaleY)(Ipp16s n, sScaleInfo* pInfo, bool bHalf);

static fScaleX pScaleX[2][2] = {{scale_oppX, scale_sameX},
                                {scale_sameX,scale_oppX}};
static fScaleY pScaleY[2][2] = {{scale_oppY, scale_sameY},
                                {scale_sameY_B, scale_oppY}};

bool PredictMVField2(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC,
                     sCoordinate* res, sScaleInfo* pInfo, bool bSecondField,
                     sCoordinate* predAEx,sCoordinate* predCEx, bool bBackward, bool bHalf)
{
    Ipp8u n = (Ipp8u)(((!predA)<<2) + ((!predB)<<1) + (!predC));
    res[0].bSecond = false;
    res[1].bSecond = true;
    bool  bBackwardFirst = (bBackward && !bSecondField);

    switch (n)
    {
    case 7: //111
        {
            res[0].x=0;
            res[0].y=0;
            res[1].x=0;
            res[1].y=0;
#ifdef VC1_ENC_DEBUG_ON
        pDebug->SetScaleType(0, bBackward);
#endif
            return false;  // opposite field
        }
    case 6: //110
        {
            bool  bSame = (predC->bSecond);
            res[predC->bSecond].x  =  predC->x;
            res[predC->bSecond].y  =  predC->y;
            predCEx->x = res[!predC->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predC->x,pInfo, bHalf);
            predCEx->y = res[!predC->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predC->y,pInfo, bHalf);
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType(bSame, bBackward);
#endif
            return predC->bSecond;
        }
    case 5: //101
        {
            bool  bSame = (predB->bSecond);
            res[predB->bSecond].x  =  predB->x;
            res[predB->bSecond].y  =  predB->y;
            res[!predB->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predB->x,pInfo, bHalf);
            res[!predB->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predB->y,pInfo, bHalf);
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType(bSame, bBackward);
#endif
            return predB->bSecond;
        }
    case 3: //011
        {
            bool  bSame = (predA->bSecond );
            res[predA->bSecond].x  =  predA->x;
            res[predA->bSecond].y  =  predA->y;
            predAEx->x = res[!predA->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predA->x,pInfo, bHalf);
            predAEx->y = res[!predA->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predA->y,pInfo, bHalf);
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType(bSame, bBackward);
#endif
            return predA->bSecond;
        }
    case 4: //100
        {
            sCoordinate B[2];
            sCoordinate C[2];
            bool  bSame = (predC->bSecond );

            C[predC->bSecond].x  =  predC->x;
            C[predC->bSecond].y  =  predC->y;
            predCEx->x = C[!predC->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predC->x,pInfo, bHalf);
            predCEx->y = C[!predC->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predC->y,pInfo, bHalf);

            bSame = (predB->bSecond );
            B[predB->bSecond].x  =  predB->x;
            B[predB->bSecond].y  =  predB->y;
            B[!predB->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predB->x,pInfo, bHalf);
            B[!predB->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predB->y,pInfo, bHalf);

            res[0].x = median3(C[0].x,B[0].x,0);
            res[1].x = median3(C[1].x,B[1].x,0);
            res[0].y = median3(C[0].y,B[0].y,0);
            res[1].y = median3(C[1].y,B[1].y,0);
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType((predB->bSecond != predC->bSecond)? false:predB->bSecond, bBackward);
#endif
            return (predB->bSecond != predC->bSecond)? false:predB->bSecond;
        }
    case 2: //010
        {
            sCoordinate A[2];
            sCoordinate C[2];
            bool  bSame = (predC->bSecond );

            C[predC->bSecond].x  =  predC->x;
            C[predC->bSecond].y  =  predC->y;
            predCEx->x =C[!predC->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predC->x,pInfo, bHalf);
            predCEx->y =C[!predC->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predC->y,pInfo, bHalf);

            bSame = (predA->bSecond );
            A[predA->bSecond].x  =  predA->x;
            A[predA->bSecond].y  =  predA->y;
            predAEx->x = A[!predA->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predA->x,pInfo, bHalf);
            predAEx->y = A[!predA->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predA->y,pInfo, bHalf);

            res[0].x = median3(C[0].x,A[0].x,0);
            res[1].x = median3(C[1].x,A[1].x,0);
            res[0].y = median3(C[0].y,A[0].y,0);
            res[1].y = median3(C[1].y,A[1].y,0);
#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType((predA->bSecond != predC->bSecond)? false:predA->bSecond, bBackward);
#endif
            return (predA->bSecond != predC->bSecond)? false:predA->bSecond;
        }
    case 1: //001
        {
            sCoordinate A[2];
            sCoordinate B[2];
            bool  bSame = (predB->bSecond );

            B[predB->bSecond].x  =  predB->x;
            B[predB->bSecond].y  =  predB->y;
            B[!predB->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predB->x,pInfo, bHalf);
            B[!predB->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predB->y,pInfo, bHalf);

            bSame = (predA->bSecond);
            A[predA->bSecond].x  =  predA->x;
            A[predA->bSecond].y  =  predA->y;
            predAEx->x = A[!predA->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predA->x,pInfo, bHalf);
            predAEx->y = A[!predA->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predA->y,pInfo, bHalf);

            res[0].x = median3(B[0].x,A[0].x,0);
            res[1].x = median3(B[1].x,A[1].x,0);
            res[0].y = median3(B[0].y,A[0].y,0);
            res[1].y = median3(B[1].y,A[1].y,0);
#ifdef VC1_ENC_DEBUG_ON
            pDebug->SetScaleType((predA->bSecond != predB->bSecond)? false:predB->bSecond, bBackward);
#endif
            return (predA->bSecond != predB->bSecond)? false:predB->bSecond;
        }
    case 0:
        {
            sCoordinate A[2];
            sCoordinate B[2];
            sCoordinate C[2];

            bool  bSame = (predC->bSecond );

            C[predC->bSecond].x  =  predC->x;
            C[predC->bSecond].y  =  predC->y;
            predCEx->x = C[!predC->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predC->x,pInfo, bHalf);
            predCEx->y = C[!predC->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predC->y,pInfo, bHalf);

            bSame = (predB->bSecond );

            B[predB->bSecond].x  =  predB->x;
            B[predB->bSecond].y  =  predB->y;
            B[!predB->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predB->x,pInfo, bHalf);
            B[!predB->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predB->y,pInfo, bHalf);

            bSame = (predA->bSecond );

            A[predA->bSecond].x  =  predA->x;
            A[predA->bSecond].y  =  predA->y;
            predAEx->x =  A[!predA->bSecond].x =  pScaleX[bBackwardFirst][!bSame](predA->x,pInfo, bHalf);
            predAEx->y =  A[!predA->bSecond].y =  pScaleY[bBackwardFirst][!bSame](predA->y,pInfo, bHalf);

            res[0].x = median3(B[0].x,A[0].x,C[0].x);
            res[1].x = median3(B[1].x,A[1].x,C[1].x);
            res[0].y = median3(B[0].y,A[0].y,C[0].y);
            res[1].y = median3(B[1].y,A[1].y,C[1].y);

#ifdef VC1_ENC_DEBUG_ON
                pDebug->SetScaleType((predA->bSecond + predB->bSecond + predC->bSecond >1)? true:false, bBackward);
#endif
            return (predA->bSecond + predB->bSecond + predC->bSecond >1)? true:false;
        }
    }
    assert(0);
    return false;
}
void PredictMVField1(sCoordinate* predA,sCoordinate* predB,sCoordinate* predC, sCoordinate* res)
{
    Ipp8u n = (Ipp8u)(((!predA)<<2) + ((!predB)<<1) + (!predC));
#ifdef VC1_ENC_DEBUG_ON
        pDebug->SetScaleType(2, 0);
        pDebug->SetScaleType(2, 1);
#endif
    switch (n)
    {
    case 7: //111
        {
            res->x=0;
            res->y=0;
            return;
        }
    case 6: //110
        {
            res->x  =  predC->x;
            res->y  =  predC->y;
            return;
        }
    case 5: //101
        {
            res->x  =  predB->x;
            res->y  =  predB->y;
            return;
        }
    case 3: //011
        {
            res->x  =  predA->x;
            res->y  =  predA->y;
            return;
        }
    case 4: //100
        {
            res->x = median3(predC->x,predB->x,0);
            res->y = median3(predC->y,predB->y,0);
            return;
        }
    case 2: //010
        {
            res->x = median3(predC->x,predA->x,0);
            res->y = median3(predC->y,predA->y,0);
            return;
        }
    case 1: //001
        {
            res->x = median3(predB->x,predA->x,0);
            res->y = median3(predB->y,predA->y,0);
            return;
        }
    case 0:
        {
            res->x = median3(predB->x,predA->x,predC->x);
            res->y = median3(predB->y,predA->y,predC->y);
            return;
        }
    }
    assert(0);
    return;
}

void Copy8x8_16s(Ipp16s*  pSrc, Ipp32u   srcStep, Ipp16s*  pDst, Ipp32u   dstStep)
{
   int i,j;
    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
            pDst[j] = pSrc[j];
        }
        pDst = (Ipp16s*)((Ipp8u*)pDst + dstStep);
        pSrc = (Ipp16s*)((Ipp8u*)pSrc + srcStep);
    }
}
//void Copy16x16_16s(Ipp16s*  pSrc, Ipp32u   srcStep, Ipp16s*  pDst, Ipp32u   dstStep)
//{
//   int i,j;
//    for (i=0;i<16;i++)
//    {
//        for (j=0;j<16;j++)
//        {
//            pDst[j] = pSrc[j];
//        }
//        pDst = (Ipp16s*)((Ipp8u*)pDst + dstStep);
//        pSrc = (Ipp16s*)((Ipp8u*)pSrc + srcStep);
//    }
//}

void ScalePredict(sCoordinate * MV, Ipp32s x,Ipp32s y,sCoordinate MVPredMin, sCoordinate MVPredMax)
{
    x += MV->x;
    y += MV->y;

    if (x < MVPredMin.x)
    {
        MV->x = MV->x - (Ipp16s)(x- MVPredMin.x);
    }
    else if (x > MVPredMax.x)
    {
        MV-> x = MV-> x - (Ipp16s)(x-MVPredMax.x);
    }

    if (y < MVPredMin.y)
    {
        MV->y = MV->y - (Ipp16s)(y - MVPredMin.y);
    }
    else if (y > MVPredMax.y)
    {
        MV->y = MV->y - (Ipp16s)(y - MVPredMax.y);
    }
}
Ipp8u HybridPrediction(     sCoordinate * mvPred,
                      const sCoordinate * MV,
                      const sCoordinate * mvA,
                      const sCoordinate * mvC)
{
    Ipp8u hybrid = 0;

    if (mvA && mvC)
    {
        Ipp32u sumA = VC1abs(mvA->x - mvPred->x) + VC1abs(mvA->y - mvPred->y);
        Ipp32u sumC = VC1abs(mvC->x - mvPred->x) + VC1abs(mvC->y - mvPred->y);

        if (sumA > 32 || sumC>32)
        {
            if (VC1abs(mvA->x - MV->x) + VC1abs(mvA->y - MV->y)<
                VC1abs(mvC->x - MV->x) + VC1abs(mvC->y - MV->y))
            {
                hybrid = 2;
                mvPred->x = mvA->x;
                mvPred->y = mvA->y;

            }
            else
            {

                hybrid = 1;
                mvPred->x = mvC->x;
                mvPred->y = mvC->y;
            }
        }
    }
    return hybrid;
}

Ipp8u isHybridPredictionFields(  sCoordinate * mvPred,
                                 const sCoordinate * mvA,
                                 const sCoordinate * mvC,
                                 Ipp32u        th)
{

    if (mvA && mvC)
    {
        Ipp32u sumA = VC1abs(mvA->x - mvPred->x) + VC1abs(mvA->y - mvPred->y);
        Ipp32u sumC = VC1abs(mvC->x - mvPred->x) + VC1abs(mvC->y - mvPred->y);
        if (sumA>th)
            return 1;
        else if (sumC>th)
            return 2;
    }
    return 0;
}

//#define _A(x) (((x)<0)?-(x):(x))
//Ipp32s FullSearch(const Ipp8u* pSrc, Ipp32u srcStep, const Ipp8u* pPred, Ipp32u predStep, sCoordinate Min, sCoordinate Max,sCoordinate * MV)
//{
//    Ipp32s x, y;
//    Ipp32s MVx=0, MVy=0;
//    Ipp32s sum  = 0x7FFFFFFF;
//    Ipp32s temp = 0;
//    Ipp32s yStep=1, xStep=1;
//    const Ipp8u* prediction = pPred;
//
//
//
//    for (y = Min.y; y <Max.y ; y+=yStep )
//    {
//        pPred = prediction + y*(Ipp32s)predStep;
//        for (x = Min.x; x < Max.x; x+=xStep )
//        {
//            ippiSAD16x16_8u32s (pSrc, srcStep, pPred+x, predStep, &temp, 0);
//            temp += (VC1abs(x)+VC1abs(y))*16;
//            if (temp < sum)
//            {
//                sum = temp;
//                MVx = x;
//                MVy = y;
//
//            }
//        }
//
//    }
//    MV->x = MVx;
//    MV->y = MVy;
//    return sum;
//}
//Ipp32s SumBlockDiffBPred16x16(const Ipp8u* pSrc, Ipp32u srcStep,const Ipp8u* pPred1, Ipp32u predStep1,
//                                                                const Ipp8u* pPred2, Ipp32u predStep2)
//{
//    Ipp32s x, y;
//    Ipp32s sum = 0;
//    for(y = 0; y <16; y++)
//    {
//        for (x=0; x<16; x++)
//        {
//            sum += VC1abs(pSrc[x]-((pPred1[x]+pPred2[x])>>2));
//        }
//        pPred1 += predStep1;
//        pPred2 += predStep2;
//        pSrc   += srcStep;
//
//    }
//
//    return sum;
//
//}
//Ipp32s SumBlockDiff16x16(const Ipp8u* pSrc1, Ipp32u srcStep1,const Ipp8u* pSrc2, Ipp32u srcStep2)
//{
//    Ipp32s x, y;
//    Ipp32s sum = 0;
//    for(y = 0; y <16; y++)
//    {
//        for (x=0; x<16; x++)
//        {
//            sum += VC1abs(pSrc1[x]-pSrc1[x]);
//        }
//        pSrc1+=srcStep1;
//        pSrc2+=srcStep2;
//    }
//    return sum;
//
//}
//void GetBlockType(Ipp16s* pBlock, Ipp32s step, Ipp8u Quant, eTransformType& type)
//{
//    int i,j;
//    Ipp32s s[4]={0};
//    bool vEdge[2]={0};
//    bool hEdge[3]={0};
//
//    for (i=0;i<8;i++)
//    {
//        for(j=0;j<8;j++)
//        {
//            s[((i>>2)<<1)+(j>>2)] += pBlock[j];
//        }
//        pBlock = (Ipp16s*)((Ipp8u*)pBlock + step);
//    }
//    s[0] = s[0]>>3;
//    s[1] = s[1]>>3;
//    s[2] = s[2]>>3;
//    s[3] = s[3]>>3;
//
//    vEdge[0] = ((VC1abs(s[0]-s[1]))>= Quant);
//    vEdge[1] = ((VC1abs(s[2]-s[3]))>= Quant);
//    hEdge[0] = ((VC1abs(s[0]-s[2]))>= Quant);
//    hEdge[1] = ((VC1abs(s[1]-s[3]))>= Quant);
//
//    if (vEdge[0]||vEdge[1])
//    {
//            type = (hEdge[0]||hEdge[1])? VC1_ENC_4x4_TRANSFORM:VC1_ENC_4x8_TRANSFORM;
//    }
//    else
//    {
//            type = (hEdge[0]||hEdge[1])? VC1_ENC_8x4_TRANSFORM:VC1_ENC_8x8_TRANSFORM;
//    }
//
//    return;
//}
//bool GetMBTSType(Ipp16s** ppBlock, Ipp32u* pStep, Ipp8u Quant /*doubleQuant*/, eTransformType* pTypes /*BlockTSTypes*/)
//{
//    Ipp8u num [4]           = {0};
//    Ipp8u max_num           = 0;
//    eTransformType max_type = VC1_ENC_8x8_TRANSFORM;
//    Ipp32s blk;
//
//    for (blk = 0; blk<6; blk++)
//    {
//        GetBlockType(ppBlock[blk],
//                     pStep[blk],
//                     Quant,
//                     (pTypes[blk]));
//        assert(pTypes[blk]<4);
//        num[pTypes[blk]]++;
//        if (num[pTypes[blk]]>max_num)
//        {
//            max_num         = num[pTypes[blk]];
//            max_type        = pTypes[blk];
//        }
//    } // for
//    if (max_num > 4)
//    {
//        for (blk = 0; blk<6; blk++)
//        {
//             pTypes[blk] = max_type;
//        }
//        return true; //bMBTSType - one type for all blocks in macroblock
//    }
//    return false;
//}
//#undef _A
//Ipp32s SumBlock16x16(const Ipp8u* pSrc, Ipp32u srcStep)
//{
//    Ipp32s x, y;
//    Ipp32s sum = 0;
//    for(y = 0; y <16; y++)
//    {
//        for (x=0; x<16; x++)
//        {
//            sum += pSrc[x];
//        }
//        pSrc += srcStep;
//    }
//    return sum;
//}


void  GetMVDirectHalf  (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)(((x*(scaleFactor-256)+255)>>9)<<1);
     mvB->y = (Ipp16s)(((y*(scaleFactor-256)+255)>>9)<<1);
     mvF->x = (Ipp16s)(((x*scaleFactor+255)>>9)<<1);
     mvF->y = (Ipp16s)(((y*scaleFactor+255)>>9)<<1);

}
void  GetMVDirectQuarter(Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)((x*(scaleFactor-256)+128)>>8);
     mvB->y = (Ipp16s)((y*(scaleFactor-256)+128)>>8);
     mvF->x = (Ipp16s)((x*scaleFactor+128)>>8);
     mvF->y = (Ipp16s)((y*scaleFactor+128)>>8);
}
void  GetMVDirectCurrHalfBackHalf (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)(((x<<1)*(scaleFactor-256)+255)>>9);
     mvB->y = (Ipp16s)(((y<<1)*(scaleFactor-256)+255)>>9);
     mvF->x = (Ipp16s)(((x<<1)*scaleFactor+255)>>9);
     mvF->y = (Ipp16s)(((y<<1)*scaleFactor+255)>>9);

}
void  GetMVDirectCurrQuarterBackHalf(Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)(((x<<1)*(scaleFactor-256)+128)>>8);
     mvB->y = (Ipp16s)(((y<<1)*(scaleFactor-256)+128)>>8);
     mvF->x = (Ipp16s)(((x<<1)*scaleFactor+128)>>8);
     mvF->y = (Ipp16s)(((y<<1)*scaleFactor+128)>>8);
}
void  GetMVDirectCurrHalfBackQuarter (Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)(((x*(scaleFactor-256)+255)>>9)<<1);
     mvB->y = (Ipp16s)(((y*(scaleFactor-256)+255)>>9)<<1);
     mvF->x = (Ipp16s)(((x*scaleFactor+255)>>9)<<1);
     mvF->y = (Ipp16s)(((y*scaleFactor+255)>>9)<<1);

}
void  GetMVDirectCurrQuarterBackQuarter(Ipp16s x, Ipp16s y, Ipp32s scaleFactor, sCoordinate * mvF, sCoordinate *mvB)
{
     mvB->x = (Ipp16s)((x*(scaleFactor-256)+128)>>8);
     mvB->y = (Ipp16s)((y*(scaleFactor-256)+128)>>8);
     mvF->x = (Ipp16s)((x*scaleFactor+128)>>8);
     mvF->y = (Ipp16s)((y*scaleFactor+128)>>8);
}
void GetChromaMV (sCoordinate LumaMV, sCoordinate * pChroma)
{
    static Ipp16s round[4]= {0,0,0,1};

    pChroma->x = (LumaMV.x + round[LumaMV.x&0x03])>>1;
    pChroma->y = (LumaMV.y + round[LumaMV.y&0x03])>>1;
}


void GetChromaMVFast(sCoordinate LumaMV, sCoordinate * pChroma)
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
void GetIntQuarterMV(sCoordinate MV,sCoordinate *pMVInt, sCoordinate *pMVIntQuarter)
{
    pMVInt->x = MV.x>>2;
    pMVInt->y = MV.y>>2;
    pMVIntQuarter->x = MV.x - (pMVInt->x<<2);
    pMVIntQuarter->y = MV.y - (pMVInt->y<<2);
}
//void GetQMV(sCoordinate *pMV,sCoordinate MVInt, sCoordinate MVIntQuarter)
//{
//    pMV->x = (MVInt.x<<2) + MVIntQuarter.x;
//    pMV->y = (MVInt.y<<2) + MVIntQuarter.y;
//}

////////////////////////////////////////////////////////////////
//--------Interpolation----------------------------------------


////////////////////////////////////////////////////////////////
//--------Inverse transform----------------------------------------


////////////////////////////////////////////////////////////////
//--------Convert----------------------------------------

bool MaxAbsDiff(Ipp8u* pSrc1, Ipp32u step1,Ipp8u* pSrc2, Ipp32u step2,IppiSize roiSize,Ipp32u max)
{
    Ipp32u  sum = max;
    for (int i = 0; i<roiSize.height; i++)
    {
       Ipp16u t1 = (VC1abs(*(pSrc1++) - *(pSrc2++)));
       Ipp16u t2 = (VC1abs(*(pSrc1++) - *(pSrc2++)));

       for (int j = 0; j<roiSize.width-2; j++)
       {
           Ipp16u t3 = (VC1abs(*(pSrc1++) - *(pSrc2++)));
           if (t2>sum &&(t1>sum || t3>sum))
           {
            return false;
           }
           t1 = t2;
           t2 = t3;
       }
       pSrc1 = pSrc1 + (step1 - roiSize.width);
       pSrc2 = pSrc2 + (step2 - roiSize.width);
    }
    return true;
}
void initMVFieldInfo(bool extendedX,        bool extendedY,const Ipp32u* pVLCTable ,sMVFieldInfo* MVFieldInfo)
{
     MVFieldInfo->bExtendedX = extendedX;
     MVFieldInfo->bExtendedY = extendedY;

    if (extendedX)
    {
        MVFieldInfo->pMVSizeOffsetFieldIndexX = MVSizeOffsetFieldExIndex;
        MVFieldInfo->pMVSizeOffsetFieldX      = MVSizeOffsetFieldEx;
        MVFieldInfo->limitX                   = 511;
    }
    else
    {
        MVFieldInfo->pMVSizeOffsetFieldIndexX = MVSizeOffsetFieldIndex;
        MVFieldInfo->pMVSizeOffsetFieldX      = MVSizeOffsetField;
        MVFieldInfo->limitX                   = 256;
    }
    if (extendedY)
    {
        MVFieldInfo->pMVSizeOffsetFieldIndexY = MVSizeOffsetFieldExIndex;
        MVFieldInfo->pMVSizeOffsetFieldY      = MVSizeOffsetFieldEx;
        MVFieldInfo->limitY                   = 511;
    }
    else
    {
        MVFieldInfo->pMVSizeOffsetFieldIndexY = MVSizeOffsetFieldIndex;
        MVFieldInfo->pMVSizeOffsetFieldY      = MVSizeOffsetField;
        MVFieldInfo->limitY                   = 256;
    }
    MVFieldInfo->pMVModeField1RefTable_VLC = pVLCTable;

}
}

#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
