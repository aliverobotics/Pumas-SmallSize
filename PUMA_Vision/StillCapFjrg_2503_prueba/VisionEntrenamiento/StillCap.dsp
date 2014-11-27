# Microsoft Developer Studio Project File - Name="StillCap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=StillCap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StillCap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StillCap.mak" CFG="StillCap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StillCap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "StillCap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "StillCap - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE "StillCap - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StillCap - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "C:\DXSDK\Samples\C++\DirectShow\BaseClasses" /I "..\..\..\..\..\..\include" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DEBUG" /D "WIN32" /D "_DEBUG" /D _WIN32_WINNT=0x400 /FR"Debug/" /Fp"Debug/StillCap.pch" /Yu"stdafx.h" /Fo"Debug/" /Fd"Debug/" /FD /GZ /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 c:\dxsdk\lib\*.lib strmbasd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib soxmanager.lib /nologo /stack:0x200000,0x200000 /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /nodefaultlib:"olepro32" /OPT:NOREF /OPT:ICF /ignore:4089 /ignore:4078
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "C:\DXSDK\Samples\C++\DirectShow\BaseClasses" /I "..\..\..\..\..\..\include" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DEBUG" /D "WIN32" /D "_DEBUG" /D _WIN32_WINNT=0x400 /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 c:\dxsdk\lib\*.lib strmbasd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib WS2_32.LIB advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib soxmanager.lib /nologo /stack:0x200000,0x200000 /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /ignore:4089 /ignore:4078
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StillCap - Win32 Release Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "_WINDOWS" /D "_MBCS" /D "RELEASE" /D "_AFXDLL" /D "NDEBUG" /D _WIN32_WINNT=0x400 /D "WIN32" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\baseclasses\release_unicode\strmbase.lib winmm.lib /nologo /stack:0x200000,0x200000 /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /nodefaultlib:"olepro32" /OPT:NOREF /OPT:ICF /ignore:4089 /ignore:4078
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DEBUG" /D "_DEBUG" /D _WIN32_WINNT=0x400 /D "WIN32" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 c:\dxsdk\lib\*.lib strmbasd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /stack:0x200000,0x200000 /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"Debug/StillCap.exe" /ignore:4089 /ignore:4078
# SUBTRACT LINK32 /profile /nodefaultlib

!ENDIF 

# Begin Target

# Name "StillCap - Win32 Release"
# Name "StillCap - Win32 Debug"
# Name "StillCap - Win32 Release Unicode"
# Name "StillCap - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\archivos.cpp

!IF  "$(CFG)" == "StillCap - Win32 Release"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "StillCap - Win32 Release Unicode"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\colores.cpp

!IF  "$(CFG)" == "StillCap - Win32 Release"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "StillCap - Win32 Release Unicode"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Colors.cpp
# End Source File
# Begin Source File

SOURCE=.\DetectColors.cpp
# End Source File
# Begin Source File

SOURCE=.\detectRobots.cpp
# End Source File
# Begin Source File

SOURCE=.\distorsion.cpp

!IF  "$(CFG)" == "StillCap - Win32 Release"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "StillCap - Win32 Release Unicode"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\images.cpp

!IF  "$(CFG)" == "StillCap - Win32 Release"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "StillCap - Win32 Release Unicode"

!ELSEIF  "$(CFG)" == "StillCap - Win32 Debug Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LinkListBlob.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkListRle.cpp
# End Source File
# Begin Source File

SOURCE=.\msocket.cpp
# End Source File
# Begin Source File

SOURCE=.\myUdpSockets.cpp
# End Source File
# Begin Source File

SOURCE=.\Persistence.cpp
# End Source File
# Begin Source File

SOURCE=.\potentialFields.cpp
# End Source File
# Begin Source File

SOURCE=.\Socklib.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StillCap.cpp
# End Source File
# Begin Source File

SOURCE=.\StillCapDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\archivos.h
# End Source File
# Begin Source File

SOURCE=.\colores.h
# End Source File
# Begin Source File

SOURCE=.\colors.h
# End Source File
# Begin Source File

SOURCE=.\DetectColors.h
# End Source File
# Begin Source File

SOURCE=.\detectRobots.h
# End Source File
# Begin Source File

SOURCE=.\distorsion.h
# End Source File
# Begin Source File

SOURCE=.\images.h
# End Source File
# Begin Source File

SOURCE=.\LinkListBlob.h
# End Source File
# Begin Source File

SOURCE=.\LinkListRle.h
# End Source File
# Begin Source File

SOURCE=.\msocket.h
# End Source File
# Begin Source File

SOURCE=.\myUdpSockets.h
# End Source File
# Begin Source File

SOURCE=.\Persistence.h
# End Source File
# Begin Source File

SOURCE=.\POINT.H
# End Source File
# Begin Source File

SOURCE=.\potentialFields.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Socklib.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StillCap.h
# End Source File
# Begin Source File

SOURCE=.\StillCapDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\StillCap.ico
# End Source File
# Begin Source File

SOURCE=.\StillCap.rc
# End Source File
# Begin Source File

SOURCE=.\res\StillCap.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\Doxyfile
# End Source File
# End Target
# End Project
