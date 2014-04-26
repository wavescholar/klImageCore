/*
//
//                INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

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

// Common timer object for "-t" option. It's initialization on Linux takes long.
CTimer cmnTimer;

static void copyright_(void)
{
  const IppLibraryVersion* ippj = ippjGetLibVersion();

  printf("Intel(R) Integrated Performance Primitives\n");
  printf("  version: %s, [%d.%d.%d.%d]\n",
    ippj->Version, ippj->major, ippj->minor, ippj->build, ippj->majorBuild);
  printf("  name:    %s\n", ippj->Name);
  printf("  date:    %s\n", ippj->BuildDate);

  return;
} // copyright_()


static void PrintError(IM_ERROR imStatus, IM_TYPE mtCodec, Ipp8u bMode)
{
  printf(format_str[mtCodec]);

  if(bMode == PRINT_DEC)
    printf(" decoder ");
  else
    printf(" encoder ");

  printf("error: ");

  switch(imStatus)
  {
  case IE_OK:
    printf("no error (?!) \n");
    break;
  case IE_RHEADER:
    printf("can not read header \n");
    break;
  case IE_RDATA:
    printf("can not read data \n");
    break;
  case IE_WHEADER:
    printf("can not write header \n");
    break;
  case IE_WDATA:
    printf("can not write data \n");
    break;
  case IE_RESET:
    printf("can not reset codec \n");
    break;
  case IE_ALLOC:
    printf("can not allocate memory \n");
    break;
  case IE_INIT:
    printf("can not initialize codec \n");
    break;
  case IE_ASTREAM:
    printf("can not attach stream \n");
    break;
  case IE_AIMAGE:
    printf("can not attach image \n");
    break;
  case IE_PARAMS:
    printf("codec parameters set error \n");
    break;
  case IE_UNSUP:
    printf("unsupported image format \n");
    break;
  case IE_FOPEN:
    printf("file open error \n");
    break;
  case IE_FCLOSE:
    printf("file close error \n");
    break;
  case IE_FSEEK:
    printf("file seek error \n");
    break;
  case IE_FREAD:
    printf("file read error \n");
    break;
  case IE_FWRITE:
    printf("file write error \n");
    break;
  case IE_PROCESS:
    printf("image process error \n");
    break;
  case IE_UNDEF:
  default:
    printf("something mysterious \n");
    break;
  }
}

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
    PrintError(imStatus, fmt, PRINT_DEC);
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
    PrintError(imStatus, fmt, PRINT_ENC);
    return ExcStatusFail;
  }

  if(msec) *msec = cmnTimer.GetTime(CTimer::msec);

  return ExcStatusOk;
} // EncodeImage()


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


static void CheckParam(Ipp32s &iValue, Ipp32s iMin, Ipp32s iMax)
{
  if(iMax == iMin)
    iValue = iMax;

  if(iValue > iMax)
    iValue = iMax;
  else if(iValue < iMin)
    iValue = iMin;
} // CheckParam()


int main(int argc, char* argv[])
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
  cmdOptions.loops = 1;
  ippStaticInit();

  copyright_();

  // Common params
  cmdline.AddKey("i","input file name", cmdOptions.src, 1, (StringA)"uic_test_image.jpg", IT_UNKNOWN);
  cmdline.AddKey("o","output file name (out.jp2 - default)", cmdOptions.dst, 1, (StringA)"out.jp2", IT_UNKNOWN);
  cmdline.AddKey("r","treat input image as raw (0 - default)", is_raw, 0, 0, IT_UNKNOWN);
  cmdline.AddKey("t","advanced timing, 0 - disable (default); 1 - enable", cmdOptions.timing, 1, 0, IT_UNKNOWN);
  cmdline.AddKey("m","number of loops for advanced timing (1 - default)", cmdOptions.loops, 1, 1, IT_UNKNOWN);
  cmdline.AddKey("d", "print problem diagnostics messages", cmdOptions.verbose_mode, 0, 0, IT_UNKNOWN);
  cmdline.AddKey("n","number of threads", cmdOptions.nthreads, 1, 0, IT_UNKNOWN);
  cmdline.AddKey("h","help key", help_flag, 0, 0, IT_UNKNOWN);

  cmdline.AddKey("W","width resolution", cmdOptions.res_width, 1, 0, IT_RAW);
  cmdline.AddKey("H","height resolution", cmdOptions.res_height, 1, 0, IT_RAW);
  cmdline.AddKey("D","data offset", cmdOptions.data_offset, 1, 0, IT_RAW);
  cmdline.AddKey("C","color format: gray, rgb (default), rgba, cmyk", cmdOptions.color_format, 1, (StringA)"rgb", IT_RAW);
  cmdline.AddKey("P","pixel format: 8u (default), 16u, 16s, 32u, 32s, 32f", cmdOptions.pixel_format, 1, (StringA)"8u", IT_RAW);

  cmdline.AddKey("q","quality [1...100]", cmdOptions.jpg_quality, 1, 100, IT_JPEG);
  cmdline.AddKey("s","sampling, 0 - 444 (default); 1 - 422; 2 - 411", cmdOptions.sampling, 1, 0, IT_JPEG);
  cmdline.AddKey("j","jpeg encoder mode: b - BASELINE (8-bit lossy); e - EXTENDED (12-bit lossy); p - PROGRESSIVE  (8-bit progressive); l - LOSSLESS (2-16 bit lossless)", cmdOptions.jmode, 1, (StringA)"b", IT_JPEG);

  cmdline.AddKey("q","quality [1...100]", cmdOptions.jpg_quality, 1, 100, IT_JPEG2000);
  cmdline.AddKey("l","encoding mode, 0 - lossy encode (default); 1 - lossless encoding", cmdOptions.lossless, 1, 0, IT_JPEG2000);

  cmdline.AddKey("f","activate all filters", cmdOptions.png_filter[4], 0, 0, IT_PNG);
  cmdline.AddKey("fs","add sub filter", cmdOptions.png_filter[0], 0, 0, IT_PNG);
  cmdline.AddKey("fu","add up filter", cmdOptions.png_filter[1], 0, 0, IT_PNG);
  cmdline.AddKey("fa","add average filter", cmdOptions.png_filter[2], 0, 0, IT_PNG);
  cmdline.AddKey("fp","add Paeth filter", cmdOptions.png_filter[3], 0, 0, IT_PNG);

  cmdline.AddKey("q","quality [1...255] (1 - lossless, default; less is better)", cmdOptions.jxr_quality, 1, 1, IT_JPEGXR);
  cmdline.AddKey("Q","planar alpha quality [1...255] (1 - lossless, default; less is better)", cmdOptions.jxr_aquality, 1, 1, IT_JPEGXR);
//  cmdline.AddKey("s","sampling, 0 - 444 (default); 1 - 422; 2 - 420", cmdOptions.sampling, 1, 0, IT_JPEGXR);
//  cmdline.AddKey("f","bitstream order, 0 - spatial (default); 1 - frequency", cmdOptions.bitstream, 1, 0, IT_JPEGXR);
  cmdline.AddKey("l","overlap mode, 0 - no overlap (default); 1 -  overlap one; 2 - overlap two", cmdOptions.overlap, 1, 0, IT_JPEGXR);
  cmdline.AddKey("a","alpha mode, 0 - interleaved alpha (default); 1 - planar alpha", cmdOptions.alpha_mode, 1, 0, IT_JPEGXR);
  cmdline.AddKey("F","trim flexbits [0...15] (0 - all flexbits, default; 15 - no flexbits)", cmdOptions.trim, 1, 0, IT_JPEGXR);
  cmdline.AddKey("b","bands mode, 0 - all bands (default); 1 - no flexbits; 2 - no highpass; 3 - dc only", cmdOptions.bands, 1, 0, IT_JPEGXR);
  cmdline.AddKey("c","encode CMYK images directly, without color conversion", cmdOptions.cmyk_direct, 0, 0, IT_JPEGXR);
  cmdline.AddKey("v","color conversion bits shift value for 16-bit and 32-bit images [0...32]", cmdOptions.bits_shift, 1, -1, IT_JPEGXR);
  cmdline.AddKey("U","amount of tiles per rows and columns (1 1 - default)", cmdOptions.tiles_amount[0], 2, 1, IT_JPEGXR);
  cmdline.AddKey("X","maximum tiles size in macroblocks (0 0 - default; 1 = 16px; override U argument)", cmdOptions.tiles_sizes[0], 2, 0, IT_JPEGXR);
#ifdef USE_TBB
  cmdline.AddKey("u","multithread mode, 0 - pipeline threading (threads limited to 3); 1 - tile threading (threads limited to number of tiles); 2 - mixed threading (default; threads limited to number of tiles + 3)", cmdOptions.jxr_thread_mode, 1, 2, IT_JPEGXR);
#endif

  if(argc == 1)
  {
    cmdline.HelpMessage(format_str);
    return -1;
  }

  if(!cmdline.Parse(argv, argc, IT_UNKNOWN, IT_UNKNOWN))
  {
    cmdline.HelpMessage(format_str);
    printf("Invalid argument!\n");
    return -1;
  }

  if(help_flag)
    cmdline.HelpMessage(format_str);

  fmtOut = ImageFormatFromExtension((const char*)cmdOptions.dst);
  if(IT_UNKNOWN == fmtOut || IT_DICOM == fmtOut)
  {
    printf("Unsupported output image format!\n");
    return -1;
  }

  if(!BaseStream::IsOk(fi.Open(cmdOptions.src)))
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

  if(!cmdline.Parse(argv, argc, fmtIn, fmtOut))
  {
    if(!help_flag)
      cmdline.HelpMessage(format_str);
    printf("Invalid argument!\n");
    return -1;
  }

  CheckParam(cmdOptions.timing, 0, 1);
  CheckParam(cmdOptions.loops,  1, IPP_MAX_16U);

  if(!cmdOptions.timing)
  {
    res = DecodeImage(fi, image, cmdOptions, fmtIn, NULL);
    if(!IsOk(res))
    {
      printf("Error in DecodeImage finction!\n");
      return 1;
    }
  }
  else
  {
    msec = 0.0;

    fi.Size(size);

    buf = (Ipp8u*)ippMalloc((int)size);
    if(0 == buf)
      return 1;

    res = fi.Read(buf, size, cnt);
     if(!IsOk(res))
      return 1;
#if !defined(__WIN32)
      printf("WARNING: initialization of CTimer may take long!\n");
#endif

    for(i = 0; i < cmdOptions.loops; i++)
    {
      mi.Open(buf, (int)size);

      res = DecodeImage(mi, image, cmdOptions, fmtIn, &decTime);
      if(!IsOk(res))
        return 1;

      msec += decTime;

      mi.Close();
    }

    ippFree(buf);

    decTime = msec / cmdOptions.loops;
  }

  printf("image: %s, %dx%dx%d, %d-bits %s, color: %s, sampling: %s\n",
    (const char*)cmdOptions.src,
    image.Width(),
    image.Height(),
    image.NChannels(),
    image.Precision(),
    pxformat_str[image.Format()],
    color_str[image.Color()],
    sampling_str[image.Sampling()]);

  if(cmdOptions.timing) printf("decode time: %.2f msec\n", decTime);


  if(!cmdOptions.timing)
  {
    res = EncodeImage(image, fo, cmdOptions, fmtOut, NULL);
    if(!IsOk(res))
    {
      printf("Error in EncodeImage finction!\n");
      return 1;
    }
  }
  else
  {
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
  }

  if(cmdOptions.timing) printf("encode time: %.2f msec\n", encTime);

  return 0;
} // main()

