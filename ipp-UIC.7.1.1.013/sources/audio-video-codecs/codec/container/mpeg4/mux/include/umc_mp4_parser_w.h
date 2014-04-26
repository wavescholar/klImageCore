/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP4_PARSER_W_H__
#define __UMC_MP4_PARSER_W_H__

#include "vm_types.h"
#include "umc_data_writer.h"

using namespace UMC;

/*******************DECLARATIONS TYPES********************/

#define MAXTRACKS 1024

struct TM_atom_mp4
{
    Ipp64u    start;      /* byte start in file */
    Ipp64u    end;        /* byte endpoint in file */
    Ipp32u    size;       /* byte size for writing */
    Ipp8s     type[4];
};

struct TM_matrix_data
{
    Ipp32f values[9];
};

struct TM_mvhd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp64u creation_time;
    Ipp64u modification_time;
    Ipp32u nTimeScale;
    Ipp64u duration;
    Ipp32f preferred_rate;
    Ipp32f preferred_volume;
    Ipp8s reserved[10];
    TM_matrix_data matrix;
    Ipp32s preview_time;
    Ipp32s preview_duration;
    Ipp32s poster_time;
    Ipp32s selection_time;
    Ipp32s selection_duration;
    Ipp32s current_time;
    Ipp32u next_track_id;
};

struct TM_iods_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp16u objectDescriptorID;
    Ipp8u OD_profileAndLevel;
    Ipp8u scene_profileAndLevel;
    Ipp8u audioProfileId;
    Ipp8u videoProfileId;
    Ipp8u graphics_profileAndLevel;
};

struct TM_tkhd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp64u creation_time;
    Ipp64u modification_time;
    Ipp32u track_id;
    Ipp32s reserved1;
    Ipp64u duration;
    Ipp8s reserved2[8];
    Ipp32s layer;
    Ipp32s alternate_group;
    Ipp32f volume;
    Ipp32s reserved3;
    TM_matrix_data matrix;
    Ipp32f track_width;
    Ipp32f track_height;
    Ipp32s is_video;
    Ipp32s is_audio;
    Ipp32s is_hint;
};

struct TM_mdhd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp64u creation_time;
    Ipp64u modification_time;
    Ipp32u nTimeScale;
    Ipp64u duration;
    Ipp16u language;
    Ipp16u pre_defined;
};

struct TM_esds_data
{
    Ipp32u size_atom;
    Ipp64u start;
    Ipp8u version;
    Ipp32s flags;
    Ipp32s decoderConfigLen;
    Ipp8u* decoderConfig;

    Ipp32s objectTypeID;
    Ipp32s streamType;
    Ipp32s bufferSizeDB;
    Ipp32u maxBitrate;
    Ipp32u avgBitrate;

    Ipp16u es_ID;
    Ipp16u ocr_ID;
    Ipp8u  streamDependenceFlag;
    Ipp8u  urlflag;
    Ipp8u  ocrflag;
    Ipp8u  streamPriority;
    Ipp8u* URLString;
};

struct TM_stsd_table_data
{
    Ipp32u size_atom;
    Ipp8s format[4];
    Ipp8s reserved[6];
    Ipp16u data_reference;

    Ipp32u version;
    Ipp32u revision;
    Ipp8s vendor[4];

    Ipp32s temporal_quality;
    Ipp32s spatial_quality;
    Ipp16u width;
    Ipp16u height;
    Ipp32u dpi_horizontal;
    Ipp32u dpi_vertical;
    Ipp32s data_size;
//    Ipp32s frames_per_sample;
    Ipp8s compressor_name[32];
    Ipp32s depth;
    Ipp32s ctab_id;
    Ipp32f gamma;
    Ipp32s fields;
    Ipp32s field_dominance;

    Ipp16u channels;
    Ipp16u sample_size;
    Ipp32s compression_id;
    Ipp32s packet_size;
    Ipp32u sample_rate;

    Ipp32s maxPktSize;

    TM_esds_data esds;
};

struct TM_stsd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    TM_stsd_table_data *table;
};

struct TM_stts_table_data
{
    Ipp32u sample_count;
    Ipp32u sample_duration;
};

struct TM_trun_table_data
{
    Ipp32u sample_count;
    Ipp32u sample_duration;
    Ipp32u sample_size;
    Ipp32u sample_flags;
    Ipp32u sample_ct_offset;
};

struct TM_stts_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    Ipp32s entries_allocated;
    TM_stts_table_data *table;
};

struct TM_stss_table_data
{
    Ipp32s sample;
};

struct TM_stss_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32u flags;
    Ipp32u total_entries;
    Ipp32u entries_allocated;
    TM_stss_table_data *table;
};

struct TM_stsc_table_data
{
    Ipp32u chunk;
    Ipp32u samples;
    Ipp32u id;
    Ipp32u first_sample;
};

struct TM_stsc_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    Ipp32u entries_allocated;
    TM_stsc_table_data *table;
};

struct TM_stsz_table_data
{
    Ipp32u size;
};

struct TM_stsz_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u sample_size;
    Ipp32u total_entries;
    Ipp32u entries_allocated;
    TM_stsz_table_data *table;
};

struct TM_stco_table_data
{
    Ipp32u offset;
};

struct TM_stco_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;

    TM_stco_table_data *table;
};

struct TM_co64_table_data
{
    Ipp64u offset;
};

struct TM_co64_data
{
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    TM_co64_table_data *table;
};

struct TM_ctts_table_data
{
    Ipp32u sample_count;
    Ipp32u sample_offset;
};

struct TM_ctts_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    Ipp32s entries_allocated;
    TM_ctts_table_data *table;
};

struct TM_stbl_data
{
    Ipp32u size_atom;
    Ipp32s version;
    Ipp32s flags;
    TM_stsd_data stsd;
    TM_stts_data stts;
    TM_stss_data stss;
    TM_stsc_data stsc;
    TM_stsz_data stsz;
    TM_stco_data stco;
    TM_co64_data co64;
    TM_ctts_data ctts;
};

struct TM_hdlr_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32u flags;
    vm_char component_type[4];
    Ipp8s component_subtype[4];
    Ipp32u component_manufacturer;
    Ipp32u component_flags;
    Ipp32u component_flag_mask;
    vm_char component_name[250];
};

struct TM_dref_table_data
{
    Ipp32u size;
    Ipp8s type[4];
    Ipp8u version;
    Ipp32s flags;
    vm_char *data_reference;
};

struct TM_dref_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32u total_entries;
    TM_dref_table_data *table;
};

struct TM_dinf_data
{
    Ipp32u size_atom;
    TM_dref_data dref;
};

struct TM_vmhd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32s graphics_mode;
    Ipp32s opcolor[3];
};

struct TM_smhd_data
{
    Ipp32u size_atom;
    Ipp8u version;
    Ipp32s flags;
    Ipp32s balance;
    Ipp32s reserved;
};

struct TM_minf_data
{
    Ipp32u size_atom;
    Ipp32s is_video;
    Ipp32s is_audio;
    Ipp32s is_hint;
    TM_vmhd_data vmhd;
    TM_smhd_data smhd;
//    TM_hmhd_data hmhd;
    TM_stbl_data stbl;
    TM_hdlr_data hdlr;
    TM_dinf_data dinf;
};

struct TM_mdia_data
{
    Ipp32u size_atom;
    TM_mdhd_data mdhd;
    TM_minf_data minf;
    TM_hdlr_data hdlr;
};

struct TM_elst_table_data
{
    Ipp32u duration;
    Ipp32u time;
    Ipp32f rate;
};

struct TM_elst_data
{
    Ipp32u size_atom;
    Ipp32u version;
    Ipp32u flags;
    Ipp32u total_entries;
    TM_elst_table_data *table;
};

struct TM_edts_data
{
    TM_elst_data elst;
};

struct TM_hint_data
{
    Ipp32s numTracks;
    Ipp32s trackIds[MAXTRACKS];
    void* traks[MAXTRACKS];
};

struct TM_tref_data
{
    Ipp32u size_atom;
    //TM_dpnd_data    dpnd;
    TM_hint_data hint;
};

struct TM_trak_data
{
    Ipp32u size_atom;
    TM_tkhd_data tkhd;
    TM_mdia_data mdia;
    TM_edts_data edts;
    TM_tref_data tref;
    TM_hdlr_data hdlr;
};

struct TM_udta_data
{
    Ipp32u size_atom;
    Ipp8s *copyright;
    Ipp32u copyright_len;
    Ipp8s *name;
    Ipp32u name_len;
    Ipp8s *info;
    Ipp32u info_len;
};

struct TM_moov
{
    Ipp32u size_atom;
    Ipp32u total_tracks;
    TM_mvhd_data mvhd;
    TM_iods_data iods;
    TM_trak_data *trak[MAXTRACKS];
    TM_udta_data udta;
};

struct TM_table_trun
{
    Ipp32u        entry_count;
    TM_trun_table_data*  trun;
    Ipp64f       stream_end;
};

struct TM_moof
{
    Ipp32u size_atom;
    Ipp32u total_tracks;
    TM_table_trun *trun;
    Ipp32u sequence_number;
};

struct TM_mvex
{
    Ipp32u size_atom;
    Ipp32u total_tracks;
};

/*******************DECLARATIONS FUNCTIONS********************/

Ipp32s Atom_Skip(DataWriter *dr, TM_atom_mp4 *atom);
Ipp32s Compare_Atom(TM_atom_mp4 *atom, Ipp8s *type);

void Write_head(DataWriter *dr);
Status Write_moov(DataWriter *dr, TM_moov *moov);
Status Write_moof(DataWriter *dr, TM_moof *moof);
Status Write_mvhd(DataWriter *dr, TM_mvhd_data *mvhd_data);
Status Write_mdhd(DataWriter *dr, TM_mdhd_data *mdhd);
Status Write_hdlr(DataWriter *dr, TM_mdhd_data *mdhd);
Status Write_iods(DataWriter *dr, TM_iods_data *iods);
Status Write_tkhd(DataWriter *dr, TM_tkhd_data *tkhd);
Status Write_mdia(DataWriter *dr, TM_mdia_data *mdia, TM_atom_mp4 *trak_atom);
Status Write_minf(DataWriter *dr, TM_minf_data *minf, TM_atom_mp4 *parent_atom);
Status Write_tref(DataWriter *dr, TM_tref_data *tref, TM_atom_mp4 *atom);
//Ipp32s Write_dpnd(DataWriter *dr, TM_dpnd_data *dpnd);
Status Write_hdlr(DataWriter *dr, TM_hdlr_data *hdlr);
//Ipp32s Write_hmhd(DataWriter *dr, TM_hmhd_data *hmhd);
Status Write_vmhd(DataWriter *dw, TM_vmhd_data *vmhd);
Status Write_smhd(DataWriter *dw, TM_smhd_data *smhd);
Status Write_stbl(DataWriter *dr, TM_minf_data *minf, TM_stbl_data *stbl, TM_atom_mp4 *parent_atom);
Status Write_stts(DataWriter *dr, TM_stts_data *stts);
Status Write_stsd(DataWriter *dr, TM_minf_data *minf, TM_stsd_data *stsd);
Status Write_stsc(DataWriter *dr, TM_stsc_data *stsc);
Status Write_stss(DataWriter *dr, TM_stss_data *stss);
Status Write_stco(DataWriter *dr, TM_stco_data *stco);
Status Write_co64(DataWriter *dr, TM_co64_data *co64);
Status Write_stsz(DataWriter *dr, TM_stsz_data *stsz);
Status Write_ctts(DataWriter *dr, TM_ctts_data *ctts);
Status Write_stsd_table(DataWriter *dr, TM_minf_data *minf, TM_stsd_table_data *table);
Status Write_trak(DataWriter *dr, TM_trak_data *trak, TM_atom_mp4 *trak_atom);
Status Write_stsd_audio(DataWriter *dr, TM_stsd_table_data *table, TM_atom_mp4 *parent_atom);
Status Write_stsd_video(DataWriter *dr, TM_stsd_table_data *table, TM_atom_mp4 *parent_atom);
Status Write_h264_video(DataWriter *dr, TM_stsd_table_data *table, TM_atom_mp4 *parent_atom);
Status Write_stsd_hint(DataWriter *dr, TM_stsd_table_data *table, TM_atom_mp4 *parent_atom);
Status Write_esds(DataWriter *dr, TM_esds_data *esds);
Status Write_avcC(DataWriter *dr, TM_esds_data *avcC);
Status Write_dinf(DataWriter *dr, TM_dinf_data *dinf, TM_atom_mp4 *dinf_atom);
Status Write_dref(DataWriter *dr, TM_dref_data *dref);
Status Write_dref_table(DataWriter *dr, TM_dref_table_data *table);
Status Write_mvex(DataWriter *dr, TM_mvex *table);

Status Write_data(DataWriter *dw, Ipp8s *data, Ipp32s size);

#endif // __UMC_MP4_PARSER_W_H__
