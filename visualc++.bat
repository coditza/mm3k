@echo off
title mm3k build environment


REM *********************************************
REM set project specific settings here
REM *********************************************

SET wxbase=c:\wxWidgets-2.8.11_src\

REM this is for visual studio (probably)
REM call "C:\Program Files\Microsoft Visual Studio 9\VC\bin\vcvars32.bat"

REM this is for visual c++ express 2008
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

SET MS_DEVENV="C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\VCExpress.exe"

%MS_DEVENV% ".\build\win\rgj1_2008.sln"
