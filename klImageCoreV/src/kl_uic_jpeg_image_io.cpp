/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "kl_image_io.h"

#include "precomp.h"
#include "string.h"

#include "uic_exc_status.h"
#include "timer.h"
#include "ippimage.h"
#include "detector.h"
#include "stdfilein.h"
#include "stdfileout.h"
#include "membuffin.h"
#include "membuffout.h"
#include "bmp.h"
#include "png.h"
#include "jpeg.h"
#include "jpeg2k.h"
#include "dicom.h"
#include "jpegxr.h"
#include "raw.h"
#include "cmdline.h"
#include "uic_transcoder_con.h"

#include "ippcore.h"
#include "ippj.h"


CTimer cmnTimer;
using namespace UIC;
typedef struct CMD_OPTIONS
{
  StringA src;
  StringA dst;
  int timing;
  int loops;

  StringA jmode;
  int jpg_quality;
  int nthreads;
  int lossless;
  int sampling;
  int jxr_quality;
  int jxr_aquality;
  int jxr_thread_mode;
  int overlap;
  int alpha_mode;
  int tile_size_x;
  int tile_size_y;
  int tiles_amount[2];
  int tiles_sizes[2];
  int bands;
  int trim;
  int bitstream;
  int bits_shift;
  int ex_channels;
  bool cmyk_direct;
  bool png_filter[5];

  StringA color_format;
  StringA pixel_format;
  int res_width;
  int res_height;
  int data_offset;

  bool verbose_mode;
} CmdOptions;



const char* pxformat_str[] =
{
  "unsigned",
  "fixed",
  "float",
  "signed"
};


const char* color_str[] =
{
  "Unknown",
  "Grayscale",
  "GrayscaleAlpha",
  "RGB",
  "BGR",
  "RGBA",
  "BGRA",
  "RGBAP",
  "BGRAP",
  "RGBE",
  "YCbCr",
  "CMYK",
  "CMYKA",
  "YCCK"
};


const char* sampling_str[] =
{
  "444",
  "422",
  "244",
  "411",
  "Other"
};


const char* format_str[] =
{
  "UNKNOWN",
  "RAW",
  "BMP",
  "JPEG",
  "JPEG2K",
  "DICOM",
  "PNG",
  "JPEG-XR",
};

static IM_TYPE ImageFormatFromExtension(const char* name)
{
  const char* pointPos;
  const char* extension;
  IM_TYPE fmt;

  pointPos = strrchr(name, '.');
  if(!pointPos)
    return IT_UNKNOWN;

  pointPos++;

  if(strpbrk(pointPos, "\"<>|/:*?\\"))
    return IT_UNKNOWN;

  extension = pointPos;

  fmt = IT_UNKNOWN;

  if(0 == strcmp(extension, "raw")) fmt = IT_RAW;
  if(0 == strcmp(extension, "jpg")) fmt = IT_JPEG;
  if(0 == strcmp(extension, "bmp")) fmt = IT_BMP;
  if(0 == strcmp(extension, "jp2")
  || 0 == strcmp(extension, "j2c")) fmt = IT_JPEG2000;
  if(0 == strcmp(extension, "dcm")) fmt = IT_DICOM;
  if(0 == strcmp(extension, "png")) fmt = IT_PNG;
  if(0 == strcmp(extension, "wdp")
  || 0 == strcmp(extension, "hdp")
  || 0 == strcmp(extension, "jxr")) fmt = IT_JPEGXR;

  return fmt;
} // ImageFormatFromExtension()

static ExcStatus DecodeImage(BaseStreamInput& in, CIppImage& image, CmdOptions& cmdOpt, IM_TYPE& fmt, double* msec)
{
  IM_ERROR  imStatus;

  if(msec) cmnTimer.Init();

  switch(fmt)
  {
    case IT_JPEG:
    {
      PARAMS_JPEG params_jpeg;

      params_jpeg.color            = IC_UNKNOWN;
      params_jpeg.comment_size     = 0;
      params_jpeg.dct_scale        = 0;
      params_jpeg.mode             = 1;
      params_jpeg.nthreads         = (cmdOpt.nthreads == 0)?1:cmdOpt.nthreads;
      params_jpeg.sampling         = IS_444;
      params_jpeg.use_qdct         = 0;
      params_jpeg.tmode            = (cmdOpt.timing == 1) ? 1 : 0;

      if(msec) cmnTimer.Start();
      imStatus = ReadImageJPEG(in, params_jpeg, image);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_JPEG2000:
    {
      PARAMS_JPEG2K params_jpeg2k;

      params_jpeg2k.nthreads   =(cmdOpt.nthreads == 0)?1:cmdOpt.nthreads;
      params_jpeg2k.arithmetic = J2K_16;

      if(msec) cmnTimer.Start();
      imStatus = ReadImageJPEG2000(in, params_jpeg2k, image, cmdOpt.verbose_mode);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_BMP:
    {
      PARAMS_BMP params_bmp;

      int order_c3[3] = { 2, 1, 0 };
      int order_c4[4] = { 2, 1, 0, 3};

      if(msec) cmnTimer.Start();
      imStatus = ReadImageBMP(in, params_bmp, image);
      if(msec) cmnTimer.Stop();

      if(imStatus != IE_OK)
        break;

      if(image.NChannels() == 3)
      {
        image.SwapChannels(order_c3);
        image.Color(IC_RGB);
      }
      else if(image.NChannels() == 4)
      {
        image.SwapChannels(order_c4);
        image.Color(IC_RGBA);
      }
    }
    break;

    case IT_PNG:
    {
      PARAMS_PNG params_png;

      if(msec) cmnTimer.Start();
      imStatus = ReadImagePNG(in, params_png, image);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_DICOM:
    {
      PARAMS_DICOM params_dicom;

      params_dicom.nFrames   = 1;
      params_dicom.frameSeek = 0;

      if(msec) cmnTimer.Start();
      imStatus = ReadImageDICOM(in, params_dicom, image, cmdOpt.verbose_mode);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_JPEGXR:
    {
      PARAMS_JPEGXR params_jpegxr;

      params_jpegxr.threads     = cmdOpt.nthreads;
      params_jpegxr.thread_mode = cmdOpt.jxr_thread_mode;
      params_jpegxr.bands       = cmdOpt.bands;

      if(msec) cmnTimer.Start();
      imStatus = ReadImageJPEGXR(in, params_jpegxr, image);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_RAW:
    {
        PARAMS_RAW params_raw;
        params_raw.color_format = cmdOpt.color_format;
        params_raw.data_offset  = cmdOpt.data_offset;
        params_raw.pixel_format = cmdOpt.pixel_format;
        params_raw.res_height   = cmdOpt.res_height;
        params_raw.res_width    = cmdOpt.res_width;

        if(msec) cmnTimer.Start();
        imStatus = ReadImageRAW(in, params_raw, image);
        if(msec) cmnTimer.Stop();
    }
    break;

    default:
      return ExcStatusFail;
  }

  if(imStatus != IE_OK)
  {
    //PrintError(imStatus, fmt, PRINT_DEC);
    return ExcStatusFail;
  }

  if(msec) *msec = cmnTimer.GetTime(CTimer::msec);

  return ExcStatusOk;
} // DecodeImage()

static ExcStatus EncodeImage(CIppImage& image, BaseStreamOutput& out, CmdOptions& cmdOpt, IM_TYPE& fmt, double* msec)
{
  IM_ERROR  imStatus;

  if(msec) cmnTimer.Init();

  if(IT_JPEG != fmt && IT_JPEG2000 != fmt)
  {
    cmdOpt.lossless = 0;
  }
  else
  {
    if(image.Precision() > 8)
      cmdOpt.lossless = 1;
  }

  switch(fmt)
  {
    case IT_JPEG:
    {
      PARAMS_JPEG params_jpeg;

	  //jpeg encoder mode: b - BASELINE (8-bit lossy); e - EXTENDED (12-bit lossy); p - PROGRESSIVE  (8-bit progressive); l - LOSSLESS (2-16 bit lossless)
      
	  params_jpeg.mode = 1;
	  cmdOpt.jpg_quality=100;
	  

	  if(0 == strncmp(cmdOpt.jmode, "b", 1)) params_jpeg.mode = 1;
      if(0 == strncmp(cmdOpt.jmode, "e", 1)) params_jpeg.mode = 2;
      if(0 == strncmp(cmdOpt.jmode, "p", 1)) params_jpeg.mode = 3;
      if(0 == strncmp(cmdOpt.jmode, "l", 1)) params_jpeg.mode = 4;

      params_jpeg.comment_size     = 0;
      params_jpeg.huffman_opt      = 0;
      params_jpeg.nthreads         = (cmdOpt.nthreads == 0)?1:cmdOpt.nthreads;
      params_jpeg.point_transform  = 0;
      params_jpeg.predictor        = 1;
      params_jpeg.quality          = cmdOpt.jpg_quality;
      params_jpeg.restart_interval = (cmdOpt.timing == 1) ? 1 : 0;
      params_jpeg.sampling         = (cmdOpt.sampling == IS_444) ? IS_444 : (cmdOpt.sampling == IS_422) ? IS_422 : IS_411;
      params_jpeg.tmode            = (cmdOpt.timing == 1) ? 1 : 0;

      if(msec) cmnTimer.Start();
      imStatus = SaveImageJPEG(image, params_jpeg, out);
	  int didresetNum;
	  ippGetNumThreads(&didresetNum);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_JPEG2000:
    {
      PARAMS_JPEG2K  params_jpeg2k;

      params_jpeg2k.nthreads   = (cmdOpt.nthreads == 0)?1:cmdOpt.nthreads;
      params_jpeg2k.useMCT     = (cmdOpt.lossless == 0) ? 1 : 0;
      params_jpeg2k.quality    = cmdOpt.jpg_quality;
      params_jpeg2k.color      = image.Color();
      params_jpeg2k.mode       = (cmdOpt.lossless == 0) ? Lossy : Lossless;
      params_jpeg2k.arithmetic = J2K_16;
      params_jpeg2k.wt         = (cmdOpt.lossless == 0) ?  WT97 : WT53;

      if(msec) cmnTimer.Start();
      imStatus = SaveImageJPEG2000(image, params_jpeg2k, out);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_BMP:
    {
      PARAMS_BMP     params_bmp;

      if(8 < image.Precision())
      {
        CIppImage reduced(image.Size(), image.NChannels(),8, 1);

        image.ReduceBits16(reduced, reduced.Step(), reduced.Size());

        if(msec) cmnTimer.Start();
        imStatus = SaveImageBMP(reduced, params_bmp, out);
        if(msec) cmnTimer.Stop();

        reduced.Free();
      }
      else
      {
        if(msec) cmnTimer.Start();
        imStatus = SaveImageBMP(image, params_bmp, out);
        if(msec) cmnTimer.Stop();
      }
    }
    break;

    case IT_PNG:
    {
      PARAMS_PNG params_png;

      params_png.filters[0] = cmdOpt.png_filter[0];
      params_png.filters[1] = cmdOpt.png_filter[1];
      params_png.filters[2] = cmdOpt.png_filter[2];
      params_png.filters[3] = cmdOpt.png_filter[3];
      params_png.filters[4] = cmdOpt.png_filter[4];

      if(msec) cmnTimer.Start();
      imStatus = SaveImagePNG(image, params_png, out);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_JPEGXR:
    {
      PARAMS_JPEGXR params_jpegxr;

      params_jpegxr.quality          = cmdOpt.jxr_quality;
      params_jpegxr.aquality         = cmdOpt.jxr_aquality;
      params_jpegxr.overlap          = cmdOpt.overlap;
      params_jpegxr.bands            = cmdOpt.bands;
      params_jpegxr.bitstream        = cmdOpt.bitstream;
      params_jpegxr.sampling         = cmdOpt.sampling;
      params_jpegxr.trim             = cmdOpt.trim;
      params_jpegxr.bits_shift       = cmdOpt.bits_shift;
      params_jpegxr.alpha_mode       = cmdOpt.alpha_mode;
      params_jpegxr.cmyk_direct      = cmdOpt.cmyk_direct;
      params_jpegxr.tiles_uniform[0] = cmdOpt.tiles_amount[0];
      params_jpegxr.tiles_uniform[1] = cmdOpt.tiles_amount[1];
      params_jpegxr.tiles_uniform[2] = cmdOpt.tiles_sizes[0];
      params_jpegxr.tiles_uniform[3] = cmdOpt.tiles_sizes[1];

      if(msec) cmnTimer.Start();
      imStatus = SaveImageJPEGXR(image, params_jpegxr, out);
      if(msec) cmnTimer.Stop();
    }
    break;

    case IT_RAW:
    {
      PARAMS_RAW params_raw;

      if(msec) cmnTimer.Start();
      imStatus = SaveImageRAW(image, params_raw, out);
      if(msec) cmnTimer.Stop();
    }
    break;

  default:
    return ExcStatusFail;
  }

  if(imStatus != IE_OK)
  {
    //PrintError(imStatus, fmt, PRINT_ENC);
    return ExcStatusFail;
  }

  if(msec) *msec = cmnTimer.GetTime(CTimer::msec);

  return ExcStatusOk;
} // EncodeImage()

int runJPEG(const char* fileName,bool inout,const char* fileID)
{
	int               i;
	double            msec;
	double            decTime;
	double            encTime;
	Ipp8u*            buf;
	IM_TYPE           fmtIn;
	IM_TYPE           fmtOut;
	ExcStatus         res;
	CIppImage         image;
	CommandLine       cmdline;
	CFormatDetector   detector;
	BaseStream::TSize cnt;
	BaseStream::TSize size;
	bool              help_flag;
	bool              is_raw = false;

	CStdFileInput   fi;
	CStdFileOutput  fo;
	CMemBuffInput   mi;
	CMemBuffOutput  mo;

	CmdOptions cmdOptions;

	cmdOptions.dst = "JPEG";
	cmdOptions.timing = 1;
	cmdOptions.loops = 1;
	cmdOptions.nthreads =12;

	ippStaticInit();
	fmtOut = IT_JPEG;
	
	cmdOptions.dst="out.jpg";
	cmdOptions.src= fileName;

	if(IT_UNKNOWN == fmtOut || IT_DICOM == fmtOut)
	{
		printf("Unsupported output image format!\n");
		return -1;
	}

	if(!BaseStream::IsOk(fi.Open(fileName)))
	{
		printf("Can not open input file!\n");
		return 1;
	}

	if(!BaseStream::IsOk(fo.Open(cmdOptions.dst)))
	{
		printf("Can not open output file!\n");
		return 1;
	}

	if(is_raw)
		fmtIn = IT_RAW;
	else
		fmtIn = detector.ImageFormat(fi);
	if(IT_UNKNOWN == fmtIn)
	{
		printf("Unsupported input image format!\n");
		return -1;
	}

	msec = 0.0;

	fi.Size(size);

	buf = (Ipp8u*)ippMalloc((int)size);
	if(0 == buf)
		return 1;

	res = fi.Read(buf, size, cnt);
	if(!IsOk(res))
		return 1;

	for(i = 0; i < cmdOptions.loops; i++)
	{
		mi.Open(buf, (int)size);

		res = DecodeImage(mi, image, cmdOptions, fmtIn, &decTime);
		if(!IsOk(res))
			return 1;

		msec += decTime;

		mi.Close();


		ippFree(buf);

		decTime = msec / cmdOptions.loops;
	}

	//cout<<"Decode, " <<(const char*)cmdOptions.src<<","<<image.Width()*image.Height()*image.NChannels()<<","<<decTime/1000.0<<endl;
	cout<<"Decode, " <<fileID<<","<<image.Width()*image.Height()*image.NChannels()<<","<<decTime/1000.0<<endl;
	BaseStream::TPosition pos = 0;

	msec = 0.0;
	size = image.Step()*image.Height()*2;

	buf = (Ipp8u*)ippMalloc((int)size);
	if(0 == buf)
		return 1;

	for(i = 0; i < cmdOptions.loops; i++)
	{
		mo.Open(buf, (int)size);

		res = EncodeImage(image, mo, cmdOptions, fmtOut, &encTime);
		if(!IsOk(res))
			return 1;

		msec += encTime;

		mo.Position(pos);

		mo.Close();
	}

	fo.Write(buf, pos, cnt);

	ippFree(buf);

	encTime = msec / cmdOptions.loops;
	//cout<<"Encode, " <<(const char*)cmdOptions.dst<<","<<image.Width()*image.Height()*image.NChannels()<<","<<encTime/1000.0<<endl;
	cout<<"Encode, " <<fileID<<","<<image.Width()*image.Height()*image.NChannels()<<","<<encTime/1000.0<<endl;
	return 0;
}

bool read_jpeg (const char* filename,unsigned int width,unsigned int height,unsigned int bands, unsigned char * inputbuf)
{
	return false;

}

bool query_jpeg (const char* filename,unsigned int &width,unsigned int &height,unsigned int &bands)
{
	return false;
}


