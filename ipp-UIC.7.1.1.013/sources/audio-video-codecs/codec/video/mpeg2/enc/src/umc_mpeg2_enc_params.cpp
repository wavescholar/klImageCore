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

#include "umc_mpeg2_enc_defs.h"
#include "vm_strings.h"

using namespace UMC;

///* identifies valid profile/level combinations */
//static Ipp8u Profile_Level_Defined[5][4] =
//{
///* HL   H-14 ML   LL  */
//  {1,   1,   1,   0},  /* HP   */
//  {0,   1,   0,   0},  /* Spat */
//  {0,   0,   1,   1},  /* SNR  */
//  {1,   1,   1,   1},  /* MP   */
//  {0,   0,   1,   0}   /* SP   */
//};

static struct LevelLimits
{
  Ipp32s f_code[2];
  Ipp32s hor_size;
  Ipp32s vert_size;
  Ipp32s sample_rate;
  Ipp32s bit_rate;         /* Mbit/s */
  Ipp32s vbv_buffer_size;  /* 16384 bit steps */
} MaxValTbl[5] =
{
  {{9, 5}, 1920, 1152, 62668800, 80, 597}, /* HL */
  {{9, 5}, 1440, 1152, 47001600, 60, 448}, /* H-14 */
  {{8, 5},  720,  576, 10368000, 15, 112}, /* ML */
  {{7, 4},  352,  288,  3041280,  4,  29}, /* LL */
  {{9, 9}, 8192, 8192, 0x7fffffff, 1000,  10000}  /* some limitations for unlimited */
};

#define SP   5
#define MP   4
#define SNR  3
#define SPAT 2
#define HP   1

#define LL  10
#define ML   8
#define H14  6
#define HL   4

/* chroma_format for MPEG2EncoderParams::chroma_format*/
#define CHROMA420 1
#define CHROMA422 2
#define CHROMA444 3

/* ISO/IEC 13818-2, 6.3.11 */
/*  this pretty table was designed to
    avoid coincidences with any GPL code */
static VM_ALIGN16_DECL(Ipp16s) DefaultIntraQuantMatrix[64] =
{
     8,
     16,

     19,        22, 26, 27,             29, 34, 16,
     16,        22,         24,         27,         29,
     34,        37,             19,     22,             26,
     27,        29,             34,     34,             38,
     22,        22,             26,     27,             29,
     34,        37,             40,     22,             26,
     27,        29,         32,         35,         40,
     48,        26, 27, 29,             32, 35, 40,
     48,        58,                     26,
     27,        29,                     34,
     38,        46,                     56,
     69,        27,                     29,
     35,        38,                     46,
     56,        69,                     83
};

static const Ipp64f ratetab[8]=
    {24000.0/1001.0,24.0,25.0,30000.0/1001.0,30.0,50.0,60000.0/1001.0,60.0};
static const Ipp32s aspecttab[3][2] =
    {{4,3},{16,9},{221,100}};

MPEG2EncoderParams::MPEG2EncoderParams()
{
  Ipp32s i;

  *IntraQMatrix = 0;
  *NonIntraQMatrix = 0;

  lFlags = FLAG_VENC_REORDER;
  m_info.iBitrate = 5000000;
  m_info.fFramerate = 30;
  m_iFramesCounter = 0;
  m_iQuality = -1;

  m_info.videoInfo.m_picStructure = PS_PROGRESSIVE;          // progressive sequence
  //progressive_frame = 1; // progressive frame
  CustomIntraQMatrix = 0;
  CustomNonIntraQMatrix = 0;
  for(i=0; i<64; i++) {   // for reconstruction
    IntraQMatrix[i] = DefaultIntraQuantMatrix[i];
    NonIntraQMatrix[i] = 16;
  }
  IPDistance = 3;         // distance between key-frames
  gopSize = 15;           // size of GOP
  m_info.videoInfo.m_iSAWidth = 4;
  m_info.videoInfo.m_iSAHeight = 3;
  m_info.iProfile = MP;
  m_info.iLevel = ML;
  //repeat_first_field = 0;
  //top_field_first = 0;    // display top field first
  //intra_dc_precision = 0; // 8 bit
  FieldPicture = 0;       // field or frame picture (if progframe=> frame)
  VBV_BufferSize = 112;
  low_delay = 0;
  frame_pred_frame_dct[0] = 1;
  frame_pred_frame_dct[1] = 1;
  frame_pred_frame_dct[2] = 1;
  intraVLCFormat[0] = 1;
  intraVLCFormat[1] = 1;
  intraVLCFormat[2] = 1;
  altscan_tab[0] = 0;
  altscan_tab[1] = 0;
  altscan_tab[2] = 0;
  mpeg1 = 0;               // 1 - mpeg1 (unsupported), 0 - mpeg2
  *idStr = 0;              // user data to put to each sequence
  UserData = idStr;
  UserDataLen = 0;
  m_iThreads = 1;
  performance = 0;
  encode_time = 0;
  motion_estimation_perf = 0;
  me_alg_num = 3;
  me_auto_range = 1;
  allow_prediction16x8 = 0;
  rc_mode = RC_CBR;
  quant_vbr[0] = 0;
  quant_vbr[1] = 0;
  quant_vbr[2] = 0;

  rangeP[0] = 8*IPDistance;
  rangeP[1] = 4*IPDistance;
  rangeB[0][0] = rangeB[1][0] = rangeP[0] >> 1;
  rangeB[0][1] = rangeB[1][1] = rangeP[1] >> 1;

}

MPEG2EncoderParams::~MPEG2EncoderParams()
{
}

Status MPEG2EncoderParams::ReadQMatrices(DString sIntraQMatrixFName, DString sNonIntraQMatrixFName)
{
  Ipp32s i, temp;
  vm_file *InputFile;

  if(!sIntraQMatrixFName.Size())
  {
    // use default intra matrix
    CustomIntraQMatrix = 0;
    for(i=0; i<64; i++)
      IntraQMatrix[i] = DefaultIntraQuantMatrix[i];
  }
  else
  {
    // load custom intra matrix
    CustomIntraQMatrix = 1;
    if( 0 == (InputFile = vm_file_open((vm_char*)sIntraQMatrixFName,VM_STRING("rt"))) )
      return UMC_ERR_OPEN_FAILED;

    for(i=0; i<64; i++)
    {
      vm_file_fscanf( InputFile, VM_STRING("%d"), &temp );
      if( temp < 1 || temp > 255 )
      {
        vm_file_fclose( InputFile );
        return UMC_ERR_FAILED;
      }
      IntraQMatrix[i] = (Ipp16s)temp;
    }

    vm_file_fclose( InputFile );
  }

  if(!sNonIntraQMatrixFName.Size())
  {
    // use default non-intra matrix
    CustomNonIntraQMatrix = 0;
    for(i=0; i<64; i++)
      NonIntraQMatrix[i] = 16;
  }
  else
  {
    // load custom non-intra matrix
    CustomNonIntraQMatrix = 1;
    if( 0 == (InputFile = vm_file_open((vm_char*)sNonIntraQMatrixFName,VM_STRING("rt"))) )
      return UMC_ERR_OPEN_FAILED;

    for(i=0; i<64; i++)
    {
      vm_file_fscanf( InputFile, VM_STRING("%d"), &temp );
      if( temp < 1 || temp > 255 )
      {
        vm_file_fclose( InputFile );
        return UMC_ERR_FAILED;
      }
      NonIntraQMatrix[i] = (Ipp16s)temp;
    }

    vm_file_fclose( InputFile );
  }

  return UMC_OK;
}

Status MPEG2EncoderParams::Profile_and_Level_Checks()
{
  Ipp32s i,j,k;
  struct LevelLimits *MaxVal;
  Ipp32s newLevel = m_info.iLevel;
  Ipp32s newProfile = m_info.iProfile;
  Status ret = UMC_OK;

  if( m_info.iProfile == SNR || m_info.iProfile == SPAT )
    return UMC_ERR_FAILED;

  // check level, select appropriate when it is wrong
  if( newLevel < HL ||
    newLevel > LL ||
    newLevel & 1 )
    newLevel = LL;
  if(m_info.videoInfo.m_colorFormat != YUV420 && newLevel == LL )
    newLevel = ML;
  if( m_info.fFramerate > ratetab[5 - 1] && newLevel >= ML )
    newLevel = H14;

  MaxVal = &MaxValTbl[(newLevel - 4) >> 1];
  while( (Ipp32s)m_info.videoInfo.m_iWidth > MaxVal->hor_size ||
      (Ipp32s)m_info.videoInfo.m_iHeight > MaxVal->vert_size ||
      m_info.videoInfo.m_iWidth * m_info.videoInfo.m_iHeight * m_info.fFramerate > MaxVal->sample_rate ||
         m_info.iBitrate > 1000000 * (Ipp32u)MaxVal->bit_rate )
  {
    if(newLevel > HL) {
      newLevel -= 2;
      MaxVal --;
    } else {
      MaxVal = &MaxValTbl[4];
      break;
    }
  }

  // check profile values
  if( newProfile < HP || newProfile > SP ) {
    newProfile = MP;
  }
  if(m_info.videoInfo.m_colorFormat != YUV420) {
    newProfile = HP;
    if(m_info.videoInfo.m_colorFormat == YUV444) {
    //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("CHROMA444 has no appropriate prifile\n"));
    }
  }

  // check profile constraints
  if( newProfile == SP && IPDistance != 1 ) {
    newProfile = MP;
  }

  // check profile - level combination
  if(newProfile == SP && newLevel != ML)
    newProfile = MP;
  if(newProfile == HP && newLevel == LL) {
    newLevel = ML;
    MaxVal = &MaxValTbl[(newLevel - 4) >> 1];
  }

  //if( newProfile != HP && intra_dc_precision == 3 )
  //  newProfile = HP;

  //// SP, MP: constrained repeat_first_field
  //if( newProfile >= MP && repeat_first_field &&
  //    ( info.framerate <= ratetab[2 - 1] ||
  //      info.framerate <= ratetab[6 - 1] && info.interlace_type == PROGRESSIVE ) ) {
  //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("repeat_first_field set to zero\n"));
  //  repeat_first_field = 0;
  //}

  if(IPDistance < 1) IPDistance = 1;
  if(gopSize < 1)    gopSize = 1;
  if(gopSize > 132)  gopSize = 132;
  if(IPDistance > gopSize) IPDistance = gopSize;

  // compute f_codes from ranges, check limits, extend to the f_code limit
  for(i = 0; i < IPP_MIN(IPDistance,2); i++) // P, B
    for(k=0; k<2; k++) {                     // FW, BW
      Ipp32s* prange;
      if (i == 0) {
        if( k==1)
          continue; // no backward for P-frame
        prange = rangeP;
      } else {
        prange = rangeB[k];
      }
      for(j=0; j<2; j++) {                   // x, y
        Ipp32s req_f_code;
        RANGE_TO_F_CODE(prange[j], req_f_code);
        if( req_f_code > MaxVal->f_code[j] ) {
          req_f_code = MaxVal->f_code[j];
          prange[j] = 4<<req_f_code;
        }
        if (i==0) // extend range only for P
          prange[j] = 4<<req_f_code;
      }
    }

  // Table 8-13
  if( VBV_BufferSize > MaxVal->vbv_buffer_size )
    VBV_BufferSize = MaxVal->vbv_buffer_size;

  if(newLevel != m_info.iLevel)
    m_info.iLevel = newLevel;
  if(newProfile != m_info.iProfile)
    m_info.iProfile = newProfile;

  return ret;
}

Status MPEG2EncoderParams::RelationChecks()
{
  Status ret = UMC_OK;

  if( mpeg1 ) {
    if(m_info.videoInfo.m_picStructure != PS_PROGRESSIVE)
      m_info.videoInfo.m_picStructure = PS_PROGRESSIVE;

    mpeg1 = 0;
  }

  //if(aspectRatio < 1 || aspectRatio > 15) {
  //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("setting aspect ratio to 1\n"));
  //  aspectRatio = 1;
  //}

  //progressive_frame = (progressive_frame != 0) ? 1 : 0;
  //if( info.interlace_type == PROGRESSIVE && !progressive_frame )
  //{
  //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("setting progressive_frame = 1\n"));
  //  progressive_frame = 1;
  //}

  //repeat_first_field = (repeat_first_field != 0) ? 1 : 0;
  ////if( !progressive_frame && repeat_first_field )
  //if( info.interlace_type != PROGRESSIVE && repeat_first_field )
  //{
  //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("setting repeat_first_field = 0\n"));
  //  repeat_first_field = 0;
  //}

  frame_pred_frame_dct[0] = (frame_pred_frame_dct[0] != 0) ? 1 : 0;
  frame_pred_frame_dct[1] = (frame_pred_frame_dct[1] != 0) ? 1 : 0;
  frame_pred_frame_dct[2] = (frame_pred_frame_dct[2] != 0) ? 1 : 0;

  //if( progressive_frame )
  if( m_info.videoInfo.m_picStructure == PS_PROGRESSIVE )
  {
    if( FieldPicture )
      FieldPicture = 0;

    if( !frame_pred_frame_dct[0] )
      frame_pred_frame_dct[0] = 1;

    if( !frame_pred_frame_dct[1] )
      frame_pred_frame_dct[1] = 1;

    if( !frame_pred_frame_dct[2] )
      frame_pred_frame_dct[2] = 1;
  }

  //top_field_first = (top_field_first != 0) ? 1 : 0;
  //if (info.interlace_type == PROGRESSIVE && !repeat_first_field && top_field_first)
  //{
  //  vm_debug_trace(VM_DEBUG_WARNING, VM_STRING("setting top_field_first = 1\n"));
  //  top_field_first = 0;
  //}

  if(m_info.videoInfo.m_colorFormat < YUV420 || m_info.videoInfo.m_colorFormat > YUV444)
    m_info.videoInfo.m_colorFormat = YUV420;

  return ret;
}


Status MPEG2EncoderParams::ReadParams(ParserCfg *par)
{
    DString   sIntraQMatrixFName, sNonIntraQMatrixFName, sTemplateLogFile;
    Ipp32s    iTimeCode[4];
    Ipp32s    chroma_format_code, aspect_code;
    Ipp32s    prog_seq, top_field_first;
    Status    ret, ret2;
    Ipp32s    i;

    UMC_CHECK(par, UMC_ERR_NULL_PTR);
    if(!par->GetNumOfkeys())
        return UMC_ERR_NOT_INITIALIZED;

    par->GetParam(VM_STRING("sIntraQMatrixFName"),    &sIntraQMatrixFName,  1);
    par->GetParam(VM_STRING("sNonIntraQMatrixFName"), &sNonIntraQMatrixFName,  1);
    par->GetParam(VM_STRING("sTemplateLogFile"),      &sTemplateLogFile,  1);
    par->GetParam(VM_STRING("iStartTimeCode"),        &iTimeCode[0],  4);
    par->GetParam(VM_STRING("iGOPSize"),              &gopSize,  1);
    par->GetParam(VM_STRING("iIPDistance"),           &IPDistance,  1);
    par->GetParam(VM_STRING("bMPEG1Conformed"),       &mpeg1,  1);
    par->GetParam(VM_STRING("iFieldPicture"),         &FieldPicture,  1);
    par->GetParam(VM_STRING("iAspectCode"),           &aspect_code,  1);
    par->GetParam(VM_STRING("iVBVBufferSize"),        &VBV_BufferSize,  1);
    par->GetParam(VM_STRING("bLowDelay"),             &low_delay,  1);
    par->GetParam(VM_STRING("bConstrParms"),          &constrparms,  1);
    par->GetParam(VM_STRING("iProfile"),              &m_info.iProfile,  1);
    par->GetParam(VM_STRING("iLevel"),                &m_info.iLevel,  1);
    par->GetParam(VM_STRING("bProgressiveSeq"),       &prog_seq,  1);
    par->GetParam(VM_STRING("iChromaFormat"),         &chroma_format_code,  1);
    par->GetParam(VM_STRING("iVideoFormat"),          &video_format,  1);
    par->GetParam(VM_STRING("iColorPrimaries"),       &color_primaries,  1);
    par->GetParam(VM_STRING("iTransCharacteristics"), &transfer_characteristics,  1);
    par->GetParam(VM_STRING("iMatrixCoefficients"),   &matrix_coefficients,  1);
    par->GetParam(VM_STRING("iTopFieldFirst"),        &top_field_first,  1);
    par->GetParam(VM_STRING("iFramePredDCT"),         &frame_pred_frame_dct[0],  3);
    par->GetParam(VM_STRING("iConcealmentMVs"),       &conceal_tab[0],  3);
    par->GetParam(VM_STRING("iIntraVLCFormat"),       &intraVLCFormat[0],  3);
    par->GetParam(VM_STRING("iAlternateScan"),        &altscan_tab[0],  3);
    par->GetParam(VM_STRING("iSearchDistP"),          &rangeP[0],  2);
    par->GetParam(VM_STRING("iSearchDistFwdB"),       &rangeB[0][0],  2);
    par->GetParam(VM_STRING("iSearchDistBwdB"),       &rangeB[1][0],  2);

    if( gopSize < 1 )
        return UMC_ERR_FAILED;
    if( IPDistance < 1 )
        return UMC_ERR_FAILED;
    if( gopSize%IPDistance != 0 )
        return UMC_ERR_FAILED;
    if( IPDistance > 128 )
        return UMC_ERR_FAILED;

    switch(chroma_format_code)
    {
    case CHROMA420:
        m_info.videoInfo.m_colorFormat = YUV420; break;
    case CHROMA422:
        m_info.videoInfo.m_colorFormat = YUV422; break;
    case CHROMA444:
        m_info.videoInfo.m_colorFormat = YUV444; break;
    default:
        chroma_format_code = CHROMA420;
        m_info.videoInfo.m_colorFormat = YUV420;
    }

    for(i=0; i<3; i++) {
        frame_pred_frame_dct[i] = !!frame_pred_frame_dct[i];
        conceal_tab[i] = !!conceal_tab[i];
        intraVLCFormat[i] = !!intraVLCFormat[i];
        altscan_tab[i] = !!altscan_tab[i];
    }

    if(aspect_code < 1 || aspect_code > 4)
        aspect_code = 2;
    if(aspect_code == 1) {
        m_info.videoInfo.m_iSAWidth = m_info.videoInfo.m_iWidth;
        m_info.videoInfo.m_iSAHeight = m_info.videoInfo.m_iHeight;
    } else {
        m_info.videoInfo.m_iSAWidth = aspecttab[aspect_code-2][0];
        m_info.videoInfo.m_iSAHeight = aspecttab[aspect_code-2][1];
    }

    m_info.videoInfo.m_picStructure = (prog_seq ? PS_PROGRESSIVE : (top_field_first ? PS_TOP_FIELD_FIRST : PS_BOTTOM_FIELD_FIRST));

    // check parameters correctness
    ret = Profile_and_Level_Checks();
    if (ret < 0)
        return ret;

    ret2 = RelationChecks();
    if (ret2 < 0)
        return ret2;
    if (ret2 != UMC_OK)
        ret = ret2;

    ret2 = ReadQMatrices(sIntraQMatrixFName, sNonIntraQMatrixFName);
    if (ret2 < 0)
        return ret2;
    if (ret2 != UMC_OK)
        ret = ret2;

    return ret;
}

#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
