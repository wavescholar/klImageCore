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

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_debug.h"
#include "vm_debug.h"
#include "umc_vc1_enc_def.h"

namespace UMC_VC1_ENCODER
{
#ifdef VC1_ENC_DEBUG_ON

VC1EncDebug *pDebug;

const Ipp32u  VC1_ENC_POSITION     = 0x00000001;  //Frame num, frame type, frame size,
                                                  //MB, Block positions, skip info
const Ipp32u  VC1_ENC_COEFFS       = 0x00000080;  // DC, AC coefficiens
const Ipp32u  VC1_ENC_AC_PRED      = 0x00004000;  // AC prediction
const Ipp32u  VC1_ENC_QUANT        = 0x00000008;  // quant info
const Ipp32u  VC1_ENC_CBP          = 0x00000002;  // coded block patern info
const Ipp32u  VC1_ENC_MV           = 0x00000020;  // motion vectors info
const Ipp32u  VC1_ENC_PRED         = 0x00000040;  //interpolation info
const Ipp32u  VC1_ENC_DEBLK_EDGE   = 0x00008000;  //deblocking edge
const Ipp32u VC1_ENC_FIELD_MV      = 0x00002000;  //field motion vectors info

const Ipp32u  VC1_ENC_DEBLOCKING   = 0x0;         //deblocking info

const Ipp32u  VC1_ENC_FRAME_DEBUG  = 0;           //on/off frame debug
const Ipp32u  VC1_ENC_FRAME_MIN    = 0;           //first frame to debug
const Ipp32u  VC1_ENC_FRAME_MAX    = 2;           //last frame to debug

const Ipp32u  VC1_ENC_DEBUG     = 0x0;//0x0000E0e3;  //current debug output

VC1EncDebug::VC1EncDebug()
{
    LogFile     = NULL;
    FrameCount  = 0;
    FrameType   = 0;
    FrameSize   = 0;
    MBWidth   = 0;
    MBHeight  = 0;
    XPos      = 0;
    YPos      = 0;
    MBs          = NULL;
    pCurrMB      = NULL;
    InterpType = 0;
    RoundControl = 0;
    HalfCoef   = 1;
    DeblkFlag    = false;
    VTSFlag      = false;
    FieldPic     = 0;
};

VC1EncDebug::~VC1EncDebug()
{
    Close();
};

void VC1EncDebug::Init(Ipp32s Width, Ipp32s Height)
{
    VM_ASSERT(Width <= 0);
    VM_ASSERT(Height <= 0);

    MBHeight = Height;
    MBWidth  = Width;
    RefNum   = 0;

    //open log file
    LogFile = vm_file_open(VM_STRING("enc_log.txt"),VM_STRING("wb"));
    VM_ASSERT(LogFile != NULL);

    MBs = (VC1MBDebugInfo*)ippMalloc(sizeof(VC1MBDebugInfo) * MBWidth * MBHeight);

    VM_ASSERT(MBs == NULL);
    memset(MBs, 0, sizeof(VC1MBDebugInfo) * MBWidth * MBHeight);

    pCurrMB = MBs;
};

void VC1EncDebug::Close()
{
    //close log file
    if(LogFile)
    {
        vm_file_close(LogFile);
        LogFile = NULL;
    }

    FrameCount  = 0;
    FrameType   = 0;
    FieldPic    = 0;
    FrameSize   = 0;
    MBWidth   = 0;
    MBHeight  = 0;
    XPos      = 0;
    YPos      = 0;
    RefNum    = 0;

    if(MBs != NULL)
    {
        ippFree(MBs);
        MBs = NULL;
    }
    pCurrMB      = NULL;
};

void VC1EncDebug::SetPicType(Ipp32s picType)
{
    FrameType = picType;
    FieldPic  = 0;

    if(FrameType == VC1_ENC_I_I_FIELD || FrameType == VC1_ENC_I_P_FIELD
        || FrameType == VC1_ENC_P_I_FIELD ||  FrameType == VC1_ENC_P_P_FIELD
        || FrameType == VC1_ENC_B_BI_FIELD || FrameType == VC1_ENC_BI_B_FIELD
        || FrameType == VC1_ENC_B_B_FIELD)
        FieldPic = 1;
};

void VC1EncDebug::SetRefNum(Ipp32u num)
{
    RefNum = num;
}

void VC1EncDebug::SetFrameSize(Ipp32s frameSize)
{
    FrameSize = frameSize;
};

void VC1EncDebug::WriteParams(Ipp32s /*_cur_frame*/, Ipp32s level, vm_char *format,...)
{
    vm_char line[1024];
    va_list args;

    if (!(level & VC1_ENC_DEBUG))
       return;

   va_start(args, format);
   vm_string_vsprintf(line, format, args);
   vm_string_fprintf(LogFile, line);
   //vm_string_printf(line);
};

void VC1EncDebug::SetRunLevelCoefs(Ipp8u* run, Ipp16s* level, Ipp8u* pairs, Ipp32s blk_num)
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s pairNum = 0;
    Ipp32s coefNum = 0;

    pCurrMB->Block[blk_num].Pairs[0] = pairs[0];
    pCurrMB->Block[blk_num].Pairs[1] = pairs[1];
    pCurrMB->Block[blk_num].Pairs[2] = pairs[2];
    pCurrMB->Block[blk_num].Pairs[3] = pairs[3];

    for(i = 0; i < 4; i++)
    {
        pairNum =  pairs[i];

        for(j = 0; j < pairNum; j++)
        {
            pCurrMB->Block[blk_num].Run[coefNum]   = run[coefNum];
            pCurrMB->Block[blk_num].Level[coefNum] = level[coefNum];
            coefNum++;
        }
    }

     VM_ASSERT(coefNum > 64);
};

void VC1EncDebug::NextMB()
{
    pCurrMB++;

    XPos++;

    XFieldPos++;
    if(XPos >= MBWidth)
    {
        XPos = 0;
        YPos++;
        XFieldPos = 0;
        YFieldPos++;
    }
};

void VC1EncDebug::SetCurrMBFirst(bool bSecondField)
{
    if(!bSecondField)
    {
        pCurrMB = MBs;
        XPos = 0;
        YPos = 0;
        XFieldPos = 0;
        YFieldPos = 0;
    }
    else
    {
        pCurrMB = MBs + MBHeight/2*MBWidth;
        XPos = 0;
        YPos = MBHeight/2;
        XFieldPos = 0;
        YFieldPos = 0;
    }
}

void VC1EncDebug::SetRLMode(Ipp8u mode, Ipp32s blk_num, Ipp32s coef_num)
{
    assert(mode < 65);
    pCurrMB->Block[blk_num].Mode[coef_num] = mode;
};


void VC1EncDebug::SetBlockAsIntra(Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].intra = 1;
};

void VC1EncDebug::SetBlockDifference(Ipp16s** pBlock, Ipp32u* step)
{
    Ipp32s blk_num = 0;
    Ipp32s i = 0;
    Ipp32s j = 0;

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        for(i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BlockDiff[i*8 + j] = pBlock[blk_num][i * step[blk_num]/2 + j];
            }
        }
    }

};

void VC1EncDebug::SetMBAsSkip()
{
    pCurrMB->skip = 1;
};

void VC1EncDebug::SetDCACInfo(Ipp32s ACPred, Ipp16s* pBlock, Ipp32s BlockStep,
                              Ipp16s* pPredBlock, Ipp32s PredBlockStep,
                              Ipp32s direction, Ipp32s blk_num)
{
    Ipp32s i = 0;

    pCurrMB->Block[blk_num].DC = pBlock[0];
    pCurrMB->Block[blk_num].DCDiff = pPredBlock[0];
    pCurrMB->Block[blk_num].DCDirection = direction;

    pCurrMB->Block[blk_num].ACPred = ACPred;

    if(ACPred >= 0)
    {
        if(direction == VC1_ENC_LEFT)
        {
            //AC left prediction
            for(i = 1; i < 8; i++)
            {
                pCurrMB->Block[blk_num].AC[i - 1] = pBlock[i * BlockStep/2] - pPredBlock[i * PredBlockStep/2];
            }
        }
        else
        {
            //AC top prediction
            for(i = 1; i < 8; i++)
            {
                pCurrMB->Block[blk_num].AC[i - 1] = pBlock[i] - pPredBlock[i];
            }
        }
    }
};

void VC1EncDebug::SetCPB(Ipp32s predcbp, Ipp32s cbp)
{
    pCurrMB->PredCBP = predcbp;
    pCurrMB->CBP = cbp;
};

void VC1EncDebug::SetQuant(Ipp32s MQuant, Ipp8u  HalfQP)
{
    pCurrMB->MQuant = MQuant;
    pCurrMB->HalfQP = HalfQP;

};

void VC1EncDebug::SetMVInfo(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 4; i++)
    {
        pCurrMB->Block[i].MV[0][forward]    = MV->x;
        pCurrMB->Block[i].MV[1][forward]    = MV->y;
        pCurrMB->Block[i].PredMV[0][forward] = predX;
        pCurrMB->Block[i].PredMV[1][forward] = predY;
    }
};

void VC1EncDebug::SetMVInfo(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward, Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].MV[0][forward]    = MV->x;
    pCurrMB->Block[blk_num].MV[1][forward]    = MV->y;
    pCurrMB->Block[blk_num].PredMV[0][forward] = predX;
    pCurrMB->Block[blk_num].PredMV[1][forward] = predY;
};

void VC1EncDebug::SetMVInfoField(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 4; i++)
    {
        pCurrMB->Block[i].MV[0][forward]    = MV->x;
        pCurrMB->Block[i].MV[1][forward]    = MV->y;
        pCurrMB->Block[i].MVField[forward]  = MV->bSecond;
        pCurrMB->Block[i].PredMV[0][forward] = predX;
        pCurrMB->Block[i].PredMV[1][forward] = predY;
    }
};

void VC1EncDebug::SetMVInfoField(sCoordinate* MV, Ipp16s predX, Ipp16s predY, Ipp32s forward, Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].MV[0][forward]    = MV->x;
    pCurrMB->Block[blk_num].MV[1][forward]    = MV->y;
    pCurrMB->Block[blk_num].MVField[forward]  = MV->bSecond;
    pCurrMB->Block[blk_num].PredMV[0][forward] = predX;
    pCurrMB->Block[blk_num].PredMV[1][forward] = predY;
};

void VC1EncDebug::SetIntrpMV(Ipp16s X, Ipp16s Y, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 4; i++)
    {
        pCurrMB->Block[i].IntrpMV[0][forward] = X;
        pCurrMB->Block[i].IntrpMV[1][forward] = Y;
    }
}

void VC1EncDebug::SetIntrpMV(Ipp16s X, Ipp16s Y, Ipp32s forward, Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].IntrpMV[0][forward] = X;
    pCurrMB->Block[blk_num].IntrpMV[1][forward] = Y;
}

void VC1EncDebug::SetMVDiff(Ipp16s DiffX, Ipp16s DiffY, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 6; i++)
    {
        pCurrMB->Block[i].difMV[0][forward]    = DiffX;
        pCurrMB->Block[i].difMV[1][forward]    = DiffY;
    }
};

void VC1EncDebug::SetMVDiff(Ipp16s DiffX, Ipp16s DiffY, Ipp32s forward, Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].difMV[0][forward]    = DiffX;
    pCurrMB->Block[blk_num].difMV[1][forward]    = DiffY;
};

void VC1EncDebug::SetFieldMVPred2Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward)
{
    Ipp32u i = 0;

    if(A)
    {
        for(i = 0; i < 6; i++)
        {
            pCurrMB->Block[i].FieldMV.pred[0][forward][0] = A->x;
            pCurrMB->Block[i].FieldMV.pred[0][forward][1] = A->y;
        }
    }

    if(C)
    {
        for(i = 0; i < 6; i++)
        {
            pCurrMB->Block[i].FieldMV.pred[2][forward][0] = C->x;
            pCurrMB->Block[i].FieldMV.pred[2][forward][1] = C->y;
        }
    }
}

void VC1EncDebug::SetFieldMVPred2Ref(sCoordinate *A, sCoordinate *C,     Ipp32s forward, Ipp32s blk)
{
    if(A)
    {
        pCurrMB->Block[blk].FieldMV.pred[0][forward][0] = A->x;
        pCurrMB->Block[blk].FieldMV.pred[0][forward][1] = A->y;
    }

    if(C)
    {
        pCurrMB->Block[blk].FieldMV.pred[2][forward][0] = C->x;
        pCurrMB->Block[blk].FieldMV.pred[2][forward][1] = C->y;
    }
}

void VC1EncDebug::SetFieldMVPred1Ref(sCoordinate *A, sCoordinate *C, Ipp32s forward)
{
    Ipp32u i = 0;

    if(A)
    {
        for(i = 0; i < 6; i++)
        {
            pCurrMB->Block[i].FieldMV.pred[0][forward][0] = A->x;
            pCurrMB->Block[i].FieldMV.pred[0][forward][1] = A->y;
        }
    }

    if(C)
    {
        for(i = 0; i < 6; i++)
        {
            pCurrMB->Block[i].FieldMV.pred[2][forward][0] = C->x;
            pCurrMB->Block[i].FieldMV.pred[2][forward][1] = C->y;
        }
    }
}

void VC1EncDebug::SetFieldMVPred1Ref(sCoordinate *A, sCoordinate *C,  Ipp32s forward, Ipp32s blk)
{
    if(A)
    {
        pCurrMB->Block[blk].FieldMV.pred[0][forward][0] = A->x;
        pCurrMB->Block[blk].FieldMV.pred[0][forward][1] = A->y;
    }

    if(C)
    {
        pCurrMB->Block[blk].FieldMV.pred[2][forward][0] = C->x;
        pCurrMB->Block[blk].FieldMV.pred[2][forward][1] = C->y;
    }
}

void VC1EncDebug::SetHybrid(Ipp8u hybrid, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 6; i++)
       pCurrMB->Block[i].FieldMV.hybrid[forward] = hybrid;
}

void VC1EncDebug::SetHybrid(Ipp8u hybrid, Ipp32s forward, Ipp32s blk)
{
   pCurrMB->Block[blk].FieldMV.hybrid[forward] = hybrid;
}

void VC1EncDebug::SetScaleType(Ipp8u type, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 6; i++)
        pCurrMB->Block[i].FieldMV.scaleType[forward] = type;
}

void VC1EncDebug::SetScaleType(Ipp8u type, Ipp32s forward, Ipp32s blk)
{
    pCurrMB->Block[blk].FieldMV.scaleType[forward] = type;
}

void VC1EncDebug::SetPredFlag(Ipp8u pred, Ipp32s forward)
{
    Ipp32u i = 0;

    for(i = 0; i < 6; i++)
        pCurrMB->Block[i].FieldMV.predFlag[forward] = pred;
}

void VC1EncDebug::SetPredFlag(Ipp8u pred, Ipp32s forward, Ipp32s blk)
{
    pCurrMB->Block[blk].FieldMV.predFlag[forward] = pred;
}

void VC1EncDebug::SetMBType(Ipp32s type)
{
    pCurrMB->MBType = type;
};

void VC1EncDebug::SetInterpInfo(Ipp8u* pBlock, Ipp32s step, Ipp32s blk_num, Ipp32s forward, const Ipp8u* pSrc, Ipp32s srcStep)
{
    Ipp32s i = 0;
    Ipp32s j = 0;


    if(!forward)
    {
        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
    }
    else
    {
        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
    }

};

void VC1EncDebug::SetInterpInfo(IppVCInterpolate_8u* YInfo, IppVCInterpolate_8u* UInfo, IppVCInterpolate_8u* VInfo, Ipp32s forward)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u* pBlock;
    Ipp32s step;
    Ipp32s blk_num;
    const Ipp8u* pSrc;
    Ipp32s srcStep;

    if(!forward)
    {
        //block 0
        pBlock = YInfo->pDst;
        step = YInfo->dstStep;
        blk_num = 0;
        pSrc = YInfo->pSrc;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 1
        pBlock = YInfo->pDst + 8;
        step = YInfo->dstStep;
        blk_num = 1;
        pSrc = YInfo->pSrc + 8;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 2
        pBlock = YInfo->pDst + 8 * YInfo->dstStep;
        step = YInfo->dstStep;
        blk_num = 2;
        pSrc = YInfo->pSrc + 8 * YInfo->srcStep;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 3
        pBlock = YInfo->pDst + 8 * YInfo->dstStep + 8;
        step = YInfo->dstStep;
        blk_num = 3;
        pSrc = YInfo->pSrc + 8 * YInfo->srcStep + 8;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 4
        pBlock = UInfo->pDst;
        step = UInfo->dstStep;
        blk_num = 4;
        pSrc = UInfo->pSrc;
        srcStep = UInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }
        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 5
        pBlock = VInfo->pDst;
        step = VInfo->dstStep;
        blk_num = 5;
        pSrc = VInfo->pSrc;
        srcStep = VInfo->srcStep;

        pCurrMB->Block[blk_num].FSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].FDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].FSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
    }
    else
    {
        //block 0
        pBlock = YInfo->pDst;
        step = YInfo->dstStep;
        blk_num = 0;
        pSrc = YInfo->pSrc;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 1
        pBlock = YInfo->pDst + 8;
        step = YInfo->dstStep;
        blk_num = 1;
        pSrc = YInfo->pSrc + 8;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 2
        pBlock = YInfo->pDst + 8 * YInfo->dstStep;
        step = YInfo->dstStep;
        blk_num = 2;
        pSrc = YInfo->pSrc + 8 * YInfo->srcStep;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
        //block 3
        pBlock = YInfo->pDst + 8 * YInfo->dstStep + 8;
        step = YInfo->dstStep;
        blk_num = 3;
        pSrc = YInfo->pSrc + 8 * YInfo->srcStep + 8;
        srcStep = YInfo->srcStep;

        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
        //block 4
        pBlock = UInfo->pDst;
        step = UInfo->dstStep;
        blk_num = 4;
        pSrc = UInfo->pSrc;
        srcStep = UInfo->srcStep;
        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }

        //block 5
        pBlock = VInfo->pDst;
        step = VInfo->dstStep;
        blk_num = 5;
        pSrc = VInfo->pSrc;
        srcStep = VInfo->srcStep;
        pCurrMB->Block[blk_num].BSrc = pSrc[0];
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                pCurrMB->Block[blk_num].BDst[i*8 + j] = pBlock[j + i * step];
            }
        }

        for(i = 0; i < 11; i++)
        {
            for(j = 0; j < 11; j++)
            {
                pCurrMB->Block[blk_num].BSource[i*11 + j] = pSrc[(j - 1) + (i-1) * srcStep];
            }
        }
    }
}

void VC1EncDebug::SetInterpolType(Ipp32s type)
{
    InterpType = type;
}

void VC1EncDebug::SetRounControl(Ipp32s rc)
{
    RoundControl = rc;
}

void VC1EncDebug::SetACDirection(Ipp32s direction, Ipp32s blk_num)
{
    pCurrMB->Block[blk_num].ACPred = direction;
};

void VC1EncDebug::SetHalfCoef(bool half)
{
    HalfCoef = 1 + (Ipp32u)(half);
};

void VC1EncDebug::SetDeblkFlag(bool flag)
{
    DeblkFlag = flag;
};

void VC1EncDebug::SetVTSFlag(bool flag)
{
    VTSFlag = flag;
};

void VC1EncDebug::SetDblkHorEdgeLuma(Ipp32u ExHorEdge,     Ipp32u InHorEdge,
                                     Ipp32u InUpHorEdge,   Ipp32u InBotHorEdge)
{
    pCurrMB->ExHorEdgeLuma = ExHorEdge;
    pCurrMB->InHorEdgeLuma = InHorEdge;
    pCurrMB->InUpHorEdgeLuma = InUpHorEdge;
    pCurrMB->InBotHorEdgeLuma = InBotHorEdge;

    //static FILE* f;
   //if (!f)
    //    f=fopen("set.txt","wb");

    //fprintf(f, "pCurrMB->ExHorEdgeLuma = %d\n",pCurrMB->ExHorEdgeLuma);
    //fprintf(f, "pCurrMB->InHorEdgeLuma = %d\n",pCurrMB->InHorEdgeLuma);
    //fprintf(f, "pCurrMB->InUpHorEdgeLuma = %d\n",pCurrMB->InUpHorEdgeLuma);
    //fprintf(f, "pCurrMB->InBotHorEdgeLuma = %d\n\n",pCurrMB->InBotHorEdgeLuma);

};
void VC1EncDebug::SetDblkVerEdgeLuma(Ipp32u ExVerEdge,     Ipp32u InVerEdge,
                        Ipp32u InLeftVerEdge, Ipp32u InRightVerEdge)
{
    pCurrMB->ExVerEdgeLuma = ExVerEdge;
    pCurrMB->InVerEdgeLuma = InVerEdge;
    pCurrMB->InLeftVerEdgeLuma = InLeftVerEdge;
    pCurrMB->InRightVerEdgeLuma = InRightVerEdge;

    //static FILE* f;

    //if (!f)
    //    f=fopen("set.txt","wb");

    //fprintf(f, "pCurrMB->ExVerEdgeLuma = %d\n",pCurrMB->ExVerEdgeLuma);
    //fprintf(f, "pCurrMB->InVerEdgeLuma = %d\n",pCurrMB->InVerEdgeLuma);
    //fprintf(f, "pCurrMB->InLeftVerEdgeLuma = %d\n",pCurrMB->InLeftVerEdgeLuma);
    //fprintf(f, "pCurrMB->InRightVerEdgeLuma = %d\n\n",pCurrMB->InRightVerEdgeLuma);
};

void VC1EncDebug::SetDblkHorEdgeU(Ipp32u ExHorEdge, Ipp32u InHorEdge)
{
    pCurrMB->ExHorEdgeU = ExHorEdge;
    pCurrMB->InHorEdgeU = InHorEdge;
};

void VC1EncDebug::SetDblkVerEdgeU(Ipp32u ExVerEdge, Ipp32u InVerEdge)
{
    pCurrMB->ExVerEdgeU = ExVerEdge;
    pCurrMB->InVerEdgeU = InVerEdge;
};

void VC1EncDebug::SetDblkHorEdgeV(Ipp32u ExHorEdge, Ipp32u InHorEdge)
{
    pCurrMB->ExHorEdgeV = ExHorEdge;
    pCurrMB->InHorEdgeV = InHorEdge;
};

void VC1EncDebug::SetDblkVerEdgeV(Ipp32u ExVerEdge, Ipp32u InVerEdge)
{
    pCurrMB->ExVerEdgeV = ExVerEdge;
    pCurrMB->InVerEdgeV = InVerEdge;
};

void VC1EncDebug::SetVTSType(eTransformType type[6])
{
    Ipp32s blk_num = 0;
    for(blk_num = 0; blk_num < 6; blk_num ++)
    {
        pCurrMB->Block[blk_num].VTSType = type[blk_num];
    }
}

void VC1EncDebug::PrintBlockDifference(Ipp32s blk_num)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("Block %d\n"), blk_num);
    for(i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("%d  "), pCurrMB->Block[blk_num].BlockDiff[i * 8 + j]);
        }
        WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("\n"));
    }
};

void VC1EncDebug::PrintInterpolationInfo()
{
    Ipp32s blk_num = 0;

    switch(pCurrMB->MBType)
    {
        case (UMC_VC1_ENCODER::VC1_ENC_I_MB):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_INTRA):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_INTRA):
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_1MV):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_1MV):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_P();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                    if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 0);
                    }
                }
            }
            PrintChroma_B_4MV(0);
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_FB):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_B):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_DIRECT):
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_F):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_B_F();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                   if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 0);
                    }
                }
            }

            PrintChroma_B_4MV(0);

            WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);
            break;
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_B):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_B_B();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                   if((0 == (pCurrMB->Block[blk_num].MV[0][1] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][1] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 1);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][1] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 1);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][1] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 1);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 1);
                    }
               }
            }

            PrintChroma_B_4MV(1);

            WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

            break;
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_FB):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_B_FB();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                   if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 0);
                    }

                    if((0 == (pCurrMB->Block[blk_num].MV[0][1] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][1] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 1);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][1] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 1);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][1] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 1);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 1);
                    }
               }
            }

            PrintChroma();

            WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

            break;
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_4MV):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_4MV):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_P();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                    if(!pCurrMB->Block[blk_num].intra)
                    {
                        if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                        {
                            PrintCopyPatchInterpolation(blk_num, 0);
                        }
                        else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                        {
                            PrintInterpQuarterPelBicubicVert(blk_num, 0);
                        }
                        else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                        {
                            PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                        }
                        else
                        {
                            PrintInterpQuarterPelBicubic(blk_num, 0);
                        }
                    }
                }
            }

            if(!pCurrMB->Block[4].intra)
                      PrintChroma_B_4MV(0);
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_DIRECT):
            Print1MVHalfPelBilinear_B_FB();
            break;
    };
};

void VC1EncDebug::PrintInterpolationInfoField()
{
    Ipp32s blk_num = 0;

    switch(pCurrMB->MBType)
    {
        case (UMC_VC1_ENCODER::VC1_ENC_I_MB):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_INTRA):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_INTRA):
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_1MV):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_1MV):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_PField();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                    if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                    {
                        PrintCopyPatchInterpolation(blk_num, 1);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicVert(blk_num, 0);
                    }
                    else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                    {
                        PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                    }
                    else
                    {
                        PrintInterpQuarterPelBicubic(blk_num, 0);
                    }
                }
            }
            PrintChroma_P_4MVField();
            break;

            //WriteParams(-1,VC1_ENC_MV,  "FORWARD: MV_X  = %d, MV_Y  = %d\n",
            //    pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
            //WriteParams(-1,VC1_ENC_MV,  "BACKWARD: MV_X  = %d, MV_Y  = %d\n",
            //    pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_F):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F):
            Print1MVHalfPelBilinear_B_F_Field();
            break;
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_B):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_B):
            Print1MVHalfPelBilinear_B_B_Field();
            break;
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_FB):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_FB):
            Print1MVHalfPelBilinear_B_FB_Field();
            break;
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_4MV):
        case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_4MV):
            if(InterpType)
            {
                Print1MVHalfPelBilinear_P();
            }
            else
            {
                for(blk_num = 0; blk_num < 4; blk_num++)
                {
                    if(!pCurrMB->Block[blk_num].intra)
                    {
                        if((0 == (pCurrMB->Block[blk_num].MV[0][0] & 3)) && (0 == (pCurrMB->Block[blk_num].MV[1][0] & 3)))
                        {
                            PrintCopyPatchInterpolation(blk_num, 1);
                        }
                        else if(0 == (pCurrMB->Block[blk_num].MV[0][0] & 3))
                        {
                            PrintInterpQuarterPelBicubicVert(blk_num, 0);
                        }
                        else if(0 == (pCurrMB->Block[blk_num].MV[1][0] & 3))
                        {
                            PrintInterpQuarterPelBicubicHoriz(blk_num, 0);
                        }
                        else
                        {
                            PrintInterpQuarterPelBicubic(blk_num, 0);
                        }
                    }
                }
            }

            if(!pCurrMB->Block[4].intra)
                      PrintChroma_B_4MV(0);
            break;

        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_DIRECT):
        case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_DIRECT):
            Print1MVHalfPelBilinear_B_FB_Field();
            break;
    };
};
void VC1EncDebug::PrintInterpQuarterPelBicubicVert(Ipp32s blk_num, Ipp8u back)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , back);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING(" vc1_MVMode1MV\n"));
    WriteParams(-1,VC1_ENC_PRED,VM_STRING("PelBicubicVert\n"));

   for (i = 0; i < 8; i ++)
   {
      for (j = 0; j < 8; j++)
      {
          if(!back)
          WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
          else
          WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
      }
       WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
    }
}

void VC1EncDebug::PrintInterpQuarterPelBicubicHoriz(Ipp32s blk_num, Ipp8u back)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , back);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING(" vc1_MVMode1MV\n"));
    WriteParams(-1,VC1_ENC_PRED,VM_STRING("PelBicubicHoriz\n"));

   for (i = 0; i < 8; i ++)
   {
      for (j = 0; j < 8; j++)
      {
          if(!back)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
          else
              WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
      }
       WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
    }
}
void VC1EncDebug::PrintInterpQuarterPelBicubic(Ipp32s blk_num, Ipp8u back)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , back);

    WriteParams(-1,VC1_ENC_PRED,VM_STRING(" vc1_MVMode1MV\n"));
    WriteParams(-1,VC1_ENC_PRED,VM_STRING("PelBicubicDiag\n"));

    if(!back)
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);
    else
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
        (pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
        (pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);


    WriteParams(-1,VC1_ENC_PRED,VM_STRING("Source\n"));


   for (i = 0; i < 11; i ++)
   {
      for (j = 0; j < 11; j++)
      {
          if(!back)
          WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FSource[j + i*11]);
          else
              WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BSource[j + i*11]);
      }
       WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
    }

   WriteParams(-1,VC1_ENC_PRED,VM_STRING("rcontrol=%d\n"),RoundControl);

   WriteParams(-1,VC1_ENC_PRED,VM_STRING("after interpolate\n\n"));

   for (i = 0; i < 8; i ++)
   {
      for (j = 0; j < 8; j++)
      {
          if(!back)
          WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
          else
          WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
      }
       WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
    }
}

void VC1EncDebug::Print1MVHalfPelBilinear_B_B()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 1);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
            (pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }

        WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

}

void VC1EncDebug::Print1MVHalfPelBilinear_B_F()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);
        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

}
void VC1EncDebug::Print1MVHalfPelBilinear_B_FB()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        //forward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }

        //backward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 1);


        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
            (pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }

    }
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

}
void VC1EncDebug::Print1MVHalfPelBilinear_B_B_Field()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].IntrpMV[0][1] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].IntrpMV[1][1] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 1);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][1] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][1] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }

        WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0], pCurrMB->Block[0].MVField[0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1], pCurrMB->Block[0].MVField[1]);
}

void VC1EncDebug::Print1MVHalfPelBilinear_B_F_Field()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);
        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0], pCurrMB->Block[0].MVField[0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1], pCurrMB->Block[0].MVField[1]);
}
void VC1EncDebug::Print1MVHalfPelBilinear_B_FB_Field()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        //forward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }

        //backward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].IntrpMV[0][1] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].IntrpMV[1][1] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 1);


        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][1] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][1] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }

    }

    WriteParams(-1,VC1_ENC_MV,  VM_STRING("FORWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0], pCurrMB->Block[0].MVField[0]);
    WriteParams(-1,VC1_ENC_MV,  VM_STRING("BACKWARD: MV_X  = %d, MV_Y  = %d, bSecond = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1], pCurrMB->Block[0].MVField[1]);

}
void VC1EncDebug::Print1MVHalfPelBilinear_P()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 4; blk_num++)
    {
        //forward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
                pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
                pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
}

void VC1EncDebug::Print1MVHalfPelBilinear_PField()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s blk_num = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 0; blk_num < 4; blk_num++)
    {
        //forward
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
                pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
                pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" MVMode1MVHalfPelBilinear\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
}

void VC1EncDebug::PrintCopyPatchInterpolation(Ipp32s blk_num, Ipp8u back)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , back);

        if(blk_num < 4)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING(" vc1_MVMode1MV\n"));

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("CopyPatch\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                if(!back)
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
                else
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
}

void VC1EncDebug::PrintChroma_B_4MV(Ipp8u back)
{
    Ipp32s blk_num = 0;
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 4; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , back);

        if(!back)
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
                (pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
                (pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);
        else
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
                (pCurrMB->Block[blk_num].MV[0][back] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
                (pCurrMB->Block[blk_num].MV[1][back] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);


        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                if(!back)
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
                else
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
}

void VC1EncDebug::PrintChroma()
{
    Ipp32s blk_num = 0;
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 4; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
                (pCurrMB->Block[blk_num].MV[0][0] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
                (pCurrMB->Block[blk_num].MV[1][0] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);


        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 1);

            WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].BSrc,
                (pCurrMB->Block[blk_num].MV[0][1] + (XPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
                (pCurrMB->Block[blk_num].MV[1][1] + (YPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);


        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                    WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].BDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
}

void VC1EncDebug::PrintChroma_P_4MVField()
{
    Ipp32s blk_num = 0;
    Ipp32s i = 0;
    Ipp32s j = 0;

    Ipp8u Xshift[6] = {0, 8, 0, 8, 0, 0};
    Ipp8u Yshift[6] = {0, 0, 8, 8, 0, 0};
    Ipp8u pixelInBlk[6] = {16, 16, 16, 16, 8, 8};

    for(blk_num = 4; blk_num < 6; blk_num++)
    {
        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Block %d\n"), blk_num);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Before Interpolation %d, %d, forward %d\n"),
            pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4,
            pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4 , 0);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d\t %d\t %d\n"),pCurrMB->Block[blk_num].FSrc,
            (pCurrMB->Block[blk_num].IntrpMV[0][0] + (XFieldPos * pixelInBlk[blk_num] + Xshift[blk_num])*4)>>2,
            (pCurrMB->Block[blk_num].IntrpMV[1][0] + (YFieldPos * pixelInBlk[blk_num] + Yshift[blk_num])*4)>>2);

        WriteParams(-1,VC1_ENC_PRED,VM_STRING("Predicted pels\n"));
        for (i = 0; i < 8; i ++)
        {
            for (j = 0; j < 8; j++)
            {
                WriteParams(-1,VC1_ENC_PRED,VM_STRING("%d "),pCurrMB->Block[blk_num].FDst[j + i*8]);
            }
            WriteParams(-1,VC1_ENC_PRED,VM_STRING("\n"));
        }
    }
}

void VC1EncDebug::PrintFieldMV(Ipp32s forward)
{
    WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("predictor_flag = %d\n"), pCurrMB->Block[0].FieldMV.predFlag[forward]);
    if(pCurrMB->Block[0].FieldMV.scaleType[forward] == 0)
        WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("opposite\n"));
    else  if(pCurrMB->Block[0].FieldMV.scaleType[forward] == 1)
        WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("same\n"));

    WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("AX = %d AY = %d\n"),
                                    pCurrMB->Block[0].FieldMV.pred[0][forward][0],
                                    pCurrMB->Block[0].FieldMV.pred[0][forward][1]);

    WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("CX = %d CY = %d\n"),
                                    pCurrMB->Block[0].FieldMV.pred[2][forward][0],
                                    pCurrMB->Block[0].FieldMV.pred[2][forward][1]);

    if(pCurrMB->Block[0].FieldMV.hybrid[forward])
        WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("hybrid\n"));
    else
        WriteParams(-1,VC1_ENC_FIELD_MV,VM_STRING("no hybrid\n"));
}

void VC1EncDebug::PrintMVInfo()
{
    switch(pCurrMB->MBType)
    {
    case (UMC_VC1_ENCODER::VC1_ENC_I_MB):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_INTRA):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_INTRA):
        break;

    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_1MV):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_1MV):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_F):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F):

        //MV diff
       if(!(pCurrMB->skip))
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);

        if((pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_B_MB_F)
            || (pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F))
            WriteParams(-1,VC1_ENC_MV, VM_STRING("forward\n"));

        //MV predictor
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);

        //MV
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);

        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_B):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_B):
        //MV diff
        if(!(pCurrMB->skip))
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][1], pCurrMB->Block[0].difMV[1][1]);

        WriteParams(-1,VC1_ENC_MV, VM_STRING("backward\n"));

            //MV predictors
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[0].PredMV[0][1], pCurrMB->Block[0].PredMV[1][1]);

        //MV
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_FB):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_FB):

        //MV diff
        if(!(pCurrMB->skip))
        {
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][1], pCurrMB->Block[0].difMV[1][1]);

            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);
        }

        WriteParams(-1,VC1_ENC_MV, VM_STRING("BDirecton\n"));
        WriteParams(-1,VC1_ENC_MV, VM_STRING("forward\n"));

        //MV predictors
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);

        WriteParams(-1,VC1_ENC_MV, VM_STRING("backward\n"));

        //MV
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][1], pCurrMB->Block[0].PredMV[1][1]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

        break;

    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_4MV):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_4MV):

        //Block 0
        if(!pCurrMB->Block[0].intra)
        {
            //MV diff
            if(!(pCurrMB->skip) || FieldPic)
                WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                        pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
        }

        //Block 1
        if(!pCurrMB->Block[1].intra)
        {
            //MV diff
            if(!(pCurrMB->skip) || FieldPic)
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[1].difMV[0][0], pCurrMB->Block[1].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[1].PredMV[0][0], pCurrMB->Block[1].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[1].MV[0][0], pCurrMB->Block[1].MV[1][0]);
        }
        //Block 2
        if(!pCurrMB->Block[2].intra)
        {
            //MV diff
            if(!(pCurrMB->skip) || FieldPic)
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[2].difMV[0][0], pCurrMB->Block[2].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[2].PredMV[0][0], pCurrMB->Block[2].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[2].MV[0][0], pCurrMB->Block[2].MV[1][0]);
        }

        //Block 3
        if(!pCurrMB->Block[3].intra)
        {
            //MV diff
            if(!(pCurrMB->skip) || FieldPic)
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[3].difMV[0][0], pCurrMB->Block[3].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[3].PredMV[0][0], pCurrMB->Block[3].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[3].MV[0][0], pCurrMB->Block[3].MV[1][0]);
        }
        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_DIRECT):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_DIRECT):
                WriteParams(-1,VC1_ENC_MV, VM_STRING("direct\n"));
        break;
    };
};

void VC1EncDebug::PrintMVFieldInfo()
{
    switch(pCurrMB->MBType)
    {
    case (UMC_VC1_ENCODER::VC1_ENC_I_MB):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_INTRA):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_INTRA):
        break;

    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_1MV):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_1MV):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_F):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F):

        //MV diff
         WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);

        PrintFieldMV(0);

        if((pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_B_MB_F)
            || (pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_F))
            WriteParams(-1,VC1_ENC_MV, VM_STRING("forward\n"));

        //MV predictor
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);

        //MV
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);

        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_B):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_B):
        //MV diff
        WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][1], pCurrMB->Block[0].difMV[1][1]);

        PrintFieldMV(1);
        WriteParams(-1,VC1_ENC_MV, VM_STRING("backward\n"));

            //MV predictors
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[0].PredMV[0][1], pCurrMB->Block[0].PredMV[1][1]);

        //MV
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_FB):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_FB):

        WriteParams(-1,VC1_ENC_MV, VM_STRING("BDirecton\n"));
        //MV diff
         WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);

        PrintFieldMV(0);
        WriteParams(-1,VC1_ENC_MV, VM_STRING("forward\n"));

        //MV predictors
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);

       //MV diff
        WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                pCurrMB->Block[0].difMV[0][1], pCurrMB->Block[0].difMV[1][1]);

        PrintFieldMV(1);
        WriteParams(-1,VC1_ENC_MV, VM_STRING("backward\n"));

        //MV
        WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
            pCurrMB->Block[0].PredMV[0][1], pCurrMB->Block[0].PredMV[1][1]);
        WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
            pCurrMB->Block[0].MV[0][1], pCurrMB->Block[0].MV[1][1]);

        break;

    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_4MV):
    case (UMC_VC1_ENCODER::VC1_ENC_P_MB_SKIP_4MV):

        //Block 0
        if(!pCurrMB->Block[0].intra)
        {
            //MV diff
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                        pCurrMB->Block[0].difMV[0][0], pCurrMB->Block[0].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[0].PredMV[0][0], pCurrMB->Block[0].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[0].MV[0][0], pCurrMB->Block[0].MV[1][0]);
        }

        //Block 1
        if(!pCurrMB->Block[1].intra)
        {
            //MV diff
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[1].difMV[0][0], pCurrMB->Block[1].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[1].PredMV[0][0], pCurrMB->Block[1].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[1].MV[0][0], pCurrMB->Block[1].MV[1][0]);
        }
        //Block 2
        if(!pCurrMB->Block[2].intra)
        {
            //MV diff
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[2].difMV[0][0], pCurrMB->Block[2].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[2].PredMV[0][0], pCurrMB->Block[2].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[2].MV[0][0], pCurrMB->Block[2].MV[1][0]);
        }

        //Block 3
        if(!pCurrMB->Block[3].intra)
        {
            //MV diff
            WriteParams(-1,VC1_ENC_MV, VM_STRING("DMV_X = %d, DMV_Y = %d\n"),
                    pCurrMB->Block[3].difMV[0][0], pCurrMB->Block[3].difMV[1][0]);

            //MV predictor
            WriteParams(-1,VC1_ENC_MV, VM_STRING("PredictorX = %d, PredictorY = %d\n"),
                pCurrMB->Block[3].PredMV[0][0], pCurrMB->Block[3].PredMV[1][0]);

            //MV
            WriteParams(-1,VC1_ENC_MV,  VM_STRING("ApplyPred : MV_X  = %d, MV_Y  = %d\n"),
                pCurrMB->Block[3].MV[0][0], pCurrMB->Block[3].MV[1][0]);
        }
        break;
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_DIRECT):
    case (UMC_VC1_ENCODER::VC1_ENC_B_MB_SKIP_DIRECT):
        WriteParams(-1,VC1_ENC_MV, VM_STRING("direct\n"));
        break;
    };
};

void VC1EncDebug::PrintRunLevel(Ipp32s blk_num)
{
    Ipp32s i = 0;
    Ipp32s SBNum = 0;
    Ipp32s pairsNum = 0;
    Ipp32s last = 0;
    Ipp32s mode = 0;
    Ipp32s coefNum = 0;

    for(SBNum = 0; SBNum < 4; SBNum++)
    {
        pairsNum = pCurrMB->Block[blk_num].Pairs[SBNum];

        for(i = 0; i < pairsNum; i++)
        {
            if(i == (pairsNum - 1)) last = 1;
            mode = pCurrMB->Block[blk_num].Mode[coefNum];

            switch(mode)
            {
            case 0:
                break;
            case 1:
            case 2:
            case 3:
                WriteParams(-1,VC1_ENC_COEFFS,VM_STRING("Index = ESCAPE\n"));
                WriteParams(-1,VC1_ENC_COEFFS,VM_STRING("Index = ESCAPE Mode %d\n"), mode);
                break;
            };

            WriteParams(-1,VC1_ENC_COEFFS,VM_STRING("AC Run=%2d Level=%c%3d Last=%d\n"),
                pCurrMB->Block[blk_num].Run [coefNum],
                (pCurrMB->Block[blk_num].Level[coefNum] < 0) ? '-' : '+',
                (pCurrMB->Block[blk_num].Level[coefNum] < 0) ?
                -pCurrMB->Block[blk_num].Level[coefNum] :
            pCurrMB->Block[blk_num].Level[coefNum],
                last);
            coefNum++;
        }

        last = 0;
    }
};

void VC1EncDebug::PrintDblkInfo()
{
    if(!VTSFlag || FrameType == 0)
        PrintDblkInfoNoVTS();
    else
        PrintDblkInfoVTS();
};
void VC1EncDebug::PrintFieldDblkInfo(bool bSecond)
{
    if(!VTSFlag || FrameType == 0)
        PrintFieldDblkInfoNoVTS(bSecond);
    else
        PrintFieldDblkInfoVTS(bSecond);
};
void VC1EncDebug::PrintDblkInfoVTS()
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    //horizontal info
    pCurrMB = MBs;

    for(i = 0; i < MBHeight; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
             //horizontal info
            //block 0
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 0);

            //internal horizontal block 0
            if((pCurrMB->InUpHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 0);

            if((pCurrMB->InUpHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 0);

            //block 1
            if((pCurrMB->InHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 1);

            if((pCurrMB->InHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 1);

            //internal horizontal block 1
            if((pCurrMB->InUpHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 1);

            if((pCurrMB->InUpHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 1);

            if(i != MBHeight - 1)
            {
                //block 2
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 2);

                //internal horizontal block 2
                if((pCurrMB->InBotHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 2);

                if((pCurrMB->InBotHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 2);

                //block 3
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 3);

                //internal horizontal block 3
                if((pCurrMB->InBotHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InBotHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 3);

                //chroma
                //block 4
                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0xC) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 4);

                //internal horizontal block 4
                if((pCurrMB->InHorEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InHorEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 4);

                //block 5
                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 5);

                //internal horizontal block 5
                if((pCurrMB->InHorEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InHorEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

    //vertical info
    pCurrMB = MBs;

    for(i = 0; i < MBHeight; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            //block 0
            if((pCurrMB->InVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 0);

            //internal vertical block 0
            if((pCurrMB->InLeftVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 0);

            if((pCurrMB->InLeftVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 0);

            //block 2
            if((pCurrMB->InVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 2);

            if((pCurrMB->InVerEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 2);

            //internal vertical block 2
            if((pCurrMB->InLeftVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 2);

            if((pCurrMB->InLeftVerEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 2);


            if(j != MBWidth - 1)
            {
                //block 1
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 1);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 1);

                //internal vertical block 1
                if((pCurrMB->InRightVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 1);

                if((pCurrMB->InRightVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 1);

                //block 3
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 3);

                //internal vertical block 3
                if((pCurrMB->InRightVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InRightVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 3);

                //block 4
                if(((pCurrMB + 1)->ExVerEdgeU & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 4);

                //internal vertical block 4
                if((pCurrMB->InVerEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InVerEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 4);

                //block 5
                if(((pCurrMB + 1)->ExVerEdgeV & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 5);

                //internal vertical block 5
                if((pCurrMB->InVerEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InVerEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 5);
            }
            else
            {
                //internal vertical block 1
                if((pCurrMB->InRightVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 1);

                if((pCurrMB->InRightVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 1);

                //internal vertical block 3
                if((pCurrMB->InRightVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InRightVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 3);

                //internal vertical block 4
                if((pCurrMB->InVerEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InVerEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 4);

                //internal vertical block 5
                if((pCurrMB->InVerEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InVerEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 5);
            }

            NextMB();
        }
    }
};

void VC1EncDebug::PrintFieldDblkInfoVTS(bool bSecond)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    //horizontal info
    if(!bSecond)
        pCurrMB = MBs;
    else
        pCurrMB = MBs + MBWidth*MBHeight/2;

    for(i = 0; i < MBHeight/2; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
             //horizontal info
            //block 0
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 0);

            //internal horizontal block 0
            if((pCurrMB->InUpHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 0);

            if((pCurrMB->InUpHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 0);

            //block 1
            if((pCurrMB->InHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 1);

            if((pCurrMB->InHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 1);

            //internal horizontal block 1
            if((pCurrMB->InUpHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 1);

            if((pCurrMB->InUpHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 1);

            if(i != MBHeight/2 - 1)
            {
                //block 2
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 2);

                //internal horizontal block 2
                if((pCurrMB->InBotHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 2);

                if((pCurrMB->InBotHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 2);

                //block 3
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 3);

                //internal horizontal block 3
                if((pCurrMB->InBotHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InBotHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 3);

                //chroma
                //block 4
                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0xC) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 4);

                //internal horizontal block 4
                if((pCurrMB->InHorEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InHorEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 4);

                //block 5
                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 5);

                //internal horizontal block 5
                if((pCurrMB->InHorEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InHorEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 VTS, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

    //vertical info
    if(!bSecond)
        pCurrMB = MBs;
    else
        pCurrMB = MBs + MBWidth*MBHeight/2;

    for(i = 0; i < MBHeight/2; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            //block 0
            if((pCurrMB->InVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 0);

            //internal vertical block 0
            if((pCurrMB->InLeftVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 0);

            if((pCurrMB->InLeftVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 0);

            //block 2
            if((pCurrMB->InVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 2);

            if((pCurrMB->InVerEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 2);

            //internal vertical block 2
            if((pCurrMB->InLeftVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 2);

            if((pCurrMB->InLeftVerEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 2);


            if(j != MBWidth - 1)
            {
                //block 1
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 1);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 1);

                //internal vertical block 1
                if((pCurrMB->InRightVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 1);

                if((pCurrMB->InRightVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 1);

                //block 3
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 3);

                //internal vertical block 3
                if((pCurrMB->InRightVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InRightVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 3);

                //block 4
                if(((pCurrMB + 1)->ExVerEdgeU & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 4);

                //internal vertical block 4
                if((pCurrMB->InVerEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InVerEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 4);

                //block 5
                if(((pCurrMB + 1)->ExVerEdgeV & 0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 5);

                //internal vertical block 5
                if((pCurrMB->InVerEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InVerEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 5);
            }
            else
            {
                //internal vertical block 1
                if((pCurrMB->InRightVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 1);

                if((pCurrMB->InRightVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 1);

                //internal vertical block 3
                if((pCurrMB->InRightVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 3);

                if((pCurrMB->InRightVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 3);

                //internal vertical block 4
                if((pCurrMB->InVerEdgeU &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 4);

                if((pCurrMB->InVerEdgeU &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 4);

                //internal vertical block 5
                if((pCurrMB->InVerEdgeV &  0x3) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 VTS, blk = %d\n"), 5);

                if((pCurrMB->InVerEdgeV &  0x0C) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 VTS, blk = %d\n"), 5);
            }

            NextMB();
        }
    }
};

void VC1EncDebug::PrintDblkInfoNoVTS()
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    //horizontal info
    pCurrMB = MBs;

    for(i = 0; i < MBHeight; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
    //static FILE* f;
    //if (!f)
    //    f=fopen("print.txt","wb");

    //fprintf(f, "pCurrMB->ExHorEdgeLuma = %d\n",pCurrMB->ExHorEdgeLuma);
    //fprintf(f, "pCurrMB->InHorEdgeLuma = %d\n",pCurrMB->InHorEdgeLuma);
    //fprintf(f, "pCurrMB->InUpHorEdgeLuma = %d\n",pCurrMB->InUpHorEdgeLuma);
    //fprintf(f, "pCurrMB->InBotHorEdgeLuma = %d\n\n",pCurrMB->InBotHorEdgeLuma);

             //horizontal info
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 1);

            if((pCurrMB->InHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 1);

            if(i != MBHeight - 1)
            {
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

    //vertical info
    pCurrMB = MBs;

    for(i = 0; i < MBHeight; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
    //static FILE* f;

    //if (!f)
    //    f=fopen("print.txt","wb");

    //fprintf(f, "pCurrMB->ExVerEdgeLuma = %d\n",pCurrMB->ExVerEdgeLuma);
    //fprintf(f, "pCurrMB->InVerEdgeLuma = %d\n",pCurrMB->InVerEdgeLuma);
    //fprintf(f, "pCurrMB->InLeftVerEdgeLuma = %d\n",pCurrMB->InLeftVerEdgeLuma);
    //fprintf(f, "pCurrMB->InRightVerEdgeLuma = %d\n\n",pCurrMB->InRightVerEdgeLuma);

            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 2);

            if((pCurrMB->InVerEdgeLuma &  0xC) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 2);

            if(j != MBWidth - 1)
            {
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 1);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 1);


                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

};
void VC1EncDebug::PrintFieldDblkInfoNoVTS(bool bSecond)
{
    Ipp32s i = 0;
    Ipp32s j = 0;

    //horizontal info
    if(!bSecond)
        pCurrMB = MBs;
    else
        pCurrMB = MBs + MBWidth*MBHeight/2;

    for(i = 0; i < MBHeight/2; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
             //horizontal info
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InHorEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 0);

            if((pCurrMB->InHorEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 1);

            if((pCurrMB->InHorEdgeLuma &  0x8) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 1);

            if(i != MBHeight/2 - 1)
            {
                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 2);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 3);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeU & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 4);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + MBWidth)->ExHorEdgeV & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("hor2 = 0, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

    //vertical info
    if(!bSecond)
        pCurrMB = MBs;
    else
        pCurrMB = MBs + MBWidth*MBHeight/2;

    for(i = 0; i < MBHeight/2; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
            WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("X = %d, Y = %d\n"), j, i);

            if((pCurrMB->InVerEdgeLuma &  0x1) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x2) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 0);

            if((pCurrMB->InVerEdgeLuma &  0x4) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 2);

            if((pCurrMB->InVerEdgeLuma &  0xC) == 0)
                WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 2);

            if(j != MBWidth - 1)
            {
                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 1);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 1);


                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x4) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeLuma &  0x8) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 3);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeU & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 4);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x1) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver1 = 0, blk = %d\n"), 5);

                if(((pCurrMB + 1)->ExVerEdgeV & 0x2) == 0)
                    WriteParams(-1, VC1_ENC_DEBLK_EDGE, VM_STRING("ver2 = 0, blk = %d\n"), 5);
            }

            NextMB();
        }
    }

};
void VC1EncDebug::PrintRestoredFrame(Ipp8u* pY, Ipp32s Ystep, Ipp8u* pU, Ipp32s Ustep, Ipp8u* pV, Ipp32s Vstep, bool BFrame)
{
    if(VC1_ENC_DEBLOCKING)
    {
        static FILE* f=0;
        static FILE* f1=0;
        static Ipp32u framenum = 0;

        FILE* out = 0;
        if (!f)
        f =fopen("ENC_Deblock.txt","wb");
        if (!f1)
        f1=fopen("ENC_Deblock_B.txt","wb");

        if(!BFrame)
            out = f;
        else
            out = f1;
        assert(out!=NULL);

        Ipp32s i, j, k, t;
        Ipp8u* ptr = NULL;

        fprintf(out, "Frame %d\n",framenum);

        for(i = 0; i < MBHeight; i++)
        {
            for (j = 0; j < MBWidth; j++)
            {
                ptr = pY + i * 16 * Ystep + 16*j;

                fprintf(out, "X == %d Y == %d\n", j, i);

                fprintf(out, "LUMA\n");

                for(k = 0; k<16; k++)
                {
                    for(t = 0; t < 16; t++)
                    {
                        fprintf(out, "%d ", *(ptr + k*Ystep + t));
                    }
                    fprintf(out, "\n");
                }

                ptr = pU + i*Ustep*8 + 8*j;

                fprintf(out, "Chroma U\n");

                for(k = 0; k < 8; k++)
                {
                    for(Ipp32s t = 0; t < 8; t++)
                    {
                        fprintf(out, "%d ", *(ptr + k*Ustep + t));
                    }
                    fprintf(out, "\n");
                }

                ptr = pV + i*Vstep*8 + 8*j;

                fprintf(out, "Chroma V\n");

                for(k = 0; k < 8; k++)
                {
                    for(Ipp32s t = 0; t < 8; t++)
                    {
                        fprintf(out, "%d ", *(ptr + k*Vstep + t));
                    }
                    fprintf(out, "\n");
                }
            }
        }

        //if (!f1)
        //    f1=fopen("encRestoredFrames.yuv","wb");
        //for (i=0; i < MBHeight*16; i++)
        //{
        //    fwrite(pY+i*Ystep, 1, MBWidth*16, f1);

        //}
        //for (i=0; i < MBHeight*8; i++)
        //{
        //    fwrite(pU+i*Ustep, 1, MBWidth*8, f1);

        //}
        //for (i=0; i < MBHeight*8; i++)
        //{
        //    fwrite(pV+i*Vstep, 1, MBWidth*8, f1);

        //}
        //fflush(f1);

        fflush(f);
        framenum++;
    }
}

void VC1EncDebug::PrintBlkVTSInfo()
{
    Ipp32s blk_num = 0;

    if (pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_I_MB)
        return;

    if(pCurrMB->skip || pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_I_MB
        || pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_P_MB_INTRA
        || pCurrMB->MBType == UMC_VC1_ENCODER::VC1_ENC_B_MB_INTRA)
        return;

    for(blk_num = 0; blk_num < 6; blk_num ++)
    {
        WriteParams(-1,VC1_ENC_CBP,VM_STRING("VTS blk = %d %d\n"), blk_num, pCurrMB->Block[blk_num].VTSType);

    }
}

void VC1EncDebug::PrintPictureType()
{
    switch(FrameType)
    {
        case VC1_ENC_I_FRAME:       WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = I frame\n"));           break;
        case VC1_ENC_P_FRAME:       WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = P frame\n"));           break;
        case VC1_ENC_P_FRAME_MIXED: WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = P mixed frame\n"));     break;
        case VC1_ENC_B_FRAME:       WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = B frame\n"));           break;
        case VC1_ENC_BI_FRAME:      WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = BI frame\n"));          break;
        case VC1_ENC_SKIP_FRAME:    WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = SKIP frame\n"));        break;
        case VC1_ENC_I_I_FIELD:     WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = I_I field frame\n"));   break;
        case VC1_ENC_I_P_FIELD:     WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = I_P field frame\n"));   break;
        case VC1_ENC_P_I_FIELD:     WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = P_I field frame\n"));   break;
        case VC1_ENC_P_P_FIELD:     WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = P_P field frame\n"));   break;
        case VC1_ENC_B_B_FIELD:     WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = B_B field frame\n"));   break;
        case VC1_ENC_B_BI_FIELD:    WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = B_BI field frame\n"));  break;
        case VC1_ENC_BI_B_FIELD:    WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = BI_B field frame\n"));  break;
        case VC1_ENC_BI_BI_FIELD:   WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = BI_BI field frame\n")); break;
        default:                    WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Picture type = %d\n"), FrameType);     break;
    }

    WriteParams(-1,VC1_ENC_POSITION, VM_STRING("Frame %d\n"), FrameCount);
    //WriteParams(-1,VC1_ENC_POSITION, "Frame %d = %d\n\n", FrameCount, FrameSize);
}

void VC1EncDebug::PrintFieldInfo(bool bSecondField)
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s t = 0;
    Ipp32s blk_num = 0;

    if(bSecondField)
    {
        WriteParams(-1,VC1_ENC_POSITION, VM_STRING("\t\t\tSecond Field \n"));
        XFieldPos = 0;
        YFieldPos = 0;
    }

    for(i = 0; i < MBHeight/2; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
            WriteParams(-1,VC1_ENC_POSITION, VM_STRING("\t\t\tX: %d, Y: %d\n"), j, i);

            //------MV information----------
            PrintMVFieldInfo();

            //----CBP---------------
            //if(!(pCurrMB->skip))
            {
                WriteParams(-1,VC1_ENC_CBP,VM_STRING("Read CBPCY: 0x%02X\n"), pCurrMB->CBP);
                //PrintBlkVTSInfo();
                WriteParams(-1,VC1_ENC_CBP,VM_STRING("Predicted CBPCY = 0x%02X\n"), pCurrMB->PredCBP);
            }


            //----quantization parameters
            {
                WriteParams(-1,VC1_ENC_QUANT, VM_STRING("MB Quant = %d\n"), pCurrMB->MQuant);
                WriteParams(-1,VC1_ENC_QUANT, VM_STRING("HalfQ = %d\n"), pCurrMB->HalfQP);
            }

            for(blk_num = 0; blk_num < 6; blk_num++)
            {
                if(pCurrMB->Block[blk_num].intra)
                {
                    //---BEGIN---DC coef information--------------
                    if(pCurrMB->Block[blk_num].ACPred!= -1)
                    {
                    if(pCurrMB->Block[blk_num].DCDirection == VC1_ENC_LEFT)
                        WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("DC left prediction\n"));
                    else
                        WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("DC top prediction\n"));
                    }

                    WriteParams(-1,VC1_ENC_COEFFS, VM_STRING("DC diff = %d\n"), pCurrMB->Block[blk_num].DCDiff);

                }

                //---BEGIN---run - level coefficients--------------
                PrintRunLevel(blk_num);

                if(pCurrMB->Block[blk_num].intra)
                {
                    //---BEGIN---AC coef information--------------
                    if((pCurrMB->Block[blk_num].ACPred) && (pCurrMB->Block[blk_num].ACPred!= -1))
                    {
                        WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("Block %d\n"), blk_num);
                        WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("AC prediction\n"));

                        for(t = 0; t < 7; t++)
                            WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("%d "),pCurrMB->Block[blk_num].AC[t]);

                        WriteParams(-1,VC1_ENC_AC_PRED,VM_STRING("\n"));
                    }

                    //---BEGIN---DC coef information--------------
                    WriteParams(-1,VC1_ENC_COEFFS, VM_STRING("DC = %d\n"), pCurrMB->Block[blk_num].DC);
                }

                //----Block difference---------------
                if(pCurrMB->Block[blk_num].intra || pCurrMB->CBP)
                    PrintBlockDifference(blk_num);
            }

            //------interpolation---------------
            PrintInterpolationInfoField();

            NextMB();
        }
    }

    if(DeblkFlag)
        PrintFieldDblkInfo(bSecondField);
}

void VC1EncDebug::PrintFrameInfo()
{
    Ipp32s i = 0;
    Ipp32s j = 0;
    Ipp32s t = 0;
    Ipp32s blk_num = 0;

    for(i = 0; i < MBHeight; i++)
    {
        for(j = 0; j < MBWidth; j++)
        {
            WriteParams(-1,VC1_ENC_POSITION, VM_STRING("\t\t\tX: %d, Y: %d\n"), j, i);

            //------MV information----------
            PrintMVInfo();

            //----CBP---------------
            if(!(pCurrMB->skip))
            {
                WriteParams(-1,VC1_ENC_CBP,VM_STRING("Read CBPCY: 0x%02X\n"), pCurrMB->CBP);
                //PrintBlkVTSInfo();
                WriteParams(-1,VC1_ENC_CBP,VM_STRING("Predicted CBPCY = 0x%02X\n"), pCurrMB->PredCBP);
            }

            //----quantization parameters
            {
                WriteParams(-1,VC1_ENC_QUANT, VM_STRING("MB Quant = %d\n"), pCurrMB->MQuant);
                WriteParams(-1,VC1_ENC_QUANT, VM_STRING("HalfQ = %d\n"), pCurrMB->HalfQP);
            }

            for(blk_num = 0; blk_num < 6; blk_num++)
            {
                if(pCurrMB->Block[blk_num].intra)
                {
                    //---BEGIN---DC coef information--------------
                    if(pCurrMB->Block[blk_num].ACPred!= -1)
                    {
                    if(pCurrMB->Block[blk_num].DCDirection == VC1_ENC_LEFT)
                        WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("DC left prediction\n"));
                    else
                        WriteParams(-1,VC1_ENC_COEFFS|VC1_ENC_AC_PRED, VM_STRING("DC top prediction\n"));
                    }

                    WriteParams(-1,VC1_ENC_COEFFS, VM_STRING("DC diff = %d\n"), pCurrMB->Block[blk_num].DCDiff);

                }

                //---BEGIN---run - level coefficients--------------
                PrintRunLevel(blk_num);

                if(pCurrMB->Block[blk_num].intra)
                {
                    //---BEGIN---AC coef information--------------
                    if((pCurrMB->Block[blk_num].ACPred) && (pCurrMB->Block[blk_num].ACPred!= -1))
                    {
                        WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("Block %d\n"), blk_num);
                        WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("AC prediction\n"));

                        for(t = 0; t < 7; t++)
                            WriteParams(-1,VC1_ENC_AC_PRED, VM_STRING("%d "),pCurrMB->Block[blk_num].AC[t]);

                        WriteParams(-1,VC1_ENC_AC_PRED,VM_STRING("\n"));
                    }

                    //---BEGIN---DC coef information--------------
                    WriteParams(-1,VC1_ENC_COEFFS, VM_STRING("DC = %d\n"), pCurrMB->Block[blk_num].DC);
                }

                //----Block difference---------------
                if(pCurrMB->Block[blk_num].intra || pCurrMB->CBP)
                    PrintBlockDifference(blk_num);
            }

            //------interpolation---------------
            PrintInterpolationInfo();

            NextMB();
        }
    }

    if(DeblkFlag)
        PrintDblkInfo();
}

void VC1EncDebug::WriteFrameInfo()
{
    Ipp32s frameDebug = VC1_ENC_FRAME_DEBUG;

    if((!frameDebug) || (FrameCount >= VC1_ENC_FRAME_MIN) && (FrameCount <= VC1_ENC_FRAME_MAX))
    {
        PrintPictureType();

        pCurrMB = MBs;
        XPos = 0;
        YPos = 0;
        XFieldPos = 0;
        YFieldPos = 0;

        if(!FieldPic)
        {
            PrintFrameInfo();
        }
        else
        {
            PrintFieldInfo(false);
            PrintFieldInfo(true);
        }
    }

    memset(MBs, 0, sizeof(VC1MBDebugInfo) * MBWidth * MBHeight);
    FrameCount++;
    RefNum = 0;
    SetCurrMBFirst();
};

#endif
}
#endif //defined (UMC_ENABLE_VC1_VIDEO_ENCODER)

