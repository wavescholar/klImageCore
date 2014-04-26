/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//      Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//  Purpose
//    bitrate control
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include <math.h>
#include "mp4_enc.h"

namespace MPEG4_ENC
{

MPEG4_RC::MPEG4_RC()
{
    ippsSet_8u(0, (Ipp8u*)this, sizeof(MPEG4_RC));
}


MPEG4_RC::~MPEG4_RC()
{
    Close();
}


void MPEG4_RC::Close()
{
    mIsInit = false;
}


Ipp32s MPEG4_RC::GetInitQP(Ipp32s frameWidth, Ipp32s frameHeight)
{
    Ipp32s q = (Ipp32s)(frameWidth * frameHeight / mBitsDesiredFrame * 3 >> 2);
    mp4_ClipL(q, 2);
    mp4_Clip(q, mQuantMin, mQuantMax);
    return q;
}


#define SetQuantB() \
    mQuantB = (((mQuantP + mQuantPrev) * 1229) >> 11) + 1; \
    mp4_Clip(mQuantB, 2, 31)


void MPEG4_RC::Init(Ipp32s qas, Ipp32s fas, Ipp32s bas, Ipp32s bitRate, Ipp64f frameRate, Ipp32s frameWidth, Ipp32s frameHeight, Ipp32s qMin, Ipp32s qMax)
{
    if (mIsInit)
        Close();
    mMethod = 1;
    if (qas <= 0)
        qas = 100;
    if (fas <= 0)
        fas = 10;
    if (bas <= 0)
        bas = 100;
    mQuantMax = qMax;
    mQuantMin = qMin;
    mBitRate = bitRate;
    mBitsDesiredTotal = 0;
    mBitsEncodedTotal = 0;
    mBitsDesiredFrame = (Ipp32s)(mBitRate / frameRate);
    Ipp32s q = GetInitQP(frameWidth, frameHeight);
    mQuantPrev = mQuantI = mQuantP = q;
    SetQuantB();
    mRCfap = fas;
    mRCqap = qas;
    mRCbap = bas;
    mRCq = q;
    mRCqa = 1. / (Ipp64f)mRCq;
    mRCfa = mBitsDesiredFrame;
    mIsInit = true;
    //mRCqh = -1; mRCqs = 0; mRCbf = 0;
}


void MPEG4_RC::PostFrame(Ipp32s frameType, Ipp64s bEncoded)
{
    Ipp64f  bo, qs, dq;
    Ipp32s  quant, bpfEncoded;

    bpfEncoded = (Ipp32s)(bEncoded - mBitsEncodedTotal);
    mBitsEncodedTotal = bEncoded;
    mBitsDesiredTotal += mBitsDesiredFrame;
    quant = (frameType == MP4_VOP_TYPE_I) ? mQuantI : (frameType == MP4_VOP_TYPE_B) ? mQuantB : mQuantP;
    mRCqa += (1. / quant - mRCqa) / mRCqap;
    mp4_Clip(mRCqa, 1./mQuantMax , 1./mQuantMin);
    if (frameType != MP4_VOP_TYPE_I/* || mRCfap < 30*/)
        mRCfa += (bpfEncoded - mRCfa) / mRCfap;
    SetQuantB();
    if (frameType == MP4_VOP_TYPE_B)
        return;
    qs = pow(mBitsDesiredFrame / mRCfa, 1.5);
    dq = mRCqa * qs;
    bo = (Ipp64f)((Ipp64s)mBitsEncodedTotal - (Ipp64s)mBitsDesiredTotal) / mRCbap / mBitsDesiredFrame;
    mp4_Clip(bo, -1.0, 1.0);
    //dq = dq * (1.0 - bo);
    dq = dq + (1./31 - dq) * bo;
    mp4_ClipL(dq, 1./31.);
    if (bo > -0.05) {
        mp4_ClipR(dq, 1./2.);
    } else {
        mp4_ClipR(dq, 1./1.);
    }
    quant = (int) (1. / dq + 0.5);
    if (quant >= mRCq + 3)
         quant = mRCq + 2;
    else if (quant > mRCq + 1)
        quant = mRCq + 1;
    else if (quant <= mRCq - 3)
        quant = mRCq - 2;
    else if (quant < mRCq - 1)
        quant = mRCq - 1;
    mp4_Clip(quant, mQuantMin, mQuantMax);
    mQuantPrev = mQuantP;
    mRCq = mQuantI = mQuantP = quant;
#if 0
#define RCBS 32
    int    bpfExpected, delay, quant, coding_type, bpfEncoded, qa;

    bpfEncoded = mBitsEncodedFrame;
    mBitsDesiredTotal += mBitsDesiredFrame;
    coding_type = VOL.short_video_header ? VOP.picture_coding_type : VOP.vop_coding_type;
    SetQuantBVOP();
    if (coding_type == MP4_VOP_TYPE_B)
        return;
    delay = (int)(mBitsDesiredTotal - mBitsEncodedTotal);
    if (coding_type == MP4_VOP_TYPE_I) {
        bpfExpected = (mBitsDesiredFrame << 2) + (delay >> 1);
        mp4_ClipL(bpfExpected, mBitsDesiredFrame * 7 >> 3);
    } else if (coding_type == MP4_VOP_TYPE_B) {
        bpfExpected = (mBitsDesiredFrame / 3) + (delay >> 2);
        mp4_Clip(bpfExpected, mBitsDesiredFrame / 4, mBitsDesiredFrame * 3 >> 2);
    } else { // P- and S(GMC)-VOP
        bpfExpected = mBitsDesiredFrame + (delay >> 1);
        mp4_Clip(bpfExpected, mBitsDesiredFrame >> 1, mBitsDesiredFrame << 1);
    }
    quant = (coding_type == MP4_VOP_TYPE_I) ? mQuantIVOP : (coding_type == MP4_VOP_TYPE_B) ? mQuantBVOP : mQuantPVOP;
    mRCqs += quant;
    mRCqh ++; if (mRCqh >= RCBS) mRCqh = 0;
    if (mRCbf) {
        mRCqs -= mRCqb[mRCqh];
        qa = (mRCqs + RCBS/2) / RCBS;
    } else {
        if (mRCqh == RCBS-1)
            mRCbf = 1;
        qa = (mRCqs + ((mRCqh + 1) >> 1)) / (mRCqh + 1);
    }
    mRCqb[mRCqh] = (Ipp8u)quant;
    if (mp4_Abs(bpfEncoded - bpfExpected) > (bpfExpected >> 2)) {
        if (bpfEncoded > bpfExpected) {
            //if (quant == 31) {
            //    mCoeffsCut ++;
            //    mp4_ClipR(mCoeffsCut, 64);
            //}
            quant ++;
            //if (bpfEncoded > 4 * bpfExpected)
            //    quant ++;
            int cq = IPP_MIN(31, qa + 4);
            mp4_ClipR(quant, cq);
        } else if (bpfEncoded < bpfExpected) {
            //mCoeffsCut --;
            //mp4_ClipL(mCoeffsCut, 0);
            quant --;
            //if (bpfEncoded * 4 < bpfExpected)
            //    quant --;
            int cq = IPP_MAX(2, qa - 4);
            mp4_ClipL(quant, cq);
        }
    }
    if (VOL.short_video_header && quant < 3)
        quant = 3;
    if (coding_type == MP4_VOP_TYPE_B) {
        //if (quant <= mQuantPVOP + 1) {
        //   quant = mQuantPVOP + 2;
        //   mp4_ClipR(quant, 31);
        //}
        //quant = mQuantPVOP * 3 >> 1;
        //mp4_Clip(quant, 4, 31);
        mQuantBVOP = quant;
    } else {
        mQuantPrev = mQuantPVOP;
        mQuantIVOP = mQuantPVOP = quant;
    }
#endif
}


Ipp32s MPEG4_RC::GetQP(Ipp32s frameType)
{
    return ((frameType == MP4_VOP_TYPE_I) ? mQuantI : (frameType == MP4_VOP_TYPE_B) ? mQuantB : mQuantP);
}

void MPEG4_RC::SetQP(Ipp32s frameType, Ipp32s q)
{
    mp4_Clip(q, mQuantMin, mQuantMax);
    if (frameType == MP4_VOP_TYPE_B)
        mQuantB = q;
    else {
        mRCq = mQuantI = mQuantP = q;
        SetQuantB();
    }
}

// ----------------------------------

MPEG4_RC_MB::MPEG4_RC_MB()
{
    ippsSet_8u(0, (Ipp8u*)this, sizeof(MPEG4_RC_MB));
}


MPEG4_RC_MB::~MPEG4_RC_MB()
{
    Close();
}


void MPEG4_RC_MB::Close()
{
    mIsInit = false;
}


Ipp32s MPEG4_RC_MB::GetInitQP(Ipp32s bitRate, Ipp64f frameRate, Ipp32s frameWidth, Ipp32s frameHeight)
{
    Ipp32s q = (Ipp32s)(frameWidth * frameHeight * frameRate * 0.5 / bitRate) + 1;
    mp4_Clip(q, mQuantMin, mQuantMax);
    return q;
}


Ipp32s MPEG4_RC_MB::GetQP()
{
    return mQuant;
}


void MPEG4_RC_MB::Init(Ipp32s bitRate, Ipp64f frameRate, Ipp32s frameWidth, Ipp32s frameHeight, Ipp32s mbPerRow, Ipp32s mbPerCol, Ipp32s keyInt, Ipp32s dp, Ipp32s qMin, Ipp32s qMax)
{
    if (mIsInit)
        Close();
    mNumMacroBlockPerRow = mbPerRow;
    mNumMacroBlockPerCol = mbPerCol;
    mIVOPdist = keyInt;
    mDP = dp;
    mQuantMax = qMax;
    mQuantMin = qMin;
    mBitRate = bitRate;
    mBitsDesiredTotal = 0;
    mBitsEncodedTotal = 0;
    mBitsDesiredFrame = (Ipp32s)(mBitRate / frameRate);
    mQuant = GetInitQP(bitRate, frameRate, frameWidth, frameHeight);
    mIsInit = true;
}


void MPEG4_RC_MB::Start(mp4_Slice *slice, Ipp32s frameType, Ipp32s quant)
{
    int bb, bpf;

    bb = (int)(mBitsDesiredTotal - mBitsEncodedTotal) * slice->numRow / mNumMacroBlockPerCol;
    if (frameType == MP4_VOP_TYPE_I) {
        bpf = 4 * mBitsDesiredFrame * slice->numRow / mNumMacroBlockPerCol;
        bpf += mIVOPdist < 30 ? (bb * 2 / mIVOPdist) : (bb >> 4);
        mp4_Clip(bpf, mBitsDesiredFrame * slice->numRow / mNumMacroBlockPerCol, mBitRate * slice->numRow / mNumMacroBlockPerCol >> 1);
    } else {
        bpf = mBitsDesiredFrame * slice->numRow / mNumMacroBlockPerCol;
        bpf += mIVOPdist < 30 ? (bb * 2 / mIVOPdist) : (bb >> 4);
        mp4_Clip(bpf, mBitsDesiredFrame * slice->numRow / mNumMacroBlockPerCol >> 1, mBitRate * slice->numRow / mNumMacroBlockPerCol >> 2);
    }
    slice->rc.bpuAvg = (bpf + (slice->numRow * mNumMacroBlockPerRow >> 1)) / (slice->numRow * mNumMacroBlockPerRow);
    slice->rc.bpsAvg = slice->rc.bpsEnc = 0;
    slice->rc.sQuant = quant;
    //slice->cBS.GetPos(&slice->rc.sBitPtr, &slice->rc.sBitOff);
    slice->rc.sBitPtr = slice->cBS.GetPtr();  slice->rc.sBitOff = 0;
    slice->cBS_1.GetPos(&slice->rc.sBitPtr_1, &slice->rc.sBitOff_1);
    slice->cBS_2.GetPos(&slice->rc.sBitPtr_2, &slice->rc.sBitOff_2);
}


void MPEG4_RC_MB::Update(mp4_Slice *slice, int *dquant, Ipp32s frameType)
{
    int    bpuEnc, bpuExp, delay, quant=0;

    bpuEnc = slice->cBS.GetNumBits(slice->rc.sBitPtr, slice->rc.sBitOff);
    slice->cBS.GetPos(&slice->rc.sBitPtr, &slice->rc.sBitOff);
    if (mDP) {
        bpuEnc += slice->cBS_1.GetNumBits(slice->rc.sBitPtr_1, slice->rc.sBitOff_1) + slice->cBS_2.GetNumBits(slice->rc.sBitPtr_2, slice->rc.sBitOff_2);
        slice->cBS_1.GetPos(&slice->rc.sBitPtr_1, &slice->rc.sBitOff_1);
        slice->cBS_2.GetPos(&slice->rc.sBitPtr_2, &slice->rc.sBitOff_2);
    }
    slice->rc.bpsAvg += slice->rc.bpuAvg;
    slice->rc.bpsEnc += bpuEnc;
    delay = slice->rc.bpsAvg - slice->rc.bpsEnc;
    bpuExp = slice->rc.bpuAvg + (delay >> 1);
    mp4_Clip(bpuExp, slice->rc.bpuAvg >> 1, slice->rc.bpuAvg << 1);
    if (mp4_Abs(bpuEnc - bpuExp) > (bpuExp >> 4)) {
        quant = slice->rc.sQuant;
        if (bpuEnc > bpuExp) {
            if (frameType != MP4_VOP_TYPE_B) {
                quant ++;
                mp4_ClipR(quant, mQuantMax);
            } else {
                if (quant + 2 <= mQuantMax)
                    quant += 2;
            }
        } else if (bpuEnc < bpuExp) {
            if (frameType != MP4_VOP_TYPE_B) {
                quant --;
                mp4_ClipL(quant, mQuantMin);
            } else {
                if (quant - 2 >= mQuantMin)
                    quant -= 2;
            }
        }
        *dquant = quant - slice->rc.sQuant;
        slice->rc.sQuant = quant;
    } else
        *dquant = 0;
}


void MPEG4_RC_MB::PostFrame(Ipp32s bpfEncoded, Ipp32s quantSum)
{
    mBitsEncodedTotal += bpfEncoded;
    mBitsDesiredTotal += mBitsDesiredFrame;
    Ipp32s mbF = mNumMacroBlockPerCol * mNumMacroBlockPerRow;
    mQuant = (quantSum + (mbF >> 1)) / mbF;
}


} //namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
