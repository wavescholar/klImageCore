/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_DEC_SETTINGS_H__
#define __SBR_DEC_SETTINGS_H__

/* my val */
#define SBR_NOT_SCALABLE   0
#define SBR_MONO_BASE      1
#define SBR_STEREO_ENHANCE 2
#define SBR_STEREO_BASE    3
#define SBR_NO_DATA        4

/* standard */
#define NUM_TIME_SLOTS     16
#define SBR_DEQUANT_OFFSET 6
#define RATE               2

#define SBR_TIME_HFGEN     8
#define SBR_TIME_HFADJ     2

/* user params for decoder */
#define HEAAC_HQ_MODE      18
#define HEAAC_LP_MODE      28

#define HEAAC_DWNSMPL_ON   1
#define HEAAC_DWNSMPL_OFF  0

#define HEAAC_PARAMS_UNDEF -999

/* SIZE OF WORK BUFFER */
#define SBR_MINSIZE_OF_WORK_BUFFER 6 * 100 * sizeof(Ipp32s)

//#define MAX_NUM_ENV_VAL  100

//#define MAX_NUM_NOISE_VAL  5

/********************************************************************/

/* ENABLE SBR */
typedef enum {
  SBR_DISABLE = 0,
  SBR_ENABLE  = 777,
  SBR_UNDEF   = -999

}eSBR_SUPPORT;

#endif             /* __SBR_DEC_SETTINGS_H__ */
