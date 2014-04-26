/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//                     motion estimation
//
*/
#include "umc_vme.h"
#ifdef ME_VME

U8 FullSP[64] = {
        0x0F,0xF0,0x01,0x01,0x10,0x10,0x0F,0x0F, 0x0F,0xF0,0xF0,0xF0,0x01,0x01,0x01,0x01,
        0x10,0x10,0x10,0x10,0x0F,0x0F,0x0F,0x0F, 0x0F,0xF0,0xF0,0xF0,0xF0,0xF0,0x01,0x01,
        0x01,0x01,0x01,0x01,0x10,0x10,0x10,0x10, 0x10,0x10,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
        0x0F,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0, 0,0,0,0,0,0,0,0};

U8  DummyLutMode[4][8] = { {0,0,0,0,0,0,0,0}, {16,16,20,16,24,32,16,20}, {32,32,40,32,48,64,32,40}, {64,64,80,64,80,96,64,72} };
U8  DummyLutXY[4][8] = { {0,0,0,0,0,0,0,0}, {10,16,20,24,28,32,36,40}, {20,32,40,48,56,64,72,80}, {40,64,80,96,112,128,144,160} };

static int    vmesetup = 0;

namespace UMC
{

MeVme::MeVme() : MeBase()
{
    vme = NULL;
}

MeVme::~MeVme()
{
    Close();
}

bool MeVme::Init(MeInitParams *par)
{
    if(!MeBase::Init(par))
        return false;

    vme    = new IntelVideoVME;
    if(NULL == vme)
        return false;
    memset(&vout,0,sizeof(VMEOutput));
    return true;
}

void MeVme::Close()
{
    MeBase::Close();
    delete vme;
    vme = NULL;
}

bool MeVme::EstimateFrame(MeParams *par)
{
    m_par = par;
    Ipp32s SKIP_TYPE = SKIP_NULL;
    bool no_skip;

    if(!CheckParams())
        return false;

    switch(m_PredictionType)
    {
    case ME_MPEG2:
        GetPredictor = &UMC::MeBase::GetPredictorMPEG2;
        break;
    case ME_VC1:
        GetPredictor = &UMC::MeBase::GetPredictorVC1;
        break;
    case ME_VC1Hybrid:
        GetPredictor = &UMC::MeBase::GetPredictorVC1_hybrid;
        break;
    default:
        assert(0);
    }

    if(m_par->SearchDirection == ME_BidirSearch)
    {
        m_MVDirectFW[0] = m_par->pRefF[0]->MVDirect;
        m_MVDirectBW[0] = m_par->pRefB[0]->MVDirect;
    }

    //estimate
    Ipp16s x, y, x16, y16;

    for(m_adr = m_par->FirstMB; m_adr <= m_par->LastMB; m_adr++)
    {
        y = (Ipp16s)(m_adr/m_WidthMB);
        x = (Ipp16s)(m_adr%m_WidthMB);
        x16 = x*16;
        y16 = y*16;
        // m_adr = m_WidthMB*y + x;

        m_CurMB.x = x;
        m_CurMB.y = y;
        m_adr = m_WidthMB*y + x;

        no_skip = !m_par->ProcessSkipped || ((m_PredictionType == ME_MPEG2) && (x == 0 || x == m_WidthMB - 1 || m_ResMB[m_adr-1].MbType == ME_MbIntra));

        if(x == 0) m_MbLeftEnable = false;
        else m_MbLeftEnable = true;

        if(x == m_WidthMB-1) m_MbRightEnable = false;
        else m_MbRightEnable = true;

        if(y == 0)
        {
            m_MbTopEnable = m_MbTopLeftEnable = m_MbTopRightEnable = false;
        }
        else
        {
            m_MbTopEnable = m_MbTopLeftEnable = m_MbTopRightEnable = true;
            if(!m_MbLeftEnable)  m_MbTopLeftEnable  = false;
            if(!m_MbRightEnable) m_MbTopRightEnable = false;
        }

        if(m_par->SearchDirection == ME_ForwardSearch)
        {
            //16x16
            (this->*(GetPredictor))(ME_Macroblock,0,m_CurPrediction);
            m_CurPredictionF[0] = m_CurPrediction[0];
            if(no_skip) SKIP_TYPE = SKIP_NULL;
            else SKIP_TYPE = SKIP_P_SKIP;

            EstimateMB(x16,y16, SKIP_TYPE);
        }
        else if(m_par->SearchDirection == ME_BidirSearch)
        {
            (this->*(GetPredictor))(ME_Macroblock,0,m_CurPrediction);
            m_CurPredictionF[0] = m_CurPrediction[0];
            (this->*(GetPredictor))(ME_Macroblock,1,m_CurPrediction);
            m_CurPredictionB[0] = m_CurPrediction[0];

            if(no_skip) SKIP_TYPE = SKIP_NULL;
            else SKIP_TYPE = SKIP_B_DIRECT;
            EstimateMB(x16,y16, SKIP_TYPE);

        }//if(m_par->SearchDirection == ME_BidirSearch)
        else assert(0);

    }//for m_adr

    return true;
}

void MeVme::EstimateMB(Ipp16s x, Ipp16s y, Ipp32s SKIP_TYPE)
{
   // Macroblock mb;
    I16PAIR srcxy, refxy, cctr1, cctr2;
    Ipp8u *pSrc  = m_pSrcFrameY;
    Ipp8u *pRef1;
    Ipp8u *pRef2;
    Ipp32s num_search_centers = 1;
    Ipp32s bidir_mask, motion_mask, weight;
    Ipp32s ph = 16*m_HeightMB;

    pRef1 = m_par->pRefF[0]->ptr[0];
    if(m_par->SearchDirection == ME_ForwardSearch)
    {
        pRef2 = NULL;
        num_search_centers = 1;
        cctr1.x = m_CurPredictionF[0].x + (x << 2);
        cctr1.y = m_CurPredictionF[0].y + (y << 2);
        cctr2.x = m_CurPredictionF[0].x + (x << 2);
        cctr2.y = m_CurPredictionF[0].y + (y << 2);
        bidir_mask = 0x0f;
        weight = 0;
        if(m_par->MbPart & ME_Mb8x8) motion_mask = 0xf6;
        else /*16x16 only*/ motion_mask = 0xfe;
    }
    else
    {
        pRef2 = m_par->pRefB[0]->ptr[0];
        num_search_centers = 2;
        cctr1.x = m_CurPredictionF[0].x + (x << 2);
        cctr1.y = m_CurPredictionF[0].y + (y << 2);
        cctr2.x = m_CurPredictionB[0].x + (x << 2);
        cctr2.y = m_CurPredictionB[0].y + (y << 2);
        bidir_mask = 0x0e;
        weight = 16;
        motion_mask = 0xfe;
    }

    vme->SetSourceFrame(pSrc, m_SrcStep);//It provides VME the source frame buffer surface and its frame pitch.
    vme->SetReferenceFrame(pRef1, pRef2, m_RefStep, ph);//It provides VME the reference frame buffer surfaces and its frame dimension.
    vme->SetIntraInterMasterSwitch( 1, 0 );//It is the master switch of doing Inter or Intra searchings (in this case intra = false, inter = true)
    vme->SetHardwareConstraints( 0 );//It requires the C-model to perform on the functionality that are supported by the Gesher HW.
    srcxy.x = x;
    srcxy.y = y;
    vme->SetSourceMB( srcxy, 0, 0);//the arguments: coordinates, Mb16x16 type (0- 16x16, 1 - 16x8, 2 - 8x16, 3 - 8x8), FRAME (is_field = false)
    refxy.x = (x-16);
    refxy.y = (y-12);
    vme->SetReferenceWindows( num_search_centers, refxy, refxy, 48, 40, 0); //the arguments: the number of windows ( = number of ref frames), top-left coordinates, the window dimensions, is_field
    vme->SetSearchPaths( num_search_centers, 0, 0, 0, 0, 64, 64);//It sets up the search path properties
    vme->SetSearchCenters( num_search_centers, cctr1, cctr2 );//the arguments: the number of searching starters (1 or 2),the 1st search starter in quarter pel relative to the reference window,the 2nd search starter in quarter pel, if needed
    vme->SetDistortionMethods( 0, 0 );//the arguments:the method for Intra-searching, the method for Inter-searching -- SAD (0)
    vme->SetSkipMVs( SKIP_TYPE, cctr1, cctr2 );//the arguments:SkipType (SKIP_NULL,SKIP_P_SKIP,SKIP_B_DIRECT),the skip motion vector in quarter-pel relative to the reference window for the first ref, the skip motion vector for the 2nd reference, if needed.
    vme->SetCostingCenters( cctr1, cctr2 );//It sets the relative motion vector center in quartel-pel for motion vector costing. (for the first referense, for the second ref.)
    vme->SetEarlyDecisions( 0,0,0,0,0 );//no (in this case)
    vme->SetBidirectionalSearchs( bidir_mask,weight, true );//the arguments: a bit mask to disable specified searching modes (all disable in this case), weight, TRUE or FALSE flag to disallow the coexistence of forward, backward, and birectional MVs within an MB;
    vme->SetMotionSearchs( 2, motion_mask, 0, 32);//the arguments: must be 0, 1, 2 to specify interger-pel, half-pel, and quarter-pel motion searching,block types that will not be allowed in the partitioning, as an OR, field enables,the maximum number of MVs
    vme->SetIntraPredictions( 0,0,0 );//no
    vme->SetTransform8x8Support( 0 );//no

    if(!vmesetup){
        vmesetup = 1;
        for(int k=0; k<8; k++){
            vme->LoadOneSearchPathFourLutSets(k, &FullSP[0], &DummyLutMode[0][0], &DummyLutXY[0][0]);
        }
    }

    vme->RunVME(&vout,0);
    //vout.MVs:
    //Resulting motion vectors relative to the top-left corner of the
    //reference windows in quarter-pel
    //grids in coded order.
    switch(vout.MbMode){
    case MODE_INTER_16X16:
        m_ResMB[m_adr].MV[0][0].x = m_ResMB[m_adr].MV[0][1].x = m_ResMB[m_adr].MV[0][2].x = m_ResMB[m_adr].MV[0][3].x = vout.Mvs[0][0].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[0][0].y = m_ResMB[m_adr].MV[0][1].y = m_ResMB[m_adr].MV[0][2].y = m_ResMB[m_adr].MV[0][3].y = vout.Mvs[0][0].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[1][0].x = m_ResMB[m_adr].MV[1][1].x = m_ResMB[m_adr].MV[1][2].x = m_ResMB[m_adr].MV[1][3].x = vout.Mvs[0][1].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[1][0].y = m_ResMB[m_adr].MV[1][1].y = m_ResMB[m_adr].MV[1][2].y = m_ResMB[m_adr].MV[1][3].y = vout.Mvs[0][1].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MbPart = ME_Mb16x16;
        switch(vout.MbType & 0x1f)
        {
        case 1://TYPE_INTER_16X16_0:
            m_ResMB[m_adr].MbType = ME_MbFrw;
            m_ResMB[m_adr].MV[1][0].x = m_ResMB[m_adr].MV[1][1].x = m_ResMB[m_adr].MV[1][2].x = m_ResMB[m_adr].MV[1][3].x = m_MVDirectBW[0] ? m_MVDirectBW[0][m_adr].x : 0;
            m_ResMB[m_adr].MV[1][0].y = m_ResMB[m_adr].MV[1][1].y = m_ResMB[m_adr].MV[1][2].y = m_ResMB[m_adr].MV[1][3].y = m_MVDirectBW[0] ? m_MVDirectBW[0][m_adr].y : 0;
            break;
        case 2://TYPE_INTER_16X16_1:
            m_ResMB[m_adr].MbType = ME_MbBkw;
            m_ResMB[m_adr].MV[0][0].x = m_ResMB[m_adr].MV[0][1].x = m_ResMB[m_adr].MV[0][2].x = m_ResMB[m_adr].MV[0][3].x = m_MVDirectFW[0] ? m_MVDirectFW[0][m_adr].x : 0;
            m_ResMB[m_adr].MV[0][0].y = m_ResMB[m_adr].MV[0][1].y = m_ResMB[m_adr].MV[0][2].y = m_ResMB[m_adr].MV[0][3].y = m_MVDirectFW[0] ? m_MVDirectFW[0][m_adr].y : 0;
            break;
        case 3://TYPE_INTER_16X16_2:
            m_ResMB[m_adr].MbType = ME_MbBidir;
            break;
        default:
            assert(false);
        }
        break;
    case MODE_INTER_8X8:
        m_ResMB[m_adr].MV[0][0].x = vout.Mvs[0][0].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[0][0].y = vout.Mvs[0][0].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[0][1].x = vout.Mvs[1][0].x + ((refxy.x - x - 8)<<2);
        m_ResMB[m_adr].MV[0][1].y = vout.Mvs[1][0].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[0][2].x = vout.Mvs[2][0].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[0][2].y = vout.Mvs[2][0].y + ((refxy.y - y - 8)<<2);
        m_ResMB[m_adr].MV[0][3].x = vout.Mvs[3][0].x + ((refxy.x - x - 8)<<2);
        m_ResMB[m_adr].MV[0][3].y = vout.Mvs[3][0].y + ((refxy.y - y - 8)<<2);

        m_ResMB[m_adr].MV[1][0].x = vout.Mvs[0][1].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[1][0].y = vout.Mvs[0][1].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[1][1].x = vout.Mvs[1][1].x + ((refxy.x - x - 8)<<2);
        m_ResMB[m_adr].MV[1][1].y = vout.Mvs[1][1].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[1][2].x = vout.Mvs[2][1].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[1][2].y = vout.Mvs[2][1].y + ((refxy.y - y - 8)<<2);
        m_ResMB[m_adr].MV[1][3].x = vout.Mvs[3][1].x + ((refxy.x - x - 8)<<2);
        m_ResMB[m_adr].MV[1][3].y = vout.Mvs[3][1].y + ((refxy.y - y - 8)<<2);

        m_ResMB[m_adr].MbPart = ME_Mb8x8;
        m_ResMB[m_adr].MbType = ME_MbFrw;//no other cases for this version
        assert(vout.MbSubPredMode == 0);//SUBDIR_REF_0 for all blocks
        m_ResMB[m_adr].BlockType[0] = ME_BlockFrw;
        m_ResMB[m_adr].BlockType[1] = ME_BlockFrw;
        m_ResMB[m_adr].BlockType[2] = ME_BlockFrw;
        m_ResMB[m_adr].BlockType[3] = ME_BlockFrw;

        break;

    case MODE_INTER_SKIP:
        m_ResMB[m_adr].MV[0][0].x = m_ResMB[m_adr].MV[0][1].x = m_ResMB[m_adr].MV[0][2].x = m_ResMB[m_adr].MV[0][3].x = vout.Mvs[0][0].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[0][0].y = m_ResMB[m_adr].MV[0][1].y = m_ResMB[m_adr].MV[0][2].y = m_ResMB[m_adr].MV[0][3].y = vout.Mvs[0][0].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MV[1][0].x = m_ResMB[m_adr].MV[1][1].x = m_ResMB[m_adr].MV[1][2].x = m_ResMB[m_adr].MV[1][3].x = vout.Mvs[0][1].x + ((refxy.x - x)<<2);
        m_ResMB[m_adr].MV[1][0].y = m_ResMB[m_adr].MV[1][1].y = m_ResMB[m_adr].MV[1][2].y = m_ResMB[m_adr].MV[1][3].y = vout.Mvs[0][1].y + ((refxy.y - y)<<2);
        m_ResMB[m_adr].MbPart = ME_Mb16x16;
        if(m_par->SearchDirection == ME_ForwardSearch)
            m_ResMB[m_adr].MbType = ME_MbFrwSkipped;
        else
            m_ResMB[m_adr].MbType = ME_MbBidirSkipped;
        break;

    default:
        assert(false);
        return;
    }

    if(m_par->PixelType == ME_HalfPixel)
    {
        m_ResMB[m_adr].MV[0][0].x = (m_ResMB[m_adr].MV[0][0].x >> 1) << 1;
        m_ResMB[m_adr].MV[0][0].y = (m_ResMB[m_adr].MV[0][0].y >> 1) << 1;
        m_ResMB[m_adr].MV[0][1].x = (m_ResMB[m_adr].MV[0][1].x >> 1) << 1;
        m_ResMB[m_adr].MV[0][1].y = (m_ResMB[m_adr].MV[0][1].y >> 1) << 1;
        m_ResMB[m_adr].MV[0][2].x = (m_ResMB[m_adr].MV[0][2].x >> 1) << 1;
        m_ResMB[m_adr].MV[0][2].y = (m_ResMB[m_adr].MV[0][2].y >> 1) << 1;
        m_ResMB[m_adr].MV[0][3].x = (m_ResMB[m_adr].MV[0][3].x >> 1) << 1;
        m_ResMB[m_adr].MV[0][3].y = (m_ResMB[m_adr].MV[0][3].y >> 1) << 1;

        m_ResMB[m_adr].MV[1][0].x = (m_ResMB[m_adr].MV[1][0].x >> 1) << 1;
        m_ResMB[m_adr].MV[1][0].y = (m_ResMB[m_adr].MV[1][0].y >> 1) << 1;
        m_ResMB[m_adr].MV[1][1].x = (m_ResMB[m_adr].MV[1][1].x >> 1) << 1;
        m_ResMB[m_adr].MV[1][1].y = (m_ResMB[m_adr].MV[1][1].y >> 1) << 1;
        m_ResMB[m_adr].MV[1][2].x = (m_ResMB[m_adr].MV[1][2].x >> 1) << 1;
        m_ResMB[m_adr].MV[1][2].y = (m_ResMB[m_adr].MV[1][2].y >> 1) << 1;
        m_ResMB[m_adr].MV[1][3].x = (m_ResMB[m_adr].MV[1][3].x >> 1) << 1;
        m_ResMB[m_adr].MV[1][3].y = (m_ResMB[m_adr].MV[1][3].y >> 1) << 1;
    }
    else if(m_par->PixelType == ME_IntegerPixel)
    {
        m_ResMB[m_adr].MV[0][0].x = (m_ResMB[m_adr].MV[0][0].x >> 2) << 2;
        m_ResMB[m_adr].MV[0][0].y = (m_ResMB[m_adr].MV[0][0].y >> 2) << 2;
        m_ResMB[m_adr].MV[0][1].x = (m_ResMB[m_adr].MV[0][1].x >> 2) << 2;
        m_ResMB[m_adr].MV[0][1].y = (m_ResMB[m_adr].MV[0][1].y >> 2) << 2;
        m_ResMB[m_adr].MV[0][2].x = (m_ResMB[m_adr].MV[0][2].x >> 2) << 2;
        m_ResMB[m_adr].MV[0][2].y = (m_ResMB[m_adr].MV[0][2].y >> 2) << 2;
        m_ResMB[m_adr].MV[0][3].x = (m_ResMB[m_adr].MV[0][3].x >> 2) << 2;
        m_ResMB[m_adr].MV[0][3].y = (m_ResMB[m_adr].MV[0][3].y >> 2) << 2;

        m_ResMB[m_adr].MV[1][0].x = (m_ResMB[m_adr].MV[1][0].x >> 2) << 2;
        m_ResMB[m_adr].MV[1][0].y = (m_ResMB[m_adr].MV[1][0].y >> 2) << 2;
        m_ResMB[m_adr].MV[1][1].x = (m_ResMB[m_adr].MV[1][1].x >> 2) << 2;
        m_ResMB[m_adr].MV[1][1].y = (m_ResMB[m_adr].MV[1][1].y >> 2) << 2;
        m_ResMB[m_adr].MV[1][2].x = (m_ResMB[m_adr].MV[1][2].x >> 2) << 2;
        m_ResMB[m_adr].MV[1][2].y = (m_ResMB[m_adr].MV[1][2].y >> 2) << 2;
        m_ResMB[m_adr].MV[1][3].x = (m_ResMB[m_adr].MV[1][3].x >> 2) << 2;
        m_ResMB[m_adr].MV[1][3].y = (m_ResMB[m_adr].MV[1][3].y >> 2) << 2;
    }

    m_ResMB[m_adr].McType = ME_FrameMc;
    m_ResMB[m_adr].MbCosts[0] = vout.MinDist;
}

}//namespace UMC
#endif //ME_VME
