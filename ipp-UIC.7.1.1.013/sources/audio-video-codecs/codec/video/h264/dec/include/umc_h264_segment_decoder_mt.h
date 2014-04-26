/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_SEGMENT_DECODER_MT_H
#define __UMC_H264_SEGMENT_DECODER_MT_H

#include "umc_h264_segment_decoder.h"

namespace UMC
{

class SegmentDecoderHPBase;

DECLALIGN(16) class H264SegmentDecoderMultiThreaded : public H264SegmentDecoder
{
public:
    // Default constructor
    H264SegmentDecoderMultiThreaded(TaskBroker * pTaskBroker);
    // Destructor
    virtual
    ~H264SegmentDecoderMultiThreaded(void);

    // Initialize object
    virtual
    Status Init(Ipp32s iNumber);

    // Decode slice's segment
    virtual
    Status ProcessSegment(void);

    // asynchronous called functions
    Status DecRecSegment(Ipp32s iCurMBNumber, Ipp32s &iMaxMBToDecRec);
    Status DecodeSegment(Ipp32s iCurMBNumber, Ipp32s &iMBToDecode);
    Status ReconstructSegment(Ipp32s iCurMBNumber, Ipp32s &iMBToReconstruct);
    virtual Status DeblockSegmentTask(Ipp32s iCurMBNumber, Ipp32s &iMBToDeblock);

    CoeffsPtrCommon GetCoefficientsBuffer(void)
    {
        return m_psBuffer;
    }

    virtual Status ProcessSlice(Ipp32s iCurMBNumber, Ipp32s &iMBToProcess);

    void RestoreErrorRect(Ipp32s startMb, Ipp32s endMb, H264Slice * pSlice);

    // Allocated more coefficients buffers
    void ReallocateCoefficientsBuffer(void);

    Status DecodeMacroBlockCAVLC(Ipp32u nCurMBNumber, Ipp32u &nMaxMBNumber);
    Status DecodeMacroBlockCABAC(Ipp32u nCurMBNumber, Ipp32u &nMaxMBNumber);

    Status ReconstructMacroBlockCAVLC(Ipp32u nCurMBNumber, Ipp32u nMaxMBNumber);
    Status ReconstructMacroBlockCABAC(Ipp32u nCurMBNumber, Ipp32u nMaxMBNumber);

    // Get direct motion vectors for block 4x4
    void GetMVD4x4_CABAC(const Ipp8u *pBlkIdx,
                           const Ipp8u *pCodMVd,
                           Ipp32u ListNum);
    void GetMVD4x4_16x8_CABAC(const Ipp8u *pCodMVd,
                                Ipp32u ListNum);
    void GetMVD4x4_8x16_CABAC(const Ipp8u *pCodMVd,
                                Ipp32u ListNum);
    void GetMVD4x4_CABAC(const Ipp8u pCodMVd,
                           Ipp32u ListNum);

    // Reconstruct skipped motion vectors
    void ReconstructSkipMotionVectors();

    // Decode motion vectors
    void DecodeMotionVectorsPSlice_CAVLC(void);
    void DecodeMotionVectors_CAVLC(bool bIsBSlice);
    void DecodeMotionVectors_CABAC();

    // Reconstruct motion vectors
    void ReconstructMotionVectors(void);

    void ReconstructMVsExternal(Ipp32s iListNum);
    void ReconstructMVsTop(Ipp32s iListNum);
    void ReconstructMVsLeft(Ipp32s iListNum);
    void ReconstructMVsInternal(Ipp32s iListNum);

    void ReconstructMVs16x16(Ipp32s iListNum);
    void ReconstructMVs16x8(Ipp32s iListNum, Ipp32s iSubBlockNum);
    void ReconstructMVs8x16(Ipp32s iListNum, Ipp32s iSubBlockNum);

    void ReconstructMVs8x8External(Ipp32s iListNum);
    void ReconstructMVs8x8Top(Ipp32s iListNum);
    void ReconstructMVs8x8Left(Ipp32s iListNum);
    void ReconstructMVs8x8Internal(Ipp32s iListNum);

    void ReconstructMVs8x4External(Ipp32s iListNum);
    void ReconstructMVs8x4Top(Ipp32s iListNum);
    void ReconstructMVs8x4Left(Ipp32s iListNum, Ipp32s iRowNum);
    void ReconstructMVs8x4Internal(Ipp32s iListNum, Ipp32s iSubBlockNum);

    void ReconstructMVs4x8External(Ipp32s iListNum);
    void ReconstructMVs4x8Top(Ipp32s iListNum, Ipp32s iColumnNum);
    void ReconstructMVs4x8Left(Ipp32s iListNum);
    void ReconstructMVs4x8Internal(Ipp32s iListNum, Ipp32s iSubBlockNum, Ipp32s iAddrC);

    void ReconstructMVs4x4External(Ipp32s iListNum);
    void ReconstructMVs4x4Top(Ipp32s iListNum, Ipp32s iColumnNum);
    void ReconstructMVs4x4Left(Ipp32s iListNum, Ipp32s iRowNum);
    void ReconstructMVs4x4Internal(Ipp32s iListNum, Ipp32s iBlockNum, Ipp32s iAddrC);
    void ReconstructMVs4x4InternalFewCheckRef(Ipp32s iListNum, Ipp32s iBlockNum, Ipp32s iAddrC);
    void ReconstructMVs4x4InternalNoCheckRef(Ipp32s iListNum, Ipp32s iBlockNum);

    void ResetMVs16x16(Ipp32s iListNum);
    void ResetMVs16x8(Ipp32s iListNum, Ipp32s iVectorOffset);
    void ResetMVs8x16(Ipp32s iListNum, Ipp32s iVectorOffset);
    void ResetMVs8x8(Ipp32s iListNum, Ipp32s iVectorOffset);
    void CopyMVs8x16(Ipp32s iListNum);
    void ReconstructMVPredictorExternalBlock(Ipp32s iListNum,
                                             const H264DecoderBlockLocation &mbAddrC,
                                             H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorExternalBlockMBAFF(Ipp32s iListNum,
                                                  H264DecoderBlockLocation mbAddrC,
                                                  H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorTopBlock(Ipp32s iListNum,
                                        Ipp32s iColumnNum,
                                        H264DecoderBlockLocation mbAddrC,
                                        H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorLeftBlock(Ipp32s iListNum,
                                         Ipp32s iRowNum,
                                         H264DecoderBlockLocation mbAddrC,
                                         H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorLeftBlockFewCheckRef(Ipp32s iListNum,
                                                    Ipp32s iRowNum,
                                                    H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorInternalBlock(Ipp32s iListNum,
                                             Ipp32s iBlockNum,
                                             Ipp32s iAddrC,
                                             H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorInternalBlockFewCheckRef(Ipp32s iListNum,
                                                        Ipp32s iBlockNum,
                                                        Ipp32s iAddrC,
                                                        H264DecoderMotionVector *pPred);
    void ReconstructMVPredictorInternalBlockNoCheckRef(Ipp32s iListNum,
                                                       Ipp32s iBlockNum,
                                                       H264DecoderMotionVector *pPred);
    void ReconstructMVPredictor16x16(Ipp32s iListNum,
                                     H264DecoderMotionVector *pPred);
    void ReconstructMVPredictor16x8(Ipp32s iListNum,
                                    Ipp32s iSubBlockNum,
                                    H264DecoderMotionVector *pPred);
    void ReconstructMVPredictor8x16(Ipp32s iListNum,
                                    Ipp32s iSubBlockNum,
                                    H264DecoderMotionVector *pPred);

    // Decode motion vectors
    void ReconstructDirectMotionVectorsSpatial(bool isDirectMB);
    void ReconstructMotionVectors4x4(const Ipp8u pCodMVd,
                                       Ipp32u ListNum);
    void ReconstructMotionVectors4x4(const Ipp8u *pBlkIdx,
                                       const Ipp8u* pCodMVd,
                                       Ipp32u ListNum);
    void ReconstructMotionVectors16x8(const Ipp8u *pCodMVd,
                                        Ipp32u ListNum);
    void ReconstructMotionVectors8x16(const Ipp8u *pCodMVd,
                                        Ipp32u ListNum);

    void DecodeDirectMotionVectors(bool isDirectMB);

    CoeffsPtrCommon  m_psBuffer;

    SegmentDecoderHPBase* m_SD;

protected:

    virtual SegmentDecoderHPBase* CreateSegmentDecoder();

    virtual void StartProcessingSegment(H264Task &Task);
    virtual void EndProcessingSegment(H264Task &Task);

private:

    // we lock the assignment operator to avoid any
    // accasional assignments
    H264SegmentDecoderMultiThreaded & operator = (H264SegmentDecoderMultiThreaded &)
    {
        return *this;

    } // H264SegmentDecoderMultiThreaded & operator = (H264SegmentDecoderMultiThreaded &)
};

} // namespace UMC

#endif /* __UMC_H264_SEGMENT_DECODER_MT_H */
