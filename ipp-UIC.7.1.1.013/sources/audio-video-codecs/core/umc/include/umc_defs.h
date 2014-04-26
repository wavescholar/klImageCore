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

#include "vm_types.h"
#include "umc_dynamic_cast.h"

// Base UMC defines

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

#define UMC_ROUND_32U(DIGIT) ((DIGIT - (Ipp32u)(DIGIT) < 0.5)?(Ipp32u)(DIGIT):(Ipp32u)(DIGIT + 1))

/***************************************************************************/

#define UMC_CHECK(EXPRESSION, ERR_CODE) \
    if(!(EXPRESSION)) return ERR_CODE;

#define UMC_CHECK_STATUS(STATUS) \
    UMC_CHECK((STATUS) == UMC_OK, STATUS)

#define UMC_CHECK_FUNC(STATUS, FUNC) \
    UMC_CHECK((STATUS = FUNC) == UMC_OK, STATUS)

#define UMC_CHECK_PTR(PTR) \
    UMC_CHECK((PTR) != NULL, UMC_ERR_NULL_PTR)

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
    if(PTR) { \
        delete PTR; \
        PTR = NULL; \
    } \
}

#define UMC_DELETE_ARR(PTR) { \
    if(PTR) { \
        delete[] PTR; \
        PTR = NULL; \
    } \
}

#define UMC_ALLOC(PTR, TYPE) { \
    UMC_FREE(PTR); \
    PTR = (TYPE*)ippMalloc(sizeof(TYPE)); \
    UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_ALLOC_ARR(PTR, TYPE, NUM) { \
    UMC_FREE(PTR);    \
    PTR = (TYPE*)ippMalloc((NUM)*sizeof(TYPE)); \
    UMC_CHECK(PTR != NULL, UMC_ERR_ALLOC); \
}

#define UMC_ALLOC_ZERO_ARR(PTR, TYPE, NUM) { \
    UMC_ALLOC_ARR(PTR, TYPE, NUM); \
    ippsSet_8u(0, (Ipp8u*)(PTR), (NUM)*sizeof(TYPE)); \
}

#define UMC_FREE(PTR) { \
    if(PTR) { \
        ippFree(PTR); \
        PTR = NULL; \
    } \
}

#define UMC_ARRAY_SIZE(ARR) (sizeof(ARR)/sizeof(ARR[0]))

#define UMC_SET_ZERO(VAR)   ippsSet_8u(0, (Ipp8u*)&(VAR), sizeof(VAR))

/***************************************************************************/

#define MAX_COLOR_PLANES    4
#define DEFAULT_ALIGN_VALUE 16

#define UMC_NTS2(x) #x
#define UMC_NTS(x) UMC_NTS2(x)
#define UMC_DEPRECATED_MESSAGE(CODEC) ""
#define UMC_DEPRECATED_REMOVE_MESSAGE(CODEC) __FILE__ "(" UMC_NTS(__LINE__) "): UMC "CODEC" is deprecated.  This component is obsolete and will be removed in one of the future IPP releases. Use the following link for details: http://software.intel.com/en-us/articles/intel-ipp-71-deprecated-features/"

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
    FLV_STREAM              = 0x08000000,
    IVF_STREAM              = 0x10000000,
    OGG_STREAM              = 0x10100000,
    MJPEG_STREAM            = 0x20000000
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
    AAC_MPEG4_STREAM        = 0x00000011,

    AMR_NB_AUDIO            = 0x00000777,  //narrow band amr
    AMR_WB_AUDIO            = 0x00000778   //wide band amr
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
    VC1_VIDEO               = 0x00050000,
    AVS_VIDEO               = 0x00060000,
    VP8_VIDEO               = 0x00070000
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
    WMV3_VIDEO,
    MULTIVIEW_VIDEO         = 0x01000000,
    SCALABLE_VIDEO          = 0x02000000
};

enum WaveFormat
{
    UMC_WAVE_FORMAT_PCM        = 1,
    UMC_WAVE_FORMAT_MPEGLAYER3 = 0x0055,
    UMC_WAVE_FORMAT_DVM        = 0x2000,
    UMC_WAVE_FORMAT_SPEECH     = 0x4D41,
    UMC_WAVE_FORMAT_AAC        = 0x5473,
    UMC_WAVE_FORMAT_EXTENSIBLE = 0xFFFE,
};

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

enum DataReaderType
{
    DEF_DATA_READER = 0,
    FILE_DATA_READER,
};

enum DataWriterType
{
    DEF_DATA_WRITER = 0,
    FILE_DATA_WRITER
};

enum VideoRenderType
{
    DEF_VIDEO_RENDER = 0,
    GDI_VIDEO_RENDER,
    OPENGL_VIDEO_RENDER,
    NULL_VIDEO_RENDER,
    FW_VIDEO_RENDER,
};

enum AudioRenderType
{
    DEF_AUDIO_RENDER = 0,
    DSOUND_AUDIO_RENDER,
    WINMM_AUDIO_RENDER,
    OSS_AUDIO_RENDER,
    NULL_AUDIO_RENDER,
    FW_AUDIO_RENDER,
};

enum VideoFilter
{
    FILTER_DEPTH_CONVERTER = 0,
    FILTER_COLOR_CONVERTER,
    FILTER_DEINTERLACER,
    FILTER_RESIZER,
    FILTER_ANAGLYPHER
};

enum ColorFormat
{
    NONE    = -1,
    YV12    =  0,   // Planar Y, V, U (4:2:0) (note V,U order!)
    NV12    ,       // Planar Y, merged U->V (4:2:0)
    YUV420  ,       // Planar Y, U, V (4:2:0)
    YUV422  ,       // Planar Y, U, V (4:2:2)
    YUV444  ,       // Planar Y, U, V (4:4:4)
    YUV411  ,       // Planar Y, U, V (4:1:1)
    Y41P    ,       // Packed Y, U, V (4:1:1)
    YUV420A ,       // Planar Y, U, V, Alpha
    YUV422A ,       // Planar Y, U, V, Alpha
    YUV444A ,       // Planar Y, U, V, Alpha
    YVU9    ,       // Planar Y, U, V
    Y411    ,       // Composite Y, U, V (4:1:1)
    YUY2    ,       // Composite Y->U->Y->V (4:2:2)
    UYVY    ,       // Composite U->Y->V->Y (4:2:2)
    GRAY    ,       // Luminance component only.
    GRAYA   ,       // Luminance with Alpha
    RGBA    ,       // Composite R->G->B->A
    RGB     ,       // Composite R->G->B
    RGB565  ,       // Composite R->G->B, 5 bit per B & R, 6 bit per G
    RGB555  ,       // Composite R->G->B->A, 5 bit per component, 1 bit per A
    RGB444  ,       // Composite R->G->B->A, 4 bit per component
    BGRA    ,       // Composite B->G->R->A
    BGR     ,       // Composite B->G->R
    BGR565  ,       // Composite B->G->R, 5 bit per B & R, 6 bit per G
    BGR555  ,       // Composite B->G->R->A, 5 bit per component, 1 bit per A
    BGR444  ,       // Composite B->G->R->A, 4 bit per component
    CMYK    ,       // Composite C, M, Y, K
    CMYKA   ,       // Composite C, M, Y, K, Alpha
    D3D_SURFACE,    // Pointer to D3D surface
    LVA_SURFACE     // Pointer to LVA surface
};

enum SampleFormat
{
    SF_UNSIGNED = 0,
    SF_SIGNED,
    SF_FLOAT,
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

enum PictureStructure
{
    PS_PROGRESSIVE = 0,
    PS_TOP_FIELD,
    PS_BOTTOM_FIELD,
    PS_TOP_FIELD_FIRST,
    PS_BOTTOM_FIELD_FIRST
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

enum // values to select video or audio output channels
{
    SELECT_ANY_VIDEO_PID    = 0x00000000, //ask for one of available video streams
    SELECT_ANY_AUDIO_PID    = 0x00000000, //ask for one of available audio streams
    SELECT_ALL_AUDIO_PIDS   = 0xffffffff, //ask for all of available audio streams
    SELECT_ALL_VIDEO_PIDS   = 0xffffffff  //ask for all of available video streams
};

enum
{
    SINGLE_CLIENT     = 0,    // Connection oriented with single client per server
    MULTIPLE_CLIENTS,         // Connection oriented with multiple clients per server
    BROADCAST                 // Connection less mode
};

enum eUMC_Status
{
    UMC_ERR_FAILED                = VM_OPERATION_FAILED, //-999,
    UMC_ERR_NOT_INITIALIZED       = VM_NOT_INITIALIZED,  //-998,
    UMC_ERR_TIMEOUT               = VM_TIMEOUT,          //-987,
    UMC_ERR_NOT_ENOUGH_DATA       = VM_NOT_ENOUGH_DATA,  //-996, // not enough input data
    UMC_ERR_NULL_PTR              = VM_NULL_PTR,                 // null pointer in input parameters
    UMC_ERR_INIT                  =-899,                         // failed to initialize codec
    UMC_ERR_SYNC                  =-897,                         // can't find sync word in buffer or requre additional sync
    UMC_ERR_NOT_ENOUGH_BUFFER     =-896,                         // not enough buffer to put output data
    UMC_ERR_END_OF_STREAM         =-895,
    UMC_ERR_OPEN_FAILED           =-894,                         // failed to open file/device
    UMC_ERR_ALLOC                 =-883,                         // failed to allocate memory
    UMC_ERR_LOCK                  =-882,                         // failed to lock memory
    UMC_ERR_INVALID_STREAM        =-881,
    UMC_ERR_UNSUPPORTED           =-879,
    UMC_ERR_NOT_IMPLEMENTED       =-878,
    UMC_ERR_INVALID_PARAMS        =-876,
    UMC_OK                        = VM_OK,               //0,    // no error
    UMC_WRN_INVALID_STREAM        = 1,
    UMC_WRN_REPOSITION_INPROGRESS = 2,
    UMC_WRN_INFO_NOT_READY        = 3,
    UMC_WRN_SKIP                  = 4,
    UMC_WRN_NEW_FRAME             = 5
};

struct RECT
{
    RECT()
    {
        iLeft   = 0;
        iTop    = 0;
        iRight  = 0;
        iBottom = 0;
    }

    Ipp16s iLeft;
    Ipp16s iTop;
    Ipp16s iRight;
    Ipp16s iBottom;

    inline void SwapBigEndian()
    {
        iLeft   = BIG_ENDIAN_SWAP16(iLeft);
        iTop    = BIG_ENDIAN_SWAP16(iTop);
        iRight  = BIG_ENDIAN_SWAP16(iRight);
        iBottom = BIG_ENDIAN_SWAP16(iBottom);
    }
};

typedef int Status;

}

#endif
