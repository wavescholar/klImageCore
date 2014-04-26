/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) frame constructor
//
*/

#ifndef __UMC_VC1_SPL_FRAME_CONSTR_H__
#define __UMC_VC1_SPL_FRAME_CONSTR_H__

#include "umc_structures.h"
#include "umc_splitter.h"

namespace UMC
{
    struct VC1FrameConstrInfo
    {
        MediaData* in;
        MediaData* out;
        MediaDataEx::_MediaDataEx *stCodes;
        Ipp32u splMode;
    };
    class vc1_frame_constructor
    {
    public:
            vc1_frame_constructor(){};
            virtual ~vc1_frame_constructor(){};

            virtual Status GetNextFrame(VC1FrameConstrInfo& pInfo) = 0;  //return 1 frame, 1 header....
            virtual Status GetFirstSeqHeader(VC1FrameConstrInfo& pInfo) = 0;
            virtual Status GetData(VC1FrameConstrInfo& pInfo) = 0;
            virtual Status ParseVC1SeqHeader (Ipp8u *data, Ipp32u* bufferSize,SplitterInfo* info) = 0;
            //virtual Status GetPicType(MediaDataEx* data, Ipp32u& picType) = 0;
            virtual void Reset() = 0;
    };

    class vc1_frame_constructor_rcv: public vc1_frame_constructor
    {
        public:
            vc1_frame_constructor_rcv();
            ~vc1_frame_constructor_rcv();

            Status GetNextFrame(VC1FrameConstrInfo& Info);
            Status GetData(VC1FrameConstrInfo& Info);
            Status GetFirstSeqHeader(VC1FrameConstrInfo& Info);
            void Reset();
            Status ParseVC1SeqHeader (Ipp8u *data, Ipp32u* bufferSize,SplitterInfo* info);
            //Status GetPicType(MediaDataEx* data, Ipp32u& picType);
    };

    class vc1_frame_constructor_vc1: public vc1_frame_constructor
    {
         public:
            vc1_frame_constructor_vc1();
            ~vc1_frame_constructor_vc1();

            Status GetNextFrame(VC1FrameConstrInfo& Info);
            Status GetData(VC1FrameConstrInfo& Info);
            Status GetFirstSeqHeader(VC1FrameConstrInfo& Info);
            void Reset();
            Status ParseVC1SeqHeader (Ipp8u *data, Ipp32u* bufferSize,SplitterInfo* info);
           //Status GetPicType(MediaDataEx* data, Ipp32u& picType);
    };
}

#endif//__UMC_VC1_SPL_FRAME_CONSTR_H__
