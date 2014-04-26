/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PTS_BUF_H__
#define __PTS_BUF_H__

#include "array.h"
#include "umc_mutex.h"
#include "vm_debug.h"
#include "umc_structures.h"

namespace UMC
{

class BufPTSArray
{
public:
    BufPTSArray(): m_dfNorm(1) {}
    ~BufPTSArray() {}

    Status Init(const Ipp32u uiArraySize,
                const Ipp64f fdNorm);

    Status SetFramePTS(const Ipp32u uiBufPos,
                        const Ipp32u uiLen,
                        const Ipp64f dfPTS);

    Status UpdateFramePTS(const Ipp32u uiOldBufPos,
                            const Ipp32u uiNewBufPos,
                            const Ipp32u uiNewLen,
                            const Ipp64f dfNewPTS);
    Ipp64f DynamicSetParams(const Ipp64f fdNorm);

    Ipp64f GetTime(const Ipp32u uiCurPos);
    void Reset();

protected:
    struct sPtsEntry
    {
        Ipp64f dfPts;
        Ipp32u uiStart;
        Ipp32u uiLen;
        sPtsEntry(): dfPts(-1), uiStart(0), uiLen(0) {}
    };

    void Normalize();

    SimpleArray<sPtsEntry>  m_PtsArray;
    Ipp64f                  m_dfNorm;
    Mutex                   m_Mutex;
};


class BufPTSArrayM
{
protected:
    struct sPtsEntry {
        Ipp64f dfPts;
        Ipp32u uiStart;
        Ipp32u uiLen;
        sPtsEntry() {
            dfPts = -1;
            uiStart = 0;
            uiLen = 0;
        }
    };

    SimpleArray<sPtsEntry> m_PtsArray;
    Ipp64f                 m_dfNorm;
    Mutex                  m_Mutex;
    Ipp32s                 m_doNormalize;

    void Normalize();

public:
    BufPTSArrayM(): m_dfNorm(1) {};
    ~BufPTSArrayM() {}

    Status Init(const Ipp32u uiArraySize,
                const Ipp64f fdNorm,
                bool bNormalize);

    Status SetFramePTS(const Ipp32u uiBufPos,
                        const Ipp32u uiLen,
                        const Ipp64f dfPTS);

    Status UpdateFramePTS(const Ipp32u uiOldBufPos,
                            const Ipp32u uiNewBufPos,
                            const Ipp32u uiNewLen,
                            const Ipp64f dfNewPTS);

    Ipp64f GetTime(const Ipp32u uiCurPos);
    void Reset();
};

}  //  namespace UMC

#endif  //  __PTS_BUF_H__
