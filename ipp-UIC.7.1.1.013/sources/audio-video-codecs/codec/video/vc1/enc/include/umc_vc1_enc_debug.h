/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, debug functionality
//
*/

#ifndef __UMC_VC1_ENC_DEBUG_H__
#define __UMC_VC1_ENC_DEBUG_H__


#include "vm_types.h"
#include "vm_debug.h"
#include "umc_vc1_enc_def.h"

#include "ippdefs.h"

namespace UMC_VC1_ENCODER
{
#ifdef VC1_ENC_DEBUG_ON

extern const Ipp32u  VC1_ENC_DEBUG;             //current debug output
extern const Ipp32u  VC1_ENC_FRAME_DEBUG;       //on/off frame debug
extern const Ipp32u  VC1_ENC_FRAME_MIN;         //first frame to debug
extern const Ipp32u  VC1_ENC_FRAME_MAX;         //last frame to debug

extern const Ipp32u VC1_ENC_POSITION;          //Frame num, frame type, frame size,
                                               //MB, Block positions, skip info
extern const Ipp32u VC1_ENC_COEFFS;            // DC, AC coefficiens
extern const Ipp32u VC1_ENC_AC_PRED;           // AC prediction
extern const Ipp32u VC1_ENC_QUANT;             //quant info
extern const Ipp32u VC1_ENC_CBP;               // coded block patern info
extern const Ipp32u VC1_ENC_MV;                // motion vectors info
extern const Ipp32u VC1_ENC_PRED;
extern const Ipp32u VC1_ENC_DEBLOCKING;        //deblocking info
extern const Ipp32u VC1_ENC_DEBLK_EDGE;        //deblocking edge
extern const Ipp32u VC1_ENC_FIELD_MV;          //field motion vectors info

typedef struct
{
    Ipp32u pred[3][2][2]; //A/B/C, foeward/backward, X/Y
    //Ipp32s predField[3];

    Ipp8u predFlag[2];
    Ipp8s scaleType[2];
    Ipp8u hybrid[2];
}FieldBlkMVInfo;

typedef struct
{
    Ipp16s BlockDiff[64];
    Ipp8u  intra;
    Ipp32s DC;
    Ipp32s DCDiff;
    Ipp32s DCDirection;
    Ipp16s AC[7];
    Ipp32s ACPred;

//motion vectors info
    Ipp16s MV[2][2];       //X/Y, forward/backward
    Ipp32s MVField[2];    //bSecond
    Ipp16s PredMV[2][2];   //X/Y, forward/backward
    Ipp16s difMV[2][2];    //X/Y, forward/backward
    Ipp16s IntrpMV[2][2];    //X/Y, forward/backward

//run, level paramc
    Ipp8u  Run  [65];
    Ipp16s Level[64];
    Ipp8u  Pairs[4];
    Ipp8u  Mode[65];

//interpolation info
    Ipp8u  FDst[64];   //forward
    Ipp8u  BDst[64];   //backward

    Ipp8u  FSource[11*11];   //forward
    Ipp8u  BSource[11*11];   //forward
    Ipp8u  FSrc;
    Ipp32s BSrc;
    eTransformType VTSType;
    FieldBlkMVInfo FieldMV;

}VC1BlockDebugInfo;

typedef struct
{
    VC1BlockDebugInfo Block[6];
    Ipp32s CBP;
    Ipp32s PredCBP;
    Ipp32s MBType;
    Ipp32s MQuant;
    Ipp8u  HalfQP;
    Ipp8u  skip;

    Ipp32u InHorEdgeLuma;
    Ipp32u InUpHorEdgeLuma;
    Ipp32u InBotHorEdgeLuma;
    Ipp32u ExHorEdgeLuma;

    Ipp32u InVerEdgeLuma;
    Ipp32u InLeftVerEdgeLuma;
    Ipp32u InRightVerEdgeLuma;
    Ipp32u ExVerEdgeLuma;

    Ipp32u InHorEdgeU;
    Ipp32u ExHorEdgeU;

    Ipp32u InVerEdgeU;
    Ipp32u ExVerEdgeU;

    Ipp32u InHorEdgeV;
    Ipp32u ExHorEdgeV;

    Ipp32u InVerEdgeV;
    Ipp32u ExVerEdgeV;

}VC1MBDebugInfo;

class VC1EncDebug
{
public:
    VC1EncDebug();
    ~VC1EncDebug();

    void Init(Ipp32s Width, Ipp32s Height);
    void Close();

    void WriteParams(Ipp32s _cur_frame, Ipp32s level,  vm_char *format,...);
    void WriteFrameInfo();   //wrote debug information
                             //and prepare frame params for next frame

    //Set picture params
    void SetPicType(Ipp32s picType);     //set current picture type
    void SetRefNum(Ipp32u num);
    void SetFrameSize(Ipp32s frameSize); //set currenr frame size
    void NextMB();      //change X, Y MB position
    void SetCurrMBFirst(bool bSecondField = 0);

    //run - level params
    void SetRunLevelCoefs(Ipp8u* run, Ipp16s* level, Ipp8u* pairs, Ipp32s blk_num);
    void SetRLMode(Ipp8u mode, Ipp32s blk_num, Ipp32s coef_num);

    //DC, AC coef information
    void SetDCACInfo(Ipp32s ACPred, Ipp16s* pBlock, Ipp32s BlockStep,
                     Ipp16s* pPredBlock, Ipp32s PredBlockStep,
                     Ipp32s direction, Ipp32s blk_num);

    //MV information
    void SetMVInfo(sCoordinate* MV, Ipp16s predX, Ipp16s predY,Ipp32s forward);
    void SetMVInfo(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward, Ipp32s blk_num);

    void SetMVInfoField(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward);
    void SetMVInfoField(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward, Ipp32s blk_num);

    void SetMVDiff(Ipp16s DiffX, Ipp16s DiffY, Ipp32s forward, Ipp32s blk_num);
    void SetMVDiff(Ipp16s DiffX, Ipp16s DiffY, Ipp32s forward);

    void SetIntrpMV(Ipp16s X, Ipp16s Y, Ipp32s forward, Ipp32s blk_num);
    void SetIntrpMV(Ipp16s X, Ipp16s Y, Ipp32s forward);

    //block difference
    void SetBlockDifference(Ipp16s** pBlock, Ipp32u* step);

    //MB information
    void SetMBAsSkip();
    void SetMBType(Ipp32s type);

    //intra information
    void SetBlockAsIntra(Ipp32s blk_num);

    //CBP
    void SetCPB(Ipp32s predcbp, Ipp32s cbp);

    //Interpolation Info
    void SetInterpInfo(Ipp8u* pBlock, Ipp32s step, Ipp32s blk_num, Ipp32s forward, const Ipp8u* pSrc, Ipp32s srcStep);
    void SetInterpInfo(IppVCInterpolate_8u* YInfo, IppVCInterpolate_8u* UInfo, IppVCInterpolate_8u* VInfo, Ipp32s forward);

    void SetInterpolType(Ipp32s type);
    void SetRounControl(Ipp32s rc);
    void VC1EncDebug::SetACDirection(Ipp32s direction, Ipp32s blk_num);

    void SetQuant(Ipp32s MQuant, Ipp8u  HalfQP);
    void SetHalfCoef(bool half);

    void SetDeblkFlag(bool flag);
    void SetVTSFlag(bool flag);

    void SetDblkHorEdgeLuma(Ipp32u ExHorEdge,     Ipp32u InHorEdge,
                            Ipp32u InUpHorEdge,   Ipp32u InBotHorEdge);
    void SetDblkVerEdgeLuma(Ipp32u ExVerEdge,     Ipp32u InVerEdge,
                            Ipp32u InLeftVerEdge, Ipp32u InRightVerEdge);

    void SetDblkHorEdgeU(Ipp32u ExHorEdge, Ipp32u InHorEdge);
    void SetDblkVerEdgeU(Ipp32u ExVerEdge, Ipp32u InVerEdge);

    void SetDblkHorEdgeV(Ipp32u ExHorEdge, Ipp32u InHorEdge);
    void SetDblkVerEdgeV(Ipp32u ExVerEdge, Ipp32u InVerEdge);

    void SetVTSType(eTransformType type[6]);

    void SetFieldMVPred2Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward, Ipp32s blk);
    void SetFieldMVPred2Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward);

    void SetFieldMVPred1Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward, Ipp32s blk);
    void SetFieldMVPred1Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward);

    void SetHybrid(Ipp8u hybrid, Ipp32s forward, Ipp32s blk);
    void SetHybrid(Ipp8u hybrid, Ipp32s forward);

    void SetScaleType(Ipp8u type, Ipp32s forward, Ipp32s blk);
    void SetScaleType(Ipp8u type, Ipp32s forward);

    void SetPredFlag(Ipp8u pred, Ipp32s forward, Ipp32s blk);
    void SetPredFlag(Ipp8u pred, Ipp32s forward);

    void PrintRestoredFrame(Ipp8u* pY, Ipp32s Ystep, Ipp8u* pU, Ipp32s Ustep, Ipp8u* pV, Ipp32s Vstep, bool BFrame);

private:
    vm_file*  LogFile;
    Ipp32u FrameCount;
    Ipp32u FrameType;
    Ipp32s FrameSize;
    Ipp32u FieldPic;
    Ipp32u RefNum;

    Ipp32s MBHeight; //frame height in macroblocks
    Ipp32s MBWidth;  //frame width in macroblocks

    Ipp32s XPos;      // current MB X position
    Ipp32s YPos;      // current MB Y position

    Ipp32s XFieldPos;      // current MB X position
    Ipp32s YFieldPos;      // current MB Y position


    VC1MBDebugInfo* MBs;
    VC1MBDebugInfo* pCurrMB;

    Ipp32s InterpType;
    Ipp32s RoundControl;
    Ipp32s HalfCoef;
    bool DeblkFlag;
    bool VTSFlag;

    void PrintBlockDifference(Ipp32s blk_num);
    void PrintInterpolationInfo();
    void PrintInterpolationInfoField();
    void PrintCopyPatchInterpolation(Ipp32s blk_num, Ipp8u back);
    void PrintInterpQuarterPelBicubic(Ipp32s blk_num, Ipp8u back);
    void PrintInterpQuarterPelBicubicVert(Ipp32s blk_num, Ipp8u back);
    void PrintInterpQuarterPelBicubicHoriz(Ipp32s blk_num, Ipp8u back);

    void PrintChroma_B_4MV(Ipp8u back);
    void PrintChroma_P_4MVField();
    void PrintChroma();
    void Print1MVHalfPelBilinear_P();
    void Print1MVHalfPelBilinear_PField();
    void Print1MVHalfPelBilinear_B_FB();
    void Print1MVHalfPelBilinear_B_F();
    void Print1MVHalfPelBilinear_B_B();
    void Print1MVHalfPelBilinear_B_FB_Field();
    void Print1MVHalfPelBilinear_B_F_Field();
    void Print1MVHalfPelBilinear_B_B_Field();

    void PrintMVInfo();
    void PrintMVFieldInfo();
    void PrintFieldMV(Ipp32s forward);

    void PrintRunLevel(Ipp32s blk_num);
    void PrintDblkInfo();
    void PrintDblkInfoVTS();
    void PrintDblkInfoNoVTS();
    void PrintFieldDblkInfo(bool bSecond);
    void PrintFieldDblkInfoVTS(bool bSecond);
    void PrintFieldDblkInfoNoVTS(bool bSecond);
    void PrintBlkVTSInfo();
    void PrintPictureType();
    void PrintFieldInfo(bool bSecondField); //wrote debug information
    void PrintFrameInfo();
};

extern VC1EncDebug *pDebug;

#endif
}//namespace

#endif //__UMC_VC1_ENC_DEBUG_H__
