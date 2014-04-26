/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1)Smoothing main/simple profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include <memory.h>

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_common_defs.h"

#include "ipps.h"


void Smoothing_I(VC1Context* pContext, Ipp32s Height)
{
    VC1MB* pCurrMB = pContext->m_pCurrMB;

    if(pCurrMB->Overlap == 0)
        return;

    {
        VC1MB* pCurrMB = pContext->m_pCurrMB;
        Ipp32s notTop = VC1_IS_NO_TOP_MB(pCurrMB->LeftTopRightPositionFlag);
        Ipp32s Width = pContext->m_seqLayerHeader->widthMB;
        Ipp32u EdgeDisabledFlag = IPPVC_EDGE_HALF_1 | IPPVC_EDGE_HALF_2;

        Ipp16s* CurrBlock = pContext->m_pBlock;
        Ipp8u* YPlane = pCurrMB->currYPlane;
        Ipp8u* UPlane = pCurrMB->currUPlane;
        Ipp8u* VPlane = pCurrMB->currVPlane;
        Ipp32s YPitch = pCurrMB->currYPitch;
        Ipp32s UPitch = pCurrMB->currUPitch;
        Ipp32s VPitch = pCurrMB->currVPitch;

        Ipp32s i, j;
        //Ipp16s* UpYrow;
        //Ipp16s* UpUrow;
        //Ipp16s* UpVrow;

       for (j = 0; j< Height; j++)
       {
           notTop = VC1_IS_NO_TOP_MB(pCurrMB->LeftTopRightPositionFlag);

           if(notTop)
           {
                YPlane = pCurrMB->currYPlane;
                UPlane = pCurrMB->currUPlane;
                VPlane = pCurrMB->currVPlane;

                //first MB in row
                //internal vertical smoothing
                ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6, VC1_PIXEL_IN_LUMA*2,
                                                             CurrBlock + 8, VC1_PIXEL_IN_LUMA*2,
                                                             YPlane + 8,    YPitch,
                                                             0,             EdgeDisabledFlag);
                for (i = 1; i < Width; i++)
                {
                    CurrBlock  += 8*8*6;
                    pCurrMB++;
                    YPlane = pCurrMB->currYPlane;
                    UPlane = pCurrMB->currUPlane;
                    VPlane = pCurrMB->currVPlane;

                    //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[i-1];
                    //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[i-1];
                    //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[i-1];

                    //LUMA
                    //left boundary vertical smoothing
                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*6+14, VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock,            VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane,               YPitch,
                                                                 0,                    EdgeDisabledFlag);
                    //internal vertical smoothing
                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6,        VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock + 8,        VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane + 8,           YPitch,
                                                                 0,                    EdgeDisabledFlag);


                    //left MB Upper horizontal edge
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*2 +6*VC1_PIXEL_IN_LUMA,              VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock - 8*8*6,   VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane - 16,         YPitch,
                                                                 EdgeDisabledFlag);

                    //left MB internal horizontal edge
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - 8*8*4 - 32, VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock - 8*8*4,      VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane - 16 + 8*YPitch, YPitch,
                                                                 EdgeDisabledFlag);

                    //CHROMA
                    //U vertical smoothing
                    ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*2 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock + 8*8*4,     VC1_PIXEL_IN_CHROMA*2,
                                                                   UPlane,                UPitch);

                    //V vertical smoothing
                    ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock + 8*8*5,   VC1_PIXEL_IN_CHROMA*2,
                                                                   VPlane,              VPitch);

                    //U top horizontal smoothing
                    ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*4 +6*VC1_PIXEL_IN_CHROMA,           VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock - 2*64, VC1_PIXEL_IN_CHROMA*2,
                                                                   UPlane - 8,       UPitch);

                    //V top horizontal smoothing
                    ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*5 +6*VC1_PIXEL_IN_CHROMA,         VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock - 64, VC1_PIXEL_IN_CHROMA*2,
                                                                   VPlane - 8,     VPitch);
                   
                }

                //RIGHT MB

                //LUMA
                //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[Width - 1];
                //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[Width - 1];
                //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[Width - 1];

                //MB Upper horizontal edge
                ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*2 +6*VC1_PIXEL_IN_LUMA,     VC1_PIXEL_IN_LUMA*2,
                                                             CurrBlock,  VC1_PIXEL_IN_LUMA*2,
                                                             YPlane,     YPitch,
                                                             EdgeDisabledFlag);

                //MB internal horizontal edge
                ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock + 8*8*2 - 32, VC1_PIXEL_IN_LUMA*2,
                                                             CurrBlock + 8*8*2,      VC1_PIXEL_IN_LUMA*2,
                                                             YPlane + 8*YPitch, YPitch,
                                                             EdgeDisabledFlag);

                //U top horizontal smoothing
                ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*4 +6*VC1_PIXEL_IN_CHROMA,            VC1_PIXEL_IN_CHROMA*2,
                                                               CurrBlock + 4*64,  VC1_PIXEL_IN_CHROMA*2,
                                                               UPlane,            UPitch);

                //V top horizontal smoothing
                ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*5 +6*VC1_PIXEL_IN_CHROMA,           VC1_PIXEL_IN_CHROMA*2,
                                                               CurrBlock + 64*5, VC1_PIXEL_IN_CHROMA*2,
                                                               VPlane,           VPitch);

                CurrBlock  += 8*8*6;
                pCurrMB++;
        }
        else
        {
                YPlane = pCurrMB->currYPlane;
                UPlane = pCurrMB->currUPlane;
                VPlane = pCurrMB->currVPlane;

                //first MB in row
                //internal vertical smoothing
                ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6,  VC1_PIXEL_IN_LUMA*2,
                                                             CurrBlock + 8,  VC1_PIXEL_IN_LUMA*2,
                                                             YPlane + 8,     YPitch,
                                                             0,              EdgeDisabledFlag);

                for (i = 1; i < Width; i++)
                {
                    CurrBlock  += 8*8*6;
                    pCurrMB++;

                    YPlane = pCurrMB->currYPlane;
                    UPlane = pCurrMB->currUPlane;
                    VPlane = pCurrMB->currVPlane;

                    //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[i-1];
                    //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[i-1];
                    //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[i-1];

                    //LUMA
                    //left boundary vertical smoothing
                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*6 + 14,  VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock,               VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane,                  YPitch,
                                                                 0,                       EdgeDisabledFlag);
                    //internal vertical smoothing
                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6,  VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock + 8,  VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane + 8,     YPitch,
                                                                 0,              EdgeDisabledFlag);

                    //left MB internal horizontal edge
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - 8*8*4 - 32,   VC1_PIXEL_IN_LUMA*2,
                                                                 CurrBlock - 8*8*4,        VC1_PIXEL_IN_LUMA*2,
                                                                 YPlane - 16 + 8*YPitch,   YPitch,
                                                                 EdgeDisabledFlag);

                    //CHROMA
                    //U vertical smoothing
                    ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*2 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock + 8*8*4,     VC1_PIXEL_IN_CHROMA*2,
                                                                   UPlane,                UPitch);

                    //V vertical smoothing
                    ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                   CurrBlock +  8*8*5,  VC1_PIXEL_IN_CHROMA*2,
                                                                    VPlane,             VPitch);

                    //copy last two srings of Left macroblock to SmoothUpperRows
                    //ippsCopy_16s(CurrBlock - 64*2 - 32, pContext->SmoothingInfo->SmoothUpperYRows[i - 1], 32);
                    //ippsCopy_16s(CurrBlock - 64 - 16, pContext->SmoothingInfo->SmoothUpperURows[i - 1], 16);
                    //ippsCopy_16s(CurrBlock - 16,  pContext->SmoothingInfo->SmoothUpperVRows[i -1], 16);
                }

                //RIGHT MB
                //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[Width - 1];
                //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[Width - 1];
                //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[Width - 1];

                //MB internal horizontal edge
                ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock + 8*8*2 - 32,  VC1_PIXEL_IN_LUMA*2,
                                                             CurrBlock + 8*8*2,       VC1_PIXEL_IN_LUMA*2,
                                                             YPlane + 8*YPitch,       YPitch,
                                                             EdgeDisabledFlag);

                CurrBlock  += 8*8*6;
                pCurrMB++;
            }
        }
    }
}

void Smoothing_P(VC1Context* pContext, Ipp32s Height)
{
    if(pContext->m_seqLayerHeader->OVERLAP == 0)
        return;

    {
        VC1MB* pCurrMB = pContext->m_pCurrMB;
        Ipp32s notTop = VC1_IS_NO_TOP_MB(pCurrMB->LeftTopRightPositionFlag);
        Ipp32s Width = pContext->m_seqLayerHeader->widthMB;
        Ipp32u EdgeDisabledFlag;
        Ipp16s* CurrBlock = pContext->m_pBlock;
        Ipp8u* YPlane = pCurrMB->currYPlane;
        Ipp8u* UPlane = pCurrMB->currUPlane;
        Ipp8u* VPlane = pCurrMB->currVPlane;

        Ipp32s YPitch = pCurrMB->currYPitch;
        Ipp32s UPitch = pCurrMB->currUPitch;
        Ipp32s VPitch = pCurrMB->currVPitch;
        Ipp32s LeftIntra;
        Ipp32s TopLeftIntra;
        Ipp32s TopIntra;
        Ipp32s CurrIntra = pCurrMB->IntraFlag*pCurrMB->Overlap;

        Ipp32s i, j;
        //Ipp16s* UpYrow;
        //Ipp16s* UpUrow;
        //Ipp16s* UpVrow;

        for (j = 0; j< Height; j++)
        {
            notTop = VC1_IS_NO_TOP_MB(pCurrMB->LeftTopRightPositionFlag);

            if(notTop)
            {
                YPlane = pCurrMB->currYPlane;
                UPlane = pCurrMB->currUPlane;
                VPlane = pCurrMB->currVPlane;

                CurrIntra    = (pCurrMB)->IntraFlag*pCurrMB->Overlap;
                TopIntra     = (pCurrMB - Width)->IntraFlag*(pCurrMB - Width)->Overlap;

                if(CurrIntra)
                {
                    //first block in row
                    //internal vertical smoothing
                    EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_1_INTRA) * (IPPVC_EDGE_HALF_1))
                                    |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_2_3_INTRA) * (IPPVC_EDGE_HALF_2));

                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6, VC1_PIXEL_IN_LUMA*2,
                                                                CurrBlock + 8, VC1_PIXEL_IN_LUMA*2,
                                                                YPlane    + 8, YPitch,
                                                                0,             EdgeDisabledFlag);
                }

                for (i = 1; i < Width; i++)
                {

                    CurrBlock  += 8*8*6;
                    pCurrMB++;

                    YPlane = pCurrMB->currYPlane;
                    UPlane = pCurrMB->currUPlane;
                    VPlane = pCurrMB->currVPlane;


                    //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[i-1];
                    //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[i-1];
                    //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[i-1];

                    CurrIntra    = (pCurrMB)->IntraFlag*pCurrMB->Overlap;
                    LeftIntra    = (pCurrMB - 1)->IntraFlag*(pCurrMB - 1)->Overlap;
                    TopLeftIntra = (pCurrMB - Width - 1)->IntraFlag*(pCurrMB - Width - 1)->Overlap;

                    if(CurrIntra)
                    {

                        //////////////////////////////////
                        //LUMA left boundary vertical smoothing
                        EdgeDisabledFlag =(((VC1_EDGE_MB(CurrIntra,VC1_BLOCK_0_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_1_INTRA)))
                            *(IPPVC_EDGE_HALF_1))|
                            (((VC1_EDGE_MB(CurrIntra,VC1_BLOCK_2_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_3_INTRA)))
                            *(IPPVC_EDGE_HALF_2));

                        ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*6+14, VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock,            VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane,               YPitch,
                                                                    0,                    EdgeDisabledFlag);
                        //////////////////////////////
                        //internal vertical smoothing
                        EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_1_INTRA) * (IPPVC_EDGE_HALF_1))
                                        |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_2_3_INTRA) * (IPPVC_EDGE_HALF_2));

                        ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6,        VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock + 8,        VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane+8,             YPitch,
                                                                    0,                    EdgeDisabledFlag);
                    }



                    if(LeftIntra)
                    {
                        ////////////////////////////////
                        //left MB Upper horizontal edge
                        EdgeDisabledFlag =(((VC1_EDGE_MB(TopLeftIntra,VC1_BLOCK_2_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_0_INTRA)))
                            *(IPPVC_EDGE_HALF_1))|
                            (((VC1_EDGE_MB(TopLeftIntra,VC1_BLOCK_3_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_1_INTRA)))
                            *(IPPVC_EDGE_HALF_2));
                        ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*2 +6*VC1_PIXEL_IN_LUMA,              VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock - 8*8*6,   VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane - 16,         YPitch,
                                                                    EdgeDisabledFlag);
                        //////////////////////////////////
                        //left MB internal horizontal edge
                        EdgeDisabledFlag = (VC1_EDGE_MB(LeftIntra,VC1_BLOCKS_0_2_INTRA) * (IPPVC_EDGE_HALF_1))
                                        |(VC1_EDGE_MB(LeftIntra,VC1_BLOCKS_1_3_INTRA) * (IPPVC_EDGE_HALF_2));
                        ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - 8*8*4 - 32, VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock - 8*8*4,      VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane - 16 + 8*YPitch, YPitch,
                                                                    EdgeDisabledFlag);
                         if(((LeftIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA) && ((CurrIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA))
                        {
                            ///////////////////////
                            //U vertical smoothing
                            ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*2 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock + 8*8*4,     VC1_PIXEL_IN_CHROMA*2,
                                                                        UPlane,                UPitch);

                            ///////////////////////
                            //V vertical smoothing
                            ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock + 8*8*5,   VC1_PIXEL_IN_CHROMA*2,
                                                                        VPlane,              VPitch);
                        }

                        if(((TopLeftIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA) && ((LeftIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA))
                        {
                            /////////////////////////////
                            //U top horizontal smoothing
                            ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*4 +6*VC1_PIXEL_IN_CHROMA,           VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock - 2*64, VC1_PIXEL_IN_CHROMA*2,
                                                                        UPlane - 8,       UPitch);

                            ////////////////////////////
                            //V top horizontal smoothing
                            ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - (Width+1)*8*8*6 + 64*5 +6*VC1_PIXEL_IN_CHROMA,         VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock - 64, VC1_PIXEL_IN_CHROMA*2,
                                                                        VPlane - 8,     VPitch);
                        }                       
                    }
                }

                //RIGHT MB
                //LUMA
                //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[Width - 1];
                //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[Width - 1];
                //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[Width - 1];

                TopIntra     = (pCurrMB - Width)->IntraFlag*(pCurrMB - Width)->Overlap;

                if(CurrIntra)
                {
                    ///////////////////////////
                    //MB Upper horizontal edge
                    EdgeDisabledFlag =(((VC1_EDGE_MB(TopIntra,VC1_BLOCK_2_INTRA)) && (VC1_EDGE_MB(CurrIntra, VC1_BLOCK_0_INTRA)))
                            *(IPPVC_EDGE_HALF_1))|
                            (((VC1_EDGE_MB(TopIntra,VC1_BLOCK_3_INTRA)) && (VC1_EDGE_MB(CurrIntra, VC1_BLOCK_1_INTRA)))
                            *(IPPVC_EDGE_HALF_2));
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*2 +6*VC1_PIXEL_IN_LUMA,     VC1_PIXEL_IN_LUMA*2,
                                                                CurrBlock,   VC1_PIXEL_IN_LUMA*2,
                                                                YPlane,      YPitch,
                                                                EdgeDisabledFlag);
                    /////////////////////////////
                    //MB internal horizontal edge
                    EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_2_INTRA) * (IPPVC_EDGE_HALF_1))
                                    |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_1_3_INTRA) * (IPPVC_EDGE_HALF_2));
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock + 8*8*2 - 32, VC1_PIXEL_IN_LUMA*2,
                                                                CurrBlock + 8*8*2,      VC1_PIXEL_IN_LUMA*2,
                                                                YPlane + 8*YPitch,      YPitch,
                                                                EdgeDisabledFlag);
                    if(((TopIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA) && ((CurrIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA))
                    {
                        /////////////////////////////
                        //U top horizontal smoothing
                        ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*4 +6*VC1_PIXEL_IN_CHROMA,            VC1_PIXEL_IN_CHROMA*2,
                                                                    CurrBlock + 4*64,  VC1_PIXEL_IN_CHROMA*2,
                                                                    UPlane,            UPitch);

                        /////////////////////////////
                        //V top horizontal smoothing
                        ippiSmoothingChroma_HorEdge_VC1_16s8u_C1R(CurrBlock - Width*8*8*6 + 64*5 +6*VC1_PIXEL_IN_CHROMA,           VC1_PIXEL_IN_CHROMA*2,
                                                                    CurrBlock + 64*5, VC1_PIXEL_IN_CHROMA*2,
                                                                    VPlane,           VPitch);
                    }                   
                }

                CurrBlock  += 8*8*6;
                pCurrMB++;
                LeftIntra    = (pCurrMB - 1)->IntraFlag*(pCurrMB - 1)->Overlap;
                TopLeftIntra = (pCurrMB - Width - 1)->IntraFlag*(pCurrMB - Width - 1)->Overlap;
            }
            else
            {
                YPlane = pCurrMB->currYPlane;
                UPlane = pCurrMB->currUPlane;
                VPlane = pCurrMB->currVPlane;

                CurrIntra    = (pCurrMB)->IntraFlag*pCurrMB->Overlap;

                if(CurrIntra)
                {
                    ////////////////////////////
                    //first block in row
                    //internal vertical smoothing
                    EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_1_INTRA) * (IPPVC_EDGE_HALF_1))
                                    |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_2_3_INTRA) * (IPPVC_EDGE_HALF_2));
                    ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6, VC1_PIXEL_IN_LUMA*2,
                                                                CurrBlock + 8, VC1_PIXEL_IN_LUMA*2,
                                                                YPlane + 8,    YPitch,
                                                                0,             EdgeDisabledFlag);
                }

                for (i = 1; i < Width; i++)
                {
                    CurrBlock  += 8*8*6;
                    pCurrMB++;

                    YPlane = pCurrMB->currYPlane;
                    UPlane = pCurrMB->currUPlane;
                    VPlane = pCurrMB->currVPlane;

                    //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[i-1];
                    //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[i-1];
                    //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[i-1];
                    CurrIntra    = (pCurrMB)->IntraFlag*pCurrMB->Overlap;
                    LeftIntra    = (pCurrMB - 1)->IntraFlag*(pCurrMB - 1)->Overlap;

                    //LUMA
                    if(CurrIntra)
                    {
                        ///////////////////////////////////
                        //left boundary vertical smoothing
                        EdgeDisabledFlag =(((VC1_EDGE_MB(CurrIntra,VC1_BLOCK_0_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_1_INTRA)))
                                *(IPPVC_EDGE_HALF_1))|
                                (((VC1_EDGE_MB(CurrIntra,VC1_BLOCK_2_INTRA)) && (VC1_EDGE_MB(LeftIntra, VC1_BLOCK_3_INTRA)))
                                *(IPPVC_EDGE_HALF_2));

                        ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*6+14, VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock,            VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane,               YPitch,
                                                                    0,                    EdgeDisabledFlag);
                        /////////////////////////////
                        //internal vertical smoothing
                        EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_1_INTRA) * (IPPVC_EDGE_HALF_1))
                                        |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_2_3_INTRA) * (IPPVC_EDGE_HALF_2));
                        ippiSmoothingLuma_VerEdge_VC1_16s8u_C1R(CurrBlock + 6,        VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock + 8,        VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane + 8,           YPitch,
                                                                    0,                    EdgeDisabledFlag);
                    }

                    if(LeftIntra)
                    {
                        ///////////////////////////////////
                        //left MB internal horizontal edge
                        EdgeDisabledFlag = (VC1_EDGE_MB(LeftIntra,VC1_BLOCKS_0_2_INTRA) * (IPPVC_EDGE_HALF_1))
                                        |(VC1_EDGE_MB(LeftIntra,VC1_BLOCKS_1_3_INTRA) * (IPPVC_EDGE_HALF_2));
                        ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock - 8*8*4 - 32, VC1_PIXEL_IN_LUMA*2,
                                                                    CurrBlock - 8*8*4,      VC1_PIXEL_IN_LUMA*2,
                                                                    YPlane - 16 + 8*YPitch, YPitch,
                                                                    EdgeDisabledFlag);
                        //CHROMA
                        if(((LeftIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA) && ((CurrIntra & VC1_BLOCK_4_INTRA) == VC1_BLOCK_4_INTRA))
                        {
                            ///////////////////////
                            //U vertical smoothing
                            ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8*2 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock + 8*8*4,     VC1_PIXEL_IN_CHROMA*2,
                                                                        UPlane,                UPitch);
                            //////////////////////
                            //V vertical smoothing
                            ippiSmoothingChroma_VerEdge_VC1_16s8u_C1R(CurrBlock - 8*8 + 6, VC1_PIXEL_IN_CHROMA*2,
                                                                        CurrBlock + 8*8*5,    VC1_PIXEL_IN_CHROMA*2,
                                                                        VPlane,               VPitch);
                        }                        
                    }
                }

                //RIGHT MB
                //LUMA
                //UpYrow = pContext->SmoothingInfo->SmoothUpperYRows[Width - 1];
                //UpUrow = pContext->SmoothingInfo->SmoothUpperURows[Width - 1];
                //UpVrow = pContext->SmoothingInfo->SmoothUpperVRows[Width - 1];

                if(CurrIntra)
                {
                    /////////////////////////////
                    //MB internal horizontal edge
                    EdgeDisabledFlag = (VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_0_2_INTRA) * (IPPVC_EDGE_HALF_1))
                                    |(VC1_EDGE_MB(CurrIntra,VC1_BLOCKS_1_3_INTRA) * (IPPVC_EDGE_HALF_2));
                    ippiSmoothingLuma_HorEdge_VC1_16s8u_C1R(CurrBlock + 8*8*2 - 32, VC1_PIXEL_IN_LUMA*2,
                                                                CurrBlock + 8*8*2,      VC1_PIXEL_IN_LUMA*2,
                                                                YPlane + 8*YPitch, YPitch,
                                                                EdgeDisabledFlag);
                }

                CurrBlock  += 8*8*6;
                pCurrMB++;
                LeftIntra    = (pCurrMB - 1)->IntraFlag*(pCurrMB - 1)->Overlap;
            }
        }
    }
}
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
