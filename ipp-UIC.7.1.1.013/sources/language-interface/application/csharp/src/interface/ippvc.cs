/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
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

// generated automatically on Wed Feb  3 17:35:41 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

   using IppVCHuffmanSpec_32s = System.Int32;

//
// enums
//
   public enum IPPVC_MC_APX {
      IPPVC_MC_APX_FF = 0,
      IPPVC_MC_APX_FH = 4,
      IPPVC_MC_APX_HF = 8,
      IPPVC_MC_APX_HH = 12,
   };
   public enum IppvcFrameFieldFlag {
      IPPVC_FRAME = 0,
      IPPVC_TOP_FIELD = 1,
      IPPVC_BOTTOM_FIELD = 2,
   };
   public enum IppIntra4x4PredMode_H264 {
      IPP_4x4_VERT = 0,
      IPP_4x4_HOR = 1,
      IPP_4x4_DC = 2,
      IPP_4x4_DIAG_DL = 3,
      IPP_4x4_DIAG_DR = 4,
      IPP_4x4_VR = 5,
      IPP_4x4_HD = 6,
      IPP_4x4_VL = 7,
      IPP_4x4_HU = 8,
      IPP_4x4_DC_TOP = 9,
      IPP_4x4_DC_LEFT = 10,
      IPP_4x4_DC_128 = 11,
   };
   public enum IppvcNoiseBlurFlag {
      IPPVC_NOISE_BLUR0 = 0,
      IPPVC_NOISE_BLUR1 = 1,
      IPPVC_NOISE_BLUR2 = 2,
      IPPVC_NOISE_BLUR3 = 3,
   };
   public enum IppIntra16x16PredMode_H264 {
      IPP_16X16_VERT = 0,
      IPP_16X16_HOR = 1,
      IPP_16X16_DC = 2,
      IPP_16X16_PLANE = 3,
      IPP_16X16_DC_TOP = 4,
      IPP_16X16_DC_LEFT = 5,
      IPP_16X16_DC_128 = 6,
   };
   public enum IPPVC_MV_TYPE {
      IPPVC_MV_FIELD = 0,
      IPPVC_MV_FRAME = 1,
   };
   public enum IPPVC_ESCAPE_FLAG {
      IPPVC_EF_NONE = 0,
      IPPVC_EF_REVERSIBLE_VLC = 1,
      IPPVC_EF_SHORT_HEADER = 2,
   };
   public enum IppIntraChromaPredMode_H264 {
      IPP_CHROMA_DC = 0,
      IPP_CHROMA_HOR = 1,
      IPP_CHROMA_VERT = 2,
      IPP_CHROMA_PLANE = 3,
      IPP_CHROMA_DC_TOP = 4,
      IPP_CHROMA_DC_LEFT = 5,
      IPP_CHROMA_DC_128 = 6,
   };
   //typedef IppIntraChromaPredMode_H264 IppIntraChromaPredMode_AVS;

   public enum IppIntra8x8PredMode_H264 {
      IPP_8x8_VERT = 0,
      IPP_8x8_HOR = 1,
      IPP_8x8_DC = 2,
      IPP_8x8_DIAG_DL = 3,
      IPP_8x8_DIAG_DR = 4,
      IPP_8x8_VR = 5,
      IPP_8x8_HD = 6,
      IPP_8x8_VL = 7,
      IPP_8x8_HU = 8,
      IPP_8x8_DC_TOP = 9,
      IPP_8x8_DC_LEFT = 10,
      IPP_8x8_DC_128 = 11,
   };
   //typedef IppIntra8x8PredMode_H264 IppIntra8x8PredMode_AVS;

//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeinterlaceBlendSpec_8u_C2 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDeinterlaceBlendState_8u_C1 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDenoiseAdaptiveState_8u_C1 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDenoiseMosquitoState_8u_C1 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiDenoiseSmoothState_8u_C1 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiQuantInterSpec_MPEG4 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiQuantIntraSpec_MPEG4 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiQuantInvInterSpec_MPEG4 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiQuantInvIntraSpec_MPEG4 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiWarpSpec_MPEG4 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppvcCABACState {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCBidir_16u {
      public ushort* pSrc1;
      public int srcStep1;
      public ushort* pSrc2;
      public int srcStep2;
      public ushort* pDst;
      public int dstStep;
      public IppiSize roiSize;
      public int bitDepth;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppVCWeightParams_8u {
      public int ulog2wd;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
      public sbyte[] iWeight;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
      public sbyte[] iOffset;
   public IppVCWeightParams_8u(int ulog2wd) {
      this.ulog2wd = ulog2wd;
      this.iWeight = new sbyte[2];
      this.iOffset = new sbyte[2];
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCInterpolateBlockIC_8u {
      public byte* pSrc;
      public int srcStep;
      public byte* pDst;
      public int dstStep;
      public byte* pLUTTop;
      public byte* pLUTBottom;
      public IppiSize sizeFrame;
      public IppiSize sizeBlock;
      public IppiPoint pointRefBlockPos;
      public IppiPoint pointVectorQuarterPix;
      public uint oppositePadding;
      public uint fieldPrediction;
      public uint roundControl;
      public uint isPredBottom;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppMotionVector {
      public short dx;
      public short dy;
   public IppMotionVector ( short dx, short dy ) {
      this.dx = dx;
      this.dy = dy;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCInterpolate_8u {
      public byte* pSrc;
      public int srcStep;
      public byte* pDst;
      public int dstStep;
      public int dx;
      public int dy;
      public IppiSize roiSize;
      public int roundControl;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppiReconstructHighMB_16s8u {
      public short** ppSrcDstCoeff;
      public byte* pSrcDstPlane;
      public int srcDstStep;
      public uint cbp;
      public int qp;
      public short* pQuantTable;
      public int bypassFlag;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCInterpolateBlock_8u {
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
      public byte*[] pSrc;
      public int srcStep;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
      public byte*[] pDst;
      public int dstStep;
      public IppiSize sizeFrame;
      public IppiSize sizeBlock;
      public IppiPoint pointBlockPos;
      public IppiPoint pointVector;
      public IppVCInterpolateBlock_8u(int srcStep, int dstStep)
      {
         pSrc = new byte*[2];
         this.srcStep = srcStep;
         pDst = new byte*[2];
         this.dstStep = dstStep;
         sizeFrame = new IppiSize();
         sizeBlock = new IppiSize();
         pointBlockPos = new IppiPoint();
         pointVector = new IppiPoint();
      }
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppiFilterDeblock_16u {
      public ushort* pSrcDstPlane;
      public int srcDstStep;
      public short* pAlpha;
      public short* pBeta;
      public short* pThresholds;
      public byte* pBs;
      public int bitDepth;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppVCHuffmanSpec_32u {
      public uint code;
      public uint len;
   public IppVCHuffmanSpec_32u ( uint code, uint len ) {
      this.code = code;
      this.len = len;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppiReconstructHighMB_32s16u {
      public int** ppSrcDstCoeff;
      public ushort* pSrcDstPlane;
      public int srcDstStep;
      public uint cbp;
      public int qp;
      public short* pQuantTable;
      public int bypassFlag;
      public int bitDepth;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCWeightBlock_8u {
      public byte* pSrc1;
      public int srcStep1;
      public byte* pSrc2;
      public int srcStep2;
      public byte* pDst;
      public int dstStep;
      public IppiSize roiSize;
   public IppVCWeightBlock_8u(byte* pSrc1, int srcStep1, byte* pSrc2, int srcStep2, byte* pDst, int dstStep, IppiSize roiSize ) {
      this.pSrc1 = pSrc1;
      this.srcStep1 = srcStep1;
      this.pSrc2 = pSrc2;
      this.srcStep2 = srcStep2;
      this.pDst = pDst;
      this.dstStep = dstStep;
      this.roiSize = roiSize;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCInterpolateBlock_16u {
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
      public ushort*[] pSrc;
      public int srcStep;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
      public ushort*[] pDst;
      public int dstStep;
      public IppiSize sizeFrame;
      public IppiSize sizeBlock;
      public IppiPoint pointBlockPos;
      public IppiPoint pointVector;
      public int bitDepth;
      public IppVCInterpolateBlock_16u(int srcStep, int dstStep, int bitDepth)
      {
         pSrc = new ushort*[2];
         this.srcStep = srcStep;
         pDst = new ushort*[2];
         this.dstStep = dstStep;
         sizeFrame = new IppiSize();
         sizeBlock = new IppiSize();
         pointBlockPos = new IppiPoint();
         pointVector = new IppiPoint();
         this.bitDepth = bitDepth;
      }
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppiFilterDeblock_8u {
      public byte* pSrcDstPlane;
      public int srcDstStep;
      public byte* pAlpha;
      public byte* pBeta;
      public byte* pThresholds;
      public byte* pBs;
   public IppiFilterDeblock_8u(byte* pSrcDstPlane, int srcDstStep, byte* pAlpha, byte* pBeta, byte* pThresholds, byte* pBs) {
      this.pSrcDstPlane = pSrcDstPlane;
      this.srcDstStep = srcDstStep;
      this.pAlpha = pAlpha;
      this.pBeta = pBeta;
      this.pThresholds = pThresholds;
      this.pBs = pBs;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppDenoiseCAST {
      public byte TemporalDifferenceThreshold;
      public byte NumberOfMotionPixelsThreshold;
      public byte StrongEdgeThreshold;
      public byte BlockWidth;
      public byte BlockHeight;
      public byte EdgePixelWeight;
      public byte NonEdgePixelWeight;
      public byte GaussianThresholdY;
      public byte GaussianThresholdUV;
      public byte HistoryWeight;
   public IppDenoiseCAST ( byte TemporalDifferenceThreshold, byte NumberOfMotionPixelsThreshold, byte StrongEdgeThreshold, byte BlockWidth, byte BlockHeight, byte EdgePixelWeight, byte NonEdgePixelWeight, byte GaussianThresholdY, byte GaussianThresholdUV, byte HistoryWeight ) {
      this.TemporalDifferenceThreshold = TemporalDifferenceThreshold;
      this.NumberOfMotionPixelsThreshold = NumberOfMotionPixelsThreshold;
      this.StrongEdgeThreshold = StrongEdgeThreshold;
      this.BlockWidth = BlockWidth;
      this.BlockHeight = BlockHeight;
      this.EdgePixelWeight = EdgePixelWeight;
      this.NonEdgePixelWeight = NonEdgePixelWeight;
      this.GaussianThresholdY = GaussianThresholdY;
      this.GaussianThresholdUV = GaussianThresholdUV;
      this.HistoryWeight = HistoryWeight;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] unsafe public struct IppVCInterpolate_16u {
      public ushort* pSrc;
      public int srcStep;
      public ushort* pDst;
      public int dstStep;
      public int dx;
      public int dy;
      public IppiSize roiSize;
      public int bitDepth;
   };

unsafe public class vc {

   internal const string libname = "ippvc-7.1.dll";

   static public int IPPVC_VLC_FORBIDDEN = 0xf0f1;
   static public int IPPVC_ESCAPE        = 0x00ff;
   static public int IPPVC_ENDOFBLOCK    = 0x00fe;
   static public int IPPVC_FRAME_PICTURE = 0x0003;
   
   static public int IPPVC_LEFT_EDGE      = 0x1;
   static public int IPPVC_RIGHT_EDGE     = 0x2;
   static public int IPPVC_TOP_EDGE       = 0x4;
   static public int IPPVC_BOTTOM_EDGE    = 0x8;
   static public int IPPVC_TOP_LEFT_EDGE  = 0x10;
   static public int IPPVC_TOP_RIGHT_EDGE = 0x20;

   static public int IPPVC_CBP_1ST_CHROMA_DC_BITPOS = 17;
   static public int IPPVC_CBP_1ST_CHROMA_AC_BITPOS = 19;
   static public int IPPVC_CBP_CHROMA_DC            = (0x3<<IPPVC_CBP_1ST_CHROMA_DC_BITPOS);
   static public int IPPVC_CBP_CHROMA_AC            = (0xff<<IPPVC_CBP_1ST_CHROMA_AC_BITPOS);
   static public int IPPVC_CBP_LUMA_AC              = (0xffff<<IPPVC_CBP_1ST_LUMA_AC_BITPOS);
   static public int IPPVC_CBP_1ST_LUMA_AC_BITPOS   = 1;
   static public int IPPVC_CBP_LUMA_DC              = 1;
   static public int MAX_CAVLC_LEVEL_VALUE          = 2063;

   static public int IPPVC_CBP_DC            = 1;
   static public int IPPVC_CBP_1ST_AC_BITPOS = 1;

   static public int IPPVC_MBTYPE_INTER        = 0;    /* P picture or P-VOP */
   static public int IPPVC_MBTYPE_INTER_Q      = 1;    /* P picture or P-VOP */
   static public int IPPVC_MBTYPE_INTER4V      = 2;    /* P picture or P-VOP */
   static public int IPPVC_MBTYPE_INTRA        = 3;    /* I and P picture, or I- and P-VOP */
   static public int IPPVC_MBTYPE_INTRA_Q      = 4;    /* I and P picture, or I- and P-VOP */
   static public int IPPVC_MBTYPE_INTER4V_Q    = 5;    /* P picture or P-VOP(H.263)*/
   static public int IPPVC_MBTYPE_DIRECT       = 6;    /* B picture or B-VOP (MPEG-4 only) */
   static public int IPPVC_MBTYPE_INTERPOLATE  = 7;    /* B picture or B-VOP */
   static public int IPPVC_MBTYPE_BACKWARD     = 8;    /* B picture or B-VOP */
   static public int IPPVC_MBTYPE_FORWARD      = 9;    /* B picture or B-VOP */
   static public int IPPVC_MB_STUFFING         = 255;

   static public int IPPVC_SCAN_NONE        = -1;
   static public int IPPVC_SCAN_ZIGZAG      = 0;
   static public int IPPVC_SCAN_VERTICAL    = 1;
   static public int IPPVC_SCAN_HORIZONTAL  = 2;

   static public int IPPVC_BLOCK_LUMA   = 0;
   static public int IPPVC_BLOCK_CHROMA = 1;

   static public int IPPVC_INTERP_NONE = 0;
   static public int IPPVC_INTERP_HORIZONTAL = 1;
   static public int IPPVC_INTERP_VERTICAL = 2;
   static public int IPPVC_INTERP_2D = 3;

   static public int IPPVC_SPRITE_STATIC = 1;
   static public int IPPVC_SPRITE_GMC    = 2;

   static public int IPPVC_ROTATE_DISABLE = 0;
   static public int IPPVC_ROTATE_90CCW   = 1;
   static public int IPPVC_ROTATE_90CW    = 2;
   static public int IPPVC_ROTATE_180     = 3;

   static public int IPPVC_CbYCr422ToBGR565 = 0;
   static public int IPPVC_CbYCr422ToBGR555 = 1;

   static public int IPPVC_EDGE_QUARTER_1    = 0x01;
   static public int IPPVC_EDGE_QUARTER_2    = 0x02;
   static public int IPPVC_EDGE_QUARTER_3    = 0x04;
   static public int IPPVC_EDGE_QUARTER_4    = 0x08;
   static public int IPPVC_EDGE_HALF_1       = IPPVC_EDGE_QUARTER_1 + IPPVC_EDGE_QUARTER_2;
   static public int IPPVC_EDGE_HALF_2       = IPPVC_EDGE_QUARTER_3 + IPPVC_EDGE_QUARTER_4;
   static public int IPPVC_EDGE_ALL          = IPPVC_EDGE_HALF_1 + IPPVC_EDGE_HALF_2;


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAdd8x8HP_16s8u_C1RS (  short *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAdd8x8_16s8u_C1IRS (  short *pSrc, int srcStep, byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAddBackPredPB_H263_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pSrcDst, int srcDstStep, int acc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAddC8x8_16s8u_C1IR (  short value, byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAverage16x16_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAverage16x16_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAverage8x8_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiAverage8x8_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBiDirWeightBlockImplicit_H264_8u_P2P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint srcStep, uint dstStep, int iWeight1, int iWeight2, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBiDirWeightBlockImplicit_H264_8u_P3P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint nSrcPitch1, uint nSrcPitch2, uint nDstPitch, int iWeight1, int iWeight2, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBiDirWeightBlock_H264_8u_C2R(ref IppVCWeightBlock_8u pIppVCWeightBlock, ref IppVCWeightParams_8u pIppVCWeightParamsP1, ref IppVCWeightParams_8u pIppVCWeightParamsP2);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBiDirWeightBlock_H264_8u_P2P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint srcStep, uint dstStep, uint ulog2wd, int iWeight1, int iOffset1, int iWeight2, int iOffset2, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBiDirWeightBlock_H264_8u_P3P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint nSrcPitch1, uint nSrcPitch2, uint nDstPitch, uint ulog2wd, int iWeight1, int iOffset1, int iWeight2, int iOffset2, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBidirWeightImplicit_H264_16u_P2P1R (  IppVCBidir_16u *bidirInfo, int iWeight1, int iWeight2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBidirWeight_H264_16u_P2P1R (  IppVCBidir_16u *bidirInfo, uint ulog2wd, int iWeight1, int iOffset1, int iWeight2, int iOffset2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiBidir_H264_16u_P2P1R (  IppVCBidir_16u *bidirInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACEncodeBinBypass_H264 (  uint code, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACEncodeBin_H264 (  uint ctxIdx, uint code, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACEncodeResidualBlock_H264_16s (  short *pResidualCoeffs, uint nLastNonZeroCoeff, uint ctxBlockCat, uint log2NumC8x8, uint bFrameBlock, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACEncodeResidualBlock_H264_32s (  int *pResidualCoeffs, uint nLastNonZeroCoeff, uint ctxBlockCat, uint log2NumC8x8, uint bFrameBlock, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACFree_H264 (  IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACGetContexts_H264 (  IppvcCABACState *pCabacState, uint offset, byte *pContexts, uint nContexts );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACGetSize_H264 (  uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACGetStreamSize_H264 (  uint *pBitStreamBits, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACInitAlloc_H264 (  IppvcCABACState **ppCabacState, byte *pBitStream, uint nBitStreamOffsetBits, uint nBitStreamSize, int SliceQPy, int cabacInitIdc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACInit_H264 (  IppvcCABACState *pCabacState, byte *pBitStream, uint nBitStreamOffsetBits, uint nBitStreamSize, int SliceQPy, int cabacInitIdc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACSetStream_H264 (  byte *pBitStream, uint uBitStreamSize, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCABACTerminateSlice_H264 (  uint *pBitStreamBytes, IppvcCABACState *pCabacState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCalcGlobalMV_MPEG4 (  int xOffset, int yOffset, IppMotionVector *pGMV, IppiWarpSpec_MPEG4 *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr420_Rotate_8u_C2P3R (  byte *pSrc, int srcStep, IppiSize srcRoi, byte **pDst, int *dstStep, int rotation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCbYCr422ToYCbCr420_Rotate_8u_P3R (  byte **pSrc, int *srcStep, IppiSize srcRoi, byte **pDst, int *dstStep, int rotation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiChangeSpriteBrightness_MPEG4_8u_C1IR (  byte *pSrcDst, int srcDstStep, int width, int height, int brightnessChangeFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy16x16HP_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy16x16QP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy16x16_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy16x8HP_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy16x8QP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy8x4HP_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy8x8HP_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy8x8QP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCopy8x8_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCountZeros8x8_16s_C1 (  short *pSrc, uint *pCount );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiCreateRLEncodeTable (  int *pSrcTable, IppVCHuffmanSpec_32s **ppDstSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT2x4x8Frw_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT2x4x8Inv_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x4x2To4x4Inv_DV_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8Fwd_8u16s_C2P2 (  byte *pSrc, int srcStep, short *pDstU, short *pDstV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8InvOrSet_16s8u_P2C2 (  short *pSrcU, short *pSrcV, byte *pDst, int dstStep, int flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8Inv_AANTransposed_16s8u_C1R (  short *pSrc, byte *pDst, int dstStep, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8Inv_AANTransposed_16s8u_P2C2R (  short *pSrcU, short *pSrcV, byte *pDstUV, int dstStep, int countU, int countV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8Inv_AANTransposed_16s_C1R (  short *pSrc, short *pDst, int dstStep, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDCT8x8Inv_AANTransposed_16s_P2C2R (  short *pSrcU, short *pSrcV, short *pDstUV, int dstStep, int countU, int countV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCChroma422DcCoeffs_H264_1u16s (  uint **ppBitStream, int *pBitOffset, short *pNumCoeff, short **ppDstCoeffs, int *pTblCoeffToken, int **ppTblTotalZerosCR, int **ppTblRunBefore );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCChroma422DcCoeffs_H264_1u32s (  uint **ppBitStream, int *pBitOffset, short *pNumCoeff, int **ppDstCoeffs, int *pTblCoeffToken, int **ppTblTotalZerosCR, int **ppTblRunBefore );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCChromaDcCoeffs_H264_1u16s (  uint **ppBitStream, int *pOffset, short *pNumCoeff, short **ppDstCoeffs, int *pTblCoeffToken, int **ppTblTotalZerosCR, int **ppTblRunBefore );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCChromaDcCoeffs_H264_1u32s (  uint **ppBitStream, int *pBitOffset, short *pNumCoeff, int **ppDstCoeffs, int *pTblCoeffToken, int **ppTblTotalZerosCR, int **ppTblRunBefore );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCCoeffs_H264_1u16s (  uint **ppBitStream, int *pOffset, short *pNumCoeff, short **ppDstCoeffs, uint uVLCSelect, short uMaxNumCoeff, int **ppTblCoeffToken, int **ppTblTotalZeros, int **ppTblRunBefore, int *pScanMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCAVLCCoeffs_H264_1u32s (  uint **ppBitStream, int *pBitOffset, short *pNumCoeff, int **ppDstCoeffs, uint uVLCSelect, short uMaxNumCoeff, int **ppTblCoeffToken, int **ppTblTotalZeros, int **ppTblRunBefore, int *pScanMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeChromaBlock_AVS_1u16s (  uint **ppBitStream, int *pBitOffset, int *pNumCoeff, short *pDstCoeffs, uint scanType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsInterRVLCBack_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoeffs, int *pIndxLastNonZero );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsInter_H261_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsInter_H263_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int modQuantFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsInter_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoeffs, int *pIndxLastNonZero, int rvlcFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsIntraRVLCBack_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoeffs, int *pIndxLastNonZero, int noDCFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsIntra_H261_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsIntra_H263_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int advIntraFlag, int modQuantFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeCoeffsIntra_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoeffs, int *pIndxLastNonZero, int rvlcFlag, int noDCFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeDCIntra_H263_1u16s (  byte **ppBitStream, int *pBitOffset, short *pDC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeDCIntra_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pDst, int blockType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeExpGolombOne_H264_1u16s (  uint **ppBitStream, int *pBitOffset, short *pDst, byte isSigned );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeExpGolombOne_H264_1u32s (  uint **ppBitStream, int *pBitOffset, int *pDst, int isSigned );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeHuffmanOne_1u32s (  uint **ppBitStream, int *pOffset, int *pDst, IppVCHuffmanSpec_32s *pDecodeTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeHuffmanPair_1u16s (  uint **ppBitStream, int *pOffset, IppVCHuffmanSpec_32s *pDecodeTable, sbyte *pFirst, short *pSecond );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeLumaBlockInter_AVS_1u16s (  uint **ppBitStream, int *pBitOffset, int *pNumCoeff, short *pDstCoeffs, uint scanType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDecodeLumaBlockIntra_AVS_1u16s (  uint **ppBitStream, int *pBitOffset, int *pNumCoeff, short *pDstCoeffs, uint scanType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendFree_8u_C1 (  IppiDeinterlaceBlendState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendFree_8u_C2 (  IppiDeinterlaceBlendSpec_8u_C2 *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendGetSize_8u_C1 (  int *pStateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendGetSize_8u_C2 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendInitAlloc_8u_C1 (  IppiSize planeSize, int *blendThresh, double *blendConstants, IppiDeinterlaceBlendState_8u_C1 **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendInitAlloc_8u_C2 (  IppiSize size, int *blendThresh, double *blendConstants, IppiDeinterlaceBlendSpec_8u_C2 **ppSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendInit_8u_C1 (  IppiSize size, int *blendThresh, double *blendConstants, IppiDeinterlaceBlendState_8u_C1 **ppState, byte *pMemState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlendInit_8u_C2 (  IppiSize size, int *blendThresh, double *blendConstants, IppiDeinterlaceBlendSpec_8u_C2 **ppSpec, byte *pMemState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlend_8u_C1 (  byte **pSrcPlane, int srcStep, byte *pDst, int dstStep, IppiSize planeSize, int topFirst, int topField, int copyField, IppiDeinterlaceBlendState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceBlend_8u_C2 (  byte *pSrcPrev, int srcStepPrev, byte *pSrcCur, int srcStepCur, byte *pSrcNext, int srcStepNext, byte *pDst, int dstStep, IppiSize size, IppvcFrameFieldFlag fieldFirst, IppvcFrameFieldFlag fieldProcess, IppBool fieldCopy, IppiDeinterlaceBlendSpec_8u_C2 *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceEdgeDetect_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize size, int fieldNum, int bCopyBorder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceFilterTriangle_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, uint centerWeight, uint layout );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceFilterTriangle_8u_C2R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, uint centerWeight, uint layout );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceMedianThreshold_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize size, int threshold, int fieldNum, int bCopyBorder );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDeinterlaceMotionAdaptive_8u_C1 (  byte **pSrcPlane, int srcStep, byte *pDst, int dstStep, IppiSize planeSize, int threshold, int topFirst, int topField, int copyField, int artifactProtection );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDequantTransformResidualAndAdd_H264_16s_C1I (  byte *pPred, short *pSrcDst, short *pDC, byte *pDst, int PredStep, int DstStep, int QP, int AC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDequantTransformResidual_H264_16s_C1I (  short *pSrcDst, int step, short *pDC, int AC, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDequantTransformResidual_SISP_H264_16s_C1I (  short *pSrcDst, short *pPredictBlock, short *pDC, int AC, int qp, int qs, int Switch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDisassembleChroma420Intra_AVS_16s8u_C1R(byte** pSrcPlane, int srcStep, byte** pDstPlane, int dstStep, short** ppDstCoeff, IppIntraChromaPredMode_H264* pPredMode, uint* pChromaCBP, uint chromaQP, uint edgeType);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDisassembleLumaIntra_AVS_16s8u_C1R(byte* pSrcPlane, int srcStep, byte* pDstPlane, int dstStep, short** ppDstCoeff, IppIntra8x8PredMode_H264* pPredModes, uint* pLumaCBP, uint QP, uint edgeType);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDownsampleFour16x16_H263_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiDownsampleFour_H263_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEdgesDetect16x16_16u_C1R (  ushort *pSrc, int srcStep, int EdgePelDifference, int EdgePelCount, int *pRes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEdgesDetect16x16_8u_C1R (  byte *pSrc, uint srcStep, byte EdgePelDifference, byte EdgePelCount, byte *pRes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeChromaDcCoeffsCAVLC_H264_16s (  short *pSrc, byte *pTrailingOnes, byte *pTrailingOneSigns, byte *pNumOutCoeffs, byte *pTotalZeros, short *pLevels, byte *pRuns );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsCAVLCChromaDC2x2_H264_32s (  int *pSrc, byte *pTrailingOnes, byte *pTrailingOneSigns, byte *pNumOutCoeffs, byte *pTotalZeroes, int *pLevels, byte *pRuns );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsCAVLCChromaDC2x4_H264_16s (  short *pSrc, byte *pTrailing_Ones, byte *pTrailing_One_Signs, byte *pNumOutCoeffs, byte *pTotalZeros, short *pLevels, byte *pRuns );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsCAVLCChromaDC2x4_H264_32s (  int *pSrc, byte *pTrailing_Ones, byte *pTrailing_One_Signs, byte *pNumOutCoeffs, byte *pTotalZeros, int *pLevels, byte *Runs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsCAVLC_H264_16s (  short *pSrc, byte AC, int *pScanMatrix, byte Count, byte *Trailing_Ones, byte *Trailing_One_Signs, byte *NumOutCoeffs, byte *TotalZeros, short *Levels, byte *Runs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsCAVLC_H264_32s (  int *pSrc, int AC, int *pScanMatrix, int Count, byte *Trailing_Ones, byte *Trailing_One_Signs, byte *NumOutCoeffs, byte *TotalZeroes, int *Levels, byte *Runs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsInter_H261_16s1u (  short *pQCoef, byte **ppBitStream, int *pBitOffset, int countNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsInter_H263_16s1u (  short *pQCoef, byte **ppBitStream, int *pBitOffset, int countNonZero, int modQuantFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsInter_MPEG4_16s1u (  short *pCoeffs, byte **ppBitStream, int *pBitOffset, int countNonZero, int rvlcFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsIntra_H261_16s1u (  short *pQCoef, byte **ppBitStream, int *pBitOffset, int countNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsIntra_H263_16s1u (  short *pQCoef, byte **ppBitStream, int *pBitOffset, int countNonZero, int advIntraFlag, int modQuantFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeCoeffsIntra_MPEG4_16s1u (  short *pCoeffs, byte **ppBitStream, int *pBitOffset, int countNonZero, int rvlcFlag, int noDCFlag, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeDCIntra_H263_16s1u (  short qDC, byte **ppBitStream, int *pBitOffset );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiEncodeDCIntra_MPEG4_16s1u (  short dcCoeff, byte **ppBitStream, int *pBitOffset, int blockType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiExpandPlane_H264_8u_C1R (  byte *StartPtr, uint uFrameWidth, uint uFrameHeight, uint uPitch, uint uPels, IppvcFrameFieldFlag uFrameFieldFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilter8x8_H261_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterBlockBoundaryHorEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterBlockBoundaryVerEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking16x16HorEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking16x16VerEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking8x8HorEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking8x8HorEdge_MPEG4_8u_C1IR (  byte *pSrcDst, int step, int QP, int THR1, int THR2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking8x8VerEdge_H263_8u_C1IR (  byte *pSrcDst, int srcDstStep, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblocking8x8VerEdge_MPEG4_8u_C1IR (  byte *pSrcDst, int step, int QP, int THR1, int THR2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422HorEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422HorEdge_H264_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422VerEdgeMBAFF_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422VerEdgeMBAFF_H264_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422VerEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma422VerEdge_H264_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChromaHorEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChromaVerEdgeMBAFF_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChromaVerEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_AVS_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_H264_8u_C2I (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_H264_8u_C2IR (  byte *pSrcDst, uint srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_VC1_8u_C1IR (  byte *pSrcDst, int pQuant, int srcdstStep, int edgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_HorEdge_VC1_8u_C2IR (  byte *pSrcDst, uint pQuant, int srcdstStep, uint uEdgeDisabledFlag, uint vEdgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_AVS_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_H264_8u_C2I (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_H264_8u_C2IR (  byte *pSrcDst, uint srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_MBAFF_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, uint nAlpha, uint nBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_VC1_8u_C1IR (  byte *pSrcDst, int pQuant, int srcdstStep, int edgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingChroma_VerEdge_VC1_8u_C2IR (  byte *pSrcDst, uint pQuant, int srcdstStep, uint uEdgeDisabledFlag, uint vEdgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLumaHorEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLumaVerEdgeMBAFF_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLumaVerEdge_H264_16u_C1IR (  IppiFilterDeblock_16u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_HorEdge_AVS_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_HorEdge_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBS );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_HorEdge_VC1_8u_C1IR (  byte *pSrcDst, int pQuant, int srcdstStep, int edgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_VerEdge_AVS_8u_C1IR (  IppiFilterDeblock_8u *pDeblockInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_VerEdge_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, byte *pAlpha, byte *pBeta, byte *pThresholds, byte *pBs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_VerEdge_MBAFF_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, uint nAlpha, uint nBeta, byte *pThresholds, byte *pBs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeblockingLuma_VerEdge_VC1_8u_C1IR (  byte *pSrcDst, int pQuant, int srcdstStep, int edgeDisabledFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseAdaptiveFree_8u_C1 (  IppiDenoiseAdaptiveState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseAdaptiveInitAlloc_8u_C1 (  IppiDenoiseAdaptiveState_8u_C1 **ppState, IppiSize roiSize, IppiSize maskSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseAdaptive_8u_C1R (  byte **pSrcPlane, int srcStep, byte *pDst, int dstStep, IppiSize size, IppiRect roi, IppiSize maskSize, int threshold, IppvcNoiseBlurFlag blurFlag, IppiDenoiseAdaptiveState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseCASTInit (  IppDenoiseCAST *pInParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseCASTYUV422_8u_C2R (  byte *pSrcCur, byte *pSrcPrev, int srcStep, byte *pSrcEdge, int srcEdgeStep, IppiSize srcRoiSize, byte *pDst, int dstStep, byte *pHistoryWeight, IppDenoiseCAST *pInParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseCAST_8u_C1R (  byte *pSrcCur, byte *pSrcPrev, int srcStep, byte *pSrcEdge, int srcEdgeStep, IppiSize srcRoiSize, byte *pDst, int dstStep, byte *pHistoryWeight, IppDenoiseCAST *pInParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseMosquitoFree_8u_C1 (  IppiDenoiseMosquitoState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseMosquitoInitAlloc_8u_C1 (  IppiDenoiseMosquitoState_8u_C1 **ppState, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseMosquito_8u_C1R (  byte **pSrcPlane, int srcStep, byte *pDst, int dstStep, IppiSize size, IppiRect roi, IppiDenoiseMosquitoState_8u_C1 *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDenoiseSmooth_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize size, IppiRect roi, int threshold, byte *pWorkBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeringingSmooth8x8_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int QP, int threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFilterDeringingThreshold_MPEG4_8u_P3R (  byte *pSrcY, int stepY, byte *pSrcCb, int stepCb, byte *pSrcCr, int stepCr, int *threshold );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFrameFieldSAD16x16_16s32s_C1R (  short *pSrc, int srcStep, int *pFrameSAD, int *pFieldSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFrameFieldSAD16x16_8u32s_C1R (  byte *pSrc, int srcStep, int *pFrameSAD, int *pFieldSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiFreeHuffmanTable_DV_32u (  uint *pHuffTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGenScaleLevel4x4_H264_8u16s_C1 (  byte *pSrcScaleMatrix, short *pDstInvScaleMatrix, short *pDstScaleMatrix, int QpRem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGenScaleLevel8x8_H264_8u16s_D2 (  byte *pSrcInvScaleMatrix, int SrcStep, short *pDstInvScaleMatrix, short *pDstScaleMatrix, int Qp_rem );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff16x16B_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiff, int dstDiffStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff16x16_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff16x8B_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiff, int dstDiffStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff16x8_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff4x4_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x16B_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiff, int dstDiffStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x16_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x4B_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiff, int dstDiffStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x4B_8u16s_C2P2 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiffU, int dstDiffStepU, short *pDstDiffV, int dstDiffStepV, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x4_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x4_8u16s_C2P2 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiffU, int dstDiffStepU, short *pDstDiffV, int dstDiffStepV, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x8B_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiff, int dstDiffStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x8B_8u16s_C2P2 (  byte *pSrcCur, int srcCurStep, byte *pSrcRefF, int srcRefStepF, int mcTypeF, byte *pSrcRefB, int srcRefStepB, int mcTypeB, short *pDstDiffU, int dstDiffStepU, short *pDstDiffV, int dstDiffStepV, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x8_8u16s_C1 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiff, int dstDiffStep, short *pDstPredictor, int dstPredictorStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiGetDiff8x8_8u16s_C2P2 (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, short *pDstDiffU, int dstDiffStepU, short *pDstDiffV, int dstDiffStepV, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHadamard8x8Sum_VC1_16s (  short *pSrc, uint srcStep, short *pDst, int *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHadamard8x8Sum_VC1_8u16s (  byte *pSrc, uint srcStep, short *pDst, int *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanDecodeSegmentOnePass_DV_8u16s (  byte *pStream, uint *pZigzagTables, uint *pHuffTable, short *pBlock, uint *pBlockParam, int nNumCoeffs );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanDecodeSegment_DV100_8u16s (  byte *pStream, uint *pZigzagTable, uint *pHuffTable, short *pBlock, uint *pBlockParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanDecodeSegment_DV_8u16s (  byte *pStream, uint *pZigzagTables, uint *pHuffTable, short *pBlock, uint *pBlockParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanRunLevelTableInitAlloc_32s (  int *pSrcTable, IppVCHuffmanSpec_32s **ppDstSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanTableFree_32s (  IppVCHuffmanSpec_32s *pDecodeTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiHuffmanTableInitAlloc_32s (  int *pSrcTable, IppVCHuffmanSpec_32s **ppDstSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInitAllocHuffmanTable_DV_32u (  int *pSrcTable1, int *pSrcTable2, uint **ppHuffTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateAverage16x16_8u_C1IR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateAverage16x8_8u_C1IR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateAverage8x4_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateAverage8x8_8u_C1IR (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int acc, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateBlock_H264_8u_P2P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint uWidth, uint uHeight, int pitch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateBlock_H264_8u_P3P1R (  byte *pSrc1, byte *pSrc2, byte *pDst, uint uWidth, uint uHeight, int iPitchSrc1, int iPitchSrc2, int iPitchDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBlock_H264_16u_P2R (  ref IppVCInterpolateBlock_16u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBlock_H264_8u_C2C2R (  ref IppVCInterpolateBlock_8u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBlock_H264_8u_C2P2R (  ref IppVCInterpolateBlock_8u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBlock_H264_8u_P2R (  ref IppVCInterpolateBlock_8u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBottom_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo, int outPixels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaBottom_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, int outPixels, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaTop_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo, int outPixels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChromaTop_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, int outPixels, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChroma_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChroma_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateChroma_H264_8u_C2P2R (  byte *pSrcUV, int srcStep, byte *pDstU, byte *pDstV, int dstStep, int dx, int dy, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateICBicubicBlock_VC1_8u_C1R (  IppVCInterpolateBlockIC_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateICBilinearBlock_VC1_8u_C1R (  IppVCInterpolateBlockIC_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateICBilinearBlock_VC1_8u_C2R (  IppVCInterpolateBlockIC_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaBlock_AVS_8u_P1R (  ref IppVCInterpolateBlock_8u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaBlock_H264_16u_P1R (  ref IppVCInterpolateBlock_16u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaBlock_H264_8u_P1R (  ref IppVCInterpolateBlock_8u interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaBottom_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo, int outPixels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaBottom_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, int outPixels, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaTop_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo, int outPixels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLumaTop_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, int outPixels, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLuma_H264_16u_C1R (  IppVCInterpolate_16u *interpolateInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateLuma_H264_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, int dx, int dy, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateQPBicubic_VC1_8u_C1R (  IppVCInterpolate_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateQPBilinear_VC1_8u_C1R (  IppVCInterpolate_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiInterpolateQPBilinear_VC1_8u_C2R (  IppVCInterpolate_8u *pParams );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x16B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x16_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x4B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x4_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x8BUV_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x8B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x8UV_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC16x8_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC2x2B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC2x2_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC2x4B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC2x4_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x2B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x2_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x4B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x4_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x8B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC4x8_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x16B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x16_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x4B_16s8u_P2C2R (  byte *pSrcRefF, int srcRefFStep, int mcTypeF, byte *pSrcRefB, int srcRefBStep, int mcTypeB, short *pSrcU, short *pSrcV, int srcUVStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x4B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x4_16s8u_P2C2R (  byte *pSrcRef, int srcRefStep, short *pSrcU, short *pSrcV, int srcUVStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x4_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x8B_16s8u_P2C2R (  byte *pSrcRefF, int srcRefFStep, int mcTypeF, byte *pSrcRefB, int srcRefBStep, int mcTypeB, short *pSrcU, short *pSrcV, int srcUVStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x8B_8u_C1 (  byte *pSrcRefF, int srcStepF, int mcTypeF, byte *pSrcRefB, int srcStepB, int mcTypeB, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x8_16s8u_P2C2R (  byte *pSrcRef, int srcRefStep, short *pSrcU, short *pSrcV, int srcUVStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMC8x8_8u_C1 (  byte *pSrcRef, int srcStep, short *pSrcYData, int srcYDataStep, byte *pDst, int dstStep, int mcType, int roundControl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMeanAbsDev16x16_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiMeanAbsDev8x8_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiOBMC16x16HP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppMotionVector *pMVCur, IppMotionVector *pMVLeft, IppMotionVector *pMVRight, IppMotionVector *pMVAbove, IppMotionVector *pMVBelow, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiOBMC8x8HP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppMotionVector *pMVCur, IppMotionVector *pMVLeft, IppMotionVector *pMVRight, IppMotionVector *pMVAbove, IppMotionVector *pMVBelow, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiOBMC8x8QP_MPEG4_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppMotionVector *pMVCur, IppMotionVector *pMVLeft, IppMotionVector *pMVRight, IppMotionVector *pMVAbove, IppMotionVector *pMVBelow, int rounding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiPredictIntraChroma8x8_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, IppIntraChromaPredMode_H264 predMode, int availability );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiPredictIntra_16x16_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, IppIntra16x16PredMode_H264 predMode, int availability );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiPredictIntra_4x4_H264_8u_C1IR (  byte *pSrcDst, int srcdstStep, IppIntra4x4PredMode_H264 predMode, int availability );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiPutIntraBlock (  uint **ppBitStream, int *pOffset, short *pSrcBlock, int *pDCPred, IppVCHuffmanSpec_32u *pDCTable, IppVCHuffmanSpec_32s *pACTable, int *pScanMatrix, int EOBLen, int EOBCode, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiPutNonIntraBlock (  uint **pBitStream, int *pOffset, short *pSrcBlock, IppVCHuffmanSpec_32s *pACTable, int *pScanMatrix, int EOBLen, int EOBCode, int count );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInterGetSize_MPEG4 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInterInit_MPEG4 (  byte *pQuantMatrix, IppiQuantInterSpec_MPEG4 *pSpec, int bitsPerPixel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInterNonuniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInterUniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInter_H263_16s_C1I (  short *pSrcDst, int QP, int *pCountNonZero, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInter_MPEG4_16s_C1I (  short *pCoeffs, IppiQuantInterSpec_MPEG4 *pSpec, int QP, int *pCountNonZero );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntraGetSize_MPEG4 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntraInit_MPEG4 (  byte *pQuantMatrix, IppiQuantIntraSpec_MPEG4 *pSpec, int bitsPerPixel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntraNonuniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntraUniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntra_H263_16s_C1I (  short *pSrcDst, int QP, int *pCountNonZero, int advIntraFlag, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntra_MPEG2_16s_C1I (  short *pSrcDst, int QP, float *pQPMatrix, int *pCount );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantIntra_MPEG4_16s_C1I (  short *pCoeffs, IppiQuantIntraSpec_MPEG4 *pSpec, int QP, int *pCountNonZero, int blockType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterGetSize_MPEG4 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterInit_MPEG4 (  byte *pQuantMatrix, IppiQuantInvInterSpec_MPEG4 *pSpec, int bitsPerPixel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterNonuniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize roiSize, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterNonuniform_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, int doubleQuant, IppiSize roiSize, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterUniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize roiSize, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInterUniform_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, int doubleQuant, IppiSize roiSize, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInter_H263_16s_C1I (  short *pSrcDst, int indxLastNonZero, int QP, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvInter_MPEG4_16s_C1I (  short *pCoeffs, int indxLastNonZero, IppiQuantInvInterSpec_MPEG4 *pSpec, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraGetSize_MPEG4 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraInit_MPEG4 (  byte *pQuantMatrix, IppiQuantInvIntraSpec_MPEG4 *pSpec, int bitsPerPixel );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraNonuniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraNonuniform_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, int doubleQuant, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraUniform_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, int doubleQuant, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntraUniform_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, int doubleQuant, IppiSize *pDstSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntra_H263_16s_C1I (  short *pSrcDst, int indxLastNonZero, int QP, int advIntraFlag, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntra_MPEG2_16s_C1I (  short *pSrcDst, int QP, short *pQPMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvIntra_MPEG4_16s_C1I (  short *pCoeffs, int indxLastNonZero, IppiQuantInvIntraSpec_MPEG4 *pSpec, int QP, int blockType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInvLuma8x8_H264_32s_C1I (  int *pSrcDst, int Qp6, short *pInvLevelScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInv_DV_16s_C1I (  short *pSrcDst, short *pDequantTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantInv_MPEG2_16s_C1I (  short *pSrcDst, int QP, short *pQPMatrix );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantLuma8x8Inv_H264_16s_C1I (  short *pSrcDst, int Qp6, short *pInvLevelScale );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantLuma8x8_H264_16s_C1 (  short *pSrc, short *pDst, int Qp6, int Intra, short *pScanMatrix, short *pScaleLevels, int *pNumLevels, int *pLastCoeff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantLuma8x8_H264_32s_C1 (  int *pSrc, int *pDst, int Qp6, int Intra, short *pScanMatrix, short *pScaleLevels, int *pNumLevels, int *pLastCoeff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantWeightBlockInv_DV100_16s_C1I (  short *pSrcDst, short *pWeightInvTable, int quantValue );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantWeightBlockInv_DV_16s_C1I (  short *pSrcDst, short *pQuantInvTable, short *pWeightInvTable );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuant_MPEG2_16s_C1I (  short *pSrcDst, int QP, float *pQPMatrix, int *pCount );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantizeResidual4x4Fwd_H264_16s32s_C1 (  short *pSrc, short *pDst, uint *pNumNonZeros, uint *pLastNonZero, int *pQuantTable, short *pScanMatrix, int QP, int roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantizeResidual4x4Fwd_H264_16s_C1 (  short *pSrc, short *pDst, uint *pNumNonZeros, uint *pLastNonZero, short *pQuantTable, short *pScanMatrix, int QP, int roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiQuantizeResidual4x4Fwd_H264_32s_C1 (  int *pSrc, int *pDst, uint *pNumNonZeros, uint *pLastNonZero, int *pQuantTable, short *pScanMatrix, int QP, int roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiRangeMapping_VC1_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int rangeMapParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422Inter4x4_H264High_16s8u_IP2R (  IppiReconstructHighMB_16s8u **pReconstructInfo, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422Inter4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422Intra4x4_H264High_16s8u_IP2R (  IppiReconstructHighMB_16s8u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeType, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422Intra4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeType, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422IntraHalf4x4_H264High_16s8u_IP2R (  IppiReconstructHighMB_16s8u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeTypeTop, uint edgeTypeBottom, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChroma422IntraHalf4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeTypeTop, uint edgeTypeBottom, uint levelScaleDCU, uint levelScaleDCV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInter4x4MB_H264_16s8u_C2R (  short **ppSrcDstCoeff, byte *pSrcDstUVPlane, uint srcdstUVStep, uint cbp4x4, uint chromaQPU, uint chromaQPV, short *pQuantTableU, short *pQuantTableV, uint bypassFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInter4x4MB_H264_16s8u_P2R (  short **ppSrcDstCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, uint cbp4x4, uint chromaQPU, uint chromaQPV, short *pQuantTableU, short *pQuantTableV, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInter4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInterMB_H264_16s8u_C2R (  short **ppSrcDstCoeff, byte *pSrcDstUVPlane, int srcDstUVStep, uint cbp4x4, uint ChromaQP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInterMB_H264_16s8u_P2R (  short **ppSrcCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstStep, uint cbp4x4, uint ChromaQP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaInter_AVS_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, int srcDstUVStep, int *pSrcNumCoeffs, uint cbp8x8, uint chromaQP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntra4x4MB_H264_16s8u_C2R (  short **ppSrcDstCoeff, byte *pSrcDstUVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intraChromaMode, uint cbp4x4, uint chromaQPU, uint chromaQPV, uint edgeType, short *pQuantTableU, short *pQuantTableV, uint bypassFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntra4x4MB_H264_16s8u_P2R (  short **ppSrcDstCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint chromaQPU, uint chromaQPV, byte edge_type, short *pQuantTableU, short *pQuantTableV, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntra4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalf4x4_H264High_32s16u_IP2R (  IppiReconstructHighMB_32s16u **pReconstructInfo, IppIntraChromaPredMode_H264 intraChromaMode, uint edgeTypeTop, uint edgeTypeBottom );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalfs4x4MB_H264_16s8u_P2R (  short **ppSrcDstCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint chromaQPU, uint chromaQPV, byte edge_type_top, byte edge_type_bottom, short *pQuantTableU, short *pQuantTableV, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalfsMB_H264_16s8u_P2R (  short **ppSrcCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint ChromaQP, byte edge_type_top, byte edge_type_bottom );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalves4x4MB_H264_16s8u_P2R (  short **ppSrcDstCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint chromaQPU, uint chromaQPV, byte edge_type_top, byte edge_type_bottom, short *pQuantTableU, short *pQuantTableV, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalvesMB_H264_16s8u_C2R (  short **ppSrcDstCoeff, byte *pSrcDstUVPlane, int srcdstUVStep, IppIntraChromaPredMode_H264 intraChromaMode, uint cbp4x4, uint ChromaQP, uint edgeTypeTop, uint edgeTypeBottom );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraHalvesMB_H264_16s8u_P2R (  short **ppSrcCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint ChromaQP, byte edge_type_top, byte edge_type_bottom );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraMB_H264_16s8u_C2R (  short **ppSrcDstCoeff, byte *pSrcDstUVPlane, int srcdstUVStep, IppIntraChromaPredMode_H264 intraChromaMode, uint cbp4x4, uint ChromaQP, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntraMB_H264_16s8u_P2R (  short **ppSrcCoeff, byte *pSrcDstUPlane, byte *pSrcDstVPlane, uint srcdstUVStep, IppIntraChromaPredMode_H264 intra_chroma_mode, uint cbp4x4, uint ChromaQP, byte edge_type );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructChromaIntra_AVS_16s8u_C1R(short** ppSrcCoeff, byte* pSrcDstUPlane, byte* pSrcDstVPlane, int srcDstUVStep, IppIntraChromaPredMode_H264 predMode, int* pSrcNumCoeffs, uint cbp8x8, uint chromaQP, uint edgeType);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructCoeffsInter_H261_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructCoeffsInter_H263_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int QP, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructCoeffsInter_MPEG4_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoeffs, int *pIndxLastNonZero, int rvlcFlag, int scan, IppiQuantInvInterSpec_MPEG4 *pQuantInvInterSpec, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructCoeffsIntra_H261_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructCoeffsIntra_H263_1u16s (  byte **ppBitStream, int *pBitOffset, short *pCoef, int *pIndxLastNonZero, int cbp, int QP, int advIntraFlag, int scan, int modQuantFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructDCTBlockIntra_MPEG1_32s (  uint **ppBitStream, int *pOffset, int *pDCSizeTable, int *pACTable, int *pScanMatrix, int QP, short *pQPMatrix, short *pDCPred, short *pDstBlock, int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructDCTBlockIntra_MPEG2_32s (  uint **ppBitStream, int *pOffset, IppVCHuffmanSpec_32s *pDCSizeTable, IppVCHuffmanSpec_32s *pACTable, int *pScanMatrix, int QP, short *pQPMatrix, short *pDCPred, int shiftDCVal, short *pDstBlock, int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructDCTBlock_MPEG1_32s (  uint **ppBitStream, int *pOffset, int *pDCSizeTable, int *pACTable, int *pScanMatrix, int QP, short *pQPMatrix, short *pDstBlock, int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructDCTBlock_MPEG2_32s (  uint **ppBitStream, int *pOffset, IppVCHuffmanSpec_32s *pDCTable, IppVCHuffmanSpec_32s *pACTable, int *pScanMatrix, int QP, short *pQPMatrix, short *pDstBlock, int *pDstSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInter4x4MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, uint srcdstYStep, uint cbp4x4, int QP, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInter4x4_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInter8x8MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, uint srcdstYStep, uint cbp8x8, int QP, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInter8x8_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInterMB_H264_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstYPlane, uint srcdstYStep, uint cbp4x4, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaInter_AVS_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstYPlane, int srcDstYStep, int *pSrcNumCoeffs, uint cbp8x8, uint QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra16x16MB_H264_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstYPlane, uint srcdstYStep, IppIntra16x16PredMode_H264 intra_luma_mode, uint cbp4x4, uint QP, byte edge_type );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra16x16_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo, IppIntra16x16PredMode_H264 intraLumaMode, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra4x4MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra4x4PredMode_H264 *pMBIntraTypes, uint cbp4x4, uint QP, byte edgeType, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra4x4_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo, IppIntra4x4PredMode_H264 *pMBIntraTypes, int edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra8x8MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra8x8PredMode_H264 *pMBIntraTypes, uint cbp8x8, uint QP, byte edgeType, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra8x8_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo, IppIntra8x8PredMode_H264 *pMBIntraTypes, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraHalf4x4MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra4x4PredMode_H264 *pMBIntraTypes, uint cbp4x2, uint QP, byte edgeType, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraHalf4x4_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo, IppIntra4x4PredMode_H264 *pMBIntraTypes, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraHalf8x8MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra8x8PredMode_H264 *pMBIntraTypes, uint cbp8x2, uint QP, byte edgeType, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraHalf8x8_H264High_32s16u_IP1R (  IppiReconstructHighMB_32s16u *pReconstructInfo, IppIntra8x8PredMode_H264 *pMBIntraTypes, uint edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraHalfMB_H264_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra4x4PredMode_H264 *pMBIntraTypes, uint cbp4x2, uint QP, byte edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntraMB_H264_16s8u_C1R (  short **ppSrcCoeff, byte *pSrcDstYPlane, int srcdstYStep, IppIntra4x4PredMode_H264 *pMBIntraTypes, uint cbp4x4, uint QP, byte edgeType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra_16x16MB_H264_16s8u_C1R (  short **ppSrcDstCoeff, byte *pSrcDstYPlane, uint srcdstYStep, IppIntra16x16PredMode_H264 intra_luma_mode, uint cbp4x4, uint QP, byte edge_type, short *pQuantTable, byte bypass_flag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiReconstructLumaIntra_AVS_16s8u_C1R(short** ppSrcCoeff, byte* pSrcDstYPlane, int srcDstYStep, IppIntra8x8PredMode_H264* pMBIntraTypes, int* pSrcNumCoeffs, uint cbp8x8, uint QP, uint edgeType);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiResample_H263_8u_P3R (  byte *pSrcY, int srcYStep, IppiSize ySrcRoiSize, byte *pSrcCb, int srcCbStep, byte *pSrcCr, int srcCrStep, byte *pDstY, int dstYStep, IppiSize yDstRoiSize, byte *pDstCb, int dstCbStep, byte *pDstCr, int dstCrStep, IppMotionVector *warpParams, int wda, int rounding, int fillMode, int *fillColor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiResizeCCRotate_8u_C2R (  byte *pSrc, int srcStep, IppiSize srcRoi, ushort *pDst, int dstStep, int zoomFactor, int interpolation, int colorConversion, int rotation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16Blocks4x4_16u32u_C1R (  ushort *pSrc, int srcStep, ushort *pRef, int refStep, uint *pDstSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16Blocks4x4_8u16u (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pDstSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16Blocks8x8_16u32u_C1R (  ushort *pSrc, int srcStep, ushort *pRef, int refStep, uint *pDstSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16Blocks8x8_8u16u (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pDstSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16_16u32s_C1R (  ushort *pSrc, int srcStep, ushort *pRef, int refStep, int *pSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16_8u32s (  byte *pSrc, int srcStep, byte *pRef, int refStep, int *pSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16xNI_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD, uint *pMinSADIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x16xN_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD16x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD2x2xN_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD4x4_16u32s_C1R (  ushort *pSrc, int srcStep, ushort *pRef, int refStep, int *pSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD4x4_8u32s (  byte *pSrc, int srcStep, byte *pRef, int refStep, int *pSAD, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD4x4xNI_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD, uint *pMinSADIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD4x4xN_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD4x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x16_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x4_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x8_16u32s_C1R (  ushort *pSrcCur, int srcCurStep, ushort *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x8_8u32s_C2R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDstU, int *pDstV, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x8xNI_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD, uint *pMinSADIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAD8x8xN_8u16u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, ushort *pSAD, int numSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAT8x8D_16u32s_C1R (  ushort *pSrcCur, int srcCurStep, ushort *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSAT8x8D_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD16x16_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD16x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD4x4_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD4x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD8x16_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD8x4_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSATD8x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSSD4x4_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSSD8x8_8u32s_C1R (  byte *pSrcCur, int srcCurStep, byte *pSrcRef, int srcRefStep, int *pDst, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiScanFwd_16s_C1 (  short *pSrc, short *pDst, int countNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiScanInv_16s_C1 (  short *pSrc, short *pDst, int indxLastNonZero, int scan );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R (  short *pSrcUpper, int srcUpperStep, short *pSrcBottom, int srcBottomStep, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingChroma_HorEdge_VC1_16s8u_P2C2R (  short *pSrcUpperU, uint srcUpperStepU, short *pSrcBottomU, uint srcBottomStepU, short *pSrcUpperV, uint srcUpperStepV, short *pSrcBottomV, uint srcBottomStepV, byte *pDst, uint dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R (  short *pSrcLeft, int srcLeftStep, short *pSrcRight, int srcRightStep, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingChroma_VerEdge_VC1_16s8u_P2C2R (  short *pSrcLeftU, uint srcLeftStepU, short *pSrcRightU, uint srcRightStepU, short *pSrcLeftV, uint srcLeftStepV, short *pSrcRightV, uint srcRightStepV, byte *pDst, uint dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R (  short *pSrcUpper, int srcUpperStep, short *pSrcBottom, int srcBottomStep, byte *pDst, int dstStep, uint edgeDisableFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R (  short *pSrcLeft, int srcLeftStep, short *pSrcRight, int srcRightStep, byte *pDst, int dstStep, uint fieldNeighbourFlag, uint edgeDisableFlag );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSpatialInterpolation_H263_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, int interpType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSqrDiff16x16B_8u32s (  byte *pSrc, int srcStep, byte *pRefF, int refStepF, int mcTypeF, byte *pRefB, int refStepB, int mcTypeB, int *pSqrDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSqrDiff16x16_8u32s (  byte *pSrc, int srcStep, byte *pRef, int refStep, int mcType, int *pSqrDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSub16x16_8u16s_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSub4x4_16u16s_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSub4x4_8u16s_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSub8x8_16u16s_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSub8x8_8u16s_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSubSAD8x8_8u16s_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, short *pDst, int dstStep, int *pSAD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSumsDiff16x16Blocks4x4_16u32s_C1R (  ushort *pSrc, int srcStep, ushort *pPred, int predStep, int *pSums, short *pDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSumsDiff16x16Blocks4x4_8u16s_C1 (  byte *pSrc, int srcStep, byte *pPred, int predStep, short *pSums, short *pDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSumsDiff8x8Blocks4x4_16u32s_C1R (  ushort *pSrc, int srcStep, ushort *pPred, int predStep, int *pSums, short *pDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSumsDiff8x8Blocks4x4_8u16s_C1 (  byte *pSrc, int srcStep, byte *pPred, int predStep, short *pSums, short *pDiff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiSumsDiff8x8Blocks4x4_8u16s_C2P2 (  byte *pSrcUV, int srcStep, byte *pPredU, int predStepU, byte *pPredV, int predStepV, short *pSumsU, short *pDiffU, short *pSumsV, short *pDiffV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x4Fwd_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x4Fwd_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x4Inv_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x4Inv_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x8Fwd_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x8Fwd_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x8Inv_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform4x8Inv_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x4Fwd_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x4Fwd_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x4Inv_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x4Inv_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x8Fwd_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x8Fwd_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x8Inv_VC1_16s_C1IR (  short *pSrcDst, int srcDstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransform8x8Inv_VC1_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep, IppiSize srcSizeNZ );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformDequantChromaDC_H264_16s_C1I (  short *pSrcDst, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformDequantChromaDC_SISP_H264_16s_C1I (  short *pSrcDst, short *pDCpredict, int qp, int qs, int Switch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformDequantLumaDC_H264_16s_C1I (  short *pSrcDst, int QP );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformFwdLuma8x8_H264_16s32s_C1 (  short *pSrc, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformFwdLuma8x8_H264_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformInvAddPredLuma8x8_H264_32s16u_C1R (  ushort *pPred, int predStep, int *pSrcDst, ushort *pDst, int dstStep, int bitDepth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformLuma8x8Fwd_H264_16s_C1I (  short *pSrcDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformLuma8x8InvAddPred_H264_16s8u_C1R (  byte *pPred, int PredStep, short *pSrcDst, byte *pDst, int DstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformPrediction_H264_8u16s_C1 (  byte *pSrc, int step, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuant8x8Fwd_AVS_16s_C1 (  short *pSrc, short *pDst, uint *pNumCoeffs, uint QP, uint roundMode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantChromaDC_H264_16s_C1I (  short *pSrcDst, short *pTBlock, int QPCroma, sbyte *pNumLevels, byte intra, byte needTransform );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwd4x4_H264_16s32s_C1 (  short *pSrc, int *pDst, int QP, int *pNumCoeffs, int Intra, short *pScanMatrix, int *pLastCoeff, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwd4x4_H264_16s_C1 (  short *pSrc, short *pDst, int Qp6, int *pNumCoeffs, int Intra, short *pScanMatrix, int *pLastCoeff, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdChromaDC2x2_H264_16s_C1I (  short *pDCBuf, short *pTBuf, int QP, int *pNumCoeffs, int Intra, int NeedTransform, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdChromaDC2x2_H264_32s_C1I (  int *pSrcDst, int *pTBlock, int QPChroma, int *NumCoeffs, int Intra, int NeedTransform, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdChromaDC2x4_H264_16s_C1I (  short *pDCBuf, short *pTBuf, int QPChroma, int *NumCoeffs, int Intra, int NeedTransform, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdChromaDC2x4_H264_32s_C1I (  int *pDCBuf, int *pTBuf, int QPChroma, int *NumCoeffs, int Intra, int NeedTransform, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdLumaDC4x4_H264_16s_C1I (  short *pDCBuf, short *pTBuf, int QP, int *pNumCoeffs, int NeedTransform, short *pScanMatrix, int *LastCoeff, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantFwdLumaDC4x4_H264_32s_C1I (  int *pDCBuf, int *pQBuf, int QP, int *NumCoeffs, int Intra, short *pScanMatrix, int *pLastCoeff, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvAddPred4x4_H264_16s_C1IR (  byte *pPred, int predStep, short *pSrcDst, short *pDC, byte *pDst, int dstStep, int QP, int AC, short *pScaleLevelsInv );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvAddPred4x4_H264_32s_C1IR (  ushort *pPred, int predStep, int *pSrcDst, int *pDC, ushort *pDst, int dstStep, int QP, int AC, int bitDepth, short *pScaleLevelsInv );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvChromaDC2x2_H264_16s_C1I (  short *pSrcDst, int QP, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvChromaDC2x2_H264_32s_C1I (  int *pSrcDst, int QP, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvChromaDC2x4_H264_16s_C1I (  short *pSrcDst, int QPChroma, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvChromaDC2x4_H264_32s_C1I (  int *pSrcDst, int QPChroma, short *pScaleLevelsInv );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvLumaDC4x4_H264_16s_C1I (  short *pSrcDst, int QP, short *pScaleLevelsInv );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantInvLumaDC4x4_H264_32s_C1I (  int *pSrcDst, int QP, short *pScaleLevels );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantLumaDC_H264_16s_C1I (  short *pSrcDst, short *pTBlock, int QP, sbyte *pNumLevels, byte needTransform, short *pScanMatrix, byte *pLastCoeff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformQuantResidual_H264_16s_C1I (  short *pSrcDst, int QP, sbyte *pNumLevels, byte intra, short *pScanMatrix, byte *pLastCoeff );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformResidual4x4Fwd_H264_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformResidual4x4Fwd_H264_32s_C1 (  int *pSrc, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformResidual4x4Inv_H264_16s_C1 (  short *pSrc, short *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiTransformResidual4x4Inv_H264_32s_C1 (  int *pSrc, int *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUniDirWeightBlock_H264_8u_C1IR (  byte *pSrcDst, uint srcDstStep, uint ulog2wd, int iWeight, int iOffset, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUniDirWeightBlock_H264_8u_C1R (  byte *pSrcDst, uint srcDstStep, uint ulog2wd, int iWeight, int iOffset, IppiSize roi );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUniDirWeightBlock_H264_8u_C2R(ref IppVCWeightBlock_8u pIppVCWeightBlock, ref IppVCWeightParams_8u pIppVCWeightParams);

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUnidirWeight_H264_16u_IP2P1R (  ushort *pSrcDst, uint srcDstStep, uint ulog2wd, int iWeight, int iOffset, IppiSize roi, int bitDepth );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUpsampleFour8x8_H263_16s_C1R (  short *pSrc, int srcStep, short *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiUpsampleFour_H263_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, int rounding, int fillColor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarMean8x8_16s32s_C1R (  short *pSrc, int srcStep, int *pVar, int *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarMean8x8_8u32s_C1R (  byte *pSrc, int srcStep, int *pVar, int *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarMeanDiff16x16_8u32s_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, int *pSrcSum, int *pVar, int *pMean, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarMeanDiff16x8_8u32s_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, int *pSrcSum, int *pVar, int *pMean, int mcType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarSum8x8_16s32s_C1R (  short *pSrc, int srcStep, int *pVar, int *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVarSum8x8_8u32s_C1R (  byte *pSrc, int srcStep, int *pVar, int *pSum );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiVariance16x16_8u32s (  byte *pSrc, int srcStep, int *pVar );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWarpChroma_MPEG4_8u_P2R (  byte *pSrcCb, int srcStepCb, byte *pSrcCr, int srcStepCr, byte *pDstCb, int dstStepCb, byte *pDstCr, int dstStepCr, IppiRect *dstRect, IppiWarpSpec_MPEG4 *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWarpGetSize_MPEG4 (  int *pSpecSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWarpInit_MPEG4 (  IppiWarpSpec_MPEG4 *pSpec, int *pDU, int *pDV, int numWarpingPoints, int spriteType, int warpingAccuracy, int roundingType, int quarterSample, int fcode, IppiRect *spriteRect, IppiRect *vopRect );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWarpLuma_MPEG4_8u_C1R (  byte *pSrcY, int srcStepY, byte *pDstY, int dstStepY, IppiRect *dstRect, IppiWarpSpec_MPEG4 *pSpec );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWeightPrediction_AVS_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, uint scale, int shift, IppiSize sizeBlock );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiWeightedAverage_H264_8u_C1IR (  byte *pSrc1, byte *pSrc2Dst, int srcDstStep, int weight1, int weight2, int shift, int offset, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_16x4x5MB_DV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstPitch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_8x8MB_DV_16s8u_P3C2R (  short *pSrc, byte *pDst, int dstPitch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_EdgeDV_16s8u_P3C2R (  short *pSrc, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut2_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut2_EdgeDV_16s8u_P3C2R (  short *pSrc, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut4_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut4_EdgeDV_16s8u_P3C2R (  short *pSrc, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut8_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb411ToYCbCr422_ZoomOut8_EdgeDV_16s8u_P3C2R (  short *pSrc, byte *pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_8x8x5MB_DV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstPitch );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_ZoomOut2_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_ZoomOut4_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb420ToYCbCr422_ZoomOut8_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_10HalvesMB16x8_DV100_16s8u_P3C2R (  short *pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_8x4x5MB_DV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_ZoomOut2_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_ZoomOut4_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst, int dstStep );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname)] public static extern
IppStatus ippiYCrCb422ToYCbCr422_ZoomOut8_5MBDV_16s8u_P3C2R (  short **pSrc, byte **pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.vc.libname,EntryPoint="ippvcGetLibVersion")] public static extern
int* xippvcGetLibVersion (    );
public static IppLibraryVersion ippvcGetLibVersion() {
   return new IppLibraryVersion( xippvcGetLibVersion() );
}
};
};
