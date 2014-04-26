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
using ipp;

namespace ExampleIP
{
   public class JPEG_SCAN
   {
      public int ncomp;
      public int[] id = new int[4];
      public int Ss;
      public int Se;
      public int Ah;
      public int Al;
   };

   unsafe class CJPEGEncoderQuantTable
   {
      const int CPU_CACHE_LINE      = 32;
      const int DCTSIZE2            = 64;

      public byte[]   m_rbf;
      public ushort[] m_qbf;

      public int m_id;
      public int m_precision;
      
      public CJPEGEncoderQuantTable()
      {
         m_id          = 0;
         m_precision   = 0;
         m_rbf = new byte[DCTSIZE2+(CPU_CACHE_LINE-1)];
         m_qbf = new ushort[DCTSIZE2+(CPU_CACHE_LINE-1)];
      }

      public JERRCODE Init(int id, int quality, byte[] raw)
      {
         IppStatus status;
         m_id        = id  & 0x0f;
         m_precision = (id & 0xf0) >> 4;
         custom_ipp.ippsCopy_8u(raw, m_rbf, DCTSIZE2);
         status = custom_ipp.ippiQuantFwdRawTableInit_JPEG_8u(m_rbf, quality);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         status = custom_ipp.ippiQuantFwdTableInit_JPEG_8u16u(m_rbf, m_qbf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         return JERRCODE.JPEG_OK;
      }
   };

   unsafe class CJPEGEncoderHuffmanTable
   {
      public byte[] m_buf;
      public int    m_id;
      public int    m_hclass;
      public byte[] m_bits;
      public byte[] m_vals;

      public CJPEGEncoderHuffmanTable()
      {
         m_bits = new byte[16];
         m_vals = new byte[256];
         m_id     = 0;
         m_hclass = 0;
      }
      ~CJPEGEncoderHuffmanTable()
      {
         Destroy();
      }
      public JERRCODE Create()
      {
         int       size;
         IppStatus status;

         status = ipp.jp.ippiEncodeHuffmanSpecGetBufSize_JPEG_8u(&size);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_buf = new byte[size];
         return JERRCODE.JPEG_OK;
      }
      JERRCODE Destroy()
      {
         m_id     = 0;
         m_hclass = 0;
         return JERRCODE.JPEG_OK;
      }
      public JERRCODE Init(int id, int hclass, byte[] bits, byte[] vals) 
      {
         IppStatus status;
         m_id     = id     & 0x0f;
         m_hclass = hclass & 0x0f;
         m_bits = bits;
         m_vals = vals;
         status = custom_ipp.ippiEncodeHuffmanSpecInit_JPEG_8u(m_bits, m_vals, m_buf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         return JERRCODE.JPEG_OK;
      }
   };

   unsafe class CJPEGEncoderHuffmanState
   {
      public byte[] m_buf;

      public CJPEGEncoderHuffmanState()
      {
      }
      ~CJPEGEncoderHuffmanState()
      {
         Destroy();
      }
      public JERRCODE Create()
      {
         int       size;
         IppStatus status;

         status = ipp.jp.ippiEncodeHuffmanStateGetBufSize_JPEG_8u(&size);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_buf = new byte[size];
         return JERRCODE.JPEG_OK;
      }
      JERRCODE Destroy()
      {
         return JERRCODE.JPEG_OK;
      }
      public JERRCODE Init()
      {
         IppStatus status;
         status = custom_ipp.ippiEncodeHuffmanStateInit_JPEG_8u(m_buf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         return JERRCODE.JPEG_OK;
      }
   };

   unsafe public class JPEGEncoder
   {
      // raw quant tables must be in zigzag order
      byte[] DefaultLuminanceQuant =
         {
            16,  11,  12,  14,  12,  10,  16,  14,
            13,  14,  18,  17,  16,  19,  24,  40,
            26,  24,  22,  22,  24,  49,  35,  37,
            29,  40,  58,  51,  61,  60,  57,  51,
            56,  55,  64,  72,  92,  78,  64,  68,
            87,  69,  55,  56,  80, 109,  81,  87,
            95,  98, 103, 104, 103,  62,  77, 113,
            121, 112, 100, 120,  92, 101, 103,  99
         };
      // raw quant tables must be in zigzag order
      byte[] DefaultChrominanceQuant =
         {
            17,  18,  18,  24,  21,  24,  47,  26,
            26,  47,  99,  66,  56,  66,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99,
            99,  99,  99,  99,  99,  99,  99,  99
         };
      byte[] DefaultLuminanceDCBits =
         {
            0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
         };
      byte[] DefaultLuminanceDCValues =
         {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b
         };
      byte[] DefaultChrominanceDCBits =
         {
            0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
         };
      byte[] DefaultChrominanceDCValues =
         {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b
         };
      byte[] DefaultLuminanceACBits =
         {
            0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
            0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d
         };
      byte[] DefaultLuminanceACValues =
         {
            0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
            0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
            0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
            0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
            0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
            0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
            0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
            0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
            0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
            0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
            0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
            0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
            0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
            0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
            0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
            0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
            0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
            0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
            0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
            0xf9, 0xfa
         };
      byte[] DefaultChrominanceACBits =
         {
            0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04,
            0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77
         };
      byte[] DefaultChrominanceACValues =
         {
            0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
            0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
            0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
            0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
            0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
            0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
            0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
            0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
            0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
            0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
            0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
            0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
            0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
            0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
            0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
            0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
            0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
            0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
            0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
            0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
            0xf9, 0xfa
         };

      const int DCTSIZE2            = 64;
      const int MAX_QUANT_TABLES    = 4;
      const int MAX_HUFF_TABLES     = 4;
      const int MAX_COMPS_PER_SCAN  = 4;
      const int CPU_CACHE_LINE      = 32;
      const int MAX_BLOCKS_PER_MCU  = 10;
      int MAX_BYTES_PER_MCU   = DCTSIZE2*sizeof(short)*MAX_BLOCKS_PER_MCU;

      IMAGE      m_src;
      BITSTREAM  m_dst;
      int        m_jpeg_ncomp;
      int        m_jpeg_precision = 8;
      JSS        m_jpeg_sampling = JSS.JS_444;
      JCOLOR     m_jpeg_color = JCOLOR.JC_UNKNOWN;
      int        m_jpeg_quality = 100;
      int        m_jpeg_restart_interval;
      JMODE      m_jpeg_mode = JMODE.JPEG_BASELINE;
      int        m_numxMCU;
      int        m_numyMCU;
      int        m_mcuWidth;
      int        m_mcuHeight;
      int        m_ccWidth;
      int        m_ccHeight;
      int        m_xPadding;
      int        m_yPadding;
      int        m_restarts_to_go;
      int        m_next_restart_num;
      int        m_scan_count;
      int        m_ss;
      int        m_se;
      int        m_al;
      int        m_ah;
      int        m_predictor = 1;
      int        m_pt = 0;
      short[]    m_coefbuf;
      CJPEGColorComponent[]      m_ccomp = new CJPEGColorComponent[MAX_COMPS_PER_SCAN];
      CJPEGEncoderQuantTable[]   m_qntbl = new CJPEGEncoderQuantTable[MAX_QUANT_TABLES];
      CJPEGEncoderHuffmanTable[] m_dctbl = new CJPEGEncoderHuffmanTable[MAX_HUFF_TABLES];
      CJPEGEncoderHuffmanTable[] m_actbl = new CJPEGEncoderHuffmanTable[MAX_HUFF_TABLES];
      CJPEGEncoderHuffmanState   m_state = new CJPEGEncoderHuffmanState();
      JPEG_SCAN[] m_scan_script;

      public JPEGEncoder()
      {
         int i;
         for( i=0; i<MAX_QUANT_TABLES; i++ )
         {
            m_qntbl[i] = new CJPEGEncoderQuantTable();
         }
         for( i=0; i<MAX_HUFF_TABLES; i++ )
         {
            m_dctbl[i] = new CJPEGEncoderHuffmanTable();
            m_actbl[i] = new CJPEGEncoderHuffmanTable();
         }
         m_se = 63;
      }

      public JERRCODE SetSource(byte* pSrc, int srcStep, IppiSize srcSize,
         int srcChannels, JCOLOR   srcColor)
      {
         m_src.Data8u    = pSrc;
         m_src.lineStep  = srcStep;
         m_src.width     = srcSize.width;
         m_src.height    = srcSize.height;
         m_src.nChannels = srcChannels;
         m_src.color     = srcColor;
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE SetDestination(ref byte[] pDst, int dstSize, 
         int dstQuality, JSS dstSampling,
         JCOLOR dstColor, JMODE dstMode, int dstRestartInt)
      {
         m_dst.pData   = pDst;
         m_dst.DataLen = dstSize;
         m_dst.currPos = 0;

         m_jpeg_quality          = dstQuality;
         m_jpeg_sampling         = dstSampling;
         m_jpeg_color            = dstColor;
         m_jpeg_mode             = dstMode;
         m_jpeg_restart_interval = dstRestartInt;
         m_restarts_to_go = m_jpeg_restart_interval;
         if( JMODE.JPEG_LOSSLESS == m_jpeg_mode )
         {
            m_mcuWidth  = 1;
            m_mcuHeight = 1;
         }
         else
         {
            m_mcuWidth  = (m_jpeg_sampling == JSS.JS_444) ?  8 : 16;
            m_mcuHeight = (m_jpeg_sampling == JSS.JS_411) ? 16 :  8;
         }
         m_numxMCU = (m_src.width  + (m_mcuWidth  - 1)) / m_mcuWidth;
         m_numyMCU = (m_src.height + (m_mcuHeight - 1)) / m_mcuHeight;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOI()
      {
         if( m_dst.currPos + 2 >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOI);
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteEOI()
      {
         if( m_dst.currPos + 2 >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_EOI);
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteAPP0()
      {
         int len;
         len = 2 + 5 + 2 + 1 + 2 + 2 + 1 + 1;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_APP0);
         m_dst._WRITE_WORD(len);
         // identificator JFIF
         m_dst._WRITE_BYTE('J');
         m_dst._WRITE_BYTE('F');
         m_dst._WRITE_BYTE('I');
         m_dst._WRITE_BYTE('F');
         m_dst._WRITE_BYTE(0);
         // version
         m_dst._WRITE_WORD(0x0102);
         // units: 0 - none, 1 - dot per inch, 2 - dot per cm
         m_dst._WRITE_BYTE(0);
         // xDensity
         m_dst._WRITE_WORD(1);
         // yDensity
         m_dst._WRITE_WORD(1);
         // xThumbnails, yThumbnails
         m_dst._WRITE_BYTE(0);
         m_dst._WRITE_BYTE(0);
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteAPP14()
      {
         int len;
         len = 2 + 5 + 2 + 2 + 2 + 1;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_APP14);
         m_dst._WRITE_WORD(len);
         // identificator Adobe
         m_dst._WRITE_BYTE('A');
         m_dst._WRITE_BYTE('d');
         m_dst._WRITE_BYTE('o');
         m_dst._WRITE_BYTE('b');
         m_dst._WRITE_BYTE('e');
         // version
         m_dst._WRITE_WORD(100);
         // Flags 0
         m_dst._WRITE_WORD(0);
         // Flags 1
         m_dst._WRITE_WORD(0);
         switch( m_jpeg_color )
         {
            case JCOLOR.JC_YCBCR:
               m_dst._WRITE_BYTE(1);
               break;
            case JCOLOR.JC_YCCK:
               m_dst._WRITE_BYTE(2);
               break;
            default:
               m_dst._WRITE_BYTE(0);
               break;
         }
         return JERRCODE.JPEG_OK;
      }

      //JERRCODE WriteCOM(char* comment)
      JERRCODE WriteCOM()
      {
         IppLibraryVersion lib = ipp.jp.ippjGetLibVersion();
         string comment = "JPEG encoder based on " + lib.Name + " " + lib.BuildDate;
         int len = comment.Length + 2;
         if (m_dst.currPos + len >= m_dst.DataLen)
         {
             return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_COM);
         m_dst._WRITE_WORD(len+1);
         for (int i = 0; i < len - 2; i++)
         {
             m_dst._WRITE_BYTE((byte)comment[i]);
         }
         m_dst._WRITE_BYTE((byte)0);
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteDQT(ref CJPEGEncoderQuantTable qtbl)
      {
         int i;
         int len;
         len = DCTSIZE2 + 2 + 1;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_DQT);
         m_dst._WRITE_WORD(len);
         // precision/id
         m_dst._WRITE_BYTE((qtbl.m_precision << 4) | qtbl.m_id);
         for( i = 0; i < DCTSIZE2; i++ )
         {
            m_dst._WRITE_BYTE(qtbl.m_rbf[i]);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteDHT(ref CJPEGEncoderHuffmanTable htbl)
      {
         int i, len, htbl_len;

         for( htbl_len = 0, i = 0; i < 16; i++ )
         {
            htbl_len += htbl.m_bits[i];
         }
         len = 16 + htbl_len + 2 + 1;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_DHT);
         m_dst._WRITE_WORD(len);
         m_dst._WRITE_BYTE((htbl.m_hclass << 4) | htbl.m_id);
         for( i = 0; i < 16; i++ )
         {
            m_dst._WRITE_BYTE(htbl.m_bits[i]);
         }
         for( i = 0; i < htbl_len; i++ )
         {
            m_dst._WRITE_BYTE(htbl.m_vals[i]);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOF0()
      {
         int len;
         len = 8 + m_jpeg_ncomp * 3;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOF0);
         m_dst._WRITE_WORD(len);
         // sample precision
         m_dst._WRITE_BYTE(8);
         m_dst._WRITE_WORD(m_src.height);
         m_dst._WRITE_WORD(m_src.width);
         m_dst._WRITE_BYTE(m_jpeg_ncomp);
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            m_dst._WRITE_BYTE(i);
            m_dst._WRITE_BYTE((m_ccomp[i].m_hsampling << 4) | m_ccomp[i].m_vsampling);
            m_dst._WRITE_BYTE(m_ccomp[i].m_q_selector);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOF1()
      {
         int len;
         len = 8 + m_jpeg_ncomp * 3;

         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOF1);
         m_dst._WRITE_WORD(len);
         // sample precision
         m_dst._WRITE_BYTE(8);
         m_dst._WRITE_WORD(m_src.height);
         m_dst._WRITE_WORD(m_src.width);
         m_dst._WRITE_BYTE(m_jpeg_ncomp);
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            m_dst._WRITE_BYTE(i);
            m_dst._WRITE_BYTE((m_ccomp[i].m_hsampling << 4) | m_ccomp[i].m_vsampling);
            m_dst._WRITE_BYTE(m_ccomp[i].m_q_selector);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOF2()
      {
         int len;
         len = 8 + m_jpeg_ncomp * 3;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOF2);
         m_dst._WRITE_WORD(len);
         // sample precision
         m_dst._WRITE_BYTE(8);
         m_dst._WRITE_WORD(m_src.height);
         m_dst._WRITE_WORD(m_src.width);
         m_dst._WRITE_BYTE(m_jpeg_ncomp);
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            m_dst._WRITE_BYTE(i);
            m_dst._WRITE_BYTE((m_ccomp[i].m_hsampling << 4) | m_ccomp[i].m_vsampling);
            m_dst._WRITE_BYTE(m_ccomp[i].m_q_selector);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOF3()
      {
         int len;
         len = 8 + m_jpeg_ncomp * 3;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOF3);
         m_dst._WRITE_WORD(len);
         // sample precision
         m_dst._WRITE_BYTE(m_jpeg_precision);
         m_dst._WRITE_WORD(m_src.height);
         m_dst._WRITE_WORD(m_src.width);
         m_dst._WRITE_BYTE(m_jpeg_ncomp);
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            m_dst._WRITE_BYTE(i);
            m_dst._WRITE_BYTE((m_ccomp[i].m_hsampling << 4) | m_ccomp[i].m_vsampling);
            m_dst._WRITE_BYTE(m_ccomp[i].m_q_selector);
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteDRI(int restart_interval)
      {
         int len;
         len = 2 + 2;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_DRI);
         m_dst._WRITE_WORD(len);
         // emit restart interval
         m_dst._WRITE_WORD(restart_interval);
         m_restarts_to_go = m_jpeg_restart_interval;
         m_next_restart_num = 0;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteRST(int next_restart_num)
      {
         int len;
         len = 2;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         // emit restart interval
         m_dst._WRITE_BYTE(0xff);
         m_dst._WRITE_BYTE(0xff00 | ((int)JMARKER.JM_RST0 + next_restart_num));
         // Update next-restart state
         m_next_restart_num = (next_restart_num + 1) & 7;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOS()
      {
         int len;
         len = 3 + m_jpeg_ncomp*2 + 3;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOS);
         m_dst._WRITE_WORD(len);
         m_dst._WRITE_BYTE(m_jpeg_ncomp);
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            m_dst._WRITE_BYTE(i);
            m_dst._WRITE_BYTE((m_ccomp[i].m_dc_selector << 4) | m_ccomp[i].m_ac_selector);
         }
         m_dst._WRITE_BYTE(m_ss); // Ss
         m_dst._WRITE_BYTE(m_se); // Se
         m_dst._WRITE_BYTE(((m_ah << 4) | m_al));  // Ah/Al
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteSOS(int ncomp, int[] id, int Ss, int Se, int Ah, int Al)
      {
         int len;

         len = 3 + ncomp*2 + 3;
         if( m_dst.currPos + len >= m_dst.DataLen )
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_dst._WRITE_WORD(0xff00 | (int)JMARKER.JM_SOS);
         m_dst._WRITE_WORD(len);
         m_dst._WRITE_BYTE(ncomp);
         for( int i = 0; i < ncomp; i++ )
         {
            m_dst._WRITE_BYTE(id[i]);
            m_dst._WRITE_BYTE((m_ccomp[id[i]].m_dc_selector << 4) | m_ccomp[id[i]].m_ac_selector);
         }
         m_dst._WRITE_BYTE(Ss);       // Ss
         m_dst._WRITE_BYTE(Se);       // Se
         m_dst._WRITE_BYTE(((Ah & 0x0f) << 4) | (Al & 0x0f));  // Ah/Al
         return JERRCODE.JPEG_OK;
      }

      JERRCODE SelectScanScripts()
      {
         switch( m_jpeg_ncomp )
         {
            case 1:
               m_scan_count = 6;
               m_scan_script = new JPEG_SCAN[m_scan_count];
               for( int i=0; i<m_scan_count; i++ )
                  m_scan_script[i] = new JPEG_SCAN();
               // 1 DC scan, def
               m_scan_script[0].ncomp = 1;
               m_scan_script[0].id[0] = 0;
               m_scan_script[0].Ss    = 0;
               m_scan_script[0].Se    = 0;
               m_scan_script[0].Ah    = 0;
               m_scan_script[0].Al    = 1;
               // 2 AC scan, def(luma)
               m_scan_script[1].ncomp = 1;
               m_scan_script[1].id[0] = 0;
               m_scan_script[1].Ss    = 1;
               m_scan_script[1].Se    = 5;
               m_scan_script[1].Ah    = 0;
               m_scan_script[1].Al    = 2;
               // 3 AC scan, def(luma)
               m_scan_script[2].ncomp = 1;
               m_scan_script[2].id[0] = 0;
               m_scan_script[2].Ss    = 6;
               m_scan_script[2].Se    = 63;
               m_scan_script[2].Ah    = 0;
               m_scan_script[2].Al    = 2;
               // 4 AC scan, ref(luma)
               m_scan_script[3].ncomp = 1;
               m_scan_script[3].id[0] = 0;
               m_scan_script[3].Ss    = 1;
               m_scan_script[3].Se    = 63;
               m_scan_script[3].Ah    = 2;
               m_scan_script[3].Al    = 1;
               // 5 DC scan, ref
               m_scan_script[4].ncomp = 1;
               m_scan_script[4].id[0] = 0;
               m_scan_script[4].Ss    = 0;
               m_scan_script[4].Se    = 0;
               m_scan_script[4].Ah    = 1;
               m_scan_script[4].Al    = 0;
               // 6 AC scan, ref(luma)
               m_scan_script[5].ncomp = 1;
               m_scan_script[5].id[0] = 0;
               m_scan_script[5].Ss    = 1;
               m_scan_script[5].Se    = 63;
               m_scan_script[5].Ah    = 1;
               m_scan_script[5].Al    = 0;
               break;
            case 3:
               m_scan_count = 10;
               m_scan_script = new JPEG_SCAN [m_scan_count];
               for( int i=0; i<m_scan_count; i++ )
                  m_scan_script[i] = new JPEG_SCAN();
               // 1 DC scan, def
               m_scan_script[0].ncomp = 3;
               m_scan_script[0].id[0] = 0;
               m_scan_script[0].id[1] = 1;
               m_scan_script[0].id[2] = 2;
               m_scan_script[0].Ss    = 0;
               m_scan_script[0].Se    = 0;
               m_scan_script[0].Ah    = 0;
               m_scan_script[0].Al    = 1;
               // 2 AC scan, def(luma)
               m_scan_script[1].ncomp = 1;
               m_scan_script[1].id[0] = 0;
               m_scan_script[1].Ss    = 1;
               m_scan_script[1].Se    = 5;
               m_scan_script[1].Ah    = 0;
               m_scan_script[1].Al    = 2;
               // 3 AC scan, def(cr)
               m_scan_script[2].ncomp = 1;
               m_scan_script[2].id[0] = 2;
               m_scan_script[2].Ss    = 1;
               m_scan_script[2].Se    = 63;
               m_scan_script[2].Ah    = 0;
               m_scan_script[2].Al    = 1;
               // 4 AC scan, def(cb)
               m_scan_script[3].ncomp = 1;
               m_scan_script[3].id[0] = 1;
               m_scan_script[3].Ss    = 1;
               m_scan_script[3].Se    = 63;
               m_scan_script[3].Ah    = 0;
               m_scan_script[3].Al    = 1;
               // 5 AC scan, def(luma)
               m_scan_script[4].ncomp = 1;
               m_scan_script[4].id[0] = 0;
               m_scan_script[4].Ss    = 6;
               m_scan_script[4].Se    = 63;
               m_scan_script[4].Ah    = 0;
               m_scan_script[4].Al    = 2;
               // 7 AC scan, ref(luma)
               m_scan_script[5].ncomp = 1;
               m_scan_script[5].id[0] = 0;
               m_scan_script[5].Ss    = 1;
               m_scan_script[5].Se    = 63;
               m_scan_script[5].Ah    = 2;
               m_scan_script[5].Al    = 1;
               // 6 DC scan, ref
               m_scan_script[6].ncomp = 3;
               m_scan_script[6].id[0] = 0;
               m_scan_script[6].id[1] = 1;
               m_scan_script[6].id[2] = 2;
               m_scan_script[6].Ss    = 0;
               m_scan_script[6].Se    = 0;
               m_scan_script[6].Ah    = 1;
               m_scan_script[6].Al    = 0;
               // 8 AC scan, ref(cr)
               m_scan_script[7].ncomp = 1;
               m_scan_script[7].id[0] = 2;
               m_scan_script[7].Ss    = 1;
               m_scan_script[7].Se    = 63;
               m_scan_script[7].Ah    = 1;
               m_scan_script[7].Al    = 0;
               // 9 AC scan, ref(cb)
               m_scan_script[8].ncomp = 1;
               m_scan_script[8].id[0] = 1;
               m_scan_script[8].Ss    = 1;
               m_scan_script[8].Se    = 63;
               m_scan_script[8].Ah    = 1;
               m_scan_script[8].Al    = 0;
               // 10 AC scan, ref(luma)
               m_scan_script[9].ncomp = 1;
               m_scan_script[9].id[0] = 0;
               m_scan_script[9].Ss    = 1;
               m_scan_script[9].Se    = 63;
               m_scan_script[9].Ah    = 1;
               m_scan_script[9].Al    = 0;
               break;
            case 4:
               m_scan_count = 18;
               m_scan_script = new JPEG_SCAN [m_scan_count];
               for( int i=0; i<m_scan_count; i++ )
                  m_scan_script[i] = new JPEG_SCAN();
               // 1 DC scan, def
               m_scan_script[0].ncomp = 4;
               m_scan_script[0].id[0] = 0;
               m_scan_script[0].id[1] = 1;
               m_scan_script[0].id[2] = 2;
               m_scan_script[0].id[3] = 3;
               m_scan_script[0].Ss    = 0;
               m_scan_script[0].Se    = 0;
               m_scan_script[0].Ah    = 0;
               m_scan_script[0].Al    = 1;
               // 2 AC scan, def(0)
               m_scan_script[1].ncomp = 1;
               m_scan_script[1].id[0] = 0;
               m_scan_script[1].Ss    = 1;
               m_scan_script[1].Se    = 5;
               m_scan_script[1].Ah    = 0;
               m_scan_script[1].Al    = 2;
               // 3 AC scan, def(1)
               m_scan_script[2].ncomp = 1;
               m_scan_script[2].id[0] = 1;
               m_scan_script[2].Ss    = 1;
               m_scan_script[2].Se    = 5;
               m_scan_script[2].Ah    = 0;
               m_scan_script[2].Al    = 2;
               // 4 AC scan, def(2)
               m_scan_script[3].ncomp = 1;
               m_scan_script[3].id[0] = 2;
               m_scan_script[3].Ss    = 1;
               m_scan_script[3].Se    = 5;
               m_scan_script[3].Ah    = 0;
               m_scan_script[3].Al    = 2;
               // 5 AC scan, def(3)
               m_scan_script[4].ncomp = 1;
               m_scan_script[4].id[0] = 3;
               m_scan_script[4].Ss    = 1;
               m_scan_script[4].Se    = 5;
               m_scan_script[4].Ah    = 0;
               m_scan_script[4].Al    = 2;
               // 6 AC scan, def(0)
               m_scan_script[5].ncomp = 1;
               m_scan_script[5].id[0] = 0;
               m_scan_script[5].Ss    = 6;
               m_scan_script[5].Se    = 63;
               m_scan_script[5].Ah    = 0;
               m_scan_script[5].Al    = 2;
               // 7 AC scan, def(1)
               m_scan_script[6].ncomp = 1;
               m_scan_script[6].id[0] = 1;
               m_scan_script[6].Ss    = 6;
               m_scan_script[6].Se    = 63;
               m_scan_script[6].Ah    = 0;
               m_scan_script[6].Al    = 2;
               // 8 AC scan, def(2)
               m_scan_script[7].ncomp = 1;
               m_scan_script[7].id[0] = 2;
               m_scan_script[7].Ss    = 6;
               m_scan_script[7].Se    = 63;
               m_scan_script[7].Ah    = 0;
               m_scan_script[7].Al    = 2;
               // 9 AC scan, def(3)
               m_scan_script[8].ncomp = 1;
               m_scan_script[8].id[0] = 3;
               m_scan_script[8].Ss    = 6;
               m_scan_script[8].Se    = 63;
               m_scan_script[8].Ah    = 0;
               m_scan_script[8].Al    = 2;
               // 10 AC scan, ref(0)
               m_scan_script[9].ncomp = 1;
               m_scan_script[9].id[0] = 0;
               m_scan_script[9].Ss    = 1;
               m_scan_script[9].Se    = 63;
               m_scan_script[9].Ah    = 2;
               m_scan_script[9].Al    = 1;
               // 11 AC scan, ref(1)
               m_scan_script[10].ncomp = 1;
               m_scan_script[10].id[0] = 1;
               m_scan_script[10].Ss    = 1;
               m_scan_script[10].Se    = 63;
               m_scan_script[10].Ah    = 2;
               m_scan_script[10].Al    = 1;
               // 12 AC scan, ref(2)
               m_scan_script[11].ncomp = 1;
               m_scan_script[11].id[0] = 2;
               m_scan_script[11].Ss    = 1;
               m_scan_script[11].Se    = 63;
               m_scan_script[11].Ah    = 2;
               m_scan_script[11].Al    = 1;
               // 13 AC scan, ref(3)
               m_scan_script[12].ncomp = 1;
               m_scan_script[12].id[0] = 3;
               m_scan_script[12].Ss    = 1;
               m_scan_script[12].Se    = 63;
               m_scan_script[12].Ah    = 2;
               m_scan_script[12].Al    = 1;
               // 14 DC scan, ref
               m_scan_script[13].ncomp = 4;
               m_scan_script[13].id[0] = 0;
               m_scan_script[13].id[1] = 1;
               m_scan_script[13].id[2] = 2;
               m_scan_script[13].id[3] = 3;
               m_scan_script[13].Ss    = 0;
               m_scan_script[13].Se    = 0;
               m_scan_script[13].Ah    = 1;
               m_scan_script[13].Al    = 0;
               // 15 AC scan, ref(0)
               m_scan_script[14].ncomp = 1;
               m_scan_script[14].id[0] = 0;
               m_scan_script[14].Ss    = 1;
               m_scan_script[14].Se    = 63;
               m_scan_script[14].Ah    = 1;
               m_scan_script[14].Al    = 0;
               // 16 AC scan, ref(1)
               m_scan_script[15].ncomp = 1;
               m_scan_script[15].id[0] = 1;
               m_scan_script[15].Ss    = 1;
               m_scan_script[15].Se    = 63;
               m_scan_script[15].Ah    = 1;
               m_scan_script[15].Al    = 0;
               // 17 AC scan, ref(2)
               m_scan_script[16].ncomp = 1;
               m_scan_script[16].id[0] = 2;
               m_scan_script[16].Ss    = 1;
               m_scan_script[16].Se    = 63;
               m_scan_script[16].Ah    = 1;
               m_scan_script[16].Al    = 0;
               // 18 AC scan, ref(3)
               m_scan_script[17].ncomp = 1;
               m_scan_script[17].id[0] = 3;
               m_scan_script[17].Ss    = 1;
               m_scan_script[17].Se    = 63;
               m_scan_script[17].Ah    = 1;
               m_scan_script[17].Al    = 0;
               break;
            default:
               return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ColorConvert()
      {
         IppStatus status;
         IppiSize roi = new IppiSize(m_src.width, m_src.height);
         byte* src = m_src.Data8u;
         if( m_jpeg_color == JCOLOR.JC_UNKNOWN && m_src.color == JCOLOR.JC_UNKNOWN )
         {
            switch( m_jpeg_ncomp )
            {
               case 1:
                  fixed( byte* pDst = m_ccomp[0].m_cc_buffer )
                  {
                     status = ipp.ip.ippiCopy_8u_C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 3:
                  fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                               pDst1 = m_ccomp[1].m_cc_buffer,
                               pDst2 = m_ccomp[2].m_cc_buffer)
                  {
                     byte*[] dst = { pDst0, pDst1, pDst2 };
                     fixed (byte** pDst = dst)
                     {
                        status = ipp.ip.ippiCopy_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                     }
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 4:
                  fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                               pDst1 = m_ccomp[1].m_cc_buffer,
                               pDst2 = m_ccomp[2].m_cc_buffer,
                               pDst3 = m_ccomp[3].m_cc_buffer)
                  {
                     byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
                     fixed (byte** pDst = dst)
                     {
                        status = ipp.ip.ippiCopy_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                     }
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               default:
                  break;
            }
         }
         // Gray to Gray
         if( m_src.color == JCOLOR.JC_GRAY && m_jpeg_color == JCOLOR.JC_GRAY )
         {
            fixed( byte* pDst = m_ccomp[0].m_cc_buffer )
            {
               status = ipp.ip.ippiCopy_8u_C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to Gray
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_GRAY )
         {
            fixed( byte* pDst = m_ccomp[0].m_cc_buffer )
            {
               status = ipp.jp.ippiRGBToY_JPEG_8u_C3C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to RGB
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_RGB )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.ip.ippiCopy_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to YCbCr
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiRGBToYCbCr_JPEG_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // BGR to YCbCr
         if( m_src.color == JCOLOR.JC_BGR && m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiBGRToYCbCr_JPEG_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to CMYK
         if( m_src.color == JCOLOR.JC_CMYK && m_jpeg_color == JCOLOR.JC_CMYK )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer,
                         pDst3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.ip.ippiCopy_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to YCCK
         if( m_src.color == JCOLOR.JC_CMYK && m_jpeg_color == JCOLOR.JC_YCCK )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer,
                         pDst3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiCMYKToYCCK_JPEG_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ColorConvert(int nMCURow)
      {
         IppStatus status;
         if( nMCURow == m_numyMCU - 1 )
         {
            m_ccHeight = m_mcuHeight - m_yPadding;
         }
         IppiSize roi = new IppiSize(m_src.width, m_ccHeight);
         byte* src = m_src.Data8u + nMCURow*m_mcuHeight*m_src.lineStep;
         if( m_jpeg_color == JCOLOR.JC_UNKNOWN && m_src.color == JCOLOR.JC_UNKNOWN )
         {
            switch(m_jpeg_ncomp)
            {
               case 1:
                  fixed( byte* pDst=m_ccomp[0].m_cc_buffer )
                  {
                     status = ipp.ip.ippiCopy_8u_C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 3:
                  fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                               pDst1 = m_ccomp[1].m_cc_buffer,
                               pDst2 = m_ccomp[2].m_cc_buffer)
                  {
                     byte*[] dst = { pDst0, pDst1, pDst2 };
                     fixed (byte** pDst = dst)
                     {
                        status = ipp.ip.ippiCopy_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                     }
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 4:
                  fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                               pDst1 = m_ccomp[1].m_cc_buffer,
                               pDst2 = m_ccomp[2].m_cc_buffer,
                               pDst3 = m_ccomp[3].m_cc_buffer)
                  {
                     byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
                     fixed (byte** pDst = dst)
                     {
                        status = ipp.ip.ippiCopy_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
                     }
                  }
                  if( IppStatus.ippStsNoErr != status )
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               default:
                  return JERRCODE.JPEG_NOT_IMPLEMENTED;
            }
         }
         // Gray to Gray
         if( m_src.color == JCOLOR.JC_GRAY && m_jpeg_color == JCOLOR.JC_GRAY )
         {
            fixed( byte* pDst=m_ccomp[0].m_cc_buffer )
            {
               status = ipp.ip.ippiCopy_8u_C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to Gray
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_GRAY )
         {
            fixed( byte* pDst=m_ccomp[0].m_cc_buffer )
            {
               status = ipp.jp.ippiRGBToY_JPEG_8u_C3C1R(src, m_src.lineStep, pDst, m_ccWidth, roi);
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to RGB
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_RGB )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.ip.ippiCopy_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to YCbCr
         if( m_src.color == JCOLOR.JC_RGB && m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiRGBToYCbCr_JPEG_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // BGR to YCbCr
         if( m_src.color == JCOLOR.JC_BGR && m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiBGRToYCbCr_JPEG_8u_C3P3R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to CMYK
         if( m_src.color == JCOLOR.JC_CMYK && m_jpeg_color == JCOLOR.JC_CMYK )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer,
                         pDst3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.ip.ippiCopy_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to YCCK
         if( m_src.color == JCOLOR.JC_CMYK && m_jpeg_color == JCOLOR.JC_YCCK )
         {
            fixed (byte* pDst0 = m_ccomp[0].m_cc_buffer,
                         pDst1 = m_ccomp[1].m_cc_buffer,
                         pDst2 = m_ccomp[2].m_cc_buffer,
                         pDst3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] dst = { pDst0, pDst1, pDst2, pDst3 };
               fixed (byte** pDst = dst)
               {
                  status = ipp.jp.ippiCMYKToYCCK_JPEG_8u_C4P4R(src, m_src.lineStep, pDst, m_ccWidth, roi);
               }
            }
            if( IppStatus.ippStsNoErr != status )
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DownSampling()
      {
         int i, j, k;
         byte val;
         byte* p, p1;

         for( k = 0; k < m_jpeg_ncomp; k++ )
         {
            // expand right edge
            if( 0 != m_xPadding )
            {
               for( i = 0; i < m_src.height; i++ )
               {
                  fixed (byte* b = m_ccomp[k].m_cc_buffer)
                  {
                     p = b + i * m_mcuWidth * m_numxMCU;
                  }
                  val = p[m_src.width - 1];
                  for( j = 0; j < m_xPadding; j++ )
                  {
                     p[m_src.width + j] = val;
                  }
               }
            }
            // expand bottom edge only for last MCU row
            if( 0 != m_yPadding )
            {
               fixed( byte* b=m_ccomp[k].m_cc_buffer )
               {
                  p = b + (m_src.height-1)*m_mcuWidth*m_numxMCU;
                  for( i = 0; i < m_yPadding; i++ )
                  {
                     p1 = b + m_src.height*m_mcuWidth*m_numxMCU + i*m_mcuWidth*m_numxMCU;
                     ipp.sp.ippsCopy_8u(p, p1, m_mcuWidth*m_numxMCU);
                  }
               }
            }
            // sampling 444
            if( m_ccomp[k].m_h_factor == 1 && m_ccomp[k].m_v_factor == 1 )
            {
               custom_ipp.ippsCopy_8u(m_ccomp[k].m_cc_buffer, m_ccomp[k].m_ss_buffer, m_ccWidth * m_mcuHeight * m_numyMCU);
            }
            IppStatus status;
            // sampling 422
            if( m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 1 )
            {
               IppiSize srcRoi = new IppiSize(m_ccWidth, m_mcuHeight*m_numyMCU);
               IppiSize dstRoi = new IppiSize(m_ccWidth>>1, m_mcuHeight*m_numyMCU);
               status = custom_ipp.ippiSampleDownH2V1_JPEG_8u_C1R(m_ccomp[k].m_cc_buffer, m_ccWidth, srcRoi, m_ccomp[k].m_ss_buffer, m_ccWidth, dstRoi);
               if( IppStatus.ippStsNoErr != status )
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
            // sampling 411
            if( m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 2 )
            {
               IppiSize srcRoi = new IppiSize(m_ccWidth, m_mcuHeight*m_numyMCU);
               IppiSize dstRoi = new IppiSize(m_ccWidth>>1, m_mcuHeight*m_numyMCU>>1);
               status = custom_ipp.ippiSampleDownH2V2_JPEG_8u_C1R(m_ccomp[k].m_cc_buffer, m_ccWidth, srcRoi, m_ccomp[k].m_ss_buffer, m_ccWidth, dstRoi);
               if( IppStatus.ippStsNoErr != status )
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
         } // for m_jpeg_ncomp
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DownSampling(int nMCURow)
      {
         int i, j, k;
         byte val;
         byte* p, p1;

         for( k = 0; k < m_jpeg_ncomp; k++ )
         {
            // expand right edge
            if( 0 != m_xPadding )
            {
               for( i = 0; i < m_ccHeight; i++ )
               {
                  fixed (byte* b = m_ccomp[k].m_cc_buffer)
                  {
                     p = b + i * m_ccWidth;
                  }
                  val = p[m_src.width - 1];
                  for( j = 0; j < m_xPadding; j++ )
                  {
                     p[m_src.width + j] = val;
                  }
               }
            }
            // expand bottom edge only for last MCU row
            if( nMCURow == m_numyMCU - 1 )
            {
               fixed( byte* b=m_ccomp[k].m_cc_buffer )
               {
                  p = b + (m_ccHeight-1)*m_ccWidth;
                  for( i = 0; i < m_yPadding; i++ )
                  {
                     p1 = b + m_ccHeight*m_ccWidth + i*m_ccWidth;
                     ipp.sp.ippsCopy_8u(p, p1, m_ccWidth);
                  }
               }
            }
            // sampling 444
            if( m_ccomp[k].m_h_factor == 1 && m_ccomp[k].m_v_factor == 1 )
            {
               custom_ipp.ippsCopy_8u(m_ccomp[k].m_cc_buffer, m_ccomp[k].m_ss_buffer, m_ccWidth * m_mcuHeight);
            }
            IppStatus status;
            // sampling 422
            if( m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 1 )
            {
               IppiSize srcRoi = new IppiSize(m_ccWidth, m_mcuHeight);
               IppiSize dstRoi = new IppiSize(m_ccWidth>>1, m_mcuHeight);
               custom_ipp.ippsZero_8u(m_ccomp[k].m_ss_buffer, m_ccWidth * m_mcuHeight);
               status = custom_ipp.ippiSampleDownH2V1_JPEG_8u_C1R(m_ccomp[k].m_cc_buffer, m_ccWidth, srcRoi, m_ccomp[k].m_ss_buffer, m_ccWidth, dstRoi);
               if( IppStatus.ippStsNoErr != status )
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
            // sampling 411
            if( m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 2 )
            {
               IppiSize srcRoi = new IppiSize(m_ccWidth, m_mcuHeight);
               IppiSize dstRoi = new IppiSize(m_ccWidth>>1, m_mcuHeight>>1);
               status = custom_ipp.ippiSampleDownH2V2_JPEG_8u_C1R(m_ccomp[k].m_cc_buffer, m_ccWidth, srcRoi, m_ccomp[k].m_ss_buffer, m_ccWidth, dstRoi);
               if( IppStatus.ippStsNoErr != status )
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
         } // for m_jpeg_ncomp
         return JERRCODE.JPEG_OK;
      }

      JERRCODE PerformDCT()
      {
         int i, j, n, k, l, size, src_step;
         byte* src;
         short*  block;
         IppStatus status;

         src_step = m_mcuWidth*m_numxMCU;
         for( size = 0, n = 0; n < m_jpeg_ncomp; n++ )
         {
            size += (m_ccomp[n].m_hsampling * m_ccomp[n].m_vsampling);
         }
         for( i = 0; i < m_numyMCU; i++ )
         {
            for( j = 0; j < m_numxMCU; j++ )
            {
               fixed( short* p=m_coefbuf )
               {
                  block = p + (DCTSIZE2*size*(j+(i*m_numxMCU)));
                  for (n = 0; n < m_jpeg_ncomp; n++)
                  {
                     fixed (ushort* qtbl = m_qntbl[m_ccomp[n].m_q_selector].m_qbf)
                     {
                        for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                        {
                           for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                           {
                              fixed (byte* ps = m_ccomp[n].m_ss_buffer)
                              {
                                 src = ps +
                                    i * 8 * m_ccomp[n].m_vsampling * m_ccWidth +
                                    j * 8 * m_ccomp[n].m_hsampling +
                                    k * 8 * m_ccWidth;
                                 src += l * 8;
                                 status = ipp.jp.ippiDCTQuantFwd8x8LS_JPEG_8u16s_C1R(
                                    src,
                                    src_step,
                                    block,
                                    qtbl);
                              }
                              if (IppStatus.ippStsNoErr != status)
                              {
                                 return JERRCODE.JPEG_INTERNAL_ERROR;
                              }
                              block += DCTSIZE2;
                           } // for m_hsampling
                        } // for m_vsampling
                     }
                  }
               }
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ProcessRestart(int[] id, int Ss, int Se, int Ah, int Al)
      {
         JERRCODE  jerr;
         IppStatus status = IppStatus.ippStsNoErr;
         // flush IppiEncodeHuffmanState
         fixed (byte* pData = m_dst.pData, state = m_state.m_buf)
         {
            int currPos = m_dst.currPos;
            switch (m_jpeg_mode)
            {
               case JMODE.JPEG_BASELINE:
                  status = ipp.jp.ippiEncodeHuffman8x8_JPEG_16s1u_C1(
                     null, pData, m_dst.DataLen, &currPos, null, null, null, (IppiEncodeHuffmanState*)state, 1);
                  break;
               case JMODE.JPEG_PROGRESSIVE:
                  if (Ss == 0 && Se == 0)
                  {
                     // DC scan
                     if (Ah == 0)
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1(
                           null, pData, m_dst.DataLen, &currPos, null, 0, null, (IppiEncodeHuffmanState*)state, 1);
                     }
                     else
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1(
                           null, pData, m_dst.DataLen, &currPos, 0, (IppiEncodeHuffmanState*)state, 1);
                     }
                  }
                  else
                  {
                     // AC scan
                     fixed (byte* actbl = m_actbl[m_ccomp[id[0]].m_ac_selector].m_buf)
                     {
                        if (Ah == 0)
                        {
                           status = ipp.jp.ippiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1(
                              null, pData, m_dst.DataLen, &currPos, Ss, Se, Al,
                              (IppiEncodeHuffmanSpec*)actbl, (IppiEncodeHuffmanState*)state, 1);
                        }
                        else
                        {
                           status = ipp.jp.ippiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1(
                              null, pData, m_dst.DataLen, &currPos, Ss, Se, Al,
                              (IppiEncodeHuffmanSpec*)actbl, (IppiEncodeHuffmanState*)state, 1);
                        }
                     }
                  }
                  break;
               case JMODE.JPEG_LOSSLESS:
                  status = ipp.jp.ippiEncodeHuffmanOne_JPEG_16s1u_C1(
                     null, pData, m_dst.DataLen, &currPos, null, (IppiEncodeHuffmanState*)state, 1);
                  break;
            }
            m_dst.currPos = currPos;
         }
         if( IppStatus.ippStsNoErr > status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         status = custom_ipp.ippiEncodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         jerr = WriteRST(m_next_restart_num);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for( int c = 0; c < m_jpeg_ncomp; c++ )
         {
            m_ccomp[c].m_lastDC = 0;
         }
         m_restarts_to_go = m_jpeg_restart_interval;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ProcessRestart(
         int[,]    stat,
         int[]    id,
         int    Ss,
         int    Se,
         int    Ah,
         int    Al)
      {
         IppStatus status;
         // flush IppiEncodeHuffmanState
         fixed (byte* state = m_state.m_buf)
         {
            if (JMODE.JPEG_PROGRESSIVE == m_jpeg_mode)
            {
               if (Ss == 0 && Se == 0)
               {
                  // DC scan
                  // nothing to do
               }
               else
               {
                  // AC scan
                  if (Ah == 0)
                  {
                     fixed (int* s = &stat[m_ccomp[id[0]].m_ac_selector, 0])
                     {
                        status = ipp.jp.ippiGetHuffmanStatistics8x8_ACFirst_JPEG_16s_C1(
                           null, s, Ss, Se, Al, (IppiEncodeHuffmanState*)state, 1);
                     }
                     if (IppStatus.ippStsNoErr > status)
                     {
                        return JERRCODE.JPEG_INTERNAL_ERROR;
                     }
                  }
                  else
                  {
                     fixed (int* s = &stat[m_ccomp[id[0]].m_ac_selector, 0])
                     {
                        status = ipp.jp.ippiGetHuffmanStatistics8x8_ACRefine_JPEG_16s_C1(
                           null, s, Ss, Se, Al, (IppiEncodeHuffmanState*)state, 1);
                     }
                     if (IppStatus.ippStsNoErr > status)
                     {
                        return JERRCODE.JPEG_INTERNAL_ERROR;
                     }
                  }
               }
            }
            status = ipp.jp.ippiEncodeHuffmanStateInit_JPEG_8u((IppiEncodeHuffmanState*)state);
         }
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for( int c = 0; c < m_jpeg_ncomp; c++ )
         {
            m_ccomp[c].m_lastDC = 0;
         }
         m_restarts_to_go = m_jpeg_restart_interval;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE Init()
      {
         JERRCODE  jerr;
         switch( m_jpeg_color )
         {
            case JCOLOR.JC_GRAY:  m_jpeg_ncomp = 1; break;
            case JCOLOR.JC_RGB:   m_jpeg_ncomp = 3; break;
            case JCOLOR.JC_YCBCR: m_jpeg_ncomp = 3; break;
            case JCOLOR.JC_CMYK:  m_jpeg_ncomp = 4; break;
            case JCOLOR.JC_YCCK:  m_jpeg_ncomp = 4; break;
            default:
               // let to user selects the number of component
               break;
         }
         m_xPadding = m_numxMCU * m_mcuWidth  - m_src.width;
         m_yPadding = m_numyMCU * m_mcuHeight - m_src.height;
         m_ccWidth  = m_mcuWidth * m_numxMCU;
         m_ccHeight = m_mcuHeight;
         for( int i = 0; i < m_jpeg_ncomp; i++ )
         {
            int cc_buf_size;
            int ss_buf_size;
            switch( m_jpeg_mode )
            {
               case JMODE.JPEG_BASELINE:
                  cc_buf_size = m_ccWidth*m_ccHeight;
                  ss_buf_size = m_ccWidth*m_ccHeight;
                  break;
               case JMODE.JPEG_PROGRESSIVE:
                  cc_buf_size = (m_mcuWidth*m_mcuHeight)*(m_numxMCU*m_numyMCU);
                  ss_buf_size = (m_mcuWidth*m_mcuHeight)*(m_numxMCU*m_numyMCU);
                  break;
               case JMODE.JPEG_LOSSLESS:
                  cc_buf_size = m_ccWidth*sizeof(short);
                  ss_buf_size = m_ccWidth*sizeof(short);
                  break;
               default:
                  return JERRCODE.JPEG_INTERNAL_ERROR;
            }
            m_ccomp[i].m_id          = i;
            m_ccomp[i].m_comp_no     = i;
            m_ccomp[i].m_hsampling   = (m_jpeg_sampling == JSS.JS_444) ? 1 : (i == 0 || i == 3 ? 2 : 1);
            m_ccomp[i].m_vsampling   = (m_jpeg_sampling == JSS.JS_411) ? (i == 0 || i == 3 ? 2 : 1) : 1;
            m_ccomp[i].m_h_factor    = (m_jpeg_sampling == JSS.JS_444) ? 1 : (i == 0 || i == 3? 1 : 2);
            m_ccomp[i].m_v_factor    = (m_jpeg_sampling == JSS.JS_411) ? (i == 0 || i == 3 ? 1 : 2) : 1;
            m_ccomp[i].m_nblocks     = m_ccomp[i].m_hsampling * m_ccomp[i].m_vsampling;
            m_ccomp[i].m_q_selector  = (i == 0 || i == 3) ? 0 : (m_jpeg_color == JCOLOR.JC_YCBCR || m_jpeg_color == JCOLOR.JC_YCCK ? 1 : 0);
            m_ccomp[i].m_dc_selector = (i == 0 || i == 3) ? 0 : 1;
            m_ccomp[i].m_ac_selector = (i == 0 || i == 3) ? 0 : 1;
            // color convert intermediate buffer
            m_ccomp[i].m_cc_buffer = new byte[cc_buf_size];
            // subsampling buffer
            m_ccomp[i].m_ss_buffer = new byte[ss_buf_size];
            // huffman buffer
            m_ccomp[i].m_top_row = new byte[ss_buf_size];
            //fixed( byte* p=m_ccomp[i].m_cc_buffer ) m_ccomp[i].m_curr_row=(short*)p;
            //fixed( byte* p=m_ccomp[i].m_ss_buffer ) m_ccomp[i].m_prev_row=(short*)p;
            m_ccomp[i].m_curr_row = m_ccomp[i].m_cc_buffer;
            m_ccomp[i].m_curr_ofs = 0;
            m_ccomp[i].m_prev_row = m_ccomp[i].m_ss_buffer;
            m_ccomp[i].m_prev_ofs = 0;
         }
         if( JMODE.JPEG_PROGRESSIVE == m_jpeg_mode )
         {
            SelectScanScripts();
         }
         if( JMODE.JPEG_PROGRESSIVE == m_jpeg_mode )
         {
            int sz = m_numxMCU*m_numyMCU*MAX_BYTES_PER_MCU;
            m_coefbuf = new short[sz];
         }
         m_dctbl[0].Create();
         m_dctbl[1].Create();
         m_actbl[0].Create();
         m_actbl[1].Create();
         if( JMODE.JPEG_LOSSLESS != m_jpeg_mode )
         {
            jerr = m_qntbl[0].Init(0, m_jpeg_quality, DefaultLuminanceQuant);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
            jerr = m_qntbl[1].Init(1, m_jpeg_quality, DefaultChrominanceQuant);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = m_dctbl[0].Init(0, 0, DefaultLuminanceDCBits, DefaultLuminanceDCValues);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = m_dctbl[1].Init(1, 0, DefaultChrominanceDCBits, DefaultChrominanceDCValues);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = m_actbl[0].Init(0, 1, DefaultLuminanceACBits, DefaultLuminanceACValues);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = m_actbl[1].Init(1, 1, DefaultChrominanceACBits, DefaultChrominanceACValues);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         m_state.Create();
         return JERRCODE.JPEG_OK;
      }

      JERRCODE GenerateHuffmanTables(int ncomp, int[] id, int Ss, int Se, int Ah, int Al)
      {
         int  i, j, k, n, l, c, size;
         int[,] dc_statistics = new int[2,256];
         int[,] ac_statistics = new int[2,256];
         JERRCODE jerr;
         IppStatus status;

         for( n = 0; n < m_jpeg_ncomp; n++ )
         {
            m_ccomp[n].m_lastDC = 0;
         }
         status = custom_ipp.ippiEncodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for( size = 0, k = 0; k < m_jpeg_ncomp; k++ )
         {
            size += (m_ccomp[k].m_hsampling * m_ccomp[k].m_vsampling);
         }
         short* block;
         fixed (short* pBuf = m_coefbuf)
         {
            if (Ss != 0 && Se != 0)
            {
               // AC scan
               for (i = 0; i < m_numyMCU; i++)
               {
                  for (k = 0; k < m_ccomp[id[0]].m_vsampling; k++)
                  {
                     if (i * m_ccomp[id[0]].m_vsampling * 8 + k * 8 >= m_src.height)
                        break;
                     for (j = 0; j < m_numxMCU; j++)
                     {
                        block = pBuf + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                        // skip any relevant components
                        for (c = 0; c < m_ccomp[id[0]].m_comp_no; c++)
                        {
                           block += (DCTSIZE2 * m_ccomp[c].m_hsampling *
                              m_ccomp[c].m_vsampling);
                        }
                        // Skip over relevant 8x8 blocks from this component.
                        block += (k * DCTSIZE2 * m_ccomp[id[0]].m_hsampling);
                        for (l = 0; l < m_ccomp[id[0]].m_hsampling; l++)
                        {
                           if (0 != m_jpeg_restart_interval)
                           {
                              if (m_restarts_to_go == 0)
                              {
                                 jerr = ProcessRestart(ac_statistics, id, Ss, Se, Ah, Al);
                                 if (JERRCODE.JPEG_OK != jerr)
                                 {
                                    return jerr;
                                 }
                              }
                           }
                           // Ignore the last column(s) of the image.
                           if (((j * m_ccomp[id[0]].m_hsampling * 8) + (l * 8)) >= m_src.width)
                              break;
                           if (Ah == 0)
                           {
                              fixed (int* p = &ac_statistics[m_ccomp[id[0]].m_ac_selector, 0])
                              {
                                 fixed (byte* state = m_state.m_buf)
                                 {
                                    status = ipp.jp.ippiGetHuffmanStatistics8x8_ACFirst_JPEG_16s_C1(
                                       block,
                                       p,
                                       Ss,
                                       Se,
                                       Al,
                                       (IppiEncodeHuffmanState*)state,
                                       0);
                                 }
                              }
                              if (IppStatus.ippStsNoErr > status)
                              {
                                 return JERRCODE.JPEG_INTERNAL_ERROR;
                              }
                           }
                           else
                           {
                              fixed (int* p = &ac_statistics[m_ccomp[id[0]].m_ac_selector, 0])
                              {
                                 fixed (byte* state = m_state.m_buf)
                                 {
                                    status = ipp.jp.ippiGetHuffmanStatistics8x8_ACRefine_JPEG_16s_C1(
                                       block,
                                       p,
                                       Ss,
                                       Se,
                                       Al,
                                       (IppiEncodeHuffmanState*)state,
                                       0);
                                 }
                              }
                              if (IppStatus.ippStsNoErr > status)
                              {
                                 return JERRCODE.JPEG_INTERNAL_ERROR;
                              }
                           }
                           block += DCTSIZE2;
                           m_restarts_to_go--;
                        } // for m_hsampling
                     } // for m_numxMCU
                  } // for m_vsampling
               } // for m_numyMCU
               if (Ah == 0)
               {
                  fixed (int* p = &ac_statistics[m_ccomp[id[0]].m_ac_selector, 0])
                  {
                     fixed (byte* state = m_state.m_buf)
                     {
                        status = ipp.jp.ippiGetHuffmanStatistics8x8_ACFirst_JPEG_16s_C1(
                           null,
                           p,
                           Ss,
                           Se,
                           Al,
                           (IppiEncodeHuffmanState*)state,
                           1);
                     }
                  }
                  if (IppStatus.ippStsNoErr > status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               else
               {
                  fixed (int* p = &ac_statistics[m_ccomp[id[0]].m_ac_selector, 0])
                  {
                     fixed (byte* state = m_state.m_buf)
                     {
                        status = ipp.jp.ippiGetHuffmanStatistics8x8_ACRefine_JPEG_16s_C1(
                           null,
                           p,
                           Ss,
                           Se,
                           Al,
                           (IppiEncodeHuffmanState*)state,
                           1);
                     }
                  }
                  if (IppStatus.ippStsNoErr > status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               byte[] bits = new byte[16];
               byte[] vals = new byte[256];
               fixed (int* p = &ac_statistics[m_ccomp[id[0]].m_ac_selector, 0])
               {
                  fixed (byte* b = bits, v = vals)
                     status = ipp.jp.ippiEncodeHuffmanRawTableInit_JPEG_8u(p, b, v);
               }
               if (IppStatus.ippStsNoErr > status)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
               jerr = m_actbl[m_ccomp[id[0]].m_ac_selector].Init(m_ccomp[id[0]].m_ac_selector, 1, bits, vals);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return jerr;
               }
            }
            else
            {
               // DC scan
               if (Ah == 0)
               {
                  for (i = 0; i < m_numyMCU; i++)
                  {
                     for (j = 0; j < m_numxMCU; j++)
                     {
                        if (0 != m_jpeg_restart_interval)
                        {
                           if (m_restarts_to_go == 0)
                           {
                              jerr = ProcessRestart(dc_statistics, id, Ss, Se, Ah, Al);
                              if (JERRCODE.JPEG_OK != jerr)
                              {
                                 return jerr;
                              }
                           }
                        }
                        block = pBuf + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                        // first DC scan
                        for (n = 0; n < m_jpeg_ncomp; n++)
                        {
                           short* lastDC;
                           fixed (short* p = &m_ccomp[n].m_lastDC) lastDC = p;
                           for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                           {
                              for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                              {
                                 fixed (int* p = &dc_statistics[m_ccomp[n].m_dc_selector, 0])
                                 {
                                    status = ipp.jp.ippiGetHuffmanStatistics8x8_DCFirst_JPEG_16s_C1(
                                       block, p, lastDC, Al);
                                 }
                                 if (IppStatus.ippStsNoErr > status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                                 block += DCTSIZE2;
                              } // for m_hsampling
                           } // for m_vsampling
                        } // for m_jpeg_ncomp
                        m_restarts_to_go--;
                     } // for m_numxMCU
                  } // for m_numyMCU
                  for (n = 0; n < ncomp; n++)
                  {
                     byte[] bits = new byte[16];
                     byte[] vals = new byte[256];
                     fixed (int* p = &dc_statistics[m_ccomp[n].m_dc_selector, 0])
                     {
                        fixed (byte* b = bits, v = vals)
                           status = ipp.jp.ippiEncodeHuffmanRawTableInit_JPEG_8u(p, b, v);
                     }
                     if (IppStatus.ippStsNoErr > status)
                     {
                        return JERRCODE.JPEG_INTERNAL_ERROR;
                     }
                     jerr = m_dctbl[m_ccomp[n].m_dc_selector].Init(m_ccomp[n].m_dc_selector, 0, bits, vals);
                     if (JERRCODE.JPEG_OK != jerr)
                     {
                        return jerr;
                     }
                  }
               } // Ah == 0
            }
         }
         return JERRCODE.JPEG_OK;
      } // CJPEGEncoder::GenerateHuffmanTables()

      JERRCODE EncodeScan(int ncomp, int[] id, int Ss, int Se, int Ah, int Al)
      {
         int  i, j, k, n, l, c, size;
         short* block;
         JERRCODE jerr;
         IppStatus status;

         GenerateHuffmanTables(ncomp, id, Ss, Se, Ah, Al);
         for( n = 0; n < m_jpeg_ncomp; n++ )
         {
            m_ccomp[n].m_lastDC = 0;
         }
         m_restarts_to_go = m_jpeg_restart_interval;
         fixed (byte* state = m_state.m_buf)
         {
            status = ipp.jp.ippiEncodeHuffmanStateInit_JPEG_8u((IppiEncodeHuffmanState*)state);
         }
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for( size = 0, k = 0; k < m_jpeg_ncomp; k++ )
         {
            size += (m_ccomp[k].m_hsampling * m_ccomp[k].m_vsampling);
         }
         if( Ss != 0 && Se != 0 )
         {
            fixed (byte* pData = m_dst.pData)
            {
               fixed (short* pBuf = m_coefbuf)
               {
                  jerr = WriteDHT(ref m_actbl[m_ccomp[id[0]].m_ac_selector]);
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  jerr = WriteSOS(ncomp, id, Ss, Se, Ah, Al);
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  // AC scan
                  for (i = 0; i < m_numyMCU; i++)
                  {
                     for (k = 0; k < m_ccomp[id[0]].m_vsampling; k++)
                     {
                        if (i * m_ccomp[id[0]].m_vsampling * 8 + k * 8 >= m_src.height)
                           break;
                        for (j = 0; j < m_numxMCU; j++)
                        {
                           block = pBuf + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                           // skip any relevant components
                           for (c = 0; c < m_ccomp[id[0]].m_comp_no; c++)
                           {
                              block += (DCTSIZE2 * m_ccomp[c].m_hsampling *
                                 m_ccomp[c].m_vsampling);
                           }
                           // Skip over relevant 8x8 blocks from this component.
                           block += (k * DCTSIZE2 * m_ccomp[id[0]].m_hsampling);
                           for (l = 0; l < m_ccomp[id[0]].m_hsampling; l++)
                           {
                              // Ignore the last column(s) of the image.
                              if (((j * m_ccomp[id[0]].m_hsampling * 8) + (l * 8)) >= m_src.width)
                                 break;
                              if (0 != m_jpeg_restart_interval)
                              {
                                 if (m_restarts_to_go == 0)
                                 {
                                    jerr = ProcessRestart(id, Ss, Se, Ah, Al);
                                    if (JERRCODE.JPEG_OK != jerr)
                                    {
                                       return jerr;
                                    }
                                 }
                              }
                              fixed (byte* actbl = m_actbl[m_ccomp[id[0]].m_ac_selector].m_buf,
                                           state = m_state.m_buf)
                              {
                                 int currPos = m_dst.currPos;
                                 if (Ah == 0)
                                 {
                                    status = ipp.jp.ippiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1(
                                       block,
                                       pData,
                                       m_dst.DataLen,
                                       &currPos,
                                       Ss,
                                       Se,
                                       Al,
                                       (IppiEncodeHuffmanSpec*)actbl,
                                       (IppiEncodeHuffmanState*)state,
                                       0);
                                    if (IppStatus.ippStsNoErr > status)
                                    {
                                       return JERRCODE.JPEG_INTERNAL_ERROR;
                                    }
                                 }
                                 else
                                 {
                                    status = ipp.jp.ippiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1(
                                       block,
                                       pData,
                                       m_dst.DataLen,
                                       &currPos,
                                       Ss,
                                       Se,
                                       Al,
                                       (IppiEncodeHuffmanSpec*)actbl,
                                       (IppiEncodeHuffmanState*)state,
                                       0);
                                    if (IppStatus.ippStsNoErr > status)
                                    {
                                       return JERRCODE.JPEG_INTERNAL_ERROR;
                                    }
                                 }
                                 m_dst.currPos = currPos;
                              }
                              block += DCTSIZE2;
                              m_restarts_to_go--;
                           } // for m_hsampling
                        } // for m_numxMCU
                     } // for m_vsampling
                  } // for m_numyMCU
                  //actbl = m_actbl[m_ccomp[id[0]].m_ac_selector].m_table;
                  fixed (byte* actbl = m_actbl[m_ccomp[id[0]].m_ac_selector].m_buf,
                               state = m_state.m_buf)
                  {
                     int currPos = m_dst.currPos;
                     if (Ah == 0)
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1(
                           null,
                           pData,
                           m_dst.DataLen,
                           &currPos,
                           Ss,
                           Se,
                           Al,
                           (IppiEncodeHuffmanSpec*)actbl,
                           (IppiEncodeHuffmanState*)state,
                           1);
                        if (IppStatus.ippStsNoErr > status)
                        {
                           return JERRCODE.JPEG_INTERNAL_ERROR;
                        }
                     }
                     else
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1(
                           null,
                           pData,
                           m_dst.DataLen,
                           &currPos,
                           Ss,
                           Se,
                           Al,
                           (IppiEncodeHuffmanSpec*)actbl,
                           (IppiEncodeHuffmanState*)state,
                           1);
                        if (IppStatus.ippStsNoErr > status)
                        {
                           return JERRCODE.JPEG_INTERNAL_ERROR;
                        }
                     }
                     m_dst.currPos = currPos;
                  }
               }
            }
         }
         else
         {
            fixed (byte* pData = m_dst.pData)
            {
               fixed (short* pBuf = m_coefbuf)
               {
                  if (Ah == 0)
                  {
                     jerr = WriteDHT(ref m_dctbl[0]);
                     if (JERRCODE.JPEG_OK != jerr)
                     {
                        return jerr;
                     }
                     if (m_jpeg_ncomp != 1)
                     {
                        jerr = WriteDHT(ref m_dctbl[1]);
                        if (JERRCODE.JPEG_OK != jerr)
                        {
                           return jerr;
                        }
                     }
                  }
                  jerr = WriteSOS(ncomp, id, Ss, Se, Ah, Al);
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  // DC scan
                  for (i = 0; i < m_numyMCU; i++)
                  {
                     for (j = 0; j < m_numxMCU; j++)
                     {
                        if (0 != m_jpeg_restart_interval)
                        {
                           if (m_restarts_to_go == 0)
                           {
                              jerr = ProcessRestart(id, Ss, Se, Ah, Al);
                              if (JERRCODE.JPEG_OK != jerr)
                              {
                                 return jerr;
                              }
                           }
                        }
                        block = pBuf + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                        if (Ah == 0)
                        {
                           // first DC scan
                           for (n = 0; n < m_jpeg_ncomp; n++)
                           {
                              fixed (byte* dctbl = m_dctbl[m_ccomp[n].m_dc_selector].m_buf,
                                           state = m_state.m_buf)
                              {
                                 short lastDC = m_ccomp[n].m_lastDC;
                                 for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                                 {
                                    for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                                    {
                                       int currPos = m_dst.currPos;
                                       status = ipp.jp.ippiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1(
                                          block,
                                          pData,
                                          m_dst.DataLen,
                                          &currPos,
                                          &lastDC,
                                          Al,
                                          (IppiEncodeHuffmanSpec*)dctbl,
                                          (IppiEncodeHuffmanState*)state,
                                          0);
                                       m_dst.currPos = currPos;
                                       m_ccomp[n].m_lastDC = lastDC;
                                       if (IppStatus.ippStsNoErr > status)
                                       {
                                          return JERRCODE.JPEG_INTERNAL_ERROR;
                                       }
                                       block += DCTSIZE2;
                                    } // for m_hsampling
                                 } // for m_vsampling
                              }
                           } // for m_jpeg_ncomp
                        }
                        else
                        {
                           // refine DC scan
                           for (n = 0; n < m_jpeg_ncomp; n++)
                           {
                              for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                              {
                                 for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                                 {
                                    int currPos = m_dst.currPos;
                                    fixed (byte* state = m_state.m_buf)
                                    {
                                       status = ipp.jp.ippiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1(
                                          block,
                                          pData,
                                          m_dst.DataLen,
                                          &currPos,
                                          Al,
                                          (IppiEncodeHuffmanState*)state,
                                          0);
                                    }
                                    m_dst.currPos = currPos;
                                    if (IppStatus.ippStsNoErr > status)
                                    {
                                       return JERRCODE.JPEG_INTERNAL_ERROR;
                                    }
                                    block += DCTSIZE2;
                                 } // for m_hsampling
                              } // for m_vsampling
                           } // for m_jpeg_ncomp
                        }
                        m_restarts_to_go--;
                     } // for m_numxMCU
                  } // for m_numyMCU
                  fixed (byte* state = m_state.m_buf)
                  {
                     int currPos = m_dst.currPos;
                     if (Ah == 0)
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1(
                           null, pData, m_dst.DataLen, &currPos, null, 0, null, (IppiEncodeHuffmanState*)state, 1);
                        if (IppStatus.ippStsNoErr > status)
                        {
                           return JERRCODE.JPEG_INTERNAL_ERROR;
                        }
                     }
                     else
                     {
                        status = ipp.jp.ippiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1(
                           null, pData, m_dst.DataLen, &currPos, 0, (IppiEncodeHuffmanState*)state, 1);
                        if (IppStatus.ippStsNoErr > status)
                        {
                           return JERRCODE.JPEG_INTERNAL_ERROR;
                        }
                     }
                     m_dst.currPos = currPos;
                  }
               }
            }
         }
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE WriteImageBaseline(ref int realSize)
      {
         JERRCODE jerr;

         jerr = Init();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteSOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_color == JCOLOR.JC_GRAY || m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            jerr = WriteAPP0();
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         if( m_jpeg_color == JCOLOR.JC_RGB  ||
            m_jpeg_color == JCOLOR.JC_CMYK ||
            m_jpeg_color == JCOLOR.JC_YCCK )
         {
            jerr = WriteAPP14();
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteCOM();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteDQT(ref m_qntbl[0]);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_ncomp != 1 )
         {
            jerr = WriteDQT(ref m_qntbl[1]);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteSOF0();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteDHT( ref m_dctbl[0]);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_ncomp != 1 )
         {
            jerr = WriteDHT(ref m_dctbl[1]);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteDHT(ref m_actbl[0]);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_ncomp != 1 )
         {
            jerr = WriteDHT(ref m_actbl[1]);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         if( 0 != m_jpeg_restart_interval )
         {
            jerr = WriteDRI(m_jpeg_restart_interval);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteSOS();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }

         IppStatus status;
         short[] block = new short[DCTSIZE2+(CPU_CACHE_LINE-1)];
         fixed (byte* state = m_state.m_buf)
         {
            status = ipp.jp.ippiEncodeHuffmanStateInit_JPEG_8u((IppiEncodeHuffmanState*)state);
         }
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         fixed (short* pBlock = block)
         {
            for (int i = 0; i < m_numyMCU; i++)
            {
               jerr = ColorConvert(i);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return jerr;
               }
               jerr = DownSampling(i);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return jerr;
               }
               byte* src = null;
               int src_step = m_ccWidth;
               for (int j = 0; j < m_numxMCU; j++)
               {
                  // process restart interval, if any
                  if (0 != m_jpeg_restart_interval)
                  {
                     if (m_restarts_to_go == 0)
                     {
                        ProcessRestart(null, 0, 63, 0, 0);
                     }
                  }
                  for (int n = 0; n < m_jpeg_ncomp; n++)
                  {
                     fixed (ushort* qtbl = m_qntbl[m_ccomp[n].m_q_selector].m_qbf)
                     {
                        fixed (byte* pBuf = m_ccomp[n].m_ss_buffer,
                                     pDCTbl = m_dctbl[m_ccomp[n].m_dc_selector].m_buf,
                                     pACTbl = m_actbl[m_ccomp[n].m_ac_selector].m_buf)
                        {
                           for (int k = 0; k < m_ccomp[n].m_vsampling; k++)
                           {
                              src = pBuf + j * 8 * m_ccomp[n].m_hsampling + k * 8 * m_ccWidth;
                              for (int l = 0; l < m_ccomp[n].m_hsampling; l++)
                              {
                                 src += l * 8;
                                 status = ipp.jp.ippiDCTQuantFwd8x8LS_JPEG_8u16s_C1R(
                                    src,
                                    src_step,
                                    pBlock,
                                    qtbl);
                                 if (IppStatus.ippStsNoErr > status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                                 fixed (byte* pData = m_dst.pData, state = m_state.m_buf)
                                 {
                                    short lastDC = m_ccomp[n].m_lastDC;
                                    int currPos = m_dst.currPos;
                                    status = ipp.jp.ippiEncodeHuffman8x8_JPEG_16s1u_C1(
                                       pBlock,
                                       pData,
                                       m_dst.DataLen,
                                       &currPos,
                                       &lastDC,
                                       (IppiEncodeHuffmanSpec*)pDCTbl,
                                       (IppiEncodeHuffmanSpec*)pACTbl,
                                       (IppiEncodeHuffmanState*)state,
                                       0);
                                    m_ccomp[n].m_lastDC = lastDC;
                                    m_dst.currPos = currPos;
                                 }
                                 if (IppStatus.ippStsNoErr > status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                              } // for m_hsampling
                           } // for m_vsampling
                        }
                     }
                  } // for m_jpeg_ncomp
                  if (0 != m_jpeg_restart_interval)
                  {
                     if (m_restarts_to_go == 0)
                     {
                        m_restarts_to_go = m_jpeg_restart_interval;
                     }
                     m_restarts_to_go--;
                  }
               } // for numxMCU
            } // for numyMCU
         }
         // flush IppiEncodeHuffmanState
         fixed( byte* pData=m_dst.pData, state = m_state.m_buf )
         {
            int currPos = m_dst.currPos;
            status = ipp.jp.ippiEncodeHuffman8x8_JPEG_16s1u_C1(
               null, pData, m_dst.DataLen, &currPos, null, null, null, (IppiEncodeHuffmanState*)state, 1);
            m_dst.currPos = currPos;
         }
         if( IppStatus.ippStsNoErr > status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         jerr = WriteEOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         realSize = m_dst.currPos;
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE WriteImageProgressive(ref int realSize)
      {
         int i;
         JERRCODE jerr;

         jerr = Init();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteSOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_color == JCOLOR.JC_GRAY || m_jpeg_color == JCOLOR.JC_YCBCR )
         {
            jerr = WriteAPP0();
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         if( m_jpeg_color == JCOLOR.JC_RGB  ||
             m_jpeg_color == JCOLOR.JC_CMYK ||
             m_jpeg_color == JCOLOR.JC_YCCK )
         {
            jerr = WriteAPP14();
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteCOM();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteDQT(ref m_qntbl[0]);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_ncomp != 1 &&
             m_jpeg_color != JCOLOR.JC_RGB &&
             m_jpeg_color != JCOLOR.JC_CMYK &&
             m_jpeg_color != JCOLOR.JC_UNKNOWN )
         {
            jerr = WriteDQT(ref m_qntbl[1]);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteSOF2();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = ColorConvert();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = DownSampling();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = PerformDCT();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( 0 != m_jpeg_restart_interval )
         {
            jerr = WriteDRI(m_jpeg_restart_interval);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         for( i = 0; i < m_scan_count; i++ )
         {
            m_next_restart_num = 0;
            m_restarts_to_go = m_jpeg_restart_interval;
            jerr = EncodeScan(
               m_scan_script[i].ncomp,
               m_scan_script[i].id,
               m_scan_script[i].Ss,
               m_scan_script[i].Se,
               m_scan_script[i].Ah,
               m_scan_script[i].Al);
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteEOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         realSize = m_dst.currPos;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE WriteImageLossless(ref int realSize)
      {
         int      i, j;
         JERRCODE jerr;

         jerr = Init();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_ncomp != 1 )
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         m_ss = m_predictor;
         m_se = 0;
         m_ah = 0;
         m_al = m_pt;
         jerr = WriteSOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( m_jpeg_color == JCOLOR.JC_GRAY )
         {
            jerr = WriteAPP0();
            if( JERRCODE.JPEG_OK != jerr )
            {
               return jerr;
            }
         }
         jerr = WriteCOM();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteSOF3();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         jerr = WriteDHT(ref m_dctbl[0]);
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         if( 0 != m_jpeg_restart_interval )
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         jerr = WriteSOS();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         byte*                  pSrc;
         short*                 pMCUBuf;
         short*                 pCurrRow;
         short*                 pPrevRow;
         IppiSize               roi;
         IppStatus              status;
         int[]                  stat = new int[256];
         byte[]                 bits = new byte[16];
         byte[]                 vals = new byte[256];

         roi = new IppiSize(m_src.width, 1);
         fixed (byte* state = m_state.m_buf)
         {
            status = ipp.jp.ippiEncodeHuffmanStateInit_JPEG_8u((IppiEncodeHuffmanState*)state);
         }
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for( i = 0; i < m_numyMCU; i++ )
         {
            pSrc     = m_src.Data8u + i*m_src.lineStep;
            fixed (byte* p = m_ccomp[0].m_top_row,
                         curr = m_ccomp[0].m_curr_row,
                         prev = m_ccomp[0].m_prev_row)
            {
               pMCUBuf = (short*)p;
               pCurrRow = (short*)curr + m_ccomp[0].m_curr_ofs;
               pPrevRow = (short*)prev + m_ccomp[0].m_prev_ofs;

               status = ipp.ip.ippiConvert_8u16s_C1R(pSrc, m_src.lineStep, pCurrRow, m_numxMCU, roi);
               if (IppStatus.ippStsNoErr > status)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
               if (0 != m_al)
               {
                  // do point transform
                  ipp.sp.ippsRShiftC_16s_I(m_al, pCurrRow, m_numxMCU);
               }
               if (i != 0)
               {
                  m_al = 0;
                  m_ss = 1;
                  status = ipp.jp.ippiDiffPredRow_JPEG_16s_C1(pCurrRow, pPrevRow, pMCUBuf, m_src.width, m_ss);
                  if (IppStatus.ippStsNoErr > status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               else
               {
                  m_al = 0;
                  status = ipp.jp.ippiDiffPredFirstRow_JPEG_16s_C1(
                     pCurrRow, pMCUBuf, m_src.width, m_jpeg_precision, m_al);
                  if (IppStatus.ippStsNoErr > status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               byte[] tmp_row = m_ccomp[0].m_curr_row;
               m_ccomp[0].m_curr_row = m_ccomp[0].m_prev_row;
               m_ccomp[0].m_prev_row = tmp_row;
               int tmp_ofs = m_ccomp[0].m_curr_ofs;
               m_ccomp[0].m_curr_ofs = m_ccomp[0].m_prev_ofs;
               m_ccomp[0].m_prev_ofs = tmp_ofs;
               for (j = 0; j < m_numxMCU; j++)
               {
                  fixed (byte* pDCTbl = m_dctbl[m_ccomp[0].m_dc_selector].m_buf,
                               state = m_state.m_buf,
                               pData = m_dst.pData)
                  {
                     int currPos = m_dst.currPos;
                     status = ipp.jp.ippiEncodeHuffmanOne_JPEG_16s1u_C1(
                        pMCUBuf,
                        pData,
                        m_dst.DataLen,
                        &currPos,
                        (IppiEncodeHuffmanSpec*)pDCTbl,
                        (IppiEncodeHuffmanState*)state,
                        0);
                     m_dst.currPos = currPos;
                  }
                  if (IppStatus.ippStsNoErr > status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  pMCUBuf++;
               } // for numxMCU
            }
         }
         // flush IppiEncodeHuffmanState
         fixed (byte* pData = m_dst.pData, state = m_state.m_buf)
         {
            int currPos = m_dst.currPos;
            status = ipp.jp.ippiEncodeHuffmanOne_JPEG_16s1u_C1(
               null, pData, m_dst.DataLen, &currPos, null, (IppiEncodeHuffmanState*)state, 1);
            m_dst.currPos = currPos;
         }
         if( IppStatus.ippStsNoErr > status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         jerr = WriteEOI();
         if( JERRCODE.JPEG_OK != jerr )
         {
            return jerr;
         }
         return JERRCODE.JPEG_OK;
      }

   };
}
