/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_LINKED_LIST_H__
#define __UMC_LINKED_LIST_H__

#include "ippdefs.h"
#include "umc_structures.h"

namespace UMC
{

template <class T> class LinkedList
{
protected:

    // Inner structure to represent one element of the list
    struct ListElement
    {
        // Constuctor
        ListElement()
        {
            pNext = NULL;
            pPrev = NULL;
        }

        // Data of the list element
        T m_data;

        // The pointer to next element of the list
        ListElement* pNext;

        // The pointer to previous element of the list
        ListElement* pPrev;
    };

public:

    // Constructor
    LinkedList()
    {
        m_pFirst = NULL;
        m_pLast = NULL;
        m_pLastReturned = NULL;
        m_pHeap = NULL;
        m_iSize = 0;
    }

    //Destructor
    virtual ~LinkedList()
    {
        ListElement* tmp;

        while (m_pFirst)
        {
            tmp = m_pFirst;
            m_pFirst = m_pFirst->pNext;
            delete tmp;
        }

        while (m_pHeap)
        {
            tmp = m_pHeap;
            m_pHeap = m_pHeap->pNext;
            delete tmp;
        }

        m_pLast = m_pLastReturned = NULL;
    };

    // Returns number of elements in the list
    virtual Ipp32u Size() const
    {
        return m_iSize;
    }

    // Function to get the first element of the list
    // returns UMC_OK if succeeds (updates m_pLastReturned)
    // returns UMC_ERR_NOT_ENOUGH_DATA if list is empty
    virtual Status First(T &data)
    {
        if (NULL == m_pFirst)
            return UMC_ERR_NOT_ENOUGH_DATA;

        m_pLastReturned = m_pFirst;
        data = m_pFirst->m_data;
        return UMC_OK;
    }

    // Function to get the last element of the list
    // returns UMC_OK if succeeds (updates m_pLastReturned)
    // returns UMC_ERR_NOT_ENOUGH_DATA if list is empty
    virtual Status Last(T &data)
    {
        if (NULL == m_pLast)
            return UMC_ERR_NOT_ENOUGH_DATA;

        m_pLastReturned = m_pLast;
        data = m_pLast->m_data;
        return UMC_OK;
    }

    // Function to get the next element after the most recently returned
    // returns UMC_OK if succeeds (updates m_pLastReturned)
    // returns UMC_ERR_FAILED if there is no last returned element
    // returns UMC_ERR_NOT_ENOUGH_DATA if there is no next element
    virtual Status Next(T& data)
    {
        if (NULL == m_pLastReturned)
            return UMC_ERR_FAILED;

        if (NULL == m_pLastReturned->pNext)
            return UMC_ERR_NOT_ENOUGH_DATA;

        m_pLastReturned = m_pLastReturned->pNext;
        data = m_pLastReturned->m_data;
        return UMC_OK;
    }

    // Function to get the previous element before the most recently returned
    // returns UMC_OK if succeeds (updates m_pLastReturned)
    // returns UMC_ERR_FAILED if there is no last returned element
    // returns UMC_ERR_NOT_ENOUGH_DATA if there is no previous element
    virtual Status Prev(T &data)
    {
        if (NULL == m_pLastReturned)
            return UMC_ERR_FAILED;

        if (NULL == m_pLastReturned->pPrev)
            return UMC_ERR_NOT_ENOUGH_DATA;

        m_pLastReturned = m_pLastReturned->pPrev;
        data = m_pLastReturned->m_data;
        return UMC_OK;
    }

    // Function to get element at a specified position in the list
    // returns UMC_OK if succeeds (updates m_pLastReturned)
    // returns UMC_ERR_FAILED if specified position is wrong
    virtual Status Get(T &data, Ipp32s index)
    {
        if (index < 0 || index >= m_iSize)
            return UMC_ERR_FAILED;

        m_pLastReturned = GetElement(index);
        data = m_pLastReturned->m_data;
        return UMC_OK;
    }

    // Function to add element to the end of the list
    // returns UMC_OK if succeeds
    // returns UMC_ERR_ALLOC if failed to allocate new element
    virtual Status Add(T& data)
    {
        ListElement* pNew;
        if (m_pHeap)
        {
            // Reuse allocated element
            pNew = m_pHeap;
            m_pHeap = m_pHeap->pNext;
            if (m_pHeap)
                m_pHeap->pPrev = NULL;
            pNew->pNext = NULL;
        }
        else
        {
            // There is no allocated elements, create new
            pNew = new ListElement;
            if (!pNew)
                return UMC_ERR_ALLOC;
        }

        pNew->m_data = data;
        if (0 == m_iSize)
            m_pFirst = m_pLast = pNew;
        else
        {
            pNew->pPrev = m_pLast;
            m_pLast->pNext = pNew;
            m_pLast = pNew;
        }

        m_iSize++;
        return UMC_OK;
    }

    // Function to add element in a specified position in the list
    // returns UMC_OK if succeeds
    // returns UMC_ERR_FAILED if specified position is wrong
    // returns UMC_ERR_ALLOC if failed to allocate new element
    virtual Status Add(T& data, Ipp32s index)
    {
        if (index < 0 || index > m_iSize)
            return UMC_ERR_FAILED;

        if (index < m_iSize)
        {
            ListElement* pNew;
            if (m_pHeap)
            {
                // Reuse allocated element
                pNew = m_pHeap;
                m_pHeap = m_pHeap->pNext;
                if (m_pHeap)
                    m_pHeap->pPrev = NULL;
            }
            else
            {
                // There is no allocated elements, create new
                pNew = new ListElement;
                if (!pNew)
                    return UMC_ERR_ALLOC;
            }

            ListElement* pAfterNew = GetElement(index);
            pNew->m_data = data;
            pNew->pNext = pAfterNew;
            pNew->pPrev = pAfterNew->pPrev;
            if (NULL == pAfterNew->pPrev)
                m_pFirst = pNew;
            else
                pAfterNew->pPrev->pNext = pNew;
            pAfterNew->pPrev = pNew;
            m_iSize++;
            return UMC_OK;
        }
        else
            return Add(data);
    }

    // Function to remove element at the end of the list
    // returns UMC_OK if succeeds
    // returns UMC_ERR_FAILED if list is empty
    virtual Status Remove(void)
    {
        if (0 == m_iSize)
            return UMC_ERR_FAILED;

        ListElement* pToRemove = m_pLast;
        m_pLast = m_pLast->pPrev;
        if (m_pLast)
            m_pLast->pNext = NULL;
        else
            m_pFirst = NULL;
        m_iSize--;

        // if last returned element is removed
        if (m_pLastReturned == pToRemove)
            m_pLastReturned = NULL;

        // add removed element to the begin of heap
        pToRemove->pPrev = NULL;
        pToRemove->pNext = m_pHeap;
        if (m_pHeap)
            m_pHeap->pPrev = pToRemove;
        m_pHeap = pToRemove;
        return UMC_OK;
    }

    // Function to remove element at the specified position in the list
    // returns UMC_OK if succeeds
    // returns UMC_ERR_FAILED if specified position is wrong
    virtual Status Remove(Ipp32s index)
    {
        if (index < 0 || index >= m_iSize)
            return UMC_ERR_FAILED;

        ListElement* pToRemove = GetElement(index);

        if (index > 0) // except for the first element
            pToRemove->pPrev->pNext = pToRemove->pNext;
        else
            m_pFirst = pToRemove->pNext;

        if (index < m_iSize - 1) // except for the last element
            pToRemove->pNext->pPrev = pToRemove->pPrev;
        else
            m_pLast = pToRemove->pPrev;

        m_iSize--;

        // if last returned element is removed
        if (m_pLastReturned == pToRemove)
            m_pLastReturned = NULL;

        pToRemove->pPrev = NULL;
        pToRemove->pNext = m_pHeap;
        if (m_pHeap)
            m_pHeap->pPrev = pToRemove;
        m_pHeap = pToRemove;
        return UMC_OK;
    }

    // Function to modify element at the specified position in the list
    // returns UMC_OK if succeeds
    // returns UMC_ERR_FAILED if specified position is wrong
    virtual Status Modify(T& data, Ipp32s index)
    {
        if (index < 0 || index >= m_iSize)
            return UMC_ERR_FAILED;

        ListElement* pToModify = GetElement(index);
        pToModify->m_data = data;
        return UMC_OK;
    }

protected:

    // Returns element at a specified position
    // Param 'index' is not checked
    ListElement* GetElement(Ipp32s index)
    {
        ListElement* pElem;
        if (index < m_iSize / 2)
        { // find from the begin
            pElem = m_pFirst;
            while (index-- > 0)
                pElem = pElem->pNext;
        }
        else
        { // find from the end
            pElem = m_pLast;
            while (++index < m_iSize)
                pElem = pElem->pPrev;
        }
        return pElem;
    }

    // The pointer to the first element of the list
    ListElement* m_pFirst;

    // The pointer to the last element of the list
    ListElement* m_pLast;

    // The pointer to the last returned element
    ListElement* m_pLastReturned;

    // The pointer to the once allocated elements but currently unused
    ListElement* m_pHeap;

    // The number of elements in the list
    Ipp32s m_iSize;

}; //template <class T> class LinkedList

}; // namespace UMC

#endif // __UMC_LINKED_LIST_H__
