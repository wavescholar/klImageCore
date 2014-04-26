/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, VC1 encoder base class
//
*/

#ifndef _ENCODER_VC1_H_
#define _ENCODER_VC1_H_

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("VC-1 encoder"))

#include "umc_video_encoder.h"
#include "umc_dynamic_cast.h"

namespace UMC
{
class VC1EncoderParams : public VideoEncoderParams
{
public:
    DYNAMIC_CAST_DECL(VC1EncoderParams, VideoEncoderParams)

    VC1EncoderParams():
        m_uiGOPLength(0),
        m_uiBFrmLength(0),
        m_bVSTransform(false),
        m_bDeblocking(false),
        m_iConstQuant(0),
        m_uiNumFrames(0),
        m_uiMESearchSpeed(66),
        m_uiHRDBufferSize(0),
        m_uiHRDBufferInitFullness(0),
        m_pStreamName(0),
        m_bFrameRecoding(false),
        m_bMixed(false),
        m_bOrigFramePred(false),
        m_bInterlace(false),
        m_uiReferenceFieldType(0),
        m_bSceneAnalyzer(false),
        m_bUseMeFeedback(true),
        m_bUseUpdateMeFeedback(false),
        m_bUseFastMeFeedback(false),
        m_bFastUVMC(true),
        m_uiQuantType(3),
        m_bSelectVLCTables(0),
        m_bChangeInterpPixelType(0),
        m_bUseTreillisQuantization(0)

    {};
    virtual ~VC1EncoderParams() {};
    virtual Status ReadParams(ParserCfg *par);

    Ipp32u              m_uiGOPLength;
    Ipp32u              m_uiBFrmLength;
    bool                m_bVSTransform;
    bool                m_bDeblocking;
    Ipp8u               m_iConstQuant;
    Ipp32u              m_uiNumFrames;
    Ipp32u              m_uiMESearchSpeed;
    Ipp32u              m_uiHRDBufferSize;
    Ipp32s              m_uiHRDBufferInitFullness;
    vm_char*            m_pStreamName;
    bool                m_bFrameRecoding;
    bool                m_bMixed;
    bool                m_bOrigFramePred;
    bool                m_bInterlace;
    Ipp32u              m_uiReferenceFieldType;
    bool                m_bSceneAnalyzer;
    bool                m_bUseMeFeedback;
    bool                m_bUseUpdateMeFeedback;
    bool                m_bUseFastMeFeedback;
    bool                m_bFastUVMC;
    Ipp32u              m_uiQuantType;
    bool                m_bSelectVLCTables;
    bool                m_bChangeInterpPixelType;
    bool                m_bUseTreillisQuantization;
};


class VC1VideoEncoder: public VideoEncoder
{
public:
    DYNAMIC_CAST_DECL(VC1VideoEncoder, VideoEncoder)

    VC1VideoEncoder();
    ~VC1VideoEncoder();

    virtual Status Init     (BaseCodecParams *init);
    virtual Status SetParams(BaseCodecParams* params);
    virtual Status GetInfo  (BaseCodecParams *info);

    virtual Status Close    ();
    virtual Status Reset    ();

    virtual Status GetFrame (MediaData *in, MediaData *out);

protected:

    virtual Status CheckParameters() {return UMC_OK;}

private:

    void*     m_pEncoderSM;
    void*     m_pEncoderADV;
    bool      m_bAdvance;

};

}

#endif
