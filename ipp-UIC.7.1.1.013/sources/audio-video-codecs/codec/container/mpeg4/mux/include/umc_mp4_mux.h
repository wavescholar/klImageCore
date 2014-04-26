/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP4_MUX_H__
#define __UMC_MP4_MUX_H__

#include "umc_muxer.h"
#include "umc_mp4_parser_w.h"
#include "umc_sample_buffer.h"

namespace UMC
{
  struct sH264_Header;
  struct sMuxTMrack;
  struct sMuxSample;

  class MP4Muxer : public Muxer
  {
  public:
    // Default constructor
    MP4Muxer();
    // Destructor
    virtual ~MP4Muxer();

    // Init muxer
    virtual Status Init(MuxerParams *lpInit);
    // Close muxer (release internal tools)
    virtual Status Close(void);
    // Flushes all data from buffers to output stream
    virtual Status Flush(void);

    virtual Status UnlockBuffer(MediaData *pData, Ipp32s iTrack);

    virtual Status PutEndOfStream(Ipp32s iTrack);

  protected:
    // Add video data to stream
    Status AdjustVideoData(MediaData* &lpData, Ipp32s nStreamNumber);
    // Add audio data to stream
    Status AdjustAudioData(MediaData* &lpData, Ipp32s nStreamNumber);
    // Provides time of first output sample
    Status GetOutputTime(Ipp32s /*nStreamNumber*/, Ipp64f &/*dTime*/) { return UMC_ERR_NOT_IMPLEMENTED; };

    Status MuxData(bool bFlushMode);

    MediaData emptyMediaData[1];
    MediaData* m_mH264DataOut;

    Ipp8s* m_pH264Data;
    Ipp32u m_iH264DataCurSize;
    Ipp32u m_nH264DataOffset;

    sH264_Header* m_pH264Header;
    bool m_bDataFromEncoder;

    Ipp32s* m_nIDRFrames;
    Ipp32s* m_FirstFrame;

    bool bMuxerInitialized;
    bool bOutFileInitialized;

    Ipp32s m_nMoovSize;
    Ipp32s m_nMoofSize;

    Mutex m_PutDataMutex;
    bool m_bMoov;

    TM_moov m_headerMPEG4;
    TM_moof m_headerMoof;
    TM_mvex m_headerMVEX;

    size_t  m_nMdatSize;
    Ipp32u  nTimeScale;

    Ipp32u *tracks_size;

    Ipp32u  header_size;

    vm_file  *m_fTempOutput1;
    vm_file **m_fTempOutput2;

    sMuxTMrack *m_sTrack;
    Ipp32u      m_nHeaderSize;

    /////////////////////////////////

    Status SetHeaderSizes(Ipp32s nMoovSize, Ipp32s nMoofSize);

    Ipp64u GetTrakDuration(Ipp32u ntrak);
    Ipp32s GetMP4VideoTypeID(VideoStreamType type);
    Ipp32s GetMP4AudioTypeID(AudioStreamType type);
    Ipp32s GetMP4StreamType(Ipp32s type);
    Status SampleInfo(MediaData *lpData, Ipp32s ntrak);
    Status WriteHeader();
    Status WriteHeaderAtStart();
    Status WriteHeaderAtEnd();
    Status WriteHeaderFragment();
    Status InitHeader();
    Status UpdateHeader();
    Status UpdateSmplDurations();

    Status WriteMdatSize();

    Ipp32s CalculateSizes();
    Ipp32s CalculateMvexExtendedSize(TM_mvex* mvex);
    Ipp32s CalculateFragmentSize(TM_moof *moof);

    Status InitTemporaryOutputFile1();
    Status InitAdditionalTemporaryOutputFiles();

    //functions
    Status InitTracks(Ipp32s nTracks);

    Status InitBuffers(Ipp32s nAudioTracks, Ipp32s nVideoTracks);

    //atom inits
    Status InitAtoms();
    Status InitStsz(Ipp32s nTrack);
    Status InitStco(Ipp32s nTrack);
    Status InitStts(Ipp32s nTrack);
    Status InitCtts(Ipp32s nTrack);
    Status InitStsc(Ipp32s nTrack);
    Status InitStss(Ipp32s nTrack);

    Status InitEsds();

    Status InitMoof();

    FrameType GetPictureType(MediaData *lpData, Ipp32s nTrak);
    Status GetBits(Ipp32s *data, Ipp32s nbits, Ipp32s *ptr, Ipp8u *buf);
    Status GetPSid(Ipp32s *pID, Ipp8s *pSrc);

    bool needNewFragment(MediaData *lpData, Ipp32s nTrak);

    Status DisposeDataToFile();

    Status TransferDataFromTemporaryFile(vm_file* fTemp);

    Ipp32s FindMinTrackEnd(sMuxTMrack* sTrack, Ipp32s nTracksNum);

    //H264 ES
    MediaData* TransformH264Frame(MediaData* mH264Data);
    Status CheckStartCode(Ipp32s nNal, Ipp8s* pData, Ipp32u nPos, MediaData* mH264Data, bool bLastPart);
  }; // class MP4Muxer

} // namespace UMC

#endif // __UMC_MP4_MUX_H__
