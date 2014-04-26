/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Jobs for thread model
//
*/

#ifndef __UMC_VC1_DEC_JOB_H__
#define __UMC_VC1_DEC_JOB_H__

#include "umc_vc1_common_defs.h"
#include "umc_structures.h"
#include "umc_vc1_dec_task.h"
#include "umc_vc1_dec_exception.h"
#include "umc_memory_allocator.h"

#include "ipps.h"

typedef VC1Status (*MBLayerDecode)(VC1Context* pContext);
extern MBLayerDecode MBLayerDecode_tbl_Adv[];
extern MBLayerDecode MBLayerDecode_tbl[];


typedef void (*Deblock)(VC1Context* pContext);
extern Deblock Deblock_tbl_Adv[];
extern Deblock Deblock_tbl[];

typedef VC1Status (*ProcessDiff)(VC1Context* pContext, Ipp32s blk_num);
extern ProcessDiff ProcessDiff_Adv[];

typedef VC1Status (*MotionComp)(VC1Context* pContex);
extern MotionComp MotionComp_Adv[];

typedef void (*MBSmooth)(VC1Context* pContext, Ipp32s Height);
extern MBSmooth MBSmooth_tbl[];

namespace UMC
{
    class VC1Task;
    class VC1TaskStore;
#ifdef  VC1_THREAD_STATISTIC
    typedef enum
    {
        VC1GetTask           = 0,
        VC1StartProcess      = 1,
        VC1FiniskProcess     = 2,
        VC1AddPerfomrd       = 3,
        VC1NotEnoughBuffer   = 4,

    } VC1StatStates;

    typedef struct
    {
        Ipp32u threadID;
        Ipp64f time;
        _Stat_VC1TaskTypes taskType;
        VC1StatStates taskState;
        Ipp32u       mbStart;
        Ipp32u       mbEnd;

    } VC1ThreadEntry;


    class VC1ThreadStatistic
    {
    public:
        VC1ThreadStatistic(Ipp32u threadNumber,
                           VC1ThreadEntry* entryList,
                           Ipp32u ListSize):m_iTreadNumber(threadNumber),
                                            m_eEntry(entryList),
                                            m_iListSize(ListSize),
                                            m_iNumEntryInList(0)

        {
            m_fInitTime = (Ipp64f)ippGetCpuClocks();
        };

        virtual ~VC1ThreadStatistic() {};

        bool init()
        {
            return true;
        }
        Ipp32u getNumOfEntries()
        {
            return m_iNumEntryInList;
        }
        void reset()
        {
            m_iNumEntryInList = 0;
        }

        inline _Stat_VC1TaskTypes TypetoStatType(VC1TaskTypes type)
        {
            Ipp32u _stat_type = 0;
            Ipp32u _type = type;
            while(_type>1)
            {
                _stat_type++;
                _type >>=1;
            }
            return (_Stat_VC1TaskTypes)_stat_type;
        }

        inline void log_entry(VC1Task* pTask,_Stat_VC1TaskTypes _stat_type,VC1StatStates taskState)
        {
            (m_eEntry+m_iNumEntryInList)->threadID = m_iTreadNumber;
            (m_eEntry+m_iNumEntryInList)->taskType = _stat_type;
            (m_eEntry+m_iNumEntryInList)->taskState = taskState;

            (m_eEntry+m_iNumEntryInList)->time = ippGetCpuClocks()-m_fInitTime;
            if (pTask->m_pSlice)
            {
                (m_eEntry+m_iNumEntryInList)->mbStart = pTask->m_pSlice->MBStartRow;
                (m_eEntry+m_iNumEntryInList)->mbEnd = pTask->m_pSlice->MBEndRow;
            }
            ++m_iNumEntryInList;
            assert(m_iNumEntryInList <= m_iListSize);
         };

        Ipp32u m_iTreadNumber;
        VC1ThreadEntry* m_eEntry;
        Ipp32u m_iListSize;
        Ipp32u m_iNumEntryInList;
        Ipp64f m_fInitTime;

    };
#define STATISTIC_PARAL_ENTRY(pTask,taskType,taskState)                                                                        \
    pTask->pJob->m_Statistic->log_entry(pTask, pTask->pJob->m_Statistic->TypetoStatType(taskType),taskState);


#else

#define STATISTIC_PARAL_ENTRY(pTask,taskType,taskState)

#endif

    class VC1TaskProcessor
    {
    public:
        virtual Status process() = 0;
        virtual Status processMainThread() = 0;
        VC1TaskProcessor(){};
        virtual ~VC1TaskProcessor() {};
    };

    class VC1TaskProcessorUMC : public VC1TaskProcessor
    {
    public:
        VC1TaskProcessorUMC(): m_pMemoryAllocator(NULL),
                            m_iNumber(0),
                            m_iMemContextID((MemID)-1),
                            m_pContext(NULL),
                            m_pSmoothingInfo(NULL),
                            m_pSingleMB(NULL),
                            m_pStore(NULL)
        {
            pReconstructTbl[0] = &VC1ProcessDiffInter;
            pReconstructTbl[1] = &VC1ProcessDiffIntra;
        };
        virtual ~VC1TaskProcessorUMC() { Release ();}
        virtual Status Init(VC1Context* pContext,
                            Ipp32s iNumber,
                            VC1TaskStore* pStore,
                            MemoryAllocator* pMemoryAllocator);
        virtual Status process();
        virtual Status processMainThread();
        inline virtual void AccelerReconstruct()
        {
            pReconstructTbl[0] = &VC1ProcessDiffSpeedUpInter;
            pReconstructTbl[1] = &VC1ProcessDiffSpeedUpIntra;
        };


#ifdef  VC1_THREAD_STATISTIC
        VC1ThreadStatistic*      m_Statistic;
#endif

        virtual VC1Status VC1Decoding                                      (VC1Context* pContext, VC1Task* pTask);
        virtual VC1Status VC1ProcessDiff                                   (VC1Context* pContext, VC1Task* pTask);
        virtual VC1Status VC1Deblocking                                    (VC1Context* pContext, VC1Task* pTask);
        virtual VC1Status VC1MotionCompensation                            (VC1Context* pContext, VC1Task* pTask);
        virtual VC1Status VC1MVCalculation                                 (VC1Context* pContext, VC1Task* pTask);
        virtual VC1Status VC1PrepPlane                                     (VC1Context* pContext, VC1Task* pTask);


   protected:

        void   Release ();
        Status InitPicParamJob();
        void   InitContextForNextTask(VC1Task* pTask);
        void   WriteDiffs(VC1Context* pContext);
        void   ProcessSmartException                                       (VC1Exceptions::SmartLevel exLevel, VC1Context* pContext, VC1Task* pTask, VC1MB* pCurrMB);

        VC1Status (*pReconstructTbl[2])(VC1Context* pContext, Ipp32s blk_num);

        void CompensateInterlacePFrame(VC1Context* pContext, VC1Task *pTask);
        void CompensateInterlaceBFrame(VC1Context* pContext, VC1Task *pTask);

        MemoryAllocator*        m_pMemoryAllocator; // (MemoryAllocator*) pointer to memory allocator
        Ipp32s                  m_iNumber;
        UMC::MemID              m_iMemContextID;
        VC1Context*             m_pContext;
        VC1Smoothing*           m_pSmoothingInfo;
        VC1SingletonMB*         m_pSingleMB;
        VC1TaskStore*           m_pStore;
        //bool                    m_bIsNeedPostProc;
    };


}

#endif //__umc_vc1_dec_job_H__
