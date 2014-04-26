/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PIXMAP_H__
#define __PIXMAP_H__

#include <QPixmap>

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


QPixmap CreateQPixmap(Ipp8u* pSrc, IppiSize size, int nchannels);
QPixmap CreateQPixmap(CIppImage& cimage);

#endif
