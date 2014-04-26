/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, deblocking functions
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_deblocking.h"

namespace UMC_VC1_ENCODER
{
static Ipp8u   curIndexesEx[2][2] = {
    {0,1}, //hor
    {0,2}  //ver
};

static Ipp8u   prevIndexesEx[2][2] = {
    {2,3}, //hor
    {1,3}  //ver
};

#if 0
static Ipp8u   curIndexesIn[2][2] = {
    {2,3}, //hor
    {1,3}  //ver
};
static Ipp8u   prevIndexesIn[2][2] = {
    {0,1}, //hor
    {0,2}  //ver
};
#endif

void GetExternalEdge4MV_VST(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     Ipp8u nCurr;
     Ipp8u nPrev;
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;

     nCurr = curIndexesEx [bVer][0];
     nPrev = prevIndexesEx[bVer][0];

     //------------------ luma edge --------------------------------------------------------------------//
     if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
     {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV(&mvCurr,nCurr);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][0]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][1]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_2;
        }
     }
     nCurr = curIndexesEx [bVer][1];
     nPrev = prevIndexesEx[bVer][1];

    if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
    {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV (&mvCurr,nCurr);
        if  (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][0]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][1]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_4;
        }
    }
    //------------------- chroma edge ---------------------------------------------------------------------//

    if(!pPred->isIntra() && !pCur->isIntra())
    {
        pPred->GetMV(&mvPrev,4);
        pCur ->GetMV(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(4,curIndexesEx [bVer][1]) && !pPred->isCoded(4,prevIndexesEx[bVer][1]))
                UFlag |= IPPVC_EDGE_HALF_2;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(5,curIndexesEx [bVer][1]) && !pPred->isCoded(5,prevIndexesEx[bVer][1]))
                VFlag |= IPPVC_EDGE_HALF_2;
        }
    }
    return;
}
void GetExternalEdge4MV_NOVST(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     Ipp8u nCurr;
     Ipp8u nPrev;
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag =  0;

     nCurr = curIndexesEx [bVer][0];
     nPrev = prevIndexesEx[bVer][0];

     //------------------ luma edge --------------------------------------------------------------------//
     if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
     {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV(&mvCurr,nCurr);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][0]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_HALF_1;
        }
     }
     nCurr = curIndexesEx [bVer][1];
     nPrev = prevIndexesEx[bVer][1];

    if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
    {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV (&mvCurr,nCurr);
        if  (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(nCurr,curIndexesEx [bVer][1]) && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_HALF_2;
        }
    }
    //------------------- chroma edge ---------------------------------------------------------------------//

    if(!pPred->isIntra() && !pCur->isIntra())
    {
        pPred->GetMV(&mvPrev,4);
        pCur ->GetMV(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag = IPPVC_EDGE_ALL;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag = IPPVC_EDGE_ALL;
        }
    }
    return;
}
void GetExternalEdge1MV_VST(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;

     if (!pPred->isIntra() && !pCur->isIntra())
     {
        pPred->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,0);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][1]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_2;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][1]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_4;
        }

        pPred->GetMV(&mvPrev,4);
        pCur ->GetMV(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(4,curIndexesEx [bVer][1]) && !pPred->isCoded(4,prevIndexesEx[bVer][1]))
                UFlag |= IPPVC_EDGE_HALF_2;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(5,curIndexesEx [bVer][1]) && !pPred->isCoded(5,prevIndexesEx[bVer][1]))
                VFlag |= IPPVC_EDGE_HALF_2;
        }
     }
    return;
}

void GetExternalEdge1MV_VST_Field(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};
     bool bSecond = false;
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;

     bSecond = (mvPrev.bSecond == mvCurr.bSecond);

     if (!pPred->isIntra() && !pCur->isIntra())
     {
        pPred->GetMV_F(&mvPrev,0);
        pCur->GetMV_F(&mvCurr,0);

        bSecond = (mvPrev.bSecond == mvCurr.bSecond);

        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y && bSecond)
        {
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][1]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_2;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][1]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][1]))
                YFlag |= IPPVC_EDGE_QUARTER_4;
        }

        pPred->GetMV(&mvPrev,4);
        pCur ->GetMV(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y && bSecond)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(4,curIndexesEx [bVer][1]) && !pPred->isCoded(4,prevIndexesEx[bVer][1]))
                UFlag |= IPPVC_EDGE_HALF_2;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(5,curIndexesEx [bVer][1]) && !pPred->isCoded(5,prevIndexesEx[bVer][1]))
                VFlag |= IPPVC_EDGE_HALF_2;
        }
     }
    return;
}

void GetExternalEdge1MV_VST_SM(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};
     bool coded  = true;
     bool VST4x4 = true;
     Ipp8u nCurr;
     Ipp8u nPrev;

     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }

     YFlag = UFlag = VFlag = 0;

     if(pPred->isIntra() || pCur->isIntra())
         return;

    pPred->GetMV(&mvPrev,0);
    pCur->GetMV(&mvCurr,0);

    if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
    {
        nCurr = curIndexesEx [bVer][0];
        nPrev = prevIndexesEx [bVer][0];
        VST4x4 = (pCur->GetBlkVSTType(nCurr) == VC1_ENC_4x4_TRANSFORM)
            || (pPred->GetBlkVSTType(nPrev) == VC1_ENC_4x4_TRANSFORM);

        coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][0])
            && !pPred->isCoded(nPrev,prevIndexesEx [bVer][0]);

        if (coded && !VST4x4)
            YFlag |= IPPVC_EDGE_QUARTER_1;

        coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][1])
            && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]);

        if (coded && !VST4x4)
            YFlag |= IPPVC_EDGE_QUARTER_2;

        nCurr = curIndexesEx [bVer][1];
        nPrev = prevIndexesEx [bVer][1];

        VST4x4 = (pCur->GetBlkVSTType(nCurr) == VC1_ENC_4x4_TRANSFORM)
            || (pPred->GetBlkVSTType(nPrev) == VC1_ENC_4x4_TRANSFORM);

        coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][0])
            && !pPred->isCoded(nPrev,prevIndexesEx[bVer][0]);

        if (coded && !VST4x4)
            YFlag |= IPPVC_EDGE_QUARTER_3;

        coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][1])
            && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]);
        if (coded && !VST4x4)
            YFlag |= IPPVC_EDGE_QUARTER_4;
    }

    pPred->GetMV(&mvPrev,4);
    pCur ->GetMV(&mvCurr,4);
    if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
    {
        VST4x4 = (pCur->GetBlkVSTType(4) == VC1_ENC_4x4_TRANSFORM)
            || (pPred->GetBlkVSTType(4) == VC1_ENC_4x4_TRANSFORM);

        coded = !pCur->isCoded(4,curIndexesEx [bVer][0])
            && !pPred->isCoded(4,prevIndexesEx[bVer][0]);

        if (coded && !VST4x4)
            UFlag |= IPPVC_EDGE_HALF_1;

        coded = !pCur->isCoded(4,curIndexesEx [bVer][1])
            && !pPred->isCoded(4,prevIndexesEx[bVer][1]);

        if (coded && !VST4x4)
            UFlag |= IPPVC_EDGE_HALF_2;

        VST4x4 = (pCur->GetBlkVSTType(5) == VC1_ENC_4x4_TRANSFORM)
            || (pPred->GetBlkVSTType(5) == VC1_ENC_4x4_TRANSFORM);

        coded = !pCur->isCoded(5,curIndexesEx [bVer][0])
            && !pPred->isCoded(5,prevIndexesEx[bVer][0]);

        if (coded && !VST4x4)
            VFlag |= IPPVC_EDGE_HALF_1;

        coded = !pCur->isCoded(5,curIndexesEx [bVer][1])
            && !pPred->isCoded(5,prevIndexesEx[bVer][1]);

        if (coded && !VST4x4)
            VFlag |= IPPVC_EDGE_HALF_2;
    }

    return;
}

void GetExternalEdge1MV_NOVST(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;

     if (!pPred->isIntra() && !pCur->isIntra())
     {
        pPred->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,0);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_HALF_2;
        }

        pPred->GetMV(&mvPrev,4);
        pCur->GetMV(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag     = IPPVC_EDGE_ALL;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag     = IPPVC_EDGE_ALL;
        }
     }
    return;
}

void GetExternalEdge1MV_NOVST_Field(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};
     bool bSecond = false;
     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;

     if (!pPred->isIntra() && !pCur->isIntra())
     {
        pPred->GetMV_F(&mvPrev,0);
        pCur->GetMV_F(&mvCurr,0);

        bSecond = (mvPrev.bSecond == mvCurr.bSecond);

        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y && bSecond)
        {
            if (!pCur->isCoded(curIndexesEx [bVer][0],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][0],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(curIndexesEx [bVer][1],curIndexesEx [bVer][0]) && !pPred->isCoded(prevIndexesEx[bVer][1],prevIndexesEx[bVer][0]))
                YFlag |= IPPVC_EDGE_HALF_2;
        }

        pPred->GetMV_F(&mvPrev,4);
        pCur->GetMV_F(&mvCurr,4);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y && bSecond)
        {
            if (!pCur->isCoded(4,curIndexesEx [bVer][0]) && !pPred->isCoded(4,prevIndexesEx[bVer][0]))
                UFlag     = IPPVC_EDGE_ALL;
            if (!pCur->isCoded(5,curIndexesEx [bVer][0]) && !pPred->isCoded(5,prevIndexesEx[bVer][0]))
                VFlag     = IPPVC_EDGE_ALL;
        }
     }
    return;
}

void GetExternalEdge4MV_VST_SM(VC1EncoderMBInfo *pPred, VC1EncoderMBInfo *pCur,bool bVer, Ipp8u& YFlag, Ipp8u& UFlag, Ipp8u& VFlag)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     Ipp8u nCurr;
     Ipp8u nPrev;
     bool coded  = true;
     bool VST4x4 = true;

     if (!pPred )
     {
        YFlag  = UFlag  = VFlag  = IPPVC_EDGE_ALL;
        return;
     }
     YFlag = UFlag = VFlag = 0;


     //------------------ luma edge --------------------------------------------------------------------//
    nCurr = curIndexesEx [bVer][0];
    nPrev = prevIndexesEx[bVer][0];

    if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
    {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV(&mvCurr,nCurr);

        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(nCurr) == VC1_ENC_4x4_TRANSFORM)
                || (pPred->GetBlkVSTType(nPrev) == VC1_ENC_4x4_TRANSFORM);


            coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][0])
                && !pPred->isCoded(nPrev,prevIndexesEx [bVer][0]);

            if (coded && !VST4x4)
                YFlag |= IPPVC_EDGE_QUARTER_1;

            coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][1])
                && !pPred->isCoded(nPrev,prevIndexesEx[bVer][1]);

            if (coded && !VST4x4)
                YFlag |= IPPVC_EDGE_QUARTER_2;
        }
    }

    nCurr = curIndexesEx [bVer][1];
    nPrev = prevIndexesEx[bVer][1];

    if (!pPred->isIntra(nPrev) && !pCur->isIntra(nCurr))
    {
        pPred->GetMV(&mvPrev,nPrev);
        pCur->GetMV (&mvCurr,nCurr);

        if  (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(nCurr) == VC1_ENC_4x4_TRANSFORM)
                || (pPred->GetBlkVSTType(nPrev) == VC1_ENC_4x4_TRANSFORM);

            coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][0])
                && !pPred->isCoded(nPrev,prevIndexesEx [bVer][0]);

            if (coded && !VST4x4)
                YFlag |= IPPVC_EDGE_QUARTER_3;

            coded = !pCur->isCoded(nCurr,curIndexesEx [bVer][1])
                && !pPred->isCoded(nPrev,prevIndexesEx [bVer][1]);

            if (coded && !VST4x4)
                YFlag |= IPPVC_EDGE_QUARTER_4;
        }
    }

    //------------------- chroma edge ---------------------------------------------------------------------//

    if(!pPred->isIntra() && !pCur->isIntra())
    {
        pPred->GetMV(&mvPrev,4);
        pCur ->GetMV(&mvCurr,4);

        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(4) == VC1_ENC_4x4_TRANSFORM)
                || (pPred->GetBlkVSTType(4) == VC1_ENC_4x4_TRANSFORM);

            coded = !pCur->isCoded(4,curIndexesEx [bVer][0])
                    && !pPred->isCoded(4,prevIndexesEx[bVer][0]);

            if (coded && !VST4x4)
                UFlag |= IPPVC_EDGE_HALF_1;

            coded = !pCur->isCoded(4,curIndexesEx [bVer][1])
                    && !pPred->isCoded(4,prevIndexesEx[bVer][1]);

            if (coded && !VST4x4)
                UFlag |= IPPVC_EDGE_HALF_2;

            coded = !pCur->isCoded(5,curIndexesEx [bVer][0])
                    && !pPred->isCoded(5,prevIndexesEx[bVer][0]);

            VST4x4 = (pCur->GetBlkVSTType(5) == VC1_ENC_4x4_TRANSFORM)
                || (pPred->GetBlkVSTType(5) == VC1_ENC_4x4_TRANSFORM);

            if (coded && !VST4x4)
                VFlag |= IPPVC_EDGE_HALF_1;

            coded = !pCur->isCoded(5,curIndexesEx [bVer][1])
                    && !pPred->isCoded(5,prevIndexesEx[bVer][1]);

            if (coded && !VST4x4)
                VFlag |= IPPVC_EDGE_HALF_2;
        }
     }
    return;
}
//======================================Internal=================================//
//====================================only for luma==============================//

void GetInternalEdge4MV_VST(VC1EncoderMBInfo *pCur, Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     YFlagV = 0;
     YFlagH = 0;

     if (!pCur->isIntra(0) && !pCur->isIntra(1))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,1);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
                YFlagV |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(0,3) && !pCur->isCoded(1,2))
                YFlagV |= IPPVC_EDGE_QUARTER_2;
        }
     }
     if (!pCur->isIntra(2) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,2);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(2,1) && !pCur->isCoded(3,0))
                YFlagV |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(2,3) && !pCur->isCoded(3,2))
                YFlagV |= IPPVC_EDGE_QUARTER_4;
        }
     }
     if (!pCur->isIntra(0) && !pCur->isIntra(2))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,2);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
                YFlagH |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(0,3) && !pCur->isCoded(2,1))
                YFlagH |= IPPVC_EDGE_QUARTER_2;
        }
     }
     if (!pCur->isIntra(1) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,1);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
                YFlagH |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(1,3) && !pCur->isCoded(3,1))
                YFlagH |= IPPVC_EDGE_QUARTER_4;
        }
     }
    return;
}
void GetInternalEdge4MV_NOVST( VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     YFlagV = 0;
     YFlagH = 0;

     if (!pCur->isIntra(0) && !pCur->isIntra(1))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,1);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
                YFlagV |= IPPVC_EDGE_HALF_1;
            }
     }
     if (!pCur->isIntra(2) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,2);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(2,1) && !pCur->isCoded(3,0))
                YFlagV |= IPPVC_EDGE_HALF_2;
        }
     }
     if (!pCur->isIntra(0) && !pCur->isIntra(2))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,2);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
                YFlagH |= IPPVC_EDGE_HALF_1;

        }
     }
     if (!pCur->isIntra(1) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,1);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
                YFlagH |= IPPVC_EDGE_HALF_2;
        }
     }

    return;
}

void GetInternalEdge1MV_VST(VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{

     YFlagV= 0;
     YFlagH= 0;

     if (!pCur->isIntra())
     {
        if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
            YFlagV |= IPPVC_EDGE_QUARTER_1;
        if (!pCur->isCoded(0,3) && !pCur->isCoded(1,2))
            YFlagV |= IPPVC_EDGE_QUARTER_2;
        if (!pCur->isCoded(2,1) && !pCur->isCoded(3,0))
            YFlagV |= IPPVC_EDGE_QUARTER_3;
        if (!pCur->isCoded(2,3) && !pCur->isCoded(3,2))
            YFlagV |= IPPVC_EDGE_QUARTER_4;
        if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
            YFlagH |= IPPVC_EDGE_QUARTER_1;
        if (!pCur->isCoded(0,3) && !pCur->isCoded(2,1))
            YFlagH |= IPPVC_EDGE_QUARTER_2;
        if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
            YFlagH |= IPPVC_EDGE_QUARTER_3;
        if (!pCur->isCoded(1,3) && !pCur->isCoded(3,1))
            YFlagH |= IPPVC_EDGE_QUARTER_4;
    }
    return;
}
void GetInternalEdge1MV_NOVST(VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     YFlagV= 0;
     YFlagH= 0;

     if (!pCur->isIntra())
     {
        if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
            YFlagV |= IPPVC_EDGE_HALF_1;
        if (!pCur->isCoded(2,1) && !pCur->isCoded(3,0))
            YFlagV |= IPPVC_EDGE_HALF_2;
        if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
            YFlagH |= IPPVC_EDGE_HALF_1;
        if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
            YFlagH |= IPPVC_EDGE_HALF_2;
    }
}


void GetInternalEdge4MV_VST_SM(VC1EncoderMBInfo *pCur, Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     YFlagV = 0;
     YFlagH = 0;

     bool coded  = true;
     bool VST4x4 = true;

    if (!pCur->isIntra(0) && !pCur->isIntra(1))
    {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,1);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(1) == VC1_ENC_4x4_TRANSFORM)
                    || (pCur->GetBlkVSTType(0)== VC1_ENC_4x4_TRANSFORM);

            coded  = !pCur->isCoded(0,1) && !pCur->isCoded(1,0);

            if (coded && !VST4x4)
                YFlagV |= IPPVC_EDGE_QUARTER_1;

            coded  = !pCur->isCoded(0,3) && !pCur->isCoded(1,2);

            if (coded && !VST4x4)
                YFlagV |= IPPVC_EDGE_QUARTER_2;
        }
    }


    if (!pCur->isIntra(2) && !pCur->isIntra(3))
    {
        //(simple profile exception)
        //instead block 2 used block 1
        pCur->GetMV(&mvPrev,2);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(1) == VC1_ENC_4x4_TRANSFORM)
                    || (pCur->GetBlkVSTType(3)== VC1_ENC_4x4_TRANSFORM);

            coded  = !pCur->isCoded(1,0) && !pCur->isCoded(3,0);

            if (coded && !VST4x4)
                YFlagV |= IPPVC_EDGE_QUARTER_3;

            coded  = !pCur->isCoded(1,2) && !pCur->isCoded(3,2);

            if (coded && !VST4x4)
                YFlagV |= IPPVC_EDGE_QUARTER_4;
        }
    }

    if (!pCur->isIntra(0) && !pCur->isIntra(2))
    {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,2);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(0) == VC1_ENC_4x4_TRANSFORM)
                    || (pCur->GetBlkVSTType(2)== VC1_ENC_4x4_TRANSFORM);

            coded  = !pCur->isCoded(0,2) && !pCur->isCoded(2,0);

            if (coded && !VST4x4)
                YFlagH |= IPPVC_EDGE_QUARTER_1;

            coded  = !pCur->isCoded(0,3) && !pCur->isCoded(2,1);

            if (coded && !VST4x4)
                YFlagH |= IPPVC_EDGE_QUARTER_2;
        }
    }

    if (!pCur->isIntra(1) && !pCur->isIntra(3))
    {
        pCur->GetMV(&mvPrev,1);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            VST4x4 = (pCur->GetBlkVSTType(1) == VC1_ENC_4x4_TRANSFORM)
                    || (pCur->GetBlkVSTType(3)== VC1_ENC_4x4_TRANSFORM);

            coded  = !pCur->isCoded(1,2) && !pCur->isCoded(3,0);

            if (coded && !VST4x4)
                YFlagH |= IPPVC_EDGE_QUARTER_3;

            coded  = !pCur->isCoded(1,3) && !pCur->isCoded(3,1);

            if (coded && !VST4x4)
                YFlagH |= IPPVC_EDGE_QUARTER_4;
        }
    }

    return;
}
void GetInternalEdge4MV_NOVST_SM( VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     sCoordinate    mvCurr    = {0,0}, mvPrev = {0,0};

     YFlagV = 0;
     YFlagH = 0;

     if (!pCur->isIntra(0) && !pCur->isIntra(1))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,1);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
                YFlagV |= IPPVC_EDGE_HALF_1;
            }
     }

     //(simple profile exception)
     //instead block 2 used block 1
     if (!pCur->isIntra(2) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,2);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(1,0) && !pCur->isCoded(3,0))
                YFlagV |= IPPVC_EDGE_HALF_2;
        }
     }
     if (!pCur->isIntra(0) && !pCur->isIntra(2))
     {
        pCur->GetMV(&mvPrev,0);
        pCur->GetMV(&mvCurr,2);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
                YFlagH |= IPPVC_EDGE_HALF_1;

        }
     }
     if (!pCur->isIntra(1) && !pCur->isIntra(3))
     {
        pCur->GetMV(&mvPrev,1);
        pCur->GetMV(&mvCurr,3);
        if (mvPrev.x == mvCurr.x && mvPrev.y == mvCurr.y)
        {
            if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
                YFlagH |= IPPVC_EDGE_HALF_2;
        }
     }

    return;
}

void GetInternalEdge1MV_VST_SM(VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     YFlagV = 0;
     YFlagH = 0;
     bool coded  = true;
     bool VST4x4 = true;

     if (pCur->isIntra())
         return;

    VST4x4 = (pCur->GetBlkVSTType(1) == VC1_ENC_4x4_TRANSFORM)
            || (pCur->GetBlkVSTType(0)== VC1_ENC_4x4_TRANSFORM);

    coded  = !pCur->isCoded(0,1) && !pCur->isCoded(1,0);

    if (coded && !VST4x4)
        YFlagV |= IPPVC_EDGE_QUARTER_1;

    coded  = !pCur->isCoded(0,3) && !pCur->isCoded(1,2);

    if (coded && !VST4x4)
        YFlagV |= IPPVC_EDGE_QUARTER_2;

    //(simple profile exception)
    //instead block 2 used block 1
    VST4x4 = (pCur->GetBlkVSTType(3) == VC1_ENC_4x4_TRANSFORM)
            || (pCur->GetBlkVSTType(1)== VC1_ENC_4x4_TRANSFORM);

    coded = !pCur->isCoded(1,0) && !pCur->isCoded(3,0);

    if (coded && !VST4x4)
        YFlagV |= IPPVC_EDGE_QUARTER_3;

    coded = !pCur->isCoded(1,2) && !pCur->isCoded(3,2);
    if (coded && !VST4x4)
        YFlagV |= IPPVC_EDGE_QUARTER_4;

    VST4x4 = (pCur->GetBlkVSTType(2) == VC1_ENC_4x4_TRANSFORM)
        || (pCur->GetBlkVSTType(0)== VC1_ENC_4x4_TRANSFORM);

        coded = !pCur->isCoded(0,2) && !pCur->isCoded(2,0);

    if (coded && !VST4x4)
        YFlagH |= IPPVC_EDGE_QUARTER_1;

    coded = !pCur->isCoded(0,3) && !pCur->isCoded(2,1);

    if (coded && !VST4x4)
        YFlagH |= IPPVC_EDGE_QUARTER_2;

    VST4x4 = (pCur->GetBlkVSTType(1) == VC1_ENC_4x4_TRANSFORM
        || (pCur->GetBlkVSTType(3)== VC1_ENC_4x4_TRANSFORM));

    coded = !pCur->isCoded(1,2) && !pCur->isCoded(3,0);

    if (coded && !VST4x4)
        YFlagH |= IPPVC_EDGE_QUARTER_3;

    coded = !pCur->isCoded(1,3) && !pCur->isCoded(3,1);

    if (coded && !VST4x4)
        YFlagH |= IPPVC_EDGE_QUARTER_4;

    return;
}
void GetInternalEdge1MV_NOVST_SM(VC1EncoderMBInfo *pCur,Ipp8u& YFlagV, Ipp8u& YFlagH)
{
     YFlagV= 0;
     YFlagH= 0;

     if (!pCur->isIntra())
     {
        if (!pCur->isCoded(0,1) && !pCur->isCoded(1,0))
            YFlagV |= IPPVC_EDGE_HALF_1;

       //(simple profile exception)
       //instead block 2 used block 1
        if (!pCur->isCoded(1,0) && !pCur->isCoded(3,0))
            YFlagV |= IPPVC_EDGE_HALF_2;

        if (!pCur->isCoded(0,2) && !pCur->isCoded(2,0))
            YFlagH |= IPPVC_EDGE_HALF_1;

        if (!pCur->isCoded(1,2) && !pCur->isCoded(3,0))
            YFlagH |= IPPVC_EDGE_HALF_2;
    }
}


void GetInternalBlockEdge(VC1EncoderMBInfo *pCur,
                          Ipp8u& YFlagUp, Ipp8u& YFlagBot, Ipp8u& UFlagH, Ipp8u& VFlagH,
                          Ipp8u& YFlagL,  Ipp8u& YFlagR,   Ipp8u& UFlagV, Ipp8u& VFlagV)
{

    YFlagUp = YFlagBot = UFlagH = VFlagH = 0;
    YFlagL  = YFlagR   = UFlagV = VFlagV = 0;

    switch (pCur->GetBlkVSTType(0))
    {
        case VC1_ENC_8x8_TRANSFORM:
            YFlagUp|= IPPVC_EDGE_HALF_1;
            YFlagL |= IPPVC_EDGE_HALF_1;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(0,0) && !pCur->isCoded(0,2))
                YFlagUp |= IPPVC_EDGE_HALF_1;
            YFlagL |= IPPVC_EDGE_HALF_1;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(0,0) && !pCur->isCoded(0,1))
                YFlagL |= IPPVC_EDGE_HALF_1;
            YFlagUp |= IPPVC_EDGE_HALF_1;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(0,0) && !pCur->isCoded(0,2))
                YFlagUp |= IPPVC_EDGE_QUARTER_1;
           if (!pCur->isCoded(0,1) && !pCur->isCoded(0,3))
                YFlagUp |= IPPVC_EDGE_QUARTER_2;
            if (!pCur->isCoded(0,0) && !pCur->isCoded(0,1))
                YFlagL |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(0,2) && !pCur->isCoded(0,3))
                YFlagL |= IPPVC_EDGE_QUARTER_2;
            break;
      default:
          break;
    }

    switch (pCur->GetBlkVSTType(1))
    {
        case VC1_ENC_8x8_TRANSFORM:
            YFlagUp |= IPPVC_EDGE_HALF_2;
            YFlagR |= IPPVC_EDGE_HALF_1;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(1,0) && !pCur->isCoded(1,2))
                YFlagUp |= IPPVC_EDGE_HALF_2;
            YFlagR |= IPPVC_EDGE_HALF_1;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(1,0) && !pCur->isCoded(1,1))
                YFlagR |= IPPVC_EDGE_HALF_1;
            YFlagUp |= IPPVC_EDGE_HALF_2;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(1,0) && !pCur->isCoded(1,2))
                YFlagUp |= IPPVC_EDGE_QUARTER_3;
           if  (!pCur->isCoded(1,1) && !pCur->isCoded(1,3))
                YFlagUp |= IPPVC_EDGE_QUARTER_4;
            if (!pCur->isCoded(1,0) && !pCur->isCoded(1,1))
                YFlagR |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(1,2) && !pCur->isCoded(1,3))
                YFlagR |= IPPVC_EDGE_QUARTER_2;
            break;
      default:
          break;
    }

    switch (pCur->GetBlkVSTType(2))
    {
        case VC1_ENC_8x8_TRANSFORM:
            YFlagBot |= IPPVC_EDGE_HALF_1;
            YFlagL |= IPPVC_EDGE_HALF_2;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(2,0) && !pCur->isCoded(2,2))
                YFlagBot |= IPPVC_EDGE_HALF_1;
            YFlagL |= IPPVC_EDGE_HALF_2;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(2,0) && !pCur->isCoded(2,1))
                YFlagL |= IPPVC_EDGE_HALF_2;
            YFlagBot |= IPPVC_EDGE_HALF_1;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(2,0) && !pCur->isCoded(2,2))
                YFlagBot |= IPPVC_EDGE_QUARTER_1;
            if (!pCur->isCoded(2,1) && !pCur->isCoded(2,3))
                YFlagBot |= IPPVC_EDGE_QUARTER_2;
            if (!pCur->isCoded(2,0) && !pCur->isCoded(2,1))
                YFlagL |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(2,2) && !pCur->isCoded(2,3))
                YFlagL |= IPPVC_EDGE_QUARTER_4;
            break;
      default:
          break;
    }

    switch (pCur->GetBlkVSTType(3))
    {
        case VC1_ENC_8x8_TRANSFORM:
            YFlagBot |= IPPVC_EDGE_HALF_2;
            YFlagR |= IPPVC_EDGE_HALF_2;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(3,0) && !pCur->isCoded(3,2))
                YFlagBot |= IPPVC_EDGE_HALF_2;
            YFlagR |= IPPVC_EDGE_HALF_2;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(3,0) && !pCur->isCoded(3,1))
                YFlagR |= IPPVC_EDGE_HALF_2;
            YFlagBot |= IPPVC_EDGE_HALF_2;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(3,0) && !pCur->isCoded(3,2))
                YFlagBot |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(3,1) && !pCur->isCoded(3,3))
                YFlagBot |= IPPVC_EDGE_QUARTER_4;
            if (!pCur->isCoded(3,0) && !pCur->isCoded(3,1))
                YFlagR |= IPPVC_EDGE_QUARTER_3;
            if (!pCur->isCoded(3,2) && !pCur->isCoded(3,3))
                YFlagR |= IPPVC_EDGE_QUARTER_4;
            break;
      default:
          break;
    }

    switch (pCur->GetBlkVSTType(4))
    {
        case VC1_ENC_8x8_TRANSFORM:
            UFlagH = IPPVC_EDGE_ALL;
            UFlagV = IPPVC_EDGE_ALL;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(4,0) && !pCur->isCoded(4,2))
                UFlagH = IPPVC_EDGE_ALL;
            UFlagV = IPPVC_EDGE_ALL;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(4,0) && !pCur->isCoded(4,1))
                UFlagV = IPPVC_EDGE_ALL;
            UFlagH = IPPVC_EDGE_ALL;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(4,0) && !pCur->isCoded(4,2))
                UFlagH |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(4,1) && !pCur->isCoded(4,3))
                UFlagH |= IPPVC_EDGE_HALF_2;
            if (!pCur->isCoded(4,0) && !pCur->isCoded(4,1))
                UFlagV |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(4,2) && !pCur->isCoded(4,3))
                UFlagV |= IPPVC_EDGE_HALF_2;
             break;
      default:
          break;
    }

    switch (pCur->GetBlkVSTType(5))
    {
        case VC1_ENC_8x8_TRANSFORM:
            VFlagH = IPPVC_EDGE_ALL;
            VFlagV = IPPVC_EDGE_ALL;
            break;
        case VC1_ENC_8x4_TRANSFORM:
            if (!pCur->isCoded(5,0) && !pCur->isCoded(5,2))
                VFlagH = IPPVC_EDGE_ALL;
            VFlagV = IPPVC_EDGE_ALL;
            break;
       case VC1_ENC_4x8_TRANSFORM:
            if (!pCur->isCoded(5,0) && !pCur->isCoded(5,1))
                VFlagV = IPPVC_EDGE_ALL;
            VFlagH = IPPVC_EDGE_ALL;
            break;
      case VC1_ENC_4x4_TRANSFORM:
            if (!pCur->isCoded(5,0) && !pCur->isCoded(5,2))
                VFlagH |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(5,1) && !pCur->isCoded(5,3))
                VFlagH |= IPPVC_EDGE_HALF_2;
            if (!pCur->isCoded(5,0) && !pCur->isCoded(5,1))
                VFlagV |= IPPVC_EDGE_HALF_1;
            if (!pCur->isCoded(5,2) && !pCur->isCoded(5,3))
                VFlagV |= IPPVC_EDGE_HALF_2;
            break;
      default:
          break;
    }

    return;
}

//Advance profile progressive mode external edge
fGetExternalEdge GetExternalEdge[2][2] = {//4 MV, VTS
    {GetExternalEdge1MV_NOVST,GetExternalEdge1MV_VST},
    {GetExternalEdge4MV_NOVST,GetExternalEdge4MV_VST}
};

//Advance profile field mode external edge
fGetExternalEdge GetFieldExternalEdge[2][2] = {//4 MV, VTS
    {GetExternalEdge1MV_NOVST_Field,GetExternalEdge1MV_VST_Field},
    {GetExternalEdge4MV_NOVST,GetExternalEdge4MV_VST}
};

//Advance profile internal edge
fGetInternalEdge GetInternalEdge[2][2] = {//4 MV, VTS
    {GetInternalEdge1MV_NOVST,GetInternalEdge1MV_VST},
    {GetInternalEdge4MV_NOVST,GetInternalEdge4MV_VST}
};

//Simple/Main profile external edge
fGetExternalEdge GetExternalEdge_SM[2][2] = {//4 MV, VTS
    {GetExternalEdge1MV_NOVST,GetExternalEdge1MV_VST_SM},
    {GetExternalEdge4MV_NOVST,GetExternalEdge4MV_VST_SM}
};

//Simple/Main profile internal edge
fGetInternalEdge GetInternalEdge_SM[2][2] = {//4 MV, VTS
    {GetInternalEdge1MV_NOVST_SM,GetInternalEdge1MV_VST_SM},
    {GetInternalEdge4MV_NOVST_SM,GetInternalEdge4MV_VST_SM}
};

fDeblock_I_MB Deblk_I_MBFunction[8] =
    {no_Deblocking_I_MB, no_Deblocking_I_MB,
     Deblock_I_LeftMB,   Deblock_I_MB,
     no_Deblocking_I_MB, no_Deblocking_I_MB, Deblock_I_LeftBottomMB, Deblock_I_BottomMB
    };

fDeblock_P_MB Deblk_P_MBFunction[2][16] =
{
    //no variable transform
    {//top row
     no_Deblocking_P_MB,           no_Deblocking_P_MB,
     no_Deblocking_P_MB,           no_Deblocking_P_MB,

     //middle row
     Deblock_P_LeftMB_NoVT,        Deblock_P_MB_NoVT,
     no_Deblocking_P_MB,           Deblock_P_RightMB_NoVT,

     //
     no_Deblocking_P_MB,           no_Deblocking_P_MB,
     no_Deblocking_P_MB,           no_Deblocking_P_MB,

     //bottom row
     Deblock_P_LeftBottomMB_NoVT,  Deblock_P_BottomMB_NoVT,
     no_Deblocking_P_MB,           Deblock_P_RightBottomMB_NoVT},

     //variable trasform
    {//top row
     no_Deblocking_P_MB,           no_Deblocking_P_MB,
     no_Deblocking_P_MB,           no_Deblocking_P_MB,

     //middle row
      Deblock_P_LeftMB_VT,         Deblock_P_MB_VT,
      no_Deblocking_P_MB,          Deblock_P_RightMB_VT,

     //
     no_Deblocking_P_MB,           no_Deblocking_P_MB,
     no_Deblocking_P_MB,           no_Deblocking_P_MB,

     //bottom row
     Deblock_P_LeftBottomMB_VT,    Deblock_P_BottomMB_VT,
     no_Deblocking_P_MB,           Deblock_P_RightBottomMB_VT}
};


}


#endif
