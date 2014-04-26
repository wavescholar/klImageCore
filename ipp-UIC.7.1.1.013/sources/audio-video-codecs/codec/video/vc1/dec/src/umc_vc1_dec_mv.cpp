/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) MV decoding
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_mvdiff_tbl.h"

static Ipp8u size_table[6] = {0, 2, 3, 4, 5, 8};
static Ipp8u offset_table[6] = {0, 1, 3, 7, 15, 31};

Ipp16u DecodeMVDiff(VC1Context* pContext,Ipp32s hpelfl,
                  Ipp16s* pdmv_x, Ipp16s* pdmv_y)
{
    IppStatus ret;
    Ipp32s data;
    Ipp16s index;
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp16u last_intra = 0;
    VC1PictureLayerHeader* picHeader = pContext->m_picLayerHeader;

#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif


    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                     &pContext->m_bitstream.bitOffset,
                                     &data,
                                     picHeader->m_pCurrMVDifftbl);
    VM_ASSERT(ret == ippStsNoErr);

    index = (Ipp16s)(data & 0x000000FF);
    data = data>>8;
    last_intra = (Ipp16u)(data >> 8);


    if(index < 35)
    {
        Ipp32s hpel = 0, sign, val;

        if(data&0x0f)
        {
            Ipp32u index1 = (data & 0x0000000f) % 6;   // = index%6
            if (hpelfl + index1 == 6)
                hpel = 1;

            VC1_GET_BITS(size_table[index1] - hpel, val);
            sign = 0 - (val & 1);
            dmv_x = (Ipp16s)(sign ^ ((val >> 1) + offset_table[index1]));
            dmv_x = dmv_x - (Ipp16s)sign;
        }


        if(index > 5)
        {
            data = data >> 4;
            hpel = 0;
            Ipp32u index2 = (data & 0x0000000f) % 6;   // = index/6

            if (hpelfl + index2 == 6)
                hpel = 1;

            VC1_GET_BITS(size_table[index2] - hpel, val);
            sign = 0 - (val & 1);
            dmv_y = (Ipp16s)(sign ^ ((val >> 1) + offset_table[index2]));
            dmv_y = dmv_y - (Ipp16s)sign;
        }
    }
    else  if (index == 35)
    {
        Ipp32s k_x = picHeader->m_pCurrMVRangetbl->k_x - hpelfl;
        Ipp32s k_y = picHeader->m_pCurrMVRangetbl->k_y - hpelfl;

        Ipp32s tmp_dmv_x = 0;
        Ipp32s tmp_dmv_y = 0;

        VC1_GET_BITS(k_x, tmp_dmv_x);
        VC1_GET_BITS(k_y, tmp_dmv_y);

        dmv_x = (Ipp16s)tmp_dmv_x;
        dmv_y = (Ipp16s)tmp_dmv_y;
    }


    * pdmv_x = dmv_x;
    * pdmv_y = dmv_y;
#ifdef UMC_STREAM_ANALYZER
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsMV += CalculateUsedBits(Bitsream,pContext->m_bitstream);
#endif

    return last_intra;
}

void PullBack_PPred(VC1Context* pContext, Ipp16s *pMVx, Ipp16s* pMVy, Ipp32s blk_num)
{
    Ipp32s Min;
    Ipp32s X = *pMVx;
    Ipp32s Y = *pMVy;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s IX = (sMB->m_currMBXpos<<6 ) + X;
    Ipp32s IY = (sMB->m_currMBYpos<<6 ) + Y;
    Ipp32s Width  = (sMB->widthMB<<6 )  - 4;
    Ipp32s Height = (sMB->heightMB<<6 ) - 4;

    Min = -60;

    if (VC1_GET_MBTYPE(pContext->m_pCurrMB->mbType)== VC1_MB_4MV_INTER)
    {
        Min = -28;

        if (blk_num==1 || blk_num==3)
        {
            IX += 32;
        }
        if (blk_num==2 || blk_num==3)
        {
            IY += 32;
        }
    }

    if (IX < Min)
    {
        X = Min-(sMB->m_currMBXpos<<6);
    }
    else if (IX > Width)
    {
        X = Width - (sMB->m_currMBXpos<<6);
    }

    if (IY < Min)
    {
        Y = Min - (sMB->m_currMBYpos<<6);
    }
    else if (IY > Height)
    {
        Y = Height - (pContext->m_pSingleMB->m_currMBYpos<<6);
    }

    (*pMVx) = (Ipp16s)X;
    (*pMVy) = (Ipp16s)Y;
}

void CropLumaPullBack(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s X = *xMV;
    Ipp32s Y = *yMV;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    Ipp32s IX = sMB->m_currMBXpos<<4;
    Ipp32s IY = sMB->m_currMBYpos<<4;
    Ipp32s Width  = (sMB->widthMB<<4);
    Ipp32s Height = (sMB->heightMB<<4);

    Ipp32s XPos = IX + (X>>2);
    Ipp32s YPos = IY + (Y>>2);

    if (XPos < -16)
    {
        X = (X&3)+((-IX-16)<<2);
    }
    else if (XPos > Width)
    {
        X = (X&3)+((Width-IX)<<2);
    }

    if (YPos < -16)
    {
        Y =(Y&3)+((-IY-16)<<2);
    }
    else if (YPos > Height)
    {
        Y= (Y&3)+((Height - IY)<<2);

    }

    (*xMV) = (Ipp16s)X;
    (*yMV) = (Ipp16s)Y;

}


void CropChromaPullBack(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s X = *xMV;
    Ipp32s Y = *yMV;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    Ipp32s IX = sMB->m_currMBXpos<<3;
    Ipp32s IY = sMB->m_currMBYpos<<3;
    Ipp32s Width  = (sMB->widthMB<<3);
    Ipp32s Height = (sMB->heightMB<<3);

    Ipp32s XPos = IX + (X >> 2);
    Ipp32s YPos = IY + (Y >> 2);

    if (XPos < -8)
    {
        X =(X&3)+((-8 - IX)<<2);
    }
    else if (XPos > Width)
    {
        X = (X&3)+((Width - IX)<<2);
    }

    if (YPos < -8)
    {
        Y =(Y&3)+((-8 - IY)<<2);;
    }
    else if (YPos > Height)
    {
        Y = (Y&3)+((Height -IY)<<2);

    }

    *xMV = (Ipp16s)X;
    *yMV = (Ipp16s)Y;

    pCurrMB->m_pBlocks[4].mv[0][0] = *xMV;
    pCurrMB->m_pBlocks[4].mv[0][1] = *yMV;
    pCurrMB->m_pBlocks[5].mv[0][0] = *xMV;
    pCurrMB->m_pBlocks[5].mv[0][1] = *yMV;
}

void CalculateProgressive1MV(VC1Context* pContext, Ipp16s *pPredMVx,Ipp16s *pPredMVy)
{
    Ipp16s x=0,y=0;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1MVPredictors* MVPred = &pContext->MVPred;

    GetPredictProgressiveMV(MVPred->AMVPred[0],
                            MVPred->BMVPred[0],
                            MVPred->CMVPred[0],
                            &x,&y,0);
    x = PullBack_PredMV(&x,(sMB->m_currMBXpos<<6), -60,(sMB->widthMB<<6)-4);
    y = PullBack_PredMV(&y,(sMB->m_currMBYpos<<6), -60,(sMB->heightMB<<6)-4);

    HybridMV(pContext,MVPred->AMVPred[0],MVPred->CMVPred[0], &x,&y,0);
    *pPredMVx=x;
    *pPredMVy=y;

}

void CalculateProgressive1MV_B(VC1Context* pContext, Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                            Ipp32s Back)
{
    Ipp16s x=0,y=0;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1MVPredictors* MVPred = &pContext->MVPred;

    GetPredictProgressiveMV(MVPred->AMVPred[0],
                            MVPred->BMVPred[0],
                            MVPred->CMVPred[0],
                            &x, &y,
                            Back);
  //  VM_Debug::GetInstance().vm_debug_frame(-1,VC1_BFRAMES,VM_STRING("predict MV (%d,%d), back = %d\n"),x,y,Back);
     x = PullBack_PredMV(&x,(sMB->m_currMBXpos<<5), -28,(sMB->widthMB<<5)-4);
     y = PullBack_PredMV(&y,(sMB->m_currMBYpos<<5), -28,(sMB->heightMB<<5)-4);

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_BFRAMES,VM_STRING("1.predict MV (%d,%d), back = %d\n"),x,y,Back);
    *pPredMVx=x;
    *pPredMVy=y;
}

void CalculateProgressive4MV(VC1Context* pContext,
                                Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                Ipp32s blk_num)
{
    Ipp16s x,y;
    VC1MVPredictors* MVPred = &pContext->MVPred;

    GetPredictProgressiveMV(MVPred->AMVPred[blk_num],
                            MVPred->BMVPred[blk_num],
                            MVPred->CMVPred[blk_num],
                            &x,&y,0);
    PullBack_PPred4MV(pContext->m_pSingleMB,&x,&y, blk_num);
    HybridMV(pContext,MVPred->AMVPred[blk_num],MVPred->CMVPred[blk_num], &x,&y,0);
    *pPredMVx=x;
    *pPredMVy=y;
}


void Scale_Direct_MV(VC1PictureLayerHeader* picHeader, Ipp16s X, Ipp16s Y,
                     Ipp16s* Xf, Ipp16s* Yf,
                     Ipp16s* Xb, Ipp16s* Yb)
{
    Ipp32s hpelfl = (Ipp32s)((picHeader->MVMODE==VC1_MVMODE_HPEL_1MV) ||
                            (picHeader->MVMODE==VC1_MVMODE_HPELBI_1MV));
    Ipp32s ScaleFactor = picHeader->ScaleFactor;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("MVMode = %d\n"),pContext->m_picLayerHeader->MVMODE);


    if (hpelfl)
    {
        * Xf = (Ipp16s)(2*((X*ScaleFactor + 255)>>9));
        * Yf = (Ipp16s)(2*((Y*ScaleFactor + 255)>>9));

        ScaleFactor -=256;
        * Xb = (Ipp16s)(2*((X*ScaleFactor + 255)>>9));
        * Yb = (Ipp16s)(2*((Y*ScaleFactor + 255)>>9));
    }
    else
    {
        * Xf =(Ipp16s)((X*ScaleFactor + 128)>>8);
        * Yf =(Ipp16s)((Y*ScaleFactor + 128)>>8);

        ScaleFactor -=256;
        * Xb =(Ipp16s)((X*ScaleFactor + 128)>>8);
        * Yb =(Ipp16s)((Y*ScaleFactor + 128)>>8);
    }
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_BFRAMES,VM_STRING("ScaleFactor %d, forw (%d,%d), back (%d,%d)\n"),ScaleFactor+256,* Xf,* Yf,* Xb,* Yb);

}
void Scale_Direct_MV_Interlace(VC1PictureLayerHeader* picHeader,
                               Ipp16s X, Ipp16s Y,
                               Ipp16s* Xf, Ipp16s* Yf,
                               Ipp16s* Xb, Ipp16s* Yb)
{
    Ipp32s ScaleFactor = picHeader->ScaleFactor;
    * Xf =(Ipp16s)((X*ScaleFactor+128)>>8);
    * Yf =(Ipp16s)((Y*ScaleFactor+128)>>8);

    ScaleFactor -=256;

    * Xb =(Ipp16s)((X*ScaleFactor+128)>>8);
    * Yb =(Ipp16s)((Y*ScaleFactor+128)>>8);
}

void DeriveSecondStageChromaMV(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp16s IX, IY;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV_BBL,VM_STRING("MV(%d,%d)\n"),*xMV,*yMV);

    if(((*xMV) == VC1_MVINTRA) || ((*yMV) == VC1_MVINTRA))
    {
        return;
    }
    else
    {
        const Ipp16s Round[4] = {0, 0, 0, 1};
        Ipp16s CMV_X, CMV_Y;

        IX = *xMV;
        IY = *yMV;

        CMV_X = (IX + Round[IX & 3]) >> 1;
        CMV_Y = (IY + Round[IY & 3]) >> 1;

        if (pContext->m_seqLayerHeader->FASTUVMC)
        {
            const Ipp16s RndTbl[3] = {1, 0, -1};
            CMV_X = CMV_X + RndTbl[1 + (VC1_SIGN(CMV_X) * (vc1_abs_16s(CMV_X) % 2))];
            CMV_Y = CMV_Y + RndTbl[1 + (VC1_SIGN(CMV_Y) * (vc1_abs_16s(CMV_Y) % 2))];
        }

        *xMV = CMV_X;
        *yMV = CMV_Y;

        pMB->m_pBlocks[4].mv[0][0] = *xMV;
        pMB->m_pBlocks[4].mv[0][1] = *yMV;
        pMB->m_pBlocks[5].mv[0][0] = *xMV;
        pMB->m_pBlocks[5].mv[0][1] = *yMV;
    }
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
