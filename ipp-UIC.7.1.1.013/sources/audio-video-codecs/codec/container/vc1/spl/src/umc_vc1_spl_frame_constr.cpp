/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) frame constructor
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_VC1_SPLITTER)

#include "umc_vc1_spl_frame_constr.h"
#include "umc_vc1_spl_tbl.h"
#include "umc_vc1_spl_defs.h"
#include "umc_vc1_common_macros_defs.h"
#include "umc_vc1_common.h"
#include "umc_vc1_common_defs.h"
#include "umc_bitstream_reader.h"
#include "umc_memory_reader.h"

using namespace UMC;
using namespace VC1Common;


vc1_frame_constructor_rcv::vc1_frame_constructor_rcv()
{
}

vc1_frame_constructor_rcv::~vc1_frame_constructor_rcv()
{
}

void vc1_frame_constructor_rcv::Reset()
{
}

Status vc1_frame_constructor_rcv::GetNextFrame(VC1FrameConstrInfo& Info)
{
    Status umcSts = UMC_ERR_NOT_ENOUGH_DATA;
    Ipp32u frameSize = 0;

    if((Ipp32u)Info.in->GetBufferSize() == 0)
    {
        if((Ipp32u)Info.out->GetDataSize() == 0)
            return UMC_ERR_END_OF_STREAM;
        else
            return UMC_OK;
    }

    if((Ipp32u)Info.out->GetDataSize() == 0)
    {
        if((Ipp32u)Info.in->GetDataSize() < (Ipp32u)Info.in->GetBufferSize() - 4)
        {
            Ipp8u* curr_pos = ((Ipp8u*)Info.in->GetBufferPointer()+(Ipp32u)Info.in->GetDataSize());
            //frameSize = *(Ipp32u*)((Ipp8u*)in->GetBufferPointer()+(Ipp32u)in->GetDataSize());
            frameSize  = ((*(curr_pos+3))<<24) + ((*(curr_pos+2))<<16) + ((*(curr_pos+1))<<8) + *(curr_pos);
            frameSize &= 0x0fffffff;
            frameSize += 8;

            if ((Ipp32u)Info.out->GetBufferSize() < frameSize)
                return UMC_ERR_NOT_ENOUGH_BUFFER;

            Info.out->SetBufferPointer((Ipp8u*)Info.out->GetBufferPointer(),frameSize);
        }
        else
        {
            Ipp8u* buf = (Ipp8u*)Info.in->GetBufferPointer();
            Ipp8u* data = (Ipp8u*)Info.in->GetBufferPointer() + (Ipp32u)Info.in->GetDataSize();
            ippsCopy_8u(data, buf, (Ipp32u)Info.in->GetBufferSize() - (Ipp32u)Info.in->GetDataSize());
            Info.in->SetDataSize((Ipp32u)Info.in->GetBufferSize() - (Ipp32u)Info.in->GetDataSize());

            return UMC_ERR_NOT_ENOUGH_DATA;
        }
    }

    umcSts = GetData(Info);

    Info.stCodes->values[0]  = 0x0D010000;
    Info.stCodes->offsets[0] = 0;

    return umcSts;
}
// Unification interface 
#if defined(_MSC_VER)
#pragma warning(disable : 4100)
#endif
Status vc1_frame_constructor_rcv::GetData(VC1FrameConstrInfo& Info)
{
    if ((Ipp32u)Info.out->GetBufferSize() < (Ipp32u)Info.out->GetDataSize())
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    Ipp32u readDataSize = (Ipp32u)Info.in->GetDataSize();
    Ipp32u readBufSize = (Ipp32u)Info.in->GetBufferSize();
    Ipp8u* readBuf = (Ipp8u*)Info.in->GetBufferPointer();

    Ipp32u currFrameSize = (Ipp32u)Info.out->GetDataSize();
    Ipp8u* currFramePos = (Ipp8u*)Info.out->GetBufferPointer() + currFrameSize;
    Ipp32u frameSize = (Ipp32u)Info.out->GetBufferSize();

    if(currFrameSize + readBufSize - readDataSize <= frameSize )
    {
        ippsCopy_8u(readBuf + readDataSize, currFramePos, readBufSize - readDataSize);

        Info.out->SetDataSize(currFrameSize + readBufSize - readDataSize);
        Info.in->SetDataSize(0);
        return UMC_ERR_NOT_ENOUGH_DATA;
    }
    else
    {
        ippsCopy_8u(readBuf + readDataSize, currFramePos, frameSize - currFrameSize);

        Info.out->SetDataSize(frameSize);
        Info.in->SetDataSize(readDataSize + frameSize - currFrameSize);
        return UMC_OK;
    }
}

Status vc1_frame_constructor_rcv::ParseVC1SeqHeader (Ipp8u *data,
                                    Ipp32u* bufferSize,SplitterInfo* info)
{
    Status umcSts = UMC_OK;
    VideoStreamInfo* video_info = (VideoStreamInfo *)info->m_ppTrackInfo[0]->m_pStreamInfo;

    Ipp32u tempData;
    Ipp32u *  pbs;
    Ipp32s  bitOffset = 31;

    pbs = (Ipp32u *)data;
    *bufferSize = 0;

    video_info->iStreamPID = 0;
    video_info->streamType = VC1_VIDEO;
    video_info->streamSubtype = VC1_VIDEO_RCV;

    video_info->videoInfo.m_picStructure = PS_PROGRESSIVE;
    video_info->fFramerate = 0;
    video_info->iBitrate = 0;

    video_info->fDuration = 0;
    video_info->videoInfo.m_colorFormat = YUV420;

    //Simple/main profile
    video_info->videoInfo.m_iSAWidth = 1;
    video_info->videoInfo.m_iSAHeight = 1;

    //PROFILE
    VC1GetNBits(pbs, bitOffset, 2, tempData);

    //Simple/Main profile
    //LEVEL
    VC1GetNBits(pbs, bitOffset, 2, tempData);

    //FRMRTQ_POSTPROC
    VC1GetNBits(pbs, bitOffset, 3, video_info->fFramerate);

    //BITRTQ_POSTPROC
    VC1GetNBits(pbs, bitOffset, 5, video_info->iBitrate);

    //LOOPFILTER
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //reserved
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //MULTIRES
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //reserved
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //FASTUVMC
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //EXTENDED_MV
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //DQUANT
    VC1GetNBits(pbs, bitOffset, 2, tempData);

    //VSTRANSFORM
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //reserved
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //OVERLAP
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //SYNCMARKER
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //RANGERED
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //MAXBFRAMES
    VC1GetNBits(pbs, bitOffset, 3, tempData);

    //QUANTIZER
    VC1GetNBits(pbs, bitOffset, 2, tempData);

    //FINTERPFLAG
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    //last bit in seq header
    VC1GetNBits(pbs, bitOffset, 1, tempData);

    *bufferSize = (video_info->videoInfo.m_iHeight+15 +128)*
        (video_info->videoInfo.m_iWidth + 15 + 128)
        + (((video_info->videoInfo.m_iHeight+15)/2 + 64)*
        ((video_info->videoInfo.m_iWidth +15)/2 + 64))*2;

    video_info->iBitrate = 1024 * video_info->iBitrate;

    return umcSts;
}

Status vc1_frame_constructor_rcv::GetFirstSeqHeader(VC1FrameConstrInfo& Info)
{
    Status umcSts = UMC_ERR_NOT_ENOUGH_DATA;
    Ipp32u frameSize = 4;
    Ipp8u* ptemp = (Ipp8u*)Info.in->GetBufferPointer() + 4;
    Ipp32u temp_size = ((*(ptemp+3))<<24) + ((*(ptemp+2))<<16) + ((*(ptemp+1))<<8) + *(ptemp);


    frameSize+= temp_size;
    frameSize+=12;
    ptemp = (Ipp8u*)Info.in->GetBufferPointer() + frameSize;
    temp_size = ((*(ptemp+3))<<24) + ((*(ptemp+2))<<16) + ((*(ptemp+1))<<8) + *(ptemp);
    frameSize+= temp_size;
    frameSize+=4;

    Info.out->SetBufferPointer((Ipp8u*)Info.out->GetBufferPointer(),frameSize);

    umcSts = GetData(Info);

    return umcSts;
}

//Status vc1_frame_constructor_rcv::GetPicType(MediaDataEx* data, Ipp32u& picType)
//{
//    Ipp32u Ipp32u*   pbs = (Ipp32u*)data->GetDataPointer();
//    Ipp32s  bitOffset = 31;
//    Ipp32u tempData = 0;
//    return UMC_OK;
//}

///////////////////////////////////////////////////////////////////////////////////////////
vc1_frame_constructor_vc1::vc1_frame_constructor_vc1()
{
}


vc1_frame_constructor_vc1::~vc1_frame_constructor_vc1()
{
}

Status vc1_frame_constructor_vc1::GetNextFrame(VC1FrameConstrInfo& Info)
{
    Status umcSts = UMC_ERR_NOT_ENOUGH_DATA;
    umcSts = GetData(Info);
    return umcSts;
}

Status vc1_frame_constructor_vc1::GetData(VC1FrameConstrInfo& Info)
{
    Ipp32u readDataSize = (Ipp32u)Info.in->GetDataSize();
    Ipp32u readBufSize = (Ipp32u)Info.in->GetBufferSize();
    Ipp8u* readBuf = (Ipp8u*)Info.in->GetBufferPointer();

    Ipp8u* readPos = readBuf + (Ipp32u)Info.in->GetDataSize();

    Ipp32u frameSize = (Ipp32u)Info.out->GetDataSize();
    Ipp8u* currFramePos = (Ipp8u*)Info.out->GetBufferPointer() + frameSize;
    Ipp32u frameBufSize = (Ipp32u)Info.out->GetBufferSize();

    Ipp8u* ptr = currFramePos;
    Ipp32u zeroNum = 0;
    Ipp32s size = 0;
    Ipp32u a = 0x0000FF00 | (*readPos);
    Ipp32u b = 0xFFFFFFFF;

    while(readPos < (readBuf + readBufSize))
    {
        //find sequence of 0x000001 or 0x000003
        while(!( b == 0x00000001 || b == 0x00000003 )
                &&(readPos < (readBuf + readBufSize)))
        {
            readPos++;
            a = (a<<8)| (Ipp32s)(*readPos);
            b = a & 0x00FFFFFF;
        }

        //check end of read buffer
        if(readPos < (readBuf + readBufSize - 1))
        {
            if(*readPos == 0x01)
            {
                if((*(readPos + 1) == VC1_Slice) || (*(readPos + 1) == VC1_Field))
                {
                    readPos+=2;
                    ptr = readPos - 5;

                    if (Info.splMode != 2)
                    {
                        //trim zero bytes
                        while ( (*ptr==0) && (ptr > readBuf) )
                            ptr--;
                    }

                    //slice or field size
                    size = (Ipp32u)(ptr - readBuf - readDataSize+1);

                    if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                    ippsCopy_8u(readBuf + readDataSize, currFramePos, size);

                    currFramePos = currFramePos + size;
                    frameSize = frameSize + size;

                    if (Info.splMode == 1)
                    {
                        zeroNum = frameSize - 4*((frameSize)/4);
                        if(zeroNum!=0)
                            zeroNum = 4 - zeroNum;
                        memset(currFramePos, 0, zeroNum);
                    }

                    //set write parameters
                    currFramePos = currFramePos + zeroNum;
                    frameSize = frameSize + zeroNum;

                    Info.stCodes->offsets[Info.stCodes->count] = frameSize;
                    //stCodes->values[stCodes->count] =*(Ipp32u*)(readPos - 4);
                    Info.stCodes->values[Info.stCodes->count]  = ((*(readPos-1))<<24) + ((*(readPos-2))<<16) + ((*(readPos-3))<<8) + (*(readPos-4));

                    readDataSize = (Ipp32u)(readPos - readBuf - 4);

                    a = 0x00010b00 |(Ipp32s)(*readPos);
                    b = a & 0x00FFFFFF;

                    zeroNum = 0;
                    Info.stCodes->count++;
                }
                else if(Info.stCodes->count != 0)
                {
                    //end of frame
                    readPos = readPos - 2;
                    ptr = readPos - 1;

                    //trim zero bytes
                    if (Info.splMode != 2)
                    {
                        while ( (*ptr==0) && (ptr > readBuf) )
                            ptr--;
                    }

                    //slice or field size
                    size = (Ipp32u)(ptr - readBuf - readDataSize +1);

                    if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                    ippsCopy_8u(readBuf + readDataSize, currFramePos, size);
                    Info.out->SetDataSize(frameSize + size);

                    //prepare read buffer
                    size = (Ipp32u)(readPos - readBuf - readDataSize);
                    readDataSize = readDataSize + size;
                    Info.in->SetDataSize(readDataSize);

                    return UMC_OK;
                }
                else
                {
                    //beginning of frame
                    readPos++;
                    a = 0x00000100 |(Ipp32s)(*readPos);
                    b = a & 0x00FFFFFF;

                    Info.stCodes->offsets[Info.stCodes->count] = (Ipp32u)(0);
                    Info.stCodes->values[Info.stCodes->count]  = ((*(readPos))<<24) + ((*(readPos-1))<<16) + ((*(readPos-2))<<8) + (*(readPos-3));

                    Info.stCodes->count++;
                    zeroNum = 0;
                }
            }
            else //if(*readPos == 0x03)
            {
                //000003
                if((*(readPos + 1) <  0x04) && (Info.splMode == 1))
                {
                    size = (Ipp32u)(readPos - readBuf - readDataSize);

                    if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                    ippsCopy_8u(readBuf + readDataSize, currFramePos, size);
                    frameSize = frameSize + size;
                    currFramePos = currFramePos + size;
                    zeroNum = 0;

                    readPos++;
                    a = (a<<8)| (Ipp32s)(*readPos);
                    b = a & 0x00FFFFFF;

                    readDataSize = readDataSize + size + 1;
                }
                else
                {
                    readPos++;
                    a = (a<<8)| (Ipp32s)(*readPos);
                    b = a & 0x00FFFFFF;
                }
            }
        }
        else    //if (readPos >= (readBuf + readBufSize - 1 ))
        {
            if(readBufSize > 4)
            {
                readPos = readBuf + readBufSize;
                size = (Ipp32u)(readBufSize - readDataSize - 4);
                VM_ASSERT(size < 1024);

                if(size >= 0)
                {
                    if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                    ippsCopy_8u(readBuf + readDataSize, currFramePos, size);
                    Info.out->SetDataSize(frameSize + size);

                    if (Info.splMode != 2)
                    {

                        ptr = readBuf;
                        *ptr = *(readPos - 4);
                        ptr++;
                        *ptr = *(readPos - 3);
                        ptr++;
                        *ptr = *(readPos - 2);
                        ptr++;
                        *ptr = *(readPos - 1);

                        Info.in->SetDataSize(4);
                    }
                }
                else
                {
                    Info.out->SetDataSize(frameSize);

                    if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                    ippsCopy_8u(readBuf + readDataSize, readBuf, readBufSize - readDataSize);
                    Info.in->SetDataSize(readBufSize - readDataSize);
                }

                return UMC_ERR_NOT_ENOUGH_DATA;
            }
            else
            {
                //end of stream
                size = (Ipp32u)(readPos- readBuf - readDataSize);

                if(frameSize + size > frameBufSize)
                        return UMC_ERR_NOT_ENOUGH_BUFFER;

                ippsCopy_8u(readBuf + readDataSize, currFramePos, size);
                Info.out->SetDataSize(frameSize + size);

                Info.in->SetDataSize(size);
                return UMC_OK;
            }
        }
    }

    //field or slice start code
    if(frameSize + 4 > frameBufSize)
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    if(readBufSize > readDataSize)
    {
        memcpy(currFramePos,readPos - 4, 4);
        Info.out->SetDataSize(frameSize + 4);
        Info.in->SetDataSize(0);

        return UMC_ERR_NOT_ENOUGH_DATA;
    }
    else
        return UMC_ERR_END_OF_STREAM;
}

void vc1_frame_constructor_vc1::Reset()
{
}

Status vc1_frame_constructor_vc1::ParseVC1SeqHeader (Ipp8u *data,
                                Ipp32u* bufferSize,SplitterInfo* info)
{
    Status umcSts = UMC_OK;
    VideoStreamInfo* video_info = (VideoStreamInfo *)info->m_ppTrackInfo[0]->m_pStreamInfo;

    Ipp32u tempData;
    Ipp32u tempData1;
    Ipp32u i=0;

    Ipp32u *pbs;
    Ipp32s  bitOffset = 31;

    pbs = (Ipp32u *)data;
    *bufferSize = 0;
    video_info->iStreamPID = 0;
    video_info->streamType = VC1_VIDEO;
    video_info->streamSubtype = VC1_VIDEO_VC1;

    video_info->videoInfo.m_picStructure = PS_PROGRESSIVE;
    video_info->fFramerate = 0;
    video_info->iBitrate = 0;

    video_info->fDuration = 0;
    video_info->videoInfo.m_colorFormat = YUV420;
    video_info->videoInfo.m_iHeight = 0;
    video_info->videoInfo.m_iWidth = 0;

    //default value for advanced profile
    //Simple/main profile
    video_info->videoInfo.m_iSAWidth  = 1;
    video_info->videoInfo.m_iSAHeight = 1;

    //PROFILE
    VC1GetNBits(pbs, bitOffset, 2, tempData);

    if(tempData == VC1_PROFILE_ADVANCED)            //PROFILE
    {
        //LEVEL
        VC1GetNBits(pbs, bitOffset, 3, tempData);

        //CHROMAFORMAT
        VC1GetNBits(pbs, bitOffset, 2,tempData);

        //FRMRTQ_POSTPROC
        VC1GetNBits(pbs, bitOffset, 3, video_info->fFramerate);

        //BITRTQ_POSTPROC
        VC1GetNBits(pbs, bitOffset, 5, video_info->iBitrate);

        //for advanced profile
        if ((video_info->fFramerate == 0)
            && (video_info->iBitrate == 31))
        {
            //Post processing indicators for Frame Rate and Bit Rate are undefined
            video_info->iBitrate = 0;
        }
        else if ((video_info->fFramerate == 0)
            && (video_info->iBitrate == 30))
        {
            video_info->fFramerate =  2;
            video_info->iBitrate = 1952;
        }
        else if ((video_info->fFramerate == 1)
            && (video_info->iBitrate == 31))
        {
            video_info->fFramerate =  6;
            video_info->iBitrate = 2016;
        }
        else
        {

            if (video_info->fFramerate == 7)
                video_info->fFramerate = 30;
            else
                video_info->fFramerate = (2+video_info->fFramerate*4);

            if (video_info->iBitrate == 31)
                video_info->iBitrate = 2016;
            else
                video_info->iBitrate = (32 + video_info->iBitrate * 64);
        }


        //POSTPROCFLAG
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //MAX_CODED_WIDTH
        VC1GetNBits(pbs, bitOffset, 12,  video_info->videoInfo.m_iWidth);
        video_info->videoInfo.m_iWidth = (video_info->videoInfo.m_iWidth +1)*2;

        //MAX_CODED_HEIGHT
        VC1GetNBits(pbs, bitOffset, 12, video_info->videoInfo.m_iHeight);
        video_info->videoInfo.m_iHeight = (video_info->videoInfo.m_iHeight + 1)*2;

        //PULLDOWN
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //INTERLACE
        VC1GetNBits(pbs, bitOffset, 1, tempData);
        if(tempData)
            video_info->videoInfo.m_picStructure = PS_TOP_FIELD_FIRST;

        //TFCNTRFLAG
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //FINTERPFLAG
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //reserved
        VC1GetNBits(pbs, bitOffset, 2, tempData);

        //DISPLAY_EXT
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        if(tempData)   //DISPLAY_EXT
        {
            //DISP_HORIZ_SIZE
            VC1GetNBits(pbs, bitOffset, 14,tempData);

            //DISP_VERT_SIZE
            VC1GetNBits(pbs, bitOffset, 14,tempData);

            //ASPECT_RATIO_FLAG
            VC1GetNBits(pbs, bitOffset, 1,tempData);

            if(tempData)    //ASPECT_RATIO_FLAG
            {
                //ASPECT_RATIO
                VC1GetNBits(pbs, bitOffset, 4,tempData);

                if(tempData == 15)
                {
                    //ASPECT_HORIZ_SIZE
                    VC1GetNBits(pbs, bitOffset, 8,
                        video_info->videoInfo.m_iSAWidth);

                    //ASPECT_VERT_SIZE
                    VC1GetNBits(pbs, bitOffset, 8,
                        video_info->videoInfo.m_iSAHeight);
                }
                else
                {
                    video_info->videoInfo.m_iSAWidth =
                        AspectRatioTable[tempData].width;
                    video_info->videoInfo.m_iSAHeight =
                        AspectRatioTable[tempData].height;
                }
            }

            //FRAMERATE_FLAG
            VC1GetNBits(pbs, bitOffset, 1,tempData);

            if(tempData)       //FRAMERATE_FLAG
            {
                //FRAMERATEIND
                VC1GetNBits(pbs, bitOffset, 1,tempData);

                if(!tempData)      //FRAMERATEIND
                {
                    //FRAMERATENR
                    VC1GetNBits(pbs, bitOffset, 8,tempData);
                    video_info->fFramerate = FrameRateNumerator[tempData];

                    //FRAMERATEDR
                    VC1GetNBits(pbs, bitOffset, 4,tempData);
                    video_info->fFramerate =
                        video_info->fFramerate/FrameRateDenomerator[tempData];
                }
                else
                {
                    //FRAMERATEEXP
                    VC1GetNBits(pbs, bitOffset, 16,tempData);
                    video_info->fFramerate = (tempData + 1)/32.0;
                }
            }
                //COLOR_FORMAT_FLAG
                VC1GetNBits(pbs, bitOffset, 1,tempData);

                if(tempData)       //COLOR_FORMAT_FLAG
                {
                    //COLOR_PRIM
                    VC1GetNBits(pbs, bitOffset, 8,tempData);

                    //TRANSFER_CHAR
                    VC1GetNBits(pbs, bitOffset, 8,tempData);

                    //MATRIX_COEF
                    VC1GetNBits(pbs, bitOffset, 8,tempData);
                }
        }

        ////HRD_PARAM_FLAG
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        if(tempData)    //HRD_PARAM_FLAG
        {
            //HRD_NUM_LEAKY_BUCKETS
            VC1GetNBits(pbs, bitOffset, 5,tempData1);

            //BIT_RATE_EXPONENT
            VC1GetNBits(pbs, bitOffset, 4,tempData);

            //BUFFER_SIZE_EXPONENT
            VC1GetNBits(pbs, bitOffset, 4,tempData);

            //!!!!!!
            for(i=0; i < tempData1; i++)//HRD_NUM_LEAKY_BUCKETS
            {
                //HRD_RATE[i]
                VC1GetNBits(pbs, bitOffset, 16,tempData);

                //HRD_BUFFER[i]
                VC1GetNBits(pbs, bitOffset, 16,tempData);
            }
        }
    }
    else
    {
        //Simple/Main profile
        //LEVEL
        VC1GetNBits(pbs, bitOffset, 2, tempData);

        //FRMRTQ_POSTPROC
        VC1GetNBits(pbs, bitOffset, 3, video_info->fFramerate);

        //BITRTQ_POSTPROC
        VC1GetNBits(pbs, bitOffset, 5, video_info->iBitrate);

        //LOOPFILTER
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //reserved
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //MULTIRES
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //reserved
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //FASTUVMC
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //EXTENDED_MV
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //DQUANT
        VC1GetNBits(pbs, bitOffset, 2, tempData);

        //VSTRANSFORM
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //reserved
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //OVERLAP
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //SYNCMARKER
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //RANGERED
        VC1GetNBits(pbs, bitOffset, 1, tempData);

        //MAXBFRAMES
        VC1GetNBits(pbs, bitOffset, 3, tempData);

        //QUANTIZER
        VC1GetNBits(pbs, bitOffset, 2, tempData);

        //FINTERPFLAG
        VC1GetNBits(pbs, bitOffset, 1, tempData);
    }

    *bufferSize = (video_info->videoInfo.m_iHeight + 128)*
        (video_info->videoInfo.m_iWidth + 128)
        + ((video_info->videoInfo.m_iHeight/2 + 64)*
        (video_info->videoInfo.m_iWidth/2 + 64))*2;

    video_info->iBitrate = 1024 * video_info->iBitrate;

    return umcSts;
}

Status vc1_frame_constructor_vc1::GetFirstSeqHeader(VC1FrameConstrInfo& Info)
{
    Status umcSts = UMC_ERR_NOT_ENOUGH_DATA;

    umcSts = GetData(Info);

    if(umcSts == UMC_OK && Info.stCodes->values[0] != 0x0F010000)
    {
        umcSts = UMC_ERR_NOT_ENOUGH_DATA;
    }

    return umcSts;
}

//Status vc1_frame_constructor_vc1::GetPicType(MediaDataEx* data, Ipp32u& picType)
//{
//    Ipp32u Ipp32u*   pbs = (Ipp32u*)data->GetDataPointer();
//    Ipp32s  bitOffset = 31;
//    Ipp32u tempData = 0;

//    if(data->GetExData()->values[0]>>24 == FrameHeader)
//    {
//        if(m_videoStreamInfo.interlace_type)
//        {
//            //Frame Coding mode
//            //0 - progressive; 10 - Frame-interlace; 11 - Field-interlace
//            ippiGetNBits(pbs,bitOffset,1,tempData); //FCM

//            if(tempData)
//            {
//                ippiGetNBits(pbs,bitOffset,1,tempData); //FCM
//                tempData = tempData + 2;
//            }
//        }


//        if(tempData != FieldInterlace)
//        {
//            //picture type
//            //110 - I picture; 0 - P picture; 10 - B picture; 1110 - BI picture; 1111 - skipped
//            ippiGetNBits(pbs,bitOffset,1,tempData);
//            if(tempData)
//            {
//                ippiGetNBits(pbs,bitOffset,1,tempData);

//                if(tempData)
//                {
//                    ippiGetNBits(pbs,bitOffset,1,tempData);

//                    if(tempData)
//                    {
//                        ippiGetNBits(pbs,bitOffset,1,tempData);

//                        if(tempData)
//                            picType = SKIPPED_FRAME; //1111
//                        else
//                            picType = BI_FRAME; //1110
//                    }
//                    else
//                        picType = I_FRAME; //110
//                }
//                else
//                    picType = B_FRAME; //10
//            }
//            else
//                picType = P_FRAME; //0
//        }
//        else
//        {
//            //interlace field frame
//            ippiGetNBits(pbs,bitOffset,3,tempData);
//
//            switch(tempData)
//            {
//            case 0:
//                //000  - I,I
//                picType = I_I_FIELD_FRAME;
//                break;
//            case 1:
//                //001 - I,P
//                picType = I_P_FIELD_FRAME;
//                break;
//            case 2:
//                //010 - P,I
//                picType = P_I_FIELD_FRAME;
//                break;
//            case 3:
//                //011 - P,P
//                picType = P_P_FIELD_FRAME;
//                break;
//            case 4:
//                //100 - B,B
//                picType = B_B_FIELD_FRAME;
//                break;
//            case 5:
//                //101 - B,BI
//                picType = B_BI_FIELD_FRAME;
//                break;
//            case 6:
//                //110 - BI,B
//                picType = BI_B_FIELD_FRAME;
//                break;
//            case 7:
//                //111 - BI,BI
//                picType = BI_BI_FIELD_FRAME;
//                break;
//            default:
//                break;
//            }
//        }
//    }
//    else
//      return UMC_ERR_INVALID_STREAM;

//      return UMC_OK;
//}

#endif //UMC_ENABLE_VC1_SPLITTER
