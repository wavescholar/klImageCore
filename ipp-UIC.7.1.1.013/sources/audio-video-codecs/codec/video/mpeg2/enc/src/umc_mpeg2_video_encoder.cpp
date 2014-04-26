/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER

#include "vm_strings.h"
#include "umc_video_data.h"
#include "umc_video_encoder.h"
#include "umc_mpeg2_video_encoder.h"
#include "umc_mpeg2_enc.h"


using namespace UMC;

//////////////////////////////////////////////////////////////////////
// class MPEG2VideoEncoder: public VideoEncoder implementation
// Just calls MPEG2VideoEncoderBase

//constructor
MPEG2VideoEncoder::MPEG2VideoEncoder()
{
  encoder = new MPEG2VideoEncoderBase;
}

//destructor
MPEG2VideoEncoder::~MPEG2VideoEncoder()
{
  delete ((MPEG2VideoEncoderBase*)encoder);
}

// Initialize codec with specified parameter(s)
Status MPEG2VideoEncoder::Init(BaseCodecParams *init)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->Init(init);
  return ret;
}

// Compress (decompress) next frame
Status MPEG2VideoEncoder::GetFrame(MediaData *in, MediaData *out)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->GetFrame(in, out);
  return ret;
}

// Compress (decompress) next frame
Status MPEG2VideoEncoderBase::GetFrame(MediaData *in, MediaData *out)
{
  VideoData *vin;
  Ipp32s user_data_size;
  size_t data_size;
  Status ret;

  if (out == NULL) {
    return UMC_ERR_NULL_PTR;
  }

  vin = DynamicCast<VideoData, MediaData> (in);
  if (in != NULL) {
    if (vin == NULL) {
      m_UserData = *in;
      return UMC_OK;
    }

    user_data_size = (Ipp32s)m_UserData.GetDataSize();
    if(user_data_size > 0) {
      setUserData((vm_char*)m_UserData.GetDataPointer(), user_data_size);
    }
  }
  data_size = out->GetDataSize();

  ret = TakeNextFrame(vin);
  if(ret == UMC_OK) {
    LockBuffers();
    PrepareFrame();

    // move to some above functions or new
    out_pointer = (Ipp8u*)out->GetDataPointer() + out->GetDataSize();
    output_buffer_size = (Ipp32s)((Ipp8u*)out->GetBufferPointer() + out->GetBufferSize() - out_pointer);

    ret = EncodeFrameReordered();

    out->SetDataSize(out->GetDataSize() + mEncodedSize);
    out->m_frameType = (picture_coding_type == MPEG2_B_PICTURE ? B_PICTURE
      : (picture_coding_type == MPEG2_P_PICTURE ? P_PICTURE : I_PICTURE));

    UnlockBuffers();
  }
  ComputeTS(vin, out);
  FrameDone(vin);
  //  VM_ASSERT(_CrtCheckMemory());

  if(ret == UMC_OK && m_UserData.GetDataSize() > 0) {
    setUserData(0, 0);
    m_UserData.SetDataSize(0);
  }

  if(data_size == out->GetDataSize() && in == 0)
    return UMC_ERR_NOT_ENOUGH_DATA;

  if (vin != NULL) {
    vin->SetDataSize(0);
  }

  return ret;
}

// THIS METHOD IS TO BE DELETED!
Status MPEG2VideoEncoder::RepeatLastFrame(Ipp64f PTS, MediaData *out)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->RepeatLastFrame(PTS, out);
  return ret;
}

// THIS METHOD IS TO BE DELETED!
Status MPEG2VideoEncoderBase::RepeatLastFrame(Ipp64f /*PTS*/, MediaData * /*out*/)
{
  return UMC_ERR_NOT_IMPLEMENTED;
}

// THIS METHOD IS TO BE DELETED!
Status MPEG2VideoEncoder::GetNextYUVBuffer(VideoData *data)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->GetNextYUVBuffer(data);
  return ret;
}

// THIS METHOD IS TO BE DELETED!
Status MPEG2VideoEncoderBase::GetNextYUVBuffer(VideoData *data)
{
  VideoData* next = GetNextYUVPointer();
  if(next == NULL)
    return UMC_ERR_FAILED;
  *data = *next;
  return UMC_OK;
}

// Get codec working (initialization) parameter(s)
Status MPEG2VideoEncoder::GetInfo(BaseCodecParams *info)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->GetInfo(info);
  return ret;
}

// Close all codec resources
Status MPEG2VideoEncoder::Close()
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->Close();
  return ret;
}

Status MPEG2VideoEncoder::Reset()
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->Reset();
  return ret;
}

Status MPEG2VideoEncoderBase::Reset()
{
  return SetParams(0); // it made so
}

Status MPEG2VideoEncoder::SetParams(BaseCodecParams* params)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->SetParams(params);
  return ret;
}

Status MPEG2VideoEncoderBase::SetParams(BaseCodecParams* params)
{
  Status ret;
  ret = Init(params);
  return ret;
}

Status MPEG2VideoEncoder::SetBitRate(Ipp32s BitRate)
{
  Status ret;
  if(encoder == 0)
    return UMC_ERR_NOT_INITIALIZED;
  ret = ((MPEG2VideoEncoderBase*)encoder)->SetBitRate(BitRate);
  return ret;
}

Status MPEG2VideoEncoderBase::SetBitRate(Ipp32s BitRate)
{
  InitRateControl(BitRate);
  return UMC_OK;
}

#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
