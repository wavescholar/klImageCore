/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VIDEO_DATA_H__
#define __UMC_VIDEO_DATA_H__

#include "umc_defs.h"
#include "umc_media_data.h"

#include "ippdefs.h"
#include "ippi.h"

/*
    USAGE MODEL:
    I. Initialization of VideoData parameters. It has to be done after construction
      or after Close.
      A. Simplest case. No additional planes, planes have equal bitdepth.
        1. Set required alignment for data with SetAlignment. Default is the sample size.
        2. Init(w,h,ColorFormat[,bitdepth]). Default bitdepth is derived from format.
      B. Advanced case
        1. Init(w,h,nplanes[,bitdepth]) and SetAlignment if required before or after.
        2. Modify bitdepth or sample size for planes where necessary
        3. Call SetColorFormat. It is the only moment to call this method.
      This stage fill all internal information about planes, including pitches.
      After this no more changes to parameters shall be introduced.
      Function GetMappingSize can be called now to determine required quantity of memory
      for all planes taking into account current alignment. All other Get methods except
      of GetPlanePointer are possible to use.

    II. Link to memory. These operations assign all plane pointers. After that
      MediaData::GetDataPointer will return aligned beginning of the first plane,
      MediaData::GetDataSize will return value equal to MappingSize,
      MediaData::GetBufferPointer can differ from DataPointer due to aligning
      Two ways:
      A. Allocation using Alloc. BufferSize will be MappingSize + alignment.
      B. Call SetBufferPointer(bufsize). After that BufferSize will be bufsize.
      Method ReleaseImage cancels this operations (unlink from memory).
      These methods only work with continuously located planes.

    III. Operations which don't change plane parameters, like SetFrameType, can be used at
      any moment. Operations SetPlanePointer and SetPlanePitch allow working with separate
      planes or without specified ColorFormat but have to be used with care. Functions like
      GetMappingSize and GetPlaneInfo can provide incorrect results.

    Note:
    parent class methods GetDataPointer, MoveDataPointer operator= shouldn't be used.
*/

namespace UMC
{

// Color format description structure
struct sPlaneFormatInfo
{
    Ipp32u iWidthDiv;  // Horizontal downsampling factor
    Ipp32u iHeightDiv; // Vertical downsampling factor
    Ipp32u iChannels;  // Number of merged channels in the plane
    Ipp32u iAlignMult; // Alignment value multiplier
};

struct sColorFormatInfo
{
    ColorFormat      cFormat;
    Ipp32u           iPlanes;        // Number of planes
    Ipp32u           iMinBitDepth;   // Minimum bitdepth
    Ipp32u           iMinAlign;      // Minimal required alignment in bytes
    sPlaneFormatInfo planeFormatInfo[MAX_COLOR_PLANES];
};

const sColorFormatInfo* GetColorFormatInfo(ColorFormat colorFormat);

class VideoData : public MediaData
{
public:
    DYNAMIC_CAST_DECL(VideoData, MediaData)

    struct PlaneData
    {
        PlaneData()
        {
            m_size.width  = 0;
            m_size.height = 0;
            m_iSampleSize = 0;
            m_iSamples    = 0;
            m_iBitDepth   = 0;
            m_iWidthDiv   = 1;
            m_iHeightDiv  = 1;

            m_pPlane   = NULL;
            m_iPitch   = 0;
            m_iMemSize = 0;
        }

        IppiSize m_size;         // width and height of the plane
        Ipp32u   m_iSampleSize;  // sample size (in bytes)
        Ipp32u   m_iSamples;     // number of samples per plane element
        Ipp32u   m_iBitDepth;    // number of significant bits per sample (should be <= 8*m_iSampleSize)
        Ipp32u   m_iWidthDiv;    // horizontal downsampling factor
        Ipp32u   m_iHeightDiv;   // vertical downsampling factor

        Ipp8u   *m_pPlane;       // pointer to plane data
        size_t   m_iPitch;       // plane pitch (should be >= width*m_iSamples*m_iSampleSize)
        size_t   m_iMemSize;     // size of occupied memory (pitch*height)
    };

    VideoData(void);
    virtual ~VideoData(void);

    // Initializes image with current values
    virtual Status Init();

    // Initialize. Only remembers image characteristics for future.
    virtual Status Init(Ipp32u iWidth, Ipp32u iHeight, ColorFormat format, Ipp32u iBitDepth = 0);

    // Initialize. Only copy image characteristics from existing data for future.
    virtual Status Init(VideoData *pData);

    // Allocate buffer for video data and initialize it.
    virtual Status Alloc(void);

    // Release video data and all internal memory. Inherited.
    virtual Status Close(void);

    // Reset all plane pointers, release memory if allocated by Alloc
    virtual Status ReleaseImage(void);

    // Update planes info
    Status MapImage(void);

    // Update plane info
    Status MapPlane(Ipp32u iPlane);

    // Set buffer pointer, assign all pointers. Inherited.
    // VideoData parameters must have been prepared
    virtual Status SetBufferPointer(Ipp8u *pBuffer, size_t iSize);

    // Set common bitdepth for all planes
    Status SetBitDepth(Ipp32u iBitDepth);
    // Set plane bitdepth
    Status SetPlaneBitDepth(Ipp32u iBitDepth, Ipp32u iPlaneNumber);
    // Get plane bitdepth
    inline Ipp32u GetPlaneBitDepth(Ipp32u iPlaneNumber) const
    {
        return ((!m_pPlaneData || (m_iPlanes <= iPlaneNumber))?0:(m_pPlaneData[iPlaneNumber].m_iBitDepth));
    }

    // Set common sample size for all planes
    Status SetSampleSize(Ipp32u iSampleSize);
    // Set plane sample size
    Status SetPlaneSampleSize(Ipp32u iSampleSize, Ipp32u iPlaneNumber);
    // Get plane sample size
    inline Ipp32u GetPlaneSampleSize(Ipp32u iPlaneNumber) const
    {
        return ((!m_pPlaneData || (m_iPlanes <= iPlaneNumber))?0:(m_pPlaneData[iPlaneNumber].m_iSampleSize));
    }
    // Get max sample size
    Ipp32u GetMaxSampleSize() const;

    // Set plane destination pointer
    Status SetPlaneDataPtr(void *pDst, Ipp32u iPlaneNumber);
    // Safely get plane destination pointer
    inline void* GetPlaneDataPtr(Ipp32u iPlaneNumber)
    {
        return ((!m_pPlaneData || (m_iPlanes <= iPlaneNumber))?0:m_pPlaneData[iPlaneNumber].m_pPlane);
    }

    // Set plane pitch
    Status SetPlanePitch(size_t nPitch, Ipp32u iPlaneNumber);
    // Safely get plane pitch
    inline size_t GetPlanePitch(Ipp32u iPlaneNumber)
    {
        return ((!m_pPlaneData || (m_iPlanes <= iPlaneNumber))?0:m_pPlaneData[iPlaneNumber].m_iPitch);
    }

    // Set common Alignment
    Status SetAlignment(Ipp32u iAlignment);

    // get buffer size
    size_t GetMappingSize(void);

    Status Crop(UMC::RECT CropArea);

     // Copy actual video data
    Status Copy(VideoData *pDstData);

    // Get total number of channels
    Ipp32u GetChannelsNumber() const;

    Ipp32u GetPlanesNumber() const { return m_iPlanes; }

    virtual PlaneData* GetPtrToPlane(Ipp32u iPlane) const { return (iPlane < m_iPlanes)?&m_pPlaneData[iPlane]:0; }

     // Copy structures data (just pointers)
    VideoData& operator=(const VideoData&);

protected:
    virtual Status AllocPlanes(Ipp32u iPlanes);

public:
    PictureStructure m_picStructure; // variants: progressive frame, top first, bottom first, only top, only bottom
    ColorFormat      m_colorFormat;  // color format of image
    SampleFormat     m_sampleFormat; // image sample format (unsigned, signed, float)
    Ipp32u           m_iWidth;       // image width
    Ipp32u           m_iHeight;      // image height
    Ipp32u           m_iSAWidth;     // sample aspect width (pixel width/height proportion, default 1,1 - square pixels)
    Ipp32u           m_iSAHeight;    // sample aspect height

protected:
    Ipp32u     m_iAlignment;   // default 1
    Ipp32u     m_iPlanes;      // number of planes
    PlaneData *m_pPlaneData;   // pointer to allocated planes info
};

} // namespace UMC

#endif // __UMC_VIDEO_DATA_H__
