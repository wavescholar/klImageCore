/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_CONFIG_H__
#define __UMC_CONFIG_H__

// This file contains defines which switch on/off support of various components

#if defined(_MSC_VER)
#pragma warning(disable:4206) // warning C4206: nonstandard extension used : translation unit is empty
#endif

/*
// Windows*
*/

#if defined WINDOWS
    // system interfaces
    #define UMC_ENABLE_SYS_WIN // allow use of Win32 window system

    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FILE_READER_MMAP
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
    #define UMC_ENABLE_FW_VIDEO_RENDER
#ifdef UMC_ENABLE_SYS_WIN // window context is required for these renders
    #define UMC_ENABLE_OPENGL_VIDEO_RENDER
    #define UMC_ENABLE_GDI_VIDEO_RENDER
#endif

    // audio renderers
//#ifdef UMC_ENABLE_SYS_WIN // window context is required for DirectSound render
//#if (_MSC_VER >= 1600)
//    #define UMC_ENABLE_DSOUND_AUDIO_RENDER
//#endif
//#endif
    #define UMC_ENABLE_FW_AUDIO_RENDER
    #define UMC_ENABLE_WINMM_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER

    // muxers
    #define UMC_ENABLE_MPEG2_MUXER
    #define UMC_ENABLE_MP4_MUXER

    // video decoders
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER

    // audio decoders
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
#endif

/*
// Linux*
*/

#if defined LINUX
    // system interfaces
    #define UMC_ENABLE_SYS_GLX // allow use of X Window system

    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FILE_READER_MMAP
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
    #define UMC_ENABLE_FW_VIDEO_RENDER
#ifdef UMC_ENABLE_SYS_GLX
    #define UMC_ENABLE_OPENGL_VIDEO_RENDER
#endif

    // audio renderers
    #define UMC_ENABLE_OSS_AUDIO_RENDER
    #define UMC_ENABLE_FW_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER

    // muxers
    #define UMC_ENABLE_MPEG2_MUXER
    #define UMC_ENABLE_MP4_MUXER

    // video decoders
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER

    // audio decoders
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
#endif

/*
// OSX*
*/

#if defined OSX
    // system interfaces
    //#define UMC_ENABLE_SYS_GLX // you need to manually add paths to X11 sdk with glx lib and header

    // readers/writers
    #define UMC_ENABLE_FILE_READER
    #define UMC_ENABLE_FILE_READER_MMAP
    #define UMC_ENABLE_FILE_WRITER

    // video renderers
    #define UMC_ENABLE_FW_VIDEO_RENDER
#ifdef UMC_ENABLE_SYS_GLX
    #define UMC_ENABLE_OPENGL_VIDEO_RENDER
#endif

    // audio renderers
    #define UMC_ENABLE_FW_AUDIO_RENDER

    // splitters
    #define UMC_ENABLE_AVI_SPLITTER
    #define UMC_ENABLE_MPEG2_SPLITTER
    #define UMC_ENABLE_MP4_SPLITTER
    #define UMC_ENABLE_H264_SPLITTER
    #define UMC_ENABLE_VC1_SPLITTER

    // muxers
    #define UMC_ENABLE_MPEG2_MUXER
    #define UMC_ENABLE_MP4_MUXER

    // video decoders
    #define UMC_ENABLE_H264_VIDEO_DECODER
    #define UMC_ENABLE_MPEG2_VIDEO_DECODER
    #define UMC_ENABLE_MPEG4_VIDEO_DECODER
    #define UMC_ENABLE_MJPEG_VIDEO_DECODER
    #define UMC_ENABLE_VC1_VIDEO_DECODER

    // video encoders
    #define UMC_ENABLE_H264_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG2_VIDEO_ENCODER
    #define UMC_ENABLE_MPEG4_VIDEO_ENCODER
    #define UMC_ENABLE_VC1_VIDEO_ENCODER

    // audio decoders
    #define UMC_ENABLE_AAC_AUDIO_DECODER
    #define UMC_ENABLE_MP3_AUDIO_DECODER
    #define UMC_ENABLE_AC3_AUDIO_DECODER

    // audio encoders
    #define UMC_ENABLE_AAC_AUDIO_ENCODER
    #define UMC_ENABLE_MP3_AUDIO_ENCODER
#endif

#ifdef UMC_ENABLE_FILE_READER_MMAP
#define UMC_ENABLE_FILE_READER
#endif

#endif
