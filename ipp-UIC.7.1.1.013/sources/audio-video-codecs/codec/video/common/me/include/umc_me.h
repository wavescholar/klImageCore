/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//          motion estimation
//
*/

#ifndef _UMC_ME_H_
#define _UMC_ME_H_

#include "umc_me_structures.h"

#define MB_NUM_ELEMENTS 256
#define B_NUM_ELEMENTS  64
#define ME_MB_ADR (999999)
#define ME_MB_IF_ADR(x) (false)


namespace UMC
{

class MePredictCalculator
{
public:
    DYNAMIC_CAST_DECL_BASE(MePredictCalculator);

    //MePredictCalculator();
    void Init(MeParams* pMeParams, MeCurrentMB* pCur); //just save external data pointers

    virtual MeMV GetPrediction();
    virtual MeMV GetPrediction(MeMV mv);
    virtual MeMV GetPrediction16x16() = 0;
    virtual MeMV GetPrediction8x8() = 0;

    virtual MeMV GetMvA(){return AMV;}
    virtual MeMV GetMvB(){return BMV;}
    virtual MeMV GetMvC(){return CMV;}
    virtual void ResetPredictors(void){return;}
    virtual MeMV GetDef_FieldMV(int /*RefDir*/, int /*BlkIdx*/){return MeMV(0);}
    virtual bool GetDef_Field(int /*RefDir*/, int /*BlkIdx*/){return false;}
    virtual void SetDefFrwBkw(MeMV &mvF, MeMV &mvB)
    {
        mvF = mvB = MeMV(0);
    };

    bool IsOutOfBound(MeMbPart mt, MePixelType pix, MeMV mv);
    void TrimSearchRange(MeMbPart mt, MePixelType pix, Ipp32s &x0, Ipp32s &x1, Ipp32s &y0, Ipp32s &y1 );
    Ipp32s GetT2();
    // only for AVS
    Ipp32s m_blockDist[2][4];                      // (Ipp32s [][]) block distances, depending on ref index
    Ipp32s m_distIdx[2][4];                        // (Ipp32s [][]) distance indecies of reference pictures

protected:
    inline Ipp16s median3( Ipp16s a, Ipp16s b, Ipp16s c ){return IPP_MIN(a,b)^IPP_MIN(b,c)^IPP_MIN(c,a);};

    void SetMbEdges();

    MeMV GetCurrentBlockMV(int isBkw, Ipp32s idx);
    Ipp32s GetT2_16x16();
    Ipp32s GetT2_8x8();

    //pointers to external data
    MeParams*    m_pMeParams;
    MeCurrentMB* m_pCur;
    MeMB*        m_pRes;

    //macro-block/block location status
    bool  MbLeftEnable;
    bool  MbTopEnable;
    bool  MbRightEnable;
    bool  MbTopLeftEnable;
    bool  MbTopRightEnable;

    bool  isAOutOfBound;
    bool  isBOutOfBound;
    bool  isCOutOfBound;

    bool  AMVbSecond;//is AMV the second field vector
    bool  BMVbSecond;//is BMV the second field vector
    bool  CMVbSecond;//is CMV the second field vector

    MeMV  AMV; //A predictor
    MeMV  BMV; //B predictor
    MeMV  CMV; //C predictor
};

extern const Ipp32s MeVc1FastRegrTableSize;
extern Ipp32s MeVc1FastRegrTable[];
extern const Ipp32s MeVc1PreciseRegrTableSize;
extern Ipp32s MeVc1PreciseRegrTable[];
extern const Ipp32s MeAVSFastRegrTableSize;
extern Ipp32s MeAVSFastRegrTable[];

class MePredictCalculator;

typedef enum
{
    ME_RG_RInter,
    ME_RG_DInter,
    ME_RG_JInter,
    ME_RG_RMV,
    ME_RG_RIntra,
    ME_RG_DIntra
}MeRegrFun;

typedef enum
{
    ME_RG_EMPTY_SET,
    ME_RG_VC1_FAST_SET,
    ME_RG_VC1_PRECISE_SET,
    ME_RG_AVS_FAST_SET
}MeRegrFunSet;

class RegrFun{ //Piecewise-Linear Fitting, x from 0
    public:
        RegrFun(Ipp32s dx, Ipp32s N);

        static void SetQP(Ipp32s qp){m_QP = qp;}; // TODO: Check qp in all functions
        void LoadPresets(MeRegrFunSet set, MeRegrFun id);
        void ProcessFeedback(Ipp32s *x, Ipp32s *y, Ipp32s N);
        Ipp32s Weight(Ipp32s x);

    //protected:  debug!!!!
        virtual void ComputeRegression(Ipp32s I, Ipp32s *x, Ipp32s *y, Ipp32s N)=0;
        virtual void AfterComputation(){};

        static const int NumOfRegInterval = 128;
        static const int MaxQpValue = 73;
        // TODO: set TargetHistorySize & FrameSize from without
        static const int FrameSize=1350; //number of MB in frame
#ifdef ME_GENERATE_PRESETS
        static const int m_TargetHistorySize = IPP_MAX_32S; //number of MB in history
#else
        static const int m_TargetHistorySize = 10*FrameSize; //number of MB in history
#endif

        Ipp32s m_dx[MaxQpValue];
        Ipp32s m_num; //number of fitting pieces
        Ipp32s m_FirstData; //first and last bin with data, all other are empty
        Ipp32s m_LastData;
        Ipp64f m_ax[MaxQpValue][NumOfRegInterval];
        Ipp64f m_ay[MaxQpValue][NumOfRegInterval];
        Ipp64f m_an[MaxQpValue][NumOfRegInterval];
        Ipp64f m_a[MaxQpValue][NumOfRegInterval];
        Ipp64f m_b[MaxQpValue][NumOfRegInterval];

        static Ipp32s m_QP; //current QP
};

class LowessRegrFun : public RegrFun
{
    public:
        LowessRegrFun(Ipp32s dx=32, Ipp32s N=128):RegrFun(dx, N){};

    protected:
        static const int Bandwidth = 30; //percent
        Ipp64f WeightPoint(Ipp64f x); //used during regression calculation

        virtual void ComputeRegression(Ipp32s I, Ipp32s *x, Ipp32s *y, Ipp32s N);
        virtual void AfterComputation();
};

class MvRegrFun : public LowessRegrFun
{
    public:
        MvRegrFun():LowessRegrFun(4,128){};
        virtual Ipp32s Weight(MeMV mv, MeMV pred);
};


///Base class for motion estimation.
///It contains standard independent functionality, including different motion estimation and mode decision algorithm.
class MeBase
{
public:
    DYNAMIC_CAST_DECL_BASE(MeBase);

    MeBase();
    virtual ~MeBase();

    //virtual bool Init(MeInitParams *par); //allocates memory
    virtual bool Init(MeInitParams *par);
    virtual bool Init(MeInitParams *par, Ipp8u* ptr, Ipp32s &size);
    virtual bool EstimateFrame(MeParams *par); //par and res should be allocated by caller, except MeFrame::MBs
    virtual void Close(); //frees memory, also called from destructor
    static void SetError(const vm_char *msg, bool condition=true); //save error message

    //1 feedback from encoder
    void ProcessFeedback(MeParams *pPar);

protected:
    //top level functions
    //Exist diffrent realizations for VC-1 and H264
    bool EstimateFrameInner(MeParams *par);
    void EstimateMB16x16(); //here both P and B frames are processing, results are written to m_ResMB
    void EstimateMB8x8(); // call EstimateMB16x16 inside
    virtual bool EstimateSkip()=0;  //return true if early exit allowed, set m_cur->SkipCost[m_cur.BlkIdx], estimate 16x16, 8x8
    virtual void SetInterpPixelType() = 0;

    //possible codec specific
    bool EarlyExitForSkip(); //return true if skip is good enough and further search may be skiped
    void EstimateInter(); //set m_cur->InterCost[RefDir][RefIdx][m_cur.BlkIdx]
    virtual void Estimate16x16Bidir(); //bidir costs is calculated here and Inter mode is choosen
    virtual bool Estimate16x16Direct(); //return true if early exit allowed
    void EstimateIntra();
    void UpdateVlcTableStatistic();
    void MakeVlcTableDecision();
    virtual MeCostRD MakeTransformDecisionInter(MeMbPart mt, MePixelType pix, MeMV mv);
    void MakeMBModeDecision16x16(); //chooses 16x16 mode, saves results to m_ResMB
    void MakeMBModeDecision16x16Org();
    void MakeMBModeDecision16x16ByFB();
    virtual void ModeDecision16x16ByFBFastFrw();
    void ModeDecision16x16ByFBFastBidir();
    virtual void ModeDecision16x16ByFBFullFrw();
    void ModeDecision16x16ByFBFullBidir();
    void MakeSkipModeDecision16x16BidirByFB();
    void SetModeDecision16x16BidirByFB(MeMV &mvF, MeMV &mvB);
    void MakeBlockModeDecision(); //chooses 8x8 mode, save results to m_cur
    void MakeMBModeDecision8x8(); //chooses between 16x16 and 8x8 mode
    void MakeMBModeDecision8x8Org();
    void MakeMBModeDecision8x8ByFB();

    //middle level
    bool IsSkipEnabled();
    void Interpolate(MeMbPart mt, MeInterpolation interp, MeMV mv, MeAdr* src, MeAdr *dst);

    //low level function
    void EstimatePointInter(MeMbPart mt, MePixelType pix, MeMV mv);
    Ipp32s EstimatePointAverage(MeMbPart mt, MePixelType pix, MeDecisionMetrics CostMetric, Ipp32s RefDirection0, Ipp32s RefIndex0, MeMV mv0, Ipp32s RefDirection1, Ipp32s RefIndex1, MeMV mv1);
    Ipp32s EstimatePoint(MeMbPart mt, MePixelType pix, MeDecisionMetrics CostMetric, MeMV mv);
    Ipp32s GetCost(MeDecisionMetrics CostMetric, MeMbPart mt, MeAdr *src, MeAdr* ref);
    void GetCost(MeMbPart mt, MeAdr* src, MeAdr* ref, Ipp32s numX, Ipp16u *sadArr);
    Ipp32s GetCostHDMR(MeDecisionMetrics CostMetric, MeMbPart mt, MeAdr* src, MeAdr* ref);
    virtual MeCostRD GetCostRD(MeDecisionMetrics CostMetric, MeMbPart mt, MeTransformType transf, MeAdr* src, MeAdr* ref);
    virtual void AddHeaderCost(MeCostRD &/*cost*/, MeTransformType * /*tansf*/, MeMbType /*MbType*/){};
    void DownsampleOne(Ipp32s level, Ipp32s x0, Ipp32s y0, Ipp32s w, Ipp32s h, MeAdr *adr);
    void DownsampleFrames();

    //small  auxiliary functions
    void SetReferenceFrame(Ipp32s RefDirection, Ipp32s RefIndex);
    Ipp32s GetPlaneLevel(MePixelType pix);
    void MakeSrcAdr(MeMbPart mt, MePixelType pix, MeAdr* adr);
    void MakeRefAdrFromMV(MeMbPart mt, MePixelType pix, MeMV mv, MeAdr* adr);
    void MakeBlockAdr(Ipp32s level, MeMbPart mt, Ipp32s BlkIdx, MeAdr* adr);
    virtual void SetMB16x16B(MeMbType mbt, MeMV mvF, MeMV mvB, Ipp32s cost);
    //MeBlockType ConvertType(MeMbType t);
    void SetMB8x8(MeMbType mbt, MeMV mv[4], Ipp32s cost[4]);
    virtual MeMV GetChromaMV (MeMV mv){return mv;};
    virtual Ipp32s GetMvSize(Ipp32s /*dx*/,Ipp32s /*dy*/){return 0;};

    //algorithms
    void EstimateMbInterFullSearch();
    void EstimateMbInterFullSearchDwn();
    void EstimateMbInterOneLevel(bool UpperLevel, Ipp32s level, MeMbPart mt, Ipp32s x0, Ipp32s x1, Ipp32s y0, Ipp32s y1);
    void EstimateMbInterFast();
    void FullSearch(MeMbPart mt, MePixelType pix, MeMV org, Ipp32s RangeX, Ipp32s RangeY);
    void DiamondSearch(MeMbPart mt, MePixelType pix, MeDiamondType dm);
    bool RefineSearch(MeMbPart mt, MePixelType pix);

    MeCurrentMB m_cur;
    MePredictCalculator* m_PredCalc;

    bool CheckParams();
    Ipp32s WeightMV(MeMV mv, MeMV predictor);


    //parameters
    MeParams*         m_par; //ptr to current frame parameters
    MeInitParams      m_InitPar; //initialization parameters
    Ipp32s            m_SkippedThreshold;//pixel abs. diff. threshhold for the skipped macroblocks (unifirm metric)
    Ipp32s m_AllocatedSize; //number of allocated byte, valid after first call of Init
    void *m_OwnMem;
    Ipp32s  CostOnInterpolation[4];

protected:
    Ipp32s QpToIndex(Ipp32s QP, bool UniformQuant);
    void LoadRegressionPreset();

    Ipp64f m_lambda;
    MeRegrFunSet  m_CurRegSet;
    LowessRegrFun m_InterRegFunD;
    LowessRegrFun m_InterRegFunR;
    LowessRegrFun m_InterRegFunJ;
    LowessRegrFun m_IntraRegFunD;
    LowessRegrFun m_IntraRegFunR;
    MvRegrFun m_MvRegrFunR;
};

}//namespace

#endif
