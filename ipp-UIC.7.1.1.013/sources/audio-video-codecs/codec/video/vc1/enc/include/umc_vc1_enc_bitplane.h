/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, bitplane coding
//
*/

#ifndef _ENCODER_VC1_BITPLANE_H_
#define _ENCODER_VC1_BITPLANE_H_

#include "umc_vc1_enc_def.h"
#include "umc_structures.h"

namespace UMC_VC1_ENCODER
{
    class VC1EncoderBitplane
    {
    private:
        bool        *pBitplane;
        Ipp16u      m_uiWidth;
        Ipp16u      m_uiHeight;

    protected:

        inline Ipp8u  Get2x3Normal(Ipp16u x,Ipp16u y);
        inline Ipp8u  Get3x2Normal(Ipp16u x,Ipp16u y);

    public:

        UMC::Status Init(Ipp16u width, Ipp16u height);
        void Close();
        UMC::Status SetValue(bool value, Ipp16u x, Ipp16u y);

        VC1EncoderBitplane():
          pBitplane(0),
          m_uiWidth(0),
          m_uiHeight(0)
          {}

        ~VC1EncoderBitplane()
        {
            Close();
        }
    private:
        VC1EncoderBitplane(const VC1EncoderBitplane&) {}
        const VC1EncoderBitplane& operator=(const VC1EncoderBitplane&) { return *this; }
    };

}
#endif //_ENCODER_VC1_BITPLANE_H_
