//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef __UMC_H264_CONFIG__
#define __UMC_H264_CONFIG__

#define BITDEPTH_9_12
#define FRAME_INTERPOLATION
#define ALT_BITSTREAM_ALLOC
//#define FRAME_TYPE_DETECT_DS  //Frame type detector with downsampling
//#define FRAME_QP_FROM_FILE "fqp.txt"


#if defined (_OPENMP)
#include "omp.h"

#define FRAMETYPE_DETECT_ST             // Threaded frame type detector for slice threading
//#define DEBLOCK_THREADING               // Threaded deblocking for slice threading. Warning, found to be unstable!
#define EXPAND_PLANE_THREAD             // Parallel expand plane for chroma
#define INTERPOLATE_FRAME_THREAD        // Parallel frame pre interpolation
#define SLICE_THREADING_LOAD_BALANCING  // Redistribute macroblocks between slices based on previously encoded frame timings
#endif

#if defined(__INTEL_COMPILER) || (_MSC_VER >= 1300) || (defined(__GNUC__) && (__GNUC__ > 3) && defined(__SSE2__))
#define INTRINSIC_OPT
#include "emmintrin.h"
#endif
#endif
