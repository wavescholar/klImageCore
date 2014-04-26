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

#include "vm_thread.h"
#include "vm_types.h"
#include "vm_time.h"
#include "umc_media_data.h"
#include "umc_sample_buffer.h"
#include "mp4cmn_config.h"
#include "bstream.h"
#include "umc_mp4_splitter.h"
#include "umc_automatic_mutex.h"
#include "umc_structures.h"


using namespace UMC;

MP4Splitter::MP4Splitter(): IndexSplitter()
{
    m_nFragPosEnd       = 0;
    m_pLastPTS          = NULL;
    m_pInitMoofThread   = NULL;
    m_bFlagStopInitMoof = false;
}

MP4Splitter::~MP4Splitter()
{
    Close();
}

Status MP4Splitter::CheckInit()
{
    // already initialized
    if ((m_pReader == NULL) ||
        (m_pTrackIndex == NULL) ||
        (m_ppMediaBuffer == NULL) ||
        (m_ppLockedFrame == NULL) ||
        (m_pIsLocked == NULL) ||
        (m_pInfo == NULL))
    return UMC_ERR_NOT_INITIALIZED;

    return UMC_OK;
}

Status MP4Splitter::CheckMoofInit()
{
    for(Ipp32u iES = 0; iES < m_headerMPEG4.moov.mvex.total_tracks; iES++)
    {
        if (!m_headerMPEG4.moov.trak[iES]->mdia.minf.stbl.stsz.max_sample_size)
            return UMC_ERR_NOT_INITIALIZED;
    }

    return UMC_OK;
}

Status MP4Splitter::Clear_track(T_trak_data* pTrak)
{
  Ipp32u i, j;

    if (pTrak)
    {
        for (i = 0; i < pTrak->mdia.minf.stbl.stsd.total_entries; i++)
        {
            if(pTrak->mdia.minf.stbl.stsd.table[i].esds.decoderConfig)
            {
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].esds.decoderConfig);
                pTrak->mdia.minf.stbl.stsd.table[i].esds.decoderConfig    = NULL;
            }
            if(pTrak->mdia.minf.stbl.stsd.table[i].avcC.decoderConfig)
            {
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].avcC.decoderConfig);
                pTrak->mdia.minf.stbl.stsd.table[i].avcC.decoderConfig    = NULL;
            }
            if(pTrak->mdia.minf.stbl.stsd.table[i].mvcC.decoderConfig)
            {
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].mvcC.decoderConfig);
                pTrak->mdia.minf.stbl.stsd.table[i].mvcC.decoderConfig    = NULL;
            }
            if(pTrak->mdia.minf.stbl.stsd.table[i].svcC.decoderConfig)
            {
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].svcC.decoderConfig);
                pTrak->mdia.minf.stbl.stsd.table[i].svcC.decoderConfig    = NULL;
            }
            if(pTrak->mdia.minf.stbl.stsd.table[i].damr.decoderConfig)
            {
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].damr.decoderConfig);
                pTrak->mdia.minf.stbl.stsd.table[i].damr.decoderConfig    = NULL;
            }
            if(pTrak->mdia.minf.stbl.stsd.table[i].vwid.views)
            {
                for(j = 0; j < pTrak->mdia.minf.stbl.stsd.table[i].vwid.num_views; j++)
                {
                    if(pTrak->mdia.minf.stbl.stsd.table[i].vwid.views[j].ref_view_id)
                    {
                        ippFree(pTrak->mdia.minf.stbl.stsd.table[i].vwid.views[j].ref_view_id);
                        pTrak->mdia.minf.stbl.stsd.table[i].vwid.views[j].ref_view_id = NULL;
                    }
                }
                ippFree(pTrak->mdia.minf.stbl.stsd.table[i].vwid.views);
                pTrak->mdia.minf.stbl.stsd.table[i].vwid.views = NULL;
            }
        }

        if(pTrak->mdia.minf.stbl.stsz.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stsz.table);
            pTrak->mdia.minf.stbl.stsz.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.stsc.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stsc.table);
            pTrak->mdia.minf.stbl.stsc.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.ctts.table)
        {
            ippFree(pTrak->mdia.minf.stbl.ctts.table);
            pTrak->mdia.minf.stbl.ctts.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.co64.table)
        {
            ippFree(pTrak->mdia.minf.stbl.co64.table);
            pTrak->mdia.minf.stbl.co64.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.stco.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stco.table);
            pTrak->mdia.minf.stbl.stco.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.stts.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stts.table);
            pTrak->mdia.minf.stbl.stts.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.stss.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stss.table);
            pTrak->mdia.minf.stbl.stss.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.stsd.table)
        {
            ippFree(pTrak->mdia.minf.stbl.stsd.table);
            pTrak->mdia.minf.stbl.stsd.table = NULL;
        }

        if(pTrak->mdia.minf.stbl.sbgp.entries)
        {
            ippFree(pTrak->mdia.minf.stbl.sbgp.entries);
            pTrak->mdia.minf.stbl.sbgp.entries = NULL;
        }

        if(pTrak->mdia.minf.stbl.sgpd.entries)
        {
            ippFree(pTrak->mdia.minf.stbl.sgpd.entries);
            pTrak->mdia.minf.stbl.sgpd.entries = NULL;
        }

        if(pTrak->mdia.minf.dinf.dref.table)
        {
            for(i = 0; i < pTrak->mdia.minf.dinf.dref.total_entries; i++)
            {
                if(pTrak->mdia.minf.dinf.dref.table[i].data_reference)
                {
                    ippFree(pTrak->mdia.minf.dinf.dref.table[i].data_reference);
                    pTrak->mdia.minf.dinf.dref.table[i].data_reference = NULL;
                }
            }
            ippFree(pTrak->mdia.minf.dinf.dref.table);
            pTrak->mdia.minf.dinf.dref.table = NULL;
        }

        if(pTrak->mdia.minf.mvci.mvcg.entries)
        {
            ippFree(pTrak->mdia.minf.mvci.mvcg.entries);
            pTrak->mdia.minf.mvci.mvcg.entries = NULL;
        }

        if(pTrak->mdia.minf.mvci.mvcg.buff.operating_points)
        {
            ippFree(pTrak->mdia.minf.mvci.mvcg.buff.operating_points);
            pTrak->mdia.minf.mvci.mvcg.buff.operating_points = NULL;
        }

        if(pTrak->mdia.minf.mvci.mvcg.mvra.common_attributes)
        {
            ippFree(pTrak->mdia.minf.mvci.mvcg.mvra.common_attributes);
            pTrak->mdia.minf.mvci.mvcg.mvra.common_attributes = NULL;
        }

        if(pTrak->mdia.minf.mvci.mvcg.mvra.differentiating_attributes)
        {
            ippFree(pTrak->mdia.minf.mvci.mvcg.mvra.differentiating_attributes);
            pTrak->mdia.minf.mvci.mvcg.mvra.differentiating_attributes = NULL;
        }

        if(pTrak->mdia.minf.mvci.swtc.mvra.common_attributes)
        {
            ippFree(pTrak->mdia.minf.mvci.swtc.mvra.common_attributes);
            pTrak->mdia.minf.mvci.swtc.mvra.common_attributes = NULL;
        }

        if(pTrak->mdia.minf.mvci.swtc.mvra.differentiating_attributes)
        {
            ippFree(pTrak->mdia.minf.mvci.swtc.mvra.differentiating_attributes);
            pTrak->mdia.minf.mvci.swtc.mvra.differentiating_attributes = NULL;
        }

        if(pTrak->mdia.minf.mvci.swtc.multiview_group_id)
        {
            ippFree(pTrak->mdia.minf.mvci.swtc.multiview_group_id);
            pTrak->mdia.minf.mvci.swtc.multiview_group_id = NULL;
        }

        ippFree(pTrak);
    }
    return UMC_OK;
}

void MP4Splitter::FillSampleSizeAndType(T_trak_data *trak, IndexFragment &frag)
{
    Ipp32u i;
    T_stsz_data stsz = trak->mdia.minf.stbl.stsz;
    T_stss_data stss = trak->mdia.minf.stbl.stss;

    for (i = 0; i < (Ipp32u)frag.iNOfEntries; i++)
    {
        if(stsz.sample_size == 0)
            frag.pEntryArray[i].uiSize = trak->mdia.minf.stbl.stsz.table[i].size;
        else
            frag.pEntryArray[i].uiSize = stsz.sample_size;

        if (stss.table == NULL)
            frag.pEntryArray[i].uiFlags = I_PICTURE;
    }

    if (stss.table)
    {
        for (i = 0; i < stss.total_entries; i++)
            frag.pEntryArray[stss.table[i].sample_number - 1].uiFlags = I_PICTURE;
    }
}

void MP4Splitter::FillSamplePos(T_trak_data *trak, IndexFragment &frag)
{
    Ipp32u i;

    // chunk offset box (32 bit version)
    T_stco_data& stco = trak->mdia.minf.stbl.stco;
    // chunk offset box (64 bit version)
    T_co64_data& co64 = trak->mdia.minf.stbl.co64;
    // sample to chunk box
    T_stsc_data& stsc = trak->mdia.minf.stbl.stsc;

    Ipp64u nSampleOffset = 0;
    Ipp32u stco_i = 1;
    Ipp32u stsc_i = 0;
    Ipp32u samp_i = 0;

    while (samp_i < (Ipp32u)frag.iNOfEntries)
    {
        if (stco.total_entries)
        {
            if(stco_i > stco.total_entries)
                break;
            nSampleOffset = stco.table[stco_i - 1].offset;
        }
        else
        {
            if(stco_i > co64.total_entries)
                break;
            nSampleOffset = co64.table[stco_i - 1].offset;
        }

        /* skipping invalid entries */
        while (stsc_i < stsc.total_entries - 1)
        {
            if (stsc.table[stsc_i + 1].chunk - stsc.table[stsc_i].chunk <= 0)
                stsc_i++;
            else
                break;
        }

        for (i = 0; i < stsc.table[stsc_i].samples; i++)
        {
            frag.pEntryArray[samp_i].stPosition = nSampleOffset;
            nSampleOffset += frag.pEntryArray[samp_i].uiSize;
            samp_i++;
            if(samp_i >= (Ipp32u)frag.iNOfEntries)
                break;
        }

        stco_i++;
        if ((stsc_i < stsc.total_entries - 1) && (stco_i == stsc.table[stsc_i + 1].chunk))
            stsc_i++;
    }
}

void MP4Splitter::FillSampleTimeStamp(T_trak_data *trak, IndexFragment &frag)
{
    // decoding time to sample box
    T_stts_data& stts = trak->mdia.minf.stbl.stts;
    // composition time to sample box
    T_ctts_data& ctts = trak->mdia.minf.stbl.ctts;
    Ipp32s nTimeScale = trak->mdia.mdhd.time_scale;

    if (nTimeScale == 0)
    {
        frag.iNOfEntries = 0;
        return;
    }

    Ipp32u i;
    Ipp32u stts_i;
    Ipp32u samp_i = 0;
    Ipp32u ctts_i = 0;
    Ipp32u nTimeStamp = 0;

    for (stts_i = 0; stts_i < stts.total_entries; stts_i++)
    {
        for (i = 0; i < stts.table[stts_i].sample_count; i++)
        {
            if (ctts.total_entries == 0)
            {   // composition time stamp not present
                frag.pEntryArray[samp_i].dPts = (Ipp64f)nTimeStamp / nTimeScale;
                frag.pEntryArray[samp_i].dDts = -1.0;
            }
            else
            {
                frag.pEntryArray[samp_i].dPts =
                frag.pEntryArray[samp_i].dDts = (Ipp64f)nTimeStamp;
            }

            nTimeStamp += stts.table[stts_i].sample_duration;
            samp_i++;
            if(samp_i >= (Ipp32u)frag.iNOfEntries)
                break;
        }
    }

    if (ctts.total_entries == 0)
    {
        if (samp_i < (Ipp32u)frag.iNOfEntries)
            frag.iNOfEntries = samp_i;
        return;
    }

    samp_i = 0;

    for (ctts_i = 0; ctts_i < ctts.total_entries; ctts_i++)
    {
        for (i = 0; i < ctts.table[ctts_i].sample_count; i++)
        {
            frag.pEntryArray[samp_i].dPts += ctts.table[ctts_i].sample_offset;
            frag.pEntryArray[samp_i].dPts /= nTimeScale;
            frag.pEntryArray[samp_i].dDts /= nTimeScale;

            samp_i++;
            if (samp_i >= (Ipp32u)frag.iNOfEntries)
                return;
        }
    }

    if(samp_i < (Ipp32u)frag.iNOfEntries)
        frag.iNOfEntries = samp_i;
}

Status MP4Splitter::AddMoovToIndex(Ipp32u iES)
{
    IndexFragment frag;
    T_trak_data *trak = m_headerMPEG4.moov.trak[iES];
    Ipp32u nEntries   = trak->mdia.minf.stbl.stsz.total_entries;

    frag.iNOfEntries = nEntries;
    if(frag.iNOfEntries <= 0)
        return UMC_OK;

    frag.pEntryArray = new IndexEntry[nEntries];
    if (NULL == frag.pEntryArray)
        return UMC_ERR_ALLOC;

    FillSampleSizeAndType(trak, frag);
    FillSamplePos(trak, frag);
    FillSampleTimeStamp(trak, frag);

    m_pLastPTS[iES] = frag.pEntryArray[frag.iNOfEntries-1].GetTimeStamp();

    if (m_pLastPTS[iES] - (frag.pEntryArray[0].GetTimeStamp()) == 0)
    {   // track duration is invalid
        delete [] frag.pEntryArray;
        frag.pEntryArray = NULL;
        frag.iNOfEntries = 0;
        return UMC_OK;
    }

    return m_pTrackIndex[iES].Add(frag);
}

Status MP4Splitter::AddMoofRunToIndex(Ipp32u iES, T_trex_data *pTrex, T_traf *pTraf, T_trun *pTrun, Ipp64u &nBaseOffset)
{
    IndexFragment frag;
    Ipp32u i;

    Ipp64f rTimeScale = m_headerMPEG4.moov.trak[iES]->mdia.mdhd.time_scale;
    if (rTimeScale <= 0)
        return UMC_ERR_FAILED;

    frag.iNOfEntries = pTrun->sample_count;
    frag.pEntryArray = new IndexEntry[pTrun->sample_count];
    if (NULL == frag.pEntryArray)
        return UMC_ERR_ALLOC;

    for (i = 0; i < pTrun->sample_count; i++)
    {
        if (m_bFlagStopInitMoof)
        {
            delete[] frag.pEntryArray;
            return UMC_OK;
        }

        if (pTrun->flags & SAMPLE_SIZE_PRESENT) // sample size present in trun
            frag.pEntryArray[i].uiSize = pTrun->table_trun[i].sample_size;
        else if (pTraf->tfhd.flags & DEFAULT_SAMPLE_SIZE_PRESENT) // sample size present in track fragment header box (tfhd)
            frag.pEntryArray[i].uiSize = pTraf->tfhd.default_sample_size;
        else // default sample size in Track Extends box
            frag.pEntryArray[i].uiSize = pTrex->default_sample_size;

        frag.pEntryArray[i].stPosition = nBaseOffset;
        // update base offset for next sample
        nBaseOffset = frag.pEntryArray[i].stPosition + frag.pEntryArray[i].uiSize;

        Ipp32u duration = 0;
        if (pTrun->flags & SAMPLE_DURATION_PRESENT) // sample duration present in Track Fragment Run box
            duration = pTrun->table_trun[i].sample_duration;
        else if (pTraf->tfhd.flags & DEFAULT_SAMPLE_DURATION_PRESENT) // sample duration present in Track Fragment Header box
            duration = pTraf->tfhd.default_sample_duration;
        else
            duration = pTrex->default_sample_duration;

        if (m_headerMPEG4.moov.trak[iES]->mdia.minf.is_video)
        {
            VideoStreamInfo *pVInfo = (VideoStreamInfo*)m_pInfo->m_ppTrackInfo[iES]->m_pStreamInfo;
            if (pVInfo->fFramerate <= 0) /*** framerate is not set ***/
                pVInfo->fFramerate = rTimeScale/duration;
        }

        // sample-composition-time-offset-present
        if (pTrun->flags & SAMPLE_COMPOSITION_TIME_OFFSETS_PRESENT)
            duration += pTrun->table_trun[i].sample_composition_time_offset;

        if (rTimeScale)
            frag.pEntryArray[i].dPts = duration/rTimeScale;

        frag.pEntryArray[i].dPts += m_pLastPTS[iES];
        m_pLastPTS[iES] = frag.pEntryArray[i].GetTimeStamp();
    }

    return m_pTrackIndex[iES].Add(frag);
}

Ipp32u MP4Splitter::InitMoofThreadCallback(void* pParam)
{
    VM_ASSERT(NULL != pParam);
    MP4Splitter* pThis = (MP4Splitter*)pParam;
    pThis->InitMoof();
    return 0;
}

Status MP4Splitter::MapTrafIDToTrackID (Ipp32u trafID, Ipp32u &nTrack)
{
    Ipp32u i;

    for(i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if(m_pInfo->m_ppTrackInfo[i]->m_iPID == trafID)
        {
            nTrack = i;
            return UMC_OK;
        }
    }
    return UMC_ERR_FAILED;
}

/* reads the next fragment and adds it to the index */
Status MP4Splitter::InitMoof()
{
    Ipp32u iES, jES;
    Ipp64u nBaseOffset;

    Status umcRes = CheckInit();
    if (umcRes != UMC_OK)
        return umcRes;

    while(!m_bFlagStopInitMoof)
    {
        umcRes = SelectNextFragment();
        UMC_CHECK_STATUS(umcRes)
        nBaseOffset = 0;

        for(iES = 0; iES < m_headerMPEG4.moof.total_tracks; iES++)
        {
            // track fragment
            T_traf* pTraf = m_headerMPEG4.moof.traf[iES];
            if(pTraf == NULL)
                continue;

            // track extend box
            T_trex_data* pTrex = NULL;
            // finding a corresponding trex box based on track ID
            for (jES = 0; jES < m_headerMPEG4.moov.mvex.total_tracks; jES++)
            {
                if (m_headerMPEG4.moov.mvex.trex[jES] == NULL)
                    continue;

                if (pTraf->tfhd.track_ID == m_headerMPEG4.moov.mvex.trex[jES]->track_ID)
                {
                    pTrex = m_headerMPEG4.moov.mvex.trex[jES];
                    break;
                }
            }
            if (pTrex == NULL)
                continue;

            if (pTraf->tfhd.flags & BASE_DATA_OFFSET_PRESENT) // base data offset present
                nBaseOffset = pTraf->tfhd.base_data_offset;

            if ((iES == 0) && (nBaseOffset == 0))
            {
                // first byte after enclosing moof box;
                nBaseOffset = m_headerMPEG4.moof.end;

                // offset by 8 bytes (compact mdat box) or 16 bytes (large size)
                if (m_headerMPEG4.data.is_large_size)
                    nBaseOffset += 16;
                else
                    nBaseOffset += 8;
            }

            if (pTraf->tfhd.flags & DURATION_IS_EMPTY)
            {
                // duration is empty, no track run
                // TO DO: add one fragment containing only one sample, sample size and
                // position are both zero. sample time stamp is set to default sample duration.
            }

            Ipp64u nLastBaseOffset = nBaseOffset;
            for (Ipp32u k = 0; k < pTraf->entry_count; k++)
            {
                // zero or more Trak Run Boxes
                T_trun* pTrun = pTraf->trun[k];

                if ((pTrun == NULL) || (pTrun->sample_count == 0))
                    continue;

                /*** where to connect current track fragment ***/
                Ipp32u nTrack = 0;
                umcRes = MapTrafIDToTrackID(pTraf->tfhd.track_ID, nTrack);
                if (umcRes != UMC_OK)
                    continue;

                if (pTrun->flags & DATA_OFFSET_PRESENT)
                {
                    // data-offset-present
                    if (nBaseOffset + pTrun->data_offset > nLastBaseOffset)
                        nLastBaseOffset = nBaseOffset + pTrun->data_offset;
                }

                umcRes = AddMoofRunToIndex(nTrack, pTrex, pTraf, pTrun, nLastBaseOffset);
                UMC_CHECK_STATUS(umcRes)

                /*** first data is read -> we can init buffer ***/
                if (!m_headerMPEG4.moov.trak[nTrack]->mdia.minf.stbl.stsz.max_sample_size)
                {
                    MediaBufferParams mParams;
                    Ipp32u nBufferSize = 0;
                    IndexEntry entry;

                    UMC_NEW(m_ppMediaBuffer[nTrack], SampleBuffer);
                    UMC_NEW(m_ppLockedFrame[nTrack], MediaData);
                    m_pIsLocked[nTrack] = 0;
                    mParams.m_numberOfFrames = NUMBEROFFRAMES;

                    if (m_headerMPEG4.moov.trak[nTrack]->mdia.minf.is_audio)
                        nBufferSize = MINAUDIOBUFFERSIZE;
                    else if (m_headerMPEG4.moov.trak[nTrack]->mdia.minf.is_video)
                        nBufferSize = MINVIDEOBUFFERSIZE;

                    mParams.m_prefInputBufferSize =  mParams.m_prefOutputBufferSize = nBufferSize;
                    UMC_CHECK_FUNC(umcRes, m_ppMediaBuffer[nTrack]->Init(&mParams));

                    m_headerMPEG4.moov.trak[nTrack]->mdia.minf.stbl.stsz.max_sample_size = nBufferSize;

                    umcRes = m_pTrackIndex[nTrack].First(entry);
                    if (umcRes != UMC_OK)
                        return umcRes;
                }
            }

            nBaseOffset = nLastBaseOffset;
        }
    }

    return UMC_OK;
}

VideoStreamType GetVideoStreamType(Ipp8u value)
{
    switch (value)
    {
    case 0x20:
        return MPEG4_VIDEO;
    case 0x60:  // 13818-2
    case 0x61:  // 13818-2
    case 0x62:  // 13818-2
    case 0x63:  // 13818-2
    case 0x64:  // 13818-2
    case 0x65:  // 13818-2
        return MPEG2_VIDEO;
    case 0x6A:
        return MPEG1_VIDEO;
    case 0x6C:  // 10918-1
        return MJPEG_VIDEO;
    case 0xf1:
        return H264_VIDEO;
    case 0xf2:
        return H263_VIDEO;
    case 0xf3:
        return H261_VIDEO;
    case 0xf4:
        return AVS_VIDEO;
    default:
        return UNDEF_VIDEO;
    }
}

AudioStreamType GetAudioStreamType(Ipp8u value)
{
    switch (value)
    {
    case 0x40:  // 14496-3
    case 0x66:  // 13818-7
    case 0x67:  // 13818-7
    case 0x68:  // 13818-7
        return AAC_MPEG4_STREAM;
    case 0x69:  // 13818-3
        return MPEG2_AUDIO;
    case 0x6B:  // 11172-3
        return MPEG1_AUDIO;
    default:
        return UNDEF_AUDIO;
    }
}

Ipp32u GetChannelMask(Ipp32s channels)
{
    switch(channels)
    {
    case 1:
        return  CHANNEL_FRONT_CENTER;
    case 2:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT;
    case 3:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER;
    case 4:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                CHANNEL_TOP_CENTER;
    case 5:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                CHANNEL_BACK_LEFT  | CHANNEL_BACK_RIGHT;
    case 6:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                CHANNEL_BACK_LEFT  | CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY;
    case 7:
        return  CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
                CHANNEL_BACK_LEFT  | CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY |
                CHANNEL_FRONT_LEFT_OF_CENTER | CHANNEL_FRONT_RIGHT_OF_CENTER;
    default:
        return 0;
    }
}

const AudioStreamType MPEGAStreamType[2][3] = {
    {MP2L1_AUDIO, MP2L2_AUDIO, MP2L3_AUDIO},
    {MP1L1_AUDIO, MP1L2_AUDIO, MP1L3_AUDIO}
};

// macro for mp3 header parsing (copied from frame_constructor)
#define MPEGA_HDR_VERSION(x)       ((x & 0x80000) >> 19)
#define MPEGA_HDR_LAYER(x)         (4 - ((x & 0x60000) >> 17))
#define MPEGA_HDR_ERRPROTECTION(x) ((x & 0x10000) >> 16)
#define MPEGA_HDR_BITRADEINDEX(x)  ((x & 0x0f000) >> 12)
#define MPEGA_HDR_SAMPLINGFREQ(x)  ((x & 0x00c00) >> 10)
#define MPEGA_HDR_PADDING(x)       ((x & 0x00200) >> 9)
#define MPEGA_HDR_MODE(x)          ((x & 0x000c0) >> 6)

Status MP4Splitter::FillMPEGAudioInfo(Ipp32u nTrack)
{
    Status umcRes;
    TrackIndex *pIndex = &m_pTrackIndex[nTrack];
    IndexEntry m_Frame;
    Ipp8u *p_frame;
    Ipp32u curPos = 0, header = 0;
    Ipp32s id = 0, layer = 0, bitrate = 0, freq = 0, mode = 0, padding = 0, protection = 0;
    //  bool bSyncWordFound = false;
    AudioStreamInfo *pAudioInfo = (AudioStreamInfo *)(m_pInfo->m_ppTrackInfo[nTrack]->m_pStreamInfo);
    Ipp32s mpg25 = 0;

    // get first frame (index is already set to First frame)
    umcRes = pIndex->Get(m_Frame);
    UMC_CHECK_STATUS(umcRes)

    p_frame = (Ipp8u *)ippMalloc((Ipp32u)m_Frame.uiSize);
    UMC_CHECK_PTR(p_frame)

    m_ReaderMutex.Lock();
    m_pReader->SetPosition((Ipp64u)m_Frame.stPosition);
    m_pReader->GetData(p_frame, m_Frame.uiSize);
    m_ReaderMutex.Unlock();

    for (curPos = 0; curPos < m_Frame.uiSize - 3; curPos++)
    {
        // find synchroword
        if (p_frame[curPos] != 0xff || (p_frame[curPos + 1] & 0xe0) != 0xe0)
            continue;

        header = (p_frame[curPos + 0] << 24) |
                    (p_frame[curPos + 1] << 16) |
                    (p_frame[curPos + 2] <<  8) |
                    (p_frame[curPos + 3]);

        id = MPEGA_HDR_VERSION(header);
        layer = MPEGA_HDR_LAYER(header);
        freq = MPEGA_HDR_SAMPLINGFREQ(header);
        bitrate = MPEGA_HDR_BITRADEINDEX(header);
        mode = MPEGA_HDR_MODE(header);
        padding = MPEGA_HDR_PADDING(header);
        protection = MPEGA_HDR_ERRPROTECTION(header);
        mpg25 = ((header >> 20) & 1) ? 0 : 2;

        // check forbidden values
        if(4 == layer || 3 == freq ||
#ifdef FREE_FORMAT_PROHIBIT
                0 == bitrate ||
#endif //FREE_FORMAT_PROHIBIT
            15 == bitrate || (mpg25 > 0 && id == 1)) // for mpeg2.5 id should be 0
            continue;

        break;
    }

    pAudioInfo->streamType = MPEGAStreamType[id][layer - 1];

    ippFree(p_frame);
    return UMC_OK;
}

Status MP4Splitter::FillAudioInfo(T_trak_data *pTrak, Ipp32u nTrack)
{
    AudioStreamInfo *pAudioInfo = (AudioStreamInfo *)(m_pInfo->m_ppTrackInfo[nTrack]->m_pStreamInfo);
    T_stsd_table_data *table = pTrak->mdia.minf.stbl.stsd.table;
    Ipp8u *ptr = NULL;
    Ipp32s len = 0;


    if (table->damr.decoderConfigLen)
    {   /*** AMR audio ***/
        len = table->damr.decoderConfigLen;
        ptr = table->damr.decoderConfig;
    }
    else if (table->esds.decoderConfigLen)
    {   /*** AAC audio ***/
        len = table->esds.decoderConfigLen;
        ptr = table->esds.decoderConfig;
    }

    if(len)
    {
        MediaData *m_pHeader = new MediaData();
        m_pHeader->Alloc(len);
        UMC_CHECK_PTR(m_pHeader)
        memcpy(m_pHeader->GetDataPointer(), ptr, len);
        m_pHeader->SetDataSize(len);
        m_pHeader->m_fPTSStart = m_pHeader->m_fPTSEnd = 0;
        m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader = m_pHeader;
    }

    if (table->damr.decoderConfigLen == 0)
    {
        pAudioInfo->audioInfo.m_iBitPerSample = 16;
        pAudioInfo->iBitrate = table->esds.avgBitrate;
        pAudioInfo->audioInfo.m_iChannels = table->channels;
        pAudioInfo->audioInfo.m_iSampleFrequency = table->sample_rate;
        pAudioInfo->audioInfo.m_iChannelMask = GetChannelMask(table->channels);
        pAudioInfo->fDuration = (Ipp64f)pTrak->mdia.mdhd.duration / pTrak->mdia.mdhd.time_scale;
        pAudioInfo->bProtected = (table->is_protected) ? true : false;
        pAudioInfo->streamType = GetAudioStreamType(table->esds.objectTypeID);
        pAudioInfo->streamSubtype = UNDEF_AUDIO_SUBTYPE;

        switch (pAudioInfo->streamType)
        {
        case AAC_MPEG4_STREAM:
            m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_AAC;

            /*** update audio_info from DecSpecInfo ***/
            if (m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader)
            {
                sAudio_specific_config  audio_config_data;
                sBitsreamBuffer         bs;

            //        T_mdia_data m_Mdia = m_headerMPEG4.moov.trak[nTrack]->mdia;

                INIT_BITSTREAM(&bs, m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader->GetDataPointer())
                bs.nDataLen = (Ipp32s)m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader->GetDataSize();
                if(!dec_audio_specific_config(&audio_config_data,&bs))
                {
                    //dec_audio_specific_config returns 0, if everything is OK.
            //          pAudioInfo->stream_type = AudioObjectTypeID(audio_config_data.audioObjectType);
                    pAudioInfo->streamType = UMC::AAC_MPEG4_STREAM;
                    pAudioInfo->audioInfo.m_iSampleFrequency = get_sampling_frequency(&audio_config_data, 0);

                    if (1 == audio_config_data.sbrPresentFlag)
                        pAudioInfo->audioInfo.m_iSampleFrequency = get_sampling_frequency(&audio_config_data, 1);

                    pAudioInfo->audioInfo.m_iChannels = get_channels_number(&audio_config_data);
                }
            }
            break;
        case MPEG2_AUDIO:
            m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MPEGA;
            break;
        case MPEG1_AUDIO:
            m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MPEGA;
            break;
        default:
            m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_UNKNOWN;
        }
    }
    else
    {   /*** AMR audio ***/
        pAudioInfo->audioInfo.m_iSampleFrequency = 8000;
        pAudioInfo->audioInfo.m_iChannels = 1;
        pAudioInfo->audioInfo.m_iBitPerSample = 16;
        pAudioInfo->streamType = AMR_NB_AUDIO;
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_AMR;
    }
    pAudioInfo->iStreamPID = pTrak->tkhd.track_id;

    return UMC_OK;
}

Status MP4Splitter::FillVideoInfo(T_trak_data *pTrak, Ipp32u nTrack)
{
    VideoStreamInfo *pVideoInfo = (VideoStreamInfo *)(m_pInfo->m_ppTrackInfo[nTrack]->m_pStreamInfo);
    T_stsd_table_data *table = pTrak->mdia.minf.stbl.stsd.table;
    Ipp8u *ptr = NULL;
    Ipp32s len = 0;
    Status umcRes = UMC_OK;

    pVideoInfo->streamSubtype           = UNDEF_VIDEO_SUBTYPE;
    pVideoInfo->fFramerate              = -1;
    pVideoInfo->videoInfo.m_iWidth      = table->width;
    pVideoInfo->videoInfo.m_iHeight     = table->height;
    pVideoInfo->videoInfo.m_iSAWidth    = 1;
    pVideoInfo->videoInfo.m_iSAHeight   = 1;
    pVideoInfo->videoInfo.m_colorFormat = YUV420;

    /*** update clip_info and interlace_type ***/
    if (table->esds.decoderConfigLen)
    {
        len = table->esds.decoderConfigLen;
        ptr = table->esds.decoderConfig;
        //get bitrate
        pVideoInfo->iBitrate = table->esds.avgBitrate;
        //get info about stream from ESDS info
        umcRes = ParseESDSHeader(pTrak, nTrack);
        UMC_CHECK_STATUS(umcRes)
    }
    else
    {
        if(table->avcC.decoderConfigLen)
        {
            //get info about stream from avcC info
            umcRes = ParseAVCCHeader(pTrak, nTrack);
            UMC_CHECK_STATUS(umcRes)
            umcRes = SaveHeader(&(m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader), &table->avcC);
            UMC_CHECK_STATUS(umcRes)
            len = 0;    // header is copied in SaveHeader
            ptr = NULL;
        }
        if(table->svcC.decoderConfigLen)
        {
            umcRes = SaveHeader(&(m_pInfo->m_ppTrackInfo[nTrack]->m_pHeaderExt), &table->svcC);
            UMC_CHECK_STATUS(umcRes)
            len = 0;    // header is copied in SaveHeader
            ptr = NULL;

            pVideoInfo->streamSubtype = SCALABLE_VIDEO;
            pVideoInfo->iViews        = pTrak->mdia.minf.stbl.stsd.table->vwid.num_views;
        }
        else if(table->mvcC.decoderConfigLen)
        {
            umcRes = SaveHeader(&(m_pInfo->m_ppTrackInfo[nTrack]->m_pHeaderExt), &table->mvcC);
            UMC_CHECK_STATUS(umcRes)
            len = 0;    // header is copied in SaveHeader
            ptr = NULL;

            pVideoInfo->streamSubtype = MULTIVIEW_VIDEO;
            pVideoInfo->iViews        = pTrak->mdia.minf.stbl.stsd.table->vwid.num_views;
        }
    }

    if(len)
    {
        MediaData *m_pHeader = new MediaData();
        m_pHeader->Alloc(len);
        UMC_CHECK_PTR(m_pHeader)
        memcpy(m_pHeader->GetDataPointer(), ptr, len);
        m_pHeader->SetDataSize(len);
        m_pHeader->m_fPTSStart = m_pHeader->m_fPTSEnd = 0;
        m_pInfo->m_ppTrackInfo[nTrack]->m_pHeader = m_pHeader;
    }
    pVideoInfo->iStreamPID = pTrak->tkhd.track_id;

    //get duration
    pVideoInfo->fDuration = (Ipp64f)pTrak->mdia.mdhd.duration / pTrak->mdia.mdhd.time_scale;

    //get framerate
    if (pVideoInfo->fDuration && (pVideoInfo->fFramerate == -1))
        pVideoInfo->fFramerate = (Ipp64f)pTrak->mdia.minf.stbl.stsz.total_entries / pVideoInfo->fDuration;

    //get stream type
    pVideoInfo->streamType = GetVideoStreamType(table->esds.objectTypeID);

    switch (pVideoInfo->streamType)
    {
    case MPEG4_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MPEG4V;
        pVideoInfo->streamSubtype = MPEG4_VIDEO_QTIME;
        break;
    case MPEG2_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MPEG2V;
        break;
    case MPEG1_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MPEG1V;
        break;
    case MJPEG_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_MJPEG;
        break;
    case H264_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_H264;
        break;
    case H263_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_H263;
        break;
    case H261_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_H261;
        break;
    case AVS_VIDEO:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_AVS;
        break;
    default:
        m_pInfo->m_ppTrackInfo[nTrack]->m_type = TRACK_UNKNOWN;
    }

    return UMC_OK;
}

Status MP4Splitter::Init(SplitterParams *pInit)
{
    Ipp32u iES, nAudio = 0, nVideo = 0;
    IndexEntry entry;
    Status umcRes, umcResOut = UMC_OK;

    if (pInit->m_pDataReader == NULL)
        return UMC_ERR_NOT_ENOUGH_DATA;

    m_bFlagStop = true;

    /* init LockableDataReader */
    umcRes = IndexSplitter::Init(pInit);
    UMC_CHECK_STATUS(umcRes)

    m_pInfo = new SplitterInfo;
    UMC_CHECK_PTR(m_pInfo)

    m_pInfo->m_iFlags = pInit->m_iFlags;
    m_pInfo->m_systemType = MP4_ATOM_STREAM;

    memset(&m_headerMPEG4, 0, sizeof(info_atoms));

    /* read moov atom */
    umcRes = ParseMP4Header();
    if (umcRes == UMC_WRN_INVALID_STREAM)
    {
        umcResOut = umcRes;
        umcRes = UMC_OK;
    }
    UMC_CHECK_STATUS(umcRes)

    m_pInfo->m_iTracks = m_headerMPEG4.moov.total_tracks;
    m_pInfo->m_fDuration = (Ipp64f)m_headerMPEG4.moov.mvhd.duration / m_headerMPEG4.moov.mvhd.time_scale;

    m_pTrackIndex = new TrackIndex[m_pInfo->m_iTracks];
    if (NULL == m_pTrackIndex)
        return UMC_ERR_ALLOC;

    UMC_ALLOC_ZERO_ARR(m_pInfo->m_ppTrackInfo, TrackInfo*, m_pInfo->m_iTracks)
    UMC_ALLOC_ZERO_ARR(m_ppMediaBuffer, MediaBuffer*, m_pInfo->m_iTracks)
    UMC_ALLOC_ZERO_ARR(m_ppLockedFrame, MediaData*, m_pInfo->m_iTracks)
    UMC_ALLOC_ZERO_ARR(m_pIsLocked, Ipp32s, m_pInfo->m_iTracks)

    UMC_NEW_ARR(m_pReadESThread, vm_thread, m_pInfo->m_iTracks)
    UMC_ALLOC_ZERO_ARR(m_pLastPTS, Ipp64f, m_pInfo->m_iTracks)

    for (iES = 0; iES < m_pInfo->m_iTracks; iES++)
    {
        T_trak_data *pTrak = m_headerMPEG4.moov.trak[iES];
        MediaBufferParams mParams;
        int nBufferSize = 0;

        umcRes = AddMoovToIndex(iES);
        UMC_CHECK_STATUS(umcRes)

        m_pInfo->m_ppTrackInfo[iES] = new MP4TrackInfo();
        m_pInfo->m_ppTrackInfo[iES]->m_iPID = pTrak->tkhd.track_id;
        MP4TrackInfo *pMP4TrackInfo = (MP4TrackInfo *)m_pInfo->m_ppTrackInfo[iES];
        pMP4TrackInfo->m_DpndPID = pTrak->tref.dpnd.idTrak;
        vm_thread_set_invalid(&m_pReadESThread[iES]);

        umcRes = m_pTrackIndex[iES].First(entry);
        if ((umcRes == UMC_ERR_NOT_ENOUGH_DATA) && (!m_headerMPEG4.moov.mvex.total_tracks))
        {
            // data is absent really or it's a fragmented file
            m_pInfo->m_ppTrackInfo[iES]->m_bEnabled = false;
            umcRes = UMC_OK;  // valid situation when track is unknown
            continue;
        }

        if (pTrak->mdia.minf.is_audio)
        {
            nAudio++;
            m_pInfo->m_ppTrackInfo[iES]->m_pStreamInfo = new AudioStreamInfo();
            umcRes = FillAudioInfo(pTrak, iES);
            UMC_CHECK_STATUS(umcRes)
            if (m_pInfo->m_ppTrackInfo[iES]->m_type == TRACK_MPEGA)
            {
                umcRes = FillMPEGAudioInfo(iES);
                UMC_CHECK_STATUS(umcRes)
            }
            if ((m_pInfo->m_fRate == 1) && (m_pInfo->m_iFlags & AUDIO_SPLITTER)) 
                m_pInfo->m_ppTrackInfo[iES]->m_bEnabled = true;
        }
        else if (pTrak->mdia.minf.is_video)
        {
            nVideo++;
            m_pInfo->m_ppTrackInfo[iES]->m_pStreamInfo = new VideoStreamInfo();
            umcRes = FillVideoInfo(pTrak, iES);
            UMC_CHECK_STATUS(umcRes)
            if (m_pInfo->m_iFlags & VIDEO_SPLITTER)
                m_pInfo->m_ppTrackInfo[iES]->m_bEnabled = true;
        } else
            continue; // skip unsupported stream

        /*** data may not be read for fragmented files ***/
        if (pTrak->mdia.minf.stbl.stsz.max_sample_size)
        {
            UMC_NEW(m_ppMediaBuffer[iES], SampleBuffer);
            UMC_NEW(m_ppLockedFrame[iES], MediaData);
            m_pIsLocked[iES] = 0;
            mParams.m_numberOfFrames = NUMBEROFFRAMES;
            nBufferSize = pTrak->mdia.minf.stbl.stsz.max_sample_size;

            if (pTrak->mdia.minf.is_audio)
            {
                if (nBufferSize < MINAUDIOBUFFERSIZE)
                    nBufferSize = MINAUDIOBUFFERSIZE;
            }
            else if (pTrak->mdia.minf.is_video)
            {
                if (nBufferSize < MINVIDEOBUFFERSIZE)
                    nBufferSize = MINVIDEOBUFFERSIZE;
            }

            mParams.m_prefInputBufferSize =  mParams.m_prefOutputBufferSize = nBufferSize;
            UMC_CHECK_FUNC(umcRes, m_ppMediaBuffer[iES]->Init(&mParams));
        }
        else
        {
            UMC_NEW(m_ppMediaBuffer[iES], SampleBuffer);
            UMC_NEW(m_ppLockedFrame[iES], MediaData);
            m_pIsLocked[iES] = 0;
            mParams.m_numberOfFrames = 1;

            mParams.m_prefInputBufferSize =  mParams.m_prefOutputBufferSize = 1;
            UMC_CHECK_FUNC(umcRes, m_ppMediaBuffer[iES]->Init(&mParams));
        }
    }

    // update splitter flag
    if (nVideo == 0)
        m_pInfo->m_iFlags &= ~VIDEO_SPLITTER;
    if (nAudio == 0)
        m_pInfo->m_iFlags &= ~AUDIO_SPLITTER;

    if (m_headerMPEG4.moov.mvex.total_tracks)
    {   // there are fragments
        Ipp32s res = 0;
        m_pInitMoofThread = new vm_thread;
        vm_thread_set_invalid(m_pInitMoofThread);
        res = vm_thread_create(m_pInitMoofThread, (vm_thread_callback)InitMoofThreadCallback, (void *)this);
        if (res != 1)
            return UMC_ERR_FAILED;

        do
        {
            vm_time_sleep(10);
            umcRes = CheckMoofInit();
        } while (umcRes != UMC_OK);
    }

    return (umcResOut != UMC_OK) ? umcResOut : umcRes;
}

Status MP4Splitter::Close()
{
    Ipp32u iES;

    Status umcRes = CheckInit();
    if (umcRes != UMC_OK)
        return umcRes;

    m_bFlagStopInitMoof = true;
    if (vm_thread_is_valid(m_pInitMoofThread))
    {
        vm_thread_wait(m_pInitMoofThread);
        vm_thread_close(m_pInitMoofThread);
        vm_thread_set_invalid(m_pInitMoofThread);
    }

    IndexSplitter::Close();

    for (iES = 0; iES < m_pInfo->m_iTracks; iES++)
    {
        Clear_track(m_headerMPEG4.moov.trak[iES]);

        if(m_ppMediaBuffer[iES])
            delete m_ppMediaBuffer[iES];
        if(m_ppLockedFrame[iES])
            delete m_ppLockedFrame[iES];
        if(m_pInfo->m_ppTrackInfo[iES]->m_pHeader)
            delete m_pInfo->m_ppTrackInfo[iES]->m_pHeader;
        if(m_pInfo->m_ppTrackInfo[iES]->m_pHeaderExt)
            delete m_pInfo->m_ppTrackInfo[iES]->m_pHeaderExt;
        if(m_pInfo->m_ppTrackInfo[iES]->m_pStreamInfo)
            delete m_pInfo->m_ppTrackInfo[iES]->m_pStreamInfo;
        if(m_pInfo->m_ppTrackInfo[iES])
            delete m_pInfo->m_ppTrackInfo[iES];
    }

    /* if there are extended tracks */
    umcRes = Clear_moof(m_headerMPEG4.moof);
    if (umcRes != UMC_OK)
        return umcRes;

    for (iES = 0; iES < m_headerMPEG4.moov.mvex.total_tracks; iES++)
    {
        ippFree(m_headerMPEG4.moov.mvex.trex[iES]);
        m_headerMPEG4.moov.mvex.trex[iES] = NULL;
    }

    if(m_pLastPTS)
    {
        ippFree(m_pLastPTS);
        m_pLastPTS = NULL;
    }
    if(m_pReadESThread)
    {
        delete[] m_pReadESThread;
        m_pReadESThread = NULL;
    }
    if(m_pIsLocked)
    {
        ippFree(m_pIsLocked);
        m_pIsLocked = NULL;
    }
    if(m_ppLockedFrame)
    {
        ippFree(m_ppLockedFrame);
        m_ppLockedFrame = NULL;
    }
    if(m_ppMediaBuffer)
    {
        ippFree(m_ppMediaBuffer);
        m_ppMediaBuffer = NULL;
    }
    if(m_pInfo->m_ppTrackInfo)
    {
        ippFree(m_pInfo->m_ppTrackInfo);
        m_pInfo->m_ppTrackInfo = NULL;
    }
    if (m_pTrackIndex)
    {
        delete[] m_pTrackIndex;
        m_pTrackIndex = NULL;
    }
    if (m_pInfo)
    {
        delete m_pInfo;
        m_pInfo = NULL;
    }

    return UMC_OK;
}

#endif
