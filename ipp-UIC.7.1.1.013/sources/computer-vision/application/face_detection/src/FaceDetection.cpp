/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Face Detection Sample for Windows*
//
//   By downloading and installing this sample, you hereby agree that the
//   accompanying Materials are being provided to you under the terms and
//   conditions of the End User License Agreement for the Intel(R) Integrated
//   Performance Primitives product previously accepted by you. Please refer
//   to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//   product installation for more information.
//
//
*/

// FaceDetection.cpp : Defines the methods for the application.
//
#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning ( disable : 4996 )
#endif
#include "stdafx.h"
#include "ippiImage.h"
#include "IppiImageStore.h"
#include "ipp.h"
#include "eqrect.h"

int minfacew=999999;
int minfaceh=999999;
int maxfacew=0;
int maxfaceh=0;
int minneighbors=2;
float fminface=10.0f;
float fmaxface=100.0f;
float sfactor=1.12f;
float distfactor=15.0f;
float distfactorrect=30.0f;
int threads=-1;
char* cascade_name ="haar.txt";
char* resfile_name ="result.bmp";
char* originalfile ="test.bmp";
char* logfile ="result.txt";
FILE *rfile;
bool silent=false;
int freq=1700;
int quality=4;
int sensitivity=1;
int prun=0;


IppiHaarClassifier_32f **pHaar;
int stages, classifiers, features, positive;
int *nLength, *nClass, *nFeat, *pNum, *nStnum;
IppiRect *pFeature;
Ipp32f *pWeight, *pThreshold, *pVal1, *pVal2, *sThreshold;
IppiSize face, classifierSize;

IppStatus  ReadHaarClassifier (char *haarfname);
IppStatus  AdjustHaarClassifier (int bord, float decstage);
IppStatus  FreeHaarClassifier (void);
static int is_equal(int x1,int y1,int w1,int x2,int y2,int w2,float distparam,float rectparam);
void ClusterFaces(CIppiImage *mask, CCluster *clusters, IppiSize face, Ipp32f factor);
void SetFaces (CCluster *clusters,CIppiImage *dst,float rfactor);


int main(int argc, char* argv[])
{
    IppStatus ret=ippStsOk;
    IppiSize roi, roi0, roi1, roi2, roi3, roi4;
    IppiRect rect, rect0;
    int i,j;
    size_t optlen=0;
    Ipp32f factor=1,bfactor,rfactor=1;
    Ipp32f decthresh=0.0001f;
    int bord=1;
    CFile* imgfile = NULL;
    CFile* savefile = NULL;
    CIppiImage *srcld=new CIppiImage();
    CIppiImage *dst=new CIppiImage();
    CIppiImage *src=new CIppiImage();
    CIppiImage *sqr=new CIppiImage();
    CIppiImage *norm=new CIppiImage();
    CIppiImage *mask=new CIppiImage();
    CIppiImage *srcbg=new CIppiImage();
    CIppiImage *srcs1=new CIppiImage();
    CIppiImage *srcs2=new CIppiImage();

    fprintf( stderr, "Usage:          -cascade=text cascade file\n" );
    fprintf( stderr, "                -input=input bmp file\n" );
    fprintf( stderr, "                -output=output bmp file\n" );
    fprintf( stderr, "                -log=text log file\n" );
    fprintf( stderr, "                -quality=1-5 the more the better\n" );
    fprintf( stderr, "                -sensitivity=1-5 the more the more sensitive\n" );
    fprintf( stderr, "                -minface=minimal face width (in percent of minimal image size)\n" );
    fprintf( stderr, "                -maxface=maximal face width (in percent of minimal image size)\n" );
    fprintf( stderr, "                -threads=number of threads\n" );

    for (i=1; i<argc; i++) {
        optlen = strlen("-cascade=");
        if( !strncmp( argv[i],"-cascade=",optlen)) {
            cascade_name=argv[i]+optlen;
            continue;
        }
        optlen = strlen("-input=");
        if( !strncmp( argv[i],"-input=",optlen)) {
            originalfile=argv[i]+optlen;
            continue;
        }
        optlen = strlen("-output=");
        if( !strncmp( argv[i],"-output=",optlen)) {
            resfile_name=argv[i]+optlen;
            continue;
        }
        optlen = strlen("-log=");
        if( !strncmp( argv[i],"-log=",optlen)) {
            logfile=argv[i]+optlen;
            continue;
        }
        optlen = strlen("-minface=");
        if( !strncmp( argv[i],"-minface=",optlen)) {
            sscanf(argv[i]+ optlen,"%f",&fminface);
            continue;
        }
        optlen = strlen("-maxface=");
        if( !strncmp( argv[i],"-maxface=",optlen)) {
            sscanf(argv[i]+ optlen,"%f",&fmaxface);
            continue;
        }
        optlen = strlen("-quality=");
        if( !strncmp( argv[i],"-quality=",optlen)) {
            sscanf(argv[i]+ optlen,"%d",&quality);
            continue;
        }
        optlen = strlen("-sensitivity=");
        if( !strncmp( argv[i],"-sensitivity=",optlen)) {
            sscanf(argv[i]+ optlen,"%d",&sensitivity);
            continue;
        }
        optlen = strlen("-threads=");
        if( !strncmp( argv[i],"-threads=",optlen)) {
            sscanf(argv[i]+ optlen,"%d",&threads);
            continue;
        }
    }

    quality = (quality<1)?1:(quality>5)?5:quality;
    sensitivity = (sensitivity<1)?1:(sensitivity>5)?5:sensitivity;
    prun = 4;
    IppiSize masksecond={2*quality-1,2*quality-1};
    IppiPoint anchor={quality-1,quality-1};
    sfactor = 1.21f - quality * 0.03f;
    distfactor = 0.f + (5.f-sensitivity) * 10.f;
    distfactorrect = 1.0f;
    for (i=5; i>sensitivity; i--) distfactorrect *= 1.2;
    distfactorrect = (distfactorrect - 0.9999f) * 100.0f;
    minneighbors = (int)(0.f + (5.f-sensitivity) * 1.5f);

    rfile=fopen(logfile,"wt");
    if (!rfile) {
        fprintf( stderr, "ERROR: log file %s can not created  \n",logfile );
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        return -1;
    }
    fprintf(rfile,"x\ty\twidth | height\n");
    fprintf(rfile,"\n");
    roi.width = roi.height = 3;
    try
    {
        imgfile = new CFile(originalfile, CFile::modeRead);
    }
    catch ( CFileException *e )
    {
        fprintf( stderr, "ERROR: photofile is not readable  \n" );
        e->Delete();
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        delete imgfile;
        return -1;
    }
    try
    {
        savefile = new CFile(resfile_name, CFile::modeWrite | CFile::modeCreate);
    }
    catch ( CFileException *e )
    {
        fprintf( stderr, "ERROR: %s is not writable  \n",resfile_name );
        e->Delete();
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        delete imgfile; delete savefile;
        return -1;
    }
    srcld->LoadImage(imgfile);
    roi0.width = srcld->Width();
    roi0.height = srcld->Height();
    if (roi0.width<=0||roi0.height<=0) {
       fprintf( stderr, "ERROR: wrong image size: width=%d  height=%d\n",roi0.width,roi0.height );
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        delete imgfile; delete savefile;
        return -1;
    }
    rect0.x = 0; rect0.y = 0;
    rect0.width = roi0.width;
    rect0.height = roi0.height;
    ret=ReadHaarClassifier(cascade_name);
    if (ret!=ippStsOk) {
        fprintf( stderr, "ERROR: cascade file is incorrect\n" );
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        delete imgfile; delete savefile;
        return -1;
    }
    ret = AdjustHaarClassifier (bord, decthresh);
    if (ret!=ippStsOk) {
        fprintf( stderr, "ERROR: cascade is illegal\n" );
        delete srcld; delete dst; delete src; delete sqr; delete norm;
        delete mask; delete srcbg; delete srcs1; delete srcs2;
        delete imgfile; delete savefile;
        return -1;
    }
    rect.x = bord;
    rect.y = bord;
    dst->CreateImage(roi0.width,roi0.height,3,pp8u);
    if (srcld->Channels()!=3) {
       int ii, jj;
       Ipp8u *srcPtr=(Ipp8u*)srcld->DataPtr();
       Ipp8u *dstPtr=(Ipp8u*)dst->DataPtr();
       for (ii=0; ii<srcld->Height(); ii++) {
          for (jj=0; jj<srcld->Width(); jj++) {
              dstPtr[ii*dst->Step()+jj*3]  =srcPtr[ii*srcld->Step()+jj];
              dstPtr[ii*dst->Step()+jj*3+1]=srcPtr[ii*srcld->Step()+jj];
              dstPtr[ii*dst->Step()+jj*3+2]=srcPtr[ii*srcld->Step()+jj];
          }
       }
    }
    else {
        CIppiImage *oldsrcld;
        CIppiImage *tmpimg=new CIppiImage();
        tmpimg->CreateImage(roi0.width,roi0.height,1,pp8u);
        ret=ippiCopy_8u_C3R((const Ipp8u*)srcld->DataPtr(),srcld->Step(),(Ipp8u*)dst->DataPtr(),dst->Step(),roi0);
        if (ret!= ippStsOk) {
            fprintf( stderr, "ERROR: src image is illegal\n" );
            return -1;
        }
        ret=ippiRGBToGray_8u_C3C1R((const Ipp8u*)srcld->DataPtr(),srcld->Step(),(Ipp8u*)tmpimg->DataPtr(),tmpimg->Step(),roi0);
        if (ret!= ippStsOk) {
            fprintf( stderr, "ERROR: src image is illegal\n" );
            return -1;
        }
        oldsrcld=srcld;
        srcld=tmpimg;
        delete oldsrcld;
    }
    if (face.width<=0||face.height<=0) {
        fprintf( stderr, "ERROR: cascade face size is illegal\n" );
        return 1;
    }
    float wsh=face.width/(float)roi0.width;
    float hsh=face.height/(float)roi0.height;
    if (wsh>=hsh) {
       if (fminface<wsh) fminface=wsh;
       if (fminface>100.0f) fminface=100.0f;
       if (fmaxface<fminface) fmaxface=fminface;
       if (fmaxface>100.0f) fmaxface=100.0f;
       minfacew=(int)(roi0.width*fminface*0.01f);
       if (minfacew<face.width) minfacew=face.width;
       if (minfacew>roi0.width) minfacew=roi0.width;
       maxfacew=(int)(roi0.width*fmaxface*0.01f);
       if (maxfacew<minfacew) maxfacew=minfacew;
       if (maxfacew>roi0.width) maxfacew=roi0.width;
    } else {
       if (fminface<hsh) fminface=hsh;
       if (fminface>100.0f) fminface=100.0f;
       if (fmaxface<fminface) fmaxface=fminface;
       if (fmaxface>100.0f) fmaxface=100.0f;
       minfacew=(int)(roi0.height*fminface*0.01f*face.width/(float)(face.height));
       if (minfacew<face.width) minfacew=face.width;
       if (minfacew>roi0.width) minfacew=roi0.width;
       maxfacew=(int)(roi0.height*fmaxface*0.01f*face.width/(float)(face.height));
       if (maxfacew<minfacew) maxfacew=minfacew;
       if (maxfacew>roi0.width) maxfacew=roi0.width;
    }
    bfactor=((float)minfacew)/((float)face.width);
    if (threads>0) ippSetNumThreads(threads);
    ippGetCpuFreqMhz(&freq);
    clock_t time = clock();
    if (bfactor>1) {
        CIppiImage *newld=new CIppiImage();
        IppiSize newroi;
        newroi.width=(int)(roi0.width/bfactor);
        newroi.height=(int)(roi0.height/bfactor);
        newld->CreateImage(newroi.width,newroi.height,1,pp8u);
        /* scale image */
        Ipp32s specSize = 0, initSize = 0, bufSize = 0;
        ippiResizeGetSize_8u(roi0, newroi, ippLanczos, 0, &specSize, &initSize);
        Ipp8u* pInitBuf = ippsMalloc_8u(initSize);
        IppiResizeSpec_32f* pSpec = (IppiResizeSpec_32f*)ippsMalloc_8u(specSize);
        ippiResizeLanczosInit_8u(roi0, newroi, 3, pSpec, pInitBuf);
        ippsFree(pInitBuf);
        ippiResizeGetBufferSize_8u(pSpec, newroi, 1, &bufSize);
        Ipp8u* pBuffer = ippsMalloc_8u(bufSize);
        IppiPoint dstOffset = {0,0};
        ret = ippiResizeLanczos_8u_C1R((const Ipp8u *)srcld->DataPtr(), srcld->Step(), (Ipp8u *)newld->DataPtr(), newld->Step(), dstOffset, newroi, ippBorderRepl, 0, pSpec, pBuffer);
        ippsFree(pSpec);
        ippsFree(pBuffer);
        /***************/
        delete srcld;
        srcld=newld;
        minfacew=face.width;
        maxfacew = (int)(maxfacew/bfactor);
        rfactor=bfactor;
        bfactor=1;
        roi0=newroi;
        rect0.width = roi0.width;
        rect0.height = roi0.height;
    }
    /* Ipp32f (float) integral function ippiSqrIntegral_8u32f64f_C1R guarantees presize  */
    /* value of integral for image of 2**17 pixels (start value -2**24, maximal precise  */
    /* value 2**24 divided by maximal pixel value 2**8. Really, presize integrals can be */
    /* calculated for bigger images because the average pixel value is less than 256     */
    /* The image is processed by fragments of about pm pixels                            */
    /* But experiments show that faces are detected correctly with Ipp32f integrals for  */
    /* images containing 2**20 - 2**22 pixels. It could be checked by changing pm value  */
    /* Sizes of auxillary images src (integrals), sqr (square integrals), norm (standard */
    /* deviation on classifier windows), mask (mask of considered pixels) could be       */
    /* decreased to pm pixels plus 2 extra rows and columns for integral images.         */
    int pm=1<<17;
    src->CreateImage ((int)(roi0.width)+2,(int)(roi0.height)+2,1,pp32f);
    sqr->CreateImage ((int)(roi0.width)+2,(int)(roi0.height)+2,1,pp64f);
    norm->CreateImage((int)(roi0.width),  (int)(roi0.height),  1,pp32f);
    mask->CreateImage((int)(roi0.width),  (int)(roi0.height),  1,pp8u );
    srcbg->CreateImage((int)(roi0.width),(int)(roi0.height),1,pp8u);
    srcs1->CreateImage((int)(roi0.width),(int)(roi0.height),1,pp8u);
    srcs2->CreateImage((int)(roi0.width),(int)(roi0.height),1,pp8u);

    CCluster *clusters=new CCluster();
    CIppiImage *img1=0, *img2=0, *img3=0;
    roi1 = roi2 = roi0;

    int bufsize=0;
    Ipp8u *buffer=0;
    float threshall=0.001f;
    ret=ippiFilterMaxGetBufferSize_8u_C3R(roi0.width, masksecond, &bufsize);
    if (ret != ippStsOk) {
        delete clusters;
        return -1;
    }
    buffer=ippsMalloc_8u(bufsize);
    if (!buffer) {
        delete clusters;
        return -1;
    }

    /* The main loop of face detection algorithm using the linear Haar cascade          */
    /* The algorithm implements Viola-Jones method with following changes:              */
    /*                                                                                  */
    /* 1. Image scaling is used instead of classifier scaling - it allows SSE using     */
    /*    and is faster for modern CPUs                                                 */
    /* 2. Pixels of the grid (prun+1)x(prun+1) are checked until the number of          */
    /*    positive responses became less than (thumb ruled) threshald threshall         */
    /* 3. Dilation of the mask with (2*prun+1)x(2*prun+1) is done and remaining stage   */
    /*    of the cascade are processed                                                  */
    /*                                                                                  */
    /* For prun=4 it is 2.5-3 times faster than processing of all pixels with the same  */
    /* quality                                                                          */
    for (factor=1.0f; roi2.width>=face.width*factor && roi2.height>=face.height*factor && face.width*factor<=maxfacew; factor*=sfactor) {
        Ipp8u *pImg,*pMask;
        Ipp32f *pSrc,*pNorm;
        if (factor==1.0f) {
           img2 = srcld;
        } else {
           roi1 = roi2;
           roi2.width = (int)(roi1.width/sfactor);
           roi2.height = (int)(roi1.height/sfactor);
           if (img1==0) {
              img1 = img2; img2 = srcs1; img3 = srcs2;
           } else {
              img1 = img2; img2 = img3; img3 = img1;
           }
           img2->SetSize(roi2.width,roi2.height);
           mask->SetSize(roi2.width,roi2.height);

           /* scale image */
           Ipp32s specSize = 0, initSize = 0, bufSize = 0;
           ippiResizeGetSize_8u(roi1, roi2, ippLanczos, 0, &specSize, &initSize);
           Ipp8u* pInitBuf = ippsMalloc_8u(initSize);
           IppiResizeSpec_32f* pSpec = (IppiResizeSpec_32f*)ippsMalloc_8u(specSize);
           ippiResizeLanczosInit_8u(roi1, roi2, 3, pSpec, pInitBuf);
           ippsFree(pInitBuf);
           ippiResizeGetBufferSize_8u(pSpec, roi2, 1, &bufSize);
           Ipp8u* pBuffer = ippsMalloc_8u(bufSize);
           IppiPoint dstOffset = {0,0};
           ret = ippiResizeLanczos_8u_C1R((const Ipp8u *)img1->DataPtr(), img1->Step(), (Ipp8u *)img2->DataPtr(), img2->Step(), dstOffset, roi2, ippBorderRepl, 0, pSpec, pBuffer);
           ippsFree(pSpec);
           ippsFree(pBuffer);
           /***************/

           if (ret != ippStsOk) return -1;
        }
        roi1.width=roi2.width-classifierSize.width+1;
        roi1.height=roi2.height-classifierSize.height+1;
        if (prun==0) {
           ret = ippiSet_8u_C1R (255, (Ipp8u*)mask->DataPtr(), mask->Step(), roi1);
        } else {
           ret = ippiSet_8u_C1R (0, (Ipp8u*)mask->DataPtr(), mask->Step(), roi2);
           pMask=(Ipp8u*)mask->DataPtr();
           for (i=0; i<roi1.height; i+=prun+1) {
              for (j=0; j<roi1.width; j+=prun+1) {
                 pMask[j] = 255;
              }
              pMask+=(prun+1)*mask->Step();
           }
        }
        roi3.width=roi2.width-roi1.width;
        roi3.height=roi1.height;
        ret = ippiSet_8u_C1R (0, (Ipp8u*)mask->DataPtr()+roi1.width, mask->Step(), roi3);
        roi3.width=roi2.width;
        roi3.height=roi2.height-roi1.height;
        ret = ippiSet_8u_C1R (0, (Ipp8u*)mask->DataPtr()+roi1.height*mask->Step(), mask->Step(), roi3);
        if (ret != ippStsOk) {printf("ERROR 3\n"); return -3;}

        int k,shda,shim,allpos;
        int h=(pm+roi2.width-1)/roi2.width;
        int N=(roi2.height-classifierSize.height+1+h-classifierSize.height-1)/(h-classifierSize.height+1);
        int X=(roi2.height-classifierSize.height+1)/N;
        int rem=(roi2.height-classifierSize.height+1)%N;
        Ipp64f *pSqr;
        rect.width  = face.width - bord - bord;
        rect.height = face.height - bord - bord;

        for (allpos=0,shda=0,shim=0,k=0; k<N; k++) {
           roi3.height=X+classifierSize.height-1+(k<rem);
           roi3.width=roi2.width;
           roi4.height=X+(k<rem);
           roi4.width=roi3.width-classifierSize.width+1;
           pImg=(Ipp8u*)img2->DataPtr()+shda*img2->Step();
           pMask=(Ipp8u*)mask->DataPtr()+shda*mask->Step();
           pSrc=(Ipp32f*)src->DataPtr()/*+shim*src->Step()/sizeof(Ipp32f)*/;
           pNorm=(Ipp32f*)norm->DataPtr()/*+shda*norm->Step()/sizeof(Ipp32f)*/;
           pSqr=(Ipp64f*)sqr->DataPtr()/*+shim*sqr->Step()/sizeof(Ipp64f)*/;
           ret=ippiSqrIntegral_8u32f64f_C1R((const Ipp8u*)pImg,img2->Step(),pSrc,src->Step(),pSqr,sqr->Step(),roi3,(Ipp32f)(-(1<<24)),0.0);
           if (ret != ippStsOk) {printf("ERROR 1\n"); return -1;}
           ret=ippiRectStdDev_32f_C1R((const Ipp32f*)pSrc,src->Step(),(const Ipp64f*)pSqr,sqr->Step(),pNorm,norm->Step(),roi4,rect);
           if (ret != ippStsOk) {printf("ERROR 2\n"); return -2;}
           positive = roi4.width*roi4.height;
           for (i=0; i<stages; i++) {
              ret=ippiApplyHaarClassifier_32f_C1R((const Ipp32f*)pSrc,src->Step(),(const Ipp32f*)pNorm,norm->Step(),
                 pMask,mask->Step(),roi4,&positive,sThreshold[i],pHaar[i]);
              if (ret != ippStsOk) {printf("exit 8\n"); return -8;}
              if (positive<roi4.width*roi4.height*threshall) break;
           }
           if (prun>0) ret = ippiFilterMaxBorderReplicate_8u_C1R(pMask,mask->Step(),pMask,mask->Step(),roi4,masksecond,anchor,buffer);
           for (; i<stages; i++) {
              ret=ippiApplyHaarClassifier_32f_C1R((const Ipp32f*)pSrc,src->Step(),(const Ipp32f*)pNorm,norm->Step(),
                 pMask,mask->Step(),roi4,&positive,sThreshold[i],pHaar[i]);
              if (ret != ippStsOk) {printf("exit 8\n"); return -8;}
           }
           shda+=roi4.height;
           shim+=roi4.height+2;
           allpos+=positive;
        }
       if (allpos) ClusterFaces(mask,clusters,face,factor);
    }
    SetFaces (clusters,dst,rfactor);
    delete clusters;

time = clock()-time;
printf("IPP,  time=%7.2f ms, %8.2f cpe/pixel, minwidth=%d, scale=%8.2f, quality=%d\n",
(double)time,(double)time*freq*1000.0f/(double)(roi0.width*roi0.height),minfacew,sfactor,quality);
    delete srcs1;
    delete srcs2;
    delete srcbg;
    delete src;
    delete sqr;
    delete norm;
    delete mask;
    dst->SaveImage(savefile);
    fclose(rfile);
    ret = FreeHaarClassifier ();
    if (ret==ippStsBadArgErr) {
        fprintf( stderr, "ERROR: cascade is incorrect\n" );
        return -1;
    }
    delete dst;
    delete srcld;
    delete savefile;
    delete imgfile;
    return 0;
}

static int is_equal(int x1,int y1,int w1,int x2,int y2,int w2,float distparam,float rectparam)
{
    float ws=(float)w1,wb=(float)w2;
    if (w1>w2) {ws=(float)w2; wb=(float)w1;}
    bool p = ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=wb*wb*0.01f*distparam*0.01f*distparam) && wb<=ws*(1.0f+0.01f*rectparam);
    return p;
}

void ClusterFaces(CIppiImage *mask, CCluster *clusters, IppiSize face, Ipp32f factor) {
    int i,j,ii,jj,xcenter,ycenter,wrect;
    bool equalrect;
    Ipp8u *maskPtr;
    maskPtr=(Ipp8u*)mask->DataPtr();
    for (ii=0; ii<mask->Height(); ii++) {
       for (jj=0; jj<mask->Width(); jj++) {
          if (!maskPtr[mask->Step()*ii+jj]) continue;
          xcenter=(int)((jj+face.width/2)*factor);
          ycenter=(int)((ii+face.width/2)*factor);
          wrect=(int)(face.width*factor);
          equalrect=false;
          for (i=0;i<clusters->numclust;i++) {
              for (j=0;j<clusters->numface[i];j++) {
                  if (is_equal(xcenter,ycenter,wrect,clusters->x[i][j],clusters->y[i][j],clusters->w[i][j],distfactor,distfactorrect)) {
                      if (!equalrect) clusters->AddFace(i,xcenter,ycenter,wrect);
                      clusters->AddToMerge(i);
                      equalrect=true;
                      break;
                  }
              }
          }
          if (!equalrect) {
              clusters->AddFace(-1,xcenter,ycenter,wrect);
          } else {
              clusters->MergeClusters();
          }
       }
    }
}

void SetFaces (CCluster *clusters,CIppiImage *dst,float rfactor) {
   int i,j,iw,ih;
   int xsum,ysum,wsum;
   Ipp8u *dstPtr=(Ipp8u*)dst->DataPtr();
   CCluster *mergedclusters=clusters;
   for (i=0,xsum=0,ysum=0,wsum=0;i<mergedclusters->numclust;i++,xsum=0,ysum=0,wsum=0) {
        int xface, yface,wface;
        int maxX;
        int maxY;
        for (j=0;j<mergedclusters->numface[i];j++) {
            xsum+=mergedclusters->x[i][j];
            ysum+=mergedclusters->y[i][j];
            wsum+=mergedclusters->w[i][j];
        }
        wface=(int) wsum/mergedclusters->numface[i];
        xface=(int) xsum/mergedclusters->numface[i]-wface/2;
        yface=(int) ysum/mergedclusters->numface[i]-wface/2;
        wface=(int)(wface*rfactor);
        xface=(int)(xface*rfactor);
        yface=(int)(yface*rfactor);
        if ((xface+wface)>dst->Width() || (yface+wface)>dst->Height())
            continue;
        fprintf(rfile,"%d\t%d\t%d\n",xface+wface/2,dst->Height()-(yface+wface/2),wface);
        maxX=xface+wface<dst->Width()-1 ? xface+wface : dst->Width()-1;
        maxY=yface+wface<dst->Height()-1 ? yface+wface : dst->Height()-1;
        for (iw=xface;iw<maxX;iw++) {
            dstPtr[iw*3+yface*dst->Step()]=0;
            dstPtr[iw*3+yface*dst->Step()+1]=0;
            dstPtr[iw*3+yface*dst->Step()+2]=255;
            dstPtr[iw*3+maxY*dst->Step()]=0;
            dstPtr[iw*3+maxY*dst->Step()+1]=0;
            dstPtr[iw*3+maxY*dst->Step()+2]=255;
        }
        for (ih=yface;ih<maxY;ih++) {
            dstPtr[xface*3+ih*dst->Step()]=0;
            dstPtr[xface*3+ih*dst->Step()+1]=0;
            dstPtr[xface*3+ih*dst->Step()+2]=255;
            dstPtr[maxX*3+ih*dst->Step()]=0;
            dstPtr[maxX*3+ih*dst->Step()+1]=0;
            dstPtr[maxX*3+ih*dst->Step()+2]=255;
        }
   }
}

IppStatus  FreeHaarClassifier (void) {
   IppStatus ret=ippStsOk;
   int ii;
   for (ii=0; ii<stages; ii++) {
      ret = ippiHaarClassifierFree_32f (pHaar[ii]);
   }
   delete pHaar;
   delete sThreshold;
   delete nLength;
   delete nClass;
   delete nFeat;
   delete pNum;
   delete pThreshold;
   delete pVal1;
   delete pVal2;
   delete pFeature;
   delete pWeight;
   delete nStnum;
   return ret;
}

IppStatus  AdjustHaarClassifier (int bord, float decstage) {
   IppStatus ret=ippStsOk;
   int ii, jj;
   IppiSize stageClassifierSize;
   classifierSize.width=0;
   classifierSize.height=0;
   float scale=1.0f/(float)((face.width-bord-bord)*(face.height-bord-bord));
   for (jj=0; jj<features; jj++) pWeight[jj] *= scale;
   for (ii=0; ii<stages; ii++) sThreshold[ii] -= decstage;
   for (jj=0; jj<features; jj++)
      pFeature[jj].y = face.height - pFeature[jj].y - pFeature[jj].height;
   for (ii=0; ii<stages; ii++) {
      ret=ippiHaarClassifierInitAlloc_32f(pHaar+ii,pFeature+nFeat[ii],pWeight+nFeat[ii],pThreshold+nClass[ii],pVal1+nClass[ii],pVal2+nClass[ii],pNum+nClass[ii],nLength[ii]);
      if (ret != ippStsOk) return ret;
      ippiGetHaarClassifierSize_32f(pHaar[ii],&stageClassifierSize);
      if (ret != ippStsOk) return ret;
      if (stageClassifierSize.width>classifierSize.width)
          classifierSize.width=stageClassifierSize.width;
      if (stageClassifierSize.height>classifierSize.height)
          classifierSize.height=stageClassifierSize.height;
   }
   return ret;
}

IppStatus  ReadHaarClassifier (char *haarfname) {
   IppStatus ret=ippStsOk;
   FILE *file = fopen(haarfname, "r");
   int ii, jj, kk, jjj=0, kkk=0;
   if (!file) return ippStsBadArgErr;
   fscanf(file, "%d %d %d %d %d", &(face.width), &(face.height), &stages, &classifiers, &features);
   pHaar      = new IppiHaarClassifier_32f* [stages];
   sThreshold = new Ipp32f   [stages];
   nLength    = new int      [stages];
   nStnum     = new int      [stages];
   nClass     = new int      [stages];
   nFeat      = new int      [stages];
   pNum       = new int      [classifiers];
   pThreshold = new Ipp32f   [classifiers];
   pVal1      = new Ipp32f   [classifiers];
   pVal2      = new Ipp32f   [classifiers];
   pFeature   = new IppiRect [features];
   pWeight    = new Ipp32f   [features];

   for (ii=0; ii<stages; ii++) {
      fscanf(file, "%d %g", nLength+ii, sThreshold+ii);
      nStnum[ii] = 0;
      nClass[ii] = jjj;
      nFeat [ii] = kkk;
      for (jj=0; jj<nLength[ii]; jjj++, jj++) {
         fscanf(file, "%d", pNum+jjj);
         nStnum[ii] += pNum[jjj];
         for (kk=0; kk<pNum[jjj]; kkk++, kk++) {
            fscanf(file, "%d %d %d %d %g", &((pFeature+kkk)->x), &((pFeature+kkk)->y), &((pFeature+kkk)->width), &((pFeature+kkk)->height), pWeight+kkk);
         }
         fscanf(file, "%g %g %g", pThreshold+jjj, pVal1+jjj, pVal2+jjj);
      }
      if (ret != ippStsOk) return ret;
   }

   fclose(file);
   if ((jjj!=classifiers) || (kkk!=features))
      ret = ippStsBadArgErr;
   return ret;
}

