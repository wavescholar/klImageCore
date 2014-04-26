/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_WAV_WRITER_H__
#define __UMC_WAV_WRITER_H__

#include "umc_media_data.h"
#include "umc_data_writer.h"

namespace UMC
{

class WAVEWriter
{
public:
    WAVEWriter(void)
    {
        m_pWriter      = NULL;
        m_iSampleRate  = 0;
        m_iSampleDepth = 0;
        m_iChannels    = 0;
        m_iDataSize    = 0;
        m_bWaveExt     = false;
    }

    Status Init(DataWriter *pWriter, Ipp32u iSampleRate, Ipp32u iSampleDepth, Ipp32u iChannels)
    {
        Status status;

        if(!pWriter || !iSampleRate || !iSampleDepth || !iChannels)
            return UMC_ERR_INVALID_PARAMS;

//        if(iSampleDepth > 16 || iChannels > 2)
//            m_bWaveExt = true;

        m_pWriter      = pWriter;
        m_iSampleRate  = iSampleRate;
        m_iSampleDepth = iSampleDepth;
        m_iChannels    = iChannels;

        UMC_CHECK_FUNC(status, m_pWriter->Put32uSwap('RIFF'));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap(0));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uSwap('WAVE'));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uSwap('fmt '));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap(16)); // 16, 18 or 40
        UMC_CHECK_FUNC(status, m_pWriter->Put16uNoSwap(UMC_WAVE_FORMAT_PCM));
        UMC_CHECK_FUNC(status, m_pWriter->Put16uNoSwap(m_iChannels));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap(m_iSampleRate));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap(m_iSampleRate * m_iChannels * m_iSampleDepth/8));
        UMC_CHECK_FUNC(status, m_pWriter->Put16uNoSwap(m_iChannels * m_iSampleDepth/8));
        UMC_CHECK_FUNC(status, m_pWriter->Put16uNoSwap(m_iSampleDepth));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uSwap('data'));
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap(0));

        return UMC_OK;
    }

    size_t WriteData(MediaData *pData)
    {
        size_t size;

        if(!m_pWriter)
            return 0;

        size = pData->GetDataSize();
        m_pWriter->PutData(pData->GetDataPointer(), size);
        m_iDataSize += size;

        return size;
    }

    Status Finalize()
    {
        Status status;

        if(!m_pWriter)
            return UMC_ERR_NOT_INITIALIZED;

        m_pWriter->SetPosition(0);
        m_pWriter->MovePosition(4);
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap((Ipp32u)(m_iDataSize + 36)));
        m_pWriter->MovePosition(32);
        UMC_CHECK_FUNC(status, m_pWriter->Put32uNoSwap((Ipp32u)m_iDataSize));

        m_pWriter->Close();

        return UMC_OK;
    }

protected:
    DataWriter *m_pWriter;
    Ipp32u      m_iSampleRate;
    Ipp32u      m_iSampleDepth;
    Ipp32u      m_iChannels;
    size_t      m_iDataSize;
    bool        m_bWaveExt;
};

};

#endif
