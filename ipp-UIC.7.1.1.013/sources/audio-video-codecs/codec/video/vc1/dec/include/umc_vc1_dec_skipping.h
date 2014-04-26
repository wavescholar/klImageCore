/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Skipping algorithms
//
*/

#ifndef __UMC_VC1_DEC_SKIPPING_H_
#define __UMC_VC1_DEC_SKIPPING_H_

#include "ippdefs.h"

namespace UMC
{
    namespace VC1Skipping
    {
        typedef enum
        {
            VC1Routine,
            VC1PostProcDisable,
            VC1MaxSpeed
        } VC1PerfFeatureMode;

        typedef enum
        {
            // rutine pipeline
            Normal = 0,
            // no post processing on B frames
            B1,
            // no post processing and simple Quant + Inv.Transform on B frames
            B2,
            // no post processing and simple Quant + Inv.Transform on B frames. Skip every 3-th
            B3,
            // no post processing and simple Quant + Inv.Transform on B frames. Skip every 2-th
            B4,
            // no post processing and simple Quant + Inv.Transform on B frames. Skip all B frames
            B5,
            // all features are disabled for B frames. no post processing on Ref frames
            Ref1,
            // all features are disabled for B frames. no post processing and simple Quant + Inv.Transform on B frames
            Ref2,
            // all features are disabled for all frames. Skip every 3-th ref frame, 2-th B frame
            Ref3,
            // all features are disabled for all frames. Skip every 2-th ref frame, all B frame
            Ref4
        } SkippingMode;


        // all skipping algorithms is described here
        class VC1SkipMaster
        {
        public:
            VC1SkipMaster():m_iRefSkipPeriod(0),
                            m_iRefSkipCounter(0),
                            m_iBSkipPeriod(0),
                            m_iBSkipCounter(0),
                            m_RefPerfMode(VC1Routine),
                            m_BPerfMode(VC1Routine),
                            m_SpeedMode(Normal)
            {
            };
            // skipping algorythms can be diffrent for reference and predictive (B/BI) frames
            // For Ref frames minimum period is 2
            // For B minimum period is 1
            void SetSkipPeriod(bool ForRefFrames, Ipp32u period);
            void SetPerformMode(bool ForRefFrames, Ipp32u perfomMode);


            void SetVideoDecodingSpeed(SkippingMode skip_mode);
            bool ChangeVideoDecodingSpeed(Ipp32s& speed_shift);

            bool IsNeedSkipFrame(Ipp32u picType);
            bool IsNeedPostProcFrame(Ipp32u picType);
            bool IsNeedSimlifyReconstruct(Ipp32u picType);


            void Reset();


            ~ VC1SkipMaster()
            {
            }
        private:
            void MoveToNextState();
            // Skip parameters for reference frames
            Ipp32u m_iRefSkipPeriod;
            Ipp32u m_iRefSkipCounter;
            // Skip parameters for predicted frames
            Ipp32u m_iBSkipPeriod;
            Ipp32u m_iBSkipCounter;

            Ipp32u  m_RefPerfMode;
            Ipp32u  m_BPerfMode;

            Ipp32s m_SpeedMode;

        };
    }
}

#endif
