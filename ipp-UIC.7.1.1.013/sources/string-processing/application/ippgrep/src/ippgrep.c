/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  th software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <stdio.h>
#include <string.h>
#include "ippcore.h"
#include "ipps.h"
#include "ippch.h"

#define OPTION_C 1
#define OPTION_F 2
#define OPTION_H 4
#define OPTION_I 8
#define OPTION_L 16
#define OPTION_N 32
#define OPTION_V 64
#define OPTION_X 128

#define LINE_SIZE 10 * 1024

static void find( Ipp8u* pLine, IppRegExpState* pRegExpState, int options, const char* fileName, FILE* stream )
{
  char         *separator = (fileName[0] != 0) ? ":" : "";
  IppRegExpFind pFind;
  int           option_x  = options & OPTION_X;
  int           option_v  = options & OPTION_V;
  int           option_c  = options & OPTION_C;
  int           option_l  = options & OPTION_L;
  int           option_n  = options & OPTION_N;
  int           numFind, lenLine, cnt, cntFind;
  IppStatus     status;

  /*
     Processing all lines from stream in series
  */
  for( cnt = cntFind = 0; !feof( stream ); cnt++ ) {
    if( NULL == fgets( (char*)pLine, LINE_SIZE, stream ) ) break;
    lenLine = (int)strlen( (char*)pLine );
    numFind = 1;
    /*
       As in ippgrep we search for matching with whole regular expression, we set numFind = 1
       and pFind[1], but function ippsRegExpFind_8u can return up to 999 pointers to the substrings
       that correspond to groupings in regular expression
    */
    status = ippsRegExpFind_8u( pLine, lenLine, pRegExpState, &pFind, &numFind );
    /*
       If function returns status other than ippStsNoErr or ippStsRegExpMatchLimitErr then
       subsequent processing is impossible
    */
    if( (ippStsNoErr != status) && (ippStsRegExpMatchLimitErr != status) ) break;
    /*
       If the option 'x' is set, but matching doesn't correspond to whole line then
       we suppose that the string isn't found
    */
    if( option_x && numFind && (pFind.lenFind != lenLine) ) numFind = 0;
    /*
       If the option 'v' is set, then result of comparing is inverted
    */
    if( (option_v && !numFind) || (!option_v && numFind) ) {
      cntFind++;
      /*
         If the option 'c' is set, then normal output is suppressed
      */
      if( !option_c && !option_l ) {
        fprintf( stdout, "%s%s", fileName, separator );
        if( option_n ) fprintf( stdout, "%d:", cnt );
        fprintf( stdout, "%s", pLine );
        if( '\n' != pLine[lenLine - 1] ) fprintf( stdout, "\n" );
      } /* if */
    } /* if */
  } /* for */
  /*
     If the option 'c' is set then the number of matching lines is printed
  */
  if( option_c ) fprintf( stdout, "%s%s%d\n", fileName, separator, cntFind );
  if( option_l && cntFind ) fprintf( stdout, "%s\n", fileName );
} /* find */

int main( int argC, char** argV ) {
  char **ppFNames;              /* Array of pointers to arguments of command line containing filenames             */
  char *pLine;                  /* The buffer for working with streams (for example, search procedure)             */
  char *pPattern;               /* Pointer to the name of file with pattern or the string with pattern             */
  IppRegExpState* pRegExpState; /* Pointer to the structure containing internal form of a regular expression       */
  int  fNCount  = 0;            /* Size of the array ppFNames                                                      */
  int  fNIndx   = 0;            /* Current index of the array ppFNames                                             */
  int  option_s = 0;            /* Mode of error message suppression                                               */
  int  options  = 0;            /* Search options read-in from command line. For every option there is certain bit */
  int  i, j;                    /* Indexes                                                                         */
  int errOffset;                /* Offset into the pattern if compiling is break                                   */
  IppStatus status;

  ippStaticInit();
  /*
     Simultaneous memory allocation both for the array ppFNames and the buffer pLine
  */
  ppFNames     = (char**)ippsMalloc_8u( sizeof(char*) * (argC - 1) + sizeof(char) * LINE_SIZE );
  pLine        = (char*)(ppFNames + (argC - 1));
  pRegExpState = NULL;
  pPattern     = NULL;
  if( NULL == ppFNames ) {
    if( !option_s ) fprintf( stderr, "Not enough memory.\n" );
    return 1;
  } /* if */
  if( argC > 1 ) {
    /*
       Parsing of command line. We've simple rule - all arguments of command line that have
       first symbol '-' contain options, others contain filenames or the pattern
    */
    for( i = 1; i < argC; i++ ) {
      if( argV[i][0] == '-' ) {
        for( j = 1; j < strchr( argV[i], 0 ) - argV[i]; j++ ) {
          /*
             For option 'f' individual parsing
          */
          if( argV[i][j] == 'f' ) {
            options |= OPTION_F;
            /*
               If we meet the option 'f' then the remainder of argument is a name of the file with pattern,
               but if the option 'f' is last in the argument then a name of the file with pattern is
               the whole next agument. After that the pPattern points to file with pattern.
            */
            if( 0 != argV[i][j + 1] ) {
              pPattern = argV[i] + j + 1;
            } /* if */
            else {
              if( i < (argC - 1) ) {
                pPattern = argV[++i];
              } /* if */
              else {
                if( !option_s ) fprintf( stderr, "The filename with pattern is absent.\n" );
                ippsFree( ppFNames );
                return 1;
              } /* else */
            } /* else */
            break;
          } /* if */
          /*
             Parsing all other options
          */
          switch( argV[i][j] ) {
          case 'V':
            { /* block */
              const IppLibraryVersion *pLibVersion = ippchGetLibVersion();
              fprintf( stdout, "Ippgrep is based on %s version %s [of the %s].\n", pLibVersion->Name, pLibVersion->Version, pLibVersion->BuildDate );
            } /* block */
            ippsFree( ppFNames );
            return 0;
          case 'c':
            options |= OPTION_C;
            break;
          case 'h':
            options |= OPTION_H;
            break;
          case 'i':
            options |= OPTION_I;
            break;
          case 'l':
            options |= OPTION_L;
            break;
          case 'n':
            options |= OPTION_N;
            break;
          case 's':
            option_s = 1;
            break;
          case 'v':
            options |= OPTION_V;
            break;
          case 'x':
            options |= OPTION_X;
            break;
          default:
            if( !option_s ) fprintf( stderr, "Undefined option -%c.\n", argV[i][j] );
            break;
         } /* switch */
        } /* for */
      } /* if */
      else {
        /*
           All filenames are put in the array ppFNames
        */
        ppFNames[fNCount++] = argV[i];
      } /* else */
    } /* for */
    if( options & OPTION_F ) {
      /*
         Opening file with pattern
      */
      FILE *pfile = fopen( pPattern, "r" );
      if( NULL != pfile ) {
        /*
           Ippgrep doesn't support the option 'x' of function ippsRegExpInitAlloc and pattern occupies only one string.
        */
        if( NULL == fgets( pLine, LINE_SIZE, pfile ) ) {
          if( !option_s ) fprintf( stderr, "Can't read pattern from file %s.\n", pPattern );
          ippsFree( ppFNames );
          return 1;
        } /* if */
        /*
           After that the pPattern points to pattern
        */
        pPattern = pLine;
        fclose( pfile );
      } /* if */
      else {
        if( !option_s ) fprintf( stderr, "Can't open file %s.\n", pPattern );
        ippsFree( ppFNames );
        return 1;
      } /* else */
    } /* if */
    else {
      /*
         If the option 'f' isn't set, then first element of the array ppFNames points to pattern
      */
      pPattern = ppFNames[fNIndx++];
    } /* else */
    if( NULL == pPattern ) {
      fprintf( stderr, "The pattern is absent.\n" );
      ippsFree( ppFNames );
      return 1;
    } /* if */
    /*
       Removing all meaningless spaces at the end of the line
    */
    for( i = (int)strlen( pPattern ); i > 0; i-- ) if( pPattern[i - 1] != ' ' ) break;
    pPattern[i] = 0;
    /*
       Compiling of pattern
    */
    status = ippsRegExpInitAlloc( pPattern, (options & OPTION_I) ? "i" : "", &pRegExpState, &errOffset );
    if( ippStsNoErr != status ) {
      if( !option_s ) fprintf( stderr, "Can't compile pattern. %s in offset %d.\n", ippGetStatusString( status ), errOffset );
      ippsFree( ppFNames );
      return 1;
    } /* if */
    /*
       Limitation of value of the matches that are kept in stack for the avoid of stack overflow
    */
    ippsRegExpSetMatchLimit( LINE_SIZE, pRegExpState );
    /*
       When the array ppFNames is empty then data for comparring are taken from stdin
    */
    if( fNIndx >= fNCount ) find( (Ipp8u*)pLine, pRegExpState, options, "", stdin );
    /*
       If the array ppFNames contains only one filename for comparing, then the option 'h' is set forced
    */
    if( (fNCount - fNIndx) == 1 ) options |= OPTION_H;
    /*
       Processing all files from the array ppFNames in series
    */
    for( ; fNIndx < fNCount; fNIndx++ ) {
      FILE *stream = fopen( ppFNames[fNIndx], "r" );
      if( NULL == stream ) {
        if( !option_s ) fprintf( stderr, "Can't open file %s.\n", ppFNames[fNIndx] );
        continue;
      } /* if */
      if( options & OPTION_H ) ppFNames[fNIndx] = "";
      find( (Ipp8u*)pLine, pRegExpState, options, ppFNames[fNIndx], stream );
      fclose( stream );
    } /* for */
  } /* if */
  else {
    fprintf( stdout, "Usage: ippgrep [OPTIONS] PATTERN [FILES...]\n" );
    fprintf( stdout, "\nOPTIONS:\n" );
    fprintf( stdout, "-f file Read the pattern from file.\n" );
    fprintf( stdout, "-V      Print information about base ippch library.\n" );
    fprintf( stdout, "-c      Suppress normal output; instead print a count of matching lines for each\n" );
    fprintf( stdout, "        input file. With the -v option - count nonmatching lines.\n" );
    fprintf( stdout, "-l      Suppress normal output; instead print the name of each input file from\n" );
    fprintf( stdout, "        which output would normally have been printed.\n" );
    fprintf( stdout, "-h      Suppress the prefixing of filenames on output when multiple files are\n" );
    fprintf( stdout, "        searched.\n" );
    fprintf( stdout, "-i      Ignore case distinctions.\n" );
    fprintf( stdout, "-n      Prefix each line of output with the line number within its input file.\n" );
    fprintf( stdout, "-s      Suppress error messages.\n" );
    fprintf( stdout, "-v      Select non-matching lines.\n" );
    fprintf( stdout, "-x      Select only those matches that exactly match the whole line.\n" );
  } /* else */
  ippsRegExpFree( pRegExpState );
  ippsFree( ppFNames );
  return 0;
} /* main */

