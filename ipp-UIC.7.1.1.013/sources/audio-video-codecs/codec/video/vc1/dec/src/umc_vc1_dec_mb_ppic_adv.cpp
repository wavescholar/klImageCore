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
#include "umc_vc1_common_interlace_mb_mode_tables.h"
#include "umc_vc1_common_zigzag_tbl.h"


#include "umc_vc1_dec_time_statistics.h"

typedef void (*MV_PREDICT_4MV)(VC1MB* pCurrMB,
                               Ipp16s pPredMV[4][2],
                               Ipp16s validPredictors[4],
                               Ipp16s MV_px[4][3],
                               Ipp16s MV_py[4][3],
                               ...);

typedef void (*DCPrediction)(VC1Context* pContext);
static const DCPrediction PDCPredictionTable[] =
{
        (DCPrediction)(GetPDCPredictors),
        (DCPrediction)(GetPScaleDCPredictors),
        (DCPrediction)(GetPScaleDCPredictors)
};

static VC1Status MBLayer_ProgressivePskipped(VC1Context* pContext)
{
    Ipp32s blk_num;
    Ipp16s X = 0, Y = 0;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    pCurrMB->m_cbpBits = 0;

    for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS/*VC1_NUM_OF_LUMA*/; blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].blkType = VC1_BLK_INTER8X8;
    }

    if(VC1_GET_MBTYPE(pCurrMB->mbType) == VC1_MB_4MV_INTER)
    {
        Progressive4MVPrediction(pContext);

        for (blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
        {
            CalculateProgressive4MV_Adv(pContext,&X, &Y, blk_num);
            ApplyMVPrediction(pContext, blk_num, &X, &Y, 0, 0, 0);
        }
    }
    else
    {
        //MV prediction
        Progressive1MVPrediction(pContext);
        CalculateProgressive1MV(pContext,&X,&Y);
        ApplyMVPrediction(pContext, 0, &X, &Y, 0, 0, 0);
    }


    return VC1_OK;
}

static VC1Status MBLayer_ProgressivePpicture1MV(VC1Context* pContext)
{
    IppStatus ret;
    Ipp16s dmv_x;
    Ipp16s dmv_y;
    Ipp16u last_intra_flag = 0;
    Ipp32s i;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    //MVDATA is a variable sized field present in P picture macroblocks
    //This field encodes the motion vector(s) for the macroblock.

    Ipp16s hpelfl = (Ipp16s)((picLayerHeader->MVMODE==VC1_MVMODE_HPEL_1MV) ||
        (picLayerHeader->MVMODE==VC1_MVMODE_HPELBI_1MV));

    last_intra_flag = DecodeMVDiff(pContext,hpelfl,&dmv_x,&dmv_y);

    if(!(last_intra_flag&0x10))
        pCurrMB->m_cbpBits = 0;

    dmv_x = dmv_x * (1+hpelfl);
    dmv_y = dmv_y * (1+hpelfl);

    for(i = 0; i < VC1_NUM_OF_LUMA; i++)
    {
        pCurrMB->m_pBlocks[i].blkType  = (last_intra_flag&0x1)?(Ipp8u)VC1_BLK_INTRA:(Ipp8u)VC1_BLK_INTER8X8;
    }

    // all blocks are intra
    if (last_intra_flag&0x1)
        pCurrMB->mbType   = VC1_MB_INTRA;

    //motion vector predictors are calculated only for non-intra blocks, otherwise they are equal to zero (8.3.5.3)
    if(pCurrMB->mbType != VC1_MB_INTRA)
    {
        Ipp16s X = 0, Y = 0;
        // HYBRIDPRED is decoded in function PredictProgressive1MV
        //PredictProgressive1MV (pContext,&X, &Y);
        Progressive1MVPrediction(pContext);
        CalculateProgressive1MV(pContext,&X,&Y);
        ApplyMVPrediction(pContext, 0, &X, &Y, dmv_x, dmv_y, 0);
    }

    // all blocks in macroblock have one block type. So croma block have the same block type
    pCurrMB->m_pBlocks[4].blkType = pCurrMB->m_pBlocks[0].blkType;
    pCurrMB->m_pBlocks[5].blkType = pCurrMB->m_pBlocks[0].blkType;


    if(pCurrMB->mbType ==VC1_MB_INTRA && (!(last_intra_flag&0x10)))
    {
        if(picLayerHeader->m_PQuant_mode >= VC1_ALTPQUANT_MB_LEVEL)
            GetMQUANT(pContext);

        //AC prediction if intra
        VC1_GET_BITS(1, pContext->m_pSingleMB->ACPRED);

    }
    else if(last_intra_flag&0x10)
    {
        //AC prediction if intra55
        if(pCurrMB->mbType == VC1_MB_INTRA)
            VC1_GET_BITS(1, pContext->m_pSingleMB->ACPRED);
        //CBPCY decoding
        ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
            &pContext->m_bitstream.bitOffset,
            &pCurrMB->m_cbpBits,
            picLayerHeader->m_pCurrCBPCYtbl);

        VM_ASSERT(ret == ippStsNoErr);

        //MB quant calculations
        if (picLayerHeader->m_PQuant_mode >= VC1_ALTPQUANT_MB_LEVEL)
            GetMQUANT(pContext);
    }
    else
    {
        //nothing more to do
        pCurrMB->m_cbpBits = 0;
    }

    //TTMB info is decoded in MBLayer_ProgressivePpicture
    return VC1_OK;
}

static VC1Status MBLayer_ProgressivePpicture4MV(VC1Context* pContext)
{
    Ipp32s i;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    IppStatus ret;
    Ipp32s Count_inter=0;
    Ipp32s n_block=0;
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;

    Ipp32u LeftTopRightPositionFlag = pCurrMB->LeftTopRightPositionFlag;

    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                    &pContext->m_bitstream.bitOffset,
                                    &pCurrMB->m_cbpBits,
                                    picLayerHeader->m_pCurrCBPCYtbl);
    VM_ASSERT(ret == ippStsNoErr);

    if (ret!=ippStsNoErr)
        return VC1_FAIL;

    Progressive4MVPrediction(pContext);

    for (i=0;i<4;i++)
    {
        if (pCurrMB->m_cbpBits&(1<<(5-i)))
        {
            Ipp16u last_intra_flag = 0;

            //BLKMVDATA
            // for 4MV blocks hpelfl = 0
            last_intra_flag = DecodeMVDiff(pContext,0,&dmv_x,&dmv_y);

            pCurrMB->m_pBlocks[i].blkType = (last_intra_flag&0x1)?
                                                (Ipp8u)VC1_BLK_INTRA:(Ipp8u)VC1_BLK_INTER8X8;
            if(!(last_intra_flag&0x10))
                pCurrMB->m_cbpBits = (Ipp8u)(pCurrMB->m_cbpBits & ~(1 << (5 - i)));
        }
        else
        {
            dmv_x = 0;
            dmv_y = 0;
            pCurrMB->m_pBlocks[i].blkType = (Ipp8u)(picLayerHeader->TTFRM);
        }

        if (!(pCurrMB->m_pBlocks[i].blkType & VC1_BLK_INTRA))
        {
            Ipp16s X,Y;
            // HYBRIDPRED is decoded in function PredictProgressive4MV
            CalculateProgressive4MV_Adv(pContext,&X, &Y, i);
            ApplyMVPrediction(pContext, i, &X, &Y, dmv_x, dmv_y, 0);
            // croma MVs are calculated in DeriveProgMV and it is called in function Interpolate block.
            Count_inter++;
            n_block = i;
        }
    }//end for

    //type for chroma blocks
    if (Count_inter>1)
    {
        pCurrMB->m_pBlocks[4].blkType = pCurrMB->m_pBlocks[n_block].blkType;
        pCurrMB->m_pBlocks[5].blkType = pCurrMB->m_pBlocks[n_block].blkType;
    }
    else
    {
        pCurrMB->m_pBlocks[4].blkType = VC1_BLK_INTRA;
        pCurrMB->m_pBlocks[5].blkType = VC1_BLK_INTRA;
    }
    if (Count_inter==4&&(pCurrMB->m_cbpBits==0))
        return VC1_OK;

    // MQDIFF, ABSMQ (7.1.3.4)

    if (picLayerHeader->m_PQuant_mode >= VC1_ALTPQUANT_MB_LEVEL)
        GetMQUANT(pContext);

    // if macroblock have predicted => ACPRED (7.1.3.2)
    {
        Ipp8u c[6];
        Ipp8u a[6];
        Ipp32s count=0;

        memset(a,0,6*sizeof(Ipp8s));
        memset(c,0,6*sizeof(Ipp8s));
        pContext->m_pSingleMB->ACPRED =0;

        if (VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
        {
            c[0] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[1].blkType&VC1_BLK_INTRA);
            c[2] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[3].blkType&VC1_BLK_INTRA);
            c[4] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[4].blkType&VC1_BLK_INTRA);
            c[5] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[5].blkType&VC1_BLK_INTRA);
        }
        if (VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
        {
            a[0] = (Ipp8u)((pCurrMB - sMB->widthMB)->m_pBlocks[2].blkType&VC1_BLK_INTRA);
            a[1] = (Ipp8u)((pCurrMB - sMB->widthMB)->m_pBlocks[3].blkType&VC1_BLK_INTRA);
            a[4] = (Ipp8u)((pCurrMB - sMB->widthMB)->m_pBlocks[4].blkType&VC1_BLK_INTRA);
            a[5] = (Ipp8u)((pCurrMB - sMB->widthMB)->m_pBlocks[5].blkType&VC1_BLK_INTRA);
        }
        c[1]=(Ipp8u)(pCurrMB->m_pBlocks[0].blkType& VC1_BLK_INTRA);
        c[3]=(Ipp8u)(pCurrMB->m_pBlocks[2].blkType& VC1_BLK_INTRA);
        a[2]=(Ipp8u)(pCurrMB->m_pBlocks[0].blkType& VC1_BLK_INTRA);
        a[3]=(Ipp8u)(pCurrMB->m_pBlocks[1].blkType& VC1_BLK_INTRA);

        for (i=0;i<VC1_NUM_OF_BLOCKS;i++)
        {
            count+=((pCurrMB->m_pBlocks[i].blkType& VC1_BLK_INTRA)&&((c[i])||(a[i])));
        }

        if (count)
            VC1_GET_BITS(1,sMB->ACPRED);
    }
    return VC1_OK;
}

static VC1Status MBLayer_InterlacePskipped(VC1Context* pContext)
{
    Ipp32s blk_num;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp16s X = 0, Y = 0;

//    static const  Ipp32u predict_offset[6] = {0,8,128,136,256,320};

    pCurrMB->mbType = VC1_MB_1MV_INTER | VC1_MB_FORWARD;
    pCurrMB->m_cbpBits = 0;
    pCurrMB->FIELDTX = 0;


    for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].blkType =  VC1_BLK_INTER8X8;
    }

    PredictInterlaceFrame1MV(pContext);
    CalculateInterlaceFrame1MV_P(&pContext->MVPred, &X, &Y);

    ApplyMVPrediction(pContext, 0, &X, &Y, 0, 0, 0);
    for( blk_num = 0;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][1] = Y;
    }


    return VC1_OK;
}

static VC1Status MBLayer_InterlacePpicture1MV(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp32s blk_num;
    Ipp16s X =0;
    Ipp16s Y = 0;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 1 MV\n"));

    if(VC1_MB_Mode_PBPic_MVPresent_Table[pContext->m_pSingleMB->MBMODEIndex])
        DecodeMVDiff_Adv(pContext,&dmv_x,&dmv_y);

    PredictInterlaceFrame1MV(pContext);
    CalculateInterlaceFrame1MV_P(&pContext->MVPred, &X, &Y);

    ApplyMVPrediction(pContext, 0, &X, &Y, dmv_x, dmv_y,0);
    for( blk_num = 0;  blk_num < 4;  blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][1] = Y;
    }

    return VC1_OK;

}

static VC1Status MBLayer_InterlacePpicture2MV(VC1Context* pContext)
{
    Ipp16s dmv_x =0;
    Ipp16s dmv_y =0;

    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    Ipp16s dmv_x_bottom =0;
    Ipp16s dmv_y_bottom =0;

    Ipp16s pMVx[2] = {0,0}; //0 - top, 1 - bottom
    Ipp16s pMVy[2] = {0,0};

    Ipp32s blk_num;
    Ipp32s BlkMVP;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 2 MV\n"));

    BlkMVP = ((0 != ( (1 << 3) & pCurrMB->MVBP) ) ? 1 : 0);
    if (BlkMVP)
        DecodeMVDiff_Adv(pContext,&dmv_x,&dmv_y);
    else
    {
        dmv_x =0;
        dmv_y =0;
    }

    BlkMVP = ((0 != ( (1 << 1) & pCurrMB->MVBP) ) ? 1 : 0);

    if (BlkMVP)
        DecodeMVDiff_Adv(pContext,&dmv_x_bottom,&dmv_y_bottom);
    else
    {
        dmv_x_bottom = 0;
        dmv_y_bottom = 0;
    }
    PredictInterlace2MV_Field_Adv(pCurrMB, pMVx,pMVy,0, 0, sMB->widthMB);


    ApplyMVPredictionCalculate(pContext,&pMVx[0],&pMVy[0],dmv_x,dmv_y);
    ApplyMVPredictionCalculate(pContext,&pMVx[1],&pMVy[1],dmv_x_bottom,dmv_y_bottom);

    for( blk_num = 0;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].mv[0][0] = pMVx[0];
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = pMVy[0];

        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][0] = pMVx[1];
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][1] = pMVy[1];
    }

    return VC1_OK;
}

static VC1Status MBLayer_InterlacePpicture4MVField(VC1Context* pContext)
{
    Ipp16s dmv_x[4] = {0};
    Ipp16s dmv_y[4] = {0};
    Ipp32s blk_num;

    Ipp32s BlkMVP;
    Ipp16s X = 0;
    Ipp16s Y = 0;

    VC1MB* pCurrMB = pContext->m_pCurrMB;

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 4 MV Field\n"));

    for(blk_num = 0;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        BlkMVP = ((0 != ( (1 << (3 - blk_num)) & pCurrMB->MVBP) ) ? 1 : 0);

        if (BlkMVP)
        {
           DecodeMVDiff_Adv(pContext,&dmv_x[blk_num],&dmv_y[blk_num]);
        }
  }

    PredictInterlace4MVField_Adv(pContext);

    for(blk_num = 0;  blk_num < 2;  blk_num++)
    {

        CalculateInterlace4MV_TopField_Adv(&pContext->MVPred,&X, &Y, blk_num);

        ApplyMVPredictionCalculate(pContext,&X,&Y,dmv_x[blk_num],dmv_y[blk_num]);

        pCurrMB->m_pBlocks[blk_num].mv[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = Y;
    }

    for(blk_num = 2;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        CalculateInterlace4MV_BottomField_Adv(&pContext->MVPred,&X, &Y, blk_num);

        ApplyMVPredictionCalculate(pContext,&X,&Y,dmv_x[blk_num],dmv_y[blk_num]);

        pCurrMB->m_pBlocks[blk_num].mv[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = Y;

        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv_bottom[0][1] = Y;

        pCurrMB->m_pBlocks[blk_num - 2].mv_bottom[0][0] = X;
        pCurrMB->m_pBlocks[blk_num - 2].mv_bottom[0][1] = Y;
     }
    return VC1_OK;
}

static VC1Status MBLayer_InterlacePpicture4MV(VC1Context* pContext)
{
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp32s blk_num;

    Ipp32s BlkMVP;

    Ipp16s X = 0;
    Ipp16s Y = 0;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 4 MV\n"));

    PredictInterlace4MVFrame_Adv(pContext);
    for( blk_num = 0;  blk_num < VC1_NUM_OF_LUMA; blk_num++)
    {
        BlkMVP = ((0 != ( (1 << (3 - blk_num)) & pCurrMB->MVBP) ) ? 1 : 0);
        if (BlkMVP)
        {
            DecodeMVDiff_Adv(pContext,&dmv_x,&dmv_y);

        } else
        {
            dmv_x = 0;
            dmv_y = 0;
        }

        Calculate4MVFrame_Adv(&pContext->MVPred,&X,&Y,blk_num);
        ApplyMVPredictionCalculate(pContext, &X, &Y, dmv_x, dmv_y);

        pCurrMB->m_pBlocks[blk_num].mv[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = Y;
    }
    // for correct Direct MB B frames
    pCurrMB->m_pBlocks[0].mv_bottom[0][0] = pCurrMB->m_pBlocks[2].mv[0][0];
    pCurrMB->m_pBlocks[0].mv_bottom[0][1] = pCurrMB->m_pBlocks[2].mv[0][1];
    return VC1_OK;
}

static VC1Status MBLayer_InterlaceFieldPpicture1MV(VC1Context* pContext)
{
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;

    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s blk_num;
    Ipp16s X =0;
    Ipp16s Y = 0;
    Ipp8u predictor_flag = 0;

    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 1 MV\n"));
    Field1MVPrediction(pContext);

    if (0 == picLayerHeader->NUMREF)
    {
        predictor_flag = (Ipp8u)(1 - picLayerHeader->REFFIELD);

        if(VC1_MB_Mode_PBFieldPic_MVData_Table[pContext->m_pSingleMB->MBMODEIndex])
              DecodeMVDiff_Adv(pContext,&dmv_x,&dmv_y);

        CalculateField1MVOneReferencePPic(pContext, &X, &Y);
        ApplyMVPredictionCalculateOneReference(picLayerHeader,&X,&Y,dmv_x,dmv_y,predictor_flag);
    }
    else
    {
        if(VC1_MB_Mode_PBFieldPic_MVData_Table[pContext->m_pSingleMB->MBMODEIndex])
            predictor_flag = DecodeMVDiff_TwoReferenceField_Adv(pContext,&dmv_x,&dmv_y);
        CalculateField1MVTwoReferencePPic(pContext, &X, &Y, &predictor_flag);
        ApplyMVPredictionCalculateTwoReference(picLayerHeader,&X,&Y,dmv_x,dmv_y,predictor_flag);
    }


    for( blk_num = 0;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        pCurrMB->m_pBlocks[blk_num].mv[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = Y;
        pCurrMB->m_pBlocks[blk_num].mv_s_polarity[0] = 1 - predictor_flag;
        pCurrMB->m_pBlocks[blk_num].fieldFlag[0] = pCurrMB->fieldFlag[0];
        pCurrMB->m_pBlocks[blk_num].fieldFlag[1] = pCurrMB->fieldFlag[1];
    }

    return VC1_OK;

}
static VC1Status MBLayer_InterlaceFieldPpicture4MV(VC1Context* pContext)
{
    Ipp16s dmv_x = 0;
    Ipp16s dmv_y = 0;
    Ipp32s blk_num;
    Ipp16s X =0;
    Ipp16s Y = 0;
    Ipp8u predictor_flag = 0;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s BlkMVP;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace 1 MV\n"));
    if (0 == picLayerHeader->NUMREF && 0 == picLayerHeader->REFFIELD)
    {
            predictor_flag = 1;
    }


    Field4MVPrediction(pContext);
    for( blk_num = 0;  blk_num < VC1_NUM_OF_LUMA;  blk_num++)
    {
        BlkMVP = ((0 != ( (1 << (3 - blk_num)) & pCurrMB->MVBP) ) ? 1 : 0);
        if (0 == picLayerHeader->NUMREF)
        {
            if (BlkMVP)
                DecodeMVDiff_Adv(pContext,&dmv_x,&dmv_y);
            else
            {
                dmv_x = 0;
                dmv_y = 0;
            }

            CalculateField4MVOneReferencePPic(pContext,&X,&Y,blk_num);
            ApplyMVPredictionCalculateOneReference(picLayerHeader,&X,&Y,dmv_x,dmv_y,predictor_flag);
        }
        else
        {
            predictor_flag = 0;
            if (BlkMVP)
                predictor_flag = DecodeMVDiff_TwoReferenceField_Adv(pContext,&dmv_x,&dmv_y);
            else
            {
                dmv_x = 0;
                dmv_y = 0;
            }

            CalculateField4MVTwoReferencePPic(pContext, &X,&Y,blk_num,&predictor_flag);
            ApplyMVPredictionCalculateTwoReference(picLayerHeader,&X,&Y,dmv_x,dmv_y,predictor_flag);
        }

        pCurrMB->m_pBlocks[blk_num].mv[0][0] = X;
        pCurrMB->m_pBlocks[blk_num].mv[0][1] = Y;
        pCurrMB->m_pBlocks[blk_num].mv_s_polarity[0] = 1 - predictor_flag;

    }
    return VC1_OK;
}
//Progressive-coded P picture MB
VC1Status MBLayer_ProgressivePpicture_Adv(VC1Context* pContext)
{
    Ipp32s SKIPMBBIT;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s blk_num;
    VC1Status vc1Res=VC1_OK;

    //for smoothing
    pCurrMB->Overlap = (Ipp8u)pContext->m_seqLayerHeader->OVERLAP;
    if(picLayerHeader->PQUANT < 9)
           pCurrMB->Overlap =0;

    pCurrMB->m_cbpBits = 0;
    sMB->m_ubNumFirstCodedBlk = 0;


    pCurrMB->LeftTopRightPositionFlag = CalculateLeftTopRightPositionFlag(sMB);
    Set_MQuant(pContext);

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

    //memset(pContext->m_pBlock, 0,sizeof(Ipp16s)*VC1_PIXEL_IN_BLOCK*VC1_PIXEL_IN_BLOCK*VC1_NUM_OF_BLOCKS);
    pCurrMB->FIELDTX = 0;

    for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
        pCurrMB->m_pBlocks[blk_num].mv_s_polarity[0] = 0;

    pCurrMB->mbType = VC1_MB_1MV_INTER | VC1_MB_FORWARD;
    if(picLayerHeader->MVMODE == VC1_MVMODE_MIXED_MV)
    {
        Ipp32s MVMODEBIT;
        //is a 1-bit field present in P frame macroblocks
        //if the frame level field MVTYPEMB (see section 3.2.1.21)
        //indicates that the raw mode is used. If MVMODEBIT = 0
        //then the macroblock is coded in 1MV mode and if
        //MVMODEBIT = 1 then the macroblock is coded in 4MV mode.
        if (VC1_IS_BITPLANE_RAW_MODE(&picLayerHeader->MVTYPEMB))
        {
            VC1_GET_BITS(1, MVMODEBIT);
        }
        else
        {
            MVMODEBIT = picLayerHeader->MVTYPEMB.m_databits
                [sMB->widthMB*sMB->m_currMBYpos + sMB->m_currMBXpos];
        }

        if(MVMODEBIT == 1)
            pCurrMB->mbType = VC1_MB_4MV_INTER | VC1_MB_FORWARD;
    }

    if(VC1_IS_BITPLANE_RAW_MODE((&picLayerHeader->SKIPMB)))
    {
        //If SKIPMBBIT = 1 then the macroblock is skipped.
        VC1_GET_BITS(1, SKIPMBBIT);
    }
    else
    {
        SKIPMBBIT = picLayerHeader->SKIPMB.m_databits
            [sMB->widthMB*sMB->m_currMBYpos + sMB->m_currMBXpos];
    }

    pCurrMB->SkipAndDirectFlag = (SKIPMBBIT<<1);

    if(SKIPMBBIT == 1)
    {
        STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

        MBLayer_ProgressivePskipped(pContext);

        STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
            m_timeStatistics->motion_vector_decoding_EndTime,
            m_timeStatistics->motion_vector_decoding_TotalTime);

        CalculateIntraFlag(pContext);
    }
    else
    {
        memset(pContext->m_pBlock, 0,sizeof(Ipp16s)*VC1_PIXEL_IN_BLOCK*VC1_PIXEL_IN_BLOCK*VC1_NUM_OF_BLOCKS);
        if((VC1_GET_MBTYPE(pCurrMB->mbType))==VC1_MB_1MV_INTER)//1 MV mode
        {
            STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

            MBLayer_ProgressivePpicture1MV(pContext);

            STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                m_timeStatistics->motion_vector_decoding_EndTime,
                m_timeStatistics->motion_vector_decoding_TotalTime);
        }
        else //(4 MV Mode)
        {
            STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

            MBLayer_ProgressivePpicture4MV(pContext);

            STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                m_timeStatistics->motion_vector_decoding_EndTime,
                m_timeStatistics->motion_vector_decoding_TotalTime);
        }

        //end 4mv mode
        if(pCurrMB->m_cbpBits &&
            pContext->m_seqLayerHeader->VSTRANSFORM == 1)
        {
            if(picLayerHeader->TTMBF == 0 && !(pCurrMB->mbType & VC1_BLK_INTRA) )
            {
                //The TTMB field is a variable length field present in P and B
                //picture macroblocks if the picture layer field TTMBF = 1.
                //As shown in tables Table 20, Table 21 and Table 22, the TTMB
                //field specifies the transform type, the signal level and
                //the subblock pattern. If the signal type specifies
                //macroblock mode the transform type decoded from the TTMB
                //field is used to decode all coded blocks in the macroblock.
                //If the signal type signals block mode then the transform
                //type decoded from the TTMB field is used to decode the first
                //coded block in the macroblock. The transform type of the
                //remaining blocks is coded at the block level. If the
                //transform type is 8x4 or 4x8 then the subblock pattern
                //indicates the subblock pattern of the first block.
                //The table used to decode the TTMB field depends on the
                //value of PQUANT. For PQUANT less than or equal to 4,
                //Table 20 is used. For PQUANT greater than 4 and less than
                //or equal to 12, Table 21 is used. For PQUANT greater than
                //12, Table 22 is used.
                //The subblock pattern indicates which of 8x4 or 4x8 subblocks
                //have at least one non-zero coefficient.
                GetTTMB(pContext);
            }
            else
            {
                if(pCurrMB->mbType != VC1_MB_INTRA)
                {
                    for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
                        if(!(pCurrMB->m_pBlocks[blk_num].blkType & VC1_BLK_INTRA))
                            pCurrMB->m_pBlocks[blk_num].blkType = (Ipp8u)(picLayerHeader->TTFRM);
                }
            }
        }

        CalculateIntraFlag(pContext);
        if(pCurrMB->IntraFlag)
            PDCPredictionTable[pContext->m_seqLayerHeader->DQUANT](pContext);

        sMB->ZigzagTable = AdvZigZagTables_PBProgressive_luma[sMB->ACPRED];

        for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
        {
            //all MB intra in I picture
            if(pCurrMB->m_pBlocks[blk_num].blkType & VC1_BLK_INTER)
            {
                vc1Res = BLKLayer_Inter_Luma_Adv(pContext, blk_num);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
            else
            {
                vc1Res = BLKLayer_Intra_Luma_Adv(pContext, blk_num, sMB->ACPRED);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
        }

        sMB->ZigzagTable = AdvZigZagTables_PBProgressive_chroma[sMB->ACPRED];

        if(pCurrMB->m_pBlocks[blk_num].blkType & VC1_BLK_INTER)
            for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
            {
                //all MB inter
                vc1Res = BLKLayer_Inter_Chroma_Adv(pContext, blk_num);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
        else
            for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
            {
                //all MB intra
                vc1Res = BLKLayer_Intra_Chroma_Adv(pContext, blk_num,sMB->ACPRED);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }

    }
    //if (pContext->m_seqLayerHeader->LOOPFILTER)
        AssignCodedBlockPattern(pCurrMB,sMB);

    return vc1Res;
}

VC1Status MBLayer_Frame_InterlacedPpicture(VC1Context* pContext)
{
    VC1Status vc1Res=VC1_OK;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s SKIPMBBIT;
    Ipp32s tempValue;
    Ipp32s blk_num;
    Ipp32u ACPRED = 0;
    //for smoothing
    pCurrMB->Overlap = (Ipp8u)pContext->m_seqLayerHeader->OVERLAP;
    if(picLayerHeader->PQUANT < 9)
           pCurrMB->Overlap =0;


    //QUANT
    Set_MQuant(pContext);

    pCurrMB->LeftTopRightPositionFlag = CalculateLeftTopRightPositionFlag(sMB);

    //memset(pContext->m_pBlock, 0, sizeof(Ipp16s)*8*8*6);

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

    pCurrMB->mbType = VC1_MB_1MV_INTER | VC1_MB_FORWARD;

    //check SKIPMB mode
    {
        if(VC1_IS_BITPLANE_RAW_MODE(&picLayerHeader->SKIPMB))
        {
            VC1_GET_BITS(1, SKIPMBBIT);
        }
        else
        {
            SKIPMBBIT = picLayerHeader->SKIPMB.m_databits
                [sMB->widthMB * sMB->m_currMBYpos +  sMB->m_currMBXpos];
        }
    }
    pCurrMB->SkipAndDirectFlag = (SKIPMBBIT<<1);

    if(SKIPMBBIT == 1)
    {
        //standard 10.7.3.3.
        MBLayer_InterlacePskipped(pContext);
        CalculateIntraFlag(pContext);
    }
    else
    {
        memset(pContext->m_pBlock, 0, sizeof(Ipp16s)*8*8*6);
        Ipp32s ret;
        ret = ippiDecodeHuffmanOne_1u32s (
            &pContext->m_bitstream.pBitstream,
            &pContext->m_bitstream.bitOffset,
            &sMB->MBMODEIndex,
            picLayerHeader->m_pMBMode
            );
        VM_ASSERT(ret == ippStsNoErr);

        pCurrMB->mbType = VC1_MB_Mode_PBPic_MBtype_Table[sMB->MBMODEIndex];

        //INTRA
        if(pCurrMB->mbType == VC1_MB_INTRA)
        {
            //check fieldtx coding mode
            {
                Ipp32s FIELDTX;
                if(VC1_IS_BITPLANE_RAW_MODE(&picLayerHeader->FIELDTX))
                {
                    VC1_GET_BITS(1, FIELDTX);
                }
                else
                {
                    FIELDTX = picLayerHeader->FIELDTX.m_databits
                        [sMB->widthMB * sMB->m_currMBYpos + sMB->m_currMBXpos];
                }
                pCurrMB->FIELDTX = FIELDTX;
            }

            VC1_GET_BITS(1, tempValue);  //CBPRESENT
            for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
            {
                pCurrMB->m_pBlocks[blk_num].blkType = VC1_BLK_INTRA;
            }


            if(tempValue == 1)       //CBPRESENT
            {
                //CBPCY decoding
                Ipp32s ret;
                ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                    &pContext->m_bitstream.bitOffset,
                    &pCurrMB->m_cbpBits,
                    picLayerHeader->m_pCurrCBPCYtbl);

                VM_ASSERT(ret == ippStsNoErr);
            }
            else
            {
                pCurrMB->m_cbpBits = 0;
            }

            VC1_GET_BITS(1, ACPRED);

            if (picLayerHeader->m_DQuantFRM)
                Set_Alt_MQUANT(pContext);


            CalculateIntraFlag(pContext);

            PDCPredictionTable[pContext->m_seqLayerHeader->DQUANT](pContext);
            sMB->ZigzagTable = AdvZigZagTables_PBInterlace_luma[ACPRED];

            for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
            {
                vc1Res = BLKLayer_Intra_Luma_Adv(pContext, blk_num,ACPRED);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
            sMB->ZigzagTable = AdvZigZagTables_PBInterlace_chroma[ACPRED];
            for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
            {
                //all MB intra
                vc1Res = BLKLayer_Intra_Chroma_Adv(pContext, blk_num,ACPRED);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
        }
        else
        {
            //inter block
            pCurrMB->mbType |= VC1_MB_FORWARD;

            pCurrMB->FIELDTX = VC1_MB_Mode_PBPic_FIELDTX_Table[sMB->MBMODEIndex];

            if(VC1_MB_Mode_PBPic_Transform_Table[sMB->MBMODEIndex] != VC1_NO_CBP_TRANSFORM)
            {
                //CBPCY decoding
                Ipp32s ret;
                ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                    &pContext->m_bitstream.bitOffset,
                    &pCurrMB->m_cbpBits,
                    picLayerHeader->m_pCurrCBPCYtbl);

                VM_ASSERT(ret == ippStsNoErr);
            }
            else
                pCurrMB->m_cbpBits = 0;


            switch (VC1_GET_MBTYPE(pCurrMB->mbType))
            {
            case VC1_MB_2MV_INTER:
                {
                    Ipp32s ret;
                    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                        &pContext->m_bitstream.bitOffset,
                        &pCurrMB->MVBP,
                        picLayerHeader->m_pMV2BP);

                    VM_ASSERT(ret == ippStsNoErr);

                    pCurrMB->MVBP = (((pCurrMB->MVBP & 2) << 2) | ((pCurrMB->MVBP & 1) << 1));


                    STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

                    MBLayer_InterlacePpicture2MV(pContext);

                    STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                        m_timeStatistics->motion_vector_decoding_EndTime,
                        m_timeStatistics->motion_vector_decoding_TotalTime);
                }
                break;
            case VC1_MB_4MV_INTER:
                {
                    Ipp32s ret;
                    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                        &pContext->m_bitstream.bitOffset,
                        &pCurrMB->MVBP,
                        picLayerHeader->m_pMV4BP);

                    VM_ASSERT(ret == ippStsNoErr);

                    STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

                    MBLayer_InterlacePpicture4MV(pContext);

                    STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                        m_timeStatistics->motion_vector_decoding_EndTime,
                        m_timeStatistics->motion_vector_decoding_TotalTime);
                }
                break;
            case VC1_MB_4MV_FIELD_INTER:
                {
                    Ipp32s ret;
                    ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                        &pContext->m_bitstream.bitOffset,
                        &pCurrMB->MVBP,
                        picLayerHeader->m_pMV4BP);

                    VM_ASSERT(ret == ippStsNoErr);

                    STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

                    MBLayer_InterlacePpicture4MVField(pContext);

                    STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                        m_timeStatistics->motion_vector_decoding_EndTime,
                        m_timeStatistics->motion_vector_decoding_TotalTime);
                }
                break;
            case VC1_MB_1MV_INTER:
                {
                    STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

                    MBLayer_InterlacePpicture1MV(pContext);

                    STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                        m_timeStatistics->motion_vector_decoding_EndTime,
                        m_timeStatistics->motion_vector_decoding_TotalTime);
                }
                break;
            default:
                VM_ASSERT(0);
                break;
            }

            for (blk_num=0; blk_num<VC1_NUM_OF_BLOCKS; blk_num++)
                pCurrMB->m_pBlocks[blk_num].blkType = (Ipp8u)picLayerHeader->TTFRM;


            if(picLayerHeader->m_DQuantFRM == 1 && pCurrMB->m_cbpBits)
                Set_Alt_MQUANT(pContext);

            DecodeTransformInfo(pContext);
            CalculateIntraFlag(pContext);

            sMB->ZigzagTable = AdvZigZagTables_PBInterlace_luma[ACPRED];

            for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
            {
                vc1Res = BLKLayer_Inter_Luma_Adv(pContext, blk_num);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }

            sMB->ZigzagTable = AdvZigZagTables_PBInterlace_chroma[ACPRED];
            for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
            {
                //all MB inter
                vc1Res = BLKLayer_Inter_Chroma_Adv(pContext,blk_num);
                if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
            }
       }

    }//SKIPBIT
    //if (pContext->m_seqLayerHeader->LOOPFILTER)
        AssignCodedBlockPattern(pCurrMB,sMB);

    return vc1Res;
}

VC1Status MBLayer_Field_InterlacedPpicture(VC1Context* pContext)
{
    Ipp32s blk_num;
    VC1Status vc1Res=VC1_OK;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32u currFieldMBYpos;
    Ipp32u currFieldMBXpos;
    Ipp32u ACPRED = 0;

    //for smoothing
    pCurrMB->Overlap = (Ipp8u)pContext->m_seqLayerHeader->OVERLAP;
    if(picLayerHeader->PQUANT < 9)
           pCurrMB->Overlap = 0;


    pCurrMB->LeftTopRightPositionFlag = CalculateLeftTopRightPositionFlag(sMB);
    Set_MQuant_Field(pContext);

    pCurrMB->FIELDTX = 0;
    pCurrMB->SkipAndDirectFlag = 0;

    {
        Ipp32s ret;
        ret = ippiDecodeHuffmanOne_1u32s (
            &pContext->m_bitstream.pBitstream,
            &pContext->m_bitstream.bitOffset,
            &sMB->MBMODEIndex,
            picLayerHeader->m_pMBMode);
        VM_ASSERT(ret == ippStsNoErr);
    }

    pCurrMB->mbType = VC1_MB_Mode_PBFieldPic_MBtype_Table[sMB->MBMODEIndex];
    pCurrMB->fieldFlag[0] = picLayerHeader->BottomField;
    pCurrMB->fieldFlag[1] = picLayerHeader->BottomField;
    memset(pContext->m_pBlock, 0, sizeof(Ipp16s)*8*8*VC1_NUM_OF_BLOCKS);

    currFieldMBYpos = sMB->m_currMBYpos;
    currFieldMBXpos = sMB->m_currMBXpos;

    if (picLayerHeader->CurrField)
        currFieldMBYpos -= sMB->heightMB/2;

    //Y
    pCurrMB->currYPitch = sMB->currYPitch;
    pCurrMB->currYPlane = sMB->currYPlane + pCurrMB->currYPitch * (currFieldMBYpos << 5)//*VC1_PIXEL_IN_LUMA
                                          + (currFieldMBXpos << 4); //*VC1_PIXEL_IN_LUMA;

    //U
    pCurrMB->currUPitch = sMB->currUPitch;
    pCurrMB->currUPlane = sMB->currUPlane   + pCurrMB->currUPitch*(currFieldMBYpos << 4) // * VC1_PIXEL_IN_CHROMA
                                            + (currFieldMBXpos << 3); //* VC1_PIXEL_IN_CHROMA;

    //V
    pCurrMB->currVPitch = sMB->currVPitch;
    pCurrMB->currVPlane = sMB->currVPlane + pCurrMB->currVPitch*(currFieldMBYpos <<4) // * VC1_PIXEL_IN_CHROMA
                                          + (currFieldMBXpos << 3); //* VC1_PIXEL_IN_CHROMA;

    pCurrMB->currYPlane = pCurrMB->currYPlane + pCurrMB->currYPitch * picLayerHeader->BottomField;
    pCurrMB->currUPlane = pCurrMB->currUPlane + pCurrMB->currUPitch * picLayerHeader->BottomField;
    pCurrMB->currVPlane = pCurrMB->currVPlane + pCurrMB->currVPitch * picLayerHeader->BottomField;

    pCurrMB->currYPitch <<= 1;
    pCurrMB->currUPitch <<= 1;
    pCurrMB->currVPitch <<= 1;

    if(pCurrMB->mbType == VC1_MB_INTRA)
    {
        //INTRA
        //  VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV,VM_STRING("Interlace Intra P\n"));

        for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
        {
            pCurrMB->m_pBlocks[blk_num].blkType = VC1_BLK_INTRA;
            pCurrMB->m_pBlocks[blk_num].fieldFlag[0] = picLayerHeader->BottomField;
            pCurrMB->m_pBlocks[blk_num].fieldFlag[1] = picLayerHeader->BottomField;
        }

        CalculateIntraFlag(pContext);

        if (picLayerHeader->m_DQuantFRM)
            Set_Alt_MQUANT(pContext);

        VC1_GET_BITS(1, ACPRED);

        if(VC1_MB_Mode_PBFieldPic_CBPPresent_Table[sMB->MBMODEIndex] != 0)
        {
            //CBPCY decoding
            Ipp32s ret;
            ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                            &pContext->m_bitstream.bitOffset,
                                            &pCurrMB->m_cbpBits,
                                            picLayerHeader->m_pCurrCBPCYtbl);

            VM_ASSERT(ret == ippStsNoErr);
        }
        else
            pCurrMB->m_cbpBits = 0;

        PDCPredictionTable[pContext->m_seqLayerHeader->DQUANT](pContext);
        sMB->ZigzagTable = AdvZigZagTables_PBField_luma[ACPRED];

        for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
        {
            vc1Res = BLKLayer_Intra_Luma_Adv(pContext, blk_num,ACPRED);

            if(vc1Res != VC1_OK)
            {
                VM_ASSERT(0);
                break;
            }
        }
        sMB->ZigzagTable = AdvZigZagTables_PBField_chroma[ACPRED];
        for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
        {
            //all MB intra
            vc1Res = BLKLayer_Intra_Chroma_Adv(pContext, blk_num,ACPRED);
            if(vc1Res != VC1_OK)
            {
                VM_ASSERT(0);
                break;
            }
        }
    }
    else
    {
        //inter

        pCurrMB->mbType |= VC1_MB_FORWARD;

        for (blk_num=0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
        {
            pCurrMB->m_pBlocks[blk_num].blkType = (Ipp8u)picLayerHeader->TTFRM;
            pCurrMB->m_pBlocks[blk_num].fieldFlag[0] = picLayerHeader->BottomField;
            pCurrMB->m_pBlocks[blk_num].fieldFlag[1] = picLayerHeader->BottomField;
        }

       CalculateIntraFlag(pContext);


        if(VC1_GET_MBTYPE(pCurrMB->mbType) == VC1_MB_1MV_INTER)
        {
            STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

            MBLayer_InterlaceFieldPpicture1MV(pContext);

            STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                m_timeStatistics->motion_vector_decoding_EndTime,
                m_timeStatistics->motion_vector_decoding_TotalTime);
        }
        else
        {
            //4MV
            Ipp32s ret;
            ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                            &pContext->m_bitstream.bitOffset,
                                            &pCurrMB->MVBP,
                                            picLayerHeader->m_pMV4BP);
            VM_ASSERT(ret == ippStsNoErr);

            STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

            MBLayer_InterlaceFieldPpicture4MV(pContext);

            STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                m_timeStatistics->motion_vector_decoding_EndTime,
                m_timeStatistics->motion_vector_decoding_TotalTime);
        }

        if(VC1_MB_Mode_PBFieldPic_CBPPresent_Table[sMB->MBMODEIndex] != 0)
        {

            //CBPCY decoding
            Ipp32s ret;
            ret = ippiDecodeHuffmanOne_1u32s(&pContext->m_bitstream.pBitstream,
                                            &pContext->m_bitstream.bitOffset,
                                            &pCurrMB->m_cbpBits,
                                            picLayerHeader->m_pCurrCBPCYtbl);

            VM_ASSERT(ret == ippStsNoErr);
        }
        else
            pCurrMB->m_cbpBits = 0;

        if(picLayerHeader->m_DQuantFRM == 1 && pCurrMB->m_cbpBits)
            Set_Alt_MQUANT(pContext);

        DecodeTransformInfo(pContext);
        sMB->ZigzagTable = AdvZigZagTables_PBField_luma[ACPRED];

        for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
        {
            vc1Res = BLKLayer_Inter_Luma_Adv(pContext, blk_num);
            if(vc1Res != VC1_OK)
            {
                VM_ASSERT(0);
                break;
            }
        }

        sMB->ZigzagTable = AdvZigZagTables_PBField_chroma[ACPRED];

        for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
        {
            //all MB inter
            vc1Res = BLKLayer_Inter_Chroma_Adv(pContext, blk_num);
            if(vc1Res != VC1_OK)
            {
                VM_ASSERT(0);
                break;
            }
        }
    }

    //if (pContext->m_seqLayerHeader->LOOPFILTER)
        AssignCodedBlockPattern(pCurrMB,sMB);

    return vc1Res;
}

#endif //UMC_ENABLE_VC_VIDEO1_DECODER
