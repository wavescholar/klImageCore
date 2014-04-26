/* //////////////////////////////// "all.h" /////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
*/

#if !defined( __ALL_H__ )

#define __ALL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ippdc.h"
#include "ippcore.h"
#include "ipps.h"

#include "bwt.h"
#include "rle.h"
#include "mtf.h"
#include "huffman.h"
#include "git.h"
#include "zlib.h"

#define BLOCKUNIT            102400
#define MAXBLOCKSIZE         9 * BLOCKUNIT
#define HEADERBYTES          4
#define GZIPBLOCKSIZE        65536
#define DEFAULTGITSTRATEGY   ippGITLeftReorder
#define GZIPNORMAL           1
#define GZIPAVERAGE          6
#define GZIPBEST             9
#define LZSSBLOCKSIZE        16384
#define BWT   1
#define GIT   2
#define LZSS  3
#define GZIP  4
#define HUFFMAN 5

#endif /* __ALL_H__ */
