/*
//
// INTEL CORPORATION PROPRIETARY INFORMATION
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Intel Corporation and may not be copied
// or disclosed except in accordance with the terms of that agreement.
// Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//   Intel(R)  Integrated Performance Primitives
//
//     USC Echo Canceller sample
//
// By downloading and installing this sample, you hereby agree that the
// accompanying Materials are being provided to you under the terms and
// conditions of the End User License Agreement for the Intel(R) Integrated
// Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
// product installation for more information.
//
// Purpose: Load static EC components.
//
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usc_ec.h"
#include "util.h"
#include "loadec.h"

StaticNameTableEC LinkedEC[] = {
   "IPP_EC_FP",&USC_ECFP_Fxns,
   "END",NULL
};

void EnumerateStaticLinkedEC(FILE *fptrLog)
{
   char pString[MAX_LEN_STRING];
   int i = 0;
   OutputInfoString(0, fptrLog, "Following echo cancellers are linked:\n");
   while(strcmp(LinkedEC[i].ECName,"END") != 0) {
      sprintf(pString, "           %s\n",LinkedEC[i].ECName);
      OutputInfoString(0, fptrLog, (const char*)pString);
      i++;
   }
}

void* GetObjectEC(const char *name)
{
   void *pAddr = NULL;
   int i=0;
   while(strcmp(LinkedEC[i].ECName,"END")!=0) {
      if(strcmp(name, LinkedEC[i].ECName) == 0) {
         pAddr = LinkedEC[i].funcPtr;
         break;
      }
      i++;
   }
   return pAddr;
}

int GetNumLinkedEC(void)
{
   int i = 0;
   while(strcmp(LinkedEC[i].ECName,"END")!=0) {
      i++;
   }
   return i;
}

int LoadECByName(const char *name, USC_EC_Params *params, FILE *f_log)
{
   char pString[MAX_LEN_STRING];

   params->pUSC_EC_Fxns = NULL;
   params->pUSC_EC_Fxns = (USC_EC_Fxns*)GetObjectEC(name);
   if(params->pUSC_EC_Fxns) {
      sprintf(pString, "Echo canceller type %s chosen\n", name);
      OutputInfoString(1, f_log, (const char*)pString);
      return 0;
   } else return -1;
}



