/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __CODEC_SELECTOR_H__
#define __CODEC_SELECTOR_H__

#include "umc_data_reader.h"
#include "umc_splitter.h"
#include "umc_audio_decoder.h"
#include "umc_dual_thread_codec.h"
#include "umc_audio_render.h"
#include "umc_video_decoder.h"
#include "umc_video_render.h"
#include "umc_video_processing.h"

#include "umc_app_pipeline.h"

using namespace UMC;


Status InitDataReader(BasePipelineParams *pParams, DataReader **ppDataReader);
Status InitSplitter(BasePipelineParams *pParams, DataReader *pDataReader, Splitter **ppSplitter);
Status InitVideoDecoder(BasePipelineParams *pParams, VideoStreamInfo *pVideoInfo, MediaData *pDecSpec, BaseCodec *pPostProcessing, VideoDecoder **ppVideoDecoder);
Status InitVideoRender(BasePipelineParams *pParams, VideoStreamInfo *pVideoInfo, VideoRender **pVideoRender);
Status InitAudioDecoder(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, MediaData *pDecSpec, AudioDecoder **ppAudioDecoder);
Status InitDTAudioDecoder(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, MediaData *pDecSpec, MediaBuffer **ppMediaBuffer, AudioDecoder **ppAudioDecoder, DualThreadedCodec **ppDTAudioCodec);
Status InitAudioRender(BasePipelineParams *pParams, AudioStreamInfo *pAudioInfo, AudioRender **ppAudioRender);

#endif
