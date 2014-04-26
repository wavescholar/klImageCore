/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//          Main ME structures
//
*/

#ifndef _UMC_ME_STRUCTURES_H_
#define _UMC_ME_STRUCTURES_H_

#include "ippcore.h"
#include "ippi.h"

#include "umc_config.h"
#include "umc_structures.h"
#include "umc_dynamic_cast.h"
#include "umc_memory_allocator.h"

#define MAX_REF         32


namespace UMC
{
    //*******************************************************************************************************

        enum{//reference frame direction
            frw=0,
            bkw=1,
            bidir=2
        };
        enum{//YUV plaines index
            Y=0,
            U=1,
            V=2
        };

    ///Pixel types. It is used to specify subpixel search level, and also used inside ME to choose downsampled level.
    ///It does not define interpolation type. See MeInterpolation.
    typedef enum
    {
        ME_IntegerPixel = 0x1, ///<integer pixel
        ME_HalfPixel    = 0x2, ///<half pixel
        ME_QuarterPixel = 0x4, ///<quarter pixel
        ME_DoublePixel = 0x8, ///< downsample 2 times plane
        ME_QuadPixel = 0x10, ///< 4 times
        ME_OctalPixel = 0x20 ///< 8 times
    } MePixelType;

    ///MB partitioning. Numbering  is important, so don't change it arbitrary!
    ///Unusual types such as 2x2 are used during downsampling search.
    typedef enum
    {
        ME_Mb2x2   = 0x00000001, ///<2x2
        ME_Mb4x4   = 0x00000002, ///<4x4
        ME_Mb8x8   = 0x00000004, ///<8x8
        ME_Mb16x16 = 0x000000008, ///<16x16
        ME_Mb16x8  = 0x000000100, ///<16x8
        ME_Mb8x16  = 0x000000200 ///<8x16
    } MeMbPart;

    ///Interpolation type.
    ///See also MePixelType
    typedef enum
    {
        ME_VC1_Bilinear,
        ME_VC1_Bicubic,
        ME_H264_Luma,
        ME_H264_Chroma,
        ME_AVS_Luma
    } MeInterpolation;

    ///MV prediction type.
    ///See MePredictCalculator.
    typedef enum
    {
        ME_VC1,
        ME_VC1Hybrid,
        ME_VC1Field1,
        ME_VC1Field2,
        ME_VC1Field2Hybrid,
        ME_MPEG2,
        ME_H264,
        ME_AVS
    } MePredictionType;

    ///Search direction.
    ///Used to choose between P and B frames.
    typedef enum
    {
        ME_ForwardSearch = 0,
        ME_BidirSearch
    }MeSearchDirection;

    ///MB type. 
    ///This is standard independent types.
    typedef enum
    {
        ME_MbIntra,
        ME_MbFrw,
        ME_MbBkw,
        ME_MbBidir,
        ME_MbFrwSkipped,
        ME_MbBkwSkipped,
        ME_MbBidirSkipped,
        ME_MbDirectSkipped,
        ME_MbDirect,
        ME_MbMixed
    } MeMbType;

#if 0
    ///Block type. 
    ///This is standard independent types.
    typedef enum
    {
        ME_BlockSkipped,
        ME_BlockFrw,
        ME_BlockBkw,
        ME_BlockBidir,
        ME_BlockIntra,
        ME_BlockOutOfBound
    } MeBlockType;
#endif

    typedef enum
    {
        ME_FrameMc,
        ME_FieldMc
    } MeMcType;

    typedef enum
    {
        ME_FrmIntra,
        ME_FrmFrw,
        ME_FrmBidir
    } MeFrameType;

    ///Different motion estimation paths. See MeBase::DiamondSearch
    ///for detailed path configuration.
    typedef enum
    {
        ME_SmallDiamond, ///<small +-1 point diamond
        ME_MediumDiamond, ///<medium +-2 point diamond
        ME_BigDiamond,///<big +-3 point diamond
        ME_SmallHexagon, ///<small +-8 points 2 level hexagon
        ME_MediumHexagon ///<medium +-16 points 4 level hexagon
    } MeDiamondType;

    ///Different cost metrics used for motion estimation and mode decision.
    ///Not all metrics may be used for motion estimation.
    typedef enum
    {
        ME_Sad, ///< SAD
        ME_Sadt, ///< Hadamard
        ME_SSD, ///< sum of square  differences
        ME_Maxad,
        ME_Hadamard,
        ME_SadtSrc, ///<calculate sum of coefficients of Hadamard transformed src, used for Intra/Inter mode decision
        ME_SadtSrcNoDC, ///<the same, but not taking DC into account
        ME_VarMean,
        ME_InterSSD,
        ME_InterRate,
        ME_IntraSSD,
        ME_IntraRate,
        ME_InterRD,
        ME_IntraRD
    }MeDecisionMetrics;

    typedef enum
    {
        ME_Block0 = 0,
        ME_Block1,
        ME_Block2,
        ME_Block3,
        ME_Macroblock = 10
    }MePredictionIndex;

    ///Different transform types.
    ///Used for standard abstraction.
    typedef enum
    {
        ME_Tranform8x8=0, ///,
        ME_Tranform8x4=1, ///,
        ME_Tranform4x8=2, ///,
        ME_Tranform4x4=3 ///,
    } MeTransformType;

    //*******************************************************************************************************

    struct MePicRange
    {
        IppiPoint top_left;
        IppiPoint bottom_right;
    };

    struct MeVC1fieldScaleInfo
    {
        Ipp16s ME_ScaleOpp;
        Ipp16s ME_ScaleSame1;
        Ipp16s ME_ScaleSame2;
        Ipp16s ME_ScaleZoneX;
        Ipp16s ME_ScaleZoneY;
        Ipp16s ME_ZoneOffsetX;
        Ipp16s ME_ZoneOffsetY;
        Ipp16s ME_RangeX;
        Ipp16s ME_RangeY;
        bool   ME_Bottom;

    };

    struct MeACTablesSet
    {
       const Ipp8u * pTableDL;
       const Ipp8u * pTableDR;
       const Ipp8u * pTableInd;
       const Ipp8u * pTableDLLast;
       const Ipp8u * pTableDRLast;
       const Ipp8u * pTableIndLast;
       const Ipp32u* pEncTable;
    }; //it is ME version of UMC_VC1_ENCODER::sACTablesSet


    //*******************************************************************************************************
    ///Motion vector. 
    ///Always in quarter pixel units.
    class MeMV
    {
    public:
        /** Default constructor */
        MeMV(){};
        ///Create MV with identical x and y component, mostly used in mv=0 sentences 
        ///\param a0 component value
        MeMV(int a0){x = (Ipp16s)a0; y=(Ipp16s)a0;};
        ///Create MV with different x and y component
        ///\param a0 x value \param a1 y value
        MeMV(int a0, int a1){x = (Ipp16s)a0; y=(Ipp16s)a1;};
        Ipp16s x;
        Ipp16s y;

        /**Set invalid value to MV.*/
        void SetInvalid(){x=0x7fff;y=0x7fff;};
        /**Check if MV is invalid.*/
        bool IsInvalid(){return x==0x7fff && y==0x7fff;}; 
        /**Add two MVs.*/
        inline MeMV operator + (const MeMV& a)
        {return MeMV(a.x + x, a.y + y);};
        /**Compare two MVs.*/
        inline bool operator == (const MeMV& a)
        {return a.x==x && a.y==y;};
    };


    ///Class used to represent addresses and steps of point in all three planes Y, U and V.
    ///Also used in downsampling search to reference to different levels.
    class MeAdr
    {
        public:
        /**Constructor */    
        MeAdr(){clear();};
        /**Clear planes pointers.*/
        inline void clear(){chroma=false; channel=0; ptr[0]=ptr[1]=ptr[2]=NULL;};
        /**Set one channel.\param ch channel Y,U or V, \param pt pointer to plane \param st plane step*/
        inline void set(Ipp32s ch, Ipp8u *pt, Ipp32s st){ptr[ch]=pt; step[ch]=st;};
        /**Move pointers in both x and y direction for given channel\param ch channel index to move \param dx x offset \param dy y offset*/
        inline void move(Ipp32s ch, Ipp32s dx, Ipp32s dy){ptr[ch]+=dy*step[ch]+dx;};
        /**Move pointers in both x and y direction for luma if chroma==false or for all channels. Chroma offset is two times smaller than for luma.
             \param dx x offset \param dy y offset*/
        inline void move(Ipp32s dx, Ipp32s dy){for(int i=0; i<(chroma?3:1);i++){move(i,dx,dy);if(i==0){dx/=2;dy/=2;}}};
        /**Return pointer in given channel offseted by x and y pixel \param ch channel index \param dx x offset \param dy y offset*/
        inline Ipp8u* at(Ipp32s ch, Ipp32s dx, Ipp32s dy){return ptr[ch]+dy*step[ch]+dx;};
        /**Operator is used to preserv chroma flag and selected channel during object copying*/        
        void operator=(const MeAdr &a){for(int i=0; i<3; i++){ptr[i]=a.ptr[i];step[i]=a.step[i];}};
            
        bool chroma; ///< true - chroma planes are valid or chroma address should be loaded
        Ipp32s channel; ///< index of active channel, it is used in some cost calculation
        Ipp8u* ptr[3];  ///< plane pointers
        Ipp32s step[3]; ///< plane steps
    };


    ///Class is used 
    ///to represent RD cost.
    class MeCostRD
    {
        public:
            MeCostRD(){};
            MeCostRD(Ipp32s x){R=D=J=x;NumOfCoeff=BlkPat=SbPat=0;};
            Ipp32s R;
            Ipp32s D;
            Ipp32s J;
            Ipp32s NumOfCoeff;
            Ipp32s BlkPat; //block pattern, bit 0 is 5-th block chroma, bit 5 is 0 block luma
            Ipp32s SbPat; //sub block pattern, for varible transform. For 8x8 transform only bit 0 is set, for 8x4 and 4x8 bit 1 is for 0 subblock, bit 0 is for 1-st block
            
            inline void operator +=(const MeCostRD& a) {R+=a.R;D+=a.D;NumOfCoeff+=a.NumOfCoeff;BlkPat|=a.BlkPat;SbPat|=a.SbPat;J=0;};
    };

    ///Statistics for one MB. It is used in feedback implementation. All fields are filled in by encoder, except dist.
    ///It is currently filled in by ME, though it is violation of architecture.
    class MeMbStat{
    public:
        Ipp16u  whole; ///<whole MB size in bit, including MV, coefficients and any additional info. 
        Ipp16u* MVF; ///<size of each MVs in bit, number of allocated element is MeFrame::NumOfMVs
        Ipp16u* MVB; ///<size of each MVs in bit, number of allocated element is MeFrame::NumOfMVs
        Ipp16u* coeff; /**< \brief size of coefficients in bit, for each block. Number of element is MeFrame::NumOfMVs.
                            Block is motion compensated entity, not the transformation block. So if there is only one MV for MB,
                            then only one element is set here, even if 4x4 transformation was used. It is intended for simplifying
                            feedback implementation.*/
                            // TODO: update usage model!
        Ipp16u  qp; ///< QP used for encoding of this MB
        Ipp8u  MbType; ///<coded MB type MbIntra, MbFrw, MbBkw, MbBidir..., may be different from MeFrame::MBs::MbType
        Ipp32u dist; ///<whole 16x16 MB distortion, MSD?
    };

    ///Class represents motion estimation and mode decision results. Also used to store some intermediate  data
    ///from motion estimation. See MeFrame::MBs.
    class MeMB {
    public:
        void Reset();
        void Reset( Ipp32s numOfMVs);
        void CnvertCoeffToRoundControl(Ipp16s *transf, Ipp16s *trellis, Ipp16s *rc, Ipp32s QP);///< function converts trellis quantized coefficients to round control value for each coefficient
        
        MeMV      *MV[2];  ///<best found MV, 0 - forward, 1- backward, order: fwd1, fwd2, fwd3, fwd4, number of allocated element is MeFrame::NumOfMVs
        Ipp32s    *Refindex[2];///<indexes for the reference frames, 0-forward, 1-backward
        Ipp32s    *MbCosts; ///<costs of best MVs. number of allocated elements is equal to number MeFrame::NumOfMVs+1
                            // TODO: chroma?
        MeMV      *MVPred[2]; ///<Predicted MV for current MB. 0 - forward, 1- backward; number of allocated elements is equal to number of MVs;
        Ipp8u     MbPart; ///<MB partitioning 16x16, 16x8, 8x16, 8x8, 4x4 -> MeMbPart
        Ipp8u     BlockPart; ///<NOT USED, block partitioning, 4x2 bits field, 00 - 8x8, 01 - 8x4, 10 - 4x8, 11 - 4x4
        Ipp8u     MbType; ///<MbIntra,MbFrw,MbBkw,MbBidir,MbFrw_Skipped,MbBkw_Skipped,MbBidir_Skipped,MbDirect  -> MeMbType
        Ipp8u     McType; ///<NOT USED, Motion compensation type, see MeMcType 
        Ipp8u     BlockType[4];///<Block type for each of 8x8 blocks. MeMbType is used to simplify 16x16 vs 8x8 comparision.
        Ipp32s     BlockTrans; ///<NOT USED, block transforming, 6x2 bits field, 00 - 8x8, 01 - 8x4, 10 - 4x8, 11 - 4x4, lowest bits 1:0 for block 0, 2:1 for block 2 and so on 11:10 for block 5
        //Ipp8u*    Difference; //NULL - no differences was calculated
        
        // AVS specific
        Ipp8u predType[4];     // (Ipp8u []) prediction types of each block: only MbIntra,MbFrw,MbBkw,MbBidir

        //VC1 specific, used for FB only
        Ipp32s  NumOfNZ; //it is number of non zero blocks after transforming and quantisation
        Ipp32s  PureSAD; //pure inter SAD, there is no MV weighting in it
        MeCostRD InterCostRD; //value for inter rate and distortion prediction
        MeCostRD IntraCostRD; //value for intra rate and distorton prediction
        MeCostRD DirectCostRD; //value for direct inter rate and distortion prediction
        MeCostRD DirectSkippedCostRD; //value for direct skipped inter rate and distortion prediction
        MeCostRD SkippedCostRD; //value for skipped inter rate and distortion prediction
        Ipp32s  DcCoeff[6]; //DC coefficients
        Ipp16s Coeff[6][64]; //quantized coefficients
    };

    ///Class represents one frame. It may be either source or reference frame. 
    ///Used to simplify interface by grouping all frame attributes in one class and to avoid unnecessary downsampling.
    class MeFrame
    {
    public:
        bool    processed; ///<Frame was already processed by ME. It is set by ME, reset by encoder
//        Ipp32s      index; //set by encoder
        MeFrameType type; ///< Frame type, set by encoder

        Ipp32s      WidthMB; ///< Frame width in MB. Set by encoder
        Ipp32s      HeightMB;///< Frame height in MB. Set by encoder
        Ipp32s      padding;  /**< \brief number of pixels that was added on each side of the frame, should be less or equal 
                                        to MeInitParams.refPadding and divisible by 16. Zero is also possible.*/
        MeAdr       plane[5]; /**< \brief Image plains. 5 is number of downsampled levels, zero level is allocated by encoder, this is original picture, all other by ME. 
                                Not all levels may be allocated. Pointers on all levels point to the left top pixel of picture without padding*/
        // TODO: add initializaton
        MeAdr       SrcPlane; //< This original, uncompressed image plane. May oy may not be the same as MeFrame::plane[0]. It is used for distortion calculation in feedback.
        MeAdr       RecPlane; //< This reconstructed image plane. May oy may not be the same as MeFrame::plane[0].  It is used for distortion calculation in feedback.

        Ipp32s      NumOfMVs; ///<num of MVs in MBs structure, it set by ME, should not be changed by encoder
        MeMB*       MBs; ///<Estimation results for current frame. Number of element is equal to number of MB in frame. Allocated by MeBase
        MeMV*       MVDirect;///< Direct motion vectors, calculated and set by encoder. Currently VC1 specific. allocated by MeBase
        Ipp32s*     RefIndx;///< Reference indexes for direct MVs. Currently VC1 specific. allocated by MeBase
        // TODO: add initializaton
        bool        StatValid; ///< true - encoder filled in stat array for this frame. It is set by encoder and reset by ME after processing
        MeMbStat*   stat;  ///< Statistics for current frame. Filled in by encoder.  Number of element is equal to number of MB in frame. allocated by MeBase
        // AVS specific
        Ipp32s m_blockDist[2][4];                      // (Ipp32s [][]) block distances, depending on ref index
        Ipp32s m_distIdx[2][4];                        // (Ipp32s [][]) distance indecies of reference pictures
        Ipp32s picture_distance;
    };

    ///Class used to initialize ME. Amount of allocated memory depend on provided values. 
    ///See MeBase::Init for detailed description of ME initialization.
    class MeInitParams
    {
    public:
        MeInitParams();
        Ipp32s            MaxNumOfFrame; ///< Maximum number of frame to allocate.
        MeFrame*          pFrames; ///< Allocated frames. Returned by ME.
        
        Ipp32s            WidthMB; ///<Frame width in MB, used only for memory allocation.
        Ipp32s            HeightMB;///<Frame height in MB.
        Ipp32s            refPadding;///<Reference frame padding. Unlike MeFrame::padding may be any value.
        MeSearchDirection SearchDirection;///<forward, bidir
        Ipp32s            MbPart;///<16x16,16x8,8x16,8x8,4x4
        bool UseStatistics;///<true - allocate memory for statistics, MeFrame::stat
        bool UseDownSampling;///<true - allocate memory for downsampling MeFrame::plane[x]
    };

    ///Class used to specify current frame estimation. All parameters should not
    ///excide value passed to MeBase::Init.
    class MeParams
    {
    public:
        MeParams();
        void SetSearchSpeed(Ipp32s Speed);

        Ipp32s   FirstMB;  ///< Address of first MB for estimation. Used for multy threading slice processing.
        Ipp32s   LastMB;  ///< Address of last MB for estimation. Used for multy threading slice processing.

        MeFrame* pSrc; ///< Pointer to source frame. Estimation results will be returned in pSrc->MBs

        Ipp32s              FRefFramesNum; ///< Number of forward reference frames
        Ipp32s              BRefFramesNum; ///< Number of backward reference frames
        MeFrame* pRefF[MAX_REF]; ///< array of forward reference frames, array allocated by ME, frames by encoder
        MeFrame* pRefB[MAX_REF]; ///< the same for backward

        IppiPoint           SearchRange; ///<This is MV range in pixel. Resulting MV will be from -4*SearchRange.z to 4*SearchRange.z-1
        MeSearchDirection   SearchDirection;///< May be forward or bidir
        MePredictionType    PredictionType;///< One of possible prediction
        MeInterpolation     Interpolation; ///< Interpolation type for luma channel
        MeInterpolation     ChromaInterpolation; ///< Interpolation type for chroma
        Ipp32s              MbPart;///< List of partitions to check. Only partition from this list will be checked.
        MePixelType         PixelType; ///<Subpixel type to search. 
        MePicRange          PicRange; /**<\brief It is area where ME is allowed relative to left top pixel of the frame. Negative values are possible.
                                All pixels of any block or MB will remain within these borders for any resulting MV.
                                Top and left ranges are inclusive. So if top_left.x=-8, MV.x=-4*8 is smallest possible x value for MB with address 0.
                                Right and bottom are not inclusive. So if bottom_right.x=720, MV.x=4*720-4*16 is biggest possible MV for 16x16 MB 0. */
        MeDecisionMetrics   CostMetric; ///< inter MB metric
        MeDecisionMetrics   SkippedMetrics;///<skip MB metric
        bool                bSecondField;///< true - the current coded picture is the second field
        Ipp32s              Quant; ///< quantizer for current picture
        bool                UniformQuant; ///< true - uniform quantization will be used. VC1 specific 
        bool                ProcessSkipped; ///< ture - estimate skipped MB
        bool                ProcessDirect; ///< true - estimate direct MB
        bool                FieldMCEnable;  ///< true - field MB mode, NOT USED
        bool                UseVarSizeTransform; ///< true - choose variable size transforming for Inter blocks. VC1 specific 

        bool UseFastInterSearch; ///< true - use fastest possible ME algorithm. Feature leads to significant speed up and to significant quality degradation.
        bool UseDownSampledImageForSearch; /**< \brief true - enable search on downsampled pictures. Feature should be enabled in MeBase::Init. 
                           It leads to significant speed up without noticeable quality degradation. It is recommended to keep it on. Except in memory tight environment.*/
        //bool UseFastIntraInterDecision;
        bool UseFeedback; /**<\brief true - Use feedback from encoder. This feature significantly (if UseFastFeedback==false) improves quality with moderate speed lost.
                                           Implemented only for 16x16 forward mode now.*/
        bool UpdateFeedback; /**<\brief false - use feedback with default presets, no actual feedback from encoder is needed. true - update presets 
                                    by feedback from encoder. Feature leads to quality improvement on some bizarre streams with some speed degradation. */
        bool UseFastFeedback; ///<false - use thorough, transform based, distortion/rate prediction. true - use less acuratre but more fast prediction
        bool UseChromaForME; ///<true - use chrome component during ME. Feature leads to small quality improvement, with some speed degradation. NOT USED.
        bool UseChromaForMD; ///<true - use chrome component during mode decision. Turn it on to avoid chromatic artifacts.
        bool ChangeInterpPixelType; ///<true - change interpolation and in the end of the search
        bool SelectVlcTables;
        bool UseTrellisQuantization;
        bool UseTrellisQuantizationChroma;

        //VC1 specific
        bool FastChroma; ///< VC1 specific
        MeVC1fieldScaleInfo ScaleInfo[2];///<the structure for field vector predictors calculation

        //VC1 specific, used for RD mode decision
        Ipp32s      MVRangeIndex;
        const Ipp8u*    MVSizeOffset;
        const Ipp8u*    MVLengthLong;

        const Ipp32u*  DcTableLuma;
        const Ipp32u*  DcTableChroma;

        const Ipp8u* ScanTable[4]; //8x8 8x4 4x8 4x4 scan tables 

        Ipp32s CbpcyTableIndex;
        Ipp32s MvTableIndex;
        Ipp32s AcTableIndex;

        //output parameters (recommendation for encoder)
        MeInterpolation     OutInterpolation;
        MePixelType         OutPixelType;
        Ipp32s OutCbpcyTableIndex;
        Ipp32s OutMvTableIndex;
        Ipp32s OutAcTableIndex;

//    protected:
        Ipp32s SearchSpeed; ///< search speed, see SetSearchSpeed
    };


    const int ME_NUM_OF_REF_FRAMES = 2;
    const int ME_NUM_OF_BLOCKS = 4;
    const int ME_BIG_COST = 0x1234567; //19.088.743 it is about 100 times smaller than 2147483647, this is reserve to prevent overflow during calculation


    class MeSearchPoint
    {
        public:
            Ipp32s cost; 
            Ipp16s x;
            Ipp16s y;
    };

    static const int NumOfLevelInTrellis=2;
    class MeTrellisNode {
    public:
        void Reset(){active=true; last = false; level=run=0; cost=0; prev=NULL;};
        
        bool active;
        bool last;
        Ipp32s level;
        Ipp32s run;
        Ipp32s cost;
        MeTrellisNode *prev;
    };
  

    class MeCurrentMB
    {
    public:
        void Reset();
        void ClearMvScoreBoardArr();
        Ipp32s CheckScoreBoardMv(MePixelType pix,MeMV mv);
        void SetScoreBoardFlag();
        void ClearScoreBoardFlag();

        Ipp32s x; //MB location relative to left top corner of the frame
        Ipp32s y;
        Ipp32s adr; //MB address

        //skip
        Ipp32s SkipCost[ME_NUM_OF_BLOCKS];
        MeMbType SkipType[ME_NUM_OF_BLOCKS]; //For VC1 - ME_MbFrwSkipped, ME_MbBkwSkipped or ME_MbBidirSkipped
        Ipp32s SkipIdx[2][ME_NUM_OF_BLOCKS]; //reference frames indexes for skip, second index - frw or bkw

        //direct
        Ipp32s DirectCost;
        MeMbType DirectType; //For VC1 - ME_MbDirectSkipped, ME_MbDirect
        Ipp32s DirectIdx[2]; //reference frames indexes for direct

        //inter
        //these variable are used not only for inter type search but also as temporal place to store mode decision results
        //for blocks. This behavior may be changed later.
        Ipp32s BestCost; //intermediate cost for current MB, block or subblock
        Ipp32s InterCost[ME_NUM_OF_BLOCKS]; //this is best found cost for frw or bkw or bidir Inter MB or block or subblock
        MeMbType InterType[ME_NUM_OF_BLOCKS]; //this is best found type of current MB, block or subblock
        Ipp32s BestIdx[2][ME_NUM_OF_BLOCKS]; //frw and bkw frames indexes of best cost
        MeMV BestMV[2][ME_NUM_OF_REF_FRAMES][ME_NUM_OF_BLOCKS]; //all best found MVs
        MeTransformType InterTransf[6]; //transformation type
        bool HybridPredictor; //current MV uses hybrid predictor. It is used during RD cost calculation. VC1 specific

        //intra
        Ipp32s IntraCost[ME_NUM_OF_BLOCKS]; //Intra cost
        //SomeType IntraType[ME_NUM_OF_BLOCKS]; //intra prediction type for H264

        //place to save calculated residuals
        MeAdr buf[3]; //temporary buffers for iterplolation, index is direction 0 - frw, 1 - bkw, 2 - bidir, all three planes are allocated for each direction

        MeMV PredMV[2][ME_NUM_OF_REF_FRAMES][ME_NUM_OF_BLOCKS]; //MV prediction

        Ipp32s RefIdx; //reference frame index
        Ipp32s RefDir; //reference frame direction frw or bkw

        Ipp32s BlkIdx; //current block idx
        MeMbPart MbPart; //current MB partitioning

        Ipp32s DwnCost[5];
        Ipp32s DwnNumOfPoints[5];
        MeSearchPoint DwnPoints[5][16*1024]; // TODO: allocate in init

        //slice level statistics
        Ipp32s CbpcyRate[4]; //number of bit to encode CBPCY element using each of four possible tables
        Ipp32s MvRate[4]; //number of bit to encode MVs using each of four possible tables
        Ipp32s AcRate[4]; //number of bit to encode AC coefficients using each of three possible table sets

        //VC1 specific
        //VLC table indexes
        Ipp32s qp8;
        Ipp32s CbpcyTableIndex;
        Ipp32s MvTableIndex;
        Ipp32s AcTableIndex;

        //trellis quant related
        bool VlcTableSearchInProgress; //true - VLC table search in progress, it is used to turn off trellis quantization, not only to speed up
                  //search but also to avoid mismatch between VLC tables that were used during trellis quantization and coding. 
        Ipp16s TrellisCoefficients[5][6][64]; // 5= (4 different transfroms for inter plus 8x8 intra), 6 blocks, 64 coeff in each
        MeTrellisNode trellis[65][NumOfLevelInTrellis+64];

        Ipp8u mvScoreBoardArr[128][64];
        Ipp32u mvScoreBoardFlag;
    };
}
#endif

