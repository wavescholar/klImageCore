/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_segment_decoder.h"
#include "umc_h264_dec_deblocking.h"

namespace UMC
{

#define IsVectorDifferenceBig(one, two, iMaxDiff) \
    /* the original code is \
    ((4 <= abs(one.mvx - two.mvx)) || \
     (iMaxDiff <= abs(one.mvy - two.mvy))); \
    but the current code is equal and uses less comparisons */ \
    ((7 <= (Ipp32u)((one).mvx - (two).mvx + 3)) || \
     ((Ipp32u)(iMaxDiff * 2 - 1) <= (Ipp32u)((one).mvy - (two).mvy + iMaxDiff - 1)))

void H264SegmentDecoder::PrepareDeblockingParametersISlice()
{
    // set deblocking flag(s)
    m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = 1;
    m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] = 1;

    // calculate strengths
    if (m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        // deblocking with strong deblocking of external edge
        SetEdgeStrength(m_deblockingParams.Strength[VERTICAL_DEBLOCKING] + 0, 4);
    }

    SetEdgeStrength(m_deblockingParams.Strength[VERTICAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(m_deblockingParams.Strength[VERTICAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(m_deblockingParams.Strength[VERTICAL_DEBLOCKING] + 12, 3);

    if (m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
    {
        if (m_deblockingParams.MBFieldCoded)
        {
            // deblocking field macroblock with external edge
            SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING] + 0, 3);
        }
        else
        {
            // deblocking with strong deblocking of external edge
            SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING] + 0, 4);
        }
    }

    SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING] + 12, 3);

} // void H264SegmentDecoder::PrepareDeblockingParametersISlice()

void H264SegmentDecoder::PrepareDeblockingParametersPSlice()
{
    Ipp32s mbtype = m_cur_mb.GlobalMacroblockInfo->mbtype;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        PrepareDeblockingParametersISlice();
        return;
    }

    // try simplest function to prepare deblocking parameters
    switch (mbtype)
    {
        // when macroblock has type inter 16 on 16
    case MBTYPE_INTER:
    case MBTYPE_FORWARD:
    case MBTYPE_BACKWARD:
    case MBTYPE_BIDIR:
        PrepareDeblockingParametersPSlice16x16Vert();
        PrepareDeblockingParametersPSlice16x16Horz();
        break;
/*
        // when macroblock has type inter 16 on 8
    case MBTYPE_INTER_16x8:
        PrepareDeblockingParametersPSlice8x16(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersPSlice16x8(HORIZONTAL_DEBLOCKING);
        return;

        // when macroblock has type inter 8 on 16
    case MBTYPE_INTER_8x16:
        PrepareDeblockingParametersPSlice16x8(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersPSlice8x16(HORIZONTAL_DEBLOCKING);
        return;
*/
    default:
        PrepareDeblockingParametersPSlice4(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersPSlice4(HORIZONTAL_DEBLOCKING);
        break;
    }

    if (m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] && !(*((Ipp32u *)(m_deblockingParams.Strength[VERTICAL_DEBLOCKING]))))
    {
        m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
    }

    if (m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] && !(*((Ipp32u *)(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING]))))
    {
        m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersPSlice()

void H264SegmentDecoder::PrepareDeblockingParametersBSlice()
{
    Ipp32s mbtype = m_cur_mb.GlobalMacroblockInfo->mbtype;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        PrepareDeblockingParametersISlice();
        return;
    }

    // try simplest function to prepare deblocking parameters
    switch (mbtype)
    {
        // when macroblock has type inter 16 on 16
    case MBTYPE_INTER:
    case MBTYPE_FORWARD:
    case MBTYPE_BACKWARD:
    case MBTYPE_BIDIR:
        PrepareDeblockingParametersBSlice16x16Vert();
        PrepareDeblockingParametersBSlice16x16Horz();
        break;

        // when macroblock has type inter 16 on 8
    case MBTYPE_INTER_16x8:
        PrepareDeblockingParametersBSlice8x16(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersBSlice16x8(HORIZONTAL_DEBLOCKING);
        return;

        // when macroblock has type inter 8 on 16
    case MBTYPE_INTER_8x16:
        PrepareDeblockingParametersBSlice16x8(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersBSlice8x16(HORIZONTAL_DEBLOCKING);
        return;

    default:
        PrepareDeblockingParametersBSlice4(VERTICAL_DEBLOCKING);
        PrepareDeblockingParametersBSlice4(HORIZONTAL_DEBLOCKING);
        break;
    }

    if (m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] && !(*((Ipp32u *)(m_deblockingParams.Strength[VERTICAL_DEBLOCKING]))))
    {
        m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
    }

    if (m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] && !(*((Ipp32u *)(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING]))))
    {
        m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
    }

    /*    Ipp32u *pStrength = (Ipp32u*)m_deblockingParams.Strength[VERTICAL_DEBLOCKING];

    if (m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] != (pStrength[0] || pStrength[1] || pStrength[2] || pStrength[3]))
        printf("");

    m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = pStrength[0] || pStrength[1] || pStrength[2] || pStrength[3];

    pStrength = (Ipp32u*)m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING];

    if (m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] != (pStrength[0] || pStrength[1] || pStrength[2] || pStrength[3]))
        printf("");

    m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] = pStrength[0] || pStrength[1] || pStrength[2] || pStrength[3];
*/
} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice()

enum
{
    VERTICAL_OUTER_EDGE_MASK    = 0x00a0a,

    VERTICAL_OUTER_EDGE_BLOCK_0 = 0x00002,
    VERTICAL_OUTER_EDGE_BLOCK_1 = 0x00008,
    VERTICAL_OUTER_EDGE_BLOCK_2 = 0x00200,
    VERTICAL_OUTER_EDGE_BLOCK_3 = 0x00800,

    HORIZONTAL_OUTER_EDGE_MASK  = 0x00066,

    HORIZONTAL_OUTER_EDGE_BLOCK_0 = 0x00002,
    HORIZONTAL_OUTER_EDGE_BLOCK_1 = 0x00004,
    HORIZONTAL_OUTER_EDGE_BLOCK_2 = 0x00020,
    HORIZONTAL_OUTER_EDGE_BLOCK_3 = 0x00040
};

static
Ipp8u InternalBlockDeblockingTable[2][16][4] =
{
    // strength arrays for vertical deblocking
    {
        {0, 0, 0, 0},
        {2, 0, 0, 0},
        {0, 0, 2, 0},
        {2, 0, 2, 0},
        {0, 2, 0, 0},
        {2, 2, 0, 0},
        {0, 2, 2, 0},
        {2, 2, 2, 0},
        {0, 0, 0, 2},
        {2, 0, 0, 2},
        {0, 0, 2, 2},
        {2, 0, 2, 2},
        {0, 2, 0, 2},
        {2, 2, 0, 2},
        {0, 2, 2, 2},
        {2, 2, 2, 2}
    },

    // strength arrays for horizontal deblocking
    {
        {0, 0, 0, 0},
        {2, 0, 0, 0},
        {0, 2, 0, 0},
        {2, 2, 0, 0},
        {0, 0, 2, 0},
        {2, 0, 2, 0},
        {0, 2, 2, 0},
        {2, 2, 2, 0},
        {0, 0, 0, 2},
        {2, 0, 0, 2},
        {0, 2, 0, 2},
        {2, 2, 0, 2},
        {0, 0, 2, 2},
        {2, 0, 2, 2},
        {0, 2, 2, 2},
        {2, 2, 2, 2}
    }
};

void H264SegmentDecoder::PrepareDeblockingParametersIntern16x16Vert()
{
    //foo_internal(pParams, VERTICAL_DEBLOCKING);
    //return;

    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[VERTICAL_DEBLOCKING];

    //
    // internal edge(s)
    //
    if (cbp4x4_luma & 0x1fffe)
    {
        Ipp32s a, b, res;

        // set deblocking flag
        m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = 1;

        // deblocking strength depends on CBP only
        // we use fast CBP comparison
        a = cbp4x4_luma >> 1;
        b = cbp4x4_luma >> 2;
        res = (a | b) & 0x505;
        res = (res & 0x05) | (res >> 7);
        CopyEdgeStrength(pStrength + 4, InternalBlockDeblockingTable[VERTICAL_DEBLOCKING][res]);

        a = cbp4x4_luma >> 5;
        res = (a | b) & 0x505;
        res = (res & 0x05) | (res >> 7);
        CopyEdgeStrength(pStrength + 8, InternalBlockDeblockingTable[VERTICAL_DEBLOCKING][res]);

        b = cbp4x4_luma >> 6;
        res = (a | b) & 0x505;
        res = (res & 0x05) | (res >> 7);
        CopyEdgeStrength(pStrength + 12, InternalBlockDeblockingTable[VERTICAL_DEBLOCKING][res]);
    }
    else
    {
        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersIntern16x16Vert()

void H264SegmentDecoder::PrepareDeblockingParametersIntern16x16Horz()
{
    //foo_internal(pParams, HORIZONTAL_DEBLOCKING);
    //return;
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING];

    //
    // internal edge(s)
    //
    if (cbp4x4_luma & 0x1fffe)
    {
        Ipp32s a, b, res;

        // set deblocking flag
        m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] |= 1;

        // deblocking strength depends on CBP only
        // we use fast CBP comparison
        a = cbp4x4_luma >> 1;
        b = cbp4x4_luma >> 3;
        res = (a | b) & 0x33;
        res = (res & 0x03) | (res >> 2);
        CopyEdgeStrength(pStrength + 4, InternalBlockDeblockingTable[HORIZONTAL_DEBLOCKING][res]);

        a = cbp4x4_luma >> 9;
        res = (a | b) & 0x33;
        res = (res & 0x03) | (res >> 2);
        CopyEdgeStrength(pStrength + 8, InternalBlockDeblockingTable[HORIZONTAL_DEBLOCKING][res]);

        b = cbp4x4_luma >> 11;
        res = (a | b) & 0x33;
        res = (res & 0x03) | (res >> 2);
        CopyEdgeStrength(pStrength + 12, InternalBlockDeblockingTable[HORIZONTAL_DEBLOCKING][res]);
    }
    else
    {
        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersIntern16x16Horz()

inline void H264SegmentDecoder::foo_external_p(Ipp32s cbp4x4_luma, Ipp32s nNeighbour, Ipp32s dir,
    Ipp32s idx,
    H264DecoderFrame **pNRefPicList0,
    ReferenceFlags *pNFields0)
{
    Ipp32s blkQ, blkP;
    Ipp32s blkQ2, blkP2;

    H264DecoderMacroblockLocalInfo *pNeighbour = &m_mbinfo.mbs[nNeighbour];
    Ipp32s nBlock, nNeighbourBlock;

    blkQ = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
    blkP = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

    blkQ2 = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx + 1];
    blkP2 = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx + 1];

    bool is_first = (cbp4x4_luma & blkQ) ||
        (pNeighbour->cbp4x4_luma & blkP);

    bool is_second = (cbp4x4_luma & blkQ2) ||
        (pNeighbour->cbp4x4_luma & blkP2);

    if (is_first && is_second)
    {
        m_deblockingParams.Strength[dir][idx] = 2;
        m_deblockingParams.Strength[dir][idx + 1] = 2;
        m_deblockingParams.DeblockingFlag[dir] = 1;
        return;
    }

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = idx * 4;
        nNeighbourBlock = nBlock + 3;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx + 12;
    }

    size_t iRefQFrw, iRefPFrw;

    // field coded image
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        Ipp32s index;

        // select reference index for current block
        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));

        // select reference index for previous block
        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
        iRefPFrw = pNRefPicList0[index]->DeblockPicID(GetReferenceField(pNFields0, index));
    }
    // frame coded image
    else
    {
        Ipp32s index;

        // select reference index for current block
        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(0);

        // select reference index for previous block
        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
        iRefPFrw = pNRefPicList0[index]->DeblockPicID(0);
    }

    // when reference indexes are equal
    if (iRefQFrw == iRefPFrw)
    {
        if (!is_first)
        {
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);

            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector))
            {
                m_deblockingParams.Strength[dir][idx] = 1;
                m_deblockingParams.DeblockingFlag[dir] = 1;
            }
            else
            {
                m_deblockingParams.Strength[dir][idx] = 0;
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }

        idx++;

        // when one of couple of blocks has coeffs
        if (!is_second)
        {
            // calc block and neighbour block number
            if (VERTICAL_DEBLOCKING == dir)
            {
                nBlock = idx * 4;
                nNeighbourBlock = nBlock + 3;
            }
            else
            {
                nBlock = idx;
                nNeighbourBlock = idx + 12;
            }

            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);

            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector))
            {
                m_deblockingParams.Strength[dir][idx] = 1;
                m_deblockingParams.DeblockingFlag[dir] = 1;
            }
            // when forward and backward reference pictures of previous block are equal
            else
            {
                m_deblockingParams.Strength[dir][idx] = 0;
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }
    }
    // when reference indexes are different
    else
    {
        m_deblockingParams.Strength[dir][idx] = (Ipp8u) (1 + (is_first ? 1 : 0));
        m_deblockingParams.Strength[dir][idx + 1] = (Ipp8u) (1 + (is_second ? 1 : 0));
        m_deblockingParams.DeblockingFlag[dir] = 1;
    }
}

inline void H264SegmentDecoder::foo_internal_p(Ipp32s dir,
    Ipp32s idx, Ipp32u cbp4x4_luma)
{
    size_t iRefQFrw, iRefPFrw;

    Ipp32s nBlock, nNeighbourBlock;

    Ipp32s blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];
    Ipp32s blkQ2 = INTERNAL_BLOCKS_MASK[dir][idx - 3];

    if ((cbp4x4_luma & blkQ) && (cbp4x4_luma & blkQ2))
    {
        m_deblockingParams.Strength[dir][idx] = 2;
        m_deblockingParams.Strength[dir][idx + 1] = 2;
        m_deblockingParams.DeblockingFlag[dir] = 1;
        return;
    }

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = (idx & 3) * 4 + (idx >> 2);
        nNeighbourBlock = nBlock - 1;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx - 4;
    }

    // field coded image
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        Ipp32s index;

        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));
        index = m_cur_mb.GetReferenceIndex(0, nNeighbourBlock);
        iRefPFrw = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));
    }
    else  // frame coded image
    {
        Ipp32s index;

        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(0);
        index = m_cur_mb.GetReferenceIndex(0, nNeighbourBlock);
        iRefPFrw = m_pRefPicList[0][index]->DeblockPicID(0);
    }

    // when reference indexes are equal
    if (iRefQFrw == iRefPFrw)
    {
        if (!(cbp4x4_luma & blkQ))
        {
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);

            // compare motion vectors
            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector))
            {
                m_deblockingParams.Strength[dir][idx] = 1;
                m_deblockingParams.DeblockingFlag[dir] = 1;
            }
            else
            {
                m_deblockingParams.Strength[dir][idx] = 0;
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }

        if (!(cbp4x4_luma & blkQ2))
        {
            idx++;
            // calc block and neighbour block number
            if (VERTICAL_DEBLOCKING == dir)
            {
                nBlock = (idx & 3) * 4 + (idx >> 2);
                nNeighbourBlock = nBlock - 1;
            }
            else
            {
                nBlock = idx;
                nNeighbourBlock = idx - 4;
            }

            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);

            // compare motion vectors
            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector))
            {
                m_deblockingParams.Strength[dir][idx] = 1;
                m_deblockingParams.DeblockingFlag[dir] = 1;
            }
            else
            {
                m_deblockingParams.Strength[dir][idx] = 0;
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx + 1] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }
    }
    // when reference indexes are different
    else
    {
        m_deblockingParams.Strength[dir][idx] = (Ipp8u) (1 + ((cbp4x4_luma & blkQ) ? 1 : 0));
        m_deblockingParams.Strength[dir][idx + 1] = (Ipp8u) (1 + ((cbp4x4_luma & blkQ2) ? 1 : 0));
        m_deblockingParams.DeblockingFlag[dir] = 1;
    }
}

void H264SegmentDecoder::PrepareDeblockingParametersPSlice4(Ipp32u dir)
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    Ipp32s *pDeblockingFlag = &(m_deblockingParams.DeblockingFlag[dir]);

    //
    // external edge
    //

    if (m_deblockingParams.ExternalEdgeFlag[dir])
    {
        // select neighbour addres
        Ipp32s nNeighbour = m_deblockingParams.nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNeighbour].mbtype))
        {
            H264DecoderFrame **pNRefPicList0 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 0)->m_RefPicList;
            ReferenceFlags *pNFields0 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 0)->m_Flags;

            // cicle on blocks
            for (Ipp32s idx = 0; idx < 4; idx += 2)
            {
                foo_external_p(cbp4x4_luma, nNeighbour, dir,
                    idx,
                    pNRefPicList0, pNFields0);
            }
        }
        // external edge required in strong filtering
        else
        {
            if ((HORIZONTAL_DEBLOCKING == dir) &&
                (m_deblockingParams.MBFieldCoded))
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    //
    // internal edge(s)
    //
    {
        // cicle of edge(s)
        // we do all edges in one cicle
        for (Ipp32s idx = 4; idx < 16; idx += 2)
        {
            foo_internal_p(dir,
                idx, cbp4x4_luma);
        }
    }
} // void H264SegmentDecoder::PrepareDeblockingParametersPSlice4(Ipp32u dir)

#define SET_DEBLOCKING_STRENGTH_P_SLICE_VERTICAL(block_num0, block_num1) \
{ \
    size_t refPrev = GetReferencePSlice(nNeighbour, block_num0 * 4 + 3); \
    if (refCur == refPrev) \
    { \
        if (0 == (VERTICAL_OUTER_EDGE_BLOCK_##block_num0 & uMask)) \
        { \
            H264DecoderMotionVector &pMVPrev = GetMV(m_gmbinfo, 0, nNeighbour, block_num0 * 4 + 3); \
            if (IsVectorDifferenceBig(mvCur, pMVPrev, m_deblockingParams.nMaxMVector)) \
            { \
                pStrength[block_num0] = 1; \
                iDeblockingFlag = 1; \
            } \
            else \
                pStrength[block_num0] = 0; \
        } \
        else \
        { \
            pStrength[block_num0] = 2; \
            iDeblockingFlag = 1; \
        } \
        if (0 == (VERTICAL_OUTER_EDGE_BLOCK_##block_num1 & uMask)) \
        { \
            H264DecoderMotionVector &pMVPrev = GetMV(m_gmbinfo, 0, nNeighbour, block_num1 * 4 + 3); \
            if (IsVectorDifferenceBig(mvCur, pMVPrev, m_deblockingParams.nMaxMVector)) \
            { \
                pStrength[block_num1] = 1; \
                iDeblockingFlag = 1; \
            } \
            else \
                pStrength[block_num1] = 0; \
        } \
        else \
        { \
            pStrength[block_num1] = 2; \
            iDeblockingFlag = 1; \
        } \
    } \
    else \
    { \
        pStrength[block_num0] = (Ipp8u) (1 + ((VERTICAL_OUTER_EDGE_BLOCK_##block_num0 & uMask) ? (1) : (0))); \
        pStrength[block_num1] = (Ipp8u) (1 + ((VERTICAL_OUTER_EDGE_BLOCK_##block_num1 & uMask) ? (1) : (0))); \
        iDeblockingFlag = 1; \
    } \
}

#define SET_DEBLOCKING_STRENGTH_P_SLICE_HORIZONTAL(block_num0, block_num1) \
{ \
    size_t refPrev = GetReferencePSlice(nNeighbour, block_num0 + 12); \
    if (refCur == refPrev) \
    { \
        if (0 == (HORIZONTAL_OUTER_EDGE_BLOCK_##block_num0 & uMask)) \
        { \
            H264DecoderMotionVector &pMVPrev = GetMV(m_gmbinfo, 0, nNeighbour, block_num0 + 12); \
            if (IsVectorDifferenceBig(mvCur, pMVPrev, m_deblockingParams.nMaxMVector)) \
            { \
                pStrength[block_num0] = 1; \
                iDeblockingFlag = 1; \
            } \
            else \
                pStrength[block_num0] = 0; \
        } \
        else \
        { \
            pStrength[block_num0] = 2; \
            iDeblockingFlag = 1; \
        } \
        if (0 == (HORIZONTAL_OUTER_EDGE_BLOCK_##block_num1 & uMask)) \
        { \
            H264DecoderMotionVector &pMVPrev = GetMV(m_gmbinfo, 0, nNeighbour, block_num1 + 12); \
            if (IsVectorDifferenceBig(mvCur, pMVPrev, m_deblockingParams.nMaxMVector)) \
            { \
                pStrength[block_num1] = 1; \
                iDeblockingFlag = 1; \
            } \
            else \
                pStrength[block_num1] = 0; \
        } \
        else \
        { \
            pStrength[block_num1] = 2; \
            iDeblockingFlag = 1; \
        } \
    } \
    else \
    { \
        pStrength[block_num0] = (Ipp8u) (1 + ((HORIZONTAL_OUTER_EDGE_BLOCK_##block_num0 & uMask) ? (1) : (0))); \
        pStrength[block_num1] = (Ipp8u) (1 + ((HORIZONTAL_OUTER_EDGE_BLOCK_##block_num1 & uMask) ? (1) : (0))); \
        iDeblockingFlag = 1; \
    } \
}

inline
size_t H264SegmentDecoder::GetReferencePSlice(Ipp32s iMBNum, Ipp32s iBlockNum)
{
    Ipp32s index;
    size_t ref;

    // select reference index for current block
    index = GetReferenceIndex(m_gmbinfo, 0, iMBNum, iBlockNum);
    if (0 <= index)
    {
        H264DecoderFrame **pRefPicList;
        H264DecoderMacroblockGlobalInfo *pMBInfo;
        Ipp32s iNum = 0;

        // obtain reference list
        pMBInfo = m_gmbinfo->mbs + iMBNum;
        pRefPicList = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 0)->m_RefPicList;
        if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
        {
            ReferenceFlags *pFields;

            pFields = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 0)->m_Flags;
            iNum = GetReferenceField(pFields, index);
        }
        ref = pRefPicList[index]->DeblockPicID(iNum);
    }
    else
        ref = (size_t)-1;

    return ref;

} // Ipp32s H264SegmentDecoder::GetReferencePSlice(Ipp32s iMBNum, Ipp32s iBlockNum)

inline void H264SegmentDecoder::GetReferencesBCurMB(Ipp32s iBlockNum, size_t *pReferences)
{
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        Ipp32s index = m_cur_mb.GetReferenceIndex(0, iBlockNum);
        pReferences[0] = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));
        index = m_cur_mb.GetReferenceIndex(1, iBlockNum);
        pReferences[1] = m_pRefPicList[1][index]->DeblockPicID(GetReferenceField(m_pFields[1], index));
    }
    else
    {
        Ipp32s index = m_cur_mb.GetReferenceIndex(0, iBlockNum);
        pReferences[0] = m_pRefPicList[0][index]->DeblockPicID(0);
        index = m_cur_mb.GetReferenceIndex(1, iBlockNum);
        pReferences[1] = m_pRefPicList[1][index]->DeblockPicID(0);
    }
}

inline void H264SegmentDecoder::GetReferencesBSlice(Ipp32s iMBNum, Ipp32s iBlockNum, size_t *pReferences)
{
    H264DecoderMacroblockGlobalInfo *pMBInfo = m_gmbinfo->mbs + iMBNum;

    H264DecoderFrame **pRefPicList0 = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 0)->m_RefPicList;
    H264DecoderFrame **pRefPicList1 = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 1)->m_RefPicList;

    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        ReferenceFlags *pFields = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 0)->m_Flags;
        Ipp32s index = GetReferenceIndex(m_gmbinfo, 0, iMBNum, iBlockNum);
        pReferences[0] = pRefPicList0[index]->DeblockPicID(GetReferenceField(pFields, index));

        pFields = m_pCurrentFrame->GetRefPicList(pMBInfo->slice_id, 1)->m_Flags;
        index = GetReferenceIndex(m_gmbinfo, 1, iMBNum, iBlockNum);
        pReferences[1] = pRefPicList1[index]->DeblockPicID(GetReferenceField(pFields, index));
    }
    else
    {
        Ipp32s index = GetReferenceIndex(m_gmbinfo, 0, iMBNum, iBlockNum);
        pReferences[0] = pRefPicList0[index]->DeblockPicID(0);

        index = GetReferenceIndex(m_gmbinfo, 1, iMBNum, iBlockNum);
        pReferences[1] = pRefPicList1[index]->DeblockPicID(0);
    }
} // Ipp32s H264SegmentDecoder::GetReferencesBSlice(Ipp32s iMBNum, Ipp32s iBlockNum, Ipp32s *pReferences)

void H264SegmentDecoder::PrepareDeblockingParametersPSlice16x16Vert()
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[VERTICAL_DEBLOCKING];
    Ipp32s iDeblockingFlag = 0;

    //
    // precalculating of reference numbers of current macroblock.
    // it is more likely we have to compare reference numbers.
    // we will use it also in horizontal deblocking
    //
    m_deblockingParams.iReferences[0][0] = GetReferencePSlice(m_CurMBAddr, 0);

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[VERTICAL_DEBLOCKING];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((m_gmbinfo->mbs + nNeighbour)->mbtype))
        {
            H264DecoderMacroblockLocalInfo *pNeighbour;
            Ipp32u uMask;

            // select neighbour
            pNeighbour = m_mbinfo.mbs + nNeighbour;

            // create special mask
            uMask = cbp4x4_luma | (pNeighbour->cbp4x4_luma >> 5);

            // when at least one from a couple of blocks has coeffs
            if (VERTICAL_OUTER_EDGE_MASK != (uMask & VERTICAL_OUTER_EDGE_MASK))
            {
                // obtain current block parameters
                size_t refCur = m_deblockingParams.iReferences[0][0];
                H264DecoderMotionVector  &mvCur = m_cur_mb.GetMV(0, 0);

                SET_DEBLOCKING_STRENGTH_P_SLICE_VERTICAL(0, 1)

                SET_DEBLOCKING_STRENGTH_P_SLICE_VERTICAL(2, 3)
            }
            else
            {
                SetEdgeStrength(pStrength + 0, 2);
                iDeblockingFlag = 1;
            }
        }
        else
        {
            SetEdgeStrength(pStrength + 0, 4);
            iDeblockingFlag = 1;
        }
    }

    m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = iDeblockingFlag;

    //
    // internal edge(s)
    //
    PrepareDeblockingParametersIntern16x16Vert();

} // void H264SegmentDecoder::PrepareDeblockingParametersPSlice16x16Vert()

void H264SegmentDecoder::PrepareDeblockingParametersPSlice16x16Horz()
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING];
    Ipp32s iDeblockingFlag = 0;

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[HORIZONTAL_DEBLOCKING];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((m_gmbinfo->mbs + nNeighbour)->mbtype))
        {
            H264DecoderMacroblockLocalInfo *pNeighbour;
            Ipp32u uMask;

            // select neighbour
            pNeighbour = m_mbinfo.mbs + nNeighbour;

            // create special mask
            uMask = cbp4x4_luma | (pNeighbour->cbp4x4_luma >> 10);

            // when at least one from a couple of blocks has coeffs
            if (HORIZONTAL_OUTER_EDGE_MASK != (uMask & HORIZONTAL_OUTER_EDGE_MASK))
            {
                // obtain current block parameters
                size_t refCur = m_deblockingParams.iReferences[0][0];
                H264DecoderMotionVector &mvCur = m_cur_mb.GetMV(0, 0);

                SET_DEBLOCKING_STRENGTH_P_SLICE_HORIZONTAL(0, 1)

                SET_DEBLOCKING_STRENGTH_P_SLICE_HORIZONTAL(2, 3)
            }
            // when at least one from a couple of blocks has coeffs
            else
            {
                SetEdgeStrength(pStrength + 0, 2);
                iDeblockingFlag = 1;
            }
        }
        else
        {
            if (m_deblockingParams.MBFieldCoded)
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            iDeblockingFlag = 1;
        }
    }

    m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] = iDeblockingFlag;

    //
    // internal edge(s)
    //
    PrepareDeblockingParametersIntern16x16Horz();

} // void H264SegmentDecoder::PrepareDeblockingParametersPSlice16x16Horz()

inline void H264SegmentDecoder::EvaluateStrengthExternal(Ipp32s cbp4x4_luma, Ipp32s nNeighbour, Ipp32s dir,
    Ipp32s idx,
    H264DecoderFrame **pNRefPicList0,
    ReferenceFlags *pNFields0,
    H264DecoderFrame **pNRefPicList1,
    ReferenceFlags *pNFields1)
{
    Ipp32s blkQ, blkP;
    Ipp32s blkQ2, blkP2;

    H264DecoderMacroblockLocalInfo *pNeighbour = &m_mbinfo.mbs[nNeighbour];
    Ipp32s nBlock, nNeighbourBlock;

    blkQ = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
    blkP = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

    blkQ2 = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx + 1];
    blkP2 = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx + 1];

    bool is_first = (cbp4x4_luma & blkQ) ||
        (pNeighbour->cbp4x4_luma & blkP);

    bool is_second = (cbp4x4_luma & blkQ2) ||
        (pNeighbour->cbp4x4_luma & blkP2);

    if (is_first && is_second)
    {
        m_deblockingParams.Strength[dir][idx] = 2;
        m_deblockingParams.Strength[dir][idx + 1] = 2;
        m_deblockingParams.DeblockingFlag[dir] = 1;
        return;
    }

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = idx * 4;
        nNeighbourBlock = nBlock + 3;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx + 12;
    }

    size_t iRefQFrw, iRefPFrw, iRefQBck, iRefPBck;

    iRefQFrw = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][0];
    iRefQBck = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][1];

    // field coded image
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        Ipp32s index;

        // select reference index for previous block
        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
        iRefPFrw = pNRefPicList0[index]->DeblockPicID(GetReferenceField(pNFields0, index));
        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
        iRefPBck = pNRefPicList1[index]->DeblockPicID(GetReferenceField(pNFields1, index));
    }
    // frame coded image
    else
    {
        Ipp32s index;
        // select reference index for previous block
        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
        iRefPFrw = pNRefPicList0[index]->DeblockPicID(0);

        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
        iRefPBck = pNRefPicList1[index]->DeblockPicID(0);
    }

    // when reference indexes are equal
    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
    {
        if (!is_first)
        {
            m_deblockingParams.Strength[dir][idx] = 0;

            // select current block motion vectors
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

            // select previous block motion vectors
            H264DecoderMotionVector pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
            H264DecoderMotionVector pVectorPBck = GetMV(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);

            // when forward and backward reference pictures of previous block are different
            if (iRefPFrw != iRefPBck)
            {
                if (iRefQFrw != iRefPFrw)
                {
                    swapValues(pVectorPFrw, pVectorPBck);
                }

                if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
            // when forward and backward reference pictures of previous block are equal
            else
            {
                if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
                    (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }

        idx++;

        // when one of couple of blocks has coeffs
        if (!is_second)
        {
            // calc block and neighbour block number
            if (VERTICAL_DEBLOCKING == dir)
            {
                nBlock = idx * 4;
                nNeighbourBlock = nBlock + 3;
            }
            else
            {
                nBlock = idx;
                nNeighbourBlock = idx + 12;
            }

            // select current block motion vectors
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

            // select previous block motion vectors
            H264DecoderMotionVector pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
            H264DecoderMotionVector pVectorPBck = GetMV(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);

            m_deblockingParams.Strength[dir][idx] = 0;

            // when forward and backward reference pictures of previous block are different
            if (iRefPFrw != iRefPBck)
            {
                if (iRefQFrw != iRefPFrw)
                {
                    swapValues(pVectorPFrw, pVectorPBck);
                }

                if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
            // when forward and backward reference pictures of previous block are equal
            else
            {
                if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
                    (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }
    }
    // when reference indexes are different
    else
    {
        m_deblockingParams.Strength[dir][idx] = (Ipp8u) (1 + (is_first ? 1 : 0));
        m_deblockingParams.Strength[dir][idx + 1] = (Ipp8u) (1 + (is_second ? 1 : 0));
        m_deblockingParams.DeblockingFlag[dir] = 1;
    }
}

inline void H264SegmentDecoder::foo_internal(Ipp32s dir,
    Ipp32s idx, Ipp32u cbp4x4_luma)
{
    size_t iRefQFrw, iRefQBck, iRefPFrw, iRefPBck;

    Ipp32s nBlock, nNeighbourBlock;

    Ipp32s blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];
    Ipp32s blkQ2 = INTERNAL_BLOCKS_MASK[dir][idx - 3];

    if ((cbp4x4_luma & blkQ) && (cbp4x4_luma & blkQ2))
    {
        m_deblockingParams.Strength[dir][idx] = 2;
        m_deblockingParams.Strength[dir][idx + 1] = 2;
        m_deblockingParams.DeblockingFlag[dir] = 1;
        return;
    }

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = (idx & 3) * 4 + (idx >> 2);
        nNeighbourBlock = nBlock - 1;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx - 4;
    }

    // field coded image
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        Ipp32s index;

        // select forward reference index for blocks
        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));
        index = m_cur_mb.GetReferenceIndex(0, nNeighbourBlock);
        iRefPFrw = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));

        // select backward reference index for blocks
        index = m_cur_mb.GetReferenceIndex(1, nBlock);
        iRefQBck = m_pRefPicList[1][index]->DeblockPicID(GetReferenceField(m_pFields[1], index));
        index = m_cur_mb.GetReferenceIndex(1, nNeighbourBlock);
        iRefPBck = m_pRefPicList[1][index]->DeblockPicID(GetReferenceField(m_pFields[1], index));
    }
    else  // frame coded image
    {
        Ipp32s index;

        index = m_cur_mb.GetReferenceIndex(0, nBlock);
        iRefQFrw = m_pRefPicList[0][index]->DeblockPicID(0);
        index = m_cur_mb.GetReferenceIndex(0, nNeighbourBlock);
        iRefPFrw = m_pRefPicList[0][index]->DeblockPicID(0);

        index = m_cur_mb.GetReferenceIndex(1, nBlock);
        iRefQBck = m_pRefPicList[1][index]->DeblockPicID(0);
        index = m_cur_mb.GetReferenceIndex(1, nNeighbourBlock);
        iRefPBck = m_pRefPicList[1][index]->DeblockPicID(0);
    }

    // when reference indexes are equal
    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
    {
        if (!(cbp4x4_luma & blkQ))
        {
            // select current block motion vectors
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

            // select previous block motion vectors
            H264DecoderMotionVector pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);
            H264DecoderMotionVector pVectorPBck = m_cur_mb.GetMV(1, nNeighbourBlock);

            // set initial value of strength
            m_deblockingParams.Strength[dir][idx] = 0;

            // when forward and backward reference pictures of previous block are different
            if (iRefPFrw != iRefPBck)
            {
                // select previous block motion vectors
                if (iRefQFrw != iRefPFrw)
                {
                    swapValues(pVectorPFrw, pVectorPBck);
                }

                // compare motion vectors
                if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }

            }
            // when forward and backward reference pictures of previous block are equal
            else
            {
                // compare motion vectors
                if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
                    (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }

        if (!(cbp4x4_luma & blkQ2))
        {
            idx++;
            // calc block and neighbour block number
            if (VERTICAL_DEBLOCKING == dir)
            {
                nBlock = (idx & 3) * 4 + (idx >> 2);
                nNeighbourBlock = nBlock - 1;
            }
            else
            {
                nBlock = idx;
                nNeighbourBlock = idx - 4;
            }

            // set initial value of strength
            m_deblockingParams.Strength[dir][idx] = 0;

            // select current block motion vectors
            H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
            H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

            // select previous block motion vectors
            H264DecoderMotionVector pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);
            H264DecoderMotionVector pVectorPBck = m_cur_mb.GetMV(1, nNeighbourBlock);

            // when forward and backward reference pictures of previous block are different
            if (iRefPFrw != iRefPBck)
            {
                // select previous block motion vectors
                if ((iRefQFrw != iRefPFrw) && (cbp4x4_luma & blkQ))
                {
                    swapValues(pVectorPFrw, pVectorPBck);
                }

                // compare motion vectors
                if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
            // when forward and backward reference pictures of previous block are equal
            else
            {
                // compare motion vectors
                if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
                    (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                    IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
                {
                    m_deblockingParams.Strength[dir][idx] = 1;
                    m_deblockingParams.DeblockingFlag[dir] = 1;
                }
            }
        }
        else
        {
            m_deblockingParams.Strength[dir][idx + 1] = 2;
            m_deblockingParams.DeblockingFlag[dir] = 1;
        }
    }
    // when reference indexes are different
    else
    {
        m_deblockingParams.Strength[dir][idx] = (Ipp8u) (1 + ((cbp4x4_luma & blkQ) ? 1 : 0));
        m_deblockingParams.Strength[dir][idx + 1] = (Ipp8u) (1 + ((cbp4x4_luma & blkQ2) ? 1 : 0));
        m_deblockingParams.DeblockingFlag[dir] = 1;
    }
}

inline Ipp8u H264SegmentDecoder::EvaluateStrengthInternal(Ipp32s dir, Ipp32s idx)
{
    Ipp32s nBlock, nNeighbourBlock;

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = VERTICAL_DEBLOCKING_BLOCKS_MAP[idx];
        nNeighbourBlock = nBlock - 1;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx - 4;
    }

    size_t iRefQFrw = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][0];
    size_t iRefQBck = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][1];
    size_t iRefPFrw = m_deblockingParams.iReferences[subblock_block_membership[nNeighbourBlock]][0];
    size_t iRefPBck = m_deblockingParams.iReferences[subblock_block_membership[nNeighbourBlock]][1];

    // when reference indexes are equal
    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
    {
        // select current block motion vectors
        H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
        H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

        // select previous block motion vectors
        H264DecoderMotionVector pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);
        H264DecoderMotionVector pVectorPBck = m_cur_mb.GetMV(1, nNeighbourBlock);

        // when forward and backward reference pictures of previous block are different
        if (iRefPFrw != iRefPBck)
        {
            // select previous block motion vectors
            if (iRefQFrw != iRefPFrw)
            {
                swapValues(pVectorPFrw, pVectorPBck);
            }

            // compare motion vectors
            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
            {
                m_deblockingParams.DeblockingFlag[dir] = 1;
                return 1;
            }

        }
        // when forward and backward reference pictures of previous block are equal
        else
        {
            // compare motion vectors
            if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
                (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
            {
                m_deblockingParams.DeblockingFlag[dir] = 1;
                return 1;
            }
        }

        return 0;
    }
    // when reference indexes are different
    else
    {
        m_deblockingParams.DeblockingFlag[dir] = 1;
        return 1;
    }
}

inline Ipp8u H264SegmentDecoder::EvaluateStrengthInternal8x8(Ipp32s dir, Ipp32s idx)
{
    Ipp32s nBlock, nNeighbourBlock;

    // calc block and neighbour block number
    if (VERTICAL_DEBLOCKING == dir)
    {
        nBlock = VERTICAL_DEBLOCKING_BLOCKS_MAP[idx];
        nNeighbourBlock = nBlock - 1;
    }
    else
    {
        nBlock = idx;
        nNeighbourBlock = idx - 4;
    }

    // select current block motion vectors
    H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, nBlock);
    H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, nBlock);

    // select previous block motion vectors
    H264DecoderMotionVector &pVectorPFrw = m_cur_mb.GetMV(0, nNeighbourBlock);
    H264DecoderMotionVector &pVectorPBck = m_cur_mb.GetMV(1, nNeighbourBlock);

    size_t iRefQFrw = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][0];
    size_t iRefQBck = m_deblockingParams.iReferences[subblock_block_membership[nBlock]][1];

    // when forward and backward reference pictures of previous block are different
    if (iRefQFrw != iRefQBck)
    {
        // compare motion vectors
        if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
            IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
        {
            m_deblockingParams.DeblockingFlag[dir] = 1;
            return 1;
        }
    }
    // when forward and backward reference pictures of previous block are equal
    else
    {
        // compare motion vectors
        if ((IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
            IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector)) &&
            (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
            IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector)))
        {
            m_deblockingParams.DeblockingFlag[dir] = 1;
            return 1;
        }
    }

    return 0;
}

inline void H264SegmentDecoder::GetReferencesB8x8()
{
    // field coded image
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
    {
        for (Ipp32s i = 0; i < 4; i++)
        {
            Ipp32s index = m_cur_mb.GetRefIdx(0, i);
            m_deblockingParams.iReferences[i][0] = m_pRefPicList[0][index]->DeblockPicID(GetReferenceField(m_pFields[0], index));

            // select backward reference index for blocks
            index = m_cur_mb.GetRefIdx(1, i);
            m_deblockingParams.iReferences[i][1] = m_pRefPicList[1][index]->DeblockPicID(GetReferenceField(m_pFields[1], index));
        }
    }
    else  // frame coded image
    {
        for (Ipp32s i = 0; i < 4; i++)
        {
            Ipp32s index;

            index = m_cur_mb.GetRefIdx(0, i);
            m_deblockingParams.iReferences[i][0] = m_pRefPicList[0][index]->DeblockPicID(0);

            index = m_cur_mb.GetRefIdx(1, i);
            m_deblockingParams.iReferences[i][1] = m_pRefPicList[1][index]->DeblockPicID(0);
        }
    }
}

inline void H264SegmentDecoder::SetStrength(Ipp32s dir, Ipp32s block_num, Ipp32u cbp4x4_luma, Ipp8u strength)
{
    Ipp32s blkQ = INTERNAL_BLOCKS_MASK[dir][block_num - 4];
    if (cbp4x4_luma & blkQ)
    {
        m_deblockingParams.Strength[dir][block_num] = 2;
        m_deblockingParams.DeblockingFlag[dir] = 1;
    }
    else
    {
        m_deblockingParams.Strength[dir][block_num] = strength;
    }
}

void H264SegmentDecoder::PrepareStrengthsInternal()
{
    Ipp32s mbtype = m_cur_mb.GlobalMacroblockInfo->mbtype;

    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;

    switch (mbtype)
    {
    case MBTYPE_INTER:
    case MBTYPE_FORWARD:
    case MBTYPE_BACKWARD:
    case MBTYPE_BIDIR:
        {
            for (Ipp32s i = 4; i < 16; i++)
            {
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, 0);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, 0);
            }
        }
        break;

    case MBTYPE_INTER_8x16:
    case MBTYPE_INTER_16x8:
        {
            for (Ipp32s i = 4; i < 8; i++)
            {
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, 0);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, 0);
            }

            Ipp8u strength_horz = 0, strength_vert = 0;

            if (mbtype == MBTYPE_INTER_8x16)
            {
                strength_vert = EvaluateStrengthInternal(VERTICAL_DEBLOCKING, 8);
            }
            else
            {
                strength_horz = EvaluateStrengthInternal(HORIZONTAL_DEBLOCKING, 8);
            }

            for (Ipp32s i = 8; i < 12; i++)
            {
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, strength_vert);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, strength_horz);
            }

            for (Ipp32s i = 12; i < 16; i++)
            {
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, 0);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, 0);
            }
        }
        break;

    case MBTYPE_INTER_8x8:
    case MBTYPE_INTER_8x8_REF0:
    default:
        {
            Ipp8u strength;

            for (Ipp32s i = 8; i < 10; i++)
            {
                strength = EvaluateStrengthInternal(VERTICAL_DEBLOCKING, i);
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, strength);
                strength = EvaluateStrengthInternal(HORIZONTAL_DEBLOCKING, i);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, strength);
            }

            for (Ipp32s i = 10; i < 12; i++)
            {
                strength = EvaluateStrengthInternal(VERTICAL_DEBLOCKING, i);
                SetStrength(VERTICAL_DEBLOCKING, i, cbp4x4_luma, strength);
                strength = EvaluateStrengthInternal(HORIZONTAL_DEBLOCKING, i);
                SetStrength(HORIZONTAL_DEBLOCKING, i, cbp4x4_luma, strength);
            }

            for (Ipp32s i = 0; i < 4; i++)
            {
                Ipp32s start_8x8 = subblock_block_mapping[i];
                Ipp32s start_block = start_8x8 + 4;
                Ipp32s start_block_vertical = VERTICAL_DEBLOCKING_BLOCKS_MAP[start_8x8 + 1];
                switch (m_cur_mb.GlobalMacroblockInfo->sbtype[i])
                {
                case SBTYPE_8x8:
                    {
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical, cbp4x4_luma, 0);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical + 1, cbp4x4_luma, 0);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block, cbp4x4_luma, 0);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block + 1, cbp4x4_luma, 0);
                    }
                    break;

                case SBTYPE_8x4:
                    {
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical, cbp4x4_luma, 0);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical + 1, cbp4x4_luma, 0);

                        strength = EvaluateStrengthInternal8x8(HORIZONTAL_DEBLOCKING, start_block);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block, cbp4x4_luma, strength);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block + 1, cbp4x4_luma, strength);
                    }
                    break;

                case SBTYPE_4x8:
                    {
                        strength = EvaluateStrengthInternal8x8(VERTICAL_DEBLOCKING, start_block_vertical);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical, cbp4x4_luma, strength);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical + 1, cbp4x4_luma, strength);

                        SetStrength(HORIZONTAL_DEBLOCKING, start_block, cbp4x4_luma, 0);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block + 1, cbp4x4_luma, 0);
                    }
                    break;

                default: // 4x4 sub division
                    {
                        strength = EvaluateStrengthInternal8x8(VERTICAL_DEBLOCKING, start_block_vertical);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical, cbp4x4_luma, strength);
                        strength = EvaluateStrengthInternal8x8(VERTICAL_DEBLOCKING, start_block_vertical + 1);
                        SetStrength(VERTICAL_DEBLOCKING, start_block_vertical + 1, cbp4x4_luma, strength);

                        strength = EvaluateStrengthInternal8x8(HORIZONTAL_DEBLOCKING, start_block);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block, cbp4x4_luma, strength);
                        strength = EvaluateStrengthInternal8x8(HORIZONTAL_DEBLOCKING, start_block + 1);
                        SetStrength(HORIZONTAL_DEBLOCKING, start_block + 1, cbp4x4_luma, strength);
                    }
                    break;
                }
            }
        }
        break;
    }
}

void H264SegmentDecoder::PrepareDeblockingParametersBSlice4(Ipp32u dir)
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    Ipp32s *pDeblockingFlag = &(m_deblockingParams.DeblockingFlag[dir]);

    GetReferencesB8x8();

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[dir])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNeighbour].mbtype))
        {
            Ipp32s idx;

            H264DecoderFrame **pNRefPicList0 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 0)->m_RefPicList;
            ReferenceFlags *pNFields0 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 0)->m_Flags;
            H264DecoderFrame **pNRefPicList1 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 1)->m_RefPicList;
            ReferenceFlags *pNFields1 = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[nNeighbour].slice_id, 1)->m_Flags;

            // cicle on blocks
            for (idx = 0; idx < 4; idx += 2)
            {
                EvaluateStrengthExternal(cbp4x4_luma, nNeighbour, dir,
                    idx,
                    pNRefPicList0, pNFields0, pNRefPicList1, pNFields1);
            }
        }
        // external edge required in strong filtering
        else
        {
            if ((HORIZONTAL_DEBLOCKING == dir) &&
                (m_deblockingParams.MBFieldCoded))
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    // static Ipp8u temp[2][160];

    if (dir == VERTICAL_DEBLOCKING)
    {
        PrepareStrengthsInternal();
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice4(Ipp32u dir)

#define SET_DEBLOCKING_STRENGTH_B_SLICE_VERTICAL(block_num0, block_num1) \
{ \
    size_t refPrev[2]; \
    GetReferencesBSlice(nNeighbour, block_num0 * 4 + 3, refPrev); \
    if ((0 == (refPrev[0] ^ refPrev[1] ^ refCurFwd ^ refCurBwd)) && \
        ((refPrev[0] == refCurFwd) || (refPrev[0] == refCurBwd))) \
    { \
        if (0 == (VERTICAL_OUTER_EDGE_BLOCK_##block_num0 & uMask)) \
        { \
            H264DecoderMotionVector pMVPrevFwd = GetMV(m_gmbinfo, 0, nNeighbour, block_num0 * 4 + 3); \
            H264DecoderMotionVector pMVPrevBwd = GetMV(m_gmbinfo, 1, nNeighbour, block_num0 * 4 + 3); \
            if (refCurFwd != refCurBwd) \
            { \
                /* exchange reference for direct equality */ \
                if (refCurFwd != refPrev[0]) \
                { \
                    swapValues(mvCurFwd, mvCurBwd); \
                    swapValues(refCurFwd, refCurBwd); \
                } \
                /* compare motion vectors */ \
                if (IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                    IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) \
                { \
                    pStrength[block_num0] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num0] = 0; \
            } \
            else \
            { \
                /* compare motion vectors */ \
                if ((IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) && \
                    (IsVectorDifferenceBig(mvCurFwd, pMVPrevBwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevFwd, m_deblockingParams.nMaxMVector))) \
                { \
                    pStrength[block_num0] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num0] = 0; \
            } \
        } \
        else \
        { \
            pStrength[block_num0] = 2; \
            iDeblockingFlag = 1; \
        } \
        if (0 == (VERTICAL_OUTER_EDGE_BLOCK_##block_num1 & uMask)) \
        { \
            H264DecoderMotionVector pMVPrevFwd = GetMV(m_gmbinfo, 0, nNeighbour, block_num1 * 4 + 3); \
            H264DecoderMotionVector pMVPrevBwd = GetMV(m_gmbinfo, 1, nNeighbour, block_num1 * 4 + 3); \
            if (refCurFwd != refCurBwd) \
            { \
                /* exchange reference for direct equality */ \
                if (refCurFwd != refPrev[0]) \
                { \
                    swapValues(mvCurFwd, mvCurBwd); \
                    swapValues(refCurFwd, refCurBwd); \
                } \
                /* compare motion vectors */ \
                if (IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                    IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) \
                { \
                    pStrength[block_num1] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num1] = 0; \
            } \
            else \
            { \
                /* compare motion vectors */ \
                if ((IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) && \
                    (IsVectorDifferenceBig(mvCurFwd, pMVPrevBwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevFwd, m_deblockingParams.nMaxMVector))) \
                { \
                    pStrength[block_num1] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num1] = 0; \
            } \
        } \
        else \
        { \
            pStrength[block_num1] = 2; \
            iDeblockingFlag = 1; \
        } \
    } \
    else \
    { \
        pStrength[block_num0] = (Ipp8u) (1 + ((VERTICAL_OUTER_EDGE_BLOCK_##block_num0 & uMask) ? (1) : (0))); \
        pStrength[block_num1] = (Ipp8u) (1 + ((VERTICAL_OUTER_EDGE_BLOCK_##block_num1 & uMask) ? (1) : (0))); \
        iDeblockingFlag = 1; \
    } \
}

#define SET_DEBLOCKING_STRENGTH_B_SLICE_HORIZONTAL(block_num0, block_num1) \
{ \
    size_t refPrev[2]; \
    GetReferencesBSlice(nNeighbour, block_num0 + 12, refPrev); \
    if ((0 == (refPrev[0] ^ refPrev[1] ^ refCurFwd ^ refCurBwd)) && \
        ((refPrev[0] == refCurFwd) || (refPrev[0] == refCurBwd))) \
    { \
        bool is_first = (0 == (HORIZONTAL_OUTER_EDGE_BLOCK_##block_num0 & uMask)); \
        if (is_first) \
        { \
            H264DecoderMotionVector pMVPrevFwd = GetMV(m_gmbinfo, 0, nNeighbour, block_num0 + 12); \
            H264DecoderMotionVector pMVPrevBwd = GetMV(m_gmbinfo, 1, nNeighbour, block_num0 + 12); \
            if (refCurFwd != refCurBwd) \
            { \
                /* exchange reference for direct equality */ \
                if (refCurFwd != refPrev[0]) \
                { \
                    swapValues(mvCurFwd, mvCurBwd); \
                    swapValues(refCurFwd, refCurBwd); \
                } \
                /* compare motion vectors */ \
                if (IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                    IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) \
                { \
                    pStrength[block_num0] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num0] = 0; \
            } \
            else \
            { \
                /* compare motion vectors */ \
                if ((IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) && \
                    (IsVectorDifferenceBig(mvCurFwd, pMVPrevBwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevFwd, m_deblockingParams.nMaxMVector))) \
                { \
                    pStrength[block_num0] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num0] = 0; \
            } \
        } \
        else \
        { \
            pStrength[block_num0] = 2; \
            iDeblockingFlag = 1; \
        } \
        if (0 == (HORIZONTAL_OUTER_EDGE_BLOCK_##block_num1 & uMask)) \
        { \
            H264DecoderMotionVector pMVPrevFwd = GetMV(m_gmbinfo, 0, nNeighbour, block_num1 + 12); \
            H264DecoderMotionVector pMVPrevBwd = GetMV(m_gmbinfo, 1, nNeighbour, block_num1 + 12); \
            if (refCurFwd != refCurBwd) \
            { \
                /* exchange reference for direct equality */ \
                if (refCurFwd != refPrev[0]) \
                { \
                    swapValues(mvCurFwd, mvCurBwd); \
                    swapValues(refCurFwd, refCurBwd); \
                } \
                /* compare motion vectors */ \
                if (IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                    IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) \
                { \
                    pStrength[block_num1] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num1] = 0; \
            } \
            else \
            { \
                /* compare motion vectors */ \
                if ((IsVectorDifferenceBig(mvCurFwd, pMVPrevFwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevBwd, m_deblockingParams.nMaxMVector)) && \
                    (IsVectorDifferenceBig(mvCurFwd, pMVPrevBwd, m_deblockingParams.nMaxMVector) || \
                     IsVectorDifferenceBig(mvCurBwd, pMVPrevFwd, m_deblockingParams.nMaxMVector))) \
                { \
                    pStrength[block_num1] = 1; \
                    iDeblockingFlag = 1; \
                } \
                else \
                    pStrength[block_num1] = 0; \
            } \
        } \
        else \
        { \
            pStrength[block_num1] = 2; \
            iDeblockingFlag = 1; \
        } \
    } \
    else \
    { \
        pStrength[block_num0] = (Ipp8u) (1 + ((HORIZONTAL_OUTER_EDGE_BLOCK_##block_num0 & uMask) ? (1) : (0))); \
        pStrength[block_num1] = (Ipp8u) (1 + ((HORIZONTAL_OUTER_EDGE_BLOCK_##block_num1 & uMask) ? (1) : (0))); \
        iDeblockingFlag = 1; \
    } \
}

void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x16Vert()
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[VERTICAL_DEBLOCKING];
    Ipp32s iDeblockingFlag = 0;

    //
    // precalculating of reference numbers of current macroblock.
    // it is more likely we have to compare reference numbers.
    // we will use it also in horizontal deblocking
    //
    GetReferencesBCurMB(0, m_deblockingParams.iReferences[0]);

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[VERTICAL_DEBLOCKING];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNeighbour].mbtype))
        {
            // select neighbour
            H264DecoderMacroblockLocalInfo *pNeighbour = &m_mbinfo.mbs[nNeighbour];

            // create special mask
            Ipp32u uMask = cbp4x4_luma | (pNeighbour->cbp4x4_luma >> 5);

            // when at least one from a couple of blocks has coeffs
            if (VERTICAL_OUTER_EDGE_MASK != (uMask & VERTICAL_OUTER_EDGE_MASK))
            {
                // obtain current block parameters
                size_t refCurFwd = m_deblockingParams.iReferences[0][0];
                size_t refCurBwd = m_deblockingParams.iReferences[0][1];
                H264DecoderMotionVector mvCurFwd = m_cur_mb.GetMV(0, 0);
                H264DecoderMotionVector mvCurBwd = m_cur_mb.GetMV(1, 0);

                SET_DEBLOCKING_STRENGTH_B_SLICE_VERTICAL(0, 1)

                SET_DEBLOCKING_STRENGTH_B_SLICE_VERTICAL(2, 3)
            }
            else
            {
                SetEdgeStrength(pStrength + 0, 2);
                iDeblockingFlag = 1;
            }
        }
        else
        {
            SetEdgeStrength(pStrength + 0, 4);
            iDeblockingFlag = 1;
        }
    }

    m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = iDeblockingFlag;

    //
    // internal edge(s)
    //
    PrepareDeblockingParametersIntern16x16Vert();

} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x16Vert()

void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x16Horz()
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING];
    Ipp32s iDeblockingFlag = 0;

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[HORIZONTAL_DEBLOCKING];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNeighbour].mbtype))
        {
            // select neighbour
            H264DecoderMacroblockLocalInfo *pNeighbour = &m_mbinfo.mbs[nNeighbour];

            // create special mask
            Ipp32u uMask = cbp4x4_luma | (pNeighbour->cbp4x4_luma >> 10);

            // when at least one from a couple of blocks has coeffs
            if (HORIZONTAL_OUTER_EDGE_MASK != (uMask & HORIZONTAL_OUTER_EDGE_MASK))
            {
                // obtain current block parameters
                size_t refCurFwd = m_deblockingParams.iReferences[0][0];
                size_t refCurBwd = m_deblockingParams.iReferences[0][1];
                H264DecoderMotionVector mvCurFwd = m_cur_mb.GetMV(0, 0);
                H264DecoderMotionVector mvCurBwd = m_cur_mb.GetMV(1, 0);

                SET_DEBLOCKING_STRENGTH_B_SLICE_HORIZONTAL(0, 1)

                SET_DEBLOCKING_STRENGTH_B_SLICE_HORIZONTAL(2, 3)
            }
            // when at least one from a couple of blocks has coeffs
            else
            {
                SetEdgeStrength(pStrength + 0, 2);
                iDeblockingFlag = 1;
            }
        }
        else
        {
            if (m_deblockingParams.MBFieldCoded)
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            iDeblockingFlag = 1;
        }
    }

    m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] = iDeblockingFlag;

    //
    // internal edge(s)
    //
    PrepareDeblockingParametersIntern16x16Horz();

} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x16Horz()

void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x8(Ipp32u dir)
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    Ipp32s *pDeblockingFlag = &(m_deblockingParams.DeblockingFlag[dir]);
    size_t iRefQFrw, iRefQBck;

    //
    // external edge
    //

    // load reference indexes & motion vector for first half of current block
    {
        // field coded image
        if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
        {
            H264DecoderFrame **pRefPicList;
            Ipp32s index;
            ReferenceFlags *pFields;

            // select reference index for current block
            index = m_cur_mb.GetRefIdx(0, 0);
            if (0 <= index)
            {
                pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                // select reference fields number array
                pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_Flags;
                iRefQFrw = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
            }
            else
                iRefQFrw = (size_t)-1;
            index = m_cur_mb.GetRefIdx(1, 0);
            if (0 <= index)
            {
                pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                // select reference fields number array
                pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_Flags;
                iRefQBck = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
            }
            else
                iRefQBck = (size_t)-1;
        }
        // frame coded image
        else
        {
            H264DecoderFrame **pRefPicList;
            Ipp32s index;

            // select reference index for current block
            index = m_cur_mb.GetRefIdx(0, 0);
            if (0 <= index)
            {
                pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                iRefQFrw = pRefPicList[index]->DeblockPicID(0);
            }
            else
                iRefQFrw = (size_t)-1;
            index = m_cur_mb.GetRefIdx(1, 0);
            if (0 <= index)
            {
                pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                iRefQBck = pRefPicList[index]->DeblockPicID(0);
            }
            else
                iRefQBck = (size_t)-1;
        }
    }

    H264DecoderMotionVector &pVectorQFrw = m_cur_mb.GetMV(0, 0);
    H264DecoderMotionVector &pVectorQBck = m_cur_mb.GetMV(1, 0);

    // prepare deblocking parameter for external edge
    if (m_deblockingParams.ExternalEdgeFlag[dir])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((m_gmbinfo->mbs + nNeighbour)->mbtype))
        {
            H264DecoderMacroblockLocalInfo *pNeighbour;
            Ipp32s idx;

            // select neighbour
            pNeighbour = m_mbinfo.mbs + nNeighbour;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32s blkQ, blkP;

                blkQ = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp4x4_luma & blkQ) ||
                    (pNeighbour->cbp4x4_luma & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32s nNeighbourBlock;
                    size_t iRefPFrw, iRefPBck;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264DecoderFrame **pRefPicList;
                        Ipp32s index;
                        ReferenceFlags *pFields;

                        // select reference index for previous block
                        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_RefPicList;
                            // select reference fields number array
                            pFields = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_Flags;
                            iRefPFrw = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                        }
                        else
                            iRefPFrw = (size_t)-1;
                        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_RefPicList;
                            // select reference fields number array
                            pFields = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_Flags;
                            iRefPBck = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                        }
                        else
                            iRefPBck = (size_t)-1;
                    }
                    // frame coded image
                    else
                    {
                        H264DecoderFrame **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_RefPicList;
                            iRefPFrw = pRefPicList[index]->DeblockPicID(0);
                        }
                        else
                            iRefPFrw = (size_t)-1;
                        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_RefPicList;
                            iRefPBck = pRefPicList[index]->DeblockPicID(0);
                        }
                        else
                            iRefPBck = (size_t)-1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        H264DecoderMotionVector pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                        H264DecoderMotionVector pVectorPBck = GetMV(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            // select previous block motion vectors
                            if (iRefQFrw != iRefPFrw)
                            {
                                swapValues(pVectorPFrw, pVectorPBck);
                            }

                            // compare motion vectors
                            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                                IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            // compare motion vectors
                            if (IsVectorDifferenceBig(pVectorQFrw, pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                                IsVectorDifferenceBig(pVectorQBck, pVectorPBck, m_deblockingParams.nMaxMVector))
                            {
                                if (IsVectorDifferenceBig(pVectorQFrw, pVectorPBck, m_deblockingParams.nMaxMVector) ||
                                    IsVectorDifferenceBig(pVectorQBck, pVectorPFrw, m_deblockingParams.nMaxMVector))
                                {
                                    pStrength[idx] = 1;
                                    *pDeblockingFlag = 1;
                                }
                            }
                        }
                    }
                    // when reference indexes are different
                    else
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                }
            }
        }
        // external edge required in strong filtering
        else
        {
            if ((HORIZONTAL_DEBLOCKING == dir) &&
                (m_deblockingParams.MBFieldCoded))
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    //if (dir == VERTICAL_DEBLOCKING)
      //  foo_internal(pParams, dir);
    //return;
    //
    // internal edge(s)
    //
    {
        Ipp32s idx;

        // cicle of edge(s)
        for (idx = 4;idx < 8;idx += 1)
        {
            Ipp32s blkQ;

            blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp4x4_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // we haven't to compare motion vectors  - they are equal
            else
                pStrength[idx] = 0;
        }

        // load reference indexes & motion vector for second half of current block
        {
            size_t iRefQFrw2, iRefQBck2;
            Ipp32u nStrength;

            // load reference indexes for current block

            // field coded image
            if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
            {
                H264DecoderFrame **pRefPicList;
                Ipp32s index;
                ReferenceFlags *pFields;

                // select reference index for current block
                index = m_cur_mb.GetRefIdx(0, 3);
                if (0 <= index)
                {
                    pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                    // select reference fields number array
                    pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_Flags;
                    iRefQFrw2 = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                }
                else
                    iRefQFrw2 = (size_t)-1;
                index = m_cur_mb.GetRefIdx(1, 3);
                if (0 <= index)
                {
                    pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                    // select reference fields number array
                    pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_Flags;
                    iRefQBck2 = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                }
                else
                    iRefQBck2 = (size_t)-1;
            }
            // frame coded image
            else
            {
                H264DecoderFrame **pRefPicList;
                Ipp32s index;

                // select reference index for current block
                index = m_cur_mb.GetRefIdx(0, 3);
                if (0 <= index)
                {
                    pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                    iRefQFrw2 = pRefPicList[index]->DeblockPicID(0);
                }
                else
                    iRefQFrw2 = (size_t)-1;
                index = m_cur_mb.GetRefIdx(1, 3);
                if (0 <= index)
                {
                    pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                    iRefQBck2 = pRefPicList[index]->DeblockPicID(0);
                }
                else
                    iRefQBck2 = (size_t)-1;
            }

            // when reference indexes are equal
            if (((iRefQFrw == iRefQFrw2) && (iRefQBck == iRefQBck2)) ||
                ((iRefQFrw == iRefQBck2) && (iRefQBck == iRefQFrw2)))
            {
                // set initial value of strength
                nStrength = 0;

                // when forward and backward reference pictures of previous block are different
                if (iRefQFrw2 != iRefQBck2)
                {
                    H264DecoderMotionVector pVectorQFrw2 = m_cur_mb.GetMV(0, 15);
                    H264DecoderMotionVector pVectorQBck2 = m_cur_mb.GetMV(1, 15);

                    // select previous block motion vectors
                    if (iRefQFrw != iRefQFrw2)
                    {
                        swapValues(pVectorQFrw2, pVectorQBck2);
                    }

                    // compare motion vectors
                    if (IsVectorDifferenceBig(pVectorQFrw, pVectorQFrw2, m_deblockingParams.nMaxMVector) ||
                        IsVectorDifferenceBig(pVectorQBck, pVectorQBck2, m_deblockingParams.nMaxMVector))
                    {
                        nStrength = 1;
                        *pDeblockingFlag = 1;
                    }
                }
                // when forward and backward reference pictures of previous block are equal
                else
                {
                    // select block second motion vectors
                    H264DecoderMotionVector & pVectorQFrw2 = m_cur_mb.GetMV(0, 15);
                    H264DecoderMotionVector & pVectorQBck2 = m_cur_mb.GetMV(1, 15);

                    // compare motion vectors
                    if (IsVectorDifferenceBig(pVectorQFrw, pVectorQFrw2, m_deblockingParams.nMaxMVector) ||
                        IsVectorDifferenceBig(pVectorQBck, pVectorQBck2, m_deblockingParams.nMaxMVector))
                    {
                        if (IsVectorDifferenceBig(pVectorQFrw, pVectorQBck2, m_deblockingParams.nMaxMVector) ||
                            IsVectorDifferenceBig(pVectorQBck, pVectorQFrw2, m_deblockingParams.nMaxMVector))
                        {
                            nStrength = 1;
                            *pDeblockingFlag = 1;
                        }
                    }
                }
            }
            // when reference indexes are different
            else
            {
                nStrength = 1;
                *pDeblockingFlag = 1;
            }

            // cicle of edge(s)
            for (idx = 8;idx < 12;idx += 1)
            {
                Ipp32s blkQ;

                blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];

                if (cbp4x4_luma & blkQ)
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // we have compared motion vectors
                else
                    pStrength[idx] = (Ipp8u) nStrength;
            }
        }

        // cicle of edge(s)
        for (idx = 12;idx < 16;idx += 1)
        {
            Ipp32s blkQ;

            blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp4x4_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // we haven't to compare motion vectors  - they are equal
            else
                pStrength[idx] = 0;
        }
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice16x8(Ipp32u dir)

void H264SegmentDecoder::PrepareDeblockingParametersBSlice8x16(Ipp32u dir)
{
    Ipp32u cbp4x4_luma = m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    Ipp32s *pDeblockingFlag = &(m_deblockingParams.DeblockingFlag[dir]);

    //
    // external edge
    //
    if (m_deblockingParams.ExternalEdgeFlag[dir])
    {
        Ipp32s nNeighbour;

        // select neighbour addres
        nNeighbour = m_deblockingParams.nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((m_gmbinfo->mbs + nNeighbour)->mbtype))
        {
            H264DecoderMacroblockLocalInfo *pNeighbour;
            Ipp32s idx;
            size_t iRefQFrw[2], iRefQBck[2];
            H264DecoderMotionVector *(pVectorQFrw[2]), *(pVectorQBck[2]);

            // in following calculations we avoided multiplication on 15
            // by using formulae a * 15 = a * 16 - a

            // load reference indexes for current block
            for (idx = 0;idx < 2;idx += 1)
            {
                // field coded image
                if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264DecoderFrame **pRefPicList;
                    Ipp32s index;
                    ReferenceFlags *pFields;

                    // select reference index for current block
                    index = m_cur_mb.GetReferenceIndex(0, idx * 16 - idx);
                    if (0 <= index)
                    {
                        pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                        // select reference fields number array
                        pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_Flags;
                        iRefQFrw[idx] = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                    }
                    else
                        iRefQFrw[idx] = (size_t)-1;
                    index = m_cur_mb.GetReferenceIndex(1, idx * 16 - idx);
                    if (0 <= index)
                    {
                        pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                        // select reference fields number array
                        pFields = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_Flags;
                        iRefQBck[idx] = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                    }
                    else
                        iRefQBck[idx] = (size_t)-1;
                }
                // frame coded image
                else
                {
                    H264DecoderFrame **pRefPicList;
                    Ipp32s index;

                    // select reference index for current block
                    index = m_cur_mb.GetReferenceIndex(0, idx * 16 - idx);
                    if (0 <= index)
                    {
                        pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 0)->m_RefPicList;
                        iRefQFrw[idx] = pRefPicList[index]->DeblockPicID(0);
                    }
                    else
                        iRefQFrw[idx] = (size_t)-1;
                    index = m_cur_mb.GetReferenceIndex(1, idx * 16 - idx);
                    if (0 <= index)
                    {
                        pRefPicList = m_pCurrentFrame->GetRefPicList(m_cur_mb.GlobalMacroblockInfo->slice_id, 1)->m_RefPicList;
                        iRefQBck[idx] = pRefPicList[index]->DeblockPicID(0);
                    }
                    else
                        iRefQBck[idx] = (size_t)-1;
                }

                // select current block motion vectors
                pVectorQFrw[idx] = m_cur_mb.GetMVPtr(0, idx * 16 - idx);
                pVectorQBck[idx] = m_cur_mb.GetMVPtr(1, idx * 16 - idx);
            }

            // select neighbour
            pNeighbour = m_mbinfo.mbs + nNeighbour;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32s blkQ, blkP;

                blkQ = EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp4x4_luma & blkQ) ||
                    (pNeighbour->cbp4x4_luma & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32s nNeighbourBlock;
                    size_t iRefPFrw, iRefPBck;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264DecoderFrame **pRefPicList;
                        Ipp32s index;
                        ReferenceFlags *pFields;

                        // select reference index for previous block
                        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_RefPicList;
                            // select reference fields number array
                            pFields = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_Flags;
                            iRefPFrw = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                        }
                        else
                            iRefPFrw = (size_t)-1;
                        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_RefPicList;
                            // select reference fields number array
                            pFields = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_Flags;
                            iRefPBck = pRefPicList[index]->DeblockPicID(GetReferenceField(pFields, index));
                        }
                        else
                            iRefPBck = (size_t)-1;
                    }
                    // frame coded image
                    else
                    {
                        H264DecoderFrame **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = GetReferenceIndex(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 0)->m_RefPicList;
                            iRefPFrw = pRefPicList[index]->DeblockPicID(0);
                        }
                        else
                            iRefPFrw = (size_t)-1;
                        index = GetReferenceIndex(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);
                        if (0 <= index)
                        {
                            pRefPicList = m_pCurrentFrame->GetRefPicList((m_gmbinfo->mbs + nNeighbour)->slice_id, 1)->m_RefPicList;
                            iRefPBck = pRefPicList[index]->DeblockPicID(0);
                        }
                        else
                            iRefPBck = (size_t)-1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw[idx / 2] == iRefPFrw) && (iRefQBck[idx / 2] == iRefPBck)) ||
                        ((iRefQFrw[idx / 2] == iRefPBck) && (iRefQBck[idx / 2] == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            H264DecoderMotionVector pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                            H264DecoderMotionVector pVectorPBck = GetMV(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);

                            // select previous block motion vectors
                            if (iRefQFrw[idx / 2] != iRefPFrw)
                            {
                                swapValues(pVectorPFrw, pVectorPBck);
                            }

                            // compare motion vectors
                            if (IsVectorDifferenceBig(*pVectorQFrw[idx / 2], pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                                IsVectorDifferenceBig(*pVectorQBck[idx / 2], pVectorPBck, m_deblockingParams.nMaxMVector))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            // select previous block motion vectors
                            H264DecoderMotionVector &pVectorPFrw = GetMV(m_gmbinfo, 0, nNeighbour, nNeighbourBlock);
                            H264DecoderMotionVector &pVectorPBck = GetMV(m_gmbinfo, 1, nNeighbour, nNeighbourBlock);

                            // compare motion vectors
                            if (IsVectorDifferenceBig(*pVectorQFrw[idx / 2], pVectorPFrw, m_deblockingParams.nMaxMVector) ||
                                IsVectorDifferenceBig(*pVectorQBck[idx / 2], pVectorPBck, m_deblockingParams.nMaxMVector))
                            {
                                if (IsVectorDifferenceBig(*pVectorQFrw[idx / 2], pVectorPBck, m_deblockingParams.nMaxMVector) ||
                                    IsVectorDifferenceBig(*pVectorQBck[idx / 2], pVectorPFrw, m_deblockingParams.nMaxMVector))
                                {
                                    pStrength[idx] = 1;
                                    *pDeblockingFlag = 1;
                                }
                            }
                        }
                    }
                    // when reference indexes are different
                    else
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                }
            }
        }
        // external edge required in strong filtering
        else
        {
            if ((HORIZONTAL_DEBLOCKING == dir) &&
                (m_deblockingParams.MBFieldCoded))
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    //if (dir == VERTICAL_DEBLOCKING)
     //   foo_internal(pParams, dir);
    //return;

    //
    // internal edge(s)
    //
    {
        Ipp32s idx;

        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);

        // set deblocking flag
        if (cbp4x4_luma & 0x1fffe)
            *pDeblockingFlag = 1;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32s blkQ;

            blkQ = INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp4x4_luma & blkQ)
                pStrength[idx] = 2;
        }
    }

} // void H264SegmentDecoder::PrepareDeblockingParametersBSlice8x16(Ipp32u dir)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
