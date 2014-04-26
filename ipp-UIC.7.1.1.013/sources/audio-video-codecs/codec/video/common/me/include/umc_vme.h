/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
//          motion estimation
//
*/

#ifndef _UMC_VME_H_
#define _UMC_VME_H_

//#define ME_VME
#ifdef ME_VME

#include "IntelVideoVME.h"
#include "umc_me.h"

struct Macroblock
{
    short    x,y;        // 16x16 macroblock coordinates
    I16PAIR    c;          // prediction center as absolute coordinates in Q-pel
    int        mode;        // 0: one 16x16, 1: two 16x8s, 2: two 8x16s, 3: four 8x8s
    int        dist;       // adjusted SAD distortion
    I16PAIR    mv[4];      // best motion vectors in differences from the prediction center for four 8x8 sub-blocks
};

namespace UMC
{
    class MeVme : public MeBase
    {
    public:
        DYNAMIC_CAST_DECL(MeVme, MeBase)

        MeVme();
        virtual ~MeVme();

        virtual bool Init(MeInitParams *par); //allocates memory
        virtual bool EstimateFrame(MeParams *par); //par and res should be allocated by caller, except MeFrame::MBs
        virtual void Close(); //frees memory, also called from destructor
    protected:
        IntelVideoVME    *vme;
        VMEOutput    vout;
        void EstimateMB(Ipp16s x, Ipp16s y, Ipp32s SKIP_TYPE);
    };

}//namespase UMC
#endif//ME_VME
#endif //_UMC_VME_H_
