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

#include "umc_me.h"
#include "umc_me_cost_func.h"

#if defined(_MSC_VER)
#pragma warning( disable: 4189 )
#endif

namespace UMC_VC1_ENCODER
{
    extern const Ipp16u* VLCTableCBPCY_PB[4];
    extern const Ipp16u*  MVDiffTablesVLC[4];
}

using namespace UMC;
using namespace UMC_VC1_ENCODER;


void MePredictCalculator::Init(MeParams* pMeParams, MeCurrentMB* pCur)
{
    m_pMeParams = pMeParams;
    m_pCur = pCur;
    m_pRes = pMeParams->pSrc->MBs;
}

MeMV MePredictCalculator::GetPrediction()
{
    switch(m_pCur->MbPart){
        case ME_Mb16x16:
            return GetPrediction16x16();
        case ME_Mb8x8:
            return GetPrediction8x8();
    }

    return 0;
}

MeMV MePredictCalculator::GetPrediction(MeMV mv)
{
    return mv;
}

bool MePredictCalculator::IsOutOfBound(MeMbPart mt, MePixelType /*pix*/, MeMV mv)
{
    if( mv.x<-4*m_pMeParams->SearchRange.x|| mv.x>=4*m_pMeParams->SearchRange.x ||
        mv.y<-4*m_pMeParams->SearchRange.y || mv.y>=4*m_pMeParams->SearchRange.y)
        return true;

    Ipp32s w=4*16, h=4*16;
    switch(mt)
    {
//    case ME_Mb16x16:
//        w = 16;
//        h = 16;
//        break;
    case ME_Mb16x8:
        w = 4*16;
        h = 4*8;
        break;
    case ME_Mb8x16:
        w = 4*8;
        h = 4*16;
        break;
    case ME_Mb8x8:
        w = 4*8;
        h = 4*8;
        break;
    case ME_Mb4x4:
        w = 4*4;
        h = 4*4;
        break;
    case ME_Mb2x2:
        w = 4*2;
        h = 4*2;
        break;
    }

    mv.x=mv.x+(Ipp16s)(4*16*m_pCur->x);
    mv.y=mv.y+(Ipp16s)(4*16*m_pCur->y);

    if(mv.x<4*m_pMeParams->PicRange.top_left.x)return true;
    if(mv.y<4*m_pMeParams->PicRange.top_left.y)return true;
    if(mv.x>4*m_pMeParams->PicRange.bottom_right.x-w)return true;
    if(mv.y>4*m_pMeParams->PicRange.bottom_right.y-h)return true;
    return false;
}

void MePredictCalculator::TrimSearchRange(MeMbPart mt, MePixelType /*pix*/, Ipp32s &x0, Ipp32s &x1, Ipp32s &y0, Ipp32s &y1 )
{
    Ipp32s x=4*16*m_pCur->x;
    Ipp32s y=4*16*m_pCur->y;

    Ipp32s w=4*16, h=4*16;
    switch(mt)
    {
//    case ME_Mb16x16:
//        w = 16;
//        h = 16;
//        break;
    case ME_Mb16x8:
        w = 4*16;
        h = 4*8;
        break;
    case ME_Mb8x16:
        w = 4*8;
        h = 4*16;
        break;
    case ME_Mb8x8:
        w = 4*8;
        h = 4*8;
        break;
    case ME_Mb4x4:
        w = 4*4;
        h = 4*4;
        break;
    case ME_Mb2x2:
        w = 4*2;
        h = 4*2;
        break;
    }

    x0 = IPP_MAX(x0,IPP_MAX(4*m_pMeParams->PicRange.top_left.x-x, -4*m_pMeParams->SearchRange.x));
    x1 = IPP_MIN(x1,IPP_MIN((4*m_pMeParams->PicRange.bottom_right.x-w)+1-x, 4*m_pMeParams->SearchRange.x));
    y0= IPP_MAX(y0,IPP_MAX(4*m_pMeParams->PicRange.top_left.y-y, -4*m_pMeParams->SearchRange.y));
    y1= IPP_MIN(y1,IPP_MIN((4*m_pMeParams->PicRange.bottom_right.y-h)+1-y, 4*m_pMeParams->SearchRange.y));
}

MeMV MePredictCalculator::GetCurrentBlockMV(int isBkw, Ipp32s idx)
{
    return m_pCur->BestMV[isBkw][m_pCur->BestIdx[isBkw][idx]][idx];
}

Ipp32s MePredictCalculator::GetT2()
{
    switch(m_pCur->MbPart){
        case ME_Mb16x16:
            return GetT2_16x16();
        case ME_Mb8x8:
            return GetT2_8x8();
        //default:
            //SetError((vm_char*));
    }
    return 0; //forse full search
}

Ipp32s MePredictCalculator::GetT2_16x16()
{
    Ipp32s T2, SadA=ME_BIG_COST, SadB=ME_BIG_COST, SadC=ME_BIG_COST;
    Ipp32s adr=m_pCur->adr;
    Ipp32s w=m_pMeParams->pSrc->WidthMB;

    if(MbTopEnable && m_pRes[adr -w].MbType != ME_MbIntra)
        SadA = m_pRes[adr-w].MbCosts[0];
    if(MbTopRightEnable && m_pRes[adr-w+1].MbType != ME_MbIntra)
        SadB = m_pRes[adr-w+1].MbCosts[0];
    if(MbLeftEnable && m_pRes[adr-1].MbType != ME_MbIntra)
        SadC = m_pRes[adr-1].MbCosts[0];

    if(SadA == ME_BIG_COST && SadB == ME_BIG_COST && SadC == ME_BIG_COST)
    {
        T2 = -1;
    }
    else
    {
        T2 = 6*IPP_MIN(IPP_MIN(SadA,SadB), SadC)/5 + 128;
    }
    //T2 = IPP_MIN(IPP_MIN(SadA,SadB), IPP_MIN(SadC,SadCol));

    return T2;
}

Ipp32s MePredictCalculator::GetT2_8x8()
{
    Ipp32s T2, SadA=ME_BIG_COST, SadB=ME_BIG_COST, SadC=ME_BIG_COST;
    Ipp32s adr=m_pCur->adr;
    Ipp32s w=m_pMeParams->pSrc->WidthMB;

    switch(m_pCur->BlkIdx)
    {
    case 0:
        if (MbLeftEnable && m_pRes[adr-1].BlockType[1] != ME_MbIntra)
        {
            SadC = m_pRes[adr-1].MbCosts[2];
        }
        if (MbTopEnable && m_pRes[adr-w].BlockType[2] != ME_MbIntra)
        {
            SadA = m_pRes[adr-w].MbCosts[3];
        }
        if (MbTopLeftEnable && m_pRes[adr-w-1].BlockType[3] != ME_MbIntra)
        {
            SadB = m_pRes[adr-w-1].MbCosts[4];
        }
        else if(MbTopEnable && m_pRes[adr-w].BlockType[3] != ME_MbIntra)
        {
            SadB = m_pRes[adr-w].MbCosts[4];
        }
        break;
    case 1:
        SadC = m_pCur->InterCost[0];

        if (MbTopEnable && m_pRes[adr-w].BlockType[3] != ME_MbIntra)
        {
            SadA = m_pRes[adr-w].MbCosts[4];
        }
        if (MbTopRightEnable && m_pRes[adr-w+1].BlockType[2] != ME_MbIntra)
        {
            SadB = m_pRes[adr-w+1].MbCosts[3];
        }
        else if(MbTopEnable && m_pRes[adr-w].BlockType[2] != ME_MbIntra)
        {
            SadB = m_pRes[adr-w].MbCosts[3];
        }
        break;
    case 2:
        if (MbLeftEnable && m_pRes[adr-1].BlockType[3] != ME_MbIntra)
        {
            SadC = m_pRes[adr-1].MbCosts[4];
        }
        SadA = m_pCur->InterCost[0];;
        SadB = m_pCur->InterCost[1];
        break;
    case 3:
        SadC = m_pCur->InterCost[2];
        SadA = m_pCur->InterCost[1];
        SadB = m_pCur->InterCost[0];
        break;
    default:
        VM_ASSERT(0);
        break;
    }
    if(SadA == ME_BIG_COST && SadB == ME_BIG_COST && SadC == ME_BIG_COST)
    {
        T2 = -1;
    }
    else
    {
        T2 = (6*IPP_MIN(IPP_MIN(SadA,SadB), SadC)/5 + 32);///2;
    }
    return T2;
}

void MePredictCalculator::SetMbEdges()
{
    // Edge MBs definitions.

    if(m_pCur->x == 0 || m_pCur->adr == m_pMeParams->FirstMB)MbLeftEnable = false;
    else MbLeftEnable = true;

    if(m_pCur->x == m_pMeParams->pSrc->WidthMB-1 || m_pCur->adr == m_pMeParams->LastMB)
        MbRightEnable = false;
    else MbRightEnable = true;

    if(m_pCur->y == 0 || m_pCur->adr - m_pMeParams->pSrc->WidthMB < m_pMeParams->FirstMB)
    {
        MbTopEnable = MbTopLeftEnable = MbTopRightEnable = false;
    }
    else
    {
        MbTopEnable = MbTopLeftEnable = MbTopRightEnable = true;
        if(!MbLeftEnable)  MbTopLeftEnable  = false;
        if(!MbRightEnable) MbTopRightEnable = false;
    }
}

//*******************************************************************************************************
MeInitParams::MeInitParams()
{
    refPadding      = 0;
    SearchDirection = ME_BidirSearch;
    MbPart          = ME_Mb16x16;
    UseStatistics = true;
    UseDownSampling = true;

}

//*******************************************************************************************************
///Function sets default value to MeParams structure.
///See also MeParams::SetSearchSpeed
MeParams::MeParams()
{
    pSrc    = NULL;
    FirstMB = -1;  //for multy threading slice processing
    LastMB  = -1;

    for(Ipp32u i = 0; i < MAX_REF; i++)
    {
        pRefF[i] = NULL;
        pRefB[i] = NULL;
    }

    SearchRange.x           = 0;
    SearchRange.y           = 0;
    SearchDirection         = ME_ForwardSearch;
    Interpolation           = ME_VC1_Bilinear;
    MbPart                  = 0;
    PixelType               = ME_IntegerPixel;
    PicRange.top_left.x     = 0;
    PicRange.top_left.y     = 0;
    PicRange.bottom_right.x = 0;
    PicRange.bottom_right.y = 0;
    SkippedMetrics          = ME_Sad;
    Quant                   = 0;
    FRefFramesNum           = 1;
    BRefFramesNum           = 0;
    ProcessSkipped          = false;
    ProcessDirect           = false;
    FieldMCEnable           = false;
    UseVarSizeTransform = false;
    memset(&ScaleInfo,0,sizeof(MeVC1fieldScaleInfo));
    bSecondField            = false;

    UseFastInterSearch=false;
    UseDownSampledImageForSearch=false;
    UseFeedback=false;
    UpdateFeedback=false;
    UseFastFeedback=false;
    UseChromaForME = false;
    UseChromaForMD = false;
    FastChroma = false;
    ChangeInterpPixelType = false;
    SelectVlcTables=false;
    UseTrellisQuantization=false;
    UseTrellisQuantizationChroma=false;
    CbpcyTableIndex=0;
    MvTableIndex=0;
    AcTableIndex=0;
    
    OutCbpcyTableIndex=0;
    OutMvTableIndex=0;
    OutAcTableIndex=0;
}


///Function sets MeParams value to default presets. Next speed defined right now:
///   - -1 change nothing, this is some kind of expert mode. 
///   - 75 and more. Fast speed search. Only fastest features are used. Really bad quality.
///   - 25-74 Quality search. Best possible quality without full search.
///   - 0-24  Full search. Really slow.
///
///\param Speed search speed
void MeParams::SetSearchSpeed(Ipp32s Speed)
{
    SearchSpeed=Speed;
        //parse Speed parameter
    if(SearchSpeed<0)
        return;

    if(SearchSpeed >= 75)
    {// paranoid speed
        UseFastInterSearch = true;
        //UseDownSampledImageForSearch = true;
    }
    else if(SearchSpeed >= 25)
    { // quality
    
#ifdef ME_DEBUG
//        if(SearchSpeed == 25) SelectVlcTables=true;
//        else SelectVlcTables=false;
        if(SearchSpeed == 29){
            SelectVlcTables=true;            UseTrellisQuantization =true;
        }else if(SearchSpeed == 28){
            SelectVlcTables=false;            UseTrellisQuantization =false;
        }else if(SearchSpeed == 27){
            SelectVlcTables=false;            UseTrellisQuantization =true;
        }else if(SearchSpeed == 26){
            SelectVlcTables=true;            UseTrellisQuantization =false;
        }else if(SearchSpeed == 25){
            SelectVlcTables=true;            UseTrellisQuantization =true;
        }else{
            UseTrellisQuantization=false;   UseTrellisQuantization =false;
        }
        UseTrellisQuant = UseTrellisQuantization;
#endif
        //SelectVlcTables=false;
        UseFastInterSearch = false;
        UseDownSampledImageForSearch = true;
    }
    else 
    { // paranoid quality
        UseFastInterSearch = false;
        UseDownSampledImageForSearch = false;
    }
}


//*******************************************************************************************************
void MeCurrentMB::Reset()
{
    //set costs
    for(Ipp32s i=0; i<ME_NUM_OF_BLOCKS; i++){
        for(Ipp32s j=0; j<ME_NUM_OF_REF_FRAMES; j++){
        }
        InterCost[i]=ME_BIG_COST;
        IntraCost[i]=ME_BIG_COST;
        SkipCost[i]=ME_BIG_COST;
    }
    DirectCost=ME_BIG_COST;
    VlcTableSearchInProgress=false;
}

void MeCurrentMB::ClearMvScoreBoardArr(){
        //ippsZero_8u(&mvEstimateDoneArr[0][0],128*64);
        memset(mvScoreBoardArr,0,128*64);
}
Ipp32s MeCurrentMB::CheckScoreBoardMv(MePixelType pix,MeMV mv) {
    if(mvScoreBoardFlag) {
        if(pix==ME_IntegerPixel) {
            if(mvScoreBoardArr[(mv.x>>2)+64][(mv.y>>2)+32] == 0){
                mvScoreBoardArr[(mv.x>>2)+64][(mv.y>>2)+32] = 1;
            } else {
                return 1;
            }
        }
    }
    return 0;
}
void MeCurrentMB::SetScoreBoardFlag(){
    mvScoreBoardFlag = 1;
}
void MeCurrentMB::ClearScoreBoardFlag(){
    mvScoreBoardFlag = 0;
}

void MeMB::Reset()
{
    NumOfNZ=0;
}

void MeMB::Reset(Ipp32s numOfMVs)
{
    NumOfNZ=0;

    for (Ipp32s i = 0; i < 2; i++)
        for (Ipp32s j = 0; j < numOfMVs; j++)
        {
            MV[i][j].SetInvalid();
            MVPred[i][j].SetInvalid();
            Refindex[i][j] = 0;
        }
    for (Ipp32s j = 0; j < numOfMVs + 1; j++)
        MbCosts[j] = ME_BIG_COST;
    
    MbPart = 0;
    BlockPart = 0;
    MbType = 0;
    McType = 0;
    BlockTrans = 0;
    memset(BlockType, 0, sizeof(BlockType));
    memset(predType, 0, sizeof(predType));
}

//function converts 64 coefficients at once
//transf - block of transformed coefficients
//trellis - block of trellis quantized coefficients
//rc - block of round control values
//to quantize with round control value use (transf[i] + rc[i])/QP
void MeMB::CnvertCoeffToRoundControl(Ipp16s *transf, Ipp16s *trellis, Ipp16s *rc, Ipp32s QP)
{
    for(Ipp32s i=0; i<64; i++){
        rc[i]=(Ipp16s)(QP*trellis[i]-transf[i]);    
    }
}


//*******************************************************************************************************
MeBase::MeBase()
{
#ifdef ME_DEBUG
    CbpcyTableIndex=0;
    MvTableIndex=0;
    AcTableIndex=0;
#endif

    m_AllocatedSize = 0;
    m_OwnMem = NULL;
    m_CurRegSet = ME_RG_EMPTY_SET;

    #ifdef ME_USE_BACK_DOOR_LAMBDA
        Ipp32s array[8];
        FILE * InPar = fopen("MeParams.bin","rb");
        if(InPar == NULL){
            array[0] = 1;
            array[1] = 1;
        } else{
            fread(&array[0], sizeof(array), 1, InPar);
            fclose(InPar);
        }        
        m_lambda = array[0];
        //IntraCoeff=m_lambda;
        m_lambda /= 100.;
        printf("lambda=%f\n", m_lambda);
    #endif

}

MeBase::~MeBase()
{
    Close();
#ifdef ME_DEBUG
    delete pStat;
#endif    

};

///First version of Init. It allocates memory by malloc. To free memory call Close or destroy object.
///Some basic checks for passed parameters are performed. If some of requested modes are not
///supported function returns false. Initialization parameters are saved inside MeBase and used later in 
///EstimateFrame to check MeParams. They should not violate initialization agreement. If, for example,
///memory was not allocated for downsampled images in Init, any attempt to perform search on downsampled
///image will lead to immediate return from EstimateFrame with false.
///\param par initialization parameters
bool MeBase::Init(MeInitParams *par)
{
    m_OwnMem=NULL;
    Ipp32s size=0;

    if(!Init(par, (Ipp8u*)m_OwnMem, size))return false;
    m_OwnMem = malloc(size);
    if(m_OwnMem==NULL)return false;
    return Init(par, (Ipp8u*)m_OwnMem, size);
}

///Second version of init. Intended for memory allocation from external buffer. Two subsequent calls are needed
///for proper allocation. First one with NULL pointer to calculate necessary memory size, second one to memory allocation.
///See also first Init.
///\param par initialization parameter
///\param ptr pointer to external buffer or NULL to calculate memory necessary  size
///\param size size of external buffer or necessary amount of memory
bool MeBase::Init(MeInitParams *par, Ipp8u* ptr, Ipp32s &size)
{

    //1 workaround
    par->UseStatistics=true;
    par->UseDownSampling=true;
    //1 end of workaround


    bool aloc=(ptr != NULL);
    if(aloc && (m_AllocatedSize==0 || m_AllocatedSize<size)){
        SetError((vm_char*)"Wrong buffer size in MeBase::Init");
        return false;
    }
    //Ipp32s NumOfMVs = par->MbPart == ME_Mb16x16?1:4;
    Ipp32s NumOfMVs = 4;
    Ipp32s NumOfMBs = par->HeightMB*par->WidthMB;

    m_InitPar = *par;

    par->pFrames = (MeFrame*)ptr;
    ptr+=par->MaxNumOfFrame*sizeof(MeFrame);

    for(Ipp32s fr = 0; fr < par->MaxNumOfFrame; fr++){
        if(aloc)par->pFrames[fr].NumOfMVs=NumOfMVs;
        if(par->SearchDirection == ME_BidirSearch){
            if(aloc) par->pFrames[fr].MVDirect = (MeMV*)ptr; ptr+=NumOfMBs*sizeof(MeMV);
            if(aloc) par->pFrames[fr].RefIndx = (Ipp32s*)ptr; ptr+=NumOfMBs*sizeof(Ipp32s);
        }else{
            if(aloc) par->pFrames[fr].MVDirect = (MeMV*)(par->pFrames[fr].RefIndx = (Ipp32s*)NULL);
        }

        //MB
        if(aloc) par->pFrames[fr].MBs = (MeMB*)ptr; ptr += NumOfMBs*sizeof(MeMB);
        for(Ipp32s m=0; m<NumOfMBs; m++)
        {
            if(aloc) par->pFrames[fr].MBs[m].MV[frw] = (MeMV*)ptr; ptr+=NumOfMVs*sizeof(MeMV);
            if(aloc) par->pFrames[fr].MBs[m].MV[bkw] = (MeMV*)ptr; ptr+=NumOfMVs*sizeof(MeMV);
            if(aloc) par->pFrames[fr].MBs[m].Refindex[frw] = (Ipp32s*)ptr; ptr+=NumOfMVs*sizeof(Ipp32s);
            if(aloc) par->pFrames[fr].MBs[m].Refindex[bkw] = (Ipp32s*)ptr; ptr+=NumOfMVs*sizeof(Ipp32s);
            if(aloc) par->pFrames[fr].MBs[m].MbCosts = (Ipp32s*)ptr; ptr+=(NumOfMVs+1)*sizeof(Ipp32s);
            if(aloc) par->pFrames[fr].MBs[m].MVPred[frw] = (MeMV*)ptr; ptr+=NumOfMVs*sizeof(MeMV);
            if(aloc) par->pFrames[fr].MBs[m].MVPred[bkw] = (MeMV*)ptr; ptr+=NumOfMVs*sizeof(MeMV);
        }

        //stat
        if(par->UseStatistics){
            if(aloc) par->pFrames[fr].stat = (MeMbStat*)ptr; ptr += NumOfMBs*sizeof(MeMbStat);
            for(Ipp32s m=0; m<NumOfMBs; m++){
                if(aloc) par->pFrames[fr].stat[m].MVF = (Ipp16u*)ptr; ptr+=NumOfMVs*sizeof(Ipp16u);
                if(aloc) par->pFrames[fr].stat[m].MVB = (Ipp16u*)ptr; ptr+=NumOfMVs*sizeof(Ipp16u);
                if(aloc) par->pFrames[fr].stat[m].coeff = (Ipp16u*)ptr; ptr+=6*sizeof(Ipp16u);
            }
        }else{
            if(aloc) par->pFrames[fr].stat = NULL;
        }

        //plane allocation including downsampling
        if(aloc) par->pFrames[fr].plane[0].clear();
        for(Ipp32s lev=1; lev<4; lev++){
            if(aloc) par->pFrames[fr].plane[lev].clear();
            if(par->UseDownSampling){
                Ipp32s scale=1<<lev;
                Ipp32s height = (16*par->HeightMB+2*par->refPadding+scale-1)/scale;
                Ipp32s width = (16*par->WidthMB+2*par->refPadding+scale-1)/scale;
                if(aloc) par->pFrames[fr].plane[lev].set(Y, ptr+(width+1)*(par->refPadding/scale), width);
                ptr += width*height;
            }
        }
    }
    //allocate different intermediate buffers
    //interpolation
    Ipp32s dim;
    for(Ipp32s dir=0; dir<3; dir++){
        dim=16;
        for(Ipp32s ch=0; ch<3; ch++){
            if(aloc) m_cur.buf[dir].set(ch,(Ipp8u*)ptr,dim); ptr+=dim*dim;
            if(ch==0)dim/=2;
        }
    }
    

    //get size
    if(!aloc)m_AllocatedSize=size=(Ipp32s)(ptr-(Ipp8u*)(NULL));        


#ifdef ME_DEBUG
    pStat = new MeStat(par->WidthMB, par->HeightMB);
#endif    

#ifdef OTL
    otl = fopen("otlad.txt","wt");
    assert(otl);
#endif

    return true;
}

int ParFileQpToMeQP(int x, bool &uniform)
{
/*
Uniform
x1
2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,  //par file
2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,  //ME

NonUniform
x2
18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56   //par file
12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50   //ME
x4
58,60,62  //par file 
54,58,62  //ME

*/
    if(x<=17){uniform=true; return x;}
    if(x<=56){uniform=false; return x-6;}
    uniform=false;
    return 2*(x-58) + 54;
}

///Perform motion estimation for one frame or slice.
///See also MeBase::Init and MeParams descriptions.
bool MeBase::EstimateFrame(MeParams *par)
{
    bool ChangeInterpPixelType = par->ChangeInterpPixelType && par->SearchDirection == ME_ForwardSearch;

    if(ChangeInterpPixelType){
        //interpolation selection
        MeParams CurPar=*par;
        CurPar.UseFastInterSearch = true;
        CurPar.UseFeedback = false;
        memset(CostOnInterpolation,0,sizeof(CostOnInterpolation));
        Ipp32s max_j = 2;
        for(int j = 0; j < max_j; j++)
        {
            switch(j)
            {
            case 0:
                CurPar.Interpolation = ME_VC1_Bilinear;
                CurPar.PixelType = ME_HalfPixel;
                break;
            case 1:
                CurPar.Interpolation = ME_VC1_Bicubic;
                CurPar.PixelType = ME_QuarterPixel;
                break;
            default:
                VM_ASSERT(0);
            }
            EstimateFrameInner(&CurPar);
        }

        SetInterpPixelType();
        par->Interpolation=par->OutInterpolation=m_par->OutInterpolation;
        par->PixelType=par->OutPixelType=m_par->OutPixelType;;
    }

    return EstimateFrameInner(par);
}

bool MeBase::EstimateFrameInner(MeParams *par)
{
#ifdef ME_DEBUG
    NumOfHibrids=0;
    MeQP = par->Quant;
    MeQPUNF=par->UniformQuant;
    MeLambda=100*m_lambda;

    //1 workaround
    par->ChromaInterpolation=ME_VC1_Bilinear;
    par->CbpcyTableIndex = CbpcyTableIndex;
    par->MvTableIndex = MvTableIndex;
    par->AcTableIndex = AcTableIndex;
    //1 end of workaround
#endif


    //check input params, init some variables
    m_par = par;
    if(!CheckParams())
        return false;

    DownsampleFrames();

    //init feedback
    LoadRegressionPreset();

    //init auxiliary classes
    m_PredCalc->Init(par, &m_cur);

    //VLC selection
    for(Ipp32s i=0; i<4; i++){
        m_cur.CbpcyRate[i]=0;
        m_cur.MvRate[i]=0;
        m_cur.AcRate[i]=0;
    }

    //estiamte all MB
    for(Ipp32s MbAdr = m_par->FirstMB; MbAdr <= m_par->LastMB; MbAdr++)
    {
        //reset costs
        m_cur.Reset();
        m_cur.y = MbAdr/par->pSrc->WidthMB;
        m_cur.x = MbAdr%par->pSrc->WidthMB;
        m_cur.adr = MbAdr;

        m_par->pSrc->MBs[m_cur.adr].Reset();
        // TODO: rewrite this
        if(m_cur.adr!=0)m_par->pSrc->MBs[m_cur.adr].NumOfNZ=m_par->pSrc->MBs[m_cur.adr-1].NumOfNZ;
        

        //set ptr to source MB
//        m_cur.SrcStep[Y] = m_par->pSrc->step[Y];
//        m_cur.pSrc[Y] = m_par->pSrc->ptr[Y]+16*m_cur.y*m_cur.SrcStep[Y]+16*m_cur.x;

        //choose estimation function
        if(m_par->MbPart == ME_Mb16x16)
        {
            EstimateMB16x16();
        }
        else if(m_par->MbPart == (ME_Mb8x8 | ME_Mb16x16))
        {
            EstimateMB8x8();
        }
        else
        {
            SetError((vm_char*)"Unsupported partitioning");
        }
        UpdateVlcTableStatistic();
    }
    MakeVlcTableDecision();

#ifdef ME_DEBUG
    //1 workaround
    CbpcyTableIndex=par->OutCbpcyTableIndex;
    MvTableIndex=par->OutMvTableIndex;
    AcTableIndex=par->OutAcTableIndex;
    //1 end of workaround
#endif
    return true;
}

void MeBase::EstimateMB16x16()
{
    //set block index and type, for 16x16 it is always 0 & ME_Mb16x16
    m_cur.BlkIdx=0;
    m_cur.MbPart = ME_Mb16x16;

    m_PredCalc->ResetPredictors();

    for(;;){
        if(EstimateSkip()) break;
        if(Estimate16x16Direct()) break;

        Ipp32s j = 0;

        for(Ipp32s i=0; i<m_par->FRefFramesNum+m_par->BRefFramesNum; i++){
            if(i<m_par->FRefFramesNum){
                j = i;
                SetReferenceFrame(frw,i);
            }else{
                j = i-m_par->FRefFramesNum;
                SetReferenceFrame(bkw,j);
            }

            m_cur.PredMV[m_cur.RefDir][j][m_cur.BlkIdx]=m_PredCalc->GetPrediction();
            EstimateInter(); //this is MV search, and inter cost calculation
        }
        Estimate16x16Bidir();//here bidir costs is calculated
        EstimateIntra();
        break;
    }

    MakeMBModeDecision16x16();
}

void MeBase::EstimateMB8x8()
{
    //estimate 16x16, exit search if result is good enough
    EstimateMB16x16();
    //if(EarlyExitAfter16x16()) return;

    m_PredCalc->ResetPredictors();
    //reset costs
    m_cur.Reset();

    //for all block in MB
    m_cur.MbPart = ME_Mb8x8;
    for(m_cur.BlkIdx=0; m_cur.BlkIdx<4; m_cur.BlkIdx++){
        for(;;){
            if(EstimateSkip()) break;
            Ipp32s j = 0;

            for(Ipp32s i=0; i<m_par->FRefFramesNum+m_par->BRefFramesNum; i++){
                if(i<m_par->FRefFramesNum){
                    j = i;
                    SetReferenceFrame(frw,i);
                }else{
                    j = i-m_par->FRefFramesNum;
                    SetReferenceFrame(bkw,j);
                }

                m_cur.PredMV[m_cur.RefDir][j][m_cur.BlkIdx]=m_PredCalc->GetPrediction();
                EstimateInter(); //this is MV search, and inter cost calculation
            }
            EstimateIntra();
            break;
        }
        MakeBlockModeDecision();
    }

    MakeMBModeDecision8x8();
}

bool MeBase::EarlyExitForSkip()
{
    Ipp32s thr=m_SkippedThreshold;
    if(m_cur.MbPart == ME_Mb8x8)
        thr/=4;

    if(m_cur.SkipCost[m_cur.BlkIdx]<thr)
        return true;

    return false;
}


void MeBase::EstimateInter()
{
    //reset intermediate cost
    m_cur.BestCost =ME_BIG_COST;

    //search
     if(m_par->UseFastInterSearch){
        EstimateMbInterFast();
     }else if(m_par->UseDownSampledImageForSearch){
         EstimateMbInterFullSearchDwn();
    }else{
         EstimateMbInterFullSearch();
    }

    //check if found MV is better
//    if(!m_par->UseFeedback)
//    {
    if(m_cur.BestCost<m_cur.InterCost[m_cur.BlkIdx]){
        m_cur.InterCost[m_cur.BlkIdx]=m_cur.BestCost;
        if(m_cur.RefDir == frw){
            m_cur.InterType[m_cur.BlkIdx] = ME_MbFrw;
            m_cur.BestIdx[frw][m_cur.BlkIdx] = m_cur.RefIdx;
            m_cur.BestIdx[bkw][m_cur.BlkIdx] = -1;
        }else{
            m_cur.InterType[m_cur.BlkIdx] = ME_MbBkw;
            m_cur.BestIdx[frw][m_cur.BlkIdx] = -1;
            m_cur.BestIdx[bkw][m_cur.BlkIdx] = m_cur.RefIdx;
        }
    }
//}
}


void MeBase::Estimate16x16Bidir()
{
    if(m_par->SearchDirection != ME_BidirSearch)
        return;

    if(m_par->UseFeedback)
        return;

    //find best bidir
    for(Ipp32s i=0; i< m_par->FRefFramesNum; i++){
        for(Ipp32s j=0; j<m_par->BRefFramesNum; j++){
            //check bidir
            Ipp32s tmpCost=EstimatePointAverage(ME_Mb16x16, m_par->PixelType, m_par->CostMetric, frw, i, m_cur.BestMV[frw][i][0], bkw, j, m_cur.BestMV[bkw][j][0]);
            tmpCost +=WeightMV(m_cur.BestMV[frw][i][0], m_cur.PredMV[frw][i][0]);
            tmpCost +=WeightMV(m_cur.BestMV[bkw][j][0], m_cur.PredMV[bkw][j][0]);
            tmpCost +=32; //some additional penalty for bidir type of MB
            if(tmpCost<m_cur.InterCost[0]){
                m_cur.InterCost[0] = tmpCost;
                m_cur.InterType[0] = ME_MbBidir;
                m_cur.BestIdx[frw][0] = i;
                m_cur.BestIdx[bkw][0] = j;
            }
        }
    }
}


bool MeBase::Estimate16x16Direct()
{
    if(!m_par->ProcessDirect)
        return false;
    if(m_par->SearchDirection != ME_BidirSearch)
        return false;

    MeMV MVDirectFW, MVDirectBW;
    Ipp32s IdxF, IdxB;

    for(IdxF = 0; IdxF < m_par->FRefFramesNum; IdxF++)
        if(!m_par->pRefF[IdxF]->MVDirect[m_cur.adr].IsInvalid())
        {
            break;
        }

    for(IdxB = 0; IdxB < m_par->BRefFramesNum; IdxB++)
        if(!m_par->pRefB[IdxB]->MVDirect[m_cur.adr].IsInvalid())
        {
            break;
        }

    if(IdxF == m_par->FRefFramesNum || IdxB == m_par->BRefFramesNum)
        SetError((vm_char*)"Wrong MVDirect in MeBase::Estimate16x16Direct");

    m_cur.DirectIdx[frw] = IdxF;
    m_cur.DirectIdx[bkw] = IdxB;

    if(m_par->UseFeedback)
        return false;

    MVDirectFW = m_par->pRefF[IdxF]->MVDirect[m_cur.adr];
    MVDirectBW = m_par->pRefB[IdxB]->MVDirect[m_cur.adr];

    //check direct
    Ipp32s tmpCost=EstimatePointAverage(ME_Mb16x16, m_par->PixelType, m_par->CostMetric,
                                        frw, IdxF, MVDirectFW,
                                        bkw, IdxB, MVDirectBW);
    m_cur.DirectCost     = tmpCost;
    m_cur.DirectType     = ME_MbDirect;
    m_cur.DirectIdx[frw] = IdxF;
    m_cur.DirectIdx[bkw] = IdxB;
    

    if(m_cur.DirectCost<m_SkippedThreshold){
        m_cur.DirectType= ME_MbDirectSkipped;
        return true;
    }

    return false;
}


//should be called after EstimateInter.
void MeBase::EstimateIntra()
{
    // TODO: move intra cost calculation from MakeMBModeDecision16x16ByFB here
    if(m_par->UseFeedback) return;

    Ipp32s i = m_cur.BlkIdx;
    if(m_cur.InterCost[i]>=ME_BIG_COST)
        return; //inter estimation was skiped, skip intra as well
    
    //usually Inter and Intra costs calculated by incompatible metrics, so recalculate inter cost
    Ipp32s InterCost;
    if(m_cur.InterType[i] != ME_MbBidir){
        //frw or bkw
        Ipp32s dir = (m_cur.InterType[i] == ME_MbFrw)?frw:bkw;
        SetReferenceFrame(dir, m_cur.BestIdx[dir][i]);
        InterCost=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sadt, m_cur.BestMV[dir][m_cur.BestIdx[dir][i]][i]);
    }else{
        //bidir
        InterCost=EstimatePointAverage(m_cur.MbPart, m_par->PixelType, ME_Sadt,
                    frw, m_cur.BestIdx[frw][i], m_cur.BestMV[frw][  m_cur.BestIdx[frw][i]  ][i],
                    bkw, m_cur.BestIdx[bkw][i], m_cur.BestMV[bkw][  m_cur.BestIdx[bkw][i]  ][i]);
    }

    // TODO: check if there is adjusted intra block, for both 16x16 and 8x8
    MeDecisionMetrics met=ME_SadtSrc;
    Ipp32s IntraCost = EstimatePoint(m_cur.MbPart, m_par->PixelType, met, 0);

    //calculate fake Intra cost
    if(IntraCost<InterCost)
        m_cur.IntraCost[i] = m_cur.InterCost[i]-1;
    else
        m_cur.IntraCost[i] = m_cur.InterCost[i]+1;
}

void MeBase::MakeVlcTableDecision()
{
    if(!m_par->SelectVlcTables)
        return;
    
    //choose tables
    Ipp32s BestPat = IPP_MIN(IPP_MIN(m_cur.CbpcyRate[0],m_cur.CbpcyRate[1]),IPP_MIN(m_cur.CbpcyRate[2],m_cur.CbpcyRate[3]));
    Ipp32s BestMV  = IPP_MIN(IPP_MIN(m_cur.MvRate[0],m_cur.MvRate[1]),IPP_MIN(m_cur.MvRate[2],m_cur.MvRate[3]));
    Ipp32s BestAC  = IPP_MIN(IPP_MIN(m_cur.AcRate[0],m_cur.AcRate[1]),IPP_MIN(m_cur.AcRate[2],m_cur.AcRate[3]));
    for(Ipp32s idx=0; idx<4; idx++)
    {
        if(m_cur.CbpcyRate[idx] == BestPat)
            m_par->OutCbpcyTableIndex = idx;

        if(m_cur.MvRate[idx] == BestMV)
            m_par->OutMvTableIndex = idx;

        if(idx != 3 && m_cur.AcRate[idx] == BestAC)
            m_par->OutAcTableIndex = idx;
    }

    //CbpcyTableIndex=3&rand();
    //MvTableIndex=3&rand();
    //AcTableIndex=rand()%3;
}

void MeBase::UpdateVlcTableStatistic()
{
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    bool intra = ((MeMbType)(mb->MbType) == ME_MbIntra);

    if(!m_par->SelectVlcTables)
        return;

    if(mb->MbType != (Ipp8u)ME_MbIntra && mb->MbType != (Ipp8u)ME_MbFrw)
        return;

    if(m_par->UseTrellisQuantization)m_cur.VlcTableSearchInProgress = true;
    //for all table set
    for(Ipp32s idx=0; idx<4; idx++){
        //select tables
        m_cur.CbpcyTableIndex=m_cur.MvTableIndex=m_cur.AcTableIndex=idx;
        if(idx==3)m_cur.AcTableIndex=2;

        //3 AC tables
        MeMV mv;
        mv = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0];
        MeMV pred = m_PredCalc->GetPrediction(mv);

        MeAdr src, ref; src.chroma=ref.chroma=true; //true means load also chroma plane addresses
        MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv, &ref);
        MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);
        //MeCostRD tmp= GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);


        MeCostRD tmp=0;
        for(Ipp32s blk=0; blk<6;blk++){
            src.channel = (blk<4?Y:(blk==4?U:V));
            m_cur.BlkIdx = blk; //it is for DC prediction calculation
            if(intra)
                tmp+=GetCostRD(ME_IntraRD, ME_Mb8x8, ME_Tranform8x8, &src, NULL);  //can be called as 16x16
            else
                tmp+=GetCostRD(ME_InterRD, ME_Mb8x8, m_cur.InterTransf[blk], &src, &ref);
            Ipp32s dx=8, dy=0;
            if(blk==1){dx=-dx; dy=8;}
            src.move(Y,dx,dy);
            if(!intra)ref.move(Y,dx,dy);
        }
        m_cur.BlkIdx =0;

        m_cur.AcRate[idx] += tmp.R;
        
        //3 CBPCY tables
        const Ipp16u*  pCBPCYTable=VLCTableCBPCY_PB[m_cur.CbpcyTableIndex];
        if(tmp.BlkPat!=0){
            m_cur.CbpcyRate[idx]+=pCBPCYTable[2*tmp.BlkPat+1];
        }
        //if(!intra&&m_cur.HybridPredictor)m_cur.CbpcyRate[idx]++;  //1 just for testing!
#ifdef ME_DEBUG
        if(!intra&&m_cur.HybridPredictor)NumOfHibrids++;
#endif
        //3 MV tables
        if(!intra){
            m_par->pSrc->MBs[m_cur.adr].NumOfNZ=tmp.NumOfCoeff;
            m_cur.MvRate[idx]+=GetMvSize(mv.x-pred.x,mv.y-pred.y);
        }else{
            Ipp32s         NotSkip =   (tmp.BlkPat != 0)?1:0;
            const Ipp16u*   MVDiffTables=MVDiffTablesVLC[m_cur.MvTableIndex];
            m_cur.MvRate[idx]+=MVDiffTables[2*(37*NotSkip + 36 - 1)+1];
        }
    }       

    //restore original tables
    m_cur.CbpcyTableIndex = m_par->CbpcyTableIndex;
    m_cur.MvTableIndex = m_par->MvTableIndex;
    m_cur.AcTableIndex = m_par->AcTableIndex;
    m_cur.VlcTableSearchInProgress = false;
}

///Function select best transform mode for Inter MB and return its cost.
///It is standard dependend functionaliry, so see derived class for implementation.
MeCostRD MeBase::MakeTransformDecisionInter(MeMbPart /*mt*/, MePixelType /*pix*/, MeMV /*mv*/)
{
    MeCostRD cost;
    cost.R=ME_BIG_COST;
    cost.D=ME_BIG_COST;
    return cost;
}

void MeBase::MakeMBModeDecision16x16()
{
    if(!m_par->UseFeedback)
        MakeMBModeDecision16x16Org();
    else
        MakeMBModeDecision16x16ByFB();
}


void MeBase::MakeMBModeDecision16x16Org()
{
    if(m_par->SearchDirection == ME_ForwardSearch){
        Ipp32s bestCost = ME_BIG_COST;
        if(m_cur.SkipCost[0] < m_SkippedThreshold){
            //skip
            m_cur.BestIdx[frw][0] = m_cur.SkipIdx[frw][0];
            m_cur.BestIdx[bkw][0] = -1;
            SetMB16x16B(m_cur.SkipType[0], m_cur.PredMV[frw][m_cur.SkipIdx[frw][0]][0], 0, m_cur.SkipCost[0]);
            bestCost = m_cur.SkipCost[0];
        }else if(m_cur.IntraCost[0]<m_cur.InterCost[0]){
            //intra
            SetMB16x16B(ME_MbIntra, 0, 0, ME_BIG_COST);
            bestCost = m_cur.InterCost[0]+1;
        }else{
            //inter
            SetMB16x16B(ME_MbFrw, m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0], 0, m_cur.InterCost[0]);
            SetError((vm_char*)"Wrong decision in MakeMBModeDecision16x16", m_cur.InterCost[0]>=ME_BIG_COST);
            bestCost = m_cur.InterCost[0];
        }
        if(m_par->ChangeInterpPixelType)
        {
            Ipp32s index = m_par->Interpolation + (m_par->PixelType >> 1);
            CostOnInterpolation[index] += bestCost;
        }
    }else{
        //Bidir
        MeMV mvF, mvB;
        if(m_par->PredictionType != ME_VC1Field2)
        {
            mvF = m_par->pRefF[m_cur.RefIdx]->MVDirect[m_cur.adr];
            mvB = m_par->pRefB[m_cur.RefIdx]->MVDirect[m_cur.adr];
            m_cur.BestIdx[frw][0] = 0;
            m_cur.BestIdx[bkw][0] = 0;
        }
        else //if(m_par->PredictionType == ME_VC1Field2)
        {
            mvF = m_PredCalc->GetDef_FieldMV(frw,0);
            mvB = m_PredCalc->GetDef_FieldMV(bkw,0);
            m_cur.BestIdx[frw][0] = (m_PredCalc->GetDef_Field(frw,0) == false? 0 : 1);
            m_cur.BestIdx[bkw][0] = (m_PredCalc->GetDef_Field(bkw,0) == false? 0 : 1);
        }

        if(m_cur.SkipCost[0] < m_SkippedThreshold){
            //skip
            switch(m_cur.SkipType[0]){
                case ME_MbFrwSkipped:
                    m_cur.BestIdx[frw][0] = m_cur.SkipIdx[frw][0];
                    mvF = m_cur.PredMV[frw][m_cur.SkipIdx[frw][0]][0];
               break;
                case ME_MbBkwSkipped:
                    m_cur.BestIdx[bkw][0] = m_cur.SkipIdx[bkw][0];
                    mvB = m_cur.PredMV[bkw][m_cur.SkipIdx[bkw][0]][0];
               break;
                case ME_MbBidirSkipped:
                    m_cur.BestIdx[frw][0] = m_cur.SkipIdx[frw][0];
                    m_cur.BestIdx[bkw][0] = m_cur.SkipIdx[bkw][0];;
                    mvF = m_cur.PredMV[frw][m_cur.SkipIdx[frw][0]][0];
                    mvB = m_cur.PredMV[bkw][m_cur.SkipIdx[bkw][0]][0];
               break;
            }
            SetMB16x16B(m_cur.SkipType[0], mvF, mvB, m_cur.SkipCost[0]);
        }else if(m_cur.DirectCost<m_SkippedThreshold ||m_cur.DirectCost<m_cur.InterCost[0]){
            //direct or direct skip
            m_cur.BestIdx[frw][0] = m_cur.DirectIdx[frw];
            m_cur.BestIdx[bkw][0] = m_cur.DirectIdx[bkw];;
            mvF = m_par->pRefF[m_cur.DirectIdx[frw]]->MVDirect[m_cur.adr];
            mvB = m_par->pRefB[m_cur.DirectIdx[bkw]]->MVDirect[m_cur.adr];
            SetMB16x16B(m_cur.DirectType, mvF, mvB, m_cur.DirectCost);
        }else if(m_cur.IntraCost[0]<m_cur.InterCost[0]){
            //intra
            SetMB16x16B(ME_MbIntra, 0, 0, ME_BIG_COST);
        }else{
            //Inter
            switch(m_cur.InterType[0]){
                case ME_MbFrw:
                    mvF = m_cur.BestMV[frw][  m_cur.BestIdx[frw][0]  ][0];
               break;
                case ME_MbBkw:
                    mvB = m_cur.BestMV[bkw][  m_cur.BestIdx[bkw][0]  ][0];
               break;
                case ME_MbBidir:
                    mvF = m_cur.BestMV[frw][  m_cur.BestIdx[frw][0]  ][0];
                    mvB = m_cur.BestMV[bkw][  m_cur.BestIdx[bkw][0]  ][0];
               break;
            }
            SetMB16x16B(m_cur.InterType[0], mvF, mvB, m_cur.InterCost[0]);
            SetError((vm_char*)"Wrong decision in MakeMBModeDecision16x16", m_cur.InterCost[0]>=ME_BIG_COST);
        }
    }

}

void MeBase::MakeMBModeDecision16x16ByFB()
{
    if(m_par->UseFastFeedback)
    {
        if(m_par->SearchDirection == ME_ForwardSearch)
            ModeDecision16x16ByFBFastFrw();
        else
            ModeDecision16x16ByFBFastBidir();
    }
    else
    {
        if(m_par->SearchDirection == ME_ForwardSearch)
            ModeDecision16x16ByFBFullFrw();
        else
            ModeDecision16x16ByFBFullBidir();
    }
}
void  MeBase::MakeSkipModeDecision16x16BidirByFB()
{
    // SKIP
    if(!m_par->ProcessSkipped)
        return;

    MeCostRD InterCostRD = 0;
    Ipp32s SkipCost,IdxF,IdxB;
    bool FOutOfBound = false, BOutOfBound = false;
    //m_cur.SkipCost[0] = ME_BIG_COST;

    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    for(IdxF = 0; IdxF < m_par->FRefFramesNum; IdxF++)
    {
        m_cur.RefDir = frw;
        m_cur.RefIdx = IdxF;

        InterCostRD.D=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SSD, m_cur.PredMV[frw][IdxF][m_cur.BlkIdx]);
        FOutOfBound = (InterCostRD.D == ME_BIG_COST)? true: false;
        if(!FOutOfBound)
        {
            AddHeaderCost(InterCostRD, NULL, ME_MbFrwSkipped);
            SkipCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*m_InterRegFunR.Weight(InterCostRD.R));
        }
        else
        {
            SkipCost = ME_BIG_COST;
        }

        if(m_cur.SkipCost[0] > SkipCost)
        {
            m_cur.SkipCost[0] = SkipCost;
            m_cur.SkipIdx[frw][0] = IdxF;
            m_cur.SkipType[0] = ME_MbFrwSkipped;
            mb->SkippedCostRD = InterCostRD;
        }
        for(IdxB = 0; IdxB < m_par->BRefFramesNum; IdxB++)
        {
            m_cur.RefDir = bkw;
            m_cur.RefIdx = IdxB;

            InterCostRD.D=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SSD, m_cur.PredMV[bkw][IdxB][m_cur.BlkIdx]);
            BOutOfBound = (InterCostRD.D == ME_BIG_COST)? true: false;
            if(!BOutOfBound)
            {
                AddHeaderCost(InterCostRD, NULL, ME_MbBkwSkipped);
                SkipCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*m_InterRegFunR.Weight(InterCostRD.R));
            }
            else
            {
                SkipCost = ME_BIG_COST;
            }

            if(m_cur.SkipCost[0] > SkipCost)
            {
                m_cur.SkipCost[0] = SkipCost;
                m_cur.SkipIdx[bkw][0] = IdxB;
                m_cur.SkipType[0] = ME_MbBkwSkipped;
                mb->SkippedCostRD = InterCostRD;
            }
            //bidir
            if(!FOutOfBound && !BOutOfBound)
            {
                InterCostRD.D=EstimatePointAverage(m_cur.MbPart, m_par->PixelType, ME_SSD,
                                        frw, IdxF, m_cur.PredMV[frw][IdxF][m_cur.BlkIdx],
                                        bkw, IdxB, m_cur.PredMV[bkw][IdxB][m_cur.BlkIdx]);
                AddHeaderCost(InterCostRD, NULL, ME_MbBidirSkipped);
                SkipCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*m_InterRegFunR.Weight(InterCostRD.R));
                if(m_cur.SkipCost[0] > SkipCost)
                {
                    m_cur.SkipCost[0] = SkipCost;
                    m_cur.SkipIdx[frw][0] = IdxF;
                    m_cur.SkipIdx[bkw][0] = IdxB;
                    m_cur.SkipType[0] = ME_MbBidirSkipped;
                    mb->SkippedCostRD = InterCostRD;
                }
            }
        }
    }

    //Direct skipped
    MeMV MVDirectFW, MVDirectBW;
    InterCostRD = 0;

    IdxF = m_cur.DirectIdx[frw];
    IdxB = m_cur.DirectIdx[bkw];

    MVDirectFW = m_par->pRefF[IdxF]->MVDirect[m_cur.adr];
    MVDirectBW = m_par->pRefB[IdxB]->MVDirect[m_cur.adr];

    InterCostRD.D=EstimatePointAverage(m_cur.MbPart, m_par->PixelType, ME_SSD,
                            frw, IdxF, MVDirectFW,
                            bkw, IdxB, MVDirectBW);
    AddHeaderCost(InterCostRD, NULL, ME_MbDirectSkipped);
    m_cur.DirectCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*m_InterRegFunR.Weight(InterCostRD.R));
    m_cur.DirectType = ME_MbDirectSkipped;
    mb->DirectSkippedCostRD = InterCostRD;
}

void MeBase::SetModeDecision16x16BidirByFB(MeMV &mvF, MeMV &mvB)
{
    Ipp32s bestCost = IPP_MIN(IPP_MIN(m_cur.IntraCost[0],m_cur.InterCost[0]),IPP_MIN(m_cur.DirectCost,m_cur.SkipCost[0]));

    if(bestCost == m_cur.SkipCost[0]){
        //skip
        switch(m_cur.SkipType[0]){
            case ME_MbFrwSkipped:
                m_cur.BestIdx[frw][0] = m_cur.SkipIdx[frw][0];
                mvF = m_cur.PredMV[frw][m_cur.SkipIdx[frw][0]][0];
           break;
            case ME_MbBkwSkipped:
                m_cur.BestIdx[bkw][0] = m_cur.SkipIdx[bkw][0];
                mvB = m_cur.PredMV[bkw][m_cur.SkipIdx[bkw][0]][0];
           break;
            case ME_MbBidirSkipped:
                m_cur.BestIdx[frw][0] = m_cur.SkipIdx[frw][0];
                m_cur.BestIdx[bkw][0] = m_cur.SkipIdx[bkw][0];;
                mvF = m_cur.PredMV[frw][m_cur.SkipIdx[frw][0]][0];
                mvB = m_cur.PredMV[bkw][m_cur.SkipIdx[bkw][0]][0];
           break;
        }
        SetMB16x16B(m_cur.SkipType[0], mvF, mvB, m_cur.SkipCost[0]);
    }else if(bestCost == m_cur.DirectCost){
        //direct or direct skip
        m_cur.BestIdx[frw][0] = m_cur.DirectIdx[frw];
        m_cur.BestIdx[bkw][0] = m_cur.DirectIdx[bkw];;
        mvF = m_par->pRefF[m_cur.DirectIdx[frw]]->MVDirect[m_cur.adr];
        mvB = m_par->pRefB[m_cur.DirectIdx[bkw]]->MVDirect[m_cur.adr];
        if(m_cur.DirectType == ME_MbDirect)
        {
            MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
            mb->InterCostRD = mb->DirectCostRD;
        }
        SetMB16x16B(m_cur.DirectType, mvF, mvB, m_cur.DirectCost);
    }else if(bestCost == m_cur.IntraCost[0]){
        //intra
        SetMB16x16B(ME_MbIntra, 0, 0, ME_BIG_COST);
    }else{
        //Inter
        switch(m_cur.InterType[0]){
            case ME_MbFrw:
                mvF = m_cur.BestMV[frw][  m_cur.BestIdx[frw][0]  ][0];
           break;
            case ME_MbBkw:
                mvB = m_cur.BestMV[bkw][  m_cur.BestIdx[bkw][0]  ][0];
           break;
            case ME_MbBidir:
                mvF = m_cur.BestMV[frw][  m_cur.BestIdx[frw][0]  ][0];
                mvB = m_cur.BestMV[bkw][  m_cur.BestIdx[bkw][0]  ][0];
           break;
        }
        SetMB16x16B(m_cur.InterType[0], mvF, mvB, m_cur.InterCost[0]);
        SetError((vm_char*)"Wrong decision in MakeMBModeDecision16x16", m_cur.InterCost[0]>=ME_BIG_COST);
    }
}

void MeBase::ModeDecision16x16ByFBFastFrw()
{
    SetError((vm_char*)"Wrong MbPart in MakeMBModeDecision16x16ByFB", m_par->MbPart != ME_Mb16x16);
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];

    // SKIP
    Ipp32s SkipCost = ME_BIG_COST;
    if(m_par->ProcessSkipped)
        SkipCost=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SSD, m_cur.PredMV[frw][0][m_cur.BlkIdx]);

    MeMV mv;
    Ipp32s InterCost, IntraCost;

        // INTER 
         mv = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0];
        MeMV pred = m_cur.PredMV[frw][m_cur.BestIdx[frw][0]][0];
        mb->PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
        mb->InterCostRD.D = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sadt, mv);
        InterCost = (Ipp32s)( m_InterRegFunD.Weight(mb->InterCostRD.D)+m_lambda*m_MvRegrFunR.Weight(mv,pred));
        // INTRA
        mb->IntraCostRD.D=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SadtSrcNoDC, 0);
        IntraCost = (Ipp32s)(m_IntraRegFunD.Weight(mb->IntraCostRD.D));

        //for preset calculation
        #ifdef ME_GENERATE_PRESETS
            SkipCost=IPP_MAX_32S;
            InterCost  = rand()/2;
            IntraCost  = rand()/2;
        #endif

        //make decision
        Ipp32s bestCost=IPP_MIN(IPP_MIN(SkipCost,InterCost),IntraCost);
        if(bestCost==SkipCost){
            //skip
            SetMB16x16B(ME_MbFrwSkipped, m_cur.PredMV[frw][0][0], 0, m_cur.SkipCost[0]);
        }else if(bestCost == IntraCost){
            //intra
            SetMB16x16B(ME_MbIntra, 0, 0, 0xBAD);
    }else{
            //inter
            SetMB16x16B(ME_MbFrw, m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0], 0, m_cur.InterCost[0]);
            SetError((vm_char*)"Wrong decision in MakeMBModeDecision16x16ByFB", m_cur.InterCost[0]>=ME_BIG_COST);
        }
}

void MeBase::ModeDecision16x16ByFBFastBidir()
{
    MeMV mvF, mvB;
    Ipp32s IdxF, IdxB;

    SetError((vm_char*)"Wrong MbPart in MakeMBModeDecision16x16ByFB", m_par->MbPart != ME_Mb16x16);

    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];

    m_PredCalc->SetDefFrwBkw(mvF, mvB);

    MakeSkipModeDecision16x16BidirByFB();

    Ipp32s InterCost, PureSAD, InterCostD;

    //Direct
    MeMV MVDirectFW, MVDirectBW;
    Ipp32s DirectCost;

    IdxF = m_cur.DirectIdx[frw];
    IdxB = m_cur.DirectIdx[bkw];

    MVDirectFW = m_par->pRefF[IdxF]->MVDirect[m_cur.adr];
    MVDirectBW = m_par->pRefB[IdxB]->MVDirect[m_cur.adr];

    //Direct:
    if(m_par->ProcessDirect)
    {
    PureSAD = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                    ME_Sad, frw, IdxF, MVDirectFW, bkw, IdxB, MVDirectBW);
    DirectCost = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                    ME_Sadt, frw, IdxF, MVDirectFW, bkw, IdxB, MVDirectBW);
    DirectCost = (Ipp32s)m_InterRegFunD.Weight(DirectCost);

    if(DirectCost < m_cur.DirectCost)
    {
        m_cur.DirectType = ME_MbDirect;
        m_cur.DirectCost = DirectCost;
    }
    }
    //Inter
    //m_cur.InterCost[0] = ME_BIG_COST;
    for(Ipp32s i=0; i< m_par->FRefFramesNum; i++)
    {
        MeMV mv = m_cur.BestMV[frw][i][0];
        MeMV pred = m_cur.PredMV[frw][i][0];
        PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
        InterCostD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sadt, mv);
        InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostD)+m_lambda*m_MvRegrFunR.Weight(mv,pred));

        if(InterCost<m_cur.InterCost[0]){
            m_cur.InterCost[0] = InterCost;
            m_cur.InterType[0] = ME_MbFrw;
            m_cur.BestIdx[frw][0] = i;
            m_cur.BestIdx[bkw][0] = -1;//?
            mb->PureSAD = PureSAD;
            mb->InterCostRD.D = InterCostD;
        }
        for(Ipp32s j=0; j< m_par->BRefFramesNum; j++)
        {
            MeMV mv = m_cur.BestMV[bkw][j][0];
            MeMV pred = m_cur.PredMV[bkw][j][0];
            PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
            InterCostD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sadt, mv);
            InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostD)+m_lambda*m_MvRegrFunR.Weight(mv,pred));

            if(InterCost<m_cur.InterCost[0]){
                m_cur.InterCost[0] = InterCost;
                m_cur.InterType[0] = ME_MbBkw;
                m_cur.BestIdx[bkw][0] = j;
                m_cur.BestIdx[frw][0] = -1;
                mb->PureSAD = PureSAD;
                mb->InterCostRD.D = InterCostD;
            }

            MeMV mv0 = m_cur.BestMV[frw][i][0];
            MeMV pred0 = m_cur.PredMV[frw][i][0];
            MeMV mv1 = m_cur.BestMV[bkw][j][0];
            MeMV pred1 = m_cur.PredMV[bkw][j][0];
            PureSAD = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                        ME_Sad, frw, i, mv0, bkw, j, mv1);
            InterCostD = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                        ME_Sadt, frw, i, mv0, bkw, j, mv1);
            InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostD)
                                    +m_lambda*(m_MvRegrFunR.Weight(mv0,pred0)+m_MvRegrFunR.Weight(mv1,pred1)));

            if(InterCost<m_cur.InterCost[0]){
                m_cur.InterCost[0] = InterCost;
                m_cur.InterType[0] = ME_MbBidir;
                m_cur.BestIdx[frw][0] = i;
                m_cur.BestIdx[bkw][0] = j;
                mb->PureSAD = PureSAD;
                mb->InterCostRD.D = InterCostD;
            }
        }//for(j)
    }//for(i)

    //INTRA
    mb->IntraCostRD.D=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SadtSrcNoDC, 0);
    m_cur.IntraCost[0] = (Ipp32s)(m_IntraRegFunD.Weight(mb->IntraCostRD.D));

    SetModeDecision16x16BidirByFB(mvF, mvB);
}

void MeBase::ModeDecision16x16ByFBFullFrw()
{
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];

    // SKIP
    Ipp32s SkipCost = ME_BIG_COST;
    if(m_par->ProcessSkipped)
        SkipCost=EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_SSD, m_cur.PredMV[frw][0][m_cur.BlkIdx]);

    MeMV mv;
    Ipp32s InterCost, IntraCost;

    // INTER 
    // TODO: rewrite this, currently we use previous MB value for MV weighting, should we use another?
    mv = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0];
    MeMV pred = m_PredCalc->GetPrediction(mv);
    mb->PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
    if(m_par->UseVarSizeTransform){
        mb->InterCostRD = MakeTransformDecisionInter(m_cur.MbPart, m_par->PixelType, mv);
    }else{
        // TODO: replace this by EstimatePoint or MakeTransformDecisionInter
        MeAdr src, ref; src.chroma=ref.chroma=true; //true means load also chroma plane addresses
        MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv, &ref);
        MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);
        mb->InterCostRD = GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);
        AddHeaderCost(mb->InterCostRD, NULL, ME_MbFrw);
    }
    m_par->pSrc->MBs[m_cur.adr].NumOfNZ=mb->InterCostRD.NumOfCoeff;
    InterCost = (Ipp32s)( m_InterRegFunD.Weight(mb->InterCostRD.D)+m_lambda*(m_InterRegFunR.Weight(mb->InterCostRD.R)+GetMvSize(mv.x-pred.x,mv.y-pred.y)));
    if(InterCost<0) InterCost= ME_BIG_COST;
    
    // INTRA
    MeAdr src; src.chroma=true;
    MakeSrcAdr(m_cur.MbPart, ME_IntegerPixel, &src);
    mb->IntraCostRD = GetCostRD(ME_IntraRD,ME_Mb16x16,ME_Tranform8x8,&src,NULL);
    AddHeaderCost(mb->IntraCostRD, NULL, ME_MbIntra);
    IntraCost = (Ipp32s)(m_IntraRegFunD.Weight(mb->IntraCostRD.D) + m_lambda*m_IntraRegFunR.Weight(mb->IntraCostRD.R));
    if(IntraCost<0) IntraCost= ME_BIG_COST;

    //for preset calculation
    #ifdef ME_GENERATE_PRESETS
        SkipCost=IPP_MAX_32S;
        InterCost  = rand()/2;
        IntraCost  = rand()/2;
    #endif

    //make decision
    Ipp32s bestCost=IPP_MIN(IPP_MIN(SkipCost,InterCost),IntraCost);
    if(bestCost==SkipCost){
        //skip
        SetMB16x16B(ME_MbFrwSkipped, m_cur.PredMV[frw][0][0], 0, bestCost);
    }else if(bestCost == IntraCost){
        //intra
        SetMB16x16B(ME_MbIntra, 0, 0, bestCost);
    }else{
        //inter
        SetMB16x16B(ME_MbFrw, m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][0], 0, bestCost);
        SetError((vm_char*)"Wrong decision in MakeMBModeDecision16x16ByFB", m_cur.InterCost[0]>=ME_BIG_COST);
    }
}

void MeBase::ModeDecision16x16ByFBFullBidir()
{
    MeMV mvF, mvB;
    Ipp32s IdxF, IdxB;

    SetError((vm_char*)"Wrong MbPart in MakeMBModeDecision16x16ByFB", m_par->MbPart != ME_Mb16x16);

    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    m_PredCalc->SetDefFrwBkw(mvF, mvB);

    MakeSkipModeDecision16x16BidirByFB();

    Ipp32s InterCost, PureSAD;
    MeCostRD InterCostRD;

    //Direct
    MeMV MVDirectFW, MVDirectBW;
    Ipp32s DirectCost;

    IdxF = m_cur.DirectIdx[frw];
    IdxB = m_cur.DirectIdx[bkw];

    MVDirectFW = m_par->pRefF[IdxF]->MVDirect[m_cur.adr];
    MVDirectBW = m_par->pRefB[IdxB]->MVDirect[m_cur.adr];

    MeAdr src, ref; src.chroma=ref.chroma=true; //true means load also chroma plane addresses
    MeAdr ref0, ref1; ref0.chroma=ref1.chroma=true; //true means load also chroma plane addresses

    if(m_par->ProcessDirect)
    {
    PureSAD = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                ME_Sad, frw, IdxF, MVDirectFW, bkw, IdxB, MVDirectBW);

    //no VST
    // TODO: replace this by EstimatePoint or MakeTransformDecisionInter
    m_cur.RefDir = frw;
    m_cur.RefIdx = IdxF;
    MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, MVDirectFW, &ref0);
    m_cur.RefDir = bkw;
    m_cur.RefIdx = IdxB;
    MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, MVDirectBW, &ref1);
    MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);

    ippiAverage16x16_8u_C1R(ref0.ptr[Y], ref0.step[Y], ref1.ptr[Y], ref1.step[Y], m_cur.buf[bidir].ptr[Y], 16);
    ref.set(Y, m_cur.buf[bidir].ptr[Y], 16);
    ippiAverage8x8_8u_C1R(ref0.ptr[U], ref0.step[U], ref1.ptr[U], ref1.step[U], m_cur.buf[bidir].ptr[U], 8);
    ref.set(U, m_cur.buf[bidir].ptr[U], 8);
    ippiAverage8x8_8u_C1R(ref0.ptr[V], ref0.step[V], ref1.ptr[V], ref1.step[V], m_cur.buf[bidir].ptr[V], 8);
    ref.set(V, m_cur.buf[bidir].ptr[V], 8);

    InterCostRD = GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);
    AddHeaderCost(InterCostRD, NULL, ME_MbDirect);

    DirectCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*m_InterRegFunR.Weight(InterCostRD.R));
    if(DirectCost<0) DirectCost= ME_BIG_COST;

    mb->DirectCostRD = InterCostRD;
    if(DirectCost < m_cur.DirectCost)
    {
        m_cur.DirectType = ME_MbDirect;
        m_cur.DirectCost = DirectCost;
    }
    }
    //Inter
    for(Ipp32s i=0; i< m_par->FRefFramesNum; i++)
    {
        m_cur.RefDir = frw;
        m_cur.RefIdx = i;
        MeMV mv = m_cur.BestMV[frw][i][0];
        MeMV pred = m_cur.PredMV[frw][i][0];
        //MeMV pred = m_PredCalc->GetPrediction(mv);

        PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
        //no VST
        // TODO: replace this by EstimatePoint or MakeTransformDecisionInter
        src.chroma=ref.chroma=true; //true means load also chroma plane addresses
        MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv, &ref);
        MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);
        InterCostRD = GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);
        AddHeaderCost(InterCostRD, NULL, ME_MbFrw);

        InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*(m_InterRegFunR.Weight(InterCostRD.R)+GetMvSize(mv.x-pred.x,mv.y-pred.y)));
        if(InterCost<0) InterCost= ME_BIG_COST;

        if(InterCost<m_cur.InterCost[0]){
            m_cur.InterCost[0] = InterCost;
            m_cur.InterType[0] = ME_MbFrw;
            m_cur.BestIdx[frw][0] = i;
            m_cur.BestIdx[bkw][0] = -1;//?
            mb->PureSAD = PureSAD;
            m_par->pSrc->MBs[m_cur.adr].NumOfNZ=InterCostRD.NumOfCoeff;
            mb->InterCostRD = InterCostRD;
        }
        for(Ipp32s j=0; j< m_par->BRefFramesNum; j++)
        {
            m_cur.RefDir = bkw;
            m_cur.RefIdx = j;
            MeMV mv = m_cur.BestMV[bkw][j][0];
            MeMV pred = m_cur.PredMV[bkw][j][0];
            //MeMV pred = m_PredCalc->GetPrediction(mv);

            PureSAD = EstimatePoint(m_cur.MbPart, m_par->PixelType, ME_Sad, mv);
            //no VST
            // TODO: replace this by EstimatePoint or MakeTransformDecisionInter
            src.chroma=ref.chroma=true; //true means load also chroma plane addresses
            MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv, &ref);
            MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);
            InterCostRD = GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);
            AddHeaderCost(InterCostRD, NULL, ME_MbBkw);

            InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*(m_InterRegFunR.Weight(InterCostRD.R)+GetMvSize(mv.x-pred.x,mv.y-pred.y)));
            if(InterCost<0) InterCost= ME_BIG_COST;

            if(InterCost<m_cur.InterCost[0]){
                m_cur.InterCost[0] = InterCost;
                m_cur.InterType[0] = ME_MbBkw;
                m_cur.BestIdx[bkw][0] = j;
                m_cur.BestIdx[frw][0] = -1;//?
                mb->PureSAD = PureSAD;
                m_par->pSrc->MBs[m_cur.adr].NumOfNZ=InterCostRD.NumOfCoeff;
                mb->InterCostRD = InterCostRD;
            }

            MeMV mv0 = m_cur.BestMV[frw][i][0];
            MeMV pred0 = m_cur.PredMV[frw][i][0];
            //MeMV pred0 = m_PredCalc->GetPrediction(mv0);

            MeMV mv1 = m_cur.BestMV[bkw][j][0];
            MeMV pred1 = m_cur.PredMV[bkw][j][0];
            //MeMV pred1 = m_PredCalc->GetPrediction(mv1);

            PureSAD = EstimatePointAverage(m_cur.MbPart, m_par->PixelType,
                        ME_Sad, frw, i, mv0, bkw, j, mv1);

            //no VST
            // TODO: replace this by EstimatePoint or MakeTransformDecisionInter
            src.chroma=ref.chroma=true; //true means load also chroma plane addresses
            ref0.chroma=ref1.chroma=true; //true means load also chroma plane addresses
            m_cur.RefDir = frw;
            m_cur.RefIdx = i;
            MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv0, &ref0);
            m_cur.RefDir = bkw;
            m_cur.RefIdx = j;
            MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv1, &ref1);
            MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);

            ippiAverage16x16_8u_C1R(ref0.ptr[Y], ref0.step[Y], ref1.ptr[Y], ref1.step[Y], m_cur.buf[bidir].ptr[Y], 16);
            ref.set(Y, m_cur.buf[bidir].ptr[Y], 16);
            ippiAverage8x8_8u_C1R(ref0.ptr[U], ref0.step[U], ref1.ptr[U], ref1.step[U], m_cur.buf[bidir].ptr[U], 8);
            ref.set(U, m_cur.buf[bidir].ptr[U], 8);
            ippiAverage8x8_8u_C1R(ref0.ptr[V], ref0.step[V], ref1.ptr[V], ref1.step[V], m_cur.buf[bidir].ptr[V], 8);
            ref.set(V, m_cur.buf[bidir].ptr[V], 8);

            InterCostRD = GetCostRD(ME_InterRD,ME_Mb16x16,ME_Tranform8x8,&src,&ref);
            AddHeaderCost(InterCostRD, NULL, ME_MbBidir);

            InterCost = (Ipp32s)( m_InterRegFunD.Weight(InterCostRD.D)+m_lambda*(m_InterRegFunR.Weight(InterCostRD.R)+GetMvSize(mv0.x-pred0.x,mv0.y-pred0.y)+GetMvSize(mv1.x-pred1.x,mv1.y-pred1.y)));
            if(InterCost<0) InterCost= ME_BIG_COST;

            if(InterCost<m_cur.InterCost[0]){
                m_cur.InterCost[0] = InterCost;
                m_cur.InterType[0] = ME_MbBidir;
                m_cur.BestIdx[frw][0] = i;
                m_cur.BestIdx[bkw][0] = j;
                mb->PureSAD = PureSAD;
                m_par->pSrc->MBs[m_cur.adr].NumOfNZ=InterCostRD.NumOfCoeff;
                mb->InterCostRD = InterCostRD;
            }
        }
    }

    //INTRA
    src.chroma=true;
    MakeSrcAdr(m_cur.MbPart, ME_IntegerPixel, &src);
    mb->IntraCostRD = GetCostRD(ME_IntraRD,ME_Mb16x16,ME_Tranform8x8,&src,NULL);
    AddHeaderCost(mb->IntraCostRD, NULL, ME_MbIntra);
    m_cur.IntraCost[0] = (Ipp32s)(m_IntraRegFunD.Weight(mb->IntraCostRD.D) + m_lambda*m_IntraRegFunR.Weight(mb->IntraCostRD.R));
    if(m_cur.IntraCost[0]<0) m_cur.IntraCost[0]= ME_BIG_COST;

    SetModeDecision16x16BidirByFB(mvF, mvB);
}

//function performs mode decision for one block and saves results to m_cur.InterType.
void MeBase::MakeBlockModeDecision()
{
    //currently two blocks type are possible skip and inter
    // TODO: move this somewhere
    if(m_par->UseFeedback) return;

    if(m_cur.SkipCost[m_cur.BlkIdx]<m_SkippedThreshold/4){
        //skip
        Ipp32s dir = (m_cur.SkipType[m_cur.BlkIdx] == ME_MbFrwSkipped) ? frw : bkw;
        Ipp32s opp_dir = (dir == frw)? bkw : frw;
        Ipp32s idx = m_cur.SkipIdx[dir][m_cur.BlkIdx];
        m_cur.InterCost[m_cur.BlkIdx] = m_cur.SkipCost[m_cur.BlkIdx];
        m_cur.InterType[m_cur.BlkIdx] = m_cur.SkipType[m_cur.BlkIdx]; //MB type is used here!
        m_cur.BestIdx[dir][m_cur.BlkIdx] = idx;
        m_cur.BestIdx[opp_dir][m_cur.BlkIdx] = -1;
        m_cur.BestMV[dir][idx][m_cur.BlkIdx] = m_cur.PredMV[dir][idx][m_cur.BlkIdx];
        m_cur.SkipCost[m_cur.BlkIdx] = ME_BIG_COST;
    }else if(m_cur.IntraCost[m_cur.BlkIdx] < m_cur.InterCost[m_cur.BlkIdx]){
        //intra
        m_cur.InterType[m_cur.BlkIdx] = ME_MbIntra; //MB type is used here!
        // TODO: some meaningful value should be assigned to cost here for proper 16x16 8x8 mode decision
//        m_cur.InterCost[m_cur.BlkIdx] = ME_BIG_COST; //for T2 calculation
    }else{
        //inter
        //leave current type
        SetError((vm_char*)"Wrong decision in MakeBlockModeDecision", m_cur.InterCost[m_cur.BlkIdx]>=ME_BIG_COST);
    }
}


void MeBase::MakeMBModeDecision8x8()
{
    if(!m_par->UseFeedback)
        MakeMBModeDecision8x8Org();
    else
        MakeMBModeDecision8x8ByFB();
}

void MeBase::MakeMBModeDecision8x8Org()
{
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];

    if(mb->MbType == ME_MbFrw || mb->MbType == ME_MbBkw){
        //choose 16x16 or 8x8
        Ipp32s InterCost=0;
        if(m_cur.SkipCost[0] != ME_BIG_COST)
        {
            for(Ipp32s i=0; i<4; i++){
                InterCost+=m_cur.SkipCost[i];
                Ipp32s dir = (m_cur.SkipType[i] == ME_MbFrwSkipped) ? frw : bkw;
                Ipp32s opp_dir = (dir == frw)? bkw : frw;
                Ipp32s idx = m_cur.SkipIdx[dir][i];
                m_cur.InterCost[i] = m_cur.SkipCost[i];
                m_cur.InterType[i] = m_cur.SkipType[i]; //MB type is used here!
                m_cur.BestIdx[dir][i] = idx;
                m_cur.BestIdx[opp_dir][i] = -1;
                m_cur.BestMV[dir][idx][i] = m_cur.PredMV[dir][idx][i];
            }
        }
        for(Ipp32s i=0; i<4; i++){
            InterCost+=m_cur.InterCost[i];
        }
        InterCost = (Ipp32s)1.2*InterCost;

        if(mb->MbCosts[0] > InterCost){
            //8x8 is better, save results
            Ipp32s dir = (m_cur.InterType[0] == ME_MbFrw ||
                            m_cur.InterType[0] == ME_MbFrwSkipped) ? frw : bkw;
            SetMB8x8(m_cur.InterType[0], m_cur.BestMV[dir][0], m_cur.InterCost);
        }
    }
}

Ipp16s median4(Ipp16s a, Ipp16s b, Ipp16s c, Ipp16s d)
{
    Ipp16s amin=IPP_MIN(IPP_MIN(a,b),IPP_MIN(c,d));
    Ipp16s amax=IPP_MAX(IPP_MAX(a,b),IPP_MAX(c,d));
    return (a+b+c+d-amin-amax)/2;
}

void MeBase::MakeMBModeDecision8x8ByFB()
{
    SetError((vm_char*)"Fast feedback is not supported in MeBase::MakeMBModeDecision8x8ByFB", m_par->UseFastFeedback);

    MeMV mv, pred;
    MeCostRD res=0, tmp;
    Ipp32s pattern=0;
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];

    m_PredCalc->ResetPredictors(); // TODO: change pred calculator to avoid this
    for(Ipp32s blk=0; blk<4;blk++){
        m_cur.BlkIdx = blk;
        mv = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx];
        m_PredCalc->GetPrediction(); //get predictor, this call initializes some inner variables
        pred = m_PredCalc->GetPrediction(mv); //this is real calculation

        MeAdr src, ref;
        MakeRefAdrFromMV(m_cur.MbPart, m_par->PixelType, mv, &ref);
        MakeSrcAdr(m_cur.MbPart, m_par->PixelType, &src);

        tmp=GetCostRD(ME_InterRD,ME_Mb8x8,ME_Tranform8x8,&src,&ref);
        res += tmp;

        m_par->pSrc->MBs[m_cur.adr].NumOfNZ=tmp.NumOfCoeff;
        int mvlen = GetMvSize(mv.x-pred.x,mv.y-pred.y);
        if(m_cur.HybridPredictor)mvlen++;  // TODO:  move to GetMvSize

        pattern = pattern<<1;
        if((mv.x-pred.x) != 0 || (mv.y-pred.y)!=0 || tmp.NumOfCoeff!=0) pattern |=1;
        res.R += mvlen;
    }

    //add chroma
    m_cur.BlkIdx=0;
    MeMV* mvx = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx];
    mv.x=median4(mvx[0].x, mvx[1].x,mvx[2].x,mvx[3].x);
    mv.y=median4(mvx[0].y, mvx[1].y,mvx[2].y,mvx[3].y);
    MeMV cmv = GetChromaMV(mv);
    
    // ME_Mb16x16 is used here to process chroma as 8x8 block
    MeAdr src, ref; src.chroma=ref.chroma=true; //true means load also chroma plane addresses
    MakeRefAdrFromMV(ME_Mb16x16, m_par->PixelType, mv, &ref);
    MakeSrcAdr(ME_Mb16x16, m_par->PixelType, &src);
    
    m_cur.BlkIdx = 4;
    src.channel = U;
    res+=GetCostRD(ME_InterRD,ME_Mb8x8,ME_Tranform8x8,&src,&ref);
    m_cur.BlkIdx = 5;
    src.channel = V;
    res+=GetCostRD(ME_InterRD,ME_Mb8x8,ME_Tranform8x8,&src,&ref);
    
     pattern = pattern<<2;
     res.BlkPat = 3&res.BlkPat;
     res.BlkPat = res.BlkPat |pattern; 
     
    AddHeaderCost(res, NULL, ME_MbFrw);

    // TODO: move this to AddHeaderCost
    res.R+=2; //this is to account for skip and 1/4 MV

    Ipp32s InterCost8x8 = (Ipp32s)( res.D+m_lambda*res.R);
    if(InterCost8x8<mb->MbCosts[0]){ //this is comparision to best 16x16 RD cost
        //printf("[%d], %d,%d,\n", m_cur.adr, res.D, res.R);
        for(Ipp32s i=0; i<4; i++) m_cur.InterType[i] = ME_MbFrw; //only inter type is estimated right now
        SetMB8x8(ME_MbFrw, m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx], m_cur.InterCost); // it is not RD costs here!
    }

    m_cur.BlkIdx=0;
}

bool MeBase::IsSkipEnabled()
{
    // TODO: add fast skip estimation for FB
    if(!m_par->ProcessSkipped || m_par->UseFeedback)
        return false;
    if((m_par->PredictionType == ME_MPEG2) && 
        (m_cur.x == 0 || m_cur.x == m_par->pSrc->WidthMB - 1 || m_par->pSrc->MBs[m_cur.adr-1].MbType == ME_MbIntra))
        return false;

    return true;
}


//Interpolate data from src to dst, only two lowest bits of MV is used.
//Src and dst are left top pixel adresses of MB, block or subblock.
//Src address wil be replaced at the end of the function by destination one.
void MeBase::Interpolate(MeMbPart mt, MeInterpolation interp, MeMV mv, MeAdr* src, MeAdr *dst)
{
    Ipp32s ch=src->channel;
    
    IppVCInterpolate_8u par;
    IppVCInterpolateBlock_8u m_lumaMCParams;
//    IppVCInterpolateBlock_8u m_chromaMCParams;

    par.roiSize.height = 16;
    par.roiSize.width = 16;
    par.pSrc = src->ptr[ch];
    par.srcStep = src->step[ch];
    par.pDst = dst->ptr[ch];
    par.dstStep = dst->step[ch];
    par.dx = 3&mv.x;
    par.dy = 3&mv.y;
    par.roundControl = 0;
    
    if (ME_AVS_Luma == interp)
    {
        m_lumaMCParams.pSrc[0] = src->ptr[ch];
        m_lumaMCParams.srcStep = src->step[ch];
        m_lumaMCParams.pDst[0] = dst->ptr[ch];
        m_lumaMCParams.dstStep = dst->step[ch];
        m_lumaMCParams.pointBlockPos.x = m_cur.x *16;
        m_lumaMCParams.pointBlockPos.y = m_cur.y *16;
        m_lumaMCParams.sizeBlock.height = 16;
        m_lumaMCParams.sizeBlock.width = 16;
        m_lumaMCParams.pointVector.x = mv.x;
        m_lumaMCParams.pointVector.y = mv.y;
        m_lumaMCParams.sizeFrame.height = m_par->pSrc->HeightMB*16;
        m_lumaMCParams.sizeFrame.width = m_par->pSrc->WidthMB*16;
        // set pSrc into begin of picture
        m_lumaMCParams.pSrc[0] = m_lumaMCParams.pSrc[0]
                                 - m_lumaMCParams.pointBlockPos.y * m_lumaMCParams.srcStep
                                 - m_lumaMCParams.pointBlockPos.x
                                 - ((m_lumaMCParams.pointVector.y)>>2) * m_lumaMCParams.srcStep
                                 - ((m_lumaMCParams.pointVector.x)>>2);
    }

    switch(mt) {
        case ME_Mb16x16:
        break;
        case ME_Mb8x8:
            par.roiSize.height = 8;
            par.roiSize.width = 8;
            m_lumaMCParams.sizeBlock.height = 8;
            m_lumaMCParams.sizeBlock.width = 8;
        break;
        default:
            SetError((vm_char*)"Wrong mt in MeBase::Interpolate");
    }
    IppStatus st = ippStsNoErr;
    switch(interp){
        case ME_VC1_Bicubic:
        st = ippiInterpolateQPBicubic_VC1_8u_C1R(&par);
        break;

        case ME_VC1_Bilinear:
        st = ippiInterpolateQPBilinear_VC1_8u_C1R(&par);
        break;

        case ME_AVS_Luma:
        st = ippiInterpolateLumaBlock_AVS_8u_P1R(&m_lumaMCParams);
        break;

        default:
            SetError((vm_char*)"Wrong interpolation type in MeBase::Interpolate");
    }
    if(st != ippStsNoErr)
        SetError((vm_char*)"Interpolation error in MeBase::Interpolate");

    src->set(ch,dst->ptr[ch],dst->step[ch]);
}


void MeBase::EstimatePointInter(MeMbPart mt, MePixelType pix, MeMV mv)
{
    Ipp32s cost = EstimatePoint(mt, pix, m_par->CostMetric, mv);
    if(cost == ME_BIG_COST)
        return;

    bool UseFB = m_par->UseFeedback;
    #ifdef ME_GENERATE_PRESETS
        UseFB = false;
    #endif

    if(!UseFB || m_cur.MbPart == ME_Mb8x8){ // TODO: add 8x8 J regression 
        cost+=WeightMV(mv, m_cur.PredMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx]);
    }else{
        if(m_par->UseFastFeedback){
            MeMV pred = m_cur.PredMV[frw][m_cur.RefIdx][0];
            cost = (Ipp32s)(m_InterRegFunJ.Weight(cost) + m_lambda*m_MvRegrFunR.Weight(mv,pred));;
        }else{
            MeMV pred = m_PredCalc->GetPrediction(mv);
            cost = (Ipp32s)(m_InterRegFunJ.Weight(cost) + m_lambda * GetMvSize(mv.x-pred.x,mv.y-pred.y));
        }
        SetError((vm_char*)"overflow was detected in EstimatePointInter", cost<0);
    }
    
    if(cost < m_cur.BestCost){
        m_cur.BestCost = cost;
        m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx] = mv;
    }
}


//MV should be valid, there is no check inside!
Ipp32s MeBase::EstimatePointAverage(MeMbPart mt, MePixelType pix, MeDecisionMetrics CostMetric, Ipp32s RefDirection0, Ipp32s RefIndex0, MeMV mv0, Ipp32s RefDirection1, Ipp32s RefIndex1, MeMV mv1)
{
    SetError((vm_char*)"Wrong partitioning in MeBase::EstimatePointAverage",mt!=ME_Mb16x16);

    MeAdr src, ref, ref0, ref1;

    //average
    SetReferenceFrame(RefDirection0,RefIndex0);
    MakeRefAdrFromMV(mt, pix, mv0, &ref0);
    SetReferenceFrame(RefDirection1,RefIndex1);
    MakeRefAdrFromMV(mt, pix, mv1, &ref1);

    // TODO: choose proper ROI here
    ippiAverage16x16_8u_C1R(ref0.ptr[Y], ref0.step[Y], ref1.ptr[Y], ref1.step[Y], m_cur.buf[bidir].ptr[Y], 16);

    //get cost
    ref.set(Y, m_cur.buf[bidir].ptr[Y], 16);
    MakeSrcAdr(mt,pix,&src);

    //get chroma addresses
/*    
    if(m_par->UseChromaForME){
        ippiAverage8x8_8u_C1R(adr0.ptr[U], adr0.step[U], adr1.ptr[U], adr1.step[U], m_cur.buf[bidir], 8);
        ippiAverage8x8_8u_C1R(adr0.ptr[V], adr0.step[V], adr1.ptr[V], adr1.step[V], m_cur.buf[bidir], 8);

        src.ptr[U] = m_cur.pSrc[U];
        src.step[U] = m_cur.SrcStep[U];
        ref.ptr[U] = m_cur.buf[bidir];
        ref.step[U] = 8;

        src.ptr[V] = m_cur.pSrc[V];
        src.step[V] = m_cur.SrcStep[V];
        ref.ptr[V] = m_cur.buf[bidir];
        ref.step[V] = 8;
    }
*/
    return GetCost(CostMetric, (MeMbPart)(mt>>GetPlaneLevel(pix)), &src, &ref);
}


//Estimate point and return its cost.
//MV should be valid, there is no check inside!
Ipp32s MeBase::EstimatePoint(MeMbPart mt, MePixelType pix, MeDecisionMetrics CostMetric, MeMV mv)
{
    //check range
    // TODO: temporal solution, move check one or two levles up. To EstimateSkip, FullSearch and so on.
    if(m_PredCalc->IsOutOfBound(mt, pix, mv))
        return ME_BIG_COST;

    MeAdr src, ref;
    if(CostMetric==ME_SSD && m_par->UseChromaForMD)
        src.chroma=ref.chroma=true;
    MakeRefAdrFromMV(mt, pix, mv, &ref);
    MakeSrcAdr(mt,pix,&src);

    //get cost
    return GetCost(CostMetric, (MeMbPart)(mt>>GetPlaneLevel(pix)), &src, &ref);
}


Ipp32s MeBase::GetCost(MeDecisionMetrics CostMetric, MeMbPart mt, MeAdr* src, MeAdr* ref)
{
    //this is shortcut due to performace reason
    Ipp32s cost=ME_BIG_COST;
    if(CostMetric == ME_Sad){
        if(mt==ME_Mb4x4)ippiSAD4x4_8u32s(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y], &cost, 0);
        else if(mt==ME_Mb8x8)ippiSAD8x8_8u32s_C1R(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y], &cost, 0);
        else if(mt==ME_Mb16x16)ippiSAD16x16_8u32s(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y], &cost, 0);
        return cost;
    }

    switch(CostMetric)
    {
        case ME_Sad: // TODO: it looks like a bug
            switch(mt){
                case ME_Mb2x2:{
                    Ipp8u *s = src->ptr[Y];
                    Ipp32s ss = src->step[Y];
                    Ipp8u *r = ref->ptr[Y]; 
                    Ipp32s rs = ref->step[Y];
                    cost= abs((Ipp32s)*(s+0*ss +0)-(Ipp32s)*(r+0*rs +0)) +  abs((Ipp32s)*(s+0*ss +1)-(Ipp32s)*(r+0*rs +1))+
                                         abs((Ipp32s)*(s+1*ss +0)-(Ipp32s)*(r+1*rs +0)) +  abs((Ipp32s)*(s+1*ss +1)-(Ipp32s)*(r+1*rs +1));
                    }
                    break;
                default:
                    SetError((vm_char*)"wrong partitioning in GetCost ME_Sad");
            }
            break;

        case ME_SSD:
            switch(mt){
                case ME_Mb16x16:
                        ippiSqrDiff16x16_8u32s(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y], IPPVC_MC_APX_FF, &cost);
                        if(src->chroma){
                            Ipp32s CostU,CostV;
                            ippiSSD8x8_8u32s_C1R(src->ptr[U], src->step[U], ref->ptr[U], ref->step[U],&CostU,0);
                            ippiSSD8x8_8u32s_C1R(src->ptr[V], src->step[V], ref->ptr[V], ref->step[V],&CostV,0);
                            cost+=CostU+CostV;
                        }
                break;

                default:
                    SetError((vm_char*)"wrong partitioning in GetCost SSD");
            }
            break;

        case ME_InterSSD:
        case ME_InterRate:
        case ME_IntraSSD:
        case ME_IntraRate:
            //cost=GetTransformBasedCost(CostMetric,  mt,  src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y]);
            SetError((vm_char*)"GetTransformBasedCost was called");
            break;

        case ME_Sadt:
        case ME_SadtSrc:
        case ME_SadtSrcNoDC:
            cost=GetCostHDMR(CostMetric, mt, src, ref);
            break;

        default:
            SetError((vm_char*)"wrong cost metric in GetCost");
    }

    return cost;
}

void MeBase::GetCost(MeMbPart mt, MeAdr* src, MeAdr* ref, Ipp32s numX, Ipp16u *sadArr)
{
    if(mt==ME_Mb4x4) ippiSAD4x4xN_8u16u_C1R( src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y],sadArr,numX);
    else if (mt==ME_Mb2x2) {
        for(Ipp32s i=0;i<numX;i++) {
            Ipp8u *s = src->ptr[Y];
            Ipp32s ss = src->step[Y];
            Ipp8u *r = ref->ptr[Y];
            Ipp32s rs = ref->step[Y];
            Ipp32s cost = abs((Ipp32s)*(s+0*ss +0)-(Ipp32s)*(r+0*rs +0)) +  abs((Ipp32s)*(s+0*ss +1)-(Ipp32s)*(r+0*rs +1))+
                          abs((Ipp32s)*(s+1*ss +0)-(Ipp32s)*(r+1*rs +0)) +  abs((Ipp32s)*(s+1*ss +1)-(Ipp32s)*(r+1*rs +1));
            sadArr[i] = (Ipp16u)cost;
        }
    }
}

//separated from GetCost due to performace reason
Ipp32s MeBase::GetCostHDMR(MeDecisionMetrics CostMetric, MeMbPart mt, MeAdr* src, MeAdr* ref)
{
    Ipp16s bufDiff[64];
    Ipp16s bufHDMR[64];

    Ipp32s cost=ME_BIG_COST;

    switch(CostMetric){

        case ME_Sadt:
            switch(mt){
                case ME_Mb16x16:
                    {
                        cost=0;
                        for(Ipp32u i=0; i<4; i++) {
                            MeAdr s=*src;
                            MeAdr r=*ref;
                            MakeBlockAdr(0,ME_Mb8x8, i, &s);
                            MakeBlockAdr(0,ME_Mb8x8, i, &r);
                            
#ifdef USE_OPTIMIZED_SATD
                             cost+=SAT8x8D(s.ptr[Y],s.step[Y], r.ptr[Y], r.step[Y]);
#else
                             ippiSub8x8_8u16s_C1R(s.ptr[Y],s.step[Y], r.ptr[Y], r.step[Y],bufDiff,16);
                             HadamardFwdFast<Ipp16s,8>(bufDiff, 16, bufHDMR);
                             for(Ipp32u j=0; j<64; j++)
                                cost+=abs((Ipp32s)bufHDMR[j]);
#endif
                        }
                }
                break;

                case ME_Mb8x8:
#ifdef USE_OPTIMIZED_SATD
                    cost=SAT8x8D(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y]);
#else
                    {
                        cost=0;
                        ippiSub8x8_8u16s_C1R(src->ptr[Y], src->step[Y], ref->ptr[Y], ref->step[Y], bufDiff, 16);
                        HadamardFwdFast<Ipp16s,8>(bufDiff, 16, bufHDMR);
                        for(Ipp32u j=0; j<64; j++)
                                cost+=abs((Ipp32s)bufHDMR[j]);
                    }
#endif
                break;

                default:
                    SetError((vm_char*)"wrong partitioning in GetCost ME_Sadt");
            }
            break;

        case ME_SadtSrc:
        case ME_SadtSrcNoDC:
            {
                cost = 0;
                switch(mt){
                    case ME_Mb16x16:
                        for(Ipp32s i=0; i<4; i++){
                            MeAdr s=*src;
                            MakeBlockAdr(0,ME_Mb8x8, i, &s);
                            HadamardFwdFast<Ipp8u,8>(s.ptr[Y],s.step[Y],bufHDMR);

                            for(Ipp32s j=(i==0 && CostMetric!=ME_SadtSrcNoDC?0:1); j<64; j++)
                                cost+=abs((Ipp32s)bufHDMR[j]);
                        }
                    break;

                    case ME_Mb8x8:
                            HadamardFwdFast<Ipp8u,8>(src->ptr[Y], src->step[Y], bufHDMR);
                        for(Ipp32s j=0; j<64; j++)
                            cost+=abs((Ipp32s)bufHDMR[j]);
                    break;

                    default:
                        SetError((vm_char*)(vm_char*)"wrong partitioning in GetCost ME_SadtSrc");
                }
            }
            break;

        }

    return cost;

}

MeCostRD MeBase::GetCostRD(MeDecisionMetrics /*CostMetric*/, MeMbPart /*mt*/, MeTransformType /*transf*/, MeAdr* /*src*/, MeAdr* /*ref*/)
{
    MeCostRD cost;
    cost.R=ME_BIG_COST;
    cost.D=ME_BIG_COST;
    return cost;
}

void MeBase::DownsampleOne(Ipp32s level, Ipp32s x0, Ipp32s y0, Ipp32s w, Ipp32s h, MeAdr *adr)
{
    Ipp32s scale = 1<<level;
    IppiSize size = {w,h};
    IppiRect roi = {0,0,size.width,size.height};
    Ipp8u*   pBuffer;
    int      bufferSize;
    IppiRect RectSrc = roi;
    IppiRect RectDst;

    Ipp8u* pSrc=adr[0].ptr[Y];
    Ipp32s SrcStep = adr[0].step[Y];
    Ipp8u* pDst=adr[level].ptr[Y];
    Ipp32s DstStep = adr[level].step[Y];

    ippiGetResizeRect(RectSrc, &RectDst, (double)(1./scale), (double)(1./scale), 0, 0, IPPI_INTER_SUPER);
    ippiResizeGetBufSize(RectSrc, RectDst, 1, IPPI_INTER_SUPER, &bufferSize);

    pBuffer = (Ipp8u*)ippMalloc(bufferSize);

    pSrc += y0*SrcStep + x0;
    pDst += (y0>>level)*DstStep + (x0>>level);
    ippiResizeSqrPixel_8u_C1R(pSrc, size, SrcStep, RectSrc, pDst, DstStep, RectDst, (double)(1./scale), (double)(1./scale), 0, 0, IPPI_INTER_SUPER, pBuffer);

    if(bufferSize > 0)
        ippFree(pBuffer);
}

void MeBase::DownsampleFrames()
{
    if(!m_InitPar.UseDownSampling)
        return;

    // TODO: remove after allocation in init
    SetError((vm_char*) "Wrong size of m_cur.DwnPoints", (Ipp32s)(sizeof(m_cur.DwnPoints)/sizeof(MeSearchPoint)/5) < m_par->pSrc->WidthMB*m_par->pSrc->HeightMB);

    for(Ipp32s lev=1; lev<4; lev++){
        Ipp32s x0, y0, w, h;

        //src
        x0=y0=0;
        w=16*m_par->pSrc->WidthMB;
        h=16*m_par->pSrc->HeightMB;
        DownsampleOne(lev, x0, y0, w, h, m_par->pSrc->plane);

        //ref
        for(Ipp32s i=0; i<m_par->FRefFramesNum+m_par->BRefFramesNum; i++){
            MeFrame *ref;
            if(i<m_par->FRefFramesNum)
                ref=m_par->pRefF[i];
            else
                ref=m_par->pRefB[i-m_par->FRefFramesNum];

            Ipp32s PadX,PadY;
            PadX=PadY=ref->padding;
            SetError((vm_char*)"Wrong padding in MeBase::DownsampleFrames", PadX>m_InitPar.refPadding || PadY>m_InitPar.refPadding || (PadX&0xf)!=0 || (PadY&0xf)!=0);
            
            x0=-PadX;
            y0=-PadY;
            w=16*ref->WidthMB+2*PadX;
            h=16*ref->HeightMB+2*PadY;
            DownsampleOne(lev, x0, y0, w, h, ref->plane);
        }
    }
}


void MeBase::SetReferenceFrame(Ipp32s RefDirection, Ipp32s RefIndex)
{
    m_cur.RefIdx=RefIndex;
    m_cur.RefDir=RefDirection;
}

Ipp32s MeBase::GetPlaneLevel(MePixelType pix)
{
    if(pix == ME_DoublePixel) return 1;
    if(pix == ME_QuadPixel) return 2;
    if(pix == ME_OctalPixel) return 3;
    return 0;
}

void MeBase::MakeSrcAdr(MeMbPart mt, MePixelType pix, MeAdr* adr)
{
    Ipp32s level=GetPlaneLevel(pix);
    *adr = m_par->pSrc->plane[level];
    adr->move((16*m_cur.x)>>level,(16*m_cur.y)>>level);
    MakeBlockAdr(level, mt, m_cur.BlkIdx, adr);
}

//Function returns "adr", this is set of pointers to the left top pixel of the MB, current block or subblock in current reference frame.
//If interpolation is needed function performs interpolation and returns pointer to the buffer with interpolated data.
void MeBase::MakeRefAdrFromMV(MeMbPart mt, MePixelType pix, MeMV mv, MeAdr* adr)
{
    MeMV cmv;
    Ipp32s level=GetPlaneLevel(pix);
    MeFrame **fr;
    if(m_cur.RefDir==frw)fr=m_par->pRefF;
    else fr=m_par->pRefB;
    *adr =fr[m_cur.RefIdx] ->plane[level];
    adr->move(Y,(16*m_cur.x+(mv.x>>2))>>level,(16*m_cur.y+(mv.y>>2))>>level);

    if(adr->chroma){
        cmv = GetChromaMV(mv);
        Ipp32s x=(8*m_cur.x+(cmv.x>>2))>>level;
        Ipp32s y=(8*m_cur.y+(cmv.y>>2))>>level;
        adr->move(U,x,y);
        adr->move(V,x,y);
    }

    MakeBlockAdr(level, mt, m_cur.BlkIdx, adr);

    if(pix==ME_HalfPixel || pix==ME_QuarterPixel){
        //all blocks are put at the beginig of the buffer, so MakeBlockAdr is not called
        adr->channel=Y;
        Interpolate(mt, m_par->Interpolation, mv, adr, &m_cur.buf[m_cur.RefDir]);
    }

    if(adr->chroma){ // TODO: remove for double and higher pixel types
        for(int ch=U; ch<=V; ch++){
            adr->channel=ch;
            Interpolate((MeMbPart)(mt>>1), m_par->ChromaInterpolation, cmv, adr, &m_cur.buf[m_cur.RefDir]);
        }
    }
}

void MeBase::MakeBlockAdr(Ipp32s level, MeMbPart mt, Ipp32s BlkIdx, MeAdr* adr)
{
    if(BlkIdx==0)return; //speed up a bit

    static const int offset=4;
    Ipp32s shiftX=0, maskX=0, shiftY=0, maskY=0;
    BlkIdx<<=offset;
    switch(mt){
//        case ME_Mb16x16:
//            return;
        case ME_Mb8x8:
            maskX=1<<offset;
            shiftX=offset-3;
            shiftY=offset-2;
            break;
        case ME_Mb4x4:
            maskX=3<<offset;
            shiftX=offset-2;
            shiftY=offset-0;
            break;
       default:
            SetError((vm_char*)"Wrong mt in MakeBlockAdr\n");
    }
    shiftX+=level;
    shiftY+=level;
    maskY=~maskX;
    
    adr->move((BlkIdx&maskX)>>shiftX,(BlkIdx&maskY)>>shiftY);
}


void MeBase::SetMB16x16B(MeMbType mbt, MeMV mvF, MeMV mvB, Ipp32s cost)
{
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    int i = 0;
    mb->MbPart = ME_Mb16x16;
    mb->McType = ME_FrameMc;
    mb->MbType = (Ipp8u)mbt;
    mb->MbCosts[0] = cost;

    if(m_cur.BestIdx[frw][0] >= 0)
        mb->MVPred[frw][0] = m_cur.PredMV[frw][m_cur.BestIdx[frw][0]][0];
    if(m_cur.BestIdx[bkw][0] >= 0)
        mb->MVPred[bkw][0] = m_cur.PredMV[bkw][m_cur.BestIdx[bkw][0]][0];

    for(i=0; i<4; i++){
        mb->MV[frw][i] = mvF;
        mb->MV[bkw][i] = mvB;
        mb->Refindex[frw][i]= m_cur.BestIdx[frw][0];
        mb->Refindex[bkw][i]= m_cur.BestIdx[bkw][0];
        mb->BlockType[i] = mb->MbType;
    }

    Ipp32s cost_4 = cost/4+1;
    for(i=1; i<5; i++)
        mb->MbCosts[i] = cost_4;

    //transform size
    for(i=5; i>=0; i--){
        mb->BlockTrans<<=2;
        mb->BlockTrans|=3&m_cur.InterTransf[i];
    }

    //quantized coefficients, rearrange during copy
    if(m_par->UseTrellisQuantization){
        if(mbt == ME_MbIntra){
                memcpy(mb->Coeff,m_cur.TrellisCoefficients[4],(6*64)*sizeof(mb->Coeff[0][0]));
        }else{
            for(Ipp32s blk=0; blk<6; blk++){
                MeTransformType tr=m_cur.InterTransf[blk];
                switch(tr){
                    case ME_Tranform4x4:
                        for(Ipp32s subblk=0; subblk<4; subblk++)
                            for(Ipp32s y=0; y<4; y++)
                                for(Ipp32s x=0; x<4; x++)
                                    //MeQuantTable[m_cur.adr][intra?0:1][m_cur.BlkIdx][(32*(subblk/2))+(4*(subblk&1))+8*y+x]=buf2[16*subblk+4*y+x]; 
                                    mb->Coeff[blk][(32*(subblk/2))+(4*(subblk&1))+8*y+x]=m_cur.TrellisCoefficients[tr][blk][16*subblk+4*y+x];
                        break;
                    case ME_Tranform4x8:
                        for(Ipp32s subblk=0; subblk<2; subblk++)
                            for(Ipp32s y=0; y<8; y++)
                                for(Ipp32s x=0; x<4; x++)
                                    mb->Coeff[blk][4*subblk+8*y+x]=m_cur.TrellisCoefficients[tr][blk][32*subblk+4*y+x];
                                    //MeQuantTable[m_cur.adr][intra?0:1][m_cur.BlkIdx][4*subblk+8*y+x]=buf2[32*subblk+4*y+x]; 
                        break;
                    case ME_Tranform8x4:
                    case ME_Tranform8x8:
                        memcpy(mb->Coeff[blk],m_cur.TrellisCoefficients[tr][blk],64*sizeof(mb->Coeff[0][0]));
                        break;
                }
            }
        }
    }
}

void MeBase::SetMB8x8(MeMbType mbt, MeMV mv[4], Ipp32s cost[4])
{
    MeMB *mb=&m_par->pSrc->MBs[m_cur.adr];
    mb->MbPart = ME_Mb8x8;
    mb->McType = ME_FrameMc;
    mb->MbType = (Ipp8u)mbt;
    Ipp32s dir = (mb->MbType == ME_MbFrw) ? frw : bkw;

    for(int i=0; i<4; i++){
        mb->Refindex[dir][i]= m_cur.BestIdx[dir][i];
        mb->MV[dir][i] = mv[i];
        mb->BlockType[i] = (Ipp8u)m_cur.InterType[i];
        mb->MbCosts[i+1] = cost[i]; //0 is for 16x16
        mb->MVPred[dir][i] = m_cur.PredMV[dir][mb->Refindex[dir][i]][i];
    }
 }

void MeBase::SetError(const vm_char *msg, bool condition)
{
    msg = msg;
    condition = condition; //warning suppression
#ifdef ME_DEBUG
    if(!condition)
        return;
    printf("\n\n%s\n\n", msg);
    abort();
#endif
}

void MeBase::EstimateMbInterFullSearch()
{
    FullSearch(m_cur.MbPart, ME_IntegerPixel,0,4*m_par->SearchRange.x,4*m_par->SearchRange.y);
    if(m_par->PixelType != ME_IntegerPixel){
        FullSearch(m_cur.MbPart, m_par->PixelType,m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx],4,4);
    }
}

void MeBase::EstimateMbInterFullSearchDwn()
{
    SetError((vm_char*)"Downsampling is disabled in MeBase::EstimateMbInterFullSearchDwn", !m_InitPar.UseDownSampling);

    //first check predictors
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, 0);
    MeMV mv=m_PredCalc->GetMvA();
    mv.x=(mv.x>>2)<<2;
    mv.y=(mv.y>>2)<<2;
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, mv);
    mv=m_PredCalc->GetMvB();
    mv.x=(mv.x>>2)<<2;
    mv.y=(mv.y>>2)<<2;
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, mv);
    mv=m_PredCalc->GetMvC();
    mv.x=(mv.x>>2)<<2;
    mv.y=(mv.y>>2)<<2;
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, mv);

    DiamondSearch(m_cur.MbPart, ME_IntegerPixel, ME_BigDiamond);
    DiamondSearch(m_cur.MbPart, ME_IntegerPixel, ME_MediumDiamond);
    DiamondSearch(m_cur.MbPart, ME_IntegerPixel, ME_SmallDiamond);

 
    // TODO: choose right upper level!
    Ipp32s UpLevel=2; //for 16x16

    for(Ipp32s l=0; l<=UpLevel; l++){
        m_cur.DwnCost[l] = ME_BIG_COST;
        m_cur.DwnNumOfPoints[l] = 0;
    }

    Ipp32s x0, x1, y0, y1=ME_BIG_COST; //any big number will sute
    x0=y0=-y1; 
    x1=y1; 
    m_PredCalc->TrimSearchRange(m_cur.MbPart,(MePixelType)0, x0, x1, y0, y1);

    //allign to 16 for 16x16 upper level search
    // TODO: add padding check here
    Ipp32s xu0=-((-x0)&(~0xf));
    Ipp32s xu1=x1&(~0xf);
    Ipp32s yu0=-((-y0)&(~0xf));
    Ipp32s yu1=y1&(~0xf);
//    if(x0!=xu0 || y0!=yu0 || x1!=xu1 || y1!=yu1) printf("x vs xu (%d:%d) (%d:%d) (%d:%d) (%d:%d) \n",x0, xu0, y0, yu0, x1, xu1, y1, yu1);

    for(Ipp32s l=UpLevel; l>=0; l--){
        EstimateMbInterOneLevel(l==UpLevel,l, m_cur.MbPart, xu0, xu1, yu0, yu1);
    }

    if(m_par->PixelType == ME_HalfPixel){
        FullSearch(m_cur.MbPart, m_par->PixelType,m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx],4,4);
    }else if(m_par->PixelType == ME_QuarterPixel){
        RefineSearch(m_cur.MbPart, m_par->PixelType);
    }
}


void MeBase::EstimateMbInterOneLevel(bool UpperLevel, Ipp32s level, MeMbPart mt, Ipp32s x0, Ipp32s x1, Ipp32s y0, Ipp32s y1)
{
    Ipp16u sadArr[8192];
    //x0, x1, y0, y1 here are limits for MV, not the pixel coordinates
    MePixelType pix=ME_IntegerPixel;
    switch(level){
        //case 0: pix = ME_IntegerPixel; break;
        case 1: pix = ME_DoublePixel; break;
        case 2: pix = ME_QuadPixel; break;
        case 3: pix = ME_OctalPixel; break;
    }


    MeAdr src, ref;
    MakeRefAdrFromMV(mt, pix, MeMV(x0,y0), &ref);
    MakeSrcAdr(mt,pix,&src);

    Ipp32s step = 4<<level;
    SetError((vm_char*)"Wrong range in EstimateMbInterOneLevel", x0>=x1 || y0>=y1 || (x0&(step-1))!=0 || (x1&(step-1))!=0 || (y0&(step-1))!=0 || (y1&(step-1))!=0);

    Ipp32s NumOfPoints=0;

    //upper level
    if(UpperLevel){

        for(Ipp32s y=y0; y<y1; y+=step,ref.move((x0-x1)>>(level+2),1)){

            Ipp32s numX = (((abs(x1-x0))/step)>>3)<<3;
            GetCost((MeMbPart)(mt>>level), &src, &ref, numX, sadArr);
            Ipp32s x2 = x0;

            for(Ipp32s i=0; i<numX; i++,x2+=step,ref.ptr[Y]+=1){
                Ipp32s cost=(Ipp32s)sadArr[i];
                if(cost<m_cur.DwnCost[level])m_cur.DwnCost[level] = cost;
                m_cur.DwnPoints[level][NumOfPoints].cost = cost;
                m_cur.DwnPoints[level][NumOfPoints].x = (Ipp16s)x2;
                m_cur.DwnPoints[level][NumOfPoints].y = (Ipp16s)y;
                NumOfPoints++;
            }

            for(Ipp32s x=x2; x<x1; x+=step,ref.ptr[Y]+=1){
                Ipp32s cost = GetCost(ME_Sad, (MeMbPart)(mt>>level), &src, &ref);
                if(cost<m_cur.DwnCost[level])m_cur.DwnCost[level] = cost;
                m_cur.DwnPoints[level][NumOfPoints].cost = cost;
                m_cur.DwnPoints[level][NumOfPoints].x = (Ipp16s)x;
                m_cur.DwnPoints[level][NumOfPoints].y = (Ipp16s)y;
                NumOfPoints++;
            }
        }
        m_cur.DwnNumOfPoints[level] =NumOfPoints;
    }else{
        //not upper level
        Ipp32s UpPoints=m_cur.DwnNumOfPoints[level+1];
        SetError((vm_char*)"NumOfPoints==0 in EstimateMbInterOneLevel1", UpPoints ==0);

        //calculate threshold
        Ipp32s MinCost = m_cur.DwnCost[level+1];
        Ipp32s thr = 4*MinCost;
        Ipp32s ActivePoints=0;
        Ipp32s MaxActivePoints=UpPoints/32+16;  //keep at least 16 points
        // TODO: speed up is needed here
        for(;;){
            ActivePoints=0;
            for(Ipp32s p=0; p<UpPoints; p++)
                if(m_cur.DwnPoints[level+1][p].cost<=thr)ActivePoints++;

            if(ActivePoints<=MaxActivePoints)break;
            thr = (thr+MinCost)/2;
            if(thr == MinCost)break;
        }

        //check number of points
        // TODO: improved algorithm is needed
        if(ActivePoints>MaxActivePoints){
            //leave only one point, closest to zero MV
            Ipp32s MinMV=ME_BIG_COST;
            Ipp32s num=0;
            for(Ipp32s p=0; p<UpPoints; p++){
                if(m_cur.DwnPoints[level+1][p].cost==thr){
                    Ipp32s a=abs(m_cur.DwnPoints[level+1][p].x)+abs(m_cur.DwnPoints[level+1][p].y);
                    if(a<MinMV){
                        num=p;
                        MinMV=a;
                    }
                }
            }
            m_cur.DwnPoints[level+1][0]=m_cur.DwnPoints[level+1][num];
            m_cur.DwnNumOfPoints[level+1]=UpPoints=1;
        }

        //estimate points on current level        
        m_cur.ClearMvScoreBoardArr();
        for(Ipp32s p=0; p<UpPoints; p++){
            if(m_cur.DwnPoints[level+1][p].cost>thr)continue;
            Ipp32s xt0, xt1, yt0, yt1;
            xt0=IPP_MAX(m_cur.DwnPoints[level+1][p].x-step,x0);
            xt1=IPP_MIN(xt0+3*step,x1);
            yt0=IPP_MAX(m_cur.DwnPoints[level+1][p].y-step,y0);
            yt1=IPP_MIN(yt0+3*step,y1);
//            xt0=m_cur.DwnPoints[level+1][p].x;
//            xt1=xt0+2*step;
//            yt0=m_cur.DwnPoints[level+1][p].y;
//            yt1=yt0+2*step;

            // TODO: move this up to avoid recalculation
            MakeRefAdrFromMV(mt, pix, MeMV(xt0,yt0), &ref);
            MakeSrcAdr(mt,pix,&src);
            
            m_cur.SetScoreBoardFlag();
            for(Ipp32s y=yt0; y<yt1; y+=step,ref.move((xt0-xt1)>>(level+2),1)){
                for(Ipp32s x=xt0; x<xt1; x+=step,ref.ptr[Y]+=1){
                    if(level!=0){
                        Ipp32s cost = GetCost(ME_Sad, (MeMbPart)(mt>>level), &src, &ref);
                        if(cost<m_cur.DwnCost[level])m_cur.DwnCost[level] = cost;
                        m_cur.DwnPoints[level][NumOfPoints].cost = cost;
                        m_cur.DwnPoints[level][NumOfPoints].x = (Ipp16s)x;
                        m_cur.DwnPoints[level][NumOfPoints].y = (Ipp16s)y;
                        NumOfPoints++;
                    }else{
                        if(!m_cur.CheckScoreBoardMv(pix,MeMV(x,y)))
                        EstimatePointInter(mt, pix, MeMV(x,y));
                    }
                }
            }
            m_cur.ClearScoreBoardFlag();
            m_cur.DwnNumOfPoints[level] =NumOfPoints;
        }
    }
}

void MeBase::EstimateMbInterFast()
{
    /*
    //check predictors
    EstimatePointInter(m_cur.MbPart, ME_IntegerPixel, 0);
    EstimatePointInter(m_cur.MbPart, ME_IntegerPixel, m_cur.PredMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx]);
    EstimatePointInter(m_cur.MbPart, ME_IntegerPixel, m_PredCalc->GetMvA());
    EstimatePointInter(m_cur.MbPart, ME_IntegerPixel, m_PredCalc->GetMvB());
    EstimatePointInter(m_cur.MbPart, ME_IntegerPixel, m_PredCalc->GetMvC());

    if(m_cur.BestCost<m_PredCalc->GetT2()){
        //prediction is good enough, refine
        if(RefineSearch(m_cur.MbPart, ME_IntegerPixel)){
            //search converged, refine to subpixel
            RefineSearch(m_cur.MbPart, m_par->PixelType);
            return;
        }
    }

    //prediction was poor, search bigger area
//    m_cur.BestCost = ME_BIG_COST;
//    m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx] = 0;
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_MediumHexagon);
//    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_SmallHexagon);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_BigDiamond);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_MediumDiamond);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_SmallDiamond);
    RefineSearch(m_cur.MbPart, m_par->PixelType);
    */

    //check predictors
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, 0);
    //EstimatePointInter(m_cur.MbPart, m_par->PixelType, m_cur.PredMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx]);
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, m_PredCalc->GetMvA());
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, m_PredCalc->GetMvB());
    EstimatePointInter(m_cur.MbPart, m_par->PixelType, m_PredCalc->GetMvC());

    MeMV BestMV = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx];
    Ipp32s BestCost = m_cur.BestCost;

    m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx].x =
        (m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx].x >> 2) << 2;
    m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx].y =
        (m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx].y >> 2) << 2;
    m_cur.BestCost = ME_BIG_COST;

    if(BestCost < m_PredCalc->GetT2()){
        //prediction is good enough, refine
        if(RefineSearch(m_cur.MbPart, ME_IntegerPixel)){
            //search converged, refine to subpixel
            RefineSearch(m_cur.MbPart, m_par->PixelType);
            if(BestCost > m_cur.BestCost)
                return;
            else
            {
                m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx] = BestMV;
                m_cur.BestCost = BestCost;
            }
        }
    }

    //prediction was poor, search bigger area
//    m_cur.BestCost = ME_BIG_COST;
//    m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx] = 0;
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_MediumHexagon);
//    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_SmallHexagon);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_BigDiamond);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_MediumDiamond);
    DiamondSearch(m_cur.MbPart,ME_IntegerPixel, ME_SmallDiamond);
    RefineSearch(m_cur.MbPart, m_par->PixelType);
    if(BestCost > m_cur.BestCost)
        return;
    else
    {
        m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx] = BestMV;
        m_cur.BestCost = BestCost;
    }
}



void MeBase::FullSearch(MeMbPart mt, MePixelType pix, MeMV org, Ipp32s RangeX, Ipp32s RangeY)
{
    Ipp32s Step=1, rangeX, rangeY;
    rangeX = RangeX;
    rangeY = RangeY;
    if(pix == ME_QuadPixel) {Step = 16; rangeX = (RangeX/16)*16; rangeY = (RangeY/16)*16;}
    if(pix == ME_IntegerPixel) Step = 4;
    if(pix == ME_HalfPixel) Step = 2;
    if(pix == ME_QuarterPixel) Step = 1;

    for(int y = -rangeY; y<=rangeY; y+=Step )
    {
        for(int x = -rangeX; x<=rangeX; x+=Step )
        {
            EstimatePointInter(mt, pix, org+MeMV(x,y));
        }
    }
}

void MeBase::DiamondSearch(MeMbPart mt, MePixelType pix, MeDiamondType dm)
{
    static const Ipp32s BigDiamondTable[][2] = { {-3,0}, {-2,2}, {0,3}, {2,2}, {3,0}, {2,-2}, {0,-3}, {-2,-2} };
    static const Ipp32s MediumDiamondTable[][2] = { {-2,0}, {-1,1}, {0,2}, {1,1}, {2,0}, {1,-1}, {0,-2}, {-1,-1} };
    static const Ipp32s SmallDiamondTable[][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    static const Ipp32s MediumHexagonTable[][2] = {
                                                {0,4}, {2,3}, {4,2}, {4,1}, {4,0}, {4,-1}, {4,-2}, {2,-3}, {0,-4}, {-2,-3}, {-4,-2}, {-4,-1}, {-4,0}, {-4,1}, {-4,2}, {-2,3},
                                                {0,8}, {4,6}, {8,4}, {8,2}, {8,0}, {8,-2}, {8,-4}, {4,-6}, {0,-8}, {-4,-6}, {-8,-4}, {-8,-2}, {-8,0}, {-8,2}, {-8,4}, {-4,6},
                                                {0,12}, {6,9}, {12,6}, {12,3}, {12,0}, {12,-3}, {12,-6}, {6,-9}, {0,-12}, {-6,-9}, {-12,-6}, {-12,-3}, {-12,0}, {-12,3}, {-12,6}, {-6,9},
                                                {0,16}, {8,12}, {16,8}, {16,4}, {16,0}, {16,-4}, {16,-8}, {8,-12}, {0,-16}, {-8,-12}, {-16,-8}, {-16,-4}, {-16,0}, {-16,4}, {-16,8}, {-8,12}};
    static const Ipp32s SmallHexagonTable[][2] = {
                                                {0,4}, {2,3}, {4,2}, {4,1}, {4,0}, {4,-1}, {4,-2}, {2,-3}, {0,-4}, {-2,-3}, {-4,-2}, {-4,-1}, {-4,0}, {-4,1}, {-4,2}, {-2,3},
                                                {0,8}, {4,6}, {8,4}, {8,2}, {8,0}, {8,-2}, {8,-4}, {4,-6}, {0,-8}, {-4,-6}, {-8,-4}, {-8,-2}, {-8,0}, {-8,2}, {-8,4}, {-4,6}};

    const Ipp32s* Table=NULL;
    Ipp32s TableSize = 0;
    Ipp32s Step = 0;
    if(dm == ME_SmallDiamond)
    {
        Table = (const Ipp32s*)SmallDiamondTable;
        TableSize = sizeof(SmallDiamondTable)/(2*sizeof(Ipp32s));
    }
    if(dm == ME_MediumDiamond)
    {
        Table = (const Ipp32s*)MediumDiamondTable;
        TableSize = sizeof(MediumDiamondTable)/(2*sizeof(Ipp32s));
    }
    if(dm == ME_BigDiamond)
    {
        Table = (const Ipp32s*)BigDiamondTable;
        TableSize = sizeof(BigDiamondTable)/(2*sizeof(Ipp32s));
    }
    if(dm == ME_SmallHexagon)
    {
        Table = (const Ipp32s*)SmallHexagonTable;
        TableSize = sizeof(SmallHexagonTable)/(2*sizeof(Ipp32s));
    }
    if(dm == ME_MediumHexagon)
    {
        Table = (const Ipp32s*)MediumHexagonTable;
        TableSize = sizeof(MediumHexagonTable)/(2*sizeof(Ipp32s));
    }


    if(pix == ME_IntegerPixel)
        Step = 4;
    if(pix == ME_HalfPixel)
        Step = 2;
    if(pix == ME_QuarterPixel)
        Step = 1;

    MeMV   CenterMV = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx];
    for(Ipp32s i=0; i<TableSize; i++)
        EstimatePointInter(mt, pix, CenterMV + MeMV(Step*Table[2*i+0], Step*Table[2*i+1]));
}


bool MeBase::RefineSearch(MeMbPart mt, MePixelType pix)
{
    SetError((vm_char*)"Wrong mt in RefineSearch", mt != ME_Mb8x8 && mt != ME_Mb16x16);

    switch(pix){
        case ME_IntegerPixel:  //this means refinement after prediction.
            for(int j=0; j<2; j++)
            { //maximum number of big steps that are allowed
                MeMV CenterMV = m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx];
                DiamondSearch(mt,pix, ME_MediumDiamond);

                //check if best MV in center
                if(CenterMV == m_cur.BestMV[m_cur.RefDir][m_cur.RefIdx][m_cur.BlkIdx])
                {
                    //refine by small diamond
                    DiamondSearch(mt,pix, ME_SmallDiamond);
                    return true;
                }
            }
            return false;

        case ME_HalfPixel:
            DiamondSearch(mt,pix, ME_MediumDiamond);
            DiamondSearch(mt,pix, ME_SmallDiamond);
            return true;

        case ME_QuarterPixel:
            DiamondSearch(mt,pix, ME_BigDiamond);
            DiamondSearch(mt,pix, ME_MediumDiamond);
            DiamondSearch(mt,pix, ME_SmallDiamond);
            return true;
        default:
            SetError((vm_char*)"Wrong pix in RefineSearch");
    }
    return false;
}

///Close motion estimator.
///Free all allocated memory.
void MeBase::Close()
{
    //save presets
    #ifdef ME_GENERATE_PRESETS    
    Ipp32s idx = QpToIndex(MeQP, MeQPUNF);
    pStat->SaveRegression((Ipp8u*)"ME_RG_DInter", 0, NULL, NULL, 0, idx, &m_InterRegFunD);
    pStat->SaveRegression((Ipp8u*)"ME_RG_RInter", 0, NULL, NULL, 0, idx, &m_InterRegFunR);
    pStat->SaveRegression((Ipp8u*)"ME_RG_JInter", 0, NULL, NULL, 0, idx, &m_InterRegFunJ);
    pStat->SaveRegression((Ipp8u*)"ME_RG_RMV",    0, NULL, NULL, 0, idx, &m_MvRegrFunR);
    pStat->SaveRegression((Ipp8u*)"ME_RG_DIntra", 0, NULL, NULL, 0, idx, &m_IntraRegFunD);
    pStat->SaveRegression((Ipp8u*)"ME_RG_RIntra", 0, NULL, NULL, 0, idx, &m_IntraRegFunR);
    #endif

    free(m_OwnMem);
    m_AllocatedSize = 0;
    m_OwnMem = NULL;
}

bool MeBase::CheckParams()
{
    m_par->MbPart |= ME_Mb16x16;


    //feedback and downsampling check
//    if(m_par->UseFeedback && (m_par->SearchDirection != ME_ForwardSearch || m_par->MbPart != ME_Mb16x16))
//        return false;

    if(m_par->UseFeedback && m_par->UpdateFeedback && !m_InitPar.UseStatistics)
        return false;

    if(m_par->UseDownSampledImageForSearch && !m_InitPar.UseDownSampling)
        return false;
   
    

    // TODO: add check for CostMetric
    // TODO: add check for enabled statistic in MEStat
    if(m_par->SkippedMetrics != ME_Maxad && m_par->SkippedMetrics != ME_Sad
       && m_par->SkippedMetrics != ME_Sadt && m_par->SkippedMetrics != ME_VarMean)
       return false;
    if(m_par->SkippedMetrics == ME_Maxad)
        m_SkippedThreshold = m_par->Quant/4;
    else if(m_par->SkippedMetrics == ME_Sad)
        m_SkippedThreshold = 150*m_par->Quant/4;
    else if(m_par->SkippedMetrics == ME_Sadt)
        m_SkippedThreshold = m_par->Quant/2;
    else if(m_par->SkippedMetrics == ME_VarMean)
        m_SkippedThreshold = m_par->Quant;

    //check actual request
    //check if current input parameters violate Init agreement
    if( m_par->SearchRange.x < 8 || m_par->SearchRange.x > 256 ||
        m_par->SearchRange.y < 8 || m_par->SearchRange.y > 256)
        return false;
    if( m_par->Interpolation != ME_VC1_Bicubic && m_par->Interpolation != ME_VC1_Bilinear
        && m_par->Interpolation != ME_H264_Luma && m_par->Interpolation != ME_H264_Chroma
        && m_par->Interpolation != ME_AVS_Luma)
        return false;
    if((m_par->MbPart & ME_Mb16x8) || (m_par->MbPart & ME_Mb8x16) || (m_par->MbPart & ME_Mb4x4))
        return false;

    if(m_par->PredictionType != ME_VC1 && m_par->PredictionType != ME_VC1Hybrid && m_par->PredictionType != ME_MPEG2
        && m_par->PredictionType != ME_VC1Field1 && m_par->PredictionType != ME_VC1Field2 &&
        m_par->PredictionType != ME_VC1Field2Hybrid && m_par->PredictionType != ME_AVS)
        return false;

    //check input ptrs
//    if( m_par->pSrc->plane[0].ptr[Y] == NULL)
//        return false;
//    if( (m_par->SearchDirection == ME_ForwardSearch || m_par->SearchDirection == ME_BidirSearch)&& (m_par->pRefF==NULL || m_par->pRefF[0]->ptr[0]==NULL))
//        return false;
//    if( m_par->SearchDirection == ME_BidirSearch && (m_par->pRefB==NULL || m_par->pRefB[0]->ptr[0]==NULL))
//        return false;

    Ipp32s NMB =  m_par->pSrc->WidthMB*m_par->pSrc->HeightMB;
    if(m_par->FirstMB ==-1 && m_par->LastMB==-1){
        m_par->FirstMB =0;
        m_par->LastMB = NMB-1;
    }else if( m_par->FirstMB < 0 || m_par->LastMB >= NMB ||  m_par->FirstMB>m_par->LastMB)
        return false;

    //check stupid errors
    if(!(m_par->MbPart & ME_Mb16x16) && !(m_par->MbPart & ME_Mb8x8)) return false;
    if(m_par->SearchDirection == ME_BidirSearch && (m_par->MbPart & ME_Mb8x8)) return false;
    if(m_par->SearchDirection != ME_ForwardSearch && m_par->SearchDirection != ME_BidirSearch) return false;
    if(m_par->PixelType != ME_IntegerPixel && m_par->PixelType != ME_HalfPixel && m_par->PixelType != ME_QuarterPixel) return false;
    if(m_par->PixelType == ME_HalfPixel && m_par->Interpolation != ME_VC1_Bilinear
       && m_par->Interpolation != ME_H264_Luma && m_par->Interpolation != ME_H264_Chroma
       && m_par->Interpolation != ME_AVS_Luma)return false;

    //VC1 specific
    m_cur.qp8=(m_par->Quant/2)>8?1:0;
    m_cur.CbpcyTableIndex = m_par->CbpcyTableIndex;
    m_cur.MvTableIndex = m_par->MvTableIndex;
    m_cur.AcTableIndex = m_par->AcTableIndex;

    return true;
 }

Ipp32s MeBase::WeightMV(MeMV mv, MeMV predictor)
{
    static const Ipp32s cost_table[128] =
                    { 2, 4, 8, 8,12,12,12,12,
                     16,16,16,16,16,16,16,16,
                     20,20,20,20,20,20,20,20,
                     20,20,20,20,20,20,20,20,
                     24,24,24,24,24,24,24,24,
                     24,24,24,24,24,24,24,24,
                     24,24,24,24,24,24,24,24,
                     24,24,24,24,24,24,24,24,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28,
                     28,28,28,28,28,28,28,28
                     };

    Ipp32s d = abs(mv.x-predictor.x) + abs(mv.y-predictor.y);
    if(d > 127) return 32;
    return cost_table[d & 0x0000007f];
}

Ipp32s MeBase::QpToIndex(Ipp32s QP, bool UniformQuant)
{
    // 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,54,58,62
    // 1,2,...,36

    //convert all QP to 1-72 range, 0 means unsupported combination
    //from 1 to 36 are uniform quants, from 37 to 72 non uniform
    Ipp32s idx=0;
    if(QP>=2 && QP <=17)idx=QP-1; //1-16
    else if(QP>=18 && QP <=50 && (QP&1)==0) idx=(QP-18)/2+17; //17-33
    else if(QP==54 ||  QP==58 || QP==62) idx=(QP-54)/4+34; //34-36
    if(idx==0){
        printf("Wrong QP=%d Unif=%d\n", QP, UniformQuant);
        SetError((vm_char*)"Wrong QP");
    }

    if(!UniformQuant)
        idx+=36;

//    printf("QpToIndex %d %d > %d\n", QP, UniformQuant, idx);
    return idx;
}

void MeBase::LoadRegressionPreset()
{
    MeRegrFunSet  RegSet=ME_RG_EMPTY_SET;
    if(!m_par->UseFeedback) return;
    if(m_par->UseFastFeedback)RegSet=ME_RG_VC1_FAST_SET;
    else RegSet=ME_RG_VC1_PRECISE_SET;

    if (m_par->PredictionType == ME_AVS)
        RegSet=ME_RG_AVS_FAST_SET;

    if(RegSet!=m_CurRegSet){
        m_CurRegSet = RegSet;
        #ifndef ME_GENERATE_PRESETS
            m_InterRegFunD.LoadPresets(m_CurRegSet,ME_RG_DInter);
            m_InterRegFunR.LoadPresets(m_CurRegSet,ME_RG_RInter);
            m_InterRegFunJ.LoadPresets(m_CurRegSet,ME_RG_JInter);
            m_IntraRegFunD.LoadPresets(m_CurRegSet,ME_RG_DIntra);
            m_IntraRegFunR.LoadPresets(m_CurRegSet,ME_RG_RIntra);
            m_MvRegrFunR.LoadPresets(m_CurRegSet,ME_RG_RMV);
        #endif    
    }

    RegrFun::SetQP(QpToIndex(m_par->Quant, m_par->UniformQuant));
#ifndef ME_USE_BACK_DOOR_LAMBDA
        if(m_par->UniformQuant){
            m_lambda= 10*m_par->Quant*m_par->Quant;
        }else{
            m_lambda= 35*m_par->Quant*m_par->Quant;
        }
        if (ME_AVS == m_par->PredictionType)
            m_lambda= m_par->Quant*m_par->Quant;

        if(m_lambda<0) m_lambda=0;
        m_lambda /= 100.;
    #ifdef ME_DEBUG
            MeLambda=100*m_lambda;
    #endif
#endif
}

void MeBase::ProcessFeedback(MeParams *pPar)
{
    // TODO: add check that statistic is valid
    //skip I&B frames for now
    MeFrame *pFrame = pPar->pSrc;

//    printf("ProcessFeedback %d\n", pFrame->type);
    if(pFrame->type == ME_FrmIntra) return;
//    if(pFrame->type != ME_FrmFrw) return;
    
    //calculate distortion
    // TODO: add check here that pFrame->SrcPlane != pFrame->RecPlane
    for(Ipp32s y=0; y<pFrame->HeightMB; y++){
        for(Ipp32s x=0; x<pFrame->WidthMB; x++){
            Ipp32s adr = pFrame->WidthMB*y + x;
            Ipp8u* pS=pFrame->SrcPlane.at(Y,16*x,16*y);//source MB
            Ipp8u* pR=pFrame->RecPlane.at(Y,16*x,16*y);; //reconstructed MB

            Ipp32s sum=0;
            ippiSqrDiff16x16_8u32s(pS, pFrame->SrcPlane.step[Y], pR, pFrame->RecPlane.step[Y], IPPVC_MC_APX_FF, &sum);
            pFrame->stat[adr].dist = sum;
        }
    }

#ifdef ME_DEBUG
    pStat->AddFrame(pPar);
#endif

    if(!pPar->UpdateFeedback)
        return;

    Ipp32s x[16384];
    Ipp32s y[16384];
    Ipp32s NumOfMB=pFrame->WidthMB*pFrame->HeightMB, N;
#ifdef ME_DEBUG
    Ipp32s QpIdx=QpToIndex(pPar->Quant, pPar->UniformQuant);
#endif

    static int counter=0;   
    counter++;

#ifdef ME_DEBUG
    bool SaveRegr = false;
    #ifdef ME_GENERATE_PRESETS    
    SaveRegr = false;
    #endif
#endif

    //1 INTER
    int MvSize=0;
    int AcSize=0;
    int WholeSize=0;
    int CbpSizeInter=0;
    int MvCbpSizeIntra=0;
    int nInter=0,nIntra=0;

    N=0; 
    for(Ipp32s i=0; i<NumOfMB; i++){
        //whole frame statistics
        Ipp32s coeffS=pFrame->stat[i].coeff[0]+pFrame->stat[i].coeff[1]+pFrame->stat[i].coeff[2]+pFrame->stat[i].coeff[3]+pFrame->stat[i].coeff[4]+pFrame->stat[i].coeff[5];
        if(pFrame->MBs[i].MbType == ME_MbIntra){
            nIntra++;
            AcSize+=coeffS;
            MvCbpSizeIntra+=pFrame->stat[i].whole-coeffS-2; // 2=1(for skip) + 1 for AC pred
            WholeSize+=pFrame->stat[i].whole;
        }

        if(pFrame->MBs[i].MbType == ME_MbFrw){
            nInter++;
            AcSize+=coeffS;
            MvSize+=pFrame->stat[i].MVF[0];
            CbpSizeInter += pFrame->stat[i].whole - coeffS - pFrame->stat[i].MVF[0] - 1; //for skip, also hibrid should be taken into account
            WholeSize+=pFrame->stat[i].whole;
        }
        
        if(pFrame->MBs[i].MbType == ME_MbFrw){
//        if(pFrame->MBs[i].MbType == ME_MbFrw && pFrame->MBs[i].MbCosts[0]!=0){ //zero cost lead to terrible regression, avoid it
            x[N] = pFrame->MBs[i].InterCostRD.D;
            //x[N] = pFrame->MBs[i].MbCosts[0]; //SAD
            if(m_par->UseFastFeedback)
                y[N] = (Ipp32s)(pFrame->stat[i].dist+m_lambda*(pFrame->stat[i].whole-pFrame->stat[i].MVF[0]));
            else
                y[N] = pFrame->stat[i].dist;
                
            //printf("[%d] D Inter  %5d, %5d dis=%d rate=%d mv=%d L=%f\n",i, x[N], y[N],pFrame->stat[i].dist, pFrame->stat[i].whole,pFrame->stat[i].MVF[0],m_lambda );
            N++;
        }
    }
    m_InterRegFunD.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_DInter", counter, x, y, N, QpIdx, &m_InterRegFunD);
    //printf("Inter Mv=%5d Ac=%5d Whole=%5d CbpSizeInter=%5d MvCbpSizeIntra=%5d  Mv+Cbp=%5d\n",  MvSize, AcSize, WholeSize, CbpSizeInter, MvCbpSizeIntra, MvSize+CbpSizeInter+MvCbpSizeIntra);
#endif

    N=0;
    for(Ipp32s i=0; i<NumOfMB && !m_par->UseFastFeedback; i++){
        if(pFrame->MBs[i].MbType == ME_MbFrw){
            //x[N] = pFrame->MBs[i].MbCosts[0];
            x[N] = pFrame->MBs[i].InterCostRD.R;
            //x[N] = pFrame->MBs[i].HdmrdCost;
//            y[N] = pFrame->stat[i].coeff[0] + pFrame->stat[i].coeff[1]+pFrame->stat[i].coeff[2]+pFrame->stat[i].coeff[3]+pFrame->stat[i].coeff[4]+pFrame->stat[i].coeff[5];
            y[N] = pFrame->stat[i].whole-pFrame->stat[i].MVF[0];
            //y[N] = pFrame->stat[i].whole;
            //printf("R inter %5d, %5d\n",x[N], y[N] );
            N++;
        }
    }
    m_InterRegFunR.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_RInter", counter, x, y, N, QpIdx, &m_InterRegFunR);
#endif


    N=0;
    for(Ipp32s i=0; i<NumOfMB; i++){
        if(pFrame->MBs[i].MbType == ME_MbFrw){
            x[N] = pFrame->MBs[i].PureSAD;
            //y[N] = (Ipp32s)( m_InterRegFunD.Weight(pFrame->MBs[i].InterCostD)+m_lambda*(m_InterRegFunR.Weight(pFrame->MBs[i].InterCostR)));
            y[N] = (Ipp32s)(pFrame->stat[i].dist+m_lambda*(pFrame->stat[i].whole-pFrame->stat[i].MVF[0]));
            N++;
        }
    }
    m_InterRegFunJ.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_JInter", counter, x, y, N, QpIdx, &m_InterRegFunJ);
#endif

    N=0;
    for(Ipp32s i=0; i<NumOfMB && m_par->UseFastFeedback; i++){
        if(pFrame->MBs[i].MbType == ME_MbFrw){
           x[N] = abs(pFrame->MBs[i].MV[frw][0].x - pFrame->MBs[i].MVPred[frw][0].x)+abs(pFrame->MBs[i].MV[frw][0].y - pFrame->MBs[i].MVPred[frw][0].y);
//            x[N] = GetMvSize(pFrame->MBs[i].MV[frw][0].x - pFrame->MBs[i].MVPred[frw][0].x,pFrame->MBs[i].MV[frw][0].y - pFrame->MBs[i].MVPred[frw][0].y,i);
//            x[N] =  MVSizeForAllMB[i];
 
            y[N] = pFrame->stat[i].MVF[0];
            //printf("%5d, %5d\n",x[N], y[N] );
            N++;
        }
    }
    m_MvRegrFunR.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_RMV", counter, x, y, N, QpIdx, &m_MvRegrFunR);
#endif

    //1 INTRA
    N=0;
    for(Ipp32s i=0; i<NumOfMB; i++){
        if(pFrame->MBs[i].MbType == ME_MbIntra){
            x[N] = pFrame->MBs[i].IntraCostRD.D;
            //x[N] = pFrame->MBs[i].MbCosts[0];
            if(m_par->UseFastFeedback)
                y[N] = (Ipp32s)(pFrame->stat[i].dist+m_lambda*pFrame->stat[i].whole);
            else
                y[N] = pFrame->stat[i].dist;
            //printf("%5d, %5d\n",x[N], y[N] );
            N++;
        }
    }
    m_IntraRegFunD.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_DIntra", counter, x, y, N, QpIdx, &m_IntraRegFunD);
#endif

    N=0;
    for(Ipp32s i=0; i<NumOfMB && !m_par->UseFastFeedback; i++){
        if(pFrame->MBs[i].MbType == ME_MbIntra){
            //x[N] = pFrame->MBs[i].MbCosts[0];
            x[N] = pFrame->MBs[i].IntraCostRD.R;
            //y[N] = pFrame->stat[i].coeff[0] + pFrame->stat[i].coeff[1]+pFrame->stat[i].coeff[2]+pFrame->stat[i].coeff[3]+pFrame->stat[i].coeff[4]+pFrame->stat[i].coeff[5];;
            y[N] = pFrame->stat[i].whole;
            //printf("%5d, %5d\n",x[N], y[N] );
            N++;
        }
    }
    m_IntraRegFunR.ProcessFeedback(x, y, N);
#ifdef ME_DEBUG
    if(SaveRegr)pStat->SaveRegression((Ipp8u*)"ME_RG_RIntra", counter, x, y, N, QpIdx, &m_IntraRegFunR);
#endif

}


//*******************************************************************************************************
//1 feedback implementation
Ipp32s RegrFun::m_QP;

RegrFun::RegrFun(Ipp32s /*dx*/, Ipp32s N)
{

    if(N>NumOfRegInterval){
        printf("RegrFun::RegrFun N > NumOfRegInterval\n");
        abort();
    }

    // TODO: check this!
    //m_dx = dx;
    m_num=N;
    m_QP=0;

    for(Ipp32s i=0; i<MaxQpValue; i++){
        m_a[i][0] = -1; //it means that function is not initialized
    }
}


void RegrFun::ProcessFeedback(Ipp32s *x, Ipp32s *y, Ipp32s N)
{
    if(N==0)return;
    
    //compute regression history ver.2
    if(m_a[m_QP][0]<0){
        //regr is uninitialized. It is possible only during preset calculation.
        printf("Regr was not initialised!!!\n");

        //find out step
        Ipp32s xMax=0;
        for(Ipp32s i=0; i<N; i++) xMax = IPP_MAX(xMax,x[i]);
        m_dx[m_QP] = 1+xMax/m_num;

        //reset history
        for(Ipp32s i=0; i<m_num; i++){ //initialize
            m_ax[m_QP][i]=0;
            m_ay[m_QP][i]=0;
            m_an[m_QP][i]=0;
        }
    }else {
        //reduce history size
        Ipp64f size=0;
        for(int i=0; i<m_num; i++)
            size+= m_an[m_QP][i];

        // TODO: set TargetHistorySize from without
        Ipp64f reduce=0;
        //Ipp32s FrameSize=1350; //number of MB in frame
        //Ipp32s TargetHistorySize = 10*FrameSize; //number of MB in history
        reduce = (m_TargetHistorySize-N)/(size+0.01);
        //printf("reduce=%f",reduce);
        //printf("size=%8.1f reduce=%11.2f\n", size, reduce);
        if(reduce<1.0){
            //reduce
            for(Ipp32s i=0; i<m_num; i++){
                m_an[m_QP][i]*=reduce;
            }
        }
        //unaverage :-)
        for(Ipp32s i=0; i<m_num; i++){
            m_ax[m_QP][i]*=m_an[m_QP][i];
            m_ay[m_QP][i]*=m_an[m_QP][i];
        }
    }


    for(Ipp32s i=0; i<N; i++){ //sum
        Ipp32s k=x[i]/m_dx[m_QP];
        if(k<0)k=0;
        if(k>=m_num) k= m_num-1;
        m_ax[m_QP][k]+=x[i];
        m_ay[m_QP][k]+=y[i];
        m_an[m_QP][k]+=1;
    }


    m_FirstData=m_num;
    m_LastData=0;
    for(Ipp32s i=0; i<m_num; i++){ //avrg
        if(m_an[m_QP][i]!=0){
            m_ax[m_QP][i]/=m_an[m_QP][i];
            m_ay[m_QP][i]/=m_an[m_QP][i];
            if(i<m_FirstData)m_FirstData=i;
            if(i>m_LastData)m_LastData=i;
        }
        //printf("[%2d] n=%7.2f x=%7.2f y=%7.2f\n", i, m_an[m_QP][i], m_ax[m_QP][i], m_ay[m_QP][i]);
        //printf("%7.2f,%7.2f,%7.2f\n", m_ax[m_QP][i], m_ay[m_QP][i],m_an[m_QP][i]);
    }

    //process for all pieces
    for(Ipp32s i=0; i<m_num; i++)
        ComputeRegression(i, x, y, N);
    AfterComputation();
}


Ipp32s RegrFun::Weight(Ipp32s x)
{
    if(m_a[m_QP][0]<0){
        //printf("Error function is not initialized");
        return x; //function is not initialized
    }

    Ipp32s i=x/m_dx[m_QP];
    if(i<0)i=0;
    if(i>=m_num) i= m_num-1;

    Ipp32s y=(Ipp32s)(m_a[m_QP][i]*x + m_b[m_QP][i]);

    //printf("QP in RegrFun::Weight=%d dx=%d x=%d i=%d y=%d\n", m_QP,m_dx[m_QP],x, i, y);

    
    if(y<0)return 0;
    if(y>ME_BIG_COST)return ME_BIG_COST;
    return y;
}

void RegrFun::LoadPresets(MeRegrFunSet set, MeRegrFun id)
{
    Ipp32s adr;
    Ipp32s *p = MeVc1PreciseRegrTable;
    Ipp32s size=MeVc1PreciseRegrTableSize;

    if(set == ME_RG_VC1_FAST_SET){
        p=MeVc1FastRegrTable;
        size=MeVc1FastRegrTableSize;
    }

    Ipp32s StepForId = 5+m_num+1;
    for(Ipp32s qp=0; qp<MaxQpValue; qp++){
        //find preset
        for(adr=0;  (p[adr]!=id ||p[adr+1]!=qp) && adr<size; adr += StepForId);
        if(adr>=size) continue;

        //check preset
        Ipp32s sum = 0;
        for(Ipp32s i=1; i<StepForId-1; i++)
            sum+=p[adr+i];
        if(p[adr+4]!=m_num || p[adr+StepForId-1]!=sum){
            printf("wrong preset data! id=%d qp=%d\n", id, qp);
            continue;
        }

        //load preset
        m_dx[qp]=p[adr+2];
        Ipp32s dy=p[adr+3];
        Ipp32s yPrev = p[adr+5];
        for(Ipp32s i=0; i<m_num-1; i++){
            Ipp64f x0=i*m_dx[qp];
            Ipp64f x1=(i+1)*m_dx[qp];
            Ipp64f y0=yPrev;
            yPrev=yPrev+p[adr+5+i+1]+dy;
            Ipp64f y1=yPrev;
            m_a[qp][i] = (y0-y1)/(x0-x1);
            m_b[qp][i] = y0 - m_a[qp][i] * x0;
            m_ax[qp][i] = (x0+x1)/2;
            m_ay[qp][i] = (y0+y1)/2;
            m_an[qp][i] = 1; // TODO: check this value, should it be related to m_TargetHistorySize
        }
        //filll in last bin
        m_a[qp][m_num-1] = m_a[qp][m_num-2];
        m_b[qp][m_num-1] = m_b[qp][m_num-2];
        m_ax[qp][m_num-1] = 0;
        m_ay[qp][m_num-1] = 0;
        m_an[qp][m_num-1] = 0;
    }
}


Ipp64f LowessRegrFun::WeightPoint(Ipp64f x)
{
    //tricube function
    Ipp64f t=x<0?-x:x;
    if(t>=1.) return 0.;
    t = 1 - t*t*t;
    return t*t*t;
}


// regression based on averaged subset of input samples
void LowessRegrFun::ComputeRegression(Ipp32s I, Ipp32s* /*x*/, Ipp32s* /*y*/, Ipp32s /*N*/)
{
    Ipp64f SSxx=0., SSyy=0., SSxy=0;
    Ipp64f avx =0., avy=0., r2=0., a=0., b=0.;
    Ipp64f sw=0; //weight and sum of all weights
    Ipp64f x0= I*m_dx[m_QP];


    //check if this bin is inside data range
    if(I<m_FirstData){
        m_a[m_QP][I] = 0;
        m_b[m_QP][I] = 0;
        return;
    }else if(I>m_LastData){
        m_a[m_QP][I]=m_a[m_QP][I-1];
        m_b[m_QP][I]=m_b[m_QP][I-1];
        return;
    }


    //find current history size
    Ipp64f size=0;
    for(int i=0; i<m_num; i++)
        size+= m_an[m_QP][i];
    if(size<10){ // TODO: is 10 ok?
        m_a[m_QP][0]=-1;
        return;
    }

    //find adaptive bandwidth, that include at least "Bandwidth" percents of samples.
    Ipp64f bw = 0, sn=0;
    for(Ipp32s i=0; i<m_num; i++)
        for(Ipp32s pass=0; pass<2; pass++){
            Ipp32s j;
            if(pass==0)
                j= I+i;
            else
                j= I-i-1;

            if(sn>Bandwidth*size/100.){
                //printf("break\n");
                break;
            }

            if(0<=j && j<m_num){
                sn+=m_an[m_QP][j];
                bw = m_dx[m_QP]*abs(j-I);
                //printf("sn=%f bw=%f const=%d\n",sn,bw,Bandwidth);
            }
        }

    //check bandwidth, it should not be too narrow. 3 here means about 6 bins span
    if(bw<3*m_dx[m_QP])
        bw=3*m_dx[m_QP];

    //find lowess for one point
    //find average and weight
    Ipp64f snL=0, snR=0, skewness;
    for(Ipp32s i=0; i<m_num; i++){
        Ipp64f w = m_an[m_QP][i]*WeightPoint((m_ax[m_QP][i]-x0)/bw);
        sw += w;
        if(i<I)snL+=w;
        if(i>I)snR+=w;

        avx += w*m_ax[m_QP][i];
        avy += w*m_ay[m_QP][i];
    }
    skewness = (snL+0.00001)/(snR+0.00001);
    skewness = IPP_MAX(skewness,1/skewness);

    if(sw>5.){ //sw<5 is extrimly rare case
        //enough points to compute regression
        avx /= sw;
        avy /= sw;

        for(Ipp32s i=0; i<m_num; i++){
            Ipp64f w = m_an[m_QP][i]*WeightPoint((m_ax[m_QP][i]-x0)/bw);

            SSxx += w*(m_ax[m_QP][i] - avx)*(m_ax[m_QP][i] - avx);
            SSyy += w*(m_ay[m_QP][i] - avy)*(m_ay[m_QP][i] - avy);
            SSxy += w*(m_ay[m_QP][i] - avy)*(m_ax[m_QP][i] - avx);
        }

        SSxx /= sw;
        SSyy /= sw;
        SSxy /= sw;

        a=SSxy/SSxx;
        b=avy-a*avx;
        r2=SSxy*SSxy/(SSxx*SSyy);
    }
//    if(sw<=5. || a<0.){
    if(sw<=5.){
        //bad regression in this bin, just repeat previous segment
        if(I==0){
            a=0; b=0;
        }else{
            a=m_a[m_QP][I-1];
            b=m_b[m_QP][I-1];
        }
    }

    m_a[m_QP][I] = a;
    m_b[m_QP][I] = b;

    //printf("[%3d] n=%d sn=%7.2f bw=%7.2f  a=%7.2f b=%9.2f r2=%7.2f  sw=%9.2f SSxx=%9.2f sk=%7.2f\n", I, N, sn, bw, a, b, r2, sw, SSxx, skewness);

}

void LowessRegrFun::AfterComputation()
{
    //remove all decreasing segments from the begining of the sequence
    for(int i=0; i<m_num; i++){
        if(m_a[m_QP][i]==0)continue;
        if(m_a[m_QP][i]>0)break;
        //printf("%d %f %f was zeroved\n", i, m_a[m_QP][i],m_b[m_QP][i]);
        m_a[m_QP][i]=0; m_b[m_QP][i]=0;
    }

    //fill in first empty bins by value from first bin with data
    int FirstData=0; //it is not nessesary equal to m_FirstData!
    for(; FirstData<m_num && m_a[m_QP][FirstData]==0; FirstData++);

    for(Ipp32s i=0; i<FirstData; i++){
        m_a[m_QP][i] = m_a[m_QP][FirstData];
        m_b[m_QP][i] = m_b[m_QP][FirstData];
    }

    //save org coefficients
    Ipp64f a[NumOfRegInterval], b[NumOfRegInterval];
    for(Ipp32s i=0; i<m_num; i++){
        a[i]=m_a[m_QP][i];
        b[i]=m_b[m_QP][i];
    }

    //remove spikes on bins borders
    for(Ipp32s i=0; i<m_num; i++){
        Ipp64f x0, x1, y0, y1;
        x0 = i*m_dx[m_QP];
        x1 = (i+1)*m_dx[m_QP];

        y0=a[i]*x0+b[i];
        if(i!=0)
            y0 = (y0+a[i-1]*x0+b[i-1])/2;

        y1=a[i]*x1+b[i];
        if(i!=m_num-1)
            y1 = (y1+a[i+1]*x1+b[i+1])/2;

        //printf("y0=%f y1=%f\n", y0, y1);
        m_a[m_QP][i] = (y0-y1)/(x0-x1);
        m_b[m_QP][i] = y0 - m_a[m_QP][i] * x0;

        //all functions in ME are increasing, so check that a>0
        if(m_a[m_QP][i]<0){
            //printf("a<0 corrected!\n");
            if(i==0){
                m_a[m_QP][i]=0; m_b[m_QP][i]=0;
            }else{
                //repead previous bin with decreased slope
                m_a[m_QP][i] = 0.7*m_a[m_QP][i-1];
                m_b[m_QP][i] = m_b[m_QP][i-1]+(m_a[m_QP][i-1]-m_a[m_QP][i])*x0;
            }
            //correct org coefficient
            if(i!=m_num-1){
                a[i]=0;
                b[i]=2*(m_a[m_QP][i]*x1+m_b[m_QP][i])-a[i+1]*x1-b[i+1];
                ///printf("a[i]=%f b[i]=%f\n", a[i], b[i]);
            }
        }

        //printf("adj [%3d] a=%7.2f b=%9.2f \n", i, m_a[m_QP][i], m_b[m_QP][i]);
    }

}

Ipp32s MvRegrFun::Weight(MeMV mv, MeMV pred)
{

    Ipp32s x = abs(mv.x-pred.x) + abs(mv.y-pred.y);
    return RegrFun::Weight(x);
}
