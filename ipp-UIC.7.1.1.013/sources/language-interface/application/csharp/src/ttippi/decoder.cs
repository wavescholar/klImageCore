/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
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
   unsafe class CJPEGDecoderQuantTable
   {
      const int CPU_CACHE_LINE      = 32;
      const int DCTSIZE2            = 64;

      public byte[]   m_rbf;
      public ushort[] m_qbf;

      public int m_id;
      public int m_precision;
      
      public CJPEGDecoderQuantTable()
      {
         m_id          = 0;
         m_precision   = 0;
         m_rbf = new byte[DCTSIZE2+(CPU_CACHE_LINE-1)];
         m_qbf = new ushort[DCTSIZE2+(CPU_CACHE_LINE-1)];
      }

      public JERRCODE Init(int id, byte[] raw)
      {
         IppStatus status;
         m_id        = id & 0x0f;
         m_precision = 0; // 8-bit hardcoded
         m_rbf = raw;
         custom_ipp.ippsCopy_8u(raw, m_rbf, raw.Length);
         status = custom_ipp.ippiQuantInvTableInit_JPEG_8u16u(m_rbf, m_qbf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         return JERRCODE.JPEG_OK;
      }
   };

   unsafe class CJPEGDecoderHuffmanTable
   {
      public byte[] m_buf;
      byte[] m_bits;
      byte[] m_vals;
      bool m_bEmpty;
      bool m_bValid;

      public int m_id;
      public int m_hclass;

      public CJPEGDecoderHuffmanTable()
      {
         m_bits = new byte[16];
         m_vals = new byte[256];
         m_id     = 0;
         m_hclass = 0;
         m_bEmpty = true;
         m_bValid = false;
      }

      ~CJPEGDecoderHuffmanTable()
      {
         Destroy();
      }

      public JERRCODE Create()
      {
         int size;
         IppStatus status;
         status = ipp.jp.ippiDecodeHuffmanSpecGetBufSize_JPEG_8u(&size);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_buf = new byte[size];
         m_bEmpty = false;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE Destroy()
      {
         m_id     = 0;
         m_hclass = 0;
         m_bEmpty = true;
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE Init(int id, int hclass, byte[] bits, byte[] vals)
      {
         IppStatus status;
         m_id     = id     & 0x0f;
         m_hclass = hclass & 0x0f;
         m_bits = bits;
         m_vals = vals;
         status = custom_ipp.ippiDecodeHuffmanSpecInit_JPEG_8u(m_bits, m_vals, m_buf);
         if( IppStatus.ippStsNoErr != status )
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_bValid = true;
         return JERRCODE.JPEG_OK;
      }

      public bool IsEmpty()
      {
         return m_bEmpty;
      }

      public bool IsValid() 
      {
         return m_bValid;
      }

      public void SetInvalid()
      {
         m_bValid = false;
         return;
      }
   };

   unsafe class CJPEGDecoderHuffmanState
   {
      public byte[] m_buf;

      public CJPEGDecoderHuffmanState()
      {
         //m_state = null;
      }

      ~CJPEGDecoderHuffmanState()
      {
         Destroy();
      }

      public JERRCODE Create()
      {
         int size;
         IppStatus status;
         status = ipp.jp.ippiDecodeHuffmanStateGetBufSize_JPEG_8u(&size);
         if (IppStatus.ippStsNoErr != status)
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
         IppStatus status = custom_ipp.ippiDecodeHuffmanStateInit_JPEG_8u(m_buf);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         return JERRCODE.JPEG_OK;
      }
   };

   unsafe public class JPEGDecoder
   {
      // JPEG embedded comments variables
      //int      m_jpeg_comment_detected;
      int m_jpeg_comment_size;
      byte[] m_jpeg_comment;

      // JFIF APP0 related varibales
      int m_jfif_app0_detected;
      int m_jfif_app0_major;
      int m_jfif_app0_minor;
      int m_jfif_app0_units;
      int m_jfif_app0_xDensity;
      int m_jfif_app0_yDensity;
      int m_jfif_app0_thumb_width;
      int m_jfif_app0_thumb_height;

      // JFXX APP0 related variables
      //int m_jfxx_app0_detected;
      int m_jfxx_thumbnails_type;

      // Adobe APP14 related variables
      int m_adobe_app14_detected;
      int m_adobe_app14_version;
      int m_adobe_app14_flags0;
      int m_adobe_app14_flags1;
      int m_adobe_app14_transform;

      JMARKER m_marker;
      BITSTREAM m_src;
      IMAGE m_dst;
      JMODE m_jpeg_mode;
      JSS m_jpeg_sampling;
      JCOLOR m_jpeg_color;
      int m_jpeg_width;
      int m_jpeg_height;
      int m_jpeg_ncomp;
      int m_jpeg_precision;
      int m_jpeg_restart_interval;
      int m_restarts_to_go;
      int m_next_restart_num;
      int m_sos_len;
      int m_curr_comp_no;
      int m_ss;
      int m_se;
      int m_al;
      int m_ah;
      int m_numxMCU;
      int m_numyMCU;
      int m_mcuWidth;
      int m_mcuHeight;
      int m_ccWidth;
      int m_ccHeight;
      int m_xPadding;
      int m_yPadding;
      short[] m_block_buffer;
      int m_nblock;
      short[] m_coefbuf;
      int m_scan_count;
      int m_ac_scans_completed;
      int m_dc_scan_completed;

      public JPEGDecoder()
      {
         int i;
         for (i = 0; i < MAX_QUANT_TABLES; i++)
         {
            m_qntbl[i] = new CJPEGDecoderQuantTable();
         }
         for (i = 0; i < MAX_HUFF_TABLES; i++)
         {
            m_dctbl[i] = new CJPEGDecoderHuffmanTable();
            m_actbl[i] = new CJPEGDecoderHuffmanTable();
         }
      }

      public JERRCODE SetSource(ref byte[] pSrc, int srcSize)
      {
         m_src.pData = pSrc;
         m_src.DataLen = srcSize;
         m_src.currPos = 0;
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE SetDestination(
         byte* pDst,
         int dstStep,
         IppiSize dstSize,
         int dstChannels,
         JCOLOR dstColor,
         int dstPrecision)
      {
         m_dst.Data8u = pDst;
         m_dst.lineStep = dstStep;
         m_dst.width = dstSize.width;
         m_dst.height = dstSize.height;
         m_dst.nChannels = dstChannels;
         m_dst.color = dstColor;
         m_dst.precision = dstPrecision;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE NextMarker(ref JMARKER marker)
      {
         int c = 0;
         int n = 0;
         for (; ; )
         {
            if (m_src.currPos >= m_src.DataLen)
            {
               return JERRCODE.JPEG_BUFF_TOO_SMALL;
            }
            m_src._READ_BYTE(ref c);
            if (c != 0xff)
            {
               do
               {
                  if (m_src.currPos >= m_src.DataLen)
                  {
                     return JERRCODE.JPEG_BUFF_TOO_SMALL;
                  }
                  n++;
                  m_src._READ_BYTE(ref c);
               } while (c != 0xff);
            }
            do
            {
               if (m_src.currPos >= m_src.DataLen)
               {
                  return JERRCODE.JPEG_BUFF_TOO_SMALL;
               }

               m_src._READ_BYTE(ref c);
            } while (c == 0xff);
            if (c != 0)
            {
               marker = (JMARKER)c;
               break;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE SkipMarker()
      {
         int len = 0;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         m_src.currPos += len - 2;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      const int APP0_JFIF_LENGTH = 14;
      const int APP0_JFXX_LENGTH = 6;
      const int CPU_CACHE_LINE = 32;
      const int DCTSIZE2 = 64;
      const int MAX_QUANT_TABLES = 4;
      const int MAX_HUFF_TABLES = 4;
      const int MAX_COMPS_PER_SCAN = 4;
      const int MAX_COMPS_PER_FRAME = 255;
      const int MAX_HUFF_BITS = 16;
      const int MAX_HUFF_VALS = 256;
      const int MAX_BLOCKS_PER_MCU = 10;
      int MAX_BYTES_PER_MCU = DCTSIZE2 * sizeof(short) * MAX_BLOCKS_PER_MCU;

      CJPEGColorComponent[] m_ccomp = new CJPEGColorComponent[MAX_COMPS_PER_SCAN];
      CJPEGDecoderQuantTable[] m_qntbl = new CJPEGDecoderQuantTable[MAX_QUANT_TABLES];
      CJPEGDecoderHuffmanTable[] m_dctbl = new CJPEGDecoderHuffmanTable[MAX_HUFF_TABLES];
      CJPEGDecoderHuffmanTable[] m_actbl = new CJPEGDecoderHuffmanTable[MAX_HUFF_TABLES];
      CJPEGDecoderHuffmanState m_state = new CJPEGDecoderHuffmanState();

      JERRCODE _set_sampling()
      {
         switch (m_jpeg_ncomp)
         {
            case 1:
               if (m_ccomp[0].m_hsampling == 1 && m_ccomp[0].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_444;
               }
               else
               {
                  return JERRCODE.JPEG_BAD_SAMPLING;
               }
               break;
            case 3:
               if (m_ccomp[0].m_hsampling == 1 && m_ccomp[0].m_vsampling == 1 &&
                   m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                   m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_444;
               }
               else if (m_ccomp[0].m_hsampling == 2 && m_ccomp[0].m_vsampling == 1 &&
                        m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                        m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_422;
               }
               else if (m_ccomp[0].m_hsampling == 2 && m_ccomp[0].m_vsampling == 2 &&
                        m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                        m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_411;
               }
               else
               {
                  m_jpeg_sampling = JSS.JS_OTHER;
               }
               break;
            case 4:
               if (m_ccomp[0].m_hsampling == 1 && m_ccomp[0].m_vsampling == 1 &&
                   m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                   m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1 &&
                   m_ccomp[3].m_hsampling == 1 && m_ccomp[3].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_444;
               }
               else if (m_ccomp[0].m_hsampling == 2 && m_ccomp[0].m_vsampling == 1 &&
                        m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                        m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1 &&
                        m_ccomp[3].m_hsampling == 2 && m_ccomp[3].m_vsampling == 1)
               {
                  m_jpeg_sampling = JSS.JS_422;
               }
               else if (m_ccomp[0].m_hsampling == 2 && m_ccomp[0].m_vsampling == 2 &&
                        m_ccomp[1].m_hsampling == 1 && m_ccomp[1].m_vsampling == 1 &&
                        m_ccomp[2].m_hsampling == 1 && m_ccomp[2].m_vsampling == 1 &&
                        m_ccomp[3].m_hsampling == 2 && m_ccomp[3].m_vsampling == 2)
               {
                  m_jpeg_sampling = JSS.JS_411;
               }
               else
               {
                  m_jpeg_sampling = JSS.JS_OTHER;
               }
               break;
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseAPP0()
      {
         int len = 0;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         if (len >= APP0_JFIF_LENGTH &&
            m_src.pData[m_src.currPos + 0] == 0x4a &&
            m_src.pData[m_src.currPos + 1] == 0x46 &&
            m_src.pData[m_src.currPos + 2] == 0x49 &&
            m_src.pData[m_src.currPos + 3] == 0x46 &&
            m_src.pData[m_src.currPos + 4] == 0)
         {
            // we've found JFIF APP0 marker
            len -= 5;
            m_src.currPos += 5;
            m_jfif_app0_detected = 1;

            m_src._READ_BYTE(ref m_jfif_app0_major);
            m_src._READ_BYTE(ref m_jfif_app0_minor);
            m_src._READ_BYTE(ref m_jfif_app0_units);
            m_src._READ_WORD(ref m_jfif_app0_xDensity);
            m_src._READ_WORD(ref m_jfif_app0_yDensity);
            m_src._READ_BYTE(ref m_jfif_app0_thumb_width);
            m_src._READ_BYTE(ref m_jfif_app0_thumb_height);
            len -= 9;
         }
         if (len >= APP0_JFXX_LENGTH &&
            m_src.pData[m_src.currPos + 0] == 0x4a &&
            m_src.pData[m_src.currPos + 1] == 0x46 &&
            m_src.pData[m_src.currPos + 2] == 0x58 &&
            m_src.pData[m_src.currPos + 3] == 0x58 &&
            m_src.pData[m_src.currPos + 4] == 0)
         {
            // we've found JFXX APP0 extension marker
            len -= 5;
            m_src.currPos += 5;
            //m_jfxx_app0_detected = 1;
            m_src._READ_BYTE(ref m_jfxx_thumbnails_type);
            switch (m_jfxx_thumbnails_type)
            {
               case 0x10: break;
               case 0x11: break;
               case 0x13: break;
               default: break;
            }
            len -= 1;
         }
         m_src.currPos += len;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      const int APP14_ADOBE_LENGTH = 12;

      JERRCODE ParseAPP14()
      {
         int len = 0;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         if (len >= APP14_ADOBE_LENGTH &&
            m_src.pData[m_src.currPos + 0] == 0x41 &&
            m_src.pData[m_src.currPos + 1] == 0x64 &&
            m_src.pData[m_src.currPos + 2] == 0x6f &&
            m_src.pData[m_src.currPos + 3] == 0x62 &&
            m_src.pData[m_src.currPos + 4] == 0x65)
         {
            // we've found Adobe APP14 marker
            len -= 5;
            m_src.currPos += 5;
            m_adobe_app14_detected = 1;
            m_src._READ_WORD(ref m_adobe_app14_version);
            m_src._READ_WORD(ref m_adobe_app14_flags0);
            m_src._READ_WORD(ref m_adobe_app14_flags1);
            m_src._READ_BYTE(ref m_adobe_app14_transform);
            len -= 7;
         }
         m_src.currPos += len;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseCOM()
      {
         int i;
         int c = 0;
         int len = 0;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         //m_jpeg_comment_detected = 1;
         m_jpeg_comment_size = len;
         m_jpeg_comment = new byte[len + 1];
         for (i = 0; i < len; i++)
         {
            m_src._READ_BYTE(ref c);
            m_jpeg_comment[i] = (byte)c;
         }
         m_jpeg_comment[len] = 0;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseDQT()
      {
         int id = 0;
         int len = 0;
         JERRCODE jerr;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         while (len > 0)
         {
            m_src._READ_BYTE(ref id);
            int precision = (id & 0xf0) >> 4;
            if ((id & 0x0f) > MAX_QUANT_TABLES)
            {
               return JERRCODE.JPEG_BAD_QUANT_SEGMENT;
            }
            int q = 0;
            byte[] qnt = new byte[DCTSIZE2];
            for (int i = 0; i < DCTSIZE2; i++)
            {
               if (precision != 0)
               {
                  m_src._READ_WORD(ref q);
               }
               else
               {
                  m_src._READ_BYTE(ref q);
               }
               qnt[i] = (byte)q;
            }
            jerr = m_qntbl[id & 0x0f].Init(id, qnt);
            if (JERRCODE.JPEG_OK != jerr)
            {
               return jerr;
            }
            len -= DCTSIZE2 + DCTSIZE2 * precision + 1;
         }
         if (len != 0)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseDHT()
      {
         int i;
         int len = 0;
         int index = 0;
         int v = 0;
         int count;
         JERRCODE jerr;

         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         byte[] bits = new byte[MAX_HUFF_BITS];
         byte[] vals = new byte[MAX_HUFF_VALS];
         while (len > 16)
         {
            m_src._READ_BYTE(ref index);
            count = 0;
            for (i = 0; i < MAX_HUFF_BITS; i++)
            {
               m_src._READ_BYTE(ref v);
               bits[i] = (byte)v;
               count += bits[i];
            }
            len -= 16 + 1;
            if (count > MAX_HUFF_VALS || count > len)
            {
               return JERRCODE.JPEG_BAD_HUFF_TBL;
            }
            for (i = 0; i < count; i++)
            {
               m_src._READ_BYTE(ref v);
               vals[i] = (byte)v;
            }
            len -= count;
            if ((index >> 4) != 0)
            {
               // make AC Huffman table
               if (m_actbl[index & 0x0f].IsEmpty())
               {
                  jerr = m_actbl[index & 0x0f].Create();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               jerr = m_actbl[index & 0x0f].Init(index & 0x0f, index >> 4, bits, vals);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
            else
            {
               // make DC Huffman table
               if (m_dctbl[index & 0x0f].IsEmpty())
               {
                  jerr = m_dctbl[index & 0x0f].Create();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
               jerr = m_dctbl[index & 0x0f].Init(index & 0x0f, index >> 4, bits, vals);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
         }
         if (len != 0)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseSOF0()
      {
         int i;
         int len = 0;
         JERRCODE jerr;

         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         m_src._READ_BYTE(ref m_jpeg_precision);
         if (m_jpeg_precision != 8)
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         m_src._READ_WORD(ref m_jpeg_height);
         m_src._READ_WORD(ref m_jpeg_width);
         m_src._READ_BYTE(ref m_jpeg_ncomp);
         if (m_jpeg_ncomp < 0 || m_jpeg_ncomp > MAX_COMPS_PER_SCAN)
         {
            return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
         }
         len -= 6;
         if (len != m_jpeg_ncomp * 3)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_src._READ_BYTE(ref m_ccomp[i].m_id);
            int ss = 0;
            m_src._READ_BYTE(ref ss);
            m_ccomp[i].m_hsampling = (ss >> 4) & 0x0f;
            m_ccomp[i].m_vsampling = (ss) & 0x0f;
            m_src._READ_BYTE(ref m_ccomp[i].m_q_selector);
            if (m_ccomp[i].m_hsampling <= 0 || m_ccomp[i].m_vsampling <= 0)
            {
               return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
            }
         }
         jerr = _set_sampling();
         if (JERRCODE.JPEG_OK != jerr)
         {
            return jerr;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_ccomp[i].m_h_factor = (m_jpeg_sampling == JSS.JS_444) ? 1 : (i == 0 || i == 3 ? 1 : 2);
            m_ccomp[i].m_v_factor = (m_jpeg_sampling == JSS.JS_411) ? (i == 0 || i == 3 ? 1 : 2) : 1;
         }
         m_jpeg_mode = JMODE.JPEG_BASELINE;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseSOF2()
      {
         int i;
         int len = 0;
         JERRCODE jerr;

         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         m_src._READ_BYTE(ref m_jpeg_precision);
         if (m_jpeg_precision != 8)
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         m_src._READ_WORD(ref m_jpeg_height);
         m_src._READ_WORD(ref m_jpeg_width);
         m_src._READ_BYTE(ref m_jpeg_ncomp);
         if (m_jpeg_ncomp < 0 || m_jpeg_ncomp > MAX_COMPS_PER_SCAN)
         {
            return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
         }
         len -= 6;
         if (len != m_jpeg_ncomp * 3)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_src._READ_BYTE(ref m_ccomp[i].m_id);
            m_ccomp[i].m_comp_no = i;
            int ss = 0;
            m_src._READ_BYTE(ref ss);
            m_ccomp[i].m_hsampling = (ss >> 4) & 0x0f;
            m_ccomp[i].m_vsampling = (ss) & 0x0f;
            m_src._READ_BYTE(ref m_ccomp[i].m_q_selector);
            if (m_ccomp[i].m_hsampling <= 0 || m_ccomp[i].m_vsampling <= 0)
            {
               return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
            }
         }
         jerr = _set_sampling();
         if (JERRCODE.JPEG_OK != jerr)
         {
            return jerr;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_ccomp[i].m_h_factor = (m_jpeg_sampling == JSS.JS_444) ? 1 : (i == 0 || i == 3 ? 1 : 2);
            m_ccomp[i].m_v_factor = (m_jpeg_sampling == JSS.JS_411) ? (i == 0 || i == 3 ? 1 : 2) : 1;
         }
         m_jpeg_mode = JMODE.JPEG_PROGRESSIVE;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseSOF3()
      {
         int i;
         int len = 0;
         JERRCODE jerr;

         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         m_src._READ_BYTE(ref m_jpeg_precision);
         if (m_jpeg_precision < 2 || m_jpeg_precision > 16)
         {
            return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
         }
         m_src._READ_WORD(ref m_jpeg_height);
         m_src._READ_WORD(ref m_jpeg_width);
         m_src._READ_BYTE(ref m_jpeg_ncomp);
         if (m_jpeg_ncomp != 1)
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         len -= 6;
         if (len != m_jpeg_ncomp * 3)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_src._READ_BYTE(ref m_ccomp[i].m_id);
            int ss = 0;
            m_src._READ_BYTE(ref ss);
            m_ccomp[i].m_hsampling = (ss >> 4) & 0x0f;
            m_ccomp[i].m_vsampling = (ss) & 0x0f;
            m_src._READ_BYTE(ref m_ccomp[i].m_q_selector);
            if (m_ccomp[i].m_hsampling <= 0 || m_ccomp[i].m_vsampling <= 0)
            {
               return JERRCODE.JPEG_BAD_FRAME_SEGMENT;
            }
         }
         jerr = _set_sampling();
         if (JERRCODE.JPEG_OK != jerr)
         {
            return jerr;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            m_ccomp[i].m_h_factor = (m_jpeg_sampling == JSS.JS_444) ? 1 : (i == 0 || i == 3 ? 1 : 2);
            m_ccomp[i].m_v_factor = (m_jpeg_sampling == JSS.JS_411) ? (i == 0 || i == 3 ? 1 : 2) : 1;
         }
         m_jpeg_mode = JMODE.JPEG_LOSSLESS;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseDRI()
      {
         int len = 0;
         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         len -= 2;
         if (len != 2)
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         m_src._READ_WORD(ref m_jpeg_restart_interval);
         m_restarts_to_go = m_jpeg_restart_interval;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseRST()
      {
         JERRCODE jerr;
         if ((int)m_marker == 0xff)
         {
            m_src.currPos--;
            m_marker = JMARKER.JM_NONE;
         }
         if (JMARKER.JM_NONE == m_marker)
         {
            jerr = NextMarker(ref m_marker);
            if (JERRCODE.JPEG_OK != jerr)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         if ((int)m_marker == ((int)JMARKER.JM_RST0 + m_next_restart_num))
         {
            m_marker = JMARKER.JM_NONE;
         }
         else
         {
            m_marker = JMARKER.JM_NONE;
         }
         // Update next-restart state
         m_next_restart_num = (m_next_restart_num + 1) & 7;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseSOS()
      {
         int i;
         int ci;
         int len = 0;

         if (m_src.currPos + 2 >= m_src.DataLen)
         {
            return JERRCODE.JPEG_BUFF_TOO_SMALL;
         }
         m_src._READ_WORD(ref len);
         // store position to return to in subsequent ReadData call
         m_sos_len = len;
         len -= 2;
         int ncomps = 0;
         m_src._READ_BYTE(ref ncomps);
         if (ncomps < 1 || ncomps > MAX_COMPS_PER_SCAN)
         {
            return JERRCODE.JPEG_BAD_SCAN_SEGMENT;
         }
         if (JMODE.JPEG_PROGRESSIVE != m_jpeg_mode && ncomps < m_jpeg_ncomp)
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         if (len != ((ncomps * 2) + 4))
         {
            return JERRCODE.JPEG_BAD_SEGMENT_LENGTH;
         }
         for (i = 0; i < ncomps; i++)
         {
            int id = 0;
            int huff_sel = 0;
            m_src._READ_BYTE(ref id);
            m_src._READ_BYTE(ref huff_sel);
            for (ci = 0; ci < m_jpeg_ncomp; ci++)
            {
               if (id == m_ccomp[ci].m_id)
               {
                  m_curr_comp_no = ci;
                  goto comp_id_match;
               }
            }
            return JERRCODE.JPEG_BAD_COMPONENT_ID;

         comp_id_match:
            m_ccomp[ci].m_dc_selector = (huff_sel >> 4) & 0x0f;
            m_ccomp[ci].m_ac_selector = (huff_sel) & 0x0f;
         }
         m_src._READ_BYTE(ref m_ss);
         m_src._READ_BYTE(ref m_se);
         int t = 0;
         m_src._READ_BYTE(ref t);
         m_ah = (t >> 4) & 0x0f;
         m_al = (t) & 0x0f;
         // detect JPEG color space
         if (m_jfif_app0_detected != 0)
         {
            switch (m_jpeg_ncomp)
            {
               case 1: m_jpeg_color = JCOLOR.JC_GRAY; break;
               case 3: m_jpeg_color = JCOLOR.JC_YCBCR; break;
               default: m_jpeg_color = JCOLOR.JC_UNKNOWN; break;
            }
         }
         if (m_adobe_app14_detected != 0)
         {
            switch (m_adobe_app14_transform)
            {
               case 0:
                  switch (m_jpeg_ncomp)
                  {
                     case 1: m_jpeg_color = JCOLOR.JC_GRAY; break;
                     case 3: m_jpeg_color = JCOLOR.JC_RGB; break;
                     case 4: m_jpeg_color = JCOLOR.JC_CMYK; break;
                     default: m_jpeg_color = JCOLOR.JC_UNKNOWN; break;
                  }
                  break;
               case 1: m_jpeg_color = JCOLOR.JC_YCBCR; break;
               case 2: m_jpeg_color = JCOLOR.JC_YCCK; break;
               default: m_jpeg_color = JCOLOR.JC_UNKNOWN; break;
            }
         }
         // try to guess what color space is used...
         if ((0 == m_jfif_app0_detected) && (0 == m_adobe_app14_detected))
         {
            switch (m_jpeg_ncomp)
            {
               case 1: m_jpeg_color = JCOLOR.JC_GRAY; break;
               case 3: m_jpeg_color = JCOLOR.JC_YCBCR; break;
               default: m_jpeg_color = JCOLOR.JC_UNKNOWN; break;
            }
         }
         m_restarts_to_go = m_jpeg_restart_interval;
         m_next_restart_num = 0;
         m_marker = JMARKER.JM_NONE;
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ParseJPEGBitStream(JOPERATION op)
      {
         JERRCODE jerr = JERRCODE.JPEG_OK;
         m_marker = JMARKER.JM_NONE;
         for (; ; )
         {
            if (JMARKER.JM_NONE == m_marker)
            {
               jerr = NextMarker(ref m_marker);
               if (JERRCODE.JPEG_OK != jerr)
               {
                  return jerr;
               }
            }
            switch (m_marker)
            {
               case JMARKER.JM_SOI:
                  m_marker = JMARKER.JM_NONE;
                  break;
               case JMARKER.JM_APP0:
                  jerr = ParseAPP0();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_APP14:
                  jerr = ParseAPP14();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_COM:
                  jerr = ParseCOM();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_DQT:
                  jerr = ParseDQT();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_SOF0:
               case JMARKER.JM_SOF1:
                  jerr = ParseSOF0();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_SOF2:
                  jerr = ParseSOF2();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_SOF3:
                  jerr = ParseSOF3();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_SOF5:
               case JMARKER.JM_SOF6:
               case JMARKER.JM_SOF7:
               case JMARKER.JM_SOF9:
               case JMARKER.JM_SOFA:
               case JMARKER.JM_SOFB:
               case JMARKER.JM_SOFD:
               case JMARKER.JM_SOFE:
               case JMARKER.JM_SOFF:
                  return JERRCODE.JPEG_NOT_IMPLEMENTED;
               case JMARKER.JM_DHT:
                  jerr = ParseDHT();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_DRI:
                  jerr = ParseDRI();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_SOS:
                  jerr = ParseSOS();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  if (JOPERATION.JO_READ_HEADER == op)
                  {
                     m_src.currPos -= m_sos_len + 2;
                     // stop here, when we are reading header
                     return JERRCODE.JPEG_OK;
                  }
                  if (JOPERATION.JO_READ_DATA == op)
                  {
                     jerr = Init();
                     if (JERRCODE.JPEG_OK != jerr)
                     {
                        return jerr;
                     }
                     switch (m_jpeg_mode)
                     {
                        case JMODE.JPEG_BASELINE:
                           jerr = DecodeScanBaseline();
                           break;
                        case JMODE.JPEG_PROGRESSIVE:
                           {
                              jerr = DecodeScanProgressive();
                              m_ac_scans_completed = 0;
                              for (int i = 0; i < m_jpeg_ncomp; i++)
                              {
                                 m_ac_scans_completed += m_ccomp[i].m_ac_scan_completed;
                              }
                              if (JERRCODE.JPEG_OK != jerr ||
                                 (m_dc_scan_completed != 0 && m_ac_scans_completed == m_jpeg_ncomp))
                              {
                                 jerr = PerformDCT();
                                 if (JERRCODE.JPEG_OK != jerr)
                                 {
                                    return jerr;
                                 }
                                 jerr = UpSampling();
                                 if (JERRCODE.JPEG_OK != jerr)
                                 {
                                    return jerr;
                                 }
                                 jerr = ColorConvert();
                                 if (JERRCODE.JPEG_OK != jerr)
                                 {
                                    return jerr;
                                 }
                              }
                              break;
                           }
                        case JMODE.JPEG_LOSSLESS:
                           jerr = DecodeScanLossless();
                           break;
                     }
                     if (JERRCODE.JPEG_OK != jerr)
                        return jerr;
                  }
                  break;
               case JMARKER.JM_RST0:
               case JMARKER.JM_RST1:
               case JMARKER.JM_RST2:
               case JMARKER.JM_RST3:
               case JMARKER.JM_RST4:
               case JMARKER.JM_RST5:
               case JMARKER.JM_RST6:
               case JMARKER.JM_RST7:
                  jerr = ParseRST();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
                  break;
               case JMARKER.JM_EOI:
                  m_marker = JMARKER.JM_NONE;
                  jerr = JERRCODE.JPEG_OK;
                  goto Exit;
               default:
                  jerr = SkipMarker();
                  if (JERRCODE.JPEG_OK != jerr)
                     return jerr;
                  break;
            }
         }
      Exit:
         return jerr;
      }

      JERRCODE Init()
      {
         int i;
         int sz;
         int ss_buf_size = 0;
         int cc_buf_size = 0;
         m_nblock = 1;

         // not implemented yet
         if (m_jpeg_sampling == JSS.JS_OTHER)
         {
            return JERRCODE.JPEG_NOT_IMPLEMENTED;
         }
         for (i = 0; i < m_jpeg_ncomp; i++)
         {
            switch (m_jpeg_mode)
            {
               case JMODE.JPEG_BASELINE:
                  {
                     switch (m_jpeg_sampling)
                     {
                        case JSS.JS_444:
                           {
                              ss_buf_size = 0;
                              m_nblock = m_jpeg_ncomp;
                              break;
                           }
                        case JSS.JS_422:
                           {
                              if (i == 0 || i == 3)
                                 ss_buf_size = 0;
                              else
                                 ss_buf_size = m_numxMCU * ((m_mcuWidth >> 1) + 2) * m_mcuHeight;
                              m_nblock = (m_jpeg_ncomp == 3) ? 4 : 6;
                              break;
                           }
                        case JSS.JS_411:
                           {
                              if (i == 0 || i == 3)
                                 ss_buf_size = 0;
                              else
                                 ss_buf_size = m_numxMCU * ((m_mcuWidth >> 1) + 2) * ((m_mcuHeight >> 1) + 2);
                              m_nblock = (m_jpeg_ncomp == 3) ? 6 : 10;
                              break;
                           }
                     }
                     cc_buf_size = m_numxMCU * m_mcuWidth * m_mcuHeight;
                     if (null == m_block_buffer)
                        m_block_buffer = new short[DCTSIZE2 * m_nblock * m_numxMCU];
                     break;
                  } // JPEG_BASELINE
               case JMODE.JPEG_PROGRESSIVE:
                  {
                     ss_buf_size = (m_numxMCU * m_mcuWidth + 2) * (m_numyMCU * m_mcuHeight + 2);
                     cc_buf_size = (m_numxMCU * m_mcuWidth) * (m_numyMCU * m_mcuHeight);
                     if (null == m_coefbuf)
                     {
                        sz = m_numxMCU * m_numyMCU * MAX_BYTES_PER_MCU;
                        m_coefbuf = new short[sz];
                     }
                     break;
                  } // JPEG_PROGRESSIVE
               case JMODE.JPEG_LOSSLESS:
                  {
                     ss_buf_size = m_numxMCU * m_mcuWidth * sizeof(short);
                     cc_buf_size = m_numxMCU * m_mcuWidth * sizeof(short);
                     break;
                  } // JPEG_LOSSLESS
            }
            m_ccomp[i].m_ss_buffer = new byte[ss_buf_size * 2];
            if (0 != ss_buf_size)
            {
               m_ccomp[i].m_curr_row = m_ccomp[i].m_ss_buffer;
               m_ccomp[i].m_curr_ofs = 0;
               m_ccomp[i].m_prev_row = m_ccomp[i].m_ss_buffer;
               m_ccomp[i].m_prev_ofs = m_dst.width;
            }
            m_ccomp[i].m_cc_buffer = new byte[cc_buf_size];
            if (m_jpeg_sampling == JSS.JS_411)
            {
               m_ccomp[i].m_top_row = new byte[(m_ccWidth >> 1) + 2];
            }
         } // m_jpeg_ncomp
         m_state.Create();
         return JERRCODE.JPEG_OK;
      }


      public JERRCODE ReadHeader(ref int width, ref int height, ref int nChannels,
         ref int precision, ref JCOLOR color, ref JSS sampling)
      {
         JERRCODE jerr = ParseJPEGBitStream(JOPERATION.JO_READ_HEADER);
         if (JERRCODE.JPEG_OK != jerr)
         {
            return jerr;
         }
         if (JMODE.JPEG_LOSSLESS == m_jpeg_mode)
         {
            m_mcuWidth = 1;
            m_mcuHeight = 1;
         }
         else
         {
            m_mcuWidth = (m_jpeg_sampling == JSS.JS_444) ? 8 : 16;
            m_mcuHeight = (m_jpeg_sampling == JSS.JS_411) ? 16 : 8;
         }
         m_numxMCU = (m_jpeg_width + (m_mcuWidth - 1)) / m_mcuWidth;
         m_numyMCU = (m_jpeg_height + (m_mcuHeight - 1)) / m_mcuHeight;
         m_xPadding = m_numxMCU * m_mcuWidth - m_jpeg_width;
         m_yPadding = m_numyMCU * m_mcuHeight - m_jpeg_height;
         m_ccWidth = m_mcuWidth * m_numxMCU;
         m_ccHeight = (JMODE.JPEG_PROGRESSIVE == m_jpeg_mode) ? m_mcuHeight * m_numyMCU : m_mcuHeight;

         width = m_jpeg_width;
         height = m_jpeg_height;
         nChannels = m_jpeg_ncomp;
         precision = m_jpeg_precision;
         color = m_jpeg_color;
         sampling = m_jpeg_sampling;
         return JERRCODE.JPEG_OK;
      }

      public JERRCODE ReadData()
      {
         return ParseJPEGBitStream(JOPERATION.JO_READ_DATA);
      }

      unsafe JERRCODE DecodeHuffmanMCURowBL(short* pMCUBuf)
      {
         int j, n, k, l;
         int srcLen;
         JERRCODE jerr;
         IppStatus status;

         srcLen = m_src.DataLen;
         for (j = 0; j < m_numxMCU; j++)
         {
            if (0 != m_jpeg_restart_interval)
            {
               if (m_restarts_to_go == 0)
               {
                  jerr = ProcessRestart();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
               }
            }
            for (n = 0; n < m_jpeg_ncomp; n++)
            {
               for (k = 0; k < m_ccomp[n].m_vsampling; k++)
               {
                  for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                  {
                     int currPos = m_src.currPos;
                     short lastDC = m_ccomp[n].m_lastDC;
                     int marker = (int)m_marker;
                     status = custom_ipp.ippiDecodeHuffman8x8_JPEG_1u16s_C1(
                        m_src.pData, srcLen, &currPos,
                        pMCUBuf, &lastDC, &marker,
                        m_dctbl[m_ccomp[n].m_dc_selector].m_buf,
                        m_actbl[m_ccomp[n].m_ac_selector].m_buf,
                        m_state.m_buf);
                     if (IppStatus.ippStsNoErr > status)
                     {
                        return JERRCODE.JPEG_INTERNAL_ERROR;
                     }
                     m_src.currPos = currPos;
                     m_ccomp[n].m_lastDC = lastDC;
                     m_marker = (JMARKER)marker;
                     pMCUBuf += DCTSIZE2;
                  } // for m_hsampling
               } // for m_vsampling
            } // for m_jpeg_ncomp
            m_restarts_to_go--;
         } // for m_numxMCU
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DecodeHuffmanMCURowLS(short* pMCUBuf)
      {
         int j, n, k, l;
         int srcLen;
         JERRCODE jerr;
         IppStatus status;

         srcLen = m_src.DataLen;
         for (j = 0; j < m_numxMCU; j++)
         {
            if (0 != m_jpeg_restart_interval)
            {
               if (m_restarts_to_go == 0)
               {
                  jerr = ProcessRestart();
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
               }
            }
            for (n = 0; n < m_jpeg_ncomp; n++)
            {
               for (k = 0; k < m_ccomp[n].m_vsampling; k++)
               {
                  for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                  {
                     int currPos = m_src.currPos;
                     int marker = (int)m_marker;
                     status = custom_ipp.ippiDecodeHuffmanOne_JPEG_1u16s_C1(
                        m_src.pData,
                        srcLen,
                        &currPos,
                        pMCUBuf,
                        &marker,
                        m_dctbl[m_ccomp[n].m_dc_selector].m_buf,
                        m_state.m_buf);
                     if (IppStatus.ippStsNoErr > status)
                     {
                        return JERRCODE.JPEG_INTERNAL_ERROR;
                     }
                     m_src.currPos = currPos;
                     m_marker = (JMARKER)marker;
                     pMCUBuf++;
                  } // for m_hsampling
               } // for m_vsampling
            } // for m_jpeg_ncomp
            m_restarts_to_go--;
         } // for m_numxMCU
         return JERRCODE.JPEG_OK;
      }

      JERRCODE UpSampling(int nMCURow)
      {
         int i, k;
         IppStatus status;
         int STEP_SS = ((m_ccWidth >> 1) + 2);

         for (k = 0; k < m_jpeg_ncomp; k++)
         {
            // sampling 444
            if (m_ccomp[k].m_h_factor == 1 && m_ccomp[k].m_v_factor == 1)
            {
               int s = m_ccomp[k].m_ss_buffer.Length;
               if (s != 0)
               {
                  custom_ipp.ippsCopy_8u(m_ccomp[k].m_ss_buffer, m_ccomp[k].m_cc_buffer, m_ccWidth * m_mcuHeight);
               }
            }
            // sampling 422
            if (m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 1)
            {
               // pad most left and most right column
               fixed (byte* ss_buf = m_ccomp[k].m_ss_buffer, cc_buf = m_ccomp[k].m_cc_buffer)
               {
                  byte* p1 = ss_buf;
                  byte* p2 = ss_buf + (m_ccWidth >> 1) + 1;
                  int step = STEP_SS;
                  for (i = 0; i < m_ccHeight; i++)
                  {
                     p1[0] = p1[1];
                     p2[0] = p2[-1];
                     p1 += step;
                     p2 += step;
                  }
                  IppiSize roiSrc = new IppiSize(m_ccWidth >> 1, m_mcuHeight);
                  IppiSize roiDst = new IppiSize(m_ccWidth, m_mcuHeight);
                  byte* src = ss_buf + 1;
                  byte* dst = cc_buf;
                  status = ipp.jp.ippiSampleUpH2V1_JPEG_8u_C1R(src, STEP_SS, roiSrc, dst, m_ccWidth, roiDst);
               }
               if (IppStatus.ippStsNoErr != status)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }
            // sampling 411
            if (m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 2)
            {
               // pad most left and most right columns
               fixed (byte* ss_buf = m_ccomp[k].m_ss_buffer, cc_buf = m_ccomp[k].m_cc_buffer)
               {
                  byte* p1 = ss_buf;
                  byte* p2 = ss_buf + (m_ccWidth >> 1) + 1;
                  int step = STEP_SS;
                  for (i = 0; i < (m_ccHeight >> 1); i++)
                  {
                     p1[0] = p1[1];
                     p2[0] = p2[-1];
                     p1 += step;
                     p2 += step;
                  }
                  IppiSize roiSrc = new IppiSize(m_ccWidth >> 1, m_mcuHeight >> 1);
                  IppiSize roiDst = new IppiSize(m_ccWidth, m_mcuHeight);
                  byte* src = ss_buf + STEP_SS + 1;
                  byte* dst = cc_buf;
                  if (nMCURow == 0)
                  {
                     p1 = ss_buf + STEP_SS;
                     p2 = ss_buf;
                     ipp.sp.ippsCopy_8u(p1, p2, STEP_SS);
                     p1 = ss_buf + STEP_SS * (8 + 0);
                     p2 = ss_buf + STEP_SS * (8 + 1);
                     ipp.sp.ippsCopy_8u(p1, p2, STEP_SS);
                     fixed (byte* p = m_ccomp[k].m_top_row)
                     {
                        p1 = p;
                        ipp.sp.ippsCopy_8u(p2, p1, STEP_SS);
                     }
                  }
                  else
                  {
                     fixed (byte* p = m_ccomp[k].m_top_row)
                     {
                        p1 = p;
                        p2 = ss_buf;
                        ipp.sp.ippsCopy_8u(p1, p2, STEP_SS);
                        p2 = ss_buf + STEP_SS * (8 + 0);
                        ipp.sp.ippsCopy_8u(p2, p1, STEP_SS);
                        p1 = ss_buf + STEP_SS * (8 + 0);
                        p2 = ss_buf + STEP_SS * (8 + 1);
                        ipp.sp.ippsCopy_8u(p1, p2, STEP_SS);
                     }
                  }
                  status = ipp.jp.ippiSampleUpH2V2_JPEG_8u_C1R(src, STEP_SS, roiSrc, dst, m_ccWidth, roiDst);
               }
               if (IppStatus.ippStsNoErr != status)
               {
                  return JERRCODE.JPEG_INTERNAL_ERROR;
               }
            }//411
         } // for m_jpeg_ncomp
         return JERRCODE.JPEG_OK;
      }

      JERRCODE UpSampling()
      {
         int i, k;
         IppStatus status;

         for (k = 0; k < m_jpeg_ncomp; k++)
         {
            // sampling 444
            if (m_ccomp[k].m_h_factor == 1 && m_ccomp[k].m_v_factor == 1)
            {
               IppiSize roi = new IppiSize(m_dst.width, m_dst.height);
               fixed (byte* src = m_ccomp[k].m_ss_buffer, dst = m_ccomp[k].m_cc_buffer)
                  ipp.ip.ippiCopy_8u_C1R(src, m_ccWidth, dst, m_ccWidth, roi);
            }
            // sampling 422
            if (m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 1)
            {
               // pad most left and most right columns
               fixed (byte* p = m_ccomp[k].m_ss_buffer)
               {
                  byte* p1 = p;
                  byte* p2 = p + (m_ccWidth >> 1) + 1;
                  int step = m_ccWidth;
                  for (i = 0; i < m_ccHeight; i++)
                  {
                     p1[0] = p1[1];
                     p2[0] = p2[-1];
                     p1 += step;
                     p2 += step;
                  }
                  IppiSize roiSrc = new IppiSize(m_ccWidth >> 1, m_ccHeight);
                  IppiSize roiDst = new IppiSize(m_dst.width, m_dst.height);
                  fixed (byte* dst = m_ccomp[k].m_cc_buffer)
                  {
                     status = ipp.jp.ippiSampleUpH2V1_JPEG_8u_C1R(p + 1, m_ccWidth, roiSrc, dst, m_ccWidth, roiDst);
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
            }
            // sampling 411
            if (m_ccomp[k].m_h_factor == 2 && m_ccomp[k].m_v_factor == 2)
            {
               // pad most left and most right columns
               fixed (byte* p = m_ccomp[k].m_ss_buffer)
               {
                  byte* p1 = p;
                  byte* p2 = p + (m_ccWidth >> 1) + 1;
                  int step = m_ccWidth;
                  for (i = 0; i < (m_ccHeight >> 1); i++)
                  {
                     p1[0] = p1[1];
                     p2[0] = p2[-1];
                     p1 += step;
                     p2 += step;
                  }
                  // replicate top row
                  p1 = p + m_ccWidth;
                  p2 = p;
                  ipp.sp.ippsCopy_8u(p1, p2, m_ccWidth);
                  p1 = p + m_ccWidth * m_ccHeight - 1;
                  p2 = p + m_ccWidth * m_ccHeight;
                  ipp.sp.ippsCopy_8u(p1, p2, m_ccWidth);
                  IppiSize roiSrc = new IppiSize(m_ccWidth >> 1, m_ccHeight >> 1);
                  IppiSize roiDst = new IppiSize(m_dst.width, m_dst.height);
                  fixed (byte* dst = m_ccomp[k].m_cc_buffer)
                  {
                     status = ipp.jp.ippiSampleUpH2V2_JPEG_8u_C1R(p + m_ccWidth + 1, m_ccWidth, roiSrc, dst, m_ccWidth, roiDst);
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
               }
            }
         } // for m_jpeg_ncomp
         return JERRCODE.JPEG_OK;
      }

      JERRCODE PerformDCT()
      {
         int i, j, n, k, l;
         int size;
         int dst_step;
         IppStatus status;

         dst_step = m_ccWidth;
         for (size = 0, n = 0; n < m_jpeg_ncomp; n++)
         {
            size += (m_ccomp[n].m_hsampling * m_ccomp[n].m_vsampling);
         }
         for (i = 0; i < m_numyMCU; i++)
         {
            for (j = 0; j < m_numxMCU; j++)
            {
               fixed (short* p = m_coefbuf)
               {
                  short* block = p + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                  for (n = 0; n < m_jpeg_ncomp; n++)
                  {
                     fixed (ushort* qtbl = m_qntbl[m_ccomp[n].m_q_selector].m_qbf)
                     {
                        for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                        {
                           for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                           {
                              fixed (byte* pd = m_ccomp[n].m_ss_buffer)
                              {
                                 byte* dst = pd +
                                    i * 8 * m_ccomp[n].m_vsampling * m_ccWidth +
                                    j * 8 * m_ccomp[n].m_hsampling +
                                    k * 8 * m_ccWidth;
                                 if (m_ccomp[n].m_v_factor == 2)
                                 {
                                    dst += m_ccWidth;
                                 }
                                 if (m_ccomp[n].m_h_factor == 2)
                                 {
                                    dst++;
                                 }
                                 dst += l * 8;
                                 status = ipp.jp.ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R(
                                    block,
                                    dst,
                                    dst_step,
                                    qtbl);
                                 if (IppStatus.ippStsNoErr != status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                                 block += DCTSIZE2;
                              }
                           } // for m_hsampling
                        } // for m_vsampling
                     }
                  }
               }
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ColorConvert(int nMCURow)
      {
         IppStatus status;
         if (nMCURow == m_numyMCU - 1)
         {
            m_ccHeight = m_mcuHeight - m_yPadding;
         }
         IppiSize roi = new IppiSize(m_dst.width, m_ccHeight);
         byte* dst = m_dst.Data8u + nMCURow * m_mcuHeight * m_dst.lineStep;

         if (m_jpeg_color == JCOLOR.JC_UNKNOWN && m_dst.color == JCOLOR.JC_UNKNOWN)
         {
            switch (m_jpeg_ncomp)
            {
               case 1:
                  fixed (byte* pSrc = m_ccomp[0].m_cc_buffer)
                  {
                     status = ipp.ip.ippiCopy_8u_C1R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 3:
                  fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                               pSrc1 = m_ccomp[1].m_cc_buffer,
                               pSrc2 = m_ccomp[2].m_cc_buffer)
                  {
                     byte*[] src = { pSrc0, pSrc1, pSrc2 };
                     fixed (byte** pSrc = src)
                     {
                        status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                     }
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 4:
                  fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                               pSrc1 = m_ccomp[1].m_cc_buffer,
                               pSrc2 = m_ccomp[2].m_cc_buffer,
                               pSrc3 = m_ccomp[3].m_cc_buffer)
                  {
                     byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
                     fixed (byte** pSrc = src)
                     {
                        status = ipp.ip.ippiCopy_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                     }
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               default:
                  return JERRCODE.JPEG_NOT_IMPLEMENTED;
            }
         }
         // Gray to Gray
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_GRAY)
         {
            fixed (byte* pSrc = m_ccomp[0].m_cc_buffer)
            {
               status = ipp.ip.ippiCopy_8u_C1R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // Gray to RGB
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* p = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { p, p, p };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // Gray to BGR
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* p = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { p, p, p };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to RGB
         if (m_jpeg_color == JCOLOR.JC_RGB && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to BGR
         if (m_jpeg_color == JCOLOR.JC_RGB && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* pSrc0 = m_ccomp[2].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCbCr to RGB
         if (m_jpeg_color == JCOLOR.JC_YCBCR && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCbCrToRGB_JPEG_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCbCr to BGR
         if (m_jpeg_color == JCOLOR.JC_YCBCR && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCbCrToBGR_JPEG_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to CMYK
         if (m_jpeg_color == JCOLOR.JC_CMYK && m_dst.color == JCOLOR.JC_CMYK)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer,
                         pSrc3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCCK to CMYK
         if (m_jpeg_color == JCOLOR.JC_YCCK && m_dst.color == JCOLOR.JC_CMYK)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer,
                         pSrc3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCCKToCMYK_JPEG_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ColorConvert()
      {
         IppStatus status;
         IppiSize roi = new IppiSize(m_dst.width, m_dst.height);
         byte* dst = m_dst.Data8u;
         if (m_jpeg_color == JCOLOR.JC_UNKNOWN && m_dst.color == JCOLOR.JC_UNKNOWN)
         {
            switch (m_jpeg_ncomp)
            {
               case 1:
                  fixed (byte* pSrc = m_ccomp[0].m_cc_buffer)
                  {
                     status = ipp.ip.ippiCopy_8u_C1R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 3:
                  fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                               pSrc1 = m_ccomp[1].m_cc_buffer,
                               pSrc2 = m_ccomp[2].m_cc_buffer)
                  {
                     byte*[] src = { pSrc0, pSrc1, pSrc2 };
                     fixed (byte** pSrc = src)
                     {
                        status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                     }
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               case 4:
                  fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                               pSrc1 = m_ccomp[1].m_cc_buffer,
                               pSrc2 = m_ccomp[2].m_cc_buffer,
                               pSrc3 = m_ccomp[3].m_cc_buffer)
                  {
                     byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
                     fixed (byte** pSrc = src)
                     {
                        status = ipp.ip.ippiCopy_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
                     }
                  }
                  if (IppStatus.ippStsNoErr != status)
                  {
                     return JERRCODE.JPEG_INTERNAL_ERROR;
                  }
                  break;
               default:
                  return JERRCODE.JPEG_NOT_IMPLEMENTED;
            }
         }
         // Gray to Gray
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_GRAY)
         {
            fixed (byte* pSrc = m_ccomp[0].m_cc_buffer)
            {
               status = ipp.ip.ippiCopy_8u_C1R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // Gray to RGB
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* p = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { p, p, p };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // Gray to BGR
         if (m_jpeg_color == JCOLOR.JC_GRAY && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* p = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { p, p, p };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to RGB
         if (m_jpeg_color == JCOLOR.JC_RGB && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // RGB to BGR
         if (m_jpeg_color == JCOLOR.JC_RGB && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* pSrc0 = m_ccomp[2].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[0].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCbCr to RGB
         if (m_jpeg_color == JCOLOR.JC_YCBCR && m_dst.color == JCOLOR.JC_RGB)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCbCrToRGB_JPEG_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCbCr to BGR
         if (m_jpeg_color == JCOLOR.JC_YCBCR && m_dst.color == JCOLOR.JC_BGR)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCbCrToBGR_JPEG_8u_P3C3R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // CMYK to CMYK
         if (m_jpeg_color == JCOLOR.JC_CMYK && m_dst.color == JCOLOR.JC_CMYK)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer,
                         pSrc3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.ip.ippiCopy_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         // YCCK to CMYK
         if (m_jpeg_color == JCOLOR.JC_YCCK && m_dst.color == JCOLOR.JC_CMYK)
         {
            fixed (byte* pSrc0 = m_ccomp[0].m_cc_buffer,
                         pSrc1 = m_ccomp[1].m_cc_buffer,
                         pSrc2 = m_ccomp[2].m_cc_buffer,
                         pSrc3 = m_ccomp[3].m_cc_buffer)
            {
               byte*[] src = { pSrc0, pSrc1, pSrc2, pSrc3 };
               fixed (byte** pSrc = src)
               {
                  status = ipp.jp.ippiYCCKToCMYK_JPEG_8u_P4C4R(pSrc, m_ccWidth, dst, m_dst.lineStep, roi);
               }
            }
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DCT_QNT_SSCC_MCURowBL(short* pMCUBuf, int mcu_row)
      {
         int j, n, k, l;
         byte* dst = null;
         int dst_step = m_ccWidth;
         JERRCODE jerr;
         IppStatus status;

         for (j = 0; j < m_numxMCU; j++)
         {
            for (n = 0; n < m_jpeg_ncomp; n++)
            {
               fixed (byte* ss_buf_0 = m_ccomp[n].m_ss_buffer, cc_buf = m_ccomp[n].m_cc_buffer)
               {
                  byte* ss_buf = null;
                  if (n != 0 && n != 3)
                  {
                     if (0 != m_ccomp[n].m_ss_buffer.Length)
                        ss_buf = ss_buf_0;
                  }
                  fixed (ushort* qtbl = m_qntbl[m_ccomp[n].m_q_selector].m_qbf)
                  {
                     int hsam = m_ccomp[n].m_hsampling;
                     int jhsam = j * 8 * hsam;
                     for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                     {
                        if (m_jpeg_sampling == JSS.JS_444 || n == 0 || n == 3)
                        {
                           dst_step = m_ccWidth;
                           dst = cc_buf + jhsam + k * 8 * dst_step;
                        }
                        else
                        {
                           dst_step = (m_ccWidth >> 1) + 2;
                           dst = ss_buf + jhsam + k * 8 * dst_step;
                        }
                        if (m_ccomp[n].m_v_factor == 2)
                        {
                           dst += dst_step;
                        }
                        if (m_ccomp[n].m_h_factor == 2)
                        {
                           dst++;
                        }
                        for (l = 0; l < hsam; l++)
                        {
                           dst += l * 8;
                           status = ipp.jp.ippiDCTQuantInv8x8LS_JPEG_16s8u_C1R(
                              pMCUBuf,
                              dst,
                              dst_step,
                              qtbl);
                           if (IppStatus.ippStsNoErr > status)
                           {
                              return JERRCODE.JPEG_INTERNAL_ERROR;
                           }
                           pMCUBuf += DCTSIZE2;
                        } // for m_hsampling
                     } // for m_vsampling
                  }
               }
            } // for m_jpeg_ncomp
         } // for m_numxMCU
         if (m_jpeg_sampling != JSS.JS_444)
         {
            jerr = UpSampling(mcu_row);
            if (JERRCODE.JPEG_OK != jerr)
            {
               return jerr;
            }
         }
         jerr = ColorConvert(mcu_row);
         if (JERRCODE.JPEG_OK != jerr)
         {
            return jerr;
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ReconstructMCURowLS(short* pMCUBuf, int mcu_row)
      {
         short* pCurrRow;
         short* pPrevRow;
         byte* pDst;
         IppiSize roi;
         IppStatus status;

         roi = new IppiSize(m_dst.width, 1);
         pDst = m_dst.Data8u + mcu_row * m_dst.width;
         fixed (byte* curr = m_ccomp[0].m_curr_row, prev = m_ccomp[0].m_prev_row)
         {
            pCurrRow = (short*)curr + m_ccomp[0].m_curr_ofs;
            pPrevRow = (short*)prev + m_ccomp[0].m_prev_ofs;
            if (mcu_row != 0)
            {
               status = ipp.jp.ippiReconstructPredRow_JPEG_16s_C1(
                  pMCUBuf, pPrevRow, pCurrRow, m_dst.width, m_ss);
            }
            else
            {
               status = ipp.jp.ippiReconstructPredFirstRow_JPEG_16s_C1(
                  pMCUBuf, pCurrRow, m_dst.width, m_jpeg_precision, m_al);
            }
         }
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         if (0 != m_al)
         {
            status = ipp.sp.ippsLShiftC_16s_I(m_al, pCurrRow, m_dst.width);
            if (IppStatus.ippStsNoErr != status)
            {
               return JERRCODE.JPEG_INTERNAL_ERROR;
            }
         }
         status = ipp.ip.ippiConvert_16s8u_C1R(pCurrRow, m_dst.width * sizeof(short), pDst, m_dst.width, roi);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         byte[] tmp_row = m_ccomp[0].m_curr_row;
         m_ccomp[0].m_curr_row = m_ccomp[0].m_prev_row;
         m_ccomp[0].m_prev_row = tmp_row;
         int tmp_ofs = m_ccomp[0].m_curr_ofs;
         m_ccomp[0].m_curr_ofs = m_ccomp[0].m_prev_ofs;
         m_ccomp[0].m_prev_ofs = tmp_ofs;

         return JERRCODE.JPEG_OK;
      }

      JERRCODE DecodeScanBaseline()
      {
         int scount = 0;
         IppStatus status;
         status = custom_ipp.ippiDecodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_marker = JMARKER.JM_NONE;
         int i;
         fixed (short* pMCUBuf = m_block_buffer)
         {
            i = 0;
            while (i < m_numyMCU)
            {
               i = scount;
               scount++;
               if (i < m_numyMCU)
               {
                  DecodeHuffmanMCURowBL(pMCUBuf);
               }
               if (i < m_numyMCU)
               {
                  DCT_QNT_SSCC_MCURowBL(pMCUBuf, i);
               }
               i++;
            } // for m_numyMCU
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DecodeScanProgressive()
      {
         int i, j, k, n, l, c;
         int size;
         int srcLen;
         JERRCODE jerr;
         IppStatus status;

         m_scan_count++;
         status = custom_ipp.ippiDecodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_marker = JMARKER.JM_NONE;
         srcLen = m_src.DataLen;
         for (size = 0, k = 0; k < m_jpeg_ncomp; k++)
         {
            size += (m_ccomp[k].m_hsampling * m_ccomp[k].m_vsampling);
         }
         short* block;
         if (m_ss != 0 && m_se != 0)
         {
            // AC scan
            for (i = 0; i < m_numyMCU; i++)
            {
               for (k = 0; k < m_ccomp[m_curr_comp_no].m_vsampling; k++)
               {
                  if (i * m_ccomp[m_curr_comp_no].m_vsampling * 8 + k * 8 >= m_jpeg_height)
                     break;
                  for (j = 0; j < m_numxMCU; j++)
                  {
                     fixed (short* p = m_coefbuf)
                     {
                        block = p + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                        // skip any relevant components
                        for (c = 0; c < m_ccomp[m_curr_comp_no].m_comp_no; c++)
                        {
                           block += (DCTSIZE2 * m_ccomp[c].m_hsampling *
                              m_ccomp[c].m_vsampling);
                        }
                        // Skip over relevant 8x8 blocks from this component.
                        block += (k * DCTSIZE2 * m_ccomp[m_curr_comp_no].m_hsampling);
                        for (l = 0; l < m_ccomp[m_curr_comp_no].m_hsampling; l++)
                        {
                           // Ignore the last column(s) of the image.
                           if (((j * m_ccomp[m_curr_comp_no].m_hsampling * 8) + (l * 8)) >= m_jpeg_width)
                              break;
                           if (0 != m_jpeg_restart_interval)
                           {
                              if (m_restarts_to_go == 0)
                              {
                                 jerr = ProcessRestart();
                                 if (JERRCODE.JPEG_OK != jerr)
                                 {
                                    return jerr;
                                 }
                              }
                           }
                           int currPos = m_src.currPos;
                           int marker = (int)m_marker;
                           if (m_ah == 0)
                           {
                              status = custom_ipp.ippiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1(
                                 m_src.pData,
                                 srcLen,
                                 &currPos,
                                 block,
                                 &marker,
                                 m_ss,
                                 m_se,
                                 m_al,
                                 m_actbl[m_ccomp[m_curr_comp_no].m_ac_selector].m_buf,
                                 m_state.m_buf);
                              if (IppStatus.ippStsNoErr > status)
                              {
                                 return JERRCODE.JPEG_INTERNAL_ERROR;
                              }
                           }
                           else
                           {
                              status = custom_ipp.ippiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1(
                                 m_src.pData,
                                 srcLen,
                                 &currPos,
                                 block,
                                 &marker,
                                 m_ss,
                                 m_se,
                                 m_al,
                                 m_actbl[m_ccomp[m_curr_comp_no].m_ac_selector].m_buf,
                                 m_state.m_buf);
                              if (IppStatus.ippStsNoErr > status)
                              {
                                 return JERRCODE.JPEG_INTERNAL_ERROR;
                              }
                           }
                           m_src.currPos = currPos;
                           m_marker = (JMARKER)marker;
                           block += DCTSIZE2;
                           m_restarts_to_go--;
                        } // for m_hsampling
                     }
                  } // for m_numxMCU
               } // for m_vsampling
            } // for m_numyMCU
            if (m_al == 0)
            {
               m_ccomp[m_curr_comp_no].m_ac_scan_completed = 1;
            }
         }
         else
         {
            // DC scan
            for (i = 0; i < m_numyMCU; i++)
            {
               for (j = 0; j < m_numxMCU; j++)
               {
                  if (0 != m_jpeg_restart_interval)
                  {
                     if (m_restarts_to_go == 0)
                     {
                        jerr = ProcessRestart();
                        if (JERRCODE.JPEG_OK != jerr)
                        {
                           return jerr;
                        }
                     }
                  }
                  fixed (short* p = m_coefbuf)
                  {
                     block = p + (DCTSIZE2 * size * (j + (i * m_numxMCU)));
                     if (m_ah == 0)
                     {
                        // first DC scan
                        for (n = 0; n < m_jpeg_ncomp; n++)
                        {
                           for (k = 0; k < m_ccomp[n].m_vsampling; k++)
                           {
                              for (l = 0; l < m_ccomp[n].m_hsampling; l++)
                              {
                                 short lastDC = m_ccomp[n].m_lastDC;
                                 int currPos = m_src.currPos;
                                 int marker = (int)m_marker;
                                 status = custom_ipp.ippiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1(
                                    m_src.pData,
                                    srcLen,
                                    &currPos,
                                    block,
                                    &lastDC,
                                    &marker,
                                    m_al,
                                    m_dctbl[m_ccomp[n].m_dc_selector].m_buf,
                                    m_state.m_buf);
                                 if (IppStatus.ippStsNoErr > status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                                 m_marker = (JMARKER)marker;
                                 m_src.currPos = currPos;
                                 m_ccomp[n].m_lastDC = lastDC;
                                 block += DCTSIZE2;
                              } // for m_hsampling
                           } // for m_vsampling
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
                                 int currPos = m_src.currPos;
                                 int marker = (int)m_marker;
                                 status = custom_ipp.ippiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1(
                                    m_src.pData,
                                    srcLen,
                                    &currPos,
                                    block,
                                    &marker,
                                    m_al,
                                    m_state.m_buf);
                                 if (IppStatus.ippStsNoErr > status)
                                 {
                                    return JERRCODE.JPEG_INTERNAL_ERROR;
                                 }
                                 m_marker = (JMARKER)marker;
                                 m_src.currPos = currPos;
                                 block += DCTSIZE2;
                              } // for m_hsampling
                           } // for m_vsampling
                        } // for m_jpeg_ncomp
                     }
                  }
                  m_restarts_to_go--;
               } // for m_numxMCU
            } // for m_numyMCU
            if (m_al == 0)
            {
               m_dc_scan_completed = 1;
            }
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE DecodeScanLossless()
      {

         JERRCODE jerr;
         IppStatus status;
         int i;

         status = custom_ipp.ippiDecodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         m_marker = JMARKER.JM_NONE;
         fixed (short* pMCUBuf = m_block_buffer)
         {
            i = 0;
            while (i < m_numyMCU)
            {
               if (i < m_numyMCU)
               {
                  jerr = DecodeHuffmanMCURowLS(pMCUBuf);
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
               }
               i++; // advance counter to the next mcu row
               if ((i - 1) < m_numyMCU)
               {
                  jerr = ReconstructMCURowLS(pMCUBuf, i - 1);
                  if (JERRCODE.JPEG_OK != jerr)
                  {
                     return jerr;
                  }
               }
            } // for m_numyMCU
         }
         return JERRCODE.JPEG_OK;
      }

      JERRCODE ProcessRestart()
      {
         JERRCODE jerr;
         IppStatus status;

         status = custom_ipp.ippiDecodeHuffmanStateInit_JPEG_8u(m_state.m_buf);
         if (IppStatus.ippStsNoErr != status)
         {
            return JERRCODE.JPEG_INTERNAL_ERROR;
         }
         for (int n = 0; n < m_jpeg_ncomp; n++)
         {
            m_ccomp[n].m_lastDC = 0;
         }
         jerr = ParseRST();
         if (JERRCODE.JPEG_OK != jerr)
         {
            //    return jerr;
         }
         m_restarts_to_go = m_jpeg_restart_interval;
         return JERRCODE.JPEG_OK;
      }

   }
}
