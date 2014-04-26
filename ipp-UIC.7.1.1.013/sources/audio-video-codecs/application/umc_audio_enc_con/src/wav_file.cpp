/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "wav_file.h"

using namespace UMC;


const Ipp32u ctRiff = 0x46464952;
const Ipp32u ctWave = 0x45564157;
const Ipp32u ctFmt  = 0x20746D66;
const Ipp32u ctData = 0x61746164;

struct t_chunk {
    Ipp32u m_ulId;
    Ipp32u m_ulSize;
};

const Ipp32u ctFmtSize = (2 + 2 + 4 + 4 + 2 + 2);

struct WaveHeader {
    Ipp32u id_riff;
    Ipp32u len_riff;

    Ipp32u id_chuck;
    Ipp32u fmt;
    Ipp32u len_chuck;

    Ipp16u type;
    Ipp16u channels;
    Ipp32u freq;
    Ipp32u bytes;
    Ipp16u align;
    Ipp16u bits;

    Ipp32u id_data;
    Ipp32u len_data;
};

struct wav_header {
    t_chunk riff_chunk;
    Ipp32u wave_signature;
    t_chunk fmt_chunk;

    Ipp16u nFormatTag;
    Ipp16u nChannels;
    Ipp32u nSamplesPerSec;
    Ipp32u nAvgBytesPerSec;
    Ipp16u nBlockAlign;
    Ipp16u wBitPerSample;

    t_chunk data_chunk;
};

struct wav_header_ex {
    t_chunk riff_chunk;
    Ipp32u wave_signature;
    t_chunk fmt_chunk;

    Ipp16u nFormatTag;
    Ipp16u nChannels;
    Ipp32u nSamplesPerSec;
    Ipp32u nAvgBytesPerSec;
    Ipp16u nBlockAlign;
    Ipp16u wBitPerSample;
    Ipp16u cbSize;

    union {
        Ipp16u wValidBitsPerSample; /* bits of precision */
        Ipp16u wSamplesPerBlock;    /* valid if wBitsPerSample==0 */
        Ipp16u wReserved;           /* If neither applies, set to zero. */
    };

    Ipp32u dwChannelMask;
    Ipp16u guid[16];

    t_chunk data_chunk;
};

WavFile::WavFile()
{
    m_is_info_valid = 0;
    m_file_handle = NULL;
    m_info.channels_number = 1;
    mReadOnly = 0;
}

WavFile::~WavFile()
{
}

Status WavFile::Open(vm_char *p_filename,
                               Ipp32u mode)
{
    Ipp32s res;
    vm_file   *p_file = NULL;

    if (mode & AFM_CREATE) {
        p_file = vm_file_open(p_filename, VM_STRING("wb"));
    } else {
        p_file = vm_file_open(p_filename, VM_STRING("rb"));
        mReadOnly = 1;
    }
    if (p_file == NULL) {
        return UMC_ERR_OPEN_FAILED;
    }
    m_file_handle = p_file;

    if (!(mode & AFM_CREATE)) {
        res = ReadHeaderInfo();
        if (res == -1)
            return UMC_ERR_OPEN_FAILED;
        if (res < 0) {
            if (mode & AFM_NO_CONTENT_WRN) {  // if WAV header is absent return ptr
                vm_file_seek((vm_file *) m_file_handle, 0, VM_FILE_SEEK_SET);
                return UMC_WRN_INVALID_STREAM;
            }
            vm_file_close((vm_file *) m_file_handle);
            return UMC_ERR_OPEN_FAILED;
        }
    } else {  // write WavFile mode
        size_t header_size = sizeof(WaveHeader);
        vm_file_seek((vm_file *) m_file_handle, (long)header_size, VM_FILE_SEEK_SET);
        m_data_size = 0;
    }
    return UMC_OK;
}

size_t WavFile::Read(void *p_data,
                          size_t size)
{
    size_t n;
#ifdef _BIG_ENDIAN_
    Ipp32s i, bsnum;
#endif

    if (m_file_handle == NULL) {
        return 0;
    }

    n = vm_file_read(p_data, 1, size, (vm_file *) m_file_handle);

#ifdef _BIG_ENDIAN_
    bsnum = m_info.resolution >> 3;

    if(bsnum == 2) {
        for (i = 0; i < n/2; i++)
            ((Ipp16u*)p_data)[i] = BIG_ENDIAN_SWAP16(((Ipp16u*)p_data)[i]);
    } else {
        if (bsnum == 3) {
            for (i = 0; i < n/3; i++) {
                Ipp8u t0 = ((Ipp8u*)p_data)[3*i];
                ((Ipp8u*)p_data)[3*i] = ((Ipp8u*)p_data)[3*i+2];
                ((Ipp8u*)p_data)[3*i+2] = t0;
            }
        } else {
            if (bsnum == 4) {
                for (i = 0; i < n/4; i++)
                    ((Ipp32u*)p_data)[i] = BIG_ENDIAN_SWAP32(((Ipp32u*)p_data)[i]);
            }
        }
    }
#endif
    return n;
}

size_t WavFile::Write(void *p_data, size_t size)
{
    size_t n;

    if (mReadOnly) {  // can't write if the file is RO
        return 0;
    }

    if (m_file_handle == NULL) {
        return 0;
    }

#ifdef _BIG_ENDIAN_
    Ipp32s i;
    for (i = 0; i < size/2; i++) {
        ((Ipp16u*)p_data)[i] = BIG_ENDIAN_SWAP16(((Ipp16u*)p_data)[i]);
    }
#endif
    n = vm_file_write(p_data, 1, (Ipp32s)size, (vm_file *) m_file_handle);

    m_data_size += (Ipp32u)size;
    return n;
}

Status WavFile::Close()
{
    size_t n, bits;
    WaveHeader wave;
    Ipp32s nCh = m_info.channels_number;
    Ipp32s CntFrameMulLenFrame = m_data_size / nCh / 2;

    if (m_file_handle == NULL) {
        return UMC_ERR_NULL_PTR;
    }

    if (!mReadOnly) {
        bits = 16;

        wave.id_riff = BIG_ENDIAN_SWAP32(0x46464952);
        wave.len_riff =BIG_ENDIAN_SWAP32(sizeof(WaveHeader) + ((CntFrameMulLenFrame) << 1) * nCh - 8);

        wave.id_chuck = BIG_ENDIAN_SWAP32(0x45564157);
        wave.fmt = BIG_ENDIAN_SWAP32(0x20746D66);
        wave.len_chuck = BIG_ENDIAN_SWAP32(0x00000010);

        wave.type = BIG_ENDIAN_SWAP16(0x0001);
        wave.channels = BIG_ENDIAN_SWAP16((Ipp16u)nCh);
        wave.freq = BIG_ENDIAN_SWAP32((Ipp32u)(m_info.sample_rate));
        wave.bytes = BIG_ENDIAN_SWAP32((m_info.sample_rate << 1) * nCh);
        wave.align = BIG_ENDIAN_SWAP16((Ipp16u)((nCh * bits) / 8));
        wave.bits = BIG_ENDIAN_SWAP16((Ipp16u)bits);

        wave.id_data = BIG_ENDIAN_SWAP32(0x61746164);
        wave.len_data = BIG_ENDIAN_SWAP32(((CntFrameMulLenFrame) << 1) * nCh);

        vm_file_seek((vm_file *) m_file_handle, 0, VM_FILE_SEEK_SET);
        n = vm_file_write(&wave, 1, sizeof(WaveHeader), (vm_file *) m_file_handle);
    }

    vm_file_close((vm_file *) m_file_handle);
    return UMC_OK;
}

Status WavFile::SetInfo(Info* p_info)
{
    if (p_info == NULL) {
        return UMC_ERR_NULL_PTR;
    }

    m_is_info_valid = 1;
    memcpy(&m_info, p_info, sizeof(m_info));

    return UMC_OK;
}

Status WavFile::GetInfo(Info* p_info)
{
    if (p_info == NULL) {
        return UMC_ERR_NULL_PTR;
    }

    if (m_is_info_valid) {
        memcpy(p_info, &m_info, sizeof(m_info));
        return UMC_OK;
    }

    return UMC_WRN_INFO_NOT_READY;
}

Ipp32s WavFile::ReadHeaderInfo()
{
    wav_header_ex header;
    t_chunk xChunk;
    Ipp32u m_ulId = 0;
    Ipp32u m_ulSize = 0;

    Ipp32u ulTemp = 0;
    Ipp32u ulOffset = 0;
    Ipp32u ulDataChunkOffset = 0;
    Ipp32s iFmtOk = 0;
    Ipp32s iDataOk = 0;

    header.nFormatTag = 0;
    header.nSamplesPerSec = 0;
    header.wBitPerSample = 0;
    header.nChannels = 0;

    if (vm_file_read(&xChunk, sizeof(xChunk), 1, (vm_file *) m_file_handle) != 1) {
// IO error
        return -1;
    }
    m_ulId = BIG_ENDIAN_SWAP32(xChunk.m_ulId);
    m_ulSize = BIG_ENDIAN_SWAP32(xChunk.m_ulSize);
    if (m_ulId != ctRiff) {
// File does not contain 'Riff' chunk !
        return -2;
    }
    if (vm_file_read(&ulTemp, sizeof(ulTemp), 1, (vm_file *) m_file_handle) != 1) {
// IO error
        return -1;
    }
    if (ulTemp != BIG_ENDIAN_SWAP32(ctWave)) {
// File does not contain 'Wave' signature !
        return -3;
    }

//  while (true) {
    for (;;) {
        if (iFmtOk && iDataOk)
            break;
        ulOffset = (Ipp32u)vm_file_tell((vm_file *) m_file_handle);

        if (vm_file_read(&xChunk, sizeof(xChunk), 1, (vm_file *) m_file_handle) != 1) {
// / IO error
            return -1;
        }

        m_ulId = BIG_ENDIAN_SWAP32(xChunk.m_ulId);
        m_ulSize = BIG_ENDIAN_SWAP32(xChunk.m_ulSize);

        switch (m_ulId) {
        case ctFmt:
            if (vm_file_read(&header.nFormatTag, ctFmtSize, 1, (vm_file *) m_file_handle) != 1) {
// / IO error
                return -1;
            }
            if (m_ulSize > ctFmtSize) {
                vm_file_seek((vm_file *) m_file_handle, m_ulSize - ctFmtSize, VM_FILE_SEEK_CUR);
            }
            iFmtOk = 1;
            break;
        case ctData:
            ulDataChunkOffset = ulOffset;
            vm_file_seek((vm_file *) m_file_handle, m_ulSize, VM_FILE_SEEK_CUR);
            iDataOk = 1;
            break;
        default:
            vm_file_seek((vm_file *) m_file_handle, m_ulSize, VM_FILE_SEEK_CUR);
            break;
        }
    }
    vm_file_seek((vm_file *) m_file_handle, ulDataChunkOffset + 8, VM_FILE_SEEK_SET);

    m_info.format_tag = BIG_ENDIAN_SWAP16(header.nFormatTag);
    m_info.sample_rate = BIG_ENDIAN_SWAP32(header.nSamplesPerSec);
    m_info.resolution = BIG_ENDIAN_SWAP16(header.wBitPerSample);
    m_info.channels_number = BIG_ENDIAN_SWAP16(header.nChannels);
    m_info.channel_mask = 0;
    m_is_info_valid = 1;
    return 0;
}
