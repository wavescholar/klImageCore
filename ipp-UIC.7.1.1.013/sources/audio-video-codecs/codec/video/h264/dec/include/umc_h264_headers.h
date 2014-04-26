/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_HEADERS_H
#define __UMC_H264_HEADERS_H

#include "umc_h264_dec_defs_dec.h"
#include "umc_h264_heap.h"
#include "umc_h264_slice_decoding.h"
#include "umc_h264_frame_info.h"

namespace UMC
{

template <typename T>
class HeaderSet
{
public:

    HeaderSet(H264_Heap_Objects  *pObjHeap)
        : m_Header(pObjHeap)
        , m_AdditionalHeaderList(pObjHeap)
        , m_pObjHeap(pObjHeap)
        , m_currentID(-1)
    {
    }

    bool AddHeader(T* hdr, bool force)
    {
        Ipp32s id = hdr->GetID();
        if (force)
        {
            m_pObjHeap->FreeObject(m_Header[id]);
            m_Header[id] = 0;
        }

        m_currentID = id;

        if (!m_Header[id])
        {
            m_Header[id] = m_pObjHeap->AllocateObject((T*)0);
            *m_Header[id] = *hdr;
            return false;
        }

        T * tmp = m_AdditionalHeaderList.FindLastByPid(id);
        if (!tmp)
        {
            tmp = m_Header[id];
        }

        if (*tmp == *hdr)
        {
            return false;
        }
        else
        {
            // add to
            T* newItem = m_pObjHeap->AllocateObject((T*)0);
            *newItem = *hdr;
            m_AdditionalHeaderList.AddItem(newItem, id);
        }
        return true;
    }

    T * GetHeader(Ipp32s id)
    {
        T * tmp = m_AdditionalHeaderList.FindLastByPid(id);
        if (tmp)
        {
            return tmp;
        }

        return m_Header[id];
    }

    const T * GetHeader(Ipp32s id) const
    {
        T * tmp = m_AdditionalHeaderList.FindLastByPid(id);
        if (tmp)
        {
            return tmp;
        }

        return m_Header[id];
    }

    void RemoveHeader(void * hdr)
    {
        T * tmp = (T *)hdr;
        if (!tmp)
        {
            VM_ASSERT(false);
            return;
        }

        Ipp32s id = tmp->GetID();
        if (tmp == m_Header[id])
        {
            m_pObjHeap->FreeObject<T>(tmp);
            m_Header[id] = m_AdditionalHeaderList.DetachItemByPid(id);
            return;
        }

        m_AdditionalHeaderList.RemoveItem(tmp);
        m_pObjHeap->FreeObject<T>(tmp);
    }

   void Reset(bool isPartialReset = false)
    {
        if (!isPartialReset)
        {
            for (Ipp32u i = 0; i < m_Header.GetItemCount(); i++)
            {
                m_pObjHeap->FreeObject<T>(m_Header[i]);
            }

            m_Header.Reset();
            m_currentID = -1;
        }

        m_AdditionalHeaderList.Reset();
    }

    void SetCurrentID(Ipp32s id)
    {
        m_currentID = id;
    }

    Ipp32s GetCurrentID() const
    {
        return m_currentID;
    }

    T * GetCurrentHeader()
    {
        if (m_currentID == -1)
            return 0;

        return GetHeader(m_currentID);
    }

    const T * GetCurrentHeader() const
    {
        if (m_currentID == -1)
            return 0;

        return GetHeader(m_currentID);
    }

private:
    H264Array<T*>             m_Header;
    H264_List<T>              m_AdditionalHeaderList;
    H264_Heap_Objects        *m_pObjHeap;

    Ipp32s                    m_currentID;
};

/****************************************************************************************************/
// Headers stuff
/****************************************************************************************************/
class Headers
{
public:

    Headers(H264_Heap_Objects  *pObjHeap)
        : m_SeqParams(pObjHeap)
        , m_SeqExParams(pObjHeap)
        , m_PicParams(pObjHeap)
        , m_SEIParams(pObjHeap)
        , m_pObjHeap(pObjHeap)
    {
    }

    void Reset(bool isPartialReset = false)
    {
        m_SeqParams.Reset(isPartialReset);
        m_SeqExParams.Reset(isPartialReset);
        m_PicParams.Reset(isPartialReset);
        m_SEIParams.Reset(isPartialReset);
    }

    void Signal(Ipp32s type, void * hdr)
    {
        switch(type)
        {
        case 0:
            m_SeqParams.RemoveHeader(hdr);
            break;
        case 1:
            m_SeqExParams.RemoveHeader(hdr);
            break;
        case 2:
            m_PicParams.RemoveHeader(hdr);
            break;
        case 3:
            m_SEIParams.RemoveHeader(hdr);
            break;
        default:
            VM_ASSERT(false);
            break;
        };
    }

    HeaderSet<H264SeqParamSet>             m_SeqParams;
    HeaderSet<H264SeqParamSetExtension>    m_SeqExParams;
    HeaderSet<H264PicParamSet>             m_PicParams;
    HeaderSet<H264SEIPayLoad>              m_SEIParams;

private:
    H264_Heap_Objects  *m_pObjHeap;
};

} // namespace UMC

#endif // __UMC_H264_HEADERS_H
