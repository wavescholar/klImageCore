/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_ENC_H
#define __UMC_MPEG2_ENC_H

#include "umc_memory_allocator.h"
#include "umc_video_processing.h"
#include "umc_mpeg2_video_encoder.h"

#include "ippcore.h"
#include "ippvc.h"
#include "ipps.h"
#include "ippi.h"


namespace UMC
{

// to have private mpeg2 types for safety
enum MPEG2FrameType
{
    MPEG2_I_PICTURE               = 1,
    MPEG2_P_PICTURE               = 2,
    MPEG2_B_PICTURE               = 3
};


struct MBInfo  // macroblock information
{
  Ipp32s mb_type;               // intra/forward/backward/interpolated
  Ipp32s dct_type;              // field/frame DCT
  Ipp32s prediction_type;       // MC_FRAME/MC_FIELD
  IppiPoint MV[2][2];           // motion vectors [vecnum][F/B]
  IppiPoint MV_P[2];            // motion vectors from P frame [vecnum]
  Ipp32s mv_field_sel[3][2];    // motion vertical field select:
                                // the first index: 0-top field, 1-bottom field, 2-frame;
                                // the second index:0-forward, 1-backward
  Ipp32s skipped;
};

struct MB_prediction_info
{
  Ipp32s mb_type;
  Ipp32s dct_type;
  Ipp32s pred_type;
  Ipp32s var_sum;
  Ipp32s var[4];
  Ipp32s mean[4];
  Ipp16s *pDiff;
};

struct MpegMotionVector2
{
  Ipp32s x;
  Ipp32s y;
  Ipp32s mctype_l;
  Ipp32s offset_l;
  Ipp32s mctype_c;
  Ipp32s offset_c;
};

struct VLCode_8u // variable length code
{
    Ipp8u code; // right justified
    Ipp8u len;
};

struct VLCode_16u // VL code longer than 8 bits
{
    Ipp16u code; // right justified
    Ipp8u  len;
};

#if defined (_WIN32_WCE) && defined (_M_IX86) && defined (__stdcall)
  #define _IPP_STDCALL_CDECL
  #undef __stdcall
#endif

typedef IppStatus (__STDCALL *functype_getdiff)(
  const Ipp8u*  pSrcCur,
        Ipp32s  srcCurStep,
  const Ipp8u*  pSrcRef,
        Ipp32s  srcRefStep,
        Ipp16s* pDstDiff,
        Ipp32s  dstDiffStep,
        Ipp16s* pDstPredictor,
        Ipp32s  dstPredictorStep,
        Ipp32s  mcType,
        Ipp32s  roundControl);

typedef IppStatus (__STDCALL *functype_getdiffB)(
  const Ipp8u*       pSrcCur,
        Ipp32s       srcCurStep,
  const Ipp8u*       pSrcRefF,
        Ipp32s       srcRefStepF,
        Ipp32s       mcTypeF,
  const Ipp8u*       pSrcRefB,
        Ipp32s       srcRefStepB,
        Ipp32s       mcTypeB,
        Ipp16s*      pDstDiff,
        Ipp32s       dstDiffStep,
        Ipp32s       roundControl);

typedef IppStatus (__STDCALL *functype_mc)(
  const Ipp8u*       pSrcRef,
        Ipp32s       srcStep,
  const Ipp16s*      pSrcYData,
        Ipp32s       srcYDataStep,
        Ipp8u*       pDst,
        Ipp32s       dstStep,
        Ipp32s       mcType,
        Ipp32s       roundControl);

#if defined (_IPP_STDCALL_CDECL)
  #undef  _IPP_STDCALL_CDECL
  #define __stdcall __cdecl
#endif

#define ME_PARAMS             \
  const Ipp8u* pRefFld,       \
  const Ipp8u* pRecFld,       \
  Ipp32s   RefStep,           \
  const Ipp8u* pBlock,        \
  Ipp32s   BlockStep,         \
  Ipp32s*  pSrcMean,          \
  Ipp32s*  pDstVar,           \
  Ipp32s*  pDstMean,          \
  Ipp32s   limit_left,        \
  Ipp32s   limit_right,       \
  Ipp32s   limit_top,         \
  Ipp32s   limit_bottom,      \
  IppiPoint InitialMV0,       \
  IppiPoint InitialMV1,       \
  IppiPoint InitialMV2,       \
  MpegMotionVector2 *vector,   \
  threadSpecificData *th,     \
  Ipp32s   i,                 \
  Ipp32s   j,                 \
  Ipp32s   *vertical_field_select, \
  Ipp32s   *currMAD,          \
  Ipp32s   parity // 0-same

class MPEG2VideoEncoderBase;

struct bitBuffer
{
  Ipp32s bit_offset;
  Ipp32s bytelen;
  Ipp8u  *start_pointer;
  Ipp32u *current_pointer;
};

struct threadSpecificData
{
  bitBuffer bBuf;
  Ipp16s *pMBlock;
  Ipp16s *pDiff;
  Ipp16s *pDiff1;
  IppiPoint PMV[2][2];
  Ipp32s start_row;
  Ipp32s numIntra;
  Ipp32s fieldCount;

  // motion estimation
  Ipp32s me_matrix_size;
  Ipp8u *me_matrix_buff;
  Ipp32s me_matrix_id;
};

struct threadInfo
{
  Ipp32s    numTh;
  MPEG2VideoEncoderBase *m_lpOwner;
  vm_event  start_event;
  vm_event  stop_event;
  vm_event  quit_event;
  vm_thread thread;
};

class MPEG2VideoEncoderBase: public VideoEncoder
{
public:
    DYNAMIC_CAST_DECL(MPEG2VideoEncoderBase, VideoEncoder)

    MPEG2VideoEncoderBase();
    ~MPEG2VideoEncoderBase();

    // Initialize codec with specified parameter(s)
    virtual Status Init(BaseCodecParams *init);
    // Compress (decompress) next frame
    virtual Status GetFrame(MediaData *in, MediaData *out);
    // Get codec working (initialization) parameter(s)
    virtual Status GetInfo(BaseCodecParams *info);
    // Repeat last frame
    Status RepeatLastFrame(Ipp64f PTS, MediaData *out);
    // Close all codec resources
    virtual Status Close();

    virtual Status Reset();

    virtual Status SetParams(BaseCodecParams* params);

    virtual Status SetBitRate(Ipp32s BitRate);

public:
  // Get source frame size (summary size of Y, U and V frames)
  Ipp32s GetYUVFrameSize();

  // Get pointer to internal encoder memory, where
  // next YUV frame can be stored before passing
  // this frame to encode function
  VideoData* GetNextYUVPointer();

  // Get buffer for next frame
  Status GetNextYUVBuffer(VideoData *data);

  // Encode reordered frames (B frames following the corresponding I/P frames).
  // frame_type must be supplied (I_PICTURE, P_PICTURE, B_PICTURE).
  // No buffering because the frames are already reordered.
  Status EncodeFrameReordered();

  void setUserData(vm_char* data, Ipp32s len);

  Ipp32s InitRateControl(Ipp32s BitRate); // inits bit rate control

  void encodePicture(Ipp32s numTh);
protected:

  // used in thread routine
  MPEG2FrameType  picture_coding_type;     // picture coding type (I, P or B)
  void encodeB(Ipp32s numTh);
  void encodeP(Ipp32s numTh);
  void encodeI(Ipp32s numTh);

  MediaData m_UserData; // user data, must have no start codes

  void PrepareBuffers();
  Status FlushBuffers(Ipp32s * size_bytes);
  void PutSequenceHeader();
  void PutSequenceExt();
  void PutSequenceDisplayExt();
  void PutUserData(Ipp32s part); // part: 0-after seq.h., 1-after gop.h., 2-after pic.h.
  void PutGOPHeader(Ipp32s Count);
  void PutPictureHeader();
  void PutPictureCodingExt();
  void PutSequenceEnd();
  void PutSliceHeader(Ipp32s RowNumber, Ipp32s numTh);
  void PutMV(Ipp32s delta, Ipp32s f_code, Ipp32s numTh);
  void PutAddrIncrement(Ipp32s increment, Ipp32s numTh);
  Ipp32s  FrameToTimecode(Ipp32s frame);
  void PutNonIntraBlock(Ipp16s* block, Ipp32s count, Ipp32s numTh);
  void PutIntraBlock(Ipp16s* block, Ipp32s* dc_dct_pred,
                     const IppVCHuffmanSpec_32u* DC_Tbl,
                     Ipp32s count, Ipp32s numTh);
  void PutIntraMacroBlock(Ipp32s numTh, Ipp32s k, const Ipp8u *BlockSrc[3], Ipp8u *BlockRec[3], Ipp32s *dc_dct_pred);
  void PutMV_FRAME(Ipp32s numTh, Ipp32s k, MpegMotionVector2 *vector, Ipp32s motion_type);
  void PutMV_FIELD(Ipp32s numTh, Ipp32s k, MpegMotionVector2 *vector, MpegMotionVector2 *vector2, Ipp32s motion_type);

  Status PutPicture();
  void NonIntraMBCoeffs(Ipp32s numTh, MB_prediction_info *prediction_info, Ipp32s *Count, Ipp32s *pCodedBlockPattern);

  /////Motion estimation
  Ipp32s MotionEstimation_Frame(ME_PARAMS);
  Ipp32s MotionEstimation_Field(ME_PARAMS);
  Ipp32s MotionEstimation_FieldPict(ME_PARAMS);
  void AdjustSearchRange(Ipp32s B_count, Ipp32s direction);

  /// debug functions
#ifdef MPEG2_DEBUG_CODE
  void DumpPSNR();
  void save_bmp(vm_char *file_name, Ipp32s flag);
#endif /* MPEG2_DEBUG_CODE */

  /////////////////////////////////////////////////////////////////////
  MPEG2EncoderParams encodeInfo;
  //ColorFormat        m_ColorFormat;
  VideoProcessing* frame_loader;

  class VideoDataBuffer : public VideoData
  {
    public:
      DYNAMIC_CAST_DECL(VideoDataBuffer, VideoData)

      VideoDataBuffer();
      ~VideoDataBuffer();
      Status Alloc();
      Status Lock();
      Status Unlock();
      Status SetAllocator(MemoryAllocator *);
      Status SetMemSize(Ipp32s Ysize, Ipp32s UVsize);
    protected:
      MemID m_mid;
      MemoryAllocator *m_pMemoryAllocator; // pointer to memory allocator
      Ipp32s m_bLocked;
  };

  // lock unlock buffers from external memory allocator
  Status      LockBuffers();    // Lock buffers
  Status      UnlockBuffers();  // Unlock buffers

  class Input {
  public:
    // order: rec0,[orig0,]rec1,[orig1,][B_0,B1,...,B_ipdist_minus_1],aux
    VideoDataBuffer*  buf_ref[2]; // refs to frame pointers [fwd/bwd], swap [0] and [1] to rotate
                                   // to access original: buf_ref[fwd/bwd][1].
    VideoDataBuffer*  buf_B;       // buf_B[B_count-1] is processed
    VideoDataBuffer*  buf_aux; // to be used if format mismatched and original frames are not stored
    VideoData*        curenc; // frame currently being encoded

    Input() { frame_buffer = 0; frame_buffer_size=0; }
    Ipp32s           frame_buffer_size; // number of allocated frames in buffer
    VideoDataBuffer* frame_buffer;      // all reference frames pointers array
  } frames;
  Status CreateFrameBuffer();
  Status DeleteFrameBuffer();
  Status RotateReferenceFrames();
  Status TakeNextFrame(VideoData *in);
  Status FrameDone(VideoData *in);
  Status PrepareFrame(); // internal pointers from IO
  Status ComputeTS(VideoData *in, MediaData *out);

  Status SelectPictureType(VideoData *in);
  Ipp32s FormatMismatch(VideoData *in);
  Status LoadToBuffer(VideoDataBuffer* frame, VideoData *in);

  // for [topfield/bottomfield][forward/backward] ME; points to refs in frames_buff
  Ipp8u       *YRefFrame[2][2];   // [top/bottom][fwd/bwd]
  Ipp8u       *URefFrame[2][2];
  Ipp8u       *VRefFrame[2][2];
  Ipp8u       *YRecFrame[2][2];   // [top/bottom][fwd/bwd]
  Ipp8u       *URecFrame[2][2];
  Ipp8u       *VRecFrame[2][2];

  // Input frame; point to tmpFrame_buf or frames_buff
  Ipp8u       *Y_src;
  Ipp8u       *U_src;
  Ipp8u       *V_src;

  Ipp32s      MBcountH;
  Ipp32s      MBcountV;
  Ipp32s      MBcount;

  // Aligned sizes of all frames
  Ipp32s      YFrameHSize;
  Ipp32s      UVFrameHSize;
  Ipp32s      YFrameVSize;
  Ipp32s      UVFrameVSize;
  Ipp32s      YFrameSize;
  Ipp32s      UVFrameSize;
  Ipp32s      YUVFrameSize;

  Ipp32s      aspectRatio_code;      // 1-square,4:3,16:9,2.21:1
  Ipp32s      frame_rate_code;       // code for values, specified in standard
  Ipp32s      frame_rate_extension_n;// 2 bits
  Ipp32s      frame_rate_extension_d;// 5 bits
  Ipp32s      picture_structure;     // picture structure (frame, top / bottom field)
  Ipp32s      top_field_first;       // display top field first
  Ipp32s      repeat_first_field;    // increased display time
  Ipp32s      ipflag;                // P-field instead of second I-field (ref only opposite fld)
  Ipp32s      temporal_reference;    // display order in the GOP
  Ipp32s      closed_gop;            // doesn't refer to previous, i.e. first

  // params for each type of frame
  MotionData  *pMotionData;          // motion estimation ranges for P, B0, B1, ...
  Ipp32s      MotionDataCount;       // count of pMotionData array elements
  const Ipp32s (*mp_f_code)[2];      // points to forward[x,y], backward[x,y]

  Ipp32s      curr_frame_dct;        // when 0 dct_type have to be found
  Ipp32s      curr_frame_pred;       // is frame_pred_frame_dct, when 0 allows 16x8 modes
  Ipp32s      curr_intra_vlc_format;
  Ipp32s      curr_scan;
  Ipp32s      intra_dc_precision;
  Ipp32s      vbv_delay;             // vbv_delay code for picture header
  Ipp32s      rc_vbv_max;            // max allowed size of the frame to be encoded in bits
  Ipp32s      rc_vbv_min;            // min allowed size of the frame to avoid overflow with next frame
  Ipp64f      rc_vbv_fullness;       // buffer fullness before frame removing in bits
  Ipp64f      rc_delay;              // vbv examination delay for current picture
  Ipp64f      rc_ip_delay;           // extra delay for I or P pictures in bits
  Ipp64f      rc_ave_frame_bits;     // bits per frame
  Ipp32s      qscale[3];             // qscale codes for 3 frame types (Ipp32s!)
  Ipp32s      prsize[3];             // bitsize of previous frame of the type
  Ipp32s      prqscale[3];           // quant scale value, used with previous frame of the type
  Ipp32s      quantiser_scale_value; // for the current frame
  Ipp32s      q_scale_type;          // 0 for linear 1 for nonlinear
  Ipp32s      quantiser_scale_code;  // bitcode for current scale value
  bool        bQuantiserChanged;     // recalculate picture with new quantizer
  bool        bSceneChanged;         // recalculate P picture to I
  bool        bExtendGOP;            // recalculate I picture to P
  Ipp64f      rc_weight[3];          // frame weight (length proportion)
  Ipp64f      rc_tagsize[3];         // bitsize target of the type
  Ipp64f      rc_dev;                // bitrate deviation (sum of GOP's frame diffs)

  Ipp32s      varThreshold;          // threshold for block variance
  Ipp32s      meanThreshold;         // threshold for block mean
  Ipp32s      sadThreshold;         // threshold for block mean

  Ipp32s      PictureRateControl(Ipp64s bits_in_headers);  // selects curr scale
  Ipp32s      PostPictureRateControl(Ipp64s bits_encoded); // adaptation here
  Ipp32s      changeQuant(Ipp32s quant_value);             // changes all related
  Ipp32s      mapQuant(Ipp32s quant_value);                // returns mapped value

  Status      SetFrameRate(Ipp64f new_fr, Ipp32s is_mpeg1 = 0); // sets framerate code and extensions
  Status      SetAspectRatio(Ipp32s hor, Ipp32s ver); // sets aspect code from h/v value
  Ipp32s      onlyIFrames;   // true if no B-frames
  Ipp32s      B_count;       // index of B frame
  Ipp32s      P_distance;    // current distance, equal to # of B in buffer or delay
  Ipp32s      GOP_count;     // index of frame in GOP (enc order)
  Ipp32s      m_GOP_Start;   // first frame of current GOP
  Ipp32s      m_GOP_Start_tmp; // first frame of current GOP (for restore)
  Ipp32s      m_FirstFrame;  // true if numEncodedFrames==0
  Ipp64f      m_PTS;
  Ipp64f      m_DTS;

  Ipp32s      block_count;   // 6 or 8 or 12 depending on chroma format

  Ipp16s*     IntraQMatrix;
  Ipp16s*     NonIntraQMatrix;
  VM_ALIGN16_DECL(Ipp32f) _InvIntraQMatrix[64];
  VM_ALIGN16_DECL(Ipp32f) _InvNonIntraQMatrix[64];
  Ipp32f*     InvIntraQMatrix;
  Ipp32f*     InvNonIntraQMatrix;

  Ipp32s      m_Inited;
  MBInfo      *pMBInfo;    // macroblock side information array

  // threads
  Ipp32s      threadsAllocated; // but used threads is encodeInfo.numThreads
  threadSpecificData *threadSpec;
  threadInfo  **threads;

  // Variable Length Coding tables
  IppVCHuffmanSpec_32s *vlcTableB5c_e;
  IppVCHuffmanSpec_32s *vlcTableB15;

  // Output bitstream
  Ipp8u  *out_pointer;
  Ipp32s output_buffer_size;
  Ipp32s mEncodedSize;
  Ipp32s thread_buffer_size;

  // Offsets
  Ipp32s block_offset_frm[12];
  Ipp32s block_offset_fld[12];
  Ipp32s frm_dct_step[12];
  Ipp32s fld_dct_step[12];
  Ipp32s frm_diff_off[12];
  Ipp32s fld_diff_off[12];

  // GetDiff & MotionCompensation functions
  functype_getdiff
    func_getdiff_frame_c, func_getdiff_field_c;
  functype_getdiffB
    func_getdiffB_frame_c, func_getdiffB_field_c;
  functype_mc
    func_mc_frame_c, func_mc_field_c;

  // Internal var's
  Ipp32s BlkWidth_c;
  Ipp32s BlkStride_c;
  Ipp32s BlkHeight_c;
  Ipp32s chroma_fld_flag;
  Ipp32s curr_field;      // 0 - top or frame coded, 1 - bottom
  Ipp32s second_field;    // set, but not used yet

  const IppVCHuffmanSpec_32u *DC_Tbl[3];

  Ipp64f cpu_freq;
  Ipp64f motion_estimation_time;

  // const tables
  static const Ipp32s color_index[12];
  static const VLCode_8u CBP_VLC_Tbl[64];
  static const VLCode_8u AddrIncrementTbl[35];
  static const VLCode_8u mbtypetab[3][32];
  static const VLCode_8u MV_VLC_Tbl[33];
  static const IppVCHuffmanSpec_32u Y_DC_Tbl[12];
  static const IppVCHuffmanSpec_32u Cr_DC_Tbl[12];
  static const Ipp32s ResetTbl[4];
  static const Ipp32s dct_coeff_next_RL[];
  static const Ipp32s Table15[];
  static const IppiPoint MV_ZERO;
};


} // namespace UMC

#endif // __UMC_MPEG2_ENC_H
