/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DEMUXER_DEFS_H__
#define __UMC_DEMUXER_DEFS_H__

#include "ippdefs.h"
#include "umc_splitter.h"

#define DEM_DURATION_PRECISION 10.0
#define DEM_CMP_DURATION(ONE, TWO) (0 == (Ipp32s)((DEM_DURATION_PRECISION * (ONE - TWO))))
#define CHECK_OBJ_INIT(CRIT_PTR) if (!CRIT_PTR) return UMC_ERR_NOT_INITIALIZED;

static const Ipp32u PURE_STREAMS_PORTION = 16 * 1024;
#define MAX_TRACK 20

#define ID_PS_PACK          0xBA
#define ID_PS_SYS           0xBB
#define ID_PS_MAP           0xBC
#define ID_PRIVATE_1        0xBD
#define ID_PADDING          0xBE
#define ID_PRIVATE_2        0xBF
#define ID_AUDIO            0xC0
#define ID_AUDIO_MAX        32
#define ID_VIDEO            0xE0
#define ID_VIDEO_MAX        16
#define ID_ECM              0xF0
#define ID_EMM              0xF1
#define ID_DSMCC            0xF2
#define ID_ISO_13522        0xF3
#define ID_H222A            0xF4
#define ID_H222B            0xF5
#define ID_H222C            0xF6
#define ID_H222D            0xF7
#define ID_H222E            0xF8
#define ID_ANCILLARY        0xF9
#define ID_ISO_14496_SL     0xFA
#define ID_ISO_14496_FLEX   0xFB
#define ID_RESERVED_0xFC    0xFC
#define ID_RESERVED_0xFD    0xFD
#define ID_RESERVED_0xFE    0xFE
#define ID_PS_DIRECTORY     0xFF

#define SUB_ID_AC3          0x80
#define SUB_ID_AC3_MAX      8
#define SUB_ID_DTS          0x88
#define SUB_ID_DTS_MAX      8
#define SUB_ID_LPCM         0xA0
#define SUB_ID_LPCM_MAX     8
#define SUB_ID_SUBPIC       0x20
#define SUB_ID_SUBPIC_MAX   8

#define ID_TS_SYNC  0x47
#define ID_TS_PAT   0x00
#define ID_TS_CAT   0x01
#define ID_TS_DT    0x02
#define ID_TS_NULL  0x1FFF

#define SC_MPEG2_PIC    0x00000100
#define SC_MPEG2_USER   0x000001B2
#define SC_MPEG2_SEQ    0x000001B3
#define SC_AC3          0x0b77

// ISO/IEC 13818-1 descriptors
#define DESC_VIDEO  0x02
#define DESC_AUDIO  0x03
#define DESC_IOD    0x1D
#define DESC_FMC    0x1F
// ETSI EN 300 468 descriptors
#define DESC_VBI        0x45
#define DESC_VBI_TXT    0x46
#define DESC_TXT        0x56
#define DESC_AC3        0x6A
#define DESC_ENH_AC3    0x7A

#ifndef WAVE_FORMAT_DVM
#define WAVE_FORMAT_DVM 0x2000
#endif //WAVE_FORMAT_DVM

#ifndef WAVE_FORMAT_MPEGLAYER3
#define WAVE_FORMAT_MPEGLAYER3 0x0055
#endif //WAVE_FORMAT_MPEGLAYER3

#ifndef WAVE_FORMAT_AAC
#define WAVE_FORMAT_AAC 0x5473
#endif //WAVE_FORMAT_AAC

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 0x0001
#endif //WAVE_FORMAT_PCM

#define FREE_FORMAT_PROHIBIT // return 0 for free-format files
#define BUF_LEN 16384 // should be >= 4000

namespace UMC
{
    inline
    Ipp64f GetTimeStampFromPes(Ipp8u *pStream)
    {
        Ipp64s bits_32_30 = (pStream[0] >> 1) & 0x7;
        Ipp32u bits_29_15 = (pStream[1] << 7) + (pStream[2] >> 1);
        Ipp32u bits_14_00 = (pStream[3] << 7) + (pStream[4] >> 1);
        return ((bits_32_30 << 30) | (bits_29_15 << 15) | bits_14_00) / 90000.;
    }

    inline
    Ipp64f GetMpeg2TsSysTime(Ipp8u *pStream)
    {
        Ipp64s base_bits = ((Ipp64s)pStream[0] << 25) + ((Ipp64s)pStream[1] << 17) + ((Ipp64s)pStream[2] << 9);
        base_bits += ((Ipp64s)pStream[3] << 1) + ((Ipp64s)pStream[4] >> 7);
        Ipp32u ext_bits = ((pStream[4] & 0x01) << 8) + (pStream[5]);
        return (300 * base_bits + ext_bits) / 27000000.;
    }

    inline
    Ipp64f GetMpeg2PsSysTime(Ipp8u *pStream)
    {
        Ipp64s base_32_30 = (pStream[0] >> 3) & 7;
        Ipp64s base_29_15 = ((pStream[0] & 3) << 13) + (pStream[1] << 5) + ((pStream[2] >> 3) & 0x1f);
        Ipp64s base_14_00 = ((pStream[2] & 3) << 13) + (pStream[3] << 5) + ((pStream[4] >> 3) & 0x1f);
        Ipp64s base_bits = (base_32_30 << 30) + (base_29_15 << 15) + base_14_00;
        Ipp32u ext_bits = ((pStream[4] & 3) << 7) + ((pStream[5] >> 1) & 0x7f);
        return (300 * base_bits + ext_bits) / 27000000.;
    }

    inline
    Ipp64f GetMpeg1PsSysTime(Ipp8u *pStream)
    {
        Ipp64s base_32_30 = (pStream[0] >> 1) & 7;
        Ipp64s base_29_15 = (pStream[1] << 7) + (pStream[2] >> 1);
        Ipp64s base_14_00 = (pStream[3] << 7) + (pStream[4] >> 1);
        return ((base_32_30 << 30) + (base_29_15 << 15) + base_14_00) / 90000.;
    }

    struct ESDescriptor
    {
        // constructor
        ESDescriptor(void);
        // releases all pointers
        void Release(void);
        // ES_ID
        Ipp16u uiEsId;
        // average bitrate (0 - variable)
        Ipp32u avgBitrate;
        // 'predefined' from SLConfigDescriptor
        Ipp8u uiPredefinedSLConfig;
        // size of DecoderSpecificInfo
        Ipp32u uiDecSpecInfoLen;
        // DecoderSpecificInfo
        Ipp8u *pDecSpecInfo;
    };

    struct Mpeg2Es
    {
        // constructor
        Mpeg2Es(void);
        // destructor
        ~Mpeg2Es(void);
        // releases all pointers and resets uiEsInfoLen fields
        // NOTE: uiType, uiPid are not PMT-specified fields
        void Release(void);
        // allocates and copies ES info from another pointer
        // returns allocated pointer if success
        Ipp8u *SetInfo(Ipp8u *pPtr, Ipp16u uiLen);
        // stream_type
        Ipp8u uiType;
        // elementary_PID
        Ipp16u uiPid;
        // ES_info_length
        Ipp32u uiEsInfoLen;
        // ES_info
        Ipp8u *pEsInfo;
        // ES Descriptors from program_info, NULL if absent
        ESDescriptor *pESDSs;
    private:
        Mpeg2Es(const Mpeg2Es&) {} // Dummy copy constructor to avoid compilation warnings
        const Mpeg2Es& operator=(const Mpeg2Es&) { return *this; } // private is to avoid explicit/implicit calling in expressions
    };

    struct Mpeg2TsPmt
    {
        // constructor
        Mpeg2TsPmt(void);
        // destructor
        ~Mpeg2TsPmt(void);
        // releases all pointers and resets all PMT-specified fields
        // NOTE: uiProgInd, uiProgNum and uiProgPid are PAT-specified fields
        void Release(void);
        // allocates and copies program info from another pointer
        // returns allocated pointer if success
        Ipp8u *SetInfo(Ipp8u *pPtr, Ipp16u uiLen);
        // version_number
        Ipp8u uiVer;
        // section_number
        Ipp16u uiSecLen;
        // number of program it's determined in PAT
        Ipp16s uiProgInd;
        // program_number
        Ipp16u uiProgNum;
        // program_PID
        Ipp16u uiProgPid;
        // PCR_PID
        Ipp16u uiPcrPid;
        // program_info_length
        Ipp16u uiProgInfoLen;
        // pointer to program descriptors
        Ipp8u *pProgInfo;
        // number of elementary streams in program
        Ipp32u uiESs;
        // array of ESs
        Mpeg2Es *pESs;
    };

    struct Mpeg2TsPat
    {
        // constructor
        Mpeg2TsPat(void);
        // destructor
        ~Mpeg2TsPat(void);
        // releases all pointers
        void Release(void);
        // returns index of program with certain PID, -1 if not found
        Ipp32s GetProgIdx(Ipp32u uiPID);
        // version_number
        Ipp8u uiVer;
        // section_number
        Ipp16u uiSecLen;
        // transport_stream_id
        Ipp16u uiTsId;
        // number of programs
        Ipp32u uiProgs;
        // array of PMTs
        Mpeg2TsPmt *pProgs;
    };

    struct TrackRule
    {
        // default initializer
        TrackRule(void);
        // initializer
        TrackRule(TrackType mask, Ipp16s iPid, Ipp32s iLimit, bool bValid);
        // track type mask
        TrackType m_TypeMask;
        // defines pid of track that should be selected if found, -1 means no pid checking
        Ipp16s m_iPID;
        // maximum number of tracks that could match the rule, -1 means all detected tracks
        Ipp32s m_iLimit;
        // specifies if rule should be used after initialization
        bool m_bValidAfterInit;
    };

    struct RulesMatchingState
    {
        // initializer
        RulesMatchingState(void);
        // reset current state of rules matching process (do not affect rules array)
        void Reset(void);

        // number of rules
        Ipp32u m_uiRules;
        // pointer to array of rules
        TrackRule *m_pRules;
        // maximum number of tracks can match rules (-1 means infinite)
        Ipp32s m_iMaxTracks;
        // current number of tracks matching rules
        Ipp32s m_iMatchedTracks;
        // array of numbers of tracks currently matched to each rule
        Ipp32s *m_pRuleState;
        // is one of rules is valid after initialization
        bool m_bAnyRuleValidAfterInit;
        // is track checked with rules
        Ipp8u m_bIsTriedByRules[MAX_TRACK];
    };

    struct CheckPoint
    {
        // default initializer
        CheckPoint(void);
        // initializer
        CheckPoint(Ipp64f time, Ipp64u pos);
        // calculates byterate between two check points
        Ipp64f GetBPS(CheckPoint &rTarget);
        // system time at any position in system stream
        Ipp64f dTime;
        // position in system stream
        Ipp64u uiPos;
    };
}

#endif /* __UMC_DEMUXER_DEFS_H__ */
