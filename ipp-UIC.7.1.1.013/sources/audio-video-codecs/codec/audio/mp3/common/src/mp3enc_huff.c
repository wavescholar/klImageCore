/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own.h"

Ipp32s mp3enc_huffInit(MP3Enc_HuffmanTable *htables, Ipp8u *mem, Ipp32s *size_all)
{
    Ipp32s  i, ts, size = 0;

    if (htables) {
      for(i = 0; i < 16; i++)
        htables[i].linbits = 0;
      htables[16].linbits = 1;
      htables[17].linbits = 2;
      htables[18].linbits = 3;
      htables[19].linbits = 4;
      htables[20].linbits = 6;
      htables[21].linbits = 8;
      htables[22].linbits = 10;
      htables[23].linbits = 13;
      htables[24].linbits = 4;
      htables[25].linbits = 5;
      htables[26].linbits = 6;
      htables[27].linbits = 7;
      htables[28].linbits = 8;
      htables[29].linbits = 9;
      htables[30].linbits = 11;
      htables[31].linbits = 13;

      for (i = 0; i < 32; i++) {
        htables[i].phuftable = NULL;
      }

      for (i = 15; i < 32; i++) {
        htables[i].mav_value = 15 + ((1 << htables[i].linbits) - 1);
      }
    }


    for (i = 1; i <= 16; i++) {
      if (mp3enc_VLCBooks[i]) {
        ippsVLCEncodeGetSize_32s(mp3enc_VLCBooks[i],
          mp3enc_VLCTableSizes[i], &ts);
        size += ts;
        if (mem) {
          htables[i].phuftable = (IppsVLCEncodeSpec_32s *)mem;
          mem += ts;
          ippsVLCEncodeInit_32s(mp3enc_VLCBooks[i],
            mp3enc_VLCTableSizes[i],
            (IppsVLCEncodeSpec_32s*)(htables[i].phuftable));
        }
      }
    }

    if (htables) {
      for (i = 17; i < 24; i++) {
        htables[i].phuftable = htables[16].phuftable;
      }
    }

    ippsVLCEncodeGetSize_32s(mp3enc_VLCBooks[24],
      mp3enc_VLCTableSizes[24], &ts);
    size += ts;
    if (mem) {
      htables[i].phuftable = (IppsVLCEncodeSpec_32s *)mem;
      mem += ts;
      ippsVLCEncodeInit_32s(mp3enc_VLCBooks[24],
        mp3enc_VLCTableSizes[24],
        (IppsVLCEncodeSpec_32s*)(htables[24].phuftable));
    }

    if (htables) {
      for (i = 25; i < 32; i++) {
        htables[i].phuftable = htables[24].phuftable;
      }
    }

    if(size_all)
      *size_all = size;

    return 1;
}

#endif //UMC_ENABLE_XXX
