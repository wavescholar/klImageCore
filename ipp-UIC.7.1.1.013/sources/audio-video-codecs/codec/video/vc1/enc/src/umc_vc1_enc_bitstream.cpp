/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, bitstream functionality
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_bitstream.h"
namespace UMC_VC1_ENCODER
{
void VC1EncoderBitStreamSM::Init(UMC::MediaData* data)
{
    m_pBitStream    = (Ipp32u*) ((Ipp8u*)data->GetBufferPointer() + data->GetDataSize());
    m_iOffset       = 32;
    m_pBitStream[0] = 0;

    m_pBufferStart  = m_pBitStream;
    m_iBufferLen    = data->GetBufferSize() - data->GetDataSize();

    m_pBlankSegment     = 0;
    m_iBlankSegmentLen  = 0;

    m_dPTS          = data->m_fPTSStart;
}
void VC1EncoderBitStreamAdv::Init(UMC::MediaData* data)
{
    m_pBitStream    = (Ipp32u*) ((Ipp8u*)data->GetBufferPointer() + data->GetDataSize());
    m_iOffset       = 32;
    m_pBitStream[0] = 0;

    m_pBufferStart  = m_pBitStream;
    m_iBufferLen    = data->GetBufferSize() - data->GetDataSize();

    m_dPTS          = data->m_fPTSStart;

    ResetCodeStatus();

    m_bLast         = false;
}
void  VC1EncoderBitStreamSM::Reset()
{
    m_pBitStream        = 0;
    m_pBufferStart      = 0;
    m_iBufferLen        = 0;
    m_iOffset           = 32;
    m_dPTS              = -1;
    m_pBlankSegment     = 0;
    m_iBlankSegmentLen  = 0;
}
void  VC1EncoderBitStreamAdv::Reset()
{
    m_pBitStream        = 0;
    m_pBufferStart      = 0;
    m_iBufferLen        = 0;
    m_iOffset           = 32;
    m_dPTS              = -1;
    m_bLast             = false;
}

UMC::Status VC1EncoderBitStreamSM::DataComplete(UMC::MediaData* data)
{
    size_t  dataLen = 0;

    dataLen = GetDataLen();
    VM_ASSERT (dataLen < m_iBufferLen);

    UMC::Status ret = AddLastBits();
    if (ret != UMC::UMC_OK) return ret;

    ret = data->SetDataSize(data->GetDataSize() + dataLen);
    if (ret != UMC::UMC_OK) return ret;

    data->m_fPTSStart = m_dPTS;

    Reset();

    return ret;
}
UMC::Status VC1EncoderBitStreamAdv::DataComplete(UMC::MediaData* data)
{
    size_t  dataLen = 0;

    UMC::Status ret = AddLastBits();
    if (ret != UMC::UMC_OK) return ret;

    dataLen = (m_pBitStream - m_pBufferStart)*sizeof(Ipp32u);
    VM_ASSERT (dataLen < m_iBufferLen);

    ret = data->SetDataSize(data->GetDataSize() + dataLen);
    if (ret != UMC::UMC_OK) return ret;

    data->m_fPTSStart = m_dPTS;

    Reset();

    return ret;
}

//UMC::Status VC1EncoderBitStreamSM::AddLastBits8u()
//{
//    UMC::Status ret = UMC::UMC_OK;
//    Ipp8u bits = m_iOffset%8;
//    if (bits!=0 )
//    {
//        ret =PutBits((1<<(bits-1)),bits);
//    }
//    return ret;
//}
UMC::Status VC1EncoderBitStreamSM::AddLastBits()
{
    UMC::Status ret = UMC::UMC_OK;
    if (m_iOffset!=32 )
    {
        ret =PutBits((1<<(m_iOffset-1)),m_iOffset);
    }
    return ret;
}
UMC::Status VC1EncoderBitStreamAdv::AddLastBits()
{
    UMC::Status ret = UMC::UMC_OK;
    assert (m_iOffset!=0);
    if (!m_bLast)
    {
        Ipp8u z = (Ipp8u)(m_iOffset%8);
        z = (z)? z : 8;
        ret = PutBits((1<<(z-1)),z);
        if (ret != UMC::UMC_OK) return ret;
        ret = PutLastBits();
    }
    m_bLast = true;
    return ret;
}
static Ipp32u mask[] = {
                   0x00000000,
                   0x00000001,0x00000003,0x00000007,0x0000000F,
                   0x0000001F,0x0000003F,0x0000007F,0x000000FF,
                   0x000001FF,0x000003FF,0x000007FF,0x00000FFF,
                   0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,
                   0x0001FFFF,0x0003FFFF,0x0007FFFF,0x000FFFFF,
                   0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,
                   0x01FFFFFF,0x03FFFFFF,0x07FFFFFF,0x0FFFFFFF,
                   0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF
};

UMC::Status VC1EncoderBitStreamSM::PutBits(Ipp32u val,Ipp32s len)
 {
    Ipp32s tmpcnt;
    Ipp32u r_tmp;

    assert(m_pBitStream!=NULL);
    assert(len<=32);

    if ((Ipp8u*)m_pBitStream + (len + m_iOffset)/8 >= (Ipp8u*)m_pBufferStart + m_iBufferLen - 1)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    val = val & mask[len];

    tmpcnt = (m_iOffset) - (len);

    if(tmpcnt <= 0)
    {
      Ipp32u z=0;
      r_tmp = (m_pBitStream)[0] | ((val) >> (-tmpcnt));
      (m_pBitStream)[0] = BSWAP(r_tmp);
      (m_pBitStream)++;
      z = (tmpcnt<0)? (val << (32 + tmpcnt)):z;
      (m_pBitStream)[0] = z ;
      (m_iOffset) = 32 + tmpcnt;
    }
    else
    {
      (m_pBitStream)[0] |= (val) << tmpcnt;
      m_iOffset = tmpcnt;
    }
    return UMC::UMC_OK;
}
 UMC::Status VC1EncoderBitStreamSM::PutBitsHeader(Ipp32u val,Ipp32s len)
 {
    Ipp32s tmpcnt;
    Ipp32u r_tmp;

    assert(m_pBitStream!=NULL);
    assert(len<=32);

    if ((Ipp8u*)m_pBitStream + (len + m_iOffset)/8 >= (Ipp8u*)m_pBufferStart + m_iBufferLen - 1)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    val = val & mask[len];

    tmpcnt = (m_iOffset) - (len);

    if(tmpcnt <= 0)
    {
      Ipp32u z=0;
      r_tmp = (m_pBitStream)[0] | ((val) >> (-tmpcnt));
      (m_pBitStream)[0] = BNOSWAP(r_tmp);
      (m_pBitStream)++;
      z = (tmpcnt<0)? (val << (32 + tmpcnt)):z;
      (m_pBitStream)[0] = z ;
      (m_iOffset) = 32 + tmpcnt;
    }
    else
    {
      (m_pBitStream)[0] |= (val) << tmpcnt;
      m_iOffset = tmpcnt;
    }
    return UMC::UMC_OK;
}
static Ipp32u maskUpper[] = {0x00000000, 0xFF000000, 0xFFFF0000,0xFFFFFF00};
static Ipp32u maskLower[] = {0xFFFFFFFF, 0x00FFFFFF, 0x0000FFFF,0x000000FF};
static Ipp32u maskL[] = {0x00000000, 0x000000FF, 0x0000FFFF,0x00000000};

UMC::Status VC1EncoderBitStreamAdv::PutBits(Ipp32u val,Ipp32s len)
 {
    Ipp32s tmpcnt;

    assert(m_pBitStream!=NULL);
    assert(len<=32);

    if ((Ipp8u*)m_pBitStream + (len + m_iOffset)/8 >= (Ipp8u*)m_pBufferStart + m_iBufferLen - 1)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    val = val & mask[len];
    tmpcnt = (m_iOffset) - (len);

    while (tmpcnt <= 0)
    {
      Ipp32s i=-1;
      Ipp8u  n_bytes  = 0;
      Ipp32u r_tmp = 0;
      Ipp32u tmp = 0;

      r_tmp = (m_pBitStream)[0] | ((val) >> (-tmpcnt));
      tmp = r_tmp;

      len = len -  m_iOffset;
      i = 0;
      while ((i = CheckCode(tmp | maskUpper[i]))>=0)
      {
         n_bytes ++;
         tmp             = (tmp & maskUpper[i]) | (0x03 <<((3-i)*8))| ((tmp & maskLower[i])>>8);
      }
      m_pBitStream[0]   =  BSWAP(tmp);
      m_pBitStream ++;

      m_iOffset         =  32 - n_bytes*8;
      m_pBitStream[0]   =  (r_tmp & maskL[n_bytes])<< m_iOffset;

      val = val & mask[len];
      tmpcnt = (m_iOffset) - (len);
    }
    if (len>0)
    {
       assert (tmpcnt>=0);
      (m_pBitStream)[0] |= (val) << tmpcnt;
       m_iOffset = tmpcnt;
    }
    return UMC::UMC_OK;
}
 UMC::Status VC1EncoderBitStreamAdv::PutLastBits()
 {
//    Ipp32s tmpcnt;
    Ipp32s pos = (32 - m_iOffset)>>3;
    Ipp32s i=-1;

    assert(m_pBitStream!=NULL);
    assert(m_iOffset%8 == 0);

    Ipp32u tmp = (m_pBitStream)[0];
    if (m_iOffset == 32)
        return UMC::UMC_OK;

    i = CheckCode(tmp);
    if (i>=0 && i<= pos)
    {
        tmp  = (tmp & maskUpper[i]) | (0x03 <<((3-i)*8))| ((tmp & maskLower[i])>>8);
    }
    m_pBitStream[0]   =  BSWAP(tmp);
    m_pBitStream ++;
    m_pBitStream[0]   = 0;
    m_iOffset = 32;

    return UMC::UMC_OK;
}
 UMC::Status VC1EncoderBitStreamAdv::PutStartCode(Ipp32u val, Ipp32s len)
 {
   Ipp32s tmpcnt;
   Ipp32u r_tmp;

   assert(m_pBitStream!=NULL);
   assert(len<=32);

    if ((Ipp8u*)m_pBitStream + (len + m_iOffset)/8 >= (Ipp8u*)m_pBufferStart + m_iBufferLen - 1)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    val = val & mask[len];
    tmpcnt = (m_iOffset) - (len);

    if(tmpcnt <= 0)
    {
      Ipp32u z=0;
      r_tmp = (m_pBitStream)[0] | ((val) >> (-tmpcnt));
      (m_pBitStream)[0] = BSWAP(r_tmp);
      (m_pBitStream)++;
      z = (tmpcnt<0)? (val << (32 + tmpcnt)):z;
      (m_pBitStream)[0] = z ;
      (m_iOffset) = 32 + tmpcnt;
    }
    else
    {
      (m_pBitStream)[0] |= (val) << tmpcnt;
      m_iOffset = tmpcnt;
    }
    m_bLast = false;
    ResetCodeStatus();
    return UMC::UMC_OK;
}
Ipp32s       VC1EncoderBitStreamAdv::CheckCode(Ipp32u code)
{
    Ipp32s i;

    for (i=0;i<4;i++)
    {
        Ipp8u nextByte = (Ipp8u)((code >>((3-i)*8))&0xFF);
        if (nextByte>3)
        {
            m_uiCodeStatus = 0;
        }
        else
        {
            if (m_uiCodeStatus <2)
            {
                m_uiCodeStatus = (nextByte == 0)?  m_uiCodeStatus + 1 : 0;
            }
            else
            {
                m_uiCodeStatus = 0;
                return i;
            }
        }
    }
    return -1;
}

 UMC::Status  VC1EncoderBitStreamSM::MakeBlankSegment(Ipp32s len)
 {
    if (m_iOffset != 32 || m_pBlankSegment)
         return UMC::UMC_ERR_NOT_IMPLEMENTED;

    if ((Ipp8u*)(m_pBitStream + len) >= (Ipp8u*)m_pBufferStart + m_iBufferLen - 1)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    m_pBlankSegment    = m_pBitStream;
    m_iBlankSegmentLen = len;
    m_pBitStream      += len;

    m_pBitStream[0]=0;
    return UMC::UMC_OK;

 }
 UMC::Status  VC1EncoderBitStreamSM::FillBlankSegment(Ipp32u value)
 {
     if (!m_pBlankSegment || !m_iBlankSegmentLen)
         return UMC::UMC_ERR_NOT_IMPLEMENTED;

     *m_pBlankSegment = BNOSWAP(value);
     m_pBlankSegment++;
     m_iBlankSegmentLen--;

     return UMC::UMC_OK;
 }
void VC1EncoderBitStreamSM::DeleteBlankSegment()
{
    m_pBlankSegment     = 0;
    m_iBlankSegmentLen  = 0;
}
}

#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
