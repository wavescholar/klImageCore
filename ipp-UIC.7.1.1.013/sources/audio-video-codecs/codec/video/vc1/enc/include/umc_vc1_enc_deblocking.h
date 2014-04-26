/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, functions for deblocking
//
*/

#ifndef __UMC_VC1_ENC_DEBLOCKING_H__
#define __UMC_VC1_ENC_DEBLOCKING_H__

#include "umc_vc1_enc_mb.h"
#include "umc_vc1_common_defs.h"
namespace UMC_VC1_ENCODER
{

void GetInternalBlockEdge(VC1EncoderMBInfo *pCur,
                          Ipp8u& YFlagUp, Ipp8u& YFlagBot, Ipp8u& UFlagH, Ipp8u& VFlagH,
                          Ipp8u& YFlagL,  Ipp8u& YFlagR,   Ipp8u& UFlagV, Ipp8u& VFlagV);

typedef void (*fGetExternalEdge)(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer,
                                Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag);
typedef void (*fGetInternalEdge)(VC1EncoderMBInfo *pCur, Ipp8u& YFlagV, Ipp8u& YFlagH);

extern fGetExternalEdge GetExternalEdge[2][2]; //4 MV, VTS
extern fGetInternalEdge GetInternalEdge[2][2]; //4 MV, VTS

extern fGetExternalEdge GetFieldExternalEdge[2][2]; //4 MV, VTS

extern fGetExternalEdge GetExternalEdge_SM[2][2]; //4 MV, VTS
extern fGetInternalEdge GetInternalEdge_SM[2][2]; //4 MV, VTS

//-----------------------Deblocking I frames-----------------------------------------
inline void Deblock_I_LumaMB(Ipp8u* pY, Ipp32s YStep, Ipp32s quant)
{
    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock top MB internal edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock top MB/current MB edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top left MB/top MB edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_LumaLeftMB(Ipp8u* pY, Ipp32s YStep, Ipp32s quant)
{
    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock top MB internal edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock top MB/current MB edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_LumaLeftBottomMB(Ipp8u* pY, Ipp32s YStep, Ipp32s quant)
{
    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock top MB internal edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock top MB/current MB edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock current MB internal edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical current MB internal edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_LumaBottomMB(Ipp8u* pY, Ipp32s YStep, Ipp32s quant)
{
    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock top MB internal edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock top MB/current MB edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //horizontal deblock current MB internal edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top left MB/top MB edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical left MB/current MB edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

    //vertical current MB internal edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, 0);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_ChromaMB(Ipp8u* pU, Ipp32s UStep, Ipp32s quant)
{
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock top MB/current MB edge
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);

   //vertical top left MB/top MB edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_ChromaLeftMB(Ipp8u* pU, Ipp32s UStep, Ipp32s quant)
{
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock top MB/current MB edge
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_ChromaBottomMB(Ipp8u* pU, Ipp32s UStep, Ipp32s quant)
{
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock top MB/current MB edge
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);

   //vertical top left MB/top MB edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);

   //vertical left MB/current MB edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, 0);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_I_FrameRow(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width, Ipp32u quant)
{
    Ipp8u* pYMBDebl = pPlanes[0];
    Ipp8u* pUMBDebl = pPlanes[1];
    Ipp8u* pVMBDebl = pPlanes[2];
    Ipp32u i = 1;

    Deblock_I_LumaLeftMB  (pYMBDebl, step[0], quant);
    Deblock_I_ChromaLeftMB(pUMBDebl, step[1], quant);
    Deblock_I_ChromaLeftMB(pVMBDebl, step[2], quant);

    for(i = 1; i < width; i++)
    {
        pYMBDebl += VC1_ENC_LUMA_SIZE;
        pUMBDebl += VC1_ENC_CHROMA_SIZE;
        pVMBDebl += VC1_ENC_CHROMA_SIZE;

        Deblock_I_LumaMB  (pYMBDebl, step[0], quant);
        Deblock_I_ChromaMB(pUMBDebl, step[1], quant);
        Deblock_I_ChromaMB(pVMBDebl, step[2], quant);
    }
}
inline void Deblock_I_FrameBottomRow(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width, Ipp32u quant)
{
    Ipp8u* pYMBDebl = pPlanes[0];
    Ipp8u* pUMBDebl = pPlanes[1];
    Ipp8u* pVMBDebl = pPlanes[2];
    Ipp32u i = 1;

    Deblock_I_LumaLeftBottomMB(pYMBDebl, step[0], quant);
    Deblock_I_ChromaLeftMB    (pUMBDebl, step[1], quant);
    Deblock_I_ChromaLeftMB    (pVMBDebl, step[2], quant);

    for(i = 1; i < width; i++)
    {
        pYMBDebl += VC1_ENC_LUMA_SIZE;
        pUMBDebl += VC1_ENC_CHROMA_SIZE;
        pVMBDebl += VC1_ENC_CHROMA_SIZE;

        Deblock_I_LumaBottomMB  (pYMBDebl, step[0], quant);
        Deblock_I_ChromaBottomMB(pUMBDebl, step[1], quant);
        Deblock_I_ChromaBottomMB(pVMBDebl, step[2], quant);
    }
}

inline void Deblock_I_MB(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant)
{
    Deblock_I_LumaMB  (pPlanes[0], step[0], quant);
    Deblock_I_ChromaMB(pPlanes[1], step[1], quant);
    Deblock_I_ChromaMB(pPlanes[2], step[2], quant);
}

inline void Deblock_I_LeftMB(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant)
{
    Deblock_I_LumaLeftMB  (pPlanes[0], step[0], quant);
    Deblock_I_ChromaLeftMB(pPlanes[1], step[1], quant);
    Deblock_I_ChromaLeftMB(pPlanes[2], step[2], quant);
}

inline void Deblock_I_LeftBottomMB(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant)
{
    Deblock_I_LumaLeftBottomMB(pPlanes[0], step[0], quant);
    Deblock_I_ChromaLeftMB(pPlanes[1], step[1], quant);
    Deblock_I_ChromaLeftMB(pPlanes[2], step[2], quant);
}

inline void Deblock_I_BottomMB(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant)
{
    Deblock_I_LumaBottomMB(pPlanes[0], step[0], quant);
    Deblock_I_ChromaBottomMB  (pPlanes[1], step[1], quant);
    Deblock_I_ChromaBottomMB  (pPlanes[2], step[2], quant);
}

inline void no_Deblocking_I_MB(Ipp8u* /*pPlanes*/[3], Ipp32u /*step*/[3], Ipp32u /*quant*/)
{
};

//-----------------------Deblocking P frames Variable Transform--------------------------------------

inline void Deblock_P_LumaLeftMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s TopLeftVerEdge  = pTop->GetLumaAdLefEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* pLeftTop,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s LeftTopTopVerEdge = pTop->GetLumaExVerEdge();
    Ipp32s TopVerEdge        = pTop->GetLumaInVerEdge();
    Ipp32s TopLeftVerEdge    = pTop->GetLumaAdLefEdge();

    Ipp32s LeftTopRightVerEdge  = pLeftTop->GetLumaAdRigEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);

    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left top right internal ver edge
    pSrc = pY - 16*YStep - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB left internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaRightMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pLeftTop,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s LeftTopRightVerEdge = pLeftTop->GetLumaAdRigEdge();
    Ipp32s LeftTopTopVerEdge   = pTop->GetLumaExVerEdge();

    Ipp32s TopLeftVerEdge  = pTop->GetLumaAdLefEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();
    Ipp32s TopRightVerEdge = pTop->GetLumaAdRigEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);

    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left top right internal ver edge
    pSrc = pY - 16*YStep - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB left internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB left internal ver edge
    pSrc = pY - 16*YStep + 12;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopRightVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}


inline void Deblock_P_LumaLeftBottomMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s TopLeftVerEdge  = pTop->GetLumaAdLefEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurUpHorEdge    = pCur->GetLumaAdUppEdge();
    Ipp32s CurBotHorEdge   = pCur->GetLumaAdBotEdge();

    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();
    Ipp32s CurLeftVerEdge  = pCur->GetLumaAdLefEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal top hor edge
    pSrc = pY + 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal bottom hor edge
    pSrc = pY + 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal left ver edge
    pSrc = pY + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurLeftVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaBottomMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* pLeftTop,
                                VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s TopLeftVerEdge  = pTop->GetLumaAdLefEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurUpHorEdge    = pCur->GetLumaAdUppEdge();
    Ipp32s CurBotHorEdge   = pCur->GetLumaAdBotEdge();

    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();
    Ipp32s CurLeftVerEdge  = pCur->GetLumaAdLefEdge();

    Ipp32s LeftTopRightVerEdge = pLeftTop->GetLumaAdRigEdge();
    Ipp32s LefTopTopVerEdge    = pTop->GetLumaExVerEdge();

    Ipp32s LeftCurVerEdge      = pCur->GetLumaExVerEdge();
    Ipp32s LeftRightVerEdge    = pLeft->GetLumaAdRigEdge();


    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal top hor edge
    pSrc = pY + 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal bottom hor edge
    pSrc = pY + 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LefTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left top right internal ver edge
    pSrc = pY - 16*YStep - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur /left MB internal ver edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftCurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left MB right internal ver edge
    pSrc = pY - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal left ver edge
    pSrc = pY + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurLeftVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaRightBottomMB_VT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* pLeftTop,
                                VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopUpHorEdge    = pTop->GetLumaAdUppEdge();
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32u TopBotHorEdge   = pTop->GetLumaAdBotEdge();

    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s TopLeftVerEdge  = pTop->GetLumaAdLefEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurUpHorEdge    = pCur->GetLumaAdUppEdge();
    Ipp32s CurBotHorEdge   = pCur->GetLumaAdBotEdge();

    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();
    Ipp32s CurLeftVerEdge  = pCur->GetLumaAdLefEdge();

    Ipp32s LeftTopRightVerEdge = pLeftTop->GetLumaAdRigEdge();
    Ipp32s LefTopTopVerEdge    = pTop->GetLumaExVerEdge();

    Ipp32s LeftCurVerEdge      = pCur->GetLumaExVerEdge();
    Ipp32s LeftRightVerEdge    = pLeft->GetLumaAdRigEdge();

    Ipp32s TopRightVerEdge = pTop->GetLumaAdRigEdge();
    Ipp32s CurRightVerEdge = pCur->GetLumaAdRigEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB top internal hor edge
    pSrc = pY - 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal Top MB bottom internal hor edge
    pSrc = pY - 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopBotHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal top hor edge
    pSrc = pY + 4*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurUpHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal bottom hor edge
    pSrc = pY + 12*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurBotHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LefTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left top right internal ver edge
    pSrc = pY - 16*YStep - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopLeftVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur /left MB internal ver edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftCurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical left MB right internal ver edge
    pSrc = pY - 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal left ver edge
    pSrc = pY + 4;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurLeftVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal right ver edge
    pSrc = pY - 16*YStep + 12;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopRightVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal right ver edge
    pSrc = pY + 12;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurRightVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_LeftMB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_LeftMB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_MB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetUAdVerEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pU - 8*UStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_MB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetVAdVerEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pV - 8*VStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_RightMB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                          VC1EncoderMBInfo* pCur,
                                          VC1EncoderMBInfo* pTop,
                                          VC1EncoderMBInfo* pTopLeft,
                                           VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();

    Ipp32s TopVerEdge        = pTop->GetUAdVerEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetUAdVerEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pU - 8*UStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB internal ver edge
   pSrc = pU - 8*UStep + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopVerEdge);
   assert(Sts == ippStsNoErr);


LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_RightMB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();

    Ipp32s TopVerEdge        = pTop->GetVAdVerEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetVAdVerEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pV - 8*VStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB internal ver edge
   pSrc = pV - 8*VStep + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_LeftBottomMB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetUAdHorEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pU + 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_LeftBottomMB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetVAdHorEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pV + 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_BottomMB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetUAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetUAdVerEdge();

    Ipp32s LeftCurVerEdge = pCur->GetUExVerEdge();
    Ipp32s LeftVerEdge    = pLeft->GetUAdVerEdge();

    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pU + 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pU - 8*UStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pU - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_BottomMB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                           VC1EncoderMBInfo* pCur,
                                           VC1EncoderMBInfo* pTop,
                                           VC1EncoderMBInfo* pTopLeft,
                                           VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetVAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetVAdVerEdge();

    Ipp32s LeftCurVerEdge = pCur->GetVExVerEdge();
    Ipp32s LeftVerEdge    = pLeft->GetUAdVerEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pV + 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pV - 8*VStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pV - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}


inline void Deblock_P_Chroma_U_RightBottomMB_VT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetUAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetUAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetUAdVerEdge();
    Ipp32s TopVerEdge        = pTop->GetUAdVerEdge();

    Ipp32s CurVerEdge        = pCur->GetUAdVerEdge();
    Ipp32s LeftCurVerEdge    = pCur->GetUExVerEdge();
    Ipp32s LeftVerEdge       = pLeft->GetUAdVerEdge();

    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pU - 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pU + 4*UStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pU - 8*UStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB ver edge
   pSrc = pU - 8*UStep + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical left MB ver edge
   pSrc = pU - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical left MB ver edge
   pSrc = pU + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, CurVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_RightBottomMB_VT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* pTopLeft,
                                     VC1EncoderMBInfo* pLeft)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopHorEdge         = pTop->GetVAdHorEdge();
    Ipp32s CurHorEdge         = pCur->GetVAdHorEdge();

    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftTopVerEdge    = pTopLeft->GetVAdVerEdge();
    Ipp32s TopVerEdge        = pTop->GetVAdVerEdge();

    Ipp32s CurVerEdge        = pCur->GetVAdVerEdge();
    Ipp32s LeftCurVerEdge    = pCur->GetVExVerEdge();
    Ipp32s LeftVerEdge       = pLeft->GetVAdVerEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock Top MB internal hor edge
   pSrc = pV - 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopHorEdge);
   assert(Sts == ippStsNoErr);

   //horizontal deblock cur MB internal hor edge
   pSrc = pV + 4*VStep;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, CurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top left MB ver edge
   pSrc = pV - 8*VStep - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB ver edge
   pSrc = pV - 8*VStep + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical left MB ver edge
   pSrc = pV - 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical left MB ver edge
   pSrc = pV + 4;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, CurVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

//-----------------------Deblocking P frames No Variable Transform--------------------------------------

inline void Deblock_P_LumaLeftMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();

    Ipp32s LeftTopTopVerEdge = pTop->GetLumaExVerEdge();
    Ipp32s TopVerEdge        = pTop->GetLumaInVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);

    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaRightMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pLeftTop*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();
    Ipp32s LeftTopTopVerEdge   = pTop->GetLumaExVerEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);

    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}


inline void Deblock_P_LumaLeftBottomMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();
    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaBottomMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();
    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();
    Ipp32s LefTopTopVerEdge    = pTop->GetLumaExVerEdge();
    Ipp32s LeftCurVerEdge      = pCur->GetLumaExVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LefTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur /left MB internal ver edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftCurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_LumaRightBottomMB_NoVT(Ipp8u* pY, Ipp32s YStep, Ipp32s quant,
                                VC1EncoderMBInfo* pCur,
                                VC1EncoderMBInfo* pTop,
                                VC1EncoderMBInfo* /*pLeftTop*/,
                                VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopHorEdge      = pTop->GetLumaInHorEdge();
    Ipp32s TopCurHorEdge   = pCur->GetLumaExHorEdge();
    Ipp32s TopVerEdge      = pTop->GetLumaInVerEdge();
    Ipp32s CurHorEdge      = pCur->GetLumaInHorEdge();
    Ipp32s CurVerEdge      = pCur->GetLumaInVerEdge();
    Ipp32s LefTopTopVerEdge    = pTop->GetLumaExVerEdge();
    Ipp32s LeftCurVerEdge      = pCur->GetLumaExVerEdge();

    Ipp8u* pSrc = pY;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    //horizontal deblock Top MB internal hor edge
    pSrc = pY - 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal TopMB/Cur MB hor edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, TopCurHorEdge);
    assert(Sts == ippStsNoErr);

    //horizontal cur MB internal hor edge
    pSrc = pY + 8*YStep;
    Sts = ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR  (pSrc,  quant, YStep, CurHorEdge);
    assert(Sts == ippStsNoErr);

    //vertical Left top/Top MB intrnal ver edge
    pSrc = pY - 16*YStep;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LefTopTopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical Top MB internal ver edge
    pSrc = pY - 16*YStep + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, TopVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur /left MB internal ver edge
    pSrc = pY;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, LeftCurVerEdge);
    assert(Sts == ippStsNoErr);

    //vertical cur MB internal ver edge
    pSrc = pY + 8;
    Sts = ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (pSrc,  quant, YStep, CurVerEdge);
    assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_LeftMB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* /*pTop*/,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_LeftMB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* /*pTop*/,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_MB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_MB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_RightMB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                          VC1EncoderMBInfo* pCur,
                                          VC1EncoderMBInfo* pTop,
                                          VC1EncoderMBInfo* /*pTopLeft*/,
                                           VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_RightMB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                     VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_LeftBottomMB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* /*pTop*/,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_LeftBottomMB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* /*pTop*/,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_U_BottomMB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftCurVerEdge = pCur->GetUExVerEdge();

    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_BottomMB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                           VC1EncoderMBInfo* pCur,
                                           VC1EncoderMBInfo* pTop,
                                           VC1EncoderMBInfo* /*pTopLeft*/,
                                           VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftCurVerEdge = pCur->GetVExVerEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}


inline void Deblock_P_Chroma_U_RightBottomMB_NoVT(Ipp8u* pU, Ipp32s UStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetUExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetUExVerEdge();
    Ipp32s LeftCurVerEdge    = pCur->GetUExVerEdge();

    Ipp8u* pSrc = pU;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pU - 8*UStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pU;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, UStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);

LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}

inline void Deblock_P_Chroma_V_RightBottomMB_NoVT(Ipp8u* pV, Ipp32s VStep, Ipp32s quant,
                                      VC1EncoderMBInfo* pCur,
                                     VC1EncoderMBInfo* pTop,
                                     VC1EncoderMBInfo* /*pTopLeft*/,
                                     VC1EncoderMBInfo* /*pLeft*/)
{
    Ipp32s TopCurHorEdge      = pCur->GetVExHorEdge();
    Ipp32s TopLeftTopVerEdge = pTop->GetVExVerEdge();
    Ipp32s LeftCurVerEdge    = pCur->GetVExVerEdge();

    Ipp8u* pSrc = pV;
    IppStatus Sts = ippStsNoErr;

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
   //horizontal deblock Cur MB/Top MB internal hor edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopCurHorEdge);
   assert(Sts == ippStsNoErr);

  //vertical top MB/top left MB ver edge
   pSrc = pV - 8*VStep;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, TopLeftTopVerEdge);
   assert(Sts == ippStsNoErr);

  //vertical cur MB/left MB ver edge
   pSrc = pV;
   Sts =  ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR(pSrc, quant, VStep, LeftCurVerEdge);
   assert(Sts == ippStsNoErr);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);
}
inline void Deblock_P_LeftMB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                 VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaLeftMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_LeftMB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_LeftMB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_MB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                             VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_MB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_MB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RightMB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                  VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaRightMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_RightMB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_RightMB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_LeftBottomMB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                 VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaLeftBottomMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_LeftBottomMB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_LeftBottomMB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_BottomMB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                             VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaBottomMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_BottomMB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_BottomMB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RightBottomMB_VT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                  VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaRightBottomMB_VT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_RightBottomMB_VT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_RightBottomMB_VT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_LeftMB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                 VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaLeftMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_LeftMB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_LeftMB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_MB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                             VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_MB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_MB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RightMB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                  VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaRightMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_RightMB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_RightMB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_LeftBottomMB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                 VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaLeftBottomMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_LeftBottomMB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_LeftBottomMB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_BottomMB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                             VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaBottomMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_BottomMB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_BottomMB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RightBottomMB_NoVT(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant, VC1EncoderMBInfo* pCur,
                                  VC1EncoderMBInfo* pTop, VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft)
{
    Deblock_P_LumaRightBottomMB_NoVT     (pPlanes[0], step[0], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_U_RightBottomMB_NoVT(pPlanes[1], step[1], quant, pCur, pTop, pLeftTop, pLeft);
    Deblock_P_Chroma_V_RightBottomMB_NoVT(pPlanes[2], step[2], quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RowVts(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width,  Ipp32u quant, VC1EncoderMBs* pMBs)
{
    VC1EncoderMBInfo  * pCur     = NULL;
    VC1EncoderMBInfo  * pTop     = NULL;
    VC1EncoderMBInfo  * pLeftTop = NULL;
    VC1EncoderMBInfo  * pLeft    = NULL;

    Ipp8u* DeblkPlanes[3] = {pPlanes[0], pPlanes[1], pPlanes[2]};

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_LeftMB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

    pMBs->NextMB();

    for(Ipp32u j = 1; j < width - 1; j++)
    {
        DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
        DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
        DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

        pCur     = pMBs->GetCurrMBInfo();
        pTop     = pMBs->GetPevMBInfo(0, 1);
        pLeftTop = pMBs->GetPevMBInfo(1, 1);
        pLeft    = pMBs->GetPevMBInfo(1, 0);

        Deblock_P_MB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

        pMBs->NextMB();
    }

    DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
    DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
    DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_RightMB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);
 }

inline void Deblock_P_BottomRowVts(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width,  Ipp32u quant, VC1EncoderMBs* pMBs)
{
    VC1EncoderMBInfo  * pCur     = NULL;
    VC1EncoderMBInfo  * pTop     = NULL;
    VC1EncoderMBInfo  * pLeftTop = NULL;
    VC1EncoderMBInfo  * pLeft    = NULL;

    Ipp8u* DeblkPlanes[3] = {pPlanes[0], pPlanes[1], pPlanes[2]};

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_LeftBottomMB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

    pMBs->NextMB();

    for(Ipp32u j = 1; j < width - 1; j++)
    {
        DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
        DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
        DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

        pCur     = pMBs->GetCurrMBInfo();
        pTop     = pMBs->GetPevMBInfo(0, 1);
        pLeftTop = pMBs->GetPevMBInfo(1, 1);
        pLeft    = pMBs->GetPevMBInfo(1, 0);

        Deblock_P_BottomMB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

        pMBs->NextMB();
    }

    DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
    DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
    DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_RightBottomMB_VT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_RowNoVts(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width,  Ipp32u quant, VC1EncoderMBs* pMBs)
{
    VC1EncoderMBInfo  * pCur     = NULL;
    VC1EncoderMBInfo  * pTop     = NULL;
    VC1EncoderMBInfo  * pLeftTop = NULL;
    VC1EncoderMBInfo  * pLeft    = NULL;

    Ipp8u* DeblkPlanes[3] = {pPlanes[0], pPlanes[1], pPlanes[2]};

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_LeftMB_NoVT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

    pMBs->NextMB();

    for(Ipp32u j = 1; j < width - 1; j++)
    {
        DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
        DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
        DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

        pCur     = pMBs->GetCurrMBInfo();
        pTop     = pMBs->GetPevMBInfo(0, 1);
        pLeftTop = pMBs->GetPevMBInfo(1, 1);
        pLeft    = pMBs->GetPevMBInfo(1, 0);

        Deblock_P_MB_NoVT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

        pMBs->NextMB();
    }

    DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
    DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
    DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_RightMB_NoVT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);
}

inline void Deblock_P_BottomRowNoVts(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u width,  Ipp32u quant, VC1EncoderMBs* pMBs)
{
    VC1EncoderMBInfo  * pCur     = NULL;
    VC1EncoderMBInfo  * pTop     = NULL;
    VC1EncoderMBInfo  * pLeftTop = NULL;
    VC1EncoderMBInfo  * pLeft    = NULL;

    Ipp8u* DeblkPlanes[3] = {pPlanes[0], pPlanes[1], pPlanes[2]};

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_LeftBottomMB_NoVT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

    pMBs->NextMB();

    for(Ipp32u j = 1; j < width - 1; j++)
    {
        DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
        DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
        DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

        pCur     = pMBs->GetCurrMBInfo();
        pTop     = pMBs->GetPevMBInfo(0, 1);
        pLeftTop = pMBs->GetPevMBInfo(1, 1);
        pLeft    = pMBs->GetPevMBInfo(1, 0);

        Deblock_P_BottomMB_NoVT(DeblkPlanes, step, quant, pCur, pTop, pLeftTop, pLeft);

        pMBs->NextMB();
    }

    DeblkPlanes[0] += VC1_ENC_LUMA_SIZE;
    DeblkPlanes[1] += VC1_ENC_CHROMA_SIZE;
    DeblkPlanes[2] += VC1_ENC_CHROMA_SIZE;

    pCur     = pMBs->GetCurrMBInfo();
    pTop     = pMBs->GetPevMBInfo(0, 1);
    pLeftTop = pMBs->GetPevMBInfo(1, 1);
    pLeft    = pMBs->GetPevMBInfo(1, 0);

    Deblock_P_RightBottomMB_NoVT(DeblkPlanes, step,  quant, pCur, pTop, pLeftTop, pLeft);
}
inline void no_Deblocking_P_MB(Ipp8u* /*pPlanes*/[3], Ipp32u /*step*/[3], Ipp32u /*quant*/, VC1EncoderMBInfo* /*pCur*/,
                                  VC1EncoderMBInfo* /*pTop*/, VC1EncoderMBInfo* /*pLeftTop*/, VC1EncoderMBInfo* /*pLeft*/)
{
};

typedef void(*fDeblock_I_MB)(Ipp8u* pPlanes[3], Ipp32u step[3], Ipp32u quant);

extern fDeblock_I_MB Deblk_I_MBFunction[8];

typedef void(*fDeblock_P_MB)(Ipp8u* pPlanes[3],      Ipp32u step[3],             Ipp32u quant,
                             VC1EncoderMBInfo* pCur,     VC1EncoderMBInfo* pTop,
                             VC1EncoderMBInfo* pLeftTop, VC1EncoderMBInfo* pLeft);
extern fDeblock_P_MB Deblk_P_MBFunction[2][16];

}

#endif
