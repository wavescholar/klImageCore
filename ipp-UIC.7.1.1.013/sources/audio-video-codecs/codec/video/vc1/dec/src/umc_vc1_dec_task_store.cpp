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

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_task_store.h"
#include "umc_vc1_dec_task.h"
#include "umc_vc1_dec_time_statistics.h"
#include "umc_vc1_dec_intens_comp_tbl.h"
#include "umc_vc1_video_decoder.h"



typedef void (*ExpandIntens)(VC1Context* pContext, Frame* pFrame);

static const ExpandIntens ExpandIntens_table[] =
             {
               (ExpandIntens)(ExpandFrame_Adv),
               (ExpandIntens)(ExpandFrame_Interlace),
               (ExpandIntens)(ExpandFrame_Interlace)
             };
static const Ipp32u FrameReadyTable[9] = {0, 0x5,     0x55,     0x555,     0x5555,
                                          0x55555, 0x555555, 0x5555555, 0x55555555};

namespace UMC
{
    VC1TaskStore::VC1TaskStore(MemoryAllocator *pMemoryAllocator):m_pCommonQueue(NULL),
                                                                  m_pAdditionalQueue(NULL),
                                                                  m_iConsumerNumber(0),
                                                                  m_pTasksInQueue(NULL),
                                                                  m_iCurrentTaskID(0),
                                                                  m_pSlicesInQueue(NULL),
                                                                  m_pDescriptorQueue(NULL),
                                                                  m_pOutFrameQueue(NULL),
                                                                  m_iNumFramesProcessing(0),
                                                                  m_iNumDSActiveinQueue(0),
                                                                  pMainVC1Decoder(NULL),
                                                                  m_lNextFrameCounter(1),
                                                                  m_lNextRefFrameCounter(1),
                                                                  m_lFirstRefFrameCounter(1),
                                                                  m_iNumDecodeRefFrames(0),
                                                                  m_pPrefDS(NULL),
                                                                  m_pDSIndicate(NULL),
                                                                  m_pDSIndicateSwap(NULL),
                                                                  m_bNeedToCompField(false),
                                                                  m_iICompFieldIndex(0),
                                                                  m_bIsNeedToDecode(true),
                                                                  m_eStreamDef(VC1_HD_STREAM),
                                                                  m_pMemoryAllocator(NULL),
                                                                  m_CurrIndex(-1),
                                                                  m_PrevIndex(-1),
                                                                  m_NextIndex(-1),
                                                                  m_BFrameIndex(-1),
                                                                  m_iICIndex(-1),
                                                                  m_pMainQueueTasksState(NULL),
                                                                  m_pAdditionaQueueTasksState(NULL),
                                                                  m_iIntStructID((MemID)0),
                                                                  m_iTSHeapID((MemID)0),
                                                                  m_pSHeap(NULL),
                                                                  m_FrameType(NONE_PICTURE)
    {
        m_pMemoryAllocator = pMemoryAllocator;
    }

    VC1TaskStore::~VC1TaskStore()
    {
        Ipp32u i;

        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            m_pGuardGet[i]->~Mutex();
            m_pGuardAdd[i]->~Mutex();
        }
        for (i = 0; i <  m_iConsumerNumber; i++)
            m_eWaiting[i]->~Event();

        if(m_pMemoryAllocator)
        {
            for (i = 0; i < m_iNumFramesProcessing; i++)
                m_pDescriptorQueue[i]->Release();

            if (m_iIntStructID != 0)
            {
                m_pMemoryAllocator->Unlock(m_iIntStructID);
                m_pMemoryAllocator->Free(m_iIntStructID);
                m_iIntStructID = (MemID)0;
            }

            if (m_iTSHeapID != 0)
            {
                m_pMemoryAllocator->Unlock(m_iTSHeapID);
                m_pMemoryAllocator->Free(m_iTSHeapID);
                m_iTSHeapID = (MemID)0;
            }
        }
    }

   bool VC1TaskStore::Init(Ipp32u iConsumerNumber,
                           Ipp32u iMaxFramesInParallel,
                           VC1VideoDecoder* pVC1Decoder)
    {
        Ipp32u i;
        m_iNumDSActiveinQueue = 0;
        m_iICompFieldIndex   =  iMaxFramesInParallel + VC1NUMREFFRAMES -1;
        pMainVC1Decoder = pVC1Decoder;
        m_iConsumerNumber = iConsumerNumber;
        m_iNumFramesProcessing = iMaxFramesInParallel;

        if (!m_pDSIndicate)
        {
            Ipp8u* ptr = NULL;
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32u)*64);
            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32u)*64);
            ptr += align_value<Ipp32u>(sizeof(VC1TSHeap));

            if (m_pMemoryAllocator->Alloc(&m_iIntStructID,
                (size_t)ptr,
                UMC_ALLOC_PERSISTENT,
                16) != UMC_OK)
                return false;

            m_pDSIndicate = (Ipp32s*)(m_pMemoryAllocator->Lock(m_iIntStructID));
            memset(m_pDSIndicate,0,size_t(ptr));
            ptr = (Ipp8u*)m_pDSIndicate;

            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            m_pDSIndicateSwap = (Ipp32s*)ptr;

            ptr +=  align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            m_pTasksInQueue = (Ipp32u*)ptr;

            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            m_pSlicesInQueue = (Ipp32u*)ptr;

            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32s));
            m_pMainQueueTasksState = (Ipp32u*)ptr;

            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32u)*64);
            m_pAdditionaQueueTasksState = (Ipp32u*)ptr;

            ptr += align_value<Ipp32u>(m_iNumFramesProcessing*sizeof(Ipp32u)*64);
            m_pSHeap = (VC1TSHeap*)ptr;

        }
        //CalculateHeapSize();


// New Allocation
        {
            // Heap Allocation
            {
                Ipp32u heapSize = CalculateHeapSize();

                if (m_pMemoryAllocator->Alloc(&m_iTSHeapID,
                    heapSize,//250000*iConsumerNumber,
                    UMC_ALLOC_PERSISTENT,
                    16) != UMC_OK)
                    return false;

                m_pSHeap = new(m_pSHeap) VC1TSHeap((Ipp8u*)m_pMemoryAllocator->Lock(m_iTSHeapID),heapSize);
            }

            m_pSHeap->s_new(&m_pGuardGet,m_iNumFramesProcessing);
            m_pSHeap->s_new(&m_pGuardAdd,m_iNumFramesProcessing);
            m_pSHeap->s_new(&m_pCommonQueue,m_iNumFramesProcessing);
            m_pSHeap->s_new(&m_pAdditionalQueue,m_iNumFramesProcessing);

            for (i = 0; i < m_iNumFramesProcessing; i++)
            {
                m_pDSIndicate[i] = i;
                m_pSHeap->s_new(&m_pCommonQueue[i],VC1SLICEINPARAL);
                m_pSHeap->s_new(&m_pAdditionalQueue[i],VC1SLICEINPARAL);
                m_pSHeap->s_new(&m_pGuardGet[i]);
                m_pSHeap->s_new(&m_pGuardAdd[i]);
            }
        }
        CreateTaskQueues();

        m_mDSGuard.Reset();
        m_pSHeap->s_new(&m_eWaiting,iConsumerNumber);
        // initilaize event(s)
        for (i = 0; i < iConsumerNumber; i += 1)
        {
            {
                m_pSHeap->s_new(&(m_eWaiting[i]));
                if (UMC_OK != m_eWaiting[i]->Init(0, 0))
                    return false;
            }
        }
        return true;
    }

    void VC1TaskStore::SetDefinition(VC1SequenceLayerHeader*    seqLayerHeader)
    {
        if ((seqLayerHeader->heightMB*seqLayerHeader->widthMB*16*16) > (1280*720) )
        {
            m_eStreamDef = VC1_HD_STREAM;
        }
        else if ((seqLayerHeader->heightMB*seqLayerHeader->widthMB*16*16) > (320*240))
        {
            m_eStreamDef = VC1_MD_STREAM;
        }
        else
            m_eStreamDef = VC1_SD_STREAM;


    }
    void VC1TaskStore::Reset()
    {
        m_iNumDecodeRefFrames = 0;
        m_lNextRefFrameCounter = 1;
        m_lFirstRefFrameCounter = 1;
        m_CurrIndex = -1;
        m_PrevIndex = -1;
        m_NextIndex = -1;
        m_BFrameIndex = -1;
        m_iICIndex = -1;

        for (Ipp32u i = 0; i < m_iConsumerNumber; i += 1)
        {
            Event *pEvent = m_eWaiting[i];

            if (pEvent)
                pEvent->Reset();
        }

    }

    inline bool VC1TaskStore::IsMainTaskPrepareForProcess(Ipp32u FrameID, Ipp32u TaskID)
    {
        return ( ((m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)] >> ((TaskID%8) << 2) )&0x7) == 0x4);

    }
    inline bool VC1TaskStore::IsAdditionalTaskPrepareForProcess(Ipp32u FrameID, Ipp32u TaskID)
    {
        return ( ((m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)] >> ((TaskID%8) << 2) )&0x7) == 0x4);
    }
    inline bool VC1TaskStore::IsFrameReadyToDisplay(Ipp32u FrameID)
    {
        Ipp32u NumDwordsForTasks = m_pTasksInQueue[FrameID]/8 + 1;
        Ipp32u RemainTasksMask = FrameReadyTable[m_pTasksInQueue[FrameID]%8];
        Ipp32u i;
        for (i = (FrameID << 6); i < (FrameID << 6) + NumDwordsForTasks - 1; i++)
        {
            if ((FrameReadyTable[8] != m_pMainQueueTasksState[i])||
                (FrameReadyTable[8] != m_pAdditionaQueueTasksState[i]))
                return false;
        }
        if ((RemainTasksMask == (m_pMainQueueTasksState[i])&&
           (RemainTasksMask == (m_pAdditionaQueueTasksState[i]))))
           return true;
        else
            return false;

    }
    // main queue processing
    inline void VC1TaskStore::SetTaskAsReady_MQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x4 << ((TaskID%8) << 2);

    }
    inline void VC1TaskStore::SetTaskAsNotReady_MQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword &= 0xFFFFFFFF - (0x4 << ((TaskID%8) << 2));

    }
    inline void VC1TaskStore::SetTaskAsDone_MQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x1 << ((TaskID%8) << 2);
    }
    inline void VC1TaskStore::EnableProcessBit_MQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x2 << ((TaskID%8) << 2);
    }
    inline void VC1TaskStore::DisableProcessBit_MQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pMainQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword &= 0xFFFFFFFF - (0x2 << ((TaskID%8) << 2));
    }

    // additional queue processing
    inline void VC1TaskStore::SetTaskAsReady_AQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x4 << ((TaskID%8) << 2);

    }
    inline void VC1TaskStore::SetTaskAsNotReady_AQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword &= 0xFFFFFFFF - (0x4 << ((TaskID%8) << 2));

    }
    inline void VC1TaskStore::SetTaskAsDone_AQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x1 << ((TaskID%8) << 2);
    }
    inline void VC1TaskStore::EnableProcessBit_AQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword |= 0x2 << ((TaskID%8) << 2);
    }
    inline void VC1TaskStore::DisableProcessBit_AQ(Ipp32u FrameID, Ipp32u TaskID)
    {
        Ipp32u *pCurrentDword = &m_pAdditionaQueueTasksState[(FrameID <<6) + (TaskID/8)];
        *pCurrentDword &= 0xFFFFFFFF - (0x2 << ((TaskID%8) << 2));
    }

    Ipp32u VC1TaskStore::CalculateHeapSize()
    {

        Ipp32u Size = 0;
        Ipp32u counter = 0;
        Ipp32u counter2 = 0;

        Size += align_value<Ipp32u>(sizeof(Mutex**)*m_iNumFramesProcessing); //m_pGuardGet
        Size += align_value<Ipp32u>(sizeof(Mutex**)*m_iNumFramesProcessing); //m_pGuardAdd
        Size += align_value<Ipp32u>(sizeof(VC1Task***)*m_iNumFramesProcessing); //m_pCommonQueue
        Size += align_value<Ipp32u>(sizeof(VC1Task***)*m_iNumFramesProcessing); //m_pAdditionalQueue

        Size += align_value<Ipp32u>(sizeof(VC1FrameDescriptor*)*(m_iNumFramesProcessing));

        for (counter = 0; counter < m_iNumFramesProcessing; counter++)
        {
            Size += align_value<Ipp32u>(sizeof(VC1Task**)*VC1SLICEINPARAL); //m_pCommonQueue
            Size += align_value<Ipp32u>(sizeof(VC1Task**)*VC1SLICEINPARAL); //m_pAdditionalQueue
            Size += align_value<Ipp32u>(sizeof(Mutex)); //m_pGuardGet
            Size += align_value<Ipp32u>(sizeof(Mutex)); //m_pGuardAdd
            Size += align_value<Ipp32u>(sizeof(VC1FrameDescriptor));
        }

        for (counter=0; counter < m_iNumFramesProcessing; counter++)
        {
            for (counter2 = 0; counter2 < VC1SLICEINPARAL; counter2++)
            {
                Size += align_value<Ipp32u>(sizeof(VC1Task)); //m_pCommonQueue
                Size += align_value<Ipp32u>(sizeof(SliceParams));
                Size += align_value<Ipp32u>(sizeof(VC1Task)); //m_pAdditionalQueue
                Size += align_value<Ipp32u>(sizeof(SliceParams));
            }
        }

        Size += align_value<Ipp32u>(sizeof(Event**)*m_iConsumerNumber);

        for (counter = 0; counter < m_iConsumerNumber; counter++)
        {
            Size += align_value<Ipp32u>(sizeof(Event));
        }

        Size += align_value<Ipp32u>(sizeof(VC1FrameIndexProc**)*(m_iNumFramesProcessing+2));

        for (counter = 0; counter < m_iNumFramesProcessing+2; counter++)
        {
            Size += align_value<Ipp32u>(sizeof(VC1FrameIndexProc));
        }



        return Size;
    }
    VC1PictureLayerHeader* VC1TaskStore::GetFirstInSecondField(Ipp32u qID)
    {
        for (Ipp32u i = 0; i < m_pTasksInQueue[qID]; i += 1)
        {
            if ((m_pCommonQueue[qID])[i]->m_isFirstInSecondSlice)
                return (m_pCommonQueue[qID])[i]->m_pSlice->m_picLayerHeader;
        }
        return NULL;
    }
    bool VC1TaskStore::CreateTaskQueues()
    {
        Ipp32u i,j;
        Ipp8u* pBuf;
        for (j=0; j < m_iNumFramesProcessing; j++)
        {
            for (i=0; i < VC1SLICEINPARAL; i++)
            {
                pBuf = m_pSHeap->s_alloc<VC1Task>();
                if (pBuf)
                {
                    m_pCommonQueue[j][i] = new(pBuf) VC1Task(0,i);
                }
                m_pSHeap->s_new(&(m_pCommonQueue[j][i]->m_pSlice));
                memset(m_pCommonQueue[j][i]->m_pSlice, 0, sizeof(SliceParams));


                pBuf = m_pSHeap->s_alloc<VC1Task>();
                if (pBuf)
                {
                    m_pAdditionalQueue[j][i] = new(pBuf) VC1Task(0,i);
                }
                m_pSHeap->s_new(&(m_pAdditionalQueue[j][i]->m_pSlice));
                memset(m_pAdditionalQueue[j][i]->m_pSlice, 0, sizeof(SliceParams));

            }
        }
        return true;

    }
    void VC1TaskStore::ReleaseTaskQueues()
    {
        Ipp32u i,j;
        for (j=0; j < m_iNumFramesProcessing; j++)
        {
            for (i=0; i < VC1SLICEINPARAL; i++)
            {
                // main queue
               if ((m_pCommonQueue[j])[i])
               {
                   if ((m_pCommonQueue[j])[i]->m_pSlice)
                   {
                       delete((m_pCommonQueue[j])[i]->m_pSlice);
                       (m_pCommonQueue[j])[i]->m_pSlice = NULL;
                   }
               }

               // add queue
                   if ((m_pAdditionalQueue[j])[i])
                   {
                       if ((m_pAdditionalQueue[j])[i]->m_pSlice)
                       {
                           delete((m_pAdditionalQueue[j])[i]->m_pSlice);
                           (m_pAdditionalQueue[j])[i]->m_pSlice = NULL;
                       }
                   }

            }
        }
    }


    bool VC1TaskStore::AddSampleTask(VC1Task* _pTask, Ipp32u qID)
    {
        Ipp32u widthMB = m_pDescriptorQueue[qID]->m_pContext->m_seqLayerHeader->widthMB;
//        static Ipp32u task_counter=0;
        Ipp16u curMBrow = _pTask->m_pSlice->MBStartRow;

        if (0 == curMBrow)
        {
            m_pTasksInQueue[qID] = 0;
            m_pSlicesInQueue[qID] = 0;
            memset(m_pMainQueueTasksState + (qID << 6),0,64*sizeof(Ipp32u));
            memset(m_pAdditionaQueueTasksState + (qID << 6),0,64*sizeof(Ipp32u));
        }


        bool isFirstSlieceDecodeTask = true;
        //VM_ASSERT(qID < m_iConsumerNumber);

        while (curMBrow  < _pTask->m_pSlice->MBEndRow)
        {
            VC1Task* pTask = (m_pCommonQueue[qID])[m_pTasksInQueue[qID]];
            pTask->m_eTasktype = VC1Decode;
            pTask->m_isFirstInSecondSlice = false;
            pTask->m_isFieldReady = false;

            memset(pTask->m_pSlice, 0, sizeof(SliceParams));

            _pTask->m_pSlice->is_NewInSlice = isFirstSlieceDecodeTask;

            if (isFirstSlieceDecodeTask)
            {
                if (m_pSlicesInQueue[qID] < VC1SLICEINPARAL)
                {
                    //pTask->m_bIsReady = true;
                    SetTaskAsReady_MQ(qID,m_pTasksInQueue[qID]);

                }

                isFirstSlieceDecodeTask = false;

                if (_pTask->m_isFirstInSecondSlice)
                {
                    m_pDescriptorQueue[qID]->m_iActiveTasksInFirstField = m_pTasksInQueue[qID] - 1;
                    pTask->m_isFirstInSecondSlice = _pTask->m_isFirstInSecondSlice;
                }
            }

            pTask->m_isFieldReady = _pTask->m_isFieldReady;

            *pTask->m_pSlice = *_pTask->m_pSlice;
            pTask->m_pSlice->MBStartRow = curMBrow;
            pTask->m_pSlice->MBEndRow = curMBrow + VC1MBQUANT;
            pTask->m_pSlice->MBRowsToDecode = VC1MBQUANT;
            pTask->pMulti = &VC1TaskProcessorUMC::VC1Decoding;


            if ((pTask->m_pSlice->MBRowsToDecode + pTask->m_pSlice->MBStartRow) > _pTask->m_pSlice->MBEndRow)
            {
                pTask->m_pSlice->MBEndRow = _pTask->m_pSlice->MBEndRow;
                pTask->m_pSlice->MBRowsToDecode = pTask->m_pSlice->MBEndRow - pTask->m_pSlice->MBStartRow;
            }
            (m_pCommonQueue[qID])[m_pTasksInQueue[qID]] = pTask;
            (m_pCommonQueue[qID])[m_pTasksInQueue[qID]]->m_pBlock = m_pDescriptorQueue[qID]->m_pDiffMem + widthMB*pTask->m_pSlice->MBStartRow*8*8*6;
            (m_pCommonQueue[qID])[m_pTasksInQueue[qID]]->m_pSlice->is_LastInSlice = false;
            ++m_pTasksInQueue[qID];
            curMBrow += VC1MBQUANT;
            ++m_iCurrentTaskID;
        }
        (m_pCommonQueue[qID])[m_pTasksInQueue[qID]-1]->m_pSlice->is_LastInSlice = true;
        ++m_pSlicesInQueue[qID];

       return true;
    }
    void VC1TaskStore::DistributeTasks(Ipp32u qID)
    {
        for (Ipp32u i = 0; i < m_pTasksInQueue[qID]; i++)
        {
            VC1Task* pTask = (m_pAdditionalQueue[qID])[i];
            pTask->m_isFirstInSecondSlice = false;
            pTask->m_isFieldReady = false;

            memset(pTask->m_pSlice, 0, sizeof(SliceParams));
            *(pTask->m_pSlice) = *((m_pCommonQueue[qID])[i]->m_pSlice);

            pTask->m_isFirstInSecondSlice = (m_pCommonQueue[qID])[i]->m_isFirstInSecondSlice;


            if (pTask->m_pSlice->is_NewInSlice)
                pTask->m_pSlice->iPrevDblkStartPos = -1;
            else
                pTask->m_pSlice->iPrevDblkStartPos = (m_pCommonQueue[qID])[i-1]->m_pSlice->MBStartRow-1;
            if (i > 0)
            {
                if ((m_pCommonQueue[qID])[(i-1)]->m_pSlice->is_NewInSlice)
                {
                    pTask->m_pSlice->iPrevDblkStartPos += 1;
                }
            }

            (m_pAdditionalQueue[qID])[i] = pTask;
            (m_pAdditionalQueue[qID])[i]->m_pBlock = (m_pCommonQueue[qID])[i]->m_pBlock;
            (m_pAdditionalQueue[qID])[i]->m_isFieldReady = (m_pCommonQueue[qID])[i]->m_isFieldReady;
            (m_pAdditionalQueue[qID])[i]->m_isFirstInSecondSlice = (m_pCommonQueue[qID])[i]->m_isFirstInSecondSlice;

            switch((m_pCommonQueue[qID])[i]->m_pSlice->m_picLayerHeader->PTYPE)
            {
            case VC1_B_FRAME:
                (m_pAdditionalQueue[qID])[i]->pMulti = &VC1TaskProcessorUMC::VC1MVCalculation;
                (m_pAdditionalQueue[qID])[i]->m_eTasktype = VC1MVCalculate;
                break;
            case VC1_P_FRAME:
                (m_pAdditionalQueue[qID])[i]->pMulti = &VC1TaskProcessorUMC::VC1MotionCompensation;
                (m_pAdditionalQueue[qID])[i]->m_eTasktype = VC1MC;
                break;
            case VC1_I_FRAME:
            case VC1_BI_FRAME:
                (m_pAdditionalQueue[qID])[i]->pMulti = &VC1TaskProcessorUMC::VC1PrepPlane;
                (m_pAdditionalQueue[qID])[i]->m_eTasktype = VC1PreparePlane;
                break;
            default:
                break;
            }
            ++m_iCurrentTaskID;
        }
    }

    bool VC1TaskStore::GetNextTask(VC1FrameDescriptor **pFrameDS, VC1Task** pTask, Ipp32u qID)
    {
        if (m_iConsumerNumber <= 2)
        {
                // intensity compensation and H/W accelerator are special cases
                if (pMainVC1Decoder->m_pContext->m_bIntensityCompensation)
                {
                    return GetNextTaskManyCPU_HD(pFrameDS,pTask,qID);
                }
                else
                    return GetNextTaskDualCPU(pFrameDS,pTask,qID);
        }
        else if (m_iConsumerNumber <= 4)
        {
            if ((m_eStreamDef == VC1_HD_STREAM)||(m_eStreamDef == VC1_SD_STREAM))
            {
                return GetNextTaskManyCPU_HD(pFrameDS,pTask,qID);
            }
            else
                return GetNextTaskManyCPU_MD(pFrameDS,pTask,qID);
        }
        else
            return GetNextTaskManyCPU_HD(pFrameDS,pTask,qID);

    }
    bool VC1TaskStore::GetNextTaskDualCPU(VC1FrameDescriptor **pFrameDS, VC1Task** pTask, Ipp32u qID)
    {
STATISTICS_START_TIME(m_timeStatistics->GetNextTask_StartTime);

        Ipp32u curFrame = qID;
        Ipp32u frameCount;
        Ipp32u i;
        for (Ipp32u count = 0; count < m_iNumFramesProcessing; count++) // for 2-core CPU
        {
            frameCount = m_pDSIndicate[curFrame];
STATISTICS_START_TIME(m_timeStatistics->GetNextTask_StartTime);
            {
            AutomaticMutex guard(*m_pGuardGet[frameCount]);

            //find in own queue
            if (m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess)
            {
                *pFrameDS = m_pDescriptorQueue[frameCount];
                // find in main queue
                for (i =0; i < m_pTasksInQueue[frameCount];i++ )
                {
                    if (IsMainTaskPrepareForProcess(frameCount,i))
                    {
                            *pTask = (m_pCommonQueue[frameCount])[i];
                            //(m_pCommonQueue[frameCount])[i]->m_bInProgress = true;

                            EnableProcessBit_MQ(frameCount,i);

                            STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1GetTask);
                            STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                                m_timeStatistics->GetNextTask_EndTime,
                                m_timeStatistics->GetNextTask_TotalTime);
                            return true;
                    }
                    else if (IsAdditionalTaskPrepareForProcess(frameCount,i))
                    {
                        *pTask = (m_pAdditionalQueue[frameCount])[i];
                        //(m_pAdditionalQueue[frameCount])[i]->m_bInProgress = true;
                        EnableProcessBit_AQ(frameCount,i);

STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1AddPerfomrd);
STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                    m_timeStatistics->GetNextTask_EndTime,
                    m_timeStatistics->GetNextTask_TotalTime);
                        return true;
                    }
                }
                if (IsFrameReadyToDisplay(frameCount))
                {
                    m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess = false;
                    m_pDescriptorQueue[frameCount]->m_bIsReadyToDisplay = true;
                    *pTask = NULL;
                    return true;
                }
            }
            guard.Unlock();
            }

            ++curFrame;
            if (curFrame == m_iNumFramesProcessing)
                curFrame = 0;

        }
        STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                            m_timeStatistics->GetNextTask_EndTime,
                            m_timeStatistics->GetNextTask_TotalTime);

        //return false;
        if (0 != qID) // main thread should check ready FrameDS permanentley
        {
            if (m_bIsNeedToDecode)
            {
                m_eWaiting[qID]->Reset(); //!!!!!!
                m_eWaiting[qID]->Wait();
                *pTask = NULL;
                return true;
            }
            else
                return false;
        }
        *pTask = NULL;
        return true;
    }

    bool VC1TaskStore::GetNextTaskManyCPU_HD(VC1FrameDescriptor **pFrameDS, VC1Task** pTask, Ipp32u qID)
    {
        STATISTICS_START_TIME(m_timeStatistics->GetNextTask_StartTime);

        Ipp32u StartFrame = (m_iConsumerNumber <= 2)?0:1;
        Ipp32s frameCount = m_pPrefDS->m_iSelfID;


        Ipp32u i =0;
        // Get Task from First frame in Queue
        if (((qID<(m_iConsumerNumber>>1)))||(m_iConsumerNumber <= 2))
        {
            AutomaticMutex guard(*m_pGuardGet[frameCount]);
            if (m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess)
            {
                *pFrameDS = m_pDescriptorQueue[frameCount];
                // find in main queue
                for (i =0; i < m_pTasksInQueue[frameCount];i++ )
                {
                    if (IsMainTaskPrepareForProcess(frameCount,i))
                    {
                        *pTask = (m_pCommonQueue[frameCount])[i];
                        EnableProcessBit_MQ(frameCount,i);
                        //(m_pCommonQueue[frameCount])[i]->m_bInProgress = true;
                        //(m_pCommonQueue[frameCount])[i]->m_bIsReady = true;

                        STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1GetTask);
                        STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                            m_timeStatistics->GetNextTask_EndTime,
                            m_timeStatistics->GetNextTask_TotalTime);
                        return true;
                    }
                    else if (IsAdditionalTaskPrepareForProcess(frameCount,i))
                    {
                        *pTask = (m_pAdditionalQueue[frameCount])[i];
                        EnableProcessBit_AQ(frameCount,i);
                        //(m_pAdditionalQueue[frameCount])[i]->m_bInProgress = true;
                        //(m_pAdditionalQueue[frameCount])[i]->m_bIsReady = true;

                        STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1AddPerfomrd);
                        STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                            m_timeStatistics->GetNextTask_EndTime,
                            m_timeStatistics->GetNextTask_TotalTime);
                        return true;
                    }
                }
            }


            if (m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess &&
                IsFrameReadyToDisplay(frameCount))
            {
                m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess = false;
                m_pDescriptorQueue[frameCount]->m_bIsReadyToDisplay = true;
                *pTask = NULL;
                STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                    m_timeStatistics->GetNextTask_EndTime,
                    m_timeStatistics->GetNextTask_TotalTime);
                return true;
            }
            guard.Unlock();
        }

        Ipp32u curFrame = qID;
        for (Ipp32u count = StartFrame; count < m_iNumFramesProcessing; count++) // for 2-core CPU
        {
            frameCount = m_pDSIndicate[count];
            STATISTICS_START_TIME(m_timeStatistics->GetNextTask_StartTime);
            //if (curPriority == m_pDescriptorQueue[frameCount]->m_iPriority)
            {
                AutomaticMutex guard(*m_pGuardGet[frameCount]);

                //find in own queue
                if (m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess)
                {
                    *pFrameDS = m_pDescriptorQueue[frameCount];
                    // find in main queue
                    for (i =0; i < m_pTasksInQueue[frameCount];i++ )
                    {
                        if (IsMainTaskPrepareForProcess(frameCount,i))
                        {
                            *pTask = (m_pCommonQueue[frameCount])[i];
                            EnableProcessBit_MQ(frameCount,i);
                            //(m_pCommonQueue[frameCount])[i]->m_bInProgress = true;
                            //(m_pCommonQueue[frameCount])[i]->m_bIsReady = true;

                            STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1GetTask);
                            STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                                m_timeStatistics->GetNextTask_EndTime,
                                m_timeStatistics->GetNextTask_TotalTime);
                            return true;
                        }
                        else if (IsAdditionalTaskPrepareForProcess(frameCount,i))
                        {
                            *pTask = (m_pAdditionalQueue[frameCount])[i];
                            EnableProcessBit_AQ(frameCount,i);
                            //(m_pAdditionalQueue[frameCount])[i]->m_bInProgress = true;
                            //(m_pAdditionalQueue[frameCount])[i]->m_bIsReady = true;

                            STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1AddPerfomrd);
                            STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                                m_timeStatistics->GetNextTask_EndTime,
                                m_timeStatistics->GetNextTask_TotalTime);
                            return true;
                        }
                    }
                    if (IsFrameReadyToDisplay(frameCount))
                    {
                        m_pDescriptorQueue[frameCount]->m_bIsReadyToProcess = false;
                        m_pDescriptorQueue[frameCount]->m_bIsReadyToDisplay = true;
                        *pTask = NULL;
                        STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
                            m_timeStatistics->GetNextTask_EndTime,
                            m_timeStatistics->GetNextTask_TotalTime);
                        return true;
                    }
                }
                guard.Unlock();
            }
            ++curFrame;
            if (curFrame > m_iNumFramesProcessing)
                curFrame = 0;

        }
        STATISTICS_END_TIME(m_timeStatistics->GetNextTask_StartTime,
            m_timeStatistics->GetNextTask_EndTime,
            m_timeStatistics->GetNextTask_TotalTime);

        //return false;
        if (0 != qID) // main thread should check ready FrameDS permanentley
        {
            if (m_bIsNeedToDecode)
            {
                m_eWaiting[qID]->Reset(); //!!!!!!
                m_eWaiting[qID]->Wait();
                *pTask = NULL;
                return true;
            }
            else
                return false;
        }
        *pTask = NULL;
        return true;
    }
    bool VC1TaskStore::GetNextTaskManyCPU_MD(VC1FrameDescriptor **pFrameDS, VC1Task** pTask, Ipp32u qID)
    {
        //same pipeline as for Dual case
        return GetNextTaskDualCPU(pFrameDS,pTask,qID);

    }
    bool VC1TaskStore::GetNextTaskMainThread(VC1FrameDescriptor **pFrameDS, VC1Task** pTask, Ipp32u qID)
    {
        if (IsPerfomedDS())
            return false;
        GetNextTask(pFrameDS,pTask,qID);
        return true;
    }
bool VC1TaskStore::AddPerfomedTask(VC1Task* pTask, VC1FrameDescriptor *pFrameDS)
    {
STATISTICS_START_TIME(m_timeStatistics->AddPerfomed_StartTime);
         Ipp32u qID = pFrameDS->m_iSelfID;
        //AutomaticMutex guard(*m_pGuardGet[qID]);

        // check error(s)
        if ((NULL == pTask) || (0 >= m_pTasksInQueue[qID])) //!!!!!!!!!!
            return false;
        VC1TaskTypes NextStateTypeofTask;
        Ipp32u i = pTask->m_iTaskID;

        if (pTask->m_eTasktype <= VC1Reconstruct)
        {

                    //AutomaticMutex guard(*m_pGuardGet[qID]);
                    AutomaticMutex guard(*m_pGuardGet[qID]);
                    NextStateTypeofTask = (m_pCommonQueue[qID])[i]->switch_task();
                    switch (NextStateTypeofTask)
                    {
                    case VC1Reconstruct:
                        SetTaskAsReady_MQ(qID, i);
                        if ((i+1) < m_pTasksInQueue[qID])
                        {
                            if (!(m_pCommonQueue[qID])[i+1]->m_pSlice->is_NewInSlice)
                            {
                                (m_pCommonQueue[qID])[i+1]->m_pSlice->m_pstart = pTask->m_pSlice->m_pstart;
                                (m_pCommonQueue[qID])[i+1]->m_pSlice->m_bitOffset = pTask->m_pSlice->m_bitOffset;
                                (m_pCommonQueue[qID])[i+1]->m_pSlice->EscInfo = pTask->m_pSlice->EscInfo;
                                SetTaskAsReady_MQ(qID, i+1);
                            }
                        }

                        if ((m_pDescriptorQueue[qID]->m_bIsReferenceReady)&&
                            ((m_pAdditionalQueue[qID])[i]->m_isFieldReady))
                        {
                            switch((m_pAdditionalQueue[qID])[i]->m_pSlice->m_picLayerHeader->PTYPE)
                            {
                            case VC1_P_FRAME:
                                 SetTaskAsReady_AQ(qID, i);
                                break;
                            case VC1_B_FRAME:
                                if (i > 0)
                                {
                                    if (((m_pAdditionalQueue[qID])[i]->m_pSlice->is_NewInSlice)|| ((m_pAdditionalQueue[qID])[i-1]->m_eTasktype > VC1MVCalculate ))
                                        SetTaskAsReady_AQ(qID, i);
                                }
                                else
                                    SetTaskAsReady_AQ(qID, i);
                                break;
                            default:
                                break;
                            }
                        }
                        break;
                    case VC1Complete:
                        if ((m_pAdditionalQueue[qID])[i]->m_eTasktype == VC1PreparePlane)
                        {
                            (m_pAdditionalQueue[qID])[i]->m_pBlock = (m_pCommonQueue[qID])[i]->m_pBlock;
                            if (i>0)
                            {
                                if (((m_pAdditionalQueue[qID])[i-1]->m_eTasktype > VC1PreparePlane)||
                                    ((m_pCommonQueue[qID])[i]->m_pSlice->is_NewInSlice))
                                {
                                    //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                    SetTaskAsReady_AQ(qID, i);
                                }
                            }
                            else
                            {
                                //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i);

                            }

                        }
                        //(m_pCommonQueue[qID])[i]->m_bDone = true;
                        SetTaskAsDone_MQ(qID, i);
                        //printf("finish %d task \n", i);
                        break;
                    default:
                        VM_ASSERT(0);

                    }
                    //printf("complete %d task \n", i);
STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1AddPerfomrd);

                    //(*m_pCommonQueue[qID])[i]->m_bInProgress = false;
                    DisableProcessBit_MQ(qID,i);

STATISTICS_END_TIME(m_timeStatistics->AddPerfomed_StartTime,
                    m_timeStatistics->AddPerfomed_EndTime,
                    m_timeStatistics->AddPerfomed_TotalTime);
                    guard.Unlock();
                    WakeUP();
                    return true;
        }
        else
        {
                    AutomaticMutex guard(*m_pGuardGet[qID]);
                    NextStateTypeofTask = (m_pAdditionalQueue[qID])[i]->switch_task();
                    switch (NextStateTypeofTask)
                    {
                    case VC1MC:
                        //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                        SetTaskAsReady_AQ(qID, i);
                        if ((i+1) < m_pTasksInQueue[qID])
                        {
                            if ( ((m_pCommonQueue[qID])[i+1]->m_eTasktype > VC1Decode)&&
                                 (!(m_pCommonQueue[qID])[i+1]->m_pSlice->is_NewInSlice)&&
                                ((m_pAdditionalQueue[qID])[i+1]->m_isFieldReady))
                            {
                                //(m_pAdditionalQueue[qID])[i+1]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i+1);
                            }
                        }
                        break;
                    case VC1PreparePlane:
                        if ((VC1Complete == (m_pCommonQueue[qID])[i]->m_eTasktype) )
                        {
                            (m_pAdditionalQueue[qID])[i]->m_pBlock = (m_pCommonQueue[qID])[i]->m_pBlock;
                            if (i > 0)
                            {
                                if ( ((m_pAdditionalQueue[qID])[i-1]->m_eTasktype > VC1PreparePlane)||
                                    ((m_pCommonQueue[qID])[i]->m_pSlice->is_NewInSlice))
                                {

                                    //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                    SetTaskAsReady_AQ(qID, i);
                                }

                                else
                                {
                                    //(m_pAdditionalQueue[qID])[i]->m_bIsReady = false;
                                    SetTaskAsNotReady_AQ(qID, i);
                                }

                            }
                            else
                            {
                                //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i);
                            }

                        } else
                        {
                            SetTaskAsNotReady_AQ(qID, i);
                        }

                        break;
                    case VC1Deblock:

                        if ((i+1) < m_pTasksInQueue[qID])
                        {
                            if (((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1PreparePlane)&&
                                ((m_pCommonQueue[qID])[i+1]->m_eTasktype == VC1Complete)&&
                                (!(m_pCommonQueue[qID])[i+1]->m_pSlice->is_NewInSlice))
                            {
                                SetTaskAsReady_AQ(qID, i+1);
                            }


                        }

                        if (pTask->m_pSlice->m_picLayerHeader->FCM == VC1_FrameInterlace) // In case of Interlace Frames slices are depends
                        {
                            Ipp8u DeblockMask = 0;  // 0 bit - Down edge, 1 bit - UP edge
                            if (i > 0)
                            {
                                if ((m_pAdditionalQueue[qID])[i]->m_pSlice->is_LastInSlice)
                                {
                                    //if ((m_pAdditionalQueue[qID])[i+1])
                                    if ((i+1) < m_pTasksInQueue[qID])
                                    {
                                        if ((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock)
                                            DeblockMask |= 1;
                                    } else
                                        DeblockMask |= 1;
                                } else
                                    DeblockMask |= 1;

                                if ((m_pAdditionalQueue[qID])[i-1]->m_eTasktype == VC1Complete)
                                    DeblockMask |= 2;

                                if ((m_pAdditionalQueue[qID])[i]->m_pSlice->is_NewInSlice)
                                {
                                    if (i > 1)
                                    {
                                        if (((m_pAdditionalQueue[qID])[i-1]->m_eTasktype == VC1Deblock)&&
                                            ((m_pAdditionalQueue[qID])[i-2]->m_eTasktype == VC1Complete))
                                        {
                                            //(m_pAdditionalQueue[qID])[i-1]->m_bIsReady = true;
                                            SetTaskAsReady_AQ(qID, i-1);
                                        }
                                    }
                                    else if ((m_pAdditionalQueue[qID])[i-1]->m_eTasktype == VC1Deblock)
                                    {
                                         //(m_pAdditionalQueue[qID])[i-1]->m_bIsReady = true;
                                         SetTaskAsReady_AQ(qID, i-1);
                                    }
                                }


                            } else
                            {
                                if ((m_pAdditionalQueue[qID])[i]->m_pSlice->is_LastInSlice)
                                {
                                    if ((i+1) < m_pTasksInQueue[qID])
                                    {
                                        if ((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock)
                                            DeblockMask = 3;
                                    } else
                                        DeblockMask = 3;
                                } else
                                    DeblockMask = 3;

                            }

                            if (3 == DeblockMask)
                            {
                                //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i);
                            }
                            else
                            {
                                //(m_pAdditionalQueue[qID])[i]->m_bIsReady = false;
                                SetTaskAsNotReady_AQ(qID, i);
                            }
                        }
                        else
                        {
                            if (i > 0)
                            {
                                if ((m_pAdditionalQueue[qID])[i-1]->m_eTasktype == VC1Complete)//||
                                    //((m_pAdditionalQueue[qID])[i]->m_pSlice->is_NewInSlice))
                                {
                                    //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                    SetTaskAsReady_AQ(qID, i);
                                }
                                else
                                {
                                    //(m_pAdditionalQueue[qID])[i]->m_bIsReady = false;
                                    SetTaskAsNotReady_AQ(qID, i);
                                }
                            }
                            else
                            {
                                //(m_pAdditionalQueue[qID])[i]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i);
                            }
                        }
                        break;
                    case VC1Complete:
                        if (!((m_pCommonQueue[qID])[0]->IsDeblocking((m_pCommonQueue[qID])[0]->m_pSlice->slice_settings)))
                        {
                                    if ((i+1) < m_pTasksInQueue[qID])
                            {
                                if (((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1PreparePlane)&&
                                    ((m_pCommonQueue[qID])[i+1]->m_eTasktype == VC1Complete)&&
                                    (!(m_pCommonQueue[qID])[i+1]->m_pSlice->is_NewInSlice))
                                {
                                    SetTaskAsReady_AQ(qID, i+1);
                                }

                            }
                        }

                        if (pTask->m_pSlice->m_picLayerHeader->FCM == VC1_FrameInterlace) // Bad case of Interlace Frames slices are depends
                        {
                            if ((i+1) < m_pTasksInQueue[qID])
                            {
                                if ((i+2) < m_pTasksInQueue[qID])
                                {
                                    if ((m_pAdditionalQueue[qID])[i+1]->m_pSlice->is_LastInSlice)
                                    {
                                        if (((m_pAdditionalQueue[qID])[i+2]->m_eTasktype == VC1Deblock)&&
                                            ((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock))
                                        {
                                            //(m_pAdditionalQueue[qID])[i+1]->m_bIsReady = true;
                                            SetTaskAsReady_AQ(qID, i+1);
                                        }
                                    } else if ((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock)
                                    {
                                        //(m_pAdditionalQueue[qID])[i+1]->m_bIsReady = true;
                                        SetTaskAsReady_AQ(qID, i+1);
                                    }
                                }
                                else if ((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock)
                                {
                                    //(m_pAdditionalQueue[qID])[i+1]->m_bIsReady = true;
                                    SetTaskAsReady_AQ(qID, i+1);
                                }
                            }

                        }
                        else if ((i+1) < m_pTasksInQueue[qID])
                        {
                            if (((m_pAdditionalQueue[qID])[i+1]->m_eTasktype == VC1Deblock)&&
                                //(!(m_pAdditionalQueue[qID])[i+1]->m_pSlice->is_NewInSlice))
                                ((((m_pAdditionalQueue[qID])[i+1]->m_isFieldReady)) ||
                                VC1_IS_NOT_PRED((m_pAdditionalQueue[qID])[i+1]->m_pSlice->m_picLayerHeader->PTYPE)) )
                            {
                                //(m_pAdditionalQueue[qID])[i+1]->m_bIsReady = true;
                                SetTaskAsReady_AQ(qID, i+1);
                            }
                            //if ((m_pAdditionalQueue[qID])[i+1]->m_isFirstInSecondSlice)
                            if ((m_pDescriptorQueue[qID]->m_pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)&&
                                (m_pDescriptorQueue[qID]->m_iActiveTasksInFirstField == 0))
                            {
                                Ipp32u count;
                                VC1Context* pContext = m_pDescriptorQueue[qID]->m_pContext;
                                VC1PictureLayerHeader * picLayerHeader = GetFirstInSecondField(qID);
                                if (picLayerHeader)
                                {
                                    if ((pContext->m_bIntensityCompensation)&&
                                        (picLayerHeader->PTYPE == VC1_P_FRAME)&&
                                        (m_pDescriptorQueue[qID]->m_bIsReferenceReady))
                                    {
                                        UpdateICTablesForSecondField(m_pDescriptorQueue[qID]->m_pContext);
                                    }
                                    else
                                    {
                                        if ((VC1_IS_REFERENCE(picLayerHeader->PTYPE)&&
                                            (m_pDescriptorQueue[qID]->m_bIsReferenceReady)))
                                            pContext->m_bIntensityCompensation = 0;

                                    }
                                }

                                if ((m_pAdditionalQueue[qID])[i+1]->m_pSlice->m_picLayerHeader->PTYPE == VC1_P_FRAME)
                                {
                                    for (count = i+1; count < m_pTasksInQueue[qID];count++)
                                    {
                                        if (((m_pCommonQueue[qID])[count]->m_eTasktype > VC1Decode)&&
                                            m_pDescriptorQueue[qID]->m_bIsReferenceReady)
                                        {
                                            //(m_pAdditionalQueue[qID])[count]->m_bIsReady = true;
                                            SetTaskAsReady_AQ(qID, count);
                                        }
                                        (m_pAdditionalQueue[qID])[count]->m_isFieldReady = true;
                                    }
                                }
                                // B frame. MVcalculate is a sequence task
                                else if ((m_pAdditionalQueue[qID])[i+1]->m_pSlice->m_picLayerHeader->PTYPE == VC1_B_FRAME)
                                {
                                    for (count = i+1; count < m_pTasksInQueue[qID];count++)
                                    {
                                        if ((( (m_pCommonQueue[qID])[count]->m_eTasktype > VC1Decode)&&
                                            ((m_pCommonQueue[qID])[count])->m_pSlice->is_NewInSlice)&&
                                            m_pDescriptorQueue[qID]->m_bIsReferenceReady)
                                        {
                                            //(m_pAdditionalQueue[qID])[count]->m_bIsReady = true;
                                            SetTaskAsReady_AQ(qID, count);
                                        }
                                        (m_pAdditionalQueue[qID])[count]->m_isFieldReady = true;
                                    }
                                }
                            }
                        }
                        --m_pDescriptorQueue[qID]->m_iActiveTasksInFirstField;
                        //(m_pAdditionalQueue[qID])[i]->m_bDone = true;
                        SetTaskAsDone_AQ(qID, i);
                        break;
                    default:
                        VM_ASSERT(0);
                    }
                    //printf("complete %d task \n", i);
STATISTIC_PARAL_ENTRY(pTask,pTask->m_eTasktype,VC1AddPerfomrd);

                    //(m_pAdditionalQueue[qID])[i]->m_bInProgress = false;
                    DisableProcessBit_AQ(qID,i);

STATISTICS_END_TIME(m_timeStatistics->AddPerfomed_StartTime,
                    m_timeStatistics->AddPerfomed_EndTime,
                    m_timeStatistics->AddPerfomed_TotalTime);
                    guard.Unlock();
                    WakeUP();
                    return true;
        }
    }
    bool VC1TaskStore::CreateDSQueue(VC1Context* pContext,
                                     bool IsReorder,
                                     Ipp16s* pResidBuf)
    {
        Ipp32u i;
        Ipp8u* pBuf;
        m_pSHeap->s_new(&m_pDescriptorQueue,m_iNumFramesProcessing);
        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            pBuf = m_pSHeap->s_alloc<VC1FrameDescriptor>();
            m_pDescriptorQueue[i] = new(pBuf) VC1FrameDescriptor(m_pMemoryAllocator);
            m_pDescriptorQueue[i]->Init(i,pContext,this,IsReorder,pResidBuf);
        }
        m_pPrefDS =  m_pDescriptorQueue[0];
        return true;
    }

    void VC1TaskStore::ResetDSQueue()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing; i++)
            m_pDescriptorQueue[i]->Reset();
        m_lNextFrameCounter = 1;
        m_lNextRefFrameCounter = 1;

    }
    inline bool VC1TaskStore::IsPerfomedDS()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            if ((m_pDescriptorQueue[i]->m_bIsReadyToDisplay)&&
                (m_lNextFrameCounter == m_pDescriptorQueue[i]->m_iFrameCounter))
                return true;
        }
        return false;
    }
    bool VC1TaskStore::IsProcessingDS()
    {
        //AutomaticMutex guard(m_mDSGuard);
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            if (!m_pDescriptorQueue[i]->m_bIsReadyToLoad)
                return true;
        }
        return false;
    }

    VC1FrameDescriptor* VC1TaskStore::GetLastDS()
    {
        //AutomaticMutex guard(m_mDSGuard);
        Ipp32u i;
        VC1FrameDescriptor* pCurrDescriptor = m_pDescriptorQueue[0];
        for (i = 0; i < m_iNumFramesProcessing-1; i++)
        {
            if (pCurrDescriptor->m_iFrameCounter < m_pDescriptorQueue[i+1]->m_iFrameCounter)
                pCurrDescriptor = m_pDescriptorQueue[i+1];

        }
        return pCurrDescriptor;
    }
    VC1FrameDescriptor* VC1TaskStore::GetFirstDS()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            if (m_pDescriptorQueue[i]->m_iFrameCounter == m_lNextFrameCounter)
                return m_pDescriptorQueue[i];

        }
        return NULL;
    }

    void VC1TaskStore::CreateOutBuffersQueue()
    {
        Ipp32u i;
        Ipp8u* pBuf;
        m_pSHeap->s_new(&m_pOutFrameQueue,m_iNumFramesProcessing+2);
        for(i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            pBuf = m_pSHeap->s_alloc<VC1FrameIndexProc>();
            m_pOutFrameQueue[i] = new(pBuf) VC1FrameIndexProc;
        }
    }
    void VC1TaskStore::OpenNextFrames(VC1FrameDescriptor* pDS,VC1FrameDescriptor** pPrevDS,Ipp32s* CurrRefDst, Ipp32s* CurBDst)
    {
        Ipp32u i = 0;
        bool isReadyReference = ((pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_B_FRAME)&&
                                 (pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_BI_FRAME))?(true):(false);

        if (isReadyReference)
            --(*CurrRefDst);
        else
            --(*CurBDst);

        *pPrevDS = NULL;

        // Swap frames to decode according to priorities
        ippsCopy_8u((Ipp8u*)m_pDSIndicate,(Ipp8u*)m_pDSIndicateSwap,m_iNumFramesProcessing*sizeof(Ipp32s));
        ippsCopy_8u((Ipp8u*)(m_pDSIndicateSwap+1),(Ipp8u*)m_pDSIndicate,(m_iNumFramesProcessing-1)*sizeof(Ipp32s));
        m_pDSIndicate[m_iNumFramesProcessing-1] = m_pDSIndicateSwap[0];

        m_pPrefDS = GetPreferedDS();
        //if  (!VC1_IS_REFERENCE(m_pPrefDS->m_pContext->m_picLayerHeader->PTYPE))
        //    GetNextRefDS(&m_pPrefDS);



        for (i = 0; i < m_iNumFramesProcessing; i++)
        {
            if (m_pDescriptorQueue[i]->m_bIsReadyToProcess)
            {
                if (isReadyReference)
                {
                    --(m_pDescriptorQueue[i]->m_iRefFramesDst);
                    if ((0 == m_pDescriptorQueue[i]->m_iRefFramesDst)&&
                        ((!m_pDescriptorQueue[i]->m_pContext->m_bIntensityCompensation&&
                          !pDS->m_pContext->m_seqLayerHeader->RANGERED)||
                         (0 == m_pDescriptorQueue[i]->m_iBFramesDst)))
                    {
                        WakeTasksInAlienQueue(m_pDescriptorQueue[i],pPrevDS);
                    }
                }
                else // only for BAD case of intensity compensation
                {
                    --(m_pDescriptorQueue[i]->m_iBFramesDst);
                    if ((0 == m_pDescriptorQueue[i]->m_iBFramesDst)&&
                        (0 == m_pDescriptorQueue[i]->m_iRefFramesDst)&&
                        ((m_pDescriptorQueue[i]->m_pContext->m_bIntensityCompensation)||
                         m_pDescriptorQueue[i]->m_pContext->m_seqLayerHeader->RANGERED))
                    {
                        WakeTasksInAlienQueue(m_pDescriptorQueue[i],pPrevDS);
                    }
                }
            }
            else if ((m_pDescriptorQueue[i]->m_bIsSkippedFrame)&&
                       (pDS->m_iSelfID != i))
            {
                if (isReadyReference)
                {
                    --(m_pDescriptorQueue[i]->m_iRefFramesDst);
                    if (0 == m_pDescriptorQueue[i]->m_iRefFramesDst)
                        m_pDescriptorQueue[i]->m_bIsReferenceReady = true;

                }
                if (m_pDescriptorQueue[i]->m_bIsReferenceReady)
                    //(m_pDescriptorQueue[i]->m_bIsBReady))
                {
                    m_pDescriptorQueue[i]->m_bIsReadyToDisplay = true;
                }
            }
        }
    }

    void VC1TaskStore::SetDstForFrameAdv(VC1FrameDescriptor* pDS, Ipp32s* CurrRefDst, Ipp32s* CurBDst)
    {
        bool isFrameReference = ((pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_B_FRAME)&&
                                 (pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_BI_FRAME)&&
                                 (!pDS->isSpecialBSkipFrame()))?(true):(false);

        pDS->m_iRefFramesDst = *CurrRefDst;
        pDS->m_iBFramesDst = *CurBDst;

        if ((0 == pDS->m_iRefFramesDst)&&
            ((!pDS->m_pContext->m_bIntensityCompensation)||
             (0 == pDS->m_iBFramesDst)))
        {
            pDS->m_bIsReferenceReady = true;
        }
        else
            pDS->m_bIsReferenceReady = false;

        if (((pDS->m_pContext->m_InitPicLayer->PTYPE == VC1_I_FRAME)&&
            (pDS->m_pContext->m_picLayerHeader->PTypeField2 == VC1_I_FRAME))||
            ((pDS->m_pContext->m_InitPicLayer->PTYPE == VC1_BI_FRAME)&&
            (pDS->m_pContext->m_picLayerHeader->PTypeField2 == VC1_BI_FRAME)))
            pDS->m_bIsReferenceReady = true;

        if ((pDS->m_bIsSkippedFrame)&&(pDS->m_bIsReferenceReady))
        {
            pDS->m_bIsReadyToDisplay = true;
        }

        if (pDS->m_pContext->m_bNeedToUseCompBuffer)
            m_bNeedToCompField = true;
        else if (VC1_IS_REFERENCE(pDS->m_pContext->m_picLayerHeader->PTYPE))
            m_bNeedToCompField = false;

        if (isFrameReference)
            ++(*CurrRefDst);
        else
            ++(*CurBDst);

        if (pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_SKIPPED_FRAME)
            pDS->m_bIsReadyToProcess = true;
        else
            pDS->m_bIsReadyToProcess = false;

    }
    void VC1TaskStore::SetDstForFrame(VC1FrameDescriptor* pDS, Ipp32s* CurrRefDst, Ipp32s* CurBDst)
    {
        bool isFrameReference = ((pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_B_FRAME)&&
                                 (pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_BI_FRAME)&&
                                 (!pDS->isSpecialBSkipFrame()))?(true):(false);

        pDS->m_iRefFramesDst = *CurrRefDst;
        pDS->m_iBFramesDst = *CurBDst;

        if ((0 == pDS->m_iRefFramesDst)&&
            ((!pDS->m_pContext->m_bIntensityCompensation
               &&!pDS->m_pContext->m_seqLayerHeader->RANGERED)||
             (0 == pDS->m_iBFramesDst)))
        {
            pDS->m_bIsReferenceReady = true;

            if ((pDS->m_pContext->m_seqLayerHeader->RANGERED)&&(VC1_IS_PRED(pDS->m_pContext->m_picLayerHeader->PTYPE)))
                RangeRefFrame(pDS->m_pContext);

        }
        else
            pDS->m_bIsReferenceReady = false;

        if ((pDS->m_pContext->m_InitPicLayer->PTYPE == VC1_I_FRAME)||
            (pDS->m_pContext->m_InitPicLayer->PTYPE == VC1_BI_FRAME))
            pDS->m_bIsReferenceReady = true;

        if ((pDS->m_bIsSkippedFrame)&&(pDS->m_bIsReferenceReady))
        {
            pDS->m_bIsReadyToDisplay = true;
        }

        if (isFrameReference)
            ++(*CurrRefDst);
        else
            ++(*CurBDst);

        if (pDS->m_pContext->m_picLayerHeader->PTYPE != VC1_SKIPPED_FRAME)
            pDS->m_bIsReadyToProcess = true;
        else
            pDS->m_bIsReadyToProcess = false;

    }

    void VC1TaskStore::WakeTasksInAlienQueue(VC1FrameDescriptor* pDS, VC1FrameDescriptor** pPrevDS)
    {
        AutomaticMutex guard(*m_pGuardGet[pDS->m_iSelfID]);
        pDS->m_bIsReferenceReady = true;
        if (pDS->m_pContext->m_bIntensityCompensation)
        {
            VC1Context* pContext = pDS->m_pContext;

            if ((pContext->m_picLayerHeader->PTypeField1 == VC1_P_FRAME)||
                (pContext->m_picLayerHeader->PTypeField2 == VC1_P_FRAME)||
                (pContext->m_picLayerHeader->PTYPE == VC1_P_FRAME))
            {
                *pPrevDS = pDS;
                pDS->m_bIsReferenceReady = false;
            }
        }
        if ((pDS->m_pContext->m_seqLayerHeader->RANGERED)&&
            (VC1_IS_PRED(pDS->m_pContext->m_picLayerHeader->PTYPE)))
        {
            *pPrevDS = pDS;
            pDS->m_bIsReferenceReady = false;
        }
        if (pDS->m_bIsReferenceReady)
        {
            for(Ipp32u i =0; i < m_pTasksInQueue[pDS->m_iSelfID]; i++)
            {
                if ( ((m_pCommonQueue[pDS->m_iSelfID])[i]->m_eTasktype > VC1Decode )&&
                    ((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_isFieldReady))
                {
                    if (((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_eTasktype == VC1MVCalculate)&&
                        ( (m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_pSlice->is_NewInSlice))
                    {
                        //(m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_bIsReady = true;
                        SetTaskAsReady_AQ(pDS->m_iSelfID, i);
                    }

                    else if ((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_eTasktype == VC1MC )
                    {
                        //(m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_bIsReady = true;
                        SetTaskAsReady_AQ(pDS->m_iSelfID, i);
                    }
                }
            }
        //}
    }

    }
    void VC1TaskStore::WakeTasksInAlienQueue(VC1FrameDescriptor* pDS)
    {
        AutomaticMutex guard(*m_pGuardGet[pDS->m_iSelfID]);
        // Reference
        pDS->m_bIsReferenceReady = true;

        for(Ipp32u i =0; i < m_pTasksInQueue[pDS->m_iSelfID]; i++)
        {
            if ( ((m_pCommonQueue[pDS->m_iSelfID])[i]->m_eTasktype > VC1Decode )&&
                ((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_isFieldReady))
            {
                if (((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_eTasktype == VC1MVCalculate)&&
                    ( (m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_pSlice->is_NewInSlice))
                {
                    //(m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_bIsReady = true;
                    SetTaskAsReady_AQ(pDS->m_iSelfID, i);
                }

                else if ((m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_eTasktype == VC1MC )
                {
                    //(m_pAdditionalQueue[pDS->m_iSelfID])[i]->m_bIsReady = true;
                    SetTaskAsReady_AQ(pDS->m_iSelfID, i);
                }
            }
        }
    }

    void VC1TaskStore::CompensateDSInQueue(VC1FrameDescriptor* pDS)
    {
        AutomaticMutex guard(*m_pGuardGet[pDS->m_iSelfID]);
        if (pDS->m_pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
        {
            if (pDS->m_iActiveTasksInFirstField == -1)
            {
                //VC1CompensateFrame(pDS->m_pContext,GetFirstInSecondField(pDS->m_iSelfID));
                 UpdateICTablesForSecondField(pDS->m_pContext);

            }
        }
    }
    Ipp32s VC1TaskStore::GetInternalIndex() // for B/BI frames
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if (!m_pOutFrameQueue[i]->m_bIsBusy)
            {
                m_pOutFrameQueue[i]->m_bIsBusy = true;
                return i;
            }
        }
        return -1;
    }
    Ipp32s VC1TaskStore::GetRefInternalIndex() // for I/P/Skipped frames
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if (!m_pOutFrameQueue[i]->m_bIsBusy)
            {
                m_pOutFrameQueue[i]->m_bIsBusy = true;
                m_pOutFrameQueue[i]->m_lRefCounter = m_lNextRefFrameCounter;
                ++m_lNextRefFrameCounter;
                return i;
            }
        }
        return -1;
    }
    Ipp32s VC1TaskStore::GetPrevIndex()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if (m_pOutFrameQueue[i]->m_lRefCounter == (m_lNextRefFrameCounter-1))
               return i;
        }
        return -1;

    }
    Ipp32s VC1TaskStore::GetNextIndex()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if (m_pOutFrameQueue[i]->m_lRefCounter == (m_lNextRefFrameCounter-2))
                return i;

        }
        return -1;
    }
    Ipp32s VC1TaskStore::GetDisplayIndex()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if (m_pOutFrameQueue[i]->m_lRefCounter == (m_lNextRefFrameCounter-1))
            {
                //if (m_bNeedToCompField)
                //    return m_iICompFieldIndex;
                //else
                    return i;
            }
        }
        return -1;
    }
    void   VC1TaskStore::FreeIndexQueue()
    {
        Ipp32u i;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            m_pOutFrameQueue[i]->m_lRefCounter = 1;
            m_pOutFrameQueue[i]->m_bIsBusy = false;
        }
    }
    void VC1TaskStore::FreeFirstReference()
    {
        Ipp32u i;
        ++m_iNumDecodeRefFrames;
        for (i = 0; i < m_iNumFramesProcessing+2; i++)
        {
            if ((m_pOutFrameQueue[i]->m_lRefCounter == m_lFirstRefFrameCounter)&&
                (m_iNumDecodeRefFrames > 2))
            {
                ++m_lFirstRefFrameCounter;
                m_pOutFrameQueue[i]->m_bIsBusy = false;
                return;
            }
        }
    }
    void VC1TaskStore::FreeBFrame(Ipp32u Index)
    {
        m_pOutFrameQueue[Index]->m_bIsBusy = false;
        //m_pOutFrameQueue[GetDisplayIndex()]->m_bIsBusy = false;
    }

    VC1TaskTypes VC1Task::switch_task()
    {
        switch (m_eTasktype)
        {
// type of tasks in main queue
        case VC1Decode:
            //this->m_pSlice->is_NewInSlice = false;
            m_eTasktype = VC1Reconstruct;
            pMulti = &VC1TaskProcessorUMC::VC1ProcessDiff;
            return m_eTasktype;
        //case VC1Dequant:
        //    m_eTasktype = VC1Reconstruct;
        //    pMulti = &VC1TaskProcessor::VC1ProcessDiff;
        //    return m_eTasktype;
        case VC1Reconstruct:
            m_eTasktype = VC1Complete;
            return VC1Complete;

// type of tasks in additional
        case VC1MVCalculate:
            m_eTasktype = VC1MC;
            pMulti = &VC1TaskProcessorUMC::VC1MotionCompensation;
            return m_eTasktype;
        case VC1MC:
            m_eTasktype = VC1PreparePlane;
            pMulti = &VC1TaskProcessorUMC::VC1PrepPlane;
            return m_eTasktype;
        case VC1PreparePlane:
            //this->m_pSlice->is_NewInSlice = false;
            if (IsDeblocking(m_pSlice->slice_settings))
            {
                m_eTasktype = VC1Deblock;
                pMulti = &VC1TaskProcessorUMC::VC1Deblocking;
            }
            else
                m_eTasktype = VC1Complete;
            return m_eTasktype;
        case VC1Deblock:
            m_eTasktype = VC1Complete;
            return m_eTasktype;
        default:
            VM_ASSERT(0);
        }
        return m_eTasktype;
     }

    void VC1Task::setSliceParams(VC1Context* pContext)
    {
        this->m_pSlice->slice_settings = VC1Decode ;

        if ( (pContext->m_picLayerHeader->PTYPE == VC1_P_FRAME) ||
            (pContext->m_picLayerHeader->PTYPE == VC1_B_FRAME) )
            this->m_pSlice->slice_settings |= VC1Reconstruct;

        if (pContext->m_seqLayerHeader->LOOPFILTER)
            this->m_pSlice->slice_settings |= VC1Deblock;
    }

}// namespace UMC
#endif 

