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

void PackDirectMVProgressive(VC1MB* pCurrMB, Ipp16s* pSavedMV)
{
    if (VC1_GET_MBTYPE(pCurrMB->mbType) == VC1_MB_1MV_INTER)
    {
        pSavedMV[0] = pCurrMB->m_pBlocks[0].mv[0][0];
        pSavedMV[1] = pCurrMB->m_pBlocks[0].mv[0][1];
    }
    else
    {
        Ipp32u blk_num;
        Ipp16s x[4];
        Ipp16s y[4];
        for (blk_num = 0; blk_num < 4; blk_num++)
        {
            if(pCurrMB->m_pBlocks[blk_num].blkType & VC1_BLK_INTRA)
                x[blk_num] = VC1_MVINTRA;
            else
            {
                x[blk_num] = pCurrMB->m_pBlocks[blk_num].mv[0][0];
                y[blk_num] = pCurrMB->m_pBlocks[blk_num].mv[0][1];
            }
        }
        CalculateMV(x, y, pSavedMV, &pSavedMV[1]);
    }

}
void PackDirectMVIField(VC1MB* pCurrMB, Ipp16s* pSavedMV, Ipp8u isBottom, Ipp8u* bRefField)
{

    if (VC1_MB_INTRA == pCurrMB->mbType)
    {
        pSavedMV[0] = 0;
        pSavedMV[1] = 0;
        bRefField[0] = 1;
    }
    else
    {
        Ipp32s count=0;
        Ipp16s* xLuMV;
        Ipp16s* yLuMV;

        Ipp16s xLuMVT[VC1_NUM_OF_LUMA];
        Ipp16s yLuMVT[VC1_NUM_OF_LUMA];

        Ipp16s xLuMVB[VC1_NUM_OF_LUMA];
        Ipp16s yLuMVB[VC1_NUM_OF_LUMA];



        Ipp32u MVcount = 0;
        Ipp32u MVBcount = 0;
        Ipp32u MVTcount = 0;

        for (count=0;count<4;count++)
        {

            if ((pCurrMB->m_pBlocks[count].mv_s_polarity[0])&&(isBottom)||
                (!pCurrMB->m_pBlocks[count].mv_s_polarity[0])&&(!isBottom))
            {
                xLuMVB[MVBcount] = pCurrMB->m_pBlocks[count].mv[0][0];
                yLuMVB[MVBcount] = pCurrMB->m_pBlocks[count].mv[0][1];
                ++MVBcount;
            }
            else
            {
                xLuMVT[MVTcount] = pCurrMB->m_pBlocks[count].mv[0][0];
                yLuMVT[MVTcount] = pCurrMB->m_pBlocks[count].mv[0][1];
                ++MVTcount;
            }
        }
        if (MVBcount == MVTcount)
        {
            MVcount = 2;
            bRefField[0] = 1;
            if (isBottom)
            {
                xLuMV = xLuMVB;
                yLuMV = yLuMVB;
            } else
            {
                xLuMV = xLuMVT;
                yLuMV = yLuMVT;
            }
        } else if (MVBcount > MVTcount)
        {
            if (!isBottom)
                bRefField[0] = 0;
            else
                bRefField[0] = 1;

            MVcount = MVBcount;
            xLuMV = xLuMVB;
            yLuMV = yLuMVB;
        }
        else
        {
            if (isBottom)
            {
                 bRefField[0] = 0;
            }
            else
            {
                 bRefField[0] = 1;
            }
            MVcount = MVTcount;
            xLuMV = xLuMVT;
            yLuMV = yLuMVT;
        }
        Derive4MV_Field(MVcount,pSavedMV,&pSavedMV[1],xLuMV,yLuMV);
    }
}
void PackDirectMVIFrame(VC1MB* pCurrMB, Ipp16s* pSavedMV)
{
    if (VC1_MB_INTRA == pCurrMB->mbType)
    {
        pSavedMV[0] = 0;
        pSavedMV[1] = 0;
        pSavedMV[2] = 0;
        pSavedMV[3] = 0;
    }
    else
    {
        pSavedMV[0] = pCurrMB->m_pBlocks[0].mv[0][0];
        pSavedMV[1] = pCurrMB->m_pBlocks[0].mv[0][1];
        pSavedMV[2] = pCurrMB->m_pBlocks[0].mv_bottom[0][0];
        pSavedMV[3] = pCurrMB->m_pBlocks[0].mv_bottom[0][1];

    }

}


void PackDirectMVs(VC1MB*  pCurrMB,
                   Ipp16s* pSavedMV,
                   Ipp8u   isBottom,
                   Ipp8u*  bRefField,
                   Ipp32u  FCM)
{
    switch (FCM)
    {
    case VC1_Progressive:
        PackDirectMVProgressive(pCurrMB, pSavedMV);
        break;
    case VC1_FieldInterlace:
        PackDirectMVIField(pCurrMB, pSavedMV, isBottom, bRefField);
        break;
    case VC1_FrameInterlace:
        PackDirectMVIFrame(pCurrMB, pSavedMV);
        break;
    default:
        break;
    }
}

void Derive4MV_Field(Ipp32u _MVcount,
                     Ipp16s* xMV, Ipp16s* yMV,
                     Ipp16s* xLuMV, Ipp16s* yLuMV)
{
    switch(_MVcount)
    {
    case 0:
    case 1:
        {
            VM_ASSERT(0);
            return;
        }
    case 2:
        {
            *xMV = (xLuMV[0] + xLuMV[1]) / 2;
            *yMV = (yLuMV[0] + yLuMV[1]) / 2;
            return;
        }
    case 3:
        {
            *xMV = median3(xLuMV);
            *yMV = median3(yLuMV);
            return;
        }
    case 4:
        {
            *xMV = median4(xLuMV);
            *yMV = median4(yLuMV);
            return;
        }
    }
}

void GetPredictProgressiveMV(VC1Block *pA,VC1Block *pB,VC1Block *pC,
                             Ipp16s *pX, Ipp16s *pY,Ipp32s Back)
{
    Ipp16s X=0,  Y=0;
    Ipp16s MV_px[] = {0,0,0};
    Ipp16s MV_py[] = {0,0,0};

    if (pA && (pA->blkType & VC1_BLK_INTER))
    {
        MV_px[0] = pA->mv[Back][0];
        MV_py[0] = pA->mv[Back][1];
    }

    if (pB && (pB->blkType & VC1_BLK_INTER))
    {
        MV_px[1] = pB->mv[Back][0];
        MV_py[1] = pB->mv[Back][1];
    }

    if (pC && (pC->blkType & VC1_BLK_INTER))
    {
        MV_px[2] = pC->mv[Back][0];
        MV_py[2] = pC->mv[Back][1];
    }

    if (pA)
    {
        if (pB == NULL)
        {
            X = MV_px[0];
            Y = MV_py[0];
        }
        else
        {
            X = median3(MV_px);
            Y = median3(MV_py);
        }
    }
    else if (pC)
    {
        X = MV_px[2];
        Y = MV_py[2];
    }

    (*pX) = X;
    (*pY) = Y;
}


void HybridMV(VC1Context* pContext,VC1Block *pA,VC1Block *pC, Ipp16s *pPredMVx,Ipp16s *pPredMVy, Ipp32s Back)
{
    Ipp16s MV_px[] = {0,0};
    Ipp16s MV_py[] = {0,0};
    Ipp16s x,  y;
    Ipp16u sum;
    Ipp32s eHybridPred;

    if ((pA == NULL) || (pC == NULL) || (pContext->m_picLayerHeader->PTYPE == VC1_B_FRAME))
    {
        return;
    }

    x = (*pPredMVx);
    y = (*pPredMVy);

    if (pA && (pA->blkType & VC1_BLK_INTER))
    {
        MV_px[0] = pA->mv[Back][0];
        MV_py[0] = pA->mv[Back][1];
    }

    if (pC && (pC->blkType & VC1_BLK_INTER))
    {
        MV_px[1] = pC->mv[Back][0];
        MV_py[1] = pC->mv[Back][1];
    }

    sum = vc1_abs_16s(x-MV_px[0]) + vc1_abs_16s(y-MV_py[0]);
    if (sum <= 32)
    {
        sum = vc1_abs_16s(x-MV_px[1]) + vc1_abs_16s(y-MV_py[1]);
    }
    if (sum <= 32)
    {
        return;
    }

    VC1_GET_BITS(1,eHybridPred );
    if (eHybridPred)
    {
        x = MV_px[0];
        y = MV_py[0];
    }
    else
    {
        x = MV_px[1];
        y = MV_py[1];
    }

    (*pPredMVx)= x;
    (*pPredMVy)= y;
}

void CalculateMV(Ipp16s x[],Ipp16s y[], Ipp16s *X, Ipp16s* Y)
{
    Ipp16s temp_x[] = {0,0,0};
    Ipp16s temp_y[] = {0,0,0};

    Ipp16u n_intra = ((Ipp16u)(x[0])==VC1_MVINTRA) +
        (((Ipp16u)(x[1])== VC1_MVINTRA)<<1) +
        (((Ipp16u)(x[2])== VC1_MVINTRA)<<2) +
        (((Ipp16u)(x[3])== VC1_MVINTRA)<<3);

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_BFRAMES,VM_STRING("history MVs(%d,%d) (%d,%d) (%d,%d) (%d,%d)\n"),x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);

    switch(n_intra)
    {
    case 0x00: //all blocks are inter
        *X = median4(x);
        *Y = median4(y);
        break;
    case 0x01:
        *X = median3(&x[1]);
        *Y = median3(&y[1]);
        break;
    case 0x02:
        temp_x[0] = x[0];
        temp_x[1] = x[2];
        temp_x[2] = x[3];
        temp_y[0] = y[0];
        temp_y[1] = y[2];
        temp_y[2] = y[3];
        *X = median3(temp_x);
        *Y = median3(temp_y);
        break;
    case 0x04:
        temp_x[0] = x[0];
        temp_x[1] = x[1];
        temp_x[2] = x[3];
        temp_y[0] = y[0];
        temp_y[1] = y[1];
        temp_y[2] = y[3];
        *X = median3(temp_x);
        *Y = median3(temp_y);
        break;
    case 0x08:
        *X = median3(x);
        *Y = median3(y);
        break;
    case 0x03:
        *X = (x[2]+ x[3])/2;
        *Y = (y[2]+ y[3])/2;
        break;
    case 0x05:
        *X = (x[1]+ x[3])/2;
        *Y = (y[1]+ y[3])/2;
        break;
    case 0x06:
        *X = (x[0]+ x[3])/2;
        *Y = (y[0]+ y[3])/2;
        break;
    case 0x09:
        *X = (x[1]+ x[2])/2;
        *Y = (y[1]+ y[2])/2;
        break;
    case 0x0C:
        *X = (x[0]+ x[1])/2;
        *Y = (y[0]+ y[1])/2;
        break;
    case 0x0A:
        *X = (x[0]+ x[2])/2;
        *Y = (y[0]+ y[2])/2;
        break;

    default:
        (*X)=VC1_MVINTRA;
        (*Y)=VC1_MVINTRA;
        break;
    }
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_BFRAMES,VM_STRING("calculated history MVs(%d,%d)\n"),*X,*Y);

}
void CalculateMV_Interlace(Ipp16s x[],Ipp16s y[], Ipp16s x_bottom[],Ipp16s y_bottom[],Ipp16s *Xt, Ipp16s* Yt,Ipp16s *Xb, Ipp16s* Yb )
{
    Ipp8s n_intra = ((Ipp16u)(x[0])==VC1_MVINTRA);
    if (n_intra) // intra co-located MB
    {
        *Xt = 0;
        *Yt = 0;
        *Xb = 0;
        *Yb = 0;
    }
    else
    {
        *Xt = x[0];
        *Yt = y[0];
        *Xb = x_bottom[0];
        *Yb = y_bottom[0];
    }

}

void CalculateMV_InterlaceField(VC1Context* pContext, Ipp16s *X, Ipp16s* Y)
{
    Ipp8u* samePolarity = pContext->savedMVSamePolarity_Curr +
                          (pContext->m_seqLayerHeader->widthMB*pContext->m_pSingleMB->m_currMBYpos
                           + pContext->m_pSingleMB->m_currMBXpos);

    VC1MB* pMB = pContext->m_pCurrMB;
    pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = pContext->m_picLayerHeader->BottomField;

    if (*X == VC1_MVINTRA)
    {
        *X = 0;
        *Y = 0;
        pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0] = 1;
        pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[1] = 1;
        pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = pContext->m_picLayerHeader->BottomField;
    }
    else
    {
        if(samePolarity[0])
        {
            if (pContext->m_picLayerHeader->BottomField)
            {
                pMB->m_pBlocks[0].mv_s_polarity[0] = 1;
                pMB->m_pBlocks[0].mv_s_polarity[1] = 1;
                pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = 1;
            }
            else
            {
                pMB->m_pBlocks[0].mv_s_polarity[0] = 1;
                pMB->m_pBlocks[0].mv_s_polarity[1] = 1;
                pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = 0;

            }

        }
        else
        {
            if (pContext->m_picLayerHeader->BottomField)
            {
                pMB->m_pBlocks[0].mv_s_polarity[0] = 0;
                pMB->m_pBlocks[0].mv_s_polarity[1] = 0;
                pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = 0;
            }
            else
            {
                pMB->m_pBlocks[0].mv_s_polarity[0] = 0;
                pMB->m_pBlocks[0].mv_s_polarity[1] = 0;
                pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0] = 1;

            }
        }

    }
     pContext->m_pCurrMB->m_pBlocks[1].mv_s_polarity[0] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];
     pContext->m_pCurrMB->m_pBlocks[2].mv_s_polarity[0] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];
     pContext->m_pCurrMB->m_pBlocks[3].mv_s_polarity[0] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];
     pContext->m_pCurrMB->m_pBlocks[1].mv_s_polarity[1] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];
     pContext->m_pCurrMB->m_pBlocks[2].mv_s_polarity[1] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];
     pContext->m_pCurrMB->m_pBlocks[3].mv_s_polarity[1] = pContext->m_pCurrMB->m_pBlocks[0].mv_s_polarity[0];

     pContext->m_pCurrMB->m_pBlocks[1].fieldFlag[0] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];
     pContext->m_pCurrMB->m_pBlocks[2].fieldFlag[0] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];
     pContext->m_pCurrMB->m_pBlocks[3].fieldFlag[0] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];
     pContext->m_pCurrMB->m_pBlocks[1].fieldFlag[1] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];
     pContext->m_pCurrMB->m_pBlocks[2].fieldFlag[1] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];
     pContext->m_pCurrMB->m_pBlocks[3].fieldFlag[1] =  pContext->m_pCurrMB->m_pBlocks[0].fieldFlag[0];

}

void Decode_BMVTYPE(VC1Context* pContext)
{
    Ipp32s value=0;
    VC1_GET_BITS(1, value);
    if (value)
    {
        VC1_GET_BITS(1, value);
        if (value)
        {
            pContext->m_pCurrMB->mbType=VC1_MB_1MV_INTER|VC1_MB_INTERP;
        }
        else
        {
            pContext->m_pCurrMB->mbType = (pContext->m_picLayerHeader->BFRACTION)?
                VC1_MB_1MV_INTER|VC1_MB_FORWARD:VC1_MB_1MV_INTER|VC1_MB_BACKWARD;
        }
    }
    else
    {
        pContext->m_pCurrMB->mbType=(pContext->m_picLayerHeader->BFRACTION)?
            VC1_MB_1MV_INTER|VC1_MB_BACKWARD:VC1_MB_1MV_INTER|VC1_MB_FORWARD;
    }
}

VC1Status MVRangeDecode(VC1Context* pContext)
{
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    if (pContext->m_seqLayerHeader->EXTENDED_MV == 1)
    {
        //Ipp32s MVRANGE;
        //0   256 128
        //10  512 256
        //110 2048 512
        //111 4096 1024

        VC1_GET_BITS(1, picLayerHeader->MVRANGE);

        if(picLayerHeader->MVRANGE)
        {
            VC1_GET_BITS(1, picLayerHeader->MVRANGE);
            if(picLayerHeader->MVRANGE)
            {
                VC1_GET_BITS(1, picLayerHeader->MVRANGE);
                picLayerHeader->MVRANGE += 1;
            }
            picLayerHeader->MVRANGE += 1;
        }
        picLayerHeader->m_pCurrMVRangetbl = &VC1_MVRangeTbl[picLayerHeader->MVRANGE];
    }
    else
    {
        picLayerHeader->m_pCurrMVRangetbl = &VC1_MVRangeTbl[0];

        picLayerHeader->MVRANGE = 0;
    }
    return VC1_OK;
}

VC1Status DMVRangeDecode(VC1Context* pContext)
{
    if(pContext->m_seqLayerHeader->EXTENDED_DMV == 1)
    {
        VC1_GET_BITS(1, pContext->m_picLayerHeader->DMVRANGE);
        if(pContext->m_picLayerHeader->DMVRANGE==0)
        {
            //binary code 0
            pContext->m_picLayerHeader->DMVRANGE = VC1_DMVRANGE_NONE;
        }
        else
        {
            VC1_GET_BITS(1, pContext->m_picLayerHeader->DMVRANGE);
            if(pContext->m_picLayerHeader->DMVRANGE==0)
            {
               //binary code 10
               pContext->m_picLayerHeader->DMVRANGE = VC1_DMVRANGE_HORIZONTAL_RANGE;
            }
            else
            {
                VC1_GET_BITS(1, pContext->m_picLayerHeader->DMVRANGE);
                if(pContext->m_picLayerHeader->DMVRANGE==0)
                {
                    //binary code 110
                    pContext->m_picLayerHeader->DMVRANGE = VC1_DMVRANGE_VERTICAL_RANGE;
                }
                else
                {
                    //binary code 111
                    pContext->m_picLayerHeader->DMVRANGE = VC1_DMVRANGE_HORIZONTAL_VERTICAL_RANGE;
                }
            }
        }
    }

    return VC1_OK;
}


void PullBack_PPred4MV(VC1SingletonMB* sMB, Ipp16s *pMVx, Ipp16s* pMVy, Ipp32s blk_num)
{
    Ipp32s Min=-28;
    Ipp32s X = *pMVx;
    Ipp32s Y = *pMVy;
    Ipp32s currMBXpos = sMB->m_currMBXpos<<6;
    Ipp32s currMBYpos = sMB->m_currMBYpos<<6;

    Ipp32u Xblk = ((blk_num&1) << 5);
    Ipp32u Yblk = ((blk_num&2) << 4);

    Ipp32s IX = currMBXpos + X + Xblk;
    Ipp32s IY = currMBYpos + Y + Yblk;

    Ipp32s Width  =(sMB->widthMB<<6) - 4;
    Ipp32s Height =(sMB->heightMB<<6) - 4;


    if (IX < Min)
    {
        X = Min - currMBXpos - Xblk;
    }
    else if (IX > Width)
    {
        X = Width - currMBXpos - Xblk;
    }

    if (IY < Min)
    {
        Y = Min - currMBYpos - Yblk;
    }
    else if (IY > Height)
    {
        Y = Height - currMBYpos - Yblk;
    }

    (*pMVx) = (Ipp16s)X;
    (*pMVy) = (Ipp16s)Y;
}

void Progressive1MVPrediction(VC1Context* pContext)
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

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pB->m_pBlocks[2];
        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_TOP_MB(LeftTopRight))
    {
        //A and B predictors are unavailable
        pC = pCurrMB - 1;

        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_LEFT_MB(LeftTopRight))
    {
        //C predictor is unavailable
        pA = pCurrMB - width;
        pB = pCurrMB - width + 1;

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pB->m_pBlocks[2];
    }
    else if(VC1_IS_RIGHT_MB(LeftTopRight))
    {
        //all predictors are available
        pA = pCurrMB - width;
        pB = pCurrMB - width - 1;
        pC = pCurrMB - 1;

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pB->m_pBlocks[3];
        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    else if(VC1_IS_TOP_RIGHT_MB(LeftTopRight))
    {
        pC = pCurrMB - 1;

        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
    }
    memcpy(&pContext->MVPred,&MVPred,sizeof(VC1MVPredictors));
}


void Progressive4MVPrediction(VC1Context* pContext)
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

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pB0->m_pBlocks[3];
        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
        MVPred.AMVPred[1] = &pA->m_pBlocks[3];
        MVPred.BMVPred[1] = &pB1->m_pBlocks[2];
        MVPred.CMVPred[2] = &pC->m_pBlocks[3];
    }
    else if(VC1_IS_TOP_MB(LeftTopRight))
    {
        pC = pCurrMB - 1;

        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
        MVPred.CMVPred[2] = &pC->m_pBlocks[3];
    }
    else if(VC1_IS_LEFT_MB(LeftTopRight))
    {
        pA = pCurrMB - width;
        pB1 = pCurrMB - width + 1;

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pA->m_pBlocks[3];
        MVPred.AMVPred[1] = &pA->m_pBlocks[3];
        MVPred.BMVPred[1] = &pB1->m_pBlocks[2];
    }
    else if (VC1_IS_RIGHT_MB(LeftTopRight))
    {
        pA = pCurrMB - width;
        pB0 = pCurrMB - width - 1;
        pC = pCurrMB - 1;

        MVPred.AMVPred[0] = &pA->m_pBlocks[2];
        MVPred.BMVPred[0] = &pB0->m_pBlocks[3];
        MVPred.CMVPred[0] = &pC->m_pBlocks[1];
        MVPred.AMVPred[1] = &pA->m_pBlocks[3];
        MVPred.BMVPred[1] = &pA->m_pBlocks[2];
        MVPred.CMVPred[2] = &pC->m_pBlocks[3];
    }
    else
    {
        if(VC1_IS_TOP_RIGHT_MB(LeftTopRight))
        {
            pC = pCurrMB - 1;

            MVPred.CMVPred[0] = &pC->m_pBlocks[1];
            MVPred.CMVPred[2] = &pC->m_pBlocks[3];
        }
        else if (VC1_IS_LEFT_RIGHT_MB(LeftTopRight))
        {
            pA = pCurrMB - width;

            MVPred.AMVPred[0] = &pA->m_pBlocks[2];
            MVPred.BMVPred[0] = &pA->m_pBlocks[3];
            MVPred.BMVPred[1] = &pA->m_pBlocks[2];
            MVPred.AMVPred[1] = &pA->m_pBlocks[3];
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

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
