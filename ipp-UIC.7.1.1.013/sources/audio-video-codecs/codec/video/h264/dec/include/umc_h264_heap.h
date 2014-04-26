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

#ifndef __UMC_H264_HEAP_H
#define __UMC_H264_HEAP_H

#include <memory>
#include "umc_mutex.h"
#include "umc_h264_dec_defs_dec.h"
#include "umc_media_data.h"

#include "ipps.h"

namespace UMC
{

//*********************************************************************************************/
//
//*********************************************************************************************/
class H264MemoryPiece
{
    friend class H264_Heap;

public:
    // Default constructor
    H264MemoryPiece()
    {
        m_pNext = 0;
        Reset();
    }

    // Destructor
    ~H264MemoryPiece()
    {
        Release();
    }

    static H264MemoryPiece * AllocateMemoryPiece(size_t nSize)
    {
        Ipp8u *ppp = (Ipp8u*)ippMalloc((int)(nSize + sizeof(H264MemoryPiece)));
        if (!ppp)
            throw h264_exception(UMC_ERR_ALLOC);
        H264MemoryPiece *piece = new(ppp) H264MemoryPiece();

        piece->m_pSourceBuffer = (Ipp8u*)ppp + sizeof(H264MemoryPiece);
        piece->m_pDataPointer = piece->m_pSourceBuffer;
        piece->m_nSourceSize = nSize;
        return piece;
    }

    static void FreeMemoryPiece(H264MemoryPiece * piece)
    {
        ippFree(piece);
    }

    void SetData(MediaData *out)
    {
        Release();

        m_pDataPointer = (Ipp8u*)out->GetDataPointer();
        m_nDataSize = out->GetDataSize();
        m_pts = out->m_fPTSStart;
    }

    // Allocate memory piece
    bool Allocate(size_t nSize)
    {
        Release();

        // DEBUG : ADB : need to use ExternalMemoryAllocator
        // allocate little more
        m_pSourceBuffer = h264_new_array_throw<Ipp8u>((Ipp32s)nSize);
        m_pDataPointer = m_pSourceBuffer;
        m_nSourceSize = nSize;
        return true;
    }

    // Get next element
    H264MemoryPiece *GetNext(){return m_pNext;}
    // Obtain data pointer
    Ipp8u *GetPointer(){return m_pDataPointer;}

    size_t GetSize() const {return m_nSourceSize;}

    size_t GetDataSize() const {return m_nDataSize;}
    void SetDataSize(size_t dataSize) {m_nDataSize = dataSize;}

    Ipp64f GetTime() const {return m_pts;}
    void SetTime(Ipp64f pts) {m_pts = pts;}

protected:
    Ipp8u *m_pSourceBuffer;                                     // (Ipp8u *) pointer to source memory
    Ipp8u *m_pDataPointer;                                      // (Ipp8u *) pointer to source memory
    size_t m_nSourceSize;                                       // (size_t) allocated memory size
    size_t m_nDataSize;                                         // (size_t) data memory size
    H264MemoryPiece *m_pNext;                                   // (H264MemoryPiece *) pointer to next memory piece
    Ipp64f   m_pts;

    void Reset()
    {
        m_pSourceBuffer = 0;
        m_pDataPointer = 0;
        m_nSourceSize = 0;
        m_nDataSize = 0;
    }

    void Release()
    {
        if (m_pSourceBuffer)
            delete[] m_pSourceBuffer;
        Reset();
    }
private:
    H264MemoryPiece(const H264MemoryPiece&) {} // Empty copy constructor
    const H264MemoryPiece& operator=(const H264MemoryPiece&) { return *this; }
};

//*********************************************************************************************/
//
//*********************************************************************************************/
//#define CONST_HEAP_DEFINE

class H264_Heap
{
public:

    H264_Heap()
        : m_pFilledMemory(0)
        , m_pFreeMemory(0)
    {
    }

    virtual ~H264_Heap()
    {
        while (m_pFreeMemory)
        {
            H264MemoryPiece *pMem = m_pFreeMemory->m_pNext;
            H264MemoryPiece::FreeMemoryPiece(m_pFreeMemory);
            m_pFreeMemory = pMem;
        }

        while (m_pFilledMemory)
        {
            H264MemoryPiece *pMem = m_pFilledMemory->m_pNext;
            H264MemoryPiece::FreeMemoryPiece(m_pFilledMemory);
            m_pFilledMemory = pMem;
        }
    }

    const H264MemoryPiece * GetLastAllocated() const
    {
        return m_pFilledMemory;
    }

    H264MemoryPiece * Allocate(size_t nSize)
    {
#ifndef CONST_HEAP_DEFINE
        H264MemoryPiece *pMem = H264MemoryPiece::AllocateMemoryPiece(nSize);
#else
        H264MemoryPiece *pMem = GetFreeMemoryPiece(nSize);
        AddFilledMemoryPiece(pMem);
#endif
        return pMem;
    }

    void Free(H264MemoryPiece *pMem)
    {
#ifndef CONST_HEAP_DEFINE
        H264MemoryPiece::FreeMemoryPiece(pMem);
#else
        if (!pMem)
            return;

        //m_pFilledMemory = m_pFilledMemory->m_pNext;
        H264MemoryPiece *pTemp = m_pFilledMemory;

        if (pTemp == pMem)
        {
            m_pFilledMemory = m_pFilledMemory->m_pNext;
        }
        else
        {
#ifdef _DEBUG
            bool is_found = false;
#endif
            // find pMem at list
            while (pTemp->m_pNext)
            {
                 if (pTemp->m_pNext == pMem)
                 {
                     pTemp->m_pNext = pMem->m_pNext;
#ifdef _DEBUG
                    is_found = true;
#endif
                     break;
                 }
                 pTemp = pTemp->m_pNext;
            }

#ifdef _DEBUG
            VM_ASSERT(is_found);
#endif
        }

        pMem->m_pNext = m_pFreeMemory;
        m_pFreeMemory = pMem;
#endif
    }

    void Reset()
    {
        while (m_pFilledMemory)
        {
            H264MemoryPiece *pMem = m_pFilledMemory;
            m_pFilledMemory = pMem->GetNext();
            pMem->m_pNext = m_pFreeMemory;
            m_pFreeMemory = pMem;
        }
    }

protected:

    // Get free piece of memory
    H264MemoryPiece *GetFreeMemoryPiece(size_t nSize)
    {
#ifndef CONST_HEAP_DEFINE
        H264MemoryPiece *pTemp = h264_new_throw<H264MemoryPiece>();
        pTemp->m_pNext = 0;
        pTemp->Allocate(nSize);
        return pTemp;
#else
        // try to find suitable already allocated memory
        H264MemoryPiece *pTemp = m_pFreeMemory;
        H264MemoryPiece *pPrev = 0;

        H264MemoryPiece *pGoodPiece = 0;
        size_t currMin = (size_t)-1;
        bool found = false;

        while (pTemp)
        {
            if (pTemp->m_nSourceSize >= nSize && currMin > (pTemp->m_nSourceSize - nSize))
            {
                pGoodPiece = pPrev;
                currMin = pTemp->m_nSourceSize;
                found = true;
            }

            pPrev = pTemp;
            pTemp = pTemp->m_pNext;
        }

        if (found)
        {
            pTemp = pGoodPiece ? pGoodPiece->m_pNext : m_pFreeMemory;

            if (pGoodPiece)
                pGoodPiece->m_pNext = pGoodPiece->m_pNext->m_pNext;
            else
                m_pFreeMemory = m_pFreeMemory->m_pNext;

            return pTemp;
        }

        // we found nothing, try to allocate new
        {
            pTemp = H264MemoryPiece::AllocateMemoryPiece(nSize * 2);
            // bind to list to avoid memory leak
            pTemp->m_pNext = m_pFreeMemory;
            m_pFreeMemory = pTemp;
            // unbind from list
            m_pFreeMemory = m_pFreeMemory->m_pNext;

            return pTemp;
        }
#endif
    }

    // Add filled piece of memory
    void AddFilledMemoryPiece(H264MemoryPiece *pMem)
    {
        if (m_pFilledMemory)
        {
            H264MemoryPiece *pTemp = m_pFilledMemory;

            // find end of list
            while (pTemp->m_pNext)
                pTemp = pTemp->m_pNext;
            pTemp->m_pNext = pMem;
        }
        else
            m_pFilledMemory = pMem;

        pMem->m_pNext = 0;

    }

protected:
    H264MemoryPiece * m_pFilledMemory;
    H264MemoryPiece * m_pFreeMemory;
};

class H264_Heap_Objects
{
public:
    H264_Heap_Objects() {}
    virtual ~H264_Heap_Objects() {}

    template <typename T>
    T* Allocate(T*, Ipp32s iSize = sizeof(T))
    {
         return (T*)ippMalloc(iSize);
    }

    template <typename T>
    T* AllocateObject(T *t)
    {
        void *ptr = Allocate(t);
        return new(ptr) T();
    }

    template <typename T>
    void FreeObject(T *obj)
    {
        if (!obj)
            return;
        obj->~T();
        Free(obj);
    }

    void Free(void *obj)
    {
        if (!obj)
            return;
        ippFree(obj);
    }
};


//*********************************************************************************************/
//
//*********************************************************************************************/
template <typename T>
class H264_List
{
public:
    class Item
    {
    public:
        Item(T *item, Ipp32s pid)
            : m_pNext(0)
            , m_Item(item)
            , m_pid(pid)
        {
        }

        Item * m_pNext;
        T *m_Item;
        Ipp32s m_pid;
    };

    H264_List(H264_Heap_Objects * pObjHeap)
        : m_pHead(0)
        , m_pObjHeap(pObjHeap)
    {
    }

    ~H264_List()
    {
        Reset();
    }

    void RemoveHead()
    {
        Item * tmp = m_pHead;
        m_pHead = m_pHead->m_pNext;
        m_pObjHeap->FreeObject(tmp);
    }

    void RemoveItem(T * item)
    {
        if (!m_pHead)
        {
            VM_ASSERT(false);
            return;
        }

        Item *tmp = m_pHead;

        if (tmp->m_Item == item)
        {
            m_pHead = m_pHead->m_pNext;
            m_pObjHeap->FreeObject(tmp);
            return;
        }

        while (tmp->m_pNext)
        {
            if (tmp->m_pNext->m_Item == item)
            {
                Item * list_item = tmp->m_pNext;
                tmp->m_pNext = tmp->m_pNext->m_pNext;
                m_pObjHeap->FreeObject(list_item);
                return;
            }

            tmp = tmp->m_pNext;
        }

        VM_ASSERT(false);
    }

    T * DetachItemByPid(Ipp32s pid)
    {
        if (!m_pHead)
        {
            return 0;
        }

        T * item = 0;
        Item *tmp = m_pHead;
        for (; tmp; tmp = tmp->m_pNext)
        {
            if (tmp->m_pid == pid)
            {
                item = tmp->m_Item;
                break;
            }
        }

        if (!tmp)
            return 0;

        tmp = m_pHead;

        if (tmp->m_Item == item)
        {
            m_pHead = m_pHead->m_pNext;
            m_pObjHeap->FreeObject(tmp);
            return item;
        }

        while (tmp->m_pNext)
        {
            if (tmp->m_pNext->m_Item == item)
            {
                Item * list_item = tmp->m_pNext;
                tmp->m_pNext = tmp->m_pNext->m_pNext;
                m_pObjHeap->FreeObject(list_item);
                return item;
            }

            tmp = tmp->m_pNext;
        }

        VM_ASSERT(false);
        return 0;
    }

    T* FindByPid(Ipp32s pid)
    {
        for (Item *tmp = m_pHead; tmp; tmp = tmp->m_pNext)
        {
            if (tmp->m_pid == pid)
                return tmp->m_Item;
        }

        return 0;
    }

    T* FindLastByPid(Ipp32s pid) const
    {
        T *last = 0;
        for (Item *tmp = m_pHead; tmp; tmp = tmp->m_pNext)
        {
            if (tmp->m_pid == pid)
                last = tmp->m_Item;
        }

        return last;
    }

    void AddItem(T *item, Ipp32s pid)
    {
        Item * buf = m_pObjHeap->Allocate((Item*)0);
        Item *newItem = new(buf) Item(item, pid);

        Item *tmp = m_pHead;
        if (m_pHead)
        {
            while (tmp->m_pNext)
            {
                tmp = tmp->m_pNext;
            }

            tmp->m_pNext = newItem;
        }
        else
        {
            m_pHead = newItem;
        }
    }

    T * GetHead()
    {
        return m_pHead->m_Item;
    }

    const T * GetHead() const
    {
        return m_pHead->m_Item;
    }

    void Reset()
    {
        for (Item *tmp = m_pHead; tmp; )
        {
            Item *tmp1 = tmp;
            tmp = tmp->m_pNext;
            m_pObjHeap->FreeObject(tmp1);
        }

        m_pHead = 0;
    }

private:
    Item * m_pHead;
    H264_Heap_Objects * m_pObjHeap;
};

// Declaration of thread-unsafe array class of simple types
template<class T>
class H264Array
{
public:
    // Default constructor
    H264Array(H264_Heap_Objects * pObjHeap)
        : m_pObjHeap(pObjHeap)
    {
        m_Null = 0;
        m_pArray = 0;
        m_nItemCount = 0;
    }

    // Destructor
    virtual ~H264Array(void)
    {
        Release();
    }

    // Initialize array
    bool Init(size_t nInitialCount)
    {
        if (false == Reallocate(nInitialCount))
            return false;

        return true;
    }

    // Index operator
    T &operator [] (Ipp32s iIndex)
    {
        // need to increase array size
        if (0 > iIndex ||
            ((size_t) iIndex >= m_nItemCount &&
             false == Reallocate(iIndex)))
        {
            return m_Null;
        }

        return m_pArray[iIndex];
    }

    const T &operator [] (Ipp32s iIndex) const
    {
        // need to increase array size
        if (0 > iIndex ||
            (size_t) iIndex >= m_nItemCount)
        {
            return m_Null;
        }

        return m_pArray[iIndex];
    }

    size_t GetItemCount(void) const
    {
        return m_nItemCount;

    }

    // Reset object to initial state
    virtual
    void Reset(void)
    {
        if (m_pArray)
            ippsSet_8u(0, (Ipp8u*)m_pArray, (Ipp32u)(sizeof(T)*m_nItemCount));
    }

    // Release object
    void Release(void)
    {
        if (m_pArray)
        {
            // we need to reset array before destruction
            Reset();

            m_pObjHeap->Free(m_pArray);
            m_pArray = NULL;
            m_nItemCount = 0;
        }
    }

protected:

    // Increase array size
    virtual
    bool Reallocate(size_t nCount)
    {
        T *pTemp;
        size_t nAllocated;

        if (m_nItemCount > nCount)
            return true;

        // allocate a little bit more
        nAllocated = IPP_MAX(nCount * 2, 4);
        pTemp = m_pObjHeap->Allocate((T*)0, (Ipp32s)(nAllocated*sizeof(T*)));

        // save array values
        if (m_pArray)
        {
            ippsCopy_8u((Ipp8u*)m_pArray, (Ipp8u*)pTemp, (Ipp32u)(sizeof(T) * m_nItemCount));
            m_pObjHeap->Free(m_pArray);
        }

        // reset rest values
        ippsSet_8u(0, (Ipp8u*)(pTemp + m_nItemCount), (Ipp32u)(sizeof(T) * (nAllocated - m_nItemCount)));

        // save values
        m_pArray = pTemp;
        m_nItemCount = nAllocated;
        return true;
    }

    T m_Null;                                                   // (T) NULL element
    T *m_pArray;                                                // (T *) pointer to array of items
    size_t m_nItemCount;                                        // (size_t) number of items in array
    H264_Heap_Objects * m_pObjHeap;
};

// Declaration of thread-unsafe array class of complex types types (like pointers)
template<class T>
class H264StructArray : public H264Array<T *>
{
public:
    // Default constructor
    H264StructArray(H264_Heap_Objects * pObjHeap)
        : H264Array<T*>(pObjHeap)
        , m_AllocatedEmpties(0)
    {
    }

    // Destructor
    virtual
    ~H264StructArray(void)
    {
        // It was a beauty class. But some gcc compilers can't
        // parse inherited templates. We have to make this template look ugly.

        H264Array<T *>::m_pObjHeap->Free(H264Array<T *>::m_pArray);
        H264Array<T *>::m_pArray = 0;
    }

    // Allocate one more item
    T * &AllocateOneMore(void)
    {
        bool bOk = true;
        size_t i;

        while (bOk)
        {
            // find free space
            for (i = 0; i < H264Array<T *>::m_nItemCount; i += 1)
            {
                if (NULL == H264Array<T *>::m_pArray[i])
                {
                    H264Array<T *>::m_pArray[i] = H264Array<T *>::m_pObjHeap->AllocateObject((T*)0);
                    return H264Array<T *>::m_pArray[i];
                }
            }

            Reallocate(i);
        }

        // illegal case. It must never hapend
        return H264Array<T *>::m_Null;
    }

    size_t GetItemCount(void) const
    {
        return H264Array<T *>::m_nItemCount;
    }

    // Reset object to initial state
    virtual
    void Reset(void)
    {
        if (H264Array<T *>::m_pArray)
        {
            size_t i;

            for (i = 0; i < H264Array<T *>::m_nItemCount; i += 1)
            {
                if (H264Array<T *>::m_pArray[i])
                    H264Array<T *>::m_pObjHeap->FreeObject(H264Array<T *>::m_pArray[i]);
            }

            ippsSet_8u(0, (Ipp8u*)H264Array<T *>::m_pArray, (Ipp32u)(sizeof(T*) * m_AllocatedEmpties));
            H264Array<T *>::m_nItemCount = 0;
        }
    }

protected:
    // Increase array size
    virtual
    bool Reallocate(size_t nCount)
    {
        if (H264Array<T *>::m_nItemCount > nCount)
            return true;

        // allocate a little bit more
        size_t nAllocated = nCount + 1;

        if (m_AllocatedEmpties < nAllocated)
        {
            T **pTemp = H264Array<T *>::m_pObjHeap->Allocate((T**)0, (Ipp32s)(nAllocated*sizeof(T*)));

            // save array values
            if (H264Array<T *>::m_pArray)
            {
                ippsCopy_8u((Ipp8u*)(H264Array<T*>::m_pArray), (Ipp8u*)pTemp, (Ipp32u)(sizeof(T*) * H264Array<T*>::m_nItemCount));
                H264Array<T *>::m_pObjHeap->Free(H264Array<T *>::m_pArray);
            }

            H264Array<T *>::m_pArray = pTemp;
            m_AllocatedEmpties = nAllocated;
        }

        // allocate rest values
        for (Ipp32u i = (Ipp32u)H264Array<T *>::m_nItemCount; i < nAllocated; i++)
        {
            H264Array<T *>::m_pArray[i] = H264Array<T *>::m_pObjHeap->AllocateObject((T*)0);
        }

        // save values
        H264Array<T *>::m_nItemCount = nAllocated;
        return true;
    }

    size_t  m_AllocatedEmpties;
};

//*********************************************************************************************/
//
//*********************************************************************************************/
class H264CoeffsBuffer
{
public:
    // Default constructor
    H264CoeffsBuffer(void);
    // Destructor
    virtual ~H264CoeffsBuffer(void);

    // Initialize buffer
    Status Init(Ipp32s numberOfItems, Ipp32s sizeOfItem);

    bool IsInputAvailable() const;
    // Lock input buffer
    Ipp8u* LockInputBuffer();
    // Unlock input buffer
    bool UnLockInputBuffer(size_t size);

    bool IsOutputAvailable() const;
    // Lock output buffer
    bool LockOutputBuffer(Ipp8u *& pointer, size_t &size);
    // Unlock output buffer
    bool UnLockOutputBuffer();
    // Release object
    void Close(void);
    // Reset object
    void Reset(void);

protected:
    Mutex mutex;

    Ipp8u *m_pbAllocatedBuffer;       // (Ipp8u *) pointer to allocated unaligned buffer
    size_t m_lAllocatedBufferSize;    // (Ipp32s) size of allocated buffer

    Ipp8u *m_pbBuffer;                // (Ipp8u *) pointer to allocated buffer
    size_t m_lBufferSize;             // (Ipp32s) size of using buffer

    Ipp8u *m_pbFree;                  // (Ipp8u *) pointer to free space
    size_t m_lFreeSize;               // (Ipp32s) size of free space

    size_t m_lItemSize;               // (Ipp32s) size of output data portion

    struct BufferInfo
    {
        Ipp8u * m_pPointer;
        size_t  m_Size;
        BufferInfo *m_pNext;
    };

    BufferInfo *m_pBuffers;           // (Buffer *) queue of filled sample info

    void Lock();
    void Unlock();
private:
    H264CoeffsBuffer(const H264CoeffsBuffer&) {}
    const H264CoeffsBuffer& operator=(const H264CoeffsBuffer&) { return *this; }
};

} // namespace UMC

#endif // __UMC_H264_HEAP_H
