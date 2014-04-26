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

// generated automatically on Fri Jul  9 10:36:54 2010

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace ipp {

//
// enums
//
   public enum IppsRijndaelKeyLength {
      IppsRijndaelKey128 = 128,
      IppsRijndaelKey192 = 192,
      IppsRijndaelKey256 = 256,
   };
   public enum IppsCPPadding {
      NONE = 0,
      IppsCPPaddingNONE = 0,
      PKCS7 = 1,
      IppsCPPaddingPKCS7 = 1,
      ZEROS = 2,
      IppsCPPaddingZEROS = 2,
   };
   public enum IppsExpMethod {
      IppsBinaryMethod = 0,
      IppsSlidingWindows = 1,
   };
   public enum IppsElementCmpResult {
      IppsElementEQ = 0,
      IppsElementNE = 1,
      IppsElementGT = 2,
      IppsElementLT = 3,
      IppsElementNA = 4,
   };
   public enum IppECResult {
      ippECValid = 0,
      ippECCompositeBase = 1,
      ippECComplicatedBase = 2,
      ippECIsZeroDiscriminant = 3,
      ippECCompositeOrder = 4,
      ippECInvalidOrder = 5,
      ippECIsWeakMOV = 6,
      ippECIsWeakSSSA = 7,
      ippECIsSupersingular = 8,
      ippECInvalidPrivateKey = 9,
      ippECInvalidPublicKey = 10,
      ippECInvalidKeyPair = 11,
      ippECPointOutOfGroup = 12,
      ippECPointIsAtInfinite = 13,
      ippECPointIsNotValid = 14,
      ippECPointIsEqual = 15,
      ippECPointIsNotEqual = 16,
      ippECInvalidSignature = 17,
   };
   public enum IppAESGCMbehaviour {
      ippAESGCMdefault = 0,
      ippAESGCMsafe = 1,
      ippAESGCMtable2K = 2,
   };
   public enum IppRSAKeyType {
      IppRSApublic = 536870912,
      IppRSAprivate = 1073741824,
   };
   public enum IppECCType {
      IppECCArbitrary = 0,
      IppECCPStd = 65536,
      IppECCPStd112r1 = 65536,
      IppECCPStd112r2 = 65537,
      IppECCPStd128r1 = 65538,
      IppECCPStd128r2 = 65539,
      IppECCPStd160r1 = 65540,
      IppECCPStd160r2 = 65541,
      IppECCPStd192r1 = 65542,
      IppECCPStd224r1 = 65543,
      IppECCPStd256r1 = 65544,
      IppECCPStd384r1 = 65545,
      IppECCPStd521r1 = 65546,
      IppECCBStd = 131072,
      IppECCBStd113r1 = 131072,
      IppECCBStd113r2 = 131073,
      IppECCBStd131r1 = 131074,
      IppECCBStd131r2 = 131075,
      IppECCBStd163r1 = 131076,
      IppECCBStd163r2 = 131077,
      IppECCBStd193r1 = 131078,
      IppECCBStd193r2 = 131079,
      IppECCBStd233r1 = 131080,
      IppECCBStd283r1 = 131081,
      IppECCBStd409r1 = 131082,
      IppECCBStd571r1 = 131083,
      IppECCKStd = 262144,
      IppECCBStd163k1 = 262144,
      IppECCBStd233k1 = 262145,
      IppECCBStd239k1 = 262146,
      IppECCBStd283k1 = 262147,
      IppECCBStd409k1 = 262148,
      IppECCBStd571k1 = 262149,
   };
   public enum IppsBigNumSGN {
      IppsBigNumNEG = 0,
      IppsBigNumPOS = 1,
   };
   public enum IppDLPKeyTag {
      IppDLPkeyP = 1,
      IppDLPkeyR = 2,
      IppDLPkeyG = 4,
   };
   public enum IppDLResult {
      ippDLValid = 0,
      ippDLBaseIsEven = 1,
      ippDLOrderIsEven = 2,
      ippDLInvalidBaseRange = 3,
      ippDLInvalidOrderRange = 4,
      ippDLCompositeBase = 5,
      ippDLCompositeOrder = 6,
      ippDLInvalidCofactor = 7,
      ippDLInvalidGenerator = 8,
      ippDLInvalidPrivateKey = 9,
      ippDLInvalidPublicKey = 10,
      ippDLInvalidKeyPair = 11,
      ippDLInvalidSignature = 12,
   };
   public enum IppRSAKeyTag {
      IppRSAkeyN = 1,
      IppRSAkeyE = 2,
      IppRSAkeyD = 4,
      IppRSAkeyP = 8,
      IppRSAkeyQ = 16,
      IppRSAkeyDp = 32,
      IppRSAkeyDq = 64,
      IppRSAkeyQinv = 128,
   };
//
// hidden or own structures
//
   [StructLayout(LayoutKind.Sequential)] public struct IppsARCFive128Spec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsARCFive64Spec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsARCFourState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsBigNumState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsBlowfishSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsCMACRijndael128State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAABlowfishState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAADESState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAARijndael128State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAARijndael192State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAARijndael256State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAATDESState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDAATwofishState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDESSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsDLPState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsECCBPointState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsECCBState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsECCPPointState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsECCPState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPECPoint {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPECState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPElement {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXECPoint {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXECState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXElement {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXQElement {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXQState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsGFPXState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACMD5State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACSHA1State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACSHA224State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACSHA256State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACSHA384State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsHMACSHA512State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsMD5State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsMontState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPRNGState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsPrimeState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRSAState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRijndael128CCMState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRijndael128GCMState {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRijndael128Spec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRijndael192Spec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsRijndael256Spec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSHA1State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSHA224State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSHA256State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSHA384State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsSHA512State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsTatePairingDE3State {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsTwofishSpec {};
   [StructLayout(LayoutKind.Sequential)] public struct IppsXCBCRijndael128State {};

unsafe public class cp {

   internal const string libname = "ippcp-7.1.dll";


public delegate IppStatus IppMGF(byte* pSeed, int seedLen, byte* pMask, int maskLen);
public delegate IppStatus IppHASH(byte* pMsg, int len, byte* pMD);
public delegate IppStatus IppBitSupplier(uint* pRand, int nBits, void* pEbsParams);
[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname,EntryPoint="ippcpGetLibVersion")] public static extern
int* xippcpGetLibVersion (    );
public static IppLibraryVersion ippcpGetLibVersion() {
   return new IppLibraryVersion( xippcpGetLibVersion() );
}

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128DecryptCBC (  byte *pSrc, byte *pDst, int length, IppsARCFive128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128DecryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128DecryptCTR (  byte *pSrc, byte *pDst, int len, IppsARCFive128Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128DecryptECB (  byte *pSrc, byte *pDst, int length, IppsARCFive128Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128DecryptOFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive128Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128EncryptCBC (  byte *pSrc, byte *pDst, int length, IppsARCFive128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128EncryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128EncryptCTR (  byte *pSrc, byte *pDst, int len, IppsARCFive128Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128EncryptECB (  byte *pSrc, byte *pDst, int length, IppsARCFive128Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128EncryptOFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive128Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128GetSize (  int rounds, int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128Init (  byte *pKey, int keylen, int rounds, IppsARCFive128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128Pack (  IppsARCFive128Spec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive128Unpack (  byte *pBuffer, IppsARCFive128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64DecryptCBC (  byte *pSrc, byte *pDst, int length, IppsARCFive64Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64DecryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive64Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64DecryptCTR (  byte *pSrc, byte *pDst, int len, IppsARCFive64Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64DecryptECB (  byte *pSrc, byte *pDst, int length, IppsARCFive64Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64DecryptOFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive64Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64EncryptCBC (  byte *pSrc, byte *pDst, int length, IppsARCFive64Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64EncryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive64Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64EncryptCTR (  byte *pSrc, byte *pDst, int len, IppsARCFive64Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64EncryptECB (  byte *pSrc, byte *pDst, int length, IppsARCFive64Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64EncryptOFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsARCFive64Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64GetSize (  int rounds, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64Init (  byte *pKey, int keylen, int rounds, IppsARCFive64Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64Pack (  IppsARCFive64Spec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFive64Unpack (  byte *pBuffer, IppsARCFive64Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourCheckKey (  byte *pKey, int keyLen, IppBool *pIsWeak );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourDecrypt (  byte *pSrc, byte *pDst, int length, IppsARCFourState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourEncrypt (  byte *pSrc, byte *pDst, int length, IppsARCFourState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourInit (  byte *pKey, int keyLen, IppsARCFourState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourPack (  IppsARCFourState *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourReset (  IppsARCFourState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsARCFourUnpack (  byte *pBuffer, IppsARCFourState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsAdd_BN (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsAdd_BNU (  uint *pA, uint *pB, uint *pR, int n, uint *carry );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBigNumGetSize (  int length, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBigNumInit (  int length, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishDecryptCBC (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishDecryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsBlowfishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishDecryptCTR (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishDecryptECB (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishDecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsBlowfishSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishEncryptCBC (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishEncryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsBlowfishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishEncryptCTR (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishEncryptECB (  byte *pSrc, byte *pDst, int len, IppsBlowfishSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishEncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsBlowfishSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishInit (  byte *pKey, int keyLen, IppsBlowfishSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishPack (  IppsBlowfishSpec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsBlowfishUnpack (  byte *pBuffer, IppsBlowfishSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACRijndael128Final (  byte *pMD, int mdLen, IppsCMACRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACRijndael128GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsCMACRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACRijndael128MessageDigest (  byte *pMsg, int msgLen, byte *pKey, IppsRijndaelKeyLength keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACRijndael128Update (  byte *pSrc, int len, IppsCMACRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCMACSafeRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsCMACRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCmpZero_BN (  IppsBigNumState *pBN, uint *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsCmp_BN (  IppsBigNumState *pA, IppsBigNumState *pB, uint *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAABlowfishFinal (  byte *pMD, int mdLen, IppsDAABlowfishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAABlowfishGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAABlowfishInit (  byte *pKey, int keyLen, IppsDAABlowfishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAABlowfishMessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAABlowfishUpdate (  byte *pSrc, int len, IppsDAABlowfishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAADESFinal (  byte *pMD, int mdLen, IppsDAADESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAADESGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAADESInit (  byte *pKey, IppsDAADESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAADESMessageDigest (  byte *pMsg, int msgLen, byte *pKey, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAADESUpdate (  byte *pSrc, int len, IppsDAADESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael128Final (  byte *pMD, int mdLen, IppsDAARijndael128State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael128GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsDAARijndael128State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael128MessageDigest (  byte *pMsg, int msgLen, byte *pKey, IppsRijndaelKeyLength keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael128Update (  byte *pSrc, int len, IppsDAARijndael128State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael192Final (  byte *pMD, int mdLen, IppsDAARijndael192State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael192GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael192Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsDAARijndael192State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael192MessageDigest (  byte *pMsg, int msgLen, byte *pKey, IppsRijndaelKeyLength keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael192Update (  byte *pSrc, int len, IppsDAARijndael192State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael256Final (  byte *pMD, int mdLen, IppsDAARijndael256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael256GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael256Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsDAARijndael256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael256MessageDigest (  byte *pMsg, int msgLen, byte *pKey, IppsRijndaelKeyLength keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAARijndael256Update (  byte *pSrc, int len, IppsDAARijndael256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAASafeRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsDAARijndael128State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATDESFinal (  byte *pMD, int mdLen, IppsDAATDESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATDESGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATDESInit (  byte *pKey1, byte *pKey2, byte *pKey3, IppsDAATDESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATDESMessageDigest (  byte *pMsg, int msgLen, byte *pKey1, byte *pKey2, byte *pKey3, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATDESUpdate (  byte *pSrc, int len, IppsDAATDESState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATwofishFinal (  byte *pMD, int mdLen, IppsDAATwofishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATwofishGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATwofishInit (  byte *pKey, int keyLen, IppsDAATwofishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATwofishMessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDAATwofishUpdate (  byte *pSrc, int len, IppsDAATwofishState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESDecryptCBC (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESDecryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsDESSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESDecryptCTR (  byte *pSrc, byte *pDst, int len, IppsDESSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESDecryptECB (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESDecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsDESSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESEncryptCBC (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESEncryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsDESSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESEncryptCTR (  byte *pSrc, byte *pDst, int len, IppsDESSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESEncryptECB (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESEncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsDESSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESGetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESInit (  byte *pKey, IppsDESSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESPack (  IppsDESSpec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDESUnpack (  byte *pBuffer, IppsDESSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
string ippsDLGetResultString (  IppDLResult code );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGenKeyPair (  IppsBigNumState *pPrvKey, IppsBigNumState *pPubKey, IppsDLPState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGenerateDH (  IppsBigNumState *pSeedIn, int nTrials, IppsDLPState *pCtx, IppsBigNumState *pSeedOut, int *pCounter, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGenerateDSA (  IppsBigNumState *pSeedIn, int nTrials, IppsDLPState *pCtx, IppsBigNumState *pSeedOut, int *pCounter, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGet (  IppsBigNumState *pP, IppsBigNumState *pR, IppsBigNumState *pG, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGetDP (  IppsBigNumState *pDP, IppDLPKeyTag tag, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPGetSize (  int bitSizeP, int bitSizeR, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPInit (  int bitSizeP, int bitSizeR, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPPack (  IppsDLPState *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPPublicKey (  IppsBigNumState *pPrvKey, IppsBigNumState *pPubKey, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPSet (  IppsBigNumState *pP, IppsBigNumState *pR, IppsBigNumState *pG, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPSetDP (  IppsBigNumState *pDP, IppDLPKeyTag tag, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPSetKeyPair (  IppsBigNumState *pPrvKey, IppsBigNumState *pPubKey, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPSharedSecretDH (  IppsBigNumState *pPrvKeyA, IppsBigNumState *pPubKeyB, IppsBigNumState *pShare, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPSignDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pPrvKey, IppsBigNumState *pSignR, IppsBigNumState *pSignS, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPUnpack (  byte *pBuffer, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPValidateDH (  int nTrials, IppDLResult *pResult, IppsDLPState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPValidateDSA (  int nTrials, IppDLResult *pResult, IppsDLPState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPValidateKeyPair (  IppsBigNumState *pPrvKey, IppsBigNumState *pPubKey, IppDLResult *pResult, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDLPVerifyDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pSignR, IppsBigNumState *pSignS, IppDLResult *pResult, IppsDLPState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDiv_64u32u (  ulong a, uint b, uint *pQ, uint *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsDiv_BN (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pQ, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBAddPoint (  IppsECCBPointState *pP, IppsECCBPointState *pQ, IppsECCBPointState *pR, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBCheckPoint (  IppsECCBPointState *pP, IppECResult *pResult, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBComparePoint (  IppsECCBPointState *pP, IppsECCBPointState *pQ, IppECResult *pResult, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBGenKeyPair (  IppsBigNumState *pPrivate, IppsECCBPointState *pPublic, IppsECCBState *pECC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBGet (  IppsBigNumState *pPrime, IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pGX, IppsBigNumState *pGY, IppsBigNumState *pOrder, int *cofactor, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBGetOrderBitSize (  int *pBitSize, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBGetPoint (  IppsBigNumState *pX, IppsBigNumState *pY, IppsECCBPointState *pPoint, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBGetSize (  int feBitSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBInit (  int feBitSize, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBMulPointScalar (  IppsECCBPointState *pP, IppsBigNumState *pK, IppsECCBPointState *pR, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBNegativePoint (  IppsECCBPointState *pP, IppsECCBPointState *pR, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBPointGetSize (  int feBitSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBPointInit (  int feBitSize, IppsECCBPointState *pPoint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBPublicKey (  IppsBigNumState *pPrivate, IppsECCBPointState *pPublic, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSet (  IppsBigNumState *pPrime, IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pGX, IppsBigNumState *pGY, IppsBigNumState *pOrder, int cofactor, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSetKeyPair (  IppsBigNumState *pPrivate, IppsECCBPointState *pPublic, IppBool regular, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSetPoint (  IppsBigNumState *pX, IppsBigNumState *pY, IppsECCBPointState *pPoint, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSetPointAtInfinity (  IppsECCBPointState *pPoint, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSetStd (  IppECCType flag, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSharedSecretDH (  IppsBigNumState *pPrivateA, IppsECCBPointState *pPublicB, IppsBigNumState *pShare, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSharedSecretDHC (  IppsBigNumState *pPrivateA, IppsECCBPointState *pPublicB, IppsBigNumState *pShare, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSignDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pPrivate, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBSignNR (  IppsBigNumState *pMsgDigest, IppsBigNumState *pPrivate, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBValidate (  int nTrials, IppECResult *pResult, IppsECCBState *pECC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBValidateKeyPair (  IppsBigNumState *pPrivate, IppsECCBPointState *pPublic, IppECResult *pResult, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBVerifyDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppECResult *pResult, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCBVerifyNR (  IppsBigNumState *pMsgDigest, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppECResult *pResult, IppsECCBState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
string ippsECCGetResultString (  IppECResult code );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPAddPoint (  IppsECCPPointState *pP, IppsECCPPointState *pQ, IppsECCPPointState *pR, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPCheckPoint (  IppsECCPPointState *pP, IppECResult *pResult, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPComparePoint (  IppsECCPPointState *pP, IppsECCPPointState *pQ, IppECResult *pResult, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPGenKeyPair (  IppsBigNumState *pPrivate, IppsECCPPointState *pPublic, IppsECCPState *pECC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPGet (  IppsBigNumState *pPrime, IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pGX, IppsBigNumState *pGY, IppsBigNumState *pOrder, int *cofactor, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPGetOrderBitSize (  int *pBitSize, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPGetPoint (  IppsBigNumState *pX, IppsBigNumState *pY, IppsECCPPointState *pPoint, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPGetSize (  int feBitSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPInit (  int feBitSize, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPMulPointScalar (  IppsECCPPointState *pP, IppsBigNumState *pK, IppsECCPPointState *pR, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPNegativePoint (  IppsECCPPointState *pP, IppsECCPPointState *pR, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPPointGetSize (  int feBitSize, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPPointInit (  int feBitSize, IppsECCPPointState *pPoint );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPPublicKey (  IppsBigNumState *pPrivate, IppsECCPPointState *pPublic, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSet (  IppsBigNumState *pPrime, IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pGX, IppsBigNumState *pGY, IppsBigNumState *pOrder, int cofactor, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSetKeyPair (  IppsBigNumState *pPrivate, IppsECCPPointState *pPublic, IppBool regular, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSetPoint (  IppsBigNumState *pX, IppsBigNumState *pY, IppsECCPPointState *pPoint, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSetPointAtInfinity (  IppsECCPPointState *pPoint, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSetStd (  IppECCType flag, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSharedSecretDH (  IppsBigNumState *pPrivateA, IppsECCPPointState *pPublicB, IppsBigNumState *pShare, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSharedSecretDHC (  IppsBigNumState *pPrivateA, IppsECCPPointState *pPublicB, IppsBigNumState *pShare, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSignDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pPrivate, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPSignNR (  IppsBigNumState *pMsgDigest, IppsBigNumState *pPrivate, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPValidate (  int nTrials, IppECResult *pResult, IppsECCPState *pECC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPValidateKeyPair (  IppsBigNumState *pPrivate, IppsECCPPointState *pPublic, IppECResult *pResult, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPVerifyDSA (  IppsBigNumState *pMsgDigest, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppECResult *pResult, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsECCPVerifyNR (  IppsBigNumState *pMsgDigest, IppsBigNumState *pSignX, IppsBigNumState *pSignY, IppECResult *pResult, IppsECCPState *pECC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsExtGet_BN (  IppsBigNumSGN *pSgn, int *pBitSize, uint *pData, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPAdd (  IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPCmpElement (  IppsGFPElement *pA, IppsGFPElement *pB, IppsElementCmpResult *pResult, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPCpyElement (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECAddPoint (  IppsGFPECPoint *pP, IppsGFPECPoint *pQ, IppsGFPECPoint *pR, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECCmpPoint (  IppsGFPECPoint *pP, IppsGFPECPoint *pQ, IppsElementCmpResult *pResult, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECCpyPoint (  IppsGFPECPoint *pA, IppsGFPECPoint *pR, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECGet (  IppsGFPECState *pEC, IppsGFPState **ppGF, uint *pElementLen, IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pX, IppsGFPElement *pY, uint **ppOrder, uint *pOrderLen, uint *pCofactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECGetPoint (  IppsGFPECPoint *pPoint, IppsGFPElement *pX, IppsGFPElement *pY, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECGetSize (  IppsGFPState *pGF, uint *pSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECInit (  IppsGFPECState *pEC, IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pX, IppsGFPElement *pY, uint *pOrder, uint opderLen, uint cofactor, IppsGFPState *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECMulPointScalar (  IppsGFPECPoint *pP, uint *pN, uint nLen, IppsGFPECPoint *pR, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECNegPoint (  IppsGFPECPoint *pP, IppsGFPECPoint *pR, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECPointGetSize (  IppsGFPECState *pEC, uint *pSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECPointInit (  IppsGFPECPoint *pPoint, IppsGFPElement *pX, IppsGFPElement *pY, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECSet (  IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pX, IppsGFPElement *pY, uint *pOrder, uint orderLen, uint cofactor, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECSetPoint (  IppsGFPElement *pX, IppsGFPElement *pY, IppsGFPECPoint *pPoint, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECSetPointAtInfinity (  IppsGFPECPoint *pPoint, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECSetPointRandom (  IppsGFPECPoint *pPoint, IppsGFPECState *pEC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECVerify (  IppsGFPECState *pEC, IppECResult *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPECVerifyPoint (  IppsGFPECPoint *pP, IppECResult *pResult, IppsGFPECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPElementGetSize (  IppsGFPState *pGFpCtx, uint *pElementSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPElementInit (  IppsGFPElement *pR, uint *pA, uint lenA, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPExp (  IppsGFPElement *pA, IppsGFPElement *pE, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPGet (  IppsGFPState *pGFpCtx, uint **ppModulus, uint *pElementLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPGetElement (  IppsGFPElement *pA, uint *pDataA, uint dataLen, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPGetSize (  uint bitSize, uint *pStateSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPInit (  IppsGFPState *pGFpCtx, uint *pPime, uint bitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPInv (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPMontDecode (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPMontEncode (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPMul (  IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPNeg (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSetElement (  uint *pA, uint lenA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSetElementPower2 (  uint power, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSetElementRandom (  IppsGFPElement *pR, IppsGFPState *pGFpCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSetElementZero (  IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSqrt (  IppsGFPElement *pA, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPSub (  IppsGFPElement *pA, IppsGFPElement *pB, IppsGFPElement *pR, IppsGFPState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXAdd (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXAdd_GFP (  IppsGFPXElement *pA, IppsGFPElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXCmpElement (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsElementCmpResult *pResult, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXCpyElement (  IppsGFPXElement *pA, IppsGFPXElement *pR, IppsGFPXState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXDiv (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pQ, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECAddPoint (  IppsGFPXECPoint *pP, IppsGFPXECPoint *pQ, IppsGFPXECPoint *pR, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECCmpPoint (  IppsGFPXECPoint *pP, IppsGFPXECPoint *pQ, IppsElementCmpResult *pResult, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECCpyPoint (  IppsGFPXECPoint *pA, IppsGFPXECPoint *pR, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECGet (  IppsGFPXECState *pEC, IppsGFPXState **ppGF, uint *pElementLen, IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pX, IppsGFPXElement *pY, uint **ppOrder, uint *pOrderLen, uint *pCofactor );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECGetPoint (  IppsGFPXECPoint *pPoint, IppsGFPXElement *pX, IppsGFPXElement *pY, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECGetSize (  IppsGFPXState *pGF, uint *pSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECInit (  IppsGFPXECState *pEC, IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pX, IppsGFPXElement *pY, uint *pOrder, uint opderLen, uint cofactor, IppsGFPXState *pGF );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECMulPointScalar (  IppsGFPXECPoint *pP, uint *pN, uint nLen, IppsGFPXECPoint *pR, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECNegPoint (  IppsGFPXECPoint *pP, IppsGFPXECPoint *pR, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECPointGetSize (  IppsGFPXECState *pEC, uint *pSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECPointInit (  IppsGFPXECPoint *pPoint, IppsGFPXElement *pX, IppsGFPXElement *pY, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECSet (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pX, IppsGFPXElement *pY, uint *pOrder, uint orderLen, uint cofactor, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECSetPoint (  IppsGFPXElement *pX, IppsGFPXElement *pY, IppsGFPXECPoint *pPoint, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECSetPointAtInfinity (  IppsGFPXECPoint *pPoint, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECSetPointRandom (  IppsGFPXECPoint *pPoint, IppsGFPXECState *pEC, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECVerify (  IppsGFPXECState *pEC, IppECResult *pResult );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXECVerifyPoint (  IppsGFPXECPoint *pP, IppECResult *pResult, IppsGFPXECState *pEC );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXElementGetSize (  IppsGFPXState *pGFPExtCtx, uint *pElementSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXElementInit (  IppsGFPXElement *pR, uint *pA, uint lenA, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXExp (  IppsGFPXElement *pA, uint *pB, uint bSize, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXGet (  IppsGFPXState *pGFPExtCtx, IppsGFPState **ppGroundField, uint *pModulus, uint *pModulusDegree, uint *pModulusLen, uint *pElementLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXGetElement (  IppsGFPXElement *pA, uint *pDataA, uint dataLen, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXGetSize (  IppsGFPState *pGroundGF, uint degree, uint *pStateSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXInit (  IppsGFPXState *pGFPExtCtx, IppsGFPState *pGroundGF, uint degree, uint *pModulus );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXInv (  IppsGFPXElement *pA, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXMul (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXMul_GFP (  IppsGFPXElement *pA, IppsGFPElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXNeg (  IppsGFPXElement *pA, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQAdd (  IppsGFPXQElement *pA, IppsGFPXQElement *pB, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQCmpElement (  IppsGFPXQElement *pA, IppsGFPXQElement *pB, IppsElementCmpResult *pResult, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQCpyElement (  IppsGFPXQElement *pA, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQElementGetSize (  IppsGFPXQState *pGFPQExtCtx, uint *pElementSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQElementInit (  IppsGFPXQElement *pR, uint *pA, uint lenA, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQExp (  IppsGFPXQElement *pA, uint *pB, uint bSize, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQGet (  IppsGFPXQState *pGFPQExtCtx, IppsGFPXState **ppGroundField, uint *pModulus, uint *pModulusDegree, uint *pModulusLen, uint *pElementLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQGetElement (  IppsGFPXQElement *pA, uint *pDataA, uint dataLen, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQGetSize (  IppsGFPXState *pGFPExtCtx, uint *pStateSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQInit (  IppsGFPXQState *pGFPQExtCtx, IppsGFPXState *pGroundField, uint *pModulus );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQInv (  IppsGFPXQElement *pA, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQMul (  IppsGFPXQElement *pA, IppsGFPXQElement *pB, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQMul_GFP (  IppsGFPXQElement *pA, IppsGFPElement *pB, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQNeg (  IppsGFPXQElement *pA, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQSetElement (  uint *pA, uint lenA, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQSetElementPowerX (  uint power, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQSetElementRandom (  IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQSetElementZero (  IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXQSub (  IppsGFPXQElement *pA, IppsGFPXQElement *pB, IppsGFPXQElement *pR, IppsGFPXQState *pGFPQExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSetElement (  uint *pA, uint lenA, IppsGFPXElement *pR, IppsGFPXState *pGFpCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSetElementPowerX (  uint power, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSetElementRandom (  IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSetElementZero (  IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSub (  IppsGFPXElement *pA, IppsGFPXElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGFPXSub_GFP (  IppsGFPXElement *pA, IppsGFPElement *pB, IppsGFPXElement *pR, IppsGFPXState *pGFPExtCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGcd_BN (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pGCD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGetOctString_BN (  byte *pStr, int strLen, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGetOctString_BNU (  uint *pBNU, int bnuSize, byte *pStr, int strLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGetSize_BN (  IppsBigNumState *pBN, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsGet_BN (  IppsBigNumSGN *pSgn, int *pLength, uint *pData, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Duplicate (  IppsHMACMD5State *pSrcCtx, IppsHMACMD5State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Final (  byte *pMD, int mdLen, IppsHMACMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5GetTag (  byte *pTag, int tagLen, IppsHMACMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Init (  byte *pKey, int keyLen, IppsHMACMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Pack (  IppsHMACMD5State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Unpack (  byte *pBuffer, IppsHMACMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACMD5Update (  byte *pSrc, int len, IppsHMACMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Duplicate (  IppsHMACSHA1State *pSrcCtx, IppsHMACSHA1State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Final (  byte *pMD, int mdLen, IppsHMACSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1GetTag (  byte *pTag, int tagLen, IppsHMACSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Init (  byte *pKey, int keyLen, IppsHMACSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Pack (  IppsHMACSHA1State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Unpack (  byte *pBuffer, IppsHMACSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA1Update (  byte *pSrc, int len, IppsHMACSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Duplicate (  IppsHMACSHA224State *pSrcCtx, IppsHMACSHA224State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Final (  byte *pMD, int mdLen, IppsHMACSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224GetTag (  byte *pTag, int tagLen, IppsHMACSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Init (  byte *pKey, int keyLen, IppsHMACSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Pack (  IppsHMACSHA224State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Unpack (  byte *pBuffer, IppsHMACSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA224Update (  byte *pSrc, int len, IppsHMACSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Duplicate (  IppsHMACSHA256State *pSrcCtx, IppsHMACSHA256State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Final (  byte *pMD, int mdLen, IppsHMACSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256GetTag (  byte *pTag, int tagLen, IppsHMACSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Init (  byte *pKey, int keyLen, IppsHMACSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Pack (  IppsHMACSHA256State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Unpack (  byte *pBuffer, IppsHMACSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA256Update (  byte *pSrc, int len, IppsHMACSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Duplicate (  IppsHMACSHA384State *pSrcCtx, IppsHMACSHA384State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Final (  byte *pMD, int mdLen, IppsHMACSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384GetTag (  byte *pTag, int tagLen, IppsHMACSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Init (  byte *pKey, int keyLen, IppsHMACSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Pack (  IppsHMACSHA384State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Unpack (  byte *pBuffer, IppsHMACSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA384Update (  byte *pSrc, int len, IppsHMACSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Duplicate (  IppsHMACSHA512State *pSrcCtx, IppsHMACSHA512State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Final (  byte *pMD, int mdLen, IppsHMACSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512GetSize (  int *size );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512GetTag (  byte *pTag, int tagLen, IppsHMACSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Init (  byte *pKey, int keyLen, IppsHMACSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512MessageDigest (  byte *pMsg, int msgLen, byte *pKey, int keyLen, byte *pMD, int mdLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Pack (  IppsHMACSHA512State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Unpack (  byte *pBuffer, IppsHMACSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsHMACSHA512Update (  byte *pSrc, int len, IppsHMACSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMACOne_BNU_I (  uint *pA, uint *pR, int n, uint w, uint *carry );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMAC_BN_I (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Duplicate (  IppsMD5State *pSrcCtx, IppsMD5State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Final (  byte *pMD, IppsMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5GetTag (  byte *pTag, uint tagLen, IppsMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Init (  IppsMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Pack (  IppsMD5State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Unpack (  byte *pBuffer, IppsMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMD5Update (  byte *pSrc, int len, IppsMD5State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_MD5 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_SHA1 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_SHA224 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_SHA256 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_SHA384 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMGF_SHA512 (  byte *pSeed, int seedLen, byte *pMask, int maskLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsModInv_BN (  IppsBigNumState *pA, IppsBigNumState *pM, IppsBigNumState *pInv );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMod_BN (  IppsBigNumState *pA, IppsBigNumState *pM, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontExp (  IppsBigNumState *pA, IppsBigNumState *pE, IppsMontState *m, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontForm (  IppsBigNumState *pA, IppsMontState *pCtx, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontGet (  uint *pModulo, int *pSize, IppsMontState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontGetSize (  IppsExpMethod method, int length, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontInit (  IppsExpMethod method, int length, IppsMontState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontMul (  IppsBigNumState *pA, IppsBigNumState *pB, IppsMontState *m, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMontSet (  uint *pModulo, int size, IppsMontState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMulOne_BNU (  uint *pA, uint *pR, int n, uint w, uint *carry );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMul_BN (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMul_BNU4 (  uint *pA, uint *pB, uint *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsMul_BNU8 (  uint *pA, uint *pB, uint *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGGetSeed (  IppsPRNGState *pCtx, IppsBigNumState *pSeed );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGInit (  int seedBits, IppsPRNGState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGSetAugment (  IppsBigNumState *pAug, IppsPRNGState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGSetH0 (  IppsBigNumState *pH0, IppsPRNGState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGSetModulus (  IppsBigNumState *pMod, IppsPRNGState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGSetSeed (  IppsBigNumState *pSeed, IppsPRNGState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGen (  uint *pRand, int nBits, char *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPRNGen_BN (  IppsBigNumState *pRand, int nBits, char *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeGen (  int nBits, int nTrials, IppsPrimeState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeGet (  uint *pPrime, int *pLen, IppsPrimeState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeGetSize (  int nMaxBits, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeGet_BN (  IppsBigNumState *pPrime, IppsPrimeState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeInit (  int nMaxBits, IppsPrimeState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeSet (  uint *pPrime, int nBits, IppsPrimeState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeSet_BN (  IppsBigNumState *pPrime, IppsPrimeState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsPrimeTest (  int nTrials, uint *pResult, IppsPrimeState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSADecrypt (  IppsBigNumState *pX, IppsBigNumState *pY, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSADecrypt_PKCSv15 (  byte *pSrc, byte *pDst, uint *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAEncrypt (  IppsBigNumState *pX, IppsBigNumState *pY, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAEncrypt_PKCSv15 (  byte *pSrc, uint srcLen, byte *pRandPS, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAGenerate (  IppsBigNumState *pE, int kn, int kp, int nTrials, IppsRSAState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAGetKey (  IppsBigNumState *pKey, IppRSAKeyTag tag, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAGetSize (  int kn, int kp, IppRSAKeyType flag, int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAInit (  int kn, int kp, IppRSAKeyType flag, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA, IppHASH hashFun, int hashLen, IppMGF mgfFunc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_MD5 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_SHA1 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_SHA224 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_SHA256 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_SHA384 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPDecrypt_SHA512 (  byte *pSrc, byte *pLabel, int labLen, byte *pDst, int *pDstLen, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA, IppHASH hashFun, int hashLen, IppMGF mgfFunc );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_MD5 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_SHA1 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_SHA224 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_SHA256 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_SHA384 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAOAEPEncrypt_SHA512 (  byte *pSrc, int srcLen, byte *pLabel, int labLen, byte *pSeed, byte *pDst, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAPack (  IppsRSAState *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign (  byte *pHMsg, int hashLen, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA, IppHASH hashFun, IppMGF mgfFun );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_MD5 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_MD5_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA1 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA1_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA224 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA224_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA256 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA256_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA384 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA384_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA512 (  byte *pHMsg, byte *pSalt, int saltLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSASign_SHA512_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify (  byte *pHMsg, int hashLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA, IppHASH hashFun, IppMGF mgfFun );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_MD5 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_MD5_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA1 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA1_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA224 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA224_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA256 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA256_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA384 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA384_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA512 (  byte *pHMsg, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASSAVerify_SHA512_PKCSv15 (  byte *pMsg, uint msgLen, byte *pSign, IppBool *pIsValid, IppsRSAState *pRSA );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSASetKey (  IppsBigNumState *pKey, IppRSAKeyTag tag, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAUnpack (  byte *pBuffer, IppsRSAState *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRSAValidate (  IppsBigNumState *pE, int nTrials, uint *pResult, IppsRSAState *pCtx, IppBitSupplier rndFunc, char *pRndParam );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRef_BN (  IppsBigNumSGN *sgn, int *bitSize, uint **ppData, IppsBigNumState *x );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMDecrypt (  byte *pSrc, byte *pDst, uint len, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMDecryptMessage (  byte *pKey, IppsRijndaelKeyLength keyLen, byte *pIV, uint ivLen, byte *pAAD, uint aadLen, byte *pSrc, byte *pDst, uint len, byte *pTag, uint tagLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMEncrypt (  byte *pSrc, byte *pDst, uint len, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMEncryptMessage (  byte *pKey, IppsRijndaelKeyLength keyLen, byte *pIV, uint ivLen, byte *pAAD, uint aadLen, byte *pSrc, byte *pDst, uint len, byte *pTag, uint tagLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMGetSize (  uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMGetTag (  byte *pTag, uint tagLen, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMInit (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMMessageLen (  ulong msgLen, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMStart (  byte *pIV, uint ivLen, byte *pAAD, uint aadLen, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128CCMTagLen (  uint tagLen, IppsRijndael128CCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptCCM (  byte *pNonce, uint lenN, byte *pHeader, ulong lenH, byte *pCtext, ulong lenC, int macLen, byte *pPtext, IppBool *pResult, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptCCM_u8 (  byte *pNonce, int lenN, byte *pHeader, int lenH, byte *pCtext, int lenC, int macLen, byte *pPtext, IppBool *pResult, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128DecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael128Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptCCM (  byte *pNonce, uint lenN, byte *pHeader, ulong lenH, byte *pPtext, ulong lenP, int macLen, byte *pCtext, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptCCM_u8 (  byte *pNonce, int lenN, byte *pHeader, int lenH, byte *pPtext, int lenP, int macLen, byte *pCtext, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael128Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael128Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128EncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael128Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMDecrypt (  byte *pSrc, byte *pDst, uint len, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMDecryptMessage (  byte *pKey, IppsRijndaelKeyLength keyLen, byte *pIV, uint ivLen, byte *pAAD, uint aadLen, byte *pSrc, byte *pDst, uint len, byte *pDstTag, uint tagLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMEncrypt (  byte *pSrc, byte *pDst, uint len, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMEncryptMessage (  byte *pKey, IppsRijndaelKeyLength keyLen, byte *pIV, uint ivLen, byte *pAAD, uint aadLen, byte *pSrc, byte *pDst, uint len, byte *pTag, uint tagLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMGetSize (  uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMGetSizeManaged (  IppAESGCMbehaviour flag, uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMGetTag (  byte *pDstTag, uint tagLen, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMInit (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMInitManaged (  IppAESGCMbehaviour flag, byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMProcessAAD (  byte *pAAD, uint ivAAD, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMProcessIV (  byte *pIV, uint ivLen, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMReset (  IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GCMStart (  byte *pIV, uint ivLen, byte *pAAD, uint aadLen, IppsRijndael128GCMState *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128Pack (  IppsRijndael128Spec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael128Unpack (  byte *pBuffer, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192DecryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192DecryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael192Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192DecryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192DecryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192DecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael192Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192EncryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192EncryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael192Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192EncryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192EncryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael192Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192EncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael192Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael192Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192Pack (  IppsRijndael192Spec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael192Unpack (  byte *pBuffer, IppsRijndael192Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256DecryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256DecryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael256Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256DecryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256DecryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256DecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael256Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256EncryptCBC (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256EncryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsRijndael256Spec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256EncryptCTR (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256EncryptECB (  byte *pSrc, byte *pDst, int len, IppsRijndael256Spec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256EncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsRijndael256Spec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael256Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256Pack (  IppsRijndael256Spec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsRijndael256Unpack (  byte *pBuffer, IppsRijndael256Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Duplicate (  IppsSHA1State *pSrcCtx, IppsSHA1State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Final (  byte *pMD, IppsSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1GetTag (  byte *pTag, uint tagLen, IppsSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Init (  IppsSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Pack (  IppsSHA1State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Unpack (  byte *pBuffer, IppsSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA1Update (  byte *pSrc, int len, IppsSHA1State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Duplicate (  IppsSHA224State *pSrcCtx, IppsSHA224State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Final (  byte *pMD, IppsSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224GetTag (  byte *pTag, uint tagLen, IppsSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Init (  IppsSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Pack (  IppsSHA224State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Unpack (  byte *pBuffer, IppsSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA224Update (  byte *pSrc, int len, IppsSHA224State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Duplicate (  IppsSHA256State *pSrcCtx, IppsSHA256State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Final (  byte *pMD, IppsSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256GetTag (  byte *pTag, uint tagLen, IppsSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Init (  IppsSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Pack (  IppsSHA256State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Unpack (  byte *pBuffer, IppsSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA256Update (  byte *pSrc, int len, IppsSHA256State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Duplicate (  IppsSHA384State *pSrcCtx, IppsSHA384State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Final (  byte *pMD, IppsSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384GetTag (  byte *pTag, uint tagLen, IppsSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Init (  IppsSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Pack (  IppsSHA384State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Unpack (  byte *pBuffer, IppsSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA384Update (  byte *pSrc, int len, IppsSHA384State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Duplicate (  IppsSHA512State *pSrcCtx, IppsSHA512State *pDstCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Final (  byte *pMD, IppsSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512GetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512GetTag (  byte *pTag, uint tagLen, IppsSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Init (  IppsSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512MessageDigest (  byte *pMsg, int len, byte *pMD );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Pack (  IppsSHA512State *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Unpack (  byte *pBuffer, IppsSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSHA512Update (  byte *pSrc, int len, IppsSHA512State *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSafeRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsRijndael128Spec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSetOctString_BN (  byte *pStr, int strLen, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSetOctString_BNU (  byte *pStr, int strLen, uint *pBNU, int *pBNUsize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSet_BN (  IppsBigNumSGN sgn, int length, uint *pData, IppsBigNumState *pBN );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSqr_32u64u (  uint *pSrc, int n, ulong *pDst );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSqr_BNU4 (  uint *pA, uint *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSqr_BNU8 (  uint *pA, uint *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSub_BN (  IppsBigNumState *pA, IppsBigNumState *pB, IppsBigNumState *pR );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsSub_BNU (  uint *pA, uint *pB, uint *pR, int n, uint *carry );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESDecryptCBC (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESDecryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESDecryptCTR (  byte *pSrc, byte *pDst, int len, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESDecryptECB (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESDecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESEncryptCBC (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESEncryptCFB (  byte *pSrc, byte *pDst, int length, int cfbBlkSize, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESEncryptCTR (  byte *pSrc, byte *pDst, int len, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESEncryptECB (  byte *pSrc, byte *pDst, int length, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTDESEncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsDESSpec *pCtx1, IppsDESSpec *pCtx2, IppsDESSpec *pCtx3, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTatePairingDE3Apply (  IppsGFPECPoint *pPointG1, IppsGFPXECPoint *pPointG2, IppsGFPXQElement *pElementGT, IppsTatePairingDE3State *pPairingCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTatePairingDE3Get (  IppsTatePairingDE3State *pPairingCtx, IppsGFPECState **ppG1, IppsGFPXECState **ppG2, IppsGFPXQState **ppGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTatePairingDE3GetSize (  IppsGFPECState *pG1, IppsGFPXECState *pG2, uint *pSizeInBytes );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTatePairingDE3Init (  IppsTatePairingDE3State *pPairingCtx, IppsGFPECState *pG1, IppsGFPXECState *pG2, IppsGFPXQState *pGT );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishDecryptCBC (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishDecryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsTwofishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishDecryptCTR (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishDecryptECB (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishDecryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsTwofishSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishEncryptCBC (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishEncryptCFB (  byte *pSrc, byte *pDst, int len, int cfbBlkSize, IppsTwofishSpec *pCtx, byte *pIV, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishEncryptCTR (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, byte *pCtrValue, int ctrNumBitSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishEncryptECB (  byte *pSrc, byte *pDst, int len, IppsTwofishSpec *pCtx, IppsCPPadding padding );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishEncryptOFB (  byte *pSrc, byte *pDst, int len, int ofbBlkSize, IppsTwofishSpec *pCtx, byte *pIV );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishGetSize (  int *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishInit (  byte *pKey, int keyLen, IppsTwofishSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishPack (  IppsTwofishSpec *pCtx, byte *pBuffer );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsTwofishUnpack (  byte *pBuffer, IppsTwofishSpec *pCtx );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128Final (  byte *pTag, uint tagLen, IppsXCBCRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128GetSize (  uint *pSize );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128GetTag (  byte *pTag, uint tagLen, IppsXCBCRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128Init (  byte *pKey, IppsRijndaelKeyLength keyLen, IppsXCBCRijndael128State *pState );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128MessageTag (  byte *pMsg, uint msgLen, byte *pKey, IppsRijndaelKeyLength keyLen, byte *pTag, uint tagLen );

[SuppressUnmanagedCodeSecurityAttribute()]
[DllImport(ipp.cp.libname)] public static extern
IppStatus ippsXCBCRijndael128Update (  byte *pSrc, uint len, IppsXCBCRijndael128State *pState );
};
};
