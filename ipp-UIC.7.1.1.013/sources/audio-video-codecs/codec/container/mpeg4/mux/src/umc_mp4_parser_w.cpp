/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MP4_MUXER

#include "umc_mp4_parser_w.h"

UMC::Status Write_data(UMC::DataWriter *dw, Ipp8s *data, Ipp32s size)
{
    UMC::Status    ret;
    size_t         temp = size;

    ret = dw->PutData(data, temp);
    ret = ((size != (Ipp32s)temp) ? UMC::UMC_ERR_FAILED : UMC::UMC_OK);
    return ret;
}

UMC::Status Write_32(UMC::DataWriter *dw, Ipp32s value)
{
    Ipp8u       data_char[4];
    UMC::Status ret;
    size_t      size = 4;

    data_char[0] = (Ipp8u)((value >> 24) & 0xff);
    data_char[1] = (Ipp8u)((value >> 16) & 0xff);
    data_char[2] = (Ipp8u)((value >> 8) & 0xff);
    data_char[3] = (Ipp8u)(value & 0xff);
    ret = dw->PutData(data_char, size);
    ret = (size != 4) ? UMC::UMC_ERR_FAILED : UMC::UMC_OK;
    return ret;
}

UMC::Status Write_24(UMC::DataWriter *dw, Ipp32s value)
{
    Ipp8u       data_char[3];
    UMC::Status ret;
    size_t      size = 3;

    data_char[0] = (Ipp8u)((value >> 16) & 0xff);
    data_char[1] = (Ipp8u)((value >> 8) & 0xff);
    data_char[2] = (Ipp8u)(value & 0xff);
    ret = dw->PutData(data_char, size);
    ret = (size != 3) ? UMC::UMC_ERR_FAILED : UMC::UMC_OK;
    return ret;
}

UMC::Status Write_16(UMC::DataWriter *dw, Ipp32s value)
{
    Ipp8u       data_char[2];
    UMC::Status ret;
    size_t      size = 2;

    data_char[0] = (Ipp8u)((value >> 8) & 0xff);
    data_char[1] = (Ipp8u)(value & 0xff);
    ret = dw->PutData(data_char, size);
    ret = (size != 2) ? UMC::UMC_ERR_FAILED : UMC::UMC_OK;
    return ret;
}

UMC::Status    Write_8(UMC::DataWriter *dw, Ipp32s value)
{
    Ipp8u       data_char[1];
    UMC::Status ret;
    size_t      size = 1;

    data_char[0] = (Ipp8u)(value & 0xff);
    ret = dw->PutData(data_char, size);
    ret = ((size != 1) ? UMC::UMC_ERR_FAILED : UMC::UMC_OK);
    return ret;
}

Ipp32s Write_mp4_descr_length(UMC::DataWriter *dw, Ipp32s length)
{
    Ipp8u  b;
    Ipp32s i;
    Ipp8u  numBytes = 4;

    for ( i = numBytes - 1; i >= 0; i-- ) {
        b = (Ipp8u)((length >> (i * 7)) & 0x7F);
        if (i != 0) {
            b |= 0x80;
        }
        Write_8(dw, b);
    }
    return numBytes;
}

UMC::Status Write_AtomHeader(UMC::DataWriter *dw, TM_atom_mp4 *atom, Ipp8s* type)
{
    UMC::Status result;

    result = Write_32(dw, atom->size);
    result = Write_data(dw, type, 4);
    return result;
}

UMC::Status Write_mvhd(UMC::DataWriter *dw, TM_mvhd_data *mvhd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32s       i;

    atom.size = mvhd->size_atom;//108
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mvhd");
    result = Write_8(dw, (Ipp8s)mvhd->version);
    result = Write_24(dw, mvhd->flags);
    result = Write_32(dw, (Ipp32s)mvhd->creation_time);
    result = Write_32(dw, (Ipp32s)mvhd->modification_time);
    result = Write_32(dw, (Ipp32s)(mvhd->nTimeScale));
    result = Write_32(dw, (Ipp32s)mvhd->duration);
    result = Write_32(dw, 0x00010000);
    result = Write_16(dw, 0x0100);
    result = Write_16(dw, 0x0000);
    result = Write_32(dw, 0x00000000);
    result = Write_32(dw, 0x00000000);
    result = Write_32(dw, 0x00010000);
    for ( i = 0; i < 3; i++ ) {
        result = Write_32(dw, 0x00000000);
    }
    result = Write_32(dw, 0x00010000);
    for ( i = 0; i < 3; i++ ) {
        result = Write_32(dw, 0x00000000);
    }
    result = Write_32(dw, 0x40000000);
    for ( i = 0; i < 6; i++ ) {
        result = Write_32(dw, 0x00000000);
    }
    result = Write_32(dw, mvhd->next_track_id);

    return result;
}

UMC::Status Write_iods(UMC::DataWriter *dw, TM_iods_data *iods)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = iods->size_atom;//24
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"iods");
    result = Write_8(dw, (Ipp8s)iods->version);
    result = Write_24(dw, iods->flags);
    Write_8(dw, 0x10);    /* MP4_IOD_Tag */
    Write_mp4_descr_length(dw, 7);    /* length */
    Write_16(dw, 0x004F); /* ObjectDescriptorID = 1 */
    Write_8(dw, 0xFF);    /* ODProfileLevel */
    Write_8(dw, 0xFF);    /* sceneProfileLevel */
    Write_8(dw, iods->audioProfileId);    /* audioProfileLevel */
    Write_8(dw, iods->videoProfileId);    /* videoProfileLevel */
    Write_8(dw, 0xFF);    /* graphicsProfileLevel */
    return result;
}

UMC::Status Write_tkhd(UMC::DataWriter *dw, TM_tkhd_data *tkhd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32s       i;

    atom.size = tkhd->size_atom;//92+

    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"tkhd");
    result = Write_8(dw, tkhd->version);
    result = Write_24(dw, tkhd->flags);
    result = Write_32(dw, (Ipp32s)tkhd->creation_time);
    result = Write_32(dw, (Ipp32s)tkhd->modification_time);
    result = Write_32(dw, tkhd->track_id);
    result = Write_32(dw, 0x00000000); //tkhd->reserved1
//saosipov ?????*3    result = Write_32(dw, (Ipp32s)tkhd->duration * 3);
    result = Write_32(dw, (Ipp32s)tkhd->duration);

/*
    for ( i = 0; i < 3; i++ )
    {
        result = Write_32(dw, 0x00000000);
    }
*/

    for ( i = 0; i < 2; i++ )
    {
        result = Write_32(dw, 0x00000000);
    }

/*
    if ( tkhd->is_audio )
    {
        result = Write_16(dw, 0x0100);
    }
    else
    {
        result = Write_16(dw, 0x0000);
    }
*/
    result = Write_16(dw, 0x0000); // layer
    result = Write_16(dw, 0x0000); // alternative_group

    if ( tkhd->is_audio ) // volume
    {
        result = Write_16(dw, 0x0100);
    }
    else
    {
        result = Write_16(dw, 0x0000);
    }
    result = Write_16(dw, 0x0000); // reserved

    /* unity matrix */
    result = Write_32(dw, 0x00010000);
    for ( i = 0; i < 3; i++ )
    {
        result = Write_32(dw, 0x00000000);
    }
    result = Write_32(dw, 0x00010000);
    for ( i = 0; i < 3; i++ )
    {
        result = Write_32(dw, 0x00000000);
    }
    result = Write_32(dw, 0x40000000);

/*
    if ( tkhd->is_video )
    {
        result = Write_32(dw, 0x01400000);
    }
    else
    {
        result = Write_32(dw, 0x00000000);
    }

    if ( tkhd->is_video )
    {
        result = Write_32(dw, 0x00F00000);
    }
    else
    {
        result = Write_32(dw, 0x00000000);
    }
*/
    result = Write_32(dw, (Ipp32s)(tkhd->track_width));    // width
    result = Write_32(dw, (Ipp32s)(tkhd->track_height));   // height
 //   result = Write_32(dw, tkhd->track_width);    // width
 //   result = Write_32(dw, tkhd->track_height);   // heght

    return result;
}

UMC::Status Write_mdhd(UMC::DataWriter *dw, TM_mdhd_data *mdhd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = mdhd->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mdhd");
    result = Write_8(dw, mdhd->version);
    result = Write_24(dw, mdhd->flags);
    result = Write_32(dw, (Ipp32s)mdhd->creation_time);
    result = Write_32(dw, (Ipp32s)mdhd->modification_time);
    result = Write_32(dw, mdhd->nTimeScale);
    result = Write_32(dw, (Ipp32s)mdhd->duration);
    result = Write_16(dw, mdhd->language);

    result = Write_16(dw, 0x0000);    //mdhd->pre_defined
    return result;
}

UMC::Status Write_hdlr(UMC::DataWriter *dw, TM_hdlr_data *hdlr)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32s i;

    atom.size = hdlr->size_atom;

    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"hdlr");
    result = Write_8(dw, hdlr->version);
    result = Write_24(dw, hdlr->flags);
    result = Write_32(dw, 0x00);

    for (i = 0; i < 4; i++)
    {
        result = Write_8(dw, hdlr->component_type[i]);
    }

    for (i = 0; i < 3; i++)
    {
        result = Write_32(dw, 0x00);
    }

    for (i = 0; i < 25; i++)
    {
        result = Write_8(dw, hdlr->component_name[i]);
    }

    Write_8(dw, 0x00);

    return result;
};

UMC::Status Write_dref_table(UMC::DataWriter *dw, TM_dref_table_data *table)
{
    UMC::Status    result;
    Ipp32s         len;
    Ipp32s         i;

    if ( table->size - 12 ) {
        len = (Ipp32s)vm_string_strlen(table->data_reference);
    } else {
        len = 0;
    }
    result = Write_32(dw, 12 + len);
    for ( i = 0; i < 4; i++ ) {
        Write_8(dw, table->type[i]);
    }
    result = Write_8(dw, table->version);
    result = Write_24(dw, table->flags);
    if ( len ) {
        result = Write_data(dw, (Ipp8s *)table->data_reference, len);
    }
    return result;
}

UMC::Status Write_dref(UMC::DataWriter *dw, TM_dref_data *dref)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = dref->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"dref");
    result = Write_8(dw, dref->version);
    result = Write_24(dw, dref->flags);

    result = Write_32(dw, dref->total_entries);
    for ( i = 0; i < dref->total_entries; i++ )
    {
        result = Write_32(dw, 0x0C);
        result = Write_8(dw, 0x75);
        result = Write_8(dw, 0x72);
        result = Write_8(dw, 0x6C);
        result = Write_8(dw, 0x20);
        result = Write_32(dw, 1);
        //Write_dref_table(dw, &(dref->table[i]));
    }
    /*result = Write_32(dw, 1);
    result = Write_32(dw, 0x0C);
    result = Write_char(dw, 0x75);
    result = Write_char(dw, 0x72);
    result = Write_char(dw, 0x6C);
    result = Write_char(dw, 0x20);
    result = Write_32(dw, 1);*/

    return result;
}

UMC::Status Write_dinf(UMC::DataWriter *dw, TM_dinf_data *dinf)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = dinf->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"dinf");
    result = Write_dref(dw, &(dinf->dref));
    return result;
}

UMC::Status Write_stts(UMC::DataWriter *dw, TM_stts_data *stts)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stts->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stts");
    result = Write_8(dw, stts->version);
    result = Write_24(dw, stts->flags);
    result = Write_32(dw, stts->total_entries);

    for ( i = 0; i < stts->total_entries; i++ )
    {
        result = Write_32(dw, stts->table[i].sample_count);
        result = Write_32(dw, stts->table[i].sample_duration);
    }

    return result;
}

UMC::Status Write_ctts(UMC::DataWriter *dw, TM_ctts_data *ctts)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = ctts->size_atom;//  10008
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"ctts");
    result = Write_8(dw, ctts->version);
    result = Write_24(dw, ctts->flags);
    result = Write_32(dw, ctts->total_entries);

    for ( i = 0; i < ctts->total_entries; i++ )
    {
        result = Write_32(dw, ctts->table[i].sample_count);
        result = Write_32(dw, ctts->table[i].sample_offset);
    }

    return result;
}

UMC::Status Write_esds(UMC::DataWriter *dw, TM_esds_data *esds)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = esds->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"esds");
    result = Write_8(dw, esds->version);
    result = Write_24(dw, esds->flags);
    result = Write_8(dw, 0x03);
    Write_mp4_descr_length(dw, 3 + (5 + (13 + (5 + esds->decoderConfigLen))) + 3);
//    result = Write_16(dw, 0);
    result = Write_16(dw, 1); // 0x0 is forbidden
    result = Write_8(dw, 0x10);
    result = Write_8(dw, 0x04);
    Write_mp4_descr_length(dw, 13 + (5 + esds->decoderConfigLen));
    result = Write_8(dw, esds->objectTypeID);
    result = Write_8(dw, esds->streamType);
    result = Write_24(dw, 6144 /* esds->bufferSizeDB */);
    result = Write_32(dw, esds->maxBitrate);
    result = Write_32(dw, esds->avgBitrate);
    result = Write_8(dw, 0x05);
    Write_mp4_descr_length(dw, esds->decoderConfigLen);
    result = Write_data(dw, (Ipp8s*)esds->decoderConfig, esds->decoderConfigLen);
// SL
    result = Write_8(dw, 0x06);
    result = Write_8(dw, 0x01);
    result = Write_8(dw, 0x02);
    return result;
}

UMC::Status Write_avcC(UMC::DataWriter *dw, TM_esds_data *esds)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = esds->decoderConfigLen + 8;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"avcC");
    result = Write_data(dw, (Ipp8s*)esds->decoderConfig, esds->decoderConfigLen);
    return result;
}

UMC::Status Write_stsd_audio(UMC::DataWriter *dw, TM_stsd_table_data *table)
{
    UMC::Status    result;

    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_16(dw, table->channels);
    result = Write_16(dw, table->sample_size);
    result = Write_32(dw, 0);
    result = Write_32(dw, table->sample_rate << 16);
    result = Write_esds(dw, &(table->esds));
    return result;
}

UMC::Status Write_stsd_video(UMC::DataWriter *dw, TM_stsd_table_data *table)
{
    UMC::Status    result;
    Ipp32s i;

    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_16(dw, table->width);
    result = Write_16(dw, table->height);
    result = Write_32(dw, 0x00480000);
    result = Write_32(dw, 0x00480000);
    result = Write_32(dw, 0);
    result = Write_16(dw, 1);
    for (i = 0; i < 32; i++)
    {
        result = Write_8(dw, 0);
    }
    //result = Write_data(dw, "________________________________", 32);
    result = Write_16(dw, 0x0018);
    result = Write_16(dw, 0xffff);
    result = Write_esds(dw, &(table->esds));
    return result;
}

UMC::Status Write_h264_video(UMC::DataWriter *dw, TM_stsd_table_data *table)
{
    UMC::Status    result;
    Ipp32s i;

    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_32(dw, 0);
    result = Write_16(dw, table->width);
    result = Write_16(dw, table->height);
    result = Write_32(dw, 0x00480000);
    result = Write_32(dw, 0x00480000);
    result = Write_32(dw, 0);
    result = Write_16(dw, 1);
    for (i = 0; i < 32; i++)
    {
        result = Write_8(dw, 0);
    }
    //result = Write_data(dw, "________________________________", 32);
    result = Write_16(dw, 0x0018);
    result = Write_16(dw, 0xffff);
    result = Write_avcC(dw, &(table->esds));
    return result;
}

UMC::Status Write_stsd_table(UMC::DataWriter *dw, TM_stsd_table_data *table)
{
    TM_atom_mp4  atom;
    UMC::Status  result = UMC::UMC_OK;

    atom.size = table->size_atom;
    if ( table->format[0] == 'm' && table->format[1] == 'p' && table->format[2] == '4' && table->format[3] == 'a' )
    {
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mp4a");
        result = Write_32(dw, 0); // table->reserved
        result = Write_16(dw, 0); // table->reserved
        result = Write_16(dw, table->data_reference);
        result = Write_stsd_audio(dw, table);
    }
    if ( table->format[0] == 'm' && table->format[1] == 'p' && table->format[2] == '4' && table->format[3] == 'v' )
    {
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mp4v");
        result = Write_32(dw, 0); // table->reserved
        result = Write_16(dw, 0); // table->reserved
        result = Write_16(dw, table->data_reference);
        result = Write_stsd_video(dw, table);
    }
    if ( table->format[0] == 'a' && table->format[1] == 'v' && table->format[2] == 'c' && table->format[3] == '1' )
    {
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"avc1");
        result = Write_32(dw, 0); // table->reserved
        result = Write_16(dw, 0); // table->reserved
        result = Write_16(dw, table->data_reference);
        result = Write_h264_video(dw, table);
    }

    return result;
}

UMC::Status Write_stsd(UMC::DataWriter *dw, TM_stsd_data *stsd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stsd->size_atom;//100 138
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stsd");
    result = Write_8(dw, stsd->version);
    result = Write_24(dw, stsd->flags);
    result = Write_32(dw, stsd->total_entries);
    for ( i = 0; i < stsd->total_entries; i++ )
    {
        result = Write_stsd_table(dw, &(stsd->table[i]));
    }
    return result;
}

UMC::Status Write_stsz(UMC::DataWriter *dw, TM_stsz_data *stsz)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stsz->size_atom;//10960 7012
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stsz");
    result = Write_8(dw, stsz->version);
    result = Write_24(dw, stsz->flags);
    result = Write_32(dw, stsz->sample_size);
    result = Write_32(dw, stsz->total_entries);
    if ( !stsz->sample_size ) {
        for ( i = 0; i < stsz->total_entries; i++ ) {
            result = Write_32(dw, stsz->table[i].size);
        }
    }
    return result;
}

UMC::Status Write_stco(UMC::DataWriter *dw, TM_stco_data *stco)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stco->size_atom;//10956 7008
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stco");
    result = Write_8(dw, stco->version);
    result = Write_24(dw, stco->flags);
    result = Write_32(dw, stco->total_entries);
    for ( i = 0; i < stco->total_entries; i++ )
    {
           result = Write_32(dw, stco->table[i].offset);
    }
    return result;
}

UMC::Status Write_stsc(UMC::DataWriter *dw, TM_stsc_data *stsc)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stsc->size_atom;//32836 20992
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stsc");
    result = Write_8(dw, stsc->version);
    result = Write_24(dw, stsc->flags);
    result = Write_32(dw, stsc->total_entries);

    for ( i = 0; i < stsc->total_entries; i++ )
    {
        result = Write_32(dw, stsc->table[i].chunk);
        result = Write_32(dw, stsc->table[i].samples);
        result = Write_32(dw, stsc->table[i].id);
    }
    return result;
}

UMC::Status Write_stbl(UMC::DataWriter *dw, TM_stbl_data *stbl)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = stbl->size_atom;//54900 46206
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stbl");
    result = Write_stts(dw, &(stbl->stts));
    if (stbl->ctts.total_entries)
      result = Write_ctts(dw, &(stbl->ctts));
    result = Write_stsd(dw, &(stbl->stsd));
    result = Write_stsz(dw, &(stbl->stsz));
    result = Write_stco(dw, &(stbl->stco));
    result = Write_stsc(dw, &(stbl->stsc));

    if (stbl->stss.total_entries)
        result = Write_stss(dw, &(stbl->stss));

    return result;
}

UMC::Status Write_stss(UMC::DataWriter *dw, TM_stss_data *stss)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = stss->size_atom;//16  1016
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"stss");
    result = Write_8(dw, stss->version);
    result = Write_24(dw, stss->flags);
    result = Write_32(dw, stss->total_entries);

    for ( i = 0; i < stss->total_entries; i++ )
    {
        result = Write_32(dw, stss->table[i].sample + 1);
    }
    return result;
}

UMC::Status Write_vmhd(UMC::DataWriter *dw, TM_vmhd_data *vmhd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = vmhd->size_atom;//20
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"vmhd");
    result = Write_8(dw, vmhd->version);
    result = Write_24(dw, vmhd->flags);
    result = Write_16(dw, 0);
    result = Write_16(dw, 0);
    result = Write_16(dw, 0);
    result = Write_16(dw, 0);
    return result;
}

UMC::Status Write_smhd(UMC::DataWriter *dw, TM_smhd_data *smhd)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = smhd->size_atom;//16
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"smhd");
    result = Write_8(dw, smhd->version);
    result = Write_24(dw, smhd->flags);
    result = Write_16(dw, 0);
    result = Write_16(dw, 0);
    return result;
}

UMC::Status Write_minf(UMC::DataWriter *dw, TM_minf_data *minf)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = minf->size_atom;//54960 46270
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"minf");
//    result = Write_dinf(dw, &(minf->dinf));
//    result = Write_stbl(dw, &(minf->stbl));
    if ( minf->is_video ) {
        result = Write_vmhd(dw, &(minf->vmhd));
    }
    if ( minf->is_audio ) {
        result = Write_smhd(dw, &(minf->smhd));
    }

    result = Write_dinf(dw, &(minf->dinf));
    result = Write_stbl(dw, &(minf->stbl));

    return result;
}

UMC::Status Write_mdia(UMC::DataWriter *dw, TM_mdia_data *mdia)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = mdia->size_atom;//55058 46368
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mdia");
    result = Write_mdhd(dw, &(mdia->mdhd));
    result = Write_hdlr(dw, &(mdia->hdlr));
    result = Write_minf(dw, &(mdia->minf));
    return result;
}
UMC::Status Write_trak(UMC::DataWriter *dw, TM_trak_data *trak)
{
    TM_atom_mp4  atom;
    UMC::Status  result;

    atom.size = trak->size_atom;//55158 46468
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"trak");
    result = Write_tkhd(dw, &(trak->tkhd));
    result = Write_mdia(dw, &(trak->mdia));
    return result;
}
UMC::Status Write_mvex(UMC::DataWriter *dw, TM_mvex *mvex)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u i;

    atom.size = mvex->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mvex");

    atom.size -= 8;
    atom.size /= mvex->total_tracks;
    for (i = 0; i< mvex->total_tracks; i++ )
    {
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"trex");
        result = Write_8(dw, 0);
        result = Write_24(dw, 0);
        result = Write_32(dw, i + 1);
        result = Write_32(dw, 0);
        result = Write_32(dw, 0);
        result = Write_32(dw, 0);
        result = Write_32(dw, 0);
    }

    return result;
}

void Write_head(UMC::DataWriter *dw)
{
    Write_24(dw, 0x00);
    Write_8(dw, 0x18);
    Write_8(dw, 'f');
    Write_8(dw, 't');
    Write_8(dw, 'y');
    Write_8(dw, 'p');
    Write_8(dw, 'm');
    Write_8(dw, 'p');
    Write_8(dw, '4');
    Write_8(dw, '1');
    Write_24(dw, 0x00);
    Write_8(dw, 0x01);
    Write_8(dw, 'm');
    Write_8(dw, 'p');
    Write_8(dw, '4');
    Write_8(dw, '1');
    Write_8(dw, 'i');
    Write_8(dw, 's');
    Write_8(dw, 'o');
    Write_8(dw, 'm');

}

UMC::Status Write_moov(UMC::DataWriter *dw, TM_moov *moov)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = moov->size_atom;//101766
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"moov");
    result = Write_mvhd(dw, &(moov->mvhd));
    result = Write_iods(dw, &(moov->iods));
    for ( i = 0; i < moov->total_tracks; i++ ) {
        result = Write_trak(dw, moov->trak[i]);
    }
    return result;
}

UMC::Status Write_moof(UMC::DataWriter *dw, TM_moof *moof)
{
    TM_atom_mp4  atom;
    UMC::Status  result;
    Ipp32u    i;

    atom.size = moof->size_atom;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"moof");

    atom.size = 16;
    result = Write_AtomHeader(dw, &atom, (Ipp8s *)"mfhd");
    Write_32(dw, 0x00);
    Write_32(dw, moof->sequence_number);

    for (i = 0; i < moof->total_tracks; i++)
    {
        atom.size = 8;
        atom.size += (8 + 4 + 4);
        atom.size += (8 + 4 + 4 + moof->trun[i].entry_count * 8);
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"traf");

        atom.size = (8 + 4 + 4);
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"tfhd");
        Write_8(dw, 0x00);
        Write_24(dw, 0x00);
        Write_32(dw, i + 1);

        atom.size = (8 + 4 + 4 + moof->trun[i].entry_count * 8);
        result = Write_AtomHeader(dw, &atom, (Ipp8s *)"trun");
        Write_8(dw, 0x00);
        Write_24(dw, 0x100 | 0x200);
        Write_32(dw, moof->trun[i].entry_count);
        for (Ipp32u j = 0; j < moof->trun[i].entry_count; j++)
        {
            Write_32(dw, moof->trun[i].trun[j].sample_duration);
            Write_32(dw, moof->trun[i].trun[j].sample_size);
        }
    }

    return result;
}

#endif
