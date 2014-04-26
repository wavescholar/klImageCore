/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PICNIC_PRECOMP_H__
#define __PICNIC_PRECOMP_H__

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef linux
#include <time.h>
#endif

#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipp.h"

#include "jpegbase.h"
#include "uic_base_stream_input.h"
#include "uic_base_stream_output.h"

#include "stdfilein.h"
#include "stdfileout.h"
#include "membuffin.h"

#include "uic_bmp_dec.h"
#include "uic_bmp_enc.h"

#include "metadata.h"
#include "jpegenc.h"
#include "jpegdec.h"
#include "uic_jpeg_dec.h"
#include "uic_jpeg_enc.h"

#include "uic_jpeg2000_dec.h"
#include "uic_jp2_dec.h"
#include "uic_jp2_enc.h"

#include "ippimage.h"
#include "bmp.h"
#include "jpeg.h"
#include "jpeg2k.h"
#include "dicom.h"
#include "raw.h"
#include "detector.h"

#endif
