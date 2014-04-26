/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include <stdio.h>
#include <stdarg.h>
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_dec_thread.h"



const Ipp32u  VC1_POSITION  = 0x00000001; // MB, Block positions, skip info
const Ipp32u  VC1_CBP       = 0x00000002; // coded block patern info
const Ipp32u  VC1_BITBLANES = 0x00000004; // bitplane information
const Ipp32u  VC1_QUANT     = 0x00000008; // transform types decoded info
const Ipp32u  VC1_TT        = 0x00000010; // transform types decoded info
const Ipp32u  VC1_MV        = 0x00000020; // motion vectors info
const Ipp32u  VC1_PRED      = 0x00000040; // predicted blocks
const Ipp32u  VC1_COEFFS    = 0x00000080; // DC, AC coefficiens
const Ipp32u  VC1_RESPEL    = 0x00000100; // pixels befor filtering
const Ipp32u  VC1_SMOOTHINT = 0x00000200; // smoothing
const Ipp32u  VC1_BFRAMES   = 0x00000400; // B frames log
const Ipp32u  VC1_INTENS    = 0x00000800; // intesity compensation tables
const Ipp32u  VC1_MV_BBL    = 0x00001000; // deblocking
const Ipp32u  VC1_MV_FIELD  = 0x00002000; // motion vectors info for field pic
const Ipp32u  VC1_TABLES    = 0x00004000; //VLC tables

const Ipp32u  VC1_DEBUG       = 0x00000000;//0x1DBF;//0x00000208; current debug output
const Ipp32u  VC1_FRAME_DEBUG = 0; //on/off frame debug
const Ipp32u  VC1_FRAME_MIN   = 5; //first frame to debug
const Ipp32u  VC1_FRAME_MAX   = 15; //last frame to debug

#if defined(_MSC_VER)
#pragma warning( disable : 4100 ) // disable debug, empty function
#endif
void VM_Debug::vm_debug_frame(Ipp32s _cur_frame, Ipp32s level,  const vm_char *format,...) {}


void VM_Debug::_print_macroblocks(VC1Context* pContext)
{
    Ipp32s i,j;
    Ipp8u* pYPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pY;
    Ipp8u* pUPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pU;
    Ipp8u* pVPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pV;

    Ipp32s YPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iYPitch;
    Ipp32s UPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iUPitch;
    Ipp32s VPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iVPitch;
    Ipp32u LeftTopRightPositionFlag = pContext->m_pCurrMB->LeftTopRightPositionFlag;

    Ipp32s currYoffset;
    Ipp32s currUoffset;
    Ipp32s currVoffset;

    Ipp32u currMBYpos = pContext->m_pSingleMB->m_currMBYpos;
    Ipp32u currMBXpos =pContext->m_pSingleMB->m_currMBXpos;


    Ipp32s fieldYPitch;
    Ipp32s fieldUPitch;
    Ipp32s fieldVPitch;

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        fieldYPitch = 2*YPitch;
        fieldUPitch = 2*UPitch;
        fieldVPitch = 2*VPitch;
    } else
    {
        fieldYPitch = YPitch;
        fieldUPitch = UPitch;
        fieldVPitch = VPitch;
    }

    if (pContext->m_picLayerHeader->CurrField)
        currMBYpos -= pContext->m_seqLayerHeader->heightMB/2;

    vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("maroblock-%d,%d\n"),pContext->m_pSingleMB->m_currMBXpos,pContext->m_pSingleMB->m_currMBYpos);
    if (currMBXpos>0&&currMBYpos>0)
    {
        currYoffset = fieldYPitch*(currMBYpos-1)*VC1_PIXEL_IN_LUMA +
            (currMBXpos-1)*VC1_PIXEL_IN_LUMA;
        currUoffset = fieldUPitch*(currMBYpos-1)*VC1_PIXEL_IN_CHROMA +
            (currMBXpos-1)*VC1_PIXEL_IN_CHROMA;
        currVoffset = fieldVPitch*(currMBYpos-1)*VC1_PIXEL_IN_CHROMA +
            (currMBXpos-1)*VC1_PIXEL_IN_CHROMA;

        if ((pContext->m_picLayerHeader->BottomField)&&(pContext->m_picLayerHeader->FCM == VC1_FieldInterlace))
        {
        currYoffset += YPitch;
        currUoffset += UPitch;
        currVoffset += VPitch;
        }

        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("left upper maroblock\n luma\n"));
        for (i=0;i<VC1_PIXEL_IN_LUMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_LUMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pYPlane[currYoffset+fieldYPitch*i+j]);

            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("U\n"));
        for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pUPlane[currUoffset+fieldUPitch*i+j]);

           vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("V\n"));
        for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pVPlane[currVoffset+fieldVPitch*i+j]);

            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
    }

    if (VC1_IS_NO_LEFT_MB(LeftTopRightPositionFlag))
    {
        currYoffset = fieldYPitch*(currMBYpos)*VC1_PIXEL_IN_LUMA +
            (currMBXpos-1)*VC1_PIXEL_IN_LUMA;
        currUoffset = fieldUPitch*(currMBYpos)*VC1_PIXEL_IN_CHROMA +
            (currMBXpos-1)*VC1_PIXEL_IN_CHROMA;
        currVoffset = fieldVPitch*(currMBYpos)*VC1_PIXEL_IN_CHROMA +
            (currMBXpos-1)*VC1_PIXEL_IN_CHROMA;

        if ((pContext->m_picLayerHeader->BottomField)&&(pContext->m_picLayerHeader->FCM == VC1_FieldInterlace))
        {
        currYoffset += YPitch;
        currUoffset += UPitch;
        currVoffset += VPitch;
        }

        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("left  maroblock\n luma\n"));
        for (i=0;i<VC1_PIXEL_IN_LUMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_LUMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pYPlane[currYoffset+fieldYPitch*i+j]);

            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("U\n"));
        for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pUPlane[currUoffset+fieldUPitch*i+j]);

            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("V\n"));
        for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
        {
            for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
                vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pVPlane[currVoffset+fieldVPitch*i+j]);

            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
        }
    }


    currYoffset = fieldYPitch*(currMBYpos)*VC1_PIXEL_IN_LUMA +
        (currMBXpos)*VC1_PIXEL_IN_LUMA;
    currUoffset = fieldUPitch*(currMBYpos)*VC1_PIXEL_IN_CHROMA +
        (currMBXpos)*VC1_PIXEL_IN_CHROMA;
    currVoffset = fieldVPitch*(currMBYpos)*VC1_PIXEL_IN_CHROMA +
        (currMBXpos)*VC1_PIXEL_IN_CHROMA;

    if ((pContext->m_picLayerHeader->BottomField)&&(pContext->m_picLayerHeader->FCM == VC1_FieldInterlace))
    {
        currYoffset += YPitch;
        currUoffset += UPitch;
        currVoffset += VPitch;
    }

    vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("current maroblock\n luma\n"));
    for (i=0;i<VC1_PIXEL_IN_LUMA;i++)
    {
        for (j=0;j<VC1_PIXEL_IN_LUMA;j++)
            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pYPlane[currYoffset+fieldYPitch*i+j]);

        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
    }
    vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("U\n"));
    for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
    {
        for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pUPlane[currUoffset+fieldUPitch*i+j]);

        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
    }
    vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("V\n"));
    for (i=0;i<VC1_PIXEL_IN_CHROMA;i++)
    {
        for (j=0;j<VC1_PIXEL_IN_CHROMA;j++)
            vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("%3d "),pVPlane[currVoffset+fieldVPitch*i+j]);

        vm_debug_frame(-1,VC1_SMOOTHINT,VM_STRING("\n "));
    }
}

void VM_Debug::_print_blocks(VC1Context* pContext)
{
    Ipp32s blk_num;

    Ipp8u* pYPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pY;
    Ipp8u* pUPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pU;
    Ipp8u* pVPlane = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pV;

    Ipp32s YPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iYPitch;
    Ipp32s UPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iUPitch;
    Ipp32s VPitch = pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_iVPitch;

    Ipp32s currYoffset;
    Ipp32s currUoffset;
    Ipp32s currVoffset;

    Ipp32u currMBYpos = pContext->m_pSingleMB->m_currMBYpos;
    Ipp32u currMBXpos = pContext->m_pSingleMB->m_currMBXpos;

    Ipp32s fieldYPitch;
    Ipp32s fieldUPitch;
    Ipp32s fieldVPitch;

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        fieldYPitch = 2*YPitch;
        fieldUPitch = 2*UPitch;
        fieldVPitch = 2*VPitch;
    } else
    {
        fieldYPitch = YPitch;
        fieldUPitch = UPitch;
        fieldVPitch = VPitch;
    }


    if (pContext->m_picLayerHeader->CurrField)
        currMBYpos -= pContext->m_seqLayerHeader->heightMB/2;

    for(blk_num = 0; blk_num < 6; blk_num++)
    {
        Ipp32s i,j;

        currYoffset = fieldYPitch*currMBYpos*VC1_PIXEL_IN_LUMA + currMBXpos*VC1_PIXEL_IN_LUMA;
        currUoffset = fieldUPitch*currMBYpos*VC1_PIXEL_IN_CHROMA + currMBXpos*VC1_PIXEL_IN_CHROMA;
        currVoffset = fieldVPitch*currMBYpos*VC1_PIXEL_IN_CHROMA + currMBXpos*VC1_PIXEL_IN_CHROMA;

        if ((pContext->m_picLayerHeader->BottomField)&&(pContext->m_picLayerHeader->FCM == VC1_FieldInterlace))
        {
        currYoffset += YPitch;
        currUoffset += UPitch;
        currVoffset += VPitch;
        }

        //MB predictiction if any and futher reconstruction

        vm_debug_frame(-1,VC1_RESPEL,VM_STRING("Result block:\n"));
        if(blk_num < 4)
        {
            if(blk_num < 2)
            {
                currYoffset += 8*blk_num;
            }
            else
            {
                currYoffset += 8*fieldYPitch+ 8*(blk_num-2);
            }

            for(i = 0; i < VC1_PIXEL_IN_BLOCK; i++)
            {
                for(j = 0; j < VC1_PIXEL_IN_BLOCK; j++)
                {
                    vm_debug_frame(-1,VC1_RESPEL,VM_STRING("%d "), pYPlane[currYoffset + i*fieldYPitch + j]);
                }
                vm_debug_frame(-1,VC1_RESPEL,VM_STRING("\n "));
            }
        }
        else if(blk_num == 4)
        {
            for(i = 0; i < VC1_PIXEL_IN_BLOCK; i++)
            {
                for(j = 0; j < VC1_PIXEL_IN_BLOCK; j++)
                {
                    vm_debug_frame(-1,VC1_RESPEL,VM_STRING("%d "), pUPlane[currUoffset + i*fieldUPitch + j]);
                }
                vm_debug_frame(-1,VC1_RESPEL,VM_STRING("\n "));
            }
        }
        else
        {
            for(i = 0; i < VC1_PIXEL_IN_BLOCK; i++)
            {
                for(j = 0; j < VC1_PIXEL_IN_BLOCK; j++)
                {
                    vm_debug_frame(-1,VC1_RESPEL,VM_STRING("%d "), pVPlane[currVoffset + i*fieldVPitch + j]);
                }
                vm_debug_frame(-1,VC1_RESPEL,VM_STRING("\n "));
            }
        }
    }
}
void VM_Debug::print_bitplane(VC1Bitplane* pBitplane, Ipp32s width, Ipp32s height)
{
    Ipp32s i,j;
    if(pBitplane->m_imode != VC1_BITPLANE_RAW_MODE)
    {
        for(i = 0; i < height; i++)
        {
            for(j = 0; j < width; j++)
            {
                vm_debug_frame(-1,VC1_BITBLANES, VM_STRING("%d\t"), pBitplane->m_databits[i*width + j]);
            }
            vm_debug_frame(-1,VC1_BITBLANES, VM_STRING("\n"));
        }
    }
}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
