/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_STRUCTURES_H__
#define __UMC_STRUCTURES_H__

#include "vm_types.h"
#include "vm_debug.h"
//#include "umc_malloc.h"

#define BSWAP16(x) \
    (Ipp16u) ((x) >> 8 | (x) << 8)

#define BSWAP32(x) \
    (Ipp32u)(((x) << 24) + \
    (((x)&0xff00) << 8) + \
    (((x) >> 8)&0xff00) + \
    ((Ipp32u)(x) >> 24))

#define BSWAP64(x) \
    (Ipp64u)(((x) << 56) + \
    (((x)&0xff00) << 40) + \
    (((x)&0xff0000) << 24) + \
    (((x)&0xff000000) << 8) + \
    (((x) >> 8)&0xff000000) + \
    (((x) >> 24)&0xff0000) + \
    (((x) >> 40)&0xff00) + \
    ((x) >> 56))

#ifdef _BIG_ENDIAN_
#   define BIG_ENDIAN_SWAP16(x) BSWAP16(x)
#   define BIG_ENDIAN_SWAP32(x) BSWAP32(x)
#   define BIG_ENDIAN_SWAP64(x) BSWAP64(x)
#   define LITTLE_ENDIAN_SWAP16(x) (x)
#   define LITTLE_ENDIAN_SWAP32(x) (x)
#   define LITTLE_ENDIAN_SWAP64(x) (x)
#else // _BIG_ENDIAN_
#   define BIG_ENDIAN_SWAP16(x) (x)
#   define BIG_ENDIAN_SWAP32(x) (x)
#   define BIG_ENDIAN_SWAP64(x) (x)
#   define LITTLE_ENDIAN_SWAP16(x) BSWAP16(x)
#   define LITTLE_ENDIAN_SWAP32(x) BSWAP32(x)
#   define LITTLE_ENDIAN_SWAP64(x) BSWAP64(x)
#endif // _BIG_ENDIAN_

// macro to create FOURCC
#ifndef DO_FOURCC
#define DO_FOURCC(ch0, ch1, ch2, ch3) \
    ( (Ipp32u)(Ipp8u)(ch0) | ( (Ipp32u)(Ipp8u)(ch1) << 8 ) | \
    ( (Ipp32u)(Ipp8u)(ch2) << 16 ) | ( (Ipp32u)(Ipp8u)(ch3) << 24 ) )
#endif // DO_FOURCC

/***************************************************************************/

#define UMC_CHECK(EXPRESSION, ERR_CODE) { \
  if (!(EXPRESSION)) { \
    return ERR_CODE; \
  } \
}

#define UMC_CHECK_STATUS(__umcRes) \
  UMC_CHECK(__umcRes == UMC_OK, __umcRes)

#define UMC_CHECK_PTR(PTR) \
  UMC_CHECK(PTR != NULL, UMC_ERR_NULL_PTR)

#define UMC_CALL(FUNC) { \
  Status _umcRes; \
  _umcRes = FUNC; \
  UMC_CHECK_STATUS(_umcRes); \
}

#define UMC_NEW(PTR, TYPE) { \
  UMC_DELETE(PTR); \
  PTR = new TYPE;   \
  UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_NEW_ARR(PTR, TYPE, NUM) { \
  UMC_DELETE_ARR(PTR);    \
  PTR = new TYPE[NUM]; \
  UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_DELETE(PTR) { \
  if (PTR) { \
    delete PTR; \
    PTR = NULL; \
  } \
}

#define UMC_DELETE_ARR(PTR) { \
  if (PTR) { \
    delete[] PTR; \
    PTR = NULL; \
  } \
}

#define UMC_ALLOC(PTR, TYPE) { \
  UMC_FREE(PTR); \
  PTR = (TYPE*)ippsMalloc_8u(sizeof(TYPE)); \
  UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_ALLOC_ARR(PTR, TYPE, NUM) { \
  UMC_FREE(PTR);    \
  PTR = (TYPE*)ippsMalloc_8u((NUM)*sizeof(TYPE)); \
  UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_ALLOC_ZERO_ARR(PTR, TYPE, NUM) { \
  UMC_ALLOC_ARR(PTR, TYPE, NUM); \
  ippsZero_8u((Ipp8u*)(PTR), (NUM)*sizeof(TYPE)); \
}

#define UMC_FREE(PTR) { \
  if (PTR) { \
    ippsFree(PTR); \
    PTR = NULL; \
  } \
}

#define UMC_ARRAY_SIZE(ARR) (sizeof(ARR)/sizeof(ARR[0]))

#define UMC_SET_ZERO(VAR)   memset(&(VAR), 0, sizeof(VAR))

/***************************************************************************/

namespace UMC
{

    enum SystemStreamType
    {
        UNDEF_STREAM            = 0x00000000, //unsupported stream type
        AVI_STREAM              = 0x00000001, //AVI RIFF
        MP4_ATOM_STREAM         = 0x00000010, //ISO/IEC 14496-14 stream
        ASF_STREAM              = 0x00000100, //ASF stream

        H26x_PURE_VIDEO_STREAM  = 0x00100000,
        H261_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00010000,
        H263_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00020000,
        H264_PURE_VIDEO_STREAM  = H26x_PURE_VIDEO_STREAM|0x00040000,

        MPEGx_SYSTEM_STREAM     = 0x00001000,                    //MPEG 1,2 - like system

        MPEG1_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000100,//MPEG 1 system
        MPEG2_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000200,//MPEG 2 system
        MPEG4_SYSTEM_STREAM     = MPEGx_SYSTEM_STREAM|0x00000400,//MPEG 4 system

        MPEGx_PURE_VIDEO_STREAM = MPEGx_SYSTEM_STREAM|0x00000010,//MPEG 1,2 - like pure video data
        MPEGx_PURE_AUDIO_STREAM = MPEGx_SYSTEM_STREAM|0x00000020,//MPEG 1,2 - like pure audio data
        MPEGx_PES_PACKETS_STREAM= MPEGx_SYSTEM_STREAM|0x00000040,//MPEG 1,2 - like pes packets system
        MPEGx_PROGRAMM_STREAM   = MPEGx_SYSTEM_STREAM|0x00000080,//MPEG 1,2 - like program system
        MPEGx_TRANSPORT_STREAM  = MPEGx_SYSTEM_STREAM|0x000000c0,//MPEG 1,2 - like transport system


        MPEG1_PURE_VIDEO_STREAM = MPEG1_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM, //MPEG1 pure video stream
        MPEG1_PURE_AUDIO_STREAM = MPEG1_SYSTEM_STREAM|MPEGx_PURE_AUDIO_STREAM, //MPEG1 pure video stream
        MPEG1_PES_PACKETS_STREAM= MPEG1_SYSTEM_STREAM|MPEGx_PES_PACKETS_STREAM,//MPEG1 pes packets stream
        MPEG1_PROGRAMM_STREAM   = MPEG1_SYSTEM_STREAM|MPEGx_PROGRAMM_STREAM,   //MPEG1 program stream

        MPEG2_PURE_VIDEO_STREAM = MPEG2_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM,//MPEG2 pure video stream
        MPEG2_PURE_AUDIO_STREAM = MPEG2_SYSTEM_STREAM|MPEGx_PURE_AUDIO_STREAM,//MPEG2 pure audio stream
        MPEG2_PES_PACKETS_STREAM= MPEG2_SYSTEM_STREAM|MPEGx_PES_PACKETS_STREAM,//MPEG2 pes packets stream
        MPEG2_PROGRAMM_STREAM   = MPEG2_SYSTEM_STREAM|MPEGx_PROGRAMM_STREAM,   //MPEG2 program stream
        MPEG2_TRANSPORT_STREAM  = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM,  //MPEG2 transport stream
        MPEG2_TRANSPORT_STREAM_TTS  = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM | 1,  //MPEG2 transport stream with valid packet time stamps
        MPEG2_TRANSPORT_STREAM_TTS0 = MPEG2_SYSTEM_STREAM|MPEGx_TRANSPORT_STREAM | 2,  //MPEG2 transport stream with zero packet time stamps

        MPEG4_PURE_VIDEO_STREAM = MPEG4_SYSTEM_STREAM|MPEGx_PURE_VIDEO_STREAM,//MPEG4 pure video stream

        WEB_CAM_STREAM          = 0x00100000,
        ADIF_STREAM             = 0x00200000,
        ADTS_STREAM             = 0x00400000,
        STILL_IMAGE             = 0x00800000,
        VC1_PURE_VIDEO_STREAM   = 0x01000000,
        WAVE_STREAM             = 0x02000000,
        AVS_PURE_VIDEO_STREAM   = 0x04000000,
        FLV_STREAM              = 0x08000000
    };

    enum AudioStreamType
    {
        UNDEF_AUDIO             = 0x00000000,
        PCM_AUDIO               = 0x00000001,
        LPCM_AUDIO              = 0x00000002,
        AC3_AUDIO               = 0x00000004,
        ALAW_AUDIO              = 0x00000006,
        MULAW_AUDIO             = 0x00000007,
        TWINVQ_AUDIO            = 0x00000008,
        DTS_AUDIO               = 0x00000016,

        MPEG1_AUDIO             = 0x00000100,
        MPEG2_AUDIO             = 0x00000200,
        MPEG_AUDIO_LAYER1       = 0x00000010,
        MPEG_AUDIO_LAYER2       = 0x00000020,
        MPEG_AUDIO_LAYER3       = 0x00000040,

        MP1L1_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER1,
        MP1L2_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER2,
        MP1L3_AUDIO             = MPEG1_AUDIO|MPEG_AUDIO_LAYER3,
        MP2L1_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER1,
        MP2L2_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER2,
        MP2L3_AUDIO             = MPEG2_AUDIO|MPEG_AUDIO_LAYER3,

        VORBIS_AUDIO            = 0x00000400,
        AAC_AUDIO               = 0x00000800,
        AAC_FMT_UNDEF           = 0x00000000,   /// Undefined stream format, the decoder have to identify by bitstream
        AAC_FMT_RAW             = 0x00000001,   /// Raw input stream format, the first frame keeps init information
        AAC_FMT_EX_GA           = 0x00000010,   /// GASpecificConfig header within the first frame.
        AAC_MPEG4_STREAM        = AAC_AUDIO | AAC_FMT_RAW | AAC_FMT_EX_GA,

        AMR_NB_AUDIO            = 0x00000777,  //narrow band amr
        AMR_WB_AUDIO            = 0x00000778   //wide band amr
    };

#define WAVE_FORMAT_SPEECH         (0x4D41)

    enum TrickModesType
    {
        UMC_TRICK_MODES_NO          = 0x00000000,
        UMC_TRICK_MODES_FORWARD     = 0x00000001,
        UMC_TRICK_MODES_FAST        = 0x00000002,
        UMC_TRICK_MODES_FASTER      = 0x00000004,
        UMC_TRICK_MODES_SLOW        = 0x00000020,
        UMC_TRICK_MODES_SLOWER      = 0x00000040,
        UMC_TRICK_MODES_REVERSE     = 0x00000200,

        UMC_TRICK_MODES_FFW_FAST    = UMC_TRICK_MODES_FAST   | UMC_TRICK_MODES_FORWARD,
        UMC_TRICK_MODES_FFW_FASTER  = UMC_TRICK_MODES_FASTER | UMC_TRICK_MODES_FORWARD,
        UMC_TRICK_MODES_SFW_SLOW    = UMC_TRICK_MODES_SLOW   | UMC_TRICK_MODES_FORWARD,
        UMC_TRICK_MODES_SFW_SLOWER  = UMC_TRICK_MODES_SLOWER | UMC_TRICK_MODES_FORWARD,

        UMC_TRICK_MODES_FR_FAST     = UMC_TRICK_MODES_FAST   | UMC_TRICK_MODES_REVERSE,
        UMC_TRICK_MODES_FR_FASTER   = UMC_TRICK_MODES_FASTER | UMC_TRICK_MODES_REVERSE,
        UMC_TRICK_MODES_SR_SLOW     = UMC_TRICK_MODES_SLOW   | UMC_TRICK_MODES_REVERSE,
        UMC_TRICK_MODES_SR_SLOWER   = UMC_TRICK_MODES_SLOWER | UMC_TRICK_MODES_REVERSE
    };

    enum AudioStreamSubType
    {
        UNDEF_AUDIO_SUBTYPE     = 0x00000000,
        AAC_LC_PROFILE          = 0x00000001,
        AAC_LTP_PROFILE         = 0x00000002,
        AAC_MAIN_PROFILE        = 0x00000004,
        AAC_SSR_PROFILE         = 0x00000008,
        AAC_HE_PROFILE          = 0x00000010,
        AAC_ALS_PROFILE         = 0x00000020,
        AAC_BSAC_PROFILE        = 0x00000040
    };

    enum VideoStreamType
    {
        UNDEF_VIDEO             = 0x00000000,
        UNCOMPRESSED_VIDEO      = 0x00000001,
        MPEG1_VIDEO             = 0x00000011,
        MPEG2_VIDEO             = 0x00000012,
        MPEG4_VIDEO             = 0x00000014,
        H261_VIDEO              = 0x00000120,
        H263_VIDEO              = 0x00000140,
        H264_VIDEO              = 0x00000180,
        DIGITAL_VIDEO_SD        = 0x00001200,
        DIGITAL_VIDEO_50        = 0x00001400,
        DIGITAL_VIDEO_HD        = 0x00001800,
        DIGITAL_VIDEO_SL        = 0x00002000,
        WMV_VIDEO               = 0x00010000,
        MJPEG_VIDEO             = 0x00020000,
        YV12_VIDEO              = 0x00040000,
        VC1_VIDEO               = 0x00050000,
        AVS_VIDEO               = 0x00060000
    };

    enum VideoRenderType
    {
        DEF_VIDEO_RENDER = 0,
        DX_VIDEO_RENDER,
        BLT_VIDEO_RENDER,
        GDI_VIDEO_RENDER,
        GX_VIDEO_RENDER,
        SDL_VIDEO_RENDER,
        FB_VIDEO_RENDER,
        NULL_VIDEO_RENDER,
        FW_VIDEO_RENDER,
        MTWREG_VIDEO_RENDER,
        OVL2_VIDEO_RENDER,
        DXWCE_VIDEO_RENDER,
        AGL_VIDEO_RENDER,
        NO_VIDEO_RENDER, // no render
        D3D_VIDEO_RENDER,
        LVA_VIDEO_RENDER,
        CRC_VIDEO_RENDER
    };

    enum AudioRenderType
    {
        DEF_AUDIO_RENDER = 0,
        DSOUND_AUDIO_RENDER,
        WINMM_AUDIO_RENDER,
        ALSA_AUDIO_RENDER,
        OSS_AUDIO_RENDER,
        NULL_AUDIO_RENDER,
        FW_AUDIO_RENDER,
        COREAUDIO_RENDER,
        SDL_AUDIO_RENDER
    };

    enum VideoStreamSubType
    {
        UNDEF_VIDEO_SUBTYPE     = 0x00000000,
        MPEG4_VIDEO_DIVX5       = 0x00000001,
        MPEG4_VIDEO_QTIME       = 0x00000002,
        DIGITAL_VIDEO_TYPE_1    = 3,
        DIGITAL_VIDEO_TYPE_2,
        MPEG4_VIDEO_DIVX3,
        MPEG4_VIDEO_DIVX4,
        MPEG4_VIDEO_XVID,
        AVC1_VIDEO,
        H263_VIDEO_SORENSON,
        VC1_VIDEO_RCV           = 0x00110000,
        VC1_VIDEO_VC1           = 0x00120000,
        WVC1_VIDEO,
        WMV3_VIDEO
    };

#ifndef UMC_RESTRICTED_CODE_VA

#define VA_COMBINATIONS(CODEC) \
    CODEC##_SW      = VA_##CODEC | VA_SW, \
    CODEC##_MC      = VA_##CODEC | VA_MC, \
    CODEC##_IT      = VA_##CODEC | VA_IT, \
    CODEC##_VLD     = VA_##CODEC | VA_VLD

    enum VideoAccelerationProfile
    {
        UNKNOWN         = 0,

        // Codecs
        VA_CODEC        = 0x00ff,
        VA_MPEG2        = 0x0001,
        VA_MPEG4        = 0x0002,
        VA_H264         = 0x0003,
        VA_VC1          = 0x0004,

        // Entry points
        VA_ENTRY_POINT  = 0xff00,
        VA_SW           = 0x0100, // no acceleration
        VA_MC           = 0x0200,
        VA_IT           = 0x0300,
        VA_VLD          = 0x0400,
        VA_VLD_SHORT    = 0x0500,
        VA_DEBLOCK      = 0x1000,

        // combinations
        VA_COMBINATIONS(MPEG2),
        VA_COMBINATIONS(MPEG4),
        VA_COMBINATIONS(H264),
        VA_COMBINATIONS(VC1),
        H264_VLD_SHORT  = VA_H264 | VA_VLD_SHORT,
        H264_DBL        = VA_H264 | VA_DEBLOCK
    };

// old names
#define UMCVAProfile    VideoAccelerationProfile
#define MPEG2_FULL      MPEG2_IT
#define MPEG4_VLD_L     MPEG4_VLD
#define H264_VLD_L      H264_VLD
#define VC1_VLD_L       VC1_VLD

    enum VideoAccelerationPlatform
    {
        VA_UNKNOWN_PLATFORM = 0,

        VA_PLATFORM  = 0x0f0000,
        VA_DXVA1     = 0x010000,
        VA_DXVA2     = 0x020000,
        VA_LINUX     = 0x030000,
        VA_SOFTWARE  = 0x040000,

        // Flags
        VA_SIMULATOR = 0x100000,
        VA_ANALYZER  = 0x200000
    };

#endif // UMC_RESTRICTED_CODE_VA

    enum ColorFormat
    {
        NONE    = -1,
        YV12    = 0,    // Planar Y, V, U (4:2:0) (note V,U order!)
        NV12    ,       // Planar Y, merged U->V (4:2:0)
        YUY2    ,       // Composite Y->U->Y->V (4:2:2)
        UYVY    ,       // Composite U->Y->V->Y (4:2:2)
        YUV411  ,       // Planar Y, U, V (4:1:1)
        YUV420  ,       // Planar Y, U, V (4:2:0)
        YUV422  ,       // Planar Y, U, V (4:2:2)
        YUV444  ,       // Planar Y, U, V (4:4:4)
        YUV_VC1 ,       // Planar Y, U, V (4:2:0), VC1 codec specific
        Y411    ,       // Composite Y, U, V (4:1:1)
        Y41P    ,       // Composite Y, U, V (4:1:1)
        RGB32   ,       // Composite B->G->R->A
        RGB24   ,       // Composite B->G->R
        RGB565  ,       // Composite B->G->R, 5 bit per B & R, 6 bit per G
        RGB555  ,       // Composite B->G->R->A, 5 bit per component, 1 bit per A
        RGB444  ,       // Composite B->G->R->A, 4 bit per component
        GRAY    ,       // Luminance component only.
        YUV420A ,       // Planar Y, U, V, Alpha
        YUV422A ,       // Planar Y, U, V, Alpha
        YUV444A ,       // Planar Y, U, V, Alpha
        YVU9    ,       // Planar Y, U, V
        GRAYA   ,       // Luminance with Alpha
        D3D_SURFACE,    // Pointer to D3D surface
        LVA_SURFACE     // Pointer to LVA surface
    };

    enum FrameType
    {
        NONE_PICTURE            = 0,
        I_PICTURE               = 1,
        P_PICTURE               = 2,
        B_PICTURE               = 3,
        D_PICTURE               = 4,
        VIDEO_FRAME             = 0x7,
        AUDIO_FRAME             = 0x8
    };

    enum InterlaceType
    {
        PROGRESSIVE                    = 0,
        //FIELD_PICTURE                  = 1,
        INTERLEAVED_TOP_FIELD_FIRST    = 2,
        INTERLEAVED_BOTTOM_FIELD_FIRST = 3
    };

    enum // decoding flags
    {
        // Init() must only check whether stream is supported
        // (looking to specified video header)
        // and not allocate any resources
        FLAG_VDEC_CHECK_ONLY      = 0x00000002,

        //receiving this flag decoder must output decompressed data
        //in proper display order, otherwise it will output decompressed data
        //in decoding order, application is responsible to reorder frames to
        //before displaying
        FLAG_VDEC_REORDER         = 0x00000004,

        //next flag describes endian related properties of input data
        //when set, means that coded data should be accessed by 4-reading operations
        //for little-endian systems it means that each 4 bytes are swapped
        //i.e [0]<->[3], [1]<->[2]
        //for big-endian systems swapping is not required
        FLAG_VDEC_4BYTE_ACCESS    = 0x00000100

        ////traditional, not UMC specific behavior
        ////original byte order, headers before data, return bytes consumed
        //FLAG_VDEC_COMPATIBLE      = 0x00001000,
    };

    enum // encoding flags
    {
        // The encoder should reorder the incoming frames in the encoding order itself.
        FLAG_VENC_REORDER       = 0x00000004
    };

    enum // video renderer flags
    {
        //render initialized with this flag will render decompressed data from decoder
        //in proper display order
        //see FLAG_VDEC_REORDER flag as pair for this
        FLAG_VREN_REORDER       = 0x00000001,
        FLAG_VREN_CONVERT       = 0x00000002,
        FLAG_VREN_USECOLORKEY   = 0x00000004
    };

    enum AudioChannelConfig
    {
        CHANNEL_FRONT_LEFT      = 0x1,
        CHANNEL_FRONT_RIGHT     = 0x2,
        CHANNEL_FRONT_CENTER    = 0x4,
        CHANNEL_LOW_FREQUENCY   = 0x8,
        CHANNEL_BACK_LEFT       = 0x10,
        CHANNEL_BACK_RIGHT      = 0x20,
        CHANNEL_FRONT_LEFT_OF_CENTER = 0x40,
        CHANNEL_FRONT_RIGHT_OF_CENTER = 0x80,
        CHANNEL_BACK_CENTER     = 0x100,
        CHANNEL_SIDE_LEFT       = 0x200,
        CHANNEL_SIDE_RIGHT      = 0x400,
        CHANNEL_TOP_CENTER      = 0x800,
        CHANNEL_TOP_FRONT_LEFT  = 0x1000,
        CHANNEL_TOP_FRONT_CENTER = 0x2000,
        CHANNEL_TOP_FRONT_RIGHT = 0x4000,
        CHANNEL_TOP_BACK_LEFT   = 0x8000,
        CHANNEL_TOP_BACK_CENTER = 0x10000,
        CHANNEL_TOP_BACK_RIGHT  = 0x20000,
        CHANNEL_RESERVED        = 0x80000000
    };

    enum // audio renderer flags
    {
        FLAG_AREN_VOID          = 0x00000001
    };

    enum // splitter flags
    {
        //invalid value
        UNDEF_SPLITTER             = 0x00000000,
        //audio splitting required in any present in stream
        AUDIO_SPLITTER             = 0x00000001,
        //video splitting required in any present in stream
        VIDEO_SPLITTER             = 0x00000002,
        //example if setup VIDEO_SPLITTER && !set AUDIO_SPLITTER, splitter will ignore
        //any audio elementary stream, only video data request will be valid

        //audio and video splitting required if any present in stream
        AV_SPLITTER                = AUDIO_SPLITTER|VIDEO_SPLITTER,

        //main video header (sequence header) is required to return from Init
        //splitter function, application is responsible to pass it to decoder
        //as a regular video data for properly decoding consequent data
        FLAG_VSPL_VIDEO_HEADER_REQ = 0x00000010,

        //the first video frame is required to return from Init
        //splitter function, application is responsible to pass it to decoder
        //as a regular video data for properly decoding consequent data.
        //The first frame will follow main video header. This flag expands
        //splitter behavior for FLAG_VSPL_VIDEO_HEADER_REQ case
        FLAG_VSPL_VIDEO_FRAME_REQ  = 0x00000020,
        FLAG_VSPL_AUDIO_INFO_REQ   = 0x00000040,
        FLAG_VSPL_VIDEO_INFO_REQ   = 0x00000080,

        //next flag describes endian related properties of input data
        //when set, means that coded data should be accessed by 4-reading operations
        //for little-endian systems it means that each 4 bytes are swapped
        //i.e [0]<->[3], [1]<->[2]
        //for big-endian systems swapping is not required
        FLAG_VSPL_4BYTE_ACCESS     = 0x00000100,

        ////traditional, not UMC specific behavior
        ////original byte order, headers before data, return bytes consumed
        //FLAG_VSPL_COMPATIBLE       = 0x00001000,

        //some splitters may have a behavior to run internal
        //to prohibit asynchronous splitting use this flag
        FLAG_VSPL_NO_INTERNAL_THREAD= 0x00002000,
        // if reposition is not supported

        FLAG_SPL_REPOSITION_DISABLED= 0x00004000
    };

   enum // values to select video or audio output channels
    {
        SELECT_ANY_VIDEO_PID    = 0x00000000, //ask for one of available video streams
        SELECT_ANY_AUDIO_PID    = 0x00000000, //ask for one of available audio streams
        SELECT_ALL_AUDIO_PIDS   = 0xffffffff, //ask for all of available audio streams
        SELECT_ALL_VIDEO_PIDS   = 0xffffffff  //ask for all of available video streams
    };

    enum
    {
        SINGLE_CLIENT           = 0,    // Connection oriented with single client per server
        MULTIPLE_CLIENTS,               // Connection oriented with multiple clients per server
        BROADCAST                       // Connection less mode
    };

    enum
    {
        MAXIMUM_PATH            = 1024
    };

    struct StreamInfo
    {
    };

    typedef struct sAudioStreamInfo : public StreamInfo
    {
        sAudioStreamInfo()
        : channels(0), sample_frequency(0), bitrate(0), bitPerSample(0), duration(0.0), stream_type(UNDEF_AUDIO)
        , stream_subtype(UNDEF_AUDIO_SUBTYPE), channel_mask(0), streamPID(0), is_protected(false), header(0)
        {
        }

        Ipp32s channels;                                        // (Ipp32s) number of audio channels
        Ipp32s sample_frequency;                                // (Ipp32s) sample rate in Hz
        Ipp32u bitrate;                                         // (Ipp32u) bitstream in bps
        Ipp32u bitPerSample;                                    // (Ipp32u) 0 if compressed

        Ipp64f duration;                                        // (Ipp64f) duration of the stream

        AudioStreamType stream_type;                            // (AudioStreamType) general type of stream
        AudioStreamSubType stream_subtype;                      // (AudioStreamSubType) minor type of stream

        Ipp32u channel_mask;                                    // (Ipp32u) channel mask
        Ipp32u streamPID;                                       // (Ipp32u) unique ID

        bool   is_protected;                                    // audio is encrypted
        Ipp32u header;                                          // (Ipp32u) can carry audio header (4-bytes)

    } AudioStreamInfo;

    typedef struct sClipInfo
    {
        Ipp32s width;                                           // (Ipp32s) width of media
        Ipp32s height;                                          // (Ipp32s) height of media

    } ClipInfo;

    typedef struct sVideoStreamInfo : public StreamInfo
    {
        sVideoStreamInfo()
        : color_format(YV12), bitrate(0), aspect_ratio_width(0), aspect_ratio_height(0), framerate(0.0)
        , duration(0.0), interlace_type(PROGRESSIVE), stream_type(UNDEF_VIDEO)
        , stream_subtype(UNDEF_VIDEO_SUBTYPE), streamPID(0), profile(0), level(0)
        {
            clip_info.width = 0;
            clip_info.height = 0;
        }

        ClipInfo            clip_info;                          // (ClipInfo) size of video stream
        ColorFormat         color_format;                       // (ColorFormat) color format of uncompressed video
        Ipp32u              bitrate;                            // (Ipp32u) bitrate of video
        Ipp32s              aspect_ratio_width;                 // (Ipp32s) pixel aspect ratio
        Ipp32s              aspect_ratio_height;                // (Ipp32s) pixel aspect ratio
        Ipp64f              framerate;                          // (Ipp64f) frame rate of video
        Ipp64f              duration;                           // (Ipp64f) duration of media stream
        InterlaceType       interlace_type;                     // (InterlaceType) interlaced info
        VideoStreamType     stream_type;                        // (VideoStreamType) video stream type
        VideoStreamSubType  stream_subtype;                     // (VideoStreamSubType) video stream type
        Ipp32u              streamPID;                          // (Ipp32u) unique ID
        Ipp32s              profile;                            // (Ipp32s) profile
        Ipp32s              level;                              // (Ipp32s) level
    } VideoStreamInfo;

    typedef struct sSystemStreamInfo
    {
        Ipp64f muxrate;                                         // (Ipp64f) stream bitrate
        SystemStreamType stream_type;                           // (SystemStreamType) stream type
    } SystemStreamInfo;

    struct RECT
    {
        RECT():
            left(0),
            top(0),
            right(0),
            bottom(0)
        {}

        Ipp16s left;
        Ipp16s top;
        Ipp16s right;
        Ipp16s bottom;
        inline
        void SwapBigEndian()
        {
            left = BIG_ENDIAN_SWAP16(left);
            top = BIG_ENDIAN_SWAP16(top);
            right = BIG_ENDIAN_SWAP16(right);
            bottom = BIG_ENDIAN_SWAP16(bottom);
        }
    };

    enum eUMC_Status
    {
        UMC_OK                        = VM_OK,               //0,    // no error
        UMC_ERR_FAILED                = VM_OPERATION_FAILED, //-999,
        UMC_ERR_NOT_INITIALIZED       = VM_NOT_INITIALIZED,  //-998,
        UMC_ERR_TIMEOUT               = VM_TIMEOUT,          //-987,
        UMC_ERR_NOT_ENOUGH_DATA       = VM_NOT_ENOUGH_DATA,  //-996, // not enough input data
        UMC_ERR_NULL_PTR              = VM_NULL_PTR,                 // null pointer in input parameters
        UMC_ERR_INIT                  =-899,                         // failed to initialize codec
        UMC_ERR_SYNC                  =-897,                         // can't find sync word in buffer
        UMC_ERR_NOT_ENOUGH_BUFFER     =-896,                         // not enough buffer to put output data
        UMC_ERR_END_OF_STREAM         =-895,
        UMC_ERR_OPEN_FAILED           =-894,                         // failed to open file/device
        UMC_ERR_ALLOC                 =-883,                         // failed to allocate memory
        UMC_ERR_INVALID_STREAM        =-882,
        UMC_ERR_UNSUPPORTED           =-879,
        UMC_ERR_NOT_IMPLEMENTED       =-878,
        UMC_ERR_INVALID_PARAMS        =-876,
        UMC_WRN_INVALID_STREAM        = 1,
        UMC_WRN_REPOSITION_INPROGRESS = 2,
        UMC_WRN_INFO_NOT_READY        = 3
    };

    typedef Ipp32s Status;
#ifdef __cplusplus
    extern "C" {
#endif
    const vm_char* GetErrString(Status ErrCode);
    const vm_char* GetStreamTypeString(SystemStreamType Code);
    const vm_char* GetFormatTypeString(ColorFormat Code);
    const vm_char* GetAudioTypeString(AudioStreamType Code);
    const vm_char* GetVideoTypeString(VideoStreamType Code);
    const vm_char* GetVideoRenderTypeString(VideoRenderType Code);
    const vm_char* GetAudioRenderTypeString(AudioRenderType Code);

    Status GetFormatType(const vm_char *Name, ColorFormat *pCode);
    Status GetStreamType(const vm_char *Name, SystemStreamType *pCode);
    Status GetAudioType(const vm_char *Name, AudioStreamType *pCode);
    Status GetVideoType(const vm_char *Name, VideoStreamType *pCode);
    Status GetAudioRenderType(const vm_char *string, AudioRenderType *pCode);
    Status GetVideoRenderType(const vm_char *string, VideoRenderType *pCode);

#ifndef UMC_RESTRICTED_CODE_VA
    const vm_char* GetVideoAccelerationString(VideoAccelerationProfile code);
    Status GetVideoAccelerationProfile(const vm_char *Name, VideoAccelerationProfile *Code);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

    enum
    {
        DEFAULT_ALIGN_VALUE     = 16
    };

    // forward declaration of template
    template<class T> inline
    T align_pointer(void *pv, size_t lAlignValue = DEFAULT_ALIGN_VALUE)
    {
        // some compilers complain to conversion to/from
        // pointer types from/to integral types.
        return (T) ((((size_t) (pv)) + (lAlignValue - 1)) &
                    ~(lAlignValue - 1));
    }

    // forward declaration of template
    template<class T> inline
    T align_value(size_t nValue, size_t lAlignValue = DEFAULT_ALIGN_VALUE)
    {
        return static_cast<T> ((nValue + (lAlignValue - 1)) &
                               ~(lAlignValue - 1));
    }

} // namespace UMC

#endif /* __UMC_STRUCTURES_H__ */
