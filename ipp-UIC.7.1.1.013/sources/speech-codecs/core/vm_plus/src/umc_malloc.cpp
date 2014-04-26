/* /////////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTeL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#define VM_MALLOC_OWN
#include "umc_malloc.h"

#if defined VM_MALLOC
#include <string.h>
#include <stdio.h>

#define vm_args const char* lpcFileName, Ipp32u iStringNumber

#if defined _WIN32_WCE
    #define OutputDir _T("\\Hard Disk2\\umc_malloc")
#else
    #define OutputDir "C:"
#endif

TCHAR g_OutputFileName[256] = _T("");

class VM_MallocItem
{
public:
    void*       m_lpv;
    Ipp32s      m_size;
    char        m_lpcBinaryName[MAX_PATH];
    const char* m_lpcFileName;
    Ipp32u      m_iStringNumber;

    void Init(void* lpv, Ipp32s size, TCHAR* cBinaryName, vm_args)
    {
        m_lpv           = lpv;
        m_size          = size;

        if (cBinaryName)
        {
#ifdef _UNICODE
            wcstombs((char*)m_lpcBinaryName, cBinaryName, MAX_PATH);
#else
            strcpy((char*)m_lpcBinaryName, cBinaryName);
#endif
        }

        m_lpcFileName   = lpcFileName;
        m_iStringNumber = iStringNumber;

        if (!_tcscmp(g_OutputFileName, _T("")))
        {
            TCHAR*  pos = NULL;

            pos = _tcsrchr(cBinaryName, _T('\\'));

            _stprintf(g_OutputFileName, _T("%s%s.csv"), OutputDir, pos);

            ClearLog();
        }
    }

    static void ClearLog()
    {
        if(FILE *f = _tfopen(g_OutputFileName, VM_STRING("w")))
        {
            fprintf(f, "size, binary, source, string\n");
            fclose(f);
        }
    }

    void Report(Ipp32s size)
    {
        if(FILE *f = _tfopen(g_OutputFileName, VM_STRING("a")))
        {
            fprintf(f, "%d, %s, %s, %d\n",
                              size,
                    0 == m_lpcBinaryName ? "UnknownBinary" : m_lpcBinaryName,
                    0 == m_lpcFileName   ? "UnknownFile"   : m_lpcFileName,
                              m_iStringNumber);

            fclose(f);
        }
    }

    void ReportLeak()
    {
        if(FILE *f = _tfopen(g_OutputFileName, VM_STRING("a")))
        {
            fprintf(f, "Memory leak %d, %s, %s, %d\n",
                              m_size,
                    0 == m_lpcBinaryName ? "UnknownBinary" : m_lpcBinaryName,
                    0 == m_lpcFileName   ? "UnknownFile"   : m_lpcFileName,
                              m_iStringNumber);

            fclose(f);
        }
    }

    void ReportAlloc() { Report( m_size); }
    void ReportFree () { Report(-m_size); }
};

VM_MallocItemArray vm_malloc_array;

extern HINSTANCE g_hInst;

#define _vm_getBinaryName\
    TCHAR cBinaryName[MAX_PATH];\
    GetModuleFileName(g_hInst, cBinaryName, (sizeof cBinaryName)/(sizeof cBinaryName[0]));

#define _vm_addMallocItem\
    VM_MallocItem* item = (VM_MallocItem*) malloc(sizeof(VM_MallocItem));\
    item->Init(lpv, size, cBinaryName, lpcFileName, iStringNumber);\
    vm_malloc_array.AddItem(item);

#define _vm_deleteMallocItem(lpv)\
    vm_malloc_array.DeleteItem(lpv);

VM_MallocItemArray::VM_MallocItemArray()
{
    m_array             = 0;
    m_count             = 0;
    m_allocated         = 0;
    m_mem_usage_max     = 0;
    m_mem_usage_current = 0;
}

VM_MallocItemArray::~VM_MallocItemArray()
{
    char tmp[] = "Maximum usage";
    VM_MallocItem item;

    item.Init(NULL, m_mem_usage_max, TEXT("Maximum usage"), "", 0);
    item.ReportAlloc();

    for(Ipp32u i=0; i<m_count; i++)
    {
#if defined(VM_MALLOC_STATISTIC)
        if(m_array[i]->m_size > 0)
            m_array[i]->ReportLeak();
#endif
        delete m_array[i];
    }

    m_array             = 0;
    m_count             = 0;
    m_allocated         = 0;
    m_mem_usage_max     = 0;
    m_mem_usage_current = 0;
}

void VM_MallocItemArray::AddItem(VM_MallocItem* item)
{
    if(m_count == m_allocated)
    {
        m_allocated += 10;
        m_array = (VM_MallocItem**)realloc(m_array, m_allocated*sizeof(VM_MallocItem*));
    }

    m_array[m_count++] = item;
    vm_malloc_array.ChangeMemUsage(item->m_size);

#if defined(VM_MALLOC_STATISTIC)
    item->ReportAlloc();
#endif
}

void VM_MallocItemArray::DeleteItem(void* lpv)
{
    for(Ipp32u i=0; i<m_count; i++)
    {
        if(m_array[i]->m_lpv == lpv)
        {
#if defined(VM_MALLOC_STATISTIC)
            m_array[i]->ReportFree();
#endif
            vm_malloc_array.ChangeMemUsage(-Ipp32s(m_array[i]->m_size));
            delete m_array[i];
            m_array[i] = m_array[--m_count];
            return;
        }
    }
}

void VM_MallocItemArray::ChangeMemUsage(Ipp32s size)
{
    m_mem_usage_current += size;

    if(m_mem_usage_current > m_mem_usage_max)
        m_mem_usage_max = m_mem_usage_current;
}

void* vm_malloc(Ipp32s size, vm_args)
{
    void* lpv = malloc(size);
    _vm_getBinaryName
    _vm_addMallocItem;
    return lpv;
}

void* vm_calloc(size_t num, Ipp32s size, vm_args)
{
    void* lpv = calloc(num, size);
    size *= num;
    _vm_getBinaryName
    _vm_addMallocItem;
    return lpv;
}

void* vm_realloc(void *lpv, Ipp32s size, vm_args)
{
    void *lpv_old = lpv;

    lpv = realloc(lpv, size);

    _vm_getBinaryName
    _vm_addMallocItem;
    _vm_deleteMallocItem(lpv_old);

    return lpv;
}

void vm_free(void *lpv)
{
    _vm_deleteMallocItem(lpv);
    free(lpv);
}

#define _ippsMalloc(type)\
Ipp##type* vm_ippsMalloc_##type(Ipp32s size, vm_args)\
{\
    Ipp##type* lpv = ippsMalloc_##type(size);\
    _vm_getBinaryName\
    _vm_addMallocItem;\
    return lpv;\
}

_ippsMalloc(8u  )
_ippsMalloc(16u )
_ippsMalloc(32u )
_ippsMalloc(8s  )
_ippsMalloc(16s )
_ippsMalloc(32s )
_ippsMalloc(64s )
_ippsMalloc(32f )
_ippsMalloc(64f )
_ippsMalloc(8sc )
_ippsMalloc(16sc)
_ippsMalloc(32sc)
_ippsMalloc(64sc)
_ippsMalloc(32fc)
_ippsMalloc(64fc)

void vm_ippsFree(void *lpv)
{
    _vm_deleteMallocItem(lpv);
    ippsFree(lpv);
}
/*
void vm_malloc_measure(Ipp32u start, vm_args)
{
    static void* lpv = 0;
    Ipp32s       size;

    static MEMORYSTATUSEX  status_before = {0,};
    static MEMORYSTATUSEX  status_after  = {0,};

    status_before.dwLength = sizeof(status_before);
    status_after .dwLength = sizeof(status_after);

    if(start)
    {
        GlobalMemoryStatusEx(&status_before);
        lpv = (void*)lpcFileName;
    }

    if(!start)
    {
        GlobalMemoryStatusEx(&status_after);

        size = (Ipp32s)IPP_MAX(0, (Ipp32s)(status_before.ullAvailPageFile - status_after.ullAvailPageFile));

        _vm_addMallocItem;
        item->m_size = 0; // Disable memory leak reporting for this block.
    }
}
*/
void* __cdecl operator new(size_t size, vm_args)
{
    return vm_malloc(size, lpcFileName, iStringNumber);
}

void* __cdecl operator new[](size_t size, vm_args)
{
    return vm_malloc(size, lpcFileName, iStringNumber);
}

void __cdecl operator delete(void *lpv)
{
    vm_free(lpv);
}

void __cdecl operator delete(void *lpv, vm_args)
{
    vm_free(lpv);
}

void operator delete[](void *lpv)
{
    vm_free(lpv);
}

void operator delete[](void *lpv, vm_args)
{
    vm_free(lpv);
}

#endif
