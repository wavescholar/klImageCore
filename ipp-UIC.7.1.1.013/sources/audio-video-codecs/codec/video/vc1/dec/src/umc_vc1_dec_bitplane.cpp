/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, BitPlane decoding
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include <string.h>

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"

//4.10    Bitplane Coding
//Certain macroblock-specific information can be encoded in one bit per
//macroblock. For example, whether or not any information is present for
// a macroblock (i.e., whether or not it is skipped) can be signaled with
// one bit. In these cases, the status for all macroblocks in a frame can
// be coded as a bitplane and transmitted in the frame header. VC1 uses
// bitplane coding in three cases to signal information about the macroblocks
// in a frame. These are: 1) signaling skipped macroblocks, 2) signaling field
// or frame macroblock mode and 3) signaling 1-MV or 4-MV motion vector mode
// for each macroblock. This section describes the bitplane coding scheme.
//Frame-level bitplane coding is used to encode two-dimensional binary arrays.
// The size of each array is rowMB ? colMB, where rowMB and colMB are the
//number of macroblock rows and columns respectively.  Within the bitstream,
// each array is coded as a set of consecutive bits.  One of seven modes is
// used to encode each array.
//The seven modes are enumerated below.
//1.    Raw mode - coded as one bit per symbol
//2.    Normal-2 mode - two symbols coded jointly
//3.    Differential-2 mode - differential coding of bitplane, followed by
//        coding two residual symbols jointly
//4.    Normal-6 mode - six symbols coded jointly
//5.    Differential-6 mode - differential coding of bitplane, followed by
//        coding six residual symbols jointly
//6.    Rowskip mode - one bit skip to signal rows with no set bits
//7.    Columnskip mode - one bit skip to signal columns with no set bits
//Section 3.3 shows the syntax elements that make up the bitplane coding scheme.
//The follow sections describe how to decode the bitstream and reconstruct the
//bitplane.



static void InverseDiff(VC1Bitplane* pBitplane, Ipp32s widthMB, Ipp32s heightMB)
{
    Ipp32s i, j;

    for(i = 0; i < heightMB; i++)
    {
        for(j = 0; j < widthMB; j++)
        {
            if((i == 0 && j == 0))
            {
                pBitplane->m_databits[i*widthMB + j] = pBitplane->m_databits[i*widthMB + j] ^ pBitplane->m_invert;
            }
            else if(j == 0)
            {
                pBitplane->m_databits[i*widthMB + j] = pBitplane->m_databits[i*widthMB + j] ^pBitplane->m_databits[widthMB*(i-1)];
            }
            else if(((i>0) && (pBitplane->m_databits[i*widthMB+j-1] != pBitplane->m_databits[(i-1)*widthMB+j])))
            {
                pBitplane->m_databits[i*widthMB + j] = pBitplane->m_databits[i*widthMB + j] ^ pBitplane->m_invert;
            }
            else
            {
                pBitplane->m_databits[i*widthMB + j] = pBitplane->m_databits[i*widthMB + j] ^ pBitplane->m_databits[i*widthMB + j - 1];
            }
        }
    }

}


static void InverseBitplane(VC1Bitplane* pBitplane, Ipp32s size)
{
    Ipp32s i;

    for(i = 0; i < size; i++)
    {
        pBitplane->m_databits[i] = pBitplane->m_databits[i] ^ 1;
    }
}


static void Norm2ModeDecode(VC1Context* pContext,VC1Bitplane* pBitplane, Ipp32s width, Ipp32s height)
{
    Ipp32s i;
    Ipp32s tmp_databits = 0;

    if((width*height) & 1)
    {
        VC1_GET_BITS(1, tmp_databits);
        pBitplane->m_databits[0] = (Ipp8u)tmp_databits;
    }

    for(i = (width*height) & 1; i < (width*height/2)*2; i+=2)
    {
        Ipp32s tmp;
        VC1_GET_BITS(1, tmp);
        if(tmp == 0)
        {
            pBitplane->m_databits[i]   = 0;
            pBitplane->m_databits[i+1] = 0;
        }
        else
        {
            VC1_GET_BITS(1, tmp);
            if(tmp == 1)
            {
                pBitplane->m_databits[i]   = 1;
                pBitplane->m_databits[i+1] = 1;
            }
            else
            {
                VC1_GET_BITS(1, tmp);
                if(tmp == 0)
                {
                    pBitplane->m_databits[i]   = 1;
                    pBitplane->m_databits[i+1] = 0;
                }
                else
                {
                    pBitplane->m_databits[i]   = 0;
                    pBitplane->m_databits[i+1] = 1;
                }
            }
        }
    }
}


static void Norm6ModeDecode(VC1Context* pContext, VC1Bitplane* pBitplane, Ipp32s width, Ipp32s height)
{
    IppStatus ret;
    Ipp32s i, j;
    Ipp32s k;
    Ipp32s ResidualX = 0;
    Ipp32s ResidualY = 0;
    Ipp8u _2x3tiled = (((width%3)!=0)&&((height%3)==0));


    if(_2x3tiled)
    {
        Ipp32s sizeW = width/2;
        Ipp32s sizeH = height/3;
        Ipp8u *currRowTails =  pBitplane->m_databits;

        for(i = 0; i < sizeH; i++)
        {
            //set pointer to start of tail row
            currRowTails =  &pBitplane->m_databits[i*3*width];

            //move tails start if number of MB in row is odd
            //this column bits will be decoded after
            currRowTails += width&1;

            for(j = 0; j < sizeW; j++)
            {
                ret = ippiDecodeHuffmanOne_1u32s (
                            &pContext->m_bitstream.pBitstream,
                            &pContext->m_bitstream.bitOffset,
                            &k,
                            pContext->m_vlcTbl->m_BitplaneTaledbits
                            );
                VM_ASSERT(ret == ippStsNoErr);

                currRowTails[0] = (Ipp8u)(k&1);
                currRowTails[1] = (Ipp8u)((k&2)>>1);

                currRowTails[width + 0] = (Ipp8u)((k&4)>>2);
                currRowTails[width + 1] = (Ipp8u)((k&8)>>3);

                currRowTails[2*width + 0] = (Ipp8u)((k&16)>>4);
                currRowTails[2*width + 1] = (Ipp8u)((k&32)>>5);

                currRowTails+=2;

             }
        }
        ResidualX = width & 1;
        ResidualY = 0;
    }
    else //3x2 tiled
    {
        Ipp32s sizeW = width/3;
        Ipp32s sizeH = height/2;
        Ipp8u *currRowTails =  pBitplane->m_databits;

        for(i = 0; i < sizeH; i++)
        {
            //set pointer to start of tail row
            currRowTails =  &pBitplane->m_databits[i*2*width];

            //move tails start if number of MB in row is odd
            //this column bits will be decoded after
            currRowTails += width%3;
            currRowTails += (height&1)*width;

            for(j = 0; j < sizeW; j++)
            {
                ret = ippiDecodeHuffmanOne_1u32s (
                            &pContext->m_bitstream.pBitstream,
                            &pContext->m_bitstream.bitOffset,
                            &k,
                            pContext->m_vlcTbl->m_BitplaneTaledbits
                            );
                VM_ASSERT(ret == ippStsNoErr);

                currRowTails[0] = (Ipp8u)(k&1);
                currRowTails[1] = (Ipp8u)((k&2)>>1);
                currRowTails[2] = ((Ipp8u)(k&4)>>2);

                currRowTails[width + 0] = (Ipp8u)((k&8)>>3);
                currRowTails[width + 1] = (Ipp8u)((k&16)>>4);
                currRowTails[width + 2] = (Ipp8u)((k&32)>>5);

                currRowTails+=3;

             }
        }
        ResidualX = width % 3;
        ResidualY = height & 1;
    }

    //ResidualY 0 or 1 or 2
    for(i = 0; i < ResidualX; i++)
    {
        Ipp32s ColSkip;
        VC1_GET_BITS(1, ColSkip);

        if(1 == ColSkip)
        {
            for(j = 0; j < height; j++)
            {
                Ipp32s Value = 0;
                VC1_GET_BITS(1, Value);
                pBitplane->m_databits[i + width * j] = (Ipp8u)Value;
            }
        }
        else
        {
            for(j = 0; j < height; j++)
            {
                pBitplane->m_databits[i + width * j] = 0;
            }
        }
    }

    //ResidualY 0 or 1
    for(j = 0; j < ResidualY; j++)
    {
        Ipp32s RowSkip;

        VC1_GET_BITS(1, RowSkip);

        if(1 == RowSkip)
        {
            for(i = ResidualX; i < width; i++)
            {
                Ipp32s Value = 0;
                VC1_GET_BITS(1, Value);
                pBitplane->m_databits[i] = (Ipp8u)Value;
            }
        }
        else
        {
            for(i = ResidualX; i < width; i++)
            {
                pBitplane->m_databits[i] = 0;
            }
        }
    }

}

void DecodeBitplane(VC1Context* pContext, VC1Bitplane* pBitplane, Ipp32s width, Ipp32s height,Ipp32s offset)
{
    Ipp32s tmp;
    Ipp32s i, j;
    IppStatus ret;
    Ipp32s tmp_invert = 0;
    Ipp32s tmp_databits = 0;

    memset(pBitplane, 0, sizeof(VC1Bitplane));
    if (offset == 0)
        ++pContext->bp_round_count;
    if (VC1_MAX_BITPANE_CHUNCKS == pContext->bp_round_count)
        pContext->bp_round_count = 0;

    Ipp32u HeightMB = pContext->m_seqLayerHeader->heightMB;
    // Special case for Fields
    if (pContext->m_seqLayerHeader->IsResize)
    {
        HeightMB -= 1;
        height -= 1;
    }


    if (pContext->bp_round_count >= 0)
        pBitplane->m_databits = pContext->m_pBitplane.m_databits +
        HeightMB*
        pContext->m_seqLayerHeader->widthMB*pContext->bp_round_count + offset;
    else
        pBitplane->m_databits = pContext->m_pBitplane.m_databits -
        HeightMB * pContext->m_seqLayerHeader->widthMB + offset;

    //4.10.1
    //The INVERT field shown in the syntax diagram of Figure 30 is a one bit
    //code, which if set indicates that the bitplane has more set bits than
    //zero bits. Depending on INVERT and the mode, the decoder must invert
    //the interpreted bitplane to recreate the original.
    VC1_GET_BITS(1, tmp_invert);
    pBitplane->m_invert = (Ipp8u)tmp_invert;

    //VC-1 Table 68: IMODE Codetable
    //CODING MODE    CODEWORD
    //Raw            0000
    //Norm-2        10
    //Diff-2        001
    //Norm-6        11
    //Diff-6        0001
    //Rowskip        010
    //Colskip        011
    ret = ippiDecodeHuffmanOne_1u32s (
                &pContext->m_bitstream.pBitstream,
                &pContext->m_bitstream.bitOffset,
                &pBitplane->m_imode,
                pContext->m_vlcTbl->m_Bitplane_IMODE
                );
    VM_ASSERT(ret == ippStsNoErr);

    //The DATABITS field shown in the syntax diagram of Figure 28 is an entropy
    //coded stream of symbols that is based on the coding mode. The seven coding
    //modes are described in the following sections.
    switch(pBitplane->m_imode)
    {
    case VC1_BITPLANE_RAW_MODE:
        //nothing to do

        break;
    case VC1_BITPLANE_NORM2_MODE:
        //4.10.3.3    Normal-2 mode
        //If rowMB x colMB is odd, the first symbol is encoded raw.  Subsequent
        //symbols are encoded pairwise, in natural scan order.  The binary VLC
        //table in Table 57 is used to encode symbol pairs.
        //Table 57: Norm-2/Diff-2 Code Table
        //SYMBOL 2N    SYMBOL 2N + 1    CODEWORD
        //    0            0                0
        //    1            0                100
        //    0            1                101
        //    1            1                11
        Norm2ModeDecode(pContext, pBitplane, width, height);

        if(pBitplane->m_invert)
        {
            InverseBitplane(pBitplane, width*height);
        }

        break;
    case VC1_BITPLANE_DIFF2_MODE:

        //decode differentional bits
        Norm2ModeDecode(pContext, pBitplane, width, height);
        //restore original
        InverseDiff(pBitplane, width, height);

        break;
    case VC1_BITPLANE_NORM6_MODE:
        //In the Norm-6 and Diff-6 modes, the bitplane is encoded in groups of
        //six pixels.  These pixels are grouped into either 2x3 or 3x2 tiles.
        //The bitplane is tiled maximally using a set of rules, and the remaining
        //pixels are encoded using a variant of row-skip and column-skip modes.
        //3x2 "vertical" tiles are used if and only if rowMB is a multiple of 3
        //and colMB is not.  Else, 2x3 "horizontal" tiles are used
        Norm6ModeDecode(pContext, pBitplane, width, height);

        if(pBitplane->m_invert)
        {
            InverseBitplane(pBitplane, width*height);
        }
        break;
    case VC1_BITPLANE_DIFF6_MODE:

        //decode differentional bits
        Norm6ModeDecode(pContext, pBitplane, width, height);
        //restore original
        InverseDiff(pBitplane, width, height);

        break;
    case VC1_BITPLANE_ROWSKIP_MODE:
        //In the row-skip coding mode, all-zero rows are skipped with one bit overhead.
        //The syntax is as shown in Figure 79.
        //If the entire row is zero, a zero bit is sent as the ROWSKIP symbol,
        //and ROWBITS is skipped.  If there is a set bit in the row, ROWSKIP
        //is set to 1, and the entire row is sent raw (ROWBITS).  Rows are
        //scanned from the top to the bottom of the frame.
        for(i = 0; i < height; i++)
        {
            VC1_GET_BITS(1, tmp);
            if(tmp == 0)
            {
                for(j = 0; j < width; j++)
                    pBitplane->m_databits[width*i + j] = 0;
            }
            else
            {
                for(j = 0; j < width; j++)
                {
                    VC1_GET_BITS(1, tmp_databits);
                    pBitplane->m_databits[width*i + j] = (Ipp8u)tmp_databits;
                }
            }
        }
        if(pBitplane->m_invert)
        {
            InverseBitplane(pBitplane, width*height);
        }

        break;
    case VC1_BITPLANE_COLSKIP_MODE:
        //Column-skip is the transpose of row-skip.  Columns are scanned from the
        //left to the right of the frame.
        for(i = 0; i < width; i++)
        {
            VC1_GET_BITS(1, tmp);
            if(tmp == 0)
            {
                for(j = 0; j < height; j++)
                    pBitplane->m_databits[i + j*width] = 0;
            }
            else
            {
                for(j = 0; j < height; j++)
                {
                    VC1_GET_BITS(1, tmp_databits);
                    pBitplane->m_databits[i + j*width] = (Ipp8u)tmp_databits;
                }
            }
        }
        if(pBitplane->m_invert)
        {
            InverseBitplane(pBitplane, width*height);
        }
        break;

    }
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
