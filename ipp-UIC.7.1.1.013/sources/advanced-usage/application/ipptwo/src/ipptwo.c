/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "ipp.h"
#include <stdio.h>

int errMsg( int ret, const char* err, const char* msg ) {
   printf( "\n--%s. %s \n\n", err, msg );
   return ret;
}

int main( void ) {

   static const char* unexpctMsg = "Unexpected Result";
   const IppLibraryVersion* libver;       /* library info structure ptr */
   IppStatus status;                      /* library status */

   int x, y;
   Ipp32f sum;

   const int side = 12, len = side * side;
   IppiSize imgSize = { side, side };      /* image size structure */
   IppiPoint roiOffset = { 0, 0 };
   IppiMomentState_64f* pMomState;
   Ipp64f mom;                            /* spatial moment value */

   Ipp32f* img = ippsMalloc_32f( len );

   /* first print the version of the libraries */
   libver = ippsGetLibVersion();
   printf( " %s %s \n", libver->Name, libver->Version );
   libver = ippiGetLibVersion();
   printf( " %s %s \n\n", libver->Name, libver->Version );

   printf( "Image img1 with special data is created ..\n" );
   for( y=0; y<side; ++y )
      for( x=0; x<side; ++x )
         img[y*side+x] = (float)x*y;

   /* direct way to compute the sum of the image by using 1D function */
   status = ippsSum_32f( img, len, &sum, ippAlgHintNone );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   printf( "Image img2 with pixel values equal to 1 is created ..\n\n" );
   /* It is a test, please use Set function */
   status = ippsThreshold_32f_I( img, len, 1, ippCmpLess );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   status = ippsThreshold_32f_I( img, len, 1, ippCmpGreater);
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   status = ippiMomentInitAlloc_64f( &pMomState, ippAlgHintNone );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   /* Compute 2D moments. 1-st must be equal to the sum computed above */
   status = ippiMoments64f_32f_C1R( img, side*sizeof(Ipp32f), imgSize, pMomState );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   /* get 1-st moment value */
   status = ippiGetSpatialMoment_64f( pMomState, 1,1,0, roiOffset, &mom );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );


   printf( " sum for img1 = %.1f, 1st moment for img2 = %.1f\n", sum, (float)mom );
   if( mom != sum )
      return errMsg( -1, unexpctMsg, "Sum of img1 and 1st moment of img2 must be equal" );

   /* get 0-st moment value */
   status = ippiGetSpatialMoment_64f( pMomState, 0,0,0, roiOffset, &mom );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   status = ippiMomentFree_64f( pMomState );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );

   /* get sum. Must be equal to 0-moment and to area of the image */
   status = ippsSum_32f( img, len, &sum, ippAlgHintNone );
   if( ippStsNoErr != status )
      return errMsg( 1, "Library Status Error", ippGetStatusString( status ) );
   printf( " 0 moment for img2 = %.1f, sum for img2 = %.1f, img2 area = %d\n\n",
      (float)mom, sum, len );
   if( mom != sum || sum != len )
      return errMsg( -2, unexpctMsg, "Sum of img2 and 0 moment of img2 must be equal to area" );

   ippsFree( img );
   return 0;
}

