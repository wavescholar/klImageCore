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

#include <stdio.h>
#include "umc_mp4_mux_defs.h"
#include "umc_mp4_mux.h"


namespace UMC
{
    /* return the index of the shortest track; -1 if all are muxed */
    Ipp32s MP4Muxer::FindMinTrackEnd(sMuxTMrack* sTrack, Ipp32s nTracksNum)
    {
      Ipp32s  nEndIndex = 0;
      Ipp64f  dEnd = MAX_TIME;
      Ipp32s  nMuxedTracks = 0;
      int i;

      for (i = 0; i < nTracksNum; i++) {
        if (sTrack && sTrack[i].m_nTrackStatus == 1) {  // track is muxed
          nMuxedTracks++;
          if (nMuxedTracks == nTracksNum) {
            return -1;
          } else {
            continue;
          }
        }
        if (sTrack && sTrack[i].m_nTrackEnd < dEnd) {
            dEnd = sTrack[i].m_nTrackEnd;
            nEndIndex = i;
        }
      }
      return nEndIndex;
    }

    Status MP4Muxer::InitTemporaryOutputFile1()
    {
        m_nMdatSize = 0;

        m_fTempOutput1 = vm_file_tmpfile();
        if(m_fTempOutput1 == NULL)
            return UMC_ERR_OPEN_FAILED;

        vm_file_fwrite("xxxxmdat", 8, 1, m_fTempOutput1);

        return UMC_OK;
    };

    Status MP4Muxer::InitAdditionalTemporaryOutputFiles()
    {
        m_fTempOutput2 = new vm_file*[m_uiTotalNumStreams];

        for (Ipp32u i = 0; i < m_uiTotalNumStreams; i++)
        {
          if (IS_AUDIO(i)) {
            m_fTempOutput2[i] = vm_file_tmpfile();
          } else {
            m_fTempOutput2[i] = NULL;
          }
        }

        return UMC_OK;
    };

    Status MP4Muxer::InitTracks(Ipp32s nTracks)
    {
        m_sTrack = _NEW_n(sMuxTMrack, nTracks);
        if(m_sTrack == NULL)
            return UMC_ERR_ALLOC;

        for (Ipp32s i = 0; i < nTracks; i++)
        {
            m_sTrack[i].m_pSamples          = _NEW_n(sMuxSample, INITIAL_TRACK_SIZE);
            UMC_CHECK_PTR(m_sTrack[i].m_pSamples)
            m_sTrack[i].m_nSamplesCount     = 0;
            m_sTrack[i].m_nTrackSize        = INITIAL_TRACK_SIZE;
            m_sTrack[i].m_nTrackEnd         = 0;
            m_sTrack[i].m_nTrackStatus      = 0;
            m_sTrack[i].m_nSttsEntries      = 1;  // at least 1 entry
            m_sTrack[i].m_nCttsEntries      = 0;  // is not needed
            m_sTrack[i].m_bSortPTSOnExit    = false;  // true in case ob B frames
        }

        return UMC_OK;
    };

    Status MP4Muxer::DisposeDataToFile()
    {
        if (m_bMoov)
        {
            m_bMoov = false;

            CalculateMvexExtendedSize(&m_headerMVEX);

            WriteHeaderAtStart();
        }
        else
        {
            WriteHeaderFragment();
        }

        //reopen file for data buffering
        InitTemporaryOutputFile1();
        InitAdditionalTemporaryOutputFiles();

        for (Ipp32u i = 0; i < m_headerMPEG4.total_tracks; i++)
        {
            m_sTrack[i].m_nSamplesCount = 0;
        }

        m_nHeaderSize = CalculateFragmentSize(&m_headerMoof);

        return UMC_OK;
    };

    Status MP4Muxer::GetBits(Ipp32s *data, Ipp32s nbits, Ipp32s *ptr, Ipp8u *buf)
    {
        Ipp32s   num, maxNum, curNum;
        Ipp32u   bits;

        num = 0;
        *data = 0;
        maxNum = BYTE_BIT - *ptr % BYTE_BIT;
        while (num < nbits)
        {
            curNum = IPP_MIN(nbits - num, maxNum);
            bits = (buf[*ptr / BYTE_BIT] >> (BYTE_BIT - *ptr % BYTE_BIT - curNum)) & ((1 << curNum) - 1);
            *data |= bits << (nbits - num - curNum);
            num += curNum;
            *ptr += curNum;
            maxNum = BYTE_BIT;
        }
        return UMC::UMC_OK;
    }

    FrameType MP4Muxer::GetPictureType(MediaData *lpData, Ipp32s nTrak)
    {
        if (!lpData) return NONE_PICTURE;
        FrameType frame_type = lpData->m_frameType;
        if (frame_type != NONE_PICTURE) {
          return frame_type;
        }
        if (UMC::H264_VIDEO == m_pTrackParams[nTrak].info.video->streamType)
        {
            if (NAL_UT_IDR_SLICE == (((Ipp8s*)lpData->GetDataPointer())[4] & NAL_UNITTYPE_BITS))
            {
                return I_PICTURE;
            } else {
                return NONE_PICTURE;
            }
        }
        if (UMC::MPEG4_VIDEO == m_pTrackParams[nTrak].info.video->streamType)
        {
//            Ipp32s nSizeToRead = 6;
            Ipp32s nCodingType = 0;

            Ipp32s nBitPos = 32; // start code

            GetBits(&nCodingType, 2/*two bits*/, &nBitPos, (Ipp8u*)lpData->GetBufferPointer());

            switch (nCodingType)
            {
              case 0: return I_PICTURE;
              case 1: return P_PICTURE;
              case 2: return B_PICTURE;
              case 3: return D_PICTURE; // SPRITE picture?
            }
        }
        return NONE_PICTURE;
    }

    bool MP4Muxer::needNewFragment(MediaData *lpData, Ipp32s nTrak)
    {
      if (m_pParams->m_lFlags & FLAG_FRAGMENTED_BY_HEADER_SIZE) {
        Ipp32u nHeaderSize = (m_bMoov ? m_nMoovSize : m_nMoofSize) - 28 * m_headerMPEG4.total_tracks;
        if (m_nHeaderSize >= nHeaderSize && I_PICTURE == GetPictureType(lpData, nTrak)) {
          return true;
        }
      }
      /*if (m_pParams->m_lFlags & FLAG_FRAGMENTED_NUMBER_FRAMES) {
        if ((m_sTrack[nTrak].m_nSamplesCount & 15) == 15) {
          vm_debug_trace_s(VM_DEBUG_PROGRESS, "needNewFragment");
          return true;
        }
      }*/
      if (m_pParams->m_lFlags & FLAG_FRAGMENTED_AT_I_PICTURES) {
        if (m_sTrack[nTrak].m_nSamplesCount > 10 && I_PICTURE == GetPictureType(lpData, nTrak)) {
          return true;
        }
      }
      return false;
    }

    Status MP4Muxer::TransferDataFromTemporaryFile(vm_file* fTemp)
    {
        DataWriter *m_lpDataWriter = m_pParams->m_lpDataWriter;
        size_t nsize = 0;
        Ipp8s buffer[1024 + 16];

        nsize = vm_file_fread(buffer, 1, 1024, fTemp);
        while(0 != nsize)
        {
            m_lpDataWriter->PutData(buffer, nsize);
            nsize = vm_file_fread(buffer, 1, 1024, fTemp);
        }
        vm_file_fclose(fTemp);

        return UMC_OK;
    };

    Status MP4Muxer::WriteMdatSize()
    {
        Status umcRes = UMC_OK;
        Ipp32u val = (Ipp32u)(m_nMdatSize + 8);
        Ipp8u data_char[4];

        data_char[0] = (Ipp8u)((val & 0xff000000) >> 24);
        data_char[1] = (Ipp8u)((val & 0xff0000) >> 16);
        data_char[2] = (Ipp8u)((val & 0xff00) >> 8);
        data_char[3] = (Ipp8u) (val & 0xff);


        if (m_pParams->m_lFlags & FLAG_START_WITH_HEADER)
        {
            _SEEK_FILE_START(m_fTempOutput1);
            umcRes = vm_file_fwrite(data_char, 4, 1, m_fTempOutput1) ? UMC_OK : UMC_ERR_FAILED;
            _SEEK_FILE_START(m_fTempOutput1);
        } else
        {
            size_t len = 4;
            umcRes = m_pParams->m_lpDataWriter->SetPosition(24); /*** "ftypisom-isom" ***/
            UMC_CHECK_STATUS(umcRes)
            umcRes = m_pParams->m_lpDataWriter->PutData(data_char, len);
        }

        return umcRes;
    }

    Status MP4Muxer::SetHeaderSizes(Ipp32s nMoovSize, Ipp32s nMoofSize)
    {
        m_nMoovSize = nMoovSize;
        m_nMoofSize = nMoofSize;

        return UMC_OK;
    };
} // namespace UMC

#endif
