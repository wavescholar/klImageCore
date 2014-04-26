/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, hypothetical reference decoder
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_hrd.h"
#include "umc_vc1_enc_debug.h"
#include "umc_vc1_enc_sequence_adv.h"
#include "umc_vc1_enc_sequence_sm.h"

#include "assert.h"

namespace UMC_VC1_ENCODER
{
Ipp32u bMax_LevelLimits[3][5] =
{
     {   /* Simple Profile */
        20,     /* Low    Level */
        77,     /* Medium Level */
        0,      /* Hight Level */
        0,
        0
    },
    {   /* Main Profile */
        306,        /* Low    Level */
        611,        /* Medium Level */
        2442,       /* Hight Level */
        0,
        0
    },
    {   /* Advanced Profile */
        250,        /*L0 level*/
        1250,       /*L1 level*/
        2500,       /*L2 level*/
        5500,       /*L3 level*/
        16500       /*L4 level*/
    }
};


Ipp32u rMax_LevelLimits[3][5] =
{
     {   /* Simple Profile */
        96000,      /* Low    Level */
        384000,     /* Medium Level */
        0,          /* Hight Level */
        0,
        0
    },
    {   /* Main Profile */
        2000000,        /* Low    Level */
        10000000,        /* Medium Level */
        20000000,       /* Hight Level */
        0,
        0
    },
    {   /* Advanced Profile */
        2000000,        /*L0 level*/
        10000000,       /*L1 level*/
        20000000,       /*L2 level*/
        45000000,       /*L3 level*/
        135000000       /*L4 level*/
    }
};

Ipp32u MBfLimits[3][5] =
{
     {   /* Simple Profile */
        99,      /* Low    Level */
        396,     /* Medium Level */
        0,       /* Hight Level */
        0,
        0
    },
    {   /* Main Profile */
        396,         /* Low    Level */
        1620,        /* Medium Level */
        8192,        /* Hight Level */
        0,
        0
    },
    {   /* Advanced Profile */
        396,        /*L0 level*/
        1620,       /*L1 level*/
        3680,       /*L2 level*/
        8192,       /*L3 level*/
        16384       /*L4 level*/
    }
};


VC1HRDecoder::VC1HRDecoder():m_IdealFrameSize(0), m_recoding(0)
{
#ifdef VC1_HRD_DEBUG
    printf("\t\tHRD constructor\n");
#endif
    memset(&m_LBuckets,0,sizeof(VC1_enc_LeakyBucketInfo));
    memset(&hrd_data, 0, sizeof(VC1_hrd_OutData));
};

VC1HRDecoder::~VC1HRDecoder()
{
};

void VC1HRDecoder::Reset()
{
#ifdef VC1_HRD_DEBUG
    printf("\t\tHRD ResetHRDecoder\n");
#endif

    m_LBuckets.vc1_decFullness     = m_LBuckets.vc1_decInitFull;
    m_LBuckets.vc1_dec_delay       = m_LBuckets.vc1_decInitFull;
    m_LBuckets.vc1_decPrevFullness = m_LBuckets.vc1_decInitFull;
    m_LBuckets.vc1_enc_delay       = 0;
    m_LBuckets.vc1_Prev_enc_delay  = 0;


    //memset(&m_LBuckets,0,sizeof(VC1_enc_LeakyBucketInfo));

    //memset(&hrd_data, 0, sizeof(VC1_hrd_OutData));

    m_recoding = 0;
};

UMC::Status VC1HRDecoder::InitBuffer(Ipp32s profile,   Ipp32s level,
                                     Ipp32s bufferSize,Ipp32s initFull,
                                     Ipp32s bitRate,   Ipp64f frameRate)
{
    Ipp32s maxBufSize = 0;
    Ipp32s i = 0; //level
    Ipp32s j = 0; //profile
    m_IdealFrameSize = (Ipp32u)(bitRate/frameRate);  //in bits

    if(profile == VC1_ENC_PROFILE_A)
    {
        j = 2;
        if(level == VC1_ENC_LEVEL_4) i = 4;
        else if(level == VC1_ENC_LEVEL_3) i = 3;
        else if(level == VC1_ENC_LEVEL_2) i = 2;
        else if(level == VC1_ENC_LEVEL_1) i = 1;

    }
    else if (profile == VC1_ENC_PROFILE_M)
    {
        j = 1;
        if(level == VC1_ENC_LEVEL_H)      i = 2;
        else if(level == VC1_ENC_LEVEL_M) i = 1;
    }
    else
    {
        j = 0;
        if(level == VC1_ENC_LEVEL_H)         i = 2;
        else  if(level == VC1_ENC_LEVEL_M)   i = 1;
    }


    if((bufferSize < (m_IdealFrameSize<<1)) && (bufferSize!=0))
    {
        assert(0);
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
    }


    assert(j<4);
    assert(i<5);

    maxBufSize = bMax_LevelLimits[j][i]*16384;
    assert(maxBufSize != 0);

    if(bufferSize <= 0)
        bufferSize = maxBufSize;

    if(bufferSize > maxBufSize)
        bufferSize = maxBufSize;

    if(initFull > bufferSize)
        initFull = (bufferSize*4)/5;

    if(initFull <=0)
        initFull = bufferSize;

    assert(bufferSize!=0);

    m_LBuckets.vc1_decBufferSize = bufferSize;
    m_LBuckets.vc1_decFullness   = initFull;
    m_LBuckets.vc1_decInitFull   = initFull;
    m_LBuckets.vc1_BitRate       = bitRate;
    m_LBuckets.vc1_FrameRate     = frameRate;
    m_LBuckets.vc1_decPrevFullness  = initFull;

    m_LBuckets.vc1_dec_delay     = initFull;

    //hrd params
    hrd_data.hrd_buffer        = bufferSize;

    SetHRDFullness();
    SetHRDRate(0);


#ifdef VC1_HRD_DEBUG
    printf("bufferSize = ,%d\n", bufferSize);
    printf("initFull   = ,%d\n", initFull);
#endif

    return UMC::UMC_OK;
};

Ipp32s VC1HRDecoder::CheckBuffer(Ipp32s frameSize)
{
    Ipp32s Sts = VC1_HRD_OK;
    Ipp32s bufFullness = 0;
    Ipp32s decFullness = 0;
    Ipp32s encDelay    = m_LBuckets.vc1_enc_delay;
    frameSize = frameSize * 8;
    m_recoding++;

#ifdef VC1_HRD_DEBUG
    printf("CheckBuffer,");
#endif

    if(frameSize > m_LBuckets.vc1_decBufferSize)
    {
        return VC1_HRD_ERR_BIG_FRAME;
    }

    decFullness = m_LBuckets.vc1_decFullness /*+ m_IdealFrameSize */+ encDelay;

    if(frameSize > decFullness)
    {
       Sts |= VC1_HRD_ERR_DEC_NOT_ENOUGH_DATA;
       encDelay = decFullness - frameSize;
       decFullness = 0;
    }
    else
    {
        decFullness = decFullness - frameSize;

       if(decFullness < m_LBuckets.vc1_decBufferSize)
       {
            encDelay = 0;
       }
       else
       {
           encDelay = decFullness- m_LBuckets.vc1_decBufferSize;
           decFullness = m_LBuckets.vc1_decBufferSize;
           Sts |= VC1_HRD_ERR_DEC_NOT_ENOUGH_BUFFER;
       }
    }

    bufFullness = decFullness + encDelay + m_IdealFrameSize;

    if(bufFullness <= m_LBuckets.vc1_decBufferSize)
    {
        encDelay = 0;
        if(bufFullness < 0)
        {
            Sts |= VC1_HRD_ERR_ENC_NOT_ENOUGH_DATA;
            encDelay = bufFullness;
            bufFullness = 0;
        }
    }
    else
    {
        encDelay = bufFullness - m_LBuckets.vc1_decBufferSize;
        bufFullness = m_LBuckets.vc1_decBufferSize;
        Sts |= VC1_HRD_ERR_ENC_NOT_ENOUGH_BUFFER;
    }



#ifdef VC1_HRD_DEBUG
   printf("+ = ,%d,", m_IdealFrameSize);
   printf("- = ,%d,", frameSize);
   printf("decFullness = ,%d,", bufFullness);
   printf("encDelay    = ,%d,", encDelay);
#endif

    return Sts;
}

Ipp32s VC1HRDecoder::ReleaseData (Ipp32s frameSize)
{
#ifdef VC1_HRD_DEBUG
    printf("SetNewData,");
#endif

    Ipp32s Sts = VC1_HRD_OK;

    m_LBuckets.vc1_decPrevFullness = m_LBuckets.vc1_decFullness;
    m_LBuckets.vc1_Prev_enc_delay  = m_LBuckets.vc1_enc_delay;

    SetHRDFullness();

    Sts = RemoveFrame(frameSize);
    Sts = AddNewData();

    SetHRDRate(frameSize);

    m_recoding = 0;

#ifdef VC1_HRD_DEBUG
    printf("+ = ,%d,", m_IdealFrameSize);
    printf("- = ,%d,", frameSize*8);
    printf("decFullness = ,%d,",  m_LBuckets.vc1_decFullness);
    printf("encDelay    = ,%d\n", m_LBuckets.vc1_enc_delay);
#endif
    return Sts;
}

Ipp32s VC1HRDecoder::RemoveFrame(Ipp32s frameSize)
{
    Ipp32s Sts = VC1_HRD_OK;
    Ipp32s bufFullness = 0;
    frameSize = frameSize * 8;

    //real fullness
    bufFullness = m_LBuckets.vc1_decFullness + m_LBuckets.vc1_enc_delay;

    if(frameSize > m_LBuckets.vc1_decBufferSize)
    {
#ifdef VC1_HRD_DEBUG
     printf("VC1_HRD_ERR_BIG_FRAME\n");
#endif
     return VC1_HRD_ERR_BIG_FRAME;
   }

    if(frameSize > bufFullness)
    {
        Sts = VC1_HRD_ERR_DEC_NOT_ENOUGH_DATA;
        m_LBuckets.vc1_enc_delay = bufFullness - frameSize;
        m_LBuckets.vc1_decFullness = 0;
    }
    else
    {
       m_LBuckets.vc1_decFullness = bufFullness - frameSize;

       if(m_LBuckets.vc1_decFullness < m_LBuckets.vc1_decBufferSize)
            m_LBuckets.vc1_enc_delay = 0;
       else
       {
           m_LBuckets.vc1_enc_delay = m_LBuckets.vc1_decFullness - m_LBuckets.vc1_decBufferSize;
           m_LBuckets.vc1_decFullness = m_LBuckets.vc1_decBufferSize;
           Sts = VC1_HRD_ERR_DEC_NOT_ENOUGH_BUFFER;
       }
    }

    if(m_LBuckets.vc1_enc_delay > m_LBuckets.vc1_decBufferSize)
        m_LBuckets.vc1_enc_delay = m_LBuckets.vc1_decBufferSize;
    if(m_LBuckets.vc1_enc_delay < - m_LBuckets.vc1_decBufferSize)
        m_LBuckets.vc1_enc_delay = - m_LBuckets.vc1_decBufferSize;

    return Sts;
}

Ipp32s VC1HRDecoder::AddNewData()
{
    Ipp32s Sts = VC1_HRD_OK;
    Ipp32s bufFullness = 0;

    bufFullness = m_LBuckets.vc1_decFullness + m_IdealFrameSize +  m_LBuckets.vc1_enc_delay;

    if(bufFullness <= m_LBuckets.vc1_decBufferSize)
    {
        if(bufFullness >= 0)
        {
            m_LBuckets.vc1_decFullness = bufFullness;
            m_LBuckets.vc1_enc_delay = 0;
        }
        else
        {
            m_LBuckets.vc1_decFullness = 0;
            m_LBuckets.vc1_enc_delay = bufFullness;
            Sts = VC1_HRD_ERR_ENC_NOT_ENOUGH_DATA;
        }
    }
    else
    {
        //bits, whitch not could be written to buffer
        m_LBuckets.vc1_enc_delay = bufFullness - m_LBuckets.vc1_decBufferSize;

        m_LBuckets.vc1_decFullness = m_LBuckets.vc1_decBufferSize;
        Sts = VC1_HRD_ERR_ENC_NOT_ENOUGH_BUFFER;
    }

    if(m_LBuckets.vc1_enc_delay > m_LBuckets.vc1_decBufferSize)
        m_LBuckets.vc1_enc_delay = m_LBuckets.vc1_decBufferSize;
    if(m_LBuckets.vc1_enc_delay < - m_LBuckets.vc1_decBufferSize)
        m_LBuckets.vc1_enc_delay = - m_LBuckets.vc1_decBufferSize;

    return Sts;
}

void VC1HRDecoder::GetHRDParams(VC1_hrd_OutData* hrdParams)
{
    memset(hrdParams, 0, sizeof(VC1_hrd_OutData));
    hrdParams->hrd_buffer   = hrd_data.hrd_buffer;
    hrdParams->hrd_fullness = hrd_data.hrd_fullness;
    hrdParams->hrd_max_rate = hrd_data.hrd_max_rate;
}

void VC1HRDecoder::SetHRDRate(Ipp32s frameSize)
{
    Ipp32s enc_delay =  m_LBuckets.vc1_decFullness - m_LBuckets.vc1_decInitFull
        + m_LBuckets.vc1_enc_delay;

    Ipp32s prev_enc_delay  = m_LBuckets.vc1_decPrevFullness - m_LBuckets.vc1_decInitFull
        + m_LBuckets.vc1_Prev_enc_delay;

    enc_delay = enc_delay - prev_enc_delay;

    Ipp64f enc_delayTime = enc_delay/m_LBuckets.vc1_BitRate;

    //should be change for interlaced and field frames
   Ipp64f T = 1/m_LBuckets.vc1_FrameRate*m_recoding;

   if((T + enc_delayTime == 0) || (m_recoding == 0))
   {
       hrd_data.hrd_max_rate = m_LBuckets.vc1_BitRate;
       return;
   }

   Ipp64f rate = (frameSize*8)/(T + enc_delayTime);

   if(rate > hrd_data.hrd_max_rate)
       hrd_data.hrd_max_rate = (Ipp32s)rate;

#ifdef VC1_HRD_DEBUG
    printf("max_rate = ,%d\n", hrd_data.hrd_max_rate);
#endif
}

void VC1HRDecoder::SetHRDFullness()
{
    Ipp32s fullness = 0;
    Ipp32s temp = 0;
    Ipp32u r    = 0;

    if( m_LBuckets.vc1_decFullness < m_LBuckets.vc1_decBufferSize)
        fullness = m_LBuckets.vc1_decFullness;
    else
        fullness = m_LBuckets.vc1_decBufferSize;

    if(fullness < 0)
        fullness = 0;

    //ROUNDUP fullness/m_LBuckets.vc1_decBufferSize
    temp = fullness/m_LBuckets.vc1_decBufferSize;
    r = ((Ipp32u)(temp * m_LBuckets.vc1_decBufferSize - fullness))>>31;
    temp += r;

    hrd_data.hrd_fullness =256*temp - 1;

#ifdef VC1_HRD_DEBUG
    printf("fullness = ,%d\n", hrd_data.hrd_fullness);
#endif
}

void VC1HRDecoder::SetBitRate(Ipp32s bitRate)
{
    m_LBuckets.vc1_BitRate = bitRate;

    //in bits
    m_IdealFrameSize = (Ipp32u)(m_LBuckets.vc1_BitRate/m_LBuckets.vc1_FrameRate);
}

void VC1HRDecoder::SetFrameRate(Ipp32s frameRate)
{
     m_LBuckets.vc1_FrameRate = frameRate;

     //in bits
     m_IdealFrameSize = (Ipp32u)(m_LBuckets.vc1_BitRate/m_LBuckets.vc1_FrameRate);
}
}
#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)

