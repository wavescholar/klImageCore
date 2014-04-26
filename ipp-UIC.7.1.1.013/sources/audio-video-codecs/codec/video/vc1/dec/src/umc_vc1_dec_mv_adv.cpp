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

void ApplyMVPredictionCalculate( VC1Context* pContext,
                                Ipp16s* pMVx,
                                Ipp16s* pMVy,
                                Ipp32s dmv_x,
                                Ipp32s dmv_y)
{
    const VC1MVRange *pMVRange = pContext->m_picLayerHeader->m_pCurrMVRangetbl;
    Ipp32s RangeX, RangeY;
    Ipp32s DMV_X, DMV_Y;
    Ipp32s PredictorX, PredictorY;
    Ipp16s MVx, MVy;

    RangeX = pMVRange->r_x;
    RangeY = pMVRange->r_y;

    DMV_X = dmv_x;
    DMV_Y = dmv_y;

    PredictorX = *pMVx;
    PredictorY = *pMVy;
    DMV_X += PredictorX;
    DMV_Y += PredictorY;
    MVx = (Ipp16s)( ((DMV_X + RangeX) & ( (RangeX << 1) - 1)) - RangeX );
    MVy = (Ipp16s)( ((DMV_Y + RangeY) & ( (RangeY << 1) - 1)) - RangeY);

    *pMVx = MVx;
    *pMVy = MVy;
}


void ApplyMVPredictionCalculateOneReference( VC1PictureLayerHeader* picLayerHeader,
                                             Ipp16s* pMVx, Ipp16s* pMVy,
                                             Ipp32s dmv_x,  Ipp32s dmv_y, Ipp8u same_polatity)
{
    const VC1MVRange *pMVRange = picLayerHeader->m_pCurrMVRangetbl;
    Ipp32s RangeX, RangeY;
    Ipp32s DMV_X, DMV_Y;
    Ipp32s PredictorX, PredictorY;
    Ipp16s MVx, MVy;

    RangeX = pMVRange->r_x;
    RangeY = pMVRange->r_y;

    DMV_X = dmv_x;
    DMV_Y = dmv_y;

    PredictorX = *pMVx;
    PredictorY = *pMVy;

    if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV
        || picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
    {
        RangeX  = RangeX << 1;
        RangeY  = RangeY << 1;
    }

    if (picLayerHeader->NUMREF == 1)
    {
        RangeY  = RangeY >> 1;
    }

    DMV_X += PredictorX;
    DMV_Y += PredictorY;

    //MVx = (Ipp16s)( ((DMV_X + RangeX) & (2 * RangeX - 1)) - RangeX );
    //MVy = (Ipp16s)( ((DMV_Y + RangeY) & (2 * RangeY - 1)) - RangeY);
    MVx = (Ipp16s)( ((DMV_X + RangeX) & ( (RangeX << 1) - 1)) - RangeX );
    MVy = (Ipp16s)( ((DMV_Y + RangeY) & ( (RangeY << 1) - 1)) - RangeY);

    if ( same_polatity && (picLayerHeader->BottomField))
        //MVy = (Ipp16s)( ( ((DMV_Y + RangeY - 1) & (2 * RangeY - 1)) - RangeY )+1);
        MVy = (Ipp16s)( ( ((DMV_Y + RangeY - 1) & ( (RangeY << 1) - 1)) - RangeY )+1);

    *pMVx = MVx;
    *pMVy = MVy;
}

void ApplyMVPredictionCalculateTwoReference( VC1PictureLayerHeader* picLayerHeader,
                                             Ipp16s* pMVx,
                                             Ipp16s* pMVy,
                                             Ipp32s dmv_x,
                                             Ipp32s dmv_y,
                                             Ipp8u same_polatity)
{
    const VC1MVRange *pMVRange = picLayerHeader->m_pCurrMVRangetbl;
    Ipp32s RangeX, RangeY;
    Ipp32s DMV_X, DMV_Y;
    Ipp32s PredictorX, PredictorY;
    Ipp16s MVx, MVy;

    RangeX = pMVRange->r_x;
    RangeY = pMVRange->r_y;

    DMV_X = dmv_x;
    DMV_Y = dmv_y;

    PredictorX = *pMVx;
    PredictorY = *pMVy;


    if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV
        || picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
    {
        RangeX  = RangeX << 1;
        RangeY  = RangeY << 1;
    }

    if (picLayerHeader->NUMREF == 1)
    {
        RangeY  = RangeY >> 1;
    }

    DMV_X += PredictorX;
    DMV_Y += PredictorY;

    MVx = (Ipp16s)( ((DMV_X + RangeX) & ((RangeX << 1) - 1)) - RangeX );
    MVy = (Ipp16s)( ((DMV_Y + RangeY) & ((RangeY << 1) - 1)) - RangeY);

    if ( same_polatity && (picLayerHeader->BottomField))
        MVy = (Ipp16s)( ( ((DMV_Y - 1 + RangeY ) & ((RangeY << 1) - 1)) - RangeY )+1);

    *pMVx = MVx;
    *pMVy = MVy;
}

void CropLumaPullBack_Adv(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s X = *xMV;
    Ipp32s Y = *yMV;
    Ipp32s xNum;
    Ipp32s yNum;
    Ipp32s IX = pContext->m_pSingleMB->m_currMBXpos;
    Ipp32s IY = pContext->m_pSingleMB->m_currMBYpos;

    Ipp32s Width  = (2*(pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1));
    Ipp32s Height = (2*(pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1) >> 1);

    xNum = (IX<<4) + (X >> 2);
    yNum = (IY<<3) + (Y >> 3);

    if (xNum < -17)
    {
        X -= ((xNum +17) << 2);
    }
    else if (xNum > Width)
    {
        X -= ((xNum -Width) << 2);

    }

    if (yNum < -18)
    {
        Y -= ((yNum+18) << 3);
    }
    else if (yNum > Height+1)
    {
        Y -= ((yNum-Height-1) << 3);
    }

    (*xMV) = (Ipp16s)X;
    (*yMV) = (Ipp16s)Y;

}
void CropLumaPullBackField_Adv(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s X = *xMV;
    Ipp32s Y = *yMV;
    Ipp32s xNum;
    Ipp32s yNum;
    Ipp32s IX = pContext->m_pSingleMB->m_currMBXpos;
    Ipp32s IY = pContext->m_pSingleMB->m_currMBYpos;

    Ipp32s Width  = ((pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1) << 1);
    Ipp32s Height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1);


    if (pContext->m_picLayerHeader->CurrField)
        IY  -= (pContext->m_seqLayerHeader->heightMB >> 1);

    IY = IY << 1;
    Y  = Y << 1;

    xNum = (IX<<4) + (X >> 2);
    yNum = (IY<<3) + (Y >> 3);

    if (xNum < -17)
    {
        X -= 4*(xNum +17);
    }
    else if (xNum > Width)
    {
        X -= 4*(xNum -Width);
    }

    if (yNum < -18)
    {
        Y -= 8*(yNum+18);
    }
    else if (yNum > Height+1)
    {
        Y -= 8*(yNum-Height-1);
    }
    Y = Y >> 1;

    (*xMV) = (Ipp16s)X;
    (*yMV) = (Ipp16s)Y;

}
void CropChromaPullBack_Adv(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    Ipp32s X = *xMV;
    Ipp32s Y = *yMV;
    Ipp32s XPos;
    Ipp32s YPos;
    Ipp32s IX = pContext->m_pSingleMB->m_currMBXpos;
    Ipp32s IY = pContext->m_pSingleMB->m_currMBYpos;

    Ipp32s Width  = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1);
    Ipp32s Height = ((pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1) >> 1);


    Ipp32s MinY = -8;
    Ipp32s MaxY = Height;

    if (pContext->m_picLayerHeader->CurrField)
        IY  -= (pContext->m_pSingleMB->heightMB >> 1);

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        --MinY;
        ++MaxY;
        IY = IY << 1;
        Y  = Y  << 1;
    }

    XPos = (IX<<3) + (X >> 2);
    YPos = (IY<<2) + (Y >> 3);

    if (XPos < -8)
    {
        X -= 4*(XPos+8);
    }
    else if (XPos > Width)
    {
        X -= ((XPos-Width) << 2);
    }

    if (YPos < -8)
    {
        Y -= 8*(YPos+8);
    }
    else if (YPos > Height)
    {
        Y -= ((YPos-Height) << 3);
    }

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
       Y = Y >> 1;
    }
    *xMV = (Ipp16s)X;
    *yMV = (Ipp16s)Y;

    pContext->m_pCurrMB->m_pBlocks[4].mv[0][0] = *xMV;
    pContext->m_pCurrMB->m_pBlocks[4].mv[0][1] = *yMV;
    pContext->m_pCurrMB->m_pBlocks[5].mv[0][0] = *xMV;
    pContext->m_pCurrMB->m_pBlocks[5].mv[0][1] = *yMV;
}

void CalculateProgressive1MV_B_Adv  (VC1Context* pContext,
                                Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                Ipp32s Back)
{
    Ipp16s x=0,y=0;

    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1MVPredictors* MVPred = &pContext->MVPred;

    GetPredictProgressiveMV(MVPred->AMVPred[0],
                            MVPred->BMVPred[0],
                            MVPred->CMVPred[0],
                            &x,&y,Back);


    x = PullBack_PredMV(&x,(sMB->m_currMBXpos<<6), -60,(sMB->widthMB<<6)-4);
    y = PullBack_PredMV(&y,(sMB->m_currMBYpos<<6), -60,(sMB->heightMB<<6)-4);
    //VM_Debug::GetInstance(VC1DebugRoutine).vm_debug_frame(-1,VC1_BFRAMES,
    //VM_STRING("1.predict MV (%d,%d), back = %d\n"),x,y,Back);
    *pPredMVx=x;
    *pPredMVy=y;
}

void CalculateProgressive4MV_Adv(VC1Context* pContext,
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

void CalculateInterlaceFrame1MV_P(VC1MVPredictors* MVPredictors,
                              Ipp16s *pPredMVx,Ipp16s *pPredMVy)
{
    VC1MVPredictors MVPred;

    Ipp32u validPredictors = 0;
    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[0])
    {
        if(MVPred.FieldMB[0][0])
        {
            MV_px[0] = (MVPred.AMVPred[0]->mv[0][0] + MVPred.AMVPred[0]->mv_bottom[0][0] + 1)>>1;
            MV_py[0] = (MVPred.AMVPred[0]->mv[0][1] + MVPred.AMVPred[0]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[0] = MVPred.AMVPred[0]->mv[0][0];
            MV_py[0] = MVPred.AMVPred[0]->mv[0][1];
        }
        validPredictors++;
    }

    if(MVPred.BMVPred[0])
    {
        if(MVPred.FieldMB[0][1])
        {
            MV_px[1] = (MVPred.BMVPred[0]->mv[0][0] + MVPred.BMVPred[0]->mv_bottom[0][0] + 1)>>1;
            MV_py[1] = (MVPred.BMVPred[0]->mv[0][1] + MVPred.BMVPred[0]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[1] = MVPred.BMVPred[0]->mv[0][0];
            MV_py[1] = MVPred.BMVPred[0]->mv[0][1];
        }
        validPredictors++;
    }

    if(MVPred.CMVPred[0])
    {
        if(MVPred.FieldMB[0][2])
        {
            MV_px[2] = (MVPred.CMVPred[0]->mv[0][0] + MVPred.CMVPred[0]->mv_bottom[0][0] + 1)>>1;
            MV_py[2] = (MVPred.CMVPred[0]->mv[0][1] + MVPred.CMVPred[0]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[2] = MVPred.CMVPred[0]->mv[0][0];
            MV_py[2] = MVPred.CMVPred[0]->mv[0][1];
        }
        validPredictors++;
    }

    //computing frame predictors
    if (validPredictors > 1)
    {
        // 2 or 3 predictors are available
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        // 1 or 0 predictor is available
        *pPredMVx = MV_px[0]+MV_px[1]+MV_px[2];
        *pPredMVy = MV_py[0]+MV_py[1]+MV_py[2];
    }
}

void CalculateInterlaceFrame1MV_B(VC1MVPredictors* MVPredictors,
                                    Ipp16s *f_x,Ipp16s *f_y,
                                    Ipp16s *b_x,Ipp16s *b_y,
                                    Ipp32u back)
{
    VC1MVPredictors MVPred;
    Ipp32u f_back = back;
    Ipp32u b_back = 1 - back;
    Ipp32u same = 0;
    Ipp32u opposite = 0;

    //Ipp32u validPredictors = 0;
    Ipp16s f_MV_px[] = {0,0,0};
    Ipp16s f_MV_py[] = {0,0,0};

    Ipp16s b_MV_px[] = {0,0,0};
    Ipp16s b_MV_py[] = {0,0,0};

    Ipp16s MV_px_sameField[] = {0,0,0};
    Ipp16s MV_py_sameField[] = {0,0,0};

    Ipp16s MV_px_oppField[] = {0,0,0};
    Ipp16s MV_py_oppField[] = {0,0,0};

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[0])
    {
        if(MVPred.FieldMB[0][0])
        {
            f_MV_px[0] = (MVPred.AMVPred[0]->mv[f_back][0]
                                + MVPred.AMVPred[0]->mv_bottom[f_back][0] + 1)>>1;
            f_MV_py[0] = (MVPred.AMVPred[0]->mv[f_back][1]
                                + MVPred.AMVPred[0]->mv_bottom[f_back][1] + 1)>>1;
        }
        else
        {
            f_MV_px[0] = MVPred.AMVPred[0]->mv[f_back][0];
            f_MV_py[0] = MVPred.AMVPred[0]->mv[f_back][1];
        }

        //validPredictors++;

        b_MV_px[0] = MVPred.AMVPred[0]->mv[b_back][0];
        b_MV_py[0] = MVPred.AMVPred[0]->mv[b_back][1];

        //classifying candidate mv
        if(b_MV_py[0] & 4)
        {
            MV_px_oppField[opposite] = b_MV_px[0];
            MV_py_oppField[opposite] = b_MV_py[0];
            ++opposite;
        }
        else
        {
            MV_px_sameField[same] = b_MV_px[0];
            MV_py_sameField[same] = b_MV_py[0];
            ++same;
        }
    }

    if(MVPred.BMVPred[0])
    {
        if(MVPred.FieldMB[0][1])
        {
            f_MV_px[1] = (MVPred.BMVPred[0]->mv[f_back][0]
                            + MVPred.BMVPred[0]->mv_bottom[f_back][0] + 1)>>1;
            f_MV_py[1] = (MVPred.BMVPred[0]->mv[f_back][1]
                            + MVPred.BMVPred[0]->mv_bottom[f_back][1] + 1)>>1;
        }
        else
        {
            f_MV_px[1] = MVPred.BMVPred[0]->mv[f_back][0];
            f_MV_py[1] = MVPred.BMVPred[0]->mv[f_back][1];
        }

        //validPredictors++;

        b_MV_px[1] = MVPred.BMVPred[0]->mv[b_back][0];
        b_MV_py[1] = MVPred.BMVPred[0]->mv[b_back][1];

        //classifying candidate mv
        if(b_MV_py[1] & 4)
        {
            MV_px_oppField[opposite] = b_MV_px[1];
            MV_py_oppField[opposite] = b_MV_py[1];
            ++opposite;
        }
        else
        {
            MV_px_sameField[same] = b_MV_px[1];
            MV_py_sameField[same] = b_MV_py[1];
            ++same;
        }
    }

    if(MVPred.CMVPred[0])
    {
        if(MVPred.FieldMB[0][2])
        {
            f_MV_px[2] = (MVPred.CMVPred[0]->mv[f_back][0]
                                    + MVPred.CMVPred[0]->mv_bottom[f_back][0] + 1)>>1;
            f_MV_py[2] = (MVPred.CMVPred[0]->mv[f_back][1]
                                    + MVPred.CMVPred[0]->mv_bottom[f_back][1] + 1)>>1;
        }
        else
        {
            f_MV_px[2] = MVPred.CMVPred[0]->mv[f_back][0];
            f_MV_py[2] = MVPred.CMVPred[0]->mv[f_back][1];
        }
        //validPredictors++;
        b_MV_px[2] = MVPred.CMVPred[0]->mv[b_back][0];
        b_MV_py[2] = MVPred.CMVPred[0]->mv[b_back][1];

        //classifying candidate mv
        if(b_MV_py[2] & 4)
        {
            MV_px_oppField[opposite] = b_MV_px[2];
            MV_py_oppField[opposite] = b_MV_py[2];
            ++opposite;
        }
        else
        {
            MV_px_sameField[same] = b_MV_px[2];
            MV_py_sameField[same] = b_MV_py[2];
            ++same;
        }
    }

    //computing frame predictors
    if (same + opposite > 1)
    {
        // 2 or 3 predictors are available
        *f_x = (Ipp16s)median3(f_MV_px);
        *f_y = (Ipp16s)median3(f_MV_py);
    }
    else
    {
        // 1 or 0 predictor is available
        *f_x = f_MV_px[0]+f_MV_px[1]+f_MV_px[2];
        *f_y = f_MV_py[0]+f_MV_py[1]+f_MV_py[2];
    }

    //computation of Field MV predictors from candidate mv
    //3 predictors are available
    if((same == 3) || (opposite == 3))
    {
        *b_x = (Ipp16s)median3(b_MV_px);
        *b_y = (Ipp16s)median3(b_MV_py);
    }
    else if(same >= opposite)
    {
        *b_x = MV_px_sameField[0];
        *b_y = MV_py_sameField[0];
    }
    else
    {
        *b_x = MV_px_oppField[0];
        *b_y = MV_py_oppField[0];
    }
}

void CalculateInterlaceFrame1MV_B_Interpolate(VC1MVPredictors* MVPredictors,
                                              Ipp16s *f_x,Ipp16s *f_y,
                                              Ipp16s *b_x,Ipp16s *b_y)
{
    VC1MVPredictors MVPred;

    Ipp32u validPredictors = 0;
    Ipp16s f_MV_px[] = {0,0,0};
    Ipp16s f_MV_py[] = {0,0,0};

    Ipp16s b_MV_px[] = {0,0,0};
    Ipp16s b_MV_py[] = {0,0,0};

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[0])
    {
        if(MVPred.FieldMB[0][0])
        {
            f_MV_px[0] = (MVPred.AMVPred[0]->mv[0][0]
                                + MVPred.AMVPred[0]->mv_bottom[0][0] + 1)>>1;
            f_MV_py[0] = (MVPred.AMVPred[0]->mv[0][1]
                                + MVPred.AMVPred[0]->mv_bottom[0][1] + 1)>>1;

            b_MV_px[0] = (MVPred.AMVPred[0]->mv[1][0]
                                + MVPred.AMVPred[0]->mv_bottom[1][0] + 1)>>1;
            b_MV_py[0] = (MVPred.AMVPred[0]->mv[1][1]
                                + MVPred.AMVPred[0]->mv_bottom[1][1] + 1)>>1;
        }
        else
        {
            f_MV_px[0] = MVPred.AMVPred[0]->mv[0][0];
            f_MV_py[0] = MVPred.AMVPred[0]->mv[0][1];
            b_MV_px[0] = MVPred.AMVPred[0]->mv[1][0];
            b_MV_py[0] = MVPred.AMVPred[0]->mv[1][1];
        }

        validPredictors++;
    }

    if(MVPred.BMVPred[0])
    {
        if(MVPred.FieldMB[0][1])
        {
            f_MV_px[1] = (MVPred.BMVPred[0]->mv[0][0]
                            + MVPred.BMVPred[0]->mv_bottom[0][0] + 1)>>1;
            f_MV_py[1] = (MVPred.BMVPred[0]->mv[0][1]
                            + MVPred.BMVPred[0]->mv_bottom[0][1] + 1)>>1;
            b_MV_px[1] = (MVPred.BMVPred[0]->mv[1][0]
                            + MVPred.BMVPred[0]->mv_bottom[1][0] + 1)>>1;
            b_MV_py[1] = (MVPred.BMVPred[0]->mv[1][1]
                            + MVPred.BMVPred[0]->mv_bottom[1][1] + 1)>>1;
        }
        else
        {
            f_MV_px[1] = MVPred.BMVPred[0]->mv[0][0];
            f_MV_py[1] = MVPred.BMVPred[0]->mv[0][1];
            b_MV_px[1] = MVPred.BMVPred[0]->mv[1][0];
            b_MV_py[1] = MVPred.BMVPred[0]->mv[1][1];
        }

        validPredictors++;
    }

    if(MVPred.CMVPred[0])
    {
        if(MVPred.FieldMB[0][2])
        {
            f_MV_px[2] = (MVPred.CMVPred[0]->mv[0][0]
                                    + MVPred.CMVPred[0]->mv_bottom[0][0] + 1)>>1;
            f_MV_py[2] = (MVPred.CMVPred[0]->mv[0][1]
                                    + MVPred.CMVPred[0]->mv_bottom[0][1] + 1)>>1;
            b_MV_px[2] = (MVPred.CMVPred[0]->mv[1][0]
                                    + MVPred.CMVPred[0]->mv_bottom[1][0] + 1)>>1;
            b_MV_py[2] = (MVPred.CMVPred[0]->mv[1][1]
                                    + MVPred.CMVPred[0]->mv_bottom[1][1] + 1)>>1;
        }
        else
        {
            f_MV_px[2] = MVPred.CMVPred[0]->mv[0][0];
            f_MV_py[2] = MVPred.CMVPred[0]->mv[0][1];
            b_MV_px[2] = MVPred.CMVPred[0]->mv[1][0];
            b_MV_py[2] = MVPred.CMVPred[0]->mv[1][1];
        }
        validPredictors++;
    }

    //computing frame predictors
    if (validPredictors > 1)
    {
        // 2 or 3 predictors are available
        *f_x = (Ipp16s)median3(f_MV_px);
        *f_y = (Ipp16s)median3(f_MV_py);

        *b_x = (Ipp16s)median3(b_MV_px);
        *b_y = (Ipp16s)median3(b_MV_py);
    }
    else
    {
        // 1 or 0 predictor is available
        *f_x = f_MV_px[0]+f_MV_px[1]+f_MV_px[2];
        *f_y = f_MV_py[0]+f_MV_py[1]+f_MV_py[2];

        *b_x = b_MV_px[0]+b_MV_px[1]+b_MV_px[2];
        *b_y = b_MV_py[0]+b_MV_py[1]+b_MV_py[2];
    }
}

void Calculate4MVFrame_Adv(VC1MVPredictors* MVPredictors,
                              Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                              Ipp32u blk_num)
{
   VC1MVPredictors MVPred;

    Ipp32u validPredictors = 0;
    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[blk_num])
    {
        if(MVPred.FieldMB[blk_num][0])
        {
            MV_px[0] = (MVPred.AMVPred[blk_num]->mv[0][0] + MVPred.AMVPred[blk_num]->mv_bottom[0][0] + 1)>>1;
            MV_py[0] = (MVPred.AMVPred[blk_num]->mv[0][1] + MVPred.AMVPred[blk_num]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[0] = MVPred.AMVPred[blk_num]->mv[0][0];
            MV_py[0] = MVPred.AMVPred[blk_num]->mv[0][1];
        }
        validPredictors++;
    }

    if(MVPred.BMVPred[blk_num])
    {
        if(MVPred.FieldMB[blk_num][1])
        {
            MV_px[1] = (MVPred.BMVPred[blk_num]->mv[0][0] + MVPred.BMVPred[blk_num]->mv_bottom[0][0] + 1)>>1;
            MV_py[1] = (MVPred.BMVPred[blk_num]->mv[0][1] + MVPred.BMVPred[blk_num]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[1] = MVPred.BMVPred[blk_num]->mv[0][0];
            MV_py[1] = MVPred.BMVPred[blk_num]->mv[0][1];
        }
        validPredictors++;
    }

    if(MVPred.CMVPred[blk_num])
    {
        if(MVPred.FieldMB[blk_num][2])
        {
            MV_px[2] = (MVPred.CMVPred[blk_num]->mv[0][0] + MVPred.CMVPred[blk_num]->mv_bottom[0][0] + 1)>>1;
            MV_py[2] = (MVPred.CMVPred[blk_num]->mv[0][1] + MVPred.CMVPred[blk_num]->mv_bottom[0][1] + 1)>>1;
        }
        else
        {
            MV_px[2] = MVPred.CMVPred[blk_num]->mv[0][0];
            MV_py[2] = MVPred.CMVPred[blk_num]->mv[0][1];
        }
        validPredictors++;
    }

    //computing frame predictors
    if (validPredictors > 1)
    {
        // 2 or 3 predictors are available
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        // 1 or 0 predictor is available
        *pPredMVx = MV_px[0]+MV_px[1]+MV_px[2];
        *pPredMVy = MV_py[0]+MV_py[1]+MV_py[2];
    }
}


//       ---- ----            ---- ----
//      | B  | C  |          | C  | B  | (LAST MB in row)
//  ---- ---- ----     or     ---- ----
// | A  |Cur |               | A  |Cur |
//  ---- ----                 ---- ----
void PredictInterlaceFrame1MV(VC1Context* pContext)
{
    VC1MVPredictors MVPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;
    Ipp32s width = pContext->m_pSingleMB->widthMB;
    VC1MB *pA = NULL, *pB = NULL, *pC = NULL;

    memset(&MVPred,0,sizeof(VC1MVPredictors));

    //A predictor
    if(VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        pA=(pContext->m_pCurrMB - 1);

        if(!pA->IntraFlag)
        {
            if(VC1_GET_MBTYPE(pA->mbType) == VC1_MB_1MV_INTER ||   VC1_GET_MBTYPE(pA->mbType) == VC1_MB_2MV_INTER)
                MVPred.AMVPred[0] = &pA->m_pBlocks[0];
            else
                MVPred.AMVPred[0] = &pA->m_pBlocks[1];

            MVPred.FieldMB[0][0] = VC1_IS_MVFIELD(pA->mbType);
        }
    }

    if (VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
    {
        //B predictor
        pB=(pContext->m_pCurrMB - width);

        if(!pB->IntraFlag)
        {
            if(VC1_GET_MBTYPE(pB->mbType) == VC1_MB_4MV_INTER)
                MVPred.BMVPred[0] = &pB->m_pBlocks[2];
            else
                MVPred.BMVPred[0] = &pB->m_pBlocks[0];

            MVPred.FieldMB[0][1] = VC1_IS_MVFIELD(pB->mbType);
        }

        //C predictor
        if (VC1_IS_NO_RIGHT_MB(LeftTopRightPositionFlag))
        {
            //this block is not last in row
            pC=(pContext->m_pCurrMB - width + 1);
            if(!pC->IntraFlag)
            {
                if(VC1_GET_MBTYPE(pC->mbType) == VC1_MB_4MV_INTER)
                    MVPred.CMVPred[0] = &pC->m_pBlocks[2];
                else
                    MVPred.CMVPred[0] = &pC->m_pBlocks[0];

                MVPred.FieldMB[0][2] = VC1_IS_MVFIELD(pC->mbType);
            }
        }
        else
        {
            //this block is last in row
            pC=(pContext->m_pCurrMB - width -1);
            if(!pC->IntraFlag)
            {
                if(VC1_GET_MBTYPE(pC->mbType) == VC1_MB_4MV_INTER)
                    MVPred.CMVPred[0] = &pC->m_pBlocks[3];
                else if (VC1_GET_MBTYPE(pC->mbType) == VC1_MB_4MV_FIELD_INTER)
                    MVPred.CMVPred[0] = &pC->m_pBlocks[1];
                else
                    MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                MVPred.FieldMB[0][2] = VC1_IS_MVFIELD(pC->mbType);
            }
        }
    }

    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));
}


void PredictInterlace4MVFrame_Adv(VC1Context* pContext)
{
    VC1MVPredictors MVPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;
    Ipp32s width = pContext->m_pSingleMB->widthMB;
    VC1MB *pA = NULL, *pB = NULL, *pC = NULL;

    memset(&MVPred,0,sizeof(VC1MVPredictors));

    // A predictor
    if(VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        pA=(pCurrMB - 1); //candidate for blocks 0 and 2

        if(!pA->IntraFlag)
        {
            if(VC1_GET_MBTYPE(pA->mbType) == VC1_MB_1MV_INTER || VC1_GET_MBTYPE(pA->mbType) == VC1_MB_2MV_INTER)
            {
                //block 0
                MVPred.AMVPred[0] = &pA->m_pBlocks[0];
                //block 2
                MVPred.AMVPred[2] = &pA->m_pBlocks[0];
            }
            else if(VC1_GET_MBTYPE(pA->mbType) == VC1_MB_4MV_INTER)
            {
                //block 0
                MVPred.AMVPred[0] = &pA->m_pBlocks[1];
                //block 2
                MVPred.AMVPred[2] = &pA->m_pBlocks[3];
            }
            else
            {
                //block 0
                MVPred.AMVPred[0] = &pA->m_pBlocks[1];
                //block 2
                MVPred.AMVPred[2] = &pA->m_pBlocks[1];
            }

            MVPred.FieldMB[0][0] = VC1_IS_MVFIELD(pA->mbType);
            MVPred.FieldMB[2][0] = MVPred.FieldMB[0][0];
        }
    }

    if(VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
    {
        // B predictor
        pB=(pCurrMB - width);
        if(!pB->IntraFlag)
        {
            if(VC1_GET_MBTYPE(pB->mbType) == VC1_MB_1MV_INTER || VC1_GET_MBTYPE(pB->mbType) == VC1_MB_2MV_INTER)
            {
                //block 0
                MVPred.BMVPred[0] = &pB->m_pBlocks[0];
                //block 1
                MVPred.BMVPred[1] = &pB->m_pBlocks[0];
            }
            else if(VC1_GET_MBTYPE(pB->mbType) == VC1_MB_4MV_INTER)
            {
                //block 0
                MVPred.BMVPred[0] = &pB->m_pBlocks[2];
                //block 1
                MVPred.BMVPred[1] = &pB->m_pBlocks[3];
            }
            else
            {
                //block 0
                MVPred.BMVPred[0] = &pB->m_pBlocks[0];
                //block 1
                MVPred.BMVPred[1] = &pB->m_pBlocks[1];
            }
            MVPred.FieldMB[0][1] = VC1_IS_MVFIELD(pB->mbType);
            MVPred.FieldMB[1][1] = MVPred.FieldMB[0][1];
        }

        // C predictor
        if (VC1_IS_NO_RIGHT_MB(LeftTopRightPositionFlag))
        {
            //this block is not last in row
            pC=(pCurrMB - width + 1);
            if(!pC->IntraFlag)
            {
                if(VC1_GET_MBTYPE(pC->mbType) != VC1_MB_4MV_INTER)
                {
                    //block 0
                    MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                    //block 1
                    MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                }
                else
                {
                    //block 0
                    MVPred.CMVPred[0] = &pC->m_pBlocks[2];
                    //block 1
                    MVPred.CMVPred[1] = &pC->m_pBlocks[2];
                }
                MVPred.FieldMB[0][2] = VC1_IS_MVFIELD(pC->mbType);
                MVPred.FieldMB[1][2] = MVPred.FieldMB[0][2];
            }
        }
        else
        {
            //this block is last in row
            pC=(pCurrMB - width -1);
            if(!pC->IntraFlag)
            {
                if(VC1_GET_MBTYPE(pC->mbType) == VC1_MB_1MV_INTER || VC1_GET_MBTYPE(pC->mbType) == VC1_MB_2MV_INTER)
                {
                    //block 0
                    MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                    //block 1
                    MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                }
                else if(VC1_GET_MBTYPE(pC->mbType) == VC1_MB_4MV_INTER)
                {
                    //block 0
                    MVPred.CMVPred[0] = &pC->m_pBlocks[3];
                    //block 1
                    MVPred.CMVPred[1] = &pC->m_pBlocks[3];
                }
                else
                {
                    //block 0
                    MVPred.CMVPred[0] = &pC->m_pBlocks[1];
                    //block 1
                    MVPred.CMVPred[1] = &pC->m_pBlocks[1];
                }
                MVPred.FieldMB[0][2] = VC1_IS_MVFIELD(pC->mbType);
                MVPred.FieldMB[1][2] = MVPred.FieldMB[0][2];
            }
        }
    }

    MVPred.AMVPred[1] = &pCurrMB->m_pBlocks[0];
    MVPred.BMVPred[2] = &pCurrMB->m_pBlocks[0];
    MVPred.CMVPred[2] = &pCurrMB->m_pBlocks[1];
    MVPred.AMVPred[3] = &pCurrMB->m_pBlocks[2];
    MVPred.BMVPred[3] = &pCurrMB->m_pBlocks[0];
    MVPred.CMVPred[3] = &pCurrMB->m_pBlocks[1];

    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));
}

void PredictInterlace4MVField_Adv(VC1Context* pContext)
{
    VC1MVPredictors MVPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;
    Ipp32s width = pContext->m_pSingleMB->widthMB;
    VC1MB *pA = NULL, *pB = NULL, *pC = NULL;

    memset(&MVPred,0,sizeof(VC1MVPredictors));

    // A predictor
    if(VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        pA=(pCurrMB - 1);

        if(!pA->IntraFlag)
        {
            switch(VC1_GET_MBTYPE(pA->mbType))
            {
            case VC1_MB_1MV_INTER:
                {
                    MVPred.AMVPred[0] = &pA->m_pBlocks[0];
                    MVPred.AMVPred[2] = &pA->m_pBlocks[0];
                }
                break;
            case VC1_MB_2MV_INTER:
                {
                    MVPred.AMVPred[0] = &pA->m_pBlocks[0];
                    MVPred.AMVPred[2] = &pA->m_pBlocks[0];
                    MVPred.FieldMB[2][0] = 1;
                }
                break;
            case VC1_MB_4MV_INTER:
                {
                    MVPred.AMVPred[0] = &pA->m_pBlocks[1];
                    MVPred.AMVPred[2] = &pA->m_pBlocks[3];
                }
                break;
            case VC1_MB_4MV_FIELD_INTER:
                {
                    MVPred.AMVPred[0] = &pA->m_pBlocks[1];
                    MVPred.AMVPred[2] = &pA->m_pBlocks[1];
                    MVPred.FieldMB[2][0] = 1;
                }
                break;
            default:
                pA=NULL;
                break;
            }
        }
    }

    if(VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
    {
        // B predictor
        pB=(pCurrMB - width);
        if(!pB->IntraFlag)
        {
            switch(VC1_GET_MBTYPE(pB->mbType))
            {
            case VC1_MB_1MV_INTER:
                {
                    MVPred.BMVPred[0] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[1] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[2] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[3] = &pB->m_pBlocks[0];
                }
                break;
            case VC1_MB_2MV_INTER:
                {
                    MVPred.BMVPred[0] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[1] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[2] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[3] = &pB->m_pBlocks[0];
                    MVPred.FieldMB[2][1] = 1;
                    MVPred.FieldMB[3][1] = 1;
                }
                break;
            case VC1_MB_4MV_INTER:
                {
                    MVPred.BMVPred[0] = &pB->m_pBlocks[2];
                    MVPred.BMVPred[1] = &pB->m_pBlocks[3];
                    MVPred.BMVPred[2] = &pB->m_pBlocks[2];
                    MVPred.BMVPred[3] = &pB->m_pBlocks[3];
                }
                break;
            case VC1_MB_4MV_FIELD_INTER:
                {
                    MVPred.BMVPred[0] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[1] = &pB->m_pBlocks[1];
                    MVPred.BMVPred[2] = &pB->m_pBlocks[0];
                    MVPred.BMVPred[3] = &pB->m_pBlocks[1];
                    MVPred.FieldMB[2][1] = 1;
                    MVPred.FieldMB[3][1] = 1;
                }
                break;
            default:
                pB=NULL;
                break;
            }
        }

        // C predictor
        if (VC1_IS_NO_RIGHT_MB(LeftTopRightPositionFlag))
        {
            //this block is not last in row
            pC=(pCurrMB - width + 1);
            if(!pC->IntraFlag)
            {
                switch(VC1_GET_MBTYPE(pC->mbType))
                {
                case VC1_MB_1MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[0];
                    }
                    break;
                case VC1_MB_2MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[0];
                        MVPred.FieldMB[2][2] = 1;
                        MVPred.FieldMB[3][2] = 1;
                    }
                    break;
                case VC1_MB_4MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[2];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[2];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[2];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[2];
                    }
                    break;
                case VC1_MB_4MV_FIELD_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[0];
                        MVPred.FieldMB[2][2] = 1;
                        MVPred.FieldMB[3][2] = 1;
                    }
                    break;
                default:
                    pC=NULL;
                    break;
                }
            }
        }
        else
        {
            //this block is last in row
            pC=(pCurrMB - width -1);
            if(!pC->IntraFlag)
            {
                switch(VC1_GET_MBTYPE(pC->mbType))
                {
                case VC1_MB_1MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[0];
                    }
                    break;
                case VC1_MB_2MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[0];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[0];
                        MVPred.FieldMB[2][2] = 1;
                        MVPred.FieldMB[3][2] = 1;
                    }
                    break;
                case VC1_MB_4MV_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[3];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[3];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[3];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[3];
                    }
                    break;
                case VC1_MB_4MV_FIELD_INTER:
                    {
                        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
                        MVPred.CMVPred[1] = &pC->m_pBlocks[1];
                        MVPred.CMVPred[2] = &pC->m_pBlocks[1];
                        MVPred.CMVPred[3] = &pC->m_pBlocks[1];
                        MVPred.FieldMB[2][2] = 1;
                        MVPred.FieldMB[3][2] = 1;
                    }
                    break;
                default:
                    pC=NULL;
                    break;
                }
            }
        }
    }


    MVPred.AMVPred[1] = &pCurrMB->m_pBlocks[0];
    MVPred.AMVPred[3] = &pCurrMB->m_pBlocks[0];
    MVPred.FieldMB[3][0] = 1;

    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));

}
//2 Field MV candidate MV derivation
void PredictInterlace2MV_Field_Adv(VC1MB* pCurrMB,
                                   Ipp16s pPredMVx[2],Ipp16s pPredMVy[2],
                                   Ipp16s backTop, Ipp16s backBottom,
                                   Ipp32u widthMB)
{
    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;
    Ipp32u OppositeTopField = 0;
    Ipp32u OppositeBottomField = 0;
    Ipp32u SameTopField = 0;
    Ipp32u SameBottomField = 0;

    Ipp16s MV_px[2][3] = {{0,0,0},{0,0,0}};
    Ipp16s MV_py[2][3] = {{0,0,0},{0,0,0}};

    Ipp16s MV_px_sameField[2][3] = {{0,0,0},{0,0,0}};
    Ipp16s MV_py_sameField[2][3] = {{0,0,0},{0,0,0}};

    Ipp16s MV_px_oppField[2][3] = {{0,0,0},{0,0,0}};
    Ipp16s MV_py_oppField[2][3] = {{0,0,0},{0,0,0}};

    VC1MB *pA = NULL, *pB = NULL, *pC = NULL;

    // A predictor
    if(VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        pA=(pCurrMB - 1);

        if(!pA->IntraFlag)
        {
            switch(VC1_GET_MBTYPE(pA->mbType))
            {
            case VC1_MB_1MV_INTER:
                {
                    //add mv of A
                    //top field
                    MV_px[0][0] = pA->m_pBlocks[0].mv[backTop][0];
                    MV_py[0][0] = pA->m_pBlocks[0].mv[backTop][1];

                    //bottom field
                    MV_px[1][0] = pA->m_pBlocks[0].mv[backBottom][0];
                    MV_py[1][0] = pA->m_pBlocks[0].mv[backBottom][1];
                }
                break;
            case VC1_MB_2MV_INTER:
                {
                    //top field
                    //add the top field mv
                    MV_px[0][0] = pA->m_pBlocks[0].mv[backTop][0];
                    MV_py[0][0] = pA->m_pBlocks[0].mv[backTop][1];

                    //bottom field
                    //add the bottom field block mv
                    MV_px[1][0] = pA->m_pBlocks[0].mv_bottom[backBottom][0];
                    MV_py[1][0] = pA->m_pBlocks[0].mv_bottom[backBottom][1];
                }
                break;
            case VC1_MB_4MV_INTER:
                {
                    //top field
                    // add the top right block mv
                    MV_px[0][0] = pA->m_pBlocks[1].mv[backTop][0];
                    MV_py[0][0] = pA->m_pBlocks[1].mv[backTop][1];

                    //bottom field
                    //add the bottom right block mv
                    MV_px[1][0] = pA->m_pBlocks[3].mv[backBottom][0];
                    MV_py[1][0] = pA->m_pBlocks[3].mv[backBottom][1];
                }
                break;
            case VC1_MB_4MV_FIELD_INTER:
                {
                    //top field
                    //add the top right field mv
                    MV_px[0][0] = pA->m_pBlocks[1].mv[backTop][0];
                    MV_py[0][0] = pA->m_pBlocks[1].mv[backTop][1];

                    //bottom feild
                    //add the bottom right field mv
                    MV_px[1][0] = pA->m_pBlocks[1].mv_bottom[backBottom][0];
                    MV_py[1][0] = pA->m_pBlocks[1].mv_bottom[backBottom][1];
                }
                break;
            default:
                pA=NULL;
                break;
            }

            //classifying top candidate mv
            if(MV_py[0][0] & 4)
            {
                MV_px_oppField[0][OppositeTopField] = MV_px[0][0];
                MV_py_oppField[0][OppositeTopField] = MV_py[0][0];
                ++OppositeTopField;
            }
            else
            {
                MV_px_sameField[0][SameTopField] = MV_px[0][0];
                MV_py_sameField[0][SameTopField] = MV_py[0][0];
                ++SameTopField;
            }

            //classifying bottom candidate mv
            if(MV_py[1][0] & 4)
            {
                MV_px_oppField[1][OppositeBottomField] = MV_px[1][0];
                MV_py_oppField[1][OppositeBottomField] = MV_py[1][0];
                ++OppositeBottomField;
            }
            else
            {
                MV_px_sameField[1][SameBottomField] = MV_px[1][0];
                MV_py_sameField[1][SameBottomField] = MV_py[1][0];
                ++SameBottomField;
            }
        }
    }

    if(VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
    {
        // B predictor
        pB=(pCurrMB - widthMB);
        if(!pB->IntraFlag)
        {
            switch(VC1_GET_MBTYPE(pB->mbType))
            {
            case VC1_MB_1MV_INTER:
                {
                    //top field
                    MV_px[0][1] = pB->m_pBlocks[0].mv[backTop][0];
                    MV_py[0][1] = pB->m_pBlocks[0].mv[backTop][1];

                    //bottom field
                    MV_px[1][1] = pB->m_pBlocks[0].mv[backBottom][0];
                    MV_py[1][1] = pB->m_pBlocks[0].mv[backBottom][1];
                }
                break;
            case VC1_MB_2MV_INTER:
                {
                    //top field
                    //add the top field mv
                    MV_px[0][1] = pB->m_pBlocks[0].mv[backTop][0];
                    MV_py[0][1] = pB->m_pBlocks[0].mv[backTop][1];

                    //bottom field
                    //add the bottom field mv
                    MV_px[1][1] = pB->m_pBlocks[0].mv_bottom[backBottom][0];
                    MV_py[1][1] = pB->m_pBlocks[0].mv_bottom[backBottom][1];
                }
                break;
            case VC1_MB_4MV_INTER:
                {
                    //top field
                    //add the bottom left block mv
                    MV_px[0][1] = pB->m_pBlocks[2].mv[backTop][0];
                    MV_py[0][1] = pB->m_pBlocks[2].mv[backTop][1];

                    //bottom field
                    //add the bottom left block mv
                    MV_px[1][1] = pB->m_pBlocks[2].mv[backBottom][0];
                    MV_py[1][1] = pB->m_pBlocks[2].mv[backBottom][1];
                }
                break;
            case VC1_MB_4MV_FIELD_INTER:
                {
                    //top field
                    //add the top left field mv
                    MV_px[0][1] = pB->m_pBlocks[0].mv[backTop][0];
                    MV_py[0][1] = pB->m_pBlocks[0].mv[backTop][1];

                    //bottom field
                    //add the bottom left field mv
                    MV_px[1][1] = pB->m_pBlocks[0].mv_bottom[backBottom][0];
                    MV_py[1][1] = pB->m_pBlocks[0].mv_bottom[backBottom][1];
                }
                break;
            default:
                pB=NULL;
                break;
            }

            //classifying top candidate mv
            if(MV_py[0][1] & 4)
            {
                MV_px_oppField[0][OppositeTopField] = MV_px[0][1];
                MV_py_oppField[0][OppositeTopField] = MV_py[0][1];
                ++OppositeTopField;
            }
            else
            {
                MV_px_sameField[0][SameTopField] = MV_px[0][1];
                MV_py_sameField[0][SameTopField] = MV_py[0][1];
                ++SameTopField;
            }

            //classifying bottom candidate mv
            if(MV_py[1][1] & 4)
            {
                MV_px_oppField[1][OppositeBottomField] = MV_px[1][1];
                MV_py_oppField[1][OppositeBottomField] = MV_py[1][1];
                ++OppositeBottomField;
            }
            else
            {
                MV_px_sameField[1][SameBottomField] = MV_px[1][1];
                MV_py_sameField[1][SameBottomField] = MV_py[1][1];
                ++SameBottomField;
            }
        }

        // C predictor
        if (VC1_IS_NO_RIGHT_MB(LeftTopRightPositionFlag))
        {
            //this block is not last in row
            pC=(pCurrMB - widthMB + 1);
            if(!pC->IntraFlag)
            {
                switch(VC1_GET_MBTYPE(pC->mbType))
                {
                case VC1_MB_1MV_INTER:
                    {
                        //top field
                        MV_px[0][2] = pC->m_pBlocks[0].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[0].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[0].mv[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[0].mv[backBottom][1];
                    }
                    break;
                case VC1_MB_2MV_INTER:
                    {
                        //top field
                        //add the top field mv
                        MV_px[0][2] = pC->m_pBlocks[0].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[0].mv[backTop][1];

                        //bottom field
                        //add the bottom field block mv
                        MV_px[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][1];
                    }
                    break;
                case VC1_MB_4MV_INTER:
                    {
                        //top field
                        //add the bottom left block mv
                        MV_px[0][2] = pC->m_pBlocks[2].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[2].mv[backTop][1];

                        //bottom filed
                        MV_px[1][2] = pC->m_pBlocks[2].mv[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[2].mv[backBottom][1];
                    }
                    break;
                case VC1_MB_4MV_FIELD_INTER:
                    {
                        //top field
                        //add the top left field block mv
                        MV_px[0][2] = pC->m_pBlocks[0].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[0].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][1];
                    }
                    break;
                default:
                    pC=NULL;
                    break;
                }

                //classifying top candidate mv
                if(MV_py[0][2] & 4)
                {
                    MV_px_oppField[0][OppositeTopField] = MV_px[0][2];
                    MV_py_oppField[0][OppositeTopField] = MV_py[0][2];
                    ++OppositeTopField;
                }
                else
                {
                    MV_px_sameField[0][SameTopField] = MV_px[0][2];
                    MV_py_sameField[0][SameTopField] = MV_py[0][2];
                    ++SameTopField;
                }

                //classifying bottom candidate mv
                if(MV_py[1][2] & 4)
                {
                    MV_px_oppField[1][OppositeBottomField] = MV_px[1][2];
                    MV_py_oppField[1][OppositeBottomField] = MV_py[1][2];
                    ++OppositeBottomField;
                }
                else
                {
                    MV_px_sameField[1][SameBottomField] = MV_px[1][2];
                    MV_py_sameField[1][SameBottomField] = MV_py[1][2];
                    ++SameBottomField;
                }
            }
        }
        else
        {
            //this block is last in row
            pC=(pCurrMB - widthMB -1);
            if(!pC->IntraFlag)
            {
                switch(VC1_GET_MBTYPE(pC->mbType))
                {
                case VC1_MB_1MV_INTER:
                    {
                        //top field
                        MV_px[0][2] = pC->m_pBlocks[0].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[0].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[0].mv[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[0].mv[backBottom][1];
                    }
                    break;
                case VC1_MB_2MV_INTER:
                    {
                        //top field
                        //add the top field mv
                        MV_px[0][2] = pC->m_pBlocks[0].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[0].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[0].mv_bottom[backBottom][1];
                    }
                    break;
                case VC1_MB_4MV_INTER:
                    {
                        //top field
                        //add the bottom right block mv
                        MV_px[0][2] = pC->m_pBlocks[3].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[3].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[3].mv[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[3].mv[backBottom][1];
                    }
                    break;
                case VC1_MB_4MV_FIELD_INTER:
                    {
                        //top field
                        //add the top right field mv
                        MV_px[0][2] = pC->m_pBlocks[1].mv[backTop][0];
                        MV_py[0][2] = pC->m_pBlocks[1].mv[backTop][1];

                        //bottom field
                        MV_px[1][2] = pC->m_pBlocks[1].mv_bottom[backBottom][0];
                        MV_py[1][2] = pC->m_pBlocks[1].mv_bottom[backBottom][1];
                    }
                    break;
                default:
                    pC=NULL;
                    break;
                }
                //classifying top candidate mv
                if(MV_py[0][2] & 4)
                {
                    MV_px_oppField[0][OppositeTopField] = MV_px[0][2];
                    MV_py_oppField[0][OppositeTopField] = MV_py[0][2];
                    ++OppositeTopField;
                }
                else
                {
                    MV_px_sameField[0][SameTopField] = MV_px[0][2];
                    MV_py_sameField[0][SameTopField] = MV_py[0][2];
                    ++SameTopField;
                }

                //classifying bottom candidate mv
                if(MV_py[1][2] & 4)
                {
                    MV_px_oppField[1][OppositeBottomField] = MV_px[1][2];
                    MV_py_oppField[1][OppositeBottomField] = MV_py[1][2];
                    ++OppositeBottomField;
                }
                else
                {
                    MV_px_sameField[1][SameBottomField] = MV_px[1][2];
                    MV_py_sameField[1][SameBottomField] = MV_py[1][2];
                    ++SameBottomField;
                }
            }
        }
    }

    //computation of Field MV predictors from candidate mv
    //3 or 2 predictors are available
    if((SameTopField == 3) || (OppositeTopField == 3))
    {
        pPredMVx[0] = (Ipp16s)median3(MV_px[0]);
        pPredMVy[0] = (Ipp16s)median3(MV_py[0]);
    }
    else if(SameTopField >= OppositeTopField)
    {
        pPredMVx[0] = MV_px_sameField[0][0];
        pPredMVy[0] = MV_py_sameField[0][0];
    }
    else
    {
        pPredMVx[0] = MV_px_oppField[0][0];
        pPredMVy[0] = MV_py_oppField[0][0];
    }

    //computation of Field MV predictors from candidate mv
    //3 or 2 predictors are available
    if((SameBottomField == 3) || (OppositeBottomField == 3))
    {
        pPredMVx[1] = (Ipp16s)median3(MV_px[1]);
        pPredMVy[1] = (Ipp16s)median3(MV_py[1]);
    }
    else if(SameBottomField >= OppositeBottomField)
    {
        pPredMVx[1] = MV_px_sameField[1][0];
        pPredMVy[1] = MV_py_sameField[1][0];
    }
    else
    {
        pPredMVx[1] = MV_px_oppField[1][0];
        pPredMVy[1] = MV_py_oppField[1][0];
    }
}

void CalculateInterlace4MV_TopField_Adv(VC1MVPredictors* MVPredictors,
                                      Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                      Ipp32u blk_num)
{
    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;

    Ipp16s MV_px[3] = {0,0,0}; //first index - top/bottom
    Ipp16s MV_py[3] = {0,0,0}; //second - A,B,C predictors

    Ipp16s MV_px_sameField[3] = {0,0,0};
    Ipp16s MV_py_sameField[3] = {0,0,0};

    Ipp16s MV_px_oppField[3] = {0,0,0};
    Ipp16s MV_py_oppField[3] = {0,0,0};

    VC1MVPredictors MVPred;

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[blk_num])
    {
        MV_px[0] = MVPred.AMVPred[blk_num]->mv[0][0];
        MV_py[0] = MVPred.AMVPred[blk_num]->mv[0][1];
        //classifying top candidate mv
        if(MV_py[0] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[0];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[0];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[0];
            MV_py_sameField[validPredictorsSameField] = MV_py[0];
            ++validPredictorsSameField;
        }
    }

    if(MVPred.BMVPred[blk_num])
    {
        MV_px[1] = MVPred.BMVPred[blk_num]->mv[0][0];
        MV_py[1] = MVPred.BMVPred[blk_num]->mv[0][1];
        //classifying top candidate mv
        if(MV_py[1] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[1];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[1];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[1];
            MV_py_sameField[validPredictorsSameField] = MV_py[1];
            ++validPredictorsSameField;
        }
    }


    if(MVPred.CMVPred[blk_num])
    {
        MV_px[2] = MVPred.CMVPred[blk_num]->mv[0][0];
        MV_py[2] = MVPred.CMVPred[blk_num]->mv[0][1];
        //classifying top candidate mv
        if(MV_py[2] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[2];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[2];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[2];
            MV_py_sameField[validPredictorsSameField] = MV_py[2];
            ++validPredictorsSameField;
        }
    }

    //computation of top Field MV predictors from candidate mv
    //3 or 2 predictors are available
    if((validPredictorsSameField == 3) || (validPredictorsOppositeField == 3))
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else if(validPredictorsSameField >= validPredictorsOppositeField)
    {
        *pPredMVx = MV_px_sameField[0];
        *pPredMVy = MV_py_sameField[0];
    }
    else
    {
        *pPredMVx = MV_px_oppField[0];
        *pPredMVy = MV_py_oppField[0];
    }
 }

void CalculateInterlace4MV_BottomField_Adv(VC1MVPredictors* MVPredictors,
                                      Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                      Ipp32u blk_num)
{
    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;

    Ipp16s MV_px[3] = {0,0,0}; //first index - top/bottom
    Ipp16s MV_py[3] = {0,0,0}; //second - A,B,C predictors

    Ipp16s MV_px_sameField[3] = {0,0,0};
    Ipp16s MV_py_sameField[3] = {0,0,0};

    Ipp16s MV_px_oppField[3] = {0,0,0};
    Ipp16s MV_py_oppField[3] = {0,0,0};

    VC1MVPredictors MVPred;

    memcpy(&MVPred,MVPredictors,sizeof(VC1MVPredictors));

    if(MVPred.AMVPred[blk_num])
    {
        if(!MVPred.FieldMB[blk_num][0])
        {
            MV_px[0] = MVPred.AMVPred[blk_num]->mv[0][0];
            MV_py[0] = MVPred.AMVPred[blk_num]->mv[0][1];
        }
        else
        {
            MV_px[0] = MVPred.AMVPred[blk_num]->mv_bottom[0][0];
            MV_py[0] = MVPred.AMVPred[blk_num]->mv_bottom[0][1];
        }

        //classifying top candidate mv
        if(MV_py[0] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[0];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[0];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[0];
            MV_py_sameField[validPredictorsSameField] = MV_py[0];
            ++validPredictorsSameField;
        }
    }

    if(MVPred.BMVPred[blk_num])
    {
        if(!MVPred.FieldMB[blk_num][1])
        {
            MV_px[1] = MVPred.BMVPred[blk_num]->mv[0][0];
            MV_py[1] = MVPred.BMVPred[blk_num]->mv[0][1];
        }
        else
        {
            MV_px[1] = MVPred.BMVPred[blk_num]->mv_bottom[0][0];
            MV_py[1] = MVPred.BMVPred[blk_num]->mv_bottom[0][1];
        }

        //classifying top candidate mv
        if(MV_py[1] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[1];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[1];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[1];
            MV_py_sameField[validPredictorsSameField] = MV_py[1];
            ++validPredictorsSameField;
        }
    }


    if(MVPred.CMVPred[blk_num])
    {
        if(!MVPred.FieldMB[blk_num][2])
        {
            MV_px[2] = MVPred.CMVPred[blk_num]->mv[0][0];
            MV_py[2] = MVPred.CMVPred[blk_num]->mv[0][1];
        }
        else
        {
            MV_px[2] = MVPred.CMVPred[blk_num]->mv_bottom[0][0];
            MV_py[2] = MVPred.CMVPred[blk_num]->mv_bottom[0][1];

        }

        //classifying top candidate mv
        if(MV_py[2] & 4)
        {
            MV_px_oppField[validPredictorsOppositeField] = MV_px[2];
            MV_py_oppField[validPredictorsOppositeField] = MV_py[2];
            ++validPredictorsOppositeField;
        }
        else
        {
            MV_px_sameField[validPredictorsSameField] = MV_px[2];
            MV_py_sameField[validPredictorsSameField] = MV_py[2];
            ++validPredictorsSameField;
        }
    }

    //computation of top Field MV predictors from candidate mv
    //3 or 2 predictors are available
    if((validPredictorsSameField == 3) || (validPredictorsOppositeField == 3))
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else if(validPredictorsSameField >= validPredictorsOppositeField)
    {
        *pPredMVx = MV_px_sameField[0];
        *pPredMVy = MV_py_sameField[0];
    }
    else
    {
        *pPredMVx = MV_px_oppField[0];
        *pPredMVy = MV_py_oppField[0];
    }
 }

void DecodeMVDiff_Adv(VC1Context* pContext,Ipp16s* pdmv_x, Ipp16s* pdmv_y)
{
    IppStatus ret;
    Ipp32s index;
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp32s sign;
    Ipp32s val;
    VC1PictureLayerHeader* picHeader = pContext->m_picLayerHeader;

    static const Ipp8u offset_table[2][9] = { {0, 1, 2, 4, 8, 16, 32, 64, 128},
                                       {0, 1, 3, 7, 15, 31, 63, 127, 255}};
    static const Ipp8u* curr_offset;

#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    ret = ippiDecodeHuffmanOne_1u32s (
        &pContext->m_bitstream.pBitstream,
        &pContext->m_bitstream.bitOffset,
        &val,
        picHeader->m_pCurrMVDifftbl
        );

    VM_ASSERT(ret == ippStsNoErr);

    index = val & 0x000000FF;

    if (index != 72)
    {
        val = val>>8;
        Ipp32s index1 = val & 0x000000FF; //index%9
        val = val>>8;
        Ipp32s index2 = val & 0x000000FF; //index/9

        Ipp32s extend_x = (picHeader->DMVRANGE & VC1_DMVRANGE_HORIZONTAL_RANGE)?1:0;
        Ipp32s extend_y = (picHeader->DMVRANGE & VC1_DMVRANGE_VERTICAL_RANGE)?1:0;

        curr_offset = offset_table[extend_x];

        if (index1)
        {
            VC1_GET_BITS(index1 + extend_x,val);
            sign = -(val & 1);
            dmv_x = (Ipp16s)((sign ^ ( (val >> 1) + curr_offset[index1])) - sign);
        }
        else
            dmv_x = 0;

        curr_offset = offset_table[extend_y];

        if (index2)
        {
            VC1_GET_BITS(index2 + extend_y,val);
            sign = -(val & 1);
            dmv_y = (Ipp16s)((sign ^ ( (val >> 1) + curr_offset[index2])) - sign);
        }
        else
            dmv_y =0;
    }
    else
    {
        Ipp8u k_x = picHeader->m_pCurrMVRangetbl->k_x;
        Ipp8u k_y = picHeader->m_pCurrMVRangetbl->k_y;

        Ipp32s tmp_dmv_x = 0;
        Ipp32s tmp_dmv_y = 0;

        VC1_GET_BITS(k_x, tmp_dmv_x);
        VC1_GET_BITS(k_y, tmp_dmv_y);

        dmv_x = (Ipp16s)tmp_dmv_x;
        dmv_y = (Ipp16s)tmp_dmv_y;
    }
   // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("DMV [X,Y] = [%d, %d]\n"), dmv_x, dmv_y);

    //dMV scaling in case of fields and Half pel resolution
    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        if (pContext->m_picLayerHeader->MVMODE == VC1_MVMODE_HPELBI_1MV ||
            pContext->m_picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
        {
            dmv_x <<=1;
            dmv_y <<=1;
        }
    }

    *pdmv_x=dmv_x;
    *pdmv_y=dmv_y;

#ifdef UMC_STREAM_ANALYZER
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsMV += CalculateUsedBits(Bitsream,pContext->m_bitstream);
#endif
}

Ipp8u DecodeMVDiff_TwoReferenceField_Adv(VC1Context* pContext,
                                         Ipp16s* pdmv_x, Ipp16s* pdmv_y)
{
    IppStatus ret;
    Ipp32s index;
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp32s sign;
    Ipp32s val;
    Ipp8u predictor_flag;

    VC1PictureLayerHeader* picHeader = pContext->m_picLayerHeader;


    static Ipp8u offset_table[2][9] = {{0, 1, 2, 4, 8, 16, 32, 64, 128},
                                             {0, 1, 3, 7, 15, 31, 63, 127, 255}};
    static Ipp8u size_table[16]   = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
    Ipp8u* curr_offset;

#ifdef UMC_STREAM_ANALYZER
    VC1Bitstream Bitsream;
    Bitsream.pBitstream = pContext->m_bitstream.pBitstream;
    Bitsream.bitOffset = pContext->m_bitstream.bitOffset;
#endif

    ret = ippiDecodeHuffmanOne_1u32s (
        &pContext->m_bitstream.pBitstream,
        &pContext->m_bitstream.bitOffset,
        &val,
        picHeader->m_pCurrMVDifftbl
        );

    VM_ASSERT(ret == ippStsNoErr);

    index = val & 0x000000FF;

    if (index != 126)
    {
        Ipp32s extend_x = (picHeader->DMVRANGE & VC1_DMVRANGE_HORIZONTAL_RANGE)? 1:0;
        Ipp32s extend_y = (picHeader->DMVRANGE & VC1_DMVRANGE_VERTICAL_RANGE)  ? 1:0;

        val = val>>8;
        Ipp32s index1 = val & 0x000000FF; //index%9
        val = val>>8;
        Ipp32s index2 = val & 0x000000FF; //index/9

        curr_offset = offset_table[extend_x];

        if (index1)
        {
            VC1_GET_BITS(index1 + extend_x,val);
            sign = -(val & 1);
            dmv_x = (Ipp16s)((sign ^ ( (val >> 1) + curr_offset[index1])) - sign);
        }
        else
            dmv_x = 0;

        curr_offset = offset_table[extend_y];

        if (index2 > 1)
        {
            VC1_GET_BITS(size_table[index2 + 2*extend_y],val);
            sign = -(val & 1);
            dmv_y = (Ipp16s)((sign ^ ( (val >> 1) + curr_offset[index2>>1])) - sign);
        }
        else
            dmv_y =0;

        predictor_flag = (Ipp8u)(index2 & 1);
    }
    else
    {
        Ipp32s k_x = picHeader->m_pCurrMVRangetbl->k_x;
        Ipp32s k_y = picHeader->m_pCurrMVRangetbl->k_y;

        Ipp32s tmp_dmv_x = 0;
        Ipp32s tmp_dmv_y = 0;

        VC1_GET_BITS(k_x, tmp_dmv_x);
        VC1_GET_BITS(k_y, tmp_dmv_y);

        dmv_x = (Ipp16s)tmp_dmv_x;
        dmv_y = (Ipp16s)tmp_dmv_y;

        predictor_flag = (Ipp8u)(dmv_y & 1);
        dmv_y = (dmv_y + 1) >> 1; // differ from standard dmv_y = (dmv_y + predictor_flag) >> 1;
    }

   // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("DMV [X,Y] = [%d, %d]\n"), dmv_x, dmv_y);

    if (picHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
    {
        dmv_x <<=1;
        dmv_y <<=1;
    }

    * pdmv_x = dmv_x;
    * pdmv_y = dmv_y;

#ifdef UMC_STREAM_ANALYZER
    pContext->m_pCurrMB->pMbAnalyzInfo->dwNumBitsMV += CalculateUsedBits(Bitsream,pContext->m_bitstream);
#endif

    return predictor_flag;
}
void DeriveSecondStageChromaMV_Interlace(VC1Context* pContext, Ipp16s* xMV, Ipp16s* yMV)
{
    VC1MB *pMB = pContext->m_pCurrMB;
    Ipp32s IX, IY;
    static const Ipp8u RndTbl[4] = {0, 0, 0, 1};
    static const Ipp8u RndTblField[16]   = {0, 0, 1, 2, 4, 4, 5, 6, 2, 2, 3, 8, 6, 6, 7, 12};
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV_BBL,VM_STRING("MV(%d,%d)\n"),*xMV,*yMV);

    if(((*xMV) == VC1_MVINTRA) || ((*yMV) == VC1_MVINTRA))
    {
        return;
    }
    else
    {
        Ipp32s CMV_X, CMV_Y;

        IX = *xMV;
        IY = *yMV;

        CMV_X = (IX + RndTbl[IX & 3]) >> 1;

        if ( (VC1_GET_MBTYPE(pContext->m_pCurrMB->mbType) == VC1_MB_2MV_INTER)
            || (VC1_GET_MBTYPE(pContext->m_pCurrMB->mbType) == VC1_MB_4MV_FIELD_INTER) )
            CMV_Y = (IY >> 4)*8 + RndTblField [IY & 0xF];
        else
            CMV_Y = (IY + RndTbl[IY & 3]) >> 1;

        *xMV              = (Ipp16s)CMV_X;
        *yMV              = (Ipp16s)CMV_Y;

        pMB->m_pBlocks[4].mv[0][0] = *xMV;
        pMB->m_pBlocks[4].mv[0][1] = *yMV;
        pMB->m_pBlocks[5].mv[0][0] = *xMV;
        pMB->m_pBlocks[5].mv[0][1] = *yMV;
    }
}

void Decode_InterlaceFrame_BMVTYPE(VC1Context* pContext)
{
    Ipp32s value=0;
    VC1_GET_BITS(1, value);
    if (value)
    {
        VC1_GET_BITS(1, value);
        if (value)
        {
            pContext->m_pCurrMB->mbType=(Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_INTERP);
        }
        else
        {
            pContext->m_pCurrMB->mbType=(pContext->m_picLayerHeader->BFRACTION)?
                (Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_FORWARD)
                :(Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_BACKWARD);
        }
    }
    else
    {
        pContext->m_pCurrMB->mbType=(pContext->m_picLayerHeader->BFRACTION)?
            (Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_BACKWARD)
            :(Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_FORWARD);
    }
}

void Decode_InterlaceField_BMVTYPE(VC1Context* pContext)
{
    Ipp32s value=0;

    VC1_GET_BITS(1, value);
    if (value)
    {
        VC1_GET_BITS(1, value);
        if (value)
        {
            //11
            pContext->m_pCurrMB->mbType = (Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_INTERP);
        }
        else
        {
            //10
            pContext->m_pCurrMB->mbType = (Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_DIRECT);
        }
    }
    else
    {
        //0
        pContext->m_pCurrMB->mbType = (Ipp8u)(pContext->m_pCurrMB->mbType|VC1_MB_BACKWARD);
    }
}

void ScaleOppositePredPPic(VC1PictureLayerHeader* picLayerHeader,Ipp16s *x, Ipp16s *y)
{
    Ipp16s scaleX = *x;
    Ipp16s scaleY = *y;

    const VC1PredictScaleValuesPPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValuePPictbl;
    Ipp16u scaleopp = ScaleValuesTable->scaleopp;

    if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
    {
        scaleX  = scaleX >> 1;
        scaleY  = scaleY >> 1;

        scaleX = ((scaleX*scaleopp)>>8);
        scaleY = ((scaleY*scaleopp)>>8);

        scaleX  = scaleX << 1;
        scaleY  = scaleY << 1;
    }
    else
    {
        scaleX = ((scaleX*scaleopp)>>8);
        scaleY = ((scaleY*scaleopp)>>8);
    }

    (*x) = scaleX;
    (*y) = scaleY;
}

void ScaleSamePredPPic(VC1PictureLayerHeader* picLayerHeader, Ipp16s *x, Ipp16s *y, Ipp32s dominant, Ipp32s fieldFlag)
{
    Ipp16s scaleX = *x;
    Ipp16s scaleY = *y;

    const VC1PredictScaleValuesPPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValuePPictbl;

    Ipp16u range_x = picLayerHeader->m_pCurrMVRangetbl->r_x;
    Ipp16u range_y = picLayerHeader->m_pCurrMVRangetbl->r_y;

    range_y  = range_y >> 1;

    if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
    {
        scaleX  = scaleX >> 1;
        scaleY  = scaleY >> 1;
    }

   // X
    if(vc1_abs_16s(scaleX) <= 255)
    {
        if(vc1_abs_16s(scaleX) < ScaleValuesTable->scalezone1_x)
           scaleX = ((scaleX*ScaleValuesTable->scalesame1)>>8);
        else
        {
            if(scaleX < 0)
                scaleX = ((scaleX * ScaleValuesTable->scalesame2)>>8) - (ScaleValuesTable->zone1offset_x);
            else
                scaleX = ( (scaleX * ScaleValuesTable->scalesame2)>>8 ) + (ScaleValuesTable->zone1offset_x);
        }
    }
    if(scaleX > range_x - 1)
        scaleX = range_x - 1;

    if(scaleX < - range_x)
      scaleX = -range_x;

    // Y
    if(vc1_abs_16s(scaleY) <= 63)
    {
        if(vc1_abs_16s(scaleY) < ScaleValuesTable->scalezone1_y)
            scaleY = ((scaleY*ScaleValuesTable->scalesame1)>>8);
        else
        {
            if(scaleY<0)
                scaleY = ((scaleY*ScaleValuesTable->scalesame2)>>8) - ScaleValuesTable->zone1offset_y;
            else
                scaleY = ((scaleY*ScaleValuesTable->scalesame2)>>8) + ScaleValuesTable->zone1offset_y;
        }
    }

    if(dominant && fieldFlag)
    {
        if(scaleY > (range_y))
        {
            scaleY = (range_y);
        }
        else if(scaleY < -(range_y) + 1)
        {
            scaleY = -(range_y) + 1;
        }
    }
    else
    {
        if(scaleY > range_y-1)
        {
            scaleY = range_y-1;
        }
        else if(scaleY < -(range_y))
        {
            scaleY = -(range_y);
        }
    }

    if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
    {
        scaleX  = scaleX << 1;
        scaleY  = scaleY << 1;
    }

    (*x) = scaleX;
    (*y) = scaleY;
}


void ScaleOppositePredBPic(VC1PictureLayerHeader* picLayerHeader, Ipp16s *x, Ipp16s *y,
                           Ipp32s dominant, Ipp32s fieldFlag, Ipp32s back)
{
    Ipp16s scaleX = *x;
    Ipp16s scaleY = *y;

    if(back && (picLayerHeader->CurrField == 0))
    {
        const VC1PredictScaleValuesBPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValueB_BPictbl;

        Ipp16s range_x = picLayerHeader->m_pCurrMVRangetbl->r_x;
        Ipp16s range_y = picLayerHeader->m_pCurrMVRangetbl->r_y;

        range_y  = range_y >> 1;

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX >> 1;
            scaleY  = scaleY >> 1;
        }

        // X
        if(vc1_abs_16s(scaleX) <= 255)
        {
            if(vc1_abs_16s(scaleX) < ScaleValuesTable->scalezone1_x)
                scaleX = (scaleX*ScaleValuesTable->scaleopp1)>>8;
            else
            {
                if(scaleX < 0)
                    scaleX = ((scaleX * ScaleValuesTable->scaleopp2)>>8) - (ScaleValuesTable->zone1offset_x);
                else
                    scaleX = ( (scaleX * ScaleValuesTable->scaleopp2)>>8 ) + (ScaleValuesTable->zone1offset_x);
            }
        }

        if(scaleX > range_x - 1)
            scaleX = range_x - 1;

        if(scaleX < - range_x)
            scaleX = -range_x;

        // Y
        if(vc1_abs_16s(scaleY) <= 63)
        {
            if(vc1_abs_16s(scaleY) < ScaleValuesTable->scalezone1_y)
                scaleY = ((scaleY*ScaleValuesTable->scaleopp1)>>8);
            else
            {
                if(scaleY<0)
                    scaleY = ((scaleY*ScaleValuesTable->scaleopp2)>>8) - ScaleValuesTable->zone1offset_y;
                else
                    scaleY = ((scaleY*ScaleValuesTable->scaleopp2)>>8) + ScaleValuesTable->zone1offset_y;
            }
        }

        if(dominant && fieldFlag)
        {
            if(scaleY > range_y - 1)
            {
                scaleY = range_y - 1;
            }
            else if(scaleY < -(range_y))
            {
                scaleY = -(range_y);
            }
        }
        else
        {
            if(scaleY > (range_y))
            {
                scaleY = (range_y);
            }
            else if(scaleY < -(range_y) + 1)
            {
                scaleY = -(range_y) + 1;
            }
        }

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX << 1;
            scaleY  = scaleY << 1;
        }
    }
    else
    {
        //forward
        const VC1PredictScaleValuesPPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValueP_BPictbl[back];
        Ipp16s scaleopp = ScaleValuesTable->scaleopp;

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX >> 1;
            scaleY  = scaleY >> 1;

            scaleX = ((scaleX*scaleopp)>>8);
            scaleY = ((scaleY*scaleopp)>>8);

            scaleX  = scaleX << 1;
            scaleY  = scaleY << 1;
        }
        else
        {
            scaleX = ((scaleX*scaleopp)>>8);
            scaleY = ((scaleY*scaleopp)>>8);
        }
    }
    (*x) = scaleX;
    (*y) = scaleY;
}

void ScaleSamePredBPic(VC1PictureLayerHeader* picLayerHeader,Ipp16s *x, Ipp16s *y,
                       Ipp32s dominant, Ipp32s fieldFlag, Ipp32s back)
{
    Ipp16s scaleX = *x;
    Ipp16s scaleY = *y;

    if(back && (picLayerHeader->CurrField == 0))
    {
        const VC1PredictScaleValuesBPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValueB_BPictbl;
        Ipp16s scalesame = ScaleValuesTable->scalesame;

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX >> 1;
            scaleY  = scaleY >> 1;

            scaleX = (Ipp16s)((scaleX*scalesame)>>8);
            scaleY = (Ipp16s)((scaleY*scalesame)>>8);

            scaleX  = scaleX << 1;
            scaleY  = scaleY << 1;
        }
        else
        {
            scaleX = (Ipp16s)((scaleX*scalesame)>>8);
            scaleY = (Ipp16s)((scaleY*scalesame)>>8);
        }

    }
    else
    {
        const VC1PredictScaleValuesPPic* ScaleValuesTable = picLayerHeader->m_pCurrPredScaleValueP_BPictbl[back];

        Ipp16s range_x = picLayerHeader->m_pCurrMVRangetbl->r_x;
        Ipp16s range_y = picLayerHeader->m_pCurrMVRangetbl->r_y;


        range_y  = range_y >> 1;

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX >> 1;
            scaleY  = scaleY >> 1;
        }

        // X
        if(vc1_abs_16s(scaleX) <= 255)
        {
            if(vc1_abs_16s(scaleX) < ScaleValuesTable->scalezone1_x)
                scaleX = (scaleX*ScaleValuesTable->scalesame1)>>8;
            else
            {
                if(scaleX < 0)
                    scaleX = ((scaleX * ScaleValuesTable->scalesame2)>>8) - (ScaleValuesTable->zone1offset_x);
                else
                    scaleX = ( (scaleX * ScaleValuesTable->scalesame2)>>8 ) + (ScaleValuesTable->zone1offset_x);
            }
        }
        if(scaleX > range_x - 1)
            scaleX = range_x - 1;

        if(scaleX < - range_x)
            scaleX = -range_x;

        // Y
        if(vc1_abs_16s(scaleY) <= 63)
        {
            if(abs(scaleY) < ScaleValuesTable->scalezone1_y)
                scaleY = (scaleY*ScaleValuesTable->scalesame1)>>8;
            else
            {
                if(scaleY<0)
                    scaleY = ((scaleY*ScaleValuesTable->scalesame2)>>8) - ScaleValuesTable->zone1offset_y;
                else
                    scaleY = ((scaleY*ScaleValuesTable->scalesame2)>>8) + ScaleValuesTable->zone1offset_y;
            }
        }

        if(dominant && fieldFlag)
        {
            if(scaleY > (range_y))
            {
                scaleY = (range_y);
            }
            else if(scaleY < -(range_y) + 1)
            {
                scaleY = -(range_y) + 1;
            }
        }
        else
        {
            if(scaleY > range_y - 1)
            {
                scaleY = range_y - 1;
            }
            else if(scaleY < -(range_y))
            {
                scaleY = -(range_y);
            }
        }

        if (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV || picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV)
        {
            scaleX  = scaleX << 1;
            scaleY  = scaleY << 1;
        }
    }

    (*x) = scaleX;
    (*y) = scaleY;
}

void HybridFieldMV(VC1Context* pContext,Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                   Ipp16s MV_px[3],Ipp16s MV_py[3])
{
    Ipp32u hybridmv_thresh;
    Ipp16u sumA;
    Ipp16u sumC;
    Ipp32u HybridPred;

    hybridmv_thresh = 32;

    sumA = vc1_abs_16s(*pPredMVx - MV_px[0]) +vc1_abs_16s(*pPredMVy - MV_py[0]);

    if(sumA > hybridmv_thresh)
    {
        VC1_GET_BITS(1,HybridPred);

        *pPredMVx = MV_px[2 - HybridPred*2];
        *pPredMVy = MV_py[2 - HybridPred*2];
    }
    else
    {
        sumC = vc1_abs_16s(*pPredMVx - MV_px[2]) +vc1_abs_16s(*pPredMVy - MV_py[2]);

        if(sumC > hybridmv_thresh)
        {
            VC1_GET_BITS(1,HybridPred);

            *pPredMVx = MV_px[2 - HybridPred*2];
            *pPredMVy = MV_py[2 - HybridPred*2];
        }
    }
}

void Field1MVPrediction(VC1Context* pContext)
{
    VC1MVPredictors MVPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1MB *pA = NULL, *pB = NULL, *pC = NULL;

    Ipp32u LeftTopRight = pCurrMB->LeftTopRightPositionFlag;
    Ipp32s width = pContext->m_pSingleMB->widthMB;

    memset(&MVPred,0,sizeof(VC1MVPredictors));

    if(LeftTopRight == VC1_COMMON_MB)
    {
        //all predictors are available
        pA = pCurrMB - width;
        pB = pCurrMB - width + 1;
        pC = pCurrMB - 1;

        if(!pA->IntraFlag)
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];

        if(!pB->IntraFlag)
            MVPred.BMVPred[0] = &pB->m_pBlocks[2];

        if(!pC->IntraFlag)
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_TOP_MB(LeftTopRight))
    {
        //A and B predictors are unavailable
        pC = pCurrMB - 1;

        if(!pC->IntraFlag)
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_LEFT_MB(LeftTopRight))
    {
        //C predictor is unavailable
        pA = pCurrMB - width;
        pB = pCurrMB - width + 1;

        if(!pA->IntraFlag)
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        if(!pB->IntraFlag)
            MVPred.BMVPred[0] = &pB->m_pBlocks[2];
    }
    else if(VC1_IS_RIGHT_MB(LeftTopRight))
    {
        //all predictors are available
        pA = pCurrMB - width;
        pB = pCurrMB - width - 1;
        pC = pCurrMB - 1;

        if(!pA->IntraFlag)
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];

        if(!pB->IntraFlag)
            MVPred.BMVPred[0] = &pB->m_pBlocks[2];

        if(!pC->IntraFlag)
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_TOP_RIGHT_MB(LeftTopRight))
    {
        pC = pCurrMB - 1;

        if(!pC->IntraFlag)
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));
}


void Field4MVPrediction(VC1Context* pContext)
{
    VC1MVPredictors MVPred;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1MB *pA = NULL, *pB0 = NULL,*pB1 = NULL, *pC = NULL;

    Ipp32u LeftTopRight = pCurrMB->LeftTopRightPositionFlag;
    Ipp32s width = pContext->m_pSingleMB->widthMB;

    memset(&MVPred,0,sizeof(VC1MVPredictors));

    if(LeftTopRight == VC1_COMMON_MB)
    {
        //all predictors are available
        pA  = pCurrMB - width;
        pB0 = pCurrMB - width - 1;
        pB1 = pCurrMB - width + 1;
        pC  = pCurrMB - 1;

        if(!pA->IntraFlag)
        {
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];
            MVPred.AMVPred[1] = &pA->m_pBlocks[3];
        }
        if(!pC->IntraFlag)
        {
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
            MVPred.CMVPred[2] = &pC->m_pBlocks[3];
        }

        if(!pB0->IntraFlag)
            MVPred.BMVPred[0] = &pB0->m_pBlocks[3];

        if(!pB1->IntraFlag)
            MVPred.BMVPred[1] = &pB1->m_pBlocks[2];
    }
    else if(VC1_IS_TOP_MB(LeftTopRight))
    {
        pC = pCurrMB - 1;

        if(!pC->IntraFlag)
        {
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
            MVPred.CMVPred[2] = &pC->m_pBlocks[3];
        }
    }
    else if(VC1_IS_LEFT_MB(LeftTopRight))
    {
        pA = pCurrMB - width;
        pB1 = pCurrMB - width + 1;

        if(!pA->IntraFlag)
        {
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];
            MVPred.BMVPred[0] = &pA->m_pBlocks[3];
            MVPred.AMVPred[1] = &pA->m_pBlocks[3];
        }
        if(!pB1->IntraFlag)
            MVPred.BMVPred[1] = &pB1->m_pBlocks[2];
    }
    else if (VC1_IS_RIGHT_MB(LeftTopRight))
    {
        pA = pCurrMB - width;
        pB0 = pCurrMB - width - 1;
        pC = pCurrMB - 1;

        if(!pA->IntraFlag)
        {
            MVPred.AMVPred[0] = &pA->m_pBlocks[2];
            MVPred.AMVPred[1] = &pA->m_pBlocks[3];
            MVPred.BMVPred[1] = &pA->m_pBlocks[2];
        }

        if(!pC->IntraFlag)
        {
            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
            MVPred.CMVPred[2] = &pC->m_pBlocks[3];
        }

        if(!pB0->IntraFlag)
        {
            MVPred.BMVPred[0] = &pB0->m_pBlocks[3];
        }
    }
    else
    {
        if(VC1_IS_TOP_RIGHT_MB(LeftTopRight))
        {
            pC = pCurrMB - 1;

            if(!pC->IntraFlag)
            {
                MVPred.CMVPred[0] = &pC->m_pBlocks[1];
                MVPred.CMVPred[2] = &pC->m_pBlocks[3];
            }
        }
        else if (VC1_IS_LEFT_RIGHT_MB(LeftTopRight))
        {
            pA = pCurrMB - width;

            if(!pA->IntraFlag)
            {
                MVPred.AMVPred[0] = &pA->m_pBlocks[2];
                MVPred.BMVPred[0] = &pA->m_pBlocks[3];
                MVPred.BMVPred[1] = &pA->m_pBlocks[2];
                MVPred.AMVPred[1] = &pA->m_pBlocks[3];
            }
        }
    }

    MVPred.CMVPred[1] = &pCurrMB->m_pBlocks[0];
    MVPred.AMVPred[2] = &pCurrMB->m_pBlocks[0];
    MVPred.BMVPred[3] = &pCurrMB->m_pBlocks[0];

    MVPred.BMVPred[2] = &pCurrMB->m_pBlocks[1];
    MVPred.AMVPred[3] = &pCurrMB->m_pBlocks[1];

    MVPred.CMVPred[3] = &pCurrMB->m_pBlocks[2];

    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));
}

void CalculateField1MVOneReferencePPic(VC1Context* pContext,
                                       Ipp16s *pPredMVx,Ipp16s *pPredMVy)
{
    VC1MVPredictors* MVPred = &pContext->MVPred;
    Ipp32u validPredictors = 0;
    Ipp32u hybryd = 0;

    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    if(MVPred->AMVPred[0])
    {
        MV_px[0] = MVPred->AMVPred[0]->mv[0][0];
        MV_py[0] = MVPred->AMVPred[0]->mv[0][1];
        validPredictors++;
        hybryd++;
    }

    if(MVPred->BMVPred[0])
    {
        MV_px[1] = MVPred->BMVPred[0]->mv[0][0];
        MV_py[1] = MVPred->BMVPred[0]->mv[0][1];
        validPredictors++;
    }


    if(MVPred->CMVPred[0])
    {
        MV_px[2] = MVPred->CMVPred[0]->mv[0][0];
        MV_py[2] = MVPred->CMVPred[0]->mv[0][1];
        validPredictors++;
        hybryd++;
    }

    //Calaculte predictors
    if(validPredictors > 1)
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
    }


    if(hybryd == 2)
        HybridFieldMV(pContext,pPredMVx,pPredMVy,MV_px,MV_py);

}

void CalculateField1MVTwoReferencePPic(VC1Context* pContext,
                                       Ipp16s *pPredMVx,
                                       Ipp16s *pPredMVy,
                                       Ipp8u* PredFlag)
{
    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    VC1MVPredictors* MVPred = &pContext->MVPred;

    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;

    Ipp32u flag_A, flag_B, flag_C, curr_flag;
    Ipp32u hybryd = 0;

    flag_A = flag_B = flag_C = curr_flag = pContext->m_pCurrMB->fieldFlag[0];

    if(MVPred->AMVPred[0])
    {
        MV_px[0] = MVPred->AMVPred[0]->mv[0][0];
        MV_py[0] = MVPred->AMVPred[0]->mv[0][1];
        flag_A = MVPred->AMVPred[0]->fieldFlag[0];
        validPredictorsOppositeField = validPredictorsOppositeField +
           (flag_A^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
             1 - (flag_A^curr_flag);

        hybryd++;
    }

    if(MVPred->BMVPred[0])
    {
        MV_px[1] = MVPred->BMVPred[0]->mv[0][0];
        MV_py[1] = MVPred->BMVPred[0]->mv[0][1];
        flag_B = MVPred->BMVPred[0]->fieldFlag[0];
        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_B^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_B^curr_flag);
    }


    if(MVPred->CMVPred[0])
    {
        MV_px[2] = MVPred->CMVPred[0]->mv[0][0];
        MV_py[2] = MVPred->CMVPred[0]->mv[0][1];
        flag_C = MVPred->CMVPred[0]->fieldFlag[0];
        hybryd++;
        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_C^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_C^curr_flag);
    }

    //Calaculte predictors
    if(validPredictorsSameField <= validPredictorsOppositeField)
    {
        *PredFlag = 1 - *PredFlag;      //opposite
    }


    if(*PredFlag == 0)
    {
        //same
        if(flag_A != curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[0], &MV_py[0],*PredFlag,curr_flag);

        if(flag_B != curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[1], &MV_py[1],*PredFlag,curr_flag);

        if(flag_C != curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[2], &MV_py[2],*PredFlag,curr_flag);
    }
    else
    {
        pContext->m_pCurrMB->fieldFlag[0] = 1 - pContext->m_pCurrMB->fieldFlag[0];
        curr_flag = pContext->m_pCurrMB->fieldFlag[0];

        if(flag_A != curr_flag)
            ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[0], &MV_py[0]);

        if(flag_B != curr_flag)
            ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[1], &MV_py[1]);

        if(flag_C != curr_flag)
            ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[2], &MV_py[2]);
    }


    if(validPredictorsSameField + validPredictorsOppositeField > 1)
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
    }


    if(hybryd == 2)
        HybridFieldMV(pContext,pPredMVx,pPredMVy,MV_px,MV_py);

}

void CalculateField4MVOneReferencePPic(VC1Context* pContext, Ipp16s *pPredMVx,Ipp16s *pPredMVy, Ipp32s blk_num)
{
    Ipp32u validPredictors = 0;
    VC1MVPredictors* MVPred = &pContext->MVPred;
    Ipp32u hybryd = 0;

    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    if(MVPred->AMVPred[blk_num])
    {
        MV_px[0] = MVPred->AMVPred[blk_num]->mv[0][0];
        MV_py[0] = MVPred->AMVPred[blk_num]->mv[0][1];
        validPredictors++;
        hybryd++;
    }

    if(MVPred->BMVPred[blk_num])
    {
        MV_px[1] = MVPred->BMVPred[blk_num]->mv[0][0];
        MV_py[1] = MVPred->BMVPred[blk_num]->mv[0][1];
        validPredictors++;
    }

    if(MVPred->CMVPred[blk_num])
    {
        MV_px[2] = MVPred->CMVPred[blk_num]->mv[0][0];
        MV_py[2] = MVPred->CMVPred[blk_num]->mv[0][1];
        validPredictors++;
        hybryd++;
    }

    //Calaculte predictors
    if(validPredictors>=2)
    {
       *pPredMVx = (Ipp16s)median3(MV_px);
       *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
    }


    if(hybryd == 2)
        HybridFieldMV(pContext,pPredMVx,pPredMVy,MV_px,MV_py);
}

void CalculateField4MVTwoReferencePPic(VC1Context* pContext,
                                              Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                              Ipp32s blk_num,  Ipp8u* PredFlag)
{
    VC1MVPredictors* MVPred = &pContext->MVPred;
    Ipp32u hybryd = 0;

    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;


    Ipp32u flag_A, flag_B, flag_C, curr_flag;
    flag_A = flag_B = flag_C = curr_flag = pContext->m_pCurrMB->fieldFlag[0];

    if(MVPred->AMVPred[blk_num])
    {
        MV_px[0] = MVPred->AMVPred[blk_num]->mv[0][0];
        MV_py[0] = MVPred->AMVPred[blk_num]->mv[0][1];

        flag_A = MVPred->AMVPred[blk_num]->fieldFlag[0];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_A^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_A^curr_flag);
        hybryd++;
    }

    if(MVPred->BMVPred[blk_num])
    {
        MV_px[1] = MVPred->BMVPred[blk_num]->mv[0][0];
        MV_py[1] = MVPred->BMVPred[blk_num]->mv[0][1];

        flag_B = MVPred->BMVPred[blk_num]->fieldFlag[0];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_B^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_B^curr_flag);
    }

    if(MVPred->CMVPred[blk_num])
    {
        MV_px[2] = MVPred->CMVPred[blk_num]->mv[0][0];
        MV_py[2] = MVPred->CMVPred[blk_num]->mv[0][1];

        flag_C = MVPred->CMVPred[blk_num]->fieldFlag[0];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_C^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_C^curr_flag);
        hybryd++;
    }


    //Calculate
    if(validPredictorsSameField <= validPredictorsOppositeField)
    {
        //opposite
        *PredFlag = 1 - *PredFlag;
    }
    else
    {
    }

    if(*PredFlag == 0)
    {
        //same
        if(flag_A != curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[0], &MV_py[0],*PredFlag,curr_flag);

        if(flag_B!=curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[1], &MV_py[1],*PredFlag,curr_flag);

        if(flag_C != curr_flag)
            ScaleSamePredPPic(pContext->m_picLayerHeader, &MV_px[2], &MV_py[2],*PredFlag,curr_flag);
    }
    else
    {
        //opposite
        pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[0] = 1 - pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[0];
        curr_flag = pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[0];

        if(flag_A != curr_flag)
             ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[0], &MV_py[0]);

        if(flag_B != curr_flag)
             ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[1], &MV_py[1]);

        if(flag_C != curr_flag)
            ScaleOppositePredPPic(pContext->m_picLayerHeader,&MV_px[2], &MV_py[2]);
    }


    if(validPredictorsSameField + validPredictorsOppositeField >1)
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
     }



    if(hybryd == 2)
        HybridFieldMV(pContext,pPredMVx,pPredMVy,MV_px,MV_py);

}

void CalculateField1MVTwoReferenceBPic(VC1Context* pContext,
                                       Ipp16s *pPredMVx,
                                       Ipp16s *pPredMVy,
                                       Ipp32s Back,
                                       Ipp8u* PredFlag)
{
    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    VC1MVPredictors* MVPred = &pContext->MVPred;

    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;

    Ipp32u flag_A, flag_B, flag_C, curr_flag;
    Ipp32u hybryd = 0;

    flag_A = flag_B = flag_C = curr_flag = pContext->m_pCurrMB->fieldFlag[Back];

    if(MVPred->AMVPred[0])
    {
        MV_px[0] = MVPred->AMVPred[0]->mv[Back][0];
        MV_py[0] = MVPred->AMVPred[0]->mv[Back][1];
        flag_A = MVPred->AMVPred[0]->fieldFlag[Back];
        validPredictorsOppositeField = validPredictorsOppositeField +
           (flag_A^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
             1 - (flag_A^curr_flag);

        hybryd++;
    }

    if(MVPred->BMVPred[0])
    {
        MV_px[1] = MVPred->BMVPred[0]->mv[Back][0];
        MV_py[1] = MVPred->BMVPred[0]->mv[Back][1];
        flag_B = MVPred->BMVPred[0]->fieldFlag[Back];
        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_B^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_B^curr_flag);
    }


    if(MVPred->CMVPred[0])
    {
        MV_px[2] = MVPred->CMVPred[0]->mv[Back][0];
        MV_py[2] = MVPred->CMVPred[0]->mv[Back][1];
        flag_C = MVPred->CMVPred[0]->fieldFlag[Back];
        hybryd++;
        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_C^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_C^curr_flag);
    }

    //Calaculte predictors
    if(validPredictorsSameField <= validPredictorsOppositeField)
    {
        *PredFlag = 1 - *PredFlag;      //opposite
    }


    if(*PredFlag == 0)
    {
        //same
        if(flag_A != curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[0], &MV_py[0],*PredFlag,curr_flag, Back);

        if(flag_B != curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[1], &MV_py[1],*PredFlag,curr_flag, Back);

        if(flag_C != curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[2], &MV_py[2],*PredFlag,curr_flag, Back);
    }
    else
    {
        pContext->m_pCurrMB->fieldFlag[Back] = 1 - pContext->m_pCurrMB->fieldFlag[Back];
        curr_flag = pContext->m_pCurrMB->fieldFlag[Back];

        if(flag_A != curr_flag)
            ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[0], &MV_py[0],*PredFlag,curr_flag, Back);

        if(flag_B != curr_flag)
            ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[1], &MV_py[1],*PredFlag,curr_flag, Back);

        if(flag_C != curr_flag)
            ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[2], &MV_py[2],*PredFlag,curr_flag, Back);
    }

    if(validPredictorsSameField + validPredictorsOppositeField > 1)
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
    }

}

void CalculateField4MVTwoReferenceBPic(VC1Context* pContext,
                                       Ipp16s *pPredMVx,Ipp16s *pPredMVy,
                                       Ipp32s blk_num,Ipp32s Back,
                                       Ipp8u* PredFlag)
{
    VC1MVPredictors* MVPred = &pContext->MVPred;

    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    Ipp32u validPredictorsOppositeField = 0;
    Ipp32u validPredictorsSameField = 0;


    Ipp32u flag_A, flag_B, flag_C, curr_flag;
    flag_A = flag_B = flag_C = curr_flag = pContext->m_pCurrMB->fieldFlag[Back];

    if(MVPred->AMVPred[blk_num])
    {
        MV_px[0] = MVPred->AMVPred[blk_num]->mv[Back][0];
        MV_py[0] = MVPred->AMVPred[blk_num]->mv[Back][1];

        flag_A = MVPred->AMVPred[blk_num]->fieldFlag[Back];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_A^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_A^curr_flag);
    }

    if(MVPred->BMVPred[blk_num])
    {
        MV_px[1] = MVPred->BMVPred[blk_num]->mv[Back][0];
        MV_py[1] = MVPred->BMVPred[blk_num]->mv[Back][1];

        flag_B = MVPred->BMVPred[blk_num]->fieldFlag[Back];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_B^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_B^curr_flag);
    }

    if(MVPred->CMVPred[blk_num])
    {
        MV_px[2] = MVPred->CMVPred[blk_num]->mv[Back][0];
        MV_py[2] = MVPred->CMVPred[blk_num]->mv[Back][1];

        flag_C = MVPred->CMVPred[blk_num]->fieldFlag[Back];

        validPredictorsOppositeField = validPredictorsOppositeField +
            (flag_C^curr_flag);

        validPredictorsSameField = validPredictorsSameField +
            1 - (flag_C^curr_flag);
    }


    //Calculate
    if(validPredictorsSameField <= validPredictorsOppositeField)
    {
        //opposite
        *PredFlag = 1 - *PredFlag;
    }
    else
    {
    }

    if(*PredFlag == 0)
    {
        //same
        if(flag_A != curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[0], &MV_py[0],*PredFlag,curr_flag, Back);

        if(flag_B!=curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[1], &MV_py[1],*PredFlag,curr_flag, Back);

        if(flag_C != curr_flag)
            ScaleSamePredBPic(pContext->m_picLayerHeader, &MV_px[2], &MV_py[2],*PredFlag,curr_flag, Back);
    }
    else
    {
        pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[Back] = 1 - pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[Back];
        curr_flag = pContext->m_pCurrMB->m_pBlocks[blk_num].fieldFlag[Back];

        if(flag_A != curr_flag)
             ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[0], &MV_py[0],*PredFlag,curr_flag, Back);

        if(flag_B != curr_flag)
             ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[1], &MV_py[1],*PredFlag,curr_flag, Back);

        if(flag_C != curr_flag)
            ScaleOppositePredBPic(pContext->m_picLayerHeader,&MV_px[2], &MV_py[2],*PredFlag,curr_flag, Back);
    }


    if(validPredictorsSameField + validPredictorsOppositeField >1)
    {
        *pPredMVx = (Ipp16s)median3(MV_px);
        *pPredMVy = (Ipp16s)median3(MV_py);
    }
    else
    {
        *pPredMVx = MV_px[0] + MV_px[1] + MV_px[2];
        *pPredMVy = MV_py[0] + MV_py[1] + MV_py[2];
    }

}


#endif //UMC_ENABLE_VC1_VIDEO_DECODER
