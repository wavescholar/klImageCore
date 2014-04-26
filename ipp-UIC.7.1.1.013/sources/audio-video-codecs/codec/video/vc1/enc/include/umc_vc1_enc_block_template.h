/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, block functionality templates
//
*/

#ifndef _ENCODER_VC1_BLOCK_T_H_
#define _ENCODER_VC1_BLOCK_T_H_

#include "umc_vc1_enc_bitstream.h"
#include "umc_vc1_enc_def.h"
#include "umc_structures.h"

namespace UMC_VC1_ENCODER
{
//template <class T>  UMC::Status   WriteBlockACIntra         (T*                     pCodedBlock,
//                                                            Ipp16s*                 pBlock,
//                                                            Ipp32u                  blockStep,
//                                                            const Ipp8u*            pScanMatrix,
//                                                            const sACTablesSet*     pACTablesSet,
//                                                            sACEscInfo*             pACEscInfo)
//   {    UMC::Status     err          = UMC::UMC_OK;
//        Ipp32s          i            = 0;
//
//        Ipp8u           nPairs       = 0;
//        Ipp16s          levels[65];
//        Ipp8u           runs[65];
//
//        const Ipp8u     *pTableDR    = pACTablesSet->pTableDR;
//        const Ipp8u     *pTableDL    = pACTablesSet->pTableDL;
//        const Ipp8u     *pTableInd   = pACTablesSet->pTableInd;
//        const Ipp32u    *pEncTable   = pACTablesSet->pEncTable;
//
//
//        runs[0] = 0;
//        //prepare runs, levels arrays
//        for (i = 1; i<64; i++)
//        {
//            Ipp32s  pos    = pScanMatrix[i];
//            Ipp16s  value = *((Ipp16s*)((Ipp8u*)pBlock + blockStep*(pos>>3)) + (pos&0x07));
//            if (!value)
//            {
//                runs[nPairs]++;
//            }
//            else
//            {
//                levels[nPairs++] = value;
//                runs[nPairs]   = 0;
//            }
//        }
//        // put codes into bitstream
//        i = 0;
//        for (Ipp32s not_last = 1; not_last>=0; not_last--)
//        {
//            for (; i < nPairs - not_last; i++)
//            {
//                bool    sign = false;
//                Ipp8u   run  = runs  [i];
//                Ipp16s  lev  = levels[i];
//
//                Ipp8u mode = GetMode( run, lev, pTableDR, pTableDL, sign);
//
//                switch (mode)
//                {
//                    case 3:
//                        err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(0,2);                       //mode
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(!not_last,1);               //last
//                        VC1_ENC_CHECK (err)
//                        if ((!pACEscInfo->levelSize) && (!pACEscInfo->runSize))
//                        {
//                            pACEscInfo->runSize = 6;
//                            pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*11 +1]==0)? 8:11;
//                            pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize + 1]<=0) ?
//                                                            2 : pACEscInfo->levelSize;
//                            err = pCodedBlock->PutBits(pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize],
//                                                       pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize +1]);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(pACEscInfo->runSize - 3, 2);
//                            VC1_ENC_CHECK (err)
//                        }
//                        err = pCodedBlock->PutBits(run,pACEscInfo->runSize);
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(sign, 1);
//                        VC1_ENC_CHECK (err)
//                        if (lev>((1<<pACEscInfo->levelSize)-1))
//                        {
//                            lev =(1<<pACEscInfo->levelSize)-1;
//                            levels[i] = lev;
//                        }
//                        err = pCodedBlock->PutBits(lev,pACEscInfo->levelSize);
//                        VC1_ENC_CHECK (err)
//                        break;
//                    case 2:
//                    case 1:
//                        err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(1,mode);                     //mode
//                        VC1_ENC_CHECK (err)
//                    case 0:
//                        Ipp16s index = pTableInd[run] + lev;
//                        err = pCodedBlock->PutBits(pEncTable[2*index], pEncTable[2*index + 1]);
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(sign, 1);
//                        VC1_ENC_CHECK (err)
//
//                        break;
//
//                }
//             }
//
//            pTableDR    = pACTablesSet->pTableDRLast;
//            pTableDL    = pACTablesSet->pTableDLLast;
//            pTableInd   = pACTablesSet->pTableIndLast;
//
//        }
//        return UMC::UMC_OK;
//   }

//   template <class T>  UMC::Status   WriteBlockInter8x8type(T*                      pCodedBlock,
//                                                            Ipp16s*                 pBlock,
//                                                            Ipp32u                  blockStep,
//                                                            const Ipp8u*            pScanMatrix,
//                                                            const sACTablesSet*     pACTablesSet,
//                                                            sACEscInfo*             pACEscInfo,
//                                                            Ipp32u                  pattern)
//   {    UMC::Status     err          = UMC::UMC_OK;
//        Ipp32s          i            = 0;
//
//        Ipp8u           nPairs       = 0;
//        Ipp16s          levels[65];
//        Ipp8u           runs[65];
//
//        const Ipp8u     *pTableDR    = pACTablesSet->pTableDR;
//        const Ipp8u     *pTableDL    = pACTablesSet->pTableDL;
//        const Ipp8u     *pTableInd   = pACTablesSet->pTableInd;
//        const Ipp32u    *pEncTable   = pACTablesSet->pEncTable;
//
//
//        runs[0] = 0;
//        //prepare runs, levels arrays
//        for (i = 0; i<64; i++)
//        {
//            Ipp32s  pos    = pScanMatrix[i];
//            Ipp16s  value = *((Ipp16s*)((Ipp8u*)pBlock + blockStep*(pos>>3)) + (pos&0x07));
//            if (!value)
//            {
//                runs[nPairs]++;
//            }
//            else
//            {
//                levels[nPairs++] = value;
//                runs[nPairs]   = 0;
//            }
//        }
//        // put codes into bitstream
//        i = 0;
//        for (Ipp32s not_last = 1; not_last>=0; not_last--)
//        {
//            for (; i < nPairs - not_last; i++)
//            {
//                bool    sign = false;
//                Ipp8u   run  = runs  [i];
//                Ipp16s  lev  = levels[i];
//
//                Ipp8u mode = GetMode( run, lev, pTableDR, pTableDL, sign);
//
//                switch (mode)
//                {
//                    case 3:
//                        err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(0,2);                       //mode
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(!not_last,1);               //last
//                        VC1_ENC_CHECK (err)
//                        if ((!pACEscInfo->levelSize) && (!pACEscInfo->runSize))
//                        {
//                            pACEscInfo->runSize = 6;
//                            pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*11 +1]==0)? 8:11;
//                            pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize + 1]<=0) ?
//                                                            2 : pACEscInfo->levelSize;
//                            err = pCodedBlock->PutBits(pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize],
//                                                       pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize +1]);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(pACEscInfo->runSize - 3, 2);
//                            VC1_ENC_CHECK (err)
//                        }
//                        err = pCodedBlock->PutBits(run,pACEscInfo->runSize);
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(sign, 1);
//                        VC1_ENC_CHECK (err)
//                        if (lev>((1<<pACEscInfo->levelSize)-1))
//                        {
//                            lev =(1<<pACEscInfo->levelSize)-1;
//                            levels[i] = lev;
//                        }
//                        err = pCodedBlock->PutBits(lev,pACEscInfo->levelSize);
//                        VC1_ENC_CHECK (err)
//                        break;
//                    case 2:
//                    case 1:
//                        err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(1,mode);                     //mode
//                        VC1_ENC_CHECK (err)
//                    case 0:
//                        Ipp16s index = pTableInd[run] + lev;
//                        err = pCodedBlock->PutBits(pEncTable[2*index], pEncTable[2*index + 1]);
//                        VC1_ENC_CHECK (err)
//                        err = pCodedBlock->PutBits(sign, 1);
//                        VC1_ENC_CHECK (err)
//                        break;
//
//                }
//             }
//
//            pTableDR    = pACTablesSet->pTableDRLast;
//            pTableDL    = pACTablesSet->pTableDLLast;
//            pTableInd   = pACTablesSet->pTableIndLast;
//
//        }
//        return UMC::UMC_OK;
//   }
//
//   template <class T>  UMC::Status   WriteBlockInter8x4type(T*                      pCodedBlock,
//                                                            Ipp16s*                 pBlock,
//                                                            Ipp32u                  blockStep,
//                                                            const Ipp8u*            pScanMatrix,
//                                                            const sACTablesSet*     pACTablesSet,
//                                                            sACEscInfo*             pACEscInfo,
//                                                            Ipp32u                  pattern)
//   {    UMC::Status     err          = UMC::UMC_OK;
//        Ipp32s          i            = 0;
//
//        Ipp8u           nPairs       = 0;
//        Ipp16s          levels[33];
//        Ipp8u           runs[33];
//        const Ipp32u    *pEncTable   = pACTablesSet->pEncTable;
//        Ipp8u           patMask[2] = {0x0C, 0x03};
//
//        for (Ipp32u nSubblock=0; nSubblock<2; nSubblock++)
//        {
//            const Ipp8u     *pTableDR    = pACTablesSet->pTableDR;
//            const Ipp8u     *pTableDL    = pACTablesSet->pTableDL;
//            const Ipp8u     *pTableInd   = pACTablesSet->pTableInd;
//            Ipp16s*          pSubBlock   = (Ipp16s*)((Ipp8u*)pBlock + blockStep*nSubblock*4);
//
//            if ((pattern & patMask[nSubblock])==0)
//                continue;
//
//            nPairs       = 0;
//            runs[0] = 0;
//            //prepare runs, levels arrays
//            for (i = 0; i<32; i++)
//            {
//                Ipp32s  pos    = pScanMatrix[i];
//                Ipp16s  value = *((Ipp16s*)((Ipp8u*)pSubBlock + blockStep*(pos>>3)) + (pos&0x07));
//                if (!value)
//                {
//                    runs[nPairs]++;
//                }
//                else
//                {
//                    levels[nPairs++] = value;
//                    runs[nPairs]   = 0;
//                }
//            }
//            // put codes into bitstream
//            i = 0;
//            for (Ipp32s not_last = 1; not_last>=0; not_last--)
//            {
//                for (; i < nPairs - not_last; i++)
//                {
//                    bool    sign = false;
//                    Ipp8u   run  = runs  [i];
//                    Ipp16s  lev  = levels[i];
//
//                    Ipp8u mode = GetMode( run, lev, pTableDR, pTableDL, sign);
//
//                    switch (mode)
//                    {
//                        case 3:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(0,2);                       //mode
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(!not_last,1);               //last
//                            VC1_ENC_CHECK (err)
//                            if ((!pACEscInfo->levelSize) && (!pACEscInfo->runSize))
//                            {
//                                pACEscInfo->runSize = 6;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*11 +1]==0)? 8:11;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize + 1]<=0) ?
//                                                                2 : pACEscInfo->levelSize;
//                                err = pCodedBlock->PutBits(pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize],
//                                                        pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize +1]);
//                                VC1_ENC_CHECK (err)
//                                err = pCodedBlock->PutBits(pACEscInfo->runSize - 3, 2);
//                                VC1_ENC_CHECK (err)
//                            }
//                            err = pCodedBlock->PutBits(run,pACEscInfo->runSize);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            if (lev>((1<<pACEscInfo->levelSize)-1))
//                            {
//                                lev =(1<<pACEscInfo->levelSize)-1;
//                                levels[i] = lev;
//                            }
//                            err = pCodedBlock->PutBits(lev,pACEscInfo->levelSize);
//                            VC1_ENC_CHECK (err)
//                            break;
//                        case 2:
//                        case 1:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(1,mode);                     //mode
//                            VC1_ENC_CHECK (err)
//                        case 0:
//                            Ipp16s index = pTableInd[run] + lev;
//                            err = pCodedBlock->PutBits(pEncTable[2*index], pEncTable[2*index + 1]);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            break;
//
//                    }
//                }
//
//                pTableDR    = pACTablesSet->pTableDRLast;
//                pTableDL    = pACTablesSet->pTableDLLast;
//                pTableInd   = pACTablesSet->pTableIndLast;
//
//            }
//        }
//        return UMC::UMC_OK;
//   }
//  template <class T>  UMC::Status   WriteBlockInter4x8type (T*                      pCodedBlock,
//                                                            Ipp16s*                 pBlock,
//                                                            Ipp32u                  blockStep,
//                                                            const Ipp8u*            pScanMatrix,
//                                                            const sACTablesSet*     pACTablesSet,
//                                                            sACEscInfo*             pACEscInfo,
//                                                            Ipp32u                  pattern)
//   {    UMC::Status     err          = UMC::UMC_OK;
//        Ipp32s          i            = 0;
//
//        Ipp8u           nPairs       = 0;
//        Ipp16s          levels[33];
//        Ipp8u           runs[33];
//        Ipp8u           patMask[2]   = {0x0A, 0x05};
//        const Ipp32u    *pEncTable   = pACTablesSet->pEncTable;
//
//        for (Ipp32u nSubblock=0; nSubblock<2; nSubblock++)
//        {
//            const Ipp8u     *pTableDR    = pACTablesSet->pTableDR;
//            const Ipp8u     *pTableDL    = pACTablesSet->pTableDL;
//            const Ipp8u     *pTableInd   = pACTablesSet->pTableInd;
//            Ipp16s*          pSubBlock   = pBlock + 4*nSubblock;
//
//            if ((pattern & patMask[nSubblock])==0)
//                continue;
//
//
//            nPairs       = 0;
//            runs[0] = 0;
//            //prepare runs, levels arrays
//            for (i = 0; i<32; i++)
//            {
//                Ipp32s  pos    = pScanMatrix[i];
//                Ipp16s  value = *((Ipp16s*)((Ipp8u*)pSubBlock + blockStep*(pos>>2)) + (pos&0x03));
//                if (!value)
//                {
//                    runs[nPairs]++;
//                }
//                else
//                {
//                    levels[nPairs++] = value;
//                    runs[nPairs]   = 0;
//                }
//            }
//            // put codes into bitstream
//            i = 0;
//            for (Ipp32s not_last = 1; not_last>=0; not_last--)
//            {
//                for (; i < nPairs - not_last; i++)
//                {
//                    bool    sign = false;
//                    Ipp8u   run  = runs  [i];
//                    Ipp16s  lev  = levels[i];
//
//                    Ipp8u mode = GetMode( run, lev, pTableDR, pTableDL, sign);
//
//                    switch (mode)
//                    {
//                        case 3:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(0,2);                       //mode
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(!not_last,1);               //last
//                            VC1_ENC_CHECK (err)
//                            if ((!pACEscInfo->levelSize) && (!pACEscInfo->runSize))
//                            {
//                                pACEscInfo->runSize = 6;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*11 +1]==0)? 8:11;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize + 1]<=0) ?
//                                                                2 : pACEscInfo->levelSize;
//                                err = pCodedBlock->PutBits(pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize],
//                                                        pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize +1]);
//                                VC1_ENC_CHECK (err)
//                                err = pCodedBlock->PutBits(pACEscInfo->runSize - 3, 2);
//                                VC1_ENC_CHECK (err)
//                            }
//                            err = pCodedBlock->PutBits(run,pACEscInfo->runSize);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            if (lev>((1<<pACEscInfo->levelSize)-1))
//                            {
//                                lev =(1<<pACEscInfo->levelSize)-1;
//                                levels[i] = lev;
//                            }
//                            err = pCodedBlock->PutBits(lev,pACEscInfo->levelSize);
//                            VC1_ENC_CHECK (err)
//                            break;
//                        case 2:
//                        case 1:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(1,mode);                     //mode
//                            VC1_ENC_CHECK (err)
//                        case 0:
//                            Ipp16s index = pTableInd[run] + lev;
//                            err = pCodedBlock->PutBits(pEncTable[2*index], pEncTable[2*index + 1]);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            break;
//
//                    }
//                }
//
//                pTableDR    = pACTablesSet->pTableDRLast;
//                pTableDL    = pACTablesSet->pTableDLLast;
//                pTableInd   = pACTablesSet->pTableIndLast;
//
//            }
//        }
//        return UMC::UMC_OK;
//   }
//
// template <class T>  UMC::Status   WriteBlockInter4x4type  (T*                      pCodedBlock,
//                                                            Ipp16s*                 pBlock,
//                                                            Ipp32u                  blockStep,
//                                                            const Ipp8u*            pScanMatrix,
//                                                            const sACTablesSet*     pACTablesSet,
//                                                            sACEscInfo*             pACEscInfo,
//                                                            Ipp32u                  pattern)
//   {    UMC::Status     err          = UMC::UMC_OK;
//        Ipp32s          i            = 0;
//
//        Ipp8u           nPairs       = 0;
//        Ipp16s          levels[17];
//        Ipp8u           runs[17];
//        const Ipp32u    *pEncTable   = pACTablesSet->pEncTable;
//
//
//        for (Ipp32u nSubblock=0; nSubblock<4; nSubblock++)
//        {
//            const Ipp8u     *pTableDR    = pACTablesSet->pTableDR;
//            const Ipp8u     *pTableDL    = pACTablesSet->pTableDL;
//            const Ipp8u     *pTableInd   = pACTablesSet->pTableInd;
//            Ipp16s*          pSubBlock   = (Ipp16s*)((Ipp8u*)pBlock + blockStep*4*(nSubblock>>1))+ 4*(nSubblock&1);
//
//            if ((pattern & (1<<(3-nSubblock)))==0)
//                continue;
//
//            nPairs       = 0;
//            runs[0] = 0;
//            //prepare runs, levels arrays
//            for (i = 0; i<16; i++)
//            {
//                Ipp32s  pos    = pScanMatrix[i];
//                Ipp16s  value = *((Ipp16s*)((Ipp8u*)pSubBlock + blockStep*(pos>>2)) + (pos&0x03));
//                if (!value)
//                {
//                    runs[nPairs]++;
//                }
//                else
//                {
//                    levels[nPairs++] = value;
//                    runs[nPairs]   = 0;
//                }
//            }
//            // put codes into bitstream
//            i = 0;
//            for (Ipp32s not_last = 1; not_last>=0; not_last--)
//            {
//                for (; i < nPairs - not_last; i++)
//                {
//                    bool    sign = false;
//                    Ipp8u   run  = runs  [i];
//                    Ipp16s  lev  = levels[i];
//
//                    Ipp8u mode = GetMode( run, lev, pTableDR, pTableDL, sign);
//
//                    switch (mode)
//                    {
//                        case 3:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(0,2);                       //mode
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(!not_last,1);               //last
//                            VC1_ENC_CHECK (err)
//                            if ((!pACEscInfo->levelSize) && (!pACEscInfo->runSize))
//                            {
//                                pACEscInfo->runSize = 6;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*11 +1]==0)? 8:11;
//                                pACEscInfo->levelSize = (pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize + 1]<=0) ?
//                                                                2 : pACEscInfo->levelSize;
//                                err = pCodedBlock->PutBits(pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize],
//                                                        pACEscInfo->pLSizeTable[2*pACEscInfo->levelSize +1]);
//                                VC1_ENC_CHECK (err)
//                                err = pCodedBlock->PutBits(pACEscInfo->runSize - 3, 2);
//                                VC1_ENC_CHECK (err)
//                            }
//                            err = pCodedBlock->PutBits(run,pACEscInfo->runSize);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            if (lev>((1<<pACEscInfo->levelSize)-1))
//                            {
//                                lev =(1<<pACEscInfo->levelSize)-1;
//                                levels[i] = lev;
//                            }
//                            err = pCodedBlock->PutBits(lev,pACEscInfo->levelSize);
//                            VC1_ENC_CHECK (err)
//                            break;
//                        case 2:
//                        case 1:
//                            err = pCodedBlock->PutBits(pEncTable[0], pEncTable[1]); //ESCAPE
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(1,mode);                     //mode
//                            VC1_ENC_CHECK (err)
//                        case 0:
//                            Ipp16s index = pTableInd[run] + lev;
//                            err = pCodedBlock->PutBits(pEncTable[2*index], pEncTable[2*index + 1]);
//                            VC1_ENC_CHECK (err)
//                            err = pCodedBlock->PutBits(sign, 1);
//                            VC1_ENC_CHECK (err)
//                            break;
//
//                    }
//                }
//
//                pTableDR    = pACTablesSet->pTableDRLast;
//                pTableDL    = pACTablesSet->pTableDLLast;
//                pTableInd   = pACTablesSet->pTableIndLast;
//
//            }
//        }
//        return UMC::UMC_OK;
//   }
//
//
   template <class T>
   UMC::Status   WriteDCQuantOther (Ipp16s                  DC,
                                    const Ipp32u*           pEncTable,
                                    T*                      pCodedBlock)
   {
        bool            sign        = (DC<0)? 1:0;
        UMC::Status     err         = UMC::UMC_OK;

        DC         = (sign)? -DC : DC;
        if (!DC )
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC],pEncTable[2*DC+1]);
            VC1_ENC_CHECK (err)
        }
        else if (DC < VC1_ENC_DC_ESCAPE_INDEX)
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC],pEncTable[2*DC+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)
        }
        else
        {
            err = pCodedBlock->PutBits(pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX],pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(DC, 8);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)
        }
        return UMC::UMC_OK;
   }

   template <class T>
   UMC::Status       WriteDCQuant1     (Ipp16s                  DC,
                                        const Ipp32u*           pEncTable,
                                        T*                      pCodedBlock)
   {
        bool    sign       = (DC<0)? 1:0;

        Ipp16s      DC1        = 0;
        Ipp16s      DC2        = 0;
        UMC::Status err        = UMC::UMC_OK;

        DC = (sign)? -DC : DC;

        DC1 = (DC+3)>>2;
        DC2 = (DC+3)&3 ;

        if (!DC)
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC],pEncTable[2*DC+1]);
            VC1_ENC_CHECK (err)
        }
        else if (DC1 < VC1_ENC_DC_ESCAPE_INDEX)
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC1],pEncTable[2*DC1+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(DC2,2);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)
        }
        else
        {
            err = pCodedBlock->PutBits(pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX],pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(DC, 10);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)
        }
        return err;
   }
   template <class T>  inline
   UMC::Status        WriteDCQuant2     (Ipp16s                  DC,
                                         const Ipp32u*           pEncTable,
                                         T*                      pCodedBlock)
   {
        bool        sign       = (DC<0)? 1:0;
        Ipp16s      DC1        = 0;
        Ipp16s      DC2        = 0;
        UMC::Status err        = UMC::UMC_OK;

        DC         = (sign)? -DC : DC;

        DC1 = (DC+1)>>1;
        DC2 = (DC+1)&1 ;

        if (!DC)
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC],pEncTable[2*DC+1]);
            VC1_ENC_CHECK (err)
        }
        else if (DC1 < VC1_ENC_DC_ESCAPE_INDEX)
        {
            err = pCodedBlock->PutBits(pEncTable[2*DC1],pEncTable[2*DC1+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(DC2,1);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)

        }
        else
        {
            err = pCodedBlock->PutBits(pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX],pEncTable[2*VC1_ENC_DC_ESCAPE_INDEX+1]);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(DC, 9);
            VC1_ENC_CHECK (err)
            err = pCodedBlock->PutBits(sign,1);
            VC1_ENC_CHECK (err)
        }
        return UMC::UMC_OK;
   }





     /*
   template <class T>  inline
   UMC::Status         VC1EncoderBlock::WriteBlockDC      (  Ipp8u                   Quant,
                                                                                const Ipp32u*           pEncTable,
                                                                                T*                      pCodedBlock)
   {
       switch (Quant)
       {
       case 1:
            return WriteDCQuant1     (pEncTable,pCodedBlock);
       case 2:
            return WriteDCQuant2     (pEncTable,pCodedBlock);
       default:
            return WriteDCQuantOther (pEncTable,pCodedBlock);
       }
   }
*/
};

#endif
