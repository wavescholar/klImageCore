/* //////////////////////////////// "git.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __GIT_H__ )

#define __GIT_H__

#include "all.h"

int EncodeGIT( Ipp8u *, int , Ipp8u *, int *, IppGITState_8u *, IppGITStrategyHint );
int DecodeGIT( Ipp8u *, int , Ipp8u *, int *, IppGITState_8u *, IppGITStrategyHint );

#endif /* __GIT_H__ */
