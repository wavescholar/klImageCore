/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "aaccmn_const.h"
#include "aaccmn_chmap.h"
#include "mp4cmn_pce.h"
#include "aaccmn_adts.h"

Ipp32s chmap_create_by_pce(sProgram_config_element* p_pce,
                           sCh_map_item*            chmap)
{
  Ipp32s i;
  Ipp32s n;
  Ipp32s tag;
  Ipp32s ch;
  Ipp32s id;
  Ipp32s numElem = 0;

  if(p_pce->num_front_channels >= MAX_CHANNELS_ELEMENTS)
      return -1;
  if(p_pce->num_back_channel_elements >= MAX_CHANNELS_ELEMENTS)
      return -1;
  if(p_pce->num_side_channel_elements >= MAX_CHANNELS_ELEMENTS)
      return -1;
  if(p_pce->num_lfe_channel_elements >= MAX_CHANNELS_ELEMENTS)
      return -1;

  /**********************************************************
                      Front Channels
    **********************************************************/
  i = 0;
  n = p_pce->num_front_channels;
  if (n & 1) {
    if (numElem >= CH_MAX) return -1;
    n--;
    tag = p_pce->front_element_tag_select[i];
    chmap[numElem].id  = (Ipp16s)ID_SCE;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)CH_FRONT_CENTER;
    i++;
    numElem++;
  }

  ch = CH_FRONT_LEFT;
  for (;i < p_pce->num_front_channel_elements; i ++) {
    if (numElem >= CH_MAX) return -1;
    tag = p_pce->front_element_tag_select[i];
    id = p_pce->front_element_is_cpe[i];
    chmap[numElem].id  = (Ipp16s)id;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)ch;
    ch += (1+id);
    numElem++;
  }

  /**********************************************************
                      Back Channels
  **********************************************************/
  i = 0;
  /*
  n = p_pce->num_back_channels;
  if (n & 1) {
    if (numElem >= CH_MAX) return -1;
    n--;
    tag = p_pce->back_element_tag_select[i];
    chmap[numElem].id  = (Ipp16s)ID_SCE;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)CH_BACK_CENTER;
    i++;
    numElem++;
  }*/
  ch = CH_BACK_LEFT;
  for (;i < p_pce->num_back_channel_elements; i ++) {
    if (numElem >= CH_MAX) return -1;
    tag = p_pce->back_element_tag_select[i];
    id = p_pce->back_element_is_cpe[i];
    chmap[numElem].id  = (Ipp16s)id;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)ch;
    ch += (1+id);
    numElem++;
  }
  /**********************************************************
                      Side Channels
  **********************************************************/
  i = 0;
  ch = CH_SIDE_LEFT;
  for (;i < p_pce->num_side_channel_elements; i ++) {
    if (numElem >= CH_MAX) return -1;
    tag = p_pce->side_element_tag_select[i];
    id = p_pce->side_element_is_cpe[i];
    chmap[numElem].id  = (Ipp16s)id;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)ch;
    ch += (1+id);
    numElem++;
  }
  /**********************************************************
                      LF Channels
  **********************************************************/
  i = 0;
  ch = CH_LOW_FREQUENCY;
  for (;i < p_pce->num_lfe_channel_elements; i ++) {
    if (numElem >= CH_MAX) return -1;
    tag = p_pce->lfe_element_tag_select[i];
    chmap[numElem].id  = (Ipp16s)ID_LFE;
    chmap[numElem].tag = (Ipp16s)tag;
    chmap[numElem].key = (Ipp16s)ch;
    ch++;
    numElem++;
  }
  chmap[0].numElem = (Ipp16s)numElem;
  return 0;
}

/********************************************************************/

Ipp32s chmap_create_by_adts(Ipp32s channel_configuration,
                            sCh_map_item* chmap,
                            sEl_map_item* elmap,
                            Ipp32s el_num)
{
  sEl_map_item local_elmap[10];
  Ipp32s i, j;
  Ipp32s numElem = 0;
  Ipp32s idx;

  if ((el_num > 10) || (channel_configuration == 0 )) return -1;
  if (channel_configuration > 7) return -1;

  for( i = 0; i < el_num; i ++) {
    local_elmap[i] = elmap[i];
  }
  for(; i < 10; i ++) {
    local_elmap[i].id = -1;
  }

  if (channel_configuration == 7)
    channel_configuration = 8;

  if (channel_configuration > 5) {
    idx = -1;
    for (i = 0; i < el_num; i++) {
      if (ID_LFE == local_elmap[i].id) {
        idx = i;
        break;
      }
    }

    if (idx == -1) {
      return -1;
    }

    chmap[numElem].id  = elmap[idx].id;
    chmap[numElem].tag = elmap[idx].tag;
    chmap[numElem].key = (Ipp16s)CH_LOW_FREQUENCY;
    numElem++;
    channel_configuration--;
  }

  if (channel_configuration != 2) {
    idx = -1;
    for (i = 0; i < el_num; i++) {
      if (ID_SCE == local_elmap[i].id) {
        idx = i;
        break;
      }
    }

    if (idx == -1) {
      return -1;
    }

    chmap[numElem].id  = local_elmap[idx].id;
    chmap[numElem].tag = local_elmap[idx].tag;
    chmap[numElem].key = (Ipp16s)CH_FRONT_CENTER;
    numElem++;
    channel_configuration--;
    local_elmap[idx].id = -1;
  }

  for (j = 0; j < channel_configuration - 1; j+=2) {
    idx = -1;
    for (i = 0; i < el_num; i++) {
      if (ID_CPE == local_elmap[i].id) {
        idx = i;
        break;
      }
    }

    if (idx == -1) {
      return -1;
    }

    chmap[numElem].id  = local_elmap[idx].id;
    chmap[numElem].tag = local_elmap[idx].tag;
    chmap[numElem].key = (Ipp16s)(CH_FRONT_LEFT + j);
    numElem++;
    local_elmap[idx].id = -1;
  }

  if (channel_configuration & 1) {
    idx = -1;
    for (i = 0; i < el_num; i++) {
      if (ID_SCE == local_elmap[i].id) {
        idx = i;
        break;
      }
    }

    if (idx == -1) {
      return -1;
    }

    chmap[numElem].id  = local_elmap[idx].id;
    chmap[numElem].tag = local_elmap[idx].tag;
    chmap[numElem].key = (Ipp16s)CH_SURROUND_CENTER;
    numElem++;
  }

  chmap[0].numElem = (Ipp16s)numElem;
  return 0;
}

/********************************************************************/

Ipp32s chmap_order(sCh_map_item* chmap,
                   sEl_map_item* elmap,
                   Ipp32s        el_num,
                   Ipp32s*       order)
{
  Ipp32s i;
  Ipp32s j;
  Ipp32s id;
  Ipp32s tag;
  Ipp32s key[CH_MAX];
  Ipp32s ch;
  Ipp32s exch;
  Ipp32s numElem = chmap[0].numElem;
  Ipp32s idx;

  ch = 0;
  for (i = 0; i < el_num; i ++) {
    id  = elmap[i].id;
    tag  = elmap[i].tag;
    if (ID_CCE == id)
        continue;

    idx = -1;
    for (j = 0; j < numElem; j++) {
      if ((chmap[j].id == id) && (chmap[j].tag == tag)) {
        idx = j;
        break;
      }
    }

    if (-1 == idx)
        continue;

    key[ch] = chmap[idx].key;
    order[ch] = elmap[i].ch;
    ch++;

    if (ID_CPE == id) {
      key[ch] = chmap[idx].key + 1;
      order[ch] = elmap[i].ch + 1;
      ch++;
    }
  }

  /// Sort
  for (i = 0; i < ch; i ++) {
    for (j = i; j < ch; j ++) {
      if (key[j] < key[i]) {
        exch = key[j];
        key[j] = key[i];
        key[i] = exch;

        exch = order[j];
        order[j] = order[i];
        order[i] = exch;
      }
    }
  }
  return ch;
}

#endif //UMC_ENABLE_XXX
