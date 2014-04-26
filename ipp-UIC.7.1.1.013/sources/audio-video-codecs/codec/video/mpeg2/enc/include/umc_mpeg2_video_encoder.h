/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//  Purpose
//    Interface class for UMC mpeg2 video encoder.
//
*/

#ifndef __UMC_MPEG2_VIDEO_ENCODER_H
#define __UMC_MPEG2_VIDEO_ENCODER_H

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("MPEG-2 encoder"))

#include "umc_video_data.h"
#include "umc_video_encoder.h"

#include "ippdefs.h"

namespace UMC
{

// rate control modes for MPEG2EncoderParams::rc_mode
// constant, [unrestricted]variable, restricted variable
#define RC_CBR 1
#define RC_VBR 2
#define RC_UVBR RC_VBR
#define RC_RVBR (RC_CBR | RC_VBR)

// used to specify motion estimation ranges for different types of pictures
struct MotionData  // motion data
{
  Ipp32s f_code[2][2];      // [forward=0/backward=1][x=0/y=1]
  Ipp32s searchRange[2][2]; // search range, in pixels, -sr <= x < sr
};


class MPEG2EncoderParams : public VideoEncoderParams
{
public:
  DYNAMIC_CAST_DECL(MPEG2EncoderParams, VideoEncoderParams)

  //constructors
  MPEG2EncoderParams();
  MPEG2EncoderParams(MPEG2EncoderParams *par);
  virtual ~MPEG2EncoderParams();

  Status ReadParams(ParserCfg *par); // opens and reads MSSG standard par-file
  Status ReadQMatrices(DString sIntraQMatrixFName, DString sNonIntraQMatrixFName);
  Status Profile_and_Level_Checks();
  Status RelationChecks();

  Ipp32s                   lFlags;                  // FLAG_VENC_REORDER or 0

  // sequence params
  Ipp32s                   mpeg1;                   // 1 - mpeg1, 0 - mpeg2
  Ipp32s                   IPDistance;              // distance between reference frames
  Ipp32s                   gopSize;                 // size of GOP

  // frame params
  Ipp32s                   FieldPicture;            // field or frame picture (if prog_frame=> frame)
  Ipp32s                   VBV_BufferSize;          // in 16 kbit units
  Ipp32s                   low_delay;               // only 0 supported

  //// params for each type of frame
  //MotionData               *pMotionData;            // motion estimation ranges for P, B0, B1, ...
  Ipp32s                   rangeP[2];               // motion estimation range for P [x,y]
  Ipp32s                   rangeB[2][2];            // motion estimation range for B [near,far][x,y]

  Ipp32s                   frame_pred_frame_dct[3]; // only frame dct and prediction
  Ipp32s                   intraVLCFormat[3];       // vlc table 0 or 1
  Ipp32s                   altscan_tab[3];          // zigzag or alternate scan

  Ipp32s                   CustomIntraQMatrix;
  Ipp32s                   CustomNonIntraQMatrix;
  VM_ALIGN16_DECL(Ipp16s)  IntraQMatrix[64];
  VM_ALIGN16_DECL(Ipp16s)  NonIntraQMatrix[64];

  Ipp32s                   me_alg_num;              // 1-local,2-log,3-both,9-full,+10-fullpixel
  Ipp32s                   me_auto_range;           // adjust search range
  Ipp32s                   allow_prediction16x8;

  Ipp32s                   rc_mode;                 // rate control mode, default RC_CBR
  Ipp32s                   quant_vbr[3];            // quantizers for VBR modes

  // performance info for output
  Ipp64f                   performance;
  Ipp64f                   encode_time;
  Ipp64f                   motion_estimation_perf;

  // these fields are currently ignored
  Ipp32s                   inputtype;   // format of input raw data
  Ipp32s                   constrparms; // used only with mpeg1
  // sequence display extension // also uses dst_ from base class
  Ipp32s                   video_format, color_primaries, transfer_characteristics;
  Ipp32s                   matrix_coefficients;
  Ipp32s                   conceal_tab[3];

  vm_char*                 UserData;                // current user data
  Ipp32s                   UserDataLen;             // current user data length, set to 0 after is used
  vm_char                  idStr[256];       // default user data to put to each sequence

};

class MPEG2VideoEncoder : public VideoEncoder
{
public:
  DYNAMIC_CAST_DECL(MPEG2VideoEncoder, VideoEncoder)

  MPEG2VideoEncoder();
  ~MPEG2VideoEncoder();

  // Initialize codec with specified parameter(s)
  virtual Status Init(BaseCodecParams *init);
  // Compress (decompress) next frame
  virtual Status GetFrame(MediaData *in, MediaData *out);
  // Get codec working (initialization) parameter(s)
  virtual Status GetInfo(BaseCodecParams *info);
  // Repeat last frame
  Status RepeatLastFrame(Ipp64f PTS, MediaData *out);
  // Get buffer for next frame
  Status GetNextYUVBuffer(VideoData *data);

  // Close all codec resources
  virtual Status Close();

  virtual Status Reset();

  virtual Status SetParams(BaseCodecParams* params);

  virtual Status SetBitRate(Ipp32s BitRate);

private:
  void * encoder;
  MPEG2VideoEncoder(const MPEG2VideoEncoder&) {}
  const MPEG2VideoEncoder& operator=(const MPEG2VideoEncoder&) { return *this; }
};

} // end namespace UMC

#endif // __UMC_MPEG2_VIDEO_ENCODER_H
