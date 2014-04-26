/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PS_DEC_SETTINGS_H__
#define __PS_DEC_SETTINGS_H__

/* ERR_SIGN */
#define PS_ERR_PARSER               (-13)
#define PS_NO_ERR                   ( 0 )

/* bit stream element */
#define EXTENSION_ID_PS             (0x2)
#define PS_EXTENSION_VER0           ( 0 )

/* MAX MODE */
#define MAX_PS_ENV                  (4+1)
#define MAX_IID_MODE                (5+1)
#define MAX_ICC_MODE                (7+1)
#define MAX_IPD_MODE                (17+1)
#define MAX_OPD_MODE                (MAX_IPD_MODE)

/* resolution bins */
#define NUM_HI_RES                  ( 34 )
#define NUM_MID_RES                 ( 20 )
#define NUM_LOW_RES                 ( 10 )

/* hybrid analysis configuration */
#define CONFIG_HA_BAND34            ( 34 )
#define CONFIG_HA_BAND1020          (1020)

/* quantizations grid for IID */
#define NUM_IID_GRID_STEPS          (7)
#define NUM_IID_FINE_GRID_STEPS     (15)

/* NULL pointer */
#ifndef NULL
#define NULL 0
#endif

/* Hybrid Analysis Filter */
#define LEN_PS_FILTER               (12)
#define DELAY_PS_FILTER             (6)

/* SBR */
#define NUM_SBR_BAND                (32)

/* support of PS */
#define PS_DISABLE                  (0)
#define PS_PARSER                   (1)
#define PS_ENABLE_BL                (111)
#define PS_ENABLE_UR                (411)

/* strategy of mixing L/R channel */
#define PS_MIX_RA                   (0)
#define PS_MIX_RB                   (1)

#endif             /* __PS_DEC_SETTINGS_H__ */
/* EOF */
