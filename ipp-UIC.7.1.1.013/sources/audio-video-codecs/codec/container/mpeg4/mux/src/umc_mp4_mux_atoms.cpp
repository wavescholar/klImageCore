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

#include <time.h>
#include "umc_mp4_mux_defs.h"
#include "umc_mp4_mux.h"
#include "vm_time.h"

#include "ippvc.h"

#ifdef OSX
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

namespace UMC
{

    Ipp32s CompareByPTS(const void *pVal0, const void *pVal1)
    {
        sMuxSample *pSmpl0 = (sMuxSample *)pVal0;
        sMuxSample *pSmpl1 = (sMuxSample *)pVal1;
        return (pSmpl0->m_nTimeStamp > pSmpl1->m_nTimeStamp) ? 1 :
            ((pSmpl0->m_nTimeStamp < pSmpl1->m_nTimeStamp) ? -1 : 0);
    }

    Ipp32s CompareByPos(const void *pVal0, const void *pVal1)
    {
        sMuxSample *pSmpl0 = (sMuxSample *)pVal0;
        sMuxSample *pSmpl1 = (sMuxSample *)pVal1;
        return (pSmpl0->m_nPosition > pSmpl1->m_nPosition) ? 1 :
            ((pSmpl0->m_nPosition < pSmpl1->m_nPosition) ? -1 : 0);
    }

    Status MP4Muxer::UpdateSmplDurations()
    {
        Ipp32u i = 0, j = 0;
        for (i = 0; i < m_headerMPEG4.total_tracks; i++)
        {
            sMuxSample *pSamples = m_sTrack[i].m_pSamples;
            if (m_sTrack[i].m_bSortPTSOnExit)
            {
                // sort by PTSes
                qsort(pSamples, m_sTrack[i].m_nSamplesCount, sizeof(sMuxSample), CompareByPTS);
                // update frames durations
                // calculate m_nSttsEntries from the beginning
                if (m_sTrack[i].m_nSamplesCount > 1)
                    m_sTrack[i].m_nSttsEntries = 2; // if >2 samples, the last sample has its own stts entry
                else
                    m_sTrack[i].m_nSttsEntries = 1;
                pSamples[0].m_nDuration = pSamples[1].m_nTimeStamp - pSamples[0].m_nTimeStamp;
                for (j = 2; j < m_sTrack[i].m_nSamplesCount; j++)
                {
                    pSamples[j - 1].m_nDuration = pSamples[j].m_nTimeStamp - pSamples[j - 1].m_nTimeStamp;
                    if (pSamples[j - 1].m_nDuration != pSamples[j - 2].m_nDuration)
                    {
                        m_sTrack[i].m_nSttsEntries++;
                    }
                }
                // restore initial order - m_nPosition is used here (not sure that it's always possible!!!)
                qsort(m_sTrack[i].m_pSamples, m_sTrack[i].m_nSamplesCount, sizeof(sMuxSample), CompareByPos);
            }
        }
        return UMC_OK;
    }

    Ipp32s MP4Muxer::GetMP4VideoTypeID(VideoStreamType type)
    {
        Ipp32s tag;

        if (type == MPEG4_VIDEO) {
          tag = 0x20; // Visual ISO/IEC 14496-2
        } else {
          tag = type;
        }
        return tag;
    }

    Ipp32s MP4Muxer::GetMP4AudioTypeID(AudioStreamType type)
    {
        Ipp32s tag;

        if (type & AAC_AUDIO) {
          tag = 0x40;
        } else
        if ((type & MPEG1_AUDIO) ||
            (type & MPEG2_AUDIO)) {
          tag = 0x6b;
        } else {
          tag = type;
        }
        return tag;
    }

    Ipp32s MP4Muxer::GetMP4StreamType(Ipp32s type)
    {
        Ipp32s tag;

        if (type == MPEG4_VIDEO_QTIME) {
          tag = 0x04; // Visual Stream
        } else
        if (type == UNDEF_AUDIO_SUBTYPE) {
          tag = 0x05;
        } else {
          tag = type;
        }
        return tag;
    }

    Ipp64u MP4Muxer::GetTrakDuration(Ipp32u ntrak)
    {
        Ipp64u duration = 0;
        Ipp32u i;

        for (i = 0; i < m_sTrack[ntrak].m_nSamplesCount; i++)
        {
            duration += (Ipp64u)m_sTrack[ntrak].m_pSamples[i].m_nDuration;
        }

        return duration;    // in nTimeScale
    }

    Ipp32s MP4Muxer::CalculateFragmentSize(TM_moof *moof)
    {
        moof->size_atom = 0;
        moof->size_atom += 8; //moof box
        moof->size_atom += (8 + 1 + 3 + 4); //mfhd

        for (Ipp32u i = 0; i < moof->total_tracks; i++)
        {
            moof->size_atom += 8;                                  //traf
            moof->size_atom += (8 + 1 + 3 + 4);                    //tfhd
            moof->size_atom += (8 + 1 + 3 + 4 + m_sTrack[i].m_nSamplesCount * 4 * 2);  //trun
        }
        return moof->size_atom;
    }

    Ipp32s MP4Muxer::CalculateMvexExtendedSize(TM_mvex* mvex)
    {
        mvex->total_tracks = m_headerMPEG4.total_tracks;

        mvex->size_atom = 0;
        mvex->size_atom += 8; //mvex box

        for (Ipp32u i = 0; i < mvex->total_tracks; i++)
        {
            mvex->size_atom += (8 + 1 + 3 + 4 + 4 + 4 + 4 + 4); //trex
        }

        m_headerMPEG4.size_atom += mvex->size_atom;

        return m_headerMPEG4.size_atom;
    }

    Ipp32s MP4Muxer::CalculateSizes()
    {
        Ipp32u               i, j;
        TM_stsz_data*        stsz;
        TM_stsd_table_data*  stsd_table;
        TM_stsd_data*        stsd;
        TM_stts_data*        stts;
        TM_ctts_data*        ctts;
        TM_stss_data*        stss;
        TM_stbl_data*        stbl;
        TM_stco_data*        stco;
        TM_dref_data*        dref;
        TM_dinf_data*        dinf;
        TM_minf_data*        minf;
        TM_mdhd_data*        mdhd;
        TM_hdlr_data*        hdlr;
        TM_mdia_data*        mdia;
        TM_tkhd_data*        tkhd;
        TM_trak_data*        trak;
        TM_iods_data*        iods;
        TM_mvhd_data*        mvhd;
        TM_vmhd_data*        vmhd;
        TM_smhd_data*        smhd;
        TM_stsc_data*        stsc;

        iods = &m_headerMPEG4.iods;
        iods->size_atom = 8 + 4*4;

        mvhd = &m_headerMPEG4.mvhd;
        mvhd->size_atom = 8 + 4*25;

        m_headerMPEG4.size_atom = 8 + mvhd->size_atom + iods->size_atom;

        if (!m_bMoov)
        {
            m_headerMPEG4.size_atom += m_headerMVEX.size_atom;
        }

        for ( i = 0; i < m_headerMPEG4.total_tracks; i++ )
        {
            stsz = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stsz;

            stsz->size_atom = 8 + 4*3;
            if (!stsz->sample_size)
                stsz->size_atom += m_sTrack[i].m_nSamplesCount * 4;

            stsd = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd;

            stsd->size_atom = 8 + 4*2;

            for ( j = 0; j < stsd->total_entries; j++ )
            {
                stsd_table = &stsd->table[j];

                stsd_table->esds.size_atom = 46 + stsd_table->esds.decoderConfigLen;

                if ( stsd_table->format[0] == 'm' && stsd_table->format[1] == 'p' && stsd_table->format[2] == '4' && stsd_table->format[3] == 'a' )
                    stsd_table->size_atom = 16 + stsd_table->esds.size_atom + 20;
                if ( stsd_table->format[0] == 'm' && stsd_table->format[1] == 'p' && stsd_table->format[2] == '4' && stsd_table->format[3] == 'v' )
                    stsd_table->size_atom = 16 + stsd_table->esds.size_atom + 70;
                if ( stsd_table->format[0] == 'a' && stsd_table->format[1] == 'v' && stsd_table->format[2] == 'c' && stsd_table->format[3] == '1' )
                    stsd_table->size_atom = 16 + stsd_table->esds.size_atom + 32;

                stsd->size_atom += stsd_table->size_atom;
            }

            stts = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stts;
            stts->size_atom = 8 + 4*2 + stts->total_entries * 8;


            ctts = &m_headerMPEG4.trak[i]->mdia.minf.stbl.ctts;
/*            ctts->size_atom = 8 + 4*2 + ctts->total_entries * 8;
*/
            stss = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stss;
            if (m_headerMPEG4.trak[i]->mdia.minf.is_video)
            {
                stss->size_atom = 8 + 4*2 + m_nIDRFrames[i] * 4;
            }

            stco = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stco;
            stco->size_atom = 8 + 4*2 + m_sTrack[i].m_nSamplesCount * 4;

            stsc = &m_headerMPEG4.trak[i]->mdia.minf.stbl.stsc;
            stsc->size_atom = 8 + 4*2 + m_sTrack[i].m_nSamplesCount * 4 * 3;

            stbl = &m_headerMPEG4.trak[i]->mdia.minf.stbl;
            stbl->size_atom = 8 + stts->size_atom + stsd->size_atom + stsz->size_atom + stco->size_atom + stsc->size_atom + ctts->size_atom;

            if (m_headerMPEG4.trak[i]->mdia.minf.is_video && stss->total_entries)
            { /* not mandatory */
                stbl->size_atom += stss->size_atom;
            }

            dref = &m_headerMPEG4.trak[i]->mdia.minf.dinf.dref;
            dref->size_atom = 0x1C; //8 + 4*2;

            dinf = &m_headerMPEG4.trak[i]->mdia.minf.dinf;
            dinf->size_atom = 8 + dref->size_atom;

            vmhd = &m_headerMPEG4.trak[i]->mdia.minf.vmhd;
            vmhd->size_atom = 8 + 12;

            smhd = &m_headerMPEG4.trak[i]->mdia.minf.smhd;
            smhd->size_atom = 8 + 8;

            minf = &m_headerMPEG4.trak[i]->mdia.minf;
            if ( minf->is_video )
                minf->size_atom = 8 + dinf->size_atom + stbl->size_atom + vmhd->size_atom;
            if ( minf->is_audio )
                minf->size_atom = 8 + dinf->size_atom + stbl->size_atom + smhd->size_atom;

            mdhd = &m_headerMPEG4.trak[i]->mdia.mdhd;
            mdhd->size_atom = 8 + 4*6;

            hdlr = &m_headerMPEG4.trak[i]->mdia.hdlr;
            hdlr->size_atom = 58;

            mdia = &m_headerMPEG4.trak[i]->mdia;
            mdia->size_atom = 8 + mdhd->size_atom + minf->size_atom + hdlr->size_atom;

            tkhd = &m_headerMPEG4.trak[i]->tkhd;
            tkhd->size_atom = 8 + 4*21;

            trak = m_headerMPEG4.trak[i];
            trak->size_atom = 8 + tkhd->size_atom + mdia->size_atom;
            m_headerMPEG4.size_atom += trak->size_atom;
        }

        return m_headerMPEG4.size_atom;
    }

    Status MP4Muxer::UpdateHeader()
    {
        Ipp32u i, j;
        vm_timeval val; //file creation time
        Ipp64u start = 0; //file creation time
        vm_status vmRes = VM_OK;

        vmRes = vm_time_gettimeofday(&val, NULL);
        if (vmRes != VM_OK)
          return UMC_ERR_FAILED;
        start = val.tv_sec;
        start += 2082844800; //sec since 104 till 1970

        // MVHD
        m_headerMPEG4.mvhd.creation_time = start;
        m_headerMPEG4.mvhd.modification_time = start;

        UpdateSmplDurations();

        // trak`s
        for (i = 0; i < m_headerMPEG4.total_tracks; i++ )
        {
            // trak->tkhd
            m_headerMPEG4.trak[i]->tkhd.creation_time =
                m_headerMPEG4.trak[i]->tkhd.modification_time = start;
            m_headerMPEG4.trak[i]->tkhd.duration = GetTrakDuration(i);
            // set maximum duration
            if (m_headerMPEG4.trak[i]->tkhd.duration > m_headerMPEG4.mvhd.duration)
                m_headerMPEG4.mvhd.duration = m_headerMPEG4.trak[i]->tkhd.duration;

            // trak->mdia.mdhd
            m_headerMPEG4.trak[i]->mdia.mdhd.creation_time =
                m_headerMPEG4.trak[i]->mdia.mdhd.modification_time = start;
            m_headerMPEG4.trak[i]->mdia.mdhd.duration = GetTrakDuration(i);

            if (IS_VIDEO(i) && UMC::H264_VIDEO == m_pTrackParams[i].info.video->streamType)
            {
                if (m_pH264Header)
                {
                    Ipp8u* pDecInfo = _NEW_n(Ipp8u, 1024 * 1024);
                    UMC_CHECK_PTR(pDecInfo)
                    memset(pDecInfo, 0, 1024 * 1024);

                    Ipp32s pos = 0;
                    pDecInfo[pos] = 1; //version
                    pos++;
                    pDecInfo[pos] = 66; //AVCProfileIndication
                    pos++;
                    pDecInfo[pos] = 224; //profile_compatibility
                    pos++;
                    pDecInfo[pos] = 30; //AVCLevelIndication
                    pos++;
                    pDecInfo[pos] = (Ipp8u)(0xFC | 3); //lengthSizeMinusOne
                    pos++;
                    pDecInfo[pos] = (Ipp8u)(0xE0 | m_pH264Header->m_nSps_num); //numOfSequenceParameterSets
                    pos++;
                    for    (j = 0; j < m_pH264Header->m_nSps_num; j++)
                    {
                        pDecInfo[pos] = (Ipp8u)((m_pH264Header->m_pNal_ut_sps[j].m_nLength & 0xff00) >> 8);
                        pos++;
                        pDecInfo[pos] = (Ipp8u)(m_pH264Header->m_pNal_ut_sps[j].m_nLength & 0xff);
                        pos++;

                        memcpy(&pDecInfo[pos], m_pH264Header->m_pNal_ut_sps[j].m_pNal_ut_sps,
                               m_pH264Header->m_pNal_ut_sps[j].m_nLength);

                        pos += m_pH264Header->m_pNal_ut_sps[j].m_nLength;
                    }

                    pDecInfo[pos] = (Ipp8u)(m_pH264Header->m_nPps_num); //numOfSequenceParameterSets
                    pos++;

                    for    (j = 0; j < m_pH264Header->m_nPps_num; j++)
                    {
                        pDecInfo[pos] = (Ipp8u)((m_pH264Header->m_pNal_ut_pps[j].m_nLength & 0xff00) >> 8);
                        pos++;
                        pDecInfo[pos] = (Ipp8u)(m_pH264Header->m_pNal_ut_pps[j].m_nLength & 0xff);
                        pos++;

                        memcpy(&pDecInfo[pos], m_pH264Header->m_pNal_ut_pps[j].m_pNal_ut_pps,
                               m_pH264Header->m_pNal_ut_pps[j].m_nLength);

                        pos += m_pH264Header->m_pNal_ut_pps[j].m_nLength;
                    }

                    m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[0].esds.decoderConfig = _NEW_n(Ipp8u, pos);
                    memcpy(m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[0].esds.decoderConfig, pDecInfo, pos);
                    m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[0].esds.decoderConfigLen = pos;

                    _SAFE_DELETE_ARRAY(pDecInfo);

                    m_nHeaderSize += (pos);
                }
            } else
            {  // if decSpecificInfo is present offsets should be incremented accordingly
              m_nHeaderSize += m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[0].esds.decoderConfigLen;
            }

            // increase header size depending from stts
            m_nHeaderSize += m_sTrack[i].m_nSttsEntries * 8;

            // count ctts_delta entries if needed
            if (m_sTrack[i].m_nCttsEntries)
            {
              TM_ctts_data *ctts = &m_headerMPEG4.trak[i]->mdia.minf.stbl.ctts;
              Ipp32s k, off, curoff, mDTS;

              sMuxSample *pSamples = m_sTrack[i].m_pSamples;
              Ipp32s  nSamples = m_sTrack[i].m_nSamplesCount;

              m_sTrack[i].m_ctts_delta = 0;
              curoff = 0;
              mDTS = pSamples[0].m_nTimeStamp + pSamples[0].m_nDuration;
              for (k = 1; k < nSamples; k++)
              {
                off = pSamples[k].m_nTimeStamp - mDTS;
                if (off != curoff) {
                  m_sTrack[i].m_nCttsEntries++;
                  curoff = off;
                  if (off < m_sTrack[i].m_ctts_delta)
                  {
                    m_sTrack[i].m_ctts_delta = off;
                  }
                }
                mDTS += pSamples[k].m_nDuration;
              }

              ctts->size_atom = 8 + 4*2 + m_sTrack[i].m_nCttsEntries * 8;
              m_nHeaderSize += ctts->size_atom;
            }
        }

        return UMC_OK;
    }

    Status MP4Muxer::InitHeader()
    {
        Ipp32u    i, j;

        // MVHD
        m_headerMPEG4.mvhd.version = 0;
        m_headerMPEG4.mvhd.flags = 0;
        m_headerMPEG4.mvhd.nTimeScale = nTimeScale;
        m_headerMPEG4.mvhd.duration = 0;
        m_headerMPEG4.mvhd.next_track_id = m_headerMPEG4.total_tracks + 1;

        // IODS
        m_headerMPEG4.iods.version = 0;
        m_headerMPEG4.iods.flags = 0;
        m_headerMPEG4.iods.audioProfileId = 0;
        m_headerMPEG4.iods.videoProfileId = 0;

        // trak`s
        for ( i = 0; i < m_headerMPEG4.total_tracks; i++ )
        {
            // trak->tkhd
            m_headerMPEG4.trak[i]->tkhd.version = 0;
            m_headerMPEG4.trak[i]->tkhd.flags = 0;

            if (IS_VIDEO(i))
            {
                m_headerMPEG4.trak[i]->tkhd.is_audio = 0;
                m_headerMPEG4.trak[i]->tkhd.is_video = 1;
                m_headerMPEG4.trak[i]->tkhd.track_width  = (Ipp32f)(m_pTrackParams[i].info.video->videoInfo.m_iWidth << 16);
                m_headerMPEG4.trak[i]->tkhd.track_height = (Ipp32f)(m_pTrackParams[i].info.video->videoInfo.m_iHeight << 16);

                vm_string_strncpy(m_headerMPEG4.trak[i]->mdia.hdlr.component_type, VM_STRING("vide"), 4);
                vm_string_strcpy(m_headerMPEG4.trak[i]->mdia.hdlr.component_name, VM_STRING("Intel Video Media Handler"));
            }
            else
            {
                m_headerMPEG4.trak[i]->tkhd.is_audio = 1;
                m_headerMPEG4.trak[i]->tkhd.is_video = 0;
                m_headerMPEG4.trak[i]->tkhd.track_width = 0;
                m_headerMPEG4.trak[i]->tkhd.track_height = 0;

                vm_string_strncpy(m_headerMPEG4.trak[i]->mdia.hdlr.component_type, VM_STRING("soun"), 4);
                vm_string_strcpy(m_headerMPEG4.trak[i]->mdia.hdlr.component_name, VM_STRING("Intel Sound Media Handler"));
            }

            // trak->mdia
            // trak->mdia.mdhd
            m_headerMPEG4.trak[i]->mdia.mdhd.version = 0;
            m_headerMPEG4.trak[i]->mdia.mdhd.flags = 0;
            m_headerMPEG4.trak[i]->mdia.mdhd.nTimeScale = nTimeScale;
            m_headerMPEG4.trak[i]->mdia.mdhd.language = 0;

            // trak->mdia.hdlf
            m_headerMPEG4.trak[i]->mdia.hdlr.version = 0;
            m_headerMPEG4.trak[i]->mdia.hdlr.flags   = 0;

            // trak->mdia.minf
            m_headerMPEG4.trak[i]->mdia.minf.is_video = 0;
            m_headerMPEG4.trak[i]->mdia.minf.is_hint = 0;
            m_headerMPEG4.trak[i]->mdia.minf.is_audio = 0;

            // trak->mdia.minf.vmhd
            m_headerMPEG4.trak[i]->mdia.minf.vmhd.version = 0;
            m_headerMPEG4.trak[i]->mdia.minf.vmhd.flags = 0;

            // trak->mdia.minf.smhd
            m_headerMPEG4.trak[i]->mdia.minf.smhd.version = 0;
            m_headerMPEG4.trak[i]->mdia.minf.smhd.flags = 0;

            // trak->mdia.minf.dinf
            // trak->mdia.minf.dinf.dref
            m_headerMPEG4.trak[i]->mdia.minf.dinf.dref.version = 0;
            m_headerMPEG4.trak[i]->mdia.minf.dinf.dref.flags = 0;
            m_headerMPEG4.trak[i]->mdia.minf.dinf.dref.total_entries = 1;

            // trak->mdia.minf.stbl.stsd
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.version = 0;
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.flags = 0;
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.total_entries = 1;

            // trak->mdia.minf.stbl.stss
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stss.version = 0;
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stss.flags = 0;
            m_headerMPEG4.trak[i]->mdia.minf.stbl.stss.total_entries = 0;

            m_headerMPEG4.trak[i]->mdia.minf.stbl.stsz.sample_size = 0;

            m_headerMPEG4.trak[i]->mdia.minf.stbl.ctts.size_atom = 0;

            for ( j = 0; j < m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.total_entries; j++ )
            {
                TM_stsd_table_data& table = m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[j];

                if (IS_VIDEO(i))
                {
                    if (UMC::H264_VIDEO == m_pTrackParams[i].info.video->streamType)
                    {
                        table.format[0] = 'a';
                        table.format[1] = 'v';
                        table.format[2] = 'c';
                        table.format[3] = '1';
                    }
                    else
                    {
                        table.format[0] = 'm';
                        table.format[1] = 'p';
                        table.format[2] = '4';
                        table.format[3] = 'v';
                    }
                    m_headerMPEG4.trak[i]->mdia.minf.is_video = 1;
                    table.width = (Ipp16u)m_pTrackParams[i].info.video->videoInfo.m_iWidth;
                    table.height = (Ipp16u)m_pTrackParams[i].info.video->videoInfo.m_iHeight;
                    table.esds.maxBitrate = m_pTrackParams[i].info.video->iBitrate;
                    table.esds.avgBitrate = m_pTrackParams[i].info.video->iBitrate;
                    table.esds.objectTypeID = GetMP4VideoTypeID(m_pTrackParams[i].info.video->streamType);
                    table.esds.streamType = GetMP4StreamType(m_pTrackParams[i].info.video->streamSubtype);
                    table.esds.streamType = (table.esds.streamType << 2) | 0x1; // 6 bits of type + 01
                }
                else
                {
                    m_headerMPEG4.trak[i]->mdia.minf.is_audio = 1;
                    table.format[0] = 'm';
                    table.format[1] = 'p';
                    table.format[2] = '4';
                    table.format[3] = 'a';
                    table.channels = (Ipp16u)(m_pTrackParams[i].info.audio->audioInfo.m_iChannels);
                    table.sample_size = (Ipp16u)(m_pTrackParams[i].info.audio->audioInfo.m_iBitPerSample);
                    table.sample_rate = m_pTrackParams[i].info.audio->audioInfo.m_iSampleFrequency;
                    table.esds.maxBitrate = m_pTrackParams[i].info.audio->iBitrate;
                    table.esds.avgBitrate = m_pTrackParams[i].info.audio->iBitrate;
                    table.esds.objectTypeID = GetMP4AudioTypeID(m_pTrackParams[i].info.audio->streamType);
                    table.esds.streamType = GetMP4StreamType(m_pTrackParams[i].info.audio->streamSubtype);
                    table.esds.streamType = (table.esds.streamType << 2) | 0x1; // 6 bits of type + 01
                }

                m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[j].data_reference = 1;
            }

            m_headerMPEG4.trak[i]->tkhd.flags = 1;
            m_headerMPEG4.trak[i]->tkhd.track_id = i + 1;
        }

        return UMC_OK;
    };

    Status MP4Muxer::InitStsz(Ipp32s nTrack)
    {
        TM_stsz_data& stsz  = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.stsz;
        stsz.sample_size    = 0;
        stsz.version        = 0;
        stsz.flags          = 0;
        stsz.total_entries  = m_sTrack[nTrack].m_nSamplesCount;
        stsz.table          = _NEW_n(TM_stsz_table_data, m_sTrack[nTrack].m_nSamplesCount);

        return UMC_OK;
    }

    Status MP4Muxer::InitStco(Ipp32s nTrack)
    {
        TM_stco_data& stco = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.stco;
        stco.version       = 0;
        stco.flags         = 0;
        stco.total_entries = m_sTrack[nTrack].m_nSamplesCount;
        stco.table         = _NEW_n(TM_stco_table_data, m_sTrack[nTrack].m_nSamplesCount);

        return UMC_OK;
    }

    Status MP4Muxer::InitStts(Ipp32s nTrack)
    {
        TM_stts_data& stts = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.stts;

        stts.version       = 0;
        stts.flags         = 0;
        stts.total_entries = m_sTrack[nTrack].m_nSttsEntries;

        stts.table = _NEW_n(TM_stts_table_data, m_sTrack[nTrack].m_nSttsEntries);

        return UMC_OK;
    }

    Status MP4Muxer::InitCtts(Ipp32s nTrack)
    {
        TM_ctts_data& ctts = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.ctts;

        ctts.version       = 0;
        ctts.flags         = 0;
        ctts.total_entries = m_sTrack[nTrack].m_nCttsEntries;

        ctts.table = _NEW_n(TM_ctts_table_data, m_sTrack[nTrack].m_nCttsEntries);

        return UMC_OK;
    }

    Status MP4Muxer::InitStss(Ipp32s nTrack)
    {
        TM_stss_data& stss = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.stss;

        stss.version       = 0;
        stss.flags         = 0;
        stss.total_entries = 0;
        stss.table         = _NEW_n(TM_stss_table_data, m_nIDRFrames[nTrack]);

        return UMC_OK;
    }

    Status MP4Muxer::InitStsc(Ipp32s nTrack)
    {
        TM_stsc_data& stsc = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl.stsc;

        stsc.version       = 0;
        stsc.flags         = 0;
        stsc.total_entries = m_sTrack[nTrack].m_nSamplesCount;
        stsc.table         = _NEW_n(TM_stsc_table_data, m_sTrack[nTrack].m_nSamplesCount);

        return UMC_OK;
    }

    Status MP4Muxer::InitAtoms()
    {
        Ipp32u nTrack = 0;
        Ipp32u i = 0;
        Ipp32u j = 0, dur;

/*
        for (nTrack = 0; nTrack < m_headerMPEG4.total_tracks; nTrack++)
        {
            TM_stbl_data& stbl = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl;
            if (!m_sTrack[nTrack].m_nCttsEntries) {  //  throw away ctts header
              m_nHeaderSize -= stbl.ctts.size_atom;
            }
        }
*/

        for (nTrack = 0; nTrack < m_headerMPEG4.total_tracks; nTrack++)
        {
            InitStsz(nTrack);
            InitStco(nTrack);
            InitStts(nTrack);
            if (m_sTrack[nTrack].m_nCttsEntries) {
              InitCtts(nTrack);
            }
            InitStsc(nTrack);
            InitStss(nTrack);

            TM_stbl_data& stbl = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl;

            for (i = 0; i < m_sTrack[nTrack].m_nSamplesCount; i++)
            {
                stbl.stsz.table[i].size     = m_sTrack[nTrack].m_pSamples[i].m_nSize;
                stbl.stco.table[i].offset   = (Ipp32u)m_sTrack[nTrack].m_pSamples[i].m_nPosition;
                stbl.stsc.table[i].chunk    =  m_sTrack[nTrack].m_pSamples[i].m_nFirstChunk;
                stbl.stsc.table[i].samples  =  m_sTrack[nTrack].m_pSamples[i].m_nSamplesPerChunk;
                stbl.stsc.table[i].id       = m_sTrack[nTrack].m_pSamples[i].m_nId;

                if (m_headerMPEG4.trak[nTrack]->mdia.minf.is_video)
                {
                    if (I_PICTURE == m_sTrack[nTrack].m_pSamples[i].m_frameTMype)
                    {
                        stbl.stss.table[stbl.stss.total_entries].sample = i;
                        stbl.stss.total_entries++;
                    }
                }
            }

            // filling stts

            if (m_sTrack[nTrack].m_nSamplesCount > 0)
            {
                dur = m_sTrack[nTrack].m_pSamples[0].m_nDuration;
                stbl.stts.table[0].sample_count = 1;
                stbl.stts.table[0].sample_duration = dur;
                for (i = 1, j = 0; i < m_sTrack[nTrack].m_nSamplesCount - 1; i++)
                {
                    if (dur != m_sTrack[nTrack].m_pSamples[i].m_nDuration)
                    {
                        j++;
                        stbl.stts.table[j].sample_count = 1;
                        dur = m_sTrack[nTrack].m_pSamples[i].m_nDuration;
                        stbl.stts.table[j].sample_duration = dur;
                    } else
                    {
                        stbl.stts.table[j].sample_count++;
                    }
                }

                if (i < m_sTrack[nTrack].m_nSamplesCount)
                {
                  if (m_sTrack[nTrack].m_pSamples[i].m_nDuration == 0) // last sample
                  {
                      m_sTrack[nTrack].m_pSamples[i].m_nDuration = m_sTrack[nTrack].m_pSamples[i - 1].m_nDuration;
                  }
                  stbl.stts.table[j + 1].sample_count = 1;
                  stbl.stts.table[j + 1].sample_duration = m_sTrack[nTrack].m_pSamples[i].m_nDuration;
                }
            }

            // filling ctts if needed
            if (m_sTrack[nTrack].m_nCttsEntries) {
              Ipp32s mDTS, curoff, off;
              sMuxSample *pSamples = m_sTrack[nTrack].m_pSamples;
              Ipp32u  nSamples = m_sTrack[nTrack].m_nSamplesCount;

              stbl.ctts.table[0].sample_count = 1;
              stbl.ctts.table[0].sample_offset = -m_sTrack[nTrack].m_ctts_delta;
              mDTS = pSamples[0].m_nTimeStamp + pSamples[0].m_nDuration;
              curoff = -m_sTrack[nTrack].m_ctts_delta;

              for (i = 1, j = 0; i < nSamples; i++) {
                off = pSamples[i].m_nTimeStamp - mDTS - m_sTrack[nTrack].m_ctts_delta;
                if (curoff != off) {
                  j++;
                  stbl.ctts.table[j].sample_count = 1;
                  stbl.ctts.table[j].sample_offset = off;
                  curoff = off;
                } else {
                  stbl.ctts.table[j].sample_count++;
                }
                mDTS += pSamples[i].m_nDuration;
              }
           }

        }

        return UMC_OK;
    }

    static Ipp32s FindStartCode(Ipp8u * (&pb), size_t& nSize)
    {
        // there is no data
        if ((Ipp32s) nSize < 4)
            return 0;

        // find start code
        while ((4 <= nSize) && ((0 != pb[0]) || (0 != pb[1]) || (1 != pb[2])))
        {
            pb += 1;
            nSize -= 1;
        }

        if (4 <= nSize)
            return ((pb[0] << 24) | (pb[1] << 16) | (pb[2] << 8) | (pb[3]));

        return 0;
    } // Ipp32s FindStartCode(Ipp8u * (&pb), size_t &nSize)

    Status MP4Muxer::GetPSid(Ipp32s *pID, Ipp8s *pSrc)
    {
        Ipp32u *pSrc_32 = NULL;
        Ipp32s bitOff = 0;
        Ipp32u buf[2], *pbuf;

        pSrc_32 = (Ipp32u *)((long)pSrc & ~3);
        bitOff = 31 - ((Ipp32u)(pSrc - (Ipp8s *)pSrc_32) * 8);
        buf[0] = LITTLE_ENDIAN_SWAP32(pSrc_32[0]);
        buf[1] = LITTLE_ENDIAN_SWAP32(pSrc_32[1]);
        pbuf = buf;

        IppStatus ippRes = ippiDecodeExpGolombOne_H264_1u32s(&pbuf, &bitOff, pID, false);

        if (ippStsNoErr > ippRes)
            return UMC_ERR_FAILED;
        else
            return UMC_OK;
    }


    Status MP4Muxer::CheckStartCode(Ipp32s nNAL, Ipp8s* pData, Ipp32u nPos, MediaData* mH264Data, bool bLast)
    {
        Ipp32u length = 0, i = 0;
        Status umcRes = UMC_ERR_FAILED;

        if (bLast)
        {
            length = (Ipp32u)mH264Data->GetDataSize() - nPos;
        }
        else
        {
            length = (Ipp32u)((Ipp8s*)pData - (Ipp8s*)mH264Data->GetBufferPointer()) - nPos;
        }

        if (NAL_UT_SPS == nNAL || NAL_UT_SPS_EX == nNAL)
        {
            Ipp32s sps_id, sps_id_new;
            /* we should skip 4 bytes (1:nalu type 3:to skip) and get Golomb element */
            umcRes = GetPSid(&sps_id_new, (Ipp8s*)mH264Data->GetBufferPointer() + nPos + 4);
            if (umcRes != UMC_OK)
                return umcRes;
            for (i = 0; i < m_pH264Header->m_nSps_num; i++)
            {
                umcRes = GetPSid(&sps_id, m_pH264Header->m_pNal_ut_sps[i].m_pNal_ut_sps + 4);
                if (umcRes != UMC_OK)
                    return umcRes;
                if (sps_id == sps_id_new)
                    break;
            }
            if ((i == m_pH264Header->m_nSps_num) && (m_pH264Header->m_nSps_num < 32))
            {   /* copy only SPSes with different id */
                m_pH264Header->m_pNal_ut_sps[m_pH264Header->m_nSps_num].m_pNal_ut_sps = _NEW_n(Ipp8s, length);
                m_pH264Header->m_pNal_ut_sps[m_pH264Header->m_nSps_num].m_nLength = length;
                memcpy(m_pH264Header->m_pNal_ut_sps[m_pH264Header->m_nSps_num].m_pNal_ut_sps,
                    (Ipp8s*)mH264Data->GetBufferPointer() + nPos, length);
                m_pH264Header->m_nSps_num++;
            }

            return UMC_HEADER;
        }

        if (NAL_UT_PPS == nNAL)
        {
            Ipp32s pps_id, pps_id_new;
            /* we should skip 1 bytes (nalu type) and get Golomb element */
            umcRes = GetPSid(&pps_id_new, (Ipp8s*)mH264Data->GetBufferPointer() + nPos + 1);
            if (umcRes != UMC_OK)
                return umcRes;
            for (i = 0; i < m_pH264Header->m_nPps_num; i++)
            {
                umcRes = GetPSid(&pps_id, m_pH264Header->m_pNal_ut_pps[i].m_pNal_ut_pps + 1);
                if (umcRes != UMC_OK)
                    return umcRes;
                if (pps_id == pps_id_new)
                    break;
            }
            if ((i == m_pH264Header->m_nPps_num) && (m_pH264Header->m_nPps_num < 256))
            {   /* copy only PPSes with different id */
                m_pH264Header->m_pNal_ut_pps[m_pH264Header->m_nPps_num].m_pNal_ut_pps = _NEW_n(Ipp8s, length);
                m_pH264Header->m_pNal_ut_pps[m_pH264Header->m_nPps_num].m_nLength = length;
                memcpy(m_pH264Header->m_pNal_ut_pps[m_pH264Header->m_nPps_num].m_pNal_ut_pps,
                    (Ipp8s*)mH264Data->GetBufferPointer() + nPos, length);
                m_pH264Header->m_nPps_num++;
            }

            return UMC_HEADER;
        }

        if (NAL_UT_SLICE == nNAL || NAL_UT_IDR_SLICE == nNAL || NAL_UT_AUXILIARY == nNAL)
        {
            Ipp32u s_length = BSWAP32(length);
            memcpy(m_pH264Data + m_nH264DataOffset, (Ipp8s*)&s_length, 4);
            m_nH264DataOffset +=4;
            memcpy(m_pH264Data + m_nH264DataOffset, (Ipp8s*)mH264Data->GetBufferPointer() + nPos, length);
            m_nH264DataOffset +=length;
            return UMC_DATA;
        }

        return UMC_NO_SUCH_CODE;
    };

    MediaData* MP4Muxer::TransformH264Frame(MediaData* mH264Data)
    {
        Ipp8u *pData = (Ipp8u *) mH264Data->GetBufferPointer();
        size_t nLen = mH264Data->GetDataSize();

        bool bStop = false;
        Ipp32s pos = 0;
        Ipp32s nNAL = 0;

        bool bIsData = false;
        Status umcRes = 0;

        //FILE* f2;
        //f2 = fopen("1.h264", "ab+");
        //fwrite(mH264Data->GetDataPointer(), 1, mH264Data->GetDataSize(), f2);
        //fclose(f2);

        if( m_iH264DataCurSize < nLen+10000 )
        {
            _SAFE_DELETE_ARRAY(m_pH264Data);
            m_iH264DataCurSize = (Ipp32u)nLen+10000; //Added ~10K, because cut of NAL unit takes 3 bytes away, but muxer adds size of NAL unit that is 4 bytest ( 1 byte difference )
            _SAFE_NEW_n(m_pH264Data, Ipp8s, m_iH264DataCurSize);
            m_mH264DataOut->SetBufferPointer((Ipp8u*)m_pH264Data, m_iH264DataCurSize);
        }
       // memset(m_pH264Data, 0, m_iH264DataCurSize);

        _SAFE_NEW_n(m_pH264Header->m_pNal_ut_sps, sNal_ut_sps, 32);
        _SAFE_NEW_n(m_pH264Header->m_pNal_ut_pps, sNal_ut_pps, 256);

        while ((Ipp8s*)pData < ((Ipp8s*)mH264Data->GetBufferPointer() + mH264Data->GetDataSize()))
        {
            Ipp32s iCode = FindStartCode(pData, nLen);

            // check start codes
/*
            if (NAL_UT_SPS == (iCode & NAL_UNITTYPE_BITS)     ||
                NAL_UT_SPS_EX == (iCode & NAL_UNITTYPE_BITS)  ||
                NAL_UT_PPS == (iCode & NAL_UNITTYPE_BITS)     ||
                NAL_UT_SLICE == (iCode & NAL_UNITTYPE_BITS)   ||
                NAL_UT_IDR_SLICE == (iCode & NAL_UNITTYPE_BITS) ||
                NAL_UT_AUXILIARY == (iCode & NAL_UNITTYPE_BITS) ||
                NAL_UT_SEI == (iCode & NAL_UNITTYPE_BITS) ||
                NAL_UT_SEI == (iCode & NAL_UNITTYPE_BITS))
*/
            if (iCode != 0)
            {
                if (bStop)
                {
                    umcRes = CheckStartCode(nNAL, (Ipp8s*)pData, pos, mH264Data, 0);
                }

                bStop = true;
            }
            else
            {
                if (bStop)
                {
                    umcRes = CheckStartCode(nNAL, (Ipp8s*)pData, pos, mH264Data, 1);
                }
                bStop = false;
            }

            nNAL = iCode & NAL_UNITTYPE_BITS;
            pos = (Ipp32u)((Ipp8s*)pData - (Ipp8s*)mH264Data->GetBufferPointer()) + 3;
            pData +=3;
            nLen -= 3;

            if (UMC_DATA == umcRes)
            {
                bIsData = true;
            }
        }

        // DataSize sets to zero if SPS/PPS only (from Splitter)
        m_mH264DataOut->SetDataSize(m_nH264DataOffset);
        m_mH264DataOut->m_frameType = mH264Data->m_frameType;
        m_mH264DataOut->m_fPTSStart = mH264Data->m_fPTSStart;
        m_mH264DataOut->m_fPTSEnd   = mH264Data->m_fPTSEnd;

        m_nH264DataOffset = 0;

        return m_mH264DataOut;
    }

    Status MP4Muxer::InitEsds()
    {
        for (Ipp32u i = 0; i < m_headerMPEG4.total_tracks; i++)
        {
            Ipp32s nIndex = m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.total_entries - 1; // now total_entries = 1, so nIndex = 0;

            TM_esds_data& esds = m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table[nIndex].esds;

            if (UMC::H264_VIDEO == m_pTrackParams[i].info.video->streamType) {
              if (!m_mH264DataOut)
                  m_mH264DataOut = new MediaData;

              if (!m_pH264Header) {
                  m_pH264Header = new sH264_Header;
                  memset(m_pH264Header, 0, sizeof(sH264_Header));
              }
            }

            esds.objectTypeID = 0;
            esds.streamType = 0;
            esds.decoderConfigLen = 0;
            esds.decoderConfig = NULL;


            esds.version = 0;
            esds.flags = 0;
        }

        return UMC_OK;
    };

    Status MP4Muxer::InitMoof()
    {
        Ipp32u nTrack = 0;
        Ipp32u i = 0;

        m_headerMoof.sequence_number++;

        m_headerMoof.size_atom = m_nHeaderSize;

        if (m_headerMoof.trun)
        {
            for (nTrack = 0; nTrack < m_headerMPEG4.total_tracks; nTrack++)
            {
                _SAFE_DELETE_ARRAY(m_headerMoof.trun[nTrack].trun);
            }
        }
        else
        {
             m_headerMoof.trun = _NEW_n(TM_table_trun, m_headerMPEG4.total_tracks);
             UMC_CHECK_PTR(m_headerMoof.trun)
        }

        for ( nTrack = 0; nTrack < m_headerMPEG4.total_tracks; nTrack++)
        {
            TM_table_trun& trunTrack = m_headerMoof.trun[nTrack];

            trunTrack.entry_count = m_sTrack[nTrack].m_nSamplesCount;
            trunTrack.trun = _NEW_n(TM_trun_table_data, trunTrack.entry_count);
            UMC_CHECK_PTR(trunTrack.trun)

            for (i = 0; i < m_sTrack[nTrack].m_nSamplesCount; i++)
            {
                if (i != m_sTrack[nTrack].m_nSamplesCount - 1) {
                    trunTrack.trun[i].sample_duration = m_sTrack[nTrack].m_pSamples[i+1].m_nTimeStamp - m_sTrack[nTrack].m_pSamples[i].m_nTimeStamp;
                } else {
                  if (i != 0) {
                    trunTrack.trun[i].sample_duration = trunTrack.trun[i-1].sample_duration;  //last sample
                  }
                }
//                trunTrack.trun[i].sample_duration = m_sTrack[nTrack].m_pSamples[i].m_nTimeStamp;
                trunTrack.trun[i].sample_size = m_sTrack[nTrack].m_pSamples[i].m_nSize;
            }
        }

        return UMC_OK;
    };

}

#endif
