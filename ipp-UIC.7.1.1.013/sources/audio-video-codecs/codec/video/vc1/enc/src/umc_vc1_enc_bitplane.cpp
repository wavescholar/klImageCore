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

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_bitplane.h"

namespace UMC_VC1_ENCODER
{
  UMC::Status VC1EncoderBitplane::Init(Ipp16u width, Ipp16u height)
  {
      Close();
      m_uiHeight    = height;
      m_uiWidth     = width;
      pBitplane = new bool[height*width];
      if (!pBitplane)
          return UMC::UMC_ERR_ALLOC;
      return UMC::UMC_OK;
  }
  void VC1EncoderBitplane::Close()
  {
      if (pBitplane)
      {
          delete [] pBitplane;
          pBitplane = 0;
      }
      m_uiHeight    = 0;
      m_uiWidth     = 0;
  }

  UMC::Status VC1EncoderBitplane::SetValue(bool value, Ipp16u x, Ipp16u y)
  {
    if (x >= m_uiWidth || y >= m_uiHeight)
        return UMC::UMC_ERR_FAILED;
       pBitplane[ x + y*m_uiWidth] = value;
    return UMC::UMC_OK;
  }

  Ipp8u VC1EncoderBitplane::Get2x3Normal(Ipp16u x,Ipp16u y)
  {
      return (pBitplane[x + (y+0)*m_uiWidth]<<0)|(pBitplane[x+1+ (y+0)*m_uiWidth]<<1)|
             (pBitplane[x + (y+1)*m_uiWidth]<<2)|(pBitplane[x+1+ (y+1)*m_uiWidth]<<3)|
             (pBitplane[x + (y+2)*m_uiWidth]<<4)|(pBitplane[x+1+ (y+2)*m_uiWidth]<<5);
  }
  Ipp8u VC1EncoderBitplane::Get3x2Normal(Ipp16u x,Ipp16u y)
  {
      return (pBitplane[x + (y+0)*m_uiWidth]<<0) |(pBitplane[x+1+ (y+0)*m_uiWidth]<<1) |(pBitplane[x+2+ (y+0)*m_uiWidth]<<2)|
             (pBitplane[x + (y+1)*m_uiWidth]<<3) |(pBitplane[x+1+ (y+1)*m_uiWidth]<<4) |(pBitplane[x+2+ (y+1)*m_uiWidth]<<5);
  }
}
#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
