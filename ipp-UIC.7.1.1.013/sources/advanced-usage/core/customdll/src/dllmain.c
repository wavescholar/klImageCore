/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

#if defined( _WIN32 )
  #define STRICT
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#elif defined( linux )
#endif  /* _WIN32 */

#include "ippcore.h"


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason,
                    LPVOID lpvReserved )
{
    switch( fdwReason ) {
      case DLL_PROCESS_ATTACH: ippStaticInit();

    default:
        hinstDLL;
        lpvReserved;
        break;
    }
    return TRUE;
}

/* //////////////////////// End of file "dllmain.c" ///////////////////////// */
