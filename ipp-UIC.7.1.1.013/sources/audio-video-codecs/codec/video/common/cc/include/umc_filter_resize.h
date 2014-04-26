/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILTER_RESIZE_H__
#define __UMC_FILTER_RESIZE_H__

#include "umc_base_codec.h"

#include "ippdefs.h"
#include "ippi.h"

namespace UMC
{

class VideoResizeParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoResizeParams, BaseCodecParams)

    VideoResizeParams(void)
    {
        m_interpolationType  = IPPI_INTER_NN;
        m_borderType         = ippBorderRepl;
        m_fCubB              = 1;
        m_fCubC              = 0;
        m_iLacLobs           = 2; // 2, 3 or 4
        m_iAntialiasing      = 0;
        m_iFillValue         = 0;
    }

    Ipp32u                m_iFillValue;
    Ipp32u                m_iAntialiasing;
    Ipp32u                m_iLacLobs;
    Ipp32f                m_fCubB;
    Ipp32f                m_fCubC;
    IppiBorderType        m_borderType;
    Ipp32s                m_interpolationType;
};

class VideoResize : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoResize, BaseCodec)

    VideoResize();
    virtual ~VideoResize();

    // Initialize codec with specified parameter(s)
    virtual Status Init(BaseCodecParams*);

    // Convert frame
    virtual Status GetFrame(MediaData *in, MediaData *out);

    // Get codec working (initialization) parameter(s)
    virtual Status GetInfo(BaseCodecParams*) { return UMC_OK; };

    // Close all codec resources
    virtual Status Close(void);

    // Set codec to initial state
    virtual Status Reset(void) { return UMC_OK; };

protected:
    Status InitInternal(MediaData *input, MediaData *output);
    Status CloseInternal();

    struct PlaneParams
    {
        IppiRect   rectSrc;
        IppiRect   rectDst;
        Ipp8u     *pIntBuffer;
        Ipp8u     *pExtBuffer;
        IppiPoint  dstOffset;
        IppiPoint  srcOffset;
    };

    VideoResizeParams m_params;
    PlaneParams      *m_plane;
    IppiSize          m_srcSize;
    IppiSize          m_dstSize;
    Ipp32u            m_iFunctions;
    Ipp32u            m_iPlanes;
    Ipp8u             m_iFillValue8;
    Ipp16u            m_iFillValue16;
    Ipp64f            m_fXRatio;
    Ipp64f            m_fYRatio;
    bool              m_bUseIntermediate;
    bool              m_bStateInitialized;
    bool              m_bAllocated;

    MediaData    *m_pInterData;
};

} // namespace UMC

#endif /* __UMC_FILTER_RESIZE_H__ */
