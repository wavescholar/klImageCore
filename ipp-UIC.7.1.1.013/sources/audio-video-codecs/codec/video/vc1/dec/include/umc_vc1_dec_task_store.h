/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Task processing base on H264
//
*/

#ifndef __UMC_UMC_VC1_DEC_TASK_STORE_H_
#define __UMC_UMC_VC1_DEC_TASK_STORE_H_

#include <new>

#include "umc_vc1_dec_task.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_frame_descr.h"
#include "vm_types.h"
#include "umc_event.h"
#include "umc_vc1_dec_job.h"
#include "umc_automatic_mutex.h"
#include "umc_vc1_dec_skipping.h"
#include "umc_vc1_dec_exception.h"


namespace UMC
{
    typedef enum
    {
        VC1_HD_STREAM = 0,
        VC1_MD_STREAM = 1,
        VC1_SD_STREAM = 2
    } VC1_STREAM_DEFINITION;

    class VC1TSHeap
    {
    public:
        VC1TSHeap(Ipp8u* pBuf, Ipp32s BufSize):m_pBuf(pBuf),
                  m_iRemSize(BufSize)
        {
        };
        virtual ~VC1TSHeap()
        {
        };

        template <class T>
            void s_new(T*** pObj, Ipp32u size)
        {
            Ipp32s size_T = sizeof(pObj)*size;
            if (m_iRemSize - align_value<Ipp32s>(size_T) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                *pObj = new(m_pBuf)(T*[size]);
                m_pBuf += align_value<Ipp32s>(size_T);
                m_iRemSize -= align_value<Ipp32s>(size_T);
            }
        };
        template <class T>
        void s_new_one(T** pObj, Ipp32u size)
        {
            Ipp32s size_T = sizeof(T)*size;
            if (m_iRemSize - align_value<Ipp32s>(size_T) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                *pObj = new(m_pBuf)(T[size]);
                m_pBuf += align_value<Ipp32s>(size_T);
                m_iRemSize -= align_value<Ipp32s>(size_T);
            }
        };
        template <class T>
            void s_new(T** pObj, Ipp32u size)
        {
            Ipp32s size_T = sizeof(T);
            if ((m_iRemSize - align_value<Ipp32s>(size_T)*size) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                for(Ipp32u i = 0; i < size; i++)
                {
                    pObj[i] = new(m_pBuf)(T);
                    m_pBuf += align_value<Ipp32s>(size_T);
                    m_iRemSize -= align_value<Ipp32s>(size_T);
                }
            }

        };
        template <class Tbase,
                  class T,
                  class Arg>
            void s_new(Tbase** pObj,
                       Ipp32u size,
                       Arg*   pArg)
        {
            Ipp32s size_T = sizeof(T);
            if ((m_iRemSize - align_value<Ipp32s>(size_T)*size) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                for(Ipp32u i = 0; i < size; i++)
                {
                    pObj[i] = new(m_pBuf)(T)(pArg);
                    m_pBuf += align_value<Ipp32s>(size_T);
                    m_iRemSize -= align_value<Ipp32s>(size_T);
                }
            }

        };

        template <class T>
            void s_new(T** pObj)
        {
            Ipp32s size_T = sizeof(T);
            if ((m_iRemSize - align_value<Ipp32s>(size_T)) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                *pObj = new(m_pBuf)(T);
                m_pBuf += align_value<Ipp32s>(size_T);
                m_iRemSize -= align_value<Ipp32s>(size_T);
            }
        }
        template <class T>
            Ipp8u* s_alloc()
        {
            Ipp8u* pRet;
            Ipp32s size_T = sizeof(T);
            if ((m_iRemSize - align_value<Ipp32s>(size_T)) < 0)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            else
            {
                pRet = m_pBuf;
                m_pBuf += align_value<Ipp32s>(size_T);
                m_iRemSize -= align_value<Ipp32s>(size_T);
                return pRet;
            }
        }
    private:
        Ipp8u*  m_pBuf;
        Ipp32s  m_iRemSize;

    };

    class VC1VideoDecoder;
    class VC1TaskStore: public VC1Skipping::VC1SkipMaster
    {
    public:

        VC1TaskStore(MemoryAllocator *pMemoryAllocator);

        virtual  ~VC1TaskStore();
        void* operator new(size_t size, void* p)
        {
            if (!p)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
            return new(p) Ipp8u[size];
        };

        // external memory management. No need to delete memory
        void operator delete(void *p)
        {
            //Anyway its incorrect when we trying free null pointer
            if (!p)
                throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
        };

        void operator delete(void *, void *) 
        {
            // delete for system exceptions case
            throw VC1Exceptions::vc1_exception(VC1Exceptions::mem_allocation_er);
        };

        bool     Init(Ipp32u iConsumerNumber,
                      Ipp32u iMaxFramesInParallel,
                      VC1VideoDecoder* pVC1Decoder);

        void Reset();

        bool AddSampleTask(VC1Task* _pTask, Ipp32u qID);
        void DistributeTasks(Ipp32u qID);
        bool GetNextTask(VC1FrameDescriptor **pFrameDS,
                         VC1Task **pTask,
                         Ipp32u qID);
        bool GetNextTaskMainThread(VC1FrameDescriptor **pFrameDS,
                                   VC1Task** pTask,
                                   Ipp32u qID);

        // pipeline for Dual CPU cases.
        bool GetNextTaskDualCPU(VC1FrameDescriptor **pFrameDS,
                                  VC1Task **pTask,
                                  Ipp32u qID);

        // pipeline for 4-CPU cases, high-defenition streams.
        bool GetNextTaskManyCPU_HD(VC1FrameDescriptor **pFrameDS,
                                   VC1Task **pTask,
                                   Ipp32u qID);

        // pipeline for 4-CPU cases, medium/small-defenition streams.
        bool GetNextTaskManyCPU_MD(VC1FrameDescriptor **pFrameDS,
                                   VC1Task **pTask,
                                   Ipp32u qID);

        bool AddPerfomedTask(VC1Task* pTask, VC1FrameDescriptor *pFrameDS);
        void SetFrameSettings(Ipp32u _frame_settings) {frame_settings = _frame_settings;}
        void ResetQueue(Ipp32u qID);

        VC1PictureLayerHeader* GetFirstInSecondField(Ipp32u qID);

        bool CreateDSQueue(VC1Context* pContext, bool IsReorder,Ipp16s* pResidBuf); //pResidBuf - memory for residual coeffs


        void ResetDSQueue();

        template <class Descriptor>
        void GetReadyDS(Descriptor** pDS)
        {
            //AutomaticMutex guard(m_mDSGuard);

            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if (m_pDescriptorQueue[i]->m_bIsReadyToLoad)
                {
                    m_pDescriptorQueue[i]->m_bIsReadyToLoad  = false;
                    m_pDescriptorQueue[i]->m_iActiveTasksInFirstField = 0;
                    *pDS = (Descriptor*)m_pDescriptorQueue[i];
                    ++m_iNumDSActiveinQueue;
                    return;
                }
            }
            *pDS = NULL;
        }

        //bool GetPerformedDS(VC1FrameDescriptor** pDS);
        template <class Descriptor>
        bool GetPerformedDS(Descriptor** pDS)
        {
            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if (m_pDescriptorQueue[i]->m_bIsReadyToDisplay)
                {
                    if ((m_lNextFrameCounter == m_pDescriptorQueue[i]->m_iFrameCounter)&&
                        (!m_pDescriptorQueue[i]->m_bIsSkippedFrame))
                    {
                        m_pDescriptorQueue[i]->m_bIsReadyToDisplay = false;
                        m_pDescriptorQueue[i]->m_bIsReadyToLoad = true;
                        --m_iNumDSActiveinQueue;
                        ++m_lNextFrameCounter;
                        *pDS = (Descriptor*)m_pDescriptorQueue[i];
                        if (!(*pDS)->isVADescriptor())
                        {
                            if (VC1_IS_REFERENCE(m_pDescriptorQueue[i]->m_pContext->m_InitPicLayer->PTYPE))
                                FreeFirstReference();
                            else if (!(*pDS)->IsNeedToDelayBFrame())
                                FreeBFrame(m_pDescriptorQueue[i]->m_pContext->m_frmBuff.m_iCurrIndex);
                        }
                        return true;
                    }
                }
            }
            *pDS = NULL;
            return false;
        }
        // need for correct work VideoAcceleration
        void SetFirstBusyDescriptorAsReady()
        {
            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if ((!m_pDescriptorQueue[i]->m_bIsReadyToDisplay)&&
                     (m_pDescriptorQueue[i]->m_iFrameCounter == m_lNextFrameCounter))
                {
                    m_pDescriptorQueue[i]->m_bIsReadyToDisplay = true;
                    m_pDescriptorQueue[i]->m_bIsReferenceReady = true;
                    return;
                }
            }
        }
        void ResetPerformedDS(VC1FrameDescriptor* pDS)
        {
            pDS->m_bIsReadyToDisplay = false;
            pDS->m_bIsReadyToLoad = true;
            --m_iNumDSActiveinQueue;
            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if (!m_pDescriptorQueue[i]->m_bIsReadyToLoad)
                    ++m_pDescriptorQueue[i]->m_iFrameCounter;
            }
            ++m_lNextFrameCounter;
        }

        void AddInvalidPerformedDS(VC1FrameDescriptor* pDS)
        {
            pDS->m_bIsReadyToProcess = false;
            pDS->m_bIsReadyToDisplay = true;
            pDS->m_bIsValidFrame = false;
        }
        void AddPerformedDS(VC1FrameDescriptor* pDS)
        {
            pDS->m_bIsReadyToProcess = false;
            pDS->m_bIsReadyToDisplay = true;
        }
        inline bool IsPerfomedDS();
        bool IsProcessingDS();
        VC1FrameDescriptor* GetLastDS();
        VC1FrameDescriptor* GetFirstDS();

        FrameType GetOutputFrameType() {return m_FrameType;}
        void      SetOutputFrameType(FrameType CurFrameType)  {m_FrameType = CurFrameType;}
        template <class Descriptor>
        bool GetReadySkippedDS(Descriptor** pDS, bool IsSpecialMode)
        {
            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if ((m_pDescriptorQueue[i]->m_bIsReferenceReady)&&
                    (m_pDescriptorQueue[i]->m_bIsSkippedFrame)&&
                    //(m_pDescriptorQueue[i]->m_bIsBReady)&&
                    (m_lNextFrameCounter == m_pDescriptorQueue[i]->m_iFrameCounter))
                {
                    *pDS = (Descriptor*)m_pDescriptorQueue[i];
                    m_pDescriptorQueue[i]->m_bIsReadyToDisplay = false;
                    m_pDescriptorQueue[i]->m_bIsReadyToLoad = true;
                    m_pDescriptorQueue[i]->m_bIsSkippedFrame = false;
                    --m_iNumDSActiveinQueue;
                    ++m_lNextFrameCounter;
                    // in case of special mode we don't copy frame in case of Skip frame
                    // Need for H/W acceleration
                    if (!IsSpecialMode)
                    {
                        if (!m_pDescriptorQueue[i]->m_bIsSpecialBSkipFrame)
                            FreeFirstReference();
                        else
                            FreeBFrame(m_pDescriptorQueue[i]->m_pContext->m_frmBuff.m_iCurrIndex);
                    }
                    return true;
                }
            }
            *pDS = NULL;
            return false;
        }
        Ipp32u GetProcFramesNumber() { return m_iNumDSActiveinQueue; }
        void OpenNextFrames(VC1FrameDescriptor* pDS,VC1FrameDescriptor** pPrevDS,Ipp32s* CurrRefDst, Ipp32s* CurBDst);
        void SetDstForFrameAdv (VC1FrameDescriptor* pDS, Ipp32s* CurrRefDst, Ipp32s* CurBDst);
        void SetDstForFrame    (VC1FrameDescriptor* pDS, Ipp32s* CurrRefDst, Ipp32s* CurBDst);

        // threads wake up
        inline void WakeUP() {for (Ipp32u i=1; i<m_iConsumerNumber;i++) m_eWaiting[i]->Set(); }
        void WakeTasksInAlienQueue(VC1FrameDescriptor* pDS, VC1FrameDescriptor** pPrevDS);
        void WakeTasksInAlienQueue(VC1FrameDescriptor* pDS);
        void CompensateDSInQueue(VC1FrameDescriptor* pDS);

        void CreateOutBuffersQueue();

        // Own manage
        Ipp32s GetInternalIndex(); // for B/BI frames
        Ipp32s GetRefInternalIndex(); // for I/P/Skipped frames
        Ipp32s GetPrevIndex();
        Ipp32s GetNextIndex();
        Ipp32s GetDisplayIndex();
        Ipp32s GetICIndex() {return m_iICompFieldIndex;};
        void  FreeIndexQueue();
        void FreeFirstReference();
        void FreeBFrame(Ipp32u Index);


        // External index managment
        Ipp32s GetCurrIndex_VA(void) {return m_CurrIndex;} ;
        Ipp32s GetPrevIndex_VA(void) {return m_PrevIndex;} ;
        Ipp32s GetNextIndex_VA(void) {return m_NextIndex;} ;
        Ipp32s GetBFrameIndex_VA(void) {return m_BFrameIndex;} ;
        Ipp32s GetICIndex_VA(void) {return m_iICIndex;} ;

        void SetCurrIndex_VA(Ipp32s Index) {m_CurrIndex = Index;};
        void SetPrevIndex_VA(Ipp32s Index) {m_PrevIndex = Index;};
        void SetNextIndex_VA(Ipp32s Index) {m_NextIndex = Index;};
        void SetBFrameIndex_VA(Ipp32s Index) {m_BFrameIndex = Index;};
        void SetICIndex_VA(Ipp32s Index) {m_iICIndex = Index;} ;






        void StartDecoding() {m_bIsNeedToDecode = true;}
        void StopDecoding() {m_bIsNeedToDecode = false;}

        void ResetDS(VC1FrameDescriptor* pDS)
        {
            pDS->m_bIsReadyToLoad = true;
            pDS->m_bIsReadyToDisplay = false;
            --m_iNumDSActiveinQueue;
        }
        bool CreateTaskQueues();
        void ReleaseTaskQueues();

        void SetDefinition(VC1SequenceLayerHeader*    seqLayerHeader);

    private:

        class VC1FrameIndexProc
        {
        public:
            VC1FrameIndexProc(): m_bIsBusy(false),
                m_lRefCounter(1)
            {
            };
            bool        m_bIsBusy;
            Ipp64u      m_lRefCounter;
        };

        inline VC1FrameDescriptor* GetPreferedDS()
        {
            VC1FrameDescriptor* pCurrDescriptor = m_pDescriptorQueue[0];
            Ipp32u i;
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if (m_pDescriptorQueue[i]->m_bIsReadyToProcess)
                {
                    pCurrDescriptor = m_pDescriptorQueue[i];
                    break;
                }
            }
            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                if ((m_pDescriptorQueue[i]->m_bIsReadyToProcess)&&
                    (m_pDescriptorQueue[i]->m_iFrameCounter < pCurrDescriptor->m_iFrameCounter))
                    pCurrDescriptor = m_pDescriptorQueue[i];
            }
            return pCurrDescriptor;
        };
        inline bool IsMainTaskPrepareForProcess(Ipp32u FrameID, Ipp32u);
        inline bool IsAdditionalTaskPrepareForProcess(Ipp32u FrameID, Ipp32u TaskID);
        inline bool IsFrameReadyToDisplay(Ipp32u FrameID);
        // main queue processing
        inline void SetTaskAsReady_MQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void SetTaskAsNotReady_MQ(Ipp32u FrameID, Ipp32u TaskID);

        inline void SetTaskAsDone_MQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void EnableProcessBit_MQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void DisableProcessBit_MQ(Ipp32u FrameID, Ipp32u TaskID);
        // additional queue processing
        inline void SetTaskAsReady_AQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void SetTaskAsNotReady_AQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void SetTaskAsDone_AQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void EnableProcessBit_AQ(Ipp32u FrameID, Ipp32u TaskID);
        inline void DisableProcessBit_AQ(Ipp32u FrameID, Ipp32u TaskID);

        Ipp32u CalculateHeapSize();

        VC1Task*** m_pCommonQueue;
        VC1Task*** m_pAdditionalQueue;

        Ipp32u  m_iConsumerNumber;
        Ipp32u* m_pTasksInQueue;
        Ipp32u  m_iCurrentTaskID;
        Ipp32u* m_pSlicesInQueue;



        VC1FrameDescriptor** m_pDescriptorQueue;
        VC1FrameIndexProc** m_pOutFrameQueue;


        Event** m_eWaiting;
        Ipp32u m_iNumFramesProcessing;
        Ipp32u m_iNumDSActiveinQueue;

        Ipp32u m_iNumRefFrames;
        Ipp32u m_iNumProcFrames;
        Mutex m_mDSGuard;


        Mutex** m_pGuardGet;
        Mutex** m_pGuardAdd;
        Ipp32u frame_settings;
        VC1VideoDecoder* pMainVC1Decoder;
        Ipp64u m_lNextFrameCounter;
        Ipp64u m_lNextRefFrameCounter;
        Ipp64u m_lFirstRefFrameCounter;
        Ipp32u m_iNumDecodeRefFrames;
        VC1FrameDescriptor* m_pPrefDS;
        Ipp32s* m_pDSIndicate;
        Ipp32s* m_pDSIndicateSwap;
        bool   m_bNeedToCompField;
        Ipp32s m_iICompFieldIndex;
        bool   m_bIsNeedToDecode;
        VC1_STREAM_DEFINITION m_eStreamDef;

        MemoryAllocator*           m_pMemoryAllocator; // (MemoryAllocator*) pointer to memory allocator

        // for correct VA support
        // H/W manage index by itself
        Ipp32s m_CurrIndex;
        Ipp32s m_PrevIndex;
        Ipp32s m_NextIndex;
        Ipp32s m_BFrameIndex;
        Ipp32s m_iICIndex;


        // each DWORD of m_pMainQueueTasksState and m_pAdditionaQueueTasksState contains state of 8 Tasks. Maximum number of tasks is 512 (row number)
        // Task state occupiers for 4 bits. xxxx
        // 0-th bit isDone
        // 1-th bit isProcess
        // 2-th bit isReady
        // 3-th bit Reserved
        Ipp32u* m_pMainQueueTasksState;
        Ipp32u* m_pAdditionaQueueTasksState;
        UMC::MemID     m_iIntStructID;
        UMC::MemID     m_iTSHeapID;

        VC1TSHeap* m_pSHeap;

        // for reordering
        FrameType     m_FrameType;
    };
}

#endif //__umc_umc_vc1_dec_task_store_H__
