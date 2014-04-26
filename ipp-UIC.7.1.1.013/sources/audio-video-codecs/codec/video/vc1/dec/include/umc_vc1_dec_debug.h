/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_VC1_DEC_DEBUG_H__
#define __UMC_VC1_DEC_DEBUG_H__

#include "umc_vc1_dec_seq.h"
#include "vm_types.h"
#include "vm_debug.h"

extern const Ipp32u  VC1_POSITION; // MB, Block positions, skip info
extern const Ipp32u  VC1_CBP; // coded block patern info
extern const Ipp32u  VC1_BITBLANES; // bitplane information
extern const Ipp32u  VC1_QUANT; // transform types decoded info
extern const Ipp32u  VC1_TT; // transform types decoded info
extern const Ipp32u  VC1_MV; // motion vectors info
extern const Ipp32u  VC1_PRED; // predicted blocks
extern const Ipp32u  VC1_COEFFS; // DC, AC coefficiens
extern const Ipp32u  VC1_RESPEL; // pixels befor filtering
extern const Ipp32u  VC1_SMOOTHINT; // smoothing
extern const Ipp32u  VC1_BFRAMES; // B frames log
extern const Ipp32u  VC1_INTENS; // intesity compensation tables
extern const Ipp32u  VC1_MV_BBL; // deblocking
extern const Ipp32u  VC1_MV_FIELD; // motion vectors info for field pic

extern const Ipp32u  VC1_DEBUG; //current debug output
extern const Ipp32u  VC1_FRAME_DEBUG; //on/off frame debug
extern const Ipp32u  VC1_FRAME_MIN; //first frame to debug
extern const Ipp32u  VC1_FRAME_MAX; //last frame to debug
extern const Ipp32u  VC1_TABLES; //VLC tables

typedef enum
{
    VC1DebugAlloc,
    VC1DebugFree,
    VC1DebugRoutine
} VC1DebugWork;

class VM_Debug
{
public:

    void vm_debug_frame(Ipp32s _cur_frame, Ipp32s level, const vm_char *format, ...);
    void _print_macroblocks(VC1Context* pContext);
    void _print_blocks(VC1Context* pContext);
    void print_bitplane(VC1Bitplane* pBitplane, Ipp32s width, Ipp32s height);
    void Release()
    {
        delete this;
    }
#if defined (WINDOWS) && (_DEBUG)
    VM_Debug():DebugThreadID(GetCurrentThreadId())
#else
    VM_Debug()
#endif
    {

    }; //only for Win debug

    ~VM_Debug()
    {
    };
#if defined (WINDOWS) && (_DEBUG)
    void setThreadToDebug(Ipp32u threadID)
    {
        DebugThreadID = threadID;
    }
#endif

private:
#if defined (WINDOWS) && (_DEBUG)
    Ipp32u DebugThreadID;
#endif

};

#endif
