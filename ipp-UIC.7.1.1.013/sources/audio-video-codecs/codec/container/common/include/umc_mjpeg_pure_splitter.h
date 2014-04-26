/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MJPEG_PURE_SPLITTER_H__
#define __UMC_MJPEG_PURE_SPLITTER_H__

#include "umc_structures.h"
#include "umc_splitter.h"
#include "umc_media_data.h"
#include "umc_index.h"
#include "umc_thread.h"
#include "umc_media_buffer.h"

namespace UMC
{

typedef enum
{
    JPEG_PROFILE_NDIFF_HUFF_BASELINE     = 10,
    JPEG_PROFILE_NDIFF_HUFF_EXTENDED     = 11,
    JPEG_PROFILE_NDIFF_HUFF_PROGRESSIVE  = 12,
    JPEG_PROFILE_NDIFF_HUFF_LOSSLESS     = 13,
    JPEG_PROFILE_DIFF_HUFF_EXTENDED      = 20,
    JPEG_PROFILE_DIFF_HUFF_PROGRESSIVE   = 21,
    JPEG_PROFILE_DIFF_HUFF_LOSSLESS      = 22,
    JPEG_PROFILE_NDIFF_ARITH_EXTENDED    = 30,
    JPEG_PROFILE_NDIFF_ARITH_PROGRESSIVE = 31,
    JPEG_PROFILE_NDIFF_ARITH_LOSSLESS    = 32,
    JPEG_PROFILE_DIFF_ARITH_EXTENDED     = 40,
    JPEG_PROFILE_DIFF_ARITH_PROGRESSIVE  = 41,
    JPEG_PROFILE_DIFF_ARITH_LOSSLESS     = 42,

} JPEG_PROFILE;


class MJPEGSplitter : public Splitter
{
public:
    DYNAMIC_CAST_DECL(MJPEGSplitter, Splitter)

    MJPEGSplitter();
    virtual ~MJPEGSplitter();

    virtual Status Init(SplitterParams *pParams);
    virtual Status Close();

    virtual Status GetNextData(MediaData *pDst, Ipp32u iTrack);
    virtual Status CheckNextData(MediaData *pDst, Ipp32u iTrack);

    virtual Status GetInfo(SplitterInfo **ppInfo) { *ppInfo = &m_info; return UMC_OK; };
    virtual Status SetTrackState(Ipp32u,TrackState) { return UMC_ERR_UNSUPPORTED; };

    virtual Status SetTimePosition(Ipp64f /*timePos*/) { return UMC_ERR_UNSUPPORTED; };
    virtual Status GetTimePosition(Ipp64f &) { return UMC_ERR_UNSUPPORTED; };

    virtual Status Run() {return UMC_OK;};
    virtual Status Stop() {return UMC_OK;};

    virtual Status SetPosition(Ipp64f /*fPosition*/) {return UMC_OK;}
    virtual Status GetPosition(Ipp64f &/*fPosition*/) {return UMC_OK;}

private:
    MediaData m_data;
    Ipp32u    m_iFrames;
    Ipp32u    m_iResilience;
};

}

#endif
