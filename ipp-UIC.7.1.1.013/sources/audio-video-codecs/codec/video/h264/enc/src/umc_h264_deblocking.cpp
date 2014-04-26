/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_h264_video_encoder.h"
#include "umc_h264_deblocking.h"
#include "umc_h264_deblocking_tools.h"
#include "vm_types.h"
#include "vm_debug.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_bme.h"

// alpha table
Ipp8u ENCODER_ALPHA_TABLE_8u[52] =
{
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    4,  4,  5,  6,  7,  8,  9,  10,
    12, 13, 15, 17, 20, 22, 25, 28,
    32, 36, 40, 45, 50, 56, 63, 71,
    80, 90, 101,113,127,144,162,182,
    203,226,255,255
};

// beta table
Ipp8u ENCODER_BETA_TABLE_8u[52] =
{
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    2,  2,  2,  3,  3,  3,  3,  4,
    4,  4,  6,  6,  7,  7,  8,  8,
    9,  9,  10, 10, 11, 11, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18
};

// clipping table
Ipp8u ENCODER_CLIP_TAB_8u[52][5] =
{
    { 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 1, 1, 1},{ 0, 0, 1, 1, 1},{ 0, 1, 1, 1, 1},
    { 0, 1, 1, 1, 1},{ 0, 1, 1, 1, 1},{ 0, 1, 1, 1, 1},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 2, 3, 3},
    { 0, 1, 2, 3, 3},{ 0, 2, 2, 3, 3},{ 0, 2, 2, 4, 4},{ 0, 2, 3, 4, 4},{ 0, 2, 3, 4, 4},{ 0, 3, 3, 5, 5},{ 0, 3, 4, 6, 6},{ 0, 3, 4, 6, 6},
    { 0, 4, 5, 7, 7},{ 0, 4, 5, 8, 8},{ 0, 4, 6, 9, 9},{ 0, 5, 7,10,10},{ 0, 6, 8,11,11},{ 0, 6, 8,13,13},{ 0, 7,10,14,14},{ 0, 8,11,16,16},
    { 0, 9,12,18,18},{ 0,10,13,20,20},{ 0,11,15,23,23},{ 0,13,17,25,25}
};

// chroma scaling QP table
Ipp8u ENCODER_QP_SCALE_CR[52] =
{
    0,  1,  2,  3,  4,  5,  6,  7,
    8,  9,  10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 29, 30,
    31, 32, 32, 33, 34, 34, 35, 35,
    36, 36, 37, 37, 37, 38, 38, 38,
    39, 39, 39, 39
};

// masks for external blocks pair "coded bits"
Ipp32u ENCODER_EXTERNAL_BLOCK_MASK[NUMBER_OF_DIRECTION][2][4] =
{
    // block mask for vertical deblocking
    {
        {2 << 0, 2 << 2, 2 << 8, 2 << 10},
        {2 << 5, 2 << 7, 2 << 13,2 << 15}
    },

    // block mask for horizontal deblocking
    {
        {2 << 0, 2 << 1, 2 << 4, 2 << 5},
        {2 << 10,2 << 11,2 << 14,2 << 15}
    }
};

#define DECL(prev, cur) (2 << (prev) | 2 << (cur))
// masks for internal blocks pair "coded bits"
Ipp32u ENCODER_INTERNAL_BLOCKS_MASK[NUMBER_OF_DIRECTION][12] =
{
    // blocks pair-mask for vertical deblocking
    {
        DECL(0, 1),  DECL(2, 3),  DECL(8, 9),  DECL(10, 11),
        DECL(1, 4),  DECL(3, 6),  DECL(9, 12), DECL(11, 14),
        DECL(4, 5),  DECL(6, 7),  DECL(12, 13),DECL(14, 15)
    },

    // blocks pair-mask for horizontal deblocking
    {
        DECL(0, 2),  DECL(1, 3),  DECL(4, 6),  DECL(5, 7),
        DECL(2, 8),  DECL(3, 9),  DECL(6, 12), DECL(7, 13),
        DECL(8, 10), DECL(9, 11), DECL(12, 14),DECL(13, 15)
    }
};
#undef DECL







//////////////





// implement array of IPP optimized luma deblocking functions
#if defined(_WIN32_WCE) && defined(_M_IX86) && defined(__stdcall)
#define _IPP_STDCALL_CDECL
#undef __stdcall
#endif // defined(_WIN32_WCE) && defined(_M_IX86) && defined(__stdcall)

IppStatus FilterDeblockingLuma_VerEdge(
    Ipp8u *pSrcDst,
    Ipp32s srcDstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    return(ippiFilterDeblockingLuma_VerEdge_H264_8u_C1IR(pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS));
}

IppStatus FilterDeblockingLuma_HorEdge(
    Ipp8u *pSrcDst,
    Ipp32s srcDstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    return(ippiFilterDeblockingLuma_HorEdge_H264_8u_C1IR(pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS));
}

IppStatus (*(EncoderIppLumaDeblocking_8u[])) (Ipp8u *, Ipp32s, Ipp8u *, Ipp8u *, Ipp8u *, Ipp8u *) =
{
    &(FilterDeblockingLuma_VerEdge),
    &(FilterDeblockingLuma_HorEdge)
};

inline IppStatus encoderIppLumaDeblocking(
    Ipp32s dir,
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBetha,
    Ipp8u* pThresholds,
    Ipp8u* pBs,
    Ipp32s)
{
    return(EncoderIppLumaDeblocking_8u[dir](pSrcDst, srcdstStep, pAlpha, pBetha, pThresholds, pBs));
}

IppStatus FilterDeblockingChroma_VerEdge(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    return ippiFilterDeblockingChroma_VerEdge_H264_8u_C1IR(pSrcDst, srcdstStep, pAlpha, pBeta, pThresholds, pBS);
}

IppStatus FilterDeblockingChroma_HorEdge(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    return ippiFilterDeblockingChroma_HorEdge_H264_8u_C1IR(pSrcDst, srcdstStep, pAlpha, pBeta, pThresholds, pBS);
}

IppStatus FilterDeblockingChroma422_VerEdge(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    CONVERT_TO_8U(2, 8);
    return ippiFilterDeblockingChroma422VerEdge_H264_8u_C1IR(&info);
}

IppStatus FilterDeblockingChroma422_HorEdge(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBeta,
    Ipp8u* pThresholds,
    Ipp8u* pBS)
{
    CONVERT_TO_8U(2, 16);
    return ippiFilterDeblockingChroma422HorEdge_H264_8u_C1IR(&info);
}

// implement array of IPP optimized chroma deblocking functions
IppStatus (*(EncoderIppChromaDeblocking_8u[])) (Ipp8u *, Ipp32s, Ipp8u *, Ipp8u *, Ipp8u *, Ipp8u *) =
{
    &(FilterDeblockingChroma_VerEdge),
    &(FilterDeblockingChroma_HorEdge),
    &(FilterDeblockingChroma422_VerEdge),
    &(FilterDeblockingChroma422_HorEdge)
};

inline IppStatus encoderIppChromaDeblocking(
    Ipp32s dir,
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp8u* pAlpha,
    Ipp8u* pBetha,
    Ipp8u* pThresholds,
    Ipp8u* pBs,
    Ipp32s)
{
    return(EncoderIppChromaDeblocking_8u[dir](pSrcDst, srcdstStep, pAlpha, pBetha, pThresholds, pBs));
}

IppStatus FilterDeblockingLuma_VerEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s bit_depth)
{
    CONVERT_TO_16U(2, 16);
    return ippiFilterDeblockingLumaVerEdge_H264_16u_C1IR(&info);
}

IppStatus FilterDeblockingLuma_HorEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s bit_depth)
{
    CONVERT_TO_16U(2, 16);
    return ippiFilterDeblockingLumaHorEdge_H264_16u_C1IR(&info);
}

IppStatus (*(EncoderIppLumaDeblocking_16s[])) (Ipp16u *, Ipp32s, Ipp8u *, Ipp8u *, Ipp8u *, Ipp8u *, Ipp32s bitdepth) =
{
    &(FilterDeblockingLuma_VerEdge),
    &(FilterDeblockingLuma_HorEdge)
};

inline IppStatus encoderIppLumaDeblocking(
    Ipp32s  dir,
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBetha,
    Ipp8u*  pThresholds,
    Ipp8u*  pBs,
    Ipp32s  bitdepth)
{
    return(EncoderIppLumaDeblocking_16s[dir](pSrcDst, srcdstStep, pAlpha, pBetha, pThresholds, pBs, bitdepth));
}

IppStatus FilterDeblockingChroma_VerEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s  bit_depth)
{
    CONVERT_TO_16U(2, 8);
    return ippiFilterDeblockingChromaVerEdge_H264_16u_C1IR(&info);
}

IppStatus FilterDeblockingChroma_HorEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s  bit_depth)
{
    CONVERT_TO_16U(2, 8);
    return ippiFilterDeblockingChromaHorEdge_H264_16u_C1IR(&info);
}

IppStatus FilterDeblockingChroma422_VerEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s  bit_depth)
{
    CONVERT_TO_16U(2, 16);
    return ippiFilterDeblockingChroma422VerEdge_H264_16u_C1IR(&info);
}

IppStatus FilterDeblockingChroma422_HorEdge(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBeta,
    Ipp8u*  pThresholds,
    Ipp8u*  pBS,
    Ipp32s  bit_depth)
{
    CONVERT_TO_16U(2, 16);
    return ippiFilterDeblockingChroma422HorEdge_H264_16u_C1IR(&info);//return DeblockChroma422_hor_16u(&info);
}

IppStatus (*(EncoderIppChromaDeblocking_16s[])) (Ipp16u *, Ipp32s, Ipp8u *, Ipp8u *, Ipp8u *, Ipp8u *, Ipp32s bitdepth) =
{
    &(FilterDeblockingChroma_VerEdge),
    &(FilterDeblockingChroma_HorEdge),
    &(FilterDeblockingChroma422_VerEdge),
    &(FilterDeblockingChroma422_HorEdge)
};

inline IppStatus encoderIppChromaDeblocking(
    Ipp32s  dir,
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp8u*  pAlpha,
    Ipp8u*  pBetha,
    Ipp8u*  pThresholds,
    Ipp8u*  pBs,
    Ipp32s  bitdepth)
{
    return(EncoderIppChromaDeblocking_16s[dir](pSrcDst, srcdstStep, pAlpha, pBetha, pThresholds, pBs, bitdepth));
}

#if defined(_IPP_STDCALL_CDECL)
#undef _IPP_STDCALL_CDECL
#define __stdcall __cdecl
#endif // defined(_IPP_STDCALL_CDECL)

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_ResetDeblockingVariables(void* state, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE *pY, *pU, *pV;
    Ipp32u offset;
    Ipp32s MBYAdjust = 0;
    Ipp32u mbXOffset, mbYOffset;
    Ipp32s pic_pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels;
    Ipp32u MBAddr = pParams->nMBAddr;
    Ipp32u nCurrMB_X, nCurrMB_Y;
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice;

    if(core_enc->m_MaxSliceSize)
        curr_slice = &core_enc->m_Slices[0];
    else
        curr_slice = &core_enc->m_Slices[core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id];

    // load planes
    pY = core_enc->m_pReconstructFrame->m_pYPlane;
    pU = core_enc->m_pReconstructFrame->m_pUPlane;
    pV = core_enc->m_pReconstructFrame->m_pVPlane;
    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
    {
        if(core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index])
        {
            pY += pic_pitchPixels;
            pU += pic_pitchPixels;
            pV += pic_pitchPixels;
        }
        if (core_enc->m_field_index)
            MBYAdjust = core_enc->m_HeightInMBs;
        pic_pitchPixels *= 2;
    }

    // prepare macroblock variables
    nCurrMB_X = (MBAddr % core_enc->m_WidthInMBs);
    nCurrMB_Y = (MBAddr / core_enc->m_WidthInMBs)- MBYAdjust;
    Ipp32s chromaShiftX;
    Ipp32s chromaShiftY;
    switch(core_enc->m_PicParamSet.chroma_format_idc) {
        case 0: //MONOCHROME
            chromaShiftX = chromaShiftY = 0;
            break;
        case 2: //422
            chromaShiftX = 3;
            chromaShiftY = 4;
            break;
        case 3: //444
            chromaShiftX = chromaShiftY = 4;
            break;
        case 1: //420
        default:
            chromaShiftX = chromaShiftY = 3;
    }
    mbXOffset = nCurrMB_X * 16;
    mbYOffset = nCurrMB_Y * 16;

    Ipp32s chromaXOffset = nCurrMB_X<<chromaShiftX;
    Ipp32s chromaYOffset = nCurrMB_Y<<chromaShiftY;
    // calc plane's offsets
    offset = mbXOffset + (mbYOffset * pic_pitchPixels);
    pY += offset;
    pU += chromaXOffset + chromaYOffset * pic_pitchPixels;
    pV += chromaXOffset + chromaYOffset * pic_pitchPixels;

    // set external edge variables
    pParams->ExternalEdgeFlag[VERTICAL_DEBLOCKING] = (nCurrMB_X != 0);
    pParams->ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = (nCurrMB_Y != 0);

    if (DEBLOCK_FILTER_ON_NO_SLICE_EDGES == curr_slice->m_disable_deblocking_filter_idc)
    {
        // don't filter at slice boundaries
        if (nCurrMB_X)
        {
            if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id !=
                core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr - 1].slice_id)
                pParams->ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
        }

        if (nCurrMB_Y)
        {
            if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id !=
                core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr - core_enc->m_WidthInMBs].slice_id)
                pParams->ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
        }
    }

    // reset external edges strength
    SetEdgeStrength(pParams->Strength[VERTICAL_DEBLOCKING], 0);
    SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING], 0);

    // set neighbour addreses
    pParams->nNeighbour[VERTICAL_DEBLOCKING] = MBAddr - 1;
    pParams->nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs;

    // set deblocking flag(s)
    pParams->DeblockingFlag[VERTICAL_DEBLOCKING] = 0;
    pParams->DeblockingFlag[HORIZONTAL_DEBLOCKING] = 0;

    // save variables
    pParams->pY = pY;
    pParams->pU = pU;
    pParams->pV = pV;
    pParams->pitchPixels = pic_pitchPixels;
    pParams->nMaxMVector = (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec) ? (2) : (4);
    pParams->MBFieldCoded = (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec);

    // set slice's variables
    pParams->nAlphaC0Offset = curr_slice->m_slice_alpha_c0_offset;
    pParams->nBetaOffset = curr_slice->m_slice_beta_offset;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockISlice(void* state, Ipp32u MBAddr)
{
    __align(16)
    DeblockingParameters<PIXTYPE> params;

    // prepare deblocking parameters
    params.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariables<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersISlice(state, &params);

    // perform deblocking
    //!!!Chroma must be called first because luma clears strength for 8x8 transform.
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockPSlice(void* state, Ipp32u MBAddr)
{
    __align(16)
    DeblockingParameters<PIXTYPE> params;

    // prepare deblocking parameters
    params.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariables<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersPSlice<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform deblocking
    //!!!Chroma must be called first because luma clears strength for 8x8 transform.
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockBSlice(void* state, Ipp32u MBAddr)
{
    __align(16)
    DeblockingParameters<PIXTYPE> params;

    // prepare deblocking parameters
    params.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariables<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersBSlice<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform deblocking
    //!!!Chroma must be called first because luma clears strength for 8x8 transform.
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params);
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockSlice(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32u uFirstMB, Ipp32u unumMBs)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CoreEncoder_DeblockingFunction pDeblocking = NULL;

    // no filtering edges of this slice
    if (DEBLOCK_FILTER_OFF == curr_slice->m_disable_deblocking_filter_idc)
        return;

    // MBAFF case
    if (core_enc->m_SliceHeader.MbaffFrameFlag)
    {
        // select optimized deblocking function
        switch (curr_slice->m_slice_type)
        {
        case INTRASLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockISliceMBAFF<COEFFSTYPE, PIXTYPE>;
            break;

        case PREDSLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockPSliceMBAFF<COEFFSTYPE, PIXTYPE>;
            break;

        case BPREDSLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockBSliceMBAFF<COEFFSTYPE, PIXTYPE>;
            break;

        default:
            pDeblocking = NULL;
            break;
        }
    }
    // non-MBAFF case
    else
    {
        // select optimized deblocking function
        switch (curr_slice->m_slice_type)
        {
        case INTRASLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockISlice<COEFFSTYPE, PIXTYPE>;
            break;

        case PREDSLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockPSlice<COEFFSTYPE, PIXTYPE>;
            break;

        case BPREDSLICE:
            pDeblocking = &H264CoreEncoder_DeblockMacroblockBSlice<COEFFSTYPE, PIXTYPE>;
            break;

        default:
            pDeblocking = NULL;
            break;
        }
    }

    if (NULL == pDeblocking)
        return;

   // perform deblocking process on every macroblock
   Ipp32u i;
   for (i = uFirstMB; i < uFirstMB + unumMBs; i++)
       (*pDeblocking)(state, i);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockLuma(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE *pY = pParams->pY;
    Ipp32s pic_pitchPixels = pParams->pitchPixels;
    Ipp32s MBAddr = pParams->nMBAddr;
    Ipp8u Clipping[16];
    Ipp8u Alpha[2];
    Ipp8u Beta[2];
    Ipp32s AlphaC0Offset = pParams->nAlphaC0Offset;
    Ipp32s BetaOffset = pParams->nBetaOffset;
    Ipp32s pmq_QP = core_enc->m_mbinfo.mbs[MBAddr].QP;

    //
    // luma deblocking
    //

    if (pParams->DeblockingFlag[dir])
    {
        Ipp8u *pClipTab;
        Ipp32s QP;
        Ipp32s index;
        Ipp8u *pStrength = pParams->Strength[dir];

        //
        // correct strengths for high profile
        //
        if (pGetMB8x8TSFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr))
        {
            SetEdgeStrength(pStrength + 4, 0); // 16 bits????
            SetEdgeStrength(pStrength + 12, 0);
        }

        if (pParams->ExternalEdgeFlag[dir])
        {
            Ipp32s pmp_QP;

            // get neighbour block QP
            pmp_QP = core_enc->m_mbinfo.mbs[pParams->nNeighbour[dir]].QP;

            // luma variables
            QP = (pmp_QP + pmq_QP + 1) >> 1 ;

            // external edge variables
            index = IClip(0, 51, QP + BetaOffset);
            Beta[0] = getEncoderBethaTable(index);

            index = IClip(0, 51, QP + AlphaC0Offset);
            Alpha[0] = getEncoderAlphaTable(index);
            pClipTab = getEncoderClipTab(index);

            // create clipping values
            Clipping[0] = pClipTab[pStrength[0]];
            Clipping[1] = pClipTab[pStrength[1]];
            Clipping[2] = pClipTab[pStrength[2]];
            Clipping[3] = pClipTab[pStrength[3]];

        }

        // internal edge variables
        QP = pmq_QP;

        index = IClip(0, 51, QP + BetaOffset);
        Beta[1] = getEncoderBethaTable(index);

        index = IClip(0, 51, QP + AlphaC0Offset);
        Alpha[1] = getEncoderAlphaTable(index);
        pClipTab = getEncoderClipTab(index);

        // create clipping values
        {
            Ipp32u edge;

            for (edge = 1;edge < 4;edge += 1)
            {
                if (*((Ipp32u *) (pStrength + edge * 4)))
                {
                    // create clipping values
                    Clipping[edge * 4 + 0] = pClipTab[pStrength[edge * 4 + 0]];
                    Clipping[edge * 4 + 1] = pClipTab[pStrength[edge * 4 + 1]];
                    Clipping[edge * 4 + 2] = pClipTab[pStrength[edge * 4 + 2]];
                    Clipping[edge * 4 + 3] = pClipTab[pStrength[edge * 4 + 3]];
                }
            }
        }

        // perform deblocking
        encoderIppLumaDeblocking(
            dir,
            pY,
            pic_pitchPixels,
            Alpha,
            Beta,
            Clipping,
            pStrength,
            core_enc->m_PicParamSet.bit_depth_luma);
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockChroma(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;

    // do no deblocking of 4:0:0 format
    if (core_enc->m_PicParamSet.chroma_format_idc)
    {
        PIXTYPE *pU = pParams->pU;
        PIXTYPE *pV = pParams->pV;
        Ipp32s pic_pitchPixels = pParams->pitchPixels;
        Ipp32s MBAddr = pParams->nMBAddr;
        Ipp8u Clipping[16];
        Ipp8u Alpha[2];
        Ipp8u Beta[2];
        Ipp32s AlphaC0Offset = pParams->nAlphaC0Offset;
        Ipp32s BetaOffset = pParams->nBetaOffset;
        Ipp32s pmq_QP = core_enc->m_mbinfo.mbs[MBAddr].QP;

        //
        // chroma deblocking
        //

        if (pParams->DeblockingFlag[dir])

        {
            Ipp8u *pClipTab;
            Ipp32s QP;
            Ipp32s index;
            Ipp8u *pStrength = pParams->Strength[dir];
            Ipp32s nPlane;

            for (nPlane = 0; nPlane < 2; nPlane += 1)
            {
                if ((0 == nPlane) || (core_enc->m_PicParamSet.chroma_qp_index_offset != core_enc->m_PicParamSet.second_chroma_qp_index_offset))
                {
                    Ipp32s chroma_qp_offset = (0 == nPlane) ? (core_enc->m_PicParamSet.chroma_qp_index_offset) : (core_enc->m_PicParamSet.second_chroma_qp_index_offset);

                    if (pParams->ExternalEdgeFlag[dir])
                    {
                        Ipp32s pmp_QP;

                        // get left block QP
                        pmp_QP = core_enc->m_mbinfo.mbs[pParams->nNeighbour[dir]].QP;

                        // external edge variables averaging???
                        //QP = ENCODER_QP_SCALE_CR[getChromaQP(pmp_QP, chroma_qp_offset, core_enc->m_SeqParamSet.bit_depth_chroma)];
                        QP = ENCODER_QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];

                        index = IClip(0, 51, QP + BetaOffset);
                        Beta[0] = getEncoderBethaTable(index);

                        index = IClip(0, 51, QP + AlphaC0Offset);
                        Alpha[0] = getEncoderAlphaTable(index);
                        pClipTab = getEncoderClipTab(index);

                        // create clipping values
                        Clipping[0] = pClipTab[pStrength[0]];
                        Clipping[1] = pClipTab[pStrength[1]];
                        Clipping[2] = pClipTab[pStrength[2]];
                        Clipping[3] = pClipTab[pStrength[3]];
                    }

                    // internal edge variables
                    QP = ENCODER_QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];
                    //QP = ENCODER_QP_SCALE_CR[getChromaQP(pmq_QP, chroma_qp_offset, core_enc->m_SeqParamSet.bit_depth_chroma)];

                    index = IClip(0, 51, QP + BetaOffset);
                    Beta[1] = getEncoderBethaTable(index);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    Alpha[1] = getEncoderAlphaTable(index);
                    pClipTab = getEncoderClipTab(index);


                    // create clipping values
                    if (core_enc->m_PicParamSet.chroma_format_idc == 2 && HORIZONTAL_DEBLOCKING == dir)
                    {
                        // create clipping values

                        Clipping[4] = (Ipp8u) (pClipTab[pStrength[4]]);
                        Clipping[5] = (Ipp8u) (pClipTab[pStrength[5]]);
                        Clipping[6] = (Ipp8u) (pClipTab[pStrength[6]]);
                        Clipping[7] = (Ipp8u) (pClipTab[pStrength[7]]);
                        Clipping[8] = (Ipp8u) (pClipTab[pStrength[8]]);
                        Clipping[9] = (Ipp8u) (pClipTab[pStrength[9]]);
                        Clipping[10] = (Ipp8u) (pClipTab[pStrength[10]]);
                        Clipping[11] = (Ipp8u) (pClipTab[pStrength[11]]);
                        Clipping[12] = (Ipp8u) (pClipTab[pStrength[12]]);
                        Clipping[13] = (Ipp8u) (pClipTab[pStrength[13]]);
                        Clipping[14] = (Ipp8u) (pClipTab[pStrength[14]]);
                        Clipping[15] = (Ipp8u) (pClipTab[pStrength[15]]);
                    }
                    else
                    {
                        // create clipping values
                        Clipping[4] = (Ipp8u) (pClipTab[pStrength[8]]);
                        Clipping[5] = (Ipp8u) (pClipTab[pStrength[9]]);
                        Clipping[6] = (Ipp8u) (pClipTab[pStrength[10]]);
                        Clipping[7] = (Ipp8u) (pClipTab[pStrength[11]]);
                    }
                }
                // perform deblocking chroma component
                encoderIppChromaDeblocking(
                    dir + (core_enc->m_PicParamSet.chroma_format_idc & 0x2),
                    (0 == nPlane) ? (pU) : (pV),
                    pic_pitchPixels,
                    Alpha,
                    Beta,
                    Clipping,
                    pStrength,
                    core_enc->m_SeqParamSet.bit_depth_chroma);
            }
        }
    }
}

template<typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersISlice(
    void*,
    DeblockingParameters<PIXTYPE> *pParams)
{
    // set deblocking flag(s)
    pParams->DeblockingFlag[VERTICAL_DEBLOCKING] = 1;
    pParams->DeblockingFlag[HORIZONTAL_DEBLOCKING] = 1;

    // calculate strengths
    if (pParams->ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        // deblocking with strong deblocking of external edge
        SetEdgeStrength(pParams->Strength[VERTICAL_DEBLOCKING] + 0, 4);
    }

    SetEdgeStrength(pParams->Strength[VERTICAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(pParams->Strength[VERTICAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(pParams->Strength[VERTICAL_DEBLOCKING] + 12, 3);

    if (pParams->ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
    {
        if (pParams->MBFieldCoded)
        {
            // deblocking field macroblock with external edge
            SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING] + 0, 3);
        }
        else
        {
            // deblocking with strong deblocking of external edge
            SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING] + 0, 4);
        }
    }

    SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(pParams->Strength[HORIZONTAL_DEBLOCKING] + 12, 3);

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice16(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;
            Ipp32s iRefQ;
            H264MotionVector *pVectorQ;

            // load reference index & motion vector for current block
            {
                // field coded image
                if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;
                    Ipp8s *pFields;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        // select reference fields number array
                        pFields = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_Prediction;
                        iRefQ = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[index],
                                pFields[index]));
                    }
                    else
                        iRefQ = -1;
                }
                // frame coded image
                else
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        iRefQ = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            0);
                    }
                    else
                        iRefQ = -1;
                }

                pVectorQ = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors;
            }

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nNeighbourBlock;
                    Ipp32s iRefP;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefP = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefP = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefP = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefP = -1;
                    }

                    VM_ASSERT((iRefP != -1) || (iRefQ != -1));

                    // when reference indexes are equal
                    if (iRefQ == iRefP)
                    {
                        H264MotionVector *pVectorP;

                        pVectorP = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;

                        // compare motion vectors
                        if ((4 <= abs(pVectorQ->mvx - pVectorP->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQ->mvy - pVectorP->mvy)))
                        {
                            pStrength[idx] = 1;
                            *pDeblockingFlag = 1;
                        }
                        else
                            pStrength[idx] = 0;
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);

        // set deblocking flag
        if (cbp_luma & 0x1fffe)
            *pDeblockingFlag = 1;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
                pStrength[idx] = 2;
        }
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice(void* state, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
    Ipp32u mbtype = (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->mbtype;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        H264CoreEncoder_PrepareDeblockingParametersISlice(state, pParams);
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
        H264CoreEncoder_PrepareDeblockingParametersPSlice16<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersPSlice16<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        break;
/*
        // when macroblock has type inter 16 on 8
    case MBTYPE_INTER_16x8:
        H264CoreEncoder_PrepareDeblockingParametersPSlice8x16(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersPSlice16x8(state, HORIZONTAL_DEBLOCKING, pParams);
        return;

        // when macroblock has type inter 8 on 16
    case MBTYPE_INTER_8x16:
        H264CoreEncoder_PrepareDeblockingParametersPSlice16x8(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersPSlice8x16(state, HORIZONTAL_DEBLOCKING, pParams);
        return;
*/
    default:
        H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        break;
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //

    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nBlock, nNeighbourBlock;
                    Ipp32s iRefQ, iRefP;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                    {
                        nBlock = idx * 4;
                        nNeighbourBlock = idx * 4 + 3;
                    }
                    else
                    {
                        nBlock = idx;
                        nNeighbourBlock = idx + 12;
                    }

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefQ = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefQ = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefP = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefP = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQ = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefQ = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefP = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefP = -1;
                    }

                    VM_ASSERT((iRefP != -1) || (iRefQ != -1));

                    // when reference indexes are equal
                    if (iRefQ == iRefP)
                    {
                        H264MotionVector *pVectorQ, *pVectorP;

                        pVectorQ = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorP = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;

                        // compare motion vectors
                        if ((4 <= abs(pVectorQ->mvx - pVectorP->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQ->mvy - pVectorP->mvy)))
                        {
                            pStrength[idx] = 1;
                            *pDeblockingFlag = 1;
                        }
                        else
                            pStrength[idx] = 0;
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // compare motion vectors & reference indexes
            else
            {
                Ipp32u nBlock, nNeighbourBlock;
                Ipp32s iRefQ, iRefP;
                Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

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

                VM_ASSERT(-1 == core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock]);
                VM_ASSERT(-1 == core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nNeighbourBlock]);

                // field coded image
                if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;
                    Ipp8s *pFields;

                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;

                    // select reference fields number array
                    pFields = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_Prediction;

                    // select reference field for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQ = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));

                    // select reference field for previous block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefP = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                }
                // frame coded image
                else
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQ = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);

                    // select reference index for previous block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefP = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                }

                VM_ASSERT((iRefP != -1) || (iRefQ != -1));

                // when reference indexes are equal
                if (iRefQ == iRefP)
                {
                    H264MotionVector *pVectorQ, *pVectorP;

                    pVectorQ = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                    pVectorP = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;

                    // compare motion vectors
                    if ((4 <= abs(pVectorQ->mvx - pVectorP->mvx)) ||
                        (nVectorDiffLimit <= abs(pVectorQ->mvy - pVectorP->mvy)))
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                    else
                        pStrength[idx] = 0;
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

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice16(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;
            Ipp32s iRefQFrw, iRefQBck;
            const H264MotionVector *pVectorQFrw, *pVectorQBck;

            // load reference indexes for current block
            {
                // field coded image
                if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;
                    Ipp8s *pFields;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        // select reference fields number array
                        pFields = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_Prediction;
                        iRefQFrw = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[index],
                                pFields[index]));
                    }
                    else
                        iRefQFrw = -1;
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        // select reference fields number array
                        pFields = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_Prediction;
                        iRefQBck = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[index],
                                pFields[index]));
                    }
                    else
                        iRefQBck = -1;
                }
                // frame coded image
                else
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        iRefQFrw = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            0);
                    }
                    else
                        iRefQFrw = -1;
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[0];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        iRefQBck = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            0);
                    }
                    else
                        iRefQBck = -1;
                }

                // select current block motion vectors
                pVectorQFrw = (iRefQFrw < 0)? &null_mv : core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors;
                pVectorQBck = (iRefQBck < 0)? &null_mv : core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors;
            }

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nNeighbourBlock;
                    Ipp32s iRefPFrw, iRefPBck;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPBck = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPBck = -1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            if (iRefQFrw == iRefPFrw)
                            {
                                pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                            }
                            else
                            {
                                pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            }

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                    (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);

        // set deblocking flag
        if (cbp_luma & 0x1fffe)
            *pDeblockingFlag = 1;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
                pStrength[idx] = 2;
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice16x8(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);
    Ipp32s iRefQFrw, iRefQBck;
    const H264MotionVector *pVectorQFrw, *pVectorQBck;

    //
    // external edge
    //

    // load reference indexes & motion vector for first half of current block
    {
        // field coded image
        if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
        {
            H264EncoderFrame<PIXTYPE> **pRefPicList;
            Ipp32s index;
            Ipp8s *pFields;

            // select reference index for current block
            index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
            if (0 <= index)
            {
                pRefPicList = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    0,
                    core_enc->m_MaxSliceSize)->m_RefPicList;
                // select reference fields number array
                pFields = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    0,
                    core_enc->m_MaxSliceSize)->m_Prediction;
                iRefQFrw = H264EncoderFrame_DeblockPicID(
                    pRefPicList[index],
                    H264EncoderFrame_GetNumberByParity(
                        pRefPicList[index],
                        pFields[index]));
            }
            else
                iRefQFrw = -1;
            index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[0];
            if (0 <= index)
            {
                pRefPicList = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    1,
                    core_enc->m_MaxSliceSize)->m_RefPicList;
                // select reference fields number array
                pFields = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    1,
                    core_enc->m_MaxSliceSize)->m_Prediction;
                iRefQBck = H264EncoderFrame_DeblockPicID(
                    pRefPicList[index],
                    H264EncoderFrame_GetNumberByParity(
                        pRefPicList[index],
                        pFields[index]));
            }
            else
                iRefQBck = -1;
        }
        // frame coded image
        else
        {
            H264EncoderFrame<PIXTYPE> **pRefPicList;
            Ipp32s index;

            // select reference index for current block
            index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[0];
            if (0 <= index)
            {
                pRefPicList = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    0,
                    core_enc->m_MaxSliceSize)->m_RefPicList;
                iRefQFrw = H264EncoderFrame_DeblockPicID(
                    pRefPicList[index],
                    0);
            }
            else
                iRefQFrw = -1;
            index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[0];
            if (0 <= index)
            {
                pRefPicList = H264EncoderFrame_GetRefPicList(
                    core_enc->m_pCurrentFrame,
                    (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                    1,
                    core_enc->m_MaxSliceSize)->m_RefPicList;
                iRefQBck = H264EncoderFrame_DeblockPicID(
                    pRefPicList[index],
                    0);
            }
            else
                iRefQBck = -1;
        }

        // select current block motion vectors
        pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors;
        pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors;
    }

    // prepare deblocking parameter for external edge
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nNeighbourBlock;
                    Ipp32s iRefPFrw, iRefPBck;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPBck = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPBck = -1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            if (iRefQFrw == iRefPFrw)
                            {
                                pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                            }
                            else
                            {
                                pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            }

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                    (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // cicle of edge(s)
        for (idx = 4;idx < 8;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
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
            Ipp32s iRefQFrw2, iRefQBck2;
            Ipp32u nStrength;
            Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

            // load reference indexes for current block

            // field coded image
            if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
            {
                H264EncoderFrame<PIXTYPE> **pRefPicList;
                Ipp32s index;
                Ipp8s *pFields;

                // select reference index for current block
                index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[15];
                if (0 <= index)
                {
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select reference fields number array
                    pFields = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_Prediction;
                    iRefQFrw2 = H264EncoderFrame_DeblockPicID(
                        pRefPicList[index],
                        H264EncoderFrame_GetNumberByParity(
                            pRefPicList[index],
                            pFields[index]));
                }
                else
                    iRefQFrw2 = -1;
                index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[15];
                if (0 <= index)
                {
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select reference fields number array
                    pFields = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_Prediction;
                    iRefQBck2 = H264EncoderFrame_DeblockPicID(
                        pRefPicList[index],
                        H264EncoderFrame_GetNumberByParity(
                            pRefPicList[index],
                            pFields[index]));
                }
                else
                    iRefQBck2 = -1;
            }
            // frame coded image
            else
            {
                H264EncoderFrame<PIXTYPE> **pRefPicList;
                Ipp32s index;

                // select reference index for current block
                index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[15];
                if (0 <= index)
                {
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    iRefQFrw2 = H264EncoderFrame_DeblockPicID(
                        pRefPicList[index],
                        0);
                }
                else
                    iRefQFrw2 = -1;
                index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[15];
                if (0 <= index)
                {
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    iRefQBck2 = H264EncoderFrame_DeblockPicID(
                        pRefPicList[index],
                        0);
                }
                else
                    iRefQBck2 = -1;
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
                    const H264MotionVector *pVectorQFrw2, *pVectorQBck2;

                    // select previous block motion vectors
                    if (iRefQFrw == iRefQFrw2)
                    {
                        pVectorQFrw2 = (iRefQFrw2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + 15;
                        pVectorQBck2 = (iRefQBck2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + 15;
                    }
                    else
                    {
                        pVectorQFrw2 = (iRefQBck2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + 15;
                        pVectorQBck2 = (iRefQFrw2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + 15;
                    }

                    // compare motion vectors
                    if ((4 <= abs(pVectorQFrw->mvx - pVectorQFrw2->mvx)) ||
                        (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorQFrw2->mvy)) ||
                        (4 <= abs(pVectorQBck->mvx - pVectorQBck2->mvx)) ||
                        (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorQBck2->mvy)))
                    {
                        nStrength = 1;
                        *pDeblockingFlag = 1;
                    }
                }
                // when forward and backward reference pictures of previous block are equal
                else
                {
                    const H264MotionVector *pVectorQFrw2, *pVectorQBck2;

                    // select block second motion vectors
                    pVectorQFrw2 = (iRefQFrw2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + 15;
                    pVectorQBck2 = (iRefQBck2 < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + 15;

                    // compare motion vectors
                    if ((4 <= abs(pVectorQFrw->mvx - pVectorQFrw2->mvx)) ||
                        (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorQFrw2->mvy)) ||
                        (4 <= abs(pVectorQBck->mvx - pVectorQBck2->mvx)) ||
                        (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorQBck2->mvy)))
                    {
                        if ((4 <= abs(pVectorQFrw->mvx - pVectorQBck2->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorQBck2->mvy)) ||
                            (4 <= abs(pVectorQBck->mvx - pVectorQFrw2->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorQFrw2->mvy)))
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
                Ipp32u blkQ;

                blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

                if (cbp_luma & blkQ)
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
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // we haven't to compare motion vectors  - they are equal
            else
                pStrength[idx] = 0;
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice8x16(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;
            Ipp32s iRefQFrw[2], iRefQBck[2];
            const H264MotionVector *(pVectorQFrw[2]), *(pVectorQBck[2]);

            // in following calculations we avoided multiplication on 15
            // by using formulae a * 15 = a * 16 - a

            // load reference indexes for current block
            for (idx = 0;idx < 2;idx += 1)
            {
                // field coded image
                if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;
                    Ipp8s *pFields;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[idx * 16 - idx];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        // select reference fields number array
                        pFields = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_Prediction;
                        iRefQFrw[idx] = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[index],
                                pFields[index]));
                    }
                    else
                        iRefQFrw[idx] = -1;
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[idx * 16 - idx];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        // select reference fields number array
                        pFields = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_Prediction;
                        iRefQBck[idx] = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[index],
                                pFields[index]));
                    }
                    else
                        iRefQBck[idx] = -1;
                }
                // frame coded image
                else
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[idx * 16 - idx];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            0,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        iRefQFrw[idx] = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            0);
                    }
                    else
                        iRefQFrw[idx] = -1;
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[idx * 16 - idx];
                    if (0 <= index)
                    {
                        pRefPicList = H264EncoderFrame_GetRefPicList(
                            core_enc->m_pCurrentFrame,
                            (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                            1,
                            core_enc->m_MaxSliceSize)->m_RefPicList;
                        iRefQBck[idx] = H264EncoderFrame_DeblockPicID(
                            pRefPicList[index],
                            0);
                    }
                    else
                        iRefQBck[idx] = -1;
                }

                // select current block motion vectors
                pVectorQFrw[idx] = (iRefQFrw[idx] < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + (idx * 16 - idx);
                pVectorQBck[idx] = (iRefQBck[idx] < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + (idx * 16 - idx);
            }

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nNeighbourBlock;
                    Ipp32s iRefPFrw, iRefPBck;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                        nNeighbourBlock = idx * 4 + 3;
                    else
                        nNeighbourBlock = idx + 12;

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPBck = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPBck = -1;
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
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            if (iRefQFrw[idx / 2] == iRefPFrw)
                            {
                                pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                            }
                            else
                            {
                                pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            }

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw[idx / 2]->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw[idx / 2]->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck[idx / 2]->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck[idx / 2]->mvy - pVectorPBck->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select previous block motion vectors
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw[idx / 2]->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw[idx / 2]->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck[idx / 2]->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck[idx / 2]->mvy - pVectorPBck->mvy)))
                            {
                                if ((4 <= abs(pVectorQFrw[idx / 2]->mvx - pVectorPBck->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQFrw[idx / 2]->mvy - pVectorPBck->mvy)) ||
                                    (4 <= abs(pVectorQBck[idx / 2]->mvx - pVectorPFrw->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQBck[idx / 2]->mvy - pVectorPFrw->mvy)))
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // reset all strengths
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 8, 0);
        SetEdgeStrength(pStrength + 12, 0);

        // set deblocking flag
        if (cbp_luma & 0x1fffe)
            *pDeblockingFlag = 1;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
                pStrength[idx] = 2;
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice(void* state, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
    Ipp32u mbtype = (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->mbtype;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        H264CoreEncoder_PrepareDeblockingParametersISlice(state, pParams);
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
        H264CoreEncoder_PrepareDeblockingParametersBSlice16<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersBSlice16<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        break;

        // when macroblock has type inter 16 on 8
    case MBTYPE_INTER_16x8:
    case MBTYPE_FWD_FWD_16x8:
    case MBTYPE_BWD_BWD_16x8:
    case MBTYPE_FWD_BWD_16x8:
    case MBTYPE_BWD_FWD_16x8:
    case MBTYPE_BIDIR_FWD_16x8:
    case MBTYPE_BIDIR_BWD_16x8:
    case MBTYPE_FWD_BIDIR_16x8:
    case MBTYPE_BWD_BIDIR_16x8:
    case MBTYPE_BIDIR_BIDIR_16x8:
        H264CoreEncoder_PrepareDeblockingParametersBSlice8x16<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersBSlice16x8<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        return;

        // when macroblock has type inter 8 on 16
    case MBTYPE_INTER_8x16:
    case MBTYPE_FWD_FWD_8x16:
    case MBTYPE_BWD_BWD_8x16:
    case MBTYPE_FWD_BWD_8x16:
    case MBTYPE_BWD_FWD_8x16:
    case MBTYPE_BIDIR_FWD_8x16:
    case MBTYPE_BIDIR_BWD_8x16:
    case MBTYPE_FWD_BIDIR_8x16:
    case MBTYPE_BWD_BIDIR_8x16:
    case MBTYPE_BIDIR_BIDIR_8x16:
        H264CoreEncoder_PrepareDeblockingParametersBSlice16x8<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersBSlice8x16<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        return;

    default:
        H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, pParams);
        H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, pParams);
        break;
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice4(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nBlock, nNeighbourBlock;
                    Ipp32s iRefQFrw, iRefPFrw, iRefQBck, iRefPBck;
                    Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

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

                    // field coded image
                    if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;
                        Ipp8s *pFields;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefQFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefQFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefQBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefQBck = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            // select reference fields number array
                            pFields = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_Prediction;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                        }
                        else
                            iRefPBck = -1;
                    }
                    // frame coded image
                    else
                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefQFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefQBck = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                        }
                        else
                            iRefPBck = -1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            const H264MotionVector *pVectorQFrw, *pVectorQBck;
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select current block motion vectors
                            pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                            pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                            // select previous block motion vectors
                            if (iRefQFrw == iRefPFrw)
                            {
                                pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                            }
                            else
                            {
                                pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            }

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            const H264MotionVector *pVectorQFrw, *pVectorQBck;
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select current block motion vectors
                            pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                            pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                            // select previous block motion vectors
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                    (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                    (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
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
                (pParams->MBFieldCoded))
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
        Ipp32u idx;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // compare motion vectors & reference indexes
            else
            {
                Ipp32u nBlock, nNeighbourBlock;
                Ipp32s iRefQFrw, iRefQBck, iRefPFrw, iRefPBck;
                Ipp32s nVectorDiffLimit = pParams->nMaxMVector;

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
                if (FRM_STRUCTURE > core_enc->m_pCurrentFrame->m_PictureStructureForDec)
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;
                    Ipp8s *pFields;

                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select reference fields number array
                    pFields = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_Prediction;

                    // select forward reference index for blocks
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));

                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select reference fields number array
                    pFields = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_Prediction;

                    // select backward reference index for blocks
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                    iRefQBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                H264EncoderFrame_GetNumberByParity(
                                    pRefPicList[index],
                                    pFields[index]));

                }
                // frame coded image
                else
                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    // select forward reference pictures list
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select forward reference index for block(s)
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);

                    // select backward reference pictures list
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select backward reference index for block(s)
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                    iRefQBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index],
                                0);
                }

                // when reference indexes are equal
                if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                    ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                {
                    // set initial value of strength
                    pStrength[idx] = 0;

                    // when forward and backward reference pictures of previous block are different
                    if (iRefPFrw != iRefPBck)
                    {
                        const H264MotionVector *pVectorQFrw, *pVectorQBck;
                        const H264MotionVector *pVectorPFrw, *pVectorPBck;

                        // select current block motion vectors
                        pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                        // select previous block motion vectors
                        if (iRefQFrw == iRefPFrw)
                        {
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;
                        }
                        else
                        {
                            pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                        }

                        // compare motion vectors
                        if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                            (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                        {
                            pStrength[idx] = 1;
                            *pDeblockingFlag = 1;
                        }
                    }
                    // when forward and backward reference pictures of previous block are equal
                    else
                    {
                        const H264MotionVector *pVectorQFrw, *pVectorQBck;
                        const H264MotionVector *pVectorPFrw, *pVectorPBck;

                        // select current block motion vectors
                        pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                        // select previous block motion vectors
                        pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                        pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;

                        // compare motion vectors
                        if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                            (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                            (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                        {
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                (nVectorDiffLimit <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
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
}


// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template void H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_DeblockSlice<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u);
template void H264CoreEncoder_PrepareDeblockingParametersPSlice<COEFFSTYPE, PIXTYPE>(void*, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersBSlice<COEFFSTYPE, PIXTYPE>(void*, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template void H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_DeblockSlice<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u);
template void H264CoreEncoder_PrepareDeblockingParametersPSlice<COEFFSTYPE, PIXTYPE>(void*, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersBSlice<COEFFSTYPE, PIXTYPE>(void*, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);
template void H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(void*, Ipp32u, DeblockingParameters<PIXTYPE>*);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER

