;////////////////////////// cpuinfo_windows_intel64.asm //////////////////////////
;
;              INTEL CORPORATION PROPRIETARY INFORMATION
;  This software is supplied under the terms of a license  agreement or
;  nondisclosure agreement with Intel Corporation and may not be copied
;  or disclosed except in  accordance  with the terms of that agreement.
;     Copyright (c) 2012 Intel Corporation. All Rights Reserved.
;


.code

;####################################################################
;#          void ownGetReg( int* buf, int valueEAX, int valueECX ); #
;####################################################################

buf       equ rcx
valueEAX  equ rdx
valueECX  equ r8

ownGetReg    PROC PUBLIC
        push rbx
        push rsi
        movsxd  rax, edx
        mov     rsi, rcx
        movsxd  rcx, r8d
        xor     ebx, ebx
        xor     edx, edx
        cpuid
        mov     [rsi], eax
        mov     [rsi + 4], ebx
        mov     [rsi + 8], ecx
        mov     [rsi + 12], edx
        pop rsi
        pop rbx
        ret

ownGetReg ENDP

;###################################################

END
