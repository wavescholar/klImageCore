/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP4_PARSER_H__
#define __UMC_MP4_PARSER_H__

#include "vm_types.h"
#include "umc_data_reader.h"
#include "umc_bitstream_reader.h"

/*******************DECLARATIONS TYPES********************/

using namespace UMC;


#define MAXTRACKS 1024

struct T_atom_mp4
{
  Ipp64u          start;          /* byte start in file */
  Ipp64u          end;            /* byte endpoint in file */
  Ipp64u          size;           /* number of byte in this box */
  bool            is_large_size;  /* true if actual size is in largesize field, false otherwise */
  char            type[4];
};

struct T_matrix_data
{
  Ipp32f          values[9];
};

struct T_mvhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp64u          creation_time;
  Ipp64u          modification_time;
  Ipp32u          time_scale;
  Ipp64u          duration;
  Ipp32f          preferred_rate;
  Ipp32f          preferred_volume;
  char            reserved[10];
  T_matrix_data   matrix;
  long            preview_time;
  long            preview_duration;
  long            poster_time;
  long            selection_time;
  long            selection_duration;
  long            current_time;
  Ipp32u          next_track_id;
};

struct T_iods_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp16u          objectDescriptorID;
  Ipp8u           OD_profileAndLevel;
  Ipp8u           scene_profileAndLevel;
  Ipp8u           audioProfileId;
  Ipp8u           videoProfileId;
  Ipp8u           graphics_profileAndLevel;
};

struct T_tkhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp64u          creation_time;
  Ipp64u          modification_time;
  Ipp32u          track_id;
  Ipp32u          reserved1;
  Ipp64u          duration;
  Ipp32u          reserved2[2];
  Ipp32s          layer;
  Ipp32s          alternate_group;
  Ipp32f          volume;
  Ipp32u          reserved3;
  T_matrix_data   matrix;
  Ipp32f          track_width;
  Ipp32f          track_height;
  Ipp32s          is_video;
  Ipp32s          is_audio;
  Ipp32s          is_hint;
};

struct T_mdhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp64u          creation_time;
  Ipp64u          modification_time;
  Ipp32u          time_scale;
  Ipp64u          duration;
  Ipp16u          language;
  Ipp16u          quality;
};

struct T_vmhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32s          graphics_mode;
  Ipp32s          opcolor[3];
};

struct T_smhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32s          balance;
  Ipp32s          reserved;
};

/* hint media handler */
struct T_hmhd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp16u          maxPDUsize;
  Ipp16u          avgPDUsize;
  Ipp32u          maxbitrate;
  Ipp32u          avgbitrate;
  Ipp32u          slidingavgbitrate;
};

struct T_esds_data
{
  Ipp64u          start;
  Ipp8u           version;
  Ipp32s          flags;
  Ipp32u          decoderConfigLen;
  Ipp8u           *decoderConfig;

  Ipp8u           objectTypeID;
  Ipp8u           streamType;
  Ipp32u          bufferSizeDB;
  Ipp32u          maxBitrate;
  Ipp32u          avgBitrate;

  Ipp16u          es_ID;
  Ipp16u          ocr_ID;
  Ipp8u           streamDependenceFlag;
  Ipp8u           urlflag;
  Ipp8u           ocrflag;
  Ipp8u           streamPriority;
  Ipp8u           *URLString;
};

struct T_vwid_v
{
    Ipp16u view_id;
    Ipp16u view_order_index;
    Ipp8u  base_view_type;
    Ipp16u num_ref_views;
    Ipp16u *ref_view_id;
};

struct T_vwid_data
{
    Ipp8u     version;
    Ipp32u    flags;
    Ipp8u     min_temporal_id;
    Ipp8u     max_temporal_id;
    Ipp16u    num_views;
    T_vwid_v *views;
};

struct T_damr_data
{
  Ipp32s          decoderConfigLen;
  Ipp8u           *decoderConfig;
};

struct T_xvcC_data
{
  Ipp32s          decoderConfigLen;
  Ipp8u           *decoderConfig;
};

struct T_stsd_table_data
{
  char            format[4];
  Ipp8u           reserved[6];
  Ipp16u          data_reference;
/* common to audio and video */
  Ipp32s          version;
  Ipp32s          revision;
  char            vendor[4];
/* video description */
  Ipp32u          temporal_quality;
  Ipp32u          spatial_quality;
  Ipp16u          width;
  Ipp16u          height;
  Ipp32u          dpi_horizontal;
  Ipp32u          dpi_vertical;
  Ipp32u          data_size;
  Ipp32s          frames_per_sample;
  char            compressor_name[32];
  Ipp32s          depth;
  Ipp32s          ctab_id;
  Ipp32f          gamma;
  Ipp32s          fields;    /* 0, 1, or 2 */
  Ipp32s          field_dominance;   /* 0 - unknown     1 - top first     2 - bottom first */
/* audio description */
  Ipp16u          channels;
  Ipp16u          sample_size;
  Ipp32s          compression_id;
  Ipp32s          packet_size;
  Ipp32u          sample_rate;
  Ipp32u          is_protected;
/* hint description */
  Ipp32s          maxPktSize;
/* MP4 elementary stream descriptor */
  T_esds_data     esds;
/* ARM elementary stream descriptor */
  T_damr_data     damr;
/* avc elementary stream descriptor */
  T_xvcC_data     avcC;
/* svc elementary stream descriptor */
  T_xvcC_data     svcC;
/* mvc elementary stream descriptor */
  T_xvcC_data     mvcC;
  T_vwid_data     vwid;
};

struct T_stsd_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
  T_stsd_table_data *table;
};

struct T_stts_table_data
{
  Ipp32u          sample_count;
  Ipp32u          sample_duration;
};

struct T_stts_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
/////    long            entries_allocated;
  T_stts_table_data *table;
};

/* sync sample */
struct T_stss_table_data
{
  Ipp32u          sample_number;
};

struct T_stss_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
/////    long entries_allocated;
  T_stss_table_data *table;
};

/* sample to chunk */
struct T_stsc_table_data
{
  Ipp32u          chunk;
  Ipp32u          samples;
  Ipp32u          id;
  Ipp32u          first_sample;
};

struct T_stsc_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
/////    Ipp32u     entries_allocated;
  T_stsc_table_data *table;
};

/* sample size */
struct T_stsz_table_data
{
  Ipp32u          size;
};

struct T_stsz_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          sample_size;
  Ipp32u          total_entries;
  Ipp32u          max_sample_size;
  T_stsz_table_data *table;
};

/* chunk offset */
struct T_stco_table_data
{
  Ipp32u          offset;
};

struct T_stco_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
/////    long            entries_allocated;
  T_stco_table_data *table;
};

struct T_co64_table_data
{
  Ipp64u          offset;
};

struct T_co64_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
  T_co64_table_data *table;
};

/* composition time to sample */
struct T_ctts_table_data
{
  Ipp32u          sample_count;
  Ipp32u          sample_offset;
};

struct T_ctts_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
/////    long            entries_allocated;
  T_ctts_table_data *table;
};

struct T_sbgp_e
{
    Ipp32u sample_count;
    Ipp32u group_description_index;
};

struct T_sbgp_data
{
    Ipp8u     version;
    Ipp32u    flags;
    Ipp32u    grouping_type;
    Ipp32u    entry_count;
    T_sbgp_e *entries;
};

struct T_sgpd_e
{
    Ipp8u handler_type[4];
};

struct T_sgpd_data
{
    Ipp8u     version;
    Ipp32u    flags;
    Ipp32u    grouping_type;
    Ipp32u    entry_count;
    T_sgpd_e *entries;
};

/* sample table */
struct T_stbl_data
{
  Ipp32s          version;
  Ipp32u          flags;
  T_stsd_data     stsd;
  T_stts_data     stts;
  T_stss_data     stss;
  T_stsc_data     stsc;
  T_stsz_data     stsz;
  T_stco_data     stco;
  T_co64_data     co64;
  T_ctts_data     ctts;
  T_sbgp_data     sbgp;
  T_sgpd_data     sgpd;
};

/* handler reference */
struct T_hdlr_data
{
  Ipp8u           version;
  Ipp32u          flags;
  char            component_type[4];
  char            component_subtype[4];
  Ipp32u          component_manufacturer;
  Ipp32u          component_flags;
  Ipp32u          component_flag_mask;
  char            component_name[256];
};

/* data reference */
struct T_dref_table_data
{
  Ipp32u          size;
  char            type[4];
  Ipp8u           version;
  Ipp32u          flags;
  char *          data_reference;
};

struct T_dref_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          total_entries;
  T_dref_table_data *table;
};

/* data information */
struct T_dinf_data
{
  T_dref_data     dref;
};

/* SVC/MVC information */
struct T_buff_op
{
    Ipp32u byte_rate;
    Ipp32u cpb_size;
    Ipp32u dpb_size;
    Ipp32u init_cpb_delay;
    Ipp32u init_dpb_delay;
};

struct T_buff_data
{
    Ipp16u     operating_point_count;
    T_buff_op *operating_points;
};

struct T_tibr_data
{
    Ipp32u baseBitRate;
    Ipp32u maxBitRate;
    Ipp32u avgBitRate;
    Ipp32u tierBaseBitRate;
    Ipp32u tierMaxBitRate;
    Ipp32u tierAvgBitRate;
};

struct T_tiri_data
{
    Ipp16u tierID;
    Ipp8u  profileIndication;
    Ipp8u  profile_compatibility;
    Ipp8u  levelIndication;
    Ipp16u visualWidth;
    Ipp16u visualHeight;
    Ipp8u  discardable;
    Ipp8u  constantFrameRate;
    Ipp16u frameRate;
};

struct T_vwdi_data
{
    Ipp8u max_disparity;
};

struct T_mvra_ca
{
    Ipp32u common_attribute;
    Ipp32u common_value;
};

struct T_mvra_data
{
    Ipp8u      version;
    Ipp32u     flags;
    Ipp16u     num_common_attributes;
    T_mvra_ca *common_attributes;
    Ipp16u     num_differentiating_attributes;
    Ipp32u    *differentiating_attributes;
};

struct T_mvcg_e
{
    Ipp8u  entry_type;
    Ipp32u track_id;
    Ipp16u tier_id;
    Ipp16u output_view_id;
    Ipp16u start_view_id;
    Ipp16u view_count;
};

struct T_mvcg_data
{
    Ipp8u        version;
    Ipp32u       flags;
    Ipp32u       multiview_group_id;
    Ipp16u       num_entries;
    Ipp8u        entry_type;
    T_mvcg_e    *entries;
    T_tiri_data  tiri;
    T_mvra_data  mvra;
    T_tibr_data  tibr;
    T_buff_data  buff;
    T_vwdi_data  vwdi;
};

struct T_swtc_data
{
    Ipp8u        version;
    Ipp32u       flags;
    Ipp32u       num_entries;
    Ipp32u      *multiview_group_id;
    T_mvra_data  mvra;
};

struct T_mvci_data
{
    Ipp8u       version;
    Ipp32u      flags;
    T_mvcg_data mvcg;
    T_swtc_data swtc;
};

struct T_minf_data
{
  Ipp32s          is_video;
  Ipp32s          is_audio;
  Ipp32s          is_hint;
  T_vmhd_data     vmhd;
  T_smhd_data     smhd;
  T_hmhd_data     hmhd;
  T_stbl_data     stbl;
  T_hdlr_data     hdlr;
  T_dinf_data     dinf;
  T_mvci_data     mvci;
};

struct T_mdia_data
{
  T_mdhd_data     mdhd;
  T_minf_data     minf;
  T_hdlr_data     hdlr;
};

/*
struct T_elst_table_data
{
  Ipp32u          duration;
  Ipp32s          time;
  Ipp32f          rate;
};

struct T_elst_data
{
  Ipp32s          version;
  Ipp32u          flags;
  Ipp32u          total_entries;
  T_elst_table_data *table;
};

struct T_edts_data
{
  T_elst_data     elst;
};
*/

struct T_hint_data
{
  Ipp32s          numTracks;
  Ipp32u          trackIds[MAXTRACKS];
  void            *traks[MAXTRACKS];
};

struct T_dpnd_data
{
  Ipp32u          idTrak;
};

struct T_tref_data
{
  T_dpnd_data     dpnd;
  T_hint_data     hint;
};

struct T_trak_data
{
  T_tkhd_data     tkhd;
  T_mdia_data     mdia;
//  T_edts_data     edts;
  T_tref_data     tref;
};

struct T_udta_data
{
  char           *copyright;
  Ipp32s          copyright_len;
  char            *name;
  Ipp32s          name_len;
  char           *info;
  Ipp32s          info_len;
};

struct T_trex_data
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          track_ID;
  Ipp32u          default_sample_description_index;
  Ipp32u          default_sample_duration;
  Ipp32u          default_sample_size;
  Ipp32u          default_sample_flags;
};

struct T_mvex_data
{
  Ipp32u          size_atom;
  Ipp32u          total_tracks;
  T_trex_data    *trex[MAXTRACKS];
};

struct T_moov
{
  Ipp32u          total_tracks;
  T_mvhd_data     mvhd;
  T_iods_data     iods;
  T_trak_data    *trak[MAXTRACKS];
  T_udta_data     udta;
  T_mvex_data     mvex;
};

struct T_trun_table_data
{
  Ipp32u          sample_duration;
  Ipp32u          sample_size;
  Ipp32u          sample_flags;
  Ipp32u          sample_composition_time_offset;
};

struct T_mfhd
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          sequence_number;
};

struct T_trun
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          sample_count;
  Ipp32s          data_offset;
  Ipp32u          first_sample_flags;
  T_trun_table_data *table_trun;
  Ipp64f          stream_end;
};

struct T_tfhd
{
  Ipp8u           version;
  Ipp32u          flags;
  Ipp32u          track_ID;
  Ipp32u          base_data_offset;
  Ipp32u          sample_description_index;
  Ipp32u          default_sample_duration;
  Ipp32u          default_sample_size;
  Ipp32u          default_sample_flags;
};

struct T_traf
{
  Ipp32u          entry_count;
  Ipp32u          max_truns;
  T_tfhd          tfhd;
  T_trun          *trun[MAXTRACKS];
};

struct T_moof
{
  Ipp32u          size_atom;
  Ipp32u          total_tracks;
  Ipp64u          end; /* byte endpoint in file */
  T_mfhd          mfhd;
  T_traf          *traf[MAXTRACKS];
};

struct info_atoms
{
  T_moov          moov;
  T_moof          moof;
  T_atom_mp4      data;
  T_atom_mp4      esds;
  bool            is_fragment;
};

// track run flags
#define DATA_OFFSET_PRESENT                         0x000001
#define FIRST_SAMPLE_FLAGS_PRESENT                  0x000004
#define SAMPLE_DURATION_PRESENT                     0x000100
#define SAMPLE_SIZE_PRESENT                         0x000200
#define SAMPLE_FLAGS_PRESENT                        0x000400
#define SAMPLE_COMPOSITION_TIME_OFFSETS_PRESENT     0x000800

// track fragment header flags
#define BASE_DATA_OFFSET_PRESENT                    0x000001
#define SAMPLE_DESCRIPTION_INDEX_PRESENT            0x000002
#define DEFAULT_SAMPLE_DURATION_PRESENT             0x000008
#define DEFAULT_SAMPLE_SIZE_PRESENT                 0x000010
#define DEFAULT_SAMPLE_FLAGS_PRESENT                0x000020
#define DURATION_IS_EMPTY                           0x010000


#endif
