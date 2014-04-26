@echo off
::
:: Copyright  (C) 1985-2012 Intel Corporation. All rights reserved.
::
:: The information and source code contained herein is the exclusive property
:: of Intel Corporation and may not be disclosed, examined, or reproduced in
:: whole or in part without explicit written authorization from the Company.
::


:: Cache some environment variables.
set IPPROOT=%~dp0
CALL :GetFullPath "%IPPROOT%\.." IPPROOT

set SCRIPT_NAME=%~nx0

:: Set the default arguments
set IPP_TARGET_ARCH=

:ParseArgs
:: Parse the incoming arguments
if /i "%1"==""        goto Build
if /i "%1"=="ia32"         (set IPP_TARGET_ARCH=ia32)    & shift & goto ParseArgs
if /i "%1"=="intel64"      (set IPP_TARGET_ARCH=intel64) & shift & goto ParseArgs
if /i "%1"=="vs2005"       shift & goto ParseArgs
if /i "%1"=="vs2008"       shift & goto ParseArgs
if /i "%1"=="vs2010"       shift & goto ParseArgs
if /i "%1"=="vs2011"       shift & goto ParseArgs
if /i "%1"=="vs11"         shift & goto ParseArgs
goto Error

:Build

:: target achitecture is mandatoty
if /i "%IPP_TARGET_ARCH%"=="" goto Syntax

:: main actions
set LIB=%IPPROOT%\lib\%IPP_TARGET_ARCH%;%IPPROOT%\..\compiler\lib\%IPP_TARGET_ARCH%;%LIB%
set PATH=%IPPROOT%\..\redist\%IPP_TARGET_ARCH%\ipp;%IPPROOT%\..\redist\%IPP_TARGET_ARCH%\compiler;%PATH%
set INCLUDE=%IPPROOT%\include;%INCLUDE%

goto End

:Error
echo Invalid command line argument: %1
echo.
exit /B 1

:Syntax
echo Syntax:
echo  %SCRIPT_NAME% ^<arch^>
echo.
echo    ^<arch^> must be is one of the following
echo        ia32         : Set up for IA-32 target
echo        intel64      : Set up for Intel(R) 64 target
echo.
exit /B 1

:End
exit /B 0


:GetFullPath
SET %2=%~f1
GOTO :eof

