/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 decoder, P picture layer for advanced profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"

#include "umc_vc1_dec_time_statistics.h"

static const Ipp32u bc_lut_1[] = {4,0,1,3};
static const Ipp32u bc_lut_2[] = {0,1,2,3};
static const Ipp32u bc_lut_4[] = {0,1,2};
static const Ipp32u bc_lut_5[] = {0,0,1};

typedef void (*ExpandIntens)(VC1Context* pContext, Frame* pFrame);

static const ExpandIntens ExpandIntens_table[] =
             {
               (ExpandIntens)(ExpandFrame_Adv),
               (ExpandIntens)(ExpandFrame_Interlace),
               (ExpandIntens)(ExpandFrame_Interlace)
             };

VC1Status DecodePictHeaderParams_ProgressivePpicture_Adv    (VC1Context* pContext)
{
    VC1Status vc1Res = VC1_OK;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;

    ChooseTTMB_TTBLK_SBP(pContext);

    //if (!pContext->m_picLayerHeader->is_slice)
    //    memset(pContext->savedMV,0X7F,sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*4*2);

    //extended MV range flag
    MVRangeDecode(pContext);

    //motion vector mode
    if(picLayerHeader->PQUANT > 12)
    {
        //VC-1 Table 45: P Picture Low rate (PQUANT > 12) MVMODE codetable
        //MVMODE VLC    Mode
        //1             1 MV Half-pel bilinear
        //01            1 MV
        //001           1 MV Half-pel
        //0000          Mixed MV
        //0001          Intensity Compensation
        Ipp32s bit_count = 1;
        VC1_GET_BITS(1, picLayerHeader->MVMODE);
        while((picLayerHeader->MVMODE == 0) && (bit_count < 4))
        {
            VC1_GET_BITS(1, picLayerHeader->MVMODE);
            bit_count++;
        }
        if (bit_count < 4)
            picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_1);
        else
            if(picLayerHeader->MVMODE == 0)
                picLayerHeader->MVMODE = VC1_MVMODE_MIXED_MV;
            else
            {
                pContext->m_bIntensityCompensation = 1;
                picLayerHeader->MVMODE = VC1_MVMODE_INTENSCOMP;
                //VC-1 Table 48: P Picture Low rate (PQUANT > 12) MVMODE2 codetable
                //MVMODE VLC    Mode
                //1                1 MV Half-pel bilinear
                //01            1 MV
                //001            1 MV Half-pel
                //000            Mixed MV
                Ipp32s bit_count = 1;
                VC1_GET_BITS(1, picLayerHeader->MVMODE);
                while((picLayerHeader->MVMODE == 0) && (bit_count < 3))
                {
                    VC1_GET_BITS(1, picLayerHeader->MVMODE);
                    bit_count++;
                }
                if (bit_count < 3)
                picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_5);
                else
                    if(picLayerHeader->MVMODE == 0)
                        picLayerHeader->MVMODE = VC1_MVMODE_MIXED_MV;
                    else
                        picLayerHeader->MVMODE = VC1_MVMODE_HPEL_1MV;
                //Luma scale
                VC1_GET_BITS(6, picLayerHeader->LUMSCALE);

                //Luma shift
                VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);
                STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);

                FillTablesForIntensityCompensation_Adv(pContext,
                                                       picLayerHeader->LUMSCALE,
                                                       picLayerHeader->LUMSHIFT,
                                                       0,
                                                       pContext->m_frmBuff.m_iPrevIndex);

            STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                m_timeStatistics->intensity_EndTime,
                                m_timeStatistics->intensity_TotalTime);

                pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].m_bIsExpanded = 0;
                pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].ICFieldMask = 0xC;
                //if (pContext->typeOfPreviousFrame != VC1_FieldInterlace)
                //ExpandIntens_table[pContext->typeOfPreviousFrame](pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);
                //else
                //    ExpandFrame_Interlace(pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);

            pContext->m_picLayerHeader->MVMODE2 = pContext->m_picLayerHeader->MVMODE;
        }
    }
    else
    {
        //VC-1 Table 46: P Picture High rate (PQUANT <= 12) MVMODE codetable
        //MVMODE VLC    Mode
        //1                1 MV
        //01            Mixed MV
        //001            1 MV Half-pel
        //0000            1 MV Half-pel bilinear
        //0001            Intensity Compensation
        Ipp32s bit_count = 1;
        VC1_GET_BITS(1, picLayerHeader->MVMODE);
        while((picLayerHeader->MVMODE == 0) && (bit_count < 4))
        {
            VC1_GET_BITS(1, picLayerHeader->MVMODE);
            bit_count++;
        }
        if (bit_count < 4)
            picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_2);
        else
            if(picLayerHeader->MVMODE == 0)
                picLayerHeader->MVMODE = VC1_MVMODE_HPELBI_1MV;
            else
            {
                pContext->m_bIntensityCompensation = 1;
                picLayerHeader->MVMODE = VC1_MVMODE_INTENSCOMP;
                //VC-1 Table 49: P Picture High rate (PQUANT <= 12) MVMODE2 codetable
                //MVMODE VLC    Mode
                //1                1 MV
                //01            Mixed MV
                //001            1 MV Half-pel
                //000            1 MV Half-pel bilinear
                Ipp32s bit_count = 1;
                VC1_GET_BITS(1, picLayerHeader->MVMODE);
                while((picLayerHeader->MVMODE == 0) && (bit_count < 3))
                {
                    VC1_GET_BITS(1, picLayerHeader->MVMODE);
                    bit_count++;
                }
                if (bit_count < 3)
                    picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_4);
                else
                    if(picLayerHeader->MVMODE == 0)
                        picLayerHeader->MVMODE = VC1_MVMODE_HPELBI_1MV;
                    else
                        picLayerHeader->MVMODE = VC1_MVMODE_HPEL_1MV;

                //Luma scale
                VC1_GET_BITS(6, picLayerHeader->LUMSCALE);

                //Luma shift
                VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);
                STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);

                FillTablesForIntensityCompensation_Adv(pContext,
                                                       picLayerHeader->LUMSCALE,
                                                       picLayerHeader->LUMSHIFT,
                                                       0,
                                                       pContext->m_frmBuff.m_iPrevIndex);

                STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                    m_timeStatistics->intensity_EndTime,
                                    m_timeStatistics->intensity_TotalTime);

                pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].m_bIsExpanded = 0;
                //if (pContext->typeOfPreviousFrame != VC1_FieldInterlace)
                //ExpandIntens_table[pContext->typeOfPreviousFrame](pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);
                //else
                //    ExpandFrame_Interlace(pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);
            pContext->m_picLayerHeader->MVMODE2 = pContext->m_picLayerHeader->MVMODE;
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].ICFieldMask = 0xC;
            }
    }

    //motion vector type bitplane
    if(picLayerHeader->MVMODE == VC1_MVMODE_MIXED_MV)
    {
        DecodeBitplane(pContext, &picLayerHeader->MVTYPEMB,
                        seqLayerHeader->widthMB, seqLayerHeader->heightMB,0);
    }

    //skipped macroblock bit syntax element
        DecodeBitplane(pContext, &picLayerHeader->SKIPMB,
            seqLayerHeader->widthMB, seqLayerHeader->heightMB,0);

    //motion vector table
    VC1_GET_BITS(2, picLayerHeader->MVTAB);       //MVTAB

    picLayerHeader->m_pCurrMVDifftbl =
        pContext->m_vlcTbl->MVDIFF_PB_TABLES[picLayerHeader->MVTAB];  //MVTAB


    //coded block pattern table
    VC1_GET_BITS(2, picLayerHeader->CBPTAB);       //CBPTAB

    picLayerHeader->m_pCurrCBPCYtbl = pContext->m_vlcTbl->CBPCY_PB_TABLES[picLayerHeader->CBPTAB];       //CBPTAB

    vc1Res = VOPDQuant(pContext);

    if(seqLayerHeader->VSTRANSFORM)
    {
        //macroblock - level transform type flag
        VC1_GET_BITS(1, picLayerHeader->TTMBF);

        if(picLayerHeader->TTMBF)
        {
            //frame-level transform type
            VC1_GET_BITS(2, picLayerHeader->TTFRM_ORIG);
            picLayerHeader->TTFRM = 1 << picLayerHeader->TTFRM_ORIG;
        }
        else
        {
            //_MAYBE_ see reference decoder - vc1decpic.c vc1DECPIC_UnpackPictureLayerPBAdvanced function
            picLayerHeader->TTFRM = VC1_BLK_INTER;
        }
    }
    else
    {
        picLayerHeader->TTFRM = VC1_BLK_INTER8X8;
    }

    //frame-level transform AC Coding set index
    VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);//TRANSACFRM

    if(picLayerHeader->TRANSACFRM == 1)
    {
        VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);
        picLayerHeader->TRANSACFRM++;
    }
    picLayerHeader->TRANSACFRM2 = picLayerHeader->TRANSACFRM;
    ChooseACTable(pContext, picLayerHeader->TRANSACFRM, picLayerHeader->TRANSACFRM);//TRANSACFRM

    //intra transfrmDC table
    VC1_GET_BITS(1, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB
    ChooseDCTable(pContext, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB

    return vc1Res;
}

VC1Status DecodePictHeaderParams_InterlacePpicture_Adv    (VC1Context* pContext)
{
    VC1Status vc1Res = VC1_OK;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;

    Ipp32u tempValue;


    ChooseTTMB_TTBLK_SBP(pContext);

    //if (!picLayerHeader->is_slice)
    //    memset(pContext->savedMV,0X7F,sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*4*2*2);

    //extended MV range flag
    MVRangeDecode(pContext);

    //extended differential MV Range Flag
    if(seqLayerHeader->EXTENDED_DMV == 1)
    {
        VC1_GET_BITS(1, picLayerHeader->DMVRANGE);
        if(picLayerHeader->DMVRANGE==0)
        {
            //binary code 0
            picLayerHeader->DMVRANGE = VC1_DMVRANGE_NONE;
        }
        else
        {
            VC1_GET_BITS(1, picLayerHeader->DMVRANGE);
            if(picLayerHeader->DMVRANGE==0)
            {
               //binary code 10
               picLayerHeader->DMVRANGE = VC1_DMVRANGE_HORIZONTAL_RANGE;
            }
            else
            {
                VC1_GET_BITS(1, picLayerHeader->DMVRANGE);
                if(picLayerHeader->DMVRANGE==0)
                {
                    //binary code 110
                    picLayerHeader->DMVRANGE = VC1_DMVRANGE_VERTICAL_RANGE;
                }
                else
                {
                    //binary code 111
                    picLayerHeader->DMVRANGE = VC1_DMVRANGE_HORIZONTAL_VERTICAL_RANGE;
                }
            }
        }
    }

    //4 motion vector switch
    VC1_GET_BITS(1, picLayerHeader->MV4SWITCH);
    if(picLayerHeader->MV4SWITCH)
       picLayerHeader->MVMODE = VC1_MVMODE_MIXED_MV;
    else
       picLayerHeader->MVMODE = VC1_MVMODE_1MV;

    //intensity compensation
    VC1_GET_BITS(1, tempValue);       //INTCOMP

    if(tempValue)       //INTCOM
    {
        pContext->m_bIntensityCompensation = 1;
        VC1_GET_BITS(6, picLayerHeader->LUMSCALE);
        VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);

       STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);

       FillTablesForIntensityCompensation_Adv(pContext,picLayerHeader->LUMSCALE,picLayerHeader->LUMSHIFT,
           0,pContext->m_frmBuff.m_iPrevIndex);

       STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                        m_timeStatistics->intensity_EndTime,
                        m_timeStatistics->intensity_TotalTime);
       pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].ICFieldMask = 0xC;

       //pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].m_bIsExpanded = 0;
       //if (pContext->typeOfPreviousFrame != VC1_FieldInterlace)
       //ExpandIntens_table[pContext->typeOfPreviousFrame](pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);
       //else
       //ExpandFrame_Interlace(pContext, &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex]);
    }


    //skipped macroblock bit syntax element
    DecodeBitplane(pContext, &picLayerHeader->SKIPMB,
                   seqLayerHeader->widthMB, seqLayerHeader->heightMB,0);

    //motion vector table
    VC1_GET_BITS(2, picLayerHeader->MBMODETAB);       //MBMODETAB
    ChooseMBModeInterlaceFrame(pContext, picLayerHeader->MV4SWITCH, picLayerHeader->MBMODETAB);

    //motion vector table
    VC1_GET_BITS(2,picLayerHeader->MVTAB);       //MVTAB
    picLayerHeader->m_pCurrMVDifftbl =
                pContext->m_vlcTbl->MV_INTERLACE_TABLES[8 + picLayerHeader->MVTAB]; //MVTAB


    //coded block pattern table
    VC1_GET_BITS(3,picLayerHeader->CBPTAB);       //CBPTAB
    picLayerHeader->m_pCurrCBPCYtbl =
                 pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[picLayerHeader->CBPTAB];     //CBPTAB

    VC1_GET_BITS(2, picLayerHeader->MV2BPTAB);       //MV2BPTAB
    picLayerHeader->m_pMV2BP = pContext->m_vlcTbl->MV2BP_TABLES[picLayerHeader->MV2BPTAB];

    if(picLayerHeader->MV4SWITCH)
    {
        VC1_GET_BITS(2, picLayerHeader->MV4BPTAB)        //MV4BPTAB;
        picLayerHeader->m_pMV4BP = pContext->m_vlcTbl->MV4BP_TABLES[picLayerHeader->MV4BPTAB];
    }

    vc1Res = VOPDQuant(pContext);

    if(seqLayerHeader->VSTRANSFORM == 1)
    {
        //macroblock - level transform type flag
        VC1_GET_BITS(1, picLayerHeader->TTMBF);

        if(picLayerHeader->TTMBF)
        {
            //frame-level transform type
            VC1_GET_BITS(2, picLayerHeader->TTFRM_ORIG);
            picLayerHeader->TTFRM = 1 << picLayerHeader->TTFRM_ORIG;
        }
        else
            picLayerHeader->TTFRM = VC1_BLK_INTER;
    }
    else
    {
        picLayerHeader->TTFRM = VC1_BLK_INTER8X8;
    }

    //frame-level transform AC Coding set index
    VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);//TRANSACFRM
    if(picLayerHeader->TRANSACFRM == 1)
    {
        VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);
        picLayerHeader->TRANSACFRM++;
    }
    ChooseACTable(pContext, picLayerHeader->TRANSACFRM, picLayerHeader->TRANSACFRM);//TRANSACFRM

    VC1_GET_BITS(1, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB
    ChooseDCTable(pContext, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB

    return vc1Res;
}

VC1Status DecodeFieldHeaderParams_InterlaceFieldPpicture_Adv (VC1Context* pContext)
{
    VC1Status vc1Res = VC1_OK;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;

    Ipp32u tempValue;


    //if (!picLayerHeader->is_slice)
    //{
    //    if (!pContext->m_picLayerHeader->CurrField)
    //        memset(pContext->savedMV,0X7F,sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*4*2);
    //    else
    //        memset(pContext->savedMV + seqLayerHeader->heightMB*seqLayerHeader->widthMB*4*2,0X7F,
    //                            sizeof(Ipp16s)*seqLayerHeader->heightMB*seqLayerHeader->widthMB*4*2);
    //}


    VC1_GET_BITS(5,picLayerHeader->PQINDEX);
    CalculatePQuant(pContext);

    ChooseTTMB_TTBLK_SBP(pContext);

    if(picLayerHeader->PQINDEX<=8)
    {
        VC1_GET_BITS(1,picLayerHeader->HALFQP);
    }
    else
        picLayerHeader->HALFQP = 0;


    if(seqLayerHeader->QUANTIZER == 1)
    {
        VC1_GET_BITS(1,picLayerHeader->PQUANTIZER);    //PQUANTIZER
    }

    if(seqLayerHeader->POSTPROCFLAG)
    {
         //post processing
          VC1_GET_BITS(2,tempValue);        //POSTPROC
    }

    //NUMREF
    VC1_GET_BITS(1,picLayerHeader->NUMREF);

    if(!picLayerHeader->NUMREF)
    {
        VC1_GET_BITS(1,picLayerHeader->REFFIELD);
    }

    //extended MV range flag
    MVRangeDecode(pContext);

    //for scaling mv predictors
    ChoosePredScaleValuePPictbl(picLayerHeader);

    //extended differential MV Range Flag
    DMVRangeDecode(pContext);

    picLayerHeader->INTCOMFIELD = 0;

    //motion vector mode
    if(picLayerHeader->PQUANT > 12)
    {
        //MVMODE VLC    Mode
        //1             1 MV Half-pel bilinear
        //01            1 MV
        //001           1 MV Half-pel
        //0000          Mixed MV
        //0001          Intensity Compensation
        Ipp32s bit_count = 1;
        VC1_GET_BITS(1, picLayerHeader->MVMODE);
        while((picLayerHeader->MVMODE == 0) && (bit_count < 4))
        {
            VC1_GET_BITS(1, picLayerHeader->MVMODE);
            bit_count++;
        }
        if (bit_count < 4)
            picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_1);
        else
            if(picLayerHeader->MVMODE == 0)
                picLayerHeader->MVMODE = VC1_MVMODE_MIXED_MV;
            else
            {
                pContext->m_bIntensityCompensation = 1;
                picLayerHeader->MVMODE = VC1_MVMODE_INTENSCOMP;

                {
                    Ipp32u is_bottom = 0;
                    Ipp32u is_top    = 0;
                    Ipp32s index_bottom = pContext->m_frmBuff.m_iPrevIndex;
                    Ipp32s index_top    = pContext->m_frmBuff.m_iPrevIndex;
                    //pContext->m_bIntensityCompensation = 0;

                    //MVMODE VLC    Mode
                    //1             1 MV Half-pel bilinear
                    //01            1 MV
                    //001           1 MV Half-pel
                    //000           Mixed MV
                    Ipp32s bit_count = 1;
                    VC1_GET_BITS(1, picLayerHeader->MVMODE);
                    while((picLayerHeader->MVMODE == 0) && (bit_count < 3))
                    {
                        VC1_GET_BITS(1, picLayerHeader->MVMODE);
                        bit_count++;
                    }
                    if (bit_count < 3)
                        picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_5);
                    else
                        if(picLayerHeader->MVMODE == 0)
                            picLayerHeader->MVMODE = VC1_MVMODE_MIXED_MV;
                        else
                            picLayerHeader->MVMODE = VC1_MVMODE_HPEL_1MV;

                    if (picLayerHeader->CurrField)
                    {
                        pContext->m_bNeedToUseCompBuffer = 1;
                        if (picLayerHeader->BottomField)
                            index_top = pContext->m_frmBuff.m_iCurrIndex;
                        else
                            index_bottom = pContext->m_frmBuff.m_iCurrIndex;
                    }

                    //INTCOMPFIELD
                    VC1_GET_BITS(1, picLayerHeader->INTCOMFIELD);
                    if(picLayerHeader->INTCOMFIELD == 1)
                    {
                        //1
                        picLayerHeader->INTCOMFIELD = VC1_INTCOMP_BOTH_FIELD;
                    }
                    else
                    {
                        VC1_GET_BITS(1, picLayerHeader->INTCOMFIELD);
                        if(picLayerHeader->INTCOMFIELD == 1)
                        {
                            //01
                            picLayerHeader->INTCOMFIELD = VC1_INTCOMP_BOTTOM_FIELD;
                        }
                        else
                        {
                            //00
                            picLayerHeader->INTCOMFIELD = VC1_INTCOMP_TOP_FIELD;
                        }
                    }

                    if(VC1_IS_INT_TOP_FIELD(picLayerHeader->INTCOMFIELD))
                    {
                        is_top    = 1;
                        //top
                        //Luma scale
                        VC1_GET_BITS(6, picLayerHeader->LUMSCALE);
                        //Luma shift
                        VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);
                        STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);
                        FillTablesForIntensityCompensation_Adv(pContext, picLayerHeader->LUMSCALE,picLayerHeader->LUMSHIFT,
                            0,index_top);

                        STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                    m_timeStatistics->intensity_EndTime,
                                    m_timeStatistics->intensity_TotalTime);
                    }

                    if(VC1_IS_INT_BOTTOM_FIELD(picLayerHeader->INTCOMFIELD) )
                    {
                        is_bottom = 1;
                        //bottom in case "both field"
                    // VM_ASSERT(0);
                        //Luma scale
                        VC1_GET_BITS(6, picLayerHeader->LUMSCALE);
                        //Luma shift
                        VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);

                        STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);
                        FillTablesForIntensityCompensation_Adv(pContext, picLayerHeader->LUMSCALE,picLayerHeader->LUMSHIFT,
                                            1,index_bottom);
                    STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                    m_timeStatistics->intensity_EndTime,
                                    m_timeStatistics->intensity_TotalTime);
                    }
                    pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].ICFieldMask |= picLayerHeader->INTCOMFIELD << (2 * (1 - picLayerHeader->CurrField));

                    pContext->m_frmBuff.m_pFrames[index_top].m_bIsExpanded = 0;

                    picLayerHeader->MVMODE2 = picLayerHeader->MVMODE;
                }
            }
    }
    else
    {
        //MVMODE VLC    Mode
        //1             1 MV
        //01            Mixed MV
        //001           1 MV Half-pel
        //0000          1 MV Half-pel bilinear
        //0001          Intensity Compensation
        Ipp32s bit_count = 1;
        VC1_GET_BITS(1, picLayerHeader->MVMODE);
        while((picLayerHeader->MVMODE == 0) && (bit_count < 4))
        {
            VC1_GET_BITS(1, picLayerHeader->MVMODE);
            bit_count++;
        }
        if (bit_count < 4)
            picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_2);
        else
            if(picLayerHeader->MVMODE == 0)
                picLayerHeader->MVMODE = VC1_MVMODE_HPELBI_1MV;
            else
            {
                pContext->m_bIntensityCompensation = 1;
                picLayerHeader->MVMODE = VC1_MVMODE_INTENSCOMP;
                {
                    Ipp32u is_bottom = 0;
                    Ipp32u is_top    = 0;
                    Ipp32s index_bottom = pContext->m_frmBuff.m_iPrevIndex;
                    Ipp32s index_top = pContext->m_frmBuff.m_iPrevIndex;

                    Ipp32s bit_count = 1;
                    VC1_GET_BITS(1, picLayerHeader->MVMODE);
                    while((picLayerHeader->MVMODE == 0) && (bit_count < 3))
                    {
                        VC1_GET_BITS(1, picLayerHeader->MVMODE);
                        bit_count++;
                    }
                    if (bit_count < 3)
                        picLayerHeader->MVMODE = VC1_LUT_SET(bit_count,bc_lut_4);
                    else
                        if(picLayerHeader->MVMODE == 0)
                            picLayerHeader->MVMODE = VC1_MVMODE_HPELBI_1MV;
                        else
                            picLayerHeader->MVMODE = VC1_MVMODE_HPEL_1MV;

                    if (picLayerHeader->CurrField)
                    {
                        pContext->m_bNeedToUseCompBuffer = 1;
                        if (picLayerHeader->BottomField)
                            index_top = pContext->m_frmBuff.m_iCurrIndex;
                        else
                            index_bottom = pContext->m_frmBuff.m_iCurrIndex;
                    }

                    //INTCOMPFIELD
                    VC1_GET_BITS(1, picLayerHeader->INTCOMFIELD);
                    if(picLayerHeader->INTCOMFIELD == 1)
                    {
                        //1
                        picLayerHeader->INTCOMFIELD = VC1_INTCOMP_BOTH_FIELD;
                    }
                    else
                    {
                        VC1_GET_BITS(1, picLayerHeader->INTCOMFIELD);
                        if(picLayerHeader->INTCOMFIELD == 1)
                        {
                            //01
                            picLayerHeader->INTCOMFIELD = VC1_INTCOMP_BOTTOM_FIELD;
                        }
                        else
                        {
                            //00
                            picLayerHeader->INTCOMFIELD = VC1_INTCOMP_TOP_FIELD;
                        }
                    }

                    if(VC1_IS_INT_TOP_FIELD(picLayerHeader->INTCOMFIELD))
                    {
                        is_top    = 1;
                        //top
                        //Luma scale
                        VC1_GET_BITS(6, picLayerHeader->LUMSCALE);
                        //Luma shift
                        VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);
                        STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);
                        FillTablesForIntensityCompensation_Adv(pContext, picLayerHeader->LUMSCALE,picLayerHeader->LUMSHIFT,
                            0,index_top);

                        STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                    m_timeStatistics->intensity_EndTime,
                                    m_timeStatistics->intensity_TotalTime);
                    }

                    if(VC1_IS_INT_BOTTOM_FIELD(picLayerHeader->INTCOMFIELD) )
                    {
                        is_bottom = 1;
                        //bottom in case "both field"
                    // VM_ASSERT(0);
                        //Luma scale
                        VC1_GET_BITS(6, picLayerHeader->LUMSCALE);
                        //Luma shift
                        VC1_GET_BITS(6, picLayerHeader->LUMSHIFT);

                        STATISTICS_START_TIME(m_timeStatistics->intensity_StartTime);
                        FillTablesForIntensityCompensation_Adv(pContext,
                                                               picLayerHeader->LUMSCALE,
                                                               picLayerHeader->LUMSHIFT,
                                                               1,
                                                               index_bottom);
                    STATISTICS_END_TIME(m_timeStatistics->intensity_StartTime,
                                    m_timeStatistics->intensity_EndTime,
                                    m_timeStatistics->intensity_TotalTime);
                    }

                    pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].ICFieldMask |= picLayerHeader->INTCOMFIELD << (2*(1 - picLayerHeader->CurrField));

                    picLayerHeader->MVMODE2 = picLayerHeader->MVMODE;
                }
            }
    }

    //motion vector table
    VC1_GET_BITS(3, picLayerHeader->MBMODETAB);       //MBMODETAB
    ChooseMBModeInterlaceField(pContext, picLayerHeader->MBMODETAB);

    //motion vector table
    if(picLayerHeader->NUMREF)
    {
        VC1_GET_BITS(3, picLayerHeader->MVTAB);       //MVTAB
        picLayerHeader->m_pCurrMVDifftbl =  pContext->m_vlcTbl->MV_INTERLACE_TABLES[picLayerHeader->MVTAB]; //MVTAB
    }
    else
    {
        VC1_GET_BITS(2, picLayerHeader->MVTAB);       //MVTAB
        picLayerHeader->m_pCurrMVDifftbl =  pContext->m_vlcTbl->MV_INTERLACE_TABLES[8 + picLayerHeader->MVTAB]; //MVTAB
    }

    //coded block pattern table
    VC1_GET_BITS(3, picLayerHeader->CBPTAB);       //CBPTAB
    picLayerHeader->m_pCurrCBPCYtbl = pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[picLayerHeader->CBPTAB]; //CBPTAB

    if(picLayerHeader->MVMODE == VC1_MVMODE_MIXED_MV)
    {
        VC1_GET_BITS(2, picLayerHeader->MV4BPTAB)        //MV4BPTAB;
        picLayerHeader->m_pMV4BP = pContext->m_vlcTbl->MV4BP_TABLES[picLayerHeader->MV4BPTAB];
    }

    vc1Res = VOPDQuant(pContext);

    if(seqLayerHeader->VSTRANSFORM == 1)
    {
        //macroblock - level transform type flag
        VC1_GET_BITS(1, picLayerHeader->TTMBF);

        if(picLayerHeader->TTMBF)
        {
            //frame-level transform type
            VC1_GET_BITS(2, picLayerHeader->TTFRM_ORIG);
            picLayerHeader->TTFRM = 1 << picLayerHeader->TTFRM_ORIG;
        }
        else
            picLayerHeader->TTFRM = VC1_BLK_INTER;
    }
    else
    {
        picLayerHeader->TTFRM = VC1_BLK_INTER8X8;
    }

    //frame-level transform AC Coding set index
    VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);//TRANSACFRM
    if(picLayerHeader->TRANSACFRM == 1)
    {
        VC1_GET_BITS(1, picLayerHeader->TRANSACFRM);
        picLayerHeader->TRANSACFRM++;
    }
     ChooseACTable(pContext, picLayerHeader->TRANSACFRM, picLayerHeader->TRANSACFRM);//TRANSACFRM

    VC1_GET_BITS(1, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB
    ChooseDCTable(pContext, picLayerHeader->TRANSDCTAB);       //TRANSDCTAB

   return vc1Res;
}

VC1Status Decode_InterlaceFieldPpicture_Adv(VC1Context* pContext)
{
    Ipp32s i, j;
    VC1Status vc1Res = VC1_OK;
    VC1SingletonMB* sMB = pContext->m_pSingleMB;

    DecodeFieldHeaderParams_InterlaceFieldPpicture_Adv(pContext);

    for(i = 0; i < sMB->widthMB; i++)
    {
        for(j = 0; j < (sMB->heightMB/2);j++)
        {
            vc1Res = MBLayer_Field_InterlacedPpicture(pContext);
            if(vc1Res != VC1_OK)
            {
                VM_ASSERT(0);
                break;
            }

            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_POSITION,VM_STRING("Macroblock Type: %d\n"), pContext->m_pCurrMB->mbType);
            sMB->m_currMBXpos++;
            pContext->m_pBlock += 8*8*6;

            pContext->m_pCurrMB++;
            pContext->CurrDC++;
        }
        sMB->m_currMBXpos = 0;
        sMB->m_currMBYpos++;
        sMB->slice_currMBYpos++;
    }

    if ((pContext->m_seqLayerHeader->LOOPFILTER))
    {
        Ipp32u deblock_offset = 0;
        if (!pContext->DeblockInfo.is_last_deblock)
            deblock_offset = 1;

        pContext->DeblockInfo.start_pos = pContext->DeblockInfo.start_pos+pContext->DeblockInfo.HeightMB-deblock_offset;
        pContext->DeblockInfo.HeightMB = sMB->slice_currMBYpos+1;

        pContext->DeblockInfo.is_last_deblock = 1;

        STATISTICS_START_TIME(m_timeStatistics->deblocking_StartTime);

        Deblocking_ProgressivePpicture_Adv(pContext);

        STATISTICS_END_TIME(m_timeStatistics->deblocking_StartTime,
                            m_timeStatistics->deblocking_EndTime,
                            m_timeStatistics->deblocking_TotalTime);
    }

    pContext->m_picLayerHeader->is_slice = 0;

    return vc1Res;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
