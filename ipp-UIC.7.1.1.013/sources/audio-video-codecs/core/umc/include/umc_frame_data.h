/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FRAME_DATA_H__
#define __UMC_FRAME_DATA_H__

#include "umc_structures.h"
#include "umc_dynamic_cast.h"
#include "umc_frame_allocator.h"
#include "umc_video_data.h"

#include "ippdefs.h"
#include "ippi.h"

namespace UMC
{

class FrameData
{
public:
    DYNAMIC_CAST_DECL_BASE(FrameData)

    struct PlaneMemoryInfo
    {
        Ipp8u* m_planePtr;
        size_t m_pitch;
    };

    FrameData();
    FrameData(const FrameData & fd);
    virtual ~FrameData();

    const VideoData * GetInfo() const;

    const PlaneMemoryInfo * GetPlaneMemoryInfo(Ipp32u plane) const;

    void Init(const VideoData* info, FrameMemID memID = FRAME_MID_INVALID, FrameAllocator * frameAlloc = 0);

    void Close();

    FrameMemID GetFrameMID() const;

    FrameMemID Release();

    void SetPlanePointer(Ipp8u* planePtr, Ipp32u plane, size_t pitch);

    FrameData& operator=(const FrameData& );

    bool            m_locked;

protected:
    VideoData       m_Info;
    FrameMemID      m_FrameMID;
    FrameAllocator *m_FrameAlloc;

    PlaneMemoryInfo m_PlaneInfo[MAX_COLOR_PLANES];
};

} // namespace UMC

#endif // __UMC_FRAME_DATA_H__
