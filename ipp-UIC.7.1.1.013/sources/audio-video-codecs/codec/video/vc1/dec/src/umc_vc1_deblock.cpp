/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, deblocking
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_debug.h"

typedef enum
{
    LFilterSimplePFrame    = 0,
    LFilterMainPFrame      = 1,
    LFilterAdvancePFrame   = 3, /* 2 - VC1_PROFILE_RESERVED */
    LFilterMainPInterFrame = 4
} LFilterType;

static const Ipp32s deblock_table[16] = {IPPVC_EDGE_ALL ,
                                      IPPVC_EDGE_QUARTER_1 +  IPPVC_EDGE_HALF_2,
                                      IPPVC_EDGE_QUARTER_2 +  IPPVC_EDGE_HALF_2,
                                      IPPVC_EDGE_HALF_2,

                                      IPPVC_EDGE_HALF_1 + IPPVC_EDGE_QUARTER_3,
                                      IPPVC_EDGE_QUARTER_1 +  IPPVC_EDGE_QUARTER_3,
                                      IPPVC_EDGE_QUARTER_2 + IPPVC_EDGE_QUARTER_3,
                                      IPPVC_EDGE_QUARTER_3,

                                      IPPVC_EDGE_HALF_1+ IPPVC_EDGE_QUARTER_4,
                                      IPPVC_EDGE_QUARTER_1+ IPPVC_EDGE_QUARTER_4,
                                      IPPVC_EDGE_QUARTER_2+ IPPVC_EDGE_QUARTER_4,
                                      IPPVC_EDGE_QUARTER_4,

                                      IPPVC_EDGE_HALF_1,
                                      IPPVC_EDGE_QUARTER_1,
                                      IPPVC_EDGE_QUARTER_2,
                                      0};


//static const Ipp32s deblock_table_2[4] = {IPPVC_EDGE_HALF_1,
//                                       IPPVC_EDGE_QUARTER_1,
//                                       IPPVC_EDGE_QUARTER_2,
//                                       0};

static const Ipp32s deblock_table_2[4] = {IPPVC_EDGE_ALL,
                                          IPPVC_EDGE_HALF_1,
                                          IPPVC_EDGE_HALF_2,
                                          0};

static void HorizontalDeblockingLumaP(Ipp8u* pUUpBlock,
                                      Ipp32u Pquant,
                                      Ipp32s Pitch,
                                      const VC1MB* _pMB,
                                      const VC1MB* _pnMB,
                                      LFilterType _type)
{
    Ipp32s SBP = 0;

    Ipp32s count = 0;
    Ipp32s flag_ext = 0;
    Ipp32s flag_int = 0;

    Ipp32s Edge_ext = 3; /* Mark bottom left and right subblocks */
    Ipp32s Edge_int = 0;

    Ipp32s SBP_cur_count  = 0;
    Ipp32s SBP_next_count = 2;
    if (_pMB != _pnMB)
    {
        SBP_next_count = 0;
        SBP_cur_count  = 2;
    }
    for(count = 0; count<2;count++)
    {
        Edge_ext = 3;
        Edge_int = 0;
        SBP = _pMB->m_pBlocks[SBP_cur_count].SBlkPattern;
        if (_pnMB)
        {
            if ((LFilterMainPInterFrame != _type)
                &&(_pMB->m_pBlocks[SBP_cur_count].blkType < VC1_BLK_INTRA_TOP)
                &&(_pnMB->m_pBlocks[SBP_next_count].blkType < VC1_BLK_INTRA_TOP)
                &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][0] == _pnMB->m_pBlocks[SBP_next_count].mv[0][0])
                &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][1] == _pnMB->m_pBlocks[SBP_next_count].mv[0][1])
                )
            {
                Ipp32s DSBP = _pnMB->m_pBlocks[SBP_next_count].SBlkPattern;
                Ipp32s TSBP = SBP;
                Edge_ext = TSBP | (DSBP>>2);
                if (LFilterMainPFrame == _type)
                {
                    /* Historical fix for 4x4 blocks */
                    if (_pMB->m_pBlocks[SBP_cur_count].blkType==VC1_BLK_INTER4X4 && TSBP)
                        TSBP = 0xF;
                    if (_pnMB->m_pBlocks[SBP_next_count].blkType==VC1_BLK_INTER4X4 && DSBP)
                        DSBP = 0xF;
                }
                Edge_ext = TSBP | (DSBP>>2);
            }
            flag_ext += ((Edge_ext&3) << (count*2));
        }
        else
            flag_ext = 0;
        if ((_pMB->m_pBlocks[SBP_cur_count].blkType==VC1_BLK_INTER8X4) || (_pMB->m_pBlocks[SBP_cur_count].blkType==VC1_BLK_INTER4X4))
        {
            if ((SBP & 8) || (SBP & 2))
                Edge_int |= 2;
            if ((SBP & 4) || (SBP & 1))
                Edge_int |= 1;
        }
        flag_int += (Edge_int << (count*2));
        ++SBP_next_count;
        ++SBP_cur_count;
    }
    ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,Pitch, deblock_table[flag_ext]);
    ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, deblock_table[flag_int]);
}

static void HorizontalDeblockingChromaP(Ipp8u* pUUpBlock,
                                      Ipp32u Pquant,
                                      Ipp32s Pitch,
                                      VC1Block* _pBlk,
                                      VC1Block* _pnBlk,
                                      LFilterType _type)
{

    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s Edge_int = 0;
    Ipp32s Edge_ext = 3;
    if (_pnBlk)
    {
        if ((LFilterMainPInterFrame != _type)
            &&(_pBlk->blkType < VC1_BLK_INTRA_TOP)
            &&(_pnBlk->blkType < VC1_BLK_INTRA_TOP)
            &&(_pBlk->mv[0][0] == _pnBlk->mv[0][0])
            &&(_pBlk->mv[0][1] == _pnBlk->mv[0][1])
            )
        {
            Ipp32s DSBP = _pnBlk->SBlkPattern;
            Ipp32s TSBP = SBP;
            if (LFilterMainPFrame == _type)
            {
                /* Historical fix for 4x4 blocks */
                if (_pBlk->blkType==VC1_BLK_INTER4X4 && TSBP)
                    TSBP = 0xF;
                if (_pnBlk->blkType==VC1_BLK_INTER4X4 && DSBP)
                    DSBP = 0xF;
            }
            Edge_ext = TSBP | (DSBP>>2);
        }
    }
    else
        Edge_ext  = 0;
    if ((_pBlk->blkType==VC1_BLK_INTER8X4) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 2))
            Edge_int |= 2;
        if ((SBP & 4) || (SBP & 1))
            Edge_int |= 1;
    }

    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,Pitch, deblock_table_2[Edge_ext&3]);
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, deblock_table_2[Edge_int]);
}

static void VerticalDeblockingLumaP(Ipp8u* pUUpLBlock,
                                    Ipp32u Pquant,
                                    Ipp32s Pitch,
                                    VC1MB* _pMB,
                                    const VC1MB* _pnMB,
                                    LFilterType _type)
{
    Ipp32s SBP = 0;
    Ipp32s LSBP = 0;
    VC1Block* pL   = 0;


    Ipp32s count = 0;
    Ipp32s flag_ext = 0;
    Ipp32s flag_int = 0;

    Ipp32s Edge_ext = 3; /* Mark bottom left and right subblocks */
    Ipp32s Edge_int = 0;

    Ipp32s SBP_cur_count  = 0;
    Ipp32s SBP_next_count = 1;
    if (_pMB != _pnMB)
    {
        SBP_next_count = 0;
        SBP_cur_count  = 1;
    }

    for(count = 0; count<2;count++)
    {
        pL   = &_pMB->m_pBlocks[SBP_cur_count];
        Edge_ext = 3;
        Edge_int = 0;
        SBP = _pMB->m_pBlocks[SBP_cur_count].SBlkPattern;
        LSBP = SBP;
        if (_pnMB)
        {
            if (2 == SBP_cur_count)
            {
                pL   = &_pMB->m_pBlocks[SBP_cur_count-1];
                LSBP = pL->SBlkPattern;
            }
            if ((LFilterMainPInterFrame != _type)
                &&(_pMB->m_pBlocks[SBP_cur_count].blkType  < VC1_BLK_INTRA_TOP)
                &&(_pnMB->m_pBlocks[SBP_next_count].blkType < VC1_BLK_INTRA_TOP)
                &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][0] == _pnMB->m_pBlocks[SBP_next_count].mv[0][0])
                &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][1] == _pnMB->m_pBlocks[SBP_next_count].mv[0][1])
                )
            {
                Ipp32s RSBP = _pnMB->m_pBlocks[SBP_next_count].SBlkPattern;
                if (LFilterMainPFrame == _type)
                {
                    /* Historical reasons of VC-1 implementation */
                    /* Historical fix for 4x4 blocks */
                    if (pL->blkType==VC1_BLK_INTER4X4 && LSBP)
                        LSBP = 0xF;
                    if (_pnMB->m_pBlocks[SBP_next_count].blkType==VC1_BLK_INTER4X4 && RSBP)
                        RSBP = 0xF;
                }
                Edge_ext = LSBP | (RSBP>>1);
                Edge_ext = ( ((Edge_ext&4)>>1) + (Edge_ext&1));
            }
            flag_ext += ((Edge_ext&3) << (count*2));
        }
        else
            flag_ext = 0;
        if ((_pMB->m_pBlocks[SBP_cur_count].blkType==VC1_BLK_INTER4X8) || (_pMB->m_pBlocks[SBP_cur_count].blkType==VC1_BLK_INTER4X4))
        {
            if ((SBP & 8) || (SBP & 4))
                Edge_int |= 2;
            if ((SBP & 2) || (SBP & 1))
                Edge_int |= 1;
        }
        flag_int += (Edge_int << (count*2));
        SBP_next_count +=2;
        SBP_cur_count +=2;
    }
    ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 8, Pquant,Pitch, deblock_table[flag_ext]);
    ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch, deblock_table[flag_int]);
}

static void VerticalDeblockingChromaP(Ipp8u* pUUpLBlock,
                                      Ipp32u Pquant,
                                      Ipp32s Pitch,
                                      VC1Block* _pBlk,
                                      VC1Block* _pnBlk,
                                      LFilterType _type)
{
    Ipp32s SBP =  _pBlk->SBlkPattern;
    Ipp32s LSBP = SBP;
    VC1Block* pL   = _pBlk;

    Ipp32s Edge_ext = 3; /* Mark bottom left and right subblocks */
    Ipp32s Edge_int = 0;


        if (_pnBlk)
        {
            if ((LFilterMainPInterFrame != _type)
                &&(_pBlk->blkType < VC1_BLK_INTRA_TOP)
                &&(_pnBlk->blkType < VC1_BLK_INTRA_TOP)
                &&(_pBlk->mv[0][0] == _pnBlk->mv[0][0])
                &&(_pBlk->mv[0][1] == _pnBlk->mv[0][1])
            )
            {
                Ipp32s RSBP = _pnBlk->SBlkPattern;
                if (LFilterMainPFrame == _type)
                {
                    /* Historical reasons of VC-1 implementation */
                    /* Historical fix for 4x4 blocks */
                    if (pL->blkType==VC1_BLK_INTER4X4 && LSBP)
                        LSBP = 0xF;
                    if (_pnBlk->blkType==VC1_BLK_INTER4X4 && RSBP)
                        RSBP = 0xF;
                }
                Edge_ext = LSBP | (RSBP>>1);
                Edge_ext = ( ((Edge_ext&4)>>1) + (Edge_ext&1));
            }
        }
        else
            Edge_ext  = 0;

        if ((_pBlk->blkType==VC1_BLK_INTER4X8) || (_pBlk->blkType==VC1_BLK_INTER4X4))
        {
            if ((SBP & 8) || (SBP & 4))
                Edge_int |= 2;
            if ((SBP & 2) || (SBP & 1))
                Edge_int |= 1;
        }
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 8, Pquant,Pitch, deblock_table_2[Edge_ext]);
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch, deblock_table_2[Edge_int]);
}

void Deblocking_ProgressiveIpicture(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s curX, curY;
    Ipp32u PQuant = pContext->m_picLayerHeader->PQUANT;

    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    VC1MB* m_CurrMB = pContext->m_pCurrMB;

    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;



    Ipp32s flag_ver = 0;

    /* Deblock horizontal edges */
    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            flag_ver = 0;
            /* Top luma blocks */
            ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+8*YPitch), PQuant, YPitch,flag_ver);

            /* Bottom Luma blocks */
            ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+YPitch*16),
                                              PQuant,YPitch,flag_ver);

            /* Horizontal deblock Cb */
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR((m_CurrMB->currUPlane+8*UPitch),
                                                PQuant,UPitch,flag_ver);
            /* Horizontal deblock Cr */
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR((m_CurrMB->currVPlane+8*VPitch),
                                                PQuant,VPitch,flag_ver);

            ++m_CurrMB;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
    for (curX=0; curX<WidthMB; curX++)
    {
        flag_ver =0;
        /* Top luma blocks */
        ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+8*YPitch), PQuant, YPitch,flag_ver);
        ++m_CurrMB;
    }

    } else
        HeightMB -= 1;

    m_CurrMB   -= WidthMB*HeightMB;

    /* Deblock vertical edges */
    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            flag_ver =0;
            ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+8), PQuant,YPitch,flag_ver);

            /* Bottom Luma blocks */
            ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+16),PQuant,YPitch,flag_ver);
            /* Vertical deblock Cb */
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currUPlane+8,PQuant,UPitch,flag_ver);
            /* Vertical deblock Cr */
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currVPlane+8,PQuant,VPitch,flag_ver);
            ++m_CurrMB;
         }
        flag_ver =0;
        ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+8), PQuant,YPitch,flag_ver);
        ++m_CurrMB;
    }


}

void Deblocking_ProgressivePpicture(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    Ipp32s curX, curY;
    Ipp32s PQuant = pContext->m_picLayerHeader->PQUANT;
    VC1MB* m_CurrMB = pContext->m_pCurrMB;

    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;

    LFilterType type = (LFilterType)(pContext->m_seqLayerHeader->PROFILE);
    LFilterType type_current = type;
    LFilterType type_current_next = type;

    if (LFilterMainPFrame == type)
    {
        if ((pContext->m_MBs[0].mbType == VC1_MB_INTRA) || (pContext->m_MBs[0].m_pBlocks[0].blkType > VC1_BLK_INTER))
        {
            type              = LFilterMainPInterFrame;
            type_current      = LFilterMainPInterFrame;
            type_current_next = LFilterMainPInterFrame;
        }
    }

    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV_BBL, VM_STRING("MB number is %d MB type %d\n"), WidthMB*curY+curX, m_CurrMB->mbType);
            if ((m_CurrMB->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
                type_current =  LFilterMainPFrame;

            HorizontalDeblockingLumaP(m_CurrMB->currYPlane,PQuant,YPitch,m_CurrMB,m_CurrMB,type_current);

            if (((m_CurrMB+WidthMB)->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
                type_current_next =  LFilterMainPFrame;

            HorizontalDeblockingLumaP(m_CurrMB->currYPlane+YPitch*8,PQuant,YPitch,m_CurrMB,m_CurrMB+WidthMB,type_current_next);


            HorizontalDeblockingChromaP(m_CurrMB->currUPlane,
                                        PQuant,
                                        UPitch,
                                        &m_CurrMB->m_pBlocks[4],
                                        &(m_CurrMB+WidthMB)->m_pBlocks[4],
                                        type_current_next);

            HorizontalDeblockingChromaP(m_CurrMB->currVPlane,
                                         PQuant,
                                         VPitch,
                                         &m_CurrMB->m_pBlocks[5],
                                         &(m_CurrMB+WidthMB)->m_pBlocks[5],
                                         type_current_next);

            type_current      = type;
            type_current_next = type;
            m_CurrMB++;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
        for (curX=0; curX<WidthMB; curX++)
    {
        if ((m_CurrMB->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
            type_current =  LFilterMainPFrame;

        HorizontalDeblockingLumaP(m_CurrMB->currYPlane,PQuant,YPitch,m_CurrMB,m_CurrMB,type_current);
        HorizontalDeblockingLumaP(m_CurrMB->currYPlane+YPitch*8,PQuant,YPitch,m_CurrMB,NULL,type_current_next);

        HorizontalDeblockingChromaP(m_CurrMB->currUPlane,
            PQuant,
            UPitch,
            &m_CurrMB->m_pBlocks[4],
            NULL,
            type_current_next);

        HorizontalDeblockingChromaP(m_CurrMB->currVPlane,
            PQuant,
            VPitch,
            &m_CurrMB->m_pBlocks[5],
            NULL,
            type_current_next);
        type_current      = type;
        type_current_next = type;
        m_CurrMB++;
    }

    } else
        HeightMB -=1;

    m_CurrMB -= WidthMB*HeightMB;


    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV_BBL, VM_STRING("MB number is %d MB type %d\n"), WidthMB*curY+curX, m_CurrMB->mbType);
            if ((m_CurrMB->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
                type_current =  LFilterMainPFrame;

            VerticalDeblockingLumaP(m_CurrMB->currYPlane,
                PQuant,
                YPitch,
                m_CurrMB,
                m_CurrMB,
                type_current);


            if (((m_CurrMB+1)->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
                type_current_next =  LFilterMainPFrame;

            VerticalDeblockingLumaP((m_CurrMB->currYPlane+8),
                PQuant,
                YPitch,
                m_CurrMB,
                m_CurrMB+1,
                type_current_next);

            VerticalDeblockingChromaP(m_CurrMB->currUPlane,
                PQuant,
                UPitch,
                &m_CurrMB->m_pBlocks[4],
                &(m_CurrMB+1)->m_pBlocks[4],
                type_current_next);

            VerticalDeblockingChromaP(m_CurrMB->currVPlane,
                PQuant,
                VPitch,
                &m_CurrMB->m_pBlocks[5],
                &(m_CurrMB+1)->m_pBlocks[5],
                type_current_next);

            type_current      = type;
            type_current_next = type;
            ++m_CurrMB;

        }

        if ((m_CurrMB->mbType&VC1_MB_4MV_INTER) == VC1_MB_4MV_INTER)
            type_current =  LFilterMainPFrame;

        VerticalDeblockingLumaP(m_CurrMB->currYPlane,
            PQuant,
            YPitch,
            m_CurrMB,
            m_CurrMB,
            type_current);
         VerticalDeblockingLumaP((m_CurrMB->currYPlane+8),
            PQuant,
            YPitch,
            m_CurrMB,
            NULL,
            type_current_next);
        VerticalDeblockingChromaP((m_CurrMB->currUPlane),
            PQuant,
            UPitch,
            &m_CurrMB->m_pBlocks[4],
            NULL,
            type_current_next);
        VerticalDeblockingChromaP((m_CurrMB->currVPlane),
            PQuant,VPitch,
            &m_CurrMB->m_pBlocks[5],
            NULL,
            type_current_next);
        type_current      = type;
        type_current_next = type;
        ++m_CurrMB;
    }

}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
