/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_WAVE_PURE_SPLITTER_H__
#define __UMC_WAVE_PURE_SPLITTER_H__

#include "umc_media_data.h"
#include "umc_data_reader.h"
#include "umc_structures.h"
#include "umc_splitter.h"

namespace UMC
{

class WAVESplitter : public Splitter
{
public:
    DYNAMIC_CAST_DECL(WAVESplitter, Splitter)

    WAVESplitter();
    virtual ~WAVESplitter();

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
    Ipp32u    m_iBufferSize;
    Ipp32u    m_iResilience;
};

}
#endif
