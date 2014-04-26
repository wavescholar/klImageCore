/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3ENC_HUFFTABLES_H__
#define __MP3ENC_HUFFTABLES_H__

#include "ippdc.h"
#include "ippac.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const Ipp32s mp3enc_VLCShifts[];
extern const Ipp32s mp3enc_VLCOffsets[];
extern const Ipp32s mp3enc_VLCTypes[];
extern const Ipp32s mp3enc_VLCTableSizes[];
extern const Ipp32s mp3enc_VLCNumSubTables[];
extern const Ipp32s *mp3enc_VLCSubTablesSizes[];
extern const IppsVLCTable_32s *mp3enc_VLCBooks[];

extern const Ipp16s mp3enc_table32[];
extern const Ipp16s mp3enc_table33[];

#ifdef __cplusplus
}
#endif

#endif  //      __MP3ENC_HUFFTABLES_H__
