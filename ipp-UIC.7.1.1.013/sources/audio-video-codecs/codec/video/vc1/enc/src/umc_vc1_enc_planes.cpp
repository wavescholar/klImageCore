/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, work with planes
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_enc_def.h"
#include "umc_vc1_enc_common.h"
#include "umc_vc1_enc_debug.h"
#include "umc_vc1_enc_planes.h"


namespace UMC_VC1_ENCODER
{
Ipp32u Frame::CalcAllocatedMemSize(Ipp32u w, Ipp32u h, Ipp32u paddingSize)
{
    Ipp32u size = 0;
    Ipp32u wMB = (w+15)>>4;
    Ipp32u hMB = (h+15)>>4;

    Ipp32u stepY =  UMC::align_value<Ipp32u>(wMB*VC1_ENC_LUMA_SIZE   + 2*paddingSize);
    Ipp32u stepUV=  UMC::align_value<Ipp32u>(wMB*VC1_ENC_CHROMA_SIZE + paddingSize);


    //Y
    size += UMC::align_value<Ipp32u>(stepY  * (hMB*VC1_ENC_LUMA_SIZE + 2*paddingSize)
                    + 32 + stepY * 4);
    //U
    size += UMC::align_value<Ipp32u>(stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ paddingSize)
                    + 32 + stepUV * 4);
    //V
    size += UMC::align_value<Ipp32u>(stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ paddingSize)
                    + 32 + stepUV * 4);
    return size;
}

UMC::Status Frame::Init(Ipp8u* pBuffer, Ipp32s memSize, Ipp32u w, Ipp32u h, Ipp32u paddingSize)
{
    Ipp32u wMB = (w+15)>>4;
    Ipp32u hMB = (h+15)>>4;

    Close();

    if(!pBuffer)
        return UMC::UMC_ERR_NULL_PTR;

    if(memSize == 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    m_widthYPlane = w;
    m_widthUVPlane = w>>1;

    m_heightYPlane = h;
    m_heightUVPlane = h>>1;

    m_paddingSize = paddingSize;

    m_stepY =  UMC::align_value<Ipp32u>(wMB*VC1_ENC_LUMA_SIZE   + 2*m_paddingSize);
    m_stepUV=  UMC::align_value<Ipp32u>(wMB*VC1_ENC_CHROMA_SIZE + m_paddingSize);

    //Y
    m_pYFrame = pBuffer;
    pBuffer += UMC::align_value<Ipp32u>(m_stepY  * (hMB*VC1_ENC_LUMA_SIZE + 2*m_paddingSize)
                    + 32 + m_stepY * 4);
    memSize -= UMC::align_value<Ipp32u>(m_stepY  * (hMB*VC1_ENC_LUMA_SIZE + 2*m_paddingSize)
                    + 32 + m_stepY * 4);

    if(memSize <= 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    //U
    m_pUFrame = pBuffer;
    pBuffer += UMC::align_value<Ipp32u>(m_stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ m_paddingSize)
                    + 32 + m_stepUV * 4);
    memSize -= UMC::align_value<Ipp32u>(m_stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ m_paddingSize)
                    + 32 + m_stepUV * 4);

    if(memSize <= 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;


     //V
    m_pVFrame = pBuffer;
    pBuffer += UMC::align_value<Ipp32u>(m_stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ m_paddingSize)
                    + 32 + m_stepUV * 4);
    memSize -= UMC::align_value<Ipp32u>(m_stepUV  * (hMB*VC1_ENC_CHROMA_SIZE+ m_paddingSize)
                    + 32 + m_stepUV * 4);

    if(memSize < 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    m_pYPlane = UMC::align_pointer<Ipp8u*>( m_pYFrame + m_stepY*(m_paddingSize + 2) + m_paddingSize);
    m_pUPlane = UMC::align_pointer<Ipp8u*>( m_pUFrame + m_stepUV*((m_paddingSize>>1) + 2) + (m_paddingSize>>1));
    m_pVPlane = UMC::align_pointer<Ipp8u*>( m_pVFrame + m_stepUV*((m_paddingSize>>1) + 2) + (m_paddingSize>>1));

    return UMC::UMC_OK;
}
void Frame::Close()
{
    m_pYFrame = 0;
    m_pUFrame = 0;
    m_pVFrame = 0;
    m_stepY = 0;
    m_stepUV = 0;

    m_pYPlane = 0;
    m_pUPlane = 0;
    m_pVPlane = 0;

    m_widthYPlane = 0;
    m_widthUVPlane = 0;

    m_heightYPlane = 0;
    m_heightUVPlane = 0;

    m_paddingSize = 0;
    m_bTaken = false;
}

UMC::Status Frame::PadFrameProgressive()
{
    Ipp32u rightY     = (((m_widthYPlane  + 15)>>4)<<4) - m_widthYPlane  + m_paddingSize;
    Ipp32u bottomY    = (((m_heightYPlane + 15)>>4)<<4) - m_heightYPlane + m_paddingSize;
    Ipp32u upperY     = m_paddingSize;
    Ipp32u leftY      = m_paddingSize;

    Ipp32u rightUV     = rightY >>1;
    Ipp32u bottomUV    = bottomY>>1;
    Ipp32u upperUV     = upperY >>1;
    Ipp32u leftUV      = leftY  >>1;

    Ipp8u            *pY = 0, *pU = 0, *pV = 0;
    Ipp32u i;

   // upper
    pY = m_pYPlane;
    pU = m_pUPlane;
    pV = m_pVPlane;

    for (i = 0; i < upperY; i++)
    {
       memcpy(pY - m_stepY, pY , m_widthYPlane);
       pY -= m_stepY;
    }

    for (i = 0; i < upperUV; i++ )
    {
       memcpy(pU - m_stepUV, pU , m_widthUVPlane);
       memcpy(pV - m_stepUV, pV , m_widthUVPlane);
       pU -= m_stepUV;
       pV -= m_stepUV;
    }

    // bottom

    pY = m_pYPlane + (m_heightYPlane  - 1)*m_stepY;
    pU = m_pUPlane + (m_heightUVPlane  - 1)*m_stepUV;
    pV = m_pVPlane + (m_heightUVPlane  - 1)*m_stepUV;

    for (i = 0; i < bottomY; i++ )
    {
       memcpy(pY + m_stepY, pY  , m_widthYPlane);
       pY += m_stepY;
    }
    for (i = 0; i < bottomUV; i++ )
    {
       memcpy(pU + m_stepUV, pU  , m_widthUVPlane);
       memcpy(pV + m_stepUV, pV  , m_widthUVPlane);

       pU += m_stepUV;
       pV += m_stepUV;
    }

    // left
    pY = m_pYPlane - m_stepY *upperY;
    pU = m_pUPlane - m_stepUV*upperUV;
    pV = m_pVPlane - m_stepUV*upperUV;


    for (i = 0; i < m_heightYPlane + upperY+bottomY; i++)
    {
        memset(pY-leftY,pY[0],leftY);
        pY += m_stepY;
    }
    for (i = 0; i < m_heightUVPlane + upperUV + bottomUV; i++)
    {
        memset(pU-leftUV,pU[0],leftUV);
        memset(pV-leftUV,pV[0],leftUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }

    //right

    pY = m_pYPlane - m_stepY*upperY + m_widthYPlane - 1;
    pU = m_pUPlane - m_stepUV*upperUV+ m_widthUVPlane - 1;
    pV = m_pVPlane - m_stepUV*upperUV+ m_widthUVPlane - 1;

    for (i = 0; i < m_heightYPlane + upperY + bottomY; i++)
    {
        memset(pY+1,pY[0],rightY);
        pY += m_stepY;
    }
    for (i = 0; i < m_heightUVPlane + upperUV + bottomUV; i++)
    {
        memset(pU+1,pU[0],rightUV);
        memset(pV+1,pV[0],rightUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }
     return UMC::UMC_OK;
}

UMC::Status Frame::PadFrameField()
{
    Ipp32u rightY     = (((m_widthYPlane  + 15)>>4)<<4) - m_widthYPlane  + m_paddingSize;
    Ipp32u bottomY    = (((m_heightYPlane + 15)>>4)<<4) - m_heightYPlane + m_paddingSize;
    Ipp32u upperY     = m_paddingSize;
    Ipp32u leftY      = m_paddingSize;

    Ipp32u rightUV     = rightY >>1;
    Ipp32u bottomUV    = bottomY>>1;
    Ipp32u upperUV     = upperY >>1;
    Ipp32u leftUV      = leftY  >>1;

    Ipp8u *pY = 0, *pU = 0, *pV = 0;
    Ipp32u i;

   // upper
    pY = m_pYPlane;
    pU = m_pUPlane;
    pV = m_pVPlane;

    for (i = 0; i < upperY/2; i++)
    {
       memcpy(pY - 2*m_stepY, pY,           m_widthYPlane); //first field
       memcpy(pY - m_stepY,   pY + m_stepY, m_widthYPlane); //second field
       pY -= 2*m_stepY;
    }

    for (i = 0; i < upperUV/2; i++ )
    {
       memcpy(pU - 2*m_stepUV, pU,            m_widthUVPlane); //first field
       memcpy(pU - m_stepUV,   pU + m_stepUV, m_widthUVPlane); //second field

       memcpy(pV - 2*m_stepUV, pV,            m_widthUVPlane); //first field
       memcpy(pV - m_stepUV,   pV + m_stepUV, m_widthUVPlane); //second field

       pU -= 2*m_stepUV;
       pV -= 2*m_stepUV;
    }

    // bottom
    pY = m_pYPlane + (m_heightYPlane  - 1)*m_stepY;
    pU = m_pUPlane + (m_heightUVPlane  - 1)*m_stepUV;
    pV = m_pVPlane + (m_heightUVPlane  - 1)*m_stepUV;

    for (i = 0; i < bottomY/2; i++ )
    {
       memcpy(pY + m_stepY,   pY - m_stepY, m_widthYPlane); //first field
       memcpy(pY + 2*m_stepY, pY,           m_widthYPlane); //second field
       pY += 2*m_stepY;
    }

    for (i = 0; i < bottomUV/2; i++ )
    {
       memcpy(pU + m_stepUV,   pU  - m_stepUV, m_widthUVPlane); //first field
       memcpy(pU + 2*m_stepUV, pU,             m_widthUVPlane); //second field

       memcpy(pV + m_stepUV,   pV  - m_stepUV, m_widthUVPlane); //first field
       memcpy(pV + 2*m_stepUV, pV,             m_widthUVPlane); //second field

       pU += 2*m_stepUV;
       pV += 2*m_stepUV;
    }

    // left
    pY = m_pYPlane - m_stepY *upperY;
    pU = m_pUPlane - m_stepUV*upperUV;
    pV = m_pVPlane - m_stepUV*upperUV;

    for (i = 0; i < m_heightYPlane + upperY + bottomY; i++)
    {
        memset(pY-leftY,pY[0],leftY);
        pY += m_stepY;
    }

    for (i = 0; i < m_heightUVPlane + upperUV + bottomUV; i++)
    {
        memset(pU-leftUV,pU[0],leftUV);
        memset(pV-leftUV,pV[0],leftUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }

    //right
    pY = m_pYPlane - m_stepY*upperY + m_widthYPlane - 1;
    pU = m_pUPlane - m_stepUV*upperUV+ m_widthUVPlane - 1;
    pV = m_pVPlane - m_stepUV*upperUV+ m_widthUVPlane - 1;

    for (i = 0; i < m_heightYPlane + upperY + bottomY; i++)
    {
        memset(pY+1,pY[0],rightY);
        pY += m_stepY;
    }
    for (i = 0; i < m_heightUVPlane + upperUV + bottomUV; i++)
    {
        memset(pU+1,pU[0],rightUV);
        memset(pV+1,pV[0],rightUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }
     return UMC::UMC_OK;
}

UMC::Status Frame::PadField(bool bBottomField)
{
    Ipp32u rightY     = (((m_widthYPlane  + 15)>>4)<<4) - m_widthYPlane  + m_paddingSize;
    Ipp32u bottomY    = (((m_heightYPlane + 15)>>4)<<4) - m_heightYPlane + m_paddingSize;
    Ipp32u upperY     = m_paddingSize;
    Ipp32u leftY      = m_paddingSize;

    Ipp32u rightUV     = rightY >>1;
    Ipp32u bottomUV    = bottomY>>1;
    Ipp32u upperUV     = upperY >>1;
    Ipp32u leftUV      = leftY  >>1;

    Ipp8u *pY = 0, *pU = 0, *pV = 0;
    Ipp32u i;

   // upper
    if(!bBottomField)
    {
        pY = m_pYPlane;
        pU = m_pUPlane;
        pV = m_pVPlane;
    }
    else
    {
        pY = m_pYPlane + m_stepY;
        pU = m_pUPlane + m_stepUV;
        pV = m_pVPlane + m_stepUV;
    }

    for (i = 0; i < upperY/2; i++)
    {
       memcpy(pY - 2*m_stepY, pY,           m_widthYPlane);
       pY -= 2*m_stepY;
    }

    for (i = 0; i < upperUV/2; i++ )
    {
       memcpy(pU - 2*m_stepUV, pU,            m_widthUVPlane);
       memcpy(pV - 2*m_stepUV, pV,            m_widthUVPlane);
       pU -= 2*m_stepUV;
       pV -= 2*m_stepUV;
    }

    // bottom
    if(!bBottomField)
    {
        pY = m_pYPlane + (m_heightYPlane  - 2)*m_stepY;
        pU = m_pUPlane + (m_heightUVPlane  - 2)*m_stepUV;
        pV = m_pVPlane + (m_heightUVPlane  - 2)*m_stepUV;
    }
    else
    {
        pY = m_pYPlane + (m_heightYPlane  - 1)*m_stepY;
        pU = m_pUPlane + (m_heightUVPlane  - 1)*m_stepUV;
        pV = m_pVPlane + (m_heightUVPlane  - 1)*m_stepUV;
    }

    for (i = 0; i < bottomY/2; i++ )
    {
       memcpy(pY + 2*m_stepY, pY, m_widthYPlane);
       pY += 2*m_stepY;
    }

    for (i = 0; i < bottomUV/2; i++ )
    {
       memcpy(pU + 2*m_stepUV, pU, m_widthUVPlane);
       memcpy(pV + 2*m_stepUV, pV, m_widthUVPlane);
       pU += 2*m_stepUV;
       pV += 2*m_stepUV;
    }

    // left
    if(!bBottomField)
    {
        pY = m_pYPlane - m_stepY *upperY;
        pU = m_pUPlane - m_stepUV*upperUV;
        pV = m_pVPlane - m_stepUV*upperUV;
    }
    else
    {
        pY = m_pYPlane - m_stepY *upperY  + m_stepY;
        pU = m_pUPlane - m_stepUV*upperUV + m_stepUV;
        pV = m_pVPlane - m_stepUV*upperUV + m_stepUV;
    }

    for (i = 0; i < (m_heightYPlane + upperY + bottomY)/2; i++)
    {
        memset(pY-leftY,pY[0],leftY);
        pY += 2*m_stepY;
    }

    for (i = 0; i < (m_heightUVPlane + upperUV + bottomUV)/2; i++)
    {
        memset(pU-leftUV,pU[0],leftUV);
        memset(pV-leftUV,pV[0],leftUV);
        pU += 2*m_stepUV;
        pV += 2*m_stepUV;
    }

    //right
    if(!bBottomField)
    {
        pY = m_pYPlane - m_stepY*upperY   + m_widthYPlane - 1;
        pU = m_pUPlane - m_stepUV*upperUV + m_widthUVPlane - 1;
        pV = m_pVPlane - m_stepUV*upperUV + m_widthUVPlane - 1;
    }
    else
    {
        pY = m_pYPlane - m_stepY*upperY   + m_widthYPlane  - 1  + m_stepY;
        pU = m_pUPlane - m_stepUV*upperUV + m_widthUVPlane - 1  + m_stepY;
        pV = m_pVPlane - m_stepUV*upperUV + m_widthUVPlane - 1  + m_stepY;
    }

    for (i = 0; i < (m_heightYPlane + upperY + bottomY)/2; i++)
    {
        memset(pY+1,pY[0],rightY);
        pY += 2*m_stepY;
    }
    for (i = 0; i < (m_heightUVPlane + upperUV + bottomUV)/2; i++)
    {
        memset(pU+1,pU[0],rightUV);
        memset(pV+1,pV[0],rightUV);
        pU += 2*m_stepUV;
        pV += 2*m_stepUV;
    }
     return UMC::UMC_OK;
}

UMC::Status Frame::PadPlaneProgressive()
{
    Ipp32u rightY     = (((m_widthYPlane  + 15)>>4)<<4) - m_widthYPlane;
    Ipp32u bottomY    = (((m_heightYPlane + 15)>>4)<<4) - m_heightYPlane;

    Ipp32u rightUV     = rightY >>1;
    Ipp32u bottomUV    = bottomY>>1;


    Ipp8u            *pY = 0, *pU = 0, *pV = 0;
    Ipp32u i;

    // bottom

    pY = m_pYPlane + (m_heightYPlane  - 1)*m_stepY;
    pU = m_pUPlane + (m_heightUVPlane  - 1)*m_stepUV;
    pV = m_pVPlane + (m_heightUVPlane  - 1)*m_stepUV;

    for (i=0; i<bottomY;i++ )
    {
       memcpy(pY + m_stepY, pY  , m_widthYPlane);
       pY += m_stepY;
    }
    for (i=0; i<bottomUV;i++ )
    {
       memcpy(pU + m_stepUV, pU  , m_widthUVPlane);
       memcpy(pV + m_stepUV, pV  , m_widthUVPlane);

       pU += m_stepUV;
       pV += m_stepUV;
    }

    //right

    pY = m_pYPlane + m_widthYPlane - 1;
    pU = m_pUPlane + m_widthUVPlane - 1;
    pV = m_pVPlane + m_widthUVPlane - 1;

    for (i=0; i<m_heightYPlane+bottomY; i++)
    {
        memset(pY+1,pY[0],rightY);
        pY += m_stepY;
    }
    for (i=0; i<m_heightUVPlane+bottomUV; i++)
    {
        memset(pU+1,pU[0],rightUV);
        memset(pV+1,pV[0],rightUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }
     return UMC::UMC_OK;
}

UMC::Status Frame::PadPlaneField()
{
    Ipp32u rightY     = (((m_widthYPlane  + 15)>>4)<<4) - m_widthYPlane;
    Ipp32u bottomY    = (((m_heightYPlane/2 + 15)>>4)<<4) - m_heightYPlane/2;

    Ipp32u rightUV     = rightY >>1;
    Ipp32u bottomUV    = bottomY>>1;


    Ipp8u            *pY = 0, *pU = 0, *pV = 0;
    Ipp32u i;

    // bottom
    pY = m_pYPlane + (m_heightYPlane  - 1)*m_stepY;
    pU = m_pUPlane + (m_heightUVPlane  - 1)*m_stepUV;
    pV = m_pVPlane + (m_heightUVPlane  - 1)*m_stepUV;

    for (i=0; i < bottomY;i++ )
    {
       memcpy(pY + m_stepY,   pY - m_stepY, m_widthYPlane); //first field
       memcpy(pY + 2*m_stepY, pY,           m_widthYPlane); //second field

       pY += 2*m_stepY;
    }
    for (i=0; i < bottomUV;i++ )
    {
       memcpy(pU + m_stepUV,   pU - m_stepUV, m_widthUVPlane); //first field
       memcpy(pU + 2*m_stepUV, pU,            m_widthUVPlane); //second field
       memcpy(pV + m_stepUV,   pV - m_stepUV, m_widthUVPlane); //first field
       memcpy(pV + 2*m_stepUV, pV,            m_widthUVPlane); //second field

       pU += 2*m_stepUV;
       pV += 2*m_stepUV;
    }

    //right
    pY = m_pYPlane + m_widthYPlane - 1;
    pU = m_pUPlane + m_widthUVPlane - 1;
    pV = m_pVPlane + m_widthUVPlane - 1;

    for (i=0; i<m_heightYPlane + bottomY; i++)
    {
        memset(pY+1,pY[0],rightY);
        pY += m_stepY;
    }

    for (i=0; i<m_heightUVPlane + bottomUV; i++)
    {
        memset(pU+1,pU[0],rightUV);
        memset(pV+1,pV[0],rightUV);
        pU += m_stepUV;
        pV += m_stepUV;
    }
     return UMC::UMC_OK;
}

UMC::Status Frame::CopyPlane ( Ipp8u* pYPlane, Ipp32u stepY,
                               Ipp8u* pUPlane, Ipp32u stepU,
                               Ipp8u* pVPlane, Ipp32u stepV,
                               ePType pictureType)
{
    IppiSize            sizeLuma        = {m_widthYPlane, m_heightYPlane};
    IppiSize            sizeChroma      = {m_widthUVPlane,m_heightUVPlane};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    ippiCopy_8u_C1R(pYPlane,stepY, m_pYPlane,m_stepY, sizeLuma);
    ippiCopy_8u_C1R(pUPlane,stepU,m_pUPlane,m_stepUV, sizeChroma);
    ippiCopy_8u_C1R(pVPlane,stepV,m_pVPlane,m_stepUV, sizeChroma);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

    m_bTaken      = true;
    m_pictureType = pictureType;
    return UMC::UMC_OK;

}
UMC::Status Frame::CopyPlane ( Frame * fr)
{
    IppiSize            sizeLuma        = {m_widthYPlane, m_heightYPlane};
    IppiSize            sizeChroma      = {m_widthUVPlane,m_heightUVPlane};

LIB_STAT_START_TIME(m_LibStat->LibStartTime);
    ippiCopy_8u_C1R(fr->m_pYFrame,fr->m_stepY, m_pYPlane,m_stepY, sizeLuma);
    ippiCopy_8u_C1R(fr->m_pUFrame,fr->m_stepUV,m_pUPlane,m_stepUV, sizeChroma);
    ippiCopy_8u_C1R(fr->m_pVFrame,fr->m_stepUV,m_pVPlane,m_stepUV, sizeChroma);
LIB_STAT_END_TIME(m_LibStat->LibStartTime, m_LibStat->LibEndTime, m_LibStat->LibTotalTime);

    m_bTaken      = true;
    m_pictureType = fr->m_pictureType;
    return UMC::UMC_OK;

}

//////////////////////////////////////////////////////////////////////////////////////
Ipp32u BufferedFrames::CalcAllocatedMemSize(Ipp32u w, Ipp32u h, Ipp32u paddingSize, Ipp32u n)
{
    Ipp32u memSize = 0;
    memSize += UMC::align_value<Ipp32u>(n*sizeof(Frame));
    memSize += n*UMC::align_value<Ipp32u>(Frame::CalcAllocatedMemSize(w, h, paddingSize));

    return memSize;
}

UMC::Status BufferedFrames::Init (Ipp8u* pBuffer, Ipp32u memSize, Ipp32u w, Ipp32u h, Ipp32u paddingSize, Ipp32u n)
{
    Ipp32u i;
    Ipp32u memShift = 0;
    UMC::Status err = UMC::UMC_OK;

    Close();

    if(!pBuffer)
        return UMC::UMC_ERR_NULL_PTR;

    if(memSize == 0)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

    m_pFrames = new (pBuffer) Frame[n];
    if (!m_pFrames)
        return UMC::UMC_ERR_ALLOC;

    memShift = UMC::align_value<Ipp32u>(n * sizeof(Frame));
    if(memSize < memShift)
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
    pBuffer += memShift;
    memSize -= memShift;

    m_bufferSize    = n;

    Ipp32u frameSize = UMC::align_value<Ipp32u>(Frame::CalcAllocatedMemSize(w,h,paddingSize));
    for (i = 0; i < n ; i++)
    {
        if(memSize < frameSize)
            return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

        err = m_pFrames[i].Init (pBuffer, memSize, w,h,paddingSize);
        if (err != UMC::UMC_OK)
            return err;

        pBuffer += frameSize;
        memSize -= frameSize;
    }
    return err;
}
void BufferedFrames::Close ()
{
    if (m_pFrames)
    {
        delete [] m_pFrames;
        m_pFrames = 0;
    }
    m_bufferSize        = 0;
    m_nBuffered         = 0;
    m_currFrameIndex    = 0;
    m_bClosed           = false;
}
UMC::Status   BufferedFrames::SaveFrame (   Ipp8u* pYPlane, Ipp32u stepY,
                                            Ipp8u* pUPlane, Ipp32u stepU,
                                            Ipp8u* pVPlane, Ipp32u stepV )
{
    if (m_currFrameIndex)
    {
        // frames from prev. seq. - ERROR;
        return UMC::UMC_ERR_FAILED;
    }

    if (m_nBuffered < m_bufferSize )
    {
        m_pFrames[m_nBuffered].CopyPlane(pYPlane,stepY,pUPlane,stepU,pVPlane,stepV);
        m_bClosed = false;
        m_nBuffered++;
        return UMC::UMC_OK;
    }
    else
    {
        return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
    }
};
UMC::Status   BufferedFrames::GetFrame (Frame** currFrame)
{
    *currFrame = 0;
    if (!m_bClosed)
    {
        //sequence isn't closed - ERROR;
        return UMC::UMC_ERR_FAILED;
    }
    if (m_nBuffered)
    {
        *currFrame = &(m_pFrames[m_currFrameIndex]);
        m_nBuffered --;
        m_currFrameIndex = (m_nBuffered) ? (m_currFrameIndex++) : 0;
        return UMC::UMC_OK;
    }
    else
    {
        return UMC::UMC_ERR_NOT_ENOUGH_DATA;
    }
}
UMC::Status BufferedFrames::GetReferenceFrame (Frame** currFrame)
{
    Ipp32u index = 0;
    *currFrame = 0;

    if (m_bClosed)
    {
        //sequence is closed - ERROR;
        return UMC::UMC_ERR_FAILED;
    }
    if (m_nBuffered)
    {
        index = m_currFrameIndex + m_nBuffered - 1;
        *currFrame = &(m_pFrames[m_currFrameIndex]);
        m_bClosed = true;
        m_nBuffered -- ;
        m_currFrameIndex = (m_nBuffered) ? m_currFrameIndex : 0;
        return UMC::UMC_OK;
    }
    else
    {
        return UMC::UMC_ERR_NOT_ENOUGH_DATA;
    }
}
UMC::Status  BufferedFrames::ReferenceFrame()
{
    if (m_currFrameIndex)
    {
         return UMC::UMC_ERR_FAILED;
    }
    if (m_nBuffered)
    {
        m_bClosed = true;
    }
    return UMC::UMC_OK;
}

}
#endif // defined (UMC_ENABLE_VC1_VIDEO_ENCODER)
