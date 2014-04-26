/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_frame_data.h"

using namespace UMC;


FrameData::FrameData()
{
    m_locked     = false;
    m_FrameMID   = FRAME_MID_INVALID;
    m_FrameAlloc = 0;
}

FrameData::FrameData(const FrameData &fd)
{
    m_locked     = false;
    m_Info       = fd.m_Info;
    m_FrameMID   = fd.m_FrameMID;
    m_FrameAlloc = fd.m_FrameAlloc;

    memcpy(m_PlaneInfo, fd.m_PlaneInfo, sizeof(m_PlaneInfo));
    if (m_FrameAlloc)
        m_FrameAlloc->IncreaseReference(m_FrameMID);
}

FrameData::~FrameData()
{
    Close();
}

FrameData& FrameData::operator=(const FrameData& fd)
{
    Close();

    m_FrameAlloc = fd.m_FrameAlloc;
    m_FrameMID = fd.m_FrameMID;
    m_Info = fd.m_Info;
    memcpy(m_PlaneInfo, fd.m_PlaneInfo, sizeof(m_PlaneInfo));
    m_locked = false;// fd.m_locked;

    if (m_FrameAlloc)
    {
        m_FrameAlloc->IncreaseReference(m_FrameMID);
    }

    return *this;
}

const VideoData * FrameData::GetInfo() const
{
    return &m_Info;
}

FrameMemID FrameData::GetFrameMID() const
{
    return m_FrameMID;
}

FrameMemID FrameData::Release()
{
    FrameMemID mid = m_FrameMID;
    m_locked = false;
    m_FrameMID = FRAME_MID_INVALID;
    m_FrameAlloc = 0;
    m_Info.Close();
    return mid;
}

void FrameData::Init(const VideoData * info, FrameMemID memID, FrameAllocator * frameAlloc)
{
    Close();

    m_Info = *info;
    m_FrameMID = memID;
    m_FrameAlloc = frameAlloc;
    if (m_FrameAlloc && m_FrameMID != FRAME_MID_INVALID)
        m_FrameAlloc->IncreaseReference(m_FrameMID);
}

void FrameData::Close()
{
    if (m_FrameAlloc && m_FrameMID != FRAME_MID_INVALID)
    {
        if (m_locked)
        {
            m_FrameAlloc->Unlock(m_FrameMID);
        }

        m_locked = false;

        m_FrameAlloc->DecreaseReference(m_FrameMID);
        m_FrameMID = FRAME_MID_INVALID;
        m_FrameAlloc = 0;
    }

    memset(m_PlaneInfo, 0, sizeof(m_PlaneInfo));
    m_Info.Close();
}

const FrameData::PlaneMemoryInfo * FrameData::GetPlaneMemoryInfo(Ipp32u plane) const
{
    if (plane >= m_Info.GetPlanesNumber())
        return 0;

    return &(m_PlaneInfo[plane]);
}

void FrameData::SetPlanePointer(Ipp8u* planePtr, Ipp32u plane, size_t pitch)
{
    if (plane >= m_Info.GetPlanesNumber())
        return;

    m_PlaneInfo[plane].m_planePtr = planePtr;
    m_PlaneInfo[plane].m_pitch = pitch;
}
