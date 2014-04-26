/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3DEC_HUFTABS_H__
#define __MP3DEC_HUFTABS_H__

#include "ippdc.h"
#include "ippac.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Ipp32s mp3dec_VLCShifts[];
extern Ipp32s mp3dec_VLCOffsets[];
extern Ipp32s mp3dec_VLCTypes[];
extern Ipp32s mp3dec_VLCTableSizes[];
extern Ipp32s mp3dec_VLCNumSubTables[];
extern Ipp32s *mp3dec_VLCSubTablesSizes[];
extern IppsVLCTable_32s *mp3dec_VLCBooks[];

extern Ipp32s mp3idec_VLCShifts[];
extern Ipp32s mp3idec_VLCOffsets[];
extern Ipp32s mp3idec_VLCTypes[];
extern Ipp32s mp3idec_VLCTableSizes[];
extern Ipp32s mp3idec_VLCNumSubTables[];
extern Ipp32s *mp3idec_VLCSubTablesSizes[];
extern IppsVLCTable_32s *mp3idec_VLCBooks[];

#ifdef __cplusplus
}
#endif

#endif //__HUFTABS_H__
