/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_VIDEO_DATA_SCENE_INFO_H
#define __UMC_VIDEO_DATA_SCENE_INFO_H

#include "umc_video_data.h"
#include "umc_scene_info.h"

namespace UMC
{

// declare types of statistics
enum
{
    SA_FRAME_DATA               = 0,
    SA_SLICE_DATA               = 1,
    SA_MB_DATA                  = 2,

    // declare number of items in the enum
    SA_TYPES_OF_DATA
};

class VideoDataSceneInfo : public VideoData
{
public:
    DYNAMIC_CAST_DECL(VideoDataSceneInfo, VideoData);

    // Default constructor
    VideoDataSceneInfo(void);
    // Destructor
    virtual
    ~VideoDataSceneInfo(void);

    // Set the pointer to array of data
    inline
    void SetAnalysisData(const UMC_SCENE_INFO *pData, Ipp32u dataType);
    // Get the pointer to array of data
    inline
    const UMC_SCENE_INFO *GetAnalysisData(Ipp32u dataType);

protected:
    const UMC_SCENE_INFO *(m_pData[SA_TYPES_OF_DATA]);
};

inline
void VideoDataSceneInfo::SetAnalysisData(const UMC_SCENE_INFO *pData, Ipp32u dataType)
{
    // check error(s)
    if (SA_TYPES_OF_DATA <= dataType)
        return;

    m_pData[dataType] = pData;

} // void VideoDataSceneInfo::SetAnalysisData(const UMC_SCENE_INFO *pData, Ipp32u dataType)

inline
const UMC_SCENE_INFO *VideoDataSceneInfo::GetAnalysisData(Ipp32u dataType)
{
    // check error(s)
    if (SA_TYPES_OF_DATA <= dataType)
        return (UMC_SCENE_INFO *) 0;

    // return the pointer
    return m_pData[dataType];

} // const UMC_SCENE_INFO *VideoDataSceneInfo::GetAnalysisData(Ipp32u dataType)

} // namespace UMC

#endif // __UMC_VIDEO_DATA_SCENE_INFO_H
