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

// eqrect.h: interface for the CCluster class.
//          It contains structure for store clusters of rectangulars
//          It contains types of prunings
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#define INC_FACE 20
#define NUM_CLUST 10
#define INC_CLUST 10

class CCluster {
        public:
        int **x;
        int **y;
        int **w;
        int *maxface;
        int *numface;
        int *mergeclust;
        int maxclust;
        int numclust;
        bool merged;
        CCluster(int num=NUM_CLUST);
        ~CCluster(void);
        void AddFace(int index,int xcenter,int ycenter,int wrect);
        void AddToMerge(int index);
        void MergeClusters(void);
};

CCluster::CCluster(int num) {
    maxclust=num;
    if (maxclust<NUM_CLUST) maxclust=NUM_CLUST;
    maxface=(int*)malloc((3*sizeof(int)+3*sizeof(int*))*maxclust);
    numface=maxface+maxclust;
    mergeclust=numface+maxclust;
    x=(int**)(mergeclust+maxclust);
    y=x+maxclust;
    w=y+maxclust;
    ippsZero_8u((Ipp8u*)maxface,(3*sizeof(int)+3*sizeof(int*))*maxclust);
    numclust=0;
    merged=true;
}

void CCluster::AddFace(int index,int xcenter,int ycenter,int wrect) {
    if (index>=numclust) return;
    if (index<0) {
        if (numclust>=maxclust) {
           int *p,*l, *m;
           int **px, **py, **pw;
           maxclust+=INC_CLUST;
           p=(int*)malloc((3*sizeof(int)+3*sizeof(int*))*maxclust);
           l=p+maxclust;
           m=l+maxclust;
           px=(int**)(m+maxclust);
           py=px+maxclust;
           pw=py+maxclust;
           ippsCopy_32s(maxface,p,numclust);
           ippsZero_32s(p+numclust,maxclust-numclust);
           ippsCopy_32s(numface,l,numclust);
           ippsZero_32s(l+numclust,maxclust-numclust);
           ippsCopy_32s(mergeclust,m,numclust);
           ippsZero_32s(m+numclust,maxclust-numclust);
           ippsCopy_8u((Ipp8u*)x,(Ipp8u*)px,numclust*sizeof(int*));
           ippsZero_8u((Ipp8u*)(px+numclust),(maxclust-numclust)*sizeof(int*));
           ippsCopy_8u((Ipp8u*)y,(Ipp8u*)py,numclust*sizeof(int*));
           ippsZero_8u((Ipp8u*)(py+numclust),(maxclust-numclust)*sizeof(int*));
           ippsCopy_8u((Ipp8u*)w,(Ipp8u*)pw,numclust*sizeof(int*));
           ippsZero_8u((Ipp8u*)(pw+numclust),(maxclust-numclust)*sizeof(int*));
           free(maxface);
           maxface=p;
           numface=l;
           mergeclust=m;
           x=px;
           y=py;
           w=pw;
        }
        numclust++;
        AddFace(numclust-1,xcenter,ycenter,wrect);
    } else {
        if (numface[index]>=maxface[index]) {
           int *px, *py, *pw;
           maxface[index]+=INC_FACE;
           px=(int*)malloc(3*maxface[index]*sizeof(int));
           py=px+maxface[index];
           pw=py+maxface[index];
           ippsCopy_32s(x[index],px,numface[index]);
           ippsZero_32s(px+numface[index],maxface[index]-numface[index]);
           ippsCopy_32s(y[index],py,numface[index]);
           ippsZero_32s(py+numface[index],maxface[index]-numface[index]);
           ippsCopy_32s(w[index],pw,numface[index]);
           ippsZero_32s(pw+numface[index],maxface[index]-numface[index]);
           free(x[index]);
           x[index]=px;
           y[index]=py;
           w[index]=pw;
        }
        x[index][numface[index]]=xcenter;
        y[index][numface[index]]=ycenter;
        w[index][numface[index]]=wrect;
        numface[index]++;
    }
}

void CCluster::AddToMerge(int index) {
   if (index<0||index>=numclust) return;
   mergeclust[index]=1;
   merged=false;
}

void CCluster::MergeClusters(void) {
   int i,j,len,k;
   if (merged) return;
   for (i=0; i<numclust; i++) {
      if (mergeclust[i]) break;
   }
   if (i>=numclust) return;
   for (len=numface[i], j=i+1; j<numclust; j++) {
      if (mergeclust[j]) len+=numface[j];
   }
   if (len==numface[i]) {
      mergeclust[i]=0;
      merged=true;
      return;
   }
   if (len>maxface[i]) {
      int *px, *py, *pw;
      maxface[i]=len+INC_FACE;
      px=(int*)malloc(3*maxface[i]*sizeof(int));
      py=px+maxface[i];
      pw=py+maxface[i];
      ippsCopy_32s(x[i],px,numface[i]);
      ippsCopy_32s(y[i],py,numface[i]);
      ippsCopy_32s(w[i],pw,numface[i]);
      free(x[i]);
      x[i]=px;
      y[i]=py;
      w[i]=pw;
   }
   for (len=numface[i], j=i+1; j<numclust; j++) {
      if (!mergeclust[j]) continue;
      ippsCopy_32s(x[j],x[i]+len,numface[j]);
      ippsCopy_32s(y[j],y[i]+len,numface[j]);
      ippsCopy_32s(w[j],w[i]+len,numface[j]);
      len+=numface[j];
   }
   ippsZero_32s(x[i]+len,maxface[i]-len);
   ippsZero_32s(y[i]+len,maxface[i]-len);
   ippsZero_32s(w[i]+len,maxface[i]-len);
   numface[i]=len;
   mergeclust[i]=0;
   for (k=j=i+1; j<numclust; j++) {
      if (mergeclust[j]) {
         free(x[j]);
         mergeclust[j]=0;
      } else {
         maxface[k]=maxface[j];
         numface[k]=numface[j];
         x[k]=x[j];
         y[k]=y[j];
         w[k]=w[j];
         k++;
      }
   }
   ippsZero_32s(maxface+k,j-k);
   ippsZero_32s(numface+k,j-k);
   ippsZero_8u((Ipp8u*)(x+k),(j-k)*sizeof(int*));
   ippsZero_8u((Ipp8u*)(y+k),(j-k)*sizeof(int*));
   ippsZero_8u((Ipp8u*)(w+k),(j-k)*sizeof(int*));
   numclust=k;

   merged=true;
}


CCluster::~CCluster(void) {
    int i;
    for (i=0; i<maxclust; i++) {
        if (x[i]) free(x[i]);
    }
    free(maxface);
}
