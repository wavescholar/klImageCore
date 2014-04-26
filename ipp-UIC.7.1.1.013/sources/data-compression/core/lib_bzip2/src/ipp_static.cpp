/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Ipp_bzip2 Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
*/

#include "ippcore.h"

extern "C" {
  int link_to_ipp_libbz2;
}

struct IPP_BZ2_STATIC_INIT {
  int state;
  IPP_BZ2_STATIC_INIT(int state_init){this->state = state_init; ippStaticInit();}
};

IPP_BZ2_STATIC_INIT IPP_BZ2_X(link_to_ipp_libbz2);
