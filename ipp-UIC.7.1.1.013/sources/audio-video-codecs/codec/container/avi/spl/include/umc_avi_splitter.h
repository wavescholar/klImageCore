/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AVI_SPL_H__
#define __UMC_AVI_SPL_H__

#include "umc_avi_types.h"
#include "umc_avi_spl_chunk.h"
#include "umc_index_spl.h"

namespace UMC
{
    class AVISplitter : public IndexSplitter
    {
    public:
        DYNAMIC_CAST_DECL(AVISplitter, IndexSplitter)

        // constructor
        AVISplitter();
        // destructor
        virtual ~AVISplitter();
        // initializes splitter, parses headers and fills info struct
        virtual Status Init(SplitterParams *pParams);
        // releases resources
        virtual Status Close();
        // creates buffers if they are not created yet and call parent's Run()
        virtual Status Run();

    protected:
        Status TerminateInit(Status umcRes);
        Status ReadFormat();
        Status GenerateIndex();
        Status ReadStreamsInfo(Ipp32u uiTrack);
        Status FillSplitterInfo();
        Status FillAudioStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo);
        Status FillVideoStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo);
        Status FillDvStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo);
        Status InitIndexUsingStandardIndex(Ipp32u nTrackNum, Ipp8u* pIndexBuffer,
            Ipp32u nEntriesInUse, Ipp64u qwBaseOffset, Ipp16u wLongsPerEntry);
        Status InitIndexUsingOldAVIIndex(Ipp8u* pIndexBuffer, Ipp32s nIndexSize,
            Ipp64u nMoviChunkStartAddr, Ipp64u nMoviChunkEndAddr);
        Status InitIndexUsingNewAVIIndex(Ipp32u nTrackNum, Ipp8u* pIndexBuffer, Ipp32s nIndexSize);

    protected:
        template <class T>
        struct SelfDestructingArray
        {
            explicit SelfDestructingArray(Ipp32u n) { m_arr = new T[n]; }
            ~SelfDestructingArray() { delete[] m_arr; }
            T& operator[](Ipp32u i) { return m_arr[i]; }
            const T& operator[](Ipp32u i) const { return m_arr[i]; }
        protected:
            T* m_arr;
        };

        // array of structures represent every AVI track
        AviTrack* m_pTrack;
        // Main AVI Header
        MainAviHeader m_AviHdr;
        // Avi chunk navigator
        AVIChunk m_AviChunkReader;
        // initialization flags
        Ipp32u m_uiFlags;
        // tracks' durations are calculated while index generating
        Ipp64f *m_pDuration;
        // is audio checked for VBR
        bool m_bCheckVBRAudio;
        // is audio VBR
        bool m_bIsVBRAudio;
    };

} // namespace UMC

#endif /* __UMC_AVI_SPL_H__ */
