/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Coded block pattern tables
//
*/

#ifndef __UMC_VC1_COMMON_DEFS_H__
#define __UMC_VC1_COMMON_DEFS_H__

#include <stdio.h>
#include "umc_vc1_common_macros_defs.h"
#include "umc_structures.h"

#include "ippcore.h"
#include "ippi.h"
#include "ippvc.h"

//#define _OWN_FUNCTION

#define VC1_ENC_RECODING_MAX_NUM 3
#define START_CODE_NUMBER 600

//frame header size in case of SM profiles
#define  VC1FHSIZE 8
//start code size in case of Advance profile
#define  VC1SCSIZE 4

typedef enum
{
    VC1_ERR_NOSIGNAL = 0,
    VC1_ERR_SIGNAL   = 1

} VC1_ERROR;

#define VC1_SIGNAL     0
#define VC1_DEC_ERR(value) (value & VC1_ERR_SIGNAL)

typedef enum
{
    VC1_OK   = 0,
    VC1_FAIL = UMC::UMC_ERR_FAILED,
    VC1_SKIP_FRAME = 2,
    VC1_NOT_ENOUGH_DATA = UMC::UMC_ERR_NOT_ENOUGH_DATA,
    VC1_WRN_INVALID_STREAM = UMC::UMC_ERR_UNSUPPORTED
}VC1Status;

#define VC1_TO_UMC_CHECK_STS(sts) if (sts != VC1_OK) return sts;


// start code (for vc1 format)
//see C24.008-VC1-Spec-CD2r1.pdf standard Annex E,E5, p. 431
typedef enum
{
    VC1_EndOfSequence              = 0x0A,
    VC1_Slice                      = 0x0B,
    VC1_Field                      = 0x0C,
    VC1_FrameHeader                = 0x0D,
    VC1_EntryPointHeader           = 0x0E,
    VC1_SequenceHeader             = 0x0F,
    VC1_SliceLevelUserData         = 0x1B,
    VC1_FieldLevelUserData         = 0x1C,
    VC1_FrameLevelUserData         = 0x1D,
    VC1_EntryPointLevelUserData    = 0x1E,
    VC1_SequenceLevelUserData      = 0x1F
} VC1StartCode;

//input file format
enum
{
    RCVFileFormat               = 0,
    VC1FileFormat               = 1,
    VC1_UnknownFileFormat       = 10
};


//frame coding mode
enum
{
    VC1_Progressive           = 0,
    VC1_FrameInterlace        = 1,
    VC1_FieldInterlace        = 2
};

enum
{
    VC1_I_FRAME = 0,
    VC1_P_FRAME = 1,
    VC1_B_FRAME = 2,
    VC1_BI_FRAME= 3,
    VC1_SKIPPED_FRAME = 4
};
#define VC1_IS_REFERENCE(value) ((value < VC1_B_FRAME)||(value == VC1_SKIPPED_FRAME))
#define VC1_IS_NOT_PRED(value) ((value == VC1_I_FRAME)||(value == VC1_BI_FRAME))
#define VC1_IS_PRED(value) ((value == VC1_P_FRAME)||(value == VC1_B_FRAME))


enum
{
    VC1_COND_OVER_FLAG_NONE = 0, //no 8x8 block boundaries are smoothed,
                             //see standart, p163
    VC1_COND_OVER_FLAG_ALL = 2,  //all 8x8 block boundaries are smoothed,
                             //see standart, p163
    VC1_COND_OVER_FLAG_SOME = 3  //some 8x8 block boundaries are smoothed,
                             //see standart, p163
};
#define VC1_MAX_BITPANE_CHUNCKS 7
#define VC1_NUM_OF_BLOCKS 6
#define VC1_NUM_OF_LUMA   4
#define VC1_PIXEL_IN_LUMA 16
#define VC1_PIXEL_IN_CHROMA 8
#define VC1_PIXEL_IN_BLOCK  8
#define VC1_PIXEL_IN_MB VC1_PIXEL_IN_BLOCK*VC1_PIXEL_IN_BLOCK*VC1_NUM_OF_BLOCKS

enum
{
    VC1_BLK_INTER8X8   = 0x1,
    VC1_BLK_INTER8X4   = 0x2,
    VC1_BLK_INTER4X8   = 0x4,
    VC1_BLK_INTER4X4   = 0x8,
    VC1_BLK_INTER      = 0xf,
    VC1_BLK_INTRA_TOP  = 0x10,
    VC1_BLK_INTRA_LEFT = 0x20,
    VC1_BLK_INTRA      = 0x30
};

#define VC1_IS_BLKINTRA(value) (value & 0x30)
enum
{
    VC1_MB_INTRA             = 0x0,
    VC1_MB_1MV_INTER         = 0x1,
    VC1_MB_2MV_INTER         = 0x2,
    VC1_MB_4MV_INTER         = 0x3,
    VC1_MB_4MV_FIELD_INTER   = 0x4,
    VC1_MB_DIRECT            = 0x0,
    VC1_MB_FORWARD           = 0x8,
    VC1_MB_BACKWARD          = 0x10,
    VC1_MB_INTERP            = 0x18
 };

#define VC1_IS_MVFIELD(value)  (((value&7) == VC1_MB_2MV_INTER)||((value&7) == VC1_MB_4MV_FIELD_INTER ))
#define VC1_GET_MBTYPE(value)  (value&7)
#define VC1_GET_PREDICT(value) (value&56)

#define VC1MBQUANT 2
#define VC1SLICEINPARAL 512
//#define VC1MBALLOCSIZE 1024
#define VC1FRAMEPARALLELPAIR 1
//#define CREATE_ES
//#define VC1_THREAD_STATISTIC

enum
{
    VC1_MVMODE_HPELBI_1MV    = 0,    //0000      1 MV Half-pel bilinear
    VC1_MVMODE_1MV           = 1,    //1         1 MV
    VC1_MVMODE_MIXED_MV      = 2,    //01        Mixed MV
    VC1_MVMODE_HPEL_1MV      = 3,    //001       1 MV Half-pel
    VC1_MVMODE_INTENSCOMP    = 4,    //0001      Intensity Compensation
};

enum
{
    VC1_DQPROFILE_ALL4EDGES  = 0,    //00    All four Edges
    VC1_DQPROFILE_DBLEDGES   = 1,    //01    Double Edges
    VC1_DQPROFILE_SNGLEDGES  = 2,    //10    Single Edges
    VC1_DQPROFILE_ALLMBLKS   = 3     //11    All Macroblocks
};
enum
{
    VC1_ALTPQUANT_LEFT            = 1,
    VC1_ALTPQUANT_TOP             = 2,
    VC1_ALTPQUANT_RIGTHT          = 4,
    VC1_ALTPQUANT_BOTTOM          = 8,
    VC1_ALTPQUANT_LEFT_TOP        = 3,
    VC1_ALTPQUANT_TOP_RIGTHT      = 6,
    VC1_ALTPQUANT_RIGTHT_BOTTOM   = 12,
    VC1_ALTPQUANT_BOTTOM_LEFT     = 9,
    VC1_ALTPQUANT_EDGES           = 15,
    VC1_ALTPQUANT_ALL             = 16,
    VC1_ALTPQUANT_NO              = 0,
    VC1_ALTPQUANT_MB_LEVEL        = 32,
    VC1_ALTPQUANT_ANY_VALUE       = 64
};
enum //profile definitions
{
    VC1_PROFILE_SIMPLE = 0, //disables X8 Intraframe, Loop filter, DQuant, and
                            //Multires while enabling the Fast Transform
    VC1_PROFILE_MAIN   = 1, //The main profile is has all the simple profile
                            //tools plus loop filter, dquant, and multires
    VC1_PROFILE_RESERVED = 2,
    VC1_PROFILE_ADVANCED= 3    //The complex profile has X8 Intraframe can use
                      //the normal IDCT transform or the VC1 Inverse Transform
};

#define VC1_IS_VALID_PROFILE(profile) ((profile == VC1_PROFILE_RESERVED)||(profile > VC1_PROFILE_ADVANCED)?false:true)

enum //bitplane modes definitions
{
    VC1_BITPLANE_RAW_MODE        = 0,//Raw           0000
    VC1_BITPLANE_NORM2_MODE      = 1,//Norm-2        10
    VC1_BITPLANE_DIFF2_MODE      = 2,//Diff-2        001
    VC1_BITPLANE_NORM6_MODE      = 3,//Norm-6        11
    VC1_BITPLANE_DIFF6_MODE      = 4,//Diff-6        0001
    VC1_BITPLANE_ROWSKIP_MODE    = 5,//Rowskip       010
    VC1_BITPLANE_COLSKIP_MODE    = 6 //Colskip       011
};

#define VC1_UNDEF_PQUANT 0
#define VC1_MVINTRA (0X7F7F)

enum //quantizer deadzone definitions
{
    VC1_QUANTIZER_UNIFORM    = 0,
    VC1_QUANTIZER_NONUNIFORM = 1
};

enum //prediction directions definitions
{
    VC1_ESCAPEMODE3_Conservative    = 0,
    VC1_ESCAPEMODE3_Efficient       = 1
};
//for subBlockPattern (numCoef)
enum
{
    VC1_SBP_0               = 0x8,
    VC1_SBP_1               = 0x4,
    VC1_SBP_2               = 0x2,
    VC1_SBP_3               = 0x1
};

//interlace frame
//field/frame transform
enum
{
    VC1_FRAME_TRANSFORM = 0,
    VC1_FIELD_TRANSFORM = 1,
    VC1_NO_CBP_TRANSFORM,
    VC1_NA_TRANSFORM
};

enum
{
    VC1_SBP_8X8_BLK          = 0,
    VC1_SBP_8X4_BOTTOM_BLK   = 1,
    VC1_SBP_8X4_TOP_BLK      = 2,
    VC1_SBP_8X4_BOTH_BLK     = 3,
    VC1_SBP_4X8_RIGHT_BLK    = 4,
    VC1_SBP_4X8_LEFT_BLK     = 5,
    VC1_SBP_4X8_BOTH_BLK     = 6,
    VC1_SBP_4X4_BLK          = 7,

    VC1_SBP_8X8_MB        = 8,
    VC1_SBP_8X4_BOTTOM_MB = 9,
    VC1_SBP_8X4_TOP_MB    = 10,
    VC1_SBP_8X4_BOTH_MB   = 11,
    VC1_SBP_4X8_RIGHT_MB  = 12,
    VC1_SBP_4X8_LEFT_MB   = 13,
    VC1_SBP_4X8_BOTH_MB   = 14,
    VC1_SBP_4X4_MB        = 15
};

//for LeftTopRightPositionFlag
enum
{
    VC1_COMMON_MB       = 0x000,
    VC1_LEFT_MB         = 0xA00,
    VC1_TOP_LEFT_MB     = 0xAC0,
    VC1_TOP_LEFT_RIGHT  = 0xAC5,
    VC1_LEFT_RIGHT_MB   = 0xA05,
    VC1_TOP_MB          = 0x0C0,
    VC1_TOP_RIGHT_MB    = 0x0C5,
    VC1_RIGHT_MB        = 0x005
};

#define VC1_IS_NO_LEFT_MB(value)  !(value&0x800)
#define VC1_IS_NO_TOP_MB(value)   !(value&0x80)
#define VC1_IS_NO_RIGHT_MB(value) (!(value&0x01))

//only left
#define VC1_IS_LEFT_MB(value)  (value&0x800)&&(!(value&0x80)) && (!(value&0x1))
//only top
#define VC1_IS_TOP_MB(value)(value&0x80)&&(!(value&0x800))&&(!(value&0x1))
//only right
#define VC1_IS_RIGHT_MB(value)  (value&0x01)&&(!(value&0x800))&&(!(value&0x80))
//left and top
#define VC1_IS_LEFT_TOP_MB(value)    (value&0x800)&&(value&0x80)&&(!(value&0x1))
#define VC1_IS_TOP_RIGHT_MB(value)   (value&0x80)&&(value&0x1)&&(!(value&0x800))

#define VC1_IS_LEFT_RIGHT_MB(value)  (value&0x800)&&(value&0x1)&&(!(value&0x80))
//#define VC1_IS_NOT_TOP_LEFT_MB(value) (!(value&0x80))&&(!(value&0x800))

//for IntraFlag
enum
{
    VC1_All_INTRA        = 0x3F,
    VC1_BLOCK_0_INTRA    = 0x01,
    VC1_BLOCK_1_INTRA    = 0x02,
    VC1_BLOCK_2_INTRA    = 0x04,
    VC1_BLOCK_3_INTRA    = 0x08,
    VC1_BLOCK_4_INTRA    = 0x10,
    VC1_BLOCKS_0_1_INTRA = 0x03,
    VC1_BLOCKS_2_3_INTRA = 0x0C,
    VC1_BLOCKS_0_2_INTRA = 0x05,
    VC1_BLOCKS_1_3_INTRA = 0x0A
};

//for smoothing
#define VC1_EDGE_MB(intraflag, value)  ((intraflag&value)==value)


#define VC1_IS_INTER_MB(value)  ((value == 0x00)||(value == 0x01)||(value == 0x02)||(value == 0x04)||(value == 0x08))

//for extended differantial MV range flag(inerlace P picture)
enum
{
    VC1_DMVRANGE_NONE = 0,
    VC1_DMVRANGE_HORIZONTAL_RANGE,
    VC1_DMVRANGE_VERTICAL_RANGE,
    VC1_DMVRANGE_HORIZONTAL_VERTICAL_RANGE
};


//intensity comprnsation
enum
{
    VC1_INTCOMP_TOP_FIELD    = 1,
    VC1_INTCOMP_BOTTOM_FIELD = 2,
    VC1_INTCOMP_BOTH_FIELD   = 3
};

#define VC1_IS_INT_TOP_FIELD(value)    (value & VC1_INTCOMP_TOP_FIELD)
#define VC1_IS_INT_BOTTOM_FIELD(value) (value & VC1_INTCOMP_BOTTOM_FIELD)
#define VC1_IS_INT_BOTH_FIELD(value)   (value & VC1_INTCOMP_BOTH_FIELD)

#define VC1_BRACTION_INVALID 0
#define VC1_BRACTION_BI 9


#define VC1_PADDING_SIZE       64
#define VC1_HORIZONTAL_PADDING 64


#define VC1_MAX_SLICE_NUM  512

struct VC1ACDecodeSet
{
    Ipp32s*             pRLTable;
    const Ipp8s*        pDeltaLevelLast0;
    const Ipp8s*        pDeltaLevelLast1;
    const Ipp8s*        pDeltaRunLast0;
    const Ipp8s*        pDeltaRunLast1;
};


#define VC1_IS_BITPLANE_RAW_MODE(bitplane) ((bitplane)->m_imode == VC1_BITPLANE_RAW_MODE)
#define VC1_IS_U_PRESENT_IN_CBPCY(value) (value & 2)
#define VC1_IS_V_PRESENT_IN_CBPCY(value) (value & 1)

#define VC1_NEXT_BITS(num_bits, value) VC1NextNBits(pContext->m_bitstream.pBitstream, pContext->m_bitstream.bitOffset, num_bits, value);
#define VC1_GET_BITS(num_bits, value)  VC1GetNBits(pContext->m_bitstream.pBitstream, pContext->m_bitstream.bitOffset, num_bits, value);

struct VC1_HRD_PARAMS
{
   Ipp32u HRD_NUM_LEAKY_BUCKETS; //5
   Ipp32u BIT_RATE_EXPONENT;     //4
   Ipp32u BUFFER_SIZE_EXPONENT;  //4

   // 32 - max size see Standard, p32
   Ipp32u HRD_RATE[32];      //16
   Ipp32u HRD_BUFFER[32];    //16
   Ipp32u HRD_FULLNESS[32];  //16
};

struct VC1Bitplane
{
    Ipp8u              m_invert;
    Ipp32s             m_imode;
    Ipp8u*             m_databits;
};

struct VC1SequenceLayerHeader
{
    //common field
    Ipp32u PROFILE;            //2

    //Advanced profile fields
    Ipp32u LEVEL;              //3
  //  Ipp32u CHROMAFORMAT;     //2

    //common fields
    Ipp32u FRMRTQ_POSTPROC;  //3
    Ipp32u BITRTQ_POSTPROC;  //5

    //Advanced profile fields
    Ipp32u POSTPROCFLAG;      //1
    Ipp32u MAX_CODED_WIDTH;   //12
    Ipp32u MAX_CODED_HEIGHT;  //12
    Ipp32u PULLDOWN;          //1
    Ipp32u INTERLACE;         //1
    Ipp32u TFCNTRFLAG;        //1

    //Simple/Main profile fields
    Ipp32u LOOPFILTER;       //1 uimsbf
    Ipp32u MULTIRES;         //1 uimsbf
    Ipp32u FASTUVMC;         //1 uimsbf
    Ipp32u EXTENDED_MV;      //1 uimsbf
    Ipp32u DQUANT;           //2 uimsbf
    Ipp32u VSTRANSFORM;      //1 uimsbf
    Ipp32u OVERLAP;          //1 uimsbf
    Ipp32u SYNCMARKER;       //1 uimsbf
    Ipp32u RANGERED;         //1 uimsbf
    Ipp32u MAXBFRAMES;       //3 uimsbf
    Ipp32u QUANTIZER;        //2 uimsbf

    //common fields
    Ipp32u FINTERPFLAG;        //1
    //Ipp32u DISPLAY_EXT;        //1
    Ipp32u HRD_PARAM_FLAG;     //1

    //HRD PARAMS
   Ipp32u HRD_NUM_LEAKY_BUCKETS; //5
   //Ipp32u BIT_RATE_EXPONENT;     //4
   //Ipp32u BUFFER_SIZE_EXPONENT;  //4

   // 32 - max size see Standard, p32
 //  Ipp32u HRD_RATE[32];   //16
 //  Ipp32u HRD_BUFFER[32]; //16

   //Ipp8u m_ubRoundCtl;

    Ipp16u widthMB;
    Ipp16u heightMB;

    Ipp32u IsResize;

//entry point
    Ipp32u BROKEN_LINK;
    Ipp32u CLOSED_ENTRY;
    Ipp32u PANSCAN_FLAG;
    Ipp32u REFDIST_FLAG;

    Ipp32u CODED_WIDTH;
    Ipp32u CODED_HEIGHT;
    Ipp32u EXTENDED_DMV;
    Ipp32u RANGE_MAPY_FLAG;
    Ipp32s RANGE_MAPY;
    Ipp32u RANGE_MAPUV_FLAG;
    Ipp32s RANGE_MAPUV;
    Ipp32u RNDCTRL;     // 1 rounding control bit

};

struct VC1MVRange
{
    Ipp8u  k_x;
    Ipp8u  k_y;
    Ipp16u r_x;
    Ipp16u r_y;
};

struct VC1PredictScaleValuesPPic
{
    Ipp16u   scaleopp;
    Ipp16u  scalesame1;
    Ipp16u   scalesame2;
    Ipp16u   scalezone1_x;
    Ipp16u   scalezone1_y;
    Ipp16u   zone1offset_x;
    Ipp16u   zone1offset_y;
};

struct VC1PredictScaleValuesBPic
{
    Ipp16u   scalesame;
    Ipp16u   scaleopp1;
    Ipp16u   scaleopp2;
    Ipp16u   scalezone1_x;
    Ipp16u   scalezone1_y;
    Ipp16u   zone1offset_x;
    Ipp16u   zone1offset_y;
};

//struct PanScanWindow
//{
//    Ipp32u PS_HOFFSET;        //18                pan scan window horizontaloffset
//    Ipp32u PS_VOFFSET;        //18                pan scan vertical window
//    Ipp32u PS_WIDTH;          //14                pan scan window width
//    Ipp32u PS_HEIGHT;         //14                pan scan window width
//};

struct VC1PictureLayerHeader
{
  //common fields
    //Ipp32u  INTERPFRM;
    Ipp32u  PTYPE;
    Ipp32u  PQINDEX;
    Ipp32u  HALFQP;
    //Ipp32u  POSTPROC;       // 2 post processing
//common fields.Slice parameters
    Ipp32u  is_slice;

//interlace fields
    Ipp8u  TFF;           // 1 top field first
    Ipp8u  PTypeField1;
    Ipp8u  PTypeField2;
    Ipp8u  CurrField;
    Ipp8u  BottomField;    //Is current field top or nor
    Ipp32u  INTCOMFIELD;     //variable size intensity compensation field

    //range reduce simple/main profile
    Ipp32s RANGEREDFRM;      //1

//I P B picture
    //Ipp32u TRANSACFRM;
    //Ipp32u TRANSACFRM2;
//I P B picture. VopDQuant
    Ipp32u  m_DQProfile;
    Ipp32u  m_DQuantFRM;
    Ipp32u  m_DQBILevel;
    //Ipp32u  m_PQDiff;
    Ipp32u  m_AltPQuant;
    Ipp32u  PQUANT;
    Ipp32u  m_PQuant_mode;
    //Ipp32u  m_curQPDeadZone;
    Ipp32u  QuantizationType;      //0 - uniform, 1 - nonuniform

//only for advanced profile
    Ipp32u  FCM;           // variable size frame coding mode

//practically not used
    //Ipp32u  RPTFRM;        // 2 repeat frame count
    //Ipp32u  UVSAMP;
    //Ipp32u  m_number_of_pan_scan_window;
    //PanScanWindow m_PanScanWindows[4];

//I BI picture
    VC1Bitplane    ACPRED;    // variable size  AC Prediction
    Ipp32u   CONDOVER;  // variable size  conditional overlap flag
    VC1Bitplane    OVERFLAGS; // variable size  conditional overlap macroblock pattern flags
//I BI picture. interlace frame
    VC1Bitplane    FIELDTX;     //variable size field transform flag

// P B picture
    Ipp32u   TTMBF;
    Ipp32u   TTFRM;
    Ipp32u    TTFRM_ORIG; //for future H/W support needs
    VC1Bitplane    SKIPMB;
    VC1Bitplane    MVTYPEMB;
    Ipp32u   MVMODE;

// P B picture. Interlace frame
    Ipp32u   DMVRANGE;              //variable size extended differential MV Range Flag
// P B picture. Interlace field
    Ipp32s   REFDIST;               //variable size  P Reference Distance


//P only. Interlace frame
 //   Ipp32u MV4SWITCH;             //1     4 motion vector switch
//P only. Interlace field
    Ipp32u NUMREF;                //1     Number of reference picture
    Ipp32u REFFIELD;              //1     Reference field picture indicator


//B only
    Ipp32u     BFRACTION;
    Ipp32u     BFRACTION_orig; // need for correct filling VA structures
    VC1Bitplane      m_DirectMB;
    Ipp32s           ScaleFactor;
// B only. Interlace field
    VC1Bitplane      FORWARDMB;         //variable size B Field forward mode
                                        //MB bit syntax element
// tables
    const VC1ACDecodeSet* m_pCurrIntraACDecSet;
    const VC1ACDecodeSet* m_pCurrInterACDecSet;
    Ipp32s*             m_pCurrCBPCYtbl;
    Ipp32s*             m_pCurrMVDifftbl;
    const VC1MVRange*  m_pCurrMVRangetbl;
    Ipp32s*             m_pCurrLumaDCDiff;
    Ipp32s*             m_pCurrChromaDCDiff;
    Ipp32s*             m_pCurrTTMBtbl;
    Ipp32s*             m_pCurrTTBLKtbl;
    Ipp32s*             m_pCurrSBPtbl;
    Ipp32s*             m_pMBMode;
    Ipp32s*             m_pMV2BP;
    Ipp32s*             m_pMV4BP;

    const VC1PredictScaleValuesPPic*      m_pCurrPredScaleValuePPictbl;
    const VC1PredictScaleValuesBPic*      m_pCurrPredScaleValueB_BPictbl;
    const VC1PredictScaleValuesPPic*      m_pCurrPredScaleValueP_BPictbl[2];//0 - forward, 1 - back

    Ipp32u RNDCTRL;     // 1 rounding control bit

    Ipp32u TRANSDCTAB;
    Ipp32u MVMODE2;
    Ipp32u MVTAB;
    Ipp32u CBPTAB;
    Ipp32u MBMODETAB;
    Ipp32u MV2BPTAB;   //2             2 mv block pattern table
    Ipp32u MV4BPTAB;   //2             4 mv block pattern table
    Ipp32u PQUANTIZER;
    Ipp32u MVRANGE;
    Ipp32u DQSBEdge;
    // P picture
    Ipp32u LUMSCALE;
    Ipp32u LUMSHIFT;
    Ipp32u LUMSCALE1;
    Ipp32u LUMSHIFT1;

    Ipp32u POSTPROC;

    Ipp32u TRANSACFRM;
    Ipp32u TRANSACFRM2;

    Ipp32u  RPTFRM;        // 2 repeat frame count
    Ipp32u  UVSAMP;

    Ipp32u MV4SWITCH;
};

struct VC1VLCTables
{
    Ipp32s *m_pLowMotionLumaDCDiff;
    Ipp32s *m_pHighMotionLumaDCDiff;
    Ipp32s *m_pLowMotionChromaDCDiff;
    Ipp32s *m_pHighMotionChromaDCDiff;
    Ipp32s *m_pCBPCY_Ipic;
    Ipp32s *m_Bitplane_IMODE;
    Ipp32s *m_BitplaneTaledbits;
    Ipp32s *MVDIFF_PB_TABLES[4];
    Ipp32s *CBPCY_PB_TABLES[4];
    Ipp32s *TTMB_PB_TABLES[3];
    Ipp32s *TTBLK_PB_TABLES[3];
    Ipp32s *SBP_PB_TABLES[3];
    Ipp32s *BFRACTION;
    Ipp32s *MBMODE_INTERLACE_FRAME_TABLES[8];
    Ipp32s *MV_INTERLACE_TABLES[12];
    Ipp32s *CBPCY_PB_INTERLACE_TABLES[8];
    Ipp32s *MV2BP_TABLES[4];
    Ipp32s *MV4BP_TABLES[4];
    Ipp32s *REFDIST_TABLE;
    Ipp32s *MBMODE_INTERLACE_FIELD_TABLES[8];
    Ipp32s *MBMODE_INTERLACE_FIELD_MIXED_TABLES[8];

    //////////////////////////////////////////////////////////////////////////
    //////////////////////Intra Decoding Sets/////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    VC1ACDecodeSet LowMotionIntraACDecodeSet;
    VC1ACDecodeSet HighMotionIntraACDecodeSet;
    VC1ACDecodeSet MidRateIntraACDecodeSet;
    VC1ACDecodeSet HighRateIntraACDecodeSet;
    VC1ACDecodeSet* IntraACDecodeSetPQINDEXle7[3];
    VC1ACDecodeSet* IntraACDecodeSetPQINDEXgt7[3];

    //////////////////////////////////////////////////////////////////////////
    //////////////////////Inter Decoding Sets/////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    VC1ACDecodeSet LowMotionInterACDecodeSet;
    VC1ACDecodeSet HighMotionInterACDecodeSet;
    VC1ACDecodeSet MidRateInterACDecodeSet;
    VC1ACDecodeSet HighRateInterACDecodeSet;
    VC1ACDecodeSet* InterACDecodeSetPQINDEXle7[3];
    VC1ACDecodeSet* InterACDecodeSetPQINDEXgt7[3];

};

struct VC1DCBlkParam
{
    Ipp16s          DC;
    Ipp16s          ACTOP[8];
    Ipp16s          ACLEFT[8];
};

struct VC1DCMBParam
{
    VC1DCBlkParam   DCBlkPred[6];
    Ipp32u          DCStepSize;
    Ipp32u          DoubleQuant;
};

//  luma
//  |--|--|--|
//  | 3| 6| 7|
//  |--|--|--|
//  | 8| 0| 1|
//  |--|--|--|
//  | 9| 2|  |
//  |--|--|--|


//  chroma
//  |--|--|   |--|--|
//  | 4|10|   | 5|12|
//  |--|--|   |--|--|
//  |11|  |   |13|  |
//  |--|--|   |--|--|

typedef struct
{
    Ipp32u          DoubleQuant[3];
    Ipp16s          DC[14];
    Ipp16s*         ACTOP[14];
    Ipp16s*         ACLEFT[14];

    Ipp8u           BlkPattern[6];
}VC1DCPredictors;

typedef struct
{
    IppiSize        DstSizeNZ;
    Ipp32u          SBlkPattern;
    Ipp16s          mv[2][2];                   // [forw/back][x/y] top field
    Ipp16s          mv_bottom[2][2];            // [forw/back][x/y] bottom field
    Ipp8u           mv_s_polarity[2];
    Ipp8u           blkType;
    Ipp8u           fieldFlag[2];   //0 - top field, 1 - bottom field
}VC1Block;

typedef struct
{
    Ipp8u   Coded;
    Ipp8u numCoef; //subblocks in inter;
}VC1SingletonBlock;
typedef struct
{
    Ipp32u      bEscapeMode3Tbl;
    Ipp32s      levelSize;
    Ipp32s      runSize;
}VC1EscInfo;

typedef struct
{
    Ipp32u*     pBitstream;
    Ipp32s      bitOffset;
}VC1Bitstream;

typedef struct
{
    Ipp32s      m_currMBYpos;
    Ipp32s      m_currMBXpos;
    Ipp8u*      currYPlane;
    Ipp8u*      currUPlane;
    Ipp8u*      currVPlane;

    Ipp32u      currYPitch;
    Ipp32u      currUPitch;
    Ipp32u      currVPitch;

    Ipp32s      slice_currMBYpos;

    Ipp32u      ACPRED;
    Ipp32u      INTERPMVP;
    Ipp32s      MBMODEIndex;
    Ipp8u       m_ubNumFirstCodedBlk;

    VC1SingletonBlock   m_pSingleBlock[6];
    //for interpolation
    Ipp16s      xLuMV[VC1_NUM_OF_LUMA];
    Ipp16s      yLuMV[VC1_NUM_OF_LUMA];
    Ipp32u      MVcount;

    Ipp16s      xLuMVT[VC1_NUM_OF_LUMA];
    Ipp16s      yLuMVT[VC1_NUM_OF_LUMA];

    Ipp16s      xLuMVB[VC1_NUM_OF_LUMA];
    Ipp16s      yLuMVB[VC1_NUM_OF_LUMA];

    Ipp16s*     x_LuMV;
    Ipp16s*     y_LuMV;

    VC1EscInfo    EscInfo;
    const Ipp8u** ZigzagTable;
    Ipp32s      widthMB;
    Ipp32s      heightMB;
}VC1SingletonMB;

#ifdef UMC_STREAM_ANALYZER
   inline static Ipp32u CalculateUsedBits(IppiBitstream BitStartPos,IppiBitstream BitEndPos)
   {
       if (BitEndPos.pBitstream == BitStartPos.pBitstream)
           return BitStartPos.bitOffset - BitEndPos.bitOffset;
       else
           // offset is value between [0,31]
           return (BitEndPos.pBitstream - BitStartPos.pBitstream)*sizeof(Ipp32u)*8 - 1 -
                   BitEndPos.bitOffset + BitStartPos.bitOffset;
   }
#endif

typedef struct
{
    Ipp32s      m_cbpBits;

    VC1Block    m_pBlocks[6];
    Ipp8u       Overlap;
    Ipp8u       mbType;

    //interlace
    Ipp32u      FIELDTX;
    Ipp8u       IntraFlag;

    Ipp8u*      currYPlane;
    Ipp8u*      currUPlane;
    Ipp8u*      currVPlane;

    Ipp32u      currYPitch;
    Ipp32u      currUPitch;
    Ipp32u      currVPitch;

    Ipp32s      MVBP;
    Ipp32s      LeftTopRightPositionFlag;
    Ipp32s      MVSW;          // for interlace frame mode in B frames

    Ipp16s      bias;
    Ipp32u       SkipAndDirectFlag; //skip or not + direct or not
    Ipp16s      dmv_x[2][4]; //for split decode and prediction calculation in B frames
    Ipp16s      dmv_y[2][4]; //for split decode and prediction calculation in B frames
    Ipp8u       predictor_flag[4]; // for B interlace fields
    Ipp8u       fieldFlag[2];
    const Ipp8u* pInterpolLumaSrc[2];    //forward/backward or top/bottom
    Ipp32s       InterpolsrcLumaStep[2]; //forward/backward or top/bottom
    const Ipp8u* pInterpolChromaUSrc[2];    //forward/backward or top/bottom
    Ipp32s       InterpolsrcChromaUStep[2]; //forward/backward or top/bottom
    const Ipp8u* pInterpolChromaVSrc[2];    //forward/backward or top/bottom
    Ipp32s       InterpolsrcChromaVStep[2]; //forward/backward or top/bottom
#ifdef UMC_STREAM_ANALYZER
    ExtraMBAnalyzer* pMbAnalyzInfo;
#endif

}VC1MB;
typedef struct
{
    VC1Block* AMVPred[4];
    VC1Block* BMVPred[4];
    VC1Block* CMVPred[4];
    Ipp8u     FieldMB[4][3]; //4 blocks, A, B, C
}VC1MVPredictors;
//1.3.3    The decoder dynamically allocates memory for frame
//buffers and other structures. Following are the size requirements.
//For the frame buffer, the following formula indicates the memory requirement:
//frame_memory = {(h + 64) x (w + 64) + [(h / 2 + 32) x (w / 2 + 32)] x 2}bytes

struct Frame
{
    Frame():m_pAllocatedMemory(NULL),
            m_pY(NULL),
            m_pU(NULL),
            m_pV(NULL),
            RANGE_MAPY(-1),
            RANGE_MAPUV(-1),
            m_iYPitch(0),
            m_iUPitch(0),
            m_iVPitch(0),
            m_AllocatedMemorySize(0),
            m_bIsExpanded(0),
            TFF(0)
    {
    }



    Ipp8u*      m_pAllocatedMemory;
    Ipp8u*      m_pY;
    Ipp8u*      m_pU;
    Ipp8u*      m_pV;
    Ipp32s      RANGE_MAPY;
    Ipp32s      RANGE_MAPUV;//[2]; //rangeMapCoef[0] = RANGE_MAPY;  - luma
                         //rangeMapCoef[1] = RANGE_MAPUV; - chroma
    Ipp32s*     pRANGE_MAPY;

    Ipp32u      m_iYPitch;
    Ipp32u      m_iUPitch;
    Ipp32u      m_iVPitch;
    Ipp32s      m_AllocatedMemorySize;

    Ipp8u       m_bIsExpanded;
    Ipp32u      FCM;
    Ipp8u      LumaTable[4][256]; //0,1 - top/bottom fields of first field. 2,3 of second field
    Ipp8u      ChromaTable[4][256];

    Ipp8u*     LumaTablePrev[4];
    Ipp8u*     ChromaTablePrev[4];

    Ipp8u*     LumaTableCurr[2];
    Ipp8u*     ChromaTableCurr[2];




    Ipp32u      TFF;

    //1000  - IC for Top First Field
    //0100  - IC for Bottom First Field
    //0010  - IC for Top Second Field
    //0001  - IC for Bottom Second Field
    Ipp32u      ICFieldMask;

    Ipp32u      isIC;
};

#define VC1MAXFRAMENUM 9*VC1FRAMEPARALLELPAIR + 9 // for <= 8 threads. Change if numThreads > 8
#define VC1NUMREFFRAMES 2 // 2 - reference frames
//typedef struct
//{
//    Frame         m_pFrames[VC1MAXFRAMENUM];
//
//    Ipp32s        m_iPrevIndex;
//    Ipp32s        m_iNextIndex;
//    Ipp32s        m_iCurrIndex;
//    Ipp32s        m_iICompFieldIndex;
//    Ipp32s        m_iDisplayIndex;
//}VC1FrameBuffer;
struct VC1FrameBuffer
{
    Frame*        m_pFrames;

    Ipp32s        m_iPrevIndex;
    Ipp32s        m_iNextIndex;
    Ipp32s        m_iCurrIndex;
    Ipp32s        m_iICompFieldIndex;
    Ipp32s        m_iDisplayIndex;
    // for external index set up
    Ipp32s        m_iBFrameIndex;

    Frame operator [] (Ipp32u number)
    {
        return m_pFrames[number];
    };

};

typedef struct
{
    Ipp16s**        SmoothUpperYRows;   //   Y:[MBwidth+1][16*2]
    Ipp16s**        SmoothUpperURows;   //   U:[MBwidth+1][8*2]
    Ipp16s**        SmoothUpperVRows;   //   V:[MBwidth+1][8*2]
}VC1Smoothing;

typedef struct
{
    Ipp32u start_pos;
    Ipp32u HeightMB;
    Ipp32u is_last_deblock;
}VC1DeblockInfo;


struct VC1Context
{
    VC1SequenceLayerHeader*    m_seqLayerHeader;
    VC1PictureLayerHeader*     m_picLayerHeader;

    VC1PictureLayerHeader*     m_InitPicLayer;

    VC1VLCTables*              m_vlcTbl;

    VC1FrameBuffer             m_frmBuff;

    //Ipp32u*                m_pbs;
    //Ipp32s                 m_bitOffset;
    VC1Bitstream           m_bitstream;
    Ipp8u*                 m_pBufferStart;
    Ipp32u                 m_FrameSize;
    //start codes
    Ipp32u*                m_Offsets;
    Ipp32u*                m_values;

    Ipp32s                 m_seqInit;

    VC1MB*                 m_pCurrMB;
    VC1MB*                 m_MBs;         //MBwidth*MBheight
    VC1DCMBParam*          CurrDC;

    VC1Smoothing*          SmoothingInfo;
    Ipp16s*                savedMV;       //MBwidth*MBheight*4*2*2
                                          //(4 luma blocks, 2 coordinates,Top/Bottom),
                                          //MVs which are used for Direct mode
    Ipp8u*                 savedMVSamePolarity;
                                          // in B frame
    Ipp16s*                savedMV_Curr;     //pointer to current array of MVs. Need for inter-frame threading
    Ipp8u*                 savedMVSamePolarity_Curr; //pointer to current array of MVPolars. Need for inter-frame threading
    VC1Bitplane            m_pBitplane;
    VC1DeblockInfo         DeblockInfo;
    Ipp32u                 RefDist;
    Ipp32u*                pRefDist;

    IppVCInterpolateBlockIC_8u interp_params_luma;
    IppVCInterpolateBlockIC_8u interp_params_chroma;


    // Intensity compensation: lookup tables only for P-frames
    Ipp8u          m_bIntensityCompensation;
    Ipp8u          m_bNeedToUseCompBuffer;

    Ipp32u typeOfPreviousFrame;
    VC1SingletonMB*                m_pSingleMB;

    Ipp32s                         iNumber; /*thread number*/
    Ipp16s*                        m_pBlock; //memory for diffrences
    Ipp32s                         iPrevDblkStartPos;
    VC1DCMBParam*                  DCACParams;
    VC1DCPredictors                DCPred;
    VC1MVPredictors                MVPred;
    Ipp8u*                        LumaTable[4]; //0,1 - top/bottom fields of first field. 2,3 of second field
    Ipp8u*                        ChromaTable[4];
    Ipp32s                        bp_round_count;
};


#endif //__umc_vc1_common_defs_H__
