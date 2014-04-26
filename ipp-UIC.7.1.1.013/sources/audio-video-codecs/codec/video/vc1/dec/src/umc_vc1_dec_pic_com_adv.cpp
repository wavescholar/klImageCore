/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1(VC1) decoder, Common picture layer advanced profile
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_job.h"
#include "umc_vc1_dec_debug.h"

typedef void (*Expand)(VC1Context* pContext, Frame* pFrame);

static const Expand Expand_table[] =
             {
               (Expand)(ExpandFrame_Adv),
               (Expand)(ExpandFrame_Interlace),
               (Expand)(ExpandFrame_Interlace)
             };

void PrepareForNextFrame(VC1Context*pContext)
{

    if(pContext->m_picLayerHeader->FCM != VC1_FieldInterlace)
    {

        //for range map
        //luma
        pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;
        pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPUV = pContext->m_seqLayerHeader->RANGE_MAPUV;

    }
    else
    {

        //for range map
        //luma
        if (pContext->m_bIntensityCompensation)
        {
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iICompFieldIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;
        }
        else
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;


        if (pContext->m_bIntensityCompensation)
        {
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iICompFieldIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].RANGE_MAPY = pContext->m_seqLayerHeader->RANGE_MAPY;
        }
        else
            pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].RANGE_MAPUV = pContext->m_seqLayerHeader->RANGE_MAPUV;

    }
}


VC1Status DecodePictureHeader_Adv(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;
    Ipp32u i = 0;
    Ipp32u tempValue;
    Ipp32u RFF = 0;
    Ipp32u number_of_pan_scan_window;
    Ipp32u RPTFRM = 0;

    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;

    if(seqLayerHeader->INTERLACE)
    {
        //Frame Coding mode
        //0 - progressive; 10 - Frame-interlace; 11 - Field-interlace
        VC1_GET_BITS(1, picLayerHeader->FCM);
        if(picLayerHeader->FCM)
        {
            VC1_GET_BITS(1, picLayerHeader->FCM);
            if(picLayerHeader->FCM)
                picLayerHeader->FCM = VC1_FieldInterlace;
            else
                picLayerHeader->FCM = VC1_FrameInterlace;
        }
        else
            picLayerHeader->FCM = VC1_Progressive;
    }


    if(picLayerHeader->FCM != VC1_FieldInterlace)
    {
        //picture type
        //110 - I picture; 0 - P picture; 10 - B picture; 1110 - BI picture; 1111 - skipped
        VC1_GET_BITS(1, picLayerHeader->PTYPE);
        if(picLayerHeader->PTYPE)
        {
            VC1_GET_BITS(1, picLayerHeader->PTYPE);
            if(picLayerHeader->PTYPE)
            {
                VC1_GET_BITS(1, picLayerHeader->PTYPE);
                if(picLayerHeader->PTYPE)
                {
                    VC1_GET_BITS(1, picLayerHeader->PTYPE);
                    if(picLayerHeader->PTYPE)
                    {
                        //1111
                        picLayerHeader->PTYPE = VC1_SKIPPED_FRAME;
                    }
                    else
                    {
                        //1110
                        picLayerHeader->PTYPE = VC1_BI_FRAME;
                    }
                }
                else
                {
                    //110
                    picLayerHeader->PTYPE = VC1_I_FRAME;
                }
            }
            else
            {
                //10
                picLayerHeader->PTYPE = VC1_B_FRAME;
            }
        }
        else
        {
            //0
            picLayerHeader->PTYPE = VC1_P_FRAME;
        }

        if(!(picLayerHeader->PTYPE == VC1_SKIPPED_FRAME))
        {
            if(seqLayerHeader->TFCNTRFLAG)
            {
                //temporal reference frame counter
                VC1_GET_BITS(8, tempValue);       //TFCNTR
            }
        }

        if(seqLayerHeader->PULLDOWN)
        {
            if(!(seqLayerHeader->INTERLACE))
            {
                //repeat frame count
                VC1_GET_BITS(2, RPTFRM);
            }
            else
            {
                Ipp32u tff;
                //top field first
                VC1_GET_BITS(1, tff);
                picLayerHeader->TFF = (Ipp8u)tff;
                //repeat first field
                VC1_GET_BITS(1, RFF);
            }
        }

        if(seqLayerHeader->PANSCAN_FLAG)
        {
            //pan scan present flag
            VC1_GET_BITS(1,tempValue);       //PS_PRESENT

            if(tempValue)       //PS_PRESENT
            {
                //calculate number ofpan scan window, see standard, p177
                if(seqLayerHeader->INTERLACE)
                {
                    if(seqLayerHeader->PULLDOWN)
                    {
                        number_of_pan_scan_window = 2 + RFF;
                    }
                    else
                    {
                        number_of_pan_scan_window = 2;
                    }
                }
                else
                {
                    if(seqLayerHeader->PULLDOWN)
                    {
                        number_of_pan_scan_window = 1 + RPTFRM;
                    }
                    else
                    {
                        number_of_pan_scan_window = 1;
                    }
                }

                //fill in pan scan window struture
                for (i = 0; i< number_of_pan_scan_window; i++)
                {
                    //PS_HOFFSET
                    VC1_GET_BITS(18,tempValue);
                    //PS_VOFFSET
                    VC1_GET_BITS(18,tempValue);
                    //PS_WIDTH
                    VC1_GET_BITS(14,tempValue);
                    //PS_HEIGHT
                    VC1_GET_BITS(14,tempValue);
                }
            }
        }

        if(!(picLayerHeader->PTYPE == VC1_SKIPPED_FRAME))
        {
            //rounding control
            VC1_GET_BITS(1,picLayerHeader->RNDCTRL);

            if((seqLayerHeader->INTERLACE) || (picLayerHeader->FCM != VC1_Progressive))
            {
                //UV sampling format
                VC1_GET_BITS(1,tempValue);//UVSAMP
            }

            if(seqLayerHeader->FINTERPFLAG && (picLayerHeader->FCM == VC1_Progressive) )
            {
                //frame interpolation hint
                VC1_GET_BITS(1,tempValue);
            }

            if(picLayerHeader->PTYPE == VC1_B_FRAME && (picLayerHeader->FCM == VC1_Progressive) )
            {
                //B picture fraction
                Ipp8s  z1;
                Ipp16s z2;
                ippiDecodeHuffmanPair_1u16s(&pContext->m_bitstream.pBitstream,
                                            &pContext->m_bitstream.bitOffset,
                                            pContext->m_vlcTbl->BFRACTION,
                                            &z1, &z2);
                VM_ASSERT (z2 != VC1_BRACTION_INVALID);
                VM_ASSERT (!(z2 == VC1_BRACTION_BI && seqLayerHeader->PROFILE==VC1_PROFILE_ADVANCED));

                if (z2 == VC1_BRACTION_BI)
                {
                    picLayerHeader->PTYPE = VC1_BI_FRAME;
                }

                picLayerHeader->BFRACTION = (z1*2>=z2)?1:0;
                picLayerHeader->ScaleFactor = ((256+z2/2)/z2)*z1;

            }

            //picture quantizer index
            VC1_GET_BITS(5,picLayerHeader->PQINDEX);
            CalculatePQuant(pContext);

            if(picLayerHeader->PQINDEX<=8)
            {
                //half QP step
                VC1_GET_BITS(1,picLayerHeader->HALFQP);
            }
            else
                picLayerHeader->HALFQP = 0;


            if(seqLayerHeader->QUANTIZER == 1)
            {
                //picture quantizer type
                VC1_GET_BITS(1,pContext->m_picLayerHeader->PQUANTIZER);    //PQUANTIZER
            }

            if(seqLayerHeader->POSTPROCFLAG)
            {
                //post processing
                VC1_GET_BITS(2,tempValue);        //POSTPROC
            }
        }
    }
    else
    {
        //FIELD INTERLACE FRAME
        DecodePictHeaderParams_InterlaceFieldPicture_Adv(pContext);
    }

    return vc1Sts;
}

typedef VC1Status (*DecoderPicHeader)(VC1Context* pContext);
static const DecoderPicHeader DecoderPicHeader_table[3][5] =
{
    {
        (DecoderPicHeader)(DecodePictHeaderParams_ProgressiveIpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_ProgressivePpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_ProgressiveBpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_ProgressiveIpicture_Adv),
        (DecoderPicHeader)(DecodeSkippicture)
    },
    {
        (DecoderPicHeader)(DecodePictHeaderParams_InterlaceIpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_InterlacePpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_InterlaceBpicture_Adv),
        (DecoderPicHeader)(DecodePictHeaderParams_InterlaceIpicture_Adv),
        (DecoderPicHeader)(DecodeSkippicture)
    },
    {
        (DecoderPicHeader)(DecodeFieldHeaderParams_InterlaceFieldIpicture_Adv),
        (DecoderPicHeader)(DecodeFieldHeaderParams_InterlaceFieldPpicture_Adv),
        (DecoderPicHeader)(DecodeFieldHeaderParams_InterlaceFieldBpicture_Adv),
        (DecoderPicHeader)(DecodeFieldHeaderParams_InterlaceFieldIpicture_Adv),
        (DecoderPicHeader)(DecodeSkippicture)
    }
};

VC1Status DecodePicHeader(VC1Context* pContext)
{
   VC1Status vc1Sts = VC1_OK;

   vc1Sts = DecoderPicHeader_table[pContext->m_picLayerHeader->FCM][pContext->m_picLayerHeader->PTYPE](pContext);

   return vc1Sts;
}

VC1Status DecodeSkippicture(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;

    ippsCopy_8u(pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].m_pAllocatedMemory,
                pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex].m_pAllocatedMemory,
                pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iPrevIndex].m_AllocatedMemorySize);

    return vc1Sts;
}

VC1Status DecodePictHeaderParams_InterlaceFieldPicture_Adv (VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;
    Ipp32u i = 0;
    Ipp32u tempValue;
    Ipp32u RFF = 0;
    Ipp32u number_of_pan_scan_window;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;


    VC1_GET_BITS(3, tempValue);
    switch(tempValue)
    {
    case 0:
        //000  - I,I
        picLayerHeader->PTypeField1 = VC1_I_FRAME;
        picLayerHeader->PTypeField2 = VC1_I_FRAME;
        break;
    case 1:
        //001 - I,P
        picLayerHeader->PTypeField1 = VC1_I_FRAME;
        picLayerHeader->PTypeField2 = VC1_P_FRAME;
        break;
    case 2:
        //010 - P,I
        picLayerHeader->PTypeField1 = VC1_P_FRAME;
        picLayerHeader->PTypeField2 = VC1_I_FRAME;
        break;
    case 3:
        //011 - P,P
        picLayerHeader->PTypeField1 = VC1_P_FRAME;
        picLayerHeader->PTypeField2 = VC1_P_FRAME;
        break;
    case 4:
        //100 - B,B
        picLayerHeader->PTypeField1 = VC1_B_FRAME;
        picLayerHeader->PTypeField2 = VC1_B_FRAME;
        break;
    case 5:
        //101 - B,BI
        picLayerHeader->PTypeField1 = VC1_B_FRAME;
        picLayerHeader->PTypeField2 = VC1_BI_FRAME;
        break;
    case 6:
        //110 - BI,B
        picLayerHeader->PTypeField1 = VC1_BI_FRAME;
        picLayerHeader->PTypeField2 = VC1_B_FRAME;
        break;
    case 7:
        //111 - BI,BI
        picLayerHeader->PTypeField1 = VC1_BI_FRAME;
        picLayerHeader->PTypeField2 = VC1_BI_FRAME;
        break;
    default:
        VM_ASSERT(0);
        break;
    }

    if(seqLayerHeader->TFCNTRFLAG)
    {
        //temporal reference frame counter
        VC1_GET_BITS(8, tempValue);       //TFCNTR
    }

    if(seqLayerHeader->PULLDOWN)
    {
       if(!(seqLayerHeader->INTERLACE))
       {
          //repeat frame count
          VC1_GET_BITS(2,tempValue);//RPTFRM
       }
       else
       {
           Ipp32u tff;
          //top field first
          VC1_GET_BITS(1, tff);
          picLayerHeader->TFF = (Ipp8u)tff;
          //repeat first field
          VC1_GET_BITS(1, RFF);
       }
    } else
        picLayerHeader->TFF = 1;

    if(seqLayerHeader->PANSCAN_FLAG)
    {
        //pan scan present flag
        VC1_GET_BITS(1,tempValue);       //PS_PRESENT

        if(tempValue)       //PS_PRESENT
        {
         //calculate number ofpan scan window, see standard, p177

          if(seqLayerHeader->PULLDOWN)
          {
            number_of_pan_scan_window = 2 + RFF;
          }
          else
          {
             number_of_pan_scan_window = 2;
          }

          //fill in pan scan window struture
          for (i = 0; i<number_of_pan_scan_window; i++)
          {
             //PS_HOFFSET
             VC1_GET_BITS(18,tempValue);
             //PS_VOFFSET
             VC1_GET_BITS(18,tempValue);
             //PS_WIDTH
             VC1_GET_BITS(14,tempValue);
             //PS_HEIGHT
             VC1_GET_BITS(14,tempValue);
           }
        }
    }
        //rounding control
        VC1_GET_BITS(1,picLayerHeader->RNDCTRL);

        //UV sampling format
        VC1_GET_BITS(1,tempValue);//UVSAMP

        if(seqLayerHeader->REFDIST_FLAG == 1 &&
            (picLayerHeader->PTypeField1 < VC1_B_FRAME &&
             picLayerHeader->PTypeField2 < VC1_B_FRAME))
        {
                Ipp32s ret;
                ret = ippiDecodeHuffmanOne_1u32s (
                                    &pContext->m_bitstream.pBitstream,
                                    &pContext->m_bitstream.bitOffset,
                                    &picLayerHeader->REFDIST,
                                    pContext->m_vlcTbl->REFDIST_TABLE);
                VM_ASSERT(ret == ippStsNoErr);

                *pContext->pRefDist = picLayerHeader->REFDIST;
        }
        else if(seqLayerHeader->REFDIST_FLAG == 0)
        {
             *pContext->pRefDist = 0;
             picLayerHeader->REFDIST = 0;
        }
        else
        {
            picLayerHeader->REFDIST = 0;
        }

        if( (picLayerHeader->PTypeField1 >= VC1_B_FRAME &&
             picLayerHeader->PTypeField2 >= VC1_B_FRAME) )
        {
           //B picture fraction
           Ipp8s  z1;
           Ipp16s z2;
           ippiDecodeHuffmanPair_1u16s(&pContext->m_bitstream.pBitstream,
                                        &pContext->m_bitstream.bitOffset,
                                         pContext->m_vlcTbl->BFRACTION,
                                        &z1, &z2);
           VM_ASSERT (z2 != VC1_BRACTION_INVALID);
           VM_ASSERT (!(z2 == VC1_BRACTION_BI && seqLayerHeader->PROFILE==VC1_PROFILE_ADVANCED));

           picLayerHeader->BFRACTION = (z1*2>=z2)?1:0;
           picLayerHeader->ScaleFactor = ((256+z2/2)/z2)*z1;
        }

    if(picLayerHeader->CurrField == 0)
    {
        picLayerHeader->PTYPE = picLayerHeader->PTypeField1;
        picLayerHeader->BottomField = (Ipp8u)(1 - picLayerHeader->TFF);
    }
    else
    {
        picLayerHeader->BottomField = (Ipp8u)(picLayerHeader->TFF);
        picLayerHeader->PTYPE = picLayerHeader->PTypeField2;
    }
    return vc1Sts;
}

VC1Status Decode_FirstField_Adv(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    picLayerHeader->BottomField = (Ipp8u)(1 - picLayerHeader->TFF);
    picLayerHeader->CurrField = 0;

    pContext->m_pSingleMB->slice_currMBYpos = 0;
    pContext->m_pSingleMB->m_currMBXpos = 0;

    picLayerHeader->PTYPE = picLayerHeader->PTypeField1;
    switch(picLayerHeader->PTypeField1)
    {
    case VC1_I_FRAME:
        Decode_InterlaceFieldIpicture_Adv(pContext);
        break;

    case VC1_P_FRAME:
        Decode_InterlaceFieldPpicture_Adv(pContext);
        break;

    case VC1_B_FRAME:
        Decode_InterlaceFieldBpicture_Adv(pContext);
        break;

    case VC1_BI_FRAME:
        Decode_InterlaceFieldIpicture_Adv(pContext);
        break;
    default:
        break;
    }

    ExpandFrame_Field(pContext,
                      &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex],
                      picLayerHeader->BottomField);

    return vc1Sts;
}

VC1Status Decode_SecondField_Adv(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    picLayerHeader->BottomField = (Ipp8u)picLayerHeader->TFF;
    picLayerHeader->PTYPE = picLayerHeader->PTypeField2;

    picLayerHeader->CurrField = 1;

    ExpandFrame_Field(pContext,
                      &pContext->m_frmBuff.m_pFrames[pContext->m_frmBuff.m_iCurrIndex],
                      picLayerHeader->BottomField);

    return vc1Sts;

}
VC1Status Decode_FieldPictureLayer_Adv(VC1Context* pContext)
{
    VC1Status vc1Sts = VC1_OK;
    Ipp32u temp_value;

    VC1_GET_BITS(1,temp_value);

    while(!(pContext->m_bitstream.bitOffset == 7||
            pContext->m_bitstream.bitOffset == 15 ||
            pContext->m_bitstream.bitOffset == 23 ||
            pContext->m_bitstream.bitOffset == 31 ))
            VC1_GET_BITS(1,temp_value);

   return vc1Sts;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER


