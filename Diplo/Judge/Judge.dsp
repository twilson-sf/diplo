# Microsoft Developer Studio Project File - Name="Judge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Judge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Judge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Judge.mak" CFG="Judge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Judge - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Judge - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Judge - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "EXPORTING" /D "AFX_RESOURCE_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0xc0c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	copy .\Release\Judge.dll ..\..\Output\Release\bin /v              	copy .\Release\Judge.lib ..\..\Output\lib /v	echo off
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Judge - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "EXPORTING" /D "AFX_RESOURCE_DLL" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/JudgeD.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	copy .\Debug\JudgeD.dll ..\..\Output\Debug\bin /v	copy          .\Debug\JudgeD.lib ..\..\Output\lib /v	copy Army.h ..\..\Output\include    /v               	copy Coast.h ..\..\Output\include /v   	copy Data.h ..\..\Output\include              /v	copy Fleet.h ..\..\Output\include    /v     	copy Game.h         ..\..\Output\include      /v	copy History.h ..\..\Output\include       /v	copy         Judge.h ..\..\Output\include /v       	copy mystruct.h  ..\..\Output\include              /v	copy Order.h ..\..\Output\include     /v   	copy  Power.h         ..\..\Output\include /v     	copy Province.h ..\..\Output\include  /v 	copy         PSFile.h  ..\..\Output\include /v     	copy  Step.h ..\..\Output\include            /v	copy Unit.h ..\..\Output\include  /v       	copy WhichCoast.h       ..\..\Output\include /v 	echo off
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Judge - Win32 Release"
# Name "Judge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Army.cpp
# End Source File
# Begin Source File

SOURCE=.\Coast.cpp
# End Source File
# Begin Source File

SOURCE=.\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Fleet.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\History.cpp
# End Source File
# Begin Source File

SOURCE=.\Judge.cpp
# End Source File
# Begin Source File

SOURCE=.\Judge.def
# End Source File
# Begin Source File

SOURCE=.\Judge.rc
# End Source File
# Begin Source File

SOURCE=.\Order.cpp
# End Source File
# Begin Source File

SOURCE=.\Power.cpp
# End Source File
# Begin Source File

SOURCE=.\Province.cpp
# End Source File
# Begin Source File

SOURCE=.\PSFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Step.cpp
# End Source File
# Begin Source File

SOURCE=.\Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\Whichcoast.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Army.h
# End Source File
# Begin Source File

SOURCE=.\Coast.h
# End Source File
# Begin Source File

SOURCE=.\Data.h
# End Source File
# Begin Source File

SOURCE=.\Fleet.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\History.h
# End Source File
# Begin Source File

SOURCE=.\Judge.h
# End Source File
# Begin Source File

SOURCE=.\mystruct.h
# End Source File
# Begin Source File

SOURCE=.\Order.h
# End Source File
# Begin Source File

SOURCE=.\Power.h
# End Source File
# Begin Source File

SOURCE=.\Province.h
# End Source File
# Begin Source File

SOURCE=.\PSFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Step.h
# End Source File
# Begin Source File

SOURCE=.\Unit.h
# End Source File
# Begin Source File

SOURCE=.\Whichcoast.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Judge.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=..\Structures.txt
# End Source File
# End Target
# End Project
