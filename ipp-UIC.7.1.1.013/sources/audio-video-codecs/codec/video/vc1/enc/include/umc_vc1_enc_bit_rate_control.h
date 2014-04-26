/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, bit rate control
//
*/

#ifndef _ENCODER_VC1_BITRATE_H_
#define _ENCODER_VC1_BITRATE_H_

#include "umc_vc1_enc_hrd.h"
#include "umc_vc1_enc_def.h"
#include "umc_vc1_enc_brc_gop.h"

#include "ippdefs.h"

namespace UMC_VC1_ENCODER
{

#define VC1_ENC_MAX_NUM_LEAKY_BUCKET 31 //0-31
#define VC1_ENC_LEAKY_BUCKET_NUM     1


#ifdef VC1_ENC_DEBUG_ON
  //#define BRC_TEST
#endif

#ifdef BRC_TEST
typedef struct
{
    FILE* RES_File;
} BRCTest;
#endif

typedef struct
{
    Ipp32s bitrate;
    Ipp64f framerate;
    Ipp8u  constQuant;
}VC1BRInfo;

class VC1BitRateControl
{
    public:
        VC1BitRateControl();
        ~VC1BitRateControl();

    protected:

        Ipp32u m_bitrate;           //current bitrate
        Ipp64f m_framerate;         //current frame rate

        //GOP BRC
        VC1BRC*                     m_BRC;

        //hypothetical ref decoder
        VC1HRDecoder** m_HRD;
        Ipp32s m_LBucketNum;

        //Frames parameters
        Ipp32s m_currSize;         //last coded frame size

        //GOP parameters
        Ipp32u m_GOPLength;         //number of frame in GOP
        Ipp32u m_BFrLength;         //number of successive B frames

         //coding params
         Ipp32u m_encMode;        //coding mode
         Ipp8u  m_lastQuant;      //last quant for skip frames

    public:
        static Ipp32s CalcAllocMemorySize(Ipp32u GOPLength, Ipp32u BFrLength);
        UMC::Status Init(Ipp8u* pBuffer,   Ipp32s AllocatedMemSize,
                         Ipp32u yuvFSize,  Ipp32u bitrate,
                         Ipp64f framerate, Ipp32u mode,
                         Ipp32u GOPLength, Ipp32u BFrLength, Ipp8u doubleQuant, Ipp8u QuantMode);

        UMC::Status InitBuffer(Ipp32s profile, Ipp32s level, Ipp32s BufferSize, Ipp32s initFull);

        void Reset();
        void Close();

        static Ipp8u GetLevel(Ipp32u profile, Ipp32u bitrate, Ipp32u widthMB, Ipp32u heightMB);

        //get and prepare data for coding (all data compressed as needed (see standard))
        void GetAllHRDParams(VC1_HRD_PARAMS* param);
        UMC::Status GetCurrentHRDParams(Ipp32s HRDNum, VC1_hrd_OutData* hrdParams);

        //return number of coeffs witch will be reduce by zero
        //1 - intra block, 0 - inter block
        //Ipp32s CheckBlockQuality(Ipp16s* pSrc, Ipp32u srcStep,
        //                         Ipp32u quant, Ipp32u intra);
        UMC::Status CheckFrameCompression(ePType picType, Ipp32u currSize);
        UMC::Status CompleteFrame(ePType picType);
        UMC::Status HandleHRDResult(Ipp32s hrdStatus);

        //return recomended quant
        void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half);
        void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform);

        UMC::Status GetBRInfo(VC1BRInfo* pInfo);

        UMC::Status ChangeGOPParams(Ipp32u GOPLength, Ipp32u BFrLength);

        Ipp8u GetLastQuant();

        Ipp32s GetLBucketNum();
protected:

        Ipp32s GetHRD_Rate  (Ipp32s* rate);
        Ipp32s GetHRD_Buffer(Ipp32s* size);

#ifdef BRC_TEST
        BRCTest  BRCFileSizeTest;
#endif
};

}

#endif
