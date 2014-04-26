/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, block coding
//
*/

#ifndef _ENCODER_VC1_BLOCK_H_
#define _ENCODER_VC1_BLOCK_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_structures.h"

namespace UMC_VC1_ENCODER
{

    typedef UMC::Status        (*fWriteBlockInterSM) (VC1EncoderBitStreamSM* pCodedBlock,
                                                     Ipp16s*                pBlock,
                                                     Ipp32u                 blockStep,
                                                     const Ipp8u*           pScanMatrix,
                                                     const sACTablesSet*    pACTablesSet,
                                                     sACEscInfo*            pACEscInfo,
                                                     Ipp32u                 pattern);
    typedef UMC::Status        (*fWriteBlockInterAdv)(VC1EncoderBitStreamAdv*pCodedBlock,
                                                     Ipp16s*                pBlock,
                                                     Ipp32u                 blockStep,
                                                     const Ipp8u*           pScanMatrix,
                                                     const sACTablesSet*    pACTablesSet,
                                                     sACEscInfo*            pACEscInfo,
                                                     Ipp32u                 pattern);

     //typedef UMC::Status        (*fWriteDCSM)         (Ipp16s                  DC,
     //                                                const Ipp32u*           pEncTable,
     //                                                VC1EncoderBitStreamSM*  pCodedBlock);

     //typedef UMC::Status       (*fWriteDCAdv)        (Ipp16s                  DC,
     //                                                const Ipp32u*           pEncTable,
     //                                                VC1EncoderBitStreamAdv* pCodedBlock);

     //extern fWriteBlockInterSM  pWriteBlockInterSM[4];
     //extern fWriteBlockInterAdv pWriteBlockInterAdv[4];
     //extern fWriteDCSM          pWriteDCSM[3];
     //extern fWriteDCAdv         pWriteDCAdv[3];

}
#endif
