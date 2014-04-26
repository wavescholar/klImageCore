/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_SCENE_ANALYZER_FRAME_H
#define __UMC_SCENE_ANALYZER_FRAME_H

#include "umc_video_data.h"
#include "umc_scene_info.h"

namespace UMC
{

class SceneAnalyzerPicture
{
public:
    // Default constructor
    SceneAnalyzerPicture(void);
    // Destructor
    virtual
    ~SceneAnalyzerPicture(void);

    // Initialize the picture
    Status Init(Ipp32s srcWidth, Ipp32s srcHeight,
                ColorFormat colorFormat);
    // Initialize the picture with given pointer.
    // This method is supposed to use only with GRAY pictures.
    Status SetPointer(const Ipp8u *pPic, size_t picStep,
                      Ipp32s srcWidth, Ipp32s srcHeight);

    const Ipp8u *(m_pPic[3]);                                   // (const Ipp8u *([])) pointer to the picture's buffer
    size_t m_picSize;                                           // (size_t) size of allocated picture
    size_t m_picStep;                                           // (Ipp32s) picture's buffer's step
    IppiSize m_picDim;                                          // (IppiSize) picture's dimensions
    IppiSize m_mbDim;                                           // (IppiSize) picture's dimensions in units of MB
    ColorFormat m_colorFormat;
    bool m_bChangeDetected;                                     // (bool) a scene change was detected

    Ipp16u *m_pSadBuffer;                                       // (Ipp16u *) temporal buffer for SADs

    UMC_SCENE_INFO m_info;                                      // (UMC_SCENE_INFO) entire frame statistics
    UMC_SCENE_INFO *m_pSliceInfo;                               // (UMC_SCENE_INFO *) pointer to array of average slice statistics
    size_t m_numSliceInfo;

protected:
    // Release the object
    void Close(void);

    // Allocate buffers for being analyzed frame
    Status AllocateBuffer(size_t bufSize);
    Status AllocateSliceInfo(size_t numSliceInfo);
    // Release the buffers
    void ReleaseBuffer(void);
    void ReleaseSliceInfo(void);

};

class SceneAnalyzerFrame : public SceneAnalyzerPicture
{
public:
    // Default constructor
    SceneAnalyzerFrame(void);
    // Destructor
    virtual
    ~SceneAnalyzerFrame(void);

    // Initialize the frame
    Status SetSource(VideoData *pSrc, PictureStructure interlaceType);

    SceneAnalyzerPicture m_scaledPic;
    SceneAnalyzerPicture m_fields[2];

    FrameType m_frameType;                                      // (FrameType) suggested frame type

    // The following variables have a big difference in meaning.
    // frameStructure is a real type of the current frame. PROGRESSIVE means
    // that it is a real progressive frame, others mean it is a couple of
    // fields. Fielded pictures are estimated as fields always. A PROGRESSIVE
    // frame should be estimated as a field couple, if the reference frame is
    // a field couple, and as a progressive frame, if the reference frame is a
    // progressive frame too. The type of the reference frame is determined by
    // its frameStructure variable. The frameEstimation variables means the
    // type of estimation of the current PROGRESSIVE by nature frame.
    PictureStructure m_frameStructure;                             // frame structure
    PictureStructure m_frameEstimation;                            // frame estimation type
};

inline
bool CheckSupportedColorFormat(VideoData *pSrc)
{
    ColorFormat colorFormat;

    // check color format
    colorFormat = pSrc->m_colorFormat;
    if ((YV12 != colorFormat) &&
        (NV12 != colorFormat) &&
        (YUV420 != colorFormat) &&
        (YUV422 != colorFormat) &&
        (YUV444 != colorFormat))
    {
        return false;
    }

    return true;

} // bool CheckSupportedColorFormat(VideoData *pSrc)

} // namespace UMC

#endif // __UMC_SCENE_ANALYZER_FRAME_H
