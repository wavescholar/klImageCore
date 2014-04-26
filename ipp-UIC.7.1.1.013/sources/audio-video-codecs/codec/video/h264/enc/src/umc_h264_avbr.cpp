/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//      Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//  Purpose
//    AVBR frame adaptive bitrate control
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include <math.h>
#include "umc_h264_avbr.h"
#include "umc_h264_config.h"

Status H264_AVBR_Create(
    H264_AVBR* state)
{
    memset(state, 0, sizeof(H264_AVBR));
    return UMC_OK;
}


void H264_AVBR_Destroy(
    H264_AVBR* state)
{
    H264_AVBR_Close(state);
}


void H264_AVBR_Close(
    H264_AVBR* state)
{
    state->mIsInit = false;
}


Ipp32s H264_AVBR_GetInitQP(
    H264_AVBR* state,
    Ipp32s bitRate,
    Ipp64f frameRate,
    Ipp32s fPixels,
    Ipp32s bitDepth,
    Ipp32s chromaSampling,
    Ipp32s alpha)
{
    const Ipp64f x0 = 0, y0 = 1.19, x1 = 1.75, y1 = 1.75;
    Ipp32s fs, fsLuma;

    fsLuma = fPixels;
    fs = fsLuma;
    if (alpha)
        fs += fsLuma;
    if (chromaSampling == 1)
        fs += fsLuma / 2;
    else if (chromaSampling == 2)
        fs += fsLuma;
    else if (chromaSampling == 3)
        fs += fsLuma * 2;
    fs = fs * bitDepth / 8;
    Ipp32s q = (Ipp32s)(1. / 1.2 * pow(10.0, (log10(fs * 2. / 3. * frameRate / bitRate) - x0) * (y1 - y0) / (x1 - x0) + y0) + 0.5);
    h264_Clip(q, 1, state->mQuantMax);
    return q;
}


Status H264_AVBR_Init(
    H264_AVBR* state,
    Ipp32s qas,
    Ipp32s fas,
    Ipp32s bas,
    Ipp32s bitRate,
    Ipp64f frameRate,
    Ipp32s fPixels,
    Ipp32s bitDepth,
    Ipp32s chromaSampling,
    Ipp32s alpha)
{
    if (state->mIsInit)
        H264_AVBR_Close(state);
    state->mQuantOffset = 6 * (bitDepth - 8);
    state->mQuantMax = 51 + state->mQuantOffset;
    if (qas <= 0)
        qas = 100;
    if (fas <= 0)
        fas = 30;
    if (bas <= 0)
        bas = 100;
    state->mBitRate = bitRate;
    state->mBitsDesiredTotal = 0;
    state->mBitsDesiredFrame = (Ipp32s)((Ipp64f)state->mBitRate / frameRate);
    Ipp32s q = H264_AVBR_GetInitQP(state, bitRate, frameRate, fPixels, bitDepth, chromaSampling, alpha);
    state->mQuantPrev = state->mQuantI = state->mQuantP = state->mQuantB = q;
    state->mRCfap = fas;
    state->mRCqap = qas;
    state->mRCbap = bas;
    state->mRCq = q;
    state->mRCqa = 1. / (Ipp64f)state->mRCq;
    state->mRCfa = state->mBitsDesiredFrame;
    state->mIsInit = true;
    return UMC_OK;
}


void H264_AVBR_PostFrame(
    H264_AVBR* state,
    EnumPicCodType frameType,
    Ipp32s bpfEncoded)
{
    Ipp64f  bo, qs, dq;
    Ipp32s  quant;

    state->mBitsEncodedTotal += bpfEncoded;
    state->mBitsDesiredTotal += state->mBitsDesiredFrame;
    quant = (frameType == INTRAPIC) ? state->mQuantI : (frameType == BPREDPIC) ? state->mQuantB : state->mQuantP;
    //if (frameType == BPREDPIC)
    //    state->mRCqa += (1. / state->mQuantP - state->mRCqa) / state->mRCqap;
    //else
        state->mRCqa += (1. / quant - state->mRCqa) / state->mRCqap;
    h264_Clip(state->mRCqa, 1./state->mQuantMax , 1./1.);
    if (frameType != INTRAPIC || state->mRCfap < 30)
        state->mRCfa += (bpfEncoded - state->mRCfa) / state->mRCfap;
    state->mQuantB = ((state->mQuantP + state->mQuantPrev) * 563 >> 10) + 1;
    h264_Clip(state->mQuantB, 1, state->mQuantMax);
    //if (frameType == BPREDPIC) return;
    qs = pow(state->mBitsDesiredFrame / state->mRCfa, 2.0);
    dq = state->mRCqa * qs;
    bo = (Ipp64f)((Ipp64s)state->mBitsEncodedTotal - (Ipp64s)state->mBitsDesiredTotal) / state->mRCbap / state->mBitsDesiredFrame;
    h264_Clip(bo, -1.0, 1.0);
    //dq = dq * (1.0 - bo);
    dq = dq + (1./state->mQuantMax - dq) * bo;
    h264_Clip(dq, 1./state->mQuantMax, 1./1.);
    quant = (int) (1. / dq + 0.5);
    //h264_Clip(quant, 1, state->mQuantMax);

    if (quant >= state->mQuantP + 5)
        quant = state->mQuantP + 3;
    else if (quant >= state->mQuantP + 3)
        quant = state->mQuantP + 2;
    else if (quant > state->mQuantP + 1)
        quant = state->mQuantP + 1;
    else if (quant <= state->mQuantP - 5)
        quant = state->mQuantP - 3;
    else if (quant <= state->mQuantP - 3)
        quant = state->mQuantP - 2;
    else if (quant < state->mQuantP - 1)
        quant = state->mQuantP - 1;

    if (frameType != BPREDPIC)
        state->mQuantPrev = state->mQuantP;
    state->mRCq = state->mQuantI = state->mQuantP = quant;
}

Ipp32s H264_AVBR_GetQP(H264_AVBR* state, EnumPicCodType frameType)
{
    return ((frameType == INTRAPIC) ? state->mQuantI : (frameType == BPREDPIC) ? state->mQuantB : state->mQuantP) - state->mQuantOffset;
}

void H264_AVBR_SetQP(
    H264_AVBR* state,
    EnumPicCodType frameType,
    Ipp32s qp)
{
    if(frameType == BPREDPIC) {
        state->mQuantB = qp + state->mQuantOffset;
        h264_Clip(state->mQuantB, 1, state->mQuantMax);
    } else {
        state->mRCq = qp + state->mQuantOffset;
        h264_Clip(state->mRCq, 1, state->mQuantMax);
        state->mQuantI = state->mQuantP = state->mRCq;
    }
    if (state->mBitsEncodedTotal == 0) {
      state->mQuantPrev = state->mRCq;
      state->mRCqa = 1. / (Ipp64f)state->mRCq;
    }
}

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
