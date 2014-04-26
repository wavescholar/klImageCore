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

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_bit_rate_control.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_enc_debug.h"

#include <new>

namespace UMC_VC1_ENCODER
{

VC1BitRateControl::VC1BitRateControl()
{
    m_BRC = NULL;
    m_HRD = NULL;
    m_bitrate = 1024000;
    m_LBucketNum = 0;
    m_framerate = 30;
    m_currSize = 0;
    m_GOPLength = 1;
    m_BFrLength = 0;
    m_encMode = VC1_BRC_HIGHT_QUALITY_MODE;
    m_lastQuant = 1;

#ifdef BRC_TEST
    memset(&BRCFileSizeTest, 0, sizeof(BRCFileSizeTest));
#endif
}

Ipp32s VC1BitRateControl::CalcAllocMemorySize(Ipp32u GOPLength, Ipp32u BFrLength)
{
    Ipp32u mem_size = 0;

    if(GOPLength == 1)
        mem_size +=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_I));
    else if (BFrLength == 0)
         mem_size +=   UMC::align_value<Ipp32u>(sizeof(VC1BRC_IP));
    else
         mem_size +=   UMC::align_value<Ipp32u>(sizeof(VC1BRC_IPB));

    mem_size +=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder*))*VC1_ENC_MAX_NUM_LEAKY_BUCKET;


    mem_size +=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder))*VC1_ENC_MAX_NUM_LEAKY_BUCKET;

   return mem_size;
}

UMC::Status VC1BitRateControl::Init(Ipp8u* pBuffer, Ipp32s AllocatedMemSize,
                                    Ipp32u yuvFSize,  Ipp32u bitrate, Ipp64f framerate, Ipp32u mode,
                                    Ipp32u GOPLength, Ipp32u BFrLength, Ipp8u doubleQuant, Ipp8u QuantMode)
{
    UMC::Status VC1Sts = UMC::UMC_OK;
    Ipp8u* ptr = pBuffer;
    Ipp32s i = 0;
    Ipp32s memSize = AllocatedMemSize;

    if(GOPLength == 0)
        return UMC::UMC_ERR_ALLOC;

    if(QuantMode == 0)
        return UMC::UMC_ERR_INIT;

    Close();

    m_encMode = mode;

    if(doubleQuant)
        m_encMode = VC1_BRC_CONST_QUANT_MODE;

    if(bitrate == 0)  bitrate = 2*1024000;
    if(framerate == 0)framerate = 30;
    if(framerate > 100)framerate = 100;
    if(bitrate > (Ipp32u)(yuvFSize*8*framerate))
        bitrate = (Ipp32u)(yuvFSize*8*framerate);

    m_bitrate   = bitrate;
    m_framerate = framerate;
    m_GOPLength = GOPLength;
    m_BFrLength = BFrLength;

    //hypothetical reference decoder
    m_HRD =  new (ptr) (VC1HRDecoder*);
    ptr +=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder*))*VC1_ENC_MAX_NUM_LEAKY_BUCKET;
    memSize -=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder*))*VC1_ENC_MAX_NUM_LEAKY_BUCKET;
    if(!m_HRD || memSize < 0)
    {
       Close();
       return UMC::UMC_ERR_ALLOC;
    }

    for(i = 0; i < VC1_ENC_MAX_NUM_LEAKY_BUCKET; i++)
    {
        m_HRD[i] = new (ptr) VC1HRDecoder();
        if (!m_HRD[i])
        {
            Close();
            return UMC::UMC_ERR_ALLOC;
        }
        ptr +=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder));
        memSize -=  UMC::align_value<Ipp32u>(sizeof(VC1HRDecoder));
        if(!m_HRD[i] || memSize < 0)
        {
           Close();
           return UMC::UMC_ERR_ALLOC;
        }
    }

    if(GOPLength == 1)
    {
        m_BRC = new VC1BRC_I();
        ptr +=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_I));
        memSize -=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_I));
    }
    else if (BFrLength == 0)
    {
        m_BRC = new VC1BRC_IP();
        ptr +=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_IP));
        memSize -=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_IP));
    }
    else
    {
        m_BRC = new VC1BRC_IPB();
        ptr +=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_IPB));
        memSize -=  UMC::align_value<Ipp32u>(sizeof(VC1BRC_IPB));
    }

    if(!m_BRC || memSize < 0)
    {
        Close();
        return UMC::UMC_ERR_ALLOC;
    }

    m_BRC->Init(yuvFSize, m_bitrate,m_framerate,m_encMode,m_GOPLength,BFrLength,doubleQuant, QuantMode);

#ifdef BRC_TEST
    memset(&BRCFileSizeTest, 0, sizeof(BRCFileSizeTest));
    BRCFileSizeTest.RES_File = fopen("BRCTest.txt","a");
#endif

    return VC1Sts;
}

VC1BitRateControl::~VC1BitRateControl()
{
    Close();
}

UMC::Status VC1BitRateControl::InitBuffer(Ipp32s profile, Ipp32s level,Ipp32s BufferSize, Ipp32s initFull)
{
    UMC::Status UMCSts = UMC::UMC_OK;

    if(BufferSize == 0)
        return UMCSts;

    if(m_LBucketNum + 1 >= VC1_ENC_MAX_NUM_LEAKY_BUCKET)
        return UMC::UMC_ERR_FAILED;

    UMCSts = m_HRD[m_LBucketNum]->InitBuffer(profile, level, BufferSize, initFull, m_bitrate, m_framerate);
    m_LBucketNum++;

    return UMCSts;
}


void VC1BitRateControl::Close()
{
//------------------------------------------------------
#ifdef BRC_TEST
    if(BRCFileSizeTest.RES_File)
        fclose(BRCFileSizeTest.RES_File);
    memset(&BRCFileSizeTest, 0, sizeof(BRCFileSizeTest));
#endif
//------------------------------------------------------
 m_HRD = NULL;
 m_LBucketNum = 0;
 m_BRC = NULL;

 Reset();
}


void VC1BitRateControl::Reset()
{
    Ipp32s i = 0;

    //hypothetical ref decoder
    for(i = 0; i < m_LBucketNum; i++)
    {
        if(m_HRD[i])
            m_HRD[i]->Reset();
    }

    if(m_BRC)
        m_BRC->Reset();
}

void VC1BitRateControl::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half)
{
    m_BRC->GetQuant(picType, PQuant, Half);
    m_lastQuant = 2*(*PQuant) + (*Half);
}

void VC1BitRateControl::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform)
{
    m_BRC->GetQuant(picType, PQuant, Half, Uniform);
    m_lastQuant = 2*(*PQuant) + (*Half);
}

Ipp8u VC1BitRateControl::GetLastQuant()
{
    return m_lastQuant;
}

UMC::Status VC1BitRateControl::CheckFrameCompression(ePType picType, Ipp32u currSize)
{
    UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32s Sts = 0;
    Ipp32s i = 0;

    VM_ASSERT(currSize > 0);

    if(currSize == 0)
    {
        return UMC::UMC_ERR_INVALID_PARAMS;
    }

    m_currSize = currSize;

#ifdef BRC_TEST
     if(BRCFileSizeTest.RES_File)
        fprintf(BRCFileSizeTest.RES_File, "%d,", currSize);
#endif

    //check HRD status
    for(i = 0; i < m_LBucketNum; i++)
    {
        Sts = m_HRD[i]->CheckBuffer(m_currSize);
        UMCSts |= HandleHRDResult(Sts);
    }

#ifdef VC1_HRD_DEBUG
  //  printf("CheckBuffer Sts = %d\n", Sts);
#endif

    //    UMCSts = UMC::UMC_OK;

    UMCSts = m_BRC->CheckFrameCompression(picType, m_currSize, UMCSts);
    return UMCSts;
}

UMC::Status VC1BitRateControl::CompleteFrame(ePType picType)
{
    Ipp32s Sts = 0;
    UMC::Status umcSts = UMC::UMC_OK;
    Ipp32s i = 0;

    for(i = 0; i < m_LBucketNum; i++)
    {
        Sts     = m_HRD[i]->ReleaseData(m_currSize);
        umcSts |= HandleHRDResult(Sts);
    }

#ifdef VC1_HRD_DEBUG
   // printf("CompleteFrame Sts = %d\n", Sts);
#endif

    if(m_encMode == VC1_BRC_CONST_QUANT_MODE)
        return umcSts;

    m_BRC->CompleteFrame(picType);

   return umcSts;
}


void VC1BitRateControl::GetAllHRDParams(VC1_HRD_PARAMS* param)
{
    VC1_hrd_OutData hrdParams;
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s BufferSize[32];
    Ipp32s Fullness[32];
    Ipp32s Rate[32];
    Ipp32s rate_exponent = 0;;
    Ipp32s buffer_size_exponent = 0;
    Ipp32s temp = 0;


    param->HRD_NUM_LEAKY_BUCKETS = m_LBucketNum;

    if(m_LBucketNum == 0)
        return;

    for(i = 0; i < m_LBucketNum; i++)
    {
        m_HRD[i]->GetHRDParams(&hrdParams);
        Fullness[i]   = hrdParams.hrd_fullness;
        BufferSize[i] = hrdParams.hrd_buffer;
        Rate[i]       = hrdParams.hrd_max_rate;
    }

    rate_exponent        = GetHRD_Rate(Rate);
    buffer_size_exponent = GetHRD_Buffer(BufferSize);

    param->BIT_RATE_EXPONENT = rate_exponent - 6;
    VM_ASSERT (param->BIT_RATE_EXPONENT > 5);
    VM_ASSERT (param->BIT_RATE_EXPONENT < 22);

    param->BUFFER_SIZE_EXPONENT = buffer_size_exponent - 4;
    VM_ASSERT(param->BUFFER_SIZE_EXPONENT > 3);
    VM_ASSERT(param->BUFFER_SIZE_EXPONENT < 20);

    for(i = 0; i < m_LBucketNum; i++)
    {
        Rate[i] = Rate[i]>>rate_exponent;
        BufferSize[i] = BufferSize[i]>>buffer_size_exponent;
    }

    //Sort data
    for(i = 0; i < m_LBucketNum - 1; i++)
        for(j = i; j < m_LBucketNum - 1; j++)
        {
            if(Rate[i] > Rate[i+1])
            {
                temp = Rate[i];
                Rate[i] = Rate[i+1];
                Rate[i+1] = temp;

                temp = BufferSize[i];
                BufferSize[i] = BufferSize[i+1];
                BufferSize[i+1] = temp;

                temp = Fullness[i];
                Fullness[i] = Fullness[i+1];
                Fullness[i+1] = temp;
            }
        }

    for(i = 0; i < m_LBucketNum; i++)
    {
        param->HRD_RATE[i]     = Rate[i];
        param->HRD_BUFFER[i]   = BufferSize[i] - 1;
        param->HRD_FULLNESS[i] = Fullness[i] - 1;
    }

}

UMC::Status VC1BitRateControl::GetCurrentHRDParams(Ipp32s HRDNum, VC1_hrd_OutData* hrdParams)
{
    UMC::Status err = UMC::UMC_OK;

    if(!m_HRD)
        return UMC::UMC_ERR_NOT_INITIALIZED;

    m_HRD[HRDNum]->GetHRDParams(hrdParams);

    return err;
}

Ipp32s VC1BitRateControl::GetHRD_Rate(Ipp32s* rate)
{
    Ipp32s exponent = 32;
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s zero_num = 0;
    Ipp32s temp = 0;

    for(i = 0; i < m_LBucketNum; i++)
    {
        zero_num = 0;
        temp = rate[i];
        j = 0;
        while((j < 32) && ((temp & 1)==0))
        {
            temp>>=1;
            zero_num++;
            j++;
        }

        if(zero_num < exponent)
            exponent = zero_num;
    }

    if(exponent > 27)
        exponent = 27;
    if(exponent < 12)
        exponent = 12;

    return exponent;
}

Ipp32s VC1BitRateControl::GetHRD_Buffer(Ipp32s* size)
{
    Ipp32s exponent = 32;
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s zero_num = 0;
    Ipp32s temp = 0;

    for(i = 0; i < m_LBucketNum; i++)
    {
        zero_num = 0;
        temp = size[i];
        j = 0;
        while((j < 32) && ((temp & 1)==0))
        {
            temp>>=1;
            zero_num++;
            j++;
        }

        if(zero_num < exponent)
            exponent = zero_num;
    }

    if(exponent > 23)
        exponent = 23;
    if(exponent < 8)
        exponent = 8;

    return exponent;
}
Ipp8u VC1BitRateControl::GetLevel(Ipp32u profile, Ipp32u bitrate, Ipp32u widthMB, Ipp32u heightMB)
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp8u level = 0;
    Ipp32u mbNum = widthMB * heightMB;

    if(profile == VC1_ENC_PROFILE_A)        j = 2;
    else
        if(profile == VC1_ENC_PROFILE_M)    j = 1;

    while((i < 4) && (rMax_LevelLimits[j][i] < bitrate))
        i++;

    while((i < 4) && (MBfLimits[j][i] < mbNum))
        i++;


    if(profile == VC1_ENC_PROFILE_A)
    {
        if(i == 4)
            level = VC1_ENC_LEVEL_4;
        else if (i == 3)
            level = VC1_ENC_LEVEL_3;
        else if (i == 2)
            level = VC1_ENC_LEVEL_2;
        else if (i == 1)
            level = VC1_ENC_LEVEL_1;
        else
            level = VC1_ENC_LEVEL_0;

        if((rMax_LevelLimits[j][i]) == 0)
            level = VC1_ENC_LEVEL_4;
    }
    else if(profile == VC1_ENC_PROFILE_M)
        {
            if(i == 2)
                level = VC1_ENC_LEVEL_H;
            else if(i == 1)
                level = VC1_ENC_LEVEL_M;
            else
                level = VC1_ENC_LEVEL_S;

            if((rMax_LevelLimits[j][i]) == 0)
                level = VC1_ENC_LEVEL_H;
        }
        else
        {
            if(i == 1)
                level = VC1_ENC_LEVEL_M;
            else
                level = VC1_ENC_LEVEL_S;

            if((rMax_LevelLimits[j][i]) == 0)
                level = VC1_ENC_LEVEL_M;
        }
    return level;
}


UMC::Status VC1BitRateControl::HandleHRDResult(Ipp32s hrdStatus)
{
    UMC::Status umcSts = UMC::UMC_OK;

    //HRD decoder returns decoder buffer status, it should be corrected for encoder
    //if decoder status NOT_ENOUGH_DATA, it means that frames size is very big,
    //decoder gets more bits(frame size), than it is possible for current bitrate

    if(hrdStatus == VC1_HRD_OK)
        umcSts = UMC::UMC_OK;
    else if((hrdStatus & VC1_HRD_ERR_ENC_NOT_ENOUGH_DATA) || (hrdStatus & VC1_HRD_ERR_DEC_NOT_ENOUGH_DATA))
            umcSts = UMC::UMC_ERR_NOT_ENOUGH_BUFFER; //frame size is very big, no data for decoder
    else if((hrdStatus & VC1_HRD_ERR_ENC_NOT_ENOUGH_BUFFER) || (hrdStatus & VC1_HRD_ERR_DEC_NOT_ENOUGH_BUFFER))
            umcSts = UMC::UMC_ERR_NOT_ENOUGH_DATA;  //frame size is very small, no place for adding data
    else umcSts = UMC::UMC_ERR_FAILED;

    return umcSts;
}

//Ipp32s VC1BitRateControl::CheckBlockQuality(Ipp16s* pSrc, Ipp32u srcStep,
//                                             Ipp32u quant, Ipp32u intra)
//{
//    Ipp32u i = 0;
//    Ipp32u j = 0;
//    Ipp32s temp = 0;
//    Ipp16s* ptr = pSrc;
//    Ipp32u zeroCoef = 0;
//
//    for(i = intra; i < VC1_PIXEL_IN_BLOCK; i++)
//    {
//        for (j = 0; j < VC1_PIXEL_IN_BLOCK; j++)
//        {
//            temp = ptr[j]/quant;
//            zeroCoef += !temp;
//        }
//
//        ptr+=srcStep;
//    }
//
//    return zeroCoef;
//}

UMC::Status VC1BitRateControl::GetBRInfo(VC1BRInfo* pInfo)
{
    UMC::Status err = UMC::UMC_OK;
    Ipp8u pQuant;
    bool   HalfQP;
    pInfo->bitrate    =  m_bitrate;
    pInfo->framerate  =  m_framerate;

    if(!m_BRC)
        return UMC::UMC_ERR_NOT_INITIALIZED;

    if(m_encMode == VC1_BRC_CONST_QUANT_MODE)
    {
        m_BRC->GetQuant(VC1_ENC_I_FRAME, &pQuant, &HalfQP);
        pInfo->constQuant = (pQuant<<1) + HalfQP;
    }
    else
    {
        pInfo->constQuant = 0;
    }

    return err;
}

UMC::Status VC1BitRateControl::ChangeGOPParams(Ipp32u GOPLength, Ipp32u BFrLength)
{
    //TODO possible need to change type of BRC
    return m_BRC->SetGOPParams(GOPLength, BFrLength);
}

}
#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
