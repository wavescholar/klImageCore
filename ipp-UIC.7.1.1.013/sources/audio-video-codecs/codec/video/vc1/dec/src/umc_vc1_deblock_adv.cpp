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

static const Ipp8u deblock_table[16] = {IPPVC_EDGE_ALL ,
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

static const Ipp8u deblock_table_2[4] = {IPPVC_EDGE_ALL,
                                         IPPVC_EDGE_HALF_1,
                                         IPPVC_EDGE_HALF_2,
                                         0};


inline static void HorizontalDeblockingLumaP(Ipp8u* pUUpBlock,
                                      Ipp32u Pquant,
                                      Ipp32s Pitch,
                                      const VC1MB* _pMB,
                                      const VC1MB* _pnMB)
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

        if ((_pMB->m_pBlocks[SBP_cur_count].blkType < VC1_BLK_INTRA_TOP)
            &&(_pnMB->m_pBlocks[SBP_next_count].blkType < VC1_BLK_INTRA_TOP)
            &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][0] == _pnMB->m_pBlocks[SBP_next_count].mv[0][0])
            &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][1] == _pnMB->m_pBlocks[SBP_next_count].mv[0][1])
            &&(_pMB->m_pBlocks[SBP_cur_count].mv_s_polarity[0] == _pnMB->m_pBlocks[SBP_next_count].mv_s_polarity[0])
            )
        {
            Ipp32s DSBP = _pnMB->m_pBlocks[SBP_next_count].SBlkPattern;
            Ipp32s TSBP = SBP;
            Edge_ext = TSBP | (DSBP>>2);
        }
        flag_ext += ((Edge_ext&3) << (count*2));
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



inline static void HorizontalDeblockingLumaPNoBotBlock(Ipp8u* pUUpBlock,
                                                Ipp32u Pquant,
                                                Ipp32s Pitch,
                                                const VC1MB* _pMB)
{
    Ipp32s count = 0;
    Ipp32s flag = 0;
    Ipp32s Edge = 0;
    Ipp32s SBP = 0;
    for(count = 0; count<2;count++)
    {
        Edge = 0;
        SBP = _pMB->m_pBlocks[count+2].SBlkPattern;
        if ((_pMB->m_pBlocks[count+2].blkType==VC1_BLK_INTER8X4) || (_pMB->m_pBlocks[count+2].blkType==VC1_BLK_INTER4X4))
        {
            if ((SBP & 8) || (SBP & 2))
                Edge |= 2;
            if ((SBP & 4) || (SBP & 1))
                Edge |= 1;
        }
        flag += (Edge << (count*2));
    }
    ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, deblock_table[flag]);
}



inline static void HorizontalDeblockingChromaP(Ipp8u* pUUpBlock,
                                        Ipp32u Pquant,
                                        Ipp32s Pitch,
                                        VC1Block* _pBlk,
                                        VC1Block* _pnBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s Edge_int = 0;
    Ipp32s Edge_ext = 3;

    if ((_pBlk->blkType < VC1_BLK_INTRA_TOP)
        &&(_pnBlk->blkType < VC1_BLK_INTRA_TOP)
        &&(_pBlk->mv[0][0] == _pnBlk->mv[0][0])
        &&(_pBlk->mv[0][1] == _pnBlk->mv[0][1])
        &&(_pBlk->mv_s_polarity[0] == _pnBlk->mv_s_polarity[0])
        )
    {
        Ipp32s DSBP = _pnBlk->SBlkPattern;
        Ipp32s TSBP = SBP;
        Edge_ext = TSBP | (DSBP>>2);
    }

    if ((_pBlk->blkType==VC1_BLK_INTER8X4) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 2))
        {
            Edge_int |= 2;
        }
        if ((SBP & 4) || (SBP & 1))
        {
            Edge_int |= 1;
        }
    }
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,Pitch, deblock_table_2[Edge_ext&3]);
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, deblock_table_2[Edge_int]);

}

inline static void HorizontalDeblockingChromaPNoBotBlock(Ipp8u* pUUpBlock,
                                                  Ipp32u Pquant,
                                                  Ipp32s Pitch,
                                                  const  VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s Edge_int = 0;


    if ((_pBlk->blkType==VC1_BLK_INTER8X4) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 2))
        {
            Edge_int |= 2;
        }
        if ((SBP & 4) || (SBP & 1))
        {
            Edge_int |= 1;
        }
    }
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, deblock_table_2[Edge_int]);
}

inline static void VerticalDeblockingLumaP(Ipp8u* pUUpLBlock,
                                    Ipp32u Pquant,
                                    Ipp32s Pitch,
                                    const VC1MB* _pMB,
                                    const VC1MB* _pnMB)
{
    Ipp32s SBP = 0;

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
        Edge_ext = 3;
        Edge_int = 0;
        SBP = _pMB->m_pBlocks[SBP_cur_count].SBlkPattern;

        if ((_pMB->m_pBlocks[SBP_cur_count].blkType < VC1_BLK_INTRA_TOP)
            &&(_pnMB->m_pBlocks[SBP_next_count].blkType < VC1_BLK_INTRA_TOP)
            &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][0] == _pnMB->m_pBlocks[SBP_next_count].mv[0][0])
            &&(_pMB->m_pBlocks[SBP_cur_count].mv[0][1] == _pnMB->m_pBlocks[SBP_next_count].mv[0][1])
            &&(_pMB->m_pBlocks[SBP_cur_count].mv_s_polarity[0] == _pnMB->m_pBlocks[SBP_next_count].mv_s_polarity[0])
            )
        {
            Ipp32s DSBP = _pnMB->m_pBlocks[SBP_next_count].SBlkPattern;
            Ipp32s TSBP = SBP;
            Edge_ext = TSBP | (DSBP>>1);
            Edge_ext = ( ((Edge_ext&4)>>1) + (Edge_ext&1));
        }
        flag_ext += (Edge_ext << (count*2));
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
inline static void VerticalDeblockingLumaPNoLeftBlock(Ipp8u* pUUpLBlock,
                                               Ipp32u Pquant,
                                               Ipp32s Pitch,
                                               const VC1MB* _pMB)
{
    Ipp32s count = 0;
    Ipp32s flag = 0;
    Ipp32s Edge = 0;
    Ipp32s SBP = 0;
    Ipp32s SBP_counter = 1;


   for(count = 0; count<2;count++)
    {
        Edge = 0;
        SBP = _pMB->m_pBlocks[SBP_counter].SBlkPattern;
        if ((_pMB->m_pBlocks[SBP_counter].blkType==VC1_BLK_INTER4X8) ||
            (_pMB->m_pBlocks[SBP_counter].blkType==VC1_BLK_INTER4X4))
        {
            if ((SBP & 8) || (SBP & 4))
                Edge |= 2;
            if ((SBP & 2) || (SBP & 1))
                Edge |= 1;
        }
        flag += (Edge << (count*2));
        SBP_counter += 2;
    }
    ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch, deblock_table[flag]);

}



inline static void VerticalDeblockingChromaP(Ipp8u* pUUpLBlock,
                                      Ipp32u Pquant,
                                      Ipp32s Pitch,
                                      VC1Block* _pBlk,
                                      VC1Block* _pnBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s Edge_int = 0;
    Ipp32s Edge_ext = 3;

        if ((_pBlk->blkType < VC1_BLK_INTRA_TOP)
        &&(_pnBlk->blkType < VC1_BLK_INTRA_TOP)
        &&(_pBlk->mv[0][0] == _pnBlk->mv[0][0])
        &&(_pBlk->mv[0][1] == _pnBlk->mv[0][1])
        &&(_pBlk->mv_s_polarity[0] == _pnBlk->mv_s_polarity[0])
        )
    {
            Ipp32s DSBP = _pnBlk->SBlkPattern;
            Ipp32s TSBP = SBP;
            Edge_ext = TSBP | (DSBP>>1);
            Edge_ext = ( ((Edge_ext&4)>>1) + (Edge_ext&1));
    }

    if ((_pBlk->blkType==VC1_BLK_INTER4X8) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 4))
        {
            Edge_int |= 2;
        }
        if ((SBP & 2) || (SBP & 1))
        {
            Edge_int |= 1;
        }
    }
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 8, Pquant,Pitch, deblock_table_2[Edge_ext]);
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch, deblock_table_2[Edge_int]);
}
inline static void VerticalDeblockingChromaPNoLeftBlock(Ipp8u* pUUpLBlock,
                                                 Ipp32u Pquant,
                                                 Ipp32s Pitch,
                                                 VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s Edge_int = 0;

    if ((_pBlk->blkType==VC1_BLK_INTER4X8) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 4))
        {
            Edge_int |= 2;
        }
        if ((SBP & 2) || (SBP & 1))
        {
            Edge_int |= 1;
        }
    }
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch, deblock_table_2[Edge_int]);
}



inline static void HorizontalDeblockingBlkB_No_DBlock(Ipp8u* pUUpBlock,
                                               Ipp32u Pquant,
                                               Ipp32s Pitch,
                                               VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s flag_ver = IPPVC_EDGE_ALL;
    if ((_pBlk->blkType==VC1_BLK_INTER8X4) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 2))
        {
            flag_ver ^= IPPVC_EDGE_HALF_1;
        }
        if ((SBP & 4) || (SBP & 1))
        {
            flag_ver ^= IPPVC_EDGE_HALF_2;
        }
    }
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, flag_ver);
}

inline static void HorizontalDeblockingBlkB(Ipp8u* pUUpBlock,
                                     Ipp32u Pquant,
                                     Ipp32s Pitch,
                                     VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s flag_ver = IPPVC_EDGE_ALL;
    //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV_BBL,VM_STRING("Edge = %d\n"),Edge);

    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,Pitch, 0);
    if ((_pBlk->blkType==VC1_BLK_INTER8X4) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 2))
        {
            flag_ver ^= IPPVC_EDGE_HALF_1;
        }
        if ((SBP & 4) || (SBP & 1))
        {
            flag_ver ^= IPPVC_EDGE_HALF_2;
        }
    }
    ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,Pitch, flag_ver);

}
inline static void VerticalDeblockingBlkB_No_DBlock(Ipp8u* pUUpLBlock,
                                             Ipp32u Pquant,
                                             Ipp32s Pitch,
                                             VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s flag_ver = IPPVC_EDGE_ALL;
    if ((_pBlk->blkType==VC1_BLK_INTER4X8) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 4))
        {
            flag_ver ^= IPPVC_EDGE_HALF_1;


        }
        if ((SBP & 2) || (SBP & 1))
        {
            flag_ver ^= IPPVC_EDGE_HALF_2;
        }
    }
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch,flag_ver);
}

inline static void VerticalDeblockingBlkB(Ipp8u* pUUpLBlock,
                                   Ipp32u Pquant,
                                   Ipp32s Pitch,
                                   VC1Block* _pBlk)
{
    Ipp32s SBP = _pBlk->SBlkPattern;
    Ipp32s flag_ver = IPPVC_EDGE_ALL;
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 8, Pquant,Pitch,0);
    if ((_pBlk->blkType==VC1_BLK_INTER4X8) || (_pBlk->blkType==VC1_BLK_INTER4X4))
    {
        if ((SBP & 8) || (SBP & 4))
        {
            /* Deblock first 4 pixels of subblock edge */
            flag_ver ^= IPPVC_EDGE_HALF_1;
        }
        if ((SBP & 2) || (SBP & 1))
        {
            /* Deblock second 4 pixels of subblock edge */
            flag_ver ^= IPPVC_EDGE_HALF_2;
        }
    }
    ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpLBlock + 4, Pquant,Pitch,flag_ver);
}



inline static void HorizontalDeblockingBlkInterlaceP_No_DBlock(Ipp8u* pUUpBlock,
                                                        Ipp32u Pquant,
                                                        Ipp32s Pitch,
                                                        VC1Block* _pBlk,
                                                        Ipp32u field)
{
    Ipp32s flag_ver=0;
    if (field)
    {
        if ((_pBlk->blkType == VC1_BLK_INTER8X4) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,2*Pitch, flag_ver);
        }

    }
}
inline static void HorizontalDeblockingBlkInterlaceP(Ipp8u* pUUpBlock,
                                              Ipp32u Pquant,
                                              Ipp32s Pitch,
                                              VC1Block* _pBlk,
                                              Ipp32u field,
                                              Ipp32u is_intern_deblock)
{
    if (field)
    {
        if ((_pBlk->blkType == VC1_BLK_INTER8X4) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,2*Pitch, 0);

        }
        ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 16*Pitch, Pquant,2*Pitch, 0);
    }
    else
    {
        if (is_intern_deblock&&((_pBlk->blkType == VC1_BLK_INTER8X4) ||
            (_pBlk->blkType == VC1_BLK_INTER4X4)))
        {
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 4*Pitch, Pquant,2*Pitch, 0);
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 5*Pitch, Pquant,2*Pitch, 0);
        }
        ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 8*Pitch, Pquant,2*Pitch, 0);
        ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pUUpBlock + 9*Pitch, Pquant,2*Pitch, 0);
    }

}
inline static void VerticalDeblockingBlkInterlaceP_No_DBlock(Ipp8u* pUUpBlock,
                                                      Ipp32u Pquant,
                                                      Ipp32s Pitch,
                                                      VC1Block* _pBlk,
                                                      Ipp32u field)
{
    Ipp32s flag_ver = IPPVC_EDGE_HALF_2;
    if (field)
    {
        if ((_pBlk->blkType == VC1_BLK_INTER4X8) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4, Pquant,2*Pitch,flag_ver);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4 +8*Pitch, Pquant,2*Pitch,flag_ver);

        }
    }
    else
    {
        if ((_pBlk->blkType == VC1_BLK_INTER4X8) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4, Pquant,2*Pitch,flag_ver);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4 + Pitch, Pquant,2*Pitch,flag_ver);
        }
    }
}
inline static void VerticalDeblockingBlkInterlaceP(Ipp8u* pUUpBlock,
                                            Ipp32u Pquant,
                                            Ipp32s Pitch,
                                            VC1Block* _pBlk,
                                            Ipp32u field)
{
    Ipp32s flag_ver = IPPVC_EDGE_HALF_2;
    if (field)
    {
        if ((_pBlk->blkType == VC1_BLK_INTER4X8) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock+ 4, Pquant,2*Pitch,flag_ver);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4 +8*Pitch, Pquant,2*Pitch,flag_ver);

        }
        ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 8, Pquant,2*Pitch,flag_ver);
        ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 8 +8*Pitch, Pquant,2*Pitch,flag_ver);

    }

    else
    {
        if ((_pBlk->blkType == VC1_BLK_INTER4X8) || (_pBlk->blkType == VC1_BLK_INTER4X4))
        {
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4, Pquant,2*Pitch,flag_ver);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 4 + Pitch, Pquant,2*Pitch,flag_ver);
        }
        ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 8, Pquant,2*Pitch,flag_ver);
        ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pUUpBlock + 8 + Pitch, Pquant,2*Pitch,flag_ver);
    }

}


void Deblocking_ProgressiveIpicture_Adv(VC1Context* pContext)
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
            //flag_ver = IPPVC_EDGE_HALF_2;
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
        flag_ver =0;
        for (curX=0; curX<WidthMB; curX++)
        {
            /* Top luma blocks */
            ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR((m_CurrMB->currYPlane+8*YPitch), PQuant, YPitch,flag_ver);
            ++m_CurrMB;
        }
    } else
        HeightMB -=1;


    m_CurrMB   -= WidthMB*HeightMB;
    /* Deblock vertical edges */
    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            flag_ver = 0;
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
void Deblocking_ProgressivePpicture_Adv(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s curX, curY;
    Ipp32s PQuant = pContext->m_picLayerHeader->PQUANT;

    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    //VC1MB* m_CurrMB = pContext->m_pCurrMB - WidthMB*(HeightMB);
    VC1MB* m_CurrMB = pContext->m_pCurrMB;
    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;

    /* Deblock horizontal edges */
    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            /* Top luma blocks */
            HorizontalDeblockingLumaP(m_CurrMB->currYPlane,
                                        PQuant,YPitch,
                                        m_CurrMB,
                                        m_CurrMB);

            /* Bottom Luma blocks */
            HorizontalDeblockingLumaP((m_CurrMB->currYPlane+YPitch*8),
                                         PQuant,
                                         YPitch,
                                         m_CurrMB,
                                         m_CurrMB+WidthMB);
            /* Horizontal deblock Cb */
            HorizontalDeblockingChromaP((m_CurrMB->currUPlane),
                                         PQuant,
                                         UPitch,
                                         &m_CurrMB->m_pBlocks[4],
                                         &(m_CurrMB+WidthMB)->m_pBlocks[4]);
            /* Horizontal deblock Cr */
            HorizontalDeblockingChromaP((m_CurrMB->currVPlane),
                                         PQuant,
                                         VPitch,
                                         &m_CurrMB->m_pBlocks[5],
                                         &(m_CurrMB+WidthMB)->m_pBlocks[5]);
            ++m_CurrMB;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
    for (curX=0; curX<WidthMB; curX++)
    {

        /* Top luma blocks */
        HorizontalDeblockingLumaP((m_CurrMB->currYPlane),
                                   PQuant,
                                   YPitch,
                                   m_CurrMB,
                                   m_CurrMB);

        HorizontalDeblockingLumaPNoBotBlock((m_CurrMB->currYPlane+YPitch*8),
                                             PQuant,
                                             YPitch,
                                             m_CurrMB);
        /* Horizontal deblock Cb */
        HorizontalDeblockingChromaPNoBotBlock((m_CurrMB->currUPlane),
                                               PQuant,
                                               UPitch,
                                               &m_CurrMB->m_pBlocks[4]);
        /* Horizontal deblock Cr */
        HorizontalDeblockingChromaPNoBotBlock((m_CurrMB->currVPlane),
                                               PQuant,
                                               VPitch,
                                               &m_CurrMB->m_pBlocks[5]);
        ++m_CurrMB;
    }
    } else
        HeightMB -=1;

    m_CurrMB   -= WidthMB*(HeightMB);

    /* Deblock vertical edges */
    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            /* Left luma blocks */
            VerticalDeblockingLumaP(m_CurrMB->currYPlane,
                                    PQuant,
                                    YPitch,
                                    m_CurrMB,
                                    m_CurrMB);
            /* Right Luma blocks */
             VerticalDeblockingLumaP(m_CurrMB->currYPlane+8,
                                    PQuant,
                                    YPitch,
                                    m_CurrMB,
                                    m_CurrMB+1);
            /* Vertical deblock Cb */
            VerticalDeblockingChromaP((m_CurrMB->currUPlane),
                PQuant,
                UPitch,
                &m_CurrMB->m_pBlocks[4],
                &(m_CurrMB+1)->m_pBlocks[4]);
            /* Vertical deblock Cr */
           VerticalDeblockingChromaP((m_CurrMB->currVPlane),
                PQuant,
                VPitch,
                &m_CurrMB->m_pBlocks[5],
                &(m_CurrMB+1)->m_pBlocks[5]);

            ++m_CurrMB;

        }
        /* Left luma blocks */
        VerticalDeblockingLumaP(m_CurrMB->currYPlane,
                                PQuant,
                                YPitch,
                                m_CurrMB,
                                m_CurrMB);

        /* Right Luma blocks */
        VerticalDeblockingLumaPNoLeftBlock(m_CurrMB->currYPlane+8,
                                           PQuant,
                                           YPitch,
                                           m_CurrMB);
        /* Vertical deblock Cb */
        VerticalDeblockingChromaPNoLeftBlock(m_CurrMB->currUPlane,
                                             PQuant,
                                             UPitch,
                                             &m_CurrMB->m_pBlocks[4]);
        /* Vertical deblock Cr */
        VerticalDeblockingChromaPNoLeftBlock(m_CurrMB->currVPlane,
                                             PQuant,VPitch,
                                             &m_CurrMB->m_pBlocks[5]);
        ++m_CurrMB;
    }
}



void Deblocking_InterlaceFieldBpicture_Adv(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s curX, curY;
    Ipp32s PQuant = pContext->m_picLayerHeader->PQUANT;

    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    VC1MB* m_CurrMB = pContext->m_pCurrMB;

    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;

    /* Deblock horizontal edges */
    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            /* Top luma blocks */
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane),
                                      PQuant,YPitch,
                                      &m_CurrMB->m_pBlocks[0]);
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane+8),
                                      PQuant,YPitch,
                                      &m_CurrMB->m_pBlocks[1]);

            /* Bottom Luma blocks */
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane+YPitch*8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[2]);
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane+YPitch*8+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[3]);
            /* Horizontal deblock Cb */
            HorizontalDeblockingBlkB((m_CurrMB->currUPlane),
                PQuant,
                UPitch,
                &m_CurrMB->m_pBlocks[4]);
            /* Horizontal deblock Cr */
            HorizontalDeblockingBlkB((m_CurrMB->currVPlane),
                PQuant,
                VPitch,
                &m_CurrMB->m_pBlocks[5]);
            ++m_CurrMB;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            /* Top luma blocks */
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[0]);
            HorizontalDeblockingBlkB((m_CurrMB->currYPlane+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[1]);
            HorizontalDeblockingBlkB_No_DBlock((m_CurrMB->currYPlane+YPitch*8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[2]);
            HorizontalDeblockingBlkB_No_DBlock((m_CurrMB->currYPlane+YPitch*8+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[3]);
            /* Horizontal deblock Cb */
            HorizontalDeblockingBlkB_No_DBlock((m_CurrMB->currUPlane),
                PQuant,
                UPitch,
                &m_CurrMB->m_pBlocks[4]);
            /* Horizontal deblock Cr */
            HorizontalDeblockingBlkB_No_DBlock((m_CurrMB->currVPlane),
                PQuant,
                VPitch,
                &m_CurrMB->m_pBlocks[5]);
            ++m_CurrMB;
        }


    }
    else
        HeightMB -=1;

    m_CurrMB   -= WidthMB*(HeightMB);


    /* Deblock vertical edges */
    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            /* Top luma blocks */
            VerticalDeblockingBlkB((m_CurrMB->currYPlane),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[0]);
            VerticalDeblockingBlkB((m_CurrMB->currYPlane+YPitch*8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[2]);
            /* Bottom Luma blocks */
            VerticalDeblockingBlkB((m_CurrMB->currYPlane+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[1]);
            VerticalDeblockingBlkB((m_CurrMB->currYPlane+YPitch*8+8),
                PQuant,YPitch,
                &m_CurrMB->m_pBlocks[3]);
            /* Vertical deblock Cb */
            VerticalDeblockingBlkB((m_CurrMB->currUPlane),
                PQuant,
                UPitch,
                &m_CurrMB->m_pBlocks[4]);
            /* Vertical deblock Cr */
            VerticalDeblockingBlkB((m_CurrMB->currVPlane),
                PQuant,
                VPitch,
                &m_CurrMB->m_pBlocks[5]);
            ++m_CurrMB;

        }

        /* Top luma blocks */
        VerticalDeblockingBlkB((m_CurrMB->currYPlane),
            PQuant,
            YPitch,
            &m_CurrMB->m_pBlocks[0]);
        VerticalDeblockingBlkB((m_CurrMB->currYPlane+YPitch*8),
            PQuant,
            YPitch,
            &m_CurrMB->m_pBlocks[2]);
        /* Bottom Luma blocks */
        VerticalDeblockingBlkB_No_DBlock((m_CurrMB->currYPlane+8),
            PQuant,
            YPitch,
            &m_CurrMB->m_pBlocks[1]);
        VerticalDeblockingBlkB_No_DBlock((m_CurrMB->currYPlane+YPitch*8+8),
            PQuant,
            YPitch,
            &m_CurrMB->m_pBlocks[3]);
        /* Vertical deblock Cb */
        VerticalDeblockingBlkB_No_DBlock((m_CurrMB->currUPlane),
            PQuant,
            UPitch,
            &m_CurrMB->m_pBlocks[4]);
        /* Vertical deblock Cr */
        VerticalDeblockingBlkB_No_DBlock((m_CurrMB->currVPlane),
            PQuant,VPitch,
            &m_CurrMB->m_pBlocks[5]);
        ++m_CurrMB;
    }
}


void Deblocking_InterlaceFrameIpicture_Adv(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s curX, curY;
    Ipp32u PQuant = pContext->m_picLayerHeader->PQUANT;

    Ipp32s fieldoffset;

    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    VC1MB* m_CurrMB = pContext->m_pCurrMB;
    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;

    fieldoffset = m_CurrMB->FIELDTX;

    /* Deblock horizontal edges */
    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            /* Top luma blocks */
            if (m_CurrMB->FIELDTX)
            {
                ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 16*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
                /* Bottom Luma blocks */
                ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 17*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
            }
            else
            {
               ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 8*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
               ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 9*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);

                /* Bottom Luma blocks */

                ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 16*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
                ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 17*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
            }

            /* Horizontal deblock Cb */
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(m_CurrMB->currUPlane + 8*UPitch,
                                                 PQuant,
                                                 2*UPitch,
                                                 0);
            ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(m_CurrMB->currUPlane + 9*UPitch,
                                                 PQuant,
                                                 2*UPitch,
                                                 0);

              /* Horizontal deblock Cr */
             ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(m_CurrMB->currVPlane + 8*VPitch,
                                                 PQuant,
                                                 2*VPitch,
                                                 0);
             ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(m_CurrMB->currVPlane + 9*VPitch,
                                                 PQuant,
                                                 2*VPitch,
                                                 0);
            ++m_CurrMB;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
        for (curX=0; curX<WidthMB; curX++)
        {
            if (!m_CurrMB->FIELDTX)
            {

               ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 8*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
               ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 9*YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 0);
            }

           ++m_CurrMB;
        }
    }
    else
        HeightMB -=1;



     m_CurrMB -= WidthMB*(HeightMB);




    /* Deblock vertical edges */
    for (curY=0; curY<HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            if (m_CurrMB->FIELDTX)
            {
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+16,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 8 + YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane + 16 + YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);

            }
           else
            {
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8 + YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+16,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+16 + YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);

               }



            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currUPlane+8,
                                                 PQuant,
                                                 2*UPitch,
                                                 IPPVC_EDGE_HALF_2);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currUPlane+8+UPitch,
                                                 PQuant,
                                                 2*UPitch,
                                                 IPPVC_EDGE_HALF_2);


            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currVPlane+8,
                                                 PQuant,
                                                 2*VPitch,
                                                 IPPVC_EDGE_HALF_2);
            ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(m_CurrMB->currVPlane+8+VPitch,
                                                 PQuant,
                                                 2*VPitch,
                                                 IPPVC_EDGE_HALF_2);
            ++m_CurrMB;
        }

            if (m_CurrMB->FIELDTX)
            {
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
                ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8+YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
            }
            else
            {
               ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8 + YPitch,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
               ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR(m_CurrMB->currYPlane+8,
                                                 PQuant,
                                                 2*YPitch,
                                                 IPPVC_EDGE_HALF_2);
             }
            ++m_CurrMB;
    }
}


void Deblocking_InterlaceFramePpicture_Adv(VC1Context* pContext)
{
    Ipp32s WidthMB =  pContext->m_seqLayerHeader->widthMB;
    Ipp32s curX, curY;
    Ipp32u PQuant = pContext->m_picLayerHeader->PQUANT;


    Ipp32s fieldoffset[] = {8,1};

    Ipp32s HeightMB   = pContext->DeblockInfo.HeightMB;
    VC1MB* m_CurrMB = pContext->m_pCurrMB;
    Ipp32s YPitch = m_CurrMB->currYPitch;
    Ipp32s UPitch = m_CurrMB->currUPitch;
    Ipp32s VPitch = m_CurrMB->currVPitch;

    Ipp32u is_intern_deblock = 0;

    /* Deblock horizontal edges */
    for (curY=0; curY<HeightMB-1; curY++)
    {
        for (curX=0; curX<WidthMB; curX++)
        {

            if (VC1_IS_NO_TOP_MB(m_CurrMB->LeftTopRightPositionFlag))
                is_intern_deblock = 1;
            else
                is_intern_deblock = 0;

            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV, VM_STRING("m_CurrMB->FIELDTX %d\n"), m_CurrMB->FIELDTX);


            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[0],
                m_CurrMB->FIELDTX,
                is_intern_deblock);
            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[1],
                m_CurrMB->FIELDTX,
                is_intern_deblock);

            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[2],
                m_CurrMB->FIELDTX,
                1);
            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                PQuant,
                YPitch,
                &m_CurrMB->m_pBlocks[3],
                m_CurrMB->FIELDTX,
                1);

            /* Horizontal deblock Cb */
            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currUPlane),
                                               PQuant,
                                               UPitch,
                                               &m_CurrMB->m_pBlocks[4],
                                               0,is_intern_deblock);

              /* Horizontal deblock Cr */
            HorizontalDeblockingBlkInterlaceP((m_CurrMB->currVPlane),
                                               PQuant,
                                               VPitch,
                                               &m_CurrMB->m_pBlocks[5],
                                               0,is_intern_deblock);
            ++m_CurrMB;
        }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
        for (curX=0; curX<WidthMB; curX++)
        {

            if (VC1_IS_NO_TOP_MB(m_CurrMB->LeftTopRightPositionFlag))
                is_intern_deblock = 1;
            else
                is_intern_deblock = 0;

            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV, VM_STRING("m_CurrMB->FIELDTX %d\n"), m_CurrMB->FIELDTX);




            if (!m_CurrMB->FIELDTX)
            {
                HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                                                PQuant,
                                                YPitch,
                                                &m_CurrMB->m_pBlocks[0],
                                                 m_CurrMB->FIELDTX,
                                                 is_intern_deblock);

                HorizontalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+8),
                                                PQuant,
                                                YPitch,
                                                &m_CurrMB->m_pBlocks[1],
                                                m_CurrMB->FIELDTX,
                                                is_intern_deblock);
            } else
            {
                HorizontalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane),
                                                PQuant,
                                                YPitch,
                                                &m_CurrMB->m_pBlocks[0],
                                                 m_CurrMB->FIELDTX);

                HorizontalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+8),
                                                PQuant,
                                                YPitch,
                                                &m_CurrMB->m_pBlocks[1],
                                                 m_CurrMB->FIELDTX);
            }

            HorizontalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                                               PQuant,
                                               YPitch,
                                               &m_CurrMB->m_pBlocks[2],
                                               m_CurrMB->FIELDTX);
            HorizontalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                                                PQuant,
                                                YPitch,
                                                &m_CurrMB->m_pBlocks[3],
                                                m_CurrMB->FIELDTX);
           ++m_CurrMB;
        }


    }
    else
    {
        HeightMB -=1;
    }



    m_CurrMB -= WidthMB*(HeightMB);

    if (VC1_IS_NO_TOP_MB(m_CurrMB->LeftTopRightPositionFlag))
    {

        m_CurrMB -= WidthMB*(pContext->iPrevDblkStartPos);


    /* Deblock vertical edges */
    for (curY=0; curY<pContext->iPrevDblkStartPos; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[0],
                                             m_CurrMB->FIELDTX);
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[2],
                                             m_CurrMB->FIELDTX);

            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+8),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[1],
                                             m_CurrMB->FIELDTX);
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[3],
                                              m_CurrMB->FIELDTX);


            /* Vertical deblock Cb */
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currUPlane),
                                             PQuant,
                                             UPitch,
                                             &m_CurrMB->m_pBlocks[4],
                                             0);
            /* Vertical deblock Cr */
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currVPlane),
                                             PQuant,
                                             VPitch,
                                             &m_CurrMB->m_pBlocks[5],
                                             0);
            ++m_CurrMB;
        }

        VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                                         PQuant,
                                         YPitch,
                                         &m_CurrMB->m_pBlocks[0],
                                          m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                                         PQuant,
                                         YPitch,
                                         &m_CurrMB->m_pBlocks[2],
                                         m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+8),
                                                   PQuant,
                                                   YPitch,
                                                   &m_CurrMB->m_pBlocks[1],
                                                   m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                                                   PQuant,
                                                   YPitch,
                                                   &m_CurrMB->m_pBlocks[3],
                                                    m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currUPlane),
                                                   PQuant,
                                                   UPitch,
                                                   &m_CurrMB->m_pBlocks[4],
                                                   0);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currVPlane),
                                                   PQuant,
                                                   VPitch,
                                                   &m_CurrMB->m_pBlocks[5],
                                                   0);
        ++m_CurrMB;
    }
    }
    if (pContext->DeblockInfo.is_last_deblock)
    {
        for (curY=0; curY < HeightMB; curY++)
    {
        for (curX=0; curX<WidthMB-1; curX++)
        {

            //VM_Debug::GetInstance().vm_debug_frame(-1,VC1_MV, VM_STRING("m_CurrMB->FIELDTX %d\n"), m_CurrMB->FIELDTX);


            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[0],
                                             m_CurrMB->FIELDTX);
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[2],
                                             m_CurrMB->FIELDTX);

            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+8),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[1],
                                             m_CurrMB->FIELDTX);
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                                             PQuant,
                                             YPitch,
                                             &m_CurrMB->m_pBlocks[3],
                                              m_CurrMB->FIELDTX);


            /* Vertical deblock Cb */
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currUPlane),
                                             PQuant,
                                             UPitch,
                                             &m_CurrMB->m_pBlocks[4],
                                             0);
            /* Vertical deblock Cr */
            VerticalDeblockingBlkInterlaceP((m_CurrMB->currVPlane),
                                             PQuant,
                                             VPitch,
                                             &m_CurrMB->m_pBlocks[5],
                                             0);
            ++m_CurrMB;
        }

        VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane),
                                         PQuant,
                                         YPitch,
                                         &m_CurrMB->m_pBlocks[0],
                                          m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]),
                                         PQuant,
                                         YPitch,
                                         &m_CurrMB->m_pBlocks[2],
                                         m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+8),
                                                   PQuant,
                                                   YPitch,
                                                   &m_CurrMB->m_pBlocks[1],
                                                   m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currYPlane+YPitch*fieldoffset[m_CurrMB->FIELDTX]+8),
                                                   PQuant,
                                                   YPitch,
                                                   &m_CurrMB->m_pBlocks[3],
                                                    m_CurrMB->FIELDTX);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currUPlane),
                                                   PQuant,
                                                   UPitch,
                                                   &m_CurrMB->m_pBlocks[4],
                                                   0);
        VerticalDeblockingBlkInterlaceP_No_DBlock((m_CurrMB->currVPlane),
                                                   PQuant,
                                                   VPitch,
                                                   &m_CurrMB->m_pBlocks[5],
                                                   0);
        ++m_CurrMB;
    }
    }


}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
