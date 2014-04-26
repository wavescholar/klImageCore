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

// generated automatically on Wed Feb  3 17:09:12 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IpprIndexType {
      ippNormInd = 3,
      ippTriInd = 4,
   };
   public enum IpprSHType {
      ipprSHNormDirect = 0,
      ipprSHNormRecurr = 1,
   };
   public enum IpprKDTreeBuildAlg {
      ippKDTBuildSimple = 0x499d3dc2,
      ippKDTBuildPureSAH = 0x2d07705b,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IpprSHState {};
   [StructLayout(LayoutKind.Sequential)] public struct IpprTriangleAccel {};
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)]
   unsafe public struct IpprPSAHBldContext {
      public IpprKDTreeBuildAlg Alg;
      public int MaxDepth;
      public float QoS;
      public int AvailMemory;
      public float* Bounds;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)] public struct IpprSmplBldContext {
      public IpprKDTreeBuildAlg Alg;
      public int MaxDepth;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)]
   unsafe public struct IpprIntersectContext {
      public float* pBound;
      public IpprTriangleAccel* pAccel;
      public IpprKDTreeNode* pRootNode;
   };
   [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi)]
   public struct IpprKDTreeNode {
      public int flag_k_ofs;
      [StructLayout(LayoutKind.Explicit,CharSet=CharSet.Ansi)]
      public struct tree_data {
         [FieldOffset(0)] public float split;
         [FieldOffset(0)] public int items;
      }
   };

unsafe public class rr {

   internal const string libname = "ippr-7.1.dll";


//typedef float IppPoint3D_32f[3] 
[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprAddMulMul_32f_AC1P3IM (  float *point, float *pSrc0, float **pSrc1, int *pMask, float **pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprCastEye_32f (  float *imPlaneOrg, float *dW, float *dH, int wB, int hB, IppiSize cBlock, float **pDirection, IppiSize blockSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprCastReflectionRay_32f (  float **pInc, int *pMask, float **pSurfNorm, float **pReflect, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprCastShadowSO_32f (  float *pOrigin, float *pSurfDotIn, float **pSurfNorm, float **pSurfHit, int *pMask, float *pDotRay, float **pDirection, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprDistAttenuationSO_32f_M (  float *point, float **pSurfHit, int *pMask, float *pDist, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprDiv_32f_C1IM (  float *pSrc, int *pMask, float *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprDotChangeNorm_32f_IM (  float **pSrc, int *pMask, float **pSrcDst, float *pDot, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprDot_32f_P3C1M (  float **pSrc0, float **pSrc1, int *pMask, float *pDot, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprFilterGetBufSize (  IpprVolume dstVolume, IpprVolume kernelVolume, int nChannel, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprFilter_16s_C1PV (  short **pSrc, int srcStep, short **pDst, int dstStep, IpprVolume dstVolume, int *pKernel, IpprVolume kernelVolume, IpprPoint anchor, int divisor, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname,EntryPoint="ipprGetLibVersion")] public static extern
int* xipprGetLibVersion (    );
public static IppLibraryVersion ipprGetLibVersion() {
   return new IppLibraryVersion( xipprGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprGetResizeCuboid (  IpprCuboid srcVOI, IpprCuboid *pDstCuboid, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprHitPoint3DEpsM0_32f_M (  float **pOrigin, float **pDirection, float *pDist, int *pMask, float **pSurfHit, int len, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprHitPoint3DEpsS0_32f_M (  float *originEye, float **pDirection, float *pDist, int *pMask, float **pSurfHit, int len, float eps );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprIntersectAnySO_32f (  float *originEye, float **pDirection, int *pOccluder, int *pMask, IppiSize blockSize, IpprIntersectContext *pContext );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprIntersectEyeSO_32f (  float *originEye, float **pDirection, float *pDist, float **pHit, int *pTrngl, IpprIntersectContext *pContext, IppiSize blockSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprIntersectMO_32f (  float **pOrigin, float **pDirection, float *pDist, float **pHit, int *pTrngl, IpprIntersectContext *pContext, IppiSize blockSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprIntersectMultipleSO_32f (  float *originEye, float **pDirection, float *pDistance, float **pHit, int *pTrngl, IpprVolume blockVolume, IpprIntersectContext *pContext );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprKDTreeBuildAlloc (  IpprKDTreeNode **pDstKDTree, float *pSrcVert, int *pSrcTriInx, int SrcVertSize, int SrcTriSize, int *pDstKDTreeSize, char *pBldContext );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
void ipprKDTreeFree (  IpprKDTreeNode *pSrcKDTree );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprMul_32f_C1IM (  float *pSrc, int *pMask, float *pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprMul_32f_C1P3IM (  float *pSrc, int *pMask, float **pSrcDst, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprRemap_16u_C1PV (  ushort **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, float **pxMap, float **pyMap, float **pzMap, int mapStep, ushort **pDst, int dstStep, IpprVolume dstVolume, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprRemap_32f_C1PV (  float **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, float **pxMap, float **pyMap, float **pzMap, int mapStep, float **pDst, int dstStep, IpprVolume dstVolume, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprRemap_8u_C1PV (  byte **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, float **pxMap, float **pyMap, float **pzMap, int mapStep, byte **pDst, int dstStep, IpprVolume dstVolume, int interpolation );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResizeGetBufSize (  IpprCuboid srcVOI, IpprCuboid dstVOI, int nChannel, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_16u_C1PV (  ushort **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, ushort **pDst, int dstStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_16u_C1V (  ushort *pSrc, IpprVolume srcVolume, int srcStep, int srcPlaneStep, IpprCuboid srcVOI, ushort *pDst, int dstStep, int dstPlaneStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_32f_C1PV (  float **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, float **pDst, int dstStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_32f_C1V (  float *pSrc, IpprVolume srcVolume, int srcStep, int srcPlaneStep, IpprCuboid srcVOI, float *pDst, int dstStep, int dstPlaneStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_8u_C1PV (  byte **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, byte **pDst, int dstStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprResize_8u_C1V (  byte *pSrc, IpprVolume srcVolume, int srcStep, int srcPlaneStep, IpprCuboid srcVOI, byte *pDst, int dstStep, int dstPlaneStep, IpprCuboid dstVOI, double xFactor, double yFactor, double zFactor, double xShift, double yShift, double zShift, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHBand_32f (  float *pX, float *pY, float *pZ, uint N, float *pDstBandYlm, uint L );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHGetSize_32f (  uint maxL, IpprSHType shType, uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHInit_32f (  IpprSHState *pSHState, uint maxL, IpprSHType shType );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTFwd_32f_C1I (  float *pX, float *pY, float *pZ, float *pSrc, uint N, float *pSrcDstClm, uint L, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTFwd_32f_C3P3I (  float *pX, float *pY, float *pZ, float *pSrc, uint N, float **pSrcDstClm, uint L, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTFwd_32f_P3I (  float *pX, float *pY, float *pZ, float **pSrc, uint N, float **pSrcDstClm, uint L, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTInv_32f_C1 (  float *pSrcClm, uint L, float *pX, float *pY, float *pZ, float *pDst, uint N, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTInv_32f_P3 (  float **pSrcClm, uint L, float *pX, float *pY, float *pZ, float **pDst, uint N, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSHTInv_32f_P3C3 (  float **pSrcClm, uint L, float *pX, float *pY, float *pZ, float *pDst, uint N, IpprSHState *pSHState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSH_32f (  float *pX, float *pY, float *pZ, uint N, float *pDstYlm, uint L, IpprSHState *pSHState );

//typedef IppPoint3D_32f IppBox3D_32f[2]
[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSetBoundBox_32f (  float *pVertCoor, int lenTri, float *pBound );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSurfFlatNormal_32f (  float *pTrnglNorm, int *pTrngl, float **pSurfNorm, int len );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprSurfSmoothNormal_32f (  float *pVertNorm, int *pIndexNorm, int *pTrngl, float **pHit, float **pSurfNorm, int len, IpprIndexType ippInd );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprTriangleAccelGetSize (  int *pTrnglAccelSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprTriangleAccelInit (  IpprTriangleAccel *pTrnglAccel, float *pVertexCoord, int *pTrnglIndex, int cntTrngl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprTriangleNormal_32f (  float *pTrnglCoor, int *pTrnglIndex, float *pTrnglNorm, int lenTrngl );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprWarpAffineGetBufSize (  IpprCuboid srcVOI, IpprCuboid dstVOI, int nChannel, int interpolation, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprWarpAffine_16u_C1PV (  ushort **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, ushort **pDst, int dstStep, IpprCuboid dstVOI, double *coeffs, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprWarpAffine_32f_C1PV (  float **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, float **pDst, int dstStep, IpprCuboid dstVOI, double *coeffs, int interpolation, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.rr.libname)] public static extern
IppStatus ipprWarpAffine_8u_C1PV (  byte **pSrc, IpprVolume srcVolume, int srcStep, IpprCuboid srcVOI, byte **pDst, int dstStep, IpprCuboid dstVOI, double *coeffs, int interpolation, byte *pBuffer );
};
};
