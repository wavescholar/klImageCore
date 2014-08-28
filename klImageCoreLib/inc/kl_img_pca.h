/*******************************
 * Copyright (c) <2007>, <Bruce Campbell> All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *  
 * Bruce B Campbell 11 30 2012  *
 ********************************/

#include "kl_image_processing_functors.h"

#include "kl_ppm_image_io.h"

#include "kl_principal_components.h"

class klPCAFunctor{
public:
	klPCAFunctor(klRasterBufferPointer lsrc)
		: src(lsrc)
	{

	}

	klRasterBufferPointer operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();
		size_t inBytes=inxStride/inbands;
		
		klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);

		klSamplePopulation<double> colors(inwidth*inheight,inbands);
		
		if(inbands!=3)
			throw "klPCAFunctor input bands must be 3";

		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();
		size_t outBytes=outxStride/outbands;

		unsigned int i;
		unsigned int j;
		unsigned int b;

		unsigned char* inbuf=src->buffer();
		unsigned char* outbuf=dst->buffer();

		for (i=0; i<inheight; i++)
		{
			for (j=0; j<inwidth; j++)
			{
				unsigned char R=*(inbuf+ i*inyStride + j*inbands + 0);
				unsigned char G=*(inbuf+ i*inyStride + j*inbands + 1);
				unsigned char B=*(inbuf+ i*inyStride + j*inbands + 2);
				colors[i*inwidth+j][0] =R;
				colors[i*inwidth+j][1] =G;
				colors[i*inwidth+j][2] =B;
			}
		}
		
		klPrincipalComponents<double> pca( colors);
		
		klMatrix<double> V=pca();
		cout<<V;

		klSamplePopulation<double> colorsV(colors);
		colorsV = colors * V.transpose();

		klRasterBufferPointer dst1 = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		klRasterBufferPointer dst2 = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		unsigned char* outbuf1=dst1->buffer();
		unsigned char* outbuf2=dst2->buffer();

		for (i=0; i<outheight; i++)
		{
			for (j=0; j<outwidth; j++)
			{
				unsigned char R=colorsV[i*outwidth+j][0];
				unsigned char G=colorsV[i*outwidth+j][1];
				unsigned char B=colorsV[i*outwidth+j][2];

				*(outbuf+ i*outyStride + j*outbands + 0)=R>255? 0: R;
				*(outbuf+ i*outyStride + j*outbands + 1)=G>255? 0: G;
				*(outbuf+ i*outyStride + j*outbands + 2)=B>255? 0: B;
			}
		}


		return dst;
		
	}
protected:
	klRasterBufferPointer src;
};

class klApplyColorUnmixingBasis{
public:
	klApplyColorUnmixingBasis(klRasterBufferPointer lsrc)
		: src(lsrc)
	{

	}

	klRasterBufferPointer operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();
		size_t inBytes=inxStride/inbands;
		
		klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);

		klSamplePopulation<double> colors(inwidth*inheight,inbands);
		
		if(inbands!=3)
			throw "klPCAFunctor input bands must be 3";

		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();
		size_t outBytes=outxStride/outbands;

		unsigned int i;
		unsigned int j;
		unsigned int b;

		unsigned char* inbuf=src->buffer();
		unsigned char* outbuf=dst->buffer();

		for (i=0; i<inheight; i++)
		{
			for (j=0; j<inwidth; j++)
			{
				unsigned char R=*(inbuf+ i*inyStride + j*inbands + 0);
				unsigned char G=*(inbuf+ i*inyStride + j*inbands + 1);
				unsigned char B=*(inbuf+ i*inyStride + j*inbands + 2);
				colors[i*inwidth+j][0] =-log((double)R/255.0);
				colors[i*inwidth+j][1] =-log((double)G/255.0);
				colors[i*inwidth+j][2] =-log((double)B/255.0);
			}
		}

		unsigned int numComponents=3;
		klMatrix<double> V(3,3);

		//H & E Liver Samples 
		//V[0][0]=-1.7594;	V[0][1]=0.7924;		V[0][2]=0.9959;
		//V[1][0]=2.1170;		V[1][1]=0.0252;		V[1][2]=-0.4828;
		//V[2][0]=0.1877;		V[2][1]=-0.5797;	V[2][2]=0.7929;
				
		
		

		V[0][0]=0.783335755728231;	V[0][1]=0.447266438576609;	V[0][2]=0.404271753240481;
		V[1][0]=0.561629364528937;	V[1][1]=0.561089471668671;	V[1][2]=-0.773927809956508;
		V[2][0]=0.251371846163357;	V[2][1]=0.644347053262721;	V[2][2]=0.456446501866149;
		
		//V[0][0]=0.723738016372563;	V[0][1]=0.300966002171305;	V[0][2]=0.568213930963361;
		//V[1][0]=0.634424147412269;	V[1][1]=0.519620921095068;	V[1][2]=-0.771123662429263;
		//V[2][0]=0.271494539240338;	V[2][1]=0.799633393435601;	V[2][2]=0.287230266338431;

		klSamplePopulation<double> colorsV(colors);
		colorsV = colors * V.transpose();

		//klRasterBufferPointer dst1 = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		//klRasterBufferPointer dst2 = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		//klRasterBufferPointer dst3 = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		//unsigned char* outbuf1=dst1->buffer();
		//unsigned char* outbuf2=dst2->buffer();
		//unsigned char* outbuf3=dst3->buffer();

		for (i=0; i<outheight; i++)
		{
			for (j=0; j<outwidth; j++)
			{
				double R=colorsV[i*outwidth+j][0];
				double G=colorsV[i*outwidth+j][1];
				double B=colorsV[i*outwidth+j][2];

				R=exp(-R)*255;
				G=exp(-G)*255;
				B=exp(-B)*255;

				if( (R+G+B)> 3*214)
				{
					R=0;
					G=0;
					B=0;
				}

				*(outbuf+ i*outyStride + j*outbands + 0)=R>255 ? 0: R;
				*(outbuf+ i*outyStride + j*outbands + 1)=G>255 ? 0: G;
				*(outbuf+ i*outyStride + j*outbands + 2)=B>255 ? 0: B;

				//*(outbuf1+ i*outwidth + j )=R>255? 0: R;
				//*(outbuf2+ i*outwidth + j )=G>255? 0: G;
				//*(outbuf3+ i*outwidth + j )=B>255? 0: B;
			}
		}
	/*	write_ppm_single_band ("SSB_b1.pgm", dst1->width(), dst1->height(),dst1->buffer());
		write_ppm_single_band ("SSB_b2.pgm", dst2->width(), dst2->height(),dst2->buffer());
		write_ppm_single_band ("SSB_b3.pgm", dst3->width(), dst3->height(),dst3->buffer());*/
		return dst;
	}
protected:
	klRasterBufferPointer src;
};






class klHandERatio{
public:
	klHandERatio(klRasterBufferPointer lsrc )
		: src(lsrc)
	{

	}

	klRasterBufferPointer operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();
		size_t inBytes=inxStride/inbands;

		
				
		if(inbands!=3)
			throw "klHandERatio input bands must be 3";

		klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, 1);
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();
		size_t outBytes=outxStride/outbands;
		

		unsigned int i;
		unsigned int j;
		unsigned int b;

		unsigned char* inbuf=src->buffer();
		unsigned char* outbuf=dst->buffer();

		for (i=0; i<inheight; i++)
		{
			for (j=0; j<inwidth; j++)
			{
				double W=*(inbuf+ i*inyStride + j*inbands + 0);
				double H=*(inbuf+ i*inyStride + j*inbands + 1);
				double E=*(inbuf+ i*inyStride + j*inbands + 2);

				double Sp = H/(E+1) * 64;
				*(outbuf+ i*outyStride + j*outbands + 0)=(Sp>255 || Sp<0 ) ? 0: Sp;
			}
		}
		return dst;
	}
protected:
	klRasterBufferPointer src;
	klRasterBufferPointer mask;
	double alpha;
	double Rm;
	double Gm;
	double Bm;
};


class klCompositeMask{
public:
	klCompositeMask(klRasterBufferPointer lsrc, klRasterBufferPointer lmask,double lalpha,double lRm,double lGm,double lBm )
		: src(lsrc), mask(lmask), alpha(lalpha), Rm(lRm), Gm(lGm), Bm(lBm)
	{

	}

	klRasterBufferPointer operator()(void)
	{
		size_t inbands=src->numBands();
		size_t inwidth =src->width();
		size_t inheight=src->height(); 
		size_t inbandStride=src->bandStride();
		size_t inxStride =src->xStride();
		size_t inyStride= src->yStride();
		size_t inBytes=inxStride/inbands;

		size_t maskbands=mask->numBands();
		size_t maskwidth =mask->width();
		size_t maskheight=mask->height(); 
		size_t maskbandStride=mask->bandStride();
		size_t maskxStride =mask->xStride();
		size_t maskyStride= mask->yStride();
		size_t maskBytes=maskxStride/maskbands;

				
		if(inbands!=3)
			throw "klCompositeMask input bands must be 3";

		klRasterBufferPointer dst = new klPackedHeapRasterBuffer<unsigned char>(inwidth,inheight, inbands);
		size_t outbands=dst->numBands();
		size_t outwidth =dst->width();
		size_t outheight=dst->height(); 
		size_t outbandStride=dst->bandStride();
		size_t outxStride =dst->xStride();
		size_t outyStride= dst->yStride();
		size_t outBytes=outxStride/outbands;
		
		if(maskbands!=1)
			throw "klCompositeMask mask bands must be 1";

		unsigned int i;
		unsigned int j;
		unsigned int b;

		unsigned char* inbuf=src->buffer();
		unsigned char* outbuf=dst->buffer();
		unsigned char* maskbuf=mask->buffer();

		for (i=0; i<inheight; i++)
		{
			for (j=0; j<inwidth; j++)
			{
				double R=*(inbuf+ i*inyStride + j*inbands + 0);
				double G=*(inbuf+ i*inyStride + j*inbands + 1);
				double B=*(inbuf+ i*inyStride + j*inbands + 2);

				double Ro=R;
				double Go=G;
				double Bo=B;

				unsigned char M= *(maskbuf+ i*maskyStride + j*maskbands + 0);
				if(M!=0)
				{
					Ro = (1-alpha)*R + alpha * Rm;
					Go = (1-alpha)*G + alpha * Gm;
					Bo = (1-alpha)*B + alpha * Bm;
					*(outbuf+ i*outyStride + j*outbands + 0)=(Ro>255 || Ro<0 ) ? R: Ro;
					*(outbuf+ i*outyStride + j*outbands + 1)=(Go>255 || Go<0 ) ? G: Go;
					*(outbuf+ i*outyStride + j*outbands + 2)=(Bo>255 || Bo<0 ) ? B: Bo;
				}
				else
				{
					*(outbuf+ i*outyStride + j*outbands + 0)= R;
					*(outbuf+ i*outyStride + j*outbands + 1)= G;
					*(outbuf+ i*outyStride + j*outbands + 2)= B;
				}
			}
		}
		return dst;
	}
protected:
	klRasterBufferPointer src;
	klRasterBufferPointer mask;
	double alpha;
	double Rm;
	double Gm;
	double Bm;
};
