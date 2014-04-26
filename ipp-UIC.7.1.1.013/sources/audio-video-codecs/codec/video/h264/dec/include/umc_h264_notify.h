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

#ifndef __UMC_H264_NOTIFY_H
#define __UMC_H264_NOTIFY_H

#include "umc_h264_heap.h"
#include <list>

namespace UMC
{

class notifier_base
{
public:
    notifier_base()
        : next_(0)
        , m_isNeedNotification(true)
    {
    }

    virtual ~notifier_base()
    {}

    virtual void Notify() = 0;

    void ClearNotification() { m_isNeedNotification = false; }

    notifier_base * next_;

protected:
    bool m_isNeedNotification;
};

template <typename Object>
class notifier0 : public notifier_base
{
public:
    typedef void (Object::*Function)();

    notifier0(Object* object, Function function)
        : object_(object)
        , function_(function)
    {
    }

    ~notifier0()
    {
        Notify();
    }

    virtual void Notify()
    {
        if (m_isNeedNotification)
            (object_->*function_)();
    }

private:
    Object* object_;
    Function function_;
};

template <typename Object, typename Param1>
class notifier1 : public notifier_base
{
public:
    typedef void (Object::*Function)(Param1 param1);

    notifier1(Object* object, Function function, Param1 param1)
        : object_(object)
        , function_(function)
        , param1_(param1)
    {
    }

    ~notifier1()
    {
        Notify();
    }

    virtual void Notify()
    {
        if (m_isNeedNotification)
            (object_->*function_)(param1_);
    }

private:
    Object* object_;
    Function function_;
    Param1 param1_;
};

template <typename Object, typename Param1, typename Param2>
class notifier2 : public notifier_base
{
public:
    typedef void (Object::*Function)(Param1 param1, Param2 param2);

    notifier2(Object* object, Function function, Param1 param1, Param2 param2)
        : object_(object)
        , function_(function)
        , param1_(param1)
        , param2_(param2)
    {
    }

    ~notifier2()
    {
        Notify();
    }

    virtual void Notify()
    {
        if (m_isNeedNotification)
            (object_->*function_)(param1_, param2_);
    }

private:
    Object* object_;
    Function function_;
    Param1 param1_;
    Param2 param2_;
};

class NotifiersChain : public notifier_base
{
public:
    NotifiersChain(H264_Heap_Objects * pObjHeap)
        : m_pObjHeap(pObjHeap)
    {
    }

    void Reset()
    {
        ChainType::iterator end = m_chain.end();
        ChainType::iterator iter = m_chain.begin();
        for (; iter != end; ++iter)
        {
            notifier_base *tmp1 = *iter;
            m_pObjHeap->Free(tmp1);
        }

        m_chain.clear();
    }

    void Abort()
    {
        m_chain.clear();
    }

    virtual ~NotifiersChain()
    {
        Reset();
    }

    bool IsEmpty() const
    {
        return m_chain.empty();
    }

    void AddNotifier(notifier_base * nt)
    {
        m_chain.push_back(nt);
    }

    void MoveNotifiers(NotifiersChain * chain)
    {
        m_chain.splice(m_chain.end(), chain->m_chain);
    }

    virtual void Notify()
    {
        ChainType::iterator end = m_chain.end();
        ChainType::iterator iter = m_chain.begin();
        for (; iter != end; ++iter)
        {
            notifier_base *tmp1 = *iter;
            tmp1->Notify();
            m_pObjHeap->Free(tmp1);
        }

        m_chain.clear();
    }

    H264_Heap_Objects * GetObjHeap()
    {
        return m_pObjHeap;
    }

private:
    typedef std::list<notifier_base *> ChainType;
    ChainType m_chain;

    H264_Heap_Objects * m_pObjHeap;
};

} // namespace UMC

#endif // __UMC_H264_NOTIFY_H
