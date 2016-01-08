# Microsoft Developer Studio Project File - Name="Diplo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Diplo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Diplo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Diplo.mak" CFG="Diplo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Diplo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Diplo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Diplo - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "__AFXCONV_H__" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0xc0c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Judge.lib OSC60a.lib OT60a.lib /nologo /subsystem:windows /incremental:yes /machine:I386
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=echo on	copy .\Release\Diplo.exe ..\Output\Setup /v                      	copy .\Judge\Release\Judge.dll ..\Output\Setup /v             	copy        .\res\Standard.jpg ..\Output\Setup /v             	copy        .\Help\Winhelp\Diplo.hlp ..\Output\Setup /v               	echo off
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Diplo - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__AFXCONV_H__" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 JudgeD.lib OSC60ad.lib OT60ad.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "Diplo - Win32 Release"
# Name "Diplo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BuildBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Diplo.cpp
# End Source File
# Begin Source File

SOURCE=.\Diplo.rc
# End Source File
# Begin Source File

SOURCE=.\DiploDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DiploTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DiploView.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryBar.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryTab.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLinkStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMiniFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\OrdersTab.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BuildBar.h
# End Source File
# Begin Source File

SOURCE=.\Diplo.h
# End Source File
# Begin Source File

SOURCE=.\DiploDoc.h
# End Source File
# Begin Source File

SOURCE=.\DiploTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DiploView.h
# End Source File
# Begin Source File

SOURCE=.\HistoryBar.h
# End Source File
# Begin Source File

SOURCE=.\HistoryTab.h
# End Source File
# Begin Source File

SOURCE=.\HTMLinkStatic.h
# End Source File
# Begin Source File

SOURCE=.\InfoTab.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyMiniFrame.h
# End Source File
# Begin Source File

SOURCE=.\OrdersTab.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Army_aus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_eng.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_fra.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_ger.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_ita.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_rus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Army_tur.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BuildBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Diplo.ico
# End Source File
# Begin Source File

SOURCE=.\res\Diplo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Diplo16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\diplo_li.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DiploDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Finger.cur
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_au.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_en.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_fr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_ge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_it.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_ru.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Fleet_tu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\history_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pslink_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Units.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ToDo.txt
# End Source File
# End Target
# End Project
