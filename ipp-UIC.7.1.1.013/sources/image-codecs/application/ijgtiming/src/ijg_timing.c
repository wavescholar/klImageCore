
/*
//
//             INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "jpeglib.h"
#include "ippdefs.h"
#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippj.h"




#define BITMAPFILEHEADER_SIZE 14
#define BITMAPINFOHEADER_SIZE 40
#define _IPP 1 // set to 0 to disable IPP calls in IJG

typedef enum _JPEG_SAMPLING
{
  SS_OTHER = 0,
  SS_444   = 444,
  SS_422   = 422,
  SS_411   = 411

} JSS;


#if defined ( linux ) || defined ( UNIX )

#define  BI_BITFIELDS     3
#define  BI_RLE4          2
#define  BI_RLE8          1
#define  BI_RGB           0

#define _MAX_PATH       256
#define _MAX_DRIVE       16
#define _MAX_DIR        256
#define _MAX_FNAME      256
#define _MAX_EXT         16


static void _splitpath(
  char* path,
  char* drive,
  char* dir,
  char* fname,
  char* ext)
{
  char* p;
  char* last_slash  = NULL;
  char* dot         = NULL;
  unsigned int len;

  /* extract drive letter, if any */
  if((strlen(path) >= (_MAX_DRIVE - 2)) && (*(path + _MAX_DRIVE - 2) == ':') )
  {
    if(drive)
    {
      strncpy(drive,path,_MAX_DRIVE - 1);
      *(drive + _MAX_DRIVE - 1) = '\0';
    }
    path += _MAX_DRIVE - 1;
  }
  else if(drive)
  {
    *drive = '\0';
  }

  /* extract path string, if any */
  for(last_slash = NULL, p = path; *p; p++)
  {
    if(*p == '/' || *p == '\\')
      last_slash = p + 1;
    else if(*p == '.')
      dot = p;
  }

  if(last_slash)
  {
    if(dir)
    {
      len = IPP_MIN((unsigned int)(((char*)last_slash - (char*)path) / sizeof(char)),(_MAX_DIR - 1));
      strncpy(dir,path,len);
      *(dir + len) = '\0';
    }
    path = last_slash;
  }
  else if(dir)
  {
    *dir = '\0';
  }

  /* extract file name, if any */
  if(dot && (dot >= path))
  {
    if(fname)
    {
      len = IPP_MIN((unsigned int)(((char*)dot - (char*)path) / sizeof(char)),(_MAX_FNAME - 1));
      strncpy(fname,path,len);
      *(fname + len) = '\0';
    }

    if(ext)
    {
      len = IPP_MIN((unsigned int)(((char*)p - (char*)dot) / sizeof(char)),(_MAX_EXT - 1));
      strncpy(ext,dot,len);
      *(ext + len) = '\0';
    }
  }
  else
  {
    if(fname)
    {
      len = IPP_MIN((unsigned int)(((char*)p - (char*)path) / sizeof(char)),(_MAX_FNAME - 1));
      strncpy(fname,path,len);
      *(fname + len) = '\0';
    }
    if(ext)
    {
      *ext = '\0';
    }
  }

  return;
} /* _splitpath() */


static void _makepath(
  char* path,
  char* drive,
  char* dir,
  char* fname,
  char* ext)
{
  char* p;

  /* make drive */
  if(drive && *drive)
  {
    *path++ = *drive;
    *path++ = ':';
  }

  /* make dir */
  if((p = dir) && *p)
  {
    do
    {
      *path++ = *p++;
    } while(*p);

    if(*(p-1) != '/' && *(p-1) != '\\')
    {
      *path++ = '\\';
    }
  }

  /* make fname */
  if(p = fname)
  {
    while(*p)
      *path++ = *p++;
  }

  /* make ext */
  if(p = ext)
  {
    if(*p && *p != '.')
    {
      *path++ = '.';
    }
    while(*path++ = *p++)
      ;
  }
  else
  {
    *path = '\0';
  }

  return;
} /* _makepath() */

#endif /* linux */


#define get_pentium_counter ippGetCpuClocks

static void copyright(void)
{
  printf("Test program for IJG library boosted with\n");
  printf("Intel(R) Integrated Performance Primitives\n");
  printf("Copyright(c) 2001-2010 Intel Corporation. All Rights Reserved.\n");
  return;
} /* copyright() */


static void usage(void)
{
  printf("use ijg_timing.exe [options]\n");
  printf("where options are:\n");
  printf("  -?|-h show usage information\n");
  printf("  -v show ippJP library version\n");
  printf("  -a auto testing with hardcoded parameters.\n");
  printf("     They are encoding/decoding 640x480 1, 3 and 4 channels images\n");
  printf("     with random pixel values and 444, 422 and 411 sampling and 75%% quality\n");
  printf("  -t<bmp name> encode bmp to jpeg, after that decode jpeg to bmp\n");
  printf("     and compare with original bmp\n");
  printf("  -d<file name> decode JPEG <file name> file\n");
  printf("  -e<file name> encode BMP <file name> file\n");
  printf("  -qQUALITY jpeg quality 1..100\n");
  printf("  -sSAMPLING jpeg sampling 444, 422, 411\n");
  return;
} /* usage() */


static void ipp_version(void)
{
  const IppLibraryVersion* version;

  printf("Intel(R) Integrated Performance Primitives\n");

  /* ippCore version info */
  version = ippGetLibVersion();

  printf("  %s, %s {%d.%d.%d.%d}, build date %s\n",
    version->Name,
    version->Version,
    version->major,version->minor,version->majorBuild,version->build,
    version->BuildDate);

  /* ippSP version info */
  version = ippsGetLibVersion();

  printf("  %s, %s {%d.%d.%d.%d}, build date %s\n",
    version->Name,
    version->Version,
    version->major,version->minor,version->majorBuild,version->build,
    version->BuildDate);

  /* ippIP version info */
  version = ippiGetLibVersion();

  printf("  %s, %s {%d.%d.%d.%d}, build date %s\n",
    version->Name,
    version->Version,
    version->major,version->minor,version->majorBuild,version->build,
    version->BuildDate);

  /* ippJP version info */
  version = ippjGetLibVersion();

  printf("  %s, %s {%d.%d.%d.%d}, build date %s\n",
    version->Name,
    version->Version,
    version->major,version->minor,version->majorBuild,version->build,
    version->BuildDate);

  return;
} /* ipp_version() */


static void set_sampling(
  struct jpeg_compress_struct* cinfo,
  JSS sampling,
  int nchannels)
{
  switch(sampling)
  {
    case SS_444:
      cinfo->comp_info[0].h_samp_factor = 1;
      cinfo->comp_info[0].v_samp_factor = 1;
      if(nchannels == 3)
      {
        cinfo->comp_info[1].h_samp_factor = 1;
        cinfo->comp_info[1].v_samp_factor = 1;
        cinfo->comp_info[2].h_samp_factor = 1;
        cinfo->comp_info[2].v_samp_factor = 1;
      }
      if(nchannels == 4)
      {
        cinfo->comp_info[3].h_samp_factor = 1;
        cinfo->comp_info[3].v_samp_factor = 1;
      }
      break;

    case SS_422:
      cinfo->comp_info[0].h_samp_factor = 2;
      cinfo->comp_info[0].v_samp_factor = 1;
      cinfo->comp_info[1].h_samp_factor = 1;
      cinfo->comp_info[1].v_samp_factor = 1;
      cinfo->comp_info[2].h_samp_factor = 1;
      cinfo->comp_info[2].v_samp_factor = 1;
      if(nchannels == 4)
      {
        cinfo->comp_info[3].h_samp_factor = 2;
        cinfo->comp_info[3].v_samp_factor = 1;
      }
      break;

    case SS_411:
      cinfo->comp_info[0].h_samp_factor = 2;
      cinfo->comp_info[0].v_samp_factor = 2;
      cinfo->comp_info[1].h_samp_factor = 1;
      cinfo->comp_info[1].v_samp_factor = 1;
      cinfo->comp_info[2].h_samp_factor = 1;
      cinfo->comp_info[2].v_samp_factor = 1;
      if(nchannels == 4)
      {
        cinfo->comp_info[3].h_samp_factor = 2;
        cinfo->comp_info[3].v_samp_factor = 2;
      }
      break;

    default:
      printf("unknown sampling - %d\n",sampling);
  }

  return;
} /* set_sampling() */


static void get_sampling(
  struct jpeg_decompress_struct* cinfo,
  JSS* sampling)
{
  if(cinfo->num_components == 1)
  {
    *sampling = SS_444;
  }
  else
  if(cinfo->num_components == 3)
  {
    if(cinfo->comp_info[0].h_samp_factor == 1 && cinfo->comp_info[0].v_samp_factor == 1 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1)
    {
      *sampling = SS_444;
    }
    else
    if(cinfo->comp_info[0].h_samp_factor == 2 && cinfo->comp_info[0].v_samp_factor == 1 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1)
    {
      *sampling = SS_422;
    }
    else
    if(cinfo->comp_info[0].h_samp_factor == 2 && cinfo->comp_info[0].v_samp_factor == 2 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1)
    {
      *sampling = SS_411;
    }
    else
    {
      printf("unknown sampling\n");
      *sampling = SS_OTHER;
    }
  }
  else
  if(cinfo->num_components == 4)
  {
    if(cinfo->comp_info[0].h_samp_factor == 1 && cinfo->comp_info[0].v_samp_factor == 1 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1 &&
       cinfo->comp_info[3].h_samp_factor == 1 && cinfo->comp_info[3].v_samp_factor == 1)
    {
      *sampling = SS_444;
    }
    else
    if(cinfo->comp_info[0].h_samp_factor == 2 && cinfo->comp_info[0].v_samp_factor == 1 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1 &&
       cinfo->comp_info[3].h_samp_factor == 2 && cinfo->comp_info[3].v_samp_factor == 1)
    {
      *sampling = SS_422;
    }
    else
    if(cinfo->comp_info[0].h_samp_factor == 2 && cinfo->comp_info[0].v_samp_factor == 2 &&
       cinfo->comp_info[1].h_samp_factor == 1 && cinfo->comp_info[1].v_samp_factor == 1 &&
       cinfo->comp_info[2].h_samp_factor == 1 && cinfo->comp_info[2].v_samp_factor == 1 &&
       cinfo->comp_info[3].h_samp_factor == 2 && cinfo->comp_info[3].v_samp_factor == 2)
    {
      *sampling = SS_411;
    }
    else
    {
      printf("unknown sampling\n");
      *sampling = SS_OTHER;
    }
  }
  else
  {
    printf("bad number of channels - %d\n",cinfo->num_components);
  }

  return;
} /* get_sampling() */


static int ijg_encode(
  Ipp8u*        inbuf,
  int           width,
  int           height,
  int           nchannels,
  JSS           sampling,
  int           quality,
  char*         outname,
  Ipp64u*       cpu_clocks)
{
  int    i;
  int    res;
  int    col;
  int    in_line_step;
  int    cur_input_row;
  Ipp64u clk0;
  Ipp64u clk1;
  Ipp8u* inptr;
  Ipp8u* outptr;
  Ipp8u* temp_buf = NULL;
  FILE*  fo = NULL;

  struct jpeg_error_mgr jerr;
  struct jpeg_compress_struct cinfo;

  fo = fopen(outname,"wb+");
  if(NULL == fo)
  {
    printf("can't create file %s\n",outname);
    res = -1;
    goto Exit;
  }

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_compress(&cinfo);

  cinfo.data_precision   = 8;
  cinfo.image_width      = width;
  cinfo.image_height     = height;
  cinfo.input_components = nchannels;
  cinfo.in_color_space   = ( (nchannels == 1) ? JCS_GRAYSCALE : ( (nchannels == 3) ? JCS_RGB : JCS_CMYK ) );

  jpeg_set_defaults(&cinfo);

  cinfo.jpeg_color_space = ( (nchannels == 1) ? JCS_GRAYSCALE : ( (nchannels == 3) ? JCS_YCbCr : JCS_YCCK) );

  jpeg_stdio_dest(&cinfo,fo);

  jpeg_set_quality(&cinfo,quality,TRUE);

  set_sampling(&cinfo,sampling,nchannels);

  in_line_step = cinfo.input_components * cinfo.image_width;

  while(in_line_step & 3)
  {
    in_line_step++;
  }

  temp_buf = malloc(in_line_step);
  if(NULL == temp_buf)
  {
    printf("can't allocate %d bytes\n",in_line_step);
    res = -1;
    goto Exit;
  }
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
  Sleep(0);
#endif
  clk0 = get_pentium_counter();

  jpeg_start_compress(&cinfo,TRUE);

  cur_input_row = 0;

  while(cinfo.next_scanline < cinfo.image_height)
  {
    cur_input_row++;
    inptr  = inbuf + in_line_step * (cinfo.image_height - cur_input_row);
    outptr = temp_buf;

    for(col = cinfo.image_width; col--;)
    {
      switch(cinfo.input_components)
      {
      case 1:
        *outptr++ = *inptr++;
        break;

      case 3:
        for(i = cinfo.input_components; i--;)
        {
          outptr[0] = inptr[2];
          outptr[1] = inptr[1];
          outptr[2] = inptr[0];
        }
        inptr  += 3;
        outptr += 3;
        break;

      case 4:
        for(i = cinfo.input_components; i--;)
        {
          outptr[0] = inptr[2];
          outptr[1] = inptr[1];
          outptr[2] = inptr[0];
          outptr[3] = inptr[3];
        }
        inptr  += 4;
        outptr += 4;
        break;
      }
    }

    while((int)outptr & 3)
    {
      *outptr++ = 0;
    }


    jpeg_write_scanlines(&cinfo,&temp_buf,1);
  }

  jpeg_finish_compress(&cinfo);

  clk1 = get_pentium_counter();
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
#endif
  *cpu_clocks = (Ipp64u)(clk1 - clk0);

  res = 0;

Exit:

  if(NULL != temp_buf)
  {
    free(temp_buf);
  }

  jpeg_destroy_compress(&cinfo);

  /* free memory and close files */
  if(NULL != fo)
  {
    fclose(fo);
  }

  return res;
} /* ijg_encode() */


static int ijg_decode(
  char*         name,
  Ipp8u**       buffer,
  int*          width,
  int*          height,
  int*          nchannels,
  JSS*          sampling,
 Ipp64u*        cpu_clocks)
{
  int    res;
  int    col;
  int    bmp_width;
  int    cur_output_row;
  int    img_size;
  JSS    ss;
  Ipp64u clk0;
  Ipp64u clk1;
  Ipp8u* inptr = NULL;
  Ipp8u* outptr = NULL;
  Ipp8u* tmp_buff = NULL;
  Ipp8u* img_buff = NULL;
  FILE*  fi = NULL;

  struct jpeg_error_mgr jerr;
  struct jpeg_decompress_struct cinfo;

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);

  fi = fopen(name,"rb");
  if(NULL == fi)
  {
    printf("can't open file - %s\n",name);
    res = -1;
    goto Exit;
  }

  jpeg_stdio_src(&cinfo,fi);

  jpeg_read_header(&cinfo,TRUE);

  jpeg_calc_output_dimensions(&cinfo);

  bmp_width = cinfo.output_components * cinfo.output_width;

  while(bmp_width & 3)
  {
    bmp_width++;
  }

  img_size = bmp_width * cinfo.output_height;

  img_buff = (Ipp8u*)malloc(img_size);
  if(NULL == img_buff)
  {
    printf("can't allocate %d bytes\n",img_size);
    res = -1;
    goto Exit;
  }

  tmp_buff = (Ipp8u*)malloc(bmp_width);
  if(NULL == tmp_buff)
  {
    printf("can't allocate %d bytes\n",bmp_width);
    res = -1;
    goto Exit;
  }
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
  Sleep(0);
#endif
  clk0 = get_pentium_counter();

  jpeg_start_decompress(&cinfo);

  cur_output_row = 0;

  while(cinfo.output_scanline < cinfo.output_height)
  {
    if(jpeg_read_scanlines(&cinfo,&  tmp_buff,1))
    {
      cur_output_row++;
      outptr = img_buff + bmp_width * (cinfo.output_height - cur_output_row);
      inptr  = tmp_buff;
      for(col = cinfo.output_width; col--;)
      {
        switch(cinfo.output_components)
        {
        case 3:
          outptr[2] = inptr[0];
          outptr[1] = inptr[1];
          outptr[0] = inptr[2];

          inptr  += 3;
          outptr += 3;

          break;

        case 4:
          outptr[0] = inptr[2];
          outptr[1] = inptr[1];
          outptr[2] = inptr[0];
          outptr[3] = inptr[3];

          inptr  += 4;
          outptr += 4;

          break;

        case 1:
          *outptr++ = *inptr++;
          break;

        default:
          break;
        }
      }

      while((int)outptr & 3)
      {
        *outptr++ = 0;
      }
    }
  }

  get_sampling(&cinfo,&ss);

  jpeg_finish_decompress(&cinfo);

  clk1 = get_pentium_counter();
#ifdef _WIN32
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
#endif
  *buffer    = img_buff;
  *width     = cinfo.output_width;
  *height    = cinfo.output_height;
  *nchannels = cinfo.output_components;
  *sampling  = ss;

  *cpu_clocks = (Ipp64u)(clk1 - clk0);

  res = 0;

Exit:

  jpeg_destroy_decompress(&cinfo);

  /* free memory and close files */
  if(NULL != fi)
  {
    fclose(fi);
  }

  if(NULL != tmp_buff)
  {
    free(tmp_buff);
  }

  if(0 != res)
  {
    if(NULL != img_buff)
    {
      free(img_buff);
    }
  }

  return res;
} /* ijg_decode() */


static int DIB_PAD_BYTES(
  int width,
  int nchannels)
{
  int pad_bytes;
  int dib_align;
  int uwidth;
  int awidth;

  dib_align = sizeof(Ipp32u) - 1;

  uwidth = width * nchannels;
  awidth = (uwidth + dib_align) & (~dib_align);

  pad_bytes = awidth - uwidth;

  return pad_bytes;
} /* DIB_PAD_BYTES() */


static unsigned int read_word(FILE* fi)
{
  unsigned int c;
  unsigned int word;

  c = fgetc(fi);
  if(EOF == c)
    return c;

  word = c;

  c = fgetc(fi);
  if(EOF == c)
    return c;

  word |= c << 8;

  return word;
} /* read_word() */


static unsigned int read_dword(FILE* fi)
{
  unsigned int word;
  unsigned int dword;

  word = read_word(fi);
  if(EOF == word)
    return word;

  dword = word;

  word = read_word(fi);
  if(EOF == word)
    return word;

  dword |= word << 16;

  return dword;
} /* read_dword() */


static int load_bmp(
  char*   bmp_name,
  Ipp8u** bmp_buff,
  int*    width,
  int*    height,
  int*    nchannels)
{
  int              res;
  int              img_size;
  int              pad_bytes;
  int              width_step;
  size_t           cnt;
  FILE*            fi = NULL;
  Ipp8u*           buff;

  Ipp16u           bfType;
  Ipp32u           bfSize;
  Ipp16u           bfReserved1;
  Ipp16u           bfReserved2;
  Ipp32u           bfOffBits;

  Ipp32u           biSize;
  Ipp32u           biWidth;
  Ipp32u           biHeight;
  Ipp16u           biPlanes;
  Ipp16u           biBitCount;
  Ipp32u           biCompression;
  Ipp32u           biSizeImage;
  Ipp32u           biXPelsPerMeter;
  Ipp32u           biYPelsPerMeter;
  Ipp32u           biClrUsed;
  Ipp32u           biClrImportant;

  fi = fopen(bmp_name,"rb");
  if(NULL == fi)
  {
    printf("can't open file %s\n",bmp_name);
    res = -1;
    goto Exit;
  }

  bfType = (Ipp16u)read_word(fi);
  if((Ipp16u)EOF == bfType)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  if(bfType != 0x4d42) /* BMP signature 'BM' */
  {
    printf("invalid BMP file\n");
    res = -1;
    goto Exit;
  }

  bfSize = read_dword(fi);
  if((Ipp32u)EOF == bfSize)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  bfReserved1 = (Ipp16u)read_word(fi);
  if((Ipp16u)EOF == bfReserved1)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  bfReserved2 = (Ipp16u)read_word(fi);
  if((Ipp16u)EOF == bfReserved2)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  bfOffBits = read_dword(fi);
  if((Ipp32u)EOF == bfOffBits)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biSize = read_dword(fi);
  if((Ipp32u)EOF == biSize)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biWidth = read_dword(fi);
  if((Ipp32u)EOF == biWidth)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biHeight = read_dword(fi);
  if((Ipp32u)EOF == biHeight)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biPlanes = (Ipp16u)read_word(fi);
  if((Ipp16u)EOF == biPlanes)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biBitCount = (Ipp16u)read_word(fi);
  if((Ipp16u)EOF == biBitCount)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biCompression = read_dword(fi);
  if((Ipp32u)EOF == biCompression)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biSizeImage = read_dword(fi);
  if((Ipp32u)EOF == biSizeImage)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biXPelsPerMeter = read_dword(fi);
  if((Ipp32u)EOF == biXPelsPerMeter)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biYPelsPerMeter = read_dword(fi);
  if((Ipp32u)EOF == biYPelsPerMeter)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biClrUsed = read_dword(fi);
  if((Ipp32u)EOF == biClrUsed)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  biClrImportant = read_dword(fi);
  if((Ipp32u)EOF == biClrImportant)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  if(biSize != BITMAPINFOHEADER_SIZE || biCompression != BI_RGB)
  {
    printf("Unsupported BMP file\n");
    res = -1;
    goto Exit;
  }

  switch(biBitCount)
  {
  case 24:
    pad_bytes = DIB_PAD_BYTES(biWidth,3);

    width_step = (biWidth * 3) + pad_bytes;

    img_size = width_step * abs(biHeight);
    break;

  case 32:
    width_step = biWidth * 4;

    img_size = width_step * abs(biHeight);
    break;

  default:
    printf("Unsupported BMP file\n");
    res = -1;
    goto Exit;
  }

  buff = (Ipp8u*)malloc(img_size);
  if(NULL == buff)
  {
    printf("can't allocate %d bytes\n",img_size);
    res = -1;
    goto Exit;
  }

  cnt = fread(buff,sizeof(Ipp8u),img_size,fi);
  if(cnt != (size_t)img_size)
  {
    printf("can't read from file\n");
    res = -1;
    goto Exit;
  }

  *bmp_buff  = buff;
  *width     = biWidth;
  *height    = biHeight;
  *nchannels = biBitCount >> 3;

  res = 0;

Exit:

  if(NULL != fi)
  {
    fclose(fi);
  }

  return res;
} /* load_bmp() */


static unsigned int write_word(Ipp16u word,FILE* fo)
{
  Ipp32u c;

  c = (Ipp32u)(word & 0xff);

  if(EOF == fputc(c,fo))
    return EOF;

  c = (Ipp32u)(word >> 8);

  if(EOF == fputc(c,fo))
    return EOF;

  return 0;
} /* write_word() */


static unsigned int write_dword(Ipp32u dword,FILE* fo)
{
  Ipp16u word;

  word = (Ipp16u)(dword & 0xffff);

  if(EOF == write_word(word,fo))
    return EOF;

  word = (Ipp16u)(dword >> 16);

  if(EOF == write_word(word,fo))
    return EOF;

  return 0;
} /* write_dword() */


static int write_bmp(
  Ipp8u* buffer,
  int    width,
  int    height,
  int    nchannels,
  char*  name)
{
  int     i;
  int     res;
  int     row;
  int     bmp_size;
  int     img_size;
  int     bmp_width;
  size_t  cnt;
  FILE*   fo = NULL;
  Ipp8u*  outptr = NULL;

  Ipp16u  bfType;
  Ipp32u  bfSize;
  Ipp16u  bfReserved1;
  Ipp16u  bfReserved2;
  Ipp32u  bfOffBits;

  Ipp32u  biSize;
  Ipp32u  biWidth;
  Ipp32u  biHeight;
  Ipp16u  biPlanes;
  Ipp16u  biBitCount;
  Ipp32u  biCompression;
  Ipp32u  biSizeImage;
  Ipp32u  biXPelsPerMeter;
  Ipp32u  biYPelsPerMeter;
  Ipp32u  biClrUsed;
  Ipp32u  biClrImportant;

  Ipp8u   palette[256*4];

  fo = fopen(name,"wb+");
  if(NULL == fo)
  {
    printf("can't open file - %s\n",name);
    res = -1;
    goto Exit;
  }

  bmp_width = nchannels * width;

  while(bmp_width & 3)
  {
    bmp_width++;
  }

  img_size = bmp_width * height;
  bmp_size = BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE + img_size;

  if(nchannels == 1)
  {
    bmp_size += sizeof(palette);
  }

  bfType      = 0x4d42; /* BMP signature 'BM' */
  bfSize      = bmp_size;
  bfReserved1 = 0;
  bfReserved2 = 0;
  bfOffBits   = BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE;

  if(EOF == write_word(bfType,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(bfSize,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_word(bfReserved1,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_word(bfReserved2,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(nchannels == 1)
  {
    bfOffBits += sizeof(palette);
  }

  if(EOF == write_dword(bfOffBits,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  biSize          = BITMAPINFOHEADER_SIZE;
  biWidth         = width;
  biHeight        = height;
  biPlanes        = 1;
  biBitCount      = (unsigned short)(nchannels << 3);
  biCompression   = BI_RGB;
  biSizeImage     = 0;
  biXPelsPerMeter = 0;
  biYPelsPerMeter = 0;
  biClrUsed       = (nchannels == 1) ? 256 : 0;
  biClrImportant  = (nchannels == 1) ? 256 : 0;

  if(EOF == write_dword(biSize,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biWidth,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biHeight,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_word(biPlanes,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_word(biBitCount,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biCompression,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biSizeImage,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biXPelsPerMeter,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biYPelsPerMeter,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biClrUsed,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(EOF == write_dword(biClrImportant,fo))
  {
    printf("can't write to file\n");
    res = -1;
    goto Exit;
  }

  if(nchannels == 1)
  {
    for(i = 0; i < 256; i += 4)
    {
      palette[i+0] = (Ipp8u)i; /* rgbBlue     */
      palette[i+1] = (Ipp8u)i; /* rgbGreen    */
      palette[i+2] = (Ipp8u)i; /* rgbRed      */
      palette[i+3] = (Ipp8u)0; /* rgbReserved */
    }

    cnt = fwrite(&palette,sizeof(Ipp8u),sizeof(palette),fo);
    if(cnt != sizeof(palette))
    {
      printf("can't write to file\n");
      res = -1;
      goto Exit;
    }
  }

  for(row = 0; row < height; row++)
  {
    outptr = buffer + row * bmp_width;

    cnt = fwrite(outptr,sizeof(Ipp8u),bmp_width,fo);
    if(cnt != (size_t)bmp_width)
    {
      printf("can't write to file\n");
      res = -1;
      goto Exit;
    }
  }

  res = 0;

Exit:

  if(NULL != fo)
  {
    fclose(fo);
  }

  return res;
} /* write_bmp() */


int get_differences(
  const Ipp8u* src,
  const Ipp8u* dst,
  int          width,
  int          height,
  int          nchannels,
  Ipp8u**      dif)
{
  int    i;
  int    j;
  int    c;
  int    res = 0;
  int    step;
  int    size;
  int    diff;
  int    C[4];
  double RL1[4];
  double RL2[4];
  double rl1[4];
  double rl2[4];
  Ipp8u* ptr1;
  Ipp8u* ptr2;
  Ipp8u* ptr3;
  Ipp8u* tmp = NULL;

  step = nchannels * width;

  while(step & 3)
  {
    step++;
  }

  size = step * height;

  tmp = (Ipp8u*)malloc(size);
  if(NULL == tmp)
  {
    res = -1;
    goto Exit;
  }

  memset(tmp,0,size);

  C[0] = 0;
  C[1] = 0;
  C[2] = 0;
  C[3] = 0;

  RL1[0] = 0.0;
  RL1[1] = 0.0;
  RL1[2] = 0.0;
  RL1[3] = 0.0;

  RL2[0] = 0.0;
  RL2[1] = 0.0;
  RL2[2] = 0.0;
  RL2[3] = 0.0;

  rl1[0] = 0.0;
  rl1[1] = 0.0;
  rl1[2] = 0.0;
  rl1[3] = 0.0;

  rl2[0] = 0.0;
  rl2[1] = 0.0;
  rl2[2] = 0.0;
  rl2[3] = 0.0;

  for(i = 0; i < height; i++)
  {
    ptr1 = (Ipp8u*)src + i*step;
    ptr2 = (Ipp8u*)dst + i*step;
    ptr3 = tmp + i*step;
    for(j = 0; j < width; j++)
    {
      for(c = 0; c < nchannels; c++)
      {
        diff = ptr1[c] - ptr2[c];

        if(abs(diff) > C[c])
        {
          C[c] = abs(diff);
        }

        RL1[c] += (double)abs(diff);
        rl1[c] += (double)ptr2[c];

        RL2[c] += (double)(diff*diff);
        rl2[c] += (double)(ptr2[c]*ptr2[c]);

        ptr3[c] = (Ipp8u)abs(diff);
      }
      ptr1 += nchannels;
      ptr2 += nchannels;
      ptr3 += nchannels;
    }
  }

  for(c = 0; c < nchannels; c++)
  {
    printf("channels %d C[%d] L1[%8d] RL1[%2.6f] L2[%8d] RL2[%2.6f]\n",
      c,C[c],(int)RL1[c],RL1[c]/rl1[c],(int)RL2[c],sqrt(RL2[c])/sqrt(rl2[c]));
  }

  *dif = tmp;

Exit:

  return res;
} /* get_differences() */


typedef struct _TEST_PARAM
{
  int   ch;
  JSS   ss;
  char* jpg;
  char* bmp;
} TP;


static int do_auto_test(void)
{
  int   i;
  Ipp64u clk;
  int    res = 0;
  Ipp8u* src    = NULL;
  Ipp8u* dst    = NULL;
  int    width   = 1024;
  int    height  = 768;
  int    quality = 75;
  TP     tp[] =
  {
    { 1, SS_444, "ae_1_444.jpg", "ad_1_444.bmp" },
    { 3, SS_444, "ae_3_444.jpg", "ad_3_444.bmp" },
    { 3, SS_422, "ae_3_422.jpg", "ad_3_422.bmp" },
    { 3, SS_411, "ae_3_411.jpg", "ad_3_411.bmp" },
    { 4, SS_444, "ae_4_444.jpg", "ad_4_444.bmp" },
    { 4, SS_422, "ae_4_422.jpg", "ad_4_422.bmp" },
    { 4, SS_411, "ae_4_411.jpg", "ad_4_411.bmp" }
  };

  src = (Ipp8u*)malloc(width*height*4);
  if(NULL == src)
  {
    printf("can't allocate %d bytes\n",width*height*4);
    res = -1;
    goto Exit;
  }

  srand(0);

  for(i = 0; i < width*height*4; i++)
  {
    src[i] = (Ipp8u)rand();
  }

  for(i = 0; i < 7; i++)
  {
    ijg_encode(src,width,height,tp[i].ch,tp[i].ss,quality,tp[i].jpg,&clk);
    printf("encode %d X %d X %d - %d - %d - cpe %f\n",
      width,height,tp[i].ch,tp[i].ss,quality,(float)clk/(width*height));
  }

  for(i = 0; i < 7; i++)
  {
    ijg_decode(tp[i].jpg,&dst,&width,&height,&tp[i].ch,&tp[i].ss,&clk);
    printf("decode %d X %d X %d - %d - %d - cpe %f\n",
      width,height,tp[i].ch,tp[i].ss,quality,(float)clk/(width*height));

    write_bmp(dst,width,height,tp[i].ch,tp[i].bmp);

    if(NULL != dst)
    {
      free(dst);
      dst = NULL;
    }
  }

Exit:

  if(NULL != src)
  {
    free(src);
  }

  return res;
} /* do_auto_test() */


static int do_through_test(
  char* bmp_name,
  int   quality,
  JSS   sampling)
{
  int    width;
  int    height;
  int    nchannels;
  Ipp64u clk;
  int    res = 0;
  Ipp8u* src = NULL;
  Ipp8u* dst = NULL;
  Ipp8u* dif = NULL;
  char*  diff_name = NULL;

  char  path_buffer[_MAX_PATH];
  char  drive[_MAX_DRIVE];
  char  dir[_MAX_DIR];
  char  fname[_MAX_FNAME];
  char  ext[_MAX_EXT];

  _splitpath(bmp_name,drive,dir,fname,ext);
  sprintf(&path_buffer[0],"%s_%d_%d_diff.bmp",fname,quality,sampling);

  diff_name = &path_buffer[0];

  res = load_bmp(bmp_name,&src,&width,&height,&nchannels);
  if(0 != res)
  {
    goto Exit;
  }

  res = ijg_encode(src,width,height,nchannels,sampling,quality,"through_test.jpg",&clk);
  if(0 != res)
  {
    goto Exit;
  }

  printf("encode %d X %d X %d - %d - %d - cpe %f\n",
    width,height,nchannels,sampling,quality,(float)clk/(width*height));

  res = ijg_decode("through_test.jpg",&dst,&width,&height,&nchannels,&sampling,&clk);
  if(0 != res)
  {
    goto Exit;
  }

  printf("decode %d X %d X %d - %d - %d - cpe %f\n",
    width,height,nchannels,sampling,quality,(float)clk/(width*height));

  res = get_differences(src,dst,width,height,nchannels,&dif);
  if(0 != res)
  {
    goto Exit;
  }

  res = write_bmp(dif,width,height,nchannels,diff_name);
  if(0 != res)
  {
    goto Exit;
  }

Exit:

  if(NULL != src)
  {
    free(src);
  }

  if(NULL != dst)
  {
    free(dst);
  }

  if(NULL != dif)
  {
    free(dif);
  }

  return res;
} /* do_through_test() */


static int do_encode(
  char* bmp_name,
  int   jpeg_quality,
  JSS   jpeg_sampling)
{
  int   res;
  int   width;
  int   height;
  int   nchannels;
  char* jpeg_name;
  Ipp8u* bmp_buff = NULL;

  double  msec;
  double  cpe;

  Ipp32s       ifreq = 0;
  Ipp64u       clocks;

  char  path_buffer[_MAX_PATH];
  char  drive[_MAX_DRIVE];
  char  dir[_MAX_DIR];
  char  fname[_MAX_FNAME];
  char  ext[_MAX_EXT];

#ifdef _DEBUG
  printf("File name: %s\n",bmp_name);
  printf("Quality:   %d\n",jpeg_quality);
  printf("Sampling:  %d\n",jpeg_sampling);
#endif

  res = load_bmp(bmp_name,&bmp_buff,&width,&height,&nchannels);
  if(0 != res)
  {
    goto Exit;
  }

  _splitpath(bmp_name,drive,dir,fname,ext);
  _makepath(path_buffer,drive,dir,fname,"jpg");

  jpeg_name = &path_buffer[0];

  res = ijg_encode(bmp_buff, width, height, nchannels, jpeg_sampling, jpeg_quality, jpeg_name, &clocks);

  ippGetCpuFreqMhz(&ifreq);
  fprintf(stderr, "cpu_freq = %u\n", ifreq);

  msec = (double)(clocks/(Ipp64f)(ifreq*1000));
  cpe  = (double)clocks / (width*height);

  printf("encode %d X %d X %d - %d - %d - clocks %lld\tcpe %8.2f\t msec %8.2f\n",
    width,
    height,
    nchannels,
    jpeg_sampling,
    jpeg_quality,
    clocks,
    cpe,
    msec);

Exit:

  if(NULL != bmp_buff)
  {
    free(bmp_buff);
  }

  return res;
} /* do_encode() */


static int do_decode(
  char* jpg_name)
{
  int    res = 0;
  int    width;
  int    height;
  int    nchannels;
  JSS    ss;
  double  msec;
  double  cpe;
  char*  bmp_name = NULL;
  Ipp8u* bmp_buff = NULL;

  Ipp32s       ifreq = 0;
  Ipp64u       clocks;

  char  path_buffer[_MAX_PATH];
  char  drive[_MAX_DRIVE];
  char  dir[_MAX_DIR];
  char  fname[_MAX_FNAME];
  char  ext[_MAX_EXT];

  _splitpath(jpg_name,drive,dir,fname,ext);
  _makepath(path_buffer,drive,dir,fname,"bmp");

  bmp_name = &path_buffer[0];

  res = ijg_decode(jpg_name,&bmp_buff,&width,&height,&nchannels,&ss,&clocks);
  if(0 != res)
  {
    printf("ijg_decode() failed!\n");
    goto Exit;
  }

  write_bmp(bmp_buff,width,height,nchannels,bmp_name);

  ippGetCpuFreqMhz(&ifreq);
  msec = (double)(clocks/(Ipp64f)(ifreq*1000));

  cpe = (double)clocks / (width*height);

  printf("image: %s %dx%dx%d - %d - clocks %lld\t cpe %f\t msec %6.2f\n",
    jpg_name,
    width,
    height,
    nchannels,
    ss,
    clocks,
    cpe,
    msec);

Exit:

  if(NULL != bmp_buff)
  {
    free(bmp_buff);
  }

  return res;
} /* do_decode() */


int main(int ArgC, char** ArgV)
{
  int   i;
  char  c;
  int   quality  = 75;
  JSS   sampling = SS_444;
  int   show_version = 0;
  int   decode_file  = 0;
  int   encode_file  = 0;
  int   auto_test    = 0;
  int   through_test = 0;
  char* p = NULL;
  char* name = NULL;

  if(ippStsNoErr > ippStaticInit())
  {
    fprintf(stderr,"Can't initialize Intel(R) IPP library\n");
    exit(EXIT_FAILURE);
  }

  if(ArgC < 2)
  {
    copyright();
    usage();
    return 0;
  }

  i = 0;

  do
  {
    p = ArgV[++i];
    if(NULL == p)
    {
      break;
    }

    c = p[0];

    if(c != '-' && c != '/')
    {
      printf("ERROR: Invalid parameter %d - [%s]\n\n",i,p);
      usage();
      return 1;
    }

    c = p[1];

    switch(c)
    {
    case '?':
    case 'h':
      copyright();
      usage();
      break;

    case 'v':
      show_version = 1;
      break;

    case 'a':
      auto_test = 1;
      break;

    case 't':
      through_test = 1;
      name = &p[2];
      break;

    case 'e':
      encode_file = 1;
      if(decode_file)
      {
        printf("WARN: only one decode or encode is possible for time.\n");
        printf("      Last options will be ignored\n\n");
        encode_file = 0;
      }
      name = &p[2];
      break;

    case 'd':
      decode_file = 1;
      if(encode_file)
      {
        printf("WARN: only one decode or encode is possible for time.\n");
        printf("      Last options will be ignored\n\n");
        decode_file = 0;
      }
      name = &p[2];
      break;

    case 'q':
      quality = atoi(&p[2]);
      break;

    case 's':
      sampling = (JSS)atoi(&p[2]);
      break;

    default:
      printf("ERROR: Invalid parameter %d - [%s]\n\n",i,p);
      usage();
      return 1;
    }
  } while(c != 0);

  if(auto_test && (encode_file || decode_file || through_test))
  {
    printf("WARN: auto test must be standalone. Other options will be ignored\n");
    encode_file  = 0;
    decode_file  = 0;
    through_test = 0;
  }

  if(show_version)
  {
    copyright();
    ipp_version();
  }

  if(auto_test)
  {
    do_auto_test();
  }

  if(through_test)
  {
    do_through_test(name,quality,sampling);
  }

  if(encode_file)
  {
    do_encode(name,quality,sampling);
  }

  if(decode_file)
  {
    do_decode(name);
  }

  return 0;
} /* main() */

