/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Using Intel(R) IPP 
//     in Microsoft* C# .NET for Windows* Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/

using System;
using System.Security;
using System.Runtime.InteropServices;
using ipp;

namespace ExampleIP
{
   public enum JMODE
   {
      JPEG_BASELINE    = 0,
      JPEG_PROGRESSIVE = 1,
      JPEG_LOSSLESS    = 2
   };
   public enum JOPERATION
   {
      JO_READ_HEADER = 0,
      JO_READ_DATA   = 1
   };
   public enum JCOLOR
   {
      JC_UNKNOWN = 0,
      JC_GRAY    = 1,
      JC_RGB     = 2,
      JC_BGR     = 3,
      JC_YCBCR   = 4,
      JC_CMYK    = 5,
      JC_YCCK    = 6
   };
   public enum JSS
   {
      JS_444   = 0,
      JS_422   = 1,
      JS_411   = 2,
      JS_OTHER = 3
   };
   public enum JMARKER
   {
      JM_NONE  = 0,
      JM_SOI   = 0xd8, /* start of image */
      JM_EOI   = 0xd9, /* end of image */

      /* start of frame */
      JM_SOF0  = 0xc0, /* Nondifferential Huffman-coding Baseline DCT */
      JM_SOF1  = 0xc1, /* Nondifferential Huffman-coding Extended sequental DCT */
      JM_SOF2  = 0xc2, /* Nondifferential Huffman-coding Progressive DCT */
      JM_SOF3  = 0xc3, /* Nondifferential Huffman-coding Lossless (sequental) */

      JM_SOF5  = 0xc5, /* Differential Huffman-coding Sequental DCT */
      JM_SOF6  = 0xc6, /* Differential Huffman-coding Progressive DCT */
      JM_SOF7  = 0xc7, /* Differential Lossless */

      JM_SOF9  = 0xc9, /* Nondifferential arithmetic-coding Extended sequental DCT */
      JM_SOFA  = 0xca, /* Nondifferential arithmetic-coding Progressive DCT */
      JM_SOFB  = 0xcb, /* Nondifferential arithmetic-coding Lossless (sequental) */

      JM_SOFD  = 0xcd, /* Differential arithmetic-coding Sequental DCT */
      JM_SOFE  = 0xce, /* Differential arithmetic-coding Progressive DCT */
      JM_SOFF  = 0xcf, /* Differential arithmetic-coding Lossless */

      JM_SOS   = 0xda, /* start of scan */
      JM_DQT   = 0xdb, /* define quantization table(s) */
      JM_DHT   = 0xc4, /* define Huffman table(s) */
      JM_APP0  = 0xe0, /* APP0 */
      JM_APP1  = 0xe1,
      JM_APP2  = 0xe2,
      JM_APP3  = 0xe3,
      JM_APP4  = 0xe4,
      JM_APP5  = 0xe5,
      JM_APP6  = 0xe6,
      JM_APP7  = 0xe7,
      JM_APP8  = 0xe8,
      JM_APP9  = 0xe9,
      JM_APP10 = 0xea,
      JM_APP11 = 0xeb,
      JM_APP12 = 0xec,
      JM_APP13 = 0xed,
      JM_APP14 = 0xee, /* APP14 */
      JM_APP15 = 0xef,
      JM_RST0  = 0xd0, /* restart with modulo 8 counter 0 */
      JM_RST1  = 0xd1, /* restart with modulo 8 counter 1 */
      JM_RST2  = 0xd2, /* restart with modulo 8 counter 2 */
      JM_RST3  = 0xd3, /* restart with modulo 8 counter 3 */
      JM_RST4  = 0xd4, /* restart with modulo 8 counter 4 */
      JM_RST5  = 0xd5, /* restart with modulo 8 counter 5 */
      JM_RST6  = 0xd6, /* restart with modulo 8 counter 6 */
      JM_RST7  = 0xd7, /* restart with modulo 8 counter 7 */
      JM_DRI   = 0xdd, /* define restart interval */
      JM_COM   = 0xfe  /* comment */
   };

   public enum JERRCODE
   {
      JPEG_OK                 =   0,
      JPEG_NOT_IMPLEMENTED    =  -1,
      JPEG_INTERNAL_ERROR     =  -2,
      JPEG_BUFF_TOO_SMALL     =  -3,
      JPEG_OUT_OF_MEMORY      =  -4,
      JPEG_BAD_SEGMENT_LENGTH =  -5,
      JPEG_BAD_HUFF_TBL       =  -6,
      JPEG_BAD_QUANT_SEGMENT  =  -7,
      JPEG_BAD_SCAN_SEGMENT   =  -8,
      JPEG_BAD_FRAME_SEGMENT  =  -9,
      JPEG_BAD_COMPONENT_ID   = -10,
      JPEG_BAD_SAMPLING       = -11,
      JPEG_BAD_RESTART        = -12
   };

   unsafe public struct BITSTREAM
   {
      public byte[] pData;
      public int DataLen;
      public int currPos;
      public JERRCODE _WRITE_BYTE(int val)
      {
         if( currPos == DataLen )
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         pData[currPos] = (byte)val;
            currPos++;
         return JERRCODE.JPEG_OK;
      }
      public JERRCODE _WRITE_WORD(int val)
      {
         JERRCODE jerr = _WRITE_BYTE(val >> 8);;
         if( JERRCODE.JPEG_OK != jerr )
            return jerr;
         jerr = _WRITE_BYTE(val);
         if( JERRCODE.JPEG_OK != jerr )
            return jerr;
         return JERRCODE.JPEG_OK;
      }
      public JERRCODE _READ_BYTE(ref int val)
      {
         if( currPos == DataLen )
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         val = pData[currPos];
         currPos++;
         return JERRCODE.JPEG_OK;
      }
      public JERRCODE _READ_WORD(ref int val)
      {
         int hb = 0;
         int lb = 0;
         JERRCODE jerr;
         jerr = _READ_BYTE(ref hb);
         if( JERRCODE.JPEG_OK != jerr )
            return jerr;
         jerr = _READ_BYTE(ref lb);
         if( JERRCODE.JPEG_OK != jerr )
            return jerr;
         val = (hb << 8) + lb;
         return JERRCODE.JPEG_OK;
      }
   }

   unsafe public struct IMAGE
   {
      public byte*  Data8u;
      public int    width;
      public int    height;
      public int    lineStep;
      public int    precision;
      public int    nChannels;
      public JCOLOR color;
   };

   unsafe public struct CJPEGColorComponent
   {
      public int m_id;
      public int m_comp_no;
      public int m_hsampling;
      public int m_vsampling;
      public int m_h_factor;
      public int m_v_factor;
      public int m_nblocks;
      public int m_q_selector;
      public int m_dc_selector;
      public int m_ac_selector;
      public int m_ac_scan_completed;
      public short m_lastDC;
      public byte[] m_cc_buffer;
      public byte[] m_ss_buffer;
      public byte[] m_top_row;
      public byte[] m_bottom_row;
      public byte[] m_curr_row;
      public byte[] m_prev_row;
      public int m_curr_ofs;
      public int m_prev_ofs;
   };

   unsafe public class custom_ipp
   {
      internal const string s_libname = "ipps-7.1.dll";
      internal const string j_libname = "ippj-7.1.dll";

      /// We want to use automatic pinning to reduce the number of the fixed operators.
      /// The use of [in, out] will incur an extra copy of the data as it moves from managed code to the native dll’s
      /// but it will be automatic.
      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(s_libname)] public static extern
      IppStatus ippsCopy_8u([In] byte[] pSrc, [Out] byte[] pDst, int len);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(s_libname)] public static extern
      IppStatus ippsZero_8u([In, Out] byte[] pDst, int len);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiQuantInvTableInit_JPEG_8u16u([In] byte[] pQuantRawTable, [Out] ushort[] pQuantInvTable);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffmanSpecInit_JPEG_8u([In] byte[] pListBits, [In] byte[] pListVals, [In, Out] byte[] pDecHuffSpec);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffmanStateInit_JPEG_8u([In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffman8x8_JPEG_1u16s_C1([In] byte[] pSrc, int srcLenBytes, int* pSrcCurrPos, short* pDst, short* pLastDC,
         int* pMarker, [In, Out] byte[] pDcTable, [In, Out] byte[] pAcTable, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffmanOne_JPEG_1u16s_C1([In] byte[] pSrc, int nSrcLenBytes, int* pSrcCurrPos, short* pDst, int* pMarker,
         [In, Out] byte[] pDecHuffTable, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1([In] byte[] pSrc, int srcLenBytes, int* pSrcCurrPos,
         short* pDst, int* pMarker, int Ss, int Se, int Al, [In, Out] byte[] pAcTable, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1([In] byte[] pSrc, int srcLenBytes, int* pSrcCurrPos,
         short* pDst, int* pMarker, int Ss, int Se, int Al, [In, Out] byte[] pAcTable, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1([In] byte[] pSrc, int srcLenBytes, int* pSrcCurrPos,
         short* pDst, short* pLastDC, int* pMarker, int Al, [In, Out] byte[] pDcTable, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1([In] byte[] pSrc, int srcLenBytes, int* pSrcCurrPos,
         short* pDst, int* pMarker, int Al, [In, Out] byte[] pDecHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiQuantFwdRawTableInit_JPEG_8u([In, Out] byte[] pQuantRawTable, int qualityFactor);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiQuantFwdTableInit_JPEG_8u16u([In] byte[] pQuantRawTable, [Out] ushort[] pQuantFwdTable);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiEncodeHuffmanSpecInit_JPEG_8u([In] byte[] pListBits, [In] byte[] pListVals, [In, Out] byte[] pEncHuffSpec);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiEncodeHuffmanStateInit_JPEG_8u([In, Out] byte[] pEncHuffState);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiSampleDownH2V1_JPEG_8u_C1R([In] byte[] pSrc, int srcStep, IppiSize srcRoiSize, [Out] byte[] pDst, int dstStep, IppiSize dstRoiSize);

      [SuppressUnmanagedCodeSecurityAttribute()]
      [DllImport(j_libname)] public static extern
      IppStatus ippiSampleDownH2V2_JPEG_8u_C1R([In] byte[] pSrc, int srcStep, IppiSize srcRoiSize, [Out] byte[] pDst, int dstStep, IppiSize dstRoiSize);

   }

}
