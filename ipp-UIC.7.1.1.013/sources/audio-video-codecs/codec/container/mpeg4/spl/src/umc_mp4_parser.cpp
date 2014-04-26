/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/


#include "umc_config.h"
#ifdef UMC_ENABLE_MP4_SPLITTER

#include "umc_mp4_splitter.h"
#include "umc_mp4_parser.h"

namespace UMC
{

Status MP4Splitter::Read_traf(DataReader *dr, T_traf *traf, T_atom_mp4 *atom)
{
    Status ret;
    T_atom_mp4    current_atom;

    traf->entry_count = 0;

    do
    {
        ret = Read_Atom(dr, &current_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&current_atom, (const char *)"tfhd"))
        {
            ret = Read_tfhd(dr, &traf->tfhd);
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &current_atom);
            UMC_CHECK_STATUS(ret)
        }

        if (Compare_Atom(&current_atom, (const char *)"trun"))
        {
            if (!traf->trun[traf->entry_count])
            {
                traf->trun[traf->entry_count] = (T_trun*) (ippMalloc(sizeof(T_trun)));
                memset(traf->trun[traf->entry_count], 0, sizeof(T_trun));
            }
            ret = Read_trun(dr, traf->trun[traf->entry_count]);
            UMC_CHECK_STATUS(ret)
            traf->entry_count++;
            ret = Atom_Skip(dr, &current_atom);
        }
    } while((dr->GetPosition() < atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    if (traf->entry_count > traf->max_truns)
        traf->max_truns = traf->entry_count;

    return ret;
}

Status MP4Splitter::Read_tfhd(DataReader *dr, T_tfhd *tfhd)
{
    dr->Get8u(&tfhd->version);
    tfhd->flags = Get_24(dr);
    dr->Get32uSwap(&tfhd->track_ID);

    if (tfhd->flags & 0x000001)
    {
        dr->Get64uSwap((Ipp64u*)&tfhd->base_data_offset);
    }
    else
    {
        tfhd->base_data_offset = 0;
    }

    if (tfhd->flags & 0x000002)
    {
        dr->Get32uSwap((Ipp32u*)&tfhd->sample_description_index);
    }
    else
    {
        tfhd->sample_description_index = 0;
    }

    if (tfhd->flags & 0x000008)
    {
        dr->Get32uSwap((Ipp32u*)&tfhd->default_sample_duration);
    }
    else
    {
        tfhd->default_sample_duration = 0;
    }

    if (tfhd->flags & 0x000010)
    {
        dr->Get32uSwap((Ipp32u*)&tfhd->default_sample_size);
    }
    else
    {
        tfhd->default_sample_size = 0;
    }

    if (tfhd->flags & 0x000020)
    {
        dr->Get32uSwap((Ipp32u*)&tfhd->default_sample_flags);
    }
    else
    {
        tfhd->default_sample_flags = 0;
    }

    return UMC_OK;
}

Status MP4Splitter::Read_trun(DataReader *dr, T_trun *trun)
{
    dr->Get8u(&trun->version);
    trun->flags = Get_24(dr);
    dr->Get32uSwap(&trun->sample_count);
    if (trun->flags & 0x000001)
    {
        dr->Get32uSwap((Ipp32u*)&trun->data_offset);
    }
    else
    {
        trun->data_offset = 0;
    }
    if (trun->flags & 0x000004)
    {
        dr->Get32uSwap((Ipp32u*)&trun->first_sample_flags);
    }
    else
    {
        trun->first_sample_flags = 0;
    }

    if (!trun->table_trun)
    {
        trun->table_trun = (T_trun_table_data*) (ippMalloc(sizeof(T_trun_table_data) * /*trun->sample_count*/40000));  // 40000 ~= number of entries in standard moof box
    }
    memset(trun->table_trun, 0, sizeof(T_trun_table_data) * /*trun->sample_count*/40000);
    return Read_trun_table(dr, trun, trun->table_trun);
}

Status MP4Splitter::Read_trun_table(DataReader *dr, T_trun* trun, T_trun_table_data* table)
{
    for (Ipp32u  i = 0; i < trun->sample_count; i++)
    {
        if (trun->flags & 0x000100)
        {
            dr->Get32uSwap((Ipp32u*)&table[i].sample_duration);
        }

        if (trun->flags & 0x000200)
        {
            dr->Get32uSwap((Ipp32u*)&table[i].sample_size);
        }

        if (trun->flags & 0x000400)
        {
            dr->Get32uSwap((Ipp32u*)&table[i].sample_flags);
        }

        if (trun->flags & 0x000800)
        {
            dr->Get32uSwap((Ipp32u*)&table[i].sample_composition_time_offset);
        }
    }
    return 0;
}

Status MP4Splitter::Clear_moof(T_moof &moof)
{
  Ipp32u i, j;

  for (i = 0; i < moof.total_tracks; i++) {
    for (j = 0; j < moof.traf[i]->entry_count; j++) {
      UMC_FREE(moof.traf[i]->trun[j]->table_trun)
      UMC_FREE(moof.traf[i]->trun[j])
    }
    UMC_FREE(moof.traf[i])
  }

  return UMC_OK;
}

Status MP4Splitter::Read_moof(DataReader *dr, T_moof *moof, T_atom_mp4 *atom)
{
  Status     umcRes;
  T_atom_mp4 current_atom;

  moof->end = atom->end;
  moof->total_tracks = 0;

  do
  {
    umcRes = Read_Atom(dr, &current_atom);
    UMC_CHECK_STATUS(umcRes)

    if (Compare_Atom(&current_atom, "mfhd")) {
      umcRes = Read_mfhd(dr, &moof->mfhd);
      UMC_CHECK_STATUS(umcRes)
      umcRes = Atom_Skip(dr, &current_atom);
    } else if (Compare_Atom(&current_atom, "traf")) {
      if (!moof->traf[moof->total_tracks]) {
        moof->traf[moof->total_tracks] = (T_traf*)(ippMalloc(sizeof(T_traf)));
        memset(moof->traf[moof->total_tracks], 0, sizeof(T_traf));
      }
      umcRes = Read_traf(dr, moof->traf[moof->total_tracks], &current_atom);
      UMC_CHECK_STATUS(umcRes)

      umcRes = Atom_Skip(dr, &current_atom);
      UMC_CHECK_STATUS(umcRes)
      moof->total_tracks++;
    } else {
      return UMC_ERR_INVALID_STREAM;
    }

  } while((dr->GetPosition() < atom->end) && (dr->GetPosition() != 0) && (umcRes == UMC_OK));

    return umcRes;
}

Ipp32u MP4Splitter::Get_24(DataReader *dr)
{
    Ipp32u     result = 0;
    Ipp32u     a, b, c;
    size_t     len;
    char       data[3];

    len = 3;
    dr->GetData(data, len);
    a = (Ipp8u )data[0];
    b = (Ipp8u )data[1];
    c = (Ipp8u )data[2];
    result = (a<<16) + (b<<8) + c;
    return result;
}

Ipp32s MP4Splitter::Read_mp4_descr_length(DataReader *dr)
{
    Ipp8u     b;
    Ipp8u            numBytes = 0;
    Ipp32s           length = 0;

    do
    {
        dr->Get8u(&b);
        numBytes++;
        length = (length << 7) | (b & 0x7F);
    } while ((b & 0x80) && numBytes < 4);

    return length;
}

Status MP4Splitter::Atom_Skip(DataReader *dr, T_atom_mp4 *atom)
{
    return dr->SetPosition(atom->end);
}

Status MP4Splitter::Compare_Atom(T_atom_mp4 *atom, const char *type)
{
    if ( atom->type[0] == type[0] &&
        atom->type[1] == type[1] &&
        atom->type[2] == type[2] &&
        atom->type[3] == type[3] )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Status MP4Splitter::Read_Atom(DataReader *dr, T_atom_mp4 *atom)
{
    Status ret;
    size_t  len;
    Ipp32u  size = 0;

    atom->start = dr->GetPosition();
    dr->Get32uSwap(&size);
    while(!size) // try to recover if position is invalid
    {
        dr->MovePosition(-3);
        atom->start = dr->GetPosition();
        dr->Get32uSwap(&size);
    }

    atom->is_large_size = (size == 1);
    len = 4;
    ret = dr->GetData(&atom->type, len);
    if (ret != UMC_OK)
    {
        return UMC_ERR_FAILED;
    }

    if (size == 1)
    {
        ret = dr->Get64uSwap(&atom->size);
        if (ret != UMC_OK)
        {
            return UMC_ERR_FAILED;
        }
    }
    else
    {
        atom->size = size;
    }

    atom->end = atom->start + atom->size;

    if (atom->type[0] == 'u' &&
        atom->type[1] == 'u' &&
        atom->type[2] == 'i' &&
        atom->type[3] == 'd')
    {
        Ipp8u userType[16];
        len = 16;
        ret = dr->GetData(userType, len);
    }

    return ret;
}

T_trak_data* MP4Splitter::Add_trak(T_moov *moov)
{
    if ( moov->total_tracks < MAXTRACKS )
    {
        moov->trak[moov->total_tracks] = (T_trak_data*) (ippMalloc(sizeof(T_trak_data)));
        if (moov->trak[moov->total_tracks]) {

            memset(moov->trak[moov->total_tracks],  0, sizeof(T_trak_data));
            moov->total_tracks++;
        } else {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return moov->trak[moov->total_tracks - 1];
}

// MOOV Box
// The metadata for a presentation is stored in the single Movie Box which occurs at the top-level of a file.
//
Status MP4Splitter::Read_moov(DataReader *dr, T_moov *moov, T_atom_mp4 *atom)
{
    Status ret;
    T_atom_mp4    current_atom;

    moov->total_tracks = 0;
    do
    {
        ret = Read_Atom(dr, &current_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&current_atom, (const char *)"mvhd"))
        {
            ret = Read_mvhd(dr, &(moov->mvhd));
        }
        else if (Compare_Atom(&current_atom, (const char *)"iods"))
        {
            ret = Read_iods(dr, &(moov->iods));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &current_atom);
        }
        else if (Compare_Atom(&current_atom, (const char *)"mvex"))
        {
            ret = Read_mvex(dr, &moov->mvex, &current_atom);
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &current_atom);
        }
        
        else if (Compare_Atom(&current_atom, (const char *)"trak"))
        {
            T_trak_data *trak = Add_trak(moov);
            if (trak == 0)
            {
                return UMC_ERR_FAILED;
            }
            ret = Read_trak(dr, trak, &current_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &current_atom);
        }
    } while ((dr->GetPosition() < atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

// MVHD Box
// This box defines overall information which is media-independent, and relevant to the entire presentation
// considered as a whole.
//
Status MP4Splitter::Read_mvhd(DataReader *dr, T_mvhd_data *mvhd)
{
    Status ret = UMC_OK;
    Ipp32u  temp;

    dr->Get8u(&mvhd->version);
    mvhd->flags = Get_24(dr);
    if ( mvhd->version )
    {
        dr->Get64uSwap(&mvhd->creation_time);
        dr->Get64uSwap(&mvhd->modification_time);
        dr->Get32uSwap(&mvhd->time_scale);
        dr->Get64uSwap(&mvhd->duration);
    }
    else
    {
        dr->Get32uSwap(&temp);
        mvhd->creation_time = temp;
        dr->Get32uSwap(&temp);
        mvhd->modification_time = temp;
        dr->Get32uSwap(&mvhd->time_scale);
        dr->Get32uSwap(&temp);
        mvhd->duration = temp;
    }
    dr->MovePosition((9 + 6) * 4 + 10 + 2 + 4); // RESERVED
    dr->Get32uSwap(&mvhd->next_track_id);

    return ret;
}

Status MP4Splitter::Read_iods(DataReader *dr, T_iods_data *iods)
{
    Status ret = UMC_OK;
    Ipp8u  tag;

    dr->Get8u(&iods->version);
    iods->flags = Get_24(dr );
    dr->Get8u(&tag);
    Read_mp4_descr_length(dr);    // skip length
    // skip ODID, ODProfile, sceneProfile
    dr->Get16uSwap(&iods->objectDescriptorID); // ODID (16 bit ?)
    dr->Get8u(&iods->OD_profileAndLevel); // ODProfile
    dr->Get8u(&iods->scene_profileAndLevel); // sceneProfile
    dr->Get8u(&iods->audioProfileId);
    dr->Get8u(&iods->videoProfileId);
    dr->Get8u(&iods->graphics_profileAndLevel);
    return ret;
}

// TRAK Box
// This is a container box for a single track of a presentation. A presentation consists of one or more tracks.
// Each track is independent of the other tracks in the presentation and carries its own temporal and spatial
// information. Each track will contain its associated Media Box.
//
Status MP4Splitter::Read_trak(DataReader *dr, T_trak_data *trak, T_atom_mp4 *trak_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    trak->tref.dpnd.idTrak = 0;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "tkhd"))
        {
            ret = Read_tkhd(dr, &(trak->tkhd));
        }
        else if (Compare_Atom(&leaf_atom, "mdia"))
        {
            ret = Read_mdia(dr, &(trak->mdia), &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "tref") )
        {
            ret = Read_tref(dr, &(trak->tref), &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ((dr->GetPosition() < trak_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

// TREF Box
// This box provides a reference from the containing track to another track in the presentation. These references
// are typed.
//
Status MP4Splitter::Read_tref(DataReader *dr, T_tref_data *tref, T_atom_mp4 *atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "dpnd"))
        {
            ret = Read_dpnd(dr, &(tref->dpnd));
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ((dr->GetPosition() < atom->end) && (dr->GetPosition() != 0)  && (ret == UMC_OK));

    return ret;
}


Status MP4Splitter::Read_dpnd(DataReader *dr, T_dpnd_data *dpnd)
{
    dr->Get32uSwap(&dpnd->idTrak);
    return UMC_OK;
}

// TKHD Box
// This box specifies the characteristics of a single track.
//
Status MP4Splitter::Read_tkhd(DataReader *dr, T_tkhd_data *tkhd)
{
    Status ret = UMC_OK;
    Ipp32u  temp;

    dr->Get8u(&tkhd->version);
    tkhd->flags = Get_24(dr);
    if ( tkhd->version )
    {
        dr->Get64uSwap(&tkhd->creation_time);
        dr->Get64uSwap(&tkhd->modification_time);
        dr->Get32uSwap(&tkhd->track_id);
        dr->MovePosition(4); //RESERVED
        dr->Get64uSwap(&tkhd->duration);
    }
    else
    {
        dr->Get32uSwap(&temp);
        tkhd->creation_time = temp;
        dr->Get32uSwap(&temp);
        tkhd->modification_time = temp;
        dr->Get32uSwap(&tkhd->track_id);
        dr->MovePosition(4); //RESERVED
        dr->Get32uSwap(&temp);
        tkhd->duration = temp;
    }
    dr->MovePosition(16 + 9 * 4); //RESERVED
    dr->Get32uSwap(&temp);
    tkhd->track_width = (Ipp32f)(temp >> 16);
    dr->Get32uSwap(&temp);
    tkhd->track_height = (Ipp32f)(temp >> 16);

    return ret;
}

// MDIA
// The media declaration container contains all the objects that declare information about the media data within a
// track.
//
Status MP4Splitter::Read_mdia(DataReader *dr, T_mdia_data *mdia, T_atom_mp4 *trak_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "mdhd"))
        {
            ret = Read_mdhd(dr, &(mdia->mdhd));
        }
        else if (Compare_Atom(&leaf_atom, "hdlr"))
        {
            ret = Read_hdlr(dr, &(mdia->hdlr));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "minf"))
        {
            ret = Read_minf(dr, &(mdia->minf), &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ((dr->GetPosition() < trak_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

// HDLR Box
// This box within a Media Box declares the process by which the media-data in the track is presented, and thus,
// the nature of the media in a track. For example, a video track would be handled by a video handler.
//
Status MP4Splitter::Read_hdlr(DataReader *dr, T_hdlr_data *hdlr)
{
    Status ret = UMC_OK;
    size_t  len;

    dr->Get8u(&hdlr->version);
    hdlr->flags = Get_24(dr);
    dr->MovePosition(4); // RESERVED
    len = 4;
    dr->GetData(hdlr->component_type, len);
    dr->MovePosition(12); // RESERVED

    return ret;
}

// MDHD Box
// The media header declares overall information that is media-independent, and relevant to characteristics of
// the media in a track.
//
Status MP4Splitter::Read_mdhd(DataReader *dr, T_mdhd_data *mdhd)
{
    Status ret = UMC_OK;
    Ipp32u  temp;

    dr->Get8u(&mdhd->version);
    mdhd->flags = Get_24(dr);
    if ( mdhd->version )
    {
        dr->Get64uSwap(&mdhd->creation_time);
        dr->Get64uSwap(&mdhd->modification_time);
        dr->Get32uSwap(&mdhd->time_scale);
        dr->Get64uSwap(&mdhd->duration);
    }
    else
    {
        dr->Get32uSwap(&temp);
        mdhd->creation_time = temp;
        dr->Get32uSwap(&temp);
        mdhd->modification_time = temp;
        dr->Get32uSwap(&mdhd->time_scale);
        dr->Get32uSwap(&temp);
        mdhd->duration = temp;
    }
    dr->Get16uSwap(&mdhd->language);
    dr->Get16uSwap(&mdhd->quality);    // RESERVED

    return ret;
}

// MINF Box
// This box contains all the objects that declare characteristic information of the media in the track.
//
Status MP4Splitter::Read_minf(DataReader *dr, T_minf_data *minf, T_atom_mp4 *parent_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "vmhd"))
        {
            minf->is_video = 1;
            ret = Read_vmhd(dr, &(minf->vmhd));
        }
        else if (Compare_Atom(&leaf_atom, "smhd"))
        {
            minf->is_audio = 1;
            ret = Read_smhd(dr, &(minf->smhd));
        }
        else if (Compare_Atom(&leaf_atom, "hmhd"))
        {
            minf->is_hint = 1;
            ret = Read_hmhd(dr, &(minf->hmhd));
        }
        else if (Compare_Atom(&leaf_atom, "dinf"))
        {
            ret = Read_dinf(dr, &(minf->dinf), &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "stbl"))
        {
            ret = Read_stbl(dr, minf, &(minf->stbl), &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "mvci"))
        {
            ret = Read_mvci(dr, &(minf->mvci), &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ( (dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

// HMHD Box
// The hint media header contains general information, independent of the protocol, for hint tracks.
//
Status MP4Splitter::Read_hmhd(DataReader *dr, T_hmhd_data *hmhd)
{
    Status ret = UMC_OK;

    dr->Get8u(&hmhd->version);
    hmhd->flags = Get_24(dr);
    dr->Get16uSwap(&hmhd->maxPDUsize);
    dr->Get16uSwap(&hmhd->avgPDUsize);
    dr->Get32uSwap(&hmhd->maxbitrate);
    dr->Get32uSwap(&hmhd->avgbitrate);
    dr->Get32uSwap(&hmhd->slidingavgbitrate);

    return ret;
}

// VMHD Box
// The video media header contains general presentation information, independent of the coding, for video
// media.
//
Status MP4Splitter::Read_vmhd(DataReader *dr, T_vmhd_data *vmhd)
{
    Status ret = UMC_OK;

    dr->Get8u(&vmhd->version);
    vmhd->flags = Get_24(dr);
    dr->MovePosition(8);    // RESERVED

    return ret;
}

// SMHD Box
// The sound media header contains general presentation information, independent of the coding, for audio
// media. This header is used for all tracks containing audio.
//
Status MP4Splitter::Read_smhd(DataReader *dr, T_smhd_data *smhd)
{
    Status ret = UMC_OK;

    dr->Get8u(&smhd->version);
    smhd->flags = Get_24(dr);
    dr->MovePosition(4); // RESERVED

    return ret;
}

Status MP4Splitter::Read_sbgp(DataReader *dr, T_sbgp_data *sbgp)
{
    Status ret = UMC_OK;
    Ipp32u i;

    dr->Get8u(&sbgp->version);
    sbgp->flags = Get_24(dr);

    ret = dr->Get32uSwap(&sbgp->grouping_type);
    ret = dr->Get32uSwap(&sbgp->entry_count);

    sbgp->entries = (T_sbgp_e*)ippMalloc(sizeof(T_sbgp_e) * sbgp->entry_count);
    if (NULL == sbgp->entries)
        return UMC_ERR_ALLOC;
    memset(sbgp->entries, 0, sizeof(T_sbgp_e) * sbgp->entry_count);

    for(i = 0; i < sbgp->entry_count; i++)
    {
        ret = dr->Get32uSwap(&sbgp->entries[i].sample_count);
        ret = dr->Get32uSwap(&sbgp->entries[i].group_description_index);
    }

    return ret;
}

Status MP4Splitter::Read_sgpd(DataReader *dr, T_sgpd_data *sgpd)
{
    Status ret = UMC_OK;
    Ipp8u *type;
    Ipp32u i;

    dr->Get8u(&sgpd->version);
    sgpd->flags = Get_24(dr);

    ret = dr->Get32uSwap(&sgpd->grouping_type);
    ret = dr->Get32uSwap(&sgpd->entry_count);

    sgpd->entries = (T_sgpd_e*)ippMalloc(sizeof(T_sgpd_e) * sgpd->entry_count);
    if (NULL == sgpd->entries)
        return UMC_ERR_ALLOC;
    memset(sgpd->entries, 0, sizeof(T_sgpd_e) * sgpd->entry_count);

    for(i = 0; i < sgpd->entry_count; i++)
    {
        ret  = dr->Get32uNoSwap((Ipp32u*)&(sgpd->entries[i].handler_type[0]));
        type = sgpd->entries[i].handler_type;/*
        if(type[0] == 'v' && type[1] == 'i' && type[2] == 'd' && type[3] == 'e') // for video tracks
        {

        }
        else if(type[0] == 's' && type[1] == 'o' && type[2] == 'u' && type[3] == 'n') // for audio tracks
        {

        }
        else if(type[0] == 'h' && type[1] == 'i' && type[2] == 'n' && type[3] == 't') // for hint tracks
        {
        
        }*/
    }

    return ret;
}

// STBL Box
// The sample table contains all the time and data indexing of the media samples in a track. Using the tables
// here, it is possible to locate samples in time, determine their type (e.g. I-frame or not), and determine their
// size, container, and offset into that container.
//
Status MP4Splitter::Read_stbl(DataReader *dr, T_minf_data *minf, T_stbl_data *stbl, T_atom_mp4 *parent_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "stsd"))
        {
            ret = Read_stsd(dr, minf, &(stbl->stsd));
            if (ret == UMC_OK) {
                ret = Atom_Skip(dr, &leaf_atom);
            }
        }
        else if ( Compare_Atom(&leaf_atom, "stts") )
        {
            ret = Read_stts(dr, &(stbl->stts));
        }
        else if ( Compare_Atom(&leaf_atom, "stss") )
        {
            ret = Read_stss(dr, &(stbl->stss));
        }
        else if ( Compare_Atom(&leaf_atom, "stsc") )
        {
            ret = Read_stsc(dr, &(stbl->stsc));
        }
        else if ( Compare_Atom(&leaf_atom, "stsz") )
        {
            ret = Read_stsz(dr, &(stbl->stsz));
        }
        else if ( Compare_Atom(&leaf_atom, "stco") )
        {
            ret = Read_stco(dr, &(stbl->stco));
        }
        else if ( Compare_Atom(&leaf_atom, "co64") )
        {
            ret = Read_co64(dr, &(stbl->co64));
        }
        else if ( Compare_Atom(&leaf_atom, "ctts") )
        {
            ret = Read_ctts(dr, &(stbl->ctts));
        }
        else if ( Compare_Atom(&leaf_atom, "sbgp") )
        {
            ret = Read_sbgp(dr, &(stbl->sbgp));
        }
        else if ( Compare_Atom(&leaf_atom, "sgpd") )
        {
            ret = Read_sgpd(dr, &(stbl->sgpd));
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ( (dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

Status MP4Splitter::Read_buff(DataReader *dr, T_buff_data *buff)
{
    Status ret = UMC_OK;

    ret = dr->Get16uSwap(&buff->operating_point_count);

    buff->operating_points = (T_buff_op*)ippMalloc(sizeof(T_buff_op) * buff->operating_point_count);
    if (NULL == buff->operating_points)
        return UMC_ERR_ALLOC;
    memset(buff->operating_points, 0, sizeof(T_buff_op) * buff->operating_point_count);

    for(Ipp32u i = 0; i < buff->operating_point_count; i++)
    {
        ret = dr->Get32uSwap(&buff->operating_points[i].byte_rate);
        ret = dr->Get32uSwap(&buff->operating_points[i].cpb_size);
        ret = dr->Get32uSwap(&buff->operating_points[i].dpb_size);
        ret = dr->Get32uSwap(&buff->operating_points[i].init_cpb_delay);
        ret = dr->Get32uSwap(&buff->operating_points[i].init_dpb_delay);
    }

    return ret;
}

Status MP4Splitter::Read_mvra(DataReader *dr, T_mvra_data *mvra)
{
    Status ret = UMC_OK;
    Ipp16u reserved;
    Ipp32u i;

    dr->Get8u(&mvra->version);
    mvra->flags = Get_24(dr);

    ret = dr->Get16uSwap(&reserved);
    ret = dr->Get16uSwap(&mvra->num_common_attributes);

    mvra->common_attributes = (T_mvra_ca*)ippMalloc(sizeof(T_mvra_ca) * mvra->num_common_attributes);
    if (NULL == mvra->common_attributes)
        return UMC_ERR_ALLOC;
    memset(mvra->common_attributes, 0, sizeof(T_mvra_ca) * mvra->num_common_attributes);

    for(i = 0; i < mvra->num_common_attributes; i++)
    {
        ret = dr->Get32uSwap(&mvra->common_attributes[i].common_attribute);
        ret = dr->Get32uNoSwap(&mvra->common_attributes[i].common_value);
    }

    ret = dr->Get16uSwap(&reserved);
    ret = dr->Get16uSwap(&mvra->num_differentiating_attributes);

    mvra->differentiating_attributes = (Ipp32u*)ippMalloc(sizeof(Ipp32u) * mvra->num_differentiating_attributes);
    if (NULL == mvra->differentiating_attributes)
        return UMC_ERR_ALLOC;
    memset(mvra->differentiating_attributes, 0, sizeof(Ipp32u) * mvra->num_differentiating_attributes);

    for (i = 0; i < mvra->num_differentiating_attributes; i++)
        ret = dr->Get32uSwap(&mvra->differentiating_attributes[i]);

    return ret;
}

Status MP4Splitter::Read_tibr(DataReader *dr, T_tibr_data *tibr)
{
    Status ret = UMC_OK;

    ret = dr->Get32uSwap(&tibr->baseBitRate);
    ret = dr->Get32uSwap(&tibr->maxBitRate);
    ret = dr->Get32uSwap(&tibr->avgBitRate);
    ret = dr->Get32uSwap(&tibr->tierBaseBitRate);
    ret = dr->Get32uSwap(&tibr->tierMaxBitRate);
    ret = dr->Get32uSwap(&tibr->tierAvgBitRate);

    return ret;
}

Status MP4Splitter::Read_tiri(DataReader *dr, T_tiri_data *tiri)
{
    Status ret = UMC_OK;
    BitstreamReader reader;
    reader.Init(dr);

    tiri->tierID                = (Ipp16u)reader.GetBits(16);
    tiri->profileIndication     = (Ipp8u)reader.GetBits(8);
    tiri->profile_compatibility = (Ipp8u)reader.GetBits(8);
    tiri->levelIndication       = (Ipp8u)reader.GetBits(8);
    reader.SkipBits(8);
    tiri->visualWidth           = (Ipp16u)reader.GetBits(16);
    tiri->visualHeight          = (Ipp16u)reader.GetBits(16);
    tiri->discardable           = (Ipp8u)reader.GetBits(2);
    tiri->constantFrameRate     = (Ipp8u)reader.GetBits(2);
    reader.SkipBits(4);
    tiri->frameRate             = (Ipp16u)reader.GetBits(16);
    reader.AlignReader();

    return ret;
}

Status MP4Splitter::Read_vwdi(DataReader *dr, T_vwdi_data *vwdi)
{
    Status ret = UMC_OK;

    ret = dr->Get8u(&vwdi->max_disparity);

    return ret;
}

// MVCG Box
// The sample table contains multiview group information
//
Status MP4Splitter::Read_mvcg(DataReader *dr, T_mvcg_data *mvcg, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;
    T_atom_mp4 leaf_atom;

    dr->Get8u(&mvcg->version);
    mvcg->flags = Get_24(dr);

    dr->Get32uSwap(&mvcg->multiview_group_id);
    dr->Get16uSwap(&mvcg->num_entries);
    dr->Get8u(&mvcg->entry_type);

    mvcg->entries = (T_mvcg_e*)ippMalloc(sizeof(T_mvcg_e) * mvcg->num_entries);
    if (NULL == mvcg->entries)
        return UMC_ERR_ALLOC;
    memset(mvcg->entries, 0, sizeof(T_mvcg_e) * mvcg->num_entries);

    if(mvcg->num_entries)
    {
        BitstreamReader reader;
        reader.Init(dr);

        for(Ipp32u i = 0; i < mvcg->num_entries; i++)
        {
            mvcg->entries[i].entry_type = (Ipp8u)reader.GetBits(8);

            if(mvcg->entries[i].entry_type == 0)
                mvcg->entries[i].track_id = reader.GetBits(32);
            else if(mvcg->entries[i].entry_type == 1)
            {
                mvcg->entries[i].track_id = reader.GetBits(32);
                mvcg->entries[i].tier_id  = (Ipp16u)reader.GetBits(16);
            }
            else if(mvcg->entries[i].entry_type == 2)
            {
                reader.SkipBits(6);
                mvcg->entries[i].output_view_id = (Ipp16u)reader.GetBits(10);
            }
            else if(mvcg->entries[i].entry_type == 3)
            {
                reader.SkipBits(6);
                mvcg->entries[i].start_view_id = (Ipp16u)reader.GetBits(10);
                mvcg->entries[i].view_count    = (Ipp16u)reader.GetBits(16);
            }
        }
        reader.AlignReader();
    }

    while ( (dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "tiri"))
        {
            ret = Read_tiri(dr, &(mvcg->tiri));
        }
        else if (Compare_Atom(&leaf_atom, "mvra"))
        {
            ret = Read_mvra(dr, &(mvcg->mvra));
        }
        else if (Compare_Atom(&leaf_atom, "tibr"))
        {
            ret = Read_tibr(dr, &(mvcg->tibr));
        }
        else if (Compare_Atom(&leaf_atom, "buff"))
        {
            ret = Read_buff(dr, &(mvcg->buff));
        }
        else if (Compare_Atom(&leaf_atom, "vwdi"))
        {
            ret = Read_vwdi(dr, &(mvcg->vwdi));
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
}

Status MP4Splitter::Read_swtc(DataReader *dr, T_swtc_data *swtc, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;
    T_atom_mp4 leaf_atom;

    dr->Get8u(&swtc->version);
    swtc->flags = Get_24(dr);

    dr->Get32uSwap(&swtc->num_entries);

    swtc->multiview_group_id = (Ipp32u*)ippMalloc(sizeof(Ipp32u) * swtc->num_entries);
    if (NULL == swtc->multiview_group_id)
        return UMC_ERR_ALLOC;
    memset(swtc->multiview_group_id, 0, sizeof(Ipp32u) * swtc->num_entries);

    for(Ipp32u i = 0; i < swtc->num_entries; i++)
        ret = dr->Get32uSwap(&swtc->multiview_group_id[i]);

    while ((dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "mvra"))
        {
            ret = Read_mvra(dr, &(swtc->mvra));
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
}

// MVCI Box
// The sample table contains multiview information
//
Status MP4Splitter::Read_mvci(DataReader *dr, T_mvci_data *mvci, T_atom_mp4 *parent_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    dr->Get8u(&mvci->version);
    mvci->flags = Get_24(dr);

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "mvcg"))
        {
            ret = Read_mvcg(dr, &(mvci->mvcg), &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "swtc"))
        {
            ret = Read_swtc(dr, &(mvci->swtc), &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    } while ( (dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

// CO64 Box
// The chunk offset table gives the index of each chunk into the containing file. Use of 64-bit offsets.
//
Status MP4Splitter::Read_co64(DataReader *dr, T_co64_data *co64)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&co64->version);
    co64->flags = Get_24(dr);
    dr->Get32uSwap(&co64->total_entries);
    co64->table = (T_co64_table_data*)ippMalloc(sizeof(T_co64_table_data) * co64->total_entries);

    // Check memory allocation
    if (NULL == co64->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(co64->table,  0, sizeof(T_co64_table_data) * co64->total_entries);


    for ( i = 0; i < co64->total_entries; i++ )
    {
        dr->Get64uSwap(&co64->table[i].offset);
    }

    return ret;
}

// CTTS Box
// This box provides the offset between decoding time and composition time.
//
Status MP4Splitter::Read_ctts(DataReader *dr, T_ctts_data *ctts)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&ctts->version);
    ctts->flags = Get_24(dr);
    dr->Get32uSwap(&ctts->total_entries);
    ctts->table = (T_ctts_table_data*)ippMalloc(sizeof(T_ctts_table_data) * ctts->total_entries);

    // Check memory allocation
    if (NULL == ctts->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(ctts->table,  0, sizeof(T_ctts_table_data) * ctts->total_entries);

    for ( i = 0; i < ctts->total_entries; i++ )
    {
        dr->Get32uSwap(&ctts->table[i].sample_count);
        dr->Get32uSwap(&ctts->table[i].sample_offset);
    }

    return ret;
}

// STCO Box
// The chunk offset table gives the index of each chunk into the containing file. Use of 32-bit offsets.
//
Status MP4Splitter::Read_stco(DataReader *dr, T_stco_data *stco)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&stco->version);
    stco->flags = Get_24(dr);
    dr->Get32uSwap(&stco->total_entries);
    stco->table = (T_stco_table_data*)ippMalloc(sizeof(T_stco_table_data) * stco->total_entries);

    // Check memory allocation
    if (NULL == stco->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(stco->table,  0, sizeof(T_stco_table_data) * stco->total_entries);

    for ( i = 0; i < stco->total_entries; i++ )
    {
        dr->Get32uSwap(&stco->table[i].offset);
    }

    return ret;
}

// STSZ Box
// This box contains the sample count and a table giving the size in bytes of each sample. This allows the media
// data itself to be unframed. The total number of samples in the media is always indicated in the sample count.
//
Status MP4Splitter::Read_stsz(DataReader *dr, T_stsz_data *stsz)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&stsz->version);
    stsz->flags = Get_24(dr);
    dr->Get32uSwap(&stsz->sample_size);
    dr->Get32uSwap(&stsz->total_entries);
    stsz->max_sample_size = stsz->sample_size;  // to calculate buffer size
    if (!stsz->sample_size) {
      stsz->table = (T_stsz_table_data*)ippMalloc(sizeof(T_stsz_table_data) * stsz->total_entries);

      // Check memory allocation
      if (NULL == stsz->table) {
          return ret;
      }

      // Zeroed allocated table

      memset(stsz->table,  0, sizeof(T_stsz_table_data) * stsz->total_entries);

      for (i = 0; i < stsz->total_entries; i++) {
        dr->Get32uSwap(&stsz->table[i].size);
        if (stsz->table[i].size > stsz->max_sample_size) {
            stsz->max_sample_size = stsz->table[i].size;
        }
      }
    }

    return ret;
}

// STSC Box
// Samples within the media data are grouped into chunks. Chunks can be of different sizes, and the samples
// within a chunk can have different sizes. This table can be used to find the chunk that contains a sample, its
// position, and the associated sample description.
//
Status MP4Splitter::Read_stsc(DataReader *dr, T_stsc_data *stsc)
{
    Status ret = UMC_OK;
    Ipp32u  i;
    Ipp32u  sample = 0;

    dr->Get8u(&stsc->version);
    stsc->flags = Get_24(dr);
    dr->Get32uSwap(&stsc->total_entries);
/////    stsc->entries_allocated = stsc->total_entries;
    stsc->table = (T_stsc_table_data*)ippMalloc(sizeof(T_stsc_table_data) * stsc->total_entries);

    // Check memory allocation
    if (NULL == stsc->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(stsc->table,  0, sizeof(T_stsc_table_data) * stsc->total_entries);

    for ( i = 0; i < stsc->total_entries; i++ )
    {
        dr->Get32uSwap(&stsc->table[i].chunk);
        dr->Get32uSwap(&stsc->table[i].samples);
        dr->Get32uSwap(&stsc->table[i].id);
    }
    for ( i = 0; i < stsc->total_entries; i++ )
    {
        stsc->table[i].first_sample = sample;
        if ( i < stsc->total_entries - 1 )
        {
            sample += (stsc->table[i+1].chunk - stsc->table[i].chunk) * stsc->table[i].samples;
        }
    }

    return ret;
}

// STTS Box
// This box contains a compact version of a table that allows indexing from decoding time to sample number.
//
Status MP4Splitter::Read_stts(DataReader *dr, T_stts_data *stts)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&stts->version);
    stts->flags = Get_24(dr);
    dr->Get32uSwap(&stts->total_entries);
    stts->table = (T_stts_table_data*)ippMalloc(sizeof(T_stts_table_data) * stts->total_entries);

    // Check memory allocation
    if (NULL == stts->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(stts->table,  0, sizeof(T_stts_table_data) * stts->total_entries);

    for( i = 0; i < stts->total_entries; i++ )
    {
        dr->Get32uSwap(&stts->table[i].sample_count);
        dr->Get32uSwap(&stts->table[i].sample_duration);
    }

    return ret;
}

// STSS Box
// This box contains a sync (key, I-frame) sample map.
//
Ipp32s MP4Splitter::Read_stss(DataReader *dr, T_stss_data *stss)
{
    Status ret = UMC_OK;
    Ipp32u i;

    dr->Get8u(&stss->version);
    stss->flags = Get_24(dr);
    dr->Get32uSwap(&stss->total_entries);
    stss->table = (T_stss_table_data*)ippMalloc(sizeof(T_stss_table_data) * stss->total_entries);

    // Check memory allocation
    if (NULL == stss->table) {
        return ret;
    }

    // Zeroed allocated table

    memset(stss->table,  0, sizeof(T_stss_table_data) * stss->total_entries);

    for( i = 0; i < stss->total_entries; i++ )
    {
        dr->Get32uSwap(&stss->table[i].sample_number);
        UMC_CHECK_STATUS(ret)
    }

    return ret;
}
// STSD Box
// The sample description table gives detailed information about the coding type used, and any initialization
// information needed for that coding.
//
Status MP4Splitter::Read_stsd(DataReader *dr, T_minf_data *minf, T_stsd_data *stsd)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&stsd->version);
    stsd->flags = Get_24(dr);
    dr->Get32uSwap(&stsd->total_entries);
    stsd->table = (T_stsd_table_data*)ippMalloc(sizeof(T_stsd_table_data) * stsd->total_entries);

    // Check memory allocation
    if (NULL == stsd->table)
    {
        return ret;
    }

    // Zeroed allocated table
    memset(stsd->table,  0, sizeof(T_stsd_table_data) * stsd->total_entries);

    for ( i = 0; i < stsd->total_entries; i++ )
    {
        ret = Read_stsd_table(dr, minf, &(stsd->table[i]));
        UMC_CHECK_STATUS(ret)
    }

    return ret;
}

Status MP4Splitter::Read_samr_audio(DataReader *dr,
                                    T_stsd_table_data *table)
{
  Status ret;
  T_atom_mp4 leaf_atom;
  Ipp16u i;
  dr->MovePosition(16); // reserved
  dr->Get16uSwap(&i);
  dr->MovePosition(2); // reserved

  ret = Read_Atom(dr, &leaf_atom);
  UMC_CHECK_STATUS(ret)

  if (leaf_atom.type[0] == 'd' && leaf_atom.type[1] == 'a' &&
      leaf_atom.type[2] == 'm' && leaf_atom.type[3] =='r') {
    return Read_damr_audio(dr, table, &leaf_atom);
  }

  return ret;
}

Status MP4Splitter::Read_damr_audio(DataReader *dr,
                                    T_stsd_table_data *table,
                                    T_atom_mp4 *parent_atom)
{
  size_t    temp;

  table->damr.decoderConfigLen = (Ipp32s)(parent_atom->size - 8);
  table->damr.decoderConfig = (Ipp8u*)(ippMalloc(table->damr.decoderConfigLen));

  // Check memory allocation
  if (NULL == table->damr.decoderConfig) {
    return UMC_OK;
  }

  // Zeroed allocated table
  memset(table->damr.decoderConfig,  0, table->damr.decoderConfigLen);

  temp = table->damr.decoderConfigLen;
  dr->GetData(table->damr.decoderConfig, temp);

  return UMC_OK;
}

Status MP4Splitter::Read_stsd_table(DataReader *dr, T_minf_data * /*minf*/, T_stsd_table_data *table)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    ret = Read_Atom(dr, &leaf_atom);
    UMC_CHECK_STATUS(ret)

    table->is_protected = 0;
    table->format[0] = leaf_atom.type[0];
    table->format[1] = leaf_atom.type[1];
    table->format[2] = leaf_atom.type[2];
    table->format[3] = leaf_atom.type[3];
    dr->MovePosition(6); // RESERVED
    dr->Get16uSwap(&table->data_reference);
    if ( table->format[0]=='m'&& table->format[1]=='p'&& table->format[2]=='4'&& table->format[3]=='a' )
    {
        ret = Read_stsd_audio(dr, table, &leaf_atom);
    }
    if ( table->format[0]=='s'&& table->format[1]=='a'&& table->format[2]=='m'&& table->format[3]=='r' )
    {
        ret = Read_samr_audio(dr, table);
    }
    if ( table->format[0]=='m'&& table->format[1]=='p'&& table->format[2]=='4'&& table->format[3]=='v' )
    {
        ret = Read_stsd_video(dr, table, &leaf_atom);
    }
    if ( table->format[0]=='m'&& table->format[1]=='p'&& table->format[2]=='4'&& table->format[3]=='s' )
    {
        ret = Read_stsd_hint(dr, table, &leaf_atom);
    }

    if ( table->format[0]=='d' && table->format[1]=='r' && table->format[2]=='m' && table->format[3]=='s' )
    {
        ret = Read_stsd_drms(dr, table, &leaf_atom);
    }

    if ( table->format[0]=='s'&& table->format[1]=='2'&& table->format[2]=='6'&& table->format[3]=='3' )
    {
        ret = Read_h263_video(dr, table);
        table->esds.objectTypeID = 0xf2;
    }

    if ( table->format[0]=='a'&& table->format[1]=='v'&& table->format[2]=='c'&& table->format[3]=='1' )
    {
        ret = Read_h264_video(dr, table, &leaf_atom);
        table->esds.objectTypeID = 0xf1;
    }
    if ( table->format[0]=='a'&& table->format[1]=='v'&& table->format[2]=='s'&& table->format[3]=='2' )
    {
        ret = Read_stsd_video(dr, table, &leaf_atom);
        table->esds.objectTypeID = 0xf4;
    }

    return ret;
}

Status MP4Splitter::Read_stsd_drms(UMC::DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;
    Ipp16u sample_rate;

    table->is_protected = 1;

    dr->MovePosition(8); // reserved

    dr->Get16uSwap(&table->channels);
    dr->Get16uSwap(&table->sample_size);

    dr->MovePosition(4); // reserved

    dr->Get16uSwap(&sample_rate);
    table->sample_rate = sample_rate;

    dr->MovePosition(2); // reserved

    while ( (dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        T_atom_mp4 leaf_atom;
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "esds"))
        {
            ret = Read_esds(dr, &(table->esds));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
} // int Read_stsd_drms(UMC::DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
// STSD_HINT
// For hint tracks, the sample description contains appropriate declarative data for the streaming protocol being
// used, and the format of the hint track. The definition of the sample description is specific to the protocol.
//
Status MP4Splitter::Read_stsd_hint(DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;

    //dr->MovePosition(16); // RESERVED
    while ((dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        T_atom_mp4 leaf_atom;
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if ( Compare_Atom(&leaf_atom, "esds") )
        {
            ret = Read_esds(dr, &(table->esds));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
}

// STSD_VIDEO
// For video tracks, a VisualSampleEntry is used.
//
Status MP4Splitter::Read_stsd_video(DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;

    dr->MovePosition(2 + 2 + 3*4);
    table->width = 0;
    table->height = 0;
    dr->Get16uSwap(&table->width);
    dr->Get16uSwap(&table->height);
    dr->Get32uSwap(&table->dpi_horizontal);
    dr->Get32uSwap(&table->dpi_vertical);
    dr->MovePosition(4 + 2 + 32 + 2 + 2);

    while ((dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        T_atom_mp4 leaf_atom;
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "esds"))
        {
            ret = Read_esds(dr, &(table->esds));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
    //return 0;
}

Status MP4Splitter::Read_h263_video(DataReader *dr, T_stsd_table_data *table)
{
    dr->MovePosition(16);

    table->width = 0;
    table->height = 0;

    dr->Get16uSwap(&table->width);
    dr->Get16uSwap(&table->height);

    dr->MovePosition(39);
    dr->MovePosition(8);

    return UMC_OK;
}

void MP4Splitter::SetH264FrameIntraSize(Ipp8u* decoderConfig)
{
    Ipp8u *p = decoderConfig;
    p += 4;
    m_nH264FrameIntraSize = *p;
    m_nH264FrameIntraSize <<= 6;
    m_nH264FrameIntraSize >>= 6; // remove reserved bits
    m_nH264FrameIntraSize++;
    return;
};

Status MP4Splitter::Read_vwid(DataReader *dr, T_vwid_data *vwid)
{
    Status ret = UMC_OK;
    Ipp32u i, j;

    dr->Get8u(&vwid->version);
    vwid->flags = Get_24(dr);

    BitstreamReader reader;
    reader.Init(dr);

    reader.SkipBits(2);
    vwid->min_temporal_id = (Ipp8u)reader.GetBits(3);
    vwid->max_temporal_id = (Ipp8u)reader.GetBits(3);
    vwid->num_views       = (Ipp16u)reader.GetBits(16);

    vwid->views = (T_vwid_v*)ippMalloc(sizeof(T_vwid_v) * vwid->num_views);
    if (NULL == vwid->views)
        return UMC_ERR_ALLOC;
    memset(vwid->views, 0, sizeof(T_vwid_v) * vwid->num_views);

    for(i = 0; i < vwid->num_views; i++)
    {
        reader.SkipBits(6);
        vwid->views[i].view_id = (Ipp16u)reader.GetBits(10);
        reader.SkipBits(6);
        vwid->views[i].view_order_index = (Ipp16u)reader.GetBits(10);
        reader.SkipBits(4);
        vwid->views[i].base_view_type = (Ipp8u)reader.GetBits(2);
        vwid->views[i].num_ref_views  = (Ipp16u)reader.GetBits(10);

        if(vwid->views[i].num_ref_views)
        {
            vwid->views[i].ref_view_id = (Ipp16u*)ippMalloc(sizeof(Ipp16u) * vwid->views[i].num_ref_views);
            if (NULL == vwid->views[i].ref_view_id)
                return UMC_ERR_ALLOC;
            memset(vwid->views[i].ref_view_id, 0, sizeof(Ipp16u) * vwid->views[i].num_ref_views);

            for (j = 0; j < vwid->views[i].num_ref_views; j++)
            {
                reader.SkipBits(6);
                vwid->views[i].ref_view_id[j] = (Ipp16u)reader.GetBits(10);
            }
        }
    }

    return ret;
}

Ipp32s MP4Splitter::Read_h264_video(DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;

    dr->MovePosition(16);

    table->width = 0;
    table->height = 0;

    dr->Get16uSwap(&table->width);
    dr->Get16uSwap(&table->height);

    dr->MovePosition(50);

    while ((dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        T_atom_mp4 leaf_atom;
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "avcC"))
        {
            size_t    temp;

            table->avcC.decoderConfigLen = (Ipp32s)(leaf_atom.size - 8);
            table->avcC.decoderConfig    = (Ipp8u*)(ippMalloc(table->avcC.decoderConfigLen));
            if (NULL == table->avcC.decoderConfig && table->avcC.decoderConfigLen > 0)
                return UMC_ERR_FAILED;
            memset(table->avcC.decoderConfig,  0, table->avcC.decoderConfigLen);

            temp = table->avcC.decoderConfigLen;
            dr->GetData(table->avcC.decoderConfig, temp);

            if (temp > 4)
                SetH264FrameIntraSize(table->avcC.decoderConfig);

            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "mvcC"))
        {
            size_t    temp;

            table->mvcC.decoderConfigLen = (Ipp32s)(leaf_atom.size - 8);
            table->mvcC.decoderConfig    = (Ipp8u*)(ippMalloc(table->mvcC.decoderConfigLen));
            if (NULL == table->mvcC.decoderConfig && table->mvcC.decoderConfigLen > 0)
                return UMC_ERR_FAILED;
            memset(table->mvcC.decoderConfig,  0, table->mvcC.decoderConfigLen);

            temp = table->mvcC.decoderConfigLen;
            dr->GetData(table->mvcC.decoderConfig, temp);

            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "svcC"))
        {
            size_t    temp;

            table->svcC.decoderConfigLen = (Ipp32s)(leaf_atom.size - 8);
            table->svcC.decoderConfig    = (Ipp8u*)(ippMalloc(table->svcC.decoderConfigLen));
            if (NULL == table->svcC.decoderConfig && table->svcC.decoderConfigLen > 0)
                return UMC_ERR_FAILED;
            memset(table->svcC.decoderConfig,  0, table->svcC.decoderConfigLen);

            temp = table->svcC.decoderConfigLen;
            dr->GetData(table->svcC.decoderConfig, temp);

            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "mvcP") )
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "vsib") )
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if (Compare_Atom(&leaf_atom, "btrt"))
        {
            dr->Get32uSwap((Ipp32u*)&table->esds.bufferSizeDB);
            dr->Get32uSwap(&table->esds.maxBitrate);
            dr->Get32uSwap(&table->esds.avgBitrate);
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "m4ds") )
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "ecam") )
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "icam") )
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else if ( Compare_Atom(&leaf_atom, "vwid") )
        {
            ret = Read_vwid(dr, &(table->vwid));
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
}

Status MP4Splitter::Read_stsd_audio(DataReader *dr, T_stsd_table_data *table, T_atom_mp4 *parent_atom)
{
    Status ret = UMC_OK;

    dr->MovePosition(8); // RESERVED
    dr->Get16uSwap(&table->channels);
    dr->Get16uSwap(&table->sample_size);
    dr->MovePosition(4); // RESERVED
    dr->Get32uSwap(&table->sample_rate);
    table->sample_rate = table->sample_rate >> 16;

    while ((dr->GetPosition() < parent_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK))
    {
        T_atom_mp4 leaf_atom;
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&leaf_atom, "esds"))
        {
            ret = Read_esds(dr, &(table->esds));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &leaf_atom);
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
        }
    }

    return ret;
}

Status MP4Splitter::Read_esds(DataReader *dr, T_esds_data *esds)
{
    Ipp8u  tag;
    Ipp8u  temp2 = 0;
    int len;
    size_t  temp;

    dr->Get8u(&esds->version);
    esds->flags = Get_24(dr);

    esds->decoderConfigLen = 0;
    /* get and verify ES_DescrTag */
    dr->Get8u(&tag);
    if ( tag == 0x03 )
    {
        len = Read_mp4_descr_length(dr);
        dr->Get16uSwap(&esds->es_ID);
        dr->Get8u(&temp2);
        esds->streamDependenceFlag = temp2 & (1 << 7) ? 1 : 0;
        esds->urlflag = temp2 & (1 << 6) ? 1 : 0;
        esds->ocrflag = temp2 & (1 << 5) ? 1 : 0;
        esds->streamPriority = temp2 & 0x1f;
        len -= 3;
        while ( len > 0 )
        {
            dr->Get8u(&tag);
            len -= 1;
            switch ( tag )
            {
                case 0x04: // verify DecoderConfigDescrTag
                {
                    Read_mp4_descr_length(dr);
                    dr->Get8u(&esds->objectTypeID);
                    dr->Get8u(&esds->streamType);    // 3 - video, 4 - graphic, 5 - audio
                    esds->bufferSizeDB = Get_24(dr);
                    dr->Get32uSwap(&esds->maxBitrate);
                    dr->Get32uSwap(&esds->avgBitrate);
                    len -= 17;
                    if (0 == len) break;

                    dr->Get8u(&tag);

                    if ( tag == 0x05 )
                    {
                        esds->decoderConfigLen = Read_mp4_descr_length(dr);
                        if ( esds->decoderConfigLen )
                        {
                            esds->decoderConfig = (Ipp8u*)(ippMalloc(esds->decoderConfigLen));

                            // Check memory allocation
                            if (NULL == esds->decoderConfig) {
                                return UMC_OK;
                            }

                            // Zeroed allocated table

                            memset(esds->decoderConfig,  0, esds->decoderConfigLen);

                            esds->start = dr->GetPosition();
                            temp = esds->decoderConfigLen;
                            dr->GetData(esds->decoderConfig, temp);
                            len -= 4 + (Ipp32u)temp;
                        }
                        else
                        {
                            esds->decoderConfigLen = 0;
                            len -= 4;
                        }
                    }
                    break;
                }
                case 0x06: // verify DecSpecificInfoTag
                {
                    temp = Read_mp4_descr_length(dr);
                    dr->MovePosition(temp);    // skip SL tag
                    len -= 4 + (Ipp32u)temp;
                    break;
                }
            } // switch tag
        } // while
    } // if (tag)
    else
    {
        return UMC_OK;
    }

    return UMC_OK;
}

// DINF Box
// The data information box contains objects that declare the location of the media information in a track.
//
Status MP4Splitter::Read_dinf(DataReader *dr, T_dinf_data *dinf, T_atom_mp4 *dinf_atom)
{
    Status ret;
    T_atom_mp4 leaf_atom;

    do
    {
        ret = Read_Atom(dr, &leaf_atom);
        UMC_CHECK_STATUS(ret)

        if(Compare_Atom(&leaf_atom, "dref"))
        {
            ret = Read_dref(dr, &(dinf->dref));
            UMC_CHECK_STATUS(ret)
        }
        else
        {
            ret = Atom_Skip(dr, &leaf_atom);
            UMC_CHECK_STATUS(ret)
        }
    } while ((dr->GetPosition() < dinf_atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}


// DREF Box
// The data reference object contains a table of data references (normally URLs) that declare the location(s) of
// the media data used within the presentation.
//
Status MP4Splitter::Read_dref(DataReader *dr, T_dref_data *dref)
{
    Status ret = UMC_OK;
    Ipp32u  i;

    dr->Get8u(&dref->version);
    dref->flags = Get_24(dr);
    dr->Get32uSwap(&dref->total_entries);
    dref->table = (T_dref_table_data*)ippMalloc(sizeof(T_dref_table_data) * dref->total_entries);

    // Check memory allocation
    if (NULL == dref->table) {
        return ret;
    }

    // Zeroed allocated table
    memset(dref->table,  0, sizeof(T_dref_table_data) * dref->total_entries);

    for ( i = 0; i < dref->total_entries; i++ )
    {
        ret = Read_dref_table(dr, &(dref->table[i]));
        UMC_CHECK_STATUS(ret)
    }

    return ret;
}

Status MP4Splitter::Read_dref_table(DataReader *dr, T_dref_table_data *table)
{
    Status ret = UMC_OK;
    size_t  len;

    dr->Get32uSwap(&table->size);
    len = 4;
    dr->GetData(table->type, len);
    dr->Get8u(&table->version);
    table->flags = Get_24(dr);
    table->data_reference = (char*)ippMalloc(table->size);

    // Check memory allocation
    if (NULL == table->data_reference) {
        return ret;
    }

    // Zeroed allocated table
    memset(table->data_reference,  0, table->size);

    len = table->size - 12;
    if ( table->size > 12 )
    {
        dr->GetData(table->data_reference, len);
    }
    table->data_reference[table->size - 12] = 0;

    return ret;
}

Status MP4Splitter::Read_mvex(DataReader *dr, T_mvex_data *mvex, T_atom_mp4 *atom)
{
    Status ret;
    T_atom_mp4    current_atom;

    mvex->total_tracks = 0;

    do
    {
        ret = Read_Atom(dr, &current_atom);
        UMC_CHECK_STATUS(ret)

        if (Compare_Atom(&current_atom, "trex"))
        {
            mvex->trex[mvex->total_tracks] = (T_trex_data*) (ippMalloc(sizeof(T_trex_data)));
            ret = Read_trex(dr, (mvex->trex[mvex->total_tracks]));
            UMC_CHECK_STATUS(ret)
            ret = Atom_Skip(dr, &current_atom);
            mvex->total_tracks++;
        }

    } while((dr->GetPosition() < atom->end) && (dr->GetPosition() != 0) && (ret == UMC_OK));

    return ret;
}

Status MP4Splitter::Read_trex(DataReader *dr, T_trex_data *trex)
{
    dr->Get8u(&trex->version);
    trex->flags = Get_24(dr);
    dr->Get32uSwap(&trex->track_ID);
    dr->Get32uSwap(&trex->default_sample_description_index);
    dr->Get32uSwap(&trex->default_sample_duration);
    dr->Get32uSwap(&trex->default_sample_size);
    dr->Get32uSwap(&trex->default_sample_flags);

    return UMC_OK;
}

Status MP4Splitter::Read_mfhd(DataReader *dr, T_mfhd *mfhd)
{
  dr->Get8u(&mfhd->version);
  mfhd->flags = Get_24(dr);
  dr->Get32uSwap(&mfhd->sequence_number);

  return UMC_OK;
}

} // namespace UMC

#endif
