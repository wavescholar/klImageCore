/*
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//       Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <memory>

#include "umc_default_frame_allocator.h"
#include "umc_frame_data.h"
#include "umc_automatic_mutex.h"

namespace UMC
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  FrameInformation class implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FrameInformation
{
public:
    FrameInformation()
    {
        m_locks            = 0;
        m_referenceCounter = 0;
        m_ptr              = 0;
        m_type             = 0;
    }

    virtual ~FrameInformation()
    {
        delete[] m_ptr;
    }

    void Reset()
    {
        m_referenceCounter = 0;
        m_locks            = 0;
    }

    Ipp32s GetType() const { return m_type; }

    size_t CalculateSize(size_t pitch)
    {
        size_t sz = 0;
        size_t planePitch;

        const VideoData *info = m_frame.GetInfo();
        VideoData::PlaneData *pPlaneInfo;

        // set correct width & height to planes
        for (Ipp32u i = 0; i < info->GetPlanesNumber(); i += 1)
        {
            pPlaneInfo = info->GetPtrToPlane(i);
            planePitch = (pitch * pPlaneInfo->m_iSamples * pPlaneInfo->m_iSampleSize + (pPlaneInfo->m_iWidthDiv - 1))/pPlaneInfo->m_iWidthDiv;
            sz += planePitch * pPlaneInfo->m_size.height;
        }

        return sz + 128;
    }

    void ApplyMemory(Ipp8u * ptr, size_t pitch)
    {
        const VideoData * info = m_frame.GetInfo();
        VideoData::PlaneData *pPlaneInfo;
        size_t offset = 0;
        size_t planePitch;

        Ipp8u* ptr1 = align_pointer<Ipp8u*>(ptr, 64);
        offset = ptr1 - ptr;

        // set correct width & height to planes
        for (Ipp32u i = 0; i < info->GetPlanesNumber(); i += 1)
        {
            pPlaneInfo = info->GetPtrToPlane(i);
            planePitch = (pitch * pPlaneInfo->m_iSamples * pPlaneInfo->m_iSampleSize + (pPlaneInfo->m_iWidthDiv - 1))/pPlaneInfo->m_iWidthDiv;
            m_frame.SetPlanePointer(ptr + offset, i, planePitch);
            offset += (planePitch) * pPlaneInfo->m_size.height;
        }
    }

private:
    Ipp32s m_locks;
    Ipp32s m_referenceCounter;
    FrameData  m_frame;
    Ipp8u*  m_ptr;
    Ipp32s  m_type;
    Ipp32s  m_flags;

    friend class DefaultFrameAllocator;

private:
    // Declare private copy constructor to avoid accidental assignment
    // and klocwork complaining.
    FrameInformation(const FrameInformation &);
    FrameInformation & operator = (const FrameInformation &);
};
}

using namespace UMC;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  DefaultFrameAllocator class implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DefaultFrameAllocator::DefaultFrameAllocator()
{
    Init(0);
}

DefaultFrameAllocator::~DefaultFrameAllocator()
{
    Close();
}

Status DefaultFrameAllocator::Init(FrameAllocatorParams* /*pParams*/)
{
    AutomaticMutex guard(m_guard);

    Close();
    return UMC_OK;
}

Status DefaultFrameAllocator::Close()
{
    AutomaticMutex guard(m_guard);

    std::vector<FrameInformation *>::iterator iter = m_frames.begin();
    for (; iter != m_frames.end(); ++iter)
    {
        FrameInformation * info = *iter;
        delete info;
    }

    m_frames.clear();

    return UMC_OK;
}

Status DefaultFrameAllocator::Reset()
{
    AutomaticMutex guard(m_guard);

    std::vector<FrameInformation *>::iterator iter = m_frames.begin();
    for (; iter != m_frames.end(); ++iter)
    {
        FrameInformation * info = *iter;
        info->Reset();
    }

    return UMC_OK;
}

Status DefaultFrameAllocator::Alloc(FrameMemID *pNewMemID, const VideoData* frameInfo, Ipp32u flags)
{
    FrameMemID idx = 0;
    if (!pNewMemID || !frameInfo)
        return UMC_ERR_NULL_PTR;
    const VideoData *pCurrentInfo;
    size_t iPitch, iSize;

    AutomaticMutex guard(m_guard);

    std::vector<FrameInformation *>::iterator iter = m_frames.begin();
    for (; iter != m_frames.end(); ++iter)
    {
        FrameInformation * info = *iter;
        if (!info->m_referenceCounter)
        {
            info->m_referenceCounter++;
            *pNewMemID = idx;
            pCurrentInfo = info->m_frame.GetInfo();

            // Reinit frame if current data format has been changed
            if(frameInfo->m_iHeight != pCurrentInfo->m_iHeight || frameInfo->m_iWidth != pCurrentInfo->m_iWidth ||
                frameInfo->m_colorFormat != pCurrentInfo->m_colorFormat || frameInfo->GetMaxSampleSize() != pCurrentInfo->GetMaxSampleSize())
            {
                delete[] info->m_ptr;
                info->m_frame.Init(frameInfo, idx, this);

                iPitch = align_value<size_t>(frameInfo->m_iWidth, 64);
                iSize  = info->CalculateSize(iPitch);

                info->m_ptr = new Ipp8u[iSize];
                info->m_flags = flags;
                info->ApplyMemory(info->m_ptr, iPitch);
                info->m_referenceCounter = 1;
            }
            return UMC_OK;
        }
        idx++;
    }

    std::auto_ptr<FrameInformation> frameMID(new FrameInformation());

    frameMID->m_frame.Init(frameInfo, idx, this);

    iPitch = align_value<size_t>(frameInfo->m_iWidth, 64);
    iSize = frameMID->CalculateSize(iPitch);

    frameMID->m_ptr = new Ipp8u[iSize];
    frameMID->m_flags = flags;

    Ipp8u *ptr = frameMID->m_ptr;
    frameMID->ApplyMemory(ptr, iPitch);

    frameMID->m_referenceCounter = 1;

    FrameInformation *p = frameMID.release();
    m_frames.push_back(p);

    *pNewMemID = (FrameMemID)m_frames.size() - 1;

    return UMC_OK;
}

const FrameData* DefaultFrameAllocator::Lock(FrameMemID mid)
{
    AutomaticMutex guard(m_guard);
    if (mid >= (FrameMemID)m_frames.size())
        return NULL;

    FrameInformation * frameMID = m_frames[mid];
    frameMID->m_locks++;
    return &frameMID->m_frame;
}

Status DefaultFrameAllocator::Unlock(FrameMemID mid)
{
    AutomaticMutex guard(m_guard);
    if (mid >= (FrameMemID)m_frames.size())
        return UMC_ERR_FAILED;

    FrameInformation * frameMID = m_frames[mid];
    frameMID->m_locks--;
    return UMC_OK;
}

Status DefaultFrameAllocator::IncreaseReference(FrameMemID mid)
{
    AutomaticMutex guard(m_guard);
    if (mid >= (FrameMemID)m_frames.size())
        return UMC_ERR_FAILED;

     FrameInformation * frameMID = m_frames[mid];

    frameMID->m_referenceCounter++;

    return UMC_OK;
}

Status DefaultFrameAllocator::DecreaseReference(FrameMemID mid)
{
    AutomaticMutex guard(m_guard);

    if (mid >= (FrameMemID)m_frames.size())
        return UMC_ERR_FAILED;

    FrameInformation * frameMID = m_frames[mid];

    frameMID->m_referenceCounter--;
    if (frameMID->m_referenceCounter == 1)
    {
        if (frameMID->m_locks)
        {
            frameMID->m_referenceCounter++;
            return UMC_ERR_FAILED;
        }

        return Free(mid);
    }

    return UMC_OK;
}

Status DefaultFrameAllocator::Free(FrameMemID mid)
{
    if (mid >= (FrameMemID)m_frames.size())
        return UMC_ERR_FAILED;

    FrameInformation * frameMID = m_frames[mid];
    //delete frameMID;
    frameMID->m_referenceCounter = 0;
    return UMC_OK;
}
