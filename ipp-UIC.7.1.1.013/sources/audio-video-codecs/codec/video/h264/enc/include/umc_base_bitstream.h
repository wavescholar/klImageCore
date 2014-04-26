//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef UMC_BASE_BITSTREAM_H__
#define UMC_BASE_BITSTREAM_H__

#include "umc_h264_defs.h"
#include "umc_h264_tables.h"

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#if (_MSC_FULL_VER >= 140050110)
//#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
#endif
#endif

//Fast CABAC implementation
//#define CABAC_FAST
#undef CABAC_FAST

// ---------------------------------------------------------------------------
//  CBaseBitstream - bitstream base class
// ---------------------------------------------------------------------------

extern const Ipp16u transIdxMPS[64];
extern const Ipp16u transIdxLPS[64];
extern const Ipp8u  rangeTabLPS[128][4];
extern const Ipp8u renormTAB[32];

// This macro is used to convert a Ipp32s VLC code into an
// Ipp32u VLC code, ready to pipe into PutVLCCode below.
// This saves having another function for that purpose and
// should be faster.
#define SIGNED_VLC_CODE(code) (2*ABS(code) - (code > 0))

typedef Ipp8u CABAC_CONTEXT;

const Ipp32s MB_TRANSFORM_SIZE_8X8_FLAG = 399; // ctxIdxOffset for transform_size_8x8_flag

enum // Syntax element type
{
    MB_SKIP_FLAG_P_SP               = 0,
    MB_SKIP_FLAG_B                  = 1,
    MB_FIELD_DECODING_FLAG          = 2,
    MB_TYPE_SI                      = 3,
    MB_TYPE_I                       = 4,
    MB_TYPE_P_SP                    = 5,
    MB_TYPE_B                       = 6,
    CODED_BLOCK_PATTERN_LUMA        = 7,
    CODED_BLOCK_PATTERN_CHROMA      = 8,
    MB_QP_DELTA                     = 9,
    PREV_INTRA4X4_PRED_MODE_FLAG    = 10,
    REM_INTRA4X4_PRED_MODE          = 11,
    INTRA_CHROMA_PRED_MODE          = 12,
    REF_IDX_L0                      = 13,
    REF_IDX_L1                      = 14,
    MVD_L0_0                        = 15,
    MVD_L1_0                        = 16,
    MVD_L0_1                        = 17,
    MVD_L1_1                        = 18,
    SUB_MB_TYPE_P_SP                = 19,
    SUB_MB_TYPE_B                   = 20,

    MAIN_SYNTAX_ELEMENT_NUMBER
};

// See table 9-30 of H.264 standard
enum // Syntax element type
{
    CODED_BLOCK_FLAG                = 0,
    SIGNIFICANT_COEFF_FLAG          = 1,
    LAST_SIGNIFICANT_COEFF_FLAG     = 2,
    COEFF_ABS_LEVEL_MINUS1          = 3,

    SYNTAX_ELEMENT_NUMBER
};

// See table 9-32 of H.264 standard
enum // Context block category
{
    BLOCK_LUMA_DC_LEVELS            = 0,
    BLOCK_LUMA_AC_LEVELS            = 1,
    BLOCK_LUMA_LEVELS               = 2,
    BLOCK_CHROMA_DC_LEVELS          = 3,
    BLOCK_CHROMA_AC_LEVELS          = 4,
    BLOCK_LUMA_64_LEVELS            = 5,

    BLOCK_CATEGORY_NUMBER
};
// See table 9-11 of H.264 standard
const Ipp32s ctxIdxOffset[MAIN_SYNTAX_ELEMENT_NUMBER] =
{
    11,
    24,
    70,
    0,
    3,
    14,
    27,
    73,
    77,
    60,
    68,
    69,
    64,
    54,
    54,
    40,
    40,
    47,
    47,
    21,
    36,
};

// See table 9-24 of H.264 standard
const Ipp32s ctxIdxOffsetFrameCoded[SYNTAX_ELEMENT_NUMBER] =
{
    85,
    105,
    166,
    227,

};

// See table 9-24 of H.264 standard
const Ipp32s ctxIdxOffsetFieldCoded[SYNTAX_ELEMENT_NUMBER] =
{
    85,
    277,
    338,
    227,
};

// See table 9-24 of H.264 standard
const Ipp32s ctxIdxOffsetFrameCoded_BlockCat_5[SYNTAX_ELEMENT_NUMBER] =
{
    0xffffffff, // na
    402,
    417,
    426
};

// See table 9-24 of H.264 standard
const Ipp32s ctxIdxOffsetFieldCoded_BlockCat_5[SYNTAX_ELEMENT_NUMBER] =
{
    0xffffffff, // na
    436,
    451,
    426
};

// See table 9-30 of H.264 standard
const Ipp32s ctxIdxBlockCatOffset[SYNTAX_ELEMENT_NUMBER][BLOCK_CATEGORY_NUMBER] =
{
    {0,  4,  8, 12, 16, -1},
    {0, 15, 29, 44, 47,  0},
    {0, 15, 29, 44, 47,  0},
    {0, 10, 20, 30, 39,  0}
};

// See table 9-34 of H.264 standard
extern const Ipp8s Table_9_34[3][64];
extern const Ipp32s p_bits[128];
extern const Ipp32u bitcount_EG0[268];
extern const Ipp32u bitcount_EG3[263];
extern const Ipp32s pref_bits[128][16];
extern __ALIGN8 const Ipp8u pref_state[128][16];

// LossRecovery return status
typedef enum
{
    BSLR_STATUS_ERROR,  // Loss recovery did not detect lost packet, or couldn't recover.
    BSLR_STATUS_OK,     // Loss recovery detected lost packet and recovered
    BSLR_STATUS_EOS     // Loss recovery detected lost packet but end of
                        // stream encountered (i.e. no more packet avail.)
} BSLR_Status;

struct H264BsBase
{
//protected:
    Ipp8u* m_pbs;  // m_pbs points to the current position of the buffer.
    Ipp8u* m_pbsBase; // m_pbsBase points to the first byte of the buffer.
    Ipp32u m_bitOffset; // Indicates the bit position (0 to 7) in the byte pointed by m_pbs.
    Ipp32u m_maxBsSize; // Maximum buffer size in bytes.};

//private:
    Ipp8u* m_pbsRBSPBase;  // Points to beginning of previous "Raw Byte Sequence Payload"
    Ipp32s num8x8Cshift2;

//public:
    CABAC_CONTEXT context_array[460];                       // (CABAC_CONTEXT []) array of cabac context(s)
    Ipp32u m_lcodIRange;                                    // arithmetic encoding engine variable
    Ipp32u m_lcodIOffset;                                   // arithmetic encoding engine variable
    Ipp32u m_nRegister;
#ifdef CABAC_FAST
    Ipp32s m_nReadyBits;
    Ipp32s m_nReadyBytes;
    Ipp32u m_nOutstandingChunks;
#else
    Ipp32u m_nReadyBits;
    Ipp32u m_nOutstandingBits;
#endif

};

// Returns the bit position of the buffer pointer relative to the beginning of the buffer.
inline
Ipp32u H264BsBase_GetBsOffset(
    H264BsBase* state)
{
    if (state->m_pbsBase == NULL)
        return (state->m_bitOffset + 128) >> 8;
    else
        return(Ipp32u(state->m_pbs - state->m_pbsBase) * 8 + state->m_bitOffset);
}

// Returns the size of the bitstream data in bytes based on the current position of the buffer pointer, m_pbs.
inline
Ipp32u H264BsBase_GetBsSize(
    H264BsBase* state)
{
    //Ipp32u size;
    //size = Ipp32u(state->m_pbs - state->m_pbsBase) + 1;
    //return(!state->m_bitOffset ? (size - 1) : size);
    return (H264BsBase_GetBsOffset(state) + 7) >> 3;
}

// Returns the base pointer to the beginning of the bitstream.
inline
Ipp8u* H264BsBase_GetBsBase(
    H264BsBase* state)
{
    return state->m_pbsBase;
}

// Returns the maximum bitstream size.
inline
Ipp32u H264BsBase_GetMaxBsSize(
    H264BsBase* state)
{
    return state->m_maxBsSize;
}

// Checks if read/write passed the maximum buffer size.
inline
bool H264BsBase_CheckBsLimit(
    H264BsBase* state)
{
    Ipp32u size;
    size = H264BsBase_GetBsSize(state);
    if (size > state->m_maxBsSize)
        return false;
    return true;
}

// Assigns new position to the buffer pointer.
inline
void H264BsBase_SetState(
    H264BsBase* state,
    Ipp8u* const pbs,
    const Ipp32u bitOffset)
{
    state->m_pbs       = pbs;
    state->m_bitOffset = bitOffset;
}

// Obtains current position of the buffer pointer.
inline
void H264BsBase_GetState(
    H264BsBase* state,
    Ipp8u** pbs,
    Ipp32u* bitOffset)
{
    *pbs       = state->m_pbs;
    *bitOffset = state->m_bitOffset;
}

// Advances buffer pointer with given number of bits.
inline
void H264BsBase_UpdateState(
    H264BsBase* state,
    const Ipp32u nbits)
{
    state->m_pbs      += (nbits + state->m_bitOffset) >> 3;
    state->m_bitOffset = (nbits + state->m_bitOffset) & 0x7;
}

// Clears the bitstream buffer.
inline
void H264BsBase_ClearBs(
    H264BsBase* state)
{
    memset(state->m_pbsBase, 0, state->m_maxBsSize);
}

// Resets pointer to the beginning and clears the bitstream buffer.
inline
void H264BsBase_Reset(
    H264BsBase* state)
{
    state->m_pbs = state->m_pbsBase;
    state->m_bitOffset = 0;
    H264BsBase_ClearBs(state);
}

// Write RBSP Trailing Bits to Byte Align
inline
void H264BsBase_WriteTrailingBits(
    H264BsBase* state)
{
    // Write Stop Bit
    state->m_pbs[0] = (Ipp8u)(state->m_pbs[0] | (Ipp8u)(0x01 << (7 - state->m_bitOffset)));

    state->m_pbs++;
    state->m_pbs[0] = 0;
    state->m_bitOffset = 0;
}

// Add zero bits to byte-align the buffer.
inline
void H264BsBase_ByteAlignWithZeros(
    H264BsBase* state)
{
    // No action needed if already byte aligned, i.e. !m_bitOffset
    if (state->m_bitOffset){ // note that prior write operation automatically clears the unused bits in the current byte*/
        state->m_pbs++;
        state->m_bitOffset = 0;
    }
}

// Add one bits to byte-align the buffer.
inline
void H264BsBase_ByteAlignWithOnes(
    H264BsBase* state)
{
    if (state->m_bitOffset){
        // No action needed if already byte aligned, i.e. !m_bitOffset
        state->m_pbs[0] = (Ipp8u)(state->m_pbs[0] | (Ipp8u)(0xff >> state->m_bitOffset));
        state->m_pbs++;
        state->m_pbs[0] = 0;
        state->m_bitOffset = 0;
    }
}

inline
Ipp32u H264BsBase_GetBsOffset_CABAC(
    H264BsBase* state)
{
#ifdef CABAC_FAST
    return Ipp32u(state->m_pbs - state->m_pbsBase) * 8 +
        state->m_bitOffset + state->m_nReadyBytes * 8 +
        state->m_nOutstandingChunks * 16 + 16 - state->m_nReadyBits;
#else
    return H264BsBase_GetBsOffset(state) + 32 - state->m_nReadyBits + state->m_nOutstandingBits;
#endif
}

void H264BsBase_CopyContextCABAC_I4x4(
    H264BsBase* state,
    H264BsBase* bstrm,
    Ipp32s isFrame);

void H264BsBase_CopyContextCABAC_I8x8(
    H264BsBase* state,
    H264BsBase* bstrm,
    Ipp32s isFrame);

void H264BsBase_CopyContextCABAC_I16x16(
    H264BsBase* state,
    H264BsBase* bstrm,
    Ipp32s isFrame);

void H264BsBase_CopyContextCABAC_InterP(
    H264BsBase* state,
    H264BsBase* bstrm,
    Ipp32s isFrame,
    Ipp32s is8x8);

void H264BsBase_CopyContextCABAC_InterB(
    H264BsBase* state,
    H264BsBase* bstrm,
    Ipp32s isFrame,
    Ipp32s is8x8);

void H264BsBase_CopyContextCABAC_Chroma(
    H264BsBase* state,
    H264BsBase* bstrm,
    int isFrame);


struct H264BsReal
{
    H264BsBase m_base;

//protected:
    CABAC_CONTEXT context_array_copy[460];                       // (CABAC_CONTEXT []) array of cabac context(s)
    Ipp8u* m_pbs_copy;
    Ipp32u m_bitOffset_copy;

    Ipp32u m_lcodIRange_copy;                                    // arithmetic encoding engine variable
    Ipp32u m_lcodIOffset_copy;                                   // arithmetic encoding engine variable
    Ipp32u m_nRegister_copy;
#ifdef CABAC_FAST
    Ipp32s m_nReadyBits_copy;
    Ipp32s m_nReadyBytes_copy;
    Ipp32u m_nOutstandingChunks_copy;
#else
    Ipp32u m_nReadyBits_copy;
    Ipp32u m_nOutstandingBits_copy;
#endif

//private:
    Ipp8u* m_pbsRBSPBase;  // Points to beginning of previous "Raw Byte Sequence Payload"
    Ipp32s num8x8Cshift2;
};

Status H264BsReal_Create(H264BsReal* state);

Status H264BsReal_Create(H264BsReal* state, Ipp8u* const pb, const Ipp32u maxsize, Ipp32s chroma_format_idc, Status &plr);

void H264BsReal_Destroy(
    H264BsReal* state);

void H264BsReal_SaveCABACState(
    void* state);

void H264BsReal_RestoreCABACState(
    void* state);

void H264BsReal_CopyContext_CABAC(
    void* state,
    H264BsBase* bstrm,
    Ipp32s isFrame,
    Ipp32s is8x8);

void H264BsReal_ResetBitStream_CABAC(
    void* state);

void H264BsReal_InitializeContextVariablesIntra_CABAC(
    void* state,
    Ipp32s SliceQPy);

void H264BsReal_InitializeContextVariablesInter_CABAC(
    void* state,
    Ipp32s SliceQPy,
    Ipp32s cabac_init_idc);

// Appends bits into the bitstream buffer.
#ifdef CABAC_FAST
void H264BsReal_WriteOneByte_CABAC(
    void* state,
    Ipp32u b);

void H264BsReal_WriteTwoBytes_CABAC(
    void* state,
    Ipp32u b);

#else

void H264BsReal_WriteBit_CABAC(
    H264BsReal* state,
    bool code);

void H264BsReal_WriteOutstandingZeroBit_CABAC(
    H264BsReal* state);

void H264BsReal_WriteOutstandingOneBit_CABAC(
    H264BsReal* state);

void H264BsReal_WriteOutstandingBit_CABAC(
    H264BsReal* state,
    bool code);

#endif

void H264BsReal_TerminateEncode_CABAC(
    void* state);

// Encode single bin from stream
void H264BsReal_EncodeSingleBin_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code);

void H264BsReal_EncodeFinalSingleBin_CABAC(
    void* state,
    Ipp32s code);

void H264BsReal_EncodeBins_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32u code,
    Ipp32s len);

void H264BsReal_EncodeBypass_CABAC(
    void* state,
    Ipp32s code);

// Appends bits into the bitstream buffer.
void H264BsReal_PutBit(
    void* state,
    Ipp32u code);

void H264BsReal_PutBits(
    void* state,
    Ipp32u code,
    Ipp32u length);

void H264BsReal_PutVLCBits(
    H264BsReal* state,
    const Ipp32u val,
    const Ipp32u len); // Writes one general VLC code to the bitstream

Ipp32u H264BsReal_PutVLCCode(
    void* state,
    const Ipp32u code);// Writes one general VLC code to the bitstream without knowing the code length... Returns the length of the code written.

//unary binarization
void H264BsReal_EncodeUnaryRepresentedSymbol_CABAC(
    H264BsReal* state,
    Ipp8u* ctx,
    Ipp32s ctxIdx,
    Ipp32s code,
    Ipp32s suppremum/* = 0x7fffffff*/);

//Exp Golomb binarization
void H264BsReal_EncodeExGRepresentedLevels_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code);

void H264BsReal_EncodeExGRepresentedMVS_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code);

void H264BsReal_Reset(
    void* state);

void H264BsReal_EncodeExGRepresentedSymbol_CABAC(
    void* state,
    Ipp32s code,
    Ipp32s log2ex);


struct H264BsFake
{
    H264BsBase m_base;

//private:
    Ipp8u* m_pbsRBSPBase;  // Points to beginning of previous "Raw Byte Sequence Payload"
    Ipp32s num8x8Cshift2;

};

Status H264BsFake_Create(
    H264BsFake* state);

void H264BsFake_Destroy(
    H264BsFake* state);

Status H264BsFake_Create(
    H264BsFake* state,
    Ipp8u* const pb,
    const Ipp32u maxsize,
    Ipp32s chroma_format_idc,
    Status &plr);

void H264BsFake_TerminateEncode_CABAC(
    void* state);

void H264BsFake_CopyContext_CABAC(
    void* state,
    H264BsBase* bstrm,
    Ipp32s isFrame,
    Ipp32s is8x8);

inline
void H264BsFake_SaveCABACState(
    void* /*state*/)
{
}

inline
void H264BsFake_RestoreCABACState(
    void* /*state*/)
{
}

inline
void H264BsFake_PutBit(
    void* state,
    Ipp32u)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_bitOffset += 256;
}

inline
void H264BsFake_PutBits(
    void* state,
    Ipp32u,
    Ipp32u length)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_bitOffset += length << 8;
}

inline
void H264BsFake_PutVLCBits(
    H264BsFake* state,
    const Ipp32u val,
    const Ipp32u len)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_bitOffset += ((((len - 1) & (~1)) + 1) << 8);
}

inline
Ipp32u H264BsFake_PutVLCCode(
    void* state,
    const Ipp32u val)
{
    H264BsFake* bs = (H264BsFake *)state;
    VM_ASSERT(val != 0);
    register Ipp32s NN = val + 1;
    register Ipp32s i = 0;
#if defined(__i386__) && defined(__GNUC__) && (__GNUC__ > 3) && !defined(__INTEL_COMPILER)
    i = 31 - __builtin_clz(NN);
#elif defined(__INTEL_COMPILER) && (defined(__i386__) || defined(WINDOWS)) && !defined(INTEL64)
    i = _bit_scan_reverse(NN);
#elif defined(_MSC_VER) && (_MSC_FULL_VER >= 140050110) && !defined(__INTEL_COMPILER)
    unsigned long idx;
    _BitScanReverse(&idx, (unsigned long)NN);
    i = (Ipp32s)idx;
#else
    if (NN & 0xffff0000) { i += 16; NN >>= 16; }
    if (NN & 0xff00) { i += 8; NN >>= 8; }
    if (NN & 0xf0) { i += 4; NN >>= 4; }
    if (NN & 0xc) { i += 2; NN >>= 2; }
    if (NN & 0x2) { i += 1; NN >>= 1; }
#endif

    register Ipp32u code_length;
    //code_length = (1 + (i << 1)) & (~1) + 1;
    code_length = 1 + (i << 1);
    bs->m_base.m_bitOffset += (code_length << 8);
    return code_length;
}

inline
void H264BsFake_EncodeBypass_CABAC(
    void* state,
    Ipp32s)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_bitOffset += 256;
}

inline
void H264BsFake_EncodeSingleBin_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code)
{
    H264BsFake* bs = (H264BsFake *)state;
    register Ipp8u pStateIdx = *ctx;
    //bs->m_base.m_bitOffset += ( code ? p_bits[pStateIdx^64] : p_bits[pStateIdx] );
    bs->m_base.m_bitOffset += p_bits[pStateIdx ^ (code << 6)];
    *ctx = transTbl[code][pStateIdx];
}

void H264BsFake_EncodeBins_CABAC(
    H264BsFake* state,
    Ipp8u* ctx,
    Ipp32u code,
    Ipp32s len);

inline
void H264BsFake_EncodeFinalSingleBin_CABAC(
    void* state,
    Ipp32s code)
{
    H264BsFake* bs = (H264BsFake *)state;
    if (code)
        bs->m_base.m_bitOffset += 7 << 8;
    else
        bs->m_base.m_bitOffset += p_bits[64];
}

//Exp Golomb binarization
inline
void H264BsFake_EncodeExGRepresentedLevels_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code)
{
    H264BsFake* bs = (H264BsFake *)state;
    if (code < 13)
    {
        bs->m_base.m_bitOffset += pref_bits[*ctx][code];
        *ctx = pref_state[*ctx][code];
    }
    else
    {
        bs->m_base.m_bitOffset  += pref_bits[*ctx][13];
        *ctx = pref_state[*ctx][13];
        if (code >= 65536 - 1 + 13)
        {
            bs->m_base.m_bitOffset += 256 * 32;
            code >>= 16;
        }

        if (code >= 256 - 1 + 13)
        {
            bs->m_base.m_bitOffset += 256 * 16;
            code >>= 8;
        }

        bs->m_base.m_bitOffset += bitcount_EG0[code];
    }
}

void H264BsFake_EncodeExGRepresentedMVS_CABAC(
    void* state,
    Ipp8u* ctx,
    Ipp32s code);

inline
Ipp32u H264BsFake_GetBsOffset(
    H264BsFake* state)
{
    H264BsFake* bs = (H264BsFake *)state;
    return (bs->m_base.m_bitOffset + 128) >> 8;
}

Ipp32u H264BsCommon_EndOfNAL(
    void* state,
    Ipp8u* const pout,
    Ipp8u const uIDC,
    NAL_Unit_Type const uUnitType,
    bool& startPicture);

#endif // UMC_BASE_BITSTREAM_H__
