/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, MB Layer in P picture for simple\main profiles
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_zigzag_tbl.h"

#include "umc_vc1_dec_time_statistics.h"

typedef void (*DCPrediction)(VC1Context* pContext);

static const DCPrediction PDCPredictionTable[] =
{
        (DCPrediction)(GetPDCPredictors),
        (DCPrediction)(GetPScaleDCPredictors),
        (DCPrediction)(GetPScaleDCPredictors)
};



static VC1Status MBLayer_ProgressivePskipped(VC1Context* pContext)
{
    Ipp32s blk_num;;
    Ipp16s X = 0, Y = 0;
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    pContext->m_pCurrMB->m_cbpBits = 0;

    for(blk_num = 0; blk_num < 6; blk_num++)
        pCurrMB->m_pBlocks[blk_num].blkType = VC1_BLK_INTER8X8;

    if((VC1_GET_MBTYPE(pCurrMB->mbType)) == VC1_MB_4MV_INTER)
    {
        Progressive4MVPrediction(pContext);
        for (blk_num=0;blk_num<4;blk_num++)
        {
            CalculateProgressive4MV(pContext,&X, &Y, blk_num);
            ApplyMVPrediction(pContext, blk_num, &X, &Y, 0, 0, 0);
        }
    }
    else
    {
        Progressive1MVPrediction(pContext);
        //MV prediction
        CalculateProgressive1MV(pContext,&X, &Y);
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
    Ipp8u blk_type;
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s i;
    //MVDATA is a variable sized field present in P picture macroblocks
    //This field encodes the motion vector(s) for the macroblock.

    Ipp16s hpelfl = (Ipp16s)((picLayerHeader->MVMODE == VC1_MVMODE_HPEL_1MV) ||
                             (picLayerHeader->MVMODE == VC1_MVMODE_HPELBI_1MV));

    last_intra_flag = DecodeMVDiff(pContext,hpelfl,&dmv_x,&dmv_y);

    if(!(last_intra_flag&0x10))
        pCurrMB->m_cbpBits = 0;

    dmv_x  = dmv_x * (1+hpelfl);
    dmv_y  = dmv_y * (1+hpelfl);

    //set BLK_TYPE
    blk_type = (last_intra_flag&0x1) ?(Ipp8u)VC1_BLK_INTRA:(Ipp8u)VC1_BLK_INTER8X8;
    for(i = 0; i < 4; i++)
       pCurrMB->m_pBlocks[i].blkType  = blk_type;

    // all blocks are intra
    if (last_intra_flag&0x1)
    {
        pCurrMB->mbType   = VC1_MB_INTRA;

        if(!(last_intra_flag&0x10))
        {
            if(picLayerHeader->m_PQuant_mode>=VC1_ALTPQUANT_MB_LEVEL)
                GetMQUANT(pContext);

            //AC prediction if intra
            VC1_GET_BITS(1, pContext->m_pSingleMB->ACPRED);
        }
        else
        {
            //AC prediction if intra55
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
     }
    else
    {
         //motion vector predictors are calculated only for non-intra blocks, otherwise they are equal to zero (8.3.5.3)
        Ipp16s X = 0, Y = 0;
        Progressive1MVPrediction(pContext);
        // HYBRIDPRED is decoded in function PredictProgressive1MV
        CalculateProgressive1MV(pContext,&X, &Y);
        ApplyMVPrediction(pContext, 0, &X, &Y, dmv_x, dmv_y, 0);

        if(last_intra_flag&0x10)
        {
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
    }

    // all blocks in macroblock have one block type. So croma block have the same block type
    pCurrMB->m_pBlocks[4].blkType = pCurrMB->m_pBlocks[0].blkType;
    pCurrMB->m_pBlocks[5].blkType = pCurrMB->m_pBlocks[0].blkType;

    return VC1_OK;
}

static VC1Status MBLayer_ProgressivePpicture4MV(VC1Context* pContext)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32s i;
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
            //not_last = (Ipp8u)(last_intra_flag>>4);
            //intra_flag = (Ipp8u)(last_intra_flag & 0x1);

            pCurrMB->m_pBlocks[i].blkType = (last_intra_flag&0x1) ?
                                    (Ipp8u)VC1_BLK_INTRA:(Ipp8u)VC1_BLK_INTER8X8;

            if(!(last_intra_flag&0x10))
                pCurrMB->m_cbpBits = (Ipp8u)(pCurrMB->m_cbpBits & ~(1 << (5 - i)));
        }
        else
        {
            dmv_x = 0;
            dmv_y = 0;
            pCurrMB->m_pBlocks[i].blkType = (Ipp8u)picLayerHeader->TTFRM;
        }

        if (!(pCurrMB->m_pBlocks[i].blkType & VC1_BLK_INTRA))
        {
            Ipp16s X,Y;
            // HYBRIDPRED is decoded in function PredictProgressive4MV
            CalculateProgressive4MV(pContext,&X, &Y, i);
            ApplyMVPrediction(pContext, i, &X, &Y, dmv_x, dmv_y,0);
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

    if (Count_inter == 4 && (pCurrMB->m_cbpBits == 0))
        return VC1_OK;

    // MQDIFF, ABSMQ (7.1.3.4)

    if (picLayerHeader->m_PQuant_mode >= VC1_ALTPQUANT_MB_LEVEL)
        GetMQUANT(pContext);

    // if macroblock have predicted => ACPRED (7.1.3.2)
    {
        Ipp8u c[6] = {0};
        Ipp8u a[6] = {0};
        Ipp32s count = 0;
        Ipp32u width = pContext->m_seqLayerHeader->widthMB;

        pContext->m_pSingleMB->ACPRED =0;

        if (VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
        {
            c[0] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[1].blkType & VC1_BLK_INTRA);
            c[2] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[3].blkType & VC1_BLK_INTRA);
            c[4] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[4].blkType & VC1_BLK_INTRA);
            c[5] = (Ipp8u)((pCurrMB - 1)->m_pBlocks[5].blkType & VC1_BLK_INTRA);
        }
        if (VC1_IS_NO_TOP_MB(LeftTopRightPositionFlag))
        {
            a[0] = (Ipp8u)((pCurrMB - width)->m_pBlocks[2].blkType & VC1_BLK_INTRA);
            a[1] = (Ipp8u)((pCurrMB - width)->m_pBlocks[3].blkType & VC1_BLK_INTRA);
            a[4] = (Ipp8u)((pCurrMB - width)->m_pBlocks[4].blkType & VC1_BLK_INTRA);
            a[5] = (Ipp8u)((pCurrMB - width)->m_pBlocks[5].blkType & VC1_BLK_INTRA);
        }
        c[1] = (Ipp8u)(pCurrMB->m_pBlocks[0].blkType & VC1_BLK_INTRA);
        c[3] = (Ipp8u)(pCurrMB->m_pBlocks[2].blkType & VC1_BLK_INTRA);
        a[2] = (Ipp8u)(pCurrMB->m_pBlocks[0].blkType & VC1_BLK_INTRA);
        a[3] = (Ipp8u)(pCurrMB->m_pBlocks[1].blkType & VC1_BLK_INTRA);

        for (i=0;i<VC1_NUM_OF_BLOCKS;i++)
        {
            count+=((pCurrMB->m_pBlocks[i].blkType & VC1_BLK_INTRA)&&((c[i])||(a[i])));
        }

        if (count)
            VC1_GET_BITS(1,pContext->m_pSingleMB->ACPRED);
    }

    return VC1_OK;
}

//Progressive-coded P picture MB
VC1Status MBLayer_ProgressivePpicture(VC1Context* pContext)
{
    Ipp32s SKIPMBBIT;
    Ipp32u blk_num;
    VC1Status vc1Res=VC1_OK;

    VC1MB* pCurrMB = pContext->m_pCurrMB;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    if (picLayerHeader->PQUANT>=9)
        pCurrMB->Overlap = (Ipp8u)pContext->m_seqLayerHeader->OVERLAP;
    else
        pCurrMB->Overlap =0;

    Set_MQuant(pContext);

    pCurrMB->LeftTopRightPositionFlag = CalculateLeftTopRightPositionFlag(sMB);

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
            MVMODEBIT = picLayerHeader->MVTYPEMB.m_databits[sMB->widthMB * sMB->m_currMBYpos +
                                                                            sMB->m_currMBXpos];

        if(MVMODEBIT == 1)
            pCurrMB->mbType = VC1_MB_4MV_INTER | VC1_MB_FORWARD;
    }

    if(VC1_IS_BITPLANE_RAW_MODE((&picLayerHeader->SKIPMB)))
    {
        //If SKIPMBBIT = 1 then the macroblock is skipped.
        VC1_GET_BITS(1, SKIPMBBIT);
    }
    else
        SKIPMBBIT = picLayerHeader->SKIPMB.m_databits[sMB->widthMB * sMB->m_currMBYpos +
                                                                        sMB->m_currMBXpos];

    pCurrMB->SkipAndDirectFlag = (SKIPMBBIT<<1);

    if(SKIPMBBIT == 1)
    {

        STATISTICS_START_TIME(m_timeStatistics->motion_vector_decoding_StartTime);

        MBLayer_ProgressivePskipped(pContext);

        STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
            m_timeStatistics->motion_vector_decoding_EndTime,
            m_timeStatistics->motion_vector_decoding_TotalTime);

        CalculateIntraFlag(pContext);

        //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_QUANT, VM_STRING("MB Quant = %d\n"), pContext->m_pCurrMB->MQUANT);
        // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_QUANT, VM_STRING("HalfQ = %d\n"), pContext->m_pCurrMB->HALFQP);
    }
    else
    {
        memset(pContext->m_pBlock, 0, sizeof(Ipp16s)*8*8*VC1_NUM_OF_BLOCKS);

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
            //3.2.2.3
            //CBPCY is a variable-length field present in both I picture and P
            //picture macroblock layers. Section 4.1.2.1 describes the CBPCY field
            //in I picture macroblocks and section 4.4.5.2 describes the CBPCY field
            //in P picture macroblocks.
            //CBPCY decoding
            MBLayer_ProgressivePpicture4MV(pContext);

            STATISTICS_END_TIME(m_timeStatistics->motion_vector_decoding_StartTime,
                m_timeStatistics->motion_vector_decoding_EndTime,
                m_timeStatistics->motion_vector_decoding_TotalTime);
        }
        //end 4mv mode

        if(pCurrMB->m_cbpBits && pContext->m_seqLayerHeader->VSTRANSFORM == 1)
            if(picLayerHeader->TTMBF == 0 &&  !(pCurrMB->mbType & VC1_BLK_INTRA) )
            {
                GetTTMB(pContext);
            }
            else
            {
                if(pCurrMB->mbType != VC1_MB_INTRA)
                {
                    for(blk_num = 0; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
                        if(!(pCurrMB->m_pBlocks[blk_num].blkType & VC1_BLK_INTRA))
                            pCurrMB->m_pBlocks[blk_num].blkType = (Ipp8u)picLayerHeader->TTFRM;
                }
            }

            CalculateIntraFlag(pContext);

            // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_QUANT, VM_STRING("MB Quant = %d\n"), pContext->m_pCurrMB->MQUANT);
            // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_QUANT, VM_STRING("HalfQ = %d\n"), pContext->m_pCurrMB->HALFQP);

            Ipp32u IntraFlag = pCurrMB->IntraFlag;

            if(IntraFlag)
                PDCPredictionTable[pContext->m_seqLayerHeader->DQUANT](pContext);

            sMB->ZigzagTable = ZigZagTables_PB_luma[sMB->ACPRED];

            for(blk_num = 0; blk_num < VC1_NUM_OF_LUMA; blk_num++)
            {
                if(IntraFlag&1)
                    vc1Res = BLKLayer_Intra_Luma(pContext, blk_num, 128,sMB->ACPRED);
                else
                    vc1Res = BLKLayer_Inter_Luma(pContext, blk_num);

               if(vc1Res != VC1_OK)
                {
                    VM_ASSERT(0);
                    break;
                }
                IntraFlag >>= 1;
             }

            sMB->ZigzagTable = ZigZagTables_PB_chroma[sMB->ACPRED];

            if(IntraFlag & 1)
                for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
                {
                    //all MB intra
                    vc1Res = BLKLayer_Intra_Chroma(pContext, blk_num, 128, sMB->ACPRED);
                    if(vc1Res != VC1_OK)
                    {
                        VM_ASSERT(0);
                        break;
                    }
                }
            else
                for(blk_num = VC1_NUM_OF_LUMA; blk_num < VC1_NUM_OF_BLOCKS; blk_num++)
                {
                    //all MB inter
                    vc1Res = BLKLayer_Inter_Chroma(pContext, blk_num);
                    if(vc1Res != VC1_OK)
                    {
                        VM_ASSERT(0);
                        break;
                    }
                }

    }//skipmb

    // VM_Debug::GetInstance().vm_debug_frame(-1,VC1_POSITION,VM_STRING("Macroblock Type: %d\n"), pContext->m_pCurrMB->mbType);

    //if (pContext->m_seqLayerHeader->LOOPFILTER)
        AssignCodedBlockPattern(pCurrMB,sMB);

    return vc1Res;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
