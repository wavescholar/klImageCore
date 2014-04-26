/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP4_MUX_DEFS_H__
#define __UMC_MP4_MUX_DEFS_H__

#include "umc_mp4_parser_w.h"


namespace UMC
{

#define IS_VIDEO(index) (m_pTrackParams[index].type == VIDEO_TRACK)
#define IS_AUDIO(index) (m_pTrackParams[index].type == AUDIO_TRACK)

#define NAL_UNITTYPE_BITS       0x1f

//  NAL Unit Types
#ifndef SHARED_ENCDECBS_STRUCTURES_DEFS
#define SHARED_ENCDECBS_STRUCTURES_DEFS

typedef enum {
    NAL_UT_RESERVED  = 0x00, // Reserved
    NAL_UT_SLICE     = 0x01, // Coded Slice - slice_layer_no_partioning_rbsp
    NAL_UT_DPA       = 0x02, // Coded Data partition A - dpa_layer_rbsp
    NAL_UT_DPB       = 0x03, // Coded Data partition A - dpa_layer_rbsp
    NAL_UT_DPC       = 0x04, // Coded Data partition A - dpa_layer_rbsp
    NAL_UT_IDR_SLICE = 0x05, // Coded Slice of a IDR Picture - slice_layer_no_partioning_rbsp
    NAL_UT_SEI       = 0x06, // Supplemental Enhancement Information - sei_rbsp
    NAL_UT_SPS       = 0x07, // Sequence Parameter Set - seq_parameter_set_rbsp
    NAL_UT_PPS       = 0x08, // Picture Parameter Set - pic_parameter_set_rbsp
    NAL_UT_PD        = 0x09, // Picture Delimiter - pic_delimiter_rbsp
    NAL_UT_FD        = 0x0a, // Filler Data - filler_data_rbsp
    NAL_UT_SPS_EX    = 0x0d,  // Sequence Parameter Set Extension - seq_parameter_set_extension_rbsp
    NAL_UT_AUXILIARY = 0x13  // Auxiliary coded picture
} NAL_Unit_Type;
#endif/* SHARED_ENCDECBS_STRUCTURES_DEFS */

typedef enum
{
  UMC_NO_SUCH_CODE = 1,
  UMC_HEADER = 2,
  UMC_DATA = 4,
} MP4Muxer_StartCode;

#define BYTE_BIT    8

#define INITIAL_TRACK_SIZE 10000

#define MAX_MOOV_SIZE 1000000
#define MAX_MOOF_SIZE 200000

#define MAX_TIME 86400; /* seconds in twenty-four hours */

#define _SAFE_NEW_n(p, a, n) {if (NULL == p) p = _NEW_n(a, n);}
#define _NEW_n(a, n) (a*) malloc ((n) * sizeof(a))
#define _NEW(a) _NEW_n(a, 1)

#define _FWRITE(a, file)  vm_file_fwrite(a.GetDataPointer(), sizeof(Ipp8s), a.GetDataSize(), file)
//*****#define _FWRITE(a, file)  fwrite(a.GetBufferPointer(), sizeof(Ipp8s), a.GetBufferSize(), file)
//*****#define _FWRITE_P(a, file)  fwrite(a->GetBufferPointer(), sizeof(Ipp8s), a->GetBufferSize(), file)

#define _SAFE_DELETE_ARRAY(a) if (a) { free(a); a = NULL; }
#define _SAFE_DELETE_CPP(a) if (a) { delete a; a = NULL; }
#define _SAFE_DELETE_CPP_ARRAY(a) if (a) { delete [] a; a = NULL; }
#define _SAFE_DELETE(a) if (a) { free(a); a = NULL; }

#define _SEEK_FILE_START(a) { if (0 != vm_file_fseek(a, 0, VM_FILE_SEEK_SET)) return UMC_ERR_FAILED; }

struct TM_table_esds{
    TM_esds_data         esds;
};

struct TM_table_stsz{
    TM_stsz_data         stsz;
};

struct TM_table_stco{
    TM_stco_data         stco;
};

struct TM_table_stsc{
    TM_stsc_data         stsc;
};

// meta information about sample
struct sMuxSample
{
    //sMuxSample();

    Ipp64u    m_nPosition;        // absolute sample offset in the file
    Ipp32u    m_nTimeStamp;       // time stamp in TimeScale
    Ipp32u    m_nDuration;        // duration in TimeScale
    Ipp32s    m_nSize;            // sample size in byte
    UMC::FrameType  m_frameTMype;        // NO_PICTMURE for audio. For video sample, initial value is
    // NO_PICTURE, but it may be set to I, P, B Picture
    // in time seeking or trick mode
    Ipp32u    m_nFirstChunk;      //is an integer that gives the index of the first chunk in this run of
    //chunks that share the same samples-per-chunk and sample-description-index;
    Ipp32u    m_nSamplesPerChunk; //is an integer that gives the number of samples in each of chunks
    Ipp32u    m_nId;              //is an integer that gives the index of the sample entry that describes the
    //samples in this chunk. TMhe index ranges from 1 to the number of sample
    //entries in the Sample Description Box
};

struct sMuxTMrack
{
    sMuxSample*   m_pSamples;
    Ipp32s        m_nSttsEntries;        // number of stts entries
    Ipp32s        m_nCttsEntries;        // number of ctts entries : 0 - ctts in unneeded
    Ipp32s        m_ctts_delta;
    Ipp32u        m_nSamplesCount;
    Ipp64u        m_nTrackSize;          //size of the track (in sMuxSample(s))
    Ipp64f        m_nTrackEnd;
    Ipp32s        m_nTrackStatus;        // 0 - muxing is in progress; 1 - track is muxed
    bool          m_bSortPTSOnExit;      // to calculate frames durations in case of B frames
};


struct sNal_ut_sps
{
    Ipp8s*    m_pNal_ut_sps;
    Ipp32u    m_nLength;
};

struct sNal_ut_pps
{
    Ipp8s*    m_pNal_ut_pps;
    Ipp32u    m_nLength;
};

struct sH264_Header
{
    sNal_ut_sps* m_pNal_ut_sps;
    sNal_ut_pps* m_pNal_ut_pps;
    Ipp32u m_nSps_num;
    Ipp32u m_nPps_num;
};

} // namespace UMC

#endif // __UMC_MP4_MUX_DEFS_H__
