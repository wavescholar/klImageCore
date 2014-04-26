/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, VOPDEQUANT
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_dec_seq.h"
#include "umc_vc1_dec_debug.h"

//Figure 14:  Syntax diagram for VOPDQUANT in
//(Progressive P, Interlace I and Interlace P) picture header
//3.2.1.27    VOPDQUANT Syntax Elements
VC1Status VOPDQuant(VC1Context* pContext)
{
    Ipp32u tempValue;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    Ipp32u DQUANT  = pContext->m_seqLayerHeader->DQUANT;


    //pContext->m_picLayerHeader->bVopdquantCoded = 1;
    if(DQUANT == 1)
    {
        //The DQUANTFRM field is a 1 bit value that is present only
        //when DQUANT = 1.  If DQUANTFRM = 0 then the current picture
        //is only quantized with PQUANT.
        VC1_GET_BITS(1, picLayerHeader->m_DQuantFRM);

        if(picLayerHeader->m_DQuantFRM == 1)
        {
            //The DQPROFILE field is a 2 bits value that is present
            //only when DQUANT = 1 and DQUANTFRM = 1.  It indicates
            //where we are allowed to change quantization step sizes
            //within the current picture.
            //Table 15:  Macroblock Quantization Profile (DQPROFILE) Code Table
            //FLC    Location
            //00    All four Edges
            //01    Double Edges
            //10    Single Edges
            //11    All Macroblocks
            VC1_GET_BITS(2,picLayerHeader->m_DQProfile);
            switch (picLayerHeader->m_DQProfile)
            {
                case VC1_DQPROFILE_ALL4EDGES:
                    picLayerHeader->m_PQuant_mode = VC1_ALTPQUANT_EDGES;
                    break;
                case VC1_DQPROFILE_SNGLEDGES:
                {
                    //Ipp32u m_DQSBEdge;
                    //The DQSBEDGE field is a 2 bits value that is present
                    //when DQPROFILE = Single Edge.  It indicates which edge
                    //will be quantized with ALTPQUANT.
                    //Table 16:  Single Boundary Edge Selection (DQSBEDGE) Code Table
                    //FLC    Boundary Edge
                    //00    Left
                    //01    Top
                    //10    Right
                    //11    Bottom
                    VC1_GET_BITS(2, picLayerHeader->DQSBEdge);
                    picLayerHeader->m_PQuant_mode = 1<<picLayerHeader->DQSBEdge;
                    break;
                }
                case VC1_DQPROFILE_DBLEDGES:
                {
                    //Ipp32u m_DQDBEdge;
                    //The DQSBEDGE field is a 2 bits value that is present
                    //when DQPROFILE = Double Edge.  It indicates which two
                    //edges will be quantized with ALTPQUANT.
                    //Table 17:  Double Boundary Edges Selection (DQDBEDGE) Code Table
                    //FLC    Boundary Edges
                    //00    Left and Top
                    //01    Top and Right
                    //10    Right and Bottom
                    //11    Bottom and Left
                    VC1_GET_BITS(2, picLayerHeader->DQSBEdge);
                    picLayerHeader->m_PQuant_mode = (picLayerHeader->DQSBEdge>1)?VC1_ALTPQUANT_BOTTOM:VC1_ALTPQUANT_TOP;
                    picLayerHeader->m_PQuant_mode |= ((picLayerHeader->DQSBEdge%3)? VC1_ALTPQUANT_RIGTHT:VC1_ALTPQUANT_LEFT);
                    break;
                }
                case VC1_DQPROFILE_ALLMBLKS:
                {
                    //The DQBILEVEL field is a 1 bit value that is present
                    //when DQPROFILE = All Macroblock.  If DQBILEVEL = 1,
                    //then each macroblock in the picture can take one of
                    //two possible values (PQUANT or ALTPQUANT).  If
                    //DQBILEVEL = 0, then each macroblock in the picture
                    //can take on any quantization step size.
                    VC1_GET_BITS(1, picLayerHeader->m_DQBILevel);
                    picLayerHeader->m_PQuant_mode = (picLayerHeader->m_DQBILevel)? VC1_ALTPQUANT_MB_LEVEL:VC1_ALTPQUANT_ANY_VALUE;
                    break;
                }
            }
        }
        else
            picLayerHeader->m_PQuant_mode=VC1_ALTPQUANT_NO;
    }
    else if (DQUANT == 2)
    {
        picLayerHeader->m_PQuant_mode = VC1_ALTPQUANT_ALL;
        //picLayerHeader->m_PQuant_mode = VC1_ALTPQUANT_EDGES;
        picLayerHeader->m_DQuantFRM = 1;
    }
    else
        picLayerHeader->m_PQuant_mode=VC1_ALTPQUANT_NO;
    //PQDIFF is a 3 bit field that encodes either the PQUANT
    //differential or encodes an escape code.
    //If PQDIFF does not equal 7 then PQDIFF encodes the
    //differential and the ABSPQ field does not follow in
    //the bitstream. In this case:
    //      ALTPQUANT = PQUANT + PQDIFF + 1
    //If PQDIFF equals 7 then the ABSPQ field follows in
    //the bitstream and ALTPQUANT is decoded as:
    //      ALTPQUANT = ABSPQ
    if (picLayerHeader->m_DQuantFRM)
    {
        if(DQUANT==2 || !(picLayerHeader->m_DQProfile == VC1_DQPROFILE_ALLMBLKS
                            && picLayerHeader->m_DQBILevel == 0))
        {
            VC1_GET_BITS(3, tempValue); //PQDIFF

            if(tempValue == 7) // escape
            {
                //ABSPQ is present in the bitstream if PQDIFF equals 7.
                //In this case, ABSPQ directly encodes the value of
                //ALTPQUANT as described above.
                VC1_GET_BITS(5, tempValue);       //m_ABSPQ

                picLayerHeader->m_AltPQuant = tempValue;       //m_ABSPQ
            }
            else
            {
                picLayerHeader->m_AltPQuant = picLayerHeader->PQUANT + tempValue + 1;
            }
        }
    }
    return VC1_OK;
}

static const Ipp8u MapPQIndToQuant_Impl[] =
{
    VC1_UNDEF_PQUANT,
    1, 2, 3, 4, 5, 6, 7, 8,
    6, 7, 8, 9, 10,11,12,13,
    14,15,16,17,18,19,20,21,
    22,23,24,25,27,29,31
};

static const Ipp8u MapPQIndToQuant_Expl5QP[] =
{
    0,
    1, 1, 1,
    2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,
    14,15,16,17,18,19,20,21,
    22,23,24,25, 26, 27,29, 31
};

VC1Status CalculatePQuant(VC1Context* pContext)
{
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;
    VC1SequenceLayerHeader* seqLayerHeader = pContext->m_seqLayerHeader;

    picLayerHeader->PQUANT = picLayerHeader->PQINDEX;
    picLayerHeader->QuantizationType = VC1_QUANTIZER_UNIFORM;

    if(seqLayerHeader->QUANTIZER == 0)
    {
        //If the implicit quantizer is used (signaled by sequence field
        //QUANTIZER = 00, see section 3.1.19) then PQINDEX specifies both
        //the picture quantizer scale (PQUANT) and the quantizer (3QP or
        //5QP deadzone) used for the frame. Table 5 shows how PQINDEX is
        //translated to PQUANT and the quantizer for implicit mode.
        if(picLayerHeader->PQINDEX < 9)
        {
            picLayerHeader->QuantizationType = VC1_QUANTIZER_UNIFORM;
        }
        else
        {
            picLayerHeader->QuantizationType = VC1_QUANTIZER_NONUNIFORM;
            picLayerHeader->PQUANT = MapPQIndToQuant_Impl[picLayerHeader->PQINDEX];
        }
    }
    else //01 or 10 or 11 binary
    {
        //If the quantizer is signaled explicitly at the sequence or frame
        //level (signaled by sequence field QUANTIZER = 01, 10 or 11 see
        //section 3.1.19) then PQINDEX is translated to the picture quantizer
        //stepsize PQUANT as indicated by Table 6.
        if(seqLayerHeader->QUANTIZER == 2)
        {
            picLayerHeader->QuantizationType = VC1_QUANTIZER_NONUNIFORM;
        }
    }
    return VC1_OK;
}

VC1Status Set_MQuant(VC1Context* pContext)
{
    Ipp32s heightMB = pContext->m_seqLayerHeader->heightMB;
    Ipp8u X;
    Ipp32s MQUANT = pContext->m_picLayerHeader->PQUANT;
    Ipp32s HALFQP = pContext->m_picLayerHeader->HALFQP;

    X = (Ipp8u)((pContext->m_pSingleMB->m_currMBXpos==0)|
        ((pContext->m_pSingleMB->m_currMBYpos==0)<<1)|
        ((pContext->m_pSingleMB->m_currMBXpos ==
            pContext->m_seqLayerHeader->widthMB -1)<<2)|
        ((pContext->m_pSingleMB->m_currMBYpos ==
            heightMB-1)<<3)|
        (1<<4));
    if ((pContext->m_picLayerHeader->m_PQuant_mode&X)&&
        (X > 16))
    {
        MQUANT = pContext->m_picLayerHeader->m_AltPQuant;
        HALFQP=0;
    }
    else
        MQUANT = pContext->m_picLayerHeader->PQUANT;

    pContext->CurrDC->DoubleQuant = MQUANT *2 + HALFQP;
    pContext->CurrDC->DCStepSize = GetDCStepSize(MQUANT);
    return VC1_OK;
}

VC1Status Set_Alt_MQUANT(VC1Context* pContext)
{
    Ipp32u MQDIFF;
    Ipp32s MQUANT = pContext->m_picLayerHeader->PQUANT;
    Ipp32s HALFQP = pContext->m_picLayerHeader->HALFQP;

    if (VC1_DQPROFILE_ALLMBLKS == pContext->m_picLayerHeader->m_DQProfile)
    {
        if (pContext->m_picLayerHeader->m_DQBILevel)
        {
            VC1_GET_BITS(1, MQDIFF);
            if (MQDIFF)
            {
                MQUANT = pContext->m_picLayerHeader->m_AltPQuant;
                HALFQP = 0;
            }
        }
        else
        {
            VC1_GET_BITS(3, MQDIFF);
            HALFQP = 0;
            if (7 != MQDIFF)
                MQUANT = (pContext->m_picLayerHeader->PQUANT + MQDIFF);
            else
                VC1_GET_BITS(5, MQUANT); // ABSMQ
        }
        //QUANT
        pContext->CurrDC->DCStepSize = GetDCStepSize(MQUANT);
        pContext->CurrDC->DoubleQuant = MQUANT *2 + HALFQP;
    }
    return VC1_OK;
}

VC1Status Set_MQuant_Field(VC1Context* pContext)
{
    Ipp32s heightMB = pContext->m_seqLayerHeader->heightMB;
    Ipp8u X;
    Ipp32s MQUANT = pContext->m_picLayerHeader->PQUANT;
    Ipp32s HALFQP = pContext->m_picLayerHeader->HALFQP;


    X = (Ipp8u)((pContext->m_pSingleMB->m_currMBXpos==0)|
        (((pContext->m_pSingleMB->m_currMBYpos==0)||
            (pContext->m_pSingleMB->m_currMBYpos == heightMB/2))<<1)|
        ((pContext->m_pSingleMB->m_currMBXpos ==
            pContext->m_seqLayerHeader->widthMB -1)<<2)|
        (((pContext->m_pSingleMB->m_currMBYpos ==
            heightMB-1)|| (pContext->m_pSingleMB->m_currMBYpos == heightMB/2-1))<<3)|(1<<4));

    if (pContext->m_picLayerHeader->m_PQuant_mode&X)
    {
        MQUANT = pContext->m_picLayerHeader->m_AltPQuant;
        HALFQP=0;
    }
    else
        MQUANT = pContext->m_picLayerHeader->PQUANT;

    pContext->CurrDC->DCStepSize = GetDCStepSize(MQUANT);
    pContext->CurrDC->DoubleQuant = MQUANT *2 + HALFQP;
    return VC1_OK;
}


void GetMQUANT(VC1Context* pContext)
{
    Ipp32s z;
    VC1PictureLayerHeader* picLayerHeader = pContext->m_picLayerHeader;

    Ipp32u MQUANT = picLayerHeader->PQUANT;
    Ipp32u HALFQP = picLayerHeader->HALFQP;

    if (picLayerHeader->m_PQuant_mode==VC1_ALTPQUANT_MB_LEVEL)
    {
        VC1_GET_BITS(1,z );

        MQUANT= (z)? picLayerHeader->m_AltPQuant: picLayerHeader->PQUANT;

    }
    else
    {
        VM_ASSERT(picLayerHeader->m_PQuant_mode==VC1_ALTPQUANT_ANY_VALUE);
        VC1_GET_BITS(3,z);
        HALFQP = 0;
        if (z!=7)
        {
            MQUANT = picLayerHeader->PQUANT + z;
        }
        else
        {
            VC1_GET_BITS(5,z);
            MQUANT=z;
        }

    }//m_DQBILevel==0

    if ((MQUANT == picLayerHeader->m_AltPQuant))
        HALFQP=0;

    pContext->CurrDC->DCStepSize = GetDCStepSize(MQUANT);
    pContext->CurrDC->DoubleQuant = MQUANT *2 + HALFQP;
}

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
