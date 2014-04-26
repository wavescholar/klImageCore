/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class VideoEncoderMPEG4 (motion estimation)
//
//  References:
//    Fast ME algorithm
//        IEEE Transactions on image processing, vol. 9, No. 2, Feb 2000
//        "A new Diamond Search Algorithm for Fast Block-Matching Motion Estimation"
//        Shan Zhu and Kai-Kuang Ma
//    Fast half-pel algorithm
//        V. Bhaskaran, K. Konstantinides
//        "Image And Video Compression Standarts"
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include "mp4_enc.h"

namespace MPEG4_ENC
{

inline void mp4_ComputeChromaMV(Ipp32s dx, Ipp32s dy, Ipp32s *cx, Ipp32s *cy)
{
    *cx = mp4_Div2Round(dx);
    *cy = mp4_Div2Round(dy);
}

inline void mp4_ComputeChromaMVQ(Ipp32s dx, Ipp32s dy, Ipp32s *cx, Ipp32s *cy)
{
    dx = mp4_Div2(dx);
    dy = mp4_Div2(dy);
    *cx = (Ipp16s)mp4_Div2Round(dx);
    *cy = (Ipp16s)mp4_Div2Round(dy);
}


inline void mp4_ComputeChromaMV(const IppMotionVector *mvLuma, IppMotionVector *mvChroma)
{
    mvChroma->dx = (Ipp16s)mp4_Div2Round(mvLuma->dx);
    mvChroma->dy = (Ipp16s)mp4_Div2Round(mvLuma->dy);
}


inline void mp4_ComputeChromaMVQ(const IppMotionVector *mvLuma, IppMotionVector *mvChroma)
{
    int  dx, dy;

    dx = mp4_Div2(mvLuma->dx);
    dy = mp4_Div2(mvLuma->dy);
    mvChroma->dx = (Ipp16s)mp4_Div2Round(dx);
    mvChroma->dy = (Ipp16s)mp4_Div2Round(dy);
}


static void mp4_ME_Int_SAD(mp4_Data_ME *meData)
{
    ippSAD_func sadFunc;
    Ipp32s  cSAD, bSAD, threshSAD;
    Ipp8u   *pRef, *pCur;
    int      step, xPos, yPos, xL, xR, yT, yB, quant, fcode, qpel, usemv, chroma;
    IppMotionVector mvPred;

    xPos = meData->xPos;
    yPos = meData->yPos;
    bSAD = meData->bestDiff;
    pCur = meData->pYc;
    pRef = meData->pYr;
    step = meData->stepL;
    threshSAD = meData->thrDiff;
    sadFunc = meData->sadFunc;
    mvPred = meData->mvPred[0];
    xL = meData->xL;
    xR = meData->xR;
    yT = meData->yT;
    yB = meData->yB;
    quant = meData->quant;
    fcode = meData->fcode;
    qpel = meData->flags & ME_QP;
    usemv = meData->flags & ME_USE_MVWEIGHT;
    chroma = meData->flags & ME_CHROMA;
    switch (meData->method) {
    case ME_SEARCH_FULL_RECT :
        {
            int  i, j;

            pRef += yT * step;
            for (i = yT; i <= yB; i ++) {
                for (j = xL; j <= xR; j ++) {
                    sadFunc(pCur, step, pRef + j, step, &cSAD, IPPVC_MC_APX_FF);
                    if (usemv)
                        cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                    if (chroma) {
                        Ipp32s sadC;
                        ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                        cSAD += sadC;
                        ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                        cSAD += sadC;
                    }
                    if (cSAD < bSAD) {
                        bSAD = cSAD;
                        yPos = i;
                        xPos = j;
                        if (cSAD <= threshSAD)
                            goto L_01;
                    }
                }
                pRef += step;
            }
        }
        break;
    case ME_SEARCH_FULL_INVOLUTE :
        {
            int  i, j, nLoop, cLoop, lPos;
            i = yPos;
            j = xPos;
            pRef += yPos * step + xPos;
            nLoop = IPP_MAX(IPP_MAX(xPos - xL, xR - xPos), IPP_MAX(yPos - yT, yB - yPos));
            for (cLoop = 1; cLoop <= nLoop; cLoop ++) {
                i --;
                j --;
                pRef -= step + 1;
                for (lPos = 0; lPos < (cLoop << 3); lPos ++) {
                    if (j >= xL && j <= xR && i >= yT && i <= yB) {
                        sadFunc(pCur, step, pRef, step, &cSAD, IPPVC_MC_APX_FF);
                        if (usemv)
                            cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                        if (chroma) {
                            Ipp32s sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                        }
                        if (cSAD < bSAD) {
                            bSAD = cSAD;
                            yPos = i;
                            xPos = j;
                            if (cSAD <= threshSAD)
                                goto L_01;
                        }
                    }
                    if (lPos < (cLoop * 2)) {
                        j ++;  pRef ++;
                    } else if (lPos < (cLoop * 4)) {
                        i ++;  pRef += step;
                    } else if (lPos < (cLoop * 6)) {
                        j --;  pRef --;
                    } else {
                        i --;  pRef -= step;
                    }
                }
            }
        }
        break;
    case ME_SEARCH_DIAMOND_BIG :
        {
            static const Ipp32s bdJ[9] = {0, -2, -1, 0, 1, 2, 1, 0, -1}, bdI[9] = {0, 0, -1, -2, -1, 0, 1, 2, 1}, sdJ[5] = {0, -1, 0, 1, 0}, sdI[5] = {0, 0, -1, 0, 1};
            static const Ipp32s bdN[9] = {8, 5, 3, 5, 3, 5, 3, 5, 3}, bdA[9][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 7, 8, 0, 0, 0}, {1, 2, 3, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 0, 0, 0}, {3, 4, 5, 0, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 0, 0, 0}, {5, 6, 7, 0, 0, 0, 0, 0}, {1, 5, 6, 7, 8, 0, 0, 0}, {1, 7, 8, 0, 0, 0, 0, 0}};
#ifdef USE_ME_SADBUFF
            Ipp32s  *sadBuf, swWidth, swHeight;
#endif
            int      i, j, k, l, m, n;

#ifdef USE_ME_SADBUFF
            sadBuf = meData->meBuff;
            swWidth = (xR - xL + 1);
            swHeight = (yB - yT + 1);
            ippsSet_8u((Ipp8u)-1, (Ipp8u*)sadBuf, swWidth * swHeight * 4);
            sadBuf[(yPos-yT)*swWidth+xPos-xL] = bSAD;
#endif
            l = 0;
            for (;;) {
                // find SAD at big diamond
                n = l;
                l = 0;
                for (m = 0; m < bdN[n]; m ++) {
                    k = bdA[n][m];
                    j = xPos + bdJ[k];
                    i = yPos + bdI[k];
                    if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
                        if (sadBuf[(i-yT)*swWidth+j-xL] == -1) {
#endif
                            sadFunc(pCur, step, pRef+i*step+j, step, &cSAD, IPPVC_MC_APX_FF);
                            if (usemv)
                                cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                            if (chroma) {
                                Ipp32s sadC;
                                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                                cSAD += sadC;
                                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                                cSAD += sadC;
                            }
                            if (cSAD <= threshSAD) {
                                xPos = j;
                                yPos = i;
                                bSAD = cSAD;
                                goto L_01;
                            }
#ifdef USE_ME_SADBUFF
                            sadBuf[(i-yT)*swWidth+j-xL] = cSAD;
#endif
                            if (cSAD < bSAD) {
                                l = k;
                                bSAD = cSAD;
                            }
#ifdef USE_ME_SADBUFF
                        }
#endif
                    }
                }
                if (l == 0) {
                    // find SAD at small diamond
                    for (k = 1; k <= 4; k ++) {
                        j = xPos + sdJ[k];
                        i = yPos + sdI[k];
                        if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
                            if (sadBuf[(i-yT)*swWidth+j-xL] == -1) {
#endif
                                sadFunc(pCur, step, pRef+i*step+j, step, &cSAD, IPPVC_MC_APX_FF);
                                if (usemv)
                                    cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                                if (chroma) {
                                    Ipp32s sadC;
                                    ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                                    cSAD += sadC;
                                    ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                                    cSAD += sadC;
                                }
                                if (cSAD <= threshSAD) {
                                    xPos = j;
                                    yPos = i;
                                    bSAD = cSAD;
                                    goto L_01;
                                }
#ifdef USE_ME_SADBUFF
                                sadBuf[(i-yT)*swWidth+j-xL]= cSAD;
#endif
                                if (cSAD < bSAD) {
                                    l = k;
                                    bSAD = cSAD;
                                }
#ifdef USE_ME_SADBUFF
                            }
#endif
                        }
                    }
                    xPos += sdJ[l];
                    yPos += sdI[l];
                    goto L_01;
                }
                yPos += bdI[l];
                xPos += bdJ[l];
            }
        }
        break;
    case ME_SEARCH_SQUARE :
        {
            static const Ipp32s bdJ[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1}, bdI[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};
            static const Ipp32s bdN[9] = {8, 5, 3, 5, 3, 5, 3, 5, 3}, bdA[9][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 7, 8, 0, 0, 0}, {1, 2, 3, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 0, 0, 0}, {3, 4, 5, 0, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 0, 0, 0}, {5, 6, 7, 0, 0, 0, 0, 0}, {5, 6, 7, 8, 1, 0, 0, 0}, {7, 8, 1, 0, 0, 0, 0, 0}};
            int      i, j, k, l, m, n;

            l = 0;
            for (;;) {
                n = l;
                l = 0;
                for (m = 0; m < bdN[n]; m ++) {
                    k = bdA[n][m];
                    j = xPos + bdJ[k];
                    i = yPos + bdI[k];
                    if (j >= xL && j <= xR && i >= yT && i <= yB) {
                        sadFunc(pCur, step, pRef+i*step+j, step, &cSAD, IPPVC_MC_APX_FF);
                        if (usemv)
                            cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                        if (chroma) {
                            Ipp32s sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                        }
                        if (cSAD <= threshSAD) {
                            xPos = j;
                            yPos = i;
                            bSAD = cSAD;
                            goto L_01;
                        }
                        if (cSAD < bSAD) {
                            l = k;
                            bSAD = cSAD;
                        }
                    }
                }
                if (l == 0)
                    break;
                xPos += bdJ[l];
                yPos += bdI[l];
            }
        }
        break;
    case ME_SEARCH_DIAMOND_SMALL :
        {
            static const Ipp32s bdJ[5] = {0, -1, 0, 1, 0}, bdI[5] = {0, 0, -1, 0, 1};
            static const Ipp32s bdN[5] = {4, 3, 3, 3, 3}, bdA[5][4] = {{1, 2, 3, 4}, {1, 2, 4, 0}, {1, 2, 3, 0}, {2, 3, 4, 0}, {3, 4, 1, 0}};
            int      i, j, k, l, m, n;

            l = 0;
            for (;;) {
                n = l;
                l = 0;
                for (m = 0; m < bdN[n]; m ++) {
                    k = bdA[n][m];
                    j = xPos + bdJ[k];
                    i = yPos + bdI[k];
                    if (j >= xL && j <= xR && i >= yT && i <= yB) {
                        sadFunc(pCur, step, pRef+i*step+j, step, &cSAD, IPPVC_MC_APX_FF);
                        if (usemv)
                            cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                        if (chroma) {
                            Ipp32s sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                        }
                        if (cSAD <= threshSAD) {
                            xPos = j;
                            yPos = i;
                            bSAD = cSAD;
                            goto L_01;
                        }
                        if (cSAD < bSAD) {
                            l = k;
                            bSAD = cSAD;
                        }
                    }
                }
                if (l == 0)
                    break;
                xPos += bdJ[l];
                yPos += bdI[l];
            }
        }
        break;
    case ME_SEARCH_LOG :
        {
            static const Ipp32s bdJ[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1}, bdI[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};
            static const Ipp32s bdN[9] = {8, 5, 3, 5, 3, 5, 3, 5, 3}, bdA[9][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 7, 8, 0, 0, 0}, {1, 2, 3, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 0, 0, 0}, {3, 4, 5, 0, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 0, 0, 0}, {5, 6, 7, 0, 0, 0, 0, 0}, {5, 6, 7, 8, 1, 0, 0, 0}, {7, 8, 1, 0, 0, 0, 0, 0}};
            int      i, j, k, l, m, n, rX, rY;

            rX = IPP_MIN(xPos - xL, xR - xPos);
            rY = IPP_MIN(yPos - yT, yB - yPos);
            if (rX <= 0)
                rX = 1;
            if (rY <= 0)
                rY = 1;
            l = 0;
            for (;;) {
                n = l;
                l = 0;
                for (m = 0; m < bdN[n]; m ++) {
                    k = bdA[n][m];
                    j = xPos + bdJ[k] * rX;
                    i = yPos + bdI[k] * rY;
                    if (j >= xL && j <= xR && i >= yT && i <= yB) {
                        sadFunc(pCur, step, pRef+i*step+j, step, &cSAD, IPPVC_MC_APX_FF);
                        if (usemv)
                            cSAD += mp4_WeightMV(j + j, i + i, mvPred, fcode, quant, qpel);
                        if (chroma) {
                            Ipp32s sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                            ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(j, i, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(j, i));
                            cSAD += sadC;
                        }
                        if (cSAD <= threshSAD) {
                            xPos = j;
                            yPos = i;
                            bSAD = cSAD;
                            goto L_01;
                        }
                        if (cSAD < bSAD) {
                            l = k;
                            bSAD = cSAD;
                        }
                    }
                }
                if (l == 0) {
                    rX >>= 1;
                    rY >>= 1;
                    if (rX == 0 || rY == 0) {
                        break;
                    }
                } else {
                    xPos += bdJ[l] * rX;
                    yPos += bdI[l] * rY;
                }
            }
        }
        break;
    default :
        break;
    }
L_01:
    meData->bestDiff = bSAD;
    meData->xPos = xPos;
    meData->yPos = yPos;
}


static void mp4_ME_HalfPel_SAD(mp4_Data_ME *meData)
{
    __ALIGN16(Ipp8u, mc, 256);
    ippSAD_func sadFunc;
    ippCopy_func copyFunc;
    Ipp32s  cSAD, bSAD, threshSAD;
    Ipp8u   *pRef, *pCur;
    int      step, xPos, yPos, xL, xR, yT, yB, quant, fcode, qpel, usemv, chroma, rt, xv, yv, bX, bY, k;
    static const Ipp32s hpX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    static const Ipp32s hpY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    IppMotionVector mvPred;

    xPos = meData->xPos * 2;
    yPos = meData->yPos * 2;
    bSAD = meData->bestDiff;
    pCur = meData->pYc;
    pRef = meData->pYr;
    step = meData->stepL;
    threshSAD = meData->thrDiff;
    sadFunc = meData->sadFunc;
    copyFunc = meData->copyHPFunc;
    mvPred = meData->mvPred[0];
    xL = meData->xL * 2;
    xR = meData->xR * 2;
    yT = meData->yT * 2;
    yB = meData->yB * 2;
    quant = meData->quant;
    fcode = meData->fcode;
    qpel = meData->flags & ME_QP;
    usemv = meData->flags & ME_USE_MVWEIGHT;
    chroma = meData->flags & ME_CHROMA;
    rt = meData->rt;
    bX = xPos;
    bY = yPos;
    for (k = 0; k < 8; k ++) {
        xv = xPos + hpX[k];
        yv = yPos + hpY[k];
        if (xv >= xL && xv <= xR && yv >= yT && yv <= yB) {
            if (rt == 0 || !(meData->flags & ME_SUBPEL_FINE))
                sadFunc(pCur, step, pRef+MP4_MV_OFF_HP(xv, yv, step), step, &cSAD, MP4_MV_ACC_HP_SAD(xv, yv));
            else {
                copyFunc(pRef+MP4_MV_OFF_HP(xv, yv, step), step, mc, 16, MP4_MV_ACC_HP(xv, yv), rt);
                sadFunc(pCur, step, mc, 16, &cSAD, IPPVC_MC_APX_FF);
            }
            if (usemv)
                cSAD += mp4_WeightMV(xv, yv, mvPred, fcode, quant, qpel);
            if (chroma) {
                Ipp32s sadC, cx, cy;
                mp4_ComputeChromaMV(xv, yv, &cx, &cy);
                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                cSAD += sadC;
                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                cSAD += sadC;
            }
            if (cSAD < bSAD) {
                bX = xv;
                bY = yv;
                bSAD = cSAD;
                if (bSAD <= threshSAD)
                    break;
            }
        }
    }
    meData->xPos = bX;
    meData->yPos = bY;
    meData->bestDiff = bSAD;
}


static void mp4_ME_HalfPel_SAD_Fast(mp4_Data_ME *meData)
{
    ippSAD_func sadFunc;
    Ipp8u   *pRef, *pCur;
    int      i, j, step, xPos, yPos, xL, xR, yT, yB, m0, m1, m2, m3, m4, quant, fcode, qpel, usemv, chroma;
#ifdef USE_ME_SADBUFF
    Ipp32s  *sadBuf, swWidth;
#endif
    IppMotionVector mvPred;

    xPos = meData->xPos;
    yPos = meData->yPos;
    pCur = meData->pYc;
    pRef = meData->pYr;
    step = meData->stepL;
    sadFunc = meData->sadFunc;
    mvPred = meData->mvPred[0];
    xL = meData->xL;
    xR = meData->xR;
    yT = meData->yT;
    yB = meData->yB;
    quant = meData->quant;
    fcode = meData->fcode;
    qpel = meData->flags & ME_QP;
    usemv = meData->flags & ME_USE_MVWEIGHT;
    chroma = meData->flags & ME_CHROMA;
#ifdef USE_ME_SADBUFF
    sadBuf = meData->meBuff;
    swWidth = (xR - xL + 1);
#endif
    m0 = meData->bestDiff;
    j = xPos;
    i = yPos - 1;
    if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
        m1 = sadBuf[(i-yT)*swWidth+j-xL];
        if (m1 == -1) {
#endif
            sadFunc(pCur, step, pRef+i*step+j, step, &m1, IPPVC_MC_APX_FF);
            if (usemv)
                m1 += mp4_WeightMV(xPos + xPos, yPos + yPos - 2, mvPred, fcode, quant, qpel);
            if (chroma) {
                Ipp32s sadC, cx, cy;
                mp4_ComputeChromaMV(j+j, i+i, &cx, &cy);
                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m1 += sadC;
                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m1 += sadC;
            }
#ifdef USE_ME_SADBUFF
        }
#endif
    } else
        m1 = SAD_MAX;
    j = xPos;
    i = yPos + 1;
    if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
        m2 = sadBuf[(i-yT)*swWidth+j-xL];
        if (m2 == -1) {
#endif
            sadFunc(pCur, step, pRef+i*step+j, step, &m2, IPPVC_MC_APX_FF);
            if (usemv)
                m2 += mp4_WeightMV(xPos + xPos, yPos + yPos + 2, mvPred, fcode, quant, qpel);
            if (chroma) {
                Ipp32s sadC, cx, cy;
                mp4_ComputeChromaMV(j+j, i+i, &cx, &cy);
                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m2 += sadC;
                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m2 += sadC;
            }
#ifdef USE_ME_SADBUFF
        }
#endif
    } else
        m2 = SAD_MAX;
    j = xPos - 1;
    i = yPos;
    if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
        m3 = sadBuf[(i-yT)*swWidth+j-xL];
        if (m3 == -1) {
#endif
            sadFunc(pCur, step, pRef+i*step+j, step, &m3, IPPVC_MC_APX_FF);
            if (usemv)
                m3 += mp4_WeightMV(xPos + xPos - 2, yPos + yPos, mvPred, fcode, quant, qpel);
            if (chroma) {
                Ipp32s sadC, cx, cy;
                mp4_ComputeChromaMV(j+j, i+i, &cx, &cy);
                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m3 += sadC;
                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m3 += sadC;
            }
#ifdef USE_ME_SADBUFF
        }
#endif
    } else
        m3 = SAD_MAX;
    j = xPos + 1;
    i = yPos;
    if (j >= xL && j <= xR && i >= yT && i <= yB) {
#ifdef USE_ME_SADBUFF
        m4 = sadBuf[(i-yT)*swWidth+j-xL];
        if (m4 == -1) {
#endif
            sadFunc(pCur, step, pRef+i*step+j, step, &m4, IPPVC_MC_APX_FF);
            if (usemv)
                m4 += mp4_WeightMV(xPos + xPos + 2, yPos + yPos, mvPred, fcode, quant, qpel);
            if (chroma) {
                Ipp32s sadC, cx, cy;
                mp4_ComputeChromaMV(j+j, i+i, &cx, &cy);
                ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m4 += sadC;
                ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                m4 += sadC;
            }
#ifdef USE_ME_SADBUFF
        }
#endif
    } else
        m4 = SAD_MAX;
    if (2*(m3-m0) < (m4-m0)) {
        j = -1;
    } else if ((m3-m0) > 2*(m4-m0)) {
        j = +1;
    } else
        j = 0;
    if (2*(m1-m0) < (m2-m0)) {
        i = -1;
    } else if ((m1-m0) > 2*(m2-m0)) {
        i = +1;
    } else
        i = 0;
    meData->xPos = (xPos << 1) + j;
    meData->yPos = (yPos << 1) + i;
    if (j != 0 || i != 0) {
        sadFunc(pCur, step, pRef+(meData->yPos >> 1)*step+(meData->xPos >> 1), step, &meData->bestDiff, MP4_MV_ACC_HP_SAD(j, i));
        if (usemv)
            meData->bestDiff += mp4_WeightMV(xPos + xPos + j, yPos + yPos + i, mvPred, fcode, quant, qpel);
        if (chroma) {
            Ipp32s sadC, cx, cy;
            mp4_ComputeChromaMV(meData->xPos, meData->yPos, &cx, &cy);
            ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
            meData->bestDiff += sadC;
            ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
            meData->bestDiff += sadC;
        }
        if (meData->bestDiff > m0) {
            // false choosing
            meData->xPos = (xPos << 1);
            meData->yPos = (yPos << 1);
            meData->bestDiff = m0;
        }
    }
}

static void mp4_ME_QuarterPel_SAD(mp4_Data_ME *meData)
{
    ippSAD_func sadFunc;
    ippCopy_func copyFunc;
    Ipp32s  cSAD, bSAD, threshSAD;
    Ipp8u   *pRef, *pCur;
    int      yv, xv, step, xPos, yPos, xL, xR, yT, yB, k, l, m, n, quant, fcode, usemv, chroma, rt, bX, bY;
    __ALIGN16(Ipp8u, mc, 16*16);
    IppMotionVector mvPred;

    rt = meData->rt;
    xPos = meData->xPos * 2;
    yPos = meData->yPos * 2;
    bSAD = meData->bestDiff;
    pCur = meData->pYc;
    pRef = meData->pYr;
    step = meData->stepL;
    threshSAD = meData->thrDiff;
    sadFunc = meData->sadFunc;
    copyFunc = meData->copyQPFunc;
    mvPred = meData->mvPred[0];
    xL = meData->xL * 4;
    xR = meData->xR * 4;
    yT = meData->yT * 4;
    yB = meData->yB * 4;
    quant = meData->quant;
    fcode = meData->fcode;
    usemv = meData->flags & ME_USE_MVWEIGHT;
    chroma = meData->flags & ME_CHROMA;
    bX = xPos;
    bY = yPos;
    copyFunc(pRef + MP4_MV_OFF_QP(xPos, yPos, step), step, mc, 16, MP4_MV_ACC_QP(xPos, yPos), rt);
    sadFunc(pCur, step, mc, 16, &bSAD, IPPVC_MC_APX_FF);
    if (usemv)
        bSAD += mp4_WeightMV(xPos, yPos, mvPred, fcode, quant, 0);
    if (bSAD <= threshSAD)
        goto L_01;
    if (meData->flags & ME_SUBPEL_FINE) {
//        static const Ipp32s bdJ[5] = {0, -1, 0, 1, 0}, bdI[5] = {0, 0, -1, 0, 1};
//        static const Ipp32s bdN[5] = {4, 3, 3, 3, 3}, bdA[5][4] = {{1, 2, 3, 4}, {1, 2, 4, 0}, {1, 2, 3, 0}, {2, 3, 4, 0}, {3, 4, 1, 0}};
        static const Ipp32s bdJ[9] = {0, -1, 0, 1, 1, 1, 0, -1, -1}, bdI[9] = {0, -1, -1, -1, 0, 1, 1, 1, 0};
        static const Ipp32s bdN[9] = {8, 5, 3, 5, 3, 5, 3, 5, 3}, bdA[9][8] = {{1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 7, 8, 0, 0, 0}, {1, 2, 3, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 0, 0, 0}, {3, 4, 5, 0, 0, 0, 0, 0}, {3, 4, 5, 6, 7, 0, 0, 0}, {5, 6, 7, 0, 0, 0, 0, 0}, {5, 6, 7, 8, 1, 0, 0, 0}, {7, 8, 1, 0, 0, 0, 0, 0}};
        xL = IPP_MAX(xL, xPos - 3);
        xR = IPP_MIN(xR, xPos + 3);
        yT = IPP_MAX(yT, yPos - 3);
        yB = IPP_MIN(yB, yPos + 3);
        l = 0;
        for (;;) {
            n = l;
            l = 0;
            for (m = 0; m < bdN[n]; m ++) {
                k = bdA[n][m];
                xv = bX + bdJ[k];
                yv = bY + bdI[k];
                if (xv >= xL && xv <= xR && yv >= yT && yv <= yB) {
                    copyFunc(pRef + MP4_MV_OFF_QP(xv, yv, step), step, mc, 16, MP4_MV_ACC_QP(xv, yv), rt);
                    sadFunc(pCur, step, mc, 16, &cSAD, IPPVC_MC_APX_FF);
                    if (usemv)
                        cSAD += mp4_WeightMV(xv, yv, mvPred, fcode, quant, 0);
                    if (chroma) {
                        Ipp32s sadC, cx, cy;
                        mp4_ComputeChromaMVQ(xv, yv, &cx, &cy);
                        ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                        cSAD += sadC;
                        ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                        cSAD += sadC;
                    }
                    if (cSAD <= threshSAD) {
                        bX = xv;
                        bY = yv;
                        bSAD = cSAD;
                        goto L_01;
                    }
                    if (cSAD < bSAD) {
                        l = k;
                        bSAD = cSAD;
                    }
                }
            }
            if (l == 0)
                break;
            bX += bdJ[l];
            bY += bdI[l];
        }
    } else {
        static const Ipp32s hpX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        static const Ipp32s hpY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        for (k = 0; k < 8; k ++) {
            xv = xPos + hpX[k];
            yv = yPos + hpY[k];
            if (xv >= xL && xv <= xR && yv >= yT && yv <= yB) {
                copyFunc(pRef + MP4_MV_OFF_QP(xv, yv, step), step, mc, 16, MP4_MV_ACC_QP(xv, yv), rt);
                sadFunc(pCur, step, mc, 16, &cSAD, IPPVC_MC_APX_FF);
                if (usemv)
                    cSAD += mp4_WeightMV(xv, yv, mvPred, fcode, quant, 0);
                if (chroma) {
                    Ipp32s sadC, cx, cy;
                    mp4_ComputeChromaMVQ(xv, yv, &cx, &cy);
                    ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                    cSAD += sadC;
                    ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(cx, cy, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(cx, cy));
                    cSAD += sadC;
                }
                if (cSAD < bSAD) {
                    bX = xv;
                    bY = yv;
                    bSAD = cSAD;
                    if (bSAD <= threshSAD)
                        break;
                }
            }
        }
    }
L_01:
    meData->xPos = bX;
    meData->yPos = bY;
    meData->bestDiff = bSAD;
}


void mp4_ME_SAD(mp4_Data_ME *meData)
{
    Ipp32s  xPos, yPos, sadL, i, j, numPred, xP, yP, xPp[10], yPp[10];
    IppMotionVector mvPred;

    numPred = 0;
    mvPred = meData->mvPred[0];
    xPos = yPos = 0;
    // find best candidate
    for (i = 0; i < meData->numPred; i ++) {
        xP = mp4_Div2(meData->mvPred[i].dx);
        yP = mp4_Div2(meData->mvPred[i].dy);
        if (meData->flags & ME_QP) {
            xP = mp4_Div2(xP);
            yP = mp4_Div2(yP);
        }
        if (xP != 0 || yP != 0) {
            // check init mv is in rect
            if (xP >= meData->xL && xP <= meData->xR && yP >= meData->yT && yP <= meData->yB) {
                for (j = 0; j < numPred; j ++) {
                    if ((xP == xPp[j]) && (yP == yPp[j]))
                        break;
                }
                if (j == numPred) {
                    xPp[numPred] = xP;
                    yPp[numPred] = yP;
                    numPred ++;
                    // find SAD at predicted MV
                    meData->sadFunc(meData->pYc, meData->stepL, meData->pYr+yP*meData->stepL+xP, meData->stepL, &sadL, IPPVC_MC_APX_FF);
                    if (meData->flags & ME_USE_MVWEIGHT)
                        sadL += mp4_WeightMV(xP * 2, yP * 2, mvPred, meData->fcode, meData->quant, meData->flags & ME_QP);
                    if (meData->flags & ME_CHROMA) {
                        Ipp32s sadC;
                        ippiSAD8x8_8u32s_C1R(meData->pUc, meData->stepC, meData->pUr+MP4_MV_OFF_HP(xP, yP, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(xP, yP));
                        sadL += sadC;
                        ippiSAD8x8_8u32s_C1R(meData->pVc, meData->stepC, meData->pVr+MP4_MV_OFF_HP(xP, yP, meData->stepC), meData->stepC, &sadC, MP4_MV_ACC_HP_SAD(xP, yP));
                        sadL += sadC;
                    }
                    if (sadL < meData->bestDiff) {
                        meData->bestDiff = sadL;
                        xPos = xP;
                        yPos = yP;
                    }
                }
            }
        }
    }
    if (meData->bestDiff > meData->thrDiff) {
        meData->xPos = xPos;
        meData->yPos = yPos;
        meData->bestDiff -= SAD_FAVOR_ZERO;
        sadL = meData->bestDiff;
        mp4_ME_Int_SAD(meData);
        if (meData->flags & (ME_HP | ME_QP)) {
            if (meData->flags & ME_HP_FAST)
                mp4_ME_HalfPel_SAD_Fast(meData);
            else
                mp4_ME_HalfPel_SAD(meData);
            if (meData->flags & ME_QP)
                mp4_ME_QuarterPel_SAD(meData);
        } else {
            meData->xPos <<= 1;
            meData->yPos <<= 1;
        }
        if (meData->bestDiff == sadL)
            meData->bestDiff += SAD_FAVOR_ZERO;
    } else {
        if (!(meData->flags & ME_QP)) {
            meData->xPos = xPos + xPos;
            meData->yPos = yPos + yPos;
        } else {
            meData->xPos = xPos * 4;
            meData->yPos = yPos * 4;
        }
    }
}


#if 0
        if (meData->yMB > startRow) { \
            /* left-top MV */ \
            if (meData->xMB > 0) { \
                meData->mvPred[numPred] = meData->MBcurr[-mNumMacroBlockPerRow-1].mv[3]; \
                numPred ++; \
            } \
            /* right-top MV */ \
            if (meData->xMB < mNumMacroBlockPerRow - 1) { \
                meData->mvPred[numPred] = meData->MBcurr[-mNumMacroBlockPerRow+1].mv[2]; \
                numPred ++; \
            } \
        } \

#endif


#define mp4_ME_SetPredictors(mvPredict, mvCurr, mvLeft, mvTop, mvRight, mvBottom, startRow, endRow) \
{ \
    int numPred; \
    /* predicted MV */ \
    meData->mvPred[0] = mvPredict; \
    numPred = 1; \
    if (meData->flags & ME_USE_MANYPRED) { \
        /* curr MV from past frame */ \
        meData->mvPred[1] = mvCurr; \
        numPred = 2; \
        /* left MV */ \
        if (meData->xMB > 0) { \
            meData->mvPred[2] = mvLeft; \
            numPred = 3; \
        } \
        /* top MV */ \
        if (meData->yMB > startRow) { \
            meData->mvPred[numPred] = mvTop; \
            numPred ++; \
        } \
        /* right MV from past frame */ \
        if (meData->xMB < mNumMacroBlockPerRow - 1) { \
            meData->mvPred[numPred] = mvRight; \
            numPred ++; \
        } \
        /* bottom MV from past frame */ \
        if (meData->yMB < endRow) { \
            meData->mvPred[numPred] = mvBottom; \
            numPred ++; \
        } \
        if (meData->yMB > startRow) { \
            /* left-top MV */ \
            if (meData->xMB > 0) { \
                meData->mvPred[numPred] = meData->MBcurr[-mNumMacroBlockPerRow-1].mv[3]; \
                numPred ++; \
            } \
            /* right-top MV */ \
            if (meData->xMB < mNumMacroBlockPerRow - 1) { \
                meData->mvPred[numPred] = meData->MBcurr[-mNumMacroBlockPerRow+1].mv[2]; \
                numPred ++; \
            } \
        } \
    } \
    meData->numPred = numPred; \
}

void VideoEncoderMPEG4::ME_MacroBlock_PSH(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow)
{
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s quant, stepL, stepC, not_coded;
    Ipp32s dev, bestSAD, sadU, sadV, sadL0, sadL1, sadL2, sadL3, ncThrL, ncThrC, sadFavorInter;
    IppMotionVector mvLuma;
    mp4_MacroBlock *MBcurr;

    MBcurr = meData->MBcurr;
    quant = meData->quant;
    // calc SAD for Y, U,V blocks at (0,0)
    pYc = meData->pYc;
    pUc = meData->pUc;
    pVc = meData->pVc;
    pYf = meData->pYr;
    pUf = meData->pUr;
    pVf = meData->pVr;
    stepL = meData->stepL;
    stepC = meData->stepC;
    ippiSAD8x8_8u32s_C1R(pYc, stepL, pYf, stepL, &sadL0, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8, stepL, pYf+8, stepL, &sadL1, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL, stepL, pYf+8*stepL, stepL, &sadL2, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL+8, stepL, pYf+8*stepL+8, stepL, &sadL3, IPPVC_MC_APX_FF);
    // not_coded decision
    ncThrL = quant * SAD_NOTCODED_THR_LUMA;
    not_coded = (sadL0 < ncThrL) && (sadL1 < ncThrL) && (sadL2 < ncThrL) && (sadL3 < ncThrL);
    sadU = sadV = 0;
    if (not_coded || (meData->flags & ME_CHROMA)) {
        ippiSAD8x8_8u32s_C1R(pUc, stepC, pUf, stepC, &sadU, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pVc, stepC, pVf, stepC, &sadV, IPPVC_MC_APX_FF);
        if (not_coded) {
            ncThrC = quant * SAD_NOTCODED_THR_CHROMA;
            not_coded = (sadU < ncThrC) && (sadV < ncThrC);
        }
    }
    if (not_coded) {
        MBcurr->not_coded = 1;
        MBcurr->type = IPPVC_MBTYPE_INTER;
        MBcurr->mv[0].dx = MBcurr->mv[0].dy = 0;
    } else {
        MBcurr->not_coded = 0;
        PredictMV(MBcurr, meData->yMB - startRow, meData->xMB, &mvLuma);
        // SAD at (0,0)
        bestSAD = sadL0 + sadL1 + sadL2 + sadL3;
        if (meData->flags & ME_USE_MVWEIGHT)
            bestSAD += mp4_WeightMV(0, 0, mvLuma, 1, quant, 0);
        if (meData->flags & ME_CHROMA)
            bestSAD += sadU + sadV;
        if (bestSAD > meData->thrDiff16x16) {
            meData->bestDiff = bestSAD;
            mp4_ME_SetPredictors(mvLuma, MBcurr[0].mv[0], MBcurr[-1].mv[0], MBcurr[-mNumMacroBlockPerRow].mv[0], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[0], startRow, endRow);
            mp4_ME_SAD(meData);
            mvLuma.dx = (Ipp16s)meData->xPos;
            mvLuma.dy = (Ipp16s)meData->yPos;
            bestSAD = meData->bestDiff;
        } else {
            mvLuma.dx = mvLuma.dy = 0;
        }
        sadFavorInter = SAD_FAVOR_INTER;
//        if (meData->flags & ME_CHROMA)
//          sadFavorInter += sadFavorInter >> 2;
        dev = SAD_MAX;
        if (bestSAD > sadFavorInter) {
            ippiMeanAbsDev16x16_8u32s_C1R(pYc, stepL, &dev);
            if (meData->flags & ME_CHROMA) {
                Ipp32s  devC;
                ippiMeanAbsDev8x8_8u32s_C1R(pUc, stepC, &devC);
                dev += devC;
                ippiMeanAbsDev8x8_8u32s_C1R(pVc, stepC, &devC);
                dev += devC;
            }
        }
        if (dev < bestSAD - sadFavorInter) {
            // intra coded
            MBcurr->type = IPPVC_MBTYPE_INTRA;
            MBcurr->mv[0].dx = MBcurr->mv[0].dy = 0;
            MBcurr->lumaErr = dev;
        } else {
            // inter coded
            MBcurr->type = IPPVC_MBTYPE_INTER;
            MBcurr->mv[0] = mvLuma;
            MBcurr->lumaErr = bestSAD;
        }
    }
}

inline void mp4_MV_GetDiff(IppMotionVector *mvC, IppMotionVector *mvP, Ipp32s fMin, Ipp32s fMax, Ipp32s fRange)
{
    Ipp32s  mvDx, mvDy;

    mvDx = mvC->dx - mvP->dx;
    mvDy = mvC->dy - mvP->dy;
    if (mvDx < fMin)
        mvDx += fRange;
    else if (mvDx > fMax)
        mvDx -= fRange;
    if (mvDy < fMin)
        mvDy += fRange;
    else if (mvDy > fMax)
        mvDy -= fRange;
    mvP->dx = (Ipp16s)mvDx;
    mvP->dy = (Ipp16s)mvDy;
}

void VideoEncoderMPEG4::ME_MacroBlock_P(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow)
{
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s  quant, stepL, stepC, bestSAD16x16, bestSAD16x8, bestSAD8x8, qpel, fcode, not_coded;
    Ipp32s  dev, bestSAD, sadU, sadV, sadL0, sadL1, sadL2, sadL3, ncThrL, ncThrC, sadFavorInter;
    IppMotionVector mvLuma = {0}, mvLumaT = {0}, mvLumaB = {0}, mvPred[4];
    mp4_MacroBlock *MBcurr;
    Ipp32s  fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
//for RD_MODE_DECISION
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    __ALIGN16(Ipp8u, mcPred, 64*6);
    int  nzCount[6], pattern, dct_type, costInter16x16, costInter16x8, costInter8x8, costIntra, costInter;
    int rt = meData->rt;

    MBcurr = meData->MBcurr;
    quant = meData->quant;
    MBcurr->validPredInter = 1;
    MBcurr->fieldmc = 0;
    // calc SAD for Y,U,V blocks at (0,0)
    pYc = meData->pYc;
    pUc = meData->pUc;
    pVc = meData->pVc;
    pYf = meData->pYr;
    pUf = meData->pUr;
    pVf = meData->pVr;
    stepL = meData->stepL;
    stepC = meData->stepC;
    ippiSAD8x8_8u32s_C1R(pYc, stepL, pYf, stepL, &sadL0, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8, stepL, pYf+8, stepL, &sadL1, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL, stepL, pYf+8*stepL, stepL, &sadL2, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL+8, stepL, pYf+8*stepL+8, stepL, &sadL3, IPPVC_MC_APX_FF);
    // not_coded decision
    ncThrL = quant * SAD_NOTCODED_THR_LUMA;
    not_coded = (mBVOPdist == 0) && (sadL0 < ncThrL) && (sadL1 < ncThrL) && (sadL2 < ncThrL) && (sadL3 < ncThrL);
    sadU = sadV = 0;
    if (not_coded || (meData->flags & ME_CHROMA)) {
        ippiSAD8x8_8u32s_C1R(pUc, stepC, pUf, stepC, &sadU, IPPVC_MC_APX_FF);
        ippiSAD8x8_8u32s_C1R(pVc, stepC, pVf, stepC, &sadV, IPPVC_MC_APX_FF);
        if (not_coded) {
            ncThrC = quant * SAD_NOTCODED_THR_CHROMA;
            not_coded = (sadU < ncThrC) && (sadV < ncThrC);
        }
    }
    if (not_coded) {
        MBcurr->not_coded = 1;
        MBcurr->type = IPPVC_MBTYPE_INTER;
        mp4_Zero4MV(MBcurr->mv);
    } else {
        fcode = meData->fcode;
        qpel = meData->flags & ME_QP;
        MBcurr->not_coded = 0;
        bestSAD16x16 = bestSAD16x8 = bestSAD8x8 = SAD_MAX;
        costInter = costInter16x16 = costInter16x8 = costInter8x8 = SAD_MAX * 1234;
        Predict1MV(MBcurr, meData->yMB - startRow, meData->xMB, &mvPred[0]);
        // frame ME
        if (!VOL.interlaced || (meData->flags & ME_FRAME)) {
            // SAD at (0,0)
            bestSAD16x16 = sadL0 + sadL1 + sadL2 + sadL3;
            if (meData->flags & ME_USE_MVWEIGHT)
                bestSAD16x16 += mp4_WeightMV(0, 0, mvPred[0], fcode, quant, 0);
            if (meData->flags & ME_CHROMA)
                bestSAD16x16 += sadU + sadV;
            if (bestSAD16x16 > meData->thrDiff16x16) {
                mp4_ME_SetPredictors(mvPred[0], MBcurr[0].mv[0], MBcurr[-1].mv[1], MBcurr[-mNumMacroBlockPerRow].mv[2], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[0], startRow, endRow);
                meData->bestDiff = bestSAD16x16;
                meData->thrDiff = meData->thrDiff16x16;
                meData->sadFunc = ippiSAD16x16_8u32s;
                meData->copyQPFunc = ippiCopy16x16QP_MPEG4_8u_C1R;
                meData->copyHPFunc = ippiCopy16x16HP_8u_C1R;
                mp4_ME_SAD(meData);
                mvLuma.dx = (Ipp16s)meData->xPos;
                mvLuma.dy = (Ipp16s)meData->yPos;
                bestSAD16x16 = meData->bestDiff;
                if (meData->flags & RD_MODE_DECISION) {
                    IppMotionVector mvChroma, mvDiff;
                    if (VOL.quarter_sample) {
                        mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, rt);
                        mp4_ComputeChromaMVQ(&mvLuma, &mvChroma);
                    } else {
                        mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, rt);
                        mp4_ComputeChromaMV(&mvLuma, &mvChroma);
                    }
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                    pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter16x16);
                    costInter16x16 += RD_MUL * (mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                    mvDiff = mvPred[0];
                    mp4_MV_GetDiff(&mvLuma, &mvDiff, fRangeMin, fRangeMax, fRange);
                    costInter16x16 += RD_MUL * mp4_CalcBitsMV(&mvDiff, fcode);
                } else if (!(meData->flags & ME_USE_MVWEIGHT))
                    bestSAD16x16 -= SAD_FAVOR_16x16;
                if ((meData->flags & ME_4MV) && (bestSAD16x16 > SAD_NOTCODED_THR_LUMA * 12 * quant)) {
                    Ipp32s sFlags = meData->flags;
                    meData->flags &= ~ME_CHROMA;
                    //meData->flags &= ~ME_USE_MVWEIGHT;
                    meData->thrDiff = meData->thrDiff8x8;
                    meData->sadFunc = ippiSAD8x8_8u32s_C1R;
                    meData->copyQPFunc = ippiCopy8x8QP_MPEG4_8u_C1R;
                    meData->copyHPFunc = ippiCopy8x8HP_8u_C1R;
                    meData->bestDiff = sadL0;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[0], fcode, quant, 0);
                //meData->mvPred[meData->numPred] = mvLuma;
                //meData->numPred ++;
                    mp4_ME_SAD(meData);
                    bestSAD8x8 = meData->bestDiff;
                    MBcurr->mv[0].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[0].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 1);
                    meData->mvPred[0] = mvPred[1];
                    meData->pYc = pYc + 8;
                    meData->pYr = pYf + 8;
                    meData->bestDiff = sadL1;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[1], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[1], MBcurr[0].mv[1], MBcurr[0].mv[0], MBcurr[-mNumMacroBlockPerRow].mv[3], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[1], startRow, endRow);
                //meData->mvPred[meData->numPred] = mvLuma;
                //meData->numPred ++;
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[1].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[1].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 2);
                    meData->mvPred[0] = mvPred[2];
                    meData->pYc = pYc + stepL * 8;
                    meData->pYr = pYf + stepL * 8;
                    meData->bestDiff = sadL2;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[2], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[2], MBcurr[0].mv[2], MBcurr[-1].mv[3], MBcurr[0].mv[0], MBcurr[1].mv[2], MBcurr[mNumMacroBlockPerRow].mv[0], startRow, endRow);
                //meData->mvPred[meData->numPred] = mvLuma;
                //meData->numPred ++;
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[2].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[2].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 3);
                    meData->mvPred[0] = mvPred[3];
                    meData->pYc = pYc + stepL * 8 + 8;
                    meData->pYr = pYf + stepL * 8 + 8;
                    meData->bestDiff = sadL3;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[3], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[3], MBcurr[0].mv[3], MBcurr[0].mv[2], MBcurr[0].mv[1], MBcurr[1].mv[2], MBcurr[mNumMacroBlockPerRow].mv[1], startRow, endRow);
                //meData->mvPred[meData->numPred] = mvLuma;
                //meData->numPred ++;
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[3].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[3].dy = (Ipp16s)meData->yPos;
                    meData->pYc = pYc;
                    meData->pYr = pYf;
                    meData->flags = sFlags;
                    if (meData->flags & RD_MODE_DECISION) {
                        IppMotionVector mvChroma, mvDiff;
                        if (qpel) {
                            mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &MBcurr->mv[0], rt);
                            mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &MBcurr->mv[1], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &MBcurr->mv[2], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &MBcurr->mv[3], rt);
                            mp4_ComputeChroma4MVQ(MBcurr->mv, &mvChroma);
                        } else {
                            mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &MBcurr->mv[0], rt);
                            mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &MBcurr->mv[1], rt);
                            mp4_Copy8x8HP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &MBcurr->mv[2], rt);
                            mp4_Copy8x8HP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &MBcurr->mv[3], rt);
                            mp4_ComputeChroma4MV(MBcurr->mv, &mvChroma);
                        }
                        mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                        mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                        pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter8x8);
                        costInter8x8 += RD_MUL * (mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER4V*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                        for (int i = 0; i < 4; i ++) {
                            mvDiff = mvPred[i];
                            mp4_MV_GetDiff(&MBcurr->mv[i], &mvDiff, fRangeMin, fRangeMax, fRange);
                            costInter8x8 += RD_MUL * mp4_CalcBitsMV(&mvDiff, fcode);
                        }
                    } else {
                        //if (meData->flags & ME_USE_MVWEIGHT) {
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[0].dx, MBcurr->mv[0].dy, mvPred[0], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[1].dx, MBcurr->mv[1].dy, mvPred[1], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[2].dx, MBcurr->mv[2].dy, mvPred[2], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[3].dx, MBcurr->mv[3].dy, mvPred[3], fcode, quant, 0);
                        //}
                        if (meData->flags & ME_CHROMA) {
                            IppMotionVector mvC;
                            Ipp32s          sadC;

                            if (!qpel)
                                mp4_ComputeChroma4MV(MBcurr->mv, &mvC);
                            else
                                mp4_ComputeChroma4MVQ(MBcurr->mv, &mvC);
                            ippiSAD8x8_8u32s_C1R(pUc, stepC, pUf+MP4_MV_OFF_HP(mvC.dx, mvC.dy, stepC), stepC, &sadC, MP4_MV_ACC_HP_SAD(mvC.dx, mvC.dy));
                            bestSAD8x8 += sadC;
                            ippiSAD8x8_8u32s_C1R(pVc, stepC, pVf+MP4_MV_OFF_HP(mvC.dx, mvC.dy, stepC), stepC, &sadC, MP4_MV_ACC_HP_SAD(mvC.dx, mvC.dy));
                            bestSAD8x8 += sadC;
                        }
                    }
                }
            } else {
                mvLuma.dx = mvLuma.dy = 0;
            }
        }
        // field ME
        if (VOL.interlaced && (meData->flags & ME_FIELD) && (bestSAD16x16 > meData->thrDiff16x16) && (bestSAD8x8 > meData->thrDiff16x16)) {
            // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
            const int mb_ftfr = 0, mb_fbfr = 1;
            Ipp32s  tSAD, bSAD, mvw;
            mvLumaT.dx = mvLumaB.dx = mvPred[0].dx;
            mvLumaT.dy = mvLumaB.dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
            mvw = mp4_WeightMV(0, 0, mvLumaT, fcode, quant, 0);
            // SAD at (0,0)
            ippiSAD16x8_8u32s_C1R(pYc, stepL << 1, pYf, stepL << 1, &tSAD, IPPVC_MC_APX_FF);
            ippiSAD16x8_8u32s_C1R(pYc+stepL, stepL << 1, pYf+stepL, stepL << 1, &bSAD, IPPVC_MC_APX_FF);
            if (meData->flags & ME_USE_MVWEIGHT) {
                tSAD += mvw;
                bSAD += mvw;
            }
            if (bSAD + tSAD > meData->thrDiff16x8) {
                Ipp32s syT = meData->yT;
                Ipp32s syB = meData->yB;
                int sFlags = meData->flags;

                meData->flags &= ~ME_CHROMA;
                meData->numPred = 1;
                if (meData->flags & ME_FRAME) {
                    meData->mvPred[1] = mvLuma;
                    meData->numPred = 2;
                }
                meData->thrDiff = meData->thrDiff16x8;
                meData->sadFunc = ippiSAD16x8_8u32s_C1R;
                meData->copyQPFunc = ippiCopy16x8QP_MPEG4_8u_C1R;
                meData->copyHPFunc = ippiCopy16x8HP_8u_C1R;
                meData->yT = mp4_Div2(syT);
                meData->yB = mp4_Div2(syB);
                meData->stepL = stepL + stepL;
                meData->mvPred[0] = mvLumaT;
                //mp4_ME_SetPredictors(mvLumaT, MBcurr[0].mv[0], MBcurr[-1].mv[1], MBcurr[-mNumMacroBlockPerRow].mv[2], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[0]);
                meData->bestDiff = tSAD;
                mp4_ME_SAD(meData);
                mvLumaT.dx = (Ipp16s)meData->xPos;
                mvLumaT.dy = (Ipp16s)meData->yPos;
                tSAD = meData->bestDiff;
                meData->pYc += stepL;
                meData->pYr += stepL;
                meData->mvPred[0] = mvLumaB;
                meData->bestDiff = bSAD;
                mp4_ME_SAD(meData);
                mvLumaB.dx = (Ipp16s)meData->xPos;
                mvLumaB.dy = (Ipp16s)meData->yPos;
                bSAD = meData->bestDiff;
                meData->yT = syT;
                meData->yB = syB;
                meData->stepL = stepL;
                meData->pYc = pYc;
                meData->pYr = pYf;
                meData->flags = sFlags;
            } else {
                mvLumaT.dx = mvLumaT.dy = mvLumaB.dx = mvLumaB.dy = 0;
            }
            bestSAD16x8 = bSAD + tSAD;
            if (!(meData->flags & ME_USE_MVWEIGHT))
                bestSAD16x8 -= SAD_FAVOR_FIELD;
            if (meData->flags & RD_MODE_DECISION) {
                IppMotionVector mvTmpT, mvTmpB, mvChromaT, mvChromaB;
                if (qpel) {
                    mp4_Copy16x8QP_8u(pYf+stepL*mb_ftfr, stepL*2, mcPred, 32, &mvLumaT, rt);
                    mp4_Copy16x8QP_8u(pYf+stepL*mb_fbfr, stepL*2, mcPred+16, 32, &mvLumaB, rt);
                    mvTmpT.dx = (Ipp16s)mp4_Div2(mvLumaT.dx);
                    mvTmpT.dy = (Ipp16s)(mp4_Div2(mvLumaT.dy << 1) >> 1);
                    mvTmpB.dx = (Ipp16s)mp4_Div2(mvLumaB.dx);
                    mvTmpB.dy = (Ipp16s)(mp4_Div2(mvLumaB.dy << 1) >> 1);
                    mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                } else {
                    mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_ftfr, stepL*2, mcPred, 32, &mvLumaT, rt);
                    mp4_Copy16x8HP_8u(pYf+mStepLuma*mb_fbfr, stepL*2, mcPred+16, 32, &mvLumaB, rt);
                    mp4_ComputeChromaMV(&mvLumaT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvLumaB, &mvChromaB);
                }
                mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*4, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*5, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter16x8);
                costInter16x8 += RD_MUL * (3 + mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                mvTmpT.dx = mvPred[0].dx;
                mvTmpT.dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
                mvTmpB = mvTmpT;
                mp4_MV_GetDiff(&mvLumaT, &mvTmpT, fRangeMin, fRangeMax, fRange);
                costInter16x8 += RD_MUL * mp4_CalcBitsMV(&mvTmpT, fcode);
                mp4_MV_GetDiff(&mvLumaB, &mvTmpB, fRangeMin, fRangeMax, fRange);
                costInter16x8 += RD_MUL * mp4_CalcBitsMV(&mvTmpB, fcode);
            } else if (meData->flags & ME_CHROMA) {
                IppMotionVector mvChromaT, mvChromaB;
                Ipp32s          sadC;
                if (qpel) {
                    IppMotionVector mvTmpT, mvTmpB;
                    mvTmpT.dx = (Ipp16s)mp4_Div2(mvLumaT.dx);
                    mvTmpT.dy = (Ipp16s)(mp4_Div2(mvLumaT.dy << 1) >> 1);
                    mvTmpB.dx = (Ipp16s)mp4_Div2(mvLumaB.dx);
                    mvTmpB.dy = (Ipp16s)(mp4_Div2(mvLumaB.dy << 1) >> 1);
                    mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                } else {
                    mp4_ComputeChromaMV(&mvLumaT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvLumaB, &mvChromaB);
                }
                mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*4, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*5, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                ippiSAD8x8_8u32s_C1R(pUc, stepC, mcPred+64*4, 8, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x8 += sadC;
                ippiSAD8x8_8u32s_C1R(pVc, stepC, mcPred+64*5, 8, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x8 += sadC;
            }
        }
        if (meData->flags & RD_MODE_DECISION) {
            bestSAD16x16 = costInter16x16;
            bestSAD8x8 = costInter8x8;
            bestSAD16x8 = costInter16x8;
            costInter = IPP_MIN(costInter16x16, IPP_MIN(costInter8x8, costInter16x8));
        }
        if ((bestSAD16x16 <= bestSAD8x8) && (bestSAD16x16 <= bestSAD16x8)) {
            bestSAD = bestSAD16x16;
            if (!(meData->flags & ME_USE_MVWEIGHT))
                bestSAD += SAD_FAVOR_16x16;
            MBcurr->type = IPPVC_MBTYPE_INTER;
            MBcurr->mv[0] = MBcurr->mv[1] = MBcurr->mv[2] = MBcurr->mv[3] = mvLuma;
            mp4_MV_GetDiff(&mvLuma, &mvPred[0], fRangeMin, fRangeMax, fRange);
            MBcurr->mvDiff[0] = mvPred[0];
        } else if (bestSAD16x8 <= bestSAD8x8) {
            bestSAD = bestSAD16x8;
            if (!(meData->flags & ME_USE_MVWEIGHT))
                bestSAD += SAD_FAVOR_FIELD;
            MBcurr->type = IPPVC_MBTYPE_INTER;
            MBcurr->mvT = mvLumaT; MBcurr->mvB = mvLumaB;
            MBcurr->mv[0].dx = MBcurr->mv[1].dx = MBcurr->mv[2].dx = MBcurr->mv[3].dx = (Ipp16s)mp4_Div2Round(mvLumaT.dx + mvLumaB.dx);
            MBcurr->mv[0].dy = MBcurr->mv[1].dy = MBcurr->mv[2].dy = MBcurr->mv[3].dy = (Ipp16s)(mvLumaT.dy + mvLumaB.dy);
            mvPred[0].dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
            mvPred[2] = mvPred[0];
            mp4_MV_GetDiff(&mvLumaT, &mvPred[0], fRangeMin, fRangeMax, fRange);
            mp4_MV_GetDiff(&mvLumaB, &mvPred[2], fRangeMin, fRangeMax, fRange);
            MBcurr->mvDiff[0] = mvPred[0];
            MBcurr->mvDiff[2] = mvPred[2];
            MBcurr->fieldmc = 1;
        } else {
            bestSAD = bestSAD8x8;
            MBcurr->type = IPPVC_MBTYPE_INTER4V;
            // ME_SAD_8x8 writes resulting MVs in the MBcurr
            mp4_MV_GetDiff(&MBcurr->mv[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
            mp4_MV_GetDiff(&MBcurr->mv[1], &mvPred[1], fRangeMin, fRangeMax, fRange);
            mp4_MV_GetDiff(&MBcurr->mv[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
            mp4_MV_GetDiff(&MBcurr->mv[3], &mvPred[3], fRangeMin, fRangeMax, fRange);
            MBcurr->mvDiff[0] = mvPred[0];
            MBcurr->mvDiff[1] = mvPred[1];
            MBcurr->mvDiff[2] = mvPred[2];
            MBcurr->mvDiff[3] = mvPred[3];
        }
        if (meData->flags & RD_MODE_DECISION) {
            int ac_pred_flag, pattern1, predDir[6], use_intra_dc_vlc = 1;

            costIntra = costInter;
            pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, meData->yMB, meData->xMB, &dct_type, use_intra_dc_vlc, MBcurr, predDir, startRow, &ac_pred_flag, &pattern1, &costIntra);
            costIntra += RD_MUL * (1 + mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTRA*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[15-(pattern >> 2)].len);
            if (costIntra < costInter) {
                MBcurr->type = IPPVC_MBTYPE_INTRA;
                mp4_Zero4MV(MBcurr->mv);
                bestSAD = costIntra;
            } else {
                MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
            }
        } else {
            sadFavorInter = SAD_FAVOR_INTER;
            //if (meData->flags & ME_CHROMA)
            //    sadFavorInter += sadFavorInter >> 2;
            if (bestSAD > sadFavorInter) {
                // choose mbtype - Inter or Intra
                ippiMeanAbsDev16x16_8u32s_C1R(pYc, stepL, &dev);
                if (meData->flags & ME_CHROMA) {
                    Ipp32s  devC;
                    ippiMeanAbsDev8x8_8u32s_C1R(pUc, stepC, &devC);
                    dev += devC;
                    ippiMeanAbsDev8x8_8u32s_C1R(pVc, stepC, &devC);
                    dev += devC;
                }
                if (meData->xMB > 0)
                    if (MBcurr[-1].type == IPPVC_MBTYPE_INTRA)
                        dev -= DEV_FAVOR_INTRA;
                if (meData->yMB - startRow > 0)
                    if (MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA)
                        dev -= DEV_FAVOR_INTRA;
                if (dev < bestSAD - sadFavorInter) {
                    MBcurr->type = IPPVC_MBTYPE_INTRA;
                    mp4_Zero4MV(MBcurr->mv);
                    bestSAD = dev;
                }
            }
        }
        MBcurr->lumaErr = bestSAD;
    }
}

void VideoEncoderMPEG4::ME_MacroBlock_S(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow)
{
    Ipp8u  *pYc, *pUc, *pVc, *pYf, *pUf, *pVf;
    Ipp32s quant, stepL, stepC, bestSAD16x16, bestSAD16x8, bestSAD8x8, bestSADGMC, qpel, fcode;
    Ipp32s dev, bestSAD, sadU, sadV, sadL0, sadL1, sadL2, sadL3, ncThrL, ncThrC, sadFavorInter;
    IppMotionVector mvLuma = {0}, mvLumaT = {0}, mvLumaB = {0}, mvPred[4], mvGMC;
    mp4_MacroBlock *MBcurr;
    IppiRect mbRectL, mbRectC;
    __ALIGN16(Ipp8u, mcPred, 64*6);
    Ipp32s  fRangeMin = -(16 << VOP.vop_fcode_forward), fRangeMax = (16 << VOP.vop_fcode_forward) - 1, fRange = fRangeMax - fRangeMin + 1;
//for RD_MODE_DECISION
    __ALIGN16(Ipp16s, coeffMB, 64*6);
    int  nzCount[6], pattern, dct_type, costInter16x16, costInter16x8, costInter8x8, costIntra, costInter, costGMC;
    int rt = meData->rt;

    MBcurr = meData->MBcurr;
    quant = meData->quant;
    MBcurr->validPredInter = 1;
    MBcurr->fieldmc = 0;
    // calc SAD for Y,U,V blocks at (0,0)
    pYc = meData->pYc;
    pUc = meData->pUc;
    pVc = meData->pVc;
    pYf = meData->pYr;
    pUf = meData->pUr;
    pVf = meData->pVr;
    stepL = meData->stepL;
    stepC = meData->stepC;
    mbRectL.x = meData->xMB * 16;
    mbRectL.y = meData->yMB * 16;
    mbRectL.width = mbRectL.height = 16;
    mbRectC.x = meData->xMB * 8;
    mbRectC.y = meData->yMB * 8;
    mbRectC.width = mbRectC.height = 8;
    // apply warping to the macroblock
    ippiWarpLuma_MPEG4_8u_C1R(mFrameF->pY, mStepLuma, mcPred, 16, &mbRectL, mWarpSpec);
    ippiWarpChroma_MPEG4_8u_P2R(mFrameF->pU, mStepChroma, mFrameF->pV, mStepChroma, mcPred+64*4, 8, mcPred+64*5, 8, &mbRectC, mWarpSpec);
    ippiSAD8x8_8u32s_C1R(pYc, stepL, mcPred, 16, &sadL0, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8, stepL, mcPred+8, 16, &sadL1, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL, stepL, mcPred+8*16, 16, &sadL2, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pYc+8*stepL+8, stepL, mcPred+8*16+8, 16, &sadL3, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pUc, stepC, mcPred+64*4, 8, &sadU, IPPVC_MC_APX_FF);
    ippiSAD8x8_8u32s_C1R(pVc, stepC, mcPred+64*5, 8, &sadV, IPPVC_MC_APX_FF);
    // not_coded decision
    ncThrL = quant * SAD_NOTCODED_THR_LUMA;
    ncThrC = quant * SAD_NOTCODED_THR_CHROMA;
    if ((sadL0 < ncThrL) && (sadL1 < ncThrL) && (sadL2 < ncThrL) && (sadL3 < ncThrL) && (sadU < ncThrC) && (sadV < ncThrC)) {
        MBcurr->not_coded = 1;
        MBcurr->mcsel = 1;
        MBcurr->type = IPPVC_MBTYPE_INTER;
        ippiCalcGlobalMV_MPEG4(meData->xMB*16, meData->yMB*16, &mvGMC, mWarpSpec);
        MBcurr->mv[0] = MBcurr->mv[1] = MBcurr->mv[2] = MBcurr->mv[3] = mvGMC;
    } else {
        bestSADGMC = sadL0 + sadL1 + sadL2 + sadL3 - SAD_FAVOR_GMC;
        if (meData->flags & ME_CHROMA)
            bestSADGMC += sadU + sadV;
        fcode = meData->fcode;
        qpel = meData->flags & ME_QP;
        MBcurr->not_coded = 0;
        bestSAD16x16 = bestSAD16x8 = bestSAD8x8 = SAD_MAX;
        costInter = costGMC = costInter16x16 = costInter16x8 = costInter8x8 = SAD_MAX * 1234;
        if (meData->flags & RD_MODE_DECISION)
            TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costGMC);
        Predict1MV(MBcurr, meData->yMB - startRow, meData->xMB, &mvPred[0]);
        // frame ME
        if (!VOL.interlaced || (meData->flags & ME_FRAME)) {
            // SAD at (0,0)
            ippiSAD16x16_8u32s(pYc, stepL, pYf, stepL, &bestSAD16x16, IPPVC_MC_APX_FF);
            if (meData->flags & ME_USE_MVWEIGHT)
                bestSAD16x16 += mp4_WeightMV(0, 0, mvPred[0], fcode, quant, 0);
            if (meData->flags & ME_CHROMA) {
                Ipp32s  sadC;
                ippiSAD8x8_8u32s_C1R(pUc, stepC, pUf, stepC, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x16 += sadC;
                ippiSAD8x8_8u32s_C1R(pVc, stepC, pVf, stepC, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x16 += sadC;
            }
            if (bestSAD16x16 > meData->thrDiff16x16) {
                mp4_ME_SetPredictors(mvPred[0], MBcurr[0].mv[0], MBcurr[-1].mv[1], MBcurr[-mNumMacroBlockPerRow].mv[2], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[0], startRow, endRow);
                meData->bestDiff = bestSAD16x16;
                meData->thrDiff = meData->thrDiff16x16;
                meData->sadFunc = ippiSAD16x16_8u32s;
                meData->copyQPFunc = ippiCopy16x16QP_MPEG4_8u_C1R;
                meData->copyHPFunc = ippiCopy16x16HP_8u_C1R;
                mp4_ME_SAD(meData);
                mvLuma.dx = (Ipp16s)meData->xPos;
                mvLuma.dy = (Ipp16s)meData->yPos;
                bestSAD16x16 = meData->bestDiff;
                if (meData->flags & RD_MODE_DECISION) {
                    IppMotionVector mvChroma, mvDiff;
                    if (VOL.quarter_sample) {
                        mp4_Copy16x16QP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, rt);
                        mp4_ComputeChromaMVQ(&mvLuma, &mvChroma);
                    } else {
                        mp4_Copy16x16HP_8u(pYf, mStepLuma, mcPred, 16, &mvLuma, rt);
                        mp4_ComputeChromaMV(&mvLuma, &mvChroma);
                    }
                    mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                    mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                    pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter16x16);
                    costInter16x16 += RD_MUL * (mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                    mvDiff = mvPred[0];
                    mp4_MV_GetDiff(&mvLuma, &mvDiff, fRangeMin, fRangeMax, fRange);
                    costInter16x16 += RD_MUL * mp4_CalcBitsMV(&mvDiff, fcode);
                } else if (!(meData->flags & ME_USE_MVWEIGHT))
                    bestSAD16x16 -= SAD_FAVOR_16x16;
                if ((meData->flags & ME_4MV) && (bestSAD16x16 > SAD_NOTCODED_THR_LUMA * 12 * quant)) {
                    Ipp32s sFlags = meData->flags;
                    meData->flags &= ~ME_CHROMA;
                    //meData->flags &= ~ME_USE_MVWEIGHT;
                    meData->thrDiff = meData->thrDiff8x8;
                    meData->sadFunc = ippiSAD8x8_8u32s_C1R;
                    meData->copyQPFunc = ippiCopy8x8QP_MPEG4_8u_C1R;
                    meData->copyHPFunc = ippiCopy8x8HP_8u_C1R;
                    meData->bestDiff = sadL0;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[0], fcode, quant, 0);
                    mp4_ME_SAD(meData);
                    bestSAD8x8 = meData->bestDiff;
                    MBcurr->mv[0].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[0].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 1);
                    meData->mvPred[0] = mvPred[1];
                    meData->pYc = pYc + 8;
                    meData->pYr = pYf + 8;
                    meData->bestDiff = sadL1;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[1], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[1], MBcurr[0].mv[1], MBcurr[0].mv[0], MBcurr[-mNumMacroBlockPerRow].mv[3], MBcurr[1].mv[0], MBcurr[mNumMacroBlockPerRow].mv[1], startRow, endRow);
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[1].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[1].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 2);
                    meData->mvPred[0] = mvPred[2];
                    meData->pYc = pYc + stepL * 8;
                    meData->pYr = pYf + stepL * 8;
                    meData->bestDiff = sadL2;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[2], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[2], MBcurr[0].mv[2], MBcurr[-1].mv[3], MBcurr[0].mv[0], MBcurr[1].mv[2], MBcurr[mNumMacroBlockPerRow].mv[0], startRow, endRow);
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[2].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[2].dy = (Ipp16s)meData->yPos;
                    Predict3MV(MBcurr, meData->yMB - startRow, meData->xMB, mvPred, 3);
                    meData->mvPred[0] = mvPred[3];
                    meData->pYc = pYc + stepL * 8 + 8;
                    meData->pYr = pYf + stepL * 8 + 8;
                    meData->bestDiff = sadL3;
                    if (meData->flags & ME_USE_MVWEIGHT)
                        meData->bestDiff += mp4_WeightMV(0, 0, mvPred[3], fcode, quant, 0);
                    mp4_ME_SetPredictors(mvPred[3], MBcurr[0].mv[3], MBcurr[0].mv[2], MBcurr[0].mv[1], MBcurr[1].mv[2], MBcurr[mNumMacroBlockPerRow].mv[1], startRow, endRow);
                    mp4_ME_SAD(meData);
                    bestSAD8x8 += meData->bestDiff;
                    MBcurr->mv[3].dx = (Ipp16s)meData->xPos;
                    MBcurr->mv[3].dy = (Ipp16s)meData->yPos;
                    meData->pYc = pYc;
                    meData->pYr = pYf;
                    meData->flags = sFlags;
                    if (meData->flags & RD_MODE_DECISION) {
                        IppMotionVector mvChroma, mvDiff;
                        if (qpel) {
                            mp4_Copy8x8QP_8u(pYf, mStepLuma, mcPred, 16, &MBcurr->mv[0], rt);
                            mp4_Copy8x8QP_8u(pYf+8, mStepLuma, mcPred+8, 16, &MBcurr->mv[1], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &MBcurr->mv[2], rt);
                            mp4_Copy8x8QP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &MBcurr->mv[3], rt);
                            mp4_ComputeChroma4MVQ(MBcurr->mv, &mvChroma);
                        } else {
                            mp4_Copy8x8HP_8u(pYf, mStepLuma, mcPred, 16, &MBcurr->mv[0], rt);
                            mp4_Copy8x8HP_8u(pYf+8, mStepLuma, mcPred+8, 16, &MBcurr->mv[1], rt);
                            mp4_Copy8x8HP_8u(pYf+mStepLuma*8, mStepLuma, mcPred+16*8, 16, &MBcurr->mv[2], rt);
                            mp4_Copy8x8HP_8u(pYf+mStepLuma*8+8, mStepLuma, mcPred+16*8+8, 16, &MBcurr->mv[3], rt);
                            mp4_ComputeChroma4MV(MBcurr->mv, &mvChroma);
                        }
                        mp4_Copy8x8HP_8u(pUf, mStepChroma, mcPred+64*4, 8, &mvChroma, rt);
                        mp4_Copy8x8HP_8u(pVf, mStepChroma, mcPred+64*5, 8, &mvChroma, rt);
                        pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter8x8);
                        costInter8x8 += RD_MUL * (mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER4V*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                        for (int i = 0; i < 4; i ++) {
                            mvDiff = mvPred[i];
                            mp4_MV_GetDiff(&MBcurr->mv[i], &mvDiff, fRangeMin, fRangeMax, fRange);
                            costInter8x8 += RD_MUL * mp4_CalcBitsMV(&mvDiff, fcode);
                        }
                    } else {
                        //if (meData->flags & ME_USE_MVWEIGHT) {
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[0].dx, MBcurr->mv[0].dy, mvPred[0], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[1].dx, MBcurr->mv[1].dy, mvPred[1], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[2].dx, MBcurr->mv[2].dy, mvPred[2], fcode, quant, 0);
                        //    bestSAD8x8 += mp4_WeightMV(MBcurr->mv[3].dx, MBcurr->mv[3].dy, mvPred[3], fcode, quant, 0);
                        //}
                        if (meData->flags & ME_CHROMA) {
                            IppMotionVector mvC;
                            Ipp32s          sadC;

                            if (!qpel)
                                mp4_ComputeChroma4MV(MBcurr->mv, &mvC);
                            else
                                mp4_ComputeChroma4MVQ(MBcurr->mv, &mvC);
                            ippiSAD8x8_8u32s_C1R(pUc, stepC, pUf+MP4_MV_OFF_HP(mvC.dx, mvC.dy, stepC), stepC, &sadC, MP4_MV_ACC_HP_SAD(mvC.dx, mvC.dy));
                            bestSAD8x8 += sadC;
                            ippiSAD8x8_8u32s_C1R(pVc, stepC, pVf+MP4_MV_OFF_HP(mvC.dx, mvC.dy, stepC), stepC, &sadC, MP4_MV_ACC_HP_SAD(mvC.dx, mvC.dy));
                            bestSAD8x8 += sadC;
                        }
                    }
                }
            } else {
                mvLuma.dx = mvLuma.dy = 0;
            }
        }
        // field ME
        if (VOL.interlaced && (meData->flags & ME_FIELD) && (bestSAD16x16 > meData->thrDiff16x16) && (bestSAD8x8 > meData->thrDiff16x16)) {
            // cross-field ME is not implemented (top_field_ref = 0 && bottom_field_ref = 1 always)
            const int mb_ftfr = 0, mb_fbfr = 1;
            Ipp32s  tSAD, bSAD, mvw;
            mvLumaT.dx = mvLumaB.dx = mvPred[0].dx;
            mvLumaT.dy = mvLumaB.dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
            mvw = mp4_WeightMV(0, 0, mvLumaT, fcode, quant, 0);
            // SAD at (0,0)
            ippiSAD16x8_8u32s_C1R(pYc, stepL << 1, pYf, stepL << 1, &tSAD, IPPVC_MC_APX_FF);
            ippiSAD16x8_8u32s_C1R(pYc+stepL, stepL << 1, pYf+stepL, stepL << 1, &bSAD, IPPVC_MC_APX_FF);
            if (meData->flags & ME_USE_MVWEIGHT) {
                tSAD += mvw;
                bSAD += mvw;
            }
            if (bSAD + tSAD > meData->thrDiff16x8) {
                Ipp32s syT = meData->yT;
                Ipp32s syB = meData->yB;
                int sFlags = meData->flags;

                meData->flags &= ~ME_CHROMA;
                meData->numPred = 1;
                if (meData->flags & ME_FRAME) {
                    meData->mvPred[1] = mvLuma;
                    meData->numPred = 2;
                }
                meData->thrDiff = meData->thrDiff16x8;
                meData->sadFunc = ippiSAD16x8_8u32s_C1R;
                meData->copyQPFunc = ippiCopy16x8QP_MPEG4_8u_C1R;
                meData->copyHPFunc = ippiCopy16x8HP_8u_C1R;
                meData->yT = mp4_Div2(syT);
                meData->yB = mp4_Div2(syB);
                meData->stepL = stepL + stepL;
                meData->mvPred[0] = mvLumaT;
                meData->bestDiff = tSAD;
                mp4_ME_SAD(meData);
                mvLumaT.dx = (Ipp16s)meData->xPos;
                mvLumaT.dy = (Ipp16s)meData->yPos;
                tSAD = meData->bestDiff;
                meData->pYc += stepL;
                meData->pYr += stepL;
                meData->mvPred[0] = mvLumaB;
                meData->bestDiff = bSAD;
                mp4_ME_SAD(meData);
                mvLumaB.dx = (Ipp16s)meData->xPos;
                mvLumaB.dy = (Ipp16s)meData->yPos;
                bSAD = meData->bestDiff;
                meData->yT = syT;
                meData->yB = syB;
                meData->stepL = stepL;
                meData->pYc = pYc;
                meData->pYr = pYf;
                meData->flags = sFlags;
            } else {
                mvLumaT.dx = mvLumaT.dy = mvLumaB.dx = mvLumaB.dy = 0;
            }
            bestSAD16x8 = bSAD + tSAD;
            if (!(meData->flags & ME_USE_MVWEIGHT))
                bestSAD16x8 -= SAD_FAVOR_FIELD;
            if (meData->flags & RD_MODE_DECISION) {
                IppMotionVector mvTmpT, mvTmpB, mvChromaT, mvChromaB;
                if (qpel) {
                    mp4_Copy16x8QP_8u(pYf+stepL*mb_ftfr, stepL*2, mcPred, 32, &mvLumaT, rt);
                    mp4_Copy16x8QP_8u(pYf+stepL*mb_fbfr, stepL*2, mcPred+16, 32, &mvLumaB, rt);
                    mvTmpT.dx = (Ipp16s)mp4_Div2(mvLumaT.dx);
                    mvTmpT.dy = (Ipp16s)(mp4_Div2(mvLumaT.dy << 1) >> 1);
                    mvTmpB.dx = (Ipp16s)mp4_Div2(mvLumaB.dx);
                    mvTmpB.dy = (Ipp16s)(mp4_Div2(mvLumaB.dy << 1) >> 1);
                    mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                } else {
                    mp4_Copy16x8HP_8u(pYf+stepL*mb_ftfr, stepL*2, mcPred, 32, &mvLumaT, rt);
                    mp4_Copy16x8HP_8u(pYf+stepL*mb_fbfr, stepL*2, mcPred+16, 32, &mvLumaB, rt);
                    mp4_ComputeChromaMV(&mvLumaT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvLumaB, &mvChromaB);
                }
                mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*4, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*5, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                pattern = TransMacroBlockInter_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, mcPred, meData->yMB, meData->xMB, &dct_type, 0, &costInter16x8);
                costInter16x8 += RD_MUL * (3 + mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTER*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[pattern >> 2].len);
                mvTmpT.dx = mvPred[0].dx;
                mvTmpT.dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
                mvTmpB = mvTmpT;
                mp4_MV_GetDiff(&mvLumaT, &mvTmpT, fRangeMin, fRangeMax, fRange);
                costInter16x8 += RD_MUL * mp4_CalcBitsMV(&mvTmpT, fcode);
                mp4_MV_GetDiff(&mvLumaB, &mvTmpB, fRangeMin, fRangeMax, fRange);
                costInter16x8 += RD_MUL * mp4_CalcBitsMV(&mvTmpB, fcode);
            } else if (meData->flags & ME_CHROMA) {
                IppMotionVector mvChromaT, mvChromaB;
                Ipp32s          sadC;
                if (qpel) {
                    IppMotionVector mvTmpT, mvTmpB;
                    mvTmpT.dx = (Ipp16s)mp4_Div2(mvLumaT.dx);
                    mvTmpT.dy = (Ipp16s)(mp4_Div2(mvLumaT.dy << 1) >> 1);
                    mvTmpB.dx = (Ipp16s)mp4_Div2(mvLumaB.dx);
                    mvTmpB.dy = (Ipp16s)(mp4_Div2(mvLumaB.dy << 1) >> 1);
                    mp4_ComputeChromaMV(&mvTmpT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvTmpB, &mvChromaB);
                } else {
                    mp4_ComputeChromaMV(&mvLumaT, &mvChromaT);
                    mp4_ComputeChromaMV(&mvLumaB, &mvChromaB);
                }
                mp4_Copy8x4HP_8u(pUf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*4, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_ftfr ? stepC : 0), stepC*2, mcPred+64*5, 16, &mvChromaT, rt);
                mp4_Copy8x4HP_8u(pUf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*4+8, 16, &mvChromaB, rt);
                mp4_Copy8x4HP_8u(pVf+(mb_fbfr ? stepC : 0), stepC*2, mcPred+64*5+8, 16, &mvChromaB, rt);
                ippiSAD8x8_8u32s_C1R(pUc, stepC, mcPred+64*4, 8, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x8 += sadC;
                ippiSAD8x8_8u32s_C1R(pVc, stepC, mcPred+64*5, 8, &sadC, IPPVC_MC_APX_FF);
                bestSAD16x8 += sadC;
            }
        }
        if (meData->flags & RD_MODE_DECISION) {
            bestSADGMC = costGMC;
            bestSAD16x16 = costInter16x16;
            bestSAD8x8 = costInter8x8;
            bestSAD16x8 = costInter16x8;
            costInter = IPP_MIN(IPP_MIN(costGMC, costInter16x16), IPP_MIN(costInter8x8, costInter16x8));
        }
        if (bestSADGMC <= bestSAD16x16 && bestSADGMC <= bestSAD8x8 && bestSADGMC <= bestSAD16x8) {
            bestSAD = bestSADGMC + SAD_FAVOR_GMC;
            MBcurr->type = IPPVC_MBTYPE_INTER;
            MBcurr->mcsel = 1;
            ippiCalcGlobalMV_MPEG4(meData->xMB*16, meData->yMB*16, &mvGMC, mWarpSpec);
            MBcurr->mv[0] = MBcurr->mv[1] = MBcurr->mv[2] = MBcurr->mv[3] = mvGMC;
        } else {
            if ((bestSAD16x16 <= bestSAD8x8) && (bestSAD16x16 <= bestSAD16x8)) {
                bestSAD = bestSAD16x16;
                if (!(meData->flags & ME_USE_MVWEIGHT))
                    bestSAD += SAD_FAVOR_16x16;
                MBcurr->type = IPPVC_MBTYPE_INTER;
                MBcurr->mv[0] = MBcurr->mv[1] = MBcurr->mv[2] = MBcurr->mv[3] = mvLuma;
                mp4_MV_GetDiff(&mvLuma, &mvPred[0], fRangeMin, fRangeMax, fRange);
                MBcurr->mvDiff[0] = mvPred[0];
            } else if (bestSAD16x8 <= bestSAD8x8) {
                bestSAD = bestSAD16x8;
                if (!(meData->flags & ME_USE_MVWEIGHT))
                    bestSAD += SAD_FAVOR_FIELD;
                MBcurr->type = IPPVC_MBTYPE_INTER;
                MBcurr->mvT = mvLumaT; MBcurr->mvB = mvLumaB;
                MBcurr->mv[0].dx = MBcurr->mv[1].dx = MBcurr->mv[2].dx = MBcurr->mv[3].dx = (Ipp16s)mp4_Div2Round(mvLumaT.dx + mvLumaB.dx);
                MBcurr->mv[0].dy = MBcurr->mv[1].dy = MBcurr->mv[2].dy = MBcurr->mv[3].dy = (Ipp16s)(mvLumaT.dy + mvLumaB.dy);
                mvPred[0].dy = (Ipp16s)mp4_Div2(mvPred[0].dy);
                mvPred[2] = mvPred[0];
                mp4_MV_GetDiff(&mvLumaT, &mvPred[0], fRangeMin, fRangeMax, fRange);
                mp4_MV_GetDiff(&mvLumaB, &mvPred[2], fRangeMin, fRangeMax, fRange);
                MBcurr->mvDiff[0] = mvPred[0];
                MBcurr->mvDiff[2] = mvPred[2];
                MBcurr->fieldmc = 1;
            } else {
                bestSAD = bestSAD8x8;
                MBcurr->type = IPPVC_MBTYPE_INTER4V;
                // ME_SAD_8x8 writes resulting MVs in the MBcurr
                mp4_MV_GetDiff(&MBcurr->mv[0], &mvPred[0], fRangeMin, fRangeMax, fRange);
                mp4_MV_GetDiff(&MBcurr->mv[1], &mvPred[1], fRangeMin, fRangeMax, fRange);
                mp4_MV_GetDiff(&MBcurr->mv[2], &mvPred[2], fRangeMin, fRangeMax, fRange);
                mp4_MV_GetDiff(&MBcurr->mv[3], &mvPred[3], fRangeMin, fRangeMax, fRange);
                MBcurr->mvDiff[0] = mvPred[0];
                MBcurr->mvDiff[1] = mvPred[1];
                MBcurr->mvDiff[2] = mvPred[2];
                MBcurr->mvDiff[3] = mvPred[3];
            }
            MBcurr->mcsel = 0;
        }
        if (meData->flags & RD_MODE_DECISION) {
            int ac_pred_flag, pattern1, predDir[6], use_intra_dc_vlc = 1;

            costIntra = costInter;
            pattern = TransMacroBlockIntra_MPEG4(pYc, pUc, pVc, coeffMB, nzCount, quant, meData->yMB, meData->xMB, &dct_type, use_intra_dc_vlc, MBcurr, predDir, startRow, &ac_pred_flag, &pattern1, &costIntra);
            costIntra += RD_MUL * (1 + mp4_VLC_MCBPC_TB7[IPPVC_MBTYPE_INTRA*4+(pattern & 3)].len + mp4_VLC_CBPY_TB8[15-(pattern >> 2)].len);
            if (costIntra < costInter) {
                MBcurr->type = IPPVC_MBTYPE_INTRA;
                mp4_Zero4MV(MBcurr->mv);
                bestSAD = costIntra;
            } else {
                MBcurr->block[0].validPredIntra = MBcurr->block[1].validPredIntra = MBcurr->block[2].validPredIntra = MBcurr->block[3].validPredIntra = MBcurr->block[4].validPredIntra = MBcurr->block[5].validPredIntra = 0;
            }
        } else {
            sadFavorInter = SAD_FAVOR_INTER;
            //        if (meData->flags & ME_CHROMA)
            //          sadFavorInter += sadFavorInter >> 2;
            if (bestSAD > sadFavorInter) {
                // choose mbtype - Inter or Intra
                ippiMeanAbsDev16x16_8u32s_C1R(pYc, stepL, &dev);
                if (meData->flags & ME_CHROMA) {
                    Ipp32s  devC;
                    ippiMeanAbsDev8x8_8u32s_C1R(pUc, stepC, &devC);
                    dev += devC;
                    ippiMeanAbsDev8x8_8u32s_C1R(pVc, stepC, &devC);
                    dev += devC;
                }
                if (meData->xMB > 0)
                    if (MBcurr[-1].type == IPPVC_MBTYPE_INTRA)
                        dev -= DEV_FAVOR_INTRA;
                if (meData->yMB - startRow > 0)
                    if (MBcurr[-mNumMacroBlockPerRow].type == IPPVC_MBTYPE_INTRA)
                        dev -= DEV_FAVOR_INTRA;
                if (dev < bestSAD - sadFavorInter) {
                    MBcurr->type = IPPVC_MBTYPE_INTRA;
                    mp4_Zero4MV(MBcurr->mv);
                    bestSAD = dev;
MBcurr->mcsel = 0;
                }
            }
        }
        MBcurr->lumaErr = bestSAD;
    }
}

#ifdef _OMP_KARABAS
#define mp4_WaitInter(curRow) \
    if (curRow > 0) { \
        for (;;) { \
            Ipp32s volatile r = mCurRowMT[curRow - 1]; \
            if (j < r) \
                break; \
        } \
    }

void VideoEncoderMPEG4::ME_VOP()
{
    Ipp32s  i;

    for (i = 0; i < mNumMacroBlockPerCol; i ++)
        mCurRowMT[i] = -1;
    for (i = 0; i < mNumThreads; i ++)
        mSliceMT[i].numIntraMB = mSliceMT[i].numNotCodedMB = 0;
    i = 0;
#ifdef _OPENMP
#pragma omp parallel num_threads(mNumThreads) shared(i)
#endif
    {
        mp4_Data_ME meData;
        Ipp32s curRow = i, j;
        Ipp32s threadNum = mp4_MT_get_thread_num();

        meData.method = mMEmethod;
        meData.flags = mMEflags;
        meData.quant = VOP.vop_quant;
#ifdef USE_ME_SADBUFF
        meData.meBuff = mMEfastSAD + mMEfastSADsize * threadNum;
#endif
        meData.stepL = mStepLuma;
        meData.stepC = mStepChroma;
        if (meData.flags & ME_ZERO_MV) {
            meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = SAD_MAX;
        } else if (meData.flags & ME_USE_THRESHOLD) {
            meData.thrDiff16x16 = VOP.vop_quant >= 6 ? 256 : (4 << VOP.vop_quant);
            meData.thrDiff8x8 = meData.thrDiff16x16 >> 2;
            meData.thrDiff16x8 = meData.thrDiff16x16 >> 1;
        } else {
            meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = 0;
        }
        if (VOL.short_video_header) {
            meData.fcode = 1;
            meData.thrDiff = meData.thrDiff16x16;
            meData.sadFunc = ippiSAD16x16_8u32s;
            meData.copyHPFunc = ippiCopy16x16HP_8u_C1R;
        } else {
            meData.fcode = VOP.vop_fcode_forward;
        }
        while (curRow < mNumMacroBlockPerCol) {
            curRow = i;
            i ++;
            if (curRow < mNumMacroBlockPerCol) {
                meData.MBcurr = MBinfo + curRow * mNumMacroBlockPerRow;
                meData.pYc = mFrameC->pY + curRow * 16 * mStepLuma;
                meData.pUc = mFrameC->pU + curRow * 8 * mStepChroma;
                meData.pVc = mFrameC->pV + curRow * 8 * mStepChroma;
                meData.pYr = mFrameF->pY + curRow * 16 * mStepLuma;
                meData.pUr = mFrameF->pU + curRow * 8 * mStepChroma;
                meData.pVr = mFrameF->pV + curRow * 8 * mStepChroma;
                meData.yMB = curRow;
                if (VOL.short_video_header) {
                    meData.yT = -IPP_MIN(curRow * 16, mPVOPsearchVer);
                    meData.yB =  IPP_MIN((mNumMacroBlockPerCol - curRow - 1) * 16, mPVOPsearchVer);
                    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
                        meData.xMB = j;
                        meData.xL = -IPP_MIN(j * 16, mPVOPsearchHor);
                        meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j - 1) * 16, mPVOPsearchHor);
                        mp4_WaitInter(curRow);
                        ME_MacroBlock_PSH(&meData, 0, mNumMacroBlockPerCol - 1);
                        if (meData.MBcurr->type >= IPPVC_MBTYPE_INTRA)
                            mSliceMT[threadNum].numIntraMB ++;
                        meData.pYc += 16; meData.pUc += 8; meData.pVc += 8;
                        meData.pYr += 16; meData.pUr += 8; meData.pVr += 8;
                        meData.MBcurr ++;
                        mCurRowMT[curRow] ++;
                    }
                } else {
                    meData.yT = -IPP_MIN(curRow * 16 + 16, mPVOPsearchVer);
                    meData.yB =  IPP_MIN((mNumMacroBlockPerCol - curRow) * 16, mPVOPsearchVer);
                    for (j = 0; j < mNumMacroBlockPerRow; j ++) {
                        meData.xMB = j;
                        meData.xL = -IPP_MIN(j * 16 + 16, mPVOPsearchHor);
                        meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mPVOPsearchHor);
                        mp4_WaitInter(curRow);
                        if (VOP.vop_coding_type == MP4_VOP_TYPE_P) {
                            ME_MacroBlock_P(&meData, 0, mNumMacroBlockPerCol - 1);
                            if (meData.MBcurr->not_coded)
                                mSliceMT[threadNum].numNotCodedMB ++;
                        } else
                            ME_MacroBlock_S(&meData, 0, mNumMacroBlockPerCol - 1);
                        if (meData.MBcurr->type >= IPPVC_MBTYPE_INTRA)
                            mSliceMT[threadNum].numIntraMB ++;
                        meData.pYc += 16; meData.pUc += 8; meData.pVc += 8;
                        meData.pYr += 16; meData.pUr += 8; meData.pVr += 8;
                        meData.MBcurr ++;
                        mCurRowMT[curRow] ++;
                    }
                }
                mCurRowMT[curRow] ++;
            }
            curRow ++;
        }
    }
    mNumIntraMB = mSliceMT[0].numIntraMB;
    mNumNotCodedMB = mSliceMT[0].numNotCodedMB;
    for (i = 1; i < mNumThreads; i ++) {
        mNumIntraMB += mSliceMT[i].numIntraMB;;
        mNumNotCodedMB += mSliceMT[i].numNotCodedMB;
    }
}
#endif // _OMP_KARABAS

void VideoEncoderMPEG4::ME_Slice(mp4_Slice *slice)
{
    mp4_Data_ME meData;
    Ipp32s      i, j, startRow;

    meData.rt = VOP.vop_rounding_type;
    startRow = slice->startRow;
    meData.method = mMEmethod;
    meData.flags = mMEflags;
    meData.quant = VOP.vop_quant;
    meData.MBcurr = MBinfo + slice->startRow * mNumMacroBlockPerRow;
#ifdef USE_ME_SADBUFF
    meData.meBuff = slice->meBuff;
#endif
    meData.stepL = mStepLuma;
    meData.stepC = mStepChroma;
    if (meData.flags & ME_ZERO_MV) {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = SAD_MAX;
    } else if (meData.flags & ME_USE_THRESHOLD) {
        meData.thrDiff16x16 = VOP.vop_quant >= 6 ? 256 : (4 << VOP.vop_quant);
        meData.thrDiff8x8 = meData.thrDiff16x16 >> 2;
        meData.thrDiff16x8 = meData.thrDiff16x16 >> 1;
    } else {
        meData.thrDiff16x16 = meData.thrDiff8x8 = meData.thrDiff16x8 = 0;
    }
    if (VOL.short_video_header) {
        meData.fcode = 1;
        meData.thrDiff = meData.thrDiff16x16;
        meData.sadFunc = ippiSAD16x16_8u32s;
        meData.copyHPFunc = ippiCopy16x16HP_8u_C1R;
    } else {
        meData.fcode = VOP.vop_fcode_forward;
    }
    for (i = startRow; i < startRow + slice->numRow; i ++) {
        meData.pYc = mFrameC->pY + i * 16 * mStepLuma;
        meData.pUc = mFrameC->pU + i * 8 * mStepChroma;
        meData.pVc = mFrameC->pV + i * 8 * mStepChroma;
        meData.pYr = mFrameF->pY + i * 16 * mStepLuma;
        meData.pUr = mFrameF->pU + i * 8 * mStepChroma;
        meData.pVr = mFrameF->pV + i * 8 * mStepChroma;
        meData.yMB = i;
        if (VOL.short_video_header) {
            meData.yT = -IPP_MIN(i * 16, mPVOPsearchVer);
            meData.yB =  IPP_MIN((mNumMacroBlockPerCol - i - 1) * 16, mPVOPsearchVer);
            for (j = 0; j < mNumMacroBlockPerRow; j ++) {
                meData.xMB = j;
                meData.xL = -IPP_MIN(j * 16, mPVOPsearchHor);
                meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j - 1) * 16, mPVOPsearchHor);
                ME_MacroBlock_PSH(&meData, startRow, startRow + slice->numRow - 1);
                if (meData.MBcurr->type >= IPPVC_MBTYPE_INTRA) {
                    slice->numIntraMB ++;
                    if (slice->numIntraMB  > mSceneChangeThreshold)
                        return;
                }
                meData.pYc += 16; meData.pUc += 8; meData.pVc += 8;
                meData.pYr += 16; meData.pUr += 8; meData.pVr += 8;
                meData.MBcurr ++;
            }
        } else {
            meData.yT = -IPP_MIN(i * 16 + 16, mPVOPsearchVer);
            meData.yB =  IPP_MIN((mNumMacroBlockPerCol - i) * 16, mPVOPsearchVer);
            for (j = 0; j < mNumMacroBlockPerRow; j ++) {
                meData.xMB = j;
                meData.xL = -IPP_MIN(j * 16 + 16, mPVOPsearchHor);
                meData.xR =  IPP_MIN((mNumMacroBlockPerRow - j) * 16, mPVOPsearchHor);
                if (VOP.vop_coding_type == MP4_VOP_TYPE_P) {
                    ME_MacroBlock_P(&meData, startRow, startRow + slice->numRow - 1);
                    if (meData.MBcurr->not_coded)
                        slice->numNotCodedMB ++;
                } else
                    ME_MacroBlock_S(&meData, startRow, startRow + slice->numRow - 1);
                if (meData.MBcurr->type >= IPPVC_MBTYPE_INTRA) {
                    slice->numIntraMB ++;
                    if (slice->numIntraMB  > mSceneChangeThreshold)
                        return;
                }
                meData.pYc += 16; meData.pUc += 8; meData.pVc += 8;
                meData.pYr += 16; meData.pUr += 8; meData.pVr += 8;
                meData.MBcurr ++;
            }
        }
    }
}

} //namespace MPEG4_ENC

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
