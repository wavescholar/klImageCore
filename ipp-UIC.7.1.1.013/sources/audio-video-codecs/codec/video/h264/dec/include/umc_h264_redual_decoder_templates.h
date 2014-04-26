/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_RESIDUAL_DECODER_TEMPLATES_H
#define __UMC_H264_RESIDUAL_DECODER_TEMPLATES_H

#include "umc_h264_dec_internal_cabac.h"

namespace UMC
{

#if defined(_MSC_VER)
// turn off the "code is not reachable from preceding code" remark
#pragma warning(disable : 128)
// turn off the "conditional expression is constant" warning
#pragma warning(disable: 280)
#pragma warning(disable: 4127)
#endif

enum
{
    LUMA_BLOCK_8X8_0            = 0x01,
    LUMA_BLOCK_8X8_1            = 0x02,
    LUMA_BLOCK_8X8_2            = 0x04,
    LUMA_BLOCK_8X8_3            = 0x08,

    CHROMA_AC_BLOCKS            = 0x20,
    CHROMA_DC_AC_BLOCKS         = 0x30
};

template <typename Coeffs, Ipp32s color_format, Ipp32s is_field>
class ResidualDecoderCAVLC
{
public:
    typedef Coeffs *  CoeffsPtr;

    virtual ~ResidualDecoderCAVLC() {}

    void DecodeCoefficients8x8_CAVLC(H264SegmentDecoderMultiThreaded * sd)
    {
        Ipp64u blockcbp;
        Ipp32u u8x8block = 1;
        Ipp32s uBlock;
        Ipp32u uBlockBit;

        Ipp32u uNC;
        Ipp32u uAboveIndex;
        Ipp32u uLeftIndex;
        Coeffs TempCoeffs[16];
        Ipp16s sNumCoeff;
        Ipp32s bFieldDecodeFlag = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo);
        CoeffsPtr pCurCoeffs;
        bool advance_to_next_block=false;
        Ipp8u bf = sd->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE || bFieldDecodeFlag;
        Ipp8u cbp = sd->m_cur_mb.LocalMacroblockInfo->cbp;

        // Initialize blockcbp bits from input cbp (from the bitstream)
        blockcbp = 0;   // no coeffs
        for (uBlock=0; uBlock<5; uBlock++)
        {
            if (cbp & u8x8block)
                blockcbp |= blockcbp_table[uBlock];
            u8x8block <<= 1;
        }

        if (cbp & u8x8block)
        {
            switch(color_format)
            {
            case 1:
                blockcbp |= CONST_LL(0x3ff0000) << 1;
                break;
            case 2:
                blockcbp |= CONST_LL(0x3ffff0000) << 1;
                break;
            case 3:
                blockcbp |= CONST_LL(0x3ffffffff0000) << 1;
                break;
            }
        }

        uBlock      = 1;        // start block loop with first luma 4x4
        uBlockBit   = 2;
        blockcbp  >>= 1;

        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;

        for (uBlock = 1; uBlock<FIRST_DC_CHROMA; uBlock++)
        {
            uAboveIndex = BlockNumToMBColLuma[uBlock];
            uLeftIndex  = BlockNumToMBRowLuma[uBlock];
            pCurCoeffs = TempCoeffs;

            sNumCoeff = 0;
            if ((blockcbp & 1) != 0)
            {
                uNC = sd->GetBlocksLumaContext(uAboveIndex,uLeftIndex);

                // Get CAVLC-code coefficient info from bitstream. Following call
                // updates pbs, bitOffset, sNumCoeff, sNumTrOnes, TrOneSigns,
                // and uTotalZero and fills CoeffBuf and uRunBeforeBuf.
                ippsSet_8u(0, (Ipp8u*)TempCoeffs, 16*sizeof(Coeffs));
                sd->m_pBitStream->GetCAVLCInfoLuma(uNC,
                                                    16,
                                                    sNumCoeff,
                                                    &pCurCoeffs,
                                                    bf);
    #ifdef STORE_VLC
                FILE *fp=fopen(__VLC_FILE__,"a+t");
                if (fp)
                {
                    fprintf(fp,"I4 L %d %d\n",uNC,sNumCoeff);
                    fclose(fp);
                }
    #endif

                //copy coeffs from tempbuffer
                for (Ipp32s i=0;i<16;i++)
                {
                    CoeffsPtr coeffsPtr = (CoeffsPtr)sd->m_pCoeffBlocksWrite;
                    coeffsPtr[hp_scan8x8[bf][((uBlock-1)&3)+i*4]] = TempCoeffs[mp_scan4x4[bf][i]];
                }

                advance_to_next_block = true;
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma |= uBlockBit;
            }

            if ((uBlock&3)==0 && advance_to_next_block) //end of 8x8 block
            {
                sd->m_pCoeffBlocksWrite += 64*(sizeof(Coeffs) / sizeof(sd->m_pCoeffBlocksWrite[0]));
                advance_to_next_block=false;
            }
            // Update num coeff storage for predicting future blocks
            sd->m_cur_mb.GetNumCoeffs()->numCoeffs[uLeftIndex * 4 + uAboveIndex] = (Ipp8u) sNumCoeff;

            blockcbp >>= 1;
            uBlockBit <<= 1;
        }   // uBlock

        if (!color_format)
            return;

#if defined(WINDOWS) && defined(__ICL)
#pragma warning(disable:111)
#endif
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;

        for (uBlock = FIRST_DC_CHROMA; uBlock < FIRST_AC_CHROMA; uBlock++)
        {
            if ((blockcbp & 1) != 0)
            {
                switch(color_format)
                {
                case 1:
                    sd->m_pBitStream->GetCAVLCInfoChroma0(sNumCoeff, (CoeffsPtr*)&sd->m_pCoeffBlocksWrite);
                    break;
                case 2:
                    sd->m_pBitStream->GetCAVLCInfoChroma2(sNumCoeff, (CoeffsPtr*)&sd->m_pCoeffBlocksWrite);
                    break;
                case 3:
                    sd->m_pBitStream->GetCAVLCInfoChroma4(sNumCoeff, (CoeffsPtr*)&sd->m_pCoeffBlocksWrite, 0);
                    break;
                default:
                    sNumCoeff = 0;
                    VM_ASSERT(false);
                    throw h264_exception(UMC_ERR_INVALID_STREAM);
                };

    #ifdef STORE_VLC
                FILE *fp=fopen(__VLC_FILE__,"a+t");
                if (fp)
                {
                    fprintf(fp,"I4 CDC %d %d\n",0,sNumCoeff);
                    fclose(fp);
                }
    #endif

                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[uBlock - FIRST_DC_CHROMA] |= (sNumCoeff ? 1 : 0);
            }

            blockcbp >>= 1;
            //  Can't early exit without setting numcoeff for rest of blocks
            if ((blockcbp == 0) && (uBlock == (FIRST_AC_CHROMA - 1)))
            {
                // no AC chroma coeffs, set chrroma NumCoef buffers to zero and exit
                Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;

                switch (color_format)
                {
                case CHROMA_FORMAT_420:
                    ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 4 * 2);
                    break;

                case CHROMA_FORMAT_422:
                    ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 8 * 2);
                    break;

                case CHROMA_FORMAT_444:
                    ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 16 * 2);
                    break;

                default:
                    break;
                }
                return;
            }
        }   // uBlock

        uBlockBit = 2;

        Ipp32s colorFactor;

        switch(color_format)
        {
        case 1:
            colorFactor = 1;
            break;
        case 2:
            colorFactor = 2;
            break;
        case 3:
            colorFactor = 4;
            break;
        default:
            colorFactor = 0;
            VM_ASSERT(false);
        };

        for (uBlock = FIRST_AC_CHROMA; uBlock < FIRST_AC_CHROMA + 8*colorFactor; uBlock++)
        {
            if (uBlock == (FIRST_AC_CHROMA + 4*colorFactor))
                uBlockBit = 2;

            uAboveIndex = BlockNumToMBColChromaAC[color_format][uBlock-FIRST_AC_CHROMA];
            uLeftIndex  = BlockNumToMBRowChromaAC[color_format][uBlock-FIRST_AC_CHROMA];
            Ipp32u addval = uBlock >= (FIRST_AC_CHROMA + 4*colorFactor) ?
                (FIRST_AC_CHROMA + 4*colorFactor - 3) : 16;
            sNumCoeff = 0;
            if ((blockcbp & 1) != 0)
            {
                switch(color_format)
                {
                case 1:
                    uNC = sd->GetBlocksChromaContextBMEH(uAboveIndex, uLeftIndex, uBlock >= (FIRST_AC_CHROMA + 4*colorFactor));
                    break;
                case 2:
                    uNC = sd->GetBlocksChromaContextH2(uAboveIndex, uLeftIndex, uBlock >= (FIRST_AC_CHROMA + 4*colorFactor));
                    break;
                case 3:
                    uNC = sd->GetBlocksChromaContextH4(uAboveIndex, uLeftIndex, uBlock >= (FIRST_AC_CHROMA + 4*colorFactor));
                    break;
                default:
                    uNC = 0;
                    VM_ASSERT(false);
                    throw h264_exception(UMC_ERR_INVALID_STREAM);
                };

                // Get CAVLC-code coefficient info from bitstream. Following call
                // updates pbs, bitOffset, sNumCoeff, sNumTrOnes, TrOneSigns,
                // and uTotalZero and fills CoeffBuf and uRunBeforeBuf.
                sd->m_pBitStream->GetCAVLCInfoLuma(uNC,
                                                    15,
                                                    sNumCoeff,
                                                    (CoeffsPtr*)&sd->m_pCoeffBlocksWrite,
                                                    bf);
    #ifdef STORE_VLC
                FILE *fp=fopen(__VLC_FILE__,"a+t");
                if (fp)
                {
                    fprintf(fp,"I4 CAC %d %d\n",uNC,sNumCoeff);
                    fclose(fp);
                }
    #endif

                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[uBlock >= (FIRST_AC_CHROMA + 4*colorFactor)]
                    |= (sNumCoeff ? uBlockBit : 0);
            }
            // Update num coeff storage for predicting future blocks
            sd->m_cur_mb.GetNumCoeffs()->numCoeffs[uLeftIndex * 2 * (1 + (Ipp32s)(color_format == 3))
                + uAboveIndex + addval] = (Ipp8u)sNumCoeff;

            blockcbp >>= 1;
            uBlockBit <<= 1;
        }   // uBlock


        // update buffer position pointer
    } // void DecodeCoefficients8x8_CAVLC(H264SegmentDecoderMultiThreaded * sd)

#define SET_TO_ZERO_COEFFS_NUMBER(x_pos, y_pos) \
    { \
        pNumCoeffsArray[(y_pos) * 4 + x_pos] = (Ipp8u) 0; \
        pNumCoeffsArray[(y_pos) * 4 + x_pos + 1] = (Ipp8u) 0; \
        pNumCoeffsArray[(y_pos + 1) * 4 + x_pos] = (Ipp8u) 0; \
        pNumCoeffsArray[(y_pos + 1) * 4 + x_pos + 1] = (Ipp8u) 0; \
    }

#define DECODE_EXTERNAL_LUMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /*  to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVCLContext; \
        iVCLContext = sd->GetBlocksLumaContextExternal(); \
        /* decode block coeffs */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVCLContext, \
                                                    iMaxNum, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr *) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[(y_pos) * 4 + x_pos] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_TOP_LUMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /*  to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVCLContext; \
        iVCLContext = sd->GetBlocksLumaContextTop(x_pos, pNumCoeffsArray[x_pos - 1]); \
        /* decode block coeffs */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVCLContext, \
                                                    iMaxNum, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr *) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[(y_pos) * 4 + x_pos] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_LEFT_LUMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /*  to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVCLContext; \
        iVCLContext = sd->GetBlocksLumaContextLeft(y_pos, pNumCoeffsArray[(y_pos - 1) * 4]); \
        /* decode block coeffs */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVCLContext, \
                                                    iMaxNum, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr *) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[(y_pos) * 4 + x_pos] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_INTERNAL_LUMA_BLOCK_CAVLC(block_num, raster_block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /*  to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVCLContext; \
        iVCLContext = sd->GetBlocksLumaContextInternal(raster_block_num, pNumCoeffsArray); \
        /* decode block coeffs */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVCLContext, \
                                                    iMaxNum, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr *) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[raster_block_num] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_CHROMA_DC_BLOCK_CAVLC(component) \
    { \
        Ipp16s iCoeffsNumber; \
        /* decode chroma DC coefficients */ \
        switch (color_format) \
        { \
        case CHROMA_FORMAT_420: \
            sd->m_pBitStream->GetCAVLCInfoChroma0(iCoeffsNumber, \
                   (CoeffsPtr*) &sd->m_pCoeffBlocksWrite); \
            break; \
        case CHROMA_FORMAT_422: \
            sd->m_pBitStream->GetCAVLCInfoChroma2(iCoeffsNumber, \
                   (CoeffsPtr*) &sd->m_pCoeffBlocksWrite); \
            break; \
        case CHROMA_FORMAT_444: \
            sd->m_pBitStream->GetCAVLCInfoChroma4(iCoeffsNumber, \
                   (CoeffsPtr*) &sd->m_pCoeffBlocksWrite, 0); \
            break; \
        default: \
            iCoeffsNumber = 0; \
            VM_ASSERT(false); \
            throw h264_exception(UMC_ERR_INVALID_STREAM); \
        } \
        /* update final CBP */ \
        uFinalCBP[component] = (iCoeffsNumber) ? (1) : (0); \
    }

#define DECODE_EXTERNAL_CHROMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /* to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVLCContext; \
        switch (color_format) \
        { \
        case CHROMA_FORMAT_420: \
            iVLCContext = sd->GetBlocksChromaContextBMEHExternal(iComponent); \
            break; \
        case CHROMA_FORMAT_422: \
            iVLCContext = sd->GetBlocksChromaContextH2(x_pos, y_pos, iComponent); \
            break; \
        case CHROMA_FORMAT_444: \
            iVLCContext = sd->GetBlocksChromaContextH4(x_pos, y_pos, iComponent); \
            break; \
        default: \
            iVLCContext = 0; \
            VM_ASSERT(false); \
            throw h264_exception(UMC_ERR_INVALID_STREAM); \
        }; \
        /* decode chrominance coefficients */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVLCContext, \
                                                    15, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr*) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP[iComponent] |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[block_num] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_TOP_CHROMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /* to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVLCContext; \
        switch (color_format) \
        { \
        case CHROMA_FORMAT_420: \
            iVLCContext = sd->GetBlocksChromaContextBMEHTop(x_pos, \
                                                            pNumCoeffsArray[0], \
                                                            iComponent); \
            break; \
        case CHROMA_FORMAT_422: \
            iVLCContext = sd->GetBlocksChromaContextH2(x_pos, y_pos, iComponent); \
            break; \
        case CHROMA_FORMAT_444: \
            iVLCContext = sd->GetBlocksChromaContextH4(x_pos, y_pos, iComponent); \
            break; \
        default: \
            iVLCContext = 0; \
            VM_ASSERT(false); \
            throw h264_exception(UMC_ERR_INVALID_STREAM);   \
        }; \
        /* decode chrominance coefficients */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVLCContext, \
                                                    15, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr*) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP[iComponent] |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[block_num] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_LEFT_CHROMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /* to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVLCContext; \
        switch (color_format) \
        { \
        case CHROMA_FORMAT_420: \
            iVLCContext = sd->GetBlocksChromaContextBMEHLeft(y_pos, \
                                                             pNumCoeffsArray[0], \
                                                             iComponent); \
            break; \
        case CHROMA_FORMAT_422: \
            iVLCContext = sd->GetBlocksChromaContextH2(x_pos, y_pos, iComponent); \
            break; \
        case CHROMA_FORMAT_444: \
            iVLCContext = sd->GetBlocksChromaContextH4(x_pos, y_pos, iComponent); \
            break; \
        default: \
            iVLCContext = 0; \
            VM_ASSERT(false); \
            throw h264_exception(UMC_ERR_INVALID_STREAM);   \
        }; \
        /* decode chrominance coefficients */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVLCContext, \
                                                    15, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr*) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP[iComponent] |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[block_num] = (Ipp8u) iCoeffsNumber; \
    }

#define DECODE_INTERNAL_CHROMA_BLOCK_CAVLC(x_pos, y_pos, block_num) \
    { \
        Ipp16s iCoeffsNumber; \
        /* to be honest, VLCContext is an average of coeffs numbers \
            of neighbouring blocks */ \
        Ipp32s iVLCContext; \
        switch (color_format) \
        { \
        case CHROMA_FORMAT_420: \
            iVLCContext = sd->GetBlocksChromaContextBMEHInternal(block_num, \
                                                                 pNumCoeffsArray); \
            break; \
        case CHROMA_FORMAT_422: \
            iVLCContext = sd->GetBlocksChromaContextH2(x_pos, y_pos, iComponent); \
            break; \
        case CHROMA_FORMAT_444: \
            iVLCContext = sd->GetBlocksChromaContextH4(x_pos, y_pos, iComponent); \
            break; \
        default: \
            iVLCContext = 0; \
            VM_ASSERT(false); \
            throw h264_exception(UMC_ERR_INVALID_STREAM);   \
        }; \
        /* decode chrominance coefficients */ \
        sd->m_pBitStream->GetCAVLCInfoLuma(iVLCContext, \
                                                    15, \
                                                    iCoeffsNumber, \
                                                    (CoeffsPtr*) &sd->m_pCoeffBlocksWrite, \
                                                    bf); \
        /* update final CBP */ \
        uFinalCBP[iComponent] |= (iCoeffsNumber) ? (1 << (block_num + 1)) : (0); \
        /* update a num coeff storage for a prediction of future blocks */ \
        pNumCoeffsArray[block_num] = (Ipp8u) iCoeffsNumber; \
    }

    void DecodeCoefficients16x16_CAVLC(H264SegmentDecoderMultiThreaded * sd)
    {
        Ipp32u iDCCBP = 0;

        // set field flag
        Ipp32s field_flag = (pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo) || is_field) ? 1 : 0;

        // decode luma DC block
        {
            Ipp16s sNumCoeff;
            Ipp32u uNC = 0;

            uNC = sd->GetDCBlocksLumaContext();
            sd->m_pBitStream->GetCAVLCInfoLuma(uNC,
                                    16,
                                    sNumCoeff,
                                    (CoeffsPtr *)&sd->m_pCoeffBlocksWrite,
                                    field_flag);

            if (sNumCoeff)
                iDCCBP = D_CBP_LUMA_DC;
        }

        DecodeCoefficients4x4_CAVLC(sd, 15);

        // save final CBP
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma |= iDCCBP;
    } // void DecodeCoefficients16x16_CAVLC(H264SegmentDecoderMultiThreaded * sd)

    void DecodeCoefficients4x4_CAVLC(H264SegmentDecoderMultiThreaded * sd,
                                       Ipp32s iMaxNum = 16)
    {
        Ipp32s bFieldDecodeFlag = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo);
        Ipp32s bf = sd->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE || bFieldDecodeFlag;
        Ipp32u cbp = sd->m_cur_mb.LocalMacroblockInfo->cbp;

        //
        // decode luminance blocks
        //
        {
            Ipp32u uFinalCBP;
            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;

            uFinalCBP = 0;
            if (cbp & LUMA_BLOCK_8X8_0)
            {
                DECODE_EXTERNAL_LUMA_BLOCK_CAVLC(0, 0, 0)
                DECODE_TOP_LUMA_BLOCK_CAVLC(1, 0, 1)
                DECODE_LEFT_LUMA_BLOCK_CAVLC(0, 1, 2)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(3, 5)
            }
            else
            {
                SET_TO_ZERO_COEFFS_NUMBER(0, 0)
            }

            if (cbp & LUMA_BLOCK_8X8_1)
            {
                DECODE_TOP_LUMA_BLOCK_CAVLC(2, 0, 4)
                DECODE_TOP_LUMA_BLOCK_CAVLC(3, 0, 5)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(6, 6)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(7, 7)
            }
            else
            {
                SET_TO_ZERO_COEFFS_NUMBER(2, 0)
            }

            if (cbp & LUMA_BLOCK_8X8_2)
            {
                DECODE_LEFT_LUMA_BLOCK_CAVLC(0, 2, 8)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(9, 9)
                DECODE_LEFT_LUMA_BLOCK_CAVLC(0, 3, 10)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(11, 13)
            }
            else
            {
                SET_TO_ZERO_COEFFS_NUMBER(0, 2)
            }

            if (cbp & LUMA_BLOCK_8X8_3)
            {
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(12, 10)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(13, 11)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(14, 14)
                DECODE_INTERNAL_LUMA_BLOCK_CAVLC(15, 15)
            }
            else
            {
                SET_TO_ZERO_COEFFS_NUMBER(2, 2)
            }

            // save final CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = uFinalCBP;
        }

        //
        // decode chrominance blocks
        //

        if (color_format && (cbp & CHROMA_DC_AC_BLOCKS))
        {
            Ipp32u uFinalCBP[2];

            // decode DC blocks
            DECODE_CHROMA_DC_BLOCK_CAVLC(0)
            DECODE_CHROMA_DC_BLOCK_CAVLC(1)

            if (cbp & CHROMA_AC_BLOCKS)
            {
                Ipp32s iComponent;

                //pNumCoeffsArray += iComponent * 4 + 16;
                Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs + 16;
                for (iComponent = 0; iComponent < 2; iComponent += 1)
                {
                    switch (color_format)
                    {
                    case CHROMA_FORMAT_420:

                        DECODE_EXTERNAL_CHROMA_BLOCK_CAVLC(0, 0, 0)
                        DECODE_TOP_CHROMA_BLOCK_CAVLC(1, 0, 1)
                        DECODE_LEFT_CHROMA_BLOCK_CAVLC(0, 1, 2)
                        DECODE_INTERNAL_CHROMA_BLOCK_CAVLC(1, 1, 3)
                        pNumCoeffsArray += 4;
                        break;

                    case CHROMA_FORMAT_422:
                        DECODE_EXTERNAL_CHROMA_BLOCK_CAVLC(0, 0, 0)
                        DECODE_TOP_CHROMA_BLOCK_CAVLC(1, 0, 1)
                        DECODE_LEFT_CHROMA_BLOCK_CAVLC(0, 1, 2)
                        DECODE_INTERNAL_CHROMA_BLOCK_CAVLC(1, 1, 3)
                        DECODE_LEFT_CHROMA_BLOCK_CAVLC(0, 2, 4)
                        DECODE_INTERNAL_CHROMA_BLOCK_CAVLC(1, 2, 5)
                        DECODE_LEFT_CHROMA_BLOCK_CAVLC(0, 3, 6)
                        DECODE_INTERNAL_CHROMA_BLOCK_CAVLC(1, 3, 7)
                        pNumCoeffsArray += 8;
                        break;

                    default:
                        break;
                    }
                }
            }

            // set the empty CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = uFinalCBP[0];
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = uFinalCBP[1];
        }
        else
        {
            // set the empty CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
        }

        // set zero values to a num coeffs storage
        if (color_format && (0 == (cbp & CHROMA_AC_BLOCKS)) )
        {
            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;

            switch (color_format)
            {
            case CHROMA_FORMAT_420:
                ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 4 * 2);
                break;

            case CHROMA_FORMAT_422:
                ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 8 * 2);
                break;

            case CHROMA_FORMAT_444:
                ippsSet_8u(0, pNumCoeffsArray + 16, sizeof(*pNumCoeffsArray) * 16 * 2);
                break;

            default:
                break;
            }
        }
    } // void DecodeCoefficients4x4_CAVLC(H264SegmentDecoderMultiThreaded * sd,
};

template <typename Coeffs, Ipp32s color_format, Ipp32s is_field>
class ResidualDecoderCABAC
{
public:
    typedef Coeffs *  CoeffsPtr;

    virtual ~ResidualDecoderCABAC() {}

    void DecodeCoefficients8x8_CABAC(H264SegmentDecoderMultiThreaded * sd)
    {
        Ipp8u cbp = sd->m_cur_mb.LocalMacroblockInfo->cbp;
        Ipp32u uBlockBit;
        CoeffsPtr pPosCoefbuf = (CoeffsPtr)sd->m_pCoeffBlocksWrite;

        Ipp32u i, j;
        Ipp32s top_bit     = 1;
        Ipp32s left_bit    = 1;
        Ipp32s def_bit = sd->m_cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTRA;
        const Ipp32u *ctxBase;
        const Ipp32s *single_scan;
        Ipp32s iMBAbove, iMBLeft;
        Ipp32u ctxIdxInc, iCtxBase;
        bool field_flag = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo) ||
            sd->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE;
        H264DecoderBlockNeighboursInfo* pN = &sd->m_cur_mb.CurrentBlockNeighbours;

        //this code need additional checking
        if (field_flag)
        {
            ctxBase = ctxIdxOffset8x8FieldCoded;
            single_scan = hp_scan8x8[1];
        } else {
            ctxBase = ctxIdxOffset8x8FrameCoded;
            single_scan = hp_scan8x8[0];
        }

        Ipp32u &cbp4x4_luma = sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma;
        cbp4x4_luma = 0;
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
        uBlockBit = (2+4+8+16);//4 bit set

        // luma coefficients
        for (i = 0; i < 4; i++)
        {
            if (cbp & mask_bit[i]) // are there any coeff in current block
            {
                //set bits for current block
                sd->m_pBitStream->ResidualBlock8x8_CABAC( field_flag,
                                                    single_scan, pPosCoefbuf);

                cbp4x4_luma |= uBlockBit;
                pPosCoefbuf += 64;
            }

            uBlockBit <<= 4;
        }

        // chroma 2x2 DC coeff
        if (cbp > 15)
        {
            if (pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo))
            {
                single_scan = mp_scan4x4[1];
            }
            else
            {
                if (sd->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE)
                    single_scan = mp_scan4x4[1];
                else
                    single_scan = mp_scan4x4[0];
            }

            // chroma 2x2 DC coeff
            if (field_flag)
                ctxBase = ctxIdxOffset4x4FieldCoded;
            else
                ctxBase = ctxIdxOffset4x4FrameCoded;

            Ipp32u numOfCoeffs = 0;
            switch (color_format)
            {
            case 1:
                numOfCoeffs = 4;
                break;
            case 2:
                numOfCoeffs = 8;
                break;
            case 3:
                numOfCoeffs = 16;
                break;
            };

            iCtxBase = ctxBase[CODED_BLOCK_FLAG] +
                    ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][BLOCK_CHROMA_DC_LEVELS + color_format];

            iMBAbove = pN->mb_above.mb_num;
            iMBLeft = pN->mbs_left[0].mb_num;

            for (i = 0; i < 2; i++)
            {
                if (0 <= iMBAbove)
                    top_bit = sd->m_mbinfo.mbs[iMBAbove].cbp4x4_chroma[i] & 1;
                else
                    top_bit = def_bit;

                if (0 <= iMBLeft)
                    left_bit = sd->m_mbinfo.mbs[iMBLeft].cbp4x4_chroma[i] & 1;
                else
                    left_bit = def_bit;

                ctxIdxInc = (top_bit<<1) + left_bit;

                if (sd->m_pBitStream->DecodeSingleBin_CABAC(iCtxBase + ctxIdxInc))
                {
                    const Ipp32s * sing_scan = 0;
                    switch (color_format)
                    {
                    case 1:
                        sing_scan = 0;
                        break;
                    case 2:
                        sing_scan = ChromaDC422RasterScan;
                        break;
                    case 3:
                        sing_scan = mp_scan4x4[0];
                        break;
                    }

                    BitStreamColorSpecific<Coeffs, color_format>::ResidualChromaDCBlock_CABAC(
                                                        ctxBase,
                                                        sing_scan,
                                                        pPosCoefbuf, sd->m_pBitStream);

                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[i] = 1;
                    pPosCoefbuf += numOfCoeffs;
                }
            }

            // chroma AC coeff, all zero from start_scan
            if (cbp > 31 && color_format)
            {
                iCtxBase = ctxBase[CODED_BLOCK_FLAG] +
                            ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][BLOCK_CHROMA_AC_LEVELS];

                for (j = 0; j < 2; j++)//plane
                {
                    Ipp32u &cbp4x4_chroma = sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[j];
                    Ipp32u addition = 16 + numOfCoeffs*j;
                    uBlockBit = 2;
                    for (i = 0; i < numOfCoeffs; i++, uBlockBit <<= 1)//block
                    {
                        Ipp32s raster_order_block = block_subblock_mapping_[i];

                        Ipp32s bit = i + 1;

                        top_bit     = def_bit;
                        left_bit    = def_bit;

                        //--- get bits from neighbouring blocks ---
                        if (sb_y[color_format][i])
                        {
                            if (color_format == 3)
                                top_bit = BIT_CHECK(cbp4x4_chroma, block_subblock_mapping_[raster_order_block - 4] + 1);
                            else
                                top_bit = BIT_CHECK(cbp4x4_chroma, bit - 2);
                        }
                        else
                        {
                            iMBAbove = pN->mb_above_chroma[0].mb_num;

                            if (0 <= iMBAbove)
                            {
                                if (color_format == 3)
                                    top_bit = BIT_CHECK(sd->m_mbinfo.mbs[iMBAbove].cbp4x4_chroma[j],
                                        block_subblock_mapping_[pN->mb_above_chroma[j].block_num + sb_x[3][i] - addition] + 1);
                                else
                                    top_bit = BIT_CHECK(sd->m_mbinfo.mbs[iMBAbove].cbp4x4_chroma[j],
                                        pN->mb_above_chroma[j].block_num + sb_x[color_format][i] - addition + 1);
                            }
                        }

                        if (sb_x[color_format][i])
                        {
                            if (color_format == 3)
                                left_bit = BIT_CHECK(cbp4x4_chroma, block_subblock_mapping_[raster_order_block - 1] + 1);
                            else
                                left_bit = BIT_CHECK(cbp4x4_chroma, bit - 1);
                        }
                        else
                        {
                            iMBLeft = pN->mbs_left_chroma[j][sb_y[color_format][i]].mb_num;

                            if (0 <= iMBLeft)
                            {
                                if (color_format == 3)
                                    left_bit = BIT_CHECK(sd->m_mbinfo.mbs[iMBLeft].cbp4x4_chroma[j],
                                        block_subblock_mapping_[pN->mbs_left_chroma[j][sb_y[3][i]].block_num - addition] + 1);
                                else
                                    left_bit = BIT_CHECK(sd->m_mbinfo.mbs[iMBLeft].cbp4x4_chroma[j],
                                        pN->mbs_left_chroma[j][sb_y[color_format][i]].block_num - addition + 1);
                            }
                        }

                        ctxIdxInc = (top_bit<<1) + left_bit;

                        if (sd->m_pBitStream->DecodeSingleBin_CABAC(iCtxBase + ctxIdxInc))
                        {
                            sd->m_pBitStream->ResidualBlock4x4_CABAC (BLOCK_CHROMA_AC_LEVELS,
                                                        ctxBase,
                                                        single_scan,
                                                        pPosCoefbuf,
                                                        14);

                            cbp4x4_chroma |= uBlockBit;

                            pPosCoefbuf += 16;
                        }
                    }
                }
            }
        }

        // update buffer position pointer
        sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon)pPosCoefbuf;
    }

    inline
    Ipp32s CheckCBP(Ipp32s iCBP, Ipp32s iBlockNum)
    {
        return (iCBP & iBlockCBPMask[iBlockNum]) ? (1) : (0);

    } // Ipp32s CheckCBP(Ipp32s iCBP, Ipp32s iBlockNum)

    inline
    Ipp32s CheckCBPChroma(Ipp32s iCBP, Ipp32s iBlockNum)
    {
        return (iCBP & iBlockCBPMaskChroma[iBlockNum]) ? (1) : (0);

    } // Ipp32s CheckCBPChroma(Ipp32s iCBP, Ipp32s iBlockNum)

#define DECODE_EXTERNAL_LUMA_BLOCK_CABAC(x_pos, y_pos, block_number) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrA; \
            mbAddrA = pN->mbs_left[y_pos]; \
            if (-1 < mbAddrA.mb_num) \
            { \
                condTermFlagA = CheckCBP(sd->m_mbinfo.mbs[mbAddrA.mb_num].cbp4x4_luma, \
                                         mbAddrA.block_num); \
            } \
            else \
                condTermFlagA = defTermFlag; \
        } \
        /* create flag for B macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrB; \
            mbAddrB = pN->mb_above; \
            if (-1 < mbAddrB.mb_num) \
            { \
                condTermFlagB = (sd->m_mbinfo.mbs[mbAddrB.mb_num].cbp4x4_luma & 0x0800) ? \
                                (1) : \
                                (0); \
            } \
            else \
                condTermFlagB = defTermFlag; \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(ctxBlockCat, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     iMaxNum); \
            /* update final CBP */ \
            uFinalCBP |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_TOP_LUMA_BLOCK_CABAC(x_pos, y_pos, block_number, left_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            condTermFlagA = (uFinalCBP & (1 << (left_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create flag for B macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrB; \
            mbAddrB = pN->mb_above; \
            if (-1 < mbAddrB.mb_num) \
            { \
                condTermFlagB = (sd->m_mbinfo.mbs[mbAddrB.mb_num].cbp4x4_luma & (0x0800 << (block_number))) ? \
                                (1) : \
                                (0); \
            } \
            else \
                condTermFlagB = defTermFlag; \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(ctxBlockCat, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     iMaxNum); \
            /* update final CBP */ \
            uFinalCBP |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_LEFT_LUMA_BLOCK_CABAC(x_pos, y_pos, block_number, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrA; \
            mbAddrA = pN->mbs_left[y_pos]; \
            if (-1 < mbAddrA.mb_num) \
            { \
                condTermFlagA = CheckCBP(sd->m_mbinfo.mbs[mbAddrA.mb_num].cbp4x4_luma, \
                                         mbAddrA.block_num); \
            } \
            else \
                condTermFlagA = defTermFlag; \
        } \
        /* create flag for B macroblock */ \
        { \
            condTermFlagB = (uFinalCBP & (1 << (top_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(ctxBlockCat, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     iMaxNum); \
            /* update final CBP */ \
            uFinalCBP |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_INTERNAL_LUMA_BLOCK_CABAC(x_pos, y_pos, block_number, left_block_num, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            condTermFlagA = (uFinalCBP & (1 << (left_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create flag for B macroblock */ \
        { \
            condTermFlagB = (uFinalCBP & (1 << (top_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(ctxBlockCat, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     iMaxNum); \
            /* update final CBP */ \
            uFinalCBP |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_CHROMA_DC_BLOCK_CABAC(component) \
    { \
        Ipp32s iNumCoeffs = (4 << (3 & (color_format - 1))); \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrA; \
            mbAddrA = pN->mbs_left[0]; \
            if (-1 < mbAddrA.mb_num) \
                condTermFlagA = (sd->m_mbinfo.mbs[mbAddrA.mb_num].cbp4x4_chroma[component] & 1); \
            else \
                condTermFlagA = defTermFlag; \
        } \
        /* create flag for B macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrB; \
            mbAddrB = pN->mb_above; \
            if (-1 < mbAddrB.mb_num) \
                condTermFlagB = (sd->m_mbinfo.mbs[mbAddrB.mb_num].cbp4x4_chroma[component] & 1); \
            else \
                condTermFlagB = defTermFlag; \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            const Ipp32s * pDCScan; \
            switch (color_format) \
            { \
            case 2: \
                pDCScan = ChromaDC422RasterScan; \
                break; \
            case 3: \
                pDCScan = mp_scan4x4[0]; \
                break; \
            default: \
                pDCScan = 0; \
                break; \
            } \
            BitStreamColorSpecific<Coeffs, color_format>::ResidualChromaDCBlock_CABAC(ctxBase, \
                                                                                      pDCScan, \
                                                                                      pPosCoefbuf, \
                                                                                      sd->m_pBitStream); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += iNumCoeffs; \
            /* update final CBP */ \
            uFinalCBP[component] = 1; \
        } \
        else \
            uFinalCBP[component] = 0; \
    }

#define DECODE_EXTERNAL_CHROMA_BLOCK_CABAC(x_pos, y_pos, block_number, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrA; \
            mbAddrA = pN->mbs_left_chroma[iComponent][y_pos]; \
            if (-1 < mbAddrA.mb_num) \
            { \
                condTermFlagA = CheckCBPChroma(sd->m_mbinfo.mbs[mbAddrA.mb_num].cbp4x4_chroma[iComponent], \
                                               mbAddrA.block_num - iBlocksBefore); \
            } \
            else \
                condTermFlagA = defTermFlag; \
        } \
        /* create flag for B macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrB; \
            mbAddrB = pN->mb_above_chroma[iComponent]; \
            if (-1 < mbAddrB.mb_num) \
            { \
                condTermFlagB = (sd->m_mbinfo.mbs[mbAddrB.mb_num].cbp4x4_chroma[iComponent] & (1 << (top_block_num + 1))) ? \
                                (1) : \
                                (0); \
            } \
            else \
                condTermFlagB = defTermFlag; \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(BLOCK_CHROMA_AC_LEVELS, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     14); \
            /* update final CBP */ \
            uFinalCBP[iComponent] |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_TOP_CHROMA_BLOCK_CABAC(x_pos, y_pos, block_number, left_block_num, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            condTermFlagA = (uFinalCBP[iComponent] & (1 << (left_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create flag for B macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrB; \
            mbAddrB = pN->mb_above_chroma[iComponent]; \
            if (-1 < mbAddrB.mb_num) \
            { \
                condTermFlagB = (sd->m_mbinfo.mbs[mbAddrB.mb_num].cbp4x4_chroma[iComponent] & (1 << (top_block_num + 1))) ? \
                                (1) : \
                                (0); \
            } \
            else \
                condTermFlagB = defTermFlag; \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(BLOCK_CHROMA_AC_LEVELS, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     14); \
            /* update final CBP */ \
            uFinalCBP[iComponent] |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_LEFT_CHROMA_BLOCK_CABAC(x_pos, y_pos, block_number, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            H264DecoderBlockLocation mbAddrA; \
            mbAddrA = pN->mbs_left_chroma[iComponent][y_pos]; \
            if (-1 < mbAddrA.mb_num) \
            { \
                condTermFlagA = CheckCBPChroma(sd->m_mbinfo.mbs[mbAddrA.mb_num].cbp4x4_chroma[iComponent], \
                                               mbAddrA.block_num - iBlocksBefore); \
            } \
            else \
                condTermFlagA = defTermFlag; \
        } \
        /* create flag for B macroblock */ \
        { \
            condTermFlagB = (uFinalCBP[iComponent] & (1 << (top_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(BLOCK_CHROMA_AC_LEVELS, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     14); \
            /* update final CBP */ \
            uFinalCBP[iComponent] |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

#define DECODE_INTERNAL_CHROMA_BLOCK_CABAC(x_pos, y_pos, block_number, left_block_num, top_block_num) \
    { \
        Ipp32s ctxIdxInc; \
        Ipp32s condTermFlagA, condTermFlagB; \
        /* create flag for A macroblock */ \
        { \
            condTermFlagA = (uFinalCBP[iComponent] & (1 << (left_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create flag for B macroblock */ \
        { \
            condTermFlagB = (uFinalCBP[iComponent] & (1 << (top_block_num + 1))) ? \
                            (1) : \
                            (0); \
        } \
        /* create context increment */ \
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB; \
        /* decode block coeffs */ \
        if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag + ctxIdxInc)) \
        { \
            sd->m_pBitStream->ResidualBlock4x4_CABAC(BLOCK_CHROMA_AC_LEVELS, \
                                                     ctxBase, \
                                                     pScan, \
                                                     pPosCoefbuf, \
                                                     14); \
            /* update final CBP */ \
            uFinalCBP[iComponent] |= (1 << (block_number + 1)); \
            /* update coefficient buffer pointer */ \
            pPosCoefbuf += 16; \
        } \
    }

    void DecodeCoefficients16x16_CABAC(H264SegmentDecoderMultiThreaded * sd)
    {
        Ipp32u iDCCBP = 0;

        const Ipp32u *ctxBase;
        const Ipp32s *pScan;
        bool field_flag = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo) ||
                          sd->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE;

        H264DecoderBlockNeighboursInfo *pN = &sd->m_cur_mb.CurrentBlockNeighbours;

        //this code need additional checking
        if (field_flag)
        {
            ctxBase = ctxIdxOffset4x4FieldCoded;
            pScan = mp_scan4x4[1];
        }
        else
        {
            ctxBase = ctxIdxOffset4x4FrameCoded;
            pScan = mp_scan4x4[0];
        }

        {

            Ipp32s iMBAbove, iMBLeft;
            Ipp32s condTermFlagA, condTermFlagB;
            Ipp32u ctxIdxInc, ctxCodedBlockFlag;

            iMBAbove = pN->mb_above.mb_num;
            iMBLeft  = pN->mbs_left[0].mb_num;

            // create flag for A macroblock
            if (0 <= iMBLeft)
                condTermFlagA = (Ipp32u) (sd->m_mbinfo.mbs[iMBLeft].cbp4x4_luma & 1);
            else
                condTermFlagA = 1;

            // create flag for B macroblock
            if (0 <= iMBAbove)
                condTermFlagB = (Ipp32u) (sd->m_mbinfo.mbs[iMBAbove].cbp4x4_luma & 1);
            else
                condTermFlagB = 1;

            // create context increment
            ctxIdxInc = condTermFlagA + 2 * condTermFlagB;

            // select context for coded block flag
            ctxCodedBlockFlag = ctxBase[CODED_BLOCK_FLAG] +
                                ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][BLOCK_LUMA_DC_LEVELS];

            // set bits for current block
            if (sd->m_pBitStream->DecodeSingleBin_CABAC(ctxCodedBlockFlag +
                                                        ctxIdxInc))
            {
                typedef Coeffs * CoeffsPtr;
                CoeffsPtr pPosCoefbuf = (CoeffsPtr)sd->m_pCoeffBlocksWrite;

                sd->m_pBitStream->ResidualBlock4x4_CABAC(BLOCK_LUMA_DC_LEVELS,
                                                         ctxBase,
                                                         pScan,
                                                         pPosCoefbuf,
                                                         15);
                iDCCBP = 1;
                // update buffer position pointer
                pPosCoefbuf += 16;
                sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon) pPosCoefbuf;

            }
        }

        DecodeCoefficients4x4_CABAC(sd, 14);

        // save final CBP
        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma |= iDCCBP;
    } // void DecodeCoefficients16x16_CABAC(H264SegmentDecoderMultiThreaded * sd)

    void DecodeCoefficients4x4_CABAC(H264SegmentDecoderMultiThreaded * sd,
                                       Ipp32s iMaxNum = 15)
    {
        CoeffsPtr pPosCoefbuf = (CoeffsPtr)sd->m_pCoeffBlocksWrite;

        Ipp32u cbp = sd->m_cur_mb.LocalMacroblockInfo->cbp;
        const Ipp32s *pScan;
        bool field_flag = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo) ||
                          sd->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE;
        H264DecoderBlockNeighboursInfo *pN = &sd->m_cur_mb.CurrentBlockNeighbours;

        Ipp32s defTermFlag;
        const Ipp32u *ctxBase;
        Ipp32u ctxBlockCat;
        Ipp32u ctxCodedBlockFlag;

        // set default bit
        defTermFlag = (sd->m_cur_mb.GlobalMacroblockInfo->mbtype < MBTYPE_PCM) ? (1) : (0);

        // this code need additional checking
        if (field_flag)
        {
            ctxBase = ctxIdxOffset4x4FieldCoded;
            pScan = mp_scan4x4[1];
        }
        else
        {
            ctxBase = ctxIdxOffset4x4FrameCoded;
            pScan = mp_scan4x4[0];
        }

        // select context for block data
        ctxBlockCat = (14 == iMaxNum) ? (BLOCK_LUMA_AC_LEVELS) : (BLOCK_LUMA_LEVELS);
        // select context for coded block flag
        ctxCodedBlockFlag = (ctxBase[CODED_BLOCK_FLAG] +
                             ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][ctxBlockCat]);


        //
        // decode luminance blocks
        //
        {
            Ipp32u uFinalCBP = 0;

            if (cbp & LUMA_BLOCK_8X8_0)
            {
                DECODE_EXTERNAL_LUMA_BLOCK_CABAC(0, 0, 0)
                DECODE_TOP_LUMA_BLOCK_CABAC(1, 0, 1, 0)
                DECODE_LEFT_LUMA_BLOCK_CABAC(0, 1, 2, 0)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(1, 1, 3, 2, 1)
            }

            if (cbp & LUMA_BLOCK_8X8_1)
            {
                DECODE_TOP_LUMA_BLOCK_CABAC(2, 0, 4, 1)
                DECODE_TOP_LUMA_BLOCK_CABAC(3, 0, 5, 4)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(2, 1, 6, 3, 4)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(3, 1, 7, 6, 5)
            }

            if (cbp & LUMA_BLOCK_8X8_2)
            {
                DECODE_LEFT_LUMA_BLOCK_CABAC(0, 2, 8, 2)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(1, 2, 9, 8, 3)
                DECODE_LEFT_LUMA_BLOCK_CABAC(0, 3, 10, 8)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(1, 3, 11, 10, 9)
            }

            if (cbp & LUMA_BLOCK_8X8_3)
            {
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(2, 2, 12, 9, 6)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(3, 2, 13, 12, 7)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(2, 3, 14, 11, 12)
                DECODE_INTERNAL_LUMA_BLOCK_CABAC(3, 3, 15, 14, 13)
            }

            // save final CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = uFinalCBP;
        }

        //
        // decode chrominance blocks
        //

        if (cbp & CHROMA_DC_AC_BLOCKS)
        {
            Ipp32u uFinalCBP[2];

            // select new context
            ctxCodedBlockFlag = ctxBase[CODED_BLOCK_FLAG] +
                                ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][BLOCK_CHROMA_DC_LEVELS +
                                                                       color_format];

            // decode DC blocks
            DECODE_CHROMA_DC_BLOCK_CABAC(0)
            DECODE_CHROMA_DC_BLOCK_CABAC(1)

            if (cbp & CHROMA_AC_BLOCKS)
            {
                Ipp32s iComponent;

                // select new context
                ctxCodedBlockFlag = ctxBase[CODED_BLOCK_FLAG] +
                                    ctxIdxBlockCatOffset[CODED_BLOCK_FLAG][BLOCK_CHROMA_AC_LEVELS];

                for (iComponent = 0; iComponent < 2; iComponent += 1)
                {
                    Ipp32s iBlocksBefore = 16 + iComponent * (2 << color_format);

                    switch (color_format)
                    {
                    case CHROMA_FORMAT_420:
                        DECODE_EXTERNAL_CHROMA_BLOCK_CABAC(0, 0, 0, 2)
                        DECODE_TOP_CHROMA_BLOCK_CABAC(1, 0, 1, 0, 3)
                        DECODE_LEFT_CHROMA_BLOCK_CABAC(0, 1, 2, 0)
                        DECODE_INTERNAL_CHROMA_BLOCK_CABAC(1, 1, 3, 2, 1)
                        break;

                    case CHROMA_FORMAT_422:
                        DECODE_EXTERNAL_CHROMA_BLOCK_CABAC(0, 0, 0, 6)
                        DECODE_TOP_CHROMA_BLOCK_CABAC(1, 0, 1, 0, 7)
                        DECODE_LEFT_CHROMA_BLOCK_CABAC(0, 1, 2, 0)
                        DECODE_INTERNAL_CHROMA_BLOCK_CABAC(1, 1, 3, 2, 1)
                        DECODE_LEFT_CHROMA_BLOCK_CABAC(0, 2, 4, 2)
                        DECODE_INTERNAL_CHROMA_BLOCK_CABAC(1, 2, 5, 4, 3)
                        DECODE_LEFT_CHROMA_BLOCK_CABAC(0, 3, 6, 4)
                        DECODE_INTERNAL_CHROMA_BLOCK_CABAC(1, 3, 7, 6, 5)
                        break;

                    default:
                        break;
                    }
                }
            }

            // set the empty CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = uFinalCBP[0];
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = uFinalCBP[1];
        }
        else
        {
            // set the empty CBP
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
        }

        // update buffer position pointer
        sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon) pPosCoefbuf;
    } // void DecodeCoefficients4x4_CABAC(H264SegmentDecoderMultiThreaded * sd)
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s color_format, Ipp32s is_field>
class ResidualDecoderPCM
{
public:
    typedef PlaneY * PlanePtrY;
    typedef PlaneUV * PlanePtrUV;
    typedef Coeffs *  CoeffsPtr;

    virtual ~ResidualDecoderPCM(){}

    ///////////////////////////////////////////////////////////////////////////////
    // decodePCMCoefficients
    //
    // Extracts raw coefficients from bitstream by:
    //  a) byte aligning bitstream pointer
    //  b) copying bitstream pointer to m_pCoeffBlocksWrite
    //  c) advancing bitstream pointer by 256+128 bytes
    //
    //    Also initializes NumCoef buffers for correct use in future MBs.
    //
    /////////////////////////////
    // Decode the coefficients for a PCM macroblock, placing them
    // in m_pCoeffBlocksBuf.
    void DecodeCoefficients_PCM(H264SegmentDecoderMultiThreaded * sd)
    {
        static const Ipp32u num_coeffs[4] = {256,384,512,768};

        Ipp32u length = num_coeffs[color_format];
        // number of raw coeff bits
        // to write pointer to non-aligned m_pCoeffBlocksWrite
        sd->m_cur_mb.LocalMacroblockInfo->QP = 0;

        PlanePtrY pCoeffBlocksWrite_Y = reinterpret_cast<PlanePtrY> (sd->m_pCoeffBlocksWrite);

        if (sd->m_pPicParamSet->entropy_coding_mode)
        {
            sd->m_pBitStream->TerminateDecode_CABAC();
        }
        else
        {
            sd->m_pBitStream->AlignPointerRight();
        }

        Ipp32u i;
        for (i = 0; i < 256; i++)
        {
            pCoeffBlocksWrite_Y[i] = (PlaneY) sd->m_pBitStream->GetBits(sd->bit_depth_luma);
        }

        sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon)((Ipp8u*)sd->m_pCoeffBlocksWrite +
                        256*sizeof(PlaneY));

        if (color_format)
        {
            PlanePtrUV pCoeffBlocksWrite_UV = (PlanePtrUV) (sd->m_pCoeffBlocksWrite);
            for (i = 0; i < length - 256; i++)
            {
                pCoeffBlocksWrite_UV[i] = (PlaneUV) sd->m_pBitStream->GetBits(sd->bit_depth_chroma);
            }

            ippsSet_8u(16, sd->m_cur_mb.GetNumCoeffs()->numCoeffs, sizeof(H264DecoderMacroblockCoeffsInfo));//set correct numcoeffs

            if(sd->m_pPicParamSet->entropy_coding_mode)
            {
                sd->m_pBitStream->InitializeDecodingEngine_CABAC();
            }

            sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon)((Ipp8u*)sd->m_pCoeffBlocksWrite +
                            (length - 256)*sizeof(PlaneUV));
        }
    } // void DecodeCoefficients_PCM(Ipp8u color_format)
};

} // namespace UMC

#endif // __UMC_H264_RESIDUAL_DECODER_TEMPLATES_H
