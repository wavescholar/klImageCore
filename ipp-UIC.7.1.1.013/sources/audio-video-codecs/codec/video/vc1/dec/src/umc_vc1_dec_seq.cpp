/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Sequence layer
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "string.h"

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_common_dc_tbl.h"
#include "umc_vc1_common_cbpcy_tbl.h"
#include "umc_vc1_dec_run_level_tbl.h"
#include "umc_vc1_common_tables.h"
#include "umc_vc1_common_mvdiff_tbl.h"
#include "umc_vc1_common_ttmb_tbl.h"
#include "umc_vc1_common_interlace_mb_mode_tables.h"
#include "umc_vc1_common_interlace_mv_tables.h"
#include "umc_vc1_common_interlaced_cbpcy_tables.h"
#include "umc_vc1_common_mv_block_pattern_tables.h"
#include "umc_vc1_common_tables_adv.h"
#include "umc_vc1_common_defs.h"

#include "umc_vc1_dec_task.h"

#include "umc_vc1_dec_time_statistics.h"
#include "umc_vc1_common.h"
#include "umc_structures.h"

#include "umc_vc1_common_acintra.h"
#include "umc_vc1_common_acinter.h"

#include <assert.h>
#include "vm_debug.h"


#ifdef BSWAP
#undef BSWAP
#define BSWAP(x)    (Ipp32u)(((x) << 24) + (((x)&0xff00) << 8) + (((x) >> 8)&0xff00) + ((x) >> 24))
#endif
using namespace UMC;
using namespace UMC::VC1Common;

static void reset_index(VC1Context* pContext)
{
    pContext->m_frmBuff.m_iPrevIndex  = -1;
    pContext->m_frmBuff.m_iNextIndex  = -1;
}


//3.1    Sequence-level Syntax and Semantics, figure 7
VC1Status SequenceLayer(VC1Context* pContext)
{
    Ipp32u reserved;
    Ipp32u i=0;
    Ipp32u tempValue;

    pContext->m_seqInit = 0;

    VC1_GET_BITS(2, pContext->m_seqLayerHeader->PROFILE);
    if (!VC1_IS_VALID_PROFILE(pContext->m_seqLayerHeader->PROFILE))
        return VC1_WRN_INVALID_STREAM;

    if(pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)
    {
        VC1_GET_BITS(3, pContext->m_seqLayerHeader->LEVEL);

        VC1_GET_BITS(2,tempValue);     //CHROMAFORMAT
    }
    else
    {
        VC1_GET_BITS(2, pContext->m_seqLayerHeader->LEVEL);
    }

    VC1_GET_BITS(3, pContext->m_seqLayerHeader->FRMRTQ_POSTPROC);

    VC1_GET_BITS(5, pContext->m_seqLayerHeader->BITRTQ_POSTPROC);

    if(pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)
    {

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->POSTPROCFLAG);

        VC1_GET_BITS(12, pContext->m_seqLayerHeader->MAX_CODED_WIDTH);

        VC1_GET_BITS(12, pContext->m_seqLayerHeader->MAX_CODED_HEIGHT);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->PULLDOWN);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->INTERLACE);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->TFCNTRFLAG);
    }
    else
    {
        VC1_GET_BITS(1, pContext->m_seqLayerHeader->LOOPFILTER);

        VC1_GET_BITS(1, reserved);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->MULTIRES);

        VC1_GET_BITS(1, reserved);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->FASTUVMC);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->EXTENDED_MV);

        VC1_GET_BITS(2, pContext->m_seqLayerHeader->DQUANT);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->VSTRANSFORM);

        VC1_GET_BITS(1, reserved);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->OVERLAP);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->SYNCMARKER);

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->RANGERED);

        VC1_GET_BITS(3, pContext->m_seqLayerHeader->MAXBFRAMES);

        VC1_GET_BITS(2, pContext->m_seqLayerHeader->QUANTIZER);
    }

    VC1_GET_BITS(1, pContext->m_seqLayerHeader->FINTERPFLAG);

    if(pContext->m_seqLayerHeader->PROFILE == VC1_PROFILE_ADVANCED)
    {
        VC1_GET_BITS(2, reserved);

        VC1_GET_BITS(1, tempValue);//DISPLAY_EXT

        if(tempValue)//DISPLAY_EXT
        {
            VC1_GET_BITS(14,tempValue);     //DISP_HORIZ_SIZE

            VC1_GET_BITS(14,tempValue);     //DISP_VERT_SIZE

            VC1_GET_BITS(1,tempValue); //ASPECT_RATIO_FLAG

            if(tempValue)
            {
                VC1_GET_BITS(4,tempValue);        //ASPECT_RATIO

                if(tempValue==15)
                {
                    VC1_GET_BITS(8,tempValue);        //ASPECT_HORIZ_SIZE

                    VC1_GET_BITS(8,tempValue);      //ASPECT_VERT_SIZE
                }
            }

            VC1_GET_BITS(1,tempValue);      //FRAMERATE_FLAG

            if(tempValue)       //FRAMERATE_FLAG
            {
                VC1_GET_BITS(1,tempValue);    //FRAMERATEIND

                if(!tempValue)      //FRAMERATEIND
                {
                    VC1_GET_BITS(8,tempValue);      //FRAMERATENR

                    VC1_GET_BITS(4,tempValue);  //FRAMERATEDR
                }
                else
                {
                    VC1_GET_BITS(16,tempValue);     //FRAMERATEEXP
                }

            }

            VC1_GET_BITS(1,tempValue);      //COLOR_FORMAT_FLAG

            if(tempValue)       //COLOR_FORMAT_FLAG
            {
                VC1_GET_BITS(8,tempValue);        //COLOR_PRIM
                VC1_GET_BITS(8,tempValue);        //TRANSFER_CHAR
                VC1_GET_BITS(8,tempValue);        //MATRIX_COEF
            }

        }

        VC1_GET_BITS(1, pContext->m_seqLayerHeader->HRD_PARAM_FLAG);
        if(pContext->m_seqLayerHeader->HRD_PARAM_FLAG)
        {
            VC1_GET_BITS(5,pContext->m_seqLayerHeader->HRD_NUM_LEAKY_BUCKETS);
            VC1_GET_BITS(4,tempValue);//BIT_RATE_EXPONENT
            VC1_GET_BITS(4,tempValue);//BUFFER_SIZE_EXPONENT

            for(i=0; i<pContext->m_seqLayerHeader->HRD_NUM_LEAKY_BUCKETS; i++)
            {
                VC1_GET_BITS(16,tempValue);//HRD_RATE[i]
                VC1_GET_BITS(16,tempValue);//HRD_BUFFER[i]
            }
        }

    }
    pContext->m_seqInit = 1;
    return VC1_OK;

}

VC1Status EntryPointLayer(VC1Context* pContext)
{
    Ipp32u i=0;
    Ipp32u tempValue;

    VC1_GET_BITS(1, pContext->m_seqLayerHeader->BROKEN_LINK);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->CLOSED_ENTRY);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->PANSCAN_FLAG);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->REFDIST_FLAG);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->LOOPFILTER);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->FASTUVMC);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->EXTENDED_MV);
    VC1_GET_BITS(2, pContext->m_seqLayerHeader->DQUANT);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->VSTRANSFORM);
    VC1_GET_BITS(1, pContext->m_seqLayerHeader->OVERLAP);
    VC1_GET_BITS(2, pContext->m_seqLayerHeader->QUANTIZER);

    if (pContext->m_seqLayerHeader->CLOSED_ENTRY)
        pContext->m_seqLayerHeader->BROKEN_LINK = 0;

    if (pContext->m_seqLayerHeader->BROKEN_LINK && !pContext->m_seqLayerHeader->CLOSED_ENTRY)
        reset_index(pContext);

    if(pContext->m_seqLayerHeader->HRD_PARAM_FLAG == 1)
    {
        for(i=0; i<pContext->m_seqLayerHeader->HRD_NUM_LEAKY_BUCKETS;i++)
        {
            VC1_GET_BITS(8, tempValue);       //m_hrd_buffer_fullness.HRD_FULLNESS[i]
        }
    }

    VC1_GET_BITS(1, tempValue);    //CODED_SIZE_FLAG
    if (tempValue == 1)
    {
        VC1_GET_BITS(12, pContext->m_seqLayerHeader->CODED_WIDTH);
        VC1_GET_BITS(12, pContext->m_seqLayerHeader->CODED_HEIGHT);
    }

    if (pContext->m_seqLayerHeader->EXTENDED_MV == 1)
    {
        VC1_GET_BITS(1, pContext->m_seqLayerHeader->EXTENDED_DMV);
    }

    VC1_GET_BITS(1, pContext->m_seqLayerHeader->RANGE_MAPY_FLAG);   //RANGE_MAPY_FLAG
    if (pContext->m_seqLayerHeader->RANGE_MAPY_FLAG == 1)
    {
        VC1_GET_BITS(3,pContext->m_seqLayerHeader->RANGE_MAPY);
    }
    else
        pContext->m_seqLayerHeader->RANGE_MAPY = -1;

    VC1_GET_BITS(1, pContext->m_seqLayerHeader->RANGE_MAPUV_FLAG); //RANGE_MAPUV_FLAG

    if (pContext->m_seqLayerHeader->RANGE_MAPUV_FLAG == 1)
    {
        VC1_GET_BITS(3,pContext->m_seqLayerHeader->RANGE_MAPUV);
    }
    else
        pContext->m_seqLayerHeader->RANGE_MAPUV = -1;

    return VC1_OK;

}
void SetDecodingTables(VC1Context* pContext)
{
    //////////////////////////////////////////////////////////////////////////
    //////////////////////Intra Decoding Sets/////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pDeltaLevelLast0 = VC1_LowMotionIntraDeltaLevelLast0;
    pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pDeltaLevelLast1 = VC1_LowMotionIntraDeltaLevelLast1;
    pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pDeltaRunLast0 = VC1_LowMotionIntraDeltaRunLast0;
    pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pDeltaRunLast1 = VC1_LowMotionIntraDeltaRunLast1;

 
    pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pDeltaLevelLast0 = VC1_HighMotionIntraDeltaLevelLast0;
    pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pDeltaLevelLast1 = VC1_HighMotionIntraDeltaLevelLast1;
    pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pDeltaRunLast0 = VC1_HighMotionIntraDeltaRunLast0;
    pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pDeltaRunLast1 = VC1_HighMotionIntraDeltaRunLast1;

    pContext->m_vlcTbl->MidRateIntraACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->MidRateIntraACDecodeSet.pDeltaLevelLast0 = VC1_MidRateIntraDeltaLevelLast0;
    pContext->m_vlcTbl->MidRateIntraACDecodeSet.pDeltaLevelLast1 = VC1_MidRateIntraDeltaLevelLast1;
    pContext->m_vlcTbl->MidRateIntraACDecodeSet.pDeltaRunLast0 = VC1_MidRateIntraDeltaRunLast0;
    pContext->m_vlcTbl->MidRateIntraACDecodeSet.pDeltaRunLast1 = VC1_MidRateIntraDeltaRunLast1;

    pContext->m_vlcTbl->HighRateIntraACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->HighRateIntraACDecodeSet.pDeltaLevelLast0 = VC1_HighRateIntraDeltaLevelLast0;
    pContext->m_vlcTbl->HighRateIntraACDecodeSet.pDeltaLevelLast1 = VC1_HighRateIntraDeltaLevelLast1;
    pContext->m_vlcTbl->HighRateIntraACDecodeSet.pDeltaRunLast0 = VC1_HighRateIntraDeltaRunLast0;
    pContext->m_vlcTbl->HighRateIntraACDecodeSet.pDeltaRunLast1 = VC1_HighRateIntraDeltaRunLast1;



    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXle7[0] = &pContext->m_vlcTbl->HighRateIntraACDecodeSet;
    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXle7[1] = &pContext->m_vlcTbl->HighMotionIntraACDecodeSet;
    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXle7[2] = &pContext->m_vlcTbl->MidRateIntraACDecodeSet;



    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXgt7[0] = &pContext->m_vlcTbl->LowMotionIntraACDecodeSet;
    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXgt7[1] = &pContext->m_vlcTbl->HighMotionIntraACDecodeSet;
    pContext->m_vlcTbl->IntraACDecodeSetPQINDEXgt7[2] = &pContext->m_vlcTbl->MidRateIntraACDecodeSet;



    //////////////////////////////////////////////////////////////////////////
    //////////////////////Inter Decoding Sets/////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    pContext->m_vlcTbl->LowMotionInterACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->LowMotionInterACDecodeSet.pDeltaLevelLast0 = VC1_LowMotionInterDeltaLevelLast0;
    pContext->m_vlcTbl->LowMotionInterACDecodeSet.pDeltaLevelLast1 = VC1_LowMotionInterDeltaLevelLast1;
    pContext->m_vlcTbl->LowMotionInterACDecodeSet.pDeltaRunLast0 = VC1_LowMotionInterDeltaRunLast0;
    pContext->m_vlcTbl->LowMotionInterACDecodeSet.pDeltaRunLast1 = VC1_LowMotionInterDeltaRunLast1;

    pContext->m_vlcTbl->HighMotionInterACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->HighMotionInterACDecodeSet.pDeltaLevelLast0 = VC1_HighMotionInterDeltaLevelLast0;
    pContext->m_vlcTbl->HighMotionInterACDecodeSet.pDeltaLevelLast1 = VC1_HighMotionInterDeltaLevelLast1;
    pContext->m_vlcTbl->HighMotionInterACDecodeSet.pDeltaRunLast0 =  VC1_HighMotionInterDeltaRunLast0;
    pContext->m_vlcTbl->HighMotionInterACDecodeSet.pDeltaRunLast1 = VC1_HighMotionInterDeltaRunLast1;

    pContext->m_vlcTbl->MidRateInterACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->MidRateInterACDecodeSet.pDeltaLevelLast0 = VC1_MidRateInterDeltaLevelLast0;
    pContext->m_vlcTbl->MidRateInterACDecodeSet.pDeltaLevelLast1 = VC1_MidRateInterDeltaLevelLast1;
    pContext->m_vlcTbl->MidRateInterACDecodeSet.pDeltaRunLast0 =  VC1_MidRateInterDeltaRunLast0;
    pContext->m_vlcTbl->MidRateInterACDecodeSet.pDeltaRunLast1 = VC1_MidRateInterDeltaRunLast1;

    pContext->m_vlcTbl->HighRateInterACDecodeSet.pRLTable = 0;
    pContext->m_vlcTbl->HighRateInterACDecodeSet.pDeltaLevelLast0 = VC1_HighRateInterDeltaLevelLast0;
    pContext->m_vlcTbl->HighRateInterACDecodeSet.pDeltaLevelLast1 = VC1_HighRateInterDeltaLevelLast1;
    pContext->m_vlcTbl->HighRateInterACDecodeSet.pDeltaRunLast0 =  VC1_HighRateInterDeltaRunLast0;
    pContext->m_vlcTbl->HighRateInterACDecodeSet.pDeltaRunLast1 = VC1_HighRateInterDeltaRunLast1;

    pContext->m_vlcTbl->InterACDecodeSetPQINDEXle7[0] = &pContext->m_vlcTbl->HighRateInterACDecodeSet;
    pContext->m_vlcTbl->InterACDecodeSetPQINDEXle7[1] = &pContext->m_vlcTbl->HighMotionInterACDecodeSet;
    pContext->m_vlcTbl->InterACDecodeSetPQINDEXle7[2] = &pContext->m_vlcTbl->MidRateInterACDecodeSet;



    pContext->m_vlcTbl->InterACDecodeSetPQINDEXgt7[0] = &pContext->m_vlcTbl->LowMotionInterACDecodeSet;
    pContext->m_vlcTbl->InterACDecodeSetPQINDEXgt7[1] = &pContext->m_vlcTbl->HighMotionInterACDecodeSet;
    pContext->m_vlcTbl->InterACDecodeSetPQINDEXgt7[2] = &pContext->m_vlcTbl->MidRateInterACDecodeSet;
};

Ipp32s InitCommonTables(VC1Context* pContext)
{
    //MOTION DC DIFF
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowMotionLumaDCDiff, &pContext->m_vlcTbl->m_pLowMotionLumaDCDiff))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighMotionLumaDCDiff, &pContext->m_vlcTbl->m_pHighMotionLumaDCDiff))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowMotionChromaDCDiff, &pContext->m_vlcTbl->m_pLowMotionChromaDCDiff))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighMotionChromaDCDiff, &pContext->m_vlcTbl->m_pHighMotionChromaDCDiff))
        return 0;

    //CBPCY
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_CBPCY_Ipic, &pContext->m_vlcTbl->m_pCBPCY_Ipic))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_CBPCY_PBpic_tbl0,
        &pContext->m_vlcTbl->CBPCY_PB_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_CBPCY_PBpic_tbl1,
        &pContext->m_vlcTbl->CBPCY_PB_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_CBPCY_PBpic_tbl2,
        &pContext->m_vlcTbl->CBPCY_PB_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_CBPCY_PBpic_tbl3,
        &pContext->m_vlcTbl->CBPCY_PB_TABLES[3]))
        return 0;

    //DEDCODE INDEX TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowMotionIntraAC,
        &pContext->m_vlcTbl->LowMotionIntraACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighMotionIntraAC,
        &pContext->m_vlcTbl->HighMotionIntraACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowMotionInterAC,
        &pContext->m_vlcTbl->LowMotionInterACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighMotionInterAC,
        &pContext->m_vlcTbl->HighMotionInterACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MidRateIntraAC,
        &pContext->m_vlcTbl->MidRateIntraACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MidRateInterAC,
        &pContext->m_vlcTbl->MidRateInterACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighRateIntraAC,
        &pContext->m_vlcTbl->HighRateIntraACDecodeSet.pRLTable))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighRateInterAC,
        &pContext->m_vlcTbl->HighRateInterACDecodeSet.pRLTable))
        return 0;

    //BITPLANE
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Bitplane_IMODE_tbl,
        &pContext->m_vlcTbl->m_Bitplane_IMODE))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_BitplaneTaledbitsTbl,
        &pContext->m_vlcTbl->m_BitplaneTaledbits))
        return 0;

    //BFRACTION
    if (ippStsNoErr != ippiHuffmanRunLevelTableInitAlloc_32s(
        VC1_BFraction_tbl,
        &pContext->m_vlcTbl->BFRACTION))
        return 0;

    //MV DIFF PB TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Progressive_MV_Diff_tbl0,
        &pContext->m_vlcTbl->MVDIFF_PB_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Progressive_MV_Diff_tbl1,
        &pContext->m_vlcTbl->MVDIFF_PB_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Progressive_MV_Diff_tbl2,
        &pContext->m_vlcTbl->MVDIFF_PB_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Progressive_MV_Diff_tbl3,
        &pContext->m_vlcTbl->MVDIFF_PB_TABLES[3]))
        return 0;

    //TTMB PB TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowRateTTMB,
        &pContext->m_vlcTbl->TTMB_PB_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MediumRateTTMB,
        &pContext->m_vlcTbl->TTMB_PB_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighRateTTMB,
        &pContext->m_vlcTbl->TTMB_PB_TABLES[2]))
        return 0;

    //TTBLK PB TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowRateTTBLK,
        &pContext->m_vlcTbl->TTBLK_PB_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MediumRateTTBLK,
        &pContext->m_vlcTbl->TTBLK_PB_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighRateTTBLK,
        &pContext->m_vlcTbl->TTBLK_PB_TABLES[2]))
        return 0;

    //SUB BLOCK PATTERN
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_HighRateSBP,
        &pContext->m_vlcTbl->SBP_PB_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MediumRateSBP,
        &pContext->m_vlcTbl->SBP_PB_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_LowRateSBP,
        &pContext->m_vlcTbl->SBP_PB_TABLES[2]))
        return 0;

return 1;
}

Ipp32s InitInterlacedTables (VC1Context* pContext)
{
    //MB MODE
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_4MV_MB_Mode_PBPic_Table0,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_4MV_MB_Mode_PBPic_Table1,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_4MV_MB_Mode_PBPic_Table2,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_4MV_MB_Mode_PBPic_Table3,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[3]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Non4MV_MB_Mode_PBPic_Table0,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[4]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Non4MV_MB_Mode_PBPic_Table1,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[5]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Non4MV_MB_Mode_PBPic_Table2,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[6]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Non4MV_MB_Mode_PBPic_Table3,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[7]))
        return 0;

    //MV TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable0,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable1,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable2,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable3,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[3]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable4,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[4]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable5,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[5]))
        return 0;
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable6,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[6]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable7,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[7]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable8,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[8]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable9,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[9]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable10,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[10]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedMVDifTable11,
        &pContext->m_vlcTbl->MV_INTERLACE_TABLES[11]))
        return 0;

    //CBPCY
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable0,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable1,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable2,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable3,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[3]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable4,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[4]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable5,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[5]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable6,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[6]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_InterlacedCBPCYTable7,
        &pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[7]))
        return 0;

    //2 MV BLOCK PATTERN P,B PICTURES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV2BlockPatternTable0,
        &pContext->m_vlcTbl->MV2BP_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV2BlockPatternTable1,
        &pContext->m_vlcTbl->MV2BP_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV2BlockPatternTable2,
        &pContext->m_vlcTbl->MV2BP_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV2BlockPatternTable3,
        &pContext->m_vlcTbl->MV2BP_TABLES[3]))
        return 0;

    //4 MV BLOCK PATTERN
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV4BlockPatternTable0,
        &pContext->m_vlcTbl->MV4BP_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV4BlockPatternTable1,
        &pContext->m_vlcTbl->MV4BP_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV4BlockPatternTable2,
        &pContext->m_vlcTbl->MV4BP_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_MV4BlockPatternTable3,
        &pContext->m_vlcTbl->MV4BP_TABLES[3]))
        return 0;

    //MB MODE TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable0,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable1,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable2,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable3,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[3]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable4,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[4]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable5,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[5]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable6,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[6]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_1MV_MB_ModeTable7,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[7]))
        return 0;

    //MIXEDMV MB TABLES
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable0,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[0]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable1,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[1]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable2,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[2]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable3,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[3]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable4,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[4]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable5,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[5]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable6,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[6]))
        return 0;

    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_Mixed_MV_MB_ModeTable7,
        &pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[7]))
        return 0;

    //REFDIST
    if (ippStsNoErr != ippiHuffmanTableInitAlloc_32s(
        VC1_FieldRefdistTable,
        &pContext->m_vlcTbl->REFDIST_TABLE))
        return 0;

    return 1;
}

bool InitTables(VC1Context* pContext)
{
    ippStaticInit();

    SetDecodingTables(pContext);
    if(!InitCommonTables(pContext))
        return false;

    if(pContext->m_seqLayerHeader->INTERLACE)
        if(!InitInterlacedTables(pContext))
            return false;
    return true;
}


void FreeTables(VC1Context* pContext)
{
    Ipp32s i;

    if(pContext->m_vlcTbl->m_pLowMotionLumaDCDiff)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_pLowMotionLumaDCDiff);
        pContext->m_vlcTbl->m_pLowMotionLumaDCDiff = NULL;
    }

    if(pContext->m_vlcTbl->m_pHighMotionLumaDCDiff)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_pHighMotionLumaDCDiff);
        pContext->m_vlcTbl->m_pHighMotionLumaDCDiff = NULL;
    }

    if(pContext->m_vlcTbl->m_pLowMotionChromaDCDiff)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_pLowMotionChromaDCDiff);
        pContext->m_vlcTbl->m_pLowMotionChromaDCDiff = NULL;
    }

    if(pContext->m_vlcTbl->m_pHighMotionChromaDCDiff)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_pHighMotionChromaDCDiff);
        pContext->m_vlcTbl->m_pHighMotionChromaDCDiff = NULL;
    }

    if(pContext->m_vlcTbl->m_pCBPCY_Ipic)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_pCBPCY_Ipic);
        pContext->m_vlcTbl->m_pCBPCY_Ipic = NULL;
    }

    if(LowMotionInterACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(LowMotionInterACDecodeSet.pRLTable);
        LowMotionInterACDecodeSet.pRLTable = NULL;
    }

    if(HighMotionInterACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(HighMotionInterACDecodeSet.pRLTable);
        HighMotionInterACDecodeSet.pRLTable = NULL;
    }

    if(LowMotionIntraACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(LowMotionIntraACDecodeSet.pRLTable);
        LowMotionIntraACDecodeSet.pRLTable = NULL;
    }

    if(HighMotionIntraACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(HighMotionIntraACDecodeSet.pRLTable);
        HighMotionIntraACDecodeSet.pRLTable = NULL;
    }


    if(MidRateIntraACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(MidRateIntraACDecodeSet.pRLTable);
        MidRateIntraACDecodeSet.pRLTable = NULL;
    }

    if(MidRateInterACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(MidRateInterACDecodeSet.pRLTable);
        MidRateInterACDecodeSet.pRLTable = NULL;
    }

    if(HighRateIntraACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(HighRateIntraACDecodeSet.pRLTable);
        HighRateIntraACDecodeSet.pRLTable = NULL;
    }

    if(HighRateInterACDecodeSet.pRLTable)
    {
        ippiHuffmanTableFree_32s(HighRateInterACDecodeSet.pRLTable);
        HighRateInterACDecodeSet.pRLTable = NULL;
    }

    if(pContext->m_vlcTbl->m_Bitplane_IMODE)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_Bitplane_IMODE);
        pContext->m_vlcTbl->m_Bitplane_IMODE = NULL;
    }

    if(pContext->m_vlcTbl->m_BitplaneTaledbits)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->m_BitplaneTaledbits);
        pContext->m_vlcTbl->m_BitplaneTaledbits = NULL;
    }

    if(pContext->m_vlcTbl->REFDIST_TABLE)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->REFDIST_TABLE);
        pContext->m_vlcTbl->REFDIST_TABLE = NULL;
    }


    {
        for(i = 0; i < 4; i++)
        {
            if(pContext->m_vlcTbl->MVDIFF_PB_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MVDIFF_PB_TABLES[i]);
                pContext->m_vlcTbl->MVDIFF_PB_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 4; i++)
        {
            if(pContext->m_vlcTbl->CBPCY_PB_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->CBPCY_PB_TABLES[i]);
                pContext->m_vlcTbl->CBPCY_PB_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 3; i++)
        {
            if(pContext->m_vlcTbl->TTMB_PB_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->TTMB_PB_TABLES[i]);
                pContext->m_vlcTbl->TTMB_PB_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 8; i++)
        {
            if(pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[i]);
                pContext->m_vlcTbl->MBMODE_INTERLACE_FRAME_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 4; i++)
        {
            if(pContext->m_vlcTbl->MV2BP_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MV2BP_TABLES[i]);
                pContext->m_vlcTbl->MV2BP_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 3; i++)
        {
            if(pContext->m_vlcTbl->TTBLK_PB_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->TTBLK_PB_TABLES[i]);
                pContext->m_vlcTbl->TTBLK_PB_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 3; i++)
        {
            if(pContext->m_vlcTbl->SBP_PB_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->SBP_PB_TABLES[i]);
                pContext->m_vlcTbl->SBP_PB_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 4; i++)
        {
            if(pContext->m_vlcTbl->MV4BP_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MV4BP_TABLES[i]);
                pContext->m_vlcTbl->MV4BP_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 8; i++)
        {
            if(pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[i]);
                pContext->m_vlcTbl->CBPCY_PB_INTERLACE_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 8; i++)
        {
            if(pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[i]);
                pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 8; i++)
        {
            if(pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[i]);
                pContext->m_vlcTbl->MBMODE_INTERLACE_FIELD_MIXED_TABLES[i] = NULL;
            }
        }
    }

    {
        for(i = 0; i < 12; i++)
        {
            if(pContext->m_vlcTbl->MV_INTERLACE_TABLES[i])
            {
                ippiHuffmanTableFree_32s(pContext->m_vlcTbl->MV_INTERLACE_TABLES[i]);
                pContext->m_vlcTbl->MV_INTERLACE_TABLES[i] = 0;
            }
        }
    }

    if(pContext->m_vlcTbl->BFRACTION)
    {
        ippiHuffmanTableFree_32s(pContext->m_vlcTbl->BFRACTION);
        pContext->m_vlcTbl->BFRACTION = NULL;
    }
}

Ipp32u DecodeBegin(VC1Context* pContext, Ipp32u stream_subtype)
{
    VM_ASSERT(pContext != NULL);
    Ipp32u width = 0;
    Ipp32u height = 0;
    Ipp32s temp_value;
    Ipp32s seq_size = 0;
    Ipp8u* seqStart = NULL;
    //!!!!!!!!!
    Ipp32u frame_size = 1;
    Ipp8u* ptr = NULL;
    Ipp32u StartCode = 0;

    if ((stream_subtype == VC1_VIDEO_VC1)||
        (stream_subtype == WVC1_VIDEO))
    {
        //read start code
        VC1_GET_BITS(32,temp_value);

        //StartCode = *((Ipp32u*)(pContext->m_pBufferStart));
        StartCode = ((*(pContext->m_pBufferStart+3))<<24) + ((*(pContext->m_pBufferStart+2))<<16) +
                    ((*(pContext->m_pBufferStart+1))<<8) + *(pContext->m_pBufferStart);

        // advance profile
        pContext->m_seqLayerHeader->PROFILE = 3;

        if(StartCode != (VC1_SequenceHeader|0x00000100))
        {
            return frame_size;
        }

        SequenceLayer(pContext);

        width  = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1)*2;
        height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1)*2;

        pContext->m_seqLayerHeader->widthMB = (Ipp16u)((width+15)/VC1_PIXEL_IN_LUMA);
        pContext->m_seqLayerHeader->heightMB = (Ipp16u)((height+15)/VC1_PIXEL_IN_LUMA);

        if ((pContext->m_seqLayerHeader->INTERLACE)&&
            (pContext->m_seqLayerHeader->heightMB & 1))
        {
            pContext->m_seqLayerHeader->heightMB += 1;
            pContext->m_seqLayerHeader->IsResize = 1;
        }

        ptr = (Ipp8u*)pContext->m_bitstream.pBitstream;
        //StartCode = (*((Ipp32u*)(ptr)))&0xFFFFFF00;

        StartCode = ((*(ptr+3))<<24) + ((*(ptr+2))<<16) + ((*(ptr+1))<<8) + *(ptr);
        StartCode &= 0xFFFFFF00;

        while((StartCode != 0x00000100) && (ptr < pContext->m_pBufferStart + pContext->m_FrameSize))
        {
            ptr++;
            //StartCode = (*((Ipp32u*)(ptr)))&0xFFFFFF00;
            StartCode = ((*(ptr+3))<<24) + ((*(ptr+2))<<16) + ((*(ptr+1))<<8) + *(ptr);
            StartCode &= 0xFFFFFF00;
        }
        pContext->m_FrameSize = (Ipp32u)(ptr - pContext->m_pBufferStart);
    }
    else
    {
        //read sequence header size
        SwapData(pContext->m_pBufferStart, pContext->m_FrameSize);

        if (stream_subtype != WMV3_VIDEO)
        {
            seqStart = pContext->m_pBufferStart + 4;
            seq_size  = ((*(seqStart+3))<<24) + ((*(seqStart+2))<<16) + ((*(seqStart+1))<<8) + *(seqStart);

            assert(seq_size > 0);
            assert(seq_size < 100);

            seqStart = pContext->m_pBufferStart + 8 + seq_size;
            height  = ((*(seqStart+3))<<24) + ((*(seqStart+2))<<16) + ((*(seqStart+1))<<8) + *(seqStart);
            seqStart+=4;
            width  = ((*(seqStart+3))<<24) + ((*(seqStart+2))<<16) + ((*(seqStart+1))<<8) + *(seqStart);

            VC1_GET_BITS(32,temp_value); //!!!!!!!
            VC1_GET_BITS(32,temp_value); //!!!!!
            pContext->m_seqLayerHeader->widthMB  = (Ipp16u)((width+15)/VC1_PIXEL_IN_LUMA);
            pContext->m_seqLayerHeader->heightMB = (Ipp16u)((height+15)/VC1_PIXEL_IN_LUMA);
            pContext->m_seqLayerHeader->MAX_CODED_HEIGHT = height/2 - 1;
            pContext->m_seqLayerHeader->MAX_CODED_WIDTH = width/2 - 1;
        }
        SwapData(pContext->m_pBufferStart, pContext->m_FrameSize);
        SequenceLayer(pContext);
    }
    if (stream_subtype == WMV3_VIDEO)
        return 1;

    height = pContext->m_seqLayerHeader->heightMB*VC1_PIXEL_IN_LUMA;
    width = (pContext->m_seqLayerHeader->widthMB * VC1_PIXEL_IN_LUMA);

    frame_size = (height + 128)*(width + 128)
                + ((height / 2 + 64)*(width / 2 + 64))*2;

    return frame_size; //!!!!!
}

VC1Status GetNextPicHeader_Adv(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;

    memset(pContext->m_picLayerHeader, 0, sizeof(VC1PictureLayerHeader));

    vc1Sts = DecodePictureHeader_Adv(pContext);

    return vc1Sts;
}

VC1Status GetNextPicHeader(VC1Context* pContext, bool isExtHeader)
{
    VC1Status vc1Sts = VC1_OK;

    memset(pContext->m_picLayerHeader, 0, sizeof(VC1PictureLayerHeader));

    vc1Sts = DecodePictureHeader(pContext, isExtHeader);

    return vc1Sts;
}

VC1Status DecodeFrame (VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;

    vc1Sts = Decode_PictureLayer(pContext);

    return vc1Sts;
}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
