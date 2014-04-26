/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DEFS_H__
#define __UMC_DEFS_H__

// This file contains defines which switch on/off support of various components

#if defined(_MSC_VER)
#pragma warning(disable:4206) // warning C4206: nonstandard extension used : translation unit is empty
#endif

/*
// Windows* on IA-32 or Intel(R) 64
*/

#if defined(WIN32) || defined (WIN64)
    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FIO_READER
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
    #define UMC_ENABLE_DX_VIDEO_RENDER
    #define UMC_ENABLE_BLT_VIDEO_RENDER
    #define UMC_ENABLE_GDI_VIDEO_RENDER
    #define UMC_ENABLE_FW_VIDEO_RENDER

    // audio renderers
    #define UMC_ENABLE_WINMM_AUDIO_RENDER
    #define UMC_ENABLE_DSOUND_AUDIO_RENDER
    #define UMC_ENABLE_FW_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER
    #define UMC_ENABLE_AVS_SPLITTER
    #define UMC_ENABLE_FLV_SPLITTER
    #define UMC_ENABLE_ASF_SPLITTER

    // video decoders
    #define UMC_ENABLE_DV_VIDEO_DECODER
    #define UMC_ENABLE_DV50_VIDEO_DECODER
    #define UMC_ENABLE_DVHD_VIDEO_DECODER
    #define UMC_ENABLE_H261_VIDEO_DECODER
    #define UMC_ENABLE_H263_VIDEO_DECODER
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER
    #define UMC_ENABLE_AVS_VIDEO_DECODER
    #define UMC_ENABLE_UNCOMPRESSED_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_DV_VIDEO_ENCODER
    #define UMC_ENABLE_DV50_VIDEO_ENCODER
    #define UMC_ENABLE_DVHD_VIDEO_ENCODER
    #define UMC_ENABLE_H261_VIDEO_ENCODER
    #define UMC_ENABLE_H263_VIDEO_ENCODER
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_MJPEG_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER
    #define UMC_ENABLE_AVS_VIDEO_ENCODER


    // audio decoders
    #define UMC_ENABLE_AAC_INT_AUDIO_DECODER
    #define UMC_ENABLE_MP3_INT_AUDIO_DECODER
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
//    #define UMC_ENABLE_AMR_AUDIO_DECODER
    #define UMC_ENABLE_LPCM_AUDIO_DECODER
    #define UMC_ENABLE_DTS_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_INT_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_INT_AUDIO_ENCODER
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_AC3_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
//    #define UMC_ENABLE_AMR_AUDIO_ENCODER
#endif

/*
// Linux* on IA-32 or Intel(R) 64
*/

#if defined(LINUX32) || defined(LINUX64)
    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FIO_READER
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
   #define UMC_ENABLE_FB_VIDEO_RENDER
   #define UMC_ENABLE_SDL_VIDEO_RENDER
    #define UMC_ENABLE_FW_VIDEO_RENDER

    // audio renderers
    #define UMC_ENABLE_ALSA_AUDIO_RENDER
    #define UMC_ENABLE_OSS_AUDIO_RENDER
    #define UMC_ENABLE_SDL_AUDIO_RENDER
    #define UMC_ENABLE_FW_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER
    #define UMC_ENABLE_AVS_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_FLV_SPLITTER

    // video decoders
    #define UMC_ENABLE_DV_VIDEO_DECODER
    #define UMC_ENABLE_DV50_VIDEO_DECODER
    #define UMC_ENABLE_DVHD_VIDEO_DECODER
    #define UMC_ENABLE_H261_VIDEO_DECODER
    #define UMC_ENABLE_H263_VIDEO_DECODER
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER
    #define UMC_ENABLE_AVS_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_DV_VIDEO_ENCODER
    #define UMC_ENABLE_DV50_VIDEO_ENCODER
    #define UMC_ENABLE_DVHD_VIDEO_ENCODER
    #define UMC_ENABLE_H261_VIDEO_ENCODER
    #define UMC_ENABLE_H263_VIDEO_ENCODER
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_MJPEG_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER

    // audio decoders
    #define UMC_ENABLE_AAC_INT_AUDIO_DECODER
    #define UMC_ENABLE_MP3_INT_AUDIO_DECODER
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
    #define UMC_ENABLE_LPCM_AUDIO_DECODER
    #define UMC_ENABLE_DTS_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_INT_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_INT_AUDIO_ENCODER
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_AC3_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
#endif

/*
// OS X* on IA-32
*/

#if defined(OSX32)
    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FIO_READER
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
    #define UMC_ENABLE_AGL_VIDEO_RENDER
    #define UMC_ENABLE_FW_VIDEO_RENDER

    // audio renderers
    #define UMC_ENABLE_COREAUDIO_RENDER
    #define UMC_ENABLE_FW_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER
    #define UMC_ENABLE_AVS_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_FLV_SPLITTER

    // video decoders
    #define UMC_ENABLE_DV_VIDEO_DECODER
    #define UMC_ENABLE_DV50_VIDEO_DECODER
    #define UMC_ENABLE_DVHD_VIDEO_DECODER
    #define UMC_ENABLE_H261_VIDEO_DECODER
    #define UMC_ENABLE_H263_VIDEO_DECODER
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER
    #define UMC_ENABLE_AVS_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_DV_VIDEO_ENCODER
    #define UMC_ENABLE_DV50_VIDEO_ENCODER
    #define UMC_ENABLE_DVHD_VIDEO_ENCODER
    #define UMC_ENABLE_H261_VIDEO_ENCODER
    #define UMC_ENABLE_H263_VIDEO_ENCODER
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_MJPEG_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER

    // audio decoders
    #define UMC_ENABLE_AAC_INT_AUDIO_DECODER
    #define UMC_ENABLE_MP3_INT_AUDIO_DECODER
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
    #define UMC_ENABLE_LPCM_AUDIO_DECODER
    #define UMC_ENABLE_DTS_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_INT_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_INT_AUDIO_ENCODER
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_AC3_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
#endif

#ifdef __cplusplus

// forward declarations
namespace UMC
{
class Splitter;
class BaseCodec;
class VideoDecoder;
class VideoEncoder;
class AudioCodec;
class Muxer;
class MediaBuffer;
class VideoRender;

// create instance of particular component
// should be deleted via delete operator

Splitter *CreateMPEG2Splitter();
Splitter *CreateMPEG2SplitterNT();
Splitter *CreateMPEG4Splitter();
Splitter *CreateAVISplitter();
Splitter *CreateVC1Splitter();
Splitter *CreateASFSplitter();

VideoDecoder *CreateMPEG2Decoder();
VideoDecoder *CreateMPEG4Decoder();
VideoDecoder *CreateH261Decoder();
VideoDecoder *CreateH263Decoder();
VideoDecoder *CreateH264Decoder();
VideoDecoder *CreateVC1Decoder();
VideoDecoder *CreateDVDecoder();
VideoDecoder *CreateDV50Decoder();
VideoDecoder *CreateDV100Decoder();

BaseCodec *CreateVideoProcessing(); // full video processing
BaseCodec *CreateD3DVideoProcessing();
BaseCodec *CreateDeinterlacing();
BaseCodec *CreateColorSpaceConversion();
BaseCodec *CreateVideoResizing();

VideoEncoder *CreateMPEG2Encoder();
VideoEncoder *CreateMPEG4Encoder();
VideoEncoder *CreateH261Encoder();
VideoEncoder *CreateH263Encoder();
VideoEncoder *CreateH264Encoder();
VideoEncoder *CreateVC1Encoder();
VideoEncoder *CreateDVEncoder();
VideoEncoder *CreateDV50Encoder();
VideoEncoder *CreateDV100Encoder();

AudioCodec *CreateAACDecoder();
AudioCodec *CreateAACEncoder();
AudioCodec *CreateMP3Decoder();
AudioCodec *CreateMP3Encoder();
AudioCodec *CreateAC3Decoder();
AudioCodec *CreatePCMDecoder();

// version without floating point operations
AudioCodec *CreateAACDecoderInt();
AudioCodec *CreateAACEncoderInt();
AudioCodec *CreateMP3DecoderInt();
AudioCodec *CreateMP3EncoderInt();

Muxer *CreateMPEG2Muxer();
Muxer *CreateMPEG4Muxer();
Muxer *CreateAVIMuxer();

MediaBuffer *CreateLinearBuffer();
MediaBuffer *CreateSampleBuffer();
MediaBuffer *CreateVideoBuffer();

// Convert decoder to asynchronous decoder.
VideoDecoder *CreateAsynchronousVideoDecoder(VideoDecoder *pDecoder);

VideoRender* CreateGDIVideoRender();
VideoRender* CreateD3DVideoRender();
VideoRender* CreateCRCVideoRender();
VideoRender* CreateFWVideoRender();
VideoRender* CreateLVAVideoRender(void);

} // namespace UMC

#endif //__cplusplus

/******************************************************************************/

#endif // __UMC_DEFS_H__

