/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, common definitions
//
*/

#ifndef _ENCODER_VC1_DEF_H_
#define _ENCODER_VC1_DEF_H_

#include "umc_vc1_common_defs_enc.h"

namespace UMC_VC1_ENCODER
{

#define VC1_ENC_STR_LEN             200
#define VC1_ENC_LUMA_SIZE           16
#define VC1_ENC_CHROMA_SIZE         8
#define VC1_ENC_DC_ESCAPE_INDEX     119
#define VC1_ENC_NUMBER_OF_BLOCKS    6

#define VC1_ENC_HALF_MV_LIMIT       95

#define VC1_ENC_CHECK_MV

#define VC1_ENC_ORIG_FRAMES_FOR_ME     1
#define VC1_ENC_ORIG_FRAMES_FOR_SKIP   2
#define VC1_ENC_ORIG_FRAMES_FOR_STAT   4

#define VC1_ENC_MAX_REF_CURR_FRAME     3

#define VC1_ENC_CHECK(err) if (err != UMC::UMC_OK) {return err;}
#define VC1_ENC_IPP_CHECK(Sts) if(Sts != ippStsNoErr) {return Sts;}

#define VC1_NULL_PTR(arg)       if (arg == NULL)        {return UMC::UMC_ERR_NULL_PTR;}

#define VC_ENC_PATTERN_POS1(blk, subblk) (((5-(blk))<<2)+ (subblk))
#define VC_ENC_PATTERN_POS(blk)  ((Ipp8u)(5-(blk)))
typedef enum
{
    VC1_ENC_PROFILE_S = 0x00,
    VC1_ENC_PROFILE_M = 0x04,
    VC1_ENC_PROFILE_A = 0x0C,

}eProfile;

enum
{
    VC1_ENC_LEVEL_0 = 0x00,
    VC1_ENC_LEVEL_1 = 0x01,
    VC1_ENC_LEVEL_2 = 0x02,
    VC1_ENC_LEVEL_3 = 0x03,
    VC1_ENC_LEVEL_4 = 0x04,
};

enum
{
    VC1_ENC_LEVEL_S = 0x00,
    VC1_ENC_LEVEL_M = 0x02,
    VC1_ENC_LEVEL_H = 0x04,
};

typedef enum
{
    VC1_ENC_I_FRAME    =  0 ,
    VC1_ENC_P_FRAME         ,
    VC1_ENC_P_FRAME_MIXED   ,
    VC1_ENC_B_FRAME         ,
    VC1_ENC_BI_FRAME        ,
    VC1_ENC_SKIP_FRAME      ,
    VC1_ENC_I_I_FIELD       ,
    VC1_ENC_I_P_FIELD       ,
    VC1_ENC_P_I_FIELD       ,
    VC1_ENC_P_P_FIELD       ,
    VC1_ENC_B_B_FIELD       ,
    VC1_ENC_B_BI_FIELD      ,
    VC1_ENC_BI_B_FIELD      ,
    VC1_ENC_BI_BI_FIELD
} ePType;

typedef enum
{
    VC1_ENC_I_MB      =  0 ,
    VC1_ENC_P_MB_1MV       ,
    VC1_ENC_P_MB_4MV       ,
    VC1_ENC_P_MB_INTRA     ,
    VC1_ENC_P_MB_SKIP_1MV  ,
    VC1_ENC_P_MB_SKIP_4MV  ,
    VC1_ENC_B_MB_F         ,
    VC1_ENC_B_MB_B         ,
    VC1_ENC_B_MB_FB        ,
    VC1_ENC_B_MB_INTRA     ,
    VC1_ENC_B_MB_SKIP_F    ,
    VC1_ENC_B_MB_SKIP_B    ,
    VC1_ENC_B_MB_SKIP_FB   ,
    VC1_ENC_B_MB_DIRECT    ,
    VC1_ENC_B_MB_SKIP_DIRECT

} eMBType;

typedef enum
{
    VC1_ENC_QTYPE_IMPL = 0, /*from table*/
    VC1_ENC_QTYPE_EXPL = 1, /*from stream*/
    VC1_ENC_QTYPE_NUF  = 2, /*non-uniform*/
    VC1_ENC_QTYPE_UF   = 3, /*uniform*/

} eQuantType;
typedef enum
{
    VC1_ENC_RES_FULL_HOR = 0,
    VC1_ENC_RES_FULL_VER = 0,
    VC1_ENC_RES_HALF_HOR = 1,
    VC1_ENC_RES_HALF_VER = 2,

} eResolution;
typedef
struct
{
    Ipp8u num;
    Ipp8u denom;
} sFraction;



typedef struct
{
    const Ipp8u * pLSizeTable;
    Ipp8u         levelSize;
    Ipp8u         runSize;
} sACEscInfo;



typedef struct
{
    Ipp8u LeftMB;
    Ipp8u TopMB;
    Ipp8u TopLeftMB;
}NeighbouringMBsIntraPattern;


enum
{
  VC1_ENC_PAT_POS_Y0 = 5,
  VC1_ENC_PAT_POS_Y1 = 4,
  VC1_ENC_PAT_POS_Y2 = 3,
  VC1_ENC_PAT_POS_Y3 = 2,
  VC1_ENC_PAT_POS_Cb = 1,
  VC1_ENC_PAT_POS_Cr = 0,
};
enum
{
    VC1_ENC_PAT_MASK_Y0  = 1 << VC1_ENC_PAT_POS_Y0,
    VC1_ENC_PAT_MASK_Y1  = 1 << VC1_ENC_PAT_POS_Y1,
    VC1_ENC_PAT_MASK_Y2  = 1 << VC1_ENC_PAT_POS_Y2,
    VC1_ENC_PAT_MASK_Y3  = 1 << VC1_ENC_PAT_POS_Y3,
    VC1_ENC_PAT_MASK_Cb  = 1 << VC1_ENC_PAT_POS_Cb,
    VC1_ENC_PAT_MASK_Cr  = 1 << VC1_ENC_PAT_POS_Cr,
    VC1_ENC_PAT_MASK_LUMA   =  VC1_ENC_PAT_MASK_Y0|VC1_ENC_PAT_MASK_Y1|VC1_ENC_PAT_MASK_Y2|VC1_ENC_PAT_MASK_Y3,
    VC1_ENC_PAT_MASK_CHROMA =  VC1_ENC_PAT_MASK_Cb|VC1_ENC_PAT_MASK_Cr,
    VC1_ENC_PAT_MASK_MB     =  VC1_ENC_PAT_MASK_LUMA|VC1_ENC_PAT_MASK_CHROMA
};
typedef enum
{
    VC1_ENC_NONE = 0,
    VC1_ENC_LEFT = 1,
    VC1_ENC_TOP  = 2,
}eDirection;

typedef enum
{
    VC1_ENC_LEV_DELTA_NOT_LAST  = 0,
    VC1_ENC_RUN_DELTA_NOT_LAST  = 1,
    VC1_ENC_IND_DELTA_NOT_LAST  = 2,
    VC1_ENC_LEV_DELTA_LAST      = 3,
    VC1_ENC_RUN_DELTA_LAST      = 4,
    VC1_ENC_IND_DELTA_LAST      = 5,

}eDeltaTableNum;



typedef enum
{
    VC1_ENC_CURR_PLANE          = 0,
    VC1_ENC_RAISED_PLANE        = 1,
    VC1_ENC_FORWARD_PLANE       = 2,
    VC1_ENC_BACKWARD_PLANE      = 3,
    VC1_ENC_FORWARD_ME_PLANE    = 4,
    VC1_ENC_BACKWARD_ME_PLANE   = 5

}ePlaneType;

typedef enum
{
    VC1_ENC_1MV_HALF_BILINEAR             = 0,
    VC1_ENC_1MV_QUARTER_BICUBIC           = 1,
    VC1_ENC_1MV_HALF_BICUBIC              = 2,
    VC1_ENC_MIXED_QUARTER_BICUBIC         = 3,
}eMVModes;

typedef enum
{
    VC1_ENC_QUANT_SINGLE                  = 0,
    VC1_ENC_QUANT_MB_ANY                  = 1,
    VC1_ENC_QUANT_MB_PAIR                 = 2,
    VC1_ENC_QUANT_EDGE_ALL                = 3,
    VC1_ENC_QUANT_EDGE_LEFT               = 4,
    VC1_ENC_QUANT_EDGE_TOP                = 5,
    VC1_ENC_QUANT_EDGE_RIGHT              = 6,
    VC1_ENC_QUANT_EDGE_BOTTOM             = 7,
    VC1_ENC_QUANT_EDGES_LEFT_TOP          = 8,
    VC1_ENC_QUANT_EDGES_TOP_RIGHT         = 9,
    VC1_ENC_QUANT_EDGES_RIGHT_BOTTOM      = 10,
    VC1_ENC_QUANT_EDGSE_BOTTOM_LEFT       = 11,

}eQuantMode;

typedef enum
{
    VC1_ENC_REF_FIELD_FIRST   = 1,
    VC1_ENC_REF_FIELD_SECOND  = 2,
    VC1_ENC_REF_FIELD_BOTH    = VC1_ENC_REF_FIELD_FIRST|VC1_ENC_REF_FIELD_SECOND

}eReferenceFieldType;
typedef enum
{
    VC1_ENC_FIELD_NONE    = 0,
    VC1_ENC_TOP_FIELD     = 1,
    VC1_ENC_BOTTOM_FIELD  = 2,
    VC1_ENC_BOTH_FIELD    = VC1_ENC_TOP_FIELD|VC1_ENC_BOTTOM_FIELD
}eFieldType;



typedef enum
{
   VC1_ENC_FORWARD = 0,
   VC1_ENC_BACKWARD =1,
   VC1_ENC_INTERPOLATED = 2,
}eBMVType;

struct sPlanes
{
    Ipp8u* pY;
    Ipp8u* pU;
    Ipp8u* pV;
    Ipp32u YStep;
    Ipp32u UStep;
    Ipp32u VStep;
};
struct sCoordinate
{
    Ipp16s x;
    Ipp16s y;
    bool   bSecond;
};
struct sScaleInfo
{
    Ipp16s scale_opp;
    Ipp16s scale_same1;
    Ipp16s scale_same2;
    Ipp16s scale_zoneX;
    Ipp16s scale_zoneY;
    Ipp16s zone_offsetX;
    Ipp16s zone_offsetY;
    Ipp16s rangeX;
    Ipp16s rangeY;
    bool   bBottom;

};
struct sMVFieldInfo
{
    const Ipp8u * pMVSizeOffsetFieldIndexX;
    const Ipp8u * pMVSizeOffsetFieldIndexY;
    const Ipp16s* pMVSizeOffsetFieldX;
    const Ipp16s* pMVSizeOffsetFieldY;
    const Ipp32u* pMVModeField1RefTable_VLC;
    Ipp16s        limitX;
    Ipp16s        limitY;
    bool          bExtendedX;
    bool          bExtendedY;
};

typedef struct
{
    Ipp32u MeCurrIndex;
    Ipp32u MeRefFIndex;
    Ipp32u MeRefBIndex;
}MeIndex;
}

#endif
