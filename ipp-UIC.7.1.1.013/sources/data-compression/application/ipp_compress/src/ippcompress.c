/* //////////////////////////////// "ippcompress.c" //////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights
//          Reserved.
//
//
//          Sample of IPP DC ( data compression ) domain
//          BWT, MTF, RLE and Huffman encoding functions usage
*/

#include "bwtscheme.h"
#include "gitscheme.h"
#include "lzssscheme.h"

void usage(char *name) {
    char usage_message[] = \
    "\nCopyright(c) 2005-2012 Intel Corporation. All Rights Reserved.\nippcompress datacompression sample\n"
    "Usage: %s [ -bwt | -git | -lzss | -gzip | -huffman] [-1...-9]source_filename destination_filename\n"
    " -bwt, -git, -lzss, -gzip, -huffman ...  Defines compression algorithms chain by name\n"
    " -1 ... -9                               Sets the custom size of the input block while encoding from 1...9 \n"
    "                                         where 1 means 100Kb, 2 means 200Kb, ... 9 means 900Kb\n"
    " source_filename                         File to encode\n"
    " destination_filename                    Encoded file\n"
    "\nNote, that order of the arguments is predefined!\n"
    "\nExample: %s -git -3 file1 file1.compr\n";
    fprintf( stderr, usage_message, name, name );
    exit(1);
}


int main( int argC, char **argV ) {

    int blocksize       = 0,
        DstLen          = 0,
        SrcLen          = 0,
        filesize        = 0,
        filesizer       = 0;

    bwtState *pBWTState = NULL;
    gitState *pGITState = NULL;
    IppHuffState_8u *pHuffState = NULL;

    Ipp8u * pSrc        = NULL,
          * pDst        = NULL,
            header[4];

    char    outmode[4];

    /* input and output files descriptors (including gzip file descriptor when -gzip method flag)*/
    FILE   * fi         = NULL,
           * fo         = NULL;
    gzFile   fgzip      = NULL;

    /* predefined variables values */
    IppGITStrategyHint strategyHint = DEFAULTGITSTRATEGY;
    int gzipMode                    = GZIPAVERAGE;
    int gzip_in_len                 = 0;
    int encodeMethod                = 0;

    /* in case, when we deal with a static libraries, *
     * init the best one for the current architecture */
    if( ippStaticInit() < 0 ) {
        fprintf( stderr, "Can't init IPP libraries. Exiting.\n");
        exit(-1);
    }

    /* in case when extra parameters or missing some of prints the usage message and exits*/
    if( argC < 4 || argC > 5 ) usage( argV[0] );

    if( argV[1][0] == '-' ) {
        if( strcmp( argV[1], "-bwt"  ) == 0 ) encodeMethod = BWT;        /* RLE->BWT->MTF->RLE->Huffman scheme */
        if( strcmp( argV[1], "-git"  ) == 0 ) encodeMethod = GIT;        /* BWT->GIT scheme */
        if( strcmp( argV[1], "-lzss" ) == 0 ) encodeMethod = LZSS;       /* LZSS scheme */
        if( strcmp( argV[1], "-gzip" ) == 0 ) encodeMethod = GZIP;       /* GZIP scheme (gzwrite, gzread) */
        if( strcmp( argV[1], "-huffman" ) == 0 ) encodeMethod = HUFFMAN; /* Huffman coding */
        if( encodeMethod == 0) usage(argV[0]);
    }
    else usage(argV[0]);    /* prints usage message */

    /* gets a size of block parameter from a command line */
    if( argV[2][0] != '-' || argV[2][1] > '9' || argV[2][1] < '1' || argV[3] == 0 || argV[4] == 0 ) {
        fprintf( stderr, "Error in command line");
        usage( argV[0] );
    }

    /* opens input file */
    if( NULL == ( fi = fopen( argV[3], "rb" ) ) ) {
        fprintf( stderr, "Error while reading source file!\n" );
        exit(1);
    } else { /* if everything is OK, checks the filesize */
        while( !feof(fi) ) filesize += (int)fread( header, sizeof(Ipp8u), 4, fi );
        blocksize = filesize;
        rewind(fi);
    }

    /* Converts blocksize from command line to int. In case of LZSS we have to encode the entire file instead of block-by-block. */
    if( encodeMethod != LZSS && encodeMethod != GZIP ) blocksize = (int)(argV[2][1] - 48) * BLOCKUNIT;

    /* opens output file (if gzip encoding case - uses gzopen function and 'gzfile *file' descriptor )*/
    if( encodeMethod == GZIP ) { /* zlib or gzip */
      sprintf(outmode, "wb%d", gzipMode);
      if( ( fgzip = gzopen( argV[4], outmode ) ) == NULL ) {
        fprintf(stderr, "Error writing destination file!\n");
        exit(2);
      }
    } else { /* others */
      if( NULL == ( fo = fopen( argV[4], "wb" ) ) ) {
        fprintf(stderr, "Error writing destination file!\n");
        exit(2);
      }
    }
    /* writes a 5 service bytes in case of -bwt, -git, -lzss and -zlib cases.
       for gzip-files compatibility, not writes this bytes in -gzip case if GZIP defined */
    if( encodeMethod == GZIP ) {
      blocksize = GZIPBLOCKSIZE; /* hint: change it to define */
    } else {
      /* writes the method (1 to 4) of compression to first byte */
      fwrite( &encodeMethod, sizeof(Ipp8u), 1, fo );
      /* writes the compression block size (or destination filesize) to next 4 bytes */
      fwrite( &blocksize, sizeof(Ipp32u), 1, fo );
    }

  /* allocates enough memory for input and output buffer */
    pSrc = (Ipp8u *)malloc(sizeof(Ipp8u)*(blocksize<<1));
    pDst = (Ipp8u *)malloc(sizeof(Ipp8u)*(blocksize<<1));

    /* make method-dependable init */
    switch( encodeMethod ) {
        case BWT: Compress1StateInitAlloc( &pBWTState, blocksize ); break;
        case GIT: Compress2StateInitAlloc( &pGITState, blocksize, strategyHint ); break;
    }

    switch ( encodeMethod ) {

        case BWT: /* BWT -> MTF -> RLE -> Huffman */
            while( ( SrcLen = (int)fread( pSrc, sizeof(Ipp8u), blocksize, fi ) ) > 0  ) {
                /* sets the size of destination buffer equal to size of memory allocated */
                DstLen = (blocksize<<1);
                /* compress read block */
                Compress1( &pSrc, SrcLen, &pDst, &DstLen, pBWTState );
                /* writes the 4 bytes (block length) into output file */
                fwrite( &DstLen, sizeof(Ipp32u), 1, fo );
                /* writes the (block length) bytes into output file */
                fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
            }
        break;

        case GIT: /* BWT -> GIT */
            while( ( SrcLen = (int)fread( pSrc, sizeof(Ipp8u), blocksize, fi ) ) > 0  ) {
                /* sets the size of destination buffer equal to size of memory allocated */
                DstLen = (blocksize<<1);
                /* compress read block */
                Compress2( &pSrc, SrcLen, &pDst, &DstLen, pGITState );
                /* writes the 4 bytes (block length) into output file */
                fwrite( &DstLen, sizeof(Ipp32u), 1, fo );
                /* then writes the (block length) bytes into output file */
                fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
            }
        break;

        case LZSS: /* LZSS */
            /* read entire file to pSrc vector */
            SrcLen = (int)fread( pSrc, sizeof(Ipp8u), blocksize, fi );
            DstLen = blocksize << 1;
            /* compress read block */
            CompressLZSS( pSrc, SrcLen, pDst, &DstLen );
            /* writes the (block length) bytes into output file */
            fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
        break;

        case GZIP: /* GZIP */
            for(;;)
            {
               gzip_in_len = (int)fread( pSrc, sizeof(Ipp8u), blocksize, fi);
               if(gzip_in_len == 0) break;
               if(gzwrite(fgzip, pSrc, (unsigned)gzip_in_len) != gzip_in_len) return (-1);
            } /* for */
        break;

        case HUFFMAN: /* HUFFMAN only */
            while( ( SrcLen = (int)fread( pSrc, sizeof(Ipp8u), blocksize, fi ) ) > 0  ) {
                /* sets the size of destination buffer equal to size of memory allocated */
                DstLen = (blocksize<<1);
                /* compress read block */
                if( !EncodeHuffman( pSrc, SrcLen, pDst, &DstLen, pHuffState ) )
                {
                    /* writes the 4 bytes (block length) into output file */
                    fwrite( &DstLen, sizeof(Ipp32u), 1, fo );
                    /* writes the (block length) bytes into output file */
                    fwrite( pDst, sizeof(Ipp8u), DstLen, fo );
                }
            }
        break;
    }

    /* does finish tasks: close files, clean buffers */
    if( encodeMethod == GZIP )
      gzclose(fgzip);
    else
      fclose(fo);

    fclose(fi);

    fo = fopen( argV[4], "rb" );
    fseek( fo, 0, SEEK_END );
    filesizer = ftell( fo );
    fclose( fo );

    printf("Compressed file %s (%d bytes) to file %s (%d bytes)\n", argV[3], filesize, argV[4], filesizer);

    if( encodeMethod == BWT ) Free1State(pBWTState);
    if( encodeMethod == GIT ) Free2State(pGITState);

    pBWTState = NULL;
    pGITState = NULL;
    free(pSrc);
    free(pDst);

    return 0; /* everything is OK */
}
