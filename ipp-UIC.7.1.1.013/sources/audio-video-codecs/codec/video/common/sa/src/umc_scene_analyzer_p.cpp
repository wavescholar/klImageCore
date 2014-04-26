/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_scene_analyzer_p.h"

namespace UMC
{

SceneAnalyzerP::SceneAnalyzerP(void)
{
    m_pCur = (SceneAnalyzerFrame *) 0;
    m_pRef = (SceneAnalyzerFrame *) 0;

    memset(&m_history, 0, sizeof(m_history));
    m_framesInHistory = 0;

} // SceneAnalyzerP::SceneAnalyzerP(void)

SceneAnalyzerP::~SceneAnalyzerP(void)
{
    // release the object
    Close();

} // SceneAnalyzerP::~SceneAnalyzerP(void)

Status SceneAnalyzerP::Close(void)
{
    if (m_pCur)
    {
        delete m_pCur;
    }
    if (m_pRef)
    {
        delete m_pRef;
    }
    m_pCur = (SceneAnalyzerFrame *) 0;
    m_pRef = (SceneAnalyzerFrame *) 0;

    // call the parent's method
    SceneAnalyzerBase::Close();

    memset(&m_history, 0, sizeof(m_history));
    m_framesInHistory = 0;

    return UMC_OK;

} // Status SceneAnalyzerP::Close(void)

Status SceneAnalyzerP::Init(BaseCodecParams *pParams)
{
    const SceneAnalyzerParams *pSAParams = DynamicCast<SceneAnalyzerParams, BaseCodecParams> (pParams);
    Status umcRes;

    // check error(s)
    if (NULL == pSAParams)
        return UMC_ERR_NULL_PTR;

    // release the object before initialization
    Close();

    // call the parent's method
    umcRes = SceneAnalyzerBase::Init(pParams);
    if (UMC_OK != umcRes)
        return umcRes;

    // allocate being analyzed frames
    m_pCur = new SceneAnalyzerFrame();
    if (NULL == m_pCur)
    {
        return UMC_ERR_ALLOC;
    }
    m_pRef = new SceneAnalyzerFrame();
    if (NULL == m_pRef)
    {
        return UMC_ERR_ALLOC;
    }

    return UMC_OK;

} // Status SceneAnalyzerP::Init(BaseCodecParams *pParams)

Status SceneAnalyzerP::GetFrame(MediaData *pSource, MediaData *pDestination)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData> (pSource);
    VideoData *pDst = DynamicCast<VideoData, MediaData> (pDestination);
    FrameType plannedFrameType, dstFrameType;
    PictureStructure dstFrameStructure;
    Status umcRes;

    // check error(s)
    if (NULL == pDst)
    {
        return UMC_ERR_NULL_PTR;
    }
    if (NULL == pSrc)
    {
        return UMC_ERR_NOT_ENOUGH_DATA;
    }
    if (NULL == m_pCur)
    {
        return UMC_ERR_NOT_INITIALIZED;
    }

    // if destination has incorrect type
    if (false == CheckSupportedColorFormat(pSrc))
    {
        return SceneAnalyzerBase::GetFrame(pSource, pDestination);
    }

    // initialize the current being analyzed frame
    umcRes = m_pCur->SetSource(pSrc, m_params.m_interlaceType);
    if (UMC_OK != umcRes)
        return umcRes;

    // get planned frame type
    plannedFrameType = GetPlannedFrameType();

    // we do I picture analysis only in the beggining.
    // In other cases we do P picture analysis to track
    // color and deviations. All statistic is reset,
    // when the real scene changing happens.

    if (0 == m_frameCount)
    {
        // set the initial value
        dstFrameType = I_PICTURE;

        // perform the analysis
        umcRes = AnalyzeFrame(m_pCur);
        if (UMC_OK != umcRes)
            return umcRes;
        UpdateHistory(m_pCur, I_PICTURE);

        // set the destination video data
        InitializeVideoData(pDst, m_pCur);

        // set the destination frame parameters
        dstFrameStructure = m_pCur->m_frameEstimation;
    }
    else
    {
        // set the initial value
        dstFrameType = plannedFrameType;
        // we support only I & P frames in this class
        if (I_PICTURE != dstFrameType)
        {
            dstFrameType = P_PICTURE;
        }

        // perform the analysis
        umcRes = AnalyzeFrame(m_pRef, m_pCur);
        if (UMC_OK != umcRes)
            return umcRes;
        UpdateHistory(m_pCur, P_PICTURE);

        // reset counter in case of scene changing
        if (m_pCur->m_bChangeDetected)
        {
            // reset the history
            m_framesInHistory = 0;

            dstFrameType = I_PICTURE;
        }

        // reset scene frame counter
        if (I_PICTURE == dstFrameType)
        {
            // reset frame counter
            m_frameNum = 0;
        }

        // set the destination frame parameters
        dstFrameStructure = m_pCur->m_frameEstimation;
    }

    // set picture's parameters
    pDst->m_frameType = dstFrameType;
    switch (dstFrameStructure)
    {
    case PS_TOP_FIELD_FIRST:
        pDst->m_picStructure = PS_TOP_FIELD_FIRST;
        break;

    case PS_BOTTOM_FIELD_FIRST:
        pDst->m_picStructure = PS_BOTTOM_FIELD_FIRST;
        break;

    default:
        pDst->m_picStructure = PS_PROGRESSIVE;
        break;
    }

    // exchange the current & the previous frame
    {
        SceneAnalyzerFrame *pTemp;

        pTemp = m_pRef;
        m_pRef = m_pCur;
        m_pCur = pTemp;
    }

    // increment frame counter(s)
    m_frameCount += 1;
    m_frameNum += 1;

    return UMC_OK;

} // Status SceneAnalyzerP::GetFrame(MediaData *pSource, MediaData *pDestination)

Status SceneAnalyzerP::InitializeVideoData(VideoData *pDst, const SceneAnalyzerFrame *pFrame)
{
    ColorFormat colorFormat = pFrame->m_colorFormat;
    Ipp32s srcHeight = pFrame->m_picDim.height;
    Ipp32s srcWidth = pFrame->m_picDim.width;
    Ipp32u planeNum, maxPlaneNum;
    Status umcRes;

    umcRes = pDst->Init(srcWidth, srcHeight, colorFormat);
    if (UMC_OK != umcRes)
        return umcRes;

    // initialize the destination video data
    // with the pointers of saved data.
    // We avoid data copying.

    maxPlaneNum = pDst->GetPlanesNumber();
    for (planeNum = 0; planeNum < maxPlaneNum; planeNum += 1)
    {
        // set destination plane
        pDst->SetPlaneDataPtr((void *) pFrame->m_pPic[planeNum],
                              planeNum);
        pDst->SetPlanePitch(pFrame->m_picStep,
                            planeNum);
    }

    return UMC_OK;

} // Status SceneAnalyzerP::InitializeVideoData(VideoData *pDst, const SceneAnalyzerFrame *pFrame)

void SceneAnalyzerP::UpdateHistory(const SceneAnalyzerFrame *pFrame, FrameType analysisType)
{
    Ipp32u i;

    for (i = sizeof(m_history) / sizeof(m_history[0]) - 1; 0 < i; i -= 1)
    {
        m_history[i] = m_history[i - 1];
    }

    m_history[0].info = pFrame->m_scaledPic.m_info;
    m_history[0].analyzedFrameType = analysisType;
    m_history[0].m_bChangeDetected = pFrame->m_bChangeDetected;

    m_framesInHistory = IPP_MIN(8, m_framesInHistory + 1);

} // void SceneAnalyzerP::UpdateHistory(const SceneAnalyzerFrame *pFrame, FrameType analysisType)

void SceneAnalyzerP::MakePPictureCodingDecision(SceneAnalyzerPicture *pRef,
                                                SceneAnalyzerPicture *pSrc)
{
    bool bChangeDetected = false;

    if (false == m_history[0].m_bChangeDetected)
    {
        Ipp32s availableFrames = m_framesInHistory;
        Ipp32u color, minColor, maxColor;
        Ipp32u intraDev, minIntraDev, maxIntraDev;
        Ipp32u interDev, minInterDev, maxInterDev;
        Ipp32u interDevEst, minInterDevEst, maxInterDevEst;
        Ipp32u bestMatches, maxBestMathes, minBestMatches;
        Ipp32s i;
        Ipp32u conditions = 0;

        // set initial values
        color = pSrc->m_info.averageDev[SA_COLOR];
        intraDev = pSrc->m_info.averageDev[SA_INTRA];
        interDev = pSrc->m_info.averageDev[SA_INTER];
        interDevEst = pSrc->m_info.averageDev[SA_INTER_ESTIMATED];
        bestMatches = pSrc->m_info.bestMatches;

        minColor = maxColor = m_history[0].info.averageDev[SA_COLOR];
        minIntraDev = maxIntraDev = m_history[0].info.averageDev[SA_INTRA];
        minInterDev = maxInterDev = m_history[0].info.averageDev[SA_INTER];
        minInterDevEst = maxInterDevEst = m_history[0].info.averageDev[SA_INTER_ESTIMATED];
        minBestMatches = maxBestMathes = m_history[0].info.bestMatches;

        // select boundaries
        for (i = 1; i < availableFrames; i += 1)
        {
            minColor = IPP_MIN(minColor, m_history[i].info.averageDev[SA_COLOR]);
            maxColor = IPP_MAX(maxColor, m_history[i].info.averageDev[SA_COLOR]);
            minIntraDev = IPP_MIN(minIntraDev, m_history[i].info.averageDev[SA_INTRA]);
            maxIntraDev = IPP_MAX(maxIntraDev, m_history[i].info.averageDev[SA_INTRA]);
            // there is not INTER data in I frames,
            // and we skip still pictures
            if (false == m_history[i].m_bChangeDetected)
            {
                minInterDev = IPP_MIN(minInterDev, m_history[i].info.averageDev[SA_INTER]);
                maxInterDev = IPP_MAX(maxInterDev, m_history[i].info.averageDev[SA_INTER]);
                minInterDevEst = IPP_MIN(minInterDevEst, m_history[i].info.averageDev[SA_INTER_ESTIMATED]);
                maxInterDevEst = IPP_MAX(maxInterDevEst, m_history[i].info.averageDev[SA_INTER_ESTIMATED]);
            }
        }

        // check conditions
        if ((color >= minColor) &&
            (color <= maxColor))
        {
            conditions += 1;
        }
        if ((intraDev >= minIntraDev) &&
            (intraDev <= maxIntraDev))
        {
            conditions += 1;
        }
        if (interDev <= maxInterDev)
        {
            conditions += 1;
        }
        if (interDevEst <= maxInterDevEst)
        {
            conditions += 1;
        }
        if (bestMatches >= minBestMatches)
        {
            conditions += 1;
        }

        if (3 <= conditions)
        {
            bChangeDetected = false;
        }
        // probably it is a frame change
        else if (0 != conditions)
        {
            if ((4 <= (Ipp32s) (minIntraDev - intraDev)) &&
                (7 <= (Ipp32s) (interDev - maxInterDev)))
            {
                bChangeDetected = true;
            }
            else if ((interDevEst > maxInterDevEst) &&
                     (interDevEst * 2 > intraDev))
            {
                if ((interDev > maxInterDev + 5) ||
                    (6 < (Ipp32s) (color - maxColor)) ||
                    (6 < (Ipp32s) (minColor - color)) ||
                    (5 < (Ipp32s) (intraDev - maxIntraDev)) ||
                    (5 < (Ipp32s) (minIntraDev - intraDev)))
                {
                    bChangeDetected = true;
                }
            }
        }
        else
        {
            if ((interDevEst + 5 < intraDev) ||
                (interDevEst * 2 <= intraDev))
            {
                bChangeDetected = false;
            }
            else
            {
                bChangeDetected = true;
            }
        }
    }
    else
    {
        Ipp32s colorResidual;
        Ipp32s intraDev, intraDevResidual;
        Ipp32s interDev;
        Ipp32s interDevEst;

        // get changes
        colorResidual = pSrc->m_info.averageDev[SA_COLOR] -
                        pRef->m_info.averageDev[SA_COLOR];
        intraDev = pSrc->m_info.averageDev[SA_INTRA];
        intraDevResidual = intraDev -
                           pRef->m_info.averageDev[SA_INTRA];
        interDev = pSrc->m_info.averageDev[SA_INTER];
        interDevEst = pSrc->m_info.averageDev[SA_INTER_ESTIMATED];

        // there are some changes in the scene,
        // but it is still the same
        if (interDevEst + 2 <= intraDev)
        {
            bChangeDetected = false;
        }
        // there is a very smooth scene
        else if (intraDev + 3 <= interDevEst)
        {
            bChangeDetected = true;
        }
        // it is an potential scene change
        else if ((5 >= interDev) ||
                 (4 >= interDevEst))
        {
            bChangeDetected = false;
        }
        else if ((6 < GetAbs(colorResidual)) ||
                 (3 < intraDevResidual) ||
                 (6 < interDevEst))
        {
            bChangeDetected = true;
        }
    }

    // set frame parameters
    pSrc->m_bChangeDetected = bChangeDetected;

} // void SceneAnalyzerP::MakePPictureCodingDecision(SceneAnalyzerPicture *pRef,

} // namespace UMC
