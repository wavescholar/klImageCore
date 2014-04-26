/* //////////////////////////////// "ippdecompress.c" ////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights
//          Reserved.
//
//          Sample of IPP DC ( data compression ) domain
*/
#include "bwtscheme.h"
#include "gitscheme.h"
#include "lzssscheme.h"

/* outputs message for users in case of wrong input */
void usage(char *name) {
    char usage_message[] = \
    "\nCopyright(c) 2005-2012 Intel Corporation. All Rights Reserved.\nippdecompress datacompression sample\n"
    "Usage: %s coded_filename restored_filename\n"
    " coded_filename                       File to decode\n"
    " restored_filename                    Restored file\n"
    "\nNote, that order of the arguments is predefined .\n"
    "\nExample: %s file1.compr file1 \n";
    fprintf( stderr, usage_message, name, name );
    exit(1);
}


int main( int argC, char **argV ) {

    /* variables */
    int blocksize       = 0;
    int DstLen          = 0;
    int SrcLen          = 0;
    int filesize        = 0;
    int filesizer       = 0;

    bwtState*        pBWTState  = NULL;
    gitState*        pGITState  = NULL;
    IppHuffState_8u* pHuffState = NULL;

    Ipp8u*  pSrc        = NULL;
    Ipp8u*  pDst        = NULL;

    /* input and output files descriptors */
    /* (including gzip file descriptor when -gzip method flag) */
    FILE*    fi         = NULL;
    FILE*    fo         = NULL;
    gzFile fgzip        = NULL;
    int gzip_out_len    = 0;

    /* predefined variables values */
    IppGITStrategyHint strategyHint = DEFAULTGITSTRATEGY;
    int decodeMethod = 0;

    /* in case, when we deal with a static libraries, *
     * init the best one for the current architecture */
    if( ippStaticInit() < 0 ) {
        fprintf( stderr, "Can't init IPP libraries. Exiting.\n");
        exit(-1);
    }

    /* in case when extra parameters or missing some of prints the usage message and exits */
    if( argC != 3 ) usage( argV[0] );

    if( NULL == ( fi = fopen( argV[1], "rb" ) ) ) {
        fprintf( stderr, "Error while reading source file!\n" );
        exit(1);
    } else { /* if everything is OK, checks the filesize */
        fseek( fi, 0, SEEK_END );
        filesize = ftell( fi );
        rewind(fi);
    }
    fread( &decodeMethod, sizeof(Ipp8u), 1, fi );
    fclose(fi);

    /* we can start a first byte value more than 5 only in gzip-file case */

    if(decodeMethod > 5) decodeMethod = GZIP;

    if(decodeMethod == GZIP) {
      if( NULL == ( fgzip = gzopen( argV[1], "rb" ) ) ) {
        fprintf( stderr, "Error while reading source file!\n" );
        exit(1);
      }
    } else {
      /* opens input file */
      if( NULL == ( fi = fopen( argV[1], "rb" ) ) ) {
        fprintf( stderr, "Error while reading source file!\n" );
        exit(1);
      }
    }

    if( NULL == ( fo = fopen( argV[2], "wb" ) ) ) {
        fprintf(stderr, "Error while writing destination file!\n");
        exit(2);
    }

    if( decodeMethod == GZIP) {
      blocksize = GZIPBLOCKSIZE;
    } else {
      fread( &decodeMethod, sizeof(Ipp8u), 1, fi );
      fread( &blocksize, sizeof(Ipp32u), 1, fi );
    }

    /* allocates enough memory for input and output buffer */
    pSrc = (Ipp8u *)malloc(sizeof(Ipp8u)*(blocksize<<1));
    pDst = (Ipp8u *)malloc(sizeof(Ipp8u)*(blocksize<<1));

    /* make method-dependable init */
    switch( decodeMethod ) {
        case BWT: Decompress1StateInitAlloc( &pBWTState, blocksize ); break;
        case GIT: Decompress2StateInitAlloc( &pGITState, blocksize, strategyHint ); break;
    }

    switch ( decodeMethod ) {

        case BWT: /* BWT -> MTF -> RLE -> Huffman */
            while( (int)fread( &SrcLen, sizeof(Ipp32u), 1, fi ) > 0 ) {
                DstLen = (blocksize<<1);
                SrcLen = (int)fread( pSrc, sizeof(Ipp8u), SrcLen, fi );
                Decompress1( &pSrc, SrcLen, &pDst, &DstLen, pBWTState );
                fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
            }
        break;

        case GIT: /* BWT -> GIT */
            while( (int)fread( &SrcLen, sizeof(Ipp32u), 1, fi ) > 0 ) {
                /* sets the size of destination buffer equal to size of memory allocated */
                DstLen = (blocksize<<1);
                SrcLen = (int)fread( pSrc, sizeof(Ipp8u), SrcLen, fi );
                Decompress2( &pSrc, SrcLen, &pDst, &DstLen, pGITState );
                fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
            }
        break;

        case LZSS: /* LZSS */
            SrcLen = (int)fread( pSrc, sizeof(Ipp8u), filesize, fi );
            DstLen = filesize << 1;
            DecompressLZSS( pSrc, SrcLen, pDst, &DstLen );
            fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
        break;

        case GZIP:
            for(;;)
            {
               gzip_out_len = gzread(fgzip, pSrc, blocksize);

               if (gzip_out_len < 0)  break;
               if (gzip_out_len == 0) break;
               if ( (int)fwrite( pSrc, 1, (unsigned)gzip_out_len, fo) != gzip_out_len ) return (-1);
            }
        break;

        case HUFFMAN: /* HUFFMAN only */
            while( (int)fread( &SrcLen, sizeof(Ipp32u), 1, fi ) > 0 ) {
                DstLen = (blocksize<<1);
                SrcLen = (int)fread( pSrc, sizeof(Ipp8u), SrcLen, fi );
                if( !DecodeHuffman( pSrc, SrcLen, pDst, &DstLen, pHuffState ) )
                    fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
            }
        break;
    }

    /* does finish tasks: close files, clean buffers */

    if( decodeMethod == GZIP )
      gzclose(fgzip);
    else
      fclose(fi);

    fclose(fo);

    fo = fopen( argV[2], "rb" );
    fseek( fo, 0, SEEK_END );
    filesizer = ftell( fo );
    fclose( fo );

    printf("Decoded file %s (%d bytes) to file %s (%d bytes)\n", argV[1], filesize, argV[2], filesizer);

    if( decodeMethod == BWT ) Free1State(pBWTState);
    if( decodeMethod == GIT ) Free2State(pGITState);

    free(pSrc);
    free(pDst);

    return 0; /* everything is OK */
}
