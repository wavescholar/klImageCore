/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/


#ifndef KL_IMAGE_FUNCTOR_H
#define KL_IMAGE_FUNCTOR_H

#include "kl_image_buffer.h"

//ppm_helper include
//#include "ppm_helper.h"  Used for debugging below


template <class TYPE> class klCopyFunctor{
public:

	//Default for band parameter copies all bands, otherwise parameter indicates the desired band
	//band is a 0-based index.
	klCopyFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst, int lband=-1)
		: src(lsrc), dst(ldst), band(lband)
	{

	}
	void operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		if((band>0)&& (band>inbands))
			throw " : CopyFunctor: invalid band specified.";

		unsigned int i;
		unsigned int j;
		unsigned int b;

		TYPE* inbuf=(TYPE*)src->buffer();

		TYPE* outbuf=(TYPE*)dst->buffer();

		unsigned typeSize=sizeof(TYPE);
		unsigned inyOffset=inyStride/typeSize;
		unsigned inxOffset=inxStride/typeSize;

		unsigned outyOffset=outyStride/typeSize;
		unsigned outxOffset=outxStride/typeSize;

		if(band==-1) //do all bands
		{
			if(inbands!=outbands || inwidth!=outwidth || inheight!=outheight)
				throw ": CopyFunctor : in and out dimensions need to be the same for default copy.";

			for (i=0; i<inheight; i++)
			{
				for (j=0; j<inwidth; j++)
				{
					for(b=0;b<inbands;b++)
					{
						*(outbuf+i*outyOffset + j*outxOffset +b)=*(inbuf+i*inyOffset+j*inxOffset+b);
					}
				}
			}
		}
		else
		{
			if(outbands!=1 || inwidth!=outwidth || inheight!=outheight)
				throw ": CopyFunctor : out bands needs to be one and in and out dimensions need to be the same for band copy.";

			if(band>inbands)
				throw ": CopyFunctor : invalid copy band specified.";

			for (i=0; i<inheight; i++)
			{
				for (j=0; j<inwidth; j++)
				{
					*(outbuf+i*outyOffset + j*outxOffset)=*(inbuf+i*inyOffset+j*inxOffset+band);

				}
			}
		}
	}
protected:

	klRasterBufferPointer src;

	klRasterBufferPointer dst;

	int band;

};

template<class TYPE_in,class TYPE_out> class klTypeConvertFunctor{
public:
	//Auto-scaling for floating point output assumes a 0,1 range is desired for min and max of the data.
	//The min and max of the input is found by iterating over input data.  Other caveats apply.  See the implementation for details.
	klTypeConvertFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst, bool ldoScaling=false,
		TYPE_in linmin=0,TYPE_in linmax=0,
		TYPE_out loutmin=0,TYPE_out loutmax=0)
		: src(lsrc), dst(ldst), doScaling(ldoScaling), inmin(linmin), inmax(linmax), outmin(loutmin), outmax(loutmax)  
	{

	}

	void operator()(void)
	{
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		if(inbands != outbands)
			throw "klTypeConvertFunctor: TypeConvertFunctor: in and out bands must be the same.";

		if(inwidth!=outwidth || inheight != outheight)
			throw "klTypeConvertFunctor: TypeConvertFunctor: in and out  dimensions must be the same";

		size_t inBytes=inxStride/inbands;
		size_t outBytes=outxStride/outbands;

		unsigned int i;
		unsigned int j;
		unsigned int b;
		TYPE_in* inbuf=(TYPE_in*)src->buffer();
		TYPE_out* outbuf=(TYPE_out*)dst->buffer();

		if(doScaling)
		{
			double tmin=DBL_MAX;
			double tmax=-1.7976931348623e+306;//close to -DBL_MAX
			for (i=0; i<outheight; i++)
			{
				for (j=0; j<outwidth; j++)
				{
					for(b=0;b<outbands;b++)
					{

						TYPE_in in=*(inbuf+ i*inwidth + j*inbands + b);
						if(in<tmin)
							tmin=in;
						if(in>tmax)
							tmax=in;

					}
				}
			}
			inmin=(TYPE_in)tmin;
			inmax=(TYPE_in)tmax;

			switch(sizeof(TYPE_out) )
			{
			case 1:
				{
					outmin=0;
					if(typeid(TYPE_out)==typeid(unsigned char) )
					{
						outmax=UCHAR_MAX;
					}
					else
						outmax=SCHAR_MAX;

					break;
				}
			case 2:
				{
					outmin=0;
					if(typeid(TYPE_out)==typeid(unsigned short) )
					{
						outmax=(TYPE_out)USHRT_MAX;
					}
					else
						outmax=(TYPE_out)SHRT_MAX;
					break;
				}
			case 4:
				{
					if(typeid(TYPE_out)==typeid(float) )
					{
						outmin=(TYPE_out)0.0;
						outmax=(TYPE_out)1.0;
					}
					if(typeid(TYPE_out)==typeid(int) || typeid(TYPE_out)==typeid(unsigned int) )
					{
						outmin=0;
					}
					break;
				}
			case 8:
				{
					outmin=(TYPE_out)0.0;
					outmax=(TYPE_out)1.0;
					break;
				}
			default:
				{
					throw "klTypeConvertFunctor  : scaling mode only applies for char, short, float, double output data types.";
				}
			}
		}

		if(inmin==inmax)
		{
			switch(sizeof(TYPE_in) )
			{
			case 1:
				{
					inmin=0;
					if(typeid(TYPE_in)==typeid(unsigned char) )
					{
						inmax=UCHAR_MAX;
					}
					else
						inmax=SCHAR_MAX;

					break;
				}
			case 2:
				{
					inmin=0;
					if(typeid(TYPE_in)==typeid(unsigned short) )
					{
						inmax=(TYPE_in)USHRT_MAX;
					}
					else
						inmax=(TYPE_in)SHRT_MAX;
					break;
				}
			case 4:
				{
					if(typeid(TYPE_in)==typeid(float) )
					{
						inmin=(TYPE_in)0.0;
						inmax=(TYPE_in)1.0;
					}
					if(typeid(TYPE_in)==typeid(int) || typeid(TYPE_in)==typeid(unsigned int) )
					{
						inmin=0;
						inmax=UCHAR_MAX;  
					}
					break;
				}
			case 8:
				{
					inmin=(TYPE_in)0.0;
					inmax=(TYPE_in)1.0;
					break;
				}
			default:
				{
					;
				}
			}
		}

		double scale=   double(outmax-outmin)/double(inmax-inmin) ;

		for (i=0; i<outheight; i++)
		{
			for (j=0; j<outwidth; j++)
			{
				for(b=0;b<outbands;b++)
				{
					TYPE_in in=*(inbuf+ i*(inyStride/sizeof(TYPE_in) ) + j* (inxStride/sizeof(TYPE_in) ) + b );
					TYPE_out t= TYPE_out( (in)*scale +outmin) ;
					*(outbuf+i*(outyStride/sizeof(TYPE_out) ) +j* (outxStride/ sizeof(TYPE_out) ) + b ) = t;
				}
			}
		}

	}
protected:
	klRasterBufferPointer src;

	klRasterBufferPointer dst;

	TYPE_in inmin;

	TYPE_in inmax;

	TYPE_out outmin;

	TYPE_out outmax;

	bool doScaling;
};

template <class TYPE> class klThresholdFunctor{
public:
	//Maps [0,thresholdLow] U [thresholdHigh,MAXVAL(TYPE) ] -----> LowVal , HighVal
	klThresholdFunctor(klRasterBufferPointer lsrc, double lthresholdHi=1.0,double lthresholdLow=0,
		bool luseLowThreshold=false,bool luseHighThreshold=true, 
		double lthresholdHiVal=1.0,double lthresholdLowVal=0.0,bool luseThresholdVals=false)
		: src(lsrc), thresholdLow(lthresholdLow), thresholdHi(lthresholdHi), 
		useLowThreshold(luseLowThreshold), useHighThreshold(luseHighThreshold), useThresholdVals(luseThresholdVals)

	{
		if(!useThresholdVals)
		{
			thresholdHiVal=thresholdHi;
			thresholdLowVal=thresholdLow;
		}
		else
		{
			thresholdHiVal=lthresholdHiVal;
			thresholdLowVal=lthresholdLowVal;
		}

	}
	void operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		unsigned int i;
		unsigned int j;
		unsigned int b;

		unsigned char* buf=src->buffer();

		for (i=0; i<inheight; i++)
		{
			for (j=0; j<inwidth; j++)
			{
				for(b=0;b<inbands;b++)
				{
					TYPE in=*(buf+ i*inyStride + j*inxStride + b);
					if(useHighThreshold && in>thresholdHi)
						in=(TYPE)thresholdHiVal;
					if(useLowThreshold && in<thresholdLow)
						in=thresholdLowVal;
					*(buf+ i*inyStride + j*inxStride + b)=in;
				}
			}
		}

	}
protected:
	klRasterBufferPointer src;

	double thresholdHi;

	double thresholdLow;

	double thresholdHiVal;

	double thresholdLowVal;

	bool useLowThreshold;

	bool useHighThreshold;

	bool useThresholdVals;

};

struct padRect;  
class klPadFunctor{
public:

	struct padRect {
		padRect(unsigned lx0,unsigned ly0,unsigned lw,unsigned lh)
		{x0=lx0;y0=ly0;w=lw;h=lh;}

		unsigned x0;
		unsigned y0;
		unsigned w;
		unsigned h;};

		enum padType { PAD_TYPE_MIRROR, PAD_TYPE_WRAP, PAD_TYPE_CONST};

		klPadFunctor(klRasterBufferPointer lsrc,klRasterBufferPointer ldst, int lpadType, padRect lrect,double lconstVal=0)
			: src(lsrc), dst(ldst), padType(lpadType), rect(lrect), constVal(lconstVal)
		{

		}

		void operator()(void);
protected:
	klRasterBufferPointer src;

	klRasterBufferPointer dst;

	int padType;

	padRect rect;

	double constVal;
};

#include "ipp.h"
class klLogicalFunctor{
public:

	enum LogicalFunctorOperationType {AND,OR,NOT};

	//Pass in NULL for the second source buffer when performing the not operation.
	//For in place operation, pass the first source buffer as the destination parameter.
	klLogicalFunctor(klRasterBufferPointer lsrc1, klRasterBufferPointer lsrc2, klRasterBufferPointer ldst, int ltype)
		: src1(lsrc1), src2(lsrc2), dst(ldst), type(LogicalFunctorOperationType(ltype) )
	{

	}
	void operator()(void)
	{
		unsigned char* srcBuf1=src1->buffer();
		size_t inbands = src1->numBands();
		size_t inwidth = src1->width();
		size_t inheight= src1->height(); 
		size_t inbandStride=src1->bandStride();
		size_t inxStride = src1->xStride();
		size_t inyStride = src1->yStride();

		unsigned char* srcBuf2;
		size_t inbands2;
		size_t inwidth2 ;
		size_t inheight2;
		size_t inbandStride2;
		size_t inxStride2;
		size_t inyStride2;

		if(type != klLogicalFunctor::LogicalFunctorOperationType::NOT )
		{
			srcBuf2=src2->buffer();
			inbands2=src2->numBands();
			inwidth2 =src2->width();
			inheight2=src2->height(); 
			inbandStride2=src2->bandStride();
			inxStride2 =src2->xStride();
			inyStride2= src2->yStride();

			if( inwidth2!=inwidth ||inheight2!=inheight)
				throw " klLogicalFunctor : source images have inconsistent dimensions.";

			if(src2->sampleBits()!=8)
				throw " klLogicalFunctor : bad bitdepth in input image";


		}
		if(inbands!=1 || (type != klLogicalFunctor::LogicalFunctorOperationType::NOT && inbands2!=1) )
			throw " klLogicalFunctor : only since band images supported at this time.";

		unsigned char* dstBuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		if(dst->sampleBits()!=8)
			throw " klLogicalFunctor : bad bitdepth in ouput image";

		if(src1->sampleBits()!=8)
			throw " klLogicalFunctor : bad bitdepth in input image";

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=dst->width();

		dstRoiSize.height=dst->height();

		switch(type)
		{
		case 0 :
			{
				if(srcBuf1==dstBuf)
					ippStatusResult=ippiAnd_8u_C1IR(srcBuf2, inyStride2 ,	srcBuf1, inyStride, dstRoiSize);
				else
					ippStatusResult=ippiAnd_8u_C1R(	srcBuf1, inyStride,srcBuf2, inyStride2 , dstBuf, outyStride, dstRoiSize);
				break;
			}
		case 1 :
			{

				if(srcBuf1==dstBuf)
					ippStatusResult=ippiOr_8u_C1IR(srcBuf2, inyStride2 ,	srcBuf1, inyStride, dstRoiSize);
				else
					ippStatusResult=ippiOr_8u_C1R(	srcBuf1, inyStride,srcBuf2, inyStride2 , dstBuf, outyStride, dstRoiSize);
				break;
			}
		case 2 :
			{
				if(srcBuf1==dstBuf)
					ippStatusResult=ippiNot_8u_C1IR(srcBuf1, inyStride, dstRoiSize);
				else
					ippStatusResult=ippiNot_8u_C1R(	srcBuf1, inyStride,dstBuf, outyStride, dstRoiSize);
				break;
			}
		default :
			{
				throw "  invalid LogicalFunctorOperationType parameter.";
			}

			if(ippStatusResult != ippStsNoErr)
			{
				throw "  Bad ipp result in klLogicalFunctor.";
			}
		}
	}
protected:
	klRasterBufferPointer src1;

	klRasterBufferPointer src2;

	klRasterBufferPointer dst;

	LogicalFunctorOperationType type;

};


//A general convolution.  This buffer op handles unsigned char and float types and 1 and 3 band image data.
template< class TYPE > class klFilterFunctor{
public:
	//If the kernel width or height is one, the seperable routines are called.
	//This is an in-place buffer op in that the source data is overwritten with the convolution results.
	klFilterFunctor(klRasterBufferPointer lsrc, TYPE* lkernel, unsigned lkernelWidth, unsigned lkernelHeight)
		: src(lsrc), kernel(lkernel), kernelWidth(lkernelWidth), kernelHeight(lkernelHeight)
	{

	}

	void operator()(void)
	{

		if(src->numBands() !=1 && src->numBands()!=3)
			throw ": klFilterFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned char* inbuf=src->buffer();
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		//Prep padded image for ipp.
		unsigned x0=2*kernelWidth;
		unsigned y0=2*kernelHeight;

		klRasterBufferPointer dst=new klPackedHeapRasterBuffer<TYPE>(inwidth+5*x0,inheight+5*y0,inbands);  

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		klPadFunctor::padRect padRect(x0, y0, inwidth,inheight);

		klPadFunctor padFn(src, dst, klPadFunctor::padType::PAD_TYPE_MIRROR, padRect); 

		unsigned padOffset=(dst->yStride() * padRect.y0+ padRect.x0 *dst->xStride()  );

		padFn();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=src->width();

		dstRoiSize.height=src->height();

		IppiSize kernelSize;

		kernelSize.width=kernelWidth;

		kernelSize.height=kernelHeight;

		unsigned anchorX=((int)std::floor((double)kernelWidth/2)%2==0) ? std::floor((double)kernelWidth/2)+1 : std::floor((double)kernelWidth/2) ;

		unsigned anchorY=((int)floor((double)kernelHeight/2)%2==0) ? floor((double)kernelHeight/2)+1 : floor((double)kernelHeight/2) ; 

		IppiPoint anchor;

		anchor.x=anchorX;

		anchor.y=anchorY;

		const type_info& typeof=typeid(TYPE);

		if(typeid(TYPE)== typeid(float))
		{
			unsigned char* pSrcf=dst->buffer()+padOffset;

			float* pKernel = (float*)kernel;

			switch(inbands)
			{
			case(1):
				{
					if(kernelWidth==1)
					{
						ippStatusResult =	ippiFilterColumn_32f_C1R(
							(float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel, kernelHeight,anchorY);
					}
					else if(kernelHeight==1)
					{
						ippStatusResult =	ippiFilterRow_32f_C1R(
							(float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel, kernelWidth,anchorX);

					}
					else
					{
						ippStatusResult = ippiFilter_32f_C1R((float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel,  kernelSize,anchor);						
						//	klRasterBufferPointer temp=new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight,1,alignment);
						//	klTypeConvertFunctor<float, unsigned char> typeConvFn(src,temp,false, 0.0,1.0, 0,255);  
						//	typeConvFn();
					}
					break;
				}
			case(3):
				{
					if(kernelWidth==1)
					{
						ippStatusResult =	ippiFilterColumn_32f_C3R(
							(float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel, kernelHeight,anchorY);

					}
					if(kernelHeight==1)
					{
						ippStatusResult =	ippiFilterRow_32f_C3R(
							(float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel, kernelWidth,anchorX);

					}
					else
					{

						ippStatusResult = ippiFilter_32f_C3R((float*)pSrcf, dst->yStride(),
							(float*)src->buffer(), src->yStride(),
							dstRoiSize, pKernel,  kernelSize,anchor);						
					}
					break;

				}
			}//end switch inbands

		}//end float case

		else if(typeid(TYPE)==typeid(unsigned char))
		{
			unsigned char* pSrcc =  dst->buffer()+padOffset;

			int* h=new int[kernelWidth*kernelHeight];
			unsigned i;
			unsigned j;
			for(i=0;i<kernelWidth;i++)
			{
				for(j=0;j<kernelHeight;j++)
				{
					*(h + i*kernelWidth + j) =*(kernel + i*kernelWidth + j);
				}
			}


			switch(inbands)
			{
			case(1):
				{
					if(kernelWidth==1)
					{
						ippStatusResult =	ippiFilterColumn_8u_C1R(
							pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h, kernelHeight,anchorY,255);
					}
					else if(kernelHeight==1)
					{
						ippStatusResult =	ippiFilterRow_8u_C1R(
							pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h, kernelWidth,anchorX,255);

					}
					else
					{
						ippStatusResult = ippiFilter_8u_C1R(pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h,  kernelSize,anchor,255);						
					}
					break;
				}
			case(3):
				{
					if(kernelWidth==1)
					{
						ippStatusResult =	ippiFilterColumn_8u_C3R(
							pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h, kernelHeight,anchorY,255);

					}
					if(kernelHeight==1)
					{
						ippStatusResult =	ippiFilterRow_8u_C3R(
							pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h, kernelWidth,anchorX, 255);

					}
					else
					{
						ippStatusResult = ippiFilter_8u_C3R(pSrcc, dst->yStride(),
							(unsigned char*)src->buffer(), src->yStride(),
							dstRoiSize, h,  kernelSize,anchor, 255);						
					}
					break;

				}
			}//end switch inbands

			delete[] h;

			if(ippStatusResult != ippStsNoErr)
				throw ": klFilterFunctor : ipp status error in convolution routine.";

		}//end unsigned char case
		else
			throw " : klFilterFunctor : unsupported data type, float and unsigned char supported at this time.";
	}//end operator() 
protected:
	klRasterBufferPointer src;
	TYPE* kernel;
	const unsigned  kernelWidth;
	const unsigned  kernelHeight;

};


template< class TYPE > class klFillHolesFunctor{
public:

	klFillHolesFunctor(klRasterBufferPointer lsrc,TYPE lbackgroundcolor=0,TYPE lforegroundcolor=255)
		: src(lsrc), backgroundcolor(lbackgroundcolor), foregroundcolor(lforegroundcolor)
	{

	}

	void operator()(void){

		if(typeid(TYPE)!=typeid(unsigned char) && typeid(TYPE)!=typeid(float) )
			throw ": klFillHolesFunctor<TYPE> only implemented for unsigned char and float.";

		if(src->numBands()!=1)
			throw ": klFillHolesFunctor<TYPE> requires single band image data.";
		if(typeid(TYPE)==typeid(unsigned char) )
		{
			Ipp8u *source=src->buffer();
			int sourceStride=src->yStride();
			IppiSize roiSize;

			int width=src->width();
			roiSize.width=width;

			int height = src->height();
			roiSize.height=height;

			// copy the image to a buffer with a black border:
			IppiSize tempBufferSize = { width + 2, height + 2 };


			klRasterBufferPointer fillBuffer=new  klPackedHeapRasterBuffer<unsigned char>(width+2,height+2,1);
			unsigned char* tempBuffer=fillBuffer->buffer();
			int tempBufferLineStride=fillBuffer->yStride();

			IppStatus status = ippiSet_8u_C1R(
				backgroundcolor,
				tempBuffer, tempBufferLineStride,
				tempBufferSize
				);

			if (status != ippStsNoErr) {
				throw "klFillHolesFunctor : ippiSet failed.";
			}

			status = ippiCopy_8u_C1R(
				source, sourceStride,
				tempBuffer + tempBufferLineStride + 1, tempBufferLineStride,
				roiSize
				);

			if (status != ippStsNoErr) {
				throw "klFillHolesFunctor: ippiCopy failed.";
			}

			int workAreaSize;
			status = ippiFloodFillGetSize(roiSize, &workAreaSize);
			if (status != ippStsNoErr) {
				throw "klFillHolesFunctor : ippiFloodFillGetSize failed.";
			}
			unsigned char* ippworkbuffer = ippsMalloc_8u(workAreaSize);

			if(!ippworkbuffer)
			{
				ippiFree(ippworkbuffer);   
				throw "klFillHolesFunctor : ipp memory error";
			}
			IppiConnectedComp regionInfo;
			IppiPoint seed = { 0, 0 };
			status = ippiFloodFill_4Con_8u_C1IR(tempBuffer, tempBufferLineStride,	tempBufferSize,	seed,
				foregroundcolor,  &regionInfo,ippworkbuffer);

			if (status != ippStsNoErr) 
			{
				throw "klFillHolesFunctor : ippiFloodFill failed.";
			}
			status = ippiNot_8u_C1IR(tempBuffer + tempBufferLineStride + 1, tempBufferLineStride,	roiSize	);

			if (status != ippStsNoErr) 
			{
				throw "klFillHolesFunctor : ippiNot failed.";
			}

			status = ippiOr_8u_C1IR( source, sourceStride,  tempBuffer + tempBufferLineStride + 1, 
				tempBufferLineStride, roiSize	);

			//savePPM_SingleBand ("HOLE_FILL_DIAGNOSTIC_fillBuffer.pgm",fillBuffer->width(),fillBuffer->height(),fillBuffer->buffer());
			//savePPM_SingleBand ("HOLE_FILL_DIAGNOSTIC_src.pgm",src->width(),src->height(),src->buffer());
			unsigned char* outbuf=src->buffer();
			size_t outbands=src->numBands();
			size_t outwidth =src->width();
			size_t outheight=src->height(); 
			size_t outbandStride=src->bandStride();
			size_t outxStride =src->xStride();
			size_t outyStride= src->yStride();
			unsigned char* inbuf = tempBuffer + tempBufferLineStride + 1;
			size_t inyStride=tempBufferLineStride;
			unsigned i,j;
			for(i=0;i<outheight;i++)
			{
				for(j=0;j<outwidth;j++)
				{
					unsigned char x,y;
					x=*(outbuf+i*outyStride+j);
					y=*(inbuf+i*inyStride+j);
					unsigned char z= x|y;
					*(outbuf+i*outyStride+j)=z;
				}
			}

			if (status != ippStsNoErr) {
				throw "ippiOr failed.";
			}
			//savePPM_SingleBand ("HOLE_FILL_DIAGNOSTIC_srcPostCopy.pgm",src->width(),src->height(),src->buffer());

		}//end unsigned char impl

		//float impl
	}
	TYPE backgroundcolor;
	TYPE foregroundcolor;
	klRasterBufferPointer src;
};


template< class TYPE > class klErodeFunctor
{
public:
	//Supports float and unsigned char data types, 1 and three band images.
	//In place operation is achieved by passing the source buffer into the destination parameter.
	klErodeFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst,  unsigned char* lkernel, unsigned lkernelWidth, unsigned lkernelHeight)
		: src(lsrc), dst(ldst), kernel(lkernel), kernelWidth(lkernelWidth), kernelHeight(lkernelHeight)
	{

	}

	void operator()(void){

		if(src->numBands() !=1 && src->numBands()!=3)
			throw ": klErodeFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned x0=2*kernelWidth;
		unsigned y0=2*kernelHeight;		

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		klRasterBufferPointer paddst=new klPackedHeapRasterBuffer<TYPE>(src->width()+2*x0,src->height()+2*y0,src->numBands()); 

		klPadFunctor::padRect padRect(x0, y0, src->width(),src->height());

		klPadFunctor padFn(src, paddst, klPadFunctor::padType::PAD_TYPE_MIRROR, padRect); 

		unsigned padOffset=(paddst->yStride() * padRect.y0+ padRect.x0 *dst->xStride()  );

		padFn();

		unsigned char* srcbuf=src->buffer();

		unsigned char* inbuf=paddst->buffer();
		size_t inbands=paddst->numBands();
		size_t inwidth =paddst->width();
		size_t inheight=paddst->height(); 
		size_t inbandStride=paddst->bandStride();
		size_t inxStride =paddst->xStride();
		size_t inyStride= paddst->yStride();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=src->width();

		dstRoiSize.height=src->height();

		IppiSize kernelSize;

		kernelSize.width=kernelWidth;

		kernelSize.height=kernelHeight;

		unsigned anchorX=((int)floor((double)kernelWidth/2)%2==0) ? floor((double)kernelWidth/2)+1 : floor((double)kernelWidth/2) ;

		unsigned anchorY=((int)floor((double)kernelHeight/2)%2==0) ? floor((double)kernelHeight/2)+1 : floor((double)kernelHeight/2) ; 

		IppiPoint anchor;

		anchor.x=anchorX;

		anchor.y=anchorY;

		const type_info& typeof=typeid(TYPE);
		const unsigned char* pkernel=kernel;
		if(typeid(TYPE)== typeid(float))
		{
			switch(inbands)
			{
			case(1):
				{
					if(srcbuf==outbuf)
						ippStatusResult= ippiErode_32f_C1IR((float*)inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult= ippiErode_32f_C1R((float*)inbuf+padOffset, inyStride,
						(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			case(3):
				{
					if(srcbuf==outbuf)
						ippStatusResult=ippiErode_32f_C3IR((float*)inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult=ippiErode_32f_C3R((float*)inbuf+padOffset, inyStride,
						(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			}//end switch inbands
		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{

			switch(inbands)
			{
			case(1):
				{
					ippStatusResult=ippiErode_8u_C1R(inbuf+padOffset, inyStride,
						outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			case(3):
				{
					if(srcbuf==outbuf)
						ippStatusResult=ippiErode_8u_C3IR(inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult=ippiErode_8u_C3R(inbuf+padOffset, inyStride,
						outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			}//end switch inbands

			if(ippStatusResult != ippStsNoErr)
				throw "klErodeFunctor : ipp status error in Erode routine.";

		}//end unsigned char case
		else
			throw "klErodeFunctor : unsupported data type, float and unsigned int supported at this time.";

	}//end operator() 

	klRasterBufferPointer src;
	klRasterBufferPointer dst;
	unsigned char* kernel;
	const unsigned  kernelWidth;
	const unsigned  kernelHeight;

};

template< class TYPE > class klDilateFunctor{
public:
	//Supports float and unsigned char data types, 1 and three band images.
	//In place operation is achieved by passing the source buffer into the destination parameter.
	klDilateFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst,  unsigned char* lkernel, unsigned lkernelWidth, unsigned lkernelHeight)
		: src(lsrc), dst(ldst), kernel(lkernel), kernelWidth(lkernelWidth), kernelHeight(lkernelHeight)
	{

	}

	void operator()(void){

		if(src->numBands() !=1 && src->numBands()!=3)
			throw "klDilateFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned x0=2*kernelWidth;
		unsigned y0=2*kernelHeight;

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		klRasterBufferPointer paddst=new klPackedHeapRasterBuffer<TYPE>(src->width()+2*x0,src->height()+2*y0,src->numBands());  

		klPadFunctor::padRect padRect(x0, y0, src->width(),src->height());

		klPadFunctor padFn(src, paddst, klPadFunctor::padType::PAD_TYPE_MIRROR, padRect);

		unsigned padOffset=(paddst->yStride() * padRect.y0+ padRect.x0 *dst->xStride()  );

		padFn();

		unsigned char* srcbuf=src->buffer();

		unsigned char* inbuf=paddst->buffer();
		size_t inbands=paddst->numBands();
		size_t inwidth =paddst->width();
		size_t inheight=paddst->height(); 
		size_t inbandStride=paddst->bandStride();
		size_t inxStride =paddst->xStride();
		size_t inyStride= paddst->yStride();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=src->width();

		dstRoiSize.height=src->height();

		IppiSize kernelSize;

		kernelSize.width=kernelWidth;

		kernelSize.height=kernelHeight;


		unsigned anchorX=((int)floor((double)kernelWidth/2)%2==0) ? floor((double)kernelWidth/2)+1 : floor((double)kernelWidth/2) ; 

		unsigned anchorY=((int)floor((double)kernelHeight/2)%2==0) ? floor((double)kernelHeight/2)+1 : floor((double)kernelHeight/2) ; 

		IppiPoint anchor;

		anchor.x=anchorX;

		anchor.y=anchorY;

		const type_info& typeof=typeid(TYPE);

		const unsigned char* pkernel=kernel;
		if(typeid(TYPE)== typeid(float))
		{
			switch(inbands)
			{
			case(1):
				{
					if(srcbuf==outbuf)
						ippStatusResult= ippiDilate_32f_C1IR((float*)inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult= ippiDilate_32f_C1R((float*)inbuf+padOffset, inyStride,
						(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			case(3):
				{
					if(srcbuf==outbuf)
						ippStatusResult=ippiDilate_32f_C3IR((float*)inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult=ippiDilate_32f_C3R((float*)inbuf+padOffset, inyStride,
						(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			}//end switch inbands

		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{

			switch(inbands)
			{
			case(1):
				{
					ippStatusResult=ippiDilate_8u_C1R(inbuf+padOffset, inyStride,
						outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;
				}
			case(3):
				{
					if(srcbuf==outbuf)
						ippStatusResult=ippiDilate_8u_C3IR(inbuf+padOffset, inyStride,
						dstRoiSize, pkernel, kernelSize, anchor);
					else
						ippStatusResult=ippiDilate_8u_C3R(inbuf+padOffset, inyStride,
						outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;
				}
			}//end switch inbands

			if(ippStatusResult != ippStsNoErr)
				throw ": klDilateFunctor : ipp status error in dilate routine.";

		}//end unsigned char case
		else
			throw " : klDilateFunctor : unsupported data type, float and unsigned int supported at this time.";

	}//end operator() 

	klRasterBufferPointer src;
	klRasterBufferPointer dst;
	unsigned char* kernel;
	const unsigned  kernelWidth;
	const unsigned  kernelHeight;

};

template< class TYPE > class klOpenFunctor
{
public:
	//Supports float and unsigned char data types, 1 and three band images.
	//In place operation is achieved by passing the source buffer into the destination parameter.
	klOpenFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst,  unsigned char* lkernel, unsigned lkernelWidth, unsigned lkernelHeight)
		: src(lsrc), dst(ldst), kernel(lkernel), kernelWidth(lkernelWidth), kernelHeight(lkernelHeight)
	{

	}

	void operator()(void)
	{

		if(src->numBands() !=1 && src->numBands()!=3)
			throw ": klErodeFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned x0=2*kernelWidth;
		unsigned y0=2*kernelHeight;		

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		klRasterBufferPointer paddst=new klPackedHeapRasterBuffer<TYPE>(src->width()+2*x0,src->height()+2*y0,src->numBands()); 

		klPadFunctor::padRect padRect(x0, y0, src->width(),src->height());

		klPadFunctor padFn(src, paddst, klPadFunctor::padType::PAD_TYPE_MIRROR, padRect); 

		unsigned padOffset=(paddst->yStride() * padRect.y0+ padRect.x0 *dst->xStride()  );

		padFn();

		unsigned char* srcbuf=src->buffer();

		unsigned char* inbuf=paddst->buffer();
		size_t inbands=paddst->numBands();
		size_t inwidth =paddst->width();
		size_t inheight=paddst->height(); 
		size_t inbandStride=paddst->bandStride();
		size_t inxStride =paddst->xStride();
		size_t inyStride= paddst->yStride();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=src->width();

		dstRoiSize.height=src->height();

		IppiSize kernelSize;

		kernelSize.width=kernelWidth;

		kernelSize.height=kernelHeight;

		unsigned anchorX=((int)floor((double)kernelWidth/2)%2==0) ? floor((double)kernelWidth/2)+1 : floor((double)kernelWidth/2) ;

		unsigned anchorY=((int)floor((double)kernelHeight/2)%2==0) ? floor((double)kernelHeight/2)+1 : floor((double)kernelHeight/2) ; 

		IppiPoint anchor;

		anchor.x=anchorX;

		anchor.y=anchorY;

		const type_info& typeof=typeid(TYPE);
		const unsigned char* pkernel=kernel;



		if(typeid(TYPE)== typeid(float))
		{
			switch(inbands)
			{
			case(1):
				{
				
				/*		ippStatusResult= ippiMorphOpenBorder_32f_C1R((float*)inbuf+padOffset, inyStride,
						(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);*/
					break;

				}
			case(3):
				{

	
						//ippStatusResult=ippiMorphOpenBorder_32f_C3R((float*)inbuf+padOffset, inyStride,
						//(float*)outbuf, outyStride, dstRoiSize, pkernel, kernelSize, anchor);
					break;

				}
			}//end switch inbands
		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{

			switch(inbands)
			{
			case(1):
				{	
					IppiMorphAdvState* pState;	
					int pSize;
					IppStatus status =ippiMorphAdvGetSize_8u_C1R( dstRoiSize, pkernel, kernelSize, &pSize);
		
					//(int roiWidth, const Ipp8u* pMask, IppiSize maskSize,  IppiPoint anchor, IppiMorphState* pState))

					 status =ippiMorphAdvInitAlloc_8u_C1R( &pState, dstRoiSize, pkernel,  kernelSize, anchor);

					 ippStatusResult= ippiMorphOpenBorder_8u_C1R(inbuf+padOffset, inyStride,outbuf, outyStride, dstRoiSize, ippBorderRepl,pState);

					break;

				}
			case(3):
				
				{	IppiMorphAdvState* pState;	
					int pSize;
					IppStatus status =ippiMorphAdvGetSize_8u_C3R( dstRoiSize, pkernel, kernelSize, &pSize);
		
					 status =ippiMorphAdvInitAlloc_8u_C3R( &pState, dstRoiSize, pkernel,  kernelSize, anchor);


					ippStatusResult= ippiMorphOpenBorder_8u_C3R(inbuf+padOffset, inyStride,outbuf, outyStride, dstRoiSize, ippBorderRepl,pState);


					break;

				}
			}//end switch inbands

			if(ippStatusResult != ippStsNoErr)
				throw "klErodeFunctor : ipp status error in Erode routine.";

		}//end unsigned char case
		else
			throw "klErodeFunctor : unsupported data type, float and unsigned int supported at this time.";

	}//end operator() 

	klRasterBufferPointer src;
	klRasterBufferPointer dst;
	unsigned char* kernel;
	const unsigned  kernelWidth;
	const unsigned  kernelHeight;

};

template< class TYPE > class klResizeFunctor
{
public:
	//Supports float and unsigned char data types, 1 and three band images.
	//This functor is uniqe in that the raster buffer result is returned from the operator();
	klResizeFunctor(klRasterBufferPointer lsrc, double xFactor,double yFactor,int interpolate)
		: m_src(lsrc), m_xFactor(xFactor), m_yFactor(yFactor),m_interpolate(interpolate)
	{

	}
	// bbcrevisit - Interpolation enum - 
	enum{
		IPPI_INTER_NN ,IPPI_INTER_LINEAR,
		IPPI_INTER_CUBIC ,
		IPPI_INTER_CUBIC2P_BSPLINE, 
		IPPI_INTER_CUBIC2P_CATMULLROM, 	/* two-parameter cubic filter (B=0, C=1/2) */
		IPPI_INTER_CUBIC2P_B05C03,      /* two-parameter cubic filter (B=1/2, C=3/10) */
		IPPI_INTER_SUPER ,
		IPPI_INTER_LANCZOS,
		IPPI_SUBPIXEL_EDGE,
		IPPI_SMOOTH_EDGE		
	}ResizeInterpolateFns;

	klRasterBufferPointer operator()(void)
	{

		if(m_src->numBands() !=1 && m_src->numBands()!=3)
			throw ": klResizeFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned char* srcbuf=m_src->buffer();

		unsigned char* inbuf=m_src->buffer();
		size_t inbands=m_src->numBands();
		size_t inwidth =m_src->width();
		size_t inheight=m_src->height(); 
		size_t inbandStride=m_src->bandStride();
		size_t inxStride =m_src->xStride();
		size_t inyStride= m_src->yStride();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;

		dstRoiSize.width=m_src->width()*m_xFactor;
		dstRoiSize.height=m_src->height()*m_yFactor;

		klRasterBufferPointer dst=new klPackedHeapRasterBuffer<TYPE>(dstRoiSize.width,dstRoiSize.height, inbands);

		IppiSize srcSize;
		srcSize.width=inwidth;
		srcSize.height=inheight;
		int srcStep=inyStride;
		IppiRect srcRoi;

		srcRoi.x=0; srcRoi.y=0; srcRoi.width=inwidth; srcRoi.height=inheight;

		unsigned char* pDst=dst->buffer();

		int dstStep=dst->yStride();

		IppiRect dstRoi;

		dstRoi.x=0; dstRoi.y=0; dstRoi.width=dst->width(); dstRoi.height=dst->height();
		double xShift=0; 
		double yShift=0;

		int BufferSize=0;

		IppStatus computeBufStatus=ippStsNoErr;

		computeBufStatus	=ippiResizeGetBufSize( srcRoi,  dstRoi, m_src->numBands(), m_interpolate, &BufferSize);

		Ipp8u* pBuffer= ippsMalloc_8u( BufferSize );

		const type_info& typeof=typeid(TYPE);

		if(typeid(TYPE)== typeid(float))
		{
			switch(inbands)
			{
			case(1):
				{
					ippStatusResult=  ippiResizeSqrPixel_32f_C1R((float*)inbuf, srcSize,  srcStep,  srcRoi,(float*) pDst,  dstStep,
						dstRoi,  m_xFactor,  m_yFactor,  xShift,  yShift, m_interpolate, pBuffer);
					break;
				}
			case(3):
				{
					ippStatusResult=  ippiResizeSqrPixel_32f_C3R((float*)inbuf, srcSize,  srcStep,  srcRoi,(float*) pDst,  dstStep,
						dstRoi,  m_xFactor,  m_yFactor,  xShift,  yShift, m_interpolate, pBuffer);
					break;

				}
			}//end switch inbands


		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{

			switch(inbands)
			{
			case(1):
				{
					ippStatusResult=  ippiResizeSqrPixel_8u_C1R((unsigned char*)inbuf, srcSize,  srcStep,  srcRoi,(unsigned char*) pDst,  dstStep,
						dstRoi,  m_xFactor,  m_yFactor,  xShift,  yShift, m_interpolate, pBuffer);

					break;

				}
			case(3):
				{
					ippStatusResult=  ippiResizeSqrPixel_8u_C3R((unsigned char*)inbuf, srcSize,  srcStep,  srcRoi, (unsigned char*)pDst,  dstStep,
						dstRoi,  m_xFactor,  m_yFactor,  xShift,  yShift, m_interpolate, pBuffer);
					break;

				}

			}//end switch inbands
			if(pBuffer)
		 {
			 ippsFree( pBuffer );
		 }
			if(ippStatusResult != ippStsNoErr)
			{
				throw " klResizeFunctor : ipp status error in resize routine.";
			}
		}//end unsigned char case
		else
			throw "  klResizeFunctor : unsupported data type, float and unsigned int supported at this time.";
		return dst;

	}//end operator() 

	klRasterBufferPointer m_src;

	double m_xFactor;
	double m_yFactor;
	int m_interpolate;

};

template< class TYPE > class klRotateFunctor
{
public:
	//Supports float and unsigned char data types, 1 and three band images.
	//This functor is uniqe in that the raster buffer result is returned from the operator();
	klRotateFunctor(klRasterBufferPointer lsrc, double angle,int interpolate)
	{
		m_src= lsrc;
		m_interpolate=interpolate;
		m_angle= angle;
	}

	enum 
	{
		IPPI_INTER_NN,// Nearest neighbor interpolation.
		IPPI_INTER_LINEAR,// Linear interpolation.
		IPPI_INTER_CUBIC,// Cubic interpolation.
		IPPI_INTER_LANCZOS,// Interpolation using 3-lobed Lanczos window function.
		IPPI_INTER_SUPER,// Supersampling interpolation.
	}RotateInterpolationFns;

	klRasterBufferPointer operator()(void)
	{

		if(m_src->numBands() !=1 && m_src->numBands()!=3)
			throw ": klRotateFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned char* srcbuf=m_src->buffer();

		unsigned char* inbuf=m_src->buffer();
		size_t inbands=m_src->numBands();
		size_t inwidth =m_src->width();
		size_t inheight=m_src->height(); 
		size_t inbandStride=m_src->bandStride();
		size_t inxStride =m_src->xStride();
		size_t inyStride= m_src->yStride();

		IppStatus  ippStatusResult;

		IppiSize dstRoiSize;


		double angle=m_angle;

		IppiSize srcSize;
		srcSize.width=inwidth;
		srcSize.height=inheight;
		int srcStep=inyStride;
		IppiRect srcRoi;

		double xShift=0;
		double yShift=0;
		srcRoi.x=0; srcRoi.y=0; srcRoi.width=inwidth; srcRoi.height=inheight;
		double quad[4][2];
		double bound[2][2];
		double xCenterSrc = inwidth/2;
		double yCenterSrc = inheight/2;
		double w = inwidth;	
		double h= inheight;

		double _pi=3.14159265358979323846264338327950288f;
		double theta=_pi * angle/180;

		//bbcrevisit
		//This is: inelegant, unrefined, without grace, lacking in good taste,
		//amateur, crude, green, incompetent, ineffectual, inefficient, inexperienced, unpolished, unprofessional,
		//unskilled, unskillful, careless, sloppy, tacky, tactless, undiplomatic,
		//ill-advised, ineffective, ineffectual, misdirected, misguided,
		//discomfiting, disconcerting, disturbing, embarrassing, flustering, uncomfortable,
		//confusing, difficult, disagreeable, impossible, inconvenient, intolerable, troublesome, unpleasant, 
		//unwieldy, debasing, degrading, demeaning, humbling, humiliating, mortifying,
		//clumsy, gauche, roughhewn, stiff, stilted, uncomfortable, uneasy, ungraceful, wooden.

		//It is also unused
		double boo_boo_bummer[3][2];
		boo_boo_bummer[0][0]=-h*sin(theta);                            boo_boo_bummer[0][1]=h*cos(theta);
		boo_boo_bummer[1][0]=w*cos(theta)-h*sin(theta);                boo_boo_bummer[1][1]=h*cos(theta)+w*sin(theta);
		boo_boo_bummer[2][0]=w*cos(theta);                             boo_boo_bummer[2][1]=w*sin(theta);
		int maxDim=0;
		unsigned int kl=0;
		for(kl=0;kl<3;kl++)
		{
			if(boo_boo_bummer[kl][0]>maxDim)
				maxDim=boo_boo_bummer[kl][0];

			if(boo_boo_bummer[kl][1]>maxDim)
				maxDim=boo_boo_bummer[kl][1];
		}

		IppiSize dstSize;
		dstSize.width=max(inwidth,inheight);
		dstSize.height=max(inwidth,inheight);
		double nw=dstSize.width;
		double nh=dstSize.height;
		int xOn=(nw-inwidth)/2;
		int yOn=(nh-inheight)/2;
		double xS=0;
		double yS=0;
		ippiGetRotateShift ( inwidth/2, inheight/2, angle, &xS, &yS );
		if(xOn>0)
			xS+= xOn;
		if(yOn>0)
			yS+=yOn;

		xShift=xS;
		yShift=yS;

		klRasterBufferPointer dst=new klPackedHeapRasterBuffer<TYPE>(dstSize.width,dstSize.height, inbands);		
		unsigned char* pDst=dst->buffer();
		int dstStep=dst->yStride();
		IppiRect dstRoi;
		dstRoi.x=0; dstRoi.y=0; dstRoi.width=dst->width(); dstRoi.height=dst->height();
		const type_info& typeof=typeid(TYPE);

		if(typeid(TYPE)== typeid(float))
		{
			switch(inbands)
			{
			case(1):
				{
					ippStatusResult = ippiSet_32f_C1R(1.0f, (float*)pDst, dstStep,  dstSize);
					ippStatusResult=  ippiRotate_32f_C1R((float*) inbuf, srcSize, srcStep,  srcRoi, (float*) pDst, 
						dstStep,  dstRoi, angle,  xShift,  yShift,  m_interpolate);
					break;
				}
			case(3):
				{
					float  bkgndcol[3];bkgndcol[0]=1.0f;bkgndcol[1]=1.0f;bkgndcol[2]=1.0f;
					ippStatusResult = ippiSet_32f_C3R(bkgndcol, (float*)pDst, dstStep,  dstSize);
					ippStatusResult=  ippiRotate_32f_C3R((float*) inbuf, srcSize, srcStep,  srcRoi, (float*) pDst, 
						dstStep,  dstRoi, angle,  xShift,  yShift,  m_interpolate);
					break;

				}
			}//end switch inbands
		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{
			switch(inbands)
			{
			case(1):
				{
					//unsigned char  bkgndcol[3];bkgndcol[0]=255;bkgndcol[1]=255;bkgndcol[2]=255;
					ippStatusResult = ippiSet_8u_C1R(255, pDst, dstStep,  dstSize);

					ippStatusResult=  ippiRotate_8u_C1R((unsigned char*) inbuf, srcSize, srcStep,  srcRoi, (unsigned char*) pDst, 
						dstStep,  dstRoi, angle,  xShift,  yShift,  m_interpolate);

					break;
				}
			case(3):
				{
					unsigned char  bkgndcol[3];bkgndcol[0]=255;bkgndcol[1]=255;bkgndcol[2]=255;
					ippStatusResult = ippiSet_8u_C3R(bkgndcol, pDst, dstStep,  dstSize);
					ippStatusResult=  ippiRotate_8u_C3R((unsigned char*) inbuf, srcSize, srcStep,  srcRoi, (unsigned char*) pDst, 
						dstStep,  dstRoi, angle,  xShift,  yShift,  m_interpolate);

					break;
				}

			}//end switch inbands
			if(ippStatusResult != ippStsNoErr)
			{
				throw ": klRotateFunctor : ipp status error in rotate routine.";
			}
		}//end unsigned char case
		else
			throw " : klRotateFunctor : unsupported data type, float and unsigned int supported at this time.";
		return dst;

	}//end operator() 

	klRasterBufferPointer m_src;
	double m_angle;
	int m_interpolate;
};


template <class TYPE> class klCopyROIFunctor{
public:

	klCopyROIFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst, klRect iRoi,klRect oRoi)
		: src(lsrc), dst(ldst), m_iRoi(iRoi),m_oRoi(oRoi)
	{

	}
	void operator()(void)
	{
		if(m_iRoi.w!=m_oRoi.w || m_iRoi.h!=m_oRoi.h)
			throw "Input ROI extent not same as output ROI extent in klCopyFunctor.";

		if(m_iRoi.x <0 || m_iRoi.y<0)
			throw "Input ROI outside input image in klCopyROIFunctor";

		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		if(inbands!=outbands)
			throw "klCopyROIFunctor: invalid bands specified.";

		unsigned int i;
		unsigned int j;
		unsigned int b;

		TYPE* inbuf=(TYPE*)src->buffer();

		TYPE* outbuf=(TYPE*)dst->buffer();

		unsigned typeSize=sizeof(TYPE);

		int srcStep=inyStride;
		int dstStep=outyStride;

		TYPE* pDst=inbuf+ m_iRoi.y  * inyStride + m_iRoi.x * inxStride;
		TYPE* pSrc=outbuf+ m_oRoi.y * outyStride + m_oRoi.x * outxStride;

		IppiSize roiSize;
		roiSize.width=m_iRoi.w;
		roiSize.height=m_iRoi.h;

		IppStatus  ippStatusResult;

		if(typeid(TYPE)== typeid(float))
		{//IPP- convention bbcrevisit souce and dst are reversed in name - this really has to be sorted out so variable names are consistent!
			switch(inbands)
			{
			case(1):
				{
					ippStatusResult= ippiCopy_32f_C1R((float*)pSrc, dstStep,(float*)pDst ,srcStep , roiSize);
					break;
				}
			case(3):
				{
					ippStatusResult= ippiCopy_32f_C3R((float*)pSrc, dstStep,(float*)pDst ,srcStep , roiSize);
					break;
				}
			}//end switch inbands
		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{
			switch(inbands)
			{
			case(1):
				{
					ippStatusResult= ippiCopy_8u_C1R((unsigned char*)pSrc, dstStep,(unsigned char*)pDst ,srcStep , roiSize);
					break;
				}
			case(3):
				{
					ippStatusResult= ippiCopy_8u_C3R((unsigned char*)pSrc, dstStep,(unsigned char*)pDst ,srcStep , roiSize);
					break;
				}

			}//end switch inbands
		}
		if(ippStatusResult != ippStsNoErr)
		{
			throw "klCopyROIFunctor : ipp status error in copy routine.";
		}
	}

	klRasterBufferPointer src;

	klRasterBufferPointer dst;

	klRect m_iRoi;

	klRect m_oRoi;

};

//Y´ = 0.299 * R´ + 0.587 * G´ + 0.114 * B´
template< class TYPE > class klColorToGrayFunctor
{
public:
	klColorToGrayFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst)
		: src(lsrc), dst(ldst)
	{

	}

	void operator()(void)
	{
		if(src->numBands() !=3 && dst->numBands()!=1)
			throw "klColorToGrayFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		unsigned char* srcbuf=src->buffer();
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		IppStatus  ippStatusResult;
		IppiSize dstRoiSize;		
		dstRoiSize.width=dst->width();
		dstRoiSize.height=dst->height();

		const type_info& typeof=typeid(TYPE);
		if(typeid(TYPE)== typeid(float))
		{			ippStatusResult= ippiRGBToGray_32f_C3C1R((float*)srcbuf, inyStride,
		(float*)outbuf, outyStride, dstRoiSize);

		if(ippStatusResult != ippStsNoErr)
			throw "klColorToGrayFunctor : ipp status error in Erode routine.";
		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{	
			ippStatusResult=ippiRGBToGray_8u_C3C1R(srcbuf, inyStride,
				outbuf, outyStride, dstRoiSize);
			if(ippStatusResult != ippStsNoErr)
				throw "klColorToGrayFunctor : ipp status error in Erode routine.";

		}//end unsigned char case
		else
			throw "klColorToGrayFunctor : unsupported data type, float and unsigned int supported at this time.";

	}//end operator() 

	klRasterBufferPointer src;
	klRasterBufferPointer dst;
};

template <class TYPE> class klCopySplitBandFunctor{
public:

	//Split 3 channel pixel interleaved to band interleaved.
	klCopySplitBandFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst)
		: src(lsrc), dst(ldst)
	{

	}
	void operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		if(inwidth!=outwidth || inheight != outheight ||inbands!=outbands ||inbands !=3 )
			throw "klCopySplitBandFunctor : in and out  dimensions must be the same";

		unsigned int i;
		unsigned int j;
		unsigned int b;

		TYPE* inbuf=(TYPE*)src->buffer();

		TYPE* outbuf=(TYPE*)dst->buffer();

		unsigned typeSize=sizeof(TYPE);

		int srcStep=inyStride;
		int dstStep=outyStride;

		IppiSize roiSize;
		roiSize.width=inwidth;
		roiSize.height=inheight;

		IppStatus  ippStatusResult;

		if(typeid(TYPE)== typeid(float))
		{
			Ipp32f* pDstPlanePtr[3];
			pDstPlanePtr[0]=(float*)dst->buffer();
			pDstPlanePtr[1]=(float*)(dst->buffer()+ outheight * outyStride);
			pDstPlanePtr[2]=(float*)(dst->buffer()+ (2 * outheight* outyStride));

			ippStatusResult=  ippiCopy_32f_C3P3R((float*) src->buffer(),  inyStride,pDstPlanePtr,  srcStep,  roiSize);

		}//end float case
		else if(typeid(TYPE)==typeid(unsigned char))
		{
			Ipp8u* pDstPlanePtr[3];
			pDstPlanePtr[0]=dst->buffer();
			pDstPlanePtr[1]=dst->buffer()+ outheight * outyStride/3;
			pDstPlanePtr[2]=dst->buffer()+ (2 * outheight* outyStride/3);
			int psb;
			Ipp8u* shift[3];
			shift[0]=ippiMalloc_8u_C1(outwidth,outheight,&psb); 
			shift[1]=ippiMalloc_8u_C1(outwidth,outheight,&psb);
			shift[2]=ippiMalloc_8u_C1(outwidth,outheight,&psb);

			ippStatusResult=  ippiCopy_8u_C3P3R((unsigned char*) src->buffer(),  inyStride,pDstPlanePtr,  outyStride/3,  roiSize);//ippiCopy_8u_C3P3R((unsigned char*) src->buffer(),  inyStride,shift,  psb,  roiSize);

		}
		if(ippStatusResult != ippStsNoErr)
		{
			throw "klCopyROIFunctor : ipp status error in copy routine.";
		}
	}

	klRasterBufferPointer src;

	klRasterBufferPointer dst;

};


template< class TYPE > class klDitheringFunctor
{
public:

	//This parameter is set as a percentage of range [0..100].
	int noise;
		
	IppiDitherType dtype;

	//The number of output levels for halftoning (dithering); can be varied in the range [2..(1<< depth)], where depth is the bit depth of the destination image.
	int levels;
	

	klDitheringFunctor(klRasterBufferPointer lsrc, klRasterBufferPointer ldst,int Levels =2, IppiDitherType dithertype = IppiDitherType::ippDitherFS,int Noise =5)
		: src(lsrc), dst(ldst)
	{
		noise =Noise;
		dtype = dithertype;//IppiDitherType::ippDitherFS;
		levels =Levels;
	}

	void operator()(void)
	{
		if(src->numBands() !=3 && dst->numBands()!=1)
			throw "klDitheringFunctor : bad number of bands, onle one and three band images supported at this time.";

		unsigned char* outbuf=dst->buffer();
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();

		unsigned char* srcbuf=src->buffer();
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();

		IppStatus  ippStatusResult;
		IppiSize dstRoiSize;		
		dstRoiSize.width=dst->width();
		dstRoiSize.height=dst->height();

		const type_info& typeof=typeid(TYPE);
		
		if(typeid(TYPE)==typeid(unsigned char))
		{	
			if(src->numBands() ==3)
			{
				IppStatus ippStatusResult= ippiReduceBits_8u_C3R(srcbuf, inyStride,	outbuf, outyStride, dstRoiSize, noise,  dtype, levels);
			}

			if(src->numBands() ==1)
			{
				IppStatus ippStatusResult= ippiReduceBits_8u_C1R(srcbuf, inyStride,	outbuf, outyStride, dstRoiSize, noise,  dtype, levels);
			}



			if(ippStatusResult != ippStsNoErr)
				throw "klDitheringFunctor : ipp status error in routine.";

		}//end unsigned char case
		else
			throw "klDitheringFunctor : unsupported data type, unsigned int supported at this time.";

	}//end operator() 

	klRasterBufferPointer src;
	klRasterBufferPointer dst;
};





#endif //KL_IMAGE_FUNCTOR_H
