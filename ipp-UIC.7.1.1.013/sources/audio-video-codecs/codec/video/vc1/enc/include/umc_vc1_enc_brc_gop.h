/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, bit rate control I frames, IP frames, IPB frames
//
*/

#ifndef _ENCODER_VC1_BITRATE_GOP_H_
#define _ENCODER_VC1_BITRATE_GOP_H_

#include "umc_structures.h"
#include "umc_vc1_enc_def.h"

namespace UMC_VC1_ENCODER
{
#ifdef VC1_ENC_DEBUG_ON
    //#define VC1_BRC_DEBUG
    //#define VC1_GOP_DEBUG
#endif

//bitrate mode
enum
{
    //could be a lot of recodingfor the best
    //possible for current bitrate quality
    VC1_BRC_HIGHT_QUALITY_MODE = 0x0,

    //quant coeefs will not changed
    VC1_BRC_CONST_QUANT_MODE = 0x1,
};

#define VC1_RATIO_ILIM     1.9
#define VC1_RATIO_PLIM     1.5
#define VC1_RATIO_BLIM     1.45

#define VC1_MIN_RATIO_HALF     0.8

#define VC1_RATIO_IMIN     0.4
#define VC1_RATIO_PMIN     0.4
#define VC1_RATIO_BMIN     1.4

#define VC1_GOOD_COMPRESSION 0.90
#define VC1_POOR_REF_FRAME   0.75

#define VC1_P_SIZE         0.8
#define VC1_B_SIZE         0.6

typedef struct
{
    Ipp8u IQuant;
    Ipp8u PQuant;
    Ipp8u BQuant;

    bool   IHalf;
    bool   PHalf;
    bool   BHalf;

    Ipp8u LimIQuant;
    Ipp8u LimPQuant;
    Ipp8u LimBQuant;
}VC1PicQuant;

typedef struct
{
    Ipp8u PQIndex;
    bool   HalfPQ;
}VC1Quant;

#define VC1_MIN_QUANT      1
#define VC1_MAX_QUANT      31
#define VC1_QUANT_CLIP(quant, lim_quant)                \
        {                                               \
          if ((quant) < (VC1_MIN_QUANT))                \
              (quant) = (VC1_MIN_QUANT);                \
          else if ((quant) > (VC1_MAX_QUANT))           \
              (quant) = (VC1_MAX_QUANT);                \
          if((lim_quant > 0) && (quant < lim_quant))    \
            quant = lim_quant;                          \
        }

#define VC1_CHECK_QUANT(quant)                          \
        {                                               \
          if ((quant) < (VC1_MIN_QUANT))                \
              (quant) = (VC1_MIN_QUANT);                \
          else if ((quant) > (VC1_MAX_QUANT))           \
              (quant) = (VC1_MAX_QUANT);                \
        }

enum
{
    VC1_BRC_OK                      = 0x0,
    VC1_BRC_ERR_BIG_FRAME           = 0x1,
    VC1_BRC_BIG_FRAME               = 0x2,
    VC1_BRC_ERR_SMALL_FRAME         = 0x4,
    VC1_BRC_SMALL_FRAME             = 0x8,
    VC1_BRC_NOT_ENOUGH_BUFFER       = 0x10
};

class VC1BRC
{
public:
    VC1BRC();
    virtual ~VC1BRC();

    virtual UMC::Status Init(Ipp32u /*yuvFSize*/,  Ipp32u /*bitrate*/, Ipp64f /*framerate*/, Ipp32u /*mode*/,
        Ipp32u /*GOPLength*/, Ipp32u /*BFrLength*/, Ipp8u /*doubleQuant*/, Ipp8u /*QuantMode*/){return UMC::UMC_ERR_NOT_IMPLEMENTED;};

    virtual void Reset(){};
    virtual void Close(){};

    //return frame compression staus
    virtual UMC::Status CheckFrameCompression(ePType /*picType*/, Ipp32u /*currSize*/, UMC::Status /*HRDSts*/){return UMC::UMC_ERR_NOT_IMPLEMENTED;};
    virtual void GetQuant(ePType /*picType*/, Ipp8u* /*PQuant*/, bool* /*Half*/){};
    virtual void GetQuant(ePType /*picType*/, Ipp8u* /*PQuant*/, bool* /*Half*/, bool* /*Uniform*/){};

    virtual void CompleteFrame(ePType /*picType*/){};
    virtual UMC::Status SetGOPParams(Ipp32u /*GOPLength*/, Ipp32u /*BFrLength*/){return UMC::UMC_ERR_NOT_IMPLEMENTED;};

protected:
    Ipp32u m_bitrate;           //current bitrate
    Ipp64f m_framerate;         //current frame rate

    //GOP parameters
    Ipp32u m_GOPLength;         //number of frame in GOP
    Ipp32u m_CurrGOPLength;      //current distance between I frames

    Ipp32u m_BFrLength;         //number of successive B frames
    Ipp32s m_SizeAbberation;    //coded size abberation

    //Flags
    Ipp32s m_recoding;         //frame was recoded

    //Quant params
    VC1Quant m_CurQuant;        //current quntization MQUANT and HALFQP
    VC1PicQuant m_Quant;        //current quant set for all picture

    //Frames parameters
    Ipp32s m_currSize;         //last coded frame size
    Ipp32s m_IdealFrameSize;    //"ideal" frame size

    //Picture params
    ePType m_picType;         //current picture type

    //coding params
    Ipp64f m_ratio_min;      //bottom coded frame size ratio
    Ipp64f m_buffer_overflow;//top coded frame size ratio
    Ipp32u m_encMode;        //coding mode
    Ipp8u  m_QuantMode;      //two bits: 1 bit - allow uniform mode coding, 2 bit - allow nonuniform mode
};

////////////////////////////////////////////
//          VC1BRC_I
////////////////////////////////////////////

class VC1BRC_I : public VC1BRC
{
public:
    VC1BRC_I();
    ~VC1BRC_I();
protected:
    Ipp32s m_needISize;        //wishful coded I frame size
public:
    UMC::Status Init(Ipp32u yuvFSize,  Ipp32u bitrate,
                         Ipp64f framerate, Ipp32u mode,
                         Ipp32u GOPLength, Ipp32u BFrLength, Ipp8u doubleQuant, Ipp8u QuantMode);

    void Reset();
    void Close();
    //return frame compression staus
    UMC::Status CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts);
    void CompleteFrame(ePType picType);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform);
    UMC::Status SetGOPParams(Ipp32u /*GOPLength*/, Ipp32u /*BFrLength*/);

protected:
    void CheckFrame_QualityMode    (Ipp32f ratio, UMC::Status HRDSts);
    void CorrectGOPQuant(Ipp32f ratio);
    Ipp32s m_prefINeedSize;

};

////////////////////////////////////////////
//          VC1BRC_IP
////////////////////////////////////////////
class VC1BRC_IP : public VC1BRC
{
public:
    VC1BRC_IP();
    ~VC1BRC_IP();
protected:
    Ipp32s m_I_GOPSize;         //I frame size in current GOP
    Ipp32s m_P_GOPSize;         //P frames size in current GOP

    Ipp8u  m_failPQuant;        //need when P ratio >> 1.5
    Ipp32s m_failGOP;        //need when P rati0 >> 1.5

    Ipp32s m_needISize;        //wishful coded I frame size
    Ipp32s m_needPSize;        //wishful coded P frame size

    Ipp32s m_INum;              //number I frame in GOP
    Ipp32s m_PNum;              //number P frame in GOP

    Ipp32s m_CurrINum;          //number coded I frame in current GOP
    Ipp32s m_CurrPNum;          //number coded P frame in current GOP
    Ipp32u m_currFrameInGOP;    //numder of coded frames in current GOP
    Ipp32s m_poorRefFrame;     //last ref frame had poor quality
    Ipp32s m_GOPHalfFlag;      //less than half of GOP were coded

    //Frames parameters
    Ipp32s m_currISize;         //last coded I frame size
    Ipp32s m_currPSize;         //last coded P frame size
    Ipp32s m_GOPSize;           //"ideal" GOP size
    Ipp32s m_currGOPSize;       //current GOP size
    Ipp32s m_nextGOPSize;       //plan GOP size

    Ipp32f m_IP_size;   //I/P frame size ratio
    Ipp32s m_AveragePQuant;
    Ipp32s m_AverageIQuant;
public:
    UMC::Status Init(Ipp32u yuvFSize,  Ipp32u bitrate,
                         Ipp64f framerate, Ipp32u mode,
                         Ipp32u GOPLength, Ipp32u BFrLength, Ipp8u doubleQuant, Ipp8u QuantMode);
    void Reset();
    void Close();

    //return frame compression staus
    UMC::Status CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts);
    void CompleteFrame(ePType picType);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform);
    UMC::Status SetGOPParams(Ipp32u GOPLength, Ipp32u /*BFrLength*/);

protected:
    void CheckFrame_QualityMode    (ePType picType, Ipp32f ratio, UMC::Status HRDSts);
    void CorrectGOPQuant(ePType picType, Ipp32f ratio);

};

////////////////////////////////////////////
//          VC1BRC_IPB
////////////////////////////////////////////
class VC1BRC_IPB :  public VC1BRC
{
public:
    VC1BRC_IPB();
    ~VC1BRC_IPB();
protected:
    Ipp32s m_I_GOPSize;         //I frame size in current GOP
    Ipp32s m_P_GOPSize;         //P frames size in current GOP
    Ipp32s m_B_GOPSize;         //B frames size in current GOP


    Ipp8u  m_failPQuant;        //need when P ratio >> 1.5
    Ipp8u  m_failBQuant;        //need when B ratio >> 1.5

    Ipp32s m_failGOP;        //need when P rati0 >> 1.5
    Ipp32s m_needISize;        //wishful coded I frame size
    Ipp32s m_needPSize;        //wishful coded I frame size
    Ipp32s m_needBSize;        //wishful coded I frame size

    Ipp32s m_INum;              //number I frame in GOP
    Ipp32s m_PNum;              //number P frame in GOP
    Ipp32s m_BNum;              //number B frame in GOP

    Ipp32s m_CurrINum;          //number coded I frame in current GOP
    Ipp32s m_CurrPNum;          //number coded P frame in current GOP
    Ipp32s m_CurrBNum;          //number coded B frame in current GOP

    Ipp32u m_currFrameInGOP;    //numder of coded frames in current GOP
    Ipp32s m_frameCount;        //numder of coded frames with current GOP params

    Ipp32s m_poorRefFrame;      //last ref frame had poor quality
    Ipp32s m_GOPHalfFlag;       //less than half of GOP were coded

    //Frames parameters
    Ipp32s m_currISize;         //last coded I frame size
    Ipp32s m_currPSize;         //last coded P frame size
    Ipp32s m_currBSize;         //last coded B frame size
    Ipp32s m_GOPSize;           //"ideal" GOP size
    Ipp32s m_currGOPSize;       //current GOP size
    Ipp32s m_nextGOPSize;       //plan GOP size
    Ipp32f m_IP_size;           //I/P frame size ratio
    Ipp32f m_IB_size;           //I/b frame size ratio

    Ipp32s m_AveragePQuant;
    Ipp32s m_AverageIQuant;
    Ipp32s m_AverageBQuant;

public:
    UMC::Status Init(Ipp32u yuvFSize,  Ipp32u bitrate,
                         Ipp64f framerate, Ipp32u mode,
                         Ipp32u GOPLength, Ipp32u BFrLength, Ipp8u doubleQuant, Ipp8u QuantMode);
    void Reset();
    void Close();

    //return frame compression staus
    UMC::Status CheckFrameCompression(ePType picType, Ipp32u currSize, UMC::Status HRDSts);
    void CompleteFrame(ePType picType);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half);
    void GetQuant(ePType picType, Ipp8u* PQuant, bool* Half, bool* Uniform);
    UMC::Status SetGOPParams(Ipp32u GOPLength, Ipp32u BFrLength);

protected:
    void CheckFrame_QualityMode    (ePType picType, Ipp32f ratio, UMC::Status HRDSts);
    void CorrectGOPQuant(ePType picType, Ipp32f ratio);
};
}

#endif //_ENCODER_VC1_BITRATE_GOP_H_
