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
#include <math.h>
#include "bstream.h"
#include "aaccmn_adts.h"
#include "mp4cmn_config.h"
#include "umc_mp4_mux_defs.h"
#include "umc_mp4_mux.h"
#include "umc_automatic_mutex.h"


#ifdef OSX
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

namespace UMC {

MP4Muxer::MP4Muxer()
{
    m_iH264DataCurSize = 0;
    m_nH264DataOffset = 0;
    m_bDataFromEncoder = false;
    bMuxerInitialized = false;
    bOutFileInitialized = false;
    m_nMoovSize = MAX_MOOV_SIZE;
    m_nMoofSize = MAX_MOOF_SIZE;
    m_bMoov = true;
    m_nMdatSize = 0;
    nTimeScale = 48000;
    m_fTempOutput1 = NULL;
    m_fTempOutput2 = NULL;
    m_nHeaderSize = 0;
    m_pH264Data = NULL;
    m_mH264DataOut = NULL;
    m_pH264Header = NULL;
    m_nIDRFrames = 0;
    m_FirstFrame = 0;

    m_headerMoof.sequence_number = 0;
    memset(&m_headerMoof,  0, sizeof(TM_moof));
    memset(&m_headerMPEG4, 0, sizeof(TM_moov));
    memset(&m_headerMVEX,  0, sizeof(TM_mvex));
}

MP4Muxer::~MP4Muxer()
{
    Close();
}

Status MP4Muxer::Init(MuxerParams *lpInit)
{
    Ipp32u i;
    Status status;

    UMC_CHECK_FUNC(status, CopyMuxerParams(lpInit));

    UMC_CHECK_PTR(m_pParams->m_lpDataWriter);

    m_bDataFromEncoder = ((m_pParams->m_lFlags & FLAG_DATA_FROM_SPLITTER) == 0);

    if ((m_pParams->m_lFlags & FLAG_FRAGMENTED_AT_I_PICTURES) ||
        (m_pParams->m_lFlags & FLAG_FRAGMENTED_BY_HEADER_SIZE))
    {
        m_pParams->m_lFlags |= FLAG_START_WITH_HEADER;
    }

    m_headerMoof.total_tracks = m_uiTotalNumStreams;
    m_headerMPEG4.total_tracks = m_uiTotalNumStreams;

    for (i = 0; i < m_uiTotalNumStreams; i++) {
      UMC_NEW(m_ppBuffers[i], SampleBuffer);
      UMC_CHECK_FUNC(status, m_ppBuffers[i]->Init(&m_pTrackParams[i].bufferParams));
    }

    for (i = 0; i < m_headerMPEG4.total_tracks; i++ )
    {
        m_headerMPEG4.trak[i] = _NEW(TM_trak_data);
        m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.total_entries = 1;
        m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table = _NEW(TM_stsd_table_data);
        m_headerMPEG4.trak[i]->mdia.minf.stbl.stts.total_entries = 0;
        m_headerMPEG4.trak[i]->mdia.minf.stbl.ctts.total_entries = 0;
    }

    m_nIDRFrames = _NEW_n(Ipp32s, m_headerMPEG4.total_tracks);
    UMC_CHECK_PTR(m_nIDRFrames);
    memset(m_nIDRFrames, 0, m_headerMPEG4.total_tracks * sizeof(Ipp32s));
    m_FirstFrame = _NEW_n(Ipp32s, m_headerMPEG4.total_tracks);
    UMC_CHECK_PTR(m_FirstFrame);
    for (i = 0; i < m_headerMPEG4.total_tracks; i++ )
    {
      m_FirstFrame[i] = 1;
    }

    InitTracks(m_headerMPEG4.total_tracks);
    InitEsds();
    InitHeader();

    //initial header and atoms sizes
    m_nHeaderSize = CalculateSizes();

    //all OK, muxer is initialized
    bMuxerInitialized = true;

    m_PutDataMutex.Reset();

    return UMC_OK;
}

Status MP4Muxer::Close(void)
{
    Status umcRes = UMC_OK;
    Ipp32u i;
    MediaData cData;

    if ( bMuxerInitialized )
    {
        //dump buffers
        umcRes = MuxData(true);
        UMC_CHECK_STATUS(umcRes)

        if (m_bMoov)
        {
            umcRes = WriteHeader();
            UMC_CHECK_STATUS(umcRes)
        } else
        {
            umcRes = WriteHeaderFragment();
            UMC_CHECK_STATUS(umcRes)
            m_headerMoof.sequence_number++;
        }

        for (i = 0; i < m_headerMPEG4.total_tracks; i++)
        {
            TM_stbl_data& stbl = m_headerMPEG4.trak[i]->mdia.minf.stbl;

            _SAFE_DELETE_ARRAY(stbl.stsc.table);
            _SAFE_DELETE_ARRAY(stbl.stts.table);
            if (m_sTrack[i].m_nCttsEntries)
              _SAFE_DELETE_ARRAY(stbl.ctts.table);
            _SAFE_DELETE_ARRAY(stbl.stco.table);
            _SAFE_DELETE_ARRAY(stbl.stsz.table);

            if (m_headerMPEG4.trak[i]->mdia.minf.is_video)
                _SAFE_DELETE_ARRAY(stbl.stss.table);

            Ipp32s nIndex = stbl.stsd.total_entries - 1; // now total_entries = 1, so nIndex = 0;
            _SAFE_DELETE_ARRAY(stbl.stsd.table[nIndex].esds.decoderConfig);

            _SAFE_DELETE_ARRAY(m_sTrack[i].m_pSamples);

            _SAFE_DELETE_ARRAY(m_headerMPEG4.trak[i]->mdia.minf.stbl.stsd.table);
            _SAFE_DELETE_ARRAY(m_headerMPEG4.trak[i]);
        }

        _SAFE_DELETE_ARRAY(m_sTrack);

        if (m_headerMoof.trun)
        {
            for (i = 0; i < m_headerMPEG4.total_tracks; i++)
            {
                _SAFE_DELETE_ARRAY(m_headerMoof.trun[i].trun);
            }
            _SAFE_DELETE_ARRAY(m_headerMoof.trun);
        }

        _SAFE_DELETE_CPP(m_mH264DataOut);

        if (m_pH264Header)
        {
            if (m_pH264Header->m_pNal_ut_sps)
            {
                for (i = 0; i < m_pH264Header->m_nSps_num; i++)
                {
                    _SAFE_DELETE_ARRAY(m_pH264Header->m_pNal_ut_sps[i].m_pNal_ut_sps);
                }
            }
            _SAFE_DELETE_ARRAY(m_pH264Header->m_pNal_ut_sps);

            if (m_pH264Header->m_pNal_ut_pps)
            {
                for (i = 0; i < m_pH264Header->m_nPps_num; i++)
                {
                    _SAFE_DELETE_ARRAY(m_pH264Header->m_pNal_ut_pps[i].m_pNal_ut_pps);
                }
            }
            _SAFE_DELETE_ARRAY(m_pH264Header->m_pNal_ut_pps);
        }
        _SAFE_DELETE_CPP(m_pH264Header);

        _SAFE_DELETE_ARRAY(m_pH264Data);
        m_iH264DataCurSize=0;

        bMuxerInitialized = false;
        bOutFileInitialized = false;
    }

    _SAFE_DELETE_ARRAY( m_nIDRFrames );
    _SAFE_DELETE_ARRAY( m_FirstFrame );

    // Close base muxer
    UMC_CHECK_FUNC(umcRes, Muxer::Close());

    return umcRes;
}

Status MP4Muxer::SampleInfo(MediaData *lpData, Ipp32s ntrak)
{
    Ipp64f nStart = 0, nEnd = 0;  /* nEnd is not used really */
    Ipp32s nSize = 0;

    nSize  = (Ipp32u)lpData->GetDataSize();
    nStart = lpData->m_fPTSStart;
    nEnd   = lpData->m_fPTSEnd;

    sMuxSample& curSample = m_sTrack[ntrak].m_pSamples[m_sTrack[ntrak].m_nSamplesCount];

    curSample.m_nSize  = nSize;

    if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
    {
        curSample.m_nPosition = vm_file_ftell(m_fTempOutput1) - nSize;
    } else
    {
        curSample.m_nPosition = m_pParams->m_lpDataWriter->GetPosition();
        curSample.m_nPosition -= nSize;
    }

    // check start codes
    if (m_headerMPEG4.trak[ntrak]->tkhd.is_video) {
      curSample.m_frameTMype = GetPictureType(lpData, ntrak);
      if (I_PICTURE == curSample.m_frameTMype)
      {
          curSample.m_frameTMype = I_PICTURE;
          m_nHeaderSize += m_bMoov ? 4 : 0;
          m_nIDRFrames[ntrak]++;
      } else if (B_PICTURE == curSample.m_frameTMype)
      {
        m_sTrack[ntrak].m_nCttsEntries = 1;
      }
    }

    curSample.m_nTimeStamp    = (Ipp32s)(nStart * nTimeScale + 0.5);
    if (nStart > m_sTrack[ntrak].m_nTrackEnd) // to exclude B frames
      m_sTrack[ntrak].m_nTrackEnd = nStart;

    curSample.m_nSamplesPerChunk  = 1;
    curSample.m_nId               = 1;
    curSample.m_nFirstChunk       = m_sTrack[ntrak].m_nSamplesCount + 1;

    // set duration
    curSample.m_nDuration = 0;
    if (m_headerMPEG4.trak[ntrak]->tkhd.is_video) {
        /*** set default duration = 1/frate ***/
        Ipp64f frate = m_pTrackParams[ntrak].info.video->fFramerate;
        if (frate <= 0) frate = 30;
        curSample.m_nDuration = (Ipp32s)((1 / frate) * nTimeScale);
    } else if (m_headerMPEG4.trak[ntrak]->tkhd.is_audio) {
        /*** duration of the 1st audio sample by default ***/
        if (m_sTrack[ntrak].m_nSamplesCount > 0)
            curSample.m_nDuration = m_sTrack[ntrak].m_pSamples[0].m_nDuration;
    }

    if (m_sTrack[ntrak].m_nSamplesCount != 0) { // not the first sample in track
        sMuxSample& prevSample = m_sTrack[ntrak].m_pSamples[m_sTrack[ntrak].m_nSamplesCount - 1];
        if (m_sTrack[ntrak].m_nSamplesCount == 1) // if >2 samples, the last sample has its own stts entry
            m_sTrack[ntrak].m_nSttsEntries++;
        if (m_headerMPEG4.trak[ntrak]->tkhd.is_video && !m_sTrack[ntrak].m_bSortPTSOnExit)
        {   // when no B frames but durations differ
            if (curSample.m_nTimeStamp > prevSample.m_nTimeStamp)
                prevSample.m_nDuration = curSample.m_nTimeStamp - prevSample.m_nTimeStamp;
            else
                m_sTrack[ntrak].m_bSortPTSOnExit = true;    /* frames durations will be recalculated on exit */
        }
        if (m_headerMPEG4.trak[ntrak]->tkhd.is_audio)
        {
            if (curSample.m_nTimeStamp > prevSample.m_nTimeStamp)
                prevSample.m_nDuration = curSample.m_nTimeStamp - prevSample.m_nTimeStamp;
        }
        if ((m_headerMPEG4.trak[ntrak]->tkhd.is_video) &&
            (m_sTrack[ntrak].m_nCttsEntries == 0))
        {
            if (curSample.m_nTimeStamp < prevSample.m_nTimeStamp)
                m_sTrack[ntrak].m_nCttsEntries = 1;
        }
        if (m_sTrack[ntrak].m_nSamplesCount > 1)
        {
            sMuxSample& prevprevSample = m_sTrack[ntrak].m_pSamples[m_sTrack[ntrak].m_nSamplesCount - 2];
            if (prevSample.m_nDuration != prevprevSample.m_nDuration)
            {
                m_sTrack[ntrak].m_nSttsEntries++;
            }
        }
    }

    m_nHeaderSize += m_bMoov ? 20 : 8;  /* 12 stsc; 4 stco; 4 stsz  */

    m_sTrack[ntrak].m_nSamplesCount++;

    if (m_sTrack[ntrak].m_nSamplesCount + 1 >= m_sTrack[ntrak].m_nTrackSize)
    {
        m_sTrack[ntrak].m_pSamples = (sMuxSample*) realloc(m_sTrack[ntrak].m_pSamples, sizeof(sMuxSample) * (INITIAL_TRACK_SIZE + (size_t)m_sTrack[ntrak].m_nTrackSize));
        m_sTrack[ntrak].m_nTrackSize += INITIAL_TRACK_SIZE;
    }

    return UMC_OK;
}

Status MP4Muxer::AdjustVideoData(MediaData* &lpData, Ipp32s nStreamNumber)
{
    if (UMC::H264_VIDEO == m_pTrackParams[nStreamNumber].info.video->streamType)
    { // collect all unique SPSes and PPSes
      // lpData keeps SPS/PPS only in case of 1st frame from Splitter
      lpData = TransformH264Frame(lpData);
      // lpData->GetDataSize() == 0 in case of 1st frame from Splitter
    } else
    {
      if (m_FirstFrame[nStreamNumber])
      {  /* first frame should be DecSpecInfo */
        m_FirstFrame[nStreamNumber] = 0;
        TM_esds_data *pEsds = &(m_headerMPEG4.trak[nStreamNumber]->mdia.minf.stbl.stsd.table[0].esds);
        if ((UMC::MPEG4_VIDEO == m_pTrackParams[nStreamNumber].info.video->streamType) &&  m_bDataFromEncoder) {
          // mp4v from encoder: fill esds and PutData
          Ipp32u i, m_InfoLen = 0;
          Ipp32u mFrameLen = (Ipp32u)lpData->GetDataSize();
          Ipp8u *pData = (Ipp8u *)lpData->GetDataPointer();
          for (i = 0; i < mFrameLen - 3; i++) {
            if (((pData[i]==0x00)&&(pData[i+1]==0x00)&&(pData[i+2]==0x01)&&(pData[i+3]==0xb6)) ||
              ((pData[i]==0x00)&&(pData[i+1]==0x00)&&(pData[i+2]==0x01)&&(pData[i+3]==0xb3))) {
                m_InfoLen = i;
                break;
            }
          }
          // m_InfoLen can't be equal to zero
          if (m_InfoLen) {
            pEsds->decoderConfigLen = m_InfoLen;
            pEsds->decoderConfig = (Ipp8u *)malloc(m_InfoLen);
            UMC_CHECK_PTR(pEsds->decoderConfig)
            memcpy(pEsds->decoderConfig, pData, m_InfoLen);
            lpData->MoveDataPointer(m_InfoLen);  // cut off mp4v header
          }
        } else
        {
          // mp4v from splitter: first frame is esds
          pEsds->decoderConfigLen = (Ipp32u)lpData->GetDataSize();
          pEsds->decoderConfig = (Ipp8u *)malloc(pEsds->decoderConfigLen);
          UMC_CHECK_PTR(pEsds->decoderConfig)
          memcpy(pEsds->decoderConfig, lpData->GetDataPointer(), lpData->GetDataSize());
          lpData->SetDataSize(0);
        }
      }
    }
    return UMC_OK;
}

static Ipp32s sampling_frequency_table[] = {
    96000, 88200, 64000, 48000, 44100, 32000, 24000,
    22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
};

Status MP4Muxer::AdjustAudioData(MediaData* &lpData, Ipp32s nStreamNumber)
{
    Ipp32s nbits, nbytes;
    sBitsreamBuffer BS;
    sAdts_fixed_header m_adts_fixed_header;
    sAdts_variable_header m_adts_variable_header;

    if (m_pTrackParams[nStreamNumber].info.audio->streamType & AAC_AUDIO)
    {
      if (m_bDataFromEncoder)
      {
        GET_INIT_BITSTREAM(&BS, lpData->GetDataPointer());
        if ((0 == dec_adts_fixed_header(&m_adts_fixed_header, &BS)) &&
            (0 == dec_adts_variable_header(&m_adts_variable_header, &BS)))
        {
          Byte_alignment(&BS);
          GET_BITS_COUNT(&BS, nbits);
          nbytes = nbits >> 3;
          if (m_adts_fixed_header.protection_absent == 0)
          {
            nbytes += 2;
          }
          lpData->MoveDataPointer(nbytes);  // cut off adts headers
        }
      }

      if (m_FirstFrame[nStreamNumber])
      {
        m_FirstFrame[nStreamNumber] = 0;
        if (m_bDataFromEncoder)
        {
          sAudio_specific_config decSpecConfig;
          MediaData p_SpecInfo;
          p_SpecInfo.Alloc(400);

          /// AudioSpecificConfig;
          decSpecConfig.sbrPresentFlag = 0;
          decSpecConfig.audioObjectType = get_audio_object_type_by_adts_header(&m_adts_fixed_header);
          decSpecConfig.samplingFrequencyIndex = m_adts_fixed_header.sampling_frequency_index;
          decSpecConfig.samplingFrequency = 0;
          decSpecConfig.channelConfiguration = m_adts_fixed_header.channel_configuration;
          decSpecConfig.extensionAudioObjectType = 0; // for SBR
          decSpecConfig.extensionSamplingFrequencyIndex = 0; // for SBR
          decSpecConfig.extensionSamplingFrequency = 0; // for SBR

          /// sGA_specific_config;
          decSpecConfig.GASpecificConfig.frameLengthFlag = 0; //1024
          decSpecConfig.GASpecificConfig.dependsOnCoreCoder = 0;
          decSpecConfig.GASpecificConfig.coreCoderDelay = 0;
          decSpecConfig.GASpecificConfig.extensionFlag = 0; // for SBR
          // decSpecConfig.GASpecificConfig.pce;

          INIT_BITSTREAM(&BS, p_SpecInfo.GetDataPointer());
          enc_audio_specific_config(&decSpecConfig, &BS);
          GET_BITS_COUNT(&BS, nbits);
          nbytes = nbits >> 3;
          p_SpecInfo.SetDataSize(nbytes);

          TM_esds_data *pEsds = &(m_headerMPEG4.trak[nStreamNumber]->mdia.minf.stbl.stsd.table[0].esds);
          pEsds->decoderConfigLen = nbytes;
          pEsds->decoderConfig = (Ipp8u *)malloc(pEsds->decoderConfigLen);
          memcpy(pEsds->decoderConfig, p_SpecInfo.GetDataPointer(), nbytes);

          TM_stsd_table_data *table = &(m_headerMPEG4.trak[nStreamNumber]->mdia.minf.stbl.stsd.table[0]);

          table->channels = (Ipp16u)(decSpecConfig.channelConfiguration);
          if (table->channels == 7) {
            table->channels = 8;
          }
          table->sample_size = 16;
          table->sample_rate = sampling_frequency_table[decSpecConfig.samplingFrequencyIndex];
        } else
        { // AAC from splitter case
          TM_esds_data *pEsds = &(m_headerMPEG4.trak[nStreamNumber]->mdia.minf.stbl.stsd.table[0].esds);
          pEsds->decoderConfigLen = (Ipp32u)lpData->GetDataSize();
          pEsds->decoderConfig = (Ipp8u *)malloc(pEsds->decoderConfigLen);
          memcpy(pEsds->decoderConfig, lpData->GetDataPointer(), lpData->GetDataSize());
          lpData->SetDataSize(0);
        }
      } // m_bFirstAudioFrame
    }

    return UMC_OK;
}

Status MP4Muxer::UnlockBuffer(MediaData *lpData, Ipp32s iTrack)
{
    Status umcRes;

    UMC_CHECK(lpData, UMC_OK);
    UMC_CHECK(lpData->GetDataSize(), UMC_OK);

    umcRes = Muxer::UnlockBuffer(lpData, iTrack);
    if (umcRes != UMC::UMC_OK) {
      return umcRes;
    }
    return MuxData(false);
}

Status MP4Muxer::PutEndOfStream(Ipp32s iTrack)
{
  Status umcRes;

  umcRes = Muxer::PutEndOfStream(iTrack);
  if ((umcRes != UMC::UMC_OK) && (umcRes != UMC::UMC_ERR_NULL_PTR)) {
    return umcRes;
  }
  return MuxData(false);

} //Status Muxer::PutEndOfStream(Ipp32u iTrack)

Status MP4Muxer::Flush(void)
{
    return MuxData(true);
} //Status MPEG2Muxer::Flush(void)

Status MP4Muxer::MuxData(bool bFlushMode)
{
    AutomaticMutex guard(m_PutDataMutex);
    Ipp32s nEndIndex;
    MediaData cData, *pcData;
    Status umcRes;
    size_t nSize;

    if (!bOutFileInitialized)
    {   /*** start writing into file (moved from Init to work with DS filters ***/
        if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
        {   //open file for data buffering
            InitTemporaryOutputFile1();
        } else
        {

            //write the very head to file ("ftypisom-isom")
            Write_head(m_pParams->m_lpDataWriter);
            m_nMdatSize = 0;
            Write_data(m_pParams->m_lpDataWriter, (Ipp8s*)"xxxxmdat", 8);
        }
        bOutFileInitialized = true;
    }

    for (;;) {
      //find shortest track, and add frame to it
      nEndIndex = FindMinTrackEnd(m_sTrack, m_uiTotalNumStreams);

      if (nEndIndex == -1) break; // EOS in all tracks

      umcRes = m_ppBuffers[nEndIndex]->LockOutputBuffer(&cData);
      if (umcRes == UMC_OK) {
        pcData = &cData;
        if (IS_VIDEO(nEndIndex)) {
          //maybe problem with video in one header, and audio in another
          if (needNewFragment(pcData, nEndIndex))
          {
            umcRes = DisposeDataToFile();
            UMC_CHECK_STATUS(umcRes)
          }

          umcRes = AdjustVideoData(pcData, nEndIndex);
          UMC_CHECK_STATUS(umcRes)
        } else {
          umcRes = AdjustAudioData(pcData, nEndIndex);
          UMC_CHECK_STATUS(umcRes)
        }

        if (pcData->GetDataSize() != 0) { // don't mux if decSpecInfo
          if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
          {
            vm_file *pFile = m_fTempOutput1;
            if(pFile == NULL)
                return UMC_ERR_OPEN_FAILED;
            if (IS_AUDIO(nEndIndex) && !m_bMoov)
              pFile = m_fTempOutput2[nEndIndex];
            nSize = vm_file_fwrite(pcData->GetDataPointer(), sizeof(Ipp8s), pcData->GetDataSize(), pFile);
          } else
          {
            nSize = pcData->GetDataSize();
            umcRes = m_pParams->m_lpDataWriter->PutData(pcData->GetDataPointer(), nSize);
            UMC_CHECK_STATUS(umcRes)
          }
          m_nMdatSize += nSize;
          SampleInfo(pcData, nEndIndex);
        }

        cData.SetDataSize(0);
        m_ppBuffers[nEndIndex]->UnLockOutputBuffer(&cData);
      } else if (umcRes == UMC_ERR_END_OF_STREAM) {
        m_sTrack[nEndIndex].m_nTrackStatus = 1;  // track is fully muxed
      } else if (umcRes == UMC_ERR_NOT_ENOUGH_DATA) { // no data
        if (bFlushMode == false) {
          break;  // break if not flush mode
        } else {
          m_sTrack[nEndIndex].m_nTrackStatus = 1;
        }
      } else {
        break; // probably error
      }
    }

    return UMC_OK;
}

Status MP4Muxer::WriteHeader()
{

    if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
    {
        return WriteHeaderAtStart();
    } else
    {
        return WriteHeaderAtEnd();
    }

}

Status MP4Muxer::WriteHeaderAtStart()
{
    Status umcRes = UMC_OK;

    //write the very head to file ("ftypisom-isom")
    Write_head(m_pParams->m_lpDataWriter);

    //update atoms in header (moov),
    umcRes = UpdateHeader();
    UMC_CHECK_STATUS(umcRes)

    //init atoms in header (moov)
    umcRes = InitAtoms();
    UMC_CHECK_STATUS(umcRes)

    //calculate sizes of all atoms in header (moov)
    CalculateSizes();

    // update header shift
    if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
    {
        for (Ipp32u nTrack = 0; nTrack < m_headerMPEG4.total_tracks; nTrack++)
        {
            TM_stbl_data& stbl = m_headerMPEG4.trak[nTrack]->mdia.minf.stbl;

            for (Ipp32u i = 0; i < m_sTrack[nTrack].m_nSamplesCount; i++)
                stbl.stco.table[i].offset += (m_headerMPEG4.size_atom + 24) + m_headerMVEX.size_atom;
        }
    }

    //write main(single, if true == m_bMoov) "header" to file
    umcRes = Write_moov(m_pParams->m_lpDataWriter, &m_headerMPEG4);
    UMC_CHECK_STATUS(umcRes)

    //if header has extensions, then file structure will has moof atoms
    if (!m_bMoov)
    {
        umcRes = Write_mvex(m_pParams->m_lpDataWriter, &m_headerMVEX);
        UMC_CHECK_STATUS(umcRes)
    }

    //write "mdat" atom size to first 4 bytes in file
    umcRes = WriteMdatSize();
    UMC_CHECK_STATUS(umcRes)

    //transfer data from temporary file to output resulting file
    umcRes = TransferDataFromTemporaryFile(m_fTempOutput1);

    return umcRes;
}

Status MP4Muxer::WriteHeaderAtEnd()
{
    Status umcRes = UMC_OK;
/*
    //write the very head to file ("ftypisom-isom")
    Write_head(m_pParams->m_lpDataWriter);
*/

    //update atoms in header (moov),
    umcRes = UpdateHeader();
    UMC_CHECK_STATUS(umcRes)

    //init atoms in header (moov)
    umcRes = InitAtoms();
    UMC_CHECK_STATUS(umcRes)

    //calculate sizes of all atoms in header (moov)
    CalculateSizes();

    //write main(single, if true == m_bMoov) "header" to file
    umcRes = Write_moov(m_pParams->m_lpDataWriter, &m_headerMPEG4);
    UMC_CHECK_STATUS(umcRes)

    //write "mdat" atom size to first 4 bytes in file
    umcRes = WriteMdatSize();
    UMC_CHECK_STATUS(umcRes)

    return umcRes;
}

Status MP4Muxer::WriteHeaderFragment()
{
    InitMoof();
    if (Write_moof(m_pParams->m_lpDataWriter, &m_headerMoof) != UMC_OK )
        return UMC_ERR_FAILED;
    WriteMdatSize();
    TransferDataFromTemporaryFile(m_fTempOutput1);
    for (Ipp32u i = 0; i < m_uiTotalNumStreams; i++)
    {
      if (IS_AUDIO(i)) {
        _SEEK_FILE_START(m_fTempOutput2[i]);
        TransferDataFromTemporaryFile(m_fTempOutput2[i]);
      }
    }
    return UMC_OK;
}

} // namespace UMC

#endif
