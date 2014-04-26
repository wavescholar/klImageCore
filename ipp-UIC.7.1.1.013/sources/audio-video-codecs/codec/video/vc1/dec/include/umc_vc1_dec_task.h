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

#ifndef __UMC_VC1_DEC_TASK_H_
#define __UMC_VC1_DEC_TASK_H_

#include "vm_types.h"
#include "umc_structures.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_seq.h"

#include "ippi.h"
#include "ipps.h"

namespace UMC
{
    class VC1TaskProcessorUMC;

    typedef struct
    {
        Ipp16u                   MBStartRow;
        Ipp16u                   MBEndRow;
        Ipp16u                   MBRowsToDecode;
        Ipp32u*                  m_pstart;
        Ipp32s                   m_bitOffset;
        VC1PictureLayerHeader*   m_picLayerHeader;
        VC1VLCTables*            m_vlcTbl;
        bool                     is_continue;
        Ipp32u                   slice_settings;
        VC1EscInfo               EscInfo;
        bool                     is_NewInSlice;
        bool                     is_LastInSlice;
        Ipp32s                   iPrevDblkStartPos; //need to interlace frames
    } SliceParams;

    typedef enum
    {
        VC1Decode        = 1,
        //VC1Dequant       = 2,
        VC1Reconstruct   = 2,
        VC1MVCalculate   = 4,
        VC1MC            = 8,
        VC1PreparePlane  = 16,
        VC1Deblock       = 32,
#ifdef VC1_THREAD_STATISTIC
        VC1Sleep         = 128,
        VC1WakeUp        = 256,
#endif
        VC1Complete      = 64
    } VC1TaskTypes;

#ifdef VC1_THREAD_STATISTIC
    typedef enum
    {
        _Stat_VC1Decode        = 0,
        _Stat_VC1Dequant       = 1,
        _Stat_VC1Reconstruct   = 2,
        _Stat_VC1MVCalculate   = 3,
        _Stat_VC1MC            = 4,
        _Stat_VC1PreparePlane  = 5,
        _Stat_VC1Deblock       = 6,
        _Stat_VC1RangeMap      = 7,
        _Stat_VC1Complete      = 8,
        _Stat_VC1Sleep         = 9,
        _Stat_VC1WakeUp        = 10
    } _Stat_VC1TaskTypes;
#endif


#pragma pack(16)

    class VC1Task
    {
    public:
        // Default constructor
        VC1Task(Ipp32s iThreadNumber,Ipp32s TaskID):m_pSlice(NULL),
                                                    m_iThreadNumber(iThreadNumber),
                                                    m_iTaskID(TaskID),
                                                    m_eTasktype(VC1Decode),
                                                    m_pBlock(NULL),
                                                    m_pPredBlock(NULL),
                                                    m_pSrcToSwap(NULL),
                                                    m_uDataSizeToSwap(0),
                                                    m_isFirstInSecondSlice(false),
                                                    m_isFieldReady(false),
                                                    pMulti(NULL)




          {
          };
          VC1Task(Ipp32s iThreadNumber):m_pSlice(NULL),
                                        m_iThreadNumber(iThreadNumber),
                                        m_iTaskID(0),
                                        m_eTasktype(VC1Decode),
                                        m_pBlock(NULL),
                                        m_pPredBlock(NULL),
                                        m_pSrcToSwap(NULL),
                                        m_uDataSizeToSwap(0),
                                        m_isFirstInSecondSlice(false),
                                        m_isFieldReady(false),
                                        pMulti(NULL)

          {
          };
          ~VC1Task()
          {
              if (m_pSlice)
                  ippFree(m_pSlice);
              m_pSlice = NULL;
          }

          Ipp32u IsDecoding (Ipp32u _task_settings) {return _task_settings&VC1Decode;}
          Ipp32u IsDeblocking(Ipp32u _task_settings) {return _task_settings&VC1Deblock;}
          void setSliceParams(VC1Context* pContext);

          VC1TaskTypes switch_task();
          SliceParams* m_pSlice;                                        //

#ifdef VC1_THREAD_STATISTIC
          VC1TaskProcessor*    pJob;
#endif
          Ipp32s m_iThreadNumber;                                     // (Ipp32s) owning thread number
          Ipp32s m_iTaskID;                                           // (Ipp32s) task identificator
          VC1TaskTypes m_eTasktype;
          Ipp16s*      m_pBlock;
          Ipp8u*       m_pPredBlock;
          Ipp8u*       m_pSrcToSwap;
          Ipp32u       m_uDataSizeToSwap;
          bool         m_isFirstInSecondSlice;
          bool         m_isFieldReady;
          VC1Status (VC1TaskProcessorUMC::*pMulti)(VC1Context* pContext, VC1Task* pTask);
    };


}
#endif //__umc_vc1_dec_task_H__
