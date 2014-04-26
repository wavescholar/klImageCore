;////////////////////////// cpuinfoa_windows_ia32.asm //////////////////////////
;
;              INTEL CORPORATION PROPRIETARY INFORMATION
;  This software is supplied under the terms of a license  agreement or
;  nondisclosure agreement with Intel Corporation and may not be copied
;  or disclosed except in  accordance  with the terms of that agreement.
;     Copyright (c) 2012 Intel Corporation. All Rights Reserved.
;

  .686
  .XMM
  .model FLAT, C

.code

;####################################################################
;#          void ownGetReg( int* buf, int valueEAX, int valueECX ); #
;####################################################################

buf       EQU 12[esp]
valueEAX  EQU 16[esp]
valueECX  EQU 20[esp]

ownGetReg PROC PUBLIC

        push    ebx
        push    esi
        mov     eax, valueEAX
        mov     ecx, valueECX
        xor     ebx, ebx
        xor     edx, edx
        mov     esi, buf
        cpuid
        mov     [esi], eax
        mov     [esi + 4], ebx
        mov     [esi + 8], ecx
        mov     [esi + 12], edx
        pop     esi
        pop     ebx
        ret

ownGetReg ENDP 

;###################################################

END
