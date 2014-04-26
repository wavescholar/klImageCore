/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class VideoEncoderMPEG4 (encode VOPs)
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include <stdio.h>

#include "mp4_enc.h"

#if defined(_MSC_VER)
#pragma warning(disable : 981)      // operands are evaluated in unspecified order
#pragma warning(disable : 279)      // controlling expression is constant
#pragma warning(disable : 4127)     // conditional expression is constant
#endif

#define VIDEOPACKETS_LE_MAX

namespace MPEG4_ENC
{

inline void mp4_ComputeChromaMV(const IppMotionVector *mvLuma, IppMotionVector *mvChroma)
{
    mvChroma->dx = (Ipp16s)mp4_Div2Round(mvLuma->dx);
    mvChroma->dy = (Ipp16s)mp4_Div2Round(mvLuma->dy);
}

inline void mp4_ComputeChromaMVQ(const IppMotionVector *mvLuma, IppMotionVector *mvChroma)
{
    Ipp32s  dx, dy;

    dx = mp4_Div2(mvLuma->dx);
    dy = mp4_Div2(mvLuma->dy);
    mvChroma->dx = (Ipp16s)mp4_Div2Round(dx);
    mvChroma->dy = (Ipp16s)mp4_Div2Round(dy);
}

static void mp4_Set8x8_8u(Ipp8u *p, Ipp32s step, Ipp8u level)
{
    Ipp32u  val;

    val = level + (level <<  8);
    val += val << 16;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val; p += step;
    ((Ipp32u*)p)[0] = val; ((Ipp32u*)p)[1] = val;
}

inline Ipp16s mp4_Median(Ipp16s a, Ipp16s b, Ipp16s c)
{
    if (a > b) {
        Ipp16s  t = a; a = b; b = t;
    }
    return (b <= c) ? b : (c >= a) ? c : a;
}

inline void mp4_MV_GetDiff(IppMotionVector *mvC, IppMotionVector *mvP, Ipp32s fMin, Ipp32s fMax, Ipp32s fRange)
{
    Ipp32s  mvDx, mvDy;

    mvDx = mvC->dx - mvP->dx;
    mvDy = mvC->dy - mvP->dy;
    if (mvDx < fMin)
        mvDx += fRange;
    else if (mvDx > fMax)
        mvDx -= fRange;
    if (mvDy < fMin)
        mvDy += fRange;
    else if (mvDy > fMax)
        mvDy -= fRange;
    mvP->dx = (Ipp16s)mvDx;
    mvP->dy = (Ipp16s)mvDy;
}

#define mp4_SetPatternInter(pattern, nzCount) \
    pattern = 0; \
    pattern |= (nzCount[0] > 0) ? 32 : 0; \
    pattern |= (nzCount[1] > 0) ? 16 : 0; \
    pattern |= (nzCount[2] > 0) ?  8 : 0; \
    pattern |= (nzCount[3] > 0) ?  4 : 0; \
    pattern |= (nzCount[4] > 0) ?  2 : 0; \
    pattern |= (nzCount[5] > 0) ?  1 : 0

#define mp4_SetPatternIntra(pattern, nzCount, coeff, use_intra_dc_vlc) \
    pattern = 0; \
    if (use_intra_dc_vlc) { \
        Ipp32s  i, pm = 32; \
        for (i = 0; i < 6; i ++) { \
            if ((nzCount[i] > 1) || ((nzCount[i] == 1) && (coeff[i*64] == 0))) \
                pattern |= pm; \
            pm >>= 1; \
        } \
    } else { \
        pattern |= (nzCount[0] > 0) ? 32 : 0; \
        pattern |= (nzCount[1] > 0) ? 16 : 0; \
        pattern |= (nzCount[2] > 0) ?  8 : 0; \
        pattern |= (nzCount[3] > 0) ?  4 : 0; \
        pattern |= (nzCount[4] > 0) ?  2 : 0; \
        pattern |= (nzCount[5] > 0) ?  1 : 0; \
    }

inline void mp4_NonZeroCount(Ipp16s *coeff, Ipp32s *nzCount)
{
    Ipp32s i;
    Ipp32u c;
    for (i = 0; i < 6; i ++) {
        ippiCountZeros8x8_16s_C1(coeff+i*64, &c);
        nzCount[i] = 64 - c;
    }
}


static int mp4_TrellisQuant(Ipp16s* tcoeff, Ipp16s* qcoeff, int quant, Ipp8u* qMatrix, const Ipp8u* scan, int nzc)
{
    tcoeff = tcoeff;
    qcoeff = qcoeff;
    quant = quant;
    qMatrix = qMatrix;
    scan = scan;
    return nzc;
}


inline void mp4_EncodeZeroBitsAlign(BitStream &cBS)
{
    if (cBS.mBitOff != 0)
        cBS.PutBits(0, 8 - cBS.mBitOff);
}

inline void mp4_EncodeStuffingBitsAlign(BitStream &cBS)
{
    cBS.PutBits(0xFF >> (cBS.mBitOff + 1), 8 - cBS.mBitOff);
}

inline void mp4_EncodeMarkerDC(BitStream &cBS)
{
    cBS.PutBits(0x6B001, 19); // 110 1011 0000 0000 0001
}

inline void mp4_EncodeMarkerMV(BitStream &cBS)
{
    cBS.PutBits(0x1F001, 17); //   1 1111 0000 0000 0001
}

inline void mp4_EncodeDquant(BitStream &cBS, Ipp32s dquant)
{
    if (dquant != -2)
        cBS.PutBits(dquant + 1, 2);
    else
        cBS.PutBits(1, 2);
}

inline void mp4_EncodeMCBPC_I(BitStream &cBS, Ipp32s mbtype, Ipp32s mcbpc)
{
    if (mbtype == IPPVC_MBTYPE_INTRA) {
        if (mcbpc == 0)
            cBS.PutBit(1);
        else
            cBS.PutBits(mcbpc, 3);
    } else {
        if (mcbpc == 0)
            cBS.PutBits(1, 4);
        else
            cBS.PutBits(mcbpc, 6);
    }
}

inline void mp4_EncodeCBPY_I(BitStream &cBS, Ipp32s pat)
{
    cBS.PutBits(mp4_VLC_CBPY_TB8[pat].code, mp4_VLC_CBPY_TB8[pat].len);
}

inline void mp4_EncodeMCBPC_P(BitStream &cBS, Ipp32s mbtype, Ipp32s pat)
{
    cBS.PutBits(mp4_VLC_MCBPC_TB7[mbtype*4+pat].code, mp4_VLC_MCBPC_TB7[mbtype*4+pat].len);
}

inline void mp4_EncodeCBPY_P(BitStream &cBS, Ipp32s mbtype, Ipp32s pat)
{
    if (mbtype <= IPPVC_MBTYPE_INTER4V)
        pat = 15 - pat;
    cBS.PutBits(mp4_VLC_CBPY_TB8[pat].code, mp4_VLC_CBPY_TB8[pat].len);
}

static void mp4_EncodeMV(BitStream &cBS, IppMotionVector *mv, Ipp32s fcode, Ipp32s mbType)
{
    Ipp32s  i, nMV = (mbType == IPPVC_MBTYPE_INTER4V) ? 4 : 1;

    for (i = 0; i < nMV; i ++) {
        if (fcode == 1) {
            cBS.PutBits(mp4_VLC_MVD_TB12[mv[i].dx+32].code, mp4_VLC_MVD_TB12[mv[i].dx+32].len);
            cBS.PutBits(mp4_VLC_MVD_TB12[mv[i].dy+32].code, mp4_VLC_MVD_TB12[mv[i].dy+32].len);
        } else {
            Ipp32s f, a, b;

            f = fcode - 1;
            if (mv[i].dx == 0) {
                cBS.PutBits(mp4_VLC_MVD_TB12[32].code, mp4_VLC_MVD_TB12[32].len);
            } else {
                if (mv[i].dx > 0) {
                    a = ((mv[i].dx - 1) >> f) + 1;
                    b = mv[i].dx - 1 - ((a - 1) << f);
                } else {
                    a = ((-mv[i].dx - 1) >> f) + 1;
                    b = -mv[i].dx - 1 - ((a - 1) << f);
                    a = -a;
                }
                cBS.PutBits(mp4_VLC_MVD_TB12[a+32].code, mp4_VLC_MVD_TB12[a+32].len);
                cBS.PutBits(b, f);
            }
            if (mv[i].dy == 0) {
                cBS.PutBits(mp4_VLC_MVD_TB12[32].code, mp4_VLC_MVD_TB12[32].len);
            } else {
                if (mv[i].dy > 0) {
                    a = ((mv[i].dy - 1) >> f) + 1;
                    b = mv[i].dy - 1 - ((a - 1) << f);
                } else {
                    a = ((-mv[i].dy - 1) >> f) + 1;
                    b = -mv[i].dy - 1 - ((a - 1) << f);
                    a = -a;
                }
                cBS.PutBits(mp4_VLC_MVD_TB12[a+32].code, mp4_VLC_MVD_TB12[a+32].len);
                cBS.PutBits(b, f);
            }
        }
    }
}

inline void mp4_EncodeMacroBlockIntra_H263(BitStream &cBS, Ipp16s *coeffMB, Ipp32s pattern, Ipp32s *nzCount)
{
    Ipp32s  i, pm = 32;

    for (i = 0; i < 6; i ++) {
        ippiEncodeDCIntra_H263_16s1u(coeffMB[i*64], &cBS.mPtr, &cBS.mBitOff);
        if (pattern & pm)
            ippiEncodeCoeffsIntra_H263_16s1u(coeffMB+i*64, &cBS.mPtr, &cBS.mBitOff, nzCount[i] - 1, 0, 0, IPPVC_SCAN_ZIGZAG);
        pm >>= 1;
    }
}

inline void mp4_EncodeMacroBlockInter_H263(BitStream &cBS, Ipp16s *coeffMB, Ipp32s pattern, Ipp32s *nzCount)
{
    Ipp32s  i, pm = 32;

    for (i = 0; i < 6; i ++) {
        if (pattern & pm)
            ippiEncodeCoeffsInter_H263_16s1u(coeffMB+i*64, &cBS.mPtr, &cBS.mBitOff, nzCount[i], 0, IPPVC_SCAN_ZIGZAG);
        pm >>= 1;
    }
}

inline void mp4_EncodeMacroBlockIntra_MPEG4(BitStream &cBS, Ipp16s *coeffMB, Ipp32s pattern, Ipp32s *nzCount, Ipp32s *predDir, Ipp32s use_intra_dc_vlc, Ipp32s alternate_vertical_scan_flag)
{
    Ipp32s  i, nzc, pm = 32;

    for (i = 0; i < 6; i ++) {
        if (use_intra_dc_vlc)
            ippiEncodeDCIntra_MPEG4_16s1u(coeffMB[i*64], &cBS.mPtr, &cBS.mBitOff, (i < 4) ? IPPVC_BLOCK_LUMA : IPPVC_BLOCK_CHROMA);
        if (pattern & pm) {
            nzc = nzCount[i];
            if (use_intra_dc_vlc && (coeffMB[i*64] != 0))
                nzc --;
            ippiEncodeCoeffsIntra_MPEG4_16s1u(coeffMB+i*64, &cBS.mPtr, &cBS.mBitOff, nzc, 0, use_intra_dc_vlc, alternate_vertical_scan_flag ? IPPVC_SCAN_VERTICAL : predDir[i]);
        }
        pm >>= 1;
    }
}

inline void mp4_EncodeMacroBlockInter_MPEG4(BitStream &cBS, Ipp16s *coeffMB, Ipp32s pattern, Ipp32s *nzCount, Ipp32s reversible_vlc, Ipp32s alternate_vertical_scan_flag)
{
    Ipp32s  i, pm = 32;

    for (i = 0; i < 6; i ++) {
        if (pattern & pm)
            ippiEncodeCoeffsInter_MPEG4_16s1u(coeffMB+i*64, &cBS.mPtr, &cBS.mBitOff, nzCount[i], reversible_vlc, alternate_vertical_scan_flag ? IPPVC_SCAN_VERTICAL : IPPVC_SCAN_ZIGZAG);
        pm >>= 1;
    }
}

inline void mp4_EncodeMacroBlockIntra_DC_MPEG4(BitStream &cBS, Ipp16s *coeffMB)
{
    Ipp32s  i;

    for (i = 0; i < 6; i ++) {
        ippiEncodeDCIntra_MPEG4_16s1u(coeffMB[i*64], &cBS.mPtr, &cBS.mBitOff, (i < 4) ? IPPVC_BLOCK_LUMA : IPPVC_BLOCK_CHROMA);
    }
}

inline void mp4_EncodeMacroBlockIntra_AC_MPEG4(BitStream &cBS, Ipp16s *coeffMB, Ipp32s pattern, Ipp32s *nzCount, Ipp32s *predDir, Ipp32s use_intra_dc_vlc, Ipp32s reversible_vlc)
{
    Ipp32s  i, nzc, pm = 32;

    for (i = 0; i < 6; i ++) {
        if (pattern & pm) {
            nzc = nzCount[i];
            if (use_intra_dc_vlc && (coeffMB[i*64] != 0))
                nzc --;
            ippiEncodeCoeffsIntra_MPEG4_16s1u(coeffMB+i*64, &cBS.mPtr, &cBS.mBitOff, nzc, reversible_vlc, use_intra_dc_vlc, predDir[i]);
        }
        pm >>= 1;
    }
}

Ipp32s VideoEncoderMPEG4::EncodeFrame(Ipp32s noMoreData)
{
    Ipp32s isIVOP, isBVOP = 0, nt;

    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    if (!noMoreData)
        PadFrame(mFrameC->pY, mFrameC->pU, mFrameC->pV);
    if (VOL.sprite_enable == MP4_SPRITE_STATIC) {
        // static sprites
        if (noMoreData)
            return MP4_STS_NODATA;
        VOP.vop_coded = 1;
        if (mFrameCount == 0) {
            VOP.vop_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            EncodeVOP_Header();
            EncodeIVOP();
            ExpandFrame(mFrameC->pY, mFrameC->pU, mFrameC->pV);
            VOP.vop_time_increment = -VOL.fixed_vop_time_increment;
        } else {
            VOP.vop_coding_type = MP4_VOP_TYPE_S;
            nt = VOP.vop_time_increment + VOL.fixed_vop_time_increment;
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            EncodeSVOP();
        }
    } else if (VOL.short_video_header) {
        // short_video_header
        if (noMoreData)
            return MP4_STS_NODATA;
        VOP.vop_coded = 1;
        isIVOP = (mFrameCount - mLastIVOP >= mIVOPdist);
        if (isIVOP) {
            VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            EncodeVOPSH_Header();
            EncodeIVOPSH();
            mLastIVOP = mFrameCount;
        } else {
            VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_P;
            VOP.vop_quant = mQuantPVOP;
            EncodeVOPSH_Header();
            EncodePVOPSH();
            if (mNumIntraMB > mSceneChangeThreshold) {
                cBS.Reset();
                VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_I;
                VOP.vop_quant = mQuantIVOP;
                EncodeVOPSH_Header();
                EncodeIVOPSH();
                mLastIVOP = mFrameCount;
            }
        }
        if (mIVOPdist != 1) {
            mFrameD = mFrameC;
            mp4_Swap(mFrameF, mFrameC);
        }
        VOP.temporal_reference += VOP.temporal_reference_increment;
    } else if (mBVOPdist == 0) {
        // without B-frames
        if (noMoreData)
            return MP4_STS_NODATA;
        VOP.vop_coded = 1;
        isIVOP = (mFrameCount - mLastIVOP >= mIVOPdist);
        if (isIVOP) {
            if (mRepeatHeaders && (mFrameCount != 0)) {
                EncodeHeader();
            }
            if (mInsertGOV) {
                GOV.time_code = (Ipp32s)(mVOPtime / VOL.vop_time_increment_resolution);
                mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                EncodeGOV_Header();
            }
            VOP.vop_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            nt = (Ipp32s)(mVOPtime - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            EncodeIVOP();
            mLastIVOP = mFrameCount;
        } else {
            VOP.vop_coding_type = (VOL.sprite_enable == MP4_SPRITE_GMC) ? MP4_VOP_TYPE_S : MP4_VOP_TYPE_P;
            VOP.vop_quant = mQuantPVOP;
            VOP.vop_fcode_forward = mPVOPfcodeForw;
            if (VOP.vop_coding_type == MP4_VOP_TYPE_S)
                if (!FindTransformGMC())
                    VOP.vop_coding_type = MP4_VOP_TYPE_P;
            nt = (Ipp32s)(mVOPtime - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            if (VOP.vop_coding_type == MP4_VOP_TYPE_P)
                EncodePVOP();
            else
                EncodeSVOP();
            if (mNumIntraMB > mSceneChangeThreshold) {
                cBS.Reset();
                if (mRepeatHeaders && (mFrameCount != 0)) {
                    EncodeHeader();
                }
                if (mInsertGOV) {
                    GOV.time_code = (Ipp32s)(mVOPtime / VOL.vop_time_increment_resolution);
                    mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                    EncodeGOV_Header();
                }
                VOP.vop_coding_type = MP4_VOP_TYPE_I;
                VOP.vop_quant = mQuantPVOP;
                nt = (Ipp32s)(mVOPtime - mSyncTime);
                VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                EncodeVOP_Header();
                EncodeIVOP();
                mLastIVOP = mFrameCount;
            } else if (mNumNotCodedMB >= mNumMacroBlockPerVOP) {
                VOP.vop_coded = 0;
                cBS.Reset();
                EncodeVOP_Header();
                EncodeStuffingBitsAlign();
            }
        }
        if (mIVOPdist != 1) {
            mFrameD = mFrameC;
            ExpandFrame(mFrameC->pY, mFrameC->pU, mFrameC->pV);
            mp4_Swap(mFrameF, mFrameC);
        }
        // reset Sync Time
        if (VOP.modulo_time_base != 0)
            mSyncTime = mVOPtime - VOP.vop_time_increment;
    } else {
        // with B-frames
        if (noMoreData) {
            if (mNumOfFrames == -1)
                mNumOfFrames = mFrameCount;
            if (mFrameCount >= mNumOfFrames + mBVOPdist)
                return MP4_STS_NODATA;
        }
        if (noMoreData && (mFrameCount >= mNumOfFrames + mBVOPdist - (mNumOfFrames - 1) % (mBVOPdist + 1))) {
            // last not closed B-frames are coded as P
            Ipp32s  bIndx;

            VOP.vop_coded = 1;
            bIndx = mIndxBVOP + 1;
            if (bIndx > 2 + mBVOPdist)
                bIndx = 2;
            mFrameC = &mFrame[bIndx];
            VOP.vop_coding_type = (VOL.sprite_enable == MP4_SPRITE_GMC) ? MP4_VOP_TYPE_S : MP4_VOP_TYPE_P;
            VOP.vop_quant = mQuantPVOP;
            VOP.vop_fcode_forward = mPVOPfcodeForw;
            if (VOP.vop_coding_type == MP4_VOP_TYPE_S)
                if (!FindTransformGMC())
                    VOP.vop_coding_type = MP4_VOP_TYPE_P;
            nt = (Ipp32s)(mVOPtime - mBVOPdist * VOL.fixed_vop_time_increment - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            if (VOP.vop_coding_type == MP4_VOP_TYPE_P)
                EncodePVOP();
            else
                EncodeSVOP();
            if (mNumIntraMB > mSceneChangeThreshold) {
                cBS.Reset();
                // not RepeatHeaders and InsertGOV here
                VOP.vop_coding_type = MP4_VOP_TYPE_I;
                VOP.vop_quant = mQuantPVOP;
                EncodeVOP_Header();
                EncodeIVOP();
                mLastIVOP = mFrameCount;
            }
            mSyncTimeB = nt;
            mIndxBVOP ++;
            if (mIndxBVOP > 2 + mBVOPdist)
                mIndxBVOP = 2;
            // reset Sync Time
            if (VOP.modulo_time_base != 0)
                mSyncTime = mVOPtime - VOP.vop_time_increment;
            ExpandFrame(mFrameC->pY, mFrameC->pU, mFrameC->pV);
            mFrameD = mFrameC;
            mp4_Swap(mFrameF, mFrameC);
        } else {
            isIVOP = (mFrameCount - mLastIVOP >= mIVOPdist);
            isBVOP = mFrameCount % (mBVOPdist + 1);
            if (!isBVOP) {
                VOP.vop_coded = 1;
                mFrameC = mFrameB;
                if (isIVOP) {
                    if (mRepeatHeaders && (mFrameCount > 0)) {
                        EncodeHeader();
                    }
                    if (mInsertGOV) {
                        GOV.time_code = (Ipp32s)(mVOPtime / VOL.vop_time_increment_resolution);
                        if (mBVOPdist != 0 && (mFrameCount > 0)) {
                            Ipp32s secB = (Ipp32s)((mVOPtime - mBVOPdist * VOL.fixed_vop_time_increment) / VOL.vop_time_increment_resolution);
                            if (secB < GOV.time_code)
                                GOV.time_code = secB;
                        }
                        mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                        EncodeGOV_Header();
                    }
                    VOP.vop_coding_type = MP4_VOP_TYPE_I;
                    VOP.vop_quant = mQuantIVOP;
                    nt = (Ipp32s)(mVOPtime - mSyncTime);
                    VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                    VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                    EncodeVOP_Header();
                    EncodeIVOP();
                    mLastIVOP = mFrameCount;
                } else {
                    VOP.vop_coding_type = (VOL.sprite_enable == MP4_SPRITE_GMC) ? MP4_VOP_TYPE_S : MP4_VOP_TYPE_P;
                    VOP.vop_quant = mQuantPVOP;
                    VOP.vop_fcode_forward = mPVOPfcodeForw;
                    if (VOP.vop_coding_type == MP4_VOP_TYPE_S)
                        if (!FindTransformGMC())
                            VOP.vop_coding_type = MP4_VOP_TYPE_P;
                    nt = (Ipp32s)(mVOPtime - mSyncTime);
                    VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                    VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                    EncodeVOP_Header();
                    if (VOP.vop_coding_type == MP4_VOP_TYPE_P)
                        EncodePVOP();
                    else
                        EncodeSVOP();
                    if (mNumIntraMB > mSceneChangeThreshold) {
                        cBS.Reset();
                        if (mRepeatHeaders && (mFrameCount != 0)) {
                            EncodeHeader();
                        }
                        if (mInsertGOV) {
                            GOV.time_code = (Ipp32s)(mVOPtime / VOL.vop_time_increment_resolution);
                            if (mBVOPdist != 0 && (mFrameCount > 0)) {
                                Ipp32s secB = (Ipp32s)((mVOPtime - mBVOPdist * VOL.fixed_vop_time_increment) / VOL.vop_time_increment_resolution);
                                if (secB < GOV.time_code)
                                    GOV.time_code = secB;
                            }
                            mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                            EncodeGOV_Header();
                        }
                        VOP.vop_coding_type = MP4_VOP_TYPE_I;
                        VOP.vop_quant = mQuantPVOP;
                        nt = (Ipp32s)(mVOPtime - mSyncTime);
                        VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                        VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                        EncodeVOP_Header();
                        EncodeIVOP();
                        mLastIVOP = mFrameCount;
                    }
                }
                mSyncTimeB = nt;
                ExpandFrame(mFrameC->pY, mFrameC->pU, mFrameC->pV);
                // reset Sync Time
                if (VOP.modulo_time_base != 0)
                    mSyncTime = mVOPtime - VOP.vop_time_increment;
            } else if (mFrameCount > mBVOPdist) {
                Ipp32s  bIndx;

                bIndx = mIndxBVOP + 1;
                if (bIndx > 2 + mBVOPdist)
                    bIndx = 2;
                mFrameC = &mFrame[bIndx];
                mTRD = mBVOPdist + 1;
                mTRB = mNumBVOP + 1;
                mTframe = 1;
                VOP.vop_coding_type = MP4_VOP_TYPE_B;
                VOP.vop_quant = mQuantBVOP;
                VOP.vop_fcode_forward = mBVOPfcodeForw;
                VOP.vop_fcode_backward = mBVOPfcodeBack;
                nt = mSyncTimeB - (mBVOPdist - mNumBVOP) * VOL.fixed_vop_time_increment;
                VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                EncodeVOP_Header();
                EncodeBVOP();
            }
            mFrameD = mFrameC;
            // setup next frame
            if (isBVOP) {
                mIndxBVOP ++;
                if (mIndxBVOP > 2 + mBVOPdist)
                    mIndxBVOP = 2;
                mNumBVOP ++;
                if (mNumBVOP == mBVOPdist) {
                    // next frame is not B
                    mNumBVOP = 0;
                    mp4_Swap(mFrameF, mFrameB);
                    mFrameC = mFrameB;
                } else {
                    // next frame is B
                    mFrameC = &mFrame[mIndxBVOP];
                }
            } else {
                // next frame is B
                mFrameC = &mFrame[mIndxBVOP];
            }
        }
    }
    if (mFrameCount > mBVOPdist + 1 || !isBVOP) {
        mBitsEncodedFrame = cBS.GetFullness() << 3;
        mBitsEncodedTotal += mBitsEncodedFrame;
        if (mRateControl == 1) {
            mRC.PostFrame(VOP.vop_coding_type, mBitsEncodedTotal);
            mQuantIVOP = mRC.GetQP(MP4_VOP_TYPE_I);
            mQuantPVOP = mRC.GetQP(MP4_VOP_TYPE_P);
            mQuantBVOP = mRC.GetQP(MP4_VOP_TYPE_B);
        } else if (mRateControl == 2) {
            mRC_MB.PostFrame(mBitsEncodedFrame, mQuantSum);
            mQuantIVOP = mQuantPVOP = mQuantBVOP = mRC_MB.GetQP();
        }
        if (VOP.vop_coding_type == MP4_VOP_TYPE_I) {
            VOP.vop_rounding_type = 0; // reset rounding_type for next P-VOP
        } else if (VOP.vop_coding_type != MP4_VOP_TYPE_B) {
            if (mRoundingControl)
                VOP.vop_rounding_type ^= 1; // switch rounding_type
        }
    }
    mVOPtime += VOL.fixed_vop_time_increment;
    mFrameCount ++;
    return (mFrameCount > mBVOPdist + 1 || !isBVOP) ? MP4_STS_NOERR : MP4_STS_BUFFERED;
}

/*
Ipp32s VideoEncoderMPEG4::EncodeFrame(Ipp32s noMoreData, Ipp32s vop_type, Ipp64s vop_time)
{
    Ipp32s isIVOP, isBVOP, nt;

    if (!mIsInit)
        return MP4_STS_ERR_NOTINIT;
    if (noMoreData)
        return MP4_STS_NODATA;
    if (vop_time < mSyncTime)
        return MP4_STS_ERR_PARAM;
    if (VOL.sprite_enable == MP4_SPRITE_STATIC) {
        // static sprites
        VOP.vop_coded = 1;
        if (mFrameCount == 0 && vop_type == MP4_VOP_TYPE_I) {
            VOP.vop_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            EncodeVOP_Header();
            EncodeIVOP();
            ExpandFrame(mCurrPtrY, mCurrPtrU, mCurrPtrV);
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
        } else if (mFrameCount > 0 && vop_type != MP4_VOP_TYPE_S) {
            VOP.vop_coding_type = MP4_VOP_TYPE_S;
            nt = (Ipp32s)(vop_time - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            if (VOP.modulo_time_base != 0)
                mSyncTime = vop_time - VOP.vop_time_increment;
            EncodeVOP_Header();
            EncodeSVOP();
            mQuantSum = 0;
        } else
            return MP4_STS_ERR_PARAM;
    } else if (VOL.short_video_header) {
        // short_video_header
        nt = (Ipp32s)(vop_time - mSyncTime);
        mSyncTime = vop_time;
        VOP.temporal_reference = nt / 1001;
        VOP.vop_coded = 1;
        if (vop_type == MP4_VOP_TYPE_I) {
            VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            EncodeVOPSH_Header();
            EncodeIVOPSH();
        } else if (vop_type == MP4_VOP_TYPE_P) {
            VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_P;
            VOP.vop_quant = mQuantPVOP;
            EncodeVOPSH_Header();
            EncodePVOPSH();
            if (mNumIntraMB > mSceneChangeThreshold) {
                cBS.Reset();
                VOP.vop_coding_type = VOP.picture_coding_type = MP4_VOP_TYPE_I;
                VOP.vop_quant = mQuantIVOP;
                EncodeVOPSH_Header();
                EncodeIVOPSH();
            }
        } else
            return MP4_STS_ERR_PARAM;
        mp4_Swap(mForwPtrY, mCurrPtrY);
        mp4_Swap(mForwPtrU, mCurrPtrU);
        mp4_Swap(mForwPtrV, mCurrPtrV);
    } else {
        VOP.vop_coded = 1;
        if (vop_type == MP4_VOP_TYPE_I) {
            if (mRepeatHeaders && (mFrameCount != 0)) {
                EncodeHeader();
            }
            if (mInsertGOV) {
                GOV.time_code = (Ipp32s)(vop_time / VOL.vop_time_increment_resolution);
                mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                EncodeGOV_Header();
            }
            VOP.vop_coding_type = MP4_VOP_TYPE_I;
            VOP.vop_quant = mQuantIVOP;
            nt = (Ipp32s)(vop_time - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            EncodeIVOP();
            mSyncTimeB = nt;
        } else if (vop_type == MP4_VOP_TYPE_P || ((VOL.sprite_enable == MP4_SPRITE_GMC) && (vop_type == MP4_VOP_TYPE_S))) {
            VOP.vop_coding_type = vop_type;
            VOP.vop_quant = mQuantPVOP;
            VOP.vop_fcode_forward = mPVOPfcodeForw;
            if (VOP.vop_coding_type == MP4_VOP_TYPE_S)
                if (!FindTransformGMC())
                    VOP.vop_coding_type = MP4_VOP_TYPE_P;
            nt = (Ipp32s)(vop_time - mSyncTime);
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            if (VOP.vop_coding_type == MP4_VOP_TYPE_P)
                EncodePVOP();
            else
                EncodeSVOP();
            if (mNumIntraMB > mSceneChangeThreshold) {
                cBS.Reset();
                if (mRepeatHeaders && (mFrameCount != 0)) {
                    EncodeHeader();
                }
                if (mInsertGOV) {
                    GOV.time_code = (Ipp32s)(vop_time / VOL.vop_time_increment_resolution);
                    mSyncTime = (Ipp64s)GOV.time_code * (Ipp64s)VOL.vop_time_increment_resolution;
                    EncodeGOV_Header();
                }
                VOP.vop_coding_type = MP4_VOP_TYPE_I;
                VOP.vop_quant = mQuantPVOP;
                nt = (Ipp32s)(vop_time - mSyncTime);
                VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
                VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
                EncodeVOP_Header();
                EncodeIVOP();
            } else if (mNumNotCodedMB >= mNumMacroBlockPerVOP) {
                VOP.vop_coded = 0;
                cBS.Reset();
                EncodeVOP_Header();
                EncodeStuffingBitsAlign();
            }
            mSyncTimeB = nt;
        } else if (vop_type == MP4_VOP_TYPE_B) {
            //mTRD = mBVOPdist + 1;
            //mTRB = mNumBVOP + 1;
            VOP.vop_coding_type = MP4_VOP_TYPE_B;
            VOP.vop_quant = mQuantBVOP;
            VOP.vop_fcode_forward = mBVOPfcodeForw;
            VOP.vop_fcode_backward = mBVOPfcodeBack;
            //nt = mSyncTimeB - (mBVOPdist - mNumBVOP) * VOL.fixed_vop_time_increment;
            VOP.modulo_time_base = nt / VOL.vop_time_increment_resolution;
            VOP.vop_time_increment = nt % VOL.vop_time_increment_resolution;
            EncodeVOP_Header();
            // if ref in the future is not coded, these B are not coded also
            if (VOP.vop_coded)
                EncodeBVOP();
        } else
            return MP4_STS_ERR_PARAM;
        if (vop_type != MP4_VOP_TYPE_B) {
            ExpandFrame(mCurrPtrY, mCurrPtrU, mCurrPtrV);
            mp4_Swap(mForwPtrY, mCurrPtrY);
            mp4_Swap(mForwPtrU, mCurrPtrU);
            mp4_Swap(mForwPtrV, mCurrPtrV);
            if (VOP.modulo_time_base != 0)
                mSyncTime = vop_time - VOP.vop_time_increment;
        }
    }
    mBitsEncodedFrame = cBS.GetFullness() << 3;
    mBitsEncodedTotal += mBitsEncodedFrame;
    if (mRateControl == 1)
        PostFrameRC();
    if (VOP.vop_coding_type == MP4_VOP_TYPE_I) {
        VOP.vop_rounding_type = 0; // reset rounding_type for next P-VOP
    } else if (VOP.vop_coding_type != MP4_VOP_TYPE_B) {
        if (mRoundingControl)
            VOP.vop_rounding_type ^= 1; // switch rounding_type
    }
    mFrameCount ++;
    return MP4_STS_NOERR;
}
*/

static void mp4_MergeBuffersDP(BitStream &cBS, BitStream &cBS_1, BitStream &cBS_2)
{
    Ipp32s   nBits_1, nBits_2;

    // merge buffer with dp_buff_1
    nBits_1 = cBS_1.GetNumBits();
    ippsCopy_1u(cBS_1.mBuffer, 0, cBS.mPtr, cBS.mBitOff, nBits_1);
    cBS.MovePtr(nBits_1);
    cBS_1.Reset();
    // merge buffer with dp_buff_2
    nBits_2 = cBS_2.GetNumBits();
    ippsCopy_1u(cBS_2.mBuffer, 0, cBS.mPtr, cBS.mBitOff, nBits_2);
    cBS.MovePtr(nBits_2);
    cBS_2.Reset();
}

// used for short_viseo_header
void VideoEncoderMPEG4::PredictMV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred)
{
    IppMotionVector *mvLeft, *mvTop, *mvRight;

    mvLeft  = MBcurr[-1].mv;
    mvTop   = MBcurr[-mNumMacroBlockPerRow].mv;
    mvRight = MBcurr[-mNumMacroBlockPerRow+1].mv;
    if (mInsertResync)
        i = (VOP.source_format <= 3) ? 0 : (i & ((VOP.source_format == 4) ? 1 : 3));
    if (i == 0 && j == 0) {
        mvPred[0].dx = mvPred[0].dy = 0;
    } else if (j == 0) {
        mvPred[0].dx = mp4_Median(0, mvTop[0].dx, mvRight[0].dx);
        mvPred[0].dy = mp4_Median(0, mvTop[0].dy, mvRight[0].dy);
    } else if (i == 0) {
        mvPred[0] = mvLeft[0];
    } else if (j == mNumMacroBlockPerRow - 1) {
        mvPred[0].dx = mp4_Median(0, mvLeft[0].dx, mvTop[0].dx);
        mvPred[0].dy = mp4_Median(0, mvLeft[0].dy, mvTop[0].dy);
    } else {
        mvPred[0].dx = mp4_Median(mvLeft[0].dx, mvTop[0].dx, mvRight[0].dx);
        mvPred[0].dy = mp4_Median(mvLeft[0].dy, mvTop[0].dy, mvRight[0].dy);
    }
}

void VideoEncoderMPEG4::Predict1MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred)
{
    IppMotionVector *mvLeft, *mvTop, *mvRight;

    // block 0
    mvLeft  = MBcurr[-1].mv;
    mvTop   = MBcurr[-mNumMacroBlockPerRow].mv;
    mvRight = MBcurr[-mNumMacroBlockPerRow+1].mv;
    if (!mInsertResync) {
        if (i == 0 && j == 0) {
            mvPred[0].dx = mvPred[0].dy = 0;
        } else if (j == 0) {
            mvPred[0].dx = mp4_Median(0, mvTop[2].dx, mvRight[2].dx);
            mvPred[0].dy = mp4_Median(0, mvTop[2].dy, mvRight[2].dy);
        } else if (i == 0) {
            mvPred[0] = mvLeft[1];
        } else if (j == mNumMacroBlockPerRow - 1) {
            mvPred[0].dx = mp4_Median(0, mvLeft[1].dx, mvTop[2].dx);
            mvPred[0].dy = mp4_Median(0, mvLeft[1].dy, mvTop[2].dy);
        } else {
            mvPred[0].dx = mp4_Median(mvLeft[1].dx, mvTop[2].dx, mvRight[2].dx);
            mvPred[0].dy = mp4_Median(mvLeft[1].dy, mvTop[2].dy, mvRight[2].dy);
        }
    } else {
        Ipp32s   validLeft, validTop, validRight;

        if (j > 0)
            validLeft = MBcurr[-1].validPredInter;
        else
            validLeft = 0;
        if (i > 0)
            validTop = MBcurr[-mNumMacroBlockPerRow].validPredInter;
        else
            validTop = 0;
        if ((i > 0) && (j < mNumMacroBlockPerRow - 1))
            validRight = MBcurr[-mNumMacroBlockPerRow+1].validPredInter;
        else
            validRight = 0;
        switch ((validLeft << 2) | (validTop << 1) | validRight) {
        case 7:
            mvPred[0].dx = mp4_Median(mvLeft[1].dx, mvTop[2].dx, mvRight[2].dx);
            mvPred[0].dy = mp4_Median(mvLeft[1].dy, mvTop[2].dy, mvRight[2].dy);
            break;
        case 6:
            mvPred[0].dx = mp4_Median(mvLeft[1].dx, mvTop[2].dx, 0);
            mvPred[0].dy = mp4_Median(mvLeft[1].dy, mvTop[2].dy, 0);
            break;
        case 5:
            mvPred[0].dx = mp4_Median(mvLeft[1].dx, 0, mvRight[2].dx);
            mvPred[0].dy = mp4_Median(mvLeft[1].dy, 0, mvRight[2].dy);
            break;
        case 4:
            mvPred[0] = mvLeft[1];
            break;
        case 3:
            mvPred[0].dx = mp4_Median(0, mvTop[2].dx, mvRight[2].dx);
            mvPred[0].dy = mp4_Median(0, mvTop[2].dy, mvRight[2].dy);
            break;
        case 2:
            mvPred[0] = mvTop[2];
            break;
        case 1:
            mvPred[0] = mvRight[2];
            break;
        default:
            mvPred[0].dx = mvPred[0].dy = 0;
            break;
        }
    }
}

void VideoEncoderMPEG4::Predict3MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred, IppMotionVector *mvCurr)
{
    IppMotionVector *mvLeft, *mvTop, *mvRight;

    mvLeft  = MBcurr[-1].mv;
    mvTop   = MBcurr[-mNumMacroBlockPerRow].mv;
    mvRight = MBcurr[-mNumMacroBlockPerRow+1].mv;
    if (!mInsertResync) {
        // block 1
        if (i == 0) {
            mvPred[1] = mvCurr[0];
        } else if (j == mNumMacroBlockPerRow - 1) {
            mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, 0);
            mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, 0);
        } else {
            mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, mvRight[2].dx);
            mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, mvRight[2].dy);
        }
        // block 2
        if (j == 0) {
            mvPred[2].dx = mp4_Median(0, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[2].dy = mp4_Median(0, mvCurr[0].dy, mvCurr[1].dy);
        } else {
            mvPred[2].dx = mp4_Median(mvLeft[3].dx, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[2].dy = mp4_Median(mvLeft[3].dy, mvCurr[0].dy, mvCurr[1].dy);
        }
        // block 3
        mvPred[3].dx = mp4_Median(mvCurr[2].dx, mvCurr[0].dx, mvCurr[1].dx);
        mvPred[3].dy = mp4_Median(mvCurr[2].dy, mvCurr[0].dy, mvCurr[1].dy);
    } else {
        Ipp32s   validLeft, validTop, validRight;

        if (j > 0)
            validLeft = MBcurr[-1].validPredInter;
        else
            validLeft = 0;
        if (i > 0)
            validTop = MBcurr[-mNumMacroBlockPerRow].validPredInter;
        else
            validTop = 0;
        if ((i > 0) && (j < mNumMacroBlockPerRow - 1))
            validRight = MBcurr[-mNumMacroBlockPerRow+1].validPredInter;
        else
            validRight = 0;
        // block 1
        switch ((validTop << 1) | validRight) {
        case 3:
            mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, mvRight[2].dx);
            mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, mvRight[2].dy);
            break;
        case 2:
            mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, 0);
            mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, 0);
            break;
        case 1:
            mvPred[1].dx = mp4_Median(mvCurr[0].dx, 0, mvRight[2].dx);
            mvPred[1].dy = mp4_Median(mvCurr[0].dy, 0, mvRight[2].dy);
            break;
        default:
            mvPred[1] = mvCurr[0];
            break;
        }
        // block 2
        if (validLeft) {
            mvPred[2].dx = mp4_Median(mvLeft[3].dx, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[2].dy = mp4_Median(mvLeft[3].dy, mvCurr[0].dy, mvCurr[1].dy);
        } else {
            mvPred[2].dx = mp4_Median(0, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[2].dy = mp4_Median(0, mvCurr[0].dy, mvCurr[1].dy);
        }
        // block 3
        mvPred[3].dx = mp4_Median(mvCurr[2].dx, mvCurr[0].dx, mvCurr[1].dx);
        mvPred[3].dy = mp4_Median(mvCurr[2].dy, mvCurr[0].dy, mvCurr[1].dy);
    }
}

void VideoEncoderMPEG4::Predict3MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred, Ipp32s nB)
{
    IppMotionVector *mvLeft, *mvTop, *mvRight, *mvCurr;

    mvCurr  = MBcurr->mv;
    mvLeft  = MBcurr[-1].mv;
    mvTop   = MBcurr[-mNumMacroBlockPerRow].mv;
    mvRight = MBcurr[-mNumMacroBlockPerRow+1].mv;
    if (!mInsertResync) {
        if (nB == 1) {
            if (i == 0) {
                mvPred[1] = mvCurr[0];
            } else if (j == mNumMacroBlockPerRow - 1) {
                mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, 0);
                mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, 0);
            } else {
                mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, mvRight[2].dx);
                mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, mvRight[2].dy);
            }
        } else if (nB == 2) {
            if (j == 0) {
                mvPred[2].dx = mp4_Median(0, mvCurr[0].dx, mvCurr[1].dx);
                mvPred[2].dy = mp4_Median(0, mvCurr[0].dy, mvCurr[1].dy);
            } else {
                mvPred[2].dx = mp4_Median(mvLeft[3].dx, mvCurr[0].dx, mvCurr[1].dx);
                mvPred[2].dy = mp4_Median(mvLeft[3].dy, mvCurr[0].dy, mvCurr[1].dy);
            }
        } else {
            mvPred[3].dx = mp4_Median(mvCurr[2].dx, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[3].dy = mp4_Median(mvCurr[2].dy, mvCurr[0].dy, mvCurr[1].dy);
        }
    } else {
        Ipp32s   validLeft, validTop, validRight;

        if (j > 0)
            validLeft = MBcurr[-1].validPredInter;
        else
            validLeft = 0;
        if (i > 0)
            validTop = MBcurr[-mNumMacroBlockPerRow].validPredInter;
        else
            validTop = 0;
        if ((i > 0) && (j < mNumMacroBlockPerRow - 1))
            validRight = MBcurr[-mNumMacroBlockPerRow+1].validPredInter;
        else
            validRight = 0;
        if (nB == 1) {
            switch ((validTop << 1) | validRight) {
            case 3:
                mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, mvRight[2].dx);
                mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, mvRight[2].dy);
                break;
            case 2:
                mvPred[1].dx = mp4_Median(mvCurr[0].dx, mvTop[3].dx, 0);
                mvPred[1].dy = mp4_Median(mvCurr[0].dy, mvTop[3].dy, 0);
                break;
            case 1:
                mvPred[1].dx = mp4_Median(mvCurr[0].dx, 0, mvRight[2].dx);
                mvPred[1].dy = mp4_Median(mvCurr[0].dy, 0, mvRight[2].dy);
                break;
            default:
                mvPred[1] = mvCurr[0];
                break;
            }
        } else if (nB == 2) {
            if (validLeft) {
                mvPred[2].dx = mp4_Median(mvLeft[3].dx, mvCurr[0].dx, mvCurr[1].dx);
                mvPred[2].dy = mp4_Median(mvLeft[3].dy, mvCurr[0].dy, mvCurr[1].dy);
            } else {
                mvPred[2].dx = mp4_Median(0, mvCurr[0].dx, mvCurr[1].dx);
                mvPred[2].dy = mp4_Median(0, mvCurr[0].dy, mvCurr[1].dy);
            }
        } else {
            mvPred[3].dx = mp4_Median(mvCurr[2].dx, mvCurr[0].dx, mvCurr[1].dx);
            mvPred[3].dy = mp4_Median(mvCurr[2].dy, mvCurr[0].dy, mvCurr[1].dy);
        }
    }
}

void VideoEncoderMPEG4::PredictIntraDCAC(mp4_MacroBlock *MBcurr, Ipp16s *dcCurr, Ipp32s quant, Ipp32s *predictDir, Ipp32s predAC, Ipp32s *pSum0, Ipp32s *pSum1, Ipp32s *nzCount, Ipp32s nRow)
{
    Ipp32s      predDir, dc, k, zC, zA, predQuantA, predQuantC, dcScaler, sum0 = 0, sum1 = 0, i;
    Ipp16s      dcA, dcB, dcC, dcP;
    Ipp16s      *predAcA, *predAcC;
    mp4_Block   *bCurr;

    for (i = 0; i < 6; i ++) {
        bCurr = &MBcurr->block[i];
        dcScaler = i < 4 ? mp4_DCScalerLuma[quant] : mp4_DCScalerChroma[quant];
        dcA = bCurr->predA ? (bCurr->predA->validPredIntra ? bCurr->predA->dct_dc : mDefDC) : mDefDC;
        // next if is needed only for slice coding
        if ((nRow == 0) && (i != 2) && (i != 3)) {
            dcB = mDefDC;
            dcC = mDefDC;
        } else {
            dcB = bCurr->predB ? (bCurr->predB->validPredIntra ? bCurr->predB->dct_dc : mDefDC) : mDefDC;
            dcC = bCurr->predC ? (bCurr->predC->validPredIntra ? bCurr->predC->dct_dc : mDefDC) : mDefDC;
        }
        if (mp4_Abs(dcA - dcB) < mp4_Abs(dcB - dcC)) {
            predDir = IPPVC_SCAN_HORIZONTAL;
            dcP = dcC;
        } else {
            predDir = IPPVC_SCAN_VERTICAL;
            dcP = dcA;
        }
        dc = bCurr->dct_dcq = *dcCurr;
        *dcCurr = (Ipp16s)(dc - mp4_DivIntraDC(dcP, dcScaler));
        // correct nzCount
        if (dc == 0 && *dcCurr != 0)
            nzCount[i] ++;
        else if (dc != 0 && *dcCurr == 0)
            nzCount[i] --;
        bCurr->dct_dc = (Ipp16s)(dc * dcScaler);
        zC = zA = 0;
        for (k = 1; k < 8; k ++) {
            bCurr->dct_acC[k] = dcCurr[k];
            if (dcCurr[k])
                zC = 1;
            bCurr->dct_acA[k] = dcCurr[k*8];
            if (dcCurr[k*8])
                zA = 1;
        }
        bCurr->dct_acC[0] = (Ipp16s)zC;
        bCurr->dct_acA[0] = (Ipp16s)zA;
        if (predAC) {
            predAcA = bCurr->predA ? (bCurr->predA->validPredIntra ? bCurr->predA->dct_acA : NULL) : NULL;
            predAcC = bCurr->predC ? (bCurr->predC->validPredIntra ? bCurr->predC->dct_acC : NULL) : NULL;
            if (predDir == IPPVC_SCAN_HORIZONTAL && predAcC) {
                if (predAcC[0]) {
                    predQuantC = bCurr->predC->quant;
                    if (predQuantC == bCurr->quant)
                        for (k = 1; k < 8; k ++) {
                            sum0 += mp4_Abs(dcCurr[k]);
                            dcCurr[k] = (Ipp16s)(dcCurr[k] - predAcC[k]);
                            sum1 += mp4_Abs(dcCurr[k]);
                        }
                    else
                        for (k = 1; k < 8; k ++) {
                            sum0 += mp4_Abs(dcCurr[k]);
                            dcCurr[k] = (Ipp16s)(dcCurr[k] - mp4_DivIntraAC(predAcC[k] * predQuantC, bCurr->quant));
                            sum1 += mp4_Abs(dcCurr[k]);
                        }
                }
            } else if (predDir == IPPVC_SCAN_VERTICAL && predAcA) {
                if (predAcA[0]) {
                    predQuantA = bCurr->predA->quant;
                    if (predQuantA == bCurr->quant)
                        for (k = 1; k < 8; k ++) {
                            sum0 += mp4_Abs(dcCurr[k*8]);
                            dcCurr[k*8] = (Ipp16s)(dcCurr[k*8] - predAcA[k]);
                            sum1 += mp4_Abs(dcCurr[k*8]);
                        }
                    else
                        for (k = 1; k < 8; k ++) {
                            sum0 += mp4_Abs(dcCurr[k*8]);
                            dcCurr[k*8] = (Ipp16s)(dcCurr[k*8] - mp4_DivIntraAC(predAcA[k] * predQuantA, bCurr->quant));
                            sum1 += mp4_Abs(dcCurr[k*8]);
                        }
                }
            }
        } else
            predDir = IPPVC_SCAN_ZIGZAG;
        predictDir[i] = predDir;
        dcCurr += 64;
    }
    *pSum0 = sum0;
    *pSum1 = sum1;
}

static void mp4_RestoreIntraAC(mp4_MacroBlock *MBcurr, Ipp16s *dcCurr, Ipp32s *predDir)
{
    Ipp32s      i, k;
    Ipp16s      *predAcA, *predAcC;
    mp4_Block   *bCurr;

    for (i = 0; i < 6; i ++) {
        bCurr = &MBcurr->block[i];
        predAcA = bCurr->predA ? (bCurr->predA->validPredIntra ? bCurr->predA->dct_acA : NULL) : NULL;
        predAcC = bCurr->predC ? (bCurr->predC->validPredIntra ? bCurr->predC->dct_acC : NULL) : NULL;
        if (predDir[i] == IPPVC_SCAN_HORIZONTAL && predAcC) {
            if (predAcC[0] != 0) {
                for (k = 1; k < 8; k ++)
                    dcCurr[k] = bCurr->dct_acC[k];
            }
        } else if (predDir[i] == IPPVC_SCAN_VERTICAL && predAcA) {
            if (predAcA[0] != 0) {
                for (k = 1; k < 8; k ++)
                    dcCurr[k*8] = bCurr->dct_acA[k];
            }
        }
        dcCurr += 64;
    }
}

Ipp32s VideoEncoderMPEG4::TransMacroBlockIntra_H263(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s *nzCount, Ipp32s quant)
{
    Ipp32s   pattern;

    ippiDCT8x8Fwd_8u16s_C1R(pY, mStepLuma, coeffMB+0*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+8, mStepLuma, coeffMB+1*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+8*mStepLuma, mStepLuma, coeffMB+2*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+8*mStepLuma+8, mStepLuma, coeffMB+3*64);
    ippiDCT8x8Fwd_8u16s_C1R(pU, mStepChroma, coeffMB+4*64);
    ippiDCT8x8Fwd_8u16s_C1R(pV, mStepChroma, coeffMB+5*64);
    ippiQuantIntra_H263_16s_C1I(coeffMB+0*64, quant, &nzCount[0], 0, 0);
    ippiQuantIntra_H263_16s_C1I(coeffMB+1*64, quant, &nzCount[1], 0, 0);
    ippiQuantIntra_H263_16s_C1I(coeffMB+2*64, quant, &nzCount[2], 0, 0);
    ippiQuantIntra_H263_16s_C1I(coeffMB+3*64, quant, &nzCount[3], 0, 0);
    ippiQuantIntra_H263_16s_C1I(coeffMB+4*64, quant, &nzCount[4], 0, 0);
    ippiQuantIntra_H263_16s_C1I(coeffMB+5*64, quant, &nzCount[5], 0, 0);
    mp4_SetPatternIntra(pattern, nzCount, coeffMB, 1);
    return pattern;
}

Ipp32s VideoEncoderMPEG4::TransMacroBlockInter_H263(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp16s *coeffMB, Ipp32s *nzCount, Ipp32s quant, Ipp8u *mcPred, Ipp32s lumaErr)
{
    Ipp32s   pattern, sU, sV, sL0, sL1, sL2, sL3, lim;

    lim = quant * 16;
    if (lumaErr < quant * 20) {
        nzCount[0] = nzCount[1] = nzCount[2] = nzCount[3] = 0;
        coeffMB[0*64] = coeffMB[1*64] = coeffMB[2*64] = coeffMB[3*64] = 0;
    } else {
        ippiSubSAD8x8_8u16s_C1R(pYc, mStepLuma, mcPred, 16, coeffMB+0*64, 16, &sL0);
        ippiSubSAD8x8_8u16s_C1R(pYc+8, mStepLuma, mcPred+8, 16, coeffMB+1*64, 16, &sL1);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma, mStepLuma, mcPred+128, 16, coeffMB+2*64, 16, &sL2);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma+8, mStepLuma, mcPred+136, 16, coeffMB+3*64, 16, &sL3);
        if (sL0 < lim) {
            nzCount[0] = 0;
            coeffMB[0*64] = 0;
        } else {
            ippiDCT8x8Fwd_16s_C1I(coeffMB+0*64);
            ippiQuantInter_H263_16s_C1I(coeffMB+0*64, quant, &nzCount[0], 0);
        }
        if (sL1 < lim) {
            nzCount[1] = 0;
            coeffMB[1*64] = 0;
        } else {
            ippiDCT8x8Fwd_16s_C1I(coeffMB+1*64);
            ippiQuantInter_H263_16s_C1I(coeffMB+1*64, quant, &nzCount[1], 0);
        }
        if (sL2 < lim) {
            nzCount[2] = 0;
            coeffMB[2*64] = 0;
        } else {
            ippiDCT8x8Fwd_16s_C1I(coeffMB+2*64);
            ippiQuantInter_H263_16s_C1I(coeffMB+2*64, quant, &nzCount[2], 0);
        }
        if (sL3 < lim) {
            nzCount[3] = 0;
            coeffMB[3*64] = 0;
        } else {
            ippiDCT8x8Fwd_16s_C1I(coeffMB+3*64);
            ippiQuantInter_H263_16s_C1I(coeffMB+3*64, quant, &nzCount[3], 0);
        }
    }
    ippiSubSAD8x8_8u16s_C1R(pUc, mStepChroma, mcPred+64*4, 8, coeffMB+4*64, 16, &sU);
    ippiSubSAD8x8_8u16s_C1R(pVc, mStepChroma, mcPred+64*5, 8, coeffMB+5*64, 16, &sV);
    if (sU < lim) {
        nzCount[4] = 0;
        coeffMB[4*64] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+4*64);
        ippiQuantInter_H263_16s_C1I(coeffMB+4*64, quant, &nzCount[4], 0);
    }
    if (sV < lim) {
        nzCount[5] = 0;
        coeffMB[5*64] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+5*64);
        ippiQuantInter_H263_16s_C1I(coeffMB+5*64, quant, &nzCount[5], 0);
    }
    mp4_SetPatternInter(pattern, nzCount);
    return pattern;
}


int VideoEncoderMPEG4::TransMacroBlockIntra_MPEG4(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s *nzCount, int quant, int row, int col, int *dct_type, int use_intra_dc_vlc, mp4_MacroBlock *MBcurr, int *predDir, int startRow, int *ac_pred, int *pat, int *costRD)
{
    int  pattern, yOff23, yStep, dctt = 0;
    int  ac_pred_flag, acPredSum0, acPredSum1, pattern1;
    __ALIGN16(Ipp16s, coeffFDCT, 64*6);

    if (VOL.interlaced) {
        // boundary MBs should be frame-DCT coded
        if (row != 0 && col != 0 && row != (mNumMacroBlockPerCol-1) && col != (mNumMacroBlockPerRow-1)) {
            Ipp32s  sfr, sfi;
            ippiFrameFieldSAD16x16_8u32s_C1R(pY, mStepLuma, &sfr, &sfi);
            dctt = (sfi + 999) < sfr;
        }
    }
    if (!dctt) {
        yOff23 = mStepLuma * 8;
        yStep = mStepLuma;
    } else {
        yOff23 = mStepLuma;
        yStep = mStepLuma * 2;
    }
    *dct_type = dctt;
    ippiDCT8x8Fwd_8u16s_C1R(pY, yStep, coeffMB+0*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+8, yStep, coeffMB+1*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+yOff23, yStep, coeffMB+2*64);
    ippiDCT8x8Fwd_8u16s_C1R(pY+yOff23+8, yStep, coeffMB+3*64);
    ippiDCT8x8Fwd_8u16s_C1R(pU, mStepChroma, coeffMB+4*64);
    ippiDCT8x8Fwd_8u16s_C1R(pV, mStepChroma, coeffMB+5*64);
    // copy DCT coeff for RD mode decision
    if (costRD != NULL)
        ippsCopy_8u((Ipp8u*)coeffMB, (Ipp8u*)coeffFDCT, 64 * 6 * sizeof(Ipp16s));
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+0*64, mQuantIntraSpec, quant, &nzCount[0], IPPVC_BLOCK_LUMA);
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+1*64, mQuantIntraSpec, quant, &nzCount[1], IPPVC_BLOCK_LUMA);
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+2*64, mQuantIntraSpec, quant, &nzCount[2], IPPVC_BLOCK_LUMA);
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+3*64, mQuantIntraSpec, quant, &nzCount[3], IPPVC_BLOCK_LUMA);
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+4*64, mQuantIntraSpec, quant, &nzCount[4], IPPVC_BLOCK_CHROMA);
    ippiQuantIntra_MPEG4_16s_C1I(coeffMB+5*64, mQuantIntraSpec, quant, &nzCount[5], IPPVC_BLOCK_CHROMA);
    mp4_SetPatternIntra(pattern1, nzCount, coeffMB, 1);
    ac_pred_flag = pattern1 ? 1 : 0;
    MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 1;
    MBcurr->block[0].quant = MBcurr->block[1].quant = MBcurr->block[2].quant = MBcurr->block[3].quant = MBcurr->block[4].quant = MBcurr->block[5].quant = (Ipp8u)quant;
    PredictIntraDCAC(MBcurr, coeffMB, quant, predDir, ac_pred_flag, &acPredSum0, &acPredSum1, nzCount, row - startRow);
    if (ac_pred_flag) {
        // check ac_pred is good
        if (acPredSum0 <= acPredSum1) {
            ac_pred_flag = 0;
            mp4_RestoreIntraAC(MBcurr, coeffMB, predDir);
            predDir[0] = predDir[1] = predDir[2] = predDir[3] = predDir[4] = predDir[5] = IPPVC_SCAN_ZIGZAG;
        } else {
            // re-count the nzCount because they could be changed during AC prediction
            // more optimal way is to correct nzCount during AC prediction
            mp4_NonZeroCount(coeffMB, nzCount);
        }
    }
    mp4_SetPatternIntra(pattern, nzCount, coeffMB, use_intra_dc_vlc);
    *ac_pred = ac_pred_flag;
    *pat = pattern1;
    if (costRD != NULL) {
        int costInter = *costRD;
        int costIntra = 0;
        for (int b = 0; b < 6; b ++) {
            const Ipp8u *scan = VOP.alternate_vertical_scan_flag ? mp4_AltVertScan : (predDir[b] == IPPVC_SCAN_HORIZONTAL) ? mp4_HorScan : (predDir[b] == IPPVC_SCAN_VERTICAL) ? mp4_AltVertScan : mp4_ZigZagScan;
            costIntra += RD_MUL * mp4_CalcBitsCoeffsIntra(coeffMB+b*64, nzCount[b], VOL.reversible_vlc, use_intra_dc_vlc, scan, b);
            coeffMB[b*64] = MBcurr->block[b].dct_dcq;
            ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+b*64, 63, mQuantInvIntraSpec, quant, (b < 4) ? IPPVC_BLOCK_LUMA : IPPVC_BLOCK_CHROMA);
            int ssd = 0;
            //for (int i = 0; i < 64; i ++)
            //    ssd += (coeffMB[b*64+i] - coeffFDCT[b*64+i]) * (coeffMB[b*64+i] - coeffFDCT[b*64+i]);
            Ipp64s n;
            ippsNormDiff_L2Sqr_16s64s_Sfs(coeffMB+b*64, coeffFDCT+b*64, 64, &n, 0);
            ssd = ssd + (int)n;
            costIntra += RD_MUL * ssd / (quant * quant);
            if (costIntra >= costInter)
                break;
        }
        *costRD = costIntra;
    }
    return pattern;
}


int VideoEncoderMPEG4::TransMacroBlockInter_MPEG4(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp16s *coeffMB, Ipp32s *nzCount, int quant, Ipp8u *mcPred, int row, int col, int *dct_type, int trellis, int *costRD)
{
    int   pattern, sE[6], lim, b, costInter;
    Ipp8u *qmat = VOL.quant_type ? VOL.nonintra_quant_mat : NULL;
    const Ipp8u *scan = VOP.alternate_vertical_scan_flag ? mp4_AltVertScan : mp4_ZigZagScan;
    __ALIGN16(Ipp16s, coeffFDCT, 64);

    costInter = 0;
    lim = (VOL.quant_type == 0) ? quant * 16 : quant * 12;
    if (VOL.interlaced) {
        __ALIGN16(Ipp16s, coeff, 64*4);
        int   off23, s, dctt = 0, off[4];

        ippiSub16x16_8u16s_C1R(pYc, mStepLuma, mcPred, 16, coeff, 32);
        // boundary MBs should be frame-DCT coded
        if (row != 0 && col != 0 && row != (mNumMacroBlockPerCol-1) && col != (mNumMacroBlockPerRow-1)) {
            Ipp32s  sfr, sfi;
            ippiFrameFieldSAD16x16_16s32s_C1R(coeff, 32, &sfr, &sfi);
            dctt = (sfi + 999) < sfr;
        }
        *dct_type = dctt;
        if (dctt) {
            ippiSAD8x8_8u32s_C1R(pYc, 2*mStepLuma, mcPred, 32, &sE[0], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+8, 2*mStepLuma, mcPred+8, 32, &sE[1], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+mStepLuma, 2*mStepLuma, mcPred+16, 32, &sE[2], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+mStepLuma+8, 2*mStepLuma, mcPred+24, 32, &sE[3], IPPVC_MC_APX_FF);
            off23 = 16;
            s = 64;
        } else {
            ippiSAD8x8_8u32s_C1R(pYc, mStepLuma, mcPred, 16, &sE[0], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+8, mStepLuma, mcPred+8, 16, &sE[1], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+8*mStepLuma, mStepLuma, mcPred+128, 16, &sE[2], IPPVC_MC_APX_FF);
            ippiSAD8x8_8u32s_C1R(pYc+8*mStepLuma+8, mStepLuma, mcPred+136, 16, &sE[3], IPPVC_MC_APX_FF);
            off23 = 2*64;
            s = 32;
        }
        off[0] = 0; off[1] = 8; off[2] = off23; off[3] = off23 + 8;
        for (b = 0; b < 4; b ++) {
            if (sE[b] < lim) {
                nzCount[b] = 0;
            } else {
                ippiDCT8x8Fwd_16s_C1R(coeff+off[b], s, coeffMB+b*64);
                if (trellis || (costRD != NULL))
                    ippsCopy_8u((Ipp8u*)(coeffMB+b*64), (Ipp8u*)coeffFDCT, 64 * sizeof(Ipp16s));
                ippiQuantInter_MPEG4_16s_C1I(coeffMB+b*64, mQuantInterSpec, quant, &nzCount[b]);
                if (trellis && (nzCount[b] != 0))
                    nzCount[b] = mp4_TrellisQuant(coeffFDCT, coeffMB+b*64, quant, qmat, scan, nzCount[b]);
                if (costRD != NULL) {
                    if (nzCount[b] != 0) {
                        costInter += RD_MUL * mp4_CalcBitsCoeffsInter(coeffMB+b*64, nzCount[b], VOL.reversible_vlc, scan);
                        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+b*64, 63, mQuantInvInterSpec, quant);
                    }
                    int ssd = 0;
                    Ipp64s n;
                    ippsNormDiff_L2Sqr_16s64s_Sfs(coeffMB+b*64, coeffFDCT, 64, &n, 0);
                    ssd = ssd + (int)n;
                    //for (int i = 0; i < 64; i ++)
                    //    ssd += (coeffMB[b*64+i] - coeffFDCT[i]) * (coeffMB[b*64+i] - coeffFDCT[i]);
                    costInter += RD_MUL * ssd / (quant * quant);
                }
            }
        }
    } else {
        *dct_type = 0;
        ippiSubSAD8x8_8u16s_C1R(pYc, mStepLuma, mcPred, 16, coeffMB+0*64, 16, &sE[0]);
        ippiSubSAD8x8_8u16s_C1R(pYc+8, mStepLuma, mcPred+8, 16, coeffMB+1*64, 16, &sE[1]);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma, mStepLuma, mcPred+128, 16, coeffMB+2*64, 16, &sE[2]);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma+8, mStepLuma, mcPred+136, 16, coeffMB+3*64, 16, &sE[3]);
        for (b = 0; b < 4; b ++) {
            if (sE[b] < lim) {
                nzCount[b] = 0;
            } else {
                ippiDCT8x8Fwd_16s_C1I(coeffMB+b*64);
                if (trellis || (costRD != NULL))
                    ippsCopy_8u((Ipp8u*)(coeffMB+b*64), (Ipp8u*)coeffFDCT, 64 * sizeof(Ipp16s));
                ippiQuantInter_MPEG4_16s_C1I(coeffMB+b*64, mQuantInterSpec, quant, &nzCount[b]);
                if (trellis && (nzCount[b] != 0))
                    nzCount[b] = mp4_TrellisQuant(coeffFDCT, coeffMB+b*64, quant, qmat, scan, nzCount[b]);
                if (costRD != NULL) {
                    if (nzCount[b] != 0) {
                        costInter += RD_MUL * mp4_CalcBitsCoeffsInter(coeffMB+b*64, nzCount[b], VOL.reversible_vlc, scan);
                        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+b*64, 63, mQuantInvInterSpec, quant);
                    }
                    int ssd = 0;
                    Ipp64s n;
                    ippsNormDiff_L2Sqr_16s64s_Sfs(coeffMB+b*64, coeffFDCT, 64, &n, 0);
                    ssd = ssd + (int)n;
                    //for (int i = 0; i < 64; i ++)
                    //    ssd += (coeffMB[b*64+i] - coeffFDCT[i]) * (coeffMB[b*64+i] - coeffFDCT[i]);
                    costInter += RD_MUL * ssd / (quant * quant);
                }
            }
        }
    }
    ippiSubSAD8x8_8u16s_C1R(pUc, mStepChroma, mcPred+64*4, 8, coeffMB+4*64, 16, &sE[4]);
    ippiSubSAD8x8_8u16s_C1R(pVc, mStepChroma, mcPred+64*5, 8, coeffMB+5*64, 16, &sE[5]);
    for (b = 4; b < 6; b ++) {
        if (sE[b] < lim) {
            nzCount[b] = 0;
        } else {
            ippiDCT8x8Fwd_16s_C1I(coeffMB+b*64);
            if (trellis || (costRD != NULL))
                ippsCopy_8u((Ipp8u*)(coeffMB+b*64), (Ipp8u*)coeffFDCT, 64 * sizeof(Ipp16s));
            ippiQuantInter_MPEG4_16s_C1I(coeffMB+b*64, mQuantInterSpec, quant, &nzCount[b]);
            if (trellis && (nzCount[b] != 0))
                nzCount[b] = mp4_TrellisQuant(coeffFDCT, coeffMB+b*64, quant, qmat, scan, nzCount[b]);
            if (costRD != NULL) {
                if (nzCount[b] != 0) {
                    costInter += RD_MUL * mp4_CalcBitsCoeffsInter(coeffMB+b*64, nzCount[b], VOL.reversible_vlc, scan);
                    ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+b*64, 63, mQuantInvInterSpec, quant);
                }
                int ssd = 0;
                Ipp64s n;
                ippsNormDiff_L2Sqr_16s64s_Sfs(coeffMB+b*64, coeffFDCT, 64, &n, 0);
                ssd = ssd + (int)n;
                //for (int i = 0; i < 64; i ++)
                //    ssd += (coeffMB[b*64+i] - coeffFDCT[i]) * (coeffMB[b*64+i] - coeffFDCT[i]);
                costInter += RD_MUL * ssd / (quant * quant);
            }
        }
    }
#if 0
    __ALIGN16(Ipp16s, coeff, 64*4);
    Ipp32s   pattern, sU, sV, sL0, sL1, sL2, sL3, lim, off23, s, dctt = 0;

    lim = (VOL.quant_type == 0) ? quant * 16 : quant * 12;
    ippiSub16x16_8u16s_C1R(pYc, mStepLuma, mcPred, 16, coeff, 32);
    if (VOL.interlaced) {
        // boundary MBs should be frame-DCT coded
        if (row != 0 && col != 0 && row != (mNumMacroBlockPerCol-1) && col != (mNumMacroBlockPerRow-1)) {
            Ipp32s  sfr, sfi;
            ippiFrameFieldSAD16x16_16s32s_C1R(coeff, 32, &sfr, &sfi);
            dctt = (sfi + 999) < sfr;
        }
    }
    if (dctt) {
        ippiSAD8x8_8u32s_C1R(pYc, 2*mStepLuma, mcPred, 32, &sL0, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+8, 2*mStepLuma, mcPred+8, 32, &sL1, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+mStepLuma, 2*mStepLuma, mcPred+16, 32, &sL2, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+mStepLuma+8, 2*mStepLuma, mcPred+24, 32, &sL3, IPPVC_MC_APX_FF);
        off23 = 16;
        s = 64;
    } else {
        ippiSAD8x8_8u32s_C1R(pYc, mStepLuma, mcPred, 16, &sL0, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+8, mStepLuma, mcPred+8, 16, &sL1, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+8*mStepLuma, mStepLuma, mcPred+128, 16, &sL2, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pYc+8*mStepLuma+8, mStepLuma, mcPred+136, 16, &sL3, IPPVC_MC_APX_FF);
        off23 = 2*64;
        s = 32;
    }
    *dct_type = dctt;
    if (sL0 < lim) {
        nzCount[0] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1R(coeff, s, coeffMB+0*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+0*64, mQuantInterSpec, quant, &nzCount[0]);
    }
    if (sL1 < lim) {
        nzCount[1] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1R(coeff+8, s, coeffMB+1*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+1*64, mQuantInterSpec, quant, &nzCount[1]);
    }
    if (sL2 < lim) {
        nzCount[2] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1R(coeff+off23, s, coeffMB+2*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+2*64, mQuantInterSpec, quant, &nzCount[2]);
    }
    if (sL3 < lim) {
        nzCount[3] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1R(coeff+off23+8, s, coeffMB+3*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+3*64, mQuantInterSpec, quant, &nzCount[3]);
    }
    ippiSAD8x8_8u32s_C1R(pUc, mStepChroma, mcPred+64*4, 8, &sU, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pVc, mStepChroma, mcPred+64*5, 8, &sV, IPPVC_MC_APX_FF);
    if (sU < lim) {
        nzCount[4] = 0;
    } else {
        ippiSub8x8_8u16s_C1R(pUc, mStepChroma, mcPred+64*4, 8, coeffMB+4*64, 16);
        ippiDCT8x8Fwd_16s_C1I(coeffMB+4*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+4*64, mQuantInterSpec, quant, &nzCount[4]);
    }
    if (sV < lim) {
        nzCount[5] = 0;
    } else {
        ippiSub8x8_8u16s_C1R(pVc, mStepChroma, mcPred+64*5, 8, coeffMB+5*64, 16);
        ippiDCT8x8Fwd_16s_C1I(coeffMB+5*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+5*64, mQuantInterSpec, quant, &nzCount[5]);
    }
/*
    lim = (VOL.quant_type == 0) ? quant * 16 : quant * 12;
    if (!dct_type) {
        ippiSubSAD8x8_8u16s_C1R(pYc, mStepLuma, mcPred, 16, coeffMB+0*64, 16, &sL0);
        ippiSubSAD8x8_8u16s_C1R(pYc+8, mStepLuma, mcPred+8, 16, coeffMB+1*64, 16, &sL1);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma, mStepLuma, mcPred+128, 16, coeffMB+2*64, 16, &sL2);
        ippiSubSAD8x8_8u16s_C1R(pYc+8*mStepLuma+8, mStepLuma, mcPred+136, 16, coeffMB+3*64, 16, &sL3);
    } else {
        ippiSubSAD8x8_8u16s_C1R(pYc, 2*mStepLuma, mcPred, 32, coeffMB+0*64, 16, &sL0);
        ippiSubSAD8x8_8u16s_C1R(pYc+8, 2*mStepLuma, mcPred+8, 32, coeffMB+1*64, 16, &sL1);
        ippiSubSAD8x8_8u16s_C1R(pYc+mStepLuma, 2*mStepLuma, mcPred+16, 32, coeffMB+2*64, 16, &sL2);
        ippiSubSAD8x8_8u16s_C1R(pYc+mStepLuma+8, 2*mStepLuma, mcPred+24, 32, coeffMB+3*64, 16, &sL3);
    }
    if (sL0 < lim) {
        nzCount[0] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+0*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+0*64, mQuantInterSpec, quant, &nzCount[0]);
    }
    if (sL1 < lim) {
        nzCount[1] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+1*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+1*64, mQuantInterSpec, quant, &nzCount[1]);
    }
    if (sL2 < lim) {
        nzCount[2] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+2*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+2*64, mQuantInterSpec, quant, &nzCount[2]);
    }
    if (sL3 < lim) {
        nzCount[3] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+3*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+3*64, mQuantInterSpec, quant, &nzCount[3]);
    }
    ippiSubSAD8x8_8u16s_C1R(pUc, mStepChroma, mcPred+64*4, 8, coeffMB+4*64, 16, &sU);
    ippiSubSAD8x8_8u16s_C1R(pVc, mStepChroma, mcPred+64*5, 8, coeffMB+5*64, 16, &sV);
    if (sU < lim) {
        nzCount[4] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+4*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+4*64, mQuantInterSpec, quant, &nzCount[4]);
    }
    if (sV < lim) {
        nzCount[5] = 0;
    } else {
        ippiDCT8x8Fwd_16s_C1I(coeffMB+5*64);
        ippiQuantInter_MPEG4_16s_C1I(coeffMB+5*64, mQuantInterSpec, quant, &nzCount[5]);
    }
*/
#endif
    if (costRD != NULL)
        *costRD = costInter;
    mp4_SetPatternInter(pattern, nzCount);
    return pattern;
}

inline void VideoEncoderMPEG4::ReconMacroBlockNotCoded(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred)
{
    ippiCopy16x16_8u_C1R(mcPred, 16, pYc, mStepLuma);
    ippiCopy8x8_8u_C1R(mcPred+64*4, 8, pUc, mStepChroma);
    ippiCopy8x8_8u_C1R(mcPred+64*5, 8, pVc, mStepChroma);
}

void VideoEncoderMPEG4::ReconMacroBlockIntra_H263(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern)
{
    if (pattern & 32) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+0*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+0*64, pY, mStepLuma);
    } else {
        mp4_Set8x8_8u(pY, mStepLuma, (Ipp8u)coeffMB[0*64]);
    }
    if (pattern & 16) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+1*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+1*64, pY+8, mStepLuma);
    } else {
        mp4_Set8x8_8u(pY+8, mStepLuma, (Ipp8u)coeffMB[1*64]);
    }
    if (pattern & 8) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+2*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+2*64, pY+8*mStepLuma, mStepLuma);
    } else {
        mp4_Set8x8_8u(pY+8*mStepLuma, mStepLuma, (Ipp8u)coeffMB[2*64]);
    }
    if (pattern & 4) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+3*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+3*64, pY+8*mStepLuma+8, mStepLuma);
    } else {
        mp4_Set8x8_8u(pY+8*mStepLuma+8, mStepLuma, (Ipp8u)coeffMB[3*64]);
    }
    if (pattern & 2) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+4*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+4*64, pU, mStepChroma);
    } else {
        mp4_Set8x8_8u(pU, mStepChroma, (Ipp8u)coeffMB[4*64]);
    }
    if (pattern & 1) {
        ippiQuantInvIntra_H263_16s_C1I(coeffMB+5*64, 63, quant, 0, 0);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+5*64, pV, mStepChroma);
    } else {
        mp4_Set8x8_8u(pV, mStepChroma, (Ipp8u)coeffMB[5*64]);
    }
}

void VideoEncoderMPEG4::ReconMacroBlockInter_H263(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern)
{
    if (pattern & 32) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+0*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+0*64);
        mp4_Add8x8_16s8u(mcPred, coeffMB+0*64, 16);
    }
    if (pattern & 16) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+1*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+1*64);
        mp4_Add8x8_16s8u(mcPred+8, coeffMB+1*64, 16);
    }
    if (pattern & 8) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+2*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+2*64);
        mp4_Add8x8_16s8u(mcPred+16*8, coeffMB+2*64, 16);
    }
    if (pattern & 4) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+3*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+3*64);
        mp4_Add8x8_16s8u(mcPred+16*8+8, coeffMB+3*64, 16);
    }
    if (pattern & 2) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+4*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+4*64);
        mp4_Add8x8_16s8u(mcPred+64*4, coeffMB+4*64, 8);
    }
    if (pattern & 1) {
        ippiQuantInvInter_H263_16s_C1I(coeffMB+5*64, 63, quant, 0);
        ippiDCT8x8Inv_16s_C1I(coeffMB+5*64);
        mp4_Add8x8_16s8u(mcPred+64*5, coeffMB+5*64, 8);
    }
    ippiCopy16x16_8u_C1R(mcPred, 16, pYc, mStepLuma);
    ippiCopy8x8_8u_C1R(mcPred+64*4, 8, pUc, mStepChroma);
    ippiCopy8x8_8u_C1R(mcPred+64*5, 8, pVc, mStepChroma);
}

void VideoEncoderMPEG4::ReconMacroBlockIntra_MPEG4(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s quant, mp4_MacroBlock *MBcurr, Ipp32s pattern, Ipp32s dct_type)
{
    Ipp32s   yOff23, yStep, dc;

    if (!dct_type) {
        yOff23 = mStepLuma * 8;
        yStep = mStepLuma;
    } else {
        yOff23 = mStepLuma;
        yStep = mStepLuma * 2;
    }
    coeffMB[0*64] = MBcurr->block[0].dct_dcq;
    coeffMB[1*64] = MBcurr->block[1].dct_dcq;
    coeffMB[2*64] = MBcurr->block[2].dct_dcq;
    coeffMB[3*64] = MBcurr->block[3].dct_dcq;
    coeffMB[4*64] = MBcurr->block[4].dct_dcq;
    coeffMB[5*64] = MBcurr->block[5].dct_dcq;
    if (pattern & 32) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+0*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_LUMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+0*64, pY, yStep);
    } else {
        dc = coeffMB[0*64] * mp4_DCScalerLuma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pY, yStep, (Ipp8u)dc);
    }
    if (pattern & 16) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+1*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_LUMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+1*64, pY+8, yStep);
    } else {
        dc = coeffMB[1*64] * mp4_DCScalerLuma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pY+8, yStep, (Ipp8u)dc);
    }
    if (pattern & 8) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+2*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_LUMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+2*64, pY+yOff23, yStep);
    } else {
        dc = coeffMB[2*64] * mp4_DCScalerLuma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pY+yOff23, yStep, (Ipp8u)dc);
    }
    if (pattern & 4) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+3*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_LUMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+3*64, pY+yOff23+8, yStep);
    } else {
        dc = coeffMB[3*64] * mp4_DCScalerLuma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pY+yOff23+8, yStep, (Ipp8u)dc);
    }
    if (pattern & 2) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+4*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_CHROMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+4*64, pU, mStepChroma);
    } else {
        dc = coeffMB[4*64] * mp4_DCScalerChroma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pU, mStepChroma, (Ipp8u)dc);
    }
    if (pattern & 1) {
        ippiQuantInvIntra_MPEG4_16s_C1I(coeffMB+5*64, 63, mQuantInvIntraSpec, quant, IPPVC_BLOCK_CHROMA);
        ippiDCT8x8Inv_16s8u_C1R(coeffMB+5*64, pV, mStepChroma);
    } else {
        dc = coeffMB[5*64] * mp4_DCScalerChroma[quant];
        dc = (dc + 4) >> 3;
        mp4_ClipR(dc, 255);
        mp4_Set8x8_8u(pV, mStepChroma, (Ipp8u)dc);
    }
}

void VideoEncoderMPEG4::ReconMacroBlockInter_MPEG4(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern, Ipp32s dct_type)
{
    Ipp32s  yOff23, yStep;

    if (!dct_type) {
        yOff23 = 16 * 8;
        yStep = 16;
    } else {
        yOff23 = 16;
        yStep = 16 * 2;
    }
    if (pattern & 32) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+0*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+0*64);
        mp4_Add8x8_16s8u(mcPred, coeffMB+0*64, yStep);
    }
    if (pattern & 16) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+1*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+1*64);
        mp4_Add8x8_16s8u(mcPred+8, coeffMB+1*64, yStep);
    }
    if (pattern & 8) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+2*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+2*64);
        mp4_Add8x8_16s8u(mcPred+yOff23, coeffMB+2*64, yStep);
    }
    if (pattern & 4) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+3*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+3*64);
        mp4_Add8x8_16s8u(mcPred+yOff23+8, coeffMB+3*64, yStep);
    }
    if (pattern & 2) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+4*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+4*64);
        mp4_Add8x8_16s8u(mcPred+64*4, coeffMB+4*64, 8);
    }
    if (pattern & 1) {
        ippiQuantInvInter_MPEG4_16s_C1I(coeffMB+5*64, 63, mQuantInvInterSpec, quant);
        ippiDCT8x8Inv_16s_C1I(coeffMB+5*64);
        mp4_Add8x8_16s8u(mcPred+64*5, coeffMB+5*64, 8);
    }
    ippiCopy16x16_8u_C1R(mcPred, 16, pYc, mStepLuma);
    ippiCopy8x8_8u_C1R(mcPred+64*4, 8, pUc, mStepChroma);
    ippiCopy8x8_8u_C1R(mcPred+64*5, 8, pVc, mStepChroma);
}

#ifdef _OMP_KARABAS
static void mp4_CopyCoeffsIntra(Ipp16s *s, Ipp16s *d, Ipp32s pat)
{
    Ipp32s  i, b;

    for (b = 0; b < 6; b ++) {
        // not correct for !use_intra_dc_vlc
        if (pat & 32) {
            for (i = 0; i < 16; i += 4) {
                ((Ipp64u*)d)[i+0] = ((Ipp64u*)s)[i+0];
                ((Ipp64u*)d)[i+1] = ((Ipp64u*)s)[i+1];
                ((Ipp64u*)d)[i+2] = ((Ipp64u*)s)[i+2];
                ((Ipp64u*)d)[i+3] = ((Ipp64u*)s)[i+3];
            }
        } else {
            d[0] = s[0];
        }
        s += 64;
        d += 64;
        pat += pat;
    }
}

static void mp4_CopyCoeffsInter(Ipp16s *s, Ipp16s *d, Ipp32s pat)
{
    Ipp32s  i, b;

    for (b = 0; b < 6; b ++) {
        if (pat & 32) {
            for (i = 0; i < 16; i += 4) {
                ((Ipp64u*)d)[i+0] = ((Ipp64u*)s)[i+0];
                ((Ipp64u*)d)[i+1] = ((Ipp64u*)s)[i+1];
                ((Ipp64u*)d)[i+2] = ((Ipp64u*)s)[i+2];
                ((Ipp64u*)d)[i+3] = ((Ipp64u*)s)[i+3];
            }
        }
        s += 64;
        d += 64;
        pat += pat;
    }
}

#define mp4_WaitIntra(curRow) \
    if (curRow > 0) { \
        for (;;) { \
            Ipp32s volatile r = mCurRowMT[curRow - 1]; \
            if (j <= r) \
                break; \
        } \
    }
#endif // _OMP_KARABAS

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodeIRowSH(Ipp32s curRow, Ipp32s threadNum, Ipp32s *nmb)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    Ipp8u  *pY, *pU, *pV;
    Ipp32s  j, quant, pattern, nmbf;
    mp4_MacroBlock *MBcurr;
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    quant = VOP.vop_quant;
    pY = mFrameC->pY + curRow * 16 * mStepLuma;
    pU = mFrameC->pU + curRow * 8 * mStepChroma;
    pV = mFrameC->pV + curRow * 8 * mStepChroma;
    nmbf = curRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + nmbf;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        pMBinfoMT->pat = TransMacroBlockIntra_H263(pY, pU, pV, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant);
        if (mIVOPdist != 1 || mReconstructAlways) {
            mp4_CopyCoeffsIntra(pMBinfoMT->dctCoeffs, coeffMB, pMBinfoMT->pat);
            ReconMacroBlockIntra_H263(pY, pU, pV, coeffMB, quant, pMBinfoMT->pat);
        }
        pMBinfoMT ++;
        pY += 16; pU += 8; pV += 8;
        MBcurr->mv[0].dx = MBcurr->mv[0].dy = 0;
        MBcurr ++;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        // for RTP support
        mMBpos[nmbf] = 8 * (Ipp32u)(cBS.mPtr - cBS.mBuffer) + cBS.mBitOff;
        mMBquant[nmbf] = (Ipp8u)quant;
        nmbf ++;
        pattern = pMBinfoMT->pat;
        mp4_EncodeMCBPC_I(cBS, IPPVC_MBTYPE_INTRA, pattern & 3);
        mp4_EncodeCBPY_I(cBS, pattern >> 2);
        mp4_EncodeMacroBlockIntra_H263(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount);
        pMBinfoMT ++;
        if (mInsertResync) {
            (*nmb) ++;
            if ((*nmb == VOP.num_macroblocks_in_gob) && (VOP.gob_number < VOP.num_gobs_in_vop)) {
                // write gob_resync_marker
                mp4_EncodeZeroBitsAlign(cBS);
                EncodeGOBHeader(cBS, VOP.gob_number, quant);
                VOP.gob_number ++;
                *nmb = 0;
            }
        }
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodeISliceSH(mp4_Slice *slice)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    Ipp8u  *pY, *pU, *pV;
    Ipp32s  i, j, quant, pattern, startRow, gob_number;
    Ipp32s  nzCount[6];
    mp4_MacroBlock *MBcurr;
    Ipp32s  nmb, nmbf, dquant;

    startRow = slice->startRow;
    gob_number = slice->gob_number;
    nmbf = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + nmbf;
    quant = VOP.vop_quant;
    dquant = 0;
    nmb = 0;
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_I, quant);
    if (startRow > 0)
        EncodeGOBHeader(slice->cBS, gob_number, quant);
    gob_number ++;
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pY = mFrameC->pY + i * 16 * mStepLuma;
        pU = mFrameC->pU + i * 8 * mStepChroma;
        pV = mFrameC->pV + i * 8 * mStepChroma;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            MBcurr->mv[0].dx = MBcurr->mv[0].dy = 0;
            // for RTP support
            mMBpos[nmbf] = slice->cBS.GetNumBits();
            mMBquant[nmbf] = (Ipp8u)quant;
            quant += dquant;
            slice->quantSum += quant;
            pattern = TransMacroBlockIntra_H263(pY, pU, pV, coeffMB, nzCount, quant);
            mp4_EncodeMCBPC_I(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q, pattern & 3);
            mp4_EncodeCBPY_I(slice->cBS, pattern >> 2);
            if (dquant != 0)
                mp4_EncodeDquant(slice->cBS, dquant);
            mp4_EncodeMacroBlockIntra_H263(slice->cBS, coeffMB, pattern, nzCount);
            if (mIVOPdist != 1 || mReconstructAlways)
                ReconMacroBlockIntra_H263(pY, pU, pV, coeffMB, quant, pattern);
            if (mInsertResync) {
                nmb ++;
                if ((nmb == VOP.num_macroblocks_in_gob) && (i < startRow + slice->numRow - 1)) {
                    // write gob_resync_marker
                    mp4_EncodeZeroBitsAlign(slice->cBS);
                    EncodeGOBHeader(slice->cBS, gob_number, quant);
                    gob_number ++;
                    nmb = 0;
                }
            }
            if (mRateControl == 2)
                mRC_MB.Update(slice, &dquant, MP4_VOP_TYPE_I);
            pY += 16; pU += 8; pV += 8;
            MBcurr ++;
            nmbf ++;
        }
    }
    mp4_EncodeZeroBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodeIVOPSH()
{
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if (mThreadingAlg == 0 && mRateControl != 2) {
            Ipp32s  i, nmb;

            VOP.gob_number = 1;
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            nmb = 0;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodeIRowSH(curRow, mp4_MT_get_thread_num(), &nmb);
                    curRow ++;
                }
            }
            mp4_EncodeZeroBitsAlign(cBS);
        } else {
            Ipp32s  i, j, k;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodeISliceSH(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodeISliceSH(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            cBS = mSliceMT[0].cBS;
            k = mSliceMT[0].cBS.GetFullness() << 3;
            for (i = 1; i < mNumThreads; i ++) {
                // update MB positions for RTP
                for (j = 0; j < mSliceMT[i].numRow * mNumMacroBlockPerRow; j ++)
                    mMBpos[mSliceMT->startRow * mNumMacroBlockPerRow + j] += k;
                k += mSliceMT[i].cBS.GetFullness() << 3;
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
            }
        }
    } else
#endif // _OMP_KARABAS
    {
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodeISliceSH(&mSlice);
        mQuantSum = mSlice.quantSum;
        cBS = mSlice.cBS;
    }
}

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodePRowSH(Ipp32s curRow, Ipp32s threadNum, Ipp32s *nmb)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s  j, quant, pattern, nmbf;
    mp4_MacroBlock *MBcurr;
    IppMotionVector mvLuma, mvChroma, mvPred;
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    quant = VOP.vop_quant;
    pYc = mFrameC->pY + curRow * 16 * mStepLuma;
    pUc = mFrameC->pU + curRow * 8 * mStepChroma;
    pVc = mFrameC->pV + curRow * 8 * mStepChroma;
    pYf = mFrameF->pY + curRow * 16 * mStepLuma;
    pUf = mFrameF->pU + curRow * 8 * mStepChroma;
    pVf = mFrameF->pV + curRow * 8 * mStepChroma;
    nmbf = curRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + nmbf;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        if (MBcurr->not_coded) {
            ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
            ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
            ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
        } else {
            if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                pMBinfoMT->pat = TransMacroBlockIntra_H263(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant);
                mp4_CopyCoeffsIntra(pMBinfoMT->dctCoeffs, coeffMB, pMBinfoMT->pat);
                ReconMacroBlockIntra_H263(pYc, pUc, pVc, coeffMB, quant, pMBinfoMT->pat);
            } else {
                mvLuma = MBcurr->mv[0];
                mp4_ComputeChromaMV(&mvLuma, &mvChroma);
                mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, 0);
                mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, 0);
                mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, 0);
                pMBinfoMT->pat = TransMacroBlockInter_H263(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, mcPred, MBcurr->lumaErr);
                if (pMBinfoMT->pat == 0 && mvLuma.dx == 0 && mvLuma.dy == 0)
                    MBcurr->not_coded = 1;
                if (pMBinfoMT->pat) {
                    mp4_CopyCoeffsInter(pMBinfoMT->dctCoeffs, coeffMB, pMBinfoMT->pat);
                    ReconMacroBlockInter_H263(pYc, pUc, pVc, mcPred, coeffMB, quant, pMBinfoMT->pat);
                } else
                    ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
            }
        }
        pMBinfoMT ++;
        pYc += 16; pUc += 8; pVc += 8;
        pYf += 16; pUf += 8; pVf += 8;
        MBcurr ++;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    MBcurr -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        // for RTP support
        mMBpos[nmbf] = 8 * (Ipp32u)(cBS.mPtr - cBS.mBuffer) + cBS.mBitOff;
        mMBquant[nmbf] = (Ipp8u)quant;
        if (MBcurr->not_coded) {
            cBS.PutBit(1);
        } else {
            cBS.PutBit(0);
            pattern = pMBinfoMT->pat;
            if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                mp4_EncodeMCBPC_P(cBS, IPPVC_MBTYPE_INTRA, pattern & 3);
                mp4_EncodeCBPY_P(cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                mp4_EncodeMacroBlockIntra_H263(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount);
            } else {
                mp4_EncodeMCBPC_P(cBS, IPPVC_MBTYPE_INTER, pattern & 3);
                mp4_EncodeCBPY_P(cBS, IPPVC_MBTYPE_INTER, pattern >> 2);
                PredictMV(MBcurr, curRow, j, &mvPred);
                // for RTP support
                mMBpredMV[nmbf] = mvPred;
                mp4_MV_GetDiff(&MBcurr->mv[0], &mvPred, -32, 31, 64);
                mp4_EncodeMV(cBS, &mvPred, 1, IPPVC_MBTYPE_INTER);
                mp4_EncodeMacroBlockInter_H263(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount);
            }
        }
        nmbf ++;
        pMBinfoMT ++;
        MBcurr ++;
        if (mInsertResync) {
            (*nmb) ++;
            if ((*nmb == VOP.num_macroblocks_in_gob) && (VOP.gob_number < VOP.num_gobs_in_vop)) {
                // write gob_resync_marker
                mp4_EncodeZeroBitsAlign(cBS);
                EncodeGOBHeader(cBS, VOP.gob_number, quant);
                VOP.gob_number ++;
                *nmb = 0;
            }
        }
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodePSliceSH(mp4_Slice *slice)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s  i, j, quant, pattern, nmb, nmbf, dquant, startRow, gob_number;
    Ipp32s  nzCount[6];
    mp4_MacroBlock *MBcurr;
    IppMotionVector mvLuma, mvChroma, mvPred;

    startRow = slice->startRow;
    gob_number = slice->gob_number;
    nmbf = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + nmbf;
    quant = VOP.vop_quant;
    dquant = 0;
    nmb = 0;
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_P, quant);
    if (startRow > 0)
        EncodeGOBHeader(slice->cBS, gob_number, quant);
    gob_number ++;
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pYc = mFrameC->pY + i * 16 * mStepLuma;
        pUc = mFrameC->pU + i * 8 * mStepChroma;
        pVc = mFrameC->pV + i * 8 * mStepChroma;
        pYf = mFrameF->pY + i * 16 * mStepLuma;
        pUf = mFrameF->pU + i * 8 * mStepChroma;
        pVf = mFrameF->pV + i * 8 * mStepChroma;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            // for RTP support
            mMBpos[nmbf] = (Ipp32u)(8 * (slice->cBS.mPtr - slice->cBS.mBuffer) + slice->cBS.mBitOff);
            mMBquant[nmbf] = (Ipp8u)quant;
            quant += dquant;
            slice->quantSum += quant;
            if (MBcurr->not_coded && quant >= VOP.vop_quant && dquant == 0) {
                // encode not_coded
                slice->cBS.PutBit(1);
                ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
                ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
                ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
            } else {
                if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                    // encode not_coded
                    slice->cBS.PutBit(0);
                    pattern = TransMacroBlockIntra_H263(pYc, pUc, pVc, coeffMB, nzCount, quant);
                    mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q, pattern & 3);
                    mp4_EncodeCBPY_P(slice->cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                    if (dquant != 0)
                        mp4_EncodeDquant(slice->cBS, dquant);
                    mp4_EncodeMacroBlockIntra_H263(slice->cBS, coeffMB, pattern, nzCount);
                    ReconMacroBlockIntra_H263(pYc, pUc, pVc, coeffMB, quant, pattern);
                } else {
                    mvLuma = MBcurr->mv[0];
                    mp4_ComputeChromaMV(&mvLuma, &mvChroma);
                    mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, 0);
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, 0);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, 0);
                    pattern = TransMacroBlockInter_H263(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, MBcurr->lumaErr);
                    if (pattern == 0 && mvLuma.dx == 0 && mvLuma.dy == 0 && dquant == 0) {
                        MBcurr->not_coded = 1;
                        // encode not_coded
                        slice->cBS.PutBit(1);
                        ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                    } else {
                        // ESC code restriction [-127;127]
                        if (coeffMB[  0] < -127 || coeffMB[  0] > 127 ||
                            coeffMB[ 64] < -127 || coeffMB[ 64] > 127 ||
                            coeffMB[128] < -127 || coeffMB[128] > 127 ||
                            coeffMB[192] < -127 || coeffMB[192] > 127 ||
                            coeffMB[256] < -127 || coeffMB[256] > 127 ||
                            coeffMB[320] < -127 || coeffMB[320] > 127) {
                                // intra coded
                                MBcurr->type = IPPVC_MBTYPE_INTRA;
                                MBcurr->mv[0].dx = MBcurr->mv[0].dy = 0;
                                // encode not_coded
                                slice->cBS.PutBit(0);
                                pattern = TransMacroBlockIntra_H263(pYc, pUc, pVc, coeffMB, nzCount, quant);
                                mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q, pattern & 3);
                                mp4_EncodeCBPY_P(slice->cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                                if (dquant != 0)
                                    mp4_EncodeDquant(slice->cBS, dquant);
                                mp4_EncodeMacroBlockIntra_H263(slice->cBS, coeffMB, pattern, nzCount);
                                ReconMacroBlockIntra_H263(pYc, pUc, pVc, coeffMB, quant, pattern);
                            } else {
                                // encode not_coded
                                slice->cBS.PutBit(0);
                                mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTER : IPPVC_MBTYPE_INTER_Q, pattern & 3);
                                mp4_EncodeCBPY_P(slice->cBS, IPPVC_MBTYPE_INTER, pattern >> 2);
                                if (dquant != 0)
                                    mp4_EncodeDquant(slice->cBS, dquant);
                                PredictMV(MBcurr, i, j, &mvPred);
                                // for RTP support
                                mMBpredMV[nmbf] = mvPred;
                                mp4_MV_GetDiff(&mvLuma, &mvPred, -32, 31, 64);
                                mp4_EncodeMV(slice->cBS, &mvPred, 1, IPPVC_MBTYPE_INTER);
                                mp4_EncodeMacroBlockInter_H263(slice->cBS, coeffMB, pattern, nzCount);
                                if (pattern)
                                    ReconMacroBlockInter_H263(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern);
                                else
                                    ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                            }
                    }
                }
            }
            if (mInsertResync) {
                nmb ++;
                if ((nmb == VOP.num_macroblocks_in_gob) && (i < startRow + slice->numRow - 1)) {
                    // write gob_resync_marker
                    mp4_EncodeZeroBitsAlign(slice->cBS);
                    EncodeGOBHeader(slice->cBS, gob_number, quant);
                    gob_number ++;
                    nmb = 0;
                }
            }
            if (mRateControl == 2)
                mRC_MB.Update(slice, &dquant, MP4_VOP_TYPE_P);
            nmbf ++;
            MBcurr ++;
            pYc += 16; pUc += 8; pVc += 8;
            pYf += 16; pUf += 8; pVf += 8;
        }
    }
    mp4_EncodeZeroBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodePVOPSH()
{
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if (mThreadingAlg == 0 && mRateControl != 2) {
            Ipp32s  i, nmb;

            // slices in this ME are used only for calculating numIntraMB
            ME_VOP();
            if (mNumIntraMB > mSceneChangeThreshold)
                return;
            VOP.gob_number = 1;
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            nmb = 0;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodePRowSH(curRow, mp4_MT_get_thread_num(), &nmb);
                    curRow ++;
                }
            }
            mp4_EncodeZeroBitsAlign(cBS);
        } else {
            Ipp32s  i, j, k;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = mSliceMT[i].numIntraMB = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma  omp parallel num_threads(mNumThreads)
            {
                ME_Slice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                ME_Slice(mSliceMT + i);
#endif
            mNumIntraMB = mSliceMT[0].numIntraMB;
            for (i = 1; i < mNumThreads; i ++)
                mNumIntraMB += mSliceMT[i].numIntraMB;;
            if (mNumIntraMB > mSceneChangeThreshold)
                return;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodePSliceSH(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodePSliceSH(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            cBS = mSliceMT[0].cBS;
            k = mSliceMT[0].cBS.GetFullness() << 3;
            for (i = 1; i < mNumThreads; i ++) {
                // update MB positions for RTP
                for (j = 0; j < mSliceMT[i].numRow * mNumMacroBlockPerRow; j ++)
                    mMBpos[mSliceMT->startRow * mNumMacroBlockPerRow + j] += k;
                k += mSliceMT[i].cBS.GetFullness() << 3;
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
            }
        }
    } else
#endif // _OMP_KARABAS
    {
        mSlice.numIntraMB = 0;
        ME_Slice(&mSlice);
        mNumIntraMB = mSlice.numIntraMB;
        if (mNumIntraMB > mSceneChangeThreshold)
            return;
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodePSliceSH(&mSlice);
        mQuantSum = mSlice.quantSum;
        cBS = mSlice.cBS;
    }
}

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodeIRow(Ipp32s curRow, Ipp32s threadNum)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    Ipp8u  *pY, *pU, *pV;
    Ipp32s   j, quant, ac_pred_flag, pattern, pattern1, use_intra_dc_vlc, dct_type;
    mp4_MacroBlock *MBcurr;
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    quant = VOP.vop_quant;
    dct_type = 0;
    use_intra_dc_vlc = 1; // in this version use_intra_dc_vlc is always 1
    pY = mFrameC->pY + curRow * 16 * mStepLuma;
    pU = mFrameC->pU + curRow * 8 * mStepChroma;
    pV = mFrameC->pV + curRow * 8 * mStepChroma;
    MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        MBcurr->not_coded = 0;
        MBcurr->mv[0].dx = MBcurr->mv[0].dy = MBcurr->mv[1].dx = MBcurr->mv[1].dy = MBcurr->mv[2].dx = MBcurr->mv[2].dy = MBcurr->mv[3].dx = MBcurr->mv[3].dy = 0;
        MBcurr->type = (Ipp8u)IPPVC_MBTYPE_INTRA;
        mp4_WaitIntra(curRow);
        pattern = TransMacroBlockIntra_MPEG4(pY, pU, pV, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, curRow, j, &dct_type, use_intra_dc_vlc, MBcurr, pMBinfoMT->predDir, 0, &ac_pred_flag, &pattern1, NULL);
        if (mIVOPdist != 1 || mReconstructAlways) {
            // restore VOP
            mp4_CopyCoeffsIntra(pMBinfoMT->dctCoeffs, coeffMB, pattern1);
            if (ac_pred_flag)
                mp4_RestoreIntraAC(MBcurr, coeffMB, pMBinfoMT->predDir);
            ReconMacroBlockIntra_MPEG4(pY, pU, pV, coeffMB, quant, MBcurr, pattern1, dct_type);
        }
        pMBinfoMT->pat = pattern;
        pMBinfoMT->ac_pred_flag = ac_pred_flag;
        pMBinfoMT->dct_type = dct_type;
        pMBinfoMT ++;
        pY += 16; pU += 8; pV += 8;
        MBcurr ++;
        mCurRowMT[curRow] ++;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    MBcurr -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        pattern = pMBinfoMT->pat;
        mp4_EncodeMCBPC_I(cBS, MBcurr->type, pattern & 3);
        cBS.PutBit(pMBinfoMT->ac_pred_flag);
        mp4_EncodeCBPY_I(cBS, pattern >> 2);
        if (VOL.interlaced)
            cBS.PutBit(pMBinfoMT->dct_type);
        mp4_EncodeMacroBlockIntra_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, pMBinfoMT->predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
        pMBinfoMT ++;
        MBcurr ++;
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodeISlice(mp4_Slice *slice)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    Ipp8u  *pY, *pU, *pV, *sPtr;
    Ipp32s  i, j, quant, pattern, startRow, mbn, mbnvp;
    Ipp32s  nzCount[6], predDir[6], ac_pred_flag, use_intra_dc_vlc, dct_type, pattern1, dquant, nBits, sBitOff;
    mp4_MacroBlock *MBcurr;
#ifdef VIDEOPACKETS_LE_MAX
    Ipp8u  *cPtr = NULL, *cPtr_1 = NULL, *cPtr_2 = NULL;
    Ipp32s  cBitOff = 0, cBitOff_1 = 0, cBitOff_2 = 0;
#endif
    bool vp_step_back = false;

    startRow = slice->startRow;
    mbn = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + mbn;
    mbnvp = dct_type = dquant = 0;
    quant = VOP.vop_quant;
    use_intra_dc_vlc = 1; // in this version use_intra_dc_vlc is always 1
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_I, quant);
    slice->cBS.GetPos(&sPtr, &sBitOff);
    if (startRow > 0)
        EncodeVideoPacketHeader(slice->cBS, mbn, quant);
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pY = mFrameC->pY + i * 16 * mStepLuma;
        pU = mFrameC->pU + i * 8 * mStepChroma;
        pV = mFrameC->pV + i * 8 * mStepChroma;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            MBcurr->type = (Ipp8u)(dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q);
            MBcurr->not_coded = 0;
            MBcurr->mv[0].dx = MBcurr->mv[0].dy = MBcurr->mv[1].dx = MBcurr->mv[1].dy = MBcurr->mv[2].dx = MBcurr->mv[2].dy = MBcurr->mv[3].dx = MBcurr->mv[3].dy = 0;
            pattern = TransMacroBlockIntra_MPEG4(pY, pU, pV, coeffMB, nzCount, quant, i, j, &dct_type, use_intra_dc_vlc, MBcurr, predDir, startRow, &ac_pred_flag, &pattern1, NULL);
            mp4_EncodeMCBPC_I(slice->cBS, MBcurr->type, pattern & 3);
            if (VOL.data_partitioned) {
                if (dquant != 0)
                    mp4_EncodeDquant(slice->cBS, dquant);
                if (use_intra_dc_vlc)
                    mp4_EncodeMacroBlockIntra_DC_MPEG4(slice->cBS, coeffMB);
                // encode to dp_buff_1
                slice->cBS_1.PutBit(ac_pred_flag);
                mp4_EncodeCBPY_I(slice->cBS_1, pattern >> 2);
                // encode to dp_buff_2
                mp4_EncodeMacroBlockIntra_AC_MPEG4(slice->cBS_2, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOL.reversible_vlc);
            } else {
                slice->cBS.PutBit(ac_pred_flag);
                mp4_EncodeCBPY_I(slice->cBS, pattern >> 2);
                if (dquant != 0)
                    mp4_EncodeDquant(slice->cBS, dquant);
                if (VOL.interlaced)
                    slice->cBS.PutBit(dct_type);
                mp4_EncodeMacroBlockIntra_MPEG4(slice->cBS, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
            }
            if (mIVOPdist != 1 || mReconstructAlways) {
                // restore VOP
                if (ac_pred_flag)
                    mp4_RestoreIntraAC(MBcurr, coeffMB, predDir);
                ReconMacroBlockIntra_MPEG4(pY, pU, pV, coeffMB, quant, MBcurr, pattern1, dct_type);
            }
            if (mInsertResync) {
                mbn ++;
                mbnvp ++;
                nBits = slice->cBS.GetNumBits(sPtr, sBitOff);
                if (VOL.data_partitioned)
                    nBits += slice->cBS_1.GetNumBits() + slice->cBS_2.GetNumBits();
#ifdef VIDEOPACKETS_LE_MAX
                if (nBits > mVideoPacketLength) {
                    if (mbnvp > 1) {
                        slice->cBS.SetPos(cPtr, cBitOff);
                        if (VOL.data_partitioned) {
                            slice->cBS_1.SetPos(cPtr_1, cBitOff_1);
                            slice->cBS_2.SetPos(cPtr_2, cBitOff_2);
                        }
                        dquant = 0;
                        pY -= 16; pU -= 8; pV -= 8;
                        j --;
                        if (j < 0) {
                            j = mNumMacroBlockPerRow - 1;
                            i --;
                        }
                        mbnvp --;
                        mbn --;
                        MBcurr --;
                        vp_step_back = true;
                    }
#else
                if ((nBits >= mVideoPacketLength) && mbn < mNumMacroBlockPerVOP) {
#endif
                    // mark MBs in current VideoPacket as invalid for prediction
                    MBcurr->block[1].validPredIntra = 0;
                    for (Ipp32s i = 0; i < IPP_MIN(mbnvp, mNumMacroBlockPerRow + 1); i ++) {
                        MBcurr[-i].block[2].validPredIntra = MBcurr[-i].block[3].validPredIntra = 0;
                        MBcurr[-i].block[4].validPredIntra = MBcurr[-i].block[5].validPredIntra = 0;
                    }
                    if (VOL.data_partitioned) {
                        mp4_EncodeMarkerDC(slice->cBS);
                        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
                    }
                    slice->cBS.GetPos(&sPtr, &sBitOff);
                    mp4_EncodeStuffingBitsAlign(slice->cBS);
                    EncodeVideoPacketHeader(slice->cBS, mbn, quant);
                    mbnvp = 0;
                }
#ifdef VIDEOPACKETS_LE_MAX
                else
                    vp_step_back = false;
                slice->cBS.GetPos(&cPtr, &cBitOff);
                if (VOL.data_partitioned) {
                    slice->cBS_1.GetPos(&cPtr_1, &cBitOff_1);
                    slice->cBS_2.GetPos(&cPtr_2, &cBitOff_2);
                }
#endif
            }
            if (!vp_step_back) {
                slice->quantSum += quant;
                if (mRateControl == 2) {
                    mRC_MB.Update(slice, &dquant, MP4_VOP_TYPE_I);
                    quant += dquant;
                }
            }
            pY += 16; pU += 8; pV += 8;
            MBcurr ++;
        }
    }
    if (VOL.data_partitioned) {
        mp4_EncodeMarkerDC(slice->cBS);
        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
    }
    mp4_EncodeStuffingBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodeIVOP()
{
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if (mThreadingAlg == 0 && mRateControl != 2 && !VOL.data_partitioned && !mInsertResync) {
            Ipp32s  i;

            for (i = 0; i < mNumMacroBlockPerCol; i ++)
                mCurRowMT[i] = -1;
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodeIRow(curRow, mp4_MT_get_thread_num());
                    curRow ++;
                }
            }
            mp4_EncodeStuffingBitsAlign(cBS);
        } else {
            Ipp32s  i;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodeISlice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodeISlice(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            cBS = mSliceMT[0].cBS;
            for (i = 1; i < mNumThreads; i ++) {
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
            }
        }
    } else
#endif // _OMP_KARABAS
    {
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodeISlice(&mSlice);
        mQuantSum = mSlice.quantSum;
        cBS = mSlice.cBS;
    }
}

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodePRow(Ipp32s curRow, Ipp32s threadNum, Ipp32s *numNotCodedMB)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    mp4_MacroBlock *MBcurr;
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s  j, quant, pattern, ac_pred_flag, use_intra_dc_vlc, dct_type, rt, pattern1;
    IppMotionVector mvLuma4[4], mvChroma, mvChromaT, mvChromaB/*, mvPred[4]*/;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
    //Ipp32s  fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    use_intra_dc_vlc = 1; //quant < mDC_VLC_Threshold[VOP.intra_dc_vlc_thr];
    MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
    rt = VOP.vop_rounding_type;
    dct_type = 0;
    quant = VOP.vop_quant;
    use_intra_dc_vlc = 1; //quant < mDC_VLC_Threshold[VOP.intra_dc_vlc_thr];
    pYc = mFrameC->pY + curRow * 16 * mStepLuma;
    pUc = mFrameC->pU + curRow * 8 * mStepChroma;
    pVc = mFrameC->pV + curRow * 8 * mStepChroma;
    pYf = mFrameF->pY + curRow * 16 * mStepLuma;
    pUf = mFrameF->pU + curRow * 8 * mStepChroma;
    pVf = mFrameF->pV + curRow * 8 * mStepChroma;
    MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        if (MBcurr->not_coded) {
            MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
            ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
            ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
            ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
        } else {
            if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                // intra coded
                mp4_WaitIntra(curRow);
                pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, curRow, j, &dct_type, use_intra_dc_vlc, MBcurr, pMBinfoMT->predDir, 0, &ac_pred_flag, &pattern1, NULL);
                mp4_CopyCoeffsIntra(pMBinfoMT->dctCoeffs, coeffMB, pattern1);
                if (ac_pred_flag)
                    mp4_RestoreIntraAC(MBcurr, coeffMB, pMBinfoMT->predDir);
                ReconMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, quant, MBcurr, pattern1, dct_type);
                pMBinfoMT->ac_pred_flag = ac_pred_flag;
            } else {
                // inter coded
                MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
                if (!MBcurr->fieldmc) {
                    mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                    // find Luma Pred
                    if (VOL.obmc_disable) {
                        if (VOL.quarter_sample) {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                            } else
                                mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                        } else {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                            } else
                                mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                        }
                    } else {
                        IppMotionVector *mvRight, *mvLeft, *mvUpper;
                        mvRight = (j == mNumMacroBlockPerRow - 1) ? &mvLuma4[1] : (MBcurr[1].type == IPPVC_MBTYPE_INTRA || MBcurr[1].type == IPPVC_MBTYPE_INTRA_Q) ? &mvLuma4[1] : MBcurr[1].mv;
                        mvLeft = (j == 0) ? mvLuma4 - 1 : (MBcurr[-1].type == IPPVC_MBTYPE_INTRA || MBcurr[-1].type == IPPVC_MBTYPE_INTRA_Q) ? mvLeft = mvLuma4 - 1 : MBcurr[-1].mv;
                        mvUpper = (curRow == 0) ? mvLuma4 - 2 : (MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA || MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA_Q) ? mvLuma4 - 2 : MBcurr[-mNumMacroBlockPerRow].mv;
                        if (VOL.quarter_sample) {
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                        } else {
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                        }
                    }
                    // calculate Chroma MV
                    if (VOL.quarter_sample) {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                            mp4_ComputeChroma4MVQ(mvLuma4, &mvChroma);
                        else
                            mp4_ComputeChromaMVQ(mvLuma4, &mvChroma);
                    } else {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                            mp4_ComputeChroma4MV(mvLuma4, &mvChroma);
                        else
                            mp4_ComputeChromaMV(mvLuma4, &mvChroma);
                    }
                    // find Chroma Pred
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                } else {
                    mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                    // find Luma Pred
                    if (VOL.quarter_sample) {
                        mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                        mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                    } else {
                        mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                        mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                    }
                    // calculate Chroma MV
                    if (VOL.quarter_sample) {
                        IppMotionVector mvTmpT, mvTmpB;
                        mvTmpT.dx = (Ipp16s)mp4_Div2(MBcurr->mvT.dx);
                        mvTmpT.dy = (Ipp16s)(mp4_Div2(MBcurr->mvT.dy << 1) >> 1);
                        mvTmpB.dx = (Ipp16s)mp4_Div2(MBcurr->mvB.dx);
                        mvTmpB.dy = (Ipp16s)(mp4_Div2(MBcurr->mvB.dy << 1) >> 1);
                        mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                        mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                    } else {
                        mp4_ComputeChromaMV(&MBcurr->mvT, &mvChromaT);
                        mp4_ComputeChromaMV(&MBcurr->mvB, &mvChromaB);
                    }
                    // find Chroma Pred
                    mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4, 16, &mvChromaT, rt);
                    mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5, 16, &mvChromaT, rt);
                    mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                    mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                }
                pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, mcPred, curRow, j, &dct_type, mMEflags & RD_TRELLIS, NULL);
                if (mBVOPdist == 0 && pattern == 0 && mvLuma4[0].dx == 0 && mvLuma4[0].dy == 0 && mvLuma4[1].dx == 0 && mvLuma4[1].dy == 0 && mvLuma4[2].dx == 0 && mvLuma4[2].dy == 0 && mvLuma4[3].dx == 0 && mvLuma4[3].dy == 0) {
                    MBcurr->not_coded = 1;
                    ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                    (*numNotCodedMB) ++;
                } else {
                    if (pattern == 0)
                        ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                    else {
                        mp4_CopyCoeffsInter(pMBinfoMT->dctCoeffs, coeffMB, pattern);
                        ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern, dct_type);
                    }
                }
            }
            pMBinfoMT->pat = pattern;
            pMBinfoMT->dct_type = dct_type;
        }
        pMBinfoMT ++;
        pYc += 16; pUc += 8; pVc += 8;
        pYf += 16; pUf += 8; pVf += 8;
        MBcurr ++;
        mCurRowMT[curRow] ++;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    MBcurr -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        if (MBcurr->not_coded) {
            cBS.PutBit(1);
        } else {
            cBS.PutBit(0);
            pattern = pMBinfoMT->pat;
            if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                mp4_EncodeMCBPC_P(cBS, IPPVC_MBTYPE_INTRA, pattern & 3);
                // encode ac_pred_flag
                cBS.PutBit(pMBinfoMT->ac_pred_flag);
                mp4_EncodeCBPY_P(cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                if (VOL.interlaced)
                    cBS.PutBit(pMBinfoMT->dct_type);
                mp4_EncodeMacroBlockIntra_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, pMBinfoMT->predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
            } else {
                if (!MBcurr->fieldmc) {
                    mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                } else {
                    mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                }
/*
                Predict1MV(MBcurr, curRow, j, &mvPred[0]);
                if (!MBcurr->fieldmc) {
                    mp4_MV_GetDiff(&mvLuma4[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
                    if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                        Predict3MV(MBcurr, curRow, j, mvPred, mvLuma4);
                        mp4_MV_GetDiff(&mvLuma4[1], &mvPred[1], fRangeMin, fRangeMax, fRange);
                        mp4_MV_GetDiff(&mvLuma4[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
                        mp4_MV_GetDiff(&mvLuma4[3], &mvPred[3], fRangeMin, fRangeMax, fRange);
                    }
                } else {
                    mvPred[0].dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
                    mvPred[2] = mvPred[0];
                    mp4_MV_GetDiff(&mvLuma4[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
                    mp4_MV_GetDiff(&mvLuma4[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
                }
*/
                mp4_EncodeMCBPC_P(cBS, MBcurr->type, pattern & 3);
                mp4_EncodeCBPY_P(cBS, MBcurr->type, pattern >> 2);
                if (VOL.interlaced) {
                    // encode dct_type
                    if (pattern)
                        cBS.PutBit(pMBinfoMT->dct_type);
                    // encode field_prediction
                    if (MBcurr->type != IPPVC_MBTYPE_INTER4V) {
                        cBS.PutBit(MBcurr->fieldmc);
                        if (MBcurr->fieldmc) {
                            cBS.PutBit(mb_ftfr);
                            cBS.PutBit(mb_fbfr);
                        }
                    }
                    mp4_EncodeMV(cBS, &MBcurr->mvDiff[0]/*&mvPred[0]*/, VOP.vop_fcode_forward, MBcurr->type);
                    if (MBcurr->fieldmc)
                        mp4_EncodeMV(cBS, &MBcurr->mvDiff[2]/*&mvPred[2]*/, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                } else
                    mp4_EncodeMV(cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                mp4_EncodeMacroBlockInter_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
            }
        }
        pMBinfoMT ++;
        MBcurr ++;
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodePSlice(mp4_Slice *slice)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    mp4_MacroBlock *MBcurr;
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf, *sPtr;
    Ipp32s  i, j, quant, pattern, nzCount[6], dquant, startRow, mbn, mbnvp;
    IppMotionVector mvLuma4[4], mvChroma, /*mvPred[4], */mvChromaT, mvChromaB;
    int     fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
    Ipp32s  predDir[6], ac_pred_flag, use_intra_dc_vlc, dct_type, rt, pattern1, nBits, sBitOff;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
#ifdef VIDEOPACKETS_LE_MAX
    Ipp8u  *cPtr = NULL, *cPtr_1 = NULL, *cPtr_2 = NULL;
    Ipp32s  cBitOff = 0, cBitOff_1 = 0, cBitOff_2 = 0;
#endif
    bool vp_step_back = false;

    startRow = slice->startRow;
    mbn = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + mbn;
    mbnvp = dct_type = dquant = 0;
    quant = VOP.vop_quant;
    use_intra_dc_vlc = 1; // in this version use_intra_dc_vlc is always 1
    rt = VOP.vop_rounding_type;
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_P, quant);
    slice->cBS.GetPos(&sPtr, &sBitOff);
    if (startRow > 0)
        EncodeVideoPacketHeader(slice->cBS, mbn, quant);
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pYc = mFrameC->pY + i * 16 * mStepLuma;
        pUc = mFrameC->pU + i * 8 * mStepChroma;
        pVc = mFrameC->pV + i * 8 * mStepChroma;
        pYf = mFrameF->pY + i * 16 * mStepLuma;
        pUf = mFrameF->pU + i * 8 * mStepChroma;
        pVf = mFrameF->pV + i * 8 * mStepChroma;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            if (dquant != 0 || quant < VOP.vop_quant) {
                // disable not_coded decision if quant should be changed between MB
                if (MBcurr->not_coded) {
                    MBcurr->not_coded = 0;
                    slice->numNotCodedMB --;
                    Predict1MV(MBcurr, i - startRow, j, &MBcurr->mvDiff[0]);
                    //int     fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
                    mp4_MV_GetDiff(&MBcurr->mv[0], &MBcurr->mvDiff[0], fRangeMin, fRangeMax, fRange);
                }
            }
            if (MBcurr->not_coded) {
                MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
                // encode not_coded
                slice->cBS.PutBit(1);
                ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
                ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
                ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
            } else {
                if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                    // intra coded
                    pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, i, j, &dct_type, use_intra_dc_vlc, MBcurr, predDir, startRow, &ac_pred_flag, &pattern1, NULL);
                    // encode not_coded
                    slice->cBS.PutBit(0);
                    mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q, pattern & 3);
                    if (VOL.data_partitioned) {
                        // encode to dp_buff_1
                        slice->cBS_1.PutBit(ac_pred_flag);
                        mp4_EncodeCBPY_P(slice->cBS_1, IPPVC_MBTYPE_INTRA, pattern >> 2);
                        if (dquant != 0)
                            mp4_EncodeDquant(slice->cBS_1, dquant);
                        if (use_intra_dc_vlc)
                            mp4_EncodeMacroBlockIntra_DC_MPEG4(slice->cBS_1, coeffMB);
                        // encode to dp_buff_2
                        mp4_EncodeMacroBlockIntra_AC_MPEG4(slice->cBS_2, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOL.reversible_vlc);
                    } else {
                        slice->cBS.PutBit(ac_pred_flag);
                        mp4_EncodeCBPY_P(slice->cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                        if (dquant != 0)
                            mp4_EncodeDquant(slice->cBS, dquant);
                        if (VOL.interlaced)
                            slice->cBS.PutBit(dct_type);
                        mp4_EncodeMacroBlockIntra_MPEG4(slice->cBS, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
                    }
                    if (ac_pred_flag)
                        mp4_RestoreIntraAC(MBcurr, coeffMB, predDir);
                    ReconMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, quant, MBcurr, pattern1, dct_type);
                } else {
                    // inter coded
                    MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
                    if (!MBcurr->fieldmc) {
                        mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                        // find Luma Pred
                        if (VOL.obmc_disable) {
                            if (VOL.quarter_sample) {
                                if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                    mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                    mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                    mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                    mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                                } else
                                    mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                            } else {
                                if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                    mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                    mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                    mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                    mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                                } else
                                    mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                            }
                        } else {
                            IppMotionVector *mvRight, *mvLeft, *mvUpper;
                            mvRight = ((j == mNumMacroBlockPerRow - 1) ? &mvLuma4[1] : ((MBcurr[1].type == IPPVC_MBTYPE_INTRA || MBcurr[1].type == IPPVC_MBTYPE_INTRA_Q) ? &mvLuma4[1] : MBcurr[1].mv));
                            mvLeft = ((j == 0) ? (mvLuma4 - 1) : ((MBcurr[-1].type == IPPVC_MBTYPE_INTRA || MBcurr[-1].type == IPPVC_MBTYPE_INTRA_Q) ? (mvLeft = mvLuma4 - 1) : MBcurr[-1].mv));
                            mvUpper = ((i == 0) ? (mvLuma4 - 2) : ((MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA || MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA_Q) ? (mvLuma4 - 2) : MBcurr[-mNumMacroBlockPerRow].mv));
                            if (VOL.quarter_sample) {
                                ippiOBMC8x8QP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                                ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                                ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                                ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                            } else {
                                ippiOBMC8x8HP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                                ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                                ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                                ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                            }
                        }
                        // calculate Chroma MV
                        if (VOL.quarter_sample) {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                                mp4_ComputeChroma4MVQ(mvLuma4, &mvChroma);
                            else
                                mp4_ComputeChromaMVQ(mvLuma4, &mvChroma);
                        } else {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                                mp4_ComputeChroma4MV(mvLuma4, &mvChroma);
                            else
                                mp4_ComputeChromaMV(mvLuma4, &mvChroma);
                        }
                        // find Chroma Pred
                        mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                        mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                    } else {
                        mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                        // find Luma Pred
                        if (VOL.quarter_sample) {
                            mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                            mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                        } else {
                            mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                            mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                        }
                        // calculate Chroma MV
                        if (VOL.quarter_sample) {
                            IppMotionVector mvTmpT, mvTmpB;
                            mvTmpT.dx = (Ipp16s)mp4_Div2(MBcurr->mvT.dx);
                            mvTmpT.dy = (Ipp16s)(mp4_Div2(MBcurr->mvT.dy << 1) >> 1);
                            mvTmpB.dx = (Ipp16s)mp4_Div2(MBcurr->mvB.dx);
                            mvTmpB.dy = (Ipp16s)(mp4_Div2(MBcurr->mvB.dy << 1) >> 1);
                            mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                            mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                        } else {
                            mp4_ComputeChromaMV(&MBcurr->mvT, &mvChromaT);
                            mp4_ComputeChromaMV(&MBcurr->mvB, &mvChromaB);
                        }
                        // find Chroma Pred
                        mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4, 16, &mvChromaT, rt);
                        mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5, 16, &mvChromaT, rt);
                        mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                        mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                    }
                    pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, i, j, &dct_type, mMEflags & RD_TRELLIS, NULL);
                    if (mBVOPdist == 0 && pattern == 0 && mvLuma4[0].dx == 0 && mvLuma4[0].dy == 0 && mvLuma4[1].dx == 0 && mvLuma4[1].dy == 0 && mvLuma4[2].dx == 0 && mvLuma4[2].dy == 0 && mvLuma4[3].dx == 0 && mvLuma4[3].dy == 0 && dquant == 0) {
                        MBcurr->not_coded = 1;
                        // encode not_coded
                        slice->cBS.PutBit(1);
                        ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                        slice->numNotCodedMB ++;
                    } else {
                        if (mInsertResync) {
                            // recompute MV differences which were computed in ME
                            Predict1MV(MBcurr, i - startRow, j, &MBcurr->mvDiff[0]);
                            if (!MBcurr->fieldmc) {
                                mp4_MV_GetDiff(&mvLuma4[0], &MBcurr->mvDiff[0], fRangeMin, fRangeMax, fRange);
                                if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                    Predict3MV(MBcurr, i - startRow, j, MBcurr->mvDiff, mvLuma4);
                                    mp4_MV_GetDiff(&mvLuma4[1], &MBcurr->mvDiff[1], fRangeMin, fRangeMax, fRange);
                                    mp4_MV_GetDiff(&mvLuma4[2], &MBcurr->mvDiff[2], fRangeMin, fRangeMax, fRange);
                                    mp4_MV_GetDiff(&mvLuma4[3], &MBcurr->mvDiff[3], fRangeMin, fRangeMax, fRange);
                                }
                            } else {
                                MBcurr->mvDiff[0].dy = (Ipp16s)mp4_Div2(MBcurr->mvDiff[0].dy);
                                MBcurr->mvDiff[2] = MBcurr->mvDiff[0];
                                mp4_MV_GetDiff(&mvLuma4[0], &MBcurr->mvDiff[0], fRangeMin, fRangeMax, fRange);
                                mp4_MV_GetDiff(&mvLuma4[2], &MBcurr->mvDiff[2], fRangeMin, fRangeMax, fRange);
                            }
                        }
                        // encode not_coded
                        slice->cBS.PutBit(0);
                        mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? MBcurr->type : IPPVC_MBTYPE_INTER_Q, pattern & 3);
                        if (VOL.data_partitioned) {
                            mp4_EncodeMV(slice->cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                            // encode to dp_buff_1
                            mp4_EncodeCBPY_P(slice->cBS_1, MBcurr->type, pattern >> 2);
                            if (dquant != 0)
                                mp4_EncodeDquant(slice->cBS_1, dquant);
                            // encode to dp_buff_2
                            mp4_EncodeMacroBlockInter_MPEG4(slice->cBS_2, coeffMB, pattern, nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
                        } else {
                            mp4_EncodeCBPY_P(slice->cBS, dquant == 0 ? MBcurr->type : IPPVC_MBTYPE_INTER_Q, pattern >> 2);
                            if (dquant != 0)
                                mp4_EncodeDquant(slice->cBS, dquant);
                            if (VOL.interlaced) {
                                // encode dct_type
                                if (pattern)
                                    slice->cBS.PutBit(dct_type);
                                else
                                    dct_type = 0;
                                // encode field_prediction
                                if (MBcurr->type != IPPVC_MBTYPE_INTER4V) {
                                    slice->cBS.PutBit(MBcurr->fieldmc);
                                    if (MBcurr->fieldmc) {
                                        slice->cBS.PutBit(mb_ftfr);
                                        slice->cBS.PutBit(mb_fbfr);
                                    }
                                }
                                mp4_EncodeMV(slice->cBS, &MBcurr->mvDiff[0]/*&mvPred[0]*/, VOP.vop_fcode_forward, MBcurr->type);
                                if (MBcurr->fieldmc)
                                    mp4_EncodeMV(slice->cBS, &MBcurr->mvDiff[2]/*&mvPred[2]*/, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                            } else
                                mp4_EncodeMV(slice->cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                            mp4_EncodeMacroBlockInter_MPEG4(slice->cBS, coeffMB, pattern, nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
                        }
                        if (pattern == 0)
                            ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                        else
                            ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern, dct_type);
                    }
                }
            }
            if (mInsertResync) {
                mbn ++;
                mbnvp ++;
                nBits = slice->cBS.GetNumBits(sPtr, sBitOff);
                if (VOL.data_partitioned)
                    nBits += slice->cBS_1.GetNumBits() + slice->cBS_2.GetNumBits();
#ifdef VIDEOPACKETS_LE_MAX
                if (nBits > mVideoPacketLength) {
                    if (mbnvp > 1) {
                        slice->cBS.SetPos(cPtr, cBitOff);
                        if (VOL.data_partitioned) {
                            slice->cBS_1.SetPos(cPtr_1, cBitOff_1);
                            slice->cBS_2.SetPos(cPtr_2, cBitOff_2);
                        }
                        dquant = 0;
                        pYc -= 16; pUc -= 8; pVc -= 8;
                        pYf -= 16; pUf -= 8; pVf -= 8;
                        j --;
                        if (j < 0) {
                            j = mNumMacroBlockPerRow - 1;
                            i --;
                        }
                        mbnvp --;
                        mbn --;
                        MBcurr --;
                        vp_step_back = true;
                    }
#else
                if ((nBits >= mVideoPacketLength) && mbn < mNumMacroBlockPerVOP) {
#endif
                    // mark MBs in current VideoPacket as invalid for prediction
                    MBcurr->block[1].validPredIntra = 0;
                    for (Ipp32s i = 0; i < IPP_MIN(mbnvp, mNumMacroBlockPerRow + 1); i ++) {
                        MBcurr[-i].block[2].validPredIntra = MBcurr[-i].block[3].validPredIntra = 0;
                        MBcurr[-i].block[4].validPredIntra = MBcurr[-i].block[5].validPredIntra = 0;
                        MBcurr[-i].validPredInter = 0;
                    }
                    if (VOL.data_partitioned) {
                        mp4_EncodeMarkerMV(slice->cBS);
                        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
                    }
                    slice->cBS.GetPos(&sPtr, &sBitOff);
                    mp4_EncodeStuffingBitsAlign(slice->cBS);
                    EncodeVideoPacketHeader(slice->cBS, mbn, quant);
                    mbnvp = 0;
                }
#ifdef VIDEOPACKETS_LE_MAX
                else
                    vp_step_back = false;
                slice->cBS.GetPos(&cPtr, &cBitOff);
                if (VOL.data_partitioned) {
                    slice->cBS_1.GetPos(&cPtr_1, &cBitOff_1);
                    slice->cBS_2.GetPos(&cPtr_2, &cBitOff_2);
                }
#endif
            }
            if (!vp_step_back) {
                slice->quantSum += quant;
                if (mRateControl == 2) {
                    mRC_MB.Update(slice, &dquant, MP4_VOP_TYPE_P);
                    quant += dquant;
                }
            }
            pYc += 16; pUc += 8; pVc += 8;
            pYf += 16; pUf += 8; pVf += 8;
            MBcurr ++;
        }
    }
    if (VOL.data_partitioned) {
        mp4_EncodeMarkerMV(slice->cBS);
        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
    }
    mp4_EncodeStuffingBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodePVOP()
{
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if ((mRateControl != 2 && !VOL.data_partitioned && !mInsertResync) && (mThreadingAlg == 0 || (mThreadingAlg == 1 && !VOL.obmc_disable)) ) {
            Ipp32s  i;

            // slices in this ME are used only for calculating numIntraMB and numNotCodedMB
            ME_VOP();
            if ((mNumIntraMB > mSceneChangeThreshold) || (mBVOPdist == 0 && mNumNotCodedMB >= mNumMacroBlockPerVOP))
                return;
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            for (i = 0; i < mNumMacroBlockPerCol; i ++)
                mCurRowMT[i] = -1;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                Ipp32s tn = mp4_MT_get_thread_num();
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodePRow(curRow, tn, &mSliceMT[tn].numNotCodedMB);
                    curRow ++;
                }
            }
            mNumNotCodedMB = mSliceMT[0].numNotCodedMB;
            for (i = 1; i < mNumThreads; i ++)
                mNumNotCodedMB += mSliceMT[i].numNotCodedMB;
            mp4_EncodeStuffingBitsAlign(cBS);
            AdjustSearchRange();
            return;
        }
        if (mThreadingAlg == 1 && VOL.obmc_disable) {
            Ipp32s  i;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = mSliceMT[i].numIntraMB = mSliceMT[i].numNotCodedMB = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma  omp parallel num_threads(mNumThreads)
            {
                ME_Slice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                ME_Slice(mSliceMT + i);
#endif
            mNumIntraMB = mSliceMT[0].numIntraMB;
            mNumNotCodedMB = mSliceMT[0].numNotCodedMB;
            for (i = 1; i < mNumThreads; i ++) {
                mNumIntraMB += mSliceMT[i].numIntraMB;;
                mNumNotCodedMB += mSliceMT[i].numNotCodedMB;
            }
            if ((mNumIntraMB > mSceneChangeThreshold) || (mBVOPdist == 0 && mNumNotCodedMB >= mNumMacroBlockPerVOP))
                return;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodePSlice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodePSlice(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            mNumNotCodedMB = mSliceMT[0].numNotCodedMB;
            cBS = mSliceMT[0].cBS;
            for (i = 1; i < mNumThreads; i ++) {
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
                mNumNotCodedMB += mSliceMT[i].numNotCodedMB;
            }
            AdjustSearchRange();
            return;
        }
    }
#endif // _OMP_KARABAS
    {
        mSlice.numIntraMB = mSlice.numNotCodedMB = 0;
        ME_Slice(&mSlice);
        mNumIntraMB = mSlice.numIntraMB;
        mNumNotCodedMB = mSlice.numNotCodedMB;
        if ((mNumIntraMB > mSceneChangeThreshold) || (mBVOPdist == 0 && mNumNotCodedMB >= mNumMacroBlockPerVOP))
            return;
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodePSlice(&mSlice);
        mQuantSum = mSlice.quantSum;
        mNumNotCodedMB = mSlice.numNotCodedMB;
        cBS = mSlice.cBS;
    }
    AdjustSearchRange();
}

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodeBRow(Ipp32s curRow, Ipp32s threadNum)
{
    // only frame MC implemented for FORWARD, BACKWARD and INTERPOLATE
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPredD, 64*6);
    __ALIGN16(Ipp8u, mcPredI, 64*6);
    __ALIGN16(Ipp8u, mcTmp, 64*6);
    Ipp8u          *pYc, *pUc, *pVc, *pYf, *pUf, *pVf, *pYb, *pUb, *pVb, *mcPred = 0;
    Ipp32s          j, mb_type, quant, pattern, modb, qpel;
    Ipp32s          bestSADDirect, bestSAD16x16Inter, bestSAD16x16Forw, bestSAD16x16Back, bestSAD;
    IppMotionVector mvLumaForw, mvPredForw, mvForwDiff, mvLumaBack, mvPredBack, mvBackDiff, mvChromaF, mvChromaB, mvLumaForwDirect[4], mvLumaBackDirect[4], mvDelta;
    mp4_MacroBlock *MBcurr;
    Ipp32s          fRangeMinForw = -(16 << VOP.vop_fcode_forward), fRangeMaxForw = (16 << VOP.vop_fcode_forward) - 1, fRangeForw = fRangeMaxForw - fRangeMinForw + 1;
    Ipp32s          fRangeMinBack = -(16 << VOP.vop_fcode_backward), fRangeMaxBack = (16 << VOP.vop_fcode_backward) - 1, fRangeBack = fRangeMaxBack - fRangeMinBack + 1;
    mp4_Data_ME     meData;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    meData.method = mMEmethod;
    meData.flags = mMEflags & (~ME_CHROMA);
    meData.quant = VOP.vop_quant;
    meData.rt = 0;
#ifdef USE_ME_SADBUFF
    meData.meBuff = mMEfastSAD + mMEfastSADsize * threadNum;
#endif
    meData.stepL = mStepLuma;
    meData.stepC = mStepChroma;
    if (meData.flags & ME_ZERO_MV) {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = SAD_MAX;
    } else if (meData.flags & ME_USE_THRESHOLD) {
        meData.thrDiff16x16 = VOP.vop_quant >= 6 ? 256 : (4 << VOP.vop_quant);
        meData.thrDiff8x8 = meData.thrDiff16x16 >> 2;
        meData.thrDiff16x8 = meData.thrDiff16x16 >> 1;
    } else {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = 0;
    }
    meData.thrDiff = meData.thrDiff16x16;
    meData.sadFunc = ippiSAD16x16_8u32s;
    meData.copyQPFunc = ippiCopy16x16QP_MPEG4_8u_C1R;
    meData.copyHPFunc = ippiCopy16x16HP_8u_C1R;
    meData.numPred = 2;
    quant = VOP.vop_quant;
    qpel = VOL.quarter_sample;
    pYc = mFrameC->pY + curRow * 16 * mStepLuma;
    pUc = mFrameC->pU + curRow * 8 * mStepChroma;
    pVc = mFrameC->pV + curRow * 8 * mStepChroma;
    pYf = mFrameF->pY + curRow * 16 * mStepLuma;
    pUf = mFrameF->pU + curRow * 8 * mStepChroma;
    pVf = mFrameF->pV + curRow * 8 * mStepChroma;
    pYb = mFrameB->pY + curRow * 16 * mStepLuma;
    pUb = mFrameB->pU + curRow * 8 * mStepChroma;
    pVb = mFrameB->pV + curRow * 8 * mStepChroma;
    MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
    mvPredForw.dx = mvPredForw.dy = mvPredBack.dx = mvPredBack.dy = 0;
    mvForwDiff.dx = mvForwDiff.dy = mvBackDiff.dx = mvBackDiff.dy = 0;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        mb_type = -1;
        if (!MBcurr->not_coded) {
            // avoid warning
            mvLumaForw.dx = mvLumaForw.dy = mvLumaBack.dx = mvLumaBack.dy = 0;
            // Direct mode with MVd = 0
            if (!MBcurr->fieldmc) {
                if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                    for (Ipp32s nB = 0; nB < 4; nB ++) {
                        mvLumaForwDirect[nB].dx = (Ipp16s)(mTRB * MBcurr->mv[nB].dx / mTRD);
                        mvLumaForwDirect[nB].dy = (Ipp16s)(mTRB * MBcurr->mv[nB].dy / mTRD);
                        mvLumaBackDirect[nB].dx = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[nB].dx / mTRD);
                        mvLumaBackDirect[nB].dy = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[nB].dy / mTRD);
                    }
                    if (VOL.quarter_sample) {
                        mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                        mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPredD+8, 16, &mvLumaForwDirect[1], 0);
                        mp4_Copy8x8QP_8u(pYf+8*mStepLuma, mStepLuma, mcPredD+8*16, 16, &mvLumaForwDirect[2], 0);
                        mp4_Copy8x8QP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPredD+8*16+8, 16, &mvLumaForwDirect[3], 0);
                        mp4_ComputeChroma4MVQ(mvLumaBackDirect, &mvChromaB);
                        Limit4MVQ(mvLumaBackDirect, j << 4, curRow << 4);
                        mp4_Copy8x8QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                        mp4_Copy8x8QP_8u(pYb+8, mStepLuma, mcTmp+8, 16, &mvLumaBackDirect[1], 0);
                        mp4_Copy8x8QP_8u(pYb+8*mStepLuma, mStepLuma, mcTmp+8*16, 16, &mvLumaBackDirect[2], 0);
                        mp4_Copy8x8QP_8u(pYb+8*mStepLuma+8, mStepLuma, mcTmp+8*16+8, 16, &mvLumaBackDirect[3], 0);
                    } else {
                        mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                        mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPredD+8, 16, &mvLumaForwDirect[1], 0);
                        mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPredD+8*16, 16, &mvLumaForwDirect[2], 0);
                        mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPredD+8*16+8, 16, &mvLumaForwDirect[3], 0);
                        mp4_ComputeChroma4MV(mvLumaBackDirect, &mvChromaB);
                        Limit4MV(mvLumaBackDirect, j << 4, curRow << 4);
                        mp4_Copy8x8HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                        mp4_Copy8x8HP_8u(pYb+8, mStepLuma, mcTmp+8, 16, &mvLumaBackDirect[1], 0);
                        mp4_Copy8x8HP_8u(pYb+8*mStepLuma, mStepLuma, mcTmp+8*16, 16, &mvLumaBackDirect[2], 0);
                        mp4_Copy8x8HP_8u(pYb+8*mStepLuma+8, mStepLuma, mcTmp+8*16+8, 16, &mvLumaBackDirect[3], 0);
                    }
                    ippiAverage8x8_8u_C1IR(mcTmp, 16, mcPredD, 16);
                    ippiAverage8x8_8u_C1IR(mcTmp+8, 16, mcPredD+8, 16);
                    ippiAverage8x8_8u_C1IR(mcTmp+8*16, 16, mcPredD+8*16, 16);
                    ippiAverage8x8_8u_C1IR(mcTmp+8*16+8, 16, mcPredD+8*16+8, 16);
                } else {
                    mvLumaForwDirect[0].dx = (Ipp16s)(mTRB * MBcurr->mv[0].dx / mTRD);
                    mvLumaForwDirect[0].dy = (Ipp16s)(mTRB * MBcurr->mv[0].dy / mTRD);
                    mvLumaBackDirect[0].dx = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[0].dx / mTRD);
                    mvLumaBackDirect[0].dy = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[0].dy / mTRD);
                    if (VOL.quarter_sample) {
                        mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                        LimitMVQ(mvLumaBackDirect, j << 4, curRow << 4);
                        mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                    } else {
                        mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                        LimitMV(mvLumaBackDirect, j << 4, curRow << 4);
                        mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                    }
                    ippiAverage16x16_8u_C1IR(mcTmp, 16, mcPredD, 16);
                }
            } else {
                Ipp32s  TRDt, TRDb, TRBt, TRBb, deltaTop, deltaBottom;

                deltaTop = mb_ftfr;
                deltaBottom = mb_fbfr - 1;
                if (VOP.top_field_first) {
                    deltaTop = -deltaTop;
                    deltaBottom = -deltaBottom;
                }
                TRDt = mp4_DivRoundInf(mTRD, mTframe) * 2 + deltaTop;
                TRDb = mp4_DivRoundInf(mTRD, mTframe) * 2 + deltaBottom;
                TRBt = mp4_DivRoundInf(mTRB, mTframe) * 2 + deltaTop;
                TRBb = mp4_DivRoundInf(mTRB, mTframe) * 2 + deltaBottom;
                mvLumaForwDirect[0].dx = (Ipp16s)((TRBt * MBcurr->mvT.dx) / TRDt);
                mvLumaBackDirect[0].dx = (Ipp16s)(((TRBt - TRDt) * MBcurr->mvT.dx) / TRDt);
                mvLumaForwDirect[0].dy = (Ipp16s)((TRBt * MBcurr->mvT.dy * 2) / TRDt);
                mvLumaBackDirect[0].dy = (Ipp16s)(((TRBt - TRDt) * MBcurr->mvT.dy * 2) / TRDt);
                mvLumaForwDirect[2].dx = (Ipp16s)((TRBb * MBcurr->mvB.dx) / TRDb);
                mvLumaBackDirect[2].dx = (Ipp16s)(((TRBb - TRDb) * MBcurr->mvB.dx) / TRDb);
                mvLumaForwDirect[2].dy = (Ipp16s)((TRBb * MBcurr->mvB.dy * 2) / TRDb);
                mvLumaBackDirect[2].dy = (Ipp16s)(((TRBb - TRDb) * MBcurr->mvB.dy * 2) / TRDb);
                mvLumaForwDirect[0].dy >>= 1;
                mvLumaBackDirect[0].dy >>= 1;
                mvLumaForwDirect[2].dy >>= 1;
                mvLumaBackDirect[2].dy >>= 1;
                if (VOL.quarter_sample) {
                    mp4_Copy16x8QP_8u(pYf+(mb_ftfr ? mStepLuma : 0), mStepLuma * 2, mcPredD, 32, &mvLumaForwDirect[0], 0);
                    mp4_Copy16x8QP_8u(pYf+(mb_fbfr ? mStepLuma : 0), mStepLuma * 2, mcPredD+16, 32, &mvLumaForwDirect[2], 0);
                    LimitFMVQ(&mvLumaBackDirect[0], j << 4, curRow << 4);
                    LimitFMVQ(&mvLumaBackDirect[2], j << 4, curRow << 4);
                    mp4_Copy16x8QP_8u(pYb                          , mStepLuma * 2, mcTmp, 32, &mvLumaBackDirect[0], 0);
                    mp4_Copy16x8QP_8u(pYb+mStepLuma                , mStepLuma * 2, mcTmp+16, 32, &mvLumaBackDirect[2], 0);
                    mvLumaForwDirect[0].dx = (Ipp16s)mp4_Div2(mvLumaForwDirect[0].dx);
                    mvLumaForwDirect[0].dy = (Ipp16s)(mp4_Div2(mvLumaForwDirect[0].dy*2) >> 1);
                    mvLumaForwDirect[2].dx = (Ipp16s)mp4_Div2(mvLumaForwDirect[2].dx);
                    mvLumaForwDirect[2].dy = (Ipp16s)(mp4_Div2(mvLumaForwDirect[2].dy*2) >> 1);
                    mvLumaBackDirect[0].dx = (Ipp16s)mp4_Div2(mvLumaBackDirect[0].dx);
                    mvLumaBackDirect[0].dy = (Ipp16s)(mp4_Div2(mvLumaBackDirect[0].dy*2) >> 1);
                    mvLumaBackDirect[2].dx = (Ipp16s)mp4_Div2(mvLumaBackDirect[2].dx);
                    mvLumaBackDirect[2].dy = (Ipp16s)(mp4_Div2(mvLumaBackDirect[2].dy*2) >> 1);
                } else {
                    mp4_Copy16x8HP_8u(pYf+(mb_ftfr ? mStepLuma : 0), mStepLuma * 2, mcPredD, 32, &mvLumaForwDirect[0], 0);
                    mp4_Copy16x8HP_8u(pYf+(mb_fbfr ? mStepLuma : 0), mStepLuma * 2, mcPredD+16, 32, &mvLumaForwDirect[2], 0);
                    LimitFMV(&mvLumaBackDirect[0], j << 4, curRow << 4);
                    LimitFMV(&mvLumaBackDirect[2], j << 4, curRow << 4);
                    mp4_Copy16x8HP_8u(pYb                          , mStepLuma * 2, mcTmp, 32, &mvLumaBackDirect[0], 0);
                    mp4_Copy16x8HP_8u(pYb+mStepLuma                , mStepLuma * 2, mcTmp+16, 32, &mvLumaBackDirect[2], 0);
                }
            }
            ippiSAD16x16_8u32s(pYc, mStepLuma, mcPredD, 16, &bestSADDirect, IPPVC_MC_APX_FF);
            bestSADDirect -= SAD_FAVOR_DIRECT;
            if (bestSADDirect <= meData.thrDiff16x16) {
                mb_type = IPPVC_MBTYPE_DIRECT;
            } else {
                meData.pYc = pYc;
                meData.fcode = VOP.vop_fcode_forward;
                // SAD at (0,0)
                ippiSAD16x16_8u32s(pYc, mStepLuma, pYf, mStepLuma, &bestSAD16x16Forw, IPPVC_MC_APX_FF);
                if (meData.flags & ME_USE_MVWEIGHT)
                    bestSAD16x16Forw += mp4_WeightMV(0, 0, mvPredForw, meData.fcode, quant, 0);
                meData.pYr = pYf;
                meData.mvPred[0] = mvPredForw;
                meData.mvPred[1] = mvLumaForwDirect[0];
                meData.bestDiff = bestSAD16x16Forw;
                meData.yT = -IPP_MIN(curRow * 16 + 16, mBVOPsearchVerForw);
                meData.yB =  IPP_MIN((mNumMacroBlockPerCol - curRow) * 16, mBVOPsearchVerForw);
                meData.xL = -IPP_MIN(j * 16 + 16, mBVOPsearchHorForw);
                meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mBVOPsearchHorForw);
                mp4_ME_SAD(&meData);
                bestSAD16x16Forw = meData.bestDiff;
                mvLumaForw.dx = (Ipp16s)meData.xPos;
                mvLumaForw.dy = (Ipp16s)meData.yPos;
                mvForwDiff = mvPredForw;
                mp4_MV_GetDiff(&mvLumaForw, &mvForwDiff, fRangeMinForw, fRangeMaxForw, fRangeForw);
                if (bestSAD16x16Forw <= meData.thrDiff16x16) {
                    mb_type = IPPVC_MBTYPE_FORWARD;
                } else {
                    meData.fcode = VOP.vop_fcode_backward;
                    // SAD at (0,0)
                    ippiSAD16x16_8u32s(pYc, mStepLuma, pYb, mStepLuma, &bestSAD16x16Back, IPPVC_MC_APX_FF);
                    if (meData.flags & ME_USE_MVWEIGHT)
                        bestSAD16x16Back += mp4_WeightMV(0, 0, mvPredBack, meData.fcode, quant, 0);
                    meData.pYr = pYb;
                    meData.mvPred[0] = mvPredBack;
                    meData.mvPred[1] = mvLumaBackDirect[0];
                    meData.bestDiff = bestSAD16x16Back;
                    meData.yT = -IPP_MIN(curRow * 16 + 16, mBVOPsearchVerBack);
                    meData.yB =  IPP_MIN((mNumMacroBlockPerCol - curRow) * 16, mBVOPsearchVerBack);
                    meData.xL = -IPP_MIN(j * 16 + 16, mBVOPsearchHorBack);
                    meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mBVOPsearchHorBack);
                    mp4_ME_SAD(&meData);
                    bestSAD16x16Back = meData.bestDiff;
                    mvLumaBack.dx = (Ipp16s)meData.xPos;
                    mvLumaBack.dy = (Ipp16s)meData.yPos;
                    mvBackDiff = mvPredBack;
                    mp4_MV_GetDiff(&mvLumaBack, &mvBackDiff, fRangeMinBack, fRangeMaxBack, fRangeBack);
                    if (bestSAD16x16Back <= meData.thrDiff16x16) {
                        mb_type = IPPVC_MBTYPE_BACKWARD;
                    } else {
                        // Inter pred
                        if (VOL.quarter_sample) {
                            mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPredI, 16, &mvLumaForw, 0);
                            mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                        } else {
                            mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPredI, 16, &mvLumaForw, 0);
                            mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                        }
                        ippiAverage16x16_8u_C1IR(mcTmp, 16, mcPredI, 16);
                        ippiSAD16x16_8u32s(pYc, mStepLuma, mcPredI, 16, &bestSAD16x16Inter, IPPVC_MC_APX_FF);
                        if (meData.flags & ME_USE_MVWEIGHT) {
                            bestSAD16x16Inter += mp4_WeightMV(mvLumaForw.dx, mvLumaForw.dy, mvPredForw, meData.fcode, quant, 0);
                            bestSAD16x16Inter += mp4_WeightMV(mvLumaBack.dx, mvLumaBack.dy, mvPredBack, meData.fcode, quant, 0);
                        }
                        bestSAD = IPP_MIN(IPP_MIN(bestSAD16x16Forw, bestSAD16x16Back), IPP_MIN(bestSADDirect, bestSAD16x16Inter));
                        if (bestSADDirect == bestSAD) {
                            mb_type = IPPVC_MBTYPE_DIRECT;
                        } else if (bestSAD16x16Back == bestSAD) {
                            mb_type = IPPVC_MBTYPE_BACKWARD;
                        } else if (bestSAD16x16Forw == bestSAD) {
                            mb_type = IPPVC_MBTYPE_FORWARD;
                        } else {
                            mb_type = IPPVC_MBTYPE_INTERPOLATE;
                        }
                    }
                }
            }
            if (mb_type == IPPVC_MBTYPE_DIRECT) {
                if (!MBcurr->fieldmc) {
                    if (VOL.quarter_sample) {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                            mp4_ComputeChroma4MVQ(mvLumaForwDirect, &mvChromaF);
                            // The Chroma BackMV computation is done in ME stage
                            //mp4_ComputeChroma4MVQ(mvLumaBackDirect, &mvChromaB);
                            LimitCMV(&mvChromaB, j << 3, curRow << 3);
                        } else {
                            mp4_ComputeChromaMVQ(&mvLumaForwDirect[0], &mvChromaF);
                            mp4_ComputeChromaMVQ(&mvLumaBackDirect[0], &mvChromaB);
                        }
                    } else {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                            mp4_ComputeChroma4MV(mvLumaForwDirect, &mvChromaF);
                            // The Chroma BackMV computation is done in ME stage
                            //mp4_ComputeChroma4MV(mvLumaBackDirect, &mvChromaB);
                            LimitCMV(&mvChromaB, j << 3, curRow << 3);
                        } else {
                            mp4_ComputeChromaMV(&mvLumaForwDirect[0], &mvChromaF);
                            mp4_ComputeChromaMV(&mvLumaBackDirect[0], &mvChromaB);
                        }
                    }
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPredD+64*4, 8, &mvChromaF, 0);
                    mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPredD+64*5, 8, &mvChromaF, 0);
                    mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                } else {
                    IppMotionVector mvChromaFFT, mvChromaFFB, mvChromaBFT, mvChromaBFB;

                    mp4_ComputeChromaMV(&mvLumaForwDirect[0], &mvChromaFFT);
                    mp4_ComputeChromaMV(&mvLumaForwDirect[2], &mvChromaFFB);
                    mp4_ComputeChromaMV(&mvLumaBackDirect[0], &mvChromaBFT);
                    mp4_ComputeChromaMV(&mvLumaBackDirect[2], &mvChromaBFB);
                    mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*4, 16, &mvChromaFFT, 0);
                    mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*5, 16, &mvChromaFFT, 0);
                    mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*4+8, 16, &mvChromaFFB, 0);
                    mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*5+8, 16, &mvChromaFFB, 0);
                    mp4_Copy8x4HP_8u(pUb                            , mStepChroma*2, mcTmp+64*4, 16, &mvChromaBFT, 0);
                    mp4_Copy8x4HP_8u(pVb                            , mStepChroma*2, mcTmp+64*5, 16, &mvChromaBFT, 0);
                    mp4_Copy8x4HP_8u(pUb+mStepChroma                , mStepChroma*2, mcTmp+64*4+8, 16, &mvChromaBFB, 0);
                    mp4_Copy8x4HP_8u(pVb+mStepChroma                , mStepChroma*2, mcTmp+64*5+8, 16, &mvChromaBFB, 0);
                }
                ippiAverage8x8_8u_C1IR(mcTmp+64*4, 8, mcPredD+64*4, 8);
                ippiAverage8x8_8u_C1IR(mcTmp+64*5, 8, mcPredD+64*5, 8);
                mcPred = mcPredD;
            } else if (mb_type == IPPVC_MBTYPE_FORWARD) {
                if (VOL.quarter_sample) {
                    mp4_Copy16x16QP_8u(pYf, mStepLuma, mcTmp, 16, &mvLumaForw, 0);
                    mp4_ComputeChromaMVQ(&mvLumaForw, &mvChromaF);
                } else {
                    mp4_Copy16x16HP_8u(pYf, mStepLuma, mcTmp, 16, &mvLumaForw, 0);
                    mp4_ComputeChromaMV(&mvLumaForw, &mvChromaF);
                }
                mp4_Copy8x8HP_8u(pUf, mStepChroma, mcTmp+64*4, 8, &mvChromaF, 0);
                mp4_Copy8x8HP_8u(pVf, mStepChroma, mcTmp+64*5, 8, &mvChromaF, 0);
                mcPred = mcTmp;
                mvPredForw = mvLumaForw;
            } else if (mb_type == IPPVC_MBTYPE_BACKWARD) {
                if (VOL.quarter_sample) {
                    mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                    mp4_ComputeChromaMVQ(&mvLumaBack, &mvChromaB);
                } else {
                    mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                    mp4_ComputeChromaMV(&mvLumaBack, &mvChromaB);
                }
                mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                mcPred = mcTmp;
                mvPredBack = mvLumaBack;
            } else {
                if (VOL.quarter_sample) {
                    mp4_ComputeChromaMVQ(&mvLumaForw, &mvChromaF);
                    mp4_ComputeChromaMVQ(&mvLumaBack, &mvChromaB);
                } else {
                    mp4_ComputeChromaMV(&mvLumaForw, &mvChromaF);
                    mp4_ComputeChromaMV(&mvLumaBack, &mvChromaB);
                }
                mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPredI+64*4, 8, &mvChromaF, 0);
                mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPredI+64*5, 8, &mvChromaF, 0);
                mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                ippiAverage8x8_8u_C1IR(mcTmp+64*4, 8, mcPredI+64*4, 8);
                ippiAverage8x8_8u_C1IR(mcTmp+64*5, 8, mcPredI+64*5, 8);
                mcPred = mcPredI;
                mvPredBack = mvLumaBack;
                mvPredForw = mvLumaForw;
            }
            pMBinfoMT->pat = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, mcPred, curRow, j, &pMBinfoMT->dct_type, mMEflags & RD_TRELLIS, NULL);
            pMBinfoMT->mb_type = mb_type;
            if (mb_type != IPPVC_MBTYPE_DIRECT) {
                pMBinfoMT->mvForw = mvForwDiff;
                pMBinfoMT->mvBack = mvBackDiff;
            }
        }
        // restore VOP
        if (mReconstructAlways) {
            if (MBcurr->not_coded) {
                ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
                ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
                ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
            } else {
                if (pMBinfoMT->pat == 0) {
                    ippiCopy16x16_8u_C1R(mcPred, 16, pYc, mStepLuma);
                    ippiCopy8x8_8u_C1R(mcPred+64*4, 8, pUc, mStepChroma);
                    ippiCopy8x8_8u_C1R(mcPred+64*5, 8, pVc, mStepChroma);
                } else {
                    mp4_CopyCoeffsInter(pMBinfoMT->dctCoeffs, coeffMB, pMBinfoMT->pat);
                    ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pMBinfoMT->pat, pMBinfoMT->dct_type);
                }
            }
        }
        MBcurr ++;
        pMBinfoMT ++;
        pYc += 16; pUc += 8; pVc += 8;
        pYf += 16; pUf += 8; pVf += 8;
        pYb += 16; pUb += 8; pVb += 8;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    MBcurr -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    mvDelta.dx = mvDelta.dy = 0;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        if (!MBcurr->not_coded) {
            pattern = pMBinfoMT->pat;
            mb_type = pMBinfoMT->mb_type;
            if (mb_type == IPPVC_MBTYPE_DIRECT && pattern == 0) {
                modb = 2;
                // encode modb
                cBS.PutBit(1);
            } else {
                modb = pattern == 0 ? 1 : 0;
                // encode modb
                cBS.PutBits(modb, 2);
                // encode mb_type
                cBS.PutBits(1, 1 + mb_type - IPPVC_MBTYPE_DIRECT);
                // encode cbpb
                if (modb == 0)
                    cBS.PutBits(pattern, 6);
                // code dbquant
                if (mb_type != IPPVC_MBTYPE_DIRECT && pattern != 0)
                    cBS.PutBit(0);
                if (VOL.interlaced) {
                    // encode dct_type
                    if (pattern)
                        cBS.PutBit(pMBinfoMT->dct_type);
                    // encode field_prediction
                    if (mb_type != IPPVC_MBTYPE_DIRECT)
                        cBS.PutBit(0);
                }
                // encode delta mv
                if (mb_type == IPPVC_MBTYPE_DIRECT)
                    mp4_EncodeMV(cBS, &mvDelta, 1, IPPVC_MBTYPE_INTER);
                // encode forward mv
                if (mb_type == IPPVC_MBTYPE_FORWARD || mb_type == IPPVC_MBTYPE_INTERPOLATE)
                    mp4_EncodeMV(cBS, &pMBinfoMT->mvForw, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                // encode backward mv
                if (mb_type == IPPVC_MBTYPE_BACKWARD || mb_type == IPPVC_MBTYPE_INTERPOLATE)
                    mp4_EncodeMV(cBS, &pMBinfoMT->mvBack, VOP.vop_fcode_backward, IPPVC_MBTYPE_INTER);
                // encode blocks
                if (pattern)
                    mp4_EncodeMacroBlockInter_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, 0, VOP.alternate_vertical_scan_flag);
            }
        }
        MBcurr ++;
        pMBinfoMT ++;
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodeBSlice(mp4_Slice *slice)
{
    // only frame MC implemented for FORWARD, BACKWARD and INTERPOLATE
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPredD, 64*6);
    __ALIGN16(Ipp8u, mcPredI, 64*6);
    __ALIGN16(Ipp8u, mcTmp, 64*6);
    Ipp8u          *pYc, *pUc, *pVc, *pYf, *pUf, *pVf, *pYb, *pUb, *pVb, *sPtr, *mcPred = 0;
    Ipp32s          i, j, pattern = 0, modb = 0, mb_type, dbquant, quant, dct_type, nBits, sBitOff, mbn, startRow, qpel, mbnvp;
    Ipp32s          nzCount[6], bestSADDirect, bestSAD16x16Inter, bestSAD16x16Forw, bestSAD16x16Back, bestSAD;
    IppMotionVector mvLumaForw, mvPredForw, mvForwDiff, mvLumaBack, mvPredBack, mvBackDiff, mvChromaF, mvChromaB, mvLumaForwDirect[4], mvDelta, mvLumaBackDirect[4];
    mp4_MacroBlock *MBcurr;
    Ipp32s          fRangeMinForw = -(16 << VOP.vop_fcode_forward), fRangeMaxForw = (16 << VOP.vop_fcode_forward) - 1, fRangeForw = fRangeMaxForw - fRangeMinForw + 1;
    Ipp32s          fRangeMinBack = -(16 << VOP.vop_fcode_backward), fRangeMaxBack = (16 << VOP.vop_fcode_backward) - 1, fRangeBack = fRangeMaxBack - fRangeMinBack + 1;
    mp4_Data_ME     meData;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
#ifdef VIDEOPACKETS_LE_MAX
    Ipp8u  *cPtr = NULL;
    Ipp32s  cBitOff = 0;
#endif

    startRow = slice->startRow;
    mbn = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + mbn;
    mbnvp = dct_type = dbquant = 0;
    quant = VOP.vop_quant;
    qpel = VOL.quarter_sample;
    mvDelta.dx = mvDelta.dy = 0;
    meData.method = mMEmethod;
    meData.flags = mMEflags & (~ME_CHROMA);
    meData.quant = VOP.vop_quant;
    meData.rt = 0;
#ifdef USE_ME_SADBUFF
    meData.meBuff = slice->meBuff;
#endif
    meData.stepL = mStepLuma;
    meData.stepC = mStepChroma;
    if (meData.flags & ME_ZERO_MV) {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = SAD_MAX;
    } else if (meData.flags & ME_USE_THRESHOLD) {
        meData.thrDiff16x16 = VOP.vop_quant >= 6 ? 256 : (4 << VOP.vop_quant);
        meData.thrDiff8x8 = meData.thrDiff16x16 >> 2;
        meData.thrDiff16x8 = meData.thrDiff16x16 >> 1;
    } else {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = 0;
    }
    meData.thrDiff = meData.thrDiff16x16;
    meData.sadFunc = ippiSAD16x16_8u32s;
    meData.copyQPFunc = ippiCopy16x16QP_MPEG4_8u_C1R;
    meData.copyHPFunc = ippiCopy16x16HP_8u_C1R;
    meData.numPred = 2;
    slice->cBS.GetPos(&sPtr, &sBitOff);
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_B, quant);
    if (startRow > 0)
        EncodeVideoPacketHeader(slice->cBS, mbn, quant);
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pYc = mFrameC->pY + i * 16 * mStepLuma;
        pUc = mFrameC->pU + i * 8 * mStepChroma;
        pVc = mFrameC->pV + i * 8 * mStepChroma;
        pYf = mFrameF->pY + i * 16 * mStepLuma;
        pUf = mFrameF->pU + i * 8 * mStepChroma;
        pVf = mFrameF->pV + i * 8 * mStepChroma;
        pYb = mFrameB->pY + i * 16 * mStepLuma;
        pUb = mFrameB->pU + i * 8 * mStepChroma;
        pVb = mFrameB->pV + i * 8 * mStepChroma;
        mvPredForw.dx = mvPredForw.dy = mvPredBack.dx = mvPredBack.dy = 0;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            mb_type = -1;
            if (!MBcurr->not_coded) {
                // avoid warning
                mvLumaForw.dx = mvLumaForw.dy = mvLumaBack.dx = mvLumaBack.dy = 0;
                // Direct mode with MVd = 0
                if (!MBcurr->fieldmc) {
                    if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                        for (Ipp32s nB = 0; nB < 4; nB ++) {
                            mvLumaForwDirect[nB].dx = (Ipp16s)(mTRB * MBcurr->mv[nB].dx / mTRD);
                            mvLumaForwDirect[nB].dy = (Ipp16s)(mTRB * MBcurr->mv[nB].dy / mTRD);
                            mvLumaBackDirect[nB].dx = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[nB].dx / mTRD);
                            mvLumaBackDirect[nB].dy = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[nB].dy / mTRD);
                        }
                        if (VOL.quarter_sample) {
                            mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                            mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPredD+8, 16, &mvLumaForwDirect[1], 0);
                            mp4_Copy8x8QP_8u(pYf+8*mStepLuma, mStepLuma, mcPredD+8*16, 16, &mvLumaForwDirect[2], 0);
                            mp4_Copy8x8QP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPredD+8*16+8, 16, &mvLumaForwDirect[3], 0);
                            mp4_ComputeChroma4MVQ(mvLumaBackDirect, &mvChromaB);
                            Limit4MVQ(mvLumaBackDirect, j << 4, i << 4);
                            mp4_Copy8x8QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                            mp4_Copy8x8QP_8u(pYb+8, mStepLuma, mcTmp+8, 16, &mvLumaBackDirect[1], 0);
                            mp4_Copy8x8QP_8u(pYb+8*mStepLuma, mStepLuma, mcTmp+8*16, 16, &mvLumaBackDirect[2], 0);
                            mp4_Copy8x8QP_8u(pYb+8*mStepLuma+8, mStepLuma, mcTmp+8*16+8, 16, &mvLumaBackDirect[3], 0);
                        } else {
                            mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                            mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPredD+8, 16, &mvLumaForwDirect[1], 0);
                            mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPredD+8*16, 16, &mvLumaForwDirect[2], 0);
                            mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPredD+8*16+8, 16, &mvLumaForwDirect[3], 0);
                            mp4_ComputeChroma4MV(mvLumaBackDirect, &mvChromaB);
                            Limit4MV(mvLumaBackDirect, j << 4, i << 4);
                            mp4_Copy8x8HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                            mp4_Copy8x8HP_8u(pYb+8, mStepLuma, mcTmp+8, 16, &mvLumaBackDirect[1], 0);
                            mp4_Copy8x8HP_8u(pYb+8*mStepLuma, mStepLuma, mcTmp+8*16, 16, &mvLumaBackDirect[2], 0);
                            mp4_Copy8x8HP_8u(pYb+8*mStepLuma+8, mStepLuma, mcTmp+8*16+8, 16, &mvLumaBackDirect[3], 0);
                        }
                        ippiAverage8x8_8u_C1IR(mcTmp, 16, mcPredD, 16);
                        ippiAverage8x8_8u_C1IR(mcTmp+8, 16, mcPredD+8, 16);
                        ippiAverage8x8_8u_C1IR(mcTmp+8*16, 16, mcPredD+8*16, 16);
                        ippiAverage8x8_8u_C1IR(mcTmp+8*16+8, 16, mcPredD+8*16+8, 16);
                    } else {
                        mvLumaForwDirect[0].dx = (Ipp16s)(mTRB * MBcurr->mv[0].dx / mTRD);
                        mvLumaForwDirect[0].dy = (Ipp16s)(mTRB * MBcurr->mv[0].dy / mTRD);
                        mvLumaBackDirect[0].dx = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[0].dx / mTRD);
                        mvLumaBackDirect[0].dy = (Ipp16s)((mTRB - mTRD) * MBcurr->mv[0].dy / mTRD);
                        if (VOL.quarter_sample) {
                            mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                            LimitMVQ(mvLumaBackDirect, j << 4, i << 4);
                            mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                        } else {
                            mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPredD, 16, &mvLumaForwDirect[0], 0);
                            LimitMV(mvLumaBackDirect, j << 4, i << 4);
                            mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBackDirect[0], 0);
                        }
                        ippiAverage16x16_8u_C1IR(mcTmp, 16, mcPredD, 16);
                    }
                } else {
                    Ipp32s  TRDt, TRDb, TRBt, TRBb, deltaTop, deltaBottom;

                    deltaTop = mb_ftfr;
                    deltaBottom = mb_fbfr - 1;
                    if (VOP.top_field_first) {
                        deltaTop = -deltaTop;
                        deltaBottom = -deltaBottom;
                    }
                    TRDt = mp4_DivRoundInf(mTRD, mTframe) * 2 + deltaTop;
                    TRDb = mp4_DivRoundInf(mTRD, mTframe) * 2 + deltaBottom;
                    TRBt = mp4_DivRoundInf(mTRB, mTframe) * 2 + deltaTop;
                    TRBb = mp4_DivRoundInf(mTRB, mTframe) * 2 + deltaBottom;
                    mvLumaForwDirect[0].dx = (Ipp16s)((TRBt * MBcurr->mvT.dx) / TRDt);
                    mvLumaBackDirect[0].dx = (Ipp16s)(((TRBt - TRDt) * MBcurr->mvT.dx) / TRDt);
                    mvLumaForwDirect[0].dy = (Ipp16s)((TRBt * MBcurr->mvT.dy * 2) / TRDt);
                    mvLumaBackDirect[0].dy = (Ipp16s)(((TRBt - TRDt) * MBcurr->mvT.dy * 2) / TRDt);
                    mvLumaForwDirect[2].dx = (Ipp16s)((TRBb * MBcurr->mvB.dx) / TRDb);
                    mvLumaBackDirect[2].dx = (Ipp16s)(((TRBb - TRDb) * MBcurr->mvB.dx) / TRDb);
                    mvLumaForwDirect[2].dy = (Ipp16s)((TRBb * MBcurr->mvB.dy * 2) / TRDb);
                    mvLumaBackDirect[2].dy = (Ipp16s)(((TRBb - TRDb) * MBcurr->mvB.dy * 2) / TRDb);
                    mvLumaForwDirect[0].dy >>= 1;
                    mvLumaBackDirect[0].dy >>= 1;
                    mvLumaForwDirect[2].dy >>= 1;
                    mvLumaBackDirect[2].dy >>= 1;
                    if (VOL.quarter_sample) {
                        mp4_Copy16x8QP_8u(pYf+(mb_ftfr ? mStepLuma : 0), mStepLuma * 2, mcPredD, 32, &mvLumaForwDirect[0], 0);
                        mp4_Copy16x8QP_8u(pYf+(mb_fbfr ? mStepLuma : 0), mStepLuma * 2, mcPredD+16, 32, &mvLumaForwDirect[2], 0);
                        LimitFMVQ(&mvLumaBackDirect[0], j << 4, i << 4);
                        LimitFMVQ(&mvLumaBackDirect[2], j << 4, i << 4);
                        mp4_Copy16x8QP_8u(pYb                          , mStepLuma * 2, mcTmp, 32, &mvLumaBackDirect[0], 0);
                        mp4_Copy16x8QP_8u(pYb+mStepLuma                , mStepLuma * 2, mcTmp+16, 32, &mvLumaBackDirect[2], 0);
                        mvLumaForwDirect[0].dx = (Ipp16s)mp4_Div2(mvLumaForwDirect[0].dx);
                        mvLumaForwDirect[0].dy = (Ipp16s)(mp4_Div2(mvLumaForwDirect[0].dy*2) >> 1);
                        mvLumaForwDirect[2].dx = (Ipp16s)mp4_Div2(mvLumaForwDirect[2].dx);
                        mvLumaForwDirect[2].dy = (Ipp16s)(mp4_Div2(mvLumaForwDirect[2].dy*2) >> 1);
                        mvLumaBackDirect[0].dx = (Ipp16s)mp4_Div2(mvLumaBackDirect[0].dx);
                        mvLumaBackDirect[0].dy = (Ipp16s)(mp4_Div2(mvLumaBackDirect[0].dy*2) >> 1);
                        mvLumaBackDirect[2].dx = (Ipp16s)mp4_Div2(mvLumaBackDirect[2].dx);
                        mvLumaBackDirect[2].dy = (Ipp16s)(mp4_Div2(mvLumaBackDirect[2].dy*2) >> 1);
                    } else {
                        mp4_Copy16x8HP_8u(pYf+(mb_ftfr ? mStepLuma : 0), mStepLuma * 2, mcPredD, 32, &mvLumaForwDirect[0], 0);
                        mp4_Copy16x8HP_8u(pYf+(mb_fbfr ? mStepLuma : 0), mStepLuma * 2, mcPredD+16, 32, &mvLumaForwDirect[2], 0);
                        LimitFMV(&mvLumaBackDirect[0], j << 4, i << 4);
                        LimitFMV(&mvLumaBackDirect[2], j << 4, i << 4);
                        mp4_Copy16x8HP_8u(pYb                          , mStepLuma * 2, mcTmp, 32, &mvLumaBackDirect[0], 0);
                        mp4_Copy16x8HP_8u(pYb+mStepLuma                , mStepLuma * 2, mcTmp+16, 32, &mvLumaBackDirect[2], 0);
                    }
                }
                ippiSAD16x16_8u32s(pYc, mStepLuma, mcPredD, 16, &bestSADDirect, IPPVC_MC_APX_FF);
                bestSADDirect -= SAD_FAVOR_DIRECT;
                if (bestSADDirect <= meData.thrDiff16x16) {
                    mb_type = IPPVC_MBTYPE_DIRECT;
                } else {
                    meData.pYc = pYc;
                    meData.fcode = VOP.vop_fcode_forward;
                    // SAD at (0,0)
                    ippiSAD16x16_8u32s(pYc, mStepLuma, pYf, mStepLuma, &bestSAD16x16Forw, IPPVC_MC_APX_FF);
                    if (meData.flags & ME_USE_MVWEIGHT)
                        bestSAD16x16Forw += mp4_WeightMV(0, 0, mvPredForw, meData.fcode, quant, 0);
                    meData.pYr = pYf;
                    meData.bestDiff = bestSAD16x16Forw;
                    meData.yT = -IPP_MIN(i * 16 + 16, mBVOPsearchVerForw);
                    meData.yB =  IPP_MIN((mNumMacroBlockPerCol - i) * 16, mBVOPsearchVerForw);
                    meData.xL = -IPP_MIN(j * 16 + 16, mBVOPsearchHorForw);
                    meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mBVOPsearchHorForw);
                    meData.mvPred[0] = mvPredForw;
                    meData.mvPred[1] = mvLumaForwDirect[0];
                    mp4_ME_SAD(&meData);
                    bestSAD16x16Forw = meData.bestDiff;
                    mvLumaForw.dx = (Ipp16s)meData.xPos;
                    mvLumaForw.dy = (Ipp16s)meData.yPos;
                    mvForwDiff = mvPredForw;
                    mp4_MV_GetDiff(&mvLumaForw, &mvForwDiff, fRangeMinForw, fRangeMaxForw, fRangeForw);
                    if (bestSAD16x16Forw <= meData.thrDiff16x16) {
                        mb_type = IPPVC_MBTYPE_FORWARD;
                    } else {
                        meData.fcode = VOP.vop_fcode_backward;
                        // SAD at (0,0)
                        ippiSAD16x16_8u32s(pYc, mStepLuma, pYb, mStepLuma, &bestSAD16x16Back, IPPVC_MC_APX_FF);
                        if (meData.flags & ME_USE_MVWEIGHT)
                            bestSAD16x16Back += mp4_WeightMV(0, 0, mvPredBack, meData.fcode, quant, 0);
                        meData.pYr = pYb;
                        meData.bestDiff = bestSAD16x16Back;
                        meData.yT = -IPP_MIN(i * 16 + 16, mBVOPsearchVerBack);
                        meData.yB =  IPP_MIN((mNumMacroBlockPerCol - i) * 16, mBVOPsearchVerBack);
                        meData.xL = -IPP_MIN(j * 16 + 16, mBVOPsearchHorBack);
                        meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mBVOPsearchHorBack);
                        meData.mvPred[0] = mvPredBack;
                        meData.mvPred[1] = mvLumaBackDirect[0];
                        mp4_ME_SAD(&meData);
                        bestSAD16x16Back = meData.bestDiff;
                        mvLumaBack.dx = (Ipp16s)meData.xPos;
                        mvLumaBack.dy = (Ipp16s)meData.yPos;
                        mvBackDiff = mvPredBack;
                        mp4_MV_GetDiff(&mvLumaBack, &mvBackDiff, fRangeMinBack, fRangeMaxBack, fRangeBack);
                        if (bestSAD16x16Back <= meData.thrDiff16x16) {
                            mb_type = IPPVC_MBTYPE_BACKWARD;
                        } else {
                            // Inter pred
                            if (VOL.quarter_sample) {
                                mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPredI, 16, &mvLumaForw, 0);
                                mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                            } else {
                                mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPredI, 16, &mvLumaForw, 0);
                                mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                            }
                            ippiAverage16x16_8u_C1IR(mcTmp, 16, mcPredI, 16);
                            ippiSAD16x16_8u32s(pYc, mStepLuma, mcPredI, 16, &bestSAD16x16Inter, IPPVC_MC_APX_FF);
                            if (meData.flags & ME_USE_MVWEIGHT) {
                                bestSAD16x16Inter += mp4_WeightMV(mvLumaForw.dx, mvLumaForw.dy, mvPredForw, meData.fcode, quant, 0);
                                bestSAD16x16Inter += mp4_WeightMV(mvLumaBack.dx, mvLumaBack.dy, mvPredBack, meData.fcode, quant, 0);
                            }
                            bestSAD = IPP_MIN(IPP_MIN(bestSAD16x16Forw, bestSAD16x16Back), IPP_MIN(bestSADDirect, bestSAD16x16Inter));
                            if (bestSADDirect == bestSAD) {
                                mb_type = IPPVC_MBTYPE_DIRECT;
                            } else if (bestSAD16x16Back == bestSAD) {
                                mb_type = IPPVC_MBTYPE_BACKWARD;
                            } else if (bestSAD16x16Forw == bestSAD) {
                                mb_type = IPPVC_MBTYPE_FORWARD;
                            } else {
                                mb_type = IPPVC_MBTYPE_INTERPOLATE;
                            }
                        }
                    }
                }
                if (mb_type == IPPVC_MBTYPE_DIRECT) {
                    if (!MBcurr->fieldmc) {
                        if (VOL.quarter_sample) {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_ComputeChroma4MVQ(mvLumaForwDirect, &mvChromaF);
                                // The Chroma BackMV computation is done in ME stage
                                //mp4_ComputeChroma4MVQ(mvLumaBackDirect, &mvChromaB);
                                LimitCMV(&mvChromaB, j << 3, i << 3);
                            } else {
                                mp4_ComputeChromaMVQ(&mvLumaForwDirect[0], &mvChromaF);
                                mp4_ComputeChromaMVQ(&mvLumaBackDirect[0], &mvChromaB);
                            }
                        } else {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_ComputeChroma4MV(mvLumaForwDirect, &mvChromaF);
                                // The Chroma BackMV computation is done in ME stage
                                //mp4_ComputeChroma4MV(mvLumaBackDirect, &mvChromaB);
                                LimitCMV(&mvChromaB, j << 3, i << 3);
                            } else {
                                mp4_ComputeChromaMV(&mvLumaForwDirect[0], &mvChromaF);
                                mp4_ComputeChromaMV(&mvLumaBackDirect[0], &mvChromaB);
                            }
                        }
                        mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPredD+64*4, 8, &mvChromaF, 0);
                        mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                        mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPredD+64*5, 8, &mvChromaF, 0);
                        mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                    } else {
                        IppMotionVector mvChromaFFT, mvChromaFFB, mvChromaBFT, mvChromaBFB;

                        mp4_ComputeChromaMV(&mvLumaForwDirect[0], &mvChromaFFT);
                        mp4_ComputeChromaMV(&mvLumaForwDirect[2], &mvChromaFFB);
                        mp4_ComputeChromaMV(&mvLumaBackDirect[0], &mvChromaBFT);
                        mp4_ComputeChromaMV(&mvLumaBackDirect[2], &mvChromaBFB);
                        mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*4, 16, &mvChromaFFT, 0);
                        mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*5, 16, &mvChromaFFT, 0);
                        mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*4+8, 16, &mvChromaFFB, 0);
                        mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPredD+64*5+8, 16, &mvChromaFFB, 0);
                        mp4_Copy8x4HP_8u(pUb                            , mStepChroma*2, mcTmp+64*4, 16, &mvChromaBFT, 0);
                        mp4_Copy8x4HP_8u(pVb                            , mStepChroma*2, mcTmp+64*5, 16, &mvChromaBFT, 0);
                        mp4_Copy8x4HP_8u(pUb+mStepChroma                , mStepChroma*2, mcTmp+64*4+8, 16, &mvChromaBFB, 0);
                        mp4_Copy8x4HP_8u(pVb+mStepChroma                , mStepChroma*2, mcTmp+64*5+8, 16, &mvChromaBFB, 0);
                    }
                    ippiAverage8x8_8u_C1IR(mcTmp+64*4, 8, mcPredD+64*4, 8);
                    ippiAverage8x8_8u_C1IR(mcTmp+64*5, 8, mcPredD+64*5, 8);
                    mcPred = mcPredD;
                } else if (mb_type == IPPVC_MBTYPE_FORWARD) {
                    if (VOL.quarter_sample) {
                        mp4_Copy16x16QP_8u(pYf, mStepLuma, mcTmp, 16, &mvLumaForw, 0);
                        mp4_ComputeChromaMVQ(&mvLumaForw, &mvChromaF);
                    } else {
                        mp4_Copy16x16HP_8u(pYf, mStepLuma, mcTmp, 16, &mvLumaForw, 0);
                        mp4_ComputeChromaMV(&mvLumaForw, &mvChromaF);
                    }
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcTmp+64*4, 8, &mvChromaF, 0);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcTmp+64*5, 8, &mvChromaF, 0);
                    mcPred = mcTmp;
                    mvPredForw = mvLumaForw;
                } else if (mb_type == IPPVC_MBTYPE_BACKWARD) {
                    if (VOL.quarter_sample) {
                        mp4_Copy16x16QP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                        mp4_ComputeChromaMVQ(&mvLumaBack, &mvChromaB);
                    } else {
                        mp4_Copy16x16HP_8u(pYb, mStepLuma, mcTmp, 16, &mvLumaBack, 0);
                        mp4_ComputeChromaMV(&mvLumaBack, &mvChromaB);
                    }
                    mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                    mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                    mcPred = mcTmp;
                    mvPredBack = mvLumaBack;
                } else {
                    if (VOL.quarter_sample) {
                        mp4_ComputeChromaMVQ(&mvLumaForw, &mvChromaF);
                        mp4_ComputeChromaMVQ(&mvLumaBack, &mvChromaB);
                    } else {
                        mp4_ComputeChromaMV(&mvLumaForw, &mvChromaF);
                        mp4_ComputeChromaMV(&mvLumaBack, &mvChromaB);
                    }
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPredI+64*4, 8, &mvChromaF, 0);
                    mp4_Copy8x8HP_8u(pUb, mStepChroma, mcTmp+64*4, 8, &mvChromaB, 0);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPredI+64*5, 8, &mvChromaF, 0);
                    mp4_Copy8x8HP_8u(pVb, mStepChroma, mcTmp+64*5, 8, &mvChromaB, 0);
                    ippiAverage8x8_8u_C1IR(mcTmp+64*4, 8, mcPredI+64*4, 8);
                    ippiAverage8x8_8u_C1IR(mcTmp+64*5, 8, mcPredI+64*5, 8);
                    mcPred = mcPredI;
                    mvPredBack = mvLumaBack;
                    mvPredForw = mvLumaForw;
                }
                pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, i, j, &dct_type, mMEflags & RD_TRELLIS, NULL);
                if (mb_type == IPPVC_MBTYPE_DIRECT && pattern == 0) {
                    modb = 2;
                    // encode modb
                    slice->cBS.PutBit(1);
                } else {
                    modb = pattern == 0 ? 1 : 0;
                    // encode modb
                    slice->cBS.PutBits(modb, 2);
                    // encode mb_type
                    slice->cBS.PutBits(1, 1 + mb_type - IPPVC_MBTYPE_DIRECT);
                    // encode cbpb
                    if (modb == 0)
                        slice->cBS.PutBits(pattern, 6);
                    // code dbquant
                    if (mb_type != IPPVC_MBTYPE_DIRECT && pattern != 0)
                    {
                        if (dbquant == 0)
                            slice->cBS.PutBit(0);
                        else
                            slice->cBS.PutBits(dbquant < 0 ? 2 : 3, 2);
                    }
                    if (VOL.interlaced) {
                        // encode dct_type
                        if (pattern)
                            slice->cBS.PutBit(dct_type);
                        // encode field_prediction
                        if (mb_type != IPPVC_MBTYPE_DIRECT)
                            slice->cBS.PutBit(0);
                    }
                    // encode delta mv
                    if (mb_type == IPPVC_MBTYPE_DIRECT)
                        mp4_EncodeMV(slice->cBS, &mvDelta, 1, IPPVC_MBTYPE_INTER);
                    // encode forward mv
                    if (mb_type == IPPVC_MBTYPE_FORWARD || mb_type == IPPVC_MBTYPE_INTERPOLATE)
                        mp4_EncodeMV(slice->cBS, &mvForwDiff, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                    // encode backward mv
                    if (mb_type == IPPVC_MBTYPE_BACKWARD || mb_type == IPPVC_MBTYPE_INTERPOLATE)
                        mp4_EncodeMV(slice->cBS, &mvBackDiff, VOP.vop_fcode_backward, IPPVC_MBTYPE_INTER);
                    // encode blocks
                    if (pattern)
                        mp4_EncodeMacroBlockInter_MPEG4(slice->cBS, coeffMB, pattern, nzCount, 0, VOP.alternate_vertical_scan_flag);
                }
            }
            // restore VOP
            if (mReconstructAlways) {
                if (MBcurr->not_coded) {
                    ippiCopy16x16_8u_C1R(pYf, mStepLuma, pYc, mStepLuma);
                    ippiCopy8x8_8u_C1R(pUf, mStepChroma, pUc, mStepChroma);
                    ippiCopy8x8_8u_C1R(pVf, mStepChroma, pVc, mStepChroma);
                } else {
                    if (modb != 0) {  // pattern == 0
                        ippiCopy16x16_8u_C1R(mcPred, 16, pYc, mStepLuma);
                        ippiCopy8x8_8u_C1R(mcPred+64*4, 8, pUc, mStepChroma);
                        ippiCopy8x8_8u_C1R(mcPred+64*5, 8, pVc, mStepChroma);
                    } else
                        ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern, dct_type);
                }
            }
            if (mInsertResync) {
                mbn ++;
                mbnvp ++;
                nBits = slice->cBS.GetNumBits(sPtr, sBitOff);
#ifdef VIDEOPACKETS_LE_MAX
                if (nBits > mVideoPacketLength) {
                    if (mbnvp > 1) {
                        slice->cBS.SetPos(cPtr, cBitOff);
                        pYc -= 16; pUc -= 8; pVc -= 8;
                        pYf -= 16; pUf -= 8; pVf -= 8;
                        pYb -= 16; pUb -= 8; pVb -= 8;
                        j --;
                        if (j < 0) {
                            j = mNumMacroBlockPerRow - 1;
                            i --;
                        }
                        mbnvp --;
                        mbn --;
                        MBcurr --;
                    }
#else
                if ((nBits >= mVideoPacketLength) && mbn < mNumMacroBlockPerVOP) {
#endif
                    mvPredForw.dx = mvPredForw.dy = mvPredBack.dx = mvPredBack.dy = 0;
                    slice->cBS.GetPos(&sPtr, &sBitOff);
                    mp4_EncodeStuffingBitsAlign(slice->cBS);
                    EncodeVideoPacketHeader(slice->cBS, mbn, quant);
                    mbnvp = 0;
                }
#ifdef VIDEOPACKETS_LE_MAX
                slice->cBS.GetPos(&cPtr, &cBitOff);
#endif
            }
            slice->quantSum += quant;
            if (mRateControl == 2) {
                if (!MBcurr->not_coded && modb != 2 && mb_type != IPPVC_MBTYPE_DIRECT && pattern) {
                    mRC_MB.Update(slice, &dbquant, MP4_VOP_TYPE_B);
                    quant += dbquant;
                }
            }
            MBcurr ++;
            pYc += 16; pUc += 8; pVc += 8;
            pYf += 16; pUf += 8; pVf += 8;
            pYb += 16; pUb += 8; pVb += 8;
        }
    }
    mp4_EncodeStuffingBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodeBVOP()
{
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if (mThreadingAlg == 0 && mRateControl != 2 && !mInsertResync) {
            Ipp32s  i;

            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodeBRow(curRow, mp4_MT_get_thread_num());
                    curRow ++;
                }
            }
            mp4_EncodeStuffingBitsAlign(cBS);
        } else {
            Ipp32s  i;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodeBSlice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodeBSlice(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            cBS = mSliceMT[0].cBS;
            for (i = 1; i < mNumThreads; i ++) {
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
            }
        }
    } else
#endif // _OMP_KARABAS
    {
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodeBSlice(&mSlice);
        mQuantSum = mSlice.quantSum;
        cBS = mSlice.cBS;
    }
}

#ifdef _OMP_KARABAS
void VideoEncoderMPEG4::EncodeSRow(Ipp32s curRow, Ipp32s threadNum)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    mp4_MacroBlock *MBcurr;
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s  j, quant, pattern, ac_pred_flag, use_intra_dc_vlc, dct_type, rt, pattern1;
    IppMotionVector mvLuma4[4], mvChroma, mvChromaT, mvChromaB/*, mvPred[4]*/;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
    IppiRect    mbRectL, mbRectC;
    //Ipp32s  fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
    mp4_MacroBlockMT* pMBinfoMT;

    mp4_MT_set_lock(mLockMT+curRow);
    pMBinfoMT = MBinfoMT + mNumMacroBlockPerRow * threadNum;
    mbRectL.y = curRow * 16;
    mbRectC.y = curRow * 8;
    mbRectL.x = mbRectC.x = 0;
    mbRectL.width = 16;  mbRectL.height = 16;
    mbRectC.width = 8;  mbRectC.height = 8;
    rt = VOP.vop_rounding_type;
    dct_type = 0;
    quant = VOP.vop_quant;
    use_intra_dc_vlc = 1; //quant < mDC_VLC_Threshold[VOP.intra_dc_vlc_thr];
    pYc = mFrameC->pY + curRow * 16 * mStepLuma;
    pUc = mFrameC->pU + curRow * 8 * mStepChroma;
    pVc = mFrameC->pV + curRow * 8 * mStepChroma;
    pYf = mFrameF->pY + curRow * 16 * mStepLuma;
    pUf = mFrameF->pU + curRow * 8 * mStepChroma;
    pVf = mFrameF->pV + curRow * 8 * mStepChroma;
    MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
            // intra coded
            mp4_WaitIntra(curRow);
            pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, curRow, j, &dct_type, use_intra_dc_vlc, MBcurr, pMBinfoMT->predDir, 0, &ac_pred_flag, &pattern1, NULL);
            mp4_CopyCoeffsIntra(pMBinfoMT->dctCoeffs, coeffMB, pattern1);
            if (ac_pred_flag)
                mp4_RestoreIntraAC(MBcurr, coeffMB, pMBinfoMT->predDir);
            ReconMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, quant, MBcurr, pattern1, dct_type);
            pMBinfoMT->ac_pred_flag = ac_pred_flag;
        } else {
            // inter coded
            MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
        if (!MBcurr->mcsel) {
            if (!MBcurr->fieldmc) {
                mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                // find Luma Pred
                if (VOL.obmc_disable) {
                    if (VOL.quarter_sample) {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                            mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                            mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                        } else
                            mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                    } else {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                            mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                            mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                            mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                            mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                        } else
                            mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                    }
                } else {
                    IppMotionVector *mvRight, *mvLeft, *mvUpper;
                    mvRight = (j == mNumMacroBlockPerRow - 1) ? &mvLuma4[1] : (MBcurr[1].type == IPPVC_MBTYPE_INTRA || MBcurr[1].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[1].mcsel) ? &mvLuma4[1] : MBcurr[1].mv;
                    mvLeft = (j == 0) ? mvLuma4 - 1 : (MBcurr[-1].type == IPPVC_MBTYPE_INTRA || MBcurr[-1].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[-1].mcsel) ? mvLeft = mvLuma4 - 1 : MBcurr[-1].mv;
                    mvUpper = (curRow == 0) ? mvLuma4 - 2 : (MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA || MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[-mNumMacroBlockPerRow].mcsel) ? mvLuma4 - 2 : MBcurr[-mNumMacroBlockPerRow].mv;
                    if (VOL.quarter_sample) {
                        ippiOBMC8x8QP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                        ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                        ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                        ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                    } else {
                        ippiOBMC8x8HP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                        ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                        ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                        ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                    }
                }
                // calculate Chroma MV
                if (VOL.quarter_sample) {
                    if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                        mp4_ComputeChroma4MVQ(mvLuma4, &mvChroma);
                    else
                        mp4_ComputeChromaMVQ(mvLuma4, &mvChroma);
                } else {
                    if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                        mp4_ComputeChroma4MV(mvLuma4, &mvChroma);
                    else
                        mp4_ComputeChromaMV(mvLuma4, &mvChroma);
                }
                // find Chroma Pred
                mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
            } else {
                mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                // find Luma Pred
                if (VOL.quarter_sample) {
                    mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                    mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                } else {
                    mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                    mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                }
                // calculate Chroma MV
                if (VOL.quarter_sample) {
                    IppMotionVector mvTmpT, mvTmpB;
                    mvTmpT.dx = (Ipp16s)mp4_Div2(MBcurr->mvT.dx);
                    mvTmpT.dy = (Ipp16s)(mp4_Div2(MBcurr->mvT.dy << 1) >> 1);
                    mvTmpB.dx = (Ipp16s)mp4_Div2(MBcurr->mvB.dx);
                    mvTmpB.dy = (Ipp16s)(mp4_Div2(MBcurr->mvB.dy << 1) >> 1);
                    mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                } else {
                    mp4_ComputeChromaMV(&MBcurr->mvT, &mvChromaT);
                    mp4_ComputeChromaMV(&MBcurr->mvB, &mvChromaB);
                }
                // find Chroma Pred
                mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5+8, 16, &mvChromaB, rt);
            }
        } else {
            // apply warping to the macroblock
            ippiWarpLuma_MPEG4_8u_C1R(mFrameF->pY, mStepLuma, mcPred, 16, &mbRectL, mWarpSpec);
            ippiWarpChroma_MPEG4_8u_P2R(mFrameF->pU, mStepChroma, mFrameF->pV, mStepChroma, mcPred+64*4, 8, mcPred+64*5, 8, &mbRectC, mWarpSpec);
        }
        if (MBcurr->not_coded) {
            pattern = 0;
        } else {
            pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, pMBinfoMT->dctCoeffs, pMBinfoMT->nzCount, quant, mcPred, curRow, j, &dct_type, mMEflags & RD_TRELLIS, NULL);
        }
            if (pattern == 0 || MBcurr->not_coded)
                ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
            else {
                mp4_CopyCoeffsInter(pMBinfoMT->dctCoeffs, coeffMB, pattern);
                ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern, dct_type);
            }
        }
        pMBinfoMT->pat = pattern;
        pMBinfoMT->dct_type = dct_type;
        pMBinfoMT ++;
        pYc += 16; pUc += 8; pVc += 8;
        pYf += 16; pUf += 8; pVf += 8;
        MBcurr ++;
        mbRectL.x += 16;
        mbRectC.x += 8;
        mCurRowMT[curRow] ++;
    }
    pMBinfoMT -= mNumMacroBlockPerRow;
    MBcurr -= mNumMacroBlockPerRow;
    if (curRow > 0) {
        mp4_MT_set_lock(mLockMT+curRow-1);
        mp4_MT_unset_lock(mLockMT+curRow-1);
    }
    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
        pattern = pMBinfoMT->pat;
        if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
            cBS.PutBit(0);
            mp4_EncodeMCBPC_P(cBS, IPPVC_MBTYPE_INTRA, pattern & 3);
            cBS.PutBit(pMBinfoMT->ac_pred_flag);
            mp4_EncodeCBPY_P(cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
            if (VOL.interlaced)
                cBS.PutBit(pMBinfoMT->dct_type);
            mp4_EncodeMacroBlockIntra_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, pMBinfoMT->predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
        } else {
            if (!MBcurr->mcsel) {
                if (!MBcurr->fieldmc) {
                    mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                } else {
                    mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                }
            }
            if (MBcurr->mcsel && pattern == 0 && MBcurr->not_coded) {
                // for B-frames notcoded mcsel MBs should be treated as coded
                MBcurr->not_coded = 0;
                // encode not_coded
                cBS.PutBit(1);
            } else {
/*
if (!MBcurr->mcsel) {
                Predict1MV(MBcurr, curRow, j, &mvPred[0]);
                if (!MBcurr->fieldmc) {
                    mp4_MV_GetDiff(&mvLuma4[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
                    if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                        Predict3MV(MBcurr, curRow, j, mvPred, mvLuma4);
                        mp4_MV_GetDiff(&mvLuma4[1], &mvPred[1], fRangeMin, fRangeMax, fRange);
                        mp4_MV_GetDiff(&mvLuma4[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
                        mp4_MV_GetDiff(&mvLuma4[3], &mvPred[3], fRangeMin, fRangeMax, fRange);
                    }
                } else {
                    mvPred[0].dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
                    mvPred[2] = mvPred[0];
                    mp4_MV_GetDiff(&mvLuma4[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
                    mp4_MV_GetDiff(&mvLuma4[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
                }
}
*/
                // encode not_coded
                cBS.PutBit(0);
                mp4_EncodeMCBPC_P(cBS, MBcurr->type, pattern & 3);
                if (MBcurr->type < IPPVC_MBTYPE_INTER4V)
                    cBS.PutBit(MBcurr->mcsel);
                mp4_EncodeCBPY_P(cBS, MBcurr->type, pattern >> 2);
                if (VOL.interlaced) {
                    // encode dct_type
                    if (pattern)
                        cBS.PutBit(pMBinfoMT->dct_type);
                    // encode field_prediction
                if (!MBcurr->mcsel) {
                    if (MBcurr->type != IPPVC_MBTYPE_INTER4V) {
                        cBS.PutBit(MBcurr->fieldmc);
                        if (MBcurr->fieldmc) {
                            cBS.PutBit(mb_ftfr);
                            cBS.PutBit(mb_fbfr);
                        }
                    }
                    mp4_EncodeMV(cBS, &MBcurr->mvDiff[0]/*&mvPred[0]*/, VOP.vop_fcode_forward, MBcurr->type);
                    if (MBcurr->fieldmc)
                        mp4_EncodeMV(cBS, &MBcurr->mvDiff[2]/*&mvPred[2]*/, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                    }
                } else
                if (!MBcurr->mcsel)
                    mp4_EncodeMV(cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                mp4_EncodeMacroBlockInter_MPEG4(cBS, pMBinfoMT->dctCoeffs, pattern, pMBinfoMT->nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
            }
        }
        pMBinfoMT ++;
        MBcurr ++;
    }
    mp4_MT_unset_lock(mLockMT+curRow);
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::EncodeSSlice(mp4_Slice *slice)
{
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    mp4_MacroBlock *MBcurr;
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf, *sPtr;
    Ipp32s  i, j, quant, pattern, nzCount[6], predDir[6], ac_pred_flag, use_intra_dc_vlc, dct_type, pattern1, rt, dquant;
    IppMotionVector mvLuma4[4], mvChroma, /*mvPred[4], */mvChromaT, mvChromaB;
    int     fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
    Ipp32s  nBits, sBitOff, startRow, mbn, mbnvp;
    const Ipp32s mb_ftfr = 0, mb_fbfr = 1; // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
    IppiRect    mbRectL, mbRectC;
#ifdef VIDEOPACKETS_LE_MAX
    Ipp8u  *cPtr = NULL, *cPtr_1 = NULL, *cPtr_2 = NULL;
    Ipp32s  cBitOff = 0, cBitOff_1 = 0, cBitOff_2 = 0;
#endif
    bool vp_step_back = false;

    startRow = slice->startRow;
    mbn = startRow * mNumMacroBlockPerRow;
    MBcurr = MBinfo + mbn;
    mbnvp = dct_type = dquant = 0;
    quant = VOP.vop_quant;
    use_intra_dc_vlc = 1; //quant < mDC_VLC_Threshold[VOP.intra_dc_vlc_thr];
    rt = VOP.vop_rounding_type;
    mbRectL.y = mbRectC.y = 0;
    mbRectL.width = 16;  mbRectL.height = 16;
    mbRectC.width = 8;  mbRectC.height = 8;
    if (mRateControl == 2)
        mRC_MB.Start(slice, MP4_VOP_TYPE_S, quant);
    slice->cBS.GetPos(&sPtr, &sBitOff);
    if (startRow > 0)
        EncodeVideoPacketHeader(slice->cBS, mbn, quant);
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        pYc = mFrameC->pY + i * 16 * mStepLuma;
        pUc = mFrameC->pU + i * 8 * mStepChroma;
        pVc = mFrameC->pV + i * 8 * mStepChroma;
        pYf = mFrameF->pY + i * 16 * mStepLuma;
        pUf = mFrameF->pU + i * 8 * mStepChroma;
        pVf = mFrameF->pV + i * 8 * mStepChroma;
        mbRectL.x = 0;
        mbRectC.x = 0;
        for (j = 0; j < mNumMacroBlockPerRow; j ++) {
            if (MBcurr->type == IPPVC_MBTYPE_INTRA) {
                // intra coded
                pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, i, j, &dct_type, use_intra_dc_vlc, MBcurr, predDir, startRow, &ac_pred_flag, &pattern1, NULL);
                // encode not_coded
                slice->cBS.PutBit(0);
                mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? IPPVC_MBTYPE_INTRA : IPPVC_MBTYPE_INTRA_Q, pattern & 3);
                if (VOL.data_partitioned) {
                    // encode to dp_buff_1
                    slice->cBS_1.PutBit(ac_pred_flag);
                    mp4_EncodeCBPY_P(slice->cBS_1, IPPVC_MBTYPE_INTRA, pattern >> 2);
                    if (dquant != 0)
                        mp4_EncodeDquant(slice->cBS_1, dquant);
                    if (use_intra_dc_vlc)
                        mp4_EncodeMacroBlockIntra_DC_MPEG4(slice->cBS_1, coeffMB);
                    // encode to dp_buff_2
                    mp4_EncodeMacroBlockIntra_AC_MPEG4(slice->cBS_2, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOL.reversible_vlc);
                } else {
                    // encode ac_pred_flag
                    slice->cBS.PutBit(ac_pred_flag);
                    mp4_EncodeCBPY_P(slice->cBS, IPPVC_MBTYPE_INTRA, pattern >> 2);
                    if (dquant != 0)
                        mp4_EncodeDquant(slice->cBS, dquant);
                    if (VOL.interlaced)
                        slice->cBS.PutBit(dct_type);
                    mp4_EncodeMacroBlockIntra_MPEG4(slice->cBS, coeffMB, pattern, nzCount, predDir, use_intra_dc_vlc, VOP.alternate_vertical_scan_flag);
                }
                if (ac_pred_flag)
                    mp4_RestoreIntraAC(MBcurr, coeffMB, predDir);
                ReconMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, quant, MBcurr, pattern1, dct_type);
            } else {
                // inter coded
                MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
                if (!MBcurr->mcsel) {
                if (!MBcurr->fieldmc) {
                    mvLuma4[0] = MBcurr->mv[0];  mvLuma4[1] = MBcurr->mv[1];  mvLuma4[2] = MBcurr->mv[2];  mvLuma4[3] = MBcurr->mv[3];
                    // find Luma Pred
                    if (VOL.obmc_disable) {
                        if (VOL.quarter_sample) {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                            } else
                                mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                        } else {
                            if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                                mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], rt);
                                mp4_Copy8x8HP_8u(pYf+8*mStepLuma, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], rt);
                                mp4_Copy8x8HP_8u(pYf+8*mStepLuma+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], rt);
                            } else
                                mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], rt);
                        }
                    } else {
                        IppMotionVector *mvRight, *mvLeft, *mvUpper;
                        mvRight = ((j == mNumMacroBlockPerRow - 1) ? &mvLuma4[1] : ((MBcurr[1].type == IPPVC_MBTYPE_INTRA || MBcurr[1].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[1].mcsel) ? &mvLuma4[1] : MBcurr[1].mv));
                        mvLeft = ((j == 0) ? (mvLuma4 - 1) : ((MBcurr[-1].type == IPPVC_MBTYPE_INTRA || MBcurr[-1].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[-1].mcsel) ? (mvLeft = mvLuma4 - 1) : MBcurr[-1].mv));
                        mvUpper = ((i == 0) ? (mvLuma4 - 2) : ((MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA || MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA_Q || MBcurr[-mNumMacroBlockPerRow].mcsel) ? (mvLuma4 - 2) : MBcurr[-mNumMacroBlockPerRow].mv));
                        if (VOL.quarter_sample) {
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                            ippiOBMC8x8QP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                        } else {
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf, mStepLuma, mcPred, 16, &mvLuma4[0], &mvLeft[1], &mvLuma4[1], &mvUpper[2], &mvLuma4[2], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+8, mStepLuma, mcPred+8, 16, &mvLuma4[1], &mvLuma4[0], &mvRight[0], &mvUpper[3], &mvLuma4[3], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &mvLuma4[2], &mvLeft[3], &mvLuma4[3], &mvLuma4[0], &mvLuma4[2], rt);
                            ippiOBMC8x8HP_MPEG4_8u_C1R(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &mvLuma4[3], &mvLuma4[2], &mvRight[2], &mvLuma4[1], &mvLuma4[3], rt);
                        }
                    }
                    // calculate Chroma MV
                    if (VOL.quarter_sample) {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                            mp4_ComputeChroma4MVQ(mvLuma4, &mvChroma);
                        else
                            mp4_ComputeChromaMVQ(mvLuma4, &mvChroma);
                    } else {
                        if (MBcurr->type == IPPVC_MBTYPE_INTER4V)
                            mp4_ComputeChroma4MV(mvLuma4, &mvChroma);
                        else
                            mp4_ComputeChromaMV(mvLuma4, &mvChroma);
                    }
                    // find Chroma Pred
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                } else {
                    mvLuma4[0] = mvLuma4[1] = MBcurr->mvT;  mvLuma4[2] = mvLuma4[3] = MBcurr->mvB;
                    // find Luma Pred
                    if (VOL.quarter_sample) {
                        mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                        mp4_Copy16x8QP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                    } else {
                        mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_ftfr, mStepLuma*2, mcPred, 32, &mvLuma4[0], rt);
                        mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_fbfr, mStepLuma*2, mcPred+16, 32, &mvLuma4[2], rt);
                    }
                    // calculate Chroma MV
                    if (VOL.quarter_sample) {
                        IppMotionVector mvTmpT, mvTmpB;
                        mvTmpT.dx = (Ipp16s)mp4_Div2(MBcurr->mvT.dx);
                        mvTmpT.dy = (Ipp16s)(mp4_Div2(MBcurr->mvT.dy << 1) >> 1);
                        mvTmpB.dx = (Ipp16s)mp4_Div2(MBcurr->mvB.dx);
                        mvTmpB.dy = (Ipp16s)(mp4_Div2(MBcurr->mvB.dy << 1) >> 1);
                        mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                        mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                    } else {
                        mp4_ComputeChromaMV(&MBcurr->mvT, &mvChromaT);
                        mp4_ComputeChromaMV(&MBcurr->mvB, &mvChromaB);
                    }
                    // find Chroma Pred
                    mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4, 16, &mvChromaT, rt);
                    mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5, 16, &mvChromaT, rt);
                    mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                    mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? mStepChroma : 0), mStepChroma*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                }
            } else {
                // apply warping to the macroblock
                ippiWarpLuma_MPEG4_8u_C1R(mFrameF->pY, mStepLuma, mcPred, 16, &mbRectL, mWarpSpec);
                ippiWarpChroma_MPEG4_8u_P2R(mFrameF->pU, mStepChroma, mFrameF->pV, mStepChroma, mcPred+64*4, 8, mcPred+64*5, 8, &mbRectC, mWarpSpec);
            }
            if (quant < VOP.vop_quant)
                MBcurr->not_coded = 0;
            if (MBcurr->not_coded) {
                pattern = 0;
            } else {
                pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, i, j, &dct_type, mMEflags & RD_TRELLIS, NULL);
            }
            if (dquant != 0)
                MBcurr->not_coded = 0;
            if (MBcurr->mcsel && pattern == 0 && MBcurr->not_coded) {
                // for B-frames notcoded mcsel MBs should be treated as coded
                MBcurr->not_coded = 0;
                // encode not_coded
                slice->cBS.PutBit(1);
                ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
            } else {
if (!MBcurr->mcsel) {
                        if (mInsertResync) {
                            // recompute MV differences which were computed in ME
                            Predict1MV(MBcurr, i - startRow, j, &MBcurr->mvDiff[0]);
                            if (!MBcurr->fieldmc) {
                                mp4_MV_GetDiff(&mvLuma4[0], &MBcurr->mvDiff[0], fRangeMin, fRangeMax, fRange);
                                if (MBcurr->type == IPPVC_MBTYPE_INTER4V) {
                                    Predict3MV(MBcurr, i - startRow, j, MBcurr->mvDiff, mvLuma4);
                                    mp4_MV_GetDiff(&mvLuma4[1], &MBcurr->mvDiff[1], fRangeMin, fRangeMax, fRange);
                                    mp4_MV_GetDiff(&mvLuma4[2], &MBcurr->mvDiff[2], fRangeMin, fRangeMax, fRange);
                                    mp4_MV_GetDiff(&mvLuma4[3], &MBcurr->mvDiff[3], fRangeMin, fRangeMax, fRange);
                                }
                            } else {
                                MBcurr->mvDiff[0].dy = (Ipp16s)mp4_Div2(MBcurr->mvDiff[0].dy);
                                MBcurr->mvDiff[2] = MBcurr->mvDiff[0];
                                mp4_MV_GetDiff(&mvLuma4[0], &MBcurr->mvDiff[0], fRangeMin, fRangeMax, fRange);
                                mp4_MV_GetDiff(&mvLuma4[2], &MBcurr->mvDiff[2], fRangeMin, fRangeMax, fRange);
                            }
                        }
}
                    // encode not_coded
                    slice->cBS.PutBit(0);
                    mp4_EncodeMCBPC_P(slice->cBS, dquant == 0 ? MBcurr->type : IPPVC_MBTYPE_INTER_Q, pattern & 3);
                    if (MBcurr->type < IPPVC_MBTYPE_INTER4V)
                        slice->cBS.PutBit(MBcurr->mcsel);
                    if (VOL.data_partitioned) {
                        if (!MBcurr->mcsel)
                            mp4_EncodeMV(slice->cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                        // encode to dp_buff_1
                        mp4_EncodeCBPY_P(slice->cBS_1, MBcurr->type, pattern >> 2);
                        if (dquant != 0)
                            mp4_EncodeDquant(slice->cBS_1, dquant);
                        // encode to dp_buff_2
                        mp4_EncodeMacroBlockInter_MPEG4(slice->cBS_2, coeffMB, pattern, nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
                    } else {
                        mp4_EncodeCBPY_P(slice->cBS, dquant == 0 ? MBcurr->type : IPPVC_MBTYPE_INTER_Q, pattern >> 2);
                        if (dquant != 0)
                            mp4_EncodeDquant(slice->cBS, dquant);
                        if (VOL.interlaced) {
                            // encode dct_type
                            if (pattern)
                                slice->cBS.PutBit(dct_type);
                            else
                                dct_type = 0;
                            // encode field_prediction
                            if (!MBcurr->mcsel) {
                                if (MBcurr->type != IPPVC_MBTYPE_INTER4V) {
                                    slice->cBS.PutBit(MBcurr->fieldmc);
                                    if (MBcurr->fieldmc) {
                                        slice->cBS.PutBit(mb_ftfr);
                                        slice->cBS.PutBit(mb_fbfr);
                                    }
                                }
                                mp4_EncodeMV(slice->cBS, &MBcurr->mvDiff[0]/*&mvPred[0]*/, VOP.vop_fcode_forward, MBcurr->type);
                                if (MBcurr->fieldmc)
                                    mp4_EncodeMV(slice->cBS, &MBcurr->mvDiff[2]/*&mvPred[2]*/, VOP.vop_fcode_forward, IPPVC_MBTYPE_INTER);
                            }
                        } else
                            if (!MBcurr->mcsel)
                                mp4_EncodeMV(slice->cBS, MBcurr->mvDiff/*mvPred*/, VOP.vop_fcode_forward, MBcurr->type);
                        mp4_EncodeMacroBlockInter_MPEG4(slice->cBS, coeffMB, pattern, nzCount, VOL.reversible_vlc, VOP.alternate_vertical_scan_flag);
                    }
                    if (pattern == 0)
                        ReconMacroBlockNotCoded(pYc, pUc, pVc, mcPred);
                    else
                        ReconMacroBlockInter_MPEG4(pYc, pUc, pVc, mcPred, coeffMB, quant, pattern, dct_type);
                }
            }
            if (mInsertResync) {
                mbn ++;
                mbnvp ++;
                nBits = slice->cBS.GetNumBits(sPtr, sBitOff);
                if (VOL.data_partitioned)
                    nBits += slice->cBS_1.GetNumBits() + slice->cBS_2.GetNumBits();
#ifdef VIDEOPACKETS_LE_MAX
                if (nBits > mVideoPacketLength) {
                    if (mbnvp > 1) {
                        slice->cBS.SetPos(cPtr, cBitOff);
                        if (VOL.data_partitioned) {
                            slice->cBS_1.SetPos(cPtr_1, cBitOff_1);
                            slice->cBS_2.SetPos(cPtr_2, cBitOff_2);
                        }
                        dquant = 0;
                        pYc -= 16; pUc -= 8; pVc -= 8;
                        pYf -= 16; pUf -= 8; pVf -= 8;
                        mbRectL.x -= 16;
                        mbRectC.x -= 8;
                        j --;
                        if (j < 0) {
                            j = mNumMacroBlockPerRow - 1;
                            i --;
                            mbRectL.y -= 16;
                            mbRectC.y -= 8;
                        }
                        mbnvp --;
                        mbn --;
                        MBcurr --;
                        vp_step_back = true;
                    }
#else
                if ((nBits >= mVideoPacketLength) && mbn < mNumMacroBlockPerVOP) {
#endif
                    // mark MBs in current VideoPacket as invalid for prediction
                    MBcurr->block[1].validPredIntra = 0;
                    for (Ipp32s i = 0; i < IPP_MIN(mbnvp, mNumMacroBlockPerRow + 1); i ++) {
                        MBcurr[-i].block[2].validPredIntra = MBcurr[-i].block[3].validPredIntra = 0;
                        MBcurr[-i].block[4].validPredIntra = MBcurr[-i].block[5].validPredIntra = 0;
                        MBcurr[-i].validPredInter = 0;
                    }
                    if (VOL.data_partitioned) {
                        mp4_EncodeMarkerMV(slice->cBS);
                        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
                    }
                    slice->cBS.GetPos(&sPtr, &sBitOff);
                    mp4_EncodeStuffingBitsAlign(slice->cBS);
                    EncodeVideoPacketHeader(slice->cBS, mbn, quant);
                    mbnvp = 0;
                }
#ifdef VIDEOPACKETS_LE_MAX
                else
                    vp_step_back = false;
                slice->cBS.GetPos(&cPtr, &cBitOff);
                if (VOL.data_partitioned) {
                    slice->cBS_1.GetPos(&cPtr_1, &cBitOff_1);
                    slice->cBS_2.GetPos(&cPtr_2, &cBitOff_2);
                }
#endif
            }
            if (!vp_step_back) {
                slice->quantSum += quant;
                if (mRateControl == 2) {
                    mRC_MB.Update(slice, &dquant, MP4_VOP_TYPE_S);
                    quant += dquant;
                }
            }
            pYc += 16; pUc += 8; pVc += 8;
            pYf += 16; pUf += 8; pVf += 8;
            MBcurr ++;
            mbRectL.x += 16;
            mbRectC.x += 8;
        }
        mbRectL.y += 16;
        mbRectC.y += 8;
    }
    if (VOL.data_partitioned) {
        mp4_EncodeMarkerMV(slice->cBS);
        mp4_MergeBuffersDP(slice->cBS, slice->cBS_1, slice->cBS_2);
    }
    mp4_EncodeStuffingBitsAlign(slice->cBS);
}

void VideoEncoderMPEG4::EncodeSVOP()
{
    if (VOL.sprite_enable != MP4_SPRITE_GMC) {
        mp4_EncodeStuffingBitsAlign(cBS);
        return;
    }
    IppiRect    vopRect;
    // init WarpSpec for GMC
    vopRect.x = 0;
    vopRect.y = 0;
    vopRect.width = VOL.video_object_layer_width;
    vopRect.height = VOL.video_object_layer_height;
    ippiWarpInit_MPEG4(mWarpSpec, VOP.warping_mv_code_du, VOP.warping_mv_code_dv,
        VOL.no_of_sprite_warping_points, MP4_SPRITE_GMC,
        VOL.sprite_warping_accuracy, VOP.vop_rounding_type, VOL.quarter_sample,
        VOP.vop_fcode_forward, &vopRect, &vopRect);
#ifdef _OMP_KARABAS
    if (mNumThreads >= 2) {
        if ((mRateControl != 2 && !VOL.data_partitioned && !mInsertResync) && (mThreadingAlg == 0 || (mThreadingAlg == 1 && !VOL.obmc_disable)) ) {
            Ipp32s  i;

            // slices in this ME are used only for calculating numIntraMB
            ME_VOP();
            if (mNumIntraMB > mSceneChangeThreshold)
                return;
            mQuantSum = VOP.vop_quant * mNumMacroBlockPerVOP;
            for (i = 0; i < mNumMacroBlockPerCol; i ++)
                mCurRowMT[i] = -1;
            i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
            {
                Ipp32s curRow = i;
                while (curRow < mNumMacroBlockPerCol) {
                    curRow = i;
                    i ++;
                    if (curRow < mNumMacroBlockPerCol)
                        EncodeSRow(curRow, mp4_MT_get_thread_num());
                    curRow ++;
                }
            }
            mp4_EncodeStuffingBitsAlign(cBS);
            AdjustSearchRange();
            return;
        }
        if (mThreadingAlg == 1 && VOL.obmc_disable) {
            Ipp32s  i;

            for (i = 0; i < mNumThreads; i ++)
                mSliceMT[i].quantSum = mSliceMT[i].numIntraMB = 0;
            mSliceMT[0].cBS = cBS;
#ifdef _OPENMP
#pragma  omp parallel num_threads(mNumThreads)
            {
                ME_Slice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                ME_Slice(mSliceMT + i);
#endif
            mNumIntraMB = mSliceMT[0].numIntraMB;
            for (i = 1; i < mNumThreads; i ++)
                mNumIntraMB += mSliceMT[i].numIntraMB;;
            if (mNumIntraMB > mSceneChangeThreshold)
                return;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads)
            {
                EncodeSSlice(mSliceMT + omp_get_thread_num());
            }
#else
            for (i = 0; i < mNumThreads; i ++)
                EncodeSSlice(mSliceMT + i);
#endif
            mQuantSum = mSliceMT[0].quantSum;
            cBS = mSliceMT[0].cBS;
            for (i = 1; i < mNumThreads; i ++) {
                // merge buffers
                ippsCopy_8u(mSliceMT[i].cBS.mBuffer, cBS.mPtr, mSliceMT[i].cBS.GetFullness());
                cBS.mPtr += mSliceMT[i].cBS.GetFullness();
                mSliceMT[i].cBS.Reset();
                mQuantSum += mSliceMT[i].quantSum;
            }
            AdjustSearchRange();
            return;
        }
    }
#endif // _OMP_KARABAS
    {
        mSlice.numIntraMB = 0;
        ME_Slice(&mSlice);
        mNumIntraMB = mSlice.numIntraMB;
        if (mNumIntraMB > mSceneChangeThreshold)
            return;
        mSlice.quantSum = 0;
        mSlice.cBS = cBS;
        EncodeSSlice(&mSlice);
        mQuantSum = mSlice.quantSum;
        cBS = mSlice.cBS;
    }
    AdjustSearchRange();
}

} // namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
