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

#ifndef _ENCODER_VC1_PLANES_H_
#define _ENCODER_VC1_PLANES_H_

#include <new>

#include "umc_vc1_enc_def.h"
#include "umc_structures.h"
#include "umc_memory_allocator.h"

#include "ippvc.h"

namespace UMC_VC1_ENCODER
{
    //|--------------|//
    //|frame         |//
    //|  |-------|   |//
    //|  | plane |   |//
    //|  |       |   |//
    //|  |-------|   |//
    //|--------------|//

    class Frame
    {
    public:
        Frame ()
           {
            m_pYFrame = 0;
            m_pUFrame = 0;
            m_pVFrame = 0;
            m_pYPlane = 0;
            m_pUPlane = 0;
            m_stepY  = 0;
            m_pVPlane = 0;
            m_stepUV  = 0;
            m_widthYPlane  = 0;
            m_widthUVPlane  = 0;
            m_heightYPlane = 0;
            m_heightUVPlane = 0;
            m_paddingSize = 0;
            m_pictureType = VC1_ENC_I_FRAME;
            m_bTaken = false;
           };
           virtual ~Frame ()
           {
                Close ();
           }
           static Ipp32u CalcAllocatedMemSize(Ipp32u w, Ipp32u h, Ipp32u paddingSize);
           UMC::Status Init(Ipp8u* pBuffer, Ipp32s memSize, Ipp32u w, Ipp32u h, Ipp32u paddingSize);
           void        Close();

           UMC::Status PadFrameProgressive();
           UMC::Status PadFrameField(); //padding for field picture (2 fields)
           UMC::Status PadField(bool bBottomField); //padding for 1 field
           UMC::Status PadPlaneProgressive();
           UMC::Status PadPlaneField();

           UMC::Status CopyPlane ( Ipp8u* pYPlane, Ipp32u stepY,
                                   Ipp8u* pUPlane, Ipp32u stepU,
                                   Ipp8u* pVPlane, Ipp32u stepV,
                                   ePType pictureType=VC1_ENC_I_FRAME);
           UMC::Status CopyPlane (Frame * fr);
           inline void SetType ( ePType pictureType)
           {
                m_pictureType = pictureType;
                m_bTaken      = true;
           };
           inline ePType GetType ( )
           {
                return m_pictureType;
           };

           inline Ipp8u* GetYPlane() {return m_pYPlane;}
           inline Ipp8u* GetUPlane() {return m_pUPlane;}
           inline Ipp8u* GetVPlane() {return m_pVPlane;}
           inline Ipp32u GetYStep()  {return m_stepY;}
           inline Ipp32u GetUStep()  {return m_stepUV;}
           inline Ipp32u GetVStep()  {return m_stepUV;}
           inline void GetPictureSizeLuma(IppiSize *pSize)
           {
                pSize->height = m_heightYPlane;
                pSize->width  = m_widthYPlane;
           }
           inline void GetPictureSizeChroma(IppiSize *pSize)
           {
                pSize->height = m_heightUVPlane;
                pSize->width  = m_widthUVPlane;
           }
           inline ePType GetPictureType()
           {
               return m_pictureType;
           }
           inline bool   isTaken()
           {
              return m_bTaken;
           }

           inline void ReleasePlane ()
           {
                m_bTaken = false;
           }

           inline void SetReferenceFrameType()
           {
               m_pictureType = VC1_ENC_I_FRAME;
           }
           inline bool isReferenceFrame()
           {
               return (m_pictureType == VC1_ENC_I_FRAME ||
                       m_pictureType == VC1_ENC_P_FRAME  ||
                       m_pictureType == VC1_ENC_P_FRAME_MIXED ||
                       m_pictureType == VC1_ENC_SKIP_FRAME ||
                       m_pictureType == VC1_ENC_I_I_FIELD ||
                       m_pictureType == VC1_ENC_P_I_FIELD ||
                       m_pictureType == VC1_ENC_I_P_FIELD ||
                       m_pictureType == VC1_ENC_P_P_FIELD );
           }
           inline bool isIntraFrame()
           {
               return (m_pictureType == VC1_ENC_I_FRAME ||
                       m_pictureType == VC1_ENC_I_I_FIELD);
           }



    private:

        Ipp8u* m_pYFrame;
        Ipp8u* m_pUFrame;
        Ipp8u* m_pVFrame;

        Ipp32u m_stepY;
        Ipp32u m_stepUV;

        Ipp8u* m_pYPlane;
        Ipp8u* m_pUPlane;
        Ipp8u* m_pVPlane;

        Ipp32u m_widthYPlane;
        Ipp32u m_widthUVPlane;

        Ipp32u m_heightYPlane;
        Ipp32u m_heightUVPlane;

        Ipp32u m_paddingSize;

        ePType m_pictureType;
        bool   m_bTaken;
    };

    class StoredFrames
    {
    public:
        StoredFrames():
            m_nFrames(0),
            m_pFrames(0)
            {};

        static Ipp32u CalcAllocatedMemSize(Ipp32u nFrames, Ipp32u w, Ipp32u h, Ipp32u paddingSize)
        {
            Ipp32u memSize = 0;
            memSize += UMC::align_value<Ipp32u>(nFrames*sizeof(Frame));
            memSize += nFrames*UMC::align_value<Ipp32u>(Frame::CalcAllocatedMemSize(w, h, paddingSize));

            return memSize;
        }

        UMC::Status Init(Ipp8u* pBuffer, Ipp32s memSize, Ipp32u nFrames, Ipp32u w, Ipp32u h, Ipp32u paddingSize)
        {
            Ipp32u      i;
            UMC::Status ret = UMC::UMC_OK;

            Close();

            if(!pBuffer)
                return UMC::UMC_ERR_NULL_PTR;

            if(memSize == 0)
                return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

            m_nFrames = nFrames; // two reference frames
            m_pFrames = new (pBuffer) Frame[m_nFrames];
            if (!m_pFrames)
                return UMC::UMC_ERR_ALLOC;

            pBuffer += UMC::align_value<Ipp32u>(m_nFrames*sizeof(Frame));
            memSize -= UMC::align_value<Ipp32u>(m_nFrames*sizeof(Frame));
            if(memSize < 0)
                return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;

            Ipp32u frameSize = UMC::align_value<Ipp32u>(Frame::CalcAllocatedMemSize(w,h,paddingSize));
            for (i=0;i<m_nFrames;i++)
            {
                ret = m_pFrames[i].Init(pBuffer, memSize, w,h,paddingSize);
                if (ret != UMC::UMC_OK)
                    return ret;
                pBuffer += frameSize;
                memSize -= frameSize;
                if(memSize < 0)
                    return UMC::UMC_ERR_NOT_ENOUGH_BUFFER;
            }
            return ret;
        }
        void        Close()
        {
            //if (m_pFrames)
            //{
            //    delete [] m_pFrames;
            //    m_pFrames = 0;
            //}
            m_nFrames = 0;
        }

        ~StoredFrames()
        {
            Close();
        }

        void Reset()
        {
            Ipp32u i;
            if (m_pFrames)
                for (i = 0; i < m_nFrames; i++)
                {
                        m_pFrames[i].ReleasePlane();
                }
        }

        inline Frame* GetFreeFramePointer ()
        {
            Ipp32u i;
            for (i = 0; i < m_nFrames; i++)
            {
                if (!m_pFrames[i].isTaken())
                {
                    return &m_pFrames[i];
                }
            }
            return 0;
        }
        inline  Ipp32u      GetYStep()
        {
            return m_pFrames[0].GetYStep();

        }

    private:
        Ipp32u  m_nFrames;
        Frame  *m_pFrames;
        bool   *m_pTaken;
    };

    class GOP
    {
    public:
        GOP()
          {
            m_maxB = 0;
            m_pInFrames = 0;
            m_numBuffB = 0;
            m_pOutFrames = 0;
            m_iCurr = 0;
          }

        virtual ~GOP()
        {
            Close();
        }
        inline bool isSkipFrame(Frame* inFrame, bool bOriginal, Ipp32u th=0)
        {
            th = th;
            bOriginal = bOriginal;
            inFrame = inFrame;
            return false;
        }

        inline Ipp32u GetNumberOfB()
        {
            return m_numBuffB;
        }
    protected:
        Ipp32u  m_maxB;
        Frame** m_pInFrames; /* 0     - forward  reference frame,
                                1     - backward reference frame
                                other - B frames*/
        Frame** m_pOutFrames;
        Ipp32u  m_numBuffB;
        Ipp32u  m_iCurr;

    protected:
        void Close()
        {
            if (m_pInFrames)
            {
                delete [] m_pInFrames;
                m_pInFrames = 0;
            }
            if (m_pOutFrames)
            {
               delete [] m_pOutFrames;
               m_pOutFrames = 0;
            }
            m_iCurr = 0;
            m_numBuffB = 0;
            m_maxB = 0;
        }

        inline bool NewGOP ()
        {

            m_pInFrames[0] ->ReleasePlane();
            m_pOutFrames[0]->ReleasePlane();

            m_pInFrames[0]   = m_pInFrames[1];
            m_pOutFrames[0]  = m_pOutFrames[1];

            m_pInFrames[1]   = 0;
            m_pOutFrames[1]  = 0;


            m_iCurr = 1;
            m_numBuffB = 0;
            return true;
        }

        inline virtual bool AddBFrame(Frame* inFrame,Frame* outFrame)
        {
            if (m_numBuffB < m_maxB &&
                m_pInFrames[0] != 0  && m_pInFrames[1] ==0&&
                m_pOutFrames[0] != 0 && m_pOutFrames[1]==0)
            {
                m_pInFrames[m_numBuffB+2]  = inFrame;
                m_pOutFrames[m_numBuffB+2] = outFrame;
                m_numBuffB ++;
                return true;
            }
            return false;
        }
        inline virtual bool AddReferenceFrame(Frame* inFrame,Frame* outFrame)
        {
            if (m_pInFrames[0] == 0 && m_pOutFrames[0] == 0)
            {
                m_pInFrames[0]  = inFrame;
                m_pOutFrames[0] = outFrame;
                return true;
            }
            else if (m_pInFrames[1] == 0 && m_pOutFrames[1] == 0)
            {
                m_pInFrames[1]  = inFrame;
                m_pOutFrames[1] = outFrame;
                return true;
            }
            return false;
        }

    public:

        inline bool AddFrame(Frame* inFrame, Frame* outFrame=0)
        {
            if (outFrame)
            {
                if (inFrame->isReferenceFrame())
                {
                    outFrame->SetType(inFrame->GetType());
                    return AddReferenceFrame(inFrame,outFrame);
                }
                else
                {
                    return AddBFrame(inFrame,inFrame);
                }
            }
            else
            {
                if (inFrame->isReferenceFrame())
                {
                    return AddReferenceFrame(inFrame,inFrame);
                }
                else
                {
                    return AddBFrame(inFrame,inFrame);
                }

            }
        }
        inline virtual UMC::Status Init(Ipp32u maxB)
        {
            Close();
            m_maxB = maxB;
            m_pInFrames = new Frame* [m_maxB+2];
            if (!m_pInFrames)
                return UMC::UMC_ERR_ALLOC;
            m_pOutFrames =  new Frame* [m_maxB+2];
            if (!m_pOutFrames)
                return UMC::UMC_ERR_ALLOC;
            Reset();
            return UMC::UMC_OK;
        }

        inline virtual void Reset()
        {
            if (m_pInFrames)
            {
                memset(m_pInFrames,0,sizeof(Frame*)*(m_maxB+2));
            }
            if (m_pOutFrames)
            {
                memset(m_pOutFrames,0,sizeof(Frame*)*(m_maxB+2));
            }
            m_iCurr = 0;
            m_numBuffB = 0;
        };

        inline Frame* GetInFrameForDecoding()
        {
            return m_pInFrames[m_iCurr];
        }
        inline Frame* GetOutFrameForDecoding()
        {
            return m_pOutFrames[m_iCurr];
        }
        inline  virtual void ReleaseCurrentFrame()
        {
            if (!m_pInFrames[m_iCurr] || !m_pOutFrames[m_iCurr])
                return;

            if (m_iCurr>1)
            {
                m_pInFrames[m_iCurr]->ReleasePlane();
                m_pOutFrames[m_iCurr]->ReleasePlane();
                m_pInFrames[m_iCurr] = 0;
                m_pOutFrames[m_iCurr] = 0;
            }
            m_iCurr++;
            if (m_iCurr>=m_numBuffB+2)
            {
                NewGOP();
            }
        }
        inline  virtual void CloseGop(ePType pPictureType=VC1_ENC_I_FRAME)
        {
            if (m_pInFrames[1]==0 && m_numBuffB>0)
            {
                m_pInFrames[1]            = m_pInFrames[2+m_numBuffB-1];
                m_pOutFrames[1]           = m_pOutFrames[2+m_numBuffB-1];
                m_pInFrames[2 + m_numBuffB-1] = 0;
                m_numBuffB --;
                m_pInFrames[1]->SetType(pPictureType);
            }
        }
        inline Frame* GetInReferenceFrame(bool bBackward = false)
        {
            return m_pInFrames[bBackward];
        }
        inline Frame* GetOutReferenceFrame(bool bBackward = false)
        {
            return m_pOutFrames[bBackward];
        }
        inline virtual ePType GetPictureType(Ipp32u frameCount,Ipp32u GOPLength, Ipp32u BFrmLength)
        {
            Ipp32s      nFrameInGOP        =  (frameCount++) % GOPLength;

            if (nFrameInGOP)
            {
                if ( nFrameInGOP %(BFrmLength+1)==0)
                    return VC1_ENC_P_FRAME;
                else
                    return VC1_ENC_B_FRAME;
            }
            return VC1_ENC_I_FRAME;
        }
    private:
        GOP(const GOP&) {}
        const GOP& operator=(const GOP&) { return *this; }
    };
    class GOPWithoutReordening : public GOP
    {
    public:

        inline virtual UMC::Status Init(Ipp32u maxB)
        {
            return GOP::Init(maxB>0);
        }

    protected:
        inline virtual bool AddBFrame(Frame* inFrame, Frame* outFrame)
        {
            if (m_numBuffB < m_maxB &&
                m_pInFrames[0] != 0 && m_pInFrames[1]!=0 &&
                m_pOutFrames[0] != 0 && m_pOutFrames[1]!=0)
            {
                m_pInFrames[m_numBuffB+2]  = inFrame;
                m_pOutFrames[m_numBuffB+2] = outFrame;
                return true;
            }
            return false;
        }
        inline virtual bool AddReferenceFrame(Frame* inFrame, Frame* outFrame)
        {
            if (!GOP::AddReferenceFrame(inFrame,outFrame))
            {
                NewGOP();
                return GOP::AddReferenceFrame(inFrame, outFrame);
            }
            return true;
        }
    public:
        inline virtual ePType GetPictureType(Ipp32u frameCount,Ipp32u GOPLength, Ipp32u BFrmLength)
        {
            Ipp32s      nFrameInGOP        =  (frameCount) % GOPLength;

            if (nFrameInGOP)
            {
                if ( (nFrameInGOP-1) %(BFrmLength+2)==0)
                    return VC1_ENC_P_FRAME;
                else
                    return VC1_ENC_B_FRAME;
            }
            return VC1_ENC_I_FRAME;
        }



        inline virtual void ReleaseCurrentFrame()
        {
            if (!m_pInFrames[m_iCurr] || !m_pOutFrames[m_iCurr])
                return;
            if (m_iCurr>1)
            {
                m_pInFrames[m_iCurr]->ReleasePlane();
                m_pOutFrames[m_iCurr]->ReleasePlane();
                m_pInFrames[m_iCurr] = 0;
                m_pOutFrames[m_iCurr] = 0;
            }
            else
            {
                m_iCurr++;
            }

        }
        inline virtual void CloseGop()
        {
            return;
        }
     };

    class WaitingList
    {
    public:
        WaitingList():
            m_maxN(0),
            m_pFrame(0),
            m_curIndex(0),
            m_nFrames(0)
            {};
        ~WaitingList()
        {Close();}
        void Close()
        {
            if (m_pFrame)
            {
                delete m_pFrame;
                m_pFrame = 0;
            }
            m_maxN = 0;
            m_curIndex = 0;
            m_nFrames = 0;
        }

        UMC::Status Init(Ipp32u maxB)
        {
            Close();
            m_maxN = maxB+2;
            m_pFrame = new Frame* [ m_maxN];
            if (!m_pFrame)
                return UMC::UMC_ERR_ALLOC;
            memset (m_pFrame,0, sizeof(Frame*)* m_maxN);
            return UMC::UMC_OK;
        }

        void Reset()
        {
            Ipp32u i = 0;
            if(m_pFrame)
                memset (m_pFrame,0, sizeof(Frame*)* m_maxN);
            m_curIndex = 0;
            m_nFrames = 0;
            if(m_pFrame)
                for(i = 0; i < m_maxN; i++)
                {
                    if(m_pFrame[i])
                        m_pFrame[i]->ReleasePlane();
                }
        }

        bool AddFrame(Frame* frame)
        {
            if (m_nFrames < m_maxN)
            {
                Ipp32u ind = (m_curIndex + m_nFrames)%m_maxN;
                m_pFrame[ind] = frame;
                m_nFrames ++;
                return true;
            }
            return false;
        }
        Frame* GetCurrFrame()
        {
            Frame* frm = 0;
            if (m_nFrames>0)
            {
                frm = m_pFrame[m_curIndex];
            }
            return frm;
        }
        bool MoveOnNextFrame()
        {
            if (m_nFrames>0)
            {
                m_pFrame[m_curIndex] = 0;
                m_curIndex = (++m_curIndex)%m_maxN;
                m_nFrames--;
                return true;
            }
            return false;
        }

    private:
        Ipp32u  m_maxN;
        Frame** m_pFrame;
        Ipp32u  m_curIndex;
        Ipp32u  m_nFrames;
    private:
        WaitingList(const WaitingList&) {}
        const WaitingList& operator=(const WaitingList&) { return *this; }
    };

    class BufferedFrames
    {

    public:
        BufferedFrames():
            m_pFrames(0),       //sequence of frames
            m_bufferSize (0),   //number of frames in sequence
            m_nBuffered (0),
            m_currFrameIndex (0),
            m_bClosed (false) // closed sequence (if the backward reference frame is present)
            {};

         virtual ~BufferedFrames()
         {
            Close();
         }
         static Ipp32u CalcAllocatedMemSize(Ipp32u w, Ipp32u h, Ipp32u paddingSize, Ipp32u n);
         UMC::Status    Init  (Ipp8u* pBuffer, Ipp32u memSize,
                               Ipp32u w, Ipp32u h, Ipp32u paddingSize, Ipp32u n);
         void           Close();
         UMC::Status    SaveFrame           (Ipp8u* pYPlane, Ipp32u stepY,
                                            Ipp8u* pUPlane, Ipp32u stepU,
                                            Ipp8u* pVPlane, Ipp32u stepV );

         UMC::Status    GetFrame            (Frame** currFrame);
         UMC::Status    GetReferenceFrame   (Frame** currFrame);

        UMC::Status     ReferenceFrame();

         inline bool isClosedSequence ()
         {
            return m_bClosed;
         }
         inline bool isBuffered()
         {
            return (m_nBuffered!=0);
         }
    private:
        Frame* m_pFrames;
        Ipp32u m_bufferSize;
        Ipp32u m_nBuffered;
        Ipp32u m_currFrameIndex;
        bool   m_bClosed; // closed sequence (if the backward reference frame is present)
    };
}

#endif
