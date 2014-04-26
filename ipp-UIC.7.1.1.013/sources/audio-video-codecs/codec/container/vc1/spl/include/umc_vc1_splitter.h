/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) splitter
//
*/

#ifndef __UMC_VC1_SPL_H__
#define __UMC_VC1_SPL_H__

#include "umc_splitter.h"
#include "umc_vc1_spl_frame_constr.h"

namespace UMC
{
    class VC1Splitter : public Splitter
    {
    private:
        DYNAMIC_CAST_DECL(VC1Splitter, Splitter)

        vc1_frame_constructor*    m_frame_constructor;
        Ipp8u*              m_readBuf;
        Ipp32u              m_readBufSize;
        size_t              m_readDataSize;

        Ipp8u*              m_frameBuf;
        Ipp32u              m_frameBufSize;
        Ipp32u              m_frameSize;
        Ipp32u              m_NextFrame;

        Ipp32u              m_seqHeaderFlag; //1 - last data - sequence header
        MediaDataEx::_MediaDataEx *m_stCodes;
        MediaData*          m_readData;
        bool                m_bNotDelete;

    private:
        void DeleteReadBuf();
        void DeleteFrameBuf();

        Status ResizeFrameBuf(Ipp32u bufferSize);
        Status GetFirstSeqHeader(MediaDataEx* mediaData, Ipp32u splMode);
    public:
        VC1Splitter();
        virtual ~VC1Splitter();

        virtual Status  Init(SplitterParams *pParams);
        virtual Status  Close();    //Close splitter

        virtual Status GetNextData(MediaData*   data, Ipp32u /*nTrack*/);
        virtual Status CheckNextData(MediaData* data, Ipp32u /*nTrack*/);

        static SystemStreamType GetStreamType(DataReader *dr);

        virtual Status Run ();    //Run   splitter
        virtual Status Stop ();    //Stop  splitter

        virtual Status SetTimePosition(Ipp64f timePos);
        virtual Status GetTimePosition(Ipp64f& timePos);

        virtual Status GetInfo(SplitterInfo** ppInfo);
        virtual Status SetRate(Ipp64f rate);

        virtual Status SetTrackState(Ipp32u /*nTrack*/, TrackState /*iState*/);
        void    SetSpecialMode() {m_bNotDelete = true;} ;
};
}
#endif // __UMC_VC1_SPL_H__
