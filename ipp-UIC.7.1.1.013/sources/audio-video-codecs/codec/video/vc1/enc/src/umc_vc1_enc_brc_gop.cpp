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

#include "umc_vc1_enc_brc_gop.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_enc_debug.h"

namespace UMC_VC1_ENCODER
{

VC1BRC::VC1BRC(): m_GOPLength (1), m_BFrLength (0), m_SizeAbberation(0),
                  m_recoding(0),   m_currSize(0),   m_IdealFrameSize(0),
                  m_picType(VC1_ENC_I_FRAME),    m_ratio_min(0.6),m_buffer_overflow(1.1),
                  m_encMode(VC1_BRC_HIGHT_QUALITY_MODE), m_QuantMode(3)
{
    m_CurQuant.PQIndex = 1;
    m_CurQuant.HalfPQ = 1;

    m_Quant.IQuant = 1;
    m_Quant.PQuant = 1;
    m_Quant.BQuant = 1;
    m_Quant.LimIQuant = 1;
    m_Quant.LimPQuant = 1;
    m_Quant.LimBQuant = 1;
    m_Quant.IHalf = 0;
    m_Quant.PHalf = 0;
    m_Quant.BHalf = 0;
};
VC1BRC::~VC1BRC()
{
};

////////////////////////////////////////////
//          VC1BRC_I
////////////////////////////////////////////
VC1BRC_I::VC1BRC_I():m_needISize(0), m_prefINeedSize(0)
{
};

VC1BRC_I::~VC1BRC_I()
{
    Close();
};

UMC::Status VC1BRC_I::Init(Ipp32u yuvFSize,  Ipp32u bitrate,  Ipp64f framerate,
                           Ipp32u mode,      Ipp32u GOPLength, Ipp32u BFrLength,
                           Ipp8u doubleQuant,Ipp8u QuantMode)
 {
    UMC::Status VC1Sts = UMC::UMC_OK;

    m_bitrate   = bitrate;
    m_framerate = framerate;
    m_GOPLength = GOPLength;
    m_BFrLength = BFrLength;
    m_encMode   = mode;
    m_QuantMode = QuantMode;

    VM_ASSERT(QuantMode == 0);

    m_IdealFrameSize = (Ipp32u)(bitrate/framerate/8);

    m_needISize = m_IdealFrameSize;
    m_prefINeedSize = m_needISize;

#ifdef VC1_GOP_DEBUG
    printf("\n\n m_GOPSize = %d\n", m_needISize);
#endif

    if(!doubleQuant)
    {
        Ipp32u AverageCompression = ((yuvFSize)/m_needISize)/3;

        VC1_CHECK_QUANT(AverageCompression);
        m_CurQuant.PQIndex = m_Quant.IQuant = (Ipp8u)AverageCompression;
    }
    else
    {
        if(doubleQuant > (VC1_MAX_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        if(doubleQuant < (VC1_MIN_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        m_encMode = VC1_BRC_CONST_QUANT_MODE;
        m_CurQuant.PQIndex = m_Quant.IQuant = m_Quant.PQuant = m_Quant.BQuant = (doubleQuant>>1);
        m_CurQuant.HalfPQ  = m_Quant.IHalf = m_Quant.PHalf = m_Quant.BHalf = doubleQuant&0x1;
    }

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
    case VC1_BRC_CONST_QUANT_MODE:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    default:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    }

    return VC1Sts;
 };

void VC1BRC_I::Close()
{
    Reset();
};

void VC1BRC_I::Reset()
{
    //GOP parameters
    m_SizeAbberation = 0;

    //Frames parameters
    m_currSize = 0;

    //Flags
    m_recoding = 0;
};

UMC::Status VC1BRC_I::CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts)
{
    UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32f ratio = 0;

    if(currSize == 0)
        return UMC::UMC_ERR_INVALID_PARAMS;

    m_currSize = currSize;

    if(picType == VC1_ENC_SKIP_FRAME)
    {
#ifdef VC1_GOP_DEBUG
     ratio = ((Ipp32f)currSize/(Ipp32f)m_needISize);

     printf("Skip");
     printf("%d  %d % d\n", m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif
        return UMCSts;
    }

    ratio = ((Ipp32f)currSize/(Ipp32f)m_needISize);

#ifdef VC1_BRC_DEBUG
    printf("current ratio = %f\n", ratio);
#endif

#ifdef VC1_GOP_DEBUG
     printf("I ");
     printf("current ratio = %f   %d  %d % d\n", ratio, m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
        CheckFrame_QualityMode(ratio, HRDSts);
        break;
    case VC1_BRC_CONST_QUANT_MODE:
        break;
    default:
        break;
    }
    m_recoding++;

#ifdef VC1_BRC_DEBUG
    printf("coded size %d\n", currSize);
#endif

    return UMCSts;
};

void VC1BRC_I::CompleteFrame(ePType picType)
{
    //UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32f ratioI = 0;
    Ipp32f ratioICorr = 1.0;

#ifdef BRC_TEST
     if(BRCFileSizeTest.RES_File)
        fprintf(BRCFileSizeTest.RES_File, "%d,",m_currSize);
#endif

    if(m_encMode == VC1_BRC_CONST_QUANT_MODE
        || !(picType== VC1_ENC_I_FRAME || picType== VC1_ENC_I_I_FIELD))
        return;

    if(picType != VC1_ENC_SKIP_FRAME)
    {
        m_prefINeedSize = m_needISize;
    }

    m_SizeAbberation += m_IdealFrameSize - m_currSize;

     //check overflow m_SizeAbberation
    if(m_SizeAbberation > 10*m_IdealFrameSize)
        m_SizeAbberation = 10*m_IdealFrameSize;

    if(m_SizeAbberation < (- 10*m_IdealFrameSize))
        m_SizeAbberation = -(10*m_IdealFrameSize);

    m_needISize = m_IdealFrameSize + m_SizeAbberation;

    //check m_needISize
    if(m_needISize > m_IdealFrameSize + (m_IdealFrameSize>>1))
    {
        m_needISize = m_IdealFrameSize + (m_IdealFrameSize>>1);
    }

    if(m_needISize < (m_IdealFrameSize>>1))
    {
        m_needISize = (m_IdealFrameSize>>1);
    }

    if(picType != VC1_ENC_SKIP_FRAME)
    {
        ratioI = ((Ipp32f)m_prefINeedSize/(Ipp32f)m_needISize)*ratioICorr;

        CorrectGOPQuant(ratioI);
    }
    else
    {
        #ifdef VC1_GOP_DEBUG
            printf("SKIP FRAME\n");
        #endif
        ratioICorr = (Ipp32f)(0.9);

        m_Quant.LimIQuant = m_Quant.LimIQuant--;
        VC1_CHECK_QUANT(m_Quant.LimIQuant);
        m_Quant.IQuant = m_Quant.IQuant--;
        VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);
    }

#ifdef VC1_GOP_DEBUG
        printf("nextGOPSize = %d\n", m_needISize);
        printf("m_SizeAbberation = %d\n", m_SizeAbberation);
        printf("ratioICorr = %f\n", ratioICorr);


    printf("\n-----------------------------------NEW GOP");
    printf("------------------------------------------\n\n\n");
#endif



    m_recoding = 0;

#ifdef VC1_BRC_DEBUG
    printf("\n\n\n");
#endif
};

void VC1BRC_I::CheckFrame_QualityMode(Ipp32f ratio, UMC::Status UMCSts)
{
    Ipp8u curQuant = 0;
    Ipp32s Sts = VC1_BRC_OK;
    bool HalfQP = 0;

    //new quant calculation
    curQuant = (Ipp8u)(ratio * m_CurQuant.PQIndex + 0.5);
    VC1_CHECK_QUANT(curQuant);

    //check compression
    if((ratio <= m_buffer_overflow) && (ratio >= VC1_GOOD_COMPRESSION))
        Sts = VC1_BRC_OK;       //good frame
    else if(ratio > m_buffer_overflow)
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER; //could be critical situation, if HRD buffer small
    else  if(ratio <= m_ratio_min)
    {
        //very small frame
        Sts |= VC1_BRC_ERR_SMALL_FRAME;

        if(m_CurQuant.PQIndex > 1)
            HalfQP = 1;
    }
    else if(ratio >= VC1_MIN_RATIO_HALF)
    {
        if((m_CurQuant.PQIndex > 1) && (curQuant > m_CurQuant.PQIndex))
            HalfQP = 1;
        Sts |= VC1_BRC_SMALL_FRAME;
    }
    else
         Sts |= VC1_BRC_SMALL_FRAME;

    //check HRD status
    if(UMCSts ==  UMC::UMC_ERR_NOT_ENOUGH_BUFFER)
    {
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER;
        HalfQP = 0;
    }

    if(Sts != VC1_BRC_OK)
    {
#ifdef VC1_BRC_DEBUG
    printf("m_LimIQuant %d\n", m_Quant.LimIQuant);
#endif
        m_Quant.IQuant = (m_Quant.IQuant + curQuant)/2;

        if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
        {
            if(ratio > VC1_RATIO_ILIM)
            {
                m_Quant.IQuant++;
                m_Quant.LimIQuant = m_CurQuant.PQIndex + 2;
            }
            else
            {
                if(m_CurQuant.PQIndex > 9)
                    m_Quant.LimIQuant = m_CurQuant.PQIndex + 1;
                else if(m_Quant.IQuant <= m_CurQuant.PQIndex)
                    HalfQP = 1;
            }
        }
        else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
        {
            m_Quant.LimIQuant--;

            if(m_Quant.IQuant >= m_CurQuant.PQIndex)
                m_Quant.IQuant--;

            if((ratio > VC1_MIN_RATIO_HALF) &&  (m_CurQuant.PQIndex > 1))
                HalfQP = 1;

        }
        else if(Sts & VC1_BRC_SMALL_FRAME)
        {
            m_Quant.LimIQuant--;

            if((m_Quant.IQuant < m_CurQuant.PQIndex) && m_Quant.IQuant &&  (m_CurQuant.PQIndex > 1))
                HalfQP = 1;

        }

        VC1_CHECK_QUANT(m_Quant.LimIQuant);
        VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);

        m_Quant.IHalf = HalfQP;
    }
 };

void VC1BRC_I::CorrectGOPQuant(Ipp32f ratio)
 {
     Ipp8u curQuant = 0;
    //new quant calculation
    curQuant = (Ipp8u)(ratio * m_Quant.IQuant + 0.5);
    VC1_CHECK_QUANT(curQuant);

    if(curQuant != m_Quant.IQuant)
        m_Quant.IHalf = 0;

    m_Quant.IQuant = (m_Quant.IQuant + curQuant + 1)/2;
    VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);
 };

void VC1BRC_I::GetQuant(ePType /*picType*/, Ipp8u* PQuant, bool* Half)
{
    m_CurQuant.PQIndex = m_Quant.IQuant;
    m_CurQuant.HalfPQ  = m_Quant.IHalf;

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;
}

void VC1BRC_I::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform)
{
    picType = picType;
    m_CurQuant.PQIndex = m_Quant.IQuant;
    m_CurQuant.HalfPQ  = m_Quant.IHalf;

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;

    switch (m_QuantMode)
    {
        case 0:
            VM_ASSERT(0);
            *Uniform = true;
            break;
        case 1:
            *Uniform = true;
            break;
        case 2:
            *Uniform = false;
            break;
        case 3:
            if(m_CurQuant.PQIndex > 8)
                *Uniform = false;
            else
                *Uniform =  true;
            break;
        default:
            VM_ASSERT(0);
            *Uniform = true;
            break;
    }
}

UMC::Status VC1BRC_I::SetGOPParams(Ipp32u /*GOPLength*/, Ipp32u /*BFrLength*/)
{
    return UMC::UMC_OK;
}

////////////////////////////////////////////
//          VC1BRC_IP
////////////////////////////////////////////
VC1BRC_IP::VC1BRC_IP()
{
    m_I_GOPSize = 0;
    m_P_GOPSize = 0;
    m_needISize = 0;
    m_needPSize = 0;
    m_INum = 0;
    m_PNum = 0;
    m_CurrINum = 0;
    m_CurrPNum = 0;
    m_currFrameInGOP = 0;
    m_poorRefFrame = 0;
    m_GOPHalfFlag = 0;
    m_GOPSize = 0;
    m_currGOPSize = 0;
    m_nextGOPSize = 0;
    m_currISize = 0;
    m_currPSize = 0;
    m_IP_size = (Ipp32f)(VC1_P_SIZE);
    m_failPQuant = 0;
    m_failGOP = 0;
    m_AverageIQuant = 0;
    m_AveragePQuant = 0;
};

VC1BRC_IP::~VC1BRC_IP()
{
    Close();
};

UMC::Status VC1BRC_IP::Init(Ipp32u yuvFSize,  Ipp32u bitrate,  Ipp64f framerate,
                           Ipp32u mode,      Ipp32u GOPLength, Ipp32u BFrLength,
                           Ipp8u doubleQuant, Ipp8u QuantMode)
 {
    UMC::Status VC1Sts = UMC::UMC_OK;

    if(doubleQuant)
    {
        if(doubleQuant > (VC1_MAX_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        if(doubleQuant < (VC1_MIN_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        m_encMode = VC1_BRC_CONST_QUANT_MODE;
    }

    m_bitrate   = bitrate;
    m_framerate = framerate;

    m_IdealFrameSize = (Ipp32u)(bitrate/framerate/8);

    VC1Sts = SetGOPParams(GOPLength, BFrLength);

    m_encMode        = mode;
    m_QuantMode      = QuantMode;

    VM_ASSERT(QuantMode != 0);

#ifdef VC1_GOP_DEBUG
    printf("\n\n m_GOPSize = %d\n", m_GOPSize);
#endif

    if(m_encMode == VC1_BRC_HIGHT_QUALITY_MODE)
    {
        Ipp32u AverageCompression = ((yuvFSize)/m_needISize)/3;

        VC1_CHECK_QUANT(AverageCompression);
        m_CurQuant.PQIndex = m_Quant.IQuant = (Ipp8u)AverageCompression;

        AverageCompression = ((yuvFSize)/m_needPSize)/5;
        VC1_CHECK_QUANT(AverageCompression);

        m_Quant.PQuant =  (Ipp8u)AverageCompression;

        if(m_Quant.PQuant > 2)
            m_Quant.PQuant -= 2;
    }
    else
    {
        m_CurQuant.PQIndex = m_Quant.IQuant = m_Quant.PQuant = m_Quant.BQuant = (doubleQuant>>1);
        m_CurQuant.HalfPQ  = m_Quant.IHalf = m_Quant.PHalf = m_Quant.BHalf = doubleQuant&0x1;
    }

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
    case VC1_BRC_CONST_QUANT_MODE:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    default:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    }

    return VC1Sts;
 };

void VC1BRC_IP::Close()
{
    Reset();
};

void VC1BRC_IP::Reset()
{
     //GOP parameters
    m_GOPSize      = m_IdealFrameSize * m_GOPLength;
    m_nextGOPSize  = m_GOPSize;
    m_currGOPSize = 0;

    m_I_GOPSize = m_P_GOPSize = 0;

    m_CurrINum = m_CurrPNum = 0;
    m_currFrameInGOP = 0;
    m_SizeAbberation = 0;

    //Frames parameters
    m_currSize = 0;

    m_needISize = (Ipp32s)(m_GOPSize/(1 + m_IP_size*m_PNum));
    m_needPSize = (Ipp32s)(m_IP_size*m_needISize);

    m_currISize = m_needISize;
    m_currPSize = m_needPSize;

    //Flags
    m_recoding     = 0;
    m_poorRefFrame = 0;
    m_GOPHalfFlag  = 1;
    m_failPQuant   = 0;
    m_failGOP      = 0;

    //Picture params
    m_picType = VC1_ENC_I_FRAME;
};

UMC::Status VC1BRC_IP::CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts)
{
    UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32f ratio = 0;

    if(currSize == 0)
        return UMC::UMC_ERR_INVALID_PARAMS;

    m_currSize = currSize;

    if(picType == VC1_ENC_SKIP_FRAME)
    {
#ifdef VC1_GOP_DEBUG
     ratio = ((Ipp32f)currSize/(Ipp32f)m_needISize);

     printf("Skip ");
     printf("%d  %d % d\n", m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif
        return UMCSts;
    }

    if(m_recoding)
        m_poorRefFrame &= 0x2;


    switch (picType)
    {
        case(VC1_ENC_I_FRAME):
        case(VC1_ENC_I_I_FIELD):
#ifdef VC1_GOP_DEBUG
     printf("I ");
#endif
            m_currISize = currSize;
            ratio = ((Ipp32f)m_currISize/(Ipp32f)m_needISize);
            if(ratio < VC1_POOR_REF_FRAME)
                m_poorRefFrame = 1;
            else
                m_poorRefFrame = 0;
            break;
        case(VC1_ENC_P_FRAME):
        case(VC1_ENC_P_FRAME_MIXED):
        case(VC1_ENC_P_I_FIELD):
        case(VC1_ENC_I_P_FIELD):
        case(VC1_ENC_P_P_FIELD):
#ifdef VC1_GOP_DEBUG
     printf("P ");
#endif
            m_currPSize = currSize;
            ratio = ((Ipp32f)m_currPSize/(Ipp32f)m_needPSize);
            if(ratio < VC1_POOR_REF_FRAME)
                m_poorRefFrame = 1;
            else
                m_poorRefFrame = 0;

            break;
        default:
            return UMCSts;
            break;
    }

#ifdef VC1_BRC_DEBUG
     printf("current ratio = %f\n", ratio);
#endif
#ifdef VC1_GOP_DEBUG
     printf("current ratio = %f   %d  %d %d\n", ratio, m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif

    if(m_failGOP)
        HRDSts = UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
        CheckFrame_QualityMode(picType, ratio, HRDSts);
        break;
    case VC1_BRC_CONST_QUANT_MODE:
        break;
    default:
        break;
    }
    m_recoding++;

#ifdef VC1_BRC_DEBUG
    printf("coded size %d\n", currSize);
#endif

    return UMCSts;
};
void VC1BRC_IP::CompleteFrame(ePType picType)
{
    //UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32f ratioI = 0;
    Ipp32f ratioP = 0;
    Ipp32s prefINeedSize = m_needISize;
    Ipp32s prefPNeedSize = m_needPSize;

#ifdef BRC_TEST
     if(BRCFileSizeTest.RES_File)
        fprintf(BRCFileSizeTest.RES_File, "%d,",m_currSize);
#endif

    if(m_encMode == VC1_BRC_CONST_QUANT_MODE)
        return;

    m_currFrameInGOP++;
    m_currGOPSize += m_currSize;

    switch (picType)
    {
    case VC1_ENC_I_FRAME:
    case VC1_ENC_I_I_FIELD:
        m_I_GOPSize += m_currSize;
        m_CurrINum++;
        m_AverageIQuant = m_CurQuant.PQIndex;
        break;
    case VC1_ENC_P_FRAME:
    case VC1_ENC_P_FRAME_MIXED:
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_P_GOPSize += m_currSize;
        m_CurrPNum++;
        m_AveragePQuant +=  (m_CurQuant.PQIndex*2 + m_CurQuant.HalfPQ);
        break;
    case VC1_ENC_SKIP_FRAME:
        if(m_CurrINum)
            m_PNum--;
        break;
    default:
        return;
        break;
    }

    if((m_GOPHalfFlag && ((m_currGOPSize > m_GOPSize/2) || m_currFrameInGOP == m_GOPLength/2))
        || (m_currFrameInGOP == 1) || m_failPQuant || picType == VC1_ENC_SKIP_FRAME)
    {

        Ipp32s PBSize = m_nextGOPSize - m_currGOPSize;

        if(m_currFrameInGOP != 1)
            m_GOPHalfFlag = 0;

        if(((PBSize) > m_IdealFrameSize) && ( m_currFrameInGOP != m_GOPLength))
        {
            m_needPSize = (Ipp32s)(PBSize/((m_PNum - m_CurrPNum)));
        }
        else
        {
            m_failGOP = 1;
            if(m_needPSize > m_IdealFrameSize/2)
                m_needPSize = (Ipp32s)(m_needPSize*0.75);
        }

        ratioP = ((Ipp32f)prefPNeedSize/(Ipp32f)m_needPSize);
        if(m_needPSize < (m_IdealFrameSize>>2))
            ratioP = 1;

        CorrectGOPQuant(VC1_ENC_P_FRAME, ratioP);
    }



    if(m_currFrameInGOP == m_GOPLength)
    {
        m_SizeAbberation += m_GOPSize - m_currGOPSize;

        //correction of I/P ratio
        if(m_I_GOPSize && m_P_GOPSize)
        {
            m_AveragePQuant /= (m_PNum*2);
            m_AveragePQuant++;
            Ipp32f P_average_size =  (Ipp32f)(m_P_GOPSize/m_CurrPNum);
            Ipp32f PRatio = (P_average_size*m_AveragePQuant)/(m_I_GOPSize*m_AverageIQuant);
            m_IP_size = (m_IP_size + PRatio)/2;

            if(m_IP_size > 1)
                m_IP_size = 1;

            #ifdef VC1_GOP_DEBUG
                    printf("\n\n I:P = %f\n\n", m_IP_size);
            #endif
        }

    #ifdef VC1_GOP_DEBUG
        printf("\n\n IQuant = %d    PQuant = %d\n\n", m_AverageIQuant, m_AveragePQuant);
        printf("\nGOP ratio = %f\n", ((Ipp32f)m_currGOPSize/(Ipp32f)m_nextGOPSize));
        printf("\nIdeal     = %f\n", ((Ipp32f)m_currGOPSize/(Ipp32f)m_GOPSize));

        printf("\n\n-----------------------------------NEW GOP");
        printf("------------------------------------------\n\n\n");
    #endif

        //check overflow m_SizeAbberation
        if(m_SizeAbberation > 4*m_GOPSize)
            m_SizeAbberation = 4*m_GOPSize;

        if(m_SizeAbberation < (- 4*m_GOPSize))
            m_SizeAbberation = -(4*m_GOPSize);

        SetGOPParams(m_GOPLength, 0);

        m_nextGOPSize = m_GOPSize + m_SizeAbberation;

        //check m_needISize
        if(m_nextGOPSize > m_GOPSize + (m_GOPSize>>1))
        m_nextGOPSize = m_GOPSize + (m_GOPSize>>1);

        if(m_nextGOPSize < (m_GOPSize>>1))
            m_nextGOPSize = (m_GOPSize>>1);

#ifdef VC1_GOP_DEBUG
    printf("nextGOPSize = %d\n", m_nextGOPSize);
    printf("m_SizeAbberation = %d\n", m_SizeAbberation);
#endif

        m_needISize = (Ipp32s)(m_nextGOPSize/(1 + m_IP_size*m_PNum));

        ratioI = ((Ipp32f)prefINeedSize/(Ipp32f)m_needISize);

        CorrectGOPQuant(VC1_ENC_I_FRAME, ratioI);
    }

    m_recoding = 0;
    m_failPQuant = 0;

#ifdef VC1_BRC_DEBUG
    printf("\n\n\n");
#endif
};


void VC1BRC_IP::CheckFrame_QualityMode(ePType picType, Ipp32f ratio, UMC::Status UMCSts)
{
    Ipp8u curQuant = 0;
    Ipp32s Sts = VC1_BRC_OK;
    bool HalfQP = 0;

    //new quant calculation
    curQuant = (Ipp8u)(ratio * m_CurQuant.PQIndex + 0.5);
    VC1_CHECK_QUANT(curQuant);

    //check compression
    if((ratio <= m_buffer_overflow) && (ratio >= VC1_GOOD_COMPRESSION))
        Sts = VC1_BRC_OK;       //good frame
    else if(ratio > m_buffer_overflow)
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER; //could be critical situation, if HRD buffer small
    else  if(ratio <= m_ratio_min)
    {
        //very small frame
        Sts |= VC1_BRC_ERR_SMALL_FRAME;

        if(m_CurQuant.PQIndex > 1)
            HalfQP = 1;
    }
    else if(ratio >= VC1_MIN_RATIO_HALF)
    {
        if((m_CurQuant.PQIndex > 1) && ((Ipp32u)curQuant > m_CurQuant.PQIndex))
            HalfQP = 1;
        Sts |= VC1_BRC_SMALL_FRAME;
    }
    else
         Sts |= VC1_BRC_SMALL_FRAME;

    //check HRD status
    if(UMCSts ==  UMC::UMC_ERR_NOT_ENOUGH_BUFFER)
    {
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER;
        HalfQP = 0;
    }

    if(Sts != VC1_BRC_OK)
    switch (picType)
    {
        case(VC1_ENC_I_FRAME):
        case (VC1_ENC_I_I_FIELD):
            {
#ifdef VC1_BRC_DEBUG
            printf("m_LimIQuant %d\n", m_Quant.LimIQuant);
#endif
            m_Quant.IQuant = (m_Quant.IQuant + curQuant)/2;

            if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
            {
                if(ratio > VC1_RATIO_ILIM)
                {
                   m_Quant.IQuant++;
                   m_Quant.LimIQuant = m_CurQuant.PQIndex + 2;
                }
                else
                {
                    if(m_CurQuant.PQIndex > 9)
                        m_Quant.LimIQuant = m_CurQuant.PQIndex + 1;
                    else if(m_Quant.IQuant <= m_CurQuant.PQIndex)
                        HalfQP = 1;
                }
            }
            else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
            {
                m_Quant.LimIQuant--;

                if(m_Quant.IQuant >= m_CurQuant.PQIndex)
                    m_Quant.IQuant--;

                if((ratio > VC1_MIN_RATIO_HALF) &&  (m_CurQuant.PQIndex > 1))
                    HalfQP = 1;

            }
            else if(Sts & VC1_BRC_SMALL_FRAME)
            {
                m_Quant.LimIQuant--;

               if((m_Quant.IQuant < m_CurQuant.PQIndex) && m_Quant.IQuant &&  (m_CurQuant.PQIndex > 1))
                    HalfQP = 1;

            }

            VC1_CHECK_QUANT(m_Quant.LimIQuant);
            VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);

            m_Quant.IHalf = HalfQP;
            }
            break;
        case(VC1_ENC_P_FRAME):
        case(VC1_ENC_P_FRAME_MIXED):
        case(VC1_ENC_P_I_FIELD):
        case(VC1_ENC_I_P_FIELD):
        case(VC1_ENC_P_P_FIELD):
            {
#ifdef VC1_BRC_DEBUG
            printf("m_LimPQuant %d\n", m_Quant.LimPQuant);
#endif

            if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
            {
                if(ratio > VC1_RATIO_ILIM)
                {
                    m_failPQuant   = (m_Quant.PQuant + curQuant + 1)/2 - m_Quant.PQuant;

                    m_Quant.PQuant++;
                    m_Quant.LimPQuant = m_CurQuant.PQIndex + 1;
                }
                else
                {
                    m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                    if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                    {
                            if(m_CurQuant.PQIndex > 9)
                            {
                                    if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                                        m_Quant.PQuant++;
                            }
                            else
                            {
                                if(!m_CurQuant.HalfPQ)
                                    HalfQP = 1;
                                else
                                {
                                    HalfQP = 0;
                                    if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                                        m_Quant.PQuant++;
                                }
                            }
                    }
                }
            }
            else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
            {
                m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                m_Quant.LimPQuant--;

                if(m_CurQuant.PQIndex > 9)
                {
                        if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                            m_Quant.PQuant--;
                }
                else
                {
                    if(m_CurQuant.HalfPQ)
                        HalfQP = 0;
                    else
                    {
                        if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                            m_Quant.PQuant--;
                    }
                }
            }
            else if(Sts & VC1_BRC_SMALL_FRAME)
            {
                m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                m_Quant.LimPQuant--;

                if(m_CurQuant.HalfPQ)
                    HalfQP = 0;
                else
                {
                    if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                    {
                        m_Quant.PQuant--;
                        if((m_CurQuant.PQIndex != VC1_MIN_QUANT) && (ratio > VC1_MIN_RATIO_HALF))
                                HalfQP = 1;
                    }
                }
            }

            VC1_CHECK_QUANT(m_Quant.LimPQuant);
            VC1_QUANT_CLIP(m_Quant.PQuant, m_Quant.LimPQuant);

            m_Quant.PHalf = HalfQP;
            }
            break;
    }
 };

void VC1BRC_IP::CorrectGOPQuant(ePType picType, Ipp32f ratio)
 {
     Ipp8u curQuant = 0;

     if(ratio == 1)
        return;

     switch (picType)
     {
         case(VC1_ENC_I_FRAME):
         case (VC1_ENC_I_I_FIELD):
             {
                 //new quant calculation
                 curQuant = (Ipp8u)(ratio * m_Quant.IQuant + 0.5);
                 VC1_CHECK_QUANT(curQuant);

                 if(curQuant != m_Quant.IQuant)
                    m_Quant.IHalf = 0;

                 m_Quant.IQuant = (m_Quant.IQuant + curQuant + 1)/2;
                 //m_Quant.LimIQuant = (m_Quant.LimIQuant + m_Quant.IQuant - 1)/2;

                 //VC1_QUANT_CLIP(m_Quant.LimIQuant,0);
                 VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);
             }
             break;
         case(VC1_ENC_P_FRAME):
         case(VC1_ENC_P_FRAME_MIXED):
         case(VC1_ENC_I_P_FIELD):
         case(VC1_ENC_P_I_FIELD):
         case(VC1_ENC_P_P_FIELD):
            {
                 //new quant calculation
                 curQuant = (Ipp8u)(ratio * m_Quant.PQuant - 0.5);
                 VC1_CHECK_QUANT(curQuant);

                 if(curQuant != m_Quant.PQuant)
                    m_Quant.PHalf = 0;

                 m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                 m_Quant.LimPQuant = (m_Quant.LimPQuant + m_Quant.PQuant - 1)/2;

                 VC1_CHECK_QUANT(m_Quant.LimPQuant);
                 VC1_QUANT_CLIP(m_Quant.PQuant, m_Quant.LimPQuant);
             }
             break;
     }
 };

void VC1BRC_IP::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half)
{
    m_picType = picType;

    switch (picType)
    {
    case(VC1_ENC_I_FRAME):
    case(VC1_ENC_I_I_FIELD):
        m_CurQuant.PQIndex = m_Quant.IQuant;
        m_CurQuant.HalfPQ  = m_Quant.IHalf;
        break;

    case(VC1_ENC_P_FRAME):
    case(VC1_ENC_P_FRAME_MIXED):
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_CurQuant.PQIndex = m_Quant.PQuant + m_failPQuant;
        m_CurQuant.HalfPQ  = m_Quant.PHalf;

        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.PQuant--;
                    m_Quant.PHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.PHalf = 0;
                }
            }

           VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimPQuant);

        break;
        default:
            break;
    }

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;
}

void VC1BRC_IP::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool *Uniform)
{
    m_picType = picType;

    switch (picType)
    {
    case(VC1_ENC_I_FRAME):
    case(VC1_ENC_I_I_FIELD):
        m_CurQuant.PQIndex = m_Quant.IQuant;
        m_CurQuant.HalfPQ  = m_Quant.IHalf;
        break;

    case(VC1_ENC_P_FRAME):
    case(VC1_ENC_P_FRAME_MIXED):
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_CurQuant.PQIndex = m_Quant.PQuant + m_failPQuant;
        m_CurQuant.HalfPQ  = m_Quant.PHalf;

        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.PQuant--;
                    m_Quant.PHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.PHalf = 0;
                }
            }

           VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimPQuant);

        break;
        default:
            break;
    }

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;

    switch (m_QuantMode)
    {
        case 0:
            VM_ASSERT(0);
            *Uniform = true;
            break;
        case 1:
            *Uniform = true;
            break;
        case 2:
            *Uniform = false;
            break;
        case 3:
            if(m_CurQuant.PQIndex > 8)
                *Uniform = false;
            else
                *Uniform =  true;
            break;
        default:
            VM_ASSERT(0);
            *Uniform = true;
            break;
    }
}

UMC::Status VC1BRC_IP::SetGOPParams(Ipp32u GOPLength, Ipp32u /*BFrLength*/)
{
    if(GOPLength == 0)
        return UMC::UMC_ERR_INVALID_PARAMS;

    m_GOPLength = GOPLength;

    m_INum = 1;
    m_PNum = GOPLength - 1;
    m_GOPSize        = m_IdealFrameSize * m_GOPLength;
    m_nextGOPSize    = m_GOPSize;

    m_needISize = (Ipp32s)(m_GOPSize/(1 + m_IP_size*m_PNum));
    m_needPSize = (Ipp32s)(m_IP_size*m_needISize);

    m_currISize = m_needISize;
    m_currPSize = m_needPSize;

    m_currFrameInGOP = 0;
    m_currGOPSize = 0;
    m_GOPHalfFlag = 1;
    m_I_GOPSize = 0;
    m_P_GOPSize = 0;
    m_CurrINum = 0;
    m_CurrPNum = 0;
    m_failGOP = 0;
    m_AveragePQuant = 0;
    m_AverageIQuant = 0;
    m_poorRefFrame = 0;

    return UMC::UMC_OK;
}
////////////////////////////////////////////
//          VC1BRC_IPB
////////////////////////////////////////////
VC1BRC_IPB::VC1BRC_IPB()
{
    m_I_GOPSize = 0;
    m_P_GOPSize = 0;
    m_B_GOPSize = 0;
    m_needISize = 0;
    m_needPSize = 0;
    m_needBSize = 0;
    m_INum = 0;
    m_PNum = 0;
    m_BNum = 0;
    m_CurrINum = 0;
    m_CurrPNum = 0;
    m_CurrBNum = 0;
    m_currFrameInGOP = 0;
    m_frameCount = 0;
    m_poorRefFrame = 0;
    m_GOPHalfFlag = 0;
    m_currISize = 0;
    m_currPSize = 0;
    m_currBSize = 0;
    m_GOPSize = 0;
    m_currGOPSize = 0;
    m_nextGOPSize = 0;
    m_IP_size = (Ipp32f)(VC1_P_SIZE);
    m_IB_size = (Ipp32f)(VC1_B_SIZE);
    m_failPQuant = 0;
    m_failBQuant = 0;
    m_failGOP = 0;
    m_AveragePQuant = 0;
    m_AverageIQuant = 0;
    m_AverageBQuant = 0;
};

VC1BRC_IPB::~VC1BRC_IPB()
{
};

UMC::Status VC1BRC_IPB::Init(Ipp32u yuvFSize,  Ipp32u bitrate,  Ipp64f framerate,
                           Ipp32u mode,        Ipp32u GOPLength,Ipp32u BFrLength,
                           Ipp8u doubleQuant, Ipp8u QuantMode)
 {
    UMC::Status VC1Sts = UMC::UMC_OK;

    m_bitrate   = bitrate;
    m_framerate = framerate;

    m_IdealFrameSize = (Ipp32u)(bitrate/framerate/8);
    if(m_IdealFrameSize == 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    VC1Sts = SetGOPParams(GOPLength, BFrLength);
    m_encMode   = mode;
    m_QuantMode = QuantMode;
    VM_ASSERT(QuantMode != 0);

    if(doubleQuant)
    {
        if(doubleQuant > (VC1_MAX_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        if(doubleQuant < (VC1_MIN_QUANT*2))
            return UMC::UMC_ERR_ALLOC;

        m_encMode = VC1_BRC_CONST_QUANT_MODE;
    }

    if(m_encMode == VC1_BRC_HIGHT_QUALITY_MODE)
    {
        Ipp32u AverageCompression = ((yuvFSize)/m_needISize)/3;

        VC1_CHECK_QUANT(AverageCompression);
        m_CurQuant.PQIndex = m_Quant.IQuant = (Ipp8u)AverageCompression;

        AverageCompression = ((yuvFSize)/m_needPSize)/5;
        VC1_CHECK_QUANT(AverageCompression);

        m_Quant.PQuant =  (Ipp8u)AverageCompression;

        if(m_Quant.PQuant > 2)
            m_Quant.PQuant -= 2;

        AverageCompression = ((yuvFSize)/m_needBSize)/6;
        VC1_CHECK_QUANT(AverageCompression);

        m_Quant.BQuant = (Ipp8u)AverageCompression;

        if(m_Quant.BQuant > 2)
            m_Quant.BQuant -= 2;
    }
    else
    {
        m_CurQuant.PQIndex = m_Quant.IQuant = m_Quant.PQuant = m_Quant.BQuant = (doubleQuant>>1);
        m_CurQuant.HalfPQ  = m_Quant.IHalf = m_Quant.PHalf = m_Quant.BHalf = doubleQuant&0x1;
    }

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
    case VC1_BRC_CONST_QUANT_MODE:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    default:
        m_ratio_min       = (Ipp32f)(0.6);
        m_buffer_overflow = (Ipp32f)(1.1);
        break;
    }

    return VC1Sts;
 };

void VC1BRC_IPB::Close()
{
    Reset();
}

void VC1BRC_IPB::Reset()
{
    m_CurrINum = m_CurrPNum = m_CurrBNum = 0;

    m_SizeAbberation = 0;

    //Frames parameters
    m_currSize = 0;

    //Flags
    m_recoding     = 0;
    m_poorRefFrame = 0;
    m_GOPHalfFlag  = 1;
    m_failPQuant   = 0;
    m_failBQuant   = 0;
    m_failGOP      = 0;

    //Picture params
    m_picType = VC1_ENC_I_FRAME;

    m_GOPSize      = m_IdealFrameSize * m_GOPLength;
    m_nextGOPSize    = m_GOPSize;

    m_needISize = (Ipp32s)(m_GOPSize/(1 + m_IP_size*m_PNum + m_IB_size*m_BNum));
    m_needPSize = (Ipp32s)(m_IP_size*m_needISize);
    m_needBSize = (Ipp32s)(m_IB_size*m_needISize);

    m_currISize = m_needISize;
    m_currPSize = m_needPSize;
    m_currBSize = m_needBSize;
    m_frameCount = 0;
};

UMC::Status VC1BRC_IPB::CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts)
{
    UMC::Status UMCSts = UMC::UMC_OK;
    Ipp32f ratio = 0;

    if(m_recoding)
        m_poorRefFrame &= 0x2;

    if(currSize == 0)
        return UMC::UMC_ERR_INVALID_PARAMS;

    m_currSize = currSize;

    if(picType == VC1_ENC_SKIP_FRAME)
    {
#ifdef VC1_GOP_DEBUG
     ratio = ((Ipp32f)currSize/(Ipp32f)m_needISize);

     printf("Skip ");
     printf("%d  %d % d\n", m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif
        return UMCSts;
    }

    switch (picType)
    {
        case(VC1_ENC_I_FRAME):
        case(VC1_ENC_I_I_FIELD):
#ifdef VC1_GOP_DEBUG
     printf("I ");
#endif
            m_currISize = currSize;
            ratio = ((Ipp32f)m_currISize/(Ipp32f)m_needISize);
            if(ratio < VC1_POOR_REF_FRAME)
                m_poorRefFrame = ((m_poorRefFrame<<1) + 1)&0x3;
            else
                m_poorRefFrame = (m_poorRefFrame<<1)&0x3;
            break;
        case(VC1_ENC_P_FRAME):
        case(VC1_ENC_P_FRAME_MIXED):
        case(VC1_ENC_I_P_FIELD):
        case(VC1_ENC_P_I_FIELD):
        case(VC1_ENC_P_P_FIELD):
#ifdef VC1_GOP_DEBUG
     printf("P ");
#endif
            m_currPSize = currSize;
            ratio = ((Ipp32f)m_currPSize/(Ipp32f)m_needPSize);
            if(ratio < VC1_POOR_REF_FRAME)
                m_poorRefFrame = ((m_poorRefFrame<<1) + 1)&0x3;
            else
                m_poorRefFrame = (m_poorRefFrame<<1)&0x3;

            break;
        case(VC1_ENC_B_FRAME):
        case(VC1_ENC_BI_B_FIELD):
        case(VC1_ENC_B_BI_FIELD):
        case(VC1_ENC_B_B_FIELD):

#ifdef VC1_GOP_DEBUG
     printf("B ");
#endif
            m_currBSize = currSize;
            ratio = ((Ipp32f)m_currBSize/(Ipp32f)m_needBSize);
            break;
    }

#ifdef VC1_BRC_DEBUG
     printf("current ratio = %f\n", ratio);
#endif
#ifdef VC1_GOP_DEBUG
     printf("current ratio = %f   %d  %d %d\n", ratio, m_CurQuant.PQIndex, m_CurQuant.HalfPQ, currSize);
#endif

    if(m_failGOP)
        HRDSts = UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    switch(m_encMode)
    {
    case VC1_BRC_HIGHT_QUALITY_MODE:
        CheckFrame_QualityMode(picType, ratio, HRDSts);
        break;
    case VC1_BRC_CONST_QUANT_MODE:
        break;
    default:
        break;
    }
    m_recoding++;

#ifdef VC1_BRC_DEBUG
    printf("coded size %d\n", currSize);
#endif

    return UMCSts;
};

void VC1BRC_IPB::CompleteFrame(ePType picType)
{
    Ipp32f ratioI = 0;
    Ipp32f ratioP = 0;
    Ipp32f ratioB = 0;
    Ipp32s prefINeedSize = m_needISize;
    Ipp32s prefPNeedSize = m_needPSize;
    Ipp32s prefBNeedSize = m_needBSize;

#ifdef BRC_TEST
     if(BRCFileSizeTest.RES_File)
        fprintf(BRCFileSizeTest.RES_File, "%d,",m_currSize);
#endif

    if(m_encMode == VC1_BRC_CONST_QUANT_MODE)
        return;

    m_currFrameInGOP++;
    m_frameCount++;
    m_currGOPSize += m_currSize;

    switch (picType)
    {
    case VC1_ENC_I_FRAME:
    case VC1_ENC_I_I_FIELD:
        m_I_GOPSize += m_currSize;
        m_CurrINum++;
        m_AverageIQuant = m_CurQuant.PQIndex;
        break;
    case VC1_ENC_P_FRAME:
    case VC1_ENC_P_FRAME_MIXED:
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_P_GOPSize += m_currSize;
        m_CurrPNum++;
        m_AveragePQuant +=  (m_CurQuant.PQIndex*2 + m_CurQuant.HalfPQ);
        break;
    case VC1_ENC_B_FRAME:
    case(VC1_ENC_BI_B_FIELD):
    case(VC1_ENC_B_BI_FIELD):
    case(VC1_ENC_B_B_FIELD):
        m_B_GOPSize += m_currSize;
        m_CurrBNum++;
        m_AverageBQuant +=  (m_CurQuant.PQIndex*2 + m_CurQuant.HalfPQ);
        break;
    case VC1_ENC_SKIP_FRAME:
        if(m_CurrINum)
        {
            if(m_currFrameInGOP % (m_BFrLength + 1))
                m_PNum--;
            else
                 m_BNum--;
        }
        break;
    default:
        return;
        break;
    }

    if((m_GOPHalfFlag && ((m_currGOPSize > m_GOPSize/2) || m_currFrameInGOP == m_CurrGOPLength/2))
        || (m_currFrameInGOP == 1) || m_failPQuant || m_failBQuant || picType == VC1_ENC_SKIP_FRAME)
    {

        Ipp32s PBSize = m_nextGOPSize - m_currGOPSize;

        if(m_currFrameInGOP != 1)
        {
            m_GOPHalfFlag = 0;
        }

        if((PBSize > m_IdealFrameSize)  && (m_currFrameInGOP != m_CurrGOPLength))
        {

            m_needPSize = (Ipp32s)(PBSize/((m_PNum - m_CurrPNum)
                + (m_IB_size/m_IP_size)*(m_BNum - m_CurrBNum)));

            m_needBSize = (Ipp32s)(m_needPSize*(m_IB_size/m_IP_size));
        }
        else
        {
            m_failGOP = 1;
            if(m_needPSize > m_IdealFrameSize/2)
                m_needPSize = (Ipp32s)(m_needPSize*0.75);
            if(m_needBSize > m_IdealFrameSize/2)
                m_needBSize = (Ipp32s)(m_needBSize*0.75);
        }


        ratioP = ((Ipp32f)prefPNeedSize/(Ipp32f)m_needPSize);
        ratioB = ((Ipp32f)prefBNeedSize/(Ipp32f)m_needBSize);

        if(m_needPSize < (m_IdealFrameSize>>2))
            ratioP = 1;
        if(m_needBSize < (m_IdealFrameSize>>2))
            ratioB = 1;

        CorrectGOPQuant(VC1_ENC_P_FRAME, ratioP);
        CorrectGOPQuant(VC1_ENC_B_FRAME, ratioB);
    }


    if(m_currFrameInGOP == m_CurrGOPLength && m_CurrPNum && m_CurrBNum)
    {
     m_SizeAbberation += m_GOPSize - m_currGOPSize;

    //check overflow m_SizeAbberation
    if(m_SizeAbberation > 4*m_GOPSize)
        m_SizeAbberation = 4*m_GOPSize;

    if(m_SizeAbberation < (- 4*m_GOPSize))
        m_SizeAbberation = -(4*m_GOPSize);

///---------------------------
        m_AveragePQuant /= (m_CurrPNum*2);
        m_AveragePQuant++;

        m_AverageBQuant /= (m_CurrBNum*2);
        m_AverageBQuant++;
///---------------------------
#ifdef VC1_GOP_DEBUG
    printf("\nGOP ratio = %f\n\n", ((Ipp32f)m_currGOPSize/(Ipp32f)m_nextGOPSize));
    printf("\nIdeal     = %f\n", ((Ipp32f)m_currGOPSize/(Ipp32f)m_GOPSize));

    printf("\n\n IQuant = %d    PQuant = %d    BQuant = %d\n\n",
        m_AverageIQuant, m_AveragePQuant, m_AverageBQuant);

    printf("-----------------------------------NEW GOP");
    printf("------------------------------------------\n\n\n");
#endif
        //correction of I/P/B ratio
        if(m_I_GOPSize && m_P_GOPSize && m_B_GOPSize)
        {
            Ipp32f P_average_size =  (Ipp32f)(m_P_GOPSize/m_PNum);
            Ipp32f PRatio = (P_average_size*m_AveragePQuant)/(m_I_GOPSize*m_AverageIQuant);
            m_IP_size = (m_IP_size + PRatio)/2;

            if(m_IP_size > 1)
                m_IP_size = 1;


            Ipp32f B_average_size =  (Ipp32f)(m_B_GOPSize/m_BNum);
            Ipp32f BIRatio = (B_average_size*m_AverageBQuant)/(m_I_GOPSize*m_AverageIQuant);
            Ipp32f BPRatio = (B_average_size*m_AverageBQuant)/(m_P_GOPSize*m_AveragePQuant);
            m_IB_size = (m_IB_size + BIRatio + BPRatio)/3;

            if(m_IB_size > 1)
                m_IB_size = 1;

#ifdef VC1_GOP_DEBUG
        printf("\n\n I:P = %f\n", m_IP_size);
        printf(" I:B = %f\n\n", m_IB_size);
#endif
        }

    SetGOPParams(m_GOPLength, m_BFrLength);
    m_nextGOPSize = m_GOPSize + m_SizeAbberation;

    //check m_needISize
    if(m_nextGOPSize > m_GOPSize + (m_GOPSize>>1))
       m_nextGOPSize = m_GOPSize + (m_GOPSize>>1);

    if(m_nextGOPSize < (m_GOPSize>>1))
        m_nextGOPSize = (m_GOPSize>>1);


#ifdef VC1_GOP_DEBUG
            printf("nextGOPSize = %d\n", m_nextGOPSize);
            printf("m_SizeAbberation = %d\n", m_SizeAbberation);
#endif

        m_needISize = (Ipp32s)(m_nextGOPSize/(1 + m_IP_size*m_PNum + m_IB_size*m_BNum));

        ratioI = ((Ipp32f)prefINeedSize/(Ipp32f)m_needISize);

        CorrectGOPQuant(VC1_ENC_I_FRAME, ratioI);
    }

    m_recoding = 0;
    m_failPQuant = 0;
    m_failBQuant = 0;

#ifdef VC1_BRC_DEBUG
    printf("\n\n\n");
#endif
};


void VC1BRC_IPB::CheckFrame_QualityMode(ePType picType, Ipp32f ratio, UMC::Status UMCSts)
{
    Ipp8u curQuant = 0;
    Ipp32s Sts = VC1_BRC_OK;
    bool HalfQP = 0;

    //new quant calculation
    curQuant = (Ipp8u)(ratio * m_CurQuant.PQIndex + 0.5);
    VC1_CHECK_QUANT(curQuant);

    //check compression
    if((ratio <= m_buffer_overflow) && (ratio >= VC1_GOOD_COMPRESSION))
        Sts = VC1_BRC_OK;       //good frame
    else if(ratio > m_buffer_overflow)
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER; //could be critical situation, if HRD buffer small
    else  if(ratio <= m_ratio_min)
    {
        //very small frame
        Sts |= VC1_BRC_ERR_SMALL_FRAME;

        if(m_CurQuant.PQIndex > 1)
            HalfQP = 1;
    }
    else if(ratio >= VC1_MIN_RATIO_HALF)
    {
        if((m_CurQuant.PQIndex > 1) && (curQuant > m_CurQuant.PQIndex))
            HalfQP = 1;
        Sts |= VC1_BRC_SMALL_FRAME;
    }
    else
         Sts |= VC1_BRC_SMALL_FRAME;

    //check HRD status
    if(UMCSts ==  UMC::UMC_ERR_NOT_ENOUGH_BUFFER)
    {
        Sts |= VC1_BRC_NOT_ENOUGH_BUFFER;
        HalfQP = 0;
    }

    if(Sts != VC1_BRC_OK)
    switch (picType)
    {
        case(VC1_ENC_I_FRAME):
        case(VC1_ENC_I_I_FIELD):
            {
#ifdef VC1_BRC_DEBUG
            printf("m_LimIQuant %d\n", m_Quant.LimIQuant);
#endif
            m_Quant.IQuant = (m_Quant.IQuant + curQuant)/2;

            if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
            {
                if(ratio > VC1_RATIO_ILIM)
                {
                   m_Quant.IQuant++;
                   m_Quant.LimIQuant = m_CurQuant.PQIndex + 2;
                }
                else
                {
                    if(m_CurQuant.PQIndex > 9)
                        m_Quant.LimIQuant = m_CurQuant.PQIndex + 1;
                    else if(m_Quant.IQuant <= m_CurQuant.PQIndex)
                        HalfQP = 1;
                }
            }
            else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
            {
                m_Quant.LimIQuant--;

                if(m_Quant.IQuant >= m_CurQuant.PQIndex)
                    m_Quant.IQuant--;

                if((ratio > VC1_MIN_RATIO_HALF) &&  (m_CurQuant.PQIndex > 1))
                    HalfQP = 1;

            }
            else if(Sts & VC1_BRC_SMALL_FRAME)
            {
                m_Quant.LimIQuant--;

               if((m_Quant.IQuant < m_CurQuant.PQIndex) && m_Quant.IQuant &&  (m_CurQuant.PQIndex > 1))
                    HalfQP = 1;

            }

            VC1_CHECK_QUANT(m_Quant.LimIQuant);
            VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);

            m_Quant.IHalf = HalfQP;
            }
            break;
        case(VC1_ENC_P_FRAME):
        case(VC1_ENC_P_FRAME_MIXED):
        case(VC1_ENC_I_P_FIELD):
        case(VC1_ENC_P_I_FIELD):
        case(VC1_ENC_P_P_FIELD):
            {
#ifdef VC1_BRC_DEBUG
            printf("m_LimPQuant %d\n", m_Quant.LimPQuant);
#endif

            if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
            {
                if(ratio > VC1_RATIO_ILIM)
                {
                    m_failPQuant   = (m_Quant.PQuant + curQuant + 1)/2 - m_Quant.PQuant;

                    m_Quant.PQuant++;
                    m_Quant.LimPQuant = m_CurQuant.PQIndex + 1;
                }
                else
                {
                    m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                    if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                    {
                        if(m_CurQuant.PQIndex > 9)
                        {
                            if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                                m_Quant.PQuant++;
                        }
                        else
                        {
                            if(!m_CurQuant.HalfPQ)
                                HalfQP = 1;
                            else
                            {
                                HalfQP = 0;
                                if(m_Quant.PQuant <= m_CurQuant.PQIndex)
                                    m_Quant.PQuant++;
                            }
                        }
                    }
                }
            }
            else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
            {
                m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                m_Quant.LimPQuant--;

                if(m_CurQuant.PQIndex > 9)
                {
                        if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                            m_Quant.PQuant--;
                }
                else
                {
                    if(m_CurQuant.HalfPQ)
                        HalfQP = 0;
                    else
                    {
                        if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                            m_Quant.PQuant--;
                    }
                }
            }
            else if(Sts & VC1_BRC_SMALL_FRAME)
            {
                m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;

                m_Quant.LimPQuant--;

                if(m_CurQuant.HalfPQ)
                    HalfQP = 0;
                else
                {
                    if(m_Quant.PQuant >= m_CurQuant.PQIndex)
                    {
                        m_Quant.PQuant--;
                        if((m_CurQuant.PQIndex != VC1_MIN_QUANT) && (ratio > VC1_MIN_RATIO_HALF))
                                HalfQP = 1;
                    }
                }
            }

            VC1_CHECK_QUANT(m_Quant.LimPQuant);
            VC1_QUANT_CLIP(m_Quant.PQuant, m_Quant.LimPQuant);

            m_Quant.PHalf = HalfQP;
            }
            break;
        case(VC1_ENC_B_FRAME):
        case(VC1_ENC_BI_B_FIELD):
        case(VC1_ENC_B_BI_FIELD):
        case(VC1_ENC_B_B_FIELD):
            {
#ifdef VC1_BRC_DEBUG
            printf("m_LimBQuant %d\n", m_Quant.LimBQuant);
#endif

            if(Sts & VC1_BRC_NOT_ENOUGH_BUFFER)
            {
                if(ratio > VC1_RATIO_ILIM)
                {
                   m_failBQuant   = (m_Quant.BQuant + curQuant + 1)/2 - m_Quant.BQuant;
                   m_Quant.BQuant++;
                   m_Quant.LimBQuant = m_CurQuant.PQIndex + 1;
                }
                else
                {
                    m_Quant.BQuant = (m_Quant.BQuant + curQuant + 1)/2;

                    if(m_Quant.BQuant <= m_CurQuant.PQIndex)
                    {
                        if(m_CurQuant.PQIndex > 9)
                        {
                                if(m_Quant.BQuant <= m_CurQuant.PQIndex)
                                    m_Quant.BQuant++;
                        }
                        else
                        {
                            if(!m_CurQuant.HalfPQ)
                                HalfQP = 1;
                            else
                            {
                                HalfQP = 0;
                                if(m_Quant.BQuant <= m_CurQuant.PQIndex)
                                    m_Quant.BQuant++;
                            }
                        }
                    }
                }
            }
            else if(Sts & VC1_BRC_ERR_SMALL_FRAME)
            {
                m_Quant.BQuant = (m_Quant.BQuant + curQuant + 1)/2;

                m_Quant.LimBQuant--;

                    if(m_CurQuant.PQIndex > 9)
                    {
                            if(m_Quant.BQuant >= m_CurQuant.PQIndex)
                                m_Quant.BQuant--;
                    }
                    else
                    {
                        if(m_CurQuant.HalfPQ)
                            HalfQP = 0;
                        else
                        {
                            if(m_Quant.BQuant >= m_CurQuant.PQIndex)
                                m_Quant.BQuant--;
                        }
                    }
            }
            else if(Sts & VC1_BRC_SMALL_FRAME)
            {
               m_Quant.BQuant = (m_Quant.BQuant + curQuant + 1)/2;

               m_Quant.LimBQuant--;

                if(m_CurQuant.HalfPQ)
                    HalfQP = 0;
                else
                {
                    if(m_Quant.BQuant >= m_CurQuant.PQIndex)
                    {
                        m_Quant.BQuant--;
                        if((m_CurQuant.PQIndex != VC1_MIN_QUANT) && (ratio > VC1_MIN_RATIO_HALF))
                                HalfQP = 1;
                    }
                }
            }

            VC1_CHECK_QUANT(m_Quant.LimBQuant);
            VC1_QUANT_CLIP(m_Quant.BQuant, m_Quant.LimBQuant);

            m_Quant.BHalf = HalfQP;
            }
            break;
    }
 };

 void VC1BRC_IPB::CorrectGOPQuant(ePType picType, Ipp32f ratio)
 {
     Ipp8u curQuant = 0;

     if(ratio == 1)
        return;

     switch (picType)
     {
         case(VC1_ENC_I_FRAME):
         case(VC1_ENC_I_I_FIELD):
             {
                 //new quant calculation
                 curQuant = (Ipp8u)(ratio * m_Quant.IQuant + 0.5);
                 VC1_CHECK_QUANT(curQuant);

                 if(curQuant != m_Quant.IQuant)
                    m_Quant.IHalf = 0;

                 m_Quant.IQuant = (m_Quant.IQuant + curQuant + 1)/2;

                // m_Quant.LimIQuant = (m_Quant.LimIQuant + m_Quant.IQuant)/2;

                 //VC1_QUANT_CLIP(m_Quant.LimIQuant,0);
                 VC1_QUANT_CLIP(m_Quant.IQuant, m_Quant.LimIQuant);
             }
             break;
         case(VC1_ENC_P_FRAME):
         case(VC1_ENC_P_FRAME_MIXED):
        case(VC1_ENC_I_P_FIELD):
        case(VC1_ENC_P_I_FIELD):
        case(VC1_ENC_P_P_FIELD):
             {
                 //new quant calculation
                 curQuant = (Ipp8u)(ratio * m_Quant.PQuant - 0.5);
                 VC1_CHECK_QUANT(curQuant);

                 if(curQuant != m_Quant.PQuant)
                    m_Quant.PHalf = 0;

                 m_Quant.PQuant = (m_Quant.PQuant + curQuant + 1)/2;
                 m_Quant.LimPQuant = (m_Quant.LimPQuant + m_Quant.PQuant)/2;

                 VC1_CHECK_QUANT(m_Quant.LimPQuant);
                 VC1_QUANT_CLIP(m_Quant.PQuant, m_Quant.LimPQuant);
             }
             break;
         case(VC1_ENC_B_FRAME):
        case(VC1_ENC_BI_B_FIELD):
        case(VC1_ENC_B_BI_FIELD):
        case(VC1_ENC_B_B_FIELD):
             {
                 //new quant calculation
                 curQuant = (Ipp8u)(ratio * m_Quant.BQuant - 0.5);
                 VC1_CHECK_QUANT(curQuant);

                 if(curQuant != m_Quant.BQuant)
                    m_Quant.BHalf = 0;

                 m_Quant.BQuant = (m_Quant.BQuant + curQuant + 1)/2;
                 m_Quant.LimBQuant = (m_Quant.LimBQuant + m_Quant.BQuant)/2;

                 VC1_CHECK_QUANT(m_Quant.LimBQuant);
                 VC1_QUANT_CLIP(m_Quant.BQuant, m_Quant.LimBQuant);
             }
             break;
     }
 };

void VC1BRC_IPB::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half)
{
    m_picType = picType;

    switch (picType)
    {
    case(VC1_ENC_I_FRAME):
    case(VC1_ENC_I_I_FIELD):
        m_CurQuant.PQIndex = m_Quant.IQuant;
        m_CurQuant.HalfPQ  = m_Quant.IHalf;
        break;
    case(VC1_ENC_P_FRAME):
    case(VC1_ENC_P_FRAME_MIXED):
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_CurQuant.PQIndex = m_Quant.PQuant + m_failPQuant;
        m_CurQuant.HalfPQ  = m_Quant.PHalf;

        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame&0x1)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.PQuant--;
                    m_Quant.PHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.PHalf = 0;
                }

            }

        VC1_CHECK_QUANT(m_CurQuant.PQIndex);
        VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimPQuant);
        break;

    case(VC1_ENC_B_FRAME):
    case(VC1_ENC_BI_B_FIELD):
    case(VC1_ENC_B_BI_FIELD):
    case(VC1_ENC_B_B_FIELD):
        m_CurQuant.PQIndex = m_Quant.BQuant  + m_failBQuant;
        m_CurQuant.HalfPQ  = m_Quant.BHalf;
        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.BQuant--;
                    m_Quant.BHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.BHalf = 0;
                }

            }

        VC1_CHECK_QUANT(m_CurQuant.PQIndex);
        VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimBQuant);

        break;
        default:
            break;
    }

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;
}

void VC1BRC_IPB::GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform)
{
    m_picType = picType;

    switch (picType)
    {
    case(VC1_ENC_I_FRAME):
    case(VC1_ENC_I_I_FIELD):
        m_CurQuant.PQIndex = m_Quant.IQuant;
        m_CurQuant.HalfPQ  = m_Quant.IHalf;
        break;
    case(VC1_ENC_P_FRAME):
    case(VC1_ENC_P_FRAME_MIXED):
    case(VC1_ENC_I_P_FIELD):
    case(VC1_ENC_P_I_FIELD):
    case(VC1_ENC_P_P_FIELD):
        m_CurQuant.PQIndex = m_Quant.PQuant + m_failPQuant;
        m_CurQuant.HalfPQ  = m_Quant.PHalf;

        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame&0x1)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.PQuant--;
                    m_Quant.PHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.PHalf = 0;
                }

            }

        VC1_CHECK_QUANT(m_CurQuant.PQIndex);
        VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimPQuant);
        break;

    case(VC1_ENC_B_FRAME):
    case(VC1_ENC_BI_B_FIELD):
    case(VC1_ENC_B_BI_FIELD):
    case(VC1_ENC_B_B_FIELD):
        m_CurQuant.PQIndex = m_Quant.BQuant  + m_failBQuant;
        m_CurQuant.HalfPQ  = m_Quant.BHalf;
        if(m_encMode != VC1_BRC_CONST_QUANT_MODE)
            if(m_poorRefFrame)
            {//correct quant for next B frame
                if(m_CurQuant.PQIndex > 9)
                {
                    m_CurQuant.PQIndex--;
                    m_CurQuant.HalfPQ = 1;
                    m_Quant.BQuant--;
                    m_Quant.BHalf = 0;
                }
                else
                {
                    m_CurQuant.HalfPQ = 0;
                    m_Quant.BHalf = 0;
                }

            }

        VC1_CHECK_QUANT(m_CurQuant.PQIndex);
        VC1_QUANT_CLIP(m_CurQuant.PQIndex, m_Quant.LimBQuant);

        break;
        default:
            break;
    }

    if(m_CurQuant.PQIndex > 8)
        m_CurQuant.HalfPQ  = 0;

#ifdef VC1_BRC_DEBUG
    printf("cur quant %d\n", m_CurQuant.PQIndex);
    printf("half quant %d\n",  m_CurQuant.HalfPQ);
#endif

    *PQuant = m_CurQuant.PQIndex;
    *Half   = m_CurQuant.HalfPQ;

    switch (m_QuantMode)
    {
        case 0:
            VM_ASSERT(0);
            *Uniform = true;
            break;
        case 1:
            *Uniform = true;
            break;
        case 2:
            *Uniform = false;
            break;
        case 3:
            if(m_CurQuant.PQIndex > 8)
                *Uniform = false;
            else
                *Uniform =  true;
            break;
        default:
            VM_ASSERT(0);
            *Uniform = true;
            break;
    }
}

UMC::Status VC1BRC_IPB::SetGOPParams(Ipp32u GOPLength, Ipp32u BFrLength)
{
    if(GOPLength == 0)
        return UMC::UMC_ERR_INVALID_PARAMS;

    if(m_GOPLength != GOPLength || m_BFrLength != BFrLength)
    {
        m_GOPLength  = GOPLength;
        m_BFrLength  = BFrLength;
        m_frameCount = 0;
    }

    m_INum = 1;
    m_PNum = (GOPLength - 1)/(BFrLength + 1);

    if(m_frameCount)
       m_BNum = GOPLength - m_PNum - 1;
    else
    {
        m_BNum = m_PNum*BFrLength;
        VM_ASSERT(m_INum + m_PNum + m_BNum <= (Ipp32s)GOPLength);
    }

    m_CurrGOPLength = m_INum + m_PNum + m_BNum;

    m_GOPSize     = m_IdealFrameSize * m_CurrGOPLength;
    m_nextGOPSize = m_GOPSize;

    m_needISize = (Ipp32s)(m_GOPSize/(1 + m_IP_size*m_PNum + m_IB_size*m_BNum));
    m_needPSize = (Ipp32s)(m_IP_size*m_needISize);
    m_needBSize = (Ipp32s)(m_IB_size*m_needISize);

    m_currISize = m_needISize;
    m_currPSize = m_needPSize;
    m_currBSize = m_needBSize;

    m_currFrameInGOP = 0;
    m_poorRefFrame = 0;
    m_currGOPSize = 0;
    m_GOPHalfFlag = 1;

    m_I_GOPSize = 0;
    m_P_GOPSize = 0;
    m_B_GOPSize = 0;

    m_CurrINum = 0;
    m_CurrPNum = 0;
    m_CurrBNum = 0;
    m_failGOP = 0;

    m_AveragePQuant = 0;
    m_AverageBQuant = 0;

    return UMC::UMC_OK;
}

}
#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
