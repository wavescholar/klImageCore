/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/
//  MPEG-2 is a international standard promoted by ISO/IEC and
//  other organizations. Implementations of this standard, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.

#ifndef __UMC_MPEG2_DEC_BASE_H
#define __UMC_MPEG2_DEC_BASE_H

//VM headers
#include "vm_debug.h"
#include "vm_thread.h"
#include "vm_event.h"
//UMC headers
#include "umc_structures.h"
#include "umc_video_decoder.h"
#include "umc_sample_buffer.h"
//MPEG-2
#include "umc_mpeg2_dec_bstream.h"
#include "umc_mpeg2_dec_defs.h"
#include "umc_mpeg2_video_decoder.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippvc.h"

namespace UMC
{

#define pack_l pack_w

class MPEG2VideoDecoderBase : public VideoDecoder
{
public:
    ///////////////////////////////////////////////////////
    /////////////High Level public interface///////////////
    ///////////////////////////////////////////////////////
    // Default constructor
    MPEG2VideoDecoderBase(void);

    // Default destructor
    virtual ~MPEG2VideoDecoderBase(void);

    // Initialize for subsequent frame decoding.
    virtual Status Init(BaseCodecParams *init);

    // Get next frame
    virtual Status GetFrame(MediaData* in, MediaData* out);

    // Close  decoding & free all allocated resources
    virtual Status Close(void);

    // Reset decoder to initial state
    virtual Status Reset(void);

    // Get video stream information, valid after initialization
    virtual Status GetInfo(BaseCodecParams* info);

    //reset skip frame counter
    virtual Status          ResetSkipCount();

    // increment skip frame counter
    virtual Status          SkipVideoFrame(Ipp32s);

    // get skip frame counter statistic
    virtual Ipp32u          GetNumOfSkippedFrames();

    //access to the latest decoded frame
    virtual Status PreviewLastFrame(VideoData *out, BaseCodec *pPostProcessing = NULL);

    // returns closed capture data from gop user data
    virtual Status GetUserData(MediaData* pCC);

    virtual Status  SetParams(BaseCodecParams* params);


#ifdef OVERLAY_SUPPORT
    virtual void   SwitchToExternalFrameBuffer(Ipp8u *ext_buffer[], Ipp32s bufstep[]);
    virtual void   SwitchToInternalFrameBuffer(void);
    virtual void*  GetCurrentFramePtr();
    virtual Ipp32s GetNumberOfBuffers();
#endif /* OVERLAY_SUPPORT */

protected:

    Status          UpdateFrameBuffer(void);
    Status          PrepareBuffer(MediaData* data);
    Status          FlushBuffer(MediaData* data, bool);
    Status          LockBuffers();    // Lock buffers
    Status          UnlockBuffers();  // Unlock buffers

    bool            AdjustSpeed(Ipp32s nframe);
    bool            AdjustSpeed(Ipp64f delta);
    Ipp32s          DecreaseSpeed(Ipp64f delta);
    Ipp32s          IncreaseSpeed(Ipp64f delta);
    Ipp32s          IncreaseSpeedN (Ipp32s numoffr);

    bool            m_bNoBframes;
    bool            m_bNoPframes;

    Ipp64f          m_dPlaybackRate;

protected:
    //The purpose of protected interface to have controlled
    //code in derived UMC MPEG2 decoder classes

    ///////////////////////////////////////////////////////
    /////////////Level 1 protected interface///////////////
    ///////////////////////////////////////////////////////
    //Level 1 can call level 2 functions or re-implement it

    // Decode next frame
    Status DecodeFrame(Mpeg2VideoContext  *video,
                        Ipp64f           currentTime,
                        MediaData        *output);

    //Sequence Header search. Stops after header start code
    Status FindSequenceHeader(Mpeg2VideoContext *video);

    //Sequence Header decode
    Status DecodeSequenceHeader(Mpeg2VideoContext *video);

    //Picture Header decode and picture
    Status DecodePicture();

    // Is current picture to be skipped
    bool IsPictureToSkip();

    ///////////////////////////////////////////////////////
    /////////////Level 2 protected interface///////////////
    ///////////////////////////////////////////////////////
    //Level 2 can call level 3 functions or re-implement it

    //Picture Header decode
    Status DecodePictureHeader();
    Status FindSliceStartCode();

    Status DecodeSlices(Ipp32s threadID);

    //Slice decode, includes MB decode
    Status DecodeSlice(Mpeg2VideoContext *video);

    // decode all headers but slice, starts right after startcode
    Status DecodeHeader(Ipp32s startcode, Mpeg2VideoContext *video);

    ///////////////////////////////////////////////////////
    /////////////Level 3 protected interface///////////////
    ///////////////////////////////////////////////////////
    //Level 3 can call level 4 functions or re-implement it

    //Slice Header decode
    Status DecodeSliceHeader(Mpeg2VideoContext *video);

    ///////////////////////////////////////////////////////
    /////////////Level 4 protected interface///////////////
    ///////////////////////////////////////////////////////
    //Level 4 is the lowest level chunk of code
    //can be used as is for basic codec to implement standard
    //or overloaded for special purposes like HWMC, smart decode

    Status DecodeSlice_MPEG1(Mpeg2VideoContext *video);

protected:
    VideoStreamInfo           m_ClipInfo;
    SampleBuffer*             m_pCCData;
    MediaData                 m_ccCurrData;
    sSequenceHeader           sequenceHeader;

    sPictureHeader            PictureHeader;

    mp2_VLCTable              vlcMBAdressing;
    mp2_VLCTable              vlcMBType[3];
    mp2_VLCTable              vlcMBPattern;
    mp2_VLCTable              vlcMotionVector;

    sFrameBuffer              frame_buffer;

    Mpeg2VideoContext**         Video;

    Ipp32u                    m_lFlags;

    Ipp32s                    m_decodedFrameNum;

    Ipp32s                    m_nNumberOfThreads;
    Ipp32s                    m_nNumberOfAllocatedThreads;
    vm_event*                 m_lpQuit;
    vm_event*                 m_lpStopEvent;
    vm_event*                 m_lpStartEvent;
    vm_thread*                m_lpThreads;

#if defined(SLICE_THREADING) || defined(SLICE_THREADING_2)
    vm_mutex                  m_Locker;
    Ipp8u*                    m_bs_curr_ptr;
    Ipp8u*                    m_bs_end_ptr;
#endif
#ifdef SLICE_THREADING_2
    sliceInfo*                m_pSliceInfo;
    bool                      m_endOfFrame;
    Ipp32s                    m_currSliceNum;
    Ipp32s                    m_numOfSlices;
#endif

    class THREAD_ID
    {
    public:
        Ipp32u                m_nNumber;
        void*                 m_lpOwner;
        Status                m_status;
    };

    THREAD_ID*                 m_lpThreadsID;
    //MediaDataEx::_MediaDataEx* m_pStartCodesData;

    Status                  ThreadingSetup(Ipp32s maxThreads);
    static  Ipp32u          VM_THREAD_CALLCONVENTION ThreadWorkingRoutine(void *lpv);

    Status                  SaveFrame(Ipp32s index, Ipp64f dTime, MediaData* output);
    bool                    DeleteTables();
    bool                    InitTables();

    //Status    DecodeBegin(Ipp64f time, sVideoStreamInfo * info);

    void      CalculateFrameTime(Ipp64f in_time, Ipp64f * out_time);

    //Status                  Macroblock_444(Mpeg2VideoContext *video) { return UMC_ERR_INVALID_STREAM; }

    Status                  DecodeSlice_FrameI_420(Mpeg2VideoContext *video);
    Status                  DecodeSlice_FrameI_422(Mpeg2VideoContext *video);
    Status                  DecodeSlice_FramePB_420(Mpeg2VideoContext *video);
    Status                  DecodeSlice_FramePB_422(Mpeg2VideoContext *video);
    Status                  DecodeSlice_FieldPB_420(Mpeg2VideoContext *video);
    Status                  DecodeSlice_FieldPB_422(Mpeg2VideoContext *video);

    Status                  mv_decode(Ipp32s r, Ipp32s s, Mpeg2VideoContext *video);
    Status                  mv_decode_dp(Mpeg2VideoContext *video);
    Status                  update_mv(Ipp16s *pval, Ipp32s s, Mpeg2VideoContext *video);

    Status                  mc_frame_forward_420(Mpeg2VideoContext *video);
    Status                  mc_frame_forward_422(Mpeg2VideoContext *video);
    Status                  mc_field_forward_420(Mpeg2VideoContext *video);
    Status                  mc_field_forward_422(Mpeg2VideoContext *video);

    Status                  mc_frame_backward_420(Mpeg2VideoContext *video);
    Status                  mc_frame_backward_422(Mpeg2VideoContext *video);
    Status                  mc_field_backward_420(Mpeg2VideoContext *video);
    Status                  mc_field_backward_422(Mpeg2VideoContext *video);

    Status                  mc_frame_backward_add_420(Mpeg2VideoContext *video);
    Status                  mc_frame_backward_add_422(Mpeg2VideoContext *video);
    Status                  mc_field_backward_add_420(Mpeg2VideoContext *video);
    Status                  mc_field_backward_add_422(Mpeg2VideoContext *video);

    Status                  mc_fullpel_forward(Mpeg2VideoContext *video);
    Status                  mc_fullpel_backward(Mpeg2VideoContext *video);
    Status                  mc_fullpel_backward_add(Mpeg2VideoContext *video);

    void                    mc_frame_forward0_420(Mpeg2VideoContext *video);
    void                    mc_frame_forward0_422(Mpeg2VideoContext *video);
    void                    mc_field_forward0_420(Mpeg2VideoContext *video);
    void                    mc_field_forward0_422(Mpeg2VideoContext *video);

    Status                  mc_dualprime_frame_420(Mpeg2VideoContext *video);
    Status                  mc_dualprime_frame_422(Mpeg2VideoContext *video);
    Status                  mc_dualprime_field_420(Mpeg2VideoContext *video);
    Status                  mc_dualprime_field_422(Mpeg2VideoContext *video);

    Status                  mc_mp2_420b_skip(Mpeg2VideoContext *video);
    Status                  mc_mp2_422b_skip(Mpeg2VideoContext *video);
    Status                  mc_mp2_420_skip(Mpeg2VideoContext *video);
    Status                  mc_mp2_422_skip(Mpeg2VideoContext *video);

    Ipp32s blkOffsets[3][8];
    Ipp32s blkPitches[3][2];


private:
    void                   sequence_display_extension();
    void                   sequence_scalable_extension();
    void                   picture_temporal_scalable_extension();
    void                   picture_spartial_scalable_extension();
    void                   picture_display_extension();
    void                   copyright_extension();
    void                   quant_matrix_extension();

    void                   ReadCCData();

    IppiSize m_dispSize;

};

}

#endif // __UMC_MPEG2_DEC_BASE_H
