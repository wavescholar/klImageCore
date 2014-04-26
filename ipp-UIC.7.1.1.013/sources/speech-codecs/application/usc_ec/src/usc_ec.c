/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives EC Sample
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
// to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  Purpose: USC Echo Canceller
//
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (WIN32)
#include <windows.h>
#endif
#include "math.h"
#include <ipps.h>
#include <ippsc.h>
#include <ippcore.h>

#include "util.h"
#include "loadec.h"
#include "usc_ec.h"

#define USAGE                   1
#define FOPEN_FAIL              2
#define MEMORY_FAIL             3
#define UNKNOWN_FORMAT          4
#define LOAD_EC_FAIL            5
#define ERROR_INIT              6

static Ipp32s PrepareInput(FILE* pFile, Ipp8s* comment, Ipp8u** pBuff, FILE *fptrLog);
/* This object and functions are used to measure echo canceller speed */
typedef struct {
   Ipp64u p_start;
   Ipp64u p_end;
   double speed_in_mhz;
}MeasureIt;

static Ipp64s diffclocks(Ipp64u s, Ipp64u e){
   if(s < e) return (e-s);
   return (IPP_MAX_64S-e+s);
}

static double getUSec(Ipp64u s, Ipp64u e) {
   return diffclocks(s,e)/1000000.0;
}

static void measure_start(MeasureIt *m)
{
   m->p_start = ippGetCpuClocks();
}
static void measure_end(MeasureIt *m)
{
   m->p_end = ippGetCpuClocks();
}
static void measure_output(FILE *f_log, MeasureIt *m, double speech_sec)
{
    Ipp8s pString[MAX_LEN_STRING];

    m->speed_in_mhz = getUSec(m->p_start,m->p_end)/speech_sec;
    sprintf(pString, "%4.2f MHz per channel\n", m->speed_in_mhz);
    OutputInfoString(1, f_log, (const Ipp8s*)pString);
}

static Ipp32s PostProcessPCMstream(Ipp8s *pSrcDst, Ipp32s len)
{
    if (pSrcDst==NULL) {
        return -1;
    }
    if (len==0) {
        return -1;
    }
#if defined( _BIG_ENDIAN )
     ippsSwapBytes_16u_I((Ipp16u*)pSrcDst, len>>1);
#endif
    return 0;
}

static Ipp32s PreProcessBitstream(Ipp8s *pSrcDst, Ipp32s len)
{
    if (pSrcDst==NULL) {
        return -1;
    }
    if (len==0) {
        return -1;
    }
#if defined( _BIG_ENDIAN )
    ippsSwapBytes_16u_I((Ipp16u*)pSrcDst, len>>1);
#endif
    return 0;
}
/************************************************************************/
#define USAGE          1
#define FOPEN_FAIL     2
#define MEMORY_FAIL    3
#define frameSize 5600  // single talk portion, G.168 6.4.1.2
#define subFrameSize 56
#define sub_frame_num frameSize/subFrameSize
static IppPCMFrequency freq = IPP_PCM_FREQ_8000;

/************************************************************************/
int main(int argc, char **argv)
{
   CommandLineParams clParams;
   USC_EC_Params ecParams;
   USC_Status USCStatus;
   MeasureIt measure;
   FILE *fp_rin = NULL;
   FILE *fp_sin = NULL;
   FILE *fp_sout = NULL;
   FILE *f_log = NULL;
   vm_file *f_csv = NULL;        /* csv File    */
   Ipp8u *in1_buff_cur, *in2_buff_cur, *out_buff_cur;
   Ipp8u *in1_buff=NULL, *in2_buff=NULL, *out_buff=NULL;

   Ipp32s flen, in_len, lCallResult;
   Ipp32s i, frameNum, tailNum;
   Ipp32s usage=0, n_repeat=1;
   double speech_sec;
   Ipp32s delay=0;
   Ipp8s* appName=argv[0];
   Ipp8s pString[MAX_LEN_STRING];
   const  IppLibraryVersion *ver = NULL;
//   Ipp32f mu,maxMu=0,erle,maxErle=-1.f,muc;
   ippStaticInit();

   SetCommandLineByDefault(&clParams);
   strcpy(clParams.csvFileName, "ec_speed.csv");

   usage = ReadCommandLine(&clParams, argc, argv);
   if(clParams.puttolog == 1) {
     if((f_log = fopen(clParams.logFileName, "a")) == NULL) return FOPEN_FAIL;
   } else f_log = NULL;
   if(usage) {
      int ret = 0;
      if(clParams.enumerate == 1) {
         EnumerateStaticLinkedEC(f_log);
         if(f_log) fclose(f_log);
      } else {
          PrintUsage((const Ipp8s *)appName, f_log);
          ret = USAGE;
      }
      if(f_log) fclose(f_log);
      return ret;
   }

   lCallResult = LoadECByName((const Ipp8s *)clParams.ECName, &ecParams, f_log);
   if(lCallResult < 0) {
      sprintf(pString, "Cannot find %s echo canceller.\n", clParams.ECName);
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(f_log) fclose(f_log);
      return LOAD_EC_FAIL;
   }
   ecParams.pUSC_EC_Fxns->std.GetInfo((USC_Handle)NULL, &ecParams.pInfo);
   ecParams.objEC = NULL;
   ecParams.pBanks = NULL;
   ecParams.nBanks = 0;

   if((fp_rin = fopen(clParams.rinFileName,"rb")) == NULL)  {
      sprintf(pString, "echo canceller: File %s [r-in] could not be open.\n", clParams.rinFileName);
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(f_log) fclose(f_log);
      return FOPEN_FAIL;
   }
   if((fp_sin = fopen(clParams.sinFileName,"rb")) == NULL) {
      sprintf(pString, "echo canceller: File %s [s-in] could not be open.\n", clParams.sinFileName);
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(f_log) fclose(f_log);
      if(fp_rin) fclose(fp_rin);
      return FOPEN_FAIL;
   }
   if((fp_sout = fopen(clParams.soutFileName,"wb")) == NULL) {
      sprintf(pString, "echo canceller: File %s [s-out] could not be open.\n", clParams.soutFileName);
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(f_log) fclose(f_log);
      if(fp_sin) fclose(fp_sin);
      if(fp_sout) fclose(fp_sout);
      if(fp_rin) fclose(fp_rin);
      return FOPEN_FAIL;
   }
   if(clParams.puttocsv) { /* open the csv file */
      if((f_csv = vm_file_open(clParams.csvFileName, "a")) == NULL) {
         sprintf(pString, "\nFile %s could not be open.\n", clParams.csvFileName);
         OutputInfoString(1, f_log, (const Ipp8s*)pString);
         if(f_log) fclose(f_log);
         if(fp_sin) fclose(fp_sin);
         if(fp_sout) fclose(fp_sout);
         if(fp_rin) fclose(fp_rin);
         return FOPEN_FAIL;
      }
   }
    if(clParams.printSysInfo){
        OutputInfoString(0, f_log,"The Intel(R) echo canceller conformant to ITU G167 and G168,\n");
        ver = ippscGetLibVersion();
        sprintf(pString, "The Intel(R) IPPSC library used:  %d.%d.%d Build %d, name %s\n",
            ver->major,ver->minor,ver->majorBuild,ver->build,ver->Name);
        OutputInfoString(0, f_log, (const Ipp8s*)pString);
        ver = ippsGetLibVersion();
        sprintf(pString, "The Intel(R) IPPSP library used:  %d.%d.%d Build %d, name %s\n",
            ver->major,ver->minor,ver->majorBuild,ver->build,ver->Name);
        OutputInfoString(0, f_log, (const Ipp8s*)pString);
    }
    sprintf(pString, "Input  rin file: %s\n", clParams.rinFileName);
    OutputInfoString(1, f_log, (const Ipp8s*)pString);
    sprintf(pString, "Input  sin file: %s\n", clParams.sinFileName);
    OutputInfoString(1, f_log, (const Ipp8s*)pString);
    sprintf(pString, "Output sout file: %s\n", clParams.soutFileName);
    OutputInfoString(1, f_log, (const Ipp8s*)pString);
    switch(clParams.nlp) {
     case 0:
         sprintf(pString, "NLP disabled\n");
         break;
     case 1:
         sprintf(pString, "NLP type 1 enabled\n");
         break;
     default:
         sprintf(pString, "NLP  type 2 enabled\n");
    }

    switch(clParams.alg) {
     case EC_FULLBAND:
            ecParams.pInfo.params.algType = EC_FULLBAND;
            sprintf(pString, "mode : fullband \n");
            break;
     case EC_SUBBAND:
            ecParams.pInfo.params.algType = EC_SUBBAND;
            sprintf(pString, "mode : subband ,");
            OutputInfoString(1, f_log, (const Ipp8s*)pString);
            if (clParams.ap==0)
                sprintf(pString, "affine projection order : %d\n", 1);
            else
                sprintf(pString, "affine projection order : %d\n", clParams.ap);
            break;
     case EC_FASTSUBBAND:
            ecParams.pInfo.params.algType = EC_FASTSUBBAND;
            sprintf(pString, "mode : fast subband \n");
            break;
     default: return UNKNOWN_FORMAT;
   }
    OutputInfoString(1, f_log, (const Ipp8s*)pString);
    switch(clParams.adapt) {
        case 0:
            sprintf(pString, "adaptation : disable \n");
            ecParams.pInfo.params.modes.adapt = AD_OFF;
            break;
        case 2:
            sprintf(pString, "adaptation : lite \n");
            ecParams.pInfo.params.modes.adapt = AD_LITEADAPT;
            break;
        default:
            ecParams.pInfo.params.modes.adapt = AD_FULLADAPT;
            sprintf(pString, "adaptation : full \n");
    }
   OutputInfoString(1, f_log, (const Ipp8s*)pString);
   sprintf(pString, "echo tail length : %d\n", clParams.tail);
   OutputInfoString(1, f_log, (const Ipp8s*)pString);
   if(clParams.puttocsv) {
    if(clParams.printSysInfo){
        sysInfoToCSV(f_csv);
    }
   }
   ecParams.pInfo.params.pcmType.sample_frequency = clParams.freq;
   ecParams.pInfo.params.pcmType.bitPerSample = 16;
   ecParams.pInfo.params.echotail = clParams.tail;
   ecParams.pInfo.params.modes.zeroCoeff = 1;///???
   ecParams.pInfo.params.modes.nlp = clParams.nlp;
   ecParams.pInfo.params.modes.cng = clParams.cng;
   ecParams.pInfo.params.modes.td = 1;
   ecParams.pInfo.params.modes.ah = clParams.ah_mode;
   ecParams.pInfo.params.modes.ap = clParams.ap;
   ecParams.pInfo.params.modes.nr = clParams.nr;
   ecParams.pInfo.params.modes.nr_smooth = clParams.nr_smooth;
   //ecParams.pInfo.params.modes.dcFlag = clParams.dcFlag;

   ecParams.pUSC_EC_Fxns->std.NumAlloc((const USC_EC_Option *)&ecParams.pInfo.params, &ecParams.nBanks);
   ecParams.pBanks = (USC_MemBank*)ippsMalloc_8u(sizeof(USC_MemBank)*ecParams.nBanks);
   if(!ecParams.pBanks) {
      sprintf(pString, "\nLow memory: %d bytes not allocated\n", (int)(sizeof(USC_MemBank)*ecParams.nBanks));
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(fp_rin) fclose(fp_rin);
      if(fp_sin) fclose(fp_sin);
      if(fp_sout) fclose(fp_sout);
      if(clParams.puttocsv) { if(f_csv) vm_file_fclose(f_csv); }
      if(f_log) fclose(f_log);
      return MEMORY_FAIL;
   }
   ecParams.pUSC_EC_Fxns->std.MemAlloc((const USC_EC_Option *)&ecParams.pInfo.params, ecParams.pBanks);
   for(i=0;i<ecParams.nBanks;i++) {
     ecParams.pBanks[i].pMem = (Ipp8s *)ippsMalloc_8u(ecParams.pBanks[i].nbytes);//375d60,377420
     if(!ecParams.pBanks[i].pMem) {
       sprintf(pString, "\nLow memory: %d bytes not allocated\n", ecParams.pBanks[i].nbytes);
       OutputInfoString(1, f_log, (const Ipp8s*)pString);
       if(ecParams.pBanks) ippsFree(ecParams.pBanks);
       if(fp_rin) fclose(fp_rin);
       if(fp_sin) fclose(fp_sin);
       if(fp_sout) fclose(fp_sout);
       if(clParams.puttocsv) { if(f_csv) vm_file_fclose(f_csv); }
       if(f_log) fclose(f_log);
       return MEMORY_FAIL;
     }
   }

   flen = PrepareInput(fp_rin, " receive-in input ", &in1_buff, f_log);
   in_len = PrepareInput(fp_sin, " send-in input ", &in2_buff, f_log);

   if(ecParams.pInfo.params.pcmType.sample_frequency == 8000){ /* 8 KHz */
      delay = (Ipp32s) (clParams.fdelay * 8000 * 2/1000);
   }else{ /* 16 KHz */
      delay = (Ipp32s) (clParams.fdelay * 16000 * 2/1000);
   }
   flen -= delay;
   if(flen < 0)  flen = 0;
   if (flen < in_len)
       in_len = flen;

   out_buff=(Ipp8u*)ippsMalloc_8u(in_len);/* allocate output buffer */
   if(!out_buff){ /* allocate output buffer */
      sprintf(pString, "\nNo memory for buffering of %d output bytes", in_len);
      OutputInfoString(1, f_log, (const Ipp8s*)pString);
      if(f_log) fclose(f_log);
      if(f_csv) fclose(f_csv);
      if(fp_rin) fclose(fp_rin);
      if(fp_sin) fclose(fp_sin);
      if(fp_sout) fclose(fp_sout);
      return MEMORY_FAIL;
   }

   /* time stamp prior to threads creation, creation and running time may overlap. */
   measure_start(&measure);

   n_repeat = clParams.nRepeat;
   while(n_repeat--) {
      USCStatus = ecParams.pUSC_EC_Fxns->std.Init((const USC_EC_Option *)&ecParams.pInfo.params, ecParams.pBanks, &ecParams.objEC);
      if(USCStatus!=USC_NoError) {
        OutputInfoString(1, f_log,"\nCan not initialize the EC object!");
        if(ecParams.pBanks) ippsFree(ecParams.pBanks);
        if(fp_rin) fclose(fp_rin);
        if(fp_sin) fclose(fp_sin);
        if(fp_sout) fclose(fp_sout);
        if(clParams.puttocsv) { if(f_csv) vm_file_fclose(f_csv); }
        if(f_log) fclose(f_log);
        return ERROR_INIT;
      }
      ecParams.pUSC_EC_Fxns->std.GetInfo(ecParams.objEC, (USC_EC_Info *)&ecParams.pInfo);
      frameNum = in_len/ecParams.pInfo.params.framesize;
      tailNum = (in_len/sizeof(Ipp16s)) - (ecParams.pInfo.params.framesize/sizeof(Ipp16s))*frameNum;

        out_buff_cur = out_buff;
        in1_buff_cur = in1_buff;
        in2_buff_cur = in2_buff+ delay; /* shift forward the sin at delay */
        for (i = 0; i < frameNum; i++) {
            ecParams.pUSC_EC_Fxns->CancelEcho(ecParams.objEC, (Ipp16s *)in2_buff_cur,
                (Ipp16s *)in1_buff_cur, (Ipp16s *)out_buff_cur);//,0.75
            in1_buff_cur += ecParams.pInfo.params.framesize;
            in2_buff_cur += ecParams.pInfo.params.framesize;
            out_buff_cur += ecParams.pInfo.params.framesize;
        }
        for (i = 0; i < tailNum; i++) {
            ippsZero_16s((Ipp16s *)out_buff_cur, tailNum);
        }
   }

   measure_end(&measure);

   if (PostProcessPCMstream((Ipp8s *)out_buff, in_len)) {
       sprintf(pString, "No memory for load of %d bytes convert from linear PCM to special pack value.",in_len);
       OutputInfoString(1, f_log, (const Ipp8s*)pString);
       if(f_log) fclose(f_log);
       if(f_csv) fclose(f_csv);
       if(fp_rin) fclose(fp_rin);
       if(fp_sin) fclose(fp_sin);
       if(fp_sout) fclose(fp_sout);
       return MEMORY_FAIL;
   }
   /* Write output PCM to the output file */
   fwrite(out_buff, 1, in_len, fp_sout);

   for(i=0; i<ecParams.nBanks;i++){
     if(ecParams.pBanks[i].pMem) ippsFree(ecParams.pBanks[i].pMem);
     ecParams.pBanks[i].pMem = NULL;
   }
   if(ecParams.pBanks) { ippsFree(ecParams.pBanks); ecParams.pBanks = NULL; }
   ippsFree(out_buff);

   speech_sec = (in_len / 2 * clParams.nRepeat)/(double)ecParams.pInfo.params.pcmType.sample_frequency;
   measure_output(f_log, &measure, speech_sec);
   sprintf(pString, "Done %d samples of %d Hz PCM wave file (%g sec)\n",
       (in_len>>1) * clParams.nRepeat, ecParams.pInfo.params.pcmType.sample_frequency, speech_sec);
   OutputInfoString(1, f_log, (const Ipp8s*)pString);

   if(clParams.puttocsv) {
       Ipp8s* rinFile;
       Ipp8s* sinFile;
       if ((rinFile = strrchr(clParams.rinFileName, '/')) != NULL) {
           rinFile += 1;
       } else if ((rinFile = strrchr(clParams.rinFileName, '\\')) != NULL) {
           rinFile += 1;
       } else
           rinFile = clParams.rinFileName;
       if ((sinFile = strrchr(clParams.sinFileName, '/')) != NULL) {
           sinFile += 1;
       } else if ((sinFile = strrchr(clParams.sinFileName, '\\')) != NULL) {
           sinFile += 1;
       } else
           sinFile = clParams.sinFileName;
       i=sprintf(pString, clParams.ECName);
       i += sprintf(pString + i,",");
       switch(clParams.alg) {
            case EC_FULLBAND:
                i += sprintf(pString + i,"fullband,");
                break;
            case EC_SUBBAND:
                i += sprintf(pString + i,"subband,");
                break;
            case EC_FASTSUBBAND:
                i += sprintf(pString + i,"fast subband,");
                break;
            default:
                i += sprintf(pString + i,"subband,");
       }
       switch(clParams.adapt) {
            case 2:
                i += sprintf(pString + i,"lite,");
                break;
            default:
                i += sprintf(pString + i,"full,");
       }
       i += sprintf(pString + i,"%d,",clParams.tail);
       i += sprintf(pString + i,"%d,%s,%s,%4.2f,%4.2f",
            ecParams.pInfo.params.pcmType.sample_frequency, rinFile, sinFile, speech_sec,
            measure.speed_in_mhz);
       vm_string_fprintf(f_csv,VM_STRING("%s\n"),pString);
       vm_file_fclose(f_csv);
   }

   ippsFree(in1_buff);
   ippsFree(in2_buff);

   fclose(fp_rin);
   fclose(fp_sin);
   fclose(fp_sout);

   OutputInfoString(1, f_log,"Completed !\n");
   if(f_log) fclose(f_log);
   return 0;
}

Ipp32s  PrepareInput(FILE* pFile, Ipp8s* comment, Ipp8u** pBuff, FILE *fptrLog)
{
   Ipp8s pString[MAX_LEN_STRING];
   Ipp32s lenFile;
   Ipp8u *buff = NULL;
   fseek(pFile,0,SEEK_END);
   lenFile = ftell(pFile); /* size of file*/
   buff=ippsMalloc_8u(lenFile);
   if(!buff){
      sprintf(pString, "\nNo memory to load %d bytes from %s file",lenFile,comment);
      OutputInfoString(1, fptrLog, (const Ipp8s*)pString);
      return MEMORY_FAIL;
   }
//   rewind(pFile);
   fseek(pFile,0,SEEK_SET);

   lenFile = (Ipp32s)fread(buff,1,lenFile,pFile);
   if (PreProcessBitstream((Ipp8s *)buff, lenFile)) {
       OutputInfoString(1, fptrLog,"\nUnknown format bitstream.\n");
       return UNKNOWN_FORMAT;
   }

   *pBuff = buff;
   return lenFile;
}
