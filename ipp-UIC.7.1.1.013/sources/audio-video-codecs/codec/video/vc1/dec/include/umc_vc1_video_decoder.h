/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VC1_VIDEO_DECODER_H_
#define __UMC_VC1_VIDEO_DECODER_H_

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("VC-1 decoder"))

#include "umc_video_decoder.h"
#include "umc_vc1_dec_frame_descr.h"
#include "umc_vc1_dec_thread.h"
#include "umc_vc1_dec_skipping.h"

class MFXVideoDECODEVC1;
namespace UMC
{

    class VC1TSHeap;
    class VC1VideoDecoder : public VideoDecoder
    {
        friend class VC1TaskStore;
        friend class MFXVideoDECODEVC1;

    public:
      DYNAMIC_CAST_DECL(VC1VideoDecoder, VideoDecoder)

        // Default constructor
        VC1VideoDecoder();
        // Default destructor
        virtual ~VC1VideoDecoder();

        // Initialize for subsequent frame decoding.
        virtual Status Init(BaseCodecParams *init);

        // Get next frame
        virtual Status GetFrame(MediaData* in, MediaData* out);
        // Close  decoding & free all allocated resources
        virtual Status Close(void);

        // Reset decoder to initial state
        virtual Status   Reset(void);

        // Get video information, valid after initialization
        virtual Status GetInfo(BaseCodecParams* inInfo);

        Status GetPerformance(Ipp64f /**perf*/);

        //reset skip frame counter
        Status    ResetSkipCount();

        // increment skip frame counter
        Status    SkipVideoFrame(Ipp32s);

        // get skip frame counter statistic
        Ipp32u    GetNumOfSkippedFrames();

        // Perfomance tools. Improve speed or quality.
        // Change Decoding speed
        Status ChangeVideoDecodingSpeed(Ipp32s& speed_shift);



    protected:

        typedef enum
        {
            Routine,
            FreePostProc,
            MaxSpeed
        } PerformanceMode;

        Status CreateFrameBuffer();
        Status CreateFrameBuffer(Ipp32u bufferSize);
        Status ResizeBuffer();

        void      SetReadFrameSize                     (MediaData* in);
        void      GetFrameSize                         (MediaData* in);
        void      GetPTS                               (Ipp64f in_pts);
        void      GetFPS();



        virtual   bool    InitAlloc                   (VC1Context* pContext, Ipp32u MaxFrameNum);

        void    FreeAlloc                             (VC1Context* pContext);

        virtual Status VC1DecodeFrame                 (VC1VideoDecoder* pDec,MediaData* in, VideoData* out_data);
        virtual Status VC1DecodeLastFrame             (VideoData* out_data);

        Status WriteFrame                             (MediaData* in_UpLeveldata,
                                                       VC1Context* pContext,
                                                       UMC::VideoData* out_data);

        virtual Ipp32u CalculateHeapSize();
        // need to overload for correct H/W support
        Status ProcessPrevFrame               (MediaData* in, VideoData* out_data);

        Status GetStartCodes                  (MediaData* in, MediaDataEx* out, Ipp32u* readSize);
        Status ProcessSeqHeader();
        Status SMProfilesProcessing(Ipp8u* pBitstream);
        virtual Status ContextAllocation(Ipp32u mbWidth,Ipp32u mbHeight);
        Status StartCodesProcessing(Ipp8u*   pBStream,
                                    Ipp32u*  pOffsets,
                                    Ipp32u*  pValues,
                                    bool     IsDataPrepare,
                                    bool     IsNeedToInitCall);

        Status ParseStreamFromMediaData     ();
        Status ParseStreamFromMediaDataEx   (MediaDataEx *in_ex);
        Status ParseInputBitstream          ();
        void   MSWMVPreprocessing           ();


        bool      InitVAEnvironment              (Ipp32u frameSize);
        Status    InitSMProfile                 (VideoDecoderParams *init);

        void    DefineSize();

        Ipp32u  GetCurrentFrameSize()
        {
            if (m_dataBuffer)
                return (Ipp32u)m_frameData->GetDataSize();
            else
                return (Ipp32u)m_pCurrentIn->GetDataSize();
        }
        // Accelarate Decoder (remove some features like deblockng, smoothing or change InvTransform and Quant)
        // speed_mode - return current mode
        // Set Decoding Speed.
        // TBD. If need - move to public methods
        void SetVideoDecodingSpeed(VC1Skipping::SkippingMode SkipMode);


        VideoStreamInfo            m_ClipInfo;
        VC1Context*                m_pContext;
        VC1ThreadDecoder**         m_pdecoder;                              // (VC1ThreadDecoder *) pointer to array of thread decoders
        Ipp32u                     m_iThreadDecoderNum;                     // (Ipp32u) number of slice decoders
        Ipp8u*                     m_dataBuffer;                            //uses for swap data into decoder
        MediaDataEx*               m_frameData;                             //uses for swap data into decoder
        MediaDataEx::_MediaDataEx* m_stCodes;
        Ipp32u                     m_decoderInitFlag;
        Ipp32u                     m_decoderFlags;
        UMC::MemID                 m_iNewMemID;                             // frame memory
        UMC::MemID                 m_iMemContextID;
        UMC::MemID                 m_iHeapID;
        UMC::MemID                 m_iFrameBufferID;
        Ipp64f                     m_pts;
        Ipp64f                     m_pts_dif;

        bool                       m_bIsWMPSplitter;
        Ipp32u                     m_iMaxFramesInProcessing;
        bool                       m_bIsFrameToOut;

        Ipp32s                     m_iRefFramesDst; // destination for reference frames
        Ipp32s                     m_iBFramesDst; // destination for B/BI frames
        VC1FrameDescriptor*        m_pPrevDescriptor;
        Ipp64u                     m_lFrameCount;
        bool                       m_bLastFrameNeedDisplay;
        bool                       m_bIsWarningStream;
        VC1VideoDecoder*           m_pSelfDecoder;
        VC1TaskStore*              m_pStore;
        VideoAccelerator*          m_va;
        PerformanceMode            m_CurrentMode;
        Ipp32u*                    m_pCutBytes;
        VC1TSHeap*                 m_pHeap;

        bool                       m_bIsReorder;
        MediaData*                 m_pCurrentIn;
        VideoData*                 m_pCurrentOut;

        bool                       m_bIsNeedAddFrameSC;
        bool                       m_bIsNeedToShiftIn;

        VideoData                  m_RangeMapY;
        VideoData                  m_RangeMapU;
        VideoData                  m_RangeMapV;

#ifdef CREATE_ES
        FILE*                      m_fPureVideo;
#endif

#ifdef  VC1_THREAD_STATISTIC
        bool PrintParallelStatistic(Ipp32u frameCount,VC1Context* pContext);
        FILE* m_parallelStat;
        VC1ThreadEntry** m_eEntryArray;
#endif
    };


    class VC1VideoDecoderParams:public VideoDecoderParams
    {
    public:
        DYNAMIC_CAST_DECL(VC1VideoDecoderParams, VideoDecoderParams)

        Ipp8u* streamName;
        VC1VideoDecoderParams(){streamName =NULL;};
     };
}


#endif //__UMC_VC1_VIDEO_DECODER_H
