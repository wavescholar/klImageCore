/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

#include <stdio.h>
#include "ipp.h"
#include "i_malloc.h"

void *my_malloc(size_t len)
{
    printf("my_malloc(%d)\n", (int)len);
    return malloc(len);
}

void *my_calloc(size_t nmemb, size_t size)
{
    printf("my_calloc(%d, %d)\n", (int)nmemb, (int)size);
    return calloc(nmemb, size);
}

void *my_realloc(void *ptr, size_t len)
{
    printf("my_realloc(%d)\n", (int)len);
    return realloc(ptr, len);
}

void my_free(void *ptr)
{
    printf("my_free()\n");
    free(ptr);
}


#define s_array 5

int main()
{
    char    *hello;
    Ipp16s  *src1, *src2, *dst;
    Ipp8u   *i_src, *i_dst;
    int     i, j, src_step, dst_step;
    IppStatus   status;
    IppiSize    size;
    unsigned int    seed;

#ifdef REPLACE_MEM
    i_malloc = my_malloc;
    i_calloc = my_calloc;
    i_realloc = my_realloc;
    i_free = my_free;
#endif

#ifdef REPLACE_MEM_DLL
    i_malloc_dll = my_malloc;
    i_calloc_dll = my_calloc;
    i_realloc_dll = my_realloc;
    i_free_dll = my_free;
#endif

    ippStaticInit();
    {
        const IppLibraryVersion* core_ver= ippGetLibVersion();
        const IppLibraryVersion* s_ver= ippsGetLibVersion();
        const IppLibraryVersion* i_ver= ippiGetLibVersion();


        printf( "\nUsage: %s v%s, %s v%s, %s v%s\n\n", core_ver->Name, core_ver->Version, s_ver->Name,
                s_ver->Version, i_ver->Name, i_ver->Version );

        printf( ">>> Allocate memory by ippMalloc\n\n" );
        hello= ippMalloc(20);
        sprintf( hello, "Hello world!\n" );
        printf( "%s", hello );
        printf( "\n<<< Free memory by ippFree\n\n" );
        ippFree( hello );

        printf( ">>> Allocate memory by ippsMalloc\n\n" );
        src1= ippsMalloc_16s(s_array);
        src2= ippsMalloc_16s(s_array);
        dst = ippsMalloc_16s(s_array);

        status= ippsRandUniform_Direct_16s( src1, s_array, 0, 10, &seed );
        if( status ) {
            ippGetStatusString( status );
            ippsFree( src1 );
            ippsFree( src2 );
            ippsFree( dst );
            return -1;
        }
        status= ippsRandUniform_Direct_16s( src2, s_array, 0, 10, &seed );
        if( status ) {
            ippGetStatusString( status );
            ippsFree( src1 );
            ippsFree( src2 );
            ippsFree( dst );
            return -1;
        }

        status= ippsAdd_16s( src1, src2, dst, s_array );
        if( status ) {
            ippGetStatusString( status );
            ippsFree( src1 );
            ippsFree( src2 );
            ippsFree( dst );
            return -1;
        }

        printf( "1 array:\t" );
        for( i= 0; i < s_array; i++ ) {
            printf( "%i\t", src1[i] );
        }
        printf( "\n" );

        printf( "2 array:\t" );
        for( i= 0; i < s_array; i++ ) {
            printf( "%i\t", src2[i] );
        }
        printf( "\n" );

        printf( "----------------------------------------------------------\n" );

        printf( "summary:\t" );
        for( i= 0; i < s_array; i++ ) {
            printf( "%i\t", dst[i] );
        }
        printf( "\n" );

        printf( "\n<<< Free memory by ippsFree\n\n" );
        ippsFree( src1 );
        ippsFree( src2 );
        ippsFree( dst );

        printf( ">>> Allocate memory by ippiMalloc\n\n" );

        size.width= 4;
        size.height= 2;

        i_src= ippiMalloc_8u_C1( size.width, size.height, &src_step );
        i_dst= ippiMalloc_8u_C1( size.width, size.height, &dst_step );

        status= ippiAddRandUniform_Direct_8u_C1IR( i_src, src_step, size, 2, 64, &seed );
        if( status ) {
            ippGetStatusString( status );
            ippiFree( i_src );
            ippiFree( i_dst );
            return -1;
        }

        status= ippiSqrt_8u_C1RSfs( i_src, src_step, i_dst, dst_step, size, 0 );
        if( status ) {
            ippGetStatusString( status );
            ippiFree( i_src );
            ippiFree( i_dst );
            return -1;
        }

        printf( "Source:\n" );
        for( j= 0; j < size.height; j++ ) {
            for( i= 0; i < size.width; i++ ) {
                printf( "%i\t", i_src[j*src_step+i] );
            }
            printf( "\n" );
        }

        printf( "\nSqrt:\n" );
        for( j= 0; j < size.height; j++ ) {
            for( i= 0; i < size.width; i++ ) {
                printf( "%i\t", i_dst[j*dst_step+i] );
            }
            printf( "\n" );
        }

        printf( "\n<<< Free memory by ippiFree\n\n" );
        ippiFree( i_src );
        ippiFree( i_dst );
    }

    return 0;
}
