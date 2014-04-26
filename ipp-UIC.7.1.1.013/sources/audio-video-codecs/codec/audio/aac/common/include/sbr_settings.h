/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_SETTINGS_H__
#define __SBR_SETTINGS_H__

/********************************************************************/
/*           MAX CHANNEL FOR HE-AAC PROFILE IS 6 (5.1) SEE SPEC     */
/********************************************************************/

#define SBR_MAX_CH 6

/********************************************************************/
/*                  USEFUL CONST                                    */
/********************************************************************/

/* NULL */
#ifndef NULL
#define NULL                0
#endif

#define RATE                2

#define EMPTY_MAPPING       (-100)

/********************************************************************/
/*                   TYPE SBR FRAME CLASS                           */
/********************************************************************/
#define FIXFIX              0
#define FIXVAR              1
#define VARFIX              2
#define VARVAR              3

/********************************************************************/
/*                   SIZE TABS/VECS                                 */
/********************************************************************/
#define MAX_SIZE_FREQ_TABS 64

#define MAX_NUM_PATCHES     6

#define MAX_NUM_ENV         5

#define MAX_NUM_NOISE_ENV   2

#define MAX_NUM_ENV_VAL   100

#define MAX_NUM_NOISE_VAL   5

/********************************************************************/
/*                   INV_FILTERING_LEVEL                            */
/********************************************************************/

#define INVF_OFF_LEVEL        0
#define INVF_LOW_LEVEL        1
#define INVF_INTERMEDIA_LEVEL 2
#define INVF_STRONG_LEVEL     3

/********************************************************************/
/*                   SBR_HEADER_PARAMS                              */
/********************************************************************/

/* default values for header extra 1 params */
#define BS_FREQ_SCALE_DEFAULT     2
#define BS_ALTER_SCALE_DEFAULT    1
#define BS_NOISE_BANDS_DEFAULT    2

/* default values for header extra 2 params */
#define BS_LIMITER_BANDS_DEFAULT  2
#define BS_LIMITER_GAINS_DEFAULT  2
#define BS_INTERPOL_FREQ_DEFAULT  1
#define BS_SMOOTHING_MODE_DEFAULT 1

/********************************************************************/
/*                   HIGH/LOW FREQ RESOLUTION                       */
/********************************************************************/
#define HI 1
#define LO 0

/********************************************************************/
/*                   SBR_ERR_ PARSER / PAYLOAD_FORMATTER            */
/********************************************************************/
#define SBR_ERR_REQUIREMENTS   (-13)

#define SBR_ERR_PARSER         (-7)
#define SBR_OK                 0

#define SBR_NO_ERR             0

/********************************************************************/

#endif             /* __SBR_SETTINGS_H__ */
