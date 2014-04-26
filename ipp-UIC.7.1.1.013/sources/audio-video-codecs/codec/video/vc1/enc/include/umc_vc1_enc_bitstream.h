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

#ifndef _ENCODER_VC1_BITSTREAM_H_
#define _ENCODER_VC1_BITSTREAM_H_

#include "vm_types.h"
#include "umc_media_data.h"
#include "vm_debug.h"
#include "assert.h"
#include "umc_vc1_enc_statistic.h"

namespace UMC_VC1_ENCODER
{

#ifdef _BIG_ENDIAN_
#define BSWAP(x) (x)
#define BNOSWAP(x) (unsigned int)(((x) << 24) + (((x)&0xff00) << 8) + (((x) >> 8)&0xff00) + ((x) >> 24))
#else
#define BNOSWAP(x) (x)
#define BSWAP(x) (unsigned int)(((x) << 24) + (((x)&0xff00) << 8) + (((x) >> 8)&0xff00) + ((x) >> 24))
#endif


class VC1EncoderBitStreamSM
{
private:
    Ipp32u*      m_pBitStream; /*  pointer to bitstream        */
    Ipp32s       m_iOffset;    /*  bit offset [0,32]           */

    Ipp32u*      m_pBufferStart;
    size_t       m_iBufferLen;

    double       m_dPTS;

    Ipp32u*      m_pBlankSegment;
    size_t       m_iBlankSegmentLen;

public:

    VC1EncoderBitStreamSM()
    {
        m_pBlankSegment = 0;
        m_pBitStream = 0;
        m_iBlankSegmentLen = 0;
        m_iOffset = 32;
        m_pBufferStart  = 0;
        m_iBufferLen = 0;
        m_dPTS = 0;
    }

    ~VC1EncoderBitStreamSM(){}

    void            Init(UMC::MediaData* data);

    UMC::Status     DataComplete(UMC::MediaData* data);

    UMC::Status     AddLastBits();
    UMC::Status     PutBits         (Ipp32u val,Ipp32s len);
    UMC::Status     PutBitsHeader   (Ipp32u val,Ipp32s len);


    UMC::Status     MakeBlankSegment(Ipp32s len);
    UMC::Status     FillBlankSegment(Ipp32u value);
    void            DeleteBlankSegment();
    Ipp32u          GetDataLen()
    {
        return ((Ipp32u)((Ipp8u*)m_pBitStream - (Ipp8u*)m_pBufferStart)) + (32-m_iOffset+7)/8;
    }
#ifdef VC1_ME_MB_STATICTICS
public:
#endif
    Ipp32u          GetCurrBit ()
    {
        return (((Ipp32u)((Ipp8u*)m_pBitStream - (Ipp8u*)m_pBufferStart))<<3)+ (32 - m_iOffset);
    }
protected:
    void            Reset();
};

class VC1EncoderBitStreamAdv
{
private:
    Ipp32u*      m_pBitStream; /*  pointer to bitstream        */
    Ipp32s       m_iOffset;    /*  bit offset [0,32]           */

    Ipp32u*      m_pBufferStart;
    size_t       m_iBufferLen;

    double       m_dPTS;
    Ipp8u        m_uiCodeStatus;
    bool         m_bLast;

public:

    VC1EncoderBitStreamAdv()
    {
        m_pBitStream = 0;
        m_iOffset = 32;
        m_pBufferStart = 0;
        m_iBufferLen = 0;
        m_dPTS = 0;
        m_uiCodeStatus = 0;
        m_bLast = false;
    }

    ~VC1EncoderBitStreamAdv(){}

    void            Init(UMC::MediaData* data);
    UMC::Status     DataComplete(UMC::MediaData* data);

    UMC::Status     AddLastBits();
    UMC::Status     PutBits      (Ipp32u val,Ipp32s len);
    UMC::Status     PutStartCode (Ipp32u val,Ipp32s len=32);

    Ipp32u          GetDataLen()
    {
        assert (m_iOffset==32);
        return ((Ipp32u)((Ipp8u*)m_pBitStream - (Ipp8u*)m_pBufferStart));
    }

protected:
    void           Reset();
    Ipp32s         CheckCode(Ipp32u code);
    void           ResetCodeStatus() {m_uiCodeStatus = 0;}
    UMC::Status    PutLastBits();

#ifdef VC1_ME_MB_STATICTICS
public:
#endif
    Ipp32u         GetCurrBit ()
    {
        return (((Ipp32u)((Ipp8u*)m_pBitStream - (Ipp8u*)m_pBufferStart))<<3)+ (32 - m_iOffset);
    }
};
}
#endif
