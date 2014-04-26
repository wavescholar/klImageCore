/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACCMN_CHMAP_H
#define __AACCMN_CHMAP_H

#include "mp4cmn_pce.h"
#include "aaccmn_adts.h"

enum eChMapOrder{
    CH_START            = 0,    /// Start offset
    CH_LIMIT            = 0x20, /// Max number of channel for each type
  /* Channels order */
    CH_FRONT            = 0,
    CH_SIDE,
    CH_BACK,
    CH_LF,
    /********************/
    CH_FRONT_CENTER     = CH_START + CH_FRONT*CH_LIMIT,
    CH_FRONT_LEFT,
//    CH_FRONT_LEFT       = CH_START + CH_FRONT*CH_LIMIT,
    CH_FRONT_RIGHT,
//    CH_FRONT_CENTER     = CH_START + (CH_FRONT+1)*CH_LIMIT - 1,
    /********************/
    CH_SIDE_LEFT        = CH_START + CH_SIDE*CH_LIMIT,
  CH_SIDE_RIGHT,
    /********************/
    CH_BACK_LEFT        = CH_START + CH_BACK*CH_LIMIT,
    CH_BACK_RIGHT,
    CH_BACK_CENTER      = CH_START + (CH_BACK+1)*CH_LIMIT - 1,
    /********************/
    CH_LOW_FREQUENCY    = CH_START + CH_LF*CH_LIMIT,

    CH_SURROUND_LEFT    = CH_BACK_LEFT,
    CH_SURROUND_RIGHT   = CH_BACK_RIGHT,
    CH_SURROUND_CENTER  = CH_BACK_CENTER,

    CH_OUTSIDE_LEFT     = CH_SIDE_LEFT,
    CH_OUTSIDE_RIGHT    = CH_SIDE_RIGHT,

/*
  CH_TOP_CENTER,
  CH_TOP_FRONT_LEFT,
  CH_TOP_FRONT_CENTER,
  CH_TOP_FRONT_RIGHT,
  CH_TOP_BACK_LEFT,
  CH_TOP_BACK_CENTER,
  CH_TOP_BACK_RIGHT,
*/
  CH_RESERVED,
  CH_MAX                = 48,
  COUPL_CH_MAX          = 2
};

enum eChMap
{
  EL_ID_MAX    = 4, /* Max elements id that is used in channel map*/
  EL_TAG_MAX   = 48,
  EL_MAP_SIZE  = 48
};

typedef struct
{
    Ipp16s      id;
    Ipp16s      tag;
    Ipp16s      key;
    Ipp16s      numElem;
} sCh_map_item;

typedef struct
{
    Ipp16s      id;
    Ipp16s      tag;
    Ipp16s      ch;
} sEl_map_item;

#ifdef  __cplusplus
extern "C" {
#endif

  Ipp32s chmap_create_by_pce(sProgram_config_element* p_pce,
                             sCh_map_item*            chmap);
  Ipp32s chmap_create_by_adts(Ipp32s        channel_configuration,
                              sCh_map_item* chmap,
                              sEl_map_item* elmap,
                              Ipp32s        el_num);
  Ipp32s chmap_order(sCh_map_item* chmap,
                     sEl_map_item* elmap,
                     Ipp32s        el_num,
                     Ipp32s*       order);

#ifdef  __cplusplus
}
#endif

#endif//__AACCMN_CHMAP_H
