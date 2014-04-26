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

#ifndef _ENCODER_VC1_HRD_H_
#define _ENCODER_VC1_HRD_H_

#include "umc_structures.h"
#include "umc_vc1_common_defs.h"

#include "ippdefs.h"

namespace UMC_VC1_ENCODER
{
//#define VC1_HRD_DEBUG

extern Ipp32u bMax_LevelLimits[3][5];
extern Ipp32u rMax_LevelLimits[3][5];
extern Ipp32u MBfLimits[3][5];


enum
{
    VC1_HRD_OK           = 0x0,
    //VC1_HRD_OK_SMALL     = 0x1,
    //VC1_HRD_OK_BIG       = 0x2,
    //VC1_HRD_ERR_SMALL    = 0x4,
    //VC1_HRD_ERR_BIG      = 0x8,
    VC1_HRD_ERR_ENC_NOT_ENOUGH_DATA   = 0x10,
    VC1_HRD_ERR_ENC_NOT_ENOUGH_BUFFER = 0x20,
    VC1_HRD_ERR_DEC_NOT_ENOUGH_DATA   = 0x40,
    VC1_HRD_ERR_DEC_NOT_ENOUGH_BUFFER = 0x80,
    VC1_HRD_ERR_BIG_FRAME             = 0x100

};

typedef struct
{
    Ipp32s vc1_decFullness;
    Ipp32s vc1_decPrevFullness;
    Ipp32s vc1_decBufferSize;
    Ipp32s vc1_BitRate;
    Ipp64f vc1_FrameRate;
    Ipp32s vc1_decInitFull;
    Ipp32s vc1_dec_delay;
    Ipp32s vc1_enc_delay;
    Ipp32s vc1_Prev_enc_delay;
}VC1_enc_LeakyBucketInfo;

typedef struct
{
    Ipp32s hrd_max_rate;                //maximum rate
    Ipp32s hrd_buffer;                  //buffer size
    Ipp32s hrd_fullness;                //hrd fullness
}VC1_hrd_OutData;

class VC1HRDecoder
{
    public:
        VC1HRDecoder();
        ~VC1HRDecoder();
        UMC::Status InitBuffer(Ipp32s profile,   Ipp32s level,
                               Ipp32s bufferSize,Ipp32s initFull,
                               Ipp32s bitRate,   Ipp64f frameRate);

        void Reset();

        Ipp32s CheckBuffer(Ipp32s frameSize);
        Ipp32s ReleaseData(Ipp32s frameSize);


        void SetBitRate(Ipp32s bitRate);
        void SetFrameRate(Ipp32s frameRate);
        void GetHRDParams(VC1_hrd_OutData* hrdParams);
protected:
        Ipp32s AddNewData();
        Ipp32s RemoveFrame(Ipp32s frameSize);
        void   SetHRDRate(Ipp32s frameSize);
        void   SetHRDFullness();

    protected:
        VC1_enc_LeakyBucketInfo m_LBuckets;
        Ipp32s m_IdealFrameSize;            //in bits
        VC1_hrd_OutData hrd_data;
        //Flags
        Ipp32s m_recoding;         //frame was recoded


};
}

#endif
