/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 motion compensation
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"
#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_intens_comp_tbl.h"
#include "umc_vc1_dec_time_statistics.h"

typedef void (*ExpandIntens)(VC1Context* pContext, Frame* pFrame);

static const ExpandIntens ExpandIntens_table[] =
             {
               (ExpandIntens)(ExpandFrame_Adv),
               (ExpandIntens)(ExpandFrame_Interlace),
               (ExpandIntens)(ExpandFrame_Interlace)
             };

VC1Status FillTablesForIntensityCompensation(VC1Context* pContext,
                                             Ipp32u scale,
                                             Ipp32u shift)
{
    /*scale, shift parameters are in [0,63]*/
    Ipp32s i;
    Ipp32s iscale = (scale)? scale+32 : -64;
    Ipp32s ishift = (scale)? shift*64 : (255-2*shift)*64;
    Ipp32s z      = (scale)? -1:2;
    Ipp32s j ;


    ishift += (shift>31)? z<<12: 0;

    for (i=0;i<256;i++)
    {
        j = (i*iscale+ishift+32)>>6;
        pContext->LumaTable[0][i] = (Ipp8u)VC1_CLIP(j);
        pContext->LumaTable[1][i] = (Ipp8u)VC1_CLIP(j);
        j = ((i-128)*iscale+128*64+32)>>6;
        pContext->ChromaTable[0][i] = (Ipp8u)VC1_CLIP(j);
        pContext->ChromaTable[1][i] = (Ipp8u)VC1_CLIP(j);

    }

    return VC1_OK;
}

VC1Status FillTablesForIntensityCompensation_Adv(VC1Context* pContext,
                                                 Ipp32u scale,
                                                 Ipp32u shift,
                                                 Ipp32u bottom_field,
                                                 Ipp32s index)
{
    /*scale, shift parameters are in [0,63]*/
    Ipp32s i;
    Ipp32s iscale = (scale)? scale+32 : -64;
    Ipp32s ishift = (scale)? shift*64 : (255-2*shift)*64;
    Ipp32s z      = (scale)? -1:2;
    Ipp32s j ;
    Ipp8u *pY, *pU, *pV;
    IppiSize roiSize;

    roiSize.width = (pContext->m_seqLayerHeader->MAX_CODED_WIDTH+1) << 1;
    roiSize.height = (pContext->m_seqLayerHeader->MAX_CODED_HEIGHT+1) << 1;

    Ipp32s YPitch =  pContext->m_frmBuff.m_pFrames[index].m_iYPitch;
    Ipp32s UPitch = pContext->m_frmBuff.m_pFrames[index].m_iUPitch;
    Ipp32s VPitch = pContext->m_frmBuff.m_pFrames[index].m_iVPitch;

    pY = pContext->m_frmBuff.m_pFrames[index].m_pY;
    pU = pContext->m_frmBuff.m_pFrames[index].m_pU;
    pV = pContext->m_frmBuff.m_pFrames[index].m_pV;

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        if (bottom_field)
        {
            pY += YPitch;
            pU += UPitch;
            pV += VPitch;
        }
        YPitch <<= 1;
        UPitch <<= 1;
        VPitch <<= 1;

        roiSize.height >>= 1;
    }

    ishift += (shift>31)? z*64*64: 0;
    Ipp32u LUTindex = bottom_field + (pContext->m_picLayerHeader->CurrField << 1);

    if (pContext->m_picLayerHeader->FCM == VC1_FieldInterlace)
    {
        for (i=0;i<256;i++)
        {
            j = (i*iscale+ishift+32)>>6;
            pContext->LumaTable[LUTindex][i] = (Ipp8u)VC1_CLIP(j);
            j = ((i-128)*iscale+128*64+32)>>6;
            pContext->ChromaTable[LUTindex][i] = (Ipp8u)VC1_CLIP(j);
        }
    }
    else
    {
        for (i=0;i<256;i++)
        {
            j = (i*iscale+ishift+32)>>6;
            pContext->LumaTable[0][i] = (Ipp8u)VC1_CLIP(j);
            pContext->LumaTable[1][i] = (Ipp8u)VC1_CLIP(j);
            j = ((i-128)*iscale+128*64+32)>>6;
            pContext->ChromaTable[0][i] = (Ipp8u)VC1_CLIP(j);
            pContext->ChromaTable[1][i] = (Ipp8u)VC1_CLIP(j);
        }
    }
    return VC1_OK;
}

void SZTables(VC1Context* pContext)
{
    Ipp32s CurrIndex = pContext->m_frmBuff.m_iCurrIndex;

    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[0] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[1] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTableCurr[0] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTableCurr[1] = 0;

    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[0] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[1] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTableCurr[0] = 0;
    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTableCurr[1] = 0;
}
void CreateComplexICTablesForFields(VC1Context* pContext)
{
    Ipp32s PrevIndex = pContext->m_frmBuff.m_iPrevIndex;
    Ipp32s CurrIndex = pContext->m_frmBuff.m_iCurrIndex;
    Ipp32s i;
    {
        //if (pContext->m_picLayerHeader->TFF)
        {
            if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 4) // top of first
            {
                pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[0] = pContext->LumaTable[0];
                pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[0] = pContext->ChromaTable[0];
            }

            if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 8) // bottom of first
            {
                pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[1] = pContext->LumaTable[1];
                pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[1] = pContext->ChromaTable[1];
            }

            if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 1) // top of second
            {
                pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTableCurr[0] = pContext->LumaTable[2];
                pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTableCurr[0] = pContext->ChromaTable[2];
            }
            if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 2) // bottom of second
            {

                //if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 8) // bottom of first
                //{
                //    for (i = 0; i < 256; i++)
                //    {
                //        pContext->LumaTable[3][i] = pContext->LumaTable[3][pContext->LumaTable[1][i]];
                //        pContext->ChromaTable[3][i] = pContext->ChromaTable[3][pContext->ChromaTable[1][i]];
                //    }
                //}

            }


                            ////need to twice IC
                //if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 4)
                //{
                //    for (i = 0; i < 256; i++)
                //    {
                //        pContext->LumaTable[3][i] = pContext->LumaTable[3][pContext->LumaTable[1][i]];
                //        pContext->ChromaTable[3][i] = pContext->ChromaTable[3][pContext->ChromaTable[1][i]];
                //    }
                //}
        }
        // else
    }

    if (pContext->m_frmBuff.m_pFrames[PrevIndex].FCM == VC1_FieldInterlace)
    {
        Ipp32s j;
        for (j = 0; j < 2; j++)
        {
            if (pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j])
            {
                if(pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j])
                {
                    //need to twice IC
                    Ipp8u temp_luma[256];
                    Ipp8u temp_chroma[256];
                    for (i = 0; i < 256; i++)
                    {
                        temp_luma[i] = pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j][pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j][i]];
                        temp_chroma[i] = pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j][pContext->m_frmBuff.m_pFrames[PrevIndex].ChromaTableCurr[j][i]];
                    }

                    //pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j] = pContext->LumaTable[1];
                    //pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j] = pContext->ChromaTable[1];


                    for (i = 0; i < 256; i++)
                    {
                        pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j][i] = temp_luma[i];
                        pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j][i] = temp_chroma[i];
                    }
                }
            }
            else if(pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j])
            {
                pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j] = pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j];
                pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j] = pContext->m_frmBuff.m_pFrames[PrevIndex].ChromaTableCurr[j];
                //pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j] = pContext->LumaTable[j];
                //pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j] = pContext->ChromaTable[j];
                //for (i = 0; i < 256; i++)
                //{
                //    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j][i] = pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j][i];
                //    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j][i] = pContext->m_frmBuff.m_pFrames[PrevIndex].ChromaTableCurr[j][i];
                //}
            }
        }
    }
    else
    {
    }
}
void CreateComplexICTablesForFrame(VC1Context* pContext)
{
    Ipp32s PrevIndex = pContext->m_frmBuff.m_iPrevIndex;
    Ipp32s i;
    {
        Ipp32s CurrIndex = pContext->m_frmBuff.m_iCurrIndex;

        if (pContext->m_bIntensityCompensation)
        {
            pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[0] = pContext->LumaTable[0];
            pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[1] = pContext->LumaTable[0];

            pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[0] = pContext->ChromaTable[0];
            pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[1] = pContext->ChromaTable[0];

            pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTableCurr[0] = 0;
            pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTableCurr[1] = 0;

            pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTableCurr[0] = 0;
            pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTableCurr[1] = 0;
        }

        if (pContext->m_frmBuff.m_pFrames[PrevIndex].FCM == VC1_FieldInterlace)
        {
            Ipp32s j;
            Ipp32s ICFieldNum = -1;
            for (j = 0; j < 2; j++)
            {
                if (pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j])
                {
                    //need to twice IC
                    if(pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j])
                    {
                        ICFieldNum = j;
                        Ipp8u temp_luma[256];
                        Ipp8u temp_chroma[256];
                        for (i = 0; i < 256; i++)
                        {
                            temp_luma[i] = pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j][pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j][i]];
                            temp_chroma[i] = pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j][pContext->m_frmBuff.m_pFrames[PrevIndex].ChromaTableCurr[j][i]];
                        }

                        pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j] = pContext->LumaTable[1];
                        pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j] = pContext->ChromaTable[1];


                        for (i = 0; i < 256; i++)
                        {
                            pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j][i] = temp_luma[i];
                            pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j][i] = temp_chroma[i];
                        }
                    }
                }
                else
                {
                    pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[j] = pContext->m_frmBuff.m_pFrames[PrevIndex].LumaTableCurr[j];
                    pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[j] = pContext->m_frmBuff.m_pFrames[PrevIndex].ChromaTableCurr[j];
                }
            }
        }
    }
}
void UpdateICTablesForSecondField(VC1Context* pContext)
{
    Ipp32s CurrIndex = pContext->m_frmBuff.m_iCurrIndex;
    Ipp32s i;

    if ((pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 2)&&
        (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 8))
    {
        Ipp8u temp_luma[256];
        Ipp8u temp_chroma[256];

        for (i = 0; i < 256; i++)
        {
            temp_luma[i] = pContext->LumaTable[3][pContext->LumaTable[1][i]];
            temp_chroma[i] = pContext->ChromaTable[3][pContext->ChromaTable[1][i]];
        }
        for (i = 0; i < 256; i++)
        {
            pContext->LumaTable[1][i] = temp_luma[i];
            pContext->ChromaTable[1][i] = temp_chroma[i];
        }
    }
    else if (pContext->m_frmBuff.m_pFrames[CurrIndex].ICFieldMask & 2)
    {
        pContext->m_frmBuff.m_pFrames[CurrIndex].LumaTablePrev[1] = pContext->LumaTable[3];
        pContext->m_frmBuff.m_pFrames[CurrIndex].ChromaTablePrev[1] = pContext->ChromaTable[3];

    }



}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
