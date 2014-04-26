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

// generated automatically on Wed Feb  3 16:23:34 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppiKernelType {
      ippKernelSobel = 0,
      ippKernelScharr = 1,
   };
   public enum IppiInpaintFlag {
      IPP_INPAINT_TELEA = 0,
      IPP_INPAINT_NS = 1,
   };
   public enum IppiNorm {
      ippiNormInf = 0,
      ippiNormL1 = 1,
      ippiNormL2 = 2,
      ippiNormFM = 3,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppFGGaussianState_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppFGGaussianState_8u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppFGHistogramState_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppFGHistogramState_8u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiConvState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiHaarClassifier_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiHaarClassifier_32s {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiInpaintState_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiInpaintState_8u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMorphAdvState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMorphGrayState_32f {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMorphGrayState_8u {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiMorphState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiOptFlowPyrLK_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiOptFlowPyrLK_16u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiOptFlowPyrLK_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_16u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_8u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_16u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidDownState_32f_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_8u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_16u_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_32f_C1R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_8u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_16u_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiPyramidUpState_32f_C3R {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiSRHNSpec_PSF2x2 {};
   [StructLayout(LayoutKind.Sequential)] public struct IppiSRHNSpec_PSF3x3 {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)]
   unsafe public struct IppiPyramid {
      public byte** pImage;
      public IppiSize* pRoi;
      public double* pRate;
      public int* pStep;
      public byte* pState;
      public int level;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppFGGaussianModel {
      public int numGauss;
      public float priorBack;
      public float updBGProb;
      public int winSize;
      public int numFrame;
      public float detectionRate;
      public float brightnessDistortion;
      public int shadowBG;
   public IppFGGaussianModel ( int numGauss, float priorBack, float updBGProb, int winSize, int numFrame, float detectionRate, float brightnessDistortion, int shadowBG ) {
      this.numGauss = numGauss;
      this.priorBack = priorBack;
      this.updBGProb = updBGProb;
      this.winSize = winSize;
      this.numFrame = numFrame;
      this.detectionRate = detectionRate;
      this.brightnessDistortion = brightnessDistortion;
      this.shadowBG = shadowBG;
   }};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppiConnectedComp {
      public double area;
      [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
      public double[] value;
      public IppiRect rect;
      public IppiConnectedComp(double area)
      {
         this.area = area;
         value = new double[3];
         rect = new IppiRect();
      }
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IppFGHistogramModel {
      public int valueQuant;
      public int changeQuant;
      public int valueUse;
      public int valueAll;
      public int changeUse;
      public int changeAll;
      public float updBGChange;
      public float updBGRef;
      public int numFrame;
      public float detectionRate;
      public float brightnessDistortion;
      public int shadowBG;
   public IppFGHistogramModel ( int valueQuant, int changeQuant, int valueUse, int valueAll, int changeUse, int changeAll, float updBGChange, float updBGRef, int numFrame, float detectionRate, float brightnessDistortion, int shadowBG ) {
      this.valueQuant = valueQuant;
      this.changeQuant = changeQuant;
      this.valueUse = valueUse;
      this.valueAll = valueAll;
      this.changeUse = changeUse;
      this.changeAll = changeAll;
      this.updBGChange = updBGChange;
      this.updBGRef = updBGRef;
      this.numFrame = numFrame;
      this.detectionRate = detectionRate;
      this.brightnessDistortion = brightnessDistortion;
      this.shadowBG = shadowBG;
   }};

unsafe public class cv {

   internal const string libname = "ippcv-7.1.dll";


[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname,EntryPoint="ippcvGetLibVersion")] public static extern
int* xippcvGetLibVersion (    );
public static IppLibraryVersion ippcvGetLibVersion() {
   return new IppLibraryVersion( xippcvGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiffC_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiffC_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiffC_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int value );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiff_16u_C1R (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, ushort *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiff_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiff_8u_C1R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAbsDiff_8u_C3R (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pDst, int dstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_16u32f_C1IMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_16u32f_C1IR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_32f_C1IMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_32f_C1IR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_8s32f_C1IMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_8s32f_C1IR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_8u32f_C1IMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddProduct_8u32f_C1IR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_16u32f_C1IMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_16u32f_C1IR (  ushort *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_32f_C1IMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_8s32f_C1IMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_8s32f_C1IR (  sbyte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_8u32f_C1IMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddSquare_8u32f_C1IR (  byte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_16u32f_C1IMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_16u32f_C1IR (  ushort *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_32f_C1IMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_32f_C1R (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, float *pDst, int dstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_8s32f_C1IMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_8s32f_C1IR (  sbyte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_8u32f_C1IMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAddWeighted_8u32f_C1IR (  byte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float alpha );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_16u32f_C1IMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_16u32f_C1IR (  ushort *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_32f_C1IMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_8s32f_C1IMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_8s32f_C1IR (  sbyte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_8u32f_C1IMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiAdd_8u32f_C1IR (  byte *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyHaarClassifier_32f_C1R (  float *pSrc, int srcStep, float *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, float threshold, IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyHaarClassifier_32s32f_C1R (  int *pSrc, int srcStep, float *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, float threshold, IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyHaarClassifier_32s_C1RSfs (  int *pSrc, int srcStep, int *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, int threshold, IppiHaarClassifier_32s *pState, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyMixedHaarClassifier_32f_C1R (  float *pSrc, int srcStep, float *pTilt, int tiltStep, float *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, float threshold, IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyMixedHaarClassifier_32s32f_C1R (  int *pSrc, int srcStep, int *pTilt, int tiltStep, float *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, float threshold, IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiApplyMixedHaarClassifier_32s_C1RSfs (  int *pSrc, int srcStep, int *pTilt, int tiltStep, int *pNorm, int normStep, byte *pMask, int maskStep, IppiSize roiSize, int *pPositive, int threshold, IppiHaarClassifier_32s *pState, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiBoundSegments_16u_C1IR (  ushort *pMarker, int markerStep, IppiSize roiSize, ushort val, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiBoundSegments_8u_C1IR (  byte *pMarker, int markerStep, IppiSize roiSize, byte val, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCannyGetSize (  IppiSize roiSize, int *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCanny_16s8u_C1R (  short *pSrcDx, int srcDxStep, short *pSrcDy, int srcDyStep, byte *pDstEdges, int dstEdgeStep, IppiSize roiSize, float lowThresh, float highThresh, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCanny_32f8u_C1R (  float *pSrcDx, int srcDxStep, float *pSrcDy, int srcDyStep, byte *pDstEdges, int dstEdgeStep, IppiSize roiSize, float lowThresh, float highThresh, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_16u32f_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_8u16u_C1R_Sfs (  byte *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpixIntersect_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiPoint_32f point, IppiPoint *pMin, IppiPoint *pMax );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_16u32f_C1R (  ushort *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float dx, float dy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float dx, float dy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float dx, float dy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_8u16u_C1R_Sfs (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float dx, float dy, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float dx, float dy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCopySubpix_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float dx, float dy );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiCreateMapCameraUndistort_32f_C1R (  float *pxMap, int xStep, float *pyMap, int yStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, float p1, float p2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDilateBorderReplicate_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_3x3_8u16u_C1R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_3x3_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_3x3_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_3x3_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_5x5_8u16u_C1R (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_5x5_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_5x5_8u_C1IR (  byte *pSrcDst, int srcDstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiDistanceTransform_5x5_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiEigenValsVecsGetBufferSize_32f_C1R (  IppiSize roiSize, int apertureSize, int avgWindow, int *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiEigenValsVecsGetBufferSize_8u32f_C1R (  IppiSize roiSize, int apertureSize, int avgWindow, int *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiEigenValsVecs_32f_C1R (  float *pSrc, int srcStep, float *pEigenVV, int eigStep, IppiSize roiSize, IppiKernelType kernType, int apertureSize, int avgWindow, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiEigenValsVecs_8u32f_C1R (  byte *pSrc, int srcStep, float *pEigenVV, int eigStep, IppiSize roiSize, IppiKernelType kernType, int apertureSize, int avgWindow, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiErodeBorderReplicate_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFastMarchingGetBufferSize_8u32f_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFastMarching_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float radius, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s8s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s8s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s8u_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s8u_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_16s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_32f_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_32f_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_Low_16s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipelineGetBufferSize_Low_16s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s8s_C1R (  short **ppSrc, sbyte *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s8s_C3R (  short **ppSrc, sbyte *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s8u_C1R (  short **ppSrc, byte *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s8u_C3R (  short **ppSrc, byte *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s_C1R (  short **ppSrc, short *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_16s_C3R (  short **ppSrc, short *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_32f_C1R (  float **ppSrc, float *pDst, int dstStep, IppiSize roiSize, float *pKernel, int kernelSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_32f_C3R (  float **ppSrc, float *pDst, int dstStep, IppiSize roiSize, float *pKernel, int kernelSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_Low_16s_C1R (  short **ppSrc, short *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterColumnPipeline_Low_16s_C3R (  short **ppSrc, short *pDst, int dstStep, IppiSize roiSize, short *pKernel, int kernelSize, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterGaussBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, int KernelSize, float sigma, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterGaussGetBufferSize_32f_C1R (  IppiSize roiSize, int KernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLaplacianGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLowpassBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLowpassBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLowpassGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterLowpassGetBufferSize_8u_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxBorderReplicate_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_32f_C1R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_32f_C3R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_32f_C4R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_8u_C1R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_8u_C3R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMaxGetBufferSize_8u_C4R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinBorderReplicate_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiSize maskSize, IppiPoint anchor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_32f_C1R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_32f_C3R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_32f_C4R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_8u_C1R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_8u_C3R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterMinGetBufferSize_8u_C4R (  int roiWidth, IppiSize maskSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_16s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_16s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_32f_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_32f_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_8u16s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_8u16s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_Low_16s_C1R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipelineGetBufferSize_Low_16s_C3R (  IppiSize roiSize, int kernelSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_16s_C1R (  short *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, short borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_16s_C3R (  short *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, short *borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_32f_C1R (  float *pSrc, int srcStep, float **ppDst, IppiSize roiSize, float *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_32f_C3R (  float *pSrc, int srcStep, float **ppDst, IppiSize roiSize, float *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, float *borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_8u16s_C1R (  byte *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_8u16s_C3R (  byte *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, byte *borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_Low_16s_C1R (  short *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, short borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterRowBorderPipeline_Low_16s_C3R (  short *pSrc, int srcStep, short **ppDst, IppiSize roiSize, short *pKernel, int kernelSize, int xAnchor, IppiBorderType borderType, short *borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizGetBufferSize_32f_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizGetBufferSize_8u16s_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrHorizGetBufferSize_8u8s_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertGetBufferSize_32f_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertGetBufferSize_8u16s_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterScharrVertGetBufferSize_8u8s_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelCrossGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelHorizSecondGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelNegVertGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, float borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondBorder_8u16s_C1R (  byte *pSrc, int srcStep, short *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondBorder_8u8s_C1R (  byte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask, IppiBorderType borderType, byte borderValue, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondGetBufferSize_32f_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondGetBufferSize_8u16s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFilterSobelVertSecondGetBufferSize_8u8s_C1R (  IppiSize roiSize, IppiMaskSize mask, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFindPeaks3x3GetBufferSize_32f_C1R (  int roiWidth, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFindPeaks3x3GetBufferSize_32s_C1R (  int roiWidth, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFindPeaks3x3_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float threshold, IppiPoint *pPeak, int maxPeakCount, int *pPeakCount, IppiNorm norm, int Border, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFindPeaks3x3_32s_C1R (  int *pSrc, int srcStep, IppiSize roiSize, int threshold, IppiPoint *pPeak, int maxPeakCount, int *pPeakCount, IppiNorm norm, int Border, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFillGetSize (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFillGetSize_Grad (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_4Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_8Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ushort minDelta, ushort maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ushort *pMinDelta, ushort *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, float minDelta, float maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, float *pMinDelta, float *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, byte minDelta, byte maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad4Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, byte *pMinDelta, byte *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ushort minDelta, ushort maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ushort *pMinDelta, ushort *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, float minDelta, float maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, float *pMinDelta, float *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, byte minDelta, byte maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Grad8Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, byte *pMinDelta, byte *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ushort minDelta, ushort maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ushort *pMinDelta, ushort *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, float minDelta, float maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, float *pMinDelta, float *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, byte minDelta, byte maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range4Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, byte *pMinDelta, byte *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_16u_C1IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort newVal, ushort minDelta, ushort maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_16u_C3IR (  ushort *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, ushort *pNewVal, ushort *pMinDelta, ushort *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_32f_C1IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float newVal, float minDelta, float maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_32f_C3IR (  float *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, float *pNewVal, float *pMinDelta, float *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_8u_C1IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte newVal, byte minDelta, byte maxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiFloodFill_Range8Con_8u_C3IR (  byte *pImage, int imageStep, IppiSize roiSize, IppiPoint seed, byte *pNewVal, byte *pMinDelta, byte *pMaxDelta, ref IppiConnectedComp pRegion, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussianFree_8u_C1R (  IppFGGaussianState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussianFree_8u_C3R (  IppFGGaussianState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussianInitAlloc_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppFGGaussianModel *pModel, IppFGGaussianState_8u_C1R **pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussianInitAlloc_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, IppFGGaussianModel *pModel, IppFGGaussianState_8u_C3R **pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussian_8u_C1R (  byte *pSrc, int srcStep, byte *pRef, int refStep, byte *pDst, int dstStep, IppiSize roiSize, IppFGGaussianState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundGaussian_8u_C3R (  byte *pSrc, int srcStep, byte *pRef, int refStep, byte *pDst, int dstStep, IppiSize roiSize, IppFGGaussianState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramFree_8u_C1R (  IppFGHistogramState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramFree_8u_C3R (  IppFGHistogramState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramInitAlloc_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppFGHistogramModel *pModel, IppFGHistogramState_8u_C1R **pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramInitAlloc_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, IppFGHistogramModel *pModel, IppFGHistogramState_8u_C3R **pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramUpdate_8u_C1R (  byte *pSrc, int srcStep, byte *pMask, int maskStep, byte *pRef, int refStep, IppiSize roiSize, IppFGHistogramState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogramUpdate_8u_C3R (  byte *pSrc, int srcStep, byte *pMask, int maskStep, byte *pRef, int refStep, IppiSize roiSize, IppFGHistogramState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogram_8u_C1R (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, IppFGHistogramState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiForegroundHistogram_8u_C3R (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, IppFGHistogramState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGenSobelKernel_16s (  short *pDst, int kernelSize, int dx, int sign );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGenSobelKernel_32f (  float *pDst, int kernelSize, int dx, int sign );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetDistanceTransformMask (  int maskType, float *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetDistanceTransformMask_32f (  int kerSize, IppiNorm norm, float *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetDistanceTransformMask_32s (  int kerSize, IppiNorm norm, int *pMetrics );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetHaarClassifierSize_32f (  IppiHaarClassifier_32f *pState, IppiSize *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetHaarClassifierSize_32s (  IppiHaarClassifier_32s *pState, IppiSize *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetPyramidDownROI (  IppiSize srcRoi, IppiSize *pDstRoi, float rate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGetPyramidUpROI (  IppiSize srcRoi, IppiSize *pDstRoiMin, IppiSize *pDstRoiMax, float rate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGradientColorToGray_16u_C3C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGradientColorToGray_32f_C3C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGradientColorToGray_8u_C3C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGrayDilateBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType border, IppiMorphGrayState_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGrayDilateBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType border, IppiMorphGrayState_8u *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGrayErodeBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType border, IppiMorphGrayState_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiGrayErodeBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType border, IppiMorphGrayState_8u *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHaarClassifierFree_32f (  IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHaarClassifierFree_32s (  IppiHaarClassifier_32s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHaarClassifierInitAlloc_32f (  IppiHaarClassifier_32f **pState, IppiRect *pFeature, float *pWeight, float *pThreshold, float *pVal1, float *pVal2, int *pNum, int length );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHaarClassifierInitAlloc_32s (  IppiHaarClassifier_32s **pState, IppiRect *pFeature, int *pWeight, int *pThreshold, int *pVal1, int *pVal2, int *pNum, int length );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHoughLineGetSize_8u_C1R (  IppiSize roiSize, IppPointPolar delta, int maxLineCount, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHoughLine_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppPointPolar delta, int threshold, IppPointPolar *pLine, int maxLineCount, int *pLineCount, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiHoughLine_Region_8u32f_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, IppPointPolar *pLine, IppPointPolar *dstRoi, int maxLineCount, int *pLineCount, IppPointPolar delta, int threshold, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaintFree_8u_C1R (  IppiInpaintState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaintFree_8u_C3R (  IppiInpaintState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaintInitAlloc_8u_C1R (  IppiInpaintState_8u_C1R **ppState, float *pDist, int distStep, byte *pMask, int maskStep, IppiSize roiSize, float radius, IppiInpaintFlag flags );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaintInitAlloc_8u_C3R (  IppiInpaintState_8u_C3R **ppState, float *pDist, int distStep, byte *pMask, int maskStep, IppiSize roiSize, float radius, IppiInpaintFlag flags );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaint_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiInpaintState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiInpaint_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiInpaintState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiIntegral_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiIntegral_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiLabelMarkersGetBufferSize_16u_C1R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiLabelMarkersGetBufferSize_8u_C1R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiLabelMarkers_16u_C1IR (  ushort *pMarker, int markerStep, IppiSize roiSize, int minLabel, int maxLabel, IppiNorm norm, int *pNumber, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiLabelMarkers_8u_C1IR (  byte *pMarker, int markerStep, IppiSize roiSize, int minLabel, int maxLabel, IppiNorm norm, int *pNumber, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_16u_C3CR (  ushort *pSrc, int srcStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_32f_C3CR (  float *pSrc, int srcStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8s_C1R (  sbyte *pSrc, int srcStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8s_C3CR (  sbyte *pSrc, int srcStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMean_StdDev_8u_C3CR (  byte *pSrc, int srcStep, IppiSize roiSize, int coi, double *pMean, double *pStdDev );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinEigenValGetBufferSize_32f_C1R (  IppiSize roiSize, int apertureSize, int avgWindow, int *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinEigenValGetBufferSize_8u32f_C1R (  IppiSize roiSize, int apertureSize, int avgWindow, int *bufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinEigenVal_32f_C1R (  float *pSrc, int srcStep, float *pMinEigenVal, int minValStep, IppiSize roiSize, IppiKernelType kernType, int apertureSize, int avgWindow, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinEigenVal_8u32f_C1R (  byte *pSrc, int srcStep, float *pMinEigenVal, int minValStep, IppiSize roiSize, IppiKernelType kernType, int apertureSize, int avgWindow, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_16u_C3CR (  ushort *pSrc, int srcStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_32f_C1R (  float *pSrc, int step, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_32f_C3CR (  float *pSrc, int step, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8s_C1R (  sbyte *pSrc, int step, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8s_C3CR (  sbyte *pSrc, int step, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMinMaxIndx_8u_C3CR (  byte *pSrc, int srcStep, IppiSize roiSize, int coi, float *pMinVal, float *pMaxVal, IppiPoint *pMinIndex, IppiPoint *pMaxIndex );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvFree (  IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_32f_C1R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_32f_C3R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_32f_C4R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_8u_C1R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_8u_C3R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvGetSize_8u_C4R (  IppiSize roiSize, byte *pMask, IppiSize maskSize, int *stateSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_32f_C1R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_32f_C3R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_32f_C4R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_8u_C1R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_8u_C3R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInitAlloc_8u_C4R (  IppiMorphAdvState **morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_32f_C1R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_32f_C3R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_32f_C4R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_8u_C1R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_8u_C3R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphAdvInit_8u_C4R (  IppiMorphAdvState *morphState, IppiSize roiSize, byte *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphBlackhatBorder_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphCloseBorder_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGradientBorder_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayFree_32f_C1R (  IppiMorphGrayState_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayFree_8u_C1R (  IppiMorphGrayState_8u *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayGetSize_32f_C1R (  IppiSize roiSize, float *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayGetSize_8u_C1R (  IppiSize roiSize, int *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayInitAlloc_32f_C1R (  IppiMorphGrayState_32f **ppState, IppiSize roiSize, float *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayInitAlloc_8u_C1R (  IppiMorphGrayState_8u **ppState, IppiSize roiSize, int *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayInit_32f_C1R (  IppiMorphGrayState_32f *pState, IppiSize roiSize, float *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphGrayInit_8u_C1R (  IppiMorphGrayState_8u *pState, IppiSize roiSize, int *pMask, IppiSize maskSize, IppiPoint anchor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphOpenBorder_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructDilate_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructDilate_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructDilate_64f_C1IR (  double *pSrc, int srcStep, double *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructDilate_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructErode_16u_C1IR (  ushort *pSrc, int srcStep, ushort *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructErode_32f_C1IR (  float *pSrc, int srcStep, float *pSrcDst, int srcDstStep, IppiSize roiSize, float *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructErode_64f_C1IR (  double *pSrc, int srcStep, double *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructErode_8u_C1IR (  byte *pSrc, int srcStep, byte *pSrcDst, int srcDstStep, IppiSize roiSize, byte *pBuf, IppiNorm norm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructGetBufferSize_16u_C1 (  IppiSize roiSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructGetBufferSize_32f_C1 (  IppiSize roiSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructGetBufferSize_64f_C1 (  IppiSize roiSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphReconstructGetBufferSize_8u_C1 (  IppiSize roiSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_32f_C4R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphTophatBorder_8u_C4R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, IppiBorderType borderType, IppiMorphAdvState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyFree (  IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_32f_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_32f_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_32f_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_8u_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_8u_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyGetSize_8u_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_32f_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_32f_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_32f_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_8u_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_8u_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInitAlloc_8u_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState **ppState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_32f_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_32f_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_32f_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_8u_C1R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_8u_C3R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiMorphologyInit_8u_C4R (  int roiWidth, byte *pMask, IppiSize maskSize, IppiPoint anchor, IppiMorphState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_Inf_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L1_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormDiff_L2_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_Inf_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L1_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_16u_C1MR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_16u_C3CMR (  ushort *pSrc1, int src1Step, ushort *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_32f_C1MR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_32f_C3CMR (  float *pSrc1, int src1Step, float *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_8s_C1MR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_8s_C3CMR (  sbyte *pSrc1, int src1Step, sbyte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_8u_C1MR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNormRel_L2_8u_C3CMR (  byte *pSrc1, int src1Step, byte *pSrc2, int src2Step, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_Inf_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L1_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_16u_C1MR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_16u_C3CMR (  ushort *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_32f_C1MR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_32f_C3CMR (  float *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_8s_C1MR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_8s_C3CMR (  sbyte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_8u_C1MR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiNorm_L2_8u_C3CMR (  byte *pSrc, int srcStep, byte *pMask, int maskStep, IppiSize roiSize, int coi, double *pNorm );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKFree_16u_C1R (  IppiOptFlowPyrLK_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKFree_32f_C1R (  IppiOptFlowPyrLK_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKFree_8u_C1R (  IppiOptFlowPyrLK_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKInitAlloc_16u_C1R (  IppiOptFlowPyrLK_16u_C1R **ppState, IppiSize roiSize, int winSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKInitAlloc_32f_C1R (  IppiOptFlowPyrLK_32f_C1R **ppState, IppiSize roiSize, int winSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLKInitAlloc_8u_C1R (  IppiOptFlowPyrLK_8u_C1R **ppState, IppiSize roiSize, int winSize, IppHintAlgorithm hint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLK_16u_C1R (  IppiPyramid *pPyr1, IppiPyramid *pPyr2, IppiPoint_32f *pPrev, IppiPoint_32f *pNext, sbyte *pStatus, float *pError, int numFeat, int winSize, int maxLev, int maxIter, float threshold, IppiOptFlowPyrLK_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLK_32f_C1R (  IppiPyramid *pPyr1, IppiPyramid *pPyr2, IppiPoint_32f *pRrev, IppiPoint_32f *pNext, sbyte *pStatus, float *pError, int numFeat, int winSize, int maxLev, int maxIter, float threshold, IppiOptFlowPyrLK_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiOpticalFlowPyrLK_8u_C1R (  IppiPyramid *pPyr1, IppiPyramid *pPyr2, IppiPoint_32f *pPrev, IppiPoint_32f *pNext, sbyte *pStatus, float *pError, int numFeat, int winSize, int maxLev, int maxIter, float threshold, IppiOptFlowPyrLK_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDownGetBufSize_Gauss5x5 (  int roiWidth, IppDataType dataType, int channels, int *bufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_8s_C1R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_8s_C3R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrDown_Gauss5x5_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUpGetBufSize_Gauss5x5 (  int roiWidth, IppDataType dataType, int channels, int *bufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_8s_C1R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_8s_C3R (  sbyte *pSrc, int srcStep, sbyte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyrUp_Gauss5x5_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidFree (  IppiPyramid *pPyr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidInitAlloc (  IppiPyramid **ppPyr, int level, IppiSize roiSize, float rate );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_16u_C1R (  IppiPyramidDownState_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_16u_C3R (  IppiPyramidDownState_16u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_32f_C1R (  IppiPyramidDownState_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_32f_C3R (  IppiPyramidDownState_32f_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_8u_C1R (  IppiPyramidDownState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownFree_8u_C3R (  IppiPyramidDownState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_16u_C1R (  IppiPyramidDownState_16u_C1R **ppState, IppiSize srcRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_16u_C3R (  IppiPyramidDownState_16u_C3R **ppState, IppiSize srcRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_32f_C1R (  IppiPyramidDownState_32f_C1R **ppState, IppiSize srcRoi, float rate, float *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_32f_C3R (  IppiPyramidDownState_32f_C3R **ppState, IppiSize srcRoi, float rate, float *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_8u_C1R (  IppiPyramidDownState_8u_C1R **ppState, IppiSize srcRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDownInitAlloc_8u_C3R (  IppiPyramidDownState_8u_C3R **ppState, IppiSize srcRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_16u_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_16u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_32f_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerDown_8u_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidDownState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_16u_C1R (  IppiPyramidUpState_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_16u_C3R (  IppiPyramidUpState_16u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_32f_C1R (  IppiPyramidUpState_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_32f_C3R (  IppiPyramidUpState_32f_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_8u_C1R (  IppiPyramidUpState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpFree_8u_C3R (  IppiPyramidUpState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_16u_C1R (  IppiPyramidUpState_16u_C1R **ppState, IppiSize dstRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_16u_C3R (  IppiPyramidUpState_16u_C3R **ppState, IppiSize dstRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_32f_C1R (  IppiPyramidUpState_32f_C1R **ppState, IppiSize dstRoi, float rate, float *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_32f_C3R (  IppiPyramidUpState_32f_C3R **ppState, IppiSize dstRoi, float rate, float *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_8u_C1R (  IppiPyramidUpState_8u_C1R **ppState, IppiSize dstRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUpInitAlloc_8u_C3R (  IppiPyramidUpState_8u_C3R **ppState, IppiSize dstRoi, float rate, short *pKernel, int kerSize, int mode );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_16u_C1R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_16u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_16u_C3R (  ushort *pSrc, int srcStep, IppiSize srcRoiSize, ushort *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_16u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_32f_C1R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_32f_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_32f_C3R (  float *pSrc, int srcStep, IppiSize srcRoiSize, float *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_32f_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_8u_C1R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_8u_C1R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiPyramidLayerUp_8u_C3R (  byte *pSrc, int srcStep, IppiSize srcRoiSize, byte *pDst, int dstStep, IppiSize dstRoiSize, IppiPyramidUpState_8u_C3R *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiRectStdDev_32f_C1R (  float *pSrc, int srcStep, double *pSqr, int sqrStep, float *pDst, int dstStep, IppiSize roiSize, IppiRect rect );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiRectStdDev_32s32f_C1R (  int *pSrc, int srcStep, double *pSqr, int sqrStep, float *pDst, int dstStep, IppiSize roiSize, IppiRect rect );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiRectStdDev_32s_C1RSfs (  int *pSrc, int srcStep, int *pSqr, int sqrStep, int *pDst, int dstStep, IppiSize roi, IppiRect rect, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNCalcResidual_PSF2x2_16u32f_C1R (  float *pHiRes, int hiResStep, ushort *pLowRes, int *pOfs, ushort *pCoeff, float *pResidual, int len, IppiSRHNSpec_PSF2x2 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNCalcResidual_PSF2x2_8u32f_C1R (  float *pHiRes, int hiResStep, byte *pLowRes, int *pOfs, ushort *pCoeff, float *pResidual, int len, IppiSRHNSpec_PSF2x2 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNCalcResidual_PSF3x3_16u32f_C1R (  float *pHiRes, int hiResStep, ushort *pLowRes, int *pOfs, ushort *pCoeff, float *pResidual, int len, IppiSRHNSpec_PSF3x3 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNCalcResidual_PSF3x3_8u32f_C1R (  float *pHiRes, int hiResStep, byte *pLowRes, int *pOfs, ushort *pCoeff, float *pResidual, int len, IppiSRHNSpec_PSF3x3 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNFree_PSF2x2 (  IppiSRHNSpec_PSF2x2 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNFree_PSF3x3 (  IppiSRHNSpec_PSF3x3 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNInitAlloc_PSF2x2 (  IppiSRHNSpec_PSF2x2 **ppPSF, float *pcTab, int tabSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNInitAlloc_PSF3x3 (  IppiSRHNSpec_PSF3x3 **ppPSF, float *pcTab, int tabSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNUpdateGradient_PSF2x2_32f_C1R (  float *pGrad, int gradStep, int *pOfs, ushort *pCoeff, float *pRob, byte *pWeight, int len, float *pwTab, IppiSRHNSpec_PSF2x2 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSRHNUpdateGradient_PSF3x3_32f_C1R (  float *pGrad, int gradStep, int *pOfs, ushort *pCoeff, float *pRob, byte *pWeight, int len, float *pwTab, IppiSRHNSpec_PSF3x3 *pPSF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_16u_C1R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_16u_C3R (  ushort *pSrc, int srcStep, IppiSize roiSize, ushort *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_32f_C1R (  float *pSrc, int srcStep, IppiSize roiSize, float *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_32f_C3R (  float *pSrc, int srcStep, IppiSize roiSize, float *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_8u_C1R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSampleLine_8u_C3R (  byte *pSrc, int srcStep, IppiSize roiSize, byte *pDst, IppiPoint pt1, IppiPoint pt2 );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentGradientGetBufferSize_8u_C1R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentGradientGetBufferSize_8u_C3R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentGradient_8u_C1IR (  byte *pSrc, int srcStep, byte *pMarker, int markerStep, IppiSize roiSize, IppiNorm norm, int flags, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentGradient_8u_C3IR (  byte *pSrc, int srcStep, byte *pMarker, int markerStep, IppiSize roiSize, IppiNorm norm, int flags, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentWatershedGetBufferSize_8u16u_C1R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentWatershedGetBufferSize_8u_C1R (  IppiSize roiSize, int *pBufSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentWatershed_8u16u_C1IR (  byte *pSrc, int srcStep, ushort *pMarker, int markerStep, IppiSize roiSize, IppiNorm norm, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSegmentWatershed_8u_C1IR (  byte *pSrc, int srcStep, byte *pMarker, int markerStep, IppiSize roiSize, IppiNorm norm, int flag, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSqrIntegral_8u32f64f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, double *pSqr, int sqrStep, IppiSize roiSize, float val, double valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSqrIntegral_8u32s64f_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, double *pSqr, int sqrStep, IppiSize roiSize, int val, double valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiSqrIntegral_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, int *pSqr, int sqrStep, IppiSize roi, int val, int valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltHaarFeatures_32f (  byte *pMask, int flag, IppiHaarClassifier_32f *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltHaarFeatures_32s (  byte *pMask, int flag, IppiHaarClassifier_32s *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedHaarClassifierInitAlloc_32f (  IppiHaarClassifier_32f **pState, IppiRect *pFeature, float *pWeight, float *pThreshold, float *pVal1, float *pVal2, int *pNum, int length );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedHaarClassifierInitAlloc_32s (  IppiHaarClassifier_32s **pState, IppiRect *pFeature, int *pWeight, int *pThreshold, int *pVal1, int *pVal2, int *pNum, int length );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedIntegral_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedIntegral_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, IppiSize roiSize, int val );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedRectStdDev_32f_C1R (  float *pSrc, int srcStep, double *pSqr, int sqrStep, float *pDst, int dstStep, IppiSize roiSize, IppiRect rect );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedRectStdDev_32s32f_C1R (  int *pSrc, int srcStep, double *pSqr, int sqrStep, float *pDst, int dstStep, IppiSize roiSize, IppiRect rect );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedRectStdDev_32s_C1RSfs (  int *pSrc, int srcStep, int *pSqr, int sqrStep, int *pDst, int dstStep, IppiSize roi, IppiRect rect, int scaleFactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedSqrIntegral_8u32f64f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, double *pSqr, int sqrStep, IppiSize roiSize, float val, double valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedSqrIntegral_8u32s64f_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, double *pSqr, int sqrStep, IppiSize roiSize, int val, double valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTiltedSqrIntegral_8u32s_C1R (  byte *pSrc, int srcStep, int *pDst, int dstStep, int *pSqr, int sqrStep, IppiSize roi, int val, int valSqr );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTrueDistanceTransformGetBufferSize_8u16u_C1RSfs (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTrueDistanceTransformGetBufferSize_8u32f_C1R (  IppiSize roiSize, int *pBufferSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTrueDistanceTransform_8u16u_C1RSfs (  byte *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, int scaleFactor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiTrueDistanceTransform_8u32f_C1R (  byte *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortGetSize (  IppiSize roiSize, int *pBufsize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_16u_C1R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_16u_C3R (  ushort *pSrc, int srcStep, ushort *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_32f_C1R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_32f_C3R (  float *pSrc, int srcStep, float *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_8u_C1R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUndistortRadial_8u_C3R (  byte *pSrc, int srcStep, byte *pDst, int dstStep, IppiSize roiSize, float fx, float fy, float cx, float cy, float k1, float k2, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUpdateMotionHistory_16u32f_C1IR (  ushort *pSilhouette, int silhStep, float *pMHI, int mhiStep, IppiSize roiSize, float timestamp, float mhiDuration );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUpdateMotionHistory_32f_C1IR (  float *pSilhouette, int silhStep, float *pMHI, int mhiStep, IppiSize roiSize, float timestamp, float mhiDuration );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippiUpdateMotionHistory_8u32f_C1IR (  byte *pSilhouette, int silhStep, float *pMHI, int mhiStep, IppiSize roiSize, float timestamp, float mhiDuration );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cv.libname)] public static extern
IppStatus ippibFastArctan_32f (  float *pSrcY, float *pSrcX, float *pDst, int length );
};
};
